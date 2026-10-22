// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"

#ಅನ್ತರ್ಭಾವ "has_non_zero.simd.hpp"
#ಅನ್ತರ್ಭಾವ "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ HasNonZeroFunc getHasNonZeroFunc(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (ಆಳ),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_hasNonZero( ಒಳಹರಿವು_Array _src, ತಾರ್ಕಿಕ & res )
{
    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type), kercn = ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(_src);
    ತಾರ್ಕಿಕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ಆದರೆ (ಆಳ == CV_64F && !doubleSupport)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಪೂರ್ಣಾಂಕ dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_ಗುಂಪು_ಗಾತ್ರ();

    ಪೂರ್ಣಾಂಕ wgs2_aligned = 1;
    ಆಗ (wgs2_aligned < (ಪೂರ್ಣಾಂಕ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, kercn)),
                         ocl::type_ಗಮ್ಯ_Str(ಆಳ), (ಪೂರ್ಣಾಂಕ)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src), src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    ಆದರೆ (k.ಸತತ(1, &globalsize, &wgs, ನಿಜ))
        ಹಿಂತಿರುಗಿ res = (saturate_cast<ಪೂರ್ಣಾಂಕ>(cv::ಮೊತ್ತ(db.getMat(ACCESS_READ))[0])>0), ನಿಜ;
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}
#ಆದರೆ_ಅಂತ್ಯ

ತಾರ್ಕಿಕ hasNonZero(ಒಳಹರಿವು_Array _src)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    ತಾರ್ಕಿಕ res = ಸುಳ್ಳು;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    CV_OCL_ಸತತ_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#ಆದರೆ_ಅಂತ್ಯ

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.ಆಳ());
    CV_Assert( func != 0 );

    ಆದರೆ (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        ಆದರೆ (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.ಒಟ್ಟು());
        ಇಲ್ಲದಿದ್ದರೆ
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = 0, ಅಡ್ಡಸಾಲುಗಳು_ಎಣಿಕೆ = src.ಅಡ್ಡಸಾಲುಗಳು ; !res && (ಅಡ್ಡಸಾಲು<ಅಡ್ಡಸಾಲುಗಳು_ಎಣಿಕೆ) ; ++ಅಡ್ಡಸಾಲು)
                res |= func(src.ptr<uchar>(ಅಡ್ಡಸಾಲು), src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
    }
    ಇಲ್ಲದಿದ್ದರೆ//if (src.dims != 2)
    {
        ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, ಶೂನ್ಯಸೂಚಕ};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        ಗಾಗಿ(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            ಸ್ಥಿರಾಂಕ Mat& plane = itNAry.planes[0];
            ಆದರೆ (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.ಒಟ್ಟು());
            ಇಲ್ಲದಿದ್ದರೆ
              ಗಾಗಿ(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = 0, ಅಡ್ಡಸಾಲುಗಳು_ಎಣಿಕೆ = plane.ಅಡ್ಡಸಾಲುಗಳು ; !res && (ಅಡ್ಡಸಾಲು<ಅಡ್ಡಸಾಲುಗಳು_ಎಣಿಕೆ) ; ++ಅಡ್ಡಸಾಲು)
                  res |= func(plane.ptr<uchar>(ಅಡ್ಡಸಾಲು), plane.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
        }
    }

    ಹಿಂತಿರುಗಿ res;
}

} // namespace
