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

using നാമാകാശ cv;
using നാമാകാശ cv::cuda;

ശൂന്യം cv::cuda::GpuMat::updateContinuityFlag()
{
    പൂർണ്ണസംഖ്യ sz[] = { rows, cols };
    size_t steps[] = { step, elemSize() };
    flags = cv::updateContinuityFlag(flags, 2, sz, steps);
}

cv::cuda::GpuMat::GpuMat(പൂർണ്ണസംഖ്യ rows_, പൂർണ്ണസംഖ്യ cols_, പൂർണ്ണസംഖ്യ type_, ശൂന്യം* data_, size_t step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(rows_), cols(cols_),
    step(step_), data((uchar*)data_), refcount(0),
    datastart((uchar*)data_), dataend((സ്ഥിരം uchar*)data_),
    allocator(defaultAllocator())
{
    size_t minstep = cols * elemSize();

    എങ്കിൽ (step == Mat::AUTO_STEP)
    {
        step = minstep;
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (rows == 1)
            step = minstep;

        CV_DbgAssert( step >= minstep );
    }

    dataend += step * (rows - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(Size size_, പൂർണ്ണസംഖ്യ type_, ശൂന്യം* data_, size_t step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(size_.height), cols(size_.width),
    step(step_), data((uchar*)data_), refcount(0),
    datastart((uchar*)data_), dataend((സ്ഥിരം uchar*)data_),
    allocator(defaultAllocator())
{
    size_t minstep = cols * elemSize();

    എങ്കിൽ (step == Mat::AUTO_STEP)
    {
        step = minstep;
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (rows == 1)
            step = minstep;

        CV_DbgAssert( step >= minstep );
    }

    dataend += step * (rows - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(സ്ഥിരം GpuMat& m, Range rowRange_, Range colRange_)
{
    flags = m.flags;
    step = m.step; refcount = m.refcount;
    data = m.data; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    എങ്കിൽ (rowRange_ == Range::all())
    {
        rows = m.rows;
    }
    അല്ലെങ്കിൽ
    {
        CV_Assert( 0 <= rowRange_.start && rowRange_.start <= rowRange_.end && rowRange_.end <= m.rows );

        rows = rowRange_.size();
        data += step*rowRange_.start;
    }

    എങ്കിൽ (colRange_ == Range::all())
    {
        cols = m.cols;
    }
    അല്ലെങ്കിൽ
    {
        CV_Assert( 0 <= colRange_.start && colRange_.start <= colRange_.end && colRange_.end <= m.cols );

        cols = colRange_.size();
        data += colRange_.start*elemSize();
    }

    എങ്കിൽ (refcount)
        CV_XADD(refcount, 1);

    എങ്കിൽ (rows <= 0 || cols <= 0)
        rows = cols = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(സ്ഥിരം GpuMat& m, Rect roi) :
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

    എങ്കിൽ (refcount)
        CV_XADD(refcount, 1);

    എങ്കിൽ (rows <= 0 || cols <= 0)
        rows = cols = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(പൂർണ്ണസംഖ്യ new_cn, പൂർണ്ണസംഖ്യ new_rows) സ്ഥിരം
{
    GpuMat hdr = *ഇത്;

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

ശൂന്യം cv::cuda::GpuMat::locateROI(Size& wholeSize, Point& ofs) സ്ഥിരം
{
    CV_DbgAssert( step > 0 );

    size_t esz = elemSize();
    ptrdiff_t delta1 = data - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    എങ്കിൽ (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    അല്ലെങ്കിൽ
    {
        ofs.y = static_cast<പൂർണ്ണസംഖ്യ>(delta1 / step);
        ofs.x = static_cast<പൂർണ്ണസംഖ്യ>((delta1 - step * ofs.y) / esz);

        CV_DbgAssert( data == datastart + ofs.y * step + ofs.x * esz );
    }

    size_t minstep = (ofs.x + cols) * esz;

    wholeSize.height = std::max(static_cast<പൂർണ്ണസംഖ്യ>((delta2 - minstep) / step + 1), ofs.y + rows);
    wholeSize.width = std::max(static_cast<പൂർണ്ണസംഖ്യ>((delta2 - step * (wholeSize.height - 1)) / esz), ofs.x + cols);
}

GpuMat& cv::cuda::GpuMat::adjustROI(പൂർണ്ണസംഖ്യ dtop, പൂർണ്ണസംഖ്യ dbottom, പൂർണ്ണസംഖ്യ dleft, പൂർണ്ണസംഖ്യ dright)
{
    Size wholeSize;
    Point ofs;
    locateROI(wholeSize, ofs);

    size_t esz = elemSize();

    പൂർണ്ണസംഖ്യ row1 = std::max(ofs.y - dtop, 0);
    പൂർണ്ണസംഖ്യ row2 = std::min(ofs.y + rows + dbottom, wholeSize.height);

    പൂർണ്ണസംഖ്യ col1 = std::max(ofs.x - dleft, 0);
    പൂർണ്ണസംഖ്യ col2 = std::min(ofs.x + cols + dright, wholeSize.width);

    data += (row1 - ofs.y) * step + (col1 - ofs.x) * esz;
    rows = row2 - row1;
    cols = col2 - col1;

    updateContinuityFlag();
    തിരികെ *ഇത്;
}

നാമാകാശ
{
    template <വർഗ്ഗം ObjType>
    ശൂന്യം createContinuousImpl(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ type, ObjType& obj)
    {
        സ്ഥിരം പൂർണ്ണസംഖ്യ area = rows * cols;

        എങ്കിൽ (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), rows);
    }
}

ശൂന്യം cv::cuda::createContinuous(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ type, OutputArray arr)
{
    തിരഞ്ഞെടുപ്പ് (arr.kind())
    {
    അവസ്ഥ നിവേശം_Array_2::MAT:
        ::createContinuousImpl(rows, cols, type, arr.getMatRef());
        നിർത്തുക;

    അവസ്ഥ നിവേശം_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(rows, cols, type, arr.getGpuMatRef());
        നിർത്തുക;

    അവസ്ഥ നിവേശം_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(rows, cols, type, arr.getHostMemRef());
        നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
        arr.create(rows, cols, type);
    }
}

നാമാകാശ
{
    template <വർഗ്ഗം ObjType>
    ശൂന്യം ensureSizeIsEnoughImpl(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ type, ObjType& obj)
    {
        എങ്കിൽ (obj.empty() || obj.type() != type || obj.data != obj.datastart)
        {
            obj.create(rows, cols, type);
        }
        അല്ലെങ്കിൽ
        {
            സ്ഥിരം size_t esz = obj.elemSize();
            സ്ഥിരം ptrdiff_t delta2 = obj.dataend - obj.datastart;

            സ്ഥിരം size_t minstep = obj.cols * esz;

            Size wholeSize;
            wholeSize.height = std::max(static_cast<പൂർണ്ണസംഖ്യ>((delta2 - minstep) / static_cast<size_t>(obj.step) + 1), obj.rows);
            wholeSize.width = std::max(static_cast<പൂർണ്ണസംഖ്യ>((delta2 - static_cast<size_t>(obj.step) * (wholeSize.height - 1)) / esz), obj.cols);

            എങ്കിൽ (wholeSize.height < rows || wholeSize.width < cols)
            {
                obj.create(rows, cols, type);
            }
            അല്ലെങ്കിൽ
            {
                obj.cols = cols;
                obj.rows = rows;
            }
        }
    }
}

ശൂന്യം cv::cuda::ensureSizeIsEnough(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ type, OutputArray arr)
{
    തിരഞ്ഞെടുപ്പ് (arr.kind())
    {
    അവസ്ഥ നിവേശം_Array_2::MAT:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getMatRef());
        നിർത്തുക;

    അവസ്ഥ നിവേശം_Array_2::CUDA_GPU_MAT:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getGpuMatRef());
        നിർത്തുക;

    അവസ്ഥ നിവേശം_Array_2::CUDA_HOST_MEM:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getHostMemRef());
        നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
        arr.create(rows, cols, type);
    }
}

GpuMat cv::cuda::get_നിവേശം_Mat(നിവേശം_Array _src, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(_src);
    CV_UNUSED(stream);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    GpuMat src;
    എങ്കിൽ (_src.kind() == നിവേശം_Array_2::CUDA_GPU_MAT)
    {
        src = _src.getGpuMat();
    }
    അല്ലെങ്കിൽ എങ്കിൽ (!_src.empty())
    {
        BufferPool pool(stream);
        src = pool.getBuffer(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    തിരികെ src;
#endif
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ type, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(_dst);
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    CV_UNUSED(stream);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    GpuMat dst;
    എങ്കിൽ (_dst.kind() == നിവേശം_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(rows, cols, type);
        dst = _dst.getGpuMat();
    }
    അല്ലെങ്കിൽ
    {
        BufferPool pool(stream);
        dst = pool.getBuffer(rows, cols, type);
    }
    തിരികെ dst;
#endif
}

ശൂന്യം cv::cuda::syncOutput(സ്ഥിരം GpuMat& dst, OutputArray _dst, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(dst);
    CV_UNUSED(_dst);
    CV_UNUSED(stream);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    എങ്കിൽ (_dst.kind() != നിവേശം_Array_2::CUDA_GPU_MAT)
    {
        എങ്കിൽ (stream)
            dst.download(_dst, stream);
        അല്ലെങ്കിൽ
            dst.download(_dst);
    }
#endif
}

#ifndef HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    തിരികെ 0;
}

ശൂന്യം cv::cuda::GpuMat::setDefaultAllocator(Allocator* allocator)
{
    CV_UNUSED(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    തിരികെ 0;
}

ശൂന്യം cv::cuda::GpuMat::create(പൂർണ്ണസംഖ്യ _rows, പൂർണ്ണസംഖ്യ _cols, പൂർണ്ണസംഖ്യ _type)
{
    CV_UNUSED(_rows);
    CV_UNUSED(_cols);
    CV_UNUSED(_type);
    throw_no_cuda();
}

ശൂന്യം cv::cuda::GpuMat::release()
{
    എങ്കിൽ( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->സ്വതന്ത്രം(ഇത്);
    dataend = datastart = data = 0;
    step = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_MASK;
}

ശൂന്യം cv::cuda::GpuMat::fit(പൂർണ്ണസംഖ്യ _rows, പൂർണ്ണസംഖ്യ _cols, പൂർണ്ണസംഖ്യ _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t oldTotalBytes = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_SIZE(_type);
    size_t newTotalBytes = (size_t)_rows * (size_t)_cols * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    എങ്കിൽ (newTotalBytes > 0 && (!isContinuous() || newTotalBytes > oldTotalBytes || data != datastart))
    {
        create(_rows, _cols, _type);
        തിരികെ;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_MASK) | CV_MAKETYPE(CV_MAT_DEPTH(_type), CV_MAT_CN(_type));
    rows = _rows;
    cols = _cols;
    step = (size_t)cols * esz;
}

ശൂന്യം cv::cuda::GpuMat::fit(Size _sz, പൂർണ്ണസംഖ്യ _type)
{
    fit(_sz.height, _sz.width, _type);
}

ശൂന്യം cv::cuda::GpuMat::upload(നിവേശം_Array arr)
{
    CV_UNUSED(arr);
    throw_no_cuda();
}

ശൂന്യം cv::cuda::GpuMat::upload(നിവേശം_Array arr, Stream& _stream)
{
    CV_UNUSED(arr);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

ശൂന്യം cv::cuda::GpuMat::download(OutputArray _dst) സ്ഥിരം
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

ശൂന്യം cv::cuda::GpuMat::download(OutputArray _dst, Stream& _stream) സ്ഥിരം
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

ശൂന്യം cv::cuda::GpuMat::copyTo(OutputArray _dst) സ്ഥിരം
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

ശൂന്യം cv::cuda::GpuMat::copyTo(OutputArray _dst, Stream& _stream) സ്ഥിരം
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

ശൂന്യം cv::cuda::GpuMat::copyTo(OutputArray _dst, നിവേശം_Array _mask, Stream& _stream) സ്ഥിരം
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

GpuMat& cv::cuda::GpuMat::setTo(Scalar s, നിവേശം_Array _mask, Stream& _stream)
{
    CV_UNUSED(s);
    CV_UNUSED(_mask);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

ശൂന്യം cv::cuda::GpuMat::convertTo(OutputArray _dst, പൂർണ്ണസംഖ്യ rtype, Stream& _stream) സ്ഥിരം
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

ശൂന്യം cv::cuda::GpuMat::convertTo(OutputArray _dst, പൂർണ്ണസംഖ്യ rtype, ഇരട്ട alpha, ഇരട്ട beta, Stream& _stream) സ്ഥിരം
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(alpha);
    CV_UNUSED(beta);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

#endif
