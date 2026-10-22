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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "arithm_expr.hpp"   // the new element-wise engine (cv::ew)
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"

ਨਾਮਾਕਾਸ਼ cv
{

/****************************************************************************************\
*                                   logical operations                                   *
\****************************************************************************************/

ਗਿਣਤੀ { OCL_OP_ਜੋੜੋ=0, OCL_OP_SUB=1, OCL_OP_RSUB=2, OCL_OP_ABSDIFF=3, OCL_OP_MUL=4,
       OCL_OP_MUL_SCALE=5, OCL_OP_DIV_SCALE=6, OCL_OP_RECIP_SCALE=7, OCL_OP_ADDW=8,
       OCL_OP_AND=9, OCL_OP_OR=10, OCL_OP_XOR=11, OCL_OP_NOT=12, OCL_OP_MIN=13, OCL_OP_MAX=14,
       OCL_OP_RDIV_SCALE=15 };

// The unified entry point for every element-wise binary op (empty + UMat/OpenCL + CPU engine). Defined
// lower down (near cv::add); forward-declared here so cv::min/cv::max (above it) can use it too.
ਸਥਿਰ ਖਾਲੀ arithm_op(ew::ਸਿਖਰ op, ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, OutputArray dst,
                      ਨਿਵੇਸ਼_Array ਨਕਾਬ, ਪੂਰਨ dtype, ਪੂਰਨ oclop, ਤਾਰਕਿਕ muldiv, ਅਚਲ Scalar& params = Scalar(1));

// The unary counterpart (bitwise NOT, unary math later). Forward-declared for the same reason.
ਸਥਿਰ ਖਾਲੀ unary_op(ew::ਸਿਖਰ op, ਨਿਵੇਸ਼_Array src, OutputArray dst, ਨਿਵੇਸ਼_Array ਨਕਾਬ,
                     ਪੂਰਨ dtype, ਅਚਲ Scalar& params = Scalar());

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਅਚਲ ਅੱਖਰ* oclop2str[] = { "OP_ADD", "OP_SUB", "OP_RSUB", "OP_ABSDIFF",
    "OP_MUL", "OP_MUL_SCALE", "OP_DIV_SCALE", "OP_RECIP_SCALE",
    "OP_ADDW", "OP_AND", "OP_OR", "OP_XOR", "OP_NOT", "OP_MIN", "OP_MAX", "OP_RDIV_SCALE", 0 };

// depths with no OpenCL type name (ocl::typeToStr() asserts on them)
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ noOclType(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਵਾਪਸ ਡੂੰਘਾਈ == CV_16BF || ਡੂੰਘਾਈ == CV_Bool || ਡੂੰਘਾਈ == CV_8F_E4M3FN || ਡੂੰਘਾਈ == CV_8F_E4M3FNUZ;
}

ਸਥਿਰ ਤਾਰਕਿਕ ocl_ਦੁਆਧਾਰੀ_op(ਨਿਵੇਸ਼_Array _src1, ਨਿਵੇਸ਼_Array _src2, OutputArray _dst,
                          ਨਿਵੇਸ਼_Array ਨਕਾਬ_2, ਤਾਰਕਿਕ bitwise, ਪੂਰਨ oclop, ਤਾਰਕਿਕ haveScalar )
{
    ਤਾਰਕਿਕ have_ਨਕਾਬ = !ਨਕਾਬ_2.empty();
    ਪੂਰਨ srctype = _src1.type();
    ਪੂਰਨ srcdepth = CV_MAT_ਡੂੰਘਾਈ(srctype);
    ਪੂਰਨ cn = CV_MAT_CN(srctype);

    ਅਚਲ ocl::Device d = ocl::Device::getDefault();
    ਤਾਰਕਿਕ doubleSupport = d.doubleFPConfig() > 0;
    ਜੇ( oclop < 0 || ((have_ਨਕਾਬ || haveScalar) && cn > 4) ||
            (!doubleSupport && srcdepth == CV_64F && !bitwise) ||
            (noOclType(srcdepth) && !bitwise))
        ਵਾਪਸ ਝੂਠ;

    ਅੱਖਰ opts[1024];
    ਪੂਰਨ kercn = have_ਨਕਾਬ || haveScalar ? cn : ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(_src1, _src2, _dst);
    ਪੂਰਨ scalarcn = kercn == 3 ? 4 : kercn;
    ਪੂਰਨ ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = d.isIntel() ? 4 : 1;

    ਅਚਲ ਪੂਰਨ dst_ਡੂੰਘਾਈ = srcdepth;
    ਅਚਲ ਪੂਰਨ dstType = CV_MAKETYPE(dst_ਡੂੰਘਾਈ, kercn);
    ਅਚਲ ਪੂਰਨ dstType1 = CV_MAKETYPE(dst_ਡੂੰਘਾਈ, 1);
    ਅਚਲ ਪੂਰਨ scalarType = CV_MAKETYPE(srcdepth, scalarcn);

    snprintf(opts, ਆਕਾਰ(opts), "-D %s%s -D %s%s -D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workST=%s -D cn=%d -D rowsPerWI=%d",
            have_ਨਕਾਬ ? "MASK_" : "", haveScalar ? "UNARY_OP" : "BINARY_OP", oclop2str[oclop],
            doubleSupport ? " -D DOUBLE_SUPPORT" : "",
            bitwise ? ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(dstType) : ocl::type_ਮੰਜ਼ਿਲ_Str(dstType),
            dst_ਡੂੰਘਾਈ,
            bitwise ? ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(dstType1) : ocl::type_ਮੰਜ਼ਿਲ_Str(dstType1),
            bitwise ? ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(scalarType) : ocl::type_ਮੰਜ਼ਿਲ_Str(scalarType),
            kercn, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI);

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), ਨਕਾਬ = ਨਕਾਬ_2.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src1, cn, kercn);
    ocl::KernelArg dstarg = have_ਨਕਾਬ ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_ਮਾਪ(ਨਕਾਬ, 1);

    ਜੇ( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(srctype)*scalarcn;
        ਦੁੱਗਣਾ buf[4] = {0,0,0,0};

        ਜੇ( oclop != OCL_OP_NOT )
        {
            Mat src2sc = _src2.getMat();
            convertAndUnrollScalar(src2sc, srctype, (uchar*)buf, 1);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        ਜੇ( !have_ਨਕਾਬ )
            k.ਦਲੀਲਾਂ(src1arg, dstarg, scalararg);
        ਨਹੀਂਤਾਂ
            k.ਦਲੀਲਾਂ(src1arg, maskarg, dstarg, scalararg);
    }
    ਨਹੀਂਤਾਂ
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src2, cn, kercn);

        ਜੇ( !have_ਨਕਾਬ )
            k.ਦਲੀਲਾਂ(src1arg, src2arg, dstarg);
        ਨਹੀਂਤਾਂ
            k.ਦਲੀਲਾਂ(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.ਖਾਨੇ_ਗਿਣਤੀ * cn / kercn, ((size_t)src1.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, 0, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

// OpenCL path for the bitwise ops (and/or/xor/not), lifted out of binary_op: choose the (possibly
// scalar) operand order, size the destination, and dispatch the shared bitwise OpenCL kernel. Returns
// true if OpenCL handled the op, false to fall through to the CPU engine. NOT arrives unary
// (src2 == src1, forced-scalar). The caller gates this on a UMat operand + dims <= 2 (via CV_OCL_RUN).
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
ਸਥਿਰ ਤਾਰਕਿਕ bitwise_op_ocl(ਨਿਵੇਸ਼_Array _src1, ਨਿਵੇਸ਼_Array _src2, OutputArray _dst,
                           ਨਿਵੇਸ਼_Array ਨਕਾਬ_2, ਪੂਰਨ oclop)
{
    ਅਚਲ ਨਿਵੇਸ਼_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag kind1 = psrc1->ਕਿਸਮ(), kind2 = psrc2->ਕਿਸਮ();
    ਪੂਰਨ type1 = psrc1->type(), type2 = psrc2->type();
    ਤਾਰਕਿਕ haveScalar = ਝੂਠ;

    ਜੇ( oclop == OCL_OP_NOT )
        haveScalar = ਸੱਚ;
    ਨਹੀਂਤਾਂ ਜੇ( (kind1 == ਨਿਵੇਸ਼_Array_2::MATX) + (kind2 == ਨਿਵੇਸ਼_Array_2::MATX) == 1 ||
             !psrc1->same_ਮਾਪ(*psrc2) || type1 != type2 )
    {
        ਜੇ( checkScalar(*psrc1, type2, kind1, kind2) )
        { std::swap(psrc1, psrc2); std::swap(type1, type2); }        // src1 is the scalar; swap it out
        ਨਹੀਂਤਾਂ ਜੇ( !checkScalar(*psrc2, type1, kind2, kind1) )
            ਵਾਪਸ ਝੂਠ;
        haveScalar = ਸੱਚ;
    }
    _dst.create_Same_ਮਾਪ(*psrc1, type1);
    ਵਾਪਸ ocl_ਦੁਆਧਾਰੀ_op(*psrc1, *psrc2, _dst, ਨਕਾਬ_2, ਸੱਚ, oclop, haveScalar);
}
#ਜੇ_ਅੰਤ  // HAVE_OPENCL

}

// bitwise and/or/xor: OpenCL runs the shared bitwise kernel (via bitwise_op_ocl); the CPU path is the
// element-wise engine (OP_AND/OR/XOR, T x T -> T dispatched by element size). oclop = -1 tells
// arithm_op the OpenCL path was already tried, so it only runs the CPU engine. NOT is unary.
ਖਾਲੀ cv::bitwise_and(ਨਿਵੇਸ਼_Array a, ਨਿਵੇਸ਼_Array b, OutputArray c, ਨਿਵੇਸ਼_Array ਨਕਾਬ)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_ਲੜੀਵਾਰ((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, ਨਕਾਬ, OCL_OP_AND))
    arithm_op(ew::OP_AND, a, b, c, ਨਕਾਬ, -1, /*oclop=*/-1, /*muldiv=*/ਝੂਠ);
}

ਖਾਲੀ cv::bitwise_or(ਨਿਵੇਸ਼_Array a, ਨਿਵੇਸ਼_Array b, OutputArray c, ਨਿਵੇਸ਼_Array ਨਕਾਬ)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_ਲੜੀਵਾਰ((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, ਨਕਾਬ, OCL_OP_OR))
    arithm_op(ew::OP_OR, a, b, c, ਨਕਾਬ, -1, /*oclop=*/-1, /*muldiv=*/ਝੂਠ);
}

ਖਾਲੀ cv::bitwise_xor(ਨਿਵੇਸ਼_Array a, ਨਿਵੇਸ਼_Array b, OutputArray c, ਨਿਵੇਸ਼_Array ਨਕਾਬ)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_ਲੜੀਵਾਰ((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, ਨਕਾਬ, OCL_OP_XOR))
    arithm_op(ew::OP_XOR, a, b, c, ਨਕਾਬ, -1, /*oclop=*/-1, /*muldiv=*/ਝੂਠ);
}

ਖਾਲੀ cv::bitwise_not(ਨਿਵੇਸ਼_Array a, OutputArray c, ਨਿਵੇਸ਼_Array ਨਕਾਬ)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_ਲੜੀਵਾਰ((a.isUMat() || c.isUMat()) && a.dims() <= 2,
               bitwise_op_ocl(a, a, c, ਨਕਾਬ, OCL_OP_NOT))
    unary_op(ew::OP_NOT, a, c, ਨਕਾਬ, -1);
}

ਖਾਲੀ cv::max( ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, ਝੂਠ);
}

ਖਾਲੀ cv::min( ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, ਝੂਠ);
}

// The concrete Mat/UMat overloads of min/max exist because C++ overload resolution needs them (a
// bare cv::min(mat, mat, mat) binds Mat& more tightly than InputArray); they must run the SAME engine
// path as the InputArray forms - NOT the legacy binary_op - so mixed types and broadcasting work.
ਖਾਲੀ cv::max(ਅਚਲ Mat& src1, ਅਚਲ Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, ਝੂਠ);
}

ਖਾਲੀ cv::min(ਅਚਲ Mat& src1, ਅਚਲ Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, ਝੂਠ);
}

ਖਾਲੀ cv::max(ਅਚਲ UMat& src1, ਅਚਲ UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, ਝੂਠ);
}

ਖਾਲੀ cv::min(ਅਚਲ UMat& src1, ਅਚਲ UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, ਝੂਠ);
}


/****************************************************************************************\
*                                      add/subtract                                      *
\****************************************************************************************/

ਨਾਮਾਕਾਸ਼ cv
{

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

// used by the OpenCL branch only (arithm_op_ocl working-type selection)
ਸਥਿਰ ਪੂਰਨ actual_Scalar_ਡੂੰਘਾਈ(ਅਚਲ ਦੁੱਗਣਾ* ਅੰਕੜੇ, ਪੂਰਨ len)
{
    ਪੂਰਨ i = 0, minval = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, maxval = INT_MIN;
    ਲਈ(; i < len; ++i)
    {
        ਪੂਰਨ ival = cv_ਗੇੜਾ(ਅੰਕੜੇ[i]);
        ਜੇ( ival != ਅੰਕੜੇ[i] )
            ਰੋਕੋ;
        minval = MIN(minval, ival);
        maxval = MAX(maxval, ival);
    }
    ਵਾਪਸ i < len ? CV_64F :
        minval >= 0 && maxval <= (ਪੂਰਨ)UCHAR_MAX ? CV_8U :
        minval >= (ਪੂਰਨ)SCHAR_MIN && maxval <= (ਪੂਰਨ)SCHAR_MAX ? CV_8S :
        minval >= 0 && maxval <= (ਪੂਰਨ)USHRT_MAX ? CV_16U :
        minval >= (ਪੂਰਨ)SHRT_MIN && maxval <= (ਪੂਰਨ)SHRT_MAX ? CV_16S :
        CV_32S;
}

ਸਥਿਰ ਪੂਰਨ coerceTypes(ਪੂਰਨ depth1, ਪੂਰਨ depth2, ਤਾਰਕਿਕ muldiv)
{
    ਵਾਪਸ depth1 == depth2 ? depth1 :
        ((depth1 <= CV_32S) & (depth2 <= CV_32S)) != 0 ?
        (((ਪੂਰਨ)!muldiv & (depth1 <= CV_8S) & (depth2 <= CV_8S)) != 0 ? CV_16S : CV_32S) :
        ((CV_ELEM_SIZE1(depth1) > 4) | (CV_ELEM_SIZE1(depth2) > 4)) != 0 ? CV_64F : CV_32F;
}

ਸਥਿਰ ਤਾਰਕਿਕ ocl_arithm_op(ਨਿਵੇਸ਼_Array _src1, ਨਿਵੇਸ਼_Array _src2, OutputArray _dst,
                          ਨਿਵੇਸ਼_Array ਨਕਾਬ_2, ਪੂਰਨ wtype,
                          ਖਾਲੀ* usrdata, ਪੂਰਨ oclop,
                          ਤਾਰਕਿਕ haveScalar )
{
    ਅਚਲ ocl::Device d = ocl::Device::getDefault();
    ਤਾਰਕਿਕ doubleSupport = d.doubleFPConfig() > 0;
    ਪੂਰਨ type1 = _src1.type(), depth1 = CV_MAT_ਡੂੰਘਾਈ(type1), cn = CV_MAT_CN(type1);
    ਤਾਰਕਿਕ have_ਨਕਾਬ = !ਨਕਾਬ_2.empty();

    ਜੇ ( (have_ਨਕਾਬ || haveScalar) && cn > 4 )
        ਵਾਪਸ ਝੂਠ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __APPLE__
    ਜੇ (depth1 == CV_16U && (oclop == OCL_OP_MUL || oclop == OCL_OP_MUL_SCALE))
        ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ

    ਪੂਰਨ dtype = _dst.type(), ddepth = CV_MAT_ਡੂੰਘਾਈ(dtype), wdepth = std::max(CV_32S, CV_MAT_ਡੂੰਘਾਈ(wtype));
    ਜੇ (!doubleSupport)
        wdepth = std::min(wdepth, CV_32F);

    wtype = CV_MAKETYPE(wdepth, cn);
    ਪੂਰਨ type2 = haveScalar ? wtype : _src2.type(), depth2 = CV_MAT_ਡੂੰਘਾਈ(type2);
    ਜੇ (!doubleSupport && (depth2 == CV_64F || depth1 == CV_64F))
        ਵਾਪਸ ਝੂਠ;

    ਜੇ (noOclType(depth1) || noOclType(depth2) || noOclType(ddepth) || noOclType(wdepth))
        ਵਾਪਸ ਝੂਠ;

    ਪੂਰਨ kercn = have_ਨਕਾਬ || haveScalar ? cn : ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(_src1, _src2, _dst);
    ਪੂਰਨ scalarcn = kercn == 3 ? 4 : kercn, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = d.isIntel() ? 4 : 1;

    ਅੱਖਰ cvtstr[4][50], opts[1024];
    snprintf(opts, ਆਕਾਰ(opts), "-D %s%s -D %s -D srcT1=%s -D srcT1_C1=%s -D srcT2=%s -D srcT2_C1=%s "
            "-D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workT=%s -D workST=%s -D scaleT=%s -D wdepth=%d -D convertToWT1=%s "
            "-D convertToWT2=%s -D convertToDT=%s%s -D cn=%d -D rowsPerWI=%d -D convertFromU=%s",
            (have_ਨਕਾਬ ? "MASK_" : ""), (haveScalar ? "UNARY_OP" : "BINARY_OP"),
            oclop2str[oclop], ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKETYPE(depth1, kercn)),
            ocl::type_ਮੰਜ਼ਿਲ_Str(depth1), ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKETYPE(depth2, kercn)),
            ocl::type_ਮੰਜ਼ਿਲ_Str(depth2), ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKETYPE(ddepth, kercn)), ddepth,
            ocl::type_ਮੰਜ਼ਿਲ_Str(ddepth), ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKETYPE(wdepth, kercn)),
            ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKETYPE(wdepth, scalarcn)),
            ocl::type_ਮੰਜ਼ਿਲ_Str(wdepth), wdepth,
            ocl::convertTypeStr(depth1, wdepth, kercn, cvtstr[0], ਆਕਾਰ(cvtstr[0])),
            ocl::convertTypeStr(depth2, wdepth, kercn, cvtstr[1], ਆਕਾਰ(cvtstr[1])),
            ocl::convertTypeStr(wdepth, ddepth, kercn, cvtstr[2], ਆਕਾਰ(cvtstr[2])),
            doubleSupport ? " -D DOUBLE_SUPPORT" : "", kercn, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI,
            oclop == OCL_OP_ABSDIFF && wdepth == CV_32S && ddepth == wdepth ?
            ocl::convertTypeStr(CV_8U, ddepth, kercn, cvtstr[3], ਆਕਾਰ(cvtstr[3])) : "noconvert");

    size_t usrdata_esz = CV_ELEM_ਮਾਪ(wdepth);
    ਅਚਲ uchar* usrdata_p = (ਅਚਲ uchar*)usrdata;
    ਅਚਲ ਦੁੱਗਣਾ* usrdata_d = (ਅਚਲ ਦੁੱਗਣਾ*)usrdata;
    ਦਸ਼ਮਲਵ usrdata_f[3];
    ਪੂਰਨ i, n = oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE ||
        oclop == OCL_OP_RDIV_SCALE || oclop == OCL_OP_RECIP_SCALE ? 1 : oclop == OCL_OP_ADDW ? 3 : 0;
    ਜੇ( usrdata && n > 0 && wdepth == CV_32F )
    {
        ਲਈ( i = 0; i < n; i++ )
            usrdata_f[i] = (ਦਸ਼ਮਲਵ)usrdata_d[i];
        usrdata_p = (ਅਚਲ uchar*)usrdata_f;
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), ਨਕਾਬ = ਨਕਾਬ_2.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src1, cn, kercn);
    ocl::KernelArg dstarg = have_ਨਕਾਬ ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_ਮਾਪ(ਨਕਾਬ, 1);

    ਜੇ( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(wtype)*scalarcn;
        ਦੁੱਗਣਾ buf[4]={0,0,0,0};
        Mat src2sc = _src2.getMat();

        ਜੇ( !src2sc.empty() )
            convertAndUnrollScalar(src2sc, wtype, (uchar*)buf, 1);
        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        ਜੇ( !have_ਨਕਾਬ )
        {
            ਜੇ(n == 0)
                k.ਦਲੀਲਾਂ(src1arg, dstarg, scalararg);
            ਨਹੀਂਤਾਂ ਜੇ(n == 1)
                k.ਦਲੀਲਾਂ(src1arg, dstarg, scalararg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            ਨਹੀਂਤਾਂ
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        ਨਹੀਂਤਾਂ
            k.ਦਲੀਲਾਂ(src1arg, maskarg, dstarg, scalararg);
    }
    ਨਹੀਂਤਾਂ
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src2, cn, kercn);

        ਜੇ( !have_ਨਕਾਬ )
        {
            ਜੇ (n == 0)
                k.ਦਲੀਲਾਂ(src1arg, src2arg, dstarg);
            ਨਹੀਂਤਾਂ ਜੇ (n == 1)
                k.ਦਲੀਲਾਂ(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            ਨਹੀਂਤਾਂ ਜੇ (n == 3)
                k.ਦਲੀਲਾਂ(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz*2, usrdata_esz));
            ਨਹੀਂਤਾਂ
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        ਨਹੀਂਤਾਂ
            k.ਦਲੀਲਾਂ(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.ਖਾਨੇ_ਗਿਣਤੀ * cn / kercn, ((size_t)src1.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

// OpenCL branch of the arithmetic ops, extracted from the former arithm_op (whose CPU compute is now
// the element-wise engine). Returns true if an OpenCL kernel handled the op, false to let the caller
// fall through to the CPU engine. min/max use the bitwise-style kernel; every other op the arithm
// kernel with a coerced working type. `usrdata` carries the mul/div/addWeighted scale block, `oclop`
// the kernel id. Only reached for a UMat dst; sizes/types that don't fit the scalar/same-size pattern
// (e.g. a broadcast) decline (return false) so the engine handles them on the CPU.
ਸਥਿਰ ਤਾਰਕਿਕ arithm_op_ocl(ਨਿਵੇਸ਼_Array _src1, ਨਿਵੇਸ਼_Array _src2, OutputArray _dst,
                          ਨਿਵੇਸ਼_Array ਨਕਾਬ_2, ਪੂਰਨ dtype, ਪੂਰਨ oclop, ਤਾਰਕਿਕ muldiv, ਖਾਲੀ* usrdata)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    CV_ਅਣਵਰਤਿਆ(_src1); CV_ਅਣਵਰਤਿਆ(_src2); CV_ਅਣਵਰਤਿਆ(_dst); CV_ਅਣਵਰਤਿਆ(ਨਕਾਬ_2);
    CV_ਅਣਵਰਤਿਆ(dtype); CV_ਅਣਵਰਤਿਆ(oclop); CV_ਅਣਵਰਤਿਆ(muldiv); CV_ਅਣਵਰਤਿਆ(usrdata);
    ਵਾਪਸ ਝੂਠ;
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਨਿਵੇਸ਼_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag kind1 = psrc1->ਕਿਸਮ(), kind2 = psrc2->ਕਿਸਮ();
    ਤਾਰਕਿਕ have_ਨਕਾਬ = !ਨਕਾਬ_2.empty();
    ਤਾਰਕਿਕ reallocate = ਝੂਠ;
    ਪੂਰਨ type1 = psrc1->type(), depth1 = CV_MAT_ਡੂੰਘਾਈ(type1), cn = CV_MAT_CN(type1);
    ਪੂਰਨ type2 = psrc2->type(), depth2 = CV_MAT_ਡੂੰਘਾਈ(type2), cn2 = CV_MAT_CN(type2);
    ਪੂਰਨ wtype, dims1 = psrc1->dims(), dims2 = psrc2->dims();
    ਮਾਪ sz1 = dims1 <= 2 ? psrc1->size() : ਮਾਪ();
    ਮਾਪ sz2 = dims2 <= 2 ? psrc2->size() : ਮਾਪ();

    ਜੇ (!(OCL_PERFORMANCE_CHECK(_dst.isUMat()) && dims1 <= 2 && dims2 <= 2))
        ਵਾਪਸ ਝੂਠ;

    // min/max: the bitwise-style OpenCL kernel (same-type element-wise, no working-type coercion).
    ਜੇ (oclop == OCL_OP_MIN || oclop == OCL_OP_MAX)
    {
        ਤਾਰਕਿਕ haveScalar = ਝੂਠ;
        ਜੇ ((kind1 == ਨਿਵੇਸ਼_Array_2::MATX) + (kind2 == ਨਿਵੇਸ਼_Array_2::MATX) == 1 ||
            !psrc1->same_ਮਾਪ(*psrc2) || type1 != type2)
        {
            ਜੇ (checkScalar(*psrc1, type2, kind1, kind2))
            { std::swap(psrc1, psrc2); std::swap(type1, type2); }
            ਨਹੀਂਤਾਂ ਜੇ (!checkScalar(*psrc2, type1, kind2, kind1))
                ਵਾਪਸ ਝੂਠ;
            haveScalar = ਸੱਚ;
        }
        _dst.create_Same_ਮਾਪ(*psrc1, type1);
        ਵਾਪਸ ocl_ਦੁਆਧਾਰੀ_op(*psrc1, *psrc2, _dst, ਨਕਾਬ_2, ਝੂਠ, oclop, haveScalar);
    }

    // reciprocal (scale/src): the OpenCL kernel is unary on src - use src2 for both operands (the CPU
    // engine received a 0-dim `1` numerator as src1, which the recip kernel would ignore). [TODO.VP: tidy]
    ਜੇ (oclop == OCL_OP_RECIP_SCALE)
    {
        psrc1 = psrc2; kind1 = kind2; type1 = type2; depth1 = depth2; cn = cn2; dims1 = dims2; sz1 = sz2;
    }

    // add/sub/mul/div/absdiff/addWeighted/recip: the arithm OpenCL kernel with a coerced work type.
    ਤਾਰਕਿਕ src1Scalar = checkScalar(*psrc1, type2, kind1, kind2);
    ਤਾਰਕਿਕ src2Scalar = checkScalar(*psrc2, type1, kind2, kind1);
    ਤਾਰਕਿਕ haveScalar = ਝੂਠ;

    ਜੇ (dims1 != dims2 || sz1 != sz2 || cn != cn2 ||
        (kind1 == ਨਿਵੇਸ਼_Array_2::MATX && (sz1 == ਮਾਪ(1,4) || sz1 == ਮਾਪ(1,1))) ||
        (kind2 == ਨਿਵੇਸ਼_Array_2::MATX && (sz2 == ਮਾਪ(1,4) || sz2 == ਮਾਪ(1,1))))
    {
        ਜੇ ((type1 == CV_64F && (sz1.ਉਚਾਈ == 1 || sz1.ਉਚਾਈ == 4)) && src1Scalar)
        {
            // src1 is a scalar; swap it with src2
            std::swap(psrc1, psrc2); std::swap(sz1, sz2); std::swap(type1, type2);
            std::swap(depth1, depth2); std::swap(cn, cn2); std::swap(dims1, dims2);
            ਜੇ (oclop == OCL_OP_SUB) oclop = OCL_OP_RSUB;
            ਜੇ (oclop == OCL_OP_DIV_SCALE) oclop = OCL_OP_RDIV_SCALE;
        }
        ਨਹੀਂਤਾਂ ਜੇ (!src2Scalar)
            ਵਾਪਸ ਝੂਠ;   // array op array with mismatched size/cn: engine broadcasts on the CPU
        haveScalar = ਸੱਚ;
        CV_Assert((type2 == CV_64F || type2 == CV_32F) && (sz2.ਉਚਾਈ == 1 || sz2.ਉਚਾਈ == 4));

        ਜੇ (!muldiv)
        {
            Mat sc = psrc2->getMat();
            depth2 = actual_Scalar_ਡੂੰਘਾਈ(sc.ptr<ਦੁੱਗਣਾ>(), sz2 == ਮਾਪ(1, 1) ? cn2 : cn);
            ਜੇ (depth2 == CV_64F && CV_ELEM_SIZE1(depth1) < 8)
                depth2 = CV_32F;
        }
        ਨਹੀਂਤਾਂ
            depth2 = CV_64F;
    }

    ਜੇ (dtype < 0)
    {
        ਜੇ (_dst.fixedType())
            dtype = _dst.type();
        ਨਹੀਂਤਾਂ
        {
            ਜੇ (!haveScalar && type1 != type2)
                CV_Error(cv::Error::StsBadArg,
                     "When the input arrays in add/subtract/multiply/divide functions have different types, "
                     "the output array type must be explicitly specified");
            dtype = type1;
        }
    }
    dtype = CV_MAT_ਡੂੰਘਾਈ(dtype);

    ਜੇ (depth1 == depth2 && dtype == depth1)
        wtype = dtype;
    ਨਹੀਂਤਾਂ ਜੇ (!muldiv)
    {
        wtype = coerceTypes(depth1, depth2, ਝੂਠ);
        wtype = coerceTypes(wtype, dtype, ਝੂਠ);
        ਜੇ (dtype < CV_32F && (depth1 < CV_32F || depth2 < CV_32F))
            wtype = CV_32S;
    }
    ਨਹੀਂਤਾਂ
    {
        wtype = coerceTypes(depth1, depth2, ਸੱਚ);
        wtype = coerceTypes(wtype, dtype, ਸੱਚ);
    }

    // The scaled OpenCL kernels (mul/div by a scale, reciprocal, addWeighted) compute in float; the old
    // same-type fast path forced max(depth, CV_32F). coerceTypes keeps same-type integer, so bump here.
    ਜੇ (oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE || oclop == OCL_OP_RDIV_SCALE ||
        oclop == OCL_OP_RECIP_SCALE || oclop == OCL_OP_ADDW)
        wtype = std::max(wtype, (ਪੂਰਨ)CV_32F);

    dtype = CV_MAKETYPE(dtype, cn);
    wtype = CV_MAKETYPE(wtype, cn);

    ਜੇ (have_ਨਕਾਬ)
    {
        ਪੂਰਨ mtype = ਨਕਾਬ_2.type();
        CV_Assert((mtype == CV_8UC1 || mtype == CV_8SC1 || mtype == CV_Bool) && ਨਕਾਬ_2.same_ਮਾਪ(*psrc1));
        reallocate = !_dst.same_ਮਾਪ(*psrc1) || _dst.type() != dtype;
    }

    _dst.create_Same_ਮਾਪ(*psrc1, dtype);
    ਜੇ (reallocate)
        _dst.ਸੈੱਟ_ਮੰਜ਼ਿਲ(0.);

    ਵਾਪਸ ocl_arithm_op(*psrc1, *psrc2, _dst, ਨਕਾਬ_2, wtype, usrdata, oclop, haveScalar);
#ਜੇ_ਅੰਤ
}

// Element-wise binary op (ADD/SUB/...) on two CPU operands - each an array Mat or a MATX scalar -
// through the new broadcasting engine. Handles mixed input types (emitBinary inserts the casts) and
// broadcasting (a scalar rides as a 0-dim per-channel CONST; arrays broadcast via outputShape/exec).
// Returns false (declining) for UMat, a write-mask, or two scalar operands, so the caller falls
// through to arithm_op. Incompatible shapes make the engine throw (matching cv::add's error).
// Does the output array already have exactly this shape+type (=> create() reuses it, no realloc)?
// Works for Mat and UMat alike (shape/type queries, no data-pointer peeking).
ਸਥਿਰ ਤਾਰਕਿਕ ਬਾਹਰਲਾ_Array_Matches(ਅਚਲ _OutputArray& a, ਅਚਲ MatShape& shp, ਪੂਰਨ type)
{
    ਜੇ (a.empty() || a.type() != type)
        ਵਾਪਸ ਝੂਠ;
    ਪੂਰਨ sz[MatShape::MAX_DIMS];
    ਪੂਰਨ nd = a.sizend(sz);
    ਜੇ (nd != (ਪੂਰਨ)shp.size())
        ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਪੂਰਨ i = 0; i < nd; i++)
        ਜੇ (sz[i] != shp[i])
            ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
}

// Are two operands' iteration shapes (spatial dims + channels innermost, exactly how the engine
// iterates) numpy-broadcast-compatible? Powers the scalar-like-Mat compat fallback in arithm_op.
// Cheap by design: sizend copies the dim arrays inline - no Mat headers, no allocations.
ਸਥਿਰ ਤਾਰਕਿਕ shapes_ਪ੍ਰਸਾਰਣ_Compat(ਨਿਵੇਸ਼_Array a, ਪੂਰਨ acn, ਨਿਵੇਸ਼_Array b, ਪੂਰਨ bcn)
{
    ਪੂਰਨ asz[MatShape::MAX_DIMS + 1], bsz[MatShape::MAX_DIMS + 1];
    ਪੂਰਨ ad = a.sizend(asz), bd = b.sizend(bsz);
    asz[ad++] = acn;
    bsz[bd++] = bcn;
    ਲਈ (ਪੂਰਨ i1 = ad - 1, i2 = bd - 1; i1 >= 0 && i2 >= 0; i1--, i2--)
        ਜੇ (asz[i1] != bsz[i2] && asz[i1] != 1 && bsz[i2] != 1)
            ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
}

// The one entry point for every element-wise binary op (add/sub/mul/div/min/max/absdiff/addWeighted/
// reciprocal). Handles empty inputs, the UMat/OpenCL branch (arithm_op_ocl) and the CPU element-wise
// engine. `oclop` selects the OpenCL kernel; `muldiv` drives the OpenCL working-type coercion + scale;
// `params` are the op scalars (params[0]=mul/div scale; {alpha,beta,gamma} for addWeighted).
// Below this many ELEMENTS (total * channels), the per-call constant overhead of the engine path
// (program build + compile + executor setup) is comparable to the actual work - take the direct
// kernel call instead. Same tier boundary as math_op's MATH_OP_SMALL for cv::exp.
ਗਿਣਤੀ { ARITHM_SMALL_ਸਿੱਧਾ = 100000 };

ਸਥਿਰ ਖਾਲੀ arithm_op(ew::ਸਿਖਰ op, ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, OutputArray dst,
                      ਨਿਵੇਸ਼_Array ਨਕਾਬ, ਪੂਰਨ dtype, ਪੂਰਨ oclop, ਤਾਰਕਿਕ muldiv, ਅਚਲ Scalar& params)
{
    ਜੇ (src1.empty() || src2.empty())          // empty inputs -> empty result
    {
        CV_Assert((src1.empty() && src2.empty()) ||
                  (src1.empty() && isScalarArg(src2, src1.channels())) ||
                  (src2.empty() && isScalarArg(src1, src2.channels())));
        dst.release();
        ਜੇ (dtype >= 0)
            dst.create(0, 0, dtype);
        ਵਾਪਸ;
    }

    // UMat -> classic OpenCL kernel. It declines (false) when OpenCL can't apply (no device, dims>2, a
    // broadcast, cn>4 masked, ...) - then the CPU engine below handles it (mapping the UMat via getMat).
    // oclop < 0 is a sentinel: the caller (e.g. cv::compare) already ran its own OpenCL path, so this
    // helper only runs the CPU engine (a UMat operand is mapped via getMat).
    ਜੇ (oclop >= 0 && cv::ocl::isOpenCLActivated() &&
        (src1.isUMat() || src2.isUMat() || dst.isUMat()))
    {
        ਦੁੱਗਣਾ abg[3] = { params[0], params[1], params[2] }, scale = params[0];
        ਖਾਲੀ* usrdata = (op == ew::OP_ADDW) ? (ਖਾਲੀ*)abg : (muldiv ? (ਖਾਲੀ*)&scale : ਖਾਲੀ_ਸੂਚਕ);
        ਜੇ (arithm_op_ocl(src1, src2, dst, ਨਕਾਬ, dtype, oclop, muldiv, usrdata))
            ਵਾਪਸ;
    }

    ਅਚਲ ਤਾਰਕਿਕ have_ਨਕਾਬ = !ਨਕਾਬ.empty();
    ਅਚਲ ਪੂਰਨ cn1 = src1.channels(), cn2 = src2.channels();
    ਤਾਰਕਿਕ s1 = isScalarArg(src1, src2), s2 = isScalarArg(src2, src1);
    ਜੇ (s1 && s2)
        s1 = s2 = ਝੂਠ;    // two scalars: treat both as tiny array operands (element-wise + broadcast),
                            // matching arithm_op (Scalar+Scalar -> 4x1, number+number -> 1x1, ...)
    // A 4x1 CV_64F Mat/UMat pseudo-scalar (see isScalarArg) is hijacked only against a REAL array:
    // when the partner is itself a tiny scalar-shaped array (e.g. compare(Mat 4x1, Mat 1x1) -
    // issue #8999), both are honest data and ride the broadcast. A genuine MATX Scalar never demotes.
    ਨਹੀਂਤਾਂ ਜੇ (s1 && src1.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::MATX && isScalarLikeMat(src2, cn1))
        s1 = ਝੂਠ;
    ਨਹੀਂਤਾਂ ਜੇ (s2 && src2.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::MATX && isScalarLikeMat(src1, cn2))
        s2 = ਝੂਠ;

    // Compat fallback: Java/Python/user code passes scalars as real little Mats (the classic 4x1
    // CV_64F column, a 1xcn/cnx1 vector - see isScalarLikeMat). Those normally ride broadcasting
    // now, so hijack one as a per-channel scalar ONLY when the shapes do NOT broadcast - just the
    // calls that would otherwise throw get the 4.x scalar semantics, every valid broadcast keeps
    // its numpy meaning. Cheap: two geometry probes, and the O(ndims) shape walk runs only when a
    // probe hits (never for ordinary same-size or MATX-scalar calls).
    ਜੇ (!s1 && !s2)
    {
        ਤਾਰਕਿਕ like1 = isScalarLikeMat(src1, cn2), like2 = isScalarLikeMat(src2, cn1);
        ਜੇ ((like1 || like2) && !shapes_ਪ੍ਰਸਾਰਣ_Compat(src1, cn1, src2, cn2))
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
    ਅਚਲ ਖਾਲੀ* dstObj = (dst.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::MAT) ? dst.getObj() : ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ ਤਾਰਕਿਕ aliased = dstObj && (dstObj == src1.getObj() || dstObj == src2.getObj());
    Mat m1loc, m2loc, mloc;
    ਸਵੈਚਾਲਿਤ asMat = [&](ਨਿਵੇਸ਼_Array a, Mat& loc) -> ਅਚਲ Mat* {
        ਜੇ (a.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::MAT && !aliased) ਵਾਪਸ (ਅਚਲ Mat*)a.getObj();
        loc = a.getMat(); ਵਾਪਸ &loc;
    };
    ਅਚਲ Mat* pm1 = s1 ? ਖਾਲੀ_ਸੂਚਕ : asMat(src1, m1loc);
    ਅਚਲ Mat* pm2 = s2 ? ਖਾਲੀ_ਸੂਚਕ : asMat(src2, m2loc);
    ਅਚਲ ਪੂਰਨ adepth = s1 ? pm2->ਡੂੰਘਾਈ() : pm1->ਡੂੰਘਾਈ();   // the (first) array operand's depth
    // auto result depth (no explicit dtype, no fixed-type dst): add/sub/mul/div keep the first array
    // operand's depth (cv::'s dtype==-1 convention), but min/max/absdiff have NO dtype argument and
    // must promote mixed inputs to their common type (min(u32,f32) in u32 would drop the float). For
    // same-type inputs promoteArith(da,db)==da, so classic behaviour is preserved (e.g. absdiff of two
    // s16 stays s16, saturating - the wide unsigned |a-b| is computed then cast back down by emitBinary).
    ਪੂਰਨ auto_ਡੂੰਘਾਈ = adepth;
    ਜੇ (op == ew::OP_MIN || op == ew::OP_MAX || op == ew::OP_ABSDIFF)
    {
        ਅਚਲ ਪੂਰਨ d1 = s1 ? ew::EW_ਡੂੰਘਾਈ_NONE : pm1->ਡੂੰਘਾਈ();
        ਅਚਲ ਪੂਰਨ d2 = s2 ? ew::EW_ਡੂੰਘਾਈ_NONE : pm2->ਡੂੰਘਾਈ();
        auto_ਡੂੰਘਾਈ = ew::promoteArith(d1, d2);
    }
    // result depth: explicit dtype wins; else a fixed-type dst dictates it; else the auto depth above.
    ਅਚਲ ਪੂਰਨ rdepth = dtype >= 0 ? CV_MAT_ਡੂੰਘਾਈ(dtype)
                     : (dst.fixedType() ? dst.ਡੂੰਘਾਈ() : auto_ਡੂੰਘਾਈ);

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
    ਜੇ (!have_ਨਕਾਬ && !s1 && !s2 &&
        (op == ew::OP_ਜੋੜੋ || op == ew::OP_SUB || op == ew::OP_MIN || op == ew::OP_MAX ||
         op == ew::OP_ABSDIFF || op == ew::OP_MUL || op == ew::OP_ADDW ||
         op == ew::OP_AND || op == ew::OP_OR || op == ew::OP_XOR) &&
        rdepth == pm1->ਡੂੰਘਾਈ() && pm1->type() == pm2->type() && pm1->size == pm2->size &&
        pm1->isContinuous() && pm2->isContinuous() &&
        pm1->ਕੁੱਲ()*cn1 <= (size_t)ARITHM_SMALL_ਸਿੱਧਾ)
    {
        ew::TKernel k = ew::getElemwiseFunc(op, rdepth, rdepth, ew::EW_ਡੂੰਘਾਈ_NONE, rdepth);
        ਜੇ (k.fptr)
        {
            dst.create_Same_ਮਾਪ(src1, pm1->type());   // whole-shape transfer, not piecemeal dims+sizes
            Mat dloc;
            Mat* pd = (dst.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::MAT) ? (Mat*)dst.getObj() : &(dloc = dst.getMat());
            ਜੇ (pd->isContinuous())     // a reused non-continuous dst view falls through to the engine
            {
                k.fptr(pm1->ਅੰਕੜੇ, 0, 1, pm2->ਅੰਕੜੇ, 0, 1, ਖਾਲੀ_ਸੂਚਕ, 0, 0,
                       pd->ਅੰਕੜੇ, 0, (ਪੂਰਨ)(pm1->ਕੁੱਲ()*cn1), 1, params.val, k.flags, k.userdata);
                ਵਾਪਸ;
            }
        }
    }

    // operand order = (src1, src2): an array becomes an INPUT, a scalar a flexible per-channel CONST
    // read straight from the caller's inline storage (getObj()), no Mat / convertTo.
    ew::TExpr p;
    ਸਵੈਚਾਲਿਤ ਜੋੜੋ_ਸੰਕਾਰਜ = [&](ਨਿਵੇਸ਼_Array src, ਤਾਰਕਿਕ isScalar, ਅਚਲ Mat* m, ਪੂਰਨ otherCn) -> ਪੂਰਨ {
        ਜੇ (!isScalar)
            ਵਾਪਸ p.ਜੋੜੋ_ਨਿਵੇਸ਼(m->ਡੂੰਘਾਈ());
        ਅਚਲ uchar* sp; ਪੂਰਨ sd;
        uchar scbuf[EW_SCALAR_BUF_ਮਾਪ];   // stack room for a UMAT scalar; addConst copies the values out
        ਅਚਲ ਪੂਰਨ scn0 = scalarArgElems(src, sp, sd, scbuf), cn = otherCn;
        ਅਚਲ ਪੂਰਨ scn = (cn == scn0 || (cn < 4 && scn0 == 4)) ? cn
                      : (scn0 == 1) ? 1 : 0;
        CV_Assert(scn > 0);
        ਵਾਪਸ p.ਜੋੜੋ_Const(ew::EW_ਡੂੰਘਾਈ_NONE, sd, sp, scn);
    };
    ਅਚਲ ਪੂਰਨ a0 = ਜੋੜੋ_ਸੰਕਾਰਜ(src1, s1, pm1, cn2);
    ਅਚਲ ਪੂਰਨ a1 = ਜੋੜੋ_ਸੰਕਾਰਜ(src2, s2, pm2, cn1);

    // Write-mask: a single-channel 1-byte array (u8/s8/bool) the size of the output spatial shape,
    // added as another broadcast input. The arithmetic result lands in a temp; a final
    // select(mask, r, dst) -> dst overwrites only mask!=0 positions of the (pre-existing) output.
    ਪੂਰਨ s_ਨਕਾਬ = 0;
    ਅਚਲ Mat* pmask = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (have_ਨਕਾਬ)
    {
        CV_Assert(ਨਕਾਬ.type() == CV_8U || ਨਕਾਬ.type() == CV_8S || ਨਕਾਬ.type() == CV_Bool);
        pmask = asMat(ਨਕਾਬ, mloc);
        s_ਨਕਾਬ = p.ਜੋੜੋ_ਨਿਵੇਸ਼(pmask->ਡੂੰਘਾਈ());
    }
    ਅਚਲ ਪੂਰਨ s_ਬਾਹਰਲਾ = p.ਜੋੜੋ_Output(rdepth);               // output BEFORE emit -> moveToOutput drops the
    ਅਚਲ ਪੂਰਨ r = p.emit_ਦੁਆਧਾਰੀ(op, a0, a1, rdepth, params);   // result temp (mask-free => none); scale
                                                        // rides params[0] (mul/div), {a,b,g} for addW
    ਜੇ (have_ਨਕਾਬ)
        p.ਜੋੜੋ_Insn(ew::OP_SELECT, s_ਨਕਾਬ, r, s_ਬਾਹਰਲਾ, s_ਬਾਹਰਲਾ);
    ਨਹੀਂਤਾਂ
        p.move_ਮੰਜ਼ਿਲ_Output(r, s_ਬਾਹਰਲਾ);
    p.compile();

    ਅਚਲ Mat* inputs[3]; ਪੂਰਨ ni = 0;                   // inputs in program (input-index) order
    ਜੇ (!s1) inputs[ni++] = pm1;
    ਜੇ (!s2) inputs[ni++] = pm2;
    ਜੇ (have_ਨਕਾਬ) inputs[ni++] = pmask;

    // Pre-create the output at the broadcast shape, ALWAYS via dst.create() - never hand exec a raw
    // dst Mat. Routing through the _OutputArray is what enforces its contract: a FIXED_SIZE/FIXED_TYPE
    // dst (in-place 'a += b' passes '(const Mat&)a') throws here if a broadcast would change its shape
    // or type (Mat(1x1) += Mat(1x4)), instead of silently reallocating - exec sees only a Mat and has
    // no view of the array's fixed flags. Cost over a direct exec is negligible: exec keeps its own
    // fast path for a matching-shape op, and dst.create() with an unchanged shape/type is a no-op.
    // With a mask, a freshly (re)allocated dst is zeroed so mask==0 reads 0 (matches arithm_op); a
    // reused dst keeps its prior content there - detect reuse by shape+type BEFORE create (correct for
    // a reused UMat dst too).
    MatShape oshape; ਪੂਰਨ ocn;
    p.outputShape(inputs, oshape, ocn);
    ਅਚਲ ਪੂਰਨ otype = CV_MAKETYPE(rdepth, ocn);
    ਅਚਲ ਤਾਰਕਿਕ reused = have_ਨਕਾਬ && ਬਾਹਰਲਾ_Array_Matches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    ਜੇ (have_ਨਕਾਬ && !reused)
        pdst->ਸੈੱਟ_Zero();                          // setZero (not setTo, which caps at 4 channels)
    p.exec(inputs, pdst);
}

// Unary element-wise op through the engine (bitwise NOT now; unary math later). One array input, an
// optional write-mask; result depth = dtype (>=0), else a fixed-type dst's depth, else the input
// depth. Mirrors arithm_op's build/mask/exec tail with a single input and no scalar operand. OpenCL is
// run by the caller (bitwise_op_ocl), so this is the CPU engine only.
ਸਥਿਰ ਖਾਲੀ unary_op(ew::ਸਿਖਰ op, ਨਿਵੇਸ਼_Array src, OutputArray dst, ਨਿਵੇਸ਼_Array ਨਕਾਬ,
                     ਪੂਰਨ dtype, ਅਚਲ Scalar& params)
{
    ਜੇ (src.empty())
    {
        dst.release();
        ਜੇ (dtype >= 0)
            dst.create(0, 0, dtype);
        ਵਾਪਸ;
    }

    ਅਚਲ ਤਾਰਕਿਕ have_ਨਕਾਬ = !ਨਕਾਬ.empty();
    // in-place (dst aliases src): keep a header copy so a shape/type-changing dst.create() can't free
    // src's data mid-op (matches arithm_op). A fixed-size/type dst never reallocs, so this is only for
    // an explicit dtype change.
    ਅਚਲ ਖਾਲੀ* dstObj = (dst.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::MAT) ? dst.getObj() : ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ ਤਾਰਕਿਕ aliased = dstObj && src.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::MAT && dstObj == src.getObj();
    Mat sloc, mloc;
    ਅਚਲ Mat* psrc = (src.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::MAT && !aliased) ? (ਅਚਲ Mat*)src.getObj()
                                                                   : &(sloc = src.getMat());
    ਅਚਲ ਪੂਰਨ rdepth = dtype >= 0 ? CV_MAT_ਡੂੰਘਾਈ(dtype)
                     : (dst.fixedType() ? dst.ਡੂੰਘਾਈ() : psrc->ਡੂੰਘਾਈ());

    ew::TExpr p;
    ਅਚਲ ਪੂਰਨ a0 = p.ਜੋੜੋ_ਨਿਵੇਸ਼(psrc->ਡੂੰਘਾਈ());
    ਪੂਰਨ s_ਨਕਾਬ = 0;
    ਅਚਲ Mat* pmask = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (have_ਨਕਾਬ)
    {
        CV_Assert(ਨਕਾਬ.type() == CV_8U || ਨਕਾਬ.type() == CV_8S || ਨਕਾਬ.type() == CV_Bool);
        pmask = (ਨਕਾਬ.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::MAT) ? (ਅਚਲ Mat*)ਨਕਾਬ.getObj() : &(mloc = ਨਕਾਬ.getMat());
        s_ਨਕਾਬ = p.ਜੋੜੋ_ਨਿਵੇਸ਼(pmask->ਡੂੰਘਾਈ());
    }
    ਅਚਲ ਪੂਰਨ s_ਬਾਹਰਲਾ = p.ਜੋੜੋ_Output(rdepth);
    ਅਚਲ ਪੂਰਨ r = p.emitUnary(op, a0, rdepth, params);
    ਜੇ (have_ਨਕਾਬ)
        p.ਜੋੜੋ_Insn(ew::OP_SELECT, s_ਨਕਾਬ, r, s_ਬਾਹਰਲਾ, s_ਬਾਹਰਲਾ);
    ਨਹੀਂਤਾਂ
        p.move_ਮੰਜ਼ਿਲ_Output(r, s_ਬਾਹਰਲਾ);
    p.compile();

    ਅਚਲ Mat* inputs[2]; ਪੂਰਨ ni = 0;
    inputs[ni++] = psrc;
    ਜੇ (have_ਨਕਾਬ) inputs[ni++] = pmask;

    MatShape oshape; ਪੂਰਨ ocn;
    p.outputShape(inputs, oshape, ocn);
    ਅਚਲ ਪੂਰਨ otype = CV_MAKETYPE(rdepth, ocn);
    ਅਚਲ ਤਾਰਕਿਕ reused = have_ਨਕਾਬ && ਬਾਹਰਲਾ_Array_Matches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    ਜੇ (have_ਨਕਾਬ && !reused)
        pdst->ਸੈੱਟ_Zero();
    p.exec(inputs, pdst);
}

}

ਖਾਲੀ cv::ਜੋੜੋ( ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, OutputArray dst,
          ਨਿਵੇਸ਼_Array ਨਕਾਬ, ਪੂਰਨ dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ਜੋੜੋ, src1, src2, dst, ਨਕਾਬ, dtype, OCL_OP_ਜੋੜੋ, ਝੂਠ);
}

ਖਾਲੀ cv::ਘਟਾਓ( ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, OutputArray dst,
                   ਨਿਵੇਸ਼_Array ਨਕਾਬ, ਪੂਰਨ dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_SUB, src1, src2, dst, ਨਕਾਬ, dtype, OCL_OP_SUB, ਝੂਠ);
}

ਖਾਲੀ cv::absdiff( ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ABSDIFF, src1, src2, dst, noArray(), -1, OCL_OP_ABSDIFF, ਝੂਠ);
}

ਖਾਲੀ cv::copy_ਮੰਜ਼ਿਲ(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2)
{
    CV_INSTRUMENT_REGION();

    _src.copy_ਮੰਜ਼ਿਲ(_dst, ਨਕਾਬ_2);
}

/****************************************************************************************\
*                                    multiply/divide                                     *
\****************************************************************************************/

ਨਾਮਾਕਾਸ਼ cv
{

ਖਾਲੀ multiply(ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2,
              OutputArray dst, ਦੁੱਗਣਾ scale, ਪੂਰਨ dtype)
{
    CV_INSTRUMENT_REGION();

    ਅਚਲ ਪੂਰਨ oclop = std::ਨਿਰਪੇਖ(scale - 1.0) < DBL_EPSILON ? OCL_OP_MUL : OCL_OP_MUL_SCALE;
    arithm_op(ew::OP_MUL, src1, src2, dst, noArray(), dtype, oclop, /*muldiv*/ ਸੱਚ, Scalar(scale));
}

ਖਾਲੀ divide(ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2,
                OutputArray dst, ਦੁੱਗਣਾ scale, ਪੂਰਨ dtype)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_DIV, src1, src2, dst, noArray(), dtype, OCL_OP_DIV_SCALE, /*muldiv*/ ਸੱਚ, Scalar(scale));
}

ਖਾਲੀ divide(ਦੁੱਗਣਾ scale, ਨਿਵੇਸ਼_Array src2,
                OutputArray dst, ਪੂਰਨ dtype)
{
    CV_INSTRUMENT_REGION();

    ਜੇ (src2.empty())
    {
        dst.release();
        ਵਾਪਸ;
    }

    // scale / src2 == scale * 1 / src2: feed a 0-dim `1` as the numerator so the CPU engine reuses the
    // normal divide (scale*a/b). Give the numerator src2's OWN depth: then OP_DIV(T, T) is same-type and
    // takes the fast per-type kernel (f32 work for <=16-bit / f16 / bf16, not the s32->f64 path a CV_32S
    // numerator would force). An integer src2 keeps its div-by-zero->0 guard (both operands integer); a
    // float src2 divides as float (1/0->inf), both matching cv::divide. dtype<0 follows src2's depth (a
    // fixed-type dst still wins). The UMat path uses the dedicated reciprocal kernel (OCL_OP_RECIP_SCALE).
    ਦੁੱਗਣਾ one = 1;
    Mat numerator(MatShape::scalar(), src2.ਡੂੰਘਾਈ(), &one);
    scalar_ਮੰਜ਼ਿਲ_Raw_ਅੰਕੜੇ(Scalar(1), &one, src2.ਡੂੰਘਾਈ(), 1);       // write `1` in src2's depth (no allocation)
    ਅਚਲ ਪੂਰਨ rtype = (dtype < 0 && !dst.fixedType()) ? src2.ਡੂੰਘਾਈ() : dtype;
    arithm_op(ew::OP_DIV, numerator, src2, dst, noArray(), rtype, OCL_OP_RECIP_SCALE, /*muldiv*/ ਸੱਚ, Scalar(scale));
}

UMat UMat::mul(ਨਿਵੇਸ਼_Array m, ਦੁੱਗਣਾ scale) ਅਚਲ
{
    UMat dst;
    multiply(*ਇਹ, m, dst, scale);
    ਵਾਪਸ dst;
}

/****************************************************************************************\
*                                      addWeighted                                       *
\****************************************************************************************/

}

ਖਾਲੀ cv::ਜੋੜੋ_Weighted( ਨਿਵੇਸ਼_Array src1, ਦੁੱਗਣਾ alpha, ਨਿਵੇਸ਼_Array src2,
                      ਦੁੱਗਣਾ beta, ਦੁੱਗਣਾ gamma, OutputArray dst, ਪੂਰਨ dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADDW, src1, src2, dst, noArray(), dtype, OCL_OP_ADDW, /*muldiv*/ ਸੱਚ,
              Scalar(alpha, beta, gamma));
}


/****************************************************************************************\
*                                          compare                                       *
\****************************************************************************************/

ਨਾਮਾਕਾਸ਼ cv
{


ਸਥਿਰ ਦੁੱਗਣਾ getMinVal(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ ਅਚਲ ਦੁੱਗਣਾ tab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        0, -128, 0, -32768, INT_MIN, -FLT_MAX, -DBL_MAX,
        -65504, -FLT_MAX, 0, 0, (ਦੁੱਗਣਾ)INT64_MIN, 0
    };
    ਵਾਪਸ tab[ਡੂੰਘਾਈ];
}

ਸਥਿਰ ਦੁੱਗਣਾ getMaxVal(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ ਅਚਲ ਦੁੱਗਣਾ tab[CV_ਡੂੰਘਾਈ_MAX] = {
        255, 127, 65535, 32767, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, FLT_MAX, DBL_MAX,
        65504, FLT_MAX, 255, (ਦੁੱਗਣਾ)UINT64_MAX, (ਦੁੱਗਣਾ)INT64_MAX, (ਦੁੱਗਣਾ)UINT32_MAX, 0
    };
    ਵਾਪਸ tab[ਡੂੰਘਾਈ];
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_compare(ਨਿਵੇਸ਼_Array _src1, ਨਿਵੇਸ਼_Array _src2, OutputArray _dst, ਪੂਰਨ op, ਤਾਰਕਿਕ haveScalar)
{
    ਅਚਲ ocl::Device& dev = ocl::Device::getDefault();
    ਤਾਰਕਿਕ doubleSupport = dev.doubleFPConfig() > 0;
    ਪੂਰਨ type1 = _src1.type(), depth1 = CV_MAT_ਡੂੰਘਾਈ(type1), cn = CV_MAT_CN(type1),
            type2 = _src2.type(), depth2 = CV_MAT_ਡੂੰਘਾਈ(type2);

    ਜੇ (!doubleSupport && depth1 == CV_64F)
        ਵਾਪਸ ਝੂਠ;

    // the integer scalar path below works in int; let the CPU handle the wider 5.x integer depths
    ਜੇ (haveScalar && (depth1 == CV_32U || depth1 == CV_64U || depth1 == CV_64S))
        ਵਾਪਸ ਝੂਠ;

    ਜੇ (!haveScalar && (!_src1.same_ਮਾਪ(_src2) || type1 != type2))
            ਵਾਪਸ ਝੂਠ;

    // This OpenCL kernel broadcasts a scalar's FIRST channel to all channels. The CPU path compares
    // per-channel (scalar[c] for channel c), so a multichannel array against a scalar with DISTINCT
    // channel values would disagree - bail to the CPU engine in that case (rare; not worth a kernel fix).
    ਜੇ (haveScalar && cn > 1)
    {
        Mat sc = _src2.getMat();
        ਪੂਰਨ scn = (ਪੂਰਨ)sc.ਕੁੱਲ();
        ਜੇ (scn > 1)
        {
            ਦੁੱਗਣਾ v[4] = { 0, 0, 0, 0 };
            getConvertFunc(sc.ਡੂੰਘਾਈ(), CV_64F)(sc.ptr(), 1, 0, 1, (uchar*)v, 1, ਮਾਪ(std::min(scn, 4), 1), 0);
            ਲਈ (ਪੂਰਨ i = 1, n = std::min(cn, scn); i < n; i++)
                ਜੇ (v[i] != v[0])
                    ਵਾਪਸ ਝੂਠ;
        }
    }

    ਪੂਰਨ kercn = haveScalar ? cn : ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(_src1, _src2, _dst), ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = dev.isIntel() ? 4 : 1;
    // Workaround for bug with "?:" operator in AMD OpenCL compiler
    ਜੇ (depth1 >= CV_16U)
        kercn = 1;

    ਪੂਰਨ scalarcn = kercn == 3 ? 4 : kercn;
    ਅਚਲ ਅੱਖਰ * ਅਚਲ operationMap[] = { "==", ">", ">=", "<", "<=", "!=" };
    ਅੱਖਰ cvt[50];

    String opts = format("-D %s -D srcT1=%s -D dstT=%s -D DEPTH_dst=%d -D workT=srcT1 -D cn=%d"
                         " -D convertToDT=%s -D OP_CMP -D CMP_OPERATOR=%s -D srcT1_C1=%s"
                         " -D srcT2_C1=%s -D dstT_C1=%s -D workST=%s -D rowsPerWI=%d%s",
                         haveScalar ? "UNARY_OP" : "BINARY_OP",
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(depth1, kercn)),
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_8UC(kercn)), CV_8U, kercn,
                         ocl::convertTypeStr(depth1, CV_8U, kercn, cvt, ਆਕਾਰ(cvt)),
                         operationMap[op], ocl::type_ਮੰਜ਼ਿਲ_Str(depth1),
                         ocl::type_ਮੰਜ਼ਿਲ_Str(depth1), ocl::type_ਮੰਜ਼ਿਲ_Str(CV_8U),
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(depth1, scalarcn)), ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat src1 = _src1.getUMat();
    ਮਾਪ size = src1.size();
    _dst.create(size, CV_8UC(cn));
    UMat dst = _dst.getUMat();

    ਜੇ (haveScalar)
    {
        size_t esz = CV_ELEM_SIZE1(type1) * scalarcn;
        ਦੁੱਗਣਾ buf[4] = { 0, 0, 0, 0 };
        Mat src2 = _src2.getMat();

        ਜੇ( depth1 > CV_32S )
            convertAndUnrollScalar( src2, depth1, (uchar *)buf, kercn );
        ਨਹੀਂਤਾਂ
        {
            ਦੁੱਗਣਾ fval = 0;
            getConvertFunc(depth2, CV_64F)(src2.ptr(), 1, 0, 1, (uchar *)&fval, 1, ਮਾਪ(1, 1), 0);
            ਜੇ( fval < getMinVal(depth1) )
                ਵਾਪਸ dst.ਸੈੱਟ_ਮੰਜ਼ਿਲ(Scalar::all(op == CMP_GT || op == CMP_GE || op == CMP_NE ? 255 : 0)), ਸੱਚ;

            ਜੇ( fval > getMaxVal(depth1) )
                ਵਾਪਸ dst.ਸੈੱਟ_ਮੰਜ਼ਿਲ(Scalar::all(op == CMP_LT || op == CMP_LE || op == CMP_NE ? 255 : 0)), ਸੱਚ;

            ਪੂਰਨ ival = cv_ਗੇੜਾ(fval);
            ਜੇ( fval != ival )
            {
                ਜੇ( op == CMP_LT || op == CMP_GE )
                    ival = cvCeil(fval);
                ਨਹੀਂਤਾਂ ਜੇ( op == CMP_LE || op == CMP_GT )
                    ival = cvFloor(fval);
                ਨਹੀਂਤਾਂ
                    ਵਾਪਸ dst.ਸੈੱਟ_ਮੰਜ਼ਿਲ(Scalar::all(op == CMP_NE ? 255 : 0)), ਸੱਚ;
            }
            convertAndUnrollScalar(Mat(1, 1, CV_32S, &ival), depth1, (uchar *)buf, kercn);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(src1, cn, kercn),
               ocl::KernelArg::WriteOnly(dst, cn, kercn), scalararg);
    }
    ਨਹੀਂਤਾਂ
    {
        UMat src2 = _src2.getUMat();

        k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(src1),
               ocl::KernelArg::Read_Only_No_ਮਾਪ(src2),
               ocl::KernelArg::WriteOnly(dst, cn, kercn));
    }

    size_t globalsize[2] = { (size_t)dst.ਖਾਨੇ_ਗਿਣਤੀ * cn / kercn, ((size_t)dst.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

}

ਖਾਲੀ cv::compare(ਨਿਵੇਸ਼_Array _src1, ਨਿਵੇਸ਼_Array _src2, OutputArray _dst, ਪੂਰਨ op)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( op == CMP_LT || op == CMP_LE || op == CMP_EQ ||
               op == CMP_NE || op == CMP_GE || op == CMP_GT );

    CV_Assert(_src1.empty() == _src2.empty());
    ਜੇ (_src1.empty() && _src2.empty())
    {
        _dst.release();
        ਵਾਪਸ;
    }

    ਤਾਰਕਿਕ haveScalar = ਝੂਠ;

    ਜੇ ((_src1.isMatx() + _src2.isMatx()) == 1
            || !_src1.same_ਮਾਪ(_src2)
            || _src1.type() != _src2.type())
    {
        ਤਾਰਕਿਕ is_src1_scalar = checkScalar(_src1, _src2.type(), _src1.ਕਿਸਮ(), _src2.ਕਿਸਮ());
        ਤਾਰਕਿਕ is_src2_scalar = checkScalar(_src2, _src1.type(), _src2.ਕਿਸਮ(), _src1.ਕਿਸਮ());

        // exactly one scalar operand: keep it as src2 (swapping flips the ordering ops) so the OpenCL
        // path sees the array-op-scalar form. Otherwise - two arrays of different size (broadcast),
        // mixed types, or two scalars - the CPU engine broadcasts / promotes them, so there is NO error
        // here: fall through to arithm_op (which handles scalar detection, broadcast and mixed types).
        ਜੇ (is_src1_scalar && !is_src2_scalar)
        {
            op = op == CMP_LT ? CMP_GT : op == CMP_LE ? CMP_GE :
                op == CMP_GE ? CMP_LE : op == CMP_GT ? CMP_LT : op;
            // src1 is a scalar; swap it with src2
            compare(_src2, _src1, _dst, op);
            ਵਾਪਸ;
        }
        haveScalar = is_src2_scalar && !is_src1_scalar;
    }
    CV_ਅਣਵਰਤਿਆ(haveScalar);   // consumed by CV_OCL_RUN only - a no-op without OpenCL

    // OpenCL handles only the same-size and array-op-scalar forms; a broadcast between two arrays of
    // different size falls through to the CPU engine.
    CV_OCL_ਲੜੀਵਾਰ(_src1.dims() <= 2 && _src2.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat())
               && (haveScalar || _src1.same_ਮਾਪ(_src2)),
               ocl_compare(_src1, _src2, _dst, op, haveScalar))

    // CPU: the element-wise engine. A CMP_* code maps to the corresponding OP_CMP_* TOp; the engine
    // forces a u8 mask (dtype = CV_8U) and compares in the common operand type (a fractional scalar
    // threshold promotes both operands to float - matching the classic ceil/floor threshold logic).
    // The scalar / broadcast / mixed-shape cases are handled inside arithm_op. oclop = -1 tells it the
    // OpenCL path was already tried above (via ocl_compare), so it only runs the CPU engine.
    ew::ਸਿਖਰ cmpOp = op == CMP_EQ ? ew::OP_CMP_EQ : op == CMP_NE ? ew::OP_CMP_NE :
                    op == CMP_LT ? ew::OP_CMP_LT : op == CMP_LE ? ew::OP_CMP_LE :
                    op == CMP_GT ? ew::OP_CMP_GT : ew::OP_CMP_GE;
    arithm_op(cmpOp, _src1, _src2, _dst, noArray(), CV_8U, /*oclop=*/-1, /*muldiv=*/ਝੂਠ);
}

/****************************************************************************************\
*                                        inRange                                         *
\****************************************************************************************/

ਨਾਮਾਕਾਸ਼ cv
{

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ ਵਿੱਚ_Range_SIMD
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ T * src1, ਅਚਲ T * src2, ਅਚਲ T * src3, uchar * dst, ਪੂਰਨ len) ਅਚਲ
    {
        ਲਈ (ਪੂਰਨ x = 0; x < len; x++)
            dst[x] = (src2[x] <= src1[x] && src1[x] <= src3[x]) ? 255 : 0;
        ਵਾਪਸ len;
    }
};

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)

ਸਾਂਚਾ <>
ਬਣਤਰ ਵਿੱਚ_Range_SIMD<uchar>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ uchar * src1, ਅਚਲ uchar * src2, ਅਚਲ uchar * src3,
        uchar * dst, ਪੂਰਨ len) ਅਚਲ
    {
        ਪੂਰਨ x = 0;
        ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = VTraits<v_uint8>::vlanes();

        ਲਈ (; x <= len - ਚੌੜਾਈ; x += ਚੌੜਾਈ)
        {
            v_uint8 values = vx_ਲੱਦੋ(src1 + x);
            v_uint8 ਨੀਵਾਂ = vx_ਲੱਦੋ(src2 + x);
            v_uint8 ਉੱਚਾ = vx_ਲੱਦੋ(src3 + x);

            v_store(dst + x, v_and(v_ge(values, ਨੀਵਾਂ), v_ge(ਉੱਚਾ, values)));
        }
        vx_cleanup();
        ਵਾਪਸ x;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ ਵਿੱਚ_Range_SIMD<schar>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ schar * src1, ਅਚਲ schar * src2, ਅਚਲ schar * src3,
        uchar * dst, ਪੂਰਨ len) ਅਚਲ
    {
        ਪੂਰਨ x = 0;
        ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = VTraits<v_int8>::vlanes();

        ਲਈ (; x <= len - ਚੌੜਾਈ; x += ਚੌੜਾਈ)
        {
            v_int8 values = vx_ਲੱਦੋ(src1 + x);
            v_int8 ਨੀਵਾਂ = vx_ਲੱਦੋ(src2 + x);
            v_int8 ਉੱਚਾ = vx_ਲੱਦੋ(src3 + x);

            v_store((schar*)(dst + x), v_and(v_ge(values, ਨੀਵਾਂ), v_ge(ਉੱਚਾ, values)));
        }
        vx_cleanup();
        ਵਾਪਸ x;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ ਵਿੱਚ_Range_SIMD<ushort>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ ushort * src1, ਅਚਲ ushort * src2, ਅਚਲ ushort * src3,
        uchar * dst, ਪੂਰਨ len) ਅਚਲ
    {
        ਪੂਰਨ x = 0;
        ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = VTraits<v_uint16>::vlanes() * 2;

        ਲਈ (; x <= len - ਚੌੜਾਈ; x += ਚੌੜਾਈ)
        {
            v_uint16 values1 = vx_ਲੱਦੋ(src1 + x);
            v_uint16 low1 = vx_ਲੱਦੋ(src2 + x);
            v_uint16 high1 = vx_ਲੱਦੋ(src3 + x);

            v_uint16 values2 = vx_ਲੱਦੋ(src1 + x + VTraits<v_uint16>::vlanes());
            v_uint16 low2 = vx_ਲੱਦੋ(src2 + x + VTraits<v_uint16>::vlanes());
            v_uint16 high2 = vx_ਲੱਦੋ(src3 + x + VTraits<v_uint16>::vlanes());

            v_store(dst + x, v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2))));
        }
        vx_cleanup();
        ਵਾਪਸ x;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ ਵਿੱਚ_Range_SIMD<ਛੋਟਾ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ ਛੋਟਾ * src1, ਅਚਲ ਛੋਟਾ * src2, ਅਚਲ ਛੋਟਾ * src3,
        uchar * dst, ਪੂਰਨ len) ਅਚਲ
    {
        ਪੂਰਨ x = 0;
        ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = (ਪੂਰਨ)VTraits<v_int16>::vlanes() * 2;

        ਲਈ (; x <= len - ਚੌੜਾਈ; x += ਚੌੜਾਈ)
        {
            v_int16 values1 = vx_ਲੱਦੋ(src1 + x);
            v_int16 low1 = vx_ਲੱਦੋ(src2 + x);
            v_int16 high1 = vx_ਲੱਦੋ(src3 + x);

            v_int16 values2 = vx_ਲੱਦੋ(src1 + x + VTraits<v_int16>::vlanes());
            v_int16 low2 = vx_ਲੱਦੋ(src2 + x + VTraits<v_int16>::vlanes());
            v_int16 high2 = vx_ਲੱਦੋ(src3 + x + VTraits<v_int16>::vlanes());

            v_store((schar*)(dst + x), v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2))));
        }
        vx_cleanup();
        ਵਾਪਸ x;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ ਵਿੱਚ_Range_SIMD<ਪੂਰਨ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ ਪੂਰਨ * src1, ਅਚਲ ਪੂਰਨ * src2, ਅਚਲ ਪੂਰਨ * src3,
        uchar * dst, ਪੂਰਨ len) ਅਚਲ
    {
        ਪੂਰਨ x = 0;
        ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = (ਪੂਰਨ)VTraits<v_int32>::vlanes() * 2;

        ਲਈ (; x <= len - ਚੌੜਾਈ; x += ਚੌੜਾਈ)
        {
            v_int32 values1 = vx_ਲੱਦੋ(src1 + x);
            v_int32 low1 = vx_ਲੱਦੋ(src2 + x);
            v_int32 high1 = vx_ਲੱਦੋ(src3 + x);

            v_int32 values2 = vx_ਲੱਦੋ(src1 + x + VTraits<v_int32>::vlanes());
            v_int32 low2 = vx_ਲੱਦੋ(src2 + x + VTraits<v_int32>::vlanes());
            v_int32 high2 = vx_ਲੱਦੋ(src3 + x + VTraits<v_int32>::vlanes());

            v_pack_store(dst + x, v_reinterpret_as_u16(v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2)))));
        }
        vx_cleanup();
        ਵਾਪਸ x;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ ਵਿੱਚ_Range_SIMD<ਦਸ਼ਮਲਵ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ ਦਸ਼ਮਲਵ * src1, ਅਚਲ ਦਸ਼ਮਲਵ * src2, ਅਚਲ ਦਸ਼ਮਲਵ * src3,
        uchar * dst, ਪੂਰਨ len) ਅਚਲ
    {
        ਪੂਰਨ x = 0;
        ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = (ਪੂਰਨ)VTraits<v_float32>::vlanes() * 2;

        ਲਈ (; x <= len - ਚੌੜਾਈ; x += ਚੌੜਾਈ)
        {
            v_float32 values1 = vx_ਲੱਦੋ(src1 + x);
            v_float32 low1 = vx_ਲੱਦੋ(src2 + x);
            v_float32 high1 = vx_ਲੱਦੋ(src3 + x);

            v_float32 values2 = vx_ਲੱਦੋ(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_ਲੱਦੋ(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_ਲੱਦੋ(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        ਵਾਪਸ x;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ ਵਿੱਚ_Range_SIMD<hfloat>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ hfloat * src1, ਅਚਲ hfloat * src2, ਅਚਲ hfloat * src3,
        uchar * dst, ਪੂਰਨ len) ਅਚਲ
    {
        ਪੂਰਨ x = 0;
        ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = (ਪੂਰਨ)VTraits<v_float32>::vlanes()*2;

        ਲਈ (; x <= len - ਚੌੜਾਈ; x += ਚੌੜਾਈ)
        {
            v_float32 values1 = vx_ਲੱਦੋ_expand(src1 + x);
            v_float32 low1 = vx_ਲੱਦੋ_expand(src2 + x);
            v_float32 high1 = vx_ਲੱਦੋ_expand(src3 + x);

            v_float32 values2 = vx_ਲੱਦੋ_expand(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_ਲੱਦੋ_expand(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_ਲੱਦੋ_expand(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        ਵਾਪਸ x;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ ਵਿੱਚ_Range_SIMD<bfloat>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ bfloat * src1, ਅਚਲ bfloat * src2, ਅਚਲ bfloat * src3,
        uchar * dst, ਪੂਰਨ len) ਅਚਲ
    {
        ਪੂਰਨ x = 0;
        ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = (ਪੂਰਨ)VTraits<v_float32>::vlanes()*2;

        ਲਈ (; x <= len - ਚੌੜਾਈ; x += ਚੌੜਾਈ)
        {
            v_float32 values1 = vx_ਲੱਦੋ_expand(src1 + x);
            v_float32 low1 = vx_ਲੱਦੋ_expand(src2 + x);
            v_float32 high1 = vx_ਲੱਦੋ_expand(src3 + x);

            v_float32 values2 = vx_ਲੱਦੋ_expand(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_ਲੱਦੋ_expand(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_ਲੱਦੋ_expand(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        ਵਾਪਸ x;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ ਵਿੱਚ_Range_SIMD<ਬਿਨਾਨਿਸ਼ਾਨੀ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ * src1, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ * src2, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ * src3,
        uchar * dst, ਪੂਰਨ len) ਅਚਲ
    {
        ਪੂਰਨ x = 0;
        ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = (ਪੂਰਨ)VTraits<v_uint32>::vlanes() * 2;

        ਲਈ (; x <= len - ਚੌੜਾਈ; x += ਚੌੜਾਈ)
        {
            v_uint32 values1 = vx_ਲੱਦੋ(src1 + x);
            v_uint32 low1 = vx_ਲੱਦੋ(src2 + x);
            v_uint32 high1 = vx_ਲੱਦੋ(src3 + x);

            v_uint32 values2 = vx_ਲੱਦੋ(src1 + x + VTraits<v_uint32>::vlanes());
            v_uint32 low2 = vx_ਲੱਦੋ(src2 + x + VTraits<v_uint32>::vlanes());
            v_uint32 high2 = vx_ਲੱਦੋ(src3 + x + VTraits<v_uint32>::vlanes());

            v_pack_store(dst + x, v_reinterpret_as_u16(v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2)))));
        }
        vx_cleanup();
        ਵਾਪਸ x;
    }
};

#ਜੇ CV_SIMD_64F

ਸਾਂਚਾ <>
ਬਣਤਰ ਵਿੱਚ_Range_SIMD<ਦੁੱਗਣਾ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ () (ਅਚਲ ਦੁੱਗਣਾ * src1, ਅਚਲ ਦੁੱਗਣਾ * src2, ਅਚਲ ਦੁੱਗਣਾ * src3,
        uchar * dst, ਪੂਰਨ len) ਅਚਲ
    {
        ਪੂਰਨ x = 0;
        ਅਚਲ ਪੂਰਨ ਕਦਮ = VTraits<v_float64>::vlanes();
        ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = ਕਦਮ * 4;

        ਲਈ (; x <= len - ਚੌੜਾਈ; x += ਚੌੜਾਈ)
        {
            v_float64 v1 = vx_ਲੱਦੋ(src1 + x);
            v_float64 l1 = vx_ਲੱਦੋ(src2 + x);
            v_float64 h1 = vx_ਲੱਦੋ(src3 + x);
            v_uint64 m1 = v_reinterpret_as_u64(v_and(v_ge(v1, l1), v_ge(h1, v1)));

            v_float64 v2 = vx_ਲੱਦੋ(src1 + x + ਕਦਮ);
            v_float64 l2 = vx_ਲੱਦੋ(src2 + x + ਕਦਮ);
            v_float64 h2 = vx_ਲੱਦੋ(src3 + x + ਕਦਮ);
            v_uint64 m2 = v_reinterpret_as_u64(v_and(v_ge(v2, l2), v_ge(h2, v2)));

            v_float64 v3 = vx_ਲੱਦੋ(src1 + x + ਕਦਮ * 2);
            v_float64 l3 = vx_ਲੱਦੋ(src2 + x + ਕਦਮ * 2);
            v_float64 h3 = vx_ਲੱਦੋ(src3 + x + ਕਦਮ * 2);
            v_uint64 m3 = v_reinterpret_as_u64(v_and(v_ge(v3, l3), v_ge(h3, v3)));

            v_float64 v4 = vx_ਲੱਦੋ(src1 + x + ਕਦਮ * 3);
            v_float64 l4 = vx_ਲੱਦੋ(src2 + x + ਕਦਮ * 3);
            v_float64 h4 = vx_ਲੱਦੋ(src3 + x + ਕਦਮ * 3);
            v_uint64 m4 = v_reinterpret_as_u64(v_and(v_ge(v4, l4), v_ge(h4, v4)));


            v_pack_store(dst + x, v_pack(v_pack(m1, m2), v_pack(m3, m4)));
        }
        vx_cleanup();
        ਵਾਪਸ x;
    }
};

#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਖਾਲੀ ਵਿੱਚ_Range(ਅਚਲ ਖਾਲੀ* _src1, size_t step1, ਅਚਲ ਖਾਲੀ* _src2, size_t step2,
         ਅਚਲ ਖਾਲੀ* _src3, size_t step3, uchar* dst, size_t ਕਦਮ,
         ਮਾਪ size)
{
    ਅਚਲ T* src1 = (ਅਚਲ T*)_src1;
    ਅਚਲ T* src2 = (ਅਚਲ T*)_src2;
    ਅਚਲ T* src3 = (ਅਚਲ T*)_src3;
    step1 /= ਆਕਾਰ(src1[0]);
    step2 /= ਆਕਾਰ(src2[0]);
    step3 /= ਆਕਾਰ(src3[0]);

    ਵਿੱਚ_Range_SIMD<T> vop;

    ਲਈ( ; size.ਉਚਾਈ--; src1 += step1, src2 += step2, src3 += step3, dst += ਕਦਮ )
    {
        ਪੂਰਨ x = vop(src1, src2, src3, dst, size.ਚੌੜਾਈ);
        #ਜੇ CV_ENABLE_UNROLLED
        ਲਈ( ; x <= size.ਚੌੜਾਈ - 4; x += 4 )
        {
            ਪੂਰਨ t0, t1;
            t0 = src2[x] <= src1[x] && src1[x] <= src3[x];
            t1 = src2[x+1] <= src1[x+1] && src1[x+1] <= src3[x+1];
            dst[x] = (uchar)-t0; dst[x+1] = (uchar)-t1;
            t0 = src2[x+2] <= src1[x+2] && src1[x+2] <= src3[x+2];
            t1 = src2[x+3] <= src1[x+3] && src1[x+3] <= src3[x+3];
            dst[x+2] = (uchar)-t0; dst[x+3] = (uchar)-t1;
        }
        #ਜੇ_ਅੰਤ
        ਲਈ( ; x < size.ਚੌੜਾਈ; x++ )
            dst[x] = (uchar)-(src2[x] <= src1[x] && src1[x] <= src3[x]);
    }
}

ਸਥਿਰ ਖਾਲੀ ਵਿੱਚ_Range_Reduce(ਅਚਲ uchar* src, uchar* dst, size_t len, ਪੂਰਨ cn)
{
    ਪੂਰਨ k = cn % 4 ? cn % 4 : 4;
    size_t i, j;
    ਜੇ( k == 1 )
        ਲਈ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j];
    ਨਹੀਂਤਾਂ ਜੇ( k == 2 )
        ਲਈ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1];
    ਨਹੀਂਤਾਂ ਜੇ( k == 3 )
        ਲਈ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2];
    ਨਹੀਂਤਾਂ
        ਲਈ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2] & src[j+3];

    ਲਈ( ; k < cn; k += 4 )
    {
        ਲਈ( i = 0, j = k; i < len; i++, j += cn )
            dst[i] &= src[j] & src[j+1] & src[j+2] & src[j+3];
    }
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ਵਿੱਚ_Range_Func)( ਅਚਲ ਖਾਲੀ* src1, size_t step1, ਅਚਲ ਖਾਲੀ* src2, size_t step2,
                             ਅਚਲ ਖਾਲੀ* src3, size_t step3, uchar* dst, size_t ਕਦਮ, ਮਾਪ sz );

ਸਥਿਰ ਵਿੱਚ_Range_Func get_ਵਿੱਚ_Range_Func(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ ਵਿੱਚ_Range_Func ਵਿੱਚ_Range_Tab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        ਵਿੱਚ_Range<uchar>,
        ਵਿੱਚ_Range<schar>,
        ਵਿੱਚ_Range<ushort>,
        ਵਿੱਚ_Range<ਛੋਟਾ>,
        ਵਿੱਚ_Range<ਪੂਰਨ>,
        ਵਿੱਚ_Range<ਦਸ਼ਮਲਵ>,
        ਵਿੱਚ_Range<ਦੁੱਗਣਾ>,
        ਵਿੱਚ_Range<hfloat>,
        ਵਿੱਚ_Range<bfloat>,
        0,
        ਵਿੱਚ_Range<uint64_t>,
        ਵਿੱਚ_Range<int64_t>,
        ਵਿੱਚ_Range<ਬਿਨਾਨਿਸ਼ਾਨੀ>,
        0,
    };

    ਵਾਪਸ ਵਿੱਚ_Range_Tab[ਡੂੰਘਾਈ];
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_ਵਿੱਚ_Range( ਨਿਵੇਸ਼_Array _src, ਨਿਵੇਸ਼_Array _lowerb,
                         ਨਿਵੇਸ਼_Array _upperb, OutputArray _dst )
{
    ਅਚਲ ocl::Device & d = ocl::Device::getDefault();
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag skind = _src.ਕਿਸਮ(), lkind = _lowerb.ਕਿਸਮ(), ukind = _upperb.ਕਿਸਮ();
    ਮਾਪ ssize = _src.size(), lsize = _lowerb.size(), usize = _upperb.size();
    ਪੂਰਨ stype = _src.type(), ltype = _lowerb.type(), utype = _upperb.type();
    ਪੂਰਨ sdepth = CV_MAT_ਡੂੰਘਾਈ(stype), ldepth = CV_MAT_ਡੂੰਘਾਈ(ltype), udepth = CV_MAT_ਡੂੰਘਾਈ(utype);
    ਪੂਰਨ cn = CV_MAT_CN(stype), ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = d.isIntel() ? 4 : 1;
    ਤਾਰਕਿਕ lbScalar = ਝੂਠ, ubScalar = ਝੂਠ;

    ਜੇ( (lkind == ਨਿਵੇਸ਼_Array_2::MATX && skind != ਨਿਵੇਸ਼_Array_2::MATX) ||
        ssize != lsize || stype != ltype )
    {
        ਜੇ( !checkScalar(_lowerb, stype, lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = ਸੱਚ;
    }

    ਜੇ( (ukind == ਨਿਵੇਸ਼_Array_2::MATX && skind != ਨਿਵੇਸ਼_Array_2::MATX) ||
        ssize != usize || stype != utype )
    {
        ਜੇ( !checkScalar(_upperb, stype, ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = ਸੱਚ;
    }

    ਜੇ (lbScalar != ubScalar)
        ਵਾਪਸ ਝੂਠ;

    ਤਾਰਕਿਕ doubleSupport = d.doubleFPConfig() > 0,
            haveScalar = lbScalar && ubScalar;

    ਜੇ ( (!doubleSupport && sdepth == CV_64F) ||
         (!haveScalar && (sdepth != ldepth || sdepth != udepth)) )
        ਵਾਪਸ ਝੂਠ;

    ਪੂਰਨ kercn = haveScalar ? cn : std::max(std::min(ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(_src, _lowerb, _upperb, _dst), 4), cn);
    ਜੇ (kercn % cn != 0)
        kercn = cn;
    ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_ਪ੍ਰਤੀ_WI = kercn / cn;
    String opts = format("%s-D CN=%d -D SRC_T=%s -D SRC_T1=%s -D DST_T=%s -D KERCN=%d -D DEPTH=%d%s -D COLS_PER_WI=%d",
                           haveScalar ? "-D HAVE_SCALAR " : "", cn, ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(sdepth, kercn)),
                           ocl::type_ਮੰਜ਼ਿਲ_Str(sdepth), ocl::type_ਮੰਜ਼ਿਲ_Str(CV_8UC(ਖਾਨੇ_ਗਿਣਤੀ_ਪ੍ਰਤੀ_WI)), kercn, sdepth,
                           doubleSupport ? " -D DOUBLE_SUPPORT" : "", ਖਾਨੇ_ਗਿਣਤੀ_ਪ੍ਰਤੀ_WI);

    ocl::Kernel ker("inrange", ocl::core::inrange_oclsrc, opts);
    ਜੇ (ker.empty())
        ਵਾਪਸ ਝੂਠ;

    _dst.create(ssize, CV_8UC1);
    UMat src = _src.getUMat(), dst = _dst.getUMat(), lscalaru, uscalaru;
    Mat lscalar, uscalar;

    ਜੇ (lbScalar && ubScalar)
    {
        lscalar = _lowerb.getMat();
        uscalar = _upperb.getMat();

        size_t esz = src.elem_ਮਾਪ();
        size_t blocksize = 36;

        Auto_ਬਫਰ<uchar> _buf(blocksize*(((ਪੂਰਨ)lbScalar + (ਪੂਰਨ)ubScalar)*esz + cn) + 2*cn*ਆਕਾਰ(ਪੂਰਨ) + 128);
        uchar *buf = alignPtr(_buf.ਅੰਕੜੇ() + blocksize*cn, 16);

        ਜੇ( ldepth != sdepth && sdepth < CV_32S )
        {
            ਪੂਰਨ* ilbuf = (ਪੂਰਨ*)alignPtr(buf + blocksize*esz, 16);
            ਪੂਰਨ* iubuf = ilbuf + cn;

            ਦੁਆਧਾਰੀ_Func sccvtfunc = getConvertFunc(ldepth, CV_32S);
            sccvtfunc(lscalar.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, ਮਾਪ(cn, 1), 0);
            sccvtfunc(uscalar.ptr(), 1, 0, 1, (uchar*)iubuf, 1, ਮਾਪ(cn, 1), 0);
            ਪੂਰਨ minval = cv_ਗੇੜਾ(getMinVal(sdepth)), maxval = cv_ਗੇੜਾ(getMaxVal(sdepth));

            ਲਈ( ਪੂਰਨ k = 0; k < cn; k++ )
            {
                ਜੇ( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lscalar = Mat(cn, 1, CV_32S, ilbuf);
            uscalar = Mat(cn, 1, CV_32S, iubuf);
        }

        lscalar.convert_ਮੰਜ਼ਿਲ(lscalar, stype);
        uscalar.convert_ਮੰਜ਼ਿਲ(uscalar, stype);
    }
    ਨਹੀਂਤਾਂ
    {
        lscalaru = _lowerb.getUMat();
        uscalaru = _upperb.getUMat();
    }

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, 1, ਖਾਨੇ_ਗਿਣਤੀ_ਪ੍ਰਤੀ_WI);

    ਜੇ (haveScalar)
    {
        lscalar.copy_ਮੰਜ਼ਿਲ(lscalaru);
        uscalar.copy_ਮੰਜ਼ਿਲ(uscalaru);

        ker.ਦਲੀਲਾਂ(srcarg, dstarg, ocl::KernelArg::PtrReadOnly(lscalaru),
               ocl::KernelArg::PtrReadOnly(uscalaru), ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI);
    }
    ਨਹੀਂਤਾਂ
        ker.ਦਲੀਲਾਂ(srcarg, dstarg, ocl::KernelArg::Read_Only_No_ਮਾਪ(lscalaru),
               ocl::KernelArg::Read_Only_No_ਮਾਪ(uscalaru), ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI);

    size_t globalsize[2] = { (size_t)ssize.ਚੌੜਾਈ / ਖਾਨੇ_ਗਿਣਤੀ_ਪ੍ਰਤੀ_WI, ((size_t)ssize.ਉਚਾਈ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ ker.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

}

ਖਾਲੀ cv::ਵਿੱਚ_Range_2(ਨਿਵੇਸ਼_Array _src, ਨਿਵੇਸ਼_Array _lowerb,
                 ਨਿਵੇਸ਼_Array _upperb, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(! _src.empty());
    ਜੇ (!get_ਵਿੱਚ_Range_Func(_src.ਡੂੰਘਾਈ()))
        CV_Error( cv::Error::StsNotImplemented,
                 "inRange: CV_Bool and CV_8F inputs are not supported, convert the input to another type first");

    CV_OCL_ਲੜੀਵਾਰ(_src.dims() <= 2 && _lowerb.dims() <= 2 &&
               _upperb.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat()),
               ocl_ਵਿੱਚ_Range(_src, _lowerb, _upperb, _dst))

    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag skind = _src.ਕਿਸਮ(), lkind = _lowerb.ਕਿਸਮ(), ukind = _upperb.ਕਿਸਮ();
    Mat src = _src.getMat(), lb = _lowerb.getMat(), ub = _upperb.getMat();

    ਤਾਰਕਿਕ lbScalar = ਝੂਠ, ubScalar = ਝੂਠ;

    ਜੇ( (lkind == ਨਿਵੇਸ਼_Array_2::MATX && skind != ਨਿਵੇਸ਼_Array_2::MATX) ||
        src.size != lb.size || src.type() != lb.type() )
    {
        ਜੇ( !checkScalar(lb, src.type(), lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = ਸੱਚ;
    }

    ਜੇ( (ukind == ਨਿਵੇਸ਼_Array_2::MATX && skind != ਨਿਵੇਸ਼_Array_2::MATX) ||
        src.size != ub.size || src.type() != ub.type() )
    {
        ਜੇ( !checkScalar(ub, src.type(), ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = ਸੱਚ;
    }

    CV_Assert(lbScalar == ubScalar);

    ਪੂਰਨ cn = src.channels(), ਡੂੰਘਾਈ = src.ਡੂੰਘਾਈ();

    size_t esz = src.elem_ਮਾਪ();
    size_t blocksize0 = (size_t)(ਖੰਡ_ਮਾਪ + esz-1)/esz;

    _dst.create_Same_ਮਾਪ(_src, CV_8UC1);
    Mat dst = _dst.getMat();
    ਵਿੱਚ_Range_Func func = get_ਵਿੱਚ_Range_Func(ਡੂੰਘਾਈ);

    ਅਚਲ Mat* arrays_sc[] = { &src, &dst, 0 };
    ਅਚਲ Mat* arrays_nosc[] = { &src, &dst, &lb, &ub, 0 };
    uchar* ptrs[4] = {};

    NAryMatIterator it(lbScalar && ubScalar ? arrays_sc : arrays_nosc, ptrs);
    size_t ਕੁੱਲ = it.size, blocksize = std::min(ਕੁੱਲ, blocksize0);

    Auto_ਬਫਰ<uchar> _buf(blocksize*(((ਪੂਰਨ)lbScalar + (ਪੂਰਨ)ubScalar)*esz + cn) + 2*cn*ਆਕਾਰ(ਪੂਰਨ) + 128);
    uchar *buf = _buf.ਅੰਕੜੇ(), *mbuf = buf, *lbuf = 0, *ubuf = 0;
    buf = alignPtr(buf + blocksize*cn, 16);

    ਜੇ( lbScalar && ubScalar )
    {
        lbuf = buf;
        ubuf = buf = alignPtr(buf + blocksize*esz, 16);

        CV_Assert( lb.type() == ub.type() );
        ਪੂਰਨ scdepth = lb.ਡੂੰਘਾਈ();

        ਜੇ( scdepth != ਡੂੰਘਾਈ && ਡੂੰਘਾਈ < CV_32S )
        {
            ਪੂਰਨ* ilbuf = (ਪੂਰਨ*)alignPtr(buf + blocksize*esz, 16);
            ਪੂਰਨ* iubuf = ilbuf + cn;

            ਦੁਆਧਾਰੀ_Func sccvtfunc = getConvertFunc(scdepth, CV_32S);
            sccvtfunc(lb.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, ਮਾਪ(cn, 1), 0);
            sccvtfunc(ub.ptr(), 1, 0, 1, (uchar*)iubuf, 1, ਮਾਪ(cn, 1), 0);
            ਪੂਰਨ minval = cv_ਗੇੜਾ(getMinVal(ਡੂੰਘਾਈ)), maxval = cv_ਗੇੜਾ(getMaxVal(ਡੂੰਘਾਈ));

            ਲਈ( ਪੂਰਨ k = 0; k < cn; k++ )
            {
                ਜੇ( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lb = Mat(cn, 1, CV_32S, ilbuf);
            ub = Mat(cn, 1, CV_32S, iubuf);
        }

        convertAndUnrollScalar( lb, src.type(), lbuf, blocksize );
        convertAndUnrollScalar( ub, src.type(), ubuf, blocksize );

        ਜੇ (ਡੂੰਘਾਈ == CV_8U && src.dims <= 2) {
            uint8_t lb_scalar = lbuf[0];
            uint8_t ub_scalar = ubuf[0];
            CALL_HAL(ਵਿੱਚ_Range_u8, cv_hal_ਵਿੱਚ_Range8u, src.ਅੰਕੜੇ, src.ਕਦਮ, dst.ਅੰਕੜੇ, dst.ਕਦਮ, dst.ਡੂੰਘਾਈ(), src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ, src.channels(),
                lb_scalar, ub_scalar);
        } ਨਹੀਂਤਾਂ ਜੇ (ਡੂੰਘਾਈ == CV_32F && src.dims <= 2) {
            ਦੁੱਗਣਾ lb_scalar = lb.ptr<ਦੁੱਗਣਾ>(0)[0];
            ਦੁੱਗਣਾ ub_scalar = ub.ptr<ਦੁੱਗਣਾ>(0)[0];
            CALL_HAL(ਵਿੱਚ_Range_f32, cv_hal_ਵਿੱਚ_Range32f, src.ਅੰਕੜੇ, src.ਕਦਮ, dst.ਅੰਕੜੇ, dst.ਕਦਮ, dst.ਡੂੰਘਾਈ(), src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ, src.channels(),
                lb_scalar, ub_scalar);
        }
    }

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ਲਈ( size_t j = 0; j < ਕੁੱਲ; j += blocksize )
        {
            ਪੂਰਨ bsz = (ਪੂਰਨ)MIN(ਕੁੱਲ - j, blocksize);
            size_t delta = bsz*esz;
            uchar *lptr = lbuf, *uptr = ubuf;
            ਜੇ( !lbScalar )
            {
                lptr = ptrs[2];
                ptrs[2] += delta;
            }
            ਜੇ( !ubScalar )
            {
                ਪੂਰਨ idx = !lbScalar ? 3 : 2;
                uptr = ptrs[idx];
                ptrs[idx] += delta;
            }
            func( ptrs[0], 0, lptr, 0, uptr, 0, cn == 1 ? ptrs[1] : mbuf, 0, ਮਾਪ(bsz*cn, 1));
            ਜੇ( cn > 1 )
                ਵਿੱਚ_Range_Reduce(mbuf, ptrs[1], bsz, cn);
            ptrs[0] += delta;
            ptrs[1] += bsz;
        }
    }
}
/* End of file. */
