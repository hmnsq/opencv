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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "arithm_expr.hpp"   // the new element-wise engine (cv::ew)
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"

ନାମାକାଶ cv
{

/****************************************************************************************\
*                                   logical operations                                   *
\****************************************************************************************/

ଗଣନା { OCL_OP_ADD=0, OCL_OP_SUB=1, OCL_OP_RSUB=2, OCL_OP_ABSDIFF=3, OCL_OP_MUL=4,
       OCL_OP_MUL_SCALE=5, OCL_OP_DIV_SCALE=6, OCL_OP_RECIP_SCALE=7, OCL_OP_ADDW=8,
       OCL_OP_AND=9, OCL_OP_OR=10, OCL_OP_XOR=11, OCL_OP_NOT=12, OCL_OP_MIN=13, OCL_OP_MAX=14,
       OCL_OP_RDIV_SCALE=15 };

// The unified entry point for every element-wise binary op (empty + UMat/OpenCL + CPU engine). Defined
// lower down (near cv::add); forward-declared here so cv::min/cv::max (above it) can use it too.
ସ୍ଥିର ଶୂନ୍ୟ arithm_op(ew::ଶୀର୍ଷ op, ନିବେଶ_Array src1, ନିବେଶ_Array src2, OutputArray dst,
                      ନିବେଶ_Array mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା oclop, ତାର୍କିକ muldiv, ଅଚଳ Scalar& params = Scalar(1));

// The unary counterpart (bitwise NOT, unary math later). Forward-declared for the same reason.
ସ୍ଥିର ଶୂନ୍ୟ unary_op(ew::ଶୀର୍ଷ op, ନିବେଶ_Array src, OutputArray dst, ନିବେଶ_Array mask,
                     ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype, ଅଚଳ Scalar& params = Scalar());

#ifdef HAVE_OPENCL

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* oclop2str[] = { "OP_ADD", "OP_SUB", "OP_RSUB", "OP_ABSDIFF",
    "OP_MUL", "OP_MUL_SCALE", "OP_DIV_SCALE", "OP_RECIP_SCALE",
    "OP_ADDW", "OP_AND", "OP_OR", "OP_XOR", "OP_NOT", "OP_MIN", "OP_MAX", "OP_RDIV_SCALE", 0 };

// depths with no OpenCL type name (ocl::typeToStr() asserts on them)
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ noOclType(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ଫେରନ୍ତୁ depth == CV_16BF || depth == CV_Bool || depth == CV_8F_E4M3FN || depth == CV_8F_E4M3FNUZ;
}

ସ୍ଥିର ତାର୍କିକ ocl_ଦ୍ୱିମିକ_op(ନିବେଶ_Array _src1, ନିବେଶ_Array _src2, OutputArray _dst,
                          ନିବେଶ_Array _mask, ତାର୍କିକ bitwise, ପୂର୍ଣ୍ଣସଂଖ୍ୟା oclop, ତାର୍କିକ haveScalar )
{
    ତାର୍କିକ haveMask = !_mask.empty();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା srctype = _src1.type();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcdepth = CV_MAT_DEPTH(srctype);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = CV_MAT_CN(srctype);

    ଅଚଳ ocl::Device d = ocl::Device::getDefault();
    ତାର୍କିକ doubleSupport = d.doubleFPConfig() > 0;
    ଯଦି( oclop < 0 || ((haveMask || haveScalar) && cn > 4) ||
            (!doubleSupport && srcdepth == CV_64F && !bitwise) ||
            (noOclType(srcdepth) && !bitwise))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅକ୍ଷର opts[1024];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା scalarcn = kercn == 3 ? 4 : kercn;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rowsPerWI = d.isIntel() ? 4 : 1;

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dstDepth = srcdepth;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dstType = CV_MAKETYPE(dstDepth, kercn);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dstType1 = CV_MAKETYPE(dstDepth, 1);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା scalarType = CV_MAKETYPE(srcdepth, scalarcn);

    snprintf(opts, ଆକାର(opts), "-D %s%s -D %s%s -D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workST=%s -D cn=%d -D rowsPerWI=%d",
            haveMask ? "MASK_" : "", haveScalar ? "UNARY_OP" : "BINARY_OP", oclop2str[oclop],
            doubleSupport ? " -D DOUBLE_SUPPORT" : "",
            bitwise ? ocl::memopTypeToStr(dstType) : ocl::typeToStr(dstType),
            dstDepth,
            bitwise ? ocl::memopTypeToStr(dstType1) : ocl::typeToStr(dstType1),
            bitwise ? ocl::memopTypeToStr(scalarType) : ocl::typeToStr(scalarType),
            kercn, rowsPerWI);

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask, 1);

    ଯଦି( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(srctype)*scalarcn;
        ଦ୍ୱିଗୁଣ buf[4] = {0,0,0,0};

        ଯଦି( oclop != OCL_OP_NOT )
        {
            Mat src2sc = _src2.getMat();
            convertAndUnrollScalar(src2sc, srctype, (uchar*)buf, 1);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        ଯଦି( !haveMask )
            k.args(src1arg, dstarg, scalararg);
        ଅନ୍ୟଥା
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    ଅନ୍ୟଥା
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::ReadOnlyNoSize(src2, cn, kercn);

        ଯଦି( !haveMask )
            k.args(src1arg, src2arg, dstarg);
        ଅନ୍ୟଥା
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, 0, ମିଥ୍ୟା);
}

#endif

// OpenCL path for the bitwise ops (and/or/xor/not), lifted out of binary_op: choose the (possibly
// scalar) operand order, size the destination, and dispatch the shared bitwise OpenCL kernel. Returns
// true if OpenCL handled the op, false to fall through to the CPU engine. NOT arrives unary
// (src2 == src1, forced-scalar). The caller gates this on a UMat operand + dims <= 2 (via CV_OCL_RUN).
#ifdef HAVE_OPENCL
ସ୍ଥିର ତାର୍କିକ bitwise_op_ocl(ନିବେଶ_Array _src1, ନିବେଶ_Array _src2, OutputArray _dst,
                           ନିବେଶ_Array _mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା oclop)
{
    ଅଚଳ ନିବେଶ_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    ନିବେଶ_Array_2::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type1 = psrc1->type(), type2 = psrc2->type();
    ତାର୍କିକ haveScalar = ମିଥ୍ୟା;

    ଯଦି( oclop == OCL_OP_NOT )
        haveScalar = ସତ୍ୟ;
    ଅନ୍ୟଥା ଯଦି( (kind1 == ନିବେଶ_Array_2::MATX) + (kind2 == ନିବେଶ_Array_2::MATX) == 1 ||
             !psrc1->sameSize(*psrc2) || type1 != type2 )
    {
        ଯଦି( checkScalar(*psrc1, type2, kind1, kind2) )
        { std::swap(psrc1, psrc2); std::swap(type1, type2); }        // src1 is the scalar; swap it out
        ଅନ୍ୟଥା ଯଦି( !checkScalar(*psrc2, type1, kind2, kind1) )
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        haveScalar = ସତ୍ୟ;
    }
    _dst.createSameSize(*psrc1, type1);
    ଫେରନ୍ତୁ ocl_ଦ୍ୱିମିକ_op(*psrc1, *psrc2, _dst, _mask, ସତ୍ୟ, oclop, haveScalar);
}
#endif  // HAVE_OPENCL

}

// bitwise and/or/xor: OpenCL runs the shared bitwise kernel (via bitwise_op_ocl); the CPU path is the
// element-wise engine (OP_AND/OR/XOR, T x T -> T dispatched by element size). oclop = -1 tells
// arithm_op the OpenCL path was already tried, so it only runs the CPU engine. NOT is unary.
ଶୂନ୍ୟ cv::bitwise_and(ନିବେଶ_Array a, ନିବେଶ_Array b, OutputArray c, ନିବେଶ_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_AND))
    arithm_op(ew::OP_AND, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/ମିଥ୍ୟା);
}

ଶୂନ୍ୟ cv::bitwise_or(ନିବେଶ_Array a, ନିବେଶ_Array b, OutputArray c, ନିବେଶ_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_OR))
    arithm_op(ew::OP_OR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/ମିଥ୍ୟା);
}

ଶୂନ୍ୟ cv::bitwise_xor(ନିବେଶ_Array a, ନିବେଶ_Array b, OutputArray c, ନିବେଶ_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_XOR))
    arithm_op(ew::OP_XOR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/ମିଥ୍ୟା);
}

ଶୂନ୍ୟ cv::bitwise_not(ନିବେଶ_Array a, OutputArray c, ନିବେଶ_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || c.isUMat()) && a.dims() <= 2,
               bitwise_op_ocl(a, a, c, mask, OCL_OP_NOT))
    unary_op(ew::OP_NOT, a, c, mask, -1);
}

ଶୂନ୍ୟ cv::max( ନିବେଶ_Array src1, ନିବେଶ_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, ମିଥ୍ୟା);
}

ଶୂନ୍ୟ cv::min( ନିବେଶ_Array src1, ନିବେଶ_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, ମିଥ୍ୟା);
}

// The concrete Mat/UMat overloads of min/max exist because C++ overload resolution needs them (a
// bare cv::min(mat, mat, mat) binds Mat& more tightly than InputArray); they must run the SAME engine
// path as the InputArray forms - NOT the legacy binary_op - so mixed types and broadcasting work.
ଶୂନ୍ୟ cv::max(ଅଚଳ Mat& src1, ଅଚଳ Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, ମିଥ୍ୟା);
}

ଶୂନ୍ୟ cv::min(ଅଚଳ Mat& src1, ଅଚଳ Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, ମିଥ୍ୟା);
}

ଶୂନ୍ୟ cv::max(ଅଚଳ UMat& src1, ଅଚଳ UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, ମିଥ୍ୟା);
}

ଶୂନ୍ୟ cv::min(ଅଚଳ UMat& src1, ଅଚଳ UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, ମିଥ୍ୟା);
}


/****************************************************************************************\
*                                      add/subtract                                      *
\****************************************************************************************/

ନାମାକାଶ cv
{

#ifdef HAVE_OPENCL

// used by the OpenCL branch only (arithm_op_ocl working-type selection)
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା actualScalarDepth(ଅଚଳ ଦ୍ୱିଗୁଣ* data, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0, minval = INT_MAX, maxval = INT_MIN;
    ପାଇଁ(; i < len; ++i)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ival = cvRound(data[i]);
        ଯଦି( ival != data[i] )
            ଭାଙ୍ଗ;
        minval = MIN(minval, ival);
        maxval = MAX(maxval, ival);
    }
    ଫେରନ୍ତୁ i < len ? CV_64F :
        minval >= 0 && maxval <= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)UCHAR_MAX ? CV_8U :
        minval >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)SCHAR_MIN && maxval <= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)SCHAR_MAX ? CV_8S :
        minval >= 0 && maxval <= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)USHRT_MAX ? CV_16U :
        minval >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)SHRT_MIN && maxval <= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)SHRT_MAX ? CV_16S :
        CV_32S;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା coerceTypes(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth2, ତାର୍କିକ muldiv)
{
    ଫେରନ୍ତୁ depth1 == depth2 ? depth1 :
        ((depth1 <= CV_32S) & (depth2 <= CV_32S)) != 0 ?
        (((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)!muldiv & (depth1 <= CV_8S) & (depth2 <= CV_8S)) != 0 ? CV_16S : CV_32S) :
        ((CV_ELEM_SIZE1(depth1) > 4) | (CV_ELEM_SIZE1(depth2) > 4)) != 0 ? CV_64F : CV_32F;
}

ସ୍ଥିର ତାର୍କିକ ocl_arithm_op(ନିବେଶ_Array _src1, ନିବେଶ_Array _src2, OutputArray _dst,
                          ନିବେଶ_Array _mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା wtype,
                          ଶୂନ୍ୟ* usrdata, ପୂର୍ଣ୍ଣସଂଖ୍ୟା oclop,
                          ତାର୍କିକ haveScalar )
{
    ଅଚଳ ocl::Device d = ocl::Device::getDefault();
    ତାର୍କିକ doubleSupport = d.doubleFPConfig() > 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    ତାର୍କିକ haveMask = !_mask.empty();

    ଯଦି ( (haveMask || haveScalar) && cn > 4 )
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

#ifdef __APPLE__
    ଯଦି (depth1 == CV_16U && (oclop == OCL_OP_MUL || oclop == OCL_OP_MUL_SCALE))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32S, CV_MAT_DEPTH(wtype));
    ଯଦି (!doubleSupport)
        wdepth = std::min(wdepth, CV_32F);

    wtype = CV_MAKETYPE(wdepth, cn);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type2 = haveScalar ? wtype : _src2.type(), depth2 = CV_MAT_DEPTH(type2);
    ଯଦି (!doubleSupport && (depth2 == CV_64F || depth1 == CV_64F))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (noOclType(depth1) || noOclType(depth2) || noOclType(ddepth) || noOclType(wdepth))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = d.isIntel() ? 4 : 1;

    ଅକ୍ଷର cvtstr[4][50], opts[1024];
    snprintf(opts, ଆକାର(opts), "-D %s%s -D %s -D srcT1=%s -D srcT1_C1=%s -D srcT2=%s -D srcT2_C1=%s "
            "-D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workT=%s -D workST=%s -D scaleT=%s -D wdepth=%d -D convertToWT1=%s "
            "-D convertToWT2=%s -D convertToDT=%s%s -D cn=%d -D rowsPerWI=%d -D convertFromU=%s",
            (haveMask ? "MASK_" : ""), (haveScalar ? "UNARY_OP" : "BINARY_OP"),
            oclop2str[oclop], ocl::typeToStr(CV_MAKETYPE(depth1, kercn)),
            ocl::typeToStr(depth1), ocl::typeToStr(CV_MAKETYPE(depth2, kercn)),
            ocl::typeToStr(depth2), ocl::typeToStr(CV_MAKETYPE(ddepth, kercn)), ddepth,
            ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKETYPE(wdepth, kercn)),
            ocl::typeToStr(CV_MAKETYPE(wdepth, scalarcn)),
            ocl::typeToStr(wdepth), wdepth,
            ocl::convertTypeStr(depth1, wdepth, kercn, cvtstr[0], ଆକାର(cvtstr[0])),
            ocl::convertTypeStr(depth2, wdepth, kercn, cvtstr[1], ଆକାର(cvtstr[1])),
            ocl::convertTypeStr(wdepth, ddepth, kercn, cvtstr[2], ଆକାର(cvtstr[2])),
            doubleSupport ? " -D DOUBLE_SUPPORT" : "", kercn, rowsPerWI,
            oclop == OCL_OP_ABSDIFF && wdepth == CV_32S && ddepth == wdepth ?
            ocl::convertTypeStr(CV_8U, ddepth, kercn, cvtstr[3], ଆକାର(cvtstr[3])) : "noconvert");

    size_t usrdata_esz = CV_ELEM_SIZE(wdepth);
    ଅଚଳ uchar* usrdata_p = (ଅଚଳ uchar*)usrdata;
    ଅଚଳ ଦ୍ୱିଗୁଣ* usrdata_d = (ଅଚଳ ଦ୍ୱିଗୁଣ*)usrdata;
    ଦଶମିକ usrdata_f[3];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, n = oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE ||
        oclop == OCL_OP_RDIV_SCALE || oclop == OCL_OP_RECIP_SCALE ? 1 : oclop == OCL_OP_ADDW ? 3 : 0;
    ଯଦି( usrdata && n > 0 && wdepth == CV_32F )
    {
        ପାଇଁ( i = 0; i < n; i++ )
            usrdata_f[i] = (ଦଶମିକ)usrdata_d[i];
        usrdata_p = (ଅଚଳ uchar*)usrdata_f;
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask, 1);

    ଯଦି( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(wtype)*scalarcn;
        ଦ୍ୱିଗୁଣ buf[4]={0,0,0,0};
        Mat src2sc = _src2.getMat();

        ଯଦି( !src2sc.empty() )
            convertAndUnrollScalar(src2sc, wtype, (uchar*)buf, 1);
        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        ଯଦି( !haveMask )
        {
            ଯଦି(n == 0)
                k.args(src1arg, dstarg, scalararg);
            ଅନ୍ୟଥା ଯଦି(n == 1)
                k.args(src1arg, dstarg, scalararg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            ଅନ୍ୟଥା
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        ଅନ୍ୟଥା
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    ଅନ୍ୟଥା
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::ReadOnlyNoSize(src2, cn, kercn);

        ଯଦି( !haveMask )
        {
            ଯଦି (n == 0)
                k.args(src1arg, src2arg, dstarg);
            ଅନ୍ୟଥା ଯଦି (n == 1)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            ଅନ୍ୟଥା ଯଦି (n == 3)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz*2, usrdata_esz));
            ଅନ୍ୟଥା
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        ଅନ୍ୟଥା
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

#endif

// OpenCL branch of the arithmetic ops, extracted from the former arithm_op (whose CPU compute is now
// the element-wise engine). Returns true if an OpenCL kernel handled the op, false to let the caller
// fall through to the CPU engine. min/max use the bitwise-style kernel; every other op the arithm
// kernel with a coerced working type. `usrdata` carries the mul/div/addWeighted scale block, `oclop`
// the kernel id. Only reached for a UMat dst; sizes/types that don't fit the scalar/same-size pattern
// (e.g. a broadcast) decline (return false) so the engine handles them on the CPU.
ସ୍ଥିର ତାର୍କିକ arithm_op_ocl(ନିବେଶ_Array _src1, ନିବେଶ_Array _src2, OutputArray _dst,
                          ନିବେଶ_Array _mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା oclop, ତାର୍କିକ muldiv, ଶୂନ୍ୟ* usrdata)
{
#ifndef HAVE_OPENCL
    CV_UNUSED(_src1); CV_UNUSED(_src2); CV_UNUSED(_dst); CV_UNUSED(_mask);
    CV_UNUSED(dtype); CV_UNUSED(oclop); CV_UNUSED(muldiv); CV_UNUSED(usrdata);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#ଅନ୍ୟଥା
    ଅଚଳ ନିବେଶ_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    ନିବେଶ_Array_2::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    ତାର୍କିକ haveMask = !_mask.empty();
    ତାର୍କିକ reallocate = ମିଥ୍ୟା;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type1 = psrc1->type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type2 = psrc2->type(), depth2 = CV_MAT_DEPTH(type2), cn2 = CV_MAT_CN(type2);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା wtype, dims1 = psrc1->dims(), dims2 = psrc2->dims();
    Size sz1 = dims1 <= 2 ? psrc1->size() : Size();
    Size sz2 = dims2 <= 2 ? psrc2->size() : Size();

    ଯଦି (!(OCL_PERFORMANCE_CHECK(_dst.isUMat()) && dims1 <= 2 && dims2 <= 2))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    // min/max: the bitwise-style OpenCL kernel (same-type element-wise, no working-type coercion).
    ଯଦି (oclop == OCL_OP_MIN || oclop == OCL_OP_MAX)
    {
        ତାର୍କିକ haveScalar = ମିଥ୍ୟା;
        ଯଦି ((kind1 == ନିବେଶ_Array_2::MATX) + (kind2 == ନିବେଶ_Array_2::MATX) == 1 ||
            !psrc1->sameSize(*psrc2) || type1 != type2)
        {
            ଯଦି (checkScalar(*psrc1, type2, kind1, kind2))
            { std::swap(psrc1, psrc2); std::swap(type1, type2); }
            ଅନ୍ୟଥା ଯଦି (!checkScalar(*psrc2, type1, kind2, kind1))
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
            haveScalar = ସତ୍ୟ;
        }
        _dst.createSameSize(*psrc1, type1);
        ଫେରନ୍ତୁ ocl_ଦ୍ୱିମିକ_op(*psrc1, *psrc2, _dst, _mask, ମିଥ୍ୟା, oclop, haveScalar);
    }

    // reciprocal (scale/src): the OpenCL kernel is unary on src - use src2 for both operands (the CPU
    // engine received a 0-dim `1` numerator as src1, which the recip kernel would ignore). [TODO.VP: tidy]
    ଯଦି (oclop == OCL_OP_RECIP_SCALE)
    {
        psrc1 = psrc2; kind1 = kind2; type1 = type2; depth1 = depth2; cn = cn2; dims1 = dims2; sz1 = sz2;
    }

    // add/sub/mul/div/absdiff/addWeighted/recip: the arithm OpenCL kernel with a coerced work type.
    ତାର୍କିକ src1Scalar = checkScalar(*psrc1, type2, kind1, kind2);
    ତାର୍କିକ src2Scalar = checkScalar(*psrc2, type1, kind2, kind1);
    ତାର୍କିକ haveScalar = ମିଥ୍ୟା;

    ଯଦି (dims1 != dims2 || sz1 != sz2 || cn != cn2 ||
        (kind1 == ନିବେଶ_Array_2::MATX && (sz1 == Size(1,4) || sz1 == Size(1,1))) ||
        (kind2 == ନିବେଶ_Array_2::MATX && (sz2 == Size(1,4) || sz2 == Size(1,1))))
    {
        ଯଦି ((type1 == CV_64F && (sz1.height == 1 || sz1.height == 4)) && src1Scalar)
        {
            // src1 is a scalar; swap it with src2
            std::swap(psrc1, psrc2); std::swap(sz1, sz2); std::swap(type1, type2);
            std::swap(depth1, depth2); std::swap(cn, cn2); std::swap(dims1, dims2);
            ଯଦି (oclop == OCL_OP_SUB) oclop = OCL_OP_RSUB;
            ଯଦି (oclop == OCL_OP_DIV_SCALE) oclop = OCL_OP_RDIV_SCALE;
        }
        ଅନ୍ୟଥା ଯଦି (!src2Scalar)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;   // array op array with mismatched size/cn: engine broadcasts on the CPU
        haveScalar = ସତ୍ୟ;
        CV_Assert((type2 == CV_64F || type2 == CV_32F) && (sz2.height == 1 || sz2.height == 4));

        ଯଦି (!muldiv)
        {
            Mat sc = psrc2->getMat();
            depth2 = actualScalarDepth(sc.ptr<ଦ୍ୱିଗୁଣ>(), sz2 == Size(1, 1) ? cn2 : cn);
            ଯଦି (depth2 == CV_64F && CV_ELEM_SIZE1(depth1) < 8)
                depth2 = CV_32F;
        }
        ଅନ୍ୟଥା
            depth2 = CV_64F;
    }

    ଯଦି (dtype < 0)
    {
        ଯଦି (_dst.fixedType())
            dtype = _dst.type();
        ଅନ୍ୟଥା
        {
            ଯଦି (!haveScalar && type1 != type2)
                CV_Error(cv::Error::StsBadArg,
                     "When the input arrays in add/subtract/multiply/divide functions have different types, "
                     "the output array type must be explicitly specified");
            dtype = type1;
        }
    }
    dtype = CV_MAT_DEPTH(dtype);

    ଯଦି (depth1 == depth2 && dtype == depth1)
        wtype = dtype;
    ଅନ୍ୟଥା ଯଦି (!muldiv)
    {
        wtype = coerceTypes(depth1, depth2, ମିଥ୍ୟା);
        wtype = coerceTypes(wtype, dtype, ମିଥ୍ୟା);
        ଯଦି (dtype < CV_32F && (depth1 < CV_32F || depth2 < CV_32F))
            wtype = CV_32S;
    }
    ଅନ୍ୟଥା
    {
        wtype = coerceTypes(depth1, depth2, ସତ୍ୟ);
        wtype = coerceTypes(wtype, dtype, ସତ୍ୟ);
    }

    // The scaled OpenCL kernels (mul/div by a scale, reciprocal, addWeighted) compute in float; the old
    // same-type fast path forced max(depth, CV_32F). coerceTypes keeps same-type integer, so bump here.
    ଯଦି (oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE || oclop == OCL_OP_RDIV_SCALE ||
        oclop == OCL_OP_RECIP_SCALE || oclop == OCL_OP_ADDW)
        wtype = std::max(wtype, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)CV_32F);

    dtype = CV_MAKETYPE(dtype, cn);
    wtype = CV_MAKETYPE(wtype, cn);

    ଯଦି (haveMask)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype = _mask.type();
        CV_Assert((mtype == CV_8UC1 || mtype == CV_8SC1 || mtype == CV_Bool) && _mask.sameSize(*psrc1));
        reallocate = !_dst.sameSize(*psrc1) || _dst.type() != dtype;
    }

    _dst.createSameSize(*psrc1, dtype);
    ଯଦି (reallocate)
        _dst.setTo(0.);

    ଫେରନ୍ତୁ ocl_arithm_op(*psrc1, *psrc2, _dst, _mask, wtype, usrdata, oclop, haveScalar);
#endif
}

// Element-wise binary op (ADD/SUB/...) on two CPU operands - each an array Mat or a MATX scalar -
// through the new broadcasting engine. Handles mixed input types (emitBinary inserts the casts) and
// broadcasting (a scalar rides as a 0-dim per-channel CONST; arrays broadcast via outputShape/exec).
// Returns false (declining) for UMat, a write-mask, or two scalar operands, so the caller falls
// through to arithm_op. Incompatible shapes make the engine throw (matching cv::add's error).
// Does the output array already have exactly this shape+type (=> create() reuses it, no realloc)?
// Works for Mat and UMat alike (shape/type queries, no data-pointer peeking).
ସ୍ଥିର ତାର୍କିକ outArrayMatches(ଅଚଳ _OutputArray& a, ଅଚଳ MatShape& shp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    ଯଦି (a.empty() || a.type() != type)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz[MatShape::MAX_DIMS];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nd = a.sizend(sz);
    ଯଦି (nd != (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)shp.size())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nd; i++)
        ଯଦି (sz[i] != shp[i])
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

// Are two operands' iteration shapes (spatial dims + channels innermost, exactly how the engine
// iterates) numpy-broadcast-compatible? Powers the scalar-like-Mat compat fallback in arithm_op.
// Cheap by design: sizend copies the dim arrays inline - no Mat headers, no allocations.
ସ୍ଥିର ତାର୍କିକ shapesBroadcastCompat(ନିବେଶ_Array a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା acn, ନିବେଶ_Array b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bcn)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା asz[MatShape::MAX_DIMS + 1], bsz[MatShape::MAX_DIMS + 1];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ad = a.sizend(asz), bd = b.sizend(bsz);
    asz[ad++] = acn;
    bsz[bd++] = bcn;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i1 = ad - 1, i2 = bd - 1; i1 >= 0 && i2 >= 0; i1--, i2--)
        ଯଦି (asz[i1] != bsz[i2] && asz[i1] != 1 && bsz[i2] != 1)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

// The one entry point for every element-wise binary op (add/sub/mul/div/min/max/absdiff/addWeighted/
// reciprocal). Handles empty inputs, the UMat/OpenCL branch (arithm_op_ocl) and the CPU element-wise
// engine. `oclop` selects the OpenCL kernel; `muldiv` drives the OpenCL working-type coercion + scale;
// `params` are the op scalars (params[0]=mul/div scale; {alpha,beta,gamma} for addWeighted).
// Below this many ELEMENTS (total * channels), the per-call constant overhead of the engine path
// (program build + compile + executor setup) is comparable to the actual work - take the direct
// kernel call instead. Same tier boundary as math_op's MATH_OP_SMALL for cv::exp.
ଗଣନା { ARITHM_SMALL_DIRECT = 100000 };

ସ୍ଥିର ଶୂନ୍ୟ arithm_op(ew::ଶୀର୍ଷ op, ନିବେଶ_Array src1, ନିବେଶ_Array src2, OutputArray dst,
                      ନିବେଶ_Array mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା oclop, ତାର୍କିକ muldiv, ଅଚଳ Scalar& params)
{
    ଯଦି (src1.empty() || src2.empty())          // empty inputs -> empty result
    {
        CV_Assert((src1.empty() && src2.empty()) ||
                  (src1.empty() && isScalarArg(src2, src1.channels())) ||
                  (src2.empty() && isScalarArg(src1, src2.channels())));
        dst.release();
        ଯଦି (dtype >= 0)
            dst.create(0, 0, dtype);
        ଫେରନ୍ତୁ;
    }

    // UMat -> classic OpenCL kernel. It declines (false) when OpenCL can't apply (no device, dims>2, a
    // broadcast, cn>4 masked, ...) - then the CPU engine below handles it (mapping the UMat via getMat).
    // oclop < 0 is a sentinel: the caller (e.g. cv::compare) already ran its own OpenCL path, so this
    // helper only runs the CPU engine (a UMat operand is mapped via getMat).
    ଯଦି (oclop >= 0 && cv::ocl::isOpenCLActivated() &&
        (src1.isUMat() || src2.isUMat() || dst.isUMat()))
    {
        ଦ୍ୱିଗୁଣ abg[3] = { params[0], params[1], params[2] }, scale = params[0];
        ଶୂନ୍ୟ* usrdata = (op == ew::OP_ADDW) ? (ଶୂନ୍ୟ*)abg : (muldiv ? (ଶୂନ୍ୟ*)&scale : nullptr);
        ଯଦି (arithm_op_ocl(src1, src2, dst, mask, dtype, oclop, muldiv, usrdata))
            ଫେରନ୍ତୁ;
    }

    ଅଚଳ ତାର୍କିକ haveMask = !mask.empty();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn1 = src1.channels(), cn2 = src2.channels();
    ତାର୍କିକ s1 = isScalarArg(src1, src2), s2 = isScalarArg(src2, src1);
    ଯଦି (s1 && s2)
        s1 = s2 = ମିଥ୍ୟା;    // two scalars: treat both as tiny array operands (element-wise + broadcast),
                            // matching arithm_op (Scalar+Scalar -> 4x1, number+number -> 1x1, ...)
    // A 4x1 CV_64F Mat/UMat pseudo-scalar (see isScalarArg) is hijacked only against a REAL array:
    // when the partner is itself a tiny scalar-shaped array (e.g. compare(Mat 4x1, Mat 1x1) -
    // issue #8999), both are honest data and ride the broadcast. A genuine MATX Scalar never demotes.
    ଅନ୍ୟଥା ଯଦି (s1 && src1.kind() != ନିବେଶ_Array_2::MATX && isScalarLikeMat(src2, cn1))
        s1 = ମିଥ୍ୟା;
    ଅନ୍ୟଥା ଯଦି (s2 && src2.kind() != ନିବେଶ_Array_2::MATX && isScalarLikeMat(src1, cn2))
        s2 = ମିଥ୍ୟା;

    // Compat fallback: Java/Python/user code passes scalars as real little Mats (the classic 4x1
    // CV_64F column, a 1xcn/cnx1 vector - see isScalarLikeMat). Those normally ride broadcasting
    // now, so hijack one as a per-channel scalar ONLY when the shapes do NOT broadcast - just the
    // calls that would otherwise throw get the 4.x scalar semantics, every valid broadcast keeps
    // its numpy meaning. Cheap: two geometry probes, and the O(ndims) shape walk runs only when a
    // probe hits (never for ordinary same-size or MATX-scalar calls).
    ଯଦି (!s1 && !s2)
    {
        ତାର୍କିକ like1 = isScalarLikeMat(src1, cn2), like2 = isScalarLikeMat(src2, cn1);
        ଯଦି ((like1 || like2) && !shapesBroadcastCompat(src1, cn1, src2, cn2))
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
    ଅଚଳ ଶୂନ୍ୟ* dstObj = (dst.kind() == ନିବେଶ_Array_2::MAT) ? dst.getObj() : nullptr;
    ଅଚଳ ତାର୍କିକ aliased = dstObj && (dstObj == src1.getObj() || dstObj == src2.getObj());
    Mat m1loc, m2loc, mloc;
    auto asMat = [&](ନିବେଶ_Array a, Mat& loc) -> ଅଚଳ Mat* {
        ଯଦି (a.kind() == ନିବେଶ_Array_2::MAT && !aliased) ଫେରନ୍ତୁ (ଅଚଳ Mat*)a.getObj();
        loc = a.getMat(); ଫେରନ୍ତୁ &loc;
    };
    ଅଚଳ Mat* pm1 = s1 ? nullptr : asMat(src1, m1loc);
    ଅଚଳ Mat* pm2 = s2 ? nullptr : asMat(src2, m2loc);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା adepth = s1 ? pm2->depth() : pm1->depth();   // the (first) array operand's depth
    // auto result depth (no explicit dtype, no fixed-type dst): add/sub/mul/div keep the first array
    // operand's depth (cv::'s dtype==-1 convention), but min/max/absdiff have NO dtype argument and
    // must promote mixed inputs to their common type (min(u32,f32) in u32 would drop the float). For
    // same-type inputs promoteArith(da,db)==da, so classic behaviour is preserved (e.g. absdiff of two
    // s16 stays s16, saturating - the wide unsigned |a-b| is computed then cast back down by emitBinary).
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା autoDepth = adepth;
    ଯଦି (op == ew::OP_MIN || op == ew::OP_MAX || op == ew::OP_ABSDIFF)
    {
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା d1 = s1 ? ew::EW_DEPTH_NONE : pm1->depth();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା d2 = s2 ? ew::EW_DEPTH_NONE : pm2->depth();
        autoDepth = ew::promoteArith(d1, d2);
    }
    // result depth: explicit dtype wins; else a fixed-type dst dictates it; else the auto depth above.
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
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
    ଯଦି (!haveMask && !s1 && !s2 &&
        (op == ew::OP_ADD || op == ew::OP_SUB || op == ew::OP_MIN || op == ew::OP_MAX ||
         op == ew::OP_ABSDIFF || op == ew::OP_MUL || op == ew::OP_ADDW ||
         op == ew::OP_AND || op == ew::OP_OR || op == ew::OP_XOR) &&
        rdepth == pm1->depth() && pm1->type() == pm2->type() && pm1->size == pm2->size &&
        pm1->isContinuous() && pm2->isContinuous() &&
        pm1->total()*cn1 <= (size_t)ARITHM_SMALL_DIRECT)
    {
        ew::TKernel k = ew::getElemwiseFunc(op, rdepth, rdepth, ew::EW_DEPTH_NONE, rdepth);
        ଯଦି (k.fptr)
        {
            dst.createSameSize(src1, pm1->type());   // whole-shape transfer, not piecemeal dims+sizes
            Mat dloc;
            Mat* pd = (dst.kind() == ନିବେଶ_Array_2::MAT) ? (Mat*)dst.getObj() : &(dloc = dst.getMat());
            ଯଦି (pd->isContinuous())     // a reused non-continuous dst view falls through to the engine
            {
                k.fptr(pm1->data, 0, 1, pm2->data, 0, 1, nullptr, 0, 0,
                       pd->data, 0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(pm1->total()*cn1), 1, params.val, k.flags, k.userdata);
                ଫେରନ୍ତୁ;
            }
        }
    }

    // operand order = (src1, src2): an array becomes an INPUT, a scalar a flexible per-channel CONST
    // read straight from the caller's inline storage (getObj()), no Mat / convertTo.
    ew::TExpr p;
    auto addOperand = [&](ନିବେଶ_Array src, ତାର୍କିକ isScalar, ଅଚଳ Mat* m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା otherCn) -> ପୂର୍ଣ୍ଣସଂଖ୍ୟା {
        ଯଦି (!isScalar)
            ଫେରନ୍ତୁ p.add_ନିବେଶ(m->depth());
        ଅଚଳ uchar* sp; ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd;
        uchar scbuf[EW_SCALAR_BUF_SIZE];   // stack room for a UMAT scalar; addConst copies the values out
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn0 = scalarArgElems(src, sp, sd, scbuf), cn = otherCn;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn = (cn == scn0 || (cn < 4 && scn0 == 4)) ? cn
                      : (scn0 == 1) ? 1 : 0;
        CV_Assert(scn > 0);
        ଫେରନ୍ତୁ p.addConst(ew::EW_DEPTH_NONE, sd, sp, scn);
    };
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା a0 = addOperand(src1, s1, pm1, cn2);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା a1 = addOperand(src2, s2, pm2, cn1);

    // Write-mask: a single-channel 1-byte array (u8/s8/bool) the size of the output spatial shape,
    // added as another broadcast input. The arithmetic result lands in a temp; a final
    // select(mask, r, dst) -> dst overwrites only mask!=0 positions of the (pre-existing) output.
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sMask = 0;
    ଅଚଳ Mat* pmask = nullptr;
    ଯଦି (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = asMat(mask, mloc);
        sMask = p.add_ନିବେଶ(pmask->depth());
    }
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sOut = p.addOutput(rdepth);               // output BEFORE emit -> moveToOutput drops the
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = p.emit_ଦ୍ୱିମିକ(op, a0, a1, rdepth, params);   // result temp (mask-free => none); scale
                                                        // rides params[0] (mul/div), {a,b,g} for addW
    ଯଦି (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    ଅନ୍ୟଥା
        p.moveToOutput(r, sOut);
    p.compile();

    ଅଚଳ Mat* inputs[3]; ପୂର୍ଣ୍ଣସଂଖ୍ୟା ni = 0;                   // inputs in program (input-index) order
    ଯଦି (!s1) inputs[ni++] = pm1;
    ଯଦି (!s2) inputs[ni++] = pm2;
    ଯଦି (haveMask) inputs[ni++] = pmask;

    // Pre-create the output at the broadcast shape, ALWAYS via dst.create() - never hand exec a raw
    // dst Mat. Routing through the _OutputArray is what enforces its contract: a FIXED_SIZE/FIXED_TYPE
    // dst (in-place 'a += b' passes '(const Mat&)a') throws here if a broadcast would change its shape
    // or type (Mat(1x1) += Mat(1x4)), instead of silently reallocating - exec sees only a Mat and has
    // no view of the array's fixed flags. Cost over a direct exec is negligible: exec keeps its own
    // fast path for a matching-shape op, and dst.create() with an unchanged shape/type is a no-op.
    // With a mask, a freshly (re)allocated dst is zeroed so mask==0 reads 0 (matches arithm_op); a
    // reused dst keeps its prior content there - detect reuse by shape+type BEFORE create (correct for
    // a reused UMat dst too).
    MatShape oshape; ପୂର୍ଣ୍ଣସଂଖ୍ୟା ocn;
    p.outputShape(inputs, oshape, ocn);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା otype = CV_MAKETYPE(rdepth, ocn);
    ଅଚଳ ତାର୍କିକ reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == ନିବେଶ_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    ଯଦି (haveMask && !reused)
        pdst->setZero();                          // setZero (not setTo, which caps at 4 channels)
    p.exec(inputs, pdst);
}

// Unary element-wise op through the engine (bitwise NOT now; unary math later). One array input, an
// optional write-mask; result depth = dtype (>=0), else a fixed-type dst's depth, else the input
// depth. Mirrors arithm_op's build/mask/exec tail with a single input and no scalar operand. OpenCL is
// run by the caller (bitwise_op_ocl), so this is the CPU engine only.
ସ୍ଥିର ଶୂନ୍ୟ unary_op(ew::ଶୀର୍ଷ op, ନିବେଶ_Array src, OutputArray dst, ନିବେଶ_Array mask,
                     ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype, ଅଚଳ Scalar& params)
{
    ଯଦି (src.empty())
    {
        dst.release();
        ଯଦି (dtype >= 0)
            dst.create(0, 0, dtype);
        ଫେରନ୍ତୁ;
    }

    ଅଚଳ ତାର୍କିକ haveMask = !mask.empty();
    // in-place (dst aliases src): keep a header copy so a shape/type-changing dst.create() can't free
    // src's data mid-op (matches arithm_op). A fixed-size/type dst never reallocs, so this is only for
    // an explicit dtype change.
    ଅଚଳ ଶୂନ୍ୟ* dstObj = (dst.kind() == ନିବେଶ_Array_2::MAT) ? dst.getObj() : nullptr;
    ଅଚଳ ତାର୍କିକ aliased = dstObj && src.kind() == ନିବେଶ_Array_2::MAT && dstObj == src.getObj();
    Mat sloc, mloc;
    ଅଚଳ Mat* psrc = (src.kind() == ନିବେଶ_Array_2::MAT && !aliased) ? (ଅଚଳ Mat*)src.getObj()
                                                                   : &(sloc = src.getMat());
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
                     : (dst.fixedType() ? dst.depth() : psrc->depth());

    ew::TExpr p;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା a0 = p.add_ନିବେଶ(psrc->depth());
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sMask = 0;
    ଅଚଳ Mat* pmask = nullptr;
    ଯଦି (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = (mask.kind() == ନିବେଶ_Array_2::MAT) ? (ଅଚଳ Mat*)mask.getObj() : &(mloc = mask.getMat());
        sMask = p.add_ନିବେଶ(pmask->depth());
    }
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sOut = p.addOutput(rdepth);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = p.emitUnary(op, a0, rdepth, params);
    ଯଦି (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    ଅନ୍ୟଥା
        p.moveToOutput(r, sOut);
    p.compile();

    ଅଚଳ Mat* inputs[2]; ପୂର୍ଣ୍ଣସଂଖ୍ୟା ni = 0;
    inputs[ni++] = psrc;
    ଯଦି (haveMask) inputs[ni++] = pmask;

    MatShape oshape; ପୂର୍ଣ୍ଣସଂଖ୍ୟା ocn;
    p.outputShape(inputs, oshape, ocn);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା otype = CV_MAKETYPE(rdepth, ocn);
    ଅଚଳ ତାର୍କିକ reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == ନିବେଶ_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    ଯଦି (haveMask && !reused)
        pdst->setZero();
    p.exec(inputs, pdst);
}

}

ଶୂନ୍ୟ cv::add( ନିବେଶ_Array src1, ନିବେଶ_Array src2, OutputArray dst,
          ନିବେଶ_Array mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADD, src1, src2, dst, mask, dtype, OCL_OP_ADD, ମିଥ୍ୟା);
}

ଶୂନ୍ୟ cv::subtract( ନିବେଶ_Array src1, ନିବେଶ_Array src2, OutputArray dst,
                   ନିବେଶ_Array mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_SUB, src1, src2, dst, mask, dtype, OCL_OP_SUB, ମିଥ୍ୟା);
}

ଶୂନ୍ୟ cv::absdiff( ନିବେଶ_Array src1, ନିବେଶ_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ABSDIFF, src1, src2, dst, noArray(), -1, OCL_OP_ABSDIFF, ମିଥ୍ୟା);
}

ଶୂନ୍ୟ cv::copyTo(ନିବେଶ_Array _src, OutputArray _dst, ନିବେଶ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    _src.copyTo(_dst, _mask);
}

/****************************************************************************************\
*                                    multiply/divide                                     *
\****************************************************************************************/

ନାମାକାଶ cv
{

ଶୂନ୍ୟ multiply(ନିବେଶ_Array src1, ନିବେଶ_Array src2,
              OutputArray dst, ଦ୍ୱିଗୁଣ scale, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype)
{
    CV_INSTRUMENT_REGION();

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା oclop = std::abs(scale - 1.0) < DBL_EPSILON ? OCL_OP_MUL : OCL_OP_MUL_SCALE;
    arithm_op(ew::OP_MUL, src1, src2, dst, noArray(), dtype, oclop, /*muldiv*/ ସତ୍ୟ, Scalar(scale));
}

ଶୂନ୍ୟ divide(ନିବେଶ_Array src1, ନିବେଶ_Array src2,
                OutputArray dst, ଦ୍ୱିଗୁଣ scale, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_DIV, src1, src2, dst, noArray(), dtype, OCL_OP_DIV_SCALE, /*muldiv*/ ସତ୍ୟ, Scalar(scale));
}

ଶୂନ୍ୟ divide(ଦ୍ୱିଗୁଣ scale, ନିବେଶ_Array src2,
                OutputArray dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype)
{
    CV_INSTRUMENT_REGION();

    ଯଦି (src2.empty())
    {
        dst.release();
        ଫେରନ୍ତୁ;
    }

    // scale / src2 == scale * 1 / src2: feed a 0-dim `1` as the numerator so the CPU engine reuses the
    // normal divide (scale*a/b). Give the numerator src2's OWN depth: then OP_DIV(T, T) is same-type and
    // takes the fast per-type kernel (f32 work for <=16-bit / f16 / bf16, not the s32->f64 path a CV_32S
    // numerator would force). An integer src2 keeps its div-by-zero->0 guard (both operands integer); a
    // float src2 divides as float (1/0->inf), both matching cv::divide. dtype<0 follows src2's depth (a
    // fixed-type dst still wins). The UMat path uses the dedicated reciprocal kernel (OCL_OP_RECIP_SCALE).
    ଦ୍ୱିଗୁଣ one = 1;
    Mat numerator(MatShape::scalar(), src2.depth(), &one);
    scalarToRawData(Scalar(1), &one, src2.depth(), 1);       // write `1` in src2's depth (no allocation)
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rtype = (dtype < 0 && !dst.fixedType()) ? src2.depth() : dtype;
    arithm_op(ew::OP_DIV, numerator, src2, dst, noArray(), rtype, OCL_OP_RECIP_SCALE, /*muldiv*/ ସତ୍ୟ, Scalar(scale));
}

UMat UMat::mul(ନିବେଶ_Array m, ଦ୍ୱିଗୁଣ scale) ଅଚଳ
{
    UMat dst;
    multiply(*ଏହି, m, dst, scale);
    ଫେରନ୍ତୁ dst;
}

/****************************************************************************************\
*                                      addWeighted                                       *
\****************************************************************************************/

}

ଶୂନ୍ୟ cv::addWeighted( ନିବେଶ_Array src1, ଦ୍ୱିଗୁଣ alpha, ନିବେଶ_Array src2,
                      ଦ୍ୱିଗୁଣ beta, ଦ୍ୱିଗୁଣ gamma, OutputArray dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADDW, src1, src2, dst, noArray(), dtype, OCL_OP_ADDW, /*muldiv*/ ସତ୍ୟ,
              Scalar(alpha, beta, gamma));
}


/****************************************************************************************\
*                                          compare                                       *
\****************************************************************************************/

ନାମାକାଶ cv
{


ସ୍ଥିର ଦ୍ୱିଗୁଣ getMinVal(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ସ୍ଥିର ଅଚଳ ଦ୍ୱିଗୁଣ tab[CV_DEPTH_MAX] =
    {
        0, -128, 0, -32768, INT_MIN, -FLT_MAX, -DBL_MAX,
        -65504, -FLT_MAX, 0, 0, (ଦ୍ୱିଗୁଣ)INT64_MIN, 0
    };
    ଫେରନ୍ତୁ tab[depth];
}

ସ୍ଥିର ଦ୍ୱିଗୁଣ getMaxVal(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ସ୍ଥିର ଅଚଳ ଦ୍ୱିଗୁଣ tab[CV_DEPTH_MAX] = {
        255, 127, 65535, 32767, INT_MAX, FLT_MAX, DBL_MAX,
        65504, FLT_MAX, 255, (ଦ୍ୱିଗୁଣ)UINT64_MAX, (ଦ୍ୱିଗୁଣ)INT64_MAX, (ଦ୍ୱିଗୁଣ)UINT32_MAX, 0
    };
    ଫେରନ୍ତୁ tab[depth];
}

#ifdef HAVE_OPENCL

ସ୍ଥିର ତାର୍କିକ ocl_compare(ନିବେଶ_Array _src1, ନିବେଶ_Array _src2, OutputArray _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା op, ତାର୍କିକ haveScalar)
{
    ଅଚଳ ocl::Device& dev = ocl::Device::getDefault();
    ତାର୍କିକ doubleSupport = dev.doubleFPConfig() > 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1),
            type2 = _src2.type(), depth2 = CV_MAT_DEPTH(type2);

    ଯଦି (!doubleSupport && depth1 == CV_64F)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    // the integer scalar path below works in int; let the CPU handle the wider 5.x integer depths
    ଯଦି (haveScalar && (depth1 == CV_32U || depth1 == CV_64U || depth1 == CV_64S))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (!haveScalar && (!_src1.sameSize(_src2) || type1 != type2))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

    // This OpenCL kernel broadcasts a scalar's FIRST channel to all channels. The CPU path compares
    // per-channel (scalar[c] for channel c), so a multichannel array against a scalar with DISTINCT
    // channel values would disagree - bail to the CPU engine in that case (rare; not worth a kernel fix).
    ଯଦି (haveScalar && cn > 1)
    {
        Mat sc = _src2.getMat();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sc.total();
        ଯଦି (scn > 1)
        {
            ଦ୍ୱିଗୁଣ v[4] = { 0, 0, 0, 0 };
            getConvertFunc(sc.depth(), CV_64F)(sc.ptr(), 1, 0, 1, (uchar*)v, 1, Size(std::min(scn, 4), 1), 0);
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1, n = std::min(cn, scn); i < n; i++)
                ଯଦି (v[i] != v[0])
                    ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା kercn = haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst), rowsPerWI = dev.isIntel() ? 4 : 1;
    // Workaround for bug with "?:" operator in AMD OpenCL compiler
    ଯଦି (depth1 >= CV_16U)
        kercn = 1;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା scalarcn = kercn == 3 ? 4 : kercn;
    ଅଚଳ ଅକ୍ଷର * ଅଚଳ operationMap[] = { "==", ">", ">=", "<", "<=", "!=" };
    ଅକ୍ଷର cvt[50];

    String opts = format("-D %s -D srcT1=%s -D dstT=%s -D DEPTH_dst=%d -D workT=srcT1 -D cn=%d"
                         " -D convertToDT=%s -D OP_CMP -D CMP_OPERATOR=%s -D srcT1_C1=%s"
                         " -D srcT2_C1=%s -D dstT_C1=%s -D workST=%s -D rowsPerWI=%d%s",
                         haveScalar ? "UNARY_OP" : "BINARY_OP",
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, kercn)),
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U, kercn,
                         ocl::convertTypeStr(depth1, CV_8U, kercn, cvt, ଆକାର(cvt)),
                         operationMap[op], ocl::typeToStr(depth1),
                         ocl::typeToStr(depth1), ocl::typeToStr(CV_8U),
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, scalarcn)), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat src1 = _src1.getUMat();
    Size size = src1.size();
    _dst.create(size, CV_8UC(cn));
    UMat dst = _dst.getUMat();

    ଯଦି (haveScalar)
    {
        size_t esz = CV_ELEM_SIZE1(type1) * scalarcn;
        ଦ୍ୱିଗୁଣ buf[4] = { 0, 0, 0, 0 };
        Mat src2 = _src2.getMat();

        ଯଦି( depth1 > CV_32S )
            convertAndUnrollScalar( src2, depth1, (uchar *)buf, kercn );
        ଅନ୍ୟଥା
        {
            ଦ୍ୱିଗୁଣ fval = 0;
            getConvertFunc(depth2, CV_64F)(src2.ptr(), 1, 0, 1, (uchar *)&fval, 1, Size(1, 1), 0);
            ଯଦି( fval < getMinVal(depth1) )
                ଫେରନ୍ତୁ dst.setTo(Scalar::all(op == CMP_GT || op == CMP_GE || op == CMP_NE ? 255 : 0)), ସତ୍ୟ;

            ଯଦି( fval > getMaxVal(depth1) )
                ଫେରନ୍ତୁ dst.setTo(Scalar::all(op == CMP_LT || op == CMP_LE || op == CMP_NE ? 255 : 0)), ସତ୍ୟ;

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ival = cvRound(fval);
            ଯଦି( fval != ival )
            {
                ଯଦି( op == CMP_LT || op == CMP_GE )
                    ival = cvCeil(fval);
                ଅନ୍ୟଥା ଯଦି( op == CMP_LE || op == CMP_GT )
                    ival = cvFloor(fval);
                ଅନ୍ୟଥା
                    ଫେରନ୍ତୁ dst.setTo(Scalar::all(op == CMP_NE ? 255 : 0)), ସତ୍ୟ;
            }
            convertAndUnrollScalar(Mat(1, 1, CV_32S, &ival), depth1, (uchar *)buf, kercn);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        k.args(ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn),
               ocl::KernelArg::WriteOnly(dst, cn, kercn), scalararg);
    }
    ଅନ୍ୟଥା
    {
        UMat src2 = _src2.getUMat();

        k.args(ocl::KernelArg::ReadOnlyNoSize(src1),
               ocl::KernelArg::ReadOnlyNoSize(src2),
               ocl::KernelArg::WriteOnly(dst, cn, kercn));
    }

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

#endif

}

ଶୂନ୍ୟ cv::compare(ନିବେଶ_Array _src1, ନିବେଶ_Array _src2, OutputArray _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା op)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( op == CMP_LT || op == CMP_LE || op == CMP_EQ ||
               op == CMP_NE || op == CMP_GE || op == CMP_GT );

    CV_Assert(_src1.empty() == _src2.empty());
    ଯଦି (_src1.empty() && _src2.empty())
    {
        _dst.release();
        ଫେରନ୍ତୁ;
    }

    ତାର୍କିକ haveScalar = ମିଥ୍ୟା;

    ଯଦି ((_src1.isMatx() + _src2.isMatx()) == 1
            || !_src1.sameSize(_src2)
            || _src1.type() != _src2.type())
    {
        ତାର୍କିକ is_src1_scalar = checkScalar(_src1, _src2.type(), _src1.kind(), _src2.kind());
        ତାର୍କିକ is_src2_scalar = checkScalar(_src2, _src1.type(), _src2.kind(), _src1.kind());

        // exactly one scalar operand: keep it as src2 (swapping flips the ordering ops) so the OpenCL
        // path sees the array-op-scalar form. Otherwise - two arrays of different size (broadcast),
        // mixed types, or two scalars - the CPU engine broadcasts / promotes them, so there is NO error
        // here: fall through to arithm_op (which handles scalar detection, broadcast and mixed types).
        ଯଦି (is_src1_scalar && !is_src2_scalar)
        {
            op = op == CMP_LT ? CMP_GT : op == CMP_LE ? CMP_GE :
                op == CMP_GE ? CMP_LE : op == CMP_GT ? CMP_LT : op;
            // src1 is a scalar; swap it with src2
            compare(_src2, _src1, _dst, op);
            ଫେରନ୍ତୁ;
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
    ew::ଶୀର୍ଷ cmpOp = op == CMP_EQ ? ew::OP_CMP_EQ : op == CMP_NE ? ew::OP_CMP_NE :
                    op == CMP_LT ? ew::OP_CMP_LT : op == CMP_LE ? ew::OP_CMP_LE :
                    op == CMP_GT ? ew::OP_CMP_GT : ew::OP_CMP_GE;
    arithm_op(cmpOp, _src1, _src2, _dst, noArray(), CV_8U, /*oclop=*/-1, /*muldiv=*/ମିଥ୍ୟା);
}

/****************************************************************************************\
*                                        inRange                                         *
\****************************************************************************************/

ନାମାକାଶ cv
{

template <typename T>
ସଂରଚନା InRange_SIMD
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator () (ଅଚଳ T * src1, ଅଚଳ T * src2, ଅଚଳ T * src3, uchar * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
    {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0; x < len; x++)
            dst[x] = (src2[x] <= src1[x] && src1[x] <= src3[x]) ? 255 : 0;
        ଫେରନ୍ତୁ len;
    }
};

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)

template <>
ସଂରଚନା InRange_SIMD<uchar>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator () (ଅଚଳ uchar * src1, ଅଚଳ uchar * src2, ଅଚଳ uchar * src3,
        uchar * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = VTraits<v_uint8>::vlanes();

        ପାଇଁ (; x <= len - width; x += width)
        {
            v_uint8 values = vx_load(src1 + x);
            v_uint8 ତଳ = vx_load(src2 + x);
            v_uint8 ଉପର = vx_load(src3 + x);

            v_store(dst + x, v_and(v_ge(values, ତଳ), v_ge(ଉପର, values)));
        }
        vx_cleanup();
        ଫେରନ୍ତୁ x;
    }
};

template <>
ସଂରଚନା InRange_SIMD<schar>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator () (ଅଚଳ schar * src1, ଅଚଳ schar * src2, ଅଚଳ schar * src3,
        uchar * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = VTraits<v_int8>::vlanes();

        ପାଇଁ (; x <= len - width; x += width)
        {
            v_int8 values = vx_load(src1 + x);
            v_int8 ତଳ = vx_load(src2 + x);
            v_int8 ଉପର = vx_load(src3 + x);

            v_store((schar*)(dst + x), v_and(v_ge(values, ତଳ), v_ge(ଉପର, values)));
        }
        vx_cleanup();
        ଫେରନ୍ତୁ x;
    }
};

template <>
ସଂରଚନା InRange_SIMD<ushort>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator () (ଅଚଳ ushort * src1, ଅଚଳ ushort * src2, ଅଚଳ ushort * src3,
        uchar * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = VTraits<v_uint16>::vlanes() * 2;

        ପାଇଁ (; x <= len - width; x += width)
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
        ଫେରନ୍ତୁ x;
    }
};

template <>
ସଂରଚନା InRange_SIMD<short>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator () (ଅଚଳ short * src1, ଅଚଳ short * src2, ଅଚଳ short * src3,
        uchar * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)VTraits<v_int16>::vlanes() * 2;

        ପାଇଁ (; x <= len - width; x += width)
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
        ଫେରନ୍ତୁ x;
    }
};

template <>
ସଂରଚନା InRange_SIMD<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator () (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା * src1, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା * src2, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା * src3,
        uchar * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)VTraits<v_int32>::vlanes() * 2;

        ପାଇଁ (; x <= len - width; x += width)
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
        ଫେରନ୍ତୁ x;
    }
};

template <>
ସଂରଚନା InRange_SIMD<ଦଶମିକ>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator () (ଅଚଳ ଦଶମିକ * src1, ଅଚଳ ଦଶମିକ * src2, ଅଚଳ ଦଶମିକ * src3,
        uchar * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)VTraits<v_float32>::vlanes() * 2;

        ପାଇଁ (; x <= len - width; x += width)
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
        ଫେରନ୍ତୁ x;
    }
};

template <>
ସଂରଚନା InRange_SIMD<hfloat>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator () (ଅଚଳ hfloat * src1, ଅଚଳ hfloat * src2, ଅଚଳ hfloat * src3,
        uchar * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)VTraits<v_float32>::vlanes()*2;

        ପାଇଁ (; x <= len - width; x += width)
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
        ଫେରନ୍ତୁ x;
    }
};

template <>
ସଂରଚନା InRange_SIMD<bfloat>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator () (ଅଚଳ bfloat * src1, ଅଚଳ bfloat * src2, ଅଚଳ bfloat * src3,
        uchar * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)VTraits<v_float32>::vlanes()*2;

        ପାଇଁ (; x <= len - width; x += width)
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
        ଫେରନ୍ତୁ x;
    }
};

template <>
ସଂରଚନା InRange_SIMD<unsigned>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator () (ଅଚଳ unsigned * src1, ଅଚଳ unsigned * src2, ଅଚଳ unsigned * src3,
        uchar * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)VTraits<v_uint32>::vlanes() * 2;

        ପାଇଁ (; x <= len - width; x += width)
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
        ଫେରନ୍ତୁ x;
    }
};

#ଯଦି CV_SIMD_64F

template <>
ସଂରଚନା InRange_SIMD<ଦ୍ୱିଗୁଣ>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator () (ଅଚଳ ଦ୍ୱିଗୁଣ * src1, ଅଚଳ ଦ୍ୱିଗୁଣ * src2, ଅଚଳ ଦ୍ୱିଗୁଣ * src3,
        uchar * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) ଅଚଳ
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା step = VTraits<v_float64>::vlanes();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = step * 4;

        ପାଇଁ (; x <= len - width; x += width)
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
        ଫେରନ୍ତୁ x;
    }
};

#endif

#endif

template <typename T>
ସ୍ଥିର ଶୂନ୍ୟ inRange_(ଅଚଳ ଶୂନ୍ୟ* _src1, size_t step1, ଅଚଳ ଶୂନ୍ୟ* _src2, size_t step2,
         ଅଚଳ ଶୂନ୍ୟ* _src3, size_t step3, uchar* dst, size_t step,
         Size size)
{
    ଅଚଳ T* src1 = (ଅଚଳ T*)_src1;
    ଅଚଳ T* src2 = (ଅଚଳ T*)_src2;
    ଅଚଳ T* src3 = (ଅଚଳ T*)_src3;
    step1 /= ଆକାର(src1[0]);
    step2 /= ଆକାର(src2[0]);
    step3 /= ଆକାର(src3[0]);

    InRange_SIMD<T> vop;

    ପାଇଁ( ; size.height--; src1 += step1, src2 += step2, src3 += step3, dst += step )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = vop(src1, src2, src3, dst, size.width);
        #ଯଦି CV_ENABLE_UNROLLED
        ପାଇଁ( ; x <= size.width - 4; x += 4 )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା t0, t1;
            t0 = src2[x] <= src1[x] && src1[x] <= src3[x];
            t1 = src2[x+1] <= src1[x+1] && src1[x+1] <= src3[x+1];
            dst[x] = (uchar)-t0; dst[x+1] = (uchar)-t1;
            t0 = src2[x+2] <= src1[x+2] && src1[x+2] <= src3[x+2];
            t1 = src2[x+3] <= src1[x+3] && src1[x+3] <= src3[x+3];
            dst[x+2] = (uchar)-t0; dst[x+3] = (uchar)-t1;
        }
        #endif
        ପାଇଁ( ; x < size.width; x++ )
            dst[x] = (uchar)-(src2[x] <= src1[x] && src1[x] <= src3[x]);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ inRangeReduce(ଅଚଳ uchar* src, uchar* dst, size_t len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = cn % 4 ? cn % 4 : 4;
    size_t i, j;
    ଯଦି( k == 1 )
        ପାଇଁ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j];
    ଅନ୍ୟଥା ଯଦି( k == 2 )
        ପାଇଁ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1];
    ଅନ୍ୟଥା ଯଦି( k == 3 )
        ପାଇଁ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2];
    ଅନ୍ୟଥା
        ପାଇଁ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2] & src[j+3];

    ପାଇଁ( ; k < cn; k += 4 )
    {
        ପାଇଁ( i = 0, j = k; i < len; i++, j += cn )
            dst[i] &= src[j] & src[j+1] & src[j+2] & src[j+3];
    }
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*InRangeFunc)( ଅଚଳ ଶୂନ୍ୟ* src1, size_t step1, ଅଚଳ ଶୂନ୍ୟ* src2, size_t step2,
                             ଅଚଳ ଶୂନ୍ୟ* src3, size_t step3, uchar* dst, size_t step, Size sz );

ସ୍ଥିର InRangeFunc getInRangeFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ସ୍ଥିର InRangeFunc inRangeTab[CV_DEPTH_MAX] =
    {
        inRange_<uchar>,
        inRange_<schar>,
        inRange_<ushort>,
        inRange_<short>,
        inRange_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>,
        inRange_<ଦଶମିକ>,
        inRange_<ଦ୍ୱିଗୁଣ>,
        inRange_<hfloat>,
        inRange_<bfloat>,
        0,
        inRange_<uint64_t>,
        inRange_<int64_t>,
        inRange_<unsigned>,
        0,
    };

    ଫେରନ୍ତୁ inRangeTab[depth];
}

#ifdef HAVE_OPENCL

ସ୍ଥିର ତାର୍କିକ ocl_inRange( ନିବେଶ_Array _src, ନିବେଶ_Array _lowerb,
                         ନିବେଶ_Array _upperb, OutputArray _dst )
{
    ଅଚଳ ocl::Device & d = ocl::Device::getDefault();
    ନିବେଶ_Array_2::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Size ssize = _src.size(), lsize = _lowerb.size(), usize = _upperb.size();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype = _src.type(), ltype = _lowerb.type(), utype = _upperb.type();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth = CV_MAT_DEPTH(stype), ldepth = CV_MAT_DEPTH(ltype), udepth = CV_MAT_DEPTH(utype);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = CV_MAT_CN(stype), rowsPerWI = d.isIntel() ? 4 : 1;
    ତାର୍କିକ lbScalar = ମିଥ୍ୟା, ubScalar = ମିଥ୍ୟା;

    ଯଦି( (lkind == ନିବେଶ_Array_2::MATX && skind != ନିବେଶ_Array_2::MATX) ||
        ssize != lsize || stype != ltype )
    {
        ଯଦି( !checkScalar(_lowerb, stype, lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = ସତ୍ୟ;
    }

    ଯଦି( (ukind == ନିବେଶ_Array_2::MATX && skind != ନିବେଶ_Array_2::MATX) ||
        ssize != usize || stype != utype )
    {
        ଯଦି( !checkScalar(_upperb, stype, ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = ସତ୍ୟ;
    }

    ଯଦି (lbScalar != ubScalar)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ତାର୍କିକ doubleSupport = d.doubleFPConfig() > 0,
            haveScalar = lbScalar && ubScalar;

    ଯଦି ( (!doubleSupport && sdepth == CV_64F) ||
         (!haveScalar && (sdepth != ldepth || sdepth != udepth)) )
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା kercn = haveScalar ? cn : std::max(std::min(ocl::predictOptimalVectorWidth(_src, _lowerb, _upperb, _dst), 4), cn);
    ଯଦି (kercn % cn != 0)
        kercn = cn;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା colsPerWI = kercn / cn;
    String opts = format("%s-D CN=%d -D SRC_T=%s -D SRC_T1=%s -D DST_T=%s -D KERCN=%d -D DEPTH=%d%s -D COLS_PER_WI=%d",
                           haveScalar ? "-D HAVE_SCALAR " : "", cn, ocl::typeToStr(CV_MAKE_TYPE(sdepth, kercn)),
                           ocl::typeToStr(sdepth), ocl::typeToStr(CV_8UC(colsPerWI)), kercn, sdepth,
                           doubleSupport ? " -D DOUBLE_SUPPORT" : "", colsPerWI);

    ocl::Kernel ker("inrange", ocl::core::inrange_oclsrc, opts);
    ଯଦି (ker.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    _dst.create(ssize, CV_8UC1);
    UMat src = _src.getUMat(), dst = _dst.getUMat(), lscalaru, uscalaru;
    Mat lscalar, uscalar;

    ଯଦି (lbScalar && ubScalar)
    {
        lscalar = _lowerb.getMat();
        uscalar = _upperb.getMat();

        size_t esz = src.elemSize();
        size_t blocksize = 36;

        AutoBuffer<uchar> _buf(blocksize*(((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)lbScalar + (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ubScalar)*esz + cn) + 2*cn*ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) + 128);
        uchar *buf = alignPtr(_buf.data() + blocksize*cn, 16);

        ଯଦି( ldepth != sdepth && sdepth < CV_32S )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା* ilbuf = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)alignPtr(buf + blocksize*esz, 16);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା* iubuf = ilbuf + cn;

            ଦ୍ୱିମିକ_Func sccvtfunc = getConvertFunc(ldepth, CV_32S);
            sccvtfunc(lscalar.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, Size(cn, 1), 0);
            sccvtfunc(uscalar.ptr(), 1, 0, 1, (uchar*)iubuf, 1, Size(cn, 1), 0);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା minval = cvRound(getMinVal(sdepth)), maxval = cvRound(getMaxVal(sdepth));

            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++ )
            {
                ଯଦି( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lscalar = Mat(cn, 1, CV_32S, ilbuf);
            uscalar = Mat(cn, 1, CV_32S, iubuf);
        }

        lscalar.convertTo(lscalar, stype);
        uscalar.convertTo(uscalar, stype);
    }
    ଅନ୍ୟଥା
    {
        lscalaru = _lowerb.getUMat();
        uscalaru = _upperb.getUMat();
    }

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, 1, colsPerWI);

    ଯଦି (haveScalar)
    {
        lscalar.copyTo(lscalaru);
        uscalar.copyTo(uscalaru);

        ker.args(srcarg, dstarg, ocl::KernelArg::PtrReadOnly(lscalaru),
               ocl::KernelArg::PtrReadOnly(uscalaru), rowsPerWI);
    }
    ଅନ୍ୟଥା
        ker.args(srcarg, dstarg, ocl::KernelArg::ReadOnlyNoSize(lscalaru),
               ocl::KernelArg::ReadOnlyNoSize(uscalaru), rowsPerWI);

    size_t globalsize[2] = { (size_t)ssize.width / colsPerWI, ((size_t)ssize.height + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ ker.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

#endif

}

ଶୂନ୍ୟ cv::inRange(ନିବେଶ_Array _src, ନିବେଶ_Array _lowerb,
                 ନିବେଶ_Array _upperb, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(! _src.empty());
    ଯଦି (!getInRangeFunc(_src.depth()))
        CV_Error( cv::Error::StsNotImplemented,
                 "inRange: CV_Bool and CV_8F inputs are not supported, convert the input to another type first");

    CV_OCL_RUN(_src.dims() <= 2 && _lowerb.dims() <= 2 &&
               _upperb.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat()),
               ocl_inRange(_src, _lowerb, _upperb, _dst))

    ନିବେଶ_Array_2::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Mat src = _src.getMat(), lb = _lowerb.getMat(), ub = _upperb.getMat();

    ତାର୍କିକ lbScalar = ମିଥ୍ୟା, ubScalar = ମିଥ୍ୟା;

    ଯଦି( (lkind == ନିବେଶ_Array_2::MATX && skind != ନିବେଶ_Array_2::MATX) ||
        src.size != lb.size || src.type() != lb.type() )
    {
        ଯଦି( !checkScalar(lb, src.type(), lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = ସତ୍ୟ;
    }

    ଯଦି( (ukind == ନିବେଶ_Array_2::MATX && skind != ନିବେଶ_Array_2::MATX) ||
        src.size != ub.size || src.type() != ub.type() )
    {
        ଯଦି( !checkScalar(ub, src.type(), ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = ସତ୍ୟ;
    }

    CV_Assert(lbScalar == ubScalar);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = src.channels(), depth = src.depth();

    size_t esz = src.elemSize();
    size_t blocksize0 = (size_t)(BLOCK_SIZE + esz-1)/esz;

    _dst.createSameSize(_src, CV_8UC1);
    Mat dst = _dst.getMat();
    InRangeFunc func = getInRangeFunc(depth);

    ଅଚଳ Mat* arrays_sc[] = { &src, &dst, 0 };
    ଅଚଳ Mat* arrays_nosc[] = { &src, &dst, &lb, &ub, 0 };
    uchar* ptrs[4] = {};

    NAryMatIterator it(lbScalar && ubScalar ? arrays_sc : arrays_nosc, ptrs);
    size_t total = it.size, blocksize = std::min(total, blocksize0);

    AutoBuffer<uchar> _buf(blocksize*(((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)lbScalar + (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ubScalar)*esz + cn) + 2*cn*ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) + 128);
    uchar *buf = _buf.data(), *mbuf = buf, *lbuf = 0, *ubuf = 0;
    buf = alignPtr(buf + blocksize*cn, 16);

    ଯଦି( lbScalar && ubScalar )
    {
        lbuf = buf;
        ubuf = buf = alignPtr(buf + blocksize*esz, 16);

        CV_Assert( lb.type() == ub.type() );
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା scdepth = lb.depth();

        ଯଦି( scdepth != depth && depth < CV_32S )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା* ilbuf = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)alignPtr(buf + blocksize*esz, 16);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା* iubuf = ilbuf + cn;

            ଦ୍ୱିମିକ_Func sccvtfunc = getConvertFunc(scdepth, CV_32S);
            sccvtfunc(lb.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, Size(cn, 1), 0);
            sccvtfunc(ub.ptr(), 1, 0, 1, (uchar*)iubuf, 1, Size(cn, 1), 0);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା minval = cvRound(getMinVal(depth)), maxval = cvRound(getMaxVal(depth));

            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++ )
            {
                ଯଦି( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lb = Mat(cn, 1, CV_32S, ilbuf);
            ub = Mat(cn, 1, CV_32S, iubuf);
        }

        convertAndUnrollScalar( lb, src.type(), lbuf, blocksize );
        convertAndUnrollScalar( ub, src.type(), ubuf, blocksize );

        ଯଦି (depth == CV_8U && src.dims <= 2) {
            uint8_t lb_scalar = lbuf[0];
            uint8_t ub_scalar = ubuf[0];
            CALL_HAL(inRange_u8, cv_hal_inRange8u, src.data, src.step, dst.data, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        } ଅନ୍ୟଥା ଯଦି (depth == CV_32F && src.dims <= 2) {
            ଦ୍ୱିଗୁଣ lb_scalar = lb.ptr<ଦ୍ୱିଗୁଣ>(0)[0];
            ଦ୍ୱିଗୁଣ ub_scalar = ub.ptr<ଦ୍ୱିଗୁଣ>(0)[0];
            CALL_HAL(inRange_f32, cv_hal_inRange32f, src.data, src.step, dst.data, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        }
    }

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ପାଇଁ( size_t j = 0; j < total; j += blocksize )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା bsz = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)MIN(total - j, blocksize);
            size_t delta = bsz*esz;
            uchar *lptr = lbuf, *uptr = ubuf;
            ଯଦି( !lbScalar )
            {
                lptr = ptrs[2];
                ptrs[2] += delta;
            }
            ଯଦି( !ubScalar )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx = !lbScalar ? 3 : 2;
                uptr = ptrs[idx];
                ptrs[idx] += delta;
            }
            func( ptrs[0], 0, lptr, 0, uptr, 0, cn == 1 ? ptrs[1] : mbuf, 0, Size(bsz*cn, 1));
            ଯଦି( cn > 1 )
                inRangeReduce(mbuf, ptrs[1], bsz, cn);
            ptrs[0] += delta;
            ptrs[1] += bsz;
        }
    }
}
/* End of file. */
