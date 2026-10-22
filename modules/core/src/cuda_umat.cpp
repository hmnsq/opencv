// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#અન્તર્ભાવ "precomp.hpp"

નામાકાશ cv { નામાકાશ cuda {

#ifndef HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    પાછા ખાલી;
}

#નહીંતર

વર્ગ CudaUMatAllocator CV_FINAL : જાહેર MatAllocator
{
જાહેર:
    UMatData* allocate(પૂર્ણાંક dims, અચલ પૂર્ણાંક* sizes, પૂર્ણાંક type, શૂન્ય* data0,
                       size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) અચલ CV_OVERRIDE
    {
        અચલ size_t elemSize = CV_ELEM_SIZE(type);
        size_t total = elemSize;
        માટે (પૂર્ણાંક i = 0; i < dims; i++)
            total *= (size_t)sizes[i];

        જો (step)
        {
            step[dims - 1] = elemSize;
            માટે (પૂર્ણાંક i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (size_t)sizes[i + 1];
        }

        UMatData* u = નવું UMatData(આ);
        u->size = total;

        જો (total > 0)
            cudaSafeCall(cudaMalloc(&u->handle, total));

        જો (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
            u->markDeviceCopyObsolete(સાચું);
        }
        નહીંતર
        {
            u->markHostCopyObsolete(સાચું);
        }
        પાછા u;
    }

    તાર્કિક allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) અચલ CV_OVERRIDE
    {
        જો (!u)
            પાછા ખોટું;
        જો (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            જો (u->data)
                u->markDeviceCopyObsolete(સાચું);
            નહીંતર
                u->markHostCopyObsolete(સાચું);
        }
        પાછા સાચું;
    }

    શૂન્ય deallocate(UMatData* u) અચલ CV_OVERRIDE
    {
        જો (!u)
            પાછા;
        જો (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        જો (u->data && !(u->flags & UMatData::USER_ALLOCATED))
            fastFree(u->data);
        delete u;
    }

    શૂન્ય map(UMatData* u, AccessFlag accessFlags) અચલ CV_OVERRIDE
    {
        જો (!u)
            પાછા;
        જો (!u->data && u->size > 0)
            u->data = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        જો (u->hostCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->data, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(ખોટું);
        }
        જો (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(સાચું);
    }

    શૂન્ય unmap(UMatData* u) અચલ CV_OVERRIDE
    {
        જો (!u)
            પાછા;
        જો (u->deviceCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->data, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(ખોટું);
        }
    }

    શૂન્ય download(UMatData* u, શૂન્ય* dstptr, પૂર્ણાંક dims, અચલ size_t sz[],
                  અચલ size_t srcofs[], અચલ size_t srcstep[],
                  અચલ size_t dststep[]) અચલ CV_OVERRIDE
    {
        જો (!u || !u->handle || !dstptr)
            પાછા;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    શૂન્ય upload(UMatData* u, અચલ શૂન્ય* srcptr, પૂર્ણાંક dims, અચલ size_t sz[],
                અચલ size_t dstofs[], અચલ size_t dststep[],
                અચલ size_t srcstep[]) અચલ CV_OVERRIDE
    {
        જો (!u || !srcptr)
            પાછા;
        જો (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, ખાલી, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(સાચું);
        u->markDeviceCopyObsolete(ખોટું);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    શૂન્ય copy(UMatData* src, UMatData* dst, પૂર્ણાંક dims, અચલ size_t sz[],
              અચલ size_t srcofs[], અચલ size_t srcstep[],
              અચલ size_t dstofs[], અચલ size_t dststep[], તાર્કિક /*sync*/) અચલ CV_OVERRIDE
    {
        જો (!src || !dst || !src->handle || !dst->handle)
            પાછા;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(સાચું);
        dst->markDeviceCopyObsolete(ખોટું);
    }

ખાનગી:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    સ્થિર શૂન્ય copyPlanes(uchar* srcbase, અચલ size_t srcofs[], અચલ size_t srcstep[],
                           uchar* dstbase, અચલ size_t dstofs[], અચલ size_t dststep[],
                           પૂર્ણાંક dims, અચલ size_t sz[], cudaMemcpyKind kind)
    {
        પૂર્ણાંક isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        માટે (પૂર્ણાંક i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)INT_MAX);
            જો (sz[i] == 0)
                પાછા;
            જો (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            જો (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (પૂર્ણાંક)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        અચલ Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        અચલ size_t planesz = it.size;
        માટે (size_t j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, નવું CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
