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

استعمال نطاق cv;
استعمال نطاق cv::cuda;

فراغ cv::cuda::GpuMat::updateContinuityFlag()
{
    صحيح sz[] = { صفون, کانن_ڳڻپ };
    size_t قدمن[] = { قدم, elem_سائيز() };
    flags = cv::updateContinuityFlag(flags, 2, sz, قدمن);
}

cv::cuda::GpuMat::GpuMat(صحيح صفون_2, صحيح کانن_ڳڻپ_2, صحيح type_, فراغ* معلومات_2, size_t قدم_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_نقاب)), صفون(صفون_2), کانن_ڳڻپ(کانن_ڳڻپ_2),
    قدم(قدم_2), معلومات((uchar*)معلومات_2), refcount(0),
    datastart((uchar*)معلومات_2), dataend((ثابت uchar*)معلومات_2),
    allocator(defaultAllocator())
{
    size_t minstep = کانن_ڳڻپ * elem_سائيز();

    إذا (قدم == Mat::AUTO_قدم)
    {
        قدم = minstep;
    }
    وإلا
    {
        إذا (صفون == 1)
            قدم = minstep;

        CV_DbgAssert( قدم >= minstep );
    }

    dataend += قدم * (صفون - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(سائيز سائيز_3, صحيح type_, فراغ* معلومات_2, size_t قدم_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_نقاب)), صفون(سائيز_3.اوچائي), کانن_ڳڻپ(سائيز_3.ويڪر),
    قدم(قدم_2), معلومات((uchar*)معلومات_2), refcount(0),
    datastart((uchar*)معلومات_2), dataend((ثابت uchar*)معلومات_2),
    allocator(defaultAllocator())
{
    size_t minstep = کانن_ڳڻپ * elem_سائيز();

    إذا (قدم == Mat::AUTO_قدم)
    {
        قدم = minstep;
    }
    وإلا
    {
        إذا (صفون == 1)
            قدم = minstep;

        CV_DbgAssert( قدم >= minstep );
    }

    dataend += قدم * (صفون - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ثابت GpuMat& m, Range صف_Range, Range کانو_Range)
{
    flags = m.flags;
    قدم = m.قدم; refcount = m.refcount;
    معلومات = m.معلومات; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    إذا (صف_Range == Range::all())
    {
        صفون = m.صفون;
    }
    وإلا
    {
        CV_Assert( 0 <= صف_Range.شروعات && صف_Range.شروعات <= صف_Range.end && صف_Range.end <= m.صفون );

        صفون = صف_Range.size();
        معلومات += قدم*صف_Range.شروعات;
    }

    إذا (کانو_Range == Range::all())
    {
        کانن_ڳڻپ = m.کانن_ڳڻپ;
    }
    وإلا
    {
        CV_Assert( 0 <= کانو_Range.شروعات && کانو_Range.شروعات <= کانو_Range.end && کانو_Range.end <= m.کانن_ڳڻپ );

        کانن_ڳڻپ = کانو_Range.size();
        معلومات += کانو_Range.شروعات*elem_سائيز();
    }

    إذا (refcount)
        CV_XADD(refcount, 1);

    إذا (صفون <= 0 || کانن_ڳڻپ <= 0)
        صفون = کانن_ڳڻپ = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ثابت GpuMat& m, Rect roi) :
    flags(m.flags), صفون(roi.اوچائي), کانن_ڳڻپ(roi.ويڪر),
    قدم(m.قدم), معلومات(m.معلومات + roi.y*قدم), refcount(m.refcount),
    datastart(m.datastart), dataend(m.dataend),
    allocator(m.allocator)
{
    معلومات += roi.x * elem_سائيز();

    CV_Assert( 0 <= roi.x && 0 <= roi.ويڪر &&
               roi.x + roi.ويڪر <= m.کانن_ڳڻپ &&
               0 <= roi.y && 0 <= roi.اوچائي &&
               roi.y + roi.اوچائي <= m.صفون );

    إذا (refcount)
        CV_XADD(refcount, 1);

    إذا (صفون <= 0 || کانن_ڳڻپ <= 0)
        صفون = کانن_ڳڻپ = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(صحيح new_cn, صحيح new_صفون) ثابت
{
    GpuMat hdr = *هذا;

    صحيح cn = channels();
    إذا (new_cn == 0)
        new_cn = cn;

    صحيح ڪل_ويڪر = کانن_ڳڻپ * cn;

    إذا ((new_cn > ڪل_ويڪر || ڪل_ويڪر % new_cn != 0) && new_صفون == 0)
        new_صفون = صفون * ڪل_ويڪر / new_cn;

    إذا (new_صفون != 0 && new_صفون != صفون)
    {
        صحيح ڪل_سائيز_2 = ڪل_ويڪر * صفون;

        إذا (!isContinuous())
            CV_Error(cv::Error::Bad_قدم, "The matrix is not continuous, thus its number of rows can not be changed");

        إذا ((بنانشان)new_صفون > (بنانشان)ڪل_سائيز_2)
            CV_Error(cv::Error::Sts_ٻاهريون_جو_Range, "Bad new number of rows");

        ڪل_ويڪر = ڪل_سائيز_2 / new_صفون;

        إذا (ڪل_ويڪر * new_صفون != ڪل_سائيز_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.صفون = new_صفون;
        hdr.قدم = ڪل_ويڪر * elemSize1();
    }

    صحيح new_ويڪر = ڪل_ويڪر / new_cn;

    إذا (new_ويڪر * new_cn != ڪل_ويڪر)
        CV_Error(cv::Error::Bad_انگ_Channels, "The total width is not divisible by the new number of channels");

    hdr.کانن_ڳڻپ = new_ويڪر;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn - 1) << CV_CN_سرڪايو);

    أرجع hdr;
}

فراغ cv::cuda::GpuMat::locateROI(سائيز& سڄو_سائيز, نقطو& ofs) ثابت
{
    CV_DbgAssert( قدم > 0 );

    size_t esz = elem_سائيز();
    ptrdiff_t delta1 = معلومات - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    إذا (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    وإلا
    {
        ofs.y = static_cast<صحيح>(delta1 / قدم);
        ofs.x = static_cast<صحيح>((delta1 - قدم * ofs.y) / esz);

        CV_DbgAssert( معلومات == datastart + ofs.y * قدم + ofs.x * esz );
    }

    size_t minstep = (ofs.x + کانن_ڳڻپ) * esz;

    سڄو_سائيز.اوچائي = std::max(static_cast<صحيح>((delta2 - minstep) / قدم + 1), ofs.y + صفون);
    سڄو_سائيز.ويڪر = std::max(static_cast<صحيح>((delta2 - قدم * (سڄو_سائيز.اوچائي - 1)) / esz), ofs.x + کانن_ڳڻپ);
}

GpuMat& cv::cuda::GpuMat::adjustROI(صحيح dtop, صحيح dbottom, صحيح dleft, صحيح dright)
{
    سائيز سڄو_سائيز;
    نقطو ofs;
    locateROI(سڄو_سائيز, ofs);

    size_t esz = elem_سائيز();

    صحيح row1 = std::max(ofs.y - dtop, 0);
    صحيح row2 = std::min(ofs.y + صفون + dbottom, سڄو_سائيز.اوچائي);

    صحيح col1 = std::max(ofs.x - dleft, 0);
    صحيح col2 = std::min(ofs.x + کانن_ڳڻپ + dright, سڄو_سائيز.ويڪر);

    معلومات += (row1 - ofs.y) * قدم + (col1 - ofs.x) * esz;
    صفون = row2 - row1;
    کانن_ڳڻپ = col2 - col1;

    updateContinuityFlag();
    أرجع *هذا;
}

نطاق
{
    سانچو <صنف ObjType>
    فراغ createContinuousImpl(صحيح صفون, صحيح کانن_ڳڻپ, صحيح type, ObjType& obj)
    {
        ثابت صحيح area = صفون * کانن_ڳڻپ;

        إذا (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), صفون);
    }
}

فراغ cv::cuda::createContinuous(صحيح صفون, صحيح کانن_ڳڻپ, صحيح type, OutputArray arr)
{
    بدل (arr.قسم())
    {
    حالة داخلا_Array_2::MAT:
        ::createContinuousImpl(صفون, کانن_ڳڻپ, type, arr.getMatRef());
        اكسر;

    حالة داخلا_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(صفون, کانن_ڳڻپ, type, arr.getGpuMatRef());
        اكسر;

    حالة داخلا_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(صفون, کانن_ڳڻپ, type, arr.getHostMemRef());
        اكسر;

    افتراضي:
        arr.create(صفون, کانن_ڳڻپ, type);
    }
}

نطاق
{
    سانچو <صنف ObjType>
    فراغ ensure_سائيز_Is_Enough_Impl(صحيح صفون, صحيح کانن_ڳڻپ, صحيح type, ObjType& obj)
    {
        إذا (obj.empty() || obj.type() != type || obj.معلومات != obj.datastart)
        {
            obj.create(صفون, کانن_ڳڻپ, type);
        }
        وإلا
        {
            ثابت size_t esz = obj.elem_سائيز();
            ثابت ptrdiff_t delta2 = obj.dataend - obj.datastart;

            ثابت size_t minstep = obj.کانن_ڳڻپ * esz;

            سائيز سڄو_سائيز;
            سڄو_سائيز.اوچائي = std::max(static_cast<صحيح>((delta2 - minstep) / static_cast<size_t>(obj.قدم) + 1), obj.صفون);
            سڄو_سائيز.ويڪر = std::max(static_cast<صحيح>((delta2 - static_cast<size_t>(obj.قدم) * (سڄو_سائيز.اوچائي - 1)) / esz), obj.کانن_ڳڻپ);

            إذا (سڄو_سائيز.اوچائي < صفون || سڄو_سائيز.ويڪر < کانن_ڳڻپ)
            {
                obj.create(صفون, کانن_ڳڻپ, type);
            }
            وإلا
            {
                obj.کانن_ڳڻپ = کانن_ڳڻپ;
                obj.صفون = صفون;
            }
        }
    }
}

فراغ cv::cuda::ensure_سائيز_Is_Enough(صحيح صفون, صحيح کانن_ڳڻپ, صحيح type, OutputArray arr)
{
    بدل (arr.قسم())
    {
    حالة داخلا_Array_2::MAT:
        ::ensure_سائيز_Is_Enough_Impl(صفون, کانن_ڳڻپ, type, arr.getMatRef());
        اكسر;

    حالة داخلا_Array_2::CUDA_GPU_MAT:
        ::ensure_سائيز_Is_Enough_Impl(صفون, کانن_ڳڻپ, type, arr.getGpuMatRef());
        اكسر;

    حالة داخلا_Array_2::CUDA_HOST_MEM:
        ::ensure_سائيز_Is_Enough_Impl(صفون, کانن_ڳڻپ, type, arr.getHostMemRef());
        اكسر;

    افتراضي:
        arr.create(صفون, کانن_ڳڻپ, type);
    }
}

GpuMat cv::cuda::get_داخلا_Mat(داخلا_Array _src, وهڪرو& stream)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(_src);
    CV_اڻ_استعمال_ٿيل(stream);
    throw_no_cuda();
#وإلا
    GpuMat src;
    إذا (_src.قسم() == داخلا_Array_2::CUDA_GPU_MAT)
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
#جيڪڏهن_پورو
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, صحيح صفون, صحيح کانن_ڳڻپ, صحيح type, وهڪرو& stream)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(_dst);
    CV_اڻ_استعمال_ٿيل(صفون);
    CV_اڻ_استعمال_ٿيل(کانن_ڳڻپ);
    CV_اڻ_استعمال_ٿيل(type);
    CV_اڻ_استعمال_ٿيل(stream);
    throw_no_cuda();
#وإلا
    GpuMat dst;
    إذا (_dst.قسم() == داخلا_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(صفون, کانن_ڳڻپ, type);
        dst = _dst.getGpuMat();
    }
    وإلا
    {
        بفر_Pool pool(stream);
        dst = pool.get_بفر(صفون, کانن_ڳڻپ, type);
    }
    أرجع dst;
#جيڪڏهن_پورو
}

فراغ cv::cuda::syncOutput(ثابت GpuMat& dst, OutputArray _dst, وهڪرو& stream)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(dst);
    CV_اڻ_استعمال_ٿيل(_dst);
    CV_اڻ_استعمال_ٿيل(stream);
    throw_no_cuda();
#وإلا
    إذا (_dst.قسم() != داخلا_Array_2::CUDA_GPU_MAT)
    {
        إذا (stream)
            dst.download(_dst, stream);
        وإلا
            dst.download(_dst);
    }
#جيڪڏهن_پورو
}

#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    أرجع 0;
}

فراغ cv::cuda::GpuMat::سيٽ_Default_Allocator(Allocator* allocator)
{
    CV_اڻ_استعمال_ٿيل(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    أرجع 0;
}

فراغ cv::cuda::GpuMat::create(صحيح صفون_3, صحيح کانن_ڳڻپ_3, صحيح _type)
{
    CV_اڻ_استعمال_ٿيل(صفون_3);
    CV_اڻ_استعمال_ٿيل(کانن_ڳڻپ_3);
    CV_اڻ_استعمال_ٿيل(_type);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::release()
{
    إذا( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->حرر(هذا);
    dataend = datastart = معلومات = 0;
    قدم = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_نقاب;
}

فراغ cv::cuda::GpuMat::fit(صحيح صفون_3, صحيح کانن_ڳڻپ_3, صحيح _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t old_ڪل_بائيٽون = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_سائيز(_type);
    size_t new_ڪل_بائيٽون = (size_t)صفون_3 * (size_t)کانن_ڳڻپ_3 * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    إذا (new_ڪل_بائيٽون > 0 && (!isContinuous() || new_ڪل_بائيٽون > old_ڪل_بائيٽون || معلومات != datastart))
    {
        create(صفون_3, کانن_ڳڻپ_3, _type);
        أرجع;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_نقاب) | CV_MAKETYPE(CV_MAT_اونهائي(_type), CV_MAT_CN(_type));
    صفون = صفون_3;
    کانن_ڳڻپ = کانن_ڳڻپ_3;
    قدم = (size_t)کانن_ڳڻپ * esz;
}

فراغ cv::cuda::GpuMat::fit(سائيز _sz, صحيح _type)
{
    fit(_sz.اوچائي, _sz.ويڪر, _type);
}

فراغ cv::cuda::GpuMat::upload(داخلا_Array arr)
{
    CV_اڻ_استعمال_ٿيل(arr);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::upload(داخلا_Array arr, وهڪرو& وهڪرو_2)
{
    CV_اڻ_استعمال_ٿيل(arr);
    CV_اڻ_استعمال_ٿيل(وهڪرو_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::download(OutputArray _dst) ثابت
{
    CV_اڻ_استعمال_ٿيل(_dst);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::download(OutputArray _dst, وهڪرو& وهڪرو_2) ثابت
{
    CV_اڻ_استعمال_ٿيل(_dst);
    CV_اڻ_استعمال_ٿيل(وهڪرو_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::copy_منزل(OutputArray _dst) ثابت
{
    CV_اڻ_استعمال_ٿيل(_dst);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::copy_منزل(OutputArray _dst, وهڪرو& وهڪرو_2) ثابت
{
    CV_اڻ_استعمال_ٿيل(_dst);
    CV_اڻ_استعمال_ٿيل(وهڪرو_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::copy_منزل(OutputArray _dst, داخلا_Array نقاب_2, وهڪرو& وهڪرو_2) ثابت
{
    CV_اڻ_استعمال_ٿيل(_dst);
    CV_اڻ_استعمال_ٿيل(نقاب_2);
    CV_اڻ_استعمال_ٿيل(وهڪرو_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::سيٽ_منزل(Scalar s, وهڪرو& وهڪرو_2)
{
    CV_اڻ_استعمال_ٿيل(s);
    CV_اڻ_استعمال_ٿيل(وهڪرو_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::سيٽ_منزل(Scalar s, داخلا_Array نقاب_2, وهڪرو& وهڪرو_2)
{
    CV_اڻ_استعمال_ٿيل(s);
    CV_اڻ_استعمال_ٿيل(نقاب_2);
    CV_اڻ_استعمال_ٿيل(وهڪرو_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::convert_منزل(OutputArray _dst, صحيح rtype, وهڪرو& وهڪرو_2) ثابت
{
    CV_اڻ_استعمال_ٿيل(_dst);
    CV_اڻ_استعمال_ٿيل(rtype);
    CV_اڻ_استعمال_ٿيل(وهڪرو_2);
    throw_no_cuda();
}

فراغ cv::cuda::GpuMat::convert_منزل(OutputArray _dst, صحيح rtype, مزدوج alpha, مزدوج beta, وهڪرو& وهڪرو_2) ثابت
{
    CV_اڻ_استعمال_ٿيل(_dst);
    CV_اڻ_استعمال_ٿيل(rtype);
    CV_اڻ_استعمال_ٿيل(alpha);
    CV_اڻ_استعمال_ٿيل(beta);
    CV_اڻ_استعمال_ٿيل(وهڪرو_2);
    throw_no_cuda();
}

#جيڪڏهن_پورو
