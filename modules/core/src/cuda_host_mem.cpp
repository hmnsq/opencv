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

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव <map>

using नामाकाश cv;
using नामाकाश cv::cuda;

#ifdef HAVE_CUDA

नामाकाश {

वर्ग HostMemAllocator : सार्वजनिक MatAllocator
{
सार्वजनिक:
    explicit HostMemAllocator(unsigned पूर्णाङ्क flags) : flags_(flags)
    {
    }

    UMatData* allocate(पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type,
                       शून्य* data0, size_t* step,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        size_t total = CV_ELEM_SIZE(type);
        प्रति (पूर्णाङ्क i = dims-1; i >= 0; i--)
        {
            जइ (step)
            {
                जइ (data0 && step[i] != CV_AUTOSTEP)
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                अण्णहा
                {
                    step[i] = total;
                }
            }

            total *= sizes[i];
        }

        UMatData* u = नूतन UMatData(एतत्);
        u->size = total;

        जइ (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
        }
        अण्णहा
        {
            शून्य* ptr = 0;
            cudaSafeCall( cudaHostAlloc(&ptr, total, flags_) );

            u->data = u->origdata = static_cast<uchar*>(ptr);
        }

        प्रत्यागम u;
    }

    तार्किक allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        प्रत्यागम (u != रिक्त);
    }

    शून्य deallocate(UMatData* u) ध्रुव CV_OVERRIDE
    {
        जइ (!u)
            प्रत्यागम;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        जइ (u->refcount == 0)
        {
            जइ ( !(u->flags & UMatData::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            delete u;
        }
    }

गुप्त:
    unsigned पूर्णाङ्क flags_;
};

} // namespace

#endif

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(alloc_type);
    throw_no_cuda();
#अण्णहा
    स्थिर std::map<unsigned पूर्णाङ्क, Ptr<MatAllocator> > allocators;

    unsigned पूर्णाङ्क flag = cudaHostAllocDefault;

    विकल्प (alloc_type)
    {
    पक्ष PAGE_LOCKED:    flag = cudaHostAllocDefault; भङ्ग;
    पक्ष SHARED:         flag = cudaHostAllocMapped;  भङ्ग;
    पक्ष WRITE_COMBINED: flag = cudaHostAllocWriteCombined; भङ्ग;
    स्वभाव:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    जइ (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    प्रत्यागम a.get();
#endif
}

#ifdef HAVE_CUDA
नामाकाश
{
    size_t alignUpStep(size_t what, size_t alignment)
    {
        size_t alignMask = alignment - 1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        प्रत्यागम res;
    }
}
#endif

शून्य cv::cuda::HostMem::create(पूर्णाङ्क rows_, पूर्णाङ्क cols_, पूर्णाङ्क type_)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows_);
    CV_UNUSED(cols_);
    CV_UNUSED(type_);
    throw_no_cuda();
#अण्णहा
    जइ (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.canMapHostMemory() );
    }

    type_ &= Mat::TYPE_MASK;

    जइ (rows == rows_ && cols == cols_ && type() == type_ && data)
        प्रत्यागम;

    जइ (data)
        release();

    CV_DbgAssert( rows_ >= 0 && cols_ >= 0 );

    जइ (rows_ > 0 && cols_ > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        rows = rows_;
        cols = cols_;
        step = elemSize() * cols;
        पूर्णाङ्क sz[] = { rows, cols };
        size_t steps[] = { step, (size_t)CV_ELEM_SIZE(type_) };
        flags = updateContinuityFlag(flags, 2, sz, steps);

        जइ (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            step = alignUpStep(step, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)step*rows;
        size_t nettosize = (size_t)_nettosize;

        जइ (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = alignSize(nettosize, (पूर्णाङ्क)परिमाण(*refcount));

        शून्य* ptr = 0;

        विकल्प (alloc_type)
        {
        पक्ष PAGE_LOCKED:    cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); भङ्ग;
        पक्ष SHARED:         cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  भङ्ग;
        पक्ष WRITE_COMBINED: cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); भङ्ग;
        स्वभाव:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = data =  (uchar*)ptr;
        dataend = data + nettosize;

        refcount = (पूर्णाङ्क*)cv::fastMalloc(परिमाण(*refcount));
        *refcount = 1;
    }
#endif
}

HostMem cv::cuda::HostMem::reshape(पूर्णाङ्क new_cn, पूर्णाङ्क new_rows) ध्रुव
{
    HostMem hdr = *एतत्;

    पूर्णाङ्क cn = channels();
    जइ (new_cn == 0)
        new_cn = cn;

    पूर्णाङ्क total_width = cols * cn;

    जइ ((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * total_width / new_cn;

    जइ (new_rows != 0 && new_rows != rows)
    {
        पूर्णाङ्क total_size = total_width * rows;

        जइ (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        जइ ((unsigned)new_rows > (unsigned)total_size)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        जइ (total_width * new_rows != total_size)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = total_width * elemSize1();
    }

    पूर्णाङ्क new_width = total_width / new_cn;

    जइ (new_width * new_cn != total_width)
        CV_Error(cv::Error::Bad_अङ्कः_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    प्रत्यागम hdr;
}

शून्य cv::cuda::HostMem::release()
{
#ifdef HAVE_CUDA
    जइ (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = data = datastart = 0;
    step = rows = cols = 0;
    refcount = 0;
#endif
}

GpuMat cv::cuda::HostMem::createGpuMatHeader() ध्रुव
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अण्णहा
    CV_Assert( alloc_type == SHARED );

    शून्य *pdev;
    cudaSafeCall( cudaHostGetDevicePointer(&pdev, data, 0) );

    प्रत्यागम GpuMat(rows, cols, type(), pdev, step);
#endif
}

शून्य cv::cuda::registerPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
    throw_no_cuda();
#अण्णहा
    CV_Assert( m.isContinuous() );
    cudaSafeCall( cudaHostRegister(m.data, m.step * m.rows, cudaHostRegisterPortable) );
#endif
}

शून्य cv::cuda::unregisterPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
#अण्णहा
    cudaSafeCall( cudaHostUnregister(m.data) );
#endif
}
