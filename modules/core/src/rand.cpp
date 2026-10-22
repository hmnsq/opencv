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

#అన్తర్భావ "precomp.hpp"

నామాకాశ cv
{

///////////////////////////// Functions Declaration //////////////////////////////////////

#నిర్వచించు CV_RNG_COEFF 4164903690U

/*
   Multiply-with-carry generator is used here:
   temp = ( A*X(n) + carry )
   X(n+1) = temp mod (2^32)
   carry = floor (temp / (2^32))
*/

#నిర్వచించు  RNG_తదుపరి(x)    ((uint64)(గుర్తులేని)(x)*CV_RNG_COEFF + ((x) >> 32))
// make it jump-less
#నిర్వచించు  CN_తదుపరి(k)     (((k) + 1) & (((k) >= cn) - 1))

గణన
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

మూస<రకం_పేరు T> స్థిర శూన్య
rand_బిట్లు( T* arr, పూర్ణాంకం len, పూర్ణాంకం cn, uint64* state, స్థిరరాశి Vec2l* p, పూర్ణాంకం flags )
{
    తార్కిక small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 తాత్కాలిక_2 = *state;
    పూర్ణాంకం i, k = 0;
    len *= cn;
    --cn;

    ఒకవేళ( !small_flag )
    {
        కొరకు( i = 0; i <= len - 4; i += 4 )
        {
            int64_t t0, t1;

            తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
            t0 = ((int64_t)తాత్కాలిక_2 & p[k][0]) + p[k][1];
            k = CN_తదుపరి(k);
            తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
            t1 = ((int64_t)తాత్కాలిక_2 & p[k][0]) + p[k][1];
            k = CN_తదుపరి(k);
            arr[i] = saturate_cast<T>(t0);
            arr[i+1] = saturate_cast<T>(t1);

            తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
            t0 = ((int64_t)తాత్కాలిక_2 & p[k][0]) + p[k][1];
            k = CN_తదుపరి(k);
            తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
            t1 = ((int64_t)తాత్కాలిక_2 & p[k][0]) + p[k][1];
            k = CN_తదుపరి(k);
            arr[i+2] = saturate_cast<T>(t0);
            arr[i+3] = saturate_cast<T>(t1);
        }
    }
    లేకపోతే
    {
        కొరకు( i = 0; i <= len - 4; i += 4 )
        {
            int64_t t0, t1, t;
            తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
            t = తాత్కాలిక_2;
            // p[i+...][0] is within 0..255 in this branch (small_flag==true),
            // so we don't need to do (t>>...)&255,
            // the upper bits will be cleaned with ... & p[i+...][0].
            t0 = (t & p[k][0]) + p[k][1];
            k = CN_తదుపరి(k);
            t1 = ((t >> 8) & p[k][0]) + p[k][1];
            k = CN_తదుపరి(k);
            arr[i] = saturate_cast<T>(t0);
            arr[i+1] = saturate_cast<T>(t1);

            t0 = ((t >> 16) & p[k][0]) + p[k][1];
            k = CN_తదుపరి(k);
            t1 = ((t >> 24) & p[k][0]) + p[k][1];
            k = CN_తదుపరి(k);
            arr[i+2] = saturate_cast<T>(t0);
            arr[i+3] = saturate_cast<T>(t1);
        }
    }

    కొరకు( ; i < len; i++ )
    {
        int64_t t0;
        తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);

        t0 = ((int64_t)తాత్కాలిక_2 & p[k][0]) + p[k][1];
        k = CN_తదుపరి(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = తాత్కాలిక_2;
}

నిర్మాణం DivStruct
{
    గుర్తులేని d;
    గుర్తులేని M;
    పూర్ణాంకం sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

మూస<రకం_పేరు T> స్థిర శూన్య
randi_( T* arr, పూర్ణాంకం len, పూర్ణాంకం cn, uint64* state, స్థిరరాశి DivStruct* p )
{
    uint64 తాత్కాలిక_2 = *state;
    పూర్ణాంకం k = 0;
    len *= cn;
    cn--;
    కొరకు( పూర్ణాంకం i = 0; i < len; i++ )
    {
        తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
        గుర్తులేని t = (గుర్తులేని)తాత్కాలిక_2;
        గుర్తులేని v = (గుర్తులేని)(((uint64)t * p[k].M) >> 32);
        v = (v + ((t - v) >> p[k].sh1)) >> p[k].sh2;
        int64_t res = (int64_t)(t - v*p[k].d) + p[k].delta;
        k = CN_తదుపరి(k);
        arr[i] = saturate_cast<T>(res);
    }
    *state = తాత్కాలిక_2;
}

స్థిర శూన్య
randi_( int64_t* arr, పూర్ణాంకం len, పూర్ణాంకం cn, uint64* state, స్థిరరాశి DivStruct* p )
{
    uint64 తాత్కాలిక_2 = *state;
    పూర్ణాంకం k = 0;
    len *= cn;
    cn--;
    కొరకు( పూర్ణాంకం i = 0; i < len; i++ )
    {
        తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
        గుర్తులేని t0 = (గుర్తులేని)తాత్కాలిక_2;
        తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
        గుర్తులేని t1 = (గుర్తులేని)తాత్కాలిక_2;
        int64_t t = (int64_t)((((uint64_t)t0 << 32) | t1) % p[k].diff) + p[k].delta;
        k = CN_తదుపరి(k);
        arr[i] = t;
    }
    *state = తాత్కాలిక_2;
}

స్థిర శూన్య
randi_( uint64_t* arr, పూర్ణాంకం len, పూర్ణాంకం cn, uint64* state, స్థిరరాశి DivStruct* p )
{
    uint64 తాత్కాలిక_2 = *state;
    పూర్ణాంకం k = 0;
    len *= cn;
    cn--;
    కొరకు( పూర్ణాంకం i = 0; i < len; i++ )
    {
        తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
        గుర్తులేని t0 = (గుర్తులేని)తాత్కాలిక_2;
        తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
        గుర్తులేని t1 = (గుర్తులేని)తాత్కాలిక_2;
        uint64_t t = (((uint64_t)t0 << 32) | t1) % p[k].diff;
        int64_t delta = p[k].delta;
        k = CN_తదుపరి(k);
        arr[i] = delta >= 0 || t >= (uint64_t)-delta ? t + (uint64_t)delta : 0;
    }
    *state = తాత్కాలిక_2;
}

మూస<రకం_పేరు T>
స్థిర శూన్య rand_బిట్లు_Wrap(శూన్య* arr, పూర్ణాంకం len, పూర్ణాంకం cn, uint64* state, స్థిరరాశి శూన్య* p, శూన్య*, పూర్ణాంకం flags)
{
    rand_బిట్లు((T*)arr, len, cn, state, (స్థిరరాశి Vec2l*)p, flags);
}

మూస<రకం_పేరు T>
స్థిర శూన్య randiWrap(శూన్య* arr, పూర్ణాంకం len, పూర్ణాంకం cn, uint64* state, స్థిరరాశి శూన్య* p, శూన్య*, పూర్ణాంకం)
{
    randi_((T*)arr, len, cn, state, (స్థిరరాశి DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
స్థిర అంతఃవరుస శూన్య cvt32f_గమ్యం_FP8(స్థిరరాశి దశాంశం* src, శూన్య* dst, పూర్ణాంకం len, పూర్ణాంకం లోతు)
{
    ఒకవేళ (లోతు == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   కొరకు (పూర్ణాంకం i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    లేకపోతే                              { fp8a_t* d = (fp8a_t*)dst; కొరకు (పూర్ణాంకం i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
స్థిర అంతఃవరుస తార్కిక is_FP8_లోతు(పూర్ణాంకం d) { తిరిగి d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

స్థిర శూన్య randf_16_or_32f( శూన్య* dst, పూర్ణాంకం len_, పూర్ణాంకం cn, uint64* state, స్థిరరాశి శూన్య* _p, శూన్య* _fbuf, పూర్ణాంకం flags )
{
    స్థిరరాశి Vec2f* p = (స్థిరరాశి Vec2f*)_p;
    దశాంశం* fbuf = (దశాంశం*)_fbuf;
    పూర్ణాంకం లోతు = CV_MAT_లోతు(flags);
    uint64 తాత్కాలిక_2 = *state;
    పూర్ణాంకం k = 0, len = len_*cn;
    దశాంశం* arr = లోతు == CV_16F || లోతు == CV_16BF || is_FP8_లోతు(లోతు) ? fbuf : (దశాంశం*)dst;
    cn--;
    కొరకు( పూర్ణాంకం i = 0; i < len; i++ )
    {
        పూర్ణాంకం t = (పూర్ణాంకం)(తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2));
        arr[i] = (దశాంశం)(t*p[k][0]);
        k = CN_తదుపరి(k);
    }
    *state = తాత్కాలిక_2;
    hal::కూడు_RNGBias32f(arr, &p[0][0], len_, cn+1);
    ఒకవేళ (లోతు == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    లేకపోతే ఒకవేళ (లోతు == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    లేకపోతే ఒకవేళ (is_FP8_లోతు(లోతు))
        cvt32f_గమ్యం_FP8(fbuf, dst, len, లోతు);
}

స్థిర శూన్య
randf_64f( శూన్య* _arr, పూర్ణాంకం len_, పూర్ణాంకం cn, uint64* state, స్థిరరాశి శూన్య* _p, శూన్య*, పూర్ణాంకం )
{
    ద్విగుణ* arr = (ద్విగుణ*)_arr;
    స్థిరరాశి Vec2d* p = (స్థిరరాశి Vec2d*)_p;
    uint64 తాత్కాలిక_2 = *state;
    పూర్ణాంకం k = 0, len = len_*cn;
    cn--;
    కొరకు( పూర్ణాంకం i = 0; i < len; i++ )
    {
        తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
        int64_t v = (int64_t)((తాత్కాలిక_2 >> 32) | (తాత్కాలిక_2 << 32));
        arr[i] = v*p[k][0];
        k = CN_తదుపరి(k);
    }
    *state = తాత్కాలిక_2;
    hal::కూడు_RNGBias64f(arr, &p[0][0], len_, cn+1);
}

రకనిర్వచనం శూన్య (*RandFunc)(శూన్య* arr, పూర్ణాంకం len, పూర్ణాంకం cn, uint64* state,
                         స్థిరరాశి శూన్య* p, శూన్య* tempbuf, పూర్ణాంకం flags);

స్థిర RandFunc randTab[CV_లోతు_MAX][CV_లోతు_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<పొట్టి>, randiWrap<పూర్ణాంకం>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<తార్కిక>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<గుర్తులేని>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        rand_బిట్లు_Wrap<uchar>, rand_బిట్లు_Wrap<schar>, rand_బిట్లు_Wrap<ushort>,
        rand_బిట్లు_Wrap<పొట్టి>, rand_బిట్లు_Wrap<పూర్ణాంకం>, 0, 0, 0, 0,
        rand_బిట్లు_Wrap<తార్కిక>, rand_బిట్లు_Wrap<uint64_t>, rand_బిట్లు_Wrap<int64_t>,
        rand_బిట్లు_Wrap<గుర్తులేని>, 0, 0, 0
    }
};

/*
   The code below implements the algorithm described in
   "The Ziggurat Method for Generating Random Variables"
   by George Marsaglia and Wai Wan Tsang, Journal of Statistical Software, 2007.
*/
స్థిర శూన్య
randn_0_1_32f( దశాంశం* arr, పూర్ణాంకం len, uint64* state )
{
    స్థిరరాశి దశాంశం r = 3.442620f; // The start of the right tail
    స్థిరరాశి దశాంశం rng_flt = 2.3283064365386962890625e-10f; // 2^-32
    స్థిర గుర్తులేని kn[128];
    స్థిర దశాంశం wn[128], fn[128];
    uint64 తాత్కాలిక_2 = *state;
    స్థిర తార్కిక initialized=అబద్ధం;
    పూర్ణాంకం i;

    ఒకవేళ( !initialized )
    {
        స్థిరరాశి ద్విగుణ m1 = 2147483648.0;
        ద్విగుణ dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        ద్విగుణ q = vn/std::ఘాతాంకం(-.5*dn*dn);
        kn[0] = (గుర్తులేని)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (దశాంశం)(q/m1);
        wn[127] = (దశాంశం)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (దశాంశం)std::ఘాతాంకం(-.5*dn*dn);

        కొరకు(i=126;i>=1;i--)
        {
            dn = std::వర్గమూలం(-2.*std::సంవర్గమానం(vn/dn+std::ఘాతాంకం(-.5*dn*dn)));
            kn[i+1] = (గుర్తులేని)((dn/tn)*m1);
            tn = dn;
            fn[i] = (దశాంశం)std::ఘాతాంకం(-.5*dn*dn);
            wn[i] = (దశాంశం)(dn/m1);
        }
        initialized = నిజం;
    }

    కొరకు( i = 0; i < len; i++ )
    {
        దశాంశం x, y;
        కొరకు(;;)
        {
            పూర్ణాంకం hz = (పూర్ణాంకం)తాత్కాలిక_2;
            తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
            పూర్ణాంకం iz = hz & 127;
            x = hz*wn[iz];
            ఒకవేళ( (గుర్తులేని)std::నిరపేక్ష(hz) < kn[iz] )
                ఆపు;
            ఒకవేళ( iz == 0) // iz==0, handles the base strip
            {
                చేయి
                {
                    x = (గుర్తులేని)తాత్కాలిక_2*rng_flt;
                    తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
                    y = (గుర్తులేని)తాత్కాలిక_2*rng_flt;
                    తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
                    x = (దశాంశం)(-std::సంవర్గమానం(x+FLT_MIN)*0.2904764);
                    y = (దశాంశం)-std::సంవర్గమానం(y+FLT_MIN);
                }	// .2904764 is 1/r
                అయితే( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                ఆపు;
            }
            // iz > 0, handle the wedges of other strips
            y = (గుర్తులేని)తాత్కాలిక_2*rng_flt;
            తాత్కాలిక_2 = RNG_తదుపరి(తాత్కాలిక_2);
            ఒకవేళ( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::ఘాతాంకం(-.5*x*x) )
                ఆపు;
        }
        arr[i] = x;
    }
    *state = తాత్కాలిక_2;
}


ద్విగుణ RNG::gaussian(ద్విగుణ sigma)
{
    దశాంశం తాత్కాలిక_2;
    randn_0_1_32f( &తాత్కాలిక_2, 1, &state );
    తిరిగి తాత్కాలిక_2*sigma;
}

మూస<రకం_పేరు T, రకం_పేరు PT> స్థిర శూన్య
randnScale_(దశాంశం* src, శూన్య* _dst, పూర్ణాంకం len, పూర్ణాంకం cn,
            స్థిరరాశి శూన్య* సగటు, స్థిరరాశి శూన్య* _stddev, పూర్ణాంకం flags )
{
    T* dst = (T*)_dst;
    స్థిరరాశి PT* సగటు_2 = (స్థిరరాశి PT*)సగటు;
    స్థిరరాశి PT* stddev = (స్థిరరాశి PT*)_stddev;
    తార్కిక stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    పూర్ణాంకం i, j, k;
    ఒకవేళ( !stdmtx || cn == 1 )
    {
        ఒకవేళ( cn == 1 )
        {
            PT a = stddev[0], b = సగటు_2[0];
            కొరకు( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        లేకపోతే
        {
            len *= cn;
            cn--;
            కొరకు( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + సగటు_2[k]);
                k = CN_తదుపరి(k);
            }
        }
    }
    లేకపోతే
    {
        len *= cn;
        cn--;
        కొరకు( i = j = 0; i < len; i++ )
        {
            PT s = సగటు_2[j];
            పూర్ణాంకం i0 = i - j;
            కొరకు( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_తదుపరి(j);
        }
    }
}

// special version for 16f, 16bf and 32f
స్థిర శూన్య
randnScale_16_or_32f(దశాంశం* fbuf, శూన్య* _dst, పూర్ణాంకం len, పూర్ణాంకం cn,
                     స్థిరరాశి శూన్య* సగటు, స్థిరరాశి శూన్య* _stddev, పూర్ణాంకం flags)
{
    దశాంశం* dst = (దశాంశం*)_dst;
    స్థిరరాశి దశాంశం* సగటు_2 = (స్థిరరాశి దశాంశం*)సగటు;
    స్థిరరాశి దశాంశం* stddev = (స్థిరరాశి దశాంశం*)_stddev;
    తార్కిక stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    పూర్ణాంకం లోతు = CV_MAT_లోతు(flags);
    దశాంశం* arr = లోతు == CV_16F || లోతు == CV_16BF || is_FP8_లోతు(లోతు) ? fbuf : dst;
    పూర్ణాంకం i, j, k;

    ఒకవేళ( !stdmtx || cn == 1 )
    {
        ఒకవేళ( cn == 1 )
        {
            దశాంశం a = stddev[0], b = సగటు_2[0];
            కొరకు( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        లేకపోతే
        {
            len *= cn;
            cn--;
            కొరకు( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + సగటు_2[k];
                k = CN_తదుపరి(k);
            }
        }
    }
    లేకపోతే ఒకవేళ( లోతు == CV_32F )
    {
        len *= cn;
        cn--;
        కొరకు( i = j = 0; i < len; i++ )
        {
            దశాంశం s = సగటు_2[j];
            పూర్ణాంకం i0 = i - j;
            కొరకు( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_తదుపరి(j);
        }
    }
    లేకపోతే
    {
        దశాంశం elembuf[CV_CN_MAX];
        len *= cn;
        కొరకు( i = 0; i < len; i += cn )
        {
            // since we process fbuf in-place,
            // we need to copy each cn-channel element
            // prior to matrix multiplication
            కొరకు (j = 0; j < cn; j++)
                elembuf[j] = fbuf[i + j];
            కొరకు (j = 0; j < cn; j++) {
                దశాంశం s = సగటు_2[j];
                కొరకు( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    ఒకవేళ (లోతు == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    లేకపోతే ఒకవేళ (లోతు == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    లేకపోతే ఒకవేళ (is_FP8_లోతు(లోతు))
        cvt32f_గమ్యం_FP8(fbuf, dst, len, లోతు);
}

రకనిర్వచనం శూన్య (*RandnScaleFunc)(దశాంశం* src, శూన్య* dst, పూర్ణాంకం len, పూర్ణాంకం cn,
                               స్థిరరాశి శూన్య* సగటు_2, స్థిరరాశి శూన్య* stddev, పూర్ణాంకం flags);

స్థిర RandnScaleFunc randnScaleTab[CV_లోతు_MAX] =
{
    randnScale_<uchar, దశాంశం>, randnScale_<schar, దశాంశం>, randnScale_<ushort, దశాంశం>,
    randnScale_<పొట్టి, దశాంశం>, randnScale_<పూర్ణాంకం, దశాంశం>, randnScale_16_or_32f,
    randnScale_<ద్విగుణ, ద్విగుణ>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<తార్కిక, దశాంశం>, randnScale_<uint64_t, ద్విగుణ>, randnScale_<int64_t, ద్విగుణ>,
    randnScale_<గుర్తులేని, దశాంశం>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

శూన్య RNG::fill( ఆదానం_Output_Array _mat, పూర్ణాంకం disttype,
                ఆదానం_Array _param1arg, ఆదానం_Array _param2arg,
                తార్కిక saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    పూర్ణాంకం j, లోతు = mat.లోతు(), cn = mat.channels();
    పూర్ణాంకం esz1 = CV_ELEM_కొలత(లోతు);
    Auto_బఫరు<ద్విగుణ> _parambuf;
    తార్కిక fast_int_బహుళకం = అబద్ధం;
    తార్కిక small_flag = అబద్ధం;
    RandFunc func = 0;
    RandnScaleFunc scaleFunc = 0;

    CV_Assert(_param1.channels() == 1 && (_param1.అడ్డువరుసలు == 1 || _param1.నిలువుల_సంఖ్య == 1) &&
              (_param1.అడ్డువరుసలు + _param1.నిలువుల_సంఖ్య - 1 == cn || _param1.అడ్డువరుసలు + _param1.నిలువుల_సంఖ్య - 1 == 1 ||
               (_param1.size() == కొలత(1, 4) && _param1.type() == CV_64F && cn <= 4)));
    CV_Assert( _param2.channels() == 1 &&
               (((_param2.అడ్డువరుసలు == 1 || _param2.నిలువుల_సంఖ్య == 1) &&
                (_param2.అడ్డువరుసలు + _param2.నిలువుల_సంఖ్య - 1 == cn || _param2.అడ్డువరుసలు + _param2.నిలువుల_సంఖ్య - 1 == 1 ||
                (_param1.size() == కొలత(1, 4) && _param1.type() == CV_64F && cn <= 4))) ||
                (_param2.అడ్డువరుసలు == cn && _param2.నిలువుల_సంఖ్య == cn && disttype == సాధారణ_3)));

    స్థిరరాశి శూన్య* uni_param = 0;
    uchar* సగటు_2 = 0;
    uchar* stddev = 0;
    తార్కిక stdmtx = అబద్ధం;
    పూర్ణాంకం n1 = (పూర్ణాంకం)_param1.మొత్తం();
    పూర్ణాంకం n2 = (పూర్ణాంకం)_param2.మొత్తం();

    ఒకవేళ( disttype == ఏకరీతి_2 )
    {
        _parambuf.allocate(cn*(పరిమాణం(DivStruct)+పరిమాణం(ద్విగుణ)-1)/పరిమాణం(ద్విగుణ) + cn*4);
        ద్విగుణ* parambuf = _parambuf.దత్తాంశం();
        ద్విగుణ* p1 = _param1.ptr<ద్విగుణ>();
        ద్విగుణ* p2 = _param2.ptr<ద్విగుణ>();

        ఒకవేళ( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convert_గమ్యం(tmp, CV_64F);
            కొరకు( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        ఒకవేళ( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convert_గమ్యం(tmp, CV_64F);
            కొరకు( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        ఒకవేళ( CV_IS_INT_TYPE(లోతు) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            కొరకు( j = 0, fast_int_బహుళకం = నిజం; j < cn; j++ )
            {
                ద్విగుణ a = std::min(p1[j], p2[j]);
                ద్విగుణ b = std::max(p1[j], p2[j]);
                ఒకవేళ( saturateRange )
                {
                    a = std::max(a, లోతు == CV_8U || లోతు == CV_16U || లోతు == CV_32U ||
                                 లోతు == CV_64U || లోతు == CV_Bool ? 0. :
                                 లోతు == CV_8S ? -128. : లోతు == CV_16S ? -32768. :
                                 లోతు == CV_32S ? (ద్విగుణ)INT_MIN : (ద్విగుణ)INT64_MIN);
                    b = std::min(b, లోతు == CV_8U ? 256. : లోతు == CV_Bool ? 2. : లోతు == CV_16U ? 65536. :
                                 లోతు == CV_8S ? 128. : లోతు == CV_16S ? 32768. : లోతు == CV_32U ? (ద్విగుణ)UINT_MAX :
                                 లోతు == CV_32S ? (ద్విగుణ)గరిష్ఠ_పూర్ణాంకం : (ద్విగుణ)INT64_MAX);
                }
                ip[j][1] = (int64_t)ceil(a);
                int64_t idiff = ip[j][0] = (int64_t)floor(b) - ip[j][1] - 1;
                ఒకవేళ (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                ద్విగుణ diff = b - a;

                fast_int_బహుళకం = fast_int_బహుళకం && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                ఒకవేళ( fast_int_బహుళకం )
                    small_flag = idiff <= 255;
                లేకపోతే
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    ఒకవేళ (లోతు == CV_64S || లోతు == CV_64U)
                    {
                        minval = INT64_MIN/2;
                        maxval = INT64_MAX;
                    }
                    ఒకవేళ( diff > (ద్విగుణ)maxval )
                        ip[j][0] = maxval;
                    ఒకవేళ( a < (ద్విగుణ)minval )
                        ip[j][1] = minval;
                }
            }

            uni_param = ip;
            ఒకవేళ( !fast_int_బహుళకం )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((శూన్య*)(ds + cn), (శూన్య*)(parambuf + _parambuf.size()), "Last byte check");
                కొరకు( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    ఒకవేళ (లోతు != CV_64U && లోతు != CV_64S) {
                        గుర్తులేని d = ds[j].d = (గుర్తులేని)(ip[j][0]+1);
                        పూర్ణాంకం l = 0;
                        అయితే(((uint64)1 << l) < d)
                            l++;
                        ds[j].M = (గుర్తులేని)(((uint64)1 << 32)*(((uint64)1 << l) - d)/d) + 1;
                        ds[j].sh1 = std::min(l, 1);
                        ds[j].sh2 = std::max(l - 1, 0);
                    }
                }
                uni_param = ds;
            }

            func = randTab[fast_int_బహుళకం ? 1 : 0][లోతు];
        }
        లేకపోతే
        {
            ద్విగుణ scale = లోతు == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            ద్విగుణ maxdiff = saturateRange ? (ద్విగుణ)FLT_MAX : DBL_MAX;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            ఒకవేళ( లోతు != CV_64F )
            {
                Vec2f* fp = (Vec2f*)(parambuf + cn*2);
                కొరకు( j = 0; j < cn; j++ )
                {
                    fp[j][0] = (దశాంశం)(std::min(maxdiff, p2[j] - p1[j])*scale);
                    fp[j][1] = (దశాంశం)((p2[j] + p1[j])*0.5);
                }
                uni_param = fp;
            }
            లేకపోతే
            {
                Vec2d* dp = (Vec2d*)(parambuf + cn*2);
                కొరకు( j = 0; j < cn; j++ )
                {
                    dp[j][0] = std::min(DBL_MAX, p2[j] - p1[j])*scale;
                    dp[j][1] = ((p2[j] + p1[j])*0.5);
                }
                uni_param = dp;
            }

            func = randTab[0][లోతు];
        }
        CV_Assert( func != 0 );
    }
    లేకపోతే ఒకవేళ( disttype == RNG::సాధారణ_3 )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        ద్విగుణ* parambuf = _parambuf.దత్తాంశం();

        పూర్ణాంకం ptype = esz1 == 8 ? CV_64F : CV_32F;

        ఒకవేళ( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            సగటు_2 = _param1.ptr();
        లేకపోతే
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convert_గమ్యం(tmp, ptype);
            సగటు_2 = (uchar*)parambuf;
        }

        ఒకవేళ( n1 < cn )
            కొరకు( j = n1*esz1; j < cn*esz1; j++ )
                సగటు_2[j] = సగటు_2[j - n1*esz1];

        ఒకవేళ( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        లేకపోతే
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convert_గమ్యం(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        ఒకవేళ( n2 < cn )
            కొరకు( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.అడ్డువరుసలు == cn && _param2.నిలువుల_సంఖ్య == cn;
        scaleFunc = randnScaleTab[లోతు];
        CV_Assert( scaleFunc != 0 );
    }
    లేకపోతే
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    స్థిరరాశి Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    దశాంశం fbuf[ఖండం_కొలత + CV_CN_MAX];
    పూర్ణాంకం మొత్తం = (పూర్ణాంకం)it.size;
    పూర్ణాంకం ఖండం_కొలత_2 = std::min((ఖండం_కొలత + cn - 1)/cn, మొత్తం);
    size_t esz = (size_t)esz1*cn;
    పూర్ణాంకం flags = mat.type();

    ఒకవేళ( disttype == ఏకరీతి_2 )
        flags |= (small_flag ? (పూర్ణాంకం)RNG_FLAG_SMALL : 0);
    లేకపోతే
        flags |= (stdmtx ? (పూర్ణాంకం)RNG_FLAG_STDMTX : 0);

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        కొరకు( j = 0; j < మొత్తం; j += ఖండం_కొలత_2 )
        {
            పూర్ణాంకం len = std::min(మొత్తం - j, ఖండం_కొలత_2);

            ఒకవేళ( disttype == ఏకరీతి_2 )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            లేకపోతే
            {
                randn_0_1_32f(fbuf, len*cn, &state);
                scaleFunc(fbuf, ptr + j*esz, len, cn, సగటు_2, stddev, flags);
            }
        }
    }
}

}

cv::RNG& cv::theRNG()
{
    తిరిగి get_Core_Tls_దత్తాంశం().rng;
}

శూన్య cv::సమితి_RNGSeed(పూర్ణాంకం విత్తనం)
{
    theRNG() = RNG(static_cast<uint64>(విత్తనం));
}


శూన్య cv::randu(ఆదానం_Output_Array dst, ఆదానం_Array దిగువ, ఆదానం_Array ఎగువ)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::ఏకరీతి_2, దిగువ, ఎగువ);
}

శూన్య cv::randn(ఆదానం_Output_Array dst, ఆదానం_Array సగటు_2, ఆదానం_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::సాధారణ_3, సగటు_2, stddev);
}

నామాకాశ cv
{

మూస<రకం_పేరు T> స్థిర శూన్య
randShuffle_( Mat& _arr, RNG& rng, ద్విగుణ )
{
    గుర్తులేని sz = (గుర్తులేని)_arr.మొత్తం();
    ఒకవేళ( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        కొరకు( గుర్తులేని i = 0; i < sz; i++ )
        {
            గుర్తులేని j = (గుర్తులేని)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    లేకపోతే
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* దత్తాంశం = _arr.ptr();
        size_t అడుగు = _arr.అడుగు;
        పూర్ణాంకం అడ్డువరుసలు = _arr.అడ్డువరుసలు;
        పూర్ణాంకం నిలువుల_సంఖ్య = _arr.నిలువుల_సంఖ్య;
        కొరకు( పూర్ణాంకం i0 = 0; i0 < అడ్డువరుసలు; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            కొరకు( పూర్ణాంకం j0 = 0; j0 < నిలువుల_సంఖ్య; j0++ )
            {
                గుర్తులేని k1 = (గుర్తులేని)rng % sz;
                పూర్ణాంకం i1 = (పూర్ణాంకం)(k1 / నిలువుల_సంఖ్య);
                పూర్ణాంకం j1 = (పూర్ణాంకం)(k1 - (గుర్తులేని)i1*(గుర్తులేని)నిలువుల_సంఖ్య);
                std::swap( p[j0], ((T*)(దత్తాంశం + అడుగు*i1))[j1] );
            }
        }
    }
}

రకనిర్వచనం శూన్య (*RandShuffleFunc)( Mat& dst, RNG& rng, ద్విగుణ iter_గుణకం );

}

శూన్య cv::randShuffle( ఆదానం_Output_Array _dst, ద్విగుణ iter_గుణకం, RNG* _rng )
{
    CV_INSTRUMENT_REGION();

    RandShuffleFunc tab[] =
    {
        0,
        randShuffle_<uchar>, // 1
        randShuffle_<ushort>, // 2
        randShuffle_<Vec<uchar,3> >, // 3
        randShuffle_<పూర్ణాంకం>, // 4
        0,
        randShuffle_<Vec<ushort,3> >, // 6
        0,
        randShuffle_<Vec<పూర్ణాంకం,2> >, // 8
        0, 0, 0,
        randShuffle_<Vec<పూర్ణాంకం,3> >, // 12
        0, 0, 0,
        randShuffle_<Vec<పూర్ణాంకం,4> >, // 16
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<పూర్ణాంకం,6> >, // 24
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<పూర్ణాంకం,8> > // 32
    };

    Mat dst = _dst.getMat();
    RNG& rng = _rng ? *_rng : theRNG();
    CV_Assert( dst.elem_కొలత() <= 32 );
    RandShuffleFunc func = tab[dst.elem_కొలత()];
    CV_Assert( func != 0 );
    func( dst, rng, iter_గుణకం );
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

cv::RNG_MT19937::RNG_MT19937(గుర్తులేని s) { విత్తనం(s); }

cv::RNG_MT19937::RNG_MT19937() { విత్తనం(5489U); }

శూన్య cv::RNG_MT19937::విత్తనం(గుర్తులేని s)
{
    state[0]= s;
    కొరకు (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

గుర్తులేని cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    స్థిర గుర్తులేని mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    స్థిరరాశి గుర్తులేని పై_ముసుగు = 0x80000000U;
    స్థిరరాశి గుర్తులేని క్రింది_ముసుగు = 0x7fffffffU;

    /* generate N words at one time */
    ఒకవేళ (mti >= N)
    {
        పూర్ణాంకం kk = 0;

        కొరకు (; kk < N - M; ++kk)
        {
            గుర్తులేని y = (state[kk] & పై_ముసుగు) | (state[kk + 1] & క్రింది_ముసుగు);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        కొరకు (; kk < N - 1; ++kk)
        {
            గుర్తులేని y = (state[kk] & పై_ముసుగు) | (state[kk + 1] & క్రింది_ముసుగు);
            state[kk] = state[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        గుర్తులేని y = (state[N - 1] & పై_ముసుగు) | (state[0] & క్రింది_ముసుగు);
        state[N - 1] = state[M - 1] ^ (y >> 1) ^ mag01[y & 0x1U];

        mti = 0;
    }

    గుర్తులేని y = state[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y <<  7) & 0x9d2c5680U;
    y ^= (y << 15) & 0xefc60000U;
    y ^= (y >> 18);

    తిరిగి y;
}

cv::RNG_MT19937::క్రియాకారకం గుర్తులేని() { తిరిగి next(); }

cv::RNG_MT19937::క్రియాకారకం పూర్ణాంకం() { తిరిగి (పూర్ణాంకం)next();}

cv::RNG_MT19937::క్రియాకారకం దశాంశం() { తిరిగి next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::క్రియాకారకం ద్విగుణ()
{
    గుర్తులేని a = next() >> 5;
    గుర్తులేని b = next() >> 6;
    తిరిగి (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

పూర్ణాంకం cv::RNG_MT19937::ఏకరీతి(పూర్ణాంకం a, పూర్ణాంకం b) { తిరిగి (పూర్ణాంకం)(next() % (b - a) + a); }

దశాంశం cv::RNG_MT19937::ఏకరీతి(దశాంశం a, దశాంశం b) { తిరిగి ((దశాంశం)*ఇది)*(b - a) + a; }

ద్విగుణ cv::RNG_MT19937::ఏకరీతి(ద్విగుణ a, ద్విగుణ b) { తిరిగి ((ద్విగుణ)*ఇది)*(b - a) + a; }

గుర్తులేని cv::RNG_MT19937::క్రియాకారకం ()(గుర్తులేని b) { తిరిగి next() % b; }

గుర్తులేని cv::RNG_MT19937::క్రియాకారకం ()() { తిరిగి next(); }

/* End of file. */
