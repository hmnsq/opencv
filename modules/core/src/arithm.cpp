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

#ضمن "precomp.hpp"
#ضمن "arithm_expr.hpp"   // the new element-wise engine (cv::ew)
#ضمن "opencl_kernels_core.hpp"

نطاق cv
{

/****************************************************************************************\
*                                   logical operations                                   *
\****************************************************************************************/

تعداد { OCL_OP_جمع_کن=0, OCL_OP_SUB=1, OCL_OP_RSUB=2, OCL_OP_ABSDIFF=3, OCL_OP_MUL=4,
       OCL_OP_MUL_SCALE=5, OCL_OP_DIV_SCALE=6, OCL_OP_RECIP_SCALE=7, OCL_OP_ADDW=8,
       OCL_OP_AND=9, OCL_OP_OR=10, OCL_OP_XOR=11, OCL_OP_NOT=12, OCL_OP_MIN=13, OCL_OP_MAX=14,
       OCL_OP_RDIV_SCALE=15 };

// The unified entry point for every element-wise binary op (empty + UMat/OpenCL + CPU engine). Defined
// lower down (near cv::add); forward-declared here so cv::min/cv::max (above it) can use it too.
ساكن فراغ arithm_op(ew::بالاترین op, درون_داد_Array src1, درون_داد_Array src2, OutputArray dst,
                      درون_داد_Array ماسک, صحيح dtype, صحيح oclop, منطقي muldiv, ثابت Scalar& params = Scalar(1));

// The unary counterpart (bitwise NOT, unary math later). Forward-declared for the same reason.
ساكن فراغ unary_op(ew::بالاترین op, درون_داد_Array src, OutputArray dst, درون_داد_Array ماسک,
                     صحيح dtype, ثابت Scalar& params = Scalar());

#اگر_تعریف_شده HAVE_OPENCL

ساكن ثابت حرف* oclop2str[] = { "OP_ADD", "OP_SUB", "OP_RSUB", "OP_ABSDIFF",
    "OP_MUL", "OP_MUL_SCALE", "OP_DIV_SCALE", "OP_RECIP_SCALE",
    "OP_ADDW", "OP_AND", "OP_OR", "OP_XOR", "OP_NOT", "OP_MIN", "OP_MAX", "OP_RDIV_SCALE", 0 };

// depths with no OpenCL type name (ocl::typeToStr() asserts on them)
ساكن مضمن منطقي noOclType(صحيح عمق)
{
    أرجع عمق == CV_16BF || عمق == CV_Bool || عمق == CV_8F_E4M3FN || عمق == CV_8F_E4M3FNUZ;
}

ساكن منطقي ocl_دودویی_op(درون_داد_Array _src1, درون_داد_Array _src2, OutputArray _dst,
                          درون_داد_Array ماسک_2, منطقي bitwise, صحيح oclop, منطقي haveScalar )
{
    منطقي have_ماسک = !ماسک_2.empty();
    صحيح srctype = _src1.type();
    صحيح srcdepth = CV_MAT_عمق(srctype);
    صحيح cn = CV_MAT_CN(srctype);

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقي doubleSupport = d.doubleFPConfig() > 0;
    إذا( oclop < 0 || ((have_ماسک || haveScalar) && cn > 4) ||
            (!doubleSupport && srcdepth == CV_64F && !bitwise) ||
            (noOclType(srcdepth) && !bitwise))
        أرجع خطأ;

    حرف opts[1024];
    صحيح kercn = have_ماسک || haveScalar ? cn : ocl::predict_Optimal_وکتور_عرض(_src1, _src2, _dst);
    صحيح scalarcn = kercn == 3 ? 4 : kercn;
    صحيح ردیفها_در_هر_WI = d.isIntel() ? 4 : 1;

    ثابت صحيح dst_عمق = srcdepth;
    ثابت صحيح dstType = CV_MAKETYPE(dst_عمق, kercn);
    ثابت صحيح dstType1 = CV_MAKETYPE(dst_عمق, 1);
    ثابت صحيح scalarType = CV_MAKETYPE(srcdepth, scalarcn);

    snprintf(opts, حجم(opts), "-D %s%s -D %s%s -D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workST=%s -D cn=%d -D rowsPerWI=%d",
            have_ماسک ? "MASK_" : "", haveScalar ? "UNARY_OP" : "BINARY_OP", oclop2str[oclop],
            doubleSupport ? " -D DOUBLE_SUPPORT" : "",
            bitwise ? ocl::memop_Type_مقصد_Str(dstType) : ocl::type_مقصد_Str(dstType),
            dst_عمق,
            bitwise ? ocl::memop_Type_مقصد_Str(dstType1) : ocl::type_مقصد_Str(dstType1),
            bitwise ? ocl::memop_Type_مقصد_Str(scalarType) : ocl::type_مقصد_Str(scalarType),
            kercn, ردیفها_در_هر_WI);

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), ماسک = ماسک_2.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_اندازه(src1, cn, kercn);
    ocl::KernelArg dstarg = have_ماسک ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_اندازه(ماسک, 1);

    إذا( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(srctype)*scalarcn;
        مزدوج buf[4] = {0,0,0,0};

        إذا( oclop != OCL_OP_NOT )
        {
            Mat src2sc = _src2.getMat();
            convertAndUnrollScalar(src2sc, srctype, (uchar*)buf, 1);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        إذا( !have_ماسک )
            k.آرگومانها(src1arg, dstarg, scalararg);
        وإلا
            k.آرگومانها(src1arg, maskarg, dstarg, scalararg);
    }
    وإلا
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::Read_Only_No_اندازه(src2, cn, kercn);

        إذا( !have_ماسک )
            k.آرگومانها(src1arg, src2arg, dstarg);
        وإلا
            k.آرگومانها(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.تعداد_ستون * cn / kercn, ((size_t)src1.ردیفها + ردیفها_در_هر_WI - 1) / ردیفها_در_هر_WI };
    أرجع k.پیاپی(2, globalsize, 0, خطأ);
}

#پایان_اگر

// OpenCL path for the bitwise ops (and/or/xor/not), lifted out of binary_op: choose the (possibly
// scalar) operand order, size the destination, and dispatch the shared bitwise OpenCL kernel. Returns
// true if OpenCL handled the op, false to fall through to the CPU engine. NOT arrives unary
// (src2 == src1, forced-scalar). The caller gates this on a UMat operand + dims <= 2 (via CV_OCL_RUN).
#اگر_تعریف_شده HAVE_OPENCL
ساكن منطقي bitwise_op_ocl(درون_داد_Array _src1, درون_داد_Array _src2, OutputArray _dst,
                           درون_داد_Array ماسک_2, صحيح oclop)
{
    ثابت درون_داد_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    درون_داد_Array_2::نوع_Flag kind1 = psrc1->نوع(), kind2 = psrc2->نوع();
    صحيح type1 = psrc1->type(), type2 = psrc2->type();
    منطقي haveScalar = خطأ;

    إذا( oclop == OCL_OP_NOT )
        haveScalar = درست;
    وإلا إذا( (kind1 == درون_داد_Array_2::MATX) + (kind2 == درون_داد_Array_2::MATX) == 1 ||
             !psrc1->same_اندازه(*psrc2) || type1 != type2 )
    {
        إذا( checkScalar(*psrc1, type2, kind1, kind2) )
        { std::swap(psrc1, psrc2); std::swap(type1, type2); }        // src1 is the scalar; swap it out
        وإلا إذا( !checkScalar(*psrc2, type1, kind2, kind1) )
            أرجع خطأ;
        haveScalar = درست;
    }
    _dst.create_Same_اندازه(*psrc1, type1);
    أرجع ocl_دودویی_op(*psrc1, *psrc2, _dst, ماسک_2, درست, oclop, haveScalar);
}
#پایان_اگر  // HAVE_OPENCL

}

// bitwise and/or/xor: OpenCL runs the shared bitwise kernel (via bitwise_op_ocl); the CPU path is the
// element-wise engine (OP_AND/OR/XOR, T x T -> T dispatched by element size). oclop = -1 tells
// arithm_op the OpenCL path was already tried, so it only runs the CPU engine. NOT is unary.
فراغ cv::bitwise_and(درون_داد_Array a, درون_داد_Array b, OutputArray c, درون_داد_Array ماسک)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_پیاپی((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, ماسک, OCL_OP_AND))
    arithm_op(ew::OP_AND, a, b, c, ماسک, -1, /*oclop=*/-1, /*muldiv=*/خطأ);
}

فراغ cv::bitwise_or(درون_داد_Array a, درون_داد_Array b, OutputArray c, درون_داد_Array ماسک)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_پیاپی((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, ماسک, OCL_OP_OR))
    arithm_op(ew::OP_OR, a, b, c, ماسک, -1, /*oclop=*/-1, /*muldiv=*/خطأ);
}

فراغ cv::bitwise_xor(درون_داد_Array a, درون_داد_Array b, OutputArray c, درون_داد_Array ماسک)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_پیاپی((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, ماسک, OCL_OP_XOR))
    arithm_op(ew::OP_XOR, a, b, c, ماسک, -1, /*oclop=*/-1, /*muldiv=*/خطأ);
}

فراغ cv::bitwise_not(درون_داد_Array a, OutputArray c, درون_داد_Array ماسک)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_پیاپی((a.isUMat() || c.isUMat()) && a.dims() <= 2,
               bitwise_op_ocl(a, a, c, ماسک, OCL_OP_NOT))
    unary_op(ew::OP_NOT, a, c, ماسک, -1);
}

فراغ cv::max( درون_داد_Array src1, درون_داد_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, خطأ);
}

فراغ cv::min( درون_داد_Array src1, درون_داد_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, خطأ);
}

// The concrete Mat/UMat overloads of min/max exist because C++ overload resolution needs them (a
// bare cv::min(mat, mat, mat) binds Mat& more tightly than InputArray); they must run the SAME engine
// path as the InputArray forms - NOT the legacy binary_op - so mixed types and broadcasting work.
فراغ cv::max(ثابت Mat& src1, ثابت Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, خطأ);
}

فراغ cv::min(ثابت Mat& src1, ثابت Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, خطأ);
}

فراغ cv::max(ثابت UMat& src1, ثابت UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, خطأ);
}

فراغ cv::min(ثابت UMat& src1, ثابت UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, خطأ);
}


/****************************************************************************************\
*                                      add/subtract                                      *
\****************************************************************************************/

نطاق cv
{

#اگر_تعریف_شده HAVE_OPENCL

// used by the OpenCL branch only (arithm_op_ocl working-type selection)
ساكن صحيح actual_Scalar_عمق(ثابت مزدوج* داده, صحيح len)
{
    صحيح i = 0, minval = بیشترین_صحیح, maxval = INT_MIN;
    لكل(; i < len; ++i)
    {
        صحيح ival = cv_دور(داده[i]);
        إذا( ival != داده[i] )
            اكسر;
        minval = MIN(minval, ival);
        maxval = MAX(maxval, ival);
    }
    أرجع i < len ? CV_64F :
        minval >= 0 && maxval <= (صحيح)UCHAR_MAX ? CV_8U :
        minval >= (صحيح)SCHAR_MIN && maxval <= (صحيح)SCHAR_MAX ? CV_8S :
        minval >= 0 && maxval <= (صحيح)USHRT_MAX ? CV_16U :
        minval >= (صحيح)SHRT_MIN && maxval <= (صحيح)SHRT_MAX ? CV_16S :
        CV_32S;
}

ساكن صحيح coerceTypes(صحيح depth1, صحيح depth2, منطقي muldiv)
{
    أرجع depth1 == depth2 ? depth1 :
        ((depth1 <= CV_32S) & (depth2 <= CV_32S)) != 0 ?
        (((صحيح)!muldiv & (depth1 <= CV_8S) & (depth2 <= CV_8S)) != 0 ? CV_16S : CV_32S) :
        ((CV_ELEM_SIZE1(depth1) > 4) | (CV_ELEM_SIZE1(depth2) > 4)) != 0 ? CV_64F : CV_32F;
}

ساكن منطقي ocl_arithm_op(درون_داد_Array _src1, درون_داد_Array _src2, OutputArray _dst,
                          درون_داد_Array ماسک_2, صحيح wtype,
                          فراغ* usrdata, صحيح oclop,
                          منطقي haveScalar )
{
    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقي doubleSupport = d.doubleFPConfig() > 0;
    صحيح type1 = _src1.type(), depth1 = CV_MAT_عمق(type1), cn = CV_MAT_CN(type1);
    منطقي have_ماسک = !ماسک_2.empty();

    إذا ( (have_ماسک || haveScalar) && cn > 4 )
        أرجع خطأ;

#اگر_تعریف_شده __APPLE__
    إذا (depth1 == CV_16U && (oclop == OCL_OP_MUL || oclop == OCL_OP_MUL_SCALE))
        أرجع خطأ;
#پایان_اگر

    صحيح dtype = _dst.type(), ddepth = CV_MAT_عمق(dtype), wdepth = std::max(CV_32S, CV_MAT_عمق(wtype));
    إذا (!doubleSupport)
        wdepth = std::min(wdepth, CV_32F);

    wtype = CV_MAKETYPE(wdepth, cn);
    صحيح type2 = haveScalar ? wtype : _src2.type(), depth2 = CV_MAT_عمق(type2);
    إذا (!doubleSupport && (depth2 == CV_64F || depth1 == CV_64F))
        أرجع خطأ;

    إذا (noOclType(depth1) || noOclType(depth2) || noOclType(ddepth) || noOclType(wdepth))
        أرجع خطأ;

    صحيح kercn = have_ماسک || haveScalar ? cn : ocl::predict_Optimal_وکتور_عرض(_src1, _src2, _dst);
    صحيح scalarcn = kercn == 3 ? 4 : kercn, ردیفها_در_هر_WI = d.isIntel() ? 4 : 1;

    حرف cvtstr[4][50], opts[1024];
    snprintf(opts, حجم(opts), "-D %s%s -D %s -D srcT1=%s -D srcT1_C1=%s -D srcT2=%s -D srcT2_C1=%s "
            "-D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workT=%s -D workST=%s -D scaleT=%s -D wdepth=%d -D convertToWT1=%s "
            "-D convertToWT2=%s -D convertToDT=%s%s -D cn=%d -D rowsPerWI=%d -D convertFromU=%s",
            (have_ماسک ? "MASK_" : ""), (haveScalar ? "UNARY_OP" : "BINARY_OP"),
            oclop2str[oclop], ocl::type_مقصد_Str(CV_MAKETYPE(depth1, kercn)),
            ocl::type_مقصد_Str(depth1), ocl::type_مقصد_Str(CV_MAKETYPE(depth2, kercn)),
            ocl::type_مقصد_Str(depth2), ocl::type_مقصد_Str(CV_MAKETYPE(ddepth, kercn)), ddepth,
            ocl::type_مقصد_Str(ddepth), ocl::type_مقصد_Str(CV_MAKETYPE(wdepth, kercn)),
            ocl::type_مقصد_Str(CV_MAKETYPE(wdepth, scalarcn)),
            ocl::type_مقصد_Str(wdepth), wdepth,
            ocl::convertTypeStr(depth1, wdepth, kercn, cvtstr[0], حجم(cvtstr[0])),
            ocl::convertTypeStr(depth2, wdepth, kercn, cvtstr[1], حجم(cvtstr[1])),
            ocl::convertTypeStr(wdepth, ddepth, kercn, cvtstr[2], حجم(cvtstr[2])),
            doubleSupport ? " -D DOUBLE_SUPPORT" : "", kercn, ردیفها_در_هر_WI,
            oclop == OCL_OP_ABSDIFF && wdepth == CV_32S && ddepth == wdepth ?
            ocl::convertTypeStr(CV_8U, ddepth, kercn, cvtstr[3], حجم(cvtstr[3])) : "noconvert");

    size_t usrdata_esz = CV_ELEM_اندازه(wdepth);
    ثابت uchar* usrdata_p = (ثابت uchar*)usrdata;
    ثابت مزدوج* usrdata_d = (ثابت مزدوج*)usrdata;
    عائم usrdata_f[3];
    صحيح i, n = oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE ||
        oclop == OCL_OP_RDIV_SCALE || oclop == OCL_OP_RECIP_SCALE ? 1 : oclop == OCL_OP_ADDW ? 3 : 0;
    إذا( usrdata && n > 0 && wdepth == CV_32F )
    {
        لكل( i = 0; i < n; i++ )
            usrdata_f[i] = (عائم)usrdata_d[i];
        usrdata_p = (ثابت uchar*)usrdata_f;
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), ماسک = ماسک_2.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_اندازه(src1, cn, kercn);
    ocl::KernelArg dstarg = have_ماسک ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_اندازه(ماسک, 1);

    إذا( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(wtype)*scalarcn;
        مزدوج buf[4]={0,0,0,0};
        Mat src2sc = _src2.getMat();

        إذا( !src2sc.empty() )
            convertAndUnrollScalar(src2sc, wtype, (uchar*)buf, 1);
        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        إذا( !have_ماسک )
        {
            إذا(n == 0)
                k.آرگومانها(src1arg, dstarg, scalararg);
            وإلا إذا(n == 1)
                k.آرگومانها(src1arg, dstarg, scalararg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            وإلا
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        وإلا
            k.آرگومانها(src1arg, maskarg, dstarg, scalararg);
    }
    وإلا
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::Read_Only_No_اندازه(src2, cn, kercn);

        إذا( !have_ماسک )
        {
            إذا (n == 0)
                k.آرگومانها(src1arg, src2arg, dstarg);
            وإلا إذا (n == 1)
                k.آرگومانها(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            وإلا إذا (n == 3)
                k.آرگومانها(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz*2, usrdata_esz));
            وإلا
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        وإلا
            k.آرگومانها(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.تعداد_ستون * cn / kercn, ((size_t)src1.ردیفها + ردیفها_در_هر_WI - 1) / ردیفها_در_هر_WI };
    أرجع k.پیاپی(2, globalsize, عدم, خطأ);
}

#پایان_اگر

// OpenCL branch of the arithmetic ops, extracted from the former arithm_op (whose CPU compute is now
// the element-wise engine). Returns true if an OpenCL kernel handled the op, false to let the caller
// fall through to the CPU engine. min/max use the bitwise-style kernel; every other op the arithm
// kernel with a coerced working type. `usrdata` carries the mul/div/addWeighted scale block, `oclop`
// the kernel id. Only reached for a UMat dst; sizes/types that don't fit the scalar/same-size pattern
// (e.g. a broadcast) decline (return false) so the engine handles them on the CPU.
ساكن منطقي arithm_op_ocl(درون_داد_Array _src1, درون_داد_Array _src2, OutputArray _dst,
                          درون_داد_Array ماسک_2, صحيح dtype, صحيح oclop, منطقي muldiv, فراغ* usrdata)
{
#اگر_تعریف_نشده HAVE_OPENCL
    CV_استفادهنشده(_src1); CV_استفادهنشده(_src2); CV_استفادهنشده(_dst); CV_استفادهنشده(ماسک_2);
    CV_استفادهنشده(dtype); CV_استفادهنشده(oclop); CV_استفادهنشده(muldiv); CV_استفادهنشده(usrdata);
    أرجع خطأ;
#وإلا
    ثابت درون_داد_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    درون_داد_Array_2::نوع_Flag kind1 = psrc1->نوع(), kind2 = psrc2->نوع();
    منطقي have_ماسک = !ماسک_2.empty();
    منطقي reallocate = خطأ;
    صحيح type1 = psrc1->type(), depth1 = CV_MAT_عمق(type1), cn = CV_MAT_CN(type1);
    صحيح type2 = psrc2->type(), depth2 = CV_MAT_عمق(type2), cn2 = CV_MAT_CN(type2);
    صحيح wtype, dims1 = psrc1->dims(), dims2 = psrc2->dims();
    اندازه sz1 = dims1 <= 2 ? psrc1->size() : اندازه();
    اندازه sz2 = dims2 <= 2 ? psrc2->size() : اندازه();

    إذا (!(OCL_PERFORMANCE_CHECK(_dst.isUMat()) && dims1 <= 2 && dims2 <= 2))
        أرجع خطأ;

    // min/max: the bitwise-style OpenCL kernel (same-type element-wise, no working-type coercion).
    إذا (oclop == OCL_OP_MIN || oclop == OCL_OP_MAX)
    {
        منطقي haveScalar = خطأ;
        إذا ((kind1 == درون_داد_Array_2::MATX) + (kind2 == درون_داد_Array_2::MATX) == 1 ||
            !psrc1->same_اندازه(*psrc2) || type1 != type2)
        {
            إذا (checkScalar(*psrc1, type2, kind1, kind2))
            { std::swap(psrc1, psrc2); std::swap(type1, type2); }
            وإلا إذا (!checkScalar(*psrc2, type1, kind2, kind1))
                أرجع خطأ;
            haveScalar = درست;
        }
        _dst.create_Same_اندازه(*psrc1, type1);
        أرجع ocl_دودویی_op(*psrc1, *psrc2, _dst, ماسک_2, خطأ, oclop, haveScalar);
    }

    // reciprocal (scale/src): the OpenCL kernel is unary on src - use src2 for both operands (the CPU
    // engine received a 0-dim `1` numerator as src1, which the recip kernel would ignore). [TODO.VP: tidy]
    إذا (oclop == OCL_OP_RECIP_SCALE)
    {
        psrc1 = psrc2; kind1 = kind2; type1 = type2; depth1 = depth2; cn = cn2; dims1 = dims2; sz1 = sz2;
    }

    // add/sub/mul/div/absdiff/addWeighted/recip: the arithm OpenCL kernel with a coerced work type.
    منطقي src1Scalar = checkScalar(*psrc1, type2, kind1, kind2);
    منطقي src2Scalar = checkScalar(*psrc2, type1, kind2, kind1);
    منطقي haveScalar = خطأ;

    إذا (dims1 != dims2 || sz1 != sz2 || cn != cn2 ||
        (kind1 == درون_داد_Array_2::MATX && (sz1 == اندازه(1,4) || sz1 == اندازه(1,1))) ||
        (kind2 == درون_داد_Array_2::MATX && (sz2 == اندازه(1,4) || sz2 == اندازه(1,1))))
    {
        إذا ((type1 == CV_64F && (sz1.ارتفاع == 1 || sz1.ارتفاع == 4)) && src1Scalar)
        {
            // src1 is a scalar; swap it with src2
            std::swap(psrc1, psrc2); std::swap(sz1, sz2); std::swap(type1, type2);
            std::swap(depth1, depth2); std::swap(cn, cn2); std::swap(dims1, dims2);
            إذا (oclop == OCL_OP_SUB) oclop = OCL_OP_RSUB;
            إذا (oclop == OCL_OP_DIV_SCALE) oclop = OCL_OP_RDIV_SCALE;
        }
        وإلا إذا (!src2Scalar)
            أرجع خطأ;   // array op array with mismatched size/cn: engine broadcasts on the CPU
        haveScalar = درست;
        CV_Assert((type2 == CV_64F || type2 == CV_32F) && (sz2.ارتفاع == 1 || sz2.ارتفاع == 4));

        إذا (!muldiv)
        {
            Mat sc = psrc2->getMat();
            depth2 = actual_Scalar_عمق(sc.ptr<مزدوج>(), sz2 == اندازه(1, 1) ? cn2 : cn);
            إذا (depth2 == CV_64F && CV_ELEM_SIZE1(depth1) < 8)
                depth2 = CV_32F;
        }
        وإلا
            depth2 = CV_64F;
    }

    إذا (dtype < 0)
    {
        إذا (_dst.fixedType())
            dtype = _dst.type();
        وإلا
        {
            إذا (!haveScalar && type1 != type2)
                CV_Error(cv::Error::StsBadArg,
                     "When the input arrays in add/subtract/multiply/divide functions have different types, "
                     "the output array type must be explicitly specified");
            dtype = type1;
        }
    }
    dtype = CV_MAT_عمق(dtype);

    إذا (depth1 == depth2 && dtype == depth1)
        wtype = dtype;
    وإلا إذا (!muldiv)
    {
        wtype = coerceTypes(depth1, depth2, خطأ);
        wtype = coerceTypes(wtype, dtype, خطأ);
        إذا (dtype < CV_32F && (depth1 < CV_32F || depth2 < CV_32F))
            wtype = CV_32S;
    }
    وإلا
    {
        wtype = coerceTypes(depth1, depth2, درست);
        wtype = coerceTypes(wtype, dtype, درست);
    }

    // The scaled OpenCL kernels (mul/div by a scale, reciprocal, addWeighted) compute in float; the old
    // same-type fast path forced max(depth, CV_32F). coerceTypes keeps same-type integer, so bump here.
    إذا (oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE || oclop == OCL_OP_RDIV_SCALE ||
        oclop == OCL_OP_RECIP_SCALE || oclop == OCL_OP_ADDW)
        wtype = std::max(wtype, (صحيح)CV_32F);

    dtype = CV_MAKETYPE(dtype, cn);
    wtype = CV_MAKETYPE(wtype, cn);

    إذا (have_ماسک)
    {
        صحيح mtype = ماسک_2.type();
        CV_Assert((mtype == CV_8UC1 || mtype == CV_8SC1 || mtype == CV_Bool) && ماسک_2.same_اندازه(*psrc1));
        reallocate = !_dst.same_اندازه(*psrc1) || _dst.type() != dtype;
    }

    _dst.create_Same_اندازه(*psrc1, dtype);
    إذا (reallocate)
        _dst.مجموعه_مقصد(0.);

    أرجع ocl_arithm_op(*psrc1, *psrc2, _dst, ماسک_2, wtype, usrdata, oclop, haveScalar);
#پایان_اگر
}

// Element-wise binary op (ADD/SUB/...) on two CPU operands - each an array Mat or a MATX scalar -
// through the new broadcasting engine. Handles mixed input types (emitBinary inserts the casts) and
// broadcasting (a scalar rides as a 0-dim per-channel CONST; arrays broadcast via outputShape/exec).
// Returns false (declining) for UMat, a write-mask, or two scalar operands, so the caller falls
// through to arithm_op. Incompatible shapes make the engine throw (matching cv::add's error).
// Does the output array already have exactly this shape+type (=> create() reuses it, no realloc)?
// Works for Mat and UMat alike (shape/type queries, no data-pointer peeking).
ساكن منطقي بیرون_Array_Matches(ثابت _OutputArray& a, ثابت MatShape& shp, صحيح type)
{
    إذا (a.empty() || a.type() != type)
        أرجع خطأ;
    صحيح sz[MatShape::MAX_DIMS];
    صحيح nd = a.sizend(sz);
    إذا (nd != (صحيح)shp.size())
        أرجع خطأ;
    لكل (صحيح i = 0; i < nd; i++)
        إذا (sz[i] != shp[i])
            أرجع خطأ;
    أرجع درست;
}

// Are two operands' iteration shapes (spatial dims + channels innermost, exactly how the engine
// iterates) numpy-broadcast-compatible? Powers the scalar-like-Mat compat fallback in arithm_op.
// Cheap by design: sizend copies the dim arrays inline - no Mat headers, no allocations.
ساكن منطقي shapes_پخش_Compat(درون_داد_Array a, صحيح acn, درون_داد_Array b, صحيح bcn)
{
    صحيح asz[MatShape::MAX_DIMS + 1], bsz[MatShape::MAX_DIMS + 1];
    صحيح ad = a.sizend(asz), bd = b.sizend(bsz);
    asz[ad++] = acn;
    bsz[bd++] = bcn;
    لكل (صحيح i1 = ad - 1, i2 = bd - 1; i1 >= 0 && i2 >= 0; i1--, i2--)
        إذا (asz[i1] != bsz[i2] && asz[i1] != 1 && bsz[i2] != 1)
            أرجع خطأ;
    أرجع درست;
}

// The one entry point for every element-wise binary op (add/sub/mul/div/min/max/absdiff/addWeighted/
// reciprocal). Handles empty inputs, the UMat/OpenCL branch (arithm_op_ocl) and the CPU element-wise
// engine. `oclop` selects the OpenCL kernel; `muldiv` drives the OpenCL working-type coercion + scale;
// `params` are the op scalars (params[0]=mul/div scale; {alpha,beta,gamma} for addWeighted).
// Below this many ELEMENTS (total * channels), the per-call constant overhead of the engine path
// (program build + compile + executor setup) is comparable to the actual work - take the direct
// kernel call instead. Same tier boundary as math_op's MATH_OP_SMALL for cv::exp.
تعداد { ARITHM_SMALL_مستقیم = 100000 };

ساكن فراغ arithm_op(ew::بالاترین op, درون_داد_Array src1, درون_داد_Array src2, OutputArray dst,
                      درون_داد_Array ماسک, صحيح dtype, صحيح oclop, منطقي muldiv, ثابت Scalar& params)
{
    إذا (src1.empty() || src2.empty())          // empty inputs -> empty result
    {
        CV_Assert((src1.empty() && src2.empty()) ||
                  (src1.empty() && isScalarArg(src2, src1.channels())) ||
                  (src2.empty() && isScalarArg(src1, src2.channels())));
        dst.release();
        إذا (dtype >= 0)
            dst.create(0, 0, dtype);
        أرجع;
    }

    // UMat -> classic OpenCL kernel. It declines (false) when OpenCL can't apply (no device, dims>2, a
    // broadcast, cn>4 masked, ...) - then the CPU engine below handles it (mapping the UMat via getMat).
    // oclop < 0 is a sentinel: the caller (e.g. cv::compare) already ran its own OpenCL path, so this
    // helper only runs the CPU engine (a UMat operand is mapped via getMat).
    إذا (oclop >= 0 && cv::ocl::isOpenCLActivated() &&
        (src1.isUMat() || src2.isUMat() || dst.isUMat()))
    {
        مزدوج abg[3] = { params[0], params[1], params[2] }, scale = params[0];
        فراغ* usrdata = (op == ew::OP_ADDW) ? (فراغ*)abg : (muldiv ? (فراغ*)&scale : اشاره_تهی);
        إذا (arithm_op_ocl(src1, src2, dst, ماسک, dtype, oclop, muldiv, usrdata))
            أرجع;
    }

    ثابت منطقي have_ماسک = !ماسک.empty();
    ثابت صحيح cn1 = src1.channels(), cn2 = src2.channels();
    منطقي s1 = isScalarArg(src1, src2), s2 = isScalarArg(src2, src1);
    إذا (s1 && s2)
        s1 = s2 = خطأ;    // two scalars: treat both as tiny array operands (element-wise + broadcast),
                            // matching arithm_op (Scalar+Scalar -> 4x1, number+number -> 1x1, ...)
    // A 4x1 CV_64F Mat/UMat pseudo-scalar (see isScalarArg) is hijacked only against a REAL array:
    // when the partner is itself a tiny scalar-shaped array (e.g. compare(Mat 4x1, Mat 1x1) -
    // issue #8999), both are honest data and ride the broadcast. A genuine MATX Scalar never demotes.
    وإلا إذا (s1 && src1.نوع() != درون_داد_Array_2::MATX && isScalarLikeMat(src2, cn1))
        s1 = خطأ;
    وإلا إذا (s2 && src2.نوع() != درون_داد_Array_2::MATX && isScalarLikeMat(src1, cn2))
        s2 = خطأ;

    // Compat fallback: Java/Python/user code passes scalars as real little Mats (the classic 4x1
    // CV_64F column, a 1xcn/cnx1 vector - see isScalarLikeMat). Those normally ride broadcasting
    // now, so hijack one as a per-channel scalar ONLY when the shapes do NOT broadcast - just the
    // calls that would otherwise throw get the 4.x scalar semantics, every valid broadcast keeps
    // its numpy meaning. Cheap: two geometry probes, and the O(ndims) shape walk runs only when a
    // probe hits (never for ordinary same-size or MATX-scalar calls).
    إذا (!s1 && !s2)
    {
        منطقي like1 = isScalarLikeMat(src1, cn2), like2 = isScalarLikeMat(src2, cn1);
        إذا ((like1 || like2) && !shapes_پخش_Compat(src1, cn1, src2, cn2))
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
    ثابت فراغ* dstObj = (dst.نوع() == درون_داد_Array_2::MAT) ? dst.getObj() : اشاره_تهی;
    ثابت منطقي aliased = dstObj && (dstObj == src1.getObj() || dstObj == src2.getObj());
    Mat m1loc, m2loc, mloc;
    خودکار asMat = [&](درون_داد_Array a, Mat& loc) -> ثابت Mat* {
        إذا (a.نوع() == درون_داد_Array_2::MAT && !aliased) أرجع (ثابت Mat*)a.getObj();
        loc = a.getMat(); أرجع &loc;
    };
    ثابت Mat* pm1 = s1 ? اشاره_تهی : asMat(src1, m1loc);
    ثابت Mat* pm2 = s2 ? اشاره_تهی : asMat(src2, m2loc);
    ثابت صحيح adepth = s1 ? pm2->عمق() : pm1->عمق();   // the (first) array operand's depth
    // auto result depth (no explicit dtype, no fixed-type dst): add/sub/mul/div keep the first array
    // operand's depth (cv::'s dtype==-1 convention), but min/max/absdiff have NO dtype argument and
    // must promote mixed inputs to their common type (min(u32,f32) in u32 would drop the float). For
    // same-type inputs promoteArith(da,db)==da, so classic behaviour is preserved (e.g. absdiff of two
    // s16 stays s16, saturating - the wide unsigned |a-b| is computed then cast back down by emitBinary).
    صحيح auto_عمق = adepth;
    إذا (op == ew::OP_MIN || op == ew::OP_MAX || op == ew::OP_ABSDIFF)
    {
        ثابت صحيح d1 = s1 ? ew::EW_عمق_NONE : pm1->عمق();
        ثابت صحيح d2 = s2 ? ew::EW_عمق_NONE : pm2->عمق();
        auto_عمق = ew::promoteArith(d1, d2);
    }
    // result depth: explicit dtype wins; else a fixed-type dst dictates it; else the auto depth above.
    ثابت صحيح rdepth = dtype >= 0 ? CV_MAT_عمق(dtype)
                     : (dst.fixedType() ? dst.عمق() : auto_عمق);

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
    إذا (!have_ماسک && !s1 && !s2 &&
        (op == ew::OP_جمع_کن || op == ew::OP_SUB || op == ew::OP_MIN || op == ew::OP_MAX ||
         op == ew::OP_ABSDIFF || op == ew::OP_MUL || op == ew::OP_ADDW ||
         op == ew::OP_AND || op == ew::OP_OR || op == ew::OP_XOR) &&
        rdepth == pm1->عمق() && pm1->type() == pm2->type() && pm1->size == pm2->size &&
        pm1->isContinuous() && pm2->isContinuous() &&
        pm1->مجموع()*cn1 <= (size_t)ARITHM_SMALL_مستقیم)
    {
        ew::TKernel k = ew::getElemwiseFunc(op, rdepth, rdepth, ew::EW_عمق_NONE, rdepth);
        إذا (k.fptr)
        {
            dst.create_Same_اندازه(src1, pm1->type());   // whole-shape transfer, not piecemeal dims+sizes
            Mat dloc;
            Mat* pd = (dst.نوع() == درون_داد_Array_2::MAT) ? (Mat*)dst.getObj() : &(dloc = dst.getMat());
            إذا (pd->isContinuous())     // a reused non-continuous dst view falls through to the engine
            {
                k.fptr(pm1->داده, 0, 1, pm2->داده, 0, 1, اشاره_تهی, 0, 0,
                       pd->داده, 0, (صحيح)(pm1->مجموع()*cn1), 1, params.val, k.flags, k.userdata);
                أرجع;
            }
        }
    }

    // operand order = (src1, src2): an array becomes an INPUT, a scalar a flexible per-channel CONST
    // read straight from the caller's inline storage (getObj()), no Mat / convertTo.
    ew::TExpr p;
    خودکار جمع_کن_عملوند = [&](درون_داد_Array src, منطقي isScalar, ثابت Mat* m, صحيح otherCn) -> صحيح {
        إذا (!isScalar)
            أرجع p.جمع_کن_درون_داد(m->عمق());
        ثابت uchar* sp; صحيح sd;
        uchar scbuf[EW_SCALAR_BUF_اندازه];   // stack room for a UMAT scalar; addConst copies the values out
        ثابت صحيح scn0 = scalarArgElems(src, sp, sd, scbuf), cn = otherCn;
        ثابت صحيح scn = (cn == scn0 || (cn < 4 && scn0 == 4)) ? cn
                      : (scn0 == 1) ? 1 : 0;
        CV_Assert(scn > 0);
        أرجع p.جمع_کن_Const(ew::EW_عمق_NONE, sd, sp, scn);
    };
    ثابت صحيح a0 = جمع_کن_عملوند(src1, s1, pm1, cn2);
    ثابت صحيح a1 = جمع_کن_عملوند(src2, s2, pm2, cn1);

    // Write-mask: a single-channel 1-byte array (u8/s8/bool) the size of the output spatial shape,
    // added as another broadcast input. The arithmetic result lands in a temp; a final
    // select(mask, r, dst) -> dst overwrites only mask!=0 positions of the (pre-existing) output.
    صحيح s_ماسک = 0;
    ثابت Mat* pmask = اشاره_تهی;
    إذا (have_ماسک)
    {
        CV_Assert(ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool);
        pmask = asMat(ماسک, mloc);
        s_ماسک = p.جمع_کن_درون_داد(pmask->عمق());
    }
    ثابت صحيح s_بیرون = p.جمع_کن_Output(rdepth);               // output BEFORE emit -> moveToOutput drops the
    ثابت صحيح r = p.emit_دودویی(op, a0, a1, rdepth, params);   // result temp (mask-free => none); scale
                                                        // rides params[0] (mul/div), {a,b,g} for addW
    إذا (have_ماسک)
        p.جمع_کن_Insn(ew::OP_SELECT, s_ماسک, r, s_بیرون, s_بیرون);
    وإلا
        p.move_مقصد_Output(r, s_بیرون);
    p.compile();

    ثابت Mat* inputs[3]; صحيح ni = 0;                   // inputs in program (input-index) order
    إذا (!s1) inputs[ni++] = pm1;
    إذا (!s2) inputs[ni++] = pm2;
    إذا (have_ماسک) inputs[ni++] = pmask;

    // Pre-create the output at the broadcast shape, ALWAYS via dst.create() - never hand exec a raw
    // dst Mat. Routing through the _OutputArray is what enforces its contract: a FIXED_SIZE/FIXED_TYPE
    // dst (in-place 'a += b' passes '(const Mat&)a') throws here if a broadcast would change its shape
    // or type (Mat(1x1) += Mat(1x4)), instead of silently reallocating - exec sees only a Mat and has
    // no view of the array's fixed flags. Cost over a direct exec is negligible: exec keeps its own
    // fast path for a matching-shape op, and dst.create() with an unchanged shape/type is a no-op.
    // With a mask, a freshly (re)allocated dst is zeroed so mask==0 reads 0 (matches arithm_op); a
    // reused dst keeps its prior content there - detect reuse by shape+type BEFORE create (correct for
    // a reused UMat dst too).
    MatShape oshape; صحيح ocn;
    p.outputShape(inputs, oshape, ocn);
    ثابت صحيح otype = CV_MAKETYPE(rdepth, ocn);
    ثابت منطقي reused = have_ماسک && بیرون_Array_Matches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.نوع() == درون_داد_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    إذا (have_ماسک && !reused)
        pdst->مجموعه_Zero();                          // setZero (not setTo, which caps at 4 channels)
    p.exec(inputs, pdst);
}

// Unary element-wise op through the engine (bitwise NOT now; unary math later). One array input, an
// optional write-mask; result depth = dtype (>=0), else a fixed-type dst's depth, else the input
// depth. Mirrors arithm_op's build/mask/exec tail with a single input and no scalar operand. OpenCL is
// run by the caller (bitwise_op_ocl), so this is the CPU engine only.
ساكن فراغ unary_op(ew::بالاترین op, درون_داد_Array src, OutputArray dst, درون_داد_Array ماسک,
                     صحيح dtype, ثابت Scalar& params)
{
    إذا (src.empty())
    {
        dst.release();
        إذا (dtype >= 0)
            dst.create(0, 0, dtype);
        أرجع;
    }

    ثابت منطقي have_ماسک = !ماسک.empty();
    // in-place (dst aliases src): keep a header copy so a shape/type-changing dst.create() can't free
    // src's data mid-op (matches arithm_op). A fixed-size/type dst never reallocs, so this is only for
    // an explicit dtype change.
    ثابت فراغ* dstObj = (dst.نوع() == درون_داد_Array_2::MAT) ? dst.getObj() : اشاره_تهی;
    ثابت منطقي aliased = dstObj && src.نوع() == درون_داد_Array_2::MAT && dstObj == src.getObj();
    Mat sloc, mloc;
    ثابت Mat* psrc = (src.نوع() == درون_داد_Array_2::MAT && !aliased) ? (ثابت Mat*)src.getObj()
                                                                   : &(sloc = src.getMat());
    ثابت صحيح rdepth = dtype >= 0 ? CV_MAT_عمق(dtype)
                     : (dst.fixedType() ? dst.عمق() : psrc->عمق());

    ew::TExpr p;
    ثابت صحيح a0 = p.جمع_کن_درون_داد(psrc->عمق());
    صحيح s_ماسک = 0;
    ثابت Mat* pmask = اشاره_تهی;
    إذا (have_ماسک)
    {
        CV_Assert(ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool);
        pmask = (ماسک.نوع() == درون_داد_Array_2::MAT) ? (ثابت Mat*)ماسک.getObj() : &(mloc = ماسک.getMat());
        s_ماسک = p.جمع_کن_درون_داد(pmask->عمق());
    }
    ثابت صحيح s_بیرون = p.جمع_کن_Output(rdepth);
    ثابت صحيح r = p.emitUnary(op, a0, rdepth, params);
    إذا (have_ماسک)
        p.جمع_کن_Insn(ew::OP_SELECT, s_ماسک, r, s_بیرون, s_بیرون);
    وإلا
        p.move_مقصد_Output(r, s_بیرون);
    p.compile();

    ثابت Mat* inputs[2]; صحيح ni = 0;
    inputs[ni++] = psrc;
    إذا (have_ماسک) inputs[ni++] = pmask;

    MatShape oshape; صحيح ocn;
    p.outputShape(inputs, oshape, ocn);
    ثابت صحيح otype = CV_MAKETYPE(rdepth, ocn);
    ثابت منطقي reused = have_ماسک && بیرون_Array_Matches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.نوع() == درون_داد_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    إذا (have_ماسک && !reused)
        pdst->مجموعه_Zero();
    p.exec(inputs, pdst);
}

}

فراغ cv::جمع_کن( درون_داد_Array src1, درون_داد_Array src2, OutputArray dst,
          درون_داد_Array ماسک, صحيح dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_جمع_کن, src1, src2, dst, ماسک, dtype, OCL_OP_جمع_کن, خطأ);
}

فراغ cv::تفریق( درون_داد_Array src1, درون_داد_Array src2, OutputArray dst,
                   درون_داد_Array ماسک, صحيح dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_SUB, src1, src2, dst, ماسک, dtype, OCL_OP_SUB, خطأ);
}

فراغ cv::absdiff( درون_داد_Array src1, درون_داد_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ABSDIFF, src1, src2, dst, noArray(), -1, OCL_OP_ABSDIFF, خطأ);
}

فراغ cv::copy_مقصد(درون_داد_Array _src, OutputArray _dst, درون_داد_Array ماسک_2)
{
    CV_INSTRUMENT_REGION();

    _src.copy_مقصد(_dst, ماسک_2);
}

/****************************************************************************************\
*                                    multiply/divide                                     *
\****************************************************************************************/

نطاق cv
{

فراغ multiply(درون_داد_Array src1, درون_داد_Array src2,
              OutputArray dst, مزدوج scale, صحيح dtype)
{
    CV_INSTRUMENT_REGION();

    ثابت صحيح oclop = std::قدر_مطلق(scale - 1.0) < DBL_EPSILON ? OCL_OP_MUL : OCL_OP_MUL_SCALE;
    arithm_op(ew::OP_MUL, src1, src2, dst, noArray(), dtype, oclop, /*muldiv*/ درست, Scalar(scale));
}

فراغ divide(درون_داد_Array src1, درون_داد_Array src2,
                OutputArray dst, مزدوج scale, صحيح dtype)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_DIV, src1, src2, dst, noArray(), dtype, OCL_OP_DIV_SCALE, /*muldiv*/ درست, Scalar(scale));
}

فراغ divide(مزدوج scale, درون_داد_Array src2,
                OutputArray dst, صحيح dtype)
{
    CV_INSTRUMENT_REGION();

    إذا (src2.empty())
    {
        dst.release();
        أرجع;
    }

    // scale / src2 == scale * 1 / src2: feed a 0-dim `1` as the numerator so the CPU engine reuses the
    // normal divide (scale*a/b). Give the numerator src2's OWN depth: then OP_DIV(T, T) is same-type and
    // takes the fast per-type kernel (f32 work for <=16-bit / f16 / bf16, not the s32->f64 path a CV_32S
    // numerator would force). An integer src2 keeps its div-by-zero->0 guard (both operands integer); a
    // float src2 divides as float (1/0->inf), both matching cv::divide. dtype<0 follows src2's depth (a
    // fixed-type dst still wins). The UMat path uses the dedicated reciprocal kernel (OCL_OP_RECIP_SCALE).
    مزدوج one = 1;
    Mat numerator(MatShape::scalar(), src2.عمق(), &one);
    scalar_مقصد_Raw_داده(Scalar(1), &one, src2.عمق(), 1);       // write `1` in src2's depth (no allocation)
    ثابت صحيح rtype = (dtype < 0 && !dst.fixedType()) ? src2.عمق() : dtype;
    arithm_op(ew::OP_DIV, numerator, src2, dst, noArray(), rtype, OCL_OP_RECIP_SCALE, /*muldiv*/ درست, Scalar(scale));
}

UMat UMat::mul(درون_داد_Array m, مزدوج scale) ثابت
{
    UMat dst;
    multiply(*هذا, m, dst, scale);
    أرجع dst;
}

/****************************************************************************************\
*                                      addWeighted                                       *
\****************************************************************************************/

}

فراغ cv::جمع_کن_Weighted( درون_داد_Array src1, مزدوج alpha, درون_داد_Array src2,
                      مزدوج beta, مزدوج gamma, OutputArray dst, صحيح dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADDW, src1, src2, dst, noArray(), dtype, OCL_OP_ADDW, /*muldiv*/ درست,
              Scalar(alpha, beta, gamma));
}


/****************************************************************************************\
*                                          compare                                       *
\****************************************************************************************/

نطاق cv
{


ساكن مزدوج getMinVal(صحيح عمق)
{
    ساكن ثابت مزدوج tab[CV_عمق_MAX] =
    {
        0, -128, 0, -32768, INT_MIN, -FLT_MAX, -DBL_MAX,
        -65504, -FLT_MAX, 0, 0, (مزدوج)INT64_MIN, 0
    };
    أرجع tab[عمق];
}

ساكن مزدوج getMaxVal(صحيح عمق)
{
    ساكن ثابت مزدوج tab[CV_عمق_MAX] = {
        255, 127, 65535, 32767, بیشترین_صحیح, FLT_MAX, DBL_MAX,
        65504, FLT_MAX, 255, (مزدوج)UINT64_MAX, (مزدوج)INT64_MAX, (مزدوج)UINT32_MAX, 0
    };
    أرجع tab[عمق];
}

#اگر_تعریف_شده HAVE_OPENCL

ساكن منطقي ocl_compare(درون_داد_Array _src1, درون_داد_Array _src2, OutputArray _dst, صحيح op, منطقي haveScalar)
{
    ثابت ocl::Device& dev = ocl::Device::getDefault();
    منطقي doubleSupport = dev.doubleFPConfig() > 0;
    صحيح type1 = _src1.type(), depth1 = CV_MAT_عمق(type1), cn = CV_MAT_CN(type1),
            type2 = _src2.type(), depth2 = CV_MAT_عمق(type2);

    إذا (!doubleSupport && depth1 == CV_64F)
        أرجع خطأ;

    // the integer scalar path below works in int; let the CPU handle the wider 5.x integer depths
    إذا (haveScalar && (depth1 == CV_32U || depth1 == CV_64U || depth1 == CV_64S))
        أرجع خطأ;

    إذا (!haveScalar && (!_src1.same_اندازه(_src2) || type1 != type2))
            أرجع خطأ;

    // This OpenCL kernel broadcasts a scalar's FIRST channel to all channels. The CPU path compares
    // per-channel (scalar[c] for channel c), so a multichannel array against a scalar with DISTINCT
    // channel values would disagree - bail to the CPU engine in that case (rare; not worth a kernel fix).
    إذا (haveScalar && cn > 1)
    {
        Mat sc = _src2.getMat();
        صحيح scn = (صحيح)sc.مجموع();
        إذا (scn > 1)
        {
            مزدوج v[4] = { 0, 0, 0, 0 };
            getConvertFunc(sc.عمق(), CV_64F)(sc.ptr(), 1, 0, 1, (uchar*)v, 1, اندازه(std::min(scn, 4), 1), 0);
            لكل (صحيح i = 1, n = std::min(cn, scn); i < n; i++)
                إذا (v[i] != v[0])
                    أرجع خطأ;
        }
    }

    صحيح kercn = haveScalar ? cn : ocl::predict_Optimal_وکتور_عرض(_src1, _src2, _dst), ردیفها_در_هر_WI = dev.isIntel() ? 4 : 1;
    // Workaround for bug with "?:" operator in AMD OpenCL compiler
    إذا (depth1 >= CV_16U)
        kercn = 1;

    صحيح scalarcn = kercn == 3 ? 4 : kercn;
    ثابت حرف * ثابت operationMap[] = { "==", ">", ">=", "<", "<=", "!=" };
    حرف cvt[50];

    String opts = format("-D %s -D srcT1=%s -D dstT=%s -D DEPTH_dst=%d -D workT=srcT1 -D cn=%d"
                         " -D convertToDT=%s -D OP_CMP -D CMP_OPERATOR=%s -D srcT1_C1=%s"
                         " -D srcT2_C1=%s -D dstT_C1=%s -D workST=%s -D rowsPerWI=%d%s",
                         haveScalar ? "UNARY_OP" : "BINARY_OP",
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(depth1, kercn)),
                         ocl::type_مقصد_Str(CV_8UC(kercn)), CV_8U, kercn,
                         ocl::convertTypeStr(depth1, CV_8U, kercn, cvt, حجم(cvt)),
                         operationMap[op], ocl::type_مقصد_Str(depth1),
                         ocl::type_مقصد_Str(depth1), ocl::type_مقصد_Str(CV_8U),
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(depth1, scalarcn)), ردیفها_در_هر_WI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    UMat src1 = _src1.getUMat();
    اندازه size = src1.size();
    _dst.create(size, CV_8UC(cn));
    UMat dst = _dst.getUMat();

    إذا (haveScalar)
    {
        size_t esz = CV_ELEM_SIZE1(type1) * scalarcn;
        مزدوج buf[4] = { 0, 0, 0, 0 };
        Mat src2 = _src2.getMat();

        إذا( depth1 > CV_32S )
            convertAndUnrollScalar( src2, depth1, (uchar *)buf, kercn );
        وإلا
        {
            مزدوج fval = 0;
            getConvertFunc(depth2, CV_64F)(src2.ptr(), 1, 0, 1, (uchar *)&fval, 1, اندازه(1, 1), 0);
            إذا( fval < getMinVal(depth1) )
                أرجع dst.مجموعه_مقصد(Scalar::all(op == CMP_GT || op == CMP_GE || op == CMP_NE ? 255 : 0)), درست;

            إذا( fval > getMaxVal(depth1) )
                أرجع dst.مجموعه_مقصد(Scalar::all(op == CMP_LT || op == CMP_LE || op == CMP_NE ? 255 : 0)), درست;

            صحيح ival = cv_دور(fval);
            إذا( fval != ival )
            {
                إذا( op == CMP_LT || op == CMP_GE )
                    ival = cvCeil(fval);
                وإلا إذا( op == CMP_LE || op == CMP_GT )
                    ival = cvFloor(fval);
                وإلا
                    أرجع dst.مجموعه_مقصد(Scalar::all(op == CMP_NE ? 255 : 0)), درست;
            }
            convertAndUnrollScalar(Mat(1, 1, CV_32S, &ival), depth1, (uchar *)buf, kercn);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        k.آرگومانها(ocl::KernelArg::Read_Only_No_اندازه(src1, cn, kercn),
               ocl::KernelArg::WriteOnly(dst, cn, kercn), scalararg);
    }
    وإلا
    {
        UMat src2 = _src2.getUMat();

        k.آرگومانها(ocl::KernelArg::Read_Only_No_اندازه(src1),
               ocl::KernelArg::Read_Only_No_اندازه(src2),
               ocl::KernelArg::WriteOnly(dst, cn, kercn));
    }

    size_t globalsize[2] = { (size_t)dst.تعداد_ستون * cn / kercn, ((size_t)dst.ردیفها + ردیفها_در_هر_WI - 1) / ردیفها_در_هر_WI };
    أرجع k.پیاپی(2, globalsize, عدم, خطأ);
}

#پایان_اگر

}

فراغ cv::compare(درون_داد_Array _src1, درون_داد_Array _src2, OutputArray _dst, صحيح op)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( op == CMP_LT || op == CMP_LE || op == CMP_EQ ||
               op == CMP_NE || op == CMP_GE || op == CMP_GT );

    CV_Assert(_src1.empty() == _src2.empty());
    إذا (_src1.empty() && _src2.empty())
    {
        _dst.release();
        أرجع;
    }

    منطقي haveScalar = خطأ;

    إذا ((_src1.isMatx() + _src2.isMatx()) == 1
            || !_src1.same_اندازه(_src2)
            || _src1.type() != _src2.type())
    {
        منطقي is_src1_scalar = checkScalar(_src1, _src2.type(), _src1.نوع(), _src2.نوع());
        منطقي is_src2_scalar = checkScalar(_src2, _src1.type(), _src2.نوع(), _src1.نوع());

        // exactly one scalar operand: keep it as src2 (swapping flips the ordering ops) so the OpenCL
        // path sees the array-op-scalar form. Otherwise - two arrays of different size (broadcast),
        // mixed types, or two scalars - the CPU engine broadcasts / promotes them, so there is NO error
        // here: fall through to arithm_op (which handles scalar detection, broadcast and mixed types).
        إذا (is_src1_scalar && !is_src2_scalar)
        {
            op = op == CMP_LT ? CMP_GT : op == CMP_LE ? CMP_GE :
                op == CMP_GE ? CMP_LE : op == CMP_GT ? CMP_LT : op;
            // src1 is a scalar; swap it with src2
            compare(_src2, _src1, _dst, op);
            أرجع;
        }
        haveScalar = is_src2_scalar && !is_src1_scalar;
    }
    CV_استفادهنشده(haveScalar);   // consumed by CV_OCL_RUN only - a no-op without OpenCL

    // OpenCL handles only the same-size and array-op-scalar forms; a broadcast between two arrays of
    // different size falls through to the CPU engine.
    CV_OCL_پیاپی(_src1.dims() <= 2 && _src2.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat())
               && (haveScalar || _src1.same_اندازه(_src2)),
               ocl_compare(_src1, _src2, _dst, op, haveScalar))

    // CPU: the element-wise engine. A CMP_* code maps to the corresponding OP_CMP_* TOp; the engine
    // forces a u8 mask (dtype = CV_8U) and compares in the common operand type (a fractional scalar
    // threshold promotes both operands to float - matching the classic ceil/floor threshold logic).
    // The scalar / broadcast / mixed-shape cases are handled inside arithm_op. oclop = -1 tells it the
    // OpenCL path was already tried above (via ocl_compare), so it only runs the CPU engine.
    ew::بالاترین cmpOp = op == CMP_EQ ? ew::OP_CMP_EQ : op == CMP_NE ? ew::OP_CMP_NE :
                    op == CMP_LT ? ew::OP_CMP_LT : op == CMP_LE ? ew::OP_CMP_LE :
                    op == CMP_GT ? ew::OP_CMP_GT : ew::OP_CMP_GE;
    arithm_op(cmpOp, _src1, _src2, _dst, noArray(), CV_8U, /*oclop=*/-1, /*muldiv=*/خطأ);
}

/****************************************************************************************\
*                                        inRange                                         *
\****************************************************************************************/

نطاق cv
{

قالب <نام_نوع T>
بنية درون_Range_SIMD
{
    صحيح عملگر () (ثابت T * src1, ثابت T * src2, ثابت T * src3, uchar * dst, صحيح len) ثابت
    {
        لكل (صحيح x = 0; x < len; x++)
            dst[x] = (src2[x] <= src1[x] && src1[x] <= src3[x]) ? 255 : 0;
        أرجع len;
    }
};

#إذا (CV_SIMD || CV_SIMD_SCALABLE)

قالب <>
بنية درون_Range_SIMD<uchar>
{
    صحيح عملگر () (ثابت uchar * src1, ثابت uchar * src2, ثابت uchar * src3,
        uchar * dst, صحيح len) ثابت
    {
        صحيح x = 0;
        ثابت صحيح عرض = VTraits<v_uint8>::vlanes();

        لكل (; x <= len - عرض; x += عرض)
        {
            v_uint8 values = vx_بارگیری(src1 + x);
            v_uint8 کم = vx_بارگیری(src2 + x);
            v_uint8 بالا = vx_بارگیری(src3 + x);

            v_store(dst + x, v_and(v_ge(values, کم), v_ge(بالا, values)));
        }
        vx_cleanup();
        أرجع x;
    }
};

قالب <>
بنية درون_Range_SIMD<schar>
{
    صحيح عملگر () (ثابت schar * src1, ثابت schar * src2, ثابت schar * src3,
        uchar * dst, صحيح len) ثابت
    {
        صحيح x = 0;
        ثابت صحيح عرض = VTraits<v_int8>::vlanes();

        لكل (; x <= len - عرض; x += عرض)
        {
            v_int8 values = vx_بارگیری(src1 + x);
            v_int8 کم = vx_بارگیری(src2 + x);
            v_int8 بالا = vx_بارگیری(src3 + x);

            v_store((schar*)(dst + x), v_and(v_ge(values, کم), v_ge(بالا, values)));
        }
        vx_cleanup();
        أرجع x;
    }
};

قالب <>
بنية درون_Range_SIMD<ushort>
{
    صحيح عملگر () (ثابت ushort * src1, ثابت ushort * src2, ثابت ushort * src3,
        uchar * dst, صحيح len) ثابت
    {
        صحيح x = 0;
        ثابت صحيح عرض = VTraits<v_uint16>::vlanes() * 2;

        لكل (; x <= len - عرض; x += عرض)
        {
            v_uint16 values1 = vx_بارگیری(src1 + x);
            v_uint16 low1 = vx_بارگیری(src2 + x);
            v_uint16 high1 = vx_بارگیری(src3 + x);

            v_uint16 values2 = vx_بارگیری(src1 + x + VTraits<v_uint16>::vlanes());
            v_uint16 low2 = vx_بارگیری(src2 + x + VTraits<v_uint16>::vlanes());
            v_uint16 high2 = vx_بارگیری(src3 + x + VTraits<v_uint16>::vlanes());

            v_store(dst + x, v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2))));
        }
        vx_cleanup();
        أرجع x;
    }
};

قالب <>
بنية درون_Range_SIMD<کوتاه>
{
    صحيح عملگر () (ثابت کوتاه * src1, ثابت کوتاه * src2, ثابت کوتاه * src3,
        uchar * dst, صحيح len) ثابت
    {
        صحيح x = 0;
        ثابت صحيح عرض = (صحيح)VTraits<v_int16>::vlanes() * 2;

        لكل (; x <= len - عرض; x += عرض)
        {
            v_int16 values1 = vx_بارگیری(src1 + x);
            v_int16 low1 = vx_بارگیری(src2 + x);
            v_int16 high1 = vx_بارگیری(src3 + x);

            v_int16 values2 = vx_بارگیری(src1 + x + VTraits<v_int16>::vlanes());
            v_int16 low2 = vx_بارگیری(src2 + x + VTraits<v_int16>::vlanes());
            v_int16 high2 = vx_بارگیری(src3 + x + VTraits<v_int16>::vlanes());

            v_store((schar*)(dst + x), v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2))));
        }
        vx_cleanup();
        أرجع x;
    }
};

قالب <>
بنية درون_Range_SIMD<صحيح>
{
    صحيح عملگر () (ثابت صحيح * src1, ثابت صحيح * src2, ثابت صحيح * src3,
        uchar * dst, صحيح len) ثابت
    {
        صحيح x = 0;
        ثابت صحيح عرض = (صحيح)VTraits<v_int32>::vlanes() * 2;

        لكل (; x <= len - عرض; x += عرض)
        {
            v_int32 values1 = vx_بارگیری(src1 + x);
            v_int32 low1 = vx_بارگیری(src2 + x);
            v_int32 high1 = vx_بارگیری(src3 + x);

            v_int32 values2 = vx_بارگیری(src1 + x + VTraits<v_int32>::vlanes());
            v_int32 low2 = vx_بارگیری(src2 + x + VTraits<v_int32>::vlanes());
            v_int32 high2 = vx_بارگیری(src3 + x + VTraits<v_int32>::vlanes());

            v_pack_store(dst + x, v_reinterpret_as_u16(v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2)))));
        }
        vx_cleanup();
        أرجع x;
    }
};

قالب <>
بنية درون_Range_SIMD<عائم>
{
    صحيح عملگر () (ثابت عائم * src1, ثابت عائم * src2, ثابت عائم * src3,
        uchar * dst, صحيح len) ثابت
    {
        صحيح x = 0;
        ثابت صحيح عرض = (صحيح)VTraits<v_float32>::vlanes() * 2;

        لكل (; x <= len - عرض; x += عرض)
        {
            v_float32 values1 = vx_بارگیری(src1 + x);
            v_float32 low1 = vx_بارگیری(src2 + x);
            v_float32 high1 = vx_بارگیری(src3 + x);

            v_float32 values2 = vx_بارگیری(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_بارگیری(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_بارگیری(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        أرجع x;
    }
};

قالب <>
بنية درون_Range_SIMD<hfloat>
{
    صحيح عملگر () (ثابت hfloat * src1, ثابت hfloat * src2, ثابت hfloat * src3,
        uchar * dst, صحيح len) ثابت
    {
        صحيح x = 0;
        ثابت صحيح عرض = (صحيح)VTraits<v_float32>::vlanes()*2;

        لكل (; x <= len - عرض; x += عرض)
        {
            v_float32 values1 = vx_بارگیری_expand(src1 + x);
            v_float32 low1 = vx_بارگیری_expand(src2 + x);
            v_float32 high1 = vx_بارگیری_expand(src3 + x);

            v_float32 values2 = vx_بارگیری_expand(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_بارگیری_expand(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_بارگیری_expand(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        أرجع x;
    }
};

قالب <>
بنية درون_Range_SIMD<bfloat>
{
    صحيح عملگر () (ثابت bfloat * src1, ثابت bfloat * src2, ثابت bfloat * src3,
        uchar * dst, صحيح len) ثابت
    {
        صحيح x = 0;
        ثابت صحيح عرض = (صحيح)VTraits<v_float32>::vlanes()*2;

        لكل (; x <= len - عرض; x += عرض)
        {
            v_float32 values1 = vx_بارگیری_expand(src1 + x);
            v_float32 low1 = vx_بارگیری_expand(src2 + x);
            v_float32 high1 = vx_بارگیری_expand(src3 + x);

            v_float32 values2 = vx_بارگیری_expand(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_بارگیری_expand(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_بارگیری_expand(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        أرجع x;
    }
};

قالب <>
بنية درون_Range_SIMD<بیعلامت>
{
    صحيح عملگر () (ثابت بیعلامت * src1, ثابت بیعلامت * src2, ثابت بیعلامت * src3,
        uchar * dst, صحيح len) ثابت
    {
        صحيح x = 0;
        ثابت صحيح عرض = (صحيح)VTraits<v_uint32>::vlanes() * 2;

        لكل (; x <= len - عرض; x += عرض)
        {
            v_uint32 values1 = vx_بارگیری(src1 + x);
            v_uint32 low1 = vx_بارگیری(src2 + x);
            v_uint32 high1 = vx_بارگیری(src3 + x);

            v_uint32 values2 = vx_بارگیری(src1 + x + VTraits<v_uint32>::vlanes());
            v_uint32 low2 = vx_بارگیری(src2 + x + VTraits<v_uint32>::vlanes());
            v_uint32 high2 = vx_بارگیری(src3 + x + VTraits<v_uint32>::vlanes());

            v_pack_store(dst + x, v_reinterpret_as_u16(v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2)))));
        }
        vx_cleanup();
        أرجع x;
    }
};

#إذا CV_SIMD_64F

قالب <>
بنية درون_Range_SIMD<مزدوج>
{
    صحيح عملگر () (ثابت مزدوج * src1, ثابت مزدوج * src2, ثابت مزدوج * src3,
        uchar * dst, صحيح len) ثابت
    {
        صحيح x = 0;
        ثابت صحيح گام = VTraits<v_float64>::vlanes();
        ثابت صحيح عرض = گام * 4;

        لكل (; x <= len - عرض; x += عرض)
        {
            v_float64 v1 = vx_بارگیری(src1 + x);
            v_float64 l1 = vx_بارگیری(src2 + x);
            v_float64 h1 = vx_بارگیری(src3 + x);
            v_uint64 m1 = v_reinterpret_as_u64(v_and(v_ge(v1, l1), v_ge(h1, v1)));

            v_float64 v2 = vx_بارگیری(src1 + x + گام);
            v_float64 l2 = vx_بارگیری(src2 + x + گام);
            v_float64 h2 = vx_بارگیری(src3 + x + گام);
            v_uint64 m2 = v_reinterpret_as_u64(v_and(v_ge(v2, l2), v_ge(h2, v2)));

            v_float64 v3 = vx_بارگیری(src1 + x + گام * 2);
            v_float64 l3 = vx_بارگیری(src2 + x + گام * 2);
            v_float64 h3 = vx_بارگیری(src3 + x + گام * 2);
            v_uint64 m3 = v_reinterpret_as_u64(v_and(v_ge(v3, l3), v_ge(h3, v3)));

            v_float64 v4 = vx_بارگیری(src1 + x + گام * 3);
            v_float64 l4 = vx_بارگیری(src2 + x + گام * 3);
            v_float64 h4 = vx_بارگیری(src3 + x + گام * 3);
            v_uint64 m4 = v_reinterpret_as_u64(v_and(v_ge(v4, l4), v_ge(h4, v4)));


            v_pack_store(dst + x, v_pack(v_pack(m1, m2), v_pack(m3, m4)));
        }
        vx_cleanup();
        أرجع x;
    }
};

#پایان_اگر

#پایان_اگر

قالب <نام_نوع T>
ساكن فراغ درون_Range(ثابت فراغ* _src1, size_t step1, ثابت فراغ* _src2, size_t step2,
         ثابت فراغ* _src3, size_t step3, uchar* dst, size_t گام,
         اندازه size)
{
    ثابت T* src1 = (ثابت T*)_src1;
    ثابت T* src2 = (ثابت T*)_src2;
    ثابت T* src3 = (ثابت T*)_src3;
    step1 /= حجم(src1[0]);
    step2 /= حجم(src2[0]);
    step3 /= حجم(src3[0]);

    درون_Range_SIMD<T> vop;

    لكل( ; size.ارتفاع--; src1 += step1, src2 += step2, src3 += step3, dst += گام )
    {
        صحيح x = vop(src1, src2, src3, dst, size.عرض);
        #إذا CV_ENABLE_UNROLLED
        لكل( ; x <= size.عرض - 4; x += 4 )
        {
            صحيح t0, t1;
            t0 = src2[x] <= src1[x] && src1[x] <= src3[x];
            t1 = src2[x+1] <= src1[x+1] && src1[x+1] <= src3[x+1];
            dst[x] = (uchar)-t0; dst[x+1] = (uchar)-t1;
            t0 = src2[x+2] <= src1[x+2] && src1[x+2] <= src3[x+2];
            t1 = src2[x+3] <= src1[x+3] && src1[x+3] <= src3[x+3];
            dst[x+2] = (uchar)-t0; dst[x+3] = (uchar)-t1;
        }
        #پایان_اگر
        لكل( ; x < size.عرض; x++ )
            dst[x] = (uchar)-(src2[x] <= src1[x] && src1[x] <= src3[x]);
    }
}

ساكن فراغ درون_Range_Reduce(ثابت uchar* src, uchar* dst, size_t len, صحيح cn)
{
    صحيح k = cn % 4 ? cn % 4 : 4;
    size_t i, j;
    إذا( k == 1 )
        لكل( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j];
    وإلا إذا( k == 2 )
        لكل( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1];
    وإلا إذا( k == 3 )
        لكل( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2];
    وإلا
        لكل( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2] & src[j+3];

    لكل( ; k < cn; k += 4 )
    {
        لكل( i = 0, j = k; i < len; i++, j += cn )
            dst[i] &= src[j] & src[j+1] & src[j+2] & src[j+3];
    }
}

تعريفنوع فراغ (*درون_Range_Func)( ثابت فراغ* src1, size_t step1, ثابت فراغ* src2, size_t step2,
                             ثابت فراغ* src3, size_t step3, uchar* dst, size_t گام, اندازه sz );

ساكن درون_Range_Func get_درون_Range_Func(صحيح عمق)
{
    ساكن درون_Range_Func درون_Range_Tab[CV_عمق_MAX] =
    {
        درون_Range<uchar>,
        درون_Range<schar>,
        درون_Range<ushort>,
        درون_Range<کوتاه>,
        درون_Range<صحيح>,
        درون_Range<عائم>,
        درون_Range<مزدوج>,
        درون_Range<hfloat>,
        درون_Range<bfloat>,
        0,
        درون_Range<uint64_t>,
        درون_Range<int64_t>,
        درون_Range<بیعلامت>,
        0,
    };

    أرجع درون_Range_Tab[عمق];
}

#اگر_تعریف_شده HAVE_OPENCL

ساكن منطقي ocl_درون_Range( درون_داد_Array _src, درون_داد_Array _lowerb,
                         درون_داد_Array _upperb, OutputArray _dst )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();
    درون_داد_Array_2::نوع_Flag skind = _src.نوع(), lkind = _lowerb.نوع(), ukind = _upperb.نوع();
    اندازه ssize = _src.size(), lsize = _lowerb.size(), usize = _upperb.size();
    صحيح stype = _src.type(), ltype = _lowerb.type(), utype = _upperb.type();
    صحيح sdepth = CV_MAT_عمق(stype), ldepth = CV_MAT_عمق(ltype), udepth = CV_MAT_عمق(utype);
    صحيح cn = CV_MAT_CN(stype), ردیفها_در_هر_WI = d.isIntel() ? 4 : 1;
    منطقي lbScalar = خطأ, ubScalar = خطأ;

    إذا( (lkind == درون_داد_Array_2::MATX && skind != درون_داد_Array_2::MATX) ||
        ssize != lsize || stype != ltype )
    {
        إذا( !checkScalar(_lowerb, stype, lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = درست;
    }

    إذا( (ukind == درون_داد_Array_2::MATX && skind != درون_داد_Array_2::MATX) ||
        ssize != usize || stype != utype )
    {
        إذا( !checkScalar(_upperb, stype, ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = درست;
    }

    إذا (lbScalar != ubScalar)
        أرجع خطأ;

    منطقي doubleSupport = d.doubleFPConfig() > 0,
            haveScalar = lbScalar && ubScalar;

    إذا ( (!doubleSupport && sdepth == CV_64F) ||
         (!haveScalar && (sdepth != ldepth || sdepth != udepth)) )
        أرجع خطأ;

    صحيح kercn = haveScalar ? cn : std::max(std::min(ocl::predict_Optimal_وکتور_عرض(_src, _lowerb, _upperb, _dst), 4), cn);
    إذا (kercn % cn != 0)
        kercn = cn;
    صحيح تعداد_ستون_در_هر_WI = kercn / cn;
    String opts = format("%s-D CN=%d -D SRC_T=%s -D SRC_T1=%s -D DST_T=%s -D KERCN=%d -D DEPTH=%d%s -D COLS_PER_WI=%d",
                           haveScalar ? "-D HAVE_SCALAR " : "", cn, ocl::type_مقصد_Str(CV_MAKE_TYPE(sdepth, kercn)),
                           ocl::type_مقصد_Str(sdepth), ocl::type_مقصد_Str(CV_8UC(تعداد_ستون_در_هر_WI)), kercn, sdepth,
                           doubleSupport ? " -D DOUBLE_SUPPORT" : "", تعداد_ستون_در_هر_WI);

    ocl::Kernel ker("inrange", ocl::core::inrange_oclsrc, opts);
    إذا (ker.empty())
        أرجع خطأ;

    _dst.create(ssize, CV_8UC1);
    UMat src = _src.getUMat(), dst = _dst.getUMat(), lscalaru, uscalaru;
    Mat lscalar, uscalar;

    إذا (lbScalar && ubScalar)
    {
        lscalar = _lowerb.getMat();
        uscalar = _upperb.getMat();

        size_t esz = src.elem_اندازه();
        size_t blocksize = 36;

        Auto_میانگیر<uchar> _buf(blocksize*(((صحيح)lbScalar + (صحيح)ubScalar)*esz + cn) + 2*cn*حجم(صحيح) + 128);
        uchar *buf = alignPtr(_buf.داده() + blocksize*cn, 16);

        إذا( ldepth != sdepth && sdepth < CV_32S )
        {
            صحيح* ilbuf = (صحيح*)alignPtr(buf + blocksize*esz, 16);
            صحيح* iubuf = ilbuf + cn;

            دودویی_Func sccvtfunc = getConvertFunc(ldepth, CV_32S);
            sccvtfunc(lscalar.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, اندازه(cn, 1), 0);
            sccvtfunc(uscalar.ptr(), 1, 0, 1, (uchar*)iubuf, 1, اندازه(cn, 1), 0);
            صحيح minval = cv_دور(getMinVal(sdepth)), maxval = cv_دور(getMaxVal(sdepth));

            لكل( صحيح k = 0; k < cn; k++ )
            {
                إذا( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lscalar = Mat(cn, 1, CV_32S, ilbuf);
            uscalar = Mat(cn, 1, CV_32S, iubuf);
        }

        lscalar.convert_مقصد(lscalar, stype);
        uscalar.convert_مقصد(uscalar, stype);
    }
    وإلا
    {
        lscalaru = _lowerb.getUMat();
        uscalaru = _upperb.getUMat();
    }

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_اندازه(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, 1, تعداد_ستون_در_هر_WI);

    إذا (haveScalar)
    {
        lscalar.copy_مقصد(lscalaru);
        uscalar.copy_مقصد(uscalaru);

        ker.آرگومانها(srcarg, dstarg, ocl::KernelArg::PtrReadOnly(lscalaru),
               ocl::KernelArg::PtrReadOnly(uscalaru), ردیفها_در_هر_WI);
    }
    وإلا
        ker.آرگومانها(srcarg, dstarg, ocl::KernelArg::Read_Only_No_اندازه(lscalaru),
               ocl::KernelArg::Read_Only_No_اندازه(uscalaru), ردیفها_در_هر_WI);

    size_t globalsize[2] = { (size_t)ssize.عرض / تعداد_ستون_در_هر_WI, ((size_t)ssize.ارتفاع + ردیفها_در_هر_WI - 1) / ردیفها_در_هر_WI };
    أرجع ker.پیاپی(2, globalsize, عدم, خطأ);
}

#پایان_اگر

}

فراغ cv::درون_Range_2(درون_داد_Array _src, درون_داد_Array _lowerb,
                 درون_داد_Array _upperb, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(! _src.empty());
    إذا (!get_درون_Range_Func(_src.عمق()))
        CV_Error( cv::Error::StsNotImplemented,
                 "inRange: CV_Bool and CV_8F inputs are not supported, convert the input to another type first");

    CV_OCL_پیاپی(_src.dims() <= 2 && _lowerb.dims() <= 2 &&
               _upperb.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat()),
               ocl_درون_Range(_src, _lowerb, _upperb, _dst))

    درون_داد_Array_2::نوع_Flag skind = _src.نوع(), lkind = _lowerb.نوع(), ukind = _upperb.نوع();
    Mat src = _src.getMat(), lb = _lowerb.getMat(), ub = _upperb.getMat();

    منطقي lbScalar = خطأ, ubScalar = خطأ;

    إذا( (lkind == درون_داد_Array_2::MATX && skind != درون_داد_Array_2::MATX) ||
        src.size != lb.size || src.type() != lb.type() )
    {
        إذا( !checkScalar(lb, src.type(), lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = درست;
    }

    إذا( (ukind == درون_داد_Array_2::MATX && skind != درون_داد_Array_2::MATX) ||
        src.size != ub.size || src.type() != ub.type() )
    {
        إذا( !checkScalar(ub, src.type(), ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = درست;
    }

    CV_Assert(lbScalar == ubScalar);

    صحيح cn = src.channels(), عمق = src.عمق();

    size_t esz = src.elem_اندازه();
    size_t blocksize0 = (size_t)(بلوک_اندازه + esz-1)/esz;

    _dst.create_Same_اندازه(_src, CV_8UC1);
    Mat dst = _dst.getMat();
    درون_Range_Func func = get_درون_Range_Func(عمق);

    ثابت Mat* arrays_sc[] = { &src, &dst, 0 };
    ثابت Mat* arrays_nosc[] = { &src, &dst, &lb, &ub, 0 };
    uchar* ptrs[4] = {};

    NAryMatIterator it(lbScalar && ubScalar ? arrays_sc : arrays_nosc, ptrs);
    size_t مجموع = it.size, blocksize = std::min(مجموع, blocksize0);

    Auto_میانگیر<uchar> _buf(blocksize*(((صحيح)lbScalar + (صحيح)ubScalar)*esz + cn) + 2*cn*حجم(صحيح) + 128);
    uchar *buf = _buf.داده(), *mbuf = buf, *lbuf = 0, *ubuf = 0;
    buf = alignPtr(buf + blocksize*cn, 16);

    إذا( lbScalar && ubScalar )
    {
        lbuf = buf;
        ubuf = buf = alignPtr(buf + blocksize*esz, 16);

        CV_Assert( lb.type() == ub.type() );
        صحيح scdepth = lb.عمق();

        إذا( scdepth != عمق && عمق < CV_32S )
        {
            صحيح* ilbuf = (صحيح*)alignPtr(buf + blocksize*esz, 16);
            صحيح* iubuf = ilbuf + cn;

            دودویی_Func sccvtfunc = getConvertFunc(scdepth, CV_32S);
            sccvtfunc(lb.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, اندازه(cn, 1), 0);
            sccvtfunc(ub.ptr(), 1, 0, 1, (uchar*)iubuf, 1, اندازه(cn, 1), 0);
            صحيح minval = cv_دور(getMinVal(عمق)), maxval = cv_دور(getMaxVal(عمق));

            لكل( صحيح k = 0; k < cn; k++ )
            {
                إذا( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lb = Mat(cn, 1, CV_32S, ilbuf);
            ub = Mat(cn, 1, CV_32S, iubuf);
        }

        convertAndUnrollScalar( lb, src.type(), lbuf, blocksize );
        convertAndUnrollScalar( ub, src.type(), ubuf, blocksize );

        إذا (عمق == CV_8U && src.dims <= 2) {
            uint8_t lb_scalar = lbuf[0];
            uint8_t ub_scalar = ubuf[0];
            CALL_HAL(درون_Range_u8, cv_hal_درون_Range8u, src.داده, src.گام, dst.داده, dst.گام, dst.عمق(), src.تعداد_ستون, src.ردیفها, src.channels(),
                lb_scalar, ub_scalar);
        } وإلا إذا (عمق == CV_32F && src.dims <= 2) {
            مزدوج lb_scalar = lb.ptr<مزدوج>(0)[0];
            مزدوج ub_scalar = ub.ptr<مزدوج>(0)[0];
            CALL_HAL(درون_Range_f32, cv_hal_درون_Range32f, src.داده, src.گام, dst.داده, dst.گام, dst.عمق(), src.تعداد_ستون, src.ردیفها, src.channels(),
                lb_scalar, ub_scalar);
        }
    }

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( size_t j = 0; j < مجموع; j += blocksize )
        {
            صحيح bsz = (صحيح)MIN(مجموع - j, blocksize);
            size_t delta = bsz*esz;
            uchar *lptr = lbuf, *uptr = ubuf;
            إذا( !lbScalar )
            {
                lptr = ptrs[2];
                ptrs[2] += delta;
            }
            إذا( !ubScalar )
            {
                صحيح idx = !lbScalar ? 3 : 2;
                uptr = ptrs[idx];
                ptrs[idx] += delta;
            }
            func( ptrs[0], 0, lptr, 0, uptr, 0, cn == 1 ? ptrs[1] : mbuf, 0, اندازه(bsz*cn, 1));
            إذا( cn > 1 )
                درون_Range_Reduce(mbuf, ptrs[1], bsz, cn);
            ptrs[0] += delta;
            ptrs[1] += bsz;
        }
    }
}
/* End of file. */
