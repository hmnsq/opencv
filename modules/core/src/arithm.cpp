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

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "arithm_expr.hpp"   // the new element-wise engine (cv::ew)
#अन्तर्भाव "opencl_kernels_core.hpp"

नामाकाश cv
{

/****************************************************************************************\
*                                   logical operations                                   *
\****************************************************************************************/

गणना { OCL_OP_ADD=0, OCL_OP_SUB=1, OCL_OP_RSUB=2, OCL_OP_ABSDIFF=3, OCL_OP_MUL=4,
       OCL_OP_MUL_SCALE=5, OCL_OP_DIV_SCALE=6, OCL_OP_RECIP_SCALE=7, OCL_OP_ADDW=8,
       OCL_OP_AND=9, OCL_OP_OR=10, OCL_OP_XOR=11, OCL_OP_NOT=12, OCL_OP_MIN=13, OCL_OP_MAX=14,
       OCL_OP_RDIV_SCALE=15 };

// The unified entry point for every element-wise binary op (empty + UMat/OpenCL + CPU engine). Defined
// lower down (near cv::add); forward-declared here so cv::min/cv::max (above it) can use it too.
स्थिर शून्य arithm_op(ew::शीर्ष op, निवेश_Array src1, निवेश_Array src2, OutputArray dst,
                      निवेश_Array mask, पूर्णाङ्क dtype, पूर्णाङ्क oclop, तार्किक muldiv, ध्रुव Scalar& params = Scalar(1));

// The unary counterpart (bitwise NOT, unary math later). Forward-declared for the same reason.
स्थिर शून्य unary_op(ew::शीर्ष op, निवेश_Array src, OutputArray dst, निवेश_Array mask,
                     पूर्णाङ्क dtype, ध्रुव Scalar& params = Scalar());

#ifdef HAVE_OPENCL

स्थिर ध्रुव वर्ण* oclop2str[] = { "OP_ADD", "OP_SUB", "OP_RSUB", "OP_ABSDIFF",
    "OP_MUL", "OP_MUL_SCALE", "OP_DIV_SCALE", "OP_RECIP_SCALE",
    "OP_ADDW", "OP_AND", "OP_OR", "OP_XOR", "OP_NOT", "OP_MIN", "OP_MAX", "OP_RDIV_SCALE", 0 };

// depths with no OpenCL type name (ocl::typeToStr() asserts on them)
स्थिर अन्तःस्थ तार्किक noOclType(पूर्णाङ्क depth)
{
    प्रत्यागम depth == CV_16BF || depth == CV_Bool || depth == CV_8F_E4M3FN || depth == CV_8F_E4M3FNUZ;
}

स्थिर तार्किक ocl_द्विआधारी_op(निवेश_Array _src1, निवेश_Array _src2, OutputArray _dst,
                          निवेश_Array _mask, तार्किक bitwise, पूर्णाङ्क oclop, तार्किक haveScalar )
{
    तार्किक haveMask = !_mask.empty();
    पूर्णाङ्क srctype = _src1.type();
    पूर्णाङ्क srcdepth = CV_MAT_DEPTH(srctype);
    पूर्णाङ्क cn = CV_MAT_CN(srctype);

    ध्रुव ocl::Device d = ocl::Device::getDefault();
    तार्किक doubleSupport = d.doubleFPConfig() > 0;
    यदि( oclop < 0 || ((haveMask || haveScalar) && cn > 4) ||
            (!doubleSupport && srcdepth == CV_64F && !bitwise) ||
            (noOclType(srcdepth) && !bitwise))
        प्रत्यागम असत्य;

    वर्ण opts[1024];
    पूर्णाङ्क kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    पूर्णाङ्क scalarcn = kercn == 3 ? 4 : kercn;
    पूर्णाङ्क rowsPerWI = d.isIntel() ? 4 : 1;

    ध्रुव पूर्णाङ्क dstDepth = srcdepth;
    ध्रुव पूर्णाङ्क dstType = CV_MAKETYPE(dstDepth, kercn);
    ध्रुव पूर्णाङ्क dstType1 = CV_MAKETYPE(dstDepth, 1);
    ध्रुव पूर्णाङ्क scalarType = CV_MAKETYPE(srcdepth, scalarcn);

    snprintf(opts, परिमाण(opts), "-D %s%s -D %s%s -D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workST=%s -D cn=%d -D rowsPerWI=%d",
            haveMask ? "MASK_" : "", haveScalar ? "UNARY_OP" : "BINARY_OP", oclop2str[oclop],
            doubleSupport ? " -D DOUBLE_SUPPORT" : "",
            bitwise ? ocl::memopTypeToStr(dstType) : ocl::typeToStr(dstType),
            dstDepth,
            bitwise ? ocl::memopTypeToStr(dstType1) : ocl::typeToStr(dstType1),
            bitwise ? ocl::memopTypeToStr(scalarType) : ocl::typeToStr(scalarType),
            kercn, rowsPerWI);

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask, 1);

    यदि( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(srctype)*scalarcn;
        द्विगुण buf[4] = {0,0,0,0};

        यदि( oclop != OCL_OP_NOT )
        {
            Mat src2sc = _src2.getMat();
            convertAndUnrollScalar(src2sc, srctype, (uchar*)buf, 1);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        यदि( !haveMask )
            k.args(src1arg, dstarg, scalararg);
        अन्यथा
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    अन्यथा
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::ReadOnlyNoSize(src2, cn, kercn);

        यदि( !haveMask )
            k.args(src1arg, src2arg, dstarg);
        अन्यथा
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    प्रत्यागम k.run(2, globalsize, 0, असत्य);
}

#endif

// OpenCL path for the bitwise ops (and/or/xor/not), lifted out of binary_op: choose the (possibly
// scalar) operand order, size the destination, and dispatch the shared bitwise OpenCL kernel. Returns
// true if OpenCL handled the op, false to fall through to the CPU engine. NOT arrives unary
// (src2 == src1, forced-scalar). The caller gates this on a UMat operand + dims <= 2 (via CV_OCL_RUN).
#ifdef HAVE_OPENCL
स्थिर तार्किक bitwise_op_ocl(निवेश_Array _src1, निवेश_Array _src2, OutputArray _dst,
                           निवेश_Array _mask, पूर्णाङ्क oclop)
{
    ध्रुव निवेश_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    निवेश_Array_2::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    पूर्णाङ्क type1 = psrc1->type(), type2 = psrc2->type();
    तार्किक haveScalar = असत्य;

    यदि( oclop == OCL_OP_NOT )
        haveScalar = सत्य;
    अन्यथा यदि( (kind1 == निवेश_Array_2::MATX) + (kind2 == निवेश_Array_2::MATX) == 1 ||
             !psrc1->sameSize(*psrc2) || type1 != type2 )
    {
        यदि( checkScalar(*psrc1, type2, kind1, kind2) )
        { std::swap(psrc1, psrc2); std::swap(type1, type2); }        // src1 is the scalar; swap it out
        अन्यथा यदि( !checkScalar(*psrc2, type1, kind2, kind1) )
            प्रत्यागम असत्य;
        haveScalar = सत्य;
    }
    _dst.createSameSize(*psrc1, type1);
    प्रत्यागम ocl_द्विआधारी_op(*psrc1, *psrc2, _dst, _mask, सत्य, oclop, haveScalar);
}
#endif  // HAVE_OPENCL

}

// bitwise and/or/xor: OpenCL runs the shared bitwise kernel (via bitwise_op_ocl); the CPU path is the
// element-wise engine (OP_AND/OR/XOR, T x T -> T dispatched by element size). oclop = -1 tells
// arithm_op the OpenCL path was already tried, so it only runs the CPU engine. NOT is unary.
शून्य cv::bitwise_and(निवेश_Array a, निवेश_Array b, OutputArray c, निवेश_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_AND))
    arithm_op(ew::OP_AND, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/असत्य);
}

शून्य cv::bitwise_or(निवेश_Array a, निवेश_Array b, OutputArray c, निवेश_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_OR))
    arithm_op(ew::OP_OR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/असत्य);
}

शून्य cv::bitwise_xor(निवेश_Array a, निवेश_Array b, OutputArray c, निवेश_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_XOR))
    arithm_op(ew::OP_XOR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/असत्य);
}

शून्य cv::bitwise_not(निवेश_Array a, OutputArray c, निवेश_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || c.isUMat()) && a.dims() <= 2,
               bitwise_op_ocl(a, a, c, mask, OCL_OP_NOT))
    unary_op(ew::OP_NOT, a, c, mask, -1);
}

शून्य cv::max( निवेश_Array src1, निवेश_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, असत्य);
}

शून्य cv::min( निवेश_Array src1, निवेश_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, असत्य);
}

// The concrete Mat/UMat overloads of min/max exist because C++ overload resolution needs them (a
// bare cv::min(mat, mat, mat) binds Mat& more tightly than InputArray); they must run the SAME engine
// path as the InputArray forms - NOT the legacy binary_op - so mixed types and broadcasting work.
शून्य cv::max(ध्रुव Mat& src1, ध्रुव Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, असत्य);
}

शून्य cv::min(ध्रुव Mat& src1, ध्रुव Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, असत्य);
}

शून्य cv::max(ध्रुव UMat& src1, ध्रुव UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, असत्य);
}

शून्य cv::min(ध्रुव UMat& src1, ध्रुव UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, असत्य);
}


/****************************************************************************************\
*                                      add/subtract                                      *
\****************************************************************************************/

नामाकाश cv
{

#ifdef HAVE_OPENCL

// used by the OpenCL branch only (arithm_op_ocl working-type selection)
स्थिर पूर्णाङ्क actualScalarDepth(ध्रुव द्विगुण* data, पूर्णाङ्क len)
{
    पूर्णाङ्क i = 0, minval = INT_MAX, maxval = INT_MIN;
    प्रति(; i < len; ++i)
    {
        पूर्णाङ्क ival = cvRound(data[i]);
        यदि( ival != data[i] )
            भङ्ग;
        minval = MIN(minval, ival);
        maxval = MAX(maxval, ival);
    }
    प्रत्यागम i < len ? CV_64F :
        minval >= 0 && maxval <= (पूर्णाङ्क)UCHAR_MAX ? CV_8U :
        minval >= (पूर्णाङ्क)SCHAR_MIN && maxval <= (पूर्णाङ्क)SCHAR_MAX ? CV_8S :
        minval >= 0 && maxval <= (पूर्णाङ्क)USHRT_MAX ? CV_16U :
        minval >= (पूर्णाङ्क)SHRT_MIN && maxval <= (पूर्णाङ्क)SHRT_MAX ? CV_16S :
        CV_32S;
}

स्थिर पूर्णाङ्क coerceTypes(पूर्णाङ्क depth1, पूर्णाङ्क depth2, तार्किक muldiv)
{
    प्रत्यागम depth1 == depth2 ? depth1 :
        ((depth1 <= CV_32S) & (depth2 <= CV_32S)) != 0 ?
        (((पूर्णाङ्क)!muldiv & (depth1 <= CV_8S) & (depth2 <= CV_8S)) != 0 ? CV_16S : CV_32S) :
        ((CV_ELEM_SIZE1(depth1) > 4) | (CV_ELEM_SIZE1(depth2) > 4)) != 0 ? CV_64F : CV_32F;
}

स्थिर तार्किक ocl_arithm_op(निवेश_Array _src1, निवेश_Array _src2, OutputArray _dst,
                          निवेश_Array _mask, पूर्णाङ्क wtype,
                          शून्य* usrdata, पूर्णाङ्क oclop,
                          तार्किक haveScalar )
{
    ध्रुव ocl::Device d = ocl::Device::getDefault();
    तार्किक doubleSupport = d.doubleFPConfig() > 0;
    पूर्णाङ्क type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    तार्किक haveMask = !_mask.empty();

    यदि ( (haveMask || haveScalar) && cn > 4 )
        प्रत्यागम असत्य;

#ifdef __APPLE__
    यदि (depth1 == CV_16U && (oclop == OCL_OP_MUL || oclop == OCL_OP_MUL_SCALE))
        प्रत्यागम असत्य;
#endif

    पूर्णाङ्क dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32S, CV_MAT_DEPTH(wtype));
    यदि (!doubleSupport)
        wdepth = std::min(wdepth, CV_32F);

    wtype = CV_MAKETYPE(wdepth, cn);
    पूर्णाङ्क type2 = haveScalar ? wtype : _src2.type(), depth2 = CV_MAT_DEPTH(type2);
    यदि (!doubleSupport && (depth2 == CV_64F || depth1 == CV_64F))
        प्रत्यागम असत्य;

    यदि (noOclType(depth1) || noOclType(depth2) || noOclType(ddepth) || noOclType(wdepth))
        प्रत्यागम असत्य;

    पूर्णाङ्क kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    पूर्णाङ्क scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = d.isIntel() ? 4 : 1;

    वर्ण cvtstr[4][50], opts[1024];
    snprintf(opts, परिमाण(opts), "-D %s%s -D %s -D srcT1=%s -D srcT1_C1=%s -D srcT2=%s -D srcT2_C1=%s "
            "-D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workT=%s -D workST=%s -D scaleT=%s -D wdepth=%d -D convertToWT1=%s "
            "-D convertToWT2=%s -D convertToDT=%s%s -D cn=%d -D rowsPerWI=%d -D convertFromU=%s",
            (haveMask ? "MASK_" : ""), (haveScalar ? "UNARY_OP" : "BINARY_OP"),
            oclop2str[oclop], ocl::typeToStr(CV_MAKETYPE(depth1, kercn)),
            ocl::typeToStr(depth1), ocl::typeToStr(CV_MAKETYPE(depth2, kercn)),
            ocl::typeToStr(depth2), ocl::typeToStr(CV_MAKETYPE(ddepth, kercn)), ddepth,
            ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKETYPE(wdepth, kercn)),
            ocl::typeToStr(CV_MAKETYPE(wdepth, scalarcn)),
            ocl::typeToStr(wdepth), wdepth,
            ocl::convertTypeStr(depth1, wdepth, kercn, cvtstr[0], परिमाण(cvtstr[0])),
            ocl::convertTypeStr(depth2, wdepth, kercn, cvtstr[1], परिमाण(cvtstr[1])),
            ocl::convertTypeStr(wdepth, ddepth, kercn, cvtstr[2], परिमाण(cvtstr[2])),
            doubleSupport ? " -D DOUBLE_SUPPORT" : "", kercn, rowsPerWI,
            oclop == OCL_OP_ABSDIFF && wdepth == CV_32S && ddepth == wdepth ?
            ocl::convertTypeStr(CV_8U, ddepth, kercn, cvtstr[3], परिमाण(cvtstr[3])) : "noconvert");

    size_t usrdata_esz = CV_ELEM_SIZE(wdepth);
    ध्रुव uchar* usrdata_p = (ध्रुव uchar*)usrdata;
    ध्रुव द्विगुण* usrdata_d = (ध्रुव द्विगुण*)usrdata;
    भिन्न usrdata_f[3];
    पूर्णाङ्क i, n = oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE ||
        oclop == OCL_OP_RDIV_SCALE || oclop == OCL_OP_RECIP_SCALE ? 1 : oclop == OCL_OP_ADDW ? 3 : 0;
    यदि( usrdata && n > 0 && wdepth == CV_32F )
    {
        प्रति( i = 0; i < n; i++ )
            usrdata_f[i] = (भिन्न)usrdata_d[i];
        usrdata_p = (ध्रुव uchar*)usrdata_f;
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::ReadOnlyNoSize(mask, 1);

    यदि( haveScalar )
    {
        size_t esz = CV_ELEM_SIZE1(wtype)*scalarcn;
        द्विगुण buf[4]={0,0,0,0};
        Mat src2sc = _src2.getMat();

        यदि( !src2sc.empty() )
            convertAndUnrollScalar(src2sc, wtype, (uchar*)buf, 1);
        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        यदि( !haveMask )
        {
            यदि(n == 0)
                k.args(src1arg, dstarg, scalararg);
            अन्यथा यदि(n == 1)
                k.args(src1arg, dstarg, scalararg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            अन्यथा
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        अन्यथा
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    अन्यथा
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::ReadOnlyNoSize(src2, cn, kercn);

        यदि( !haveMask )
        {
            यदि (n == 0)
                k.args(src1arg, src2arg, dstarg);
            अन्यथा यदि (n == 1)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            अन्यथा यदि (n == 3)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz, usrdata_esz),
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p + usrdata_esz*2, usrdata_esz));
            अन्यथा
                CV_Error(Error::StsNotImplemented, "unsupported number of extra parameters");
        }
        अन्यथा
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    प्रत्यागम k.run(2, globalsize, रिक्त, असत्य);
}

#endif

// OpenCL branch of the arithmetic ops, extracted from the former arithm_op (whose CPU compute is now
// the element-wise engine). Returns true if an OpenCL kernel handled the op, false to let the caller
// fall through to the CPU engine. min/max use the bitwise-style kernel; every other op the arithm
// kernel with a coerced working type. `usrdata` carries the mul/div/addWeighted scale block, `oclop`
// the kernel id. Only reached for a UMat dst; sizes/types that don't fit the scalar/same-size pattern
// (e.g. a broadcast) decline (return false) so the engine handles them on the CPU.
स्थिर तार्किक arithm_op_ocl(निवेश_Array _src1, निवेश_Array _src2, OutputArray _dst,
                          निवेश_Array _mask, पूर्णाङ्क dtype, पूर्णाङ्क oclop, तार्किक muldiv, शून्य* usrdata)
{
#ifndef HAVE_OPENCL
    CV_UNUSED(_src1); CV_UNUSED(_src2); CV_UNUSED(_dst); CV_UNUSED(_mask);
    CV_UNUSED(dtype); CV_UNUSED(oclop); CV_UNUSED(muldiv); CV_UNUSED(usrdata);
    प्रत्यागम असत्य;
#अन्यथा
    ध्रुव निवेश_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    निवेश_Array_2::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    तार्किक haveMask = !_mask.empty();
    तार्किक reallocate = असत्य;
    पूर्णाङ्क type1 = psrc1->type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    पूर्णाङ्क type2 = psrc2->type(), depth2 = CV_MAT_DEPTH(type2), cn2 = CV_MAT_CN(type2);
    पूर्णाङ्क wtype, dims1 = psrc1->dims(), dims2 = psrc2->dims();
    Size sz1 = dims1 <= 2 ? psrc1->size() : Size();
    Size sz2 = dims2 <= 2 ? psrc2->size() : Size();

    यदि (!(OCL_PERFORMANCE_CHECK(_dst.isUMat()) && dims1 <= 2 && dims2 <= 2))
        प्रत्यागम असत्य;

    // min/max: the bitwise-style OpenCL kernel (same-type element-wise, no working-type coercion).
    यदि (oclop == OCL_OP_MIN || oclop == OCL_OP_MAX)
    {
        तार्किक haveScalar = असत्य;
        यदि ((kind1 == निवेश_Array_2::MATX) + (kind2 == निवेश_Array_2::MATX) == 1 ||
            !psrc1->sameSize(*psrc2) || type1 != type2)
        {
            यदि (checkScalar(*psrc1, type2, kind1, kind2))
            { std::swap(psrc1, psrc2); std::swap(type1, type2); }
            अन्यथा यदि (!checkScalar(*psrc2, type1, kind2, kind1))
                प्रत्यागम असत्य;
            haveScalar = सत्य;
        }
        _dst.createSameSize(*psrc1, type1);
        प्रत्यागम ocl_द्विआधारी_op(*psrc1, *psrc2, _dst, _mask, असत्य, oclop, haveScalar);
    }

    // reciprocal (scale/src): the OpenCL kernel is unary on src - use src2 for both operands (the CPU
    // engine received a 0-dim `1` numerator as src1, which the recip kernel would ignore). [TODO.VP: tidy]
    यदि (oclop == OCL_OP_RECIP_SCALE)
    {
        psrc1 = psrc2; kind1 = kind2; type1 = type2; depth1 = depth2; cn = cn2; dims1 = dims2; sz1 = sz2;
    }

    // add/sub/mul/div/absdiff/addWeighted/recip: the arithm OpenCL kernel with a coerced work type.
    तार्किक src1Scalar = checkScalar(*psrc1, type2, kind1, kind2);
    तार्किक src2Scalar = checkScalar(*psrc2, type1, kind2, kind1);
    तार्किक haveScalar = असत्य;

    यदि (dims1 != dims2 || sz1 != sz2 || cn != cn2 ||
        (kind1 == निवेश_Array_2::MATX && (sz1 == Size(1,4) || sz1 == Size(1,1))) ||
        (kind2 == निवेश_Array_2::MATX && (sz2 == Size(1,4) || sz2 == Size(1,1))))
    {
        यदि ((type1 == CV_64F && (sz1.height == 1 || sz1.height == 4)) && src1Scalar)
        {
            // src1 is a scalar; swap it with src2
            std::swap(psrc1, psrc2); std::swap(sz1, sz2); std::swap(type1, type2);
            std::swap(depth1, depth2); std::swap(cn, cn2); std::swap(dims1, dims2);
            यदि (oclop == OCL_OP_SUB) oclop = OCL_OP_RSUB;
            यदि (oclop == OCL_OP_DIV_SCALE) oclop = OCL_OP_RDIV_SCALE;
        }
        अन्यथा यदि (!src2Scalar)
            प्रत्यागम असत्य;   // array op array with mismatched size/cn: engine broadcasts on the CPU
        haveScalar = सत्य;
        CV_Assert((type2 == CV_64F || type2 == CV_32F) && (sz2.height == 1 || sz2.height == 4));

        यदि (!muldiv)
        {
            Mat sc = psrc2->getMat();
            depth2 = actualScalarDepth(sc.ptr<द्विगुण>(), sz2 == Size(1, 1) ? cn2 : cn);
            यदि (depth2 == CV_64F && CV_ELEM_SIZE1(depth1) < 8)
                depth2 = CV_32F;
        }
        अन्यथा
            depth2 = CV_64F;
    }

    यदि (dtype < 0)
    {
        यदि (_dst.fixedType())
            dtype = _dst.type();
        अन्यथा
        {
            यदि (!haveScalar && type1 != type2)
                CV_Error(cv::Error::StsBadArg,
                     "When the input arrays in add/subtract/multiply/divide functions have different types, "
                     "the output array type must be explicitly specified");
            dtype = type1;
        }
    }
    dtype = CV_MAT_DEPTH(dtype);

    यदि (depth1 == depth2 && dtype == depth1)
        wtype = dtype;
    अन्यथा यदि (!muldiv)
    {
        wtype = coerceTypes(depth1, depth2, असत्य);
        wtype = coerceTypes(wtype, dtype, असत्य);
        यदि (dtype < CV_32F && (depth1 < CV_32F || depth2 < CV_32F))
            wtype = CV_32S;
    }
    अन्यथा
    {
        wtype = coerceTypes(depth1, depth2, सत्य);
        wtype = coerceTypes(wtype, dtype, सत्य);
    }

    // The scaled OpenCL kernels (mul/div by a scale, reciprocal, addWeighted) compute in float; the old
    // same-type fast path forced max(depth, CV_32F). coerceTypes keeps same-type integer, so bump here.
    यदि (oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE || oclop == OCL_OP_RDIV_SCALE ||
        oclop == OCL_OP_RECIP_SCALE || oclop == OCL_OP_ADDW)
        wtype = std::max(wtype, (पूर्णाङ्क)CV_32F);

    dtype = CV_MAKETYPE(dtype, cn);
    wtype = CV_MAKETYPE(wtype, cn);

    यदि (haveMask)
    {
        पूर्णाङ्क mtype = _mask.type();
        CV_Assert((mtype == CV_8UC1 || mtype == CV_8SC1 || mtype == CV_Bool) && _mask.sameSize(*psrc1));
        reallocate = !_dst.sameSize(*psrc1) || _dst.type() != dtype;
    }

    _dst.createSameSize(*psrc1, dtype);
    यदि (reallocate)
        _dst.setTo(0.);

    प्रत्यागम ocl_arithm_op(*psrc1, *psrc2, _dst, _mask, wtype, usrdata, oclop, haveScalar);
#endif
}

// Element-wise binary op (ADD/SUB/...) on two CPU operands - each an array Mat or a MATX scalar -
// through the new broadcasting engine. Handles mixed input types (emitBinary inserts the casts) and
// broadcasting (a scalar rides as a 0-dim per-channel CONST; arrays broadcast via outputShape/exec).
// Returns false (declining) for UMat, a write-mask, or two scalar operands, so the caller falls
// through to arithm_op. Incompatible shapes make the engine throw (matching cv::add's error).
// Does the output array already have exactly this shape+type (=> create() reuses it, no realloc)?
// Works for Mat and UMat alike (shape/type queries, no data-pointer peeking).
स्थिर तार्किक outArrayMatches(ध्रुव _OutputArray& a, ध्रुव MatShape& shp, पूर्णाङ्क type)
{
    यदि (a.empty() || a.type() != type)
        प्रत्यागम असत्य;
    पूर्णाङ्क sz[MatShape::MAX_DIMS];
    पूर्णाङ्क nd = a.sizend(sz);
    यदि (nd != (पूर्णाङ्क)shp.size())
        प्रत्यागम असत्य;
    प्रति (पूर्णाङ्क i = 0; i < nd; i++)
        यदि (sz[i] != shp[i])
            प्रत्यागम असत्य;
    प्रत्यागम सत्य;
}

// Are two operands' iteration shapes (spatial dims + channels innermost, exactly how the engine
// iterates) numpy-broadcast-compatible? Powers the scalar-like-Mat compat fallback in arithm_op.
// Cheap by design: sizend copies the dim arrays inline - no Mat headers, no allocations.
स्थिर तार्किक shapesBroadcastCompat(निवेश_Array a, पूर्णाङ्क acn, निवेश_Array b, पूर्णाङ्क bcn)
{
    पूर्णाङ्क asz[MatShape::MAX_DIMS + 1], bsz[MatShape::MAX_DIMS + 1];
    पूर्णाङ्क ad = a.sizend(asz), bd = b.sizend(bsz);
    asz[ad++] = acn;
    bsz[bd++] = bcn;
    प्रति (पूर्णाङ्क i1 = ad - 1, i2 = bd - 1; i1 >= 0 && i2 >= 0; i1--, i2--)
        यदि (asz[i1] != bsz[i2] && asz[i1] != 1 && bsz[i2] != 1)
            प्रत्यागम असत्य;
    प्रत्यागम सत्य;
}

// The one entry point for every element-wise binary op (add/sub/mul/div/min/max/absdiff/addWeighted/
// reciprocal). Handles empty inputs, the UMat/OpenCL branch (arithm_op_ocl) and the CPU element-wise
// engine. `oclop` selects the OpenCL kernel; `muldiv` drives the OpenCL working-type coercion + scale;
// `params` are the op scalars (params[0]=mul/div scale; {alpha,beta,gamma} for addWeighted).
// Below this many ELEMENTS (total * channels), the per-call constant overhead of the engine path
// (program build + compile + executor setup) is comparable to the actual work - take the direct
// kernel call instead. Same tier boundary as math_op's MATH_OP_SMALL for cv::exp.
गणना { ARITHM_SMALL_DIRECT = 100000 };

स्थिर शून्य arithm_op(ew::शीर्ष op, निवेश_Array src1, निवेश_Array src2, OutputArray dst,
                      निवेश_Array mask, पूर्णाङ्क dtype, पूर्णाङ्क oclop, तार्किक muldiv, ध्रुव Scalar& params)
{
    यदि (src1.empty() || src2.empty())          // empty inputs -> empty result
    {
        CV_Assert((src1.empty() && src2.empty()) ||
                  (src1.empty() && isScalarArg(src2, src1.channels())) ||
                  (src2.empty() && isScalarArg(src1, src2.channels())));
        dst.release();
        यदि (dtype >= 0)
            dst.create(0, 0, dtype);
        प्रत्यागम;
    }

    // UMat -> classic OpenCL kernel. It declines (false) when OpenCL can't apply (no device, dims>2, a
    // broadcast, cn>4 masked, ...) - then the CPU engine below handles it (mapping the UMat via getMat).
    // oclop < 0 is a sentinel: the caller (e.g. cv::compare) already ran its own OpenCL path, so this
    // helper only runs the CPU engine (a UMat operand is mapped via getMat).
    यदि (oclop >= 0 && cv::ocl::isOpenCLActivated() &&
        (src1.isUMat() || src2.isUMat() || dst.isUMat()))
    {
        द्विगुण abg[3] = { params[0], params[1], params[2] }, scale = params[0];
        शून्य* usrdata = (op == ew::OP_ADDW) ? (शून्य*)abg : (muldiv ? (शून्य*)&scale : nullptr);
        यदि (arithm_op_ocl(src1, src2, dst, mask, dtype, oclop, muldiv, usrdata))
            प्रत्यागम;
    }

    ध्रुव तार्किक haveMask = !mask.empty();
    ध्रुव पूर्णाङ्क cn1 = src1.channels(), cn2 = src2.channels();
    तार्किक s1 = isScalarArg(src1, src2), s2 = isScalarArg(src2, src1);
    यदि (s1 && s2)
        s1 = s2 = असत्य;    // two scalars: treat both as tiny array operands (element-wise + broadcast),
                            // matching arithm_op (Scalar+Scalar -> 4x1, number+number -> 1x1, ...)
    // A 4x1 CV_64F Mat/UMat pseudo-scalar (see isScalarArg) is hijacked only against a REAL array:
    // when the partner is itself a tiny scalar-shaped array (e.g. compare(Mat 4x1, Mat 1x1) -
    // issue #8999), both are honest data and ride the broadcast. A genuine MATX Scalar never demotes.
    अन्यथा यदि (s1 && src1.kind() != निवेश_Array_2::MATX && isScalarLikeMat(src2, cn1))
        s1 = असत्य;
    अन्यथा यदि (s2 && src2.kind() != निवेश_Array_2::MATX && isScalarLikeMat(src1, cn2))
        s2 = असत्य;

    // Compat fallback: Java/Python/user code passes scalars as real little Mats (the classic 4x1
    // CV_64F column, a 1xcn/cnx1 vector - see isScalarLikeMat). Those normally ride broadcasting
    // now, so hijack one as a per-channel scalar ONLY when the shapes do NOT broadcast - just the
    // calls that would otherwise throw get the 4.x scalar semantics, every valid broadcast keeps
    // its numpy meaning. Cheap: two geometry probes, and the O(ndims) shape walk runs only when a
    // probe hits (never for ordinary same-size or MATX-scalar calls).
    यदि (!s1 && !s2)
    {
        तार्किक like1 = isScalarLikeMat(src1, cn2), like2 = isScalarLikeMat(src2, cn1);
        यदि ((like1 || like2) && !shapesBroadcastCompat(src1, cn1, src2, cn2))
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
    ध्रुव शून्य* dstObj = (dst.kind() == निवेश_Array_2::MAT) ? dst.getObj() : nullptr;
    ध्रुव तार्किक aliased = dstObj && (dstObj == src1.getObj() || dstObj == src2.getObj());
    Mat m1loc, m2loc, mloc;
    auto asMat = [&](निवेश_Array a, Mat& loc) -> ध्रुव Mat* {
        यदि (a.kind() == निवेश_Array_2::MAT && !aliased) प्रत्यागम (ध्रुव Mat*)a.getObj();
        loc = a.getMat(); प्रत्यागम &loc;
    };
    ध्रुव Mat* pm1 = s1 ? nullptr : asMat(src1, m1loc);
    ध्रुव Mat* pm2 = s2 ? nullptr : asMat(src2, m2loc);
    ध्रुव पूर्णाङ्क adepth = s1 ? pm2->depth() : pm1->depth();   // the (first) array operand's depth
    // auto result depth (no explicit dtype, no fixed-type dst): add/sub/mul/div keep the first array
    // operand's depth (cv::'s dtype==-1 convention), but min/max/absdiff have NO dtype argument and
    // must promote mixed inputs to their common type (min(u32,f32) in u32 would drop the float). For
    // same-type inputs promoteArith(da,db)==da, so classic behaviour is preserved (e.g. absdiff of two
    // s16 stays s16, saturating - the wide unsigned |a-b| is computed then cast back down by emitBinary).
    पूर्णाङ्क autoDepth = adepth;
    यदि (op == ew::OP_MIN || op == ew::OP_MAX || op == ew::OP_ABSDIFF)
    {
        ध्रुव पूर्णाङ्क d1 = s1 ? ew::EW_DEPTH_NONE : pm1->depth();
        ध्रुव पूर्णाङ्क d2 = s2 ? ew::EW_DEPTH_NONE : pm2->depth();
        autoDepth = ew::promoteArith(d1, d2);
    }
    // result depth: explicit dtype wins; else a fixed-type dst dictates it; else the auto depth above.
    ध्रुव पूर्णाङ्क rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
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
    यदि (!haveMask && !s1 && !s2 &&
        (op == ew::OP_ADD || op == ew::OP_SUB || op == ew::OP_MIN || op == ew::OP_MAX ||
         op == ew::OP_ABSDIFF || op == ew::OP_MUL || op == ew::OP_ADDW ||
         op == ew::OP_AND || op == ew::OP_OR || op == ew::OP_XOR) &&
        rdepth == pm1->depth() && pm1->type() == pm2->type() && pm1->size == pm2->size &&
        pm1->isContinuous() && pm2->isContinuous() &&
        pm1->total()*cn1 <= (size_t)ARITHM_SMALL_DIRECT)
    {
        ew::TKernel k = ew::getElemwiseFunc(op, rdepth, rdepth, ew::EW_DEPTH_NONE, rdepth);
        यदि (k.fptr)
        {
            dst.createSameSize(src1, pm1->type());   // whole-shape transfer, not piecemeal dims+sizes
            Mat dloc;
            Mat* pd = (dst.kind() == निवेश_Array_2::MAT) ? (Mat*)dst.getObj() : &(dloc = dst.getMat());
            यदि (pd->isContinuous())     // a reused non-continuous dst view falls through to the engine
            {
                k.fptr(pm1->data, 0, 1, pm2->data, 0, 1, nullptr, 0, 0,
                       pd->data, 0, (पूर्णाङ्क)(pm1->total()*cn1), 1, params.val, k.flags, k.userdata);
                प्रत्यागम;
            }
        }
    }

    // operand order = (src1, src2): an array becomes an INPUT, a scalar a flexible per-channel CONST
    // read straight from the caller's inline storage (getObj()), no Mat / convertTo.
    ew::TExpr p;
    auto addOperand = [&](निवेश_Array src, तार्किक isScalar, ध्रुव Mat* m, पूर्णाङ्क otherCn) -> पूर्णाङ्क {
        यदि (!isScalar)
            प्रत्यागम p.add_निवेश(m->depth());
        ध्रुव uchar* sp; पूर्णाङ्क sd;
        uchar scbuf[EW_SCALAR_BUF_SIZE];   // stack room for a UMAT scalar; addConst copies the values out
        ध्रुव पूर्णाङ्क scn0 = scalarArgElems(src, sp, sd, scbuf), cn = otherCn;
        ध्रुव पूर्णाङ्क scn = (cn == scn0 || (cn < 4 && scn0 == 4)) ? cn
                      : (scn0 == 1) ? 1 : 0;
        CV_Assert(scn > 0);
        प्रत्यागम p.addConst(ew::EW_DEPTH_NONE, sd, sp, scn);
    };
    ध्रुव पूर्णाङ्क a0 = addOperand(src1, s1, pm1, cn2);
    ध्रुव पूर्णाङ्क a1 = addOperand(src2, s2, pm2, cn1);

    // Write-mask: a single-channel 1-byte array (u8/s8/bool) the size of the output spatial shape,
    // added as another broadcast input. The arithmetic result lands in a temp; a final
    // select(mask, r, dst) -> dst overwrites only mask!=0 positions of the (pre-existing) output.
    पूर्णाङ्क sMask = 0;
    ध्रुव Mat* pmask = nullptr;
    यदि (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = asMat(mask, mloc);
        sMask = p.add_निवेश(pmask->depth());
    }
    ध्रुव पूर्णाङ्क sOut = p.addOutput(rdepth);               // output BEFORE emit -> moveToOutput drops the
    ध्रुव पूर्णाङ्क r = p.emit_द्विआधारी(op, a0, a1, rdepth, params);   // result temp (mask-free => none); scale
                                                        // rides params[0] (mul/div), {a,b,g} for addW
    यदि (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    अन्यथा
        p.moveToOutput(r, sOut);
    p.compile();

    ध्रुव Mat* inputs[3]; पूर्णाङ्क ni = 0;                   // inputs in program (input-index) order
    यदि (!s1) inputs[ni++] = pm1;
    यदि (!s2) inputs[ni++] = pm2;
    यदि (haveMask) inputs[ni++] = pmask;

    // Pre-create the output at the broadcast shape, ALWAYS via dst.create() - never hand exec a raw
    // dst Mat. Routing through the _OutputArray is what enforces its contract: a FIXED_SIZE/FIXED_TYPE
    // dst (in-place 'a += b' passes '(const Mat&)a') throws here if a broadcast would change its shape
    // or type (Mat(1x1) += Mat(1x4)), instead of silently reallocating - exec sees only a Mat and has
    // no view of the array's fixed flags. Cost over a direct exec is negligible: exec keeps its own
    // fast path for a matching-shape op, and dst.create() with an unchanged shape/type is a no-op.
    // With a mask, a freshly (re)allocated dst is zeroed so mask==0 reads 0 (matches arithm_op); a
    // reused dst keeps its prior content there - detect reuse by shape+type BEFORE create (correct for
    // a reused UMat dst too).
    MatShape oshape; पूर्णाङ्क ocn;
    p.outputShape(inputs, oshape, ocn);
    ध्रुव पूर्णाङ्क otype = CV_MAKETYPE(rdepth, ocn);
    ध्रुव तार्किक reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == निवेश_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    यदि (haveMask && !reused)
        pdst->setZero();                          // setZero (not setTo, which caps at 4 channels)
    p.exec(inputs, pdst);
}

// Unary element-wise op through the engine (bitwise NOT now; unary math later). One array input, an
// optional write-mask; result depth = dtype (>=0), else a fixed-type dst's depth, else the input
// depth. Mirrors arithm_op's build/mask/exec tail with a single input and no scalar operand. OpenCL is
// run by the caller (bitwise_op_ocl), so this is the CPU engine only.
स्थिर शून्य unary_op(ew::शीर्ष op, निवेश_Array src, OutputArray dst, निवेश_Array mask,
                     पूर्णाङ्क dtype, ध्रुव Scalar& params)
{
    यदि (src.empty())
    {
        dst.release();
        यदि (dtype >= 0)
            dst.create(0, 0, dtype);
        प्रत्यागम;
    }

    ध्रुव तार्किक haveMask = !mask.empty();
    // in-place (dst aliases src): keep a header copy so a shape/type-changing dst.create() can't free
    // src's data mid-op (matches arithm_op). A fixed-size/type dst never reallocs, so this is only for
    // an explicit dtype change.
    ध्रुव शून्य* dstObj = (dst.kind() == निवेश_Array_2::MAT) ? dst.getObj() : nullptr;
    ध्रुव तार्किक aliased = dstObj && src.kind() == निवेश_Array_2::MAT && dstObj == src.getObj();
    Mat sloc, mloc;
    ध्रुव Mat* psrc = (src.kind() == निवेश_Array_2::MAT && !aliased) ? (ध्रुव Mat*)src.getObj()
                                                                   : &(sloc = src.getMat());
    ध्रुव पूर्णाङ्क rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
                     : (dst.fixedType() ? dst.depth() : psrc->depth());

    ew::TExpr p;
    ध्रुव पूर्णाङ्क a0 = p.add_निवेश(psrc->depth());
    पूर्णाङ्क sMask = 0;
    ध्रुव Mat* pmask = nullptr;
    यदि (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = (mask.kind() == निवेश_Array_2::MAT) ? (ध्रुव Mat*)mask.getObj() : &(mloc = mask.getMat());
        sMask = p.add_निवेश(pmask->depth());
    }
    ध्रुव पूर्णाङ्क sOut = p.addOutput(rdepth);
    ध्रुव पूर्णाङ्क r = p.emitUnary(op, a0, rdepth, params);
    यदि (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    अन्यथा
        p.moveToOutput(r, sOut);
    p.compile();

    ध्रुव Mat* inputs[2]; पूर्णाङ्क ni = 0;
    inputs[ni++] = psrc;
    यदि (haveMask) inputs[ni++] = pmask;

    MatShape oshape; पूर्णाङ्क ocn;
    p.outputShape(inputs, oshape, ocn);
    ध्रुव पूर्णाङ्क otype = CV_MAKETYPE(rdepth, ocn);
    ध्रुव तार्किक reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == निवेश_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    यदि (haveMask && !reused)
        pdst->setZero();
    p.exec(inputs, pdst);
}

}

शून्य cv::add( निवेश_Array src1, निवेश_Array src2, OutputArray dst,
          निवेश_Array mask, पूर्णाङ्क dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADD, src1, src2, dst, mask, dtype, OCL_OP_ADD, असत्य);
}

शून्य cv::subtract( निवेश_Array src1, निवेश_Array src2, OutputArray dst,
                   निवेश_Array mask, पूर्णाङ्क dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_SUB, src1, src2, dst, mask, dtype, OCL_OP_SUB, असत्य);
}

शून्य cv::absdiff( निवेश_Array src1, निवेश_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ABSDIFF, src1, src2, dst, noArray(), -1, OCL_OP_ABSDIFF, असत्य);
}

शून्य cv::copyTo(निवेश_Array _src, OutputArray _dst, निवेश_Array _mask)
{
    CV_INSTRUMENT_REGION();

    _src.copyTo(_dst, _mask);
}

/****************************************************************************************\
*                                    multiply/divide                                     *
\****************************************************************************************/

नामाकाश cv
{

शून्य multiply(निवेश_Array src1, निवेश_Array src2,
              OutputArray dst, द्विगुण scale, पूर्णाङ्क dtype)
{
    CV_INSTRUMENT_REGION();

    ध्रुव पूर्णाङ्क oclop = std::abs(scale - 1.0) < DBL_EPSILON ? OCL_OP_MUL : OCL_OP_MUL_SCALE;
    arithm_op(ew::OP_MUL, src1, src2, dst, noArray(), dtype, oclop, /*muldiv*/ सत्य, Scalar(scale));
}

शून्य divide(निवेश_Array src1, निवेश_Array src2,
                OutputArray dst, द्विगुण scale, पूर्णाङ्क dtype)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_DIV, src1, src2, dst, noArray(), dtype, OCL_OP_DIV_SCALE, /*muldiv*/ सत्य, Scalar(scale));
}

शून्य divide(द्विगुण scale, निवेश_Array src2,
                OutputArray dst, पूर्णाङ्क dtype)
{
    CV_INSTRUMENT_REGION();

    यदि (src2.empty())
    {
        dst.release();
        प्रत्यागम;
    }

    // scale / src2 == scale * 1 / src2: feed a 0-dim `1` as the numerator so the CPU engine reuses the
    // normal divide (scale*a/b). Give the numerator src2's OWN depth: then OP_DIV(T, T) is same-type and
    // takes the fast per-type kernel (f32 work for <=16-bit / f16 / bf16, not the s32->f64 path a CV_32S
    // numerator would force). An integer src2 keeps its div-by-zero->0 guard (both operands integer); a
    // float src2 divides as float (1/0->inf), both matching cv::divide. dtype<0 follows src2's depth (a
    // fixed-type dst still wins). The UMat path uses the dedicated reciprocal kernel (OCL_OP_RECIP_SCALE).
    द्विगुण one = 1;
    Mat numerator(MatShape::scalar(), src2.depth(), &one);
    scalarToRawData(Scalar(1), &one, src2.depth(), 1);       // write `1` in src2's depth (no allocation)
    ध्रुव पूर्णाङ्क rtype = (dtype < 0 && !dst.fixedType()) ? src2.depth() : dtype;
    arithm_op(ew::OP_DIV, numerator, src2, dst, noArray(), rtype, OCL_OP_RECIP_SCALE, /*muldiv*/ सत्य, Scalar(scale));
}

UMat UMat::mul(निवेश_Array m, द्विगुण scale) ध्रुव
{
    UMat dst;
    multiply(*एतत्, m, dst, scale);
    प्रत्यागम dst;
}

/****************************************************************************************\
*                                      addWeighted                                       *
\****************************************************************************************/

}

शून्य cv::addWeighted( निवेश_Array src1, द्विगुण alpha, निवेश_Array src2,
                      द्विगुण beta, द्विगुण gamma, OutputArray dst, पूर्णाङ्क dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADDW, src1, src2, dst, noArray(), dtype, OCL_OP_ADDW, /*muldiv*/ सत्य,
              Scalar(alpha, beta, gamma));
}


/****************************************************************************************\
*                                          compare                                       *
\****************************************************************************************/

नामाकाश cv
{


स्थिर द्विगुण getMinVal(पूर्णाङ्क depth)
{
    स्थिर ध्रुव द्विगुण tab[CV_DEPTH_MAX] =
    {
        0, -128, 0, -32768, INT_MIN, -FLT_MAX, -DBL_MAX,
        -65504, -FLT_MAX, 0, 0, (द्विगुण)INT64_MIN, 0
    };
    प्रत्यागम tab[depth];
}

स्थिर द्विगुण getMaxVal(पूर्णाङ्क depth)
{
    स्थिर ध्रुव द्विगुण tab[CV_DEPTH_MAX] = {
        255, 127, 65535, 32767, INT_MAX, FLT_MAX, DBL_MAX,
        65504, FLT_MAX, 255, (द्विगुण)UINT64_MAX, (द्विगुण)INT64_MAX, (द्विगुण)UINT32_MAX, 0
    };
    प्रत्यागम tab[depth];
}

#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_compare(निवेश_Array _src1, निवेश_Array _src2, OutputArray _dst, पूर्णाङ्क op, तार्किक haveScalar)
{
    ध्रुव ocl::Device& dev = ocl::Device::getDefault();
    तार्किक doubleSupport = dev.doubleFPConfig() > 0;
    पूर्णाङ्क type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1),
            type2 = _src2.type(), depth2 = CV_MAT_DEPTH(type2);

    यदि (!doubleSupport && depth1 == CV_64F)
        प्रत्यागम असत्य;

    // the integer scalar path below works in int; let the CPU handle the wider 5.x integer depths
    यदि (haveScalar && (depth1 == CV_32U || depth1 == CV_64U || depth1 == CV_64S))
        प्रत्यागम असत्य;

    यदि (!haveScalar && (!_src1.sameSize(_src2) || type1 != type2))
            प्रत्यागम असत्य;

    // This OpenCL kernel broadcasts a scalar's FIRST channel to all channels. The CPU path compares
    // per-channel (scalar[c] for channel c), so a multichannel array against a scalar with DISTINCT
    // channel values would disagree - bail to the CPU engine in that case (rare; not worth a kernel fix).
    यदि (haveScalar && cn > 1)
    {
        Mat sc = _src2.getMat();
        पूर्णाङ्क scn = (पूर्णाङ्क)sc.total();
        यदि (scn > 1)
        {
            द्विगुण v[4] = { 0, 0, 0, 0 };
            getConvertFunc(sc.depth(), CV_64F)(sc.ptr(), 1, 0, 1, (uchar*)v, 1, Size(std::min(scn, 4), 1), 0);
            प्रति (पूर्णाङ्क i = 1, n = std::min(cn, scn); i < n; i++)
                यदि (v[i] != v[0])
                    प्रत्यागम असत्य;
        }
    }

    पूर्णाङ्क kercn = haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst), rowsPerWI = dev.isIntel() ? 4 : 1;
    // Workaround for bug with "?:" operator in AMD OpenCL compiler
    यदि (depth1 >= CV_16U)
        kercn = 1;

    पूर्णाङ्क scalarcn = kercn == 3 ? 4 : kercn;
    ध्रुव वर्ण * ध्रुव operationMap[] = { "==", ">", ">=", "<", "<=", "!=" };
    वर्ण cvt[50];

    String opts = format("-D %s -D srcT1=%s -D dstT=%s -D DEPTH_dst=%d -D workT=srcT1 -D cn=%d"
                         " -D convertToDT=%s -D OP_CMP -D CMP_OPERATOR=%s -D srcT1_C1=%s"
                         " -D srcT2_C1=%s -D dstT_C1=%s -D workST=%s -D rowsPerWI=%d%s",
                         haveScalar ? "UNARY_OP" : "BINARY_OP",
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, kercn)),
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U, kercn,
                         ocl::convertTypeStr(depth1, CV_8U, kercn, cvt, परिमाण(cvt)),
                         operationMap[op], ocl::typeToStr(depth1),
                         ocl::typeToStr(depth1), ocl::typeToStr(CV_8U),
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, scalarcn)), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat src1 = _src1.getUMat();
    Size size = src1.size();
    _dst.create(size, CV_8UC(cn));
    UMat dst = _dst.getUMat();

    यदि (haveScalar)
    {
        size_t esz = CV_ELEM_SIZE1(type1) * scalarcn;
        द्विगुण buf[4] = { 0, 0, 0, 0 };
        Mat src2 = _src2.getMat();

        यदि( depth1 > CV_32S )
            convertAndUnrollScalar( src2, depth1, (uchar *)buf, kercn );
        अन्यथा
        {
            द्विगुण fval = 0;
            getConvertFunc(depth2, CV_64F)(src2.ptr(), 1, 0, 1, (uchar *)&fval, 1, Size(1, 1), 0);
            यदि( fval < getMinVal(depth1) )
                प्रत्यागम dst.setTo(Scalar::all(op == CMP_GT || op == CMP_GE || op == CMP_NE ? 255 : 0)), सत्य;

            यदि( fval > getMaxVal(depth1) )
                प्रत्यागम dst.setTo(Scalar::all(op == CMP_LT || op == CMP_LE || op == CMP_NE ? 255 : 0)), सत्य;

            पूर्णाङ्क ival = cvRound(fval);
            यदि( fval != ival )
            {
                यदि( op == CMP_LT || op == CMP_GE )
                    ival = cvCeil(fval);
                अन्यथा यदि( op == CMP_LE || op == CMP_GT )
                    ival = cvFloor(fval);
                अन्यथा
                    प्रत्यागम dst.setTo(Scalar::all(op == CMP_NE ? 255 : 0)), सत्य;
            }
            convertAndUnrollScalar(Mat(1, 1, CV_32S, &ival), depth1, (uchar *)buf, kercn);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        k.args(ocl::KernelArg::ReadOnlyNoSize(src1, cn, kercn),
               ocl::KernelArg::WriteOnly(dst, cn, kercn), scalararg);
    }
    अन्यथा
    {
        UMat src2 = _src2.getUMat();

        k.args(ocl::KernelArg::ReadOnlyNoSize(src1),
               ocl::KernelArg::ReadOnlyNoSize(src2),
               ocl::KernelArg::WriteOnly(dst, cn, kercn));
    }

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    प्रत्यागम k.run(2, globalsize, रिक्त, असत्य);
}

#endif

}

शून्य cv::compare(निवेश_Array _src1, निवेश_Array _src2, OutputArray _dst, पूर्णाङ्क op)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( op == CMP_LT || op == CMP_LE || op == CMP_EQ ||
               op == CMP_NE || op == CMP_GE || op == CMP_GT );

    CV_Assert(_src1.empty() == _src2.empty());
    यदि (_src1.empty() && _src2.empty())
    {
        _dst.release();
        प्रत्यागम;
    }

    तार्किक haveScalar = असत्य;

    यदि ((_src1.isMatx() + _src2.isMatx()) == 1
            || !_src1.sameSize(_src2)
            || _src1.type() != _src2.type())
    {
        तार्किक is_src1_scalar = checkScalar(_src1, _src2.type(), _src1.kind(), _src2.kind());
        तार्किक is_src2_scalar = checkScalar(_src2, _src1.type(), _src2.kind(), _src1.kind());

        // exactly one scalar operand: keep it as src2 (swapping flips the ordering ops) so the OpenCL
        // path sees the array-op-scalar form. Otherwise - two arrays of different size (broadcast),
        // mixed types, or two scalars - the CPU engine broadcasts / promotes them, so there is NO error
        // here: fall through to arithm_op (which handles scalar detection, broadcast and mixed types).
        यदि (is_src1_scalar && !is_src2_scalar)
        {
            op = op == CMP_LT ? CMP_GT : op == CMP_LE ? CMP_GE :
                op == CMP_GE ? CMP_LE : op == CMP_GT ? CMP_LT : op;
            // src1 is a scalar; swap it with src2
            compare(_src2, _src1, _dst, op);
            प्रत्यागम;
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
    ew::शीर्ष cmpOp = op == CMP_EQ ? ew::OP_CMP_EQ : op == CMP_NE ? ew::OP_CMP_NE :
                    op == CMP_LT ? ew::OP_CMP_LT : op == CMP_LE ? ew::OP_CMP_LE :
                    op == CMP_GT ? ew::OP_CMP_GT : ew::OP_CMP_GE;
    arithm_op(cmpOp, _src1, _src2, _dst, noArray(), CV_8U, /*oclop=*/-1, /*muldiv=*/असत्य);
}

/****************************************************************************************\
*                                        inRange                                         *
\****************************************************************************************/

नामाकाश cv
{

template <typename T>
संरचना InRange_SIMD
{
    पूर्णाङ्क operator () (ध्रुव T * src1, ध्रुव T * src2, ध्रुव T * src3, uchar * dst, पूर्णाङ्क len) ध्रुव
    {
        प्रति (पूर्णाङ्क x = 0; x < len; x++)
            dst[x] = (src2[x] <= src1[x] && src1[x] <= src3[x]) ? 255 : 0;
        प्रत्यागम len;
    }
};

#यदि (CV_SIMD || CV_SIMD_SCALABLE)

template <>
संरचना InRange_SIMD<uchar>
{
    पूर्णाङ्क operator () (ध्रुव uchar * src1, ध्रुव uchar * src2, ध्रुव uchar * src3,
        uchar * dst, पूर्णाङ्क len) ध्रुव
    {
        पूर्णाङ्क x = 0;
        ध्रुव पूर्णाङ्क width = VTraits<v_uint8>::vlanes();

        प्रति (; x <= len - width; x += width)
        {
            v_uint8 values = vx_load(src1 + x);
            v_uint8 निचला = vx_load(src2 + x);
            v_uint8 ऊपरी = vx_load(src3 + x);

            v_store(dst + x, v_and(v_ge(values, निचला), v_ge(ऊपरी, values)));
        }
        vx_cleanup();
        प्रत्यागम x;
    }
};

template <>
संरचना InRange_SIMD<schar>
{
    पूर्णाङ्क operator () (ध्रुव schar * src1, ध्रुव schar * src2, ध्रुव schar * src3,
        uchar * dst, पूर्णाङ्क len) ध्रुव
    {
        पूर्णाङ्क x = 0;
        ध्रुव पूर्णाङ्क width = VTraits<v_int8>::vlanes();

        प्रति (; x <= len - width; x += width)
        {
            v_int8 values = vx_load(src1 + x);
            v_int8 निचला = vx_load(src2 + x);
            v_int8 ऊपरी = vx_load(src3 + x);

            v_store((schar*)(dst + x), v_and(v_ge(values, निचला), v_ge(ऊपरी, values)));
        }
        vx_cleanup();
        प्रत्यागम x;
    }
};

template <>
संरचना InRange_SIMD<ushort>
{
    पूर्णाङ्क operator () (ध्रुव ushort * src1, ध्रुव ushort * src2, ध्रुव ushort * src3,
        uchar * dst, पूर्णाङ्क len) ध्रुव
    {
        पूर्णाङ्क x = 0;
        ध्रुव पूर्णाङ्क width = VTraits<v_uint16>::vlanes() * 2;

        प्रति (; x <= len - width; x += width)
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
        प्रत्यागम x;
    }
};

template <>
संरचना InRange_SIMD<short>
{
    पूर्णाङ्क operator () (ध्रुव short * src1, ध्रुव short * src2, ध्रुव short * src3,
        uchar * dst, पूर्णाङ्क len) ध्रुव
    {
        पूर्णाङ्क x = 0;
        ध्रुव पूर्णाङ्क width = (पूर्णाङ्क)VTraits<v_int16>::vlanes() * 2;

        प्रति (; x <= len - width; x += width)
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
        प्रत्यागम x;
    }
};

template <>
संरचना InRange_SIMD<पूर्णाङ्क>
{
    पूर्णाङ्क operator () (ध्रुव पूर्णाङ्क * src1, ध्रुव पूर्णाङ्क * src2, ध्रुव पूर्णाङ्क * src3,
        uchar * dst, पूर्णाङ्क len) ध्रुव
    {
        पूर्णाङ्क x = 0;
        ध्रुव पूर्णाङ्क width = (पूर्णाङ्क)VTraits<v_int32>::vlanes() * 2;

        प्रति (; x <= len - width; x += width)
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
        प्रत्यागम x;
    }
};

template <>
संरचना InRange_SIMD<भिन्न>
{
    पूर्णाङ्क operator () (ध्रुव भिन्न * src1, ध्रुव भिन्न * src2, ध्रुव भिन्न * src3,
        uchar * dst, पूर्णाङ्क len) ध्रुव
    {
        पूर्णाङ्क x = 0;
        ध्रुव पूर्णाङ्क width = (पूर्णाङ्क)VTraits<v_float32>::vlanes() * 2;

        प्रति (; x <= len - width; x += width)
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
        प्रत्यागम x;
    }
};

template <>
संरचना InRange_SIMD<hfloat>
{
    पूर्णाङ्क operator () (ध्रुव hfloat * src1, ध्रुव hfloat * src2, ध्रुव hfloat * src3,
        uchar * dst, पूर्णाङ्क len) ध्रुव
    {
        पूर्णाङ्क x = 0;
        ध्रुव पूर्णाङ्क width = (पूर्णाङ्क)VTraits<v_float32>::vlanes()*2;

        प्रति (; x <= len - width; x += width)
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
        प्रत्यागम x;
    }
};

template <>
संरचना InRange_SIMD<bfloat>
{
    पूर्णाङ्क operator () (ध्रुव bfloat * src1, ध्रुव bfloat * src2, ध्रुव bfloat * src3,
        uchar * dst, पूर्णाङ्क len) ध्रुव
    {
        पूर्णाङ्क x = 0;
        ध्रुव पूर्णाङ्क width = (पूर्णाङ्क)VTraits<v_float32>::vlanes()*2;

        प्रति (; x <= len - width; x += width)
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
        प्रत्यागम x;
    }
};

template <>
संरचना InRange_SIMD<unsigned>
{
    पूर्णाङ्क operator () (ध्रुव unsigned * src1, ध्रुव unsigned * src2, ध्रुव unsigned * src3,
        uchar * dst, पूर्णाङ्क len) ध्रुव
    {
        पूर्णाङ्क x = 0;
        ध्रुव पूर्णाङ्क width = (पूर्णाङ्क)VTraits<v_uint32>::vlanes() * 2;

        प्रति (; x <= len - width; x += width)
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
        प्रत्यागम x;
    }
};

#यदि CV_SIMD_64F

template <>
संरचना InRange_SIMD<द्विगुण>
{
    पूर्णाङ्क operator () (ध्रुव द्विगुण * src1, ध्रुव द्विगुण * src2, ध्रुव द्विगुण * src3,
        uchar * dst, पूर्णाङ्क len) ध्रुव
    {
        पूर्णाङ्क x = 0;
        ध्रुव पूर्णाङ्क step = VTraits<v_float64>::vlanes();
        ध्रुव पूर्णाङ्क width = step * 4;

        प्रति (; x <= len - width; x += width)
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
        प्रत्यागम x;
    }
};

#endif

#endif

template <typename T>
स्थिर शून्य inRange_(ध्रुव शून्य* _src1, size_t step1, ध्रुव शून्य* _src2, size_t step2,
         ध्रुव शून्य* _src3, size_t step3, uchar* dst, size_t step,
         Size size)
{
    ध्रुव T* src1 = (ध्रुव T*)_src1;
    ध्रुव T* src2 = (ध्रुव T*)_src2;
    ध्रुव T* src3 = (ध्रुव T*)_src3;
    step1 /= परिमाण(src1[0]);
    step2 /= परिमाण(src2[0]);
    step3 /= परिमाण(src3[0]);

    InRange_SIMD<T> vop;

    प्रति( ; size.height--; src1 += step1, src2 += step2, src3 += step3, dst += step )
    {
        पूर्णाङ्क x = vop(src1, src2, src3, dst, size.width);
        #यदि CV_ENABLE_UNROLLED
        प्रति( ; x <= size.width - 4; x += 4 )
        {
            पूर्णाङ्क t0, t1;
            t0 = src2[x] <= src1[x] && src1[x] <= src3[x];
            t1 = src2[x+1] <= src1[x+1] && src1[x+1] <= src3[x+1];
            dst[x] = (uchar)-t0; dst[x+1] = (uchar)-t1;
            t0 = src2[x+2] <= src1[x+2] && src1[x+2] <= src3[x+2];
            t1 = src2[x+3] <= src1[x+3] && src1[x+3] <= src3[x+3];
            dst[x+2] = (uchar)-t0; dst[x+3] = (uchar)-t1;
        }
        #endif
        प्रति( ; x < size.width; x++ )
            dst[x] = (uchar)-(src2[x] <= src1[x] && src1[x] <= src3[x]);
    }
}

स्थिर शून्य inRangeReduce(ध्रुव uchar* src, uchar* dst, size_t len, पूर्णाङ्क cn)
{
    पूर्णाङ्क k = cn % 4 ? cn % 4 : 4;
    size_t i, j;
    यदि( k == 1 )
        प्रति( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j];
    अन्यथा यदि( k == 2 )
        प्रति( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1];
    अन्यथा यदि( k == 3 )
        प्रति( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2];
    अन्यथा
        प्रति( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2] & src[j+3];

    प्रति( ; k < cn; k += 4 )
    {
        प्रति( i = 0, j = k; i < len; i++, j += cn )
            dst[i] &= src[j] & src[j+1] & src[j+2] & src[j+3];
    }
}

प्रकारनिर्देश शून्य (*InRangeFunc)( ध्रुव शून्य* src1, size_t step1, ध्रुव शून्य* src2, size_t step2,
                             ध्रुव शून्य* src3, size_t step3, uchar* dst, size_t step, Size sz );

स्थिर InRangeFunc getInRangeFunc(पूर्णाङ्क depth)
{
    स्थिर InRangeFunc inRangeTab[CV_DEPTH_MAX] =
    {
        inRange_<uchar>,
        inRange_<schar>,
        inRange_<ushort>,
        inRange_<short>,
        inRange_<पूर्णाङ्क>,
        inRange_<भिन्न>,
        inRange_<द्विगुण>,
        inRange_<hfloat>,
        inRange_<bfloat>,
        0,
        inRange_<uint64_t>,
        inRange_<int64_t>,
        inRange_<unsigned>,
        0,
    };

    प्रत्यागम inRangeTab[depth];
}

#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_inRange( निवेश_Array _src, निवेश_Array _lowerb,
                         निवेश_Array _upperb, OutputArray _dst )
{
    ध्रुव ocl::Device & d = ocl::Device::getDefault();
    निवेश_Array_2::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Size ssize = _src.size(), lsize = _lowerb.size(), usize = _upperb.size();
    पूर्णाङ्क stype = _src.type(), ltype = _lowerb.type(), utype = _upperb.type();
    पूर्णाङ्क sdepth = CV_MAT_DEPTH(stype), ldepth = CV_MAT_DEPTH(ltype), udepth = CV_MAT_DEPTH(utype);
    पूर्णाङ्क cn = CV_MAT_CN(stype), rowsPerWI = d.isIntel() ? 4 : 1;
    तार्किक lbScalar = असत्य, ubScalar = असत्य;

    यदि( (lkind == निवेश_Array_2::MATX && skind != निवेश_Array_2::MATX) ||
        ssize != lsize || stype != ltype )
    {
        यदि( !checkScalar(_lowerb, stype, lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = सत्य;
    }

    यदि( (ukind == निवेश_Array_2::MATX && skind != निवेश_Array_2::MATX) ||
        ssize != usize || stype != utype )
    {
        यदि( !checkScalar(_upperb, stype, ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = सत्य;
    }

    यदि (lbScalar != ubScalar)
        प्रत्यागम असत्य;

    तार्किक doubleSupport = d.doubleFPConfig() > 0,
            haveScalar = lbScalar && ubScalar;

    यदि ( (!doubleSupport && sdepth == CV_64F) ||
         (!haveScalar && (sdepth != ldepth || sdepth != udepth)) )
        प्रत्यागम असत्य;

    पूर्णाङ्क kercn = haveScalar ? cn : std::max(std::min(ocl::predictOptimalVectorWidth(_src, _lowerb, _upperb, _dst), 4), cn);
    यदि (kercn % cn != 0)
        kercn = cn;
    पूर्णाङ्क colsPerWI = kercn / cn;
    String opts = format("%s-D CN=%d -D SRC_T=%s -D SRC_T1=%s -D DST_T=%s -D KERCN=%d -D DEPTH=%d%s -D COLS_PER_WI=%d",
                           haveScalar ? "-D HAVE_SCALAR " : "", cn, ocl::typeToStr(CV_MAKE_TYPE(sdepth, kercn)),
                           ocl::typeToStr(sdepth), ocl::typeToStr(CV_8UC(colsPerWI)), kercn, sdepth,
                           doubleSupport ? " -D DOUBLE_SUPPORT" : "", colsPerWI);

    ocl::Kernel ker("inrange", ocl::core::inrange_oclsrc, opts);
    यदि (ker.empty())
        प्रत्यागम असत्य;

    _dst.create(ssize, CV_8UC1);
    UMat src = _src.getUMat(), dst = _dst.getUMat(), lscalaru, uscalaru;
    Mat lscalar, uscalar;

    यदि (lbScalar && ubScalar)
    {
        lscalar = _lowerb.getMat();
        uscalar = _upperb.getMat();

        size_t esz = src.elemSize();
        size_t blocksize = 36;

        AutoBuffer<uchar> _buf(blocksize*(((पूर्णाङ्क)lbScalar + (पूर्णाङ्क)ubScalar)*esz + cn) + 2*cn*परिमाण(पूर्णाङ्क) + 128);
        uchar *buf = alignPtr(_buf.data() + blocksize*cn, 16);

        यदि( ldepth != sdepth && sdepth < CV_32S )
        {
            पूर्णाङ्क* ilbuf = (पूर्णाङ्क*)alignPtr(buf + blocksize*esz, 16);
            पूर्णाङ्क* iubuf = ilbuf + cn;

            द्विआधारी_Func sccvtfunc = getConvertFunc(ldepth, CV_32S);
            sccvtfunc(lscalar.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, Size(cn, 1), 0);
            sccvtfunc(uscalar.ptr(), 1, 0, 1, (uchar*)iubuf, 1, Size(cn, 1), 0);
            पूर्णाङ्क minval = cvRound(getMinVal(sdepth)), maxval = cvRound(getMaxVal(sdepth));

            प्रति( पूर्णाङ्क k = 0; k < cn; k++ )
            {
                यदि( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lscalar = Mat(cn, 1, CV_32S, ilbuf);
            uscalar = Mat(cn, 1, CV_32S, iubuf);
        }

        lscalar.convertTo(lscalar, stype);
        uscalar.convertTo(uscalar, stype);
    }
    अन्यथा
    {
        lscalaru = _lowerb.getUMat();
        uscalaru = _upperb.getUMat();
    }

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, 1, colsPerWI);

    यदि (haveScalar)
    {
        lscalar.copyTo(lscalaru);
        uscalar.copyTo(uscalaru);

        ker.args(srcarg, dstarg, ocl::KernelArg::PtrReadOnly(lscalaru),
               ocl::KernelArg::PtrReadOnly(uscalaru), rowsPerWI);
    }
    अन्यथा
        ker.args(srcarg, dstarg, ocl::KernelArg::ReadOnlyNoSize(lscalaru),
               ocl::KernelArg::ReadOnlyNoSize(uscalaru), rowsPerWI);

    size_t globalsize[2] = { (size_t)ssize.width / colsPerWI, ((size_t)ssize.height + rowsPerWI - 1) / rowsPerWI };
    प्रत्यागम ker.run(2, globalsize, रिक्त, असत्य);
}

#endif

}

शून्य cv::inRange(निवेश_Array _src, निवेश_Array _lowerb,
                 निवेश_Array _upperb, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(! _src.empty());
    यदि (!getInRangeFunc(_src.depth()))
        CV_Error( cv::Error::StsNotImplemented,
                 "inRange: CV_Bool and CV_8F inputs are not supported, convert the input to another type first");

    CV_OCL_RUN(_src.dims() <= 2 && _lowerb.dims() <= 2 &&
               _upperb.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat()),
               ocl_inRange(_src, _lowerb, _upperb, _dst))

    निवेश_Array_2::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Mat src = _src.getMat(), lb = _lowerb.getMat(), ub = _upperb.getMat();

    तार्किक lbScalar = असत्य, ubScalar = असत्य;

    यदि( (lkind == निवेश_Array_2::MATX && skind != निवेश_Array_2::MATX) ||
        src.size != lb.size || src.type() != lb.type() )
    {
        यदि( !checkScalar(lb, src.type(), lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = सत्य;
    }

    यदि( (ukind == निवेश_Array_2::MATX && skind != निवेश_Array_2::MATX) ||
        src.size != ub.size || src.type() != ub.type() )
    {
        यदि( !checkScalar(ub, src.type(), ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = सत्य;
    }

    CV_Assert(lbScalar == ubScalar);

    पूर्णाङ्क cn = src.channels(), depth = src.depth();

    size_t esz = src.elemSize();
    size_t blocksize0 = (size_t)(BLOCK_SIZE + esz-1)/esz;

    _dst.createSameSize(_src, CV_8UC1);
    Mat dst = _dst.getMat();
    InRangeFunc func = getInRangeFunc(depth);

    ध्रुव Mat* arrays_sc[] = { &src, &dst, 0 };
    ध्रुव Mat* arrays_nosc[] = { &src, &dst, &lb, &ub, 0 };
    uchar* ptrs[4] = {};

    NAryMatIterator it(lbScalar && ubScalar ? arrays_sc : arrays_nosc, ptrs);
    size_t total = it.size, blocksize = std::min(total, blocksize0);

    AutoBuffer<uchar> _buf(blocksize*(((पूर्णाङ्क)lbScalar + (पूर्णाङ्क)ubScalar)*esz + cn) + 2*cn*परिमाण(पूर्णाङ्क) + 128);
    uchar *buf = _buf.data(), *mbuf = buf, *lbuf = 0, *ubuf = 0;
    buf = alignPtr(buf + blocksize*cn, 16);

    यदि( lbScalar && ubScalar )
    {
        lbuf = buf;
        ubuf = buf = alignPtr(buf + blocksize*esz, 16);

        CV_Assert( lb.type() == ub.type() );
        पूर्णाङ्क scdepth = lb.depth();

        यदि( scdepth != depth && depth < CV_32S )
        {
            पूर्णाङ्क* ilbuf = (पूर्णाङ्क*)alignPtr(buf + blocksize*esz, 16);
            पूर्णाङ्क* iubuf = ilbuf + cn;

            द्विआधारी_Func sccvtfunc = getConvertFunc(scdepth, CV_32S);
            sccvtfunc(lb.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, Size(cn, 1), 0);
            sccvtfunc(ub.ptr(), 1, 0, 1, (uchar*)iubuf, 1, Size(cn, 1), 0);
            पूर्णाङ्क minval = cvRound(getMinVal(depth)), maxval = cvRound(getMaxVal(depth));

            प्रति( पूर्णाङ्क k = 0; k < cn; k++ )
            {
                यदि( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lb = Mat(cn, 1, CV_32S, ilbuf);
            ub = Mat(cn, 1, CV_32S, iubuf);
        }

        convertAndUnrollScalar( lb, src.type(), lbuf, blocksize );
        convertAndUnrollScalar( ub, src.type(), ubuf, blocksize );

        यदि (depth == CV_8U && src.dims <= 2) {
            uint8_t lb_scalar = lbuf[0];
            uint8_t ub_scalar = ubuf[0];
            CALL_HAL(inRange_u8, cv_hal_inRange8u, src.data, src.step, dst.data, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        } अन्यथा यदि (depth == CV_32F && src.dims <= 2) {
            द्विगुण lb_scalar = lb.ptr<द्विगुण>(0)[0];
            द्विगुण ub_scalar = ub.ptr<द्विगुण>(0)[0];
            CALL_HAL(inRange_f32, cv_hal_inRange32f, src.data, src.step, dst.data, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        }
    }

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        प्रति( size_t j = 0; j < total; j += blocksize )
        {
            पूर्णाङ्क bsz = (पूर्णाङ्क)MIN(total - j, blocksize);
            size_t delta = bsz*esz;
            uchar *lptr = lbuf, *uptr = ubuf;
            यदि( !lbScalar )
            {
                lptr = ptrs[2];
                ptrs[2] += delta;
            }
            यदि( !ubScalar )
            {
                पूर्णाङ्क idx = !lbScalar ? 3 : 2;
                uptr = ptrs[idx];
                ptrs[idx] += delta;
            }
            func( ptrs[0], 0, lptr, 0, uptr, 0, cn == 1 ? ptrs[1] : mbuf, 0, Size(bsz*cn, 1));
            यदि( cn > 1 )
                inRangeReduce(mbuf, ptrs[1], bsz, cn);
            ptrs[0] += delta;
            ptrs[1] += bsz;
        }
    }
}
/* End of file. */
