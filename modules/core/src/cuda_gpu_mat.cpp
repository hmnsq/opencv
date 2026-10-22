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

उपयुज्य नामाकाश cv;
उपयुज्य नामाकाश cv::cuda;

शून्य cv::cuda::GpuMat::updateContinuityFlag()
{
    पूर्णाङ्क sz[] = { पङ्क्तिकाः, स्तम्भसङ्ख्या };
    size_t चरणाः[] = { चरणः, elem_आकार() };
    flags = cv::updateContinuityFlag(flags, 2, sz, चरणाः);
}

cv::cuda::GpuMat::GpuMat(पूर्णाङ्क पङ्क्तिकाः_2, पूर्णाङ्क स्तम्भसङ्ख्या_2, पूर्णाङ्क type_, शून्य* दत्त_2, size_t चरणः_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_आवरणम्)), पङ्क्तिकाः(पङ्क्तिकाः_2), स्तम्भसङ्ख्या(स्तम्भसङ्ख्या_2),
    चरणः(चरणः_2), दत्त((uchar*)दत्त_2), refcount(0),
    datastart((uchar*)दत्त_2), dataend((ध्रुव uchar*)दत्त_2),
    allocator(defaultAllocator())
{
    size_t minstep = स्तम्भसङ्ख्या * elem_आकार();

    यदि (चरणः == Mat::AUTO_चरणः)
    {
        चरणः = minstep;
    }
    अन्यथा
    {
        यदि (पङ्क्तिकाः == 1)
            चरणः = minstep;

        CV_DbgAssert( चरणः >= minstep );
    }

    dataend += चरणः * (पङ्क्तिकाः - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(आकार आकार_3, पूर्णाङ्क type_, शून्य* दत्त_2, size_t चरणः_2) :
    flags(Mat::MAGIC_VAL + (type_ & Mat::TYPE_आवरणम्)), पङ्क्तिकाः(आकार_3.height), स्तम्भसङ्ख्या(आकार_3.विस्तारः),
    चरणः(चरणः_2), दत्त((uchar*)दत्त_2), refcount(0),
    datastart((uchar*)दत्त_2), dataend((ध्रुव uchar*)दत्त_2),
    allocator(defaultAllocator())
{
    size_t minstep = स्तम्भसङ्ख्या * elem_आकार();

    यदि (चरणः == Mat::AUTO_चरणः)
    {
        चरणः = minstep;
    }
    अन्यथा
    {
        यदि (पङ्क्तिकाः == 1)
            चरणः = minstep;

        CV_DbgAssert( चरणः >= minstep );
    }

    dataend += चरणः * (पङ्क्तिकाः - 1) + minstep;
    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ध्रुव GpuMat& m, Range rowRange_, Range colRange_)
{
    flags = m.flags;
    चरणः = m.चरणः; refcount = m.refcount;
    दत्त = m.दत्त; datastart = m.datastart; dataend = m.dataend;
    allocator = m.allocator;

    यदि (rowRange_ == Range::all())
    {
        पङ्क्तिकाः = m.पङ्क्तिकाः;
    }
    अन्यथा
    {
        CV_Assert( 0 <= rowRange_.start && rowRange_.start <= rowRange_.end && rowRange_.end <= m.पङ्क्तिकाः );

        पङ्क्तिकाः = rowRange_.size();
        दत्त += चरणः*rowRange_.start;
    }

    यदि (colRange_ == Range::all())
    {
        स्तम्भसङ्ख्या = m.स्तम्भसङ्ख्या;
    }
    अन्यथा
    {
        CV_Assert( 0 <= colRange_.start && colRange_.start <= colRange_.end && colRange_.end <= m.स्तम्भसङ्ख्या );

        स्तम्भसङ्ख्या = colRange_.size();
        दत्त += colRange_.start*elem_आकार();
    }

    यदि (refcount)
        CV_XADD(refcount, 1);

    यदि (पङ्क्तिकाः <= 0 || स्तम्भसङ्ख्या <= 0)
        पङ्क्तिकाः = स्तम्भसङ्ख्या = 0;

    updateContinuityFlag();
}

cv::cuda::GpuMat::GpuMat(ध्रुव GpuMat& m, Rect roi) :
    flags(m.flags), पङ्क्तिकाः(roi.height), स्तम्भसङ्ख्या(roi.विस्तारः),
    चरणः(m.चरणः), दत्त(m.दत्त + roi.y*चरणः), refcount(m.refcount),
    datastart(m.datastart), dataend(m.dataend),
    allocator(m.allocator)
{
    दत्त += roi.x * elem_आकार();

    CV_Assert( 0 <= roi.x && 0 <= roi.विस्तारः &&
               roi.x + roi.विस्तारः <= m.स्तम्भसङ्ख्या &&
               0 <= roi.y && 0 <= roi.height &&
               roi.y + roi.height <= m.पङ्क्तिकाः );

    यदि (refcount)
        CV_XADD(refcount, 1);

    यदि (पङ्क्तिकाः <= 0 || स्तम्भसङ्ख्या <= 0)
        पङ्क्तिकाः = स्तम्भसङ्ख्या = 0;
    updateContinuityFlag();
}

GpuMat cv::cuda::GpuMat::reshape(पूर्णाङ्क new_cn, पूर्णाङ्क new_पङ्क्तिकाः) ध्रुव
{
    GpuMat hdr = *एतत्;

    पूर्णाङ्क cn = channels();
    यदि (new_cn == 0)
        new_cn = cn;

    पूर्णाङ्क योग_विस्तारः = स्तम्भसङ्ख्या * cn;

    यदि ((new_cn > योग_विस्तारः || योग_विस्तारः % new_cn != 0) && new_पङ्क्तिकाः == 0)
        new_पङ्क्तिकाः = पङ्क्तिकाः * योग_विस्तारः / new_cn;

    यदि (new_पङ्क्तिकाः != 0 && new_पङ्क्तिकाः != पङ्क्तिकाः)
    {
        पूर्णाङ्क योग_आकार_2 = योग_विस्तारः * पङ्क्तिकाः;

        यदि (!isContinuous())
            CV_Error(cv::Error::Bad_चरणः, "The matrix is not continuous, thus its number of rows can not be changed");

        यदि ((अचिह्नित)new_पङ्क्तिकाः > (अचिह्नित)योग_आकार_2)
            CV_Error(cv::Error::Sts_बहिर्गमनम्_सम्बन्धि_Range, "Bad new number of rows");

        योग_विस्तारः = योग_आकार_2 / new_पङ्क्तिकाः;

        यदि (योग_विस्तारः * new_पङ्क्तिकाः != योग_आकार_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.पङ्क्तिकाः = new_पङ्क्तिकाः;
        hdr.चरणः = योग_विस्तारः * elemSize1();
    }

    पूर्णाङ्क new_विस्तारः = योग_विस्तारः / new_cn;

    यदि (new_विस्तारः * new_cn != योग_विस्तारः)
        CV_Error(cv::Error::Bad_अङ्कः_Channels, "The total width is not divisible by the new number of channels");

    hdr.स्तम्भसङ्ख्या = new_विस्तारः;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_आवरणम्) | ((new_cn - 1) << CV_CN_सरणम्);

    प्रत्यागम hdr;
}

शून्य cv::cuda::GpuMat::locateROI(आकार& सम्पूर्णम्_आकार, Point& ofs) ध्रुव
{
    CV_DbgAssert( चरणः > 0 );

    size_t esz = elem_आकार();
    ptrdiff_t delta1 = दत्त - datastart;
    ptrdiff_t delta2 = dataend - datastart;

    यदि (delta1 == 0)
    {
        ofs.x = ofs.y = 0;
    }
    अन्यथा
    {
        ofs.y = static_cast<पूर्णाङ्क>(delta1 / चरणः);
        ofs.x = static_cast<पूर्णाङ्क>((delta1 - चरणः * ofs.y) / esz);

        CV_DbgAssert( दत्त == datastart + ofs.y * चरणः + ofs.x * esz );
    }

    size_t minstep = (ofs.x + स्तम्भसङ्ख्या) * esz;

    सम्पूर्णम्_आकार.height = std::max(static_cast<पूर्णाङ्क>((delta2 - minstep) / चरणः + 1), ofs.y + पङ्क्तिकाः);
    सम्पूर्णम्_आकार.विस्तारः = std::max(static_cast<पूर्णाङ्क>((delta2 - चरणः * (सम्पूर्णम्_आकार.height - 1)) / esz), ofs.x + स्तम्भसङ्ख्या);
}

GpuMat& cv::cuda::GpuMat::adjustROI(पूर्णाङ्क dtop, पूर्णाङ्क dbottom, पूर्णाङ्क dleft, पूर्णाङ्क dright)
{
    आकार सम्पूर्णम्_आकार;
    Point ofs;
    locateROI(सम्पूर्णम्_आकार, ofs);

    size_t esz = elem_आकार();

    पूर्णाङ्क row1 = std::max(ofs.y - dtop, 0);
    पूर्णाङ्क row2 = std::min(ofs.y + पङ्क्तिकाः + dbottom, सम्पूर्णम्_आकार.height);

    पूर्णाङ्क col1 = std::max(ofs.x - dleft, 0);
    पूर्णाङ्क col2 = std::min(ofs.x + स्तम्भसङ्ख्या + dright, सम्पूर्णम्_आकार.विस्तारः);

    दत्त += (row1 - ofs.y) * चरणः + (col1 - ofs.x) * esz;
    पङ्क्तिकाः = row2 - row1;
    स्तम्भसङ्ख्या = col2 - col1;

    updateContinuityFlag();
    प्रत्यागम *एतत्;
}

नामाकाश
{
    प्रतिरूपम् <वर्ग ObjType>
    शून्य createContinuousImpl(पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क type, ObjType& obj)
    {
        ध्रुव पूर्णाङ्क area = पङ्क्तिकाः * स्तम्भसङ्ख्या;

        यदि (obj.empty() || obj.type() != type || !obj.isContinuous() || obj.size().area() != area)
            obj.create(1, area, type);

        obj = obj.reshape(obj.channels(), पङ्क्तिकाः);
    }
}

शून्य cv::cuda::createContinuous(पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क type, OutputArray arr)
{
    विकल्प (arr.विधा())
    {
    पक्ष निवेशः_Array_2::MAT:
        ::createContinuousImpl(पङ्क्तिकाः, स्तम्भसङ्ख्या, type, arr.getMatRef());
        भङ्ग;

    पक्ष निवेशः_Array_2::CUDA_GPU_MAT:
        ::createContinuousImpl(पङ्क्तिकाः, स्तम्भसङ्ख्या, type, arr.getGpuMatRef());
        भङ्ग;

    पक्ष निवेशः_Array_2::CUDA_HOST_MEM:
        ::createContinuousImpl(पङ्क्तिकाः, स्तम्भसङ्ख्या, type, arr.getHostMemRef());
        भङ्ग;

    स्वभाव:
        arr.create(पङ्क्तिकाः, स्तम्भसङ्ख्या, type);
    }
}

नामाकाश
{
    प्रतिरूपम् <वर्ग ObjType>
    शून्य ensure_आकार_Is_Enough_Impl(पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क type, ObjType& obj)
    {
        यदि (obj.empty() || obj.type() != type || obj.दत्त != obj.datastart)
        {
            obj.create(पङ्क्तिकाः, स्तम्भसङ्ख्या, type);
        }
        अन्यथा
        {
            ध्रुव size_t esz = obj.elem_आकार();
            ध्रुव ptrdiff_t delta2 = obj.dataend - obj.datastart;

            ध्रुव size_t minstep = obj.स्तम्भसङ्ख्या * esz;

            आकार सम्पूर्णम्_आकार;
            सम्पूर्णम्_आकार.height = std::max(static_cast<पूर्णाङ्क>((delta2 - minstep) / static_cast<size_t>(obj.चरणः) + 1), obj.पङ्क्तिकाः);
            सम्पूर्णम्_आकार.विस्तारः = std::max(static_cast<पूर्णाङ्क>((delta2 - static_cast<size_t>(obj.चरणः) * (सम्पूर्णम्_आकार.height - 1)) / esz), obj.स्तम्भसङ्ख्या);

            यदि (सम्पूर्णम्_आकार.height < पङ्क्तिकाः || सम्पूर्णम्_आकार.विस्तारः < स्तम्भसङ्ख्या)
            {
                obj.create(पङ्क्तिकाः, स्तम्भसङ्ख्या, type);
            }
            अन्यथा
            {
                obj.स्तम्भसङ्ख्या = स्तम्भसङ्ख्या;
                obj.पङ्क्तिकाः = पङ्क्तिकाः;
            }
        }
    }
}

शून्य cv::cuda::ensure_आकार_Is_Enough(पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क type, OutputArray arr)
{
    विकल्प (arr.विधा())
    {
    पक्ष निवेशः_Array_2::MAT:
        ::ensure_आकार_Is_Enough_Impl(पङ्क्तिकाः, स्तम्भसङ्ख्या, type, arr.getMatRef());
        भङ्ग;

    पक्ष निवेशः_Array_2::CUDA_GPU_MAT:
        ::ensure_आकार_Is_Enough_Impl(पङ्क्तिकाः, स्तम्भसङ्ख्या, type, arr.getGpuMatRef());
        भङ्ग;

    पक्ष निवेशः_Array_2::CUDA_HOST_MEM:
        ::ensure_आकार_Is_Enough_Impl(पङ्क्तिकाः, स्तम्भसङ्ख्या, type, arr.getHostMemRef());
        भङ्ग;

    स्वभाव:
        arr.create(पङ्क्तिकाः, स्तम्भसङ्ख्या, type);
    }
}

GpuMat cv::cuda::get_निवेशः_Mat(निवेशः_Array _src, प्रवाहः& stream)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(_src);
    CV_अप्रयुक्तम्(stream);
    throw_no_cuda();
#अन्यथा
    GpuMat src;
    यदि (_src.विधा() == निवेशः_Array_2::CUDA_GPU_MAT)
    {
        src = _src.getGpuMat();
    }
    अन्यथा यदि (!_src.empty())
    {
        बफर_Pool pool(stream);
        src = pool.get_बफर(_src.size(), _src.type());
        src.upload(_src, stream);
    }
    प्रत्यागम src;
#यदि_अन्तः
}

GpuMat cv::cuda::getOutputMat(OutputArray _dst, पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क type, प्रवाहः& stream)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(_dst);
    CV_अप्रयुक्तम्(पङ्क्तिकाः);
    CV_अप्रयुक्तम्(स्तम्भसङ्ख्या);
    CV_अप्रयुक्तम्(type);
    CV_अप्रयुक्तम्(stream);
    throw_no_cuda();
#अन्यथा
    GpuMat dst;
    यदि (_dst.विधा() == निवेशः_Array_2::CUDA_GPU_MAT)
    {
        _dst.create(पङ्क्तिकाः, स्तम्भसङ्ख्या, type);
        dst = _dst.getGpuMat();
    }
    अन्यथा
    {
        बफर_Pool pool(stream);
        dst = pool.get_बफर(पङ्क्तिकाः, स्तम्भसङ्ख्या, type);
    }
    प्रत्यागम dst;
#यदि_अन्तः
}

शून्य cv::cuda::syncOutput(ध्रुव GpuMat& dst, OutputArray _dst, प्रवाहः& stream)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(dst);
    CV_अप्रयुक्तम्(_dst);
    CV_अप्रयुक्तम्(stream);
    throw_no_cuda();
#अन्यथा
    यदि (_dst.विधा() != निवेशः_Array_2::CUDA_GPU_MAT)
    {
        यदि (stream)
            dst.download(_dst, stream);
        अन्यथा
            dst.download(_dst);
    }
#यदि_अन्तः
}

#यदि_अपरिभाषितम् HAVE_CUDA

GpuMat::Allocator* cv::cuda::GpuMat::defaultAllocator()
{
    प्रत्यागम 0;
}

शून्य cv::cuda::GpuMat::समुच्चयः_Default_Allocator(Allocator* allocator)
{
    CV_अप्रयुक्तम्(allocator);
    throw_no_cuda();
}

GpuMat::Allocator* cv::cuda::GpuMat::getStdAllocator()
{
    प्रत्यागम 0;
}

शून्य cv::cuda::GpuMat::create(पूर्णाङ्क पङ्क्तिकाः_3, पूर्णाङ्क स्तम्भसङ्ख्या_3, पूर्णाङ्क _type)
{
    CV_अप्रयुक्तम्(पङ्क्तिकाः_3);
    CV_अप्रयुक्तम्(स्तम्भसङ्ख्या_3);
    CV_अप्रयुक्तम्(_type);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::release()
{
    यदि( refcount && CV_XADD(refcount, -1) == 1 )
        defaultAllocator()->मुक्त(एतत्);
    dataend = datastart = दत्त = 0;
    चरणः = 0;
    refcount = 0;
    flags &= ~Mat::MAGIC_आवरणम्;
}

शून्य cv::cuda::GpuMat::fit(पूर्णाङ्क पङ्क्तिकाः_3, पूर्णाङ्क स्तम्भसङ्ख्या_3, पूर्णाङ्क _type)
{
    // Emulate Mat::fit semantics for GPU: avoid reallocation when possible
    size_t old_योग_अष्टकानि = (datastart && dataend) ? (size_t)(dataend - datastart) : 0;
    size_t esz = CV_ELEM_आकार(_type);
    size_t new_योग_अष्टकानि = (size_t)पङ्क्तिकाः_3 * (size_t)स्तम्भसङ्ख्या_3 * esz;

    // Reallocate if:
    // - need non-contiguous layout (GPU pitch) or ROI, or
    // - requested bytes exceed current capacity, or
    // - data pointer is not at the start (ROI)
    यदि (new_योग_अष्टकानि > 0 && (!isContinuous() || new_योग_अष्टकानि > old_योग_अष्टकानि || दत्त != datastart))
    {
        create(पङ्क्तिकाः_3, स्तम्भसङ्ख्या_3, _type);
        प्रत्यागम;
    }

    // Reuse existing allocation, just adjust header/type
    flags = (flags & ~Mat::TYPE_आवरणम्) | CV_MAKETYPE(CV_MAT_गभीरता(_type), CV_MAT_CN(_type));
    पङ्क्तिकाः = पङ्क्तिकाः_3;
    स्तम्भसङ्ख्या = स्तम्भसङ्ख्या_3;
    चरणः = (size_t)स्तम्भसङ्ख्या * esz;
}

शून्य cv::cuda::GpuMat::fit(आकार _sz, पूर्णाङ्क _type)
{
    fit(_sz.height, _sz.विस्तारः, _type);
}

शून्य cv::cuda::GpuMat::upload(निवेशः_Array arr)
{
    CV_अप्रयुक्तम्(arr);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::upload(निवेशः_Array arr, प्रवाहः& प्रवाहः_2)
{
    CV_अप्रयुक्तम्(arr);
    CV_अप्रयुक्तम्(प्रवाहः_2);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::download(OutputArray _dst) ध्रुव
{
    CV_अप्रयुक्तम्(_dst);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::download(OutputArray _dst, प्रवाहः& प्रवाहः_2) ध्रुव
{
    CV_अप्रयुक्तम्(_dst);
    CV_अप्रयुक्तम्(प्रवाहः_2);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::copy_गन्तव्यम्(OutputArray _dst) ध्रुव
{
    CV_अप्रयुक्तम्(_dst);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::copy_गन्तव्यम्(OutputArray _dst, प्रवाहः& प्रवाहः_2) ध्रुव
{
    CV_अप्रयुक्तम्(_dst);
    CV_अप्रयुक्तम्(प्रवाहः_2);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::copy_गन्तव्यम्(OutputArray _dst, निवेशः_Array आवरणम्_2, प्रवाहः& प्रवाहः_2) ध्रुव
{
    CV_अप्रयुक्तम्(_dst);
    CV_अप्रयुक्तम्(आवरणम्_2);
    CV_अप्रयुक्तम्(प्रवाहः_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::समुच्चयः_गन्तव्यम्(Scalar s, प्रवाहः& प्रवाहः_2)
{
    CV_अप्रयुक्तम्(s);
    CV_अप्रयुक्तम्(प्रवाहः_2);
    throw_no_cuda();
}

GpuMat& cv::cuda::GpuMat::समुच्चयः_गन्तव्यम्(Scalar s, निवेशः_Array आवरणम्_2, प्रवाहः& प्रवाहः_2)
{
    CV_अप्रयुक्तम्(s);
    CV_अप्रयुक्तम्(आवरणम्_2);
    CV_अप्रयुक्तम्(प्रवाहः_2);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::convert_गन्तव्यम्(OutputArray _dst, पूर्णाङ्क rtype, प्रवाहः& प्रवाहः_2) ध्रुव
{
    CV_अप्रयुक्तम्(_dst);
    CV_अप्रयुक्तम्(rtype);
    CV_अप्रयुक्तम्(प्रवाहः_2);
    throw_no_cuda();
}

शून्य cv::cuda::GpuMat::convert_गन्तव्यम्(OutputArray _dst, पूर्णाङ्क rtype, द्विगुण alpha, द्विगुण beta, प्रवाहः& प्रवाहः_2) ध्रुव
{
    CV_अप्रयुक्तम्(_dst);
    CV_अप्रयुक्तम्(rtype);
    CV_अप्रयुक्तम्(alpha);
    CV_अप्रयुक्तम्(beta);
    CV_अप्रयुक्तम्(प्रवाहः_2);
    throw_no_cuda();
}

#यदि_अन्तः
