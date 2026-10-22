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
    प्रत्यागम रिक्त;
}

#अण्णहा

वर्ग CudaUMatAllocator CV_FINAL : सार्वजनिक MatAllocator
{
सार्वजनिक:
    UMatData* allocate(पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type, शून्य* data0,
                       size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        ध्रुव size_t elemSize = CV_ELEM_SIZE(type);
        size_t total = elemSize;
        प्रति (पूर्णाङ्क i = 0; i < dims; i++)
            total *= (size_t)sizes[i];

        जइ (step)
        {
            step[dims - 1] = elemSize;
            प्रति (पूर्णाङ्क i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (size_t)sizes[i + 1];
        }

        UMatData* u = नूतन UMatData(एतत्);
        u->size = total;

        जइ (total > 0)
            cudaSafeCall(cudaMalloc(&u->handle, total));

        जइ (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
            u->markDeviceCopyObsolete(सच्च);
        }
        अण्णहा
        {
            u->markHostCopyObsolete(सच्च);
        }
        प्रत्यागम u;
    }

    तार्किक allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        जइ (!u)
            प्रत्यागम मिच्छ;
        जइ (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            जइ (u->data)
                u->markDeviceCopyObsolete(सच्च);
            अण्णहा
                u->markHostCopyObsolete(सच्च);
        }
        प्रत्यागम सच्च;
    }

    शून्य deallocate(UMatData* u) ध्रुव CV_OVERRIDE
    {
        जइ (!u)
            प्रत्यागम;
        जइ (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        जइ (u->data && !(u->flags & UMatData::USER_ALLOCATED))
            fastFree(u->data);
        delete u;
    }

    शून्य map(UMatData* u, AccessFlag accessFlags) ध्रुव CV_OVERRIDE
    {
        जइ (!u)
            प्रत्यागम;
        जइ (!u->data && u->size > 0)
            u->data = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        जइ (u->hostCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->data, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(मिच्छ);
        }
        जइ (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(सच्च);
    }

    शून्य unmap(UMatData* u) ध्रुव CV_OVERRIDE
    {
        जइ (!u)
            प्रत्यागम;
        जइ (u->deviceCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->data, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(मिच्छ);
        }
    }

    शून्य download(UMatData* u, शून्य* dstptr, पूर्णाङ्क dims, ध्रुव size_t sz[],
                  ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
                  ध्रुव size_t dststep[]) ध्रुव CV_OVERRIDE
    {
        जइ (!u || !u->handle || !dstptr)
            प्रत्यागम;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    शून्य upload(UMatData* u, ध्रुव शून्य* srcptr, पूर्णाङ्क dims, ध्रुव size_t sz[],
                ध्रुव size_t dstofs[], ध्रुव size_t dststep[],
                ध्रुव size_t srcstep[]) ध्रुव CV_OVERRIDE
    {
        जइ (!u || !srcptr)
            प्रत्यागम;
        जइ (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, रिक्त, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(सच्च);
        u->markDeviceCopyObsolete(मिच्छ);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    शून्य copy(UMatData* src, UMatData* dst, पूर्णाङ्क dims, ध्रुव size_t sz[],
              ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
              ध्रुव size_t dstofs[], ध्रुव size_t dststep[], तार्किक /*sync*/) ध्रुव CV_OVERRIDE
    {
        जइ (!src || !dst || !src->handle || !dst->handle)
            प्रत्यागम;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(सच्च);
        dst->markDeviceCopyObsolete(मिच्छ);
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
        प्रति (पूर्णाङ्क i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)INT_MAX);
            जइ (sz[i] == 0)
                प्रत्यागम;
            जइ (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            जइ (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (पूर्णाङ्क)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        ध्रुव Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        ध्रुव size_t planesz = it.size;
        प्रति (size_t j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, नूतन CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
