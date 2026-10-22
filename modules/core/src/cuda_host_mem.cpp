/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#समावेश "precomp.hpp"
#समावेश <map>

using namespace cv;
using namespace cv::cuda;

#अगर_घोषित HAVE_CUDA

namespace {

श्रेणी HostMemAllocator : खुला MatAllocator
{
खुला:
    explicit HostMemAllocator(अचिन्हित पूर्णांक flags) : flags_(flags)
    {
    }

    UMat_तथ्य* allocate(पूर्णांक dims, स्थिर पूर्णांक* sizes, पूर्णांक type,
                       व्योम* data0, माप_प्रकार* step,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) स्थिर CV_OVERRIDE
    {
        माप_प्रकार योग = CV_ELEM_आकार(type);
        क्रम (पूर्णांक i = dims-1; i >= 0; i--)
        {
            अगर (step)
            {
                अगर (data0 && step[i] != CV_AUTOSTEP)
                {
                    CV_Assert(योग <= step[i]);
                    योग = step[i];
                }
                अन्यथा
                {
                    step[i] = योग;
                }
            }

            योग *= sizes[i];
        }

        UMat_तथ्य* u = नया UMat_तथ्य(यह);
        u->size = योग;

        अगर (data0)
        {
            u->तथ्य = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_तथ्य::USER_ALLOCATED;
        }
        अन्यथा
        {
            व्योम* ptr = 0;
            cudaSafeCall( cudaHostAlloc(&ptr, योग, flags_) );

            u->तथ्य = u->origdata = static_cast<uchar*>(ptr);
        }

        वापस u;
    }

    bool allocate(UMat_तथ्य* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) स्थिर CV_OVERRIDE
    {
        वापस (u != शून्य);
    }

    व्योम deallocate(UMat_तथ्य* u) स्थिर CV_OVERRIDE
    {
        अगर (!u)
            वापस;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        अगर (u->refcount == 0)
        {
            अगर ( !(u->flags & UMat_तथ्य::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            मिटाओ u;
        }
    }

निजी:
    अचिन्हित पूर्णांक flags_;
};

} // namespace

#पूर्ण_अगर

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(alloc_type);
    throw_no_cuda();
#अन्यथा
    अटल std::map<अचिन्हित पूर्णांक, Ptr<MatAllocator> > allocators;

    अचिन्हित पूर्णांक flag = cudaHostAllocDefault;

    चयन (alloc_type)
    {
    हाल PAGE_LOCKED:    flag = cudaHostAllocDefault; अवरोध;
    हाल SHARED:         flag = cudaHostAllocMapped;  अवरोध;
    हाल WRITE_COMBINED: flag = cudaHostAllocWriteCombined; अवरोध;
    शेष:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    अगर (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    वापस a.लो();
#पूर्ण_अगर
}

#अगर_घोषित HAVE_CUDA
namespace
{
    माप_प्रकार alignUpStep(माप_प्रकार what, माप_प्रकार alignment)
    {
        माप_प्रकार alignMask = alignment - 1;
        माप_प्रकार inverseAlignMask = ~alignMask;
        माप_प्रकार res = (what + alignMask) & inverseAlignMask;
        वापस res;
    }
}
#पूर्ण_अगर

व्योम cv::cuda::HostMem::create(पूर्णांक rows_, पूर्णांक cols_, पूर्णांक type_)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(rows_);
    CV_UNUSED(cols_);
    CV_UNUSED(type_);
    throw_no_cuda();
#अन्यथा
    अगर (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.canMapHostMemory() );
    }

    type_ &= Mat::TYPE_MASK;

    अगर (rows == rows_ && cols == cols_ && type() == type_ && तथ्य)
        वापस;

    अगर (तथ्य)
        release();

    CV_DbgAssert( rows_ >= 0 && cols_ >= 0 );

    अगर (rows_ > 0 && cols_ > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        rows = rows_;
        cols = cols_;
        step = elem_आकार() * cols;
        पूर्णांक sz[] = { rows, cols };
        माप_प्रकार steps[] = { step, (माप_प्रकार)CV_ELEM_आकार(type_) };
        flags = updateContinuityFlag(flags, 2, sz, steps);

        अगर (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            step = alignUpStep(step, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)step*rows;
        माप_प्रकार nettosize = (माप_प्रकार)_nettosize;

        अगर (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        माप_प्रकार datasize = align_आकार(nettosize, (पूर्णांक)माप(*refcount));

        व्योम* ptr = 0;

        चयन (alloc_type)
        {
        हाल PAGE_LOCKED:    cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); अवरोध;
        हाल SHARED:         cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  अवरोध;
        हाल WRITE_COMBINED: cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); अवरोध;
        शेष:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = तथ्य =  (uchar*)ptr;
        dataend = तथ्य + nettosize;

        refcount = (पूर्णांक*)cv::fastMalloc(माप(*refcount));
        *refcount = 1;
    }
#पूर्ण_अगर
}

HostMem cv::cuda::HostMem::reshape(पूर्णांक new_cn, पूर्णांक new_rows) स्थिर
{
    HostMem hdr = *यह;

    पूर्णांक cn = channels();
    अगर (new_cn == 0)
        new_cn = cn;

    पूर्णांक योग_width = cols * cn;

    अगर ((new_cn > योग_width || योग_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * योग_width / new_cn;

    अगर (new_rows != 0 && new_rows != rows)
    {
        पूर्णांक योग_आकार_2 = योग_width * rows;

        अगर (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        अगर ((अचिन्हित)new_rows > (अचिन्हित)योग_आकार_2)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        योग_width = योग_आकार_2 / new_rows;

        अगर (योग_width * new_rows != योग_आकार_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = योग_width * elemSize1();
    }

    पूर्णांक new_width = योग_width / new_cn;

    अगर (new_width * new_cn != योग_width)
        CV_Error(cv::Error::Bad_अंक_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    वापस hdr;
}

व्योम cv::cuda::HostMem::release()
{
#अगर_घोषित HAVE_CUDA
    अगर (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = तथ्य = datastart = 0;
    step = rows = cols = 0;
    refcount = 0;
#पूर्ण_अगर
}

GpuMat cv::cuda::HostMem::createGpuMatHeader() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    CV_Assert( alloc_type == SHARED );

    व्योम *pdev;
    cudaSafeCall( cudaHostGetDevicePointer(&pdev, तथ्य, 0) );

    वापस GpuMat(rows, cols, type(), pdev, step);
#पूर्ण_अगर
}

व्योम cv::cuda::registerPageLocked(Mat& m)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(m);
    throw_no_cuda();
#अन्यथा
    CV_Assert( m.isContinuous() );
    cudaSafeCall( cudaHostRegister(m.तथ्य, m.step * m.rows, cudaHostRegisterPortable) );
#पूर्ण_अगर
}

व्योम cv::cuda::unregisterPageLocked(Mat& m)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(m);
#अन्यथा
    cudaSafeCall( cudaHostUnregister(m.तथ्य) );
#पूर्ण_अगर
}
