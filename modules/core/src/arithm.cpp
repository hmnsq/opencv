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

#समावेश "precomp.hpp"
#समावेश "arithm_expr.hpp"   // the new element-wise engine (cv::ew)
#समावेश "opencl_kernels_core.hpp"

namespace cv
{

/****************************************************************************************\
*                                   logical operations                                   *
\****************************************************************************************/

क्रमागत { OCL_OP_ADD=0, OCL_OP_SUB=1, OCL_OP_RSUB=2, OCL_OP_ABSDIFF=3, OCL_OP_MUL=4,
       OCL_OP_MUL_SCALE=5, OCL_OP_DIV_SCALE=6, OCL_OP_RECIP_SCALE=7, OCL_OP_ADDW=8,
       OCL_OP_AND=9, OCL_OP_OR=10, OCL_OP_XOR=11, OCL_OP_NOT=12, OCL_OP_MIN=13, OCL_OP_MAX=14,
       OCL_OP_RDIV_SCALE=15 };

// The unified entry point for every element-wise binary op (empty + UMat/OpenCL + CPU engine). Defined
// lower down (near cv::add); forward-declared here so cv::min/cv::max (above it) can use it too.
अटल व्योम arithm_op(ew::शीर्ष op, निवेश_Array src1, निवेश_Array src2, OutputArray dst,
                      निवेश_Array mask, पूर्णांक dtype, पूर्णांक oclop, bool muldiv, स्थिर Scalar& params = Scalar(1));

// The unary counterpart (bitwise NOT, unary math later). Forward-declared for the same reason.
अटल व्योम unary_op(ew::शीर्ष op, निवेश_Array src, OutputArray dst, निवेश_Array mask,
                     पूर्णांक dtype, स्थिर Scalar& params = Scalar());

#अगर_घोषित HAVE_OPENCL

अटल स्थिर अक्षर* oclop2str[] = { "OP_ADD", "OP_SUB", "OP_RSUB", "OP_ABSDIFF",
    "OP_MUL", "OP_MUL_SCALE", "OP_DIV_SCALE", "OP_RECIP_SCALE",
    "OP_ADDW", "OP_AND", "OP_OR", "OP_XOR", "OP_NOT", "OP_MIN", "OP_MAX", "OP_RDIV_SCALE", 0 };

// depths with no OpenCL type name (ocl::typeToStr() asserts on them)
अटल अंतरभूत bool noOclType(पूर्णांक depth)
{
    वापस depth == CV_16BF || depth == CV_Bool || depth == CV_8F_E4M3FN || depth == CV_8F_E4M3FNUZ;
}

अटल bool ocl_द्विआधारी_op(निवेश_Array _src1, निवेश_Array _src2, OutputArray _dst,
                          निवेश_Array _mask, bool bitwise, पूर्णांक oclop, bool haveScalar )
{
    bool haveMask = !_mask.empty();
    पूर्णांक srctype = _src1.type();
    पूर्णांक srcdepth = CV_MAT_DEPTH(srctype);
    पूर्णांक cn = CV_MAT_CN(srctype);

    स्थिर ocl::Device d = ocl::Device::getDefault();
    bool doubleSupport = d.doubleFPConfig() > 0;
    अगर( oclop < 0 || ((haveMask || haveScalar) && cn > 4) ||
            (!doubleSupport && srcdepth == CV_64F && !bitwise) ||
            (noOclType(srcdepth) && !bitwise))
        वापस false;

    अक्षर opts[1024];
    पूर्णांक kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    पूर्णांक scalarcn = kercn == 3 ? 4 : kercn;
    पूर्णांक rowsPerWI = d.isIntel() ? 4 : 1;

    स्थिर पूर्णांक dstDepth = srcdepth;
    स्थिर पूर्णांक dstType = CV_MAKETYPE(dstDepth, kercn);
    स्थिर पूर्णांक dstType1 = CV_MAKETYPE(dstDepth, 1);
    स्थिर पूर्णांक scalarType = CV_MAKETYPE(srcdepth, scalarcn);

    snprintf(opts, माप(opts), "-D %s%s -D %s%s -D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workST=%s -D cn=%d -D rowsPerWI=%d",
            haveMask ? "MASK_" : "", haveScalar ? "UNARY_OP" : "BINARY_OP", oclop2str[oclop],
            doubleSupport ? " -D DOUBLE_SUPPORT" : "",
            bitwise ? ocl::memopTypeToStr(dstType) : ocl::typeToStr(dstType),
            dstDepth,
            bitwise ? ocl::memopTypeToStr(dstType1) : ocl::typeToStr(dstType1),
            bitwise ? ocl::memopTypeToStr(scalarType) : ocl::typeToStr(scalarType),
            kercn, rowsPerWI);

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    अगर (k.empty())
        वापस false;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_आकार(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_आकार(mask, 1);

    अगर( haveScalar )
    {
        माप_प्रकार esz = CV_ELEM_SIZE1(srctype)*scalarcn;
        द्विगुन buf[4] = {0,0,0,0};

        अगर( oclop != OCL_OP_NOT )
        {
            Mat src2sc = _src2.getMat();
            convertAndUnrollScalar(src2sc, srctype, (uchar*)buf, 1);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        अगर( !haveMask )
            k.args(src1arg, dstarg, scalararg);
        अन्यथा
            k.args(src1arg, maskarg, dstarg, scalararg);
    }
    अन्यथा
    {
        src2 = _src2.getUMat();
        ocl::KernelArg src2arg = ocl::KernelArg::Read_Only_No_आकार(src2, cn, kercn);

        अगर( !haveMask )
            k.args(src1arg, src2arg, dstarg);
        अन्यथा
            k.args(src1arg, src2arg, maskarg, dstarg);
    }

    माप_प्रकार globalsize[] = { (माप_प्रकार)src1.cols * cn / kercn, ((माप_प्रकार)src1.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, 0, false);
}

#पूर्ण_अगर

// OpenCL path for the bitwise ops (and/or/xor/not), lifted out of binary_op: choose the (possibly
// scalar) operand order, size the destination, and dispatch the shared bitwise OpenCL kernel. Returns
// true if OpenCL handled the op, false to fall through to the CPU engine. NOT arrives unary
// (src2 == src1, forced-scalar). The caller gates this on a UMat operand + dims <= 2 (via CV_OCL_RUN).
#अगर_घोषित HAVE_OPENCL
अटल bool bitwise_op_ocl(निवेश_Array _src1, निवेश_Array _src2, OutputArray _dst,
                           निवेश_Array _mask, पूर्णांक oclop)
{
    स्थिर निवेश_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    निवेश_Array_2::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    पूर्णांक type1 = psrc1->type(), type2 = psrc2->type();
    bool haveScalar = false;

    अगर( oclop == OCL_OP_NOT )
        haveScalar = true;
    अन्यथा अगर( (kind1 == निवेश_Array_2::MATX) + (kind2 == निवेश_Array_2::MATX) == 1 ||
             !psrc1->same_आकार(*psrc2) || type1 != type2 )
    {
        अगर( checkScalar(*psrc1, type2, kind1, kind2) )
        { std::swap(psrc1, psrc2); std::swap(type1, type2); }        // src1 is the scalar; swap it out
        अन्यथा अगर( !checkScalar(*psrc2, type1, kind2, kind1) )
            वापस false;
        haveScalar = true;
    }
    _dst.create_Same_आकार(*psrc1, type1);
    वापस ocl_द्विआधारी_op(*psrc1, *psrc2, _dst, _mask, true, oclop, haveScalar);
}
#पूर्ण_अगर  // HAVE_OPENCL

}

// bitwise and/or/xor: OpenCL runs the shared bitwise kernel (via bitwise_op_ocl); the CPU path is the
// element-wise engine (OP_AND/OR/XOR, T x T -> T dispatched by element size). oclop = -1 tells
// arithm_op the OpenCL path was already tried, so it only runs the CPU engine. NOT is unary.
व्योम cv::bitwise_and(निवेश_Array a, निवेश_Array b, OutputArray c, निवेश_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_AND))
    arithm_op(ew::OP_AND, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/false);
}

व्योम cv::bitwise_or(निवेश_Array a, निवेश_Array b, OutputArray c, निवेश_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_OR))
    arithm_op(ew::OP_OR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/false);
}

व्योम cv::bitwise_xor(निवेश_Array a, निवेश_Array b, OutputArray c, निवेश_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || b.isUMat() || c.isUMat()) && a.dims() <= 2 && b.dims() <= 2,
               bitwise_op_ocl(a, b, c, mask, OCL_OP_XOR))
    arithm_op(ew::OP_XOR, a, b, c, mask, -1, /*oclop=*/-1, /*muldiv=*/false);
}

व्योम cv::bitwise_not(निवेश_Array a, OutputArray c, निवेश_Array mask)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN((a.isUMat() || c.isUMat()) && a.dims() <= 2,
               bitwise_op_ocl(a, a, c, mask, OCL_OP_NOT))
    unary_op(ew::OP_NOT, a, c, mask, -1);
}

व्योम cv::max( निवेश_Array src1, निवेश_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, false);
}

व्योम cv::min( निवेश_Array src1, निवेश_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, false);
}

// The concrete Mat/UMat overloads of min/max exist because C++ overload resolution needs them (a
// bare cv::min(mat, mat, mat) binds Mat& more tightly than InputArray); they must run the SAME engine
// path as the InputArray forms - NOT the legacy binary_op - so mixed types and broadcasting work.
व्योम cv::max(स्थिर Mat& src1, स्थिर Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, false);
}

व्योम cv::min(स्थिर Mat& src1, स्थिर Mat& src2, Mat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, false);
}

व्योम cv::max(स्थिर UMat& src1, स्थिर UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MAX, src1, src2, dst, noArray(), -1, OCL_OP_MAX, false);
}

व्योम cv::min(स्थिर UMat& src1, स्थिर UMat& src2, UMat& dst)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_MIN, src1, src2, dst, noArray(), -1, OCL_OP_MIN, false);
}


/****************************************************************************************\
*                                      add/subtract                                      *
\****************************************************************************************/

namespace cv
{

#अगर_घोषित HAVE_OPENCL

// used by the OpenCL branch only (arithm_op_ocl working-type selection)
अटल पूर्णांक actualScalarDepth(स्थिर द्विगुन* तथ्य, पूर्णांक len)
{
    पूर्णांक i = 0, minval = पूर्णांक_उच्च, maxval = पूर्णांक_न्यून;
    क्रम(; i < len; ++i)
    {
        पूर्णांक ival = cvRound(तथ्य[i]);
        अगर( ival != तथ्य[i] )
            अवरोध;
        minval = MIN(minval, ival);
        maxval = MAX(maxval, ival);
    }
    वापस i < len ? CV_64F :
        minval >= 0 && maxval <= (पूर्णांक)अच_अक्षर_उच्च ? CV_8U :
        minval >= (पूर्णांक)च_अक्षर_न्यून && maxval <= (पूर्णांक)च_अक्षर_उच्च ? CV_8S :
        minval >= 0 && maxval <= (पूर्णांक)अच_लघु_उच्च ? CV_16U :
        minval >= (पूर्णांक)लघु_न्यून && maxval <= (पूर्णांक)लघु_उच्च ? CV_16S :
        CV_32S;
}

अटल पूर्णांक coerceTypes(पूर्णांक depth1, पूर्णांक depth2, bool muldiv)
{
    वापस depth1 == depth2 ? depth1 :
        ((depth1 <= CV_32S) & (depth2 <= CV_32S)) != 0 ?
        (((पूर्णांक)!muldiv & (depth1 <= CV_8S) & (depth2 <= CV_8S)) != 0 ? CV_16S : CV_32S) :
        ((CV_ELEM_SIZE1(depth1) > 4) | (CV_ELEM_SIZE1(depth2) > 4)) != 0 ? CV_64F : CV_32F;
}

अटल bool ocl_arithm_op(निवेश_Array _src1, निवेश_Array _src2, OutputArray _dst,
                          निवेश_Array _mask, पूर्णांक wtype,
                          व्योम* usrdata, पूर्णांक oclop,
                          bool haveScalar )
{
    स्थिर ocl::Device d = ocl::Device::getDefault();
    bool doubleSupport = d.doubleFPConfig() > 0;
    पूर्णांक type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    bool haveMask = !_mask.empty();

    अगर ( (haveMask || haveScalar) && cn > 4 )
        वापस false;

#अगर_घोषित __APPLE__
    अगर (depth1 == CV_16U && (oclop == OCL_OP_MUL || oclop == OCL_OP_MUL_SCALE))
        वापस false;
#पूर्ण_अगर

    पूर्णांक dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), wdepth = std::max(CV_32S, CV_MAT_DEPTH(wtype));
    अगर (!doubleSupport)
        wdepth = std::min(wdepth, CV_32F);

    wtype = CV_MAKETYPE(wdepth, cn);
    पूर्णांक type2 = haveScalar ? wtype : _src2.type(), depth2 = CV_MAT_DEPTH(type2);
    अगर (!doubleSupport && (depth2 == CV_64F || depth1 == CV_64F))
        वापस false;

    अगर (noOclType(depth1) || noOclType(depth2) || noOclType(ddepth) || noOclType(wdepth))
        वापस false;

    पूर्णांक kercn = haveMask || haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);
    पूर्णांक scalarcn = kercn == 3 ? 4 : kercn, rowsPerWI = d.isIntel() ? 4 : 1;

    अक्षर cvtstr[4][50], opts[1024];
    snprintf(opts, माप(opts), "-D %s%s -D %s -D srcT1=%s -D srcT1_C1=%s -D srcT2=%s -D srcT2_C1=%s "
            "-D dstT=%s -D DEPTH_dst=%d -D dstT_C1=%s -D workT=%s -D workST=%s -D scaleT=%s -D wdepth=%d -D convertToWT1=%s "
            "-D convertToWT2=%s -D convertToDT=%s%s -D cn=%d -D rowsPerWI=%d -D convertFromU=%s",
            (haveMask ? "MASK_" : ""), (haveScalar ? "UNARY_OP" : "BINARY_OP"),
            oclop2str[oclop], ocl::typeToStr(CV_MAKETYPE(depth1, kercn)),
            ocl::typeToStr(depth1), ocl::typeToStr(CV_MAKETYPE(depth2, kercn)),
            ocl::typeToStr(depth2), ocl::typeToStr(CV_MAKETYPE(ddepth, kercn)), ddepth,
            ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKETYPE(wdepth, kercn)),
            ocl::typeToStr(CV_MAKETYPE(wdepth, scalarcn)),
            ocl::typeToStr(wdepth), wdepth,
            ocl::convertTypeStr(depth1, wdepth, kercn, cvtstr[0], माप(cvtstr[0])),
            ocl::convertTypeStr(depth2, wdepth, kercn, cvtstr[1], माप(cvtstr[1])),
            ocl::convertTypeStr(wdepth, ddepth, kercn, cvtstr[2], माप(cvtstr[2])),
            doubleSupport ? " -D DOUBLE_SUPPORT" : "", kercn, rowsPerWI,
            oclop == OCL_OP_ABSDIFF && wdepth == CV_32S && ddepth == wdepth ?
            ocl::convertTypeStr(CV_8U, ddepth, kercn, cvtstr[3], माप(cvtstr[3])) : "noconvert");

    माप_प्रकार usrdata_esz = CV_ELEM_आकार(wdepth);
    स्थिर uchar* usrdata_p = (स्थिर uchar*)usrdata;
    स्थिर द्विगुन* usrdata_d = (स्थिर द्विगुन*)usrdata;
    भग्न usrdata_f[3];
    पूर्णांक i, n = oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE ||
        oclop == OCL_OP_RDIV_SCALE || oclop == OCL_OP_RECIP_SCALE ? 1 : oclop == OCL_OP_ADDW ? 3 : 0;
    अगर( usrdata && n > 0 && wdepth == CV_32F )
    {
        क्रम( i = 0; i < n; i++ )
            usrdata_f[i] = (भग्न)usrdata_d[i];
        usrdata_p = (स्थिर uchar*)usrdata_f;
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    अगर (k.empty())
        वापस false;

    UMat src1 = _src1.getUMat(), src2;
    UMat dst = _dst.getUMat(), mask = _mask.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_आकार(src1, cn, kercn);
    ocl::KernelArg dstarg = haveMask ? ocl::KernelArg::ReadWrite(dst, cn, kercn) :
                                       ocl::KernelArg::WriteOnly(dst, cn, kercn);
    ocl::KernelArg maskarg = ocl::KernelArg::Read_Only_No_आकार(mask, 1);

    अगर( haveScalar )
    {
        माप_प्रकार esz = CV_ELEM_SIZE1(wtype)*scalarcn;
        द्विगुन buf[4]={0,0,0,0};
        Mat src2sc = _src2.getMat();

        अगर( !src2sc.empty() )
            convertAndUnrollScalar(src2sc, wtype, (uchar*)buf, 1);
        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        अगर( !haveMask )
        {
            अगर(n == 0)
                k.args(src1arg, dstarg, scalararg);
            अन्यथा अगर(n == 1)
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
        ocl::KernelArg src2arg = ocl::KernelArg::Read_Only_No_आकार(src2, cn, kercn);

        अगर( !haveMask )
        {
            अगर (n == 0)
                k.args(src1arg, src2arg, dstarg);
            अन्यथा अगर (n == 1)
                k.args(src1arg, src2arg, dstarg,
                       ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, usrdata_p, usrdata_esz));
            अन्यथा अगर (n == 3)
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

    माप_प्रकार globalsize[] = { (माप_प्रकार)src1.cols * cn / kercn, ((माप_प्रकार)src1.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

#पूर्ण_अगर

// OpenCL branch of the arithmetic ops, extracted from the former arithm_op (whose CPU compute is now
// the element-wise engine). Returns true if an OpenCL kernel handled the op, false to let the caller
// fall through to the CPU engine. min/max use the bitwise-style kernel; every other op the arithm
// kernel with a coerced working type. `usrdata` carries the mul/div/addWeighted scale block, `oclop`
// the kernel id. Only reached for a UMat dst; sizes/types that don't fit the scalar/same-size pattern
// (e.g. a broadcast) decline (return false) so the engine handles them on the CPU.
अटल bool arithm_op_ocl(निवेश_Array _src1, निवेश_Array _src2, OutputArray _dst,
                          निवेश_Array _mask, पूर्णांक dtype, पूर्णांक oclop, bool muldiv, व्योम* usrdata)
{
#अगर_अघोषित HAVE_OPENCL
    CV_UNUSED(_src1); CV_UNUSED(_src2); CV_UNUSED(_dst); CV_UNUSED(_mask);
    CV_UNUSED(dtype); CV_UNUSED(oclop); CV_UNUSED(muldiv); CV_UNUSED(usrdata);
    वापस false;
#अन्यथा
    स्थिर निवेश_Array_2 *psrc1 = &_src1, *psrc2 = &_src2;
    निवेश_Array_2::KindFlag kind1 = psrc1->kind(), kind2 = psrc2->kind();
    bool haveMask = !_mask.empty();
    bool reallocate = false;
    पूर्णांक type1 = psrc1->type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1);
    पूर्णांक type2 = psrc2->type(), depth2 = CV_MAT_DEPTH(type2), cn2 = CV_MAT_CN(type2);
    पूर्णांक wtype, dims1 = psrc1->dims(), dims2 = psrc2->dims();
    आकार sz1 = dims1 <= 2 ? psrc1->size() : आकार();
    आकार sz2 = dims2 <= 2 ? psrc2->size() : आकार();

    अगर (!(OCL_PERFORMANCE_CHECK(_dst.isUMat()) && dims1 <= 2 && dims2 <= 2))
        वापस false;

    // min/max: the bitwise-style OpenCL kernel (same-type element-wise, no working-type coercion).
    अगर (oclop == OCL_OP_MIN || oclop == OCL_OP_MAX)
    {
        bool haveScalar = false;
        अगर ((kind1 == निवेश_Array_2::MATX) + (kind2 == निवेश_Array_2::MATX) == 1 ||
            !psrc1->same_आकार(*psrc2) || type1 != type2)
        {
            अगर (checkScalar(*psrc1, type2, kind1, kind2))
            { std::swap(psrc1, psrc2); std::swap(type1, type2); }
            अन्यथा अगर (!checkScalar(*psrc2, type1, kind2, kind1))
                वापस false;
            haveScalar = true;
        }
        _dst.create_Same_आकार(*psrc1, type1);
        वापस ocl_द्विआधारी_op(*psrc1, *psrc2, _dst, _mask, false, oclop, haveScalar);
    }

    // reciprocal (scale/src): the OpenCL kernel is unary on src - use src2 for both operands (the CPU
    // engine received a 0-dim `1` numerator as src1, which the recip kernel would ignore). [TODO.VP: tidy]
    अगर (oclop == OCL_OP_RECIP_SCALE)
    {
        psrc1 = psrc2; kind1 = kind2; type1 = type2; depth1 = depth2; cn = cn2; dims1 = dims2; sz1 = sz2;
    }

    // add/sub/mul/div/absdiff/addWeighted/recip: the arithm OpenCL kernel with a coerced work type.
    bool src1Scalar = checkScalar(*psrc1, type2, kind1, kind2);
    bool src2Scalar = checkScalar(*psrc2, type1, kind2, kind1);
    bool haveScalar = false;

    अगर (dims1 != dims2 || sz1 != sz2 || cn != cn2 ||
        (kind1 == निवेश_Array_2::MATX && (sz1 == आकार(1,4) || sz1 == आकार(1,1))) ||
        (kind2 == निवेश_Array_2::MATX && (sz2 == आकार(1,4) || sz2 == आकार(1,1))))
    {
        अगर ((type1 == CV_64F && (sz1.height == 1 || sz1.height == 4)) && src1Scalar)
        {
            // src1 is a scalar; swap it with src2
            std::swap(psrc1, psrc2); std::swap(sz1, sz2); std::swap(type1, type2);
            std::swap(depth1, depth2); std::swap(cn, cn2); std::swap(dims1, dims2);
            अगर (oclop == OCL_OP_SUB) oclop = OCL_OP_RSUB;
            अगर (oclop == OCL_OP_DIV_SCALE) oclop = OCL_OP_RDIV_SCALE;
        }
        अन्यथा अगर (!src2Scalar)
            वापस false;   // array op array with mismatched size/cn: engine broadcasts on the CPU
        haveScalar = true;
        CV_Assert((type2 == CV_64F || type2 == CV_32F) && (sz2.height == 1 || sz2.height == 4));

        अगर (!muldiv)
        {
            Mat sc = psrc2->getMat();
            depth2 = actualScalarDepth(sc.ptr<द्विगुन>(), sz2 == आकार(1, 1) ? cn2 : cn);
            अगर (depth2 == CV_64F && CV_ELEM_SIZE1(depth1) < 8)
                depth2 = CV_32F;
        }
        अन्यथा
            depth2 = CV_64F;
    }

    अगर (dtype < 0)
    {
        अगर (_dst.fixedType())
            dtype = _dst.type();
        अन्यथा
        {
            अगर (!haveScalar && type1 != type2)
                CV_Error(cv::Error::StsBadArg,
                     "When the input arrays in add/subtract/multiply/divide functions have different types, "
                     "the output array type must be explicitly specified");
            dtype = type1;
        }
    }
    dtype = CV_MAT_DEPTH(dtype);

    अगर (depth1 == depth2 && dtype == depth1)
        wtype = dtype;
    अन्यथा अगर (!muldiv)
    {
        wtype = coerceTypes(depth1, depth2, false);
        wtype = coerceTypes(wtype, dtype, false);
        अगर (dtype < CV_32F && (depth1 < CV_32F || depth2 < CV_32F))
            wtype = CV_32S;
    }
    अन्यथा
    {
        wtype = coerceTypes(depth1, depth2, true);
        wtype = coerceTypes(wtype, dtype, true);
    }

    // The scaled OpenCL kernels (mul/div by a scale, reciprocal, addWeighted) compute in float; the old
    // same-type fast path forced max(depth, CV_32F). coerceTypes keeps same-type integer, so bump here.
    अगर (oclop == OCL_OP_MUL_SCALE || oclop == OCL_OP_DIV_SCALE || oclop == OCL_OP_RDIV_SCALE ||
        oclop == OCL_OP_RECIP_SCALE || oclop == OCL_OP_ADDW)
        wtype = std::max(wtype, (पूर्णांक)CV_32F);

    dtype = CV_MAKETYPE(dtype, cn);
    wtype = CV_MAKETYPE(wtype, cn);

    अगर (haveMask)
    {
        पूर्णांक mtype = _mask.type();
        CV_Assert((mtype == CV_8UC1 || mtype == CV_8SC1 || mtype == CV_Bool) && _mask.same_आकार(*psrc1));
        reallocate = !_dst.same_आकार(*psrc1) || _dst.type() != dtype;
    }

    _dst.create_Same_आकार(*psrc1, dtype);
    अगर (reallocate)
        _dst.setTo(0.);

    वापस ocl_arithm_op(*psrc1, *psrc2, _dst, _mask, wtype, usrdata, oclop, haveScalar);
#पूर्ण_अगर
}

// Element-wise binary op (ADD/SUB/...) on two CPU operands - each an array Mat or a MATX scalar -
// through the new broadcasting engine. Handles mixed input types (emitBinary inserts the casts) and
// broadcasting (a scalar rides as a 0-dim per-channel CONST; arrays broadcast via outputShape/exec).
// Returns false (declining) for UMat, a write-mask, or two scalar operands, so the caller falls
// through to arithm_op. Incompatible shapes make the engine throw (matching cv::add's error).
// Does the output array already have exactly this shape+type (=> create() reuses it, no realloc)?
// Works for Mat and UMat alike (shape/type queries, no data-pointer peeking).
अटल bool outArrayMatches(स्थिर _OutputArray& a, स्थिर MatShape& shp, पूर्णांक type)
{
    अगर (a.empty() || a.type() != type)
        वापस false;
    पूर्णांक sz[MatShape::MAX_DIMS];
    पूर्णांक nd = a.sizend(sz);
    अगर (nd != (पूर्णांक)shp.size())
        वापस false;
    क्रम (पूर्णांक i = 0; i < nd; i++)
        अगर (sz[i] != shp[i])
            वापस false;
    वापस true;
}

// Are two operands' iteration shapes (spatial dims + channels innermost, exactly how the engine
// iterates) numpy-broadcast-compatible? Powers the scalar-like-Mat compat fallback in arithm_op.
// Cheap by design: sizend copies the dim arrays inline - no Mat headers, no allocations.
अटल bool shapesBroadcastCompat(निवेश_Array a, पूर्णांक acn, निवेश_Array b, पूर्णांक bcn)
{
    पूर्णांक asz[MatShape::MAX_DIMS + 1], bsz[MatShape::MAX_DIMS + 1];
    पूर्णांक ad = a.sizend(asz), bd = b.sizend(bsz);
    asz[ad++] = acn;
    bsz[bd++] = bcn;
    क्रम (पूर्णांक i1 = ad - 1, i2 = bd - 1; i1 >= 0 && i2 >= 0; i1--, i2--)
        अगर (asz[i1] != bsz[i2] && asz[i1] != 1 && bsz[i2] != 1)
            वापस false;
    वापस true;
}

// The one entry point for every element-wise binary op (add/sub/mul/div/min/max/absdiff/addWeighted/
// reciprocal). Handles empty inputs, the UMat/OpenCL branch (arithm_op_ocl) and the CPU element-wise
// engine. `oclop` selects the OpenCL kernel; `muldiv` drives the OpenCL working-type coercion + scale;
// `params` are the op scalars (params[0]=mul/div scale; {alpha,beta,gamma} for addWeighted).
// Below this many ELEMENTS (total * channels), the per-call constant overhead of the engine path
// (program build + compile + executor setup) is comparable to the actual work - take the direct
// kernel call instead. Same tier boundary as math_op's MATH_OP_SMALL for cv::exp.
क्रमागत { ARITHM_SMALL_DIRECT = 100000 };

अटल व्योम arithm_op(ew::शीर्ष op, निवेश_Array src1, निवेश_Array src2, OutputArray dst,
                      निवेश_Array mask, पूर्णांक dtype, पूर्णांक oclop, bool muldiv, स्थिर Scalar& params)
{
    अगर (src1.empty() || src2.empty())          // empty inputs -> empty result
    {
        CV_Assert((src1.empty() && src2.empty()) ||
                  (src1.empty() && isScalarArg(src2, src1.channels())) ||
                  (src2.empty() && isScalarArg(src1, src2.channels())));
        dst.release();
        अगर (dtype >= 0)
            dst.create(0, 0, dtype);
        वापस;
    }

    // UMat -> classic OpenCL kernel. It declines (false) when OpenCL can't apply (no device, dims>2, a
    // broadcast, cn>4 masked, ...) - then the CPU engine below handles it (mapping the UMat via getMat).
    // oclop < 0 is a sentinel: the caller (e.g. cv::compare) already ran its own OpenCL path, so this
    // helper only runs the CPU engine (a UMat operand is mapped via getMat).
    अगर (oclop >= 0 && cv::ocl::isOpenCLActivated() &&
        (src1.isUMat() || src2.isUMat() || dst.isUMat()))
    {
        द्विगुन abg[3] = { params[0], params[1], params[2] }, scale = params[0];
        व्योम* usrdata = (op == ew::OP_ADDW) ? (व्योम*)abg : (muldiv ? (व्योम*)&scale : nullptr);
        अगर (arithm_op_ocl(src1, src2, dst, mask, dtype, oclop, muldiv, usrdata))
            वापस;
    }

    स्थिर bool haveMask = !mask.empty();
    स्थिर पूर्णांक cn1 = src1.channels(), cn2 = src2.channels();
    bool s1 = isScalarArg(src1, src2), s2 = isScalarArg(src2, src1);
    अगर (s1 && s2)
        s1 = s2 = false;    // two scalars: treat both as tiny array operands (element-wise + broadcast),
                            // matching arithm_op (Scalar+Scalar -> 4x1, number+number -> 1x1, ...)
    // A 4x1 CV_64F Mat/UMat pseudo-scalar (see isScalarArg) is hijacked only against a REAL array:
    // when the partner is itself a tiny scalar-shaped array (e.g. compare(Mat 4x1, Mat 1x1) -
    // issue #8999), both are honest data and ride the broadcast. A genuine MATX Scalar never demotes.
    अन्यथा अगर (s1 && src1.kind() != निवेश_Array_2::MATX && isScalarLikeMat(src2, cn1))
        s1 = false;
    अन्यथा अगर (s2 && src2.kind() != निवेश_Array_2::MATX && isScalarLikeMat(src1, cn2))
        s2 = false;

    // Compat fallback: Java/Python/user code passes scalars as real little Mats (the classic 4x1
    // CV_64F column, a 1xcn/cnx1 vector - see isScalarLikeMat). Those normally ride broadcasting
    // now, so hijack one as a per-channel scalar ONLY when the shapes do NOT broadcast - just the
    // calls that would otherwise throw get the 4.x scalar semantics, every valid broadcast keeps
    // its numpy meaning. Cheap: two geometry probes, and the O(ndims) shape walk runs only when a
    // probe hits (never for ordinary same-size or MATX-scalar calls).
    अगर (!s1 && !s2)
    {
        bool like1 = isScalarLikeMat(src1, cn2), like2 = isScalarLikeMat(src2, cn1);
        अगर ((like1 || like2) && !shapesBroadcastCompat(src1, cn1, src2, cn2))
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
    स्थिर व्योम* dstObj = (dst.kind() == निवेश_Array_2::MAT) ? dst.getObj() : nullptr;
    स्थिर bool aliased = dstObj && (dstObj == src1.getObj() || dstObj == src2.getObj());
    Mat m1loc, m2loc, mloc;
    स्वतः asMat = [&](निवेश_Array a, Mat& loc) -> स्थिर Mat* {
        अगर (a.kind() == निवेश_Array_2::MAT && !aliased) वापस (स्थिर Mat*)a.getObj();
        loc = a.getMat(); वापस &loc;
    };
    स्थिर Mat* pm1 = s1 ? nullptr : asMat(src1, m1loc);
    स्थिर Mat* pm2 = s2 ? nullptr : asMat(src2, m2loc);
    स्थिर पूर्णांक adepth = s1 ? pm2->depth() : pm1->depth();   // the (first) array operand's depth
    // auto result depth (no explicit dtype, no fixed-type dst): add/sub/mul/div keep the first array
    // operand's depth (cv::'s dtype==-1 convention), but min/max/absdiff have NO dtype argument and
    // must promote mixed inputs to their common type (min(u32,f32) in u32 would drop the float). For
    // same-type inputs promoteArith(da,db)==da, so classic behaviour is preserved (e.g. absdiff of two
    // s16 stays s16, saturating - the wide unsigned |a-b| is computed then cast back down by emitBinary).
    पूर्णांक autoDepth = adepth;
    अगर (op == ew::OP_MIN || op == ew::OP_MAX || op == ew::OP_ABSDIFF)
    {
        स्थिर पूर्णांक d1 = s1 ? ew::EW_DEPTH_NONE : pm1->depth();
        स्थिर पूर्णांक d2 = s2 ? ew::EW_DEPTH_NONE : pm2->depth();
        autoDepth = ew::promoteArith(d1, d2);
    }
    // result depth: explicit dtype wins; else a fixed-type dst dictates it; else the auto depth above.
    स्थिर पूर्णांक rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
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
    अगर (!haveMask && !s1 && !s2 &&
        (op == ew::OP_ADD || op == ew::OP_SUB || op == ew::OP_MIN || op == ew::OP_MAX ||
         op == ew::OP_ABSDIFF || op == ew::OP_MUL || op == ew::OP_ADDW ||
         op == ew::OP_AND || op == ew::OP_OR || op == ew::OP_XOR) &&
        rdepth == pm1->depth() && pm1->type() == pm2->type() && pm1->size == pm2->size &&
        pm1->isContinuous() && pm2->isContinuous() &&
        pm1->योग()*cn1 <= (माप_प्रकार)ARITHM_SMALL_DIRECT)
    {
        ew::TKernel k = ew::getElemwiseFunc(op, rdepth, rdepth, ew::EW_DEPTH_NONE, rdepth);
        अगर (k.fptr)
        {
            dst.create_Same_आकार(src1, pm1->type());   // whole-shape transfer, not piecemeal dims+sizes
            Mat dloc;
            Mat* pd = (dst.kind() == निवेश_Array_2::MAT) ? (Mat*)dst.getObj() : &(dloc = dst.getMat());
            अगर (pd->isContinuous())     // a reused non-continuous dst view falls through to the engine
            {
                k.fptr(pm1->तथ्य, 0, 1, pm2->तथ्य, 0, 1, nullptr, 0, 0,
                       pd->तथ्य, 0, (पूर्णांक)(pm1->योग()*cn1), 1, params.val, k.flags, k.userdata);
                वापस;
            }
        }
    }

    // operand order = (src1, src2): an array becomes an INPUT, a scalar a flexible per-channel CONST
    // read straight from the caller's inline storage (getObj()), no Mat / convertTo.
    ew::TExpr p;
    स्वतः addOperand = [&](निवेश_Array src, bool isScalar, स्थिर Mat* m, पूर्णांक otherCn) -> पूर्णांक {
        अगर (!isScalar)
            वापस p.add_निवेश(m->depth());
        स्थिर uchar* sp; पूर्णांक sd;
        uchar scbuf[EW_SCALAR_BUF_आकार];   // stack room for a UMAT scalar; addConst copies the values out
        स्थिर पूर्णांक scn0 = scalarArgElems(src, sp, sd, scbuf), cn = otherCn;
        स्थिर पूर्णांक scn = (cn == scn0 || (cn < 4 && scn0 == 4)) ? cn
                      : (scn0 == 1) ? 1 : 0;
        CV_Assert(scn > 0);
        वापस p.addConst(ew::EW_DEPTH_NONE, sd, sp, scn);
    };
    स्थिर पूर्णांक a0 = addOperand(src1, s1, pm1, cn2);
    स्थिर पूर्णांक a1 = addOperand(src2, s2, pm2, cn1);

    // Write-mask: a single-channel 1-byte array (u8/s8/bool) the size of the output spatial shape,
    // added as another broadcast input. The arithmetic result lands in a temp; a final
    // select(mask, r, dst) -> dst overwrites only mask!=0 positions of the (pre-existing) output.
    पूर्णांक sMask = 0;
    स्थिर Mat* pmask = nullptr;
    अगर (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = asMat(mask, mloc);
        sMask = p.add_निवेश(pmask->depth());
    }
    स्थिर पूर्णांक sOut = p.addOutput(rdepth);               // output BEFORE emit -> moveToOutput drops the
    स्थिर पूर्णांक r = p.emit_द्विआधारी(op, a0, a1, rdepth, params);   // result temp (mask-free => none); scale
                                                        // rides params[0] (mul/div), {a,b,g} for addW
    अगर (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    अन्यथा
        p.moveToOutput(r, sOut);
    p.compile();

    स्थिर Mat* inputs[3]; पूर्णांक ni = 0;                   // inputs in program (input-index) order
    अगर (!s1) inputs[ni++] = pm1;
    अगर (!s2) inputs[ni++] = pm2;
    अगर (haveMask) inputs[ni++] = pmask;

    // Pre-create the output at the broadcast shape, ALWAYS via dst.create() - never hand exec a raw
    // dst Mat. Routing through the _OutputArray is what enforces its contract: a FIXED_SIZE/FIXED_TYPE
    // dst (in-place 'a += b' passes '(const Mat&)a') throws here if a broadcast would change its shape
    // or type (Mat(1x1) += Mat(1x4)), instead of silently reallocating - exec sees only a Mat and has
    // no view of the array's fixed flags. Cost over a direct exec is negligible: exec keeps its own
    // fast path for a matching-shape op, and dst.create() with an unchanged shape/type is a no-op.
    // With a mask, a freshly (re)allocated dst is zeroed so mask==0 reads 0 (matches arithm_op); a
    // reused dst keeps its prior content there - detect reuse by shape+type BEFORE create (correct for
    // a reused UMat dst too).
    MatShape oshape; पूर्णांक ocn;
    p.outputShape(inputs, oshape, ocn);
    स्थिर पूर्णांक otype = CV_MAKETYPE(rdepth, ocn);
    स्थिर bool reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == निवेश_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    अगर (haveMask && !reused)
        pdst->setZero();                          // setZero (not setTo, which caps at 4 channels)
    p.exec(inputs, pdst);
}

// Unary element-wise op through the engine (bitwise NOT now; unary math later). One array input, an
// optional write-mask; result depth = dtype (>=0), else a fixed-type dst's depth, else the input
// depth. Mirrors arithm_op's build/mask/exec tail with a single input and no scalar operand. OpenCL is
// run by the caller (bitwise_op_ocl), so this is the CPU engine only.
अटल व्योम unary_op(ew::शीर्ष op, निवेश_Array src, OutputArray dst, निवेश_Array mask,
                     पूर्णांक dtype, स्थिर Scalar& params)
{
    अगर (src.empty())
    {
        dst.release();
        अगर (dtype >= 0)
            dst.create(0, 0, dtype);
        वापस;
    }

    स्थिर bool haveMask = !mask.empty();
    // in-place (dst aliases src): keep a header copy so a shape/type-changing dst.create() can't free
    // src's data mid-op (matches arithm_op). A fixed-size/type dst never reallocs, so this is only for
    // an explicit dtype change.
    स्थिर व्योम* dstObj = (dst.kind() == निवेश_Array_2::MAT) ? dst.getObj() : nullptr;
    स्थिर bool aliased = dstObj && src.kind() == निवेश_Array_2::MAT && dstObj == src.getObj();
    Mat sloc, mloc;
    स्थिर Mat* psrc = (src.kind() == निवेश_Array_2::MAT && !aliased) ? (स्थिर Mat*)src.getObj()
                                                                   : &(sloc = src.getMat());
    स्थिर पूर्णांक rdepth = dtype >= 0 ? CV_MAT_DEPTH(dtype)
                     : (dst.fixedType() ? dst.depth() : psrc->depth());

    ew::TExpr p;
    स्थिर पूर्णांक a0 = p.add_निवेश(psrc->depth());
    पूर्णांक sMask = 0;
    स्थिर Mat* pmask = nullptr;
    अगर (haveMask)
    {
        CV_Assert(mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);
        pmask = (mask.kind() == निवेश_Array_2::MAT) ? (स्थिर Mat*)mask.getObj() : &(mloc = mask.getMat());
        sMask = p.add_निवेश(pmask->depth());
    }
    स्थिर पूर्णांक sOut = p.addOutput(rdepth);
    स्थिर पूर्णांक r = p.emitUnary(op, a0, rdepth, params);
    अगर (haveMask)
        p.addInsn(ew::OP_SELECT, sMask, r, sOut, sOut);
    अन्यथा
        p.moveToOutput(r, sOut);
    p.compile();

    स्थिर Mat* inputs[2]; पूर्णांक ni = 0;
    inputs[ni++] = psrc;
    अगर (haveMask) inputs[ni++] = pmask;

    MatShape oshape; पूर्णांक ocn;
    p.outputShape(inputs, oshape, ocn);
    स्थिर पूर्णांक otype = CV_MAKETYPE(rdepth, ocn);
    स्थिर bool reused = haveMask && outArrayMatches(dst, oshape, otype);
    dst.create(oshape, otype);
    Mat dstloc;
    Mat* pdst = (dst.kind() == निवेश_Array_2::MAT) ? (Mat*)dst.getObj() : &(dstloc = dst.getMat());
    अगर (haveMask && !reused)
        pdst->setZero();
    p.exec(inputs, pdst);
}

}

व्योम cv::add( निवेश_Array src1, निवेश_Array src2, OutputArray dst,
          निवेश_Array mask, पूर्णांक dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ADD, src1, src2, dst, mask, dtype, OCL_OP_ADD, false);
}

व्योम cv::subtract( निवेश_Array src1, निवेश_Array src2, OutputArray dst,
                   निवेश_Array mask, पूर्णांक dtype )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_SUB, src1, src2, dst, mask, dtype, OCL_OP_SUB, false);
}

व्योम cv::absdiff( निवेश_Array src1, निवेश_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_ABSDIFF, src1, src2, dst, noArray(), -1, OCL_OP_ABSDIFF, false);
}

व्योम cv::copyTo(निवेश_Array _src, OutputArray _dst, निवेश_Array _mask)
{
    CV_INSTRUMENT_REGION();

    _src.copyTo(_dst, _mask);
}

/****************************************************************************************\
*                                    multiply/divide                                     *
\****************************************************************************************/

namespace cv
{

व्योम multiply(निवेश_Array src1, निवेश_Array src2,
              OutputArray dst, द्विगुन scale, पूर्णांक dtype)
{
    CV_INSTRUMENT_REGION();

    स्थिर पूर्णांक oclop = std::असल(scale - 1.0) < द्विग_अंतर ? OCL_OP_MUL : OCL_OP_MUL_SCALE;
    arithm_op(ew::OP_MUL, src1, src2, dst, noArray(), dtype, oclop, /*muldiv*/ true, Scalar(scale));
}

व्योम divide(निवेश_Array src1, निवेश_Array src2,
                OutputArray dst, द्विगुन scale, पूर्णांक dtype)
{
    CV_INSTRUMENT_REGION();

    arithm_op(ew::OP_DIV, src1, src2, dst, noArray(), dtype, OCL_OP_DIV_SCALE, /*muldiv*/ true, Scalar(scale));
}

व्योम divide(द्विगुन scale, निवेश_Array src2,
                OutputArray dst, पूर्णांक dtype)
{
    CV_INSTRUMENT_REGION();

    अगर (src2.empty())
    {
        dst.release();
        वापस;
    }

    // scale / src2 == scale * 1 / src2: feed a 0-dim `1` as the numerator so the CPU engine reuses the
    // normal divide (scale*a/b). Give the numerator src2's OWN depth: then OP_DIV(T, T) is same-type and
    // takes the fast per-type kernel (f32 work for <=16-bit / f16 / bf16, not the s32->f64 path a CV_32S
    // numerator would force). An integer src2 keeps its div-by-zero->0 guard (both operands integer); a
    // float src2 divides as float (1/0->inf), both matching cv::divide. dtype<0 follows src2's depth (a
    // fixed-type dst still wins). The UMat path uses the dedicated reciprocal kernel (OCL_OP_RECIP_SCALE).
    द्विगुन one = 1;
    Mat numerator(MatShape::scalar(), src2.depth(), &one);
    scalar_To_Raw_तथ्य(Scalar(1), &one, src2.depth(), 1);       // write `1` in src2's depth (no allocation)
    स्थिर पूर्णांक rtype = (dtype < 0 && !dst.fixedType()) ? src2.depth() : dtype;
    arithm_op(ew::OP_DIV, numerator, src2, dst, noArray(), rtype, OCL_OP_RECIP_SCALE, /*muldiv*/ true, Scalar(scale));
}

UMat UMat::mul(निवेश_Array m, द्विगुन scale) स्थिर
{
    UMat dst;
    multiply(*यह, m, dst, scale);
    वापस dst;
}

/****************************************************************************************\
*                                      addWeighted                                       *
\****************************************************************************************/

}

व्योम cv::addWeighted( निवेश_Array src1, द्विगुन alpha, निवेश_Array src2,
                      द्विगुन beta, द्विगुन gamma, OutputArray dst, पूर्णांक dtype )
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


अटल द्विगुन getMinVal(पूर्णांक depth)
{
    अटल स्थिर द्विगुन tab[CV_DEPTH_MAX] =
    {
        0, -128, 0, -32768, पूर्णांक_न्यून, -भग्न_उच्च, -द्विग_उच्च,
        -65504, -भग्न_उच्च, 0, 0, (द्विगुन)INT64_MIN, 0
    };
    वापस tab[depth];
}

अटल द्विगुन getMaxVal(पूर्णांक depth)
{
    अटल स्थिर द्विगुन tab[CV_DEPTH_MAX] = {
        255, 127, 65535, 32767, पूर्णांक_उच्च, भग्न_उच्च, द्विग_उच्च,
        65504, भग्न_उच्च, 255, (द्विगुन)UINT64_MAX, (द्विगुन)INT64_MAX, (द्विगुन)UINT32_MAX, 0
    };
    वापस tab[depth];
}

#अगर_घोषित HAVE_OPENCL

अटल bool ocl_compare(निवेश_Array _src1, निवेश_Array _src2, OutputArray _dst, पूर्णांक op, bool haveScalar)
{
    स्थिर ocl::Device& dev = ocl::Device::getDefault();
    bool doubleSupport = dev.doubleFPConfig() > 0;
    पूर्णांक type1 = _src1.type(), depth1 = CV_MAT_DEPTH(type1), cn = CV_MAT_CN(type1),
            type2 = _src2.type(), depth2 = CV_MAT_DEPTH(type2);

    अगर (!doubleSupport && depth1 == CV_64F)
        वापस false;

    // the integer scalar path below works in int; let the CPU handle the wider 5.x integer depths
    अगर (haveScalar && (depth1 == CV_32U || depth1 == CV_64U || depth1 == CV_64S))
        वापस false;

    अगर (!haveScalar && (!_src1.same_आकार(_src2) || type1 != type2))
            वापस false;

    // This OpenCL kernel broadcasts a scalar's FIRST channel to all channels. The CPU path compares
    // per-channel (scalar[c] for channel c), so a multichannel array against a scalar with DISTINCT
    // channel values would disagree - bail to the CPU engine in that case (rare; not worth a kernel fix).
    अगर (haveScalar && cn > 1)
    {
        Mat sc = _src2.getMat();
        पूर्णांक scn = (पूर्णांक)sc.योग();
        अगर (scn > 1)
        {
            द्विगुन v[4] = { 0, 0, 0, 0 };
            getConvertFunc(sc.depth(), CV_64F)(sc.ptr(), 1, 0, 1, (uchar*)v, 1, आकार(std::min(scn, 4), 1), 0);
            क्रम (पूर्णांक i = 1, n = std::min(cn, scn); i < n; i++)
                अगर (v[i] != v[0])
                    वापस false;
        }
    }

    पूर्णांक kercn = haveScalar ? cn : ocl::predictOptimalVectorWidth(_src1, _src2, _dst), rowsPerWI = dev.isIntel() ? 4 : 1;
    // Workaround for bug with "?:" operator in AMD OpenCL compiler
    अगर (depth1 >= CV_16U)
        kercn = 1;

    पूर्णांक scalarcn = kercn == 3 ? 4 : kercn;
    स्थिर अक्षर * स्थिर operationMap[] = { "==", ">", ">=", "<", "<=", "!=" };
    अक्षर cvt[50];

    String opts = format("-D %s -D srcT1=%s -D dstT=%s -D DEPTH_dst=%d -D workT=srcT1 -D cn=%d"
                         " -D convertToDT=%s -D OP_CMP -D CMP_OPERATOR=%s -D srcT1_C1=%s"
                         " -D srcT2_C1=%s -D dstT_C1=%s -D workST=%s -D rowsPerWI=%d%s",
                         haveScalar ? "UNARY_OP" : "BINARY_OP",
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, kercn)),
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U, kercn,
                         ocl::convertTypeStr(depth1, CV_8U, kercn, cvt, माप(cvt)),
                         operationMap[op], ocl::typeToStr(depth1),
                         ocl::typeToStr(depth1), ocl::typeToStr(CV_8U),
                         ocl::typeToStr(CV_MAKE_TYPE(depth1, scalarcn)), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, opts);
    अगर (k.empty())
        वापस false;

    UMat src1 = _src1.getUMat();
    आकार size = src1.size();
    _dst.create(size, CV_8UC(cn));
    UMat dst = _dst.getUMat();

    अगर (haveScalar)
    {
        माप_प्रकार esz = CV_ELEM_SIZE1(type1) * scalarcn;
        द्विगुन buf[4] = { 0, 0, 0, 0 };
        Mat src2 = _src2.getMat();

        अगर( depth1 > CV_32S )
            convertAndUnrollScalar( src2, depth1, (uchar *)buf, kercn );
        अन्यथा
        {
            द्विगुन fval = 0;
            getConvertFunc(depth2, CV_64F)(src2.ptr(), 1, 0, 1, (uchar *)&fval, 1, आकार(1, 1), 0);
            अगर( fval < getMinVal(depth1) )
                वापस dst.setTo(Scalar::all(op == CMP_GT || op == CMP_GE || op == CMP_NE ? 255 : 0)), true;

            अगर( fval > getMaxVal(depth1) )
                वापस dst.setTo(Scalar::all(op == CMP_LT || op == CMP_LE || op == CMP_NE ? 255 : 0)), true;

            पूर्णांक ival = cvRound(fval);
            अगर( fval != ival )
            {
                अगर( op == CMP_LT || op == CMP_GE )
                    ival = cvCeil(fval);
                अन्यथा अगर( op == CMP_LE || op == CMP_GT )
                    ival = cvFloor(fval);
                अन्यथा
                    वापस dst.setTo(Scalar::all(op == CMP_NE ? 255 : 0)), true;
            }
            convertAndUnrollScalar(Mat(1, 1, CV_32S, &ival), depth1, (uchar *)buf, kercn);
        }

        ocl::KernelArg scalararg = ocl::KernelArg(ocl::KernelArg::CONSTANT, 0, 0, 0, buf, esz);

        k.args(ocl::KernelArg::Read_Only_No_आकार(src1, cn, kercn),
               ocl::KernelArg::WriteOnly(dst, cn, kercn), scalararg);
    }
    अन्यथा
    {
        UMat src2 = _src2.getUMat();

        k.args(ocl::KernelArg::Read_Only_No_आकार(src1),
               ocl::KernelArg::Read_Only_No_आकार(src2),
               ocl::KernelArg::WriteOnly(dst, cn, kercn));
    }

    माप_प्रकार globalsize[2] = { (माप_प्रकार)dst.cols * cn / kercn, ((माप_प्रकार)dst.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

#पूर्ण_अगर

}

व्योम cv::compare(निवेश_Array _src1, निवेश_Array _src2, OutputArray _dst, पूर्णांक op)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( op == CMP_LT || op == CMP_LE || op == CMP_EQ ||
               op == CMP_NE || op == CMP_GE || op == CMP_GT );

    CV_Assert(_src1.empty() == _src2.empty());
    अगर (_src1.empty() && _src2.empty())
    {
        _dst.release();
        वापस;
    }

    bool haveScalar = false;

    अगर ((_src1.isMatx() + _src2.isMatx()) == 1
            || !_src1.same_आकार(_src2)
            || _src1.type() != _src2.type())
    {
        bool is_src1_scalar = checkScalar(_src1, _src2.type(), _src1.kind(), _src2.kind());
        bool is_src2_scalar = checkScalar(_src2, _src1.type(), _src2.kind(), _src1.kind());

        // exactly one scalar operand: keep it as src2 (swapping flips the ordering ops) so the OpenCL
        // path sees the array-op-scalar form. Otherwise - two arrays of different size (broadcast),
        // mixed types, or two scalars - the CPU engine broadcasts / promotes them, so there is NO error
        // here: fall through to arithm_op (which handles scalar detection, broadcast and mixed types).
        अगर (is_src1_scalar && !is_src2_scalar)
        {
            op = op == CMP_LT ? CMP_GT : op == CMP_LE ? CMP_GE :
                op == CMP_GE ? CMP_LE : op == CMP_GT ? CMP_LT : op;
            // src1 is a scalar; swap it with src2
            compare(_src2, _src1, _dst, op);
            वापस;
        }
        haveScalar = is_src2_scalar && !is_src1_scalar;
    }
    CV_UNUSED(haveScalar);   // consumed by CV_OCL_RUN only - a no-op without OpenCL

    // OpenCL handles only the same-size and array-op-scalar forms; a broadcast between two arrays of
    // different size falls through to the CPU engine.
    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat())
               && (haveScalar || _src1.same_आकार(_src2)),
               ocl_compare(_src1, _src2, _dst, op, haveScalar))

    // CPU: the element-wise engine. A CMP_* code maps to the corresponding OP_CMP_* TOp; the engine
    // forces a u8 mask (dtype = CV_8U) and compares in the common operand type (a fractional scalar
    // threshold promotes both operands to float - matching the classic ceil/floor threshold logic).
    // The scalar / broadcast / mixed-shape cases are handled inside arithm_op. oclop = -1 tells it the
    // OpenCL path was already tried above (via ocl_compare), so it only runs the CPU engine.
    ew::शीर्ष cmpOp = op == CMP_EQ ? ew::OP_CMP_EQ : op == CMP_NE ? ew::OP_CMP_NE :
                    op == CMP_LT ? ew::OP_CMP_LT : op == CMP_LE ? ew::OP_CMP_LE :
                    op == CMP_GT ? ew::OP_CMP_GT : ew::OP_CMP_GE;
    arithm_op(cmpOp, _src1, _src2, _dst, noArray(), CV_8U, /*oclop=*/-1, /*muldiv=*/false);
}

/****************************************************************************************\
*                                        inRange                                         *
\****************************************************************************************/

namespace cv
{

ढाँचा <typename T>
काष्ठा InRange_SIMD
{
    पूर्णांक चालक () (स्थिर T * src1, स्थिर T * src2, स्थिर T * src3, uchar * dst, पूर्णांक len) स्थिर
    {
        क्रम (पूर्णांक x = 0; x < len; x++)
            dst[x] = (src2[x] <= src1[x] && src1[x] <= src3[x]) ? 255 : 0;
        वापस len;
    }
};

#अगर (CV_SIMD || CV_SIMD_SCALABLE)

ढाँचा <>
काष्ठा InRange_SIMD<uchar>
{
    पूर्णांक चालक () (स्थिर uchar * src1, स्थिर uchar * src2, स्थिर uchar * src3,
        uchar * dst, पूर्णांक len) स्थिर
    {
        पूर्णांक x = 0;
        स्थिर पूर्णांक width = VTraits<v_uint8>::vlanes();

        क्रम (; x <= len - width; x += width)
        {
            v_uint8 values = vx_load(src1 + x);
            v_uint8 निचला = vx_load(src2 + x);
            v_uint8 ऊपरी = vx_load(src3 + x);

            v_store(dst + x, v_and(v_ge(values, निचला), v_ge(ऊपरी, values)));
        }
        vx_cleanup();
        वापस x;
    }
};

ढाँचा <>
काष्ठा InRange_SIMD<schar>
{
    पूर्णांक चालक () (स्थिर schar * src1, स्थिर schar * src2, स्थिर schar * src3,
        uchar * dst, पूर्णांक len) स्थिर
    {
        पूर्णांक x = 0;
        स्थिर पूर्णांक width = VTraits<v_int8>::vlanes();

        क्रम (; x <= len - width; x += width)
        {
            v_int8 values = vx_load(src1 + x);
            v_int8 निचला = vx_load(src2 + x);
            v_int8 ऊपरी = vx_load(src3 + x);

            v_store((schar*)(dst + x), v_and(v_ge(values, निचला), v_ge(ऊपरी, values)));
        }
        vx_cleanup();
        वापस x;
    }
};

ढाँचा <>
काष्ठा InRange_SIMD<ushort>
{
    पूर्णांक चालक () (स्थिर ushort * src1, स्थिर ushort * src2, स्थिर ushort * src3,
        uchar * dst, पूर्णांक len) स्थिर
    {
        पूर्णांक x = 0;
        स्थिर पूर्णांक width = VTraits<v_uint16>::vlanes() * 2;

        क्रम (; x <= len - width; x += width)
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
        वापस x;
    }
};

ढाँचा <>
काष्ठा InRange_SIMD<लघु>
{
    पूर्णांक चालक () (स्थिर लघु * src1, स्थिर लघु * src2, स्थिर लघु * src3,
        uchar * dst, पूर्णांक len) स्थिर
    {
        पूर्णांक x = 0;
        स्थिर पूर्णांक width = (पूर्णांक)VTraits<v_int16>::vlanes() * 2;

        क्रम (; x <= len - width; x += width)
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
        वापस x;
    }
};

ढाँचा <>
काष्ठा InRange_SIMD<पूर्णांक>
{
    पूर्णांक चालक () (स्थिर पूर्णांक * src1, स्थिर पूर्णांक * src2, स्थिर पूर्णांक * src3,
        uchar * dst, पूर्णांक len) स्थिर
    {
        पूर्णांक x = 0;
        स्थिर पूर्णांक width = (पूर्णांक)VTraits<v_int32>::vlanes() * 2;

        क्रम (; x <= len - width; x += width)
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
        वापस x;
    }
};

ढाँचा <>
काष्ठा InRange_SIMD<भग्न>
{
    पूर्णांक चालक () (स्थिर भग्न * src1, स्थिर भग्न * src2, स्थिर भग्न * src3,
        uchar * dst, पूर्णांक len) स्थिर
    {
        पूर्णांक x = 0;
        स्थिर पूर्णांक width = (पूर्णांक)VTraits<v_float32>::vlanes() * 2;

        क्रम (; x <= len - width; x += width)
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
        वापस x;
    }
};

ढाँचा <>
काष्ठा InRange_SIMD<hfloat>
{
    पूर्णांक चालक () (स्थिर hfloat * src1, स्थिर hfloat * src2, स्थिर hfloat * src3,
        uchar * dst, पूर्णांक len) स्थिर
    {
        पूर्णांक x = 0;
        स्थिर पूर्णांक width = (पूर्णांक)VTraits<v_float32>::vlanes()*2;

        क्रम (; x <= len - width; x += width)
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
        वापस x;
    }
};

ढाँचा <>
काष्ठा InRange_SIMD<bfloat>
{
    पूर्णांक चालक () (स्थिर bfloat * src1, स्थिर bfloat * src2, स्थिर bfloat * src3,
        uchar * dst, पूर्णांक len) स्थिर
    {
        पूर्णांक x = 0;
        स्थिर पूर्णांक width = (पूर्णांक)VTraits<v_float32>::vlanes()*2;

        क्रम (; x <= len - width; x += width)
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
        वापस x;
    }
};

ढाँचा <>
काष्ठा InRange_SIMD<अचिन्हित>
{
    पूर्णांक चालक () (स्थिर अचिन्हित * src1, स्थिर अचिन्हित * src2, स्थिर अचिन्हित * src3,
        uchar * dst, पूर्णांक len) स्थिर
    {
        पूर्णांक x = 0;
        स्थिर पूर्णांक width = (पूर्णांक)VTraits<v_uint32>::vlanes() * 2;

        क्रम (; x <= len - width; x += width)
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
        वापस x;
    }
};

#अगर CV_SIMD_64F

ढाँचा <>
काष्ठा InRange_SIMD<द्विगुन>
{
    पूर्णांक चालक () (स्थिर द्विगुन * src1, स्थिर द्विगुन * src2, स्थिर द्विगुन * src3,
        uchar * dst, पूर्णांक len) स्थिर
    {
        पूर्णांक x = 0;
        स्थिर पूर्णांक step = VTraits<v_float64>::vlanes();
        स्थिर पूर्णांक width = step * 4;

        क्रम (; x <= len - width; x += width)
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
        वापस x;
    }
};

#पूर्ण_अगर

#पूर्ण_अगर

ढाँचा <typename T>
अटल व्योम inRange_(स्थिर व्योम* _src1, माप_प्रकार step1, स्थिर व्योम* _src2, माप_प्रकार step2,
         स्थिर व्योम* _src3, माप_प्रकार step3, uchar* dst, माप_प्रकार step,
         आकार size)
{
    स्थिर T* src1 = (स्थिर T*)_src1;
    स्थिर T* src2 = (स्थिर T*)_src2;
    स्थिर T* src3 = (स्थिर T*)_src3;
    step1 /= माप(src1[0]);
    step2 /= माप(src2[0]);
    step3 /= माप(src3[0]);

    InRange_SIMD<T> vop;

    क्रम( ; size.height--; src1 += step1, src2 += step2, src3 += step3, dst += step )
    {
        पूर्णांक x = vop(src1, src2, src3, dst, size.width);
        #अगर CV_ENABLE_UNROLLED
        क्रम( ; x <= size.width - 4; x += 4 )
        {
            पूर्णांक t0, t1;
            t0 = src2[x] <= src1[x] && src1[x] <= src3[x];
            t1 = src2[x+1] <= src1[x+1] && src1[x+1] <= src3[x+1];
            dst[x] = (uchar)-t0; dst[x+1] = (uchar)-t1;
            t0 = src2[x+2] <= src1[x+2] && src1[x+2] <= src3[x+2];
            t1 = src2[x+3] <= src1[x+3] && src1[x+3] <= src3[x+3];
            dst[x+2] = (uchar)-t0; dst[x+3] = (uchar)-t1;
        }
        #पूर्ण_अगर
        क्रम( ; x < size.width; x++ )
            dst[x] = (uchar)-(src2[x] <= src1[x] && src1[x] <= src3[x]);
    }
}

अटल व्योम inRangeReduce(स्थिर uchar* src, uchar* dst, माप_प्रकार len, पूर्णांक cn)
{
    पूर्णांक k = cn % 4 ? cn % 4 : 4;
    माप_प्रकार i, j;
    अगर( k == 1 )
        क्रम( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j];
    अन्यथा अगर( k == 2 )
        क्रम( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1];
    अन्यथा अगर( k == 3 )
        क्रम( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2];
    अन्यथा
        क्रम( i = j = 0; i < len; i++, j += cn )
            dst[i] = src[j] & src[j+1] & src[j+2] & src[j+3];

    क्रम( ; k < cn; k += 4 )
    {
        क्रम( i = 0, j = k; i < len; i++, j += cn )
            dst[i] &= src[j] & src[j+1] & src[j+2] & src[j+3];
    }
}

प्रकार व्योम (*InRangeFunc)( स्थिर व्योम* src1, माप_प्रकार step1, स्थिर व्योम* src2, माप_प्रकार step2,
                             स्थिर व्योम* src3, माप_प्रकार step3, uchar* dst, माप_प्रकार step, आकार sz );

अटल InRangeFunc getInRangeFunc(पूर्णांक depth)
{
    अटल InRangeFunc inRangeTab[CV_DEPTH_MAX] =
    {
        inRange_<uchar>,
        inRange_<schar>,
        inRange_<ushort>,
        inRange_<लघु>,
        inRange_<पूर्णांक>,
        inRange_<भग्न>,
        inRange_<द्विगुन>,
        inRange_<hfloat>,
        inRange_<bfloat>,
        0,
        inRange_<uint64_t>,
        inRange_<int64_t>,
        inRange_<अचिन्हित>,
        0,
    };

    वापस inRangeTab[depth];
}

#अगर_घोषित HAVE_OPENCL

अटल bool ocl_inRange( निवेश_Array _src, निवेश_Array _lowerb,
                         निवेश_Array _upperb, OutputArray _dst )
{
    स्थिर ocl::Device & d = ocl::Device::getDefault();
    निवेश_Array_2::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    आकार ssize = _src.size(), lsize = _lowerb.size(), usize = _upperb.size();
    पूर्णांक stype = _src.type(), ltype = _lowerb.type(), utype = _upperb.type();
    पूर्णांक sdepth = CV_MAT_DEPTH(stype), ldepth = CV_MAT_DEPTH(ltype), udepth = CV_MAT_DEPTH(utype);
    पूर्णांक cn = CV_MAT_CN(stype), rowsPerWI = d.isIntel() ? 4 : 1;
    bool lbScalar = false, ubScalar = false;

    अगर( (lkind == निवेश_Array_2::MATX && skind != निवेश_Array_2::MATX) ||
        ssize != lsize || stype != ltype )
    {
        अगर( !checkScalar(_lowerb, stype, lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = true;
    }

    अगर( (ukind == निवेश_Array_2::MATX && skind != निवेश_Array_2::MATX) ||
        ssize != usize || stype != utype )
    {
        अगर( !checkScalar(_upperb, stype, ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = true;
    }

    अगर (lbScalar != ubScalar)
        वापस false;

    bool doubleSupport = d.doubleFPConfig() > 0,
            haveScalar = lbScalar && ubScalar;

    अगर ( (!doubleSupport && sdepth == CV_64F) ||
         (!haveScalar && (sdepth != ldepth || sdepth != udepth)) )
        वापस false;

    पूर्णांक kercn = haveScalar ? cn : std::max(std::min(ocl::predictOptimalVectorWidth(_src, _lowerb, _upperb, _dst), 4), cn);
    अगर (kercn % cn != 0)
        kercn = cn;
    पूर्णांक colsPerWI = kercn / cn;
    String opts = format("%s-D CN=%d -D SRC_T=%s -D SRC_T1=%s -D DST_T=%s -D KERCN=%d -D DEPTH=%d%s -D COLS_PER_WI=%d",
                           haveScalar ? "-D HAVE_SCALAR " : "", cn, ocl::typeToStr(CV_MAKE_TYPE(sdepth, kercn)),
                           ocl::typeToStr(sdepth), ocl::typeToStr(CV_8UC(colsPerWI)), kercn, sdepth,
                           doubleSupport ? " -D DOUBLE_SUPPORT" : "", colsPerWI);

    ocl::Kernel ker("inrange", ocl::core::inrange_oclsrc, opts);
    अगर (ker.empty())
        वापस false;

    _dst.create(ssize, CV_8UC1);
    UMat src = _src.getUMat(), dst = _dst.getUMat(), lscalaru, uscalaru;
    Mat lscalar, uscalar;

    अगर (lbScalar && ubScalar)
    {
        lscalar = _lowerb.getMat();
        uscalar = _upperb.getMat();

        माप_प्रकार esz = src.elem_आकार();
        माप_प्रकार blocksize = 36;

        Auto_बफर<uchar> _buf(blocksize*(((पूर्णांक)lbScalar + (पूर्णांक)ubScalar)*esz + cn) + 2*cn*माप(पूर्णांक) + 128);
        uchar *buf = alignPtr(_buf.तथ्य() + blocksize*cn, 16);

        अगर( ldepth != sdepth && sdepth < CV_32S )
        {
            पूर्णांक* ilbuf = (पूर्णांक*)alignPtr(buf + blocksize*esz, 16);
            पूर्णांक* iubuf = ilbuf + cn;

            द्विआधारी_Func sccvtfunc = getConvertFunc(ldepth, CV_32S);
            sccvtfunc(lscalar.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, आकार(cn, 1), 0);
            sccvtfunc(uscalar.ptr(), 1, 0, 1, (uchar*)iubuf, 1, आकार(cn, 1), 0);
            पूर्णांक minval = cvRound(getMinVal(sdepth)), maxval = cvRound(getMaxVal(sdepth));

            क्रम( पूर्णांक k = 0; k < cn; k++ )
            {
                अगर( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
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

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_आकार(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, 1, colsPerWI);

    अगर (haveScalar)
    {
        lscalar.copyTo(lscalaru);
        uscalar.copyTo(uscalaru);

        ker.args(srcarg, dstarg, ocl::KernelArg::PtrReadOnly(lscalaru),
               ocl::KernelArg::PtrReadOnly(uscalaru), rowsPerWI);
    }
    अन्यथा
        ker.args(srcarg, dstarg, ocl::KernelArg::Read_Only_No_आकार(lscalaru),
               ocl::KernelArg::Read_Only_No_आकार(uscalaru), rowsPerWI);

    माप_प्रकार globalsize[2] = { (माप_प्रकार)ssize.width / colsPerWI, ((माप_प्रकार)ssize.height + rowsPerWI - 1) / rowsPerWI };
    वापस ker.run(2, globalsize, शून्य, false);
}

#पूर्ण_अगर

}

व्योम cv::inRange(निवेश_Array _src, निवेश_Array _lowerb,
                 निवेश_Array _upperb, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(! _src.empty());
    अगर (!getInRangeFunc(_src.depth()))
        CV_Error( cv::Error::StsNotImplemented,
                 "inRange: CV_Bool and CV_8F inputs are not supported, convert the input to another type first");

    CV_OCL_RUN(_src.dims() <= 2 && _lowerb.dims() <= 2 &&
               _upperb.dims() <= 2 && OCL_PERFORMANCE_CHECK(_dst.isUMat()),
               ocl_inRange(_src, _lowerb, _upperb, _dst))

    निवेश_Array_2::KindFlag skind = _src.kind(), lkind = _lowerb.kind(), ukind = _upperb.kind();
    Mat src = _src.getMat(), lb = _lowerb.getMat(), ub = _upperb.getMat();

    bool lbScalar = false, ubScalar = false;

    अगर( (lkind == निवेश_Array_2::MATX && skind != निवेश_Array_2::MATX) ||
        src.size != lb.size || src.type() != lb.type() )
    {
        अगर( !checkScalar(lb, src.type(), lkind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The lower boundary is neither an array of the same size and same type as src, nor a scalar");
        lbScalar = true;
    }

    अगर( (ukind == निवेश_Array_2::MATX && skind != निवेश_Array_2::MATX) ||
        src.size != ub.size || src.type() != ub.type() )
    {
        अगर( !checkScalar(ub, src.type(), ukind, skind) )
            CV_Error( cv::Error::StsUnmatchedSizes,
                     "The upper boundary is neither an array of the same size and same type as src, nor a scalar");
        ubScalar = true;
    }

    CV_Assert(lbScalar == ubScalar);

    पूर्णांक cn = src.channels(), depth = src.depth();

    माप_प्रकार esz = src.elem_आकार();
    माप_प्रकार blocksize0 = (माप_प्रकार)(BLOCK_आकार + esz-1)/esz;

    _dst.create_Same_आकार(_src, CV_8UC1);
    Mat dst = _dst.getMat();
    InRangeFunc func = getInRangeFunc(depth);

    स्थिर Mat* arrays_sc[] = { &src, &dst, 0 };
    स्थिर Mat* arrays_nosc[] = { &src, &dst, &lb, &ub, 0 };
    uchar* ptrs[4] = {};

    NAryMatIterator it(lbScalar && ubScalar ? arrays_sc : arrays_nosc, ptrs);
    माप_प्रकार योग = it.size, blocksize = std::min(योग, blocksize0);

    Auto_बफर<uchar> _buf(blocksize*(((पूर्णांक)lbScalar + (पूर्णांक)ubScalar)*esz + cn) + 2*cn*माप(पूर्णांक) + 128);
    uchar *buf = _buf.तथ्य(), *mbuf = buf, *lbuf = 0, *ubuf = 0;
    buf = alignPtr(buf + blocksize*cn, 16);

    अगर( lbScalar && ubScalar )
    {
        lbuf = buf;
        ubuf = buf = alignPtr(buf + blocksize*esz, 16);

        CV_Assert( lb.type() == ub.type() );
        पूर्णांक scdepth = lb.depth();

        अगर( scdepth != depth && depth < CV_32S )
        {
            पूर्णांक* ilbuf = (पूर्णांक*)alignPtr(buf + blocksize*esz, 16);
            पूर्णांक* iubuf = ilbuf + cn;

            द्विआधारी_Func sccvtfunc = getConvertFunc(scdepth, CV_32S);
            sccvtfunc(lb.ptr(), 1, 0, 1, (uchar*)ilbuf, 1, आकार(cn, 1), 0);
            sccvtfunc(ub.ptr(), 1, 0, 1, (uchar*)iubuf, 1, आकार(cn, 1), 0);
            पूर्णांक minval = cvRound(getMinVal(depth)), maxval = cvRound(getMaxVal(depth));

            क्रम( पूर्णांक k = 0; k < cn; k++ )
            {
                अगर( ilbuf[k] > iubuf[k] || ilbuf[k] > maxval || iubuf[k] < minval )
                    ilbuf[k] = minval+1, iubuf[k] = minval;
            }
            lb = Mat(cn, 1, CV_32S, ilbuf);
            ub = Mat(cn, 1, CV_32S, iubuf);
        }

        convertAndUnrollScalar( lb, src.type(), lbuf, blocksize );
        convertAndUnrollScalar( ub, src.type(), ubuf, blocksize );

        अगर (depth == CV_8U && src.dims <= 2) {
            uint8_t lb_scalar = lbuf[0];
            uint8_t ub_scalar = ubuf[0];
            CALL_HAL(inRange_u8, cv_hal_inRange8u, src.तथ्य, src.step, dst.तथ्य, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        } अन्यथा अगर (depth == CV_32F && src.dims <= 2) {
            द्विगुन lb_scalar = lb.ptr<द्विगुन>(0)[0];
            द्विगुन ub_scalar = ub.ptr<द्विगुन>(0)[0];
            CALL_HAL(inRange_f32, cv_hal_inRange32f, src.तथ्य, src.step, dst.तथ्य, dst.step, dst.depth(), src.cols, src.rows, src.channels(),
                lb_scalar, ub_scalar);
        }
    }

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        क्रम( माप_प्रकार j = 0; j < योग; j += blocksize )
        {
            पूर्णांक bsz = (पूर्णांक)MIN(योग - j, blocksize);
            माप_प्रकार delta = bsz*esz;
            uchar *lptr = lbuf, *uptr = ubuf;
            अगर( !lbScalar )
            {
                lptr = ptrs[2];
                ptrs[2] += delta;
            }
            अगर( !ubScalar )
            {
                पूर्णांक idx = !lbScalar ? 3 : 2;
                uptr = ptrs[idx];
                ptrs[idx] += delta;
            }
            func( ptrs[0], 0, lptr, 0, uptr, 0, cn == 1 ? ptrs[1] : mbuf, 0, आकार(bsz*cn, 1));
            अगर( cn > 1 )
                inRangeReduce(mbuf, ptrs[1], bsz, cn);
            ptrs[0] += delta;
            ptrs[1] += bsz;
        }
    }
}
/* End of file. */
