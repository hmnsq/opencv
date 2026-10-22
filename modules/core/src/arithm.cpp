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

#شامل "precomp.hpp"
#شامل "arithm_expr.hpp"   // the new element-wise engine (cv::ew)
#شامل "opencl_kernels_core.hpp"

فضاینام cv
{

/****************************************************************************************\
*                                   logical operations                                   *
\****************************************************************************************/

شمارش { OCL_OP_ADD=0, OCL_OP_SUB=1, OCL_OP_RSUB=2, OCL_OP_ABSDIFF=3, OCL_OP_MUL=4,
       OCL_OP_MUL_SCALE=5, OCL_OP_DIV_SCALE=6, OCL_OP_RECIP_SCALE=7, OCL_OP_ADDW=8,
       OCL_OP_AND=9, OCL_OP_OR=10, OCL_OP_XOR=11, OCL_OP_NOT=12, OCL_OP_MIN=13, OCL_OP_MAX=14,
       OCL_OP_RDIV_SCALE=15 };

// The unified entry point for every element-wise binary op (empty + UMat/OpenCL + CPU engine). Defined
// lower down (near cv::add); forward-declared here so cv::min/cv::max (above it) can use it too.
ایستا تهی arithm_op(ew::بالاترین op, درون_داد_Array src1, درون_داد_Array src2, OutputArray dst,
                      درون_داد_Array mask, صحیح dtype, صحیح oclop, منطقی muldiv, ثابت Scalar& params = Scalar(1));

// The unary counterpart (bitwise NOT, unary math later). Forward-declared for the same reason.
ایستا تهی unary_op(ew::بالاترین op, درون_داد_Array src, OutputArray dst, درون_داد_Array mask,
                     صحیح dtype, ثابت Scalar& params = Scalar());

#ifdef HAVE_OPENCL

ایستا ثابت نویسه* oclop2str[] = { "OP_ADD", "OP_SUB", "OP_RSUB", "OP_ABSDIFF",
    "OP_MUL", "OP_MUL_SCALE", "OP_DIV_SCALE", "OP_RECIP_SCALE",
    "OP_ADDW", "OP_AND", "OP_OR", "OP_XOR", "OP_NOT", "OP_MIN", "OP_MAX", "OP_RDIV_SCALE", 0 };

// depths with no OpenCL type name (ocl::typeToStr() asserts on them)
ایستا درونخطی منطقی noOclType(صحیح depth)
{
    بازگشت depth == CV_16BF || depth == CV_Bool || depth == CV_8F_E4M3FN || depth == CV_8F_E4M3FNUZ;
}

ایستا منطقی ocl_دودویی_op(درون_داد_Array _src1, درون_داد_Array _src2, OutputArray _dst,
                          درون_داد_Array _mask, منطقی bitwise, صحیح oclop, منطقی haveScalar )
{
    منطقی haveMask = !_mask.empty();
    صحیح srctype = _src1.type();
    صحیح srcdepth = CV_MAT_DEPTH(srctype);
    صحیح cn = CV_MAT_CN(srctype);

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقی doubleSupport = d.doubleFPConfig() > 0;
    اگر( oclop < 0 || ((haveMask || haveScalar) && cn > 4) ||
            (!doubleSupport && srcdepth == CV_64F && !bitwise) ||
            (noOclType(srcdepth) && !bitwise))
        بازگشت نادرست;

    نویسه opts[1024];
    صحیح kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    صحیح scalarcn = kercn == 3 ? 4 : kercn;
    صحیح rowsPerWI = d.isIntel() ? 4 : 1;

    ثابت صحیح dstDepth = srcdepth;
    ثابت صحیح dstType = CV_MAKETYPE(dstDepth, kercn);
    ثابت صحیح dstType1 = CV_MAKETYPE(dstDepth, 1);
    ثابت صحیح scalarType = CV_MAKETYPE(srcdepth, scalarcn);

    snprintf(opts, اندازه(opts), "-D %s%s -D %s%s -D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workST=%s -D cn=%d -D rowsPerWI=%d",
            haveMask ? "MASK_" : "", haveScalar ? "UNARY_OP" : "BINARY_OP", oclop2str[oclop],
            doubleSupport ? " -D DOUBLE_SUPPORT" : "",
            bitwise ? ocl::memopTypeToStr(dstType) : ocl::typeToStr(dstType),
            dstDepth,
            bitwise ? ocl::memopTypeToStr(dstType1) : ocl::typeToStr(dstType1),
            bitwise ? ocl::memopTypeToStr(scalarType) : ocl::typeToStr(scalarType),
            kercn, rowsPerWI);

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    اگر (k.empty())
        بازگشت نادرست;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask, 1);

    اگر( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(srctype)*scalarcn;
        دوگانه buf[4] = {0,0,0,0};

        اگر( oclop != OCL_OP_NOT )
        {
            Mat src2sc = _src2.getMat();
            convertAndUnrollScalar(src2sc, srctype, (uchar*)buf, 1);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        اگر( !haveMask )
            k.args(src1arg, dstarg, scalararg);
        وگرنه
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    وگرنه
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::ReadOnlyNoSize(src2, cn, kercn);

        اگر( !haveMask )
            k.args(src1arg, src2arg, dstarg);
        وگرنه
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    بازگشت k.run(2, globalsize, 0, نادرست);
}

#endif

// OpenCL path for the bitwise ops (and/or/xor/not), lifted out of binary_op: choose the (possibly
// scalar) operand order, size the destination, and dispatch the shared bitwise OpenCL kernel. Returns
// true if OpenCL handled the op, false to fall through to the CPU engine. NOT arrives unary
// (src2 == src1, forced-scalar). The caller gates this on a UMat operand + dims <= 2 (via CV_OCL_RUN).
#ifdef HAVE_OPENCL
ایستا منطقی bitwise_op_ocl(درون_داد_Array _src1, درون_داد_Array _src2, OutputArray _dst,
                           درون_داد_Array _mask, صحیح oclop)
{
    ثابت درون_داد_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    درون_داد_Array_2::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    صحیح type1 = psrc1->type(), type2 = psrc2->type();
    منطقی haveScalar = نادرست;

    اگر( oclop == OCL_OP_NOT )
        haveScalar = درست;
    وگرنه اگر( (kind1 == درون_داد_Array_2::MATX) + (kind2 == درون_داد_Array_2::MATX) == 1 ||
             !psrc1->sameSize(*psrc2) || type1 != type2 )
    {
        اگر( checkScalar(*psrc1, type2, kind1, kind2) )
        { std::swap(psrc1, psrc2); std::swap(type1, type2); }        // src1 is the scalar; swap it out
        وگرنه اگر( !checkScalar(*psrc2, type1, kind2, kind1) )
            بازگشت نادرست;
        haveScalar = درست;
    }
    _dst.createSameSize(*psrc1, type1);
    بازگشت ocl_دودویی_op(*psrc1, *psrc2, _dst, _mask, درست, oclop, haveScalar);
}
#endif  // HAVE_OPENCL

}

// bitwise and/or/xor: OpenCL runs the shared bitwise kernel (via bitwise_op_ocl); the CPU path is the
// element-wise engine (OP_AND/OR/XOR, T x T -> T dispatched by element size). oclop = -1 tells
// arithm_op the OpenCL path was already tried, so it only runs the CPU engine. NOT is unary.
تهی cv::bitwise_and(درون_داد_Array a, درون_داد_Array b, OutputArray c, درون_داد_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_AND))
    arithm_op(ew::OP_AND, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/نادرست);
}

تهی cv::bitwise_or(درون_داد_Array a, درون_داد_Array b, OutputArray c, درون_داد_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_OR))
    arithm_op(ew::OP_OR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/نادرست);
}

تهی cv::bitwise_xor(درون_داد_Array a, درون_داد_Array b, OutputArray c, درون_داد_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_XOR))
    arithm_op(ew::OP_XOR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/نادرست);
}

تهی cv::bitwise_not(درون_داد_Array a, OutputArray c, درون_داد_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || c.isUMat()) && a.dims() <= 2,
               bitwise_op_ocl(a, a, c, mask, OCL_OP_NOT))
    unary_op(ew::OP_NOT, a, c, mask, -1);
}

تهی cv::max( درون_داد_Array src1, درون_داد_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, نادرست);
}

تهی cv::min( درون_داد_Array src1, درون_داد_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, نادرست);
}

// The concrete Mat/UMat overloads of min/max exist because C++ overload resolution needs them (a
// bare cv::min(mat, mat, mat) binds Mat& more tightly than InputArray); they must run the SAME engine
// path as the InputArray forms - NOT the legacy binary_op - so mixed types and broadcasting work.
تهی cv::max(ثابت Mat& src1, ثابت Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, نادرست);
}

تهی cv::min(ثابت Mat& src1, ثابت Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, نادرست);
}

تهی cv::max(ثابت UMat& src1, ثابت UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, نادرست);
}

تهی cv::min(ثابت UMat& src1, ثابت UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, نادرست);
}


/****************************************************************************************\
*                                      add/subtract                                      *
\****************************************************************************************/

فضاینام cv
{

#ifdef HAVE_OPENCL

// used by the OpenCL branch only (arithm_op_ocl working-type selection)
ایستا صحیح actualScalarDepth(ثابت دوگانه* data, صحیح len)
{
    صحیح i = 0, minval = INT_MAX, maxval = INT_MIN;
    برای(; i < len; ++i)
    {
        صحیح ival = cvRound(data[i]);
        اگر( ival != data[i] )
            بشکن;
        minval = MIN(minval, ival);
        maxval = MAX(maxval, ival);
    }
    بازگشت i < len ? CV_64F :
        minval >= 0 && maxval <= (صحیح)UCHAR_MAX ? CV_8U :
        minval >= (صحیح)SCHAR_MIN && maxval <= (صحیح)SCHAR_MAX ? CV_8S :
        minval >= 0 && maxval <= (صحیح)USHRT_MAX ? CV_16U :
        minval >= (صحیح)SHRT_MIN && maxval <= (صحیح)SHRT_MAX ? CV_16S :
        CV_32S;
}

ایستا صحیح coerceTypes(صحیح depth1, صحیح depth2, منطقی muldiv)
{
    بازگشت depth1 == depth2 ? depth1 :
        ((depth1 <= CV_32S) & (depth2 <= CV_32S)) != 0 ?
        (((صحیح)!muldiv & (depth1 <= CV_8S) & (depth2 <= CV_8S)) != 0 ? CV_16S : CV_32S) :
        ((CV_ELEM_SIZE1(depth1) > 4) | (CV_ELEM_SIZE1(depth2) > 4)) != 0 ? CV_64F : CV_32F;
}

ایستا منطقی ocl_arithm_op(درون_داد_Array _src1, درون_داد_Array _src2, OutputArray _dst,
                          درون_داد_Array _mask, صحیح wtype,
                          تهی* usrdata, صحیح oclop,
                          منطقی haveScalar )
{
    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقی doubleSupport = d.doubleFPConfig() > 0;
    صحیح type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    منطقی haveMask = !_mask.empty();

    اگر ( (haveMask || haveScalar) && cn > 4 )
        بازگشت نادرست;

#ifdef __APPLE__
    اگر (depth1 == CV_16U && (oclop == OCL_OP_MUL || oclop == OCL_OP_MUL_SCALE))
        بازگشت نادرست;
#endif

    صحیح dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32S, CV_MAT_DEPTH(wtype));
    اگر (!doubleSupport)
        wdepth = std::min(wdepth, CV_32F);

    wtype = CV_MAKETYPE(wdepth, cn);
    صحیح type2 = haveScalar ? wtype : _src2.type(), depth2 = CV_MAT_DEPTH(type2);
    اگر (!doubleSupport && (depth2 == CV_64F || depth1 == CV_64F))
        بازگشت نادرست;

    اگر (noOclType(depth1) || noOclType(depth2) || noOclType(ddepth) || noOclType(wdepth))
        بازگشت نادرست;

    صحیح kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    صحیح scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = d.isIntel() ? 4 : 1;

    نویسه cvtstr[4][50], opts[1024];
    snprintf(opts, اندازه(opts), "-D %s%s -D %s -D srcT1=%s -D srcT1_C1=%s -D srcT2=%s -D srcT2_C1=%s "
            "-D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workT=%s -D workST=%s -D scaleT=%s -D wdepth=%d -D convertToWT1=%s "
            "-D convertToWT2=%s -D convertToDT=%s%s -D cn=%d -D rowsPerWI=%d -D convertFromU=%s",
            (haveMask ? "MASK_" : ""), (haveScalar ? "UNARY_OP" : "BINARY_OP"),
            oclop2str[oclop], ocl::typeToStr(CV_MAKETYPE(depth1, kercn)),
            ocl::typeToStr(depth1), ocl::typeToStr(CV_MAKETYPE(depth2, kercn)),
            ocl::typeToStr(depth2), ocl::typeToStr(CV_MAKETYPE(ddepth, kercn)), ddepth,
            ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKETYPE(wdepth, kercn)),
            ocl::typeToStr(CV_MAKETYPE(wdepth, scalarcn)),
            ocl::typeToStr(wdepth), wdepth,
            ocl::convertTypeStr(depth1, wdepth, kercn, cvtstr[0], اندازه(cvtstr[0])),
            ocl::convertTypeStr(depth2, wdepth, kercn, cvtstr[1], اندازه(cvtstr[1])),
            ocl::convertTypeStr(wdepth, ddepth, kercn, cvtstr[2], اندازه(cvtstr[2])),
            doubleSupport ? " -D DOUBLE_SUPPORT" : "", kercn, rowsPerWI,
            oclop == OCL_OP_ABSDIFF && wdepth == CV_32S && ddepth == wdepth ?
            ocl::convertTypeStr(CV_8U, ddepth, kercn, cvtstr[3], اندازه(cvtstr[3])) : "noconvert");

    size_t usrdata_esz = CV_ELEM_SIZE(wdepth);
    ثابت uchar* usrdata_p = (ثابت uchar*)usrdata;
    ثابت دوگانه* usrdata_d = (ثابت دوگانه*)usrdata;
    اعشاری usrdata_f[3];
    صحیح i, n = oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE ||
        oclop == OCL_OP_RDIV_SCALE || oclop == OCL_OP_RECIP_SCALE ? 1 : oclop == OCL_OP_ADDW ? 3 : 0;
    اگر( usrdata && n > 0 && wdepth == CV_32F )
    {
        برای( i = 0; i < n; i++ )
            usrdata_f[i] = (اعشاری)usrdata_d[i];
        usrdata_p = (ثابت uchar*)usrdata_f;
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    اگر (k.empty())
        بازگشت نادرست;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask, 1);

    اگر( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(wtype)*scalarcn;
        دوگانه buf[4]={0,0,0,0};
        Mat src2sc = _src2.getMat();

        اگر( !src2sc.empty() )
            convertAndUnrollScalar(src2sc, wtype, (uchar*)buf, 1);
        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        اگر( !haveMask )
        {
            اگر(n == 0)
                k.args(src1arg, dstarg, scalararg);
            وگرنه اگر(n == 1)
                k.args(src1arg, dstarg, scalararg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            وگرنه
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        وگرنه
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    وگرنه
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::ReadOnlyNoSize(src2, cn, kercn);

        اگر( !haveMask )
        {
            اگر (n == 0)
                k.args(src1arg, src2arg, dstarg);
            وگرنه اگر (n == 1)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            وگرنه اگر (n == 3)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz*2, usrdata_esz));
            وگرنه
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        وگرنه
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    بازگشت k.run(2, globalsize, پوچ, نادرست);
}

#endif

// OpenCL branch of the arithmetic ops, extracted from the former arithm_op (whose CPU compute is now
// the element-wise engine). Returns true if an OpenCL kernel handled the op, false to let the caller
// fall through to the CPU engine. min/max use the bitwise-style kernel; every other op the arithm
// kernel with a coerced working type. `usrdata` carries the mul/div/addWeighted scale block, `oclop`
// the kernel id. Only reached for a UMat dst; sizes/types that don't fit the scalar/same-size pattern
// (e.g. a broadcast) decline (return false) so the engine handles them on the CPU.
ایستا منطقی arithm_op_ocl(درون_داد_Array _src1, درون_داد_Array _src2, OutputArray _dst,
                          درون_داد_Array _mask, صحیح dtype, صحیح oclop, منطقی muldiv, تهی* usrdata)
{
#ifndef HAVE_OPENCL
    CV_UNUSED(_src1); CV_UNUSED(_src2); CV_UNUSED(_dst); CV_UNUSED(_mask);
    CV_UNUSED(dtype); CV_UNUSED(oclop); CV_UNUSED(muldiv); CV_UNUSED(usrdata);
    بازگشت نادرست;
#وگرنه
    ثابت درون_داد_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    درون_داد_Array_2::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    منطقی haveMask = !_mask.empty();
    منطقی reallocate = نادرست;
    صحیح type1 = psrc1->type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    صحیح type2 = psrc2->type(), depth2 = CV_MAT_DEPTH(type2), cn2 = CV_MAT_CN(type2);
    صحیح wtype, dims1 = psrc1->dims(), dims2 = psrc2->dims();
    Size sz1 = dims1 <= 2 ? psrc1->size() : Size();
    Size sz2 = dims2 <= 2 ? psrc2->size() : Size();

    اگر (!(OCL_PERFORMANCE_CHECK(_dst.isUMat()) && dims1 <= 2 && dims2 <= 2))
        بازگشت نادرست;

    // min/max: the bitwise-style OpenCL kernel (same-type element-wise, no working-type coercion).
    اگر (oclop == OCL_OP_MIN || oclop == OCL_OP_MAX)
    {
        منطقی haveScalar = نادرست;
        اگر ((kind1 == درون_داد_Array_2::MATX) + (kind2 == درون_داد_Array_2::MATX) == 1 ||
            !psrc1->sameSize(*psrc2) || type1 != type2)
        {
            اگر (checkScalar(*psrc1, type2, kind1, kind2))
            { std::swap(psrc1, psrc2); std::swap(type1, type2); }
            وگرنه اگر (!checkScalar(*psrc2, type1, kind2, kind1))
                بازگشت نادرست;
            haveScalar = درست;
        }
        _dst.createSameSize(*psrc1, type1);
        بازگشت ocl_دودویی_op(*psrc1, *psrc2, _dst, _mask, نادرست, oclop, haveScalar);
    }

    // reciprocal (scale/src): the OpenCL kernel is unary on src - use src2 for both operands (the CPU
    // engine received a 0-dim `1` numerator as src1, which the recip kernel would ignore). [TODO.VP: tidy]
    اگر (oclop == OCL_OP_RECIP_SCALE)
    {
        psrc1 = psrc2; kind1 = kind2; type1 = type2; depth1 = depth2; cn = cn2; dims1 = dims2; sz1 = sz2;
    }

    // add/sub/mul/div/absdiff/addWeighted/recip: the arithm OpenCL kernel with a coerced work type.
    منطقی src1Scalar = checkScalar(*psrc1, type2, kind1, kind2);
    منطقی src2Scalar = checkScalar(*psrc2, type1, kind2, kind1);
    منطقی haveScalar = نادرست;

    اگر (dims1 != dims2 || sz1 != sz2 || cn != cn2 ||
        (kind1 == درون_داد_Array_2::MATX && (sz1 == Size(1,4) || sz1 == Size(1,1))) ||
        (kind2 == درون_داد_Array_2::MATX && (sz2 == Size(1,4) || sz2 == Size(1,1))))
    {
        اگر ((type1 == CV_64F && (sz1.height == 1 || sz1.height == 4)) && src1Scalar)
        {
            // src1 is a scalar; swap it with src2
            std::swap(psrc1, psrc2); std::swap(sz1, sz2); std::swap(type1, type2);
            std::swap(depth1, depth2); std::swap(cn, cn2); std::swap(dims1, dims2);
            اگر (oclop == OCL_OP_SUB) oclop = OCL_OP_RSUB;
            اگر (oclop == OCL_OP_DIV_SCALE) oclop = OCL_OP_RDIV_SCALE;
        }
        وگرنه اگر (!src2Scalar)
            بازگشت نادرست;   // array op array with mismatched size/cn: engine broadcasts on the CPU
        haveScalar = درست;
        CV_Assert((type2 == CV_64F || type2 == CV_32F) && (sz2.height == 1 || sz2.height == 4));

        اگر (!muldiv)
        {
            Mat sc = psrc2->getMat();
            depth2 = actualScalarDepth(sc.ptr<دوگانه>(), sz2 == Size(1, 1) ? cn2 : cn);
            اگر (depth2 == CV_64F && CV_ELEM_SIZE1(depth1) < 8)
                depth2 = CV_32F;
        }
        وگرنه
            depth2 = CV_64F;
    }

    اگر (dtype < 0)
    {
        اگر (_dst.fixedType())
            dtype = _dst.type();
        وگرنه
        {
            اگر (!haveScalar && type1 != type2)
                CV_Error(cv::Error::StsBadArg,
                     "When the input arrays in add/subtract/multiply/divide functions have different types, "
                     "the output array type must be explicitly specified");
            dtype = type1;
        }
    }
    dtype = CV_MAT_DEPTH(dtype);

    اگر (depth1 == depth2 && dtype == depth1)
        wtype = dtype;
    وگرنه اگر (!muldiv)
    {
        wtype = coerceTypes(depth1, depth2, نادرست);
        wtype = coerceTypes(wtype, dtype, نادرست);
        اگر (dtype < CV_32F && (depth1 < CV_32F || depth2 < CV_32F))
            wtype = CV_32S;
    }
    وگرنه
    {
        wtype = coerceTypes(depth1, depth2, درست);
        wtype = coerceTypes(wtype, dtype, درست);
    }

    // The scaled OpenCL kernels (mul/div by a scale, reciprocal, addWeighted) compute in float; the old
    // same-type fast path forced max(depth, CV_32F). coerceTypes keeps same-type integer, so bump here.
    اگر (oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE || oclop == OCL_OP_RDIV_SCALE ||
        oclop == OCL_OP_RECIP_SCALE || oclop == OCL_OP_ADDW)
        wtype = std::max(wtype, (صحیح)CV_32F);

    dtype = CV_MAKETYPE(dtype, cn);
    wtype = CV_MAKETYPE(wtype, cn);

    اگر (haveMask)
    {
        صحیح mtype = _mask.type();
        CV_Assert((mtype == CV_8UC1 || mtype == CV_8SC1 || mtype == CV_Bool) && _mask.sameSize(*psrc1));
        reallocate = !_dst.sameSize(*psrc1) || _dst.type() != dtype;
    }

    _dst.createSameSize(*psrc1, dtype);
    اگر (reallocate)
        _dst.setTo(0.);

    بازگشت ocl_arithm_op(*psrc1, *psrc2, _dst, _mask, wtype, usrdata, oclop, haveScalar);
#endif
}

// Element-wise binary op (ADD/SUB/...) on two CPU operands - each an array Mat or a MATX scalar -
// through the new broadcasting engine. Handles mixed input types (emitBinary inserts the casts) and
// broadcasting (a scalar rides as a 0-dim per-channel CONST; arrays broadcast via outputShape/exec).
// Returns false (declining) for UMat, a write-mask, or two scalar operands, so the caller falls
// through to arithm_op. Incompatible shapes make the engine throw (matching cv::add's error).
// Does the output array already have exactly this shape+type (=> create() reuses it, no realloc)?
// Works for Mat and UMat alike (shape/type queries, no data-pointer peeking).
ایستا منطقی outArrayMatches(ثابت _OutputArray& a, ثابت MatShape& shp, صحیح type)
{
    اگر (a.empty() || a.type() != type)
        بازگشت نادرست;
    صحیح sz[MatShape::MAX_DIMS];
    صحیح nd = a.sizend(sz);
    اگر (nd != (صحیح)shp.size())
        بازگشت نادرست;
    برای (صحیح i = 0; i < nd; i++)
        اگر (sz[i] != shp[i])
            بازگشت نادرست;
    بازگشت درست;
}

// Are two operands' iteration shapes (spatial dims + channels innermost, exactly how the engine
// iterates) numpy-broadcast-compatible? Powers the scalar-like-Mat compat fallback in arithm_op.
// Cheap by design: sizend copies the dim arrays inline - no Mat headers, no allocations.
ایستا منطقی shapesBroadcastCompat(درون_داد_Array a, صحیح acn, درون_داد_Array b, صحیح bcn)
{
    صحیح asz[MatShape::MAX_DIMS + 1], bsz[MatShape::MAX_DIMS + 1];
    صحیح ad = a.sizend(asz), bd = b.sizend(bsz);
    asz[ad++] = acn;
    bsz[bd++] = bcn;
    برای (صحیح i1 = ad - 1, i2 = bd - 1; i1 >= 0 && i2 >= 0; i1--, i2--)
        اگر (asz[i1] != bsz[i2] && asz[i1] != 1 && bsz[i2] != 1)
            بازگشت نادرست;
    بازگشت درست;
}

// The one entry point for every element-wise binary op (add/sub/mul/div/min/max/absdiff/addWeighted/
// reciprocal). Handles empty inputs, the UMat/OpenCL branch (arithm_op_ocl) and the CPU element-wise
// engine. `oclop` selects the OpenCL kernel; `muldiv` drives the OpenCL working-type coercion + scale;
// `params` are the op scalars (params[0]=mul/div scale; {alpha,beta,gamma} for addWeighted).
// Below this many ELEMENTS (total * channels), the per-call constant overhead of the engine path
// (program build + compile + executor setup) is comparable to the actual work - take the direct
// kernel call instead. Same tier boundary as math_op's MATH_OP_SMALL for cv::exp.
شمارش { ARITHM_SMALL_DIRECT = 100000 };

ایستا تهی arithm_op(ew::بالاترین op, درون_داد_Array src1, درون_داد_Array src2, OutputArray dst,
                      درون_داد_Array mask, صحیح dtype, صحیح oclop, منطقی muldiv, ثابت Scalar& params)
{
    اگر (src1.empty() || src2.empty())          // empty inputs -> empty result
    {
        CV_Assert((src1.empty() && src2.empty()) ||
                  (src1.empty() && isScalarArg(src2, src1.channels())) ||
                  (src2.empty() && isScalarArg(src1, src2.channels())));
        dst.release();
        اگر (dtype >= 0)
            dst.create(0, 0, dtype);
        بازگشت;
    }

    // UMat -> classic OpenCL kernel. It declines (false) when OpenCL can't apply (no device, dims>2, a
    // broadcast, cn>4 masked, ...) - then the CPU engine below handles it (mapping the UMat via getMat).
    // oclop < 0 is a sentinel: the caller (e.g. cv::compare) already ran its own OpenCL path, so this
    // helper only runs the CPU engine (a UMat operand is mapped via getMat).
    اگر (oclop >= 0 && cv::ocl::isOpenCLActivated() &&
        (src1.isUMat() || src2.isUMat() || dst.isUMat()))
    {
        دوگانه abg[3] = { params[0], params[1], params[2] }, scale = params[0];
        تهی* usrdata = (op == ew::OP_ADDW) ? (تهی*)abg : (muldiv ? (تهی*)&scale : nullptr);
        اگر (arithm_op_ocl(src1, src2, dst, mask, dtype, oclop, muldiv, usrdata))
            بازگشت;
    }

    ثابت منطقی haveMask = !mask.empty();
    ثابت صحیح cn1 = src1.channels(), cn2 = src2.channels();
    منطقی s1 = isScalarArg(src1, src2), s2 = isScalarArg(src2, src1);
    اگر (s1 && s2)
        s1 = s2 = نادرست;    // two scalars: treat both as tiny array operands (element-wise + broadcast),
                            // matching arithm_op (Scalar+Scalar -> 4x1, number+number -> 1x1, ...)
    // A 4x1 CV_64F Mat/UMat pseudo-scalar (see isScalarArg) is hijacked only against a REAL array:
    // when the partner is itself a tiny scalar-shaped array (e.g. compare(Mat 4x1, Mat 1x1) -
    // issue #8999), both are honest data and ride the broadcast. A genuine MATX Scalar never demotes.
    وگرنه اگر (s1 && src1.kind() != درون_داد_Array_2::MATX && isScalarLikeMat(src2, cn1))
        s1 = نادرست;
    وگرنه اگر (s2 && src2.kind() != درون_داد_Array_2::MATX && isScalarLikeMat(src1, cn2))
        s2 = نادرست;

    // Compat fallback: Java/Python/user code passes scalars as real little Mats (the classic 4x1
    // CV_64F column, a 1xcn/cnx1 vector - see isScalarLikeMat). Those normally ride broadcasting
    // now, so hijack one as a per-channel scalar ONLY when the shapes do NOT broadcast - just the
    // calls that would otherwise throw get the 4.x scalar semantics, every valid broadcast keeps
    // its numpy meaning. Cheap: two geometry probes, and the O(ndims) shape walk runs only when a
    // probe hits (never for ordinary same-size or MATX-scalar calls).
    اگر (!s1 && !s2)
    {
        منطقی like1 = isScalarLikeMat(src1, cn2), like2 = isScalarLikeMat(src2, cn1);
        اگر ((like1 || like2) && !shapesBroadcastCompat(src1, cn1, src2, cn2))
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
    ثابت تهی* dstObj = (dst.kind() == درون_داد_Array_2::MAT) ? dst.getObj() : nullptr;
    ثابت منطقی aliased = dstObj && (dstObj == src1.getObj() || dstObj == src2.getObj());
    Mat m1loc, m2loc, mloc;
    auto asMat = [&](درون_داد_Array a, Mat& loc) -> ثابت Mat* {
        اگر (a.kind() == درون_داد_Array_2::MAT && !aliased) بازگشت (ثابت Mat*)a.getObj();
        loc = a.getMat(); بازگشت &loc;
    };
    ثابت Mat* pm1 = s1 ? nullptr : asMat(src1, m1loc);
    ثابت Mat* pm2 = s2 ? nullptr : asMat(src2, m2loc);
    ثابت صحیح adepth = s1 ? pm2->depth() : pm1->depth();   // the (first) array operand's depth
    // auto result depth (no explicit dtype, no fixed-type dst): add/sub/mul/div keep the first array
    // operand's depth (cv::'s dtype==-1 convention), but min/max/absdiff have NO dtype argument and
    // must promote mixed inputs to their common type (min(u32,f32) in u32 would drop the float). For
    // same-type inputs promoteArith(da,db)==da, so classic behaviour is preserved (e.g. absdiff of two
    // s16 stays s16, saturating - the wide unsigned |a-b| is computed then cast back down by emitBinary).
    صحیح autoDepth = adepth;
    اگر (op == ew::OP_MIN || op == ew::OP_MAX || op == ew::OP_ABSDIFF)
    {
        ثابت صحیح d1 = s1 ? ew::EW_DEPTH_NONE : pm1->depth();
        ثابت صحیح d2 = s2 ? ew::EW_DEPTH_NONE : pm2->depth();
        autoDepth = ew::promoteArith(d1, d2);
    }
    // result depth: explicit dtype wins; else a fixed-type dst dictates it; else the auto depth above.
    ثابت صحیح rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
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
    اگر (!haveMask && !s1 && !s2 &&
        (op == ew::OP_ADD || op == ew::OP_SUB || op == ew::OP_MIN || op == ew::OP_MAX ||
         op == ew::OP_ABSDIFF || op == ew::OP_MUL || op == ew::OP_ADDW ||
         op == ew::OP_AND || op == ew::OP_OR || op == ew::OP_XOR) &&
        rdepth == pm1->depth() && pm1->type() == pm2->type() && pm1->size == pm2->size &&
        pm1->isContinuous() && pm2->isContinuous() &&
        pm1->total()*cn1 <= (size_t)ARITHM_SMALL_DIRECT)
    {
        ew::TKernel k = ew::getElemwiseFunc(op, rdepth, rdepth, ew::EW_DEPTH_NONE, rdepth);
        اگر (k.fptr)
        {
            dst.createSameSize(src1, pm1->type());   // whole-shape transfer, not piecemeal dims+sizes
            Mat dloc;
            Mat* pd = (dst.kind() == درون_داد_Array_2::MAT) ? (Mat*)dst.getObj() : &(dloc = dst.getMat());
            اگر (pd->isContinuous())     // a reused non-continuous dst view falls through to the engine
            {
                k.fptr(pm1->data, 0, 1, pm2->data, 0, 1, nullptr, 0, 0,
                       pd->data, 0, (صحیح)(pm1->total()*cn1), 1, params.val, k.flags, k.userdata);
                بازگشت;
            }
        }
    }

    // operand order = (src1, src2): an array becomes an INPUT, a scalar a flexible per-channel CONST
    // read straight from the caller's inline storage (getObj()), no Mat / convertTo.
    ew::TExpr p;
    auto addOperand = [&](درون_داد_Array src, منطقی isScalar, ثابت Mat* m, صحیح otherCn) -> صحیح {
        اگر (!isScalar)
            بازگشت p.add_درون_داد(m->depth());
        ثابت uchar* sp; صحیح sd;
        uchar scbuf[EW_SCALAR_BUF_SIZE];   // stack room for a UMAT scalar; addConst copies the values out
        ثابت صحیح scn0 = scalarArgElems(src, sp, sd, scbuf), cn = otherCn;
        ثابت صحیح scn = (cn == scn0 || (cn < 4 && scn0 == 4)) ? cn
                      : (scn0 == 1) ? 1 : 0;
        CV_Assert(scn > 0);
        بازگشت p.addConst(ew::EW_DEPTH_NONE, sd, sp, scn);
    };
    ثابت صحیح a0 = addOperand(src1, s1, pm1, cn2);
    ثابت صحیح a1 = addOperand(src2, s2, pm2, cn1);

    // Write-mask: a single-channel 1-byte array (u8/s8/bool) the size of the output spatial shape,
    // added as another broadcast input. The arithmetic result lands in a temp; a final
    // select(mask, r, dst) -> dst overwrites only mask!=0 positions of the (pre-existing) output.
    صحیح sMask = 0;
    ثابت Mat* pmask = nullptr;
    اگر (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = asMat(mask, mloc);
        sMask = p.add_درون_داد(pmask->depth());
    }
    ثابت صحیح sOut = p.addOutput(rdepth);               // output BEFORE emit -> moveToOutput drops the
    ثابت صحیح r = p.emit_دودویی(op, a0, a1, rdepth, params);   // result temp (mask-free => none); scale
                                                        // rides params[0] (mul/div), {a,b,g} for addW
    اگر (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    وگرنه
        p.moveToOutput(r, sOut);
    p.compile();

    ثابت Mat* inputs[3]; صحیح ni = 0;                   // inputs in program (input-index) order
    اگر (!s1) inputs[ni++] = pm1;
    اگر (!s2) inputs[ni++] = pm2;
    اگر (haveMask) inputs[ni++] = pmask;

    // Pre-create the output at the broadcast shape, ALWAYS via dst.create() - never hand exec a raw
    // dst Mat. Routing through the _OutputArray is what enforces its contract: a FIXED_SIZE/FIXED_TYPE
    // dst (in-place 'a += b' passes '(const Mat&)a') throws here if a broadcast would change its shape
    // or type (Mat(1x1) += Mat(1x4)), instead of silently reallocating - exec sees only a Mat and has
    // no view of the array's fixed flags. Cost over a direct exec is negligible: exec keeps its own
    // fast path for a matching-shape op, and dst.create() with an unchanged shape/type is a no-op.
    // With a mask, a freshly (re)allocated dst is zeroed so mask==0 reads 0 (matches arithm_op); a
    // reused dst keeps its prior content there - detect reuse by shape+type BEFORE create (correct for
    // a reused UMat dst too).
    MatShape oshape; صحیح ocn;
    p.outputShape(inputs, oshape, ocn);
    ثابت صحیح otype = CV_MAKETYPE(rdepth, ocn);
    ثابت منطقی reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == درون_داد_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    اگر (haveMask && !reused)
        pdst->setZero();                          // setZero (not setTo, which caps at 4 channels)
    p.exec(inputs, pdst);
}

// Unary element-wise op through the engine (bitwise NOT now; unary math later). One array input, an
// optional write-mask; result depth = dtype (>=0), else a fixed-type dst's depth, else the input
// depth. Mirrors arithm_op's build/mask/exec tail with a single input and no scalar operand. OpenCL is
// run by the caller (bitwise_op_ocl), so this is the CPU engine only.
ایستا تهی unary_op(ew::بالاترین op, درون_داد_Array src, OutputArray dst, درون_داد_Array mask,
                     صحیح dtype, ثابت Scalar& params)
{
    اگر (src.empty())
    {
        dst.release();
        اگر (dtype >= 0)
            dst.create(0, 0, dtype);
        بازگشت;
    }

    ثابت منطقی haveMask = !mask.empty();
    // in-place (dst aliases src): keep a header copy so a shape/type-changing dst.create() can't free
    // src's data mid-op (matches arithm_op). A fixed-size/type dst never reallocs, so this is only for
    // an explicit dtype change.
    ثابت تهی* dstObj = (dst.kind() == درون_داد_Array_2::MAT) ? dst.getObj() : nullptr;
    ثابت منطقی aliased = dstObj && src.kind() == درون_داد_Array_2::MAT && dstObj == src.getObj();
    Mat sloc, mloc;
    ثابت Mat* psrc = (src.kind() == درون_داد_Array_2::MAT && !aliased) ? (ثابت Mat*)src.getObj()
                                                                   : &(sloc = src.getMat());
    ثابت صحیح rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
                     : (dst.fixedType() ? dst.depth() : psrc->depth());

    ew::TExpr p;
    ثابت صحیح a0 = p.add_درون_داد(psrc->depth());
    صحیح sMask = 0;
    ثابت Mat* pmask = nullptr;
    اگر (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = (mask.kind() == درون_داد_Array_2::MAT) ? (ثابت Mat*)mask.getObj() : &(mloc = mask.getMat());
        sMask = p.add_درون_داد(pmask->depth());
    }
    ثابت صحیح sOut = p.addOutput(rdepth);
    ثابت صحیح r = p.emitUnary(op, a0, rdepth, params);
    اگر (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    وگرنه
        p.moveToOutput(r, sOut);
    p.compile();

    ثابت Mat* inputs[2]; صحیح ni = 0;
    inputs[ni++] = psrc;
    اگر (haveMask) inputs[ni++] = pmask;

    MatShape oshape; صحیح ocn;
    p.outputShape(inputs, oshape, ocn);
    ثابت صحیح otype = CV_MAKETYPE(rdepth, ocn);
    ثابت منطقی reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == درون_داد_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    اگر (haveMask && !reused)
        pdst->setZero();
    p.exec(inputs, pdst);
}

}

تهی cv::add( درون_داد_Array src1, درون_داد_Array src2, OutputArray dst,
          درون_داد_Array mask, صحیح dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADD, src1, src2, dst, mask, dtype, OCL_OP_ADD, نادرست);
}

تهی cv::subtract( درون_داد_Array src1, درون_داد_Array src2, OutputArray dst,
                   درون_داد_Array mask, صحیح dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_SUB, src1, src2, dst, mask, dtype, OCL_OP_SUB, نادرست);
}

تهی cv::absdiff( درون_داد_Array src1, درون_داد_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ABSDIFF, src1, src2, dst, noArray(), -1, OCL_OP_ABSDIFF, نادرست);
}

تهی cv::copyTo(درون_داد_Array _src, OutputArray _dst, درون_داد_Array _mask)
{
    CV_INSTRUMENT_REGION();

    _src.copyTo(_dst, _mask);
}

/****************************************************************************************\
*                                    multiply/divide                                     *
\****************************************************************************************/

فضاینام cv
{

تهی multiply(درون_داد_Array src1, درون_داد_Array src2,
              OutputArray dst, دوگانه scale, صحیح dtype)
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح oclop = std::abs(scale - 1.0) < DBL_EPSILON ? OCL_OP_MUL : OCL_OP_MUL_SCALE;
    arithm_op(ew::OP_MUL, src1, src2, dst, noArray(), dtype, oclop, /*muldiv*/ درست, Scalar(scale));
}

تهی divide(درون_داد_Array src1, درون_داد_Array src2,
                OutputArray dst, دوگانه scale, صحیح dtype)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_DIV, src1, src2, dst, noArray(), dtype, OCL_OP_DIV_SCALE, /*muldiv*/ درست, Scalar(scale));
}

تهی divide(دوگانه scale, درون_داد_Array src2,
                OutputArray dst, صحیح dtype)
{
    CV_INSTRUMENT_REGION();

    اگر (src2.empty())
    {
        dst.release();
        بازگشت;
    }

    // scale / src2 == scale * 1 / src2: feed a 0-dim `1` as the numerator so the CPU engine reuses the
    // normal divide (scale*a/b). Give the numerator src2's OWN depth: then OP_DIV(T, T) is same-type and
    // takes the fast per-type kernel (f32 work for <=16-bit / f16 / bf16, not the s32->f64 path a CV_32S
    // numerator would force). An integer src2 keeps its div-by-zero->0 guard (both operands integer); a
    // float src2 divides as float (1/0->inf), both matching cv::divide. dtype<0 follows src2's depth (a
    // fixed-type dst still wins). The UMat path uses the dedicated reciprocal kernel (OCL_OP_RECIP_SCALE).
    دوگانه one = 1;
    Mat numerator(MatShape::scalar(), src2.depth(), &one);
    scalarToRawData(Scalar(1), &one, src2.depth(), 1);       // write `1` in src2's depth (no allocation)
    ثابت صحیح rtype = (dtype < 0 && !dst.fixedType()) ? src2.depth() : dtype;
    arithm_op(ew::OP_DIV, numerator, src2, dst, noArray(), rtype, OCL_OP_RECIP_SCALE, /*muldiv*/ درست, Scalar(scale));
}

UMat UMat::mul(درون_داد_Array m, دوگانه scale) ثابت
{
    UMat dst;
    multiply(*این, m, dst, scale);
    بازگشت dst;
}

/****************************************************************************************\
*                                      addWeighted                                       *
\****************************************************************************************/

}

تهی cv::addWeighted( درون_داد_Array src1, دوگانه alpha, درون_داد_Array src2,
                      دوگانه beta, دوگانه gamma, OutputArray dst, صحیح dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADDW, src1, src2, dst, noArray(), dtype, OCL_OP_ADDW, /*muldiv*/ درست,
              Scalar(alpha, beta, gamma));
}


/****************************************************************************************\
*                                          compare                                       *
\****************************************************************************************/

فضاینام cv
{


ایستا دوگانه getMinVal(صحیح depth)
{
    ایستا ثابت دوگانه tab[CV_DEPTH_MAX] =
    {
        0, -128, 0, -32768, INT_MIN, -FLT_MAX, -DBL_MAX,
        -65504, -FLT_MAX, 0, 0, (دوگانه)INT64_MIN, 0
    };
    بازگشت tab[depth];
}

ایستا دوگانه getMaxVal(صحیح depth)
{
    ایستا ثابت دوگانه tab[CV_DEPTH_MAX] = {
        255, 127, 65535, 32767, INT_MAX, FLT_MAX, DBL_MAX,
        65504, FLT_MAX, 255, (دوگانه)UINT64_MAX, (دوگانه)INT64_MAX, (دوگانه)UINT32_MAX, 0
    };
    بازگشت tab[depth];
}

#ifdef HAVE_OPENCL

ایستا منطقی ocl_compare(درون_داد_Array _src1, درون_داد_Array _src2, OutputArray _dst, صحیح op, منطقی haveScalar)
{
    ثابت ocl::Device& dev = ocl::Device::getDefault();
    منطقی doubleSupport = dev.doubleFPConfig() > 0;
    صحیح type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1),
            type2 = _src2.type(), depth2 = CV_MAT_DEPTH(type2);

    اگر (!doubleSupport && depth1 == CV_64F)
        بازگشت نادرست;

    // the integer scalar path below works in int; let the CPU handle the wider 5.x integer depths
    اگر (haveScalar && (depth1 == CV_32U || depth1 == CV_64U || depth1 == CV_64S))
        بازگشت نادرست;

    اگر (!haveScalar && (!_src1.sameSize(_src2) || type1 != type2))
            بازگشت نادرست;

    // This OpenCL kernel broadcasts a scalar's FIRST channel to all channels. The CPU path compares
    // per-channel (scalar[c] for channel c), so a multichannel array against a scalar with DISTINCT
    // channel values would disagree - bail to the CPU engine in that case (rare; not worth a kernel fix).
    اگر (haveScalar && cn > 1)
    {
        Mat sc = _src2.getMat();
        صحیح scn = (صحیح)sc.total();
        اگر (scn > 1)
        {
            دوگانه v[4] = { 0, 0, 0, 0 };
            getConvertFunc(sc.depth(), CV_64F)(sc.ptr(), 1, 0, 1, (uchar*)v, 1, Size(std::min(scn, 4), 1), 0);
            برای (صحیح i = 1, n = std::min(cn, scn); i < n; i++)
                اگر (v[i] != v[0])
                    بازگشت نادرست;
        }
    }

    صحیح kercn = haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst), rowsPerWI = dev.isIntel() ? 4 : 1;
    // Workaround for bug with "?:" operator in AMD OpenCL compiler
    اگر (depth1 >= CV_16U)
        kercn = 1;

    صحیح scalarcn = kercn == 3 ? 4 : kercn;
    ثابت نویسه * ثابت operationMap[] = { "==", ">", ">=", "<", "<=", "!=" };
    نویسه cvt[50];

    String opts = format("-D %s -D srcT1=%s -D dstT=%s -D DEPTH_dst=%d -D workT=srcT1 -D cn=%d"
                         " -D convertToDT=%s -D OP_CMP -D CMP_OPERATOR=%s -D srcT1_C1=%s"
                         " -D srcT2_C1=%s -D dstT_C1=%s -D workST=%s -D rowsPerWI=%d%s",
                         haveScalar ? "UNARY_OP" : "BINARY_OP",
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, kercn)),
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U, kercn,
                         ocl::convertTypeStr(depth1, CV_8U, kercn, cvt, اندازه(cvt)),
                         operationMap[op], ocl::typeToStr(depth1),
                         ocl::typeToStr(depth1), ocl::typeToStr(CV_8U),
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, scalarcn)), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    اگر (k.empty())
        بازگشت نادرست;

    UMat src1 = _src1.getUMat();
    Size size = src1.size();
    _dst.create(size, CV_8UC(cn));
    UMat dst = _dst.getUMat();

    اگر (haveScalar)
    {
        size_t esz = CV_ELEM_SIZE1(type1) * scalarcn;
        دوگانه buf[4] = { 0, 0, 0, 0 };
        Mat src2 = _src2.getMat();

        اگر( depth1 > CV_32S )
            convertAndUnrollScalar( src2, depth1, (uchar *)buf, kercn );
        وگرنه
        {
            دوگانه fval = 0;
            getConvertFunc(depth2, CV_64F)(src2.ptr(), 1, 0, 1, (uchar *)&fval, 1, Size(1, 1), 0);
            اگر( fval < getMinVal(depth1) )
                بازگشت dst.setTo(Scalar::all(op == CMP_GT || op == CMP_GE || op == CMP_NE ? 255 : 0)), درست;

            اگر( fval > getMaxVal(depth1) )
                بازگشت dst.setTo(Scalar::all(op == CMP_LT || op == CMP_LE || op == CMP_NE ? 255 : 0)), درست;

            صحیح ival = cvRound(fval);
            اگر( fval != ival )
            {
                اگر( op == CMP_LT || op == CMP_GE )
                    ival = cvCeil(fval);
                وگرنه اگر( op == CMP_LE || op == CMP_GT )
                    ival = cvFloor(fval);
                وگرنه
                    بازگشت dst.setTo(Scalar::all(op == CMP_NE ? 255 : 0)), درست;
            }
            convertAndUnrollScalar(Mat(1, 1, CV_32S, &ival), depth1, (uchar *)buf, kercn);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        k.args(ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn),
               ocl::KernelArg::WriteOnly(dst, cn, kercn), scalararg);
    }
    وگرنه
    {
        UMat src2 = _src2.getUMat();

        k.args(ocl::KernelArg::ReadOnlyNoSize(src1),
               ocl::KernelArg::ReadOnlyNoSize(src2),
               ocl::KernelArg::WriteOnly(dst, cn, kercn));
    }

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    بازگشت k.run(2, globalsize, پوچ, نادرست);
}

#endif

}

تهی cv::compare(درون_داد_Array _src1, درون_داد_Array _src2, OutputArray _dst, صحیح op)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( op == CMP_LT || op == CMP_LE || op == CMP_EQ ||
               op == CMP_NE || op == CMP_GE || op == CMP_GT );

    CV_Assert(_src1.empty() == _src2.empty());
    اگر (_src1.empty() && _src2.empty())
    {
        _dst.release();
        بازگشت;
    }

    منطقی haveScalar = نادرست;

    اگر ((_src1.isMatx() + _src2.isMatx()) == 1
            || !_src1.sameSize(_src2)
            || _src1.type() != _src2.type())
    {
        منطقی is_src1_scalar = checkScalar(_src1, _src2.type(), _src1.kind(), _src2.kind());
        منطقی is_src2_scalar = checkScalar(_src2, _src1.type(), _src2.kind(), _src1.kind());

        // exactly one scalar operand: keep it as src2 (swapping flips the ordering ops) so the OpenCL
        // path sees the array-op-scalar form. Otherwise - two arrays of different size (broadcast),
        // mixed types, or two scalars - the CPU engine broadcasts / promotes them, so there is NO error
        // here: fall through to arithm_op (which handles scalar detection, broadcast and mixed types).
        اگر (is_src1_scalar && !is_src2_scalar)
        {
            op = op == CMP_LT ? CMP_GT : op == CMP_LE ? CMP_GE :
                op == CMP_GE ? CMP_LE : op == CMP_GT ? CMP_LT : op;
            // src1 is a scalar; swap it with src2
            compare(_src2, _src1, _dst, op);
            بازگشت;
        }
        haveScalar = is_src2_scalar && !is_src1_scalar;
    }
    CV_UNUSED(haveScalar);   // consumed by CV_OCL_RUN only - a no-op without OpenCL

    // OpenCL handles only the same-size and array-op-scalar forms; a broadcast between two arrays of
    // different size falls through to the CPU engine.
    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat())
               && (haveScalar || _src1.sameSize(_src2)),
               ocl_compare(_src1, _src2, _dst, op, haveScalar))

    // CPU: the element-wise engine. A CMP_* code maps to the corresponding OP_CMP_* TOp; the engine
    // forces a u8 mask (dtype = CV_8U) and compares in the common operand type (a fractional scalar
    // threshold promotes both operands to float - matching the classic ceil/floor threshold logic).
    // The scalar / broadcast / mixed-shape cases are handled inside arithm_op. oclop = -1 tells it the
    // OpenCL path was already tried above (via ocl_compare), so it only runs the CPU engine.
    ew::بالاترین cmpOp = op == CMP_EQ ? ew::OP_CMP_EQ : op == CMP_NE ? ew::OP_CMP_NE :
                    op == CMP_LT ? ew::OP_CMP_LT : op == CMP_LE ? ew::OP_CMP_LE :
                    op == CMP_GT ? ew::OP_CMP_GT : ew::OP_CMP_GE;
    arithm_op(cmpOp, _src1, _src2, _dst, noArray(), CV_8U, /*oclop=*/-1, /*muldiv=*/نادرست);
}

/****************************************************************************************\
*                                        inRange                                         *
\****************************************************************************************/

فضاینام cv
{

template <typename T>
ساختار InRange_SIMD
{
    صحیح operator () (ثابت T * src1, ثابت T * src2, ثابت T * src3, uchar * dst, صحیح len) ثابت
    {
        برای (صحیح x = 0; x < len; x++)
            dst[x] = (src2[x] <= src1[x] && src1[x] <= src3[x]) ? 255 : 0;
        بازگشت len;
    }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)

template <>
ساختار InRange_SIMD<uchar>
{
    صحیح operator () (ثابت uchar * src1, ثابت uchar * src2, ثابت uchar * src3,
        uchar * dst, صحیح len) ثابت
    {
        صحیح x = 0;
        ثابت صحیح width = VTraits<v_uint8>::vlanes();

        برای (; x <= len - width; x += width)
        {
            v_uint8 values = vx_load(src1 + x);
            v_uint8 کم = vx_load(src2 + x);
            v_uint8 بالا = vx_load(src3 + x);

            v_store(dst + x, v_and(v_ge(values, کم), v_ge(بالا, values)));
        }
        vx_cleanup();
        بازگشت x;
    }
};

template <>
ساختار InRange_SIMD<schar>
{
    صحیح operator () (ثابت schar * src1, ثابت schar * src2, ثابت schar * src3,
        uchar * dst, صحیح len) ثابت
    {
        صحیح x = 0;
        ثابت صحیح width = VTraits<v_int8>::vlanes();

        برای (; x <= len - width; x += width)
        {
            v_int8 values = vx_load(src1 + x);
            v_int8 کم = vx_load(src2 + x);
            v_int8 بالا = vx_load(src3 + x);

            v_store((schar*)(dst + x), v_and(v_ge(values, کم), v_ge(بالا, values)));
        }
        vx_cleanup();
        بازگشت x;
    }
};

template <>
ساختار InRange_SIMD<ushort>
{
    صحیح operator () (ثابت ushort * src1, ثابت ushort * src2, ثابت ushort * src3,
        uchar * dst, صحیح len) ثابت
    {
        صحیح x = 0;
        ثابت صحیح width = VTraits<v_uint16>::vlanes() * 2;

        برای (; x <= len - width; x += width)
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
        بازگشت x;
    }
};

template <>
ساختار InRange_SIMD<short>
{
    صحیح operator () (ثابت short * src1, ثابت short * src2, ثابت short * src3,
        uchar * dst, صحیح len) ثابت
    {
        صحیح x = 0;
        ثابت صحیح width = (صحیح)VTraits<v_int16>::vlanes() * 2;

        برای (; x <= len - width; x += width)
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
        بازگشت x;
    }
};

template <>
ساختار InRange_SIMD<صحیح>
{
    صحیح operator () (ثابت صحیح * src1, ثابت صحیح * src2, ثابت صحیح * src3,
        uchar * dst, صحیح len) ثابت
    {
        صحیح x = 0;
        ثابت صحیح width = (صحیح)VTraits<v_int32>::vlanes() * 2;

        برای (; x <= len - width; x += width)
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
        بازگشت x;
    }
};

template <>
ساختار InRange_SIMD<اعشاری>
{
    صحیح operator () (ثابت اعشاری * src1, ثابت اعشاری * src2, ثابت اعشاری * src3,
        uchar * dst, صحیح len) ثابت
    {
        صحیح x = 0;
        ثابت صحیح width = (صحیح)VTraits<v_float32>::vlanes() * 2;

        برای (; x <= len - width; x += width)
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
        بازگشت x;
    }
};

template <>
ساختار InRange_SIMD<hfloat>
{
    صحیح operator () (ثابت hfloat * src1, ثابت hfloat * src2, ثابت hfloat * src3,
        uchar * dst, صحیح len) ثابت
    {
        صحیح x = 0;
        ثابت صحیح width = (صحیح)VTraits<v_float32>::vlanes()*2;

        برای (; x <= len - width; x += width)
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
        بازگشت x;
    }
};

template <>
ساختار InRange_SIMD<bfloat>
{
    صحیح operator () (ثابت bfloat * src1, ثابت bfloat * src2, ثابت bfloat * src3,
        uchar * dst, صحیح len) ثابت
    {
        صحیح x = 0;
        ثابت صحیح width = (صحیح)VTraits<v_float32>::vlanes()*2;

        برای (; x <= len - width; x += width)
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
        بازگشت x;
    }
};

template <>
ساختار InRange_SIMD<unsigned>
{
    صحیح operator () (ثابت unsigned * src1, ثابت unsigned * src2, ثابت unsigned * src3,
        uchar * dst, صحیح len) ثابت
    {
        صحیح x = 0;
        ثابت صحیح width = (صحیح)VTraits<v_uint32>::vlanes() * 2;

        برای (; x <= len - width; x += width)
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
        بازگشت x;
    }
};

#اگر CV_SIMD_64F

template <>
ساختار InRange_SIMD<دوگانه>
{
    صحیح operator () (ثابت دوگانه * src1, ثابت دوگانه * src2, ثابت دوگانه * src3,
        uchar * dst, صحیح len) ثابت
    {
        صحیح x = 0;
        ثابت صحیح step = VTraits<v_float64>::vlanes();
        ثابت صحیح width = step * 4;

        برای (; x <= len - width; x += width)
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
        بازگشت x;
    }
};

#endif

#endif

template <typename T>
ایستا تهی inRange_(ثابت تهی* _src1, size_t step1, ثابت تهی* _src2, size_t step2,
         ثابت تهی* _src3, size_t step3, uchar* dst, size_t step,
         Size size)
{
    ثابت T* src1 = (ثابت T*)_src1;
    ثابت T* src2 = (ثابت T*)_src2;
    ثابت T* src3 = (ثابت T*)_src3;
    step1 /= اندازه(src1[0]);
    step2 /= اندازه(src2[0]);
    step3 /= اندازه(src3[0]);

    InRange_SIMD<T> vop;

    برای( ; size.height--; src1 += step1, src2 += step2, src3 += step3, dst += step )
    {
        صحیح x = vop(src1, src2, src3, dst, size.width);
        #اگر CV_ENABLE_UNROLLED
        برای( ; x <= size.width - 4; x += 4 )
        {
            صحیح t0, t1;
            t0 = src2[x] <= src1[x] && src1[x] <= src3[x];
            t1 = src2[x+1] <= src1[x+1] && src1[x+1] <= src3[x+1];
            dst[x] = (uchar)-t0; dst[x+1] = (uchar)-t1;
            t0 = src2[x+2] <= src1[x+2] && src1[x+2] <= src3[x+2];
            t1 = src2[x+3] <= src1[x+3] && src1[x+3] <= src3[x+3];
            dst[x+2] = (uchar)-t0; dst[x+3] = (uchar)-t1;
        }
        #endif
        برای( ; x < size.width; x++ )
            dst[x] = (uchar)-(src2[x] <= src1[x] && src1[x] <= src3[x]);
    }
}

ایستا تهی inRangeReduce(ثابت uchar* src, uchar* dst, size_t len, صحیح cn)
{
    صحیح k = cn % 4 ? cn % 4 : 4;
    size_t i, j;
    اگر( k == 1 )
        برای( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j];
    وگرنه اگر( k == 2 )
        برای( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1];
    وگرنه اگر( k == 3 )
        برای( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2];
    وگرنه
        برای( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2] & src[j+3];

    برای( ; k < cn; k += 4 )
    {
        برای( i = 0, j = k; i < len; i++, j += cn )
            dst[i] &= src[j] & src[j+1] & src[j+2] & src[j+3];
    }
}

تعریفنوع تهی (*InRangeFunc)( ثابت تهی* src1, size_t step1, ثابت تهی* src2, size_t step2,
                             ثابت تهی* src3, size_t step3, uchar* dst, size_t step, Size sz );

ایستا InRangeFunc getInRangeFunc(صحیح depth)
{
    ایستا InRangeFunc inRangeTab[CV_DEPTH_MAX] =
    {
        inRange_<uchar>,
        inRange_<schar>,
        inRange_<ushort>,
        inRange_<short>,
        inRange_<صحیح>,
        inRange_<اعشاری>,
        inRange_<دوگانه>,
        inRange_<hfloat>,
        inRange_<bfloat>,
        0,
        inRange_<uint64_t>,
        inRange_<int64_t>,
        inRange_<unsigned>,
        0,
    };

    بازگشت inRangeTab[depth];
}

#ifdef HAVE_OPENCL

ایستا منطقی ocl_inRange( درون_داد_Array _src, درون_داد_Array _lowerb,
                         درون_داد_Array _upperb, OutputArray _dst )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();
    درون_داد_Array_2::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Size ssize = _src.size(), lsize = _lowerb.size(), usize = _upperb.size();
    صحیح stype = _src.type(), ltype = _lowerb.type(), utype = _upperb.type();
    صحیح sdepth = CV_MAT_DEPTH(stype), ldepth = CV_MAT_DEPTH(ltype), udepth = CV_MAT_DEPTH(utype);
    صحیح cn = CV_MAT_CN(stype), rowsPerWI = d.isIntel() ? 4 : 1;
    منطقی lbScalar = نادرست, ubScalar = نادرست;

    اگر( (lkind == درون_داد_Array_2::MATX && skind != درون_داد_Array_2::MATX) ||
        ssize != lsize || stype != ltype )
    {
        اگر( !checkScalar(_lowerb, stype, lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = درست;
    }

    اگر( (ukind == درون_داد_Array_2::MATX && skind != درون_داد_Array_2::MATX) ||
        ssize != usize || stype != utype )
    {
        اگر( !checkScalar(_upperb, stype, ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = درست;
    }

    اگر (lbScalar != ubScalar)
        بازگشت نادرست;

    منطقی doubleSupport = d.doubleFPConfig() > 0,
            haveScalar = lbScalar && ubScalar;

    اگر ( (!doubleSupport && sdepth == CV_64F) ||
         (!haveScalar && (sdepth != ldepth || sdepth != udepth)) )
        بازگشت نادرست;

    صحیح kercn = haveScalar ? cn : std::max(std::min(ocl::predictOptimalVectorWidth(_src, _lowerb, _upperb, _dst), 4), cn);
    اگر (kercn % cn != 0)
        kercn = cn;
    صحیح colsPerWI = kercn / cn;
    String opts = format("%s-D CN=%d -D SRC_T=%s -D SRC_T1=%s -D DST_T=%s -D KERCN=%d -D DEPTH=%d%s -D COLS_PER_WI=%d",
                           haveScalar ? "-D HAVE_SCALAR " : "", cn, ocl::typeToStr(CV_MAKE_TYPE(sdepth, kercn)),
                           ocl::typeToStr(sdepth), ocl::typeToStr(CV_8UC(colsPerWI)), kercn, sdepth,
                           doubleSupport ? " -D DOUBLE_SUPPORT" : "", colsPerWI);

    ocl::Kernel ker("inrange", ocl::core::inrange_oclsrc, opts);
    اگر (ker.empty())
        بازگشت نادرست;

    _dst.create(ssize, CV_8UC1);
    UMat src = _src.getUMat(), dst = _dst.getUMat(), lscalaru, uscalaru;
    Mat lscalar, uscalar;

    اگر (lbScalar && ubScalar)
    {
        lscalar = _lowerb.getMat();
        uscalar = _upperb.getMat();

        size_t esz = src.elemSize();
        size_t blocksize = 36;

        AutoBuffer<uchar> _buf(blocksize*(((صحیح)lbScalar + (صحیح)ubScalar)*esz + cn) + 2*cn*اندازه(صحیح) + 128);
        uchar *buf = alignPtr(_buf.data() + blocksize*cn, 16);

        اگر( ldepth != sdepth && sdepth < CV_32S )
        {
            صحیح* ilbuf = (صحیح*)alignPtr(buf + blocksize*esz, 16);
            صحیح* iubuf = ilbuf + cn;

            دودویی_Func sccvtfunc = getConvertFunc(ldepth, CV_32S);
            sccvtfunc(lscalar.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, Size(cn, 1), 0);
            sccvtfunc(uscalar.ptr(), 1, 0, 1, (uchar*)iubuf, 1, Size(cn, 1), 0);
            صحیح minval = cvRound(getMinVal(sdepth)), maxval = cvRound(getMaxVal(sdepth));

            برای( صحیح k = 0; k < cn; k++ )
            {
                اگر( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lscalar = Mat(cn, 1, CV_32S, ilbuf);
            uscalar = Mat(cn, 1, CV_32S, iubuf);
        }

        lscalar.convertTo(lscalar, stype);
        uscalar.convertTo(uscalar, stype);
    }
    وگرنه
    {
        lscalaru = _lowerb.getUMat();
        uscalaru = _upperb.getUMat();
    }

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, 1, colsPerWI);

    اگر (haveScalar)
    {
        lscalar.copyTo(lscalaru);
        uscalar.copyTo(uscalaru);

        ker.args(srcarg, dstarg, ocl::KernelArg::PtrReadOnly(lscalaru),
               ocl::KernelArg::PtrReadOnly(uscalaru), rowsPerWI);
    }
    وگرنه
        ker.args(srcarg, dstarg, ocl::KernelArg::ReadOnlyNoSize(lscalaru),
               ocl::KernelArg::ReadOnlyNoSize(uscalaru), rowsPerWI);

    size_t globalsize[2] = { (size_t)ssize.width / colsPerWI, ((size_t)ssize.height + rowsPerWI - 1) / rowsPerWI };
    بازگشت ker.run(2, globalsize, پوچ, نادرست);
}

#endif

}

تهی cv::inRange(درون_داد_Array _src, درون_داد_Array _lowerb,
                 درون_داد_Array _upperb, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(! _src.empty());
    اگر (!getInRangeFunc(_src.depth()))
        CV_Error( cv::Error::StsNotImplemented,
                 "inRange: CV_Bool and CV_8F inputs are not supported, convert the input to another type first");

    CV_OCL_RUN(_src.dims() <= 2 && _lowerb.dims() <= 2 &&
               _upperb.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat()),
               ocl_inRange(_src, _lowerb, _upperb, _dst))

    درون_داد_Array_2::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Mat src = _src.getMat(), lb = _lowerb.getMat(), ub = _upperb.getMat();

    منطقی lbScalar = نادرست, ubScalar = نادرست;

    اگر( (lkind == درون_داد_Array_2::MATX && skind != درون_داد_Array_2::MATX) ||
        src.size != lb.size || src.type() != lb.type() )
    {
        اگر( !checkScalar(lb, src.type(), lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = درست;
    }

    اگر( (ukind == درون_داد_Array_2::MATX && skind != درون_داد_Array_2::MATX) ||
        src.size != ub.size || src.type() != ub.type() )
    {
        اگر( !checkScalar(ub, src.type(), ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = درست;
    }

    CV_Assert(lbScalar == ubScalar);

    صحیح cn = src.channels(), depth = src.depth();

    size_t esz = src.elemSize();
    size_t blocksize0 = (size_t)(BLOCK_SIZE + esz-1)/esz;

    _dst.createSameSize(_src, CV_8UC1);
    Mat dst = _dst.getMat();
    InRangeFunc func = getInRangeFunc(depth);

    ثابت Mat* arrays_sc[] = { &src, &dst, 0 };
    ثابت Mat* arrays_nosc[] = { &src, &dst, &lb, &ub, 0 };
    uchar* ptrs[4] = {};

    NAryMatIterator it(lbScalar && ubScalar ? arrays_sc : arrays_nosc, ptrs);
    size_t total = it.size, blocksize = std::min(total, blocksize0);

    AutoBuffer<uchar> _buf(blocksize*(((صحیح)lbScalar + (صحیح)ubScalar)*esz + cn) + 2*cn*اندازه(صحیح) + 128);
    uchar *buf = _buf.data(), *mbuf = buf, *lbuf = 0, *ubuf = 0;
    buf = alignPtr(buf + blocksize*cn, 16);

    اگر( lbScalar && ubScalar )
    {
        lbuf = buf;
        ubuf = buf = alignPtr(buf + blocksize*esz, 16);

        CV_Assert( lb.type() == ub.type() );
        صحیح scdepth = lb.depth();

        اگر( scdepth != depth && depth < CV_32S )
        {
            صحیح* ilbuf = (صحیح*)alignPtr(buf + blocksize*esz, 16);
            صحیح* iubuf = ilbuf + cn;

            دودویی_Func sccvtfunc = getConvertFunc(scdepth, CV_32S);
            sccvtfunc(lb.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, Size(cn, 1), 0);
            sccvtfunc(ub.ptr(), 1, 0, 1, (uchar*)iubuf, 1, Size(cn, 1), 0);
            صحیح minval = cvRound(getMinVal(depth)), maxval = cvRound(getMaxVal(depth));

            برای( صحیح k = 0; k < cn; k++ )
            {
                اگر( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lb = Mat(cn, 1, CV_32S, ilbuf);
            ub = Mat(cn, 1, CV_32S, iubuf);
        }

        convertAndUnrollScalar( lb, src.type(), lbuf, blocksize );
        convertAndUnrollScalar( ub, src.type(), ubuf, blocksize );

        اگر (depth == CV_8U && src.dims <= 2) {
            uint8_t lb_scalar = lbuf[0];
            uint8_t ub_scalar = ubuf[0];
            CALL_HAL(inRange_u8, cv_hal_inRange8u, src.data, src.step, dst.data, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        } وگرنه اگر (depth == CV_32F && src.dims <= 2) {
            دوگانه lb_scalar = lb.ptr<دوگانه>(0)[0];
            دوگانه ub_scalar = ub.ptr<دوگانه>(0)[0];
            CALL_HAL(inRange_f32, cv_hal_inRange32f, src.data, src.step, dst.data, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        }
    }

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برای( size_t j = 0; j < total; j += blocksize )
        {
            صحیح bsz = (صحیح)MIN(total - j, blocksize);
            size_t delta = bsz*esz;
            uchar *lptr = lbuf, *uptr = ubuf;
            اگر( !lbScalar )
            {
                lptr = ptrs[2];
                ptrs[2] += delta;
            }
            اگر( !ubScalar )
            {
                صحیح idx = !lbScalar ? 3 : 2;
                uptr = ptrs[idx];
                ptrs[idx] += delta;
            }
            func( ptrs[0], 0, lptr, 0, uptr, 0, cn == 1 ? ptrs[1] : mbuf, 0, Size(bsz*cn, 1));
            اگر( cn > 1 )
                inRangeReduce(mbuf, ptrs[1], bsz, cn);
            ptrs[0] += delta;
            ptrs[1] += bsz;
        }
    }
}
/* End of file. */
