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

#ضمن "precomp.hpp"

نطاق cv
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

تعداد
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

template<typename T> ساكن فراغ
randBits_( T* arr, صحيح len, صحيح cn, uint64* state, ثابت Vec2l* p, صحيح flags )
{
    منطقي small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 temp = *state;
    صحيح i, k = 0;
    len *= cn;
    --cn;

    إذا( !small_flag )
    {
        لكل( i = 0; i <= len - 4; i += 4 )
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
    وإلا
    {
        لكل( i = 0; i <= len - 4; i += 4 )
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

    لكل( ; i < len; i++ )
    {
        int64_t t0;
        temp = RNG_NEXT(temp);

        t0 = ((int64_t)temp & p[k][0]) + p[k][1];
        k = CN_NEXT(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = temp;
}

بنية DivStruct
{
    unsigned d;
    unsigned M;
    صحيح sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

template<typename T> ساكن فراغ
randi_( T* arr, صحيح len, صحيح cn, uint64* state, ثابت DivStruct* p )
{
    uint64 temp = *state;
    صحيح k = 0;
    len *= cn;
    cn--;
    لكل( صحيح i = 0; i < len; i++ )
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

ساكن فراغ
randi_( int64_t* arr, صحيح len, صحيح cn, uint64* state, ثابت DivStruct* p )
{
    uint64 temp = *state;
    صحيح k = 0;
    len *= cn;
    cn--;
    لكل( صحيح i = 0; i < len; i++ )
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

ساكن فراغ
randi_( uint64_t* arr, صحيح len, صحيح cn, uint64* state, ثابت DivStruct* p )
{
    uint64 temp = *state;
    صحيح k = 0;
    len *= cn;
    cn--;
    لكل( صحيح i = 0; i < len; i++ )
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
ساكن فراغ randBitsWrap(فراغ* arr, صحيح len, صحيح cn, uint64* state, ثابت فراغ* p, فراغ*, صحيح flags)
{
    randBits_((T*)arr, len, cn, state, (ثابت Vec2l*)p, flags);
}

template<typename T>
ساكن فراغ randiWrap(فراغ* arr, صحيح len, صحيح cn, uint64* state, ثابت فراغ* p, فراغ*, صحيح)
{
    randi_((T*)arr, len, cn, state, (ثابت DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
ساكن مضمن فراغ cvt32fToFP8(ثابت عائم* src, فراغ* dst, صحيح len, صحيح depth)
{
    إذا (depth == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   لكل (صحيح i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    وإلا                              { fp8a_t* d = (fp8a_t*)dst; لكل (صحيح i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
ساكن مضمن منطقي isFP8Depth(صحيح d) { أرجع d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

ساكن فراغ randf_16_or_32f( فراغ* dst, صحيح len_, صحيح cn, uint64* state, ثابت فراغ* _p, فراغ* _fbuf, صحيح flags )
{
    ثابت Vec2f* p = (ثابت Vec2f*)_p;
    عائم* fbuf = (عائم*)_fbuf;
    صحيح depth = CV_MAT_DEPTH(flags);
    uint64 temp = *state;
    صحيح k = 0, len = len_*cn;
    عائم* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : (عائم*)dst;
    cn--;
    لكل( صحيح i = 0; i < len; i++ )
    {
        صحيح t = (صحيح)(temp = RNG_NEXT(temp));
        arr[i] = (عائم)(t*p[k][0]);
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias32f(arr, &p[0][0], len_, cn+1);
    إذا (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    وإلا إذا (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    وإلا إذا (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

ساكن فراغ
randf_64f( فراغ* _arr, صحيح len_, صحيح cn, uint64* state, ثابت فراغ* _p, فراغ*, صحيح )
{
    مزدوج* arr = (مزدوج*)_arr;
    ثابت Vec2d* p = (ثابت Vec2d*)_p;
    uint64 temp = *state;
    صحيح k = 0, len = len_*cn;
    cn--;
    لكل( صحيح i = 0; i < len; i++ )
    {
        temp = RNG_NEXT(temp);
        int64_t v = (int64_t)((temp >> 32) | (temp << 32));
        arr[i] = v*p[k][0];
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias64f(arr, &p[0][0], len_, cn+1);
}

تعريفنوع فراغ (*RandFunc)(فراغ* arr, صحيح len, صحيح cn, uint64* state,
                         ثابت فراغ* p, فراغ* tempbuf, صحيح flags);

ساكن RandFunc randTab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<short>, randiWrap<صحيح>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<منطقي>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<unsigned>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        randBitsWrap<uchar>, randBitsWrap<schar>, randBitsWrap<ushort>,
        randBitsWrap<short>, randBitsWrap<صحيح>, 0, 0, 0, 0,
        randBitsWrap<منطقي>, randBitsWrap<uint64_t>, randBitsWrap<int64_t>,
        randBitsWrap<unsigned>, 0, 0, 0
    }
};

/*
   The code below implements the algorithm described in
   "The Ziggurat Method for Generating Random Variables"
   by George Marsaglia and Wai Wan Tsang, Journal of Statistical Software, 2007.
*/
ساكن فراغ
randn_0_1_32f( عائم* arr, صحيح len, uint64* state )
{
    ثابت عائم r = 3.442620f; // The start of the right tail
    ثابت عائم rng_flt = 2.3283064365386962890625e-10f; // 2^-32
    ساكن unsigned kn[128];
    ساكن عائم wn[128], fn[128];
    uint64 temp = *state;
    ساكن منطقي initialized=خطأ;
    صحيح i;

    إذا( !initialized )
    {
        ثابت مزدوج m1 = 2147483648.0;
        مزدوج dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        مزدوج q = vn/std::exp(-.5*dn*dn);
        kn[0] = (unsigned)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (عائم)(q/m1);
        wn[127] = (عائم)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (عائم)std::exp(-.5*dn*dn);

        لكل(i=126;i>=1;i--)
        {
            dn = std::sqrt(-2.*std::log(vn/dn+std::exp(-.5*dn*dn)));
            kn[i+1] = (unsigned)((dn/tn)*m1);
            tn = dn;
            fn[i] = (عائم)std::exp(-.5*dn*dn);
            wn[i] = (عائم)(dn/m1);
        }
        initialized = true;
    }

    لكل( i = 0; i < len; i++ )
    {
        عائم x, y;
        لكل(;;)
        {
            صحيح hz = (صحيح)temp;
            temp = RNG_NEXT(temp);
            صحيح iz = hz & 127;
            x = hz*wn[iz];
            إذا( (unsigned)std::abs(hz) < kn[iz] )
                اكسر;
            إذا( iz == 0) // iz==0, handles the base strip
            {
                افعل
                {
                    x = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    y = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    x = (عائم)(-std::log(x+FLT_MIN)*0.2904764);
                    y = (عائم)-std::log(y+FLT_MIN);
                }	// .2904764 is 1/r
                طالما( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                اكسر;
            }
            // iz > 0, handle the wedges of other strips
            y = (unsigned)temp*rng_flt;
            temp = RNG_NEXT(temp);
            إذا( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::exp(-.5*x*x) )
                اكسر;
        }
        arr[i] = x;
    }
    *state = temp;
}


مزدوج RNG::gaussian(مزدوج sigma)
{
    عائم temp;
    randn_0_1_32f( &temp, 1, &state );
    أرجع temp*sigma;
}

template<typename T, typename PT> ساكن فراغ
randnScale_(عائم* src, فراغ* _dst, صحيح len, صحيح cn,
            ثابت فراغ* _mean, ثابت فراغ* _stddev, صحيح flags )
{
    T* dst = (T*)_dst;
    ثابت PT* mean = (ثابت PT*)_mean;
    ثابت PT* stddev = (ثابت PT*)_stddev;
    منطقي stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    صحيح i, j, k;
    إذا( !stdmtx || cn == 1 )
    {
        إذا( cn == 1 )
        {
            PT a = stddev[0], b = mean[0];
            لكل( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        وإلا
        {
            len *= cn;
            cn--;
            لكل( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + mean[k]);
                k = CN_NEXT(k);
            }
        }
    }
    وإلا
    {
        len *= cn;
        cn--;
        لكل( i = j = 0; i < len; i++ )
        {
            PT s = mean[j];
            صحيح i0 = i - j;
            لكل( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_NEXT(j);
        }
    }
}

// special version for 16f, 16bf and 32f
ساكن فراغ
randnScale_16_or_32f(عائم* fbuf, فراغ* _dst, صحيح len, صحيح cn,
                     ثابت فراغ* _mean, ثابت فراغ* _stddev, صحيح flags)
{
    عائم* dst = (عائم*)_dst;
    ثابت عائم* mean = (ثابت عائم*)_mean;
    ثابت عائم* stddev = (ثابت عائم*)_stddev;
    منطقي stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    صحيح depth = CV_MAT_DEPTH(flags);
    عائم* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : dst;
    صحيح i, j, k;

    إذا( !stdmtx || cn == 1 )
    {
        إذا( cn == 1 )
        {
            عائم a = stddev[0], b = mean[0];
            لكل( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        وإلا
        {
            len *= cn;
            cn--;
            لكل( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + mean[k];
                k = CN_NEXT(k);
            }
        }
    }
    وإلا إذا( depth == CV_32F )
    {
        len *= cn;
        cn--;
        لكل( i = j = 0; i < len; i++ )
        {
            عائم s = mean[j];
            صحيح i0 = i - j;
            لكل( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_NEXT(j);
        }
    }
    وإلا
    {
        عائم elembuf[CV_CN_MAX];
        len *= cn;
        لكل( i = 0; i < len; i += cn )
        {
            // since we process fbuf in-place,
            // we need to copy each cn-channel element
            // prior to matrix multiplication
            لكل (j = 0; j < cn; j++)
                elembuf[j] = fbuf[i + j];
            لكل (j = 0; j < cn; j++) {
                عائم s = mean[j];
                لكل( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    إذا (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    وإلا إذا (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    وإلا إذا (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

تعريفنوع فراغ (*RandnScaleFunc)(عائم* src, فراغ* dst, صحيح len, صحيح cn,
                               ثابت فراغ* mean, ثابت فراغ* stddev, صحيح flags);

ساكن RandnScaleFunc randnScaleTab[CV_DEPTH_MAX] =
{
    randnScale_<uchar, عائم>, randnScale_<schar, عائم>, randnScale_<ushort, عائم>,
    randnScale_<short, عائم>, randnScale_<صحيح, عائم>, randnScale_16_or_32f,
    randnScale_<مزدوج, مزدوج>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<منطقي, عائم>, randnScale_<uint64_t, مزدوج>, randnScale_<int64_t, مزدوج>,
    randnScale_<unsigned, عائم>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

فراغ RNG::fill( كىرگۈزۈش_Output_Array _mat, صحيح disttype,
                كىرگۈزۈش_Array _param1arg, كىرگۈزۈش_Array _param2arg,
                منطقي saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    صحيح j, depth = mat.depth(), cn = mat.channels();
    صحيح esz1 = CV_ELEM_SIZE(depth);
    AutoBuffer<مزدوج> _parambuf;
    منطقي fast_int_mode = خطأ;
    منطقي small_flag = خطأ;
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

    ثابت فراغ* uni_param = 0;
    uchar* mean = 0;
    uchar* stddev = 0;
    منطقي stdmtx = خطأ;
    صحيح n1 = (صحيح)_param1.total();
    صحيح n2 = (صحيح)_param2.total();

    إذا( disttype == UNIFORM )
    {
        _parambuf.allocate(cn*(حجم(DivStruct)+حجم(مزدوج)-1)/حجم(مزدوج) + cn*4);
        مزدوج* parambuf = _parambuf.data();
        مزدوج* p1 = _param1.ptr<مزدوج>();
        مزدوج* p2 = _param2.ptr<مزدوج>();

        إذا( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convertTo(tmp, CV_64F);
            لكل( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        إذا( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convertTo(tmp, CV_64F);
            لكل( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        إذا( CV_IS_INT_TYPE(depth) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            لكل( j = 0, fast_int_mode = true; j < cn; j++ )
            {
                مزدوج a = std::min(p1[j], p2[j]);
                مزدوج b = std::max(p1[j], p2[j]);
                إذا( saturateRange )
                {
                    a = std::max(a, depth == CV_8U || depth == CV_16U || depth == CV_32U ||
                                 depth == CV_64U || depth == CV_Bool ? 0. :
                                 depth == CV_8S ? -128. : depth == CV_16S ? -32768. :
                                 depth == CV_32S ? (مزدوج)INT_MIN : (مزدوج)INT64_MIN);
                    b = std::min(b, depth == CV_8U ? 256. : depth == CV_Bool ? 2. : depth == CV_16U ? 65536. :
                                 depth == CV_8S ? 128. : depth == CV_16S ? 32768. : depth == CV_32U ? (مزدوج)UINT_MAX :
                                 depth == CV_32S ? (مزدوج)INT_MAX : (مزدوج)INT64_MAX);
                }
                ip[j][1] = (int64_t)ceil(a);
                int64_t idiff = ip[j][0] = (int64_t)floor(b) - ip[j][1] - 1;
                إذا (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                مزدوج diff = b - a;

                fast_int_mode = fast_int_mode && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                إذا( fast_int_mode )
                    small_flag = idiff <= 255;
                وإلا
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    إذا (depth == CV_64S || depth == CV_64U)
                    {
                        minval = INT64_MIN/2;
                        maxval = INT64_MAX;
                    }
                    إذا( diff > (مزدوج)maxval )
                        ip[j][0] = maxval;
                    إذا( a < (مزدوج)minval )
                        ip[j][1] = minval;
                }
            }

            uni_param = ip;
            إذا( !fast_int_mode )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((فراغ*)(ds + cn), (فراغ*)(parambuf + _parambuf.size()), "Last byte check");
                لكل( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    إذا (depth != CV_64U && depth != CV_64S) {
                        unsigned d = ds[j].d = (unsigned)(ip[j][0]+1);
                        صحيح l = 0;
                        طالما(((uint64)1 << l) < d)
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
        وإلا
        {
            مزدوج scale = depth == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            مزدوج maxdiff = saturateRange ? (مزدوج)FLT_MAX : DBL_MAX;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            إذا( depth != CV_64F )
            {
                Vec2f* fp = (Vec2f*)(parambuf + cn*2);
                لكل( j = 0; j < cn; j++ )
                {
                    fp[j][0] = (عائم)(std::min(maxdiff, p2[j] - p1[j])*scale);
                    fp[j][1] = (عائم)((p2[j] + p1[j])*0.5);
                }
                uni_param = fp;
            }
            وإلا
            {
                Vec2d* dp = (Vec2d*)(parambuf + cn*2);
                لكل( j = 0; j < cn; j++ )
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
    وإلا إذا( disttype == RNG::NORMAL )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        مزدوج* parambuf = _parambuf.data();

        صحيح ptype = esz1 == 8 ? CV_64F : CV_32F;

        إذا( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            mean = _param1.ptr();
        وإلا
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convertTo(tmp, ptype);
            mean = (uchar*)parambuf;
        }

        إذا( n1 < cn )
            لكل( j = n1*esz1; j < cn*esz1; j++ )
                mean[j] = mean[j - n1*esz1];

        إذا( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        وإلا
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convertTo(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        إذا( n2 < cn )
            لكل( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.rows == cn && _param2.cols == cn;
        scaleFunc = randnScaleTab[depth];
        CV_Assert( scaleFunc != 0 );
    }
    وإلا
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    ثابت Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    عائم fbuf[BLOCK_SIZE + CV_CN_MAX];
    صحيح total = (صحيح)it.size;
    صحيح blockSize = std::min((BLOCK_SIZE + cn - 1)/cn, total);
    size_t esz = (size_t)esz1*cn;
    صحيح flags = mat.type();

    إذا( disttype == UNIFORM )
        flags |= (small_flag ? (صحيح)RNG_FLAG_SMALL : 0);
    وإلا
        flags |= (stdmtx ? (صحيح)RNG_FLAG_STDMTX : 0);

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < total; j += blockSize )
        {
            صحيح len = std::min(total - j, blockSize);

            إذا( disttype == UNIFORM )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            وإلا
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
    أرجع getCoreTlsData().rng;
}

فراغ cv::setRNGSeed(صحيح seed)
{
    theRNG() = RNG(static_cast<uint64>(seed));
}


فراغ cv::randu(كىرگۈزۈش_Output_Array dst, كىرگۈزۈش_Array تۆۋەن, كىرگۈزۈش_Array يۇقىرى)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::UNIFORM, تۆۋەن, يۇقىرى);
}

فراغ cv::randn(كىرگۈزۈش_Output_Array dst, كىرگۈزۈش_Array mean, كىرگۈزۈش_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::NORMAL, mean, stddev);
}

نطاق cv
{

template<typename T> ساكن فراغ
randShuffle_( Mat& _arr, RNG& rng, مزدوج )
{
    unsigned sz = (unsigned)_arr.total();
    إذا( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        لكل( unsigned i = 0; i < sz; i++ )
        {
            unsigned j = (unsigned)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    وإلا
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* data = _arr.ptr();
        size_t step = _arr.step;
        صحيح rows = _arr.rows;
        صحيح cols = _arr.cols;
        لكل( صحيح i0 = 0; i0 < rows; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            لكل( صحيح j0 = 0; j0 < cols; j0++ )
            {
                unsigned k1 = (unsigned)rng % sz;
                صحيح i1 = (صحيح)(k1 / cols);
                صحيح j1 = (صحيح)(k1 - (unsigned)i1*(unsigned)cols);
                std::swap( p[j0], ((T*)(data + step*i1))[j1] );
            }
        }
    }
}

تعريفنوع فراغ (*RandShuffleFunc)( Mat& dst, RNG& rng, مزدوج iterFactor );

}

فراغ cv::randShuffle( كىرگۈزۈش_Output_Array _dst, مزدوج iterFactor, RNG* _rng )
{
    CV_INSTRUMENT_REGION();

    RandShuffleFunc tab[] =
    {
        0,
        randShuffle_<uchar>, // 1
        randShuffle_<ushort>, // 2
        randShuffle_<Vec<uchar,3> >, // 3
        randShuffle_<صحيح>, // 4
        0,
        randShuffle_<Vec<ushort,3> >, // 6
        0,
        randShuffle_<Vec<صحيح,2> >, // 8
        0, 0, 0,
        randShuffle_<Vec<صحيح,3> >, // 12
        0, 0, 0,
        randShuffle_<Vec<صحيح,4> >, // 16
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<صحيح,6> >, // 24
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<صحيح,8> > // 32
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

فراغ cv::RNG_MT19937::seed(unsigned s)
{
    state[0]= s;
    لكل (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

unsigned cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    ساكن unsigned mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    ثابت unsigned UPPER_MASK = 0x80000000U;
    ثابت unsigned LOWER_MASK = 0x7fffffffU;

    /* generate N words at one time */
    إذا (mti >= N)
    {
        صحيح kk = 0;

        لكل (; kk < N - M; ++kk)
        {
            unsigned y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        لكل (; kk < N - 1; ++kk)
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

    أرجع y;
}

cv::RNG_MT19937::operator unsigned() { أرجع next(); }

cv::RNG_MT19937::operator صحيح() { أرجع (صحيح)next();}

cv::RNG_MT19937::operator عائم() { أرجع next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::operator مزدوج()
{
    unsigned a = next() >> 5;
    unsigned b = next() >> 6;
    أرجع (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

صحيح cv::RNG_MT19937::uniform(صحيح a, صحيح b) { أرجع (صحيح)(next() % (b - a) + a); }

عائم cv::RNG_MT19937::uniform(عائم a, عائم b) { أرجع ((عائم)*هذا)*(b - a) + a; }

مزدوج cv::RNG_MT19937::uniform(مزدوج a, مزدوج b) { أرجع ((مزدوج)*هذا)*(b - a) + a; }

unsigned cv::RNG_MT19937::operator ()(unsigned b) { أرجع next() % b; }

unsigned cv::RNG_MT19937::operator ()() { أرجع next(); }

/* End of file. */
