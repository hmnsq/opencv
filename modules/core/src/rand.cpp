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

#شامل "precomp.hpp"

فضاینام cv
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

شمارش
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

template<typename T> ایستا تهی
randBits_( T* arr, صحیح len, صحیح cn, uint64* state, ثابت Vec2l* p, صحیح flags )
{
    منطقی small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 temp = *state;
    صحیح i, k = 0;
    len *= cn;
    --cn;

    اگر( !small_flag )
    {
        برای( i = 0; i <= len - 4; i += 4 )
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
    وگرنه
    {
        برای( i = 0; i <= len - 4; i += 4 )
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

    برای( ; i < len; i++ )
    {
        int64_t t0;
        temp = RNG_NEXT(temp);

        t0 = ((int64_t)temp & p[k][0]) + p[k][1];
        k = CN_NEXT(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = temp;
}

ساختار DivStruct
{
    unsigned d;
    unsigned M;
    صحیح sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

template<typename T> ایستا تهی
randi_( T* arr, صحیح len, صحیح cn, uint64* state, ثابت DivStruct* p )
{
    uint64 temp = *state;
    صحیح k = 0;
    len *= cn;
    cn--;
    برای( صحیح i = 0; i < len; i++ )
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

ایستا تهی
randi_( int64_t* arr, صحیح len, صحیح cn, uint64* state, ثابت DivStruct* p )
{
    uint64 temp = *state;
    صحیح k = 0;
    len *= cn;
    cn--;
    برای( صحیح i = 0; i < len; i++ )
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

ایستا تهی
randi_( uint64_t* arr, صحیح len, صحیح cn, uint64* state, ثابت DivStruct* p )
{
    uint64 temp = *state;
    صحیح k = 0;
    len *= cn;
    cn--;
    برای( صحیح i = 0; i < len; i++ )
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
ایستا تهی randBitsWrap(تهی* arr, صحیح len, صحیح cn, uint64* state, ثابت تهی* p, تهی*, صحیح flags)
{
    randBits_((T*)arr, len, cn, state, (ثابت Vec2l*)p, flags);
}

template<typename T>
ایستا تهی randiWrap(تهی* arr, صحیح len, صحیح cn, uint64* state, ثابت تهی* p, تهی*, صحیح)
{
    randi_((T*)arr, len, cn, state, (ثابت DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
ایستا درونخطی تهی cvt32fToFP8(ثابت اعشاری* src, تهی* dst, صحیح len, صحیح depth)
{
    اگر (depth == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   برای (صحیح i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    وگرنه                              { fp8a_t* d = (fp8a_t*)dst; برای (صحیح i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
ایستا درونخطی منطقی isFP8Depth(صحیح d) { بازگشت d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

ایستا تهی randf_16_or_32f( تهی* dst, صحیح len_, صحیح cn, uint64* state, ثابت تهی* _p, تهی* _fbuf, صحیح flags )
{
    ثابت Vec2f* p = (ثابت Vec2f*)_p;
    اعشاری* fbuf = (اعشاری*)_fbuf;
    صحیح depth = CV_MAT_DEPTH(flags);
    uint64 temp = *state;
    صحیح k = 0, len = len_*cn;
    اعشاری* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : (اعشاری*)dst;
    cn--;
    برای( صحیح i = 0; i < len; i++ )
    {
        صحیح t = (صحیح)(temp = RNG_NEXT(temp));
        arr[i] = (اعشاری)(t*p[k][0]);
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias32f(arr, &p[0][0], len_, cn+1);
    اگر (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    وگرنه اگر (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    وگرنه اگر (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

ایستا تهی
randf_64f( تهی* _arr, صحیح len_, صحیح cn, uint64* state, ثابت تهی* _p, تهی*, صحیح )
{
    دوگانه* arr = (دوگانه*)_arr;
    ثابت Vec2d* p = (ثابت Vec2d*)_p;
    uint64 temp = *state;
    صحیح k = 0, len = len_*cn;
    cn--;
    برای( صحیح i = 0; i < len; i++ )
    {
        temp = RNG_NEXT(temp);
        int64_t v = (int64_t)((temp >> 32) | (temp << 32));
        arr[i] = v*p[k][0];
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias64f(arr, &p[0][0], len_, cn+1);
}

تعریفنوع تهی (*RandFunc)(تهی* arr, صحیح len, صحیح cn, uint64* state,
                         ثابت تهی* p, تهی* tempbuf, صحیح flags);

ایستا RandFunc randTab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<short>, randiWrap<صحیح>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<منطقی>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<unsigned>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        randBitsWrap<uchar>, randBitsWrap<schar>, randBitsWrap<ushort>,
        randBitsWrap<short>, randBitsWrap<صحیح>, 0, 0, 0, 0,
        randBitsWrap<منطقی>, randBitsWrap<uint64_t>, randBitsWrap<int64_t>,
        randBitsWrap<unsigned>, 0, 0, 0
    }
};

/*
   The code below implements the algorithm described in
   "The Ziggurat Method for Generating Random Variables"
   by George Marsaglia and Wai Wan Tsang, Journal of Statistical Software, 2007.
*/
ایستا تهی
randn_0_1_32f( اعشاری* arr, صحیح len, uint64* state )
{
    ثابت اعشاری r = 3.442620f; // The start of the right tail
    ثابت اعشاری rng_flt = 2.3283064365386962890625e-10f; // 2^-32
    ایستا unsigned kn[128];
    ایستا اعشاری wn[128], fn[128];
    uint64 temp = *state;
    ایستا منطقی initialized=نادرست;
    صحیح i;

    اگر( !initialized )
    {
        ثابت دوگانه m1 = 2147483648.0;
        دوگانه dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        دوگانه q = vn/std::exp(-.5*dn*dn);
        kn[0] = (unsigned)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (اعشاری)(q/m1);
        wn[127] = (اعشاری)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (اعشاری)std::exp(-.5*dn*dn);

        برای(i=126;i>=1;i--)
        {
            dn = std::sqrt(-2.*std::log(vn/dn+std::exp(-.5*dn*dn)));
            kn[i+1] = (unsigned)((dn/tn)*m1);
            tn = dn;
            fn[i] = (اعشاری)std::exp(-.5*dn*dn);
            wn[i] = (اعشاری)(dn/m1);
        }
        initialized = درست;
    }

    برای( i = 0; i < len; i++ )
    {
        اعشاری x, y;
        برای(;;)
        {
            صحیح hz = (صحیح)temp;
            temp = RNG_NEXT(temp);
            صحیح iz = hz & 127;
            x = hz*wn[iz];
            اگر( (unsigned)std::abs(hz) < kn[iz] )
                بشکن;
            اگر( iz == 0) // iz==0, handles the base strip
            {
                انجامبده
                {
                    x = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    y = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    x = (اعشاری)(-std::log(x+FLT_MIN)*0.2904764);
                    y = (اعشاری)-std::log(y+FLT_MIN);
                }	// .2904764 is 1/r
                وقتی( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                بشکن;
            }
            // iz > 0, handle the wedges of other strips
            y = (unsigned)temp*rng_flt;
            temp = RNG_NEXT(temp);
            اگر( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::exp(-.5*x*x) )
                بشکن;
        }
        arr[i] = x;
    }
    *state = temp;
}


دوگانه RNG::gaussian(دوگانه sigma)
{
    اعشاری temp;
    randn_0_1_32f( &temp, 1, &state );
    بازگشت temp*sigma;
}

template<typename T, typename PT> ایستا تهی
randnScale_(اعشاری* src, تهی* _dst, صحیح len, صحیح cn,
            ثابت تهی* _mean, ثابت تهی* _stddev, صحیح flags )
{
    T* dst = (T*)_dst;
    ثابت PT* mean = (ثابت PT*)_mean;
    ثابت PT* stddev = (ثابت PT*)_stddev;
    منطقی stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    صحیح i, j, k;
    اگر( !stdmtx || cn == 1 )
    {
        اگر( cn == 1 )
        {
            PT a = stddev[0], b = mean[0];
            برای( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        وگرنه
        {
            len *= cn;
            cn--;
            برای( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + mean[k]);
                k = CN_NEXT(k);
            }
        }
    }
    وگرنه
    {
        len *= cn;
        cn--;
        برای( i = j = 0; i < len; i++ )
        {
            PT s = mean[j];
            صحیح i0 = i - j;
            برای( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_NEXT(j);
        }
    }
}

// special version for 16f, 16bf and 32f
ایستا تهی
randnScale_16_or_32f(اعشاری* fbuf, تهی* _dst, صحیح len, صحیح cn,
                     ثابت تهی* _mean, ثابت تهی* _stddev, صحیح flags)
{
    اعشاری* dst = (اعشاری*)_dst;
    ثابت اعشاری* mean = (ثابت اعشاری*)_mean;
    ثابت اعشاری* stddev = (ثابت اعشاری*)_stddev;
    منطقی stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    صحیح depth = CV_MAT_DEPTH(flags);
    اعشاری* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : dst;
    صحیح i, j, k;

    اگر( !stdmtx || cn == 1 )
    {
        اگر( cn == 1 )
        {
            اعشاری a = stddev[0], b = mean[0];
            برای( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        وگرنه
        {
            len *= cn;
            cn--;
            برای( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + mean[k];
                k = CN_NEXT(k);
            }
        }
    }
    وگرنه اگر( depth == CV_32F )
    {
        len *= cn;
        cn--;
        برای( i = j = 0; i < len; i++ )
        {
            اعشاری s = mean[j];
            صحیح i0 = i - j;
            برای( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_NEXT(j);
        }
    }
    وگرنه
    {
        اعشاری elembuf[CV_CN_MAX];
        len *= cn;
        برای( i = 0; i < len; i += cn )
        {
            // since we process fbuf in-place,
            // we need to copy each cn-channel element
            // prior to matrix multiplication
            برای (j = 0; j < cn; j++)
                elembuf[j] = fbuf[i + j];
            برای (j = 0; j < cn; j++) {
                اعشاری s = mean[j];
                برای( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    اگر (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    وگرنه اگر (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    وگرنه اگر (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

تعریفنوع تهی (*RandnScaleFunc)(اعشاری* src, تهی* dst, صحیح len, صحیح cn,
                               ثابت تهی* mean, ثابت تهی* stddev, صحیح flags);

ایستا RandnScaleFunc randnScaleTab[CV_DEPTH_MAX] =
{
    randnScale_<uchar, اعشاری>, randnScale_<schar, اعشاری>, randnScale_<ushort, اعشاری>,
    randnScale_<short, اعشاری>, randnScale_<صحیح, اعشاری>, randnScale_16_or_32f,
    randnScale_<دوگانه, دوگانه>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<منطقی, اعشاری>, randnScale_<uint64_t, دوگانه>, randnScale_<int64_t, دوگانه>,
    randnScale_<unsigned, اعشاری>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

تهی RNG::fill( درون_داد_Output_Array _mat, صحیح disttype,
                درون_داد_Array _param1arg, درون_داد_Array _param2arg,
                منطقی saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    صحیح j, depth = mat.depth(), cn = mat.channels();
    صحیح esz1 = CV_ELEM_SIZE(depth);
    AutoBuffer<دوگانه> _parambuf;
    منطقی fast_int_mode = نادرست;
    منطقی small_flag = نادرست;
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

    ثابت تهی* uni_param = 0;
    uchar* mean = 0;
    uchar* stddev = 0;
    منطقی stdmtx = نادرست;
    صحیح n1 = (صحیح)_param1.total();
    صحیح n2 = (صحیح)_param2.total();

    اگر( disttype == UNIFORM )
    {
        _parambuf.allocate(cn*(اندازه(DivStruct)+اندازه(دوگانه)-1)/اندازه(دوگانه) + cn*4);
        دوگانه* parambuf = _parambuf.data();
        دوگانه* p1 = _param1.ptr<دوگانه>();
        دوگانه* p2 = _param2.ptr<دوگانه>();

        اگر( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convertTo(tmp, CV_64F);
            برای( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        اگر( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convertTo(tmp, CV_64F);
            برای( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        اگر( CV_IS_INT_TYPE(depth) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            برای( j = 0, fast_int_mode = درست; j < cn; j++ )
            {
                دوگانه a = std::min(p1[j], p2[j]);
                دوگانه b = std::max(p1[j], p2[j]);
                اگر( saturateRange )
                {
                    a = std::max(a, depth == CV_8U || depth == CV_16U || depth == CV_32U ||
                                 depth == CV_64U || depth == CV_Bool ? 0. :
                                 depth == CV_8S ? -128. : depth == CV_16S ? -32768. :
                                 depth == CV_32S ? (دوگانه)INT_MIN : (دوگانه)INT64_MIN);
                    b = std::min(b, depth == CV_8U ? 256. : depth == CV_Bool ? 2. : depth == CV_16U ? 65536. :
                                 depth == CV_8S ? 128. : depth == CV_16S ? 32768. : depth == CV_32U ? (دوگانه)UINT_MAX :
                                 depth == CV_32S ? (دوگانه)INT_MAX : (دوگانه)INT64_MAX);
                }
                ip[j][1] = (int64_t)ceil(a);
                int64_t idiff = ip[j][0] = (int64_t)floor(b) - ip[j][1] - 1;
                اگر (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                دوگانه diff = b - a;

                fast_int_mode = fast_int_mode && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                اگر( fast_int_mode )
                    small_flag = idiff <= 255;
                وگرنه
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    اگر (depth == CV_64S || depth == CV_64U)
                    {
                        minval = INT64_MIN/2;
                        maxval = INT64_MAX;
                    }
                    اگر( diff > (دوگانه)maxval )
                        ip[j][0] = maxval;
                    اگر( a < (دوگانه)minval )
                        ip[j][1] = minval;
                }
            }

            uni_param = ip;
            اگر( !fast_int_mode )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((تهی*)(ds + cn), (تهی*)(parambuf + _parambuf.size()), "Last byte check");
                برای( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    اگر (depth != CV_64U && depth != CV_64S) {
                        unsigned d = ds[j].d = (unsigned)(ip[j][0]+1);
                        صحیح l = 0;
                        وقتی(((uint64)1 << l) < d)
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
        وگرنه
        {
            دوگانه scale = depth == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            دوگانه maxdiff = saturateRange ? (دوگانه)FLT_MAX : DBL_MAX;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            اگر( depth != CV_64F )
            {
                Vec2f* fp = (Vec2f*)(parambuf + cn*2);
                برای( j = 0; j < cn; j++ )
                {
                    fp[j][0] = (اعشاری)(std::min(maxdiff, p2[j] - p1[j])*scale);
                    fp[j][1] = (اعشاری)((p2[j] + p1[j])*0.5);
                }
                uni_param = fp;
            }
            وگرنه
            {
                Vec2d* dp = (Vec2d*)(parambuf + cn*2);
                برای( j = 0; j < cn; j++ )
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
    وگرنه اگر( disttype == RNG::NORMAL )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        دوگانه* parambuf = _parambuf.data();

        صحیح ptype = esz1 == 8 ? CV_64F : CV_32F;

        اگر( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            mean = _param1.ptr();
        وگرنه
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convertTo(tmp, ptype);
            mean = (uchar*)parambuf;
        }

        اگر( n1 < cn )
            برای( j = n1*esz1; j < cn*esz1; j++ )
                mean[j] = mean[j - n1*esz1];

        اگر( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        وگرنه
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convertTo(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        اگر( n2 < cn )
            برای( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.rows == cn && _param2.cols == cn;
        scaleFunc = randnScaleTab[depth];
        CV_Assert( scaleFunc != 0 );
    }
    وگرنه
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    ثابت Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    اعشاری fbuf[BLOCK_SIZE + CV_CN_MAX];
    صحیح total = (صحیح)it.size;
    صحیح blockSize = std::min((BLOCK_SIZE + cn - 1)/cn, total);
    size_t esz = (size_t)esz1*cn;
    صحیح flags = mat.type();

    اگر( disttype == UNIFORM )
        flags |= (small_flag ? (صحیح)RNG_FLAG_SMALL : 0);
    وگرنه
        flags |= (stdmtx ? (صحیح)RNG_FLAG_STDMTX : 0);

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برای( j = 0; j < total; j += blockSize )
        {
            صحیح len = std::min(total - j, blockSize);

            اگر( disttype == UNIFORM )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            وگرنه
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
    بازگشت getCoreTlsData().rng;
}

تهی cv::setRNGSeed(صحیح seed)
{
    theRNG() = RNG(static_cast<uint64>(seed));
}


تهی cv::randu(درون_داد_Output_Array dst, درون_داد_Array کم, درون_داد_Array بالا)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::UNIFORM, کم, بالا);
}

تهی cv::randn(درون_داد_Output_Array dst, درون_داد_Array mean, درون_داد_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::NORMAL, mean, stddev);
}

فضاینام cv
{

template<typename T> ایستا تهی
randShuffle_( Mat& _arr, RNG& rng, دوگانه )
{
    unsigned sz = (unsigned)_arr.total();
    اگر( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        برای( unsigned i = 0; i < sz; i++ )
        {
            unsigned j = (unsigned)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    وگرنه
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* data = _arr.ptr();
        size_t step = _arr.step;
        صحیح rows = _arr.rows;
        صحیح cols = _arr.cols;
        برای( صحیح i0 = 0; i0 < rows; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            برای( صحیح j0 = 0; j0 < cols; j0++ )
            {
                unsigned k1 = (unsigned)rng % sz;
                صحیح i1 = (صحیح)(k1 / cols);
                صحیح j1 = (صحیح)(k1 - (unsigned)i1*(unsigned)cols);
                std::swap( p[j0], ((T*)(data + step*i1))[j1] );
            }
        }
    }
}

تعریفنوع تهی (*RandShuffleFunc)( Mat& dst, RNG& rng, دوگانه iterFactor );

}

تهی cv::randShuffle( درون_داد_Output_Array _dst, دوگانه iterFactor, RNG* _rng )
{
    CV_INSTRUMENT_REGION();

    RandShuffleFunc tab[] =
    {
        0,
        randShuffle_<uchar>, // 1
        randShuffle_<ushort>, // 2
        randShuffle_<Vec<uchar,3> >, // 3
        randShuffle_<صحیح>, // 4
        0,
        randShuffle_<Vec<ushort,3> >, // 6
        0,
        randShuffle_<Vec<صحیح,2> >, // 8
        0, 0, 0,
        randShuffle_<Vec<صحیح,3> >, // 12
        0, 0, 0,
        randShuffle_<Vec<صحیح,4> >, // 16
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<صحیح,6> >, // 24
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<صحیح,8> > // 32
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

تهی cv::RNG_MT19937::seed(unsigned s)
{
    state[0]= s;
    برای (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

unsigned cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    ایستا unsigned mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    ثابت unsigned UPPER_MASK = 0x80000000U;
    ثابت unsigned LOWER_MASK = 0x7fffffffU;

    /* generate N words at one time */
    اگر (mti >= N)
    {
        صحیح kk = 0;

        برای (; kk < N - M; ++kk)
        {
            unsigned y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        برای (; kk < N - 1; ++kk)
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

    بازگشت y;
}

cv::RNG_MT19937::operator unsigned() { بازگشت next(); }

cv::RNG_MT19937::operator صحیح() { بازگشت (صحیح)next();}

cv::RNG_MT19937::operator اعشاری() { بازگشت next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::operator دوگانه()
{
    unsigned a = next() >> 5;
    unsigned b = next() >> 6;
    بازگشت (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

صحیح cv::RNG_MT19937::uniform(صحیح a, صحیح b) { بازگشت (صحیح)(next() % (b - a) + a); }

اعشاری cv::RNG_MT19937::uniform(اعشاری a, اعشاری b) { بازگشت ((اعشاری)*این)*(b - a) + a; }

دوگانه cv::RNG_MT19937::uniform(دوگانه a, دوگانه b) { بازگشت ((دوگانه)*این)*(b - a) + a; }

unsigned cv::RNG_MT19937::operator ()(unsigned b) { بازگشت next() % b; }

unsigned cv::RNG_MT19937::operator ()() { بازگشت next(); }

/* End of file. */
