// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#अन्तर्भाव "precomp.hpp"

नामाकाश cv { नामाकाश cuda {

#ifndef HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    प्रत्यागम रिक्त;
}

#अन्यथा

वर्ग CudaUMatAllocator CV_FINAL : सार्वजनिक MatAllocator
{
सार्वजनिक:
    UMatData* allocate(पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type, शून्य* data0,
                       माप_प्रकार* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        ध्रुव माप_प्रकार elemSize = CV_ELEM_SIZE(type);
        माप_प्रकार total = elemSize;
        प्रति (पूर्णाङ्क i = 0; i < dims; i++)
            total *= (माप_प्रकार)sizes[i];

        यदि (step)
        {
            step[dims - 1] = elemSize;
            प्रति (पूर्णाङ्क i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (माप_प्रकार)sizes[i + 1];
        }

        UMatData* u = नूतन UMatData(एतत्);
        u->size = total;

        यदि (total > 0)
            cudaSafeCall(cudaMalloc(&u->handle, total));

        यदि (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
            u->markDeviceCopyObsolete(सत्य);
        }
        अन्यथा
        {
            u->markHostCopyObsolete(सत्य);
        }
        प्रत्यागम u;
    }

    तार्किक allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        यदि (!u)
            प्रत्यागम असत्य;
        यदि (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            यदि (u->data)
                u->markDeviceCopyObsolete(सत्य);
            अन्यथा
                u->markHostCopyObsolete(सत्य);
        }
        प्रत्यागम सत्य;
    }

    शून्य deallocate(UMatData* u) ध्रुव CV_OVERRIDE
    {
        यदि (!u)
            प्रत्यागम;
        यदि (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        यदि (u->data && !(u->flags & UMatData::USER_ALLOCATED))
            fastFree(u->data);
        delete u;
    }

    शून्य map(UMatData* u, AccessFlag accessFlags) ध्रुव CV_OVERRIDE
    {
        यदि (!u)
            प्रत्यागम;
        यदि (!u->data && u->size > 0)
            u->data = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        यदि (u->hostCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->data, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(असत्य);
        }
        यदि (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(सत्य);
    }

    शून्य unmap(UMatData* u) ध्रुव CV_OVERRIDE
    {
        यदि (!u)
            प्रत्यागम;
        यदि (u->deviceCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->data, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(असत्य);
        }
    }

    शून्य download(UMatData* u, शून्य* dstptr, पूर्णाङ्क dims, ध्रुव माप_प्रकार sz[],
                  ध्रुव माप_प्रकार srcofs[], ध्रुव माप_प्रकार srcstep[],
                  ध्रुव माप_प्रकार dststep[]) ध्रुव CV_OVERRIDE
    {
        यदि (!u || !u->handle || !dstptr)
            प्रत्यागम;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    शून्य upload(UMatData* u, ध्रुव शून्य* srcptr, पूर्णाङ्क dims, ध्रुव माप_प्रकार sz[],
                ध्रुव माप_प्रकार dstofs[], ध्रुव माप_प्रकार dststep[],
                ध्रुव माप_प्रकार srcstep[]) ध्रुव CV_OVERRIDE
    {
        यदि (!u || !srcptr)
            प्रत्यागम;
        यदि (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, रिक्त, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(सत्य);
        u->markDeviceCopyObsolete(असत्य);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    शून्य copy(UMatData* src, UMatData* dst, पूर्णाङ्क dims, ध्रुव माप_प्रकार sz[],
              ध्रुव माप_प्रकार srcofs[], ध्रुव माप_प्रकार srcstep[],
              ध्रुव माप_प्रकार dstofs[], ध्रुव माप_प्रकार dststep[], तार्किक /*sync*/) ध्रुव CV_OVERRIDE
    {
        यदि (!src || !dst || !src->handle || !dst->handle)
            प्रत्यागम;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(सत्य);
        dst->markDeviceCopyObsolete(असत्य);
    }

गुप्त:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    स्थिर शून्य copyPlanes(uchar* srcbase, ध्रुव माप_प्रकार srcofs[], ध्रुव माप_प्रकार srcstep[],
                           uchar* dstbase, ध्रुव माप_प्रकार dstofs[], ध्रुव माप_प्रकार dststep[],
                           पूर्णाङ्क dims, ध्रुव माप_प्रकार sz[], cudaMemcpyKind kind)
    {
        पूर्णाङ्क isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        प्रति (पूर्णाङ्क i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (माप_प्रकार)पूर्णांक_उच्च);
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
        ध्रुव माप_प्रकार planesz = it.size;
        प्रति (माप_प्रकार j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, नूतन CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
