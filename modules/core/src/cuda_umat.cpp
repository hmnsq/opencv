// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// CUDA-backed MatAllocator for UMat (Phase P1): synchronous host<->device transfers.

#అన్తర్భావ "precomp.hpp"

నామాకాశ cv { నామాకాశ cuda {

#నిర్వచితం_కాకపోతే HAVE_CUDA

MatAllocator* getCudaAllocator()
{
    తిరిగి ఖాళీ;
}

#లేకపోతే

తరగతి CudaUMatAllocator CV_FINAL : బహిరంగ MatAllocator
{
బహిరంగ:
    UMat_దత్తాంశం* allocate(పూర్ణాంకం dims, స్థిరరాశి పూర్ణాంకం* sizes, పూర్ణాంకం type, శూన్య* data0,
                       size_t* అడుగు, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) స్థిరరాశి CV_OVERRIDE
    {
        స్థిరరాశి size_t elem_కొలత = CV_ELEM_కొలత(type);
        size_t మొత్తం = elem_కొలత;
        కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
            మొత్తం *= (size_t)sizes[i];

        ఒకవేళ (అడుగు)
        {
            అడుగు[dims - 1] = elem_కొలత;
            కొరకు (పూర్ణాంకం i = dims - 2; i >= 0; i--)
                అడుగు[i] = అడుగు[i + 1] * (size_t)sizes[i + 1];
        }

        UMat_దత్తాంశం* u = కొత్త UMat_దత్తాంశం(ఇది);
        u->size = మొత్తం;

        ఒకవేళ (మొత్తం > 0)
            cuda_సురక్షిత_Call(cudaMalloc(&u->handle, మొత్తం));

        ఒకవేళ (data0)
        {
            u->దత్తాంశం = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_దత్తాంశం::USER_ALLOCATED;
            u->markDeviceCopyObsolete(నిజం);
        }
        లేకపోతే
        {
            u->markHostCopyObsolete(నిజం);
        }
        తిరిగి u;
    }

    తార్కిక allocate(UMat_దత్తాంశం* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ (!u)
            తిరిగి అబద్ధం;
        ఒకవేళ (!u->handle && u->size > 0)
        {
            cuda_సురక్షిత_Call(cudaMalloc(&u->handle, u->size));
            ఒకవేళ (u->దత్తాంశం)
                u->markDeviceCopyObsolete(నిజం);
            లేకపోతే
                u->markHostCopyObsolete(నిజం);
        }
        తిరిగి నిజం;
    }

    శూన్య deallocate(UMat_దత్తాంశం* u) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ (!u)
            తిరిగి;
        ఒకవేళ (u->handle)
            cuda_సురక్షిత_Call(cudaFree(u->handle));
        ఒకవేళ (u->దత్తాంశం && !(u->flags & UMat_దత్తాంశం::USER_ALLOCATED))
            fastFree(u->దత్తాంశం);
        తొలగించు u;
    }

    శూన్య map(UMat_దత్తాంశం* u, AccessFlag accessFlags) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ (!u)
            తిరిగి;
        ఒకవేళ (!u->దత్తాంశం && u->size > 0)
            u->దత్తాంశం = u->origdata = static_cast<uchar*>(fastMalloc(u->size));
        ఒకవేళ (u->hostCopyObsolete() && u->handle && u->దత్తాంశం)
        {
            cuda_సురక్షిత_Call(cudaMemcpy(u->దత్తాంశం, u->handle, u->size, cuda_Memcpy_Device_గమ్యం_Host));
            u->markHostCopyObsolete(అబద్ధం);
        }
        ఒకవేళ (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(నిజం);
    }

    శూన్య unmap(UMat_దత్తాంశం* u) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ (!u)
            తిరిగి;
        ఒకవేళ (u->deviceCopyObsolete() && u->handle && u->దత్తాంశం)
        {
            cuda_సురక్షిత_Call(cudaMemcpy(u->handle, u->దత్తాంశం, u->size, cuda_Memcpy_Host_గమ్యం_Device));
            u->markDeviceCopyObsolete(అబద్ధం);
        }
    }

    శూన్య download(UMat_దత్తాంశం* u, శూన్య* dstptr, పూర్ణాంకం dims, స్థిరరాశి size_t sz[],
                  స్థిరరాశి size_t srcofs[], స్థిరరాశి size_t srcstep[],
                  స్థిరరాశి size_t dststep[]) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ (!u || !u->handle || !dstptr)
            తిరిగి;
        copyPlanes((uchar*)u->handle, srcofs, srcstep, (uchar*)dstptr, 0, dststep,
                   dims, sz, cuda_Memcpy_Device_గమ్యం_Host);
    }

    శూన్య upload(UMat_దత్తాంశం* u, స్థిరరాశి శూన్య* srcptr, పూర్ణాంకం dims, స్థిరరాశి size_t sz[],
                స్థిరరాశి size_t dstofs[], స్థిరరాశి size_t dststep[],
                స్థిరరాశి size_t srcstep[]) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ (!u || !srcptr)
            తిరిగి;
        ఒకవేళ (!u->handle && u->size > 0)
            cuda_సురక్షిత_Call(cudaMalloc(&u->handle, u->size));
        copyPlanes((uchar*)srcptr, ఖాళీ, srcstep, (uchar*)u->handle, dstofs, dststep,
                   dims, sz, cuda_Memcpy_Host_గమ్యం_Device);
        u->markHostCopyObsolete(నిజం);
        u->markDeviceCopyObsolete(అబద్ధం);
    }

    // Same-world D2D only; UMat::copyTo calls copy() solely when both share this allocator.
    శూన్య copy(UMat_దత్తాంశం* src, UMat_దత్తాంశం* dst, పూర్ణాంకం dims, స్థిరరాశి size_t sz[],
              స్థిరరాశి size_t srcofs[], స్థిరరాశి size_t srcstep[],
              స్థిరరాశి size_t dstofs[], స్థిరరాశి size_t dststep[], తార్కిక /*sync*/) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ (!src || !dst || !src->handle || !dst->handle)
            తిరిగి;
        copyPlanes((uchar*)src->handle, srcofs, srcstep, (uchar*)dst->handle, dstofs, dststep,
                   dims, sz, cuda_Memcpy_Device_గమ్యం_Device);
        dst->markHostCopyObsolete(నిజం);
        dst->markDeviceCopyObsolete(అబద్ధం);
    }

అంతరంగ:
    // sz/steps follow the MatAllocator convention: the last dim carries element bytes.
    స్థిర శూన్య copyPlanes(uchar* srcbase, స్థిరరాశి size_t srcofs[], స్థిరరాశి size_t srcstep[],
                           uchar* dstbase, స్థిరరాశి size_t dstofs[], స్థిరరాశి size_t dststep[],
                           పూర్ణాంకం dims, స్థిరరాశి size_t sz[], cuda_Memcpy_రకం రకం)
    {
        పూర్ణాంకం isz[CV_MAX_DIM];
        uchar* srcptr = srcbase;
        uchar* dstptr = dstbase;
        కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
        {
            CV_Assert(sz[i] <= (size_t)గరిష్ఠ_పూర్ణాంకం);
            ఒకవేళ (sz[i] == 0)
                తిరిగి;
            ఒకవేళ (srcofs)
                srcptr += srcofs[i] * (i <= dims - 2 ? srcstep[i] : 1);
            ఒకవేళ (dstofs)
                dstptr += dstofs[i] * (i <= dims - 2 ? dststep[i] : 1);
            isz[i] = (పూర్ణాంకం)sz[i];
        }

        Mat src(dims, isz, CV_8U, srcptr, srcstep);
        Mat dst(dims, isz, CV_8U, dstptr, dststep);

        స్థిరరాశి Mat* arrays[] = { &src, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        స్థిరరాశి size_t planesz = it.size;
        కొరకు (size_t j = 0; j < it.nplanes; j++, ++it)
            cuda_సురక్షిత_Call(cudaMemcpy(ptrs[1], ptrs[0], planesz, రకం));
    }
};

MatAllocator* getCudaAllocator()
{
    CV_SINGLETON_LAZY_INIT(CudaUMatAllocator, కొత్త CudaUMatAllocator())
}

#అయితే_ముగింపు

}} // namespace cv { namespace cuda {
