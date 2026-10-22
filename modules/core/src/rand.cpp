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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv
{

///////////////////////////// Functions Declaration //////////////////////////////////////

#ವ್ಯಾಖ್ಯಾನಿಸು CV_RNG_COEFF 4164903690U

/*
   Multiply-with-carry generator is used here:
   temp = ( A*X(n) + carry )
   X(n+1) = temp mod (2^32)
   carry = floor (temp / (2^32))
*/

#ವ್ಯಾಖ್ಯಾನಿಸು  RNG_ಮುಂದಿನ(x)    ((uint64)(ಚಿಹ್ನೆರಹಿತ)(x)*CV_RNG_COEFF + ((x) >> 32))
// make it jump-less
#ವ್ಯಾಖ್ಯಾನಿಸು  CN_ಮುಂದಿನ(k)     (((k) + 1) & (((k) >= cn) - 1))

ಎಣಿಕೆ
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
rand_ಬಿಟ್ಟುಗಳು( T* arr, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, uint64* state, ಸ್ಥಿರಾಂಕ Vec2l* p, ಪೂರ್ಣಾಂಕ flags )
{
    ತಾರ್ಕಿಕ small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 ತಾತ್ಕಾಲಿಕ_2 = *state;
    ಪೂರ್ಣಾಂಕ i, k = 0;
    len *= cn;
    --cn;

    ಆದರೆ( !small_flag )
    {
        ಗಾಗಿ( i = 0; i <= len - 4; i += 4 )
        {
            int64_t t0, t1;

            ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
            t0 = ((int64_t)ತಾತ್ಕಾಲಿಕ_2 & p[k][0]) + p[k][1];
            k = CN_ಮುಂದಿನ(k);
            ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
            t1 = ((int64_t)ತಾತ್ಕಾಲಿಕ_2 & p[k][0]) + p[k][1];
            k = CN_ಮುಂದಿನ(k);
            arr[i] = saturate_cast<T>(t0);
            arr[i+1] = saturate_cast<T>(t1);

            ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
            t0 = ((int64_t)ತಾತ್ಕಾಲಿಕ_2 & p[k][0]) + p[k][1];
            k = CN_ಮುಂದಿನ(k);
            ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
            t1 = ((int64_t)ತಾತ್ಕಾಲಿಕ_2 & p[k][0]) + p[k][1];
            k = CN_ಮುಂದಿನ(k);
            arr[i+2] = saturate_cast<T>(t0);
            arr[i+3] = saturate_cast<T>(t1);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( i = 0; i <= len - 4; i += 4 )
        {
            int64_t t0, t1, t;
            ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
            t = ತಾತ್ಕಾಲಿಕ_2;
            // p[i+...][0] is within 0..255 in this branch (small_flag==true),
            // so we don't need to do (t>>...)&255,
            // the upper bits will be cleaned with ... & p[i+...][0].
            t0 = (t & p[k][0]) + p[k][1];
            k = CN_ಮುಂದಿನ(k);
            t1 = ((t >> 8) & p[k][0]) + p[k][1];
            k = CN_ಮುಂದಿನ(k);
            arr[i] = saturate_cast<T>(t0);
            arr[i+1] = saturate_cast<T>(t1);

            t0 = ((t >> 16) & p[k][0]) + p[k][1];
            k = CN_ಮುಂದಿನ(k);
            t1 = ((t >> 24) & p[k][0]) + p[k][1];
            k = CN_ಮುಂದಿನ(k);
            arr[i+2] = saturate_cast<T>(t0);
            arr[i+3] = saturate_cast<T>(t1);
        }
    }

    ಗಾಗಿ( ; i < len; i++ )
    {
        int64_t t0;
        ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);

        t0 = ((int64_t)ತಾತ್ಕಾಲಿಕ_2 & p[k][0]) + p[k][1];
        k = CN_ಮುಂದಿನ(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = ತಾತ್ಕಾಲಿಕ_2;
}

ರಚನೆ DivStruct
{
    ಚಿಹ್ನೆರಹಿತ d;
    ಚಿಹ್ನೆರಹಿತ M;
    ಪೂರ್ಣಾಂಕ sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
randi_( T* arr, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, uint64* state, ಸ್ಥಿರಾಂಕ DivStruct* p )
{
    uint64 ತಾತ್ಕಾಲಿಕ_2 = *state;
    ಪೂರ್ಣಾಂಕ k = 0;
    len *= cn;
    cn--;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++ )
    {
        ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
        ಚಿಹ್ನೆರಹಿತ t = (ಚಿಹ್ನೆರಹಿತ)ತಾತ್ಕಾಲಿಕ_2;
        ಚಿಹ್ನೆರಹಿತ v = (ಚಿಹ್ನೆರಹಿತ)(((uint64)t * p[k].M) >> 32);
        v = (v + ((t - v) >> p[k].sh1)) >> p[k].sh2;
        int64_t res = (int64_t)(t - v*p[k].d) + p[k].delta;
        k = CN_ಮುಂದಿನ(k);
        arr[i] = saturate_cast<T>(res);
    }
    *state = ತಾತ್ಕಾಲಿಕ_2;
}

ಸ್ಥಿರ ಶೂನ್ಯ
randi_( int64_t* arr, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, uint64* state, ಸ್ಥಿರಾಂಕ DivStruct* p )
{
    uint64 ತಾತ್ಕಾಲಿಕ_2 = *state;
    ಪೂರ್ಣಾಂಕ k = 0;
    len *= cn;
    cn--;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++ )
    {
        ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
        ಚಿಹ್ನೆರಹಿತ t0 = (ಚಿಹ್ನೆರಹಿತ)ತಾತ್ಕಾಲಿಕ_2;
        ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
        ಚಿಹ್ನೆರಹಿತ t1 = (ಚಿಹ್ನೆರಹಿತ)ತಾತ್ಕಾಲಿಕ_2;
        int64_t t = (int64_t)((((uint64_t)t0 << 32) | t1) % p[k].diff) + p[k].delta;
        k = CN_ಮುಂದಿನ(k);
        arr[i] = t;
    }
    *state = ತಾತ್ಕಾಲಿಕ_2;
}

ಸ್ಥಿರ ಶೂನ್ಯ
randi_( uint64_t* arr, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, uint64* state, ಸ್ಥಿರಾಂಕ DivStruct* p )
{
    uint64 ತಾತ್ಕಾಲಿಕ_2 = *state;
    ಪೂರ್ಣಾಂಕ k = 0;
    len *= cn;
    cn--;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++ )
    {
        ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
        ಚಿಹ್ನೆರಹಿತ t0 = (ಚಿಹ್ನೆರಹಿತ)ತಾತ್ಕಾಲಿಕ_2;
        ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
        ಚಿಹ್ನೆರಹಿತ t1 = (ಚಿಹ್ನೆರಹಿತ)ತಾತ್ಕಾಲಿಕ_2;
        uint64_t t = (((uint64_t)t0 << 32) | t1) % p[k].diff;
        int64_t delta = p[k].delta;
        k = CN_ಮುಂದಿನ(k);
        arr[i] = delta >= 0 || t >= (uint64_t)-delta ? t + (uint64_t)delta : 0;
    }
    *state = ತಾತ್ಕಾಲಿಕ_2;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಶೂನ್ಯ rand_ಬಿಟ್ಟುಗಳು_Wrap(ಶೂನ್ಯ* arr, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, uint64* state, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* p, ಶೂನ್ಯ*, ಪೂರ್ಣಾಂಕ flags)
{
    rand_ಬಿಟ್ಟುಗಳು((T*)arr, len, cn, state, (ಸ್ಥಿರಾಂಕ Vec2l*)p, flags);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಶೂನ್ಯ randiWrap(ಶೂನ್ಯ* arr, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, uint64* state, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* p, ಶೂನ್ಯ*, ಪೂರ್ಣಾಂಕ)
{
    randi_((T*)arr, len, cn, state, (ಸ್ಥಿರಾಂಕ DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ cvt32f_ಗಮ್ಯ_FP8(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಆದರೆ (ಆಳ == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    ಇಲ್ಲದಿದ್ದರೆ                              { fp8a_t* d = (fp8a_t*)dst; ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ is_FP8_ಆಳ(ಪೂರ್ಣಾಂಕ d) { ಹಿಂತಿರುಗಿ d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

ಸ್ಥಿರ ಶೂನ್ಯ randf_16_or_32f( ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಂಕ len_, ಪೂರ್ಣಾಂಕ cn, uint64* state, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _p, ಶೂನ್ಯ* _fbuf, ಪೂರ್ಣಾಂಕ flags )
{
    ಸ್ಥಿರಾಂಕ Vec2f* p = (ಸ್ಥಿರಾಂಕ Vec2f*)_p;
    ದಶಮಾಂಶ* fbuf = (ದಶಮಾಂಶ*)_fbuf;
    ಪೂರ್ಣಾಂಕ ಆಳ = CV_MAT_ಆಳ(flags);
    uint64 ತಾತ್ಕಾಲಿಕ_2 = *state;
    ಪೂರ್ಣಾಂಕ k = 0, len = len_*cn;
    ದಶಮಾಂಶ* arr = ಆಳ == CV_16F || ಆಳ == CV_16BF || is_FP8_ಆಳ(ಆಳ) ? fbuf : (ದಶಮಾಂಶ*)dst;
    cn--;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++ )
    {
        ಪೂರ್ಣಾಂಕ t = (ಪೂರ್ಣಾಂಕ)(ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2));
        arr[i] = (ದಶಮಾಂಶ)(t*p[k][0]);
        k = CN_ಮುಂದಿನ(k);
    }
    *state = ತಾತ್ಕಾಲಿಕ_2;
    hal::ಕೂಡಿಸು_RNGBias32f(arr, &p[0][0], len_, cn+1);
    ಆದರೆ (ಆಳ == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆಳ == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_FP8_ಆಳ(ಆಳ))
        cvt32f_ಗಮ್ಯ_FP8(fbuf, dst, len, ಆಳ);
}

ಸ್ಥಿರ ಶೂನ್ಯ
randf_64f( ಶೂನ್ಯ* _arr, ಪೂರ್ಣಾಂಕ len_, ಪೂರ್ಣಾಂಕ cn, uint64* state, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _p, ಶೂನ್ಯ*, ಪೂರ್ಣಾಂಕ )
{
    ದ್ವಿಗುಣ* arr = (ದ್ವಿಗುಣ*)_arr;
    ಸ್ಥಿರಾಂಕ Vec2d* p = (ಸ್ಥಿರಾಂಕ Vec2d*)_p;
    uint64 ತಾತ್ಕಾಲಿಕ_2 = *state;
    ಪೂರ್ಣಾಂಕ k = 0, len = len_*cn;
    cn--;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++ )
    {
        ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
        int64_t v = (int64_t)((ತಾತ್ಕಾಲಿಕ_2 >> 32) | (ತಾತ್ಕಾಲಿಕ_2 << 32));
        arr[i] = v*p[k][0];
        k = CN_ಮುಂದಿನ(k);
    }
    *state = ತಾತ್ಕಾಲಿಕ_2;
    hal::ಕೂಡಿಸು_RNGBias64f(arr, &p[0][0], len_, cn+1);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*RandFunc)(ಶೂನ್ಯ* arr, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, uint64* state,
                         ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* p, ಶೂನ್ಯ* tempbuf, ಪೂರ್ಣಾಂಕ flags);

ಸ್ಥಿರ RandFunc randTab[CV_ಆಳ_MAX][CV_ಆಳ_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<ಗಿಡ್ಡ>, randiWrap<ಪೂರ್ಣಾಂಕ>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<ತಾರ್ಕಿಕ>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<ಚಿಹ್ನೆರಹಿತ>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        rand_ಬಿಟ್ಟುಗಳು_Wrap<uchar>, rand_ಬಿಟ್ಟುಗಳು_Wrap<schar>, rand_ಬಿಟ್ಟುಗಳು_Wrap<ushort>,
        rand_ಬಿಟ್ಟುಗಳು_Wrap<ಗಿಡ್ಡ>, rand_ಬಿಟ್ಟುಗಳು_Wrap<ಪೂರ್ಣಾಂಕ>, 0, 0, 0, 0,
        rand_ಬಿಟ್ಟುಗಳು_Wrap<ತಾರ್ಕಿಕ>, rand_ಬಿಟ್ಟುಗಳು_Wrap<uint64_t>, rand_ಬಿಟ್ಟುಗಳು_Wrap<int64_t>,
        rand_ಬಿಟ್ಟುಗಳು_Wrap<ಚಿಹ್ನೆರಹಿತ>, 0, 0, 0
    }
};

/*
   The code below implements the algorithm described in
   "The Ziggurat Method for Generating Random Variables"
   by George Marsaglia and Wai Wan Tsang, Journal of Statistical Software, 2007.
*/
ಸ್ಥಿರ ಶೂನ್ಯ
randn_0_1_32f( ದಶಮಾಂಶ* arr, ಪೂರ್ಣಾಂಕ len, uint64* state )
{
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ r = 3.442620f; // The start of the right tail
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ rng_flt = 2.3283064365386962890625e-10f; // 2^-32
    ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ kn[128];
    ಸ್ಥಿರ ದಶಮಾಂಶ wn[128], fn[128];
    uint64 ತಾತ್ಕಾಲಿಕ_2 = *state;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized=ಸುಳ್ಳು;
    ಪೂರ್ಣಾಂಕ i;

    ಆದರೆ( !initialized )
    {
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ m1 = 2147483648.0;
        ದ್ವಿಗುಣ dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        ದ್ವಿಗುಣ q = vn/std::ಘಾತಾಂಕ(-.5*dn*dn);
        kn[0] = (ಚಿಹ್ನೆರಹಿತ)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (ದಶಮಾಂಶ)(q/m1);
        wn[127] = (ದಶಮಾಂಶ)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (ದಶಮಾಂಶ)std::ಘಾತಾಂಕ(-.5*dn*dn);

        ಗಾಗಿ(i=126;i>=1;i--)
        {
            dn = std::ವರ್ಗಮೂಲ(-2.*std::ಲಘುಗಣಕ(vn/dn+std::ಘಾತಾಂಕ(-.5*dn*dn)));
            kn[i+1] = (ಚಿಹ್ನೆರಹಿತ)((dn/tn)*m1);
            tn = dn;
            fn[i] = (ದಶಮಾಂಶ)std::ಘಾತಾಂಕ(-.5*dn*dn);
            wn[i] = (ದಶಮಾಂಶ)(dn/m1);
        }
        initialized = ನಿಜ;
    }

    ಗಾಗಿ( i = 0; i < len; i++ )
    {
        ದಶಮಾಂಶ x, y;
        ಗಾಗಿ(;;)
        {
            ಪೂರ್ಣಾಂಕ hz = (ಪೂರ್ಣಾಂಕ)ತಾತ್ಕಾಲಿಕ_2;
            ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
            ಪೂರ್ಣಾಂಕ iz = hz & 127;
            x = hz*wn[iz];
            ಆದರೆ( (ಚಿಹ್ನೆರಹಿತ)std::ನಿರಪೇಕ್ಷ(hz) < kn[iz] )
                ನಿಲ್ಲಿಸು;
            ಆದರೆ( iz == 0) // iz==0, handles the base strip
            {
                ಮಾಡು
                {
                    x = (ಚಿಹ್ನೆರಹಿತ)ತಾತ್ಕಾಲಿಕ_2*rng_flt;
                    ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
                    y = (ಚಿಹ್ನೆರಹಿತ)ತಾತ್ಕಾಲಿಕ_2*rng_flt;
                    ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
                    x = (ದಶಮಾಂಶ)(-std::ಲಘುಗಣಕ(x+FLT_MIN)*0.2904764);
                    y = (ದಶಮಾಂಶ)-std::ಲಘುಗಣಕ(y+FLT_MIN);
                }	// .2904764 is 1/r
                ಆಗ( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                ನಿಲ್ಲಿಸು;
            }
            // iz > 0, handle the wedges of other strips
            y = (ಚಿಹ್ನೆರಹಿತ)ತಾತ್ಕಾಲಿಕ_2*rng_flt;
            ತಾತ್ಕಾಲಿಕ_2 = RNG_ಮುಂದಿನ(ತಾತ್ಕಾಲಿಕ_2);
            ಆದರೆ( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::ಘಾತಾಂಕ(-.5*x*x) )
                ನಿಲ್ಲಿಸು;
        }
        arr[i] = x;
    }
    *state = ತಾತ್ಕಾಲಿಕ_2;
}


ದ್ವಿಗುಣ RNG::gaussian(ದ್ವಿಗುಣ sigma)
{
    ದಶಮಾಂಶ ತಾತ್ಕಾಲಿಕ_2;
    randn_0_1_32f( &ತಾತ್ಕಾಲಿಕ_2, 1, &state );
    ಹಿಂತಿರುಗಿ ತಾತ್ಕಾಲಿಕ_2*sigma;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು PT> ಸ್ಥಿರ ಶೂನ್ಯ
randnScale_(ದಶಮಾಂಶ* src, ಶೂನ್ಯ* _dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn,
            ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* ಸರಾಸರಿ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _stddev, ಪೂರ್ಣಾಂಕ flags )
{
    T* dst = (T*)_dst;
    ಸ್ಥಿರಾಂಕ PT* ಸರಾಸರಿ_2 = (ಸ್ಥಿರಾಂಕ PT*)ಸರಾಸರಿ;
    ಸ್ಥಿರಾಂಕ PT* stddev = (ಸ್ಥಿರಾಂಕ PT*)_stddev;
    ತಾರ್ಕಿಕ stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    ಪೂರ್ಣಾಂಕ i, j, k;
    ಆದರೆ( !stdmtx || cn == 1 )
    {
        ಆದರೆ( cn == 1 )
        {
            PT a = stddev[0], b = ಸರಾಸರಿ_2[0];
            ಗಾಗಿ( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            len *= cn;
            cn--;
            ಗಾಗಿ( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + ಸರಾಸರಿ_2[k]);
                k = CN_ಮುಂದಿನ(k);
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        len *= cn;
        cn--;
        ಗಾಗಿ( i = j = 0; i < len; i++ )
        {
            PT s = ಸರಾಸರಿ_2[j];
            ಪೂರ್ಣಾಂಕ i0 = i - j;
            ಗಾಗಿ( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_ಮುಂದಿನ(j);
        }
    }
}

// special version for 16f, 16bf and 32f
ಸ್ಥಿರ ಶೂನ್ಯ
randnScale_16_or_32f(ದಶಮಾಂಶ* fbuf, ಶೂನ್ಯ* _dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn,
                     ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* ಸರಾಸರಿ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _stddev, ಪೂರ್ಣಾಂಕ flags)
{
    ದಶಮಾಂಶ* dst = (ದಶಮಾಂಶ*)_dst;
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* ಸರಾಸರಿ_2 = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)ಸರಾಸರಿ;
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* stddev = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)_stddev;
    ತಾರ್ಕಿಕ stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    ಪೂರ್ಣಾಂಕ ಆಳ = CV_MAT_ಆಳ(flags);
    ದಶಮಾಂಶ* arr = ಆಳ == CV_16F || ಆಳ == CV_16BF || is_FP8_ಆಳ(ಆಳ) ? fbuf : dst;
    ಪೂರ್ಣಾಂಕ i, j, k;

    ಆದರೆ( !stdmtx || cn == 1 )
    {
        ಆದರೆ( cn == 1 )
        {
            ದಶಮಾಂಶ a = stddev[0], b = ಸರಾಸರಿ_2[0];
            ಗಾಗಿ( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            len *= cn;
            cn--;
            ಗಾಗಿ( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + ಸರಾಸರಿ_2[k];
                k = CN_ಮುಂದಿನ(k);
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_32F )
    {
        len *= cn;
        cn--;
        ಗಾಗಿ( i = j = 0; i < len; i++ )
        {
            ದಶಮಾಂಶ s = ಸರಾಸರಿ_2[j];
            ಪೂರ್ಣಾಂಕ i0 = i - j;
            ಗಾಗಿ( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_ಮುಂದಿನ(j);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ದಶಮಾಂಶ elembuf[CV_CN_MAX];
        len *= cn;
        ಗಾಗಿ( i = 0; i < len; i += cn )
        {
            // since we process fbuf in-place,
            // we need to copy each cn-channel element
            // prior to matrix multiplication
            ಗಾಗಿ (j = 0; j < cn; j++)
                elembuf[j] = fbuf[i + j];
            ಗಾಗಿ (j = 0; j < cn; j++) {
                ದಶಮಾಂಶ s = ಸರಾಸರಿ_2[j];
                ಗಾಗಿ( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    ಆದರೆ (ಆಳ == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆಳ == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_FP8_ಆಳ(ಆಳ))
        cvt32f_ಗಮ್ಯ_FP8(fbuf, dst, len, ಆಳ);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*RandnScaleFunc)(ದಶಮಾಂಶ* src, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn,
                               ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* ಸರಾಸರಿ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* stddev, ಪೂರ್ಣಾಂಕ flags);

ಸ್ಥಿರ RandnScaleFunc randnScaleTab[CV_ಆಳ_MAX] =
{
    randnScale_<uchar, ದಶಮಾಂಶ>, randnScale_<schar, ದಶಮಾಂಶ>, randnScale_<ushort, ದಶಮಾಂಶ>,
    randnScale_<ಗಿಡ್ಡ, ದಶಮಾಂಶ>, randnScale_<ಪೂರ್ಣಾಂಕ, ದಶಮಾಂಶ>, randnScale_16_or_32f,
    randnScale_<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<ತಾರ್ಕಿಕ, ದಶಮಾಂಶ>, randnScale_<uint64_t, ದ್ವಿಗುಣ>, randnScale_<int64_t, ದ್ವಿಗುಣ>,
    randnScale_<ಚಿಹ್ನೆರಹಿತ, ದಶಮಾಂಶ>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

ಶೂನ್ಯ RNG::fill( ಒಳಹರಿವು_Output_Array _mat, ಪೂರ್ಣಾಂಕ disttype,
                ಒಳಹರಿವು_Array _param1arg, ಒಳಹರಿವು_Array _param2arg,
                ತಾರ್ಕಿಕ saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    ಪೂರ್ಣಾಂಕ j, ಆಳ = mat.ಆಳ(), cn = mat.channels();
    ಪೂರ್ಣಾಂಕ esz1 = CV_ELEM_ಗಾತ್ರ(ಆಳ);
    Auto_ಬಫರ್<ದ್ವಿಗುಣ> _parambuf;
    ತಾರ್ಕಿಕ fast_int_ಬಹುಲಕ = ಸುಳ್ಳು;
    ತಾರ್ಕಿಕ small_flag = ಸುಳ್ಳು;
    RandFunc func = 0;
    RandnScaleFunc scaleFunc = 0;

    CV_Assert(_param1.channels() == 1 && (_param1.ಅಡ್ಡಸಾಲುಗಳು == 1 || _param1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1) &&
              (_param1.ಅಡ್ಡಸಾಲುಗಳು + _param1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1 == cn || _param1.ಅಡ್ಡಸಾಲುಗಳು + _param1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1 == 1 ||
               (_param1.size() == ಗಾತ್ರ_2(1, 4) && _param1.type() == CV_64F && cn <= 4)));
    CV_Assert( _param2.channels() == 1 &&
               (((_param2.ಅಡ್ಡಸಾಲುಗಳು == 1 || _param2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1) &&
                (_param2.ಅಡ್ಡಸಾಲುಗಳು + _param2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1 == cn || _param2.ಅಡ್ಡಸಾಲುಗಳು + _param2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1 == 1 ||
                (_param1.size() == ಗಾತ್ರ_2(1, 4) && _param1.type() == CV_64F && cn <= 4))) ||
                (_param2.ಅಡ್ಡಸಾಲುಗಳು == cn && _param2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == cn && disttype == ಸಾಮಾನ್ಯ_3)));

    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* uni_param = 0;
    uchar* ಸರಾಸರಿ_2 = 0;
    uchar* stddev = 0;
    ತಾರ್ಕಿಕ stdmtx = ಸುಳ್ಳು;
    ಪೂರ್ಣಾಂಕ n1 = (ಪೂರ್ಣಾಂಕ)_param1.ಒಟ್ಟು();
    ಪೂರ್ಣಾಂಕ n2 = (ಪೂರ್ಣಾಂಕ)_param2.ಒಟ್ಟು();

    ಆದರೆ( disttype == ಏಕರೂಪ_2 )
    {
        _parambuf.allocate(cn*(ಗಾತ್ರ(DivStruct)+ಗಾತ್ರ(ದ್ವಿಗುಣ)-1)/ಗಾತ್ರ(ದ್ವಿಗುಣ) + cn*4);
        ದ್ವಿಗುಣ* parambuf = _parambuf.ದತ್ತಾಂಶ();
        ದ್ವಿಗುಣ* p1 = _param1.ptr<ದ್ವಿಗುಣ>();
        ದ್ವಿಗುಣ* p2 = _param2.ptr<ದ್ವಿಗುಣ>();

        ಆದರೆ( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convert_ಗಮ್ಯ(tmp, CV_64F);
            ಗಾಗಿ( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        ಆದರೆ( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convert_ಗಮ್ಯ(tmp, CV_64F);
            ಗಾಗಿ( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        ಆದರೆ( CV_IS_INT_TYPE(ಆಳ) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            ಗಾಗಿ( j = 0, fast_int_ಬಹುಲಕ = ನಿಜ; j < cn; j++ )
            {
                ದ್ವಿಗುಣ a = std::min(p1[j], p2[j]);
                ದ್ವಿಗುಣ b = std::max(p1[j], p2[j]);
                ಆದರೆ( saturateRange )
                {
                    a = std::max(a, ಆಳ == CV_8U || ಆಳ == CV_16U || ಆಳ == CV_32U ||
                                 ಆಳ == CV_64U || ಆಳ == CV_Bool ? 0. :
                                 ಆಳ == CV_8S ? -128. : ಆಳ == CV_16S ? -32768. :
                                 ಆಳ == CV_32S ? (ದ್ವಿಗುಣ)INT_MIN : (ದ್ವಿಗುಣ)INT64_MIN);
                    b = std::min(b, ಆಳ == CV_8U ? 256. : ಆಳ == CV_Bool ? 2. : ಆಳ == CV_16U ? 65536. :
                                 ಆಳ == CV_8S ? 128. : ಆಳ == CV_16S ? 32768. : ಆಳ == CV_32U ? (ದ್ವಿಗುಣ)UINT_MAX :
                                 ಆಳ == CV_32S ? (ದ್ವಿಗುಣ)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ : (ದ್ವಿಗುಣ)INT64_MAX);
                }
                ip[j][1] = (int64_t)ceil(a);
                int64_t idiff = ip[j][0] = (int64_t)floor(b) - ip[j][1] - 1;
                ಆದರೆ (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                ದ್ವಿಗುಣ diff = b - a;

                fast_int_ಬಹುಲಕ = fast_int_ಬಹುಲಕ && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                ಆದರೆ( fast_int_ಬಹುಲಕ )
                    small_flag = idiff <= 255;
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    ಆದರೆ (ಆಳ == CV_64S || ಆಳ == CV_64U)
                    {
                        minval = INT64_MIN/2;
                        maxval = INT64_MAX;
                    }
                    ಆದರೆ( diff > (ದ್ವಿಗುಣ)maxval )
                        ip[j][0] = maxval;
                    ಆದರೆ( a < (ದ್ವಿಗುಣ)minval )
                        ip[j][1] = minval;
                }
            }

            uni_param = ip;
            ಆದರೆ( !fast_int_ಬಹುಲಕ )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((ಶೂನ್ಯ*)(ds + cn), (ಶೂನ್ಯ*)(parambuf + _parambuf.size()), "Last byte check");
                ಗಾಗಿ( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    ಆದರೆ (ಆಳ != CV_64U && ಆಳ != CV_64S) {
                        ಚಿಹ್ನೆರಹಿತ d = ds[j].d = (ಚಿಹ್ನೆರಹಿತ)(ip[j][0]+1);
                        ಪೂರ್ಣಾಂಕ l = 0;
                        ಆಗ(((uint64)1 << l) < d)
                            l++;
                        ds[j].M = (ಚಿಹ್ನೆರಹಿತ)(((uint64)1 << 32)*(((uint64)1 << l) - d)/d) + 1;
                        ds[j].sh1 = std::min(l, 1);
                        ds[j].sh2 = std::max(l - 1, 0);
                    }
                }
                uni_param = ds;
            }

            func = randTab[fast_int_ಬಹುಲಕ ? 1 : 0][ಆಳ];
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ದ್ವಿಗುಣ scale = ಆಳ == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            ದ್ವಿಗುಣ maxdiff = saturateRange ? (ದ್ವಿಗುಣ)FLT_MAX : DBL_MAX;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            ಆದರೆ( ಆಳ != CV_64F )
            {
                Vec2f* fp = (Vec2f*)(parambuf + cn*2);
                ಗಾಗಿ( j = 0; j < cn; j++ )
                {
                    fp[j][0] = (ದಶಮಾಂಶ)(std::min(maxdiff, p2[j] - p1[j])*scale);
                    fp[j][1] = (ದಶಮಾಂಶ)((p2[j] + p1[j])*0.5);
                }
                uni_param = fp;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                Vec2d* dp = (Vec2d*)(parambuf + cn*2);
                ಗಾಗಿ( j = 0; j < cn; j++ )
                {
                    dp[j][0] = std::min(DBL_MAX, p2[j] - p1[j])*scale;
                    dp[j][1] = ((p2[j] + p1[j])*0.5);
                }
                uni_param = dp;
            }

            func = randTab[0][ಆಳ];
        }
        CV_Assert( func != 0 );
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( disttype == RNG::ಸಾಮಾನ್ಯ_3 )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        ದ್ವಿಗುಣ* parambuf = _parambuf.ದತ್ತಾಂಶ();

        ಪೂರ್ಣಾಂಕ ptype = esz1 == 8 ? CV_64F : CV_32F;

        ಆದರೆ( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            ಸರಾಸರಿ_2 = _param1.ptr();
        ಇಲ್ಲದಿದ್ದರೆ
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convert_ಗಮ್ಯ(tmp, ptype);
            ಸರಾಸರಿ_2 = (uchar*)parambuf;
        }

        ಆದರೆ( n1 < cn )
            ಗಾಗಿ( j = n1*esz1; j < cn*esz1; j++ )
                ಸರಾಸರಿ_2[j] = ಸರಾಸರಿ_2[j - n1*esz1];

        ಆದರೆ( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        ಇಲ್ಲದಿದ್ದರೆ
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convert_ಗಮ್ಯ(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        ಆದರೆ( n2 < cn )
            ಗಾಗಿ( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.ಅಡ್ಡಸಾಲುಗಳು == cn && _param2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == cn;
        scaleFunc = randnScaleTab[ಆಳ];
        CV_Assert( scaleFunc != 0 );
    }
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    ದಶಮಾಂಶ fbuf[ಖಂಡ_ಗಾತ್ರ + CV_CN_MAX];
    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)it.size;
    ಪೂರ್ಣಾಂಕ ಖಂಡ_ಗಾತ್ರ_2 = std::min((ಖಂಡ_ಗಾತ್ರ + cn - 1)/cn, ಒಟ್ಟು);
    size_t esz = (size_t)esz1*cn;
    ಪೂರ್ಣಾಂಕ flags = mat.type();

    ಆದರೆ( disttype == ಏಕರೂಪ_2 )
        flags |= (small_flag ? (ಪೂರ್ಣಾಂಕ)RNG_FLAG_SMALL : 0);
    ಇಲ್ಲದಿದ್ದರೆ
        flags |= (stdmtx ? (ಪೂರ್ಣಾಂಕ)RNG_FLAG_STDMTX : 0);

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಗಾಗಿ( j = 0; j < ಒಟ್ಟು; j += ಖಂಡ_ಗಾತ್ರ_2 )
        {
            ಪೂರ್ಣಾಂಕ len = std::min(ಒಟ್ಟು - j, ಖಂಡ_ಗಾತ್ರ_2);

            ಆದರೆ( disttype == ಏಕರೂಪ_2 )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            ಇಲ್ಲದಿದ್ದರೆ
            {
                randn_0_1_32f(fbuf, len*cn, &state);
                scaleFunc(fbuf, ptr + j*esz, len, cn, ಸರಾಸರಿ_2, stddev, flags);
            }
        }
    }
}

}

cv::RNG& cv::theRNG()
{
    ಹಿಂತಿರುಗಿ get_Core_Tls_ದತ್ತಾಂಶ().rng;
}

ಶೂನ್ಯ cv::ಗಣ_RNGSeed(ಪೂರ್ಣಾಂಕ ಬೀಜ)
{
    theRNG() = RNG(static_cast<uint64>(ಬೀಜ));
}


ಶೂನ್ಯ cv::randu(ಒಳಹರಿವು_Output_Array dst, ಒಳಹರಿವು_Array ಕೆಳಗಿನ, ಒಳಹರಿವು_Array ಮೇಲಿನ)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::ಏಕರೂಪ_2, ಕೆಳಗಿನ, ಮೇಲಿನ);
}

ಶೂನ್ಯ cv::randn(ಒಳಹರಿವು_Output_Array dst, ಒಳಹರಿವು_Array ಸರಾಸರಿ_2, ಒಳಹರಿವು_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::ಸಾಮಾನ್ಯ_3, ಸರಾಸರಿ_2, stddev);
}

ನಾಮಾಕಾಶ cv
{

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
randShuffle_( Mat& _arr, RNG& rng, ದ್ವಿಗುಣ )
{
    ಚಿಹ್ನೆರಹಿತ sz = (ಚಿಹ್ನೆರಹಿತ)_arr.ಒಟ್ಟು();
    ಆದರೆ( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        ಗಾಗಿ( ಚಿಹ್ನೆರಹಿತ i = 0; i < sz; i++ )
        {
            ಚಿಹ್ನೆರಹಿತ j = (ಚಿಹ್ನೆರಹಿತ)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* ದತ್ತಾಂಶ = _arr.ptr();
        size_t ಹೆಜ್ಜೆ = _arr.ಹೆಜ್ಜೆ;
        ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು = _arr.ಅಡ್ಡಸಾಲುಗಳು;
        ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = _arr.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i0 = 0; i0 < ಅಡ್ಡಸಾಲುಗಳು; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ j0 = 0; j0 < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; j0++ )
            {
                ಚಿಹ್ನೆರಹಿತ k1 = (ಚಿಹ್ನೆರಹಿತ)rng % sz;
                ಪೂರ್ಣಾಂಕ i1 = (ಪೂರ್ಣಾಂಕ)(k1 / ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
                ಪೂರ್ಣಾಂಕ j1 = (ಪೂರ್ಣಾಂಕ)(k1 - (ಚಿಹ್ನೆರಹಿತ)i1*(ಚಿಹ್ನೆರಹಿತ)ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
                std::swap( p[j0], ((T*)(ದತ್ತಾಂಶ + ಹೆಜ್ಜೆ*i1))[j1] );
            }
        }
    }
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*RandShuffleFunc)( Mat& dst, RNG& rng, ದ್ವಿಗುಣ iter_ಗುಣಕ );

}

ಶೂನ್ಯ cv::randShuffle( ಒಳಹರಿವು_Output_Array _dst, ದ್ವಿಗುಣ iter_ಗುಣಕ, RNG* _rng )
{
    CV_INSTRUMENT_REGION();

    RandShuffleFunc tab[] =
    {
        0,
        randShuffle_<uchar>, // 1
        randShuffle_<ushort>, // 2
        randShuffle_<Vec<uchar,3> >, // 3
        randShuffle_<ಪೂರ್ಣಾಂಕ>, // 4
        0,
        randShuffle_<Vec<ushort,3> >, // 6
        0,
        randShuffle_<Vec<ಪೂರ್ಣಾಂಕ,2> >, // 8
        0, 0, 0,
        randShuffle_<Vec<ಪೂರ್ಣಾಂಕ,3> >, // 12
        0, 0, 0,
        randShuffle_<Vec<ಪೂರ್ಣಾಂಕ,4> >, // 16
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<ಪೂರ್ಣಾಂಕ,6> >, // 24
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<ಪೂರ್ಣಾಂಕ,8> > // 32
    };

    Mat dst = _dst.getMat();
    RNG& rng = _rng ? *_rng : theRNG();
    CV_Assert( dst.elem_ಗಾತ್ರ() <= 32 );
    RandShuffleFunc func = tab[dst.elem_ಗಾತ್ರ()];
    CV_Assert( func != 0 );
    func( dst, rng, iter_ಗುಣಕ );
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

cv::RNG_MT19937::RNG_MT19937(ಚಿಹ್ನೆರಹಿತ s) { ಬೀಜ(s); }

cv::RNG_MT19937::RNG_MT19937() { ಬೀಜ(5489U); }

ಶೂನ್ಯ cv::RNG_MT19937::ಬೀಜ(ಚಿಹ್ನೆರಹಿತ s)
{
    state[0]= s;
    ಗಾಗಿ (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

ಚಿಹ್ನೆರಹಿತ cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಊರ್ಧ್ವ_ಮುಖವಾಡ = 0x80000000U;
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಧೋ_ಮುಖವಾಡ = 0x7fffffffU;

    /* generate N words at one time */
    ಆದರೆ (mti >= N)
    {
        ಪೂರ್ಣಾಂಕ kk = 0;

        ಗಾಗಿ (; kk < N - M; ++kk)
        {
            ಚಿಹ್ನೆರಹಿತ y = (state[kk] & ಊರ್ಧ್ವ_ಮುಖವಾಡ) | (state[kk + 1] & ಅಧೋ_ಮುಖವಾಡ);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        ಗಾಗಿ (; kk < N - 1; ++kk)
        {
            ಚಿಹ್ನೆರಹಿತ y = (state[kk] & ಊರ್ಧ್ವ_ಮುಖವಾಡ) | (state[kk + 1] & ಅಧೋ_ಮುಖವಾಡ);
            state[kk] = state[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        ಚಿಹ್ನೆರಹಿತ y = (state[N - 1] & ಊರ್ಧ್ವ_ಮುಖವಾಡ) | (state[0] & ಅಧೋ_ಮುಖವಾಡ);
        state[N - 1] = state[M - 1] ^ (y >> 1) ^ mag01[y & 0x1U];

        mti = 0;
    }

    ಚಿಹ್ನೆರಹಿತ y = state[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y <<  7) & 0x9d2c5680U;
    y ^= (y << 15) & 0xefc60000U;
    y ^= (y >> 18);

    ಹಿಂತಿರುಗಿ y;
}

cv::RNG_MT19937::ಕ್ರಿಯಾಕಾರಕ ಚಿಹ್ನೆರಹಿತ() { ಹಿಂತಿರುಗಿ next(); }

cv::RNG_MT19937::ಕ್ರಿಯಾಕಾರಕ ಪೂರ್ಣಾಂಕ() { ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)next();}

cv::RNG_MT19937::ಕ್ರಿಯಾಕಾರಕ ದಶಮಾಂಶ() { ಹಿಂತಿರುಗಿ next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::ಕ್ರಿಯಾಕಾರಕ ದ್ವಿಗುಣ()
{
    ಚಿಹ್ನೆರಹಿತ a = next() >> 5;
    ಚಿಹ್ನೆರಹಿತ b = next() >> 6;
    ಹಿಂತಿರುಗಿ (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

ಪೂರ್ಣಾಂಕ cv::RNG_MT19937::ಏಕರೂಪ(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b) { ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)(next() % (b - a) + a); }

ದಶಮಾಂಶ cv::RNG_MT19937::ಏಕರೂಪ(ದಶಮಾಂಶ a, ದಶಮಾಂಶ b) { ಹಿಂತಿರುಗಿ ((ದಶಮಾಂಶ)*ಇದು)*(b - a) + a; }

ದ್ವಿಗುಣ cv::RNG_MT19937::ಏಕರೂಪ(ದ್ವಿಗುಣ a, ದ್ವಿಗುಣ b) { ಹಿಂತಿರುಗಿ ((ದ್ವಿಗುಣ)*ಇದು)*(b - a) + a; }

ಚಿಹ್ನೆರಹಿತ cv::RNG_MT19937::ಕ್ರಿಯಾಕಾರಕ ()(ಚಿಹ್ನೆರಹಿತ b) { ಹಿಂತಿರುಗಿ next() % b; }

ಚಿಹ್ನೆರಹಿತ cv::RNG_MT19937::ಕ್ರಿಯಾಕಾರಕ ()() { ಹಿಂತಿರುಗಿ next(); }

/* End of file. */
