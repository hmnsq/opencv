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

#അന്തര്ഭാവ "precomp.hpp"

നാമാകാശ cv
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

എണ്ണൽ
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

template<typename T> സ്ഥിതം ശൂന്യം
randBits_( T* arr, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, uint64* state, സ്ഥിരം Vec2l* p, പൂർണ്ണസംഖ്യ flags )
{
    തർക്കം small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 temp = *state;
    പൂർണ്ണസംഖ്യ i, k = 0;
    len *= cn;
    --cn;

    എങ്കിൽ( !small_flag )
    {
        വേണ്ടി( i = 0; i <= len - 4; i += 4 )
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
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( i = 0; i <= len - 4; i += 4 )
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

    വേണ്ടി( ; i < len; i++ )
    {
        int64_t t0;
        temp = RNG_NEXT(temp);

        t0 = ((int64_t)temp & p[k][0]) + p[k][1];
        k = CN_NEXT(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = temp;
}

ഘടന DivStruct
{
    unsigned d;
    unsigned M;
    പൂർണ്ണസംഖ്യ sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

template<typename T> സ്ഥിതം ശൂന്യം
randi_( T* arr, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, uint64* state, സ്ഥിരം DivStruct* p )
{
    uint64 temp = *state;
    പൂർണ്ണസംഖ്യ k = 0;
    len *= cn;
    cn--;
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i++ )
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

സ്ഥിതം ശൂന്യം
randi_( int64_t* arr, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, uint64* state, സ്ഥിരം DivStruct* p )
{
    uint64 temp = *state;
    പൂർണ്ണസംഖ്യ k = 0;
    len *= cn;
    cn--;
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i++ )
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

സ്ഥിതം ശൂന്യം
randi_( uint64_t* arr, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, uint64* state, സ്ഥിരം DivStruct* p )
{
    uint64 temp = *state;
    പൂർണ്ണസംഖ്യ k = 0;
    len *= cn;
    cn--;
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i++ )
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
സ്ഥിതം ശൂന്യം randBitsWrap(ശൂന്യം* arr, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, uint64* state, സ്ഥിരം ശൂന്യം* p, ശൂന്യം*, പൂർണ്ണസംഖ്യ flags)
{
    randBits_((T*)arr, len, cn, state, (സ്ഥിരം Vec2l*)p, flags);
}

template<typename T>
സ്ഥിതം ശൂന്യം randiWrap(ശൂന്യം* arr, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, uint64* state, സ്ഥിരം ശൂന്യം* p, ശൂന്യം*, പൂർണ്ണസംഖ്യ)
{
    randi_((T*)arr, len, cn, state, (സ്ഥിരം DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
സ്ഥിതം വരിയിൽ ശൂന്യം cvt32fToFP8(സ്ഥിരം ദശാംശം* src, ശൂന്യം* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ depth)
{
    എങ്കിൽ (depth == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    അല്ലെങ്കിൽ                              { fp8a_t* d = (fp8a_t*)dst; വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
സ്ഥിതം വരിയിൽ തർക്കം isFP8Depth(പൂർണ്ണസംഖ്യ d) { തിരികെ d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

സ്ഥിതം ശൂന്യം randf_16_or_32f( ശൂന്യം* dst, പൂർണ്ണസംഖ്യ len_, പൂർണ്ണസംഖ്യ cn, uint64* state, സ്ഥിരം ശൂന്യം* _p, ശൂന്യം* _fbuf, പൂർണ്ണസംഖ്യ flags )
{
    സ്ഥിരം Vec2f* p = (സ്ഥിരം Vec2f*)_p;
    ദശാംശം* fbuf = (ദശാംശം*)_fbuf;
    പൂർണ്ണസംഖ്യ depth = CV_MAT_DEPTH(flags);
    uint64 temp = *state;
    പൂർണ്ണസംഖ്യ k = 0, len = len_*cn;
    ദശാംശം* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : (ദശാംശം*)dst;
    cn--;
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i++ )
    {
        പൂർണ്ണസംഖ്യ t = (പൂർണ്ണസംഖ്യ)(temp = RNG_NEXT(temp));
        arr[i] = (ദശാംശം)(t*p[k][0]);
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias32f(arr, &p[0][0], len_, cn+1);
    എങ്കിൽ (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    അല്ലെങ്കിൽ എങ്കിൽ (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    അല്ലെങ്കിൽ എങ്കിൽ (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

സ്ഥിതം ശൂന്യം
randf_64f( ശൂന്യം* _arr, പൂർണ്ണസംഖ്യ len_, പൂർണ്ണസംഖ്യ cn, uint64* state, സ്ഥിരം ശൂന്യം* _p, ശൂന്യം*, പൂർണ്ണസംഖ്യ )
{
    ഇരട്ട* arr = (ഇരട്ട*)_arr;
    സ്ഥിരം Vec2d* p = (സ്ഥിരം Vec2d*)_p;
    uint64 temp = *state;
    പൂർണ്ണസംഖ്യ k = 0, len = len_*cn;
    cn--;
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i++ )
    {
        temp = RNG_NEXT(temp);
        int64_t v = (int64_t)((temp >> 32) | (temp << 32));
        arr[i] = v*p[k][0];
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias64f(arr, &p[0][0], len_, cn+1);
}

തരനിർവചനം ശൂന്യം (*RandFunc)(ശൂന്യം* arr, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, uint64* state,
                         സ്ഥിരം ശൂന്യം* p, ശൂന്യം* tempbuf, പൂർണ്ണസംഖ്യ flags);

സ്ഥിതം RandFunc randTab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<short>, randiWrap<പൂർണ്ണസംഖ്യ>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<തർക്കം>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<unsigned>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        randBitsWrap<uchar>, randBitsWrap<schar>, randBitsWrap<ushort>,
        randBitsWrap<short>, randBitsWrap<പൂർണ്ണസംഖ്യ>, 0, 0, 0, 0,
        randBitsWrap<തർക്കം>, randBitsWrap<uint64_t>, randBitsWrap<int64_t>,
        randBitsWrap<unsigned>, 0, 0, 0
    }
};

/*
   The code below implements the algorithm described in
   "The Ziggurat Method for Generating Random Variables"
   by George Marsaglia and Wai Wan Tsang, Journal of Statistical Software, 2007.
*/
സ്ഥിതം ശൂന്യം
randn_0_1_32f( ദശാംശം* arr, പൂർണ്ണസംഖ്യ len, uint64* state )
{
    സ്ഥിരം ദശാംശം r = 3.442620f; // The start of the right tail
    സ്ഥിരം ദശാംശം rng_flt = 2.3283064365386962890625e-10f; // 2^-32
    സ്ഥിതം unsigned kn[128];
    സ്ഥിതം ദശാംശം wn[128], fn[128];
    uint64 temp = *state;
    സ്ഥിതം തർക്കം initialized=അസത്യം;
    പൂർണ്ണസംഖ്യ i;

    എങ്കിൽ( !initialized )
    {
        സ്ഥിരം ഇരട്ട m1 = 2147483648.0;
        ഇരട്ട dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        ഇരട്ട q = vn/std::exp(-.5*dn*dn);
        kn[0] = (unsigned)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (ദശാംശം)(q/m1);
        wn[127] = (ദശാംശം)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (ദശാംശം)std::exp(-.5*dn*dn);

        വേണ്ടി(i=126;i>=1;i--)
        {
            dn = std::sqrt(-2.*std::log(vn/dn+std::exp(-.5*dn*dn)));
            kn[i+1] = (unsigned)((dn/tn)*m1);
            tn = dn;
            fn[i] = (ദശാംശം)std::exp(-.5*dn*dn);
            wn[i] = (ദശാംശം)(dn/m1);
        }
        initialized = സത്യം;
    }

    വേണ്ടി( i = 0; i < len; i++ )
    {
        ദശാംശം x, y;
        വേണ്ടി(;;)
        {
            പൂർണ്ണസംഖ്യ hz = (പൂർണ്ണസംഖ്യ)temp;
            temp = RNG_NEXT(temp);
            പൂർണ്ണസംഖ്യ iz = hz & 127;
            x = hz*wn[iz];
            എങ്കിൽ( (unsigned)std::abs(hz) < kn[iz] )
                നിർത്തുക;
            എങ്കിൽ( iz == 0) // iz==0, handles the base strip
            {
                ചെയ്യുക
                {
                    x = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    y = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    x = (ദശാംശം)(-std::log(x+FLT_MIN)*0.2904764);
                    y = (ദശാംശം)-std::log(y+FLT_MIN);
                }	// .2904764 is 1/r
                ആകുമ്പോൾ( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                നിർത്തുക;
            }
            // iz > 0, handle the wedges of other strips
            y = (unsigned)temp*rng_flt;
            temp = RNG_NEXT(temp);
            എങ്കിൽ( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::exp(-.5*x*x) )
                നിർത്തുക;
        }
        arr[i] = x;
    }
    *state = temp;
}


ഇരട്ട RNG::gaussian(ഇരട്ട sigma)
{
    ദശാംശം temp;
    randn_0_1_32f( &temp, 1, &state );
    തിരികെ temp*sigma;
}

template<typename T, typename PT> സ്ഥിതം ശൂന്യം
randnScale_(ദശാംശം* src, ശൂന്യം* _dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn,
            സ്ഥിരം ശൂന്യം* _mean, സ്ഥിരം ശൂന്യം* _stddev, പൂർണ്ണസംഖ്യ flags )
{
    T* dst = (T*)_dst;
    സ്ഥിരം PT* mean = (സ്ഥിരം PT*)_mean;
    സ്ഥിരം PT* stddev = (സ്ഥിരം PT*)_stddev;
    തർക്കം stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    പൂർണ്ണസംഖ്യ i, j, k;
    എങ്കിൽ( !stdmtx || cn == 1 )
    {
        എങ്കിൽ( cn == 1 )
        {
            PT a = stddev[0], b = mean[0];
            വേണ്ടി( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        അല്ലെങ്കിൽ
        {
            len *= cn;
            cn--;
            വേണ്ടി( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + mean[k]);
                k = CN_NEXT(k);
            }
        }
    }
    അല്ലെങ്കിൽ
    {
        len *= cn;
        cn--;
        വേണ്ടി( i = j = 0; i < len; i++ )
        {
            PT s = mean[j];
            പൂർണ്ണസംഖ്യ i0 = i - j;
            വേണ്ടി( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_NEXT(j);
        }
    }
}

// special version for 16f, 16bf and 32f
സ്ഥിതം ശൂന്യം
randnScale_16_or_32f(ദശാംശം* fbuf, ശൂന്യം* _dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn,
                     സ്ഥിരം ശൂന്യം* _mean, സ്ഥിരം ശൂന്യം* _stddev, പൂർണ്ണസംഖ്യ flags)
{
    ദശാംശം* dst = (ദശാംശം*)_dst;
    സ്ഥിരം ദശാംശം* mean = (സ്ഥിരം ദശാംശം*)_mean;
    സ്ഥിരം ദശാംശം* stddev = (സ്ഥിരം ദശാംശം*)_stddev;
    തർക്കം stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    പൂർണ്ണസംഖ്യ depth = CV_MAT_DEPTH(flags);
    ദശാംശം* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : dst;
    പൂർണ്ണസംഖ്യ i, j, k;

    എങ്കിൽ( !stdmtx || cn == 1 )
    {
        എങ്കിൽ( cn == 1 )
        {
            ദശാംശം a = stddev[0], b = mean[0];
            വേണ്ടി( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        അല്ലെങ്കിൽ
        {
            len *= cn;
            cn--;
            വേണ്ടി( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + mean[k];
                k = CN_NEXT(k);
            }
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_32F )
    {
        len *= cn;
        cn--;
        വേണ്ടി( i = j = 0; i < len; i++ )
        {
            ദശാംശം s = mean[j];
            പൂർണ്ണസംഖ്യ i0 = i - j;
            വേണ്ടി( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_NEXT(j);
        }
    }
    അല്ലെങ്കിൽ
    {
        ദശാംശം elembuf[CV_CN_MAX];
        len *= cn;
        വേണ്ടി( i = 0; i < len; i += cn )
        {
            // since we process fbuf in-place,
            // we need to copy each cn-channel element
            // prior to matrix multiplication
            വേണ്ടി (j = 0; j < cn; j++)
                elembuf[j] = fbuf[i + j];
            വേണ്ടി (j = 0; j < cn; j++) {
                ദശാംശം s = mean[j];
                വേണ്ടി( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    എങ്കിൽ (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    അല്ലെങ്കിൽ എങ്കിൽ (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    അല്ലെങ്കിൽ എങ്കിൽ (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

തരനിർവചനം ശൂന്യം (*RandnScaleFunc)(ദശാംശം* src, ശൂന്യം* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn,
                               സ്ഥിരം ശൂന്യം* mean, സ്ഥിരം ശൂന്യം* stddev, പൂർണ്ണസംഖ്യ flags);

സ്ഥിതം RandnScaleFunc randnScaleTab[CV_DEPTH_MAX] =
{
    randnScale_<uchar, ദശാംശം>, randnScale_<schar, ദശാംശം>, randnScale_<ushort, ദശാംശം>,
    randnScale_<short, ദശാംശം>, randnScale_<പൂർണ്ണസംഖ്യ, ദശാംശം>, randnScale_16_or_32f,
    randnScale_<ഇരട്ട, ഇരട്ട>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<തർക്കം, ദശാംശം>, randnScale_<uint64_t, ഇരട്ട>, randnScale_<int64_t, ഇരട്ട>,
    randnScale_<unsigned, ദശാംശം>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

ശൂന്യം RNG::fill( നിവേശം_Output_Array _mat, പൂർണ്ണസംഖ്യ disttype,
                നിവേശം_Array _param1arg, നിവേശം_Array _param2arg,
                തർക്കം saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    പൂർണ്ണസംഖ്യ j, depth = mat.depth(), cn = mat.channels();
    പൂർണ്ണസംഖ്യ esz1 = CV_ELEM_SIZE(depth);
    AutoBuffer<ഇരട്ട> _parambuf;
    തർക്കം fast_int_mode = അസത്യം;
    തർക്കം small_flag = അസത്യം;
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

    സ്ഥിരം ശൂന്യം* uni_param = 0;
    uchar* mean = 0;
    uchar* stddev = 0;
    തർക്കം stdmtx = അസത്യം;
    പൂർണ്ണസംഖ്യ n1 = (പൂർണ്ണസംഖ്യ)_param1.total();
    പൂർണ്ണസംഖ്യ n2 = (പൂർണ്ണസംഖ്യ)_param2.total();

    എങ്കിൽ( disttype == UNIFORM )
    {
        _parambuf.allocate(cn*(വലുപ്പം(DivStruct)+വലുപ്പം(ഇരട്ട)-1)/വലുപ്പം(ഇരട്ട) + cn*4);
        ഇരട്ട* parambuf = _parambuf.data();
        ഇരട്ട* p1 = _param1.ptr<ഇരട്ട>();
        ഇരട്ട* p2 = _param2.ptr<ഇരട്ട>();

        എങ്കിൽ( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convertTo(tmp, CV_64F);
            വേണ്ടി( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        എങ്കിൽ( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convertTo(tmp, CV_64F);
            വേണ്ടി( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        എങ്കിൽ( CV_IS_INT_TYPE(depth) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            വേണ്ടി( j = 0, fast_int_mode = സത്യം; j < cn; j++ )
            {
                ഇരട്ട a = std::min(p1[j], p2[j]);
                ഇരട്ട b = std::max(p1[j], p2[j]);
                എങ്കിൽ( saturateRange )
                {
                    a = std::max(a, depth == CV_8U || depth == CV_16U || depth == CV_32U ||
                                 depth == CV_64U || depth == CV_Bool ? 0. :
                                 depth == CV_8S ? -128. : depth == CV_16S ? -32768. :
                                 depth == CV_32S ? (ഇരട്ട)INT_MIN : (ഇരട്ട)INT64_MIN);
                    b = std::min(b, depth == CV_8U ? 256. : depth == CV_Bool ? 2. : depth == CV_16U ? 65536. :
                                 depth == CV_8S ? 128. : depth == CV_16S ? 32768. : depth == CV_32U ? (ഇരട്ട)UINT_MAX :
                                 depth == CV_32S ? (ഇരട്ട)INT_MAX : (ഇരട്ട)INT64_MAX);
                }
                ip[j][1] = (int64_t)ceil(a);
                int64_t idiff = ip[j][0] = (int64_t)floor(b) - ip[j][1] - 1;
                എങ്കിൽ (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                ഇരട്ട diff = b - a;

                fast_int_mode = fast_int_mode && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                എങ്കിൽ( fast_int_mode )
                    small_flag = idiff <= 255;
                അല്ലെങ്കിൽ
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    എങ്കിൽ (depth == CV_64S || depth == CV_64U)
                    {
                        minval = INT64_MIN/2;
                        maxval = INT64_MAX;
                    }
                    എങ്കിൽ( diff > (ഇരട്ട)maxval )
                        ip[j][0] = maxval;
                    എങ്കിൽ( a < (ഇരട്ട)minval )
                        ip[j][1] = minval;
                }
            }

            uni_param = ip;
            എങ്കിൽ( !fast_int_mode )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((ശൂന്യം*)(ds + cn), (ശൂന്യം*)(parambuf + _parambuf.size()), "Last byte check");
                വേണ്ടി( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    എങ്കിൽ (depth != CV_64U && depth != CV_64S) {
                        unsigned d = ds[j].d = (unsigned)(ip[j][0]+1);
                        പൂർണ്ണസംഖ്യ l = 0;
                        ആകുമ്പോൾ(((uint64)1 << l) < d)
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
        അല്ലെങ്കിൽ
        {
            ഇരട്ട scale = depth == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            ഇരട്ട maxdiff = saturateRange ? (ഇരട്ട)FLT_MAX : DBL_MAX;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            എങ്കിൽ( depth != CV_64F )
            {
                Vec2f* fp = (Vec2f*)(parambuf + cn*2);
                വേണ്ടി( j = 0; j < cn; j++ )
                {
                    fp[j][0] = (ദശാംശം)(std::min(maxdiff, p2[j] - p1[j])*scale);
                    fp[j][1] = (ദശാംശം)((p2[j] + p1[j])*0.5);
                }
                uni_param = fp;
            }
            അല്ലെങ്കിൽ
            {
                Vec2d* dp = (Vec2d*)(parambuf + cn*2);
                വേണ്ടി( j = 0; j < cn; j++ )
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
    അല്ലെങ്കിൽ എങ്കിൽ( disttype == RNG::NORMAL )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        ഇരട്ട* parambuf = _parambuf.data();

        പൂർണ്ണസംഖ്യ ptype = esz1 == 8 ? CV_64F : CV_32F;

        എങ്കിൽ( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            mean = _param1.ptr();
        അല്ലെങ്കിൽ
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convertTo(tmp, ptype);
            mean = (uchar*)parambuf;
        }

        എങ്കിൽ( n1 < cn )
            വേണ്ടി( j = n1*esz1; j < cn*esz1; j++ )
                mean[j] = mean[j - n1*esz1];

        എങ്കിൽ( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        അല്ലെങ്കിൽ
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convertTo(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        എങ്കിൽ( n2 < cn )
            വേണ്ടി( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.rows == cn && _param2.cols == cn;
        scaleFunc = randnScaleTab[depth];
        CV_Assert( scaleFunc != 0 );
    }
    അല്ലെങ്കിൽ
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    സ്ഥിരം Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    ദശാംശം fbuf[BLOCK_SIZE + CV_CN_MAX];
    പൂർണ്ണസംഖ്യ total = (പൂർണ്ണസംഖ്യ)it.size;
    പൂർണ്ണസംഖ്യ blockSize = std::min((BLOCK_SIZE + cn - 1)/cn, total);
    size_t esz = (size_t)esz1*cn;
    പൂർണ്ണസംഖ്യ flags = mat.type();

    എങ്കിൽ( disttype == UNIFORM )
        flags |= (small_flag ? (പൂർണ്ണസംഖ്യ)RNG_FLAG_SMALL : 0);
    അല്ലെങ്കിൽ
        flags |= (stdmtx ? (പൂർണ്ണസംഖ്യ)RNG_FLAG_STDMTX : 0);

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        വേണ്ടി( j = 0; j < total; j += blockSize )
        {
            പൂർണ്ണസംഖ്യ len = std::min(total - j, blockSize);

            എങ്കിൽ( disttype == UNIFORM )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            അല്ലെങ്കിൽ
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
    തിരികെ getCoreTlsData().rng;
}

ശൂന്യം cv::setRNGSeed(പൂർണ്ണസംഖ്യ seed)
{
    theRNG() = RNG(static_cast<uint64>(seed));
}


ശൂന്യം cv::randu(നിവേശം_Output_Array dst, നിവേശം_Array താഴെയുള്ള, നിവേശം_Array മുകളിലെ)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::UNIFORM, താഴെയുള്ള, മുകളിലെ);
}

ശൂന്യം cv::randn(നിവേശം_Output_Array dst, നിവേശം_Array mean, നിവേശം_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::NORMAL, mean, stddev);
}

നാമാകാശ cv
{

template<typename T> സ്ഥിതം ശൂന്യം
randShuffle_( Mat& _arr, RNG& rng, ഇരട്ട )
{
    unsigned sz = (unsigned)_arr.total();
    എങ്കിൽ( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        വേണ്ടി( unsigned i = 0; i < sz; i++ )
        {
            unsigned j = (unsigned)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    അല്ലെങ്കിൽ
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* data = _arr.ptr();
        size_t step = _arr.step;
        പൂർണ്ണസംഖ്യ rows = _arr.rows;
        പൂർണ്ണസംഖ്യ cols = _arr.cols;
        വേണ്ടി( പൂർണ്ണസംഖ്യ i0 = 0; i0 < rows; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            വേണ്ടി( പൂർണ്ണസംഖ്യ j0 = 0; j0 < cols; j0++ )
            {
                unsigned k1 = (unsigned)rng % sz;
                പൂർണ്ണസംഖ്യ i1 = (പൂർണ്ണസംഖ്യ)(k1 / cols);
                പൂർണ്ണസംഖ്യ j1 = (പൂർണ്ണസംഖ്യ)(k1 - (unsigned)i1*(unsigned)cols);
                std::swap( p[j0], ((T*)(data + step*i1))[j1] );
            }
        }
    }
}

തരനിർവചനം ശൂന്യം (*RandShuffleFunc)( Mat& dst, RNG& rng, ഇരട്ട iterFactor );

}

ശൂന്യം cv::randShuffle( നിവേശം_Output_Array _dst, ഇരട്ട iterFactor, RNG* _rng )
{
    CV_INSTRUMENT_REGION();

    RandShuffleFunc tab[] =
    {
        0,
        randShuffle_<uchar>, // 1
        randShuffle_<ushort>, // 2
        randShuffle_<Vec<uchar,3> >, // 3
        randShuffle_<പൂർണ്ണസംഖ്യ>, // 4
        0,
        randShuffle_<Vec<ushort,3> >, // 6
        0,
        randShuffle_<Vec<പൂർണ്ണസംഖ്യ,2> >, // 8
        0, 0, 0,
        randShuffle_<Vec<പൂർണ്ണസംഖ്യ,3> >, // 12
        0, 0, 0,
        randShuffle_<Vec<പൂർണ്ണസംഖ്യ,4> >, // 16
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<പൂർണ്ണസംഖ്യ,6> >, // 24
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<പൂർണ്ണസംഖ്യ,8> > // 32
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

ശൂന്യം cv::RNG_MT19937::seed(unsigned s)
{
    state[0]= s;
    വേണ്ടി (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

unsigned cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    സ്ഥിതം unsigned mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    സ്ഥിരം unsigned UPPER_MASK = 0x80000000U;
    സ്ഥിരം unsigned LOWER_MASK = 0x7fffffffU;

    /* generate N words at one time */
    എങ്കിൽ (mti >= N)
    {
        പൂർണ്ണസംഖ്യ kk = 0;

        വേണ്ടി (; kk < N - M; ++kk)
        {
            unsigned y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        വേണ്ടി (; kk < N - 1; ++kk)
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

    തിരികെ y;
}

cv::RNG_MT19937::operator unsigned() { തിരികെ next(); }

cv::RNG_MT19937::operator പൂർണ്ണസംഖ്യ() { തിരികെ (പൂർണ്ണസംഖ്യ)next();}

cv::RNG_MT19937::operator ദശാംശം() { തിരികെ next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::operator ഇരട്ട()
{
    unsigned a = next() >> 5;
    unsigned b = next() >> 6;
    തിരികെ (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

പൂർണ്ണസംഖ്യ cv::RNG_MT19937::uniform(പൂർണ്ണസംഖ്യ a, പൂർണ്ണസംഖ്യ b) { തിരികെ (പൂർണ്ണസംഖ്യ)(next() % (b - a) + a); }

ദശാംശം cv::RNG_MT19937::uniform(ദശാംശം a, ദശാംശം b) { തിരികെ ((ദശാംശം)*ഇത്)*(b - a) + a; }

ഇരട്ട cv::RNG_MT19937::uniform(ഇരട്ട a, ഇരട്ട b) { തിരികെ ((ഇരട്ട)*ഇത്)*(b - a) + a; }

unsigned cv::RNG_MT19937::operator ()(unsigned b) { തിരികെ next() % b; }

unsigned cv::RNG_MT19937::operator ()() { തിരികെ next(); }

/* End of file. */
