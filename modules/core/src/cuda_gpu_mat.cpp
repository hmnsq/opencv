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

#અન્તર્ભાવ "precomp.hpp"

using નામાકાશ cv;
using નામાકાશ cv::cuda;

શૂન્ય cv::cuda::GpuMat::updateContinuityFlag()
{
    પૂર્ણાંક sz[] = { rows, cols };
    size_t steps[] = { step, elemSize() };
    flags = cv::updateContinuityFlag(flags, 2, sz, steps);
}

cv::cuda::GpuMat::GpuMat(પૂર્ણાંક rows_, પૂર્ણાંક cols_, પૂર્ણાંક type_, શૂન્ય* data_, size_t step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(rows_), cols(cols_),
    step(step_), data((uchar*)data_), refcount(0),
    datastart((uchar*)data_), dataend((અચલ uchar*)data_),
    allocator(defaultAllocator())
{
    size_t minstep = cols * elemSize();

    જો (step == Mat::AUTO_STEP)
    {
        step = minstep;
    }
    નહીંતર
    {
        જો (rows == 1)
            step = minstep;

        CV_DbgAssert( step >= minstep );
    }

    dataend += step * (rows - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(Size size_, પૂર્ણાંક type_, શૂન્ય* data_, size_t step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(size_.height), cols(size_.width),
    step(step_), data((uchar*)data_), refcount(0),
    datastart((uchar*)data_), dataend((અચલ uchar*)data_),
    allocator(defaultAllocator())
{
    size_t minstep = cols * elemSize();

    જો (step == Mat::AUTO_STEP)
    {
        step = minstep;
    }
    નહીંતર
    {
        જો (rows == 1)
            step = minstep;

        CV_DbgAssert( step >= minstep );
    }

    dataend += step * (rows - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(અચલ GpuMat& m, Range rowRange_, Range colRange_)
{
    flags = m.flags;
    step = m.step; refcount = m.refcount;
    data = m.data; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    જો (rowRange_ == Range::all())
    {
        rows = m.rows;
    }
    નહીંતર
    {
        CV_Assert( 0 <= rowRange_.start && rowRange_.start <= rowRange_.end && rowRange_.end <= m.rows );

        rows = rowRange_.size();
        data += step*rowRange_.start;
    }

    જો (colRange_ == Range::all())
    {
        cols = m.cols;
    }
    નહીંતર
    {
        CV_Assert( 0 <= colRange_.start && colRange_.start <= colRange_.end && colRange_.end <= m.cols );

        cols = colRange_.size();
        data += colRange_.start*elemSize();
    }

    જો (refcount)
        CV_XADD(refcount, 1);

    જો (rows <= 0 || cols <= 0)
        rows = cols = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(અચલ GpuMat& m, Rect roi) :
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

    જો (refcount)
        CV_XADD(refcount, 1);

    જો (rows <= 0 || cols <= 0)
        rows = cols = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(પૂર્ણાંક new_cn, પૂર્ણાંક new_rows) અચલ
{
    GpuMat hdr = *આ;

    પૂર્ણાંક cn = channels();
    જો (new_cn == 0)
        new_cn = cn;

    પૂર્ણાંક total_width = cols * cn;

    જો ((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * total_width / new_cn;

    જો (new_rows != 0 && new_rows != rows)
    {
        પૂર્ણાંક total_size = total_width * rows;

        જો (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        જો ((unsigned)new_rows > (unsigned)total_size)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        જો (total_width * new_rows != total_size)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = total_width * elemSize1();
    }

    પૂર્ણાંક new_width = total_width / new_cn;

    જો (new_width * new_cn != total_width)
        CV_Error(cv::Error::Bad_અંક_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    પાછા hdr;
}

શૂન્ય cv::cuda::GpuMat::locateROI(Size& wholeSize, Point& ofs) અચલ
{
    CV_DbgAssert( step > 0 );

    size_t esz = elemSize();
    ptrdiff_t delta1 = data - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    જો (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    નહીંતર
    {
        ofs.y = static_cast<પૂર્ણાંક>(delta1 / step);
        ofs.x = static_cast<પૂર્ણાંક>((delta1 - step * ofs.y) / esz);

        CV_DbgAssert( data == datastart + ofs.y * step + ofs.x * esz );
    }

    size_t minstep = (ofs.x + cols) * esz;

    wholeSize.height = std::max(static_cast<પૂર્ણાંક>((delta2 - minstep) / step + 1), ofs.y + rows);
    wholeSize.width = std::max(static_cast<પૂર્ણાંક>((delta2 - step * (wholeSize.height - 1)) / esz), ofs.x + cols);
}

GpuMat& cv::cuda::GpuMat::adjustROI(પૂર્ણાંક dtop, પૂર્ણાંક dbottom, પૂર્ણાંક dleft, પૂર્ણાંક dright)
{
    Size wholeSize;
    Point ofs;
    locateROI(wholeSize, ofs);

    size_t esz = elemSize();

    પૂર્ણાંક row1 = std::max(ofs.y - dtop, 0);
    પૂર્ણાંક row2 = std::min(ofs.y + rows + dbottom, wholeSize.height);

    પૂર્ણાંક col1 = std::max(ofs.x - dleft, 0);
    પૂર્ણાંક col2 = std::min(ofs.x + cols + dright, wholeSize.width);

    data += (row1 - ofs.y) * step + (col1 - ofs.x) * esz;
    rows = row2 - row1;
    cols = col2 - col1;

    updateContinuityFlag();
    પાછા *આ;
}

નામાકાશ
{
    template <વર્ગ ObjType>
    શૂન્ય createContinuousImpl(પૂર્ણાંક rows, પૂર્ણાંક cols, પૂર્ણાંક type, ObjType& obj)
    {
        અચલ પૂર્ણાંક area = rows * cols;

        જો (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), rows);
    }
}

શૂન્ય cv::cuda::createContinuous(પૂર્ણાંક rows, પૂર્ણાંક cols, પૂર્ણાંક type, OutputArray arr)
{
    પસંદગી (arr.kind())
    {
    સ્થિતિ આગત_Array_2::MAT:
        ::createContinuousImpl(rows, cols, type, arr.getMatRef());
        તોડો;

    સ્થિતિ આગત_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(rows, cols, type, arr.getGpuMatRef());
        તોડો;

    સ્થિતિ આગત_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(rows, cols, type, arr.getHostMemRef());
        તોડો;

    મૂળભૂત:
        arr.create(rows, cols, type);
    }
}

નામાકાશ
{
    template <વર્ગ ObjType>
    શૂન્ય ensureSizeIsEnoughImpl(પૂર્ણાંક rows, પૂર્ણાંક cols, પૂર્ણાંક type, ObjType& obj)
    {
        જો (obj.empty() || obj.type() != type || obj.data != obj.datastart)
        {
            obj.create(rows, cols, type);
        }
        નહીંતર
        {
            અચલ size_t esz = obj.elemSize();
            અચલ ptrdiff_t delta2 = obj.dataend - obj.datastart;

            અચલ size_t minstep = obj.cols * esz;

            Size wholeSize;
            wholeSize.height = std::max(static_cast<પૂર્ણાંક>((delta2 - minstep) / static_cast<size_t>(obj.step) + 1), obj.rows);
            wholeSize.width = std::max(static_cast<પૂર્ણાંક>((delta2 - static_cast<size_t>(obj.step) * (wholeSize.height - 1)) / esz), obj.cols);

            જો (wholeSize.height < rows || wholeSize.width < cols)
            {
                obj.create(rows, cols, type);
            }
            નહીંતર
            {
                obj.cols = cols;
                obj.rows = rows;
            }
        }
    }
}

શૂન્ય cv::cuda::ensureSizeIsEnough(પૂર્ણાંક rows, પૂર્ણાંક cols, પૂર્ણાંક type, OutputArray arr)
{
    પસંદગી (arr.kind())
    {
    સ્થિતિ આગત_Array_2::MAT:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getMatRef());
        તોડો;

    સ્થિતિ આગત_Array_2::CUDA_GPU_MAT:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getGpuMatRef());
        તોડો;

    સ્થિતિ આગત_Array_2::CUDA_HOST_MEM:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getHostMemRef());
        તોડો;

    મૂળભૂત:
        arr.create(rows, cols, type);
    }
}

GpuMat cv::cuda::get_આગત_Mat(આગત_Array _src, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(_src);
    CV_UNUSED(stream);
    throw_no_cuda();
#નહીંતર
    GpuMat src;
    જો (_src.kind() == આગત_Array_2::CUDA_GPU_MAT)
    {
        src = _src.getGpuMat();
    }
    નહીંતર જો (!_src.empty())
    {
        BufferPool pool(stream);
        src = pool.getBuffer(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    પાછા src;
#endif
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, પૂર્ણાંક rows, પૂર્ણાંક cols, પૂર્ણાંક type, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(_dst);
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    CV_UNUSED(stream);
    throw_no_cuda();
#નહીંતર
    GpuMat dst;
    જો (_dst.kind() == આગત_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(rows, cols, type);
        dst = _dst.getGpuMat();
    }
    નહીંતર
    {
        BufferPool pool(stream);
        dst = pool.getBuffer(rows, cols, type);
    }
    પાછા dst;
#endif
}

શૂન્ય cv::cuda::syncOutput(અચલ GpuMat& dst, OutputArray _dst, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(dst);
    CV_UNUSED(_dst);
    CV_UNUSED(stream);
    throw_no_cuda();
#નહીંતર
    જો (_dst.kind() != આગત_Array_2::CUDA_GPU_MAT)
    {
        જો (stream)
            dst.download(_dst, stream);
        નહીંતર
            dst.download(_dst);
    }
#endif
}

#ifndef HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    પાછા 0;
}

શૂન્ય cv::cuda::GpuMat::setDefaultAllocator(Allocator* allocator)
{
    CV_UNUSED(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    પાછા 0;
}

શૂન્ય cv::cuda::GpuMat::create(પૂર્ણાંક _rows, પૂર્ણાંક _cols, પૂર્ણાંક _type)
{
    CV_UNUSED(_rows);
    CV_UNUSED(_cols);
    CV_UNUSED(_type);
    throw_no_cuda();
}

શૂન્ય cv::cuda::GpuMat::release()
{
    જો( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->મુક્ત(આ);
    dataend = datastart = data = 0;
    step = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_MASK;
}

શૂન્ય cv::cuda::GpuMat::fit(પૂર્ણાંક _rows, પૂર્ણાંક _cols, પૂર્ણાંક _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t oldTotalBytes = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_SIZE(_type);
    size_t newTotalBytes = (size_t)_rows * (size_t)_cols * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    જો (newTotalBytes > 0 && (!isContinuous() || newTotalBytes > oldTotalBytes || data != datastart))
    {
        create(_rows, _cols, _type);
        પાછા;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_MASK) | CV_MAKETYPE(CV_MAT_DEPTH(_type), CV_MAT_CN(_type));
    rows = _rows;
    cols = _cols;
    step = (size_t)cols * esz;
}

શૂન્ય cv::cuda::GpuMat::fit(Size _sz, પૂર્ણાંક _type)
{
    fit(_sz.height, _sz.width, _type);
}

શૂન્ય cv::cuda::GpuMat::upload(આગત_Array arr)
{
    CV_UNUSED(arr);
    throw_no_cuda();
}

શૂન્ય cv::cuda::GpuMat::upload(આગત_Array arr, Stream& _stream)
{
    CV_UNUSED(arr);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

શૂન્ય cv::cuda::GpuMat::download(OutputArray _dst) અચલ
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

શૂન્ય cv::cuda::GpuMat::download(OutputArray _dst, Stream& _stream) અચલ
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

શૂન્ય cv::cuda::GpuMat::copyTo(OutputArray _dst) અચલ
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

શૂન્ય cv::cuda::GpuMat::copyTo(OutputArray _dst, Stream& _stream) અચલ
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

શૂન્ય cv::cuda::GpuMat::copyTo(OutputArray _dst, આગત_Array _mask, Stream& _stream) અચલ
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

GpuMat& cv::cuda::GpuMat::setTo(Scalar s, આગત_Array _mask, Stream& _stream)
{
    CV_UNUSED(s);
    CV_UNUSED(_mask);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

શૂન્ય cv::cuda::GpuMat::convertTo(OutputArray _dst, પૂર્ણાંક rtype, Stream& _stream) અચલ
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

શૂન્ય cv::cuda::GpuMat::convertTo(OutputArray _dst, પૂર્ણાંક rtype, દ્વિગુણ alpha, દ્વિગુણ beta, Stream& _stream) અચલ
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(alpha);
    CV_UNUSED(beta);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

#endif
