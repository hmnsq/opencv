// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#അന്തര്ഭാവ "precomp.hpp"

നാമാകാശ cv { നാമാകാശ cuda {

#ifndef HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    തിരികെ ശൂന്യ;
}

#അല്ലെങ്കിൽ

വർഗ്ഗം CudaUMatAllocator CV_FINAL : പൊതു MatAllocator
{
പൊതു:
    UMatData* allocate(പൂർണ്ണസംഖ്യ dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* sizes, പൂർണ്ണസംഖ്യ type, ശൂന്യം* data0,
                       size_t* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) സ്ഥിരം CV_OVERRIDE
    {
        സ്ഥിരം size_t elemSize = CV_ELEM_SIZE(type);
        size_t total = elemSize;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
            total *= (size_t)sizes[i];

        എങ്കിൽ (step)
        {
            step[dims - 1] = elemSize;
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (size_t)sizes[i + 1];
        }

        UMatData* u = പുതിയ UMatData(ഇത്);
        u->size = total;

        എങ്കിൽ (total > 0)
            cudaSafeCall(cudaMalloc(&u->handle, total));

        എങ്കിൽ (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
            u->markDeviceCopyObsolete(സത്യം);
        }
        അല്ലെങ്കിൽ
        {
            u->markHostCopyObsolete(സത്യം);
        }
        തിരികെ u;
    }

    തർക്കം allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ (!u)
            തിരികെ അസത്യം;
        എങ്കിൽ (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            എങ്കിൽ (u->data)
                u->markDeviceCopyObsolete(സത്യം);
            അല്ലെങ്കിൽ
                u->markHostCopyObsolete(സത്യം);
        }
        തിരികെ സത്യം;
    }

    ശൂന്യം deallocate(UMatData* u) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ (!u)
            തിരികെ;
        എങ്കിൽ (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        എങ്കിൽ (u->data && !(u->flags & UMatData::USER_ALLOCATED))
            fastFree(u->data);
        delete u;
    }

    ശൂന്യം map(UMatData* u, AccessFlag accessFlags) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ (!u)
            തിരികെ;
        എങ്കിൽ (!u->data && u->size > 0)
            u->data = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        എങ്കിൽ (u->hostCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->data, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(അസത്യം);
        }
        എങ്കിൽ (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(സത്യം);
    }

    ശൂന്യം unmap(UMatData* u) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ (!u)
            തിരികെ;
        എങ്കിൽ (u->deviceCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->data, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(അസത്യം);
        }
    }

    ശൂന്യം download(UMatData* u, ശൂന്യം* dstptr, പൂർണ്ണസംഖ്യ dims, സ്ഥിരം size_t sz[],
                  സ്ഥിരം size_t srcofs[], സ്ഥിരം size_t srcstep[],
                  സ്ഥിരം size_t dststep[]) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ (!u || !u->handle || !dstptr)
            തിരികെ;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    ശൂന്യം upload(UMatData* u, സ്ഥിരം ശൂന്യം* srcptr, പൂർണ്ണസംഖ്യ dims, സ്ഥിരം size_t sz[],
                സ്ഥിരം size_t dstofs[], സ്ഥിരം size_t dststep[],
                സ്ഥിരം size_t srcstep[]) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ (!u || !srcptr)
            തിരികെ;
        എങ്കിൽ (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, ശൂന്യ, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(സത്യം);
        u->markDeviceCopyObsolete(അസത്യം);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    ശൂന്യം copy(UMatData* src, UMatData* dst, പൂർണ്ണസംഖ്യ dims, സ്ഥിരം size_t sz[],
              സ്ഥിരം size_t srcofs[], സ്ഥിരം size_t srcstep[],
              സ്ഥിരം size_t dstofs[], സ്ഥിരം size_t dststep[], തർക്കം /*sync*/) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ (!src || !dst || !src->handle || !dst->handle)
            തിരികെ;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(സത്യം);
        dst->markDeviceCopyObsolete(അസത്യം);
    }

സ്വകാര്യ:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    സ്ഥിതം ശൂന്യം copyPlanes(uchar* srcbase, സ്ഥിരം size_t srcofs[], സ്ഥിരം size_t srcstep[],
                           uchar* dstbase, സ്ഥിരം size_t dstofs[], സ്ഥിരം size_t dststep[],
                           പൂർണ്ണസംഖ്യ dims, സ്ഥിരം size_t sz[], cudaMemcpyKind kind)
    {
        പൂർണ്ണസംഖ്യ isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)INT_MAX);
            എങ്കിൽ (sz[i] == 0)
                തിരികെ;
            എങ്കിൽ (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            എങ്കിൽ (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (പൂർണ്ണസംഖ്യ)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        സ്ഥിരം Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        സ്ഥിരം size_t planesz = it.size;
        വേണ്ടി (size_t j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, പുതിയ CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
