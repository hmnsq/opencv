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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ಬಳಸಿ ನಾಮಾಕಾಶ cv;
ಬಳಸಿ ನಾಮಾಕಾಶ cv::cuda;

ಶೂನ್ಯ cv::cuda::GpuMat::updateContinuityFlag()
{
    ಪೂರ್ಣಾಂಕ sz[] = { ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ };
    size_t ಹೆಜ್ಜೆಗಳು[] = { ಹೆಜ್ಜೆ, elem_ಗಾತ್ರ() };
    flags = cv::updateContinuityFlag(flags, 2, sz, ಹೆಜ್ಜೆಗಳು);
}

cv::cuda::GpuMat::GpuMat(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_2, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2, ಪೂರ್ಣಾಂಕ type_, ಶೂನ್ಯ* ದತ್ತಾಂಶ_2, size_t ಹೆಜ್ಜೆ_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_ಮುಖವಾಡ)), ಅಡ್ಡಸಾಲುಗಳು(ಅಡ್ಡಸಾಲುಗಳು_2), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2),
    ಹೆಜ್ಜೆ(ಹೆಜ್ಜೆ_2), ದತ್ತಾಂಶ((uchar*)ದತ್ತಾಂಶ_2), refcount(0),
    datastart((uchar*)ದತ್ತಾಂಶ_2), dataend((ಸ್ಥಿರಾಂಕ uchar*)ದತ್ತಾಂಶ_2),
    allocator(defaultAllocator())
{
    size_t minstep = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * elem_ಗಾತ್ರ();

    ಆದರೆ (ಹೆಜ್ಜೆ == Mat::AUTO_ಹೆಜ್ಜೆ)
    {
        ಹೆಜ್ಜೆ = minstep;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (ಅಡ್ಡಸಾಲುಗಳು == 1)
            ಹೆಜ್ಜೆ = minstep;

        CV_DbgAssert( ಹೆಜ್ಜೆ >= minstep );
    }

    dataend += ಹೆಜ್ಜೆ * (ಅಡ್ಡಸಾಲುಗಳು - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ಗಾತ್ರ_2 ಗಾತ್ರ_4, ಪೂರ್ಣಾಂಕ type_, ಶೂನ್ಯ* ದತ್ತಾಂಶ_2, size_t ಹೆಜ್ಜೆ_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_ಮುಖವಾಡ)), ಅಡ್ಡಸಾಲುಗಳು(ಗಾತ್ರ_4.ಎತ್ತರ), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(ಗಾತ್ರ_4.ಅಗಲ),
    ಹೆಜ್ಜೆ(ಹೆಜ್ಜೆ_2), ದತ್ತಾಂಶ((uchar*)ದತ್ತಾಂಶ_2), refcount(0),
    datastart((uchar*)ದತ್ತಾಂಶ_2), dataend((ಸ್ಥಿರಾಂಕ uchar*)ದತ್ತಾಂಶ_2),
    allocator(defaultAllocator())
{
    size_t minstep = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * elem_ಗಾತ್ರ();

    ಆದರೆ (ಹೆಜ್ಜೆ == Mat::AUTO_ಹೆಜ್ಜೆ)
    {
        ಹೆಜ್ಜೆ = minstep;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (ಅಡ್ಡಸಾಲುಗಳು == 1)
            ಹೆಜ್ಜೆ = minstep;

        CV_DbgAssert( ಹೆಜ್ಜೆ >= minstep );
    }

    dataend += ಹೆಜ್ಜೆ * (ಅಡ್ಡಸಾಲುಗಳು - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ಸ್ಥಿರಾಂಕ GpuMat& m, Range ಅಡ್ಡಸಾಲು_Range, Range ಲಂಬಸಾಲು_Range)
{
    flags = m.flags;
    ಹೆಜ್ಜೆ = m.ಹೆಜ್ಜೆ; refcount = m.refcount;
    ದತ್ತಾಂಶ = m.ದತ್ತಾಂಶ; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    ಆದರೆ (ಅಡ್ಡಸಾಲು_Range == Range::all())
    {
        ಅಡ್ಡಸಾಲುಗಳು = m.ಅಡ್ಡಸಾಲುಗಳು;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Assert( 0 <= ಅಡ್ಡಸಾಲು_Range.ಆರಂಭ && ಅಡ್ಡಸಾಲು_Range.ಆರಂಭ <= ಅಡ್ಡಸಾಲು_Range.end && ಅಡ್ಡಸಾಲು_Range.end <= m.ಅಡ್ಡಸಾಲುಗಳು );

        ಅಡ್ಡಸಾಲುಗಳು = ಅಡ್ಡಸಾಲು_Range.size();
        ದತ್ತಾಂಶ += ಹೆಜ್ಜೆ*ಅಡ್ಡಸಾಲು_Range.ಆರಂಭ;
    }

    ಆದರೆ (ಲಂಬಸಾಲು_Range == Range::all())
    {
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Assert( 0 <= ಲಂಬಸಾಲು_Range.ಆರಂಭ && ಲಂಬಸಾಲು_Range.ಆರಂಭ <= ಲಂಬಸಾಲು_Range.end && ಲಂಬಸಾಲು_Range.end <= m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ );

        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = ಲಂಬಸಾಲು_Range.size();
        ದತ್ತಾಂಶ += ಲಂಬಸಾಲು_Range.ಆರಂಭ*elem_ಗಾತ್ರ();
    }

    ಆದರೆ (refcount)
        CV_XADD(refcount, 1);

    ಆದರೆ (ಅಡ್ಡಸಾಲುಗಳು <= 0 || ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ <= 0)
        ಅಡ್ಡಸಾಲುಗಳು = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ಸ್ಥಿರಾಂಕ GpuMat& m, Rect roi) :
    flags(m.flags), ಅಡ್ಡಸಾಲುಗಳು(roi.ಎತ್ತರ), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ(roi.ಅಗಲ),
    ಹೆಜ್ಜೆ(m.ಹೆಜ್ಜೆ), ದತ್ತಾಂಶ(m.ದತ್ತಾಂಶ + roi.y*ಹೆಜ್ಜೆ), refcount(m.refcount),
    datastart(m.datastart), dataend(m.dataend),
    allocator(m.allocator)
{
    ದತ್ತಾಂಶ += roi.x * elem_ಗಾತ್ರ();

    CV_Assert( 0 <= roi.x && 0 <= roi.ಅಗಲ &&
               roi.x + roi.ಅಗಲ <= m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ &&
               0 <= roi.y && 0 <= roi.ಎತ್ತರ &&
               roi.y + roi.ಎತ್ತರ <= m.ಅಡ್ಡಸಾಲುಗಳು );

    ಆದರೆ (refcount)
        CV_XADD(refcount, 1);

    ಆದರೆ (ಅಡ್ಡಸಾಲುಗಳು <= 0 || ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ <= 0)
        ಅಡ್ಡಸಾಲುಗಳು = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(ಪೂರ್ಣಾಂಕ new_cn, ಪೂರ್ಣಾಂಕ new_ಅಡ್ಡಸಾಲುಗಳು) ಸ್ಥಿರಾಂಕ
{
    GpuMat hdr = *ಇದು;

    ಪೂರ್ಣಾಂಕ cn = channels();
    ಆದರೆ (new_cn == 0)
        new_cn = cn;

    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_ಅಗಲ = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn;

    ಆದರೆ ((new_cn > ಒಟ್ಟು_ಅಗಲ || ಒಟ್ಟು_ಅಗಲ % new_cn != 0) && new_ಅಡ್ಡಸಾಲುಗಳು == 0)
        new_ಅಡ್ಡಸಾಲುಗಳು = ಅಡ್ಡಸಾಲುಗಳು * ಒಟ್ಟು_ಅಗಲ / new_cn;

    ಆದರೆ (new_ಅಡ್ಡಸಾಲುಗಳು != 0 && new_ಅಡ್ಡಸಾಲುಗಳು != ಅಡ್ಡಸಾಲುಗಳು)
    {
        ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_ಗಾತ್ರ_2 = ಒಟ್ಟು_ಅಗಲ * ಅಡ್ಡಸಾಲುಗಳು;

        ಆದರೆ (!isContinuous())
            CV_Error(cv::Error::Bad_ಹೆಜ್ಜೆ, "The matrix is not continuous, thus its number of rows can not be changed");

        ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ)new_ಅಡ್ಡಸಾಲುಗಳು > (ಚಿಹ್ನೆರಹಿತ)ಒಟ್ಟು_ಗಾತ್ರ_2)
            CV_Error(cv::Error::Sts_ಹೊರಹೋಗು_ದ_Range, "Bad new number of rows");

        ಒಟ್ಟು_ಅಗಲ = ಒಟ್ಟು_ಗಾತ್ರ_2 / new_ಅಡ್ಡಸಾಲುಗಳು;

        ಆದರೆ (ಒಟ್ಟು_ಅಗಲ * new_ಅಡ್ಡಸಾಲುಗಳು != ಒಟ್ಟು_ಗಾತ್ರ_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.ಅಡ್ಡಸಾಲುಗಳು = new_ಅಡ್ಡಸಾಲುಗಳು;
        hdr.ಹೆಜ್ಜೆ = ಒಟ್ಟು_ಅಗಲ * elemSize1();
    }

    ಪೂರ್ಣಾಂಕ new_ಅಗಲ = ಒಟ್ಟು_ಅಗಲ / new_cn;

    ಆದರೆ (new_ಅಗಲ * new_cn != ಒಟ್ಟು_ಅಗಲ)
        CV_Error(cv::Error::Bad_ಅಂಕೆ_Channels, "The total width is not divisible by the new number of channels");

    hdr.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = new_ಅಗಲ;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ಮುಖವಾಡ) | ((new_cn - 1) << CV_CN_ಸರಿಸು);

    ಹಿಂತಿರುಗಿ hdr;
}

ಶೂನ್ಯ cv::cuda::GpuMat::locateROI(ಗಾತ್ರ_2& ಸಂಪೂರ್ಣ_ಗಾತ್ರ, ಬಿಂದು& ofs) ಸ್ಥಿರಾಂಕ
{
    CV_DbgAssert( ಹೆಜ್ಜೆ > 0 );

    size_t esz = elem_ಗಾತ್ರ();
    ptrdiff_t delta1 = ದತ್ತಾಂಶ - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    ಆದರೆ (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ofs.y = static_cast<ಪೂರ್ಣಾಂಕ>(delta1 / ಹೆಜ್ಜೆ);
        ofs.x = static_cast<ಪೂರ್ಣಾಂಕ>((delta1 - ಹೆಜ್ಜೆ * ofs.y) / esz);

        CV_DbgAssert( ದತ್ತಾಂಶ == datastart + ofs.y * ಹೆಜ್ಜೆ + ofs.x * esz );
    }

    size_t minstep = (ofs.x + ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ) * esz;

    ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ = std::max(static_cast<ಪೂರ್ಣಾಂಕ>((delta2 - minstep) / ಹೆಜ್ಜೆ + 1), ofs.y + ಅಡ್ಡಸಾಲುಗಳು);
    ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ = std::max(static_cast<ಪೂರ್ಣಾಂಕ>((delta2 - ಹೆಜ್ಜೆ * (ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ - 1)) / esz), ofs.x + ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
}

GpuMat& cv::cuda::GpuMat::adjustROI(ಪೂರ್ಣಾಂಕ dtop, ಪೂರ್ಣಾಂಕ dbottom, ಪೂರ್ಣಾಂಕ dleft, ಪೂರ್ಣಾಂಕ dright)
{
    ಗಾತ್ರ_2 ಸಂಪೂರ್ಣ_ಗಾತ್ರ;
    ಬಿಂದು ofs;
    locateROI(ಸಂಪೂರ್ಣ_ಗಾತ್ರ, ofs);

    size_t esz = elem_ಗಾತ್ರ();

    ಪೂರ್ಣಾಂಕ row1 = std::max(ofs.y - dtop, 0);
    ಪೂರ್ಣಾಂಕ row2 = std::min(ofs.y + ಅಡ್ಡಸಾಲುಗಳು + dbottom, ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ);

    ಪೂರ್ಣಾಂಕ col1 = std::max(ofs.x - dleft, 0);
    ಪೂರ್ಣಾಂಕ col2 = std::min(ofs.x + ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + dright, ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ);

    ದತ್ತಾಂಶ += (row1 - ofs.y) * ಹೆಜ್ಜೆ + (col1 - ofs.x) * esz;
    ಅಡ್ಡಸಾಲುಗಳು = row2 - row1;
    ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = col2 - col1;

    updateContinuityFlag();
    ಹಿಂತಿರುಗಿ *ಇದು;
}

ನಾಮಾಕಾಶ
{
    ಮಾದರಿ <ವರ್ಗ ObjType>
    ಶೂನ್ಯ createContinuousImpl(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ type, ObjType& obj)
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ area = ಅಡ್ಡಸಾಲುಗಳು * ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;

        ಆದರೆ (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), ಅಡ್ಡಸಾಲುಗಳು);
    }
}

ಶೂನ್ಯ cv::cuda::createContinuous(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ type, OutputArray arr)
{
    ಆಯ್ಕೆ (arr.ಬಗೆ())
    {
    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::MAT:
        ::createContinuousImpl(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type, arr.getMatRef());
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type, arr.getGpuMatRef());
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type, arr.getHostMemRef());
        ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
        arr.create(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type);
    }
}

ನಾಮಾಕಾಶ
{
    ಮಾದರಿ <ವರ್ಗ ObjType>
    ಶೂನ್ಯ ensure_ಗಾತ್ರ_Is_Enough_Impl(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ type, ObjType& obj)
    {
        ಆದರೆ (obj.empty() || obj.type() != type || obj.ದತ್ತಾಂಶ != obj.datastart)
        {
            obj.create(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಸ್ಥಿರಾಂಕ size_t esz = obj.elem_ಗಾತ್ರ();
            ಸ್ಥಿರಾಂಕ ptrdiff_t delta2 = obj.dataend - obj.datastart;

            ಸ್ಥಿರಾಂಕ size_t minstep = obj.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * esz;

            ಗಾತ್ರ_2 ಸಂಪೂರ್ಣ_ಗಾತ್ರ;
            ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ = std::max(static_cast<ಪೂರ್ಣಾಂಕ>((delta2 - minstep) / static_cast<size_t>(obj.ಹೆಜ್ಜೆ) + 1), obj.ಅಡ್ಡಸಾಲುಗಳು);
            ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ = std::max(static_cast<ಪೂರ್ಣಾಂಕ>((delta2 - static_cast<size_t>(obj.ಹೆಜ್ಜೆ) * (ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ - 1)) / esz), obj.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);

            ಆದರೆ (ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಎತ್ತರ < ಅಡ್ಡಸಾಲುಗಳು || ಸಂಪೂರ್ಣ_ಗಾತ್ರ.ಅಗಲ < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)
            {
                obj.create(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                obj.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
                obj.ಅಡ್ಡಸಾಲುಗಳು = ಅಡ್ಡಸಾಲುಗಳು;
            }
        }
    }
}

ಶೂನ್ಯ cv::cuda::ensure_ಗಾತ್ರ_Is_Enough(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ type, OutputArray arr)
{
    ಆಯ್ಕೆ (arr.ಬಗೆ())
    {
    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::MAT:
        ::ensure_ಗಾತ್ರ_Is_Enough_Impl(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type, arr.getMatRef());
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT:
        ::ensure_ಗಾತ್ರ_Is_Enough_Impl(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type, arr.getGpuMatRef());
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::CUDA_HOST_MEM:
        ::ensure_ಗಾತ್ರ_Is_Enough_Impl(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type, arr.getHostMemRef());
        ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
        arr.create(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type);
    }
}

GpuMat cv::cuda::get_ಒಳಹರಿವು_Mat(ಒಳಹರಿವು_Array _src, ಹರಿವು& stream)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(_src);
    CV_ಬಳಸದ(stream);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    GpuMat src;
    ಆದರೆ (_src.ಬಗೆ() == ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT)
    {
        src = _src.getGpuMat();
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!_src.empty())
    {
        ಬಫರ್_Pool pool(stream);
        src = pool.get_ಬಫರ್(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    ಹಿಂತಿರುಗಿ src;
#ಆದರೆ_ಅಂತ್ಯ
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ type, ಹರಿವು& stream)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(_dst);
    CV_ಬಳಸದ(ಅಡ್ಡಸಾಲುಗಳು);
    CV_ಬಳಸದ(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
    CV_ಬಳಸದ(type);
    CV_ಬಳಸದ(stream);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    GpuMat dst;
    ಆದರೆ (_dst.ಬಗೆ() == ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type);
        dst = _dst.getGpuMat();
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಬಫರ್_Pool pool(stream);
        dst = pool.get_ಬಫರ್(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type);
    }
    ಹಿಂತಿರುಗಿ dst;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::cuda::syncOutput(ಸ್ಥಿರಾಂಕ GpuMat& dst, OutputArray _dst, ಹರಿವು& stream)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(dst);
    CV_ಬಳಸದ(_dst);
    CV_ಬಳಸದ(stream);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ (_dst.ಬಗೆ() != ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT)
    {
        ಆದರೆ (stream)
            dst.download(_dst, stream);
        ಇಲ್ಲದಿದ್ದರೆ
            dst.download(_dst);
    }
#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ cv::cuda::GpuMat::ಗಣ_Default_Allocator(Allocator* allocator)
{
    CV_ಬಳಸದ(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ cv::cuda::GpuMat::create(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_3, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಪೂರ್ಣಾಂಕ _type)
{
    CV_ಬಳಸದ(ಅಡ್ಡಸಾಲುಗಳು_3);
    CV_ಬಳಸದ(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3);
    CV_ಬಳಸದ(_type);
    throw_no_cuda();
}

ಶೂನ್ಯ cv::cuda::GpuMat::release()
{
    ಆದರೆ( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->ಬಿಡುಗಡೆ(ಇದು);
    dataend = datastart = ದತ್ತಾಂಶ = 0;
    ಹೆಜ್ಜೆ = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_ಮುಖವಾಡ;
}

ಶೂನ್ಯ cv::cuda::GpuMat::fit(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_3, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಪೂರ್ಣಾಂಕ _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t old_ಒಟ್ಟು_ಬೈಟುಗಳು = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_ಗಾತ್ರ(_type);
    size_t new_ಒಟ್ಟು_ಬೈಟುಗಳು = (size_t)ಅಡ್ಡಸಾಲುಗಳು_3 * (size_t)ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3 * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    ಆದರೆ (new_ಒಟ್ಟು_ಬೈಟುಗಳು > 0 && (!isContinuous() || new_ಒಟ್ಟು_ಬೈಟುಗಳು > old_ಒಟ್ಟು_ಬೈಟುಗಳು || ದತ್ತಾಂಶ != datastart))
    {
        create(ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, _type);
        ಹಿಂತಿರುಗಿ;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_ಮುಖವಾಡ) | CV_MAKETYPE(CV_MAT_ಆಳ(_type), CV_MAT_CN(_type));
    ಅಡ್ಡಸಾಲುಗಳು = ಅಡ್ಡಸಾಲುಗಳು_3;
    ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3;
    ಹೆಜ್ಜೆ = (size_t)ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * esz;
}

ಶೂನ್ಯ cv::cuda::GpuMat::fit(ಗಾತ್ರ_2 _sz, ಪೂರ್ಣಾಂಕ _type)
{
    fit(_sz.ಎತ್ತರ, _sz.ಅಗಲ, _type);
}

ಶೂನ್ಯ cv::cuda::GpuMat::upload(ಒಳಹರಿವು_Array arr)
{
    CV_ಬಳಸದ(arr);
    throw_no_cuda();
}

ಶೂನ್ಯ cv::cuda::GpuMat::upload(ಒಳಹರಿವು_Array arr, ಹರಿವು& ಹರಿವು_2)
{
    CV_ಬಳಸದ(arr);
    CV_ಬಳಸದ(ಹರಿವು_2);
    throw_no_cuda();
}

ಶೂನ್ಯ cv::cuda::GpuMat::download(OutputArray _dst) ಸ್ಥಿರಾಂಕ
{
    CV_ಬಳಸದ(_dst);
    throw_no_cuda();
}

ಶೂನ್ಯ cv::cuda::GpuMat::download(OutputArray _dst, ಹರಿವು& ಹರಿವು_2) ಸ್ಥಿರಾಂಕ
{
    CV_ಬಳಸದ(_dst);
    CV_ಬಳಸದ(ಹರಿವು_2);
    throw_no_cuda();
}

ಶೂನ್ಯ cv::cuda::GpuMat::copy_ಗಮ್ಯ(OutputArray _dst) ಸ್ಥಿರಾಂಕ
{
    CV_ಬಳಸದ(_dst);
    throw_no_cuda();
}

ಶೂನ್ಯ cv::cuda::GpuMat::copy_ಗಮ್ಯ(OutputArray _dst, ಹರಿವು& ಹರಿವು_2) ಸ್ಥಿರಾಂಕ
{
    CV_ಬಳಸದ(_dst);
    CV_ಬಳಸದ(ಹರಿವು_2);
    throw_no_cuda();
}

ಶೂನ್ಯ cv::cuda::GpuMat::copy_ಗಮ್ಯ(OutputArray _dst, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2, ಹರಿವು& ಹರಿವು_2) ಸ್ಥಿರಾಂಕ
{
    CV_ಬಳಸದ(_dst);
    CV_ಬಳಸದ(ಮುಖವಾಡ_2);
    CV_ಬಳಸದ(ಹರಿವು_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::ಗಣ_ಗಮ್ಯ(Scalar s, ಹರಿವು& ಹರಿವು_2)
{
    CV_ಬಳಸದ(s);
    CV_ಬಳಸದ(ಹರಿವು_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::ಗಣ_ಗಮ್ಯ(Scalar s, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2, ಹರಿವು& ಹರಿವು_2)
{
    CV_ಬಳಸದ(s);
    CV_ಬಳಸದ(ಮುಖವಾಡ_2);
    CV_ಬಳಸದ(ಹರಿವು_2);
    throw_no_cuda();
}

ಶೂನ್ಯ cv::cuda::GpuMat::convert_ಗಮ್ಯ(OutputArray _dst, ಪೂರ್ಣಾಂಕ rtype, ಹರಿವು& ಹರಿವು_2) ಸ್ಥಿರಾಂಕ
{
    CV_ಬಳಸದ(_dst);
    CV_ಬಳಸದ(rtype);
    CV_ಬಳಸದ(ಹರಿವು_2);
    throw_no_cuda();
}

ಶೂನ್ಯ cv::cuda::GpuMat::convert_ಗಮ್ಯ(OutputArray _dst, ಪೂರ್ಣಾಂಕ rtype, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta, ಹರಿವು& ಹರಿವು_2) ಸ್ಥಿರಾಂಕ
{
    CV_ಬಳಸದ(_dst);
    CV_ಬಳಸದ(rtype);
    CV_ಬಳಸದ(alpha);
    CV_ಬಳಸದ(beta);
    CV_ಬಳಸದ(ಹರಿವು_2);
    throw_no_cuda();
}

#ಆದರೆ_ಅಂತ್ಯ
