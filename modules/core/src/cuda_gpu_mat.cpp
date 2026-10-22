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

#समावेश "precomp.hpp"

using namespace cv;
using namespace cv::cuda;

व्योम cv::cuda::GpuMat::updateContinuityFlag()
{
    पूर्णांक sz[] = { rows, cols };
    माप_प्रकार steps[] = { step, elem_आकार() };
    flags = cv::updateContinuityFlag(flags, 2, sz, steps);
}

cv::cuda::GpuMat::GpuMat(पूर्णांक rows_, पूर्णांक cols_, पूर्णांक type_, व्योम* तथ्य_2, माप_प्रकार step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(rows_), cols(cols_),
    step(step_), तथ्य((uchar*)तथ्य_2), refcount(0),
    datastart((uchar*)तथ्य_2), dataend((स्थिर uchar*)तथ्य_2),
    allocator(defaultAllocator())
{
    माप_प्रकार minstep = cols * elem_आकार();

    अगर (step == Mat::AUTO_STEP)
    {
        step = minstep;
    }
    अन्यथा
    {
        अगर (rows == 1)
            step = minstep;

        CV_DbgAssert( step >= minstep );
    }

    dataend += step * (rows - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(आकार आकार_3, पूर्णांक type_, व्योम* तथ्य_2, माप_प्रकार step_) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_MASK)), rows(आकार_3.height), cols(आकार_3.width),
    step(step_), तथ्य((uchar*)तथ्य_2), refcount(0),
    datastart((uchar*)तथ्य_2), dataend((स्थिर uchar*)तथ्य_2),
    allocator(defaultAllocator())
{
    माप_प्रकार minstep = cols * elem_आकार();

    अगर (step == Mat::AUTO_STEP)
    {
        step = minstep;
    }
    अन्यथा
    {
        अगर (rows == 1)
            step = minstep;

        CV_DbgAssert( step >= minstep );
    }

    dataend += step * (rows - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(स्थिर GpuMat& m, Range rowRange_, Range colRange_)
{
    flags = m.flags;
    step = m.step; refcount = m.refcount;
    तथ्य = m.तथ्य; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    अगर (rowRange_ == Range::all())
    {
        rows = m.rows;
    }
    अन्यथा
    {
        CV_Assert( 0 <= rowRange_.start && rowRange_.start <= rowRange_.end && rowRange_.end <= m.rows );

        rows = rowRange_.size();
        तथ्य += step*rowRange_.start;
    }

    अगर (colRange_ == Range::all())
    {
        cols = m.cols;
    }
    अन्यथा
    {
        CV_Assert( 0 <= colRange_.start && colRange_.start <= colRange_.end && colRange_.end <= m.cols );

        cols = colRange_.size();
        तथ्य += colRange_.start*elem_आकार();
    }

    अगर (refcount)
        CV_XADD(refcount, 1);

    अगर (rows <= 0 || cols <= 0)
        rows = cols = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(स्थिर GpuMat& m, Rect roi) :
    flags(m.flags), rows(roi.height), cols(roi.width),
    step(m.step), तथ्य(m.तथ्य + roi.y*step), refcount(m.refcount),
    datastart(m.datastart), dataend(m.dataend),
    allocator(m.allocator)
{
    तथ्य += roi.x * elem_आकार();

    CV_Assert( 0 <= roi.x && 0 <= roi.width &&
               roi.x + roi.width <= m.cols &&
               0 <= roi.y && 0 <= roi.height &&
               roi.y + roi.height <= m.rows );

    अगर (refcount)
        CV_XADD(refcount, 1);

    अगर (rows <= 0 || cols <= 0)
        rows = cols = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(पूर्णांक new_cn, पूर्णांक new_rows) स्थिर
{
    GpuMat hdr = *यह;

    पूर्णांक cn = channels();
    अगर (new_cn == 0)
        new_cn = cn;

    पूर्णांक योग_width = cols * cn;

    अगर ((new_cn > योग_width || योग_width % new_cn != 0) && new_rows == 0)
        new_rows = rows * योग_width / new_cn;

    अगर (new_rows != 0 && new_rows != rows)
    {
        पूर्णांक योग_आकार_2 = योग_width * rows;

        अगर (!isContinuous())
            CV_Error(cv::Error::BadStep, "The matrix is not continuous, thus its number of rows can not be changed");

        अगर ((अचिन्हित)new_rows > (अचिन्हित)योग_आकार_2)
            CV_Error(cv::Error::StsOutOfRange, "Bad new number of rows");

        योग_width = योग_आकार_2 / new_rows;

        अगर (योग_width * new_rows != योग_आकार_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.rows = new_rows;
        hdr.step = योग_width * elemSize1();
    }

    पूर्णांक new_width = योग_width / new_cn;

    अगर (new_width * new_cn != योग_width)
        CV_Error(cv::Error::Bad_अंक_Channels, "The total width is not divisible by the new number of channels");

    hdr.cols = new_width;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_MASK) | ((new_cn - 1) << CV_CN_SHIFT);

    वापस hdr;
}

व्योम cv::cuda::GpuMat::locateROI(आकार& whole_आकार, Point& ofs) स्थिर
{
    CV_DbgAssert( step > 0 );

    माप_प्रकार esz = elem_आकार();
    सूचक_भेद_प्रकार delta1 = तथ्य - datastart;
    सूचक_भेद_प्रकार delta2 = dataend - datastart;

    अगर (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    अन्यथा
    {
        ofs.y = static_cast<पूर्णांक>(delta1 / step);
        ofs.x = static_cast<पूर्णांक>((delta1 - step * ofs.y) / esz);

        CV_DbgAssert( तथ्य == datastart + ofs.y * step + ofs.x * esz );
    }

    माप_प्रकार minstep = (ofs.x + cols) * esz;

    whole_आकार.height = std::max(static_cast<पूर्णांक>((delta2 - minstep) / step + 1), ofs.y + rows);
    whole_आकार.width = std::max(static_cast<पूर्णांक>((delta2 - step * (whole_आकार.height - 1)) / esz), ofs.x + cols);
}

GpuMat& cv::cuda::GpuMat::adjustROI(पूर्णांक dtop, पूर्णांक dbottom, पूर्णांक dleft, पूर्णांक dright)
{
    आकार whole_आकार;
    Point ofs;
    locateROI(whole_आकार, ofs);

    माप_प्रकार esz = elem_आकार();

    पूर्णांक row1 = std::max(ofs.y - dtop, 0);
    पूर्णांक row2 = std::min(ofs.y + rows + dbottom, whole_आकार.height);

    पूर्णांक col1 = std::max(ofs.x - dleft, 0);
    पूर्णांक col2 = std::min(ofs.x + cols + dright, whole_आकार.width);

    तथ्य += (row1 - ofs.y) * step + (col1 - ofs.x) * esz;
    rows = row2 - row1;
    cols = col2 - col1;

    updateContinuityFlag();
    वापस *यह;
}

namespace
{
    ढाँचा <श्रेणी ObjType>
    व्योम createContinuousImpl(पूर्णांक rows, पूर्णांक cols, पूर्णांक type, ObjType& obj)
    {
        स्थिर पूर्णांक area = rows * cols;

        अगर (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), rows);
    }
}

व्योम cv::cuda::createContinuous(पूर्णांक rows, पूर्णांक cols, पूर्णांक type, OutputArray arr)
{
    चयन (arr.kind())
    {
    हाल निवेश_Array_2::MAT:
        ::createContinuousImpl(rows, cols, type, arr.getMatRef());
        अवरोध;

    हाल निवेश_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(rows, cols, type, arr.getGpuMatRef());
        अवरोध;

    हाल निवेश_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(rows, cols, type, arr.getHostMemRef());
        अवरोध;

    शेष:
        arr.create(rows, cols, type);
    }
}

namespace
{
    ढाँचा <श्रेणी ObjType>
    व्योम ensure_आकार_Is_Enough_Impl(पूर्णांक rows, पूर्णांक cols, पूर्णांक type, ObjType& obj)
    {
        अगर (obj.empty() || obj.type() != type || obj.तथ्य != obj.datastart)
        {
            obj.create(rows, cols, type);
        }
        अन्यथा
        {
            स्थिर माप_प्रकार esz = obj.elem_आकार();
            स्थिर सूचक_भेद_प्रकार delta2 = obj.dataend - obj.datastart;

            स्थिर माप_प्रकार minstep = obj.cols * esz;

            आकार whole_आकार;
            whole_आकार.height = std::max(static_cast<पूर्णांक>((delta2 - minstep) / static_cast<माप_प्रकार>(obj.step) + 1), obj.rows);
            whole_आकार.width = std::max(static_cast<पूर्णांक>((delta2 - static_cast<माप_प्रकार>(obj.step) * (whole_आकार.height - 1)) / esz), obj.cols);

            अगर (whole_आकार.height < rows || whole_आकार.width < cols)
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

व्योम cv::cuda::ensure_आकार_Is_Enough(पूर्णांक rows, पूर्णांक cols, पूर्णांक type, OutputArray arr)
{
    चयन (arr.kind())
    {
    हाल निवेश_Array_2::MAT:
        ::ensure_आकार_Is_Enough_Impl(rows, cols, type, arr.getMatRef());
        अवरोध;

    हाल निवेश_Array_2::CUDA_GPU_MAT:
        ::ensure_आकार_Is_Enough_Impl(rows, cols, type, arr.getGpuMatRef());
        अवरोध;

    हाल निवेश_Array_2::CUDA_HOST_MEM:
        ::ensure_आकार_Is_Enough_Impl(rows, cols, type, arr.getHostMemRef());
        अवरोध;

    शेष:
        arr.create(rows, cols, type);
    }
}

GpuMat cv::cuda::get_निवेश_Mat(निवेश_Array _src, Stream& stream)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(_src);
    CV_UNUSED(stream);
    throw_no_cuda();
#अन्यथा
    GpuMat src;
    अगर (_src.kind() == निवेश_Array_2::CUDA_GPU_MAT)
    {
        src = _src.getGpuMat();
    }
    अन्यथा अगर (!_src.empty())
    {
        बफर_Pool pool(stream);
        src = pool.get_बफर(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    वापस src;
#पूर्ण_अगर
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, पूर्णांक rows, पूर्णांक cols, पूर्णांक type, Stream& stream)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(_dst);
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    CV_UNUSED(stream);
    throw_no_cuda();
#अन्यथा
    GpuMat dst;
    अगर (_dst.kind() == निवेश_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(rows, cols, type);
        dst = _dst.getGpuMat();
    }
    अन्यथा
    {
        बफर_Pool pool(stream);
        dst = pool.get_बफर(rows, cols, type);
    }
    वापस dst;
#पूर्ण_अगर
}

व्योम cv::cuda::syncOutput(स्थिर GpuMat& dst, OutputArray _dst, Stream& stream)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(dst);
    CV_UNUSED(_dst);
    CV_UNUSED(stream);
    throw_no_cuda();
#अन्यथा
    अगर (_dst.kind() != निवेश_Array_2::CUDA_GPU_MAT)
    {
        अगर (stream)
            dst.download(_dst, stream);
        अन्यथा
            dst.download(_dst);
    }
#पूर्ण_अगर
}

#अगर_अघोषित HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    वापस 0;
}

व्योम cv::cuda::GpuMat::setDefaultAllocator(Allocator* allocator)
{
    CV_UNUSED(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    वापस 0;
}

व्योम cv::cuda::GpuMat::create(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type)
{
    CV_UNUSED(_rows);
    CV_UNUSED(_cols);
    CV_UNUSED(_type);
    throw_no_cuda();
}

व्योम cv::cuda::GpuMat::release()
{
    अगर( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->मुक्त(यह);
    dataend = datastart = तथ्य = 0;
    step = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_MASK;
}

व्योम cv::cuda::GpuMat::fit(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    माप_प्रकार old_योग_Bytes = (datastart && dataend) ? (माप_प्रकार)(dataend - datastart) : 0;
    माप_प्रकार esz = CV_ELEM_आकार(_type);
    माप_प्रकार new_योग_Bytes = (माप_प्रकार)_rows * (माप_प्रकार)_cols * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    अगर (new_योग_Bytes > 0 && (!isContinuous() || new_योग_Bytes > old_योग_Bytes || तथ्य != datastart))
    {
        create(_rows, _cols, _type);
        वापस;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_MASK) | CV_MAKETYPE(CV_MAT_DEPTH(_type), CV_MAT_CN(_type));
    rows = _rows;
    cols = _cols;
    step = (माप_प्रकार)cols * esz;
}

व्योम cv::cuda::GpuMat::fit(आकार _sz, पूर्णांक _type)
{
    fit(_sz.height, _sz.width, _type);
}

व्योम cv::cuda::GpuMat::upload(निवेश_Array arr)
{
    CV_UNUSED(arr);
    throw_no_cuda();
}

व्योम cv::cuda::GpuMat::upload(निवेश_Array arr, Stream& _stream)
{
    CV_UNUSED(arr);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

व्योम cv::cuda::GpuMat::download(OutputArray _dst) स्थिर
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

व्योम cv::cuda::GpuMat::download(OutputArray _dst, Stream& _stream) स्थिर
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

व्योम cv::cuda::GpuMat::copyTo(OutputArray _dst) स्थिर
{
    CV_UNUSED(_dst);
    throw_no_cuda();
}

व्योम cv::cuda::GpuMat::copyTo(OutputArray _dst, Stream& _stream) स्थिर
{
    CV_UNUSED(_dst);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

व्योम cv::cuda::GpuMat::copyTo(OutputArray _dst, निवेश_Array _mask, Stream& _stream) स्थिर
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

GpuMat& cv::cuda::GpuMat::setTo(Scalar s, निवेश_Array _mask, Stream& _stream)
{
    CV_UNUSED(s);
    CV_UNUSED(_mask);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

व्योम cv::cuda::GpuMat::convertTo(OutputArray _dst, पूर्णांक rtype, Stream& _stream) स्थिर
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

व्योम cv::cuda::GpuMat::convertTo(OutputArray _dst, पूर्णांक rtype, द्विगुन alpha, द्विगुन beta, Stream& _stream) स्थिर
{
    CV_UNUSED(_dst);
    CV_UNUSED(rtype);
    CV_UNUSED(alpha);
    CV_UNUSED(beta);
    CV_UNUSED(_stream);
    throw_no_cuda();
}

#पूर्ण_अगर
