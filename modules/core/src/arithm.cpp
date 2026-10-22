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

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "arithm_expr.hpp"   // the new element-wise engine (cv::ew)
#અન્તર્ભાવ "opencl_kernels_core.hpp"

નામાકાશ cv
{

/****************************************************************************************\
*                                   logical operations                                   *
\****************************************************************************************/

ગણના { OCL_OP_ADD=0, OCL_OP_SUB=1, OCL_OP_RSUB=2, OCL_OP_ABSDIFF=3, OCL_OP_MUL=4,
       OCL_OP_MUL_SCALE=5, OCL_OP_DIV_SCALE=6, OCL_OP_RECIP_SCALE=7, OCL_OP_ADDW=8,
       OCL_OP_AND=9, OCL_OP_OR=10, OCL_OP_XOR=11, OCL_OP_NOT=12, OCL_OP_MIN=13, OCL_OP_MAX=14,
       OCL_OP_RDIV_SCALE=15 };

// The unified entry point for every element-wise binary op (empty + UMat/OpenCL + CPU engine). Defined
// lower down (near cv::add); forward-declared here so cv::min/cv::max (above it) can use it too.
સ્થિર શૂન્ય arithm_op(ew::ટોચ op, આગત_Array src1, આગત_Array src2, OutputArray dst,
                      આગત_Array mask, પૂર્ણાંક dtype, પૂર્ણાંક oclop, તાર્કિક muldiv, અચલ Scalar& params = Scalar(1));

// The unary counterpart (bitwise NOT, unary math later). Forward-declared for the same reason.
સ્થિર શૂન્ય unary_op(ew::ટોચ op, આગત_Array src, OutputArray dst, આગત_Array mask,
                     પૂર્ણાંક dtype, અચલ Scalar& params = Scalar());

#ifdef HAVE_OPENCL

સ્થિર અચલ અક્ષર* oclop2str[] = { "OP_ADD", "OP_SUB", "OP_RSUB", "OP_ABSDIFF",
    "OP_MUL", "OP_MUL_SCALE", "OP_DIV_SCALE", "OP_RECIP_SCALE",
    "OP_ADDW", "OP_AND", "OP_OR", "OP_XOR", "OP_NOT", "OP_MIN", "OP_MAX", "OP_RDIV_SCALE", 0 };

// depths with no OpenCL type name (ocl::typeToStr() asserts on them)
સ્થિર અંતઃરેખ તાર્કિક noOclType(પૂર્ણાંક depth)
{
    પાછા depth == CV_16BF || depth == CV_Bool || depth == CV_8F_E4M3FN || depth == CV_8F_E4M3FNUZ;
}

સ્થિર તાર્કિક ocl_દ્વિઅંકી_op(આગત_Array _src1, આગત_Array _src2, OutputArray _dst,
                          આગત_Array _mask, તાર્કિક bitwise, પૂર્ણાંક oclop, તાર્કિક haveScalar )
{
    તાર્કિક haveMask = !_mask.empty();
    પૂર્ણાંક srctype = _src1.type();
    પૂર્ણાંક srcdepth = CV_MAT_DEPTH(srctype);
    પૂર્ણાંક cn = CV_MAT_CN(srctype);

    અચલ ocl::Device d = ocl::Device::getDefault();
    તાર્કિક doubleSupport = d.doubleFPConfig() > 0;
    જો( oclop < 0 || ((haveMask || haveScalar) && cn > 4) ||
            (!doubleSupport && srcdepth == CV_64F && !bitwise) ||
            (noOclType(srcdepth) && !bitwise))
        પાછા ખોટું;

    અક્ષર opts[1024];
    પૂર્ણાંક kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    પૂર્ણાંક scalarcn = kercn == 3 ? 4 : kercn;
    પૂર્ણાંક rowsPerWI = d.isIntel() ? 4 : 1;

    અચલ પૂર્ણાંક dstDepth = srcdepth;
    અચલ પૂર્ણાંક dstType = CV_MAKETYPE(dstDepth, kercn);
    અચલ પૂર્ણાંક dstType1 = CV_MAKETYPE(dstDepth, 1);
    અચલ પૂર્ણાંક scalarType = CV_MAKETYPE(srcdepth, scalarcn);

    snprintf(opts, કદ(opts), "-D %s%s -D %s%s -D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workST=%s -D cn=%d -D rowsPerWI=%d",
            haveMask ? "MASK_" : "", haveScalar ? "UNARY_OP" : "BINARY_OP", oclop2str[oclop],
            doubleSupport ? " -D DOUBLE_SUPPORT" : "",
            bitwise ? ocl::memopTypeToStr(dstType) : ocl::typeToStr(dstType),
            dstDepth,
            bitwise ? ocl::memopTypeToStr(dstType1) : ocl::typeToStr(dstType1),
            bitwise ? ocl::memopTypeToStr(scalarType) : ocl::typeToStr(scalarType),
            kercn, rowsPerWI);

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    જો (k.empty())
        પાછા ખોટું;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask, 1);

    જો( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(srctype)*scalarcn;
        દ્વિગુણ buf[4] = {0,0,0,0};

        જો( oclop != OCL_OP_NOT )
        {
            Mat src2sc = _src2.getMat();
            convertAndUnrollScalar(src2sc, srctype, (uchar*)buf, 1);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        જો( !haveMask )
            k.args(src1arg, dstarg, scalararg);
        નહીંતર
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    નહીંતર
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::ReadOnlyNoSize(src2, cn, kercn);

        જો( !haveMask )
            k.args(src1arg, src2arg, dstarg);
        નહીંતર
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    પાછા k.run(2, globalsize, 0, ખોટું);
}

#endif

// OpenCL path for the bitwise ops (and/or/xor/not), lifted out of binary_op: choose the (possibly
// scalar) operand order, size the destination, and dispatch the shared bitwise OpenCL kernel. Returns
// true if OpenCL handled the op, false to fall through to the CPU engine. NOT arrives unary
// (src2 == src1, forced-scalar). The caller gates this on a UMat operand + dims <= 2 (via CV_OCL_RUN).
#ifdef HAVE_OPENCL
સ્થિર તાર્કિક bitwise_op_ocl(આગત_Array _src1, આગત_Array _src2, OutputArray _dst,
                           આગત_Array _mask, પૂર્ણાંક oclop)
{
    અચલ આગત_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    આગત_Array_2::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    પૂર્ણાંક type1 = psrc1->type(), type2 = psrc2->type();
    તાર્કિક haveScalar = ખોટું;

    જો( oclop == OCL_OP_NOT )
        haveScalar = સાચું;
    નહીંતર જો( (kind1 == આગત_Array_2::MATX) + (kind2 == આગત_Array_2::MATX) == 1 ||
             !psrc1->sameSize(*psrc2) || type1 != type2 )
    {
        જો( checkScalar(*psrc1, type2, kind1, kind2) )
        { std::swap(psrc1, psrc2); std::swap(type1, type2); }        // src1 is the scalar; swap it out
        નહીંતર જો( !checkScalar(*psrc2, type1, kind2, kind1) )
            પાછા ખોટું;
        haveScalar = સાચું;
    }
    _dst.createSameSize(*psrc1, type1);
    પાછા ocl_દ્વિઅંકી_op(*psrc1, *psrc2, _dst, _mask, સાચું, oclop, haveScalar);
}
#endif  // HAVE_OPENCL

}

// bitwise and/or/xor: OpenCL runs the shared bitwise kernel (via bitwise_op_ocl); the CPU path is the
// element-wise engine (OP_AND/OR/XOR, T x T -> T dispatched by element size). oclop = -1 tells
// arithm_op the OpenCL path was already tried, so it only runs the CPU engine. NOT is unary.
શૂન્ય cv::bitwise_and(આગત_Array a, આગત_Array b, OutputArray c, આગત_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_AND))
    arithm_op(ew::OP_AND, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/ખોટું);
}

શૂન્ય cv::bitwise_or(આગત_Array a, આગત_Array b, OutputArray c, આગત_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_OR))
    arithm_op(ew::OP_OR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/ખોટું);
}

શૂન્ય cv::bitwise_xor(આગત_Array a, આગત_Array b, OutputArray c, આગત_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_XOR))
    arithm_op(ew::OP_XOR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/ખોટું);
}

શૂન્ય cv::bitwise_not(આગત_Array a, OutputArray c, આગત_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || c.isUMat()) && a.dims() <= 2,
               bitwise_op_ocl(a, a, c, mask, OCL_OP_NOT))
    unary_op(ew::OP_NOT, a, c, mask, -1);
}

શૂન્ય cv::max( આગત_Array src1, આગત_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, ખોટું);
}

શૂન્ય cv::min( આગત_Array src1, આગત_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, ખોટું);
}

// The concrete Mat/UMat overloads of min/max exist because C++ overload resolution needs them (a
// bare cv::min(mat, mat, mat) binds Mat& more tightly than InputArray); they must run the SAME engine
// path as the InputArray forms - NOT the legacy binary_op - so mixed types and broadcasting work.
શૂન્ય cv::max(અચલ Mat& src1, અચલ Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, ખોટું);
}

શૂન્ય cv::min(અચલ Mat& src1, અચલ Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, ખોટું);
}

શૂન્ય cv::max(અચલ UMat& src1, અચલ UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, ખોટું);
}

શૂન્ય cv::min(અચલ UMat& src1, અચલ UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, ખોટું);
}


/****************************************************************************************\
*                                      add/subtract                                      *
\****************************************************************************************/

નામાકાશ cv
{

#ifdef HAVE_OPENCL

// used by the OpenCL branch only (arithm_op_ocl working-type selection)
સ્થિર પૂર્ણાંક actualScalarDepth(અચલ દ્વિગુણ* data, પૂર્ણાંક len)
{
    પૂર્ણાંક i = 0, minval = INT_MAX, maxval = INT_MIN;
    માટે(; i < len; ++i)
    {
        પૂર્ણાંક ival = cvRound(data[i]);
        જો( ival != data[i] )
            તોડો;
        minval = MIN(minval, ival);
        maxval = MAX(maxval, ival);
    }
    પાછા i < len ? CV_64F :
        minval >= 0 && maxval <= (પૂર્ણાંક)UCHAR_MAX ? CV_8U :
        minval >= (પૂર્ણાંક)SCHAR_MIN && maxval <= (પૂર્ણાંક)SCHAR_MAX ? CV_8S :
        minval >= 0 && maxval <= (પૂર્ણાંક)USHRT_MAX ? CV_16U :
        minval >= (પૂર્ણાંક)SHRT_MIN && maxval <= (પૂર્ણાંક)SHRT_MAX ? CV_16S :
        CV_32S;
}

સ્થિર પૂર્ણાંક coerceTypes(પૂર્ણાંક depth1, પૂર્ણાંક depth2, તાર્કિક muldiv)
{
    પાછા depth1 == depth2 ? depth1 :
        ((depth1 <= CV_32S) & (depth2 <= CV_32S)) != 0 ?
        (((પૂર્ણાંક)!muldiv & (depth1 <= CV_8S) & (depth2 <= CV_8S)) != 0 ? CV_16S : CV_32S) :
        ((CV_ELEM_SIZE1(depth1) > 4) | (CV_ELEM_SIZE1(depth2) > 4)) != 0 ? CV_64F : CV_32F;
}

સ્થિર તાર્કિક ocl_arithm_op(આગત_Array _src1, આગત_Array _src2, OutputArray _dst,
                          આગત_Array _mask, પૂર્ણાંક wtype,
                          શૂન્ય* usrdata, પૂર્ણાંક oclop,
                          તાર્કિક haveScalar )
{
    અચલ ocl::Device d = ocl::Device::getDefault();
    તાર્કિક doubleSupport = d.doubleFPConfig() > 0;
    પૂર્ણાંક type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    તાર્કિક haveMask = !_mask.empty();

    જો ( (haveMask || haveScalar) && cn > 4 )
        પાછા ખોટું;

#ifdef __APPLE__
    જો (depth1 == CV_16U && (oclop == OCL_OP_MUL || oclop == OCL_OP_MUL_SCALE))
        પાછા ખોટું;
#endif

    પૂર્ણાંક dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32S, CV_MAT_DEPTH(wtype));
    જો (!doubleSupport)
        wdepth = std::min(wdepth, CV_32F);

    wtype = CV_MAKETYPE(wdepth, cn);
    પૂર્ણાંક type2 = haveScalar ? wtype : _src2.type(), depth2 = CV_MAT_DEPTH(type2);
    જો (!doubleSupport && (depth2 == CV_64F || depth1 == CV_64F))
        પાછા ખોટું;

    જો (noOclType(depth1) || noOclType(depth2) || noOclType(ddepth) || noOclType(wdepth))
        પાછા ખોટું;

    પૂર્ણાંક kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    પૂર્ણાંક scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = d.isIntel() ? 4 : 1;

    અક્ષર cvtstr[4][50], opts[1024];
    snprintf(opts, કદ(opts), "-D %s%s -D %s -D srcT1=%s -D srcT1_C1=%s -D srcT2=%s -D srcT2_C1=%s "
            "-D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workT=%s -D workST=%s -D scaleT=%s -D wdepth=%d -D convertToWT1=%s "
            "-D convertToWT2=%s -D convertToDT=%s%s -D cn=%d -D rowsPerWI=%d -D convertFromU=%s",
            (haveMask ? "MASK_" : ""), (haveScalar ? "UNARY_OP" : "BINARY_OP"),
            oclop2str[oclop], ocl::typeToStr(CV_MAKETYPE(depth1, kercn)),
            ocl::typeToStr(depth1), ocl::typeToStr(CV_MAKETYPE(depth2, kercn)),
            ocl::typeToStr(depth2), ocl::typeToStr(CV_MAKETYPE(ddepth, kercn)), ddepth,
            ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKETYPE(wdepth, kercn)),
            ocl::typeToStr(CV_MAKETYPE(wdepth, scalarcn)),
            ocl::typeToStr(wdepth), wdepth,
            ocl::convertTypeStr(depth1, wdepth, kercn, cvtstr[0], કદ(cvtstr[0])),
            ocl::convertTypeStr(depth2, wdepth, kercn, cvtstr[1], કદ(cvtstr[1])),
            ocl::convertTypeStr(wdepth, ddepth, kercn, cvtstr[2], કદ(cvtstr[2])),
            doubleSupport ? " -D DOUBLE_SUPPORT" : "", kercn, rowsPerWI,
            oclop == OCL_OP_ABSDIFF && wdepth == CV_32S && ddepth == wdepth ?
            ocl::convertTypeStr(CV_8U, ddepth, kercn, cvtstr[3], કદ(cvtstr[3])) : "noconvert");

    size_t usrdata_esz = CV_ELEM_SIZE(wdepth);
    અચલ uchar* usrdata_p = (અચલ uchar*)usrdata;
    અચલ દ્વિગુણ* usrdata_d = (અચલ દ્વિગુણ*)usrdata;
    દશાંશ usrdata_f[3];
    પૂર્ણાંક i, n = oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE ||
        oclop == OCL_OP_RDIV_SCALE || oclop == OCL_OP_RECIP_SCALE ? 1 : oclop == OCL_OP_ADDW ? 3 : 0;
    જો( usrdata && n > 0 && wdepth == CV_32F )
    {
        માટે( i = 0; i < n; i++ )
            usrdata_f[i] = (દશાંશ)usrdata_d[i];
        usrdata_p = (અચલ uchar*)usrdata_f;
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    જો (k.empty())
        પાછા ખોટું;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask, 1);

    જો( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(wtype)*scalarcn;
        દ્વિગુણ buf[4]={0,0,0,0};
        Mat src2sc = _src2.getMat();

        જો( !src2sc.empty() )
            convertAndUnrollScalar(src2sc, wtype, (uchar*)buf, 1);
        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        જો( !haveMask )
        {
            જો(n == 0)
                k.args(src1arg, dstarg, scalararg);
            નહીંતર જો(n == 1)
                k.args(src1arg, dstarg, scalararg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            નહીંતર
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        નહીંતર
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    નહીંતર
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::ReadOnlyNoSize(src2, cn, kercn);

        જો( !haveMask )
        {
            જો (n == 0)
                k.args(src1arg, src2arg, dstarg);
            નહીંતર જો (n == 1)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            નહીંતર જો (n == 3)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz*2, usrdata_esz));
            નહીંતર
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        નહીંતર
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    પાછા k.run(2, globalsize, ખાલી, ખોટું);
}

#endif

// OpenCL branch of the arithmetic ops, extracted from the former arithm_op (whose CPU compute is now
// the element-wise engine). Returns true if an OpenCL kernel handled the op, false to let the caller
// fall through to the CPU engine. min/max use the bitwise-style kernel; every other op the arithm
// kernel with a coerced working type. `usrdata` carries the mul/div/addWeighted scale block, `oclop`
// the kernel id. Only reached for a UMat dst; sizes/types that don't fit the scalar/same-size pattern
// (e.g. a broadcast) decline (return false) so the engine handles them on the CPU.
સ્થિર તાર્કિક arithm_op_ocl(આગત_Array _src1, આગત_Array _src2, OutputArray _dst,
                          આગત_Array _mask, પૂર્ણાંક dtype, પૂર્ણાંક oclop, તાર્કિક muldiv, શૂન્ય* usrdata)
{
#ifndef HAVE_OPENCL
    CV_UNUSED(_src1); CV_UNUSED(_src2); CV_UNUSED(_dst); CV_UNUSED(_mask);
    CV_UNUSED(dtype); CV_UNUSED(oclop); CV_UNUSED(muldiv); CV_UNUSED(usrdata);
    પાછા ખોટું;
#નહીંતર
    અચલ આગત_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    આગત_Array_2::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    તાર્કિક haveMask = !_mask.empty();
    તાર્કિક reallocate = ખોટું;
    પૂર્ણાંક type1 = psrc1->type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    પૂર્ણાંક type2 = psrc2->type(), depth2 = CV_MAT_DEPTH(type2), cn2 = CV_MAT_CN(type2);
    પૂર્ણાંક wtype, dims1 = psrc1->dims(), dims2 = psrc2->dims();
    Size sz1 = dims1 <= 2 ? psrc1->size() : Size();
    Size sz2 = dims2 <= 2 ? psrc2->size() : Size();

    જો (!(OCL_PERFORMANCE_CHECK(_dst.isUMat()) && dims1 <= 2 && dims2 <= 2))
        પાછા ખોટું;

    // min/max: the bitwise-style OpenCL kernel (same-type element-wise, no working-type coercion).
    જો (oclop == OCL_OP_MIN || oclop == OCL_OP_MAX)
    {
        તાર્કિક haveScalar = ખોટું;
        જો ((kind1 == આગત_Array_2::MATX) + (kind2 == આગત_Array_2::MATX) == 1 ||
            !psrc1->sameSize(*psrc2) || type1 != type2)
        {
            જો (checkScalar(*psrc1, type2, kind1, kind2))
            { std::swap(psrc1, psrc2); std::swap(type1, type2); }
            નહીંતર જો (!checkScalar(*psrc2, type1, kind2, kind1))
                પાછા ખોટું;
            haveScalar = સાચું;
        }
        _dst.createSameSize(*psrc1, type1);
        પાછા ocl_દ્વિઅંકી_op(*psrc1, *psrc2, _dst, _mask, ખોટું, oclop, haveScalar);
    }

    // reciprocal (scale/src): the OpenCL kernel is unary on src - use src2 for both operands (the CPU
    // engine received a 0-dim `1` numerator as src1, which the recip kernel would ignore). [TODO.VP: tidy]
    જો (oclop == OCL_OP_RECIP_SCALE)
    {
        psrc1 = psrc2; kind1 = kind2; type1 = type2; depth1 = depth2; cn = cn2; dims1 = dims2; sz1 = sz2;
    }

    // add/sub/mul/div/absdiff/addWeighted/recip: the arithm OpenCL kernel with a coerced work type.
    તાર્કિક src1Scalar = checkScalar(*psrc1, type2, kind1, kind2);
    તાર્કિક src2Scalar = checkScalar(*psrc2, type1, kind2, kind1);
    તાર્કિક haveScalar = ખોટું;

    જો (dims1 != dims2 || sz1 != sz2 || cn != cn2 ||
        (kind1 == આગત_Array_2::MATX && (sz1 == Size(1,4) || sz1 == Size(1,1))) ||
        (kind2 == આગત_Array_2::MATX && (sz2 == Size(1,4) || sz2 == Size(1,1))))
    {
        જો ((type1 == CV_64F && (sz1.height == 1 || sz1.height == 4)) && src1Scalar)
        {
            // src1 is a scalar; swap it with src2
            std::swap(psrc1, psrc2); std::swap(sz1, sz2); std::swap(type1, type2);
            std::swap(depth1, depth2); std::swap(cn, cn2); std::swap(dims1, dims2);
            જો (oclop == OCL_OP_SUB) oclop = OCL_OP_RSUB;
            જો (oclop == OCL_OP_DIV_SCALE) oclop = OCL_OP_RDIV_SCALE;
        }
        નહીંતર જો (!src2Scalar)
            પાછા ખોટું;   // array op array with mismatched size/cn: engine broadcasts on the CPU
        haveScalar = સાચું;
        CV_Assert((type2 == CV_64F || type2 == CV_32F) && (sz2.height == 1 || sz2.height == 4));

        જો (!muldiv)
        {
            Mat sc = psrc2->getMat();
            depth2 = actualScalarDepth(sc.ptr<દ્વિગુણ>(), sz2 == Size(1, 1) ? cn2 : cn);
            જો (depth2 == CV_64F && CV_ELEM_SIZE1(depth1) < 8)
                depth2 = CV_32F;
        }
        નહીંતર
            depth2 = CV_64F;
    }

    જો (dtype < 0)
    {
        જો (_dst.fixedType())
            dtype = _dst.type();
        નહીંતર
        {
            જો (!haveScalar && type1 != type2)
                CV_Error(cv::Error::StsBadArg,
                     "When the input arrays in add/subtract/multiply/divide functions have different types, "
                     "the output array type must be explicitly specified");
            dtype = type1;
        }
    }
    dtype = CV_MAT_DEPTH(dtype);

    જો (depth1 == depth2 && dtype == depth1)
        wtype = dtype;
    નહીંતર જો (!muldiv)
    {
        wtype = coerceTypes(depth1, depth2, ખોટું);
        wtype = coerceTypes(wtype, dtype, ખોટું);
        જો (dtype < CV_32F && (depth1 < CV_32F || depth2 < CV_32F))
            wtype = CV_32S;
    }
    નહીંતર
    {
        wtype = coerceTypes(depth1, depth2, સાચું);
        wtype = coerceTypes(wtype, dtype, સાચું);
    }

    // The scaled OpenCL kernels (mul/div by a scale, reciprocal, addWeighted) compute in float; the old
    // same-type fast path forced max(depth, CV_32F). coerceTypes keeps same-type integer, so bump here.
    જો (oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE || oclop == OCL_OP_RDIV_SCALE ||
        oclop == OCL_OP_RECIP_SCALE || oclop == OCL_OP_ADDW)
        wtype = std::max(wtype, (પૂર્ણાંક)CV_32F);

    dtype = CV_MAKETYPE(dtype, cn);
    wtype = CV_MAKETYPE(wtype, cn);

    જો (haveMask)
    {
        પૂર્ણાંક mtype = _mask.type();
        CV_Assert((mtype == CV_8UC1 || mtype == CV_8SC1 || mtype == CV_Bool) && _mask.sameSize(*psrc1));
        reallocate = !_dst.sameSize(*psrc1) || _dst.type() != dtype;
    }

    _dst.createSameSize(*psrc1, dtype);
    જો (reallocate)
        _dst.setTo(0.);

    પાછા ocl_arithm_op(*psrc1, *psrc2, _dst, _mask, wtype, usrdata, oclop, haveScalar);
#endif
}

// Element-wise binary op (ADD/SUB/...) on two CPU operands - each an array Mat or a MATX scalar -
// through the new broadcasting engine. Handles mixed input types (emitBinary inserts the casts) and
// broadcasting (a scalar rides as a 0-dim per-channel CONST; arrays broadcast via outputShape/exec).
// Returns false (declining) for UMat, a write-mask, or two scalar operands, so the caller falls
// through to arithm_op. Incompatible shapes make the engine throw (matching cv::add's error).
// Does the output array already have exactly this shape+type (=> create() reuses it, no realloc)?
// Works for Mat and UMat alike (shape/type queries, no data-pointer peeking).
સ્થિર તાર્કિક outArrayMatches(અચલ _OutputArray& a, અચલ MatShape& shp, પૂર્ણાંક type)
{
    જો (a.empty() || a.type() != type)
        પાછા ખોટું;
    પૂર્ણાંક sz[MatShape::MAX_DIMS];
    પૂર્ણાંક nd = a.sizend(sz);
    જો (nd != (પૂર્ણાંક)shp.size())
        પાછા ખોટું;
    માટે (પૂર્ણાંક i = 0; i < nd; i++)
        જો (sz[i] != shp[i])
            પાછા ખોટું;
    પાછા સાચું;
}

// Are two operands' iteration shapes (spatial dims + channels innermost, exactly how the engine
// iterates) numpy-broadcast-compatible? Powers the scalar-like-Mat compat fallback in arithm_op.
// Cheap by design: sizend copies the dim arrays inline - no Mat headers, no allocations.
સ્થિર તાર્કિક shapesBroadcastCompat(આગત_Array a, પૂર્ણાંક acn, આગત_Array b, પૂર્ણાંક bcn)
{
    પૂર્ણાંક asz[MatShape::MAX_DIMS + 1], bsz[MatShape::MAX_DIMS + 1];
    પૂર્ણાંક ad = a.sizend(asz), bd = b.sizend(bsz);
    asz[ad++] = acn;
    bsz[bd++] = bcn;
    માટે (પૂર્ણાંક i1 = ad - 1, i2 = bd - 1; i1 >= 0 && i2 >= 0; i1--, i2--)
        જો (asz[i1] != bsz[i2] && asz[i1] != 1 && bsz[i2] != 1)
            પાછા ખોટું;
    પાછા સાચું;
}

// The one entry point for every element-wise binary op (add/sub/mul/div/min/max/absdiff/addWeighted/
// reciprocal). Handles empty inputs, the UMat/OpenCL branch (arithm_op_ocl) and the CPU element-wise
// engine. `oclop` selects the OpenCL kernel; `muldiv` drives the OpenCL working-type coercion + scale;
// `params` are the op scalars (params[0]=mul/div scale; {alpha,beta,gamma} for addWeighted).
// Below this many ELEMENTS (total * channels), the per-call constant overhead of the engine path
// (program build + compile + executor setup) is comparable to the actual work - take the direct
// kernel call instead. Same tier boundary as math_op's MATH_OP_SMALL for cv::exp.
ગણના { ARITHM_SMALL_DIRECT = 100000 };

સ્થિર શૂન્ય arithm_op(ew::ટોચ op, આગત_Array src1, આગત_Array src2, OutputArray dst,
                      આગત_Array mask, પૂર્ણાંક dtype, પૂર્ણાંક oclop, તાર્કિક muldiv, અચલ Scalar& params)
{
    જો (src1.empty() || src2.empty())          // empty inputs -> empty result
    {
        CV_Assert((src1.empty() && src2.empty()) ||
                  (src1.empty() && isScalarArg(src2, src1.channels())) ||
                  (src2.empty() && isScalarArg(src1, src2.channels())));
        dst.release();
        જો (dtype >= 0)
            dst.create(0, 0, dtype);
        પાછા;
    }

    // UMat -> classic OpenCL kernel. It declines (false) when OpenCL can't apply (no device, dims>2, a
    // broadcast, cn>4 masked, ...) - then the CPU engine below handles it (mapping the UMat via getMat).
    // oclop < 0 is a sentinel: the caller (e.g. cv::compare) already ran its own OpenCL path, so this
    // helper only runs the CPU engine (a UMat operand is mapped via getMat).
    જો (oclop >= 0 && cv::ocl::isOpenCLActivated() &&
        (src1.isUMat() || src2.isUMat() || dst.isUMat()))
    {
        દ્વિગુણ abg[3] = { params[0], params[1], params[2] }, scale = params[0];
        શૂન્ય* usrdata = (op == ew::OP_ADDW) ? (શૂન્ય*)abg : (muldiv ? (શૂન્ય*)&scale : nullptr);
        જો (arithm_op_ocl(src1, src2, dst, mask, dtype, oclop, muldiv, usrdata))
            પાછા;
    }

    અચલ તાર્કિક haveMask = !mask.empty();
    અચલ પૂર્ણાંક cn1 = src1.channels(), cn2 = src2.channels();
    તાર્કિક s1 = isScalarArg(src1, src2), s2 = isScalarArg(src2, src1);
    જો (s1 && s2)
        s1 = s2 = ખોટું;    // two scalars: treat both as tiny array operands (element-wise + broadcast),
                            // matching arithm_op (Scalar+Scalar -> 4x1, number+number -> 1x1, ...)
    // A 4x1 CV_64F Mat/UMat pseudo-scalar (see isScalarArg) is hijacked only against a REAL array:
    // when the partner is itself a tiny scalar-shaped array (e.g. compare(Mat 4x1, Mat 1x1) -
    // issue #8999), both are honest data and ride the broadcast. A genuine MATX Scalar never demotes.
    નહીંતર જો (s1 && src1.kind() != આગત_Array_2::MATX && isScalarLikeMat(src2, cn1))
        s1 = ખોટું;
    નહીંતર જો (s2 && src2.kind() != આગત_Array_2::MATX && isScalarLikeMat(src1, cn2))
        s2 = ખોટું;

    // Compat fallback: Java/Python/user code passes scalars as real little Mats (the classic 4x1
    // CV_64F column, a 1xcn/cnx1 vector - see isScalarLikeMat). Those normally ride broadcasting
    // now, so hijack one as a per-channel scalar ONLY when the shapes do NOT broadcast - just the
    // calls that would otherwise throw get the 4.x scalar semantics, every valid broadcast keeps
    // its numpy meaning. Cheap: two geometry probes, and the O(ndims) shape walk runs only when a
    // probe hits (never for ordinary same-size or MATX-scalar calls).
    જો (!s1 && !s2)
    {
        તાર્કિક like1 = isScalarLikeMat(src1, cn2), like2 = isScalarLikeMat(src2, cn1);
        જો ((like1 || like2) && !shapesBroadcastCompat(src1, cn1, src2, cn2))
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
    અચલ શૂન્ય* dstObj = (dst.kind() == આગત_Array_2::MAT) ? dst.getObj() : nullptr;
    અચલ તાર્કિક aliased = dstObj && (dstObj == src1.getObj() || dstObj == src2.getObj());
    Mat m1loc, m2loc, mloc;
    auto asMat = [&](આગત_Array a, Mat& loc) -> અચલ Mat* {
        જો (a.kind() == આગત_Array_2::MAT && !aliased) પાછા (અચલ Mat*)a.getObj();
        loc = a.getMat(); પાછા &loc;
    };
    અચલ Mat* pm1 = s1 ? nullptr : asMat(src1, m1loc);
    અચલ Mat* pm2 = s2 ? nullptr : asMat(src2, m2loc);
    અચલ પૂર્ણાંક adepth = s1 ? pm2->depth() : pm1->depth();   // the (first) array operand's depth
    // auto result depth (no explicit dtype, no fixed-type dst): add/sub/mul/div keep the first array
    // operand's depth (cv::'s dtype==-1 convention), but min/max/absdiff have NO dtype argument and
    // must promote mixed inputs to their common type (min(u32,f32) in u32 would drop the float). For
    // same-type inputs promoteArith(da,db)==da, so classic behaviour is preserved (e.g. absdiff of two
    // s16 stays s16, saturating - the wide unsigned |a-b| is computed then cast back down by emitBinary).
    પૂર્ણાંક autoDepth = adepth;
    જો (op == ew::OP_MIN || op == ew::OP_MAX || op == ew::OP_ABSDIFF)
    {
        અચલ પૂર્ણાંક d1 = s1 ? ew::EW_DEPTH_NONE : pm1->depth();
        અચલ પૂર્ણાંક d2 = s2 ? ew::EW_DEPTH_NONE : pm2->depth();
        autoDepth = ew::promoteArith(d1, d2);
    }
    // result depth: explicit dtype wins; else a fixed-type dst dictates it; else the auto depth above.
    અચલ પૂર્ણાંક rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
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
    જો (!haveMask && !s1 && !s2 &&
        (op == ew::OP_ADD || op == ew::OP_SUB || op == ew::OP_MIN || op == ew::OP_MAX ||
         op == ew::OP_ABSDIFF || op == ew::OP_MUL || op == ew::OP_ADDW ||
         op == ew::OP_AND || op == ew::OP_OR || op == ew::OP_XOR) &&
        rdepth == pm1->depth() && pm1->type() == pm2->type() && pm1->size == pm2->size &&
        pm1->isContinuous() && pm2->isContinuous() &&
        pm1->total()*cn1 <= (size_t)ARITHM_SMALL_DIRECT)
    {
        ew::TKernel k = ew::getElemwiseFunc(op, rdepth, rdepth, ew::EW_DEPTH_NONE, rdepth);
        જો (k.fptr)
        {
            dst.createSameSize(src1, pm1->type());   // whole-shape transfer, not piecemeal dims+sizes
            Mat dloc;
            Mat* pd = (dst.kind() == આગત_Array_2::MAT) ? (Mat*)dst.getObj() : &(dloc = dst.getMat());
            જો (pd->isContinuous())     // a reused non-continuous dst view falls through to the engine
            {
                k.fptr(pm1->data, 0, 1, pm2->data, 0, 1, nullptr, 0, 0,
                       pd->data, 0, (પૂર્ણાંક)(pm1->total()*cn1), 1, params.val, k.flags, k.userdata);
                પાછા;
            }
        }
    }

    // operand order = (src1, src2): an array becomes an INPUT, a scalar a flexible per-channel CONST
    // read straight from the caller's inline storage (getObj()), no Mat / convertTo.
    ew::TExpr p;
    auto addOperand = [&](આગત_Array src, તાર્કિક isScalar, અચલ Mat* m, પૂર્ણાંક otherCn) -> પૂર્ણાંક {
        જો (!isScalar)
            પાછા p.add_આગત(m->depth());
        અચલ uchar* sp; પૂર્ણાંક sd;
        uchar scbuf[EW_SCALAR_BUF_SIZE];   // stack room for a UMAT scalar; addConst copies the values out
        અચલ પૂર્ણાંક scn0 = scalarArgElems(src, sp, sd, scbuf), cn = otherCn;
        અચલ પૂર્ણાંક scn = (cn == scn0 || (cn < 4 && scn0 == 4)) ? cn
                      : (scn0 == 1) ? 1 : 0;
        CV_Assert(scn > 0);
        પાછા p.addConst(ew::EW_DEPTH_NONE, sd, sp, scn);
    };
    અચલ પૂર્ણાંક a0 = addOperand(src1, s1, pm1, cn2);
    અચલ પૂર્ણાંક a1 = addOperand(src2, s2, pm2, cn1);

    // Write-mask: a single-channel 1-byte array (u8/s8/bool) the size of the output spatial shape,
    // added as another broadcast input. The arithmetic result lands in a temp; a final
    // select(mask, r, dst) -> dst overwrites only mask!=0 positions of the (pre-existing) output.
    પૂર્ણાંક sMask = 0;
    અચલ Mat* pmask = nullptr;
    જો (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = asMat(mask, mloc);
        sMask = p.add_આગત(pmask->depth());
    }
    અચલ પૂર્ણાંક sOut = p.addOutput(rdepth);               // output BEFORE emit -> moveToOutput drops the
    અચલ પૂર્ણાંક r = p.emit_દ્વિઅંકી(op, a0, a1, rdepth, params);   // result temp (mask-free => none); scale
                                                        // rides params[0] (mul/div), {a,b,g} for addW
    જો (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    નહીંતર
        p.moveToOutput(r, sOut);
    p.compile();

    અચલ Mat* inputs[3]; પૂર્ણાંક ni = 0;                   // inputs in program (input-index) order
    જો (!s1) inputs[ni++] = pm1;
    જો (!s2) inputs[ni++] = pm2;
    જો (haveMask) inputs[ni++] = pmask;

    // Pre-create the output at the broadcast shape, ALWAYS via dst.create() - never hand exec a raw
    // dst Mat. Routing through the _OutputArray is what enforces its contract: a FIXED_SIZE/FIXED_TYPE
    // dst (in-place 'a += b' passes '(const Mat&)a') throws here if a broadcast would change its shape
    // or type (Mat(1x1) += Mat(1x4)), instead of silently reallocating - exec sees only a Mat and has
    // no view of the array's fixed flags. Cost over a direct exec is negligible: exec keeps its own
    // fast path for a matching-shape op, and dst.create() with an unchanged shape/type is a no-op.
    // With a mask, a freshly (re)allocated dst is zeroed so mask==0 reads 0 (matches arithm_op); a
    // reused dst keeps its prior content there - detect reuse by shape+type BEFORE create (correct for
    // a reused UMat dst too).
    MatShape oshape; પૂર્ણાંક ocn;
    p.outputShape(inputs, oshape, ocn);
    અચલ પૂર્ણાંક otype = CV_MAKETYPE(rdepth, ocn);
    અચલ તાર્કિક reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == આગત_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    જો (haveMask && !reused)
        pdst->setZero();                          // setZero (not setTo, which caps at 4 channels)
    p.exec(inputs, pdst);
}

// Unary element-wise op through the engine (bitwise NOT now; unary math later). One array input, an
// optional write-mask; result depth = dtype (>=0), else a fixed-type dst's depth, else the input
// depth. Mirrors arithm_op's build/mask/exec tail with a single input and no scalar operand. OpenCL is
// run by the caller (bitwise_op_ocl), so this is the CPU engine only.
સ્થિર શૂન્ય unary_op(ew::ટોચ op, આગત_Array src, OutputArray dst, આગત_Array mask,
                     પૂર્ણાંક dtype, અચલ Scalar& params)
{
    જો (src.empty())
    {
        dst.release();
        જો (dtype >= 0)
            dst.create(0, 0, dtype);
        પાછા;
    }

    અચલ તાર્કિક haveMask = !mask.empty();
    // in-place (dst aliases src): keep a header copy so a shape/type-changing dst.create() can't free
    // src's data mid-op (matches arithm_op). A fixed-size/type dst never reallocs, so this is only for
    // an explicit dtype change.
    અચલ શૂન્ય* dstObj = (dst.kind() == આગત_Array_2::MAT) ? dst.getObj() : nullptr;
    અચલ તાર્કિક aliased = dstObj && src.kind() == આગત_Array_2::MAT && dstObj == src.getObj();
    Mat sloc, mloc;
    અચલ Mat* psrc = (src.kind() == આગત_Array_2::MAT && !aliased) ? (અચલ Mat*)src.getObj()
                                                                   : &(sloc = src.getMat());
    અચલ પૂર્ણાંક rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
                     : (dst.fixedType() ? dst.depth() : psrc->depth());

    ew::TExpr p;
    અચલ પૂર્ણાંક a0 = p.add_આગત(psrc->depth());
    પૂર્ણાંક sMask = 0;
    અચલ Mat* pmask = nullptr;
    જો (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = (mask.kind() == આગત_Array_2::MAT) ? (અચલ Mat*)mask.getObj() : &(mloc = mask.getMat());
        sMask = p.add_આગત(pmask->depth());
    }
    અચલ પૂર્ણાંક sOut = p.addOutput(rdepth);
    અચલ પૂર્ણાંક r = p.emitUnary(op, a0, rdepth, params);
    જો (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    નહીંતર
        p.moveToOutput(r, sOut);
    p.compile();

    અચલ Mat* inputs[2]; પૂર્ણાંક ni = 0;
    inputs[ni++] = psrc;
    જો (haveMask) inputs[ni++] = pmask;

    MatShape oshape; પૂર્ણાંક ocn;
    p.outputShape(inputs, oshape, ocn);
    અચલ પૂર્ણાંક otype = CV_MAKETYPE(rdepth, ocn);
    અચલ તાર્કિક reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == આગત_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    જો (haveMask && !reused)
        pdst->setZero();
    p.exec(inputs, pdst);
}

}

શૂન્ય cv::add( આગત_Array src1, આગત_Array src2, OutputArray dst,
          આગત_Array mask, પૂર્ણાંક dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADD, src1, src2, dst, mask, dtype, OCL_OP_ADD, ખોટું);
}

શૂન્ય cv::subtract( આગત_Array src1, આગત_Array src2, OutputArray dst,
                   આગત_Array mask, પૂર્ણાંક dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_SUB, src1, src2, dst, mask, dtype, OCL_OP_SUB, ખોટું);
}

શૂન્ય cv::absdiff( આગત_Array src1, આગત_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ABSDIFF, src1, src2, dst, noArray(), -1, OCL_OP_ABSDIFF, ખોટું);
}

શૂન્ય cv::copyTo(આગત_Array _src, OutputArray _dst, આગત_Array _mask)
{
    CV_INSTRUMENT_REGION();

    _src.copyTo(_dst, _mask);
}

/****************************************************************************************\
*                                    multiply/divide                                     *
\****************************************************************************************/

નામાકાશ cv
{

શૂન્ય multiply(આગત_Array src1, આગત_Array src2,
              OutputArray dst, દ્વિગુણ scale, પૂર્ણાંક dtype)
{
    CV_INSTRUMENT_REGION();

    અચલ પૂર્ણાંક oclop = std::abs(scale - 1.0) < DBL_EPSILON ? OCL_OP_MUL : OCL_OP_MUL_SCALE;
    arithm_op(ew::OP_MUL, src1, src2, dst, noArray(), dtype, oclop, /*muldiv*/ સાચું, Scalar(scale));
}

શૂન્ય divide(આગત_Array src1, આગત_Array src2,
                OutputArray dst, દ્વિગુણ scale, પૂર્ણાંક dtype)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_DIV, src1, src2, dst, noArray(), dtype, OCL_OP_DIV_SCALE, /*muldiv*/ સાચું, Scalar(scale));
}

શૂન્ય divide(દ્વિગુણ scale, આગત_Array src2,
                OutputArray dst, પૂર્ણાંક dtype)
{
    CV_INSTRUMENT_REGION();

    જો (src2.empty())
    {
        dst.release();
        પાછા;
    }

    // scale / src2 == scale * 1 / src2: feed a 0-dim `1` as the numerator so the CPU engine reuses the
    // normal divide (scale*a/b). Give the numerator src2's OWN depth: then OP_DIV(T, T) is same-type and
    // takes the fast per-type kernel (f32 work for <=16-bit / f16 / bf16, not the s32->f64 path a CV_32S
    // numerator would force). An integer src2 keeps its div-by-zero->0 guard (both operands integer); a
    // float src2 divides as float (1/0->inf), both matching cv::divide. dtype<0 follows src2's depth (a
    // fixed-type dst still wins). The UMat path uses the dedicated reciprocal kernel (OCL_OP_RECIP_SCALE).
    દ્વિગુણ one = 1;
    Mat numerator(MatShape::scalar(), src2.depth(), &one);
    scalarToRawData(Scalar(1), &one, src2.depth(), 1);       // write `1` in src2's depth (no allocation)
    અચલ પૂર્ણાંક rtype = (dtype < 0 && !dst.fixedType()) ? src2.depth() : dtype;
    arithm_op(ew::OP_DIV, numerator, src2, dst, noArray(), rtype, OCL_OP_RECIP_SCALE, /*muldiv*/ સાચું, Scalar(scale));
}

UMat UMat::mul(આગત_Array m, દ્વિગુણ scale) અચલ
{
    UMat dst;
    multiply(*આ, m, dst, scale);
    પાછા dst;
}

/****************************************************************************************\
*                                      addWeighted                                       *
\****************************************************************************************/

}

શૂન્ય cv::addWeighted( આગત_Array src1, દ્વિગુણ alpha, આગત_Array src2,
                      દ્વિગુણ beta, દ્વિગુણ gamma, OutputArray dst, પૂર્ણાંક dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADDW, src1, src2, dst, noArray(), dtype, OCL_OP_ADDW, /*muldiv*/ સાચું,
              Scalar(alpha, beta, gamma));
}


/****************************************************************************************\
*                                          compare                                       *
\****************************************************************************************/

નામાકાશ cv
{


સ્થિર દ્વિગુણ getMinVal(પૂર્ણાંક depth)
{
    સ્થિર અચલ દ્વિગુણ tab[CV_DEPTH_MAX] =
    {
        0, -128, 0, -32768, INT_MIN, -FLT_MAX, -DBL_MAX,
        -65504, -FLT_MAX, 0, 0, (દ્વિગુણ)INT64_MIN, 0
    };
    પાછા tab[depth];
}

સ્થિર દ્વિગુણ getMaxVal(પૂર્ણાંક depth)
{
    સ્થિર અચલ દ્વિગુણ tab[CV_DEPTH_MAX] = {
        255, 127, 65535, 32767, INT_MAX, FLT_MAX, DBL_MAX,
        65504, FLT_MAX, 255, (દ્વિગુણ)UINT64_MAX, (દ્વિગુણ)INT64_MAX, (દ્વિગુણ)UINT32_MAX, 0
    };
    પાછા tab[depth];
}

#ifdef HAVE_OPENCL

સ્થિર તાર્કિક ocl_compare(આગત_Array _src1, આગત_Array _src2, OutputArray _dst, પૂર્ણાંક op, તાર્કિક haveScalar)
{
    અચલ ocl::Device& dev = ocl::Device::getDefault();
    તાર્કિક doubleSupport = dev.doubleFPConfig() > 0;
    પૂર્ણાંક type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1),
            type2 = _src2.type(), depth2 = CV_MAT_DEPTH(type2);

    જો (!doubleSupport && depth1 == CV_64F)
        પાછા ખોટું;

    // the integer scalar path below works in int; let the CPU handle the wider 5.x integer depths
    જો (haveScalar && (depth1 == CV_32U || depth1 == CV_64U || depth1 == CV_64S))
        પાછા ખોટું;

    જો (!haveScalar && (!_src1.sameSize(_src2) || type1 != type2))
            પાછા ખોટું;

    // This OpenCL kernel broadcasts a scalar's FIRST channel to all channels. The CPU path compares
    // per-channel (scalar[c] for channel c), so a multichannel array against a scalar with DISTINCT
    // channel values would disagree - bail to the CPU engine in that case (rare; not worth a kernel fix).
    જો (haveScalar && cn > 1)
    {
        Mat sc = _src2.getMat();
        પૂર્ણાંક scn = (પૂર્ણાંક)sc.total();
        જો (scn > 1)
        {
            દ્વિગુણ v[4] = { 0, 0, 0, 0 };
            getConvertFunc(sc.depth(), CV_64F)(sc.ptr(), 1, 0, 1, (uchar*)v, 1, Size(std::min(scn, 4), 1), 0);
            માટે (પૂર્ણાંક i = 1, n = std::min(cn, scn); i < n; i++)
                જો (v[i] != v[0])
                    પાછા ખોટું;
        }
    }

    પૂર્ણાંક kercn = haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst), rowsPerWI = dev.isIntel() ? 4 : 1;
    // Workaround for bug with "?:" operator in AMD OpenCL compiler
    જો (depth1 >= CV_16U)
        kercn = 1;

    પૂર્ણાંક scalarcn = kercn == 3 ? 4 : kercn;
    અચલ અક્ષર * અચલ operationMap[] = { "==", ">", ">=", "<", "<=", "!=" };
    અક્ષર cvt[50];

    String opts = format("-D %s -D srcT1=%s -D dstT=%s -D DEPTH_dst=%d -D workT=srcT1 -D cn=%d"
                         " -D convertToDT=%s -D OP_CMP -D CMP_OPERATOR=%s -D srcT1_C1=%s"
                         " -D srcT2_C1=%s -D dstT_C1=%s -D workST=%s -D rowsPerWI=%d%s",
                         haveScalar ? "UNARY_OP" : "BINARY_OP",
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, kercn)),
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U, kercn,
                         ocl::convertTypeStr(depth1, CV_8U, kercn, cvt, કદ(cvt)),
                         operationMap[op], ocl::typeToStr(depth1),
                         ocl::typeToStr(depth1), ocl::typeToStr(CV_8U),
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, scalarcn)), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    જો (k.empty())
        પાછા ખોટું;

    UMat src1 = _src1.getUMat();
    Size size = src1.size();
    _dst.create(size, CV_8UC(cn));
    UMat dst = _dst.getUMat();

    જો (haveScalar)
    {
        size_t esz = CV_ELEM_SIZE1(type1) * scalarcn;
        દ્વિગુણ buf[4] = { 0, 0, 0, 0 };
        Mat src2 = _src2.getMat();

        જો( depth1 > CV_32S )
            convertAndUnrollScalar( src2, depth1, (uchar *)buf, kercn );
        નહીંતર
        {
            દ્વિગુણ fval = 0;
            getConvertFunc(depth2, CV_64F)(src2.ptr(), 1, 0, 1, (uchar *)&fval, 1, Size(1, 1), 0);
            જો( fval < getMinVal(depth1) )
                પાછા dst.setTo(Scalar::all(op == CMP_GT || op == CMP_GE || op == CMP_NE ? 255 : 0)), સાચું;

            જો( fval > getMaxVal(depth1) )
                પાછા dst.setTo(Scalar::all(op == CMP_LT || op == CMP_LE || op == CMP_NE ? 255 : 0)), સાચું;

            પૂર્ણાંક ival = cvRound(fval);
            જો( fval != ival )
            {
                જો( op == CMP_LT || op == CMP_GE )
                    ival = cvCeil(fval);
                નહીંતર જો( op == CMP_LE || op == CMP_GT )
                    ival = cvFloor(fval);
                નહીંતર
                    પાછા dst.setTo(Scalar::all(op == CMP_NE ? 255 : 0)), સાચું;
            }
            convertAndUnrollScalar(Mat(1, 1, CV_32S, &ival), depth1, (uchar *)buf, kercn);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        k.args(ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn),
               ocl::KernelArg::WriteOnly(dst, cn, kercn), scalararg);
    }
    નહીંતર
    {
        UMat src2 = _src2.getUMat();

        k.args(ocl::KernelArg::ReadOnlyNoSize(src1),
               ocl::KernelArg::ReadOnlyNoSize(src2),
               ocl::KernelArg::WriteOnly(dst, cn, kercn));
    }

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    પાછા k.run(2, globalsize, ખાલી, ખોટું);
}

#endif

}

શૂન્ય cv::compare(આગત_Array _src1, આગત_Array _src2, OutputArray _dst, પૂર્ણાંક op)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( op == CMP_LT || op == CMP_LE || op == CMP_EQ ||
               op == CMP_NE || op == CMP_GE || op == CMP_GT );

    CV_Assert(_src1.empty() == _src2.empty());
    જો (_src1.empty() && _src2.empty())
    {
        _dst.release();
        પાછા;
    }

    તાર્કિક haveScalar = ખોટું;

    જો ((_src1.isMatx() + _src2.isMatx()) == 1
            || !_src1.sameSize(_src2)
            || _src1.type() != _src2.type())
    {
        તાર્કિક is_src1_scalar = checkScalar(_src1, _src2.type(), _src1.kind(), _src2.kind());
        તાર્કિક is_src2_scalar = checkScalar(_src2, _src1.type(), _src2.kind(), _src1.kind());

        // exactly one scalar operand: keep it as src2 (swapping flips the ordering ops) so the OpenCL
        // path sees the array-op-scalar form. Otherwise - two arrays of different size (broadcast),
        // mixed types, or two scalars - the CPU engine broadcasts / promotes them, so there is NO error
        // here: fall through to arithm_op (which handles scalar detection, broadcast and mixed types).
        જો (is_src1_scalar && !is_src2_scalar)
        {
            op = op == CMP_LT ? CMP_GT : op == CMP_LE ? CMP_GE :
                op == CMP_GE ? CMP_LE : op == CMP_GT ? CMP_LT : op;
            // src1 is a scalar; swap it with src2
            compare(_src2, _src1, _dst, op);
            પાછા;
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
    ew::ટોચ cmpOp = op == CMP_EQ ? ew::OP_CMP_EQ : op == CMP_NE ? ew::OP_CMP_NE :
                    op == CMP_LT ? ew::OP_CMP_LT : op == CMP_LE ? ew::OP_CMP_LE :
                    op == CMP_GT ? ew::OP_CMP_GT : ew::OP_CMP_GE;
    arithm_op(cmpOp, _src1, _src2, _dst, noArray(), CV_8U, /*oclop=*/-1, /*muldiv=*/ખોટું);
}

/****************************************************************************************\
*                                        inRange                                         *
\****************************************************************************************/

નામાકાશ cv
{

template <typename T>
સંરચના InRange_SIMD
{
    પૂર્ણાંક operator () (અચલ T * src1, અચલ T * src2, અચલ T * src3, uchar * dst, પૂર્ણાંક len) અચલ
    {
        માટે (પૂર્ણાંક x = 0; x < len; x++)
            dst[x] = (src2[x] <= src1[x] && src1[x] <= src3[x]) ? 255 : 0;
        પાછા len;
    }
};

#જો (CV_SIMD || CV_SIMD_SCALABLE)

template <>
સંરચના InRange_SIMD<uchar>
{
    પૂર્ણાંક operator () (અચલ uchar * src1, અચલ uchar * src2, અચલ uchar * src3,
        uchar * dst, પૂર્ણાંક len) અચલ
    {
        પૂર્ણાંક x = 0;
        અચલ પૂર્ણાંક width = VTraits<v_uint8>::vlanes();

        માટે (; x <= len - width; x += width)
        {
            v_uint8 values = vx_load(src1 + x);
            v_uint8 નીચલું = vx_load(src2 + x);
            v_uint8 ઉપલું = vx_load(src3 + x);

            v_store(dst + x, v_and(v_ge(values, નીચલું), v_ge(ઉપલું, values)));
        }
        vx_cleanup();
        પાછા x;
    }
};

template <>
સંરચના InRange_SIMD<schar>
{
    પૂર્ણાંક operator () (અચલ schar * src1, અચલ schar * src2, અચલ schar * src3,
        uchar * dst, પૂર્ણાંક len) અચલ
    {
        પૂર્ણાંક x = 0;
        અચલ પૂર્ણાંક width = VTraits<v_int8>::vlanes();

        માટે (; x <= len - width; x += width)
        {
            v_int8 values = vx_load(src1 + x);
            v_int8 નીચલું = vx_load(src2 + x);
            v_int8 ઉપલું = vx_load(src3 + x);

            v_store((schar*)(dst + x), v_and(v_ge(values, નીચલું), v_ge(ઉપલું, values)));
        }
        vx_cleanup();
        પાછા x;
    }
};

template <>
સંરચના InRange_SIMD<ushort>
{
    પૂર્ણાંક operator () (અચલ ushort * src1, અચલ ushort * src2, અચલ ushort * src3,
        uchar * dst, પૂર્ણાંક len) અચલ
    {
        પૂર્ણાંક x = 0;
        અચલ પૂર્ણાંક width = VTraits<v_uint16>::vlanes() * 2;

        માટે (; x <= len - width; x += width)
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
        પાછા x;
    }
};

template <>
સંરચના InRange_SIMD<short>
{
    પૂર્ણાંક operator () (અચલ short * src1, અચલ short * src2, અચલ short * src3,
        uchar * dst, પૂર્ણાંક len) અચલ
    {
        પૂર્ણાંક x = 0;
        અચલ પૂર્ણાંક width = (પૂર્ણાંક)VTraits<v_int16>::vlanes() * 2;

        માટે (; x <= len - width; x += width)
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
        પાછા x;
    }
};

template <>
સંરચના InRange_SIMD<પૂર્ણાંક>
{
    પૂર્ણાંક operator () (અચલ પૂર્ણાંક * src1, અચલ પૂર્ણાંક * src2, અચલ પૂર્ણાંક * src3,
        uchar * dst, પૂર્ણાંક len) અચલ
    {
        પૂર્ણાંક x = 0;
        અચલ પૂર્ણાંક width = (પૂર્ણાંક)VTraits<v_int32>::vlanes() * 2;

        માટે (; x <= len - width; x += width)
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
        પાછા x;
    }
};

template <>
સંરચના InRange_SIMD<દશાંશ>
{
    પૂર્ણાંક operator () (અચલ દશાંશ * src1, અચલ દશાંશ * src2, અચલ દશાંશ * src3,
        uchar * dst, પૂર્ણાંક len) અચલ
    {
        પૂર્ણાંક x = 0;
        અચલ પૂર્ણાંક width = (પૂર્ણાંક)VTraits<v_float32>::vlanes() * 2;

        માટે (; x <= len - width; x += width)
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
        પાછા x;
    }
};

template <>
સંરચના InRange_SIMD<hfloat>
{
    પૂર્ણાંક operator () (અચલ hfloat * src1, અચલ hfloat * src2, અચલ hfloat * src3,
        uchar * dst, પૂર્ણાંક len) અચલ
    {
        પૂર્ણાંક x = 0;
        અચલ પૂર્ણાંક width = (પૂર્ણાંક)VTraits<v_float32>::vlanes()*2;

        માટે (; x <= len - width; x += width)
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
        પાછા x;
    }
};

template <>
સંરચના InRange_SIMD<bfloat>
{
    પૂર્ણાંક operator () (અચલ bfloat * src1, અચલ bfloat * src2, અચલ bfloat * src3,
        uchar * dst, પૂર્ણાંક len) અચલ
    {
        પૂર્ણાંક x = 0;
        અચલ પૂર્ણાંક width = (પૂર્ણાંક)VTraits<v_float32>::vlanes()*2;

        માટે (; x <= len - width; x += width)
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
        પાછા x;
    }
};

template <>
સંરચના InRange_SIMD<unsigned>
{
    પૂર્ણાંક operator () (અચલ unsigned * src1, અચલ unsigned * src2, અચલ unsigned * src3,
        uchar * dst, પૂર્ણાંક len) અચલ
    {
        પૂર્ણાંક x = 0;
        અચલ પૂર્ણાંક width = (પૂર્ણાંક)VTraits<v_uint32>::vlanes() * 2;

        માટે (; x <= len - width; x += width)
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
        પાછા x;
    }
};

#જો CV_SIMD_64F

template <>
સંરચના InRange_SIMD<દ્વિગુણ>
{
    પૂર્ણાંક operator () (અચલ દ્વિગુણ * src1, અચલ દ્વિગુણ * src2, અચલ દ્વિગુણ * src3,
        uchar * dst, પૂર્ણાંક len) અચલ
    {
        પૂર્ણાંક x = 0;
        અચલ પૂર્ણાંક step = VTraits<v_float64>::vlanes();
        અચલ પૂર્ણાંક width = step * 4;

        માટે (; x <= len - width; x += width)
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
        પાછા x;
    }
};

#endif

#endif

template <typename T>
સ્થિર શૂન્ય inRange_(અચલ શૂન્ય* _src1, size_t step1, અચલ શૂન્ય* _src2, size_t step2,
         અચલ શૂન્ય* _src3, size_t step3, uchar* dst, size_t step,
         Size size)
{
    અચલ T* src1 = (અચલ T*)_src1;
    અચલ T* src2 = (અચલ T*)_src2;
    અચલ T* src3 = (અચલ T*)_src3;
    step1 /= કદ(src1[0]);
    step2 /= કદ(src2[0]);
    step3 /= કદ(src3[0]);

    InRange_SIMD<T> vop;

    માટે( ; size.height--; src1 += step1, src2 += step2, src3 += step3, dst += step )
    {
        પૂર્ણાંક x = vop(src1, src2, src3, dst, size.width);
        #જો CV_ENABLE_UNROLLED
        માટે( ; x <= size.width - 4; x += 4 )
        {
            પૂર્ણાંક t0, t1;
            t0 = src2[x] <= src1[x] && src1[x] <= src3[x];
            t1 = src2[x+1] <= src1[x+1] && src1[x+1] <= src3[x+1];
            dst[x] = (uchar)-t0; dst[x+1] = (uchar)-t1;
            t0 = src2[x+2] <= src1[x+2] && src1[x+2] <= src3[x+2];
            t1 = src2[x+3] <= src1[x+3] && src1[x+3] <= src3[x+3];
            dst[x+2] = (uchar)-t0; dst[x+3] = (uchar)-t1;
        }
        #endif
        માટે( ; x < size.width; x++ )
            dst[x] = (uchar)-(src2[x] <= src1[x] && src1[x] <= src3[x]);
    }
}

સ્થિર શૂન્ય inRangeReduce(અચલ uchar* src, uchar* dst, size_t len, પૂર્ણાંક cn)
{
    પૂર્ણાંક k = cn % 4 ? cn % 4 : 4;
    size_t i, j;
    જો( k == 1 )
        માટે( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j];
    નહીંતર જો( k == 2 )
        માટે( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1];
    નહીંતર જો( k == 3 )
        માટે( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2];
    નહીંતર
        માટે( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2] & src[j+3];

    માટે( ; k < cn; k += 4 )
    {
        માટે( i = 0, j = k; i < len; i++, j += cn )
            dst[i] &= src[j] & src[j+1] & src[j+2] & src[j+3];
    }
}

પ્રકારવ્યાખ્યા શૂન્ય (*InRangeFunc)( અચલ શૂન્ય* src1, size_t step1, અચલ શૂન્ય* src2, size_t step2,
                             અચલ શૂન્ય* src3, size_t step3, uchar* dst, size_t step, Size sz );

સ્થિર InRangeFunc getInRangeFunc(પૂર્ણાંક depth)
{
    સ્થિર InRangeFunc inRangeTab[CV_DEPTH_MAX] =
    {
        inRange_<uchar>,
        inRange_<schar>,
        inRange_<ushort>,
        inRange_<short>,
        inRange_<પૂર્ણાંક>,
        inRange_<દશાંશ>,
        inRange_<દ્વિગુણ>,
        inRange_<hfloat>,
        inRange_<bfloat>,
        0,
        inRange_<uint64_t>,
        inRange_<int64_t>,
        inRange_<unsigned>,
        0,
    };

    પાછા inRangeTab[depth];
}

#ifdef HAVE_OPENCL

સ્થિર તાર્કિક ocl_inRange( આગત_Array _src, આગત_Array _lowerb,
                         આગત_Array _upperb, OutputArray _dst )
{
    અચલ ocl::Device & d = ocl::Device::getDefault();
    આગત_Array_2::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Size ssize = _src.size(), lsize = _lowerb.size(), usize = _upperb.size();
    પૂર્ણાંક stype = _src.type(), ltype = _lowerb.type(), utype = _upperb.type();
    પૂર્ણાંક sdepth = CV_MAT_DEPTH(stype), ldepth = CV_MAT_DEPTH(ltype), udepth = CV_MAT_DEPTH(utype);
    પૂર્ણાંક cn = CV_MAT_CN(stype), rowsPerWI = d.isIntel() ? 4 : 1;
    તાર્કિક lbScalar = ખોટું, ubScalar = ખોટું;

    જો( (lkind == આગત_Array_2::MATX && skind != આગત_Array_2::MATX) ||
        ssize != lsize || stype != ltype )
    {
        જો( !checkScalar(_lowerb, stype, lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = સાચું;
    }

    જો( (ukind == આગત_Array_2::MATX && skind != આગત_Array_2::MATX) ||
        ssize != usize || stype != utype )
    {
        જો( !checkScalar(_upperb, stype, ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = સાચું;
    }

    જો (lbScalar != ubScalar)
        પાછા ખોટું;

    તાર્કિક doubleSupport = d.doubleFPConfig() > 0,
            haveScalar = lbScalar && ubScalar;

    જો ( (!doubleSupport && sdepth == CV_64F) ||
         (!haveScalar && (sdepth != ldepth || sdepth != udepth)) )
        પાછા ખોટું;

    પૂર્ણાંક kercn = haveScalar ? cn : std::max(std::min(ocl::predictOptimalVectorWidth(_src, _lowerb, _upperb, _dst), 4), cn);
    જો (kercn % cn != 0)
        kercn = cn;
    પૂર્ણાંક colsPerWI = kercn / cn;
    String opts = format("%s-D CN=%d -D SRC_T=%s -D SRC_T1=%s -D DST_T=%s -D KERCN=%d -D DEPTH=%d%s -D COLS_PER_WI=%d",
                           haveScalar ? "-D HAVE_SCALAR " : "", cn, ocl::typeToStr(CV_MAKE_TYPE(sdepth, kercn)),
                           ocl::typeToStr(sdepth), ocl::typeToStr(CV_8UC(colsPerWI)), kercn, sdepth,
                           doubleSupport ? " -D DOUBLE_SUPPORT" : "", colsPerWI);

    ocl::Kernel ker("inrange", ocl::core::inrange_oclsrc, opts);
    જો (ker.empty())
        પાછા ખોટું;

    _dst.create(ssize, CV_8UC1);
    UMat src = _src.getUMat(), dst = _dst.getUMat(), lscalaru, uscalaru;
    Mat lscalar, uscalar;

    જો (lbScalar && ubScalar)
    {
        lscalar = _lowerb.getMat();
        uscalar = _upperb.getMat();

        size_t esz = src.elemSize();
        size_t blocksize = 36;

        AutoBuffer<uchar> _buf(blocksize*(((પૂર્ણાંક)lbScalar + (પૂર્ણાંક)ubScalar)*esz + cn) + 2*cn*કદ(પૂર્ણાંક) + 128);
        uchar *buf = alignPtr(_buf.data() + blocksize*cn, 16);

        જો( ldepth != sdepth && sdepth < CV_32S )
        {
            પૂર્ણાંક* ilbuf = (પૂર્ણાંક*)alignPtr(buf + blocksize*esz, 16);
            પૂર્ણાંક* iubuf = ilbuf + cn;

            દ્વિઅંકી_Func sccvtfunc = getConvertFunc(ldepth, CV_32S);
            sccvtfunc(lscalar.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, Size(cn, 1), 0);
            sccvtfunc(uscalar.ptr(), 1, 0, 1, (uchar*)iubuf, 1, Size(cn, 1), 0);
            પૂર્ણાંક minval = cvRound(getMinVal(sdepth)), maxval = cvRound(getMaxVal(sdepth));

            માટે( પૂર્ણાંક k = 0; k < cn; k++ )
            {
                જો( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lscalar = Mat(cn, 1, CV_32S, ilbuf);
            uscalar = Mat(cn, 1, CV_32S, iubuf);
        }

        lscalar.convertTo(lscalar, stype);
        uscalar.convertTo(uscalar, stype);
    }
    નહીંતર
    {
        lscalaru = _lowerb.getUMat();
        uscalaru = _upperb.getUMat();
    }

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, 1, colsPerWI);

    જો (haveScalar)
    {
        lscalar.copyTo(lscalaru);
        uscalar.copyTo(uscalaru);

        ker.args(srcarg, dstarg, ocl::KernelArg::PtrReadOnly(lscalaru),
               ocl::KernelArg::PtrReadOnly(uscalaru), rowsPerWI);
    }
    નહીંતર
        ker.args(srcarg, dstarg, ocl::KernelArg::ReadOnlyNoSize(lscalaru),
               ocl::KernelArg::ReadOnlyNoSize(uscalaru), rowsPerWI);

    size_t globalsize[2] = { (size_t)ssize.width / colsPerWI, ((size_t)ssize.height + rowsPerWI - 1) / rowsPerWI };
    પાછા ker.run(2, globalsize, ખાલી, ખોટું);
}

#endif

}

શૂન્ય cv::inRange(આગત_Array _src, આગત_Array _lowerb,
                 આગત_Array _upperb, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(! _src.empty());
    જો (!getInRangeFunc(_src.depth()))
        CV_Error( cv::Error::StsNotImplemented,
                 "inRange: CV_Bool and CV_8F inputs are not supported, convert the input to another type first");

    CV_OCL_RUN(_src.dims() <= 2 && _lowerb.dims() <= 2 &&
               _upperb.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat()),
               ocl_inRange(_src, _lowerb, _upperb, _dst))

    આગત_Array_2::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Mat src = _src.getMat(), lb = _lowerb.getMat(), ub = _upperb.getMat();

    તાર્કિક lbScalar = ખોટું, ubScalar = ખોટું;

    જો( (lkind == આગત_Array_2::MATX && skind != આગત_Array_2::MATX) ||
        src.size != lb.size || src.type() != lb.type() )
    {
        જો( !checkScalar(lb, src.type(), lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = સાચું;
    }

    જો( (ukind == આગત_Array_2::MATX && skind != આગત_Array_2::MATX) ||
        src.size != ub.size || src.type() != ub.type() )
    {
        જો( !checkScalar(ub, src.type(), ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = સાચું;
    }

    CV_Assert(lbScalar == ubScalar);

    પૂર્ણાંક cn = src.channels(), depth = src.depth();

    size_t esz = src.elemSize();
    size_t blocksize0 = (size_t)(BLOCK_SIZE + esz-1)/esz;

    _dst.createSameSize(_src, CV_8UC1);
    Mat dst = _dst.getMat();
    InRangeFunc func = getInRangeFunc(depth);

    અચલ Mat* arrays_sc[] = { &src, &dst, 0 };
    અચલ Mat* arrays_nosc[] = { &src, &dst, &lb, &ub, 0 };
    uchar* ptrs[4] = {};

    NAryMatIterator it(lbScalar && ubScalar ? arrays_sc : arrays_nosc, ptrs);
    size_t total = it.size, blocksize = std::min(total, blocksize0);

    AutoBuffer<uchar> _buf(blocksize*(((પૂર્ણાંક)lbScalar + (પૂર્ણાંક)ubScalar)*esz + cn) + 2*cn*કદ(પૂર્ણાંક) + 128);
    uchar *buf = _buf.data(), *mbuf = buf, *lbuf = 0, *ubuf = 0;
    buf = alignPtr(buf + blocksize*cn, 16);

    જો( lbScalar && ubScalar )
    {
        lbuf = buf;
        ubuf = buf = alignPtr(buf + blocksize*esz, 16);

        CV_Assert( lb.type() == ub.type() );
        પૂર્ણાંક scdepth = lb.depth();

        જો( scdepth != depth && depth < CV_32S )
        {
            પૂર્ણાંક* ilbuf = (પૂર્ણાંક*)alignPtr(buf + blocksize*esz, 16);
            પૂર્ણાંક* iubuf = ilbuf + cn;

            દ્વિઅંકી_Func sccvtfunc = getConvertFunc(scdepth, CV_32S);
            sccvtfunc(lb.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, Size(cn, 1), 0);
            sccvtfunc(ub.ptr(), 1, 0, 1, (uchar*)iubuf, 1, Size(cn, 1), 0);
            પૂર્ણાંક minval = cvRound(getMinVal(depth)), maxval = cvRound(getMaxVal(depth));

            માટે( પૂર્ણાંક k = 0; k < cn; k++ )
            {
                જો( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lb = Mat(cn, 1, CV_32S, ilbuf);
            ub = Mat(cn, 1, CV_32S, iubuf);
        }

        convertAndUnrollScalar( lb, src.type(), lbuf, blocksize );
        convertAndUnrollScalar( ub, src.type(), ubuf, blocksize );

        જો (depth == CV_8U && src.dims <= 2) {
            uint8_t lb_scalar = lbuf[0];
            uint8_t ub_scalar = ubuf[0];
            CALL_HAL(inRange_u8, cv_hal_inRange8u, src.data, src.step, dst.data, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        } નહીંતર જો (depth == CV_32F && src.dims <= 2) {
            દ્વિગુણ lb_scalar = lb.ptr<દ્વિગુણ>(0)[0];
            દ્વિગુણ ub_scalar = ub.ptr<દ્વિગુણ>(0)[0];
            CALL_HAL(inRange_f32, cv_hal_inRange32f, src.data, src.step, dst.data, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        }
    }

    માટે( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        માટે( size_t j = 0; j < total; j += blocksize )
        {
            પૂર્ણાંક bsz = (પૂર્ણાંક)MIN(total - j, blocksize);
            size_t delta = bsz*esz;
            uchar *lptr = lbuf, *uptr = ubuf;
            જો( !lbScalar )
            {
                lptr = ptrs[2];
                ptrs[2] += delta;
            }
            જો( !ubScalar )
            {
                પૂર્ણાંક idx = !lbScalar ? 3 : 2;
                uptr = ptrs[idx];
                ptrs[idx] += delta;
            }
            func( ptrs[0], 0, lptr, 0, uptr, 0, cn == 1 ? ptrs[1] : mbuf, 0, Size(bsz*cn, 1));
            જો( cn > 1 )
                inRangeReduce(mbuf, ptrs[1], bsz, cn);
            ptrs[0] += delta;
            ptrs[1] += bsz;
        }
    }
}
/* End of file. */
