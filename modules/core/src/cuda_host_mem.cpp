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

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ <map>

using നാമാകാശ cv;
using നാമാകാശ cv::cuda;

#ifdef HAVE_CUDA

നാമാകാശ {

വർഗ്ഗം HostMemAllocator : പൊതു MatAllocator
{
പൊതു:
    explicit HostMemAllocator(unsigned പൂർണ്ണസംഖ്യ flags) : flags_(flags)
    {
    }

    UMatData* allocate(പൂർണ്ണസംഖ്യ dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* sizes, പൂർണ്ണസംഖ്യ type,
                       ശൂന്യം* data0, size_t* step,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) സ്ഥിരം CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = dims-1; i >= 0; i--)
        {
            എങ്കിൽ (step)
            {
                എങ്കിൽ (data0 && step[i] != CV_AUTOSTEP)
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                അല്ലെങ്കിൽ
                {
                    step[i] = total;
                }
            }

            total *= sizes[i];
        }

        UMatData* u = പുതിയ UMatData(ഇത്);
        u->size = total;

        എങ്കിൽ (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
        }
        അല്ലെങ്കിൽ
        {
            ശൂന്യം* ptr = 0;
            cudaSafeCall( cudaHostAlloc(&ptr, total, flags_) );

            u->data = u->origdata = static_cast<uchar*>(ptr);
        }

        തിരികെ u;
    }

    തർക്കം allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) സ്ഥിരം CV_OVERRIDE
    {
        തിരികെ (u != ശൂന്യ);
    }

    ശൂന്യം deallocate(UMatData* u) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ (!u)
            തിരികെ;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        എങ്കിൽ (u->refcount == 0)
        {
            എങ്കിൽ ( !(u->flags & UMatData::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            delete u;
        }
    }

സ്വകാര്യ:
    unsigned പൂർണ്ണസംഖ്യ flags_;
};

} // namespace

#endif

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(alloc_type);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    സ്ഥിതം std::map<unsigned പൂർണ്ണസംഖ്യ, Ptr<MatAllocator> > allocators;

    unsigned പൂർണ്ണസംഖ്യ flag = cudaHostAllocDefault;

    തിരഞ്ഞെടുപ്പ് (alloc_type)
    {
    അവസ്ഥ PAGE_LOCKED:    flag = cudaHostAllocDefault; നിർത്തുക;
    അവസ്ഥ SHARED:         flag = cudaHostAllocMapped;  നിർത്തുക;
    അവസ്ഥ WRITE_COMBINED: flag = cudaHostAllocWriteCombined; നിർത്തുക;
    സ്ഥിരസ്ഥിതി:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    എങ്കിൽ (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    തിരികെ a.get();
#endif
}

#ifdef HAVE_CUDA
നാമാകാശ
{
    size_t alignUpStep(size_t what, size_t alignment)
    {
        size_t alignMask = alignment - 1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        തിരികെ res;
    }
}
#endif

ശൂന്യം cv::cuda::HostMem::create(പൂർണ്ണസംഖ്യ rows_, പൂർണ്ണസംഖ്യ cols_, പൂർണ്ണസംഖ്യ type_)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows_);
    CV_UNUSED(cols_);
    CV_UNUSED(type_);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    എങ്കിൽ (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.canMapHostMemory() );
    }

    type_ &= Mat::TYPE_MASK;

    എങ്കിൽ (rows == rows_ && cols == cols_ && type() == type_ && data)
        തിരികെ;

    എങ്കിൽ (data)
        release();

    CV_DbgAssert( rows_ >= 0 && cols_ >= 0 );

    എങ്കിൽ (rows_ > 0 && cols_ > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        rows = rows_;
        cols = cols_;
        step = elemSize() * cols;
        പൂർണ്ണസംഖ്യ sz[] = { rows, cols };
        size_t steps[] = { step, (size_t)CV_ELEM_SIZE(type_) };
        flags = updateContinuityFlag(flags, 2, sz, steps);

        എങ്കിൽ (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            step = alignUpStep(step, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)step*rows;
        size_t nettosize = (size_t)_nettosize;

        എങ്കിൽ (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = alignSize(nettosize, (പൂർണ്ണസംഖ്യ)വലുപ്പം(*refcount));

        ശൂന്യം* ptr = 0;

        തിരഞ്ഞെടുപ്പ് (alloc_type)
        {
        അവസ്ഥ PAGE_LOCKED:    cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); നിർത്തുക;
        അവസ്ഥ SHARED:         cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  നിർത്തുക;
        അവസ്ഥ WRITE_COMBINED: cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); നിർത്തുക;
        സ്ഥിരസ്ഥിതി:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = data =  (uchar*)ptr;
        dataend = data + nettosize;

        refcount = (പൂർണ്ണസംഖ്യ*)cv::fastMalloc(വലുപ്പം(*refcount));
        *refcount = 1;
    }
#endif
}

HostMem cv::cuda::HostMem::reshape(പൂർണ്ണസംഖ്യ new_cn, പൂർണ്ണസംഖ്യ new_rows) സ്ഥിരം
{
    HostMem hdr = *ഇത്;

    പൂർണ്ണസംഖ്യ cn = channels();
    എങ്കിൽ (new_cn == 0)
        new_cn = cn;

    പൂർണ്ണസംഖ്യ total_width = cols * cn;

    എങ്കിൽ ((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * total_width / new_cn;

    എങ്കിൽ (new_rows != 0 && new_rows != rows)
    {
        പൂർണ്ണസംഖ്യ total_size = total_width * rows;

        എങ്കിൽ (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        എങ്കിൽ ((unsigned)new_rows > (unsigned)total_size)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        എങ്കിൽ (total_width * new_rows != total_size)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = total_width * elemSize1();
    }

    പൂർണ്ണസംഖ്യ new_width = total_width / new_cn;

    എങ്കിൽ (new_width * new_cn != total_width)
        CV_Error(cv::Error::Bad_അക്കം_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    തിരികെ hdr;
}

ശൂന്യം cv::cuda::HostMem::release()
{
#ifdef HAVE_CUDA
    എങ്കിൽ (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = data = datastart = 0;
    step = rows = cols = 0;
    refcount = 0;
#endif
}

GpuMat cv::cuda::HostMem::createGpuMatHeader() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    CV_Assert( alloc_type == SHARED );

    ശൂന്യം *pdev;
    cudaSafeCall( cudaHostGetDevicePointer(&pdev, data, 0) );

    തിരികെ GpuMat(rows, cols, type(), pdev, step);
#endif
}

ശൂന്യം cv::cuda::registerPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    CV_Assert( m.isContinuous() );
    cudaSafeCall( cudaHostRegister(m.data, m.step * m.rows, cudaHostRegisterPortable) );
#endif
}

ശൂന്യം cv::cuda::unregisterPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
#അല്ലെങ്കിൽ
    cudaSafeCall( cudaHostUnregister(m.data) );
#endif
}
