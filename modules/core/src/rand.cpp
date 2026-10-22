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

#define CV_RNG_COEFF 4164903690U

/*
   Multiply-with-carry generator is used here:
   temp = ( A*X(n) + carry )
   X(n+1) = temp mod (2^32)
   carry = floor (temp / (2^32))
*/

#define  RNG_NEXT(x)    ((uint64)(unsigned)(x)*CV_RNG_COEFF + ((x) >> 32))
// make it jump-less
#define  CN_NEXT(k)     (((k) + 1) & (((k) >= cn) - 1))

ಗಣನಾ
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

template<typename T> ಸ್ಥಿರ ಶೂನ್ಯ
randBits_( T* arr, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, uint64* state, ಧ್ರುವ Vec2l* p, ಪೂರ್ಣಾಙ್ಕ flags )
{
    ತಾರ್ಕಿಕ small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 temp = *state;
    ಪೂರ್ಣಾಙ್ಕ i, k = 0;
    len *= cn;
    --cn;

    ಯದಿ( !small_flag )
    {
        ಪ್ರತಿ( i = 0; i <= len - 4; i += 4 )
        {
            int64_t t0, t1;

            temp = RNG_NEXT(temp);
            t0 = ((int64_t)temp & p[k][0]) + p[k][1];
            k = CN_NEXT(k);
            temp = RNG_NEXT(temp);
            t1 = ((int64_t)temp & p[k][0]) + p[k][1];
            k = CN_NEXT(k);
            arr[i] = saturate_cast<T>(t0);
            arr[i+1] = saturate_cast<T>(t1);

            temp = RNG_NEXT(temp);
            t0 = ((int64_t)temp & p[k][0]) + p[k][1];
            k = CN_NEXT(k);
            temp = RNG_NEXT(temp);
            t1 = ((int64_t)temp & p[k][0]) + p[k][1];
            k = CN_NEXT(k);
            arr[i+2] = saturate_cast<T>(t0);
            arr[i+3] = saturate_cast<T>(t1);
        }
    }
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( i = 0; i <= len - 4; i += 4 )
        {
            int64_t t0, t1, t;
            temp = RNG_NEXT(temp);
            t = temp;
            // p[i+...][0] is within 0..255 in this branch (small_flag==true),
            // so we don't need to do (t>>...)&255,
            // the upper bits will be cleaned with ... & p[i+...][0].
            t0 = (t & p[k][0]) + p[k][1];
            k = CN_NEXT(k);
            t1 = ((t >> 8) & p[k][0]) + p[k][1];
            k = CN_NEXT(k);
            arr[i] = saturate_cast<T>(t0);
            arr[i+1] = saturate_cast<T>(t1);

            t0 = ((t >> 16) & p[k][0]) + p[k][1];
            k = CN_NEXT(k);
            t1 = ((t >> 24) & p[k][0]) + p[k][1];
            k = CN_NEXT(k);
            arr[i+2] = saturate_cast<T>(t0);
            arr[i+3] = saturate_cast<T>(t1);
        }
    }

    ಪ್ರತಿ( ; i < len; i++ )
    {
        int64_t t0;
        temp = RNG_NEXT(temp);

        t0 = ((int64_t)temp & p[k][0]) + p[k][1];
        k = CN_NEXT(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = temp;
}

ಸಂರಚನಾ DivStruct
{
    unsigned d;
    unsigned M;
    ಪೂರ್ಣಾಙ್ಕ sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

template<typename T> ಸ್ಥಿರ ಶೂನ್ಯ
randi_( T* arr, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, uint64* state, ಧ್ರುವ DivStruct* p )
{
    uint64 temp = *state;
    ಪೂರ್ಣಾಙ್ಕ k = 0;
    len *= cn;
    cn--;
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++ )
    {
        temp = RNG_NEXT(temp);
        unsigned t = (unsigned)temp;
        unsigned v = (unsigned)(((uint64)t * p[k].M) >> 32);
        v = (v + ((t - v) >> p[k].sh1)) >> p[k].sh2;
        int64_t res = (int64_t)(t - v*p[k].d) + p[k].delta;
        k = CN_NEXT(k);
        arr[i] = saturate_cast<T>(res);
    }
    *state = temp;
}

ಸ್ಥಿರ ಶೂನ್ಯ
randi_( int64_t* arr, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, uint64* state, ಧ್ರುವ DivStruct* p )
{
    uint64 temp = *state;
    ಪೂರ್ಣಾಙ್ಕ k = 0;
    len *= cn;
    cn--;
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++ )
    {
        temp = RNG_NEXT(temp);
        unsigned t0 = (unsigned)temp;
        temp = RNG_NEXT(temp);
        unsigned t1 = (unsigned)temp;
        int64_t t = (int64_t)((((uint64_t)t0 << 32) | t1) % p[k].diff) + p[k].delta;
        k = CN_NEXT(k);
        arr[i] = t;
    }
    *state = temp;
}

ಸ್ಥಿರ ಶೂನ್ಯ
randi_( uint64_t* arr, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, uint64* state, ಧ್ರುವ DivStruct* p )
{
    uint64 temp = *state;
    ಪೂರ್ಣಾಙ್ಕ k = 0;
    len *= cn;
    cn--;
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++ )
    {
        temp = RNG_NEXT(temp);
        unsigned t0 = (unsigned)temp;
        temp = RNG_NEXT(temp);
        unsigned t1 = (unsigned)temp;
        uint64_t t = (((uint64_t)t0 << 32) | t1) % p[k].diff;
        int64_t delta = p[k].delta;
        k = CN_NEXT(k);
        arr[i] = delta >= 0 || t >= (uint64_t)-delta ? t + (uint64_t)delta : 0;
    }
    *state = temp;
}

template<typename T>
ಸ್ಥಿರ ಶೂನ್ಯ randBitsWrap(ಶೂನ್ಯ* arr, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, uint64* state, ಧ್ರುವ ಶೂನ್ಯ* p, ಶೂನ್ಯ*, ಪೂರ್ಣಾಙ್ಕ flags)
{
    randBits_((T*)arr, len, cn, state, (ಧ್ರುವ Vec2l*)p, flags);
}

template<typename T>
ಸ್ಥಿರ ಶೂನ್ಯ randiWrap(ಶೂನ್ಯ* arr, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, uint64* state, ಧ್ರುವ ಶೂನ್ಯ* p, ಶೂನ್ಯ*, ಪೂರ್ಣಾಙ್ಕ)
{
    randi_((T*)arr, len, cn, state, (ಧ್ರುವ DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ cvt32fToFP8(ಧ್ರುವ ಭಿನ್ನ* src, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಯದಿ (depth == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    ಅನ್ಯಥಾ                              { fp8a_t* d = (fp8a_t*)dst; ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isFP8Depth(ಪೂರ್ಣಾಙ್ಕ d) { ಪ್ರತ್ಯಾಗಮ d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

ಸ್ಥಿರ ಶೂನ್ಯ randf_16_or_32f( ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಙ್ಕ len_, ಪೂರ್ಣಾಙ್ಕ cn, uint64* state, ಧ್ರುವ ಶೂನ್ಯ* _p, ಶೂನ್ಯ* _fbuf, ಪೂರ್ಣಾಙ್ಕ flags )
{
    ಧ್ರುವ Vec2f* p = (ಧ್ರುವ Vec2f*)_p;
    ಭಿನ್ನ* fbuf = (ಭಿನ್ನ*)_fbuf;
    ಪೂರ್ಣಾಙ್ಕ depth = CV_MAT_DEPTH(flags);
    uint64 temp = *state;
    ಪೂರ್ಣಾಙ್ಕ k = 0, len = len_*cn;
    ಭಿನ್ನ* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : (ಭಿನ್ನ*)dst;
    cn--;
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++ )
    {
        ಪೂರ್ಣಾಙ್ಕ t = (ಪೂರ್ಣಾಙ್ಕ)(temp = RNG_NEXT(temp));
        arr[i] = (ಭಿನ್ನ)(t*p[k][0]);
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias32f(arr, &p[0][0], len_, cn+1);
    ಯದಿ (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    ಅನ್ಯಥಾ ಯದಿ (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    ಅನ್ಯಥಾ ಯದಿ (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

ಸ್ಥಿರ ಶೂನ್ಯ
randf_64f( ಶೂನ್ಯ* _arr, ಪೂರ್ಣಾಙ್ಕ len_, ಪೂರ್ಣಾಙ್ಕ cn, uint64* state, ಧ್ರುವ ಶೂನ್ಯ* _p, ಶೂನ್ಯ*, ಪೂರ್ಣಾಙ್ಕ )
{
    ದ್ವಿಗುಣ* arr = (ದ್ವಿಗುಣ*)_arr;
    ಧ್ರುವ Vec2d* p = (ಧ್ರುವ Vec2d*)_p;
    uint64 temp = *state;
    ಪೂರ್ಣಾಙ್ಕ k = 0, len = len_*cn;
    cn--;
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++ )
    {
        temp = RNG_NEXT(temp);
        int64_t v = (int64_t)((temp >> 32) | (temp << 32));
        arr[i] = v*p[k][0];
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias64f(arr, &p[0][0], len_, cn+1);
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*RandFunc)(ಶೂನ್ಯ* arr, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, uint64* state,
                         ಧ್ರುವ ಶೂನ್ಯ* p, ಶೂನ್ಯ* tempbuf, ಪೂರ್ಣಾಙ್ಕ flags);

ಸ್ಥಿರ RandFunc randTab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<short>, randiWrap<ಪೂರ್ಣಾಙ್ಕ>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<ತಾರ್ಕಿಕ>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<unsigned>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        randBitsWrap<uchar>, randBitsWrap<schar>, randBitsWrap<ushort>,
        randBitsWrap<short>, randBitsWrap<ಪೂರ್ಣಾಙ್ಕ>, 0, 0, 0, 0,
        randBitsWrap<ತಾರ್ಕಿಕ>, randBitsWrap<uint64_t>, randBitsWrap<int64_t>,
        randBitsWrap<unsigned>, 0, 0, 0
    }
};

/*
   The code below implements the algorithm described in
   "The Ziggurat Method for Generating Random Variables"
   by George Marsaglia and Wai Wan Tsang, Journal of Statistical Software, 2007.
*/
ಸ್ಥಿರ ಶೂನ್ಯ
randn_0_1_32f( ಭಿನ್ನ* arr, ಪೂರ್ಣಾಙ್ಕ len, uint64* state )
{
    ಧ್ರುವ ಭಿನ್ನ r = 3.442620f; // The start of the right tail
    ಧ್ರುವ ಭಿನ್ನ rng_flt = 2.3283064365386962890625e-10f; // 2^-32
    ಸ್ಥಿರ unsigned kn[128];
    ಸ್ಥಿರ ಭಿನ್ನ wn[128], fn[128];
    uint64 temp = *state;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized=ಅಸತ್ಯ;
    ಪೂರ್ಣಾಙ್ಕ i;

    ಯದಿ( !initialized )
    {
        ಧ್ರುವ ದ್ವಿಗುಣ m1 = 2147483648.0;
        ದ್ವಿಗುಣ dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        ದ್ವಿಗುಣ q = vn/std::exp(-.5*dn*dn);
        kn[0] = (unsigned)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (ಭಿನ್ನ)(q/m1);
        wn[127] = (ಭಿನ್ನ)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (ಭಿನ್ನ)std::exp(-.5*dn*dn);

        ಪ್ರತಿ(i=126;i>=1;i--)
        {
            dn = std::sqrt(-2.*std::log(vn/dn+std::exp(-.5*dn*dn)));
            kn[i+1] = (unsigned)((dn/tn)*m1);
            tn = dn;
            fn[i] = (ಭಿನ್ನ)std::exp(-.5*dn*dn);
            wn[i] = (ಭಿನ್ನ)(dn/m1);
        }
        initialized = ಸತ್ಯ;
    }

    ಪ್ರತಿ( i = 0; i < len; i++ )
    {
        ಭಿನ್ನ x, y;
        ಪ್ರತಿ(;;)
        {
            ಪೂರ್ಣಾಙ್ಕ hz = (ಪೂರ್ಣಾಙ್ಕ)temp;
            temp = RNG_NEXT(temp);
            ಪೂರ್ಣಾಙ್ಕ iz = hz & 127;
            x = hz*wn[iz];
            ಯದಿ( (unsigned)std::abs(hz) < kn[iz] )
                ಭಙ್ಗ;
            ಯದಿ( iz == 0) // iz==0, handles the base strip
            {
                ಕುರು
                {
                    x = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    y = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    x = (ಭಿನ್ನ)(-std::log(x+FLT_MIN)*0.2904764);
                    y = (ಭಿನ್ನ)-std::log(y+FLT_MIN);
                }	// .2904764 is 1/r
                ಯಾವತ್( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                ಭಙ್ಗ;
            }
            // iz > 0, handle the wedges of other strips
            y = (unsigned)temp*rng_flt;
            temp = RNG_NEXT(temp);
            ಯದಿ( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::exp(-.5*x*x) )
                ಭಙ್ಗ;
        }
        arr[i] = x;
    }
    *state = temp;
}


ದ್ವಿಗುಣ RNG::gaussian(ದ್ವಿಗುಣ sigma)
{
    ಭಿನ್ನ temp;
    randn_0_1_32f( &temp, 1, &state );
    ಪ್ರತ್ಯಾಗಮ temp*sigma;
}

template<typename T, typename PT> ಸ್ಥಿರ ಶೂನ್ಯ
randnScale_(ಭಿನ್ನ* src, ಶೂನ್ಯ* _dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn,
            ಧ್ರುವ ಶೂನ್ಯ* _mean, ಧ್ರುವ ಶೂನ್ಯ* _stddev, ಪೂರ್ಣಾಙ್ಕ flags )
{
    T* dst = (T*)_dst;
    ಧ್ರುವ PT* mean = (ಧ್ರುವ PT*)_mean;
    ಧ್ರುವ PT* stddev = (ಧ್ರುವ PT*)_stddev;
    ತಾರ್ಕಿಕ stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    ಪೂರ್ಣಾಙ್ಕ i, j, k;
    ಯದಿ( !stdmtx || cn == 1 )
    {
        ಯದಿ( cn == 1 )
        {
            PT a = stddev[0], b = mean[0];
            ಪ್ರತಿ( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        ಅನ್ಯಥಾ
        {
            len *= cn;
            cn--;
            ಪ್ರತಿ( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + mean[k]);
                k = CN_NEXT(k);
            }
        }
    }
    ಅನ್ಯಥಾ
    {
        len *= cn;
        cn--;
        ಪ್ರತಿ( i = j = 0; i < len; i++ )
        {
            PT s = mean[j];
            ಪೂರ್ಣಾಙ್ಕ i0 = i - j;
            ಪ್ರತಿ( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_NEXT(j);
        }
    }
}

// special version for 16f, 16bf and 32f
ಸ್ಥಿರ ಶೂನ್ಯ
randnScale_16_or_32f(ಭಿನ್ನ* fbuf, ಶೂನ್ಯ* _dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn,
                     ಧ್ರುವ ಶೂನ್ಯ* _mean, ಧ್ರುವ ಶೂನ್ಯ* _stddev, ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಭಿನ್ನ* dst = (ಭಿನ್ನ*)_dst;
    ಧ್ರುವ ಭಿನ್ನ* mean = (ಧ್ರುವ ಭಿನ್ನ*)_mean;
    ಧ್ರುವ ಭಿನ್ನ* stddev = (ಧ್ರುವ ಭಿನ್ನ*)_stddev;
    ತಾರ್ಕಿಕ stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    ಪೂರ್ಣಾಙ್ಕ depth = CV_MAT_DEPTH(flags);
    ಭಿನ್ನ* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : dst;
    ಪೂರ್ಣಾಙ್ಕ i, j, k;

    ಯದಿ( !stdmtx || cn == 1 )
    {
        ಯದಿ( cn == 1 )
        {
            ಭಿನ್ನ a = stddev[0], b = mean[0];
            ಪ್ರತಿ( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        ಅನ್ಯಥಾ
        {
            len *= cn;
            cn--;
            ಪ್ರತಿ( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + mean[k];
                k = CN_NEXT(k);
            }
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( depth == CV_32F )
    {
        len *= cn;
        cn--;
        ಪ್ರತಿ( i = j = 0; i < len; i++ )
        {
            ಭಿನ್ನ s = mean[j];
            ಪೂರ್ಣಾಙ್ಕ i0 = i - j;
            ಪ್ರತಿ( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_NEXT(j);
        }
    }
    ಅನ್ಯಥಾ
    {
        ಭಿನ್ನ elembuf[CV_CN_MAX];
        len *= cn;
        ಪ್ರತಿ( i = 0; i < len; i += cn )
        {
            // since we process fbuf in-place,
            // we need to copy each cn-channel element
            // prior to matrix multiplication
            ಪ್ರತಿ (j = 0; j < cn; j++)
                elembuf[j] = fbuf[i + j];
            ಪ್ರತಿ (j = 0; j < cn; j++) {
                ಭಿನ್ನ s = mean[j];
                ಪ್ರತಿ( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    ಯದಿ (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    ಅನ್ಯಥಾ ಯದಿ (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    ಅನ್ಯಥಾ ಯದಿ (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*RandnScaleFunc)(ಭಿನ್ನ* src, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn,
                               ಧ್ರುವ ಶೂನ್ಯ* mean, ಧ್ರುವ ಶೂನ್ಯ* stddev, ಪೂರ್ಣಾಙ್ಕ flags);

ಸ್ಥಿರ RandnScaleFunc randnScaleTab[CV_DEPTH_MAX] =
{
    randnScale_<uchar, ಭಿನ್ನ>, randnScale_<schar, ಭಿನ್ನ>, randnScale_<ushort, ಭಿನ್ನ>,
    randnScale_<short, ಭಿನ್ನ>, randnScale_<ಪೂರ್ಣಾಙ್ಕ, ಭಿನ್ನ>, randnScale_16_or_32f,
    randnScale_<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<ತಾರ್ಕಿಕ, ಭಿನ್ನ>, randnScale_<uint64_t, ದ್ವಿಗುಣ>, randnScale_<int64_t, ದ್ವಿಗುಣ>,
    randnScale_<unsigned, ಭಿನ್ನ>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

ಶೂನ್ಯ RNG::fill( ಒಳಹರಿವು_Output_Array _mat, ಪೂರ್ಣಾಙ್ಕ disttype,
                ಒಳಹರಿವು_Array _param1arg, ಒಳಹರಿವು_Array _param2arg,
                ತಾರ್ಕಿಕ saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    ಪೂರ್ಣಾಙ್ಕ j, depth = mat.depth(), cn = mat.channels();
    ಪೂರ್ಣಾಙ್ಕ esz1 = CV_ELEM_SIZE(depth);
    AutoBuffer<ದ್ವಿಗುಣ> _parambuf;
    ತಾರ್ಕಿಕ fast_int_mode = ಅಸತ್ಯ;
    ತಾರ್ಕಿಕ small_flag = ಅಸತ್ಯ;
    RandFunc func = 0;
    RandnScaleFunc scaleFunc = 0;

    CV_Assert(_param1.channels() == 1 && (_param1.rows == 1 || _param1.cols == 1) &&
              (_param1.rows + _param1.cols - 1 == cn || _param1.rows + _param1.cols - 1 == 1 ||
               (_param1.size() == Size(1, 4) && _param1.type() == CV_64F && cn <= 4)));
    CV_Assert( _param2.channels() == 1 &&
               (((_param2.rows == 1 || _param2.cols == 1) &&
                (_param2.rows + _param2.cols - 1 == cn || _param2.rows + _param2.cols - 1 == 1 ||
                (_param1.size() == Size(1, 4) && _param1.type() == CV_64F && cn <= 4))) ||
                (_param2.rows == cn && _param2.cols == cn && disttype == NORMAL)));

    ಧ್ರುವ ಶೂನ್ಯ* uni_param = 0;
    uchar* mean = 0;
    uchar* stddev = 0;
    ತಾರ್ಕಿಕ stdmtx = ಅಸತ್ಯ;
    ಪೂರ್ಣಾಙ್ಕ n1 = (ಪೂರ್ಣಾಙ್ಕ)_param1.total();
    ಪೂರ್ಣಾಙ್ಕ n2 = (ಪೂರ್ಣಾಙ್ಕ)_param2.total();

    ಯದಿ( disttype == UNIFORM )
    {
        _parambuf.allocate(cn*(ಪರಿಮಾಣ(DivStruct)+ಪರಿಮಾಣ(ದ್ವಿಗುಣ)-1)/ಪರಿಮಾಣ(ದ್ವಿಗುಣ) + cn*4);
        ದ್ವಿಗುಣ* parambuf = _parambuf.data();
        ದ್ವಿಗುಣ* p1 = _param1.ptr<ದ್ವಿಗುಣ>();
        ದ್ವಿಗುಣ* p2 = _param2.ptr<ದ್ವಿಗುಣ>();

        ಯದಿ( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convertTo(tmp, CV_64F);
            ಪ್ರತಿ( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        ಯದಿ( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convertTo(tmp, CV_64F);
            ಪ್ರತಿ( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        ಯದಿ( CV_IS_INT_TYPE(depth) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            ಪ್ರತಿ( j = 0, fast_int_mode = ಸತ್ಯ; j < cn; j++ )
            {
                ದ್ವಿಗುಣ a = std::min(p1[j], p2[j]);
                ದ್ವಿಗುಣ b = std::max(p1[j], p2[j]);
                ಯದಿ( saturateRange )
                {
                    a = std::max(a, depth == CV_8U || depth == CV_16U || depth == CV_32U ||
                                 depth == CV_64U || depth == CV_Bool ? 0. :
                                 depth == CV_8S ? -128. : depth == CV_16S ? -32768. :
                                 depth == CV_32S ? (ದ್ವಿಗುಣ)INT_MIN : (ದ್ವಿಗುಣ)INT64_MIN);
                    b = std::min(b, depth == CV_8U ? 256. : depth == CV_Bool ? 2. : depth == CV_16U ? 65536. :
                                 depth == CV_8S ? 128. : depth == CV_16S ? 32768. : depth == CV_32U ? (ದ್ವಿಗುಣ)UINT_MAX :
                                 depth == CV_32S ? (ದ್ವಿಗುಣ)INT_MAX : (ದ್ವಿಗುಣ)INT64_MAX);
                }
                ip[j][1] = (int64_t)ceil(a);
                int64_t idiff = ip[j][0] = (int64_t)floor(b) - ip[j][1] - 1;
                ಯದಿ (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                ದ್ವಿಗುಣ diff = b - a;

                fast_int_mode = fast_int_mode && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                ಯದಿ( fast_int_mode )
                    small_flag = idiff <= 255;
                ಅನ್ಯಥಾ
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    ಯದಿ (depth == CV_64S || depth == CV_64U)
                    {
                        minval = INT64_MIN/2;
                        maxval = INT64_MAX;
                    }
                    ಯದಿ( diff > (ದ್ವಿಗುಣ)maxval )
                        ip[j][0] = maxval;
                    ಯದಿ( a < (ದ್ವಿಗುಣ)minval )
                        ip[j][1] = minval;
                }
            }

            uni_param = ip;
            ಯದಿ( !fast_int_mode )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((ಶೂನ್ಯ*)(ds + cn), (ಶೂನ್ಯ*)(parambuf + _parambuf.size()), "Last byte check");
                ಪ್ರತಿ( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    ಯದಿ (depth != CV_64U && depth != CV_64S) {
                        unsigned d = ds[j].d = (unsigned)(ip[j][0]+1);
                        ಪೂರ್ಣಾಙ್ಕ l = 0;
                        ಯಾವತ್(((uint64)1 << l) < d)
                            l++;
                        ds[j].M = (unsigned)(((uint64)1 << 32)*(((uint64)1 << l) - d)/d) + 1;
                        ds[j].sh1 = std::min(l, 1);
                        ds[j].sh2 = std::max(l - 1, 0);
                    }
                }
                uni_param = ds;
            }

            func = randTab[fast_int_mode ? 1 : 0][depth];
        }
        ಅನ್ಯಥಾ
        {
            ದ್ವಿಗುಣ scale = depth == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            ದ್ವಿಗುಣ maxdiff = saturateRange ? (ದ್ವಿಗುಣ)FLT_MAX : DBL_MAX;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            ಯದಿ( depth != CV_64F )
            {
                Vec2f* fp = (Vec2f*)(parambuf + cn*2);
                ಪ್ರತಿ( j = 0; j < cn; j++ )
                {
                    fp[j][0] = (ಭಿನ್ನ)(std::min(maxdiff, p2[j] - p1[j])*scale);
                    fp[j][1] = (ಭಿನ್ನ)((p2[j] + p1[j])*0.5);
                }
                uni_param = fp;
            }
            ಅನ್ಯಥಾ
            {
                Vec2d* dp = (Vec2d*)(parambuf + cn*2);
                ಪ್ರತಿ( j = 0; j < cn; j++ )
                {
                    dp[j][0] = std::min(DBL_MAX, p2[j] - p1[j])*scale;
                    dp[j][1] = ((p2[j] + p1[j])*0.5);
                }
                uni_param = dp;
            }

            func = randTab[0][depth];
        }
        CV_Assert( func != 0 );
    }
    ಅನ್ಯಥಾ ಯದಿ( disttype == RNG::NORMAL )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        ದ್ವಿಗುಣ* parambuf = _parambuf.data();

        ಪೂರ್ಣಾಙ್ಕ ptype = esz1 == 8 ? CV_64F : CV_32F;

        ಯದಿ( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            mean = _param1.ptr();
        ಅನ್ಯಥಾ
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convertTo(tmp, ptype);
            mean = (uchar*)parambuf;
        }

        ಯದಿ( n1 < cn )
            ಪ್ರತಿ( j = n1*esz1; j < cn*esz1; j++ )
                mean[j] = mean[j - n1*esz1];

        ಯದಿ( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        ಅನ್ಯಥಾ
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convertTo(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        ಯದಿ( n2 < cn )
            ಪ್ರತಿ( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.rows == cn && _param2.cols == cn;
        scaleFunc = randnScaleTab[depth];
        CV_Assert( scaleFunc != 0 );
    }
    ಅನ್ಯಥಾ
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    ಧ್ರುವ Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    ಭಿನ್ನ fbuf[BLOCK_SIZE + CV_CN_MAX];
    ಪೂರ್ಣಾಙ್ಕ total = (ಪೂರ್ಣಾಙ್ಕ)it.size;
    ಪೂರ್ಣಾಙ್ಕ blockSize = std::min((BLOCK_SIZE + cn - 1)/cn, total);
    size_t esz = (size_t)esz1*cn;
    ಪೂರ್ಣಾಙ್ಕ flags = mat.type();

    ಯದಿ( disttype == UNIFORM )
        flags |= (small_flag ? (ಪೂರ್ಣಾಙ್ಕ)RNG_FLAG_SMALL : 0);
    ಅನ್ಯಥಾ
        flags |= (stdmtx ? (ಪೂರ್ಣಾಙ್ಕ)RNG_FLAG_STDMTX : 0);

    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಪ್ರತಿ( j = 0; j < total; j += blockSize )
        {
            ಪೂರ್ಣಾಙ್ಕ len = std::min(total - j, blockSize);

            ಯದಿ( disttype == UNIFORM )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            ಅನ್ಯಥಾ
            {
                randn_0_1_32f(fbuf, len*cn, &state);
                scaleFunc(fbuf, ptr + j*esz, len, cn, mean, stddev, flags);
            }
        }
    }
}

}

cv::RNG& cv::theRNG()
{
    ಪ್ರತ್ಯಾಗಮ getCoreTlsData().rng;
}

ಶೂನ್ಯ cv::setRNGSeed(ಪೂರ್ಣಾಙ್ಕ seed)
{
    theRNG() = RNG(static_cast<uint64>(seed));
}


ಶೂನ್ಯ cv::randu(ಒಳಹರಿವು_Output_Array dst, ಒಳಹರಿವು_Array ಕೆಳಗಿನ, ಒಳಹರಿವು_Array ಮೇಲಿನ)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::UNIFORM, ಕೆಳಗಿನ, ಮೇಲಿನ);
}

ಶೂನ್ಯ cv::randn(ಒಳಹರಿವು_Output_Array dst, ಒಳಹರಿವು_Array mean, ಒಳಹರಿವು_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::NORMAL, mean, stddev);
}

ನಾಮಾಕಾಶ cv
{

template<typename T> ಸ್ಥಿರ ಶೂನ್ಯ
randShuffle_( Mat& _arr, RNG& rng, ದ್ವಿಗುಣ )
{
    unsigned sz = (unsigned)_arr.total();
    ಯದಿ( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        ಪ್ರತಿ( unsigned i = 0; i < sz; i++ )
        {
            unsigned j = (unsigned)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    ಅನ್ಯಥಾ
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* data = _arr.ptr();
        size_t step = _arr.step;
        ಪೂರ್ಣಾಙ್ಕ rows = _arr.rows;
        ಪೂರ್ಣಾಙ್ಕ cols = _arr.cols;
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i0 = 0; i0 < rows; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ j0 = 0; j0 < cols; j0++ )
            {
                unsigned k1 = (unsigned)rng % sz;
                ಪೂರ್ಣಾಙ್ಕ i1 = (ಪೂರ್ಣಾಙ್ಕ)(k1 / cols);
                ಪೂರ್ಣಾಙ್ಕ j1 = (ಪೂರ್ಣಾಙ್ಕ)(k1 - (unsigned)i1*(unsigned)cols);
                std::swap( p[j0], ((T*)(data + step*i1))[j1] );
            }
        }
    }
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*RandShuffleFunc)( Mat& dst, RNG& rng, ದ್ವಿಗುಣ iterFactor );

}

ಶೂನ್ಯ cv::randShuffle( ಒಳಹರಿವು_Output_Array _dst, ದ್ವಿಗುಣ iterFactor, RNG* _rng )
{
    CV_INSTRUMENT_REGION();

    RandShuffleFunc tab[] =
    {
        0,
        randShuffle_<uchar>, // 1
        randShuffle_<ushort>, // 2
        randShuffle_<Vec<uchar,3> >, // 3
        randShuffle_<ಪೂರ್ಣಾಙ್ಕ>, // 4
        0,
        randShuffle_<Vec<ushort,3> >, // 6
        0,
        randShuffle_<Vec<ಪೂರ್ಣಾಙ್ಕ,2> >, // 8
        0, 0, 0,
        randShuffle_<Vec<ಪೂರ್ಣಾಙ್ಕ,3> >, // 12
        0, 0, 0,
        randShuffle_<Vec<ಪೂರ್ಣಾಙ್ಕ,4> >, // 16
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<ಪೂರ್ಣಾಙ್ಕ,6> >, // 24
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<ಪೂರ್ಣಾಙ್ಕ,8> > // 32
    };

    Mat dst = _dst.getMat();
    RNG& rng = _rng ? *_rng : theRNG();
    CV_Assert( dst.elemSize() <= 32 );
    RandShuffleFunc func = tab[dst.elemSize()];
    CV_Assert( func != 0 );
    func( dst, rng, iterFactor );
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

cv::RNG_MT19937::RNG_MT19937(unsigned s) { seed(s); }

cv::RNG_MT19937::RNG_MT19937() { seed(5489U); }

ಶೂನ್ಯ cv::RNG_MT19937::seed(unsigned s)
{
    state[0]= s;
    ಪ್ರತಿ (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

unsigned cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    ಸ್ಥಿರ unsigned mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    ಧ್ರುವ unsigned UPPER_MASK = 0x80000000U;
    ಧ್ರುವ unsigned LOWER_MASK = 0x7fffffffU;

    /* generate N words at one time */
    ಯದಿ (mti >= N)
    {
        ಪೂರ್ಣಾಙ್ಕ kk = 0;

        ಪ್ರತಿ (; kk < N - M; ++kk)
        {
            unsigned y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        ಪ್ರತಿ (; kk < N - 1; ++kk)
        {
            unsigned y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
            state[kk] = state[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        unsigned y = (state[N - 1] & UPPER_MASK) | (state[0] & LOWER_MASK);
        state[N - 1] = state[M - 1] ^ (y >> 1) ^ mag01[y & 0x1U];

        mti = 0;
    }

    unsigned y = state[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y <<  7) & 0x9d2c5680U;
    y ^= (y << 15) & 0xefc60000U;
    y ^= (y >> 18);

    ಪ್ರತ್ಯಾಗಮ y;
}

cv::RNG_MT19937::operator unsigned() { ಪ್ರತ್ಯಾಗಮ next(); }

cv::RNG_MT19937::operator ಪೂರ್ಣಾಙ್ಕ() { ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)next();}

cv::RNG_MT19937::operator ಭಿನ್ನ() { ಪ್ರತ್ಯಾಗಮ next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::operator ದ್ವಿಗುಣ()
{
    unsigned a = next() >> 5;
    unsigned b = next() >> 6;
    ಪ್ರತ್ಯಾಗಮ (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

ಪೂರ್ಣಾಙ್ಕ cv::RNG_MT19937::uniform(ಪೂರ್ಣಾಙ್ಕ a, ಪೂರ್ಣಾಙ್ಕ b) { ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)(next() % (b - a) + a); }

ಭಿನ್ನ cv::RNG_MT19937::uniform(ಭಿನ್ನ a, ಭಿನ್ನ b) { ಪ್ರತ್ಯಾಗಮ ((ಭಿನ್ನ)*ಏತತ್)*(b - a) + a; }

ದ್ವಿಗುಣ cv::RNG_MT19937::uniform(ದ್ವಿಗುಣ a, ದ್ವಿಗುಣ b) { ಪ್ರತ್ಯಾಗಮ ((ದ್ವಿಗುಣ)*ಏತತ್)*(b - a) + a; }

unsigned cv::RNG_MT19937::operator ()(unsigned b) { ಪ್ರತ್ಯಾಗಮ next() % b; }

unsigned cv::RNG_MT19937::operator ()() { ಪ್ರತ್ಯಾಗಮ next(); }

/* End of file. */
