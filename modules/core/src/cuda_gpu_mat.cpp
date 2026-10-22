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

#అన్తర్భావ "precomp.hpp"

ఉపయోగించి నామాకాశ cv;
ఉపయోగించి నామాకాశ cv::cuda;

శూన్య cv::cuda::GpuMat::updateContinuityFlag()
{
    పూర్ణాంకం sz[] = { అడ్డువరుసలు, నిలువుల_సంఖ్య };
    size_t అడుగులు[] = { అడుగు, elem_కొలత() };
    flags = cv::updateContinuityFlag(flags, 2, sz, అడుగులు);
}

cv::cuda::GpuMat::GpuMat(పూర్ణాంకం అడ్డువరుసలు_2, పూర్ణాంకం నిలువుల_సంఖ్య_2, పూర్ణాంకం type_, శూన్య* దత్తాంశం_2, size_t అడుగు_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_ముసుగు)), అడ్డువరుసలు(అడ్డువరుసలు_2), నిలువుల_సంఖ్య(నిలువుల_సంఖ్య_2),
    అడుగు(అడుగు_2), దత్తాంశం((uchar*)దత్తాంశం_2), refcount(0),
    datastart((uchar*)దత్తాంశం_2), dataend((స్థిరరాశి uchar*)దత్తాంశం_2),
    allocator(defaultAllocator())
{
    size_t minstep = నిలువుల_సంఖ్య * elem_కొలత();

    ఒకవేళ (అడుగు == Mat::AUTO_అడుగు)
    {
        అడుగు = minstep;
    }
    లేకపోతే
    {
        ఒకవేళ (అడ్డువరుసలు == 1)
            అడుగు = minstep;

        CV_DbgAssert( అడుగు >= minstep );
    }

    dataend += అడుగు * (అడ్డువరుసలు - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(కొలత కొలత_3, పూర్ణాంకం type_, శూన్య* దత్తాంశం_2, size_t అడుగు_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_ముసుగు)), అడ్డువరుసలు(కొలత_3.ఎత్తు), నిలువుల_సంఖ్య(కొలత_3.వెడల్పు),
    అడుగు(అడుగు_2), దత్తాంశం((uchar*)దత్తాంశం_2), refcount(0),
    datastart((uchar*)దత్తాంశం_2), dataend((స్థిరరాశి uchar*)దత్తాంశం_2),
    allocator(defaultAllocator())
{
    size_t minstep = నిలువుల_సంఖ్య * elem_కొలత();

    ఒకవేళ (అడుగు == Mat::AUTO_అడుగు)
    {
        అడుగు = minstep;
    }
    లేకపోతే
    {
        ఒకవేళ (అడ్డువరుసలు == 1)
            అడుగు = minstep;

        CV_DbgAssert( అడుగు >= minstep );
    }

    dataend += అడుగు * (అడ్డువరుసలు - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(స్థిరరాశి GpuMat& m, Range అడ్డువరుస_Range, Range నిలువు_Range)
{
    flags = m.flags;
    అడుగు = m.అడుగు; refcount = m.refcount;
    దత్తాంశం = m.దత్తాంశం; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    ఒకవేళ (అడ్డువరుస_Range == Range::all())
    {
        అడ్డువరుసలు = m.అడ్డువరుసలు;
    }
    లేకపోతే
    {
        CV_Assert( 0 <= అడ్డువరుస_Range.ప్రారంభం && అడ్డువరుస_Range.ప్రారంభం <= అడ్డువరుస_Range.end && అడ్డువరుస_Range.end <= m.అడ్డువరుసలు );

        అడ్డువరుసలు = అడ్డువరుస_Range.size();
        దత్తాంశం += అడుగు*అడ్డువరుస_Range.ప్రారంభం;
    }

    ఒకవేళ (నిలువు_Range == Range::all())
    {
        నిలువుల_సంఖ్య = m.నిలువుల_సంఖ్య;
    }
    లేకపోతే
    {
        CV_Assert( 0 <= నిలువు_Range.ప్రారంభం && నిలువు_Range.ప్రారంభం <= నిలువు_Range.end && నిలువు_Range.end <= m.నిలువుల_సంఖ్య );

        నిలువుల_సంఖ్య = నిలువు_Range.size();
        దత్తాంశం += నిలువు_Range.ప్రారంభం*elem_కొలత();
    }

    ఒకవేళ (refcount)
        CV_XADD(refcount, 1);

    ఒకవేళ (అడ్డువరుసలు <= 0 || నిలువుల_సంఖ్య <= 0)
        అడ్డువరుసలు = నిలువుల_సంఖ్య = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(స్థిరరాశి GpuMat& m, Rect roi) :
    flags(m.flags), అడ్డువరుసలు(roi.ఎత్తు), నిలువుల_సంఖ్య(roi.వెడల్పు),
    అడుగు(m.అడుగు), దత్తాంశం(m.దత్తాంశం + roi.y*అడుగు), refcount(m.refcount),
    datastart(m.datastart), dataend(m.dataend),
    allocator(m.allocator)
{
    దత్తాంశం += roi.x * elem_కొలత();

    CV_Assert( 0 <= roi.x && 0 <= roi.వెడల్పు &&
               roi.x + roi.వెడల్పు <= m.నిలువుల_సంఖ్య &&
               0 <= roi.y && 0 <= roi.ఎత్తు &&
               roi.y + roi.ఎత్తు <= m.అడ్డువరుసలు );

    ఒకవేళ (refcount)
        CV_XADD(refcount, 1);

    ఒకవేళ (అడ్డువరుసలు <= 0 || నిలువుల_సంఖ్య <= 0)
        అడ్డువరుసలు = నిలువుల_సంఖ్య = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(పూర్ణాంకం new_cn, పూర్ణాంకం new_అడ్డువరుసలు) స్థిరరాశి
{
    GpuMat hdr = *ఇది;

    పూర్ణాంకం cn = channels();
    ఒకవేళ (new_cn == 0)
        new_cn = cn;

    పూర్ణాంకం మొత్తం_వెడల్పు = నిలువుల_సంఖ్య * cn;

    ఒకవేళ ((new_cn > మొత్తం_వెడల్పు || మొత్తం_వెడల్పు % new_cn != 0) && new_అడ్డువరుసలు == 0)
        new_అడ్డువరుసలు = అడ్డువరుసలు * మొత్తం_వెడల్పు / new_cn;

    ఒకవేళ (new_అడ్డువరుసలు != 0 && new_అడ్డువరుసలు != అడ్డువరుసలు)
    {
        పూర్ణాంకం మొత్తం_కొలత_2 = మొత్తం_వెడల్పు * అడ్డువరుసలు;

        ఒకవేళ (!isContinuous())
            CV_Error(cv::Error::Bad_అడుగు, "The matrix is not continuous, thus its number of rows can not be changed");

        ఒకవేళ ((గుర్తులేని)new_అడ్డువరుసలు > (గుర్తులేని)మొత్తం_కొలత_2)
            CV_Error(cv::Error::Sts_బహిర్గతం_యొక్క_Range, "Bad new number of rows");

        మొత్తం_వెడల్పు = మొత్తం_కొలత_2 / new_అడ్డువరుసలు;

        ఒకవేళ (మొత్తం_వెడల్పు * new_అడ్డువరుసలు != మొత్తం_కొలత_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.అడ్డువరుసలు = new_అడ్డువరుసలు;
        hdr.అడుగు = మొత్తం_వెడల్పు * elemSize1();
    }

    పూర్ణాంకం new_వెడల్పు = మొత్తం_వెడల్పు / new_cn;

    ఒకవేళ (new_వెడల్పు * new_cn != మొత్తం_వెడల్పు)
        CV_Error(cv::Error::Bad_అంకె_Channels, "The total width is not divisible by the new number of channels");

    hdr.నిలువుల_సంఖ్య = new_వెడల్పు;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ముసుగు) | ((new_cn - 1) << CV_CN_జరుపు);

    తిరిగి hdr;
}

శూన్య cv::cuda::GpuMat::locateROI(కొలత& సంపూర్ణ_కొలత, బిందువు& ofs) స్థిరరాశి
{
    CV_DbgAssert( అడుగు > 0 );

    size_t esz = elem_కొలత();
    ptrdiff_t delta1 = దత్తాంశం - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    ఒకవేళ (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    లేకపోతే
    {
        ofs.y = static_cast<పూర్ణాంకం>(delta1 / అడుగు);
        ofs.x = static_cast<పూర్ణాంకం>((delta1 - అడుగు * ofs.y) / esz);

        CV_DbgAssert( దత్తాంశం == datastart + ofs.y * అడుగు + ofs.x * esz );
    }

    size_t minstep = (ofs.x + నిలువుల_సంఖ్య) * esz;

    సంపూర్ణ_కొలత.ఎత్తు = std::max(static_cast<పూర్ణాంకం>((delta2 - minstep) / అడుగు + 1), ofs.y + అడ్డువరుసలు);
    సంపూర్ణ_కొలత.వెడల్పు = std::max(static_cast<పూర్ణాంకం>((delta2 - అడుగు * (సంపూర్ణ_కొలత.ఎత్తు - 1)) / esz), ofs.x + నిలువుల_సంఖ్య);
}

GpuMat& cv::cuda::GpuMat::adjustROI(పూర్ణాంకం dtop, పూర్ణాంకం dbottom, పూర్ణాంకం dleft, పూర్ణాంకం dright)
{
    కొలత సంపూర్ణ_కొలత;
    బిందువు ofs;
    locateROI(సంపూర్ణ_కొలత, ofs);

    size_t esz = elem_కొలత();

    పూర్ణాంకం row1 = std::max(ofs.y - dtop, 0);
    పూర్ణాంకం row2 = std::min(ofs.y + అడ్డువరుసలు + dbottom, సంపూర్ణ_కొలత.ఎత్తు);

    పూర్ణాంకం col1 = std::max(ofs.x - dleft, 0);
    పూర్ణాంకం col2 = std::min(ofs.x + నిలువుల_సంఖ్య + dright, సంపూర్ణ_కొలత.వెడల్పు);

    దత్తాంశం += (row1 - ofs.y) * అడుగు + (col1 - ofs.x) * esz;
    అడ్డువరుసలు = row2 - row1;
    నిలువుల_సంఖ్య = col2 - col1;

    updateContinuityFlag();
    తిరిగి *ఇది;
}

నామాకాశ
{
    మూస <తరగతి ObjType>
    శూన్య createContinuousImpl(పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం type, ObjType& obj)
    {
        స్థిరరాశి పూర్ణాంకం area = అడ్డువరుసలు * నిలువుల_సంఖ్య;

        ఒకవేళ (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), అడ్డువరుసలు);
    }
}

శూన్య cv::cuda::createContinuous(పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం type, OutputArray arr)
{
    ఎంపిక (arr.రకం())
    {
    స్థితి ఆదానం_Array_2::MAT:
        ::createContinuousImpl(అడ్డువరుసలు, నిలువుల_సంఖ్య, type, arr.getMatRef());
        ఆపు;

    స్థితి ఆదానం_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(అడ్డువరుసలు, నిలువుల_సంఖ్య, type, arr.getGpuMatRef());
        ఆపు;

    స్థితి ఆదానం_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(అడ్డువరుసలు, నిలువుల_సంఖ్య, type, arr.getHostMemRef());
        ఆపు;

    అప్రమేయ:
        arr.create(అడ్డువరుసలు, నిలువుల_సంఖ్య, type);
    }
}

నామాకాశ
{
    మూస <తరగతి ObjType>
    శూన్య ensure_కొలత_Is_Enough_Impl(పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం type, ObjType& obj)
    {
        ఒకవేళ (obj.empty() || obj.type() != type || obj.దత్తాంశం != obj.datastart)
        {
            obj.create(అడ్డువరుసలు, నిలువుల_సంఖ్య, type);
        }
        లేకపోతే
        {
            స్థిరరాశి size_t esz = obj.elem_కొలత();
            స్థిరరాశి ptrdiff_t delta2 = obj.dataend - obj.datastart;

            స్థిరరాశి size_t minstep = obj.నిలువుల_సంఖ్య * esz;

            కొలత సంపూర్ణ_కొలత;
            సంపూర్ణ_కొలత.ఎత్తు = std::max(static_cast<పూర్ణాంకం>((delta2 - minstep) / static_cast<size_t>(obj.అడుగు) + 1), obj.అడ్డువరుసలు);
            సంపూర్ణ_కొలత.వెడల్పు = std::max(static_cast<పూర్ణాంకం>((delta2 - static_cast<size_t>(obj.అడుగు) * (సంపూర్ణ_కొలత.ఎత్తు - 1)) / esz), obj.నిలువుల_సంఖ్య);

            ఒకవేళ (సంపూర్ణ_కొలత.ఎత్తు < అడ్డువరుసలు || సంపూర్ణ_కొలత.వెడల్పు < నిలువుల_సంఖ్య)
            {
                obj.create(అడ్డువరుసలు, నిలువుల_సంఖ్య, type);
            }
            లేకపోతే
            {
                obj.నిలువుల_సంఖ్య = నిలువుల_సంఖ్య;
                obj.అడ్డువరుసలు = అడ్డువరుసలు;
            }
        }
    }
}

శూన్య cv::cuda::ensure_కొలత_Is_Enough(పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం type, OutputArray arr)
{
    ఎంపిక (arr.రకం())
    {
    స్థితి ఆదానం_Array_2::MAT:
        ::ensure_కొలత_Is_Enough_Impl(అడ్డువరుసలు, నిలువుల_సంఖ్య, type, arr.getMatRef());
        ఆపు;

    స్థితి ఆదానం_Array_2::CUDA_GPU_MAT:
        ::ensure_కొలత_Is_Enough_Impl(అడ్డువరుసలు, నిలువుల_సంఖ్య, type, arr.getGpuMatRef());
        ఆపు;

    స్థితి ఆదానం_Array_2::CUDA_HOST_MEM:
        ::ensure_కొలత_Is_Enough_Impl(అడ్డువరుసలు, నిలువుల_సంఖ్య, type, arr.getHostMemRef());
        ఆపు;

    అప్రమేయ:
        arr.create(అడ్డువరుసలు, నిలువుల_సంఖ్య, type);
    }
}

GpuMat cv::cuda::get_ఆదానం_Mat(ఆదానం_Array _src, ప్రవాహం& stream)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(_src);
    CV_వాడని(stream);
    throw_no_cuda();
#లేకపోతే
    GpuMat src;
    ఒకవేళ (_src.రకం() == ఆదానం_Array_2::CUDA_GPU_MAT)
    {
        src = _src.getGpuMat();
    }
    లేకపోతే ఒకవేళ (!_src.empty())
    {
        బఫరు_Pool pool(stream);
        src = pool.get_బఫరు(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    తిరిగి src;
#అయితే_ముగింపు
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం type, ప్రవాహం& stream)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(_dst);
    CV_వాడని(అడ్డువరుసలు);
    CV_వాడని(నిలువుల_సంఖ్య);
    CV_వాడని(type);
    CV_వాడని(stream);
    throw_no_cuda();
#లేకపోతే
    GpuMat dst;
    ఒకవేళ (_dst.రకం() == ఆదానం_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(అడ్డువరుసలు, నిలువుల_సంఖ్య, type);
        dst = _dst.getGpuMat();
    }
    లేకపోతే
    {
        బఫరు_Pool pool(stream);
        dst = pool.get_బఫరు(అడ్డువరుసలు, నిలువుల_సంఖ్య, type);
    }
    తిరిగి dst;
#అయితే_ముగింపు
}

శూన్య cv::cuda::syncOutput(స్థిరరాశి GpuMat& dst, OutputArray _dst, ప్రవాహం& stream)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(dst);
    CV_వాడని(_dst);
    CV_వాడని(stream);
    throw_no_cuda();
#లేకపోతే
    ఒకవేళ (_dst.రకం() != ఆదానం_Array_2::CUDA_GPU_MAT)
    {
        ఒకవేళ (stream)
            dst.download(_dst, stream);
        లేకపోతే
            dst.download(_dst);
    }
#అయితే_ముగింపు
}

#నిర్వచితం_కాకపోతే HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    తిరిగి 0;
}

శూన్య cv::cuda::GpuMat::సమితి_Default_Allocator(Allocator* allocator)
{
    CV_వాడని(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    తిరిగి 0;
}

శూన్య cv::cuda::GpuMat::create(పూర్ణాంకం అడ్డువరుసలు_3, పూర్ణాంకం నిలువుల_సంఖ్య_3, పూర్ణాంకం _type)
{
    CV_వాడని(అడ్డువరుసలు_3);
    CV_వాడని(నిలువుల_సంఖ్య_3);
    CV_వాడని(_type);
    throw_no_cuda();
}

శూన్య cv::cuda::GpuMat::release()
{
    ఒకవేళ( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->విడుదల(ఇది);
    dataend = datastart = దత్తాంశం = 0;
    అడుగు = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_ముసుగు;
}

శూన్య cv::cuda::GpuMat::fit(పూర్ణాంకం అడ్డువరుసలు_3, పూర్ణాంకం నిలువుల_సంఖ్య_3, పూర్ణాంకం _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t old_మొత్తం_బైట్లు = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_కొలత(_type);
    size_t new_మొత్తం_బైట్లు = (size_t)అడ్డువరుసలు_3 * (size_t)నిలువుల_సంఖ్య_3 * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    ఒకవేళ (new_మొత్తం_బైట్లు > 0 && (!isContinuous() || new_మొత్తం_బైట్లు > old_మొత్తం_బైట్లు || దత్తాంశం != datastart))
    {
        create(అడ్డువరుసలు_3, నిలువుల_సంఖ్య_3, _type);
        తిరిగి;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_ముసుగు) | CV_MAKETYPE(CV_MAT_లోతు(_type), CV_MAT_CN(_type));
    అడ్డువరుసలు = అడ్డువరుసలు_3;
    నిలువుల_సంఖ్య = నిలువుల_సంఖ్య_3;
    అడుగు = (size_t)నిలువుల_సంఖ్య * esz;
}

శూన్య cv::cuda::GpuMat::fit(కొలత _sz, పూర్ణాంకం _type)
{
    fit(_sz.ఎత్తు, _sz.వెడల్పు, _type);
}

శూన్య cv::cuda::GpuMat::upload(ఆదానం_Array arr)
{
    CV_వాడని(arr);
    throw_no_cuda();
}

శూన్య cv::cuda::GpuMat::upload(ఆదానం_Array arr, ప్రవాహం& ప్రవాహం_2)
{
    CV_వాడని(arr);
    CV_వాడని(ప్రవాహం_2);
    throw_no_cuda();
}

శూన్య cv::cuda::GpuMat::download(OutputArray _dst) స్థిరరాశి
{
    CV_వాడని(_dst);
    throw_no_cuda();
}

శూన్య cv::cuda::GpuMat::download(OutputArray _dst, ప్రవాహం& ప్రవాహం_2) స్థిరరాశి
{
    CV_వాడని(_dst);
    CV_వాడని(ప్రవాహం_2);
    throw_no_cuda();
}

శూన్య cv::cuda::GpuMat::copy_గమ్యం(OutputArray _dst) స్థిరరాశి
{
    CV_వాడని(_dst);
    throw_no_cuda();
}

శూన్య cv::cuda::GpuMat::copy_గమ్యం(OutputArray _dst, ప్రవాహం& ప్రవాహం_2) స్థిరరాశి
{
    CV_వాడని(_dst);
    CV_వాడని(ప్రవాహం_2);
    throw_no_cuda();
}

శూన్య cv::cuda::GpuMat::copy_గమ్యం(OutputArray _dst, ఆదానం_Array ముసుగు_2, ప్రవాహం& ప్రవాహం_2) స్థిరరాశి
{
    CV_వాడని(_dst);
    CV_వాడని(ముసుగు_2);
    CV_వాడని(ప్రవాహం_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::సమితి_గమ్యం(Scalar s, ప్రవాహం& ప్రవాహం_2)
{
    CV_వాడని(s);
    CV_వాడని(ప్రవాహం_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::సమితి_గమ్యం(Scalar s, ఆదానం_Array ముసుగు_2, ప్రవాహం& ప్రవాహం_2)
{
    CV_వాడని(s);
    CV_వాడని(ముసుగు_2);
    CV_వాడని(ప్రవాహం_2);
    throw_no_cuda();
}

శూన్య cv::cuda::GpuMat::convert_గమ్యం(OutputArray _dst, పూర్ణాంకం rtype, ప్రవాహం& ప్రవాహం_2) స్థిరరాశి
{
    CV_వాడని(_dst);
    CV_వాడని(rtype);
    CV_వాడని(ప్రవాహం_2);
    throw_no_cuda();
}

శూన్య cv::cuda::GpuMat::convert_గమ్యం(OutputArray _dst, పూర్ణాంకం rtype, ద్విగుణ alpha, ద్విగుణ beta, ప్రవాహం& ప్రవాహం_2) స్థిరరాశి
{
    CV_వాడని(_dst);
    CV_వాడని(rtype);
    CV_వాడని(alpha);
    CV_వాడని(beta);
    CV_వాడని(ప్రవాహం_2);
    throw_no_cuda();
}

#అయితే_ముగింపు
