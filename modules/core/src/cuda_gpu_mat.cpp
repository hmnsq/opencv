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

#شامل "precomp.hpp"

استعمال نامخلا cv;
استعمال نامخلا cv::cuda;

خالی cv::cuda::GpuMat::updateContinuityFlag()
{
    صحیح sz[] = { صفیں, ستون_تعداد };
    size_t اقدام[] = { قدم, elem_سائز() };
    flags = cv::updateContinuityFlag(flags, 2, sz, اقدام);
}

cv::cuda::GpuMat::GpuMat(صحیح صفیں_2, صحیح ستون_تعداد_2, صحیح type_, خالی* ڈیٹا_2, size_t قدم_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_نقاب)), صفیں(صفیں_2), ستون_تعداد(ستون_تعداد_2),
    قدم(قدم_2), ڈیٹا((uchar*)ڈیٹا_2), refcount(0),
    datastart((uchar*)ڈیٹا_2), dataend((ثابت uchar*)ڈیٹا_2),
    allocator(defaultAllocator())
{
    size_t minstep = ستون_تعداد * elem_سائز();

    اگر (قدم == Mat::AUTO_قدم)
    {
        قدم = minstep;
    }
    ورنہ
    {
        اگر (صفیں == 1)
            قدم = minstep;

        CV_DbgAssert( قدم >= minstep );
    }

    dataend += قدم * (صفیں - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(سائز سائز_3, صحیح type_, خالی* ڈیٹا_2, size_t قدم_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_نقاب)), صفیں(سائز_3.اونچائی), ستون_تعداد(سائز_3.چوڑائی),
    قدم(قدم_2), ڈیٹا((uchar*)ڈیٹا_2), refcount(0),
    datastart((uchar*)ڈیٹا_2), dataend((ثابت uchar*)ڈیٹا_2),
    allocator(defaultAllocator())
{
    size_t minstep = ستون_تعداد * elem_سائز();

    اگر (قدم == Mat::AUTO_قدم)
    {
        قدم = minstep;
    }
    ورنہ
    {
        اگر (صفیں == 1)
            قدم = minstep;

        CV_DbgAssert( قدم >= minstep );
    }

    dataend += قدم * (صفیں - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ثابت GpuMat& m, Range صف_Range, Range ستون_Range)
{
    flags = m.flags;
    قدم = m.قدم; refcount = m.refcount;
    ڈیٹا = m.ڈیٹا; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    اگر (صف_Range == Range::all())
    {
        صفیں = m.صفیں;
    }
    ورنہ
    {
        CV_Assert( 0 <= صف_Range.آغاز && صف_Range.آغاز <= صف_Range.end && صف_Range.end <= m.صفیں );

        صفیں = صف_Range.size();
        ڈیٹا += قدم*صف_Range.آغاز;
    }

    اگر (ستون_Range == Range::all())
    {
        ستون_تعداد = m.ستون_تعداد;
    }
    ورنہ
    {
        CV_Assert( 0 <= ستون_Range.آغاز && ستون_Range.آغاز <= ستون_Range.end && ستون_Range.end <= m.ستون_تعداد );

        ستون_تعداد = ستون_Range.size();
        ڈیٹا += ستون_Range.آغاز*elem_سائز();
    }

    اگر (refcount)
        CV_XADD(refcount, 1);

    اگر (صفیں <= 0 || ستون_تعداد <= 0)
        صفیں = ستون_تعداد = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ثابت GpuMat& m, Rect roi) :
    flags(m.flags), صفیں(roi.اونچائی), ستون_تعداد(roi.چوڑائی),
    قدم(m.قدم), ڈیٹا(m.ڈیٹا + roi.y*قدم), refcount(m.refcount),
    datastart(m.datastart), dataend(m.dataend),
    allocator(m.allocator)
{
    ڈیٹا += roi.x * elem_سائز();

    CV_Assert( 0 <= roi.x && 0 <= roi.چوڑائی &&
               roi.x + roi.چوڑائی <= m.ستون_تعداد &&
               0 <= roi.y && 0 <= roi.اونچائی &&
               roi.y + roi.اونچائی <= m.صفیں );

    اگر (refcount)
        CV_XADD(refcount, 1);

    اگر (صفیں <= 0 || ستون_تعداد <= 0)
        صفیں = ستون_تعداد = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(صحیح new_cn, صحیح new_صفیں) ثابت
{
    GpuMat hdr = *یہ;

    صحیح cn = channels();
    اگر (new_cn == 0)
        new_cn = cn;

    صحیح کل_چوڑائی = ستون_تعداد * cn;

    اگر ((new_cn > کل_چوڑائی || کل_چوڑائی % new_cn != 0) && new_صفیں == 0)
        new_صفیں = صفیں * کل_چوڑائی / new_cn;

    اگر (new_صفیں != 0 && new_صفیں != صفیں)
    {
        صحیح کل_سائز_2 = کل_چوڑائی * صفیں;

        اگر (!isContinuous())
            CV_Error(cv::Error::Bad_قدم, "The matrix is not continuous, thus its number of rows can not be changed");

        اگر ((غیرعلامتی)new_صفیں > (غیرعلامتی)کل_سائز_2)
            CV_Error(cv::Error::Sts_برآمد_کا_Range, "Bad new number of rows");

        کل_چوڑائی = کل_سائز_2 / new_صفیں;

        اگر (کل_چوڑائی * new_صفیں != کل_سائز_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.صفیں = new_صفیں;
        hdr.قدم = کل_چوڑائی * elemSize1();
    }

    صحیح new_چوڑائی = کل_چوڑائی / new_cn;

    اگر (new_چوڑائی * new_cn != کل_چوڑائی)
        CV_Error(cv::Error::Bad_عدد_Channels, "The total width is not divisible by the new number of channels");

    hdr.ستون_تعداد = new_چوڑائی;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn - 1) << CV_CN_سرکاؤ);

    واپس hdr;
}

خالی cv::cuda::GpuMat::locateROI(سائز& پورا_سائز, نقطہ& ofs) ثابت
{
    CV_DbgAssert( قدم > 0 );

    size_t esz = elem_سائز();
    ptrdiff_t delta1 = ڈیٹا - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    اگر (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    ورنہ
    {
        ofs.y = static_cast<صحیح>(delta1 / قدم);
        ofs.x = static_cast<صحیح>((delta1 - قدم * ofs.y) / esz);

        CV_DbgAssert( ڈیٹا == datastart + ofs.y * قدم + ofs.x * esz );
    }

    size_t minstep = (ofs.x + ستون_تعداد) * esz;

    پورا_سائز.اونچائی = std::max(static_cast<صحیح>((delta2 - minstep) / قدم + 1), ofs.y + صفیں);
    پورا_سائز.چوڑائی = std::max(static_cast<صحیح>((delta2 - قدم * (پورا_سائز.اونچائی - 1)) / esz), ofs.x + ستون_تعداد);
}

GpuMat& cv::cuda::GpuMat::adjustROI(صحیح dtop, صحیح dbottom, صحیح dleft, صحیح dright)
{
    سائز پورا_سائز;
    نقطہ ofs;
    locateROI(پورا_سائز, ofs);

    size_t esz = elem_سائز();

    صحیح row1 = std::max(ofs.y - dtop, 0);
    صحیح row2 = std::min(ofs.y + صفیں + dbottom, پورا_سائز.اونچائی);

    صحیح col1 = std::max(ofs.x - dleft, 0);
    صحیح col2 = std::min(ofs.x + ستون_تعداد + dright, پورا_سائز.چوڑائی);

    ڈیٹا += (row1 - ofs.y) * قدم + (col1 - ofs.x) * esz;
    صفیں = row2 - row1;
    ستون_تعداد = col2 - col1;

    updateContinuityFlag();
    واپس *یہ;
}

نامخلا
{
    سانچہ <جماعت ObjType>
    خالی createContinuousImpl(صحیح صفیں, صحیح ستون_تعداد, صحیح type, ObjType& obj)
    {
        ثابت صحیح area = صفیں * ستون_تعداد;

        اگر (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), صفیں);
    }
}

خالی cv::cuda::createContinuous(صحیح صفیں, صحیح ستون_تعداد, صحیح type, OutputArray arr)
{
    انتخاب (arr.قسم())
    {
    حالت داخلہ_Array_2::MAT:
        ::createContinuousImpl(صفیں, ستون_تعداد, type, arr.getMatRef());
        توڑو;

    حالت داخلہ_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(صفیں, ستون_تعداد, type, arr.getGpuMatRef());
        توڑو;

    حالت داخلہ_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(صفیں, ستون_تعداد, type, arr.getHostMemRef());
        توڑو;

    طےشدہ:
        arr.create(صفیں, ستون_تعداد, type);
    }
}

نامخلا
{
    سانچہ <جماعت ObjType>
    خالی ensure_سائز_Is_Enough_Impl(صحیح صفیں, صحیح ستون_تعداد, صحیح type, ObjType& obj)
    {
        اگر (obj.empty() || obj.type() != type || obj.ڈیٹا != obj.datastart)
        {
            obj.create(صفیں, ستون_تعداد, type);
        }
        ورنہ
        {
            ثابت size_t esz = obj.elem_سائز();
            ثابت ptrdiff_t delta2 = obj.dataend - obj.datastart;

            ثابت size_t minstep = obj.ستون_تعداد * esz;

            سائز پورا_سائز;
            پورا_سائز.اونچائی = std::max(static_cast<صحیح>((delta2 - minstep) / static_cast<size_t>(obj.قدم) + 1), obj.صفیں);
            پورا_سائز.چوڑائی = std::max(static_cast<صحیح>((delta2 - static_cast<size_t>(obj.قدم) * (پورا_سائز.اونچائی - 1)) / esz), obj.ستون_تعداد);

            اگر (پورا_سائز.اونچائی < صفیں || پورا_سائز.چوڑائی < ستون_تعداد)
            {
                obj.create(صفیں, ستون_تعداد, type);
            }
            ورنہ
            {
                obj.ستون_تعداد = ستون_تعداد;
                obj.صفیں = صفیں;
            }
        }
    }
}

خالی cv::cuda::ensure_سائز_Is_Enough(صحیح صفیں, صحیح ستون_تعداد, صحیح type, OutputArray arr)
{
    انتخاب (arr.قسم())
    {
    حالت داخلہ_Array_2::MAT:
        ::ensure_سائز_Is_Enough_Impl(صفیں, ستون_تعداد, type, arr.getMatRef());
        توڑو;

    حالت داخلہ_Array_2::CUDA_GPU_MAT:
        ::ensure_سائز_Is_Enough_Impl(صفیں, ستون_تعداد, type, arr.getGpuMatRef());
        توڑو;

    حالت داخلہ_Array_2::CUDA_HOST_MEM:
        ::ensure_سائز_Is_Enough_Impl(صفیں, ستون_تعداد, type, arr.getHostMemRef());
        توڑو;

    طےشدہ:
        arr.create(صفیں, ستون_تعداد, type);
    }
}

GpuMat cv::cuda::get_داخلہ_Mat(داخلہ_Array _src, دھارا& stream)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(_src);
    CV_غیر_مستعمل(stream);
    throw_no_cuda();
#ورنہ
    GpuMat src;
    اگر (_src.قسم() == داخلہ_Array_2::CUDA_GPU_MAT)
    {
        src = _src.getGpuMat();
    }
    ورنہ اگر (!_src.empty())
    {
        بفر_Pool pool(stream);
        src = pool.get_بفر(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    واپس src;
#اگر_ختم
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, صحیح صفیں, صحیح ستون_تعداد, صحیح type, دھارا& stream)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(_dst);
    CV_غیر_مستعمل(صفیں);
    CV_غیر_مستعمل(ستون_تعداد);
    CV_غیر_مستعمل(type);
    CV_غیر_مستعمل(stream);
    throw_no_cuda();
#ورنہ
    GpuMat dst;
    اگر (_dst.قسم() == داخلہ_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(صفیں, ستون_تعداد, type);
        dst = _dst.getGpuMat();
    }
    ورنہ
    {
        بفر_Pool pool(stream);
        dst = pool.get_بفر(صفیں, ستون_تعداد, type);
    }
    واپس dst;
#اگر_ختم
}

خالی cv::cuda::syncOutput(ثابت GpuMat& dst, OutputArray _dst, دھارا& stream)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(dst);
    CV_غیر_مستعمل(_dst);
    CV_غیر_مستعمل(stream);
    throw_no_cuda();
#ورنہ
    اگر (_dst.قسم() != داخلہ_Array_2::CUDA_GPU_MAT)
    {
        اگر (stream)
            dst.download(_dst, stream);
        ورنہ
            dst.download(_dst);
    }
#اگر_ختم
}

#اگر_غیر_تعریف_شدہ HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    واپس 0;
}

خالی cv::cuda::GpuMat::سیٹ_Default_Allocator(Allocator* allocator)
{
    CV_غیر_مستعمل(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    واپس 0;
}

خالی cv::cuda::GpuMat::create(صحیح صفیں_3, صحیح ستون_تعداد_3, صحیح _type)
{
    CV_غیر_مستعمل(صفیں_3);
    CV_غیر_مستعمل(ستون_تعداد_3);
    CV_غیر_مستعمل(_type);
    throw_no_cuda();
}

خالی cv::cuda::GpuMat::release()
{
    اگر( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->آزاد(یہ);
    dataend = datastart = ڈیٹا = 0;
    قدم = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_نقاب;
}

خالی cv::cuda::GpuMat::fit(صحیح صفیں_3, صحیح ستون_تعداد_3, صحیح _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t old_کل_بائٹیں = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_سائز(_type);
    size_t new_کل_بائٹیں = (size_t)صفیں_3 * (size_t)ستون_تعداد_3 * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    اگر (new_کل_بائٹیں > 0 && (!isContinuous() || new_کل_بائٹیں > old_کل_بائٹیں || ڈیٹا != datastart))
    {
        create(صفیں_3, ستون_تعداد_3, _type);
        واپس;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_نقاب) | CV_MAKETYPE(CV_MAT_گہرائی(_type), CV_MAT_CN(_type));
    صفیں = صفیں_3;
    ستون_تعداد = ستون_تعداد_3;
    قدم = (size_t)ستون_تعداد * esz;
}

خالی cv::cuda::GpuMat::fit(سائز _sz, صحیح _type)
{
    fit(_sz.اونچائی, _sz.چوڑائی, _type);
}

خالی cv::cuda::GpuMat::upload(داخلہ_Array arr)
{
    CV_غیر_مستعمل(arr);
    throw_no_cuda();
}

خالی cv::cuda::GpuMat::upload(داخلہ_Array arr, دھارا& دھارا_2)
{
    CV_غیر_مستعمل(arr);
    CV_غیر_مستعمل(دھارا_2);
    throw_no_cuda();
}

خالی cv::cuda::GpuMat::download(OutputArray _dst) ثابت
{
    CV_غیر_مستعمل(_dst);
    throw_no_cuda();
}

خالی cv::cuda::GpuMat::download(OutputArray _dst, دھارا& دھارا_2) ثابت
{
    CV_غیر_مستعمل(_dst);
    CV_غیر_مستعمل(دھارا_2);
    throw_no_cuda();
}

خالی cv::cuda::GpuMat::copy_مقصد(OutputArray _dst) ثابت
{
    CV_غیر_مستعمل(_dst);
    throw_no_cuda();
}

خالی cv::cuda::GpuMat::copy_مقصد(OutputArray _dst, دھارا& دھارا_2) ثابت
{
    CV_غیر_مستعمل(_dst);
    CV_غیر_مستعمل(دھارا_2);
    throw_no_cuda();
}

خالی cv::cuda::GpuMat::copy_مقصد(OutputArray _dst, داخلہ_Array نقاب_2, دھارا& دھارا_2) ثابت
{
    CV_غیر_مستعمل(_dst);
    CV_غیر_مستعمل(نقاب_2);
    CV_غیر_مستعمل(دھارا_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::سیٹ_مقصد(Scalar s, دھارا& دھارا_2)
{
    CV_غیر_مستعمل(s);
    CV_غیر_مستعمل(دھارا_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::سیٹ_مقصد(Scalar s, داخلہ_Array نقاب_2, دھارا& دھارا_2)
{
    CV_غیر_مستعمل(s);
    CV_غیر_مستعمل(نقاب_2);
    CV_غیر_مستعمل(دھارا_2);
    throw_no_cuda();
}

خالی cv::cuda::GpuMat::convert_مقصد(OutputArray _dst, صحیح rtype, دھارا& دھارا_2) ثابت
{
    CV_غیر_مستعمل(_dst);
    CV_غیر_مستعمل(rtype);
    CV_غیر_مستعمل(دھارا_2);
    throw_no_cuda();
}

خالی cv::cuda::GpuMat::convert_مقصد(OutputArray _dst, صحیح rtype, دوہرا alpha, دوہرا beta, دھارا& دھارا_2) ثابت
{
    CV_غیر_مستعمل(_dst);
    CV_غیر_مستعمل(rtype);
    CV_غیر_مستعمل(alpha);
    CV_غیر_مستعمل(beta);
    CV_غیر_مستعمل(دھارا_2);
    throw_no_cuda();
}

#اگر_ختم
