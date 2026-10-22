// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "stat.hpp"

#अन्तर्भाव "has_non_zero.simd.hpp"
#अन्तर्भाव "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv {

स्थिर HasNonZeroFunc getHasNonZeroFunc(पूर्णाङ्क गभीरता)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (गभीरता),
        CV_CPU_DISPATCH_MODES_ALL);
}

#यदि_परिभाषितम् HAVE_OPENCL
स्थिर तार्किक ocl_hasNonZero( निवेशः_Array _src, तार्किक & res )
{
    पूर्णाङ्क type = _src.type(), गभीरता = CV_MAT_गभीरता(type), kercn = ocl::predict_Optimal_सदिशम्_विस्तारः(_src);
    तार्किक doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    यदि (गभीरता == CV_64F && !doubleSupport)
        प्रत्यागम असत्य;

    पूर्णाङ्क dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_गणः_आकार();

    पूर्णाङ्क wgs2_aligned = 1;
    यावत् (wgs2_aligned < (पूर्णाङ्क)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_गन्तव्यम्_Str(CV_MAKE_TYPE(गभीरता, kercn)),
                         ocl::type_गन्तव्यम्_Str(गभीरता), (पूर्णाङ्क)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(src), src.स्तम्भसङ्ख्या, (पूर्णाङ्क)src.योग(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    यदि (k.सततम्(1, &globalsize, &wgs, सत्य))
        प्रत्यागम res = (saturate_cast<पूर्णाङ्क>(cv::योग_2(db.getMat(ACCESS_READ))[0])>0), सत्य;
    प्रत्यागम असत्य;
}
#यदि_अन्तः

तार्किक hasNonZero(निवेशः_Array _src)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    तार्किक res = असत्य;

#यदि_परिभाषितम् HAVE_OPENCL
    CV_OCL_सततम्_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#यदि_अन्तः

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.गभीरता());
    CV_Assert( func != 0 );

    यदि (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        यदि (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.योग());
        अन्यथा
            प्रति(पूर्णाङ्क row = 0, पङ्क्तिकाः_गणनम् = src.पङ्क्तिकाः ; !res && (row<पङ्क्तिकाः_गणनम्) ; ++row)
                res |= func(src.ptr<uchar>(row), src.स्तम्भसङ्ख्या);
    }
    अन्यथा//if (src.dims != 2)
    {
        ध्रुव Mat* arrays[] = {&src, शून्यसूचकः};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        प्रति(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            ध्रुव Mat& plane = itNAry.planes[0];
            यदि (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.योग());
            अन्यथा
              प्रति(पूर्णाङ्क row = 0, पङ्क्तिकाः_गणनम् = plane.पङ्क्तिकाः ; !res && (row<पङ्क्तिकाः_गणनम्) ; ++row)
                  res |= func(plane.ptr<uchar>(row), plane.स्तम्भसङ्ख्या);
        }
    }

    प्रत्यागम res;
}

} // namespace
