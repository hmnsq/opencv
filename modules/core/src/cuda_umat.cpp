// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ cuda {

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    ਵਾਪਸ ਨਿਰਾ;
}

#ਨਹੀਂਤਾਂ

ਵਰਗ CudaUMatAllocator CV_FINAL : ਜਨਤਕ MatAllocator
{
ਜਨਤਕ:
    UMat_ਅੰਕੜੇ* allocate(ਪੂਰਨ dims, ਅਚਲ ਪੂਰਨ* sizes, ਪੂਰਨ type, ਖਾਲੀ* data0,
                       size_t* ਕਦਮ, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ਅਚਲ CV_OVERRIDE
    {
        ਅਚਲ size_t elem_ਮਾਪ = CV_ELEM_ਮਾਪ(type);
        size_t ਕੁੱਲ = elem_ਮਾਪ;
        ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
            ਕੁੱਲ *= (size_t)sizes[i];

        ਜੇ (ਕਦਮ)
        {
            ਕਦਮ[dims - 1] = elem_ਮਾਪ;
            ਲਈ (ਪੂਰਨ i = dims - 2; i >= 0; i--)
                ਕਦਮ[i] = ਕਦਮ[i + 1] * (size_t)sizes[i + 1];
        }

        UMat_ਅੰਕੜੇ* u = ਨਵਾਂ UMat_ਅੰਕੜੇ(ਇਹ);
        u->size = ਕੁੱਲ;

        ਜੇ (ਕੁੱਲ > 0)
            cuda_ਸੁਰੱਖਿਅਤ_Call(cudaMalloc(&u->handle, ਕੁੱਲ));

        ਜੇ (data0)
        {
            u->ਅੰਕੜੇ = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_ਅੰਕੜੇ::USER_ALLOCATED;
            u->markDeviceCopyObsolete(ਸੱਚ);
        }
        ਨਹੀਂਤਾਂ
        {
            u->markHostCopyObsolete(ਸੱਚ);
        }
        ਵਾਪਸ u;
    }

    ਤਾਰਕਿਕ allocate(UMat_ਅੰਕੜੇ* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ (!u)
            ਵਾਪਸ ਝੂਠ;
        ਜੇ (!u->handle && u->size > 0)
        {
            cuda_ਸੁਰੱਖਿਅਤ_Call(cudaMalloc(&u->handle, u->size));
            ਜੇ (u->ਅੰਕੜੇ)
                u->markDeviceCopyObsolete(ਸੱਚ);
            ਨਹੀਂਤਾਂ
                u->markHostCopyObsolete(ਸੱਚ);
        }
        ਵਾਪਸ ਸੱਚ;
    }

    ਖਾਲੀ deallocate(UMat_ਅੰਕੜੇ* u) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ (!u)
            ਵਾਪਸ;
        ਜੇ (u->handle)
            cuda_ਸੁਰੱਖਿਅਤ_Call(cudaFree(u->handle));
        ਜੇ (u->ਅੰਕੜੇ && !(u->flags & UMat_ਅੰਕੜੇ::USER_ALLOCATED))
            fastFree(u->ਅੰਕੜੇ);
        ਮਿਟਾਓ u;
    }

    ਖਾਲੀ map(UMat_ਅੰਕੜੇ* u, AccessFlag accessFlags) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ (!u)
            ਵਾਪਸ;
        ਜੇ (!u->ਅੰਕੜੇ && u->size > 0)
            u->ਅੰਕੜੇ = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        ਜੇ (u->hostCopyObsolete() && u->handle && u->ਅੰਕੜੇ)
        {
            cuda_ਸੁਰੱਖਿਅਤ_Call(cudaMemcpy(u->ਅੰਕੜੇ, u->handle, u->size, cuda_Memcpy_Device_ਮੰਜ਼ਿਲ_Host));
            u->markHostCopyObsolete(ਝੂਠ);
        }
        ਜੇ (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(ਸੱਚ);
    }

    ਖਾਲੀ unmap(UMat_ਅੰਕੜੇ* u) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ (!u)
            ਵਾਪਸ;
        ਜੇ (u->deviceCopyObsolete() && u->handle && u->ਅੰਕੜੇ)
        {
            cuda_ਸੁਰੱਖਿਅਤ_Call(cudaMemcpy(u->handle, u->ਅੰਕੜੇ, u->size, cuda_Memcpy_Host_ਮੰਜ਼ਿਲ_Device));
            u->markDeviceCopyObsolete(ਝੂਠ);
        }
    }

    ਖਾਲੀ download(UMat_ਅੰਕੜੇ* u, ਖਾਲੀ* dstptr, ਪੂਰਨ dims, ਅਚਲ size_t sz[],
                  ਅਚਲ size_t srcofs[], ਅਚਲ size_t srcstep[],
                  ਅਚਲ size_t dststep[]) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ (!u || !u->handle || !dstptr)
            ਵਾਪਸ;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cuda_Memcpy_Device_ਮੰਜ਼ਿਲ_Host);
    }

    ਖਾਲੀ upload(UMat_ਅੰਕੜੇ* u, ਅਚਲ ਖਾਲੀ* srcptr, ਪੂਰਨ dims, ਅਚਲ size_t sz[],
                ਅਚਲ size_t dstofs[], ਅਚਲ size_t dststep[],
                ਅਚਲ size_t srcstep[]) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ (!u || !srcptr)
            ਵਾਪਸ;
        ਜੇ (!u->handle && u->size > 0)
            cuda_ਸੁਰੱਖਿਅਤ_Call(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, ਨਿਰਾ, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cuda_Memcpy_Host_ਮੰਜ਼ਿਲ_Device);
        u->markHostCopyObsolete(ਸੱਚ);
        u->markDeviceCopyObsolete(ਝੂਠ);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    ਖਾਲੀ copy(UMat_ਅੰਕੜੇ* src, UMat_ਅੰਕੜੇ* dst, ਪੂਰਨ dims, ਅਚਲ size_t sz[],
              ਅਚਲ size_t srcofs[], ਅਚਲ size_t srcstep[],
              ਅਚਲ size_t dstofs[], ਅਚਲ size_t dststep[], ਤਾਰਕਿਕ /*sync*/) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ (!src || !dst || !src->handle || !dst->handle)
            ਵਾਪਸ;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cuda_Memcpy_Device_ਮੰਜ਼ਿਲ_Device);
        dst->markHostCopyObsolete(ਸੱਚ);
        dst->markDeviceCopyObsolete(ਝੂਠ);
    }

ਨਿਜੀ:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    ਸਥਿਰ ਖਾਲੀ copyPlanes(uchar* srcbase, ਅਚਲ size_t srcofs[], ਅਚਲ size_t srcstep[],
                           uchar* dstbase, ਅਚਲ size_t dstofs[], ਅਚਲ size_t dststep[],
                           ਪੂਰਨ dims, ਅਚਲ size_t sz[], cuda_Memcpy_ਕਿਸਮ ਕਿਸਮ)
    {
        ਪੂਰਨ isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);
            ਜੇ (sz[i] == 0)
                ਵਾਪਸ;
            ਜੇ (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            ਜੇ (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (ਪੂਰਨ)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        ਅਚਲ Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        ਅਚਲ size_t planesz = it.size;
        ਲਈ (size_t j = 0; j < it.nplanes; j++, ++it)
            cuda_ਸੁਰੱਖਿਅਤ_Call(cudaMemcpy(ptrs[1], ptrs[0], planesz, ਕਿਸਮ));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, ਨਵਾਂ CudaUMatAllocator())
}

#ਜੇ_ਅੰਤ

}} // namespace cv { namespace cuda {
