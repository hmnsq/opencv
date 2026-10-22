// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#समावेश "precomp.hpp"

namespace cv { namespace cuda {

#अगर_अघोषित HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    वापस शून्य;
}

#अन्यथा

श्रेणी CudaUMatAllocator CV_FINAL : खुला MatAllocator
{
खुला:
    UMat_तथ्य* allocate(पूर्णांक dims, स्थिर पूर्णांक* sizes, पूर्णांक type, व्योम* data0,
                       माप_प्रकार* step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) स्थिर CV_OVERRIDE
    {
        स्थिर माप_प्रकार elem_आकार = CV_ELEM_आकार(type);
        माप_प्रकार योग = elem_आकार;
        क्रम (पूर्णांक i = 0; i < dims; i++)
            योग *= (माप_प्रकार)sizes[i];

        अगर (step)
        {
            step[dims - 1] = elem_आकार;
            क्रम (पूर्णांक i = dims - 2; i >= 0; i--)
                step[i] = step[i + 1] * (माप_प्रकार)sizes[i + 1];
        }

        UMat_तथ्य* u = नया UMat_तथ्य(यह);
        u->size = योग;

        अगर (योग > 0)
            cudaSafeCall(cudaMalloc(&u->handle, योग));

        अगर (data0)
        {
            u->तथ्य = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_तथ्य::USER_ALLOCATED;
            u->markDeviceCopyObsolete(true);
        }
        अन्यथा
        {
            u->markHostCopyObsolete(true);
        }
        वापस u;
    }

    bool allocate(UMat_तथ्य* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) स्थिर CV_OVERRIDE
    {
        अगर (!u)
            वापस false;
        अगर (!u->handle && u->size > 0)
        {
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
            अगर (u->तथ्य)
                u->markDeviceCopyObsolete(true);
            अन्यथा
                u->markHostCopyObsolete(true);
        }
        वापस true;
    }

    व्योम deallocate(UMat_तथ्य* u) स्थिर CV_OVERRIDE
    {
        अगर (!u)
            वापस;
        अगर (u->handle)
            cudaSafeCall(cudaFree(u->handle));
        अगर (u->तथ्य && !(u->flags & UMat_तथ्य::USER_ALLOCATED))
            fastFree(u->तथ्य);
        मिटाओ u;
    }

    व्योम map(UMat_तथ्य* u, AccessFlag accessFlags) स्थिर CV_OVERRIDE
    {
        अगर (!u)
            वापस;
        अगर (!u->तथ्य && u->size > 0)
            u->तथ्य = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        अगर (u->hostCopyObsolete() && u->handle && u->तथ्य)
        {
            cudaSafeCall(cudaMemcpy(u->तथ्य, u->handle, u->size, cudaMemcpyDeviceToHost));
            u->markHostCopyObsolete(false);
        }
        अगर (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(true);
    }

    व्योम unmap(UMat_तथ्य* u) स्थिर CV_OVERRIDE
    {
        अगर (!u)
            वापस;
        अगर (u->deviceCopyObsolete() && u->handle && u->तथ्य)
        {
            cudaSafeCall(cudaMemcpy(u->handle, u->तथ्य, u->size, cudaMemcpyHostToDevice));
            u->markDeviceCopyObsolete(false);
        }
    }

    व्योम download(UMat_तथ्य* u, व्योम* dstptr, पूर्णांक dims, स्थिर माप_प्रकार sz[],
                  स्थिर माप_प्रकार srcofs[], स्थिर माप_प्रकार srcstep[],
                  स्थिर माप_प्रकार dststep[]) स्थिर CV_OVERRIDE
    {
        अगर (!u || !u->handle || !dstptr)
            वापस;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cudaMemcpyDeviceToHost);
    }

    व्योम upload(UMat_तथ्य* u, स्थिर व्योम* srcptr, पूर्णांक dims, स्थिर माप_प्रकार sz[],
                स्थिर माप_प्रकार dstofs[], स्थिर माप_प्रकार dststep[],
                स्थिर माप_प्रकार srcstep[]) स्थिर CV_OVERRIDE
    {
        अगर (!u || !srcptr)
            वापस;
        अगर (!u->handle && u->size > 0)
            cudaSafeCall(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, शून्य, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyHostToDevice);
        u->markHostCopyObsolete(true);
        u->markDeviceCopyObsolete(false);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    व्योम copy(UMat_तथ्य* src, UMat_तथ्य* dst, पूर्णांक dims, स्थिर माप_प्रकार sz[],
              स्थिर माप_प्रकार srcofs[], स्थिर माप_प्रकार srcstep[],
              स्थिर माप_प्रकार dstofs[], स्थिर माप_प्रकार dststep[], bool /*sync*/) स्थिर CV_OVERRIDE
    {
        अगर (!src || !dst || !src->handle || !dst->handle)
            वापस;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cudaMemcpyDeviceToDevice);
        dst->markHostCopyObsolete(true);
        dst->markDeviceCopyObsolete(false);
    }

निजी:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    अटल व्योम copyPlanes(uchar* srcbase, स्थिर माप_प्रकार srcofs[], स्थिर माप_प्रकार srcstep[],
                           uchar* dstbase, स्थिर माप_प्रकार dstofs[], स्थिर माप_प्रकार dststep[],
                           पूर्णांक dims, स्थिर माप_प्रकार sz[], cudaMemcpyKind kind)
    {
        पूर्णांक isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        क्रम (पूर्णांक i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (माप_प्रकार)पूर्णांक_उच्च);
            अगर (sz[i] == 0)
                वापस;
            अगर (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            अगर (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (पूर्णांक)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        स्थिर Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        स्थिर माप_प्रकार planesz = it.size;
        क्रम (माप_प्रकार j = 0; j < it.nplanes; j++, ++it)
            cudaSafeCall(cudaMemcpy(ptrs[1], ptrs[0], planesz, kind));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, नया CudaUMatAllocator())
}

#पूर्ण_अगर

}} // namespace cv { namespace cuda {
