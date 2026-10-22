// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "stat.hpp"

#अन्तर्भाव "has_non_zero.simd.hpp"
#अन्तर्भाव "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv {

स्थिर HasNonZeroFunc getHasNonZeroFunc(पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
स्थिर तार्किक ocl_hasNonZero( आदान_Array _src, तार्किक & res )
{
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    तार्किक doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    जर (depth == CV_64F && !doubleSupport)
        परत असत्य;

    पूर्णांक dbsize = ocl::Device::getDefault().maxComputeUnits();
    माप_प्रकार wgs = ocl::Device::getDefault().maxWorkGroupSize();

    पूर्णांक wgs2_aligned = 1;
    पर्यंत (wgs2_aligned < (पूर्णांक)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (पूर्णांक)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    जर (k.empty())
        परत असत्य;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (पूर्णांक)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    माप_प्रकार globalsize = dbsize * wgs;
    जर (k.run(1, &globalsize, &wgs, सत्य))
        परत res = (saturate_cast<पूर्णांक>(cv::sum(db.getMat(ACCESS_READ))[0])>0), सत्य;
    परत असत्य;
}
#endif

तार्किक hasNonZero(आदान_Array _src)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    तार्किक res = असत्य;

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#endif

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.depth());
    CV_Assert( func != 0 );

    जर (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        जर (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.total());
        अन्यथा
            साठी(पूर्णांक row = 0, rows_मोजणी = src.rows ; !res && (row<rows_मोजणी) ; ++row)
                res |= func(src.ptr<uchar>(row), src.cols);
    }
    अन्यथा//if (src.dims != 2)
    {
        स्थिरांक Mat* arrays[] = {&src, nullptr};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        साठी(माप_प्रकार p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            स्थिरांक Mat& plane = itNAry.planes[0];
            जर (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.total());
            अन्यथा
              साठी(पूर्णांक row = 0, rows_मोजणी = plane.rows ; !res && (row<rows_मोजणी) ; ++row)
                  res |= func(plane.ptr<uchar>(row), plane.cols);
        }
    }

    परत res;
}

} // namespace
