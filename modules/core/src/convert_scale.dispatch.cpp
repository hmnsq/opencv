// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"

#समावेश "convert_scale.simd.hpp"
#समावेश "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv
{

अटल द्विआधारी_Func getCvtScaleAbsFunc(पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

द्विआधारी_Func getConvertScaleFunc(पूर्णांक sdepth, पूर्णांक ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#अगर_घोषित HAVE_OPENCL

अटल bool ocl_convertScaleAbs( निवेश_Array _src, OutputArray _dst, द्विगुन alpha, द्विगुन beta )
{
    स्थिर ocl::Device & d = ocl::Device::getDefault();

    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    bool doubleSupport = d.doubleFPConfig() > 0;
    अगर (!doubleSupport && depth == CV_64F)
        वापस false;

    _dst.create_Same_आकार(_src, CV_8UC(cn));
    पूर्णांक kercn = 1;
    अगर (d.isIntel())
    {
        अटल स्थिर पूर्णांक vectorWidths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::checkOptimalVectorWidth( vectorWidths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_VECTOR_MAX);
    }
    अन्यथा
        kercn = ocl::predictOptimalVectorWidthMax(_src, _dst);

    पूर्णांक rowsPerWI = d.isIntel() ? 4 : 1;
    अक्षर cvt[2][50];
    पूर्णांक wdepth = std::max(depth, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], माप(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], माप(cvt[1])),
                         ocl::typeToStr(wdepth), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    अगर (k.empty())
        वापस false;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_आकार(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    अगर (wdepth == CV_32F)
        k.args(srcarg, dstarg, (भग्न)alpha, (भग्न)beta);
    अन्यथा अगर (wdepth == CV_64F)
        k.args(srcarg, dstarg, alpha, beta);

    माप_प्रकार globalsize[2] = { (माप_प्रकार)src.cols * cn / kercn, ((माप_प्रकार)src.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

#पूर्ण_अगर

व्योम convertScaleAbs(निवेश_Array _src, OutputArray _dst, द्विगुन alpha, द्विगुन beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    पूर्णांक cn = src.channels();
    द्विगुन scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    द्विआधारी_Func func = getCvtScaleAbsFunc(src.depth());
    CV_Assert( func != 0 );

    अगर( src.dims <= 2 )
    {
        आकार sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.step, 0, 0, dst.ptr(), dst.step, sz, scale );
    }
    अन्यथा
    {
        स्थिर Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        आकार sz((पूर्णांक)it.size*cn, 1);

        क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
