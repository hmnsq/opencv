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

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব <map>

using নামাকাশ cv;
using নামাকাশ cv::cuda;

#ifdef HAVE_CUDA

নামাকাশ {

শ্রেণী HostMemAllocator : সর্বজনীন MatAllocator
{
সর্বজনীন:
    explicit HostMemAllocator(unsigned পূর্ণসংখ্যা flags) : flags_(flags)
    {
    }

    UMat_তথ্য* allocate(পূর্ণসংখ্যা dims, ধ্রুবক পূর্ণসংখ্যা* sizes, পূর্ণসংখ্যা type,
                       শূন্য* data0, size_t* step,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ধ্রুবক CV_OVERRIDE
    {
        size_t যোগ = CV_ELEM_আকার(type);
        জন্য (পূর্ণসংখ্যা i = dims-1; i >= 0; i--)
        {
            যদি (step)
            {
                যদি (data0 && step[i] != CV_AUTOSTEP)
                {
                    CV_Assert(যোগ <= step[i]);
                    যোগ = step[i];
                }
                অন্যথা
                {
                    step[i] = যোগ;
                }
            }

            যোগ *= sizes[i];
        }

        UMat_তথ্য* u = নতুন UMat_তথ্য(এই);
        u->size = যোগ;

        যদি (data0)
        {
            u->তথ্য = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_তথ্য::USER_ALLOCATED;
        }
        অন্যথা
        {
            শূন্য* ptr = 0;
            cudaSafeCall( cudaHostAlloc(&ptr, যোগ, flags_) );

            u->তথ্য = u->origdata = static_cast<uchar*>(ptr);
        }

        ফেরত u;
    }

    যৌক্তিক allocate(UMat_তথ্য* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ধ্রুবক CV_OVERRIDE
    {
        ফেরত (u != ফাঁকা);
    }

    শূন্য deallocate(UMat_তথ্য* u) ধ্রুবক CV_OVERRIDE
    {
        যদি (!u)
            ফেরত;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        যদি (u->refcount == 0)
        {
            যদি ( !(u->flags & UMat_তথ্য::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            delete u;
        }
    }

ব্যক্তিগত:
    unsigned পূর্ণসংখ্যা flags_;
};

} // namespace

#endif

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(alloc_type);
    throw_no_cuda();
#অন্যথা
    স্থির std::map<unsigned পূর্ণসংখ্যা, Ptr<MatAllocator> > allocators;

    unsigned পূর্ণসংখ্যা flag = cudaHostAllocDefault;

    নির্বাচন (alloc_type)
    {
    ক্ষেত্র PAGE_LOCKED:    flag = cudaHostAllocDefault; বিরতি;
    ক্ষেত্র SHARED:         flag = cudaHostAllocMapped;  বিরতি;
    ক্ষেত্র WRITE_COMBINED: flag = cudaHostAllocWriteCombined; বিরতি;
    পূর্বনির্ধারিত:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    যদি (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    ফেরত a.get();
#endif
}

#ifdef HAVE_CUDA
নামাকাশ
{
    size_t alignUpStep(size_t what, size_t alignment)
    {
        size_t alignMask = alignment - 1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        ফেরত res;
    }
}
#endif

শূন্য cv::cuda::HostMem::create(পূর্ণসংখ্যা rows_, পূর্ণসংখ্যা cols_, পূর্ণসংখ্যা type_)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows_);
    CV_UNUSED(cols_);
    CV_UNUSED(type_);
    throw_no_cuda();
#অন্যথা
    যদি (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.canMapHostMemory() );
    }

    type_ &= Mat::TYPE_MASK;

    যদি (rows == rows_ && cols == cols_ && type() == type_ && তথ্য)
        ফেরত;

    যদি (তথ্য)
        release();

    CV_DbgAssert( rows_ >= 0 && cols_ >= 0 );

    যদি (rows_ > 0 && cols_ > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        rows = rows_;
        cols = cols_;
        step = elem_আকার() * cols;
        পূর্ণসংখ্যা sz[] = { rows, cols };
        size_t steps[] = { step, (size_t)CV_ELEM_আকার(type_) };
        flags = updateContinuityFlag(flags, 2, sz, steps);

        যদি (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            step = alignUpStep(step, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)step*rows;
        size_t nettosize = (size_t)_nettosize;

        যদি (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = align_আকার(nettosize, (পূর্ণসংখ্যা)আকার(*refcount));

        শূন্য* ptr = 0;

        নির্বাচন (alloc_type)
        {
        ক্ষেত্র PAGE_LOCKED:    cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); বিরতি;
        ক্ষেত্র SHARED:         cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  বিরতি;
        ক্ষেত্র WRITE_COMBINED: cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); বিরতি;
        পূর্বনির্ধারিত:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = তথ্য =  (uchar*)ptr;
        dataend = তথ্য + nettosize;

        refcount = (পূর্ণসংখ্যা*)cv::fastMalloc(আকার(*refcount));
        *refcount = 1;
    }
#endif
}

HostMem cv::cuda::HostMem::reshape(পূর্ণসংখ্যা new_cn, পূর্ণসংখ্যা new_rows) ধ্রুবক
{
    HostMem hdr = *এই;

    পূর্ণসংখ্যা cn = channels();
    যদি (new_cn == 0)
        new_cn = cn;

    পূর্ণসংখ্যা যোগ_width = cols * cn;

    যদি ((new_cn > যোগ_width || যোগ_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * যোগ_width / new_cn;

    যদি (new_rows != 0 && new_rows != rows)
    {
        পূর্ণসংখ্যা যোগ_আকার_2 = যোগ_width * rows;

        যদি (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        যদি ((unsigned)new_rows > (unsigned)যোগ_আকার_2)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        যোগ_width = যোগ_আকার_2 / new_rows;

        যদি (যোগ_width * new_rows != যোগ_আকার_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = যোগ_width * elemSize1();
    }

    পূর্ণসংখ্যা new_width = যোগ_width / new_cn;

    যদি (new_width * new_cn != যোগ_width)
        CV_Error(cv::Error::Bad_অঙ্ক_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    ফেরত hdr;
}

শূন্য cv::cuda::HostMem::release()
{
#ifdef HAVE_CUDA
    যদি (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = তথ্য = datastart = 0;
    step = rows = cols = 0;
    refcount = 0;
#endif
}

GpuMat cv::cuda::HostMem::createGpuMatHeader() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    CV_Assert( alloc_type == SHARED );

    শূন্য *pdev;
    cudaSafeCall( cudaHostGetDevicePointer(&pdev, তথ্য, 0) );

    ফেরত GpuMat(rows, cols, type(), pdev, step);
#endif
}

শূন্য cv::cuda::registerPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
    throw_no_cuda();
#অন্যথা
    CV_Assert( m.isContinuous() );
    cudaSafeCall( cudaHostRegister(m.তথ্য, m.step * m.rows, cudaHostRegisterPortable) );
#endif
}

শূন্য cv::cuda::unregisterPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
#অন্যথা
    cudaSafeCall( cudaHostUnregister(m.তথ্য) );
#endif
}
