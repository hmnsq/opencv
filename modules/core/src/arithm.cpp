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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "arithm_expr.hpp"   // the new element-wise engine (cv::ew)
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

ನಾಮಾಕಾಶ cv
{

/****************************************************************************************\
*                                   logical operations                                   *
\****************************************************************************************/

ಎಣಿಕೆ { OCL_OP_ಕೂಡಿಸು=0, OCL_OP_SUB=1, OCL_OP_RSUB=2, OCL_OP_ABSDIFF=3, OCL_OP_MUL=4,
       OCL_OP_MUL_SCALE=5, OCL_OP_DIV_SCALE=6, OCL_OP_RECIP_SCALE=7, OCL_OP_ADDW=8,
       OCL_OP_AND=9, OCL_OP_OR=10, OCL_OP_XOR=11, OCL_OP_NOT=12, OCL_OP_MIN=13, OCL_OP_MAX=14,
       OCL_OP_RDIV_SCALE=15 };

// The unified entry point for every element-wise binary op (empty + UMat/OpenCL + CPU engine). Defined
// lower down (near cv::add); forward-declared here so cv::min/cv::max (above it) can use it too.
ಸ್ಥಿರ ಶೂನ್ಯ arithm_op(ew::ತುದಿ op, ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst,
                      ಒಳಹರಿವು_Array ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ dtype, ಪೂರ್ಣಾಂಕ oclop, ತಾರ್ಕಿಕ muldiv, ಸ್ಥಿರಾಂಕ Scalar& params = Scalar(1));

// The unary counterpart (bitwise NOT, unary math later). Forward-declared for the same reason.
ಸ್ಥಿರ ಶೂನ್ಯ unary_op(ew::ತುದಿ op, ಒಳಹರಿವು_Array src, OutputArray dst, ಒಳಹರಿವು_Array ಮುಖವಾಡ,
                     ಪೂರ್ಣಾಂಕ dtype, ಸ್ಥಿರಾಂಕ Scalar& params = Scalar());

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* oclop2str[] = { "OP_ADD", "OP_SUB", "OP_RSUB", "OP_ABSDIFF",
    "OP_MUL", "OP_MUL_SCALE", "OP_DIV_SCALE", "OP_RECIP_SCALE",
    "OP_ADDW", "OP_AND", "OP_OR", "OP_XOR", "OP_NOT", "OP_MIN", "OP_MAX", "OP_RDIV_SCALE", 0 };

// depths with no OpenCL type name (ocl::typeToStr() asserts on them)
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ noOclType(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಹಿಂತಿರುಗಿ ಆಳ == CV_16BF || ಆಳ == CV_Bool || ಆಳ == CV_8F_E4M3FN || ಆಳ == CV_8F_E4M3FNUZ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_ದ್ವಿಮಾನ_op(ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2, OutputArray _dst,
                          ಒಳಹರಿವು_Array ಮುಖವಾಡ_2, ತಾರ್ಕಿಕ bitwise, ಪೂರ್ಣಾಂಕ oclop, ತಾರ್ಕಿಕ haveScalar )
{
    ತಾರ್ಕಿಕ have_ಮುಖವಾಡ = !ಮುಖವಾಡ_2.empty();
    ಪೂರ್ಣಾಂಕ srctype = _src1.type();
    ಪೂರ್ಣಾಂಕ srcdepth = CV_MAT_ಆಳ(srctype);
    ಪೂರ್ಣಾಂಕ cn = CV_MAT_CN(srctype);

    ಸ್ಥಿರಾಂಕ ocl::Device d = ocl::Device::getDefault();
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;
    ಆದರೆ( oclop < 0 || ((have_ಮುಖವಾಡ || haveScalar) && cn > 4) ||
            (!doubleSupport && srcdepth == CV_64F && !bitwise) ||
            (noOclType(srcdepth) && !bitwise))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಅಕ್ಷರ opts[1024];
    ಪೂರ್ಣಾಂಕ kercn = have_ಮುಖವಾಡ || haveScalar ? cn : ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(_src1, _src2, _dst);
    ಪೂರ್ಣಾಂಕ scalarcn = kercn == 3 ? 4 : kercn;
    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = d.isIntel() ? 4 : 1;

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dst_ಆಳ = srcdepth;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dstType = CV_MAKETYPE(dst_ಆಳ, kercn);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dstType1 = CV_MAKETYPE(dst_ಆಳ, 1);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ scalarType = CV_MAKETYPE(srcdepth, scalarcn);

    snprintf(opts, ಗಾತ್ರ(opts), "-D %s%s -D %s%s -D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workST=%s -D cn=%d -D rowsPerWI=%d",
            have_ಮುಖವಾಡ ? "MASK_" : "", haveScalar ? "UNARY_OP" : "BINARY_OP", oclop2str[oclop],
            doubleSupport ? " -D DOUBLE_SUPPORT" : "",
            bitwise ? ocl::memop_Type_ಗಮ್ಯ_Str(dstType) : ocl::type_ಗಮ್ಯ_Str(dstType),
            dst_ಆಳ,
            bitwise ? ocl::memop_Type_ಗಮ್ಯ_Str(dstType1) : ocl::type_ಗಮ್ಯ_Str(dstType1),
            bitwise ? ocl::memop_Type_ಗಮ್ಯ_Str(scalarType) : ocl::type_ಗಮ್ಯ_Str(scalarType),
            kercn, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI);

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), ಮುಖವಾಡ = ಮುಖವಾಡ_2.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src1, cn, kercn);
    ocl::KernelArg dstarg = have_ಮುಖವಾಡ ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(ಮುಖವಾಡ, 1);

    ಆದರೆ( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(srctype)*scalarcn;
        ದ್ವಿಗುಣ buf[4] = {0,0,0,0};

        ಆದರೆ( oclop != OCL_OP_NOT )
        {
            Mat src2sc = _src2.getMat();
            convertAndUnrollScalar(src2sc, srctype, (uchar*)buf, 1);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        ಆದರೆ( !have_ಮುಖವಾಡ )
            k.ವಾದಗಳು(src1arg, dstarg, scalararg);
        ಇಲ್ಲದಿದ್ದರೆ
            k.ವಾದಗಳು(src1arg, maskarg, dstarg, scalararg);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src2, cn, kercn);

        ಆದರೆ( !have_ಮುಖವಾಡ )
            k.ವಾದಗಳು(src1arg, src2arg, dstarg);
        ಇಲ್ಲದಿದ್ದರೆ
            k.ವಾದಗಳು(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn / kercn, ((size_t)src1.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, 0, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

// OpenCL path for the bitwise ops (and/or/xor/not), lifted out of binary_op: choose the (possibly
// scalar) operand order, size the destination, and dispatch the shared bitwise OpenCL kernel. Returns
// true if OpenCL handled the op, false to fall through to the CPU engine. NOT arrives unary
// (src2 == src1, forced-scalar). The caller gates this on a UMat operand + dims <= 2 (via CV_OCL_RUN).
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ bitwise_op_ocl(ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2, OutputArray _dst,
                           ಒಳಹರಿವು_Array ಮುಖವಾಡ_2, ಪೂರ್ಣಾಂಕ oclop)
{
    ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag kind1 = psrc1->ಬಗೆ(), kind2 = psrc2->ಬಗೆ();
    ಪೂರ್ಣಾಂಕ type1 = psrc1->type(), type2 = psrc2->type();
    ತಾರ್ಕಿಕ haveScalar = ಸುಳ್ಳು;

    ಆದರೆ( oclop == OCL_OP_NOT )
        haveScalar = ನಿಜ;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( (kind1 == ಒಳಹರಿವು_Array_2::MATX) + (kind2 == ಒಳಹರಿವು_Array_2::MATX) == 1 ||
             !psrc1->same_ಗಾತ್ರ(*psrc2) || type1 != type2 )
    {
        ಆದರೆ( checkScalar(*psrc1, type2, kind1, kind2) )
        { std::swap(psrc1, psrc2); std::swap(type1, type2); }        // src1 is the scalar; swap it out
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( !checkScalar(*psrc2, type1, kind2, kind1) )
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        haveScalar = ನಿಜ;
    }
    _dst.create_Same_ಗಾತ್ರ(*psrc1, type1);
    ಹಿಂತಿರುಗಿ ocl_ದ್ವಿಮಾನ_op(*psrc1, *psrc2, _dst, ಮುಖವಾಡ_2, ನಿಜ, oclop, haveScalar);
}
#ಆದರೆ_ಅಂತ್ಯ  // HAVE_OPENCL

}

// bitwise and/or/xor: OpenCL runs the shared bitwise kernel (via bitwise_op_ocl); the CPU path is the
// element-wise engine (OP_AND/OR/XOR, T x T -> T dispatched by element size). oclop = -1 tells
// arithm_op the OpenCL path was already tried, so it only runs the CPU engine. NOT is unary.
ಶೂನ್ಯ cv::bitwise_and(ಒಳಹರಿವು_Array a, ಒಳಹರಿವು_Array b, OutputArray c, ಒಳಹರಿವು_Array ಮುಖವಾಡ)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_ಸತತ((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, ಮುಖವಾಡ, OCL_OP_AND))
    arithm_op(ew::OP_AND, a, b, c, ಮುಖವಾಡ, -1, /*oclop=*/-1, /*muldiv=*/ಸುಳ್ಳು);
}

ಶೂನ್ಯ cv::bitwise_or(ಒಳಹರಿವು_Array a, ಒಳಹರಿವು_Array b, OutputArray c, ಒಳಹರಿವು_Array ಮುಖವಾಡ)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_ಸತತ((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, ಮುಖವಾಡ, OCL_OP_OR))
    arithm_op(ew::OP_OR, a, b, c, ಮುಖವಾಡ, -1, /*oclop=*/-1, /*muldiv=*/ಸುಳ್ಳು);
}

ಶೂನ್ಯ cv::bitwise_xor(ಒಳಹರಿವು_Array a, ಒಳಹರಿವು_Array b, OutputArray c, ಒಳಹರಿವು_Array ಮುಖವಾಡ)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_ಸತತ((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, ಮುಖವಾಡ, OCL_OP_XOR))
    arithm_op(ew::OP_XOR, a, b, c, ಮುಖವಾಡ, -1, /*oclop=*/-1, /*muldiv=*/ಸುಳ್ಳು);
}

ಶೂನ್ಯ cv::bitwise_not(ಒಳಹರಿವು_Array a, OutputArray c, ಒಳಹರಿವು_Array ಮುಖವಾಡ)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_ಸತತ((a.isUMat() || c.isUMat()) && a.dims() <= 2,
               bitwise_op_ocl(a, a, c, ಮುಖವಾಡ, OCL_OP_NOT))
    unary_op(ew::OP_NOT, a, c, ಮುಖವಾಡ, -1);
}

ಶೂನ್ಯ cv::max( ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, ಸುಳ್ಳು);
}

ಶೂನ್ಯ cv::min( ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, ಸುಳ್ಳು);
}

// The concrete Mat/UMat overloads of min/max exist because C++ overload resolution needs them (a
// bare cv::min(mat, mat, mat) binds Mat& more tightly than InputArray); they must run the SAME engine
// path as the InputArray forms - NOT the legacy binary_op - so mixed types and broadcasting work.
ಶೂನ್ಯ cv::max(ಸ್ಥಿರಾಂಕ Mat& src1, ಸ್ಥಿರಾಂಕ Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, ಸುಳ್ಳು);
}

ಶೂನ್ಯ cv::min(ಸ್ಥಿರಾಂಕ Mat& src1, ಸ್ಥಿರಾಂಕ Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, ಸುಳ್ಳು);
}

ಶೂನ್ಯ cv::max(ಸ್ಥಿರಾಂಕ UMat& src1, ಸ್ಥಿರಾಂಕ UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, ಸುಳ್ಳು);
}

ಶೂನ್ಯ cv::min(ಸ್ಥಿರಾಂಕ UMat& src1, ಸ್ಥಿರಾಂಕ UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, ಸುಳ್ಳು);
}


/****************************************************************************************\
*                                      add/subtract                                      *
\****************************************************************************************/

ನಾಮಾಕಾಶ cv
{

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

// used by the OpenCL branch only (arithm_op_ocl working-type selection)
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ actual_Scalar_ಆಳ(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ len)
{
    ಪೂರ್ಣಾಂಕ i = 0, minval = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, maxval = INT_MIN;
    ಗಾಗಿ(; i < len; ++i)
    {
        ಪೂರ್ಣಾಂಕ ival = cv_ಸುತ್ತು(ದತ್ತಾಂಶ[i]);
        ಆದರೆ( ival != ದತ್ತಾಂಶ[i] )
            ನಿಲ್ಲಿಸು;
        minval = MIN(minval, ival);
        maxval = MAX(maxval, ival);
    }
    ಹಿಂತಿರುಗಿ i < len ? CV_64F :
        minval >= 0 && maxval <= (ಪೂರ್ಣಾಂಕ)UCHAR_MAX ? CV_8U :
        minval >= (ಪೂರ್ಣಾಂಕ)SCHAR_MIN && maxval <= (ಪೂರ್ಣಾಂಕ)SCHAR_MAX ? CV_8S :
        minval >= 0 && maxval <= (ಪೂರ್ಣಾಂಕ)USHRT_MAX ? CV_16U :
        minval >= (ಪೂರ್ಣಾಂಕ)SHRT_MIN && maxval <= (ಪೂರ್ಣಾಂಕ)SHRT_MAX ? CV_16S :
        CV_32S;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ coerceTypes(ಪೂರ್ಣಾಂಕ depth1, ಪೂರ್ಣಾಂಕ depth2, ತಾರ್ಕಿಕ muldiv)
{
    ಹಿಂತಿರುಗಿ depth1 == depth2 ? depth1 :
        ((depth1 <= CV_32S) & (depth2 <= CV_32S)) != 0 ?
        (((ಪೂರ್ಣಾಂಕ)!muldiv & (depth1 <= CV_8S) & (depth2 <= CV_8S)) != 0 ? CV_16S : CV_32S) :
        ((CV_ELEM_SIZE1(depth1) > 4) | (CV_ELEM_SIZE1(depth2) > 4)) != 0 ? CV_64F : CV_32F;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_arithm_op(ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2, OutputArray _dst,
                          ಒಳಹರಿವು_Array ಮುಖವಾಡ_2, ಪೂರ್ಣಾಂಕ wtype,
                          ಶೂನ್ಯ* usrdata, ಪೂರ್ಣಾಂಕ oclop,
                          ತಾರ್ಕಿಕ haveScalar )
{
    ಸ್ಥಿರಾಂಕ ocl::Device d = ocl::Device::getDefault();
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;
    ಪೂರ್ಣಾಂಕ type1 = _src1.type(), depth1 = CV_MAT_ಆಳ(type1), cn = CV_MAT_CN(type1);
    ತಾರ್ಕಿಕ have_ಮುಖವಾಡ = !ಮುಖವಾಡ_2.empty();

    ಆದರೆ ( (have_ಮುಖವಾಡ || haveScalar) && cn > 4 )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __APPLE__
    ಆದರೆ (depth1 == CV_16U && (oclop == OCL_OP_MUL || oclop == OCL_OP_MUL_SCALE))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ

    ಪೂರ್ಣಾಂಕ dtype = _dst.type(), ddepth = CV_MAT_ಆಳ(dtype), wdepth = std::max(CV_32S, CV_MAT_ಆಳ(wtype));
    ಆದರೆ (!doubleSupport)
        wdepth = std::min(wdepth, CV_32F);

    wtype = CV_MAKETYPE(wdepth, cn);
    ಪೂರ್ಣಾಂಕ type2 = haveScalar ? wtype : _src2.type(), depth2 = CV_MAT_ಆಳ(type2);
    ಆದರೆ (!doubleSupport && (depth2 == CV_64F || depth1 == CV_64F))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ (noOclType(depth1) || noOclType(depth2) || noOclType(ddepth) || noOclType(wdepth))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಪೂರ್ಣಾಂಕ kercn = have_ಮುಖವಾಡ || haveScalar ? cn : ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(_src1, _src2, _dst);
    ಪೂರ್ಣಾಂಕ scalarcn = kercn == 3 ? 4 : kercn, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = d.isIntel() ? 4 : 1;

    ಅಕ್ಷರ cvtstr[4][50], opts[1024];
    snprintf(opts, ಗಾತ್ರ(opts), "-D %s%s -D %s -D srcT1=%s -D srcT1_C1=%s -D srcT2=%s -D srcT2_C1=%s "
            "-D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workT=%s -D workST=%s -D scaleT=%s -D wdepth=%d -D convertToWT1=%s "
            "-D convertToWT2=%s -D convertToDT=%s%s -D cn=%d -D rowsPerWI=%d -D convertFromU=%s",
            (have_ಮುಖವಾಡ ? "MASK_" : ""), (haveScalar ? "UNARY_OP" : "BINARY_OP"),
            oclop2str[oclop], ocl::type_ಗಮ್ಯ_Str(CV_MAKETYPE(depth1, kercn)),
            ocl::type_ಗಮ್ಯ_Str(depth1), ocl::type_ಗಮ್ಯ_Str(CV_MAKETYPE(depth2, kercn)),
            ocl::type_ಗಮ್ಯ_Str(depth2), ocl::type_ಗಮ್ಯ_Str(CV_MAKETYPE(ddepth, kercn)), ddepth,
            ocl::type_ಗಮ್ಯ_Str(ddepth), ocl::type_ಗಮ್ಯ_Str(CV_MAKETYPE(wdepth, kercn)),
            ocl::type_ಗಮ್ಯ_Str(CV_MAKETYPE(wdepth, scalarcn)),
            ocl::type_ಗಮ್ಯ_Str(wdepth), wdepth,
            ocl::convertTypeStr(depth1, wdepth, kercn, cvtstr[0], ಗಾತ್ರ(cvtstr[0])),
            ocl::convertTypeStr(depth2, wdepth, kercn, cvtstr[1], ಗಾತ್ರ(cvtstr[1])),
            ocl::convertTypeStr(wdepth, ddepth, kercn, cvtstr[2], ಗಾತ್ರ(cvtstr[2])),
            doubleSupport ? " -D DOUBLE_SUPPORT" : "", kercn, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI,
            oclop == OCL_OP_ABSDIFF && wdepth == CV_32S && ddepth == wdepth ?
            ocl::convertTypeStr(CV_8U, ddepth, kercn, cvtstr[3], ಗಾತ್ರ(cvtstr[3])) : "noconvert");

    size_t usrdata_esz = CV_ELEM_ಗಾತ್ರ(wdepth);
    ಸ್ಥಿರಾಂಕ uchar* usrdata_p = (ಸ್ಥಿರಾಂಕ uchar*)usrdata;
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* usrdata_d = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)usrdata;
    ದಶಮಾಂಶ usrdata_f[3];
    ಪೂರ್ಣಾಂಕ i, n = oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE ||
        oclop == OCL_OP_RDIV_SCALE || oclop == OCL_OP_RECIP_SCALE ? 1 : oclop == OCL_OP_ADDW ? 3 : 0;
    ಆದರೆ( usrdata && n > 0 && wdepth == CV_32F )
    {
        ಗಾಗಿ( i = 0; i < n; i++ )
            usrdata_f[i] = (ದಶಮಾಂಶ)usrdata_d[i];
        usrdata_p = (ಸ್ಥಿರಾಂಕ uchar*)usrdata_f;
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), ಮುಖವಾಡ = ಮುಖವಾಡ_2.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src1, cn, kercn);
    ocl::KernelArg dstarg = have_ಮುಖವಾಡ ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(ಮುಖವಾಡ, 1);

    ಆದರೆ( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(wtype)*scalarcn;
        ದ್ವಿಗುಣ buf[4]={0,0,0,0};
        Mat src2sc = _src2.getMat();

        ಆದರೆ( !src2sc.empty() )
            convertAndUnrollScalar(src2sc, wtype, (uchar*)buf, 1);
        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        ಆದರೆ( !have_ಮುಖವಾಡ )
        {
            ಆದರೆ(n == 0)
                k.ವಾದಗಳು(src1arg, dstarg, scalararg);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(n == 1)
                k.ವಾದಗಳು(src1arg, dstarg, scalararg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            ಇಲ್ಲದಿದ್ದರೆ
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        ಇಲ್ಲದಿದ್ದರೆ
            k.ವಾದಗಳು(src1arg, maskarg, dstarg, scalararg);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src2, cn, kercn);

        ಆದರೆ( !have_ಮುಖವಾಡ )
        {
            ಆದರೆ (n == 0)
                k.ವಾದಗಳು(src1arg, src2arg, dstarg);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (n == 1)
                k.ವಾದಗಳು(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (n == 3)
                k.ವಾದಗಳು(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz*2, usrdata_esz));
            ಇಲ್ಲದಿದ್ದರೆ
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        ಇಲ್ಲದಿದ್ದರೆ
            k.ವಾದಗಳು(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn / kercn, ((size_t)src1.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

// OpenCL branch of the arithmetic ops, extracted from the former arithm_op (whose CPU compute is now
// the element-wise engine). Returns true if an OpenCL kernel handled the op, false to let the caller
// fall through to the CPU engine. min/max use the bitwise-style kernel; every other op the arithm
// kernel with a coerced working type. `usrdata` carries the mul/div/addWeighted scale block, `oclop`
// the kernel id. Only reached for a UMat dst; sizes/types that don't fit the scalar/same-size pattern
// (e.g. a broadcast) decline (return false) so the engine handles them on the CPU.
ಸ್ಥಿರ ತಾರ್ಕಿಕ arithm_op_ocl(ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2, OutputArray _dst,
                          ಒಳಹರಿವು_Array ಮುಖವಾಡ_2, ಪೂರ್ಣಾಂಕ dtype, ಪೂರ್ಣಾಂಕ oclop, ತಾರ್ಕಿಕ muldiv, ಶೂನ್ಯ* usrdata)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENCL
    CV_ಬಳಸದ(_src1); CV_ಬಳಸದ(_src2); CV_ಬಳಸದ(_dst); CV_ಬಳಸದ(ಮುಖವಾಡ_2);
    CV_ಬಳಸದ(dtype); CV_ಬಳಸದ(oclop); CV_ಬಳಸದ(muldiv); CV_ಬಳಸದ(usrdata);
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag kind1 = psrc1->ಬಗೆ(), kind2 = psrc2->ಬಗೆ();
    ತಾರ್ಕಿಕ have_ಮುಖವಾಡ = !ಮುಖವಾಡ_2.empty();
    ತಾರ್ಕಿಕ reallocate = ಸುಳ್ಳು;
    ಪೂರ್ಣಾಂಕ type1 = psrc1->type(), depth1 = CV_MAT_ಆಳ(type1), cn = CV_MAT_CN(type1);
    ಪೂರ್ಣಾಂಕ type2 = psrc2->type(), depth2 = CV_MAT_ಆಳ(type2), cn2 = CV_MAT_CN(type2);
    ಪೂರ್ಣಾಂಕ wtype, dims1 = psrc1->dims(), dims2 = psrc2->dims();
    ಗಾತ್ರ_2 sz1 = dims1 <= 2 ? psrc1->size() : ಗಾತ್ರ_2();
    ಗಾತ್ರ_2 sz2 = dims2 <= 2 ? psrc2->size() : ಗಾತ್ರ_2();

    ಆದರೆ (!(OCL_PERFORMANCE_CHECK(_dst.isUMat()) && dims1 <= 2 && dims2 <= 2))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    // min/max: the bitwise-style OpenCL kernel (same-type element-wise, no working-type coercion).
    ಆದರೆ (oclop == OCL_OP_MIN || oclop == OCL_OP_MAX)
    {
        ತಾರ್ಕಿಕ haveScalar = ಸುಳ್ಳು;
        ಆದರೆ ((kind1 == ಒಳಹರಿವು_Array_2::MATX) + (kind2 == ಒಳಹರಿವು_Array_2::MATX) == 1 ||
            !psrc1->same_ಗಾತ್ರ(*psrc2) || type1 != type2)
        {
            ಆದರೆ (checkScalar(*psrc1, type2, kind1, kind2))
            { std::swap(psrc1, psrc2); std::swap(type1, type2); }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!checkScalar(*psrc2, type1, kind2, kind1))
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
            haveScalar = ನಿಜ;
        }
        _dst.create_Same_ಗಾತ್ರ(*psrc1, type1);
        ಹಿಂತಿರುಗಿ ocl_ದ್ವಿಮಾನ_op(*psrc1, *psrc2, _dst, ಮುಖವಾಡ_2, ಸುಳ್ಳು, oclop, haveScalar);
    }

    // reciprocal (scale/src): the OpenCL kernel is unary on src - use src2 for both operands (the CPU
    // engine received a 0-dim `1` numerator as src1, which the recip kernel would ignore). [TODO.VP: tidy]
    ಆದರೆ (oclop == OCL_OP_RECIP_SCALE)
    {
        psrc1 = psrc2; kind1 = kind2; type1 = type2; depth1 = depth2; cn = cn2; dims1 = dims2; sz1 = sz2;
    }

    // add/sub/mul/div/absdiff/addWeighted/recip: the arithm OpenCL kernel with a coerced work type.
    ತಾರ್ಕಿಕ src1Scalar = checkScalar(*psrc1, type2, kind1, kind2);
    ತಾರ್ಕಿಕ src2Scalar = checkScalar(*psrc2, type1, kind2, kind1);
    ತಾರ್ಕಿಕ haveScalar = ಸುಳ್ಳು;

    ಆದರೆ (dims1 != dims2 || sz1 != sz2 || cn != cn2 ||
        (kind1 == ಒಳಹರಿವು_Array_2::MATX && (sz1 == ಗಾತ್ರ_2(1,4) || sz1 == ಗಾತ್ರ_2(1,1))) ||
        (kind2 == ಒಳಹರಿವು_Array_2::MATX && (sz2 == ಗಾತ್ರ_2(1,4) || sz2 == ಗಾತ್ರ_2(1,1))))
    {
        ಆದರೆ ((type1 == CV_64F && (sz1.ಎತ್ತರ == 1 || sz1.ಎತ್ತರ == 4)) && src1Scalar)
        {
            // src1 is a scalar; swap it with src2
            std::swap(psrc1, psrc2); std::swap(sz1, sz2); std::swap(type1, type2);
            std::swap(depth1, depth2); std::swap(cn, cn2); std::swap(dims1, dims2);
            ಆದರೆ (oclop == OCL_OP_SUB) oclop = OCL_OP_RSUB;
            ಆದರೆ (oclop == OCL_OP_DIV_SCALE) oclop = OCL_OP_RDIV_SCALE;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!src2Scalar)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;   // array op array with mismatched size/cn: engine broadcasts on the CPU
        haveScalar = ನಿಜ;
        CV_Assert((type2 == CV_64F || type2 == CV_32F) && (sz2.ಎತ್ತರ == 1 || sz2.ಎತ್ತರ == 4));

        ಆದರೆ (!muldiv)
        {
            Mat sc = psrc2->getMat();
            depth2 = actual_Scalar_ಆಳ(sc.ptr<ದ್ವಿಗುಣ>(), sz2 == ಗಾತ್ರ_2(1, 1) ? cn2 : cn);
            ಆದರೆ (depth2 == CV_64F && CV_ELEM_SIZE1(depth1) < 8)
                depth2 = CV_32F;
        }
        ಇಲ್ಲದಿದ್ದರೆ
            depth2 = CV_64F;
    }

    ಆದರೆ (dtype < 0)
    {
        ಆದರೆ (_dst.fixedType())
            dtype = _dst.type();
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ (!haveScalar && type1 != type2)
                CV_Error(cv::Error::StsBadArg,
                     "When the input arrays in add/subtract/multiply/divide functions have different types, "
                     "the output array type must be explicitly specified");
            dtype = type1;
        }
    }
    dtype = CV_MAT_ಆಳ(dtype);

    ಆದರೆ (depth1 == depth2 && dtype == depth1)
        wtype = dtype;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!muldiv)
    {
        wtype = coerceTypes(depth1, depth2, ಸುಳ್ಳು);
        wtype = coerceTypes(wtype, dtype, ಸುಳ್ಳು);
        ಆದರೆ (dtype < CV_32F && (depth1 < CV_32F || depth2 < CV_32F))
            wtype = CV_32S;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        wtype = coerceTypes(depth1, depth2, ನಿಜ);
        wtype = coerceTypes(wtype, dtype, ನಿಜ);
    }

    // The scaled OpenCL kernels (mul/div by a scale, reciprocal, addWeighted) compute in float; the old
    // same-type fast path forced max(depth, CV_32F). coerceTypes keeps same-type integer, so bump here.
    ಆದರೆ (oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE || oclop == OCL_OP_RDIV_SCALE ||
        oclop == OCL_OP_RECIP_SCALE || oclop == OCL_OP_ADDW)
        wtype = std::max(wtype, (ಪೂರ್ಣಾಂಕ)CV_32F);

    dtype = CV_MAKETYPE(dtype, cn);
    wtype = CV_MAKETYPE(wtype, cn);

    ಆದರೆ (have_ಮುಖವಾಡ)
    {
        ಪೂರ್ಣಾಂಕ mtype = ಮುಖವಾಡ_2.type();
        CV_Assert((mtype == CV_8UC1 || mtype == CV_8SC1 || mtype == CV_Bool) && ಮುಖವಾಡ_2.same_ಗಾತ್ರ(*psrc1));
        reallocate = !_dst.same_ಗಾತ್ರ(*psrc1) || _dst.type() != dtype;
    }

    _dst.create_Same_ಗಾತ್ರ(*psrc1, dtype);
    ಆದರೆ (reallocate)
        _dst.ಗಣ_ಗಮ್ಯ(0.);

    ಹಿಂತಿರುಗಿ ocl_arithm_op(*psrc1, *psrc2, _dst, ಮುಖವಾಡ_2, wtype, usrdata, oclop, haveScalar);
#ಆದರೆ_ಅಂತ್ಯ
}

// Element-wise binary op (ADD/SUB/...) on two CPU operands - each an array Mat or a MATX scalar -
// through the new broadcasting engine. Handles mixed input types (emitBinary inserts the casts) and
// broadcasting (a scalar rides as a 0-dim per-channel CONST; arrays broadcast via outputShape/exec).
// Returns false (declining) for UMat, a write-mask, or two scalar operands, so the caller falls
// through to arithm_op. Incompatible shapes make the engine throw (matching cv::add's error).
// Does the output array already have exactly this shape+type (=> create() reuses it, no realloc)?
// Works for Mat and UMat alike (shape/type queries, no data-pointer peeking).
ಸ್ಥಿರ ತಾರ್ಕಿಕ ಹೊರಹೋಗು_Array_Matches(ಸ್ಥಿರಾಂಕ _OutputArray& a, ಸ್ಥಿರಾಂಕ MatShape& shp, ಪೂರ್ಣಾಂಕ type)
{
    ಆದರೆ (a.empty() || a.type() != type)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಪೂರ್ಣಾಂಕ sz[MatShape::MAX_DIMS];
    ಪೂರ್ಣಾಂಕ nd = a.sizend(sz);
    ಆದರೆ (nd != (ಪೂರ್ಣಾಂಕ)shp.size())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nd; i++)
        ಆದರೆ (sz[i] != shp[i])
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

// Are two operands' iteration shapes (spatial dims + channels innermost, exactly how the engine
// iterates) numpy-broadcast-compatible? Powers the scalar-like-Mat compat fallback in arithm_op.
// Cheap by design: sizend copies the dim arrays inline - no Mat headers, no allocations.
ಸ್ಥಿರ ತಾರ್ಕಿಕ shapes_ಪ್ರಸಾರ_Compat(ಒಳಹರಿವು_Array a, ಪೂರ್ಣಾಂಕ acn, ಒಳಹರಿವು_Array b, ಪೂರ್ಣಾಂಕ bcn)
{
    ಪೂರ್ಣಾಂಕ asz[MatShape::MAX_DIMS + 1], bsz[MatShape::MAX_DIMS + 1];
    ಪೂರ್ಣಾಂಕ ad = a.sizend(asz), bd = b.sizend(bsz);
    asz[ad++] = acn;
    bsz[bd++] = bcn;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i1 = ad - 1, i2 = bd - 1; i1 >= 0 && i2 >= 0; i1--, i2--)
        ಆದರೆ (asz[i1] != bsz[i2] && asz[i1] != 1 && bsz[i2] != 1)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

// The one entry point for every element-wise binary op (add/sub/mul/div/min/max/absdiff/addWeighted/
// reciprocal). Handles empty inputs, the UMat/OpenCL branch (arithm_op_ocl) and the CPU element-wise
// engine. `oclop` selects the OpenCL kernel; `muldiv` drives the OpenCL working-type coercion + scale;
// `params` are the op scalars (params[0]=mul/div scale; {alpha,beta,gamma} for addWeighted).
// Below this many ELEMENTS (total * channels), the per-call constant overhead of the engine path
// (program build + compile + executor setup) is comparable to the actual work - take the direct
// kernel call instead. Same tier boundary as math_op's MATH_OP_SMALL for cv::exp.
ಎಣಿಕೆ { ARITHM_SMALL_ನೇರ = 100000 };

ಸ್ಥಿರ ಶೂನ್ಯ arithm_op(ew::ತುದಿ op, ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst,
                      ಒಳಹರಿವು_Array ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ dtype, ಪೂರ್ಣಾಂಕ oclop, ತಾರ್ಕಿಕ muldiv, ಸ್ಥಿರಾಂಕ Scalar& params)
{
    ಆದರೆ (src1.empty() || src2.empty())          // empty inputs -> empty result
    {
        CV_Assert((src1.empty() && src2.empty()) ||
                  (src1.empty() && isScalarArg(src2, src1.channels())) ||
                  (src2.empty() && isScalarArg(src1, src2.channels())));
        dst.release();
        ಆದರೆ (dtype >= 0)
            dst.create(0, 0, dtype);
        ಹಿಂತಿರುಗಿ;
    }

    // UMat -> classic OpenCL kernel. It declines (false) when OpenCL can't apply (no device, dims>2, a
    // broadcast, cn>4 masked, ...) - then the CPU engine below handles it (mapping the UMat via getMat).
    // oclop < 0 is a sentinel: the caller (e.g. cv::compare) already ran its own OpenCL path, so this
    // helper only runs the CPU engine (a UMat operand is mapped via getMat).
    ಆದರೆ (oclop >= 0 && cv::ocl::isOpenCLActivated() &&
        (src1.isUMat() || src2.isUMat() || dst.isUMat()))
    {
        ದ್ವಿಗುಣ abg[3] = { params[0], params[1], params[2] }, scale = params[0];
        ಶೂನ್ಯ* usrdata = (op == ew::OP_ADDW) ? (ಶೂನ್ಯ*)abg : (muldiv ? (ಶೂನ್ಯ*)&scale : ಶೂನ್ಯಸೂಚಕ);
        ಆದರೆ (arithm_op_ocl(src1, src2, dst, ಮುಖವಾಡ, dtype, oclop, muldiv, usrdata))
            ಹಿಂತಿರುಗಿ;
    }

    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ have_ಮುಖವಾಡ = !ಮುಖವಾಡ.empty();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn1 = src1.channels(), cn2 = src2.channels();
    ತಾರ್ಕಿಕ s1 = isScalarArg(src1, src2), s2 = isScalarArg(src2, src1);
    ಆದರೆ (s1 && s2)
        s1 = s2 = ಸುಳ್ಳು;    // two scalars: treat both as tiny array operands (element-wise + broadcast),
                            // matching arithm_op (Scalar+Scalar -> 4x1, number+number -> 1x1, ...)
    // A 4x1 CV_64F Mat/UMat pseudo-scalar (see isScalarArg) is hijacked only against a REAL array:
    // when the partner is itself a tiny scalar-shaped array (e.g. compare(Mat 4x1, Mat 1x1) -
    // issue #8999), both are honest data and ride the broadcast. A genuine MATX Scalar never demotes.
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s1 && src1.ಬಗೆ() != ಒಳಹರಿವು_Array_2::MATX && isScalarLikeMat(src2, cn1))
        s1 = ಸುಳ್ಳು;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s2 && src2.ಬಗೆ() != ಒಳಹರಿವು_Array_2::MATX && isScalarLikeMat(src1, cn2))
        s2 = ಸುಳ್ಳು;

    // Compat fallback: Java/Python/user code passes scalars as real little Mats (the classic 4x1
    // CV_64F column, a 1xcn/cnx1 vector - see isScalarLikeMat). Those normally ride broadcasting
    // now, so hijack one as a per-channel scalar ONLY when the shapes do NOT broadcast - just the
    // calls that would otherwise throw get the 4.x scalar semantics, every valid broadcast keeps
    // its numpy meaning. Cheap: two geometry probes, and the O(ndims) shape walk runs only when a
    // probe hits (never for ordinary same-size or MATX-scalar calls).
    ಆದರೆ (!s1 && !s2)
    {
        ತಾರ್ಕಿಕ like1 = isScalarLikeMat(src1, cn2), like2 = isScalarLikeMat(src2, cn1);
        ಆದರೆ ((like1 || like2) && !shapes_ಪ್ರಸಾರ_Compat(src1, cn1, src2, cn2))
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
    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* dstObj = (dst.ಬಗೆ() == ಒಳಹರಿವು_Array_2::MAT) ? dst.getObj() : ಶೂನ್ಯಸೂಚಕ;
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ aliased = dstObj && (dstObj == src1.getObj() || dstObj == src2.getObj());
    Mat m1loc, m2loc, mloc;
    ಸ್ವಯಂಚಾಲಿತ asMat = [&](ಒಳಹರಿವು_Array a, Mat& loc) -> ಸ್ಥಿರಾಂಕ Mat* {
        ಆದರೆ (a.ಬಗೆ() == ಒಳಹರಿವು_Array_2::MAT && !aliased) ಹಿಂತಿರುಗಿ (ಸ್ಥಿರಾಂಕ Mat*)a.getObj();
        loc = a.getMat(); ಹಿಂತಿರುಗಿ &loc;
    };
    ಸ್ಥಿರಾಂಕ Mat* pm1 = s1 ? ಶೂನ್ಯಸೂಚಕ : asMat(src1, m1loc);
    ಸ್ಥಿರಾಂಕ Mat* pm2 = s2 ? ಶೂನ್ಯಸೂಚಕ : asMat(src2, m2loc);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ adepth = s1 ? pm2->ಆಳ() : pm1->ಆಳ();   // the (first) array operand's depth
    // auto result depth (no explicit dtype, no fixed-type dst): add/sub/mul/div keep the first array
    // operand's depth (cv::'s dtype==-1 convention), but min/max/absdiff have NO dtype argument and
    // must promote mixed inputs to their common type (min(u32,f32) in u32 would drop the float). For
    // same-type inputs promoteArith(da,db)==da, so classic behaviour is preserved (e.g. absdiff of two
    // s16 stays s16, saturating - the wide unsigned |a-b| is computed then cast back down by emitBinary).
    ಪೂರ್ಣಾಂಕ auto_ಆಳ = adepth;
    ಆದರೆ (op == ew::OP_MIN || op == ew::OP_MAX || op == ew::OP_ABSDIFF)
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ d1 = s1 ? ew::EW_ಆಳ_NONE : pm1->ಆಳ();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ d2 = s2 ? ew::EW_ಆಳ_NONE : pm2->ಆಳ();
        auto_ಆಳ = ew::promoteArith(d1, d2);
    }
    // result depth: explicit dtype wins; else a fixed-type dst dictates it; else the auto depth above.
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ rdepth = dtype >= 0 ? CV_MAT_ಆಳ(dtype)
                     : (dst.fixedType() ? dst.ಆಳ() : auto_ಆಳ);

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
    ಆದರೆ (!have_ಮುಖವಾಡ && !s1 && !s2 &&
        (op == ew::OP_ಕೂಡಿಸು || op == ew::OP_SUB || op == ew::OP_MIN || op == ew::OP_MAX ||
         op == ew::OP_ABSDIFF || op == ew::OP_MUL || op == ew::OP_ADDW ||
         op == ew::OP_AND || op == ew::OP_OR || op == ew::OP_XOR) &&
        rdepth == pm1->ಆಳ() && pm1->type() == pm2->type() && pm1->size == pm2->size &&
        pm1->isContinuous() && pm2->isContinuous() &&
        pm1->ಒಟ್ಟು()*cn1 <= (size_t)ARITHM_SMALL_ನೇರ)
    {
        ew::TKernel k = ew::getElemwiseFunc(op, rdepth, rdepth, ew::EW_ಆಳ_NONE, rdepth);
        ಆದರೆ (k.fptr)
        {
            dst.create_Same_ಗಾತ್ರ(src1, pm1->type());   // whole-shape transfer, not piecemeal dims+sizes
            Mat dloc;
            Mat* pd = (dst.ಬಗೆ() == ಒಳಹರಿವು_Array_2::MAT) ? (Mat*)dst.getObj() : &(dloc = dst.getMat());
            ಆದರೆ (pd->isContinuous())     // a reused non-continuous dst view falls through to the engine
            {
                k.fptr(pm1->ದತ್ತಾಂಶ, 0, 1, pm2->ದತ್ತಾಂಶ, 0, 1, ಶೂನ್ಯಸೂಚಕ, 0, 0,
                       pd->ದತ್ತಾಂಶ, 0, (ಪೂರ್ಣಾಂಕ)(pm1->ಒಟ್ಟು()*cn1), 1, params.val, k.flags, k.userdata);
                ಹಿಂತಿರುಗಿ;
            }
        }
    }

    // operand order = (src1, src2): an array becomes an INPUT, a scalar a flexible per-channel CONST
    // read straight from the caller's inline storage (getObj()), no Mat / convertTo.
    ew::TExpr p;
    ಸ್ವಯಂಚಾಲಿತ ಕೂಡಿಸು_ಕಾರ್ಯಾಂಶ = [&](ಒಳಹರಿವು_Array src, ತಾರ್ಕಿಕ isScalar, ಸ್ಥಿರಾಂಕ Mat* m, ಪೂರ್ಣಾಂಕ otherCn) -> ಪೂರ್ಣಾಂಕ {
        ಆದರೆ (!isScalar)
            ಹಿಂತಿರುಗಿ p.ಕೂಡಿಸು_ಒಳಹರಿವು(m->ಆಳ());
        ಸ್ಥಿರಾಂಕ uchar* sp; ಪೂರ್ಣಾಂಕ sd;
        uchar scbuf[EW_SCALAR_BUF_ಗಾತ್ರ];   // stack room for a UMAT scalar; addConst copies the values out
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ scn0 = scalarArgElems(src, sp, sd, scbuf), cn = otherCn;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ scn = (cn == scn0 || (cn < 4 && scn0 == 4)) ? cn
                      : (scn0 == 1) ? 1 : 0;
        CV_Assert(scn > 0);
        ಹಿಂತಿರುಗಿ p.ಕೂಡಿಸು_Const(ew::EW_ಆಳ_NONE, sd, sp, scn);
    };
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ a0 = ಕೂಡಿಸು_ಕಾರ್ಯಾಂಶ(src1, s1, pm1, cn2);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ a1 = ಕೂಡಿಸು_ಕಾರ್ಯಾಂಶ(src2, s2, pm2, cn1);

    // Write-mask: a single-channel 1-byte array (u8/s8/bool) the size of the output spatial shape,
    // added as another broadcast input. The arithmetic result lands in a temp; a final
    // select(mask, r, dst) -> dst overwrites only mask!=0 positions of the (pre-existing) output.
    ಪೂರ್ಣಾಂಕ s_ಮುಖವಾಡ = 0;
    ಸ್ಥಿರಾಂಕ Mat* pmask = ಶೂನ್ಯಸೂಚಕ;
    ಆದರೆ (have_ಮುಖವಾಡ)
    {
        CV_Assert(ಮುಖವಾಡ.type() == CV_8U || ಮುಖವಾಡ.type() == CV_8S || ಮುಖವಾಡ.type() == CV_Bool);
        pmask = asMat(ಮುಖವಾಡ, mloc);
        s_ಮುಖವಾಡ = p.ಕೂಡಿಸು_ಒಳಹರಿವು(pmask->ಆಳ());
    }
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ s_ಹೊರಹೋಗು = p.ಕೂಡಿಸು_Output(rdepth);               // output BEFORE emit -> moveToOutput drops the
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ r = p.emit_ದ್ವಿಮಾನ(op, a0, a1, rdepth, params);   // result temp (mask-free => none); scale
                                                        // rides params[0] (mul/div), {a,b,g} for addW
    ಆದರೆ (have_ಮುಖವಾಡ)
        p.ಕೂಡಿಸು_Insn(ew::OP_SELECT, s_ಮುಖವಾಡ, r, s_ಹೊರಹೋಗು, s_ಹೊರಹೋಗು);
    ಇಲ್ಲದಿದ್ದರೆ
        p.move_ಗಮ್ಯ_Output(r, s_ಹೊರಹೋಗು);
    p.compile();

    ಸ್ಥಿರಾಂಕ Mat* inputs[3]; ಪೂರ್ಣಾಂಕ ni = 0;                   // inputs in program (input-index) order
    ಆದರೆ (!s1) inputs[ni++] = pm1;
    ಆದರೆ (!s2) inputs[ni++] = pm2;
    ಆದರೆ (have_ಮುಖವಾಡ) inputs[ni++] = pmask;

    // Pre-create the output at the broadcast shape, ALWAYS via dst.create() - never hand exec a raw
    // dst Mat. Routing through the _OutputArray is what enforces its contract: a FIXED_SIZE/FIXED_TYPE
    // dst (in-place 'a += b' passes '(const Mat&)a') throws here if a broadcast would change its shape
    // or type (Mat(1x1) += Mat(1x4)), instead of silently reallocating - exec sees only a Mat and has
    // no view of the array's fixed flags. Cost over a direct exec is negligible: exec keeps its own
    // fast path for a matching-shape op, and dst.create() with an unchanged shape/type is a no-op.
    // With a mask, a freshly (re)allocated dst is zeroed so mask==0 reads 0 (matches arithm_op); a
    // reused dst keeps its prior content there - detect reuse by shape+type BEFORE create (correct for
    // a reused UMat dst too).
    MatShape oshape; ಪೂರ್ಣಾಂಕ ocn;
    p.outputShape(inputs, oshape, ocn);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ otype = CV_MAKETYPE(rdepth, ocn);
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ reused = have_ಮುಖವಾಡ && ಹೊರಹೋಗು_Array_Matches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.ಬಗೆ() == ಒಳಹರಿವು_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    ಆದರೆ (have_ಮುಖವಾಡ && !reused)
        pdst->ಗಣ_Zero();                          // setZero (not setTo, which caps at 4 channels)
    p.exec(inputs, pdst);
}

// Unary element-wise op through the engine (bitwise NOT now; unary math later). One array input, an
// optional write-mask; result depth = dtype (>=0), else a fixed-type dst's depth, else the input
// depth. Mirrors arithm_op's build/mask/exec tail with a single input and no scalar operand. OpenCL is
// run by the caller (bitwise_op_ocl), so this is the CPU engine only.
ಸ್ಥಿರ ಶೂನ್ಯ unary_op(ew::ತುದಿ op, ಒಳಹರಿವು_Array src, OutputArray dst, ಒಳಹರಿವು_Array ಮುಖವಾಡ,
                     ಪೂರ್ಣಾಂಕ dtype, ಸ್ಥಿರಾಂಕ Scalar& params)
{
    ಆದರೆ (src.empty())
    {
        dst.release();
        ಆದರೆ (dtype >= 0)
            dst.create(0, 0, dtype);
        ಹಿಂತಿರುಗಿ;
    }

    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ have_ಮುಖವಾಡ = !ಮುಖವಾಡ.empty();
    // in-place (dst aliases src): keep a header copy so a shape/type-changing dst.create() can't free
    // src's data mid-op (matches arithm_op). A fixed-size/type dst never reallocs, so this is only for
    // an explicit dtype change.
    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* dstObj = (dst.ಬಗೆ() == ಒಳಹರಿವು_Array_2::MAT) ? dst.getObj() : ಶೂನ್ಯಸೂಚಕ;
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ aliased = dstObj && src.ಬಗೆ() == ಒಳಹರಿವು_Array_2::MAT && dstObj == src.getObj();
    Mat sloc, mloc;
    ಸ್ಥಿರಾಂಕ Mat* psrc = (src.ಬಗೆ() == ಒಳಹರಿವು_Array_2::MAT && !aliased) ? (ಸ್ಥಿರಾಂಕ Mat*)src.getObj()
                                                                   : &(sloc = src.getMat());
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ rdepth = dtype >= 0 ? CV_MAT_ಆಳ(dtype)
                     : (dst.fixedType() ? dst.ಆಳ() : psrc->ಆಳ());

    ew::TExpr p;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ a0 = p.ಕೂಡಿಸು_ಒಳಹರಿವು(psrc->ಆಳ());
    ಪೂರ್ಣಾಂಕ s_ಮುಖವಾಡ = 0;
    ಸ್ಥಿರಾಂಕ Mat* pmask = ಶೂನ್ಯಸೂಚಕ;
    ಆದರೆ (have_ಮುಖವಾಡ)
    {
        CV_Assert(ಮುಖವಾಡ.type() == CV_8U || ಮುಖವಾಡ.type() == CV_8S || ಮುಖವಾಡ.type() == CV_Bool);
        pmask = (ಮುಖವಾಡ.ಬಗೆ() == ಒಳಹರಿವು_Array_2::MAT) ? (ಸ್ಥಿರಾಂಕ Mat*)ಮುಖವಾಡ.getObj() : &(mloc = ಮುಖವಾಡ.getMat());
        s_ಮುಖವಾಡ = p.ಕೂಡಿಸು_ಒಳಹರಿವು(pmask->ಆಳ());
    }
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ s_ಹೊರಹೋಗು = p.ಕೂಡಿಸು_Output(rdepth);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ r = p.emitUnary(op, a0, rdepth, params);
    ಆದರೆ (have_ಮುಖವಾಡ)
        p.ಕೂಡಿಸು_Insn(ew::OP_SELECT, s_ಮುಖವಾಡ, r, s_ಹೊರಹೋಗು, s_ಹೊರಹೋಗು);
    ಇಲ್ಲದಿದ್ದರೆ
        p.move_ಗಮ್ಯ_Output(r, s_ಹೊರಹೋಗು);
    p.compile();

    ಸ್ಥಿರಾಂಕ Mat* inputs[2]; ಪೂರ್ಣಾಂಕ ni = 0;
    inputs[ni++] = psrc;
    ಆದರೆ (have_ಮುಖವಾಡ) inputs[ni++] = pmask;

    MatShape oshape; ಪೂರ್ಣಾಂಕ ocn;
    p.outputShape(inputs, oshape, ocn);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ otype = CV_MAKETYPE(rdepth, ocn);
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ reused = have_ಮುಖವಾಡ && ಹೊರಹೋಗು_Array_Matches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.ಬಗೆ() == ಒಳಹರಿವು_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    ಆದರೆ (have_ಮುಖವಾಡ && !reused)
        pdst->ಗಣ_Zero();
    p.exec(inputs, pdst);
}

}

ಶೂನ್ಯ cv::ಕೂಡಿಸು( ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst,
          ಒಳಹರಿವು_Array ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ಕೂಡಿಸು, src1, src2, dst, ಮುಖವಾಡ, dtype, OCL_OP_ಕೂಡಿಸು, ಸುಳ್ಳು);
}

ಶೂನ್ಯ cv::ಕಳೆ( ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst,
                   ಒಳಹರಿವು_Array ಮುಖವಾಡ, ಪೂರ್ಣಾಂಕ dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_SUB, src1, src2, dst, ಮುಖವಾಡ, dtype, OCL_OP_SUB, ಸುಳ್ಳು);
}

ಶೂನ್ಯ cv::absdiff( ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ABSDIFF, src1, src2, dst, noArray(), -1, OCL_OP_ABSDIFF, ಸುಳ್ಳು);
}

ಶೂನ್ಯ cv::copy_ಗಮ್ಯ(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2)
{
    CV_INSTRUMENT_REGION();

    _src.copy_ಗಮ್ಯ(_dst, ಮುಖವಾಡ_2);
}

/****************************************************************************************\
*                                    multiply/divide                                     *
\****************************************************************************************/

ನಾಮಾಕಾಶ cv
{

ಶೂನ್ಯ multiply(ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2,
              OutputArray dst, ದ್ವಿಗುಣ scale, ಪೂರ್ಣಾಂಕ dtype)
{
    CV_INSTRUMENT_REGION();

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ oclop = std::ನಿರಪೇಕ್ಷ(scale - 1.0) < DBL_EPSILON ? OCL_OP_MUL : OCL_OP_MUL_SCALE;
    arithm_op(ew::OP_MUL, src1, src2, dst, noArray(), dtype, oclop, /*muldiv*/ ನಿಜ, Scalar(scale));
}

ಶೂನ್ಯ divide(ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2,
                OutputArray dst, ದ್ವಿಗುಣ scale, ಪೂರ್ಣಾಂಕ dtype)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_DIV, src1, src2, dst, noArray(), dtype, OCL_OP_DIV_SCALE, /*muldiv*/ ನಿಜ, Scalar(scale));
}

ಶೂನ್ಯ divide(ದ್ವಿಗುಣ scale, ಒಳಹರಿವು_Array src2,
                OutputArray dst, ಪೂರ್ಣಾಂಕ dtype)
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ (src2.empty())
    {
        dst.release();
        ಹಿಂತಿರುಗಿ;
    }

    // scale / src2 == scale * 1 / src2: feed a 0-dim `1` as the numerator so the CPU engine reuses the
    // normal divide (scale*a/b). Give the numerator src2's OWN depth: then OP_DIV(T, T) is same-type and
    // takes the fast per-type kernel (f32 work for <=16-bit / f16 / bf16, not the s32->f64 path a CV_32S
    // numerator would force). An integer src2 keeps its div-by-zero->0 guard (both operands integer); a
    // float src2 divides as float (1/0->inf), both matching cv::divide. dtype<0 follows src2's depth (a
    // fixed-type dst still wins). The UMat path uses the dedicated reciprocal kernel (OCL_OP_RECIP_SCALE).
    ದ್ವಿಗುಣ one = 1;
    Mat numerator(MatShape::scalar(), src2.ಆಳ(), &one);
    scalar_ಗಮ್ಯ_Raw_ದತ್ತಾಂಶ(Scalar(1), &one, src2.ಆಳ(), 1);       // write `1` in src2's depth (no allocation)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ rtype = (dtype < 0 && !dst.fixedType()) ? src2.ಆಳ() : dtype;
    arithm_op(ew::OP_DIV, numerator, src2, dst, noArray(), rtype, OCL_OP_RECIP_SCALE, /*muldiv*/ ನಿಜ, Scalar(scale));
}

UMat UMat::mul(ಒಳಹರಿವು_Array m, ದ್ವಿಗುಣ scale) ಸ್ಥಿರಾಂಕ
{
    UMat dst;
    multiply(*ಇದು, m, dst, scale);
    ಹಿಂತಿರುಗಿ dst;
}

/****************************************************************************************\
*                                      addWeighted                                       *
\****************************************************************************************/

}

ಶೂನ್ಯ cv::ಕೂಡಿಸು_Weighted( ಒಳಹರಿವು_Array src1, ದ್ವಿಗುಣ alpha, ಒಳಹರಿವು_Array src2,
                      ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ gamma, OutputArray dst, ಪೂರ್ಣಾಂಕ dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADDW, src1, src2, dst, noArray(), dtype, OCL_OP_ADDW, /*muldiv*/ ನಿಜ,
              Scalar(alpha, beta, gamma));
}


/****************************************************************************************\
*                                          compare                                       *
\****************************************************************************************/

ನಾಮಾಕಾಶ cv
{


ಸ್ಥಿರ ದ್ವಿಗುಣ getMinVal(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ tab[CV_ಆಳ_MAX] =
    {
        0, -128, 0, -32768, INT_MIN, -FLT_MAX, -DBL_MAX,
        -65504, -FLT_MAX, 0, 0, (ದ್ವಿಗುಣ)INT64_MIN, 0
    };
    ಹಿಂತಿರುಗಿ tab[ಆಳ];
}

ಸ್ಥಿರ ದ್ವಿಗುಣ getMaxVal(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ tab[CV_ಆಳ_MAX] = {
        255, 127, 65535, 32767, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, FLT_MAX, DBL_MAX,
        65504, FLT_MAX, 255, (ದ್ವಿಗುಣ)UINT64_MAX, (ದ್ವಿಗುಣ)INT64_MAX, (ದ್ವಿಗುಣ)UINT32_MAX, 0
    };
    ಹಿಂತಿರುಗಿ tab[ಆಳ];
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_compare(ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2, OutputArray _dst, ಪೂರ್ಣಾಂಕ op, ತಾರ್ಕಿಕ haveScalar)
{
    ಸ್ಥಿರಾಂಕ ocl::Device& dev = ocl::Device::getDefault();
    ತಾರ್ಕಿಕ doubleSupport = dev.doubleFPConfig() > 0;
    ಪೂರ್ಣಾಂಕ type1 = _src1.type(), depth1 = CV_MAT_ಆಳ(type1), cn = CV_MAT_CN(type1),
            type2 = _src2.type(), depth2 = CV_MAT_ಆಳ(type2);

    ಆದರೆ (!doubleSupport && depth1 == CV_64F)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    // the integer scalar path below works in int; let the CPU handle the wider 5.x integer depths
    ಆದರೆ (haveScalar && (depth1 == CV_32U || depth1 == CV_64U || depth1 == CV_64S))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ (!haveScalar && (!_src1.same_ಗಾತ್ರ(_src2) || type1 != type2))
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    // This OpenCL kernel broadcasts a scalar's FIRST channel to all channels. The CPU path compares
    // per-channel (scalar[c] for channel c), so a multichannel array against a scalar with DISTINCT
    // channel values would disagree - bail to the CPU engine in that case (rare; not worth a kernel fix).
    ಆದರೆ (haveScalar && cn > 1)
    {
        Mat sc = _src2.getMat();
        ಪೂರ್ಣಾಂಕ scn = (ಪೂರ್ಣಾಂಕ)sc.ಒಟ್ಟು();
        ಆದರೆ (scn > 1)
        {
            ದ್ವಿಗುಣ v[4] = { 0, 0, 0, 0 };
            getConvertFunc(sc.ಆಳ(), CV_64F)(sc.ptr(), 1, 0, 1, (uchar*)v, 1, ಗಾತ್ರ_2(std::min(scn, 4), 1), 0);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1, n = std::min(cn, scn); i < n; i++)
                ಆದರೆ (v[i] != v[0])
                    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
    }

    ಪೂರ್ಣಾಂಕ kercn = haveScalar ? cn : ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(_src1, _src2, _dst), ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = dev.isIntel() ? 4 : 1;
    // Workaround for bug with "?:" operator in AMD OpenCL compiler
    ಆದರೆ (depth1 >= CV_16U)
        kercn = 1;

    ಪೂರ್ಣಾಂಕ scalarcn = kercn == 3 ? 4 : kercn;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ operationMap[] = { "==", ">", ">=", "<", "<=", "!=" };
    ಅಕ್ಷರ cvt[50];

    String opts = format("-D %s -D srcT1=%s -D dstT=%s -D DEPTH_dst=%d -D workT=srcT1 -D cn=%d"
                         " -D convertToDT=%s -D OP_CMP -D CMP_OPERATOR=%s -D srcT1_C1=%s"
                         " -D srcT2_C1=%s -D dstT_C1=%s -D workST=%s -D rowsPerWI=%d%s",
                         haveScalar ? "UNARY_OP" : "BINARY_OP",
                         ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(depth1, kercn)),
                         ocl::type_ಗಮ್ಯ_Str(CV_8UC(kercn)), CV_8U, kercn,
                         ocl::convertTypeStr(depth1, CV_8U, kercn, cvt, ಗಾತ್ರ(cvt)),
                         operationMap[op], ocl::type_ಗಮ್ಯ_Str(depth1),
                         ocl::type_ಗಮ್ಯ_Str(depth1), ocl::type_ಗಮ್ಯ_Str(CV_8U),
                         ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(depth1, scalarcn)), ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src1 = _src1.getUMat();
    ಗಾತ್ರ_2 size = src1.size();
    _dst.create(size, CV_8UC(cn));
    UMat dst = _dst.getUMat();

    ಆದರೆ (haveScalar)
    {
        size_t esz = CV_ELEM_SIZE1(type1) * scalarcn;
        ದ್ವಿಗುಣ buf[4] = { 0, 0, 0, 0 };
        Mat src2 = _src2.getMat();

        ಆದರೆ( depth1 > CV_32S )
            convertAndUnrollScalar( src2, depth1, (uchar *)buf, kercn );
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ದ್ವಿಗುಣ fval = 0;
            getConvertFunc(depth2, CV_64F)(src2.ptr(), 1, 0, 1, (uchar *)&fval, 1, ಗಾತ್ರ_2(1, 1), 0);
            ಆದರೆ( fval < getMinVal(depth1) )
                ಹಿಂತಿರುಗಿ dst.ಗಣ_ಗಮ್ಯ(Scalar::all(op == CMP_GT || op == CMP_GE || op == CMP_NE ? 255 : 0)), ನಿಜ;

            ಆದರೆ( fval > getMaxVal(depth1) )
                ಹಿಂತಿರುಗಿ dst.ಗಣ_ಗಮ್ಯ(Scalar::all(op == CMP_LT || op == CMP_LE || op == CMP_NE ? 255 : 0)), ನಿಜ;

            ಪೂರ್ಣಾಂಕ ival = cv_ಸುತ್ತು(fval);
            ಆದರೆ( fval != ival )
            {
                ಆದರೆ( op == CMP_LT || op == CMP_GE )
                    ival = cvCeil(fval);
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( op == CMP_LE || op == CMP_GT )
                    ival = cvFloor(fval);
                ಇಲ್ಲದಿದ್ದರೆ
                    ಹಿಂತಿರುಗಿ dst.ಗಣ_ಗಮ್ಯ(Scalar::all(op == CMP_NE ? 255 : 0)), ನಿಜ;
            }
            convertAndUnrollScalar(Mat(1, 1, CV_32S, &ival), depth1, (uchar *)buf, kercn);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src1, cn, kercn),
               ocl::KernelArg::WriteOnly(dst, cn, kercn), scalararg);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        UMat src2 = _src2.getUMat();

        k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src1),
               ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src2),
               ocl::KernelArg::WriteOnly(dst, cn, kercn));
    }

    size_t globalsize[2] = { (size_t)dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn / kercn, ((size_t)dst.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

}

ಶೂನ್ಯ cv::compare(ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2, OutputArray _dst, ಪೂರ್ಣಾಂಕ op)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( op == CMP_LT || op == CMP_LE || op == CMP_EQ ||
               op == CMP_NE || op == CMP_GE || op == CMP_GT );

    CV_Assert(_src1.empty() == _src2.empty());
    ಆದರೆ (_src1.empty() && _src2.empty())
    {
        _dst.release();
        ಹಿಂತಿರುಗಿ;
    }

    ತಾರ್ಕಿಕ haveScalar = ಸುಳ್ಳು;

    ಆದರೆ ((_src1.isMatx() + _src2.isMatx()) == 1
            || !_src1.same_ಗಾತ್ರ(_src2)
            || _src1.type() != _src2.type())
    {
        ತಾರ್ಕಿಕ is_src1_scalar = checkScalar(_src1, _src2.type(), _src1.ಬಗೆ(), _src2.ಬಗೆ());
        ತಾರ್ಕಿಕ is_src2_scalar = checkScalar(_src2, _src1.type(), _src2.ಬಗೆ(), _src1.ಬಗೆ());

        // exactly one scalar operand: keep it as src2 (swapping flips the ordering ops) so the OpenCL
        // path sees the array-op-scalar form. Otherwise - two arrays of different size (broadcast),
        // mixed types, or two scalars - the CPU engine broadcasts / promotes them, so there is NO error
        // here: fall through to arithm_op (which handles scalar detection, broadcast and mixed types).
        ಆದರೆ (is_src1_scalar && !is_src2_scalar)
        {
            op = op == CMP_LT ? CMP_GT : op == CMP_LE ? CMP_GE :
                op == CMP_GE ? CMP_LE : op == CMP_GT ? CMP_LT : op;
            // src1 is a scalar; swap it with src2
            compare(_src2, _src1, _dst, op);
            ಹಿಂತಿರುಗಿ;
        }
        haveScalar = is_src2_scalar && !is_src1_scalar;
    }
    CV_ಬಳಸದ(haveScalar);   // consumed by CV_OCL_RUN only - a no-op without OpenCL

    // OpenCL handles only the same-size and array-op-scalar forms; a broadcast between two arrays of
    // different size falls through to the CPU engine.
    CV_OCL_ಸತತ(_src1.dims() <= 2 && _src2.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat())
               && (haveScalar || _src1.same_ಗಾತ್ರ(_src2)),
               ocl_compare(_src1, _src2, _dst, op, haveScalar))

    // CPU: the element-wise engine. A CMP_* code maps to the corresponding OP_CMP_* TOp; the engine
    // forces a u8 mask (dtype = CV_8U) and compares in the common operand type (a fractional scalar
    // threshold promotes both operands to float - matching the classic ceil/floor threshold logic).
    // The scalar / broadcast / mixed-shape cases are handled inside arithm_op. oclop = -1 tells it the
    // OpenCL path was already tried above (via ocl_compare), so it only runs the CPU engine.
    ew::ತುದಿ cmpOp = op == CMP_EQ ? ew::OP_CMP_EQ : op == CMP_NE ? ew::OP_CMP_NE :
                    op == CMP_LT ? ew::OP_CMP_LT : op == CMP_LE ? ew::OP_CMP_LE :
                    op == CMP_GT ? ew::OP_CMP_GT : ew::OP_CMP_GE;
    arithm_op(cmpOp, _src1, _src2, _dst, noArray(), CV_8U, /*oclop=*/-1, /*muldiv=*/ಸುಳ್ಳು);
}

/****************************************************************************************\
*                                        inRange                                         *
\****************************************************************************************/

ನಾಮಾಕಾಶ cv
{

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
ರಚನೆ ಒಳ_Range_SIMD
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ T * src1, ಸ್ಥಿರಾಂಕ T * src2, ಸ್ಥಿರಾಂಕ T * src3, uchar * dst, ಪೂರ್ಣಾಂಕ len) ಸ್ಥಿರಾಂಕ
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < len; x++)
            dst[x] = (src2[x] <= src1[x] && src1[x] <= src3[x]) ? 255 : 0;
        ಹಿಂತಿರುಗಿ len;
    }
};

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)

ಮಾದರಿ <>
ರಚನೆ ಒಳ_Range_SIMD<uchar>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ uchar * src1, ಸ್ಥಿರಾಂಕ uchar * src2, ಸ್ಥಿರಾಂಕ uchar * src3,
        uchar * dst, ಪೂರ್ಣಾಂಕ len) ಸ್ಥಿರಾಂಕ
    {
        ಪೂರ್ಣಾಂಕ x = 0;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = VTraits<v_uint8>::vlanes();

        ಗಾಗಿ (; x <= len - ಅಗಲ; x += ಅಗಲ)
        {
            v_uint8 values = vx_ತುಂಬು(src1 + x);
            v_uint8 ಕೆಳಗಿನ = vx_ತುಂಬು(src2 + x);
            v_uint8 ಮೇಲಿನ = vx_ತುಂಬು(src3 + x);

            v_store(dst + x, v_and(v_ge(values, ಕೆಳಗಿನ), v_ge(ಮೇಲಿನ, values)));
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ x;
    }
};

ಮಾದರಿ <>
ರಚನೆ ಒಳ_Range_SIMD<schar>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ schar * src1, ಸ್ಥಿರಾಂಕ schar * src2, ಸ್ಥಿರಾಂಕ schar * src3,
        uchar * dst, ಪೂರ್ಣಾಂಕ len) ಸ್ಥಿರಾಂಕ
    {
        ಪೂರ್ಣಾಂಕ x = 0;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = VTraits<v_int8>::vlanes();

        ಗಾಗಿ (; x <= len - ಅಗಲ; x += ಅಗಲ)
        {
            v_int8 values = vx_ತುಂಬು(src1 + x);
            v_int8 ಕೆಳಗಿನ = vx_ತುಂಬು(src2 + x);
            v_int8 ಮೇಲಿನ = vx_ತುಂಬು(src3 + x);

            v_store((schar*)(dst + x), v_and(v_ge(values, ಕೆಳಗಿನ), v_ge(ಮೇಲಿನ, values)));
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ x;
    }
};

ಮಾದರಿ <>
ರಚನೆ ಒಳ_Range_SIMD<ushort>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ ushort * src1, ಸ್ಥಿರಾಂಕ ushort * src2, ಸ್ಥಿರಾಂಕ ushort * src3,
        uchar * dst, ಪೂರ್ಣಾಂಕ len) ಸ್ಥಿರಾಂಕ
    {
        ಪೂರ್ಣಾಂಕ x = 0;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = VTraits<v_uint16>::vlanes() * 2;

        ಗಾಗಿ (; x <= len - ಅಗಲ; x += ಅಗಲ)
        {
            v_uint16 values1 = vx_ತುಂಬು(src1 + x);
            v_uint16 low1 = vx_ತುಂಬು(src2 + x);
            v_uint16 high1 = vx_ತುಂಬು(src3 + x);

            v_uint16 values2 = vx_ತುಂಬು(src1 + x + VTraits<v_uint16>::vlanes());
            v_uint16 low2 = vx_ತುಂಬು(src2 + x + VTraits<v_uint16>::vlanes());
            v_uint16 high2 = vx_ತುಂಬು(src3 + x + VTraits<v_uint16>::vlanes());

            v_store(dst + x, v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2))));
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ x;
    }
};

ಮಾದರಿ <>
ರಚನೆ ಒಳ_Range_SIMD<ಗಿಡ್ಡ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ * src1, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ * src2, ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ * src3,
        uchar * dst, ಪೂರ್ಣಾಂಕ len) ಸ್ಥಿರಾಂಕ
    {
        ಪೂರ್ಣಾಂಕ x = 0;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = (ಪೂರ್ಣಾಂಕ)VTraits<v_int16>::vlanes() * 2;

        ಗಾಗಿ (; x <= len - ಅಗಲ; x += ಅಗಲ)
        {
            v_int16 values1 = vx_ತುಂಬು(src1 + x);
            v_int16 low1 = vx_ತುಂಬು(src2 + x);
            v_int16 high1 = vx_ತುಂಬು(src3 + x);

            v_int16 values2 = vx_ತುಂಬು(src1 + x + VTraits<v_int16>::vlanes());
            v_int16 low2 = vx_ತುಂಬು(src2 + x + VTraits<v_int16>::vlanes());
            v_int16 high2 = vx_ತುಂಬು(src3 + x + VTraits<v_int16>::vlanes());

            v_store((schar*)(dst + x), v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2))));
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ x;
    }
};

ಮಾದರಿ <>
ರಚನೆ ಒಳ_Range_SIMD<ಪೂರ್ಣಾಂಕ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ * src1, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ * src2, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ * src3,
        uchar * dst, ಪೂರ್ಣಾಂಕ len) ಸ್ಥಿರಾಂಕ
    {
        ಪೂರ್ಣಾಂಕ x = 0;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = (ಪೂರ್ಣಾಂಕ)VTraits<v_int32>::vlanes() * 2;

        ಗಾಗಿ (; x <= len - ಅಗಲ; x += ಅಗಲ)
        {
            v_int32 values1 = vx_ತುಂಬು(src1 + x);
            v_int32 low1 = vx_ತುಂಬು(src2 + x);
            v_int32 high1 = vx_ತುಂಬು(src3 + x);

            v_int32 values2 = vx_ತುಂಬು(src1 + x + VTraits<v_int32>::vlanes());
            v_int32 low2 = vx_ತುಂಬು(src2 + x + VTraits<v_int32>::vlanes());
            v_int32 high2 = vx_ತುಂಬು(src3 + x + VTraits<v_int32>::vlanes());

            v_pack_store(dst + x, v_reinterpret_as_u16(v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2)))));
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ x;
    }
};

ಮಾದರಿ <>
ರಚನೆ ಒಳ_Range_SIMD<ದಶಮಾಂಶ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ * src1, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ * src2, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ * src3,
        uchar * dst, ಪೂರ್ಣಾಂಕ len) ಸ್ಥಿರಾಂಕ
    {
        ಪೂರ್ಣಾಂಕ x = 0;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = (ಪೂರ್ಣಾಂಕ)VTraits<v_float32>::vlanes() * 2;

        ಗಾಗಿ (; x <= len - ಅಗಲ; x += ಅಗಲ)
        {
            v_float32 values1 = vx_ತುಂಬು(src1 + x);
            v_float32 low1 = vx_ತುಂಬು(src2 + x);
            v_float32 high1 = vx_ತುಂಬು(src3 + x);

            v_float32 values2 = vx_ತುಂಬು(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_ತುಂಬು(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_ತುಂಬು(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ x;
    }
};

ಮಾದರಿ <>
ರಚನೆ ಒಳ_Range_SIMD<hfloat>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ hfloat * src1, ಸ್ಥಿರಾಂಕ hfloat * src2, ಸ್ಥಿರಾಂಕ hfloat * src3,
        uchar * dst, ಪೂರ್ಣಾಂಕ len) ಸ್ಥಿರಾಂಕ
    {
        ಪೂರ್ಣಾಂಕ x = 0;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = (ಪೂರ್ಣಾಂಕ)VTraits<v_float32>::vlanes()*2;

        ಗಾಗಿ (; x <= len - ಅಗಲ; x += ಅಗಲ)
        {
            v_float32 values1 = vx_ತುಂಬು_expand(src1 + x);
            v_float32 low1 = vx_ತುಂಬು_expand(src2 + x);
            v_float32 high1 = vx_ತುಂಬು_expand(src3 + x);

            v_float32 values2 = vx_ತುಂಬು_expand(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_ತುಂಬು_expand(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_ತುಂಬು_expand(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ x;
    }
};

ಮಾದರಿ <>
ರಚನೆ ಒಳ_Range_SIMD<bfloat>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ bfloat * src1, ಸ್ಥಿರಾಂಕ bfloat * src2, ಸ್ಥಿರಾಂಕ bfloat * src3,
        uchar * dst, ಪೂರ್ಣಾಂಕ len) ಸ್ಥಿರಾಂಕ
    {
        ಪೂರ್ಣಾಂಕ x = 0;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = (ಪೂರ್ಣಾಂಕ)VTraits<v_float32>::vlanes()*2;

        ಗಾಗಿ (; x <= len - ಅಗಲ; x += ಅಗಲ)
        {
            v_float32 values1 = vx_ತುಂಬು_expand(src1 + x);
            v_float32 low1 = vx_ತುಂಬು_expand(src2 + x);
            v_float32 high1 = vx_ತುಂಬು_expand(src3 + x);

            v_float32 values2 = vx_ತುಂಬು_expand(src1 + x + VTraits<v_float32>::vlanes());
            v_float32 low2 = vx_ತುಂಬು_expand(src2 + x + VTraits<v_float32>::vlanes());
            v_float32 high2 = vx_ತುಂಬು_expand(src3 + x + VTraits<v_float32>::vlanes());

            v_pack_store(dst + x, v_pack(v_and(v_reinterpret_as_u32(v_ge(values1, low1)), v_reinterpret_as_u32(v_ge(high1, values1))),
                                         v_and(v_reinterpret_as_u32(v_ge(values2, low2)), v_reinterpret_as_u32(v_ge(high2, values2)))));
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ x;
    }
};

ಮಾದರಿ <>
ರಚನೆ ಒಳ_Range_SIMD<ಚಿಹ್ನೆರಹಿತ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ * src1, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ * src2, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ * src3,
        uchar * dst, ಪೂರ್ಣಾಂಕ len) ಸ್ಥಿರಾಂಕ
    {
        ಪೂರ್ಣಾಂಕ x = 0;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = (ಪೂರ್ಣಾಂಕ)VTraits<v_uint32>::vlanes() * 2;

        ಗಾಗಿ (; x <= len - ಅಗಲ; x += ಅಗಲ)
        {
            v_uint32 values1 = vx_ತುಂಬು(src1 + x);
            v_uint32 low1 = vx_ತುಂಬು(src2 + x);
            v_uint32 high1 = vx_ತುಂಬು(src3 + x);

            v_uint32 values2 = vx_ತುಂಬು(src1 + x + VTraits<v_uint32>::vlanes());
            v_uint32 low2 = vx_ತುಂಬು(src2 + x + VTraits<v_uint32>::vlanes());
            v_uint32 high2 = vx_ತುಂಬು(src3 + x + VTraits<v_uint32>::vlanes());

            v_pack_store(dst + x, v_reinterpret_as_u16(v_pack(v_and(v_ge(values1, low1), v_ge(high1, values1)), v_and(v_ge(values2, low2), v_ge(high2, values2)))));
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ x;
    }
};

#ಆದರೆ CV_SIMD_64F

ಮಾದರಿ <>
ರಚನೆ ಒಳ_Range_SIMD<ದ್ವಿಗುಣ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ () (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ * src1, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ * src2, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ * src3,
        uchar * dst, ಪೂರ್ಣಾಂಕ len) ಸ್ಥಿರಾಂಕ
    {
        ಪೂರ್ಣಾಂಕ x = 0;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ = VTraits<v_float64>::vlanes();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = ಹೆಜ್ಜೆ * 4;

        ಗಾಗಿ (; x <= len - ಅಗಲ; x += ಅಗಲ)
        {
            v_float64 v1 = vx_ತುಂಬು(src1 + x);
            v_float64 l1 = vx_ತುಂಬು(src2 + x);
            v_float64 h1 = vx_ತುಂಬು(src3 + x);
            v_uint64 m1 = v_reinterpret_as_u64(v_and(v_ge(v1, l1), v_ge(h1, v1)));

            v_float64 v2 = vx_ತುಂಬು(src1 + x + ಹೆಜ್ಜೆ);
            v_float64 l2 = vx_ತುಂಬು(src2 + x + ಹೆಜ್ಜೆ);
            v_float64 h2 = vx_ತುಂಬು(src3 + x + ಹೆಜ್ಜೆ);
            v_uint64 m2 = v_reinterpret_as_u64(v_and(v_ge(v2, l2), v_ge(h2, v2)));

            v_float64 v3 = vx_ತುಂಬು(src1 + x + ಹೆಜ್ಜೆ * 2);
            v_float64 l3 = vx_ತುಂಬು(src2 + x + ಹೆಜ್ಜೆ * 2);
            v_float64 h3 = vx_ತುಂಬು(src3 + x + ಹೆಜ್ಜೆ * 2);
            v_uint64 m3 = v_reinterpret_as_u64(v_and(v_ge(v3, l3), v_ge(h3, v3)));

            v_float64 v4 = vx_ತುಂಬು(src1 + x + ಹೆಜ್ಜೆ * 3);
            v_float64 l4 = vx_ತುಂಬು(src2 + x + ಹೆಜ್ಜೆ * 3);
            v_float64 h4 = vx_ತುಂಬು(src3 + x + ಹೆಜ್ಜೆ * 3);
            v_uint64 m4 = v_reinterpret_as_u64(v_and(v_ge(v4, l4), v_ge(h4, v4)));


            v_pack_store(dst + x, v_pack(v_pack(m1, m2), v_pack(m3, m4)));
        }
        vx_cleanup();
        ಹಿಂತಿರುಗಿ x;
    }
};

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಶೂನ್ಯ ಒಳ_Range(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src1, size_t step1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src2, size_t step2,
         ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src3, size_t step3, uchar* dst, size_t ಹೆಜ್ಜೆ,
         ಗಾತ್ರ_2 size)
{
    ಸ್ಥಿರಾಂಕ T* src1 = (ಸ್ಥಿರಾಂಕ T*)_src1;
    ಸ್ಥಿರಾಂಕ T* src2 = (ಸ್ಥಿರಾಂಕ T*)_src2;
    ಸ್ಥಿರಾಂಕ T* src3 = (ಸ್ಥಿರಾಂಕ T*)_src3;
    step1 /= ಗಾತ್ರ(src1[0]);
    step2 /= ಗಾತ್ರ(src2[0]);
    step3 /= ಗಾತ್ರ(src3[0]);

    ಒಳ_Range_SIMD<T> vop;

    ಗಾಗಿ( ; size.ಎತ್ತರ--; src1 += step1, src2 += step2, src3 += step3, dst += ಹೆಜ್ಜೆ )
    {
        ಪೂರ್ಣಾಂಕ x = vop(src1, src2, src3, dst, size.ಅಗಲ);
        #ಆದರೆ CV_ENABLE_UNROLLED
        ಗಾಗಿ( ; x <= size.ಅಗಲ - 4; x += 4 )
        {
            ಪೂರ್ಣಾಂಕ t0, t1;
            t0 = src2[x] <= src1[x] && src1[x] <= src3[x];
            t1 = src2[x+1] <= src1[x+1] && src1[x+1] <= src3[x+1];
            dst[x] = (uchar)-t0; dst[x+1] = (uchar)-t1;
            t0 = src2[x+2] <= src1[x+2] && src1[x+2] <= src3[x+2];
            t1 = src2[x+3] <= src1[x+3] && src1[x+3] <= src3[x+3];
            dst[x+2] = (uchar)-t0; dst[x+3] = (uchar)-t1;
        }
        #ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ( ; x < size.ಅಗಲ; x++ )
            dst[x] = (uchar)-(src2[x] <= src1[x] && src1[x] <= src3[x]);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ ಒಳ_Range_Reduce(ಸ್ಥಿರಾಂಕ uchar* src, uchar* dst, size_t len, ಪೂರ್ಣಾಂಕ cn)
{
    ಪೂರ್ಣಾಂಕ k = cn % 4 ? cn % 4 : 4;
    size_t i, j;
    ಆದರೆ( k == 1 )
        ಗಾಗಿ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j];
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == 2 )
        ಗಾಗಿ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1];
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == 3 )
        ಗಾಗಿ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2];
    ಇಲ್ಲದಿದ್ದರೆ
        ಗಾಗಿ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2] & src[j+3];

    ಗಾಗಿ( ; k < cn; k += 4 )
    {
        ಗಾಗಿ( i = 0, j = k; i < len; i++, j += cn )
            dst[i] &= src[j] & src[j+1] & src[j+2] & src[j+3];
    }
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*ಒಳ_Range_Func)( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, size_t step1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, size_t step2,
                             ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src3, size_t step3, uchar* dst, size_t ಹೆಜ್ಜೆ, ಗಾತ್ರ_2 sz );

ಸ್ಥಿರ ಒಳ_Range_Func get_ಒಳ_Range_Func(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ ಒಳ_Range_Func ಒಳ_Range_Tab[CV_ಆಳ_MAX] =
    {
        ಒಳ_Range<uchar>,
        ಒಳ_Range<schar>,
        ಒಳ_Range<ushort>,
        ಒಳ_Range<ಗಿಡ್ಡ>,
        ಒಳ_Range<ಪೂರ್ಣಾಂಕ>,
        ಒಳ_Range<ದಶಮಾಂಶ>,
        ಒಳ_Range<ದ್ವಿಗುಣ>,
        ಒಳ_Range<hfloat>,
        ಒಳ_Range<bfloat>,
        0,
        ಒಳ_Range<uint64_t>,
        ಒಳ_Range<int64_t>,
        ಒಳ_Range<ಚಿಹ್ನೆರಹಿತ>,
        0,
    };

    ಹಿಂತಿರುಗಿ ಒಳ_Range_Tab[ಆಳ];
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_ಒಳ_Range( ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Array _lowerb,
                         ಒಳಹರಿವು_Array _upperb, OutputArray _dst )
{
    ಸ್ಥಿರಾಂಕ ocl::Device & d = ocl::Device::getDefault();
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag skind = _src.ಬಗೆ(), lkind = _lowerb.ಬಗೆ(), ukind = _upperb.ಬಗೆ();
    ಗಾತ್ರ_2 ssize = _src.size(), lsize = _lowerb.size(), usize = _upperb.size();
    ಪೂರ್ಣಾಂಕ stype = _src.type(), ltype = _lowerb.type(), utype = _upperb.type();
    ಪೂರ್ಣಾಂಕ sdepth = CV_MAT_ಆಳ(stype), ldepth = CV_MAT_ಆಳ(ltype), udepth = CV_MAT_ಆಳ(utype);
    ಪೂರ್ಣಾಂಕ cn = CV_MAT_CN(stype), ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = d.isIntel() ? 4 : 1;
    ತಾರ್ಕಿಕ lbScalar = ಸುಳ್ಳು, ubScalar = ಸುಳ್ಳು;

    ಆದರೆ( (lkind == ಒಳಹರಿವು_Array_2::MATX && skind != ಒಳಹರಿವು_Array_2::MATX) ||
        ssize != lsize || stype != ltype )
    {
        ಆದರೆ( !checkScalar(_lowerb, stype, lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = ನಿಜ;
    }

    ಆದರೆ( (ukind == ಒಳಹರಿವು_Array_2::MATX && skind != ಒಳಹರಿವು_Array_2::MATX) ||
        ssize != usize || stype != utype )
    {
        ಆದರೆ( !checkScalar(_upperb, stype, ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = ನಿಜ;
    }

    ಆದರೆ (lbScalar != ubScalar)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0,
            haveScalar = lbScalar && ubScalar;

    ಆದರೆ ( (!doubleSupport && sdepth == CV_64F) ||
         (!haveScalar && (sdepth != ldepth || sdepth != udepth)) )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಪೂರ್ಣಾಂಕ kercn = haveScalar ? cn : std::max(std::min(ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(_src, _lowerb, _upperb, _dst), 4), cn);
    ಆದರೆ (kercn % cn != 0)
        kercn = cn;
    ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_ಪ್ರತಿ_WI = kercn / cn;
    String opts = format("%s-D CN=%d -D SRC_T=%s -D SRC_T1=%s -D DST_T=%s -D KERCN=%d -D DEPTH=%d%s -D COLS_PER_WI=%d",
                           haveScalar ? "-D HAVE_SCALAR " : "", cn, ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(sdepth, kercn)),
                           ocl::type_ಗಮ್ಯ_Str(sdepth), ocl::type_ಗಮ್ಯ_Str(CV_8UC(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_ಪ್ರತಿ_WI)), kercn, sdepth,
                           doubleSupport ? " -D DOUBLE_SUPPORT" : "", ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_ಪ್ರತಿ_WI);

    ocl::Kernel ker("inrange", ocl::core::inrange_oclsrc, opts);
    ಆದರೆ (ker.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    _dst.create(ssize, CV_8UC1);
    UMat src = _src.getUMat(), dst = _dst.getUMat(), lscalaru, uscalaru;
    Mat lscalar, uscalar;

    ಆದರೆ (lbScalar && ubScalar)
    {
        lscalar = _lowerb.getMat();
        uscalar = _upperb.getMat();

        size_t esz = src.elem_ಗಾತ್ರ();
        size_t blocksize = 36;

        Auto_ಬಫರ್<uchar> _buf(blocksize*(((ಪೂರ್ಣಾಂಕ)lbScalar + (ಪೂರ್ಣಾಂಕ)ubScalar)*esz + cn) + 2*cn*ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) + 128);
        uchar *buf = alignPtr(_buf.ದತ್ತಾಂಶ() + blocksize*cn, 16);

        ಆದರೆ( ldepth != sdepth && sdepth < CV_32S )
        {
            ಪೂರ್ಣಾಂಕ* ilbuf = (ಪೂರ್ಣಾಂಕ*)alignPtr(buf + blocksize*esz, 16);
            ಪೂರ್ಣಾಂಕ* iubuf = ilbuf + cn;

            ದ್ವಿಮಾನ_Func sccvtfunc = getConvertFunc(ldepth, CV_32S);
            sccvtfunc(lscalar.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, ಗಾತ್ರ_2(cn, 1), 0);
            sccvtfunc(uscalar.ptr(), 1, 0, 1, (uchar*)iubuf, 1, ಗಾತ್ರ_2(cn, 1), 0);
            ಪೂರ್ಣಾಂಕ minval = cv_ಸುತ್ತು(getMinVal(sdepth)), maxval = cv_ಸುತ್ತು(getMaxVal(sdepth));

            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++ )
            {
                ಆದರೆ( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lscalar = Mat(cn, 1, CV_32S, ilbuf);
            uscalar = Mat(cn, 1, CV_32S, iubuf);
        }

        lscalar.convert_ಗಮ್ಯ(lscalar, stype);
        uscalar.convert_ಗಮ್ಯ(uscalar, stype);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        lscalaru = _lowerb.getUMat();
        uscalaru = _upperb.getUMat();
    }

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, 1, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_ಪ್ರತಿ_WI);

    ಆದರೆ (haveScalar)
    {
        lscalar.copy_ಗಮ್ಯ(lscalaru);
        uscalar.copy_ಗಮ್ಯ(uscalaru);

        ker.ವಾದಗಳು(srcarg, dstarg, ocl::KernelArg::PtrReadOnly(lscalaru),
               ocl::KernelArg::PtrReadOnly(uscalaru), ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI);
    }
    ಇಲ್ಲದಿದ್ದರೆ
        ker.ವಾದಗಳು(srcarg, dstarg, ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(lscalaru),
               ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(uscalaru), ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI);

    size_t globalsize[2] = { (size_t)ssize.ಅಗಲ / ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_ಪ್ರತಿ_WI, ((size_t)ssize.ಎತ್ತರ + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ ker.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

}

ಶೂನ್ಯ cv::ಒಳ_Range_2(ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Array _lowerb,
                 ಒಳಹರಿವು_Array _upperb, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(! _src.empty());
    ಆದರೆ (!get_ಒಳ_Range_Func(_src.ಆಳ()))
        CV_Error( cv::Error::StsNotImplemented,
                 "inRange: CV_Bool and CV_8F inputs are not supported, convert the input to another type first");

    CV_OCL_ಸತತ(_src.dims() <= 2 && _lowerb.dims() <= 2 &&
               _upperb.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat()),
               ocl_ಒಳ_Range(_src, _lowerb, _upperb, _dst))

    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag skind = _src.ಬಗೆ(), lkind = _lowerb.ಬಗೆ(), ukind = _upperb.ಬಗೆ();
    Mat src = _src.getMat(), lb = _lowerb.getMat(), ub = _upperb.getMat();

    ತಾರ್ಕಿಕ lbScalar = ಸುಳ್ಳು, ubScalar = ಸುಳ್ಳು;

    ಆದರೆ( (lkind == ಒಳಹರಿವು_Array_2::MATX && skind != ಒಳಹರಿವು_Array_2::MATX) ||
        src.size != lb.size || src.type() != lb.type() )
    {
        ಆದರೆ( !checkScalar(lb, src.type(), lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = ನಿಜ;
    }

    ಆದರೆ( (ukind == ಒಳಹರಿವು_Array_2::MATX && skind != ಒಳಹರಿವು_Array_2::MATX) ||
        src.size != ub.size || src.type() != ub.type() )
    {
        ಆದರೆ( !checkScalar(ub, src.type(), ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = ನಿಜ;
    }

    CV_Assert(lbScalar == ubScalar);

    ಪೂರ್ಣಾಂಕ cn = src.channels(), ಆಳ = src.ಆಳ();

    size_t esz = src.elem_ಗಾತ್ರ();
    size_t blocksize0 = (size_t)(ಖಂಡ_ಗಾತ್ರ + esz-1)/esz;

    _dst.create_Same_ಗಾತ್ರ(_src, CV_8UC1);
    Mat dst = _dst.getMat();
    ಒಳ_Range_Func func = get_ಒಳ_Range_Func(ಆಳ);

    ಸ್ಥಿರಾಂಕ Mat* arrays_sc[] = { &src, &dst, 0 };
    ಸ್ಥಿರಾಂಕ Mat* arrays_nosc[] = { &src, &dst, &lb, &ub, 0 };
    uchar* ptrs[4] = {};

    NAryMatIterator it(lbScalar && ubScalar ? arrays_sc : arrays_nosc, ptrs);
    size_t ಒಟ್ಟು = it.size, blocksize = std::min(ಒಟ್ಟು, blocksize0);

    Auto_ಬಫರ್<uchar> _buf(blocksize*(((ಪೂರ್ಣಾಂಕ)lbScalar + (ಪೂರ್ಣಾಂಕ)ubScalar)*esz + cn) + 2*cn*ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ) + 128);
    uchar *buf = _buf.ದತ್ತಾಂಶ(), *mbuf = buf, *lbuf = 0, *ubuf = 0;
    buf = alignPtr(buf + blocksize*cn, 16);

    ಆದರೆ( lbScalar && ubScalar )
    {
        lbuf = buf;
        ubuf = buf = alignPtr(buf + blocksize*esz, 16);

        CV_Assert( lb.type() == ub.type() );
        ಪೂರ್ಣಾಂಕ scdepth = lb.ಆಳ();

        ಆದರೆ( scdepth != ಆಳ && ಆಳ < CV_32S )
        {
            ಪೂರ್ಣಾಂಕ* ilbuf = (ಪೂರ್ಣಾಂಕ*)alignPtr(buf + blocksize*esz, 16);
            ಪೂರ್ಣಾಂಕ* iubuf = ilbuf + cn;

            ದ್ವಿಮಾನ_Func sccvtfunc = getConvertFunc(scdepth, CV_32S);
            sccvtfunc(lb.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, ಗಾತ್ರ_2(cn, 1), 0);
            sccvtfunc(ub.ptr(), 1, 0, 1, (uchar*)iubuf, 1, ಗಾತ್ರ_2(cn, 1), 0);
            ಪೂರ್ಣಾಂಕ minval = cv_ಸುತ್ತು(getMinVal(ಆಳ)), maxval = cv_ಸುತ್ತು(getMaxVal(ಆಳ));

            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++ )
            {
                ಆದರೆ( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lb = Mat(cn, 1, CV_32S, ilbuf);
            ub = Mat(cn, 1, CV_32S, iubuf);
        }

        convertAndUnrollScalar( lb, src.type(), lbuf, blocksize );
        convertAndUnrollScalar( ub, src.type(), ubuf, blocksize );

        ಆದರೆ (ಆಳ == CV_8U && src.dims <= 2) {
            uint8_t lb_scalar = lbuf[0];
            uint8_t ub_scalar = ubuf[0];
            CALL_HAL(ಒಳ_Range_u8, cv_hal_ಒಳ_Range8u, src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, dst.ದತ್ತಾಂಶ, dst.ಹೆಜ್ಜೆ, dst.ಆಳ(), src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು, src.channels(),
                lb_scalar, ub_scalar);
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆಳ == CV_32F && src.dims <= 2) {
            ದ್ವಿಗುಣ lb_scalar = lb.ptr<ದ್ವಿಗುಣ>(0)[0];
            ದ್ವಿಗುಣ ub_scalar = ub.ptr<ದ್ವಿಗುಣ>(0)[0];
            CALL_HAL(ಒಳ_Range_f32, cv_hal_ಒಳ_Range32f, src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, dst.ದತ್ತಾಂಶ, dst.ಹೆಜ್ಜೆ, dst.ಆಳ(), src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು, src.channels(),
                lb_scalar, ub_scalar);
        }
    }

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಗಾಗಿ( size_t j = 0; j < ಒಟ್ಟು; j += blocksize )
        {
            ಪೂರ್ಣಾಂಕ bsz = (ಪೂರ್ಣಾಂಕ)MIN(ಒಟ್ಟು - j, blocksize);
            size_t delta = bsz*esz;
            uchar *lptr = lbuf, *uptr = ubuf;
            ಆದರೆ( !lbScalar )
            {
                lptr = ptrs[2];
                ptrs[2] += delta;
            }
            ಆದರೆ( !ubScalar )
            {
                ಪೂರ್ಣಾಂಕ idx = !lbScalar ? 3 : 2;
                uptr = ptrs[idx];
                ptrs[idx] += delta;
            }
            func( ptrs[0], 0, lptr, 0, uptr, 0, cn == 1 ? ptrs[1] : mbuf, 0, ಗಾತ್ರ_2(bsz*cn, 1));
            ಆದರೆ( cn > 1 )
                ಒಳ_Range_Reduce(mbuf, ptrs[1], bsz, cn);
            ptrs[0] += delta;
            ptrs[1] += bsz;
        }
    }
}
/* End of file. */
