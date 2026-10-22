// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#شامل "precomp.hpp"

نامخلا cv { نامخلا cuda {

#اگر_غیر_تعریف_شدہ HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    واپس تہی;
}

#ورنہ

جماعت CudaUMatAllocator CV_FINAL : عوامی MatAllocator
{
عوامی:
    UMat_ڈیٹا* allocate(صحیح dims, ثابت صحیح* sizes, صحیح type, خالی* data0,
                       size_t* قدم, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        ثابت size_t elem_سائز = CV_ELEM_سائز(type);
        size_t کل = elem_سائز;
        برائے (صحیح i = 0; i < dims; i++)
            کل *= (size_t)sizes[i];

        اگر (قدم)
        {
            قدم[dims - 1] = elem_سائز;
            برائے (صحیح i = dims - 2; i >= 0; i--)
                قدم[i] = قدم[i + 1] * (size_t)sizes[i + 1];
        }

        UMat_ڈیٹا* u = نیا UMat_ڈیٹا(یہ);
        u->size = کل;

        اگر (کل > 0)
            cuda_محفوظ_Call(cudaMalloc(&u->handle, کل));

        اگر (data0)
        {
            u->ڈیٹا = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_ڈیٹا::USER_ALLOCATED;
            u->markDeviceCopyObsolete(سچ);
        }
        ورنہ
        {
            u->markHostCopyObsolete(سچ);
        }
        واپس u;
    }

    منطقی allocate(UMat_ڈیٹا* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        اگر (!u)
            واپس جھوٹ;
        اگر (!u->handle && u->size > 0)
        {
            cuda_محفوظ_Call(cudaMalloc(&u->handle, u->size));
            اگر (u->ڈیٹا)
                u->markDeviceCopyObsolete(سچ);
            ورنہ
                u->markHostCopyObsolete(سچ);
        }
        واپس سچ;
    }

    خالی deallocate(UMat_ڈیٹا* u) ثابت CV_OVERRIDE
    {
        اگر (!u)
            واپس;
        اگر (u->handle)
            cuda_محفوظ_Call(cudaFree(u->handle));
        اگر (u->ڈیٹا && !(u->flags & UMat_ڈیٹا::USER_ALLOCATED))
            fastFree(u->ڈیٹا);
        حذف u;
    }

    خالی map(UMat_ڈیٹا* u, AccessFlag accessFlags) ثابت CV_OVERRIDE
    {
        اگر (!u)
            واپس;
        اگر (!u->ڈیٹا && u->size > 0)
            u->ڈیٹا = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        اگر (u->hostCopyObsolete() && u->handle && u->ڈیٹا)
        {
            cuda_محفوظ_Call(cudaMemcpy(u->ڈیٹا, u->handle, u->size, cuda_Memcpy_Device_مقصد_Host));
            u->markHostCopyObsolete(جھوٹ);
        }
        اگر (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(سچ);
    }

    خالی unmap(UMat_ڈیٹا* u) ثابت CV_OVERRIDE
    {
        اگر (!u)
            واپس;
        اگر (u->deviceCopyObsolete() && u->handle && u->ڈیٹا)
        {
            cuda_محفوظ_Call(cudaMemcpy(u->handle, u->ڈیٹا, u->size, cuda_Memcpy_Host_مقصد_Device));
            u->markDeviceCopyObsolete(جھوٹ);
        }
    }

    خالی download(UMat_ڈیٹا* u, خالی* dstptr, صحیح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dststep[]) ثابت CV_OVERRIDE
    {
        اگر (!u || !u->handle || !dstptr)
            واپس;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cuda_Memcpy_Device_مقصد_Host);
    }

    خالی upload(UMat_ڈیٹا* u, ثابت خالی* srcptr, صحیح dims, ثابت size_t sz[],
                ثابت size_t dstofs[], ثابت size_t dststep[],
                ثابت size_t srcstep[]) ثابت CV_OVERRIDE
    {
        اگر (!u || !srcptr)
            واپس;
        اگر (!u->handle && u->size > 0)
            cuda_محفوظ_Call(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, تہی, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cuda_Memcpy_Host_مقصد_Device);
        u->markHostCopyObsolete(سچ);
        u->markDeviceCopyObsolete(جھوٹ);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    خالی copy(UMat_ڈیٹا* src, UMat_ڈیٹا* dst, صحیح dims, ثابت size_t sz[],
              ثابت size_t srcofs[], ثابت size_t srcstep[],
              ثابت size_t dstofs[], ثابت size_t dststep[], منطقی /*sync*/) ثابت CV_OVERRIDE
    {
        اگر (!src || !dst || !src->handle || !dst->handle)
            واپس;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cuda_Memcpy_Device_مقصد_Device);
        dst->markHostCopyObsolete(سچ);
        dst->markDeviceCopyObsolete(جھوٹ);
    }

نجی:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    ساکن خالی copyPlanes(uchar* srcbase, ثابت size_t srcofs[], ثابت size_t srcstep[],
                           uchar* dstbase, ثابت size_t dstofs[], ثابت size_t dststep[],
                           صحیح dims, ثابت size_t sz[], cuda_Memcpy_قسم قسم)
    {
        صحیح isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        برائے (صحیح i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)زیادہ_سے_زیادہ_عدد);
            اگر (sz[i] == 0)
                واپس;
            اگر (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            اگر (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (صحیح)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        ثابت Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        ثابت size_t planesz = it.size;
        برائے (size_t j = 0; j < it.nplanes; j++, ++it)
            cuda_محفوظ_Call(cudaMemcpy(ptrs[1], ptrs[0], planesz, قسم));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, نیا CudaUMatAllocator())
}

#اگر_ختم

}} // namespace cv { namespace cuda {
