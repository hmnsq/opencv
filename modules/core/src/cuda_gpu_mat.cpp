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

استفاده نطاق cv;
استفاده نطاق cv::cuda;

فراغ cv::cuda::GpuMat::updateContinuityFlag()
{
    صحيح sz[] = { ردیفها, تعداد_ستون };
    size_t گامها[] = { گام, elem_اندازه() };
    flags = cv::updateContinuityFlag(flags, 2, sz, گامها);
}

cv::cuda::GpuMat::GpuMat(صحيح ردیفها_2, صحيح تعداد_ستون_2, صحيح type_, فراغ* داده_2, size_t گام_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_ماسک)), ردیفها(ردیفها_2), تعداد_ستون(تعداد_ستون_2),
    گام(گام_2), داده((uchar*)داده_2), refcount(0),
    datastart((uchar*)داده_2), dataend((ثابت uchar*)داده_2),
    allocator(defaultAllocator())
{
    size_t minstep = تعداد_ستون * elem_اندازه();

    إذا (گام == Mat::AUTO_گام)
    {
        گام = minstep;
    }
    وإلا
    {
        إذا (ردیفها == 1)
            گام = minstep;

        CV_DbgAssert( گام >= minstep );
    }

    dataend += گام * (ردیفها - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(اندازه اندازه_3, صحيح type_, فراغ* داده_2, size_t گام_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_ماسک)), ردیفها(اندازه_3.ارتفاع), تعداد_ستون(اندازه_3.عرض),
    گام(گام_2), داده((uchar*)داده_2), refcount(0),
    datastart((uchar*)داده_2), dataend((ثابت uchar*)داده_2),
    allocator(defaultAllocator())
{
    size_t minstep = تعداد_ستون * elem_اندازه();

    إذا (گام == Mat::AUTO_گام)
    {
        گام = minstep;
    }
    وإلا
    {
        إذا (ردیفها == 1)
            گام = minstep;

        CV_DbgAssert( گام >= minstep );
    }

    dataend += گام * (ردیفها - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ثابت GpuMat& m, Range ردیف_Range, Range ستون_Range)
{
    flags = m.flags;
    گام = m.گام; refcount = m.refcount;
    داده = m.داده; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    إذا (ردیف_Range == Range::all())
    {
        ردیفها = m.ردیفها;
    }
    وإلا
    {
        CV_Assert( 0 <= ردیف_Range.شروع && ردیف_Range.شروع <= ردیف_Range.end && ردیف_Range.end <= m.ردیفها );

        ردیفها = ردیف_Range.size();
        داده += گام*ردیف_Range.شروع;
    }

    إذا (ستون_Range == Range::all())
    {
        تعداد_ستون = m.تعداد_ستون;
    }
    وإلا
    {
        CV_Assert( 0 <= ستون_Range.شروع && ستون_Range.شروع <= ستون_Range.end && ستون_Range.end <= m.تعداد_ستون );

        تعداد_ستون = ستون_Range.size();
        داده += ستون_Range.شروع*elem_اندازه();
    }

    إذا (refcount)
        CV_XADD(refcount, 1);

    إذا (ردیفها <= 0 || تعداد_ستون <= 0)
        ردیفها = تعداد_ستون = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ثابت GpuMat& m, Rect roi) :
    flags(m.flags), ردیفها(roi.ارتفاع), تعداد_ستون(roi.عرض),
    گام(m.گام), داده(m.داده + roi.y*گام), refcount(m.refcount),
    datastart(m.datastart), dataend(m.dataend),
    allocator(m.allocator)
{
    داده += roi.x * elem_اندازه();

    CV_Assert( 0 <= roi.x && 0 <= roi.عرض &&
               roi.x + roi.عرض <= m.تعداد_ستون &&
               0 <= roi.y && 0 <= roi.ارتفاع &&
               roi.y + roi.ارتفاع <= m.ردیفها );

    إذا (refcount)
        CV_XADD(refcount, 1);

    إذا (ردیفها <= 0 || تعداد_ستون <= 0)
        ردیفها = تعداد_ستون = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(صحيح new_cn, صحيح new_ردیفها) ثابت
{
    GpuMat hdr = *هذا;

    صحيح cn = channels();
    إذا (new_cn == 0)
        new_cn = cn;

    صحيح مجموع_عرض = تعداد_ستون * cn;

    إذا ((new_cn > مجموع_عرض || مجموع_عرض % new_cn != 0) && new_ردیفها == 0)
        new_ردیفها = ردیفها * مجموع_عرض / new_cn;

    إذا (new_ردیفها != 0 && new_ردیفها != ردیفها)
    {
        صحيح مجموع_اندازه_2 = مجموع_عرض * ردیفها;

        إذا (!isContinuous())
            CV_Error(cv::Error::Bad_گام, "The matrix is not continuous, thus its number of rows can not be changed");

        إذا ((بیعلامت)new_ردیفها > (بیعلامت)مجموع_اندازه_2)
            CV_Error(cv::Error::Sts_بیرون_از_Range, "Bad new number of rows");

        مجموع_عرض = مجموع_اندازه_2 / new_ردیفها;

        إذا (مجموع_عرض * new_ردیفها != مجموع_اندازه_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.ردیفها = new_ردیفها;
        hdr.گام = مجموع_عرض * elemSize1();
    }

    صحيح new_عرض = مجموع_عرض / new_cn;

    إذا (new_عرض * new_cn != مجموع_عرض)
        CV_Error(cv::Error::Bad_رقم_Channels, "The total width is not divisible by the new number of channels");

    hdr.تعداد_ستون = new_عرض;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ماسک) | ((new_cn - 1) << CV_CN_لغزش);

    أرجع hdr;
}

فراغ cv::cuda::GpuMat::locateROI(اندازه& کامل_اندازه, نقطه& ofs) ثابت
{
    CV_DbgAssert( گام > 0 );

    size_t esz = elem_اندازه();
    ptrdiff_t delta1 = داده - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    إذا (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    وإلا
    {
        ofs.y = static_cast<صحيح>(delta1 / گام);
        ofs.x = static_cast<صحيح>((delta1 - گام * ofs.y) / esz);

        CV_DbgAssert( داده == datastart + ofs.y * گام + ofs.x * esz );
    }

    size_t minstep = (ofs.x + تعداد_ستون) * esz;

    کامل_اندازه.ارتفاع = std::max(static_cast<صحيح>((delta2 - minstep) / گام + 1), ofs.y + ردیفها);
    کامل_اندازه.عرض = std::max(static_cast<صحيح>((delta2 - گام * (کامل_اندازه.ارتفاع - 1)) / esz), ofs.x + تعداد_ستون);
}

GpuMat& cv::cuda::GpuMat::adjustROI(صحيح dtop, صحيح dbottom, صحيح dleft, صحيح dright)
{
    اندازه کامل_اندازه;
    نقطه ofs;
    locateROI(کامل_اندازه, ofs);

    size_t esz = elem_اندازه();

    صحيح row1 = std::max(ofs.y - dtop, 0);
    صحيح row2 = std::min(ofs.y + ردیفها + dbottom, کامل_اندازه.ارتفاع);

    صحيح col1 = std::max(ofs.x - dleft, 0);
    صحيح col2 = std::min(ofs.x + تعداد_ستون + dright, کامل_اندازه.عرض);

    داده += (row1 - ofs.y) * گام + (col1 - ofs.x) * esz;
    ردیفها = row2 - row1;
    تعداد_ستون = col2 - col1;

    updateContinuityFlag();
    أرجع *هذا;
}

نطاق
{
    قالب <صنف ObjType>
    فراغ createContinuousImpl(صحيح ردیفها, صحيح تعداد_ستون, صحيح type, ObjType& obj)
    {
        ثابت صحيح area = ردیفها * تعداد_ستون;

        إذا (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), ردیفها);
    }
}

فراغ cv::cuda::createContinuous(صحيح ردیفها, صحيح تعداد_ستون, صحيح type, OutputArray arr)
{
    بدل (arr.نوع())
    {
    حالة درون_داد_Array_2::MAT:
        ::createContinuousImpl(ردیفها, تعداد_ستون, type, arr.getMatRef());
        اكسر;

    حالة درون_داد_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(ردیفها, تعداد_ستون, type, arr.getGpuMatRef());
        اكسر;

    حالة درون_داد_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(ردیفها, تعداد_ستون, type, arr.getHostMemRef());
        اكسر;

    افتراضي:
        arr.create(ردیفها, تعداد_ستون, type);
    }
}

نطاق
{
    قالب <صنف ObjType>
    فراغ ensure_اندازه_Is_Enough_Impl(صحيح ردیفها, صحيح تعداد_ستون, صحيح type, ObjType& obj)
    {
        إذا (obj.empty() || obj.type() != type || obj.داده != obj.datastart)
        {
            obj.create(ردیفها, تعداد_ستون, type);
        }
        وإلا
        {
            ثابت size_t esz = obj.elem_اندازه();
            ثابت ptrdiff_t delta2 = obj.dataend - obj.datastart;

            ثابت size_t minstep = obj.تعداد_ستون * esz;

            اندازه کامل_اندازه;
            کامل_اندازه.ارتفاع = std::max(static_cast<صحيح>((delta2 - minstep) / static_cast<size_t>(obj.گام) + 1), obj.ردیفها);
            کامل_اندازه.عرض = std::max(static_cast<صحيح>((delta2 - static_cast<size_t>(obj.گام) * (کامل_اندازه.ارتفاع - 1)) / esz), obj.تعداد_ستون);

            إذا (کامل_اندازه.ارتفاع < ردیفها || کامل_اندازه.عرض < تعداد_ستون)
            {
                obj.create(ردیفها, تعداد_ستون, type);
            }
            وإلا
            {
                obj.تعداد_ستون = تعداد_ستون;
                obj.ردیفها = ردیفها;
            }
        }
    }
}

فراغ cv::cuda::ensure_اندازه_Is_Enough(صحيح ردیفها, صحيح تعداد_ستون, صحيح type, OutputArray arr)
{
    بدل (arr.نوع())
    {
    حالة درون_داد_Array_2::MAT:
        ::ensure_اندازه_Is_Enough_Impl(ردیفها, تعداد_ستون, type, arr.getMatRef());
        اكسر;

    حالة درون_داد_Array_2::CUDA_GPU_MAT:
        ::ensure_اندازه_Is_Enough_Impl(ردیفها, تعداد_ستون, type, arr.getGpuMatRef());
        اكسر;

    حالة درون_داد_Array_2::CUDA_HOST_MEM:
        ::ensure_اندازه_Is_Enough_Impl(ردیفها, تعداد_ستون, type, arr.getHostMemRef());
        اكسر;

    افتراضي:
        arr.create(ردیفها, تعداد_ستون, type);
    }
}

GpuMat cv::cuda::get_درون_داد_Mat(درون_داد_Array _src, جریان& stream)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(_src);
    CV_استفادهنشده(stream);
    throw_no_cuda();
#وإلا
    GpuMat src;
    إذا (_src.نوع() == درون_داد_Array_2::CUDA_GPU_MAT)
    {
        src = _src.getGpuMat();
    }
    وإلا إذا (!_src.empty())
    {
        میانگیر_Pool pool(stream);
        src = pool.get_میانگیر(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    أرجع src;
#پایان_اگر
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, صحيح ردیفها, صحيح تعداد_ستون, صحيح type, جریان& stream)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(_dst);
    CV_استفادهنشده(ردیفها);
    CV_استفادهنشده(تعداد_ستون);
    CV_استفادهنشده(type);
    CV_استفادهنشده(stream);
    throw_no_cuda();
#وإلا
    GpuMat dst;
    إذا (_dst.نوع() == درون_داد_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(ردیفها, تعداد_ستون, type);
        dst = _dst.getGpuMat();
    }
    وإلا
    {
        میانگیر_Pool pool(stream);
        dst = pool.get_میانگیر(ردیفها, تعداد_ستون, type);
    }
    أرجع dst;
#پایان_اگر
}

فراغ cv::cuda::syncOutput(ثابت GpuMat& dst, OutputArray _dst, جریان& stream)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(dst);
    CV_استفادهنشده(_dst);
    CV_استفادهنشده(stream);
    throw_no_cuda();
#وإلا
    إذا (_dst.نوع() != درون_داد_Array_2::CUDA_GPU_MAT)
    {
        إذا (stream)
            dst.download(_dst, stream);
        وإلا
            dst.download(_dst);
    }
#پایان_اگر
}

#اگر_تعریف_نشده HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    أرجع 0;
}

فراغ cv::cuda::GpuMat::مجموعه_Default_Allocator(Allocator* allocator)
{
    CV_استفادهنشده(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    أرجع 0;
}

فراغ cv::cuda::GpuMat::create(صحيح ردیفها_3, صحيح تعداد_ستون_3, صحيح _type)
{
    CV_استفادهنشده(ردیفها_3);
    CV_استفادهنشده(تعداد_ستون_3);
    CV_استفادهنشده(_type);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::release()
{
    إذا( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->حرر(هذا);
    dataend = datastart = داده = 0;
    گام = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_ماسک;
}

فراغ cv::cuda::GpuMat::fit(صحيح ردیفها_3, صحيح تعداد_ستون_3, صحيح _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t old_مجموع_بایتها = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_اندازه(_type);
    size_t new_مجموع_بایتها = (size_t)ردیفها_3 * (size_t)تعداد_ستون_3 * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    إذا (new_مجموع_بایتها > 0 && (!isContinuous() || new_مجموع_بایتها > old_مجموع_بایتها || داده != datastart))
    {
        create(ردیفها_3, تعداد_ستون_3, _type);
        أرجع;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_ماسک) | CV_MAKETYPE(CV_MAT_عمق(_type), CV_MAT_CN(_type));
    ردیفها = ردیفها_3;
    تعداد_ستون = تعداد_ستون_3;
    گام = (size_t)تعداد_ستون * esz;
}

فراغ cv::cuda::GpuMat::fit(اندازه _sz, صحيح _type)
{
    fit(_sz.ارتفاع, _sz.عرض, _type);
}

فراغ cv::cuda::GpuMat::upload(درون_داد_Array arr)
{
    CV_استفادهنشده(arr);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::upload(درون_داد_Array arr, جریان& جریان_2)
{
    CV_استفادهنشده(arr);
    CV_استفادهنشده(جریان_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::download(OutputArray _dst) ثابت
{
    CV_استفادهنشده(_dst);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::download(OutputArray _dst, جریان& جریان_2) ثابت
{
    CV_استفادهنشده(_dst);
    CV_استفادهنشده(جریان_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::copy_مقصد(OutputArray _dst) ثابت
{
    CV_استفادهنشده(_dst);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::copy_مقصد(OutputArray _dst, جریان& جریان_2) ثابت
{
    CV_استفادهنشده(_dst);
    CV_استفادهنشده(جریان_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::copy_مقصد(OutputArray _dst, درون_داد_Array ماسک_2, جریان& جریان_2) ثابت
{
    CV_استفادهنشده(_dst);
    CV_استفادهنشده(ماسک_2);
    CV_استفادهنشده(جریان_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::مجموعه_مقصد(Scalar s, جریان& جریان_2)
{
    CV_استفادهنشده(s);
    CV_استفادهنشده(جریان_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::مجموعه_مقصد(Scalar s, درون_داد_Array ماسک_2, جریان& جریان_2)
{
    CV_استفادهنشده(s);
    CV_استفادهنشده(ماسک_2);
    CV_استفادهنشده(جریان_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::convert_مقصد(OutputArray _dst, صحيح rtype, جریان& جریان_2) ثابت
{
    CV_استفادهنشده(_dst);
    CV_استفادهنشده(rtype);
    CV_استفادهنشده(جریان_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::convert_مقصد(OutputArray _dst, صحيح rtype, مزدوج alpha, مزدوج beta, جریان& جریان_2) ثابت
{
    CV_استفادهنشده(_dst);
    CV_استفادهنشده(rtype);
    CV_استفادهنشده(alpha);
    CV_استفادهنشده(beta);
    CV_استفادهنشده(جریان_2);
    throw_no_cuda();
}

#پایان_اگر
