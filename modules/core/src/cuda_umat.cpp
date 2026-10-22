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
    ফেরত ফাঁকা;
}

#অন্যথা

শ্রেণী CudaUMatAllocator CV_FINAL : সর্বজনীন MatAllocator
{
সর্বজনীন:
    UMat_তথ্য* allocate(পূর্ণসংখ্যা dims, ধ্রুবক পূর্ণসংখ্যা* sizes, পূর্ণসংখ্যা type, শূন্য* data0,
                       size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ধ্রুবক CV_OVERRIDE
    {
        ধ্রুবক size_t elem_আকার = CV_ELEM_আকার(type);
        size_t যোগ = elem_আকার;
        জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
            যোগ *= (size_t)sizes[i];

        যদি (step)
        {
            step[dims - 1] = elem_আকার;
            জন্য (পূর্ণসংখ্যা i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (size_t)sizes[i + 1];
        }

        UMat_তথ্য* u = নতুন UMat_তথ্য(এই);
        u->size = যোগ;

        যদি (যোগ > 0)
            cudaSafeCall(cudaMalloc(&u->handle, যোগ));

        যদি (data0)
        {
            u->তথ্য = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_তথ্য::USER_ALLOCATED;
            u->markDeviceCopyObsolete(সত্য);
        }
        অন্যথা
        {
            u->markHostCopyObsolete(সত্য);
        }
        ফেরত u;
    }

    যৌক্তিক allocate(UMat_তথ্য* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ধ্রুবক CV_OVERRIDE
    {
        যদি (!u)
            ফেরত মিথ্যা;
        যদি (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            যদি (u->তথ্য)
                u->markDeviceCopyObsolete(সত্য);
            অন্যথা
                u->markHostCopyObsolete(সত্য);
        }
        ফেরত সত্য;
    }

    শূন্য deallocate(UMat_তথ্য* u) ধ্রুবক CV_OVERRIDE
    {
        যদি (!u)
            ফেরত;
        যদি (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        যদি (u->তথ্য && !(u->flags & UMat_তথ্য::USER_ALLOCATED))
            fastFree(u->তথ্য);
        delete u;
    }

    শূন্য map(UMat_তথ্য* u, AccessFlag accessFlags) ধ্রুবক CV_OVERRIDE
    {
        যদি (!u)
            ফেরত;
        যদি (!u->তথ্য && u->size > 0)
            u->তথ্য = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        যদি (u->hostCopyObsolete() && u->handle && u->তথ্য)
        {
            cudaSafeCall(cudaMemcpy(u->তথ্য, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(মিথ্যা);
        }
        যদি (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(সত্য);
    }

    শূন্য unmap(UMat_তথ্য* u) ধ্রুবক CV_OVERRIDE
    {
        যদি (!u)
            ফেরত;
        যদি (u->deviceCopyObsolete() && u->handle && u->তথ্য)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->তথ্য, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(মিথ্যা);
        }
    }

    শূন্য download(UMat_তথ্য* u, শূন্য* dstptr, পূর্ণসংখ্যা dims, ধ্রুবক size_t sz[],
                  ধ্রুবক size_t srcofs[], ধ্রুবক size_t srcstep[],
                  ধ্রুবক size_t dststep[]) ধ্রুবক CV_OVERRIDE
    {
        যদি (!u || !u->handle || !dstptr)
            ফেরত;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    শূন্য upload(UMat_তথ্য* u, ধ্রুবক শূন্য* srcptr, পূর্ণসংখ্যা dims, ধ্রুবক size_t sz[],
                ধ্রুবক size_t dstofs[], ধ্রুবক size_t dststep[],
                ধ্রুবক size_t srcstep[]) ধ্রুবক CV_OVERRIDE
    {
        যদি (!u || !srcptr)
            ফেরত;
        যদি (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, ফাঁকা, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(সত্য);
        u->markDeviceCopyObsolete(মিথ্যা);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    শূন্য copy(UMat_তথ্য* src, UMat_তথ্য* dst, পূর্ণসংখ্যা dims, ধ্রুবক size_t sz[],
              ধ্রুবক size_t srcofs[], ধ্রুবক size_t srcstep[],
              ধ্রুবক size_t dstofs[], ধ্রুবক size_t dststep[], যৌক্তিক /*sync*/) ধ্রুবক CV_OVERRIDE
    {
        যদি (!src || !dst || !src->handle || !dst->handle)
            ফেরত;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(সত্য);
        dst->markDeviceCopyObsolete(মিথ্যা);
    }

ব্যক্তিগত:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    স্থির শূন্য copyPlanes(uchar* srcbase, ধ্রুবক size_t srcofs[], ধ্রুবক size_t srcstep[],
                           uchar* dstbase, ধ্রুবক size_t dstofs[], ধ্রুবক size_t dststep[],
                           পূর্ণসংখ্যা dims, ধ্রুবক size_t sz[], cudaMemcpyKind kind)
    {
        পূর্ণসংখ্যা isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)INT_MAX);
            যদি (sz[i] == 0)
                ফেরত;
            যদি (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            যদি (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (পূর্ণসংখ্যা)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        ধ্রুবক Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        ধ্রুবক size_t planesz = it.size;
        জন্য (size_t j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, নতুন CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
