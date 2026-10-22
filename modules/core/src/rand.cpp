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

#અન્તર્ભાવ "precomp.hpp"

નામાકાશ cv
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

ગણના
{
    RNG_FLAG_SMALL = 0x40000000,
    RNG_FLAG_STDMTX = 0x80000000
};

/***************************************************************************************\
*                           Pseudo-Random Number Generators (PRNGs)                     *
\***************************************************************************************/

template<typename T> સ્થિર શૂન્ય
randBits_( T* arr, પૂર્ણાંક len, પૂર્ણાંક cn, uint64* state, અચલ Vec2l* p, પૂર્ણાંક flags )
{
    તાર્કિક small_flag = (flags & RNG_FLAG_SMALL) != 0;
    uint64 temp = *state;
    પૂર્ણાંક i, k = 0;
    len *= cn;
    --cn;

    જો( !small_flag )
    {
        માટે( i = 0; i <= len - 4; i += 4 )
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
    નહીંતર
    {
        માટે( i = 0; i <= len - 4; i += 4 )
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

    માટે( ; i < len; i++ )
    {
        int64_t t0;
        temp = RNG_NEXT(temp);

        t0 = ((int64_t)temp & p[k][0]) + p[k][1];
        k = CN_NEXT(k);
        arr[i] = saturate_cast<T>(t0);
    }

    *state = temp;
}

સંરચના DivStruct
{
    unsigned d;
    unsigned M;
    પૂર્ણાંક sh1, sh2;
    int64_t delta;
    uint64_t diff;
};

template<typename T> સ્થિર શૂન્ય
randi_( T* arr, પૂર્ણાંક len, પૂર્ણાંક cn, uint64* state, અચલ DivStruct* p )
{
    uint64 temp = *state;
    પૂર્ણાંક k = 0;
    len *= cn;
    cn--;
    માટે( પૂર્ણાંક i = 0; i < len; i++ )
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

સ્થિર શૂન્ય
randi_( int64_t* arr, પૂર્ણાંક len, પૂર્ણાંક cn, uint64* state, અચલ DivStruct* p )
{
    uint64 temp = *state;
    પૂર્ણાંક k = 0;
    len *= cn;
    cn--;
    માટે( પૂર્ણાંક i = 0; i < len; i++ )
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

સ્થિર શૂન્ય
randi_( uint64_t* arr, પૂર્ણાંક len, પૂર્ણાંક cn, uint64* state, અચલ DivStruct* p )
{
    uint64 temp = *state;
    પૂર્ણાંક k = 0;
    len *= cn;
    cn--;
    માટે( પૂર્ણાંક i = 0; i < len; i++ )
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
સ્થિર શૂન્ય randBitsWrap(શૂન્ય* arr, પૂર્ણાંક len, પૂર્ણાંક cn, uint64* state, અચલ શૂન્ય* p, શૂન્ય*, પૂર્ણાંક flags)
{
    randBits_((T*)arr, len, cn, state, (અચલ Vec2l*)p, flags);
}

template<typename T>
સ્થિર શૂન્ય randiWrap(શૂન્ય* arr, પૂર્ણાંક len, પૂર્ણાંક cn, uint64* state, અચલ શૂન્ય* p, શૂન્ય*, પૂર્ણાંક)
{
    randi_((T*)arr, len, cn, state, (અચલ DivStruct*)p);
}

// Narrow an f32 buffer into one of the 1-byte FP8 destinations.
સ્થિર અંતઃરેખ શૂન્ય cvt32fToFP8(અચલ દશાંશ* src, શૂન્ય* dst, પૂર્ણાંક len, પૂર્ણાંક depth)
{
    જો (depth == CV_8F_E4M3FN)        { fp8_t*   d = (fp8_t*)dst;   માટે (પૂર્ણાંક i = 0; i < len; i++) d[i] = fp8_t(src[i]); }
    નહીંતર                              { fp8a_t* d = (fp8a_t*)dst; માટે (પૂર્ણાંક i = 0; i < len; i++) d[i] = fp8a_t(src[i]); }
}
સ્થિર અંતઃરેખ તાર્કિક isFP8Depth(પૂર્ણાંક d) { પાછા d >= CV_8F_E4M3FN && d <= CV_8F_E4M3FNUZ; }

સ્થિર શૂન્ય randf_16_or_32f( શૂન્ય* dst, પૂર્ણાંક len_, પૂર્ણાંક cn, uint64* state, અચલ શૂન્ય* _p, શૂન્ય* _fbuf, પૂર્ણાંક flags )
{
    અચલ Vec2f* p = (અચલ Vec2f*)_p;
    દશાંશ* fbuf = (દશાંશ*)_fbuf;
    પૂર્ણાંક depth = CV_MAT_DEPTH(flags);
    uint64 temp = *state;
    પૂર્ણાંક k = 0, len = len_*cn;
    દશાંશ* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : (દશાંશ*)dst;
    cn--;
    માટે( પૂર્ણાંક i = 0; i < len; i++ )
    {
        પૂર્ણાંક t = (પૂર્ણાંક)(temp = RNG_NEXT(temp));
        arr[i] = (દશાંશ)(t*p[k][0]);
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias32f(arr, &p[0][0], len_, cn+1);
    જો (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    નહીંતર જો (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    નહીંતર જો (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

સ્થિર શૂન્ય
randf_64f( શૂન્ય* _arr, પૂર્ણાંક len_, પૂર્ણાંક cn, uint64* state, અચલ શૂન્ય* _p, શૂન્ય*, પૂર્ણાંક )
{
    દ્વિગુણ* arr = (દ્વિગુણ*)_arr;
    અચલ Vec2d* p = (અચલ Vec2d*)_p;
    uint64 temp = *state;
    પૂર્ણાંક k = 0, len = len_*cn;
    cn--;
    માટે( પૂર્ણાંક i = 0; i < len; i++ )
    {
        temp = RNG_NEXT(temp);
        int64_t v = (int64_t)((temp >> 32) | (temp << 32));
        arr[i] = v*p[k][0];
        k = CN_NEXT(k);
    }
    *state = temp;
    hal::addRNGBias64f(arr, &p[0][0], len_, cn+1);
}

પ્રકારવ્યાખ્યા શૂન્ય (*RandFunc)(શૂન્ય* arr, પૂર્ણાંક len, પૂર્ણાંક cn, uint64* state,
                         અચલ શૂન્ય* p, શૂન્ય* tempbuf, પૂર્ણાંક flags);

સ્થિર RandFunc randTab[CV_DEPTH_MAX][CV_DEPTH_MAX] =
{
    {
        randiWrap<uchar>, randiWrap<schar>, randiWrap<ushort>,
        randiWrap<short>, randiWrap<પૂર્ણાંક>, randf_16_or_32f,
        randf_64f, randf_16_or_32f, randf_16_or_32f,
        randiWrap<તાર્કિક>, randiWrap<uint64_t>, randiWrap<int64_t>,
        randiWrap<unsigned>,
        randf_16_or_32f, randf_16_or_32f    // CV_8F_E4M3FN, E4M3FNUZ
    },
    {
        randBitsWrap<uchar>, randBitsWrap<schar>, randBitsWrap<ushort>,
        randBitsWrap<short>, randBitsWrap<પૂર્ણાંક>, 0, 0, 0, 0,
        randBitsWrap<તાર્કિક>, randBitsWrap<uint64_t>, randBitsWrap<int64_t>,
        randBitsWrap<unsigned>, 0, 0, 0
    }
};

/*
   The code below implements the algorithm described in
   "The Ziggurat Method for Generating Random Variables"
   by George Marsaglia and Wai Wan Tsang, Journal of Statistical Software, 2007.
*/
સ્થિર શૂન્ય
randn_0_1_32f( દશાંશ* arr, પૂર્ણાંક len, uint64* state )
{
    અચલ દશાંશ r = 3.442620f; // The start of the right tail
    અચલ દશાંશ rng_flt = 2.3283064365386962890625e-10f; // 2^-32
    સ્થિર unsigned kn[128];
    સ્થિર દશાંશ wn[128], fn[128];
    uint64 temp = *state;
    સ્થિર તાર્કિક initialized=ખોટું;
    પૂર્ણાંક i;

    જો( !initialized )
    {
        અચલ દ્વિગુણ m1 = 2147483648.0;
        દ્વિગુણ dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;

        // Set up the tables
        દ્વિગુણ q = vn/std::exp(-.5*dn*dn);
        kn[0] = (unsigned)((dn/q)*m1);
        kn[1] = 0;

        wn[0] = (દશાંશ)(q/m1);
        wn[127] = (દશાંશ)(dn/m1);

        fn[0] = 1.f;
        fn[127] = (દશાંશ)std::exp(-.5*dn*dn);

        માટે(i=126;i>=1;i--)
        {
            dn = std::sqrt(-2.*std::log(vn/dn+std::exp(-.5*dn*dn)));
            kn[i+1] = (unsigned)((dn/tn)*m1);
            tn = dn;
            fn[i] = (દશાંશ)std::exp(-.5*dn*dn);
            wn[i] = (દશાંશ)(dn/m1);
        }
        initialized = સાચું;
    }

    માટે( i = 0; i < len; i++ )
    {
        દશાંશ x, y;
        માટે(;;)
        {
            પૂર્ણાંક hz = (પૂર્ણાંક)temp;
            temp = RNG_NEXT(temp);
            પૂર્ણાંક iz = hz & 127;
            x = hz*wn[iz];
            જો( (unsigned)std::abs(hz) < kn[iz] )
                તોડો;
            જો( iz == 0) // iz==0, handles the base strip
            {
                કરો
                {
                    x = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    y = (unsigned)temp*rng_flt;
                    temp = RNG_NEXT(temp);
                    x = (દશાંશ)(-std::log(x+FLT_MIN)*0.2904764);
                    y = (દશાંશ)-std::log(y+FLT_MIN);
                }	// .2904764 is 1/r
                જ્યારે( y + y < x*x );
                x = hz > 0 ? r + x : -r - x;
                તોડો;
            }
            // iz > 0, handle the wedges of other strips
            y = (unsigned)temp*rng_flt;
            temp = RNG_NEXT(temp);
            જો( fn[iz] + y*(fn[iz - 1] - fn[iz]) < std::exp(-.5*x*x) )
                તોડો;
        }
        arr[i] = x;
    }
    *state = temp;
}


દ્વિગુણ RNG::gaussian(દ્વિગુણ sigma)
{
    દશાંશ temp;
    randn_0_1_32f( &temp, 1, &state );
    પાછા temp*sigma;
}

template<typename T, typename PT> સ્થિર શૂન્ય
randnScale_(દશાંશ* src, શૂન્ય* _dst, પૂર્ણાંક len, પૂર્ણાંક cn,
            અચલ શૂન્ય* _mean, અચલ શૂન્ય* _stddev, પૂર્ણાંક flags )
{
    T* dst = (T*)_dst;
    અચલ PT* mean = (અચલ PT*)_mean;
    અચલ PT* stddev = (અચલ PT*)_stddev;
    તાર્કિક stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    પૂર્ણાંક i, j, k;
    જો( !stdmtx || cn == 1 )
    {
        જો( cn == 1 )
        {
            PT a = stddev[0], b = mean[0];
            માટે( i = 0; i < len; i++ )
                dst[i] = saturate_cast<T>(src[i]*a + b);
        }
        નહીંતર
        {
            len *= cn;
            cn--;
            માટે( i = k = 0; i < len; i++ ) {
                dst[i] = saturate_cast<T>(src[i]*stddev[k] + mean[k]);
                k = CN_NEXT(k);
            }
        }
    }
    નહીંતર
    {
        len *= cn;
        cn--;
        માટે( i = j = 0; i < len; i++ )
        {
            PT s = mean[j];
            પૂર્ણાંક i0 = i - j;
            માટે( k = 0; k <= cn; k++ )
                s += src[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = saturate_cast<T>(s);
            j = CN_NEXT(j);
        }
    }
}

// special version for 16f, 16bf and 32f
સ્થિર શૂન્ય
randnScale_16_or_32f(દશાંશ* fbuf, શૂન્ય* _dst, પૂર્ણાંક len, પૂર્ણાંક cn,
                     અચલ શૂન્ય* _mean, અચલ શૂન્ય* _stddev, પૂર્ણાંક flags)
{
    દશાંશ* dst = (દશાંશ*)_dst;
    અચલ દશાંશ* mean = (અચલ દશાંશ*)_mean;
    અચલ દશાંશ* stddev = (અચલ દશાંશ*)_stddev;
    તાર્કિક stdmtx = (flags & RNG_FLAG_STDMTX) != 0;
    પૂર્ણાંક depth = CV_MAT_DEPTH(flags);
    દશાંશ* arr = depth == CV_16F || depth == CV_16BF || isFP8Depth(depth) ? fbuf : dst;
    પૂર્ણાંક i, j, k;

    જો( !stdmtx || cn == 1 )
    {
        જો( cn == 1 )
        {
            દશાંશ a = stddev[0], b = mean[0];
            માટે( i = 0; i < len; i++ )
                arr[i] = fbuf[i]*a + b;
        }
        નહીંતર
        {
            len *= cn;
            cn--;
            માટે( i = k = 0; i < len; i++ ) {
                arr[i] = fbuf[i]*stddev[k] + mean[k];
                k = CN_NEXT(k);
            }
        }
    }
    નહીંતર જો( depth == CV_32F )
    {
        len *= cn;
        cn--;
        માટે( i = j = 0; i < len; i++ )
        {
            દશાંશ s = mean[j];
            પૂર્ણાંક i0 = i - j;
            માટે( k = 0; k <= cn; k++ )
                s += fbuf[i0 + k]*stddev[j*(cn+1) + k];
            dst[i] = s;
            j = CN_NEXT(j);
        }
    }
    નહીંતર
    {
        દશાંશ elembuf[CV_CN_MAX];
        len *= cn;
        માટે( i = 0; i < len; i += cn )
        {
            // since we process fbuf in-place,
            // we need to copy each cn-channel element
            // prior to matrix multiplication
            માટે (j = 0; j < cn; j++)
                elembuf[j] = fbuf[i + j];
            માટે (j = 0; j < cn; j++) {
                દશાંશ s = mean[j];
                માટે( k = 0; k < cn; k++ )
                    s += elembuf[k]*stddev[j*cn + k];
                fbuf[i + j] = s;
            }
        }
    }
    જો (depth == CV_16F)
        hal::cvt32f16f(fbuf, (hfloat*)dst, len);
    નહીંતર જો (depth == CV_16BF)
        hal::cvt32f16bf(fbuf, (bfloat*)dst, len);
    નહીંતર જો (isFP8Depth(depth))
        cvt32fToFP8(fbuf, dst, len, depth);
}

પ્રકારવ્યાખ્યા શૂન્ય (*RandnScaleFunc)(દશાંશ* src, શૂન્ય* dst, પૂર્ણાંક len, પૂર્ણાંક cn,
                               અચલ શૂન્ય* mean, અચલ શૂન્ય* stddev, પૂર્ણાંક flags);

સ્થિર RandnScaleFunc randnScaleTab[CV_DEPTH_MAX] =
{
    randnScale_<uchar, દશાંશ>, randnScale_<schar, દશાંશ>, randnScale_<ushort, દશાંશ>,
    randnScale_<short, દશાંશ>, randnScale_<પૂર્ણાંક, દશાંશ>, randnScale_16_or_32f,
    randnScale_<દ્વિગુણ, દ્વિગુણ>, randnScale_16_or_32f, randnScale_16_or_32f,
    randnScale_<તાર્કિક, દશાંશ>, randnScale_<uint64_t, દ્વિગુણ>, randnScale_<int64_t, દ્વિગુણ>,
    randnScale_<unsigned, દશાંશ>,
    randnScale_16_or_32f, randnScale_16_or_32f   // CV_8F_E4M3FN, E4M3FNUZ
};

શૂન્ય RNG::fill( આગત_Output_Array _mat, પૂર્ણાંક disttype,
                આગત_Array _param1arg, આગત_Array _param2arg,
                તાર્કિક saturateRange )
{
    CV_Assert(!_mat.empty());

    Mat mat = _mat.getMat(), _param1 = _param1arg.getMat(), _param2 = _param2arg.getMat();
    પૂર્ણાંક j, depth = mat.depth(), cn = mat.channels();
    પૂર્ણાંક esz1 = CV_ELEM_SIZE(depth);
    AutoBuffer<દ્વિગુણ> _parambuf;
    તાર્કિક fast_int_mode = ખોટું;
    તાર્કિક small_flag = ખોટું;
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

    અચલ શૂન્ય* uni_param = 0;
    uchar* mean = 0;
    uchar* stddev = 0;
    તાર્કિક stdmtx = ખોટું;
    પૂર્ણાંક n1 = (પૂર્ણાંક)_param1.total();
    પૂર્ણાંક n2 = (પૂર્ણાંક)_param2.total();

    જો( disttype == UNIFORM )
    {
        _parambuf.allocate(cn*(કદ(DivStruct)+કદ(દ્વિગુણ)-1)/કદ(દ્વિગુણ) + cn*4);
        દ્વિગુણ* parambuf = _parambuf.data();
        દ્વિગુણ* p1 = _param1.ptr<દ્વિગુણ>();
        દ્વિગુણ* p2 = _param2.ptr<દ્વિગુણ>();

        જો( !_param1.isContinuous() || _param1.type() != CV_64F || n1 != cn )
        {
            p1 = parambuf;
            Mat tmp(_param1.size(), CV_64F, p1);
            _param1.convertTo(tmp, CV_64F);
            માટે( j = n1; j < cn; j++ )
                p1[j] = p1[j-n1];
        }

        જો( !_param2.isContinuous() || _param2.type() != CV_64F || n2 != cn )
        {
            p2 = parambuf + cn;
            Mat tmp(_param2.size(), CV_64F, p2);
            _param2.convertTo(tmp, CV_64F);
            માટે( j = n2; j < cn; j++ )
                p2[j] = p2[j-n2];
        }

        જો( CV_IS_INT_TYPE(depth) )
        {
            Vec2l* ip = (Vec2l*)(parambuf + cn*2);
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            માટે( j = 0, fast_int_mode = સાચું; j < cn; j++ )
            {
                દ્વિગુણ a = std::min(p1[j], p2[j]);
                દ્વિગુણ b = std::max(p1[j], p2[j]);
                જો( saturateRange )
                {
                    a = std::max(a, depth == CV_8U || depth == CV_16U || depth == CV_32U ||
                                 depth == CV_64U || depth == CV_Bool ? 0. :
                                 depth == CV_8S ? -128. : depth == CV_16S ? -32768. :
                                 depth == CV_32S ? (દ્વિગુણ)INT_MIN : (દ્વિગુણ)INT64_MIN);
                    b = std::min(b, depth == CV_8U ? 256. : depth == CV_Bool ? 2. : depth == CV_16U ? 65536. :
                                 depth == CV_8S ? 128. : depth == CV_16S ? 32768. : depth == CV_32U ? (દ્વિગુણ)UINT_MAX :
                                 depth == CV_32S ? (દ્વિગુણ)INT_MAX : (દ્વિગુણ)INT64_MAX);
                }
                ip[j][1] = (int64_t)ceil(a);
                int64_t idiff = ip[j][0] = (int64_t)floor(b) - ip[j][1] - 1;
                જો (idiff < 0)
                {
                    idiff = 0;
                    ip[j][0] = 0;
                }
                દ્વિગુણ diff = b - a;

                fast_int_mode = fast_int_mode && diff <= 4294967296. && (idiff & (idiff+1)) == 0;
                જો( fast_int_mode )
                    small_flag = idiff <= 255;
                નહીંતર
                {
                    int64_t minval = INT32_MIN/2, maxval = INT32_MAX;
                    જો (depth == CV_64S || depth == CV_64U)
                    {
                        minval = INT64_MIN/2;
                        maxval = INT64_MAX;
                    }
                    જો( diff > (દ્વિગુણ)maxval )
                        ip[j][0] = maxval;
                    જો( a < (દ્વિગુણ)minval )
                        ip[j][1] = minval;
                }
            }

            uni_param = ip;
            જો( !fast_int_mode )
            {
                DivStruct* ds = (DivStruct*)(ip + cn);
                CV_DbgCheckLE((શૂન્ય*)(ds + cn), (શૂન્ય*)(parambuf + _parambuf.size()), "Last byte check");
                માટે( j = 0; j < cn; j++ )
                {
                    ds[j].delta = ip[j][1];
                    ds[j].diff = ip[j][0];
                    જો (depth != CV_64U && depth != CV_64S) {
                        unsigned d = ds[j].d = (unsigned)(ip[j][0]+1);
                        પૂર્ણાંક l = 0;
                        જ્યારે(((uint64)1 << l) < d)
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
        નહીંતર
        {
            દ્વિગુણ scale = depth == CV_64F ?
                5.4210108624275221700372640043497e-20 : // 2**-64
                2.3283064365386962890625e-10;           // 2**-32
            દ્વિગુણ maxdiff = saturateRange ? (દ્વિગુણ)FLT_MAX : DBL_MAX;

            // for each channel i compute such dparam[0][i] & dparam[1][i],
            // so that a signed 32/64-bit integer X is transformed to
            // the range [param1.val[i], param2.val[i]) using
            // dparam[0][i]*X + dparam[1][i]
            CV_DbgCheckLT((size_t)(cn*4 - 1), _parambuf.size(), "");
            જો( depth != CV_64F )
            {
                Vec2f* fp = (Vec2f*)(parambuf + cn*2);
                માટે( j = 0; j < cn; j++ )
                {
                    fp[j][0] = (દશાંશ)(std::min(maxdiff, p2[j] - p1[j])*scale);
                    fp[j][1] = (દશાંશ)((p2[j] + p1[j])*0.5);
                }
                uni_param = fp;
            }
            નહીંતર
            {
                Vec2d* dp = (Vec2d*)(parambuf + cn*2);
                માટે( j = 0; j < cn; j++ )
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
    નહીંતર જો( disttype == RNG::NORMAL )
    {
        _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
        દ્વિગુણ* parambuf = _parambuf.data();

        પૂર્ણાંક ptype = esz1 == 8 ? CV_64F : CV_32F;

        જો( _param1.isContinuous() && _param1.type() == ptype && n1 >= cn)
            mean = _param1.ptr();
        નહીંતર
        {
            Mat tmp(_param1.size(), ptype, parambuf);
            _param1.convertTo(tmp, ptype);
            mean = (uchar*)parambuf;
        }

        જો( n1 < cn )
            માટે( j = n1*esz1; j < cn*esz1; j++ )
                mean[j] = mean[j - n1*esz1];

        જો( _param2.isContinuous() && _param2.type() == ptype && n2 >= cn)
            stddev = _param2.ptr();
        નહીંતર
        {
            Mat tmp(_param2.size(), ptype, parambuf + MAX(n1, cn));
            _param2.convertTo(tmp, ptype);
            stddev = (uchar*)(parambuf + MAX(n1, cn));
        }

        જો( n2 < cn )
            માટે( j = n2*esz1; j < cn*esz1; j++ )
                stddev[j] = stddev[j - n2*esz1];

        stdmtx = _param2.rows == cn && _param2.cols == cn;
        scaleFunc = randnScaleTab[depth];
        CV_Assert( scaleFunc != 0 );
    }
    નહીંતર
        CV_Error( cv::Error::StsBadArg, "Unknown distribution type" );

    અચલ Mat* arrays[] = {&mat, 0};
    uchar* ptr = 0;
    NAryMatIterator it(arrays, &ptr, 1);
    દશાંશ fbuf[BLOCK_SIZE + CV_CN_MAX];
    પૂર્ણાંક total = (પૂર્ણાંક)it.size;
    પૂર્ણાંક blockSize = std::min((BLOCK_SIZE + cn - 1)/cn, total);
    size_t esz = (size_t)esz1*cn;
    પૂર્ણાંક flags = mat.type();

    જો( disttype == UNIFORM )
        flags |= (small_flag ? (પૂર્ણાંક)RNG_FLAG_SMALL : 0);
    નહીંતર
        flags |= (stdmtx ? (પૂર્ણાંક)RNG_FLAG_STDMTX : 0);

    માટે( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        માટે( j = 0; j < total; j += blockSize )
        {
            પૂર્ણાંક len = std::min(total - j, blockSize);

            જો( disttype == UNIFORM )
                func(ptr + j*esz, len, cn, &state, uni_param, fbuf, flags);
            નહીંતર
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
    પાછા getCoreTlsData().rng;
}

શૂન્ય cv::setRNGSeed(પૂર્ણાંક seed)
{
    theRNG() = RNG(static_cast<uint64>(seed));
}


શૂન્ય cv::randu(આગત_Output_Array dst, આગત_Array નીચલું, આગત_Array ઉપલું)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::UNIFORM, નીચલું, ઉપલું);
}

શૂન્ય cv::randn(આગત_Output_Array dst, આગત_Array mean, આગત_Array stddev)
{
    CV_INSTRUMENT_REGION();

    theRNG().fill(dst, RNG::NORMAL, mean, stddev);
}

નામાકાશ cv
{

template<typename T> સ્થિર શૂન્ય
randShuffle_( Mat& _arr, RNG& rng, દ્વિગુણ )
{
    unsigned sz = (unsigned)_arr.total();
    જો( _arr.isContinuous() )
    {
        T* arr = _arr.ptr<T>();
        માટે( unsigned i = 0; i < sz; i++ )
        {
            unsigned j = (unsigned)rng % sz;
            std::swap( arr[j], arr[i] );
        }
    }
    નહીંતર
    {
        CV_Assert( _arr.dims <= 2 );
        uchar* data = _arr.ptr();
        size_t step = _arr.step;
        પૂર્ણાંક rows = _arr.rows;
        પૂર્ણાંક cols = _arr.cols;
        માટે( પૂર્ણાંક i0 = 0; i0 < rows; i0++ )
        {
            T* p = _arr.ptr<T>(i0);
            માટે( પૂર્ણાંક j0 = 0; j0 < cols; j0++ )
            {
                unsigned k1 = (unsigned)rng % sz;
                પૂર્ણાંક i1 = (પૂર્ણાંક)(k1 / cols);
                પૂર્ણાંક j1 = (પૂર્ણાંક)(k1 - (unsigned)i1*(unsigned)cols);
                std::swap( p[j0], ((T*)(data + step*i1))[j1] );
            }
        }
    }
}

પ્રકારવ્યાખ્યા શૂન્ય (*RandShuffleFunc)( Mat& dst, RNG& rng, દ્વિગુણ iterFactor );

}

શૂન્ય cv::randShuffle( આગત_Output_Array _dst, દ્વિગુણ iterFactor, RNG* _rng )
{
    CV_INSTRUMENT_REGION();

    RandShuffleFunc tab[] =
    {
        0,
        randShuffle_<uchar>, // 1
        randShuffle_<ushort>, // 2
        randShuffle_<Vec<uchar,3> >, // 3
        randShuffle_<પૂર્ણાંક>, // 4
        0,
        randShuffle_<Vec<ushort,3> >, // 6
        0,
        randShuffle_<Vec<પૂર્ણાંક,2> >, // 8
        0, 0, 0,
        randShuffle_<Vec<પૂર્ણાંક,3> >, // 12
        0, 0, 0,
        randShuffle_<Vec<પૂર્ણાંક,4> >, // 16
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<પૂર્ણાંક,6> >, // 24
        0, 0, 0, 0, 0, 0, 0,
        randShuffle_<Vec<પૂર્ણાંક,8> > // 32
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

શૂન્ય cv::RNG_MT19937::seed(unsigned s)
{
    state[0]= s;
    માટે (mti = 1; mti < N; mti++)
    {
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        state[mti] = (1812433253U * (state[mti - 1] ^ (state[mti - 1] >> 30)) + mti);
    }
}

unsigned cv::RNG_MT19937::next()
{
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    સ્થિર unsigned mag01[2] = { 0x0U, /*MATRIX_A*/ 0x9908b0dfU};

    અચલ unsigned UPPER_MASK = 0x80000000U;
    અચલ unsigned LOWER_MASK = 0x7fffffffU;

    /* generate N words at one time */
    જો (mti >= N)
    {
        પૂર્ણાંક kk = 0;

        માટે (; kk < N - M; ++kk)
        {
            unsigned y = (state[kk] & UPPER_MASK) | (state[kk + 1] & LOWER_MASK);
            state[kk] = state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }

        માટે (; kk < N - 1; ++kk)
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

    પાછા y;
}

cv::RNG_MT19937::operator unsigned() { પાછા next(); }

cv::RNG_MT19937::operator પૂર્ણાંક() { પાછા (પૂર્ણાંક)next();}

cv::RNG_MT19937::operator દશાંશ() { પાછા next() * (1.f / 4294967296.f); }

cv::RNG_MT19937::operator દ્વિગુણ()
{
    unsigned a = next() >> 5;
    unsigned b = next() >> 6;
    પાછા (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

પૂર્ણાંક cv::RNG_MT19937::uniform(પૂર્ણાંક a, પૂર્ણાંક b) { પાછા (પૂર્ણાંક)(next() % (b - a) + a); }

દશાંશ cv::RNG_MT19937::uniform(દશાંશ a, દશાંશ b) { પાછા ((દશાંશ)*આ)*(b - a) + a; }

દ્વિગુણ cv::RNG_MT19937::uniform(દ્વિગુણ a, દ્વિગુણ b) { પાછા ((દ્વિગુણ)*આ)*(b - a) + a; }

unsigned cv::RNG_MT19937::operator ()(unsigned b) { પાછા next() % b; }

unsigned cv::RNG_MT19937::operator ()() { પાછા next(); }

/* End of file. */
