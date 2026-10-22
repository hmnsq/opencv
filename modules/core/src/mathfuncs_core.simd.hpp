// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#समावेश "mathfuncs.hpp"

namespace cv { namespace hal {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
व्योम cartToPolar32f(स्थिर भग्न *X, स्थिर भग्न *Y, भग्न* mag, भग्न *angle, पूर्णांक len, bool angleInDegrees);
व्योम cartToPolar64f(स्थिर द्विगुन *X, स्थिर द्विगुन *Y, द्विगुन* mag, द्विगुन *angle, पूर्णांक len, bool angleInDegrees);
व्योम polarToCart32f(स्थिर भग्न *mag, स्थिर भग्न *angle, भग्न *X, भग्न *Y, पूर्णांक len, bool angleInDegrees);
व्योम polarToCart64f(स्थिर द्विगुन *mag, स्थिर द्विगुन *angle, द्विगुन *X, द्विगुन *Y, पूर्णांक len, bool angleInDegrees);
व्योम fastAtan32f(स्थिर भग्न *Y, स्थिर भग्न *X, भग्न *angle, पूर्णांक len, bool angleInDegrees);
व्योम fastAtan64f(स्थिर द्विगुन *Y, स्थिर द्विगुन *X, द्विगुन *angle, पूर्णांक len, bool angleInDegrees);
व्योम fastAtan2(स्थिर भग्न *Y, स्थिर भग्न *X, भग्न *angle, पूर्णांक len, bool angleInDegrees);
व्योम magnitude32f(स्थिर भग्न* x, स्थिर भग्न* y, भग्न* mag, पूर्णांक len);
व्योम magnitude64f(स्थिर द्विगुन* x, स्थिर द्विगुन* y, द्विगुन* mag, पूर्णांक len);
व्योम invSqrt32f(स्थिर भग्न* src, भग्न* dst, पूर्णांक len);
व्योम invSqrt64f(स्थिर द्विगुन* src, द्विगुन* dst, पूर्णांक len);
व्योम sqrt32f(स्थिर भग्न* src, भग्न* dst, पूर्णांक len);
व्योम sqrt64f(स्थिर द्विगुन* src, द्विगुन* dst, पूर्णांक len);
भग्न fastAtan2(भग्न y, भग्न x);

#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

using namespace std;
using namespace cv;

namespace {

अटल स्थिर भग्न atan2_p1 = 0.9997878412794807f*(भग्न)(180/CV_PI);
अटल स्थिर भग्न atan2_p3 = -0.3258083974640975f*(भग्न)(180/CV_PI);
अटल स्थिर भग्न atan2_p5 = 0.1555786518463281f*(भग्न)(180/CV_PI);
अटल स्थिर भग्न atan2_p7 = -0.04432655554792128f*(भग्न)(180/CV_PI);

#अगर_घोषित __EMSCRIPTEN__
अटल अंतरभूत भग्न atan_f32(भग्न y, भग्न x)
{
    भग्न a = atan2(y, x) * 180.0f / CV_PI;
    अगर (a < 0.0f)
        a += 360.0f;
    अगर (a >= 360.0f)
        a -= 360.0f;
    वापस a; // range [0; 360)
}
#अन्यथा
अटल अंतरभूत भग्न atan_f32(भग्न y, भग्न x)
{
    भग्न ax = std::असल(x), ay = std::असल(y);
    भग्न a, c, c2;
    अगर( ax >= ay )
    {
        c = ay/(ax + (भग्न)द्विग_अंतर);
        c2 = c*c;
        a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    अन्यथा
    {
        c = ax/(ay + (भग्न)द्विग_अंतर);
        c2 = c*c;
        a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    अगर( x < 0 )
        a = 180.f - a;
    अगर( y < 0 )
        a = 360.f - a;
    वापस a;
}
#पूर्ण_अगर

#अगर (CV_SIMD || CV_SIMD_SCALABLE)

v_float32 v_atan_f32(स्थिर v_float32& y, स्थिर v_float32& x)
{
    v_float32 eps = vx_setall_f32((भग्न)द्विग_अंतर);
    v_float32 z = vx_setzero_f32();
    v_float32 p7 = vx_setall_f32(atan2_p7);
    v_float32 p5 = vx_setall_f32(atan2_p5);
    v_float32 p3 = vx_setall_f32(atan2_p3);
    v_float32 p1 = vx_setall_f32(atan2_p1);
    v_float32 val90 = vx_setall_f32(90.f);
    v_float32 val180 = vx_setall_f32(180.f);
    v_float32 val360 = vx_setall_f32(360.f);

    v_float32 ax = v_abs(x);
    v_float32 ay = v_abs(y);
    v_float32 c = v_div(v_min(ax, ay), v_add(v_max(ax, ay), eps));
    v_float32 cc = v_mul(c, c);
    v_float32 a = v_mul(v_fma(v_fma(v_fma(cc, p7, p5), cc, p3), cc, p1), c);
    a = v_select(v_ge(ax, ay), a, v_sub(val90, a));
    a = v_select(v_lt(x, z), v_sub(val180, a), a);
    a = v_select(v_lt(y, z), v_sub(val360, a), a);
    वापस a;
}

#पूर्ण_अगर

} // anonymous::

अटल व्योम cartToPolar32f_(स्थिर भग्न *X, स्थिर भग्न *Y, भग्न *mag, भग्न *angle, पूर्णांक len, bool angleInDegrees )
{
    भग्न scale = angleInDegrees ? 1.f : (भग्न)(CV_PI/180);
    पूर्णांक i = 0;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिर पूर्णांक VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    क्रम( ; i < len; i += VECSZ*2 )
    {
        अगर( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            अगर( i == 0 || angle == X || angle == Y )
                अवरोध;
            i = len - VECSZ*2;
        }

        v_float32 x0 = vx_load(X + i);
        v_float32 y0 = vx_load(Y + i);
        v_float32 x1 = vx_load(X + i + VECSZ);
        v_float32 y1 = vx_load(Y + i + VECSZ);

        v_float32 m0 = v_sqrt(v_muladd(x0, x0, v_mul(y0, y0)));
        v_float32 m1 = v_sqrt(v_muladd(x1, x1, v_mul(y1, y1)));

        v_float32 r0 = v_mul(v_atan_f32(y0, x0), s);
        v_float32 r1 = v_mul(v_atan_f32(y1, x1), s);

        v_store(mag + i, m0);
        v_store(mag + i + VECSZ, m1);

        v_store(angle + i, r0);
        v_store(angle + i + VECSZ, r1);
    }
    vx_cleanup();
#पूर्ण_अगर

    क्रम( ; i < len; i++ )
    {
        भग्न x0 = X[i], y0 = Y[i];
        mag[i] = std::वर्ग_मूल(x0*x0 + y0*y0);
        angle[i] = atan_f32(y0, x0)*scale;
    }
}

व्योम cartToPolar32f(स्थिर भग्न *X, स्थिर भग्न *Y, भग्न *mag, भग्न *angle, पूर्णांक len, bool angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    cartToPolar32f_(X, Y, mag, angle, len, angleInDegrees );
}

व्योम cartToPolar64f(स्थिर द्विगुन *X, स्थिर द्विगुन *Y, द्विगुन *mag, द्विगुन *angle, पूर्णांक len, bool angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    स्थिर पूर्णांक BLKSZ = 128;
    भग्न ybuf[BLKSZ], xbuf[BLKSZ], mbuf[BLKSZ], abuf[BLKSZ];
    क्रम( पूर्णांक i = 0; i < len; i += BLKSZ )
    {
        पूर्णांक j, blksz = std::min(BLKSZ, len - i);
        क्रम( j = 0; j < blksz; j++ )
        {
            xbuf[j] = (भग्न)X[i + j];
            ybuf[j] = (भग्न)Y[i + j];
        }
        cartToPolar32f_(xbuf, ybuf, mbuf, abuf, blksz, angleInDegrees);
        क्रम( j = 0; j < blksz; j++ )
            mag[i + j] = mbuf[j];
        क्रम( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

namespace {

अटल अंतरभूत व्योम SinCos_32f(स्थिर भग्न* mag, स्थिर भग्न* angle, भग्न* cosval, भग्न* sinval, पूर्णांक len, पूर्णांक angle_in_degrees)
{
    स्थिर पूर्णांक N = 64;

    अटल स्थिर द्विगुन sin_table[] =
    {
     0.00000000000000000000,     0.09801714032956060400,
     0.19509032201612825000,     0.29028467725446233000,
     0.38268343236508978000,     0.47139673682599764000,
     0.55557023301960218000,     0.63439328416364549000,
     0.70710678118654746000,     0.77301045336273699000,
     0.83146961230254524000,     0.88192126434835494000,
     0.92387953251128674000,     0.95694033573220894000,
     0.98078528040323043000,     0.99518472667219682000,
     1.00000000000000000000,     0.99518472667219693000,
     0.98078528040323043000,     0.95694033573220894000,
     0.92387953251128674000,     0.88192126434835505000,
     0.83146961230254546000,     0.77301045336273710000,
     0.70710678118654757000,     0.63439328416364549000,
     0.55557023301960218000,     0.47139673682599786000,
     0.38268343236508989000,     0.29028467725446239000,
     0.19509032201612861000,     0.09801714032956082600,
     0.00000000000000012246,    -0.09801714032956059000,
    -0.19509032201612836000,    -0.29028467725446211000,
    -0.38268343236508967000,    -0.47139673682599764000,
    -0.55557023301960196000,    -0.63439328416364527000,
    -0.70710678118654746000,    -0.77301045336273666000,
    -0.83146961230254524000,    -0.88192126434835494000,
    -0.92387953251128652000,    -0.95694033573220882000,
    -0.98078528040323032000,    -0.99518472667219693000,
    -1.00000000000000000000,    -0.99518472667219693000,
    -0.98078528040323043000,    -0.95694033573220894000,
    -0.92387953251128663000,    -0.88192126434835505000,
    -0.83146961230254546000,    -0.77301045336273688000,
    -0.70710678118654768000,    -0.63439328416364593000,
    -0.55557023301960218000,    -0.47139673682599792000,
    -0.38268343236509039000,    -0.29028467725446250000,
    -0.19509032201612872000,    -0.09801714032956050600,
    };

    अटल स्थिर द्विगुन k2 = (2*CV_PI)/N;

    अटल स्थिर द्विगुन sin_a0 = -0.166630293345647*k2*k2*k2;
    अटल स्थिर द्विगुन sin_a2 = k2;

    अटल स्थिर द्विगुन cos_a0 = -0.499818138450326*k2*k2;
    /*static const double cos_a2 =  1;*/

    द्विगुन k1;
    पूर्णांक i = 0;

    अगर( !angle_in_degrees )
        k1 = N/(2*CV_PI);
    अन्यथा
        k1 = N/360.;

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिर पूर्णांक VECSZ = VTraits<v_float32>::vlanes();
    स्थिर v_float32 scale = vx_setall_f32(angle_in_degrees ? (भग्न)CV_PI / 180.f : 1.f);

    क्रम( ; i < len; i += VECSZ*2 )
    {
        अगर( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            अगर( i == 0 || angle == cosval || angle == sinval || mag == cosval || mag == sinval )
                अवरोध;
            i = len - VECSZ*2;
        }

        v_float32 r0 = v_mul(vx_load(angle + i), scale);
        v_float32 r1 = v_mul(vx_load(angle + i + VECSZ), scale);

        v_float32 c0, c1, s0, s1;
        v_sincos(r0, s0, c0);
        v_sincos(r1, s1, c1);

        अगर( mag )
        {
            v_float32 m0 = vx_load(mag + i);
            v_float32 m1 = vx_load(mag + i + VECSZ);
            c0 = v_mul(c0, m0);
            c1 = v_mul(c1, m1);
            s0 = v_mul(s0, m0);
            s1 = v_mul(s1, m1);
        }

        v_store(cosval + i, c0);
        v_store(cosval + i + VECSZ, c1);

        v_store(sinval + i, s0);
        v_store(sinval + i + VECSZ, s1);
    }
    vx_cleanup();
#पूर्ण_अगर

    क्रम( ; i < len; i++ )
    {
        द्विगुन t = angle[i]*k1;
        पूर्णांक it = cvRound(t);
        t -= it;
        पूर्णांक sin_idx = it & (N - 1);
        पूर्णांक cos_idx = (N/4 - sin_idx) & (N - 1);

        द्विगुन sin_b = (sin_a0*t*t + sin_a2)*t;
        द्विगुन cos_b = cos_a0*t*t + 1;

        द्विगुन sin_a = sin_table[sin_idx];
        द्विगुन cos_a = sin_table[cos_idx];

        द्विगुन sin_val = sin_a*cos_b + cos_a*sin_b;
        द्विगुन cos_val = cos_a*cos_b - sin_a*sin_b;

        अगर (mag)
        {
            द्विगुन mag_val = mag[i];
            sin_val *= mag_val;
            cos_val *= mag_val;
        }

        sinval[i] = (भग्न)sin_val;
        cosval[i] = (भग्न)cos_val;
    }
}

} // anonymous::

व्योम polarToCart32f(स्थिर भग्न *mag, स्थिर भग्न *angle, भग्न *X, भग्न *Y, पूर्णांक len, bool angleInDegrees)
{
    CV_INSTRUMENT_REGION();
    SinCos_32f(mag, angle, X, Y, len, angleInDegrees);
}

व्योम polarToCart64f(स्थिर द्विगुन *mag, स्थिर द्विगुन *angle, द्विगुन *X, द्विगुन *Y, पूर्णांक len, bool angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    स्थिर पूर्णांक BLKSZ = 128;
    भग्न ybuf[BLKSZ], xbuf[BLKSZ], _mbuf[BLKSZ], abuf[BLKSZ];
    भग्न* mbuf = mag ? _mbuf : nullptr;
    क्रम( पूर्णांक i = 0; i < len; i += BLKSZ )
    {
        पूर्णांक j, blksz = std::min(BLKSZ, len - i);
        क्रम( j = 0; j < blksz; j++ )
        {
            अगर (mbuf)
                mbuf[j] = (भग्न)mag[i + j];
            abuf[j] = (भग्न)angle[i + j];
        }
        SinCos_32f(mbuf, abuf, xbuf, ybuf, blksz, angleInDegrees);
        क्रम( j = 0; j < blksz; j++ )
            X[i + j] = xbuf[j];
        क्रम( j = 0; j < blksz; j++ )
            Y[i + j] = ybuf[j];
    }
}

अटल व्योम fastAtan32f_(स्थिर भग्न *Y, स्थिर भग्न *X, भग्न *angle, पूर्णांक len, bool angleInDegrees )
{
    भग्न scale = angleInDegrees ? 1.f : (भग्न)(CV_PI/180);
    पूर्णांक i = 0;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिर पूर्णांक VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    क्रम( ; i < len; i += VECSZ*2 )
    {
        अगर( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            अगर( i == 0 || angle == X || angle == Y )
                अवरोध;
            i = len - VECSZ*2;
        }

        v_float32 y0 = vx_load(Y + i);
        v_float32 x0 = vx_load(X + i);
        v_float32 y1 = vx_load(Y + i + VECSZ);
        v_float32 x1 = vx_load(X + i + VECSZ);

        v_float32 r0 = v_mul(v_atan_f32(y0, x0), s);
        v_float32 r1 = v_mul(v_atan_f32(y1, x1), s);

        v_store(angle + i, r0);
        v_store(angle + i + VECSZ, r1);
    }
    vx_cleanup();
#पूर्ण_अगर

    क्रम( ; i < len; i++ )
        angle[i] = atan_f32(Y[i], X[i])*scale;
}

व्योम fastAtan32f(स्थिर भग्न *Y, स्थिर भग्न *X, भग्न *angle, पूर्णांक len, bool angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f_(Y, X, angle, len, angleInDegrees );
}

व्योम fastAtan64f(स्थिर द्विगुन *Y, स्थिर द्विगुन *X, द्विगुन *angle, पूर्णांक len, bool angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    स्थिर पूर्णांक BLKSZ = 128;
    भग्न ybuf[BLKSZ], xbuf[BLKSZ], abuf[BLKSZ];
    क्रम( पूर्णांक i = 0; i < len; i += BLKSZ )
    {
        पूर्णांक j, blksz = std::min(BLKSZ, len - i);
        क्रम( j = 0; j < blksz; j++ )
        {
            ybuf[j] = (भग्न)Y[i + j];
            xbuf[j] = (भग्न)X[i + j];
        }
        fastAtan32f_(ybuf, xbuf, abuf, blksz, angleInDegrees);
        क्रम( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

// deprecated
व्योम fastAtan2(स्थिर भग्न *Y, स्थिर भग्न *X, भग्न *angle, पूर्णांक len, bool angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

व्योम magnitude32f(स्थिर भग्न* x, स्थिर भग्न* y, भग्न* mag, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक i = 0;

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिर पूर्णांक VECSZ = VTraits<v_float32>::vlanes();
    क्रम( ; i < len; i += VECSZ*2 )
    {
        अगर( i + VECSZ*2 > len )
        {
            अगर( i == 0 || mag == x || mag == y )
                अवरोध;
            i = len - VECSZ*2;
        }
        v_float32 x0 = vx_load(x + i), x1 = vx_load(x + i + VECSZ);
        v_float32 y0 = vx_load(y + i), y1 = vx_load(y + i + VECSZ);
        x0 = v_sqrt(v_muladd(x0, x0, v_mul(y0, y0)));
        x1 = v_sqrt(v_muladd(x1, x1, v_mul(y1, y1)));
        v_store(mag + i, x0);
        v_store(mag + i + VECSZ, x1);
    }
    vx_cleanup();
#पूर्ण_अगर

    क्रम( ; i < len; i++ )
    {
        भग्न x0 = x[i], y0 = y[i];
        mag[i] = std::वर्ग_मूल(x0*x0 + y0*y0);
    }
}

व्योम magnitude64f(स्थिर द्विगुन* x, स्थिर द्विगुन* y, द्विगुन* mag, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक i = 0;

#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    स्थिर पूर्णांक VECSZ = VTraits<v_float64>::vlanes();
    क्रम( ; i < len; i += VECSZ*2 )
    {
        अगर( i + VECSZ*2 > len )
        {
            अगर( i == 0 || mag == x || mag == y )
                अवरोध;
            i = len - VECSZ*2;
        }
        v_float64 x0 = vx_load(x + i), x1 = vx_load(x + i + VECSZ);
        v_float64 y0 = vx_load(y + i), y1 = vx_load(y + i + VECSZ);
        x0 = v_sqrt(v_muladd(x0, x0, v_mul(y0, y0)));
        x1 = v_sqrt(v_muladd(x1, x1, v_mul(y1, y1)));
        v_store(mag + i, x0);
        v_store(mag + i + VECSZ, x1);
    }
    vx_cleanup();
#पूर्ण_अगर

    क्रम( ; i < len; i++ )
    {
        द्विगुन x0 = x[i], y0 = y[i];
        mag[i] = std::वर्ग_मूल(x0*x0 + y0*y0);
    }
}


व्योम invSqrt32f(स्थिर भग्न* src, भग्न* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक i = 0;

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिर पूर्णांक VECSZ = VTraits<v_float32>::vlanes();
    क्रम( ; i < len; i += VECSZ*2 )
    {
        अगर( i + VECSZ*2 > len )
        {
            अगर( i == 0 || src == dst )
                अवरोध;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#पूर्ण_अगर

    क्रम( ; i < len; i++ )
        dst[i] = 1/std::वर्ग_मूल(src[i]);
}


व्योम invSqrt64f(स्थिर द्विगुन* src, द्विगुन* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    पूर्णांक i = 0;

#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    स्थिर पूर्णांक VECSZ = VTraits<v_float64>::vlanes();
    क्रम ( ; i < len; i += VECSZ*2)
    {
        अगर( i + VECSZ*2 > len )
        {
            अगर( i == 0 || src == dst )
                अवरोध;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
#पूर्ण_अगर

    क्रम( ; i < len; i++ )
        dst[i] = 1/std::वर्ग_मूल(src[i]);
}


व्योम sqrt32f(स्थिर भग्न* src, भग्न* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक i = 0;

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिर पूर्णांक VECSZ = VTraits<v_float32>::vlanes();
    क्रम( ; i < len; i += VECSZ*2 )
    {
        अगर( i + VECSZ*2 > len )
        {
            अगर( i == 0 || src == dst )
                अवरोध;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#पूर्ण_अगर

    क्रम( ; i < len; i++ )
        dst[i] = std::वर्ग_मूल(src[i]);
}


व्योम sqrt64f(स्थिर द्विगुन* src, द्विगुन* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक i = 0;

#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    स्थिर पूर्णांक VECSZ = VTraits<v_float64>::vlanes();
    क्रम( ; i < len; i += VECSZ*2 )
    {
        अगर( i + VECSZ*2 > len )
        {
            अगर( i == 0 || src == dst )
                अवरोध;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#पूर्ण_अगर

    क्रम( ; i < len; i++ )
        dst[i] = std::वर्ग_मूल(src[i]);
}

// Workaround for ICE in MSVS 2015 update 3 (issue #7795)
// CV_AVX is not used here, because generated code is faster in non-AVX mode.
// (tested with disabled IPP on i5-6300U)
#अगर (defined _MSC_VER && _MSC_VER >= 1900 && (defined(_M_IX86) || defined(_M_X64))) || defined(__EMSCRIPTEN__)
व्योम exp32f(स्थिर भग्न *src, भग्न *dst, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    क्रम (पूर्णांक i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

व्योम exp64f(स्थिर द्विगुन *src, द्विगुन *dst, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    क्रम (पूर्णांक i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

व्योम log32f(स्थिर भग्न *src, भग्न *dst, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    क्रम (पूर्णांक i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
व्योम log64f(स्थिर द्विगुन *src, द्विगुन *dst, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    क्रम (पूर्णांक i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
#अन्यथा

////////////////////////////////////// EXP /////////////////////////////////////


#पूर्ण_अगर // issue 7795

भग्न fastAtan2( भग्न y, भग्न x )
{
    वापस atan_f32(y, x);
}

#पूर्ण_अगर // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

}} // namespace cv::hal
