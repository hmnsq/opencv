// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Kernels of the 1D DFT/DCT engine, compiled once per SIMD baseline (registered via
// ocv_add_dispatched_file in modules/core/CMakeLists.txt). See dxt.hpp for the engine overview
// and dxt.cpp for the plan builder and the driver. getDFTKernels_() returns the table of kernels
// optimized for THIS baseline; dxt.cpp reaches it through CV_CPU_DISPATCH.
//
// Conventions shared by all kernels:
//  - split layout: separate re[] / im[] arrays of the plan's element type T;
//  - all twiddles are FORWARD (exp(-2*pi*i*...)); inverse transforms are handled by the driver
//    with the conjugation trick, kernels are direction-free;
//  - group B kernels read (sre, sim) and write (dre, dim); src == dst is legal only for stages
//    that the plan marked as in-place safe (plan.pingpong == false), because the vector loops
//    use overlapping "back-off" stores near the end of a span;
//  - group B kernels always step the twiddle pointer by the number of lanes; the plan supplies a
//    table layout (leg-major, tw_stride per leg) that makes this correct. Kernels never
//    special-case span < VL themselves: the plan selects DFT_STAGE_SCALAR for those.

#ضمن "opencv2/core/hal/intrin.hpp"
#ضمن "dxt.hpp"
#ضمن <cmath>
#ضمن <cstring>

نطاق cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

DFTKernels getDFTKernels_(صحيح عمق);

#اگر_تعریف_نشده CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

نطاق {

قالب<نام_نوع T> بنية DftConst
{
    // sin(2*pi/3), (cos(2pi/5)-cos(4pi/5))/2, sin(2pi/5), sin(4pi/5), sqrt(2)/2
    ساكن ثابت T sin120, fft5_2, sin72, sin144, sin45;
};
قالب<نام_نوع T> ثابت T DftConst<T>::sin120 = (T)0.86602540378443864676372317075294;
قالب<نام_نوع T> ثابت T DftConst<T>::fft5_2 = (T)0.559016994374947424102293417182819;
قالب<نام_نوع T> ثابت T DftConst<T>::sin72  = (T)0.951056516295153572116439333379382;
قالب<نام_نوع T> ثابت T DftConst<T>::sin144 = (T)0.587785252292473129168705954639073;
قالب<نام_نوع T> ثابت T DftConst<T>::sin45  = (T)0.70710678118654752440084436210485;

// 4-point forward DFT of (x0,x1,x2,x3) -> (X0,X1,X2,X3), scalar.
// (internal temporaries are prefixed with _t so that callers may pass any names as outputs)
#تعریف_کن DFT4_SCALAR(x0r,x0i,x1r,x1i,x2r,x2i,x3r,x3i, X0r,X0i,X1r,X1i,X2r,X2i,X3r,X3i) \
    { \
        T _t0r = (x0r) + (x2r), _t0i = (x0i) + (x2i), _t1r = (x0r) - (x2r), _t1i = (x0i) - (x2i); \
        T _t2r = (x1r) + (x3r), _t2i = (x1i) + (x3i), _t3r = (x1r) - (x3r), _t3i = (x1i) - (x3i); \
        X0r = _t0r + _t2r; X0i = _t0i + _t2i; X2r = _t0r - _t2r; X2i = _t0i - _t2i; \
        X1r = _t1r + _t3i; X1i = _t1i - _t3r; X3r = _t1r - _t3i; X3i = _t1i + _t3r; \
    }

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
مضمن v_float32 v_setall_(عائم x) { أرجع vx_setall_f32(x); }
// all-lanes sign-bit mask (when neg) or zero: x ^ mask negates x when neg, keeps it otherwise
مضمن v_float32 v_signmask_(منطقي neg, عائم) { أرجع v_reinterpret_as_f32(vx_setall_u32(neg ? 0x80000000u : 0u)); }
#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
مضمن v_float64 v_setall_(مزدوج x) { أرجع vx_setall_f64(x); }
مضمن v_float64 v_signmask_(منطقي neg, مزدوج) { أرجع v_reinterpret_as_f64(vx_setall_u64(neg ? 0x8000000000000000ULL : 0ULL)); }
#پایان_اگر
#پایان_اگر

//////////////////////////////// group A: gather + first stage ////////////////////////////////
//
// Two flavours:
//  - preprocRadix0 (first radix 1, odd nc): table-driven scalar gather, plan.itab holds two
//    element indices (re, im) per complex element; im = 0 gather when plan.real_input;
//  - preprocRadix2/4/8: "transposed" first stage. The r0 inputs of group g are the elements
//    x[j + q*M], M = nc/r0, q = 0..r0-1, where j = base(g) runs over [0, M) in digit-reversed
//    order. Iterating over j instead of g makes the loads contiguous: VL lanes load the r0 legs
//    with v_load_deinterleave, the r0-point DFT runs in registers, and the r0 outputs of every
//    lane are stored contiguously at plan.itab[j] (= g*r0) through a 4-channel
//    v_store_interleave into a small stack buffer + short copies. plan.itab has M entries.
//    The source must be interleaved complex with sstep == 1; DCT input goes through
//    preprocDCT (Makhoul permutation into the temp) first.

قالب<نام_نوع T>
فراغ preprocRadix0_(ثابت DftPlan& plan, ثابت فراغ* _src, size_t sstep, فراغ* _re, فراغ* _im, منطقي conj)
{
    ثابت T* src = (ثابت T*)_src;
    T* re = (T*)_re; T* im = (T*)_im;
    ثابت صحيح* itab = plan.itab.داده();
    صحيح nc = plan.nc;
    إذا (plan.real_درون_داد)
    {
        لكل (صحيح i = 0; i < nc; i++)
        {
            re[i] = src[itab[i*2]*sstep];
            im[i] = 0;
        }
    }
    وإلا إذا (!conj)
    {
        لكل (صحيح i = 0; i < nc; i++)
        {
            re[i] = src[itab[i*2]*sstep];
            im[i] = src[itab[i*2+1]*sstep];
        }
    }
    وإلا
    {
        لكل (صحيح i = 0; i < nc; i++)
        {
            re[i] = src[itab[i*2]*sstep];
            im[i] = -src[itab[i*2+1]*sstep];
        }
    }
}

// scalar r0-point DFTs of one column j (used for the vector tail and the no-SIMD build)
قالب<نام_نوع T>
ساكن مضمن فراغ اولی_Stage2_scalar(ثابت T* x, صحيح M, صحيح j, T* re, T* im, T conjsign)
{
    T x0r = x[j*2], x0i = conjsign*x[j*2+1], x1r = x[(j+M)*2], x1i = conjsign*x[(j+M)*2+1];
    re[0] = x0r + x1r; im[0] = x0i + x1i;
    re[1] = x0r - x1r; im[1] = x0i - x1i;
}

قالب<نام_نوع T>
ساكن مضمن فراغ اولی_Stage4_scalar(ثابت T* x, صحيح M, صحيح j, T* re, T* im, T conjsign)
{
    T x0r = x[j*2], x0i = conjsign*x[j*2+1];
    T x1r = x[(j+M)*2], x1i = conjsign*x[(j+M)*2+1];
    T x2r = x[(j+2*M)*2], x2i = conjsign*x[(j+2*M)*2+1];
    T x3r = x[(j+3*M)*2], x3i = conjsign*x[(j+3*M)*2+1];
    DFT4_SCALAR(x0r,x0i,x1r,x1i,x2r,x2i,x3r,x3i, re[0],im[0],re[1],im[1],re[2],im[2],re[3],im[3]);
}

قالب<نام_نوع T>
ساكن مضمن فراغ اولی_Stage8_scalar(ثابت T* x, صحيح M, صحيح j, T* re, T* im, T conjsign)
{
    ثابت T c = DftConst<T>::sin45;
    T xr[8], xi[8];
    لكل (صحيح q = 0; q < 8; q++)
    {
        xr[q] = x[(j + q*M)*2];
        xi[q] = conjsign*x[(j + q*M)*2 + 1];
    }
    T e0r, e0i, e1r, e1i, e2r, e2i, e3r, e3i, o0r, o0i, o1r, o1i, o2r, o2i, o3r, o3i;
    DFT4_SCALAR(xr[0],xi[0],xr[2],xi[2],xr[4],xi[4],xr[6],xi[6], e0r,e0i,e1r,e1i,e2r,e2i,e3r,e3i);
    DFT4_SCALAR(xr[1],xi[1],xr[3],xi[3],xr[5],xi[5],xr[7],xi[7], o0r,o0i,o1r,o1i,o2r,o2i,o3r,o3i);
    // twiddle the odd part: W8^1 = c(1-i), W8^2 = -i, W8^3 = c(-1-i)
    T t1r = c*(o1r + o1i), t1i = c*(o1i - o1r);
    T t2r = o2i, t2i = -o2r;
    T t3r = c*(o3i - o3r), t3i = -c*(o3r + o3i);
    re[0] = e0r + o0r; im[0] = e0i + o0i; re[4] = e0r - o0r; im[4] = e0i - o0i;
    re[1] = e1r + t1r; im[1] = e1i + t1i; re[5] = e1r - t1r; im[5] = e1i - t1i;
    re[2] = e2r + t2r; im[2] = e2i + t2i; re[6] = e2r - t2r; im[6] = e2i - t2i;
    re[3] = e3r + t3r; im[3] = e3i + t3i; re[7] = e3r - t3r; im[7] = e3i - t3i;
}

#إذا (CV_SIMD || CV_SIMD_SCALABLE)

// 4-point forward DFT on vectors
#تعریف_کن DFT4_VEC(x0r,x0i,x1r,x1i,x2r,x2i,x3r,x3i, X0r,X0i,X1r,X1i,X2r,X2i,X3r,X3i) \
    { \
        VT _t0r = v_جمع_کن(x0r, x2r), _t0i = v_جمع_کن(x0i, x2i), _t1r = v_sub(x0r, x2r), _t1i = v_sub(x0i, x2i); \
        VT _t2r = v_جمع_کن(x1r, x3r), _t2i = v_جمع_کن(x1i, x3i), _t3r = v_sub(x1r, x3r), _t3i = v_sub(x1i, x3i); \
        X0r = v_جمع_کن(_t0r, _t2r); X0i = v_جمع_کن(_t0i, _t2i); X2r = v_sub(_t0r, _t2r); X2i = v_sub(_t0i, _t2i); \
        X1r = v_جمع_کن(_t1r, _t3i); X1i = v_sub(_t1i, _t3r); X3r = v_sub(_t1r, _t3i); X3i = v_جمع_کن(_t1i, _t3r); \
    }

// loads leg q of lanes j..j+VL-1: x[(j + q*M)*2 ...]; imsign (all sign bits or zero, computed
// once per call) conjugates the value without a branch
#تعریف_کن DFT_بارگیری_LEG(q, xr, xi) \
    v_بارگیری_deinterleave(x + (j + (q)*M)*2, xr, xi); \
    xi = v_xor(xi, imsign);

// scatters the r0 outputs of every lane: buf holds them lane-major (r0 consecutive values per lane)
قالب<نام_نوع T, صحيح R0>
ساكن مضمن فراغ scatterLanes(ثابت T* bufr, ثابت T* bufi, T* re, T* im, ثابت صحيح* gtab, صحيح VL)
{
    لكل (صحيح l = 0; l < VL; l++)
    {
        صحيح پایه = gtab[l];
        کپی_حافظه(re + پایه, bufr + l*R0, R0*حجم(T));
        کپی_حافظه(im + پایه, bufi + l*R0, R0*حجم(T));
    }
}

قالب<نام_نوع T, نام_نوع VT>
ساكن فراغ اولی_Stage2_vec(ثابت T* x, صحيح M, ثابت صحيح* gtab, T* re, T* im, منطقي conj)
{
    ثابت صحيح VL = VTraits<VT>::vlanes();
    ثابت VT imsign = v_signmask_(conj, (T)0);
    T bufr[2*VTraits<VT>::max_nlanes], bufi[2*VTraits<VT>::max_nlanes];
    صحيح j = 0;
    لكل (; j + VL <= M; j += VL)
    {
        VT x0r, x0i, x1r, x1i;
        DFT_بارگیری_LEG(0, x0r, x0i)
        DFT_بارگیری_LEG(1, x1r, x1i)
        v_store_interleave(bufr, v_جمع_کن(x0r, x1r), v_sub(x0r, x1r));
        v_store_interleave(bufi, v_جمع_کن(x0i, x1i), v_sub(x0i, x1i));
        scatterLanes<T, 2>(bufr, bufi, re, im, gtab + j, VL);
    }
    T conjsign = conj ? (T)-1 : (T)1;
    لكل (; j < M; j++)
        اولی_Stage2_scalar(x, M, j, re + gtab[j], im + gtab[j], conjsign);
}

قالب<نام_نوع T, نام_نوع VT>
ساكن فراغ اولی_Stage4_vec(ثابت T* x, صحيح M, ثابت صحيح* gtab, T* re, T* im, منطقي conj)
{
    ثابت صحيح VL = VTraits<VT>::vlanes();
    ثابت VT imsign = v_signmask_(conj, (T)0);
    T bufr[4*VTraits<VT>::max_nlanes], bufi[4*VTraits<VT>::max_nlanes];
    صحيح j = 0;
    لكل (; j + VL <= M; j += VL)
    {
        VT x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i, y0r, y0i, y1r, y1i, y2r, y2i, y3r, y3i;
        DFT_بارگیری_LEG(0, x0r, x0i)
        DFT_بارگیری_LEG(1, x1r, x1i)
        DFT_بارگیری_LEG(2, x2r, x2i)
        DFT_بارگیری_LEG(3, x3r, x3i)
        DFT4_VEC(x0r,x0i,x1r,x1i,x2r,x2i,x3r,x3i, y0r,y0i,y1r,y1i,y2r,y2i,y3r,y3i);
        v_store_interleave(bufr, y0r, y1r, y2r, y3r);
        v_store_interleave(bufi, y0i, y1i, y2i, y3i);
        scatterLanes<T, 4>(bufr, bufi, re, im, gtab + j, VL);
    }
    T conjsign = conj ? (T)-1 : (T)1;
    لكل (; j < M; j++)
        اولی_Stage4_scalar(x, M, j, re + gtab[j], im + gtab[j], conjsign);
}

قالب<نام_نوع T, نام_نوع VT>
ساكن فراغ اولی_Stage8_vec(ثابت T* x, صحيح M, ثابت صحيح* gtab, T* re, T* im, منطقي conj)
{
    ثابت صحيح VL = VTraits<VT>::vlanes();
    ثابت VT zero = v_setall_((T)0), imsign = v_signmask_(conj, (T)0);
    ثابت VT c = v_setall_((T)DftConst<T>::sin45);
    T bufr[8*VTraits<VT>::max_nlanes], bufi[8*VTraits<VT>::max_nlanes];
    صحيح j = 0;
    لكل (; j + VL <= M; j += VL)
    {
        VT x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i, x4r, x4i, x5r, x5i, x6r, x6i, x7r, x7i;
        VT e0r, e0i, e1r, e1i, e2r, e2i, e3r, e3i, o0r, o0i, o1r, o1i, o2r, o2i, o3r, o3i;
        DFT_بارگیری_LEG(0, x0r, x0i)
        DFT_بارگیری_LEG(1, x1r, x1i)
        DFT_بارگیری_LEG(2, x2r, x2i)
        DFT_بارگیری_LEG(3, x3r, x3i)
        DFT_بارگیری_LEG(4, x4r, x4i)
        DFT_بارگیری_LEG(5, x5r, x5i)
        DFT_بارگیری_LEG(6, x6r, x6i)
        DFT_بارگیری_LEG(7, x7r, x7i)
        DFT4_VEC(x0r,x0i,x2r,x2i,x4r,x4i,x6r,x6i, e0r,e0i,e1r,e1i,e2r,e2i,e3r,e3i);
        DFT4_VEC(x1r,x1i,x3r,x3i,x5r,x5i,x7r,x7i, o0r,o0i,o1r,o1i,o2r,o2i,o3r,o3i);
        // twiddle the odd part: W8^1 = c(1-i), W8^2 = -i, W8^3 = c(-1-i)
        VT t1r = v_mul(c, v_جمع_کن(o1r, o1i)), t1i = v_mul(c, v_sub(o1i, o1r));
        VT t2r = o2i, t2i = v_sub(zero, o2r);
        VT t3r = v_mul(c, v_sub(o3i, o3r)), t3i = v_sub(zero, v_mul(c, v_جمع_کن(o3r, o3i)));
        // outputs 0..3 -> buf[0..4*VL), 4..7 -> buf[4*VL..8*VL) (lane-major within each half)
        v_store_interleave(bufr, v_جمع_کن(e0r, o0r), v_جمع_کن(e1r, t1r), v_جمع_کن(e2r, t2r), v_جمع_کن(e3r, t3r));
        v_store_interleave(bufi, v_جمع_کن(e0i, o0i), v_جمع_کن(e1i, t1i), v_جمع_کن(e2i, t2i), v_جمع_کن(e3i, t3i));
        v_store_interleave(bufr + 4*VL, v_sub(e0r, o0r), v_sub(e1r, t1r), v_sub(e2r, t2r), v_sub(e3r, t3r));
        v_store_interleave(bufi + 4*VL, v_sub(e0i, o0i), v_sub(e1i, t1i), v_sub(e2i, t2i), v_sub(e3i, t3i));
        لكل (صحيح l = 0; l < VL; l++)
        {
            صحيح پایه = gtab[j + l];
            کپی_حافظه(re + پایه, bufr + l*4, 4*حجم(T));
            کپی_حافظه(re + پایه + 4, bufr + 4*VL + l*4, 4*حجم(T));
            کپی_حافظه(im + پایه, bufi + l*4, 4*حجم(T));
            کپی_حافظه(im + پایه + 4, bufi + 4*VL + l*4, 4*حجم(T));
        }
    }
    T conjsign = conj ? (T)-1 : (T)1;
    لكل (; j < M; j++)
        اولی_Stage8_scalar(x, M, j, re + gtab[j], im + gtab[j], conjsign);
}

#لغو_تعریف DFT_بارگیری_LEG
#لغو_تعریف DFT4_VEC

#پایان_اگر // CV_SIMD || CV_SIMD_SCALABLE

قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ preprocRadix2_(ثابت DftPlan& plan, ثابت فراغ* _src, size_t sstep, فراغ* _re, فراغ* _im, منطقي conj)
{
    CV_DbgAssert(sstep == 1); CV_استفادهنشده(sstep);
    ثابت T* x = (ثابت T*)_src;
    T* re = (T*)_re; T* im = (T*)_im;
    ثابت صحيح* gtab = plan.itab.داده();
    صحيح M = plan.nc/2;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
        اولی_Stage2_vec<T, VT>(x, M, gtab, re, im, conj);
    وإلا
#پایان_اگر
    {
        T conjsign = conj ? (T)-1 : (T)1;
        لكل (صحيح j = 0; j < M; j++)
            اولی_Stage2_scalar(x, M, j, re + gtab[j], im + gtab[j], conjsign);
    }
}

قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ preprocRadix4_(ثابت DftPlan& plan, ثابت فراغ* _src, size_t sstep, فراغ* _re, فراغ* _im, منطقي conj)
{
    CV_DbgAssert(sstep == 1); CV_استفادهنشده(sstep);
    ثابت T* x = (ثابت T*)_src;
    T* re = (T*)_re; T* im = (T*)_im;
    ثابت صحيح* gtab = plan.itab.داده();
    صحيح M = plan.nc/4;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
        اولی_Stage4_vec<T, VT>(x, M, gtab, re, im, conj);
    وإلا
#پایان_اگر
    {
        T conjsign = conj ? (T)-1 : (T)1;
        لكل (صحيح j = 0; j < M; j++)
            اولی_Stage4_scalar(x, M, j, re + gtab[j], im + gtab[j], conjsign);
    }
}

قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ preprocRadix8_(ثابت DftPlan& plan, ثابت فراغ* _src, size_t sstep, فراغ* _re, فراغ* _im, منطقي conj)
{
    CV_DbgAssert(sstep == 1); CV_استفادهنشده(sstep);
    ثابت T* x = (ثابت T*)_src;
    T* re = (T*)_re; T* im = (T*)_im;
    ثابت صحيح* gtab = plan.itab.داده();
    صحيح M = plan.nc/8;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
        اولی_Stage8_vec<T, VT>(x, M, gtab, re, im, conj);
    وإلا
#پایان_اگر
    {
        T conjsign = conj ? (T)-1 : (T)1;
        لكل (صحيح j = 0; j < M; j++)
            اولی_Stage8_scalar(x, M, j, re + gtab[j], im + gtab[j], conjsign);
    }
}

// DCT input permutation (Makhoul): v[j] = x[2j], v[n-1-j] = x[2j+1], j in [0, n/2), into the temp
قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ preprocDCT_(ثابت DftPlan& plan, ثابت فراغ* _src, size_t sstep, فراغ* _tmp)
{
    ثابت T* x = (ثابت T*)_src;
    T* v = (T*)_tmp;
    صحيح n = plan.n, nc = plan.nc, j = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
    {
        إذا (sstep == 1)
        {
            ثابت صحيح VL = VTraits<VT>::vlanes();
            لكل (; j + VL <= nc; j += VL)
            {
                VT even, odd;
                v_بارگیری_deinterleave(x + j*2, even, odd);
                v_store(v + j, even);
                v_store(v + n - j - VL, v_معکوس(odd));
            }
        }
    }
#پایان_اگر
    لكل (; j < nc; j++)
    {
        v[j] = x[(2*j)*sstep];
        v[n - 1 - j] = x[(2*j + 1)*sstep];
    }
}

// Inverse untangle: the "2Z" spectrum of z[j] = x[2j] + i*x[2j+1] from X[k], X[nc-k]:
//   A = X[k] + conj(X[nc-k]), B = X[k] - conj(X[nc-k]), C = W_n^{-k}*B,
//   2Z[k] = A + i*C = (Ar - Ci, Ai + Cr), 2Z[nc-k] = conj(A) + i*conj(C) = (Ar + Ci, Cr - Ai)
#تعریف_کن DFT_UNTANGLE_INV_SCALAR(k, akr, aki, amr, ami, zkr, zki, zmr, zmi) \
    T Ar = akr + amr, Ai = aki - ami; \
    T Br = akr - amr, Bi = aki + ami; \
    T Cr = wr[k]*Br + wi[k]*Bi, Ci = wr[k]*Bi - wi[k]*Br; \
    T zkr = Ar - Ci, zki = Ai + Cr, zmr = Ar + Ci, zmi = Cr - Ai;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
#تعریف_کن DFT_UNTANGLE_INV_VEC(k, akr, aki, amr, ami, zkr, zki, zmr, zmi) \
    VT Ar = v_جمع_کن(akr, amr), Ai = v_sub(aki, ami); \
    VT Br = v_sub(akr, amr), Bi = v_جمع_کن(aki, ami); \
    VT wkr = vx_بارگیری(wr + (k)), wki = vx_بارگیری(wi + (k)); \
    VT Cr = v_fma(wkr, Br, v_mul(wki, Bi)), Ci = v_sub(v_mul(wkr, Bi), v_mul(wki, Br)); \
    VT zkr = v_sub(Ar, Ci), zki = v_جمع_کن(Ai, Cr), zmr = v_جمع_کن(Ar, Ci), zmi = v_sub(Cr, Ai);
#پایان_اگر

// x = interleaved (re, im) pairs: X[k] = (x[2k], x[2k+1]) for k in [1, nc-1]; tmp receives the
// interleaved 2Z values; the driver then runs the complex nc-point transform on it with conj = true.
// The factor 2 is what makes scale = 1/n correct for the nc-point inverse (nc*2*z = n*z).
// 2Z[0] = (X0+Xnc) + i(X0-Xnc), 2Z[nc/2] = 2*conj(X[n/4]).
قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
ساكن مضمن فراغ ccsUntangleInv(ثابت DftPlan& plan, T X0, T Xnc, ثابت T* x, T* tmp)
{
    صحيح nc = plan.nc;
    ثابت T* wr = (ثابت T*)plan.rtw_re;
    ثابت T* wi = (ثابت T*)plan.rtw_im;
    tmp[0] = X0 + Xnc;
    tmp[1] = X0 - Xnc;
    صحيح k = 1, m = nc - 1;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
    {
        ثابت صحيح VL = VTraits<VT>::vlanes();
        لكل (; m - k + 1 >= 2*VL; k += VL, m -= VL)
        {
            VT akr, aki, amr, ami;
            v_بارگیری_deinterleave(x + k*2, akr, aki);
            v_بارگیری_deinterleave(x + (m - VL + 1)*2, amr, ami);
            amr = v_معکوس(amr); ami = v_معکوس(ami);
            DFT_UNTANGLE_INV_VEC(k, akr, aki, amr, ami, zkr, zki, zmr, zmi)
            v_store_interleave(tmp + k*2, zkr, zki);
            v_store_interleave(tmp + (m - VL + 1)*2, v_معکوس(zmr), v_معکوس(zmi));
        }
    }
#پایان_اگر
    لكل (; k < m; k++, m--)
    {
        T akr = x[k*2], aki = x[k*2+1], amr = x[m*2], ami = x[m*2+1];
        DFT_UNTANGLE_INV_SCALAR(k, akr, aki, amr, ami, zkr, zki, zmr, zmi)
        tmp[k*2] = zkr; tmp[k*2+1] = zki;
        tmp[m*2] = zmr; tmp[m*2+1] = zmi;
    }
    إذا ((nc & 1) == 0 && nc >= 2)
    {
        k = nc/2;
        tmp[k*2] = x[k*2]*2;
        tmp[k*2+1] = -x[k*2+1]*2;
    }
}

قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ preprocCCS_(ثابت DftPlan& plan, ثابت فراغ* _src, فراغ* _tmp, منطقي complex_درون_داد)
{
    ثابت T* src = (ثابت T*)_src;
    T* tmp = (T*)_tmp;
    صحيح n = plan.n, nc = plan.nc;
    إذا (nc == n)
    {
        // odd n: expand the packed half-spectrum into the full conjugate-symmetric spectrum
        tmp[0] = src[0]; tmp[1] = 0;
        ثابت T* x = complex_درون_داد ? src + 2 : src + 1;   // X[k] = (x[2(k-1)], x[2(k-1)+1])
        لكل (صحيح k = 1; k <= (n-1)/2; k++)
        {
            T xr = x[(k-1)*2], xi = x[(k-1)*2+1];
            tmp[k*2] = xr; tmp[k*2+1] = xi;
            tmp[(n-k)*2] = xr; tmp[(n-k)*2+1] = -xi;
        }
        أرجع;
    }
    T X0 = src[0], Xnc = complex_درون_داد ? src[n] : src[n-1];
    // CCS: X[k] = (src[2k-1], src[2k]); complex: X[k] = (src[2k], src[2k+1])
    ccsUntangleInv<T, VT, haveSIMD>(plan, X0, Xnc, complex_درون_داد ? src : src - 1, tmp);
}

قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ preprocIDCT_(ثابت DftPlan& plan, ثابت فراغ* _src, size_t sstep, فراغ* _tmp)
{
    ثابت T* src = (ثابت T*)_src;
    T* tmp = (T*)_tmp;
    صحيح n = plan.n, nc = plan.nc;
    ثابت T* dr = (ثابت T*)plan.dct_re;
    ثابت T* di = (ثابت T*)plan.dct_im;
    ثابت T* wr = (ثابت T*)plan.rtw_re;
    ثابت T* wi = (ثابت T*)plan.rtw_im;
    // pre-twiddle: X[k] = conj(w_k) * (src[k] - i*src[n-k]), k in [1, nc-1],
    // X0 = 2*src[0]*w_0*sin45, Xnc = 2*src[nc]*w_nc (the two special normalizations of DCT-III).
    T X0 = src[0]*2*dr[0]*DftConst<T>::sin45;
    T Xnc = src[nc*sstep]*2*dr[nc];
    tmp[0] = X0 + Xnc;
    tmp[1] = X0 - Xnc;
    صحيح k = 1, m = nc - 1;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
    {
        إذا (sstep == 1)
        {
            ثابت صحيح VL = VTraits<VT>::vlanes();
            لكل (; m - k + 1 >= 2*VL; k += VL, m -= VL)
            {
                VT a = vx_بارگیری(src + k), b = v_معکوس(vx_بارگیری(src + n - k - VL + 1));   // src[k+l], src[n-k-l]
                VT dkr = vx_بارگیری(dr + k), dki = vx_بارگیری(di + k);
                VT akr = v_sub(v_mul(dkr, a), v_mul(dki, b)), aki = v_sub(v_sub(v_setall_((T)0), v_mul(dkr, b)), v_mul(dki, a));
                a = v_معکوس(vx_بارگیری(src + m - VL + 1)); b = vx_بارگیری(src + n - m);      // src[m-l], src[n-m+l]
                VT dmr = v_معکوس(vx_بارگیری(dr + m - VL + 1)), dmi = v_معکوس(vx_بارگیری(di + m - VL + 1));
                VT amr = v_sub(v_mul(dmr, a), v_mul(dmi, b)), ami = v_sub(v_sub(v_setall_((T)0), v_mul(dmr, b)), v_mul(dmi, a));
                DFT_UNTANGLE_INV_VEC(k, akr, aki, amr, ami, zkr, zki, zmr, zmi)
                v_store_interleave(tmp + k*2, zkr, zki);
                v_store_interleave(tmp + (m - VL + 1)*2, v_معکوس(zmr), v_معکوس(zmi));
            }
        }
    }
#پایان_اگر
    لكل (; k < m; k++, m--)
    {
        T a = src[k*sstep], b = src[(n-k)*sstep];
        T akr = dr[k]*a - di[k]*b, aki = -dr[k]*b - di[k]*a;
        a = src[m*sstep]; b = src[(n-m)*sstep];
        T amr = dr[m]*a - di[m]*b, ami = -dr[m]*b - di[m]*a;
        DFT_UNTANGLE_INV_SCALAR(k, akr, aki, amr, ami, zkr, zki, zmr, zmi)
        tmp[k*2] = zkr; tmp[k*2+1] = zki;
        tmp[m*2] = zmr; tmp[m*2+1] = zmi;
    }
    إذا ((nc & 1) == 0 && nc >= 2)
    {
        k = nc/2;
        T a = src[k*sstep], b = src[(n-k)*sstep];
        tmp[k*2] = (dr[k]*a - di[k]*b)*2;
        tmp[k*2+1] = -(-dr[k]*b - di[k]*a)*2;
    }
}

#لغو_تعریف DFT_UNTANGLE_INV_SCALAR
#لغو_تعریف DFT_UNTANGLE_INV_VEC

//////////////////////////////// group B: middle stages ////////////////////////////////
//
// Stage (radix r, span L) combines, for every group g (G = nc/(r*L) groups) and every j in [0, L),
// the r inputs at base + q*L + j (base = g*r*L, q = 0..r-1), each multiplied by the twiddle
// W_{rL}^{q*j}, with an r-point DFT and writes the outputs to the same offsets.
//
// Vector bodies (DFT_STAGE_FULL, L >= VL) use one flat loop (brief, section 5): a single counter
// `j` addresses both the data and the twiddles and is clamped to L - VL before use, so the last
// vector of a span re-does a few lanes (back-off). The overlapping stores are legal only because
// such stages run out-of-place (plan.pingpong): never turn them into in-place stages.
// The wrap to the next group is branchless.

#تعریف_کن DFT_CMUL(xr, xi, ar, ai, wr, wi) { xr = (ar)*(wr) - (ai)*(wi); xi = (ar)*(wi) + (ai)*(wr); }

قالب<نام_نوع T>
فراغ radix3_scalar(ثابت DftStage& st, ثابت T* sre, ثابت T* sim, T* dre, T* dim)
{
    ثابت T* twr = (ثابت T*)st.tw_re; ثابت T* twi = (ثابت T*)st.tw_im;
    صحيح L = st.بازه, G = st.ngroups, ts = st.tw_stride;
    ثابت T s120 = DftConst<T>::sin120;
    لكل (صحيح g = 0; g < G; g++)
    {
        صحيح پایه = g*3*L;
        لكل (صحيح j = 0; j < L; j++)
        {
            صحيح i0 = پایه + j, i1 = i0 + L, i2 = i1 + L;
            T x0r = sre[i0], x0i = sim[i0], x1r, x1i, x2r, x2i;
            DFT_CMUL(x1r, x1i, sre[i1], sim[i1], twr[j], twi[j]);
            DFT_CMUL(x2r, x2i, sre[i2], sim[i2], twr[ts + j], twi[ts + j]);
            T sr = x1r + x2r, si = x1i + x2i;
            T dr = s120*(x1r - x2r), di = s120*(x1i - x2i);
            T mr = x0r - (T)0.5*sr, mi = x0i - (T)0.5*si;
            dre[i0] = x0r + sr; dim[i0] = x0i + si;
            dre[i1] = mr + di; dim[i1] = mi - dr;
            dre[i2] = mr - di; dim[i2] = mi + dr;
        }
    }
}

قالب<نام_نوع T>
فراغ radix4_scalar(ثابت DftStage& st, ثابت T* sre, ثابت T* sim, T* dre, T* dim)
{
    ثابت T* twr = (ثابت T*)st.tw_re; ثابت T* twi = (ثابت T*)st.tw_im;
    صحيح L = st.بازه, G = st.ngroups, ts = st.tw_stride;
    لكل (صحيح g = 0; g < G; g++)
    {
        صحيح پایه = g*4*L;
        لكل (صحيح j = 0; j < L; j++)
        {
            صحيح i0 = پایه + j, i1 = i0 + L, i2 = i1 + L, i3 = i2 + L;
            T x0r = sre[i0], x0i = sim[i0], x1r, x1i, x2r, x2i, x3r, x3i;
            DFT_CMUL(x1r, x1i, sre[i1], sim[i1], twr[j], twi[j]);
            DFT_CMUL(x2r, x2i, sre[i2], sim[i2], twr[ts + j], twi[ts + j]);
            DFT_CMUL(x3r, x3i, sre[i3], sim[i3], twr[ts*2 + j], twi[ts*2 + j]);
            DFT4_SCALAR(x0r,x0i,x1r,x1i,x2r,x2i,x3r,x3i,
                        dre[i0],dim[i0],dre[i1],dim[i1],dre[i2],dim[i2],dre[i3],dim[i3]);
        }
    }
}

قالب<نام_نوع T>
فراغ radix5_scalar(ثابت DftStage& st, ثابت T* sre, ثابت T* sim, T* dre, T* dim)
{
    ثابت T* twr = (ثابت T*)st.tw_re; ثابت T* twi = (ثابت T*)st.tw_im;
    صحيح L = st.بازه, G = st.ngroups, ts = st.tw_stride;
    ثابت T c5 = DftConst<T>::fft5_2, s1 = DftConst<T>::sin72, s2 = DftConst<T>::sin144;
    لكل (صحيح g = 0; g < G; g++)
    {
        صحيح پایه = g*5*L;
        لكل (صحيح j = 0; j < L; j++)
        {
            صحيح i0 = پایه + j, i1 = i0 + L, i2 = i1 + L, i3 = i2 + L, i4 = i3 + L;
            T x0r = sre[i0], x0i = sim[i0], x1r, x1i, x2r, x2i, x3r, x3i, x4r, x4i;
            DFT_CMUL(x1r, x1i, sre[i1], sim[i1], twr[j], twi[j]);
            DFT_CMUL(x2r, x2i, sre[i2], sim[i2], twr[ts + j], twi[ts + j]);
            DFT_CMUL(x3r, x3i, sre[i3], sim[i3], twr[ts*2 + j], twi[ts*2 + j]);
            DFT_CMUL(x4r, x4i, sre[i4], sim[i4], twr[ts*3 + j], twi[ts*3 + j]);
            T a1r = x1r + x4r, a1i = x1i + x4i, b1r = x1r - x4r, b1i = x1i - x4i;
            T a2r = x2r + x3r, a2i = x2i + x3i, b2r = x2r - x3r, b2i = x2i - x3i;
            T tr = x0r - (T)0.25*(a1r + a2r), ti = x0i - (T)0.25*(a1i + a2i);
            T ur = c5*(a1r - a2r), ui = c5*(a1i - a2i);
            T v1r = s1*b1r + s2*b2r, v1i = s1*b1i + s2*b2i;
            T v2r = s2*b1r - s1*b2r, v2i = s2*b1i - s1*b2i;
            T pr = tr + ur, pi = ti + ui, qr = tr - ur, qi = ti - ui;
            dre[i0] = x0r + a1r + a2r; dim[i0] = x0i + a1i + a2i;
            dre[i1] = pr + v1i; dim[i1] = pi - v1r;
            dre[i4] = pr - v1i; dim[i4] = pi + v1r;
            dre[i2] = qr + v2i; dim[i2] = qi - v2r;
            dre[i3] = qr - v2i; dim[i3] = qi + v2r;
        }
    }
}


// generic odd radix r = 2h+1 (r >= 7); scratch holds a[h], b[h] (complex) for the current butterfly
قالب<نام_نوع T>
فراغ radixOdd_scalar(ثابت DftStage& st, ثابت T* sre, ثابت T* sim, T* dre, T* dim, T* scratch)
{
    ثابت T* twr = (ثابت T*)st.tw_re; ثابت T* twi = (ثابت T*)st.tw_im;
    ثابت T* cs = (ثابت T*)st.cs; ثابت T* sn = (ثابت T*)st.sn;
    صحيح L = st.بازه, G = st.ngroups, ts = st.tw_stride, r = st.radix, h = (r - 1)/2;
    T* ar = scratch; T* ai = ar + h; T* br = ai + h; T* bi = br + h;
    لكل (صحيح g = 0; g < G; g++)
    {
        صحيح پایه = g*r*L;
        لكل (صحيح j = 0; j < L; j++)
        {
            صحيح i0 = پایه + j;
            T x0r = sre[i0], x0i = sim[i0];
            T X0r = x0r, X0i = x0i;
            لكل (صحيح p = 1; p <= h; p++)
            {
                صحيح ip = i0 + p*L, iq = i0 + (r-p)*L;
                T xpr, xpi, xqr, xqi;
                DFT_CMUL(xpr, xpi, sre[ip], sim[ip], twr[(p-1)*ts + j], twi[(p-1)*ts + j]);
                DFT_CMUL(xqr, xqi, sre[iq], sim[iq], twr[(r-p-1)*ts + j], twi[(r-p-1)*ts + j]);
                ar[p-1] = xpr + xqr; ai[p-1] = xpi + xqi;
                br[p-1] = xpr - xqr; bi[p-1] = xpi - xqi;
                X0r += ar[p-1]; X0i += ai[p-1];
            }
            dre[i0] = X0r; dim[i0] = X0i;
            لكل (صحيح k = 1; k <= h; k++)
            {
                T sr = x0r, si = x0i, tr = 0, ti = 0;
                لكل (صحيح p = 1; p <= h; p++)
                {
                    T c = cs[(p-1)*h + (k-1)], s = sn[(p-1)*h + (k-1)];
                    sr += c*ar[p-1]; si += c*ai[p-1];
                    tr += s*br[p-1]; ti += s*bi[p-1];
                }
                dre[i0 + k*L] = sr + ti; dim[i0 + k*L] = si - tr;
                dre[i0 + (r-k)*L] = sr - ti; dim[i0 + (r-k)*L] = si + tr;
            }
        }
    }
}

#لغو_تعریف DFT_CMUL

#إذا (CV_SIMD || CV_SIMD_SCALABLE)

// dst = a*s + c, s is a scalar constant of the stage (the compiler hoists the broadcast)
قالب<نام_نوع VT, نام_نوع T>
مضمن VT v_fma_n(ثابت VT& a, T s, ثابت VT& c) { أرجع v_fma(a, v_setall_(s), c); }

// (xr, xi) = (ar, ai) * (wr, wi)
#تعریف_کن DFT_VCMUL(xr, xi, ar, ai, wr, wi) \
    { xr = v_sub(v_mul(ar, wr), v_mul(ai, wi)); xi = v_fma(ar, wi, v_mul(ai, wr)); }

// the flat loop of the brief (section 5): clamp, body, branchless wrap
#تعریف_کن DFT_VLOOP_BEGIN(r) \
    صحيح j = 0, پایه = 0; \
    لكل (صحيح t = 0; t < st.nsteps; t++) \
    { \
        j = std::min(j, L - VL);
#تعریف_کن DFT_VLOOP_انتها(r) \
        صحيح nj = j + VL; \
        صحيح wrap = -(صحيح)(nj >= L); \
        پایه += ((r)*L) & wrap; \
        j = nj & ~wrap; \
    }

قالب<نام_نوع T, نام_نوع VT>
فراغ radix3_vec(ثابت DftStage& st, ثابت T* sre, ثابت T* sim, T* dre, T* dim)
{
    ثابت T* twr = (ثابت T*)st.tw_re; ثابت T* twi = (ثابت T*)st.tw_im;
    ثابت صحيح L = st.بازه, ts = st.tw_stride, VL = VTraits<VT>::vlanes();
    ثابت VT s120 = v_setall_((T)DftConst<T>::sin120), half = v_setall_((T)0.5);
    DFT_VLOOP_BEGIN(3)
        صحيح i0 = پایه + j, i1 = i0 + L, i2 = i1 + L;
        VT x0r = vx_بارگیری(sre + i0), x0i = vx_بارگیری(sim + i0), x1r, x1i, x2r, x2i;
        DFT_VCMUL(x1r, x1i, vx_بارگیری(sre + i1), vx_بارگیری(sim + i1), vx_بارگیری(twr + j), vx_بارگیری(twi + j));
        DFT_VCMUL(x2r, x2i, vx_بارگیری(sre + i2), vx_بارگیری(sim + i2), vx_بارگیری(twr + ts + j), vx_بارگیری(twi + ts + j));
        VT sr = v_جمع_کن(x1r, x2r), si = v_جمع_کن(x1i, x2i);
        VT dr = v_mul(s120, v_sub(x1r, x2r)), di = v_mul(s120, v_sub(x1i, x2i));
        VT mr = v_sub(x0r, v_mul(half, sr)), mi = v_sub(x0i, v_mul(half, si));
        v_store(dre + i0, v_جمع_کن(x0r, sr)); v_store(dim + i0, v_جمع_کن(x0i, si));
        v_store(dre + i1, v_جمع_کن(mr, di)); v_store(dim + i1, v_sub(mi, dr));
        v_store(dre + i2, v_sub(mr, di)); v_store(dim + i2, v_جمع_کن(mi, dr));
    DFT_VLOOP_انتها(3)
}

قالب<نام_نوع T, نام_نوع VT>
فراغ radix4_vec(ثابت DftStage& st, ثابت T* sre, ثابت T* sim, T* dre, T* dim)
{
    ثابت T* twr = (ثابت T*)st.tw_re; ثابت T* twi = (ثابت T*)st.tw_im;
    ثابت صحيح L = st.بازه, ts = st.tw_stride, VL = VTraits<VT>::vlanes();
    DFT_VLOOP_BEGIN(4)
        صحيح i0 = پایه + j, i1 = i0 + L, i2 = i1 + L, i3 = i2 + L;
        VT x0r = vx_بارگیری(sre + i0), x0i = vx_بارگیری(sim + i0), x1r, x1i, x2r, x2i, x3r, x3i;
        DFT_VCMUL(x1r, x1i, vx_بارگیری(sre + i1), vx_بارگیری(sim + i1), vx_بارگیری(twr + j), vx_بارگیری(twi + j));
        DFT_VCMUL(x2r, x2i, vx_بارگیری(sre + i2), vx_بارگیری(sim + i2), vx_بارگیری(twr + ts + j), vx_بارگیری(twi + ts + j));
        DFT_VCMUL(x3r, x3i, vx_بارگیری(sre + i3), vx_بارگیری(sim + i3), vx_بارگیری(twr + ts*2 + j), vx_بارگیری(twi + ts*2 + j));
        VT y0r = v_جمع_کن(x0r, x2r), y0i = v_جمع_کن(x0i, x2i), y1r = v_sub(x0r, x2r), y1i = v_sub(x0i, x2i);
        VT y2r = v_جمع_کن(x1r, x3r), y2i = v_جمع_کن(x1i, x3i), y3r = v_sub(x1r, x3r), y3i = v_sub(x1i, x3i);
        v_store(dre + i0, v_جمع_کن(y0r, y2r)); v_store(dim + i0, v_جمع_کن(y0i, y2i));
        v_store(dre + i2, v_sub(y0r, y2r)); v_store(dim + i2, v_sub(y0i, y2i));
        v_store(dre + i1, v_جمع_کن(y1r, y3i)); v_store(dim + i1, v_sub(y1i, y3r));
        v_store(dre + i3, v_sub(y1r, y3i)); v_store(dim + i3, v_جمع_کن(y1i, y3r));
    DFT_VLOOP_انتها(4)
}

قالب<نام_نوع T, نام_نوع VT>
فراغ radix5_vec(ثابت DftStage& st, ثابت T* sre, ثابت T* sim, T* dre, T* dim)
{
    ثابت T* twr = (ثابت T*)st.tw_re; ثابت T* twi = (ثابت T*)st.tw_im;
    ثابت صحيح L = st.بازه, ts = st.tw_stride, VL = VTraits<VT>::vlanes();
    ثابت T c5 = DftConst<T>::fft5_2, s1 = DftConst<T>::sin72, s2 = DftConst<T>::sin144;
    ثابت VT quarter = v_setall_((T)0.25);
    DFT_VLOOP_BEGIN(5)
        صحيح i0 = پایه + j, i1 = i0 + L, i2 = i1 + L, i3 = i2 + L, i4 = i3 + L;
        VT x0r = vx_بارگیری(sre + i0), x0i = vx_بارگیری(sim + i0), x1r, x1i, x2r, x2i, x3r, x3i, x4r, x4i;
        DFT_VCMUL(x1r, x1i, vx_بارگیری(sre + i1), vx_بارگیری(sim + i1), vx_بارگیری(twr + j), vx_بارگیری(twi + j));
        DFT_VCMUL(x2r, x2i, vx_بارگیری(sre + i2), vx_بارگیری(sim + i2), vx_بارگیری(twr + ts + j), vx_بارگیری(twi + ts + j));
        DFT_VCMUL(x3r, x3i, vx_بارگیری(sre + i3), vx_بارگیری(sim + i3), vx_بارگیری(twr + ts*2 + j), vx_بارگیری(twi + ts*2 + j));
        DFT_VCMUL(x4r, x4i, vx_بارگیری(sre + i4), vx_بارگیری(sim + i4), vx_بارگیری(twr + ts*3 + j), vx_بارگیری(twi + ts*3 + j));
        VT a1r = v_جمع_کن(x1r, x4r), a1i = v_جمع_کن(x1i, x4i), b1r = v_sub(x1r, x4r), b1i = v_sub(x1i, x4i);
        VT a2r = v_جمع_کن(x2r, x3r), a2i = v_جمع_کن(x2i, x3i), b2r = v_sub(x2r, x3r), b2i = v_sub(x2i, x3i);
        VT sar = v_جمع_کن(a1r, a2r), sai = v_جمع_کن(a1i, a2i);
        VT tr = v_sub(x0r, v_mul(quarter, sar)), ti = v_sub(x0i, v_mul(quarter, sai));
        VT ur = v_mul(v_setall_(c5), v_sub(a1r, a2r)), ui = v_mul(v_setall_(c5), v_sub(a1i, a2i));
        VT v1r = v_fma_n(b1r, s1, v_mul(v_setall_(s2), b2r)), v1i = v_fma_n(b1i, s1, v_mul(v_setall_(s2), b2i));
        VT v2r = v_fma_n(b1r, s2, v_mul(v_setall_(-s1), b2r)), v2i = v_fma_n(b1i, s2, v_mul(v_setall_(-s1), b2i));
        VT pr = v_جمع_کن(tr, ur), pi = v_جمع_کن(ti, ui), qr = v_sub(tr, ur), qi = v_sub(ti, ui);
        v_store(dre + i0, v_جمع_کن(x0r, sar)); v_store(dim + i0, v_جمع_کن(x0i, sai));
        v_store(dre + i1, v_جمع_کن(pr, v1i)); v_store(dim + i1, v_sub(pi, v1r));
        v_store(dre + i4, v_sub(pr, v1i)); v_store(dim + i4, v_جمع_کن(pi, v1r));
        v_store(dre + i2, v_جمع_کن(qr, v2i)); v_store(dim + i2, v_sub(qi, v2r));
        v_store(dre + i3, v_sub(qr, v2i)); v_store(dim + i3, v_جمع_کن(qi, v2r));
    DFT_VLOOP_انتها(5)
}


// generic odd radix, vector lanes; scratch holds a[h], b[h] as VL-wide vectors (4*h*VL elements)
قالب<نام_نوع T, نام_نوع VT>
فراغ radixOdd_vec(ثابت DftStage& st, ثابت T* sre, ثابت T* sim, T* dre, T* dim, T* scratch)
{
    ثابت T* twr = (ثابت T*)st.tw_re; ثابت T* twi = (ثابت T*)st.tw_im;
    ثابت T* cs = (ثابت T*)st.cs; ثابت T* sn = (ثابت T*)st.sn;
    ثابت صحيح L = st.بازه, ts = st.tw_stride, VL = VTraits<VT>::vlanes(), r = st.radix, h = (r - 1)/2;
    T* ar = scratch; T* ai = ar + h*VL; T* br = ai + h*VL; T* bi = br + h*VL;
    DFT_VLOOP_BEGIN(r)
        صحيح i0 = پایه + j;
        VT x0r = vx_بارگیری(sre + i0), x0i = vx_بارگیری(sim + i0);
        VT X0r = x0r, X0i = x0i;
        لكل (صحيح p = 1; p <= h; p++)
        {
            صحيح ip = i0 + p*L, iq = i0 + (r-p)*L;
            VT xpr, xpi, xqr, xqi;
            DFT_VCMUL(xpr, xpi, vx_بارگیری(sre + ip), vx_بارگیری(sim + ip),
                      vx_بارگیری(twr + (p-1)*ts + j), vx_بارگیری(twi + (p-1)*ts + j));
            DFT_VCMUL(xqr, xqi, vx_بارگیری(sre + iq), vx_بارگیری(sim + iq),
                      vx_بارگیری(twr + (r-p-1)*ts + j), vx_بارگیری(twi + (r-p-1)*ts + j));
            VT apr = v_جمع_کن(xpr, xqr), api = v_جمع_کن(xpi, xqi);
            v_store(ar + (p-1)*VL, apr); v_store(ai + (p-1)*VL, api);
            v_store(br + (p-1)*VL, v_sub(xpr, xqr)); v_store(bi + (p-1)*VL, v_sub(xpi, xqi));
            X0r = v_جمع_کن(X0r, apr); X0i = v_جمع_کن(X0i, api);
        }
        v_store(dre + i0, X0r); v_store(dim + i0, X0i);
        لكل (صحيح k = 1; k <= h; k++)
        {
            VT sr = x0r, si = x0i, tr = v_setall_((T)0), ti = tr;
            لكل (صحيح p = 1; p <= h; p++)
            {
                T c = cs[(p-1)*h + (k-1)], s = sn[(p-1)*h + (k-1)];
                sr = v_fma_n(vx_بارگیری(ar + (p-1)*VL), c, sr); si = v_fma_n(vx_بارگیری(ai + (p-1)*VL), c, si);
                tr = v_fma_n(vx_بارگیری(br + (p-1)*VL), s, tr); ti = v_fma_n(vx_بارگیری(bi + (p-1)*VL), s, ti);
            }
            v_store(dre + i0 + k*L, v_جمع_کن(sr, ti)); v_store(dim + i0 + k*L, v_sub(si, tr));
            v_store(dre + i0 + (r-k)*L, v_sub(sr, ti)); v_store(dim + i0 + (r-k)*L, v_جمع_کن(si, tr));
        }
    DFT_VLOOP_انتها(r)
}

#لغو_تعریف DFT_VLOOP_BEGIN
#لغو_تعریف DFT_VLOOP_انتها
#لغو_تعریف DFT_VCMUL

#پایان_اگر // CV_SIMD || CV_SIMD_SCALABLE

// entry points: the plan's stage mode selects the body; VT = T (no SIMD for this type) means scalar only
قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ radix3_(ثابت DftStage& st, ثابت فراغ* sre, ثابت فراغ* sim, فراغ* dre, فراغ* dim, فراغ*)
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
    {
        إذا (st.نما == DFT_STAGE_پر)
        {
            radix3_vec<T, VT>(st, (ثابت T*)sre, (ثابت T*)sim, (T*)dre, (T*)dim);
            أرجع;
        }
    }
#پایان_اگر
    radix3_scalar<T>(st, (ثابت T*)sre, (ثابت T*)sim, (T*)dre, (T*)dim);
}

قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ radix4_(ثابت DftStage& st, ثابت فراغ* sre, ثابت فراغ* sim, فراغ* dre, فراغ* dim, فراغ*)
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
    {
        إذا (st.نما == DFT_STAGE_پر)
        {
            radix4_vec<T, VT>(st, (ثابت T*)sre, (ثابت T*)sim, (T*)dre, (T*)dim);
            أرجع;
        }
    }
#پایان_اگر
    radix4_scalar<T>(st, (ثابت T*)sre, (ثابت T*)sim, (T*)dre, (T*)dim);
}

قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ radix5_(ثابت DftStage& st, ثابت فراغ* sre, ثابت فراغ* sim, فراغ* dre, فراغ* dim, فراغ*)
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
    {
        إذا (st.نما == DFT_STAGE_پر)
        {
            radix5_vec<T, VT>(st, (ثابت T*)sre, (ثابت T*)sim, (T*)dre, (T*)dim);
            أرجع;
        }
    }
#پایان_اگر
    radix5_scalar<T>(st, (ثابت T*)sre, (ثابت T*)sim, (T*)dre, (T*)dim);
}


قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ radixOdd_(ثابت DftStage& st, ثابت فراغ* sre, ثابت فراغ* sim, فراغ* dre, فراغ* dim, فراغ* scratch)
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
    {
        إذا (st.نما == DFT_STAGE_پر)
        {
            radixOdd_vec<T, VT>(st, (ثابت T*)sre, (ثابت T*)sim, (T*)dre, (T*)dim, (T*)scratch);
            أرجع;
        }
    }
#پایان_اگر
    radixOdd_scalar<T>(st, (ثابت T*)sre, (ثابت T*)sim, (T*)dre, (T*)dim, (T*)scratch);
}


//////////////////////////////// group C: postprocessing ////////////////////////////////
//
// Pure map passes over the split arrays (plus the C2R / IDCT input untangles, which are the
// mirror images and live here too). Each is written once: a vector body over whole vectors and a
// scalar tail. The "pair" passes (RFFT untangle, DCT) process k ascending together with m = nc-k
// descending; the descending side is loaded/stored through v_reverse.

قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ postprocDFT_(ثابت DftPlan& plan, ثابت فراغ* _re, ثابت فراغ* _im, فراغ* _dst,
                  مزدوج re_scale, مزدوج im_scale)
{
    ثابت T* re = (ثابت T*)_re; ثابت T* im = (ثابت T*)_im;
    T* dst = (T*)_dst;
    صحيح nc = plan.nc, i = 0;
    T rs = (T)re_scale, is = (T)im_scale;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
    {
        ثابت صحيح VL = VTraits<VT>::vlanes();
        VT vrs = v_setall_(rs), vis = v_setall_(is);
        لكل (; i + VL <= nc; i += VL)
            v_store_interleave(dst + i*2, v_mul(vx_بارگیری(re + i), vrs), v_mul(vx_بارگیری(im + i), vis));
    }
#پایان_اگر
    لكل (; i < nc; i++)
    {
        dst[i*2] = re[i]*rs;
        dst[i*2+1] = im[i]*is;
    }
}

قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ postprocReal_(ثابت DftPlan& plan, ثابت فراغ* _re, فراغ* _dst, مزدوج scale)
{
    ثابت T* re = (ثابت T*)_re;
    T* dst = (T*)_dst;
    صحيح nc = plan.nc, i = 0;
    T s = (T)scale;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
    {
        ثابت صحيح VL = VTraits<VT>::vlanes();
        VT vs = v_setall_(s);
        لكل (; i + VL <= nc; i += VL)
            v_store(dst + i, v_mul(vx_بارگیری(re + i), vs));
    }
#پایان_اگر
    لكل (; i < nc; i++)
        dst[i] = re[i]*s;
}

// Forward untangle of the nc-point spectrum Z of z[j] = x[2j] + i*x[2j+1] into the spectrum X of x:
//   E = (Z[k] + conj Z[nc-k])/2, O = (Z[k] - conj Z[nc-k])/(2i), X[k] = E + W_n^k*O,
//   X[nc-k] = conj(E - W_n^k*O), X[0] = Z0.re + Z0.im, X[nc] = Z0.re - Z0.im, X[n/4] = conj Z[nc/2].
// X[k], k in [1, nc-1] is written to (out[k*2-1], out[k*2]), i.e. CCS order; the caller handles
// X[0] and X[nc].
#تعریف_کن DFT_UNTANGLE_FWD_SCALAR(k, m, Xkr, Xki, Xmr, Xmi) \
    T Er = (re[k] + re[m])*(T)0.5, Ei = (im[k] - im[m])*(T)0.5; \
    T Or = (im[k] + im[m])*(T)0.5, Oi = (re[m] - re[k])*(T)0.5; \
    T Pr = wr[k]*Or - wi[k]*Oi, Pi = wr[k]*Oi + wi[k]*Or; \
    T Xkr = Er + Pr, Xki = Ei + Pi, Xmr = Er - Pr, Xmi = Pi - Ei;

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
// vector version: lanes l = 0..VL-1 hold k+l (ascending) and m-l (descending; loaded reversed)
#تعریف_کن DFT_UNTANGLE_FWD_VEC(k, m, Xkr, Xki, Xmr, Xmi) \
    VT rk = vx_بارگیری(re + (k)), ik = vx_بارگیری(im + (k)); \
    VT rm = v_معکوس(vx_بارگیری(re + (m) - VL + 1)), imm = v_معکوس(vx_بارگیری(im + (m) - VL + 1)); \
    VT Er = v_mul(v_جمع_کن(rk, rm), half), Ei = v_mul(v_sub(ik, imm), half); \
    VT Or = v_mul(v_جمع_کن(ik, imm), half), Oi = v_mul(v_sub(rm, rk), half); \
    VT wkr = vx_بارگیری(wr + (k)), wki = vx_بارگیری(wi + (k)); \
    VT Pr = v_sub(v_mul(wkr, Or), v_mul(wki, Oi)), Pi = v_fma(wkr, Oi, v_mul(wki, Or)); \
    VT Xkr = v_جمع_کن(Er, Pr), Xki = v_جمع_کن(Ei, Pi), Xmr = v_sub(Er, Pr), Xmi = v_sub(Pi, Ei);
#پایان_اگر

قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
ساكن مضمن فراغ ccsUntangleFwd(ثابت DftPlan& plan, ثابت T* re, ثابت T* im, T s, T* out)
{
    صحيح nc = plan.nc;
    ثابت T* wr = (ثابت T*)plan.rtw_re;
    ثابت T* wi = (ثابت T*)plan.rtw_im;
    صحيح k = 1, m = nc - 1;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
    {
        ثابت صحيح VL = VTraits<VT>::vlanes();
        ثابت VT half = v_setall_((T)0.5), vs = v_setall_(s);
        لكل (; m - k + 1 >= 2*VL; k += VL, m -= VL)
        {
            DFT_UNTANGLE_FWD_VEC(k, m, Xkr, Xki, Xmr, Xmi)
            v_store_interleave(out + k*2 - 1, v_mul(Xkr, vs), v_mul(Xki, vs));
            v_store_interleave(out + (m - VL + 1)*2 - 1, v_معکوس(v_mul(Xmr, vs)), v_معکوس(v_mul(Xmi, vs)));
        }
    }
#پایان_اگر
    لكل (; k < m; k++, m--)
    {
        DFT_UNTANGLE_FWD_SCALAR(k, m, Xkr, Xki, Xmr, Xmi)
        out[k*2-1] = Xkr*s; out[k*2] = Xki*s;
        out[m*2-1] = Xmr*s; out[m*2] = Xmi*s;
    }
    إذا ((nc & 1) == 0 && nc >= 2)
    {
        k = nc/2;
        out[k*2-1] = re[k]*s; out[k*2] = -im[k]*s;
    }
}

قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ postprocRealDFT_(ثابت DftPlan& plan, ثابت فراغ* _re, ثابت فراغ* _im, فراغ* _dst,
                      مزدوج scale, منطقي complex_output)
{
    ثابت T* re = (ثابت T*)_re; ثابت T* im = (ثابت T*)_im;
    T* dst = (T*)_dst;
    صحيح n = plan.n, nc = plan.nc;
    T s = (T)scale;
    إذا (nc == n)
    {
        // odd n: the full complex spectrum was computed
        إذا (complex_output)
            postprocDFT_<T, VT, haveSIMD>(plan, _re, _im, _dst, scale, scale);
        وإلا
        {
            dst[0] = re[0]*s;
            لكل (صحيح k = 1; k <= (n-1)/2; k++)
            {
                dst[k*2-1] = re[k]*s;
                dst[k*2] = im[k]*s;
            }
        }
        أرجع;
    }
    T X0 = (re[0] + im[0])*s, Xnc = (re[0] - im[0])*s;
    ccsUntangleFwd<T, VT, haveSIMD>(plan, re, im, s, complex_output ? dst + 1 : dst);
    إذا (complex_output)
    {
        dst[0] = X0; dst[1] = 0;
        dst[n] = Xnc; dst[n+1] = 0;
    }
    وإلا
    {
        dst[0] = X0;
        dst[n-1] = Xnc;
    }
}

قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ postprocDCT_(ثابت DftPlan& plan, ثابت فراغ* _re, ثابت فراغ* _im, فراغ* _dst, size_t dstep)
{
    ثابت T* re = (ثابت T*)_re; ثابت T* im = (ثابت T*)_im;
    T* dst = (T*)_dst;
    صحيح n = plan.n, nc = plan.nc;
    ثابت T* wr = (ثابت T*)plan.rtw_re;
    ثابت T* wi = (ثابت T*)plan.rtw_im;
    ثابت T* dr = (ثابت T*)plan.dct_re;
    ثابت T* di = (ثابت T*)plan.dct_im;
    // dst[k] = Re(X_k*w_k), dst[n-k] = -Im(X_k*w_k); w_k = s*W_4n^k (s = DCT normalization);
    // the k = 0 and k = nc terms get the extra sin45 (= 1/sqrt(2)) of DCT-II.
    T X0 = re[0] + im[0], Xnc = re[0] - im[0];
    dst[0] = X0*dr[0]*DftConst<T>::sin45;
    dst[nc*dstep] = Xnc*dr[nc];
    صحيح k = 1, m = nc - 1;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
    {
        إذا (dstep == 1)
        {
            ثابت صحيح VL = VTraits<VT>::vlanes();
            ثابت VT half = v_setall_((T)0.5);
            لكل (; m - k + 1 >= 2*VL; k += VL, m -= VL)
            {
                DFT_UNTANGLE_FWD_VEC(k, m, Xkr, Xki, Xmr, Xmi)
                VT dkr = vx_بارگیری(dr + k), dki = vx_بارگیری(di + k);
                VT dmr = v_معکوس(vx_بارگیری(dr + m - VL + 1)), dmi = v_معکوس(vx_بارگیری(di + m - VL + 1));
                v_store(dst + k, v_sub(v_mul(Xkr, dkr), v_mul(Xki, dki)));                          // dst[k+l]
                v_store(dst + n - k - VL + 1, v_معکوس(v_sub(v_mul(v_sub(v_setall_((T)0), Xkr), dki), v_mul(Xki, dkr)))); // dst[n-k-l]
                v_store(dst + m - VL + 1, v_معکوس(v_sub(v_mul(Xmr, dmr), v_mul(Xmi, dmi))));      // dst[m-l]
                v_store(dst + n - m, v_sub(v_mul(v_sub(v_setall_((T)0), Xmr), dmi), v_mul(Xmi, dmr)));   // dst[n-m+l]
            }
        }
    }
#پایان_اگر
    لكل (; k < m; k++, m--)
    {
        DFT_UNTANGLE_FWD_SCALAR(k, m, Xkr, Xki, Xmr, Xmi)
        dst[k*dstep] = Xkr*dr[k] - Xki*di[k];
        dst[(n-k)*dstep] = -(Xkr*di[k] + Xki*dr[k]);
        dst[m*dstep] = Xmr*dr[m] - Xmi*di[m];
        dst[(n-m)*dstep] = -(Xmr*di[m] + Xmi*dr[m]);
    }
    إذا ((nc & 1) == 0 && nc >= 2)
    {
        k = nc/2;
        T Xkr = re[k], Xki = -im[k];
        dst[k*dstep] = Xkr*dr[k] - Xki*di[k];
        dst[(n-k)*dstep] = -(Xkr*di[k] + Xki*dr[k]);
    }
}

#لغو_تعریف DFT_UNTANGLE_FWD_SCALAR
#لغو_تعریف DFT_UNTANGLE_FWD_VEC

قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
فراغ postprocIDCT_(ثابت DftPlan& plan, ثابت فراغ* _re, ثابت فراغ* _im, فراغ* _dst, size_t dstep)
{
    ثابت T* re = (ثابت T*)_re; ثابت T* im = (ثابت T*)_im;
    T* dst = (T*)_dst;
    صحيح n = plan.n, nc = plan.nc;
    // out[2m] = re[m], out[2m+1] = -im[m] (conjugation trick) is the CCS inverse result;
    // Makhoul reordering: dst[2j] = out[j], dst[2j+1] = out[n-1-j], i.e. for j = 2m, 2m+1:
    // dst[4m..4m+3] = (re[m], -im[nc-1-m], -im[m], re[nc-1-m]).
    صحيح j = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا constexpr (haveSIMD)
    {
        إذا (dstep == 1)
        {
            ثابت صحيح VL = VTraits<VT>::vlanes();
            ثابت VT zero = v_setall_((T)0);
            صحيح m = 0;
            لكل (; m + VL <= nc/2; m += VL)
            {
                VT a = vx_بارگیری(re + m);
                VT b = v_sub(zero, v_معکوس(vx_بارگیری(im + nc - m - VL)));
                VT c = v_sub(zero, vx_بارگیری(im + m));
                VT d = v_معکوس(vx_بارگیری(re + nc - m - VL));
                v_store_interleave(dst + m*4, a, b, c, d);
            }
            j = m*2;
        }
    }
#پایان_اگر
    // j even: out[j] = re[j/2], out[n-1-j] = -im[(n-1-j)/2]; j odd: out[j] = -im[j/2], out[n-1-j] = re[(n-1-j)/2]
    لكل (; j + 1 < nc; j += 2)
    {
        صحيح m = j >> 1, q = (n - 1 - j) >> 1;
        dst[(2*j)*dstep] = re[m];
        dst[(2*j+1)*dstep] = -im[q];
        dst[(2*j+2)*dstep] = -im[m];
        dst[(2*j+3)*dstep] = re[q];
    }
    إذا (j < nc)     // nc odd: the last j is even
    {
        صحيح m = j >> 1, q = (n - 1 - j) >> 1;
        dst[(2*j)*dstep] = re[m];
        dst[(2*j+1)*dstep] = -im[q];
    }
}

#لغو_تعریف DFT4_SCALAR

قالب<نام_نوع T, نام_نوع VT, منطقي haveSIMD>
DFTKernels makeKernels()
{
    DFTKernels k;
    k.preprocRadix0 = preprocRadix0_<T>;
    k.preprocRadix2 = preprocRadix2_<T, VT, haveSIMD>;
    k.preprocRadix4 = preprocRadix4_<T, VT, haveSIMD>;
    k.preprocRadix8 = preprocRadix8_<T, VT, haveSIMD>;
    k.preprocDCT = preprocDCT_<T, VT, haveSIMD>;
    k.preprocCCS = preprocCCS_<T, VT, haveSIMD>;
    k.preprocIDCT = preprocIDCT_<T, VT, haveSIMD>;
    k.radix3 = radix3_<T, VT, haveSIMD>;
    k.radix4 = radix4_<T, VT, haveSIMD>;
    k.radix5 = radix5_<T, VT, haveSIMD>;
    k.radixOdd = radixOdd_<T, VT, haveSIMD>;
    k.postprocDFT = postprocDFT_<T, VT, haveSIMD>;
    k.postprocRealDFT = postprocRealDFT_<T, VT, haveSIMD>;
    k.postprocReal = postprocReal_<T, VT, haveSIMD>;
    k.postprocDCT = postprocDCT_<T, VT, haveSIMD>;
    k.postprocIDCT = postprocIDCT_<T, VT, haveSIMD>;
    إذا constexpr (haveSIMD)
        k.vlanes = VTraits<VT>::vlanes();
    وإلا
        k.vlanes = 1;
    أرجع k;
}

} // namespace

DFTKernels getDFTKernels_(صحيح عمق)
{
    إذا (عمق == CV_32F)
    {
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        أرجع makeKernels<عائم, v_float32, درست>();
#وإلا
        أرجع makeKernels<عائم, عائم, خطأ>();
#پایان_اگر
    }
    CV_Assert(عمق == CV_64F);
#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    أرجع makeKernels<مزدوج, v_float64, درست>();
#وإلا
    أرجع makeKernels<مزدوج, مزدوج, خطأ>();
#پایان_اگر
}

#پایان_اگر // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_انتها
}
