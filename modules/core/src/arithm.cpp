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

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "arithm_expr.hpp"   // the new element-wise engine (cv::ew)
#অন্তর্ভাব "opencl_kernels_core.hpp"

নামাকাশ cv
{

/****************************************************************************************\
*                                   logical operations                                   *
\****************************************************************************************/

গণনা { OCL_OP_ADD=0, OCL_OP_SUB=1, OCL_OP_RSUB=2, OCL_OP_ABSDIFF=3, OCL_OP_MUL=4,
       OCL_OP_MUL_SCALE=5, OCL_OP_DIV_SCALE=6, OCL_OP_RECIP_SCALE=7, OCL_OP_ADDW=8,
       OCL_OP_AND=9, OCL_OP_OR=10, OCL_OP_XOR=11, OCL_OP_NOT=12, OCL_OP_MIN=13, OCL_OP_MAX=14,
       OCL_OP_RDIV_SCALE=15 };

// The unified entry point for every element-wise binary op (empty + UMat/OpenCL + CPU engine). Defined
// lower down (near cv::add); forward-declared here so cv::min/cv::max (above it) can use it too.
স্থির শূন্য arithm_op(ew::শীর্ষ op, প্রবেশ_Array src1, প্রবেশ_Array src2, OutputArray dst,
                      প্রবেশ_Array mask, পূর্ণসংখ্যা dtype, পূর্ণসংখ্যা oclop, যৌক্তিক muldiv, ধ্রুবক Scalar& params = Scalar(1));

// The unary counterpart (bitwise NOT, unary math later). Forward-declared for the same reason.
স্থির শূন্য unary_op(ew::শীর্ষ op, প্রবেশ_Array src, OutputArray dst, প্রবেশ_Array mask,
                     পূর্ণসংখ্যা dtype, ধ্রুবক Scalar& params = Scalar());

#ifdef HAVE_OPENCL

স্থির ধ্রুবক অক্ষর* oclop2str[] = { "OP_ADD", "OP_SUB", "OP_RSUB", "OP_ABSDIFF",
    "OP_MUL", "OP_MUL_SCALE", "OP_DIV_SCALE", "OP_RECIP_SCALE",
    "OP_ADDW", "OP_AND", "OP_OR", "OP_XOR", "OP_NOT", "OP_MIN", "OP_MAX", "OP_RDIV_SCALE", 0 };

// depths with no OpenCL type name (ocl::typeToStr() asserts on them)
স্থির অন্তঃরেখ যৌক্তিক noOclType(পূর্ণসংখ্যা depth)
{
    ফেরত depth == CV_16BF || depth == CV_Bool || depth == CV_8F_E4M3FN || depth == CV_8F_E4M3FNUZ;
}

স্থির যৌক্তিক ocl_দ্বিমিক_op(প্রবেশ_Array _src1, প্রবেশ_Array _src2, OutputArray _dst,
                          প্রবেশ_Array _mask, যৌক্তিক bitwise, পূর্ণসংখ্যা oclop, যৌক্তিক haveScalar )
{
    যৌক্তিক haveMask = !_mask.empty();
    পূর্ণসংখ্যা srctype = _src1.type();
    পূর্ণসংখ্যা srcdepth = CV_MAT_DEPTH(srctype);
    পূর্ণসংখ্যা cn = CV_MAT_CN(srctype);

    ধ্রুবক ocl::Device d = ocl::Device::getDefault();
    যৌক্তিক doubleSupport = d.doubleFPConfig() > 0;
    যদি( oclop < 0 || ((haveMask || haveScalar) && cn > 4) ||
            (!doubleSupport && srcdepth == CV_64F && !bitwise) ||
            (noOclType(srcdepth) && !bitwise))
        ফেরত মিথ্যা;

    অক্ষর opts[1024];
    পূর্ণসংখ্যা kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    পূর্ণসংখ্যা scalarcn = kercn == 3 ? 4 : kercn;
    পূর্ণসংখ্যা rowsPerWI = d.isIntel() ? 4 : 1;

    ধ্রুবক পূর্ণসংখ্যা dstDepth = srcdepth;
    ধ্রুবক পূর্ণসংখ্যা dstType = CV_MAKETYPE(dstDepth, kercn);
    ধ্রুবক পূর্ণসংখ্যা dstType1 = CV_MAKETYPE(dstDepth, 1);
    ধ্রুবক পূর্ণসংখ্যা scalarType = CV_MAKETYPE(srcdepth, scalarcn);

    snprintf(opts, আকার(opts), "-D %s%s -D %s%s -D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workST=%s -D cn=%d -D rowsPerWI=%d",
            haveMask ? "MASK_" : "", haveScalar ? "UNARY_OP" : "BINARY_OP", oclop2str[oclop],
            doubleSupport ? " -D DOUBLE_SUPPORT" : "",
            bitwise ? ocl::memopTypeToStr(dstType) : ocl::typeToStr(dstType),
            dstDepth,
            bitwise ? ocl::memopTypeToStr(dstType1) : ocl::typeToStr(dstType1),
            bitwise ? ocl::memopTypeToStr(scalarType) : ocl::typeToStr(scalarType),
            kercn, rowsPerWI);

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_আকার(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_আকার(mask, 1);

    যদি( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(srctype)*scalarcn;
        দ্বিগুণ buf[4] = {0,0,0,0};

        যদি( oclop != OCL_OP_NOT )
        {
            Mat src2sc = _src2.getMat();
            convertAndUnrollScalar(src2sc, srctype, (uchar*)buf, 1);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        যদি( !haveMask )
            k.args(src1arg, dstarg, scalararg);
        অন্যথা
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    অন্যথা
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::Read_Only_No_আকার(src2, cn, kercn);

        যদি( !haveMask )
            k.args(src1arg, src2arg, dstarg);
        অন্যথা
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, 0, মিথ্যা);
}

#endif

// OpenCL path for the bitwise ops (and/or/xor/not), lifted out of binary_op: choose the (possibly
// scalar) operand order, size the destination, and dispatch the shared bitwise OpenCL kernel. Returns
// true if OpenCL handled the op, false to fall through to the CPU engine. NOT arrives unary
// (src2 == src1, forced-scalar). The caller gates this on a UMat operand + dims <= 2 (via CV_OCL_RUN).
#ifdef HAVE_OPENCL
স্থির যৌক্তিক bitwise_op_ocl(প্রবেশ_Array _src1, প্রবেশ_Array _src2, OutputArray _dst,
                           প্রবেশ_Array _mask, পূর্ণসংখ্যা oclop)
{
    ধ্রুবক প্রবেশ_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    প্রবেশ_Array_2::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    পূর্ণসংখ্যা type1 = psrc1->type(), type2 = psrc2->type();
    যৌক্তিক haveScalar = মিথ্যা;

    যদি( oclop == OCL_OP_NOT )
        haveScalar = সত্য;
    অন্যথা যদি( (kind1 == প্রবেশ_Array_2::MATX) + (kind2 == প্রবেশ_Array_2::MATX) == 1 ||
             !psrc1->same_আকার(*psrc2) || type1 != type2 )
    {
        যদি( checkScalar(*psrc1, type2, kind1, kind2) )
        { std::swap(psrc1, psrc2); std::swap(type1, type2); }        // src1 is the scalar; swap it out
        অন্যথা যদি( !checkScalar(*psrc2, type1, kind2, kind1) )
            ফেরত মিথ্যা;
        haveScalar = সত্য;
    }
    _dst.create_Same_আকার(*psrc1, type1);
    ফেরত ocl_দ্বিমিক_op(*psrc1, *psrc2, _dst, _mask, সত্য, oclop, haveScalar);
}
#endif  // HAVE_OPENCL

}

// bitwise and/or/xor: OpenCL runs the shared bitwise kernel (via bitwise_op_ocl); the CPU path is the
// element-wise engine (OP_AND/OR/XOR, T x T -> T dispatched by element size). oclop = -1 tells
// arithm_op the OpenCL path was already tried, so it only runs the CPU engine. NOT is unary.
শূন্য cv::bitwise_and(প্রবেশ_Array a, প্রবেশ_Array b, OutputArray c, প্রবেশ_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_AND))
    arithm_op(ew::OP_AND, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/মিথ্যা);
}

শূন্য cv::bitwise_or(প্রবেশ_Array a, প্রবেশ_Array b, OutputArray c, প্রবেশ_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_OR))
    arithm_op(ew::OP_OR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/মিথ্যা);
}

শূন্য cv::bitwise_xor(প্রবেশ_Array a, প্রবেশ_Array b, OutputArray c, প্রবেশ_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_XOR))
    arithm_op(ew::OP_XOR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/মিথ্যা);
}

শূন্য cv::bitwise_not(প্রবেশ_Array a, OutputArray c, প্রবেশ_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || c.isUMat()) && a.dims() <= 2,
               bitwise_op_ocl(a, a, c, mask, OCL_OP_NOT))
    unary_op(ew::OP_NOT, a, c, mask, -1);
}

শূন্য cv::max( প্রবেশ_Array src1, প্রবেশ_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, মিথ্যা);
}

শূন্য cv::min( প্রবেশ_Array src1, প্রবেশ_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, মিথ্যা);
}

// The concrete Mat/UMat overloads of min/max exist because C++ overload resolution needs them (a
// bare cv::min(mat, mat, mat) binds Mat& more tightly than InputArray); they must run the SAME engine
// path as the InputArray forms - NOT the legacy binary_op - so mixed types and broadcasting work.
শূন্য cv::max(ধ্রুবক Mat& src1, ধ্রুবক Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, মিথ্যা);
}

শূন্য cv::min(ধ্রুবক Mat& src1, ধ্রুবক Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, মিথ্যা);
}

শূন্য cv::max(ধ্রুবক UMat& src1, ধ্রুবক UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, মিথ্যা);
}

শূন্য cv::min(ধ্রুবক UMat& src1, ধ্রুবক UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, মিথ্যা);
}


/****************************************************************************************\
*                                      add/subtract                                      *
\****************************************************************************************/

নামাকাশ cv
{

#ifdef HAVE_OPENCL

// used by the OpenCL branch only (arithm_op_ocl working-type selection)
স্থির পূর্ণসংখ্যা actualScalarDepth(ধ্রুবক দ্বিগুণ* তথ্য, পূর্ণসংখ্যা len)
{
    পূর্ণসংখ্যা i = 0, minval = INT_MAX, maxval = INT_MIN;
    জন্য(; i < len; ++i)
    {
        পূর্ণসংখ্যা ival = cvRound(তথ্য[i]);
        যদি( ival != তথ্য[i] )
            বিরতি;
        minval = MIN(minval, ival);
        maxval = MAX(maxval, ival);
    }
    ফেরত i < len ? CV_64F :
        minval >= 0 && maxval <= (পূর্ণসংখ্যা)UCHAR_MAX ? CV_8U :
        minval >= (পূর্ণসংখ্যা)SCHAR_MIN && maxval <= (পূর্ণসংখ্যা)SCHAR_MAX ? CV_8S :
        minval >= 0 && maxval <= (পূর্ণসংখ্যা)USHRT_MAX ? CV_16U :
        minval >= (পূর্ণসংখ্যা)SHRT_MIN && maxval <= (পূর্ণসংখ্যা)SHRT_MAX ? CV_16S :
        CV_32S;
}

স্থির পূর্ণসংখ্যা coerceTypes(পূর্ণসংখ্যা depth1, পূর্ণসংখ্যা depth2, যৌক্তিক muldiv)
{
    ফেরত depth1 == depth2 ? depth1 :
        ((depth1 <= CV_32S) & (depth2 <= CV_32S)) != 0 ?
        (((পূর্ণসংখ্যা)!muldiv & (depth1 <= CV_8S) & (depth2 <= CV_8S)) != 0 ? CV_16S : CV_32S) :
        ((CV_ELEM_SIZE1(depth1) > 4) | (CV_ELEM_SIZE1(depth2) > 4)) != 0 ? CV_64F : CV_32F;
}

স্থির যৌক্তিক ocl_arithm_op(প্রবেশ_Array _src1, প্রবেশ_Array _src2, OutputArray _dst,
                          প্রবেশ_Array _mask, পূর্ণসংখ্যা wtype,
                          শূন্য* usrdata, পূর্ণসংখ্যা oclop,
                          যৌক্তিক haveScalar )
{
    ধ্রুবক ocl::Device d = ocl::Device::getDefault();
    যৌক্তিক doubleSupport = d.doubleFPConfig() > 0;
    পূর্ণসংখ্যা type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    যৌক্তিক haveMask = !_mask.empty();

    যদি ( (haveMask || haveScalar) && cn > 4 )
        ফেরত মিথ্যা;

#ifdef __APPLE__
    যদি (depth1 == CV_16U && (oclop == OCL_OP_MUL || oclop == OCL_OP_MUL_SCALE))
        ফেরত মিথ্যা;
#endif

    পূর্ণসংখ্যা dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32S, CV_MAT_DEPTH(wtype));
    যদি (!doubleSupport)
        wdepth = std::min(wdepth, CV_32F);

    wtype = CV_MAKETYPE(wdepth, cn);
    পূর্ণসংখ্যা type2 = haveScalar ? wtype : _src2.type(), depth2 = CV_MAT_DEPTH(type2);
    যদি (!doubleSupport && (depth2 == CV_64F || depth1 == CV_64F))
        ফেরত মিথ্যা;

    যদি (noOclType(depth1) || noOclType(depth2) || noOclType(ddepth) || noOclType(wdepth))
        ফেরত মিথ্যা;

    পূর্ণসংখ্যা kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    পূর্ণসংখ্যা scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = d.isIntel() ? 4 : 1;

    অক্ষর cvtstr[4][50], opts[1024];
    snprintf(opts, আকার(opts), "-D %s%s -D %s -D srcT1=%s -D srcT1_C1=%s -D srcT2=%s -D srcT2_C1=%s "
            "-D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workT=%s -D workST=%s -D scaleT=%s -D wdepth=%d -D convertToWT1=%s "
            "-D convertToWT2=%s -D convertToDT=%s%s -D cn=%d -D rowsPerWI=%d -D convertFromU=%s",
            (haveMask ? "MASK_" : ""), (haveScalar ? "UNARY_OP" : "BINARY_OP"),
            oclop2str[oclop], ocl::typeToStr(CV_MAKETYPE(depth1, kercn)),
            ocl::typeToStr(depth1), ocl::typeToStr(CV_MAKETYPE(depth2, kercn)),
            ocl::typeToStr(depth2), ocl::typeToStr(CV_MAKETYPE(ddepth, kercn)), ddepth,
            ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKETYPE(wdepth, kercn)),
            ocl::typeToStr(CV_MAKETYPE(wdepth, scalarcn)),
            ocl::typeToStr(wdepth), wdepth,
            ocl::convertTypeStr(depth1, wdepth, kercn, cvtstr[0], আকার(cvtstr[0])),
            ocl::convertTypeStr(depth2, wdepth, kercn, cvtstr[1], আকার(cvtstr[1])),
            ocl::convertTypeStr(wdepth, ddepth, kercn, cvtstr[2], আকার(cvtstr[2])),
            doubleSupport ? " -D DOUBLE_SUPPORT" : "", kercn, rowsPerWI,
            oclop == OCL_OP_ABSDIFF && wdepth == CV_32S && ddepth == wdepth ?
            ocl::convertTypeStr(CV_8U, ddepth, kercn, cvtstr[3], আকার(cvtstr[3])) : "noconvert");

    size_t usrdata_esz = CV_ELEM_আকার(wdepth);
    ধ্রুবক uchar* usrdata_p = (ধ্রুবক uchar*)usrdata;
    ধ্রুবক দ্বিগুণ* usrdata_d = (ধ্রুবক দ্বিগুণ*)usrdata;
    দশমিক usrdata_f[3];
    পূর্ণসংখ্যা i, n = oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE ||
        oclop == OCL_OP_RDIV_SCALE || oclop == OCL_OP_RECIP_SCALE ? 1 : oclop == OCL_OP_ADDW ? 3 : 0;
    যদি( usrdata && n > 0 && wdepth == CV_32F )
    {
        জন্য( i = 0; i < n; i++ )
            usrdata_f[i] = (দশমিক)usrdata_d[i];
        usrdata_p = (ধ্রুবক uchar*)usrdata_f;
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_আকার(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_আকার(mask, 1);

    যদি( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(wtype)*scalarcn;
        দ্বিগুণ buf[4]={0,0,0,0};
        Mat src2sc = _src2.getMat();

        যদি( !src2sc.empty() )
            convertAndUnrollScalar(src2sc, wtype, (uchar*)buf, 1);
        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        যদি( !haveMask )
        {
            যদি(n == 0)
                k.args(src1arg, dstarg, scalararg);
            অন্যথা যদি(n == 1)
                k.args(src1arg, dstarg, scalararg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            অন্যথা
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        অন্যথা
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    অন্যথা
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::Read_Only_No_আকার(src2, cn, kercn);

        যদি( !haveMask )
        {
            যদি (n == 0)
                k.args(src1arg, src2arg, dstarg);
            অন্যথা যদি (n == 1)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            অন্যথা যদি (n == 3)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz*2, usrdata_esz));
            অন্যথা
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        অন্যথা
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

#endif

// OpenCL branch of the arithmetic ops, extracted from the former arithm_op (whose CPU compute is now
// the element-wise engine). Returns true if an OpenCL kernel handled the op, false to let the caller
// fall through to the CPU engine. min/max use the bitwise-style kernel; every other op the arithm
// kernel with a coerced working type. `usrdata` carries the mul/div/addWeighted scale block, `oclop`
// the kernel id. Only reached for a UMat dst; sizes/types that don't fit the scalar/same-size pattern
// (e.g. a broadcast) decline (return false) so the engine handles them on the CPU.
স্থির যৌক্তিক arithm_op_ocl(প্রবেশ_Array _src1, প্রবেশ_Array _src2, OutputArray _dst,
                          প্রবেশ_Array _mask, পূর্ণসংখ্যা dtype, পূর্ণসংখ্যা oclop, যৌক্তিক muldiv, শূন্য* usrdata)
{
#ifndef HAVE_OPENCL
    CV_UNUSED(_src1); CV_UNUSED(_src2); CV_UNUSED(_dst); CV_UNUSED(_mask);
    CV_UNUSED(dtype); CV_UNUSED(oclop); CV_UNUSED(muldiv); CV_UNUSED(usrdata);
    ফেরত মিথ্যা;
#অন্যথা
    ধ্রুবক প্রবেশ_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    প্রবেশ_Array_2::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    যৌক্তিক haveMask = !_mask.empty();
    যৌক্তিক reallocate = মিথ্যা;
    পূর্ণসংখ্যা type1 = psrc1->type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    পূর্ণসংখ্যা type2 = psrc2->type(), depth2 = CV_MAT_DEPTH(type2), cn2 = CV_MAT_CN(type2);
    পূর্ণসংখ্যা wtype, dims1 = psrc1->dims(), dims2 = psrc2->dims();
    আকার_2 sz1 = dims1 <= 2 ? psrc1->size() : আকার_2();
    আকার_2 sz2 = dims2 <= 2 ? psrc2->size() : আকার_2();

    যদি (!(OCL_PERFORMANCE_CHECK(_dst.isUMat()) && dims1 <= 2 && dims2 <= 2))
        ফেরত মিথ্যা;

    // min/max: the bitwise-style OpenCL kernel (same-type element-wise, no working-type coercion).
    যদি (oclop == OCL_OP_MIN || oclop == OCL_OP_MAX)
    {
        যৌক্তিক haveScalar = মিথ্যা;
        যদি ((kind1 == প্রবেশ_Array_2::MATX) + (kind2 == প্রবেশ_Array_2::MATX) == 1 ||
            !psrc1->same_আকার(*psrc2) || type1 != type2)
        {
            যদি (checkScalar(*psrc1, type2, kind1, kind2))
            { std::swap(psrc1, psrc2); std::swap(type1, type2); }
            অন্যথা যদি (!checkScalar(*psrc2, type1, kind2, kind1))
                ফেরত মিথ্যা;
            haveScalar = সত্য;
        }
        _dst.create_Same_আকার(*psrc1, type1);
        ফেরত ocl_দ্বিমিক_op(*psrc1, *psrc2, _dst, _mask, মিথ্যা, oclop, haveScalar);
    }

    // reciprocal (scale/src): the OpenCL kernel is unary on src - use src2 for both operands (the CPU
    // engine received a 0-dim `1` numerator as src1, which the recip kernel would ignore). [TODO.VP: tidy]
    যদি (oclop == OCL_OP_RECIP_SCALE)
    {
        psrc1 = psrc2; kind1 = kind2; type1 = type2; depth1 = depth2; cn = cn2; dims1 = dims2; sz1 = sz2;
    }

    // add/sub/mul/div/absdiff/addWeighted/recip: the arithm OpenCL kernel with a coerced work type.
    যৌক্তিক src1Scalar = checkScalar(*psrc1, type2, kind1, kind2);
    যৌক্তিক src2Scalar = checkScalar(*psrc2, type1, kind2, kind1);
    যৌক্তিক haveScalar = মিথ্যা;

    যদি (dims1 != dims2 || sz1 != sz2 || cn != cn2 ||
        (kind1 == প্রবেশ_Array_2::MATX && (sz1 == আকার_2(1,4) || sz1 == আকার_2(1,1))) ||
        (kind2 == প্রবেশ_Array_2::MATX && (sz2 == আকার_2(1,4) || sz2 == আকার_2(1,1))))
    {
        যদি ((type1 == CV_64F && (sz1.height == 1 || sz1.height == 4)) && src1Scalar)
        {
            // src1 is a scalar; swap it with src2
            std::swap(psrc1, psrc2); std::swap(sz1, sz2); std::swap(type1, type2);
            std::swap(depth1, depth2); std::swap(cn, cn2); std::swap(dims1, dims2);
            যদি (oclop == OCL_OP_SUB) oclop = OCL_OP_RSUB;
            যদি (oclop == OCL_OP_DIV_SCALE) oclop = OCL_OP_RDIV_SCALE;
        }
        অন্যথা যদি (!src2Scalar)
            ফেরত মিথ্যা;   // array op array with mismatched size/cn: engine broadcasts on the CPU
        haveScalar = সত্য;
        CV_Assert((type2 == CV_64F || type2 == CV_32F) && (sz2.height == 1 || sz2.height == 4));

        যদি (!muldiv)
        {
            Mat sc = psrc2->getMat();
            depth2 = actualScalarDepth(sc.ptr<দ্বিগুণ>(), sz2 == আকার_2(1, 1) ? cn2 : cn);
            যদি (depth2 == CV_64F && CV_ELEM_SIZE1(depth1) < 8)
                depth2 = CV_32F;
        }
        অন্যথা
            depth2 = CV_64F;
    }

    যদি (dtype < 0)
    {
        যদি (_dst.fixedType())
            dtype = _dst.type();
        অন্যথা
        {
            যদি (!haveScalar && type1 != type2)
                CV_Error(cv::Error::StsBadArg,
                     "When the input arrays in add/subtract/multiply/divide functions have different types, "
                     "the output array type must be explicitly specified");
            dtype = type1;
        }
    }
    dtype = CV_MAT_DEPTH(dtype);

    যদি (depth1 == depth2 && dtype == depth1)
        wtype = dtype;
    অন্যথা যদি (!muldiv)
    {
        wtype = coerceTypes(depth1, depth2, মিথ্যা);
        wtype = coerceTypes(wtype, dtype, মিথ্যা);
        যদি (dtype < CV_32F && (depth1 < CV_32F || depth2 < CV_32F))
            wtype = CV_32S;
    }
    অন্যথা
    {
        wtype = coerceTypes(depth1, depth2, সত্য);
        wtype = coerceTypes(wtype, dtype, সত্য);
    }

    // The scaled OpenCL kernels (mul/div by a scale, reciprocal, addWeighted) compute in float; the old
    // same-type fast path forced max(depth, CV_32F). coerceTypes keeps same-type integer, so bump here.
    যদি (oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE || oclop == OCL_OP_RDIV_SCALE ||
        oclop == OCL_OP_RECIP_SCALE || oclop == OCL_OP_ADDW)
        wtype = std::max(wtype, (পূর্ণসংখ্যা)CV_32F);

    dtype = CV_MAKETYPE(dtype, cn);
    wtype = CV_MAKETYPE(wtype, cn);

    যদি (haveMask)
    {
        পূর্ণসংখ্যা mtype = _mask.type();
        CV_Assert((mtype == CV_8UC1 || mtype == CV_8SC1 || mtype == CV_Bool) && _mask.same_আকার(*psrc1));
        reallocate = !_dst.same_আকার(*psrc1) || _dst.type() != dtype;
    }

    _dst.create_Same_আকার(*psrc1, dtype);
    যদি (reallocate)
        _dst.setTo(0.);

    ফেরত ocl_arithm_op(*psrc1, *psrc2, _dst, _mask, wtype, usrdata, oclop, haveScalar);
#endif
}

// Element-wise binary op (ADD/SUB/...) on two CPU operands - each an array Mat or a MATX scalar -
// through the new broadcasting engine. Handles mixed input types (emitBinary inserts the casts) and
// broadcasting (a scalar rides as a 0-dim per-channel CONST; arrays broadcast via outputShape/exec).
// Returns false (declining) for UMat, a write-mask, or two scalar operands, so the caller falls
// through to arithm_op. Incompatible shapes make the engine throw (matching cv::add's error).
// Does the output array already have exactly this shape+type (=> create() reuses it, no realloc)?
// Works for Mat and UMat alike (shape/type queries, no data-pointer peeking).
স্থির যৌক্তিক outArrayMatches(ধ্রুবক _OutputArray& a, ধ্রুবক MatShape& shp, পূর্ণসংখ্যা type)
{
    যদি (a.empty() || a.type() != type)
        ফেরত মিথ্যা;
    পূর্ণসংখ্যা sz[MatShape::MAX_DIMS];
    পূর্ণসংখ্যা nd = a.sizend(sz);
    যদি (nd != (পূর্ণসংখ্যা)shp.size())
        ফেরত মিথ্যা;
    জন্য (পূর্ণসংখ্যা i = 0; i < nd; i++)
        যদি (sz[i] != shp[i])
            ফেরত মিথ্যা;
    ফেরত সত্য;
}

// Are two operands' iteration shapes (spatial dims + channels innermost, exactly how the engine
// iterates) numpy-broadcast-compatible? Powers the scalar-like-Mat compat fallback in arithm_op.
// Cheap by design: sizend copies the dim arrays inline - no Mat headers, no allocations.
স্থির যৌক্তিক shapesBroadcastCompat(প্রবেশ_Array a, পূর্ণসংখ্যা acn, প্রবেশ_Array b, পূর্ণসংখ্যা bcn)
{
    পূর্ণসংখ্যা asz[MatShape::MAX_DIMS + 1], bsz[MatShape::MAX_DIMS + 1];
    পূর্ণসংখ্যা ad = a.sizend(asz), bd = b.sizend(bsz);
    asz[ad++] = acn;
    bsz[bd++] = bcn;
    জন্য (পূর্ণসংখ্যা i1 = ad - 1, i2 = bd - 1; i1 >= 0 && i2 >= 0; i1--, i2--)
        যদি (asz[i1] != bsz[i2] && asz[i1] != 1 && bsz[i2] != 1)
            ফেরত মিথ্যা;
    ফেরত সত্য;
}

// The one entry point for every element-wise binary op (add/sub/mul/div/min/max/absdiff/addWeighted/
// reciprocal). Handles empty inputs, the UMat/OpenCL branch (arithm_op_ocl) and the CPU element-wise
// engine. `oclop` selects the OpenCL kernel; `muldiv` drives the OpenCL working-type coercion + scale;
// `params` are the op scalars (params[0]=mul/div scale; {alpha,beta,gamma} for addWeighted).
// Below this many ELEMENTS (total * channels), the per-call constant overhead of the engine path
// (program build + compile + executor setup) is comparable to the actual work - take the direct
// kernel call instead. Same tier boundary as math_op's MATH_OP_SMALL for cv::exp.
গণনা { ARITHM_SMALL_DIRECT = 100000 };

স্থির শূন্য arithm_op(ew::শীর্ষ op, প্রবেশ_Array src1, প্রবেশ_Array src2, OutputArray dst,
                      প্রবেশ_Array mask, পূর্ণসংখ্যা dtype, পূর্ণসংখ্যা oclop, যৌক্তিক muldiv, ধ্রুবক Scalar& params)
{
    যদি (src1.empty() || src2.empty())          // empty inputs -> empty result
    {
        CV_Assert((src1.empty() && src2.empty()) ||
                  (src1.empty() && isScalarArg(src2, src1.channels())) ||
                  (src2.empty() && isScalarArg(src1, src2.channels())));
        dst.release();
        যদি (dtype >= 0)
            dst.create(0, 0, dtype);
        ফেরত;
    }

    // UMat -> classic OpenCL kernel. It declines (false) when OpenCL can't apply (no device, dims>2, a
    // broadcast, cn>4 masked, ...) - then the CPU engine below handles it (mapping the UMat via getMat).
    // oclop < 0 is a sentinel: the caller (e.g. cv::compare) already ran its own OpenCL path, so this
    // helper only runs the CPU engine (a UMat operand is mapped via getMat).
    যদি (oclop >= 0 && cv::ocl::isOpenCLActivated() &&
        (src1.isUMat() || src2.isUMat() || dst.isUMat()))
    {
        দ্বিগুণ abg[3] = { params[0], params[1], params[2] }, scale = params[0];
        শূন্য* usrdata = (op == ew::OP_ADDW) ? (শূন্য*)abg : (muldiv ? (শূন্য*)&scale : nullptr);
        যদি (arithm_op_ocl(src1, src2, dst, mask, dtype, oclop, muldiv, usrdata))
            ফেরত;
    }

    ধ্রুবক যৌক্তিক haveMask = !mask.empty();
    ধ্রুবক পূর্ণসংখ্যা cn1 = src1.channels(), cn2 = src2.channels();
    যৌক্তিক s1 = isScalarArg(src1, src2), s2 = isScalarArg(src2, src1);
    যদি (s1 && s2)
        s1 = s2 = মিথ্যা;    // two scalars: treat both as tiny array operands (element-wise + broadcast),
                            // matching arithm_op (Scalar+Scalar -> 4x1, number+number -> 1x1, ...)
    // A 4x1 CV_64F Mat/UMat pseudo-scalar (see isScalarArg) is hijacked only against a REAL array:
    // when the partner is itself a tiny scalar-shaped array (e.g. compare(Mat 4x1, Mat 1x1) -
    // issue #8999), both are honest data and ride the broadcast. A genuine MATX Scalar never demotes.
    অন্যথা যদি (s1 && src1.kind() != প্রবেশ_Array_2::MATX && isScalarLikeMat(src2, cn1))
        s1 = মিথ্যা;
    অন্যথা যদি (s2 && src2.kind() != প্রবেশ_Array_2::MATX && isScalarLikeMat(src1, cn2))
        s2 = মিথ্যা;

    // Compat fallback: Java/Python/user code passes scalars as real little Mats (the classic 4x1
    // CV_64F column, a 1xcn/cnx1 vector - see isScalarLikeMat). Those normally ride broadcasting
    // now, so hijack one as a per-channel scalar ONLY when the shapes do NOT broadcast - just the
    // calls that would otherwise throw get the 4.x scalar semantics, every valid broadcast keeps
    // its numpy meaning. Cheap: two geometry probes, and the O(ndims) shape walk runs only when a
    // probe hits (never for ordinary same-size or MATX-scalar calls).
    যদি (!s1 && !s2)
    {
        যৌক্তিক like1 = isScalarLikeMat(src1, cn2), like2 = isScalarLikeMat(src2, cn1);
        যদি ((like1 || like2) && !shapesBroadcastCompat(src1, cn1, src2, cn2))
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
    ধ্রুবক শূন্য* dstObj = (dst.kind() == প্রবেশ_Array_2::MAT) ? dst.getObj() : nullptr;
    ধ্রুবক যৌক্তিক aliased = dstObj && (dstObj == src1.getObj() || dstObj == src2.getObj());
    Mat m1loc, m2loc, mloc;
    auto asMat = [&](প্রবেশ_Array a, Mat& loc) -> ধ্রুবক Mat* {
        যদি (a.kind() == প্রবেশ_Array_2::MAT && !aliased) ফেরত (ধ্রুবক Mat*)a.getObj();
        loc = a.getMat(); ফেরত &loc;
    };
    ধ্রুবক Mat* pm1 = s1 ? nullptr : asMat(src1, m1loc);
    ধ্রুবক Mat* pm2 = s2 ? nullptr : asMat(src2, m2loc);
    ধ্রুবক পূর্ণসংখ্যা adepth = s1 ? pm2->depth() : pm1->depth();   // the (first) array operand's depth
    // auto result depth (no explicit dtype, no fixed-type dst): add/sub/mul/div keep the first array
    // operand's depth (cv::'s dtype==-1 convention), but min/max/absdiff have NO dtype argument and
    // must promote mixed inputs to their common type (min(u32,f32) in u32 would drop the float). For
    // same-type inputs promoteArith(da,db)==da, so classic behaviour is preserved (e.g. absdiff of two
    // s16 stays s16, saturating - the wide unsigned |a-b| is computed then cast back down by emitBinary).
    পূর্ণসংখ্যা autoDepth = adepth;
    যদি (op == ew::OP_MIN || op == ew::OP_MAX || op == ew::OP_ABSDIFF)
    {
        ধ্রুবক পূর্ণসংখ্যা d1 = s1 ? ew::EW_DEPTH_NONE : pm1->depth();
        ধ্রুবক পূর্ণসংখ্যা d2 = s2 ? ew::EW_DEPTH_NONE : pm2->depth();
        autoDepth = ew::promoteArith(d1, d2);
    }
    // result depth: explicit dtype wins; else a fixed-type dst dictates it; else the auto depth above.
    ধ্রুবক পূর্ণসংখ্যা rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
                     : (dst.fixedType() ? dst.depth() : autoDepth);

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
    যদি (!haveMask && !s1 && !s2 &&
        (op == ew::OP_ADD || op == ew::OP_SUB || op == ew::OP_MIN || op == ew::OP_MAX ||
         op == ew::OP_ABSDIFF || op == ew::OP_MUL || op == ew::OP_ADDW ||
         op == ew::OP_AND || op == ew::OP_OR || op == ew::OP_XOR) &&
        rdepth == pm1->depth() && pm1->type() == pm2->type() && pm1->size == pm2->size &&
        pm1->isContinuous() && pm2->isContinuous() &&
        pm1->যোগ()*cn1 <= (size_t)ARITHM_SMALL_DIRECT)
    {
        ew::TKernel k = ew::getElemwiseFunc(op, rdepth, rdepth, ew::EW_DEPTH_NONE, rdepth);
        যদি (k.fptr)
        {
            dst.create_Same_আকার(src1, pm1->type());   // whole-shape transfer, not piecemeal dims+sizes
            Mat dloc;
            Mat* pd = (dst.kind() == প্রবেশ_Array_2::MAT) ? (Mat*)dst.getObj() : &(dloc = dst.getMat());
            যদি (pd->isContinuous())     // a reused non-continuous dst view falls through to the engine
            {
                k.fptr(pm1->তথ্য, 0, 1, pm2->তথ্য, 0, 1, nullptr, 0, 0,
                       pd->তথ্য, 0, (পূর্ণসংখ্যা)(pm1->যোগ()*cn1), 1, params.val, k.flags, k.userdata);
                ফেরত;
            }
        }
    }

    // operand order = (src1, src2): an array becomes an INPUT, a scalar a flexible per-channel CONST
    // read straight from the caller's inline storage (getObj()), no Mat / convertTo.
    ew::TExpr p;
    auto addOperand = [&](প্রবেশ_Array src, যৌক্তিক isScalar, ধ্রুবক Mat* m, পূর্ণসংখ্যা otherCn) -> পূর্ণসংখ্যা {
        যদি (!isScalar)
            ফেরত p.add_প্রবেশ(m->depth());
        ধ্রুবক uchar* sp; পূর্ণসংখ্যা sd;
        uchar scbuf[EW_SCALAR_BUF_আকার];   // stack room for a UMAT scalar; addConst copies the values out
        ধ্রুবক পূর্ণসংখ্যা scn0 = scalarArgElems(src, sp, sd, scbuf), cn = otherCn;
        ধ্রুবক পূর্ণসংখ্যা scn = (cn == scn0 || (cn < 4 && scn0 == 4)) ? cn
                      : (scn0 == 1) ? 1 : 0;
        CV_Assert(scn > 0);
        ফেরত p.addConst(ew::EW_DEPTH_NONE, sd, sp, scn);
    };
    ধ্রুবক পূর্ণসংখ্যা a0 = addOperand(src1, s1, pm1, cn2);
    ধ্রুবক পূর্ণসংখ্যা a1 = addOperand(src2, s2, pm2, cn1);

    // Write-mask: a single-channel 1-byte array (u8/s8/bool) the size of the output spatial shape,
    // added as another broadcast input. The arithmetic result lands in a temp; a final
    // select(mask, r, dst) -> dst overwrites only mask!=0 positions of the (pre-existing) output.
    পূর্ণসংখ্যা sMask = 0;
    ধ্রুবক Mat* pmask = nullptr;
    যদি (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = asMat(mask, mloc);
        sMask = p.add_প্রবেশ(pmask->depth());
    }
    ধ্রুবক পূর্ণসংখ্যা sOut = p.addOutput(rdepth);               // output BEFORE emit -> moveToOutput drops the
    ধ্রুবক পূর্ণসংখ্যা r = p.emit_দ্বিমিক(op, a0, a1, rdepth, params);   // result temp (mask-free => none); scale
                                                        // rides params[0] (mul/div), {a,b,g} for addW
    যদি (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    অন্যথা
        p.moveToOutput(r, sOut);
    p.compile();

    ধ্রুবক Mat* inputs[3]; পূর্ণসংখ্যা ni = 0;                   // inputs in program (input-index) order
    যদি (!s1) inputs[ni++] = pm1;
    যদি (!s2) inputs[ni++] = pm2;
    যদি (haveMask) inputs[ni++] = pmask;

    // Pre-create the output at the broadcast shape, ALWAYS via dst.create() - never hand exec a raw
    // dst Mat. Routing through the _OutputArray is what enforces its contract: a FIXED_SIZE/FIXED_TYPE
    // dst (in-place 'a += b' passes '(const Mat&)a') throws here if a broadcast would change its shape
    // or type (Mat(1x1) += Mat(1x4)), instead of silently reallocating - exec sees only a Mat and has
    // no view of the array's fixed flags. Cost over a direct exec is negligible: exec keeps its own
    // fast path for a matching-shape op, and dst.create() with an unchanged shape/type is a no-op.
    // With a mask, a freshly (re)allocated dst is zeroed so mask==0 reads 0 (matches arithm_op); a
    // reused dst keeps its prior content there - detect reuse by shape+type BEFORE create (correct for
    // a reused UMat dst too).
    MatShape oshape; পূর্ণসংখ্যা ocn;
    p.outputShape(inputs, oshape, ocn);
    ধ্রুবক পূর্ণসংখ্যা otype = CV_MAKETYPE(rdepth, ocn);
    ধ্রুবক যৌক্তিক reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == প্রবেশ_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    যদি (haveMask && !reused)
        pdst->setZero();                          // setZero (not setTo, which caps at 4 channels)
    p.exec(inputs, pdst);
}

// Unary element-wise op through the engine (bitwise NOT now; unary math later). One array input, an
// optional write-mask; result depth = dtype (>=0), else a fixed-type dst's depth, else the input
// depth. Mirrors arithm_op's build/mask/exec tail with a single input and no scalar operand. OpenCL is
// run by the caller (bitwise_op_ocl), so this is the CPU engine only.
স্থির শূন্য unary_op(ew::শীর্ষ op, প্রবেশ_Array src, OutputArray dst, প্রবেশ_Array mask,
                     পূর্ণসংখ্যা dtype, ধ্রুবক Scalar& params)
{
    যদি (src.empty())
    {
        dst.release();
        যদি (dtype >= 0)
            dst.create(0, 0, dtype);
        ফেরত;
    }

    ধ্রুবক যৌক্তিক haveMask = !mask.empty();
    // in-place (dst aliases src): keep a header copy so a shape/type-changing dst.create() can't free
    // src's data mid-op (matches arithm_op). A fixed-size/type dst never reallocs, so this is only for
    // an explicit dtype change.
    ধ্রুবক শূন্য* dstObj = (dst.kind() == প্রবেশ_Array_2::MAT) ? dst.getObj() : nullptr;
    ধ্রুবক যৌক্তিক aliased = dstObj && src.kind() == প্রবেশ_Array_2::MAT && dstObj == src.getObj();
    Mat sloc, mloc;
    ধ্রুবক Mat* psrc = (src.kind() == প্রবেশ_Array_2::MAT && !aliased) ? (ধ্রুবক Mat*)src.getObj()
                                                                   : &(sloc = src.getMat());
    ধ্রুবক পূর্ণসংখ্যা rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
                     : (dst.fixedType() ? dst.depth() : psrc->depth());

    ew::TExpr p;
    ধ্রুবক পূর্ণসংখ্যা a0 = p.add_প্রবেশ(psrc->depth());
    পূর্ণসংখ্যা sMask = 0;
    ধ্রুবক Mat* pmask = nullptr;
    যদি (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = (mask.kind() == প্রবেশ_Array_2::MAT) ? (ধ্রুবক Mat*)mask.getObj() : &(mloc = mask.getMat());
        sMask = p.add_প্রবেশ(pmask->depth());
    }
    ধ্রুবক পূর্ণসংখ্যা sOut = p.addOutput(rdepth);
    ধ্রুবক পূর্ণসংখ্যা r = p.emitUnary(op, a0, rdepth, params);
    যদি (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    অন্যথা
        p.moveToOutput(r, sOut);
    p.compile();

    ধ্রুবক Mat* inputs[2]; পূর্ণসংখ্যা ni = 0;
    inputs[ni++] = psrc;
    যদি (haveMask) inputs[ni++] = pmask;

    MatShape oshape; পূর্ণসংখ্যা ocn;
    p.outputShape(inputs, oshape, ocn);
    ধ্রুবক পূর্ণসংখ্যা otype = CV_MAKETYPE(rdepth, ocn);
    ধ্রুবক যৌক্তিক reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == প্রবেশ_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    যদি (haveMask && !reused)
        pdst->setZero();
    p.exec(inputs, pdst);
}

}

শূন্য cv::add( প্রবেশ_Array src1, প্রবেশ_Array src2, OutputArray dst,
          প্রবেশ_Array mask, পূর্ণসংখ্যা dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADD, src1, src2, dst, mask, dtype, OCL_OP_ADD, মিথ্যা);
}

শূন্য cv::subtract( প্রবেশ_Array src1, প্রবেশ_Array src2, OutputArray dst,
                   প্রবেশ_Array mask, পূর্ণসংখ্যা dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_SUB, src1, src2, dst, mask, dtype, OCL_OP_SUB, মিথ্যা);
}

শূন্য cv::absdiff( প্রবেশ_Array src1, প্রবেশ_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ABSDIFF, src1, src2, dst, noArray(), -1, OCL_OP_ABSDIFF, মিথ্যা);
}

শূন্য cv::copyTo(প্রবেশ_Array _src, OutputArray _dst, প্রবেশ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    _src.copyTo(_dst, _mask);
}

/****************************************************************************************\
*                                    multiply/divide                                     *
\****************************************************************************************/

নামাকাশ cv
{

শূন্য multiply(প্রবেশ_Array src1, প্রবেশ_Array src2,
              OutputArray dst, দ্বিগুণ scale, পূর্ণসংখ্যা dtype)
{
    CV_INSTRUMENT_REGION();

    ধ্রুবক পূর্ণসংখ্যা oclop = std::abs(scale - 1.0) < DBL_EPSILON ? OCL_OP_MUL : OCL_OP_MUL_SCALE;
    arithm_op(ew::OP_MUL, src1, src2, dst, noArray(), dtype, oclop, /*muldiv*/ সত্য, Scalar(scale));
}

শূন্য divide(প্রবেশ_Array src1, প্রবেশ_Array src2,
                OutputArray dst, দ্বিগুণ scale, পূর্ণসংখ্যা dtype)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_DIV, src1, src2, dst, noArray(), dtype, OCL_OP_DIV_SCALE, /*muldiv*/ সত্য, Scalar(scale));
}

শূন্য divide(দ্বিগুণ scale, প্রবেশ_Array src2,
                OutputArray dst, পূর্ণসংখ্যা dtype)
{
    CV_INSTRUMENT_REGION();

    যদি (src2.empty())
    {
        dst.release();
        ফেরত;
    }

    // scale / src2 == scale * 1 / src2: feed a 0-dim `1` as the numerator so the CPU engine reuses the
    // normal divide (scale*a/b). Give the numerator src2's OWN depth: then OP_DIV(T, T) is same-type and
    // takes the fast per-type kernel (f32 work for <=16-bit / f16 / bf16, not the s32->f64 path a CV_32S
    // numerator would force). An integer src2 keeps its div-by-zero->0 guard (both operands integer); a
    // float src2 divides as float (1/0->inf), both matching cv::divide. dtype<0 follows src2's depth (a
    // fixed-type dst still wins). The UMat path uses the dedicated reciprocal kernel (OCL_OP_RECIP_SCALE).
    দ্বিগুণ one = 1;
    Mat numerator(MatShape::scalar(), src2.depth(), &one);
    scalar_To_Raw_তথ্য(Scalar(1), &one, src2.depth(), 1);       // write `1` in src2's depth (no allocation)
    ধ্রুবক পূর্ণসংখ্যা rtype = (dtype < 0 && !dst.fixedType()) ? src2.depth() : dtype;
    arithm_op(ew::OP_DIV, numerator, src2, dst, noArray(), rtype, OCL_OP_RECIP_SCALE, /*muldiv*/ সত্য, Scalar(scale));
}

UMat UMat::mul(প্রবেশ_Array m, দ্বিগুণ scale) ধ্রুবক
{
    UMat dst;
    multiply(*এই, m, dst, scale);
    ফেরত dst;
}

/****************************************************************************************\
*                                      addWeighted                                       *
\****************************************************************************************/

}

শূন্য cv::addWeighted( প্রবেশ_Array src1, দ্বিগুণ alpha, প্রবেশ_Array src2,
                      দ্বিগুণ beta, দ্বিগুণ gamma, OutputArray dst, পূর্ণসংখ্যা dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADDW, src1, src2, dst, noArray(), dtype, OCL_OP_ADDW, /*muldiv*/ সত্য,
              Scalar(alpha, beta, gamma));
}


/****************************************************************************************\
*                                          compare                                       *
\****************************************************************************************/

নামাকাশ cv
{


স্থির দ্বিগুণ getMinVal(পূর্ণসংখ্যা depth)
{
    স্থির ধ্রুবক দ্বিগুণ tab[CV_DEPTH_MAX] =
    {
        0, -128, 0, -32768, INT_MIN, -FLT_MAX, -DBL_MAX,
        -65504, -FLT_MAX, 0, 0, (দ্বিগুণ)INT64_MIN, 0
    };
    ফেরত tab[depth];
}

স্থির দ্বিগুণ getMaxVal(পূর্ণসংখ্যা depth)
{
    স্থির ধ্রুবক দ্বিগুণ tab[CV_DEPTH_MAX] = {
        255, 127, 65535, 32767, INT_MAX, FLT_MAX, DBL_MAX,
        65504, FLT_MAX, 255, (দ্বিগুণ)UINT64_MAX, (দ্বিগুণ)INT64_MAX, (দ্বিগুণ)UINT32_MAX, 0
    };
    ফেরত tab[depth];
}

#ifdef HAVE_OPENCL

স্থির যৌক্তিক ocl_compare(প্রবেশ_Array _src1, প্রবেশ_Array _src2, OutputArray _dst, পূর্ণসংখ্যা op, যৌক্তিক haveScalar)
{
    ধ্রুবক ocl::Device& dev = ocl::Device::getDefault();
    যৌক্তিক doubleSupport = dev.doubleFPConfig() > 0;
    পূর্ণসংখ্যা type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1),
            type2 = _src2.type(), depth2 = CV_MAT_DEPTH(type2);

    যদি (!doubleSupport && depth1 == CV_64F)
        ফেরত মিথ্যা;

    // the integer scalar path below works in int; let the CPU handle the wider 5.x integer depths
    যদি (haveScalar && (depth1 == CV_32U || depth1 == CV_64U || depth1 == CV_64S))
        ফেরত মিথ্যা;

    যদি (!haveScalar && (!_src1.same_আকার(_src2) || type1 != type2))
            ফেরত মিথ্যা;

    // This OpenCL kernel broadcasts a scalar's FIRST channel to all channels. The CPU path compares
    // per-channel (scalar[c] for channel c), so a multichannel array against a scalar with DISTINCT
    // channel values would disagree - bail to the CPU engine in that case (rare; not worth a kernel fix).
    যদি (haveScalar && cn > 1)
    {
        Mat sc = _src2.getMat();
        পূর্ণসংখ্যা scn = (পূর্ণসংখ্যা)sc.যোগ();
        যদি (scn > 1)
        {
            দ্বিগুণ v[4] = { 0, 0, 0, 0 };
            getConvertFunc(sc.depth(), CV_64F)(sc.ptr(), 1, 0, 1, (uchar*)v, 1, আকার_2(std::min(scn, 4), 1), 0);
            জন্য (পূর্ণসংখ্যা i = 1, n = std::min(cn, scn); i < n; i++)
                যদি (v[i] != v[0])
                    ফেরত মিথ্যা;
        }
    }

    পূর্ণসংখ্যা kercn = haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst), rowsPerWI = dev.isIntel() ? 4 : 1;
    // Workaround for bug with "?:" operator in AMD OpenCL compiler
    যদি (depth1 >= CV_16U)
        kercn = 1;

    পূর্ণসংখ্যা scalarcn = kercn == 3 ? 4 : kercn;
    ধ্রুবক অক্ষর * ধ্রুবক operationMap[] = { "==", ">", ">=", "<", "<=", "!=" };
    অক্ষর cvt[50];

    String opts = format("-D %s -D srcT1=%s -D dstT=%s -D DEPTH_dst=%d -D workT=srcT1 -D cn=%d"
                         " -D convertToDT=%s -D OP_CMP -D CMP_OPERATOR=%s -D srcT1_C1=%s"
                         " -D srcT2_C1=%s -D dstT_C1=%s -D workST=%s -D rowsPerWI=%d%s",
                         haveScalar ? "UNARY_OP" : "BINARY_OP",
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, kercn)),
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U, kercn,
                         ocl::convertTypeStr(depth1, CV_8U, kercn, cvt, আকার(cvt)),
                         operationMap[op], ocl::typeToStr(depth1),
                         ocl::typeToStr(depth1), ocl::typeToStr(CV_8U),
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, scalarcn)), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat src1 = _src1.getUMat();
    আকার_2 size = src1.size();
    _dst.create(size, CV_8UC(cn));
    UMat dst = _dst.getUMat();

    যদি (haveScalar)
    {
        size_t esz = CV_ELEM_SIZE1(type1) * scalarcn;
        দ্বিগুণ buf[4] = { 0, 0, 0, 0 };
        Mat src2 = _src2.getMat();

        যদি( depth1 > CV_32S )
            convertAndUnrollScalar( src2, depth1, (uchar *)buf, kercn );
        অন্যথা
        {
            দ্বিগুণ fval = 0;
            getConvertFunc(depth2, CV_64F)(src2.ptr(), 1, 0, 1, (uchar *)&fval, 1, আকার_2(1, 1), 0);
            যদি( fval < getMinVal(depth1) )
                ফেরত dst.setTo(Scalar::all(op == CMP_GT || op == CMP_GE || op == CMP_NE ? 255 : 0)), সত্য;

            যদি( fval > getMaxVal(depth1) )
                ফেরত dst.setTo(Scalar::all(op == CMP_LT || op == CMP_LE || op == CMP_NE ? 255 : 0)), সত্য;

            পূর্ণসংখ্যা ival = cvRound(fval);
            যদি( fval != ival )
            {
                যদি( op == CMP_LT || op == CMP_GE )
                    ival = cvCeil(fval);
                অন্যথা যদি( op == CMP_LE || op == CMP_GT )
                    ival = cvFloor(fval);
                অন্যথা
                    ফেরত dst.setTo(Scalar::all(op == CMP_NE ? 255 : 0)), সত্য;
            }
            convertAndUnrollScalar(Mat(1, 1, CV_32S, &ival), depth1, (uchar *)buf, kercn);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        k.args(ocl::KernelArg::Read_Only_No_আকার(src1, cn, kercn),
               ocl::KernelArg::WriteOnly(dst, cn, kercn), scalararg);
    }
    অন্যথা
    {
        UMat src2 = _src2.getUMat();

        k.args(ocl::KernelArg::Read_Only_No_আকার(src1),
               ocl::KernelArg::Read_Only_No_আকার(src2),
               ocl::KernelArg::WriteOnly(dst, cn, kercn));
    }

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

#endif

}

শূন্য cv::compare(প্রবেশ_Array _src1, প্রবেশ_Array _src2, OutputArray _dst, পূর্ণসংখ্যা op)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( op == CMP_LT || op == CMP_LE || op == CMP_EQ ||
               op == CMP_NE || op == CMP_GE || op == CMP_GT );

    CV_Assert(_src1.empty() == _src2.empty());
    যদি (_src1.empty() && _src2.empty())
    {
        _dst.release();
        ফেরত;
    }

    যৌক্তিক haveScalar = মিথ্যা;

    যদি ((_src1.isMatx() + _src2.isMatx()) == 1
            || !_src1.same_আকার(_src2)
            || _src1.type() != _src2.type())
    {
        যৌক্তিক is_src1_scalar = checkScalar(_src1, _src2.type(), _src1.kind(), _src2.kind());
        যৌক্তিক is_src2_scalar = checkScalar(_src2, _src1.type(), _src2.kind(), _src1.kind());

        // exactly one scalar operand: keep it as src2 (swapping flips the ordering ops) so the OpenCL
        // path sees the array-op-scalar form. Otherwise - two arrays of different size (broadcast),
        // mixed types, or two scalars - the CPU engine broadcasts / promotes them, so there is NO error
        // here: fall through to arithm_op (which handles scalar detection, broadcast and mixed types).
        যদি (is_src1_scalar && !is_src2_scalar)
        {
            op = op == CMP_LT ? CMP_GT : op == CMP_LE ? CMP_GE :
                op == CMP_GE ? CMP_LE : op == CMP_GT ? CMP_LT : op;
            // src1 is a scalar; swap it with src2
            compare(_src2, _src1, _dst, op);
            ফেরত;
        }
        haveScalar = is_src2_scalar && !is_src1_scalar;
    }
    CV_UNUSED(haveScalar);   // consumed by CV_OCL_RUN only - a no-op without OpenCL

    // OpenCL handles only the same-size and array-op-scalar forms; a broadcast between two arrays of
    // different size falls through to the CPU engine.
    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat())
               && (haveScalar || _src1.same_আকার(_src2)),
               ocl_compare(_src1, _src2, _dst, op, haveScalar))

    // CPU: the element-wise engine. A CMP_* code maps to the corresponding OP_CMP_* TOp; the engine
    // forces a u8 mask (dtype = CV_8U) and compares in the common operand type (a fractional scalar
    // threshold promotes both operands to float - matching the classic ceil/floor threshold logic).
    // The scalar / broadcast / mixed-shape cases are handled inside arithm_op. oclop = -1 tells it the
    // OpenCL path was already tried above (via ocl_compare), so it only runs the CPU engine.
    ew::শীর্ষ cmpOp = op == CMP_EQ ? ew::OP_CMP_EQ : op == CMP_NE ? ew::OP_CMP_NE :
                    op == CMP_LT ? ew::OP_CMP_LT : op == CMP_LE ? ew::OP_CMP_LE :
                    op == CMP_GT ? ew::OP_CMP_GT : ew::OP_CMP_GE;
    arithm_op(cmpOp, _src1, _src2, _dst, noArray(), CV_8U, /*oclop=*/-1, /*muldiv=*/মিথ্যা);
}

/****************************************************************************************\
*                                        inRange                                         *
\****************************************************************************************/

নামাকাশ cv
{

template <typename T>
কাঠামো InRange_SIMD
{
    পূর্ণসংখ্যা operator () (ধ্রুবক T * src1, ধ্রুবক T * src2, ধ্রুবক T * src3, uchar * dst, পূর্ণসংখ্যা len) ধ্রুবক
    {
        জন্য (পূর্ণসংখ্যা x = 0; x < len; x++)
            dst[x] = (src2[x] <= src1[x] && src1[x] <= src3[x]) ? 255 : 0;
        ফেরত len;
    }
};

#যদি (CV_SIMD || CV_SIMD_SCALABLE)

template <>
কাঠামো InRange_SIMD<uchar>
{
    পূর্ণসংখ্যা operator () (ধ্রুবক uchar * src1, ধ্রুবক uchar * src2, ধ্রুবক uchar * src3,
        uchar * dst, পূর্ণসংখ্যা len) ধ্রুবক
    {
        পূর্ণসংখ্যা x = 0;
        ধ্রুবক পূর্ণসংখ্যা width = VTraits<v_uint8>::vlanes();

        জন্য (; x <= len - width; x += width)
        {
            v_uint8 values = vx_load(src1 + x);
            v_uint8 নিচের = vx_load(src2 + x);
            v_uint8 উপরের = vx_load(src3 + x);

            v_store(dst + x, v_and(v_ge(values, নিচের), v_ge(উপরের, values)));
        }
        vx_cleanup();
        ফেরত x;
    }
};

template <>
কাঠামো InRange_SIMD<schar>
{
    পূর্ণসংখ্যা operator () (ধ্রুবক schar * src1, ধ্রুবক schar * src2, ধ্রুবক schar * src3,
        uchar * dst, পূর্ণসংখ্যা len) ধ্রুবক
    {
        পূর্ণসংখ্যা x = 0;
        ধ্রুবক পূর্ণসংখ্যা width = VTraits<v_int8>::vlanes();

        জন্য (; x <= len - width; x += width)
        {
            v_int8 values = vx_load(src1 + x);
            v_int8 নিচের = vx_load(src2 + x);
            v_int8 উপরের = vx_load(src3 + x);

            v_store((schar*)(dst + x), v_and(v_ge(values, নিচের), v_ge(উপরের, values)));
        }
        vx_cleanup();
        ফেরত x;
    }
};

template <>
কাঠামো InRange_SIMD<ushort>
{
    পূর্ণসংখ্যা operator () (ধ্রুবক ushort * src1, ধ্রুবক ushort * src2, ধ্রুবক ushort * src3,
        uchar * dst, পূর্ণসংখ্যা len) ধ্রুবক
    {
        পূর্ণসংখ্যা x = 0;
        ধ্রুবক পূর্ণসংখ্যা width = VTraits<v_uint16>::vlanes() * 2;

        জন্য (; x <= len - width; x += width)
        {
            v_uint16 values1 = vx_load(src1 + x);
            v_uint16 low1 = vx_load(src2 + x);
            v_uint16 high1 = vx_load(src3 + x);

            v_uint16 values2 = vx_load(src1 + x + VTraits<v_uint16>::vlanes());
            v_uint16 low2 = vx_load(src2 + x + VTraits<v_uint16>::vlanes());
            v_uint16 high2 = vx_load(src3 + x + VTraits<v_uint16>::vlanes());

            v_store(dst + x, v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2))));
        }
        vx_cleanup();
        ফেরত x;
    }
};

template <>
কাঠামো InRange_SIMD<short>
{
    পূর্ণসংখ্যা operator () (ধ্রুবক short * src1, ধ্রুবক short * src2, ধ্রুবক short * src3,
        uchar * dst, পূর্ণসংখ্যা len) ধ্রুবক
    {
        পূর্ণসংখ্যা x = 0;
        ধ্রুবক পূর্ণসংখ্যা width = (পূর্ণসংখ্যা)VTraits<v_int16>::vlanes() * 2;

        জন্য (; x <= len - width; x += width)
        {
            v_int16 values1 = vx_load(src1 + x);
            v_int16 low1 = vx_load(src2 + x);
            v_int16 high1 = vx_load(src3 + x);

            v_int16 values2 = vx_load(src1 + x + VTraits<v_int16>::vlanes());
            v_int16 low2 = vx_load(src2 + x + VTraits<v_int16>::vlanes());
            v_int16 high2 = vx_load(src3 + x + VTraits<v_int16>::vlanes());

            v_store((schar*)(dst + x), v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2))));
        }
        vx_cleanup();
        ফেরত x;
    }
};

template <>
কাঠামো InRange_SIMD<পূর্ণসংখ্যা>
{
    পূর্ণসংখ্যা operator () (ধ্রুবক পূর্ণসংখ্যা * src1, ধ্রুবক পূর্ণসংখ্যা * src2, ধ্রুবক পূর্ণসংখ্যা * src3,
        uchar * dst, পূর্ণসংখ্যা len) ধ্রুবক
    {
        পূর্ণসংখ্যা x = 0;
        ধ্রুবক পূর্ণসংখ্যা width = (পূর্ণসংখ্যা)VTraits<v_int32>::vlanes() * 2;

        জন্য (; x <= len - width; x += width)
        {
            v_int32 values1 = vx_load(src1 + x);
            v_int32 low1 = vx_load(src2 + x);
            v_int32 high1 = vx_load(src3 + x);

            v_int32 values2 = vx_load(src1 + x + VTraits<v_int32>::vlanes());
            v_int32 low2 = vx_load(src2 + x + VTraits<v_int32>::vlanes());
            v_int32 high2 = vx_load(src3 + x + VTraits<v_int32>::vlanes());

            v_pack_store(dst + x, v_reinterpret_as_u16(v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2)))));
        }
        vx_cleanup();
        ফেরত x;
    }
};

template <>
কাঠামো InRange_SIMD<দশমিক>
{
    পূর্ণসংখ্যা operator () (ধ্রুবক দশমিক * src1, ধ্রুবক দশমিক * src2, ধ্রুবক দশমিক * src3,
        uchar * dst, পূর্ণসংখ্যা len) ধ্রুবক
    {
        পূর্ণসংখ্যা x = 0;
        ধ্রুবক পূর্ণসংখ্যা width = (পূর্ণসংখ্যা)VTraits<v_float32>::vlanes() * 2;

        জন্য (; x <= len - width; x += width)
        {
            v_float32 values1 = vx_load(src1 + x);
            v_float32 low1 = vx_load(src2 + x);
            v_float32 high1 = vx_load(src3 + x);

            v_float32 values2 = vx_load(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_load(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_load(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        ফেরত x;
    }
};

template <>
কাঠামো InRange_SIMD<hfloat>
{
    পূর্ণসংখ্যা operator () (ধ্রুবক hfloat * src1, ধ্রুবক hfloat * src2, ধ্রুবক hfloat * src3,
        uchar * dst, পূর্ণসংখ্যা len) ধ্রুবক
    {
        পূর্ণসংখ্যা x = 0;
        ধ্রুবক পূর্ণসংখ্যা width = (পূর্ণসংখ্যা)VTraits<v_float32>::vlanes()*2;

        জন্য (; x <= len - width; x += width)
        {
            v_float32 values1 = vx_load_expand(src1 + x);
            v_float32 low1 = vx_load_expand(src2 + x);
            v_float32 high1 = vx_load_expand(src3 + x);

            v_float32 values2 = vx_load_expand(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_load_expand(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_load_expand(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        ফেরত x;
    }
};

template <>
কাঠামো InRange_SIMD<bfloat>
{
    পূর্ণসংখ্যা operator () (ধ্রুবক bfloat * src1, ধ্রুবক bfloat * src2, ধ্রুবক bfloat * src3,
        uchar * dst, পূর্ণসংখ্যা len) ধ্রুবক
    {
        পূর্ণসংখ্যা x = 0;
        ধ্রুবক পূর্ণসংখ্যা width = (পূর্ণসংখ্যা)VTraits<v_float32>::vlanes()*2;

        জন্য (; x <= len - width; x += width)
        {
            v_float32 values1 = vx_load_expand(src1 + x);
            v_float32 low1 = vx_load_expand(src2 + x);
            v_float32 high1 = vx_load_expand(src3 + x);

            v_float32 values2 = vx_load_expand(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_load_expand(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_load_expand(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        ফেরত x;
    }
};

template <>
কাঠামো InRange_SIMD<unsigned>
{
    পূর্ণসংখ্যা operator () (ধ্রুবক unsigned * src1, ধ্রুবক unsigned * src2, ধ্রুবক unsigned * src3,
        uchar * dst, পূর্ণসংখ্যা len) ধ্রুবক
    {
        পূর্ণসংখ্যা x = 0;
        ধ্রুবক পূর্ণসংখ্যা width = (পূর্ণসংখ্যা)VTraits<v_uint32>::vlanes() * 2;

        জন্য (; x <= len - width; x += width)
        {
            v_uint32 values1 = vx_load(src1 + x);
            v_uint32 low1 = vx_load(src2 + x);
            v_uint32 high1 = vx_load(src3 + x);

            v_uint32 values2 = vx_load(src1 + x + VTraits<v_uint32>::vlanes());
            v_uint32 low2 = vx_load(src2 + x + VTraits<v_uint32>::vlanes());
            v_uint32 high2 = vx_load(src3 + x + VTraits<v_uint32>::vlanes());

            v_pack_store(dst + x, v_reinterpret_as_u16(v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2)))));
        }
        vx_cleanup();
        ফেরত x;
    }
};

#যদি CV_SIMD_64F

template <>
কাঠামো InRange_SIMD<দ্বিগুণ>
{
    পূর্ণসংখ্যা operator () (ধ্রুবক দ্বিগুণ * src1, ধ্রুবক দ্বিগুণ * src2, ধ্রুবক দ্বিগুণ * src3,
        uchar * dst, পূর্ণসংখ্যা len) ধ্রুবক
    {
        পূর্ণসংখ্যা x = 0;
        ধ্রুবক পূর্ণসংখ্যা step = VTraits<v_float64>::vlanes();
        ধ্রুবক পূর্ণসংখ্যা width = step * 4;

        জন্য (; x <= len - width; x += width)
        {
            v_float64 v1 = vx_load(src1 + x);
            v_float64 l1 = vx_load(src2 + x);
            v_float64 h1 = vx_load(src3 + x);
            v_uint64 m1 = v_reinterpret_as_u64(v_and(v_ge(v1, l1), v_ge(h1, v1)));

            v_float64 v2 = vx_load(src1 + x + step);
            v_float64 l2 = vx_load(src2 + x + step);
            v_float64 h2 = vx_load(src3 + x + step);
            v_uint64 m2 = v_reinterpret_as_u64(v_and(v_ge(v2, l2), v_ge(h2, v2)));

            v_float64 v3 = vx_load(src1 + x + step * 2);
            v_float64 l3 = vx_load(src2 + x + step * 2);
            v_float64 h3 = vx_load(src3 + x + step * 2);
            v_uint64 m3 = v_reinterpret_as_u64(v_and(v_ge(v3, l3), v_ge(h3, v3)));

            v_float64 v4 = vx_load(src1 + x + step * 3);
            v_float64 l4 = vx_load(src2 + x + step * 3);
            v_float64 h4 = vx_load(src3 + x + step * 3);
            v_uint64 m4 = v_reinterpret_as_u64(v_and(v_ge(v4, l4), v_ge(h4, v4)));


            v_pack_store(dst + x, v_pack(v_pack(m1, m2), v_pack(m3, m4)));
        }
        vx_cleanup();
        ফেরত x;
    }
};

#endif

#endif

template <typename T>
স্থির শূন্য inRange_(ধ্রুবক শূন্য* _src1, size_t step1, ধ্রুবক শূন্য* _src2, size_t step2,
         ধ্রুবক শূন্য* _src3, size_t step3, uchar* dst, size_t step,
         আকার_2 size)
{
    ধ্রুবক T* src1 = (ধ্রুবক T*)_src1;
    ধ্রুবক T* src2 = (ধ্রুবক T*)_src2;
    ধ্রুবক T* src3 = (ধ্রুবক T*)_src3;
    step1 /= আকার(src1[0]);
    step2 /= আকার(src2[0]);
    step3 /= আকার(src3[0]);

    InRange_SIMD<T> vop;

    জন্য( ; size.height--; src1 += step1, src2 += step2, src3 += step3, dst += step )
    {
        পূর্ণসংখ্যা x = vop(src1, src2, src3, dst, size.width);
        #যদি CV_ENABLE_UNROLLED
        জন্য( ; x <= size.width - 4; x += 4 )
        {
            পূর্ণসংখ্যা t0, t1;
            t0 = src2[x] <= src1[x] && src1[x] <= src3[x];
            t1 = src2[x+1] <= src1[x+1] && src1[x+1] <= src3[x+1];
            dst[x] = (uchar)-t0; dst[x+1] = (uchar)-t1;
            t0 = src2[x+2] <= src1[x+2] && src1[x+2] <= src3[x+2];
            t1 = src2[x+3] <= src1[x+3] && src1[x+3] <= src3[x+3];
            dst[x+2] = (uchar)-t0; dst[x+3] = (uchar)-t1;
        }
        #endif
        জন্য( ; x < size.width; x++ )
            dst[x] = (uchar)-(src2[x] <= src1[x] && src1[x] <= src3[x]);
    }
}

স্থির শূন্য inRangeReduce(ধ্রুবক uchar* src, uchar* dst, size_t len, পূর্ণসংখ্যা cn)
{
    পূর্ণসংখ্যা k = cn % 4 ? cn % 4 : 4;
    size_t i, j;
    যদি( k == 1 )
        জন্য( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j];
    অন্যথা যদি( k == 2 )
        জন্য( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1];
    অন্যথা যদি( k == 3 )
        জন্য( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2];
    অন্যথা
        জন্য( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2] & src[j+3];

    জন্য( ; k < cn; k += 4 )
    {
        জন্য( i = 0, j = k; i < len; i++, j += cn )
            dst[i] &= src[j] & src[j+1] & src[j+2] & src[j+3];
    }
}

প্রকারলেখ শূন্য (*InRangeFunc)( ধ্রুবক শূন্য* src1, size_t step1, ধ্রুবক শূন্য* src2, size_t step2,
                             ধ্রুবক শূন্য* src3, size_t step3, uchar* dst, size_t step, আকার_2 sz );

স্থির InRangeFunc getInRangeFunc(পূর্ণসংখ্যা depth)
{
    স্থির InRangeFunc inRangeTab[CV_DEPTH_MAX] =
    {
        inRange_<uchar>,
        inRange_<schar>,
        inRange_<ushort>,
        inRange_<short>,
        inRange_<পূর্ণসংখ্যা>,
        inRange_<দশমিক>,
        inRange_<দ্বিগুণ>,
        inRange_<hfloat>,
        inRange_<bfloat>,
        0,
        inRange_<uint64_t>,
        inRange_<int64_t>,
        inRange_<unsigned>,
        0,
    };

    ফেরত inRangeTab[depth];
}

#ifdef HAVE_OPENCL

স্থির যৌক্তিক ocl_inRange( প্রবেশ_Array _src, প্রবেশ_Array _lowerb,
                         প্রবেশ_Array _upperb, OutputArray _dst )
{
    ধ্রুবক ocl::Device & d = ocl::Device::getDefault();
    প্রবেশ_Array_2::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    আকার_2 ssize = _src.size(), lsize = _lowerb.size(), usize = _upperb.size();
    পূর্ণসংখ্যা stype = _src.type(), ltype = _lowerb.type(), utype = _upperb.type();
    পূর্ণসংখ্যা sdepth = CV_MAT_DEPTH(stype), ldepth = CV_MAT_DEPTH(ltype), udepth = CV_MAT_DEPTH(utype);
    পূর্ণসংখ্যা cn = CV_MAT_CN(stype), rowsPerWI = d.isIntel() ? 4 : 1;
    যৌক্তিক lbScalar = মিথ্যা, ubScalar = মিথ্যা;

    যদি( (lkind == প্রবেশ_Array_2::MATX && skind != প্রবেশ_Array_2::MATX) ||
        ssize != lsize || stype != ltype )
    {
        যদি( !checkScalar(_lowerb, stype, lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = সত্য;
    }

    যদি( (ukind == প্রবেশ_Array_2::MATX && skind != প্রবেশ_Array_2::MATX) ||
        ssize != usize || stype != utype )
    {
        যদি( !checkScalar(_upperb, stype, ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = সত্য;
    }

    যদি (lbScalar != ubScalar)
        ফেরত মিথ্যা;

    যৌক্তিক doubleSupport = d.doubleFPConfig() > 0,
            haveScalar = lbScalar && ubScalar;

    যদি ( (!doubleSupport && sdepth == CV_64F) ||
         (!haveScalar && (sdepth != ldepth || sdepth != udepth)) )
        ফেরত মিথ্যা;

    পূর্ণসংখ্যা kercn = haveScalar ? cn : std::max(std::min(ocl::predictOptimalVectorWidth(_src, _lowerb, _upperb, _dst), 4), cn);
    যদি (kercn % cn != 0)
        kercn = cn;
    পূর্ণসংখ্যা colsPerWI = kercn / cn;
    String opts = format("%s-D CN=%d -D SRC_T=%s -D SRC_T1=%s -D DST_T=%s -D KERCN=%d -D DEPTH=%d%s -D COLS_PER_WI=%d",
                           haveScalar ? "-D HAVE_SCALAR " : "", cn, ocl::typeToStr(CV_MAKE_TYPE(sdepth, kercn)),
                           ocl::typeToStr(sdepth), ocl::typeToStr(CV_8UC(colsPerWI)), kercn, sdepth,
                           doubleSupport ? " -D DOUBLE_SUPPORT" : "", colsPerWI);

    ocl::Kernel ker("inrange", ocl::core::inrange_oclsrc, opts);
    যদি (ker.empty())
        ফেরত মিথ্যা;

    _dst.create(ssize, CV_8UC1);
    UMat src = _src.getUMat(), dst = _dst.getUMat(), lscalaru, uscalaru;
    Mat lscalar, uscalar;

    যদি (lbScalar && ubScalar)
    {
        lscalar = _lowerb.getMat();
        uscalar = _upperb.getMat();

        size_t esz = src.elem_আকার();
        size_t blocksize = 36;

        Auto_বাফার<uchar> _buf(blocksize*(((পূর্ণসংখ্যা)lbScalar + (পূর্ণসংখ্যা)ubScalar)*esz + cn) + 2*cn*আকার(পূর্ণসংখ্যা) + 128);
        uchar *buf = alignPtr(_buf.তথ্য() + blocksize*cn, 16);

        যদি( ldepth != sdepth && sdepth < CV_32S )
        {
            পূর্ণসংখ্যা* ilbuf = (পূর্ণসংখ্যা*)alignPtr(buf + blocksize*esz, 16);
            পূর্ণসংখ্যা* iubuf = ilbuf + cn;

            দ্বিমিক_Func sccvtfunc = getConvertFunc(ldepth, CV_32S);
            sccvtfunc(lscalar.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, আকার_2(cn, 1), 0);
            sccvtfunc(uscalar.ptr(), 1, 0, 1, (uchar*)iubuf, 1, আকার_2(cn, 1), 0);
            পূর্ণসংখ্যা minval = cvRound(getMinVal(sdepth)), maxval = cvRound(getMaxVal(sdepth));

            জন্য( পূর্ণসংখ্যা k = 0; k < cn; k++ )
            {
                যদি( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lscalar = Mat(cn, 1, CV_32S, ilbuf);
            uscalar = Mat(cn, 1, CV_32S, iubuf);
        }

        lscalar.convertTo(lscalar, stype);
        uscalar.convertTo(uscalar, stype);
    }
    অন্যথা
    {
        lscalaru = _lowerb.getUMat();
        uscalaru = _upperb.getUMat();
    }

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_আকার(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, 1, colsPerWI);

    যদি (haveScalar)
    {
        lscalar.copyTo(lscalaru);
        uscalar.copyTo(uscalaru);

        ker.args(srcarg, dstarg, ocl::KernelArg::PtrReadOnly(lscalaru),
               ocl::KernelArg::PtrReadOnly(uscalaru), rowsPerWI);
    }
    অন্যথা
        ker.args(srcarg, dstarg, ocl::KernelArg::Read_Only_No_আকার(lscalaru),
               ocl::KernelArg::Read_Only_No_আকার(uscalaru), rowsPerWI);

    size_t globalsize[2] = { (size_t)ssize.width / colsPerWI, ((size_t)ssize.height + rowsPerWI - 1) / rowsPerWI };
    ফেরত ker.run(2, globalsize, ফাঁকা, মিথ্যা);
}

#endif

}

শূন্য cv::inRange(প্রবেশ_Array _src, প্রবেশ_Array _lowerb,
                 প্রবেশ_Array _upperb, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(! _src.empty());
    যদি (!getInRangeFunc(_src.depth()))
        CV_Error( cv::Error::StsNotImplemented,
                 "inRange: CV_Bool and CV_8F inputs are not supported, convert the input to another type first");

    CV_OCL_RUN(_src.dims() <= 2 && _lowerb.dims() <= 2 &&
               _upperb.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat()),
               ocl_inRange(_src, _lowerb, _upperb, _dst))

    প্রবেশ_Array_2::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Mat src = _src.getMat(), lb = _lowerb.getMat(), ub = _upperb.getMat();

    যৌক্তিক lbScalar = মিথ্যা, ubScalar = মিথ্যা;

    যদি( (lkind == প্রবেশ_Array_2::MATX && skind != প্রবেশ_Array_2::MATX) ||
        src.size != lb.size || src.type() != lb.type() )
    {
        যদি( !checkScalar(lb, src.type(), lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = সত্য;
    }

    যদি( (ukind == প্রবেশ_Array_2::MATX && skind != প্রবেশ_Array_2::MATX) ||
        src.size != ub.size || src.type() != ub.type() )
    {
        যদি( !checkScalar(ub, src.type(), ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = সত্য;
    }

    CV_Assert(lbScalar == ubScalar);

    পূর্ণসংখ্যা cn = src.channels(), depth = src.depth();

    size_t esz = src.elem_আকার();
    size_t blocksize0 = (size_t)(BLOCK_আকার + esz-1)/esz;

    _dst.create_Same_আকার(_src, CV_8UC1);
    Mat dst = _dst.getMat();
    InRangeFunc func = getInRangeFunc(depth);

    ধ্রুবক Mat* arrays_sc[] = { &src, &dst, 0 };
    ধ্রুবক Mat* arrays_nosc[] = { &src, &dst, &lb, &ub, 0 };
    uchar* ptrs[4] = {};

    NAryMatIterator it(lbScalar && ubScalar ? arrays_sc : arrays_nosc, ptrs);
    size_t যোগ = it.size, blocksize = std::min(যোগ, blocksize0);

    Auto_বাফার<uchar> _buf(blocksize*(((পূর্ণসংখ্যা)lbScalar + (পূর্ণসংখ্যা)ubScalar)*esz + cn) + 2*cn*আকার(পূর্ণসংখ্যা) + 128);
    uchar *buf = _buf.তথ্য(), *mbuf = buf, *lbuf = 0, *ubuf = 0;
    buf = alignPtr(buf + blocksize*cn, 16);

    যদি( lbScalar && ubScalar )
    {
        lbuf = buf;
        ubuf = buf = alignPtr(buf + blocksize*esz, 16);

        CV_Assert( lb.type() == ub.type() );
        পূর্ণসংখ্যা scdepth = lb.depth();

        যদি( scdepth != depth && depth < CV_32S )
        {
            পূর্ণসংখ্যা* ilbuf = (পূর্ণসংখ্যা*)alignPtr(buf + blocksize*esz, 16);
            পূর্ণসংখ্যা* iubuf = ilbuf + cn;

            দ্বিমিক_Func sccvtfunc = getConvertFunc(scdepth, CV_32S);
            sccvtfunc(lb.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, আকার_2(cn, 1), 0);
            sccvtfunc(ub.ptr(), 1, 0, 1, (uchar*)iubuf, 1, আকার_2(cn, 1), 0);
            পূর্ণসংখ্যা minval = cvRound(getMinVal(depth)), maxval = cvRound(getMaxVal(depth));

            জন্য( পূর্ণসংখ্যা k = 0; k < cn; k++ )
            {
                যদি( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lb = Mat(cn, 1, CV_32S, ilbuf);
            ub = Mat(cn, 1, CV_32S, iubuf);
        }

        convertAndUnrollScalar( lb, src.type(), lbuf, blocksize );
        convertAndUnrollScalar( ub, src.type(), ubuf, blocksize );

        যদি (depth == CV_8U && src.dims <= 2) {
            uint8_t lb_scalar = lbuf[0];
            uint8_t ub_scalar = ubuf[0];
            CALL_HAL(inRange_u8, cv_hal_inRange8u, src.তথ্য, src.step, dst.তথ্য, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        } অন্যথা যদি (depth == CV_32F && src.dims <= 2) {
            দ্বিগুণ lb_scalar = lb.ptr<দ্বিগুণ>(0)[0];
            দ্বিগুণ ub_scalar = ub.ptr<দ্বিগুণ>(0)[0];
            CALL_HAL(inRange_f32, cv_hal_inRange32f, src.তথ্য, src.step, dst.তথ্য, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        }
    }

    জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        জন্য( size_t j = 0; j < যোগ; j += blocksize )
        {
            পূর্ণসংখ্যা bsz = (পূর্ণসংখ্যা)MIN(যোগ - j, blocksize);
            size_t delta = bsz*esz;
            uchar *lptr = lbuf, *uptr = ubuf;
            যদি( !lbScalar )
            {
                lptr = ptrs[2];
                ptrs[2] += delta;
            }
            যদি( !ubScalar )
            {
                পূর্ণসংখ্যা idx = !lbScalar ? 3 : 2;
                uptr = ptrs[idx];
                ptrs[idx] += delta;
            }
            func( ptrs[0], 0, lptr, 0, uptr, 0, cn == 1 ? ptrs[1] : mbuf, 0, আকার_2(bsz*cn, 1));
            যদি( cn > 1 )
                inRangeReduce(mbuf, ptrs[1], bsz, cn);
            ptrs[0] += delta;
            ptrs[1] += bsz;
        }
    }
}
/* End of file. */
