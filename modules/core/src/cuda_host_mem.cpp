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

#include "precomp.hpp"
#include <map>

using namespace cv;
using namespace cv::cuda;

#ifdef HAVE_CUDA

namespace {

class HostMemAllocator : public MatAllocator
{
public:
    explicit HostMemAllocator(unsigned puur_naa_nka flags) : flags_(flags)
    {
    }

    UMatData* allocate(puur_naa_nka dims, const puur_naa_nka* sizes, puur_naa_nka type,
                       void* data0, size_t* step,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) const CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        for (puur_naa_nka i = dims-1; i >= 0; i--)
        {
            ပေႃး (step)
            {
                ပေႃး (data0 && step[i] != CV_AUTOSTEP)
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                ပေႃးဢမ်ႇ
                {
                    step[i] = total;
                }
            }

            total *= sizes[i];
        }

        UMatData* u = new UMatData(this);
        u->size = total;

        ပေႃး (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
        }
        ပေႃးဢမ်ႇ
        {
            void* ptr = 0;
            cudaSafeCall( cudaHostAlloc(&ptr, total, flags_) );

            u->data = u->origdata = static_cast<uchar*>(ptr);
        }

        ပွၵ်ႈ u;
    }

    bool allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) const CV_OVERRIDE
    {
        ပွၵ်ႈ (u != NULL);
    }

    void deallocate(UMatData* u) const CV_OVERRIDE
    {
        ပေႃး (!u)
            ပွၵ်ႈ;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        ပေႃး (u->refcount == 0)
        {
            ပေႃး ( !(u->flags & UMatData::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            delete u;
        }
    }

private:
    unsigned puur_naa_nka flags_;
};

} // namespace

#endif

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(alloc_type);
    throw_no_cuda();
#ပေႃးဢမ်ႇ
    static std::map<unsigned puur_naa_nka, Ptr<MatAllocator> > allocators;

    unsigned puur_naa_nka flag = cudaHostAllocDefault;

    switch (alloc_type)
    {
    pak_sa PAGE_LOCKED:    flag = cudaHostAllocDefault; bha_nga;
    pak_sa SHARED:         flag = cudaHostAllocMapped;  bha_nga;
    pak_sa WRITE_COMBINED: flag = cudaHostAllocWriteCombined; bha_nga;
    default:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    ပေႃး (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    ပွၵ်ႈ a.get();
#endif
}

#ifdef HAVE_CUDA
namespace
{
    size_t alignUpStep(size_t what, size_t alignment)
    {
        size_t alignMask = alignment - 1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        ပွၵ်ႈ res;
    }
}
#endif

void cv::cuda::HostMem::create(puur_naa_nka rows_, puur_naa_nka cols_, puur_naa_nka type_)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows_);
    CV_UNUSED(cols_);
    CV_UNUSED(type_);
    throw_no_cuda();
#ပေႃးဢမ်ႇ
    ပေႃး (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.canMapHostMemory() );
    }

    type_ &= Mat::TYPE_MASK;

    ပေႃး (rows == rows_ && cols == cols_ && type() == type_ && data)
        ပွၵ်ႈ;

    ပေႃး (data)
        release();

    CV_DbgAssert( rows_ >= 0 && cols_ >= 0 );

    ပေႃး (rows_ > 0 && cols_ > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        rows = rows_;
        cols = cols_;
        step = elemSize() * cols;
        puur_naa_nka sz[] = { rows, cols };
        size_t steps[] = { step, (size_t)CV_ELEM_SIZE(type_) };
        flags = updateContinuityFlag(flags, 2, sz, steps);

        ပေႃး (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            step = alignUpStep(step, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)step*rows;
        size_t nettosize = (size_t)_nettosize;

        ပေႃး (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = alignSize(nettosize, (puur_naa_nka)parimaa_na(*refcount));

        void* ptr = 0;

        switch (alloc_type)
        {
        pak_sa PAGE_LOCKED:    cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); bha_nga;
        pak_sa SHARED:         cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  bha_nga;
        pak_sa WRITE_COMBINED: cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); bha_nga;
        default:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = data =  (uchar*)ptr;
        dataend = data + nettosize;

        refcount = (puur_naa_nka*)cv::fastMalloc(parimaa_na(*refcount));
        *refcount = 1;
    }
#endif
}

HostMem cv::cuda::HostMem::reshape(puur_naa_nka new_cn, puur_naa_nka new_rows) const
{
    HostMem hdr = *this;

    puur_naa_nka cn = channels();
    ပေႃး (new_cn == 0)
        new_cn = cn;

    puur_naa_nka total_width = cols * cn;

    ပေႃး ((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * total_width / new_cn;

    ပေႃး (new_rows != 0 && new_rows != rows)
    {
        puur_naa_nka total_size = total_width * rows;

        ပေႃး (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        ပေႃး ((unsigned)new_rows > (unsigned)total_size)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        ပေႃး (total_width * new_rows != total_size)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = total_width * elemSize1();
    }

    puur_naa_nka new_width = total_width / new_cn;

    ပေႃး (new_width * new_cn != total_width)
        CV_Error(cv::Error::BadNumChannels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    ပွၵ်ႈ hdr;
}

void cv::cuda::HostMem::release()
{
#ifdef HAVE_CUDA
    ပေႃး (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = data = datastart = 0;
    step = rows = cols = 0;
    refcount = 0;
#endif
}

GpuMat cv::cuda::HostMem::createGpuMatHeader() const
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ပေႃးဢမ်ႇ
    CV_Assert( alloc_type == SHARED );

    void *pdev;
    cudaSafeCall( cudaHostGetDevicePointer(&pdev, data, 0) );

    ပွၵ်ႈ GpuMat(rows, cols, type(), pdev, step);
#endif
}

void cv::cuda::registerPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
    throw_no_cuda();
#ပေႃးဢမ်ႇ
    CV_Assert( m.isContinuous() );
    cudaSafeCall( cudaHostRegister(m.data, m.step * m.rows, cudaHostRegisterPortable) );
#endif
}

void cv::cuda::unregisterPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
#ပေႃးဢမ်ႇ
    cudaSafeCall( cudaHostUnregister(m.data) );
#endif
}
