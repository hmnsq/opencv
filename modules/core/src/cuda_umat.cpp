// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ cuda {

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

#ಇಲ್ಲದಿದ್ದರೆ

ವರ್ಗ CudaUMatAllocator CV_FINAL : ಸಾರ್ವಜನಿಕ MatAllocator
{
ಸಾರ್ವಜನಿಕ:
    UMat_ದತ್ತಾಂಶ* allocate(ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sizes, ಪೂರ್ಣಾಂಕ type, ಶೂನ್ಯ* data0,
                       size_t* ಹೆಜ್ಜೆ, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಸ್ಥಿರಾಂಕ size_t elem_ಗಾತ್ರ = CV_ELEM_ಗಾತ್ರ(type);
        size_t ಒಟ್ಟು = elem_ಗಾತ್ರ;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
            ಒಟ್ಟು *= (size_t)sizes[i];

        ಆದರೆ (ಹೆಜ್ಜೆ)
        {
            ಹೆಜ್ಜೆ[dims - 1] = elem_ಗಾತ್ರ;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = dims - 2; i >= 0; i--)
                ಹೆಜ್ಜೆ[i] = ಹೆಜ್ಜೆ[i + 1] * (size_t)sizes[i + 1];
        }

        UMat_ದತ್ತಾಂಶ* u = ಹೊಸ UMat_ದತ್ತಾಂಶ(ಇದು);
        u->size = ಒಟ್ಟು;

        ಆದರೆ (ಒಟ್ಟು > 0)
            cuda_ಸುರಕ್ಷಿತ_Call(cudaMalloc(&u->handle, ಒಟ್ಟು));

        ಆದರೆ (data0)
        {
            u->ದತ್ತಾಂಶ = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_ದತ್ತಾಂಶ::USER_ALLOCATED;
            u->markDeviceCopyObsolete(ನಿಜ);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            u->markHostCopyObsolete(ನಿಜ);
        }
        ಹಿಂತಿರುಗಿ u;
    }

    ತಾರ್ಕಿಕ allocate(UMat_ದತ್ತಾಂಶ* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ (!u)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಆದರೆ (!u->handle && u->size > 0)
        {
            cuda_ಸುರಕ್ಷಿತ_Call(cudaMalloc(&u->handle, u->size));
            ಆದರೆ (u->ದತ್ತಾಂಶ)
                u->markDeviceCopyObsolete(ನಿಜ);
            ಇಲ್ಲದಿದ್ದರೆ
                u->markHostCopyObsolete(ನಿಜ);
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

    ಶೂನ್ಯ deallocate(UMat_ದತ್ತಾಂಶ* u) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ (!u)
            ಹಿಂತಿರುಗಿ;
        ಆದರೆ (u->handle)
            cuda_ಸುರಕ್ಷಿತ_Call(cudaFree(u->handle));
        ಆದರೆ (u->ದತ್ತಾಂಶ && !(u->flags & UMat_ದತ್ತಾಂಶ::USER_ALLOCATED))
            fastFree(u->ದತ್ತಾಂಶ);
        ಅಳಿಸು u;
    }

    ಶೂನ್ಯ map(UMat_ದತ್ತಾಂಶ* u, AccessFlag accessFlags) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ (!u)
            ಹಿಂತಿರುಗಿ;
        ಆದರೆ (!u->ದತ್ತಾಂಶ && u->size > 0)
            u->ದತ್ತಾಂಶ = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        ಆದರೆ (u->hostCopyObsolete() && u->handle && u->ದತ್ತಾಂಶ)
        {
            cuda_ಸುರಕ್ಷಿತ_Call(cudaMemcpy(u->ದತ್ತಾಂಶ, u->handle, u->size, cuda_Memcpy_Device_ಗಮ್ಯ_Host));
            u->markHostCopyObsolete(ಸುಳ್ಳು);
        }
        ಆದರೆ (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(ನಿಜ);
    }

    ಶೂನ್ಯ unmap(UMat_ದತ್ತಾಂಶ* u) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ (!u)
            ಹಿಂತಿರುಗಿ;
        ಆದರೆ (u->deviceCopyObsolete() && u->handle && u->ದತ್ತಾಂಶ)
        {
            cuda_ಸುರಕ್ಷಿತ_Call(cudaMemcpy(u->handle, u->ದತ್ತಾಂಶ, u->size, cuda_Memcpy_Host_ಗಮ್ಯ_Device));
            u->markDeviceCopyObsolete(ಸುಳ್ಳು);
        }
    }

    ಶೂನ್ಯ download(UMat_ದತ್ತಾಂಶ* u, ಶೂನ್ಯ* dstptr, ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ size_t sz[],
                  ಸ್ಥಿರಾಂಕ size_t srcofs[], ಸ್ಥಿರಾಂಕ size_t srcstep[],
                  ಸ್ಥಿರಾಂಕ size_t dststep[]) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ (!u || !u->handle || !dstptr)
            ಹಿಂತಿರುಗಿ;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cuda_Memcpy_Device_ಗಮ್ಯ_Host);
    }

    ಶೂನ್ಯ upload(UMat_ದತ್ತಾಂಶ* u, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* srcptr, ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ size_t sz[],
                ಸ್ಥಿರಾಂಕ size_t dstofs[], ಸ್ಥಿರಾಂಕ size_t dststep[],
                ಸ್ಥಿರಾಂಕ size_t srcstep[]) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ (!u || !srcptr)
            ಹಿಂತಿರುಗಿ;
        ಆದರೆ (!u->handle && u->size > 0)
            cuda_ಸುರಕ್ಷಿತ_Call(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, ಖಾಲಿ, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cuda_Memcpy_Host_ಗಮ್ಯ_Device);
        u->markHostCopyObsolete(ನಿಜ);
        u->markDeviceCopyObsolete(ಸುಳ್ಳು);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    ಶೂನ್ಯ copy(UMat_ದತ್ತಾಂಶ* src, UMat_ದತ್ತಾಂಶ* dst, ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ size_t sz[],
              ಸ್ಥಿರಾಂಕ size_t srcofs[], ಸ್ಥಿರಾಂಕ size_t srcstep[],
              ಸ್ಥಿರಾಂಕ size_t dstofs[], ಸ್ಥಿರಾಂಕ size_t dststep[], ತಾರ್ಕಿಕ /*sync*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ (!src || !dst || !src->handle || !dst->handle)
            ಹಿಂತಿರುಗಿ;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cuda_Memcpy_Device_ಗಮ್ಯ_Device);
        dst->markHostCopyObsolete(ನಿಜ);
        dst->markDeviceCopyObsolete(ಸುಳ್ಳು);
    }

ಖಾಸಗಿ:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    ಸ್ಥಿರ ಶೂನ್ಯ copyPlanes(uchar* srcbase, ಸ್ಥಿರಾಂಕ size_t srcofs[], ಸ್ಥಿರಾಂಕ size_t srcstep[],
                           uchar* dstbase, ಸ್ಥಿರಾಂಕ size_t dstofs[], ಸ್ಥಿರಾಂಕ size_t dststep[],
                           ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ size_t sz[], cuda_Memcpy_ಬಗೆ ಬಗೆ)
    {
        ಪೂರ್ಣಾಂಕ isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);
            ಆದರೆ (sz[i] == 0)
                ಹಿಂತಿರುಗಿ;
            ಆದರೆ (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            ಆದರೆ (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (ಪೂರ್ಣಾಂಕ)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        ಸ್ಥಿರಾಂಕ Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        ಸ್ಥಿರಾಂಕ size_t planesz = it.size;
        ಗಾಗಿ (size_t j = 0; j < it.nplanes; j++, ++it)
            cuda_ಸುರಕ್ಷಿತ_Call(cudaMemcpy(ptrs[1], ptrs[0], planesz, ಬಗೆ));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, ಹೊಸ CudaUMatAllocator())
}

#ಆದರೆ_ಅಂತ್ಯ

}} // namespace cv { namespace cuda {
