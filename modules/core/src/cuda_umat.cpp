// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

ନାମାକାଶ cv { ନାମାକାଶ cuda {

#ifndef HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    ଫେରନ୍ତୁ ଖାଲି;
}

#ଅନ୍ୟଥା

ଶ୍ରେଣୀ CudaUMatAllocator CV_FINAL : ସର୍ବସାଧାରଣ MatAllocator
{
ସର୍ବସାଧାରଣ:
    UMatData* allocate(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ଶୂନ୍ୟ* data0,
                       size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ଅଚଳ CV_OVERRIDE
    {
        ଅଚଳ size_t elemSize = CV_ELEM_SIZE(type);
        size_t total = elemSize;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
            total *= (size_t)sizes[i];

        ଯଦି (step)
        {
            step[dims - 1] = elemSize;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (size_t)sizes[i + 1];
        }

        UMatData* u = ନୂତନ UMatData(ଏହି);
        u->size = total;

        ଯଦି (total > 0)
            cudaSafeCall(cudaMalloc(&u->handle, total));

        ଯଦି (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
            u->markDeviceCopyObsolete(ସତ୍ୟ);
        }
        ଅନ୍ୟଥା
        {
            u->markHostCopyObsolete(ସତ୍ୟ);
        }
        ଫେରନ୍ତୁ u;
    }

    ତାର୍କିକ allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି (!u)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଯଦି (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            ଯଦି (u->data)
                u->markDeviceCopyObsolete(ସତ୍ୟ);
            ଅନ୍ୟଥା
                u->markHostCopyObsolete(ସତ୍ୟ);
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ଶୂନ୍ୟ deallocate(UMatData* u) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି (!u)
            ଫେରନ୍ତୁ;
        ଯଦି (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        ଯଦି (u->data && !(u->flags & UMatData::USER_ALLOCATED))
            fastFree(u->data);
        delete u;
    }

    ଶୂନ୍ୟ map(UMatData* u, AccessFlag accessFlags) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି (!u)
            ଫେରନ୍ତୁ;
        ଯଦି (!u->data && u->size > 0)
            u->data = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        ଯଦି (u->hostCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->data, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(ମିଥ୍ୟା);
        }
        ଯଦି (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(ସତ୍ୟ);
    }

    ଶୂନ୍ୟ unmap(UMatData* u) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି (!u)
            ଫେରନ୍ତୁ;
        ଯଦି (u->deviceCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->data, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(ମିଥ୍ୟା);
        }
    }

    ଶୂନ୍ୟ download(UMatData* u, ଶୂନ୍ୟ* dstptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ size_t sz[],
                  ଅଚଳ size_t srcofs[], ଅଚଳ size_t srcstep[],
                  ଅଚଳ size_t dststep[]) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି (!u || !u->handle || !dstptr)
            ଫେରନ୍ତୁ;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    ଶୂନ୍ୟ upload(UMatData* u, ଅଚଳ ଶୂନ୍ୟ* srcptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ size_t sz[],
                ଅଚଳ size_t dstofs[], ଅଚଳ size_t dststep[],
                ଅଚଳ size_t srcstep[]) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି (!u || !srcptr)
            ଫେରନ୍ତୁ;
        ଯଦି (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, ଖାଲି, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(ସତ୍ୟ);
        u->markDeviceCopyObsolete(ମିଥ୍ୟା);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    ଶୂନ୍ୟ copy(UMatData* src, UMatData* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ size_t sz[],
              ଅଚଳ size_t srcofs[], ଅଚଳ size_t srcstep[],
              ଅଚଳ size_t dstofs[], ଅଚଳ size_t dststep[], ତାର୍କିକ /*sync*/) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି (!src || !dst || !src->handle || !dst->handle)
            ଫେରନ୍ତୁ;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(ସତ୍ୟ);
        dst->markDeviceCopyObsolete(ମିଥ୍ୟା);
    }

ବ୍ୟକ୍ତିଗତ:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    ସ୍ଥିର ଶୂନ୍ୟ copyPlanes(uchar* srcbase, ଅଚଳ size_t srcofs[], ଅଚଳ size_t srcstep[],
                           uchar* dstbase, ଅଚଳ size_t dstofs[], ଅଚଳ size_t dststep[],
                           ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ size_t sz[], cudaMemcpyKind kind)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)INT_MAX);
            ଯଦି (sz[i] == 0)
                ଫେରନ୍ତୁ;
            ଯଦି (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            ଯଦି (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        ଅଚଳ Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        ଅଚଳ size_t planesz = it.size;
        ପାଇଁ (size_t j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, ନୂତନ CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
