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

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "arithm_expr.hpp"   // the new element-wise engine (cv::ew)
#அந்தர்பாவ "opencl_kernels_core.hpp"

நாமாகாஶ cv
{

/****************************************************************************************\
*                                   logical operations                                   *
\****************************************************************************************/

எண்ணிக்கை { OCL_OP_ADD=0, OCL_OP_SUB=1, OCL_OP_RSUB=2, OCL_OP_ABSDIFF=3, OCL_OP_MUL=4,
       OCL_OP_MUL_SCALE=5, OCL_OP_DIV_SCALE=6, OCL_OP_RECIP_SCALE=7, OCL_OP_ADDW=8,
       OCL_OP_AND=9, OCL_OP_OR=10, OCL_OP_XOR=11, OCL_OP_NOT=12, OCL_OP_MIN=13, OCL_OP_MAX=14,
       OCL_OP_RDIV_SCALE=15 };

// The unified entry point for every element-wise binary op (empty + UMat/OpenCL + CPU engine). Defined
// lower down (near cv::add); forward-declared here so cv::min/cv::max (above it) can use it too.
நிலையான வெறுமை arithm_op(ew::உச்சி op, உள்ளீடு_Array src1, உள்ளீடு_Array src2, OutputArray dst,
                      உள்ளீடு_Array mask, முழு dtype, முழு oclop, தருக்க muldiv, மாறிலி Scalar& params = Scalar(1));

// The unary counterpart (bitwise NOT, unary math later). Forward-declared for the same reason.
நிலையான வெறுமை unary_op(ew::உச்சி op, உள்ளீடு_Array src, OutputArray dst, உள்ளீடு_Array mask,
                     முழு dtype, மாறிலி Scalar& params = Scalar());

#ifdef HAVE_OPENCL

நிலையான மாறிலி எழுத்து* oclop2str[] = { "OP_ADD", "OP_SUB", "OP_RSUB", "OP_ABSDIFF",
    "OP_MUL", "OP_MUL_SCALE", "OP_DIV_SCALE", "OP_RECIP_SCALE",
    "OP_ADDW", "OP_AND", "OP_OR", "OP_XOR", "OP_NOT", "OP_MIN", "OP_MAX", "OP_RDIV_SCALE", 0 };

// depths with no OpenCL type name (ocl::typeToStr() asserts on them)
நிலையான உள்வரி தருக்க noOclType(முழு depth)
{
    திரும்பு depth == CV_16BF || depth == CV_Bool || depth == CV_8F_E4M3FN || depth == CV_8F_E4M3FNUZ;
}

நிலையான தருக்க ocl_இருமம்_op(உள்ளீடு_Array _src1, உள்ளீடு_Array _src2, OutputArray _dst,
                          உள்ளீடு_Array _mask, தருக்க bitwise, முழு oclop, தருக்க haveScalar )
{
    தருக்க haveMask = !_mask.empty();
    முழு srctype = _src1.type();
    முழு srcdepth = CV_MAT_DEPTH(srctype);
    முழு cn = CV_MAT_CN(srctype);

    மாறிலி ocl::Device d = ocl::Device::getDefault();
    தருக்க doubleSupport = d.doubleFPConfig() > 0;
    எனில்( oclop < 0 || ((haveMask || haveScalar) && cn > 4) ||
            (!doubleSupport && srcdepth == CV_64F && !bitwise) ||
            (noOclType(srcdepth) && !bitwise))
        திரும்பு பொய்;

    எழுத்து opts[1024];
    முழு kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    முழு scalarcn = kercn == 3 ? 4 : kercn;
    முழு rowsPerWI = d.isIntel() ? 4 : 1;

    மாறிலி முழு dstDepth = srcdepth;
    மாறிலி முழு dstType = CV_MAKETYPE(dstDepth, kercn);
    மாறிலி முழு dstType1 = CV_MAKETYPE(dstDepth, 1);
    மாறிலி முழு scalarType = CV_MAKETYPE(srcdepth, scalarcn);

    snprintf(opts, அளவு(opts), "-D %s%s -D %s%s -D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workST=%s -D cn=%d -D rowsPerWI=%d",
            haveMask ? "MASK_" : "", haveScalar ? "UNARY_OP" : "BINARY_OP", oclop2str[oclop],
            doubleSupport ? " -D DOUBLE_SUPPORT" : "",
            bitwise ? ocl::memopTypeToStr(dstType) : ocl::typeToStr(dstType),
            dstDepth,
            bitwise ? ocl::memopTypeToStr(dstType1) : ocl::typeToStr(dstType1),
            bitwise ? ocl::memopTypeToStr(scalarType) : ocl::typeToStr(scalarType),
            kercn, rowsPerWI);

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    எனில் (k.empty())
        திரும்பு பொய்;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask, 1);

    எனில்( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(srctype)*scalarcn;
        இரட்டை buf[4] = {0,0,0,0};

        எனில்( oclop != OCL_OP_NOT )
        {
            Mat src2sc = _src2.getMat();
            convertAndUnrollScalar(src2sc, srctype, (uchar*)buf, 1);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        எனில்( !haveMask )
            k.args(src1arg, dstarg, scalararg);
        இல்லையெனில்
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    இல்லையெனில்
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::ReadOnlyNoSize(src2, cn, kercn);

        எனில்( !haveMask )
            k.args(src1arg, src2arg, dstarg);
        இல்லையெனில்
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    திரும்பு k.run(2, globalsize, 0, பொய்);
}

#endif

// OpenCL path for the bitwise ops (and/or/xor/not), lifted out of binary_op: choose the (possibly
// scalar) operand order, size the destination, and dispatch the shared bitwise OpenCL kernel. Returns
// true if OpenCL handled the op, false to fall through to the CPU engine. NOT arrives unary
// (src2 == src1, forced-scalar). The caller gates this on a UMat operand + dims <= 2 (via CV_OCL_RUN).
#ifdef HAVE_OPENCL
நிலையான தருக்க bitwise_op_ocl(உள்ளீடு_Array _src1, உள்ளீடு_Array _src2, OutputArray _dst,
                           உள்ளீடு_Array _mask, முழு oclop)
{
    மாறிலி உள்ளீடு_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    உள்ளீடு_Array_2::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    முழு type1 = psrc1->type(), type2 = psrc2->type();
    தருக்க haveScalar = பொய்;

    எனில்( oclop == OCL_OP_NOT )
        haveScalar = உண்மை;
    இல்லையெனில் எனில்( (kind1 == உள்ளீடு_Array_2::MATX) + (kind2 == உள்ளீடு_Array_2::MATX) == 1 ||
             !psrc1->sameSize(*psrc2) || type1 != type2 )
    {
        எனில்( checkScalar(*psrc1, type2, kind1, kind2) )
        { std::swap(psrc1, psrc2); std::swap(type1, type2); }        // src1 is the scalar; swap it out
        இல்லையெனில் எனில்( !checkScalar(*psrc2, type1, kind2, kind1) )
            திரும்பு பொய்;
        haveScalar = உண்மை;
    }
    _dst.createSameSize(*psrc1, type1);
    திரும்பு ocl_இருமம்_op(*psrc1, *psrc2, _dst, _mask, உண்மை, oclop, haveScalar);
}
#endif  // HAVE_OPENCL

}

// bitwise and/or/xor: OpenCL runs the shared bitwise kernel (via bitwise_op_ocl); the CPU path is the
// element-wise engine (OP_AND/OR/XOR, T x T -> T dispatched by element size). oclop = -1 tells
// arithm_op the OpenCL path was already tried, so it only runs the CPU engine. NOT is unary.
வெறுமை cv::bitwise_and(உள்ளீடு_Array a, உள்ளீடு_Array b, OutputArray c, உள்ளீடு_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_AND))
    arithm_op(ew::OP_AND, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/பொய்);
}

வெறுமை cv::bitwise_or(உள்ளீடு_Array a, உள்ளீடு_Array b, OutputArray c, உள்ளீடு_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_OR))
    arithm_op(ew::OP_OR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/பொய்);
}

வெறுமை cv::bitwise_xor(உள்ளீடு_Array a, உள்ளீடு_Array b, OutputArray c, உள்ளீடு_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_XOR))
    arithm_op(ew::OP_XOR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/பொய்);
}

வெறுமை cv::bitwise_not(உள்ளீடு_Array a, OutputArray c, உள்ளீடு_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || c.isUMat()) && a.dims() <= 2,
               bitwise_op_ocl(a, a, c, mask, OCL_OP_NOT))
    unary_op(ew::OP_NOT, a, c, mask, -1);
}

வெறுமை cv::max( உள்ளீடு_Array src1, உள்ளீடு_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, பொய்);
}

வெறுமை cv::min( உள்ளீடு_Array src1, உள்ளீடு_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, பொய்);
}

// The concrete Mat/UMat overloads of min/max exist because C++ overload resolution needs them (a
// bare cv::min(mat, mat, mat) binds Mat& more tightly than InputArray); they must run the SAME engine
// path as the InputArray forms - NOT the legacy binary_op - so mixed types and broadcasting work.
வெறுமை cv::max(மாறிலி Mat& src1, மாறிலி Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, பொய்);
}

வெறுமை cv::min(மாறிலி Mat& src1, மாறிலி Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, பொய்);
}

வெறுமை cv::max(மாறிலி UMat& src1, மாறிலி UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, பொய்);
}

வெறுமை cv::min(மாறிலி UMat& src1, மாறிலி UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, பொய்);
}


/****************************************************************************************\
*                                      add/subtract                                      *
\****************************************************************************************/

நாமாகாஶ cv
{

#ifdef HAVE_OPENCL

// used by the OpenCL branch only (arithm_op_ocl working-type selection)
நிலையான முழு actualScalarDepth(மாறிலி இரட்டை* data, முழு len)
{
    முழு i = 0, minval = INT_MAX, maxval = INT_MIN;
    க்கு(; i < len; ++i)
    {
        முழு ival = cvRound(data[i]);
        எனில்( ival != data[i] )
            நிறுத்து;
        minval = MIN(minval, ival);
        maxval = MAX(maxval, ival);
    }
    திரும்பு i < len ? CV_64F :
        minval >= 0 && maxval <= (முழு)UCHAR_MAX ? CV_8U :
        minval >= (முழு)SCHAR_MIN && maxval <= (முழு)SCHAR_MAX ? CV_8S :
        minval >= 0 && maxval <= (முழு)USHRT_MAX ? CV_16U :
        minval >= (முழு)SHRT_MIN && maxval <= (முழு)SHRT_MAX ? CV_16S :
        CV_32S;
}

நிலையான முழு coerceTypes(முழு depth1, முழு depth2, தருக்க muldiv)
{
    திரும்பு depth1 == depth2 ? depth1 :
        ((depth1 <= CV_32S) & (depth2 <= CV_32S)) != 0 ?
        (((முழு)!muldiv & (depth1 <= CV_8S) & (depth2 <= CV_8S)) != 0 ? CV_16S : CV_32S) :
        ((CV_ELEM_SIZE1(depth1) > 4) | (CV_ELEM_SIZE1(depth2) > 4)) != 0 ? CV_64F : CV_32F;
}

நிலையான தருக்க ocl_arithm_op(உள்ளீடு_Array _src1, உள்ளீடு_Array _src2, OutputArray _dst,
                          உள்ளீடு_Array _mask, முழு wtype,
                          வெறுமை* usrdata, முழு oclop,
                          தருக்க haveScalar )
{
    மாறிலி ocl::Device d = ocl::Device::getDefault();
    தருக்க doubleSupport = d.doubleFPConfig() > 0;
    முழு type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    தருக்க haveMask = !_mask.empty();

    எனில் ( (haveMask || haveScalar) && cn > 4 )
        திரும்பு பொய்;

#ifdef __APPLE__
    எனில் (depth1 == CV_16U && (oclop == OCL_OP_MUL || oclop == OCL_OP_MUL_SCALE))
        திரும்பு பொய்;
#endif

    முழு dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32S, CV_MAT_DEPTH(wtype));
    எனில் (!doubleSupport)
        wdepth = std::min(wdepth, CV_32F);

    wtype = CV_MAKETYPE(wdepth, cn);
    முழு type2 = haveScalar ? wtype : _src2.type(), depth2 = CV_MAT_DEPTH(type2);
    எனில் (!doubleSupport && (depth2 == CV_64F || depth1 == CV_64F))
        திரும்பு பொய்;

    எனில் (noOclType(depth1) || noOclType(depth2) || noOclType(ddepth) || noOclType(wdepth))
        திரும்பு பொய்;

    முழு kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    முழு scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = d.isIntel() ? 4 : 1;

    எழுத்து cvtstr[4][50], opts[1024];
    snprintf(opts, அளவு(opts), "-D %s%s -D %s -D srcT1=%s -D srcT1_C1=%s -D srcT2=%s -D srcT2_C1=%s "
            "-D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workT=%s -D workST=%s -D scaleT=%s -D wdepth=%d -D convertToWT1=%s "
            "-D convertToWT2=%s -D convertToDT=%s%s -D cn=%d -D rowsPerWI=%d -D convertFromU=%s",
            (haveMask ? "MASK_" : ""), (haveScalar ? "UNARY_OP" : "BINARY_OP"),
            oclop2str[oclop], ocl::typeToStr(CV_MAKETYPE(depth1, kercn)),
            ocl::typeToStr(depth1), ocl::typeToStr(CV_MAKETYPE(depth2, kercn)),
            ocl::typeToStr(depth2), ocl::typeToStr(CV_MAKETYPE(ddepth, kercn)), ddepth,
            ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKETYPE(wdepth, kercn)),
            ocl::typeToStr(CV_MAKETYPE(wdepth, scalarcn)),
            ocl::typeToStr(wdepth), wdepth,
            ocl::convertTypeStr(depth1, wdepth, kercn, cvtstr[0], அளவு(cvtstr[0])),
            ocl::convertTypeStr(depth2, wdepth, kercn, cvtstr[1], அளவு(cvtstr[1])),
            ocl::convertTypeStr(wdepth, ddepth, kercn, cvtstr[2], அளவு(cvtstr[2])),
            doubleSupport ? " -D DOUBLE_SUPPORT" : "", kercn, rowsPerWI,
            oclop == OCL_OP_ABSDIFF && wdepth == CV_32S && ddepth == wdepth ?
            ocl::convertTypeStr(CV_8U, ddepth, kercn, cvtstr[3], அளவு(cvtstr[3])) : "noconvert");

    size_t usrdata_esz = CV_ELEM_SIZE(wdepth);
    மாறிலி uchar* usrdata_p = (மாறிலி uchar*)usrdata;
    மாறிலி இரட்டை* usrdata_d = (மாறிலி இரட்டை*)usrdata;
    தசம usrdata_f[3];
    முழு i, n = oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE ||
        oclop == OCL_OP_RDIV_SCALE || oclop == OCL_OP_RECIP_SCALE ? 1 : oclop == OCL_OP_ADDW ? 3 : 0;
    எனில்( usrdata && n > 0 && wdepth == CV_32F )
    {
        க்கு( i = 0; i < n; i++ )
            usrdata_f[i] = (தசம)usrdata_d[i];
        usrdata_p = (மாறிலி uchar*)usrdata_f;
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    எனில் (k.empty())
        திரும்பு பொய்;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask, 1);

    எனில்( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(wtype)*scalarcn;
        இரட்டை buf[4]={0,0,0,0};
        Mat src2sc = _src2.getMat();

        எனில்( !src2sc.empty() )
            convertAndUnrollScalar(src2sc, wtype, (uchar*)buf, 1);
        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        எனில்( !haveMask )
        {
            எனில்(n == 0)
                k.args(src1arg, dstarg, scalararg);
            இல்லையெனில் எனில்(n == 1)
                k.args(src1arg, dstarg, scalararg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            இல்லையெனில்
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        இல்லையெனில்
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    இல்லையெனில்
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::ReadOnlyNoSize(src2, cn, kercn);

        எனில்( !haveMask )
        {
            எனில் (n == 0)
                k.args(src1arg, src2arg, dstarg);
            இல்லையெனில் எனில் (n == 1)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            இல்லையெனில் எனில் (n == 3)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz*2, usrdata_esz));
            இல்லையெனில்
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        இல்லையெனில்
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    திரும்பு k.run(2, globalsize, வெற்று, பொய்);
}

#endif

// OpenCL branch of the arithmetic ops, extracted from the former arithm_op (whose CPU compute is now
// the element-wise engine). Returns true if an OpenCL kernel handled the op, false to let the caller
// fall through to the CPU engine. min/max use the bitwise-style kernel; every other op the arithm
// kernel with a coerced working type. `usrdata` carries the mul/div/addWeighted scale block, `oclop`
// the kernel id. Only reached for a UMat dst; sizes/types that don't fit the scalar/same-size pattern
// (e.g. a broadcast) decline (return false) so the engine handles them on the CPU.
நிலையான தருக்க arithm_op_ocl(உள்ளீடு_Array _src1, உள்ளீடு_Array _src2, OutputArray _dst,
                          உள்ளீடு_Array _mask, முழு dtype, முழு oclop, தருக்க muldiv, வெறுமை* usrdata)
{
#ifndef HAVE_OPENCL
    CV_UNUSED(_src1); CV_UNUSED(_src2); CV_UNUSED(_dst); CV_UNUSED(_mask);
    CV_UNUSED(dtype); CV_UNUSED(oclop); CV_UNUSED(muldiv); CV_UNUSED(usrdata);
    திரும்பு பொய்;
#இல்லையெனில்
    மாறிலி உள்ளீடு_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    உள்ளீடு_Array_2::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    தருக்க haveMask = !_mask.empty();
    தருக்க reallocate = பொய்;
    முழு type1 = psrc1->type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    முழு type2 = psrc2->type(), depth2 = CV_MAT_DEPTH(type2), cn2 = CV_MAT_CN(type2);
    முழு wtype, dims1 = psrc1->dims(), dims2 = psrc2->dims();
    Size sz1 = dims1 <= 2 ? psrc1->size() : Size();
    Size sz2 = dims2 <= 2 ? psrc2->size() : Size();

    எனில் (!(OCL_PERFORMANCE_CHECK(_dst.isUMat()) && dims1 <= 2 && dims2 <= 2))
        திரும்பு பொய்;

    // min/max: the bitwise-style OpenCL kernel (same-type element-wise, no working-type coercion).
    எனில் (oclop == OCL_OP_MIN || oclop == OCL_OP_MAX)
    {
        தருக்க haveScalar = பொய்;
        எனில் ((kind1 == உள்ளீடு_Array_2::MATX) + (kind2 == உள்ளீடு_Array_2::MATX) == 1 ||
            !psrc1->sameSize(*psrc2) || type1 != type2)
        {
            எனில் (checkScalar(*psrc1, type2, kind1, kind2))
            { std::swap(psrc1, psrc2); std::swap(type1, type2); }
            இல்லையெனில் எனில் (!checkScalar(*psrc2, type1, kind2, kind1))
                திரும்பு பொய்;
            haveScalar = உண்மை;
        }
        _dst.createSameSize(*psrc1, type1);
        திரும்பு ocl_இருமம்_op(*psrc1, *psrc2, _dst, _mask, பொய், oclop, haveScalar);
    }

    // reciprocal (scale/src): the OpenCL kernel is unary on src - use src2 for both operands (the CPU
    // engine received a 0-dim `1` numerator as src1, which the recip kernel would ignore). [TODO.VP: tidy]
    எனில் (oclop == OCL_OP_RECIP_SCALE)
    {
        psrc1 = psrc2; kind1 = kind2; type1 = type2; depth1 = depth2; cn = cn2; dims1 = dims2; sz1 = sz2;
    }

    // add/sub/mul/div/absdiff/addWeighted/recip: the arithm OpenCL kernel with a coerced work type.
    தருக்க src1Scalar = checkScalar(*psrc1, type2, kind1, kind2);
    தருக்க src2Scalar = checkScalar(*psrc2, type1, kind2, kind1);
    தருக்க haveScalar = பொய்;

    எனில் (dims1 != dims2 || sz1 != sz2 || cn != cn2 ||
        (kind1 == உள்ளீடு_Array_2::MATX && (sz1 == Size(1,4) || sz1 == Size(1,1))) ||
        (kind2 == உள்ளீடு_Array_2::MATX && (sz2 == Size(1,4) || sz2 == Size(1,1))))
    {
        எனில் ((type1 == CV_64F && (sz1.height == 1 || sz1.height == 4)) && src1Scalar)
        {
            // src1 is a scalar; swap it with src2
            std::swap(psrc1, psrc2); std::swap(sz1, sz2); std::swap(type1, type2);
            std::swap(depth1, depth2); std::swap(cn, cn2); std::swap(dims1, dims2);
            எனில் (oclop == OCL_OP_SUB) oclop = OCL_OP_RSUB;
            எனில் (oclop == OCL_OP_DIV_SCALE) oclop = OCL_OP_RDIV_SCALE;
        }
        இல்லையெனில் எனில் (!src2Scalar)
            திரும்பு பொய்;   // array op array with mismatched size/cn: engine broadcasts on the CPU
        haveScalar = உண்மை;
        CV_Assert((type2 == CV_64F || type2 == CV_32F) && (sz2.height == 1 || sz2.height == 4));

        எனில் (!muldiv)
        {
            Mat sc = psrc2->getMat();
            depth2 = actualScalarDepth(sc.ptr<இரட்டை>(), sz2 == Size(1, 1) ? cn2 : cn);
            எனில் (depth2 == CV_64F && CV_ELEM_SIZE1(depth1) < 8)
                depth2 = CV_32F;
        }
        இல்லையெனில்
            depth2 = CV_64F;
    }

    எனில் (dtype < 0)
    {
        எனில் (_dst.fixedType())
            dtype = _dst.type();
        இல்லையெனில்
        {
            எனில் (!haveScalar && type1 != type2)
                CV_Error(cv::Error::StsBadArg,
                     "When the input arrays in add/subtract/multiply/divide functions have different types, "
                     "the output array type must be explicitly specified");
            dtype = type1;
        }
    }
    dtype = CV_MAT_DEPTH(dtype);

    எனில் (depth1 == depth2 && dtype == depth1)
        wtype = dtype;
    இல்லையெனில் எனில் (!muldiv)
    {
        wtype = coerceTypes(depth1, depth2, பொய்);
        wtype = coerceTypes(wtype, dtype, பொய்);
        எனில் (dtype < CV_32F && (depth1 < CV_32F || depth2 < CV_32F))
            wtype = CV_32S;
    }
    இல்லையெனில்
    {
        wtype = coerceTypes(depth1, depth2, உண்மை);
        wtype = coerceTypes(wtype, dtype, உண்மை);
    }

    // The scaled OpenCL kernels (mul/div by a scale, reciprocal, addWeighted) compute in float; the old
    // same-type fast path forced max(depth, CV_32F). coerceTypes keeps same-type integer, so bump here.
    எனில் (oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE || oclop == OCL_OP_RDIV_SCALE ||
        oclop == OCL_OP_RECIP_SCALE || oclop == OCL_OP_ADDW)
        wtype = std::max(wtype, (முழு)CV_32F);

    dtype = CV_MAKETYPE(dtype, cn);
    wtype = CV_MAKETYPE(wtype, cn);

    எனில் (haveMask)
    {
        முழு mtype = _mask.type();
        CV_Assert((mtype == CV_8UC1 || mtype == CV_8SC1 || mtype == CV_Bool) && _mask.sameSize(*psrc1));
        reallocate = !_dst.sameSize(*psrc1) || _dst.type() != dtype;
    }

    _dst.createSameSize(*psrc1, dtype);
    எனில் (reallocate)
        _dst.setTo(0.);

    திரும்பு ocl_arithm_op(*psrc1, *psrc2, _dst, _mask, wtype, usrdata, oclop, haveScalar);
#endif
}

// Element-wise binary op (ADD/SUB/...) on two CPU operands - each an array Mat or a MATX scalar -
// through the new broadcasting engine. Handles mixed input types (emitBinary inserts the casts) and
// broadcasting (a scalar rides as a 0-dim per-channel CONST; arrays broadcast via outputShape/exec).
// Returns false (declining) for UMat, a write-mask, or two scalar operands, so the caller falls
// through to arithm_op. Incompatible shapes make the engine throw (matching cv::add's error).
// Does the output array already have exactly this shape+type (=> create() reuses it, no realloc)?
// Works for Mat and UMat alike (shape/type queries, no data-pointer peeking).
நிலையான தருக்க outArrayMatches(மாறிலி _OutputArray& a, மாறிலி MatShape& shp, முழு type)
{
    எனில் (a.empty() || a.type() != type)
        திரும்பு பொய்;
    முழு sz[MatShape::MAX_DIMS];
    முழு nd = a.sizend(sz);
    எனில் (nd != (முழு)shp.size())
        திரும்பு பொய்;
    க்கு (முழு i = 0; i < nd; i++)
        எனில் (sz[i] != shp[i])
            திரும்பு பொய்;
    திரும்பு உண்மை;
}

// Are two operands' iteration shapes (spatial dims + channels innermost, exactly how the engine
// iterates) numpy-broadcast-compatible? Powers the scalar-like-Mat compat fallback in arithm_op.
// Cheap by design: sizend copies the dim arrays inline - no Mat headers, no allocations.
நிலையான தருக்க shapesBroadcastCompat(உள்ளீடு_Array a, முழு acn, உள்ளீடு_Array b, முழு bcn)
{
    முழு asz[MatShape::MAX_DIMS + 1], bsz[MatShape::MAX_DIMS + 1];
    முழு ad = a.sizend(asz), bd = b.sizend(bsz);
    asz[ad++] = acn;
    bsz[bd++] = bcn;
    க்கு (முழு i1 = ad - 1, i2 = bd - 1; i1 >= 0 && i2 >= 0; i1--, i2--)
        எனில் (asz[i1] != bsz[i2] && asz[i1] != 1 && bsz[i2] != 1)
            திரும்பு பொய்;
    திரும்பு உண்மை;
}

// The one entry point for every element-wise binary op (add/sub/mul/div/min/max/absdiff/addWeighted/
// reciprocal). Handles empty inputs, the UMat/OpenCL branch (arithm_op_ocl) and the CPU element-wise
// engine. `oclop` selects the OpenCL kernel; `muldiv` drives the OpenCL working-type coercion + scale;
// `params` are the op scalars (params[0]=mul/div scale; {alpha,beta,gamma} for addWeighted).
// Below this many ELEMENTS (total * channels), the per-call constant overhead of the engine path
// (program build + compile + executor setup) is comparable to the actual work - take the direct
// kernel call instead. Same tier boundary as math_op's MATH_OP_SMALL for cv::exp.
எண்ணிக்கை { ARITHM_SMALL_DIRECT = 100000 };

நிலையான வெறுமை arithm_op(ew::உச்சி op, உள்ளீடு_Array src1, உள்ளீடு_Array src2, OutputArray dst,
                      உள்ளீடு_Array mask, முழு dtype, முழு oclop, தருக்க muldiv, மாறிலி Scalar& params)
{
    எனில் (src1.empty() || src2.empty())          // empty inputs -> empty result
    {
        CV_Assert((src1.empty() && src2.empty()) ||
                  (src1.empty() && isScalarArg(src2, src1.channels())) ||
                  (src2.empty() && isScalarArg(src1, src2.channels())));
        dst.release();
        எனில் (dtype >= 0)
            dst.create(0, 0, dtype);
        திரும்பு;
    }

    // UMat -> classic OpenCL kernel. It declines (false) when OpenCL can't apply (no device, dims>2, a
    // broadcast, cn>4 masked, ...) - then the CPU engine below handles it (mapping the UMat via getMat).
    // oclop < 0 is a sentinel: the caller (e.g. cv::compare) already ran its own OpenCL path, so this
    // helper only runs the CPU engine (a UMat operand is mapped via getMat).
    எனில் (oclop >= 0 && cv::ocl::isOpenCLActivated() &&
        (src1.isUMat() || src2.isUMat() || dst.isUMat()))
    {
        இரட்டை abg[3] = { params[0], params[1], params[2] }, scale = params[0];
        வெறுமை* usrdata = (op == ew::OP_ADDW) ? (வெறுமை*)abg : (muldiv ? (வெறுமை*)&scale : nullptr);
        எனில் (arithm_op_ocl(src1, src2, dst, mask, dtype, oclop, muldiv, usrdata))
            திரும்பு;
    }

    மாறிலி தருக்க haveMask = !mask.empty();
    மாறிலி முழு cn1 = src1.channels(), cn2 = src2.channels();
    தருக்க s1 = isScalarArg(src1, src2), s2 = isScalarArg(src2, src1);
    எனில் (s1 && s2)
        s1 = s2 = பொய்;    // two scalars: treat both as tiny array operands (element-wise + broadcast),
                            // matching arithm_op (Scalar+Scalar -> 4x1, number+number -> 1x1, ...)
    // A 4x1 CV_64F Mat/UMat pseudo-scalar (see isScalarArg) is hijacked only against a REAL array:
    // when the partner is itself a tiny scalar-shaped array (e.g. compare(Mat 4x1, Mat 1x1) -
    // issue #8999), both are honest data and ride the broadcast. A genuine MATX Scalar never demotes.
    இல்லையெனில் எனில் (s1 && src1.kind() != உள்ளீடு_Array_2::MATX && isScalarLikeMat(src2, cn1))
        s1 = பொய்;
    இல்லையெனில் எனில் (s2 && src2.kind() != உள்ளீடு_Array_2::MATX && isScalarLikeMat(src1, cn2))
        s2 = பொய்;

    // Compat fallback: Java/Python/user code passes scalars as real little Mats (the classic 4x1
    // CV_64F column, a 1xcn/cnx1 vector - see isScalarLikeMat). Those normally ride broadcasting
    // now, so hijack one as a per-channel scalar ONLY when the shapes do NOT broadcast - just the
    // calls that would otherwise throw get the 4.x scalar semantics, every valid broadcast keeps
    // its numpy meaning. Cheap: two geometry probes, and the O(ndims) shape walk runs only when a
    // probe hits (never for ordinary same-size or MATX-scalar calls).
    எனில் (!s1 && !s2)
    {
        தருக்க like1 = isScalarLikeMat(src1, cn2), like2 = isScalarLikeMat(src2, cn1);
        எனில் ((like1 || like2) && !shapesBroadcastCompat(src1, cn1, src2, cn2))
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
    மாறிலி வெறுமை* dstObj = (dst.kind() == உள்ளீடு_Array_2::MAT) ? dst.getObj() : nullptr;
    மாறிலி தருக்க aliased = dstObj && (dstObj == src1.getObj() || dstObj == src2.getObj());
    Mat m1loc, m2loc, mloc;
    auto asMat = [&](உள்ளீடு_Array a, Mat& loc) -> மாறிலி Mat* {
        எனில் (a.kind() == உள்ளீடு_Array_2::MAT && !aliased) திரும்பு (மாறிலி Mat*)a.getObj();
        loc = a.getMat(); திரும்பு &loc;
    };
    மாறிலி Mat* pm1 = s1 ? nullptr : asMat(src1, m1loc);
    மாறிலி Mat* pm2 = s2 ? nullptr : asMat(src2, m2loc);
    மாறிலி முழு adepth = s1 ? pm2->depth() : pm1->depth();   // the (first) array operand's depth
    // auto result depth (no explicit dtype, no fixed-type dst): add/sub/mul/div keep the first array
    // operand's depth (cv::'s dtype==-1 convention), but min/max/absdiff have NO dtype argument and
    // must promote mixed inputs to their common type (min(u32,f32) in u32 would drop the float). For
    // same-type inputs promoteArith(da,db)==da, so classic behaviour is preserved (e.g. absdiff of two
    // s16 stays s16, saturating - the wide unsigned |a-b| is computed then cast back down by emitBinary).
    முழு autoDepth = adepth;
    எனில் (op == ew::OP_MIN || op == ew::OP_MAX || op == ew::OP_ABSDIFF)
    {
        மாறிலி முழு d1 = s1 ? ew::EW_DEPTH_NONE : pm1->depth();
        மாறிலி முழு d2 = s2 ? ew::EW_DEPTH_NONE : pm2->depth();
        autoDepth = ew::promoteArith(d1, d2);
    }
    // result depth: explicit dtype wins; else a fixed-type dst dictates it; else the auto depth above.
    மாறிலி முழு rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
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
    எனில் (!haveMask && !s1 && !s2 &&
        (op == ew::OP_ADD || op == ew::OP_SUB || op == ew::OP_MIN || op == ew::OP_MAX ||
         op == ew::OP_ABSDIFF || op == ew::OP_MUL || op == ew::OP_ADDW ||
         op == ew::OP_AND || op == ew::OP_OR || op == ew::OP_XOR) &&
        rdepth == pm1->depth() && pm1->type() == pm2->type() && pm1->size == pm2->size &&
        pm1->isContinuous() && pm2->isContinuous() &&
        pm1->total()*cn1 <= (size_t)ARITHM_SMALL_DIRECT)
    {
        ew::TKernel k = ew::getElemwiseFunc(op, rdepth, rdepth, ew::EW_DEPTH_NONE, rdepth);
        எனில் (k.fptr)
        {
            dst.createSameSize(src1, pm1->type());   // whole-shape transfer, not piecemeal dims+sizes
            Mat dloc;
            Mat* pd = (dst.kind() == உள்ளீடு_Array_2::MAT) ? (Mat*)dst.getObj() : &(dloc = dst.getMat());
            எனில் (pd->isContinuous())     // a reused non-continuous dst view falls through to the engine
            {
                k.fptr(pm1->data, 0, 1, pm2->data, 0, 1, nullptr, 0, 0,
                       pd->data, 0, (முழு)(pm1->total()*cn1), 1, params.val, k.flags, k.userdata);
                திரும்பு;
            }
        }
    }

    // operand order = (src1, src2): an array becomes an INPUT, a scalar a flexible per-channel CONST
    // read straight from the caller's inline storage (getObj()), no Mat / convertTo.
    ew::TExpr p;
    auto addOperand = [&](உள்ளீடு_Array src, தருக்க isScalar, மாறிலி Mat* m, முழு otherCn) -> முழு {
        எனில் (!isScalar)
            திரும்பு p.add_உள்ளீடு(m->depth());
        மாறிலி uchar* sp; முழு sd;
        uchar scbuf[EW_SCALAR_BUF_SIZE];   // stack room for a UMAT scalar; addConst copies the values out
        மாறிலி முழு scn0 = scalarArgElems(src, sp, sd, scbuf), cn = otherCn;
        மாறிலி முழு scn = (cn == scn0 || (cn < 4 && scn0 == 4)) ? cn
                      : (scn0 == 1) ? 1 : 0;
        CV_Assert(scn > 0);
        திரும்பு p.addConst(ew::EW_DEPTH_NONE, sd, sp, scn);
    };
    மாறிலி முழு a0 = addOperand(src1, s1, pm1, cn2);
    மாறிலி முழு a1 = addOperand(src2, s2, pm2, cn1);

    // Write-mask: a single-channel 1-byte array (u8/s8/bool) the size of the output spatial shape,
    // added as another broadcast input. The arithmetic result lands in a temp; a final
    // select(mask, r, dst) -> dst overwrites only mask!=0 positions of the (pre-existing) output.
    முழு sMask = 0;
    மாறிலி Mat* pmask = nullptr;
    எனில் (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = asMat(mask, mloc);
        sMask = p.add_உள்ளீடு(pmask->depth());
    }
    மாறிலி முழு sOut = p.addOutput(rdepth);               // output BEFORE emit -> moveToOutput drops the
    மாறிலி முழு r = p.emit_இருமம்(op, a0, a1, rdepth, params);   // result temp (mask-free => none); scale
                                                        // rides params[0] (mul/div), {a,b,g} for addW
    எனில் (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    இல்லையெனில்
        p.moveToOutput(r, sOut);
    p.compile();

    மாறிலி Mat* inputs[3]; முழு ni = 0;                   // inputs in program (input-index) order
    எனில் (!s1) inputs[ni++] = pm1;
    எனில் (!s2) inputs[ni++] = pm2;
    எனில் (haveMask) inputs[ni++] = pmask;

    // Pre-create the output at the broadcast shape, ALWAYS via dst.create() - never hand exec a raw
    // dst Mat. Routing through the _OutputArray is what enforces its contract: a FIXED_SIZE/FIXED_TYPE
    // dst (in-place 'a += b' passes '(const Mat&)a') throws here if a broadcast would change its shape
    // or type (Mat(1x1) += Mat(1x4)), instead of silently reallocating - exec sees only a Mat and has
    // no view of the array's fixed flags. Cost over a direct exec is negligible: exec keeps its own
    // fast path for a matching-shape op, and dst.create() with an unchanged shape/type is a no-op.
    // With a mask, a freshly (re)allocated dst is zeroed so mask==0 reads 0 (matches arithm_op); a
    // reused dst keeps its prior content there - detect reuse by shape+type BEFORE create (correct for
    // a reused UMat dst too).
    MatShape oshape; முழு ocn;
    p.outputShape(inputs, oshape, ocn);
    மாறிலி முழு otype = CV_MAKETYPE(rdepth, ocn);
    மாறிலி தருக்க reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == உள்ளீடு_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    எனில் (haveMask && !reused)
        pdst->setZero();                          // setZero (not setTo, which caps at 4 channels)
    p.exec(inputs, pdst);
}

// Unary element-wise op through the engine (bitwise NOT now; unary math later). One array input, an
// optional write-mask; result depth = dtype (>=0), else a fixed-type dst's depth, else the input
// depth. Mirrors arithm_op's build/mask/exec tail with a single input and no scalar operand. OpenCL is
// run by the caller (bitwise_op_ocl), so this is the CPU engine only.
நிலையான வெறுமை unary_op(ew::உச்சி op, உள்ளீடு_Array src, OutputArray dst, உள்ளீடு_Array mask,
                     முழு dtype, மாறிலி Scalar& params)
{
    எனில் (src.empty())
    {
        dst.release();
        எனில் (dtype >= 0)
            dst.create(0, 0, dtype);
        திரும்பு;
    }

    மாறிலி தருக்க haveMask = !mask.empty();
    // in-place (dst aliases src): keep a header copy so a shape/type-changing dst.create() can't free
    // src's data mid-op (matches arithm_op). A fixed-size/type dst never reallocs, so this is only for
    // an explicit dtype change.
    மாறிலி வெறுமை* dstObj = (dst.kind() == உள்ளீடு_Array_2::MAT) ? dst.getObj() : nullptr;
    மாறிலி தருக்க aliased = dstObj && src.kind() == உள்ளீடு_Array_2::MAT && dstObj == src.getObj();
    Mat sloc, mloc;
    மாறிலி Mat* psrc = (src.kind() == உள்ளீடு_Array_2::MAT && !aliased) ? (மாறிலி Mat*)src.getObj()
                                                                   : &(sloc = src.getMat());
    மாறிலி முழு rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
                     : (dst.fixedType() ? dst.depth() : psrc->depth());

    ew::TExpr p;
    மாறிலி முழு a0 = p.add_உள்ளீடு(psrc->depth());
    முழு sMask = 0;
    மாறிலி Mat* pmask = nullptr;
    எனில் (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = (mask.kind() == உள்ளீடு_Array_2::MAT) ? (மாறிலி Mat*)mask.getObj() : &(mloc = mask.getMat());
        sMask = p.add_உள்ளீடு(pmask->depth());
    }
    மாறிலி முழு sOut = p.addOutput(rdepth);
    மாறிலி முழு r = p.emitUnary(op, a0, rdepth, params);
    எனில் (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    இல்லையெனில்
        p.moveToOutput(r, sOut);
    p.compile();

    மாறிலி Mat* inputs[2]; முழு ni = 0;
    inputs[ni++] = psrc;
    எனில் (haveMask) inputs[ni++] = pmask;

    MatShape oshape; முழு ocn;
    p.outputShape(inputs, oshape, ocn);
    மாறிலி முழு otype = CV_MAKETYPE(rdepth, ocn);
    மாறிலி தருக்க reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == உள்ளீடு_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    எனில் (haveMask && !reused)
        pdst->setZero();
    p.exec(inputs, pdst);
}

}

வெறுமை cv::add( உள்ளீடு_Array src1, உள்ளீடு_Array src2, OutputArray dst,
          உள்ளீடு_Array mask, முழு dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADD, src1, src2, dst, mask, dtype, OCL_OP_ADD, பொய்);
}

வெறுமை cv::subtract( உள்ளீடு_Array src1, உள்ளீடு_Array src2, OutputArray dst,
                   உள்ளீடு_Array mask, முழு dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_SUB, src1, src2, dst, mask, dtype, OCL_OP_SUB, பொய்);
}

வெறுமை cv::absdiff( உள்ளீடு_Array src1, உள்ளீடு_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ABSDIFF, src1, src2, dst, noArray(), -1, OCL_OP_ABSDIFF, பொய்);
}

வெறுமை cv::copyTo(உள்ளீடு_Array _src, OutputArray _dst, உள்ளீடு_Array _mask)
{
    CV_INSTRUMENT_REGION();

    _src.copyTo(_dst, _mask);
}

/****************************************************************************************\
*                                    multiply/divide                                     *
\****************************************************************************************/

நாமாகாஶ cv
{

வெறுமை multiply(உள்ளீடு_Array src1, உள்ளீடு_Array src2,
              OutputArray dst, இரட்டை scale, முழு dtype)
{
    CV_INSTRUMENT_REGION();

    மாறிலி முழு oclop = std::abs(scale - 1.0) < DBL_EPSILON ? OCL_OP_MUL : OCL_OP_MUL_SCALE;
    arithm_op(ew::OP_MUL, src1, src2, dst, noArray(), dtype, oclop, /*muldiv*/ உண்மை, Scalar(scale));
}

வெறுமை divide(உள்ளீடு_Array src1, உள்ளீடு_Array src2,
                OutputArray dst, இரட்டை scale, முழு dtype)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_DIV, src1, src2, dst, noArray(), dtype, OCL_OP_DIV_SCALE, /*muldiv*/ உண்மை, Scalar(scale));
}

வெறுமை divide(இரட்டை scale, உள்ளீடு_Array src2,
                OutputArray dst, முழு dtype)
{
    CV_INSTRUMENT_REGION();

    எனில் (src2.empty())
    {
        dst.release();
        திரும்பு;
    }

    // scale / src2 == scale * 1 / src2: feed a 0-dim `1` as the numerator so the CPU engine reuses the
    // normal divide (scale*a/b). Give the numerator src2's OWN depth: then OP_DIV(T, T) is same-type and
    // takes the fast per-type kernel (f32 work for <=16-bit / f16 / bf16, not the s32->f64 path a CV_32S
    // numerator would force). An integer src2 keeps its div-by-zero->0 guard (both operands integer); a
    // float src2 divides as float (1/0->inf), both matching cv::divide. dtype<0 follows src2's depth (a
    // fixed-type dst still wins). The UMat path uses the dedicated reciprocal kernel (OCL_OP_RECIP_SCALE).
    இரட்டை one = 1;
    Mat numerator(MatShape::scalar(), src2.depth(), &one);
    scalarToRawData(Scalar(1), &one, src2.depth(), 1);       // write `1` in src2's depth (no allocation)
    மாறிலி முழு rtype = (dtype < 0 && !dst.fixedType()) ? src2.depth() : dtype;
    arithm_op(ew::OP_DIV, numerator, src2, dst, noArray(), rtype, OCL_OP_RECIP_SCALE, /*muldiv*/ உண்மை, Scalar(scale));
}

UMat UMat::mul(உள்ளீடு_Array m, இரட்டை scale) மாறிலி
{
    UMat dst;
    multiply(*இது, m, dst, scale);
    திரும்பு dst;
}

/****************************************************************************************\
*                                      addWeighted                                       *
\****************************************************************************************/

}

வெறுமை cv::addWeighted( உள்ளீடு_Array src1, இரட்டை alpha, உள்ளீடு_Array src2,
                      இரட்டை beta, இரட்டை gamma, OutputArray dst, முழு dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADDW, src1, src2, dst, noArray(), dtype, OCL_OP_ADDW, /*muldiv*/ உண்மை,
              Scalar(alpha, beta, gamma));
}


/****************************************************************************************\
*                                          compare                                       *
\****************************************************************************************/

நாமாகாஶ cv
{


நிலையான இரட்டை getMinVal(முழு depth)
{
    நிலையான மாறிலி இரட்டை tab[CV_DEPTH_MAX] =
    {
        0, -128, 0, -32768, INT_MIN, -FLT_MAX, -DBL_MAX,
        -65504, -FLT_MAX, 0, 0, (இரட்டை)INT64_MIN, 0
    };
    திரும்பு tab[depth];
}

நிலையான இரட்டை getMaxVal(முழு depth)
{
    நிலையான மாறிலி இரட்டை tab[CV_DEPTH_MAX] = {
        255, 127, 65535, 32767, INT_MAX, FLT_MAX, DBL_MAX,
        65504, FLT_MAX, 255, (இரட்டை)UINT64_MAX, (இரட்டை)INT64_MAX, (இரட்டை)UINT32_MAX, 0
    };
    திரும்பு tab[depth];
}

#ifdef HAVE_OPENCL

நிலையான தருக்க ocl_compare(உள்ளீடு_Array _src1, உள்ளீடு_Array _src2, OutputArray _dst, முழு op, தருக்க haveScalar)
{
    மாறிலி ocl::Device& dev = ocl::Device::getDefault();
    தருக்க doubleSupport = dev.doubleFPConfig() > 0;
    முழு type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1),
            type2 = _src2.type(), depth2 = CV_MAT_DEPTH(type2);

    எனில் (!doubleSupport && depth1 == CV_64F)
        திரும்பு பொய்;

    // the integer scalar path below works in int; let the CPU handle the wider 5.x integer depths
    எனில் (haveScalar && (depth1 == CV_32U || depth1 == CV_64U || depth1 == CV_64S))
        திரும்பு பொய்;

    எனில் (!haveScalar && (!_src1.sameSize(_src2) || type1 != type2))
            திரும்பு பொய்;

    // This OpenCL kernel broadcasts a scalar's FIRST channel to all channels. The CPU path compares
    // per-channel (scalar[c] for channel c), so a multichannel array against a scalar with DISTINCT
    // channel values would disagree - bail to the CPU engine in that case (rare; not worth a kernel fix).
    எனில் (haveScalar && cn > 1)
    {
        Mat sc = _src2.getMat();
        முழு scn = (முழு)sc.total();
        எனில் (scn > 1)
        {
            இரட்டை v[4] = { 0, 0, 0, 0 };
            getConvertFunc(sc.depth(), CV_64F)(sc.ptr(), 1, 0, 1, (uchar*)v, 1, Size(std::min(scn, 4), 1), 0);
            க்கு (முழு i = 1, n = std::min(cn, scn); i < n; i++)
                எனில் (v[i] != v[0])
                    திரும்பு பொய்;
        }
    }

    முழு kercn = haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst), rowsPerWI = dev.isIntel() ? 4 : 1;
    // Workaround for bug with "?:" operator in AMD OpenCL compiler
    எனில் (depth1 >= CV_16U)
        kercn = 1;

    முழு scalarcn = kercn == 3 ? 4 : kercn;
    மாறிலி எழுத்து * மாறிலி operationMap[] = { "==", ">", ">=", "<", "<=", "!=" };
    எழுத்து cvt[50];

    String opts = format("-D %s -D srcT1=%s -D dstT=%s -D DEPTH_dst=%d -D workT=srcT1 -D cn=%d"
                         " -D convertToDT=%s -D OP_CMP -D CMP_OPERATOR=%s -D srcT1_C1=%s"
                         " -D srcT2_C1=%s -D dstT_C1=%s -D workST=%s -D rowsPerWI=%d%s",
                         haveScalar ? "UNARY_OP" : "BINARY_OP",
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, kercn)),
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U, kercn,
                         ocl::convertTypeStr(depth1, CV_8U, kercn, cvt, அளவு(cvt)),
                         operationMap[op], ocl::typeToStr(depth1),
                         ocl::typeToStr(depth1), ocl::typeToStr(CV_8U),
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, scalarcn)), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    எனில் (k.empty())
        திரும்பு பொய்;

    UMat src1 = _src1.getUMat();
    Size size = src1.size();
    _dst.create(size, CV_8UC(cn));
    UMat dst = _dst.getUMat();

    எனில் (haveScalar)
    {
        size_t esz = CV_ELEM_SIZE1(type1) * scalarcn;
        இரட்டை buf[4] = { 0, 0, 0, 0 };
        Mat src2 = _src2.getMat();

        எனில்( depth1 > CV_32S )
            convertAndUnrollScalar( src2, depth1, (uchar *)buf, kercn );
        இல்லையெனில்
        {
            இரட்டை fval = 0;
            getConvertFunc(depth2, CV_64F)(src2.ptr(), 1, 0, 1, (uchar *)&fval, 1, Size(1, 1), 0);
            எனில்( fval < getMinVal(depth1) )
                திரும்பு dst.setTo(Scalar::all(op == CMP_GT || op == CMP_GE || op == CMP_NE ? 255 : 0)), உண்மை;

            எனில்( fval > getMaxVal(depth1) )
                திரும்பு dst.setTo(Scalar::all(op == CMP_LT || op == CMP_LE || op == CMP_NE ? 255 : 0)), உண்மை;

            முழு ival = cvRound(fval);
            எனில்( fval != ival )
            {
                எனில்( op == CMP_LT || op == CMP_GE )
                    ival = cvCeil(fval);
                இல்லையெனில் எனில்( op == CMP_LE || op == CMP_GT )
                    ival = cvFloor(fval);
                இல்லையெனில்
                    திரும்பு dst.setTo(Scalar::all(op == CMP_NE ? 255 : 0)), உண்மை;
            }
            convertAndUnrollScalar(Mat(1, 1, CV_32S, &ival), depth1, (uchar *)buf, kercn);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        k.args(ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn),
               ocl::KernelArg::WriteOnly(dst, cn, kercn), scalararg);
    }
    இல்லையெனில்
    {
        UMat src2 = _src2.getUMat();

        k.args(ocl::KernelArg::ReadOnlyNoSize(src1),
               ocl::KernelArg::ReadOnlyNoSize(src2),
               ocl::KernelArg::WriteOnly(dst, cn, kercn));
    }

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    திரும்பு k.run(2, globalsize, வெற்று, பொய்);
}

#endif

}

வெறுமை cv::compare(உள்ளீடு_Array _src1, உள்ளீடு_Array _src2, OutputArray _dst, முழு op)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( op == CMP_LT || op == CMP_LE || op == CMP_EQ ||
               op == CMP_NE || op == CMP_GE || op == CMP_GT );

    CV_Assert(_src1.empty() == _src2.empty());
    எனில் (_src1.empty() && _src2.empty())
    {
        _dst.release();
        திரும்பு;
    }

    தருக்க haveScalar = பொய்;

    எனில் ((_src1.isMatx() + _src2.isMatx()) == 1
            || !_src1.sameSize(_src2)
            || _src1.type() != _src2.type())
    {
        தருக்க is_src1_scalar = checkScalar(_src1, _src2.type(), _src1.kind(), _src2.kind());
        தருக்க is_src2_scalar = checkScalar(_src2, _src1.type(), _src2.kind(), _src1.kind());

        // exactly one scalar operand: keep it as src2 (swapping flips the ordering ops) so the OpenCL
        // path sees the array-op-scalar form. Otherwise - two arrays of different size (broadcast),
        // mixed types, or two scalars - the CPU engine broadcasts / promotes them, so there is NO error
        // here: fall through to arithm_op (which handles scalar detection, broadcast and mixed types).
        எனில் (is_src1_scalar && !is_src2_scalar)
        {
            op = op == CMP_LT ? CMP_GT : op == CMP_LE ? CMP_GE :
                op == CMP_GE ? CMP_LE : op == CMP_GT ? CMP_LT : op;
            // src1 is a scalar; swap it with src2
            compare(_src2, _src1, _dst, op);
            திரும்பு;
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
    ew::உச்சி cmpOp = op == CMP_EQ ? ew::OP_CMP_EQ : op == CMP_NE ? ew::OP_CMP_NE :
                    op == CMP_LT ? ew::OP_CMP_LT : op == CMP_LE ? ew::OP_CMP_LE :
                    op == CMP_GT ? ew::OP_CMP_GT : ew::OP_CMP_GE;
    arithm_op(cmpOp, _src1, _src2, _dst, noArray(), CV_8U, /*oclop=*/-1, /*muldiv=*/பொய்);
}

/****************************************************************************************\
*                                        inRange                                         *
\****************************************************************************************/

நாமாகாஶ cv
{

template <typename T>
கட்டமைப்பு InRange_SIMD
{
    முழு operator () (மாறிலி T * src1, மாறிலி T * src2, மாறிலி T * src3, uchar * dst, முழு len) மாறிலி
    {
        க்கு (முழு x = 0; x < len; x++)
            dst[x] = (src2[x] <= src1[x] && src1[x] <= src3[x]) ? 255 : 0;
        திரும்பு len;
    }
};

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)

template <>
கட்டமைப்பு InRange_SIMD<uchar>
{
    முழு operator () (மாறிலி uchar * src1, மாறிலி uchar * src2, மாறிலி uchar * src3,
        uchar * dst, முழு len) மாறிலி
    {
        முழு x = 0;
        மாறிலி முழு width = VTraits<v_uint8>::vlanes();

        க்கு (; x <= len - width; x += width)
        {
            v_uint8 values = vx_load(src1 + x);
            v_uint8 கீழ் = vx_load(src2 + x);
            v_uint8 உயர்வு = vx_load(src3 + x);

            v_store(dst + x, v_and(v_ge(values, கீழ்), v_ge(உயர்வு, values)));
        }
        vx_cleanup();
        திரும்பு x;
    }
};

template <>
கட்டமைப்பு InRange_SIMD<schar>
{
    முழு operator () (மாறிலி schar * src1, மாறிலி schar * src2, மாறிலி schar * src3,
        uchar * dst, முழு len) மாறிலி
    {
        முழு x = 0;
        மாறிலி முழு width = VTraits<v_int8>::vlanes();

        க்கு (; x <= len - width; x += width)
        {
            v_int8 values = vx_load(src1 + x);
            v_int8 கீழ் = vx_load(src2 + x);
            v_int8 உயர்வு = vx_load(src3 + x);

            v_store((schar*)(dst + x), v_and(v_ge(values, கீழ்), v_ge(உயர்வு, values)));
        }
        vx_cleanup();
        திரும்பு x;
    }
};

template <>
கட்டமைப்பு InRange_SIMD<ushort>
{
    முழு operator () (மாறிலி ushort * src1, மாறிலி ushort * src2, மாறிலி ushort * src3,
        uchar * dst, முழு len) மாறிலி
    {
        முழு x = 0;
        மாறிலி முழு width = VTraits<v_uint16>::vlanes() * 2;

        க்கு (; x <= len - width; x += width)
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
        திரும்பு x;
    }
};

template <>
கட்டமைப்பு InRange_SIMD<short>
{
    முழு operator () (மாறிலி short * src1, மாறிலி short * src2, மாறிலி short * src3,
        uchar * dst, முழு len) மாறிலி
    {
        முழு x = 0;
        மாறிலி முழு width = (முழு)VTraits<v_int16>::vlanes() * 2;

        க்கு (; x <= len - width; x += width)
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
        திரும்பு x;
    }
};

template <>
கட்டமைப்பு InRange_SIMD<முழு>
{
    முழு operator () (மாறிலி முழு * src1, மாறிலி முழு * src2, மாறிலி முழு * src3,
        uchar * dst, முழு len) மாறிலி
    {
        முழு x = 0;
        மாறிலி முழு width = (முழு)VTraits<v_int32>::vlanes() * 2;

        க்கு (; x <= len - width; x += width)
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
        திரும்பு x;
    }
};

template <>
கட்டமைப்பு InRange_SIMD<தசம>
{
    முழு operator () (மாறிலி தசம * src1, மாறிலி தசம * src2, மாறிலி தசம * src3,
        uchar * dst, முழு len) மாறிலி
    {
        முழு x = 0;
        மாறிலி முழு width = (முழு)VTraits<v_float32>::vlanes() * 2;

        க்கு (; x <= len - width; x += width)
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
        திரும்பு x;
    }
};

template <>
கட்டமைப்பு InRange_SIMD<hfloat>
{
    முழு operator () (மாறிலி hfloat * src1, மாறிலி hfloat * src2, மாறிலி hfloat * src3,
        uchar * dst, முழு len) மாறிலி
    {
        முழு x = 0;
        மாறிலி முழு width = (முழு)VTraits<v_float32>::vlanes()*2;

        க்கு (; x <= len - width; x += width)
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
        திரும்பு x;
    }
};

template <>
கட்டமைப்பு InRange_SIMD<bfloat>
{
    முழு operator () (மாறிலி bfloat * src1, மாறிலி bfloat * src2, மாறிலி bfloat * src3,
        uchar * dst, முழு len) மாறிலி
    {
        முழு x = 0;
        மாறிலி முழு width = (முழு)VTraits<v_float32>::vlanes()*2;

        க்கு (; x <= len - width; x += width)
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
        திரும்பு x;
    }
};

template <>
கட்டமைப்பு InRange_SIMD<unsigned>
{
    முழு operator () (மாறிலி unsigned * src1, மாறிலி unsigned * src2, மாறிலி unsigned * src3,
        uchar * dst, முழு len) மாறிலி
    {
        முழு x = 0;
        மாறிலி முழு width = (முழு)VTraits<v_uint32>::vlanes() * 2;

        க்கு (; x <= len - width; x += width)
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
        திரும்பு x;
    }
};

#எனில் CV_SIMD_64F

template <>
கட்டமைப்பு InRange_SIMD<இரட்டை>
{
    முழு operator () (மாறிலி இரட்டை * src1, மாறிலி இரட்டை * src2, மாறிலி இரட்டை * src3,
        uchar * dst, முழு len) மாறிலி
    {
        முழு x = 0;
        மாறிலி முழு step = VTraits<v_float64>::vlanes();
        மாறிலி முழு width = step * 4;

        க்கு (; x <= len - width; x += width)
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
        திரும்பு x;
    }
};

#endif

#endif

template <typename T>
நிலையான வெறுமை inRange_(மாறிலி வெறுமை* _src1, size_t step1, மாறிலி வெறுமை* _src2, size_t step2,
         மாறிலி வெறுமை* _src3, size_t step3, uchar* dst, size_t step,
         Size size)
{
    மாறிலி T* src1 = (மாறிலி T*)_src1;
    மாறிலி T* src2 = (மாறிலி T*)_src2;
    மாறிலி T* src3 = (மாறிலி T*)_src3;
    step1 /= அளவு(src1[0]);
    step2 /= அளவு(src2[0]);
    step3 /= அளவு(src3[0]);

    InRange_SIMD<T> vop;

    க்கு( ; size.height--; src1 += step1, src2 += step2, src3 += step3, dst += step )
    {
        முழு x = vop(src1, src2, src3, dst, size.width);
        #எனில் CV_ENABLE_UNROLLED
        க்கு( ; x <= size.width - 4; x += 4 )
        {
            முழு t0, t1;
            t0 = src2[x] <= src1[x] && src1[x] <= src3[x];
            t1 = src2[x+1] <= src1[x+1] && src1[x+1] <= src3[x+1];
            dst[x] = (uchar)-t0; dst[x+1] = (uchar)-t1;
            t0 = src2[x+2] <= src1[x+2] && src1[x+2] <= src3[x+2];
            t1 = src2[x+3] <= src1[x+3] && src1[x+3] <= src3[x+3];
            dst[x+2] = (uchar)-t0; dst[x+3] = (uchar)-t1;
        }
        #endif
        க்கு( ; x < size.width; x++ )
            dst[x] = (uchar)-(src2[x] <= src1[x] && src1[x] <= src3[x]);
    }
}

நிலையான வெறுமை inRangeReduce(மாறிலி uchar* src, uchar* dst, size_t len, முழு cn)
{
    முழு k = cn % 4 ? cn % 4 : 4;
    size_t i, j;
    எனில்( k == 1 )
        க்கு( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j];
    இல்லையெனில் எனில்( k == 2 )
        க்கு( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1];
    இல்லையெனில் எனில்( k == 3 )
        க்கு( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2];
    இல்லையெனில்
        க்கு( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2] & src[j+3];

    க்கு( ; k < cn; k += 4 )
    {
        க்கு( i = 0, j = k; i < len; i++, j += cn )
            dst[i] &= src[j] & src[j+1] & src[j+2] & src[j+3];
    }
}

வகைவரை வெறுமை (*InRangeFunc)( மாறிலி வெறுமை* src1, size_t step1, மாறிலி வெறுமை* src2, size_t step2,
                             மாறிலி வெறுமை* src3, size_t step3, uchar* dst, size_t step, Size sz );

நிலையான InRangeFunc getInRangeFunc(முழு depth)
{
    நிலையான InRangeFunc inRangeTab[CV_DEPTH_MAX] =
    {
        inRange_<uchar>,
        inRange_<schar>,
        inRange_<ushort>,
        inRange_<short>,
        inRange_<முழு>,
        inRange_<தசம>,
        inRange_<இரட்டை>,
        inRange_<hfloat>,
        inRange_<bfloat>,
        0,
        inRange_<uint64_t>,
        inRange_<int64_t>,
        inRange_<unsigned>,
        0,
    };

    திரும்பு inRangeTab[depth];
}

#ifdef HAVE_OPENCL

நிலையான தருக்க ocl_inRange( உள்ளீடு_Array _src, உள்ளீடு_Array _lowerb,
                         உள்ளீடு_Array _upperb, OutputArray _dst )
{
    மாறிலி ocl::Device & d = ocl::Device::getDefault();
    உள்ளீடு_Array_2::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Size ssize = _src.size(), lsize = _lowerb.size(), usize = _upperb.size();
    முழு stype = _src.type(), ltype = _lowerb.type(), utype = _upperb.type();
    முழு sdepth = CV_MAT_DEPTH(stype), ldepth = CV_MAT_DEPTH(ltype), udepth = CV_MAT_DEPTH(utype);
    முழு cn = CV_MAT_CN(stype), rowsPerWI = d.isIntel() ? 4 : 1;
    தருக்க lbScalar = பொய், ubScalar = பொய்;

    எனில்( (lkind == உள்ளீடு_Array_2::MATX && skind != உள்ளீடு_Array_2::MATX) ||
        ssize != lsize || stype != ltype )
    {
        எனில்( !checkScalar(_lowerb, stype, lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = உண்மை;
    }

    எனில்( (ukind == உள்ளீடு_Array_2::MATX && skind != உள்ளீடு_Array_2::MATX) ||
        ssize != usize || stype != utype )
    {
        எனில்( !checkScalar(_upperb, stype, ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = உண்மை;
    }

    எனில் (lbScalar != ubScalar)
        திரும்பு பொய்;

    தருக்க doubleSupport = d.doubleFPConfig() > 0,
            haveScalar = lbScalar && ubScalar;

    எனில் ( (!doubleSupport && sdepth == CV_64F) ||
         (!haveScalar && (sdepth != ldepth || sdepth != udepth)) )
        திரும்பு பொய்;

    முழு kercn = haveScalar ? cn : std::max(std::min(ocl::predictOptimalVectorWidth(_src, _lowerb, _upperb, _dst), 4), cn);
    எனில் (kercn % cn != 0)
        kercn = cn;
    முழு colsPerWI = kercn / cn;
    String opts = format("%s-D CN=%d -D SRC_T=%s -D SRC_T1=%s -D DST_T=%s -D KERCN=%d -D DEPTH=%d%s -D COLS_PER_WI=%d",
                           haveScalar ? "-D HAVE_SCALAR " : "", cn, ocl::typeToStr(CV_MAKE_TYPE(sdepth, kercn)),
                           ocl::typeToStr(sdepth), ocl::typeToStr(CV_8UC(colsPerWI)), kercn, sdepth,
                           doubleSupport ? " -D DOUBLE_SUPPORT" : "", colsPerWI);

    ocl::Kernel ker("inrange", ocl::core::inrange_oclsrc, opts);
    எனில் (ker.empty())
        திரும்பு பொய்;

    _dst.create(ssize, CV_8UC1);
    UMat src = _src.getUMat(), dst = _dst.getUMat(), lscalaru, uscalaru;
    Mat lscalar, uscalar;

    எனில் (lbScalar && ubScalar)
    {
        lscalar = _lowerb.getMat();
        uscalar = _upperb.getMat();

        size_t esz = src.elemSize();
        size_t blocksize = 36;

        AutoBuffer<uchar> _buf(blocksize*(((முழு)lbScalar + (முழு)ubScalar)*esz + cn) + 2*cn*அளவு(முழு) + 128);
        uchar *buf = alignPtr(_buf.data() + blocksize*cn, 16);

        எனில்( ldepth != sdepth && sdepth < CV_32S )
        {
            முழு* ilbuf = (முழு*)alignPtr(buf + blocksize*esz, 16);
            முழு* iubuf = ilbuf + cn;

            இருமம்_Func sccvtfunc = getConvertFunc(ldepth, CV_32S);
            sccvtfunc(lscalar.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, Size(cn, 1), 0);
            sccvtfunc(uscalar.ptr(), 1, 0, 1, (uchar*)iubuf, 1, Size(cn, 1), 0);
            முழு minval = cvRound(getMinVal(sdepth)), maxval = cvRound(getMaxVal(sdepth));

            க்கு( முழு k = 0; k < cn; k++ )
            {
                எனில்( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lscalar = Mat(cn, 1, CV_32S, ilbuf);
            uscalar = Mat(cn, 1, CV_32S, iubuf);
        }

        lscalar.convertTo(lscalar, stype);
        uscalar.convertTo(uscalar, stype);
    }
    இல்லையெனில்
    {
        lscalaru = _lowerb.getUMat();
        uscalaru = _upperb.getUMat();
    }

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, 1, colsPerWI);

    எனில் (haveScalar)
    {
        lscalar.copyTo(lscalaru);
        uscalar.copyTo(uscalaru);

        ker.args(srcarg, dstarg, ocl::KernelArg::PtrReadOnly(lscalaru),
               ocl::KernelArg::PtrReadOnly(uscalaru), rowsPerWI);
    }
    இல்லையெனில்
        ker.args(srcarg, dstarg, ocl::KernelArg::ReadOnlyNoSize(lscalaru),
               ocl::KernelArg::ReadOnlyNoSize(uscalaru), rowsPerWI);

    size_t globalsize[2] = { (size_t)ssize.width / colsPerWI, ((size_t)ssize.height + rowsPerWI - 1) / rowsPerWI };
    திரும்பு ker.run(2, globalsize, வெற்று, பொய்);
}

#endif

}

வெறுமை cv::inRange(உள்ளீடு_Array _src, உள்ளீடு_Array _lowerb,
                 உள்ளீடு_Array _upperb, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(! _src.empty());
    எனில் (!getInRangeFunc(_src.depth()))
        CV_Error( cv::Error::StsNotImplemented,
                 "inRange: CV_Bool and CV_8F inputs are not supported, convert the input to another type first");

    CV_OCL_RUN(_src.dims() <= 2 && _lowerb.dims() <= 2 &&
               _upperb.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat()),
               ocl_inRange(_src, _lowerb, _upperb, _dst))

    உள்ளீடு_Array_2::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Mat src = _src.getMat(), lb = _lowerb.getMat(), ub = _upperb.getMat();

    தருக்க lbScalar = பொய், ubScalar = பொய்;

    எனில்( (lkind == உள்ளீடு_Array_2::MATX && skind != உள்ளீடு_Array_2::MATX) ||
        src.size != lb.size || src.type() != lb.type() )
    {
        எனில்( !checkScalar(lb, src.type(), lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = உண்மை;
    }

    எனில்( (ukind == உள்ளீடு_Array_2::MATX && skind != உள்ளீடு_Array_2::MATX) ||
        src.size != ub.size || src.type() != ub.type() )
    {
        எனில்( !checkScalar(ub, src.type(), ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = உண்மை;
    }

    CV_Assert(lbScalar == ubScalar);

    முழு cn = src.channels(), depth = src.depth();

    size_t esz = src.elemSize();
    size_t blocksize0 = (size_t)(BLOCK_SIZE + esz-1)/esz;

    _dst.createSameSize(_src, CV_8UC1);
    Mat dst = _dst.getMat();
    InRangeFunc func = getInRangeFunc(depth);

    மாறிலி Mat* arrays_sc[] = { &src, &dst, 0 };
    மாறிலி Mat* arrays_nosc[] = { &src, &dst, &lb, &ub, 0 };
    uchar* ptrs[4] = {};

    NAryMatIterator it(lbScalar && ubScalar ? arrays_sc : arrays_nosc, ptrs);
    size_t total = it.size, blocksize = std::min(total, blocksize0);

    AutoBuffer<uchar> _buf(blocksize*(((முழு)lbScalar + (முழு)ubScalar)*esz + cn) + 2*cn*அளவு(முழு) + 128);
    uchar *buf = _buf.data(), *mbuf = buf, *lbuf = 0, *ubuf = 0;
    buf = alignPtr(buf + blocksize*cn, 16);

    எனில்( lbScalar && ubScalar )
    {
        lbuf = buf;
        ubuf = buf = alignPtr(buf + blocksize*esz, 16);

        CV_Assert( lb.type() == ub.type() );
        முழு scdepth = lb.depth();

        எனில்( scdepth != depth && depth < CV_32S )
        {
            முழு* ilbuf = (முழு*)alignPtr(buf + blocksize*esz, 16);
            முழு* iubuf = ilbuf + cn;

            இருமம்_Func sccvtfunc = getConvertFunc(scdepth, CV_32S);
            sccvtfunc(lb.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, Size(cn, 1), 0);
            sccvtfunc(ub.ptr(), 1, 0, 1, (uchar*)iubuf, 1, Size(cn, 1), 0);
            முழு minval = cvRound(getMinVal(depth)), maxval = cvRound(getMaxVal(depth));

            க்கு( முழு k = 0; k < cn; k++ )
            {
                எனில்( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lb = Mat(cn, 1, CV_32S, ilbuf);
            ub = Mat(cn, 1, CV_32S, iubuf);
        }

        convertAndUnrollScalar( lb, src.type(), lbuf, blocksize );
        convertAndUnrollScalar( ub, src.type(), ubuf, blocksize );

        எனில் (depth == CV_8U && src.dims <= 2) {
            uint8_t lb_scalar = lbuf[0];
            uint8_t ub_scalar = ubuf[0];
            CALL_HAL(inRange_u8, cv_hal_inRange8u, src.data, src.step, dst.data, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        } இல்லையெனில் எனில் (depth == CV_32F && src.dims <= 2) {
            இரட்டை lb_scalar = lb.ptr<இரட்டை>(0)[0];
            இரட்டை ub_scalar = ub.ptr<இரட்டை>(0)[0];
            CALL_HAL(inRange_f32, cv_hal_inRange32f, src.data, src.step, dst.data, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        }
    }

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        க்கு( size_t j = 0; j < total; j += blocksize )
        {
            முழு bsz = (முழு)MIN(total - j, blocksize);
            size_t delta = bsz*esz;
            uchar *lptr = lbuf, *uptr = ubuf;
            எனில்( !lbScalar )
            {
                lptr = ptrs[2];
                ptrs[2] += delta;
            }
            எனில்( !ubScalar )
            {
                முழு idx = !lbScalar ? 3 : 2;
                uptr = ptrs[idx];
                ptrs[idx] += delta;
            }
            func( ptrs[0], 0, lptr, 0, uptr, 0, cn == 1 ? ptrs[1] : mbuf, 0, Size(bsz*cn, 1));
            எனில்( cn > 1 )
                inRangeReduce(mbuf, ptrs[1], bsz, cn);
            ptrs[0] += delta;
            ptrs[1] += bsz;
        }
    }
}
/* End of file. */
