// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#अन्तर्भाव "precomp.hpp"

नामाकाश cv { नामाकाश cuda {

#यदि_अपरिभाषितम् HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    प्रत्यागम रिक्त;
}

#अन्यथा

वर्ग CudaUMatAllocator CV_FINAL : सार्वजनिक MatAllocator
{
सार्वजनिक:
    UMat_दत्त* allocate(पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type, शून्य* data0,
                       size_t* चरणः, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        ध्रुव size_t elem_आकार = CV_ELEM_आकार(type);
        size_t योग = elem_आकार;
        प्रति (पूर्णाङ्क i = 0; i < dims; i++)
            योग *= (size_t)sizes[i];

        यदि (चरणः)
        {
            चरणः[dims - 1] = elem_आकार;
            प्रति (पूर्णाङ्क i = dims - 2; i >= 0; i--)
                चरणः[i] = चरणः[i + 1] * (size_t)sizes[i + 1];
        }

        UMat_दत्त* u = नूतन UMat_दत्त(एतत्);
        u->size = योग;

        यदि (योग > 0)
            cudaSafeCall(cudaMalloc(&u->handle, योग));

        यदि (data0)
        {
            u->दत्त = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_दत्त::USER_ALLOCATED;
            u->markDeviceCopyObsolete(सत्य);
        }
        अन्यथा
        {
            u->markHostCopyObsolete(सत्य);
        }
        प्रत्यागम u;
    }

    तार्किक allocate(UMat_दत्त* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        यदि (!u)
            प्रत्यागम असत्य;
        यदि (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            यदि (u->दत्त)
                u->markDeviceCopyObsolete(सत्य);
            अन्यथा
                u->markHostCopyObsolete(सत्य);
        }
        प्रत्यागम सत्य;
    }

    शून्य deallocate(UMat_दत्त* u) ध्रुव CV_OVERRIDE
    {
        यदि (!u)
            प्रत्यागम;
        यदि (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        यदि (u->दत्त && !(u->flags & UMat_दत्त::USER_ALLOCATED))
            fastFree(u->दत्त);
        विलोपय u;
    }

    शून्य map(UMat_दत्त* u, AccessFlag accessFlags) ध्रुव CV_OVERRIDE
    {
        यदि (!u)
            प्रत्यागम;
        यदि (!u->दत्त && u->size > 0)
            u->दत्त = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        यदि (u->hostCopyObsolete() && u->handle && u->दत्त)
        {
            cudaSafeCall(cudaMemcpy(u->दत्त, u->handle, u->size, cuda_Memcpy_Device_गन्तव्यम्_Host));
            u->markHostCopyObsolete(असत्य);
        }
        यदि (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(सत्य);
    }

    शून्य unmap(UMat_दत्त* u) ध्रुव CV_OVERRIDE
    {
        यदि (!u)
            प्रत्यागम;
        यदि (u->deviceCopyObsolete() && u->handle && u->दत्त)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->दत्त, u->size, cuda_Memcpy_Host_गन्तव्यम्_Device));
            u->markDeviceCopyObsolete(असत्य);
        }
    }

    शून्य download(UMat_दत्त* u, शून्य* dstptr, पूर्णाङ्क dims, ध्रुव size_t sz[],
                  ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
                  ध्रुव size_t dststep[]) ध्रुव CV_OVERRIDE
    {
        यदि (!u || !u->handle || !dstptr)
            प्रत्यागम;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cuda_Memcpy_Device_गन्तव्यम्_Host);
    }

    शून्य upload(UMat_दत्त* u, ध्रुव शून्य* srcptr, पूर्णाङ्क dims, ध्रुव size_t sz[],
                ध्रुव size_t dstofs[], ध्रुव size_t dststep[],
                ध्रुव size_t srcstep[]) ध्रुव CV_OVERRIDE
    {
        यदि (!u || !srcptr)
            प्रत्यागम;
        यदि (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, रिक्त, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cuda_Memcpy_Host_गन्तव्यम्_Device);
        u->markHostCopyObsolete(सत्य);
        u->markDeviceCopyObsolete(असत्य);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    शून्य copy(UMat_दत्त* src, UMat_दत्त* dst, पूर्णाङ्क dims, ध्रुव size_t sz[],
              ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
              ध्रुव size_t dstofs[], ध्रुव size_t dststep[], तार्किक /*sync*/) ध्रुव CV_OVERRIDE
    {
        यदि (!src || !dst || !src->handle || !dst->handle)
            प्रत्यागम;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cuda_Memcpy_Device_गन्तव्यम्_Device);
        dst->markHostCopyObsolete(सत्य);
        dst->markDeviceCopyObsolete(असत्य);
    }

गुप्त:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    स्थिर शून्य copyPlanes(uchar* srcbase, ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
                           uchar* dstbase, ध्रुव size_t dstofs[], ध्रुव size_t dststep[],
                           पूर्णाङ्क dims, ध्रुव size_t sz[], cuda_Memcpy_विधा विधा)
    {
        पूर्णाङ्क isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        प्रति (पूर्णाङ्क i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)अधिकतम_पूर्णाङ्कः);
            यदि (sz[i] == 0)
                प्रत्यागम;
            यदि (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            यदि (dstofs)
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
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, विधा));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, नूतन CudaUMatAllocator())
}

#यदि_अन्तः

}} // namespace cv { namespace cuda {
