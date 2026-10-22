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
    পূর্ণসংখ্যা sz[] = { rows, cols };
    size_t steps[] = { step, elem_আকার() };
    flags = cv::updateContinuityFlag(flags, 2, sz, steps);
}

cv::cuda::GpuMat::GpuMat(পূর্ণসংখ্যা rows_, পূর্ণসংখ্যা cols_, পূর্ণসংখ্যা type_, শূন্য* তথ্য_2, size_t step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(rows_), cols(cols_),
    step(step_), তথ্য((uchar*)তথ্য_2), refcount(0),
    datastart((uchar*)তথ্য_2), dataend((ধ্রুবক uchar*)তথ্য_2),
    allocator(defaultAllocator())
{
    size_t minstep = cols * elem_আকার();

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

cv::cuda::GpuMat::GpuMat(আকার_2 আকার_4, পূর্ণসংখ্যা type_, শূন্য* তথ্য_2, size_t step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(আকার_4.height), cols(আকার_4.width),
    step(step_), তথ্য((uchar*)তথ্য_2), refcount(0),
    datastart((uchar*)তথ্য_2), dataend((ধ্রুবক uchar*)তথ্য_2),
    allocator(defaultAllocator())
{
    size_t minstep = cols * elem_আকার();

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

cv::cuda::GpuMat::GpuMat(ধ্রুবক GpuMat& m, Range rowRange_, Range colRange_)
{
    flags = m.flags;
    step = m.step; refcount = m.refcount;
    তথ্য = m.তথ্য; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    যদি (rowRange_ == Range::all())
    {
        rows = m.rows;
    }
    অন্যথা
    {
        CV_Assert( 0 <= rowRange_.start && rowRange_.start <= rowRange_.end && rowRange_.end <= m.rows );

        rows = rowRange_.size();
        তথ্য += step*rowRange_.start;
    }

    যদি (colRange_ == Range::all())
    {
        cols = m.cols;
    }
    অন্যথা
    {
        CV_Assert( 0 <= colRange_.start && colRange_.start <= colRange_.end && colRange_.end <= m.cols );

        cols = colRange_.size();
        তথ্য += colRange_.start*elem_আকার();
    }

    যদি (refcount)
        CV_XADD(refcount, 1);

    যদি (rows <= 0 || cols <= 0)
        rows = cols = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ধ্রুবক GpuMat& m, Rect roi) :
    flags(m.flags), rows(roi.height), cols(roi.width),
    step(m.step), তথ্য(m.তথ্য + roi.y*step), refcount(m.refcount),
    datastart(m.datastart), dataend(m.dataend),
    allocator(m.allocator)
{
    তথ্য += roi.x * elem_আকার();

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

GpuMat cv::cuda::GpuMat::reshape(পূর্ণসংখ্যা new_cn, পূর্ণসংখ্যা new_rows) ধ্রুবক
{
    GpuMat hdr = *এই;

    পূর্ণসংখ্যা cn = channels();
    যদি (new_cn == 0)
        new_cn = cn;

    পূর্ণসংখ্যা যোগ_width = cols * cn;

    যদি ((new_cn > যোগ_width || যোগ_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * যোগ_width / new_cn;

    যদি (new_rows != 0 && new_rows != rows)
    {
        পূর্ণসংখ্যা যোগ_আকার_2 = যোগ_width * rows;

        যদি (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        যদি ((unsigned)new_rows > (unsigned)যোগ_আকার_2)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        যোগ_width = যোগ_আকার_2 / new_rows;

        যদি (যোগ_width * new_rows != যোগ_আকার_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = যোগ_width * elemSize1();
    }

    পূর্ণসংখ্যা new_width = যোগ_width / new_cn;

    যদি (new_width * new_cn != যোগ_width)
        CV_Error(cv::Error::Bad_অঙ্ক_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    ফেরত hdr;
}

শূন্য cv::cuda::GpuMat::locateROI(আকার_2& whole_আকার, Point& ofs) ধ্রুবক
{
    CV_DbgAssert( step > 0 );

    size_t esz = elem_আকার();
    ptrdiff_t delta1 = তথ্য - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    যদি (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    অন্যথা
    {
        ofs.y = static_cast<পূর্ণসংখ্যা>(delta1 / step);
        ofs.x = static_cast<পূর্ণসংখ্যা>((delta1 - step * ofs.y) / esz);

        CV_DbgAssert( তথ্য == datastart + ofs.y * step + ofs.x * esz );
    }

    size_t minstep = (ofs.x + cols) * esz;

    whole_আকার.height = std::max(static_cast<পূর্ণসংখ্যা>((delta2 - minstep) / step + 1), ofs.y + rows);
    whole_আকার.width = std::max(static_cast<পূর্ণসংখ্যা>((delta2 - step * (whole_আকার.height - 1)) / esz), ofs.x + cols);
}

GpuMat& cv::cuda::GpuMat::adjustROI(পূর্ণসংখ্যা dtop, পূর্ণসংখ্যা dbottom, পূর্ণসংখ্যা dleft, পূর্ণসংখ্যা dright)
{
    আকার_2 whole_আকার;
    Point ofs;
    locateROI(whole_আকার, ofs);

    size_t esz = elem_আকার();

    পূর্ণসংখ্যা row1 = std::max(ofs.y - dtop, 0);
    পূর্ণসংখ্যা row2 = std::min(ofs.y + rows + dbottom, whole_আকার.height);

    পূর্ণসংখ্যা col1 = std::max(ofs.x - dleft, 0);
    পূর্ণসংখ্যা col2 = std::min(ofs.x + cols + dright, whole_আকার.width);

    তথ্য += (row1 - ofs.y) * step + (col1 - ofs.x) * esz;
    rows = row2 - row1;
    cols = col2 - col1;

    updateContinuityFlag();
    ফেরত *এই;
}

নামাকাশ
{
    template <শ্রেণী ObjType>
    শূন্য createContinuousImpl(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা type, ObjType& obj)
    {
        ধ্রুবক পূর্ণসংখ্যা area = rows * cols;

        যদি (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), rows);
    }
}

শূন্য cv::cuda::createContinuous(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা type, OutputArray arr)
{
    নির্বাচন (arr.kind())
    {
    ক্ষেত্র প্রবেশ_Array_2::MAT:
        ::createContinuousImpl(rows, cols, type, arr.getMatRef());
        বিরতি;

    ক্ষেত্র প্রবেশ_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(rows, cols, type, arr.getGpuMatRef());
        বিরতি;

    ক্ষেত্র প্রবেশ_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(rows, cols, type, arr.getHostMemRef());
        বিরতি;

    পূর্বনির্ধারিত:
        arr.create(rows, cols, type);
    }
}

নামাকাশ
{
    template <শ্রেণী ObjType>
    শূন্য ensure_আকার_Is_Enough_Impl(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা type, ObjType& obj)
    {
        যদি (obj.empty() || obj.type() != type || obj.তথ্য != obj.datastart)
        {
            obj.create(rows, cols, type);
        }
        অন্যথা
        {
            ধ্রুবক size_t esz = obj.elem_আকার();
            ধ্রুবক ptrdiff_t delta2 = obj.dataend - obj.datastart;

            ধ্রুবক size_t minstep = obj.cols * esz;

            আকার_2 whole_আকার;
            whole_আকার.height = std::max(static_cast<পূর্ণসংখ্যা>((delta2 - minstep) / static_cast<size_t>(obj.step) + 1), obj.rows);
            whole_আকার.width = std::max(static_cast<পূর্ণসংখ্যা>((delta2 - static_cast<size_t>(obj.step) * (whole_আকার.height - 1)) / esz), obj.cols);

            যদি (whole_আকার.height < rows || whole_আকার.width < cols)
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

শূন্য cv::cuda::ensure_আকার_Is_Enough(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা type, OutputArray arr)
{
    নির্বাচন (arr.kind())
    {
    ক্ষেত্র প্রবেশ_Array_2::MAT:
        ::ensure_আকার_Is_Enough_Impl(rows, cols, type, arr.getMatRef());
        বিরতি;

    ক্ষেত্র প্রবেশ_Array_2::CUDA_GPU_MAT:
        ::ensure_আকার_Is_Enough_Impl(rows, cols, type, arr.getGpuMatRef());
        বিরতি;

    ক্ষেত্র প্রবেশ_Array_2::CUDA_HOST_MEM:
        ::ensure_আকার_Is_Enough_Impl(rows, cols, type, arr.getHostMemRef());
        বিরতি;

    পূর্বনির্ধারিত:
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
        বাফার_Pool pool(stream);
        src = pool.get_বাফার(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    ফেরত src;
#endif
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা type, Stream& stream)
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
        বাফার_Pool pool(stream);
        dst = pool.get_বাফার(rows, cols, type);
    }
    ফেরত dst;
#endif
}

শূন্য cv::cuda::syncOutput(ধ্রুবক GpuMat& dst, OutputArray _dst, Stream& stream)
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
    ফেরত 0;
}

শূন্য cv::cuda::GpuMat::setDefaultAllocator(Allocator* allocator)
{
    CV_UNUSED(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    ফেরত 0;
}

শূন্য cv::cuda::GpuMat::create(পূর্ণসংখ্যা _rows, পূর্ণসংখ্যা _cols, পূর্ণসংখ্যা _type)
{
    CV_UNUSED(_rows);
    CV_UNUSED(_cols);
    CV_UNUSED(_type);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::release()
{
    যদি( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->মুক্ত(এই);
    dataend = datastart = তথ্য = 0;
    step = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_MASK;
}

শূন্য cv::cuda::GpuMat::fit(পূর্ণসংখ্যা _rows, পূর্ণসংখ্যা _cols, পূর্ণসংখ্যা _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t old_যোগ_Bytes = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_আকার(_type);
    size_t new_যোগ_Bytes = (size_t)_rows * (size_t)_cols * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    যদি (new_যোগ_Bytes > 0 && (!isContinuous() || new_যোগ_Bytes > old_যোগ_Bytes || তথ্য != datastart))
    {
        create(_rows, _cols, _type);
        ফেরত;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_MASK) | CV_MAKETYPE(CV_MAT_DEPTH(_type), CV_MAT_CN(_type));
    rows = _rows;
    cols = _cols;
    step = (size_t)cols * esz;
}

শূন্য cv::cuda::GpuMat::fit(আকার_2 _sz, পূর্ণসংখ্যা _type)
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

শূন্য cv::cuda::GpuMat::download(OutputArray _dst) ধ্রুবক
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::download(OutputArray _dst, Stream& _stream) ধ্রুবক
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::copyTo(OutputArray _dst) ধ্রুবক
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::copyTo(OutputArray _dst, Stream& _stream) ধ্রুবক
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::copyTo(OutputArray _dst, প্রবেশ_Array _mask, Stream& _stream) ধ্রুবক
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

শূন্য cv::cuda::GpuMat::convertTo(OutputArray _dst, পূর্ণসংখ্যা rtype, Stream& _stream) ধ্রুবক
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

শূন্য cv::cuda::GpuMat::convertTo(OutputArray _dst, পূর্ণসংখ্যা rtype, দ্বিগুণ alpha, দ্বিগুণ beta, Stream& _stream) ধ্রুবক
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(alpha);
    CV_UNUSED(beta);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

#endif
