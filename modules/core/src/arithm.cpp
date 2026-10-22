/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
// Copyright (C) 2014-2015, Itseez Inc., all rights reserved.
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

/* ////////////////////////////////////////////////////////////////////
//
//  Arithmetic and logical operations: +, -, *, /, &, |, ^, ~, abs ...
//
// */

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "arithm_expr.hpp"   // the new element-wise engine (cv::ew)
#అన్తర్భావ "opencl_kernels_core.hpp"

నామాకాశ cv
{

/****************************************************************************************\
*                                   logical operations                                   *
\****************************************************************************************/

గణన { OCL_OP_కూడు=0, OCL_OP_SUB=1, OCL_OP_RSUB=2, OCL_OP_ABSDIFF=3, OCL_OP_MUL=4,
       OCL_OP_MUL_SCALE=5, OCL_OP_DIV_SCALE=6, OCL_OP_RECIP_SCALE=7, OCL_OP_ADDW=8,
       OCL_OP_AND=9, OCL_OP_OR=10, OCL_OP_XOR=11, OCL_OP_NOT=12, OCL_OP_MIN=13, OCL_OP_MAX=14,
       OCL_OP_RDIV_SCALE=15 };

// The unified entry point for every element-wise binary op (empty + UMat/OpenCL + CPU engine). Defined
// lower down (near cv::add); forward-declared here so cv::min/cv::max (above it) can use it too.
స్థిర శూన్య arithm_op(ew::శిఖరం op, ఆదానం_Array src1, ఆదానం_Array src2, OutputArray dst,
                      ఆదానం_Array ముసుగు, పూర్ణాంకం dtype, పూర్ణాంకం oclop, తార్కిక muldiv, స్థిరరాశి Scalar& params = Scalar(1));

// The unary counterpart (bitwise NOT, unary math later). Forward-declared for the same reason.
స్థిర శూన్య unary_op(ew::శిఖరం op, ఆదానం_Array src, OutputArray dst, ఆదానం_Array ముసుగు,
                     పూర్ణాంకం dtype, స్థిరరాశి Scalar& params = Scalar());

#నిర్వచితమైతే HAVE_OPENCL

స్థిర స్థిరరాశి అక్షరం* oclop2str[] = { "OP_ADD", "OP_SUB", "OP_RSUB", "OP_ABSDIFF",
    "OP_MUL", "OP_MUL_SCALE", "OP_DIV_SCALE", "OP_RECIP_SCALE",
    "OP_ADDW", "OP_AND", "OP_OR", "OP_XOR", "OP_NOT", "OP_MIN", "OP_MAX", "OP_RDIV_SCALE", 0 };

// depths with no OpenCL type name (ocl::typeToStr() asserts on them)
స్థిర అంతఃవరుస తార్కిక noOclType(పూర్ణాంకం లోతు)
{
    తిరిగి లోతు == CV_16BF || లోతు == CV_Bool || లోతు == CV_8F_E4M3FN || లోతు == CV_8F_E4M3FNUZ;
}

స్థిర తార్కిక ocl_ద్విమాన_op(ఆదానం_Array _src1, ఆదానం_Array _src2, OutputArray _dst,
                          ఆదానం_Array ముసుగు_2, తార్కిక bitwise, పూర్ణాంకం oclop, తార్కిక haveScalar )
{
    తార్కిక have_ముసుగు = !ముసుగు_2.empty();
    పూర్ణాంకం srctype = _src1.type();
    పూర్ణాంకం srcdepth = CV_MAT_లోతు(srctype);
    పూర్ణాంకం cn = CV_MAT_CN(srctype);

    స్థిరరాశి ocl::Device d = ocl::Device::getDefault();
    తార్కిక doubleSupport = d.doubleFPConfig() > 0;
    ఒకవేళ( oclop < 0 || ((have_ముసుగు || haveScalar) && cn > 4) ||
            (!doubleSupport && srcdepth == CV_64F && !bitwise) ||
            (noOclType(srcdepth) && !bitwise))
        తిరిగి అబద్ధం;

    అక్షరం opts[1024];
    పూర్ణాంకం kercn = have_ముసుగు || haveScalar ? cn : ocl::predict_Optimal_సదిశ_వెడల్పు(_src1, _src2, _dst);
    పూర్ణాంకం scalarcn = kercn == 3 ? 4 : kercn;
    పూర్ణాంకం అడ్డువరుసలు_చొప్పున_WI = d.isIntel() ? 4 : 1;

    స్థిరరాశి పూర్ణాంకం dst_లోతు = srcdepth;
    స్థిరరాశి పూర్ణాంకం dstType = CV_MAKETYPE(dst_లోతు, kercn);
    స్థిరరాశి పూర్ణాంకం dstType1 = CV_MAKETYPE(dst_లోతు, 1);
    స్థిరరాశి పూర్ణాంకం scalarType = CV_MAKETYPE(srcdepth, scalarcn);

    snprintf(opts, పరిమాణం(opts), "-D %s%s -D %s%s -D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workST=%s -D cn=%d -D rowsPerWI=%d",
            have_ముసుగు ? "MASK_" : "", haveScalar ? "UNARY_OP" : "BINARY_OP", oclop2str[oclop],
            doubleSupport ? " -D DOUBLE_SUPPORT" : "",
            bitwise ? ocl::memop_Type_గమ్యం_Str(dstType) : ocl::type_గమ్యం_Str(dstType),
            dst_లోతు,
            bitwise ? ocl::memop_Type_గమ్యం_Str(dstType1) : ocl::type_గమ్యం_Str(dstType1),
            bitwise ? ocl::memop_Type_గమ్యం_Str(scalarType) : ocl::type_గమ్యం_Str(scalarType),
            kercn, అడ్డువరుసలు_చొప్పున_WI);

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), ముసుగు = ముసుగు_2.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_కొలత(src1, cn, kercn);
    ocl::KernelArg dstarg = have_ముసుగు ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_కొలత(ముసుగు, 1);

    ఒకవేళ( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(srctype)*scalarcn;
        ద్విగుణ buf[4] = {0,0,0,0};

        ఒకవేళ( oclop != OCL_OP_NOT )
        {
            Mat src2sc = _src2.getMat();
            convertAndUnrollScalar(src2sc, srctype, (uchar*)buf, 1);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        ఒకవేళ( !have_ముసుగు )
            k.వాదనలు(src1arg, dstarg, scalararg);
        లేకపోతే
            k.వాదనలు(src1arg, maskarg, dstarg, scalararg);
    }
    లేకపోతే
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::Read_Only_No_కొలత(src2, cn, kercn);

        ఒకవేళ( !have_ముసుగు )
            k.వాదనలు(src1arg, src2arg, dstarg);
        లేకపోతే
            k.వాదనలు(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.నిలువుల_సంఖ్య * cn / kercn, ((size_t)src1.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, 0, అబద్ధం);
}

#అయితే_ముగింపు

// OpenCL path for the bitwise ops (and/or/xor/not), lifted out of binary_op: choose the (possibly
// scalar) operand order, size the destination, and dispatch the shared bitwise OpenCL kernel. Returns
// true if OpenCL handled the op, false to fall through to the CPU engine. NOT arrives unary
// (src2 == src1, forced-scalar). The caller gates this on a UMat operand + dims <= 2 (via CV_OCL_RUN).
#నిర్వచితమైతే HAVE_OPENCL
స్థిర తార్కిక bitwise_op_ocl(ఆదానం_Array _src1, ఆదానం_Array _src2, OutputArray _dst,
                           ఆదానం_Array ముసుగు_2, పూర్ణాంకం oclop)
{
    స్థిరరాశి ఆదానం_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    ఆదానం_Array_2::రకం_Flag kind1 = psrc1->రకం(), kind2 = psrc2->రకం();
    పూర్ణాంకం type1 = psrc1->type(), type2 = psrc2->type();
    తార్కిక haveScalar = అబద్ధం;

    ఒకవేళ( oclop == OCL_OP_NOT )
        haveScalar = నిజం;
    లేకపోతే ఒకవేళ( (kind1 == ఆదానం_Array_2::MATX) + (kind2 == ఆదానం_Array_2::MATX) == 1 ||
             !psrc1->same_కొలత(*psrc2) || type1 != type2 )
    {
        ఒకవేళ( checkScalar(*psrc1, type2, kind1, kind2) )
        { std::swap(psrc1, psrc2); std::swap(type1, type2); }        // src1 is the scalar; swap it out
        లేకపోతే ఒకవేళ( !checkScalar(*psrc2, type1, kind2, kind1) )
            తిరిగి అబద్ధం;
        haveScalar = నిజం;
    }
    _dst.create_Same_కొలత(*psrc1, type1);
    తిరిగి ocl_ద్విమాన_op(*psrc1, *psrc2, _dst, ముసుగు_2, నిజం, oclop, haveScalar);
}
#అయితే_ముగింపు  // HAVE_OPENCL

}

// bitwise and/or/xor: OpenCL runs the shared bitwise kernel (via bitwise_op_ocl); the CPU path is the
// element-wise engine (OP_AND/OR/XOR, T x T -> T dispatched by element size). oclop = -1 tells
// arithm_op the OpenCL path was already tried, so it only runs the CPU engine. NOT is unary.
శూన్య cv::bitwise_and(ఆదానం_Array a, ఆదానం_Array b, OutputArray c, ఆదానం_Array ముసుగు)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_నిరంతరం((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, ముసుగు, OCL_OP_AND))
    arithm_op(ew::OP_AND, a, b, c, ముసుగు, -1, /*oclop=*/-1, /*muldiv=*/అబద్ధం);
}

శూన్య cv::bitwise_or(ఆదానం_Array a, ఆదానం_Array b, OutputArray c, ఆదానం_Array ముసుగు)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_నిరంతరం((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, ముసుగు, OCL_OP_OR))
    arithm_op(ew::OP_OR, a, b, c, ముసుగు, -1, /*oclop=*/-1, /*muldiv=*/అబద్ధం);
}

శూన్య cv::bitwise_xor(ఆదానం_Array a, ఆదానం_Array b, OutputArray c, ఆదానం_Array ముసుగు)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_నిరంతరం((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, ముసుగు, OCL_OP_XOR))
    arithm_op(ew::OP_XOR, a, b, c, ముసుగు, -1, /*oclop=*/-1, /*muldiv=*/అబద్ధం);
}

శూన్య cv::bitwise_not(ఆదానం_Array a, OutputArray c, ఆదానం_Array ముసుగు)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_నిరంతరం((a.isUMat() || c.isUMat()) && a.dims() <= 2,
               bitwise_op_ocl(a, a, c, ముసుగు, OCL_OP_NOT))
    unary_op(ew::OP_NOT, a, c, ముసుగు, -1);
}

శూన్య cv::max( ఆదానం_Array src1, ఆదానం_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, అబద్ధం);
}

శూన్య cv::min( ఆదానం_Array src1, ఆదానం_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, అబద్ధం);
}

// The concrete Mat/UMat overloads of min/max exist because C++ overload resolution needs them (a
// bare cv::min(mat, mat, mat) binds Mat& more tightly than InputArray); they must run the SAME engine
// path as the InputArray forms - NOT the legacy binary_op - so mixed types and broadcasting work.
శూన్య cv::max(స్థిరరాశి Mat& src1, స్థిరరాశి Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, అబద్ధం);
}

శూన్య cv::min(స్థిరరాశి Mat& src1, స్థిరరాశి Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, అబద్ధం);
}

శూన్య cv::max(స్థిరరాశి UMat& src1, స్థిరరాశి UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, అబద్ధం);
}

శూన్య cv::min(స్థిరరాశి UMat& src1, స్థిరరాశి UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, అబద్ధం);
}


/****************************************************************************************\
*                                      add/subtract                                      *
\****************************************************************************************/

నామాకాశ cv
{

#నిర్వచితమైతే HAVE_OPENCL

// used by the OpenCL branch only (arithm_op_ocl working-type selection)
స్థిర పూర్ణాంకం actual_Scalar_లోతు(స్థిరరాశి ద్విగుణ* దత్తాంశం, పూర్ణాంకం len)
{
    పూర్ణాంకం i = 0, minval = గరిష్ఠ_పూర్ణాంకం, maxval = INT_MIN;
    కొరకు(; i < len; ++i)
    {
        పూర్ణాంకం ival = cv_విడత(దత్తాంశం[i]);
        ఒకవేళ( ival != దత్తాంశం[i] )
            ఆపు;
        minval = MIN(minval, ival);
        maxval = MAX(maxval, ival);
    }
    తిరిగి i < len ? CV_64F :
        minval >= 0 && maxval <= (పూర్ణాంకం)UCHAR_MAX ? CV_8U :
        minval >= (పూర్ణాంకం)SCHAR_MIN && maxval <= (పూర్ణాంకం)SCHAR_MAX ? CV_8S :
        minval >= 0 && maxval <= (పూర్ణాంకం)USHRT_MAX ? CV_16U :
        minval >= (పూర్ణాంకం)SHRT_MIN && maxval <= (పూర్ణాంకం)SHRT_MAX ? CV_16S :
        CV_32S;
}

స్థిర పూర్ణాంకం coerceTypes(పూర్ణాంకం depth1, పూర్ణాంకం depth2, తార్కిక muldiv)
{
    తిరిగి depth1 == depth2 ? depth1 :
        ((depth1 <= CV_32S) & (depth2 <= CV_32S)) != 0 ?
        (((పూర్ణాంకం)!muldiv & (depth1 <= CV_8S) & (depth2 <= CV_8S)) != 0 ? CV_16S : CV_32S) :
        ((CV_ELEM_SIZE1(depth1) > 4) | (CV_ELEM_SIZE1(depth2) > 4)) != 0 ? CV_64F : CV_32F;
}

స్థిర తార్కిక ocl_arithm_op(ఆదానం_Array _src1, ఆదానం_Array _src2, OutputArray _dst,
                          ఆదానం_Array ముసుగు_2, పూర్ణాంకం wtype,
                          శూన్య* usrdata, పూర్ణాంకం oclop,
                          తార్కిక haveScalar )
{
    స్థిరరాశి ocl::Device d = ocl::Device::getDefault();
    తార్కిక doubleSupport = d.doubleFPConfig() > 0;
    పూర్ణాంకం type1 = _src1.type(), depth1 = CV_MAT_లోతు(type1), cn = CV_MAT_CN(type1);
    తార్కిక have_ముసుగు = !ముసుగు_2.empty();

    ఒకవేళ ( (have_ముసుగు || haveScalar) && cn > 4 )
        తిరిగి అబద్ధం;

#నిర్వచితమైతే __APPLE__
    ఒకవేళ (depth1 == CV_16U && (oclop == OCL_OP_MUL || oclop == OCL_OP_MUL_SCALE))
        తిరిగి అబద్ధం;
#అయితే_ముగింపు

    పూర్ణాంకం dtype = _dst.type(), ddepth = CV_MAT_లోతు(dtype), wdepth = std::max(CV_32S, CV_MAT_లోతు(wtype));
    ఒకవేళ (!doubleSupport)
        wdepth = std::min(wdepth, CV_32F);

    wtype = CV_MAKETYPE(wdepth, cn);
    పూర్ణాంకం type2 = haveScalar ? wtype : _src2.type(), depth2 = CV_MAT_లోతు(type2);
    ఒకవేళ (!doubleSupport && (depth2 == CV_64F || depth1 == CV_64F))
        తిరిగి అబద్ధం;

    ఒకవేళ (noOclType(depth1) || noOclType(depth2) || noOclType(ddepth) || noOclType(wdepth))
        తిరిగి అబద్ధం;

    పూర్ణాంకం kercn = have_ముసుగు || haveScalar ? cn : ocl::predict_Optimal_సదిశ_వెడల్పు(_src1, _src2, _dst);
    పూర్ణాంకం scalarcn = kercn == 3 ? 4 : kercn, అడ్డువరుసలు_చొప్పున_WI = d.isIntel() ? 4 : 1;

    అక్షరం cvtstr[4][50], opts[1024];
    snprintf(opts, పరిమాణం(opts), "-D %s%s -D %s -D srcT1=%s -D srcT1_C1=%s -D srcT2=%s -D srcT2_C1=%s "
            "-D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workT=%s -D workST=%s -D scaleT=%s -D wdepth=%d -D convertToWT1=%s "
            "-D convertToWT2=%s -D convertToDT=%s%s -D cn=%d -D rowsPerWI=%d -D convertFromU=%s",
            (have_ముసుగు ? "MASK_" : ""), (haveScalar ? "UNARY_OP" : "BINARY_OP"),
            oclop2str[oclop], ocl::type_గమ్యం_Str(CV_MAKETYPE(depth1, kercn)),
            ocl::type_గమ్యం_Str(depth1), ocl::type_గమ్యం_Str(CV_MAKETYPE(depth2, kercn)),
            ocl::type_గమ్యం_Str(depth2), ocl::type_గమ్యం_Str(CV_MAKETYPE(ddepth, kercn)), ddepth,
            ocl::type_గమ్యం_Str(ddepth), ocl::type_గమ్యం_Str(CV_MAKETYPE(wdepth, kercn)),
            ocl::type_గమ్యం_Str(CV_MAKETYPE(wdepth, scalarcn)),
            ocl::type_గమ్యం_Str(wdepth), wdepth,
            ocl::convertTypeStr(depth1, wdepth, kercn, cvtstr[0], పరిమాణం(cvtstr[0])),
            ocl::convertTypeStr(depth2, wdepth, kercn, cvtstr[1], పరిమాణం(cvtstr[1])),
            ocl::convertTypeStr(wdepth, ddepth, kercn, cvtstr[2], పరిమాణం(cvtstr[2])),
            doubleSupport ? " -D DOUBLE_SUPPORT" : "", kercn, అడ్డువరుసలు_చొప్పున_WI,
            oclop == OCL_OP_ABSDIFF && wdepth == CV_32S && ddepth == wdepth ?
            ocl::convertTypeStr(CV_8U, ddepth, kercn, cvtstr[3], పరిమాణం(cvtstr[3])) : "noconvert");

    size_t usrdata_esz = CV_ELEM_కొలత(wdepth);
    స్థిరరాశి uchar* usrdata_p = (స్థిరరాశి uchar*)usrdata;
    స్థిరరాశి ద్విగుణ* usrdata_d = (స్థిరరాశి ద్విగుణ*)usrdata;
    దశాంశం usrdata_f[3];
    పూర్ణాంకం i, n = oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE ||
        oclop == OCL_OP_RDIV_SCALE || oclop == OCL_OP_RECIP_SCALE ? 1 : oclop == OCL_OP_ADDW ? 3 : 0;
    ఒకవేళ( usrdata && n > 0 && wdepth == CV_32F )
    {
        కొరకు( i = 0; i < n; i++ )
            usrdata_f[i] = (దశాంశం)usrdata_d[i];
        usrdata_p = (స్థిరరాశి uchar*)usrdata_f;
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), ముసుగు = ముసుగు_2.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_కొలత(src1, cn, kercn);
    ocl::KernelArg dstarg = have_ముసుగు ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_కొలత(ముసుగు, 1);

    ఒకవేళ( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(wtype)*scalarcn;
        ద్విగుణ buf[4]={0,0,0,0};
        Mat src2sc = _src2.getMat();

        ఒకవేళ( !src2sc.empty() )
            convertAndUnrollScalar(src2sc, wtype, (uchar*)buf, 1);
        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        ఒకవేళ( !have_ముసుగు )
        {
            ఒకవేళ(n == 0)
                k.వాదనలు(src1arg, dstarg, scalararg);
            లేకపోతే ఒకవేళ(n == 1)
                k.వాదనలు(src1arg, dstarg, scalararg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            లేకపోతే
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        లేకపోతే
            k.వాదనలు(src1arg, maskarg, dstarg, scalararg);
    }
    లేకపోతే
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::Read_Only_No_కొలత(src2, cn, kercn);

        ఒకవేళ( !have_ముసుగు )
        {
            ఒకవేళ (n == 0)
                k.వాదనలు(src1arg, src2arg, dstarg);
            లేకపోతే ఒకవేళ (n == 1)
                k.వాదనలు(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            లేకపోతే ఒకవేళ (n == 3)
                k.వాదనలు(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz*2, usrdata_esz));
            లేకపోతే
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        లేకపోతే
            k.వాదనలు(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.నిలువుల_సంఖ్య * cn / kercn, ((size_t)src1.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

#అయితే_ముగింపు

// OpenCL branch of the arithmetic ops, extracted from the former arithm_op (whose CPU compute is now
// the element-wise engine). Returns true if an OpenCL kernel handled the op, false to let the caller
// fall through to the CPU engine. min/max use the bitwise-style kernel; every other op the arithm
// kernel with a coerced working type. `usrdata` carries the mul/div/addWeighted scale block, `oclop`
// the kernel id. Only reached for a UMat dst; sizes/types that don't fit the scalar/same-size pattern
// (e.g. a broadcast) decline (return false) so the engine handles them on the CPU.
స్థిర తార్కిక arithm_op_ocl(ఆదానం_Array _src1, ఆదానం_Array _src2, OutputArray _dst,
                          ఆదానం_Array ముసుగు_2, పూర్ణాంకం dtype, పూర్ణాంకం oclop, తార్కిక muldiv, శూన్య* usrdata)
{
#నిర్వచితం_కాకపోతే HAVE_OPENCL
    CV_వాడని(_src1); CV_వాడని(_src2); CV_వాడని(_dst); CV_వాడని(ముసుగు_2);
    CV_వాడని(dtype); CV_వాడని(oclop); CV_వాడని(muldiv); CV_వాడని(usrdata);
    తిరిగి అబద్ధం;
#లేకపోతే
    స్థిరరాశి ఆదానం_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    ఆదానం_Array_2::రకం_Flag kind1 = psrc1->రకం(), kind2 = psrc2->రకం();
    తార్కిక have_ముసుగు = !ముసుగు_2.empty();
    తార్కిక reallocate = అబద్ధం;
    పూర్ణాంకం type1 = psrc1->type(), depth1 = CV_MAT_లోతు(type1), cn = CV_MAT_CN(type1);
    పూర్ణాంకం type2 = psrc2->type(), depth2 = CV_MAT_లోతు(type2), cn2 = CV_MAT_CN(type2);
    పూర్ణాంకం wtype, dims1 = psrc1->dims(), dims2 = psrc2->dims();
    కొలత sz1 = dims1 <= 2 ? psrc1->size() : కొలత();
    కొలత sz2 = dims2 <= 2 ? psrc2->size() : కొలత();

    ఒకవేళ (!(OCL_PERFORMANCE_CHECK(_dst.isUMat()) && dims1 <= 2 && dims2 <= 2))
        తిరిగి అబద్ధం;

    // min/max: the bitwise-style OpenCL kernel (same-type element-wise, no working-type coercion).
    ఒకవేళ (oclop == OCL_OP_MIN || oclop == OCL_OP_MAX)
    {
        తార్కిక haveScalar = అబద్ధం;
        ఒకవేళ ((kind1 == ఆదానం_Array_2::MATX) + (kind2 == ఆదానం_Array_2::MATX) == 1 ||
            !psrc1->same_కొలత(*psrc2) || type1 != type2)
        {
            ఒకవేళ (checkScalar(*psrc1, type2, kind1, kind2))
            { std::swap(psrc1, psrc2); std::swap(type1, type2); }
            లేకపోతే ఒకవేళ (!checkScalar(*psrc2, type1, kind2, kind1))
                తిరిగి అబద్ధం;
            haveScalar = నిజం;
        }
        _dst.create_Same_కొలత(*psrc1, type1);
        తిరిగి ocl_ద్విమాన_op(*psrc1, *psrc2, _dst, ముసుగు_2, అబద్ధం, oclop, haveScalar);
    }

    // reciprocal (scale/src): the OpenCL kernel is unary on src - use src2 for both operands (the CPU
    // engine received a 0-dim `1` numerator as src1, which the recip kernel would ignore). [TODO.VP: tidy]
    ఒకవేళ (oclop == OCL_OP_RECIP_SCALE)
    {
        psrc1 = psrc2; kind1 = kind2; type1 = type2; depth1 = depth2; cn = cn2; dims1 = dims2; sz1 = sz2;
    }

    // add/sub/mul/div/absdiff/addWeighted/recip: the arithm OpenCL kernel with a coerced work type.
    తార్కిక src1Scalar = checkScalar(*psrc1, type2, kind1, kind2);
    తార్కిక src2Scalar = checkScalar(*psrc2, type1, kind2, kind1);
    తార్కిక haveScalar = అబద్ధం;

    ఒకవేళ (dims1 != dims2 || sz1 != sz2 || cn != cn2 ||
        (kind1 == ఆదానం_Array_2::MATX && (sz1 == కొలత(1,4) || sz1 == కొలత(1,1))) ||
        (kind2 == ఆదానం_Array_2::MATX && (sz2 == కొలత(1,4) || sz2 == కొలత(1,1))))
    {
        ఒకవేళ ((type1 == CV_64F && (sz1.ఎత్తు == 1 || sz1.ఎత్తు == 4)) && src1Scalar)
        {
            // src1 is a scalar; swap it with src2
            std::swap(psrc1, psrc2); std::swap(sz1, sz2); std::swap(type1, type2);
            std::swap(depth1, depth2); std::swap(cn, cn2); std::swap(dims1, dims2);
            ఒకవేళ (oclop == OCL_OP_SUB) oclop = OCL_OP_RSUB;
            ఒకవేళ (oclop == OCL_OP_DIV_SCALE) oclop = OCL_OP_RDIV_SCALE;
        }
        లేకపోతే ఒకవేళ (!src2Scalar)
            తిరిగి అబద్ధం;   // array op array with mismatched size/cn: engine broadcasts on the CPU
        haveScalar = నిజం;
        CV_Assert((type2 == CV_64F || type2 == CV_32F) && (sz2.ఎత్తు == 1 || sz2.ఎత్తు == 4));

        ఒకవేళ (!muldiv)
        {
            Mat sc = psrc2->getMat();
            depth2 = actual_Scalar_లోతు(sc.ptr<ద్విగుణ>(), sz2 == కొలత(1, 1) ? cn2 : cn);
            ఒకవేళ (depth2 == CV_64F && CV_ELEM_SIZE1(depth1) < 8)
                depth2 = CV_32F;
        }
        లేకపోతే
            depth2 = CV_64F;
    }

    ఒకవేళ (dtype < 0)
    {
        ఒకవేళ (_dst.fixedType())
            dtype = _dst.type();
        లేకపోతే
        {
            ఒకవేళ (!haveScalar && type1 != type2)
                CV_Error(cv::Error::StsBadArg,
                     "When the input arrays in add/subtract/multiply/divide functions have different types, "
                     "the output array type must be explicitly specified");
            dtype = type1;
        }
    }
    dtype = CV_MAT_లోతు(dtype);

    ఒకవేళ (depth1 == depth2 && dtype == depth1)
        wtype = dtype;
    లేకపోతే ఒకవేళ (!muldiv)
    {
        wtype = coerceTypes(depth1, depth2, అబద్ధం);
        wtype = coerceTypes(wtype, dtype, అబద్ధం);
        ఒకవేళ (dtype < CV_32F && (depth1 < CV_32F || depth2 < CV_32F))
            wtype = CV_32S;
    }
    లేకపోతే
    {
        wtype = coerceTypes(depth1, depth2, నిజం);
        wtype = coerceTypes(wtype, dtype, నిజం);
    }

    // The scaled OpenCL kernels (mul/div by a scale, reciprocal, addWeighted) compute in float; the old
    // same-type fast path forced max(depth, CV_32F). coerceTypes keeps same-type integer, so bump here.
    ఒకవేళ (oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE || oclop == OCL_OP_RDIV_SCALE ||
        oclop == OCL_OP_RECIP_SCALE || oclop == OCL_OP_ADDW)
        wtype = std::max(wtype, (పూర్ణాంకం)CV_32F);

    dtype = CV_MAKETYPE(dtype, cn);
    wtype = CV_MAKETYPE(wtype, cn);

    ఒకవేళ (have_ముసుగు)
    {
        పూర్ణాంకం mtype = ముసుగు_2.type();
        CV_Assert((mtype == CV_8UC1 || mtype == CV_8SC1 || mtype == CV_Bool) && ముసుగు_2.same_కొలత(*psrc1));
        reallocate = !_dst.same_కొలత(*psrc1) || _dst.type() != dtype;
    }

    _dst.create_Same_కొలత(*psrc1, dtype);
    ఒకవేళ (reallocate)
        _dst.సమితి_గమ్యం(0.);

    తిరిగి ocl_arithm_op(*psrc1, *psrc2, _dst, ముసుగు_2, wtype, usrdata, oclop, haveScalar);
#అయితే_ముగింపు
}

// Element-wise binary op (ADD/SUB/...) on two CPU operands - each an array Mat or a MATX scalar -
// through the new broadcasting engine. Handles mixed input types (emitBinary inserts the casts) and
// broadcasting (a scalar rides as a 0-dim per-channel CONST; arrays broadcast via outputShape/exec).
// Returns false (declining) for UMat, a write-mask, or two scalar operands, so the caller falls
// through to arithm_op. Incompatible shapes make the engine throw (matching cv::add's error).
// Does the output array already have exactly this shape+type (=> create() reuses it, no realloc)?
// Works for Mat and UMat alike (shape/type queries, no data-pointer peeking).
స్థిర తార్కిక బహిర్గతం_Array_Matches(స్థిరరాశి _OutputArray& a, స్థిరరాశి MatShape& shp, పూర్ణాంకం type)
{
    ఒకవేళ (a.empty() || a.type() != type)
        తిరిగి అబద్ధం;
    పూర్ణాంకం sz[MatShape::MAX_DIMS];
    పూర్ణాంకం nd = a.sizend(sz);
    ఒకవేళ (nd != (పూర్ణాంకం)shp.size())
        తిరిగి అబద్ధం;
    కొరకు (పూర్ణాంకం i = 0; i < nd; i++)
        ఒకవేళ (sz[i] != shp[i])
            తిరిగి అబద్ధం;
    తిరిగి నిజం;
}

// Are two operands' iteration shapes (spatial dims + channels innermost, exactly how the engine
// iterates) numpy-broadcast-compatible? Powers the scalar-like-Mat compat fallback in arithm_op.
// Cheap by design: sizend copies the dim arrays inline - no Mat headers, no allocations.
స్థిర తార్కిక shapes_ప్రసారం_Compat(ఆదానం_Array a, పూర్ణాంకం acn, ఆదానం_Array b, పూర్ణాంకం bcn)
{
    పూర్ణాంకం asz[MatShape::MAX_DIMS + 1], bsz[MatShape::MAX_DIMS + 1];
    పూర్ణాంకం ad = a.sizend(asz), bd = b.sizend(bsz);
    asz[ad++] = acn;
    bsz[bd++] = bcn;
    కొరకు (పూర్ణాంకం i1 = ad - 1, i2 = bd - 1; i1 >= 0 && i2 >= 0; i1--, i2--)
        ఒకవేళ (asz[i1] != bsz[i2] && asz[i1] != 1 && bsz[i2] != 1)
            తిరిగి అబద్ధం;
    తిరిగి నిజం;
}

// The one entry point for every element-wise binary op (add/sub/mul/div/min/max/absdiff/addWeighted/
// reciprocal). Handles empty inputs, the UMat/OpenCL branch (arithm_op_ocl) and the CPU element-wise
// engine. `oclop` selects the OpenCL kernel; `muldiv` drives the OpenCL working-type coercion + scale;
// `params` are the op scalars (params[0]=mul/div scale; {alpha,beta,gamma} for addWeighted).
// Below this many ELEMENTS (total * channels), the per-call constant overhead of the engine path
// (program build + compile + executor setup) is comparable to the actual work - take the direct
// kernel call instead. Same tier boundary as math_op's MATH_OP_SMALL for cv::exp.
గణన { ARITHM_SMALL_ప్రత్యక్ష = 100000 };

స్థిర శూన్య arithm_op(ew::శిఖరం op, ఆదానం_Array src1, ఆదానం_Array src2, OutputArray dst,
                      ఆదానం_Array ముసుగు, పూర్ణాంకం dtype, పూర్ణాంకం oclop, తార్కిక muldiv, స్థిరరాశి Scalar& params)
{
    ఒకవేళ (src1.empty() || src2.empty())          // empty inputs -> empty result
    {
        CV_Assert((src1.empty() && src2.empty()) ||
                  (src1.empty() && isScalarArg(src2, src1.channels())) ||
                  (src2.empty() && isScalarArg(src1, src2.channels())));
        dst.release();
        ఒకవేళ (dtype >= 0)
            dst.create(0, 0, dtype);
        తిరిగి;
    }

    // UMat -> classic OpenCL kernel. It declines (false) when OpenCL can't apply (no device, dims>2, a
    // broadcast, cn>4 masked, ...) - then the CPU engine below handles it (mapping the UMat via getMat).
    // oclop < 0 is a sentinel: the caller (e.g. cv::compare) already ran its own OpenCL path, so this
    // helper only runs the CPU engine (a UMat operand is mapped via getMat).
    ఒకవేళ (oclop >= 0 && cv::ocl::isOpenCLActivated() &&
        (src1.isUMat() || src2.isUMat() || dst.isUMat()))
    {
        ద్విగుణ abg[3] = { params[0], params[1], params[2] }, scale = params[0];
        శూన్య* usrdata = (op == ew::OP_ADDW) ? (శూన్య*)abg : (muldiv ? (శూన్య*)&scale : శూన్యసూచిక);
        ఒకవేళ (arithm_op_ocl(src1, src2, dst, ముసుగు, dtype, oclop, muldiv, usrdata))
            తిరిగి;
    }

    స్థిరరాశి తార్కిక have_ముసుగు = !ముసుగు.empty();
    స్థిరరాశి పూర్ణాంకం cn1 = src1.channels(), cn2 = src2.channels();
    తార్కిక s1 = isScalarArg(src1, src2), s2 = isScalarArg(src2, src1);
    ఒకవేళ (s1 && s2)
        s1 = s2 = అబద్ధం;    // two scalars: treat both as tiny array operands (element-wise + broadcast),
                            // matching arithm_op (Scalar+Scalar -> 4x1, number+number -> 1x1, ...)
    // A 4x1 CV_64F Mat/UMat pseudo-scalar (see isScalarArg) is hijacked only against a REAL array:
    // when the partner is itself a tiny scalar-shaped array (e.g. compare(Mat 4x1, Mat 1x1) -
    // issue #8999), both are honest data and ride the broadcast. A genuine MATX Scalar never demotes.
    లేకపోతే ఒకవేళ (s1 && src1.రకం() != ఆదానం_Array_2::MATX && isScalarLikeMat(src2, cn1))
        s1 = అబద్ధం;
    లేకపోతే ఒకవేళ (s2 && src2.రకం() != ఆదానం_Array_2::MATX && isScalarLikeMat(src1, cn2))
        s2 = అబద్ధం;

    // Compat fallback: Java/Python/user code passes scalars as real little Mats (the classic 4x1
    // CV_64F column, a 1xcn/cnx1 vector - see isScalarLikeMat). Those normally ride broadcasting
    // now, so hijack one as a per-channel scalar ONLY when the shapes do NOT broadcast - just the
    // calls that would otherwise throw get the 4.x scalar semantics, every valid broadcast keeps
    // its numpy meaning. Cheap: two geometry probes, and the O(ndims) shape walk runs only when a
    // probe hits (never for ordinary same-size or MATX-scalar calls).
    ఒకవేళ (!s1 && !s2)
    {
        తార్కిక like1 = isScalarLikeMat(src1, cn2), like2 = isScalarLikeMat(src2, cn1);
        ఒకవేళ ((like1 || like2) && !shapes_ప్రసారం_Compat(src1, cn1, src2, cn2))
        {
            s1 = like1;             // src1 preferred when both qualify, like the old checkScalar order
            s2 = like2 && !like1;
        }
    }

    // Direct Mat pointers - NO header copy / refcount atomics (the InputArrays outlive this call, so
    // their data stays alive). A MAT operand is used in place via getObj(); a non-MAT one (UMat, or a
    // two-scalar operand handled as an array) is mapped once into a local. EXCEPTION - in-place with a
    // realloc: if dst IS one of the sources, dst.create() may free that source's data mid-op, so take a
    // header copy (increfs, keeping the old data alive) of the sources in that case only.
    స్థిరరాశి శూన్య* dstObj = (dst.రకం() == ఆదానం_Array_2::MAT) ? dst.getObj() : శూన్యసూచిక;
    స్థిరరాశి తార్కిక aliased = dstObj && (dstObj == src1.getObj() || dstObj == src2.getObj());
    Mat m1loc, m2loc, mloc;
    స్వయంచాలక asMat = [&](ఆదానం_Array a, Mat& loc) -> స్థిరరాశి Mat* {
        ఒకవేళ (a.రకం() == ఆదానం_Array_2::MAT && !aliased) తిరిగి (స్థిరరాశి Mat*)a.getObj();
        loc = a.getMat(); తిరిగి &loc;
    };
    స్థిరరాశి Mat* pm1 = s1 ? శూన్యసూచిక : asMat(src1, m1loc);
    స్థిరరాశి Mat* pm2 = s2 ? శూన్యసూచిక : asMat(src2, m2loc);
    స్థిరరాశి పూర్ణాంకం adepth = s1 ? pm2->లోతు() : pm1->లోతు();   // the (first) array operand's depth
    // auto result depth (no explicit dtype, no fixed-type dst): add/sub/mul/div keep the first array
    // operand's depth (cv::'s dtype==-1 convention), but min/max/absdiff have NO dtype argument and
    // must promote mixed inputs to their common type (min(u32,f32) in u32 would drop the float). For
    // same-type inputs promoteArith(da,db)==da, so classic behaviour is preserved (e.g. absdiff of two
    // s16 stays s16, saturating - the wide unsigned |a-b| is computed then cast back down by emitBinary).
    పూర్ణాంకం auto_లోతు = adepth;
    ఒకవేళ (op == ew::OP_MIN || op == ew::OP_MAX || op == ew::OP_ABSDIFF)
    {
        స్థిరరాశి పూర్ణాంకం d1 = s1 ? ew::EW_లోతు_NONE : pm1->లోతు();
        స్థిరరాశి పూర్ణాంకం d2 = s2 ? ew::EW_లోతు_NONE : pm2->లోతు();
        auto_లోతు = ew::promoteArith(d1, d2);
    }
    // result depth: explicit dtype wins; else a fixed-type dst dictates it; else the auto depth above.
    స్థిరరాశి పూర్ణాంకం rdepth = dtype >= 0 ? CV_MAT_లోతు(dtype)
                     : (dst.fixedType() ? dst.లోతు() : auto_లోతు);

    // FAST PATH for the classic hot call: two same-type same-shape continuous arrays, no mask, no
    // scalar, no broadcast, result depth == input depth, and the array is SMALL. Building and
    // compiling the 1-instruction program plus the BroadcastOp setup costs ~40-250ns per call -
    // negligible on big images, dominant at 127x61-class sizes. Call the T x T -> T kernel directly
    // over the flattened elements instead (the same tier design as math_op for cv::exp). Only ops
    // whose emitBinary lowering for this exact type combination IS the plain direct kernel are
    // listed - compare (boundary rewrite/flags) and divide (int guards) lower to more than one
    // kernel call and keep the ordinary path. addWeighted qualifies exactly when its direct fused
    // T -> T kernel exists (u8..f32; the 32/64-bit ints lower to wide-compute + cast and get {}
    // from getElemwiseFunc below, falling through naturally).
    ఒకవేళ (!have_ముసుగు && !s1 && !s2 &&
        (op == ew::OP_కూడు || op == ew::OP_SUB || op == ew::OP_MIN || op == ew::OP_MAX ||
         op == ew::OP_ABSDIFF || op == ew::OP_MUL || op == ew::OP_ADDW ||
         op == ew::OP_AND || op == ew::OP_OR || op == ew::OP_XOR) &&
        rdepth == pm1->లోతు() && pm1->type() == pm2->type() && pm1->size == pm2->size &&
        pm1->isContinuous() && pm2->isContinuous() &&
        pm1->మొత్తం()*cn1 <= (size_t)ARITHM_SMALL_ప్రత్యక్ష)
    {
        ew::TKernel k = ew::getElemwiseFunc(op, rdepth, rdepth, ew::EW_లోతు_NONE, rdepth);
        ఒకవేళ (k.fptr)
        {
            dst.create_Same_కొలత(src1, pm1->type());   // whole-shape transfer, not piecemeal dims+sizes
            Mat dloc;
            Mat* pd = (dst.రకం() == ఆదానం_Array_2::MAT) ? (Mat*)dst.getObj() : &(dloc = dst.getMat());
            ఒకవేళ (pd->isContinuous())     // a reused non-continuous dst view falls through to the engine
            {
                k.fptr(pm1->దత్తాంశం, 0, 1, pm2->దత్తాంశం, 0, 1, శూన్యసూచిక, 0, 0,
                       pd->దత్తాంశం, 0, (పూర్ణాంకం)(pm1->మొత్తం()*cn1), 1, params.val, k.flags, k.userdata);
                తిరిగి;
            }
        }
    }

    // operand order = (src1, src2): an array becomes an INPUT, a scalar a flexible per-channel CONST
    // read straight from the caller's inline storage (getObj()), no Mat / convertTo.
    ew::TExpr p;
    స్వయంచాలక కూడు_నిర్వాహ్యం = [&](ఆదానం_Array src, తార్కిక isScalar, స్థిరరాశి Mat* m, పూర్ణాంకం otherCn) -> పూర్ణాంకం {
        ఒకవేళ (!isScalar)
            తిరిగి p.కూడు_ఆదానం(m->లోతు());
        స్థిరరాశి uchar* sp; పూర్ణాంకం sd;
        uchar scbuf[EW_SCALAR_BUF_కొలత];   // stack room for a UMAT scalar; addConst copies the values out
        స్థిరరాశి పూర్ణాంకం scn0 = scalarArgElems(src, sp, sd, scbuf), cn = otherCn;
        స్థిరరాశి పూర్ణాంకం scn = (cn == scn0 || (cn < 4 && scn0 == 4)) ? cn
                      : (scn0 == 1) ? 1 : 0;
        CV_Assert(scn > 0);
        తిరిగి p.కూడు_Const(ew::EW_లోతు_NONE, sd, sp, scn);
    };
    స్థిరరాశి పూర్ణాంకం a0 = కూడు_నిర్వాహ్యం(src1, s1, pm1, cn2);
    స్థిరరాశి పూర్ణాంకం a1 = కూడు_నిర్వాహ్యం(src2, s2, pm2, cn1);

    // Write-mask: a single-channel 1-byte array (u8/s8/bool) the size of the output spatial shape,
    // added as another broadcast input. The arithmetic result lands in a temp; a final
    // select(mask, r, dst) -> dst overwrites only mask!=0 positions of the (pre-existing) output.
    పూర్ణాంకం s_ముసుగు = 0;
    స్థిరరాశి Mat* pmask = శూన్యసూచిక;
    ఒకవేళ (have_ముసుగు)
    {
        CV_Assert(ముసుగు.type() == CV_8U || ముసుగు.type() == CV_8S || ముసుగు.type() == CV_Bool);
        pmask = asMat(ముసుగు, mloc);
        s_ముసుగు = p.కూడు_ఆదానం(pmask->లోతు());
    }
    స్థిరరాశి పూర్ణాంకం s_బహిర్గతం = p.కూడు_Output(rdepth);               // output BEFORE emit -> moveToOutput drops the
    స్థిరరాశి పూర్ణాంకం r = p.emit_ద్విమాన(op, a0, a1, rdepth, params);   // result temp (mask-free => none); scale
                                                        // rides params[0] (mul/div), {a,b,g} for addW
    ఒకవేళ (have_ముసుగు)
        p.కూడు_Insn(ew::OP_SELECT, s_ముసుగు, r, s_బహిర్గతం, s_బహిర్గతం);
    లేకపోతే
        p.move_గమ్యం_Output(r, s_బహిర్గతం);
    p.compile();

    స్థిరరాశి Mat* inputs[3]; పూర్ణాంకం ni = 0;                   // inputs in program (input-index) order
    ఒకవేళ (!s1) inputs[ni++] = pm1;
    ఒకవేళ (!s2) inputs[ni++] = pm2;
    ఒకవేళ (have_ముసుగు) inputs[ni++] = pmask;

    // Pre-create the output at the broadcast shape, ALWAYS via dst.create() - never hand exec a raw
    // dst Mat. Routing through the _OutputArray is what enforces its contract: a FIXED_SIZE/FIXED_TYPE
    // dst (in-place 'a += b' passes '(const Mat&)a') throws here if a broadcast would change its shape
    // or type (Mat(1x1) += Mat(1x4)), instead of silently reallocating - exec sees only a Mat and has
    // no view of the array's fixed flags. Cost over a direct exec is negligible: exec keeps its own
    // fast path for a matching-shape op, and dst.create() with an unchanged shape/type is a no-op.
    // With a mask, a freshly (re)allocated dst is zeroed so mask==0 reads 0 (matches arithm_op); a
    // reused dst keeps its prior content there - detect reuse by shape+type BEFORE create (correct for
    // a reused UMat dst too).
    MatShape oshape; పూర్ణాంకం ocn;
    p.outputShape(inputs, oshape, ocn);
    స్థిరరాశి పూర్ణాంకం otype = CV_MAKETYPE(rdepth, ocn);
    స్థిరరాశి తార్కిక reused = have_ముసుగు && బహిర్గతం_Array_Matches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.రకం() == ఆదానం_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    ఒకవేళ (have_ముసుగు && !reused)
        pdst->సమితి_Zero();                          // setZero (not setTo, which caps at 4 channels)
    p.exec(inputs, pdst);
}

// Unary element-wise op through the engine (bitwise NOT now; unary math later). One array input, an
// optional write-mask; result depth = dtype (>=0), else a fixed-type dst's depth, else the input
// depth. Mirrors arithm_op's build/mask/exec tail with a single input and no scalar operand. OpenCL is
// run by the caller (bitwise_op_ocl), so this is the CPU engine only.
స్థిర శూన్య unary_op(ew::శిఖరం op, ఆదానం_Array src, OutputArray dst, ఆదానం_Array ముసుగు,
                     పూర్ణాంకం dtype, స్థిరరాశి Scalar& params)
{
    ఒకవేళ (src.empty())
    {
        dst.release();
        ఒకవేళ (dtype >= 0)
            dst.create(0, 0, dtype);
        తిరిగి;
    }

    స్థిరరాశి తార్కిక have_ముసుగు = !ముసుగు.empty();
    // in-place (dst aliases src): keep a header copy so a shape/type-changing dst.create() can't free
    // src's data mid-op (matches arithm_op). A fixed-size/type dst never reallocs, so this is only for
    // an explicit dtype change.
    స్థిరరాశి శూన్య* dstObj = (dst.రకం() == ఆదానం_Array_2::MAT) ? dst.getObj() : శూన్యసూచిక;
    స్థిరరాశి తార్కిక aliased = dstObj && src.రకం() == ఆదానం_Array_2::MAT && dstObj == src.getObj();
    Mat sloc, mloc;
    స్థిరరాశి Mat* psrc = (src.రకం() == ఆదానం_Array_2::MAT && !aliased) ? (స్థిరరాశి Mat*)src.getObj()
                                                                   : &(sloc = src.getMat());
    స్థిరరాశి పూర్ణాంకం rdepth = dtype >= 0 ? CV_MAT_లోతు(dtype)
                     : (dst.fixedType() ? dst.లోతు() : psrc->లోతు());

    ew::TExpr p;
    స్థిరరాశి పూర్ణాంకం a0 = p.కూడు_ఆదానం(psrc->లోతు());
    పూర్ణాంకం s_ముసుగు = 0;
    స్థిరరాశి Mat* pmask = శూన్యసూచిక;
    ఒకవేళ (have_ముసుగు)
    {
        CV_Assert(ముసుగు.type() == CV_8U || ముసుగు.type() == CV_8S || ముసుగు.type() == CV_Bool);
        pmask = (ముసుగు.రకం() == ఆదానం_Array_2::MAT) ? (స్థిరరాశి Mat*)ముసుగు.getObj() : &(mloc = ముసుగు.getMat());
        s_ముసుగు = p.కూడు_ఆదానం(pmask->లోతు());
    }
    స్థిరరాశి పూర్ణాంకం s_బహిర్గతం = p.కూడు_Output(rdepth);
    స్థిరరాశి పూర్ణాంకం r = p.emitUnary(op, a0, rdepth, params);
    ఒకవేళ (have_ముసుగు)
        p.కూడు_Insn(ew::OP_SELECT, s_ముసుగు, r, s_బహిర్గతం, s_బహిర్గతం);
    లేకపోతే
        p.move_గమ్యం_Output(r, s_బహిర్గతం);
    p.compile();

    స్థిరరాశి Mat* inputs[2]; పూర్ణాంకం ni = 0;
    inputs[ni++] = psrc;
    ఒకవేళ (have_ముసుగు) inputs[ni++] = pmask;

    MatShape oshape; పూర్ణాంకం ocn;
    p.outputShape(inputs, oshape, ocn);
    స్థిరరాశి పూర్ణాంకం otype = CV_MAKETYPE(rdepth, ocn);
    స్థిరరాశి తార్కిక reused = have_ముసుగు && బహిర్గతం_Array_Matches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.రకం() == ఆదానం_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    ఒకవేళ (have_ముసుగు && !reused)
        pdst->సమితి_Zero();
    p.exec(inputs, pdst);
}

}

శూన్య cv::కూడు( ఆదానం_Array src1, ఆదానం_Array src2, OutputArray dst,
          ఆదానం_Array ముసుగు, పూర్ణాంకం dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_కూడు, src1, src2, dst, ముసుగు, dtype, OCL_OP_కూడు, అబద్ధం);
}

శూన్య cv::వ్యవకలనం( ఆదానం_Array src1, ఆదానం_Array src2, OutputArray dst,
                   ఆదానం_Array ముసుగు, పూర్ణాంకం dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_SUB, src1, src2, dst, ముసుగు, dtype, OCL_OP_SUB, అబద్ధం);
}

శూన్య cv::absdiff( ఆదానం_Array src1, ఆదానం_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ABSDIFF, src1, src2, dst, noArray(), -1, OCL_OP_ABSDIFF, అబద్ధం);
}

శూన్య cv::copy_గమ్యం(ఆదానం_Array _src, OutputArray _dst, ఆదానం_Array ముసుగు_2)
{
    CV_INSTRUMENT_REGION();

    _src.copy_గమ్యం(_dst, ముసుగు_2);
}

/****************************************************************************************\
*                                    multiply/divide                                     *
\****************************************************************************************/

నామాకాశ cv
{

శూన్య multiply(ఆదానం_Array src1, ఆదానం_Array src2,
              OutputArray dst, ద్విగుణ scale, పూర్ణాంకం dtype)
{
    CV_INSTRUMENT_REGION();

    స్థిరరాశి పూర్ణాంకం oclop = std::నిరపేక్ష(scale - 1.0) < DBL_EPSILON ? OCL_OP_MUL : OCL_OP_MUL_SCALE;
    arithm_op(ew::OP_MUL, src1, src2, dst, noArray(), dtype, oclop, /*muldiv*/ నిజం, Scalar(scale));
}

శూన్య divide(ఆదానం_Array src1, ఆదానం_Array src2,
                OutputArray dst, ద్విగుణ scale, పూర్ణాంకం dtype)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_DIV, src1, src2, dst, noArray(), dtype, OCL_OP_DIV_SCALE, /*muldiv*/ నిజం, Scalar(scale));
}

శూన్య divide(ద్విగుణ scale, ఆదానం_Array src2,
                OutputArray dst, పూర్ణాంకం dtype)
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ (src2.empty())
    {
        dst.release();
        తిరిగి;
    }

    // scale / src2 == scale * 1 / src2: feed a 0-dim `1` as the numerator so the CPU engine reuses the
    // normal divide (scale*a/b). Give the numerator src2's OWN depth: then OP_DIV(T, T) is same-type and
    // takes the fast per-type kernel (f32 work for <=16-bit / f16 / bf16, not the s32->f64 path a CV_32S
    // numerator would force). An integer src2 keeps its div-by-zero->0 guard (both operands integer); a
    // float src2 divides as float (1/0->inf), both matching cv::divide. dtype<0 follows src2's depth (a
    // fixed-type dst still wins). The UMat path uses the dedicated reciprocal kernel (OCL_OP_RECIP_SCALE).
    ద్విగుణ one = 1;
    Mat numerator(MatShape::scalar(), src2.లోతు(), &one);
    scalar_గమ్యం_Raw_దత్తాంశం(Scalar(1), &one, src2.లోతు(), 1);       // write `1` in src2's depth (no allocation)
    స్థిరరాశి పూర్ణాంకం rtype = (dtype < 0 && !dst.fixedType()) ? src2.లోతు() : dtype;
    arithm_op(ew::OP_DIV, numerator, src2, dst, noArray(), rtype, OCL_OP_RECIP_SCALE, /*muldiv*/ నిజం, Scalar(scale));
}

UMat UMat::mul(ఆదానం_Array m, ద్విగుణ scale) స్థిరరాశి
{
    UMat dst;
    multiply(*ఇది, m, dst, scale);
    తిరిగి dst;
}

/****************************************************************************************\
*                                      addWeighted                                       *
\****************************************************************************************/

}

శూన్య cv::కూడు_Weighted( ఆదానం_Array src1, ద్విగుణ alpha, ఆదానం_Array src2,
                      ద్విగుణ beta, ద్విగుణ gamma, OutputArray dst, పూర్ణాంకం dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADDW, src1, src2, dst, noArray(), dtype, OCL_OP_ADDW, /*muldiv*/ నిజం,
              Scalar(alpha, beta, gamma));
}


/****************************************************************************************\
*                                          compare                                       *
\****************************************************************************************/

నామాకాశ cv
{


స్థిర ద్విగుణ getMinVal(పూర్ణాంకం లోతు)
{
    స్థిర స్థిరరాశి ద్విగుణ tab[CV_లోతు_MAX] =
    {
        0, -128, 0, -32768, INT_MIN, -FLT_MAX, -DBL_MAX,
        -65504, -FLT_MAX, 0, 0, (ద్విగుణ)INT64_MIN, 0
    };
    తిరిగి tab[లోతు];
}

స్థిర ద్విగుణ getMaxVal(పూర్ణాంకం లోతు)
{
    స్థిర స్థిరరాశి ద్విగుణ tab[CV_లోతు_MAX] = {
        255, 127, 65535, 32767, గరిష్ఠ_పూర్ణాంకం, FLT_MAX, DBL_MAX,
        65504, FLT_MAX, 255, (ద్విగుణ)UINT64_MAX, (ద్విగుణ)INT64_MAX, (ద్విగుణ)UINT32_MAX, 0
    };
    తిరిగి tab[లోతు];
}

#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_compare(ఆదానం_Array _src1, ఆదానం_Array _src2, OutputArray _dst, పూర్ణాంకం op, తార్కిక haveScalar)
{
    స్థిరరాశి ocl::Device& dev = ocl::Device::getDefault();
    తార్కిక doubleSupport = dev.doubleFPConfig() > 0;
    పూర్ణాంకం type1 = _src1.type(), depth1 = CV_MAT_లోతు(type1), cn = CV_MAT_CN(type1),
            type2 = _src2.type(), depth2 = CV_MAT_లోతు(type2);

    ఒకవేళ (!doubleSupport && depth1 == CV_64F)
        తిరిగి అబద్ధం;

    // the integer scalar path below works in int; let the CPU handle the wider 5.x integer depths
    ఒకవేళ (haveScalar && (depth1 == CV_32U || depth1 == CV_64U || depth1 == CV_64S))
        తిరిగి అబద్ధం;

    ఒకవేళ (!haveScalar && (!_src1.same_కొలత(_src2) || type1 != type2))
            తిరిగి అబద్ధం;

    // This OpenCL kernel broadcasts a scalar's FIRST channel to all channels. The CPU path compares
    // per-channel (scalar[c] for channel c), so a multichannel array against a scalar with DISTINCT
    // channel values would disagree - bail to the CPU engine in that case (rare; not worth a kernel fix).
    ఒకవేళ (haveScalar && cn > 1)
    {
        Mat sc = _src2.getMat();
        పూర్ణాంకం scn = (పూర్ణాంకం)sc.మొత్తం();
        ఒకవేళ (scn > 1)
        {
            ద్విగుణ v[4] = { 0, 0, 0, 0 };
            getConvertFunc(sc.లోతు(), CV_64F)(sc.ptr(), 1, 0, 1, (uchar*)v, 1, కొలత(std::min(scn, 4), 1), 0);
            కొరకు (పూర్ణాంకం i = 1, n = std::min(cn, scn); i < n; i++)
                ఒకవేళ (v[i] != v[0])
                    తిరిగి అబద్ధం;
        }
    }

    పూర్ణాంకం kercn = haveScalar ? cn : ocl::predict_Optimal_సదిశ_వెడల్పు(_src1, _src2, _dst), అడ్డువరుసలు_చొప్పున_WI = dev.isIntel() ? 4 : 1;
    // Workaround for bug with "?:" operator in AMD OpenCL compiler
    ఒకవేళ (depth1 >= CV_16U)
        kercn = 1;

    పూర్ణాంకం scalarcn = kercn == 3 ? 4 : kercn;
    స్థిరరాశి అక్షరం * స్థిరరాశి operationMap[] = { "==", ">", ">=", "<", "<=", "!=" };
    అక్షరం cvt[50];

    String opts = format("-D %s -D srcT1=%s -D dstT=%s -D DEPTH_dst=%d -D workT=srcT1 -D cn=%d"
                         " -D convertToDT=%s -D OP_CMP -D CMP_OPERATOR=%s -D srcT1_C1=%s"
                         " -D srcT2_C1=%s -D dstT_C1=%s -D workST=%s -D rowsPerWI=%d%s",
                         haveScalar ? "UNARY_OP" : "BINARY_OP",
                         ocl::type_గమ్యం_Str(CV_MAKE_TYPE(depth1, kercn)),
                         ocl::type_గమ్యం_Str(CV_8UC(kercn)), CV_8U, kercn,
                         ocl::convertTypeStr(depth1, CV_8U, kercn, cvt, పరిమాణం(cvt)),
                         operationMap[op], ocl::type_గమ్యం_Str(depth1),
                         ocl::type_గమ్యం_Str(depth1), ocl::type_గమ్యం_Str(CV_8U),
                         ocl::type_గమ్యం_Str(CV_MAKE_TYPE(depth1, scalarcn)), అడ్డువరుసలు_చొప్పున_WI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat src1 = _src1.getUMat();
    కొలత size = src1.size();
    _dst.create(size, CV_8UC(cn));
    UMat dst = _dst.getUMat();

    ఒకవేళ (haveScalar)
    {
        size_t esz = CV_ELEM_SIZE1(type1) * scalarcn;
        ద్విగుణ buf[4] = { 0, 0, 0, 0 };
        Mat src2 = _src2.getMat();

        ఒకవేళ( depth1 > CV_32S )
            convertAndUnrollScalar( src2, depth1, (uchar *)buf, kercn );
        లేకపోతే
        {
            ద్విగుణ fval = 0;
            getConvertFunc(depth2, CV_64F)(src2.ptr(), 1, 0, 1, (uchar *)&fval, 1, కొలత(1, 1), 0);
            ఒకవేళ( fval < getMinVal(depth1) )
                తిరిగి dst.సమితి_గమ్యం(Scalar::all(op == CMP_GT || op == CMP_GE || op == CMP_NE ? 255 : 0)), నిజం;

            ఒకవేళ( fval > getMaxVal(depth1) )
                తిరిగి dst.సమితి_గమ్యం(Scalar::all(op == CMP_LT || op == CMP_LE || op == CMP_NE ? 255 : 0)), నిజం;

            పూర్ణాంకం ival = cv_విడత(fval);
            ఒకవేళ( fval != ival )
            {
                ఒకవేళ( op == CMP_LT || op == CMP_GE )
                    ival = cvCeil(fval);
                లేకపోతే ఒకవేళ( op == CMP_LE || op == CMP_GT )
                    ival = cvFloor(fval);
                లేకపోతే
                    తిరిగి dst.సమితి_గమ్యం(Scalar::all(op == CMP_NE ? 255 : 0)), నిజం;
            }
            convertAndUnrollScalar(Mat(1, 1, CV_32S, &ival), depth1, (uchar *)buf, kercn);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(src1, cn, kercn),
               ocl::KernelArg::WriteOnly(dst, cn, kercn), scalararg);
    }
    లేకపోతే
    {
        UMat src2 = _src2.getUMat();

        k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(src1),
               ocl::KernelArg::Read_Only_No_కొలత(src2),
               ocl::KernelArg::WriteOnly(dst, cn, kercn));
    }

    size_t globalsize[2] = { (size_t)dst.నిలువుల_సంఖ్య * cn / kercn, ((size_t)dst.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

#అయితే_ముగింపు

}

శూన్య cv::compare(ఆదానం_Array _src1, ఆదానం_Array _src2, OutputArray _dst, పూర్ణాంకం op)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( op == CMP_LT || op == CMP_LE || op == CMP_EQ ||
               op == CMP_NE || op == CMP_GE || op == CMP_GT );

    CV_Assert(_src1.empty() == _src2.empty());
    ఒకవేళ (_src1.empty() && _src2.empty())
    {
        _dst.release();
        తిరిగి;
    }

    తార్కిక haveScalar = అబద్ధం;

    ఒకవేళ ((_src1.isMatx() + _src2.isMatx()) == 1
            || !_src1.same_కొలత(_src2)
            || _src1.type() != _src2.type())
    {
        తార్కిక is_src1_scalar = checkScalar(_src1, _src2.type(), _src1.రకం(), _src2.రకం());
        తార్కిక is_src2_scalar = checkScalar(_src2, _src1.type(), _src2.రకం(), _src1.రకం());

        // exactly one scalar operand: keep it as src2 (swapping flips the ordering ops) so the OpenCL
        // path sees the array-op-scalar form. Otherwise - two arrays of different size (broadcast),
        // mixed types, or two scalars - the CPU engine broadcasts / promotes them, so there is NO error
        // here: fall through to arithm_op (which handles scalar detection, broadcast and mixed types).
        ఒకవేళ (is_src1_scalar && !is_src2_scalar)
        {
            op = op == CMP_LT ? CMP_GT : op == CMP_LE ? CMP_GE :
                op == CMP_GE ? CMP_LE : op == CMP_GT ? CMP_LT : op;
            // src1 is a scalar; swap it with src2
            compare(_src2, _src1, _dst, op);
            తిరిగి;
        }
        haveScalar = is_src2_scalar && !is_src1_scalar;
    }
    CV_వాడని(haveScalar);   // consumed by CV_OCL_RUN only - a no-op without OpenCL

    // OpenCL handles only the same-size and array-op-scalar forms; a broadcast between two arrays of
    // different size falls through to the CPU engine.
    CV_OCL_నిరంతరం(_src1.dims() <= 2 && _src2.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat())
               && (haveScalar || _src1.same_కొలత(_src2)),
               ocl_compare(_src1, _src2, _dst, op, haveScalar))

    // CPU: the element-wise engine. A CMP_* code maps to the corresponding OP_CMP_* TOp; the engine
    // forces a u8 mask (dtype = CV_8U) and compares in the common operand type (a fractional scalar
    // threshold promotes both operands to float - matching the classic ceil/floor threshold logic).
    // The scalar / broadcast / mixed-shape cases are handled inside arithm_op. oclop = -1 tells it the
    // OpenCL path was already tried above (via ocl_compare), so it only runs the CPU engine.
    ew::శిఖరం cmpOp = op == CMP_EQ ? ew::OP_CMP_EQ : op == CMP_NE ? ew::OP_CMP_NE :
                    op == CMP_LT ? ew::OP_CMP_LT : op == CMP_LE ? ew::OP_CMP_LE :
                    op == CMP_GT ? ew::OP_CMP_GT : ew::OP_CMP_GE;
    arithm_op(cmpOp, _src1, _src2, _dst, noArray(), CV_8U, /*oclop=*/-1, /*muldiv=*/అబద్ధం);
}

/****************************************************************************************\
*                                        inRange                                         *
\****************************************************************************************/

నామాకాశ cv
{

మూస <రకం_పేరు T>
నిర్మాణం లోపల_Range_SIMD
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి T * src1, స్థిరరాశి T * src2, స్థిరరాశి T * src3, uchar * dst, పూర్ణాంకం len) స్థిరరాశి
    {
        కొరకు (పూర్ణాంకం x = 0; x < len; x++)
            dst[x] = (src2[x] <= src1[x] && src1[x] <= src3[x]) ? 255 : 0;
        తిరిగి len;
    }
};

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)

మూస <>
నిర్మాణం లోపల_Range_SIMD<uchar>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి uchar * src1, స్థిరరాశి uchar * src2, స్థిరరాశి uchar * src3,
        uchar * dst, పూర్ణాంకం len) స్థిరరాశి
    {
        పూర్ణాంకం x = 0;
        స్థిరరాశి పూర్ణాంకం వెడల్పు = VTraits<v_uint8>::vlanes();

        కొరకు (; x <= len - వెడల్పు; x += వెడల్పు)
        {
            v_uint8 values = vx_ఎక్కించు(src1 + x);
            v_uint8 దిగువ = vx_ఎక్కించు(src2 + x);
            v_uint8 ఎగువ = vx_ఎక్కించు(src3 + x);

            v_store(dst + x, v_and(v_ge(values, దిగువ), v_ge(ఎగువ, values)));
        }
        vx_cleanup();
        తిరిగి x;
    }
};

మూస <>
నిర్మాణం లోపల_Range_SIMD<schar>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి schar * src1, స్థిరరాశి schar * src2, స్థిరరాశి schar * src3,
        uchar * dst, పూర్ణాంకం len) స్థిరరాశి
    {
        పూర్ణాంకం x = 0;
        స్థిరరాశి పూర్ణాంకం వెడల్పు = VTraits<v_int8>::vlanes();

        కొరకు (; x <= len - వెడల్పు; x += వెడల్పు)
        {
            v_int8 values = vx_ఎక్కించు(src1 + x);
            v_int8 దిగువ = vx_ఎక్కించు(src2 + x);
            v_int8 ఎగువ = vx_ఎక్కించు(src3 + x);

            v_store((schar*)(dst + x), v_and(v_ge(values, దిగువ), v_ge(ఎగువ, values)));
        }
        vx_cleanup();
        తిరిగి x;
    }
};

మూస <>
నిర్మాణం లోపల_Range_SIMD<ushort>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి ushort * src1, స్థిరరాశి ushort * src2, స్థిరరాశి ushort * src3,
        uchar * dst, పూర్ణాంకం len) స్థిరరాశి
    {
        పూర్ణాంకం x = 0;
        స్థిరరాశి పూర్ణాంకం వెడల్పు = VTraits<v_uint16>::vlanes() * 2;

        కొరకు (; x <= len - వెడల్పు; x += వెడల్పు)
        {
            v_uint16 values1 = vx_ఎక్కించు(src1 + x);
            v_uint16 low1 = vx_ఎక్కించు(src2 + x);
            v_uint16 high1 = vx_ఎక్కించు(src3 + x);

            v_uint16 values2 = vx_ఎక్కించు(src1 + x + VTraits<v_uint16>::vlanes());
            v_uint16 low2 = vx_ఎక్కించు(src2 + x + VTraits<v_uint16>::vlanes());
            v_uint16 high2 = vx_ఎక్కించు(src3 + x + VTraits<v_uint16>::vlanes());

            v_store(dst + x, v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2))));
        }
        vx_cleanup();
        తిరిగి x;
    }
};

మూస <>
నిర్మాణం లోపల_Range_SIMD<పొట్టి>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి పొట్టి * src1, స్థిరరాశి పొట్టి * src2, స్థిరరాశి పొట్టి * src3,
        uchar * dst, పూర్ణాంకం len) స్థిరరాశి
    {
        పూర్ణాంకం x = 0;
        స్థిరరాశి పూర్ణాంకం వెడల్పు = (పూర్ణాంకం)VTraits<v_int16>::vlanes() * 2;

        కొరకు (; x <= len - వెడల్పు; x += వెడల్పు)
        {
            v_int16 values1 = vx_ఎక్కించు(src1 + x);
            v_int16 low1 = vx_ఎక్కించు(src2 + x);
            v_int16 high1 = vx_ఎక్కించు(src3 + x);

            v_int16 values2 = vx_ఎక్కించు(src1 + x + VTraits<v_int16>::vlanes());
            v_int16 low2 = vx_ఎక్కించు(src2 + x + VTraits<v_int16>::vlanes());
            v_int16 high2 = vx_ఎక్కించు(src3 + x + VTraits<v_int16>::vlanes());

            v_store((schar*)(dst + x), v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2))));
        }
        vx_cleanup();
        తిరిగి x;
    }
};

మూస <>
నిర్మాణం లోపల_Range_SIMD<పూర్ణాంకం>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి పూర్ణాంకం * src1, స్థిరరాశి పూర్ణాంకం * src2, స్థిరరాశి పూర్ణాంకం * src3,
        uchar * dst, పూర్ణాంకం len) స్థిరరాశి
    {
        పూర్ణాంకం x = 0;
        స్థిరరాశి పూర్ణాంకం వెడల్పు = (పూర్ణాంకం)VTraits<v_int32>::vlanes() * 2;

        కొరకు (; x <= len - వెడల్పు; x += వెడల్పు)
        {
            v_int32 values1 = vx_ఎక్కించు(src1 + x);
            v_int32 low1 = vx_ఎక్కించు(src2 + x);
            v_int32 high1 = vx_ఎక్కించు(src3 + x);

            v_int32 values2 = vx_ఎక్కించు(src1 + x + VTraits<v_int32>::vlanes());
            v_int32 low2 = vx_ఎక్కించు(src2 + x + VTraits<v_int32>::vlanes());
            v_int32 high2 = vx_ఎక్కించు(src3 + x + VTraits<v_int32>::vlanes());

            v_pack_store(dst + x, v_reinterpret_as_u16(v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2)))));
        }
        vx_cleanup();
        తిరిగి x;
    }
};

మూస <>
నిర్మాణం లోపల_Range_SIMD<దశాంశం>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి దశాంశం * src1, స్థిరరాశి దశాంశం * src2, స్థిరరాశి దశాంశం * src3,
        uchar * dst, పూర్ణాంకం len) స్థిరరాశి
    {
        పూర్ణాంకం x = 0;
        స్థిరరాశి పూర్ణాంకం వెడల్పు = (పూర్ణాంకం)VTraits<v_float32>::vlanes() * 2;

        కొరకు (; x <= len - వెడల్పు; x += వెడల్పు)
        {
            v_float32 values1 = vx_ఎక్కించు(src1 + x);
            v_float32 low1 = vx_ఎక్కించు(src2 + x);
            v_float32 high1 = vx_ఎక్కించు(src3 + x);

            v_float32 values2 = vx_ఎక్కించు(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_ఎక్కించు(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_ఎక్కించు(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        తిరిగి x;
    }
};

మూస <>
నిర్మాణం లోపల_Range_SIMD<hfloat>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి hfloat * src1, స్థిరరాశి hfloat * src2, స్థిరరాశి hfloat * src3,
        uchar * dst, పూర్ణాంకం len) స్థిరరాశి
    {
        పూర్ణాంకం x = 0;
        స్థిరరాశి పూర్ణాంకం వెడల్పు = (పూర్ణాంకం)VTraits<v_float32>::vlanes()*2;

        కొరకు (; x <= len - వెడల్పు; x += వెడల్పు)
        {
            v_float32 values1 = vx_ఎక్కించు_expand(src1 + x);
            v_float32 low1 = vx_ఎక్కించు_expand(src2 + x);
            v_float32 high1 = vx_ఎక్కించు_expand(src3 + x);

            v_float32 values2 = vx_ఎక్కించు_expand(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_ఎక్కించు_expand(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_ఎక్కించు_expand(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        తిరిగి x;
    }
};

మూస <>
నిర్మాణం లోపల_Range_SIMD<bfloat>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి bfloat * src1, స్థిరరాశి bfloat * src2, స్థిరరాశి bfloat * src3,
        uchar * dst, పూర్ణాంకం len) స్థిరరాశి
    {
        పూర్ణాంకం x = 0;
        స్థిరరాశి పూర్ణాంకం వెడల్పు = (పూర్ణాంకం)VTraits<v_float32>::vlanes()*2;

        కొరకు (; x <= len - వెడల్పు; x += వెడల్పు)
        {
            v_float32 values1 = vx_ఎక్కించు_expand(src1 + x);
            v_float32 low1 = vx_ఎక్కించు_expand(src2 + x);
            v_float32 high1 = vx_ఎక్కించు_expand(src3 + x);

            v_float32 values2 = vx_ఎక్కించు_expand(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_ఎక్కించు_expand(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_ఎక్కించు_expand(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        తిరిగి x;
    }
};

మూస <>
నిర్మాణం లోపల_Range_SIMD<గుర్తులేని>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి గుర్తులేని * src1, స్థిరరాశి గుర్తులేని * src2, స్థిరరాశి గుర్తులేని * src3,
        uchar * dst, పూర్ణాంకం len) స్థిరరాశి
    {
        పూర్ణాంకం x = 0;
        స్థిరరాశి పూర్ణాంకం వెడల్పు = (పూర్ణాంకం)VTraits<v_uint32>::vlanes() * 2;

        కొరకు (; x <= len - వెడల్పు; x += వెడల్పు)
        {
            v_uint32 values1 = vx_ఎక్కించు(src1 + x);
            v_uint32 low1 = vx_ఎక్కించు(src2 + x);
            v_uint32 high1 = vx_ఎక్కించు(src3 + x);

            v_uint32 values2 = vx_ఎక్కించు(src1 + x + VTraits<v_uint32>::vlanes());
            v_uint32 low2 = vx_ఎక్కించు(src2 + x + VTraits<v_uint32>::vlanes());
            v_uint32 high2 = vx_ఎక్కించు(src3 + x + VTraits<v_uint32>::vlanes());

            v_pack_store(dst + x, v_reinterpret_as_u16(v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2)))));
        }
        vx_cleanup();
        తిరిగి x;
    }
};

#ఒకవేళ CV_SIMD_64F

మూస <>
నిర్మాణం లోపల_Range_SIMD<ద్విగుణ>
{
    పూర్ణాంకం క్రియాకారకం () (స్థిరరాశి ద్విగుణ * src1, స్థిరరాశి ద్విగుణ * src2, స్థిరరాశి ద్విగుణ * src3,
        uchar * dst, పూర్ణాంకం len) స్థిరరాశి
    {
        పూర్ణాంకం x = 0;
        స్థిరరాశి పూర్ణాంకం అడుగు = VTraits<v_float64>::vlanes();
        స్థిరరాశి పూర్ణాంకం వెడల్పు = అడుగు * 4;

        కొరకు (; x <= len - వెడల్పు; x += వెడల్పు)
        {
            v_float64 v1 = vx_ఎక్కించు(src1 + x);
            v_float64 l1 = vx_ఎక్కించు(src2 + x);
            v_float64 h1 = vx_ఎక్కించు(src3 + x);
            v_uint64 m1 = v_reinterpret_as_u64(v_and(v_ge(v1, l1), v_ge(h1, v1)));

            v_float64 v2 = vx_ఎక్కించు(src1 + x + అడుగు);
            v_float64 l2 = vx_ఎక్కించు(src2 + x + అడుగు);
            v_float64 h2 = vx_ఎక్కించు(src3 + x + అడుగు);
            v_uint64 m2 = v_reinterpret_as_u64(v_and(v_ge(v2, l2), v_ge(h2, v2)));

            v_float64 v3 = vx_ఎక్కించు(src1 + x + అడుగు * 2);
            v_float64 l3 = vx_ఎక్కించు(src2 + x + అడుగు * 2);
            v_float64 h3 = vx_ఎక్కించు(src3 + x + అడుగు * 2);
            v_uint64 m3 = v_reinterpret_as_u64(v_and(v_ge(v3, l3), v_ge(h3, v3)));

            v_float64 v4 = vx_ఎక్కించు(src1 + x + అడుగు * 3);
            v_float64 l4 = vx_ఎక్కించు(src2 + x + అడుగు * 3);
            v_float64 h4 = vx_ఎక్కించు(src3 + x + అడుగు * 3);
            v_uint64 m4 = v_reinterpret_as_u64(v_and(v_ge(v4, l4), v_ge(h4, v4)));


            v_pack_store(dst + x, v_pack(v_pack(m1, m2), v_pack(m3, m4)));
        }
        vx_cleanup();
        తిరిగి x;
    }
};

#అయితే_ముగింపు

#అయితే_ముగింపు

మూస <రకం_పేరు T>
స్థిర శూన్య లోపల_Range(స్థిరరాశి శూన్య* _src1, size_t step1, స్థిరరాశి శూన్య* _src2, size_t step2,
         స్థిరరాశి శూన్య* _src3, size_t step3, uchar* dst, size_t అడుగు,
         కొలత size)
{
    స్థిరరాశి T* src1 = (స్థిరరాశి T*)_src1;
    స్థిరరాశి T* src2 = (స్థిరరాశి T*)_src2;
    స్థిరరాశి T* src3 = (స్థిరరాశి T*)_src3;
    step1 /= పరిమాణం(src1[0]);
    step2 /= పరిమాణం(src2[0]);
    step3 /= పరిమాణం(src3[0]);

    లోపల_Range_SIMD<T> vop;

    కొరకు( ; size.ఎత్తు--; src1 += step1, src2 += step2, src3 += step3, dst += అడుగు )
    {
        పూర్ణాంకం x = vop(src1, src2, src3, dst, size.వెడల్పు);
        #ఒకవేళ CV_ENABLE_UNROLLED
        కొరకు( ; x <= size.వెడల్పు - 4; x += 4 )
        {
            పూర్ణాంకం t0, t1;
            t0 = src2[x] <= src1[x] && src1[x] <= src3[x];
            t1 = src2[x+1] <= src1[x+1] && src1[x+1] <= src3[x+1];
            dst[x] = (uchar)-t0; dst[x+1] = (uchar)-t1;
            t0 = src2[x+2] <= src1[x+2] && src1[x+2] <= src3[x+2];
            t1 = src2[x+3] <= src1[x+3] && src1[x+3] <= src3[x+3];
            dst[x+2] = (uchar)-t0; dst[x+3] = (uchar)-t1;
        }
        #అయితే_ముగింపు
        కొరకు( ; x < size.వెడల్పు; x++ )
            dst[x] = (uchar)-(src2[x] <= src1[x] && src1[x] <= src3[x]);
    }
}

స్థిర శూన్య లోపల_Range_Reduce(స్థిరరాశి uchar* src, uchar* dst, size_t len, పూర్ణాంకం cn)
{
    పూర్ణాంకం k = cn % 4 ? cn % 4 : 4;
    size_t i, j;
    ఒకవేళ( k == 1 )
        కొరకు( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j];
    లేకపోతే ఒకవేళ( k == 2 )
        కొరకు( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1];
    లేకపోతే ఒకవేళ( k == 3 )
        కొరకు( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2];
    లేకపోతే
        కొరకు( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2] & src[j+3];

    కొరకు( ; k < cn; k += 4 )
    {
        కొరకు( i = 0, j = k; i < len; i++, j += cn )
            dst[i] &= src[j] & src[j+1] & src[j+2] & src[j+3];
    }
}

రకనిర్వచనం శూన్య (*లోపల_Range_Func)( స్థిరరాశి శూన్య* src1, size_t step1, స్థిరరాశి శూన్య* src2, size_t step2,
                             స్థిరరాశి శూన్య* src3, size_t step3, uchar* dst, size_t అడుగు, కొలత sz );

స్థిర లోపల_Range_Func get_లోపల_Range_Func(పూర్ణాంకం లోతు)
{
    స్థిర లోపల_Range_Func లోపల_Range_Tab[CV_లోతు_MAX] =
    {
        లోపల_Range<uchar>,
        లోపల_Range<schar>,
        లోపల_Range<ushort>,
        లోపల_Range<పొట్టి>,
        లోపల_Range<పూర్ణాంకం>,
        లోపల_Range<దశాంశం>,
        లోపల_Range<ద్విగుణ>,
        లోపల_Range<hfloat>,
        లోపల_Range<bfloat>,
        0,
        లోపల_Range<uint64_t>,
        లోపల_Range<int64_t>,
        లోపల_Range<గుర్తులేని>,
        0,
    };

    తిరిగి లోపల_Range_Tab[లోతు];
}

#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_లోపల_Range( ఆదానం_Array _src, ఆదానం_Array _lowerb,
                         ఆదానం_Array _upperb, OutputArray _dst )
{
    స్థిరరాశి ocl::Device & d = ocl::Device::getDefault();
    ఆదానం_Array_2::రకం_Flag skind = _src.రకం(), lkind = _lowerb.రకం(), ukind = _upperb.రకం();
    కొలత ssize = _src.size(), lsize = _lowerb.size(), usize = _upperb.size();
    పూర్ణాంకం stype = _src.type(), ltype = _lowerb.type(), utype = _upperb.type();
    పూర్ణాంకం sdepth = CV_MAT_లోతు(stype), ldepth = CV_MAT_లోతు(ltype), udepth = CV_MAT_లోతు(utype);
    పూర్ణాంకం cn = CV_MAT_CN(stype), అడ్డువరుసలు_చొప్పున_WI = d.isIntel() ? 4 : 1;
    తార్కిక lbScalar = అబద్ధం, ubScalar = అబద్ధం;

    ఒకవేళ( (lkind == ఆదానం_Array_2::MATX && skind != ఆదానం_Array_2::MATX) ||
        ssize != lsize || stype != ltype )
    {
        ఒకవేళ( !checkScalar(_lowerb, stype, lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = నిజం;
    }

    ఒకవేళ( (ukind == ఆదానం_Array_2::MATX && skind != ఆదానం_Array_2::MATX) ||
        ssize != usize || stype != utype )
    {
        ఒకవేళ( !checkScalar(_upperb, stype, ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = నిజం;
    }

    ఒకవేళ (lbScalar != ubScalar)
        తిరిగి అబద్ధం;

    తార్కిక doubleSupport = d.doubleFPConfig() > 0,
            haveScalar = lbScalar && ubScalar;

    ఒకవేళ ( (!doubleSupport && sdepth == CV_64F) ||
         (!haveScalar && (sdepth != ldepth || sdepth != udepth)) )
        తిరిగి అబద్ధం;

    పూర్ణాంకం kercn = haveScalar ? cn : std::max(std::min(ocl::predict_Optimal_సదిశ_వెడల్పు(_src, _lowerb, _upperb, _dst), 4), cn);
    ఒకవేళ (kercn % cn != 0)
        kercn = cn;
    పూర్ణాంకం నిలువుల_సంఖ్య_చొప్పున_WI = kercn / cn;
    String opts = format("%s-D CN=%d -D SRC_T=%s -D SRC_T1=%s -D DST_T=%s -D KERCN=%d -D DEPTH=%d%s -D COLS_PER_WI=%d",
                           haveScalar ? "-D HAVE_SCALAR " : "", cn, ocl::type_గమ్యం_Str(CV_MAKE_TYPE(sdepth, kercn)),
                           ocl::type_గమ్యం_Str(sdepth), ocl::type_గమ్యం_Str(CV_8UC(నిలువుల_సంఖ్య_చొప్పున_WI)), kercn, sdepth,
                           doubleSupport ? " -D DOUBLE_SUPPORT" : "", నిలువుల_సంఖ్య_చొప్పున_WI);

    ocl::Kernel ker("inrange", ocl::core::inrange_oclsrc, opts);
    ఒకవేళ (ker.empty())
        తిరిగి అబద్ధం;

    _dst.create(ssize, CV_8UC1);
    UMat src = _src.getUMat(), dst = _dst.getUMat(), lscalaru, uscalaru;
    Mat lscalar, uscalar;

    ఒకవేళ (lbScalar && ubScalar)
    {
        lscalar = _lowerb.getMat();
        uscalar = _upperb.getMat();

        size_t esz = src.elem_కొలత();
        size_t blocksize = 36;

        Auto_బఫరు<uchar> _buf(blocksize*(((పూర్ణాంకం)lbScalar + (పూర్ణాంకం)ubScalar)*esz + cn) + 2*cn*పరిమాణం(పూర్ణాంకం) + 128);
        uchar *buf = alignPtr(_buf.దత్తాంశం() + blocksize*cn, 16);

        ఒకవేళ( ldepth != sdepth && sdepth < CV_32S )
        {
            పూర్ణాంకం* ilbuf = (పూర్ణాంకం*)alignPtr(buf + blocksize*esz, 16);
            పూర్ణాంకం* iubuf = ilbuf + cn;

            ద్విమాన_Func sccvtfunc = getConvertFunc(ldepth, CV_32S);
            sccvtfunc(lscalar.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, కొలత(cn, 1), 0);
            sccvtfunc(uscalar.ptr(), 1, 0, 1, (uchar*)iubuf, 1, కొలత(cn, 1), 0);
            పూర్ణాంకం minval = cv_విడత(getMinVal(sdepth)), maxval = cv_విడత(getMaxVal(sdepth));

            కొరకు( పూర్ణాంకం k = 0; k < cn; k++ )
            {
                ఒకవేళ( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lscalar = Mat(cn, 1, CV_32S, ilbuf);
            uscalar = Mat(cn, 1, CV_32S, iubuf);
        }

        lscalar.convert_గమ్యం(lscalar, stype);
        uscalar.convert_గమ్యం(uscalar, stype);
    }
    లేకపోతే
    {
        lscalaru = _lowerb.getUMat();
        uscalaru = _upperb.getUMat();
    }

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_కొలత(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, 1, నిలువుల_సంఖ్య_చొప్పున_WI);

    ఒకవేళ (haveScalar)
    {
        lscalar.copy_గమ్యం(lscalaru);
        uscalar.copy_గమ్యం(uscalaru);

        ker.వాదనలు(srcarg, dstarg, ocl::KernelArg::PtrReadOnly(lscalaru),
               ocl::KernelArg::PtrReadOnly(uscalaru), అడ్డువరుసలు_చొప్పున_WI);
    }
    లేకపోతే
        ker.వాదనలు(srcarg, dstarg, ocl::KernelArg::Read_Only_No_కొలత(lscalaru),
               ocl::KernelArg::Read_Only_No_కొలత(uscalaru), అడ్డువరుసలు_చొప్పున_WI);

    size_t globalsize[2] = { (size_t)ssize.వెడల్పు / నిలువుల_సంఖ్య_చొప్పున_WI, ((size_t)ssize.ఎత్తు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి ker.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

#అయితే_ముగింపు

}

శూన్య cv::లోపల_Range_2(ఆదానం_Array _src, ఆదానం_Array _lowerb,
                 ఆదానం_Array _upperb, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(! _src.empty());
    ఒకవేళ (!get_లోపల_Range_Func(_src.లోతు()))
        CV_Error( cv::Error::StsNotImplemented,
                 "inRange: CV_Bool and CV_8F inputs are not supported, convert the input to another type first");

    CV_OCL_నిరంతరం(_src.dims() <= 2 && _lowerb.dims() <= 2 &&
               _upperb.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat()),
               ocl_లోపల_Range(_src, _lowerb, _upperb, _dst))

    ఆదానం_Array_2::రకం_Flag skind = _src.రకం(), lkind = _lowerb.రకం(), ukind = _upperb.రకం();
    Mat src = _src.getMat(), lb = _lowerb.getMat(), ub = _upperb.getMat();

    తార్కిక lbScalar = అబద్ధం, ubScalar = అబద్ధం;

    ఒకవేళ( (lkind == ఆదానం_Array_2::MATX && skind != ఆదానం_Array_2::MATX) ||
        src.size != lb.size || src.type() != lb.type() )
    {
        ఒకవేళ( !checkScalar(lb, src.type(), lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = నిజం;
    }

    ఒకవేళ( (ukind == ఆదానం_Array_2::MATX && skind != ఆదానం_Array_2::MATX) ||
        src.size != ub.size || src.type() != ub.type() )
    {
        ఒకవేళ( !checkScalar(ub, src.type(), ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = నిజం;
    }

    CV_Assert(lbScalar == ubScalar);

    పూర్ణాంకం cn = src.channels(), లోతు = src.లోతు();

    size_t esz = src.elem_కొలత();
    size_t blocksize0 = (size_t)(ఖండం_కొలత + esz-1)/esz;

    _dst.create_Same_కొలత(_src, CV_8UC1);
    Mat dst = _dst.getMat();
    లోపల_Range_Func func = get_లోపల_Range_Func(లోతు);

    స్థిరరాశి Mat* arrays_sc[] = { &src, &dst, 0 };
    స్థిరరాశి Mat* arrays_nosc[] = { &src, &dst, &lb, &ub, 0 };
    uchar* ptrs[4] = {};

    NAryMatIterator it(lbScalar && ubScalar ? arrays_sc : arrays_nosc, ptrs);
    size_t మొత్తం = it.size, blocksize = std::min(మొత్తం, blocksize0);

    Auto_బఫరు<uchar> _buf(blocksize*(((పూర్ణాంకం)lbScalar + (పూర్ణాంకం)ubScalar)*esz + cn) + 2*cn*పరిమాణం(పూర్ణాంకం) + 128);
    uchar *buf = _buf.దత్తాంశం(), *mbuf = buf, *lbuf = 0, *ubuf = 0;
    buf = alignPtr(buf + blocksize*cn, 16);

    ఒకవేళ( lbScalar && ubScalar )
    {
        lbuf = buf;
        ubuf = buf = alignPtr(buf + blocksize*esz, 16);

        CV_Assert( lb.type() == ub.type() );
        పూర్ణాంకం scdepth = lb.లోతు();

        ఒకవేళ( scdepth != లోతు && లోతు < CV_32S )
        {
            పూర్ణాంకం* ilbuf = (పూర్ణాంకం*)alignPtr(buf + blocksize*esz, 16);
            పూర్ణాంకం* iubuf = ilbuf + cn;

            ద్విమాన_Func sccvtfunc = getConvertFunc(scdepth, CV_32S);
            sccvtfunc(lb.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, కొలత(cn, 1), 0);
            sccvtfunc(ub.ptr(), 1, 0, 1, (uchar*)iubuf, 1, కొలత(cn, 1), 0);
            పూర్ణాంకం minval = cv_విడత(getMinVal(లోతు)), maxval = cv_విడత(getMaxVal(లోతు));

            కొరకు( పూర్ణాంకం k = 0; k < cn; k++ )
            {
                ఒకవేళ( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lb = Mat(cn, 1, CV_32S, ilbuf);
            ub = Mat(cn, 1, CV_32S, iubuf);
        }

        convertAndUnrollScalar( lb, src.type(), lbuf, blocksize );
        convertAndUnrollScalar( ub, src.type(), ubuf, blocksize );

        ఒకవేళ (లోతు == CV_8U && src.dims <= 2) {
            uint8_t lb_scalar = lbuf[0];
            uint8_t ub_scalar = ubuf[0];
            CALL_HAL(లోపల_Range_u8, cv_hal_లోపల_Range8u, src.దత్తాంశం, src.అడుగు, dst.దత్తాంశం, dst.అడుగు, dst.లోతు(), src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు, src.channels(),
                lb_scalar, ub_scalar);
        } లేకపోతే ఒకవేళ (లోతు == CV_32F && src.dims <= 2) {
            ద్విగుణ lb_scalar = lb.ptr<ద్విగుణ>(0)[0];
            ద్విగుణ ub_scalar = ub.ptr<ద్విగుణ>(0)[0];
            CALL_HAL(లోపల_Range_f32, cv_hal_లోపల_Range32f, src.దత్తాంశం, src.అడుగు, dst.దత్తాంశం, dst.అడుగు, dst.లోతు(), src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు, src.channels(),
                lb_scalar, ub_scalar);
        }
    }

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        కొరకు( size_t j = 0; j < మొత్తం; j += blocksize )
        {
            పూర్ణాంకం bsz = (పూర్ణాంకం)MIN(మొత్తం - j, blocksize);
            size_t delta = bsz*esz;
            uchar *lptr = lbuf, *uptr = ubuf;
            ఒకవేళ( !lbScalar )
            {
                lptr = ptrs[2];
                ptrs[2] += delta;
            }
            ఒకవేళ( !ubScalar )
            {
                పూర్ణాంకం idx = !lbScalar ? 3 : 2;
                uptr = ptrs[idx];
                ptrs[idx] += delta;
            }
            func( ptrs[0], 0, lptr, 0, uptr, 0, cn == 1 ? ptrs[1] : mbuf, 0, కొలత(bsz*cn, 1));
            ఒకవేళ( cn > 1 )
                లోపల_Range_Reduce(mbuf, ptrs[1], bsz, cn);
            ptrs[0] += delta;
            ptrs[1] += bsz;
        }
    }
}
/* End of file. */
