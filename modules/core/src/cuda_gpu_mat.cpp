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

#ضمن "precomp.hpp"

کارول نطاق cv;
کارول نطاق cv::cuda;

فراغ cv::cuda::GpuMat::updateContinuityFlag()
{
    صحيح sz[] = { لیکې, ستون_شمېر };
    size_t ګامونه[] = { ګام, elem_اندازه() };
    flags = cv::updateContinuityFlag(flags, 2, sz, ګامونه);
}

cv::cuda::GpuMat::GpuMat(صحيح لیکې_2, صحيح ستون_شمېر_2, صحيح type_, فراغ* معلومات_2, size_t ګام_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_ماسک)), لیکې(لیکې_2), ستون_شمېر(ستون_شمېر_2),
    ګام(ګام_2), معلومات((uchar*)معلومات_2), refcount(0),
    datastart((uchar*)معلومات_2), dataend((ثابت uchar*)معلومات_2),
    allocator(defaultAllocator())
{
    size_t minstep = ستون_شمېر * elem_اندازه();

    إذا (ګام == Mat::AUTO_ګام)
    {
        ګام = minstep;
    }
    وإلا
    {
        إذا (لیکې == 1)
            ګام = minstep;

        CV_DbgAssert( ګام >= minstep );
    }

    dataend += ګام * (لیکې - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(اندازه اندازه_3, صحيح type_, فراغ* معلومات_2, size_t ګام_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_ماسک)), لیکې(اندازه_3.لوړوالی), ستون_شمېر(اندازه_3.پلنوالی),
    ګام(ګام_2), معلومات((uchar*)معلومات_2), refcount(0),
    datastart((uchar*)معلومات_2), dataend((ثابت uchar*)معلومات_2),
    allocator(defaultAllocator())
{
    size_t minstep = ستون_شمېر * elem_اندازه();

    إذا (ګام == Mat::AUTO_ګام)
    {
        ګام = minstep;
    }
    وإلا
    {
        إذا (لیکې == 1)
            ګام = minstep;

        CV_DbgAssert( ګام >= minstep );
    }

    dataend += ګام * (لیکې - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ثابت GpuMat& m, Range لیکه_Range, Range ستون_Range)
{
    flags = m.flags;
    ګام = m.ګام; refcount = m.refcount;
    معلومات = m.معلومات; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    إذا (لیکه_Range == Range::all())
    {
        لیکې = m.لیکې;
    }
    وإلا
    {
        CV_Assert( 0 <= لیکه_Range.پیل && لیکه_Range.پیل <= لیکه_Range.end && لیکه_Range.end <= m.لیکې );

        لیکې = لیکه_Range.size();
        معلومات += ګام*لیکه_Range.پیل;
    }

    إذا (ستون_Range == Range::all())
    {
        ستون_شمېر = m.ستون_شمېر;
    }
    وإلا
    {
        CV_Assert( 0 <= ستون_Range.پیل && ستون_Range.پیل <= ستون_Range.end && ستون_Range.end <= m.ستون_شمېر );

        ستون_شمېر = ستون_Range.size();
        معلومات += ستون_Range.پیل*elem_اندازه();
    }

    إذا (refcount)
        CV_XADD(refcount, 1);

    إذا (لیکې <= 0 || ستون_شمېر <= 0)
        لیکې = ستون_شمېر = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ثابت GpuMat& m, Rect roi) :
    flags(m.flags), لیکې(roi.لوړوالی), ستون_شمېر(roi.پلنوالی),
    ګام(m.ګام), معلومات(m.معلومات + roi.y*ګام), refcount(m.refcount),
    datastart(m.datastart), dataend(m.dataend),
    allocator(m.allocator)
{
    معلومات += roi.x * elem_اندازه();

    CV_Assert( 0 <= roi.x && 0 <= roi.پلنوالی &&
               roi.x + roi.پلنوالی <= m.ستون_شمېر &&
               0 <= roi.y && 0 <= roi.لوړوالی &&
               roi.y + roi.لوړوالی <= m.لیکې );

    إذا (refcount)
        CV_XADD(refcount, 1);

    إذا (لیکې <= 0 || ستون_شمېر <= 0)
        لیکې = ستون_شمېر = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(صحيح new_cn, صحيح new_لیکې) ثابت
{
    GpuMat hdr = *هذا;

    صحيح cn = channels();
    إذا (new_cn == 0)
        new_cn = cn;

    صحيح ټول_پلنوالی = ستون_شمېر * cn;

    إذا ((new_cn > ټول_پلنوالی || ټول_پلنوالی % new_cn != 0) && new_لیکې == 0)
        new_لیکې = لیکې * ټول_پلنوالی / new_cn;

    إذا (new_لیکې != 0 && new_لیکې != لیکې)
    {
        صحيح ټول_اندازه_2 = ټول_پلنوالی * لیکې;

        إذا (!isContinuous())
            CV_Error(cv::Error::Bad_ګام, "The matrix is not continuous, thus its number of rows can not be changed");

        إذا ((بینښه)new_لیکې > (بینښه)ټول_اندازه_2)
            CV_Error(cv::Error::Sts_بهر_د_Range, "Bad new number of rows");

        ټول_پلنوالی = ټول_اندازه_2 / new_لیکې;

        إذا (ټول_پلنوالی * new_لیکې != ټول_اندازه_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.لیکې = new_لیکې;
        hdr.ګام = ټول_پلنوالی * elemSize1();
    }

    صحيح new_پلنوالی = ټول_پلنوالی / new_cn;

    إذا (new_پلنوالی * new_cn != ټول_پلنوالی)
        CV_Error(cv::Error::Bad_عدد_Channels, "The total width is not divisible by the new number of channels");

    hdr.ستون_شمېر = new_پلنوالی;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ماسک) | ((new_cn - 1) << CV_CN_ښويدنه);

    أرجع hdr;
}

فراغ cv::cuda::GpuMat::locateROI(اندازه& ټولیز_اندازه, ټکی& ofs) ثابت
{
    CV_DbgAssert( ګام > 0 );

    size_t esz = elem_اندازه();
    ptrdiff_t delta1 = معلومات - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    إذا (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    وإلا
    {
        ofs.y = static_cast<صحيح>(delta1 / ګام);
        ofs.x = static_cast<صحيح>((delta1 - ګام * ofs.y) / esz);

        CV_DbgAssert( معلومات == datastart + ofs.y * ګام + ofs.x * esz );
    }

    size_t minstep = (ofs.x + ستون_شمېر) * esz;

    ټولیز_اندازه.لوړوالی = std::max(static_cast<صحيح>((delta2 - minstep) / ګام + 1), ofs.y + لیکې);
    ټولیز_اندازه.پلنوالی = std::max(static_cast<صحيح>((delta2 - ګام * (ټولیز_اندازه.لوړوالی - 1)) / esz), ofs.x + ستون_شمېر);
}

GpuMat& cv::cuda::GpuMat::adjustROI(صحيح dtop, صحيح dbottom, صحيح dleft, صحيح dright)
{
    اندازه ټولیز_اندازه;
    ټکی ofs;
    locateROI(ټولیز_اندازه, ofs);

    size_t esz = elem_اندازه();

    صحيح row1 = std::max(ofs.y - dtop, 0);
    صحيح row2 = std::min(ofs.y + لیکې + dbottom, ټولیز_اندازه.لوړوالی);

    صحيح col1 = std::max(ofs.x - dleft, 0);
    صحيح col2 = std::min(ofs.x + ستون_شمېر + dright, ټولیز_اندازه.پلنوالی);

    معلومات += (row1 - ofs.y) * ګام + (col1 - ofs.x) * esz;
    لیکې = row2 - row1;
    ستون_شمېر = col2 - col1;

    updateContinuityFlag();
    أرجع *هذا;
}

نطاق
{
    کينډۍ <صنف ObjType>
    فراغ createContinuousImpl(صحيح لیکې, صحيح ستون_شمېر, صحيح type, ObjType& obj)
    {
        ثابت صحيح area = لیکې * ستون_شمېر;

        إذا (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), لیکې);
    }
}

فراغ cv::cuda::createContinuous(صحيح لیکې, صحيح ستون_شمېر, صحيح type, OutputArray arr)
{
    بدل (arr.ډول())
    {
    حالة ننوت_Array_2::MAT:
        ::createContinuousImpl(لیکې, ستون_شمېر, type, arr.getMatRef());
        اكسر;

    حالة ننوت_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(لیکې, ستون_شمېر, type, arr.getGpuMatRef());
        اكسر;

    حالة ننوت_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(لیکې, ستون_شمېر, type, arr.getHostMemRef());
        اكسر;

    افتراضي:
        arr.create(لیکې, ستون_شمېر, type);
    }
}

نطاق
{
    کينډۍ <صنف ObjType>
    فراغ ensure_اندازه_Is_Enough_Impl(صحيح لیکې, صحيح ستون_شمېر, صحيح type, ObjType& obj)
    {
        إذا (obj.empty() || obj.type() != type || obj.معلومات != obj.datastart)
        {
            obj.create(لیکې, ستون_شمېر, type);
        }
        وإلا
        {
            ثابت size_t esz = obj.elem_اندازه();
            ثابت ptrdiff_t delta2 = obj.dataend - obj.datastart;

            ثابت size_t minstep = obj.ستون_شمېر * esz;

            اندازه ټولیز_اندازه;
            ټولیز_اندازه.لوړوالی = std::max(static_cast<صحيح>((delta2 - minstep) / static_cast<size_t>(obj.ګام) + 1), obj.لیکې);
            ټولیز_اندازه.پلنوالی = std::max(static_cast<صحيح>((delta2 - static_cast<size_t>(obj.ګام) * (ټولیز_اندازه.لوړوالی - 1)) / esz), obj.ستون_شمېر);

            إذا (ټولیز_اندازه.لوړوالی < لیکې || ټولیز_اندازه.پلنوالی < ستون_شمېر)
            {
                obj.create(لیکې, ستون_شمېر, type);
            }
            وإلا
            {
                obj.ستون_شمېر = ستون_شمېر;
                obj.لیکې = لیکې;
            }
        }
    }
}

فراغ cv::cuda::ensure_اندازه_Is_Enough(صحيح لیکې, صحيح ستون_شمېر, صحيح type, OutputArray arr)
{
    بدل (arr.ډول())
    {
    حالة ننوت_Array_2::MAT:
        ::ensure_اندازه_Is_Enough_Impl(لیکې, ستون_شمېر, type, arr.getMatRef());
        اكسر;

    حالة ننوت_Array_2::CUDA_GPU_MAT:
        ::ensure_اندازه_Is_Enough_Impl(لیکې, ستون_شمېر, type, arr.getGpuMatRef());
        اكسر;

    حالة ننوت_Array_2::CUDA_HOST_MEM:
        ::ensure_اندازه_Is_Enough_Impl(لیکې, ستون_شمېر, type, arr.getHostMemRef());
        اكسر;

    افتراضي:
        arr.create(لیکې, ستون_شمېر, type);
    }
}

GpuMat cv::cuda::get_ننوت_Mat(ننوت_Array _src, بهیر& stream)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(_src);
    CV_ناکارول_شوی(stream);
    throw_no_cuda();
#وإلا
    GpuMat src;
    إذا (_src.ډول() == ننوت_Array_2::CUDA_GPU_MAT)
    {
        src = _src.getGpuMat();
    }
    وإلا إذا (!_src.empty())
    {
        بفر_Pool pool(stream);
        src = pool.get_بفر(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    أرجع src;
#د_که_پای
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, صحيح لیکې, صحيح ستون_شمېر, صحيح type, بهیر& stream)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(_dst);
    CV_ناکارول_شوی(لیکې);
    CV_ناکارول_شوی(ستون_شمېر);
    CV_ناکارول_شوی(type);
    CV_ناکارول_شوی(stream);
    throw_no_cuda();
#وإلا
    GpuMat dst;
    إذا (_dst.ډول() == ننوت_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(لیکې, ستون_شمېر, type);
        dst = _dst.getGpuMat();
    }
    وإلا
    {
        بفر_Pool pool(stream);
        dst = pool.get_بفر(لیکې, ستون_شمېر, type);
    }
    أرجع dst;
#د_که_پای
}

فراغ cv::cuda::syncOutput(ثابت GpuMat& dst, OutputArray _dst, بهیر& stream)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(dst);
    CV_ناکارول_شوی(_dst);
    CV_ناکارول_شوی(stream);
    throw_no_cuda();
#وإلا
    إذا (_dst.ډول() != ننوت_Array_2::CUDA_GPU_MAT)
    {
        إذا (stream)
            dst.download(_dst, stream);
        وإلا
            dst.download(_dst);
    }
#د_که_پای
}

#که_نه_تعریف_شوی HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    أرجع 0;
}

فراغ cv::cuda::GpuMat::سېټ_Default_Allocator(Allocator* allocator)
{
    CV_ناکارول_شوی(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    أرجع 0;
}

فراغ cv::cuda::GpuMat::create(صحيح لیکې_3, صحيح ستون_شمېر_3, صحيح _type)
{
    CV_ناکارول_شوی(لیکې_3);
    CV_ناکارول_شوی(ستون_شمېر_3);
    CV_ناکارول_شوی(_type);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::release()
{
    إذا( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->حرر(هذا);
    dataend = datastart = معلومات = 0;
    ګام = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_ماسک;
}

فراغ cv::cuda::GpuMat::fit(صحيح لیکې_3, صحيح ستون_شمېر_3, صحيح _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t old_ټول_بایټونه = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_اندازه(_type);
    size_t new_ټول_بایټونه = (size_t)لیکې_3 * (size_t)ستون_شمېر_3 * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    إذا (new_ټول_بایټونه > 0 && (!isContinuous() || new_ټول_بایټونه > old_ټول_بایټونه || معلومات != datastart))
    {
        create(لیکې_3, ستون_شمېر_3, _type);
        أرجع;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_ماسک) | CV_MAKETYPE(CV_MAT_ژوروالی(_type), CV_MAT_CN(_type));
    لیکې = لیکې_3;
    ستون_شمېر = ستون_شمېر_3;
    ګام = (size_t)ستون_شمېر * esz;
}

فراغ cv::cuda::GpuMat::fit(اندازه _sz, صحيح _type)
{
    fit(_sz.لوړوالی, _sz.پلنوالی, _type);
}

فراغ cv::cuda::GpuMat::upload(ننوت_Array arr)
{
    CV_ناکارول_شوی(arr);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::upload(ننوت_Array arr, بهیر& بهیر_2)
{
    CV_ناکارول_شوی(arr);
    CV_ناکارول_شوی(بهیر_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::download(OutputArray _dst) ثابت
{
    CV_ناکارول_شوی(_dst);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::download(OutputArray _dst, بهیر& بهیر_2) ثابت
{
    CV_ناکارول_شوی(_dst);
    CV_ناکارول_شوی(بهیر_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::copy_منزل(OutputArray _dst) ثابت
{
    CV_ناکارول_شوی(_dst);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::copy_منزل(OutputArray _dst, بهیر& بهیر_2) ثابت
{
    CV_ناکارول_شوی(_dst);
    CV_ناکارول_شوی(بهیر_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::copy_منزل(OutputArray _dst, ننوت_Array ماسک_2, بهیر& بهیر_2) ثابت
{
    CV_ناکارول_شوی(_dst);
    CV_ناکارول_شوی(ماسک_2);
    CV_ناکارول_شوی(بهیر_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::سېټ_منزل(Scalar s, بهیر& بهیر_2)
{
    CV_ناکارول_شوی(s);
    CV_ناکارول_شوی(بهیر_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::سېټ_منزل(Scalar s, ننوت_Array ماسک_2, بهیر& بهیر_2)
{
    CV_ناکارول_شوی(s);
    CV_ناکارول_شوی(ماسک_2);
    CV_ناکارول_شوی(بهیر_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::convert_منزل(OutputArray _dst, صحيح rtype, بهیر& بهیر_2) ثابت
{
    CV_ناکارول_شوی(_dst);
    CV_ناکارول_شوی(rtype);
    CV_ناکارول_شوی(بهیر_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::convert_منزل(OutputArray _dst, صحيح rtype, مزدوج alpha, مزدوج beta, بهیر& بهیر_2) ثابت
{
    CV_ناکارول_شوی(_dst);
    CV_ناکارول_شوی(rtype);
    CV_ناکارول_شوی(alpha);
    CV_ناکارول_شوی(beta);
    CV_ناکارول_شوی(بهیر_2);
    throw_no_cuda();
}

#د_که_پای
