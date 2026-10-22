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
    ལོག NULL;
}

#དེ་མིན

class CudaUMatAllocator CV_FINAL : public MatAllocator
{
public:
    UMatData* allocate(ཧྲིལ dims, const ཧྲིལ* sizes, ཧྲིལ type, void* data0,
                       size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) const CV_OVERRIDE
    {
        const size_t elemSize = CV_ELEM_SIZE(type);
        size_t total = elemSize;
        དོན་ལུ (ཧྲིལ i = 0; i < dims; i++)
            total *= (size_t)sizes[i];

        གལ་སྲིད (step)
        {
            step[dims - 1] = elemSize;
            དོན་ལུ (ཧྲིལ i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (size_t)sizes[i + 1];
        }

        UMatData* u = new UMatData(this);
        u->size = total;

        གལ་སྲིད (total > 0)
            cudaSafeCall(cudaMalloc(&u->handle, total));

        གལ་སྲིད (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
            u->markDeviceCopyObsolete(true);
        }
        དེ་མིན
        {
            u->markHostCopyObsolete(true);
        }
        ལོག u;
    }

    bool allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) const CV_OVERRIDE
    {
        གལ་སྲིད (!u)
            ལོག false;
        གལ་སྲིད (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            གལ་སྲིད (u->data)
                u->markDeviceCopyObsolete(true);
            དེ་མིན
                u->markHostCopyObsolete(true);
        }
        ལོག true;
    }

    void deallocate(UMatData* u) const CV_OVERRIDE
    {
        གལ་སྲིད (!u)
            ལོག;
        གལ་སྲིད (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        གལ་སྲིད (u->data && !(u->flags & UMatData::USER_ALLOCATED))
            fastFree(u->data);
        delete u;
    }

    void map(UMatData* u, AccessFlag accessFlags) const CV_OVERRIDE
    {
        གལ་སྲིད (!u)
            ལོག;
        གལ་སྲིད (!u->data && u->size > 0)
            u->data = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        གལ་སྲིད (u->hostCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->data, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(false);
        }
        གལ་སྲིད (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(true);
    }

    void unmap(UMatData* u) const CV_OVERRIDE
    {
        གལ་སྲིད (!u)
            ལོག;
        གལ་སྲིད (u->deviceCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->data, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(false);
        }
    }

    void download(UMatData* u, void* dstptr, ཧྲིལ dims, const size_t sz[],
                  const size_t srcofs[], const size_t srcstep[],
                  const size_t dststep[]) const CV_OVERRIDE
    {
        གལ་སྲིད (!u || !u->handle || !dstptr)
            ལོག;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    void upload(UMatData* u, const void* srcptr, ཧྲིལ dims, const size_t sz[],
                const size_t dstofs[], const size_t dststep[],
                const size_t srcstep[]) const CV_OVERRIDE
    {
        གལ་སྲིད (!u || !srcptr)
            ལོག;
        གལ་སྲིད (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, NULL, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(true);
        u->markDeviceCopyObsolete(false);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    void copy(UMatData* src, UMatData* dst, ཧྲིལ dims, const size_t sz[],
              const size_t srcofs[], const size_t srcstep[],
              const size_t dstofs[], const size_t dststep[], bool /*sync*/) const CV_OVERRIDE
    {
        གལ་སྲིད (!src || !dst || !src->handle || !dst->handle)
            ལོག;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(true);
        dst->markDeviceCopyObsolete(false);
    }

private:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    static void copyPlanes(uchar* srcbase, const size_t srcofs[], const size_t srcstep[],
                           uchar* dstbase, const size_t dstofs[], const size_t dststep[],
                           ཧྲིལ dims, const size_t sz[], cudaMemcpyKind kind)
    {
        ཧྲིལ isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        དོན་ལུ (ཧྲིལ i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)INT_MAX);
            གལ་སྲིད (sz[i] == 0)
                ལོག;
            གལ་སྲིད (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            གལ་སྲིད (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (ཧྲིལ)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        const Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        const size_t planesz = it.size;
        དོན་ལུ (size_t j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, new CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
