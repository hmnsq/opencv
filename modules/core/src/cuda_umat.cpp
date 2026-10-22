// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#अन्तर्भाव "precomp.hpp"

नामाकाश cv { नामाकाश cuda {

#ifndef HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    पटिनिवत्त रिक्त;
}

#अञ्ञथा

वग्ग CudaUMatAllocator CV_FINAL : सार्वजनिक MatAllocator
{
सार्वजनिक:
    UMatData* allocate(पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type, शून्य* data0,
                       size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        ध्रुव size_t elemSize = CV_ELEM_SIZE(type);
        size_t total = elemSize;
        अत्थाय (पूर्णाङ्क i = 0; i < dims; i++)
            total *= (size_t)sizes[i];

        सचे (step)
        {
            step[dims - 1] = elemSize;
            अत्थाय (पूर्णाङ्क i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (size_t)sizes[i + 1];
        }

        UMatData* u = नूतन UMatData(एतत्);
        u->size = total;

        सचे (total > 0)
            cudaSafeCall(cudaMalloc(&u->handle, total));

        सचे (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
            u->markDeviceCopyObsolete(सच्च);
        }
        अञ्ञथा
        {
            u->markHostCopyObsolete(सच्च);
        }
        पटिनिवत्त u;
    }

    तार्किक allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        सचे (!u)
            पटिनिवत्त मिच्छा;
        सचे (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            सचे (u->data)
                u->markDeviceCopyObsolete(सच्च);
            अञ्ञथा
                u->markHostCopyObsolete(सच्च);
        }
        पटिनिवत्त सच्च;
    }

    शून्य deallocate(UMatData* u) ध्रुव CV_OVERRIDE
    {
        सचे (!u)
            पटिनिवत्त;
        सचे (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        सचे (u->data && !(u->flags & UMatData::USER_ALLOCATED))
            fastFree(u->data);
        delete u;
    }

    शून्य map(UMatData* u, AccessFlag accessFlags) ध्रुव CV_OVERRIDE
    {
        सचे (!u)
            पटिनिवत्त;
        सचे (!u->data && u->size > 0)
            u->data = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        सचे (u->hostCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->data, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(मिच्छा);
        }
        सचे (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(सच्च);
    }

    शून्य unmap(UMatData* u) ध्रुव CV_OVERRIDE
    {
        सचे (!u)
            पटिनिवत्त;
        सचे (u->deviceCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->data, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(मिच्छा);
        }
    }

    शून्य download(UMatData* u, शून्य* dstptr, पूर्णाङ्क dims, ध्रुव size_t sz[],
                  ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
                  ध्रुव size_t dststep[]) ध्रुव CV_OVERRIDE
    {
        सचे (!u || !u->handle || !dstptr)
            पटिनिवत्त;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    शून्य upload(UMatData* u, ध्रुव शून्य* srcptr, पूर्णाङ्क dims, ध्रुव size_t sz[],
                ध्रुव size_t dstofs[], ध्रुव size_t dststep[],
                ध्रुव size_t srcstep[]) ध्रुव CV_OVERRIDE
    {
        सचे (!u || !srcptr)
            पटिनिवत्त;
        सचे (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, रिक्त, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(सच्च);
        u->markDeviceCopyObsolete(मिच्छा);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    शून्य copy(UMatData* src, UMatData* dst, पूर्णाङ्क dims, ध्रुव size_t sz[],
              ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
              ध्रुव size_t dstofs[], ध्रुव size_t dststep[], तार्किक /*sync*/) ध्रुव CV_OVERRIDE
    {
        सचे (!src || !dst || !src->handle || !dst->handle)
            पटिनिवत्त;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(सच्च);
        dst->markDeviceCopyObsolete(मिच्छा);
    }

गुप्त:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    स्थिर शून्य copyPlanes(uchar* srcbase, ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
                           uchar* dstbase, ध्रुव size_t dstofs[], ध्रुव size_t dststep[],
                           पूर्णाङ्क dims, ध्रुव size_t sz[], cudaMemcpyKind kind)
    {
        पूर्णाङ्क isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        अत्थाय (पूर्णाङ्क i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)INT_MAX);
            सचे (sz[i] == 0)
                पटिनिवत्त;
            सचे (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            सचे (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (पूर्णाङ्क)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        ध्रुव Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        ध्रुव size_t planesz = it.size;
        अत्थाय (size_t j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, नूतन CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
