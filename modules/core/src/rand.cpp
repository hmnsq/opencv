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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

ନାମାକାଶ cv
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

ଗଣନା
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

template<typename T> ସ୍ଥିର ଶୂନ୍ୟ
randBits_( T* arr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, uint64* state, ଅଚଳ Vec2l* p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    ତାର୍କିକ small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 temp = *state;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, k = 0;
    len *= cn;
    --cn;

    ଯଦି( !small_flag )
    {
        ପାଇଁ( i = 0; i <= len - 4; i += 4 )
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
    ଅନ୍ୟଥା
    {
        ପାଇଁ( i = 0; i <= len - 4; i += 4 )
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

    ପାଇଁ( ; i < len; i++ )
    {
        int64_t t0;
        temp = RNG_NEXT(temp);

        t0 = ((int64_t)temp & p[k][0]) + p[k][1];
        k = CN_NEXT(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = temp;
}

ସଂରଚନା DivStruct
{
    unsigned d;
    unsigned M;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

template<typename T> ସ୍ଥିର ଶୂନ୍ୟ
randi_( T* arr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, uint64* state, ଅଚଳ DivStruct* p )
{
    uint64 temp = *state;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
    len *= cn;
    cn--;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++ )
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

ସ୍ଥିର ଶୂନ୍ୟ
randi_( int64_t* arr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, uint64* state, ଅଚଳ DivStruct* p )
{
    uint64 temp = *state;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
    len *= cn;
    cn--;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++ )
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

ସ୍ଥିର ଶୂନ୍ୟ
randi_( uint64_t* arr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, uint64* state, ଅଚଳ DivStruct* p )
{
    uint64 temp = *state;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0;
    len *= cn;
    cn--;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++ )
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
ସ୍ଥିର ଶୂନ୍ୟ randBitsWrap(ଶୂନ୍ୟ* arr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, uint64* state, ଅଚଳ ଶୂନ୍ୟ* p, ଶୂନ୍ୟ*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    randBits_((T*)arr, len, cn, state, (ଅଚଳ Vec2l*)p, flags);
}

template<typename T>
ସ୍ଥିର ଶୂନ୍ୟ randiWrap(ଶୂନ୍ୟ* arr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, uint64* state, ଅଚଳ ଶୂନ୍ୟ* p, ଶୂନ୍ୟ*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
{
    randi_((T*)arr, len, cn, state, (ଅଚଳ DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ cvt32fToFP8(ଅଚଳ ଦଶମିକ* src, ଶୂନ୍ୟ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ଯଦି (depth == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    ଅନ୍ୟଥା                              { fp8a_t* d = (fp8a_t*)dst; ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isFP8Depth(ପୂର୍ଣ୍ଣସଂଖ୍ୟା d) { ଫେରନ୍ତୁ d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

ସ୍ଥିର ଶୂନ୍ୟ randf_16_or_32f( ଶୂନ୍ୟ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, uint64* state, ଅଚଳ ଶୂନ୍ୟ* _p, ଶୂନ୍ୟ* _fbuf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    ଅଚଳ Vec2f* p = (ଅଚଳ Vec2f*)_p;
    ଦଶମିକ* fbuf = (ଦଶମିକ*)_fbuf;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = CV_MAT_DEPTH(flags);
    uint64 temp = *state;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0, len = len_*cn;
    ଦଶମିକ* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : (ଦଶମିକ*)dst;
    cn--;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++ )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(temp = RNG_NEXT(temp));
        arr[i] = (ଦଶମିକ)(t*p[k][0]);
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias32f(arr, &p[0][0], len_, cn+1);
    ଯଦି (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    ଅନ୍ୟଥା ଯଦି (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    ଅନ୍ୟଥା ଯଦି (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

ସ୍ଥିର ଶୂନ୍ୟ
randf_64f( ଶୂନ୍ୟ* _arr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, uint64* state, ଅଚଳ ଶୂନ୍ୟ* _p, ଶୂନ୍ୟ*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା )
{
    ଦ୍ୱିଗୁଣ* arr = (ଦ୍ୱିଗୁଣ*)_arr;
    ଅଚଳ Vec2d* p = (ଅଚଳ Vec2d*)_p;
    uint64 temp = *state;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0, len = len_*cn;
    cn--;
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++ )
    {
        temp = RNG_NEXT(temp);
        int64_t v = (int64_t)((temp >> 32) | (temp << 32));
        arr[i] = v*p[k][0];
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias64f(arr, &p[0][0], len_, cn+1);
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*RandFunc)(ଶୂନ୍ୟ* arr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, uint64* state,
                         ଅଚଳ ଶୂନ୍ୟ* p, ଶୂନ୍ୟ* tempbuf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);

ସ୍ଥିର RandFunc randTab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<short>, randiWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<ତାର୍କିକ>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<unsigned>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        randBitsWrap<uchar>, randBitsWrap<schar>, randBitsWrap<ushort>,
        randBitsWrap<short>, randBitsWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, 0, 0, 0, 0,
        randBitsWrap<ତାର୍କିକ>, randBitsWrap<uint64_t>, randBitsWrap<int64_t>,
        randBitsWrap<unsigned>, 0, 0, 0
    }
};

/*
   The code below implements the algorithm described in
   "The Ziggurat Method for Generating Random Variables"
   by George Marsaglia and Wai Wan Tsang, Journal of Statistical Software, 2007.
*/
ସ୍ଥିର ଶୂନ୍ୟ
randn_0_1_32f( ଦଶମିକ* arr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, uint64* state )
{
    ଅଚଳ ଦଶମିକ r = 3.442620f; // The start of the right tail
    ଅଚଳ ଦଶମିକ rng_flt = 2.3283064365386962890625e-10f; // 2^-32
    ସ୍ଥିର unsigned kn[128];
    ସ୍ଥିର ଦଶମିକ wn[128], fn[128];
    uint64 temp = *state;
    ସ୍ଥିର ତାର୍କିକ initialized=ମିଥ୍ୟା;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

    ଯଦି( !initialized )
    {
        ଅଚଳ ଦ୍ୱିଗୁଣ m1 = 2147483648.0;
        ଦ୍ୱିଗୁଣ dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        ଦ୍ୱିଗୁଣ q = vn/std::exp(-.5*dn*dn);
        kn[0] = (unsigned)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (ଦଶମିକ)(q/m1);
        wn[127] = (ଦଶମିକ)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (ଦଶମିକ)std::exp(-.5*dn*dn);

        ପାଇଁ(i=126;i>=1;i--)
        {
            dn = std::sqrt(-2.*std::log(vn/dn+std::exp(-.5*dn*dn)));
            kn[i+1] = (unsigned)((dn/tn)*m1);
            tn = dn;
            fn[i] = (ଦଶମିକ)std::exp(-.5*dn*dn);
            wn[i] = (ଦଶମିକ)(dn/m1);
        }
        initialized = ସତ୍ୟ;
    }

    ପାଇଁ( i = 0; i < len; i++ )
    {
        ଦଶମିକ x, y;
        ପାଇଁ(;;)
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା hz = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)temp;
            temp = RNG_NEXT(temp);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା iz = hz & 127;
            x = hz*wn[iz];
            ଯଦି( (unsigned)std::abs(hz) < kn[iz] )
                ଭାଙ୍ଗ;
            ଯଦି( iz == 0) // iz==0, handles the base strip
            {
                କର
                {
                    x = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    y = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    x = (ଦଶମିକ)(-std::log(x+FLT_MIN)*0.2904764);
                    y = (ଦଶମିକ)-std::log(y+FLT_MIN);
                }	// .2904764 is 1/r
                ଯେତେବେଳେ( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                ଭାଙ୍ଗ;
            }
            // iz > 0, handle the wedges of other strips
            y = (unsigned)temp*rng_flt;
            temp = RNG_NEXT(temp);
            ଯଦି( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::exp(-.5*x*x) )
                ଭାଙ୍ଗ;
        }
        arr[i] = x;
    }
    *state = temp;
}


ଦ୍ୱିଗୁଣ RNG::gaussian(ଦ୍ୱିଗୁଣ sigma)
{
    ଦଶମିକ temp;
    randn_0_1_32f( &temp, 1, &state );
    ଫେରନ୍ତୁ temp*sigma;
}

template<typename T, typename PT> ସ୍ଥିର ଶୂନ୍ୟ
randnScale_(ଦଶମିକ* src, ଶୂନ୍ୟ* _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn,
            ଅଚଳ ଶୂନ୍ୟ* _mean, ଅଚଳ ଶୂନ୍ୟ* _stddev, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    T* dst = (T*)_dst;
    ଅଚଳ PT* mean = (ଅଚଳ PT*)_mean;
    ଅଚଳ PT* stddev = (ଅଚଳ PT*)_stddev;
    ତାର୍କିକ stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    ଯଦି( !stdmtx || cn == 1 )
    {
        ଯଦି( cn == 1 )
        {
            PT a = stddev[0], b = mean[0];
            ପାଇଁ( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        ଅନ୍ୟଥା
        {
            len *= cn;
            cn--;
            ପାଇଁ( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + mean[k]);
                k = CN_NEXT(k);
            }
        }
    }
    ଅନ୍ୟଥା
    {
        len *= cn;
        cn--;
        ପାଇଁ( i = j = 0; i < len; i++ )
        {
            PT s = mean[j];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i0 = i - j;
            ପାଇଁ( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_NEXT(j);
        }
    }
}

// special version for 16f, 16bf and 32f
ସ୍ଥିର ଶୂନ୍ୟ
randnScale_16_or_32f(ଦଶମିକ* fbuf, ଶୂନ୍ୟ* _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn,
                     ଅଚଳ ଶୂନ୍ୟ* _mean, ଅଚଳ ଶୂନ୍ୟ* _stddev, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    ଦଶମିକ* dst = (ଦଶମିକ*)_dst;
    ଅଚଳ ଦଶମିକ* mean = (ଅଚଳ ଦଶମିକ*)_mean;
    ଅଚଳ ଦଶମିକ* stddev = (ଅଚଳ ଦଶମିକ*)_stddev;
    ତାର୍କିକ stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = CV_MAT_DEPTH(flags);
    ଦଶମିକ* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : dst;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;

    ଯଦି( !stdmtx || cn == 1 )
    {
        ଯଦି( cn == 1 )
        {
            ଦଶମିକ a = stddev[0], b = mean[0];
            ପାଇଁ( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        ଅନ୍ୟଥା
        {
            len *= cn;
            cn--;
            ପାଇଁ( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + mean[k];
                k = CN_NEXT(k);
            }
        }
    }
    ଅନ୍ୟଥା ଯଦି( depth == CV_32F )
    {
        len *= cn;
        cn--;
        ପାଇଁ( i = j = 0; i < len; i++ )
        {
            ଦଶମିକ s = mean[j];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i0 = i - j;
            ପାଇଁ( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_NEXT(j);
        }
    }
    ଅନ୍ୟଥା
    {
        ଦଶମିକ elembuf[CV_CN_MAX];
        len *= cn;
        ପାଇଁ( i = 0; i < len; i += cn )
        {
            // since we process fbuf in-place,
            // we need to copy each cn-channel element
            // prior to matrix multiplication
            ପାଇଁ (j = 0; j < cn; j++)
                elembuf[j] = fbuf[i + j];
            ପାଇଁ (j = 0; j < cn; j++) {
                ଦଶମିକ s = mean[j];
                ପାଇଁ( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    ଯଦି (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    ଅନ୍ୟଥା ଯଦି (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    ଅନ୍ୟଥା ଯଦି (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*RandnScaleFunc)(ଦଶମିକ* src, ଶୂନ୍ୟ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn,
                               ଅଚଳ ଶୂନ୍ୟ* mean, ଅଚଳ ଶୂନ୍ୟ* stddev, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);

ସ୍ଥିର RandnScaleFunc randnScaleTab[CV_DEPTH_MAX] =
{
    randnScale_<uchar, ଦଶମିକ>, randnScale_<schar, ଦଶମିକ>, randnScale_<ushort, ଦଶମିକ>,
    randnScale_<short, ଦଶମିକ>, randnScale_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଦଶମିକ>, randnScale_16_or_32f,
    randnScale_<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<ତାର୍କିକ, ଦଶମିକ>, randnScale_<uint64_t, ଦ୍ୱିଗୁଣ>, randnScale_<int64_t, ଦ୍ୱିଗୁଣ>,
    randnScale_<unsigned, ଦଶମିକ>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

ଶୂନ୍ୟ RNG::fill( ନିବେଶ_Output_Array _mat, ପୂର୍ଣ୍ଣସଂଖ୍ୟା disttype,
                ନିବେଶ_Array _param1arg, ନିବେଶ_Array _param2arg,
                ତାର୍କିକ saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, depth = mat.depth(), cn = mat.channels();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା esz1 = CV_ELEM_SIZE(depth);
    AutoBuffer<ଦ୍ୱିଗୁଣ> _parambuf;
    ତାର୍କିକ fast_int_mode = ମିଥ୍ୟା;
    ତାର୍କିକ small_flag = ମିଥ୍ୟା;
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

    ଅଚଳ ଶୂନ୍ୟ* uni_param = 0;
    uchar* mean = 0;
    uchar* stddev = 0;
    ତାର୍କିକ stdmtx = ମିଥ୍ୟା;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_param1.total();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n2 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_param2.total();

    ଯଦି( disttype == UNIFORM )
    {
        _parambuf.allocate(cn*(ଆକାର(DivStruct)+ଆକାର(ଦ୍ୱିଗୁଣ)-1)/ଆକାର(ଦ୍ୱିଗୁଣ) + cn*4);
        ଦ୍ୱିଗୁଣ* parambuf = _parambuf.data();
        ଦ୍ୱିଗୁଣ* p1 = _param1.ptr<ଦ୍ୱିଗୁଣ>();
        ଦ୍ୱିଗୁଣ* p2 = _param2.ptr<ଦ୍ୱିଗୁଣ>();

        ଯଦି( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convertTo(tmp, CV_64F);
            ପାଇଁ( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        ଯଦି( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convertTo(tmp, CV_64F);
            ପାଇଁ( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        ଯଦି( CV_IS_INT_TYPE(depth) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            ପାଇଁ( j = 0, fast_int_mode = ସତ୍ୟ; j < cn; j++ )
            {
                ଦ୍ୱିଗୁଣ a = std::min(p1[j], p2[j]);
                ଦ୍ୱିଗୁଣ b = std::max(p1[j], p2[j]);
                ଯଦି( saturateRange )
                {
                    a = std::max(a, depth == CV_8U || depth == CV_16U || depth == CV_32U ||
                                 depth == CV_64U || depth == CV_Bool ? 0. :
                                 depth == CV_8S ? -128. : depth == CV_16S ? -32768. :
                                 depth == CV_32S ? (ଦ୍ୱିଗୁଣ)INT_MIN : (ଦ୍ୱିଗୁଣ)INT64_MIN);
                    b = std::min(b, depth == CV_8U ? 256. : depth == CV_Bool ? 2. : depth == CV_16U ? 65536. :
                                 depth == CV_8S ? 128. : depth == CV_16S ? 32768. : depth == CV_32U ? (ଦ୍ୱିଗୁଣ)UINT_MAX :
                                 depth == CV_32S ? (ଦ୍ୱିଗୁଣ)INT_MAX : (ଦ୍ୱିଗୁଣ)INT64_MAX);
                }
                ip[j][1] = (int64_t)ceil(a);
                int64_t idiff = ip[j][0] = (int64_t)floor(b) - ip[j][1] - 1;
                ଯଦି (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                ଦ୍ୱିଗୁଣ diff = b - a;

                fast_int_mode = fast_int_mode && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                ଯଦି( fast_int_mode )
                    small_flag = idiff <= 255;
                ଅନ୍ୟଥା
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    ଯଦି (depth == CV_64S || depth == CV_64U)
                    {
                        minval = INT64_MIN/2;
                        maxval = INT64_MAX;
                    }
                    ଯଦି( diff > (ଦ୍ୱିଗୁଣ)maxval )
                        ip[j][0] = maxval;
                    ଯଦି( a < (ଦ୍ୱିଗୁଣ)minval )
                        ip[j][1] = minval;
                }
            }

            uni_param = ip;
            ଯଦି( !fast_int_mode )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((ଶୂନ୍ୟ*)(ds + cn), (ଶୂନ୍ୟ*)(parambuf + _parambuf.size()), "Last byte check");
                ପାଇଁ( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    ଯଦି (depth != CV_64U && depth != CV_64S) {
                        unsigned d = ds[j].d = (unsigned)(ip[j][0]+1);
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = 0;
                        ଯେତେବେଳେ(((uint64)1 << l) < d)
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
        ଅନ୍ୟଥା
        {
            ଦ୍ୱିଗୁଣ scale = depth == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            ଦ୍ୱିଗୁଣ maxdiff = saturateRange ? (ଦ୍ୱିଗୁଣ)FLT_MAX : DBL_MAX;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            ଯଦି( depth != CV_64F )
            {
                Vec2f* fp = (Vec2f*)(parambuf + cn*2);
                ପାଇଁ( j = 0; j < cn; j++ )
                {
                    fp[j][0] = (ଦଶମିକ)(std::min(maxdiff, p2[j] - p1[j])*scale);
                    fp[j][1] = (ଦଶମିକ)((p2[j] + p1[j])*0.5);
                }
                uni_param = fp;
            }
            ଅନ୍ୟଥା
            {
                Vec2d* dp = (Vec2d*)(parambuf + cn*2);
                ପାଇଁ( j = 0; j < cn; j++ )
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
    ଅନ୍ୟଥା ଯଦି( disttype == RNG::NORMAL )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        ଦ୍ୱିଗୁଣ* parambuf = _parambuf.data();

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ptype = esz1 == 8 ? CV_64F : CV_32F;

        ଯଦି( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            mean = _param1.ptr();
        ଅନ୍ୟଥା
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convertTo(tmp, ptype);
            mean = (uchar*)parambuf;
        }

        ଯଦି( n1 < cn )
            ପାଇଁ( j = n1*esz1; j < cn*esz1; j++ )
                mean[j] = mean[j - n1*esz1];

        ଯଦି( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        ଅନ୍ୟଥା
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convertTo(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        ଯଦି( n2 < cn )
            ପାଇଁ( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.rows == cn && _param2.cols == cn;
        scaleFunc = randnScaleTab[depth];
        CV_Assert( scaleFunc != 0 );
    }
    ଅନ୍ୟଥା
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    ଅଚଳ Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    ଦଶମିକ fbuf[BLOCK_SIZE + CV_CN_MAX];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା blockSize = std::min((BLOCK_SIZE + cn - 1)/cn, total);
    size_t esz = (size_t)esz1*cn;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = mat.type();

    ଯଦି( disttype == UNIFORM )
        flags |= (small_flag ? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)RNG_FLAG_SMALL : 0);
    ଅନ୍ୟଥା
        flags |= (stdmtx ? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)RNG_FLAG_STDMTX : 0);

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ପାଇଁ( j = 0; j < total; j += blockSize )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = std::min(total - j, blockSize);

            ଯଦି( disttype == UNIFORM )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            ଅନ୍ୟଥା
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
    ଫେରନ୍ତୁ getCoreTlsData().rng;
}

ଶୂନ୍ୟ cv::setRNGSeed(ପୂର୍ଣ୍ଣସଂଖ୍ୟା seed)
{
    theRNG() = RNG(static_cast<uint64>(seed));
}


ଶୂନ୍ୟ cv::randu(ନିବେଶ_Output_Array dst, ନିବେଶ_Array ତଳ, ନିବେଶ_Array ଉପର)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::UNIFORM, ତଳ, ଉପର);
}

ଶୂନ୍ୟ cv::randn(ନିବେଶ_Output_Array dst, ନିବେଶ_Array mean, ନିବେଶ_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::NORMAL, mean, stddev);
}

ନାମାକାଶ cv
{

template<typename T> ସ୍ଥିର ଶୂନ୍ୟ
randShuffle_( Mat& _arr, RNG& rng, ଦ୍ୱିଗୁଣ )
{
    unsigned sz = (unsigned)_arr.total();
    ଯଦି( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        ପାଇଁ( unsigned i = 0; i < sz; i++ )
        {
            unsigned j = (unsigned)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    ଅନ୍ୟଥା
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* data = _arr.ptr();
        size_t step = _arr.step;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows = _arr.rows;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols = _arr.cols;
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i0 = 0; i0 < rows; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j0 = 0; j0 < cols; j0++ )
            {
                unsigned k1 = (unsigned)rng % sz;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା i1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(k1 / cols);
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା j1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(k1 - (unsigned)i1*(unsigned)cols);
                std::swap( p[j0], ((T*)(data + step*i1))[j1] );
            }
        }
    }
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*RandShuffleFunc)( Mat& dst, RNG& rng, ଦ୍ୱିଗୁଣ iterFactor );

}

ଶୂନ୍ୟ cv::randShuffle( ନିବେଶ_Output_Array _dst, ଦ୍ୱିଗୁଣ iterFactor, RNG* _rng )
{
    CV_INSTRUMENT_REGION();

    RandShuffleFunc tab[] =
    {
        0,
        randShuffle_<uchar>, // 1
        randShuffle_<ushort>, // 2
        randShuffle_<Vec<uchar,3> >, // 3
        randShuffle_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, // 4
        0,
        randShuffle_<Vec<ushort,3> >, // 6
        0,
        randShuffle_<Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା,2> >, // 8
        0, 0, 0,
        randShuffle_<Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା,3> >, // 12
        0, 0, 0,
        randShuffle_<Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା,4> >, // 16
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା,6> >, // 24
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା,8> > // 32
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

ଶୂନ୍ୟ cv::RNG_MT19937::seed(unsigned s)
{
    state[0]= s;
    ପାଇଁ (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

unsigned cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    ସ୍ଥିର unsigned mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    ଅଚଳ unsigned UPPER_MASK = 0x80000000U;
    ଅଚଳ unsigned LOWER_MASK = 0x7fffffffU;

    /* generate N words at one time */
    ଯଦି (mti >= N)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା kk = 0;

        ପାଇଁ (; kk < N - M; ++kk)
        {
            unsigned y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        ପାଇଁ (; kk < N - 1; ++kk)
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

    ଫେରନ୍ତୁ y;
}

cv::RNG_MT19937::operator unsigned() { ଫେରନ୍ତୁ next(); }

cv::RNG_MT19937::operator ପୂର୍ଣ୍ଣସଂଖ୍ୟା() { ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)next();}

cv::RNG_MT19937::operator ଦଶମିକ() { ଫେରନ୍ତୁ next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::operator ଦ୍ୱିଗୁଣ()
{
    unsigned a = next() >> 5;
    unsigned b = next() >> 6;
    ଫେରନ୍ତୁ (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::RNG_MT19937::uniform(ପୂର୍ଣ୍ଣସଂଖ୍ୟା a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା b) { ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(next() % (b - a) + a); }

ଦଶମିକ cv::RNG_MT19937::uniform(ଦଶମିକ a, ଦଶମିକ b) { ଫେରନ୍ତୁ ((ଦଶମିକ)*ଏହି)*(b - a) + a; }

ଦ୍ୱିଗୁଣ cv::RNG_MT19937::uniform(ଦ୍ୱିଗୁଣ a, ଦ୍ୱିଗୁଣ b) { ଫେରନ୍ତୁ ((ଦ୍ୱିଗୁଣ)*ଏହି)*(b - a) + a; }

unsigned cv::RNG_MT19937::operator ()(unsigned b) { ଫେରନ୍ତୁ next() % b; }

unsigned cv::RNG_MT19937::operator ()() { ଫେରନ୍ତୁ next(); }

/* End of file. */
