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
    explicit HostMemAllocator(अचिन्हित पूर्णांक flags) : flags_(flags)
    {
    }

    UMatData* allocate(पूर्णांक dims, स्थिरांक पूर्णांक* sizes, पूर्णांक type,
                       शून्य* data0, माप_प्रकार* step,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) स्थिरांक CV_OVERRIDE
    {
        माप_प्रकार total = CV_ELEM_SIZE(type);
        साठी (पूर्णांक i = dims-1; i >= 0; i--)
        {
            जर (step)
            {
                जर (data0 && step[i] != CV_AUTOSTEP)
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                अन्यथा
                {
                    step[i] = total;
                }
            }

            total *= sizes[i];
        }

        UMatData* u = नवीन UMatData(हे);
        u->size = total;

        जर (data0)
        {
            u->data = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMatData::USER_ALLOCATED;
        }
        अन्यथा
        {
            शून्य* ptr = 0;
            cudaSafeCall( cudaHostAlloc(&ptr, total, flags_) );

            u->data = u->origdata = static_cast<uchar*>(ptr);
        }

        परत u;
    }

    तार्किक allocate(UMatData* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) स्थिरांक CV_OVERRIDE
    {
        परत (u != रिक्त);
    }

    शून्य deallocate(UMatData* u) स्थिरांक CV_OVERRIDE
    {
        जर (!u)
            परत;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        जर (u->refcount == 0)
        {
            जर ( !(u->flags & UMatData::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            delete u;
        }
    }

खाजगी:
    अचिन्हित पूर्णांक flags_;
};

} // namespace

#endif

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(alloc_type);
    throw_no_cuda();
#अन्यथा
    स्थिर std::map<अचिन्हित पूर्णांक, Ptr<MatAllocator> > allocators;

    अचिन्हित पूर्णांक flag = cudaHostAllocDefault;

    निवड (alloc_type)
    {
    स्थिती PAGE_LOCKED:    flag = cudaHostAllocDefault; थांबा;
    स्थिती SHARED:         flag = cudaHostAllocMapped;  थांबा;
    स्थिती WRITE_COMBINED: flag = cudaHostAllocWriteCombined; थांबा;
    पूर्वनिर्धारित:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    जर (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    परत a.get();
#endif
}

#ifdef HAVE_CUDA
नामाकाश
{
    माप_प्रकार alignUpStep(माप_प्रकार what, माप_प्रकार alignment)
    {
        माप_प्रकार alignMask = alignment - 1;
        माप_प्रकार inverseAlignMask = ~alignMask;
        माप_प्रकार res = (what + alignMask) & inverseAlignMask;
        परत res;
    }
}
#endif

शून्य cv::cuda::HostMem::create(पूर्णांक rows_, पूर्णांक cols_, पूर्णांक type_)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows_);
    CV_UNUSED(cols_);
    CV_UNUSED(type_);
    throw_no_cuda();
#अन्यथा
    जर (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.canMapHostMemory() );
    }

    type_ &= Mat::TYPE_MASK;

    जर (rows == rows_ && cols == cols_ && type() == type_ && data)
        परत;

    जर (data)
        release();

    CV_DbgAssert( rows_ >= 0 && cols_ >= 0 );

    जर (rows_ > 0 && cols_ > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        rows = rows_;
        cols = cols_;
        step = elemSize() * cols;
        पूर्णांक sz[] = { rows, cols };
        माप_प्रकार steps[] = { step, (माप_प्रकार)CV_ELEM_SIZE(type_) };
        flags = updateContinuityFlag(flags, 2, sz, steps);

        जर (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            step = alignUpStep(step, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)step*rows;
        माप_प्रकार nettosize = (माप_प्रकार)_nettosize;

        जर (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        माप_प्रकार datasize = alignSize(nettosize, (पूर्णांक)आकार(*refcount));

        शून्य* ptr = 0;

        निवड (alloc_type)
        {
        स्थिती PAGE_LOCKED:    cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); थांबा;
        स्थिती SHARED:         cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  थांबा;
        स्थिती WRITE_COMBINED: cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); थांबा;
        पूर्वनिर्धारित:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = data =  (uchar*)ptr;
        dataend = data + nettosize;

        refcount = (पूर्णांक*)cv::fastMalloc(आकार(*refcount));
        *refcount = 1;
    }
#endif
}

HostMem cv::cuda::HostMem::reshape(पूर्णांक new_cn, पूर्णांक new_rows) स्थिरांक
{
    HostMem hdr = *हे;

    पूर्णांक cn = channels();
    जर (new_cn == 0)
        new_cn = cn;

    पूर्णांक total_width = cols * cn;

    जर ((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * total_width / new_cn;

    जर (new_rows != 0 && new_rows != rows)
    {
        पूर्णांक total_size = total_width * rows;

        जर (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        जर ((अचिन्हित)new_rows > (अचिन्हित)total_size)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        जर (total_width * new_rows != total_size)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = total_width * elemSize1();
    }

    पूर्णांक new_width = total_width / new_cn;

    जर (new_width * new_cn != total_width)
        CV_Error(cv::Error::Bad_अंक_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    परत hdr;
}

शून्य cv::cuda::HostMem::release()
{
#ifdef HAVE_CUDA
    जर (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = data = datastart = 0;
    step = rows = cols = 0;
    refcount = 0;
#endif
}

GpuMat cv::cuda::HostMem::createGpuMatHeader() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    CV_Assert( alloc_type == SHARED );

    शून्य *pdev;
    cudaSafeCall( cudaHostGetDevicePointer(&pdev, data, 0) );

    परत GpuMat(rows, cols, type(), pdev, step);
#endif
}

शून्य cv::cuda::registerPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
    throw_no_cuda();
#अन्यथा
    CV_Assert( m.isContinuous() );
    cudaSafeCall( cudaHostRegister(m.data, m.step * m.rows, cudaHostRegisterPortable) );
#endif
}

शून्य cv::cuda::unregisterPageLocked(Mat& m)
{
#ifndef HAVE_CUDA
    CV_UNUSED(m);
#अन्यथा
    cudaSafeCall( cudaHostUnregister(m.data) );
#endif
}
