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

نامخلا cv
{

///////////////////////////// Functions Declaration //////////////////////////////////////

#تعریف_کرو CV_RNG_COEFF 4164903690U

/*
   Multiply-with-carry generator is used here:
   temp = ( A*X(n) + carry )
   X(n+1) = temp mod (2^32)
   carry = floor (temp / (2^32))
*/

#تعریف_کرو  RNG_اگلا(x)    ((uint64)(غیرعلامتی)(x)*CV_RNG_COEFF + ((x) >> 32))
// make it jump-less
#تعریف_کرو  CN_اگلا(k)     (((k) + 1) & (((k) >= cn) - 1))

شمار
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

سانچہ<قسم_نام T> ساکن خالی
rand_بٹ( T* arr, صحیح len, صحیح cn, uint64* state, ثابت Vec2l* p, صحیح flags )
{
    منطقی small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 عارضی_2 = *state;
    صحیح i, k = 0;
    len *= cn;
    --cn;

    اگر( !small_flag )
    {
        برائے( i = 0; i <= len - 4; i += 4 )
        {
            int64_t t0, t1;

            عارضی_2 = RNG_اگلا(عارضی_2);
            t0 = ((int64_t)عارضی_2 & p[k][0]) + p[k][1];
            k = CN_اگلا(k);
            عارضی_2 = RNG_اگلا(عارضی_2);
            t1 = ((int64_t)عارضی_2 & p[k][0]) + p[k][1];
            k = CN_اگلا(k);
            arr[i] = saturate_cast<T>(t0);
            arr[i+1] = saturate_cast<T>(t1);

            عارضی_2 = RNG_اگلا(عارضی_2);
            t0 = ((int64_t)عارضی_2 & p[k][0]) + p[k][1];
            k = CN_اگلا(k);
            عارضی_2 = RNG_اگلا(عارضی_2);
            t1 = ((int64_t)عارضی_2 & p[k][0]) + p[k][1];
            k = CN_اگلا(k);
            arr[i+2] = saturate_cast<T>(t0);
            arr[i+3] = saturate_cast<T>(t1);
        }
    }
    ورنہ
    {
        برائے( i = 0; i <= len - 4; i += 4 )
        {
            int64_t t0, t1, t;
            عارضی_2 = RNG_اگلا(عارضی_2);
            t = عارضی_2;
            // p[i+...][0] is within 0..255 in this branch (small_flag==true),
            // so we don't need to do (t>>...)&255,
            // the upper bits will be cleaned with ... & p[i+...][0].
            t0 = (t & p[k][0]) + p[k][1];
            k = CN_اگلا(k);
            t1 = ((t >> 8) & p[k][0]) + p[k][1];
            k = CN_اگلا(k);
            arr[i] = saturate_cast<T>(t0);
            arr[i+1] = saturate_cast<T>(t1);

            t0 = ((t >> 16) & p[k][0]) + p[k][1];
            k = CN_اگلا(k);
            t1 = ((t >> 24) & p[k][0]) + p[k][1];
            k = CN_اگلا(k);
            arr[i+2] = saturate_cast<T>(t0);
            arr[i+3] = saturate_cast<T>(t1);
        }
    }

    برائے( ; i < len; i++ )
    {
        int64_t t0;
        عارضی_2 = RNG_اگلا(عارضی_2);

        t0 = ((int64_t)عارضی_2 & p[k][0]) + p[k][1];
        k = CN_اگلا(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = عارضی_2;
}

ساخت DivStruct
{
    غیرعلامتی d;
    غیرعلامتی M;
    صحیح sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

سانچہ<قسم_نام T> ساکن خالی
randi_( T* arr, صحیح len, صحیح cn, uint64* state, ثابت DivStruct* p )
{
    uint64 عارضی_2 = *state;
    صحیح k = 0;
    len *= cn;
    cn--;
    برائے( صحیح i = 0; i < len; i++ )
    {
        عارضی_2 = RNG_اگلا(عارضی_2);
        غیرعلامتی t = (غیرعلامتی)عارضی_2;
        غیرعلامتی v = (غیرعلامتی)(((uint64)t * p[k].M) >> 32);
        v = (v + ((t - v) >> p[k].sh1)) >> p[k].sh2;
        int64_t res = (int64_t)(t - v*p[k].d) + p[k].delta;
        k = CN_اگلا(k);
        arr[i] = saturate_cast<T>(res);
    }
    *state = عارضی_2;
}

ساکن خالی
randi_( int64_t* arr, صحیح len, صحیح cn, uint64* state, ثابت DivStruct* p )
{
    uint64 عارضی_2 = *state;
    صحیح k = 0;
    len *= cn;
    cn--;
    برائے( صحیح i = 0; i < len; i++ )
    {
        عارضی_2 = RNG_اگلا(عارضی_2);
        غیرعلامتی t0 = (غیرعلامتی)عارضی_2;
        عارضی_2 = RNG_اگلا(عارضی_2);
        غیرعلامتی t1 = (غیرعلامتی)عارضی_2;
        int64_t t = (int64_t)((((uint64_t)t0 << 32) | t1) % p[k].diff) + p[k].delta;
        k = CN_اگلا(k);
        arr[i] = t;
    }
    *state = عارضی_2;
}

ساکن خالی
randi_( uint64_t* arr, صحیح len, صحیح cn, uint64* state, ثابت DivStruct* p )
{
    uint64 عارضی_2 = *state;
    صحیح k = 0;
    len *= cn;
    cn--;
    برائے( صحیح i = 0; i < len; i++ )
    {
        عارضی_2 = RNG_اگلا(عارضی_2);
        غیرعلامتی t0 = (غیرعلامتی)عارضی_2;
        عارضی_2 = RNG_اگلا(عارضی_2);
        غیرعلامتی t1 = (غیرعلامتی)عارضی_2;
        uint64_t t = (((uint64_t)t0 << 32) | t1) % p[k].diff;
        int64_t delta = p[k].delta;
        k = CN_اگلا(k);
        arr[i] = delta >= 0 || t >= (uint64_t)-delta ? t + (uint64_t)delta : 0;
    }
    *state = عارضی_2;
}

سانچہ<قسم_نام T>
ساکن خالی rand_بٹ_Wrap(خالی* arr, صحیح len, صحیح cn, uint64* state, ثابت خالی* p, خالی*, صحیح flags)
{
    rand_بٹ((T*)arr, len, cn, state, (ثابت Vec2l*)p, flags);
}

سانچہ<قسم_نام T>
ساکن خالی randiWrap(خالی* arr, صحیح len, صحیح cn, uint64* state, ثابت خالی* p, خالی*, صحیح)
{
    randi_((T*)arr, len, cn, state, (ثابت DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
ساکن اندرونی خالی cvt32f_مقصد_FP8(ثابت اعشاری* src, خالی* dst, صحیح len, صحیح گہرائی)
{
    اگر (گہرائی == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   برائے (صحیح i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    ورنہ                              { fp8a_t* d = (fp8a_t*)dst; برائے (صحیح i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
ساکن اندرونی منطقی is_FP8_گہرائی(صحیح d) { واپس d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

ساکن خالی randf_16_or_32f( خالی* dst, صحیح len_, صحیح cn, uint64* state, ثابت خالی* _p, خالی* _fbuf, صحیح flags )
{
    ثابت Vec2f* p = (ثابت Vec2f*)_p;
    اعشاری* fbuf = (اعشاری*)_fbuf;
    صحیح گہرائی = CV_MAT_گہرائی(flags);
    uint64 عارضی_2 = *state;
    صحیح k = 0, len = len_*cn;
    اعشاری* arr = گہرائی == CV_16F || گہرائی == CV_16BF || is_FP8_گہرائی(گہرائی) ? fbuf : (اعشاری*)dst;
    cn--;
    برائے( صحیح i = 0; i < len; i++ )
    {
        صحیح t = (صحیح)(عارضی_2 = RNG_اگلا(عارضی_2));
        arr[i] = (اعشاری)(t*p[k][0]);
        k = CN_اگلا(k);
    }
    *state = عارضی_2;
    hal::جوڑو_RNGBias32f(arr, &p[0][0], len_, cn+1);
    اگر (گہرائی == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    ورنہ اگر (گہرائی == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    ورنہ اگر (is_FP8_گہرائی(گہرائی))
        cvt32f_مقصد_FP8(fbuf, dst, len, گہرائی);
}

ساکن خالی
randf_64f( خالی* _arr, صحیح len_, صحیح cn, uint64* state, ثابت خالی* _p, خالی*, صحیح )
{
    دوہرا* arr = (دوہرا*)_arr;
    ثابت Vec2d* p = (ثابت Vec2d*)_p;
    uint64 عارضی_2 = *state;
    صحیح k = 0, len = len_*cn;
    cn--;
    برائے( صحیح i = 0; i < len; i++ )
    {
        عارضی_2 = RNG_اگلا(عارضی_2);
        int64_t v = (int64_t)((عارضی_2 >> 32) | (عارضی_2 << 32));
        arr[i] = v*p[k][0];
        k = CN_اگلا(k);
    }
    *state = عارضی_2;
    hal::جوڑو_RNGBias64f(arr, &p[0][0], len_, cn+1);
}

قسمتعریف خالی (*RandFunc)(خالی* arr, صحیح len, صحیح cn, uint64* state,
                         ثابت خالی* p, خالی* tempbuf, صحیح flags);

ساکن RandFunc randTab[CV_گہرائی_MAX][CV_گہرائی_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<مختصر>, randiWrap<صحیح>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<منطقی>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<غیرعلامتی>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        rand_بٹ_Wrap<uchar>, rand_بٹ_Wrap<schar>, rand_بٹ_Wrap<ushort>,
        rand_بٹ_Wrap<مختصر>, rand_بٹ_Wrap<صحیح>, 0, 0, 0, 0,
        rand_بٹ_Wrap<منطقی>, rand_بٹ_Wrap<uint64_t>, rand_بٹ_Wrap<int64_t>,
        rand_بٹ_Wrap<غیرعلامتی>, 0, 0, 0
    }
};

/*
   The code below implements the algorithm described in
   "The Ziggurat Method for Generating Random Variables"
   by George Marsaglia and Wai Wan Tsang, Journal of Statistical Software, 2007.
*/
ساکن خالی
randn_0_1_32f( اعشاری* arr, صحیح len, uint64* state )
{
    ثابت اعشاری r = 3.442620f; // The start of the right tail
    ثابت اعشاری rng_flt = 2.3283064365386962890625e-10f; // 2^-32
    ساکن غیرعلامتی kn[128];
    ساکن اعشاری wn[128], fn[128];
    uint64 عارضی_2 = *state;
    ساکن منطقی initialized=جھوٹ;
    صحیح i;

    اگر( !initialized )
    {
        ثابت دوہرا m1 = 2147483648.0;
        دوہرا dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        دوہرا q = vn/std::قوت_نما(-.5*dn*dn);
        kn[0] = (غیرعلامتی)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (اعشاری)(q/m1);
        wn[127] = (اعشاری)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (اعشاری)std::قوت_نما(-.5*dn*dn);

        برائے(i=126;i>=1;i--)
        {
            dn = std::جذر(-2.*std::لوگارتھم(vn/dn+std::قوت_نما(-.5*dn*dn)));
            kn[i+1] = (غیرعلامتی)((dn/tn)*m1);
            tn = dn;
            fn[i] = (اعشاری)std::قوت_نما(-.5*dn*dn);
            wn[i] = (اعشاری)(dn/m1);
        }
        initialized = سچ;
    }

    برائے( i = 0; i < len; i++ )
    {
        اعشاری x, y;
        برائے(;;)
        {
            صحیح hz = (صحیح)عارضی_2;
            عارضی_2 = RNG_اگلا(عارضی_2);
            صحیح iz = hz & 127;
            x = hz*wn[iz];
            اگر( (غیرعلامتی)std::مطلق_قدر(hz) < kn[iz] )
                توڑو;
            اگر( iz == 0) // iz==0, handles the base strip
            {
                کرو
                {
                    x = (غیرعلامتی)عارضی_2*rng_flt;
                    عارضی_2 = RNG_اگلا(عارضی_2);
                    y = (غیرعلامتی)عارضی_2*rng_flt;
                    عارضی_2 = RNG_اگلا(عارضی_2);
                    x = (اعشاری)(-std::لوگارتھم(x+FLT_MIN)*0.2904764);
                    y = (اعشاری)-std::لوگارتھم(y+FLT_MIN);
                }	// .2904764 is 1/r
                جب( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                توڑو;
            }
            // iz > 0, handle the wedges of other strips
            y = (غیرعلامتی)عارضی_2*rng_flt;
            عارضی_2 = RNG_اگلا(عارضی_2);
            اگر( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::قوت_نما(-.5*x*x) )
                توڑو;
        }
        arr[i] = x;
    }
    *state = عارضی_2;
}


دوہرا RNG::gaussian(دوہرا sigma)
{
    اعشاری عارضی_2;
    randn_0_1_32f( &عارضی_2, 1, &state );
    واپس عارضی_2*sigma;
}

سانچہ<قسم_نام T, قسم_نام PT> ساکن خالی
randnScale_(اعشاری* src, خالی* _dst, صحیح len, صحیح cn,
            ثابت خالی* اوسط, ثابت خالی* _stddev, صحیح flags )
{
    T* dst = (T*)_dst;
    ثابت PT* اوسط_2 = (ثابت PT*)اوسط;
    ثابت PT* stddev = (ثابت PT*)_stddev;
    منطقی stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    صحیح i, j, k;
    اگر( !stdmtx || cn == 1 )
    {
        اگر( cn == 1 )
        {
            PT a = stddev[0], b = اوسط_2[0];
            برائے( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        ورنہ
        {
            len *= cn;
            cn--;
            برائے( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + اوسط_2[k]);
                k = CN_اگلا(k);
            }
        }
    }
    ورنہ
    {
        len *= cn;
        cn--;
        برائے( i = j = 0; i < len; i++ )
        {
            PT s = اوسط_2[j];
            صحیح i0 = i - j;
            برائے( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_اگلا(j);
        }
    }
}

// special version for 16f, 16bf and 32f
ساکن خالی
randnScale_16_or_32f(اعشاری* fbuf, خالی* _dst, صحیح len, صحیح cn,
                     ثابت خالی* اوسط, ثابت خالی* _stddev, صحیح flags)
{
    اعشاری* dst = (اعشاری*)_dst;
    ثابت اعشاری* اوسط_2 = (ثابت اعشاری*)اوسط;
    ثابت اعشاری* stddev = (ثابت اعشاری*)_stddev;
    منطقی stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    صحیح گہرائی = CV_MAT_گہرائی(flags);
    اعشاری* arr = گہرائی == CV_16F || گہرائی == CV_16BF || is_FP8_گہرائی(گہرائی) ? fbuf : dst;
    صحیح i, j, k;

    اگر( !stdmtx || cn == 1 )
    {
        اگر( cn == 1 )
        {
            اعشاری a = stddev[0], b = اوسط_2[0];
            برائے( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        ورنہ
        {
            len *= cn;
            cn--;
            برائے( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + اوسط_2[k];
                k = CN_اگلا(k);
            }
        }
    }
    ورنہ اگر( گہرائی == CV_32F )
    {
        len *= cn;
        cn--;
        برائے( i = j = 0; i < len; i++ )
        {
            اعشاری s = اوسط_2[j];
            صحیح i0 = i - j;
            برائے( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_اگلا(j);
        }
    }
    ورنہ
    {
        اعشاری elembuf[CV_CN_MAX];
        len *= cn;
        برائے( i = 0; i < len; i += cn )
        {
            // since we process fbuf in-place,
            // we need to copy each cn-channel element
            // prior to matrix multiplication
            برائے (j = 0; j < cn; j++)
                elembuf[j] = fbuf[i + j];
            برائے (j = 0; j < cn; j++) {
                اعشاری s = اوسط_2[j];
                برائے( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    اگر (گہرائی == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    ورنہ اگر (گہرائی == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    ورنہ اگر (is_FP8_گہرائی(گہرائی))
        cvt32f_مقصد_FP8(fbuf, dst, len, گہرائی);
}

قسمتعریف خالی (*RandnScaleFunc)(اعشاری* src, خالی* dst, صحیح len, صحیح cn,
                               ثابت خالی* اوسط_2, ثابت خالی* stddev, صحیح flags);

ساکن RandnScaleFunc randnScaleTab[CV_گہرائی_MAX] =
{
    randnScale_<uchar, اعشاری>, randnScale_<schar, اعشاری>, randnScale_<ushort, اعشاری>,
    randnScale_<مختصر, اعشاری>, randnScale_<صحیح, اعشاری>, randnScale_16_or_32f,
    randnScale_<دوہرا, دوہرا>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<منطقی, اعشاری>, randnScale_<uint64_t, دوہرا>, randnScale_<int64_t, دوہرا>,
    randnScale_<غیرعلامتی, اعشاری>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

خالی RNG::fill( داخلہ_Output_Array _mat, صحیح disttype,
                داخلہ_Array _param1arg, داخلہ_Array _param2arg,
                منطقی saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    صحیح j, گہرائی = mat.گہرائی(), cn = mat.channels();
    صحیح esz1 = CV_ELEM_سائز(گہرائی);
    Auto_بفر<دوہرا> _parambuf;
    منطقی fast_int_منوال = جھوٹ;
    منطقی small_flag = جھوٹ;
    RandFunc func = 0;
    RandnScaleFunc scaleFunc = 0;

    CV_Assert(_param1.channels() == 1 && (_param1.صفیں == 1 || _param1.ستون_تعداد == 1) &&
              (_param1.صفیں + _param1.ستون_تعداد - 1 == cn || _param1.صفیں + _param1.ستون_تعداد - 1 == 1 ||
               (_param1.size() == سائز(1, 4) && _param1.type() == CV_64F && cn <= 4)));
    CV_Assert( _param2.channels() == 1 &&
               (((_param2.صفیں == 1 || _param2.ستون_تعداد == 1) &&
                (_param2.صفیں + _param2.ستون_تعداد - 1 == cn || _param2.صفیں + _param2.ستون_تعداد - 1 == 1 ||
                (_param1.size() == سائز(1, 4) && _param1.type() == CV_64F && cn <= 4))) ||
                (_param2.صفیں == cn && _param2.ستون_تعداد == cn && disttype == معمول_3)));

    ثابت خالی* uni_param = 0;
    uchar* اوسط_2 = 0;
    uchar* stddev = 0;
    منطقی stdmtx = جھوٹ;
    صحیح n1 = (صحیح)_param1.کل();
    صحیح n2 = (صحیح)_param2.کل();

    اگر( disttype == یکساں_2 )
    {
        _parambuf.allocate(cn*(حجم(DivStruct)+حجم(دوہرا)-1)/حجم(دوہرا) + cn*4);
        دوہرا* parambuf = _parambuf.ڈیٹا();
        دوہرا* p1 = _param1.ptr<دوہرا>();
        دوہرا* p2 = _param2.ptr<دوہرا>();

        اگر( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convert_مقصد(tmp, CV_64F);
            برائے( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        اگر( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convert_مقصد(tmp, CV_64F);
            برائے( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        اگر( CV_IS_INT_TYPE(گہرائی) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            برائے( j = 0, fast_int_منوال = سچ; j < cn; j++ )
            {
                دوہرا a = std::min(p1[j], p2[j]);
                دوہرا b = std::max(p1[j], p2[j]);
                اگر( saturateRange )
                {
                    a = std::max(a, گہرائی == CV_8U || گہرائی == CV_16U || گہرائی == CV_32U ||
                                 گہرائی == CV_64U || گہرائی == CV_Bool ? 0. :
                                 گہرائی == CV_8S ? -128. : گہرائی == CV_16S ? -32768. :
                                 گہرائی == CV_32S ? (دوہرا)INT_MIN : (دوہرا)INT64_MIN);
                    b = std::min(b, گہرائی == CV_8U ? 256. : گہرائی == CV_Bool ? 2. : گہرائی == CV_16U ? 65536. :
                                 گہرائی == CV_8S ? 128. : گہرائی == CV_16S ? 32768. : گہرائی == CV_32U ? (دوہرا)UINT_MAX :
                                 گہرائی == CV_32S ? (دوہرا)زیادہ_سے_زیادہ_عدد : (دوہرا)INT64_MAX);
                }
                ip[j][1] = (int64_t)ceil(a);
                int64_t idiff = ip[j][0] = (int64_t)floor(b) - ip[j][1] - 1;
                اگر (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                دوہرا diff = b - a;

                fast_int_منوال = fast_int_منوال && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                اگر( fast_int_منوال )
                    small_flag = idiff <= 255;
                ورنہ
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    اگر (گہرائی == CV_64S || گہرائی == CV_64U)
                    {
                        minval = INT64_MIN/2;
                        maxval = INT64_MAX;
                    }
                    اگر( diff > (دوہرا)maxval )
                        ip[j][0] = maxval;
                    اگر( a < (دوہرا)minval )
                        ip[j][1] = minval;
                }
            }

            uni_param = ip;
            اگر( !fast_int_منوال )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((خالی*)(ds + cn), (خالی*)(parambuf + _parambuf.size()), "Last byte check");
                برائے( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    اگر (گہرائی != CV_64U && گہرائی != CV_64S) {
                        غیرعلامتی d = ds[j].d = (غیرعلامتی)(ip[j][0]+1);
                        صحیح l = 0;
                        جب(((uint64)1 << l) < d)
                            l++;
                        ds[j].M = (غیرعلامتی)(((uint64)1 << 32)*(((uint64)1 << l) - d)/d) + 1;
                        ds[j].sh1 = std::min(l, 1);
                        ds[j].sh2 = std::max(l - 1, 0);
                    }
                }
                uni_param = ds;
            }

            func = randTab[fast_int_منوال ? 1 : 0][گہرائی];
        }
        ورنہ
        {
            دوہرا scale = گہرائی == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            دوہرا maxdiff = saturateRange ? (دوہرا)FLT_MAX : DBL_MAX;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            اگر( گہرائی != CV_64F )
            {
                Vec2f* fp = (Vec2f*)(parambuf + cn*2);
                برائے( j = 0; j < cn; j++ )
                {
                    fp[j][0] = (اعشاری)(std::min(maxdiff, p2[j] - p1[j])*scale);
                    fp[j][1] = (اعشاری)((p2[j] + p1[j])*0.5);
                }
                uni_param = fp;
            }
            ورنہ
            {
                Vec2d* dp = (Vec2d*)(parambuf + cn*2);
                برائے( j = 0; j < cn; j++ )
                {
                    dp[j][0] = std::min(DBL_MAX, p2[j] - p1[j])*scale;
                    dp[j][1] = ((p2[j] + p1[j])*0.5);
                }
                uni_param = dp;
            }

            func = randTab[0][گہرائی];
        }
        CV_Assert( func != 0 );
    }
    ورنہ اگر( disttype == RNG::معمول_3 )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        دوہرا* parambuf = _parambuf.ڈیٹا();

        صحیح ptype = esz1 == 8 ? CV_64F : CV_32F;

        اگر( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            اوسط_2 = _param1.ptr();
        ورنہ
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convert_مقصد(tmp, ptype);
            اوسط_2 = (uchar*)parambuf;
        }

        اگر( n1 < cn )
            برائے( j = n1*esz1; j < cn*esz1; j++ )
                اوسط_2[j] = اوسط_2[j - n1*esz1];

        اگر( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        ورنہ
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convert_مقصد(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        اگر( n2 < cn )
            برائے( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.صفیں == cn && _param2.ستون_تعداد == cn;
        scaleFunc = randnScaleTab[گہرائی];
        CV_Assert( scaleFunc != 0 );
    }
    ورنہ
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    ثابت Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    اعشاری fbuf[بلاک_سائز + CV_CN_MAX];
    صحیح کل = (صحیح)it.size;
    صحیح بلاک_سائز_2 = std::min((بلاک_سائز + cn - 1)/cn, کل);
    size_t esz = (size_t)esz1*cn;
    صحیح flags = mat.type();

    اگر( disttype == یکساں_2 )
        flags |= (small_flag ? (صحیح)RNG_FLAG_SMALL : 0);
    ورنہ
        flags |= (stdmtx ? (صحیح)RNG_FLAG_STDMTX : 0);

    برائے( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برائے( j = 0; j < کل; j += بلاک_سائز_2 )
        {
            صحیح len = std::min(کل - j, بلاک_سائز_2);

            اگر( disttype == یکساں_2 )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            ورنہ
            {
                randn_0_1_32f(fbuf, len*cn, &state);
                scaleFunc(fbuf, ptr + j*esz, len, cn, اوسط_2, stddev, flags);
            }
        }
    }
}

}

cv::RNG& cv::theRNG()
{
    واپس get_Core_Tls_ڈیٹا().rng;
}

خالی cv::سیٹ_RNGSeed(صحیح بیج)
{
    theRNG() = RNG(static_cast<uint64>(بیج));
}


خالی cv::randu(داخلہ_Output_Array dst, داخلہ_Array کم, داخلہ_Array بلند)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::یکساں_2, کم, بلند);
}

خالی cv::randn(داخلہ_Output_Array dst, داخلہ_Array اوسط_2, داخلہ_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::معمول_3, اوسط_2, stddev);
}

نامخلا cv
{

سانچہ<قسم_نام T> ساکن خالی
randShuffle_( Mat& _arr, RNG& rng, دوہرا )
{
    غیرعلامتی sz = (غیرعلامتی)_arr.کل();
    اگر( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        برائے( غیرعلامتی i = 0; i < sz; i++ )
        {
            غیرعلامتی j = (غیرعلامتی)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    ورنہ
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* ڈیٹا = _arr.ptr();
        size_t قدم = _arr.قدم;
        صحیح صفیں = _arr.صفیں;
        صحیح ستون_تعداد = _arr.ستون_تعداد;
        برائے( صحیح i0 = 0; i0 < صفیں; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            برائے( صحیح j0 = 0; j0 < ستون_تعداد; j0++ )
            {
                غیرعلامتی k1 = (غیرعلامتی)rng % sz;
                صحیح i1 = (صحیح)(k1 / ستون_تعداد);
                صحیح j1 = (صحیح)(k1 - (غیرعلامتی)i1*(غیرعلامتی)ستون_تعداد);
                std::swap( p[j0], ((T*)(ڈیٹا + قدم*i1))[j1] );
            }
        }
    }
}

قسمتعریف خالی (*RandShuffleFunc)( Mat& dst, RNG& rng, دوہرا iter_ضربی_عدد );

}

خالی cv::randShuffle( داخلہ_Output_Array _dst, دوہرا iter_ضربی_عدد, RNG* _rng )
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
    CV_Assert( dst.elem_سائز() <= 32 );
    RandShuffleFunc func = tab[dst.elem_سائز()];
    CV_Assert( func != 0 );
    func( dst, rng, iter_ضربی_عدد );
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

cv::RNG_MT19937::RNG_MT19937(غیرعلامتی s) { بیج(s); }

cv::RNG_MT19937::RNG_MT19937() { بیج(5489U); }

خالی cv::RNG_MT19937::بیج(غیرعلامتی s)
{
    state[0]= s;
    برائے (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

غیرعلامتی cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    ساکن غیرعلامتی mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    ثابت غیرعلامتی بالائی_نقاب = 0x80000000U;
    ثابت غیرعلامتی زیریں_نقاب = 0x7fffffffU;

    /* generate N words at one time */
    اگر (mti >= N)
    {
        صحیح kk = 0;

        برائے (; kk < N - M; ++kk)
        {
            غیرعلامتی y = (state[kk] & بالائی_نقاب) | (state[kk + 1] & زیریں_نقاب);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        برائے (; kk < N - 1; ++kk)
        {
            غیرعلامتی y = (state[kk] & بالائی_نقاب) | (state[kk + 1] & زیریں_نقاب);
            state[kk] = state[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        غیرعلامتی y = (state[N - 1] & بالائی_نقاب) | (state[0] & زیریں_نقاب);
        state[N - 1] = state[M - 1] ^ (y >> 1) ^ mag01[y & 0x1U];

        mti = 0;
    }

    غیرعلامتی y = state[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y <<  7) & 0x9d2c5680U;
    y ^= (y << 15) & 0xefc60000U;
    y ^= (y >> 18);

    واپس y;
}

cv::RNG_MT19937::عامل غیرعلامتی() { واپس next(); }

cv::RNG_MT19937::عامل صحیح() { واپس (صحیح)next();}

cv::RNG_MT19937::عامل اعشاری() { واپس next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::عامل دوہرا()
{
    غیرعلامتی a = next() >> 5;
    غیرعلامتی b = next() >> 6;
    واپس (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

صحیح cv::RNG_MT19937::یکساں(صحیح a, صحیح b) { واپس (صحیح)(next() % (b - a) + a); }

اعشاری cv::RNG_MT19937::یکساں(اعشاری a, اعشاری b) { واپس ((اعشاری)*یہ)*(b - a) + a; }

دوہرا cv::RNG_MT19937::یکساں(دوہرا a, دوہرا b) { واپس ((دوہرا)*یہ)*(b - a) + a; }

غیرعلامتی cv::RNG_MT19937::عامل ()(غیرعلامتی b) { واپس next() % b; }

غیرعلامتی cv::RNG_MT19937::عامل ()() { واپس next(); }

/* End of file. */
