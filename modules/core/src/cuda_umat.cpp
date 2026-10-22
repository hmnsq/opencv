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
    परत रिक्त;
}

#अन्यथा

वर्ग CudaUMatAllocator CV_FINAL : सार्वजनिक MatAllocator
{
सार्वजनिक:
    UMatData* allocate(पूर्णांक dims, स्थिरांक पूर्णांक* sizes, पूर्णांक type, शून्य* data0,
                       माप_प्रकार* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) स्थिरांक CV_OVERRIDE
    {
        स्थिरांक माप_प्रकार elemSize = CV_ELEM_SIZE(type);
        माप_प्रकार total = elemSize;
        साठी (पूर्णांक i = 0; i < dims; i++)
            total *= (माप_प्रकार)sizes[i];

        जर (step)
        {
            step[dims - 1] = elemSize;
            साठी (पूर्णांक i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (माप_प्रकार)sizes[i + 1];
        }

        UMatData* u = नवीन UMatData(हे);
        u->size = total;

        जर (total > 0)
            cudaSafeCall(cudaMalloc(&u->handle, total));

        जर (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
            u->markDeviceCopyObsolete(सत्य);
        }
        अन्यथा
        {
            u->markHostCopyObsolete(सत्य);
        }
        परत u;
    }

    तार्किक allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) स्थिरांक CV_OVERRIDE
    {
        जर (!u)
            परत असत्य;
        जर (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            जर (u->data)
                u->markDeviceCopyObsolete(सत्य);
            अन्यथा
                u->markHostCopyObsolete(सत्य);
        }
        परत सत्य;
    }

    शून्य deallocate(UMatData* u) स्थिरांक CV_OVERRIDE
    {
        जर (!u)
            परत;
        जर (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        जर (u->data && !(u->flags & UMatData::USER_ALLOCATED))
            fastFree(u->data);
        delete u;
    }

    शून्य map(UMatData* u, AccessFlag accessFlags) स्थिरांक CV_OVERRIDE
    {
        जर (!u)
            परत;
        जर (!u->data && u->size > 0)
            u->data = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        जर (u->hostCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->data, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(असत्य);
        }
        जर (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(सत्य);
    }

    शून्य unmap(UMatData* u) स्थिरांक CV_OVERRIDE
    {
        जर (!u)
            परत;
        जर (u->deviceCopyObsolete() && u->handle && u->data)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->data, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(असत्य);
        }
    }

    शून्य download(UMatData* u, शून्य* dstptr, पूर्णांक dims, स्थिरांक माप_प्रकार sz[],
                  स्थिरांक माप_प्रकार srcofs[], स्थिरांक माप_प्रकार srcstep[],
                  स्थिरांक माप_प्रकार dststep[]) स्थिरांक CV_OVERRIDE
    {
        जर (!u || !u->handle || !dstptr)
            परत;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    शून्य upload(UMatData* u, स्थिरांक शून्य* srcptr, पूर्णांक dims, स्थिरांक माप_प्रकार sz[],
                स्थिरांक माप_प्रकार dstofs[], स्थिरांक माप_प्रकार dststep[],
                स्थिरांक माप_प्रकार srcstep[]) स्थिरांक CV_OVERRIDE
    {
        जर (!u || !srcptr)
            परत;
        जर (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, रिक्त, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(सत्य);
        u->markDeviceCopyObsolete(असत्य);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    शून्य copy(UMatData* src, UMatData* dst, पूर्णांक dims, स्थिरांक माप_प्रकार sz[],
              स्थिरांक माप_प्रकार srcofs[], स्थिरांक माप_प्रकार srcstep[],
              स्थिरांक माप_प्रकार dstofs[], स्थिरांक माप_प्रकार dststep[], तार्किक /*sync*/) स्थिरांक CV_OVERRIDE
    {
        जर (!src || !dst || !src->handle || !dst->handle)
            परत;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(सत्य);
        dst->markDeviceCopyObsolete(असत्य);
    }

खाजगी:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    स्थिर शून्य copyPlanes(uchar* srcbase, स्थिरांक माप_प्रकार srcofs[], स्थिरांक माप_प्रकार srcstep[],
                           uchar* dstbase, स्थिरांक माप_प्रकार dstofs[], स्थिरांक माप_प्रकार dststep[],
                           पूर्णांक dims, स्थिरांक माप_प्रकार sz[], cudaMemcpyKind kind)
    {
        पूर्णांक isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        साठी (पूर्णांक i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (माप_प्रकार)पूर्णांक_उच्च);
            जर (sz[i] == 0)
                परत;
            जर (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            जर (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (पूर्णांक)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        स्थिरांक Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        स्थिरांक माप_प्रकार planesz = it.size;
        साठी (माप_प्रकार j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, नवीन CudaUMatAllocator())
}

#endif

}} // namespace cv { namespace cuda {
