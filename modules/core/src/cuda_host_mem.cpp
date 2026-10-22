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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ <map>

using ନାମାକାଶ cv;
using ନାମାକାଶ cv::cuda;

#ifdef HAVE_CUDA

ନାମାକାଶ {

ଶ୍ରେଣୀ HostMemAllocator : ସର୍ବସାଧାରଣ MatAllocator
{
ସର୍ବସାଧାରଣ:
    explicit HostMemAllocator(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags) : flags_(flags)
    {
    }

    UMatData* allocate(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type,
                       ଶୂନ୍ୟ* data0, size_t* step,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ଅଚଳ CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = dims-1; i >= 0; i--)
        {
            ଯଦି (step)
            {
                ଯଦି (data0 && step[i] != CV_AUTOSTEP)
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                ଅନ୍ୟଥା
                {
                    step[i] = total;
                }
            }

            total *= sizes[i];
        }

        UMatData* u = ନୂତନ UMatData(ଏହି);
        u->size = total;

        ଯଦି (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
        }
        ଅନ୍ୟଥା
        {
            ଶୂନ୍ୟ* ptr = 0;
            cudaSafeCall( cudaHostAlloc(&ptr, total, flags_) );

            u->data = u->origdata = static_cast<uchar*>(ptr);
        }

        ଫେରନ୍ତୁ u;
    }

    ତାର୍କିକ allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ଅଚଳ CV_OVERRIDE
    {
        ଫେରନ୍ତୁ (u != ଖାଲି);
    }

    ଶୂନ୍ୟ deallocate(UMatData* u) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି (!u)
            ଫେରନ୍ତୁ;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        ଯଦି (u->refcount == 0)
        {
            ଯଦି ( !(u->flags & UMatData::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            delete u;
        }
    }

ବ୍ୟକ୍ତିଗତ:
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags_;
};

} // namespace

#endif

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(alloc_type);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ସ୍ଥିର std::map<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା, Ptr<MatAllocator> > allocators;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag = cudaHostAllocDefault;

    ବାଛ (alloc_type)
    {
    ସ୍ଥିତି PAGE_LOCKED:    flag = cudaHostAllocDefault; ଭାଙ୍ଗ;
    ସ୍ଥିତି SHARED:         flag = cudaHostAllocMapped;  ଭାଙ୍ଗ;
    ସ୍ଥିତି WRITE_COMBINED: flag = cudaHostAllocWriteCombined; ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    ଯଦି (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    ଫେରନ୍ତୁ a.get();
#endif
}

#ifdef HAVE_CUDA
ନାମାକାଶ
{
    size_t alignUpStep(size_t what, size_t alignment)
    {
        size_t alignMask = alignment - 1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        ଫେରନ୍ତୁ res;
    }
}
#endif

ଶୂନ୍ୟ cv::cuda::HostMem::create(ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type_)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows_);
    CV_UNUSED(cols_);
    CV_UNUSED(type_);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଯଦି (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.canMapHostMemory() );
    }

    type_ &= Mat::TYPE_MASK;

    ଯଦି (rows == rows_ && cols == cols_ && type() == type_ && data)
        ଫେରନ୍ତୁ;

    ଯଦି (data)
        release();

    CV_DbgAssert( rows_ >= 0 && cols_ >= 0 );

    ଯଦି (rows_ > 0 && cols_ > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        rows = rows_;
        cols = cols_;
        step = elemSize() * cols;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz[] = { rows, cols };
        size_t steps[] = { step, (size_t)CV_ELEM_SIZE(type_) };
        flags = updateContinuityFlag(flags, 2, sz, steps);

        ଯଦି (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            step = alignUpStep(step, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)step*rows;
        size_t nettosize = (size_t)_nettosize;

        ଯଦି (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = alignSize(nettosize, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(*refcount));

        ଶୂନ୍ୟ* ptr = 0;

        ବାଛ (alloc_type)
        {
        ସ୍ଥିତି PAGE_LOCKED:    cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); ଭାଙ୍ଗ;
        ସ୍ଥିତି SHARED:         cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  ଭାଙ୍ଗ;
        ସ୍ଥିତି WRITE_COMBINED: cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = data =  (uchar*)ptr;
        dataend = data + nettosize;

        refcount = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)cv::fastMalloc(ଆକାର(*refcount));
        *refcount = 1;
    }
#endif
}

HostMem cv::cuda::HostMem::reshape(ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_rows) ଅଚଳ
{
    HostMem hdr = *ଏହି;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = channels();
    ଯଦି (new_cn == 0)
        new_cn = cn;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_width = cols * cn;

    ଯଦି ((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * total_width / new_cn;

    ଯଦି (new_rows != 0 && new_rows != rows)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_size = total_width * rows;

        ଯଦି (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        ଯଦି ((unsigned)new_rows > (unsigned)total_size)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        ଯଦି (total_width * new_rows != total_size)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = total_width * elemSize1();
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_width = total_width / new_cn;

    ଯଦି (new_width * new_cn != total_width)
        CV_Error(cv::Error::Bad_ଅଙ୍କ_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    ଫେରନ୍ତୁ hdr;
}

ଶୂନ୍ୟ cv::cuda::HostMem::release()
{
#ifdef HAVE_CUDA
    ଯଦି (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = data = datastart = 0;
    step = rows = cols = 0;
    refcount = 0;
#endif
}

GpuMat cv::cuda::HostMem::createGpuMatHeader() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    CV_Assert( alloc_type == SHARED );

    ଶୂନ୍ୟ *pdev;
    cudaSafeCall( cudaHostGetDevicePointer(&pdev, data, 0) );

    ଫେରନ୍ତୁ GpuMat(rows, cols, type(), pdev, step);
#endif
}

ଶୂନ୍ୟ cv::cuda::registerPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
    throw_no_cuda();
#ଅନ୍ୟଥା
    CV_Assert( m.isContinuous() );
    cudaSafeCall( cudaHostRegister(m.data, m.step * m.rows, cudaHostRegisterPortable) );
#endif
}

ଶୂନ୍ୟ cv::cuda::unregisterPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
#ଅନ୍ୟଥା
    cudaSafeCall( cudaHostUnregister(m.data) );
#endif
}
