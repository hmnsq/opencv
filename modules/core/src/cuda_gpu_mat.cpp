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

#অন্তর্ভাব "precomp.hpp"

using নামাকাশ cv;
using নামাকাশ cv::cuda;

শূন্য cv::cuda::GpuMat::updateContinuityFlag()
{
    পূর্ণাঙ্ক sz[] = { rows, cols };
    size_t steps[] = { step, elemSize() };
    flags = cv::updateContinuityFlag(flags, 2, sz, steps);
}

cv::cuda::GpuMat::GpuMat(পূর্ণাঙ্ক rows_, পূর্ণাঙ্ক cols_, পূর্ণাঙ্ক type_, শূন্য* data_, size_t step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(rows_), cols(cols_),
    step(step_), data((uchar*)data_), refcount(0),
    datastart((uchar*)data_), dataend((ধ্রুব uchar*)data_),
    allocator(defaultAllocator())
{
    size_t minstep = cols * elemSize();

    যদি (step == Mat::AUTO_STEP)
    {
        step = minstep;
    }
    অন্যথা
    {
        যদি (rows == 1)
            step = minstep;

        CV_DbgAssert( step >= minstep );
    }

    dataend += step * (rows - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(Size size_, পূর্ণাঙ্ক type_, শূন্য* data_, size_t step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(size_.height), cols(size_.width),
    step(step_), data((uchar*)data_), refcount(0),
    datastart((uchar*)data_), dataend((ধ্রুব uchar*)data_),
    allocator(defaultAllocator())
{
    size_t minstep = cols * elemSize();

    যদি (step == Mat::AUTO_STEP)
    {
        step = minstep;
    }
    অন্যথা
    {
        যদি (rows == 1)
            step = minstep;

        CV_DbgAssert( step >= minstep );
    }

    dataend += step * (rows - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ধ্রুব GpuMat& m, Range rowRange_, Range colRange_)
{
    flags = m.flags;
    step = m.step; refcount = m.refcount;
    data = m.data; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    যদি (rowRange_ == Range::all())
    {
        rows = m.rows;
    }
    অন্যথা
    {
        CV_Assert( 0 <= rowRange_.start && rowRange_.start <= rowRange_.end && rowRange_.end <= m.rows );

        rows = rowRange_.size();
        data += step*rowRange_.start;
    }

    যদি (colRange_ == Range::all())
    {
        cols = m.cols;
    }
    অন্যথা
    {
        CV_Assert( 0 <= colRange_.start && colRange_.start <= colRange_.end && colRange_.end <= m.cols );

        cols = colRange_.size();
        data += colRange_.start*elemSize();
    }

    যদি (refcount)
        CV_XADD(refcount, 1);

    যদি (rows <= 0 || cols <= 0)
        rows = cols = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ধ্রুব GpuMat& m, Rect roi) :
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

    যদি (refcount)
        CV_XADD(refcount, 1);

    যদি (rows <= 0 || cols <= 0)
        rows = cols = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(পূর্ণাঙ্ক new_cn, পূর্ণাঙ্ক new_rows) ধ্রুব
{
    GpuMat hdr = *এতত্;

    পূর্ণাঙ্ক cn = channels();
    যদি (new_cn == 0)
        new_cn = cn;

    পূর্ণাঙ্ক total_width = cols * cn;

    যদি ((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * total_width / new_cn;

    যদি (new_rows != 0 && new_rows != rows)
    {
        পূর্ণাঙ্ক total_size = total_width * rows;

        যদি (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        যদি ((unsigned)new_rows > (unsigned)total_size)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        যদি (total_width * new_rows != total_size)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = total_width * elemSize1();
    }

    পূর্ণাঙ্ক new_width = total_width / new_cn;

    যদি (new_width * new_cn != total_width)
        CV_Error(cv::Error::Bad_অঙ্ক_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    প্রত্যাগম hdr;
}

শূন্য cv::cuda::GpuMat::locateROI(Size& wholeSize, Point& ofs) ধ্রুব
{
    CV_DbgAssert( step > 0 );

    size_t esz = elemSize();
    ptrdiff_t delta1 = data - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    যদি (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    অন্যথা
    {
        ofs.y = static_cast<পূর্ণাঙ্ক>(delta1 / step);
        ofs.x = static_cast<পূর্ণাঙ্ক>((delta1 - step * ofs.y) / esz);

        CV_DbgAssert( data == datastart + ofs.y * step + ofs.x * esz );
    }

    size_t minstep = (ofs.x + cols) * esz;

    wholeSize.height = std::max(static_cast<পূর্ণাঙ্ক>((delta2 - minstep) / step + 1), ofs.y + rows);
    wholeSize.width = std::max(static_cast<পূর্ণাঙ্ক>((delta2 - step * (wholeSize.height - 1)) / esz), ofs.x + cols);
}

GpuMat& cv::cuda::GpuMat::adjustROI(পূর্ণাঙ্ক dtop, পূর্ণাঙ্ক dbottom, পূর্ণাঙ্ক dleft, পূর্ণাঙ্ক dright)
{
    Size wholeSize;
    Point ofs;
    locateROI(wholeSize, ofs);

    size_t esz = elemSize();

    পূর্ণাঙ্ক row1 = std::max(ofs.y - dtop, 0);
    পূর্ণাঙ্ক row2 = std::min(ofs.y + rows + dbottom, wholeSize.height);

    পূর্ণাঙ্ক col1 = std::max(ofs.x - dleft, 0);
    পূর্ণাঙ্ক col2 = std::min(ofs.x + cols + dright, wholeSize.width);

    data += (row1 - ofs.y) * step + (col1 - ofs.x) * esz;
    rows = row2 - row1;
    cols = col2 - col1;

    updateContinuityFlag();
    প্রত্যাগম *এতত্;
}

নামাকাশ
{
    template <বর্গ ObjType>
    শূন্য createContinuousImpl(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type, ObjType& obj)
    {
        ধ্রুব পূর্ণাঙ্ক area = rows * cols;

        যদি (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), rows);
    }
}

শূন্য cv::cuda::createContinuous(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type, OutputArray arr)
{
    বিকল্প (arr.kind())
    {
    পক্ষ প্রবেশ_Array_2::MAT:
        ::createContinuousImpl(rows, cols, type, arr.getMatRef());
        ভঙ্গ;

    পক্ষ প্রবেশ_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(rows, cols, type, arr.getGpuMatRef());
        ভঙ্গ;

    পক্ষ প্রবেশ_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(rows, cols, type, arr.getHostMemRef());
        ভঙ্গ;

    স্বভাব:
        arr.create(rows, cols, type);
    }
}

নামাকাশ
{
    template <বর্গ ObjType>
    শূন্য ensureSizeIsEnoughImpl(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type, ObjType& obj)
    {
        যদি (obj.empty() || obj.type() != type || obj.data != obj.datastart)
        {
            obj.create(rows, cols, type);
        }
        অন্যথা
        {
            ধ্রুব size_t esz = obj.elemSize();
            ধ্রুব ptrdiff_t delta2 = obj.dataend - obj.datastart;

            ধ্রুব size_t minstep = obj.cols * esz;

            Size wholeSize;
            wholeSize.height = std::max(static_cast<পূর্ণাঙ্ক>((delta2 - minstep) / static_cast<size_t>(obj.step) + 1), obj.rows);
            wholeSize.width = std::max(static_cast<পূর্ণাঙ্ক>((delta2 - static_cast<size_t>(obj.step) * (wholeSize.height - 1)) / esz), obj.cols);

            যদি (wholeSize.height < rows || wholeSize.width < cols)
            {
                obj.create(rows, cols, type);
            }
            অন্যথা
            {
                obj.cols = cols;
                obj.rows = rows;
            }
        }
    }
}

শূন্য cv::cuda::ensureSizeIsEnough(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type, OutputArray arr)
{
    বিকল্প (arr.kind())
    {
    পক্ষ প্রবেশ_Array_2::MAT:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getMatRef());
        ভঙ্গ;

    পক্ষ প্রবেশ_Array_2::CUDA_GPU_MAT:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getGpuMatRef());
        ভঙ্গ;

    পক্ষ প্রবেশ_Array_2::CUDA_HOST_MEM:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getHostMemRef());
        ভঙ্গ;

    স্বভাব:
        arr.create(rows, cols, type);
    }
}

GpuMat cv::cuda::get_প্রবেশ_Mat(প্রবেশ_Array _src, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(_src);
    CV_UNUSED(stream);
    throw_no_cuda();
#অন্যথা
    GpuMat src;
    যদি (_src.kind() == প্রবেশ_Array_2::CUDA_GPU_MAT)
    {
        src = _src.getGpuMat();
    }
    অন্যথা যদি (!_src.empty())
    {
        BufferPool pool(stream);
        src = pool.getBuffer(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    প্রত্যাগম src;
#endif
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(_dst);
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    CV_UNUSED(stream);
    throw_no_cuda();
#অন্যথা
    GpuMat dst;
    যদি (_dst.kind() == প্রবেশ_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(rows, cols, type);
        dst = _dst.getGpuMat();
    }
    অন্যথা
    {
        BufferPool pool(stream);
        dst = pool.getBuffer(rows, cols, type);
    }
    প্রত্যাগম dst;
#endif
}

শূন্য cv::cuda::syncOutput(ধ্রুব GpuMat& dst, OutputArray _dst, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(dst);
    CV_UNUSED(_dst);
    CV_UNUSED(stream);
    throw_no_cuda();
#অন্যথা
    যদি (_dst.kind() != প্রবেশ_Array_2::CUDA_GPU_MAT)
    {
        যদি (stream)
            dst.download(_dst, stream);
        অন্যথা
            dst.download(_dst);
    }
#endif
}

#ifndef HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    প্রত্যাগম 0;
}

শূন্য cv::cuda::GpuMat::setDefaultAllocator(Allocator* allocator)
{
    CV_UNUSED(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    প্রত্যাগম 0;
}

শূন্য cv::cuda::GpuMat::create(পূর্ণাঙ্ক _rows, পূর্ণাঙ্ক _cols, পূর্ণাঙ্ক _type)
{
    CV_UNUSED(_rows);
    CV_UNUSED(_cols);
    CV_UNUSED(_type);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::release()
{
    যদি( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->মুক্ত(এতত্);
    dataend = datastart = data = 0;
    step = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_MASK;
}

শূন্য cv::cuda::GpuMat::fit(পূর্ণাঙ্ক _rows, পূর্ণাঙ্ক _cols, পূর্ণাঙ্ক _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t oldTotalBytes = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_SIZE(_type);
    size_t newTotalBytes = (size_t)_rows * (size_t)_cols * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    যদি (newTotalBytes > 0 && (!isContinuous() || newTotalBytes > oldTotalBytes || data != datastart))
    {
        create(_rows, _cols, _type);
        প্রত্যাগম;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_MASK) | CV_MAKETYPE(CV_MAT_DEPTH(_type), CV_MAT_CN(_type));
    rows = _rows;
    cols = _cols;
    step = (size_t)cols * esz;
}

শূন্য cv::cuda::GpuMat::fit(Size _sz, পূর্ণাঙ্ক _type)
{
    fit(_sz.height, _sz.width, _type);
}

শূন্য cv::cuda::GpuMat::upload(প্রবেশ_Array arr)
{
    CV_UNUSED(arr);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::upload(প্রবেশ_Array arr, Stream& _stream)
{
    CV_UNUSED(arr);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::download(OutputArray _dst) ধ্রুব
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::download(OutputArray _dst, Stream& _stream) ধ্রুব
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::copyTo(OutputArray _dst) ধ্রুব
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::copyTo(OutputArray _dst, Stream& _stream) ধ্রুব
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::copyTo(OutputArray _dst, প্রবেশ_Array _mask, Stream& _stream) ধ্রুব
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

GpuMat& cv::cuda::GpuMat::setTo(Scalar s, প্রবেশ_Array _mask, Stream& _stream)
{
    CV_UNUSED(s);
    CV_UNUSED(_mask);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::convertTo(OutputArray _dst, পূর্ণাঙ্ক rtype, Stream& _stream) ধ্রুব
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::convertTo(OutputArray _dst, পূর্ণাঙ্ক rtype, দ্বিগুণ alpha, দ্বিগুণ beta, Stream& _stream) ধ্রুব
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(alpha);
    CV_UNUSED(beta);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

#endif
