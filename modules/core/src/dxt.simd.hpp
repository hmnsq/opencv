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

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/hal/intrin.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "dxt.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstring>

ਨਾਮਾਕਾਸ਼ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

DFTKernels getDFTKernels_(ਪੂਰਨ ਡੂੰਘਾਈ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ਨਾਮਾਕਾਸ਼ {

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ DftConst
{
    // sin(2*pi/3), (cos(2pi/5)-cos(4pi/5))/2, sin(2pi/5), sin(4pi/5), sqrt(2)/2
    ਸਥਿਰ ਅਚਲ T sin120, fft5_2, sin72, sin144, sin45;
};
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਅਚਲ T DftConst<T>::sin120 = (T)0.86602540378443864676372317075294;
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਅਚਲ T DftConst<T>::fft5_2 = (T)0.559016994374947424102293417182819;
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਅਚਲ T DftConst<T>::sin72  = (T)0.951056516295153572116439333379382;
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਅਚਲ T DftConst<T>::sin144 = (T)0.587785252292473129168705954639073;
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਅਚਲ T DftConst<T>::sin45  = (T)0.70710678118654752440084436210485;

// 4-point forward DFT of (x0,x1,x2,x3) -> (X0,X1,X2,X3), scalar.
// (internal temporaries are prefixed with _t so that callers may pass any names as outputs)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DFT4_SCALAR(x0r,x0i,x1r,x1i,x2r,x2i,x3r,x3i, X0r,X0i,X1r,X1i,X2r,X2i,X3r,X3i) \
    { \
        T _t0r = (x0r) + (x2r), _t0i = (x0i) + (x2i), _t1r = (x0r) - (x2r), _t1i = (x0i) - (x2i); \
        T _t2r = (x1r) + (x3r), _t2i = (x1i) + (x3i), _t3r = (x1r) - (x3r), _t3i = (x1i) - (x3i); \
        X0r = _t0r + _t2r; X0i = _t0i + _t2i; X2r = _t0r - _t2r; X2i = _t0i - _t2i; \
        X1r = _t1r + _t3i; X1i = _t1i - _t3r; X3r = _t1r - _t3i; X3i = _t1i + _t3r; \
    }

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
ਸਤਰਅੰਦਰ v_float32 v_setall_(ਦਸ਼ਮਲਵ x) { ਵਾਪਸ vx_setall_f32(x); }
// all-lanes sign-bit mask (when neg) or zero: x ^ mask negates x when neg, keeps it otherwise
ਸਤਰਅੰਦਰ v_float32 v_signmask_(ਤਾਰਕਿਕ neg, ਦਸ਼ਮਲਵ) { ਵਾਪਸ v_reinterpret_as_f32(vx_setall_u32(neg ? 0x80000000u : 0u)); }
#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ਸਤਰਅੰਦਰ v_float64 v_setall_(ਦੁੱਗਣਾ x) { ਵਾਪਸ vx_setall_f64(x); }
ਸਤਰਅੰਦਰ v_float64 v_signmask_(ਤਾਰਕਿਕ neg, ਦੁੱਗਣਾ) { ਵਾਪਸ v_reinterpret_as_f64(vx_setall_u64(neg ? 0x8000000000000000ULL : 0ULL)); }
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

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

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਖਾਲੀ preprocRadix0_(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* _src, size_t sstep, ਖਾਲੀ* _re, ਖਾਲੀ* _im, ਤਾਰਕਿਕ conj)
{
    ਅਚਲ T* src = (ਅਚਲ T*)_src;
    T* re = (T*)_re; T* im = (T*)_im;
    ਅਚਲ ਪੂਰਨ* itab = plan.itab.ਅੰਕੜੇ();
    ਪੂਰਨ nc = plan.nc;
    ਜੇ (plan.real_ਨਿਵੇਸ਼)
    {
        ਲਈ (ਪੂਰਨ i = 0; i < nc; i++)
        {
            re[i] = src[itab[i*2]*sstep];
            im[i] = 0;
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ (!conj)
    {
        ਲਈ (ਪੂਰਨ i = 0; i < nc; i++)
        {
            re[i] = src[itab[i*2]*sstep];
            im[i] = src[itab[i*2+1]*sstep];
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ (ਪੂਰਨ i = 0; i < nc; i++)
        {
            re[i] = src[itab[i*2]*sstep];
            im[i] = -src[itab[i*2+1]*sstep];
        }
    }
}

// scalar r0-point DFTs of one column j (used for the vector tail and the no-SIMD build)
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਪਹਿਲਾ_Stage2_scalar(ਅਚਲ T* x, ਪੂਰਨ M, ਪੂਰਨ j, T* re, T* im, T conjsign)
{
    T x0r = x[j*2], x0i = conjsign*x[j*2+1], x1r = x[(j+M)*2], x1i = conjsign*x[(j+M)*2+1];
    re[0] = x0r + x1r; im[0] = x0i + x1i;
    re[1] = x0r - x1r; im[1] = x0i - x1i;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਪਹਿਲਾ_Stage4_scalar(ਅਚਲ T* x, ਪੂਰਨ M, ਪੂਰਨ j, T* re, T* im, T conjsign)
{
    T x0r = x[j*2], x0i = conjsign*x[j*2+1];
    T x1r = x[(j+M)*2], x1i = conjsign*x[(j+M)*2+1];
    T x2r = x[(j+2*M)*2], x2i = conjsign*x[(j+2*M)*2+1];
    T x3r = x[(j+3*M)*2], x3i = conjsign*x[(j+3*M)*2+1];
    DFT4_SCALAR(x0r,x0i,x1r,x1i,x2r,x2i,x3r,x3i, re[0],im[0],re[1],im[1],re[2],im[2],re[3],im[3]);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਪਹਿਲਾ_Stage8_scalar(ਅਚਲ T* x, ਪੂਰਨ M, ਪੂਰਨ j, T* re, T* im, T conjsign)
{
    ਅਚਲ T c = DftConst<T>::sin45;
    T xr[8], xi[8];
    ਲਈ (ਪੂਰਨ q = 0; q < 8; q++)
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

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)

// 4-point forward DFT on vectors
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DFT4_VEC(x0r,x0i,x1r,x1i,x2r,x2i,x3r,x3i, X0r,X0i,X1r,X1i,X2r,X2i,X3r,X3i) \
    { \
        VT _t0r = v_ਜੋੜੋ(x0r, x2r), _t0i = v_ਜੋੜੋ(x0i, x2i), _t1r = v_sub(x0r, x2r), _t1i = v_sub(x0i, x2i); \
        VT _t2r = v_ਜੋੜੋ(x1r, x3r), _t2i = v_ਜੋੜੋ(x1i, x3i), _t3r = v_sub(x1r, x3r), _t3i = v_sub(x1i, x3i); \
        X0r = v_ਜੋੜੋ(_t0r, _t2r); X0i = v_ਜੋੜੋ(_t0i, _t2i); X2r = v_sub(_t0r, _t2r); X2i = v_sub(_t0i, _t2i); \
        X1r = v_ਜੋੜੋ(_t1r, _t3i); X1i = v_sub(_t1i, _t3r); X3r = v_sub(_t1r, _t3i); X3i = v_ਜੋੜੋ(_t1i, _t3r); \
    }

// loads leg q of lanes j..j+VL-1: x[(j + q*M)*2 ...]; imsign (all sign bits or zero, computed
// once per call) conjugates the value without a branch
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DFT_ਲੱਦੋ_LEG(q, xr, xi) \
    v_ਲੱਦੋ_deinterleave(x + (j + (q)*M)*2, xr, xi); \
    xi = v_xor(xi, imsign);

// scatters the r0 outputs of every lane: buf holds them lane-major (r0 consecutive values per lane)
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਪੂਰਨ R0>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scatterLanes(ਅਚਲ T* bufr, ਅਚਲ T* bufi, T* re, T* im, ਅਚਲ ਪੂਰਨ* gtab, ਪੂਰਨ VL)
{
    ਲਈ (ਪੂਰਨ l = 0; l < VL; l++)
    {
        ਪੂਰਨ ਅਧਾਰ = gtab[l];
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(re + ਅਧਾਰ, bufr + l*R0, R0*ਆਕਾਰ(T));
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(im + ਅਧਾਰ, bufi + l*R0, R0*ਆਕਾਰ(T));
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT>
ਸਥਿਰ ਖਾਲੀ ਪਹਿਲਾ_Stage2_vec(ਅਚਲ T* x, ਪੂਰਨ M, ਅਚਲ ਪੂਰਨ* gtab, T* re, T* im, ਤਾਰਕਿਕ conj)
{
    ਅਚਲ ਪੂਰਨ VL = VTraits<VT>::vlanes();
    ਅਚਲ VT imsign = v_signmask_(conj, (T)0);
    T bufr[2*VTraits<VT>::max_nlanes], bufi[2*VTraits<VT>::max_nlanes];
    ਪੂਰਨ j = 0;
    ਲਈ (; j + VL <= M; j += VL)
    {
        VT x0r, x0i, x1r, x1i;
        DFT_ਲੱਦੋ_LEG(0, x0r, x0i)
        DFT_ਲੱਦੋ_LEG(1, x1r, x1i)
        v_store_interleave(bufr, v_ਜੋੜੋ(x0r, x1r), v_sub(x0r, x1r));
        v_store_interleave(bufi, v_ਜੋੜੋ(x0i, x1i), v_sub(x0i, x1i));
        scatterLanes<T, 2>(bufr, bufi, re, im, gtab + j, VL);
    }
    T conjsign = conj ? (T)-1 : (T)1;
    ਲਈ (; j < M; j++)
        ਪਹਿਲਾ_Stage2_scalar(x, M, j, re + gtab[j], im + gtab[j], conjsign);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT>
ਸਥਿਰ ਖਾਲੀ ਪਹਿਲਾ_Stage4_vec(ਅਚਲ T* x, ਪੂਰਨ M, ਅਚਲ ਪੂਰਨ* gtab, T* re, T* im, ਤਾਰਕਿਕ conj)
{
    ਅਚਲ ਪੂਰਨ VL = VTraits<VT>::vlanes();
    ਅਚਲ VT imsign = v_signmask_(conj, (T)0);
    T bufr[4*VTraits<VT>::max_nlanes], bufi[4*VTraits<VT>::max_nlanes];
    ਪੂਰਨ j = 0;
    ਲਈ (; j + VL <= M; j += VL)
    {
        VT x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i, y0r, y0i, y1r, y1i, y2r, y2i, y3r, y3i;
        DFT_ਲੱਦੋ_LEG(0, x0r, x0i)
        DFT_ਲੱਦੋ_LEG(1, x1r, x1i)
        DFT_ਲੱਦੋ_LEG(2, x2r, x2i)
        DFT_ਲੱਦੋ_LEG(3, x3r, x3i)
        DFT4_VEC(x0r,x0i,x1r,x1i,x2r,x2i,x3r,x3i, y0r,y0i,y1r,y1i,y2r,y2i,y3r,y3i);
        v_store_interleave(bufr, y0r, y1r, y2r, y3r);
        v_store_interleave(bufi, y0i, y1i, y2i, y3i);
        scatterLanes<T, 4>(bufr, bufi, re, im, gtab + j, VL);
    }
    T conjsign = conj ? (T)-1 : (T)1;
    ਲਈ (; j < M; j++)
        ਪਹਿਲਾ_Stage4_scalar(x, M, j, re + gtab[j], im + gtab[j], conjsign);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT>
ਸਥਿਰ ਖਾਲੀ ਪਹਿਲਾ_Stage8_vec(ਅਚਲ T* x, ਪੂਰਨ M, ਅਚਲ ਪੂਰਨ* gtab, T* re, T* im, ਤਾਰਕਿਕ conj)
{
    ਅਚਲ ਪੂਰਨ VL = VTraits<VT>::vlanes();
    ਅਚਲ VT zero = v_setall_((T)0), imsign = v_signmask_(conj, (T)0);
    ਅਚਲ VT c = v_setall_((T)DftConst<T>::sin45);
    T bufr[8*VTraits<VT>::max_nlanes], bufi[8*VTraits<VT>::max_nlanes];
    ਪੂਰਨ j = 0;
    ਲਈ (; j + VL <= M; j += VL)
    {
        VT x0r, x0i, x1r, x1i, x2r, x2i, x3r, x3i, x4r, x4i, x5r, x5i, x6r, x6i, x7r, x7i;
        VT e0r, e0i, e1r, e1i, e2r, e2i, e3r, e3i, o0r, o0i, o1r, o1i, o2r, o2i, o3r, o3i;
        DFT_ਲੱਦੋ_LEG(0, x0r, x0i)
        DFT_ਲੱਦੋ_LEG(1, x1r, x1i)
        DFT_ਲੱਦੋ_LEG(2, x2r, x2i)
        DFT_ਲੱਦੋ_LEG(3, x3r, x3i)
        DFT_ਲੱਦੋ_LEG(4, x4r, x4i)
        DFT_ਲੱਦੋ_LEG(5, x5r, x5i)
        DFT_ਲੱਦੋ_LEG(6, x6r, x6i)
        DFT_ਲੱਦੋ_LEG(7, x7r, x7i)
        DFT4_VEC(x0r,x0i,x2r,x2i,x4r,x4i,x6r,x6i, e0r,e0i,e1r,e1i,e2r,e2i,e3r,e3i);
        DFT4_VEC(x1r,x1i,x3r,x3i,x5r,x5i,x7r,x7i, o0r,o0i,o1r,o1i,o2r,o2i,o3r,o3i);
        // twiddle the odd part: W8^1 = c(1-i), W8^2 = -i, W8^3 = c(-1-i)
        VT t1r = v_mul(c, v_ਜੋੜੋ(o1r, o1i)), t1i = v_mul(c, v_sub(o1i, o1r));
        VT t2r = o2i, t2i = v_sub(zero, o2r);
        VT t3r = v_mul(c, v_sub(o3i, o3r)), t3i = v_sub(zero, v_mul(c, v_ਜੋੜੋ(o3r, o3i)));
        // outputs 0..3 -> buf[0..4*VL), 4..7 -> buf[4*VL..8*VL) (lane-major within each half)
        v_store_interleave(bufr, v_ਜੋੜੋ(e0r, o0r), v_ਜੋੜੋ(e1r, t1r), v_ਜੋੜੋ(e2r, t2r), v_ਜੋੜੋ(e3r, t3r));
        v_store_interleave(bufi, v_ਜੋੜੋ(e0i, o0i), v_ਜੋੜੋ(e1i, t1i), v_ਜੋੜੋ(e2i, t2i), v_ਜੋੜੋ(e3i, t3i));
        v_store_interleave(bufr + 4*VL, v_sub(e0r, o0r), v_sub(e1r, t1r), v_sub(e2r, t2r), v_sub(e3r, t3r));
        v_store_interleave(bufi + 4*VL, v_sub(e0i, o0i), v_sub(e1i, t1i), v_sub(e2i, t2i), v_sub(e3i, t3i));
        ਲਈ (ਪੂਰਨ l = 0; l < VL; l++)
        {
            ਪੂਰਨ ਅਧਾਰ = gtab[j + l];
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(re + ਅਧਾਰ, bufr + l*4, 4*ਆਕਾਰ(T));
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(re + ਅਧਾਰ + 4, bufr + 4*VL + l*4, 4*ਆਕਾਰ(T));
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(im + ਅਧਾਰ, bufi + l*4, 4*ਆਕਾਰ(T));
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(im + ਅਧਾਰ + 4, bufi + 4*VL + l*4, 4*ਆਕਾਰ(T));
        }
    }
    T conjsign = conj ? (T)-1 : (T)1;
    ਲਈ (; j < M; j++)
        ਪਹਿਲਾ_Stage8_scalar(x, M, j, re + gtab[j], im + gtab[j], conjsign);
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DFT_ਲੱਦੋ_LEG
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DFT4_VEC

#ਜੇ_ਅੰਤ // CV_SIMD || CV_SIMD_SCALABLE

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ preprocRadix2_(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* _src, size_t sstep, ਖਾਲੀ* _re, ਖਾਲੀ* _im, ਤਾਰਕਿਕ conj)
{
    CV_DbgAssert(sstep == 1); CV_ਅਣਵਰਤਿਆ(sstep);
    ਅਚਲ T* x = (ਅਚਲ T*)_src;
    T* re = (T*)_re; T* im = (T*)_im;
    ਅਚਲ ਪੂਰਨ* gtab = plan.itab.ਅੰਕੜੇ();
    ਪੂਰਨ M = plan.nc/2;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
        ਪਹਿਲਾ_Stage2_vec<T, VT>(x, M, gtab, re, im, conj);
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
        T conjsign = conj ? (T)-1 : (T)1;
        ਲਈ (ਪੂਰਨ j = 0; j < M; j++)
            ਪਹਿਲਾ_Stage2_scalar(x, M, j, re + gtab[j], im + gtab[j], conjsign);
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ preprocRadix4_(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* _src, size_t sstep, ਖਾਲੀ* _re, ਖਾਲੀ* _im, ਤਾਰਕਿਕ conj)
{
    CV_DbgAssert(sstep == 1); CV_ਅਣਵਰਤਿਆ(sstep);
    ਅਚਲ T* x = (ਅਚਲ T*)_src;
    T* re = (T*)_re; T* im = (T*)_im;
    ਅਚਲ ਪੂਰਨ* gtab = plan.itab.ਅੰਕੜੇ();
    ਪੂਰਨ M = plan.nc/4;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
        ਪਹਿਲਾ_Stage4_vec<T, VT>(x, M, gtab, re, im, conj);
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
        T conjsign = conj ? (T)-1 : (T)1;
        ਲਈ (ਪੂਰਨ j = 0; j < M; j++)
            ਪਹਿਲਾ_Stage4_scalar(x, M, j, re + gtab[j], im + gtab[j], conjsign);
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ preprocRadix8_(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* _src, size_t sstep, ਖਾਲੀ* _re, ਖਾਲੀ* _im, ਤਾਰਕਿਕ conj)
{
    CV_DbgAssert(sstep == 1); CV_ਅਣਵਰਤਿਆ(sstep);
    ਅਚਲ T* x = (ਅਚਲ T*)_src;
    T* re = (T*)_re; T* im = (T*)_im;
    ਅਚਲ ਪੂਰਨ* gtab = plan.itab.ਅੰਕੜੇ();
    ਪੂਰਨ M = plan.nc/8;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
        ਪਹਿਲਾ_Stage8_vec<T, VT>(x, M, gtab, re, im, conj);
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
        T conjsign = conj ? (T)-1 : (T)1;
        ਲਈ (ਪੂਰਨ j = 0; j < M; j++)
            ਪਹਿਲਾ_Stage8_scalar(x, M, j, re + gtab[j], im + gtab[j], conjsign);
    }
}

// DCT input permutation (Makhoul): v[j] = x[2j], v[n-1-j] = x[2j+1], j in [0, n/2), into the temp
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ preprocDCT_(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* _src, size_t sstep, ਖਾਲੀ* _tmp)
{
    ਅਚਲ T* x = (ਅਚਲ T*)_src;
    T* v = (T*)_tmp;
    ਪੂਰਨ n = plan.n, nc = plan.nc, j = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
    {
        ਜੇ (sstep == 1)
        {
            ਅਚਲ ਪੂਰਨ VL = VTraits<VT>::vlanes();
            ਲਈ (; j + VL <= nc; j += VL)
            {
                VT even, odd;
                v_ਲੱਦੋ_deinterleave(x + j*2, even, odd);
                v_store(v + j, even);
                v_store(v + n - j - VL, v_ਉਲਟ(odd));
            }
        }
    }
#ਜੇ_ਅੰਤ
    ਲਈ (; j < nc; j++)
    {
        v[j] = x[(2*j)*sstep];
        v[n - 1 - j] = x[(2*j + 1)*sstep];
    }
}

// Inverse untangle: the "2Z" spectrum of z[j] = x[2j] + i*x[2j+1] from X[k], X[nc-k]:
//   A = X[k] + conj(X[nc-k]), B = X[k] - conj(X[nc-k]), C = W_n^{-k}*B,
//   2Z[k] = A + i*C = (Ar - Ci, Ai + Cr), 2Z[nc-k] = conj(A) + i*conj(C) = (Ar + Ci, Cr - Ai)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DFT_UNTANGLE_INV_SCALAR(k, akr, aki, amr, ami, zkr, zki, zmr, zmi) \
    T Ar = akr + amr, Ai = aki - ami; \
    T Br = akr - amr, Bi = aki + ami; \
    T Cr = wr[k]*Br + wi[k]*Bi, Ci = wr[k]*Bi - wi[k]*Br; \
    T zkr = Ar - Ci, zki = Ai + Cr, zmr = Ar + Ci, zmi = Cr - Ai;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DFT_UNTANGLE_INV_VEC(k, akr, aki, amr, ami, zkr, zki, zmr, zmi) \
    VT Ar = v_ਜੋੜੋ(akr, amr), Ai = v_sub(aki, ami); \
    VT Br = v_sub(akr, amr), Bi = v_ਜੋੜੋ(aki, ami); \
    VT wkr = vx_ਲੱਦੋ(wr + (k)), wki = vx_ਲੱਦੋ(wi + (k)); \
    VT Cr = v_fma(wkr, Br, v_mul(wki, Bi)), Ci = v_sub(v_mul(wkr, Bi), v_mul(wki, Br)); \
    VT zkr = v_sub(Ar, Ci), zki = v_ਜੋੜੋ(Ai, Cr), zmr = v_ਜੋੜੋ(Ar, Ci), zmi = v_sub(Cr, Ai);
#ਜੇ_ਅੰਤ

// x = interleaved (re, im) pairs: X[k] = (x[2k], x[2k+1]) for k in [1, nc-1]; tmp receives the
// interleaved 2Z values; the driver then runs the complex nc-point transform on it with conj = true.
// The factor 2 is what makes scale = 1/n correct for the nc-point inverse (nc*2*z = n*z).
// 2Z[0] = (X0+Xnc) + i(X0-Xnc), 2Z[nc/2] = 2*conj(X[n/4]).
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ccsUntangleInv(ਅਚਲ DftPlan& plan, T X0, T Xnc, ਅਚਲ T* x, T* tmp)
{
    ਪੂਰਨ nc = plan.nc;
    ਅਚਲ T* wr = (ਅਚਲ T*)plan.rtw_re;
    ਅਚਲ T* wi = (ਅਚਲ T*)plan.rtw_im;
    tmp[0] = X0 + Xnc;
    tmp[1] = X0 - Xnc;
    ਪੂਰਨ k = 1, m = nc - 1;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
    {
        ਅਚਲ ਪੂਰਨ VL = VTraits<VT>::vlanes();
        ਲਈ (; m - k + 1 >= 2*VL; k += VL, m -= VL)
        {
            VT akr, aki, amr, ami;
            v_ਲੱਦੋ_deinterleave(x + k*2, akr, aki);
            v_ਲੱਦੋ_deinterleave(x + (m - VL + 1)*2, amr, ami);
            amr = v_ਉਲਟ(amr); ami = v_ਉਲਟ(ami);
            DFT_UNTANGLE_INV_VEC(k, akr, aki, amr, ami, zkr, zki, zmr, zmi)
            v_store_interleave(tmp + k*2, zkr, zki);
            v_store_interleave(tmp + (m - VL + 1)*2, v_ਉਲਟ(zmr), v_ਉਲਟ(zmi));
        }
    }
#ਜੇ_ਅੰਤ
    ਲਈ (; k < m; k++, m--)
    {
        T akr = x[k*2], aki = x[k*2+1], amr = x[m*2], ami = x[m*2+1];
        DFT_UNTANGLE_INV_SCALAR(k, akr, aki, amr, ami, zkr, zki, zmr, zmi)
        tmp[k*2] = zkr; tmp[k*2+1] = zki;
        tmp[m*2] = zmr; tmp[m*2+1] = zmi;
    }
    ਜੇ ((nc & 1) == 0 && nc >= 2)
    {
        k = nc/2;
        tmp[k*2] = x[k*2]*2;
        tmp[k*2+1] = -x[k*2+1]*2;
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ preprocCCS_(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* _src, ਖਾਲੀ* _tmp, ਤਾਰਕਿਕ complex_ਨਿਵੇਸ਼)
{
    ਅਚਲ T* src = (ਅਚਲ T*)_src;
    T* tmp = (T*)_tmp;
    ਪੂਰਨ n = plan.n, nc = plan.nc;
    ਜੇ (nc == n)
    {
        // odd n: expand the packed half-spectrum into the full conjugate-symmetric spectrum
        tmp[0] = src[0]; tmp[1] = 0;
        ਅਚਲ T* x = complex_ਨਿਵੇਸ਼ ? src + 2 : src + 1;   // X[k] = (x[2(k-1)], x[2(k-1)+1])
        ਲਈ (ਪੂਰਨ k = 1; k <= (n-1)/2; k++)
        {
            T xr = x[(k-1)*2], xi = x[(k-1)*2+1];
            tmp[k*2] = xr; tmp[k*2+1] = xi;
            tmp[(n-k)*2] = xr; tmp[(n-k)*2+1] = -xi;
        }
        ਵਾਪਸ;
    }
    T X0 = src[0], Xnc = complex_ਨਿਵੇਸ਼ ? src[n] : src[n-1];
    // CCS: X[k] = (src[2k-1], src[2k]); complex: X[k] = (src[2k], src[2k+1])
    ccsUntangleInv<T, VT, haveSIMD>(plan, X0, Xnc, complex_ਨਿਵੇਸ਼ ? src : src - 1, tmp);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ preprocIDCT_(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* _src, size_t sstep, ਖਾਲੀ* _tmp)
{
    ਅਚਲ T* src = (ਅਚਲ T*)_src;
    T* tmp = (T*)_tmp;
    ਪੂਰਨ n = plan.n, nc = plan.nc;
    ਅਚਲ T* dr = (ਅਚਲ T*)plan.dct_re;
    ਅਚਲ T* di = (ਅਚਲ T*)plan.dct_im;
    ਅਚਲ T* wr = (ਅਚਲ T*)plan.rtw_re;
    ਅਚਲ T* wi = (ਅਚਲ T*)plan.rtw_im;
    // pre-twiddle: X[k] = conj(w_k) * (src[k] - i*src[n-k]), k in [1, nc-1],
    // X0 = 2*src[0]*w_0*sin45, Xnc = 2*src[nc]*w_nc (the two special normalizations of DCT-III).
    T X0 = src[0]*2*dr[0]*DftConst<T>::sin45;
    T Xnc = src[nc*sstep]*2*dr[nc];
    tmp[0] = X0 + Xnc;
    tmp[1] = X0 - Xnc;
    ਪੂਰਨ k = 1, m = nc - 1;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
    {
        ਜੇ (sstep == 1)
        {
            ਅਚਲ ਪੂਰਨ VL = VTraits<VT>::vlanes();
            ਲਈ (; m - k + 1 >= 2*VL; k += VL, m -= VL)
            {
                VT a = vx_ਲੱਦੋ(src + k), b = v_ਉਲਟ(vx_ਲੱਦੋ(src + n - k - VL + 1));   // src[k+l], src[n-k-l]
                VT dkr = vx_ਲੱਦੋ(dr + k), dki = vx_ਲੱਦੋ(di + k);
                VT akr = v_sub(v_mul(dkr, a), v_mul(dki, b)), aki = v_sub(v_sub(v_setall_((T)0), v_mul(dkr, b)), v_mul(dki, a));
                a = v_ਉਲਟ(vx_ਲੱਦੋ(src + m - VL + 1)); b = vx_ਲੱਦੋ(src + n - m);      // src[m-l], src[n-m+l]
                VT dmr = v_ਉਲਟ(vx_ਲੱਦੋ(dr + m - VL + 1)), dmi = v_ਉਲਟ(vx_ਲੱਦੋ(di + m - VL + 1));
                VT amr = v_sub(v_mul(dmr, a), v_mul(dmi, b)), ami = v_sub(v_sub(v_setall_((T)0), v_mul(dmr, b)), v_mul(dmi, a));
                DFT_UNTANGLE_INV_VEC(k, akr, aki, amr, ami, zkr, zki, zmr, zmi)
                v_store_interleave(tmp + k*2, zkr, zki);
                v_store_interleave(tmp + (m - VL + 1)*2, v_ਉਲਟ(zmr), v_ਉਲਟ(zmi));
            }
        }
    }
#ਜੇ_ਅੰਤ
    ਲਈ (; k < m; k++, m--)
    {
        T a = src[k*sstep], b = src[(n-k)*sstep];
        T akr = dr[k]*a - di[k]*b, aki = -dr[k]*b - di[k]*a;
        a = src[m*sstep]; b = src[(n-m)*sstep];
        T amr = dr[m]*a - di[m]*b, ami = -dr[m]*b - di[m]*a;
        DFT_UNTANGLE_INV_SCALAR(k, akr, aki, amr, ami, zkr, zki, zmr, zmi)
        tmp[k*2] = zkr; tmp[k*2+1] = zki;
        tmp[m*2] = zmr; tmp[m*2+1] = zmi;
    }
    ਜੇ ((nc & 1) == 0 && nc >= 2)
    {
        k = nc/2;
        T a = src[k*sstep], b = src[(n-k)*sstep];
        tmp[k*2] = (dr[k]*a - di[k]*b)*2;
        tmp[k*2+1] = -(-dr[k]*b - di[k]*a)*2;
    }
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DFT_UNTANGLE_INV_SCALAR
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DFT_UNTANGLE_INV_VEC

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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DFT_CMUL(xr, xi, ar, ai, wr, wi) { xr = (ar)*(wr) - (ai)*(wi); xi = (ar)*(wi) + (ai)*(wr); }

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਖਾਲੀ radix3_scalar(ਅਚਲ DftStage& st, ਅਚਲ T* sre, ਅਚਲ T* sim, T* dre, T* dim)
{
    ਅਚਲ T* twr = (ਅਚਲ T*)st.tw_re; ਅਚਲ T* twi = (ਅਚਲ T*)st.tw_im;
    ਪੂਰਨ L = st.ਵਿਸਤਾਰ, G = st.ngroups, ts = st.tw_stride;
    ਅਚਲ T s120 = DftConst<T>::sin120;
    ਲਈ (ਪੂਰਨ g = 0; g < G; g++)
    {
        ਪੂਰਨ ਅਧਾਰ = g*3*L;
        ਲਈ (ਪੂਰਨ j = 0; j < L; j++)
        {
            ਪੂਰਨ i0 = ਅਧਾਰ + j, i1 = i0 + L, i2 = i1 + L;
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

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਖਾਲੀ radix4_scalar(ਅਚਲ DftStage& st, ਅਚਲ T* sre, ਅਚਲ T* sim, T* dre, T* dim)
{
    ਅਚਲ T* twr = (ਅਚਲ T*)st.tw_re; ਅਚਲ T* twi = (ਅਚਲ T*)st.tw_im;
    ਪੂਰਨ L = st.ਵਿਸਤਾਰ, G = st.ngroups, ts = st.tw_stride;
    ਲਈ (ਪੂਰਨ g = 0; g < G; g++)
    {
        ਪੂਰਨ ਅਧਾਰ = g*4*L;
        ਲਈ (ਪੂਰਨ j = 0; j < L; j++)
        {
            ਪੂਰਨ i0 = ਅਧਾਰ + j, i1 = i0 + L, i2 = i1 + L, i3 = i2 + L;
            T x0r = sre[i0], x0i = sim[i0], x1r, x1i, x2r, x2i, x3r, x3i;
            DFT_CMUL(x1r, x1i, sre[i1], sim[i1], twr[j], twi[j]);
            DFT_CMUL(x2r, x2i, sre[i2], sim[i2], twr[ts + j], twi[ts + j]);
            DFT_CMUL(x3r, x3i, sre[i3], sim[i3], twr[ts*2 + j], twi[ts*2 + j]);
            DFT4_SCALAR(x0r,x0i,x1r,x1i,x2r,x2i,x3r,x3i,
                        dre[i0],dim[i0],dre[i1],dim[i1],dre[i2],dim[i2],dre[i3],dim[i3]);
        }
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਖਾਲੀ radix5_scalar(ਅਚਲ DftStage& st, ਅਚਲ T* sre, ਅਚਲ T* sim, T* dre, T* dim)
{
    ਅਚਲ T* twr = (ਅਚਲ T*)st.tw_re; ਅਚਲ T* twi = (ਅਚਲ T*)st.tw_im;
    ਪੂਰਨ L = st.ਵਿਸਤਾਰ, G = st.ngroups, ts = st.tw_stride;
    ਅਚਲ T c5 = DftConst<T>::fft5_2, s1 = DftConst<T>::sin72, s2 = DftConst<T>::sin144;
    ਲਈ (ਪੂਰਨ g = 0; g < G; g++)
    {
        ਪੂਰਨ ਅਧਾਰ = g*5*L;
        ਲਈ (ਪੂਰਨ j = 0; j < L; j++)
        {
            ਪੂਰਨ i0 = ਅਧਾਰ + j, i1 = i0 + L, i2 = i1 + L, i3 = i2 + L, i4 = i3 + L;
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
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਖਾਲੀ radixOdd_scalar(ਅਚਲ DftStage& st, ਅਚਲ T* sre, ਅਚਲ T* sim, T* dre, T* dim, T* scratch)
{
    ਅਚਲ T* twr = (ਅਚਲ T*)st.tw_re; ਅਚਲ T* twi = (ਅਚਲ T*)st.tw_im;
    ਅਚਲ T* cs = (ਅਚਲ T*)st.cs; ਅਚਲ T* sn = (ਅਚਲ T*)st.sn;
    ਪੂਰਨ L = st.ਵਿਸਤਾਰ, G = st.ngroups, ts = st.tw_stride, r = st.radix, h = (r - 1)/2;
    T* ar = scratch; T* ai = ar + h; T* br = ai + h; T* bi = br + h;
    ਲਈ (ਪੂਰਨ g = 0; g < G; g++)
    {
        ਪੂਰਨ ਅਧਾਰ = g*r*L;
        ਲਈ (ਪੂਰਨ j = 0; j < L; j++)
        {
            ਪੂਰਨ i0 = ਅਧਾਰ + j;
            T x0r = sre[i0], x0i = sim[i0];
            T X0r = x0r, X0i = x0i;
            ਲਈ (ਪੂਰਨ p = 1; p <= h; p++)
            {
                ਪੂਰਨ ip = i0 + p*L, iq = i0 + (r-p)*L;
                T xpr, xpi, xqr, xqi;
                DFT_CMUL(xpr, xpi, sre[ip], sim[ip], twr[(p-1)*ts + j], twi[(p-1)*ts + j]);
                DFT_CMUL(xqr, xqi, sre[iq], sim[iq], twr[(r-p-1)*ts + j], twi[(r-p-1)*ts + j]);
                ar[p-1] = xpr + xqr; ai[p-1] = xpi + xqi;
                br[p-1] = xpr - xqr; bi[p-1] = xpi - xqi;
                X0r += ar[p-1]; X0i += ai[p-1];
            }
            dre[i0] = X0r; dim[i0] = X0i;
            ਲਈ (ਪੂਰਨ k = 1; k <= h; k++)
            {
                T sr = x0r, si = x0i, tr = 0, ti = 0;
                ਲਈ (ਪੂਰਨ p = 1; p <= h; p++)
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

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DFT_CMUL

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)

// dst = a*s + c, s is a scalar constant of the stage (the compiler hoists the broadcast)
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ VT, ਕਿਸਮ_ਨਾਂ T>
ਸਤਰਅੰਦਰ VT v_fma_n(ਅਚਲ VT& a, T s, ਅਚਲ VT& c) { ਵਾਪਸ v_fma(a, v_setall_(s), c); }

// (xr, xi) = (ar, ai) * (wr, wi)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DFT_VCMUL(xr, xi, ar, ai, wr, wi) \
    { xr = v_sub(v_mul(ar, wr), v_mul(ai, wi)); xi = v_fma(ar, wi, v_mul(ai, wr)); }

// the flat loop of the brief (section 5): clamp, body, branchless wrap
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DFT_VLOOP_BEGIN(r) \
    ਪੂਰਨ j = 0, ਅਧਾਰ = 0; \
    ਲਈ (ਪੂਰਨ t = 0; t < st.nsteps; t++) \
    { \
        j = std::min(j, L - VL);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DFT_VLOOP_ਸਿਰਾ(r) \
        ਪੂਰਨ nj = j + VL; \
        ਪੂਰਨ wrap = -(ਪੂਰਨ)(nj >= L); \
        ਅਧਾਰ += ((r)*L) & wrap; \
        j = nj & ~wrap; \
    }

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT>
ਖਾਲੀ radix3_vec(ਅਚਲ DftStage& st, ਅਚਲ T* sre, ਅਚਲ T* sim, T* dre, T* dim)
{
    ਅਚਲ T* twr = (ਅਚਲ T*)st.tw_re; ਅਚਲ T* twi = (ਅਚਲ T*)st.tw_im;
    ਅਚਲ ਪੂਰਨ L = st.ਵਿਸਤਾਰ, ts = st.tw_stride, VL = VTraits<VT>::vlanes();
    ਅਚਲ VT s120 = v_setall_((T)DftConst<T>::sin120), half = v_setall_((T)0.5);
    DFT_VLOOP_BEGIN(3)
        ਪੂਰਨ i0 = ਅਧਾਰ + j, i1 = i0 + L, i2 = i1 + L;
        VT x0r = vx_ਲੱਦੋ(sre + i0), x0i = vx_ਲੱਦੋ(sim + i0), x1r, x1i, x2r, x2i;
        DFT_VCMUL(x1r, x1i, vx_ਲੱਦੋ(sre + i1), vx_ਲੱਦੋ(sim + i1), vx_ਲੱਦੋ(twr + j), vx_ਲੱਦੋ(twi + j));
        DFT_VCMUL(x2r, x2i, vx_ਲੱਦੋ(sre + i2), vx_ਲੱਦੋ(sim + i2), vx_ਲੱਦੋ(twr + ts + j), vx_ਲੱਦੋ(twi + ts + j));
        VT sr = v_ਜੋੜੋ(x1r, x2r), si = v_ਜੋੜੋ(x1i, x2i);
        VT dr = v_mul(s120, v_sub(x1r, x2r)), di = v_mul(s120, v_sub(x1i, x2i));
        VT mr = v_sub(x0r, v_mul(half, sr)), mi = v_sub(x0i, v_mul(half, si));
        v_store(dre + i0, v_ਜੋੜੋ(x0r, sr)); v_store(dim + i0, v_ਜੋੜੋ(x0i, si));
        v_store(dre + i1, v_ਜੋੜੋ(mr, di)); v_store(dim + i1, v_sub(mi, dr));
        v_store(dre + i2, v_sub(mr, di)); v_store(dim + i2, v_ਜੋੜੋ(mi, dr));
    DFT_VLOOP_ਸਿਰਾ(3)
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT>
ਖਾਲੀ radix4_vec(ਅਚਲ DftStage& st, ਅਚਲ T* sre, ਅਚਲ T* sim, T* dre, T* dim)
{
    ਅਚਲ T* twr = (ਅਚਲ T*)st.tw_re; ਅਚਲ T* twi = (ਅਚਲ T*)st.tw_im;
    ਅਚਲ ਪੂਰਨ L = st.ਵਿਸਤਾਰ, ts = st.tw_stride, VL = VTraits<VT>::vlanes();
    DFT_VLOOP_BEGIN(4)
        ਪੂਰਨ i0 = ਅਧਾਰ + j, i1 = i0 + L, i2 = i1 + L, i3 = i2 + L;
        VT x0r = vx_ਲੱਦੋ(sre + i0), x0i = vx_ਲੱਦੋ(sim + i0), x1r, x1i, x2r, x2i, x3r, x3i;
        DFT_VCMUL(x1r, x1i, vx_ਲੱਦੋ(sre + i1), vx_ਲੱਦੋ(sim + i1), vx_ਲੱਦੋ(twr + j), vx_ਲੱਦੋ(twi + j));
        DFT_VCMUL(x2r, x2i, vx_ਲੱਦੋ(sre + i2), vx_ਲੱਦੋ(sim + i2), vx_ਲੱਦੋ(twr + ts + j), vx_ਲੱਦੋ(twi + ts + j));
        DFT_VCMUL(x3r, x3i, vx_ਲੱਦੋ(sre + i3), vx_ਲੱਦੋ(sim + i3), vx_ਲੱਦੋ(twr + ts*2 + j), vx_ਲੱਦੋ(twi + ts*2 + j));
        VT y0r = v_ਜੋੜੋ(x0r, x2r), y0i = v_ਜੋੜੋ(x0i, x2i), y1r = v_sub(x0r, x2r), y1i = v_sub(x0i, x2i);
        VT y2r = v_ਜੋੜੋ(x1r, x3r), y2i = v_ਜੋੜੋ(x1i, x3i), y3r = v_sub(x1r, x3r), y3i = v_sub(x1i, x3i);
        v_store(dre + i0, v_ਜੋੜੋ(y0r, y2r)); v_store(dim + i0, v_ਜੋੜੋ(y0i, y2i));
        v_store(dre + i2, v_sub(y0r, y2r)); v_store(dim + i2, v_sub(y0i, y2i));
        v_store(dre + i1, v_ਜੋੜੋ(y1r, y3i)); v_store(dim + i1, v_sub(y1i, y3r));
        v_store(dre + i3, v_sub(y1r, y3i)); v_store(dim + i3, v_ਜੋੜੋ(y1i, y3r));
    DFT_VLOOP_ਸਿਰਾ(4)
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT>
ਖਾਲੀ radix5_vec(ਅਚਲ DftStage& st, ਅਚਲ T* sre, ਅਚਲ T* sim, T* dre, T* dim)
{
    ਅਚਲ T* twr = (ਅਚਲ T*)st.tw_re; ਅਚਲ T* twi = (ਅਚਲ T*)st.tw_im;
    ਅਚਲ ਪੂਰਨ L = st.ਵਿਸਤਾਰ, ts = st.tw_stride, VL = VTraits<VT>::vlanes();
    ਅਚਲ T c5 = DftConst<T>::fft5_2, s1 = DftConst<T>::sin72, s2 = DftConst<T>::sin144;
    ਅਚਲ VT quarter = v_setall_((T)0.25);
    DFT_VLOOP_BEGIN(5)
        ਪੂਰਨ i0 = ਅਧਾਰ + j, i1 = i0 + L, i2 = i1 + L, i3 = i2 + L, i4 = i3 + L;
        VT x0r = vx_ਲੱਦੋ(sre + i0), x0i = vx_ਲੱਦੋ(sim + i0), x1r, x1i, x2r, x2i, x3r, x3i, x4r, x4i;
        DFT_VCMUL(x1r, x1i, vx_ਲੱਦੋ(sre + i1), vx_ਲੱਦੋ(sim + i1), vx_ਲੱਦੋ(twr + j), vx_ਲੱਦੋ(twi + j));
        DFT_VCMUL(x2r, x2i, vx_ਲੱਦੋ(sre + i2), vx_ਲੱਦੋ(sim + i2), vx_ਲੱਦੋ(twr + ts + j), vx_ਲੱਦੋ(twi + ts + j));
        DFT_VCMUL(x3r, x3i, vx_ਲੱਦੋ(sre + i3), vx_ਲੱਦੋ(sim + i3), vx_ਲੱਦੋ(twr + ts*2 + j), vx_ਲੱਦੋ(twi + ts*2 + j));
        DFT_VCMUL(x4r, x4i, vx_ਲੱਦੋ(sre + i4), vx_ਲੱਦੋ(sim + i4), vx_ਲੱਦੋ(twr + ts*3 + j), vx_ਲੱਦੋ(twi + ts*3 + j));
        VT a1r = v_ਜੋੜੋ(x1r, x4r), a1i = v_ਜੋੜੋ(x1i, x4i), b1r = v_sub(x1r, x4r), b1i = v_sub(x1i, x4i);
        VT a2r = v_ਜੋੜੋ(x2r, x3r), a2i = v_ਜੋੜੋ(x2i, x3i), b2r = v_sub(x2r, x3r), b2i = v_sub(x2i, x3i);
        VT sar = v_ਜੋੜੋ(a1r, a2r), sai = v_ਜੋੜੋ(a1i, a2i);
        VT tr = v_sub(x0r, v_mul(quarter, sar)), ti = v_sub(x0i, v_mul(quarter, sai));
        VT ur = v_mul(v_setall_(c5), v_sub(a1r, a2r)), ui = v_mul(v_setall_(c5), v_sub(a1i, a2i));
        VT v1r = v_fma_n(b1r, s1, v_mul(v_setall_(s2), b2r)), v1i = v_fma_n(b1i, s1, v_mul(v_setall_(s2), b2i));
        VT v2r = v_fma_n(b1r, s2, v_mul(v_setall_(-s1), b2r)), v2i = v_fma_n(b1i, s2, v_mul(v_setall_(-s1), b2i));
        VT pr = v_ਜੋੜੋ(tr, ur), pi = v_ਜੋੜੋ(ti, ui), qr = v_sub(tr, ur), qi = v_sub(ti, ui);
        v_store(dre + i0, v_ਜੋੜੋ(x0r, sar)); v_store(dim + i0, v_ਜੋੜੋ(x0i, sai));
        v_store(dre + i1, v_ਜੋੜੋ(pr, v1i)); v_store(dim + i1, v_sub(pi, v1r));
        v_store(dre + i4, v_sub(pr, v1i)); v_store(dim + i4, v_ਜੋੜੋ(pi, v1r));
        v_store(dre + i2, v_ਜੋੜੋ(qr, v2i)); v_store(dim + i2, v_sub(qi, v2r));
        v_store(dre + i3, v_sub(qr, v2i)); v_store(dim + i3, v_ਜੋੜੋ(qi, v2r));
    DFT_VLOOP_ਸਿਰਾ(5)
}


// generic odd radix, vector lanes; scratch holds a[h], b[h] as VL-wide vectors (4*h*VL elements)
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT>
ਖਾਲੀ radixOdd_vec(ਅਚਲ DftStage& st, ਅਚਲ T* sre, ਅਚਲ T* sim, T* dre, T* dim, T* scratch)
{
    ਅਚਲ T* twr = (ਅਚਲ T*)st.tw_re; ਅਚਲ T* twi = (ਅਚਲ T*)st.tw_im;
    ਅਚਲ T* cs = (ਅਚਲ T*)st.cs; ਅਚਲ T* sn = (ਅਚਲ T*)st.sn;
    ਅਚਲ ਪੂਰਨ L = st.ਵਿਸਤਾਰ, ts = st.tw_stride, VL = VTraits<VT>::vlanes(), r = st.radix, h = (r - 1)/2;
    T* ar = scratch; T* ai = ar + h*VL; T* br = ai + h*VL; T* bi = br + h*VL;
    DFT_VLOOP_BEGIN(r)
        ਪੂਰਨ i0 = ਅਧਾਰ + j;
        VT x0r = vx_ਲੱਦੋ(sre + i0), x0i = vx_ਲੱਦੋ(sim + i0);
        VT X0r = x0r, X0i = x0i;
        ਲਈ (ਪੂਰਨ p = 1; p <= h; p++)
        {
            ਪੂਰਨ ip = i0 + p*L, iq = i0 + (r-p)*L;
            VT xpr, xpi, xqr, xqi;
            DFT_VCMUL(xpr, xpi, vx_ਲੱਦੋ(sre + ip), vx_ਲੱਦੋ(sim + ip),
                      vx_ਲੱਦੋ(twr + (p-1)*ts + j), vx_ਲੱਦੋ(twi + (p-1)*ts + j));
            DFT_VCMUL(xqr, xqi, vx_ਲੱਦੋ(sre + iq), vx_ਲੱਦੋ(sim + iq),
                      vx_ਲੱਦੋ(twr + (r-p-1)*ts + j), vx_ਲੱਦੋ(twi + (r-p-1)*ts + j));
            VT apr = v_ਜੋੜੋ(xpr, xqr), api = v_ਜੋੜੋ(xpi, xqi);
            v_store(ar + (p-1)*VL, apr); v_store(ai + (p-1)*VL, api);
            v_store(br + (p-1)*VL, v_sub(xpr, xqr)); v_store(bi + (p-1)*VL, v_sub(xpi, xqi));
            X0r = v_ਜੋੜੋ(X0r, apr); X0i = v_ਜੋੜੋ(X0i, api);
        }
        v_store(dre + i0, X0r); v_store(dim + i0, X0i);
        ਲਈ (ਪੂਰਨ k = 1; k <= h; k++)
        {
            VT sr = x0r, si = x0i, tr = v_setall_((T)0), ti = tr;
            ਲਈ (ਪੂਰਨ p = 1; p <= h; p++)
            {
                T c = cs[(p-1)*h + (k-1)], s = sn[(p-1)*h + (k-1)];
                sr = v_fma_n(vx_ਲੱਦੋ(ar + (p-1)*VL), c, sr); si = v_fma_n(vx_ਲੱਦੋ(ai + (p-1)*VL), c, si);
                tr = v_fma_n(vx_ਲੱਦੋ(br + (p-1)*VL), s, tr); ti = v_fma_n(vx_ਲੱਦੋ(bi + (p-1)*VL), s, ti);
            }
            v_store(dre + i0 + k*L, v_ਜੋੜੋ(sr, ti)); v_store(dim + i0 + k*L, v_sub(si, tr));
            v_store(dre + i0 + (r-k)*L, v_sub(sr, ti)); v_store(dim + i0 + (r-k)*L, v_ਜੋੜੋ(si, tr));
        }
    DFT_VLOOP_ਸਿਰਾ(r)
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DFT_VLOOP_BEGIN
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DFT_VLOOP_ਸਿਰਾ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DFT_VCMUL

#ਜੇ_ਅੰਤ // CV_SIMD || CV_SIMD_SCALABLE

// entry points: the plan's stage mode selects the body; VT = T (no SIMD for this type) means scalar only
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ radix3_(ਅਚਲ DftStage& st, ਅਚਲ ਖਾਲੀ* sre, ਅਚਲ ਖਾਲੀ* sim, ਖਾਲੀ* dre, ਖਾਲੀ* dim, ਖਾਲੀ*)
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
    {
        ਜੇ (st.ਬਹੁਲਕ == DFT_STAGE_ਭਰਿਆ)
        {
            radix3_vec<T, VT>(st, (ਅਚਲ T*)sre, (ਅਚਲ T*)sim, (T*)dre, (T*)dim);
            ਵਾਪਸ;
        }
    }
#ਜੇ_ਅੰਤ
    radix3_scalar<T>(st, (ਅਚਲ T*)sre, (ਅਚਲ T*)sim, (T*)dre, (T*)dim);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ radix4_(ਅਚਲ DftStage& st, ਅਚਲ ਖਾਲੀ* sre, ਅਚਲ ਖਾਲੀ* sim, ਖਾਲੀ* dre, ਖਾਲੀ* dim, ਖਾਲੀ*)
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
    {
        ਜੇ (st.ਬਹੁਲਕ == DFT_STAGE_ਭਰਿਆ)
        {
            radix4_vec<T, VT>(st, (ਅਚਲ T*)sre, (ਅਚਲ T*)sim, (T*)dre, (T*)dim);
            ਵਾਪਸ;
        }
    }
#ਜੇ_ਅੰਤ
    radix4_scalar<T>(st, (ਅਚਲ T*)sre, (ਅਚਲ T*)sim, (T*)dre, (T*)dim);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ radix5_(ਅਚਲ DftStage& st, ਅਚਲ ਖਾਲੀ* sre, ਅਚਲ ਖਾਲੀ* sim, ਖਾਲੀ* dre, ਖਾਲੀ* dim, ਖਾਲੀ*)
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
    {
        ਜੇ (st.ਬਹੁਲਕ == DFT_STAGE_ਭਰਿਆ)
        {
            radix5_vec<T, VT>(st, (ਅਚਲ T*)sre, (ਅਚਲ T*)sim, (T*)dre, (T*)dim);
            ਵਾਪਸ;
        }
    }
#ਜੇ_ਅੰਤ
    radix5_scalar<T>(st, (ਅਚਲ T*)sre, (ਅਚਲ T*)sim, (T*)dre, (T*)dim);
}


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ radixOdd_(ਅਚਲ DftStage& st, ਅਚਲ ਖਾਲੀ* sre, ਅਚਲ ਖਾਲੀ* sim, ਖਾਲੀ* dre, ਖਾਲੀ* dim, ਖਾਲੀ* scratch)
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
    {
        ਜੇ (st.ਬਹੁਲਕ == DFT_STAGE_ਭਰਿਆ)
        {
            radixOdd_vec<T, VT>(st, (ਅਚਲ T*)sre, (ਅਚਲ T*)sim, (T*)dre, (T*)dim, (T*)scratch);
            ਵਾਪਸ;
        }
    }
#ਜੇ_ਅੰਤ
    radixOdd_scalar<T>(st, (ਅਚਲ T*)sre, (ਅਚਲ T*)sim, (T*)dre, (T*)dim, (T*)scratch);
}


//////////////////////////////// group C: postprocessing ////////////////////////////////
//
// Pure map passes over the split arrays (plus the C2R / IDCT input untangles, which are the
// mirror images and live here too). Each is written once: a vector body over whole vectors and a
// scalar tail. The "pair" passes (RFFT untangle, DCT) process k ascending together with m = nc-k
// descending; the descending side is loaded/stored through v_reverse.

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ postprocDFT_(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* _re, ਅਚਲ ਖਾਲੀ* _im, ਖਾਲੀ* _dst,
                  ਦੁੱਗਣਾ re_scale, ਦੁੱਗਣਾ im_scale)
{
    ਅਚਲ T* re = (ਅਚਲ T*)_re; ਅਚਲ T* im = (ਅਚਲ T*)_im;
    T* dst = (T*)_dst;
    ਪੂਰਨ nc = plan.nc, i = 0;
    T rs = (T)re_scale, is = (T)im_scale;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
    {
        ਅਚਲ ਪੂਰਨ VL = VTraits<VT>::vlanes();
        VT vrs = v_setall_(rs), vis = v_setall_(is);
        ਲਈ (; i + VL <= nc; i += VL)
            v_store_interleave(dst + i*2, v_mul(vx_ਲੱਦੋ(re + i), vrs), v_mul(vx_ਲੱਦੋ(im + i), vis));
    }
#ਜੇ_ਅੰਤ
    ਲਈ (; i < nc; i++)
    {
        dst[i*2] = re[i]*rs;
        dst[i*2+1] = im[i]*is;
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ postprocReal_(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* _re, ਖਾਲੀ* _dst, ਦੁੱਗਣਾ scale)
{
    ਅਚਲ T* re = (ਅਚਲ T*)_re;
    T* dst = (T*)_dst;
    ਪੂਰਨ nc = plan.nc, i = 0;
    T s = (T)scale;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
    {
        ਅਚਲ ਪੂਰਨ VL = VTraits<VT>::vlanes();
        VT vs = v_setall_(s);
        ਲਈ (; i + VL <= nc; i += VL)
            v_store(dst + i, v_mul(vx_ਲੱਦੋ(re + i), vs));
    }
#ਜੇ_ਅੰਤ
    ਲਈ (; i < nc; i++)
        dst[i] = re[i]*s;
}

// Forward untangle of the nc-point spectrum Z of z[j] = x[2j] + i*x[2j+1] into the spectrum X of x:
//   E = (Z[k] + conj Z[nc-k])/2, O = (Z[k] - conj Z[nc-k])/(2i), X[k] = E + W_n^k*O,
//   X[nc-k] = conj(E - W_n^k*O), X[0] = Z0.re + Z0.im, X[nc] = Z0.re - Z0.im, X[n/4] = conj Z[nc/2].
// X[k], k in [1, nc-1] is written to (out[k*2-1], out[k*2]), i.e. CCS order; the caller handles
// X[0] and X[nc].
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DFT_UNTANGLE_FWD_SCALAR(k, m, Xkr, Xki, Xmr, Xmi) \
    T Er = (re[k] + re[m])*(T)0.5, Ei = (im[k] - im[m])*(T)0.5; \
    T Or = (im[k] + im[m])*(T)0.5, Oi = (re[m] - re[k])*(T)0.5; \
    T Pr = wr[k]*Or - wi[k]*Oi, Pi = wr[k]*Oi + wi[k]*Or; \
    T Xkr = Er + Pr, Xki = Ei + Pi, Xmr = Er - Pr, Xmi = Pi - Ei;

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
// vector version: lanes l = 0..VL-1 hold k+l (ascending) and m-l (descending; loaded reversed)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DFT_UNTANGLE_FWD_VEC(k, m, Xkr, Xki, Xmr, Xmi) \
    VT rk = vx_ਲੱਦੋ(re + (k)), ik = vx_ਲੱਦੋ(im + (k)); \
    VT rm = v_ਉਲਟ(vx_ਲੱਦੋ(re + (m) - VL + 1)), imm = v_ਉਲਟ(vx_ਲੱਦੋ(im + (m) - VL + 1)); \
    VT Er = v_mul(v_ਜੋੜੋ(rk, rm), half), Ei = v_mul(v_sub(ik, imm), half); \
    VT Or = v_mul(v_ਜੋੜੋ(ik, imm), half), Oi = v_mul(v_sub(rm, rk), half); \
    VT wkr = vx_ਲੱਦੋ(wr + (k)), wki = vx_ਲੱਦੋ(wi + (k)); \
    VT Pr = v_sub(v_mul(wkr, Or), v_mul(wki, Oi)), Pi = v_fma(wkr, Oi, v_mul(wki, Or)); \
    VT Xkr = v_ਜੋੜੋ(Er, Pr), Xki = v_ਜੋੜੋ(Ei, Pi), Xmr = v_sub(Er, Pr), Xmi = v_sub(Pi, Ei);
#ਜੇ_ਅੰਤ

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ccsUntangleFwd(ਅਚਲ DftPlan& plan, ਅਚਲ T* re, ਅਚਲ T* im, T s, T* out)
{
    ਪੂਰਨ nc = plan.nc;
    ਅਚਲ T* wr = (ਅਚਲ T*)plan.rtw_re;
    ਅਚਲ T* wi = (ਅਚਲ T*)plan.rtw_im;
    ਪੂਰਨ k = 1, m = nc - 1;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
    {
        ਅਚਲ ਪੂਰਨ VL = VTraits<VT>::vlanes();
        ਅਚਲ VT half = v_setall_((T)0.5), vs = v_setall_(s);
        ਲਈ (; m - k + 1 >= 2*VL; k += VL, m -= VL)
        {
            DFT_UNTANGLE_FWD_VEC(k, m, Xkr, Xki, Xmr, Xmi)
            v_store_interleave(out + k*2 - 1, v_mul(Xkr, vs), v_mul(Xki, vs));
            v_store_interleave(out + (m - VL + 1)*2 - 1, v_ਉਲਟ(v_mul(Xmr, vs)), v_ਉਲਟ(v_mul(Xmi, vs)));
        }
    }
#ਜੇ_ਅੰਤ
    ਲਈ (; k < m; k++, m--)
    {
        DFT_UNTANGLE_FWD_SCALAR(k, m, Xkr, Xki, Xmr, Xmi)
        out[k*2-1] = Xkr*s; out[k*2] = Xki*s;
        out[m*2-1] = Xmr*s; out[m*2] = Xmi*s;
    }
    ਜੇ ((nc & 1) == 0 && nc >= 2)
    {
        k = nc/2;
        out[k*2-1] = re[k]*s; out[k*2] = -im[k]*s;
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ postprocRealDFT_(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* _re, ਅਚਲ ਖਾਲੀ* _im, ਖਾਲੀ* _dst,
                      ਦੁੱਗਣਾ scale, ਤਾਰਕਿਕ complex_output)
{
    ਅਚਲ T* re = (ਅਚਲ T*)_re; ਅਚਲ T* im = (ਅਚਲ T*)_im;
    T* dst = (T*)_dst;
    ਪੂਰਨ n = plan.n, nc = plan.nc;
    T s = (T)scale;
    ਜੇ (nc == n)
    {
        // odd n: the full complex spectrum was computed
        ਜੇ (complex_output)
            postprocDFT_<T, VT, haveSIMD>(plan, _re, _im, _dst, scale, scale);
        ਨਹੀਂਤਾਂ
        {
            dst[0] = re[0]*s;
            ਲਈ (ਪੂਰਨ k = 1; k <= (n-1)/2; k++)
            {
                dst[k*2-1] = re[k]*s;
                dst[k*2] = im[k]*s;
            }
        }
        ਵਾਪਸ;
    }
    T X0 = (re[0] + im[0])*s, Xnc = (re[0] - im[0])*s;
    ccsUntangleFwd<T, VT, haveSIMD>(plan, re, im, s, complex_output ? dst + 1 : dst);
    ਜੇ (complex_output)
    {
        dst[0] = X0; dst[1] = 0;
        dst[n] = Xnc; dst[n+1] = 0;
    }
    ਨਹੀਂਤਾਂ
    {
        dst[0] = X0;
        dst[n-1] = Xnc;
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ postprocDCT_(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* _re, ਅਚਲ ਖਾਲੀ* _im, ਖਾਲੀ* _dst, size_t dstep)
{
    ਅਚਲ T* re = (ਅਚਲ T*)_re; ਅਚਲ T* im = (ਅਚਲ T*)_im;
    T* dst = (T*)_dst;
    ਪੂਰਨ n = plan.n, nc = plan.nc;
    ਅਚਲ T* wr = (ਅਚਲ T*)plan.rtw_re;
    ਅਚਲ T* wi = (ਅਚਲ T*)plan.rtw_im;
    ਅਚਲ T* dr = (ਅਚਲ T*)plan.dct_re;
    ਅਚਲ T* di = (ਅਚਲ T*)plan.dct_im;
    // dst[k] = Re(X_k*w_k), dst[n-k] = -Im(X_k*w_k); w_k = s*W_4n^k (s = DCT normalization);
    // the k = 0 and k = nc terms get the extra sin45 (= 1/sqrt(2)) of DCT-II.
    T X0 = re[0] + im[0], Xnc = re[0] - im[0];
    dst[0] = X0*dr[0]*DftConst<T>::sin45;
    dst[nc*dstep] = Xnc*dr[nc];
    ਪੂਰਨ k = 1, m = nc - 1;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
    {
        ਜੇ (dstep == 1)
        {
            ਅਚਲ ਪੂਰਨ VL = VTraits<VT>::vlanes();
            ਅਚਲ VT half = v_setall_((T)0.5);
            ਲਈ (; m - k + 1 >= 2*VL; k += VL, m -= VL)
            {
                DFT_UNTANGLE_FWD_VEC(k, m, Xkr, Xki, Xmr, Xmi)
                VT dkr = vx_ਲੱਦੋ(dr + k), dki = vx_ਲੱਦੋ(di + k);
                VT dmr = v_ਉਲਟ(vx_ਲੱਦੋ(dr + m - VL + 1)), dmi = v_ਉਲਟ(vx_ਲੱਦੋ(di + m - VL + 1));
                v_store(dst + k, v_sub(v_mul(Xkr, dkr), v_mul(Xki, dki)));                          // dst[k+l]
                v_store(dst + n - k - VL + 1, v_ਉਲਟ(v_sub(v_mul(v_sub(v_setall_((T)0), Xkr), dki), v_mul(Xki, dkr)))); // dst[n-k-l]
                v_store(dst + m - VL + 1, v_ਉਲਟ(v_sub(v_mul(Xmr, dmr), v_mul(Xmi, dmi))));      // dst[m-l]
                v_store(dst + n - m, v_sub(v_mul(v_sub(v_setall_((T)0), Xmr), dmi), v_mul(Xmi, dmr)));   // dst[n-m+l]
            }
        }
    }
#ਜੇ_ਅੰਤ
    ਲਈ (; k < m; k++, m--)
    {
        DFT_UNTANGLE_FWD_SCALAR(k, m, Xkr, Xki, Xmr, Xmi)
        dst[k*dstep] = Xkr*dr[k] - Xki*di[k];
        dst[(n-k)*dstep] = -(Xkr*di[k] + Xki*dr[k]);
        dst[m*dstep] = Xmr*dr[m] - Xmi*di[m];
        dst[(n-m)*dstep] = -(Xmr*di[m] + Xmi*dr[m]);
    }
    ਜੇ ((nc & 1) == 0 && nc >= 2)
    {
        k = nc/2;
        T Xkr = re[k], Xki = -im[k];
        dst[k*dstep] = Xkr*dr[k] - Xki*di[k];
        dst[(n-k)*dstep] = -(Xkr*di[k] + Xki*dr[k]);
    }
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DFT_UNTANGLE_FWD_SCALAR
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DFT_UNTANGLE_FWD_VEC

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
ਖਾਲੀ postprocIDCT_(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* _re, ਅਚਲ ਖਾਲੀ* _im, ਖਾਲੀ* _dst, size_t dstep)
{
    ਅਚਲ T* re = (ਅਚਲ T*)_re; ਅਚਲ T* im = (ਅਚਲ T*)_im;
    T* dst = (T*)_dst;
    ਪੂਰਨ n = plan.n, nc = plan.nc;
    // out[2m] = re[m], out[2m+1] = -im[m] (conjugation trick) is the CCS inverse result;
    // Makhoul reordering: dst[2j] = out[j], dst[2j+1] = out[n-1-j], i.e. for j = 2m, 2m+1:
    // dst[4m..4m+3] = (re[m], -im[nc-1-m], -im[m], re[nc-1-m]).
    ਪੂਰਨ j = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (haveSIMD)
    {
        ਜੇ (dstep == 1)
        {
            ਅਚਲ ਪੂਰਨ VL = VTraits<VT>::vlanes();
            ਅਚਲ VT zero = v_setall_((T)0);
            ਪੂਰਨ m = 0;
            ਲਈ (; m + VL <= nc/2; m += VL)
            {
                VT a = vx_ਲੱਦੋ(re + m);
                VT b = v_sub(zero, v_ਉਲਟ(vx_ਲੱਦੋ(im + nc - m - VL)));
                VT c = v_sub(zero, vx_ਲੱਦੋ(im + m));
                VT d = v_ਉਲਟ(vx_ਲੱਦੋ(re + nc - m - VL));
                v_store_interleave(dst + m*4, a, b, c, d);
            }
            j = m*2;
        }
    }
#ਜੇ_ਅੰਤ
    // j even: out[j] = re[j/2], out[n-1-j] = -im[(n-1-j)/2]; j odd: out[j] = -im[j/2], out[n-1-j] = re[(n-1-j)/2]
    ਲਈ (; j + 1 < nc; j += 2)
    {
        ਪੂਰਨ m = j >> 1, q = (n - 1 - j) >> 1;
        dst[(2*j)*dstep] = re[m];
        dst[(2*j+1)*dstep] = -im[q];
        dst[(2*j+2)*dstep] = -im[m];
        dst[(2*j+3)*dstep] = re[q];
    }
    ਜੇ (j < nc)     // nc odd: the last j is even
    {
        ਪੂਰਨ m = j >> 1, q = (n - 1 - j) >> 1;
        dst[(2*j)*dstep] = re[m];
        dst[(2*j+1)*dstep] = -im[q];
    }
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DFT4_SCALAR

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VT, ਤਾਰਕਿਕ haveSIMD>
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
    ਜੇ constexpr (haveSIMD)
        k.vlanes = VTraits<VT>::vlanes();
    ਨਹੀਂਤਾਂ
        k.vlanes = 1;
    ਵਾਪਸ k;
}

} // namespace

DFTKernels getDFTKernels_(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਜੇ (ਡੂੰਘਾਈ == CV_32F)
    {
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        ਵਾਪਸ makeKernels<ਦਸ਼ਮਲਵ, v_float32, ਸੱਚ>();
#ਨਹੀਂਤਾਂ
        ਵਾਪਸ makeKernels<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਝੂਠ>();
#ਜੇ_ਅੰਤ
    }
    CV_Assert(ਡੂੰਘਾਈ == CV_64F);
#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ਵਾਪਸ makeKernels<ਦੁੱਗਣਾ, v_float64, ਸੱਚ>();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ makeKernels<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, ਝੂਠ>();
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਅੰਤ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
}
