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

ورتوں نطاق cv;
ورتوں نطاق cv::cuda;

فراغ cv::cuda::GpuMat::updateContinuityFlag()
{
    صحيح sz[] = { صفاں, خانے_گنتی };
    size_t قدماں[] = { قدم, elem_ناپ() };
    flags = cv::updateContinuityFlag(flags, 2, sz, قدماں);
}

cv::cuda::GpuMat::GpuMat(صحيح صفاں_2, صحيح خانے_گنتی_2, صحيح type_, فراغ* انکڑے_2, size_t قدم_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_نقاب)), صفاں(صفاں_2), خانے_گنتی(خانے_گنتی_2),
    قدم(قدم_2), انکڑے((uchar*)انکڑے_2), refcount(0),
    datastart((uchar*)انکڑے_2), dataend((ثابت uchar*)انکڑے_2),
    allocator(defaultAllocator())
{
    size_t minstep = خانے_گنتی * elem_ناپ();

    إذا (قدم == Mat::AUTO_قدم)
    {
        قدم = minstep;
    }
    وإلا
    {
        إذا (صفاں == 1)
            قدم = minstep;

        CV_DbgAssert( قدم >= minstep );
    }

    dataend += قدم * (صفاں - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ناپ ناپ_3, صحيح type_, فراغ* انکڑے_2, size_t قدم_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_نقاب)), صفاں(ناپ_3.اچائی), خانے_گنتی(ناپ_3.چوڑائی),
    قدم(قدم_2), انکڑے((uchar*)انکڑے_2), refcount(0),
    datastart((uchar*)انکڑے_2), dataend((ثابت uchar*)انکڑے_2),
    allocator(defaultAllocator())
{
    size_t minstep = خانے_گنتی * elem_ناپ();

    إذا (قدم == Mat::AUTO_قدم)
    {
        قدم = minstep;
    }
    وإلا
    {
        إذا (صفاں == 1)
            قدم = minstep;

        CV_DbgAssert( قدم >= minstep );
    }

    dataend += قدم * (صفاں - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ثابت GpuMat& m, Range صف_Range, Range خانہ_Range)
{
    flags = m.flags;
    قدم = m.قدم; refcount = m.refcount;
    انکڑے = m.انکڑے; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    إذا (صف_Range == Range::all())
    {
        صفاں = m.صفاں;
    }
    وإلا
    {
        CV_Assert( 0 <= صف_Range.شروع && صف_Range.شروع <= صف_Range.end && صف_Range.end <= m.صفاں );

        صفاں = صف_Range.size();
        انکڑے += قدم*صف_Range.شروع;
    }

    إذا (خانہ_Range == Range::all())
    {
        خانے_گنتی = m.خانے_گنتی;
    }
    وإلا
    {
        CV_Assert( 0 <= خانہ_Range.شروع && خانہ_Range.شروع <= خانہ_Range.end && خانہ_Range.end <= m.خانے_گنتی );

        خانے_گنتی = خانہ_Range.size();
        انکڑے += خانہ_Range.شروع*elem_ناپ();
    }

    إذا (refcount)
        CV_XADD(refcount, 1);

    إذا (صفاں <= 0 || خانے_گنتی <= 0)
        صفاں = خانے_گنتی = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ثابت GpuMat& m, Rect roi) :
    flags(m.flags), صفاں(roi.اچائی), خانے_گنتی(roi.چوڑائی),
    قدم(m.قدم), انکڑے(m.انکڑے + roi.y*قدم), refcount(m.refcount),
    datastart(m.datastart), dataend(m.dataend),
    allocator(m.allocator)
{
    انکڑے += roi.x * elem_ناپ();

    CV_Assert( 0 <= roi.x && 0 <= roi.چوڑائی &&
               roi.x + roi.چوڑائی <= m.خانے_گنتی &&
               0 <= roi.y && 0 <= roi.اچائی &&
               roi.y + roi.اچائی <= m.صفاں );

    إذا (refcount)
        CV_XADD(refcount, 1);

    إذا (صفاں <= 0 || خانے_گنتی <= 0)
        صفاں = خانے_گنتی = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(صحيح new_cn, صحيح new_صفاں) ثابت
{
    GpuMat hdr = *هذا;

    صحيح cn = channels();
    إذا (new_cn == 0)
        new_cn = cn;

    صحيح کل_چوڑائی = خانے_گنتی * cn;

    إذا ((new_cn > کل_چوڑائی || کل_چوڑائی % new_cn != 0) && new_صفاں == 0)
        new_صفاں = صفاں * کل_چوڑائی / new_cn;

    إذا (new_صفاں != 0 && new_صفاں != صفاں)
    {
        صحيح کل_ناپ_2 = کل_چوڑائی * صفاں;

        إذا (!isContinuous())
            CV_Error(cv::Error::Bad_قدم, "The matrix is not continuous, thus its number of rows can not be changed");

        إذا ((بنانشانی)new_صفاں > (بنانشانی)کل_ناپ_2)
            CV_Error(cv::Error::Sts_باہرلا_دا_Range, "Bad new number of rows");

        کل_چوڑائی = کل_ناپ_2 / new_صفاں;

        إذا (کل_چوڑائی * new_صفاں != کل_ناپ_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.صفاں = new_صفاں;
        hdr.قدم = کل_چوڑائی * elemSize1();
    }

    صحيح new_چوڑائی = کل_چوڑائی / new_cn;

    إذا (new_چوڑائی * new_cn != کل_چوڑائی)
        CV_Error(cv::Error::Bad_عدد_Channels, "The total width is not divisible by the new number of channels");

    hdr.خانے_گنتی = new_چوڑائی;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn - 1) << CV_CN_کھسکاؤ);

    أرجع hdr;
}

فراغ cv::cuda::GpuMat::locateROI(ناپ& سارا_ناپ, نقطہ& ofs) ثابت
{
    CV_DbgAssert( قدم > 0 );

    size_t esz = elem_ناپ();
    ptrdiff_t delta1 = انکڑے - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    إذا (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    وإلا
    {
        ofs.y = static_cast<صحيح>(delta1 / قدم);
        ofs.x = static_cast<صحيح>((delta1 - قدم * ofs.y) / esz);

        CV_DbgAssert( انکڑے == datastart + ofs.y * قدم + ofs.x * esz );
    }

    size_t minstep = (ofs.x + خانے_گنتی) * esz;

    سارا_ناپ.اچائی = std::max(static_cast<صحيح>((delta2 - minstep) / قدم + 1), ofs.y + صفاں);
    سارا_ناپ.چوڑائی = std::max(static_cast<صحيح>((delta2 - قدم * (سارا_ناپ.اچائی - 1)) / esz), ofs.x + خانے_گنتی);
}

GpuMat& cv::cuda::GpuMat::adjustROI(صحيح dtop, صحيح dbottom, صحيح dleft, صحيح dright)
{
    ناپ سارا_ناپ;
    نقطہ ofs;
    locateROI(سارا_ناپ, ofs);

    size_t esz = elem_ناپ();

    صحيح row1 = std::max(ofs.y - dtop, 0);
    صحيح row2 = std::min(ofs.y + صفاں + dbottom, سارا_ناپ.اچائی);

    صحيح col1 = std::max(ofs.x - dleft, 0);
    صحيح col2 = std::min(ofs.x + خانے_گنتی + dright, سارا_ناپ.چوڑائی);

    انکڑے += (row1 - ofs.y) * قدم + (col1 - ofs.x) * esz;
    صفاں = row2 - row1;
    خانے_گنتی = col2 - col1;

    updateContinuityFlag();
    أرجع *هذا;
}

نطاق
{
    سانچا <صنف ObjType>
    فراغ createContinuousImpl(صحيح صفاں, صحيح خانے_گنتی, صحيح type, ObjType& obj)
    {
        ثابت صحيح area = صفاں * خانے_گنتی;

        إذا (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), صفاں);
    }
}

فراغ cv::cuda::createContinuous(صحيح صفاں, صحيح خانے_گنتی, صحيح type, OutputArray arr)
{
    بدل (arr.قسم())
    {
    حالة داخلہ_Array_2::MAT:
        ::createContinuousImpl(صفاں, خانے_گنتی, type, arr.getMatRef());
        اكسر;

    حالة داخلہ_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(صفاں, خانے_گنتی, type, arr.getGpuMatRef());
        اكسر;

    حالة داخلہ_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(صفاں, خانے_گنتی, type, arr.getHostMemRef());
        اكسر;

    افتراضي:
        arr.create(صفاں, خانے_گنتی, type);
    }
}

نطاق
{
    سانچا <صنف ObjType>
    فراغ ensure_ناپ_Is_Enough_Impl(صحيح صفاں, صحيح خانے_گنتی, صحيح type, ObjType& obj)
    {
        إذا (obj.empty() || obj.type() != type || obj.انکڑے != obj.datastart)
        {
            obj.create(صفاں, خانے_گنتی, type);
        }
        وإلا
        {
            ثابت size_t esz = obj.elem_ناپ();
            ثابت ptrdiff_t delta2 = obj.dataend - obj.datastart;

            ثابت size_t minstep = obj.خانے_گنتی * esz;

            ناپ سارا_ناپ;
            سارا_ناپ.اچائی = std::max(static_cast<صحيح>((delta2 - minstep) / static_cast<size_t>(obj.قدم) + 1), obj.صفاں);
            سارا_ناپ.چوڑائی = std::max(static_cast<صحيح>((delta2 - static_cast<size_t>(obj.قدم) * (سارا_ناپ.اچائی - 1)) / esz), obj.خانے_گنتی);

            إذا (سارا_ناپ.اچائی < صفاں || سارا_ناپ.چوڑائی < خانے_گنتی)
            {
                obj.create(صفاں, خانے_گنتی, type);
            }
            وإلا
            {
                obj.خانے_گنتی = خانے_گنتی;
                obj.صفاں = صفاں;
            }
        }
    }
}

فراغ cv::cuda::ensure_ناپ_Is_Enough(صحيح صفاں, صحيح خانے_گنتی, صحيح type, OutputArray arr)
{
    بدل (arr.قسم())
    {
    حالة داخلہ_Array_2::MAT:
        ::ensure_ناپ_Is_Enough_Impl(صفاں, خانے_گنتی, type, arr.getMatRef());
        اكسر;

    حالة داخلہ_Array_2::CUDA_GPU_MAT:
        ::ensure_ناپ_Is_Enough_Impl(صفاں, خانے_گنتی, type, arr.getGpuMatRef());
        اكسر;

    حالة داخلہ_Array_2::CUDA_HOST_MEM:
        ::ensure_ناپ_Is_Enough_Impl(صفاں, خانے_گنتی, type, arr.getHostMemRef());
        اكسر;

    افتراضي:
        arr.create(صفاں, خانے_گنتی, type);
    }
}

GpuMat cv::cuda::get_داخلہ_Mat(داخلہ_Array _src, دھارا& stream)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(_src);
    CV_بن_ورتیا(stream);
    throw_no_cuda();
#وإلا
    GpuMat src;
    إذا (_src.قسم() == داخلہ_Array_2::CUDA_GPU_MAT)
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
#جے_انت
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, صحيح صفاں, صحيح خانے_گنتی, صحيح type, دھارا& stream)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(_dst);
    CV_بن_ورتیا(صفاں);
    CV_بن_ورتیا(خانے_گنتی);
    CV_بن_ورتیا(type);
    CV_بن_ورتیا(stream);
    throw_no_cuda();
#وإلا
    GpuMat dst;
    إذا (_dst.قسم() == داخلہ_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(صفاں, خانے_گنتی, type);
        dst = _dst.getGpuMat();
    }
    وإلا
    {
        بفر_Pool pool(stream);
        dst = pool.get_بفر(صفاں, خانے_گنتی, type);
    }
    أرجع dst;
#جے_انت
}

فراغ cv::cuda::syncOutput(ثابت GpuMat& dst, OutputArray _dst, دھارا& stream)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(dst);
    CV_بن_ورتیا(_dst);
    CV_بن_ورتیا(stream);
    throw_no_cuda();
#وإلا
    إذا (_dst.قسم() != داخلہ_Array_2::CUDA_GPU_MAT)
    {
        إذا (stream)
            dst.download(_dst, stream);
        وإلا
            dst.download(_dst);
    }
#جے_انت
}

#جے_بنا_تعریف HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    أرجع 0;
}

فراغ cv::cuda::GpuMat::سیٹ_Default_Allocator(Allocator* allocator)
{
    CV_بن_ورتیا(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    أرجع 0;
}

فراغ cv::cuda::GpuMat::create(صحيح صفاں_3, صحيح خانے_گنتی_3, صحيح _type)
{
    CV_بن_ورتیا(صفاں_3);
    CV_بن_ورتیا(خانے_گنتی_3);
    CV_بن_ورتیا(_type);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::release()
{
    إذا( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->حرر(هذا);
    dataend = datastart = انکڑے = 0;
    قدم = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_نقاب;
}

فراغ cv::cuda::GpuMat::fit(صحيح صفاں_3, صحيح خانے_گنتی_3, صحيح _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t old_کل_بائٹاں = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_ناپ(_type);
    size_t new_کل_بائٹاں = (size_t)صفاں_3 * (size_t)خانے_گنتی_3 * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    إذا (new_کل_بائٹاں > 0 && (!isContinuous() || new_کل_بائٹاں > old_کل_بائٹاں || انکڑے != datastart))
    {
        create(صفاں_3, خانے_گنتی_3, _type);
        أرجع;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_نقاب) | CV_MAKETYPE(CV_MAT_ڈونگھائی(_type), CV_MAT_CN(_type));
    صفاں = صفاں_3;
    خانے_گنتی = خانے_گنتی_3;
    قدم = (size_t)خانے_گنتی * esz;
}

فراغ cv::cuda::GpuMat::fit(ناپ _sz, صحيح _type)
{
    fit(_sz.اچائی, _sz.چوڑائی, _type);
}

فراغ cv::cuda::GpuMat::upload(داخلہ_Array arr)
{
    CV_بن_ورتیا(arr);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::upload(داخلہ_Array arr, دھارا& دھارا_2)
{
    CV_بن_ورتیا(arr);
    CV_بن_ورتیا(دھارا_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::download(OutputArray _dst) ثابت
{
    CV_بن_ورتیا(_dst);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::download(OutputArray _dst, دھارا& دھارا_2) ثابت
{
    CV_بن_ورتیا(_dst);
    CV_بن_ورتیا(دھارا_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::copy_منزل(OutputArray _dst) ثابت
{
    CV_بن_ورتیا(_dst);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::copy_منزل(OutputArray _dst, دھارا& دھارا_2) ثابت
{
    CV_بن_ورتیا(_dst);
    CV_بن_ورتیا(دھارا_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::copy_منزل(OutputArray _dst, داخلہ_Array نقاب_2, دھارا& دھارا_2) ثابت
{
    CV_بن_ورتیا(_dst);
    CV_بن_ورتیا(نقاب_2);
    CV_بن_ورتیا(دھارا_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::سیٹ_منزل(Scalar s, دھارا& دھارا_2)
{
    CV_بن_ورتیا(s);
    CV_بن_ورتیا(دھارا_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::سیٹ_منزل(Scalar s, داخلہ_Array نقاب_2, دھارا& دھارا_2)
{
    CV_بن_ورتیا(s);
    CV_بن_ورتیا(نقاب_2);
    CV_بن_ورتیا(دھارا_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::convert_منزل(OutputArray _dst, صحيح rtype, دھارا& دھارا_2) ثابت
{
    CV_بن_ورتیا(_dst);
    CV_بن_ورتیا(rtype);
    CV_بن_ورتیا(دھارا_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::convert_منزل(OutputArray _dst, صحيح rtype, مزدوج alpha, مزدوج beta, دھارا& دھارا_2) ثابت
{
    CV_بن_ورتیا(_dst);
    CV_بن_ورتیا(rtype);
    CV_بن_ورتیا(alpha);
    CV_بن_ورتیا(beta);
    CV_بن_ورتیا(دھارا_2);
    throw_no_cuda();
}

#جے_انت
