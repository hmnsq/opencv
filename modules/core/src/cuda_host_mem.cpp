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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ <map>

using ನಾಮಾಕಾಶ cv;
using ನಾಮಾಕಾಶ cv::cuda;

#ifdef HAVE_CUDA

ನಾಮಾಕಾಶ {

ವರ್ಗ HostMemAllocator : ಸಾರ್ವಜನಿಕ MatAllocator
{
ಸಾರ್ವಜನಿಕ:
    explicit HostMemAllocator(unsigned ಪೂರ್ಣಾಙ್ಕ flags) : flags_(flags)
    {
    }

    UMatData* allocate(ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sizes, ಪೂರ್ಣಾಙ್ಕ type,
                       ಶೂನ್ಯ* data0, size_t* step,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ಧ್ರುವ CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = dims-1; i >= 0; i--)
        {
            ಯದಿ (step)
            {
                ಯದಿ (data0 && step[i] != CV_AUTOSTEP)
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                ಅನ್ಯಥಾ
                {
                    step[i] = total;
                }
            }

            total *= sizes[i];
        }

        UMatData* u = ನೂತನ UMatData(ಏತತ್);
        u->size = total;

        ಯದಿ (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
        }
        ಅನ್ಯಥಾ
        {
            ಶೂನ್ಯ* ptr = 0;
            cudaSafeCall( cudaHostAlloc(&ptr, total, flags_) );

            u->data = u->origdata = static_cast<uchar*>(ptr);
        }

        ಪ್ರತ್ಯಾಗಮ u;
    }

    ತಾರ್ಕಿಕ allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ಧ್ರುವ CV_OVERRIDE
    {
        ಪ್ರತ್ಯಾಗಮ (u != ರಿಕ್ತ);
    }

    ಶೂನ್ಯ deallocate(UMatData* u) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ (!u)
            ಪ್ರತ್ಯಾಗಮ;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        ಯದಿ (u->refcount == 0)
        {
            ಯದಿ ( !(u->flags & UMatData::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            delete u;
        }
    }

ಗುಪ್ತ:
    unsigned ಪೂರ್ಣಾಙ್ಕ flags_;
};

} // namespace

#endif

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(alloc_type);
    throw_no_cuda();
#ಅನ್ಯಥಾ
    ಸ್ಥಿರ std::map<unsigned ಪೂರ್ಣಾಙ್ಕ, Ptr<MatAllocator> > allocators;

    unsigned ಪೂರ್ಣಾಙ್ಕ flag = cudaHostAllocDefault;

    ವಿಕಲ್ಪ (alloc_type)
    {
    ಪಕ್ಷ PAGE_LOCKED:    flag = cudaHostAllocDefault; ಭಙ್ಗ;
    ಪಕ್ಷ SHARED:         flag = cudaHostAllocMapped;  ಭಙ್ಗ;
    ಪಕ್ಷ WRITE_COMBINED: flag = cudaHostAllocWriteCombined; ಭಙ್ಗ;
    ಸ್ವಭಾವ:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    ಯದಿ (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    ಪ್ರತ್ಯಾಗಮ a.get();
#endif
}

#ifdef HAVE_CUDA
ನಾಮಾಕಾಶ
{
    size_t alignUpStep(size_t what, size_t alignment)
    {
        size_t alignMask = alignment - 1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        ಪ್ರತ್ಯಾಗಮ res;
    }
}
#endif

ಶೂನ್ಯ cv::cuda::HostMem::create(ಪೂರ್ಣಾಙ್ಕ rows_, ಪೂರ್ಣಾಙ್ಕ cols_, ಪೂರ್ಣಾಙ್ಕ type_)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows_);
    CV_UNUSED(cols_);
    CV_UNUSED(type_);
    throw_no_cuda();
#ಅನ್ಯಥಾ
    ಯದಿ (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.canMapHostMemory() );
    }

    type_ &= Mat::TYPE_MASK;

    ಯದಿ (rows == rows_ && cols == cols_ && type() == type_ && data)
        ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ (data)
        release();

    CV_DbgAssert( rows_ >= 0 && cols_ >= 0 );

    ಯದಿ (rows_ > 0 && cols_ > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        rows = rows_;
        cols = cols_;
        step = elemSize() * cols;
        ಪೂರ್ಣಾಙ್ಕ sz[] = { rows, cols };
        size_t steps[] = { step, (size_t)CV_ELEM_SIZE(type_) };
        flags = updateContinuityFlag(flags, 2, sz, steps);

        ಯದಿ (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            step = alignUpStep(step, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)step*rows;
        size_t nettosize = (size_t)_nettosize;

        ಯದಿ (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = alignSize(nettosize, (ಪೂರ್ಣಾಙ್ಕ)ಪರಿಮಾಣ(*refcount));

        ಶೂನ್ಯ* ptr = 0;

        ವಿಕಲ್ಪ (alloc_type)
        {
        ಪಕ್ಷ PAGE_LOCKED:    cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); ಭಙ್ಗ;
        ಪಕ್ಷ SHARED:         cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  ಭಙ್ಗ;
        ಪಕ್ಷ WRITE_COMBINED: cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); ಭಙ್ಗ;
        ಸ್ವಭಾವ:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = data =  (uchar*)ptr;
        dataend = data + nettosize;

        refcount = (ಪೂರ್ಣಾಙ್ಕ*)cv::fastMalloc(ಪರಿಮಾಣ(*refcount));
        *refcount = 1;
    }
#endif
}

HostMem cv::cuda::HostMem::reshape(ಪೂರ್ಣಾಙ್ಕ new_cn, ಪೂರ್ಣಾಙ್ಕ new_rows) ಧ್ರುವ
{
    HostMem hdr = *ಏತತ್;

    ಪೂರ್ಣಾಙ್ಕ cn = channels();
    ಯದಿ (new_cn == 0)
        new_cn = cn;

    ಪೂರ್ಣಾಙ್ಕ total_width = cols * cn;

    ಯದಿ ((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * total_width / new_cn;

    ಯದಿ (new_rows != 0 && new_rows != rows)
    {
        ಪೂರ್ಣಾಙ್ಕ total_size = total_width * rows;

        ಯದಿ (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        ಯದಿ ((unsigned)new_rows > (unsigned)total_size)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        ಯದಿ (total_width * new_rows != total_size)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = total_width * elemSize1();
    }

    ಪೂರ್ಣಾಙ್ಕ new_width = total_width / new_cn;

    ಯದಿ (new_width * new_cn != total_width)
        CV_Error(cv::Error::Bad_ಅಂಕೆ_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    ಪ್ರತ್ಯಾಗಮ hdr;
}

ಶೂನ್ಯ cv::cuda::HostMem::release()
{
#ifdef HAVE_CUDA
    ಯದಿ (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = data = datastart = 0;
    step = rows = cols = 0;
    refcount = 0;
#endif
}

GpuMat cv::cuda::HostMem::createGpuMatHeader() ಧ್ರುವ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ಅನ್ಯಥಾ
    CV_Assert( alloc_type == SHARED );

    ಶೂನ್ಯ *pdev;
    cudaSafeCall( cudaHostGetDevicePointer(&pdev, data, 0) );

    ಪ್ರತ್ಯಾಗಮ GpuMat(rows, cols, type(), pdev, step);
#endif
}

ಶೂನ್ಯ cv::cuda::registerPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
    throw_no_cuda();
#ಅನ್ಯಥಾ
    CV_Assert( m.isContinuous() );
    cudaSafeCall( cudaHostRegister(m.data, m.step * m.rows, cudaHostRegisterPortable) );
#endif
}

ಶೂನ್ಯ cv::cuda::unregisterPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
#ಅನ್ಯಥಾ
    cudaSafeCall( cudaHostUnregister(m.data) );
#endif
}
