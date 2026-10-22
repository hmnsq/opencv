// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#ضمن "precomp.hpp"

نطاق cv { نطاق cuda {

#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    أرجع عدم;
}

#وإلا

صنف CudaUMatAllocator CV_FINAL : عام MatAllocator
{
عام:
    UMat_معلومات* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type, فراغ* data0,
                       size_t* قدم, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        ثابت size_t elem_سائيز = CV_ELEM_سائيز(type);
        size_t ڪل = elem_سائيز;
        لكل (صحيح i = 0; i < dims; i++)
            ڪل *= (size_t)sizes[i];

        إذا (قدم)
        {
            قدم[dims - 1] = elem_سائيز;
            لكل (صحيح i = dims - 2; i >= 0; i--)
                قدم[i] = قدم[i + 1] * (size_t)sizes[i + 1];
        }

        UMat_معلومات* u = جديد UMat_معلومات(هذا);
        u->size = ڪل;

        إذا (ڪل > 0)
            cuda_محفوظ_Call(cudaMalloc(&u->handle, ڪل));

        إذا (data0)
        {
            u->معلومات = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_معلومات::USER_ALLOCATED;
            u->markDeviceCopyObsolete(سچ);
        }
        وإلا
        {
            u->markHostCopyObsolete(سچ);
        }
        أرجع u;
    }

    منطقي allocate(UMat_معلومات* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        إذا (!u)
            أرجع خطأ;
        إذا (!u->handle && u->size > 0)
        {
            cuda_محفوظ_Call(cudaMalloc(&u->handle, u->size));
            إذا (u->معلومات)
                u->markDeviceCopyObsolete(سچ);
            وإلا
                u->markHostCopyObsolete(سچ);
        }
        أرجع سچ;
    }

    فراغ deallocate(UMat_معلومات* u) ثابت CV_OVERRIDE
    {
        إذا (!u)
            أرجع;
        إذا (u->handle)
            cuda_محفوظ_Call(cudaFree(u->handle));
        إذا (u->معلومات && !(u->flags & UMat_معلومات::USER_ALLOCATED))
            fastFree(u->معلومات);
        ميٽيو u;
    }

    فراغ map(UMat_معلومات* u, AccessFlag accessFlags) ثابت CV_OVERRIDE
    {
        إذا (!u)
            أرجع;
        إذا (!u->معلومات && u->size > 0)
            u->معلومات = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        إذا (u->hostCopyObsolete() && u->handle && u->معلومات)
        {
            cuda_محفوظ_Call(cudaMemcpy(u->معلومات, u->handle, u->size, cuda_Memcpy_Device_منزل_Host));
            u->markHostCopyObsolete(خطأ);
        }
        إذا (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(سچ);
    }

    فراغ unmap(UMat_معلومات* u) ثابت CV_OVERRIDE
    {
        إذا (!u)
            أرجع;
        إذا (u->deviceCopyObsolete() && u->handle && u->معلومات)
        {
            cuda_محفوظ_Call(cudaMemcpy(u->handle, u->معلومات, u->size, cuda_Memcpy_Host_منزل_Device));
            u->markDeviceCopyObsolete(خطأ);
        }
    }

    فراغ download(UMat_معلومات* u, فراغ* dstptr, صحيح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dststep[]) ثابت CV_OVERRIDE
    {
        إذا (!u || !u->handle || !dstptr)
            أرجع;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cuda_Memcpy_Device_منزل_Host);
    }

    فراغ upload(UMat_معلومات* u, ثابت فراغ* srcptr, صحيح dims, ثابت size_t sz[],
                ثابت size_t dstofs[], ثابت size_t dststep[],
                ثابت size_t srcstep[]) ثابت CV_OVERRIDE
    {
        إذا (!u || !srcptr)
            أرجع;
        إذا (!u->handle && u->size > 0)
            cuda_محفوظ_Call(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, عدم, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cuda_Memcpy_Host_منزل_Device);
        u->markHostCopyObsolete(سچ);
        u->markDeviceCopyObsolete(خطأ);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    فراغ copy(UMat_معلومات* src, UMat_معلومات* dst, صحيح dims, ثابت size_t sz[],
              ثابت size_t srcofs[], ثابت size_t srcstep[],
              ثابت size_t dstofs[], ثابت size_t dststep[], منطقي /*sync*/) ثابت CV_OVERRIDE
    {
        إذا (!src || !dst || !src->handle || !dst->handle)
            أرجع;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cuda_Memcpy_Device_منزل_Device);
        dst->markHostCopyObsolete(سچ);
        dst->markDeviceCopyObsolete(خطأ);
    }

خاص:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    ساكن فراغ copyPlanes(uchar* srcbase, ثابت size_t srcofs[], ثابت size_t srcstep[],
                           uchar* dstbase, ثابت size_t dstofs[], ثابت size_t dststep[],
                           صحيح dims, ثابت size_t sz[], cuda_Memcpy_قسم قسم)
    {
        صحيح isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        لكل (صحيح i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)وڌ_کان_وڌ_عدد);
            إذا (sz[i] == 0)
                أرجع;
            إذا (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            إذا (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (صحيح)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        ثابت Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        ثابت size_t planesz = it.size;
        لكل (size_t j = 0; j < it.nplanes; j++, ++it)
            cuda_محفوظ_Call(cudaMemcpy(ptrs[1], ptrs[0], planesz, قسم));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, جديد CudaUMatAllocator())
}

#جيڪڏهن_پورو

}} // namespace cv { namespace cuda {
