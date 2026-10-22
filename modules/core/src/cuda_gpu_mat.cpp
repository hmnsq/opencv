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

#அந்தர்பாவ "precomp.hpp"

using நாமாகாஶ cv;
using நாமாகாஶ cv::cuda;

வெறுமை cv::cuda::GpuMat::updateContinuityFlag()
{
    முழு sz[] = { rows, cols };
    size_t steps[] = { step, elemSize() };
    flags = cv::updateContinuityFlag(flags, 2, sz, steps);
}

cv::cuda::GpuMat::GpuMat(முழு rows_, முழு cols_, முழு type_, வெறுமை* data_, size_t step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(rows_), cols(cols_),
    step(step_), data((uchar*)data_), refcount(0),
    datastart((uchar*)data_), dataend((மாறிலி uchar*)data_),
    allocator(defaultAllocator())
{
    size_t minstep = cols * elemSize();

    எனில் (step == Mat::AUTO_STEP)
    {
        step = minstep;
    }
    இல்லையெனில்
    {
        எனில் (rows == 1)
            step = minstep;

        CV_DbgAssert( step >= minstep );
    }

    dataend += step * (rows - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(Size size_, முழு type_, வெறுமை* data_, size_t step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(size_.height), cols(size_.width),
    step(step_), data((uchar*)data_), refcount(0),
    datastart((uchar*)data_), dataend((மாறிலி uchar*)data_),
    allocator(defaultAllocator())
{
    size_t minstep = cols * elemSize();

    எனில் (step == Mat::AUTO_STEP)
    {
        step = minstep;
    }
    இல்லையெனில்
    {
        எனில் (rows == 1)
            step = minstep;

        CV_DbgAssert( step >= minstep );
    }

    dataend += step * (rows - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(மாறிலி GpuMat& m, Range rowRange_, Range colRange_)
{
    flags = m.flags;
    step = m.step; refcount = m.refcount;
    data = m.data; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    எனில் (rowRange_ == Range::all())
    {
        rows = m.rows;
    }
    இல்லையெனில்
    {
        CV_Assert( 0 <= rowRange_.start && rowRange_.start <= rowRange_.end && rowRange_.end <= m.rows );

        rows = rowRange_.size();
        data += step*rowRange_.start;
    }

    எனில் (colRange_ == Range::all())
    {
        cols = m.cols;
    }
    இல்லையெனில்
    {
        CV_Assert( 0 <= colRange_.start && colRange_.start <= colRange_.end && colRange_.end <= m.cols );

        cols = colRange_.size();
        data += colRange_.start*elemSize();
    }

    எனில் (refcount)
        CV_XADD(refcount, 1);

    எனில் (rows <= 0 || cols <= 0)
        rows = cols = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(மாறிலி GpuMat& m, Rect roi) :
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

    எனில் (refcount)
        CV_XADD(refcount, 1);

    எனில் (rows <= 0 || cols <= 0)
        rows = cols = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(முழு new_cn, முழு new_rows) மாறிலி
{
    GpuMat hdr = *இது;

    முழு cn = channels();
    எனில் (new_cn == 0)
        new_cn = cn;

    முழு total_width = cols * cn;

    எனில் ((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * total_width / new_cn;

    எனில் (new_rows != 0 && new_rows != rows)
    {
        முழு total_size = total_width * rows;

        எனில் (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        எனில் ((unsigned)new_rows > (unsigned)total_size)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        எனில் (total_width * new_rows != total_size)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = total_width * elemSize1();
    }

    முழு new_width = total_width / new_cn;

    எனில் (new_width * new_cn != total_width)
        CV_Error(cv::Error::Bad_இலக்கம்_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    திரும்பு hdr;
}

வெறுமை cv::cuda::GpuMat::locateROI(Size& wholeSize, Point& ofs) மாறிலி
{
    CV_DbgAssert( step > 0 );

    size_t esz = elemSize();
    ptrdiff_t delta1 = data - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    எனில் (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    இல்லையெனில்
    {
        ofs.y = static_cast<முழு>(delta1 / step);
        ofs.x = static_cast<முழு>((delta1 - step * ofs.y) / esz);

        CV_DbgAssert( data == datastart + ofs.y * step + ofs.x * esz );
    }

    size_t minstep = (ofs.x + cols) * esz;

    wholeSize.height = std::max(static_cast<முழு>((delta2 - minstep) / step + 1), ofs.y + rows);
    wholeSize.width = std::max(static_cast<முழு>((delta2 - step * (wholeSize.height - 1)) / esz), ofs.x + cols);
}

GpuMat& cv::cuda::GpuMat::adjustROI(முழு dtop, முழு dbottom, முழு dleft, முழு dright)
{
    Size wholeSize;
    Point ofs;
    locateROI(wholeSize, ofs);

    size_t esz = elemSize();

    முழு row1 = std::max(ofs.y - dtop, 0);
    முழு row2 = std::min(ofs.y + rows + dbottom, wholeSize.height);

    முழு col1 = std::max(ofs.x - dleft, 0);
    முழு col2 = std::min(ofs.x + cols + dright, wholeSize.width);

    data += (row1 - ofs.y) * step + (col1 - ofs.x) * esz;
    rows = row2 - row1;
    cols = col2 - col1;

    updateContinuityFlag();
    திரும்பு *இது;
}

நாமாகாஶ
{
    template <வகுப்பு ObjType>
    வெறுமை createContinuousImpl(முழு rows, முழு cols, முழு type, ObjType& obj)
    {
        மாறிலி முழு area = rows * cols;

        எனில் (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), rows);
    }
}

வெறுமை cv::cuda::createContinuous(முழு rows, முழு cols, முழு type, OutputArray arr)
{
    தேர்வு (arr.kind())
    {
    நிலை உள்ளீடு_Array_2::MAT:
        ::createContinuousImpl(rows, cols, type, arr.getMatRef());
        நிறுத்து;

    நிலை உள்ளீடு_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(rows, cols, type, arr.getGpuMatRef());
        நிறுத்து;

    நிலை உள்ளீடு_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(rows, cols, type, arr.getHostMemRef());
        நிறுத்து;

    இயல்பு:
        arr.create(rows, cols, type);
    }
}

நாமாகாஶ
{
    template <வகுப்பு ObjType>
    வெறுமை ensureSizeIsEnoughImpl(முழு rows, முழு cols, முழு type, ObjType& obj)
    {
        எனில் (obj.empty() || obj.type() != type || obj.data != obj.datastart)
        {
            obj.create(rows, cols, type);
        }
        இல்லையெனில்
        {
            மாறிலி size_t esz = obj.elemSize();
            மாறிலி ptrdiff_t delta2 = obj.dataend - obj.datastart;

            மாறிலி size_t minstep = obj.cols * esz;

            Size wholeSize;
            wholeSize.height = std::max(static_cast<முழு>((delta2 - minstep) / static_cast<size_t>(obj.step) + 1), obj.rows);
            wholeSize.width = std::max(static_cast<முழு>((delta2 - static_cast<size_t>(obj.step) * (wholeSize.height - 1)) / esz), obj.cols);

            எனில் (wholeSize.height < rows || wholeSize.width < cols)
            {
                obj.create(rows, cols, type);
            }
            இல்லையெனில்
            {
                obj.cols = cols;
                obj.rows = rows;
            }
        }
    }
}

வெறுமை cv::cuda::ensureSizeIsEnough(முழு rows, முழு cols, முழு type, OutputArray arr)
{
    தேர்வு (arr.kind())
    {
    நிலை உள்ளீடு_Array_2::MAT:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getMatRef());
        நிறுத்து;

    நிலை உள்ளீடு_Array_2::CUDA_GPU_MAT:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getGpuMatRef());
        நிறுத்து;

    நிலை உள்ளீடு_Array_2::CUDA_HOST_MEM:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getHostMemRef());
        நிறுத்து;

    இயல்பு:
        arr.create(rows, cols, type);
    }
}

GpuMat cv::cuda::get_உள்ளீடு_Mat(உள்ளீடு_Array _src, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(_src);
    CV_UNUSED(stream);
    throw_no_cuda();
#இல்லையெனில்
    GpuMat src;
    எனில் (_src.kind() == உள்ளீடு_Array_2::CUDA_GPU_MAT)
    {
        src = _src.getGpuMat();
    }
    இல்லையெனில் எனில் (!_src.empty())
    {
        BufferPool pool(stream);
        src = pool.getBuffer(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    திரும்பு src;
#endif
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, முழு rows, முழு cols, முழு type, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(_dst);
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    CV_UNUSED(stream);
    throw_no_cuda();
#இல்லையெனில்
    GpuMat dst;
    எனில் (_dst.kind() == உள்ளீடு_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(rows, cols, type);
        dst = _dst.getGpuMat();
    }
    இல்லையெனில்
    {
        BufferPool pool(stream);
        dst = pool.getBuffer(rows, cols, type);
    }
    திரும்பு dst;
#endif
}

வெறுமை cv::cuda::syncOutput(மாறிலி GpuMat& dst, OutputArray _dst, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(dst);
    CV_UNUSED(_dst);
    CV_UNUSED(stream);
    throw_no_cuda();
#இல்லையெனில்
    எனில் (_dst.kind() != உள்ளீடு_Array_2::CUDA_GPU_MAT)
    {
        எனில் (stream)
            dst.download(_dst, stream);
        இல்லையெனில்
            dst.download(_dst);
    }
#endif
}

#ifndef HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    திரும்பு 0;
}

வெறுமை cv::cuda::GpuMat::setDefaultAllocator(Allocator* allocator)
{
    CV_UNUSED(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    திரும்பு 0;
}

வெறுமை cv::cuda::GpuMat::create(முழு _rows, முழு _cols, முழு _type)
{
    CV_UNUSED(_rows);
    CV_UNUSED(_cols);
    CV_UNUSED(_type);
    throw_no_cuda();
}

வெறுமை cv::cuda::GpuMat::release()
{
    எனில்( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->விடுவி(இது);
    dataend = datastart = data = 0;
    step = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_MASK;
}

வெறுமை cv::cuda::GpuMat::fit(முழு _rows, முழு _cols, முழு _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t oldTotalBytes = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_SIZE(_type);
    size_t newTotalBytes = (size_t)_rows * (size_t)_cols * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    எனில் (newTotalBytes > 0 && (!isContinuous() || newTotalBytes > oldTotalBytes || data != datastart))
    {
        create(_rows, _cols, _type);
        திரும்பு;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_MASK) | CV_MAKETYPE(CV_MAT_DEPTH(_type), CV_MAT_CN(_type));
    rows = _rows;
    cols = _cols;
    step = (size_t)cols * esz;
}

வெறுமை cv::cuda::GpuMat::fit(Size _sz, முழு _type)
{
    fit(_sz.height, _sz.width, _type);
}

வெறுமை cv::cuda::GpuMat::upload(உள்ளீடு_Array arr)
{
    CV_UNUSED(arr);
    throw_no_cuda();
}

வெறுமை cv::cuda::GpuMat::upload(உள்ளீடு_Array arr, Stream& _stream)
{
    CV_UNUSED(arr);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

வெறுமை cv::cuda::GpuMat::download(OutputArray _dst) மாறிலி
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

வெறுமை cv::cuda::GpuMat::download(OutputArray _dst, Stream& _stream) மாறிலி
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

வெறுமை cv::cuda::GpuMat::copyTo(OutputArray _dst) மாறிலி
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

வெறுமை cv::cuda::GpuMat::copyTo(OutputArray _dst, Stream& _stream) மாறிலி
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

வெறுமை cv::cuda::GpuMat::copyTo(OutputArray _dst, உள்ளீடு_Array _mask, Stream& _stream) மாறிலி
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

GpuMat& cv::cuda::GpuMat::setTo(Scalar s, உள்ளீடு_Array _mask, Stream& _stream)
{
    CV_UNUSED(s);
    CV_UNUSED(_mask);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

வெறுமை cv::cuda::GpuMat::convertTo(OutputArray _dst, முழு rtype, Stream& _stream) மாறிலி
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

வெறுமை cv::cuda::GpuMat::convertTo(OutputArray _dst, முழு rtype, இரட்டை alpha, இரட்டை beta, Stream& _stream) மாறிலி
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(alpha);
    CV_UNUSED(beta);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

#endif
