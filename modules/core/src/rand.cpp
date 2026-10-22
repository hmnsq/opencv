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

#وصف_ڪريو CV_RNG_COEFF 4164903690U

/*
   Multiply-with-carry generator is used here:
   temp = ( A*X(n) + carry )
   X(n+1) = temp mod (2^32)
   carry = floor (temp / (2^32))
*/

#وصف_ڪريو  RNG_ايندڙ(x)    ((uint64)(بنانشان)(x)*CV_RNG_COEFF + ((x) >> 32))
// make it jump-less
#وصف_ڪريو  CN_ايندڙ(k)     (((k) + 1) & (((k) >= cn) - 1))

تعداد
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

سانچو<قسم_نالو T> ساكن فراغ
rand_بٽ( T* arr, صحيح len, صحيح cn, uint64* state, ثابت Vec2l* p, صحيح flags )
{
    منطقي small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 عارضي_2 = *state;
    صحيح i, k = 0;
    len *= cn;
    --cn;

    إذا( !small_flag )
    {
        لكل( i = 0; i <= len - 4; i += 4 )
        {
            int64_t t0, t1;

            عارضي_2 = RNG_ايندڙ(عارضي_2);
            t0 = ((int64_t)عارضي_2 & p[k][0]) + p[k][1];
            k = CN_ايندڙ(k);
            عارضي_2 = RNG_ايندڙ(عارضي_2);
            t1 = ((int64_t)عارضي_2 & p[k][0]) + p[k][1];
            k = CN_ايندڙ(k);
            arr[i] = saturate_cast<T>(t0);
            arr[i+1] = saturate_cast<T>(t1);

            عارضي_2 = RNG_ايندڙ(عارضي_2);
            t0 = ((int64_t)عارضي_2 & p[k][0]) + p[k][1];
            k = CN_ايندڙ(k);
            عارضي_2 = RNG_ايندڙ(عارضي_2);
            t1 = ((int64_t)عارضي_2 & p[k][0]) + p[k][1];
            k = CN_ايندڙ(k);
            arr[i+2] = saturate_cast<T>(t0);
            arr[i+3] = saturate_cast<T>(t1);
        }
    }
    وإلا
    {
        لكل( i = 0; i <= len - 4; i += 4 )
        {
            int64_t t0, t1, t;
            عارضي_2 = RNG_ايندڙ(عارضي_2);
            t = عارضي_2;
            // p[i+...][0] is within 0..255 in this branch (small_flag==true),
            // so we don't need to do (t>>...)&255,
            // the upper bits will be cleaned with ... & p[i+...][0].
            t0 = (t & p[k][0]) + p[k][1];
            k = CN_ايندڙ(k);
            t1 = ((t >> 8) & p[k][0]) + p[k][1];
            k = CN_ايندڙ(k);
            arr[i] = saturate_cast<T>(t0);
            arr[i+1] = saturate_cast<T>(t1);

            t0 = ((t >> 16) & p[k][0]) + p[k][1];
            k = CN_ايندڙ(k);
            t1 = ((t >> 24) & p[k][0]) + p[k][1];
            k = CN_ايندڙ(k);
            arr[i+2] = saturate_cast<T>(t0);
            arr[i+3] = saturate_cast<T>(t1);
        }
    }

    لكل( ; i < len; i++ )
    {
        int64_t t0;
        عارضي_2 = RNG_ايندڙ(عارضي_2);

        t0 = ((int64_t)عارضي_2 & p[k][0]) + p[k][1];
        k = CN_ايندڙ(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = عارضي_2;
}

بنية DivStruct
{
    بنانشان d;
    بنانشان M;
    صحيح sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

سانچو<قسم_نالو T> ساكن فراغ
randi_( T* arr, صحيح len, صحيح cn, uint64* state, ثابت DivStruct* p )
{
    uint64 عارضي_2 = *state;
    صحيح k = 0;
    len *= cn;
    cn--;
    لكل( صحيح i = 0; i < len; i++ )
    {
        عارضي_2 = RNG_ايندڙ(عارضي_2);
        بنانشان t = (بنانشان)عارضي_2;
        بنانشان v = (بنانشان)(((uint64)t * p[k].M) >> 32);
        v = (v + ((t - v) >> p[k].sh1)) >> p[k].sh2;
        int64_t res = (int64_t)(t - v*p[k].d) + p[k].delta;
        k = CN_ايندڙ(k);
        arr[i] = saturate_cast<T>(res);
    }
    *state = عارضي_2;
}

ساكن فراغ
randi_( int64_t* arr, صحيح len, صحيح cn, uint64* state, ثابت DivStruct* p )
{
    uint64 عارضي_2 = *state;
    صحيح k = 0;
    len *= cn;
    cn--;
    لكل( صحيح i = 0; i < len; i++ )
    {
        عارضي_2 = RNG_ايندڙ(عارضي_2);
        بنانشان t0 = (بنانشان)عارضي_2;
        عارضي_2 = RNG_ايندڙ(عارضي_2);
        بنانشان t1 = (بنانشان)عارضي_2;
        int64_t t = (int64_t)((((uint64_t)t0 << 32) | t1) % p[k].diff) + p[k].delta;
        k = CN_ايندڙ(k);
        arr[i] = t;
    }
    *state = عارضي_2;
}

ساكن فراغ
randi_( uint64_t* arr, صحيح len, صحيح cn, uint64* state, ثابت DivStruct* p )
{
    uint64 عارضي_2 = *state;
    صحيح k = 0;
    len *= cn;
    cn--;
    لكل( صحيح i = 0; i < len; i++ )
    {
        عارضي_2 = RNG_ايندڙ(عارضي_2);
        بنانشان t0 = (بنانشان)عارضي_2;
        عارضي_2 = RNG_ايندڙ(عارضي_2);
        بنانشان t1 = (بنانشان)عارضي_2;
        uint64_t t = (((uint64_t)t0 << 32) | t1) % p[k].diff;
        int64_t delta = p[k].delta;
        k = CN_ايندڙ(k);
        arr[i] = delta >= 0 || t >= (uint64_t)-delta ? t + (uint64_t)delta : 0;
    }
    *state = عارضي_2;
}

سانچو<قسم_نالو T>
ساكن فراغ rand_بٽ_Wrap(فراغ* arr, صحيح len, صحيح cn, uint64* state, ثابت فراغ* p, فراغ*, صحيح flags)
{
    rand_بٽ((T*)arr, len, cn, state, (ثابت Vec2l*)p, flags);
}

سانچو<قسم_نالو T>
ساكن فراغ randiWrap(فراغ* arr, صحيح len, صحيح cn, uint64* state, ثابت فراغ* p, فراغ*, صحيح)
{
    randi_((T*)arr, len, cn, state, (ثابت DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
ساكن مضمن فراغ cvt32f_منزل_FP8(ثابت عائم* src, فراغ* dst, صحيح len, صحيح اونهائي)
{
    إذا (اونهائي == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   لكل (صحيح i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    وإلا                              { fp8a_t* d = (fp8a_t*)dst; لكل (صحيح i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
ساكن مضمن منطقي is_FP8_اونهائي(صحيح d) { أرجع d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

ساكن فراغ randf_16_or_32f( فراغ* dst, صحيح len_, صحيح cn, uint64* state, ثابت فراغ* _p, فراغ* _fbuf, صحيح flags )
{
    ثابت Vec2f* p = (ثابت Vec2f*)_p;
    عائم* fbuf = (عائم*)_fbuf;
    صحيح اونهائي = CV_MAT_اونهائي(flags);
    uint64 عارضي_2 = *state;
    صحيح k = 0, len = len_*cn;
    عائم* arr = اونهائي == CV_16F || اونهائي == CV_16BF || is_FP8_اونهائي(اونهائي) ? fbuf : (عائم*)dst;
    cn--;
    لكل( صحيح i = 0; i < len; i++ )
    {
        صحيح t = (صحيح)(عارضي_2 = RNG_ايندڙ(عارضي_2));
        arr[i] = (عائم)(t*p[k][0]);
        k = CN_ايندڙ(k);
    }
    *state = عارضي_2;
    hal::جوڙيو_RNGBias32f(arr, &p[0][0], len_, cn+1);
    إذا (اونهائي == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    وإلا إذا (اونهائي == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    وإلا إذا (is_FP8_اونهائي(اونهائي))
        cvt32f_منزل_FP8(fbuf, dst, len, اونهائي);
}

ساكن فراغ
randf_64f( فراغ* _arr, صحيح len_, صحيح cn, uint64* state, ثابت فراغ* _p, فراغ*, صحيح )
{
    مزدوج* arr = (مزدوج*)_arr;
    ثابت Vec2d* p = (ثابت Vec2d*)_p;
    uint64 عارضي_2 = *state;
    صحيح k = 0, len = len_*cn;
    cn--;
    لكل( صحيح i = 0; i < len; i++ )
    {
        عارضي_2 = RNG_ايندڙ(عارضي_2);
        int64_t v = (int64_t)((عارضي_2 >> 32) | (عارضي_2 << 32));
        arr[i] = v*p[k][0];
        k = CN_ايندڙ(k);
    }
    *state = عارضي_2;
    hal::جوڙيو_RNGBias64f(arr, &p[0][0], len_, cn+1);
}

تعريفنوع فراغ (*RandFunc)(فراغ* arr, صحيح len, صحيح cn, uint64* state,
                         ثابت فراغ* p, فراغ* tempbuf, صحيح flags);

ساكن RandFunc randTab[CV_اونهائي_MAX][CV_اونهائي_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<ننڍو>, randiWrap<صحيح>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<منطقي>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<بنانشان>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        rand_بٽ_Wrap<uchar>, rand_بٽ_Wrap<schar>, rand_بٽ_Wrap<ushort>,
        rand_بٽ_Wrap<ننڍو>, rand_بٽ_Wrap<صحيح>, 0, 0, 0, 0,
        rand_بٽ_Wrap<منطقي>, rand_بٽ_Wrap<uint64_t>, rand_بٽ_Wrap<int64_t>,
        rand_بٽ_Wrap<بنانشان>, 0, 0, 0
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
    ساكن بنانشان kn[128];
    ساكن عائم wn[128], fn[128];
    uint64 عارضي_2 = *state;
    ساكن منطقي initialized=خطأ;
    صحيح i;

    إذا( !initialized )
    {
        ثابت مزدوج m1 = 2147483648.0;
        مزدوج dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        مزدوج q = vn/std::قوت_نما(-.5*dn*dn);
        kn[0] = (بنانشان)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (عائم)(q/m1);
        wn[127] = (عائم)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (عائم)std::قوت_نما(-.5*dn*dn);

        لكل(i=126;i>=1;i--)
        {
            dn = std::جذر(-2.*std::لاگرٿم(vn/dn+std::قوت_نما(-.5*dn*dn)));
            kn[i+1] = (بنانشان)((dn/tn)*m1);
            tn = dn;
            fn[i] = (عائم)std::قوت_نما(-.5*dn*dn);
            wn[i] = (عائم)(dn/m1);
        }
        initialized = سچ;
    }

    لكل( i = 0; i < len; i++ )
    {
        عائم x, y;
        لكل(;;)
        {
            صحيح hz = (صحيح)عارضي_2;
            عارضي_2 = RNG_ايندڙ(عارضي_2);
            صحيح iz = hz & 127;
            x = hz*wn[iz];
            إذا( (بنانشان)std::مطلق_قدر(hz) < kn[iz] )
                اكسر;
            إذا( iz == 0) // iz==0, handles the base strip
            {
                افعل
                {
                    x = (بنانشان)عارضي_2*rng_flt;
                    عارضي_2 = RNG_ايندڙ(عارضي_2);
                    y = (بنانشان)عارضي_2*rng_flt;
                    عارضي_2 = RNG_ايندڙ(عارضي_2);
                    x = (عائم)(-std::لاگرٿم(x+FLT_MIN)*0.2904764);
                    y = (عائم)-std::لاگرٿم(y+FLT_MIN);
                }	// .2904764 is 1/r
                طالما( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                اكسر;
            }
            // iz > 0, handle the wedges of other strips
            y = (بنانشان)عارضي_2*rng_flt;
            عارضي_2 = RNG_ايندڙ(عارضي_2);
            إذا( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::قوت_نما(-.5*x*x) )
                اكسر;
        }
        arr[i] = x;
    }
    *state = عارضي_2;
}


مزدوج RNG::gaussian(مزدوج sigma)
{
    عائم عارضي_2;
    randn_0_1_32f( &عارضي_2, 1, &state );
    أرجع عارضي_2*sigma;
}

سانچو<قسم_نالو T, قسم_نالو PT> ساكن فراغ
randnScale_(عائم* src, فراغ* _dst, صحيح len, صحيح cn,
            ثابت فراغ* سراسري, ثابت فراغ* _stddev, صحيح flags )
{
    T* dst = (T*)_dst;
    ثابت PT* سراسري_2 = (ثابت PT*)سراسري;
    ثابت PT* stddev = (ثابت PT*)_stddev;
    منطقي stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    صحيح i, j, k;
    إذا( !stdmtx || cn == 1 )
    {
        إذا( cn == 1 )
        {
            PT a = stddev[0], b = سراسري_2[0];
            لكل( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        وإلا
        {
            len *= cn;
            cn--;
            لكل( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + سراسري_2[k]);
                k = CN_ايندڙ(k);
            }
        }
    }
    وإلا
    {
        len *= cn;
        cn--;
        لكل( i = j = 0; i < len; i++ )
        {
            PT s = سراسري_2[j];
            صحيح i0 = i - j;
            لكل( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_ايندڙ(j);
        }
    }
}

// special version for 16f, 16bf and 32f
ساكن فراغ
randnScale_16_or_32f(عائم* fbuf, فراغ* _dst, صحيح len, صحيح cn,
                     ثابت فراغ* سراسري, ثابت فراغ* _stddev, صحيح flags)
{
    عائم* dst = (عائم*)_dst;
    ثابت عائم* سراسري_2 = (ثابت عائم*)سراسري;
    ثابت عائم* stddev = (ثابت عائم*)_stddev;
    منطقي stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    صحيح اونهائي = CV_MAT_اونهائي(flags);
    عائم* arr = اونهائي == CV_16F || اونهائي == CV_16BF || is_FP8_اونهائي(اونهائي) ? fbuf : dst;
    صحيح i, j, k;

    إذا( !stdmtx || cn == 1 )
    {
        إذا( cn == 1 )
        {
            عائم a = stddev[0], b = سراسري_2[0];
            لكل( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        وإلا
        {
            len *= cn;
            cn--;
            لكل( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + سراسري_2[k];
                k = CN_ايندڙ(k);
            }
        }
    }
    وإلا إذا( اونهائي == CV_32F )
    {
        len *= cn;
        cn--;
        لكل( i = j = 0; i < len; i++ )
        {
            عائم s = سراسري_2[j];
            صحيح i0 = i - j;
            لكل( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_ايندڙ(j);
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
                عائم s = سراسري_2[j];
                لكل( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    إذا (اونهائي == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    وإلا إذا (اونهائي == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    وإلا إذا (is_FP8_اونهائي(اونهائي))
        cvt32f_منزل_FP8(fbuf, dst, len, اونهائي);
}

تعريفنوع فراغ (*RandnScaleFunc)(عائم* src, فراغ* dst, صحيح len, صحيح cn,
                               ثابت فراغ* سراسري_2, ثابت فراغ* stddev, صحيح flags);

ساكن RandnScaleFunc randnScaleTab[CV_اونهائي_MAX] =
{
    randnScale_<uchar, عائم>, randnScale_<schar, عائم>, randnScale_<ushort, عائم>,
    randnScale_<ننڍو, عائم>, randnScale_<صحيح, عائم>, randnScale_16_or_32f,
    randnScale_<مزدوج, مزدوج>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<منطقي, عائم>, randnScale_<uint64_t, مزدوج>, randnScale_<int64_t, مزدوج>,
    randnScale_<بنانشان, عائم>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

فراغ RNG::fill( داخلا_Output_Array _mat, صحيح disttype,
                داخلا_Array _param1arg, داخلا_Array _param2arg,
                منطقي saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    صحيح j, اونهائي = mat.اونهائي(), cn = mat.channels();
    صحيح esz1 = CV_ELEM_سائيز(اونهائي);
    Auto_بفر<مزدوج> _parambuf;
    منطقي fast_int_گهڻائي = خطأ;
    منطقي small_flag = خطأ;
    RandFunc func = 0;
    RandnScaleFunc scaleFunc = 0;

    CV_Assert(_param1.channels() == 1 && (_param1.صفون == 1 || _param1.کانن_ڳڻپ == 1) &&
              (_param1.صفون + _param1.کانن_ڳڻپ - 1 == cn || _param1.صفون + _param1.کانن_ڳڻپ - 1 == 1 ||
               (_param1.size() == سائيز(1, 4) && _param1.type() == CV_64F && cn <= 4)));
    CV_Assert( _param2.channels() == 1 &&
               (((_param2.صفون == 1 || _param2.کانن_ڳڻپ == 1) &&
                (_param2.صفون + _param2.کانن_ڳڻپ - 1 == cn || _param2.صفون + _param2.کانن_ڳڻپ - 1 == 1 ||
                (_param1.size() == سائيز(1, 4) && _param1.type() == CV_64F && cn <= 4))) ||
                (_param2.صفون == cn && _param2.کانن_ڳڻپ == cn && disttype == عام_4)));

    ثابت فراغ* uni_param = 0;
    uchar* سراسري_2 = 0;
    uchar* stddev = 0;
    منطقي stdmtx = خطأ;
    صحيح n1 = (صحيح)_param1.ڪل();
    صحيح n2 = (صحيح)_param2.ڪل();

    إذا( disttype == هڪجهڙو_2 )
    {
        _parambuf.allocate(cn*(حجم(DivStruct)+حجم(مزدوج)-1)/حجم(مزدوج) + cn*4);
        مزدوج* parambuf = _parambuf.معلومات();
        مزدوج* p1 = _param1.ptr<مزدوج>();
        مزدوج* p2 = _param2.ptr<مزدوج>();

        إذا( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convert_منزل(tmp, CV_64F);
            لكل( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        إذا( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convert_منزل(tmp, CV_64F);
            لكل( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        إذا( CV_IS_INT_TYPE(اونهائي) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            لكل( j = 0, fast_int_گهڻائي = سچ; j < cn; j++ )
            {
                مزدوج a = std::min(p1[j], p2[j]);
                مزدوج b = std::max(p1[j], p2[j]);
                إذا( saturateRange )
                {
                    a = std::max(a, اونهائي == CV_8U || اونهائي == CV_16U || اونهائي == CV_32U ||
                                 اونهائي == CV_64U || اونهائي == CV_Bool ? 0. :
                                 اونهائي == CV_8S ? -128. : اونهائي == CV_16S ? -32768. :
                                 اونهائي == CV_32S ? (مزدوج)INT_MIN : (مزدوج)INT64_MIN);
                    b = std::min(b, اونهائي == CV_8U ? 256. : اونهائي == CV_Bool ? 2. : اونهائي == CV_16U ? 65536. :
                                 اونهائي == CV_8S ? 128. : اونهائي == CV_16S ? 32768. : اونهائي == CV_32U ? (مزدوج)UINT_MAX :
                                 اونهائي == CV_32S ? (مزدوج)وڌ_کان_وڌ_عدد : (مزدوج)INT64_MAX);
                }
                ip[j][1] = (int64_t)ceil(a);
                int64_t idiff = ip[j][0] = (int64_t)floor(b) - ip[j][1] - 1;
                إذا (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                مزدوج diff = b - a;

                fast_int_گهڻائي = fast_int_گهڻائي && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                إذا( fast_int_گهڻائي )
                    small_flag = idiff <= 255;
                وإلا
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    إذا (اونهائي == CV_64S || اونهائي == CV_64U)
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
            إذا( !fast_int_گهڻائي )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((فراغ*)(ds + cn), (فراغ*)(parambuf + _parambuf.size()), "Last byte check");
                لكل( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    إذا (اونهائي != CV_64U && اونهائي != CV_64S) {
                        بنانشان d = ds[j].d = (بنانشان)(ip[j][0]+1);
                        صحيح l = 0;
                        طالما(((uint64)1 << l) < d)
                            l++;
                        ds[j].M = (بنانشان)(((uint64)1 << 32)*(((uint64)1 << l) - d)/d) + 1;
                        ds[j].sh1 = std::min(l, 1);
                        ds[j].sh2 = std::max(l - 1, 0);
                    }
                }
                uni_param = ds;
            }

            func = randTab[fast_int_گهڻائي ? 1 : 0][اونهائي];
        }
        وإلا
        {
            مزدوج scale = اونهائي == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            مزدوج maxdiff = saturateRange ? (مزدوج)FLT_MAX : DBL_MAX;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            إذا( اونهائي != CV_64F )
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

            func = randTab[0][اونهائي];
        }
        CV_Assert( func != 0 );
    }
    وإلا إذا( disttype == RNG::عام_4 )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        مزدوج* parambuf = _parambuf.معلومات();

        صحيح ptype = esz1 == 8 ? CV_64F : CV_32F;

        إذا( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            سراسري_2 = _param1.ptr();
        وإلا
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convert_منزل(tmp, ptype);
            سراسري_2 = (uchar*)parambuf;
        }

        إذا( n1 < cn )
            لكل( j = n1*esz1; j < cn*esz1; j++ )
                سراسري_2[j] = سراسري_2[j - n1*esz1];

        إذا( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        وإلا
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convert_منزل(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        إذا( n2 < cn )
            لكل( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.صفون == cn && _param2.کانن_ڳڻپ == cn;
        scaleFunc = randnScaleTab[اونهائي];
        CV_Assert( scaleFunc != 0 );
    }
    وإلا
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    ثابت Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    عائم fbuf[بلاڪ_سائيز + CV_CN_MAX];
    صحيح ڪل = (صحيح)it.size;
    صحيح بلاڪ_سائيز_2 = std::min((بلاڪ_سائيز + cn - 1)/cn, ڪل);
    size_t esz = (size_t)esz1*cn;
    صحيح flags = mat.type();

    إذا( disttype == هڪجهڙو_2 )
        flags |= (small_flag ? (صحيح)RNG_FLAG_SMALL : 0);
    وإلا
        flags |= (stdmtx ? (صحيح)RNG_FLAG_STDMTX : 0);

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < ڪل; j += بلاڪ_سائيز_2 )
        {
            صحيح len = std::min(ڪل - j, بلاڪ_سائيز_2);

            إذا( disttype == هڪجهڙو_2 )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            وإلا
            {
                randn_0_1_32f(fbuf, len*cn, &state);
                scaleFunc(fbuf, ptr + j*esz, len, cn, سراسري_2, stddev, flags);
            }
        }
    }
}

}

cv::RNG& cv::theRNG()
{
    أرجع get_Core_Tls_معلومات().rng;
}

فراغ cv::سيٽ_RNGSeed(صحيح ٻج)
{
    theRNG() = RNG(static_cast<uint64>(ٻج));
}


فراغ cv::randu(داخلا_Output_Array dst, داخلا_Array هيٺيون, داخلا_Array مٿاهون)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::هڪجهڙو_2, هيٺيون, مٿاهون);
}

فراغ cv::randn(داخلا_Output_Array dst, داخلا_Array سراسري_2, داخلا_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::عام_4, سراسري_2, stddev);
}

نطاق cv
{

سانچو<قسم_نالو T> ساكن فراغ
randShuffle_( Mat& _arr, RNG& rng, مزدوج )
{
    بنانشان sz = (بنانشان)_arr.ڪل();
    إذا( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        لكل( بنانشان i = 0; i < sz; i++ )
        {
            بنانشان j = (بنانشان)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    وإلا
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* معلومات = _arr.ptr();
        size_t قدم = _arr.قدم;
        صحيح صفون = _arr.صفون;
        صحيح کانن_ڳڻپ = _arr.کانن_ڳڻپ;
        لكل( صحيح i0 = 0; i0 < صفون; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            لكل( صحيح j0 = 0; j0 < کانن_ڳڻپ; j0++ )
            {
                بنانشان k1 = (بنانشان)rng % sz;
                صحيح i1 = (صحيح)(k1 / کانن_ڳڻپ);
                صحيح j1 = (صحيح)(k1 - (بنانشان)i1*(بنانشان)کانن_ڳڻپ);
                std::swap( p[j0], ((T*)(معلومات + قدم*i1))[j1] );
            }
        }
    }
}

تعريفنوع فراغ (*RandShuffleFunc)( Mat& dst, RNG& rng, مزدوج iter_ضرب_ڪندڙ );

}

فراغ cv::randShuffle( داخلا_Output_Array _dst, مزدوج iter_ضرب_ڪندڙ, RNG* _rng )
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
    CV_Assert( dst.elem_سائيز() <= 32 );
    RandShuffleFunc func = tab[dst.elem_سائيز()];
    CV_Assert( func != 0 );
    func( dst, rng, iter_ضرب_ڪندڙ );
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

cv::RNG_MT19937::RNG_MT19937(بنانشان s) { ٻج(s); }

cv::RNG_MT19937::RNG_MT19937() { ٻج(5489U); }

فراغ cv::RNG_MT19937::ٻج(بنانشان s)
{
    state[0]= s;
    لكل (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

بنانشان cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    ساكن بنانشان mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    ثابت بنانشان مٿانهون_نقاب = 0x80000000U;
    ثابت بنانشان هيٺاهون_نقاب = 0x7fffffffU;

    /* generate N words at one time */
    إذا (mti >= N)
    {
        صحيح kk = 0;

        لكل (; kk < N - M; ++kk)
        {
            بنانشان y = (state[kk] & مٿانهون_نقاب) | (state[kk + 1] & هيٺاهون_نقاب);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        لكل (; kk < N - 1; ++kk)
        {
            بنانشان y = (state[kk] & مٿانهون_نقاب) | (state[kk + 1] & هيٺاهون_نقاب);
            state[kk] = state[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        بنانشان y = (state[N - 1] & مٿانهون_نقاب) | (state[0] & هيٺاهون_نقاب);
        state[N - 1] = state[M - 1] ^ (y >> 1) ^ mag01[y & 0x1U];

        mti = 0;
    }

    بنانشان y = state[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y <<  7) & 0x9d2c5680U;
    y ^= (y << 15) & 0xefc60000U;
    y ^= (y >> 18);

    أرجع y;
}

cv::RNG_MT19937::عامل بنانشان() { أرجع next(); }

cv::RNG_MT19937::عامل صحيح() { أرجع (صحيح)next();}

cv::RNG_MT19937::عامل عائم() { أرجع next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::عامل مزدوج()
{
    بنانشان a = next() >> 5;
    بنانشان b = next() >> 6;
    أرجع (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

صحيح cv::RNG_MT19937::هڪجهڙو(صحيح a, صحيح b) { أرجع (صحيح)(next() % (b - a) + a); }

عائم cv::RNG_MT19937::هڪجهڙو(عائم a, عائم b) { أرجع ((عائم)*هذا)*(b - a) + a; }

مزدوج cv::RNG_MT19937::هڪجهڙو(مزدوج a, مزدوج b) { أرجع ((مزدوج)*هذا)*(b - a) + a; }

بنانشان cv::RNG_MT19937::عامل ()(بنانشان b) { أرجع next() % b; }

بنانشان cv::RNG_MT19937::عامل ()() { أرجع next(); }

/* End of file. */
