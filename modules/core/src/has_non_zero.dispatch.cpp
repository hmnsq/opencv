// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"
#അന്തര്ഭാവ "stat.hpp"

#അന്തര്ഭാവ "has_non_zero.simd.hpp"
#അന്തര്ഭാവ "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

നാമാകാശ cv {

സ്ഥിതം HasNonZeroFunc getHasNonZeroFunc(പൂർണ്ണസംഖ്യ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
സ്ഥിതം തർക്കം ocl_hasNonZero( നിവേശം_Array _src, തർക്കം & res )
{
    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    തർക്കം doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    എങ്കിൽ (depth == CV_64F && !doubleSupport)
        തിരികെ അസത്യം;

    പൂർണ്ണസംഖ്യ dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    പൂർണ്ണസംഖ്യ wgs2_aligned = 1;
    ആകുമ്പോൾ (wgs2_aligned < (പൂർണ്ണസംഖ്യ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (പൂർണ്ണസംഖ്യ)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (പൂർണ്ണസംഖ്യ)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    എങ്കിൽ (k.run(1, &globalsize, &wgs, സത്യം))
        തിരികെ res = (saturate_cast<പൂർണ്ണസംഖ്യ>(cv::sum(db.getMat(ACCESS_READ))[0])>0), സത്യം;
    തിരികെ അസത്യം;
}
#endif

തർക്കം hasNonZero(നിവേശം_Array _src)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    തർക്കം res = അസത്യം;

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#endif

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.depth());
    CV_Assert( func != 0 );

    എങ്കിൽ (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        എങ്കിൽ (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.total());
        അല്ലെങ്കിൽ
            വേണ്ടി(പൂർണ്ണസംഖ്യ row = 0, rows_എണ്ണം = src.rows ; !res && (row<rows_എണ്ണം) ; ++row)
                res |= func(src.ptr<uchar>(row), src.cols);
    }
    അല്ലെങ്കിൽ//if (src.dims != 2)
    {
        സ്ഥിരം Mat* arrays[] = {&src, nullptr};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        വേണ്ടി(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            സ്ഥിരം Mat& plane = itNAry.planes[0];
            എങ്കിൽ (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.total());
            അല്ലെങ്കിൽ
              വേണ്ടി(പൂർണ്ണസംഖ്യ row = 0, rows_എണ്ണം = plane.rows ; !res && (row<rows_എണ്ണം) ; ++row)
                  res |= func(plane.ptr<uchar>(row), plane.cols);
        }
    }

    തിരികെ res;
}

} // namespace
