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

#ifndef __OPENCV_PRECOMP_H__
#define __OPENCV_PRECOMP_H__

#ifdef _MSC_VER
#  define _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING
#  define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#endif

#ifdef BUILD_PLUGIN
#include "opencv2/core/utility.hpp"
#否则  // BUILD_PLUGIN

#include "opencv2/opencv_modules.hpp"
#include "cvconfig.h"

#include "opencv2/core/utility.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/core/opengl.hpp"
#include "opencv2/core/va_intel.hpp"

#include "opencv2/core/private.hpp"
#include "opencv2/core/private.cuda.hpp"
#ifdef HAVE_OPENCL
#include "opencv2/core/ocl.hpp"
#endif

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <float.h>
#include <cstring>
#include <cassert>

#define USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#define USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#define USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#define USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#include "opencv2/core/hal/hal.hpp"
#include "opencv2/core/hal/intrin.hpp"
#include "opencv2/core/neon_utils.hpp"
#include "opencv2/core/vsx_utils.hpp"
#include "hal_replacement.hpp"


namespace cv
{

// -128.f ... 255.f
extern const float g_8x32fTab[];
#define CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

extern const ushort g_8x16uSqrTab[];
#define CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

extern const uchar g_Saturate8u[];
#define CV_FAST_CAST_8U(t)   (assert(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#define CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#define CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

template<typename T1, typename T2=T1, typename T3=T1> struct OpNop
{
    typedef T1 type1;
    typedef T2 type2;
    typedef T3 rtype;
    T3 operator ()(const T1 a) const { 返回 saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> struct OpSqr
{
    typedef T1 type1;
    typedef T2 type2;
    typedef T3 rtype;
    T3 operator ()(const T1 a) const { 返回 saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> struct OpAdd
{
    typedef T1 type1;
    typedef T2 type2;
    typedef T3 rtype;
    T3 operator ()(const T1 a, const T2 b) const { 返回 saturate_cast<T3>(a + b); }
};

template<typename T1, typename T2=T1, typename T3=T1> struct OpAddSqr
{
    typedef T1 type1;
    typedef T2 type2;
    typedef T3 rtype;
    T3 operator ()(const T1 a, const T2 b) const { 返回 saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

template<typename T1, typename T2=T1, typename T3=T1> struct OpSub
{
    typedef T1 type1;
    typedef T2 type2;
    typedef T3 rtype;
    T3 operator ()(const T1 a, const T2 b) const { 返回 saturate_cast<T3>(a - b); }
};

template<typename T1, typename T2=T1, typename T3=T1> struct OpRSub
{
    typedef T1 type1;
    typedef T2 type2;
    typedef T3 rtype;
    T3 operator ()(const T1 a, const T2 b) const { 返回 saturate_cast<T3>(b - a); }
};

template<typename T> struct OpMin
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator ()(const T a, const T b) const { 返回 std::min(a, b); }
};

template<typename T> struct OpMax
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator ()(const T a, const T b) const { 返回 std::max(a, b); }
};

template<typename T> struct OpAbsDiff
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator()(T a, T b) const { 返回 a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
template<> struct OpAbsDiff<float>
{
    typedef float type1;
    typedef float type2;
    typedef float rtype;
    float operator()(float a, float b) const { 返回 std::abs(a - b); }
};
template<> struct OpAbsDiff<double>
{
    typedef double type1;
    typedef double type2;
    typedef double rtype;
    double operator()(double a, double b) const { 返回 std::abs(a - b); }
};

template<typename T> struct OpAnd
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator()( T a, T b ) const { 返回 a & b; }
};

template<typename T> struct OpOr
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator()( T a, T b ) const { 返回 a | b; }
};

template<typename T> struct OpXor
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator()( T a, T b ) const { 返回 a ^ b; }
};

template<typename T> struct OpNot
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator()( T a, T ) const { 返回 ~a; }
};

template<> inline uchar OpAdd<uchar>::operator ()(uchar a, uchar b) const
{ 返回 CV_FAST_CAST_8U(a + b); }

template<> inline uchar OpSub<uchar>::operator ()(uchar a, uchar b) const
{ 返回 CV_FAST_CAST_8U(a - b); }

template<> inline short OpAbsDiff<short>::operator ()(short a, short b) const
{ 返回 saturate_cast<short>(std::abs(a - b)); }

template<> inline schar OpAbsDiff<schar>::operator ()(schar a, schar b) const
{ 返回 saturate_cast<schar>(std::abs(a - b)); }

template<> inline uchar OpMin<uchar>::operator ()(uchar a, uchar b) const { 返回 CV_MIN_8U(a, b); }

template<> inline uchar OpMax<uchar>::operator ()(uchar a, uchar b) const { 返回 CV_MAX_8U(a, b); }

typedef void (*UnaryFunc)(const uchar* src1, size_t step1,
                       uchar* dst, size_t step, Size sz,
                       void*);

typedef void (*BinaryFunc)(const uchar* src1, size_t step1,
                       const uchar* src2, size_t step2,
                       uchar* dst, size_t step, Size sz,
                       void*);

typedef void (*BinaryFuncC)(const void* src1, size_t step1,
                       const void* src2, size_t step2,
                       void* dst, size_t step, 整数 width, 整数 height,
                       void*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS BinaryFunc getConvertFunc(整数 sdepth, 整数 ddepth);
CV_EXPORTS BinaryFunc getConvertScaleFunc(整数 sdepth, 整数 ddepth);
BinaryFunc getCopyMaskFunc(size_t esz);

/* default memory block for sparse array elements */
#define  CV_SPARSE_MAT_BLOCK     (1<<12)

/* initial hash table size */
#define  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CV_SPARSE_HASH_RATIO    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
Size getContinuousSize2D(Mat& m1, 整数 widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, 整数 widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, 整数 widthScale=1);

void setSize( Mat& m, 整数 _dims, const 整数* _sz, const size_t* _steps, bool autoSteps=false );
void finalizeHdr(Mat& m);
整数 updateContinuityFlag(整数 flags, 整数 dims, const 整数* size, const size_t* step);

struct NoVec
{
    size_t operator()(const void*, const void*, void*, size_t) const { 返回 0; }
};

#define CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn) ((INT_MAX/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

enum { BLOCK_SIZE = 1024 };

#如果 defined HAVE_IPP && (IPP_VERSION_X100 >= 700)
#define ARITHM_USE_IPP 1
#否则
#define ARITHM_USE_IPP 0
#endif

inline bool checkScalar(const Mat& sc, 整数 atype, _InputArray::KindFlag sckind, _InputArray::KindFlag akind)
{
    如果( sc.dims > 2 || !sc.isContinuous() )
        返回 false;
    Size sz = sc.size();
    如果(sz.width != 1 && sz.height != 1)
        返回 false;
    整数 cn = CV_MAT_CN(atype);
    如果( akind == _InputArray::MATX && sckind != _InputArray::MATX )
        返回 false;
    返回 sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

inline bool checkScalar(InputArray sc, 整数 atype, _InputArray::KindFlag sckind, _InputArray::KindFlag akind)
{
    如果( sc.dims() > 2 || !sc.isContinuous() )
        返回 false;
    Size sz = sc.size();
    如果(sz.width != 1 && sz.height != 1)
        返回 false;
    整数 cn = CV_MAT_CN(atype);
    如果( akind == _InputArray::MATX && sckind != _InputArray::MATX )
        返回 false;
    返回 sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
inline bool sameShapeAsArg(const _InputArray& sc, const _InputArray& arr)
{
    返回 arr.channels() == 1 && sc.sameSize(arr);
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
inline bool isScalarArg(const _InputArray& sc, const _InputArray& arr)
{
    const _InputArray::KindFlag kind = sc.kind();
    const 整数 cn = arr.channels();
    如果 (kind == _InputArray::MATX)
    {
        Size sz = sc.getSz();
        整数 scn0 = sz.width * sz.height;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        如果 (scn0 != sz.width + sz.height - 1)
            返回 false;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        返回 scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    如果 (kind == _InputArray::MAT)
    {
        const Mat& m = *(const Mat*)sc.getObj();
        返回 m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    如果 (kind == _InputArray::UMAT)
    {
        const UMat& m = *(const UMat*)sc.getObj();
        返回 m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    返回 false;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
inline bool isScalarLikeMat(const _InputArray& sc, 整数 cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    整数 rows, cols, nval;
    bool continuous;
    const _InputArray::KindFlag kind = sc.kind();
    如果 (kind == _InputArray::MAT)
    {
        const Mat& m = *(const Mat*)sc.getObj();
        如果 ((m.rows != 1 && m.cols != 1) || m.dims > 2) 返回 false;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    否则 如果 (kind == _InputArray::UMAT)
    {
        const UMat& m = *(const UMat*)sc.getObj();
        如果 ((m.rows != 1 && m.cols != 1) || m.dims > 2) 返回 false;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    否则
        返回 false;
    如果 (!continuous || nval > 4 || (nval != 1 && nval != cn))
        返回 false;
    返回 (rows == 1 && cols == 1) || (rows == cn && cols == 1) || (rows == 1 && cols == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
enum { EW_SCALAR_BUF_SIZE = 4 * sizeof(double) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
inline 整数 scalarArgElems(const _InputArray& sc, const uchar*& p, 整数& d, uchar* scbuf)
{
    const _InputArray::KindFlag kind = sc.kind();
    如果 (kind == _InputArray::MAT)
    {
        const Mat& m = *(const Mat*)sc.getObj();
        p = m.data;
        d = m.depth();
        返回 (整数)m.total() * m.channels();
    }
    如果 (kind == _InputArray::UMAT)
    {
        const UMat& u = *(const UMat*)sc.getObj();
        d = u.depth();
        整数 n = (整数)u.total() * u.channels();
        CV_Assert(n * (整数)CV_ELEM_SIZE1(d) <= (整数)EW_SCALAR_BUF_SIZE);
        Mat header(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copyTo(header);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        返回 n;
    }
    p = (const uchar*)sc.getObj();
    d = sc.depth();
    Size sz = sc.getSz();
    返回 sz.width * sz.height;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
inline double elemToDouble(整数 d, const uchar* p)
{
    switch (d)
    {
    case CV_8U:   返回 *(const uchar*)p;
    case CV_8S:   返回 *(const schar*)p;
    case CV_16U:  返回 *(const ushort*)p;
    case CV_16S:  返回 *(const short*)p;
    case CV_32U:  返回 *(const unsigned*)p;
    case CV_32S:  返回 *(const 整数*)p;
    case CV_64U:  返回 (double)*(const uint64_t*)p;
    case CV_64S:  返回 (double)*(const int64_t*)p;
    case CV_16F:  返回 (float)*(const hfloat*)p;
    case CV_16BF: 返回 (float)*(const bfloat*)p;
    case CV_32F:  返回 *(const float*)p;
    case CV_64F:  返回 *(const double*)p;
    default:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
inline 整数 readScalarArg(const _InputArray& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    const uchar* p; 整数 d; uchar scbuf[EW_SCALAR_BUF_SIZE];
    整数 n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    为 (整数 i = 0; i < n; i++)
        out[i] = elemToDouble(d, p + (size_t)i * esz);
    返回 n;
}

void convertAndUnrollScalar( const Mat& sc, 整数 buftype, uchar* scbuf, size_t blocksize );

#ifdef CV_COLLECT_IMPL_DATA
struct ImplCollector
{
    ImplCollector()
    {
        useCollection   = false;
        implFlags       = 0;
    }
    bool useCollection; // enable/disable impl data collection

    整数 implFlags;
    std::vector<整数>    implCode;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#endif

struct CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(false), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    bool oclExecutionContextInitialized;
    整数 useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    整数 useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    整数 useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& getCoreTlsData();

#如果 defined(BUILD_SHARED_LIBS)
#如果 defined _WIN32 || defined WINCE
#define CL_RUNTIME_EXPORT __declspec(dllexport)
#elif defined __GNUC__
#define CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#否则
#define CL_RUNTIME_EXPORT
#endif
#否则
#define CL_RUNTIME_EXPORT
#endif

extern CV_EXPORTS
bool __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#define CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_VALUE) \
    static TYPE* const instance = INITIALIZER; \
    返回 RET_VALUE;

#define CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#define CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS void releaseTlsStorageThread();

整数 cv_snprintf(char* buf, 整数 len, const char* fmt, ...);
整数 cv_vsnprintf(char* buf, 整数 len, const char* fmt, va_list args);
}

#endif  // BUILD_PLUGIN
#endif  // __OPENCV_PRECOMP_H__
