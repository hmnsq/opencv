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

#ضمن "precomp.hpp"
#ضمن <map>

using نطاق cv;
using نطاق cv::cuda;

#ifdef HAVE_CUDA

نطاق {

صنف HostMemAllocator : عام MatAllocator
{
عام:
    explicit HostMemAllocator(unsigned صحيح flags) : flags_(flags)
    {
    }

    UMatData* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type,
                       فراغ* data0, size_t* step,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        لكل (صحيح i = dims-1; i >= 0; i--)
        {
            إذا (step)
            {
                إذا (data0 && step[i] != CV_AUTOSTEP)
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                وإلا
                {
                    step[i] = total;
                }
            }

            total *= sizes[i];
        }

        UMatData* u = جديد UMatData(هذا);
        u->size = total;

        إذا (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
        }
        وإلا
        {
            فراغ* ptr = 0;
            cudaSafeCall( cudaHostAlloc(&ptr, total, flags_) );

            u->data = u->origdata = static_cast<uchar*>(ptr);
        }

        إرجاع u;
    }

    منطقي allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        إرجاع (u != عدم);
    }

    فراغ deallocate(UMatData* u) ثابت CV_OVERRIDE
    {
        إذا (!u)
            إرجاع;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        إذا (u->refcount == 0)
        {
            إذا ( !(u->flags & UMatData::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            delete u;
        }
    }

خاص:
    unsigned صحيح flags_;
};

} // namespace

#endif

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(alloc_type);
    throw_no_cuda();
#وإلا
    ساكن std::map<unsigned صحيح, Ptr<MatAllocator> > allocators;

    unsigned صحيح flag = cudaHostAllocDefault;

    بدل (alloc_type)
    {
    حالة PAGE_LOCKED:    flag = cudaHostAllocDefault; اكسر;
    حالة SHARED:         flag = cudaHostAllocMapped;  اكسر;
    حالة WRITE_COMBINED: flag = cudaHostAllocWriteCombined; اكسر;
    افتراضي:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    إذا (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    إرجاع a.get();
#endif
}

#ifdef HAVE_CUDA
نطاق
{
    size_t alignUpStep(size_t what, size_t alignment)
    {
        size_t alignMask = alignment - 1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        إرجاع res;
    }
}
#endif

فراغ cv::cuda::HostMem::create(صحيح rows_, صحيح cols_, صحيح type_)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows_);
    CV_UNUSED(cols_);
    CV_UNUSED(type_);
    throw_no_cuda();
#وإلا
    إذا (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.canMapHostMemory() );
    }

    type_ &= Mat::TYPE_MASK;

    إذا (rows == rows_ && cols == cols_ && type() == type_ && data)
        إرجاع;

    إذا (data)
        release();

    CV_DbgAssert( rows_ >= 0 && cols_ >= 0 );

    إذا (rows_ > 0 && cols_ > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        rows = rows_;
        cols = cols_;
        step = elemSize() * cols;
        صحيح sz[] = { rows, cols };
        size_t steps[] = { step, (size_t)CV_ELEM_SIZE(type_) };
        flags = updateContinuityFlag(flags, 2, sz, steps);

        إذا (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            step = alignUpStep(step, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)step*rows;
        size_t nettosize = (size_t)_nettosize;

        إذا (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = alignSize(nettosize, (صحيح)حجم(*refcount));

        فراغ* ptr = 0;

        بدل (alloc_type)
        {
        حالة PAGE_LOCKED:    cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); اكسر;
        حالة SHARED:         cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  اكسر;
        حالة WRITE_COMBINED: cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); اكسر;
        افتراضي:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = data =  (uchar*)ptr;
        dataend = data + nettosize;

        refcount = (صحيح*)cv::fastMalloc(حجم(*refcount));
        *refcount = 1;
    }
#endif
}

HostMem cv::cuda::HostMem::reshape(صحيح new_cn, صحيح new_rows) ثابت
{
    HostMem hdr = *هذا;

    صحيح cn = channels();
    إذا (new_cn == 0)
        new_cn = cn;

    صحيح total_width = cols * cn;

    إذا ((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * total_width / new_cn;

    إذا (new_rows != 0 && new_rows != rows)
    {
        صحيح total_size = total_width * rows;

        إذا (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        إذا ((unsigned)new_rows > (unsigned)total_size)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        إذا (total_width * new_rows != total_size)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = total_width * elemSize1();
    }

    صحيح new_width = total_width / new_cn;

    إذا (new_width * new_cn != total_width)
        CV_Error(cv::Error::Bad_رقم_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    إرجاع hdr;
}

فراغ cv::cuda::HostMem::release()
{
#ifdef HAVE_CUDA
    إذا (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = data = datastart = 0;
    step = rows = cols = 0;
    refcount = 0;
#endif
}

GpuMat cv::cuda::HostMem::createGpuMatHeader() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وإلا
    CV_Assert( alloc_type == SHARED );

    فراغ *pdev;
    cudaSafeCall( cudaHostGetDevicePointer(&pdev, data, 0) );

    إرجاع GpuMat(rows, cols, type(), pdev, step);
#endif
}

فراغ cv::cuda::registerPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
    throw_no_cuda();
#وإلا
    CV_Assert( m.isContinuous() );
    cudaSafeCall( cudaHostRegister(m.data, m.step * m.rows, cudaHostRegisterPortable) );
#endif
}

فراغ cv::cuda::unregisterPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
#وإلا
    cudaSafeCall( cudaHostUnregister(m.data) );
#endif
}
