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

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ <map>

using નામાકાશ cv;
using નામાકાશ cv::cuda;

#ifdef HAVE_CUDA

નામાકાશ {

વર્ગ HostMemAllocator : જાહેર MatAllocator
{
જાહેર:
    explicit HostMemAllocator(unsigned પૂર્ણાંક flags) : flags_(flags)
    {
    }

    UMatData* allocate(પૂર્ણાંક dims, અચલ પૂર્ણાંક* sizes, પૂર્ણાંક type,
                       શૂન્ય* data0, size_t* step,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) અચલ CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        માટે (પૂર્ણાંક i = dims-1; i >= 0; i--)
        {
            જો (step)
            {
                જો (data0 && step[i] != CV_AUTOSTEP)
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                નહીંતર
                {
                    step[i] = total;
                }
            }

            total *= sizes[i];
        }

        UMatData* u = નવું UMatData(આ);
        u->size = total;

        જો (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
        }
        નહીંતર
        {
            શૂન્ય* ptr = 0;
            cudaSafeCall( cudaHostAlloc(&ptr, total, flags_) );

            u->data = u->origdata = static_cast<uchar*>(ptr);
        }

        પાછા u;
    }

    તાર્કિક allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) અચલ CV_OVERRIDE
    {
        પાછા (u != ખાલી);
    }

    શૂન્ય deallocate(UMatData* u) અચલ CV_OVERRIDE
    {
        જો (!u)
            પાછા;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        જો (u->refcount == 0)
        {
            જો ( !(u->flags & UMatData::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            delete u;
        }
    }

ખાનગી:
    unsigned પૂર્ણાંક flags_;
};

} // namespace

#endif

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(alloc_type);
    throw_no_cuda();
#નહીંતર
    સ્થિર std::map<unsigned પૂર્ણાંક, Ptr<MatAllocator> > allocators;

    unsigned પૂર્ણાંક flag = cudaHostAllocDefault;

    પસંદગી (alloc_type)
    {
    સ્થિતિ PAGE_LOCKED:    flag = cudaHostAllocDefault; તોડો;
    સ્થિતિ SHARED:         flag = cudaHostAllocMapped;  તોડો;
    સ્થિતિ WRITE_COMBINED: flag = cudaHostAllocWriteCombined; તોડો;
    મૂળભૂત:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    જો (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    પાછા a.get();
#endif
}

#ifdef HAVE_CUDA
નામાકાશ
{
    size_t alignUpStep(size_t what, size_t alignment)
    {
        size_t alignMask = alignment - 1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        પાછા res;
    }
}
#endif

શૂન્ય cv::cuda::HostMem::create(પૂર્ણાંક rows_, પૂર્ણાંક cols_, પૂર્ણાંક type_)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows_);
    CV_UNUSED(cols_);
    CV_UNUSED(type_);
    throw_no_cuda();
#નહીંતર
    જો (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.canMapHostMemory() );
    }

    type_ &= Mat::TYPE_MASK;

    જો (rows == rows_ && cols == cols_ && type() == type_ && data)
        પાછા;

    જો (data)
        release();

    CV_DbgAssert( rows_ >= 0 && cols_ >= 0 );

    જો (rows_ > 0 && cols_ > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        rows = rows_;
        cols = cols_;
        step = elemSize() * cols;
        પૂર્ણાંક sz[] = { rows, cols };
        size_t steps[] = { step, (size_t)CV_ELEM_SIZE(type_) };
        flags = updateContinuityFlag(flags, 2, sz, steps);

        જો (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            step = alignUpStep(step, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)step*rows;
        size_t nettosize = (size_t)_nettosize;

        જો (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = alignSize(nettosize, (પૂર્ણાંક)કદ(*refcount));

        શૂન્ય* ptr = 0;

        પસંદગી (alloc_type)
        {
        સ્થિતિ PAGE_LOCKED:    cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); તોડો;
        સ્થિતિ SHARED:         cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  તોડો;
        સ્થિતિ WRITE_COMBINED: cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); તોડો;
        મૂળભૂત:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = data =  (uchar*)ptr;
        dataend = data + nettosize;

        refcount = (પૂર્ણાંક*)cv::fastMalloc(કદ(*refcount));
        *refcount = 1;
    }
#endif
}

HostMem cv::cuda::HostMem::reshape(પૂર્ણાંક new_cn, પૂર્ણાંક new_rows) અચલ
{
    HostMem hdr = *આ;

    પૂર્ણાંક cn = channels();
    જો (new_cn == 0)
        new_cn = cn;

    પૂર્ણાંક total_width = cols * cn;

    જો ((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * total_width / new_cn;

    જો (new_rows != 0 && new_rows != rows)
    {
        પૂર્ણાંક total_size = total_width * rows;

        જો (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        જો ((unsigned)new_rows > (unsigned)total_size)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        જો (total_width * new_rows != total_size)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = total_width * elemSize1();
    }

    પૂર્ણાંક new_width = total_width / new_cn;

    જો (new_width * new_cn != total_width)
        CV_Error(cv::Error::Bad_અંક_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    પાછા hdr;
}

શૂન્ય cv::cuda::HostMem::release()
{
#ifdef HAVE_CUDA
    જો (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = data = datastart = 0;
    step = rows = cols = 0;
    refcount = 0;
#endif
}

GpuMat cv::cuda::HostMem::createGpuMatHeader() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    CV_Assert( alloc_type == SHARED );

    શૂન્ય *pdev;
    cudaSafeCall( cudaHostGetDevicePointer(&pdev, data, 0) );

    પાછા GpuMat(rows, cols, type(), pdev, step);
#endif
}

શૂન્ય cv::cuda::registerPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
    throw_no_cuda();
#નહીંતર
    CV_Assert( m.isContinuous() );
    cudaSafeCall( cudaHostRegister(m.data, m.step * m.rows, cudaHostRegisterPortable) );
#endif
}

શૂન્ય cv::cuda::unregisterPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
#નહીંતર
    cudaSafeCall( cudaHostUnregister(m.data) );
#endif
}
