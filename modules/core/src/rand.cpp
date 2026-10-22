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

#अन्तर्भाव "precomp.hpp"

नामाकाश cv
{

///////////////////////////// Functions Declaration //////////////////////////////////////

#परिभाषय CV_RNG_COEFF 4164903690U

/*
   Multiply-with-carry generator is used here:
   temp = ( A*X(n) + carry )
   X(n+1) = temp mod (2^32)
   carry = floor (temp / (2^32))
*/

#परिभाषय  RNG_अग्र(x)    ((uint64)(अचिह्नित)(x)*CV_RNG_COEFF + ((x) >> 32))
// make it jump-less
#परिभाषय  CN_अग्र(k)     (((k) + 1) & (((k) >= cn) - 1))

गणना
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

प्रतिरूपम्<प्रकारनाम T> स्थिर शून्य
rand_द्वयङ्काः( T* arr, पूर्णाङ्क len, पूर्णाङ्क cn, uint64* state, ध्रुव Vec2l* p, पूर्णाङ्क flags )
{
    तार्किक small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 अस्थायी_2 = *state;
    पूर्णाङ्क i, k = 0;
    len *= cn;
    --cn;

    यदि( !small_flag )
    {
        प्रति( i = 0; i <= len - 4; i += 4 )
        {
            int64_t t0, t1;

            अस्थायी_2 = RNG_अग्र(अस्थायी_2);
            t0 = ((int64_t)अस्थायी_2 & p[k][0]) + p[k][1];
            k = CN_अग्र(k);
            अस्थायी_2 = RNG_अग्र(अस्थायी_2);
            t1 = ((int64_t)अस्थायी_2 & p[k][0]) + p[k][1];
            k = CN_अग्र(k);
            arr[i] = saturate_cast<T>(t0);
            arr[i+1] = saturate_cast<T>(t1);

            अस्थायी_2 = RNG_अग्र(अस्थायी_2);
            t0 = ((int64_t)अस्थायी_2 & p[k][0]) + p[k][1];
            k = CN_अग्र(k);
            अस्थायी_2 = RNG_अग्र(अस्थायी_2);
            t1 = ((int64_t)अस्थायी_2 & p[k][0]) + p[k][1];
            k = CN_अग्र(k);
            arr[i+2] = saturate_cast<T>(t0);
            arr[i+3] = saturate_cast<T>(t1);
        }
    }
    अन्यथा
    {
        प्रति( i = 0; i <= len - 4; i += 4 )
        {
            int64_t t0, t1, t;
            अस्थायी_2 = RNG_अग्र(अस्थायी_2);
            t = अस्थायी_2;
            // p[i+...][0] is within 0..255 in this branch (small_flag==true),
            // so we don't need to do (t>>...)&255,
            // the upper bits will be cleaned with ... & p[i+...][0].
            t0 = (t & p[k][0]) + p[k][1];
            k = CN_अग्र(k);
            t1 = ((t >> 8) & p[k][0]) + p[k][1];
            k = CN_अग्र(k);
            arr[i] = saturate_cast<T>(t0);
            arr[i+1] = saturate_cast<T>(t1);

            t0 = ((t >> 16) & p[k][0]) + p[k][1];
            k = CN_अग्र(k);
            t1 = ((t >> 24) & p[k][0]) + p[k][1];
            k = CN_अग्र(k);
            arr[i+2] = saturate_cast<T>(t0);
            arr[i+3] = saturate_cast<T>(t1);
        }
    }

    प्रति( ; i < len; i++ )
    {
        int64_t t0;
        अस्थायी_2 = RNG_अग्र(अस्थायी_2);

        t0 = ((int64_t)अस्थायी_2 & p[k][0]) + p[k][1];
        k = CN_अग्र(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = अस्थायी_2;
}

संरचना DivStruct
{
    अचिह्नित d;
    अचिह्नित M;
    पूर्णाङ्क sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

प्रतिरूपम्<प्रकारनाम T> स्थिर शून्य
randi_( T* arr, पूर्णाङ्क len, पूर्णाङ्क cn, uint64* state, ध्रुव DivStruct* p )
{
    uint64 अस्थायी_2 = *state;
    पूर्णाङ्क k = 0;
    len *= cn;
    cn--;
    प्रति( पूर्णाङ्क i = 0; i < len; i++ )
    {
        अस्थायी_2 = RNG_अग्र(अस्थायी_2);
        अचिह्नित t = (अचिह्नित)अस्थायी_2;
        अचिह्नित v = (अचिह्नित)(((uint64)t * p[k].M) >> 32);
        v = (v + ((t - v) >> p[k].sh1)) >> p[k].sh2;
        int64_t res = (int64_t)(t - v*p[k].d) + p[k].delta;
        k = CN_अग्र(k);
        arr[i] = saturate_cast<T>(res);
    }
    *state = अस्थायी_2;
}

स्थिर शून्य
randi_( int64_t* arr, पूर्णाङ्क len, पूर्णाङ्क cn, uint64* state, ध्रुव DivStruct* p )
{
    uint64 अस्थायी_2 = *state;
    पूर्णाङ्क k = 0;
    len *= cn;
    cn--;
    प्रति( पूर्णाङ्क i = 0; i < len; i++ )
    {
        अस्थायी_2 = RNG_अग्र(अस्थायी_2);
        अचिह्नित t0 = (अचिह्नित)अस्थायी_2;
        अस्थायी_2 = RNG_अग्र(अस्थायी_2);
        अचिह्नित t1 = (अचिह्नित)अस्थायी_2;
        int64_t t = (int64_t)((((uint64_t)t0 << 32) | t1) % p[k].diff) + p[k].delta;
        k = CN_अग्र(k);
        arr[i] = t;
    }
    *state = अस्थायी_2;
}

स्थिर शून्य
randi_( uint64_t* arr, पूर्णाङ्क len, पूर्णाङ्क cn, uint64* state, ध्रुव DivStruct* p )
{
    uint64 अस्थायी_2 = *state;
    पूर्णाङ्क k = 0;
    len *= cn;
    cn--;
    प्रति( पूर्णाङ्क i = 0; i < len; i++ )
    {
        अस्थायी_2 = RNG_अग्र(अस्थायी_2);
        अचिह्नित t0 = (अचिह्नित)अस्थायी_2;
        अस्थायी_2 = RNG_अग्र(अस्थायी_2);
        अचिह्नित t1 = (अचिह्नित)अस्थायी_2;
        uint64_t t = (((uint64_t)t0 << 32) | t1) % p[k].diff;
        int64_t delta = p[k].delta;
        k = CN_अग्र(k);
        arr[i] = delta >= 0 || t >= (uint64_t)-delta ? t + (uint64_t)delta : 0;
    }
    *state = अस्थायी_2;
}

प्रतिरूपम्<प्रकारनाम T>
स्थिर शून्य rand_द्वयङ्काः_Wrap(शून्य* arr, पूर्णाङ्क len, पूर्णाङ्क cn, uint64* state, ध्रुव शून्य* p, शून्य*, पूर्णाङ्क flags)
{
    rand_द्वयङ्काः((T*)arr, len, cn, state, (ध्रुव Vec2l*)p, flags);
}

प्रतिरूपम्<प्रकारनाम T>
स्थिर शून्य randiWrap(शून्य* arr, पूर्णाङ्क len, पूर्णाङ्क cn, uint64* state, ध्रुव शून्य* p, शून्य*, पूर्णाङ्क)
{
    randi_((T*)arr, len, cn, state, (ध्रुव DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
स्थिर अन्तःस्थ शून्य cvt32f_गन्तव्यम्_FP8(ध्रुव भिन्न* src, शून्य* dst, पूर्णाङ्क len, पूर्णाङ्क गभीरता)
{
    यदि (गभीरता == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   प्रति (पूर्णाङ्क i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    अन्यथा                              { fp8a_t* d = (fp8a_t*)dst; प्रति (पूर्णाङ्क i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
स्थिर अन्तःस्थ तार्किक is_FP8_गभीरता(पूर्णाङ्क d) { प्रत्यागम d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

स्थिर शून्य randf_16_or_32f( शून्य* dst, पूर्णाङ्क len_, पूर्णाङ्क cn, uint64* state, ध्रुव शून्य* _p, शून्य* _fbuf, पूर्णाङ्क flags )
{
    ध्रुव Vec2f* p = (ध्रुव Vec2f*)_p;
    भिन्न* fbuf = (भिन्न*)_fbuf;
    पूर्णाङ्क गभीरता = CV_MAT_गभीरता(flags);
    uint64 अस्थायी_2 = *state;
    पूर्णाङ्क k = 0, len = len_*cn;
    भिन्न* arr = गभीरता == CV_16F || गभीरता == CV_16BF || is_FP8_गभीरता(गभीरता) ? fbuf : (भिन्न*)dst;
    cn--;
    प्रति( पूर्णाङ्क i = 0; i < len; i++ )
    {
        पूर्णाङ्क t = (पूर्णाङ्क)(अस्थायी_2 = RNG_अग्र(अस्थायी_2));
        arr[i] = (भिन्न)(t*p[k][0]);
        k = CN_अग्र(k);
    }
    *state = अस्थायी_2;
    hal::सङ्कलय_RNGBias32f(arr, &p[0][0], len_, cn+1);
    यदि (गभीरता == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    अन्यथा यदि (गभीरता == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    अन्यथा यदि (is_FP8_गभीरता(गभीरता))
        cvt32f_गन्तव्यम्_FP8(fbuf, dst, len, गभीरता);
}

स्थिर शून्य
randf_64f( शून्य* _arr, पूर्णाङ्क len_, पूर्णाङ्क cn, uint64* state, ध्रुव शून्य* _p, शून्य*, पूर्णाङ्क )
{
    द्विगुण* arr = (द्विगुण*)_arr;
    ध्रुव Vec2d* p = (ध्रुव Vec2d*)_p;
    uint64 अस्थायी_2 = *state;
    पूर्णाङ्क k = 0, len = len_*cn;
    cn--;
    प्रति( पूर्णाङ्क i = 0; i < len; i++ )
    {
        अस्थायी_2 = RNG_अग्र(अस्थायी_2);
        int64_t v = (int64_t)((अस्थायी_2 >> 32) | (अस्थायी_2 << 32));
        arr[i] = v*p[k][0];
        k = CN_अग्र(k);
    }
    *state = अस्थायी_2;
    hal::सङ्कलय_RNGBias64f(arr, &p[0][0], len_, cn+1);
}

प्रकारनिर्देश शून्य (*RandFunc)(शून्य* arr, पूर्णाङ्क len, पूर्णाङ्क cn, uint64* state,
                         ध्रुव शून्य* p, शून्य* tempbuf, पूर्णाङ्क flags);

स्थिर RandFunc randTab[CV_गभीरता_MAX][CV_गभीरता_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<ह्रस्व>, randiWrap<पूर्णाङ्क>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<तार्किक>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<अचिह्नित>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        rand_द्वयङ्काः_Wrap<uchar>, rand_द्वयङ्काः_Wrap<schar>, rand_द्वयङ्काः_Wrap<ushort>,
        rand_द्वयङ्काः_Wrap<ह्रस्व>, rand_द्वयङ्काः_Wrap<पूर्णाङ्क>, 0, 0, 0, 0,
        rand_द्वयङ्काः_Wrap<तार्किक>, rand_द्वयङ्काः_Wrap<uint64_t>, rand_द्वयङ्काः_Wrap<int64_t>,
        rand_द्वयङ्काः_Wrap<अचिह्नित>, 0, 0, 0
    }
};

/*
   The code below implements the algorithm described in
   "The Ziggurat Method for Generating Random Variables"
   by George Marsaglia and Wai Wan Tsang, Journal of Statistical Software, 2007.
*/
स्थिर शून्य
randn_0_1_32f( भिन्न* arr, पूर्णाङ्क len, uint64* state )
{
    ध्रुव भिन्न r = 3.442620f; // The start of the right tail
    ध्रुव भिन्न rng_flt = 2.3283064365386962890625e-10f; // 2^-32
    स्थिर अचिह्नित kn[128];
    स्थिर भिन्न wn[128], fn[128];
    uint64 अस्थायी_2 = *state;
    स्थिर तार्किक initialized=असत्य;
    पूर्णाङ्क i;

    यदि( !initialized )
    {
        ध्रुव द्विगुण m1 = 2147483648.0;
        द्विगुण dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        द्विगुण q = vn/std::घातांकः(-.5*dn*dn);
        kn[0] = (अचिह्नित)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (भिन्न)(q/m1);
        wn[127] = (भिन्न)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (भिन्न)std::घातांकः(-.5*dn*dn);

        प्रति(i=126;i>=1;i--)
        {
            dn = std::वर्गमूलम्(-2.*std::लघुगणकः(vn/dn+std::घातांकः(-.5*dn*dn)));
            kn[i+1] = (अचिह्नित)((dn/tn)*m1);
            tn = dn;
            fn[i] = (भिन्न)std::घातांकः(-.5*dn*dn);
            wn[i] = (भिन्न)(dn/m1);
        }
        initialized = सत्य;
    }

    प्रति( i = 0; i < len; i++ )
    {
        भिन्न x, y;
        प्रति(;;)
        {
            पूर्णाङ्क hz = (पूर्णाङ्क)अस्थायी_2;
            अस्थायी_2 = RNG_अग्र(अस्थायी_2);
            पूर्णाङ्क iz = hz & 127;
            x = hz*wn[iz];
            यदि( (अचिह्नित)std::निरपेक्षम्(hz) < kn[iz] )
                भङ्ग;
            यदि( iz == 0) // iz==0, handles the base strip
            {
                कुरु
                {
                    x = (अचिह्नित)अस्थायी_2*rng_flt;
                    अस्थायी_2 = RNG_अग्र(अस्थायी_2);
                    y = (अचिह्नित)अस्थायी_2*rng_flt;
                    अस्थायी_2 = RNG_अग्र(अस्थायी_2);
                    x = (भिन्न)(-std::लघुगणकः(x+FLT_MIN)*0.2904764);
                    y = (भिन्न)-std::लघुगणकः(y+FLT_MIN);
                }	// .2904764 is 1/r
                यावत्( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                भङ्ग;
            }
            // iz > 0, handle the wedges of other strips
            y = (अचिह्नित)अस्थायी_2*rng_flt;
            अस्थायी_2 = RNG_अग्र(अस्थायी_2);
            यदि( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::घातांकः(-.5*x*x) )
                भङ्ग;
        }
        arr[i] = x;
    }
    *state = अस्थायी_2;
}


द्विगुण RNG::gaussian(द्विगुण sigma)
{
    भिन्न अस्थायी_2;
    randn_0_1_32f( &अस्थायी_2, 1, &state );
    प्रत्यागम अस्थायी_2*sigma;
}

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम PT> स्थिर शून्य
randnScale_(भिन्न* src, शून्य* _dst, पूर्णाङ्क len, पूर्णाङ्क cn,
            ध्रुव शून्य* माध्यम्, ध्रुव शून्य* _stddev, पूर्णाङ्क flags )
{
    T* dst = (T*)_dst;
    ध्रुव PT* माध्यम्_2 = (ध्रुव PT*)माध्यम्;
    ध्रुव PT* stddev = (ध्रुव PT*)_stddev;
    तार्किक stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    पूर्णाङ्क i, j, k;
    यदि( !stdmtx || cn == 1 )
    {
        यदि( cn == 1 )
        {
            PT a = stddev[0], b = माध्यम्_2[0];
            प्रति( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        अन्यथा
        {
            len *= cn;
            cn--;
            प्रति( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + माध्यम्_2[k]);
                k = CN_अग्र(k);
            }
        }
    }
    अन्यथा
    {
        len *= cn;
        cn--;
        प्रति( i = j = 0; i < len; i++ )
        {
            PT s = माध्यम्_2[j];
            पूर्णाङ्क i0 = i - j;
            प्रति( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_अग्र(j);
        }
    }
}

// special version for 16f, 16bf and 32f
स्थिर शून्य
randnScale_16_or_32f(भिन्न* fbuf, शून्य* _dst, पूर्णाङ्क len, पूर्णाङ्क cn,
                     ध्रुव शून्य* माध्यम्, ध्रुव शून्य* _stddev, पूर्णाङ्क flags)
{
    भिन्न* dst = (भिन्न*)_dst;
    ध्रुव भिन्न* माध्यम्_2 = (ध्रुव भिन्न*)माध्यम्;
    ध्रुव भिन्न* stddev = (ध्रुव भिन्न*)_stddev;
    तार्किक stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    पूर्णाङ्क गभीरता = CV_MAT_गभीरता(flags);
    भिन्न* arr = गभीरता == CV_16F || गभीरता == CV_16BF || is_FP8_गभीरता(गभीरता) ? fbuf : dst;
    पूर्णाङ्क i, j, k;

    यदि( !stdmtx || cn == 1 )
    {
        यदि( cn == 1 )
        {
            भिन्न a = stddev[0], b = माध्यम्_2[0];
            प्रति( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        अन्यथा
        {
            len *= cn;
            cn--;
            प्रति( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + माध्यम्_2[k];
                k = CN_अग्र(k);
            }
        }
    }
    अन्यथा यदि( गभीरता == CV_32F )
    {
        len *= cn;
        cn--;
        प्रति( i = j = 0; i < len; i++ )
        {
            भिन्न s = माध्यम्_2[j];
            पूर्णाङ्क i0 = i - j;
            प्रति( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_अग्र(j);
        }
    }
    अन्यथा
    {
        भिन्न elembuf[CV_CN_MAX];
        len *= cn;
        प्रति( i = 0; i < len; i += cn )
        {
            // since we process fbuf in-place,
            // we need to copy each cn-channel element
            // prior to matrix multiplication
            प्रति (j = 0; j < cn; j++)
                elembuf[j] = fbuf[i + j];
            प्रति (j = 0; j < cn; j++) {
                भिन्न s = माध्यम्_2[j];
                प्रति( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    यदि (गभीरता == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    अन्यथा यदि (गभीरता == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    अन्यथा यदि (is_FP8_गभीरता(गभीरता))
        cvt32f_गन्तव्यम्_FP8(fbuf, dst, len, गभीरता);
}

प्रकारनिर्देश शून्य (*RandnScaleFunc)(भिन्न* src, शून्य* dst, पूर्णाङ्क len, पूर्णाङ्क cn,
                               ध्रुव शून्य* माध्यम्_2, ध्रुव शून्य* stddev, पूर्णाङ्क flags);

स्थिर RandnScaleFunc randnScaleTab[CV_गभीरता_MAX] =
{
    randnScale_<uchar, भिन्न>, randnScale_<schar, भिन्न>, randnScale_<ushort, भिन्न>,
    randnScale_<ह्रस्व, भिन्न>, randnScale_<पूर्णाङ्क, भिन्न>, randnScale_16_or_32f,
    randnScale_<द्विगुण, द्विगुण>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<तार्किक, भिन्न>, randnScale_<uint64_t, द्विगुण>, randnScale_<int64_t, द्विगुण>,
    randnScale_<अचिह्नित, भिन्न>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

शून्य RNG::fill( निवेशः_Output_Array _mat, पूर्णाङ्क disttype,
                निवेशः_Array _param1arg, निवेशः_Array _param2arg,
                तार्किक saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    पूर्णाङ्क j, गभीरता = mat.गभीरता(), cn = mat.channels();
    पूर्णाङ्क esz1 = CV_ELEM_आकार(गभीरता);
    Auto_बफर<द्विगुण> _parambuf;
    तार्किक fast_int_बहुलकम् = असत्य;
    तार्किक small_flag = असत्य;
    RandFunc func = 0;
    RandnScaleFunc scaleFunc = 0;

    CV_Assert(_param1.channels() == 1 && (_param1.पङ्क्तिकाः == 1 || _param1.स्तम्भसङ्ख्या == 1) &&
              (_param1.पङ्क्तिकाः + _param1.स्तम्भसङ्ख्या - 1 == cn || _param1.पङ्क्तिकाः + _param1.स्तम्भसङ्ख्या - 1 == 1 ||
               (_param1.size() == आकार(1, 4) && _param1.type() == CV_64F && cn <= 4)));
    CV_Assert( _param2.channels() == 1 &&
               (((_param2.पङ्क्तिकाः == 1 || _param2.स्तम्भसङ्ख्या == 1) &&
                (_param2.पङ्क्तिकाः + _param2.स्तम्भसङ्ख्या - 1 == cn || _param2.पङ्क्तिकाः + _param2.स्तम्भसङ्ख्या - 1 == 1 ||
                (_param1.size() == आकार(1, 4) && _param1.type() == CV_64F && cn <= 4))) ||
                (_param2.पङ्क्तिकाः == cn && _param2.स्तम्भसङ्ख्या == cn && disttype == प्रसामान्यम्_3)));

    ध्रुव शून्य* uni_param = 0;
    uchar* माध्यम्_2 = 0;
    uchar* stddev = 0;
    तार्किक stdmtx = असत्य;
    पूर्णाङ्क n1 = (पूर्णाङ्क)_param1.योग();
    पूर्णाङ्क n2 = (पूर्णाङ्क)_param2.योग();

    यदि( disttype == एकरूपम्_2 )
    {
        _parambuf.allocate(cn*(परिमाण(DivStruct)+परिमाण(द्विगुण)-1)/परिमाण(द्विगुण) + cn*4);
        द्विगुण* parambuf = _parambuf.दत्त();
        द्विगुण* p1 = _param1.ptr<द्विगुण>();
        द्विगुण* p2 = _param2.ptr<द्विगुण>();

        यदि( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convert_गन्तव्यम्(tmp, CV_64F);
            प्रति( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        यदि( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convert_गन्तव्यम्(tmp, CV_64F);
            प्रति( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        यदि( CV_IS_INT_TYPE(गभीरता) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            प्रति( j = 0, fast_int_बहुलकम् = सत्य; j < cn; j++ )
            {
                द्विगुण a = std::min(p1[j], p2[j]);
                द्विगुण b = std::max(p1[j], p2[j]);
                यदि( saturateRange )
                {
                    a = std::max(a, गभीरता == CV_8U || गभीरता == CV_16U || गभीरता == CV_32U ||
                                 गभीरता == CV_64U || गभीरता == CV_Bool ? 0. :
                                 गभीरता == CV_8S ? -128. : गभीरता == CV_16S ? -32768. :
                                 गभीरता == CV_32S ? (द्विगुण)INT_MIN : (द्विगुण)INT64_MIN);
                    b = std::min(b, गभीरता == CV_8U ? 256. : गभीरता == CV_Bool ? 2. : गभीरता == CV_16U ? 65536. :
                                 गभीरता == CV_8S ? 128. : गभीरता == CV_16S ? 32768. : गभीरता == CV_32U ? (द्विगुण)UINT_MAX :
                                 गभीरता == CV_32S ? (द्विगुण)अधिकतम_पूर्णाङ्कः : (द्विगुण)INT64_MAX);
                }
                ip[j][1] = (int64_t)ceil(a);
                int64_t idiff = ip[j][0] = (int64_t)floor(b) - ip[j][1] - 1;
                यदि (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                द्विगुण diff = b - a;

                fast_int_बहुलकम् = fast_int_बहुलकम् && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                यदि( fast_int_बहुलकम् )
                    small_flag = idiff <= 255;
                अन्यथा
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    यदि (गभीरता == CV_64S || गभीरता == CV_64U)
                    {
                        minval = INT64_MIN/2;
                        maxval = INT64_MAX;
                    }
                    यदि( diff > (द्विगुण)maxval )
                        ip[j][0] = maxval;
                    यदि( a < (द्विगुण)minval )
                        ip[j][1] = minval;
                }
            }

            uni_param = ip;
            यदि( !fast_int_बहुलकम् )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((शून्य*)(ds + cn), (शून्य*)(parambuf + _parambuf.size()), "Last byte check");
                प्रति( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    यदि (गभीरता != CV_64U && गभीरता != CV_64S) {
                        अचिह्नित d = ds[j].d = (अचिह्नित)(ip[j][0]+1);
                        पूर्णाङ्क l = 0;
                        यावत्(((uint64)1 << l) < d)
                            l++;
                        ds[j].M = (अचिह्नित)(((uint64)1 << 32)*(((uint64)1 << l) - d)/d) + 1;
                        ds[j].sh1 = std::min(l, 1);
                        ds[j].sh2 = std::max(l - 1, 0);
                    }
                }
                uni_param = ds;
            }

            func = randTab[fast_int_बहुलकम् ? 1 : 0][गभीरता];
        }
        अन्यथा
        {
            द्विगुण scale = गभीरता == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            द्विगुण maxdiff = saturateRange ? (द्विगुण)FLT_MAX : DBL_MAX;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            यदि( गभीरता != CV_64F )
            {
                Vec2f* fp = (Vec2f*)(parambuf + cn*2);
                प्रति( j = 0; j < cn; j++ )
                {
                    fp[j][0] = (भिन्न)(std::min(maxdiff, p2[j] - p1[j])*scale);
                    fp[j][1] = (भिन्न)((p2[j] + p1[j])*0.5);
                }
                uni_param = fp;
            }
            अन्यथा
            {
                Vec2d* dp = (Vec2d*)(parambuf + cn*2);
                प्रति( j = 0; j < cn; j++ )
                {
                    dp[j][0] = std::min(DBL_MAX, p2[j] - p1[j])*scale;
                    dp[j][1] = ((p2[j] + p1[j])*0.5);
                }
                uni_param = dp;
            }

            func = randTab[0][गभीरता];
        }
        CV_Assert( func != 0 );
    }
    अन्यथा यदि( disttype == RNG::प्रसामान्यम्_3 )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        द्विगुण* parambuf = _parambuf.दत्त();

        पूर्णाङ्क ptype = esz1 == 8 ? CV_64F : CV_32F;

        यदि( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            माध्यम्_2 = _param1.ptr();
        अन्यथा
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convert_गन्तव्यम्(tmp, ptype);
            माध्यम्_2 = (uchar*)parambuf;
        }

        यदि( n1 < cn )
            प्रति( j = n1*esz1; j < cn*esz1; j++ )
                माध्यम्_2[j] = माध्यम्_2[j - n1*esz1];

        यदि( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        अन्यथा
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convert_गन्तव्यम्(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        यदि( n2 < cn )
            प्रति( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.पङ्क्तिकाः == cn && _param2.स्तम्भसङ्ख्या == cn;
        scaleFunc = randnScaleTab[गभीरता];
        CV_Assert( scaleFunc != 0 );
    }
    अन्यथा
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    ध्रुव Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    भिन्न fbuf[खण्डम्_आकार + CV_CN_MAX];
    पूर्णाङ्क योग = (पूर्णाङ्क)it.size;
    पूर्णाङ्क खण्डम्_आकार_2 = std::min((खण्डम्_आकार + cn - 1)/cn, योग);
    size_t esz = (size_t)esz1*cn;
    पूर्णाङ्क flags = mat.type();

    यदि( disttype == एकरूपम्_2 )
        flags |= (small_flag ? (पूर्णाङ्क)RNG_FLAG_SMALL : 0);
    अन्यथा
        flags |= (stdmtx ? (पूर्णाङ्क)RNG_FLAG_STDMTX : 0);

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        प्रति( j = 0; j < योग; j += खण्डम्_आकार_2 )
        {
            पूर्णाङ्क len = std::min(योग - j, खण्डम्_आकार_2);

            यदि( disttype == एकरूपम्_2 )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            अन्यथा
            {
                randn_0_1_32f(fbuf, len*cn, &state);
                scaleFunc(fbuf, ptr + j*esz, len, cn, माध्यम्_2, stddev, flags);
            }
        }
    }
}

}

cv::RNG& cv::theRNG()
{
    प्रत्यागम get_Core_Tls_दत्त().rng;
}

शून्य cv::समुच्चयः_RNGSeed(पूर्णाङ्क बीजम्)
{
    theRNG() = RNG(static_cast<uint64>(बीजम्));
}


शून्य cv::randu(निवेशः_Output_Array dst, निवेशः_Array नीचः, निवेशः_Array उच्चः)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::एकरूपम्_2, नीचः, उच्चः);
}

शून्य cv::randn(निवेशः_Output_Array dst, निवेशः_Array माध्यम्_2, निवेशः_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::प्रसामान्यम्_3, माध्यम्_2, stddev);
}

नामाकाश cv
{

प्रतिरूपम्<प्रकारनाम T> स्थिर शून्य
randShuffle_( Mat& _arr, RNG& rng, द्विगुण )
{
    अचिह्नित sz = (अचिह्नित)_arr.योग();
    यदि( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        प्रति( अचिह्नित i = 0; i < sz; i++ )
        {
            अचिह्नित j = (अचिह्नित)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    अन्यथा
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* दत्त = _arr.ptr();
        size_t चरणः = _arr.चरणः;
        पूर्णाङ्क पङ्क्तिकाः = _arr.पङ्क्तिकाः;
        पूर्णाङ्क स्तम्भसङ्ख्या = _arr.स्तम्भसङ्ख्या;
        प्रति( पूर्णाङ्क i0 = 0; i0 < पङ्क्तिकाः; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            प्रति( पूर्णाङ्क j0 = 0; j0 < स्तम्भसङ्ख्या; j0++ )
            {
                अचिह्नित k1 = (अचिह्नित)rng % sz;
                पूर्णाङ्क i1 = (पूर्णाङ्क)(k1 / स्तम्भसङ्ख्या);
                पूर्णाङ्क j1 = (पूर्णाङ्क)(k1 - (अचिह्नित)i1*(अचिह्नित)स्तम्भसङ्ख्या);
                std::swap( p[j0], ((T*)(दत्त + चरणः*i1))[j1] );
            }
        }
    }
}

प्रकारनिर्देश शून्य (*RandShuffleFunc)( Mat& dst, RNG& rng, द्विगुण iter_गुणकः );

}

शून्य cv::randShuffle( निवेशः_Output_Array _dst, द्विगुण iter_गुणकः, RNG* _rng )
{
    CV_INSTRUMENT_REGION();

    RandShuffleFunc tab[] =
    {
        0,
        randShuffle_<uchar>, // 1
        randShuffle_<ushort>, // 2
        randShuffle_<Vec<uchar,3> >, // 3
        randShuffle_<पूर्णाङ्क>, // 4
        0,
        randShuffle_<Vec<ushort,3> >, // 6
        0,
        randShuffle_<Vec<पूर्णाङ्क,2> >, // 8
        0, 0, 0,
        randShuffle_<Vec<पूर्णाङ्क,3> >, // 12
        0, 0, 0,
        randShuffle_<Vec<पूर्णाङ्क,4> >, // 16
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<पूर्णाङ्क,6> >, // 24
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<पूर्णाङ्क,8> > // 32
    };

    Mat dst = _dst.getMat();
    RNG& rng = _rng ? *_rng : theRNG();
    CV_Assert( dst.elem_आकार() <= 32 );
    RandShuffleFunc func = tab[dst.elem_आकार()];
    CV_Assert( func != 0 );
    func( dst, rng, iter_गुणकः );
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

cv::RNG_MT19937::RNG_MT19937(अचिह्नित s) { बीजम्(s); }

cv::RNG_MT19937::RNG_MT19937() { बीजम्(5489U); }

शून्य cv::RNG_MT19937::बीजम्(अचिह्नित s)
{
    state[0]= s;
    प्रति (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

अचिह्नित cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    स्थिर अचिह्नित mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    ध्रुव अचिह्नित ऊर्ध्वस्थम्_आवरणम् = 0x80000000U;
    ध्रुव अचिह्नित अधस्तनम्_आवरणम् = 0x7fffffffU;

    /* generate N words at one time */
    यदि (mti >= N)
    {
        पूर्णाङ्क kk = 0;

        प्रति (; kk < N - M; ++kk)
        {
            अचिह्नित y = (state[kk] & ऊर्ध्वस्थम्_आवरणम्) | (state[kk + 1] & अधस्तनम्_आवरणम्);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        प्रति (; kk < N - 1; ++kk)
        {
            अचिह्नित y = (state[kk] & ऊर्ध्वस्थम्_आवरणम्) | (state[kk + 1] & अधस्तनम्_आवरणम्);
            state[kk] = state[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        अचिह्नित y = (state[N - 1] & ऊर्ध्वस्थम्_आवरणम्) | (state[0] & अधस्तनम्_आवरणम्);
        state[N - 1] = state[M - 1] ^ (y >> 1) ^ mag01[y & 0x1U];

        mti = 0;
    }

    अचिह्नित y = state[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y <<  7) & 0x9d2c5680U;
    y ^= (y << 15) & 0xefc60000U;
    y ^= (y >> 18);

    प्रत्यागम y;
}

cv::RNG_MT19937::संक्रियकः अचिह्नित() { प्रत्यागम next(); }

cv::RNG_MT19937::संक्रियकः पूर्णाङ्क() { प्रत्यागम (पूर्णाङ्क)next();}

cv::RNG_MT19937::संक्रियकः भिन्न() { प्रत्यागम next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::संक्रियकः द्विगुण()
{
    अचिह्नित a = next() >> 5;
    अचिह्नित b = next() >> 6;
    प्रत्यागम (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

पूर्णाङ्क cv::RNG_MT19937::एकरूपम्(पूर्णाङ्क a, पूर्णाङ्क b) { प्रत्यागम (पूर्णाङ्क)(next() % (b - a) + a); }

भिन्न cv::RNG_MT19937::एकरूपम्(भिन्न a, भिन्न b) { प्रत्यागम ((भिन्न)*एतत्)*(b - a) + a; }

द्विगुण cv::RNG_MT19937::एकरूपम्(द्विगुण a, द्विगुण b) { प्रत्यागम ((द्विगुण)*एतत्)*(b - a) + a; }

अचिह्नित cv::RNG_MT19937::संक्रियकः ()(अचिह्नित b) { प्रत्यागम next() % b; }

अचिह्नित cv::RNG_MT19937::संक्रियकः ()() { प्रत्यागम next(); }

/* End of file. */
