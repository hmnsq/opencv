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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv;
ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::cuda;

ਖਾਲੀ cv::cuda::GpuMat::updateContinuityFlag()
{
    ਪੂਰਨ sz[] = { ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ };
    size_t ਕਦਮਾਂ[] = { ਕਦਮ, elem_ਮਾਪ() };
    flags = cv::updateContinuityFlag(flags, 2, sz, ਕਦਮਾਂ);
}

cv::cuda::GpuMat::GpuMat(ਪੂਰਨ ਪੰਕਤੀਆਂ_2, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_2, ਪੂਰਨ type_, ਖਾਲੀ* ਅੰਕੜੇ_2, size_t ਕਦਮ_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_ਨਕਾਬ)), ਪੰਕਤੀਆਂ(ਪੰਕਤੀਆਂ_2), ਖਾਨੇ_ਗਿਣਤੀ(ਖਾਨੇ_ਗਿਣਤੀ_2),
    ਕਦਮ(ਕਦਮ_2), ਅੰਕੜੇ((uchar*)ਅੰਕੜੇ_2), refcount(0),
    datastart((uchar*)ਅੰਕੜੇ_2), dataend((ਅਚਲ uchar*)ਅੰਕੜੇ_2),
    allocator(defaultAllocator())
{
    size_t minstep = ਖਾਨੇ_ਗਿਣਤੀ * elem_ਮਾਪ();

    ਜੇ (ਕਦਮ == Mat::AUTO_ਕਦਮ)
    {
        ਕਦਮ = minstep;
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (ਪੰਕਤੀਆਂ == 1)
            ਕਦਮ = minstep;

        CV_DbgAssert( ਕਦਮ >= minstep );
    }

    dataend += ਕਦਮ * (ਪੰਕਤੀਆਂ - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ਮਾਪ ਮਾਪ_3, ਪੂਰਨ type_, ਖਾਲੀ* ਅੰਕੜੇ_2, size_t ਕਦਮ_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_ਨਕਾਬ)), ਪੰਕਤੀਆਂ(ਮਾਪ_3.ਉਚਾਈ), ਖਾਨੇ_ਗਿਣਤੀ(ਮਾਪ_3.ਚੌੜਾਈ),
    ਕਦਮ(ਕਦਮ_2), ਅੰਕੜੇ((uchar*)ਅੰਕੜੇ_2), refcount(0),
    datastart((uchar*)ਅੰਕੜੇ_2), dataend((ਅਚਲ uchar*)ਅੰਕੜੇ_2),
    allocator(defaultAllocator())
{
    size_t minstep = ਖਾਨੇ_ਗਿਣਤੀ * elem_ਮਾਪ();

    ਜੇ (ਕਦਮ == Mat::AUTO_ਕਦਮ)
    {
        ਕਦਮ = minstep;
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (ਪੰਕਤੀਆਂ == 1)
            ਕਦਮ = minstep;

        CV_DbgAssert( ਕਦਮ >= minstep );
    }

    dataend += ਕਦਮ * (ਪੰਕਤੀਆਂ - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ਅਚਲ GpuMat& m, Range ਪੰਕਤੀ_Range, Range ਖਾਨਾ_Range)
{
    flags = m.flags;
    ਕਦਮ = m.ਕਦਮ; refcount = m.refcount;
    ਅੰਕੜੇ = m.ਅੰਕੜੇ; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    ਜੇ (ਪੰਕਤੀ_Range == Range::all())
    {
        ਪੰਕਤੀਆਂ = m.ਪੰਕਤੀਆਂ;
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Assert( 0 <= ਪੰਕਤੀ_Range.ਸ਼ੁਰੂ && ਪੰਕਤੀ_Range.ਸ਼ੁਰੂ <= ਪੰਕਤੀ_Range.end && ਪੰਕਤੀ_Range.end <= m.ਪੰਕਤੀਆਂ );

        ਪੰਕਤੀਆਂ = ਪੰਕਤੀ_Range.size();
        ਅੰਕੜੇ += ਕਦਮ*ਪੰਕਤੀ_Range.ਸ਼ੁਰੂ;
    }

    ਜੇ (ਖਾਨਾ_Range == Range::all())
    {
        ਖਾਨੇ_ਗਿਣਤੀ = m.ਖਾਨੇ_ਗਿਣਤੀ;
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Assert( 0 <= ਖਾਨਾ_Range.ਸ਼ੁਰੂ && ਖਾਨਾ_Range.ਸ਼ੁਰੂ <= ਖਾਨਾ_Range.end && ਖਾਨਾ_Range.end <= m.ਖਾਨੇ_ਗਿਣਤੀ );

        ਖਾਨੇ_ਗਿਣਤੀ = ਖਾਨਾ_Range.size();
        ਅੰਕੜੇ += ਖਾਨਾ_Range.ਸ਼ੁਰੂ*elem_ਮਾਪ();
    }

    ਜੇ (refcount)
        CV_XADD(refcount, 1);

    ਜੇ (ਪੰਕਤੀਆਂ <= 0 || ਖਾਨੇ_ਗਿਣਤੀ <= 0)
        ਪੰਕਤੀਆਂ = ਖਾਨੇ_ਗਿਣਤੀ = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ਅਚਲ GpuMat& m, Rect roi) :
    flags(m.flags), ਪੰਕਤੀਆਂ(roi.ਉਚਾਈ), ਖਾਨੇ_ਗਿਣਤੀ(roi.ਚੌੜਾਈ),
    ਕਦਮ(m.ਕਦਮ), ਅੰਕੜੇ(m.ਅੰਕੜੇ + roi.y*ਕਦਮ), refcount(m.refcount),
    datastart(m.datastart), dataend(m.dataend),
    allocator(m.allocator)
{
    ਅੰਕੜੇ += roi.x * elem_ਮਾਪ();

    CV_Assert( 0 <= roi.x && 0 <= roi.ਚੌੜਾਈ &&
               roi.x + roi.ਚੌੜਾਈ <= m.ਖਾਨੇ_ਗਿਣਤੀ &&
               0 <= roi.y && 0 <= roi.ਉਚਾਈ &&
               roi.y + roi.ਉਚਾਈ <= m.ਪੰਕਤੀਆਂ );

    ਜੇ (refcount)
        CV_XADD(refcount, 1);

    ਜੇ (ਪੰਕਤੀਆਂ <= 0 || ਖਾਨੇ_ਗਿਣਤੀ <= 0)
        ਪੰਕਤੀਆਂ = ਖਾਨੇ_ਗਿਣਤੀ = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(ਪੂਰਨ new_cn, ਪੂਰਨ new_ਪੰਕਤੀਆਂ) ਅਚਲ
{
    GpuMat hdr = *ਇਹ;

    ਪੂਰਨ cn = channels();
    ਜੇ (new_cn == 0)
        new_cn = cn;

    ਪੂਰਨ ਕੁੱਲ_ਚੌੜਾਈ = ਖਾਨੇ_ਗਿਣਤੀ * cn;

    ਜੇ ((new_cn > ਕੁੱਲ_ਚੌੜਾਈ || ਕੁੱਲ_ਚੌੜਾਈ % new_cn != 0) && new_ਪੰਕਤੀਆਂ == 0)
        new_ਪੰਕਤੀਆਂ = ਪੰਕਤੀਆਂ * ਕੁੱਲ_ਚੌੜਾਈ / new_cn;

    ਜੇ (new_ਪੰਕਤੀਆਂ != 0 && new_ਪੰਕਤੀਆਂ != ਪੰਕਤੀਆਂ)
    {
        ਪੂਰਨ ਕੁੱਲ_ਮਾਪ_2 = ਕੁੱਲ_ਚੌੜਾਈ * ਪੰਕਤੀਆਂ;

        ਜੇ (!isContinuous())
            CV_Error(cv::Error::Bad_ਕਦਮ, "The matrix is not continuous, thus its number of rows can not be changed");

        ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ)new_ਪੰਕਤੀਆਂ > (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਕੁੱਲ_ਮਾਪ_2)
            CV_Error(cv::Error::Sts_ਬਾਹਰਲਾ_ਦਾ_Range, "Bad new number of rows");

        ਕੁੱਲ_ਚੌੜਾਈ = ਕੁੱਲ_ਮਾਪ_2 / new_ਪੰਕਤੀਆਂ;

        ਜੇ (ਕੁੱਲ_ਚੌੜਾਈ * new_ਪੰਕਤੀਆਂ != ਕੁੱਲ_ਮਾਪ_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.ਪੰਕਤੀਆਂ = new_ਪੰਕਤੀਆਂ;
        hdr.ਕਦਮ = ਕੁੱਲ_ਚੌੜਾਈ * elemSize1();
    }

    ਪੂਰਨ new_ਚੌੜਾਈ = ਕੁੱਲ_ਚੌੜਾਈ / new_cn;

    ਜੇ (new_ਚੌੜਾਈ * new_cn != ਕੁੱਲ_ਚੌੜਾਈ)
        CV_Error(cv::Error::Bad_ਅੰਕ_Channels, "The total width is not divisible by the new number of channels");

    hdr.ਖਾਨੇ_ਗਿਣਤੀ = new_ਚੌੜਾਈ;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ਨਕਾਬ) | ((new_cn - 1) << CV_CN_ਖਿਸਕਾਓ);

    ਵਾਪਸ hdr;
}

ਖਾਲੀ cv::cuda::GpuMat::locateROI(ਮਾਪ& ਸਾਰਾ_ਮਾਪ, ਬਿੰਦੂ& ofs) ਅਚਲ
{
    CV_DbgAssert( ਕਦਮ > 0 );

    size_t esz = elem_ਮਾਪ();
    ptrdiff_t delta1 = ਅੰਕੜੇ - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    ਜੇ (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    ਨਹੀਂਤਾਂ
    {
        ofs.y = static_cast<ਪੂਰਨ>(delta1 / ਕਦਮ);
        ofs.x = static_cast<ਪੂਰਨ>((delta1 - ਕਦਮ * ofs.y) / esz);

        CV_DbgAssert( ਅੰਕੜੇ == datastart + ofs.y * ਕਦਮ + ofs.x * esz );
    }

    size_t minstep = (ofs.x + ਖਾਨੇ_ਗਿਣਤੀ) * esz;

    ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ = std::max(static_cast<ਪੂਰਨ>((delta2 - minstep) / ਕਦਮ + 1), ofs.y + ਪੰਕਤੀਆਂ);
    ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ = std::max(static_cast<ਪੂਰਨ>((delta2 - ਕਦਮ * (ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ - 1)) / esz), ofs.x + ਖਾਨੇ_ਗਿਣਤੀ);
}

GpuMat& cv::cuda::GpuMat::adjustROI(ਪੂਰਨ dtop, ਪੂਰਨ dbottom, ਪੂਰਨ dleft, ਪੂਰਨ dright)
{
    ਮਾਪ ਸਾਰਾ_ਮਾਪ;
    ਬਿੰਦੂ ofs;
    locateROI(ਸਾਰਾ_ਮਾਪ, ofs);

    size_t esz = elem_ਮਾਪ();

    ਪੂਰਨ row1 = std::max(ofs.y - dtop, 0);
    ਪੂਰਨ row2 = std::min(ofs.y + ਪੰਕਤੀਆਂ + dbottom, ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ);

    ਪੂਰਨ col1 = std::max(ofs.x - dleft, 0);
    ਪੂਰਨ col2 = std::min(ofs.x + ਖਾਨੇ_ਗਿਣਤੀ + dright, ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ);

    ਅੰਕੜੇ += (row1 - ofs.y) * ਕਦਮ + (col1 - ofs.x) * esz;
    ਪੰਕਤੀਆਂ = row2 - row1;
    ਖਾਨੇ_ਗਿਣਤੀ = col2 - col1;

    updateContinuityFlag();
    ਵਾਪਸ *ਇਹ;
}

ਨਾਮਾਕਾਸ਼
{
    ਸਾਂਚਾ <ਵਰਗ ObjType>
    ਖਾਲੀ createContinuousImpl(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ type, ObjType& obj)
    {
        ਅਚਲ ਪੂਰਨ area = ਪੰਕਤੀਆਂ * ਖਾਨੇ_ਗਿਣਤੀ;

        ਜੇ (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), ਪੰਕਤੀਆਂ);
    }
}

ਖਾਲੀ cv::cuda::createContinuous(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ type, OutputArray arr)
{
    ਚੋਣ (arr.ਕਿਸਮ())
    {
    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::MAT:
        ::createContinuousImpl(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type, arr.getMatRef());
        ਰੋਕੋ;

    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type, arr.getGpuMatRef());
        ਰੋਕੋ;

    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type, arr.getHostMemRef());
        ਰੋਕੋ;

    ਮੂਲ:
        arr.create(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type);
    }
}

ਨਾਮਾਕਾਸ਼
{
    ਸਾਂਚਾ <ਵਰਗ ObjType>
    ਖਾਲੀ ensure_ਮਾਪ_Is_Enough_Impl(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ type, ObjType& obj)
    {
        ਜੇ (obj.empty() || obj.type() != type || obj.ਅੰਕੜੇ != obj.datastart)
        {
            obj.create(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type);
        }
        ਨਹੀਂਤਾਂ
        {
            ਅਚਲ size_t esz = obj.elem_ਮਾਪ();
            ਅਚਲ ptrdiff_t delta2 = obj.dataend - obj.datastart;

            ਅਚਲ size_t minstep = obj.ਖਾਨੇ_ਗਿਣਤੀ * esz;

            ਮਾਪ ਸਾਰਾ_ਮਾਪ;
            ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ = std::max(static_cast<ਪੂਰਨ>((delta2 - minstep) / static_cast<size_t>(obj.ਕਦਮ) + 1), obj.ਪੰਕਤੀਆਂ);
            ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ = std::max(static_cast<ਪੂਰਨ>((delta2 - static_cast<size_t>(obj.ਕਦਮ) * (ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ - 1)) / esz), obj.ਖਾਨੇ_ਗਿਣਤੀ);

            ਜੇ (ਸਾਰਾ_ਮਾਪ.ਉਚਾਈ < ਪੰਕਤੀਆਂ || ਸਾਰਾ_ਮਾਪ.ਚੌੜਾਈ < ਖਾਨੇ_ਗਿਣਤੀ)
            {
                obj.create(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type);
            }
            ਨਹੀਂਤਾਂ
            {
                obj.ਖਾਨੇ_ਗਿਣਤੀ = ਖਾਨੇ_ਗਿਣਤੀ;
                obj.ਪੰਕਤੀਆਂ = ਪੰਕਤੀਆਂ;
            }
        }
    }
}

ਖਾਲੀ cv::cuda::ensure_ਮਾਪ_Is_Enough(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ type, OutputArray arr)
{
    ਚੋਣ (arr.ਕਿਸਮ())
    {
    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::MAT:
        ::ensure_ਮਾਪ_Is_Enough_Impl(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type, arr.getMatRef());
        ਰੋਕੋ;

    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::CUDA_GPU_MAT:
        ::ensure_ਮਾਪ_Is_Enough_Impl(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type, arr.getGpuMatRef());
        ਰੋਕੋ;

    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::CUDA_HOST_MEM:
        ::ensure_ਮਾਪ_Is_Enough_Impl(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type, arr.getHostMemRef());
        ਰੋਕੋ;

    ਮੂਲ:
        arr.create(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type);
    }
}

GpuMat cv::cuda::get_ਨਿਵੇਸ਼_Mat(ਨਿਵੇਸ਼_Array _src, ਧਾਰਾ& stream)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(_src);
    CV_ਅਣਵਰਤਿਆ(stream);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    GpuMat src;
    ਜੇ (_src.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::CUDA_GPU_MAT)
    {
        src = _src.getGpuMat();
    }
    ਨਹੀਂਤਾਂ ਜੇ (!_src.empty())
    {
        ਬਫਰ_Pool pool(stream);
        src = pool.get_ਬਫਰ(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    ਵਾਪਸ src;
#ਜੇ_ਅੰਤ
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ type, ਧਾਰਾ& stream)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(_dst);
    CV_ਅਣਵਰਤਿਆ(ਪੰਕਤੀਆਂ);
    CV_ਅਣਵਰਤਿਆ(ਖਾਨੇ_ਗਿਣਤੀ);
    CV_ਅਣਵਰਤਿਆ(type);
    CV_ਅਣਵਰਤਿਆ(stream);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    GpuMat dst;
    ਜੇ (_dst.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type);
        dst = _dst.getGpuMat();
    }
    ਨਹੀਂਤਾਂ
    {
        ਬਫਰ_Pool pool(stream);
        dst = pool.get_ਬਫਰ(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type);
    }
    ਵਾਪਸ dst;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::cuda::syncOutput(ਅਚਲ GpuMat& dst, OutputArray _dst, ਧਾਰਾ& stream)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(dst);
    CV_ਅਣਵਰਤਿਆ(_dst);
    CV_ਅਣਵਰਤਿਆ(stream);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਜੇ (_dst.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::CUDA_GPU_MAT)
    {
        ਜੇ (stream)
            dst.download(_dst, stream);
        ਨਹੀਂਤਾਂ
            dst.download(_dst);
    }
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    ਵਾਪਸ 0;
}

ਖਾਲੀ cv::cuda::GpuMat::ਸੈੱਟ_Default_Allocator(Allocator* allocator)
{
    CV_ਅਣਵਰਤਿਆ(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    ਵਾਪਸ 0;
}

ਖਾਲੀ cv::cuda::GpuMat::create(ਪੂਰਨ ਪੰਕਤੀਆਂ_3, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੂਰਨ _type)
{
    CV_ਅਣਵਰਤਿਆ(ਪੰਕਤੀਆਂ_3);
    CV_ਅਣਵਰਤਿਆ(ਖਾਨੇ_ਗਿਣਤੀ_3);
    CV_ਅਣਵਰਤਿਆ(_type);
    throw_no_cuda();
}

ਖਾਲੀ cv::cuda::GpuMat::release()
{
    ਜੇ( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->ਮੁਕਤ(ਇਹ);
    dataend = datastart = ਅੰਕੜੇ = 0;
    ਕਦਮ = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_ਨਕਾਬ;
}

ਖਾਲੀ cv::cuda::GpuMat::fit(ਪੂਰਨ ਪੰਕਤੀਆਂ_3, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੂਰਨ _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t old_ਕੁੱਲ_ਬਾਈਟਾਂ = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_ਮਾਪ(_type);
    size_t new_ਕੁੱਲ_ਬਾਈਟਾਂ = (size_t)ਪੰਕਤੀਆਂ_3 * (size_t)ਖਾਨੇ_ਗਿਣਤੀ_3 * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    ਜੇ (new_ਕੁੱਲ_ਬਾਈਟਾਂ > 0 && (!isContinuous() || new_ਕੁੱਲ_ਬਾਈਟਾਂ > old_ਕੁੱਲ_ਬਾਈਟਾਂ || ਅੰਕੜੇ != datastart))
    {
        create(ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3, _type);
        ਵਾਪਸ;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_ਨਕਾਬ) | CV_MAKETYPE(CV_MAT_ਡੂੰਘਾਈ(_type), CV_MAT_CN(_type));
    ਪੰਕਤੀਆਂ = ਪੰਕਤੀਆਂ_3;
    ਖਾਨੇ_ਗਿਣਤੀ = ਖਾਨੇ_ਗਿਣਤੀ_3;
    ਕਦਮ = (size_t)ਖਾਨੇ_ਗਿਣਤੀ * esz;
}

ਖਾਲੀ cv::cuda::GpuMat::fit(ਮਾਪ _sz, ਪੂਰਨ _type)
{
    fit(_sz.ਉਚਾਈ, _sz.ਚੌੜਾਈ, _type);
}

ਖਾਲੀ cv::cuda::GpuMat::upload(ਨਿਵੇਸ਼_Array arr)
{
    CV_ਅਣਵਰਤਿਆ(arr);
    throw_no_cuda();
}

ਖਾਲੀ cv::cuda::GpuMat::upload(ਨਿਵੇਸ਼_Array arr, ਧਾਰਾ& ਧਾਰਾ_2)
{
    CV_ਅਣਵਰਤਿਆ(arr);
    CV_ਅਣਵਰਤਿਆ(ਧਾਰਾ_2);
    throw_no_cuda();
}

ਖਾਲੀ cv::cuda::GpuMat::download(OutputArray _dst) ਅਚਲ
{
    CV_ਅਣਵਰਤਿਆ(_dst);
    throw_no_cuda();
}

ਖਾਲੀ cv::cuda::GpuMat::download(OutputArray _dst, ਧਾਰਾ& ਧਾਰਾ_2) ਅਚਲ
{
    CV_ਅਣਵਰਤਿਆ(_dst);
    CV_ਅਣਵਰਤਿਆ(ਧਾਰਾ_2);
    throw_no_cuda();
}

ਖਾਲੀ cv::cuda::GpuMat::copy_ਮੰਜ਼ਿਲ(OutputArray _dst) ਅਚਲ
{
    CV_ਅਣਵਰਤਿਆ(_dst);
    throw_no_cuda();
}

ਖਾਲੀ cv::cuda::GpuMat::copy_ਮੰਜ਼ਿਲ(OutputArray _dst, ਧਾਰਾ& ਧਾਰਾ_2) ਅਚਲ
{
    CV_ਅਣਵਰਤਿਆ(_dst);
    CV_ਅਣਵਰਤਿਆ(ਧਾਰਾ_2);
    throw_no_cuda();
}

ਖਾਲੀ cv::cuda::GpuMat::copy_ਮੰਜ਼ਿਲ(OutputArray _dst, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2, ਧਾਰਾ& ਧਾਰਾ_2) ਅਚਲ
{
    CV_ਅਣਵਰਤਿਆ(_dst);
    CV_ਅਣਵਰਤਿਆ(ਨਕਾਬ_2);
    CV_ਅਣਵਰਤਿਆ(ਧਾਰਾ_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::ਸੈੱਟ_ਮੰਜ਼ਿਲ(Scalar s, ਧਾਰਾ& ਧਾਰਾ_2)
{
    CV_ਅਣਵਰਤਿਆ(s);
    CV_ਅਣਵਰਤਿਆ(ਧਾਰਾ_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::ਸੈੱਟ_ਮੰਜ਼ਿਲ(Scalar s, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2, ਧਾਰਾ& ਧਾਰਾ_2)
{
    CV_ਅਣਵਰਤਿਆ(s);
    CV_ਅਣਵਰਤਿਆ(ਨਕਾਬ_2);
    CV_ਅਣਵਰਤਿਆ(ਧਾਰਾ_2);
    throw_no_cuda();
}

ਖਾਲੀ cv::cuda::GpuMat::convert_ਮੰਜ਼ਿਲ(OutputArray _dst, ਪੂਰਨ rtype, ਧਾਰਾ& ਧਾਰਾ_2) ਅਚਲ
{
    CV_ਅਣਵਰਤਿਆ(_dst);
    CV_ਅਣਵਰਤਿਆ(rtype);
    CV_ਅਣਵਰਤਿਆ(ਧਾਰਾ_2);
    throw_no_cuda();
}

ਖਾਲੀ cv::cuda::GpuMat::convert_ਮੰਜ਼ਿਲ(OutputArray _dst, ਪੂਰਨ rtype, ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta, ਧਾਰਾ& ਧਾਰਾ_2) ਅਚਲ
{
    CV_ਅਣਵਰਤਿਆ(_dst);
    CV_ਅਣਵਰਤਿਆ(rtype);
    CV_ਅਣਵਰਤਿਆ(alpha);
    CV_ਅਣਵਰਤਿਆ(beta);
    CV_ਅਣਵਰਤਿਆ(ਧਾਰਾ_2);
    throw_no_cuda();
}

#ਜੇ_ਅੰਤ
