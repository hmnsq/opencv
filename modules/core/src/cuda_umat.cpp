// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#شامل "precomp.hpp"

فضاینام cv { فضاینام cuda {

#ifndef HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    بازگشت پوچ;
}

#وگرنه

رده CudaUMatAllocator CV_FINAL : عمومی MatAllocator
{
عمومی:
    UMatData* allocate(صحیح dims, ثابت صحیح* sizes, صحیح type, تهی* data0,
                       size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        ثابت size_t elemSize = CV_ELEM_SIZE(type);
        size_t total = elemSize;
        برای (صحیح i = 0; i < dims; i++)
            total *= (size_t)sizes[i];

        اگر (step)
        {
            step[dims - 1] = elemSize;
            برای (صحیح i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (size_t)sizes[i + 1];
        }

        UMatData* u = جدید UMatData(این);
        u->size = total;

        اگر (total > 0)
            cudaSafeCall(cudaMalloc(&u->handle, total));

        اگر (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
            u->markDeviceCopyObsolete(درست);
        }
        وگرنه
        {
            u->markHostCopyObsolete(درست);
        }
        بازگشت u;
    }

    منطقی allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        اگر (!u)
            بازگشت نادرست;
        اگر (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            اگر (u->data)
                u->markDeviceCopyObsolete(درست);
            وگرنه
                u->markHostCopyObsolete(درست);
        }
        بازگشت درست;
    }

    تهی deallocate(UMatData* u) ثابت CV_OVERRIDE
    {
        اگر (!u)
            بازگشت;
        اگر (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        اگر (u->data && !(u->flags & UMatData::USER_ALLOCATED))
            fastFree(u->data);
        delete u;
    }

    تهی map(UMatData* u, AccessFlag accessFlags) ثابت CV_OVERRIDE
    {
        اگر (!u)
            بازگشت;
        اگر (!u->data && u->size > 0)
            u->data = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        اگر (u->hostCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->data, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(نادرست);
        }
        اگر (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(درست);
    }

    تهی unmap(UMatData* u) ثابت CV_OVERRIDE
    {
        اگر (!u)
            بازگشت;
        اگر (u->deviceCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->data, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(نادرست);
        }
    }

    تهی download(UMatData* u, تهی* dstptr, صحیح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dststep[]) ثابت CV_OVERRIDE
    {
        اگر (!u || !u->handle || !dstptr)
            بازگشت;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    تهی upload(UMatData* u, ثابت تهی* srcptr, صحیح dims, ثابت size_t sz[],
                ثابت size_t dstofs[], ثابت size_t dststep[],
                ثابت size_t srcstep[]) ثابت CV_OVERRIDE
    {
        اگر (!u || !srcptr)
            بازگشت;
        اگر (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, پوچ, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(درست);
        u->markDeviceCopyObsolete(نادرست);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    تهی copy(UMatData* src, UMatData* dst, صحیح dims, ثابت size_t sz[],
              ثابت size_t srcofs[], ثابت size_t srcstep[],
              ثابت size_t dstofs[], ثابت size_t dststep[], منطقی /*sync*/) ثابت CV_OVERRIDE
    {
        اگر (!src || !dst || !src->handle || !dst->handle)
            بازگشت;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(درست);
        dst->markDeviceCopyObsolete(نادرست);
    }

خصوصی:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    ایستا تهی copyPlanes(uchar* srcbase, ثابت size_t srcofs[], ثابت size_t srcstep[],
                           uchar* dstbase, ثابت size_t dstofs[], ثابت size_t dststep[],
                           صحیح dims, ثابت size_t sz[], cudaMemcpyKind kind)
    {
        صحیح isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        برای (صحیح i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)INT_MAX);
            اگر (sz[i] == 0)
                بازگشت;
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
        برای (size_t j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, جدید CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
