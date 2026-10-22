// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#அந்தர்பாவ "precomp.hpp"

நாமாகாஶ cv { நாமாகாஶ cuda {

#ifndef HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    திரும்பு வெற்று;
}

#இல்லையெனில்

வகுப்பு CudaUMatAllocator CV_FINAL : பொது MatAllocator
{
பொது:
    UMatData* allocate(முழு dims, மாறிலி முழு* sizes, முழு type, வெறுமை* data0,
                       size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) மாறிலி CV_OVERRIDE
    {
        மாறிலி size_t elemSize = CV_ELEM_SIZE(type);
        size_t total = elemSize;
        க்கு (முழு i = 0; i < dims; i++)
            total *= (size_t)sizes[i];

        எனில் (step)
        {
            step[dims - 1] = elemSize;
            க்கு (முழு i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (size_t)sizes[i + 1];
        }

        UMatData* u = புதிய UMatData(இது);
        u->size = total;

        எனில் (total > 0)
            cudaSafeCall(cudaMalloc(&u->handle, total));

        எனில் (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
            u->markDeviceCopyObsolete(உண்மை);
        }
        இல்லையெனில்
        {
            u->markHostCopyObsolete(உண்மை);
        }
        திரும்பு u;
    }

    தருக்க allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) மாறிலி CV_OVERRIDE
    {
        எனில் (!u)
            திரும்பு பொய்;
        எனில் (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            எனில் (u->data)
                u->markDeviceCopyObsolete(உண்மை);
            இல்லையெனில்
                u->markHostCopyObsolete(உண்மை);
        }
        திரும்பு உண்மை;
    }

    வெறுமை deallocate(UMatData* u) மாறிலி CV_OVERRIDE
    {
        எனில் (!u)
            திரும்பு;
        எனில் (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        எனில் (u->data && !(u->flags & UMatData::USER_ALLOCATED))
            fastFree(u->data);
        delete u;
    }

    வெறுமை map(UMatData* u, AccessFlag accessFlags) மாறிலி CV_OVERRIDE
    {
        எனில் (!u)
            திரும்பு;
        எனில் (!u->data && u->size > 0)
            u->data = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        எனில் (u->hostCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->data, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(பொய்);
        }
        எனில் (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(உண்மை);
    }

    வெறுமை unmap(UMatData* u) மாறிலி CV_OVERRIDE
    {
        எனில் (!u)
            திரும்பு;
        எனில் (u->deviceCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->data, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(பொய்);
        }
    }

    வெறுமை download(UMatData* u, வெறுமை* dstptr, முழு dims, மாறிலி size_t sz[],
                  மாறிலி size_t srcofs[], மாறிலி size_t srcstep[],
                  மாறிலி size_t dststep[]) மாறிலி CV_OVERRIDE
    {
        எனில் (!u || !u->handle || !dstptr)
            திரும்பு;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    வெறுமை upload(UMatData* u, மாறிலி வெறுமை* srcptr, முழு dims, மாறிலி size_t sz[],
                மாறிலி size_t dstofs[], மாறிலி size_t dststep[],
                மாறிலி size_t srcstep[]) மாறிலி CV_OVERRIDE
    {
        எனில் (!u || !srcptr)
            திரும்பு;
        எனில் (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, வெற்று, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(உண்மை);
        u->markDeviceCopyObsolete(பொய்);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    வெறுமை copy(UMatData* src, UMatData* dst, முழு dims, மாறிலி size_t sz[],
              மாறிலி size_t srcofs[], மாறிலி size_t srcstep[],
              மாறிலி size_t dstofs[], மாறிலி size_t dststep[], தருக்க /*sync*/) மாறிலி CV_OVERRIDE
    {
        எனில் (!src || !dst || !src->handle || !dst->handle)
            திரும்பு;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(உண்மை);
        dst->markDeviceCopyObsolete(பொய்);
    }

தனிப்பட்ட:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    நிலையான வெறுமை copyPlanes(uchar* srcbase, மாறிலி size_t srcofs[], மாறிலி size_t srcstep[],
                           uchar* dstbase, மாறிலி size_t dstofs[], மாறிலி size_t dststep[],
                           முழு dims, மாறிலி size_t sz[], cudaMemcpyKind kind)
    {
        முழு isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        க்கு (முழு i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)INT_MAX);
            எனில் (sz[i] == 0)
                திரும்பு;
            எனில் (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            எனில் (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (முழு)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        மாறிலி Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        மாறிலி size_t planesz = it.size;
        க்கு (size_t j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, புதிய CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
