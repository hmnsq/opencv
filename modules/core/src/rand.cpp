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

#समावेश "precomp.hpp"

namespace cv
{

///////////////////////////// Functions Declaration //////////////////////////////////////

#घोषणा CV_RNG_COEFF 4164903690U

/*
   Multiply-with-carry generator is used here:
   temp = ( A*X(n) + carry )
   X(n+1) = temp mod (2^32)
   carry = floor (temp / (2^32))
*/

#घोषणा  RNG_अग्र(x)    ((uint64)(अचिन्हित)(x)*CV_RNG_COEFF + ((x) >> 32))
// make it jump-less
#घोषणा  CN_अग्र(k)     (((k) + 1) & (((k) >= cn) - 1))

क्रमागत
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

ढाँचा<typename T> अटल व्योम
randBits_( T* arr, पूर्णांक len, पूर्णांक cn, uint64* state, स्थिर Vec2l* p, पूर्णांक flags )
{
    bool small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 अस्थायी_2 = *state;
    पूर्णांक i, k = 0;
    len *= cn;
    --cn;

    अगर( !small_flag )
    {
        क्रम( i = 0; i <= len - 4; i += 4 )
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
        क्रम( i = 0; i <= len - 4; i += 4 )
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

    क्रम( ; i < len; i++ )
    {
        int64_t t0;
        अस्थायी_2 = RNG_अग्र(अस्थायी_2);

        t0 = ((int64_t)अस्थायी_2 & p[k][0]) + p[k][1];
        k = CN_अग्र(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = अस्थायी_2;
}

काष्ठा DivStruct
{
    अचिन्हित d;
    अचिन्हित M;
    पूर्णांक sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

ढाँचा<typename T> अटल व्योम
randi_( T* arr, पूर्णांक len, पूर्णांक cn, uint64* state, स्थिर DivStruct* p )
{
    uint64 अस्थायी_2 = *state;
    पूर्णांक k = 0;
    len *= cn;
    cn--;
    क्रम( पूर्णांक i = 0; i < len; i++ )
    {
        अस्थायी_2 = RNG_अग्र(अस्थायी_2);
        अचिन्हित t = (अचिन्हित)अस्थायी_2;
        अचिन्हित v = (अचिन्हित)(((uint64)t * p[k].M) >> 32);
        v = (v + ((t - v) >> p[k].sh1)) >> p[k].sh2;
        int64_t res = (int64_t)(t - v*p[k].d) + p[k].delta;
        k = CN_अग्र(k);
        arr[i] = saturate_cast<T>(res);
    }
    *state = अस्थायी_2;
}

अटल व्योम
randi_( int64_t* arr, पूर्णांक len, पूर्णांक cn, uint64* state, स्थिर DivStruct* p )
{
    uint64 अस्थायी_2 = *state;
    पूर्णांक k = 0;
    len *= cn;
    cn--;
    क्रम( पूर्णांक i = 0; i < len; i++ )
    {
        अस्थायी_2 = RNG_अग्र(अस्थायी_2);
        अचिन्हित t0 = (अचिन्हित)अस्थायी_2;
        अस्थायी_2 = RNG_अग्र(अस्थायी_2);
        अचिन्हित t1 = (अचिन्हित)अस्थायी_2;
        int64_t t = (int64_t)((((uint64_t)t0 << 32) | t1) % p[k].diff) + p[k].delta;
        k = CN_अग्र(k);
        arr[i] = t;
    }
    *state = अस्थायी_2;
}

अटल व्योम
randi_( uint64_t* arr, पूर्णांक len, पूर्णांक cn, uint64* state, स्थिर DivStruct* p )
{
    uint64 अस्थायी_2 = *state;
    पूर्णांक k = 0;
    len *= cn;
    cn--;
    क्रम( पूर्णांक i = 0; i < len; i++ )
    {
        अस्थायी_2 = RNG_अग्र(अस्थायी_2);
        अचिन्हित t0 = (अचिन्हित)अस्थायी_2;
        अस्थायी_2 = RNG_अग्र(अस्थायी_2);
        अचिन्हित t1 = (अचिन्हित)अस्थायी_2;
        uint64_t t = (((uint64_t)t0 << 32) | t1) % p[k].diff;
        int64_t delta = p[k].delta;
        k = CN_अग्र(k);
        arr[i] = delta >= 0 || t >= (uint64_t)-delta ? t + (uint64_t)delta : 0;
    }
    *state = अस्थायी_2;
}

ढाँचा<typename T>
अटल व्योम randBitsWrap(व्योम* arr, पूर्णांक len, पूर्णांक cn, uint64* state, स्थिर व्योम* p, व्योम*, पूर्णांक flags)
{
    randBits_((T*)arr, len, cn, state, (स्थिर Vec2l*)p, flags);
}

ढाँचा<typename T>
अटल व्योम randiWrap(व्योम* arr, पूर्णांक len, पूर्णांक cn, uint64* state, स्थिर व्योम* p, व्योम*, पूर्णांक)
{
    randi_((T*)arr, len, cn, state, (स्थिर DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
अटल अंतरभूत व्योम cvt32fToFP8(स्थिर भग्न* src, व्योम* dst, पूर्णांक len, पूर्णांक depth)
{
    अगर (depth == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   क्रम (पूर्णांक i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    अन्यथा                              { fp8a_t* d = (fp8a_t*)dst; क्रम (पूर्णांक i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
अटल अंतरभूत bool isFP8Depth(पूर्णांक d) { वापस d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

अटल व्योम randf_16_or_32f( व्योम* dst, पूर्णांक len_, पूर्णांक cn, uint64* state, स्थिर व्योम* _p, व्योम* _fbuf, पूर्णांक flags )
{
    स्थिर Vec2f* p = (स्थिर Vec2f*)_p;
    भग्न* fbuf = (भग्न*)_fbuf;
    पूर्णांक depth = CV_MAT_DEPTH(flags);
    uint64 अस्थायी_2 = *state;
    पूर्णांक k = 0, len = len_*cn;
    भग्न* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : (भग्न*)dst;
    cn--;
    क्रम( पूर्णांक i = 0; i < len; i++ )
    {
        पूर्णांक t = (पूर्णांक)(अस्थायी_2 = RNG_अग्र(अस्थायी_2));
        arr[i] = (भग्न)(t*p[k][0]);
        k = CN_अग्र(k);
    }
    *state = अस्थायी_2;
    hal::addRNGBias32f(arr, &p[0][0], len_, cn+1);
    अगर (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    अन्यथा अगर (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    अन्यथा अगर (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

अटल व्योम
randf_64f( व्योम* _arr, पूर्णांक len_, पूर्णांक cn, uint64* state, स्थिर व्योम* _p, व्योम*, पूर्णांक )
{
    द्विगुन* arr = (द्विगुन*)_arr;
    स्थिर Vec2d* p = (स्थिर Vec2d*)_p;
    uint64 अस्थायी_2 = *state;
    पूर्णांक k = 0, len = len_*cn;
    cn--;
    क्रम( पूर्णांक i = 0; i < len; i++ )
    {
        अस्थायी_2 = RNG_अग्र(अस्थायी_2);
        int64_t v = (int64_t)((अस्थायी_2 >> 32) | (अस्थायी_2 << 32));
        arr[i] = v*p[k][0];
        k = CN_अग्र(k);
    }
    *state = अस्थायी_2;
    hal::addRNGBias64f(arr, &p[0][0], len_, cn+1);
}

प्रकार व्योम (*RandFunc)(व्योम* arr, पूर्णांक len, पूर्णांक cn, uint64* state,
                         स्थिर व्योम* p, व्योम* tempbuf, पूर्णांक flags);

अटल RandFunc randTab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<लघु>, randiWrap<पूर्णांक>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<bool>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<अचिन्हित>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        randBitsWrap<uchar>, randBitsWrap<schar>, randBitsWrap<ushort>,
        randBitsWrap<लघु>, randBitsWrap<पूर्णांक>, 0, 0, 0, 0,
        randBitsWrap<bool>, randBitsWrap<uint64_t>, randBitsWrap<int64_t>,
        randBitsWrap<अचिन्हित>, 0, 0, 0
    }
};

/*
   The code below implements the algorithm described in
   "The Ziggurat Method for Generating Random Variables"
   by George Marsaglia and Wai Wan Tsang, Journal of Statistical Software, 2007.
*/
अटल व्योम
randn_0_1_32f( भग्न* arr, पूर्णांक len, uint64* state )
{
    स्थिर भग्न r = 3.442620f; // The start of the right tail
    स्थिर भग्न rng_flt = 2.3283064365386962890625e-10f; // 2^-32
    अटल अचिन्हित kn[128];
    अटल भग्न wn[128], fn[128];
    uint64 अस्थायी_2 = *state;
    अटल bool initialized=false;
    पूर्णांक i;

    अगर( !initialized )
    {
        स्थिर द्विगुन m1 = 2147483648.0;
        द्विगुन dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        द्विगुन q = vn/std::exp(-.5*dn*dn);
        kn[0] = (अचिन्हित)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (भग्न)(q/m1);
        wn[127] = (भग्न)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (भग्न)std::exp(-.5*dn*dn);

        क्रम(i=126;i>=1;i--)
        {
            dn = std::वर्ग_मूल(-2.*std::log(vn/dn+std::exp(-.5*dn*dn)));
            kn[i+1] = (अचिन्हित)((dn/tn)*m1);
            tn = dn;
            fn[i] = (भग्न)std::exp(-.5*dn*dn);
            wn[i] = (भग्न)(dn/m1);
        }
        initialized = true;
    }

    क्रम( i = 0; i < len; i++ )
    {
        भग्न x, y;
        क्रम(;;)
        {
            पूर्णांक hz = (पूर्णांक)अस्थायी_2;
            अस्थायी_2 = RNG_अग्र(अस्थायी_2);
            पूर्णांक iz = hz & 127;
            x = hz*wn[iz];
            अगर( (अचिन्हित)std::असल(hz) < kn[iz] )
                अवरोध;
            अगर( iz == 0) // iz==0, handles the base strip
            {
                करो
                {
                    x = (अचिन्हित)अस्थायी_2*rng_flt;
                    अस्थायी_2 = RNG_अग्र(अस्थायी_2);
                    y = (अचिन्हित)अस्थायी_2*rng_flt;
                    अस्थायी_2 = RNG_अग्र(अस्थायी_2);
                    x = (भग्न)(-std::log(x+भग्न_न्यून)*0.2904764);
                    y = (भग्न)-std::log(y+भग्न_न्यून);
                }	// .2904764 is 1/r
                जबतक( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                अवरोध;
            }
            // iz > 0, handle the wedges of other strips
            y = (अचिन्हित)अस्थायी_2*rng_flt;
            अस्थायी_2 = RNG_अग्र(अस्थायी_2);
            अगर( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::exp(-.5*x*x) )
                अवरोध;
        }
        arr[i] = x;
    }
    *state = अस्थायी_2;
}


द्विगुन RNG::gaussian(द्विगुन sigma)
{
    भग्न अस्थायी_2;
    randn_0_1_32f( &अस्थायी_2, 1, &state );
    वापस अस्थायी_2*sigma;
}

ढाँचा<typename T, typename PT> अटल व्योम
randnScale_(भग्न* src, व्योम* _dst, पूर्णांक len, पूर्णांक cn,
            स्थिर व्योम* _mean, स्थिर व्योम* _stddev, पूर्णांक flags )
{
    T* dst = (T*)_dst;
    स्थिर PT* mean = (स्थिर PT*)_mean;
    स्थिर PT* stddev = (स्थिर PT*)_stddev;
    bool stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    पूर्णांक i, j, k;
    अगर( !stdmtx || cn == 1 )
    {
        अगर( cn == 1 )
        {
            PT a = stddev[0], b = mean[0];
            क्रम( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        अन्यथा
        {
            len *= cn;
            cn--;
            क्रम( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + mean[k]);
                k = CN_अग्र(k);
            }
        }
    }
    अन्यथा
    {
        len *= cn;
        cn--;
        क्रम( i = j = 0; i < len; i++ )
        {
            PT s = mean[j];
            पूर्णांक i0 = i - j;
            क्रम( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_अग्र(j);
        }
    }
}

// special version for 16f, 16bf and 32f
अटल व्योम
randnScale_16_or_32f(भग्न* fbuf, व्योम* _dst, पूर्णांक len, पूर्णांक cn,
                     स्थिर व्योम* _mean, स्थिर व्योम* _stddev, पूर्णांक flags)
{
    भग्न* dst = (भग्न*)_dst;
    स्थिर भग्न* mean = (स्थिर भग्न*)_mean;
    स्थिर भग्न* stddev = (स्थिर भग्न*)_stddev;
    bool stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    पूर्णांक depth = CV_MAT_DEPTH(flags);
    भग्न* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : dst;
    पूर्णांक i, j, k;

    अगर( !stdmtx || cn == 1 )
    {
        अगर( cn == 1 )
        {
            भग्न a = stddev[0], b = mean[0];
            क्रम( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        अन्यथा
        {
            len *= cn;
            cn--;
            क्रम( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + mean[k];
                k = CN_अग्र(k);
            }
        }
    }
    अन्यथा अगर( depth == CV_32F )
    {
        len *= cn;
        cn--;
        क्रम( i = j = 0; i < len; i++ )
        {
            भग्न s = mean[j];
            पूर्णांक i0 = i - j;
            क्रम( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_अग्र(j);
        }
    }
    अन्यथा
    {
        भग्न elembuf[CV_CN_MAX];
        len *= cn;
        क्रम( i = 0; i < len; i += cn )
        {
            // since we process fbuf in-place,
            // we need to copy each cn-channel element
            // prior to matrix multiplication
            क्रम (j = 0; j < cn; j++)
                elembuf[j] = fbuf[i + j];
            क्रम (j = 0; j < cn; j++) {
                भग्न s = mean[j];
                क्रम( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    अगर (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    अन्यथा अगर (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    अन्यथा अगर (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

प्रकार व्योम (*RandnScaleFunc)(भग्न* src, व्योम* dst, पूर्णांक len, पूर्णांक cn,
                               स्थिर व्योम* mean, स्थिर व्योम* stddev, पूर्णांक flags);

अटल RandnScaleFunc randnScaleTab[CV_DEPTH_MAX] =
{
    randnScale_<uchar, भग्न>, randnScale_<schar, भग्न>, randnScale_<ushort, भग्न>,
    randnScale_<लघु, भग्न>, randnScale_<पूर्णांक, भग्न>, randnScale_16_or_32f,
    randnScale_<द्विगुन, द्विगुन>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<bool, भग्न>, randnScale_<uint64_t, द्विगुन>, randnScale_<int64_t, द्विगुन>,
    randnScale_<अचिन्हित, भग्न>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

व्योम RNG::fill( निवेश_Output_Array _mat, पूर्णांक disttype,
                निवेश_Array _param1arg, निवेश_Array _param2arg,
                bool saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    पूर्णांक j, depth = mat.depth(), cn = mat.channels();
    पूर्णांक esz1 = CV_ELEM_आकार(depth);
    Auto_बफर<द्विगुन> _parambuf;
    bool fast_int_mode = false;
    bool small_flag = false;
    RandFunc func = 0;
    RandnScaleFunc scaleFunc = 0;

    CV_Assert(_param1.channels() == 1 && (_param1.rows == 1 || _param1.cols == 1) &&
              (_param1.rows + _param1.cols - 1 == cn || _param1.rows + _param1.cols - 1 == 1 ||
               (_param1.size() == आकार(1, 4) && _param1.type() == CV_64F && cn <= 4)));
    CV_Assert( _param2.channels() == 1 &&
               (((_param2.rows == 1 || _param2.cols == 1) &&
                (_param2.rows + _param2.cols - 1 == cn || _param2.rows + _param2.cols - 1 == 1 ||
                (_param1.size() == आकार(1, 4) && _param1.type() == CV_64F && cn <= 4))) ||
                (_param2.rows == cn && _param2.cols == cn && disttype == NORMAL)));

    स्थिर व्योम* uni_param = 0;
    uchar* mean = 0;
    uchar* stddev = 0;
    bool stdmtx = false;
    पूर्णांक n1 = (पूर्णांक)_param1.योग();
    पूर्णांक n2 = (पूर्णांक)_param2.योग();

    अगर( disttype == UNIFORM )
    {
        _parambuf.allocate(cn*(माप(DivStruct)+माप(द्विगुन)-1)/माप(द्विगुन) + cn*4);
        द्विगुन* parambuf = _parambuf.तथ्य();
        द्विगुन* p1 = _param1.ptr<द्विगुन>();
        द्विगुन* p2 = _param2.ptr<द्विगुन>();

        अगर( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convertTo(tmp, CV_64F);
            क्रम( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        अगर( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convertTo(tmp, CV_64F);
            क्रम( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        अगर( CV_IS_INT_TYPE(depth) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((माप_प्रकार)(cn*4 - 1), _parambuf.size(), "");
            क्रम( j = 0, fast_int_mode = true; j < cn; j++ )
            {
                द्विगुन a = std::min(p1[j], p2[j]);
                द्विगुन b = std::max(p1[j], p2[j]);
                अगर( saturateRange )
                {
                    a = std::max(a, depth == CV_8U || depth == CV_16U || depth == CV_32U ||
                                 depth == CV_64U || depth == CV_Bool ? 0. :
                                 depth == CV_8S ? -128. : depth == CV_16S ? -32768. :
                                 depth == CV_32S ? (द्विगुन)पूर्णांक_न्यून : (द्विगुन)INT64_MIN);
                    b = std::min(b, depth == CV_8U ? 256. : depth == CV_Bool ? 2. : depth == CV_16U ? 65536. :
                                 depth == CV_8S ? 128. : depth == CV_16S ? 32768. : depth == CV_32U ? (द्विगुन)अच_पूर्णांक_उच्च :
                                 depth == CV_32S ? (द्विगुन)पूर्णांक_उच्च : (द्विगुन)INT64_MAX);
                }
                ip[j][1] = (int64_t)उच्चमान(a);
                int64_t idiff = ip[j][0] = (int64_t)न्यूनमान(b) - ip[j][1] - 1;
                अगर (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                द्विगुन diff = b - a;

                fast_int_mode = fast_int_mode && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                अगर( fast_int_mode )
                    small_flag = idiff <= 255;
                अन्यथा
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    अगर (depth == CV_64S || depth == CV_64U)
                    {
                        minval = INT64_MIN/2;
                        maxval = INT64_MAX;
                    }
                    अगर( diff > (द्विगुन)maxval )
                        ip[j][0] = maxval;
                    अगर( a < (द्विगुन)minval )
                        ip[j][1] = minval;
                }
            }

            uni_param = ip;
            अगर( !fast_int_mode )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((व्योम*)(ds + cn), (व्योम*)(parambuf + _parambuf.size()), "Last byte check");
                क्रम( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    अगर (depth != CV_64U && depth != CV_64S) {
                        अचिन्हित d = ds[j].d = (अचिन्हित)(ip[j][0]+1);
                        पूर्णांक l = 0;
                        जबतक(((uint64)1 << l) < d)
                            l++;
                        ds[j].M = (अचिन्हित)(((uint64)1 << 32)*(((uint64)1 << l) - d)/d) + 1;
                        ds[j].sh1 = std::min(l, 1);
                        ds[j].sh2 = std::max(l - 1, 0);
                    }
                }
                uni_param = ds;
            }

            func = randTab[fast_int_mode ? 1 : 0][depth];
        }
        अन्यथा
        {
            द्विगुन scale = depth == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            द्विगुन maxdiff = saturateRange ? (द्विगुन)भग्न_उच्च : द्विग_उच्च;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((माप_प्रकार)(cn*4 - 1), _parambuf.size(), "");
            अगर( depth != CV_64F )
            {
                Vec2f* fp = (Vec2f*)(parambuf + cn*2);
                क्रम( j = 0; j < cn; j++ )
                {
                    fp[j][0] = (भग्न)(std::min(maxdiff, p2[j] - p1[j])*scale);
                    fp[j][1] = (भग्न)((p2[j] + p1[j])*0.5);
                }
                uni_param = fp;
            }
            अन्यथा
            {
                Vec2d* dp = (Vec2d*)(parambuf + cn*2);
                क्रम( j = 0; j < cn; j++ )
                {
                    dp[j][0] = std::min(द्विग_उच्च, p2[j] - p1[j])*scale;
                    dp[j][1] = ((p2[j] + p1[j])*0.5);
                }
                uni_param = dp;
            }

            func = randTab[0][depth];
        }
        CV_Assert( func != 0 );
    }
    अन्यथा अगर( disttype == RNG::NORMAL )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        द्विगुन* parambuf = _parambuf.तथ्य();

        पूर्णांक ptype = esz1 == 8 ? CV_64F : CV_32F;

        अगर( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            mean = _param1.ptr();
        अन्यथा
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convertTo(tmp, ptype);
            mean = (uchar*)parambuf;
        }

        अगर( n1 < cn )
            क्रम( j = n1*esz1; j < cn*esz1; j++ )
                mean[j] = mean[j - n1*esz1];

        अगर( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        अन्यथा
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convertTo(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        अगर( n2 < cn )
            क्रम( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.rows == cn && _param2.cols == cn;
        scaleFunc = randnScaleTab[depth];
        CV_Assert( scaleFunc != 0 );
    }
    अन्यथा
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    स्थिर Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    भग्न fbuf[BLOCK_आकार + CV_CN_MAX];
    पूर्णांक योग = (पूर्णांक)it.size;
    पूर्णांक block_आकार = std::min((BLOCK_आकार + cn - 1)/cn, योग);
    माप_प्रकार esz = (माप_प्रकार)esz1*cn;
    पूर्णांक flags = mat.type();

    अगर( disttype == UNIFORM )
        flags |= (small_flag ? (पूर्णांक)RNG_FLAG_SMALL : 0);
    अन्यथा
        flags |= (stdmtx ? (पूर्णांक)RNG_FLAG_STDMTX : 0);

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        क्रम( j = 0; j < योग; j += block_आकार )
        {
            पूर्णांक len = std::min(योग - j, block_आकार);

            अगर( disttype == UNIFORM )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            अन्यथा
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
    वापस get_Core_Tls_तथ्य().rng;
}

व्योम cv::setRNGSeed(पूर्णांक seed)
{
    theRNG() = RNG(static_cast<uint64>(seed));
}


व्योम cv::randu(निवेश_Output_Array dst, निवेश_Array निचला, निवेश_Array ऊपरी)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::UNIFORM, निचला, ऊपरी);
}

व्योम cv::randn(निवेश_Output_Array dst, निवेश_Array mean, निवेश_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::NORMAL, mean, stddev);
}

namespace cv
{

ढाँचा<typename T> अटल व्योम
randShuffle_( Mat& _arr, RNG& rng, द्विगुन )
{
    अचिन्हित sz = (अचिन्हित)_arr.योग();
    अगर( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        क्रम( अचिन्हित i = 0; i < sz; i++ )
        {
            अचिन्हित j = (अचिन्हित)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    अन्यथा
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* तथ्य = _arr.ptr();
        माप_प्रकार step = _arr.step;
        पूर्णांक rows = _arr.rows;
        पूर्णांक cols = _arr.cols;
        क्रम( पूर्णांक i0 = 0; i0 < rows; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            क्रम( पूर्णांक j0 = 0; j0 < cols; j0++ )
            {
                अचिन्हित k1 = (अचिन्हित)rng % sz;
                पूर्णांक i1 = (पूर्णांक)(k1 / cols);
                पूर्णांक j1 = (पूर्णांक)(k1 - (अचिन्हित)i1*(अचिन्हित)cols);
                std::swap( p[j0], ((T*)(तथ्य + step*i1))[j1] );
            }
        }
    }
}

प्रकार व्योम (*RandShuffleFunc)( Mat& dst, RNG& rng, द्विगुन iterFactor );

}

व्योम cv::randShuffle( निवेश_Output_Array _dst, द्विगुन iterFactor, RNG* _rng )
{
    CV_INSTRUMENT_REGION();

    RandShuffleFunc tab[] =
    {
        0,
        randShuffle_<uchar>, // 1
        randShuffle_<ushort>, // 2
        randShuffle_<Vec<uchar,3> >, // 3
        randShuffle_<पूर्णांक>, // 4
        0,
        randShuffle_<Vec<ushort,3> >, // 6
        0,
        randShuffle_<Vec<पूर्णांक,2> >, // 8
        0, 0, 0,
        randShuffle_<Vec<पूर्णांक,3> >, // 12
        0, 0, 0,
        randShuffle_<Vec<पूर्णांक,4> >, // 16
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<पूर्णांक,6> >, // 24
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<पूर्णांक,8> > // 32
    };

    Mat dst = _dst.getMat();
    RNG& rng = _rng ? *_rng : theRNG();
    CV_Assert( dst.elem_आकार() <= 32 );
    RandShuffleFunc func = tab[dst.elem_आकार()];
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

cv::RNG_MT19937::RNG_MT19937(अचिन्हित s) { seed(s); }

cv::RNG_MT19937::RNG_MT19937() { seed(5489U); }

व्योम cv::RNG_MT19937::seed(अचिन्हित s)
{
    state[0]= s;
    क्रम (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

अचिन्हित cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    अटल अचिन्हित mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    स्थिर अचिन्हित UPPER_MASK = 0x80000000U;
    स्थिर अचिन्हित LOWER_MASK = 0x7fffffffU;

    /* generate N words at one time */
    अगर (mti >= N)
    {
        पूर्णांक kk = 0;

        क्रम (; kk < N - M; ++kk)
        {
            अचिन्हित y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        क्रम (; kk < N - 1; ++kk)
        {
            अचिन्हित y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
            state[kk] = state[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        अचिन्हित y = (state[N - 1] & UPPER_MASK) | (state[0] & LOWER_MASK);
        state[N - 1] = state[M - 1] ^ (y >> 1) ^ mag01[y & 0x1U];

        mti = 0;
    }

    अचिन्हित y = state[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y <<  7) & 0x9d2c5680U;
    y ^= (y << 15) & 0xefc60000U;
    y ^= (y >> 18);

    वापस y;
}

cv::RNG_MT19937::चालक अचिन्हित() { वापस next(); }

cv::RNG_MT19937::चालक पूर्णांक() { वापस (पूर्णांक)next();}

cv::RNG_MT19937::चालक भग्न() { वापस next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::चालक द्विगुन()
{
    अचिन्हित a = next() >> 5;
    अचिन्हित b = next() >> 6;
    वापस (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

पूर्णांक cv::RNG_MT19937::uniform(पूर्णांक a, पूर्णांक b) { वापस (पूर्णांक)(next() % (b - a) + a); }

भग्न cv::RNG_MT19937::uniform(भग्न a, भग्न b) { वापस ((भग्न)*यह)*(b - a) + a; }

द्विगुन cv::RNG_MT19937::uniform(द्विगुन a, द्विगुन b) { वापस ((द्विगुन)*यह)*(b - a) + a; }

अचिन्हित cv::RNG_MT19937::चालक ()(अचिन्हित b) { वापस next() % b; }

अचिन्हित cv::RNG_MT19937::चालक ()() { वापस next(); }

/* End of file. */
