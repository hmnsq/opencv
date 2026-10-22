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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਨਾਮਾਕਾਸ਼ cv
{

///////////////////////////// Functions Declaration //////////////////////////////////////

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_RNG_COEFF 4164903690U

/*
   Multiply-with-carry generator is used here:
   temp = ( A*X(n) + carry )
   X(n+1) = temp mod (2^32)
   carry = floor (temp / (2^32))
*/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ  RNG_ਅਗਲਾ(x)    ((uint64)(ਬਿਨਾਨਿਸ਼ਾਨੀ)(x)*CV_RNG_COEFF + ((x) >> 32))
// make it jump-less
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ  CN_ਅਗਲਾ(k)     (((k) + 1) & (((k) >= cn) - 1))

ਗਿਣਤੀ
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
rand_ਬਿੱਟ( T* arr, ਪੂਰਨ len, ਪੂਰਨ cn, uint64* state, ਅਚਲ Vec2l* p, ਪੂਰਨ flags )
{
    ਤਾਰਕਿਕ small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 ਆਰਜ਼ੀ_2 = *state;
    ਪੂਰਨ i, k = 0;
    len *= cn;
    --cn;

    ਜੇ( !small_flag )
    {
        ਲਈ( i = 0; i <= len - 4; i += 4 )
        {
            int64_t t0, t1;

            ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
            t0 = ((int64_t)ਆਰਜ਼ੀ_2 & p[k][0]) + p[k][1];
            k = CN_ਅਗਲਾ(k);
            ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
            t1 = ((int64_t)ਆਰਜ਼ੀ_2 & p[k][0]) + p[k][1];
            k = CN_ਅਗਲਾ(k);
            arr[i] = saturate_cast<T>(t0);
            arr[i+1] = saturate_cast<T>(t1);

            ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
            t0 = ((int64_t)ਆਰਜ਼ੀ_2 & p[k][0]) + p[k][1];
            k = CN_ਅਗਲਾ(k);
            ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
            t1 = ((int64_t)ਆਰਜ਼ੀ_2 & p[k][0]) + p[k][1];
            k = CN_ਅਗਲਾ(k);
            arr[i+2] = saturate_cast<T>(t0);
            arr[i+3] = saturate_cast<T>(t1);
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( i = 0; i <= len - 4; i += 4 )
        {
            int64_t t0, t1, t;
            ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
            t = ਆਰਜ਼ੀ_2;
            // p[i+...][0] is within 0..255 in this branch (small_flag==true),
            // so we don't need to do (t>>...)&255,
            // the upper bits will be cleaned with ... & p[i+...][0].
            t0 = (t & p[k][0]) + p[k][1];
            k = CN_ਅਗਲਾ(k);
            t1 = ((t >> 8) & p[k][0]) + p[k][1];
            k = CN_ਅਗਲਾ(k);
            arr[i] = saturate_cast<T>(t0);
            arr[i+1] = saturate_cast<T>(t1);

            t0 = ((t >> 16) & p[k][0]) + p[k][1];
            k = CN_ਅਗਲਾ(k);
            t1 = ((t >> 24) & p[k][0]) + p[k][1];
            k = CN_ਅਗਲਾ(k);
            arr[i+2] = saturate_cast<T>(t0);
            arr[i+3] = saturate_cast<T>(t1);
        }
    }

    ਲਈ( ; i < len; i++ )
    {
        int64_t t0;
        ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);

        t0 = ((int64_t)ਆਰਜ਼ੀ_2 & p[k][0]) + p[k][1];
        k = CN_ਅਗਲਾ(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = ਆਰਜ਼ੀ_2;
}

ਬਣਤਰ DivStruct
{
    ਬਿਨਾਨਿਸ਼ਾਨੀ d;
    ਬਿਨਾਨਿਸ਼ਾਨੀ M;
    ਪੂਰਨ sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
randi_( T* arr, ਪੂਰਨ len, ਪੂਰਨ cn, uint64* state, ਅਚਲ DivStruct* p )
{
    uint64 ਆਰਜ਼ੀ_2 = *state;
    ਪੂਰਨ k = 0;
    len *= cn;
    cn--;
    ਲਈ( ਪੂਰਨ i = 0; i < len; i++ )
    {
        ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
        ਬਿਨਾਨਿਸ਼ਾਨੀ t = (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਆਰਜ਼ੀ_2;
        ਬਿਨਾਨਿਸ਼ਾਨੀ v = (ਬਿਨਾਨਿਸ਼ਾਨੀ)(((uint64)t * p[k].M) >> 32);
        v = (v + ((t - v) >> p[k].sh1)) >> p[k].sh2;
        int64_t res = (int64_t)(t - v*p[k].d) + p[k].delta;
        k = CN_ਅਗਲਾ(k);
        arr[i] = saturate_cast<T>(res);
    }
    *state = ਆਰਜ਼ੀ_2;
}

ਸਥਿਰ ਖਾਲੀ
randi_( int64_t* arr, ਪੂਰਨ len, ਪੂਰਨ cn, uint64* state, ਅਚਲ DivStruct* p )
{
    uint64 ਆਰਜ਼ੀ_2 = *state;
    ਪੂਰਨ k = 0;
    len *= cn;
    cn--;
    ਲਈ( ਪੂਰਨ i = 0; i < len; i++ )
    {
        ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
        ਬਿਨਾਨਿਸ਼ਾਨੀ t0 = (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਆਰਜ਼ੀ_2;
        ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
        ਬਿਨਾਨਿਸ਼ਾਨੀ t1 = (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਆਰਜ਼ੀ_2;
        int64_t t = (int64_t)((((uint64_t)t0 << 32) | t1) % p[k].diff) + p[k].delta;
        k = CN_ਅਗਲਾ(k);
        arr[i] = t;
    }
    *state = ਆਰਜ਼ੀ_2;
}

ਸਥਿਰ ਖਾਲੀ
randi_( uint64_t* arr, ਪੂਰਨ len, ਪੂਰਨ cn, uint64* state, ਅਚਲ DivStruct* p )
{
    uint64 ਆਰਜ਼ੀ_2 = *state;
    ਪੂਰਨ k = 0;
    len *= cn;
    cn--;
    ਲਈ( ਪੂਰਨ i = 0; i < len; i++ )
    {
        ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
        ਬਿਨਾਨਿਸ਼ਾਨੀ t0 = (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਆਰਜ਼ੀ_2;
        ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
        ਬਿਨਾਨਿਸ਼ਾਨੀ t1 = (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਆਰਜ਼ੀ_2;
        uint64_t t = (((uint64_t)t0 << 32) | t1) % p[k].diff;
        int64_t delta = p[k].delta;
        k = CN_ਅਗਲਾ(k);
        arr[i] = delta >= 0 || t >= (uint64_t)-delta ? t + (uint64_t)delta : 0;
    }
    *state = ਆਰਜ਼ੀ_2;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਖਾਲੀ rand_ਬਿੱਟ_Wrap(ਖਾਲੀ* arr, ਪੂਰਨ len, ਪੂਰਨ cn, uint64* state, ਅਚਲ ਖਾਲੀ* p, ਖਾਲੀ*, ਪੂਰਨ flags)
{
    rand_ਬਿੱਟ((T*)arr, len, cn, state, (ਅਚਲ Vec2l*)p, flags);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਖਾਲੀ randiWrap(ਖਾਲੀ* arr, ਪੂਰਨ len, ਪੂਰਨ cn, uint64* state, ਅਚਲ ਖਾਲੀ* p, ਖਾਲੀ*, ਪੂਰਨ)
{
    randi_((T*)arr, len, cn, state, (ਅਚਲ DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ cvt32f_ਮੰਜ਼ਿਲ_FP8(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਖਾਲੀ* dst, ਪੂਰਨ len, ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਜੇ (ਡੂੰਘਾਈ == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   ਲਈ (ਪੂਰਨ i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    ਨਹੀਂਤਾਂ                              { fp8a_t* d = (fp8a_t*)dst; ਲਈ (ਪੂਰਨ i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ is_FP8_ਡੂੰਘਾਈ(ਪੂਰਨ d) { ਵਾਪਸ d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

ਸਥਿਰ ਖਾਲੀ randf_16_or_32f( ਖਾਲੀ* dst, ਪੂਰਨ len_, ਪੂਰਨ cn, uint64* state, ਅਚਲ ਖਾਲੀ* _p, ਖਾਲੀ* _fbuf, ਪੂਰਨ flags )
{
    ਅਚਲ Vec2f* p = (ਅਚਲ Vec2f*)_p;
    ਦਸ਼ਮਲਵ* fbuf = (ਦਸ਼ਮਲਵ*)_fbuf;
    ਪੂਰਨ ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(flags);
    uint64 ਆਰਜ਼ੀ_2 = *state;
    ਪੂਰਨ k = 0, len = len_*cn;
    ਦਸ਼ਮਲਵ* arr = ਡੂੰਘਾਈ == CV_16F || ਡੂੰਘਾਈ == CV_16BF || is_FP8_ਡੂੰਘਾਈ(ਡੂੰਘਾਈ) ? fbuf : (ਦਸ਼ਮਲਵ*)dst;
    cn--;
    ਲਈ( ਪੂਰਨ i = 0; i < len; i++ )
    {
        ਪੂਰਨ t = (ਪੂਰਨ)(ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2));
        arr[i] = (ਦਸ਼ਮਲਵ)(t*p[k][0]);
        k = CN_ਅਗਲਾ(k);
    }
    *state = ਆਰਜ਼ੀ_2;
    hal::ਜੋੜੋ_RNGBias32f(arr, &p[0][0], len_, cn+1);
    ਜੇ (ਡੂੰਘਾਈ == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    ਨਹੀਂਤਾਂ ਜੇ (ਡੂੰਘਾਈ == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    ਨਹੀਂਤਾਂ ਜੇ (is_FP8_ਡੂੰਘਾਈ(ਡੂੰਘਾਈ))
        cvt32f_ਮੰਜ਼ਿਲ_FP8(fbuf, dst, len, ਡੂੰਘਾਈ);
}

ਸਥਿਰ ਖਾਲੀ
randf_64f( ਖਾਲੀ* _arr, ਪੂਰਨ len_, ਪੂਰਨ cn, uint64* state, ਅਚਲ ਖਾਲੀ* _p, ਖਾਲੀ*, ਪੂਰਨ )
{
    ਦੁੱਗਣਾ* arr = (ਦੁੱਗਣਾ*)_arr;
    ਅਚਲ Vec2d* p = (ਅਚਲ Vec2d*)_p;
    uint64 ਆਰਜ਼ੀ_2 = *state;
    ਪੂਰਨ k = 0, len = len_*cn;
    cn--;
    ਲਈ( ਪੂਰਨ i = 0; i < len; i++ )
    {
        ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
        int64_t v = (int64_t)((ਆਰਜ਼ੀ_2 >> 32) | (ਆਰਜ਼ੀ_2 << 32));
        arr[i] = v*p[k][0];
        k = CN_ਅਗਲਾ(k);
    }
    *state = ਆਰਜ਼ੀ_2;
    hal::ਜੋੜੋ_RNGBias64f(arr, &p[0][0], len_, cn+1);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*RandFunc)(ਖਾਲੀ* arr, ਪੂਰਨ len, ਪੂਰਨ cn, uint64* state,
                         ਅਚਲ ਖਾਲੀ* p, ਖਾਲੀ* tempbuf, ਪੂਰਨ flags);

ਸਥਿਰ RandFunc randTab[CV_ਡੂੰਘਾਈ_MAX][CV_ਡੂੰਘਾਈ_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<ਛੋਟਾ>, randiWrap<ਪੂਰਨ>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<ਤਾਰਕਿਕ>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<ਬਿਨਾਨਿਸ਼ਾਨੀ>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        rand_ਬਿੱਟ_Wrap<uchar>, rand_ਬਿੱਟ_Wrap<schar>, rand_ਬਿੱਟ_Wrap<ushort>,
        rand_ਬਿੱਟ_Wrap<ਛੋਟਾ>, rand_ਬਿੱਟ_Wrap<ਪੂਰਨ>, 0, 0, 0, 0,
        rand_ਬਿੱਟ_Wrap<ਤਾਰਕਿਕ>, rand_ਬਿੱਟ_Wrap<uint64_t>, rand_ਬਿੱਟ_Wrap<int64_t>,
        rand_ਬਿੱਟ_Wrap<ਬਿਨਾਨਿਸ਼ਾਨੀ>, 0, 0, 0
    }
};

/*
   The code below implements the algorithm described in
   "The Ziggurat Method for Generating Random Variables"
   by George Marsaglia and Wai Wan Tsang, Journal of Statistical Software, 2007.
*/
ਸਥਿਰ ਖਾਲੀ
randn_0_1_32f( ਦਸ਼ਮਲਵ* arr, ਪੂਰਨ len, uint64* state )
{
    ਅਚਲ ਦਸ਼ਮਲਵ r = 3.442620f; // The start of the right tail
    ਅਚਲ ਦਸ਼ਮਲਵ rng_flt = 2.3283064365386962890625e-10f; // 2^-32
    ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ kn[128];
    ਸਥਿਰ ਦਸ਼ਮਲਵ wn[128], fn[128];
    uint64 ਆਰਜ਼ੀ_2 = *state;
    ਸਥਿਰ ਤਾਰਕਿਕ initialized=ਝੂਠ;
    ਪੂਰਨ i;

    ਜੇ( !initialized )
    {
        ਅਚਲ ਦੁੱਗਣਾ m1 = 2147483648.0;
        ਦੁੱਗਣਾ dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        ਦੁੱਗਣਾ q = vn/std::ਘਾਤਾਂਕ(-.5*dn*dn);
        kn[0] = (ਬਿਨਾਨਿਸ਼ਾਨੀ)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (ਦਸ਼ਮਲਵ)(q/m1);
        wn[127] = (ਦਸ਼ਮਲਵ)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (ਦਸ਼ਮਲਵ)std::ਘਾਤਾਂਕ(-.5*dn*dn);

        ਲਈ(i=126;i>=1;i--)
        {
            dn = std::ਵਰਗਮੂਲ(-2.*std::ਲਘੂਗਣਕ(vn/dn+std::ਘਾਤਾਂਕ(-.5*dn*dn)));
            kn[i+1] = (ਬਿਨਾਨਿਸ਼ਾਨੀ)((dn/tn)*m1);
            tn = dn;
            fn[i] = (ਦਸ਼ਮਲਵ)std::ਘਾਤਾਂਕ(-.5*dn*dn);
            wn[i] = (ਦਸ਼ਮਲਵ)(dn/m1);
        }
        initialized = ਸੱਚ;
    }

    ਲਈ( i = 0; i < len; i++ )
    {
        ਦਸ਼ਮਲਵ x, y;
        ਲਈ(;;)
        {
            ਪੂਰਨ hz = (ਪੂਰਨ)ਆਰਜ਼ੀ_2;
            ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
            ਪੂਰਨ iz = hz & 127;
            x = hz*wn[iz];
            ਜੇ( (ਬਿਨਾਨਿਸ਼ਾਨੀ)std::ਨਿਰਪੇਖ(hz) < kn[iz] )
                ਰੋਕੋ;
            ਜੇ( iz == 0) // iz==0, handles the base strip
            {
                ਕਰੋ
                {
                    x = (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਆਰਜ਼ੀ_2*rng_flt;
                    ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
                    y = (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਆਰਜ਼ੀ_2*rng_flt;
                    ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
                    x = (ਦਸ਼ਮਲਵ)(-std::ਲਘੂਗਣਕ(x+FLT_MIN)*0.2904764);
                    y = (ਦਸ਼ਮਲਵ)-std::ਲਘੂਗਣਕ(y+FLT_MIN);
                }	// .2904764 is 1/r
                ਜਦੋਂ( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                ਰੋਕੋ;
            }
            // iz > 0, handle the wedges of other strips
            y = (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਆਰਜ਼ੀ_2*rng_flt;
            ਆਰਜ਼ੀ_2 = RNG_ਅਗਲਾ(ਆਰਜ਼ੀ_2);
            ਜੇ( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::ਘਾਤਾਂਕ(-.5*x*x) )
                ਰੋਕੋ;
        }
        arr[i] = x;
    }
    *state = ਆਰਜ਼ੀ_2;
}


ਦੁੱਗਣਾ RNG::gaussian(ਦੁੱਗਣਾ sigma)
{
    ਦਸ਼ਮਲਵ ਆਰਜ਼ੀ_2;
    randn_0_1_32f( &ਆਰਜ਼ੀ_2, 1, &state );
    ਵਾਪਸ ਆਰਜ਼ੀ_2*sigma;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ PT> ਸਥਿਰ ਖਾਲੀ
randnScale_(ਦਸ਼ਮਲਵ* src, ਖਾਲੀ* _dst, ਪੂਰਨ len, ਪੂਰਨ cn,
            ਅਚਲ ਖਾਲੀ* ਔਸਤ, ਅਚਲ ਖਾਲੀ* _stddev, ਪੂਰਨ flags )
{
    T* dst = (T*)_dst;
    ਅਚਲ PT* ਔਸਤ_2 = (ਅਚਲ PT*)ਔਸਤ;
    ਅਚਲ PT* stddev = (ਅਚਲ PT*)_stddev;
    ਤਾਰਕਿਕ stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    ਪੂਰਨ i, j, k;
    ਜੇ( !stdmtx || cn == 1 )
    {
        ਜੇ( cn == 1 )
        {
            PT a = stddev[0], b = ਔਸਤ_2[0];
            ਲਈ( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        ਨਹੀਂਤਾਂ
        {
            len *= cn;
            cn--;
            ਲਈ( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + ਔਸਤ_2[k]);
                k = CN_ਅਗਲਾ(k);
            }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        len *= cn;
        cn--;
        ਲਈ( i = j = 0; i < len; i++ )
        {
            PT s = ਔਸਤ_2[j];
            ਪੂਰਨ i0 = i - j;
            ਲਈ( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_ਅਗਲਾ(j);
        }
    }
}

// special version for 16f, 16bf and 32f
ਸਥਿਰ ਖਾਲੀ
randnScale_16_or_32f(ਦਸ਼ਮਲਵ* fbuf, ਖਾਲੀ* _dst, ਪੂਰਨ len, ਪੂਰਨ cn,
                     ਅਚਲ ਖਾਲੀ* ਔਸਤ, ਅਚਲ ਖਾਲੀ* _stddev, ਪੂਰਨ flags)
{
    ਦਸ਼ਮਲਵ* dst = (ਦਸ਼ਮਲਵ*)_dst;
    ਅਚਲ ਦਸ਼ਮਲਵ* ਔਸਤ_2 = (ਅਚਲ ਦਸ਼ਮਲਵ*)ਔਸਤ;
    ਅਚਲ ਦਸ਼ਮਲਵ* stddev = (ਅਚਲ ਦਸ਼ਮਲਵ*)_stddev;
    ਤਾਰਕਿਕ stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    ਪੂਰਨ ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(flags);
    ਦਸ਼ਮਲਵ* arr = ਡੂੰਘਾਈ == CV_16F || ਡੂੰਘਾਈ == CV_16BF || is_FP8_ਡੂੰਘਾਈ(ਡੂੰਘਾਈ) ? fbuf : dst;
    ਪੂਰਨ i, j, k;

    ਜੇ( !stdmtx || cn == 1 )
    {
        ਜੇ( cn == 1 )
        {
            ਦਸ਼ਮਲਵ a = stddev[0], b = ਔਸਤ_2[0];
            ਲਈ( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        ਨਹੀਂਤਾਂ
        {
            len *= cn;
            cn--;
            ਲਈ( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + ਔਸਤ_2[k];
                k = CN_ਅਗਲਾ(k);
            }
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_32F )
    {
        len *= cn;
        cn--;
        ਲਈ( i = j = 0; i < len; i++ )
        {
            ਦਸ਼ਮਲਵ s = ਔਸਤ_2[j];
            ਪੂਰਨ i0 = i - j;
            ਲਈ( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_ਅਗਲਾ(j);
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਦਸ਼ਮਲਵ elembuf[CV_CN_MAX];
        len *= cn;
        ਲਈ( i = 0; i < len; i += cn )
        {
            // since we process fbuf in-place,
            // we need to copy each cn-channel element
            // prior to matrix multiplication
            ਲਈ (j = 0; j < cn; j++)
                elembuf[j] = fbuf[i + j];
            ਲਈ (j = 0; j < cn; j++) {
                ਦਸ਼ਮਲਵ s = ਔਸਤ_2[j];
                ਲਈ( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    ਜੇ (ਡੂੰਘਾਈ == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    ਨਹੀਂਤਾਂ ਜੇ (ਡੂੰਘਾਈ == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    ਨਹੀਂਤਾਂ ਜੇ (is_FP8_ਡੂੰਘਾਈ(ਡੂੰਘਾਈ))
        cvt32f_ਮੰਜ਼ਿਲ_FP8(fbuf, dst, len, ਡੂੰਘਾਈ);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*RandnScaleFunc)(ਦਸ਼ਮਲਵ* src, ਖਾਲੀ* dst, ਪੂਰਨ len, ਪੂਰਨ cn,
                               ਅਚਲ ਖਾਲੀ* ਔਸਤ_2, ਅਚਲ ਖਾਲੀ* stddev, ਪੂਰਨ flags);

ਸਥਿਰ RandnScaleFunc randnScaleTab[CV_ਡੂੰਘਾਈ_MAX] =
{
    randnScale_<uchar, ਦਸ਼ਮਲਵ>, randnScale_<schar, ਦਸ਼ਮਲਵ>, randnScale_<ushort, ਦਸ਼ਮਲਵ>,
    randnScale_<ਛੋਟਾ, ਦਸ਼ਮਲਵ>, randnScale_<ਪੂਰਨ, ਦਸ਼ਮਲਵ>, randnScale_16_or_32f,
    randnScale_<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<ਤਾਰਕਿਕ, ਦਸ਼ਮਲਵ>, randnScale_<uint64_t, ਦੁੱਗਣਾ>, randnScale_<int64_t, ਦੁੱਗਣਾ>,
    randnScale_<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦਸ਼ਮਲਵ>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

ਖਾਲੀ RNG::fill( ਨਿਵੇਸ਼_Output_Array _mat, ਪੂਰਨ disttype,
                ਨਿਵੇਸ਼_Array _param1arg, ਨਿਵੇਸ਼_Array _param2arg,
                ਤਾਰਕਿਕ saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    ਪੂਰਨ j, ਡੂੰਘਾਈ = mat.ਡੂੰਘਾਈ(), cn = mat.channels();
    ਪੂਰਨ esz1 = CV_ELEM_ਮਾਪ(ਡੂੰਘਾਈ);
    Auto_ਬਫਰ<ਦੁੱਗਣਾ> _parambuf;
    ਤਾਰਕਿਕ fast_int_ਬਹੁਲਕ = ਝੂਠ;
    ਤਾਰਕਿਕ small_flag = ਝੂਠ;
    RandFunc func = 0;
    RandnScaleFunc scaleFunc = 0;

    CV_Assert(_param1.channels() == 1 && (_param1.ਪੰਕਤੀਆਂ == 1 || _param1.ਖਾਨੇ_ਗਿਣਤੀ == 1) &&
              (_param1.ਪੰਕਤੀਆਂ + _param1.ਖਾਨੇ_ਗਿਣਤੀ - 1 == cn || _param1.ਪੰਕਤੀਆਂ + _param1.ਖਾਨੇ_ਗਿਣਤੀ - 1 == 1 ||
               (_param1.size() == ਮਾਪ(1, 4) && _param1.type() == CV_64F && cn <= 4)));
    CV_Assert( _param2.channels() == 1 &&
               (((_param2.ਪੰਕਤੀਆਂ == 1 || _param2.ਖਾਨੇ_ਗਿਣਤੀ == 1) &&
                (_param2.ਪੰਕਤੀਆਂ + _param2.ਖਾਨੇ_ਗਿਣਤੀ - 1 == cn || _param2.ਪੰਕਤੀਆਂ + _param2.ਖਾਨੇ_ਗਿਣਤੀ - 1 == 1 ||
                (_param1.size() == ਮਾਪ(1, 4) && _param1.type() == CV_64F && cn <= 4))) ||
                (_param2.ਪੰਕਤੀਆਂ == cn && _param2.ਖਾਨੇ_ਗਿਣਤੀ == cn && disttype == ਸਾਧਾਰਨ_3)));

    ਅਚਲ ਖਾਲੀ* uni_param = 0;
    uchar* ਔਸਤ_2 = 0;
    uchar* stddev = 0;
    ਤਾਰਕਿਕ stdmtx = ਝੂਠ;
    ਪੂਰਨ n1 = (ਪੂਰਨ)_param1.ਕੁੱਲ();
    ਪੂਰਨ n2 = (ਪੂਰਨ)_param2.ਕੁੱਲ();

    ਜੇ( disttype == ਇਕਸਾਰ_2 )
    {
        _parambuf.allocate(cn*(ਆਕਾਰ(DivStruct)+ਆਕਾਰ(ਦੁੱਗਣਾ)-1)/ਆਕਾਰ(ਦੁੱਗਣਾ) + cn*4);
        ਦੁੱਗਣਾ* parambuf = _parambuf.ਅੰਕੜੇ();
        ਦੁੱਗਣਾ* p1 = _param1.ptr<ਦੁੱਗਣਾ>();
        ਦੁੱਗਣਾ* p2 = _param2.ptr<ਦੁੱਗਣਾ>();

        ਜੇ( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convert_ਮੰਜ਼ਿਲ(tmp, CV_64F);
            ਲਈ( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        ਜੇ( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convert_ਮੰਜ਼ਿਲ(tmp, CV_64F);
            ਲਈ( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        ਜੇ( CV_IS_INT_TYPE(ਡੂੰਘਾਈ) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            ਲਈ( j = 0, fast_int_ਬਹੁਲਕ = ਸੱਚ; j < cn; j++ )
            {
                ਦੁੱਗਣਾ a = std::min(p1[j], p2[j]);
                ਦੁੱਗਣਾ b = std::max(p1[j], p2[j]);
                ਜੇ( saturateRange )
                {
                    a = std::max(a, ਡੂੰਘਾਈ == CV_8U || ਡੂੰਘਾਈ == CV_16U || ਡੂੰਘਾਈ == CV_32U ||
                                 ਡੂੰਘਾਈ == CV_64U || ਡੂੰਘਾਈ == CV_Bool ? 0. :
                                 ਡੂੰਘਾਈ == CV_8S ? -128. : ਡੂੰਘਾਈ == CV_16S ? -32768. :
                                 ਡੂੰਘਾਈ == CV_32S ? (ਦੁੱਗਣਾ)INT_MIN : (ਦੁੱਗਣਾ)INT64_MIN);
                    b = std::min(b, ਡੂੰਘਾਈ == CV_8U ? 256. : ਡੂੰਘਾਈ == CV_Bool ? 2. : ਡੂੰਘਾਈ == CV_16U ? 65536. :
                                 ਡੂੰਘਾਈ == CV_8S ? 128. : ਡੂੰਘਾਈ == CV_16S ? 32768. : ਡੂੰਘਾਈ == CV_32U ? (ਦੁੱਗਣਾ)UINT_MAX :
                                 ਡੂੰਘਾਈ == CV_32S ? (ਦੁੱਗਣਾ)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ : (ਦੁੱਗਣਾ)INT64_MAX);
                }
                ip[j][1] = (int64_t)ceil(a);
                int64_t idiff = ip[j][0] = (int64_t)floor(b) - ip[j][1] - 1;
                ਜੇ (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                ਦੁੱਗਣਾ diff = b - a;

                fast_int_ਬਹੁਲਕ = fast_int_ਬਹੁਲਕ && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                ਜੇ( fast_int_ਬਹੁਲਕ )
                    small_flag = idiff <= 255;
                ਨਹੀਂਤਾਂ
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    ਜੇ (ਡੂੰਘਾਈ == CV_64S || ਡੂੰਘਾਈ == CV_64U)
                    {
                        minval = INT64_MIN/2;
                        maxval = INT64_MAX;
                    }
                    ਜੇ( diff > (ਦੁੱਗਣਾ)maxval )
                        ip[j][0] = maxval;
                    ਜੇ( a < (ਦੁੱਗਣਾ)minval )
                        ip[j][1] = minval;
                }
            }

            uni_param = ip;
            ਜੇ( !fast_int_ਬਹੁਲਕ )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((ਖਾਲੀ*)(ds + cn), (ਖਾਲੀ*)(parambuf + _parambuf.size()), "Last byte check");
                ਲਈ( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    ਜੇ (ਡੂੰਘਾਈ != CV_64U && ਡੂੰਘਾਈ != CV_64S) {
                        ਬਿਨਾਨਿਸ਼ਾਨੀ d = ds[j].d = (ਬਿਨਾਨਿਸ਼ਾਨੀ)(ip[j][0]+1);
                        ਪੂਰਨ l = 0;
                        ਜਦੋਂ(((uint64)1 << l) < d)
                            l++;
                        ds[j].M = (ਬਿਨਾਨਿਸ਼ਾਨੀ)(((uint64)1 << 32)*(((uint64)1 << l) - d)/d) + 1;
                        ds[j].sh1 = std::min(l, 1);
                        ds[j].sh2 = std::max(l - 1, 0);
                    }
                }
                uni_param = ds;
            }

            func = randTab[fast_int_ਬਹੁਲਕ ? 1 : 0][ਡੂੰਘਾਈ];
        }
        ਨਹੀਂਤਾਂ
        {
            ਦੁੱਗਣਾ scale = ਡੂੰਘਾਈ == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            ਦੁੱਗਣਾ maxdiff = saturateRange ? (ਦੁੱਗਣਾ)FLT_MAX : DBL_MAX;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            ਜੇ( ਡੂੰਘਾਈ != CV_64F )
            {
                Vec2f* fp = (Vec2f*)(parambuf + cn*2);
                ਲਈ( j = 0; j < cn; j++ )
                {
                    fp[j][0] = (ਦਸ਼ਮਲਵ)(std::min(maxdiff, p2[j] - p1[j])*scale);
                    fp[j][1] = (ਦਸ਼ਮਲਵ)((p2[j] + p1[j])*0.5);
                }
                uni_param = fp;
            }
            ਨਹੀਂਤਾਂ
            {
                Vec2d* dp = (Vec2d*)(parambuf + cn*2);
                ਲਈ( j = 0; j < cn; j++ )
                {
                    dp[j][0] = std::min(DBL_MAX, p2[j] - p1[j])*scale;
                    dp[j][1] = ((p2[j] + p1[j])*0.5);
                }
                uni_param = dp;
            }

            func = randTab[0][ਡੂੰਘਾਈ];
        }
        CV_Assert( func != 0 );
    }
    ਨਹੀਂਤਾਂ ਜੇ( disttype == RNG::ਸਾਧਾਰਨ_3 )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        ਦੁੱਗਣਾ* parambuf = _parambuf.ਅੰਕੜੇ();

        ਪੂਰਨ ptype = esz1 == 8 ? CV_64F : CV_32F;

        ਜੇ( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            ਔਸਤ_2 = _param1.ptr();
        ਨਹੀਂਤਾਂ
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convert_ਮੰਜ਼ਿਲ(tmp, ptype);
            ਔਸਤ_2 = (uchar*)parambuf;
        }

        ਜੇ( n1 < cn )
            ਲਈ( j = n1*esz1; j < cn*esz1; j++ )
                ਔਸਤ_2[j] = ਔਸਤ_2[j - n1*esz1];

        ਜੇ( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        ਨਹੀਂਤਾਂ
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convert_ਮੰਜ਼ਿਲ(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        ਜੇ( n2 < cn )
            ਲਈ( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.ਪੰਕਤੀਆਂ == cn && _param2.ਖਾਨੇ_ਗਿਣਤੀ == cn;
        scaleFunc = randnScaleTab[ਡੂੰਘਾਈ];
        CV_Assert( scaleFunc != 0 );
    }
    ਨਹੀਂਤਾਂ
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    ਅਚਲ Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    ਦਸ਼ਮਲਵ fbuf[ਖੰਡ_ਮਾਪ + CV_CN_MAX];
    ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)it.size;
    ਪੂਰਨ ਖੰਡ_ਮਾਪ_2 = std::min((ਖੰਡ_ਮਾਪ + cn - 1)/cn, ਕੁੱਲ);
    size_t esz = (size_t)esz1*cn;
    ਪੂਰਨ flags = mat.type();

    ਜੇ( disttype == ਇਕਸਾਰ_2 )
        flags |= (small_flag ? (ਪੂਰਨ)RNG_FLAG_SMALL : 0);
    ਨਹੀਂਤਾਂ
        flags |= (stdmtx ? (ਪੂਰਨ)RNG_FLAG_STDMTX : 0);

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ਲਈ( j = 0; j < ਕੁੱਲ; j += ਖੰਡ_ਮਾਪ_2 )
        {
            ਪੂਰਨ len = std::min(ਕੁੱਲ - j, ਖੰਡ_ਮਾਪ_2);

            ਜੇ( disttype == ਇਕਸਾਰ_2 )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            ਨਹੀਂਤਾਂ
            {
                randn_0_1_32f(fbuf, len*cn, &state);
                scaleFunc(fbuf, ptr + j*esz, len, cn, ਔਸਤ_2, stddev, flags);
            }
        }
    }
}

}

cv::RNG& cv::theRNG()
{
    ਵਾਪਸ get_Core_Tls_ਅੰਕੜੇ().rng;
}

ਖਾਲੀ cv::ਸੈੱਟ_RNGSeed(ਪੂਰਨ ਬੀਜ)
{
    theRNG() = RNG(static_cast<uint64>(ਬੀਜ));
}


ਖਾਲੀ cv::randu(ਨਿਵੇਸ਼_Output_Array dst, ਨਿਵੇਸ਼_Array ਨੀਵਾਂ, ਨਿਵੇਸ਼_Array ਉੱਚਾ)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::ਇਕਸਾਰ_2, ਨੀਵਾਂ, ਉੱਚਾ);
}

ਖਾਲੀ cv::randn(ਨਿਵੇਸ਼_Output_Array dst, ਨਿਵੇਸ਼_Array ਔਸਤ_2, ਨਿਵੇਸ਼_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::ਸਾਧਾਰਨ_3, ਔਸਤ_2, stddev);
}

ਨਾਮਾਕਾਸ਼ cv
{

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
randShuffle_( Mat& _arr, RNG& rng, ਦੁੱਗਣਾ )
{
    ਬਿਨਾਨਿਸ਼ਾਨੀ sz = (ਬਿਨਾਨਿਸ਼ਾਨੀ)_arr.ਕੁੱਲ();
    ਜੇ( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        ਲਈ( ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < sz; i++ )
        {
            ਬਿਨਾਨਿਸ਼ਾਨੀ j = (ਬਿਨਾਨਿਸ਼ਾਨੀ)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* ਅੰਕੜੇ = _arr.ptr();
        size_t ਕਦਮ = _arr.ਕਦਮ;
        ਪੂਰਨ ਪੰਕਤੀਆਂ = _arr.ਪੰਕਤੀਆਂ;
        ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ = _arr.ਖਾਨੇ_ਗਿਣਤੀ;
        ਲਈ( ਪੂਰਨ i0 = 0; i0 < ਪੰਕਤੀਆਂ; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            ਲਈ( ਪੂਰਨ j0 = 0; j0 < ਖਾਨੇ_ਗਿਣਤੀ; j0++ )
            {
                ਬਿਨਾਨਿਸ਼ਾਨੀ k1 = (ਬਿਨਾਨਿਸ਼ਾਨੀ)rng % sz;
                ਪੂਰਨ i1 = (ਪੂਰਨ)(k1 / ਖਾਨੇ_ਗਿਣਤੀ);
                ਪੂਰਨ j1 = (ਪੂਰਨ)(k1 - (ਬਿਨਾਨਿਸ਼ਾਨੀ)i1*(ਬਿਨਾਨਿਸ਼ਾਨੀ)ਖਾਨੇ_ਗਿਣਤੀ);
                std::swap( p[j0], ((T*)(ਅੰਕੜੇ + ਕਦਮ*i1))[j1] );
            }
        }
    }
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*RandShuffleFunc)( Mat& dst, RNG& rng, ਦੁੱਗਣਾ iter_ਗੁਣਕ );

}

ਖਾਲੀ cv::randShuffle( ਨਿਵੇਸ਼_Output_Array _dst, ਦੁੱਗਣਾ iter_ਗੁਣਕ, RNG* _rng )
{
    CV_INSTRUMENT_REGION();

    RandShuffleFunc tab[] =
    {
        0,
        randShuffle_<uchar>, // 1
        randShuffle_<ushort>, // 2
        randShuffle_<Vec<uchar,3> >, // 3
        randShuffle_<ਪੂਰਨ>, // 4
        0,
        randShuffle_<Vec<ushort,3> >, // 6
        0,
        randShuffle_<Vec<ਪੂਰਨ,2> >, // 8
        0, 0, 0,
        randShuffle_<Vec<ਪੂਰਨ,3> >, // 12
        0, 0, 0,
        randShuffle_<Vec<ਪੂਰਨ,4> >, // 16
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<ਪੂਰਨ,6> >, // 24
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<ਪੂਰਨ,8> > // 32
    };

    Mat dst = _dst.getMat();
    RNG& rng = _rng ? *_rng : theRNG();
    CV_Assert( dst.elem_ਮਾਪ() <= 32 );
    RandShuffleFunc func = tab[dst.elem_ਮਾਪ()];
    CV_Assert( func != 0 );
    func( dst, rng, iter_ਗੁਣਕ );
}


// Mersenne Twister random number generator.
// Inspired by http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/CODES/mt19937ar.c

/*
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

cv::RNG_MT19937::RNG_MT19937(ਬਿਨਾਨਿਸ਼ਾਨੀ s) { ਬੀਜ(s); }

cv::RNG_MT19937::RNG_MT19937() { ਬੀਜ(5489U); }

ਖਾਲੀ cv::RNG_MT19937::ਬੀਜ(ਬਿਨਾਨਿਸ਼ਾਨੀ s)
{
    state[0]= s;
    ਲਈ (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

ਬਿਨਾਨਿਸ਼ਾਨੀ cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਉੱਤਲਾ_ਨਕਾਬ = 0x80000000U;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਹੇਠਲਾ_ਨਕਾਬ = 0x7fffffffU;

    /* generate N words at one time */
    ਜੇ (mti >= N)
    {
        ਪੂਰਨ kk = 0;

        ਲਈ (; kk < N - M; ++kk)
        {
            ਬਿਨਾਨਿਸ਼ਾਨੀ y = (state[kk] & ਉੱਤਲਾ_ਨਕਾਬ) | (state[kk + 1] & ਹੇਠਲਾ_ਨਕਾਬ);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        ਲਈ (; kk < N - 1; ++kk)
        {
            ਬਿਨਾਨਿਸ਼ਾਨੀ y = (state[kk] & ਉੱਤਲਾ_ਨਕਾਬ) | (state[kk + 1] & ਹੇਠਲਾ_ਨਕਾਬ);
            state[kk] = state[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        ਬਿਨਾਨਿਸ਼ਾਨੀ y = (state[N - 1] & ਉੱਤਲਾ_ਨਕਾਬ) | (state[0] & ਹੇਠਲਾ_ਨਕਾਬ);
        state[N - 1] = state[M - 1] ^ (y >> 1) ^ mag01[y & 0x1U];

        mti = 0;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ y = state[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y <<  7) & 0x9d2c5680U;
    y ^= (y << 15) & 0xefc60000U;
    y ^= (y >> 18);

    ਵਾਪਸ y;
}

cv::RNG_MT19937::ਸੰਚਾਲਕ ਬਿਨਾਨਿਸ਼ਾਨੀ() { ਵਾਪਸ next(); }

cv::RNG_MT19937::ਸੰਚਾਲਕ ਪੂਰਨ() { ਵਾਪਸ (ਪੂਰਨ)next();}

cv::RNG_MT19937::ਸੰਚਾਲਕ ਦਸ਼ਮਲਵ() { ਵਾਪਸ next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::ਸੰਚਾਲਕ ਦੁੱਗਣਾ()
{
    ਬਿਨਾਨਿਸ਼ਾਨੀ a = next() >> 5;
    ਬਿਨਾਨਿਸ਼ਾਨੀ b = next() >> 6;
    ਵਾਪਸ (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

ਪੂਰਨ cv::RNG_MT19937::ਇਕਸਾਰ(ਪੂਰਨ a, ਪੂਰਨ b) { ਵਾਪਸ (ਪੂਰਨ)(next() % (b - a) + a); }

ਦਸ਼ਮਲਵ cv::RNG_MT19937::ਇਕਸਾਰ(ਦਸ਼ਮਲਵ a, ਦਸ਼ਮਲਵ b) { ਵਾਪਸ ((ਦਸ਼ਮਲਵ)*ਇਹ)*(b - a) + a; }

ਦੁੱਗਣਾ cv::RNG_MT19937::ਇਕਸਾਰ(ਦੁੱਗਣਾ a, ਦੁੱਗਣਾ b) { ਵਾਪਸ ((ਦੁੱਗਣਾ)*ਇਹ)*(b - a) + a; }

ਬਿਨਾਨਿਸ਼ਾਨੀ cv::RNG_MT19937::ਸੰਚਾਲਕ ()(ਬਿਨਾਨਿਸ਼ਾਨੀ b) { ਵਾਪਸ next() % b; }

ਬਿਨਾਨਿਸ਼ਾਨੀ cv::RNG_MT19937::ਸੰਚਾਲਕ ()() { ਵਾਪਸ next(); }

/* End of file. */
