// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#include "precomp.hpp"

namespace cv { namespace cuda {

#ifndef HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    ꦮꦭꦶ NULL;
}

#ꦭꦶꦪ

class CudaUMatAllocator CV_FINAL : public MatAllocator
{
public:
    UMatData* allocate(puur_naa_nka dims, const puur_naa_nka* sizes, puur_naa_nka type, void* data0,
                       size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) const CV_OVERRIDE
    {
        const size_t elemSize = CV_ELEM_SIZE(type);
        size_t total = elemSize;
        ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < dims; i++)
            total *= (size_t)sizes[i];

        ꦪꦼꦤ꧀ (step)
        {
            step[dims - 1] = elemSize;
            ꦏꦁꦒꦺꦴ (puur_naa_nka i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (size_t)sizes[i + 1];
        }

        UMatData* u = new UMatData(this);
        u->size = total;

        ꦪꦼꦤ꧀ (total > 0)
            cudaSafeCall(cudaMalloc(&u->handle, total));

        ꦪꦼꦤ꧀ (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
            u->markDeviceCopyObsolete(true);
        }
        ꦭꦶꦪ
        {
            u->markHostCopyObsolete(true);
        }
        ꦮꦭꦶ u;
    }

    bool allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) const CV_OVERRIDE
    {
        ꦪꦼꦤ꧀ (!u)
            ꦮꦭꦶ false;
        ꦪꦼꦤ꧀ (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            ꦪꦼꦤ꧀ (u->data)
                u->markDeviceCopyObsolete(true);
            ꦭꦶꦪ
                u->markHostCopyObsolete(true);
        }
        ꦮꦭꦶ true;
    }

    void deallocate(UMatData* u) const CV_OVERRIDE
    {
        ꦪꦼꦤ꧀ (!u)
            ꦮꦭꦶ;
        ꦪꦼꦤ꧀ (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        ꦪꦼꦤ꧀ (u->data && !(u->flags & UMatData::USER_ALLOCATED))
            fastFree(u->data);
        delete u;
    }

    void map(UMatData* u, AccessFlag accessFlags) const CV_OVERRIDE
    {
        ꦪꦼꦤ꧀ (!u)
            ꦮꦭꦶ;
        ꦪꦼꦤ꧀ (!u->data && u->size > 0)
            u->data = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        ꦪꦼꦤ꧀ (u->hostCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->data, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(false);
        }
        ꦪꦼꦤ꧀ (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(true);
    }

    void unmap(UMatData* u) const CV_OVERRIDE
    {
        ꦪꦼꦤ꧀ (!u)
            ꦮꦭꦶ;
        ꦪꦼꦤ꧀ (u->deviceCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->data, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(false);
        }
    }

    void download(UMatData* u, void* dstptr, puur_naa_nka dims, const size_t sz[],
                  const size_t srcofs[], const size_t srcstep[],
                  const size_t dststep[]) const CV_OVERRIDE
    {
        ꦪꦼꦤ꧀ (!u || !u->handle || !dstptr)
            ꦮꦭꦶ;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    void upload(UMatData* u, const void* srcptr, puur_naa_nka dims, const size_t sz[],
                const size_t dstofs[], const size_t dststep[],
                const size_t srcstep[]) const CV_OVERRIDE
    {
        ꦪꦼꦤ꧀ (!u || !srcptr)
            ꦮꦭꦶ;
        ꦪꦼꦤ꧀ (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, NULL, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(true);
        u->markDeviceCopyObsolete(false);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    void copy(UMatData* src, UMatData* dst, puur_naa_nka dims, const size_t sz[],
              const size_t srcofs[], const size_t srcstep[],
              const size_t dstofs[], const size_t dststep[], bool /*sync*/) const CV_OVERRIDE
    {
        ꦪꦼꦤ꧀ (!src || !dst || !src->handle || !dst->handle)
            ꦮꦭꦶ;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(true);
        dst->markDeviceCopyObsolete(false);
    }

private:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    static void copyPlanes(uchar* srcbase, const size_t srcofs[], const size_t srcstep[],
                           uchar* dstbase, const size_t dstofs[], const size_t dststep[],
                           puur_naa_nka dims, const size_t sz[], cudaMemcpyKind kind)
    {
        puur_naa_nka isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)INT_MAX);
            ꦪꦼꦤ꧀ (sz[i] == 0)
                ꦮꦭꦶ;
            ꦪꦼꦤ꧀ (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            ꦪꦼꦤ꧀ (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (puur_naa_nka)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        const Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        const size_t planesz = it.size;
        ꦏꦁꦒꦺꦴ (size_t j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, new CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
