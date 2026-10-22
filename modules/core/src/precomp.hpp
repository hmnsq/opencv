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
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
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

#నిర్వచితం_కాకపోతే __OPENCV_PRECOMP_H__
#నిర్వచించు __OPENCV_PRECOMP_H__

#నిర్వచితమైతే _MSC_VER
#  define SILENCE_CXX17_C_శీర్షిక_DEPRECATION_WARNING
#  define SILENCE_CXX17_CODECVT_శీర్షిక_DEPRECATION_WARNING
#అయితే_ముగింపు

#నిర్వచితమైతే BUILD_PLUGIN
#అన్తర్భావ "opencv2/core/utility.hpp"
#లేకపోతే  // BUILD_PLUGIN

#అన్తర్భావ "opencv2/opencv_modules.hpp"
#అన్తర్భావ "cvconfig.h"

#అన్తర్భావ "opencv2/core/utility.hpp"
#అన్తర్భావ "opencv2/core/cuda.hpp"
#అన్తర్భావ "opencv2/core/opengl.hpp"
#అన్తర్భావ "opencv2/core/va_intel.hpp"

#అన్తర్భావ "opencv2/core/private.hpp"
#అన్తర్భావ "opencv2/core/private.cuda.hpp"
#నిర్వచితమైతే HAVE_OPENCL
#అన్తర్భావ "opencv2/core/ocl.hpp"
#అయితే_ముగింపు

#అన్తర్భావ <ctype.h>
#అన్తర్భావ <float.h>
#అన్తర్భావ <limits.h>
#అన్తర్భావ <math.h>
#అన్తర్భావ <stdarg.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ <algorithm>
#అన్తర్భావ <cmath>
#అన్తర్భావ <cstdlib>
#అన్తర్భావ <limits>
#అన్తర్భావ <float.h>
#అన్తర్భావ <cstring>
#అన్తర్భావ <cassert>

#నిర్వచించు USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#నిర్వచించు USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#నిర్వచించు USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#నిర్వచించు USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#అన్తర్భావ "opencv2/core/hal/hal.hpp"
#అన్తర్భావ "opencv2/core/hal/intrin.hpp"
#అన్తర్భావ "opencv2/core/neon_utils.hpp"
#అన్తర్భావ "opencv2/core/vsx_utils.hpp"
#అన్తర్భావ "hal_replacement.hpp"


నామాకాశ cv
{

// -128.f ... 255.f
బాహ్య స్థిరరాశి దశాంశం g_8x32fTab[];
#నిర్వచించు CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

బాహ్య స్థిరరాశి ushort g_8x16uSqrTab[];
#నిర్వచించు CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

బాహ్య స్థిరరాశి uchar g_Saturate8u[];
#నిర్వచించు CV_FAST_CAST_8U(t)   (ప్రతిజ్ఞా(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#నిర్వచించు CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#నిర్వచించు CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

మూస<రకం_పేరు T1, రకం_పేరు T2=T1, రకం_పేరు T3=T1> నిర్మాణం OpNop
{
    రకనిర్వచనం T1 type1;
    రకనిర్వచనం T2 type2;
    రకనిర్వచనం T3 rtype;
    T3 క్రియాకారకం ()(స్థిరరాశి T1 a) స్థిరరాశి { తిరిగి saturate_cast<T3>(a); }
};

మూస<రకం_పేరు T1, రకం_పేరు T2=T1, రకం_పేరు T3=T1> నిర్మాణం OpSqr
{
    రకనిర్వచనం T1 type1;
    రకనిర్వచనం T2 type2;
    రకనిర్వచనం T3 rtype;
    T3 క్రియాకారకం ()(స్థిరరాశి T1 a) స్థిరరాశి { తిరిగి saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

మూస<రకం_పేరు T1, రకం_పేరు T2=T1, రకం_పేరు T3=T1> నిర్మాణం Op_కూడు
{
    రకనిర్వచనం T1 type1;
    రకనిర్వచనం T2 type2;
    రకనిర్వచనం T3 rtype;
    T3 క్రియాకారకం ()(స్థిరరాశి T1 a, స్థిరరాశి T2 b) స్థిరరాశి { తిరిగి saturate_cast<T3>(a + b); }
};

మూస<రకం_పేరు T1, రకం_పేరు T2=T1, రకం_పేరు T3=T1> నిర్మాణం Op_కూడు_Sqr
{
    రకనిర్వచనం T1 type1;
    రకనిర్వచనం T2 type2;
    రకనిర్వచనం T3 rtype;
    T3 క్రియాకారకం ()(స్థిరరాశి T1 a, స్థిరరాశి T2 b) స్థిరరాశి { తిరిగి saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

మూస<రకం_పేరు T1, రకం_పేరు T2=T1, రకం_పేరు T3=T1> నిర్మాణం OpSub
{
    రకనిర్వచనం T1 type1;
    రకనిర్వచనం T2 type2;
    రకనిర్వచనం T3 rtype;
    T3 క్రియాకారకం ()(స్థిరరాశి T1 a, స్థిరరాశి T2 b) స్థిరరాశి { తిరిగి saturate_cast<T3>(a - b); }
};

మూస<రకం_పేరు T1, రకం_పేరు T2=T1, రకం_పేరు T3=T1> నిర్మాణం OpRSub
{
    రకనిర్వచనం T1 type1;
    రకనిర్వచనం T2 type2;
    రకనిర్వచనం T3 rtype;
    T3 క్రియాకారకం ()(స్థిరరాశి T1 a, స్థిరరాశి T2 b) స్థిరరాశి { తిరిగి saturate_cast<T3>(b - a); }
};

మూస<రకం_పేరు T> నిర్మాణం OpMin
{
    రకనిర్వచనం T type1;
    రకనిర్వచనం T type2;
    రకనిర్వచనం T rtype;
    T క్రియాకారకం ()(స్థిరరాశి T a, స్థిరరాశి T b) స్థిరరాశి { తిరిగి std::min(a, b); }
};

మూస<రకం_పేరు T> నిర్మాణం OpMax
{
    రకనిర్వచనం T type1;
    రకనిర్వచనం T type2;
    రకనిర్వచనం T rtype;
    T క్రియాకారకం ()(స్థిరరాశి T a, స్థిరరాశి T b) స్థిరరాశి { తిరిగి std::max(a, b); }
};

మూస<రకం_పేరు T> నిర్మాణం OpAbsDiff
{
    రకనిర్వచనం T type1;
    రకనిర్వచనం T type2;
    రకనిర్వచనం T rtype;
    T క్రియాకారకం()(T a, T b) స్థిరరాశి { తిరిగి a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
మూస<> నిర్మాణం OpAbsDiff<దశాంశం>
{
    రకనిర్వచనం దశాంశం type1;
    రకనిర్వచనం దశాంశం type2;
    రకనిర్వచనం దశాంశం rtype;
    దశాంశం క్రియాకారకం()(దశాంశం a, దశాంశం b) స్థిరరాశి { తిరిగి std::నిరపేక్ష(a - b); }
};
మూస<> నిర్మాణం OpAbsDiff<ద్విగుణ>
{
    రకనిర్వచనం ద్విగుణ type1;
    రకనిర్వచనం ద్విగుణ type2;
    రకనిర్వచనం ద్విగుణ rtype;
    ద్విగుణ క్రియాకారకం()(ద్విగుణ a, ద్విగుణ b) స్థిరరాశి { తిరిగి std::నిరపేక్ష(a - b); }
};

మూస<రకం_పేరు T> నిర్మాణం OpAnd
{
    రకనిర్వచనం T type1;
    రకనిర్వచనం T type2;
    రకనిర్వచనం T rtype;
    T క్రియాకారకం()( T a, T b ) స్థిరరాశి { తిరిగి a & b; }
};

మూస<రకం_పేరు T> నిర్మాణం OpOr
{
    రకనిర్వచనం T type1;
    రకనిర్వచనం T type2;
    రకనిర్వచనం T rtype;
    T క్రియాకారకం()( T a, T b ) స్థిరరాశి { తిరిగి a | b; }
};

మూస<రకం_పేరు T> నిర్మాణం OpXor
{
    రకనిర్వచనం T type1;
    రకనిర్వచనం T type2;
    రకనిర్వచనం T rtype;
    T క్రియాకారకం()( T a, T b ) స్థిరరాశి { తిరిగి a ^ b; }
};

మూస<రకం_పేరు T> నిర్మాణం OpNot
{
    రకనిర్వచనం T type1;
    రకనిర్వచనం T type2;
    రకనిర్వచనం T rtype;
    T క్రియాకారకం()( T a, T ) స్థిరరాశి { తిరిగి ~a; }
};

మూస<> అంతఃవరుస uchar Op_కూడు<uchar>::క్రియాకారకం ()(uchar a, uchar b) స్థిరరాశి
{ తిరిగి CV_FAST_CAST_8U(a + b); }

మూస<> అంతఃవరుస uchar OpSub<uchar>::క్రియాకారకం ()(uchar a, uchar b) స్థిరరాశి
{ తిరిగి CV_FAST_CAST_8U(a - b); }

మూస<> అంతఃవరుస పొట్టి OpAbsDiff<పొట్టి>::క్రియాకారకం ()(పొట్టి a, పొట్టి b) స్థిరరాశి
{ తిరిగి saturate_cast<పొట్టి>(std::నిరపేక్ష(a - b)); }

మూస<> అంతఃవరుస schar OpAbsDiff<schar>::క్రియాకారకం ()(schar a, schar b) స్థిరరాశి
{ తిరిగి saturate_cast<schar>(std::నిరపేక్ష(a - b)); }

మూస<> అంతఃవరుస uchar OpMin<uchar>::క్రియాకారకం ()(uchar a, uchar b) స్థిరరాశి { తిరిగి CV_MIN_8U(a, b); }

మూస<> అంతఃవరుస uchar OpMax<uchar>::క్రియాకారకం ()(uchar a, uchar b) స్థిరరాశి { తిరిగి CV_MAX_8U(a, b); }

రకనిర్వచనం శూన్య (*UnaryFunc)(స్థిరరాశి uchar* src1, size_t step1,
                       uchar* dst, size_t అడుగు, కొలత sz,
                       శూన్య*);

రకనిర్వచనం శూన్య (*ద్విమాన_Func)(స్థిరరాశి uchar* src1, size_t step1,
                       స్థిరరాశి uchar* src2, size_t step2,
                       uchar* dst, size_t అడుగు, కొలత sz,
                       శూన్య*);

రకనిర్వచనం శూన్య (*ద్విమాన_Func_C)(స్థిరరాశి శూన్య* src1, size_t step1,
                       స్థిరరాశి శూన్య* src2, size_t step2,
                       శూన్య* dst, size_t అడుగు, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు,
                       శూన్య*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS ద్విమాన_Func getConvertFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth);
CV_EXPORTS ద్విమాన_Func getConvertScaleFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth);
ద్విమాన_Func get_Copy_ముసుగు_Func(size_t esz);

/* default memory block for sparse array elements */
#నిర్వచించు  CV_SPARSE_MAT_ఖండం     (1<<12)

/* initial hash table size */
#నిర్వచించు  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#నిర్వచించు  CV_SPARSE_HASH_నిష్పత్తి    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
కొలత getContinuousSize2D(Mat& m1, పూర్ణాంకం వెడల్పు_Scale=1);
కొలత getContinuousSize2D(Mat& m1, Mat& m2, పూర్ణాంకం వెడల్పు_Scale=1);
కొలత getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, పూర్ణాంకం వెడల్పు_Scale=1);

శూన్య సమితి_కొలత( Mat& m, పూర్ణాంకం _dims, స్థిరరాశి పూర్ణాంకం* _sz, స్థిరరాశి size_t* అడుగులు_2, తార్కిక auto_అడుగులు=అబద్ధం );
శూన్య finalizeHdr(Mat& m);
పూర్ణాంకం updateContinuityFlag(పూర్ణాంకం flags, పూర్ణాంకం dims, స్థిరరాశి పూర్ణాంకం* size, స్థిరరాశి size_t* అడుగు);

నిర్మాణం NoVec
{
    size_t క్రియాకారకం()(స్థిరరాశి శూన్య*, స్థిరరాశి శూన్య*, శూన్య*, size_t) స్థిరరాశి { తిరిగి 0; }
};

#నిర్వచించు CV_విభజించు_విలీనం_MAX_ఖండం_కొలత(cn) ((గరిష్ఠ_పూర్ణాంకం/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

గణన { ఖండం_కొలత = 1024 };

#ఒకవేళ defined HAVE_IPP && (IPP_సంస్కరణ_X100 >= 700)
#నిర్వచించు ARITHM_USE_IPP 1
#లేకపోతే
#నిర్వచించు ARITHM_USE_IPP 0
#అయితే_ముగింపు

అంతఃవరుస తార్కిక checkScalar(స్థిరరాశి Mat& sc, పూర్ణాంకం atype, ఆదానం_Array_2::రకం_Flag sckind, ఆదానం_Array_2::రకం_Flag akind)
{
    ఒకవేళ( sc.dims > 2 || !sc.isContinuous() )
        తిరిగి అబద్ధం;
    కొలత sz = sc.size();
    ఒకవేళ(sz.వెడల్పు != 1 && sz.ఎత్తు != 1)
        తిరిగి అబద్ధం;
    పూర్ణాంకం cn = CV_MAT_CN(atype);
    ఒకవేళ( akind == ఆదానం_Array_2::MATX && sckind != ఆదానం_Array_2::MATX )
        తిరిగి అబద్ధం;
    తిరిగి sz == కొలత(1, 1) || sz == కొలత(1, cn) || sz == కొలత(cn, 1) ||
           (sz == కొలత(1, 4) && sc.type() == CV_64F && cn <= 4);
}

అంతఃవరుస తార్కిక checkScalar(ఆదానం_Array sc, పూర్ణాంకం atype, ఆదానం_Array_2::రకం_Flag sckind, ఆదానం_Array_2::రకం_Flag akind)
{
    ఒకవేళ( sc.dims() > 2 || !sc.isContinuous() )
        తిరిగి అబద్ధం;
    కొలత sz = sc.size();
    ఒకవేళ(sz.వెడల్పు != 1 && sz.ఎత్తు != 1)
        తిరిగి అబద్ధం;
    పూర్ణాంకం cn = CV_MAT_CN(atype);
    ఒకవేళ( akind == ఆదానం_Array_2::MATX && sckind != ఆదానం_Array_2::MATX )
        తిరిగి అబద్ధం;
    తిరిగి sz == కొలత(1, 1) || sz == కొలత(1, cn) || sz == కొలత(cn, 1) ||
           (sz == కొలత(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
అంతఃవరుస తార్కిక sameShapeAsArg(స్థిరరాశి ఆదానం_Array_2& sc, స్థిరరాశి ఆదానం_Array_2& arr)
{
    తిరిగి arr.channels() == 1 && sc.same_కొలత(arr);
}

// New element-wise engine scalar handling. A genuine number / Scalar / Vec / Matx operand to an
// arithmetic op arrives via _InputArray::MATX (its data is inline in the caller's object;
// getObj() points straight at it). In addition, the EXACT Scalar materialization that the
// python/java bindings and operator-(Mat, Matx) produce - a 2-D 4x1 CV_64F single-channel Mat or
// UMat - is a scalar UNCONDITIONALLY: by coincidence it can be broadcast-COMPATIBLE with the array
// (a 4-row array, a 1-D array make (4,1) legal numpy-wise), and the 4.x per-channel-scalar
// semantics must win there for binding users. Any other real Mat/UMat rides normal broadcasting
// (but see isScalarLikeMat below for the shape-incompatible compat fallback).
// `arr` is the OTHER operand: its channel count decides what a MATX scalar may broadcast over, its
// shape whether the bindings-style column below is a scalar at all.
అంతఃవరుస తార్కిక isScalarArg(స్థిరరాశి ఆదానం_Array_2& sc, స్థిరరాశి ఆదానం_Array_2& arr)
{
    స్థిరరాశి ఆదానం_Array_2::రకం_Flag రకం = sc.రకం();
    స్థిరరాశి పూర్ణాంకం cn = arr.channels();
    ఒకవేళ (రకం == ఆదానం_Array_2::MATX)
    {
        కొలత sz = sc.getSz();
        పూర్ణాంకం scn0 = sz.వెడల్పు * sz.ఎత్తు;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        ఒకవేళ (scn0 != sz.వెడల్పు + sz.ఎత్తు - 1)
            తిరిగి అబద్ధం;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        తిరిగి scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    ఒకవేళ (రకం == ఆదానం_Array_2::MAT)
    {
        స్థిరరాశి Mat& m = *(స్థిరరాశి Mat*)sc.getObj();
        తిరిగి m.అడ్డువరుసలు == 4 && m.నిలువుల_సంఖ్య == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    ఒకవేళ (రకం == ఆదానం_Array_2::UMAT)
    {
        స్థిరరాశి UMat& m = *(స్థిరరాశి UMat*)sc.getObj();
        తిరిగి m.అడ్డువరుసలు == 4 && m.నిలువుల_సంఖ్య == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    తిరిగి అబద్ధం;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
అంతఃవరుస తార్కిక isScalarLikeMat(స్థిరరాశి ఆదానం_Array_2& sc, పూర్ణాంకం cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    పూర్ణాంకం అడ్డువరుసలు, నిలువుల_సంఖ్య, nval;
    తార్కిక continuous;
    స్థిరరాశి ఆదానం_Array_2::రకం_Flag రకం = sc.రకం();
    ఒకవేళ (రకం == ఆదానం_Array_2::MAT)
    {
        స్థిరరాశి Mat& m = *(స్థిరరాశి Mat*)sc.getObj();
        ఒకవేళ ((m.అడ్డువరుసలు != 1 && m.నిలువుల_సంఖ్య != 1) || m.dims > 2) తిరిగి అబద్ధం;
        అడ్డువరుసలు = m.అడ్డువరుసలు; నిలువుల_సంఖ్య = m.నిలువుల_సంఖ్య; nval = అడ్డువరుసలు * నిలువుల_సంఖ్య * m.channels(); continuous = m.isContinuous();
    }
    లేకపోతే ఒకవేళ (రకం == ఆదానం_Array_2::UMAT)
    {
        స్థిరరాశి UMat& m = *(స్థిరరాశి UMat*)sc.getObj();
        ఒకవేళ ((m.అడ్డువరుసలు != 1 && m.నిలువుల_సంఖ్య != 1) || m.dims > 2) తిరిగి అబద్ధం;
        అడ్డువరుసలు = m.అడ్డువరుసలు; నిలువుల_సంఖ్య = m.నిలువుల_సంఖ్య; nval = అడ్డువరుసలు * నిలువుల_సంఖ్య * m.channels(); continuous = m.isContinuous();
    }
    లేకపోతే
        తిరిగి అబద్ధం;
    ఒకవేళ (!continuous || nval > 4 || (nval != 1 && nval != cn))
        తిరిగి అబద్ధం;
    తిరిగి (అడ్డువరుసలు == 1 && నిలువుల_సంఖ్య == 1) || (అడ్డువరుసలు == cn && నిలువుల_సంఖ్య == 1) || (అడ్డువరుసలు == 1 && నిలువుల_సంఖ్య == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
గణన { EW_SCALAR_BUF_కొలత = 4 * పరిమాణం(ద్విగుణ) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
అంతఃవరుస పూర్ణాంకం scalarArgElems(స్థిరరాశి ఆదానం_Array_2& sc, స్థిరరాశి uchar*& p, పూర్ణాంకం& d, uchar* scbuf)
{
    స్థిరరాశి ఆదానం_Array_2::రకం_Flag రకం = sc.రకం();
    ఒకవేళ (రకం == ఆదానం_Array_2::MAT)
    {
        స్థిరరాశి Mat& m = *(స్థిరరాశి Mat*)sc.getObj();
        p = m.దత్తాంశం;
        d = m.లోతు();
        తిరిగి (పూర్ణాంకం)m.మొత్తం() * m.channels();
    }
    ఒకవేళ (రకం == ఆదానం_Array_2::UMAT)
    {
        స్థిరరాశి UMat& u = *(స్థిరరాశి UMat*)sc.getObj();
        d = u.లోతు();
        పూర్ణాంకం n = (పూర్ణాంకం)u.మొత్తం() * u.channels();
        CV_Assert(n * (పూర్ణాంకం)CV_ELEM_SIZE1(d) <= (పూర్ణాంకం)EW_SCALAR_BUF_కొలత);
        Mat శీర్షిక(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copy_గమ్యం(శీర్షిక);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        తిరిగి n;
    }
    p = (స్థిరరాశి uchar*)sc.getObj();
    d = sc.లోతు();
    కొలత sz = sc.getSz();
    తిరిగి sz.వెడల్పు * sz.ఎత్తు;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
అంతఃవరుస ద్విగుణ elem_గమ్యం_Double(పూర్ణాంకం d, స్థిరరాశి uchar* p)
{
    ఎంపిక (d)
    {
    స్థితి CV_8U:   తిరిగి *(స్థిరరాశి uchar*)p;
    స్థితి CV_8S:   తిరిగి *(స్థిరరాశి schar*)p;
    స్థితి CV_16U:  తిరిగి *(స్థిరరాశి ushort*)p;
    స్థితి CV_16S:  తిరిగి *(స్థిరరాశి పొట్టి*)p;
    స్థితి CV_32U:  తిరిగి *(స్థిరరాశి గుర్తులేని*)p;
    స్థితి CV_32S:  తిరిగి *(స్థిరరాశి పూర్ణాంకం*)p;
    స్థితి CV_64U:  తిరిగి (ద్విగుణ)*(స్థిరరాశి uint64_t*)p;
    స్థితి CV_64S:  తిరిగి (ద్విగుణ)*(స్థిరరాశి int64_t*)p;
    స్థితి CV_16F:  తిరిగి (దశాంశం)*(స్థిరరాశి hfloat*)p;
    స్థితి CV_16BF: తిరిగి (దశాంశం)*(స్థిరరాశి bfloat*)p;
    స్థితి CV_32F:  తిరిగి *(స్థిరరాశి దశాంశం*)p;
    స్థితి CV_64F:  తిరిగి *(స్థిరరాశి ద్విగుణ*)p;
    అప్రమేయ:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
అంతఃవరుస పూర్ణాంకం readScalarArg(స్థిరరాశి ఆదానం_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    స్థిరరాశి uchar* p; పూర్ణాంకం d; uchar scbuf[EW_SCALAR_BUF_కొలత];
    పూర్ణాంకం n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    కొరకు (పూర్ణాంకం i = 0; i < n; i++)
        out[i] = elem_గమ్యం_Double(d, p + (size_t)i * esz);
    తిరిగి n;
}

శూన్య convertAndUnrollScalar( స్థిరరాశి Mat& sc, పూర్ణాంకం buftype, uchar* scbuf, size_t blocksize );

#నిర్వచితమైతే CV_COLLECT_IMPL_దత్తాంశం
నిర్మాణం ImplCollector
{
    ImplCollector()
    {
        useCollection   = అబద్ధం;
        implFlags       = 0;
    }
    తార్కిక useCollection; // enable/disable impl data collection

    పూర్ణాంకం implFlags;
    std::vector<పూర్ణాంకం>    impl_సంకేతం;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#అయితే_ముగింపు

నిర్మాణం CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(అబద్ధం), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    తార్కిక oclExecutionContextInitialized;
    పూర్ణాంకం useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    పూర్ణాంకం useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    పూర్ణాంకం useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& get_Core_Tls_దత్తాంశం();

#ఒకవేళ defined(BUILD_SHARED_LIBS)
#ఒకవేళ defined _WIN32 || defined WINCE
#నిర్వచించు CL_RUNTIME_EXPORT __declspec(dllexport)
#లేదావేళ defined __GNUC__
#నిర్వచించు CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#లేకపోతే
#నిర్వచించు CL_RUNTIME_EXPORT
#అయితే_ముగింపు
#లేకపోతే
#నిర్వచించు CL_RUNTIME_EXPORT
#అయితే_ముగింపు

బాహ్య CV_EXPORTS
తార్కిక __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#నిర్వచించు CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_విలువ) \
    స్థిర TYPE* స్థిరరాశి instance = INITIALIZER; \
    తిరిగి RET_విలువ;

#నిర్వచించు CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#నిర్వచించు CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS శూన్య releaseTlsStorageThread();

పూర్ణాంకం cv_snprintf(అక్షరం* buf, పూర్ణాంకం len, స్థిరరాశి అక్షరం* fmt, ...);
పూర్ణాంకం cv_vsnprintf(అక్షరం* buf, పూర్ణాంకం len, స్థిరరాశి అక్షరం* fmt, va_list వాదనలు);
}

#అయితే_ముగింపు  // BUILD_PLUGIN
#అయితే_ముగింపు  // __OPENCV_PRECOMP_H__
