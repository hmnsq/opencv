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

#অন্তর্ভাৱ "precomp.hpp"

নামাকাশ cv
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

গণনা
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

template<typename T> স্থির শূন্য
randBits_( T* arr, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, uint64* state, ধ্রুৱ Vec2l* p, পূর্ণাঙ্ক flags )
{
    তার্কিক small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 temp = *state;
    পূর্ণাঙ্ক i, k = 0;
    len *= cn;
    --cn;

    যদি( !small_flag )
    {
        বাবে( i = 0; i <= len - 4; i += 4 )
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
    অন্যথা
    {
        বাবে( i = 0; i <= len - 4; i += 4 )
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

    বাবে( ; i < len; i++ )
    {
        int64_t t0;
        temp = RNG_NEXT(temp);

        t0 = ((int64_t)temp & p[k][0]) + p[k][1];
        k = CN_NEXT(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = temp;
}

সংরচনা DivStruct
{
    unsigned d;
    unsigned M;
    পূর্ণাঙ্ক sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

template<typename T> স্থির শূন্য
randi_( T* arr, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, uint64* state, ধ্রুৱ DivStruct* p )
{
    uint64 temp = *state;
    পূর্ণাঙ্ক k = 0;
    len *= cn;
    cn--;
    বাবে( পূর্ণাঙ্ক i = 0; i < len; i++ )
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

স্থির শূন্য
randi_( int64_t* arr, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, uint64* state, ধ্রুৱ DivStruct* p )
{
    uint64 temp = *state;
    পূর্ণাঙ্ক k = 0;
    len *= cn;
    cn--;
    বাবে( পূর্ণাঙ্ক i = 0; i < len; i++ )
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

স্থির শূন্য
randi_( uint64_t* arr, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, uint64* state, ধ্রুৱ DivStruct* p )
{
    uint64 temp = *state;
    পূর্ণাঙ্ক k = 0;
    len *= cn;
    cn--;
    বাবে( পূর্ণাঙ্ক i = 0; i < len; i++ )
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
স্থির শূন্য randBitsWrap(শূন্য* arr, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, uint64* state, ধ্রুৱ শূন্য* p, শূন্য*, পূর্ণাঙ্ক flags)
{
    randBits_((T*)arr, len, cn, state, (ধ্রুৱ Vec2l*)p, flags);
}

template<typename T>
স্থির শূন্য randiWrap(শূন্য* arr, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, uint64* state, ধ্রুৱ শূন্য* p, শূন্য*, পূর্ণাঙ্ক)
{
    randi_((T*)arr, len, cn, state, (ধ্রুৱ DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
স্থির অন্তঃস্থ শূন্য cvt32fToFP8(ধ্রুৱ ভিন্ন* src, শূন্য* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক depth)
{
    যদি (depth == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   বাবে (পূর্ণাঙ্ক i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    অন্যথা                              { fp8a_t* d = (fp8a_t*)dst; বাবে (পূর্ণাঙ্ক i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
স্থির অন্তঃস্থ তার্কিক isFP8Depth(পূর্ণাঙ্ক d) { ঘূৰাই d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

স্থির শূন্য randf_16_or_32f( শূন্য* dst, পূর্ণাঙ্ক len_, পূর্ণাঙ্ক cn, uint64* state, ধ্রুৱ শূন্য* _p, শূন্য* _fbuf, পূর্ণাঙ্ক flags )
{
    ধ্রুৱ Vec2f* p = (ধ্রুৱ Vec2f*)_p;
    ভিন্ন* fbuf = (ভিন্ন*)_fbuf;
    পূর্ণাঙ্ক depth = CV_MAT_DEPTH(flags);
    uint64 temp = *state;
    পূর্ণাঙ্ক k = 0, len = len_*cn;
    ভিন্ন* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : (ভিন্ন*)dst;
    cn--;
    বাবে( পূর্ণাঙ্ক i = 0; i < len; i++ )
    {
        পূর্ণাঙ্ক t = (পূর্ণাঙ্ক)(temp = RNG_NEXT(temp));
        arr[i] = (ভিন্ন)(t*p[k][0]);
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias32f(arr, &p[0][0], len_, cn+1);
    যদি (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    অন্যথা যদি (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    অন্যথা যদি (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

স্থির শূন্য
randf_64f( শূন্য* _arr, পূর্ণাঙ্ক len_, পূর্ণাঙ্ক cn, uint64* state, ধ্রুৱ শূন্য* _p, শূন্য*, পূর্ণাঙ্ক )
{
    দ্ৱিগুণ* arr = (দ্ৱিগুণ*)_arr;
    ধ্রুৱ Vec2d* p = (ধ্রুৱ Vec2d*)_p;
    uint64 temp = *state;
    পূর্ণাঙ্ক k = 0, len = len_*cn;
    cn--;
    বাবে( পূর্ণাঙ্ক i = 0; i < len; i++ )
    {
        temp = RNG_NEXT(temp);
        int64_t v = (int64_t)((temp >> 32) | (temp << 32));
        arr[i] = v*p[k][0];
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias64f(arr, &p[0][0], len_, cn+1);
}

প্রকারনির্দেশ শূন্য (*RandFunc)(শূন্য* arr, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, uint64* state,
                         ধ্রুৱ শূন্য* p, শূন্য* tempbuf, পূর্ণাঙ্ক flags);

স্থির RandFunc randTab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<short>, randiWrap<পূর্ণাঙ্ক>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<তার্কিক>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<unsigned>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        randBitsWrap<uchar>, randBitsWrap<schar>, randBitsWrap<ushort>,
        randBitsWrap<short>, randBitsWrap<পূর্ণাঙ্ক>, 0, 0, 0, 0,
        randBitsWrap<তার্কিক>, randBitsWrap<uint64_t>, randBitsWrap<int64_t>,
        randBitsWrap<unsigned>, 0, 0, 0
    }
};

/*
   The code below implements the algorithm described in
   "The Ziggurat Method for Generating Random Variables"
   by George Marsaglia and Wai Wan Tsang, Journal of Statistical Software, 2007.
*/
স্থির শূন্য
randn_0_1_32f( ভিন্ন* arr, পূর্ণাঙ্ক len, uint64* state )
{
    ধ্রুৱ ভিন্ন r = 3.442620f; // The start of the right tail
    ধ্রুৱ ভিন্ন rng_flt = 2.3283064365386962890625e-10f; // 2^-32
    স্থির unsigned kn[128];
    স্থির ভিন্ন wn[128], fn[128];
    uint64 temp = *state;
    স্থির তার্কিক initialized=মিছা;
    পূর্ণাঙ্ক i;

    যদি( !initialized )
    {
        ধ্রুৱ দ্ৱিগুণ m1 = 2147483648.0;
        দ্ৱিগুণ dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        দ্ৱিগুণ q = vn/std::exp(-.5*dn*dn);
        kn[0] = (unsigned)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (ভিন্ন)(q/m1);
        wn[127] = (ভিন্ন)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (ভিন্ন)std::exp(-.5*dn*dn);

        বাবে(i=126;i>=1;i--)
        {
            dn = std::sqrt(-2.*std::log(vn/dn+std::exp(-.5*dn*dn)));
            kn[i+1] = (unsigned)((dn/tn)*m1);
            tn = dn;
            fn[i] = (ভিন্ন)std::exp(-.5*dn*dn);
            wn[i] = (ভিন্ন)(dn/m1);
        }
        initialized = সঁচা;
    }

    বাবে( i = 0; i < len; i++ )
    {
        ভিন্ন x, y;
        বাবে(;;)
        {
            পূর্ণাঙ্ক hz = (পূর্ণাঙ্ক)temp;
            temp = RNG_NEXT(temp);
            পূর্ণাঙ্ক iz = hz & 127;
            x = hz*wn[iz];
            যদি( (unsigned)std::abs(hz) < kn[iz] )
                ভঙ্গ;
            যদি( iz == 0) // iz==0, handles the base strip
            {
                কৰা
                {
                    x = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    y = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    x = (ভিন্ন)(-std::log(x+FLT_MIN)*0.2904764);
                    y = (ভিন্ন)-std::log(y+FLT_MIN);
                }	// .2904764 is 1/r
                যেতিয়ালৈ( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                ভঙ্গ;
            }
            // iz > 0, handle the wedges of other strips
            y = (unsigned)temp*rng_flt;
            temp = RNG_NEXT(temp);
            যদি( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::exp(-.5*x*x) )
                ভঙ্গ;
        }
        arr[i] = x;
    }
    *state = temp;
}


দ্ৱিগুণ RNG::gaussian(দ্ৱিগুণ sigma)
{
    ভিন্ন temp;
    randn_0_1_32f( &temp, 1, &state );
    ঘূৰাই temp*sigma;
}

template<typename T, typename PT> স্থির শূন্য
randnScale_(ভিন্ন* src, শূন্য* _dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn,
            ধ্রুৱ শূন্য* _mean, ধ্রুৱ শূন্য* _stddev, পূর্ণাঙ্ক flags )
{
    T* dst = (T*)_dst;
    ধ্রুৱ PT* mean = (ধ্রুৱ PT*)_mean;
    ধ্রুৱ PT* stddev = (ধ্রুৱ PT*)_stddev;
    তার্কিক stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    পূর্ণাঙ্ক i, j, k;
    যদি( !stdmtx || cn == 1 )
    {
        যদি( cn == 1 )
        {
            PT a = stddev[0], b = mean[0];
            বাবে( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        অন্যথা
        {
            len *= cn;
            cn--;
            বাবে( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + mean[k]);
                k = CN_NEXT(k);
            }
        }
    }
    অন্যথা
    {
        len *= cn;
        cn--;
        বাবে( i = j = 0; i < len; i++ )
        {
            PT s = mean[j];
            পূর্ণাঙ্ক i0 = i - j;
            বাবে( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_NEXT(j);
        }
    }
}

// special version for 16f, 16bf and 32f
স্থির শূন্য
randnScale_16_or_32f(ভিন্ন* fbuf, শূন্য* _dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn,
                     ধ্রুৱ শূন্য* _mean, ধ্রুৱ শূন্য* _stddev, পূর্ণাঙ্ক flags)
{
    ভিন্ন* dst = (ভিন্ন*)_dst;
    ধ্রুৱ ভিন্ন* mean = (ধ্রুৱ ভিন্ন*)_mean;
    ধ্রুৱ ভিন্ন* stddev = (ধ্রুৱ ভিন্ন*)_stddev;
    তার্কিক stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    পূর্ণাঙ্ক depth = CV_MAT_DEPTH(flags);
    ভিন্ন* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : dst;
    পূর্ণাঙ্ক i, j, k;

    যদি( !stdmtx || cn == 1 )
    {
        যদি( cn == 1 )
        {
            ভিন্ন a = stddev[0], b = mean[0];
            বাবে( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        অন্যথা
        {
            len *= cn;
            cn--;
            বাবে( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + mean[k];
                k = CN_NEXT(k);
            }
        }
    }
    অন্যথা যদি( depth == CV_32F )
    {
        len *= cn;
        cn--;
        বাবে( i = j = 0; i < len; i++ )
        {
            ভিন্ন s = mean[j];
            পূর্ণাঙ্ক i0 = i - j;
            বাবে( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_NEXT(j);
        }
    }
    অন্যথা
    {
        ভিন্ন elembuf[CV_CN_MAX];
        len *= cn;
        বাবে( i = 0; i < len; i += cn )
        {
            // since we process fbuf in-place,
            // we need to copy each cn-channel element
            // prior to matrix multiplication
            বাবে (j = 0; j < cn; j++)
                elembuf[j] = fbuf[i + j];
            বাবে (j = 0; j < cn; j++) {
                ভিন্ন s = mean[j];
                বাবে( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    যদি (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    অন্যথা যদি (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    অন্যথা যদি (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

প্রকারনির্দেশ শূন্য (*RandnScaleFunc)(ভিন্ন* src, শূন্য* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn,
                               ধ্রুৱ শূন্য* mean, ধ্রুৱ শূন্য* stddev, পূর্ণাঙ্ক flags);

স্থির RandnScaleFunc randnScaleTab[CV_DEPTH_MAX] =
{
    randnScale_<uchar, ভিন্ন>, randnScale_<schar, ভিন্ন>, randnScale_<ushort, ভিন্ন>,
    randnScale_<short, ভিন্ন>, randnScale_<পূর্ণাঙ্ক, ভিন্ন>, randnScale_16_or_32f,
    randnScale_<দ্ৱিগুণ, দ্ৱিগুণ>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<তার্কিক, ভিন্ন>, randnScale_<uint64_t, দ্ৱিগুণ>, randnScale_<int64_t, দ্ৱিগুণ>,
    randnScale_<unsigned, ভিন্ন>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

শূন্য RNG::fill( প্ৰবেশ_Output_Array _mat, পূর্ণাঙ্ক disttype,
                প্ৰবেশ_Array _param1arg, প্ৰবেশ_Array _param2arg,
                তার্কিক saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    পূর্ণাঙ্ক j, depth = mat.depth(), cn = mat.channels();
    পূর্ণাঙ্ক esz1 = CV_ELEM_SIZE(depth);
    AutoBuffer<দ্ৱিগুণ> _parambuf;
    তার্কিক fast_int_mode = মিছা;
    তার্কিক small_flag = মিছা;
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

    ধ্রুৱ শূন্য* uni_param = 0;
    uchar* mean = 0;
    uchar* stddev = 0;
    তার্কিক stdmtx = মিছা;
    পূর্ণাঙ্ক n1 = (পূর্ণাঙ্ক)_param1.total();
    পূর্ণাঙ্ক n2 = (পূর্ণাঙ্ক)_param2.total();

    যদি( disttype == UNIFORM )
    {
        _parambuf.allocate(cn*(পরিমাণ(DivStruct)+পরিমাণ(দ্ৱিগুণ)-1)/পরিমাণ(দ্ৱিগুণ) + cn*4);
        দ্ৱিগুণ* parambuf = _parambuf.data();
        দ্ৱিগুণ* p1 = _param1.ptr<দ্ৱিগুণ>();
        দ্ৱিগুণ* p2 = _param2.ptr<দ্ৱিগুণ>();

        যদি( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convertTo(tmp, CV_64F);
            বাবে( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        যদি( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convertTo(tmp, CV_64F);
            বাবে( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        যদি( CV_IS_INT_TYPE(depth) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            বাবে( j = 0, fast_int_mode = সঁচা; j < cn; j++ )
            {
                দ্ৱিগুণ a = std::min(p1[j], p2[j]);
                দ্ৱিগুণ b = std::max(p1[j], p2[j]);
                যদি( saturateRange )
                {
                    a = std::max(a, depth == CV_8U || depth == CV_16U || depth == CV_32U ||
                                 depth == CV_64U || depth == CV_Bool ? 0. :
                                 depth == CV_8S ? -128. : depth == CV_16S ? -32768. :
                                 depth == CV_32S ? (দ্ৱিগুণ)INT_MIN : (দ্ৱিগুণ)INT64_MIN);
                    b = std::min(b, depth == CV_8U ? 256. : depth == CV_Bool ? 2. : depth == CV_16U ? 65536. :
                                 depth == CV_8S ? 128. : depth == CV_16S ? 32768. : depth == CV_32U ? (দ্ৱিগুণ)UINT_MAX :
                                 depth == CV_32S ? (দ্ৱিগুণ)INT_MAX : (দ্ৱিগুণ)INT64_MAX);
                }
                ip[j][1] = (int64_t)ceil(a);
                int64_t idiff = ip[j][0] = (int64_t)floor(b) - ip[j][1] - 1;
                যদি (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                দ্ৱিগুণ diff = b - a;

                fast_int_mode = fast_int_mode && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                যদি( fast_int_mode )
                    small_flag = idiff <= 255;
                অন্যথা
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    যদি (depth == CV_64S || depth == CV_64U)
                    {
                        minval = INT64_MIN/2;
                        maxval = INT64_MAX;
                    }
                    যদি( diff > (দ্ৱিগুণ)maxval )
                        ip[j][0] = maxval;
                    যদি( a < (দ্ৱিগুণ)minval )
                        ip[j][1] = minval;
                }
            }

            uni_param = ip;
            যদি( !fast_int_mode )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((শূন্য*)(ds + cn), (শূন্য*)(parambuf + _parambuf.size()), "Last byte check");
                বাবে( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    যদি (depth != CV_64U && depth != CV_64S) {
                        unsigned d = ds[j].d = (unsigned)(ip[j][0]+1);
                        পূর্ণাঙ্ক l = 0;
                        যেতিয়ালৈ(((uint64)1 << l) < d)
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
        অন্যথা
        {
            দ্ৱিগুণ scale = depth == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            দ্ৱিগুণ maxdiff = saturateRange ? (দ্ৱিগুণ)FLT_MAX : DBL_MAX;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            যদি( depth != CV_64F )
            {
                Vec2f* fp = (Vec2f*)(parambuf + cn*2);
                বাবে( j = 0; j < cn; j++ )
                {
                    fp[j][0] = (ভিন্ন)(std::min(maxdiff, p2[j] - p1[j])*scale);
                    fp[j][1] = (ভিন্ন)((p2[j] + p1[j])*0.5);
                }
                uni_param = fp;
            }
            অন্যথা
            {
                Vec2d* dp = (Vec2d*)(parambuf + cn*2);
                বাবে( j = 0; j < cn; j++ )
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
    অন্যথা যদি( disttype == RNG::NORMAL )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        দ্ৱিগুণ* parambuf = _parambuf.data();

        পূর্ণাঙ্ক ptype = esz1 == 8 ? CV_64F : CV_32F;

        যদি( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            mean = _param1.ptr();
        অন্যথা
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convertTo(tmp, ptype);
            mean = (uchar*)parambuf;
        }

        যদি( n1 < cn )
            বাবে( j = n1*esz1; j < cn*esz1; j++ )
                mean[j] = mean[j - n1*esz1];

        যদি( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        অন্যথা
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convertTo(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        যদি( n2 < cn )
            বাবে( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.rows == cn && _param2.cols == cn;
        scaleFunc = randnScaleTab[depth];
        CV_Assert( scaleFunc != 0 );
    }
    অন্যথা
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    ধ্রুৱ Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    ভিন্ন fbuf[BLOCK_SIZE + CV_CN_MAX];
    পূর্ণাঙ্ক total = (পূর্ণাঙ্ক)it.size;
    পূর্ণাঙ্ক blockSize = std::min((BLOCK_SIZE + cn - 1)/cn, total);
    size_t esz = (size_t)esz1*cn;
    পূর্ণাঙ্ক flags = mat.type();

    যদি( disttype == UNIFORM )
        flags |= (small_flag ? (পূর্ণাঙ্ক)RNG_FLAG_SMALL : 0);
    অন্যথা
        flags |= (stdmtx ? (পূর্ণাঙ্ক)RNG_FLAG_STDMTX : 0);

    বাবে( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        বাবে( j = 0; j < total; j += blockSize )
        {
            পূর্ণাঙ্ক len = std::min(total - j, blockSize);

            যদি( disttype == UNIFORM )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            অন্যথা
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
    ঘূৰাই getCoreTlsData().rng;
}

শূন্য cv::setRNGSeed(পূর্ণাঙ্ক seed)
{
    theRNG() = RNG(static_cast<uint64>(seed));
}


শূন্য cv::randu(প্ৰবেশ_Output_Array dst, প্ৰবেশ_Array নিচেৰ, প্ৰবেশ_Array উপৰেৰ)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::UNIFORM, নিচেৰ, উপৰেৰ);
}

শূন্য cv::randn(প্ৰবেশ_Output_Array dst, প্ৰবেশ_Array mean, প্ৰবেশ_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::NORMAL, mean, stddev);
}

নামাকাশ cv
{

template<typename T> স্থির শূন্য
randShuffle_( Mat& _arr, RNG& rng, দ্ৱিগুণ )
{
    unsigned sz = (unsigned)_arr.total();
    যদি( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        বাবে( unsigned i = 0; i < sz; i++ )
        {
            unsigned j = (unsigned)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    অন্যথা
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* data = _arr.ptr();
        size_t step = _arr.step;
        পূর্ণাঙ্ক rows = _arr.rows;
        পূর্ণাঙ্ক cols = _arr.cols;
        বাবে( পূর্ণাঙ্ক i0 = 0; i0 < rows; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            বাবে( পূর্ণাঙ্ক j0 = 0; j0 < cols; j0++ )
            {
                unsigned k1 = (unsigned)rng % sz;
                পূর্ণাঙ্ক i1 = (পূর্ণাঙ্ক)(k1 / cols);
                পূর্ণাঙ্ক j1 = (পূর্ণাঙ্ক)(k1 - (unsigned)i1*(unsigned)cols);
                std::swap( p[j0], ((T*)(data + step*i1))[j1] );
            }
        }
    }
}

প্রকারনির্দেশ শূন্য (*RandShuffleFunc)( Mat& dst, RNG& rng, দ্ৱিগুণ iterFactor );

}

শূন্য cv::randShuffle( প্ৰবেশ_Output_Array _dst, দ্ৱিগুণ iterFactor, RNG* _rng )
{
    CV_INSTRUMENT_REGION();

    RandShuffleFunc tab[] =
    {
        0,
        randShuffle_<uchar>, // 1
        randShuffle_<ushort>, // 2
        randShuffle_<Vec<uchar,3> >, // 3
        randShuffle_<পূর্ণাঙ্ক>, // 4
        0,
        randShuffle_<Vec<ushort,3> >, // 6
        0,
        randShuffle_<Vec<পূর্ণাঙ্ক,2> >, // 8
        0, 0, 0,
        randShuffle_<Vec<পূর্ণাঙ্ক,3> >, // 12
        0, 0, 0,
        randShuffle_<Vec<পূর্ণাঙ্ক,4> >, // 16
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<পূর্ণাঙ্ক,6> >, // 24
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<পূর্ণাঙ্ক,8> > // 32
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

শূন্য cv::RNG_MT19937::seed(unsigned s)
{
    state[0]= s;
    বাবে (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

unsigned cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    স্থির unsigned mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    ধ্রুৱ unsigned UPPER_MASK = 0x80000000U;
    ধ্রুৱ unsigned LOWER_MASK = 0x7fffffffU;

    /* generate N words at one time */
    যদি (mti >= N)
    {
        পূর্ণাঙ্ক kk = 0;

        বাবে (; kk < N - M; ++kk)
        {
            unsigned y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        বাবে (; kk < N - 1; ++kk)
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

    ঘূৰাই y;
}

cv::RNG_MT19937::operator unsigned() { ঘূৰাই next(); }

cv::RNG_MT19937::operator পূর্ণাঙ্ক() { ঘূৰাই (পূর্ণাঙ্ক)next();}

cv::RNG_MT19937::operator ভিন্ন() { ঘূৰাই next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::operator দ্ৱিগুণ()
{
    unsigned a = next() >> 5;
    unsigned b = next() >> 6;
    ঘূৰাই (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

পূর্ণাঙ্ক cv::RNG_MT19937::uniform(পূর্ণাঙ্ক a, পূর্ণাঙ্ক b) { ঘূৰাই (পূর্ণাঙ্ক)(next() % (b - a) + a); }

ভিন্ন cv::RNG_MT19937::uniform(ভিন্ন a, ভিন্ন b) { ঘূৰাই ((ভিন্ন)*এতত্)*(b - a) + a; }

দ্ৱিগুণ cv::RNG_MT19937::uniform(দ্ৱিগুণ a, দ্ৱিগুণ b) { ঘূৰাই ((দ্ৱিগুণ)*এতত্)*(b - a) + a; }

unsigned cv::RNG_MT19937::operator ()(unsigned b) { ঘূৰাই next() % b; }

unsigned cv::RNG_MT19937::operator ()() { ঘূৰাই next(); }

/* End of file. */
