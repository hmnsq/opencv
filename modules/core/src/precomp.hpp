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

#اگر_غیر_تعریف_شدہ __OPENCV_PRECOMP_H__
#تعریف_کرو __OPENCV_PRECOMP_H__

#اگر_تعریف_شدہ _MSC_VER
#  define SILENCE_CXX17_C_سرخی_DEPRECATION_WARNING
#  define SILENCE_CXX17_CODECVT_سرخی_DEPRECATION_WARNING
#اگر_ختم

#اگر_تعریف_شدہ BUILD_PLUGIN
#شامل "opencv2/core/utility.hpp"
#ورنہ  // BUILD_PLUGIN

#شامل "opencv2/opencv_modules.hpp"
#شامل "cvconfig.h"

#شامل "opencv2/core/utility.hpp"
#شامل "opencv2/core/cuda.hpp"
#شامل "opencv2/core/opengl.hpp"
#شامل "opencv2/core/va_intel.hpp"

#شامل "opencv2/core/private.hpp"
#شامل "opencv2/core/private.cuda.hpp"
#اگر_تعریف_شدہ HAVE_OPENCL
#شامل "opencv2/core/ocl.hpp"
#اگر_ختم

#شامل <ctype.h>
#شامل <float.h>
#شامل <limits.h>
#شامل <math.h>
#شامل <stdarg.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>

#شامل <algorithm>
#شامل <cmath>
#شامل <cstdlib>
#شامل <limits>
#شامل <float.h>
#شامل <cstring>
#شامل <cassert>

#تعریف_کرو USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#تعریف_کرو USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#تعریف_کرو USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#تعریف_کرو USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#شامل "opencv2/core/hal/hal.hpp"
#شامل "opencv2/core/hal/intrin.hpp"
#شامل "opencv2/core/neon_utils.hpp"
#شامل "opencv2/core/vsx_utils.hpp"
#شامل "hal_replacement.hpp"


نامخلا cv
{

// -128.f ... 255.f
خارجی ثابت اعشاری g_8x32fTab[];
#تعریف_کرو CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

خارجی ثابت ushort g_8x16uSqrTab[];
#تعریف_کرو CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

خارجی ثابت uchar g_Saturate8u[];
#تعریف_کرو CV_FAST_CAST_8U(t)   (أكد(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#تعریف_کرو CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#تعریف_کرو CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

سانچہ<قسم_نام T1, قسم_نام T2=T1, قسم_نام T3=T1> ساخت OpNop
{
    قسمتعریف T1 type1;
    قسمتعریف T2 type2;
    قسمتعریف T3 rtype;
    T3 عامل ()(ثابت T1 a) ثابت { واپس saturate_cast<T3>(a); }
};

سانچہ<قسم_نام T1, قسم_نام T2=T1, قسم_نام T3=T1> ساخت OpSqr
{
    قسمتعریف T1 type1;
    قسمتعریف T2 type2;
    قسمتعریف T3 rtype;
    T3 عامل ()(ثابت T1 a) ثابت { واپس saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

سانچہ<قسم_نام T1, قسم_نام T2=T1, قسم_نام T3=T1> ساخت Op_جوڑو
{
    قسمتعریف T1 type1;
    قسمتعریف T2 type2;
    قسمتعریف T3 rtype;
    T3 عامل ()(ثابت T1 a, ثابت T2 b) ثابت { واپس saturate_cast<T3>(a + b); }
};

سانچہ<قسم_نام T1, قسم_نام T2=T1, قسم_نام T3=T1> ساخت Op_جوڑو_Sqr
{
    قسمتعریف T1 type1;
    قسمتعریف T2 type2;
    قسمتعریف T3 rtype;
    T3 عامل ()(ثابت T1 a, ثابت T2 b) ثابت { واپس saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

سانچہ<قسم_نام T1, قسم_نام T2=T1, قسم_نام T3=T1> ساخت OpSub
{
    قسمتعریف T1 type1;
    قسمتعریف T2 type2;
    قسمتعریف T3 rtype;
    T3 عامل ()(ثابت T1 a, ثابت T2 b) ثابت { واپس saturate_cast<T3>(a - b); }
};

سانچہ<قسم_نام T1, قسم_نام T2=T1, قسم_نام T3=T1> ساخت OpRSub
{
    قسمتعریف T1 type1;
    قسمتعریف T2 type2;
    قسمتعریف T3 rtype;
    T3 عامل ()(ثابت T1 a, ثابت T2 b) ثابت { واپس saturate_cast<T3>(b - a); }
};

سانچہ<قسم_نام T> ساخت OpMin
{
    قسمتعریف T type1;
    قسمتعریف T type2;
    قسمتعریف T rtype;
    T عامل ()(ثابت T a, ثابت T b) ثابت { واپس std::min(a, b); }
};

سانچہ<قسم_نام T> ساخت OpMax
{
    قسمتعریف T type1;
    قسمتعریف T type2;
    قسمتعریف T rtype;
    T عامل ()(ثابت T a, ثابت T b) ثابت { واپس std::max(a, b); }
};

سانچہ<قسم_نام T> ساخت OpAbsDiff
{
    قسمتعریف T type1;
    قسمتعریف T type2;
    قسمتعریف T rtype;
    T عامل()(T a, T b) ثابت { واپس a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
سانچہ<> ساخت OpAbsDiff<اعشاری>
{
    قسمتعریف اعشاری type1;
    قسمتعریف اعشاری type2;
    قسمتعریف اعشاری rtype;
    اعشاری عامل()(اعشاری a, اعشاری b) ثابت { واپس std::مطلق_قدر(a - b); }
};
سانچہ<> ساخت OpAbsDiff<دوہرا>
{
    قسمتعریف دوہرا type1;
    قسمتعریف دوہرا type2;
    قسمتعریف دوہرا rtype;
    دوہرا عامل()(دوہرا a, دوہرا b) ثابت { واپس std::مطلق_قدر(a - b); }
};

سانچہ<قسم_نام T> ساخت OpAnd
{
    قسمتعریف T type1;
    قسمتعریف T type2;
    قسمتعریف T rtype;
    T عامل()( T a, T b ) ثابت { واپس a & b; }
};

سانچہ<قسم_نام T> ساخت OpOr
{
    قسمتعریف T type1;
    قسمتعریف T type2;
    قسمتعریف T rtype;
    T عامل()( T a, T b ) ثابت { واپس a | b; }
};

سانچہ<قسم_نام T> ساخت OpXor
{
    قسمتعریف T type1;
    قسمتعریف T type2;
    قسمتعریف T rtype;
    T عامل()( T a, T b ) ثابت { واپس a ^ b; }
};

سانچہ<قسم_نام T> ساخت OpNot
{
    قسمتعریف T type1;
    قسمتعریف T type2;
    قسمتعریف T rtype;
    T عامل()( T a, T ) ثابت { واپس ~a; }
};

سانچہ<> اندرونی uchar Op_جوڑو<uchar>::عامل ()(uchar a, uchar b) ثابت
{ واپس CV_FAST_CAST_8U(a + b); }

سانچہ<> اندرونی uchar OpSub<uchar>::عامل ()(uchar a, uchar b) ثابت
{ واپس CV_FAST_CAST_8U(a - b); }

سانچہ<> اندرونی مختصر OpAbsDiff<مختصر>::عامل ()(مختصر a, مختصر b) ثابت
{ واپس saturate_cast<مختصر>(std::مطلق_قدر(a - b)); }

سانچہ<> اندرونی schar OpAbsDiff<schar>::عامل ()(schar a, schar b) ثابت
{ واپس saturate_cast<schar>(std::مطلق_قدر(a - b)); }

سانچہ<> اندرونی uchar OpMin<uchar>::عامل ()(uchar a, uchar b) ثابت { واپس CV_MIN_8U(a, b); }

سانچہ<> اندرونی uchar OpMax<uchar>::عامل ()(uchar a, uchar b) ثابت { واپس CV_MAX_8U(a, b); }

قسمتعریف خالی (*UnaryFunc)(ثابت uchar* src1, size_t step1,
                       uchar* dst, size_t قدم, سائز sz,
                       خالی*);

قسمتعریف خالی (*ثنائی_Func)(ثابت uchar* src1, size_t step1,
                       ثابت uchar* src2, size_t step2,
                       uchar* dst, size_t قدم, سائز sz,
                       خالی*);

قسمتعریف خالی (*ثنائی_Func_C)(ثابت خالی* src1, size_t step1,
                       ثابت خالی* src2, size_t step2,
                       خالی* dst, size_t قدم, صحیح چوڑائی, صحیح اونچائی,
                       خالی*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS ثنائی_Func getConvertFunc(صحیح sdepth, صحیح ddepth);
CV_EXPORTS ثنائی_Func getConvertScaleFunc(صحیح sdepth, صحیح ddepth);
ثنائی_Func get_Copy_نقاب_Func(size_t esz);

/* default memory block for sparse array elements */
#تعریف_کرو  CV_SPARSE_MAT_بلاک     (1<<12)

/* initial hash table size */
#تعریف_کرو  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#تعریف_کرو  CV_SPARSE_HASH_تناسب    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
سائز getContinuousSize2D(Mat& m1, صحیح چوڑائی_Scale=1);
سائز getContinuousSize2D(Mat& m1, Mat& m2, صحیح چوڑائی_Scale=1);
سائز getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, صحیح چوڑائی_Scale=1);

خالی سیٹ_سائز( Mat& m, صحیح _dims, ثابت صحیح* _sz, ثابت size_t* اقدام_2, منطقی auto_اقدام=جھوٹ );
خالی finalizeHdr(Mat& m);
صحیح updateContinuityFlag(صحیح flags, صحیح dims, ثابت صحیح* size, ثابت size_t* قدم);

ساخت NoVec
{
    size_t عامل()(ثابت خالی*, ثابت خالی*, خالی*, size_t) ثابت { واپس 0; }
};

#تعریف_کرو CV_بانٹو_ملاؤ_MAX_بلاک_سائز(cn) ((زیادہ_سے_زیادہ_عدد/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

شمار { بلاک_سائز = 1024 };

#اگر defined HAVE_IPP && (IPP_نسخہ_X100 >= 700)
#تعریف_کرو ARITHM_USE_IPP 1
#ورنہ
#تعریف_کرو ARITHM_USE_IPP 0
#اگر_ختم

اندرونی منطقی checkScalar(ثابت Mat& sc, صحیح atype, داخلہ_Array_2::قسم_Flag sckind, داخلہ_Array_2::قسم_Flag akind)
{
    اگر( sc.dims > 2 || !sc.isContinuous() )
        واپس جھوٹ;
    سائز sz = sc.size();
    اگر(sz.چوڑائی != 1 && sz.اونچائی != 1)
        واپس جھوٹ;
    صحیح cn = CV_MAT_CN(atype);
    اگر( akind == داخلہ_Array_2::MATX && sckind != داخلہ_Array_2::MATX )
        واپس جھوٹ;
    واپس sz == سائز(1, 1) || sz == سائز(1, cn) || sz == سائز(cn, 1) ||
           (sz == سائز(1, 4) && sc.type() == CV_64F && cn <= 4);
}

اندرونی منطقی checkScalar(داخلہ_Array sc, صحیح atype, داخلہ_Array_2::قسم_Flag sckind, داخلہ_Array_2::قسم_Flag akind)
{
    اگر( sc.dims() > 2 || !sc.isContinuous() )
        واپس جھوٹ;
    سائز sz = sc.size();
    اگر(sz.چوڑائی != 1 && sz.اونچائی != 1)
        واپس جھوٹ;
    صحیح cn = CV_MAT_CN(atype);
    اگر( akind == داخلہ_Array_2::MATX && sckind != داخلہ_Array_2::MATX )
        واپس جھوٹ;
    واپس sz == سائز(1, 1) || sz == سائز(1, cn) || sz == سائز(cn, 1) ||
           (sz == سائز(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
اندرونی منطقی sameShapeAsArg(ثابت داخلہ_Array_2& sc, ثابت داخلہ_Array_2& arr)
{
    واپس arr.channels() == 1 && sc.same_سائز(arr);
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
اندرونی منطقی isScalarArg(ثابت داخلہ_Array_2& sc, ثابت داخلہ_Array_2& arr)
{
    ثابت داخلہ_Array_2::قسم_Flag قسم = sc.قسم();
    ثابت صحیح cn = arr.channels();
    اگر (قسم == داخلہ_Array_2::MATX)
    {
        سائز sz = sc.getSz();
        صحیح scn0 = sz.چوڑائی * sz.اونچائی;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        اگر (scn0 != sz.چوڑائی + sz.اونچائی - 1)
            واپس جھوٹ;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        واپس scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    اگر (قسم == داخلہ_Array_2::MAT)
    {
        ثابت Mat& m = *(ثابت Mat*)sc.getObj();
        واپس m.صفیں == 4 && m.ستون_تعداد == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    اگر (قسم == داخلہ_Array_2::UMAT)
    {
        ثابت UMat& m = *(ثابت UMat*)sc.getObj();
        واپس m.صفیں == 4 && m.ستون_تعداد == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    واپس جھوٹ;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
اندرونی منطقی isScalarLikeMat(ثابت داخلہ_Array_2& sc, صحیح cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    صحیح صفیں, ستون_تعداد, nval;
    منطقی continuous;
    ثابت داخلہ_Array_2::قسم_Flag قسم = sc.قسم();
    اگر (قسم == داخلہ_Array_2::MAT)
    {
        ثابت Mat& m = *(ثابت Mat*)sc.getObj();
        اگر ((m.صفیں != 1 && m.ستون_تعداد != 1) || m.dims > 2) واپس جھوٹ;
        صفیں = m.صفیں; ستون_تعداد = m.ستون_تعداد; nval = صفیں * ستون_تعداد * m.channels(); continuous = m.isContinuous();
    }
    ورنہ اگر (قسم == داخلہ_Array_2::UMAT)
    {
        ثابت UMat& m = *(ثابت UMat*)sc.getObj();
        اگر ((m.صفیں != 1 && m.ستون_تعداد != 1) || m.dims > 2) واپس جھوٹ;
        صفیں = m.صفیں; ستون_تعداد = m.ستون_تعداد; nval = صفیں * ستون_تعداد * m.channels(); continuous = m.isContinuous();
    }
    ورنہ
        واپس جھوٹ;
    اگر (!continuous || nval > 4 || (nval != 1 && nval != cn))
        واپس جھوٹ;
    واپس (صفیں == 1 && ستون_تعداد == 1) || (صفیں == cn && ستون_تعداد == 1) || (صفیں == 1 && ستون_تعداد == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
شمار { EW_SCALAR_BUF_سائز = 4 * حجم(دوہرا) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
اندرونی صحیح scalarArgElems(ثابت داخلہ_Array_2& sc, ثابت uchar*& p, صحیح& d, uchar* scbuf)
{
    ثابت داخلہ_Array_2::قسم_Flag قسم = sc.قسم();
    اگر (قسم == داخلہ_Array_2::MAT)
    {
        ثابت Mat& m = *(ثابت Mat*)sc.getObj();
        p = m.ڈیٹا;
        d = m.گہرائی();
        واپس (صحیح)m.کل() * m.channels();
    }
    اگر (قسم == داخلہ_Array_2::UMAT)
    {
        ثابت UMat& u = *(ثابت UMat*)sc.getObj();
        d = u.گہرائی();
        صحیح n = (صحیح)u.کل() * u.channels();
        CV_Assert(n * (صحیح)CV_ELEM_SIZE1(d) <= (صحیح)EW_SCALAR_BUF_سائز);
        Mat سرخی(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copy_مقصد(سرخی);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        واپس n;
    }
    p = (ثابت uchar*)sc.getObj();
    d = sc.گہرائی();
    سائز sz = sc.getSz();
    واپس sz.چوڑائی * sz.اونچائی;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
اندرونی دوہرا elem_مقصد_Double(صحیح d, ثابت uchar* p)
{
    انتخاب (d)
    {
    حالت CV_8U:   واپس *(ثابت uchar*)p;
    حالت CV_8S:   واپس *(ثابت schar*)p;
    حالت CV_16U:  واپس *(ثابت ushort*)p;
    حالت CV_16S:  واپس *(ثابت مختصر*)p;
    حالت CV_32U:  واپس *(ثابت غیرعلامتی*)p;
    حالت CV_32S:  واپس *(ثابت صحیح*)p;
    حالت CV_64U:  واپس (دوہرا)*(ثابت uint64_t*)p;
    حالت CV_64S:  واپس (دوہرا)*(ثابت int64_t*)p;
    حالت CV_16F:  واپس (اعشاری)*(ثابت hfloat*)p;
    حالت CV_16BF: واپس (اعشاری)*(ثابت bfloat*)p;
    حالت CV_32F:  واپس *(ثابت اعشاری*)p;
    حالت CV_64F:  واپس *(ثابت دوہرا*)p;
    طےشدہ:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
اندرونی صحیح readScalarArg(ثابت داخلہ_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    ثابت uchar* p; صحیح d; uchar scbuf[EW_SCALAR_BUF_سائز];
    صحیح n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    برائے (صحیح i = 0; i < n; i++)
        out[i] = elem_مقصد_Double(d, p + (size_t)i * esz);
    واپس n;
}

خالی convertAndUnrollScalar( ثابت Mat& sc, صحیح buftype, uchar* scbuf, size_t blocksize );

#اگر_تعریف_شدہ CV_COLLECT_IMPL_ڈیٹا
ساخت ImplCollector
{
    ImplCollector()
    {
        useCollection   = جھوٹ;
        implFlags       = 0;
    }
    منطقی useCollection; // enable/disable impl data collection

    صحیح implFlags;
    std::vector<صحیح>    impl_رمز;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#اگر_ختم

ساخت CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(جھوٹ), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    منطقی oclExecutionContextInitialized;
    صحیح useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    صحیح useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    صحیح useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& get_Core_Tls_ڈیٹا();

#اگر defined(BUILD_SHARED_LIBS)
#اگر defined _WIN32 || defined WINCE
#تعریف_کرو CL_RUNTIME_EXPORT __declspec(dllexport)
#وگرنہ_اگر defined __GNUC__
#تعریف_کرو CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#ورنہ
#تعریف_کرو CL_RUNTIME_EXPORT
#اگر_ختم
#ورنہ
#تعریف_کرو CL_RUNTIME_EXPORT
#اگر_ختم

خارجی CV_EXPORTS
منطقی __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#تعریف_کرو CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_قدر) \
    ساکن TYPE* ثابت instance = INITIALIZER; \
    واپس RET_قدر;

#تعریف_کرو CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#تعریف_کرو CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS خالی releaseTlsStorageThread();

صحیح cv_snprintf(حرف* buf, صحیح len, ثابت حرف* fmt, ...);
صحیح cv_vsnprintf(حرف* buf, صحیح len, ثابت حرف* fmt, va_list دلائل);
}

#اگر_ختم  // BUILD_PLUGIN
#اگر_ختم  // __OPENCV_PRECOMP_H__
