// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"
#समावेश "stat.hpp"

#समावेश "has_non_zero.simd.hpp"
#समावेश "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

अटल HasNonZeroFunc getHasNonZeroFunc(पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#अगर_घोषित HAVE_OPENCL
अटल bool ocl_hasNonZero( निवेश_Array _src, bool & res )
{
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    अगर (depth == CV_64F && !doubleSupport)
        वापस false;

    पूर्णांक dbsize = ocl::Device::getDefault().maxComputeUnits();
    माप_प्रकार wgs = ocl::Device::getDefault().max_Work_Group_आकार();

    पूर्णांक wgs2_aligned = 1;
    जबतक (wgs2_aligned < (पूर्णांक)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (पूर्णांक)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    अगर (k.empty())
        वापस false;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::Read_Only_No_आकार(src), src.cols, (पूर्णांक)src.योग(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    माप_प्रकार globalsize = dbsize * wgs;
    अगर (k.run(1, &globalsize, &wgs, true))
        वापस res = (saturate_cast<पूर्णांक>(cv::योग_2(db.getMat(ACCESS_READ))[0])>0), true;
    वापस false;
}
#पूर्ण_अगर

bool hasNonZero(निवेश_Array _src)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    bool res = false;

#अगर_घोषित HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#पूर्ण_अगर

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.depth());
    CV_Assert( func != 0 );

    अगर (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        अगर (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.योग());
        अन्यथा
            क्रम(पूर्णांक row = 0, rows_गिनती = src.rows ; !res && (row<rows_गिनती) ; ++row)
                res |= func(src.ptr<uchar>(row), src.cols);
    }
    अन्यथा//if (src.dims != 2)
    {
        स्थिर Mat* arrays[] = {&src, nullptr};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        क्रम(माप_प्रकार p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            स्थिर Mat& plane = itNAry.planes[0];
            अगर (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.योग());
            अन्यथा
              क्रम(पूर्णांक row = 0, rows_गिनती = plane.rows ; !res && (row<rows_गिनती) ; ++row)
                  res |= func(plane.ptr<uchar>(row), plane.cols);
        }
    }

    वापस res;
}

} // namespace
