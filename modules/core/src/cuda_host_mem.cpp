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

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ <map>

using நாமாகாஶ cv;
using நாமாகாஶ cv::cuda;

#ifdef HAVE_CUDA

நாமாகாஶ {

வகுப்பு HostMemAllocator : பொது MatAllocator
{
பொது:
    explicit HostMemAllocator(unsigned முழு flags) : flags_(flags)
    {
    }

    UMatData* allocate(முழு dims, மாறிலி முழு* sizes, முழு type,
                       வெறுமை* data0, size_t* step,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) மாறிலி CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        க்கு (முழு i = dims-1; i >= 0; i--)
        {
            எனில் (step)
            {
                எனில் (data0 && step[i] != CV_AUTOSTEP)
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                இல்லையெனில்
                {
                    step[i] = total;
                }
            }

            total *= sizes[i];
        }

        UMatData* u = புதிய UMatData(இது);
        u->size = total;

        எனில் (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
        }
        இல்லையெனில்
        {
            வெறுமை* ptr = 0;
            cudaSafeCall( cudaHostAlloc(&ptr, total, flags_) );

            u->data = u->origdata = static_cast<uchar*>(ptr);
        }

        திரும்பு u;
    }

    தருக்க allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) மாறிலி CV_OVERRIDE
    {
        திரும்பு (u != வெற்று);
    }

    வெறுமை deallocate(UMatData* u) மாறிலி CV_OVERRIDE
    {
        எனில் (!u)
            திரும்பு;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        எனில் (u->refcount == 0)
        {
            எனில் ( !(u->flags & UMatData::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            delete u;
        }
    }

தனிப்பட்ட:
    unsigned முழு flags_;
};

} // namespace

#endif

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(alloc_type);
    throw_no_cuda();
#இல்லையெனில்
    நிலையான std::map<unsigned முழு, Ptr<MatAllocator> > allocators;

    unsigned முழு flag = cudaHostAllocDefault;

    தேர்வு (alloc_type)
    {
    நிலை PAGE_LOCKED:    flag = cudaHostAllocDefault; நிறுத்து;
    நிலை SHARED:         flag = cudaHostAllocMapped;  நிறுத்து;
    நிலை WRITE_COMBINED: flag = cudaHostAllocWriteCombined; நிறுத்து;
    இயல்பு:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    எனில் (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    திரும்பு a.get();
#endif
}

#ifdef HAVE_CUDA
நாமாகாஶ
{
    size_t alignUpStep(size_t what, size_t alignment)
    {
        size_t alignMask = alignment - 1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        திரும்பு res;
    }
}
#endif

வெறுமை cv::cuda::HostMem::create(முழு rows_, முழு cols_, முழு type_)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows_);
    CV_UNUSED(cols_);
    CV_UNUSED(type_);
    throw_no_cuda();
#இல்லையெனில்
    எனில் (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.canMapHostMemory() );
    }

    type_ &= Mat::TYPE_MASK;

    எனில் (rows == rows_ && cols == cols_ && type() == type_ && data)
        திரும்பு;

    எனில் (data)
        release();

    CV_DbgAssert( rows_ >= 0 && cols_ >= 0 );

    எனில் (rows_ > 0 && cols_ > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        rows = rows_;
        cols = cols_;
        step = elemSize() * cols;
        முழு sz[] = { rows, cols };
        size_t steps[] = { step, (size_t)CV_ELEM_SIZE(type_) };
        flags = updateContinuityFlag(flags, 2, sz, steps);

        எனில் (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            step = alignUpStep(step, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)step*rows;
        size_t nettosize = (size_t)_nettosize;

        எனில் (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = alignSize(nettosize, (முழு)அளவு(*refcount));

        வெறுமை* ptr = 0;

        தேர்வு (alloc_type)
        {
        நிலை PAGE_LOCKED:    cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); நிறுத்து;
        நிலை SHARED:         cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  நிறுத்து;
        நிலை WRITE_COMBINED: cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); நிறுத்து;
        இயல்பு:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = data =  (uchar*)ptr;
        dataend = data + nettosize;

        refcount = (முழு*)cv::fastMalloc(அளவு(*refcount));
        *refcount = 1;
    }
#endif
}

HostMem cv::cuda::HostMem::reshape(முழு new_cn, முழு new_rows) மாறிலி
{
    HostMem hdr = *இது;

    முழு cn = channels();
    எனில் (new_cn == 0)
        new_cn = cn;

    முழு total_width = cols * cn;

    எனில் ((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * total_width / new_cn;

    எனில் (new_rows != 0 && new_rows != rows)
    {
        முழு total_size = total_width * rows;

        எனில் (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        எனில் ((unsigned)new_rows > (unsigned)total_size)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        எனில் (total_width * new_rows != total_size)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = total_width * elemSize1();
    }

    முழு new_width = total_width / new_cn;

    எனில் (new_width * new_cn != total_width)
        CV_Error(cv::Error::Bad_இலக்கம்_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    திரும்பு hdr;
}

வெறுமை cv::cuda::HostMem::release()
{
#ifdef HAVE_CUDA
    எனில் (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = data = datastart = 0;
    step = rows = cols = 0;
    refcount = 0;
#endif
}

GpuMat cv::cuda::HostMem::createGpuMatHeader() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    CV_Assert( alloc_type == SHARED );

    வெறுமை *pdev;
    cudaSafeCall( cudaHostGetDevicePointer(&pdev, data, 0) );

    திரும்பு GpuMat(rows, cols, type(), pdev, step);
#endif
}

வெறுமை cv::cuda::registerPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
    throw_no_cuda();
#இல்லையெனில்
    CV_Assert( m.isContinuous() );
    cudaSafeCall( cudaHostRegister(m.data, m.step * m.rows, cudaHostRegisterPortable) );
#endif
}

வெறுமை cv::cuda::unregisterPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
#இல்லையெனில்
    cudaSafeCall( cudaHostUnregister(m.data) );
#endif
}
