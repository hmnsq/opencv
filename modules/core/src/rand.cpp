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

#அந்தர்பாவ "precomp.hpp"

நாமாகாஶ cv
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

எண்ணிக்கை
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

template<typename T> நிலையான வெறுமை
randBits_( T* arr, முழு len, முழு cn, uint64* state, மாறிலி Vec2l* p, முழு flags )
{
    தருக்க small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 temp = *state;
    முழு i, k = 0;
    len *= cn;
    --cn;

    எனில்( !small_flag )
    {
        க்கு( i = 0; i <= len - 4; i += 4 )
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
    இல்லையெனில்
    {
        க்கு( i = 0; i <= len - 4; i += 4 )
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

    க்கு( ; i < len; i++ )
    {
        int64_t t0;
        temp = RNG_NEXT(temp);

        t0 = ((int64_t)temp & p[k][0]) + p[k][1];
        k = CN_NEXT(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = temp;
}

கட்டமைப்பு DivStruct
{
    unsigned d;
    unsigned M;
    முழு sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

template<typename T> நிலையான வெறுமை
randi_( T* arr, முழு len, முழு cn, uint64* state, மாறிலி DivStruct* p )
{
    uint64 temp = *state;
    முழு k = 0;
    len *= cn;
    cn--;
    க்கு( முழு i = 0; i < len; i++ )
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

நிலையான வெறுமை
randi_( int64_t* arr, முழு len, முழு cn, uint64* state, மாறிலி DivStruct* p )
{
    uint64 temp = *state;
    முழு k = 0;
    len *= cn;
    cn--;
    க்கு( முழு i = 0; i < len; i++ )
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

நிலையான வெறுமை
randi_( uint64_t* arr, முழு len, முழு cn, uint64* state, மாறிலி DivStruct* p )
{
    uint64 temp = *state;
    முழு k = 0;
    len *= cn;
    cn--;
    க்கு( முழு i = 0; i < len; i++ )
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
நிலையான வெறுமை randBitsWrap(வெறுமை* arr, முழு len, முழு cn, uint64* state, மாறிலி வெறுமை* p, வெறுமை*, முழு flags)
{
    randBits_((T*)arr, len, cn, state, (மாறிலி Vec2l*)p, flags);
}

template<typename T>
நிலையான வெறுமை randiWrap(வெறுமை* arr, முழு len, முழு cn, uint64* state, மாறிலி வெறுமை* p, வெறுமை*, முழு)
{
    randi_((T*)arr, len, cn, state, (மாறிலி DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
நிலையான உள்வரி வெறுமை cvt32fToFP8(மாறிலி தசம* src, வெறுமை* dst, முழு len, முழு depth)
{
    எனில் (depth == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   க்கு (முழு i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    இல்லையெனில்                              { fp8a_t* d = (fp8a_t*)dst; க்கு (முழு i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
நிலையான உள்வரி தருக்க isFP8Depth(முழு d) { திரும்பு d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

நிலையான வெறுமை randf_16_or_32f( வெறுமை* dst, முழு len_, முழு cn, uint64* state, மாறிலி வெறுமை* _p, வெறுமை* _fbuf, முழு flags )
{
    மாறிலி Vec2f* p = (மாறிலி Vec2f*)_p;
    தசம* fbuf = (தசம*)_fbuf;
    முழு depth = CV_MAT_DEPTH(flags);
    uint64 temp = *state;
    முழு k = 0, len = len_*cn;
    தசம* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : (தசம*)dst;
    cn--;
    க்கு( முழு i = 0; i < len; i++ )
    {
        முழு t = (முழு)(temp = RNG_NEXT(temp));
        arr[i] = (தசம)(t*p[k][0]);
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias32f(arr, &p[0][0], len_, cn+1);
    எனில் (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    இல்லையெனில் எனில் (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    இல்லையெனில் எனில் (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

நிலையான வெறுமை
randf_64f( வெறுமை* _arr, முழு len_, முழு cn, uint64* state, மாறிலி வெறுமை* _p, வெறுமை*, முழு )
{
    இரட்டை* arr = (இரட்டை*)_arr;
    மாறிலி Vec2d* p = (மாறிலி Vec2d*)_p;
    uint64 temp = *state;
    முழு k = 0, len = len_*cn;
    cn--;
    க்கு( முழு i = 0; i < len; i++ )
    {
        temp = RNG_NEXT(temp);
        int64_t v = (int64_t)((temp >> 32) | (temp << 32));
        arr[i] = v*p[k][0];
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias64f(arr, &p[0][0], len_, cn+1);
}

வகைவரை வெறுமை (*RandFunc)(வெறுமை* arr, முழு len, முழு cn, uint64* state,
                         மாறிலி வெறுமை* p, வெறுமை* tempbuf, முழு flags);

நிலையான RandFunc randTab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<short>, randiWrap<முழு>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<தருக்க>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<unsigned>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        randBitsWrap<uchar>, randBitsWrap<schar>, randBitsWrap<ushort>,
        randBitsWrap<short>, randBitsWrap<முழு>, 0, 0, 0, 0,
        randBitsWrap<தருக்க>, randBitsWrap<uint64_t>, randBitsWrap<int64_t>,
        randBitsWrap<unsigned>, 0, 0, 0
    }
};

/*
   The code below implements the algorithm described in
   "The Ziggurat Method for Generating Random Variables"
   by George Marsaglia and Wai Wan Tsang, Journal of Statistical Software, 2007.
*/
நிலையான வெறுமை
randn_0_1_32f( தசம* arr, முழு len, uint64* state )
{
    மாறிலி தசம r = 3.442620f; // The start of the right tail
    மாறிலி தசம rng_flt = 2.3283064365386962890625e-10f; // 2^-32
    நிலையான unsigned kn[128];
    நிலையான தசம wn[128], fn[128];
    uint64 temp = *state;
    நிலையான தருக்க initialized=பொய்;
    முழு i;

    எனில்( !initialized )
    {
        மாறிலி இரட்டை m1 = 2147483648.0;
        இரட்டை dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        இரட்டை q = vn/std::exp(-.5*dn*dn);
        kn[0] = (unsigned)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (தசம)(q/m1);
        wn[127] = (தசம)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (தசம)std::exp(-.5*dn*dn);

        க்கு(i=126;i>=1;i--)
        {
            dn = std::sqrt(-2.*std::log(vn/dn+std::exp(-.5*dn*dn)));
            kn[i+1] = (unsigned)((dn/tn)*m1);
            tn = dn;
            fn[i] = (தசம)std::exp(-.5*dn*dn);
            wn[i] = (தசம)(dn/m1);
        }
        initialized = உண்மை;
    }

    க்கு( i = 0; i < len; i++ )
    {
        தசம x, y;
        க்கு(;;)
        {
            முழு hz = (முழு)temp;
            temp = RNG_NEXT(temp);
            முழு iz = hz & 127;
            x = hz*wn[iz];
            எனில்( (unsigned)std::abs(hz) < kn[iz] )
                நிறுத்து;
            எனில்( iz == 0) // iz==0, handles the base strip
            {
                செய்
                {
                    x = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    y = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    x = (தசம)(-std::log(x+FLT_MIN)*0.2904764);
                    y = (தசம)-std::log(y+FLT_MIN);
                }	// .2904764 is 1/r
                போது( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                நிறுத்து;
            }
            // iz > 0, handle the wedges of other strips
            y = (unsigned)temp*rng_flt;
            temp = RNG_NEXT(temp);
            எனில்( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::exp(-.5*x*x) )
                நிறுத்து;
        }
        arr[i] = x;
    }
    *state = temp;
}


இரட்டை RNG::gaussian(இரட்டை sigma)
{
    தசம temp;
    randn_0_1_32f( &temp, 1, &state );
    திரும்பு temp*sigma;
}

template<typename T, typename PT> நிலையான வெறுமை
randnScale_(தசம* src, வெறுமை* _dst, முழு len, முழு cn,
            மாறிலி வெறுமை* _mean, மாறிலி வெறுமை* _stddev, முழு flags )
{
    T* dst = (T*)_dst;
    மாறிலி PT* mean = (மாறிலி PT*)_mean;
    மாறிலி PT* stddev = (மாறிலி PT*)_stddev;
    தருக்க stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    முழு i, j, k;
    எனில்( !stdmtx || cn == 1 )
    {
        எனில்( cn == 1 )
        {
            PT a = stddev[0], b = mean[0];
            க்கு( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        இல்லையெனில்
        {
            len *= cn;
            cn--;
            க்கு( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + mean[k]);
                k = CN_NEXT(k);
            }
        }
    }
    இல்லையெனில்
    {
        len *= cn;
        cn--;
        க்கு( i = j = 0; i < len; i++ )
        {
            PT s = mean[j];
            முழு i0 = i - j;
            க்கு( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_NEXT(j);
        }
    }
}

// special version for 16f, 16bf and 32f
நிலையான வெறுமை
randnScale_16_or_32f(தசம* fbuf, வெறுமை* _dst, முழு len, முழு cn,
                     மாறிலி வெறுமை* _mean, மாறிலி வெறுமை* _stddev, முழு flags)
{
    தசம* dst = (தசம*)_dst;
    மாறிலி தசம* mean = (மாறிலி தசம*)_mean;
    மாறிலி தசம* stddev = (மாறிலி தசம*)_stddev;
    தருக்க stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    முழு depth = CV_MAT_DEPTH(flags);
    தசம* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : dst;
    முழு i, j, k;

    எனில்( !stdmtx || cn == 1 )
    {
        எனில்( cn == 1 )
        {
            தசம a = stddev[0], b = mean[0];
            க்கு( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        இல்லையெனில்
        {
            len *= cn;
            cn--;
            க்கு( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + mean[k];
                k = CN_NEXT(k);
            }
        }
    }
    இல்லையெனில் எனில்( depth == CV_32F )
    {
        len *= cn;
        cn--;
        க்கு( i = j = 0; i < len; i++ )
        {
            தசம s = mean[j];
            முழு i0 = i - j;
            க்கு( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_NEXT(j);
        }
    }
    இல்லையெனில்
    {
        தசம elembuf[CV_CN_MAX];
        len *= cn;
        க்கு( i = 0; i < len; i += cn )
        {
            // since we process fbuf in-place,
            // we need to copy each cn-channel element
            // prior to matrix multiplication
            க்கு (j = 0; j < cn; j++)
                elembuf[j] = fbuf[i + j];
            க்கு (j = 0; j < cn; j++) {
                தசம s = mean[j];
                க்கு( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    எனில் (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    இல்லையெனில் எனில் (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    இல்லையெனில் எனில் (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

வகைவரை வெறுமை (*RandnScaleFunc)(தசம* src, வெறுமை* dst, முழு len, முழு cn,
                               மாறிலி வெறுமை* mean, மாறிலி வெறுமை* stddev, முழு flags);

நிலையான RandnScaleFunc randnScaleTab[CV_DEPTH_MAX] =
{
    randnScale_<uchar, தசம>, randnScale_<schar, தசம>, randnScale_<ushort, தசம>,
    randnScale_<short, தசம>, randnScale_<முழு, தசம>, randnScale_16_or_32f,
    randnScale_<இரட்டை, இரட்டை>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<தருக்க, தசம>, randnScale_<uint64_t, இரட்டை>, randnScale_<int64_t, இரட்டை>,
    randnScale_<unsigned, தசம>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

வெறுமை RNG::fill( உள்ளீடு_Output_Array _mat, முழு disttype,
                உள்ளீடு_Array _param1arg, உள்ளீடு_Array _param2arg,
                தருக்க saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    முழு j, depth = mat.depth(), cn = mat.channels();
    முழு esz1 = CV_ELEM_SIZE(depth);
    AutoBuffer<இரட்டை> _parambuf;
    தருக்க fast_int_mode = பொய்;
    தருக்க small_flag = பொய்;
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

    மாறிலி வெறுமை* uni_param = 0;
    uchar* mean = 0;
    uchar* stddev = 0;
    தருக்க stdmtx = பொய்;
    முழு n1 = (முழு)_param1.total();
    முழு n2 = (முழு)_param2.total();

    எனில்( disttype == UNIFORM )
    {
        _parambuf.allocate(cn*(அளவு(DivStruct)+அளவு(இரட்டை)-1)/அளவு(இரட்டை) + cn*4);
        இரட்டை* parambuf = _parambuf.data();
        இரட்டை* p1 = _param1.ptr<இரட்டை>();
        இரட்டை* p2 = _param2.ptr<இரட்டை>();

        எனில்( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convertTo(tmp, CV_64F);
            க்கு( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        எனில்( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convertTo(tmp, CV_64F);
            க்கு( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        எனில்( CV_IS_INT_TYPE(depth) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            க்கு( j = 0, fast_int_mode = உண்மை; j < cn; j++ )
            {
                இரட்டை a = std::min(p1[j], p2[j]);
                இரட்டை b = std::max(p1[j], p2[j]);
                எனில்( saturateRange )
                {
                    a = std::max(a, depth == CV_8U || depth == CV_16U || depth == CV_32U ||
                                 depth == CV_64U || depth == CV_Bool ? 0. :
                                 depth == CV_8S ? -128. : depth == CV_16S ? -32768. :
                                 depth == CV_32S ? (இரட்டை)INT_MIN : (இரட்டை)INT64_MIN);
                    b = std::min(b, depth == CV_8U ? 256. : depth == CV_Bool ? 2. : depth == CV_16U ? 65536. :
                                 depth == CV_8S ? 128. : depth == CV_16S ? 32768. : depth == CV_32U ? (இரட்டை)UINT_MAX :
                                 depth == CV_32S ? (இரட்டை)INT_MAX : (இரட்டை)INT64_MAX);
                }
                ip[j][1] = (int64_t)ceil(a);
                int64_t idiff = ip[j][0] = (int64_t)floor(b) - ip[j][1] - 1;
                எனில் (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                இரட்டை diff = b - a;

                fast_int_mode = fast_int_mode && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                எனில்( fast_int_mode )
                    small_flag = idiff <= 255;
                இல்லையெனில்
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    எனில் (depth == CV_64S || depth == CV_64U)
                    {
                        minval = INT64_MIN/2;
                        maxval = INT64_MAX;
                    }
                    எனில்( diff > (இரட்டை)maxval )
                        ip[j][0] = maxval;
                    எனில்( a < (இரட்டை)minval )
                        ip[j][1] = minval;
                }
            }

            uni_param = ip;
            எனில்( !fast_int_mode )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((வெறுமை*)(ds + cn), (வெறுமை*)(parambuf + _parambuf.size()), "Last byte check");
                க்கு( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    எனில் (depth != CV_64U && depth != CV_64S) {
                        unsigned d = ds[j].d = (unsigned)(ip[j][0]+1);
                        முழு l = 0;
                        போது(((uint64)1 << l) < d)
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
        இல்லையெனில்
        {
            இரட்டை scale = depth == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            இரட்டை maxdiff = saturateRange ? (இரட்டை)FLT_MAX : DBL_MAX;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            எனில்( depth != CV_64F )
            {
                Vec2f* fp = (Vec2f*)(parambuf + cn*2);
                க்கு( j = 0; j < cn; j++ )
                {
                    fp[j][0] = (தசம)(std::min(maxdiff, p2[j] - p1[j])*scale);
                    fp[j][1] = (தசம)((p2[j] + p1[j])*0.5);
                }
                uni_param = fp;
            }
            இல்லையெனில்
            {
                Vec2d* dp = (Vec2d*)(parambuf + cn*2);
                க்கு( j = 0; j < cn; j++ )
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
    இல்லையெனில் எனில்( disttype == RNG::NORMAL )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        இரட்டை* parambuf = _parambuf.data();

        முழு ptype = esz1 == 8 ? CV_64F : CV_32F;

        எனில்( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            mean = _param1.ptr();
        இல்லையெனில்
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convertTo(tmp, ptype);
            mean = (uchar*)parambuf;
        }

        எனில்( n1 < cn )
            க்கு( j = n1*esz1; j < cn*esz1; j++ )
                mean[j] = mean[j - n1*esz1];

        எனில்( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        இல்லையெனில்
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convertTo(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        எனில்( n2 < cn )
            க்கு( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.rows == cn && _param2.cols == cn;
        scaleFunc = randnScaleTab[depth];
        CV_Assert( scaleFunc != 0 );
    }
    இல்லையெனில்
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    மாறிலி Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    தசம fbuf[BLOCK_SIZE + CV_CN_MAX];
    முழு total = (முழு)it.size;
    முழு blockSize = std::min((BLOCK_SIZE + cn - 1)/cn, total);
    size_t esz = (size_t)esz1*cn;
    முழு flags = mat.type();

    எனில்( disttype == UNIFORM )
        flags |= (small_flag ? (முழு)RNG_FLAG_SMALL : 0);
    இல்லையெனில்
        flags |= (stdmtx ? (முழு)RNG_FLAG_STDMTX : 0);

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        க்கு( j = 0; j < total; j += blockSize )
        {
            முழு len = std::min(total - j, blockSize);

            எனில்( disttype == UNIFORM )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            இல்லையெனில்
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
    திரும்பு getCoreTlsData().rng;
}

வெறுமை cv::setRNGSeed(முழு seed)
{
    theRNG() = RNG(static_cast<uint64>(seed));
}


வெறுமை cv::randu(உள்ளீடு_Output_Array dst, உள்ளீடு_Array கீழ், உள்ளீடு_Array உயர்வு)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::UNIFORM, கீழ், உயர்வு);
}

வெறுமை cv::randn(உள்ளீடு_Output_Array dst, உள்ளீடு_Array mean, உள்ளீடு_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::NORMAL, mean, stddev);
}

நாமாகாஶ cv
{

template<typename T> நிலையான வெறுமை
randShuffle_( Mat& _arr, RNG& rng, இரட்டை )
{
    unsigned sz = (unsigned)_arr.total();
    எனில்( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        க்கு( unsigned i = 0; i < sz; i++ )
        {
            unsigned j = (unsigned)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    இல்லையெனில்
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* data = _arr.ptr();
        size_t step = _arr.step;
        முழு rows = _arr.rows;
        முழு cols = _arr.cols;
        க்கு( முழு i0 = 0; i0 < rows; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            க்கு( முழு j0 = 0; j0 < cols; j0++ )
            {
                unsigned k1 = (unsigned)rng % sz;
                முழு i1 = (முழு)(k1 / cols);
                முழு j1 = (முழு)(k1 - (unsigned)i1*(unsigned)cols);
                std::swap( p[j0], ((T*)(data + step*i1))[j1] );
            }
        }
    }
}

வகைவரை வெறுமை (*RandShuffleFunc)( Mat& dst, RNG& rng, இரட்டை iterFactor );

}

வெறுமை cv::randShuffle( உள்ளீடு_Output_Array _dst, இரட்டை iterFactor, RNG* _rng )
{
    CV_INSTRUMENT_REGION();

    RandShuffleFunc tab[] =
    {
        0,
        randShuffle_<uchar>, // 1
        randShuffle_<ushort>, // 2
        randShuffle_<Vec<uchar,3> >, // 3
        randShuffle_<முழு>, // 4
        0,
        randShuffle_<Vec<ushort,3> >, // 6
        0,
        randShuffle_<Vec<முழு,2> >, // 8
        0, 0, 0,
        randShuffle_<Vec<முழு,3> >, // 12
        0, 0, 0,
        randShuffle_<Vec<முழு,4> >, // 16
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<முழு,6> >, // 24
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<முழு,8> > // 32
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

வெறுமை cv::RNG_MT19937::seed(unsigned s)
{
    state[0]= s;
    க்கு (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

unsigned cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    நிலையான unsigned mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    மாறிலி unsigned UPPER_MASK = 0x80000000U;
    மாறிலி unsigned LOWER_MASK = 0x7fffffffU;

    /* generate N words at one time */
    எனில் (mti >= N)
    {
        முழு kk = 0;

        க்கு (; kk < N - M; ++kk)
        {
            unsigned y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        க்கு (; kk < N - 1; ++kk)
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

    திரும்பு y;
}

cv::RNG_MT19937::operator unsigned() { திரும்பு next(); }

cv::RNG_MT19937::operator முழு() { திரும்பு (முழு)next();}

cv::RNG_MT19937::operator தசம() { திரும்பு next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::operator இரட்டை()
{
    unsigned a = next() >> 5;
    unsigned b = next() >> 6;
    திரும்பு (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

முழு cv::RNG_MT19937::uniform(முழு a, முழு b) { திரும்பு (முழு)(next() % (b - a) + a); }

தசம cv::RNG_MT19937::uniform(தசம a, தசம b) { திரும்பு ((தசம)*இது)*(b - a) + a; }

இரட்டை cv::RNG_MT19937::uniform(இரட்டை a, இரட்டை b) { திரும்பு ((இரட்டை)*இது)*(b - a) + a; }

unsigned cv::RNG_MT19937::operator ()(unsigned b) { திரும்பு next() % b; }

unsigned cv::RNG_MT19937::operator ()() { திரும்பு next(); }

/* End of file. */
