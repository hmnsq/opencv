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

#شامل "precomp.hpp"
#شامل <map>

using فضاینام cv;
using فضاینام cv::cuda;

#ifdef HAVE_CUDA

فضاینام {

رده HostMemAllocator : عمومی MatAllocator
{
عمومی:
    explicit HostMemAllocator(unsigned صحیح flags) : flags_(flags)
    {
    }

    UMatData* allocate(صحیح dims, ثابت صحیح* sizes, صحیح type,
                       تهی* data0, size_t* step,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        برای (صحیح i = dims-1; i >= 0; i--)
        {
            اگر (step)
            {
                اگر (data0 && step[i] != CV_AUTOSTEP)
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                وگرنه
                {
                    step[i] = total;
                }
            }

            total *= sizes[i];
        }

        UMatData* u = جدید UMatData(این);
        u->size = total;

        اگر (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
        }
        وگرنه
        {
            تهی* ptr = 0;
            cudaSafeCall( cudaHostAlloc(&ptr, total, flags_) );

            u->data = u->origdata = static_cast<uchar*>(ptr);
        }

        بازگشت u;
    }

    منطقی allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        بازگشت (u != پوچ);
    }

    تهی deallocate(UMatData* u) ثابت CV_OVERRIDE
    {
        اگر (!u)
            بازگشت;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        اگر (u->refcount == 0)
        {
            اگر ( !(u->flags & UMatData::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            delete u;
        }
    }

خصوصی:
    unsigned صحیح flags_;
};

} // namespace

#endif

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(alloc_type);
    throw_no_cuda();
#وگرنه
    ایستا std::map<unsigned صحیح, Ptr<MatAllocator> > allocators;

    unsigned صحیح flag = cudaHostAllocDefault;

    انتخاب (alloc_type)
    {
    حالت PAGE_LOCKED:    flag = cudaHostAllocDefault; بشکن;
    حالت SHARED:         flag = cudaHostAllocMapped;  بشکن;
    حالت WRITE_COMBINED: flag = cudaHostAllocWriteCombined; بشکن;
    پیشفرض:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    اگر (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    بازگشت a.get();
#endif
}

#ifdef HAVE_CUDA
فضاینام
{
    size_t alignUpStep(size_t what, size_t alignment)
    {
        size_t alignMask = alignment - 1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        بازگشت res;
    }
}
#endif

تهی cv::cuda::HostMem::create(صحیح rows_, صحیح cols_, صحیح type_)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows_);
    CV_UNUSED(cols_);
    CV_UNUSED(type_);
    throw_no_cuda();
#وگرنه
    اگر (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.canMapHostMemory() );
    }

    type_ &= Mat::TYPE_MASK;

    اگر (rows == rows_ && cols == cols_ && type() == type_ && data)
        بازگشت;

    اگر (data)
        release();

    CV_DbgAssert( rows_ >= 0 && cols_ >= 0 );

    اگر (rows_ > 0 && cols_ > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        rows = rows_;
        cols = cols_;
        step = elemSize() * cols;
        صحیح sz[] = { rows, cols };
        size_t steps[] = { step, (size_t)CV_ELEM_SIZE(type_) };
        flags = updateContinuityFlag(flags, 2, sz, steps);

        اگر (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            step = alignUpStep(step, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)step*rows;
        size_t nettosize = (size_t)_nettosize;

        اگر (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = alignSize(nettosize, (صحیح)اندازه(*refcount));

        تهی* ptr = 0;

        انتخاب (alloc_type)
        {
        حالت PAGE_LOCKED:    cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); بشکن;
        حالت SHARED:         cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  بشکن;
        حالت WRITE_COMBINED: cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); بشکن;
        پیشفرض:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = data =  (uchar*)ptr;
        dataend = data + nettosize;

        refcount = (صحیح*)cv::fastMalloc(اندازه(*refcount));
        *refcount = 1;
    }
#endif
}

HostMem cv::cuda::HostMem::reshape(صحیح new_cn, صحیح new_rows) ثابت
{
    HostMem hdr = *این;

    صحیح cn = channels();
    اگر (new_cn == 0)
        new_cn = cn;

    صحیح total_width = cols * cn;

    اگر ((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * total_width / new_cn;

    اگر (new_rows != 0 && new_rows != rows)
    {
        صحیح total_size = total_width * rows;

        اگر (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        اگر ((unsigned)new_rows > (unsigned)total_size)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        اگر (total_width * new_rows != total_size)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = total_width * elemSize1();
    }

    صحیح new_width = total_width / new_cn;

    اگر (new_width * new_cn != total_width)
        CV_Error(cv::Error::Bad_رقم_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    بازگشت hdr;
}

تهی cv::cuda::HostMem::release()
{
#ifdef HAVE_CUDA
    اگر (refcount && CV_XADD(refcount, -1) == 1)
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
#وگرنه
    CV_Assert( alloc_type == SHARED );

    تهی *pdev;
    cudaSafeCall( cudaHostGetDevicePointer(&pdev, data, 0) );

    بازگشت GpuMat(rows, cols, type(), pdev, step);
#endif
}

تهی cv::cuda::registerPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
    throw_no_cuda();
#وگرنه
    CV_Assert( m.isContinuous() );
    cudaSafeCall( cudaHostRegister(m.data, m.step * m.rows, cudaHostRegisterPortable) );
#endif
}

تهی cv::cuda::unregisterPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
#وگرنه
    cudaSafeCall( cudaHostUnregister(m.data) );
#endif
}
