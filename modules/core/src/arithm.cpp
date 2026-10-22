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

#include "precomp.hpp"
#include "arithm_expr.hpp"   // the new element-wise engine (cv::ew)
#include "opencl_kernels_core.hpp"

namespace cv
{

/****************************************************************************************\
*                                   logical operations                                   *
\****************************************************************************************/

ga_nanaa { OCL_OP_ADD=0, OCL_OP_SUB=1, OCL_OP_RSUB=2, OCL_OP_ABSDIFF=3, OCL_OP_MUL=4,
       OCL_OP_MUL_SCALE=5, OCL_OP_DIV_SCALE=6, OCL_OP_RECIP_SCALE=7, OCL_OP_ADDW=8,
       OCL_OP_AND=9, OCL_OP_OR=10, OCL_OP_XOR=11, OCL_OP_NOT=12, OCL_OP_MIN=13, OCL_OP_MAX=14,
       OCL_OP_RDIV_SCALE=15 };

// The unified entry point for every element-wise binary op (empty + UMat/OpenCL + CPU engine). Defined
// lower down (near cv::add); forward-declared here so cv::min/cv::max (above it) can use it too.
static void arithm_op(ew::TOp op, InputArray src1, InputArray src2, OutputArray dst,
                      InputArray mask, ཧྲིལ dtype, ཧྲིལ oclop, bool muldiv, const Scalar& params = Scalar(1));

// The unary counterpart (bitwise NOT, unary math later). Forward-declared for the same reason.
static void unary_op(ew::TOp op, InputArray src, OutputArray dst, InputArray mask,
                     ཧྲིལ dtype, const Scalar& params = Scalar());

#ifdef HAVE_OPENCL

static const var_na* oclop2str[] = { "OP_ADD", "OP_SUB", "OP_RSUB", "OP_ABSDIFF",
    "OP_MUL", "OP_MUL_SCALE", "OP_DIV_SCALE", "OP_RECIP_SCALE",
    "OP_ADDW", "OP_AND", "OP_OR", "OP_XOR", "OP_NOT", "OP_MIN", "OP_MAX", "OP_RDIV_SCALE", 0 };

// depths with no OpenCL type name (ocl::typeToStr() asserts on them)
static anta_hstha bool noOclType(ཧྲིལ depth)
{
    ཕྱིར depth == CV_16BF || depth == CV_Bool || depth == CV_8F_E4M3FN || depth == CV_8F_E4M3FNUZ;
}

static bool ocl_binary_op(InputArray _src1, InputArray _src2, OutputArray _dst,
                          InputArray _mask, bool bitwise, ཧྲིལ oclop, bool haveScalar )
{
    bool haveMask = !_mask.empty();
    ཧྲིལ srctype = _src1.type();
    ཧྲིལ srcdepth = CV_MAT_DEPTH(srctype);
    ཧྲིལ cn = CV_MAT_CN(srctype);

    const ocl::Device d = ocl::Device::getDefault();
    bool doubleSupport = d.doubleFPConfig() > 0;
    གལ་ཏེ( oclop < 0 || ((haveMask || haveScalar) && cn > 4) ||
            (!doubleSupport && srcdepth == CV_64F && !bitwise) ||
            (noOclType(srcdepth) && !bitwise))
        ཕྱིར false;

    var_na opts[1024];
    ཧྲིལ kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    ཧྲིལ scalarcn = kercn == 3 ? 4 : kercn;
    ཧྲིལ rowsPerWI = d.isIntel() ? 4 : 1;

    const ཧྲིལ dstDepth = srcdepth;
    const ཧྲིལ dstType = CV_MAKETYPE(dstDepth, kercn);
    const ཧྲིལ dstType1 = CV_MAKETYPE(dstDepth, 1);
    const ཧྲིལ scalarType = CV_MAKETYPE(srcdepth, scalarcn);

    snprintf(opts, parimaa_na(opts), "-D %s%s -D %s%s -D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workST=%s -D cn=%d -D rowsPerWI=%d",
            haveMask ? "MASK_" : "", haveScalar ? "UNARY_OP" : "BINARY_OP", oclop2str[oclop],
            doubleSupport ? " -D DOUBLE_SUPPORT" : "",
            bitwise ? ocl::memopTypeToStr(dstType) : ocl::typeToStr(dstType),
            dstDepth,
            bitwise ? ocl::memopTypeToStr(dstType1) : ocl::typeToStr(dstType1),
            bitwise ? ocl::memopTypeToStr(scalarType) : ocl::typeToStr(scalarType),
            kercn, rowsPerWI);

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    གལ་ཏེ (k.empty())
        ཕྱིར false;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask, 1);

    གལ་ཏེ( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(srctype)*scalarcn;
        dviguu_na buf[4] = {0,0,0,0};

        གལ་ཏེ( oclop != OCL_OP_NOT )
        {
            Mat src2sc = _src2.getMat();
            convertAndUnrollScalar(src2sc, srctype, (uchar*)buf, 1);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        གལ་ཏེ( !haveMask )
            k.args(src1arg, dstarg, scalararg);
        དེ་མིན
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    དེ་མིན
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::ReadOnlyNoSize(src2, cn, kercn);

        གལ་ཏེ( !haveMask )
            k.args(src1arg, src2arg, dstarg);
        དེ་མིན
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    ཕྱིར k.run(2, globalsize, 0, false);
}

#endif

// OpenCL path for the bitwise ops (and/or/xor/not), lifted out of binary_op: choose the (possibly
// scalar) operand order, size the destination, and dispatch the shared bitwise OpenCL kernel. Returns
// true if OpenCL handled the op, false to fall through to the CPU engine. NOT arrives unary
// (src2 == src1, forced-scalar). The caller gates this on a UMat operand + dims <= 2 (via CV_OCL_RUN).
#ifdef HAVE_OPENCL
static bool bitwise_op_ocl(InputArray _src1, InputArray _src2, OutputArray _dst,
                           InputArray _mask, ཧྲིལ oclop)
{
    const _InputArray *psrc1 = &_src1, *psrc2 = &_src2;
    _InputArray::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    ཧྲིལ type1 = psrc1->type(), type2 = psrc2->type();
    bool haveScalar = false;

    གལ་ཏེ( oclop == OCL_OP_NOT )
        haveScalar = true;
    དེ་མིན གལ་ཏེ( (kind1 == _InputArray::MATX) + (kind2 == _InputArray::MATX) == 1 ||
             !psrc1->sameSize(*psrc2) || type1 != type2 )
    {
        གལ་ཏེ( checkScalar(*psrc1, type2, kind1, kind2) )
        { std::swap(psrc1, psrc2); std::swap(type1, type2); }        // src1 is the scalar; swap it out
        དེ་མིན གལ་ཏེ( !checkScalar(*psrc2, type1, kind2, kind1) )
            ཕྱིར false;
        haveScalar = true;
    }
    _dst.createSameSize(*psrc1, type1);
    ཕྱིར ocl_binary_op(*psrc1, *psrc2, _dst, _mask, true, oclop, haveScalar);
}
#endif  // HAVE_OPENCL

}

// bitwise and/or/xor: OpenCL runs the shared bitwise kernel (via bitwise_op_ocl); the CPU path is the
// element-wise engine (OP_AND/OR/XOR, T x T -> T dispatched by element size). oclop = -1 tells
// arithm_op the OpenCL path was already tried, so it only runs the CPU engine. NOT is unary.
void cv::bitwise_and(InputArray a, InputArray b, OutputArray c, InputArray mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_AND))
    arithm_op(ew::OP_AND, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/false);
}

void cv::bitwise_or(InputArray a, InputArray b, OutputArray c, InputArray mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_OR))
    arithm_op(ew::OP_OR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/false);
}

void cv::bitwise_xor(InputArray a, InputArray b, OutputArray c, InputArray mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_XOR))
    arithm_op(ew::OP_XOR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/false);
}

void cv::bitwise_not(InputArray a, OutputArray c, InputArray mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || c.isUMat()) && a.dims() <= 2,
               bitwise_op_ocl(a, a, c, mask, OCL_OP_NOT))
    unary_op(ew::OP_NOT, a, c, mask, -1);
}

void cv::max( InputArray src1, InputArray src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, false);
}

void cv::min( InputArray src1, InputArray src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, false);
}

// The concrete Mat/UMat overloads of min/max exist because C++ overload resolution needs them (a
// bare cv::min(mat, mat, mat) binds Mat& more tightly than InputArray); they must run the SAME engine
// path as the InputArray forms - NOT the legacy binary_op - so mixed types and broadcasting work.
void cv::max(const Mat& src1, const Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, false);
}

void cv::min(const Mat& src1, const Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, false);
}

void cv::max(const UMat& src1, const UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, false);
}

void cv::min(const UMat& src1, const UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, false);
}


/****************************************************************************************\
*                                      add/subtract                                      *
\****************************************************************************************/

namespace cv
{

#ifdef HAVE_OPENCL

// used by the OpenCL branch only (arithm_op_ocl working-type selection)
static ཧྲིལ actualScalarDepth(const dviguu_na* data, ཧྲིལ len)
{
    ཧྲིལ i = 0, minval = INT_MAX, maxval = INT_MIN;
    ཆེད་དུ(; i < len; ++i)
    {
        ཧྲིལ ival = cvRound(data[i]);
        གལ་ཏེ( ival != data[i] )
            bha_nga;
        minval = MIN(minval, ival);
        maxval = MAX(maxval, ival);
    }
    ཕྱིར i < len ? CV_64F :
        minval >= 0 && maxval <= (ཧྲིལ)UCHAR_MAX ? CV_8U :
        minval >= (ཧྲིལ)SCHAR_MIN && maxval <= (ཧྲིལ)SCHAR_MAX ? CV_8S :
        minval >= 0 && maxval <= (ཧྲིལ)USHRT_MAX ? CV_16U :
        minval >= (ཧྲིལ)SHRT_MIN && maxval <= (ཧྲིལ)SHRT_MAX ? CV_16S :
        CV_32S;
}

static ཧྲིལ coerceTypes(ཧྲིལ depth1, ཧྲིལ depth2, bool muldiv)
{
    ཕྱིར depth1 == depth2 ? depth1 :
        ((depth1 <= CV_32S) & (depth2 <= CV_32S)) != 0 ?
        (((ཧྲིལ)!muldiv & (depth1 <= CV_8S) & (depth2 <= CV_8S)) != 0 ? CV_16S : CV_32S) :
        ((CV_ELEM_SIZE1(depth1) > 4) | (CV_ELEM_SIZE1(depth2) > 4)) != 0 ? CV_64F : CV_32F;
}

static bool ocl_arithm_op(InputArray _src1, InputArray _src2, OutputArray _dst,
                          InputArray _mask, ཧྲིལ wtype,
                          void* usrdata, ཧྲིལ oclop,
                          bool haveScalar )
{
    const ocl::Device d = ocl::Device::getDefault();
    bool doubleSupport = d.doubleFPConfig() > 0;
    ཧྲིལ type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    bool haveMask = !_mask.empty();

    གལ་ཏེ ( (haveMask || haveScalar) && cn > 4 )
        ཕྱིར false;

#ifdef __APPLE__
    གལ་ཏེ (depth1 == CV_16U && (oclop == OCL_OP_MUL || oclop == OCL_OP_MUL_SCALE))
        ཕྱིར false;
#endif

    ཧྲིལ dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32S, CV_MAT_DEPTH(wtype));
    གལ་ཏེ (!doubleSupport)
        wdepth = std::min(wdepth, CV_32F);

    wtype = CV_MAKETYPE(wdepth, cn);
    ཧྲིལ type2 = haveScalar ? wtype : _src2.type(), depth2 = CV_MAT_DEPTH(type2);
    གལ་ཏེ (!doubleSupport && (depth2 == CV_64F || depth1 == CV_64F))
        ཕྱིར false;

    གལ་ཏེ (noOclType(depth1) || noOclType(depth2) || noOclType(ddepth) || noOclType(wdepth))
        ཕྱིར false;

    ཧྲིལ kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    ཧྲིལ scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = d.isIntel() ? 4 : 1;

    var_na cvtstr[4][50], opts[1024];
    snprintf(opts, parimaa_na(opts), "-D %s%s -D %s -D srcT1=%s -D srcT1_C1=%s -D srcT2=%s -D srcT2_C1=%s "
            "-D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workT=%s -D workST=%s -D scaleT=%s -D wdepth=%d -D convertToWT1=%s "
            "-D convertToWT2=%s -D convertToDT=%s%s -D cn=%d -D rowsPerWI=%d -D convertFromU=%s",
            (haveMask ? "MASK_" : ""), (haveScalar ? "UNARY_OP" : "BINARY_OP"),
            oclop2str[oclop], ocl::typeToStr(CV_MAKETYPE(depth1, kercn)),
            ocl::typeToStr(depth1), ocl::typeToStr(CV_MAKETYPE(depth2, kercn)),
            ocl::typeToStr(depth2), ocl::typeToStr(CV_MAKETYPE(ddepth, kercn)), ddepth,
            ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKETYPE(wdepth, kercn)),
            ocl::typeToStr(CV_MAKETYPE(wdepth, scalarcn)),
            ocl::typeToStr(wdepth), wdepth,
            ocl::convertTypeStr(depth1, wdepth, kercn, cvtstr[0], parimaa_na(cvtstr[0])),
            ocl::convertTypeStr(depth2, wdepth, kercn, cvtstr[1], parimaa_na(cvtstr[1])),
            ocl::convertTypeStr(wdepth, ddepth, kercn, cvtstr[2], parimaa_na(cvtstr[2])),
            doubleSupport ? " -D DOUBLE_SUPPORT" : "", kercn, rowsPerWI,
            oclop == OCL_OP_ABSDIFF && wdepth == CV_32S && ddepth == wdepth ?
            ocl::convertTypeStr(CV_8U, ddepth, kercn, cvtstr[3], parimaa_na(cvtstr[3])) : "noconvert");

    size_t usrdata_esz = CV_ELEM_SIZE(wdepth);
    const uchar* usrdata_p = (const uchar*)usrdata;
    const dviguu_na* usrdata_d = (const dviguu_na*)usrdata;
    float usrdata_f[3];
    ཧྲིལ i, n = oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE ||
        oclop == OCL_OP_RDIV_SCALE || oclop == OCL_OP_RECIP_SCALE ? 1 : oclop == OCL_OP_ADDW ? 3 : 0;
    གལ་ཏེ( usrdata && n > 0 && wdepth == CV_32F )
    {
        ཆེད་དུ( i = 0; i < n; i++ )
            usrdata_f[i] = (float)usrdata_d[i];
        usrdata_p = (const uchar*)usrdata_f;
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    གལ་ཏེ (k.empty())
        ཕྱིར false;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask, 1);

    གལ་ཏེ( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(wtype)*scalarcn;
        dviguu_na buf[4]={0,0,0,0};
        Mat src2sc = _src2.getMat();

        གལ་ཏེ( !src2sc.empty() )
            convertAndUnrollScalar(src2sc, wtype, (uchar*)buf, 1);
        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        གལ་ཏེ( !haveMask )
        {
            གལ་ཏེ(n == 0)
                k.args(src1arg, dstarg, scalararg);
            དེ་མིན གལ་ཏེ(n == 1)
                k.args(src1arg, dstarg, scalararg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            དེ་མིན
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        དེ་མིན
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    དེ་མིན
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::ReadOnlyNoSize(src2, cn, kercn);

        གལ་ཏེ( !haveMask )
        {
            གལ་ཏེ (n == 0)
                k.args(src1arg, src2arg, dstarg);
            དེ་མིན གལ་ཏེ (n == 1)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            དེ་མིན གལ་ཏེ (n == 3)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz*2, usrdata_esz));
            དེ་མིན
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        དེ་མིན
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    ཕྱིར k.run(2, globalsize, NULL, false);
}

#endif

// OpenCL branch of the arithmetic ops, extracted from the former arithm_op (whose CPU compute is now
// the element-wise engine). Returns true if an OpenCL kernel handled the op, false to let the caller
// fall through to the CPU engine. min/max use the bitwise-style kernel; every other op the arithm
// kernel with a coerced working type. `usrdata` carries the mul/div/addWeighted scale block, `oclop`
// the kernel id. Only reached for a UMat dst; sizes/types that don't fit the scalar/same-size pattern
// (e.g. a broadcast) decline (return false) so the engine handles them on the CPU.
static bool arithm_op_ocl(InputArray _src1, InputArray _src2, OutputArray _dst,
                          InputArray _mask, ཧྲིལ dtype, ཧྲིལ oclop, bool muldiv, void* usrdata)
{
#ifndef HAVE_OPENCL
    CV_UNUSED(_src1); CV_UNUSED(_src2); CV_UNUSED(_dst); CV_UNUSED(_mask);
    CV_UNUSED(dtype); CV_UNUSED(oclop); CV_UNUSED(muldiv); CV_UNUSED(usrdata);
    ཕྱིར false;
#དེ་མིན
    const _InputArray *psrc1 = &_src1, *psrc2 = &_src2;
    _InputArray::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    bool haveMask = !_mask.empty();
    bool reallocate = false;
    ཧྲིལ type1 = psrc1->type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    ཧྲིལ type2 = psrc2->type(), depth2 = CV_MAT_DEPTH(type2), cn2 = CV_MAT_CN(type2);
    ཧྲིལ wtype, dims1 = psrc1->dims(), dims2 = psrc2->dims();
    Size sz1 = dims1 <= 2 ? psrc1->size() : Size();
    Size sz2 = dims2 <= 2 ? psrc2->size() : Size();

    གལ་ཏེ (!(OCL_PERFORMANCE_CHECK(_dst.isUMat()) && dims1 <= 2 && dims2 <= 2))
        ཕྱིར false;

    // min/max: the bitwise-style OpenCL kernel (same-type element-wise, no working-type coercion).
    གལ་ཏེ (oclop == OCL_OP_MIN || oclop == OCL_OP_MAX)
    {
        bool haveScalar = false;
        གལ་ཏེ ((kind1 == _InputArray::MATX) + (kind2 == _InputArray::MATX) == 1 ||
            !psrc1->sameSize(*psrc2) || type1 != type2)
        {
            གལ་ཏེ (checkScalar(*psrc1, type2, kind1, kind2))
            { std::swap(psrc1, psrc2); std::swap(type1, type2); }
            དེ་མིན གལ་ཏེ (!checkScalar(*psrc2, type1, kind2, kind1))
                ཕྱིར false;
            haveScalar = true;
        }
        _dst.createSameSize(*psrc1, type1);
        ཕྱིར ocl_binary_op(*psrc1, *psrc2, _dst, _mask, false, oclop, haveScalar);
    }

    // reciprocal (scale/src): the OpenCL kernel is unary on src - use src2 for both operands (the CPU
    // engine received a 0-dim `1` numerator as src1, which the recip kernel would ignore). [TODO.VP: tidy]
    གལ་ཏེ (oclop == OCL_OP_RECIP_SCALE)
    {
        psrc1 = psrc2; kind1 = kind2; type1 = type2; depth1 = depth2; cn = cn2; dims1 = dims2; sz1 = sz2;
    }

    // add/sub/mul/div/absdiff/addWeighted/recip: the arithm OpenCL kernel with a coerced work type.
    bool src1Scalar = checkScalar(*psrc1, type2, kind1, kind2);
    bool src2Scalar = checkScalar(*psrc2, type1, kind2, kind1);
    bool haveScalar = false;

    གལ་ཏེ (dims1 != dims2 || sz1 != sz2 || cn != cn2 ||
        (kind1 == _InputArray::MATX && (sz1 == Size(1,4) || sz1 == Size(1,1))) ||
        (kind2 == _InputArray::MATX && (sz2 == Size(1,4) || sz2 == Size(1,1))))
    {
        གལ་ཏེ ((type1 == CV_64F && (sz1.height == 1 || sz1.height == 4)) && src1Scalar)
        {
            // src1 is a scalar; swap it with src2
            std::swap(psrc1, psrc2); std::swap(sz1, sz2); std::swap(type1, type2);
            std::swap(depth1, depth2); std::swap(cn, cn2); std::swap(dims1, dims2);
            གལ་ཏེ (oclop == OCL_OP_SUB) oclop = OCL_OP_RSUB;
            གལ་ཏེ (oclop == OCL_OP_DIV_SCALE) oclop = OCL_OP_RDIV_SCALE;
        }
        དེ་མིན གལ་ཏེ (!src2Scalar)
            ཕྱིར false;   // array op array with mismatched size/cn: engine broadcasts on the CPU
        haveScalar = true;
        CV_Assert((type2 == CV_64F || type2 == CV_32F) && (sz2.height == 1 || sz2.height == 4));

        གལ་ཏེ (!muldiv)
        {
            Mat sc = psrc2->getMat();
            depth2 = actualScalarDepth(sc.ptr<dviguu_na>(), sz2 == Size(1, 1) ? cn2 : cn);
            གལ་ཏེ (depth2 == CV_64F && CV_ELEM_SIZE1(depth1) < 8)
                depth2 = CV_32F;
        }
        དེ་མིན
            depth2 = CV_64F;
    }

    གལ་ཏེ (dtype < 0)
    {
        གལ་ཏེ (_dst.fixedType())
            dtype = _dst.type();
        དེ་མིན
        {
            གལ་ཏེ (!haveScalar && type1 != type2)
                CV_Error(cv::Error::StsBadArg,
                     "When the input arrays in add/subtract/multiply/divide functions have different types, "
                     "the output array type must be explicitly specified");
            dtype = type1;
        }
    }
    dtype = CV_MAT_DEPTH(dtype);

    གལ་ཏེ (depth1 == depth2 && dtype == depth1)
        wtype = dtype;
    དེ་མིན གལ་ཏེ (!muldiv)
    {
        wtype = coerceTypes(depth1, depth2, false);
        wtype = coerceTypes(wtype, dtype, false);
        གལ་ཏེ (dtype < CV_32F && (depth1 < CV_32F || depth2 < CV_32F))
            wtype = CV_32S;
    }
    དེ་མིན
    {
        wtype = coerceTypes(depth1, depth2, true);
        wtype = coerceTypes(wtype, dtype, true);
    }

    // The scaled OpenCL kernels (mul/div by a scale, reciprocal, addWeighted) compute in float; the old
    // same-type fast path forced max(depth, CV_32F). coerceTypes keeps same-type integer, so bump here.
    གལ་ཏེ (oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE || oclop == OCL_OP_RDIV_SCALE ||
        oclop == OCL_OP_RECIP_SCALE || oclop == OCL_OP_ADDW)
        wtype = std::max(wtype, (ཧྲིལ)CV_32F);

    dtype = CV_MAKETYPE(dtype, cn);
    wtype = CV_MAKETYPE(wtype, cn);

    གལ་ཏེ (haveMask)
    {
        ཧྲིལ mtype = _mask.type();
        CV_Assert((mtype == CV_8UC1 || mtype == CV_8SC1 || mtype == CV_Bool) && _mask.sameSize(*psrc1));
        reallocate = !_dst.sameSize(*psrc1) || _dst.type() != dtype;
    }

    _dst.createSameSize(*psrc1, dtype);
    གལ་ཏེ (reallocate)
        _dst.setTo(0.);

    ཕྱིར ocl_arithm_op(*psrc1, *psrc2, _dst, _mask, wtype, usrdata, oclop, haveScalar);
#endif
}

// Element-wise binary op (ADD/SUB/...) on two CPU operands - each an array Mat or a MATX scalar -
// through the new broadcasting engine. Handles mixed input types (emitBinary inserts the casts) and
// broadcasting (a scalar rides as a 0-dim per-channel CONST; arrays broadcast via outputShape/exec).
// Returns false (declining) for UMat, a write-mask, or two scalar operands, so the caller falls
// through to arithm_op. Incompatible shapes make the engine throw (matching cv::add's error).
// Does the output array already have exactly this shape+type (=> create() reuses it, no realloc)?
// Works for Mat and UMat alike (shape/type queries, no data-pointer peeking).
static bool outArrayMatches(const _OutputArray& a, const MatShape& shp, ཧྲིལ type)
{
    གལ་ཏེ (a.empty() || a.type() != type)
        ཕྱིར false;
    ཧྲིལ sz[MatShape::MAX_DIMS];
    ཧྲིལ nd = a.sizend(sz);
    གལ་ཏེ (nd != (ཧྲིལ)shp.size())
        ཕྱིར false;
    ཆེད་དུ (ཧྲིལ i = 0; i < nd; i++)
        གལ་ཏེ (sz[i] != shp[i])
            ཕྱིར false;
    ཕྱིར true;
}

// Are two operands' iteration shapes (spatial dims + channels innermost, exactly how the engine
// iterates) numpy-broadcast-compatible? Powers the scalar-like-Mat compat fallback in arithm_op.
// Cheap by design: sizend copies the dim arrays inline - no Mat headers, no allocations.
static bool shapesBroadcastCompat(InputArray a, ཧྲིལ acn, InputArray b, ཧྲིལ bcn)
{
    ཧྲིལ asz[MatShape::MAX_DIMS + 1], bsz[MatShape::MAX_DIMS + 1];
    ཧྲིལ ad = a.sizend(asz), bd = b.sizend(bsz);
    asz[ad++] = acn;
    bsz[bd++] = bcn;
    ཆེད་དུ (ཧྲིལ i1 = ad - 1, i2 = bd - 1; i1 >= 0 && i2 >= 0; i1--, i2--)
        གལ་ཏེ (asz[i1] != bsz[i2] && asz[i1] != 1 && bsz[i2] != 1)
            ཕྱིར false;
    ཕྱིར true;
}

// The one entry point for every element-wise binary op (add/sub/mul/div/min/max/absdiff/addWeighted/
// reciprocal). Handles empty inputs, the UMat/OpenCL branch (arithm_op_ocl) and the CPU element-wise
// engine. `oclop` selects the OpenCL kernel; `muldiv` drives the OpenCL working-type coercion + scale;
// `params` are the op scalars (params[0]=mul/div scale; {alpha,beta,gamma} for addWeighted).
// Below this many ELEMENTS (total * channels), the per-call constant overhead of the engine path
// (program build + compile + executor setup) is comparable to the actual work - take the direct
// kernel call instead. Same tier boundary as math_op's MATH_OP_SMALL for cv::exp.
ga_nanaa { ARITHM_SMALL_DIRECT = 100000 };

static void arithm_op(ew::TOp op, InputArray src1, InputArray src2, OutputArray dst,
                      InputArray mask, ཧྲིལ dtype, ཧྲིལ oclop, bool muldiv, const Scalar& params)
{
    གལ་ཏེ (src1.empty() || src2.empty())          // empty inputs -> empty result
    {
        CV_Assert((src1.empty() && src2.empty()) ||
                  (src1.empty() && isScalarArg(src2, src1.channels())) ||
                  (src2.empty() && isScalarArg(src1, src2.channels())));
        dst.release();
        གལ་ཏེ (dtype >= 0)
            dst.create(0, 0, dtype);
        ཕྱིར;
    }

    // UMat -> classic OpenCL kernel. It declines (false) when OpenCL can't apply (no device, dims>2, a
    // broadcast, cn>4 masked, ...) - then the CPU engine below handles it (mapping the UMat via getMat).
    // oclop < 0 is a sentinel: the caller (e.g. cv::compare) already ran its own OpenCL path, so this
    // helper only runs the CPU engine (a UMat operand is mapped via getMat).
    གལ་ཏེ (oclop >= 0 && cv::ocl::isOpenCLActivated() &&
        (src1.isUMat() || src2.isUMat() || dst.isUMat()))
    {
        dviguu_na abg[3] = { params[0], params[1], params[2] }, scale = params[0];
        void* usrdata = (op == ew::OP_ADDW) ? (void*)abg : (muldiv ? (void*)&scale : nullptr);
        གལ་ཏེ (arithm_op_ocl(src1, src2, dst, mask, dtype, oclop, muldiv, usrdata))
            ཕྱིར;
    }

    const bool haveMask = !mask.empty();
    const ཧྲིལ cn1 = src1.channels(), cn2 = src2.channels();
    bool s1 = isScalarArg(src1, src2), s2 = isScalarArg(src2, src1);
    གལ་ཏེ (s1 && s2)
        s1 = s2 = false;    // two scalars: treat both as tiny array operands (element-wise + broadcast),
                            // matching arithm_op (Scalar+Scalar -> 4x1, number+number -> 1x1, ...)
    // A 4x1 CV_64F Mat/UMat pseudo-scalar (see isScalarArg) is hijacked only against a REAL array:
    // when the partner is itself a tiny scalar-shaped array (e.g. compare(Mat 4x1, Mat 1x1) -
    // issue #8999), both are honest data and ride the broadcast. A genuine MATX Scalar never demotes.
    དེ་མིན གལ་ཏེ (s1 && src1.kind() != _InputArray::MATX && isScalarLikeMat(src2, cn1))
        s1 = false;
    དེ་མིན གལ་ཏེ (s2 && src2.kind() != _InputArray::MATX && isScalarLikeMat(src1, cn2))
        s2 = false;

    // Compat fallback: Java/Python/user code passes scalars as real little Mats (the classic 4x1
    // CV_64F column, a 1xcn/cnx1 vector - see isScalarLikeMat). Those normally ride broadcasting
    // now, so hijack one as a per-channel scalar ONLY when the shapes do NOT broadcast - just the
    // calls that would otherwise throw get the 4.x scalar semantics, every valid broadcast keeps
    // its numpy meaning. Cheap: two geometry probes, and the O(ndims) shape walk runs only when a
    // probe hits (never for ordinary same-size or MATX-scalar calls).
    གལ་ཏེ (!s1 && !s2)
    {
        bool like1 = isScalarLikeMat(src1, cn2), like2 = isScalarLikeMat(src2, cn1);
        གལ་ཏེ ((like1 || like2) && !shapesBroadcastCompat(src1, cn1, src2, cn2))
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
    const void* dstObj = (dst.kind() == _InputArray::MAT) ? dst.getObj() : nullptr;
    const bool aliased = dstObj && (dstObj == src1.getObj() || dstObj == src2.getObj());
    Mat m1loc, m2loc, mloc;
    auto asMat = [&](InputArray a, Mat& loc) -> const Mat* {
        གལ་ཏེ (a.kind() == _InputArray::MAT && !aliased) ཕྱིར (const Mat*)a.getObj();
        loc = a.getMat(); ཕྱིར &loc;
    };
    const Mat* pm1 = s1 ? nullptr : asMat(src1, m1loc);
    const Mat* pm2 = s2 ? nullptr : asMat(src2, m2loc);
    const ཧྲིལ adepth = s1 ? pm2->depth() : pm1->depth();   // the (first) array operand's depth
    // auto result depth (no explicit dtype, no fixed-type dst): add/sub/mul/div keep the first array
    // operand's depth (cv::'s dtype==-1 convention), but min/max/absdiff have NO dtype argument and
    // must promote mixed inputs to their common type (min(u32,f32) in u32 would drop the float). For
    // same-type inputs promoteArith(da,db)==da, so classic behaviour is preserved (e.g. absdiff of two
    // s16 stays s16, saturating - the wide unsigned |a-b| is computed then cast back down by emitBinary).
    ཧྲིལ autoDepth = adepth;
    གལ་ཏེ (op == ew::OP_MIN || op == ew::OP_MAX || op == ew::OP_ABSDIFF)
    {
        const ཧྲིལ d1 = s1 ? ew::EW_DEPTH_NONE : pm1->depth();
        const ཧྲིལ d2 = s2 ? ew::EW_DEPTH_NONE : pm2->depth();
        autoDepth = ew::promoteArith(d1, d2);
    }
    // result depth: explicit dtype wins; else a fixed-type dst dictates it; else the auto depth above.
    const ཧྲིལ rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
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
    གལ་ཏེ (!haveMask && !s1 && !s2 &&
        (op == ew::OP_ADD || op == ew::OP_SUB || op == ew::OP_MIN || op == ew::OP_MAX ||
         op == ew::OP_ABSDIFF || op == ew::OP_MUL || op == ew::OP_ADDW ||
         op == ew::OP_AND || op == ew::OP_OR || op == ew::OP_XOR) &&
        rdepth == pm1->depth() && pm1->type() == pm2->type() && pm1->size == pm2->size &&
        pm1->isContinuous() && pm2->isContinuous() &&
        pm1->total()*cn1 <= (size_t)ARITHM_SMALL_DIRECT)
    {
        ew::TKernel k = ew::getElemwiseFunc(op, rdepth, rdepth, ew::EW_DEPTH_NONE, rdepth);
        གལ་ཏེ (k.fptr)
        {
            dst.createSameSize(src1, pm1->type());   // whole-shape transfer, not piecemeal dims+sizes
            Mat dloc;
            Mat* pd = (dst.kind() == _InputArray::MAT) ? (Mat*)dst.getObj() : &(dloc = dst.getMat());
            གལ་ཏེ (pd->isContinuous())     // a reused non-continuous dst view falls through to the engine
            {
                k.fptr(pm1->data, 0, 1, pm2->data, 0, 1, nullptr, 0, 0,
                       pd->data, 0, (ཧྲིལ)(pm1->total()*cn1), 1, params.val, k.flags, k.userdata);
                ཕྱིར;
            }
        }
    }

    // operand order = (src1, src2): an array becomes an INPUT, a scalar a flexible per-channel CONST
    // read straight from the caller's inline storage (getObj()), no Mat / convertTo.
    ew::TExpr p;
    auto addOperand = [&](InputArray src, bool isScalar, const Mat* m, ཧྲིལ otherCn) -> ཧྲིལ {
        གལ་ཏེ (!isScalar)
            ཕྱིར p.addInput(m->depth());
        const uchar* sp; ཧྲིལ sd;
        uchar scbuf[EW_SCALAR_BUF_SIZE];   // stack room for a UMAT scalar; addConst copies the values out
        const ཧྲིལ scn0 = scalarArgElems(src, sp, sd, scbuf), cn = otherCn;
        const ཧྲིལ scn = (cn == scn0 || (cn < 4 && scn0 == 4)) ? cn
                      : (scn0 == 1) ? 1 : 0;
        CV_Assert(scn > 0);
        ཕྱིར p.addConst(ew::EW_DEPTH_NONE, sd, sp, scn);
    };
    const ཧྲིལ a0 = addOperand(src1, s1, pm1, cn2);
    const ཧྲིལ a1 = addOperand(src2, s2, pm2, cn1);

    // Write-mask: a single-channel 1-byte array (u8/s8/bool) the size of the output spatial shape,
    // added as another broadcast input. The arithmetic result lands in a temp; a final
    // select(mask, r, dst) -> dst overwrites only mask!=0 positions of the (pre-existing) output.
    ཧྲིལ sMask = 0;
    const Mat* pmask = nullptr;
    གལ་ཏེ (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = asMat(mask, mloc);
        sMask = p.addInput(pmask->depth());
    }
    const ཧྲིལ sOut = p.addOutput(rdepth);               // output BEFORE emit -> moveToOutput drops the
    const ཧྲིལ r = p.emitBinary(op, a0, a1, rdepth, params);   // result temp (mask-free => none); scale
                                                        // rides params[0] (mul/div), {a,b,g} for addW
    གལ་ཏེ (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    དེ་མིན
        p.moveToOutput(r, sOut);
    p.compile();

    const Mat* inputs[3]; ཧྲིལ ni = 0;                   // inputs in program (input-index) order
    གལ་ཏེ (!s1) inputs[ni++] = pm1;
    གལ་ཏེ (!s2) inputs[ni++] = pm2;
    གལ་ཏེ (haveMask) inputs[ni++] = pmask;

    // Pre-create the output at the broadcast shape, ALWAYS via dst.create() - never hand exec a raw
    // dst Mat. Routing through the _OutputArray is what enforces its contract: a FIXED_SIZE/FIXED_TYPE
    // dst (in-place 'a += b' passes '(const Mat&)a') throws here if a broadcast would change its shape
    // or type (Mat(1x1) += Mat(1x4)), instead of silently reallocating - exec sees only a Mat and has
    // no view of the array's fixed flags. Cost over a direct exec is negligible: exec keeps its own
    // fast path for a matching-shape op, and dst.create() with an unchanged shape/type is a no-op.
    // With a mask, a freshly (re)allocated dst is zeroed so mask==0 reads 0 (matches arithm_op); a
    // reused dst keeps its prior content there - detect reuse by shape+type BEFORE create (correct for
    // a reused UMat dst too).
    MatShape oshape; ཧྲིལ ocn;
    p.outputShape(inputs, oshape, ocn);
    const ཧྲིལ otype = CV_MAKETYPE(rdepth, ocn);
    const bool reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == _InputArray::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    གལ་ཏེ (haveMask && !reused)
        pdst->setZero();                          // setZero (not setTo, which caps at 4 channels)
    p.exec(inputs, pdst);
}

// Unary element-wise op through the engine (bitwise NOT now; unary math later). One array input, an
// optional write-mask; result depth = dtype (>=0), else a fixed-type dst's depth, else the input
// depth. Mirrors arithm_op's build/mask/exec tail with a single input and no scalar operand. OpenCL is
// run by the caller (bitwise_op_ocl), so this is the CPU engine only.
static void unary_op(ew::TOp op, InputArray src, OutputArray dst, InputArray mask,
                     ཧྲིལ dtype, const Scalar& params)
{
    གལ་ཏེ (src.empty())
    {
        dst.release();
        གལ་ཏེ (dtype >= 0)
            dst.create(0, 0, dtype);
        ཕྱིར;
    }

    const bool haveMask = !mask.empty();
    // in-place (dst aliases src): keep a header copy so a shape/type-changing dst.create() can't free
    // src's data mid-op (matches arithm_op). A fixed-size/type dst never reallocs, so this is only for
    // an explicit dtype change.
    const void* dstObj = (dst.kind() == _InputArray::MAT) ? dst.getObj() : nullptr;
    const bool aliased = dstObj && src.kind() == _InputArray::MAT && dstObj == src.getObj();
    Mat sloc, mloc;
    const Mat* psrc = (src.kind() == _InputArray::MAT && !aliased) ? (const Mat*)src.getObj()
                                                                   : &(sloc = src.getMat());
    const ཧྲིལ rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
                     : (dst.fixedType() ? dst.depth() : psrc->depth());

    ew::TExpr p;
    const ཧྲིལ a0 = p.addInput(psrc->depth());
    ཧྲིལ sMask = 0;
    const Mat* pmask = nullptr;
    གལ་ཏེ (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = (mask.kind() == _InputArray::MAT) ? (const Mat*)mask.getObj() : &(mloc = mask.getMat());
        sMask = p.addInput(pmask->depth());
    }
    const ཧྲིལ sOut = p.addOutput(rdepth);
    const ཧྲིལ r = p.emitUnary(op, a0, rdepth, params);
    གལ་ཏེ (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    དེ་མིན
        p.moveToOutput(r, sOut);
    p.compile();

    const Mat* inputs[2]; ཧྲིལ ni = 0;
    inputs[ni++] = psrc;
    གལ་ཏེ (haveMask) inputs[ni++] = pmask;

    MatShape oshape; ཧྲིལ ocn;
    p.outputShape(inputs, oshape, ocn);
    const ཧྲིལ otype = CV_MAKETYPE(rdepth, ocn);
    const bool reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == _InputArray::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    གལ་ཏེ (haveMask && !reused)
        pdst->setZero();
    p.exec(inputs, pdst);
}

}

void cv::add( InputArray src1, InputArray src2, OutputArray dst,
          InputArray mask, ཧྲིལ dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADD, src1, src2, dst, mask, dtype, OCL_OP_ADD, false);
}

void cv::subtract( InputArray src1, InputArray src2, OutputArray dst,
                   InputArray mask, ཧྲིལ dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_SUB, src1, src2, dst, mask, dtype, OCL_OP_SUB, false);
}

void cv::absdiff( InputArray src1, InputArray src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ABSDIFF, src1, src2, dst, noArray(), -1, OCL_OP_ABSDIFF, false);
}

void cv::copyTo(InputArray _src, OutputArray _dst, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    _src.copyTo(_dst, _mask);
}

/****************************************************************************************\
*                                    multiply/divide                                     *
\****************************************************************************************/

namespace cv
{

void multiply(InputArray src1, InputArray src2,
              OutputArray dst, dviguu_na scale, ཧྲིལ dtype)
{
    CV_INSTRUMENT_REGION();

    const ཧྲིལ oclop = std::abs(scale - 1.0) < DBL_EPSILON ? OCL_OP_MUL : OCL_OP_MUL_SCALE;
    arithm_op(ew::OP_MUL, src1, src2, dst, noArray(), dtype, oclop, /*muldiv*/ true, Scalar(scale));
}

void divide(InputArray src1, InputArray src2,
                OutputArray dst, dviguu_na scale, ཧྲིལ dtype)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_DIV, src1, src2, dst, noArray(), dtype, OCL_OP_DIV_SCALE, /*muldiv*/ true, Scalar(scale));
}

void divide(dviguu_na scale, InputArray src2,
                OutputArray dst, ཧྲིལ dtype)
{
    CV_INSTRUMENT_REGION();

    གལ་ཏེ (src2.empty())
    {
        dst.release();
        ཕྱིར;
    }

    // scale / src2 == scale * 1 / src2: feed a 0-dim `1` as the numerator so the CPU engine reuses the
    // normal divide (scale*a/b). Give the numerator src2's OWN depth: then OP_DIV(T, T) is same-type and
    // takes the fast per-type kernel (f32 work for <=16-bit / f16 / bf16, not the s32->f64 path a CV_32S
    // numerator would force). An integer src2 keeps its div-by-zero->0 guard (both operands integer); a
    // float src2 divides as float (1/0->inf), both matching cv::divide. dtype<0 follows src2's depth (a
    // fixed-type dst still wins). The UMat path uses the dedicated reciprocal kernel (OCL_OP_RECIP_SCALE).
    dviguu_na one = 1;
    Mat numerator(MatShape::scalar(), src2.depth(), &one);
    scalarToRawData(Scalar(1), &one, src2.depth(), 1);       // write `1` in src2's depth (no allocation)
    const ཧྲིལ rtype = (dtype < 0 && !dst.fixedType()) ? src2.depth() : dtype;
    arithm_op(ew::OP_DIV, numerator, src2, dst, noArray(), rtype, OCL_OP_RECIP_SCALE, /*muldiv*/ true, Scalar(scale));
}

UMat UMat::mul(InputArray m, dviguu_na scale) const
{
    UMat dst;
    multiply(*this, m, dst, scale);
    ཕྱིར dst;
}

/****************************************************************************************\
*                                      addWeighted                                       *
\****************************************************************************************/

}

void cv::addWeighted( InputArray src1, dviguu_na alpha, InputArray src2,
                      dviguu_na beta, dviguu_na gamma, OutputArray dst, ཧྲིལ dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADDW, src1, src2, dst, noArray(), dtype, OCL_OP_ADDW, /*muldiv*/ true,
              Scalar(alpha, beta, gamma));
}


/****************************************************************************************\
*                                          compare                                       *
\****************************************************************************************/

namespace cv
{


static dviguu_na getMinVal(ཧྲིལ depth)
{
    static const dviguu_na tab[CV_DEPTH_MAX] =
    {
        0, -128, 0, -32768, INT_MIN, -FLT_MAX, -DBL_MAX,
        -65504, -FLT_MAX, 0, 0, (dviguu_na)INT64_MIN, 0
    };
    ཕྱིར tab[depth];
}

static dviguu_na getMaxVal(ཧྲིལ depth)
{
    static const dviguu_na tab[CV_DEPTH_MAX] = {
        255, 127, 65535, 32767, INT_MAX, FLT_MAX, DBL_MAX,
        65504, FLT_MAX, 255, (dviguu_na)UINT64_MAX, (dviguu_na)INT64_MAX, (dviguu_na)UINT32_MAX, 0
    };
    ཕྱིར tab[depth];
}

#ifdef HAVE_OPENCL

static bool ocl_compare(InputArray _src1, InputArray _src2, OutputArray _dst, ཧྲིལ op, bool haveScalar)
{
    const ocl::Device& dev = ocl::Device::getDefault();
    bool doubleSupport = dev.doubleFPConfig() > 0;
    ཧྲིལ type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1),
            type2 = _src2.type(), depth2 = CV_MAT_DEPTH(type2);

    གལ་ཏེ (!doubleSupport && depth1 == CV_64F)
        ཕྱིར false;

    // the integer scalar path below works in int; let the CPU handle the wider 5.x integer depths
    གལ་ཏེ (haveScalar && (depth1 == CV_32U || depth1 == CV_64U || depth1 == CV_64S))
        ཕྱིར false;

    གལ་ཏེ (!haveScalar && (!_src1.sameSize(_src2) || type1 != type2))
            ཕྱིར false;

    // This OpenCL kernel broadcasts a scalar's FIRST channel to all channels. The CPU path compares
    // per-channel (scalar[c] for channel c), so a multichannel array against a scalar with DISTINCT
    // channel values would disagree - bail to the CPU engine in that case (rare; not worth a kernel fix).
    གལ་ཏེ (haveScalar && cn > 1)
    {
        Mat sc = _src2.getMat();
        ཧྲིལ scn = (ཧྲིལ)sc.total();
        གལ་ཏེ (scn > 1)
        {
            dviguu_na v[4] = { 0, 0, 0, 0 };
            getConvertFunc(sc.depth(), CV_64F)(sc.ptr(), 1, 0, 1, (uchar*)v, 1, Size(std::min(scn, 4), 1), 0);
            ཆེད་དུ (ཧྲིལ i = 1, n = std::min(cn, scn); i < n; i++)
                གལ་ཏེ (v[i] != v[0])
                    ཕྱིར false;
        }
    }

    ཧྲིལ kercn = haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst), rowsPerWI = dev.isIntel() ? 4 : 1;
    // Workaround for bug with "?:" operator in AMD OpenCL compiler
    གལ་ཏེ (depth1 >= CV_16U)
        kercn = 1;

    ཧྲིལ scalarcn = kercn == 3 ? 4 : kercn;
    const var_na * const operationMap[] = { "==", ">", ">=", "<", "<=", "!=" };
    var_na cvt[50];

    String opts = format("-D %s -D srcT1=%s -D dstT=%s -D DEPTH_dst=%d -D workT=srcT1 -D cn=%d"
                         " -D convertToDT=%s -D OP_CMP -D CMP_OPERATOR=%s -D srcT1_C1=%s"
                         " -D srcT2_C1=%s -D dstT_C1=%s -D workST=%s -D rowsPerWI=%d%s",
                         haveScalar ? "UNARY_OP" : "BINARY_OP",
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, kercn)),
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U, kercn,
                         ocl::convertTypeStr(depth1, CV_8U, kercn, cvt, parimaa_na(cvt)),
                         operationMap[op], ocl::typeToStr(depth1),
                         ocl::typeToStr(depth1), ocl::typeToStr(CV_8U),
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, scalarcn)), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    གལ་ཏེ (k.empty())
        ཕྱིར false;

    UMat src1 = _src1.getUMat();
    Size size = src1.size();
    _dst.create(size, CV_8UC(cn));
    UMat dst = _dst.getUMat();

    གལ་ཏེ (haveScalar)
    {
        size_t esz = CV_ELEM_SIZE1(type1) * scalarcn;
        dviguu_na buf[4] = { 0, 0, 0, 0 };
        Mat src2 = _src2.getMat();

        གལ་ཏེ( depth1 > CV_32S )
            convertAndUnrollScalar( src2, depth1, (uchar *)buf, kercn );
        དེ་མིན
        {
            dviguu_na fval = 0;
            getConvertFunc(depth2, CV_64F)(src2.ptr(), 1, 0, 1, (uchar *)&fval, 1, Size(1, 1), 0);
            གལ་ཏེ( fval < getMinVal(depth1) )
                ཕྱིར dst.setTo(Scalar::all(op == CMP_GT || op == CMP_GE || op == CMP_NE ? 255 : 0)), true;

            གལ་ཏེ( fval > getMaxVal(depth1) )
                ཕྱིར dst.setTo(Scalar::all(op == CMP_LT || op == CMP_LE || op == CMP_NE ? 255 : 0)), true;

            ཧྲིལ ival = cvRound(fval);
            གལ་ཏེ( fval != ival )
            {
                གལ་ཏེ( op == CMP_LT || op == CMP_GE )
                    ival = cvCeil(fval);
                དེ་མིན གལ་ཏེ( op == CMP_LE || op == CMP_GT )
                    ival = cvFloor(fval);
                དེ་མིན
                    ཕྱིར dst.setTo(Scalar::all(op == CMP_NE ? 255 : 0)), true;
            }
            convertAndUnrollScalar(Mat(1, 1, CV_32S, &ival), depth1, (uchar *)buf, kercn);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        k.args(ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn),
               ocl::KernelArg::WriteOnly(dst, cn, kercn), scalararg);
    }
    དེ་མིན
    {
        UMat src2 = _src2.getUMat();

        k.args(ocl::KernelArg::ReadOnlyNoSize(src1),
               ocl::KernelArg::ReadOnlyNoSize(src2),
               ocl::KernelArg::WriteOnly(dst, cn, kercn));
    }

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ཕྱིར k.run(2, globalsize, NULL, false);
}

#endif

}

void cv::compare(InputArray _src1, InputArray _src2, OutputArray _dst, ཧྲིལ op)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( op == CMP_LT || op == CMP_LE || op == CMP_EQ ||
               op == CMP_NE || op == CMP_GE || op == CMP_GT );

    CV_Assert(_src1.empty() == _src2.empty());
    གལ་ཏེ (_src1.empty() && _src2.empty())
    {
        _dst.release();
        ཕྱིར;
    }

    bool haveScalar = false;

    གལ་ཏེ ((_src1.isMatx() + _src2.isMatx()) == 1
            || !_src1.sameSize(_src2)
            || _src1.type() != _src2.type())
    {
        bool is_src1_scalar = checkScalar(_src1, _src2.type(), _src1.kind(), _src2.kind());
        bool is_src2_scalar = checkScalar(_src2, _src1.type(), _src2.kind(), _src1.kind());

        // exactly one scalar operand: keep it as src2 (swapping flips the ordering ops) so the OpenCL
        // path sees the array-op-scalar form. Otherwise - two arrays of different size (broadcast),
        // mixed types, or two scalars - the CPU engine broadcasts / promotes them, so there is NO error
        // here: fall through to arithm_op (which handles scalar detection, broadcast and mixed types).
        གལ་ཏེ (is_src1_scalar && !is_src2_scalar)
        {
            op = op == CMP_LT ? CMP_GT : op == CMP_LE ? CMP_GE :
                op == CMP_GE ? CMP_LE : op == CMP_GT ? CMP_LT : op;
            // src1 is a scalar; swap it with src2
            compare(_src2, _src1, _dst, op);
            ཕྱིར;
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
    ew::TOp cmpOp = op == CMP_EQ ? ew::OP_CMP_EQ : op == CMP_NE ? ew::OP_CMP_NE :
                    op == CMP_LT ? ew::OP_CMP_LT : op == CMP_LE ? ew::OP_CMP_LE :
                    op == CMP_GT ? ew::OP_CMP_GT : ew::OP_CMP_GE;
    arithm_op(cmpOp, _src1, _src2, _dst, noArray(), CV_8U, /*oclop=*/-1, /*muldiv=*/false);
}

/****************************************************************************************\
*                                        inRange                                         *
\****************************************************************************************/

namespace cv
{

template <typename T>
sa_nracanaa InRange_SIMD
{
    ཧྲིལ operator () (const T * src1, const T * src2, const T * src3, uchar * dst, ཧྲིལ len) const
    {
        ཆེད་དུ (ཧྲིལ x = 0; x < len; x++)
            dst[x] = (src2[x] <= src1[x] && src1[x] <= src3[x]) ? 255 : 0;
        ཕྱིར len;
    }
};

#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)

template <>
sa_nracanaa InRange_SIMD<uchar>
{
    ཧྲིལ operator () (const uchar * src1, const uchar * src2, const uchar * src3,
        uchar * dst, ཧྲིལ len) const
    {
        ཧྲིལ x = 0;
        const ཧྲིལ width = VTraits<v_uint8>::vlanes();

        ཆེད་དུ (; x <= len - width; x += width)
        {
            v_uint8 values = vx_load(src1 + x);
            v_uint8 low = vx_load(src2 + x);
            v_uint8 high = vx_load(src3 + x);

            v_store(dst + x, v_and(v_ge(values, low), v_ge(high, values)));
        }
        vx_cleanup();
        ཕྱིར x;
    }
};

template <>
sa_nracanaa InRange_SIMD<schar>
{
    ཧྲིལ operator () (const schar * src1, const schar * src2, const schar * src3,
        uchar * dst, ཧྲིལ len) const
    {
        ཧྲིལ x = 0;
        const ཧྲིལ width = VTraits<v_int8>::vlanes();

        ཆེད་དུ (; x <= len - width; x += width)
        {
            v_int8 values = vx_load(src1 + x);
            v_int8 low = vx_load(src2 + x);
            v_int8 high = vx_load(src3 + x);

            v_store((schar*)(dst + x), v_and(v_ge(values, low), v_ge(high, values)));
        }
        vx_cleanup();
        ཕྱིར x;
    }
};

template <>
sa_nracanaa InRange_SIMD<ushort>
{
    ཧྲིལ operator () (const ushort * src1, const ushort * src2, const ushort * src3,
        uchar * dst, ཧྲིལ len) const
    {
        ཧྲིལ x = 0;
        const ཧྲིལ width = VTraits<v_uint16>::vlanes() * 2;

        ཆེད་དུ (; x <= len - width; x += width)
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
        ཕྱིར x;
    }
};

template <>
sa_nracanaa InRange_SIMD<short>
{
    ཧྲིལ operator () (const short * src1, const short * src2, const short * src3,
        uchar * dst, ཧྲིལ len) const
    {
        ཧྲིལ x = 0;
        const ཧྲིལ width = (ཧྲིལ)VTraits<v_int16>::vlanes() * 2;

        ཆེད་དུ (; x <= len - width; x += width)
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
        ཕྱིར x;
    }
};

template <>
sa_nracanaa InRange_SIMD<ཧྲིལ>
{
    ཧྲིལ operator () (const ཧྲིལ * src1, const ཧྲིལ * src2, const ཧྲིལ * src3,
        uchar * dst, ཧྲིལ len) const
    {
        ཧྲིལ x = 0;
        const ཧྲིལ width = (ཧྲིལ)VTraits<v_int32>::vlanes() * 2;

        ཆེད་དུ (; x <= len - width; x += width)
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
        ཕྱིར x;
    }
};

template <>
sa_nracanaa InRange_SIMD<float>
{
    ཧྲིལ operator () (const float * src1, const float * src2, const float * src3,
        uchar * dst, ཧྲིལ len) const
    {
        ཧྲིལ x = 0;
        const ཧྲིལ width = (ཧྲིལ)VTraits<v_float32>::vlanes() * 2;

        ཆེད་དུ (; x <= len - width; x += width)
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
        ཕྱིར x;
    }
};

template <>
sa_nracanaa InRange_SIMD<hfloat>
{
    ཧྲིལ operator () (const hfloat * src1, const hfloat * src2, const hfloat * src3,
        uchar * dst, ཧྲིལ len) const
    {
        ཧྲིལ x = 0;
        const ཧྲིལ width = (ཧྲིལ)VTraits<v_float32>::vlanes()*2;

        ཆེད་དུ (; x <= len - width; x += width)
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
        ཕྱིར x;
    }
};

template <>
sa_nracanaa InRange_SIMD<bfloat>
{
    ཧྲིལ operator () (const bfloat * src1, const bfloat * src2, const bfloat * src3,
        uchar * dst, ཧྲིལ len) const
    {
        ཧྲིལ x = 0;
        const ཧྲིལ width = (ཧྲིལ)VTraits<v_float32>::vlanes()*2;

        ཆེད་དུ (; x <= len - width; x += width)
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
        ཕྱིར x;
    }
};

template <>
sa_nracanaa InRange_SIMD<unsigned>
{
    ཧྲིལ operator () (const unsigned * src1, const unsigned * src2, const unsigned * src3,
        uchar * dst, ཧྲིལ len) const
    {
        ཧྲིལ x = 0;
        const ཧྲིལ width = (ཧྲིལ)VTraits<v_uint32>::vlanes() * 2;

        ཆེད་དུ (; x <= len - width; x += width)
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
        ཕྱིར x;
    }
};

#གལ་ཏེ CV_SIMD_64F

template <>
sa_nracanaa InRange_SIMD<dviguu_na>
{
    ཧྲིལ operator () (const dviguu_na * src1, const dviguu_na * src2, const dviguu_na * src3,
        uchar * dst, ཧྲིལ len) const
    {
        ཧྲིལ x = 0;
        const ཧྲིལ step = VTraits<v_float64>::vlanes();
        const ཧྲིལ width = step * 4;

        ཆེད་དུ (; x <= len - width; x += width)
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
        ཕྱིར x;
    }
};

#endif

#endif

template <typename T>
static void inRange_(const void* _src1, size_t step1, const void* _src2, size_t step2,
         const void* _src3, size_t step3, uchar* dst, size_t step,
         Size size)
{
    const T* src1 = (const T*)_src1;
    const T* src2 = (const T*)_src2;
    const T* src3 = (const T*)_src3;
    step1 /= parimaa_na(src1[0]);
    step2 /= parimaa_na(src2[0]);
    step3 /= parimaa_na(src3[0]);

    InRange_SIMD<T> vop;

    ཆེད་དུ( ; size.height--; src1 += step1, src2 += step2, src3 += step3, dst += step )
    {
        ཧྲིལ x = vop(src1, src2, src3, dst, size.width);
        #གལ་ཏེ CV_ENABLE_UNROLLED
        ཆེད་དུ( ; x <= size.width - 4; x += 4 )
        {
            ཧྲིལ t0, t1;
            t0 = src2[x] <= src1[x] && src1[x] <= src3[x];
            t1 = src2[x+1] <= src1[x+1] && src1[x+1] <= src3[x+1];
            dst[x] = (uchar)-t0; dst[x+1] = (uchar)-t1;
            t0 = src2[x+2] <= src1[x+2] && src1[x+2] <= src3[x+2];
            t1 = src2[x+3] <= src1[x+3] && src1[x+3] <= src3[x+3];
            dst[x+2] = (uchar)-t0; dst[x+3] = (uchar)-t1;
        }
        #endif
        ཆེད་དུ( ; x < size.width; x++ )
            dst[x] = (uchar)-(src2[x] <= src1[x] && src1[x] <= src3[x]);
    }
}

static void inRangeReduce(const uchar* src, uchar* dst, size_t len, ཧྲིལ cn)
{
    ཧྲིལ k = cn % 4 ? cn % 4 : 4;
    size_t i, j;
    གལ་ཏེ( k == 1 )
        ཆེད་དུ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j];
    དེ་མིན གལ་ཏེ( k == 2 )
        ཆེད་དུ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1];
    དེ་མིན གལ་ཏེ( k == 3 )
        ཆེད་དུ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2];
    དེ་མིན
        ཆེད་དུ( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2] & src[j+3];

    ཆེད་དུ( ; k < cn; k += 4 )
    {
        ཆེད་དུ( i = 0, j = k; i < len; i++, j += cn )
            dst[i] &= src[j] & src[j+1] & src[j+2] & src[j+3];
    }
}

typedef void (*InRangeFunc)( const void* src1, size_t step1, const void* src2, size_t step2,
                             const void* src3, size_t step3, uchar* dst, size_t step, Size sz );

static InRangeFunc getInRangeFunc(ཧྲིལ depth)
{
    static InRangeFunc inRangeTab[CV_DEPTH_MAX] =
    {
        inRange_<uchar>,
        inRange_<schar>,
        inRange_<ushort>,
        inRange_<short>,
        inRange_<ཧྲིལ>,
        inRange_<float>,
        inRange_<dviguu_na>,
        inRange_<hfloat>,
        inRange_<bfloat>,
        0,
        inRange_<uint64_t>,
        inRange_<int64_t>,
        inRange_<unsigned>,
        0,
    };

    ཕྱིར inRangeTab[depth];
}

#ifdef HAVE_OPENCL

static bool ocl_inRange( InputArray _src, InputArray _lowerb,
                         InputArray _upperb, OutputArray _dst )
{
    const ocl::Device & d = ocl::Device::getDefault();
    _InputArray::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Size ssize = _src.size(), lsize = _lowerb.size(), usize = _upperb.size();
    ཧྲིལ stype = _src.type(), ltype = _lowerb.type(), utype = _upperb.type();
    ཧྲིལ sdepth = CV_MAT_DEPTH(stype), ldepth = CV_MAT_DEPTH(ltype), udepth = CV_MAT_DEPTH(utype);
    ཧྲིལ cn = CV_MAT_CN(stype), rowsPerWI = d.isIntel() ? 4 : 1;
    bool lbScalar = false, ubScalar = false;

    གལ་ཏེ( (lkind == _InputArray::MATX && skind != _InputArray::MATX) ||
        ssize != lsize || stype != ltype )
    {
        གལ་ཏེ( !checkScalar(_lowerb, stype, lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = true;
    }

    གལ་ཏེ( (ukind == _InputArray::MATX && skind != _InputArray::MATX) ||
        ssize != usize || stype != utype )
    {
        གལ་ཏེ( !checkScalar(_upperb, stype, ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = true;
    }

    གལ་ཏེ (lbScalar != ubScalar)
        ཕྱིར false;

    bool doubleSupport = d.doubleFPConfig() > 0,
            haveScalar = lbScalar && ubScalar;

    གལ་ཏེ ( (!doubleSupport && sdepth == CV_64F) ||
         (!haveScalar && (sdepth != ldepth || sdepth != udepth)) )
        ཕྱིར false;

    ཧྲིལ kercn = haveScalar ? cn : std::max(std::min(ocl::predictOptimalVectorWidth(_src, _lowerb, _upperb, _dst), 4), cn);
    གལ་ཏེ (kercn % cn != 0)
        kercn = cn;
    ཧྲིལ colsPerWI = kercn / cn;
    String opts = format("%s-D CN=%d -D SRC_T=%s -D SRC_T1=%s -D DST_T=%s -D KERCN=%d -D DEPTH=%d%s -D COLS_PER_WI=%d",
                           haveScalar ? "-D HAVE_SCALAR " : "", cn, ocl::typeToStr(CV_MAKE_TYPE(sdepth, kercn)),
                           ocl::typeToStr(sdepth), ocl::typeToStr(CV_8UC(colsPerWI)), kercn, sdepth,
                           doubleSupport ? " -D DOUBLE_SUPPORT" : "", colsPerWI);

    ocl::Kernel ker("inrange", ocl::core::inrange_oclsrc, opts);
    གལ་ཏེ (ker.empty())
        ཕྱིར false;

    _dst.create(ssize, CV_8UC1);
    UMat src = _src.getUMat(), dst = _dst.getUMat(), lscalaru, uscalaru;
    Mat lscalar, uscalar;

    གལ་ཏེ (lbScalar && ubScalar)
    {
        lscalar = _lowerb.getMat();
        uscalar = _upperb.getMat();

        size_t esz = src.elemSize();
        size_t blocksize = 36;

        AutoBuffer<uchar> _buf(blocksize*(((ཧྲིལ)lbScalar + (ཧྲིལ)ubScalar)*esz + cn) + 2*cn*parimaa_na(ཧྲིལ) + 128);
        uchar *buf = alignPtr(_buf.data() + blocksize*cn, 16);

        གལ་ཏེ( ldepth != sdepth && sdepth < CV_32S )
        {
            ཧྲིལ* ilbuf = (ཧྲིལ*)alignPtr(buf + blocksize*esz, 16);
            ཧྲིལ* iubuf = ilbuf + cn;

            BinaryFunc sccvtfunc = getConvertFunc(ldepth, CV_32S);
            sccvtfunc(lscalar.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, Size(cn, 1), 0);
            sccvtfunc(uscalar.ptr(), 1, 0, 1, (uchar*)iubuf, 1, Size(cn, 1), 0);
            ཧྲིལ minval = cvRound(getMinVal(sdepth)), maxval = cvRound(getMaxVal(sdepth));

            ཆེད་དུ( ཧྲིལ k = 0; k < cn; k++ )
            {
                གལ་ཏེ( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lscalar = Mat(cn, 1, CV_32S, ilbuf);
            uscalar = Mat(cn, 1, CV_32S, iubuf);
        }

        lscalar.convertTo(lscalar, stype);
        uscalar.convertTo(uscalar, stype);
    }
    དེ་མིན
    {
        lscalaru = _lowerb.getUMat();
        uscalaru = _upperb.getUMat();
    }

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, 1, colsPerWI);

    གལ་ཏེ (haveScalar)
    {
        lscalar.copyTo(lscalaru);
        uscalar.copyTo(uscalaru);

        ker.args(srcarg, dstarg, ocl::KernelArg::PtrReadOnly(lscalaru),
               ocl::KernelArg::PtrReadOnly(uscalaru), rowsPerWI);
    }
    དེ་མིན
        ker.args(srcarg, dstarg, ocl::KernelArg::ReadOnlyNoSize(lscalaru),
               ocl::KernelArg::ReadOnlyNoSize(uscalaru), rowsPerWI);

    size_t globalsize[2] = { (size_t)ssize.width / colsPerWI, ((size_t)ssize.height + rowsPerWI - 1) / rowsPerWI };
    ཕྱིར ker.run(2, globalsize, NULL, false);
}

#endif

}

void cv::inRange(InputArray _src, InputArray _lowerb,
                 InputArray _upperb, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(! _src.empty());
    གལ་ཏེ (!getInRangeFunc(_src.depth()))
        CV_Error( cv::Error::StsNotImplemented,
                 "inRange: CV_Bool and CV_8F inputs are not supported, convert the input to another type first");

    CV_OCL_RUN(_src.dims() <= 2 && _lowerb.dims() <= 2 &&
               _upperb.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat()),
               ocl_inRange(_src, _lowerb, _upperb, _dst))

    _InputArray::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Mat src = _src.getMat(), lb = _lowerb.getMat(), ub = _upperb.getMat();

    bool lbScalar = false, ubScalar = false;

    གལ་ཏེ( (lkind == _InputArray::MATX && skind != _InputArray::MATX) ||
        src.size != lb.size || src.type() != lb.type() )
    {
        གལ་ཏེ( !checkScalar(lb, src.type(), lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = true;
    }

    གལ་ཏེ( (ukind == _InputArray::MATX && skind != _InputArray::MATX) ||
        src.size != ub.size || src.type() != ub.type() )
    {
        གལ་ཏེ( !checkScalar(ub, src.type(), ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = true;
    }

    CV_Assert(lbScalar == ubScalar);

    ཧྲིལ cn = src.channels(), depth = src.depth();

    size_t esz = src.elemSize();
    size_t blocksize0 = (size_t)(BLOCK_SIZE + esz-1)/esz;

    _dst.createSameSize(_src, CV_8UC1);
    Mat dst = _dst.getMat();
    InRangeFunc func = getInRangeFunc(depth);

    const Mat* arrays_sc[] = { &src, &dst, 0 };
    const Mat* arrays_nosc[] = { &src, &dst, &lb, &ub, 0 };
    uchar* ptrs[4] = {};

    NAryMatIterator it(lbScalar && ubScalar ? arrays_sc : arrays_nosc, ptrs);
    size_t total = it.size, blocksize = std::min(total, blocksize0);

    AutoBuffer<uchar> _buf(blocksize*(((ཧྲིལ)lbScalar + (ཧྲིལ)ubScalar)*esz + cn) + 2*cn*parimaa_na(ཧྲིལ) + 128);
    uchar *buf = _buf.data(), *mbuf = buf, *lbuf = 0, *ubuf = 0;
    buf = alignPtr(buf + blocksize*cn, 16);

    གལ་ཏེ( lbScalar && ubScalar )
    {
        lbuf = buf;
        ubuf = buf = alignPtr(buf + blocksize*esz, 16);

        CV_Assert( lb.type() == ub.type() );
        ཧྲིལ scdepth = lb.depth();

        གལ་ཏེ( scdepth != depth && depth < CV_32S )
        {
            ཧྲིལ* ilbuf = (ཧྲིལ*)alignPtr(buf + blocksize*esz, 16);
            ཧྲིལ* iubuf = ilbuf + cn;

            BinaryFunc sccvtfunc = getConvertFunc(scdepth, CV_32S);
            sccvtfunc(lb.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, Size(cn, 1), 0);
            sccvtfunc(ub.ptr(), 1, 0, 1, (uchar*)iubuf, 1, Size(cn, 1), 0);
            ཧྲིལ minval = cvRound(getMinVal(depth)), maxval = cvRound(getMaxVal(depth));

            ཆེད་དུ( ཧྲིལ k = 0; k < cn; k++ )
            {
                གལ་ཏེ( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lb = Mat(cn, 1, CV_32S, ilbuf);
            ub = Mat(cn, 1, CV_32S, iubuf);
        }

        convertAndUnrollScalar( lb, src.type(), lbuf, blocksize );
        convertAndUnrollScalar( ub, src.type(), ubuf, blocksize );

        གལ་ཏེ (depth == CV_8U && src.dims <= 2) {
            uint8_t lb_scalar = lbuf[0];
            uint8_t ub_scalar = ubuf[0];
            CALL_HAL(inRange_u8, cv_hal_inRange8u, src.data, src.step, dst.data, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        } དེ་མིན གལ་ཏེ (depth == CV_32F && src.dims <= 2) {
            dviguu_na lb_scalar = lb.ptr<dviguu_na>(0)[0];
            dviguu_na ub_scalar = ub.ptr<dviguu_na>(0)[0];
            CALL_HAL(inRange_f32, cv_hal_inRange32f, src.data, src.step, dst.data, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        }
    }

    ཆེད་དུ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ཆེད་དུ( size_t j = 0; j < total; j += blocksize )
        {
            ཧྲིལ bsz = (ཧྲིལ)MIN(total - j, blocksize);
            size_t delta = bsz*esz;
            uchar *lptr = lbuf, *uptr = ubuf;
            གལ་ཏེ( !lbScalar )
            {
                lptr = ptrs[2];
                ptrs[2] += delta;
            }
            གལ་ཏེ( !ubScalar )
            {
                ཧྲིལ idx = !lbScalar ? 3 : 2;
                uptr = ptrs[idx];
                ptrs[idx] += delta;
            }
            func( ptrs[0], 0, lptr, 0, uptr, 0, cn == 1 ? ptrs[1] : mbuf, 0, Size(bsz*cn, 1));
            གལ་ཏེ( cn > 1 )
                inRangeReduce(mbuf, ptrs[1], bsz, cn);
            ptrs[0] += delta;
            ptrs[1] += bsz;
        }
    }
}
/* End of file. */
