// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#অন্তর্ভাব "precomp.hpp"

নামাকাশ cv { নামাকাশ cuda {

#ifndef HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    প্রত্যাগম রিক্ত;
}

#অন্যথা

বর্গ CudaUMatAllocator CV_FINAL : সার্বজনিক MatAllocator
{
সার্বজনিক:
    UMatData* allocate(পূর্ণাঙ্ক dims, ধ্রুব পূর্ণাঙ্ক* sizes, পূর্ণাঙ্ক type, শূন্য* data0,
                       size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ধ্রুব CV_OVERRIDE
    {
        ধ্রুব size_t elemSize = CV_ELEM_SIZE(type);
        size_t total = elemSize;
        প্রতি (পূর্ণাঙ্ক i = 0; i < dims; i++)
            total *= (size_t)sizes[i];

        যদি (step)
        {
            step[dims - 1] = elemSize;
            প্রতি (পূর্ণাঙ্ক i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (size_t)sizes[i + 1];
        }

        UMatData* u = নূতন UMatData(এতত্);
        u->size = total;

        যদি (total > 0)
            cudaSafeCall(cudaMalloc(&u->handle, total));

        যদি (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
            u->markDeviceCopyObsolete(সত্য);
        }
        অন্যথা
        {
            u->markHostCopyObsolete(সত্য);
        }
        প্রত্যাগম u;
    }

    তার্কিক allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ধ্রুব CV_OVERRIDE
    {
        যদি (!u)
            প্রত্যাগম অসত্য;
        যদি (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            যদি (u->data)
                u->markDeviceCopyObsolete(সত্য);
            অন্যথা
                u->markHostCopyObsolete(সত্য);
        }
        প্রত্যাগম সত্য;
    }

    শূন্য deallocate(UMatData* u) ধ্রুব CV_OVERRIDE
    {
        যদি (!u)
            প্রত্যাগম;
        যদি (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        যদি (u->data && !(u->flags & UMatData::USER_ALLOCATED))
            fastFree(u->data);
        delete u;
    }

    শূন্য map(UMatData* u, AccessFlag accessFlags) ধ্রুব CV_OVERRIDE
    {
        যদি (!u)
            প্রত্যাগম;
        যদি (!u->data && u->size > 0)
            u->data = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        যদি (u->hostCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->data, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(অসত্য);
        }
        যদি (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(সত্য);
    }

    শূন্য unmap(UMatData* u) ধ্রুব CV_OVERRIDE
    {
        যদি (!u)
            প্রত্যাগম;
        যদি (u->deviceCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->data, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(অসত্য);
        }
    }

    শূন্য download(UMatData* u, শূন্য* dstptr, পূর্ণাঙ্ক dims, ধ্রুব size_t sz[],
                  ধ্রুব size_t srcofs[], ধ্রুব size_t srcstep[],
                  ধ্রুব size_t dststep[]) ধ্রুব CV_OVERRIDE
    {
        যদি (!u || !u->handle || !dstptr)
            প্রত্যাগম;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    শূন্য upload(UMatData* u, ধ্রুব শূন্য* srcptr, পূর্ণাঙ্ক dims, ধ্রুব size_t sz[],
                ধ্রুব size_t dstofs[], ধ্রুব size_t dststep[],
                ধ্রুব size_t srcstep[]) ধ্রুব CV_OVERRIDE
    {
        যদি (!u || !srcptr)
            প্রত্যাগম;
        যদি (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, রিক্ত, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(সত্য);
        u->markDeviceCopyObsolete(অসত্য);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    শূন্য copy(UMatData* src, UMatData* dst, পূর্ণাঙ্ক dims, ধ্রুব size_t sz[],
              ধ্রুব size_t srcofs[], ধ্রুব size_t srcstep[],
              ধ্রুব size_t dstofs[], ধ্রুব size_t dststep[], তার্কিক /*sync*/) ধ্রুব CV_OVERRIDE
    {
        যদি (!src || !dst || !src->handle || !dst->handle)
            প্রত্যাগম;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(সত্য);
        dst->markDeviceCopyObsolete(অসত্য);
    }

গুপ্ত:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    স্থির শূন্য copyPlanes(uchar* srcbase, ধ্রুব size_t srcofs[], ধ্রুব size_t srcstep[],
                           uchar* dstbase, ধ্রুব size_t dstofs[], ধ্রুব size_t dststep[],
                           পূর্ণাঙ্ক dims, ধ্রুব size_t sz[], cudaMemcpyKind kind)
    {
        পূর্ণাঙ্ক isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        প্রতি (পূর্ণাঙ্ক i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)INT_MAX);
            যদি (sz[i] == 0)
                প্রত্যাগম;
            যদি (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            যদি (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (পূর্ণাঙ্ক)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        ধ্রুব Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        ধ্রুব size_t planesz = it.size;
        প্রতি (size_t j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, নূতন CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
