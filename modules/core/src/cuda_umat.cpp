// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#ضمن "precomp.hpp"

نطاق cv { نطاق cuda {

#ifndef HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    أرجع عدم;
}

#وإلا

صنف CudaUMatAllocator CV_FINAL : عام MatAllocator
{
عام:
    UMatData* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type, فراغ* data0,
                       size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        ثابت size_t elemSize = CV_ELEM_SIZE(type);
        size_t total = elemSize;
        لكل (صحيح i = 0; i < dims; i++)
            total *= (size_t)sizes[i];

        𐡄𐡍 (step)
        {
            step[dims - 1] = elemSize;
            لكل (صحيح i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (size_t)sizes[i + 1];
        }

        UMatData* u = جديد UMatData(هذا);
        u->size = total;

        𐡄𐡍 (total > 0)
            cudaSafeCall(cudaMalloc(&u->handle, total));

        𐡄𐡍 (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
            u->markDeviceCopyObsolete(𐡒𐡔𐡈);
        }
        وإلا
        {
            u->markHostCopyObsolete(𐡒𐡔𐡈);
        }
        أرجع u;
    }

    منطقي allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        𐡄𐡍 (!u)
            أرجع خطأ;
        𐡄𐡍 (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            𐡄𐡍 (u->data)
                u->markDeviceCopyObsolete(𐡒𐡔𐡈);
            وإلا
                u->markHostCopyObsolete(𐡒𐡔𐡈);
        }
        أرجع 𐡒𐡔𐡈;
    }

    فراغ deallocate(UMatData* u) ثابت CV_OVERRIDE
    {
        𐡄𐡍 (!u)
            أرجع;
        𐡄𐡍 (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        𐡄𐡍 (u->data && !(u->flags & UMatData::USER_ALLOCATED))
            fastFree(u->data);
        delete u;
    }

    فراغ map(UMatData* u, AccessFlag accessFlags) ثابت CV_OVERRIDE
    {
        𐡄𐡍 (!u)
            أرجع;
        𐡄𐡍 (!u->data && u->size > 0)
            u->data = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        𐡄𐡍 (u->hostCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->data, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(خطأ);
        }
        𐡄𐡍 (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(𐡒𐡔𐡈);
    }

    فراغ unmap(UMatData* u) ثابت CV_OVERRIDE
    {
        𐡄𐡍 (!u)
            أرجع;
        𐡄𐡍 (u->deviceCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->data, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(خطأ);
        }
    }

    فراغ download(UMatData* u, فراغ* dstptr, صحيح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dststep[]) ثابت CV_OVERRIDE
    {
        𐡄𐡍 (!u || !u->handle || !dstptr)
            أرجع;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    فراغ upload(UMatData* u, ثابت فراغ* srcptr, صحيح dims, ثابت size_t sz[],
                ثابت size_t dstofs[], ثابت size_t dststep[],
                ثابت size_t srcstep[]) ثابت CV_OVERRIDE
    {
        𐡄𐡍 (!u || !srcptr)
            أرجع;
        𐡄𐡍 (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, عدم, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(𐡒𐡔𐡈);
        u->markDeviceCopyObsolete(خطأ);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    فراغ copy(UMatData* src, UMatData* dst, صحيح dims, ثابت size_t sz[],
              ثابت size_t srcofs[], ثابت size_t srcstep[],
              ثابت size_t dstofs[], ثابت size_t dststep[], منطقي /*sync*/) ثابت CV_OVERRIDE
    {
        𐡄𐡍 (!src || !dst || !src->handle || !dst->handle)
            أرجع;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(𐡒𐡔𐡈);
        dst->markDeviceCopyObsolete(خطأ);
    }

خاص:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    ساكن فراغ copyPlanes(uchar* srcbase, ثابت size_t srcofs[], ثابت size_t srcstep[],
                           uchar* dstbase, ثابت size_t dstofs[], ثابت size_t dststep[],
                           صحيح dims, ثابت size_t sz[], cudaMemcpyKind kind)
    {
        صحيح isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        لكل (صحيح i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)INT_MAX);
            𐡄𐡍 (sz[i] == 0)
                أرجع;
            𐡄𐡍 (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            𐡄𐡍 (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (صحيح)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        ثابت Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        ثابت size_t planesz = it.size;
        لكل (size_t j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, جديد CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
