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

#اگر_تعریف_نشده __OPENCV_PRECOMP_H__
#تعریف_کن __OPENCV_PRECOMP_H__

#اگر_تعریف_شده _MSC_VER
#  define SILENCE_CXX17_C_سرآیند_DEPRECATION_WARNING
#  define SILENCE_CXX17_CODECVT_سرآیند_DEPRECATION_WARNING
#پایان_اگر

#اگر_تعریف_شده BUILD_PLUGIN
#ضمن "opencv2/core/utility.hpp"
#وإلا  // BUILD_PLUGIN

#ضمن "opencv2/opencv_modules.hpp"
#ضمن "cvconfig.h"

#ضمن "opencv2/core/utility.hpp"
#ضمن "opencv2/core/cuda.hpp"
#ضمن "opencv2/core/opengl.hpp"
#ضمن "opencv2/core/va_intel.hpp"

#ضمن "opencv2/core/private.hpp"
#ضمن "opencv2/core/private.cuda.hpp"
#اگر_تعریف_شده HAVE_OPENCL
#ضمن "opencv2/core/ocl.hpp"
#پایان_اگر

#ضمن <ctype.h>
#ضمن <float.h>
#ضمن <limits.h>
#ضمن <math.h>
#ضمن <stdarg.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>

#ضمن <algorithm>
#ضمن <cmath>
#ضمن <cstdlib>
#ضمن <limits>
#ضمن <float.h>
#ضمن <cstring>
#ضمن <cassert>

#تعریف_کن USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#تعریف_کن USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#تعریف_کن USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#تعریف_کن USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#ضمن "opencv2/core/hal/hal.hpp"
#ضمن "opencv2/core/hal/intrin.hpp"
#ضمن "opencv2/core/neon_utils.hpp"
#ضمن "opencv2/core/vsx_utils.hpp"
#ضمن "hal_replacement.hpp"


نطاق cv
{

// -128.f ... 255.f
خارجي ثابت عائم g_8x32fTab[];
#تعریف_کن CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

خارجي ثابت ushort g_8x16uSqrTab[];
#تعریف_کن CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

خارجي ثابت uchar g_Saturate8u[];
#تعریف_کن CV_FAST_CAST_8U(t)   (أكد(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#تعریف_کن CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#تعریف_کن CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

قالب<نام_نوع T1, نام_نوع T2=T1, نام_نوع T3=T1> بنية OpNop
{
    تعريفنوع T1 type1;
    تعريفنوع T2 type2;
    تعريفنوع T3 rtype;
    T3 عملگر ()(ثابت T1 a) ثابت { أرجع saturate_cast<T3>(a); }
};

قالب<نام_نوع T1, نام_نوع T2=T1, نام_نوع T3=T1> بنية OpSqr
{
    تعريفنوع T1 type1;
    تعريفنوع T2 type2;
    تعريفنوع T3 rtype;
    T3 عملگر ()(ثابت T1 a) ثابت { أرجع saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

قالب<نام_نوع T1, نام_نوع T2=T1, نام_نوع T3=T1> بنية Op_جمع_کن
{
    تعريفنوع T1 type1;
    تعريفنوع T2 type2;
    تعريفنوع T3 rtype;
    T3 عملگر ()(ثابت T1 a, ثابت T2 b) ثابت { أرجع saturate_cast<T3>(a + b); }
};

قالب<نام_نوع T1, نام_نوع T2=T1, نام_نوع T3=T1> بنية Op_جمع_کن_Sqr
{
    تعريفنوع T1 type1;
    تعريفنوع T2 type2;
    تعريفنوع T3 rtype;
    T3 عملگر ()(ثابت T1 a, ثابت T2 b) ثابت { أرجع saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

قالب<نام_نوع T1, نام_نوع T2=T1, نام_نوع T3=T1> بنية OpSub
{
    تعريفنوع T1 type1;
    تعريفنوع T2 type2;
    تعريفنوع T3 rtype;
    T3 عملگر ()(ثابت T1 a, ثابت T2 b) ثابت { أرجع saturate_cast<T3>(a - b); }
};

قالب<نام_نوع T1, نام_نوع T2=T1, نام_نوع T3=T1> بنية OpRSub
{
    تعريفنوع T1 type1;
    تعريفنوع T2 type2;
    تعريفنوع T3 rtype;
    T3 عملگر ()(ثابت T1 a, ثابت T2 b) ثابت { أرجع saturate_cast<T3>(b - a); }
};

قالب<نام_نوع T> بنية OpMin
{
    تعريفنوع T type1;
    تعريفنوع T type2;
    تعريفنوع T rtype;
    T عملگر ()(ثابت T a, ثابت T b) ثابت { أرجع std::min(a, b); }
};

قالب<نام_نوع T> بنية OpMax
{
    تعريفنوع T type1;
    تعريفنوع T type2;
    تعريفنوع T rtype;
    T عملگر ()(ثابت T a, ثابت T b) ثابت { أرجع std::max(a, b); }
};

قالب<نام_نوع T> بنية OpAbsDiff
{
    تعريفنوع T type1;
    تعريفنوع T type2;
    تعريفنوع T rtype;
    T عملگر()(T a, T b) ثابت { أرجع a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
قالب<> بنية OpAbsDiff<عائم>
{
    تعريفنوع عائم type1;
    تعريفنوع عائم type2;
    تعريفنوع عائم rtype;
    عائم عملگر()(عائم a, عائم b) ثابت { أرجع std::قدر_مطلق(a - b); }
};
قالب<> بنية OpAbsDiff<مزدوج>
{
    تعريفنوع مزدوج type1;
    تعريفنوع مزدوج type2;
    تعريفنوع مزدوج rtype;
    مزدوج عملگر()(مزدوج a, مزدوج b) ثابت { أرجع std::قدر_مطلق(a - b); }
};

قالب<نام_نوع T> بنية OpAnd
{
    تعريفنوع T type1;
    تعريفنوع T type2;
    تعريفنوع T rtype;
    T عملگر()( T a, T b ) ثابت { أرجع a & b; }
};

قالب<نام_نوع T> بنية OpOr
{
    تعريفنوع T type1;
    تعريفنوع T type2;
    تعريفنوع T rtype;
    T عملگر()( T a, T b ) ثابت { أرجع a | b; }
};

قالب<نام_نوع T> بنية OpXor
{
    تعريفنوع T type1;
    تعريفنوع T type2;
    تعريفنوع T rtype;
    T عملگر()( T a, T b ) ثابت { أرجع a ^ b; }
};

قالب<نام_نوع T> بنية OpNot
{
    تعريفنوع T type1;
    تعريفنوع T type2;
    تعريفنوع T rtype;
    T عملگر()( T a, T ) ثابت { أرجع ~a; }
};

قالب<> مضمن uchar Op_جمع_کن<uchar>::عملگر ()(uchar a, uchar b) ثابت
{ أرجع CV_FAST_CAST_8U(a + b); }

قالب<> مضمن uchar OpSub<uchar>::عملگر ()(uchar a, uchar b) ثابت
{ أرجع CV_FAST_CAST_8U(a - b); }

قالب<> مضمن کوتاه OpAbsDiff<کوتاه>::عملگر ()(کوتاه a, کوتاه b) ثابت
{ أرجع saturate_cast<کوتاه>(std::قدر_مطلق(a - b)); }

قالب<> مضمن schar OpAbsDiff<schar>::عملگر ()(schar a, schar b) ثابت
{ أرجع saturate_cast<schar>(std::قدر_مطلق(a - b)); }

قالب<> مضمن uchar OpMin<uchar>::عملگر ()(uchar a, uchar b) ثابت { أرجع CV_MIN_8U(a, b); }

قالب<> مضمن uchar OpMax<uchar>::عملگر ()(uchar a, uchar b) ثابت { أرجع CV_MAX_8U(a, b); }

تعريفنوع فراغ (*UnaryFunc)(ثابت uchar* src1, size_t step1,
                       uchar* dst, size_t گام, اندازه sz,
                       فراغ*);

تعريفنوع فراغ (*دودویی_Func)(ثابت uchar* src1, size_t step1,
                       ثابت uchar* src2, size_t step2,
                       uchar* dst, size_t گام, اندازه sz,
                       فراغ*);

تعريفنوع فراغ (*دودویی_Func_C)(ثابت فراغ* src1, size_t step1,
                       ثابت فراغ* src2, size_t step2,
                       فراغ* dst, size_t گام, صحيح عرض, صحيح ارتفاع,
                       فراغ*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS دودویی_Func getConvertFunc(صحيح sdepth, صحيح ddepth);
CV_EXPORTS دودویی_Func getConvertScaleFunc(صحيح sdepth, صحيح ddepth);
دودویی_Func get_Copy_ماسک_Func(size_t esz);

/* default memory block for sparse array elements */
#تعریف_کن  CV_SPARSE_MAT_بلوک     (1<<12)

/* initial hash table size */
#تعریف_کن  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#تعریف_کن  CV_SPARSE_HASH_نسبت    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
اندازه getContinuousSize2D(Mat& m1, صحيح عرض_Scale=1);
اندازه getContinuousSize2D(Mat& m1, Mat& m2, صحيح عرض_Scale=1);
اندازه getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, صحيح عرض_Scale=1);

فراغ مجموعه_اندازه( Mat& m, صحيح _dims, ثابت صحيح* _sz, ثابت size_t* گامها_2, منطقي auto_گامها=خطأ );
فراغ finalizeHdr(Mat& m);
صحيح updateContinuityFlag(صحيح flags, صحيح dims, ثابت صحيح* size, ثابت size_t* گام);

بنية NoVec
{
    size_t عملگر()(ثابت فراغ*, ثابت فراغ*, فراغ*, size_t) ثابت { أرجع 0; }
};

#تعریف_کن CV_تقسیم_ادغام_MAX_بلوک_اندازه(cn) ((بیشترین_صحیح/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

تعداد { بلوک_اندازه = 1024 };

#إذا defined HAVE_IPP && (IPP_نسخه_X100 >= 700)
#تعریف_کن ARITHM_USE_IPP 1
#وإلا
#تعریف_کن ARITHM_USE_IPP 0
#پایان_اگر

مضمن منطقي checkScalar(ثابت Mat& sc, صحيح atype, درون_داد_Array_2::نوع_Flag sckind, درون_داد_Array_2::نوع_Flag akind)
{
    إذا( sc.dims > 2 || !sc.isContinuous() )
        أرجع خطأ;
    اندازه sz = sc.size();
    إذا(sz.عرض != 1 && sz.ارتفاع != 1)
        أرجع خطأ;
    صحيح cn = CV_MAT_CN(atype);
    إذا( akind == درون_داد_Array_2::MATX && sckind != درون_داد_Array_2::MATX )
        أرجع خطأ;
    أرجع sz == اندازه(1, 1) || sz == اندازه(1, cn) || sz == اندازه(cn, 1) ||
           (sz == اندازه(1, 4) && sc.type() == CV_64F && cn <= 4);
}

مضمن منطقي checkScalar(درون_داد_Array sc, صحيح atype, درون_داد_Array_2::نوع_Flag sckind, درون_داد_Array_2::نوع_Flag akind)
{
    إذا( sc.dims() > 2 || !sc.isContinuous() )
        أرجع خطأ;
    اندازه sz = sc.size();
    إذا(sz.عرض != 1 && sz.ارتفاع != 1)
        أرجع خطأ;
    صحيح cn = CV_MAT_CN(atype);
    إذا( akind == درون_داد_Array_2::MATX && sckind != درون_داد_Array_2::MATX )
        أرجع خطأ;
    أرجع sz == اندازه(1, 1) || sz == اندازه(1, cn) || sz == اندازه(cn, 1) ||
           (sz == اندازه(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
مضمن منطقي sameShapeAsArg(ثابت درون_داد_Array_2& sc, ثابت درون_داد_Array_2& arr)
{
    أرجع arr.channels() == 1 && sc.same_اندازه(arr);
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
مضمن منطقي isScalarArg(ثابت درون_داد_Array_2& sc, ثابت درون_داد_Array_2& arr)
{
    ثابت درون_داد_Array_2::نوع_Flag نوع = sc.نوع();
    ثابت صحيح cn = arr.channels();
    إذا (نوع == درون_داد_Array_2::MATX)
    {
        اندازه sz = sc.getSz();
        صحيح scn0 = sz.عرض * sz.ارتفاع;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        إذا (scn0 != sz.عرض + sz.ارتفاع - 1)
            أرجع خطأ;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        أرجع scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    إذا (نوع == درون_داد_Array_2::MAT)
    {
        ثابت Mat& m = *(ثابت Mat*)sc.getObj();
        أرجع m.ردیفها == 4 && m.تعداد_ستون == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    إذا (نوع == درون_داد_Array_2::UMAT)
    {
        ثابت UMat& m = *(ثابت UMat*)sc.getObj();
        أرجع m.ردیفها == 4 && m.تعداد_ستون == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    أرجع خطأ;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
مضمن منطقي isScalarLikeMat(ثابت درون_داد_Array_2& sc, صحيح cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    صحيح ردیفها, تعداد_ستون, nval;
    منطقي continuous;
    ثابت درون_داد_Array_2::نوع_Flag نوع = sc.نوع();
    إذا (نوع == درون_داد_Array_2::MAT)
    {
        ثابت Mat& m = *(ثابت Mat*)sc.getObj();
        إذا ((m.ردیفها != 1 && m.تعداد_ستون != 1) || m.dims > 2) أرجع خطأ;
        ردیفها = m.ردیفها; تعداد_ستون = m.تعداد_ستون; nval = ردیفها * تعداد_ستون * m.channels(); continuous = m.isContinuous();
    }
    وإلا إذا (نوع == درون_داد_Array_2::UMAT)
    {
        ثابت UMat& m = *(ثابت UMat*)sc.getObj();
        إذا ((m.ردیفها != 1 && m.تعداد_ستون != 1) || m.dims > 2) أرجع خطأ;
        ردیفها = m.ردیفها; تعداد_ستون = m.تعداد_ستون; nval = ردیفها * تعداد_ستون * m.channels(); continuous = m.isContinuous();
    }
    وإلا
        أرجع خطأ;
    إذا (!continuous || nval > 4 || (nval != 1 && nval != cn))
        أرجع خطأ;
    أرجع (ردیفها == 1 && تعداد_ستون == 1) || (ردیفها == cn && تعداد_ستون == 1) || (ردیفها == 1 && تعداد_ستون == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
تعداد { EW_SCALAR_BUF_اندازه = 4 * حجم(مزدوج) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
مضمن صحيح scalarArgElems(ثابت درون_داد_Array_2& sc, ثابت uchar*& p, صحيح& d, uchar* scbuf)
{
    ثابت درون_داد_Array_2::نوع_Flag نوع = sc.نوع();
    إذا (نوع == درون_داد_Array_2::MAT)
    {
        ثابت Mat& m = *(ثابت Mat*)sc.getObj();
        p = m.داده;
        d = m.عمق();
        أرجع (صحيح)m.مجموع() * m.channels();
    }
    إذا (نوع == درون_داد_Array_2::UMAT)
    {
        ثابت UMat& u = *(ثابت UMat*)sc.getObj();
        d = u.عمق();
        صحيح n = (صحيح)u.مجموع() * u.channels();
        CV_Assert(n * (صحيح)CV_ELEM_SIZE1(d) <= (صحيح)EW_SCALAR_BUF_اندازه);
        Mat سرآیند(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copy_مقصد(سرآیند);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        أرجع n;
    }
    p = (ثابت uchar*)sc.getObj();
    d = sc.عمق();
    اندازه sz = sc.getSz();
    أرجع sz.عرض * sz.ارتفاع;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
مضمن مزدوج elem_مقصد_Double(صحيح d, ثابت uchar* p)
{
    بدل (d)
    {
    حالة CV_8U:   أرجع *(ثابت uchar*)p;
    حالة CV_8S:   أرجع *(ثابت schar*)p;
    حالة CV_16U:  أرجع *(ثابت ushort*)p;
    حالة CV_16S:  أرجع *(ثابت کوتاه*)p;
    حالة CV_32U:  أرجع *(ثابت بیعلامت*)p;
    حالة CV_32S:  أرجع *(ثابت صحيح*)p;
    حالة CV_64U:  أرجع (مزدوج)*(ثابت uint64_t*)p;
    حالة CV_64S:  أرجع (مزدوج)*(ثابت int64_t*)p;
    حالة CV_16F:  أرجع (عائم)*(ثابت hfloat*)p;
    حالة CV_16BF: أرجع (عائم)*(ثابت bfloat*)p;
    حالة CV_32F:  أرجع *(ثابت عائم*)p;
    حالة CV_64F:  أرجع *(ثابت مزدوج*)p;
    افتراضي:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
مضمن صحيح readScalarArg(ثابت درون_داد_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    ثابت uchar* p; صحيح d; uchar scbuf[EW_SCALAR_BUF_اندازه];
    صحيح n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    لكل (صحيح i = 0; i < n; i++)
        out[i] = elem_مقصد_Double(d, p + (size_t)i * esz);
    أرجع n;
}

فراغ convertAndUnrollScalar( ثابت Mat& sc, صحيح buftype, uchar* scbuf, size_t blocksize );

#اگر_تعریف_شده CV_COLLECT_IMPL_داده
بنية ImplCollector
{
    ImplCollector()
    {
        useCollection   = خطأ;
        implFlags       = 0;
    }
    منطقي useCollection; // enable/disable impl data collection

    صحيح implFlags;
    std::vector<صحيح>    impl_کد;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#پایان_اگر

بنية CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(خطأ), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    منطقي oclExecutionContextInitialized;
    صحيح useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    صحيح useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    صحيح useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& get_Core_Tls_داده();

#إذا defined(BUILD_SHARED_LIBS)
#إذا defined _WIN32 || defined WINCE
#تعریف_کن CL_RUNTIME_EXPORT __declspec(dllexport)
#وإلاإذا defined __GNUC__
#تعریف_کن CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#وإلا
#تعریف_کن CL_RUNTIME_EXPORT
#پایان_اگر
#وإلا
#تعریف_کن CL_RUNTIME_EXPORT
#پایان_اگر

خارجي CV_EXPORTS
منطقي __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#تعریف_کن CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_مقدار) \
    ساكن TYPE* ثابت instance = INITIALIZER; \
    أرجع RET_مقدار;

#تعریف_کن CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#تعریف_کن CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS فراغ releaseTlsStorageThread();

صحيح cv_snprintf(حرف* buf, صحيح len, ثابت حرف* fmt, ...);
صحيح cv_vsnprintf(حرف* buf, صحيح len, ثابت حرف* fmt, va_list آرگومانها);
}

#پایان_اگر  // BUILD_PLUGIN
#پایان_اگر  // __OPENCV_PRECOMP_H__
