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

using नामाकाश cv;
using नामाकाश cv::cuda;

शून्य cv::cuda::GpuMat::updateContinuityFlag()
{
    पूर्णांक sz[] = { rows, cols };
    माप_प्रकार steps[] = { step, elemSize() };
    flags = cv::updateContinuityFlag(flags, 2, sz, steps);
}

cv::cuda::GpuMat::GpuMat(पूर्णांक rows_, पूर्णांक cols_, पूर्णांक type_, शून्य* data_, माप_प्रकार step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(rows_), cols(cols_),
    step(step_), data((uchar*)data_), refcount(0),
    datastart((uchar*)data_), dataend((स्थिरांक uchar*)data_),
    allocator(defaultAllocator())
{
    माप_प्रकार minstep = cols * elemSize();

    जर (step == Mat::AUTO_STEP)
    {
        step = minstep;
    }
    अन्यथा
    {
        जर (rows == 1)
            step = minstep;

        CV_DbgAssert( step >= minstep );
    }

    dataend += step * (rows - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(Size size_, पूर्णांक type_, शून्य* data_, माप_प्रकार step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(size_.height), cols(size_.width),
    step(step_), data((uchar*)data_), refcount(0),
    datastart((uchar*)data_), dataend((स्थिरांक uchar*)data_),
    allocator(defaultAllocator())
{
    माप_प्रकार minstep = cols * elemSize();

    जर (step == Mat::AUTO_STEP)
    {
        step = minstep;
    }
    अन्यथा
    {
        जर (rows == 1)
            step = minstep;

        CV_DbgAssert( step >= minstep );
    }

    dataend += step * (rows - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(स्थिरांक GpuMat& m, Range rowRange_, Range colRange_)
{
    flags = m.flags;
    step = m.step; refcount = m.refcount;
    data = m.data; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    जर (rowRange_ == Range::all())
    {
        rows = m.rows;
    }
    अन्यथा
    {
        CV_Assert( 0 <= rowRange_.start && rowRange_.start <= rowRange_.end && rowRange_.end <= m.rows );

        rows = rowRange_.size();
        data += step*rowRange_.start;
    }

    जर (colRange_ == Range::all())
    {
        cols = m.cols;
    }
    अन्यथा
    {
        CV_Assert( 0 <= colRange_.start && colRange_.start <= colRange_.end && colRange_.end <= m.cols );

        cols = colRange_.size();
        data += colRange_.start*elemSize();
    }

    जर (refcount)
        CV_XADD(refcount, 1);

    जर (rows <= 0 || cols <= 0)
        rows = cols = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(स्थिरांक GpuMat& m, Rect roi) :
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

    जर (refcount)
        CV_XADD(refcount, 1);

    जर (rows <= 0 || cols <= 0)
        rows = cols = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(पूर्णांक new_cn, पूर्णांक new_rows) स्थिरांक
{
    GpuMat hdr = *हे;

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

शून्य cv::cuda::GpuMat::locateROI(Size& wholeSize, Point& ofs) स्थिरांक
{
    CV_DbgAssert( step > 0 );

    माप_प्रकार esz = elemSize();
    सूचक_भेद_प्रकार delta1 = data - datastart;
    सूचक_भेद_प्रकार delta2 = dataend - datastart;

    जर (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    अन्यथा
    {
        ofs.y = static_cast<पूर्णांक>(delta1 / step);
        ofs.x = static_cast<पूर्णांक>((delta1 - step * ofs.y) / esz);

        CV_DbgAssert( data == datastart + ofs.y * step + ofs.x * esz );
    }

    माप_प्रकार minstep = (ofs.x + cols) * esz;

    wholeSize.height = std::max(static_cast<पूर्णांक>((delta2 - minstep) / step + 1), ofs.y + rows);
    wholeSize.width = std::max(static_cast<पूर्णांक>((delta2 - step * (wholeSize.height - 1)) / esz), ofs.x + cols);
}

GpuMat& cv::cuda::GpuMat::adjustROI(पूर्णांक dtop, पूर्णांक dbottom, पूर्णांक dleft, पूर्णांक dright)
{
    Size wholeSize;
    Point ofs;
    locateROI(wholeSize, ofs);

    माप_प्रकार esz = elemSize();

    पूर्णांक row1 = std::max(ofs.y - dtop, 0);
    पूर्णांक row2 = std::min(ofs.y + rows + dbottom, wholeSize.height);

    पूर्णांक col1 = std::max(ofs.x - dleft, 0);
    पूर्णांक col2 = std::min(ofs.x + cols + dright, wholeSize.width);

    data += (row1 - ofs.y) * step + (col1 - ofs.x) * esz;
    rows = row2 - row1;
    cols = col2 - col1;

    updateContinuityFlag();
    परत *हे;
}

नामाकाश
{
    ढाँचा <वर्ग ObjType>
    शून्य createContinuousImpl(पूर्णांक rows, पूर्णांक cols, पूर्णांक type, ObjType& obj)
    {
        स्थिरांक पूर्णांक area = rows * cols;

        जर (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), rows);
    }
}

शून्य cv::cuda::createContinuous(पूर्णांक rows, पूर्णांक cols, पूर्णांक type, OutputArray arr)
{
    निवड (arr.kind())
    {
    स्थिती आदान_Array_2::MAT:
        ::createContinuousImpl(rows, cols, type, arr.getMatRef());
        थांबा;

    स्थिती आदान_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(rows, cols, type, arr.getGpuMatRef());
        थांबा;

    स्थिती आदान_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(rows, cols, type, arr.getHostMemRef());
        थांबा;

    पूर्वनिर्धारित:
        arr.create(rows, cols, type);
    }
}

नामाकाश
{
    ढाँचा <वर्ग ObjType>
    शून्य ensureSizeIsEnoughImpl(पूर्णांक rows, पूर्णांक cols, पूर्णांक type, ObjType& obj)
    {
        जर (obj.empty() || obj.type() != type || obj.data != obj.datastart)
        {
            obj.create(rows, cols, type);
        }
        अन्यथा
        {
            स्थिरांक माप_प्रकार esz = obj.elemSize();
            स्थिरांक सूचक_भेद_प्रकार delta2 = obj.dataend - obj.datastart;

            स्थिरांक माप_प्रकार minstep = obj.cols * esz;

            Size wholeSize;
            wholeSize.height = std::max(static_cast<पूर्णांक>((delta2 - minstep) / static_cast<माप_प्रकार>(obj.step) + 1), obj.rows);
            wholeSize.width = std::max(static_cast<पूर्णांक>((delta2 - static_cast<माप_प्रकार>(obj.step) * (wholeSize.height - 1)) / esz), obj.cols);

            जर (wholeSize.height < rows || wholeSize.width < cols)
            {
                obj.create(rows, cols, type);
            }
            अन्यथा
            {
                obj.cols = cols;
                obj.rows = rows;
            }
        }
    }
}

शून्य cv::cuda::ensureSizeIsEnough(पूर्णांक rows, पूर्णांक cols, पूर्णांक type, OutputArray arr)
{
    निवड (arr.kind())
    {
    स्थिती आदान_Array_2::MAT:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getMatRef());
        थांबा;

    स्थिती आदान_Array_2::CUDA_GPU_MAT:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getGpuMatRef());
        थांबा;

    स्थिती आदान_Array_2::CUDA_HOST_MEM:
        ::ensureSizeIsEnoughImpl(rows, cols, type, arr.getHostMemRef());
        थांबा;

    पूर्वनिर्धारित:
        arr.create(rows, cols, type);
    }
}

GpuMat cv::cuda::get_आदान_Mat(आदान_Array _src, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(_src);
    CV_UNUSED(stream);
    throw_no_cuda();
#अन्यथा
    GpuMat src;
    जर (_src.kind() == आदान_Array_2::CUDA_GPU_MAT)
    {
        src = _src.getGpuMat();
    }
    अन्यथा जर (!_src.empty())
    {
        BufferPool pool(stream);
        src = pool.getBuffer(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    परत src;
#endif
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, पूर्णांक rows, पूर्णांक cols, पूर्णांक type, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(_dst);
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    CV_UNUSED(stream);
    throw_no_cuda();
#अन्यथा
    GpuMat dst;
    जर (_dst.kind() == आदान_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(rows, cols, type);
        dst = _dst.getGpuMat();
    }
    अन्यथा
    {
        BufferPool pool(stream);
        dst = pool.getBuffer(rows, cols, type);
    }
    परत dst;
#endif
}

शून्य cv::cuda::syncOutput(स्थिरांक GpuMat& dst, OutputArray _dst, Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(dst);
    CV_UNUSED(_dst);
    CV_UNUSED(stream);
    throw_no_cuda();
#अन्यथा
    जर (_dst.kind() != आदान_Array_2::CUDA_GPU_MAT)
    {
        जर (stream)
            dst.download(_dst, stream);
        अन्यथा
            dst.download(_dst);
    }
#endif
}

#ifndef HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    परत 0;
}

शून्य cv::cuda::GpuMat::setDefaultAllocator(Allocator* allocator)
{
    CV_UNUSED(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    परत 0;
}

शून्य cv::cuda::GpuMat::create(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type)
{
    CV_UNUSED(_rows);
    CV_UNUSED(_cols);
    CV_UNUSED(_type);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::release()
{
    जर( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->मुक्त(हे);
    dataend = datastart = data = 0;
    step = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_MASK;
}

शून्य cv::cuda::GpuMat::fit(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    माप_प्रकार oldTotalBytes = (datastart && dataend) ? (माप_प्रकार)(dataend - datastart) : 0;
    माप_प्रकार esz = CV_ELEM_SIZE(_type);
    माप_प्रकार newTotalBytes = (माप_प्रकार)_rows * (माप_प्रकार)_cols * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    जर (newTotalBytes > 0 && (!isContinuous() || newTotalBytes > oldTotalBytes || data != datastart))
    {
        create(_rows, _cols, _type);
        परत;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_MASK) | CV_MAKETYPE(CV_MAT_DEPTH(_type), CV_MAT_CN(_type));
    rows = _rows;
    cols = _cols;
    step = (माप_प्रकार)cols * esz;
}

शून्य cv::cuda::GpuMat::fit(Size _sz, पूर्णांक _type)
{
    fit(_sz.height, _sz.width, _type);
}

शून्य cv::cuda::GpuMat::upload(आदान_Array arr)
{
    CV_UNUSED(arr);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::upload(आदान_Array arr, Stream& _stream)
{
    CV_UNUSED(arr);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::download(OutputArray _dst) स्थिरांक
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::download(OutputArray _dst, Stream& _stream) स्थिरांक
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::copyTo(OutputArray _dst) स्थिरांक
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::copyTo(OutputArray _dst, Stream& _stream) स्थिरांक
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::copyTo(OutputArray _dst, आदान_Array _mask, Stream& _stream) स्थिरांक
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

GpuMat& cv::cuda::GpuMat::setTo(Scalar s, आदान_Array _mask, Stream& _stream)
{
    CV_UNUSED(s);
    CV_UNUSED(_mask);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::convertTo(OutputArray _dst, पूर्णांक rtype, Stream& _stream) स्थिरांक
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::convertTo(OutputArray _dst, पूर्णांक rtype, द्विगुण alpha, द्विगुण beta, Stream& _stream) स्थिरांक
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(alpha);
    CV_UNUSED(beta);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

#endif
