// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ cuda {

#ifndef HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

#ಅನ್ಯಥಾ

ವರ್ಗ CudaUMatAllocator CV_FINAL : ಸಾರ್ವಜನಿಕ MatAllocator
{
ಸಾರ್ವಜನಿಕ:
    UMatData* allocate(ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sizes, ಪೂರ್ಣಾಙ್ಕ type, ಶೂನ್ಯ* data0,
                       size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ಧ್ರುವ CV_OVERRIDE
    {
        ಧ್ರುವ size_t elemSize = CV_ELEM_SIZE(type);
        size_t total = elemSize;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
            total *= (size_t)sizes[i];

        ಯದಿ (step)
        {
            step[dims - 1] = elemSize;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (size_t)sizes[i + 1];
        }

        UMatData* u = ನೂತನ UMatData(ಏತತ್);
        u->size = total;

        ಯದಿ (total > 0)
            cudaSafeCall(cudaMalloc(&u->handle, total));

        ಯದಿ (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
            u->markDeviceCopyObsolete(ಸತ್ಯ);
        }
        ಅನ್ಯಥಾ
        {
            u->markHostCopyObsolete(ಸತ್ಯ);
        }
        ಪ್ರತ್ಯಾಗಮ u;
    }

    ತಾರ್ಕಿಕ allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ (!u)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಯದಿ (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            ಯದಿ (u->data)
                u->markDeviceCopyObsolete(ಸತ್ಯ);
            ಅನ್ಯಥಾ
                u->markHostCopyObsolete(ಸತ್ಯ);
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ಶೂನ್ಯ deallocate(UMatData* u) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ (!u)
            ಪ್ರತ್ಯಾಗಮ;
        ಯದಿ (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        ಯದಿ (u->data && !(u->flags & UMatData::USER_ALLOCATED))
            fastFree(u->data);
        delete u;
    }

    ಶೂನ್ಯ map(UMatData* u, AccessFlag accessFlags) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ (!u)
            ಪ್ರತ್ಯಾಗಮ;
        ಯದಿ (!u->data && u->size > 0)
            u->data = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        ಯದಿ (u->hostCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->data, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(ಅಸತ್ಯ);
        }
        ಯದಿ (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(ಸತ್ಯ);
    }

    ಶೂನ್ಯ unmap(UMatData* u) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ (!u)
            ಪ್ರತ್ಯಾಗಮ;
        ಯದಿ (u->deviceCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->data, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(ಅಸತ್ಯ);
        }
    }

    ಶೂನ್ಯ download(UMatData* u, ಶೂನ್ಯ* dstptr, ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ size_t sz[],
                  ಧ್ರುವ size_t srcofs[], ಧ್ರುವ size_t srcstep[],
                  ಧ್ರುವ size_t dststep[]) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ (!u || !u->handle || !dstptr)
            ಪ್ರತ್ಯಾಗಮ;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    ಶೂನ್ಯ upload(UMatData* u, ಧ್ರುವ ಶೂನ್ಯ* srcptr, ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ size_t sz[],
                ಧ್ರುವ size_t dstofs[], ಧ್ರುವ size_t dststep[],
                ಧ್ರುವ size_t srcstep[]) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ (!u || !srcptr)
            ಪ್ರತ್ಯಾಗಮ;
        ಯದಿ (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, ರಿಕ್ತ, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(ಸತ್ಯ);
        u->markDeviceCopyObsolete(ಅಸತ್ಯ);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    ಶೂನ್ಯ copy(UMatData* src, UMatData* dst, ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ size_t sz[],
              ಧ್ರುವ size_t srcofs[], ಧ್ರುವ size_t srcstep[],
              ಧ್ರುವ size_t dstofs[], ಧ್ರುವ size_t dststep[], ತಾರ್ಕಿಕ /*sync*/) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ (!src || !dst || !src->handle || !dst->handle)
            ಪ್ರತ್ಯಾಗಮ;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(ಸತ್ಯ);
        dst->markDeviceCopyObsolete(ಅಸತ್ಯ);
    }

ಗುಪ್ತ:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    ಸ್ಥಿರ ಶೂನ್ಯ copyPlanes(uchar* srcbase, ಧ್ರುವ size_t srcofs[], ಧ್ರುವ size_t srcstep[],
                           uchar* dstbase, ಧ್ರುವ size_t dstofs[], ಧ್ರುವ size_t dststep[],
                           ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ size_t sz[], cudaMemcpyKind kind)
    {
        ಪೂರ್ಣಾಙ್ಕ isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)INT_MAX);
            ಯದಿ (sz[i] == 0)
                ಪ್ರತ್ಯಾಗಮ;
            ಯದಿ (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            ಯದಿ (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (ಪೂರ್ಣಾಙ್ಕ)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        ಧ್ರುವ Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        ಧ್ರುವ size_t planesz = it.size;
        ಪ್ರತಿ (size_t j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, ನೂತನ CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
