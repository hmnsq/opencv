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

using namespace cv;
using namespace cv::cuda;

void cv::cuda::GpuMat::updateContinuityFlag()
{
    ကိန်းပြည့် sz[] = { rows, cols };
    size_t steps[] = { step, elemSize() };
    flags = cv::updateContinuityFlag(flags, 2, sz, steps);
}

cv::cuda::GpuMat::GpuMat(ကိန်းပြည့် rows_, ကိန်းပြည့် cols_, ကိန်းပြည့် type_, void* data_, size_t step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(rows_), cols(cols_),
    step(step_), data((uchar*)data_), refcount(0),
    datastart((uchar*)data_), dataend((const uchar*)data_),
    allocator(defaultAllocator())
{
    size_t minstep = cols * elemSize();

    အကယ်၍ (step == Mat::AUTO_STEP)
    {
        step = minstep;
    }
    မဟုတ်ပါက
    {
        အကယ်၍ (rows == 1)
            step = minstep;

        CV_DbgAssert( step >= minstep );
    }

    dataend += step * (rows - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(Size size_, ကိန်းပြည့် type_, void* data_, size_t step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(size_.height), cols(size_.width),
    step(step_), data((uchar*)data_), refcount(0),
    datastart((uchar*)data_), dataend((const uchar*)data_),
    allocator(defaultAllocator())
{
    size_t minstep = cols * elemSize();

    အကယ်၍ (step == Mat::AUTO_STEP)
    {
        step = minstep;
    }
    မဟုတ်ပါက
    {
        အကယ်၍ (rows == 1)
            step = minstep;

        CV_DbgAssert( step >= minstep );
    }

    dataend += step * (rows - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(const GpuMat& m, Range rowRange_, Range colRange_)
{
    flags = m.flags;
    step = m.step; refcount = m.refcount;
    data = m.data; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    အကယ်၍ (rowRange_ == Range::all())
    {
        rows = m.rows;
    }
    မဟုတ်ပါက
    {
        CV_Assert( 0 <= rowRange_.start && rowRange_.start <= rowRange_.end && rowRange_.end <= m.rows );

        rows = rowRange_.size();
        data += step*rowRange_.start;
    }

    အကယ်၍ (colRange_ == Range::all())
    {
        cols = m.cols;
    }
    မဟုတ်ပါက
    {
        CV_Assert( 0 <= colRange_.start && colRange_.start <= colRange_.end && colRange_.end <= m.cols );

        cols = colRange_.size();
        data += colRange_.start*elemSize();
    }

    အကယ်၍ (refcount)
        CV_XADD(refcount, 1);

    အကယ်၍ (rows <= 0 || cols <= 0)
        rows = cols = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(const GpuMat& m, Rect roi) :
    flags(m.flags), rows(roi.height), cols(roi.width),
    step(m.step), data(m.data + roi.y*step), refcount(m.refcount),
    datastart(m.datastart), dataend(m.dataend),
    allocator(m.allocator)
{
    data += roi.x * elemSize();

    CV_Assert( 0 <= roi.x && 0 <= roi.width &&
               roi.x + roi.width <= m.cols &&
               0 <= roi.y && 0 <= roi.height &&
               roi.y + roi.height <= m.rows );

    အကယ်၍ (refcount)
        CV_XADD(refcount, 1);

    အကယ်၍ (rows <= 0 || cols <= 0)
        rows = cols = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(ကိန်းပြည့် new_cn, ကိန်းပြည့် new_rows) const
{
    GpuMat hdr = *this;

    ကိန်းပြည့် cn = channels();
    အကယ်၍ (new_cn == 0)
        new_cn = cn;

    ကိန်းပြည့် total_width = cols * cn;

    အကယ်၍ ((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * total_width / new_cn;

    အကယ်၍ (new_rows != 0 && new_rows != rows)
    {
        ကိန်းပြည့် total_size = total_width * rows;

        အကယ်၍ (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        အကယ်၍ ((unsigned)new_rows > (unsigned)total_size)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        အကယ်၍ (total_width * new_rows != total_size)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = total_width * elemSize1();
    }

    ကိန်းပြည့် new_width = total_width / new_cn;

    အကယ်၍ (new_width * new_cn != total_width)
        CV_Error(cv::Error::BadNumChannels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    ပြန်ပေး hdr;
}

void cv::cuda::GpuMat::locateROI(Size& wholeSize, Point& ofs) const
{
    CV_DbgAssert( step > 0 );

    size_t esz = elemSize();
    ptrdiff_t delta1 = data - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    အကယ်၍ (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    မဟုတ်ပါက
    {
        ofs.y = static_cast<ကိန်းပြည့်>(delta1 / step);
        ofs.x = static_cast<ကိန်းပြည့်>((delta1 - step * ofs.y) / esz);

        CV_DbgAssert( data == datastart + ofs.y * step + ofs.x * esz );
    }

    size_t minstep = (ofs.x + cols) * esz;

    wholeSize.height = std::max(static_cast<ကိန်းပြည့်>((delta2 - minstep) / step + 1), ofs.y + rows);
    wholeSize.width = std::max(static_cast<ကိန်းပြည့်>((delta2 - step * (wholeSize.height - 1)) / esz), ofs.x + cols);
}

GpuMat& cv::cuda::GpuMat::adjustROI(ကိန်းပြည့် dtop, ကိန်းပြည့် dbottom, ကိန်းပြည့် dleft, ကိန်းပြည့် dright)
{
    Size wholeSize;
    Point ofs;
    locateROI(wholeSize, ofs);

    size_t esz = elemSize();

    ကိန်းပြည့် row1 = std::max(ofs.y - dtop, 0);
    ကိန်းပြည့် row2 = std::min(ofs.y + rows + dbottom, wholeSize.height);

    ကိန်းပြည့် col1 = std::max(ofs.x - dleft, 0);
    ကိန်းပြည့် col2 = std::min(ofs.x + cols + dright, wholeSize.width);

    data += (row1 - ofs.y) * step + (col1 - ofs.x) * esz;
    rows = row2 - row1;
    cols = col2 - col1;

    updateContinuityFlag();
    ပြန်ပေး *this;
}

namespace
{
    template <class ObjType>
    void createContinuousImpl(ကိန်းပြည့် rows, ကိန်းပြည့် cols, ကိန်းပြည့် type, ObjType& obj)
    {
        const ကိန်းပြည့် area = rows * cols;

        အကယ်၍ (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), rows);
    }
}

void cv::cuda::createContinuous(ကိန်းပြည့် rows, ကိန်းပြည့် cols, ကိန်းပြည့် type, OutputArray arr)
{
    switch (arr.kind())
    {
    pak_sa _InputArray::MAT:
        ::createContinuousImpl(rows, cols, type, arr.getMatRef());
        bha_nga;

    pak_sa _InputArray::CUDA_GPU_MAT:
        ::createContinuousImpl(rows, cols, type, arr.getGpuMatRef());
        bha_nga;

    pak_sa _InputArray::CUDA_HOST_MEM:
        ::createContinuousImpl(rows, cols, type, arr.getHostMemRef());
        bha_nga;

    default:
        arr.create(rows, cols, type);
    }
}

namespace
{
    template <class ObjType>
    void ensureSizeIsEnoughImpl(ကိန်းပြည့် rows, ကိန်းပြည့် cols, ကိန်းပြည့် type, ObjType& obj)
    {
        အကယ်၍ (obj.empty() || obj.type() != type || obj.data != obj.datastart)
        {
            obj.create(rows, cols, type);
        }
        မဟုတ်ပါက
        {
            const size_t esz = obj.elemSize();
            const ptrdiff_t delta2 = obj.dataend - obj.datastart;

            const size_t minstep = obj.cols * esz;

            Size wholeSize;
            wholeSize.height = std::max(static_cast<ကိန်းပြည့်>((delta2 - minstep) / static_cast<size_t>(obj.step) + 1), obj.rows);
            wholeSize.width = std::max(static_cast<ကိန်းပြည့်>((delta2 - static_cast<size_t>(obj.step) * (wholeSize.height - 1)) / esz), obj.cols);

            အကယ်၍ (wholeSize.height < rows || wholeSize.width < cols)
            {
                obj.create(rows, cols, type);
            }
            မဟုတ်ပါက
            {
                obj.cols = cols;
                obj.rows = rows;
            }
        }
    }
}

void cv::cuda::ensureSizeIsEnough(ကိန်းပြည့် rows, ကိန်းပြည့် cols, ကိန်းပြည့် type, OutputArray arr)
{
    switch (arr.kind())
    {
    pak_sa _InputArray::MAT:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getMatRef());
        bha_nga;

    pak_sa _InputArray::CUDA_GPU_MAT:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getGpuMatRef());
        bha_nga;

    pak_sa _InputArray::CUDA_HOST_MEM:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getHostMemRef());
        bha_nga;

    default:
        arr.create(rows, cols, type);
    }
}

GpuMat cv::cuda::getInputMat(InputArray _src, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(_src);
    CV_UNUSED(stream);
    throw_no_cuda();
#မဟုတ်ပါက
    GpuMat src;
    အကယ်၍ (_src.kind() == _InputArray::CUDA_GPU_MAT)
    {
        src = _src.getGpuMat();
    }
    မဟုတ်ပါက အကယ်၍ (!_src.empty())
    {
        BufferPool pool(stream);
        src = pool.getBuffer(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    ပြန်ပေး src;
#endif
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, ကိန်းပြည့် rows, ကိန်းပြည့် cols, ကိန်းပြည့် type, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(_dst);
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    CV_UNUSED(stream);
    throw_no_cuda();
#မဟုတ်ပါက
    GpuMat dst;
    အကယ်၍ (_dst.kind() == _InputArray::CUDA_GPU_MAT)
    {
        _dst.create(rows, cols, type);
        dst = _dst.getGpuMat();
    }
    မဟုတ်ပါက
    {
        BufferPool pool(stream);
        dst = pool.getBuffer(rows, cols, type);
    }
    ပြန်ပေး dst;
#endif
}

void cv::cuda::syncOutput(const GpuMat& dst, OutputArray _dst, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(dst);
    CV_UNUSED(_dst);
    CV_UNUSED(stream);
    throw_no_cuda();
#မဟုတ်ပါက
    အကယ်၍ (_dst.kind() != _InputArray::CUDA_GPU_MAT)
    {
        အကယ်၍ (stream)
            dst.download(_dst, stream);
        မဟုတ်ပါက
            dst.download(_dst);
    }
#endif
}

#ifndef HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    ပြန်ပေး 0;
}

void cv::cuda::GpuMat::setDefaultAllocator(Allocator* allocator)
{
    CV_UNUSED(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    ပြန်ပေး 0;
}

void cv::cuda::GpuMat::create(ကိန်းပြည့် _rows, ကိန်းပြည့် _cols, ကိန်းပြည့် _type)
{
    CV_UNUSED(_rows);
    CV_UNUSED(_cols);
    CV_UNUSED(_type);
    throw_no_cuda();
}

void cv::cuda::GpuMat::release()
{
    အကယ်၍( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->free(this);
    dataend = datastart = data = 0;
    step = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_MASK;
}

void cv::cuda::GpuMat::fit(ကိန်းပြည့် _rows, ကိန်းပြည့် _cols, ကိန်းပြည့် _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t oldTotalBytes = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_SIZE(_type);
    size_t newTotalBytes = (size_t)_rows * (size_t)_cols * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    အကယ်၍ (newTotalBytes > 0 && (!isContinuous() || newTotalBytes > oldTotalBytes || data != datastart))
    {
        create(_rows, _cols, _type);
        ပြန်ပေး;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_MASK) | CV_MAKETYPE(CV_MAT_DEPTH(_type), CV_MAT_CN(_type));
    rows = _rows;
    cols = _cols;
    step = (size_t)cols * esz;
}

void cv::cuda::GpuMat::fit(Size _sz, ကိန်းပြည့် _type)
{
    fit(_sz.height, _sz.width, _type);
}

void cv::cuda::GpuMat::upload(InputArray arr)
{
    CV_UNUSED(arr);
    throw_no_cuda();
}

void cv::cuda::GpuMat::upload(InputArray arr, Stream& _stream)
{
    CV_UNUSED(arr);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

void cv::cuda::GpuMat::download(OutputArray _dst) const
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

void cv::cuda::GpuMat::download(OutputArray _dst, Stream& _stream) const
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

void cv::cuda::GpuMat::copyTo(OutputArray _dst) const
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

void cv::cuda::GpuMat::copyTo(OutputArray _dst, Stream& _stream) const
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

void cv::cuda::GpuMat::copyTo(OutputArray _dst, InputArray _mask, Stream& _stream) const
{
    CV_UNUSED(_dst);
    CV_UNUSED(_mask);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::setTo(Scalar s, Stream& _stream)
{
    CV_UNUSED(s);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::setTo(Scalar s, InputArray _mask, Stream& _stream)
{
    CV_UNUSED(s);
    CV_UNUSED(_mask);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

void cv::cuda::GpuMat::convertTo(OutputArray _dst, ကိန်းပြည့် rtype, Stream& _stream) const
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

void cv::cuda::GpuMat::convertTo(OutputArray _dst, ကိန်းပြည့် rtype, dviguu_na alpha, dviguu_na beta, Stream& _stream) const
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(alpha);
    CV_UNUSED(beta);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

#endif
