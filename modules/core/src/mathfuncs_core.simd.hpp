// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "mathfuncs.hpp"

فضاینام cv { فضاینام hal {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
تهی cartToPolar32f(ثابت اعشاری *X, ثابت اعشاری *Y, اعشاری* mag, اعشاری *angle, صحیح len, منطقی angleInDegrees);
تهی cartToPolar64f(ثابت دوگانه *X, ثابت دوگانه *Y, دوگانه* mag, دوگانه *angle, صحیح len, منطقی angleInDegrees);
تهی polarToCart32f(ثابت اعشاری *mag, ثابت اعشاری *angle, اعشاری *X, اعشاری *Y, صحیح len, منطقی angleInDegrees);
تهی polarToCart64f(ثابت دوگانه *mag, ثابت دوگانه *angle, دوگانه *X, دوگانه *Y, صحیح len, منطقی angleInDegrees);
تهی fastAtan32f(ثابت اعشاری *Y, ثابت اعشاری *X, اعشاری *angle, صحیح len, منطقی angleInDegrees);
تهی fastAtan64f(ثابت دوگانه *Y, ثابت دوگانه *X, دوگانه *angle, صحیح len, منطقی angleInDegrees);
تهی fastAtan2(ثابت اعشاری *Y, ثابت اعشاری *X, اعشاری *angle, صحیح len, منطقی angleInDegrees);
تهی magnitude32f(ثابت اعشاری* x, ثابت اعشاری* y, اعشاری* mag, صحیح len);
تهی magnitude64f(ثابت دوگانه* x, ثابت دوگانه* y, دوگانه* mag, صحیح len);
تهی invSqrt32f(ثابت اعشاری* src, اعشاری* dst, صحیح len);
تهی invSqrt64f(ثابت دوگانه* src, دوگانه* dst, صحیح len);
تهی sqrt32f(ثابت اعشاری* src, اعشاری* dst, صحیح len);
تهی sqrt64f(ثابت دوگانه* src, دوگانه* dst, صحیح len);
اعشاری fastAtan2(اعشاری y, اعشاری x);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

using فضاینام std;
using فضاینام cv;

فضاینام {

ایستا ثابت اعشاری atan2_p1 = 0.9997878412794807f*(اعشاری)(180/CV_PI);
ایستا ثابت اعشاری atan2_p3 = -0.3258083974640975f*(اعشاری)(180/CV_PI);
ایستا ثابت اعشاری atan2_p5 = 0.1555786518463281f*(اعشاری)(180/CV_PI);
ایستا ثابت اعشاری atan2_p7 = -0.04432655554792128f*(اعشاری)(180/CV_PI);

#ifdef __EMSCRIPTEN__
ایستا درونخطی اعشاری atan_f32(اعشاری y, اعشاری x)
{
    اعشاری a = atan2(y, x) * 180.0f / CV_PI;
    اگر (a < 0.0f)
        a += 360.0f;
    اگر (a >= 360.0f)
        a -= 360.0f;
    بازگشت a; // range [0; 360)
}
#وگرنه
ایستا درونخطی اعشاری atan_f32(اعشاری y, اعشاری x)
{
    اعشاری ax = std::abs(x), ay = std::abs(y);
    اعشاری a, c, c2;
    اگر( ax >= ay )
    {
        c = ay/(ax + (اعشاری)DBL_EPSILON);
        c2 = c*c;
        a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    وگرنه
    {
        c = ax/(ay + (اعشاری)DBL_EPSILON);
        c2 = c*c;
        a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    اگر( x < 0 )
        a = 180.f - a;
    اگر( y < 0 )
        a = 360.f - a;
    بازگشت a;
}
#endif

#اگر (CV_SIMD || CV_SIMD_SCALABLE)

v_float32 v_atan_f32(ثابت v_float32& y, ثابت v_float32& x)
{
    v_float32 eps = vx_setall_f32((اعشاری)DBL_EPSILON);
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
    بازگشت a;
}

#endif

} // anonymous::

ایستا تهی cartToPolar32f_(ثابت اعشاری *X, ثابت اعشاری *Y, اعشاری *mag, اعشاری *angle, صحیح len, منطقی angleInDegrees )
{
    اعشاری scale = angleInDegrees ? 1.f : (اعشاری)(CV_PI/180);
    صحیح i = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    برای( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            اگر( i == 0 || angle == X || angle == Y )
                بشکن;
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
#endif

    برای( ; i < len; i++ )
    {
        اعشاری x0 = X[i], y0 = Y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
        angle[i] = atan_f32(y0, x0)*scale;
    }
}

تهی cartToPolar32f(ثابت اعشاری *X, ثابت اعشاری *Y, اعشاری *mag, اعشاری *angle, صحیح len, منطقی angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    cartToPolar32f_(X, Y, mag, angle, len, angleInDegrees );
}

تهی cartToPolar64f(ثابت دوگانه *X, ثابت دوگانه *Y, دوگانه *mag, دوگانه *angle, صحیح len, منطقی angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح BLKSZ = 128;
    اعشاری ybuf[BLKSZ], xbuf[BLKSZ], mbuf[BLKSZ], abuf[BLKSZ];
    برای( صحیح i = 0; i < len; i += BLKSZ )
    {
        صحیح j, blksz = std::min(BLKSZ, len - i);
        برای( j = 0; j < blksz; j++ )
        {
            xbuf[j] = (اعشاری)X[i + j];
            ybuf[j] = (اعشاری)Y[i + j];
        }
        cartToPolar32f_(xbuf, ybuf, mbuf, abuf, blksz, angleInDegrees);
        برای( j = 0; j < blksz; j++ )
            mag[i + j] = mbuf[j];
        برای( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

فضاینام {

ایستا درونخطی تهی SinCos_32f(ثابت اعشاری* mag, ثابت اعشاری* angle, اعشاری* cosval, اعشاری* sinval, صحیح len, صحیح angle_in_degrees)
{
    ثابت صحیح N = 64;

    ایستا ثابت دوگانه sin_table[] =
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

    ایستا ثابت دوگانه k2 = (2*CV_PI)/N;

    ایستا ثابت دوگانه sin_a0 = -0.166630293345647*k2*k2*k2;
    ایستا ثابت دوگانه sin_a2 = k2;

    ایستا ثابت دوگانه cos_a0 = -0.499818138450326*k2*k2;
    /*static const double cos_a2 =  1;*/

    دوگانه k1;
    صحیح i = 0;

    اگر( !angle_in_degrees )
        k1 = N/(2*CV_PI);
    وگرنه
        k1 = N/360.;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    ثابت v_float32 scale = vx_setall_f32(angle_in_degrees ? (اعشاری)CV_PI / 180.f : 1.f);

    برای( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            اگر( i == 0 || angle == cosval || angle == sinval || mag == cosval || mag == sinval )
                بشکن;
            i = len - VECSZ*2;
        }

        v_float32 r0 = v_mul(vx_load(angle + i), scale);
        v_float32 r1 = v_mul(vx_load(angle + i + VECSZ), scale);

        v_float32 c0, c1, s0, s1;
        v_sincos(r0, s0, c0);
        v_sincos(r1, s1, c1);

        اگر( mag )
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
#endif

    برای( ; i < len; i++ )
    {
        دوگانه t = angle[i]*k1;
        صحیح it = cvRound(t);
        t -= it;
        صحیح sin_idx = it & (N - 1);
        صحیح cos_idx = (N/4 - sin_idx) & (N - 1);

        دوگانه sin_b = (sin_a0*t*t + sin_a2)*t;
        دوگانه cos_b = cos_a0*t*t + 1;

        دوگانه sin_a = sin_table[sin_idx];
        دوگانه cos_a = sin_table[cos_idx];

        دوگانه sin_val = sin_a*cos_b + cos_a*sin_b;
        دوگانه cos_val = cos_a*cos_b - sin_a*sin_b;

        اگر (mag)
        {
            دوگانه mag_val = mag[i];
            sin_val *= mag_val;
            cos_val *= mag_val;
        }

        sinval[i] = (اعشاری)sin_val;
        cosval[i] = (اعشاری)cos_val;
    }
}

} // anonymous::

تهی polarToCart32f(ثابت اعشاری *mag, ثابت اعشاری *angle, اعشاری *X, اعشاری *Y, صحیح len, منطقی angleInDegrees)
{
    CV_INSTRUMENT_REGION();
    SinCos_32f(mag, angle, X, Y, len, angleInDegrees);
}

تهی polarToCart64f(ثابت دوگانه *mag, ثابت دوگانه *angle, دوگانه *X, دوگانه *Y, صحیح len, منطقی angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح BLKSZ = 128;
    اعشاری ybuf[BLKSZ], xbuf[BLKSZ], _mbuf[BLKSZ], abuf[BLKSZ];
    اعشاری* mbuf = mag ? _mbuf : nullptr;
    برای( صحیح i = 0; i < len; i += BLKSZ )
    {
        صحیح j, blksz = std::min(BLKSZ, len - i);
        برای( j = 0; j < blksz; j++ )
        {
            اگر (mbuf)
                mbuf[j] = (اعشاری)mag[i + j];
            abuf[j] = (اعشاری)angle[i + j];
        }
        SinCos_32f(mbuf, abuf, xbuf, ybuf, blksz, angleInDegrees);
        برای( j = 0; j < blksz; j++ )
            X[i + j] = xbuf[j];
        برای( j = 0; j < blksz; j++ )
            Y[i + j] = ybuf[j];
    }
}

ایستا تهی fastAtan32f_(ثابت اعشاری *Y, ثابت اعشاری *X, اعشاری *angle, صحیح len, منطقی angleInDegrees )
{
    اعشاری scale = angleInDegrees ? 1.f : (اعشاری)(CV_PI/180);
    صحیح i = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    برای( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            اگر( i == 0 || angle == X || angle == Y )
                بشکن;
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
#endif

    برای( ; i < len; i++ )
        angle[i] = atan_f32(Y[i], X[i])*scale;
}

تهی fastAtan32f(ثابت اعشاری *Y, ثابت اعشاری *X, اعشاری *angle, صحیح len, منطقی angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f_(Y, X, angle, len, angleInDegrees );
}

تهی fastAtan64f(ثابت دوگانه *Y, ثابت دوگانه *X, دوگانه *angle, صحیح len, منطقی angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح BLKSZ = 128;
    اعشاری ybuf[BLKSZ], xbuf[BLKSZ], abuf[BLKSZ];
    برای( صحیح i = 0; i < len; i += BLKSZ )
    {
        صحیح j, blksz = std::min(BLKSZ, len - i);
        برای( j = 0; j < blksz; j++ )
        {
            ybuf[j] = (اعشاری)Y[i + j];
            xbuf[j] = (اعشاری)X[i + j];
        }
        fastAtan32f_(ybuf, xbuf, abuf, blksz, angleInDegrees);
        برای( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

// deprecated
تهی fastAtan2(ثابت اعشاری *Y, ثابت اعشاری *X, اعشاری *angle, صحیح len, منطقی angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

تهی magnitude32f(ثابت اعشاری* x, ثابت اعشاری* y, اعشاری* mag, صحیح len)
{
    CV_INSTRUMENT_REGION();

    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    برای( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            اگر( i == 0 || mag == x || mag == y )
                بشکن;
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
#endif

    برای( ; i < len; i++ )
    {
        اعشاری x0 = x[i], y0 = y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
    }
}

تهی magnitude64f(ثابت دوگانه* x, ثابت دوگانه* y, دوگانه* mag, صحیح len)
{
    CV_INSTRUMENT_REGION();

    صحیح i = 0;

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ثابت صحیح VECSZ = VTraits<v_float64>::vlanes();
    برای( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            اگر( i == 0 || mag == x || mag == y )
                بشکن;
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
#endif

    برای( ; i < len; i++ )
    {
        دوگانه x0 = x[i], y0 = y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
    }
}


تهی invSqrt32f(ثابت اعشاری* src, اعشاری* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();

    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    برای( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            اگر( i == 0 || src == dst )
                بشکن;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    برای( ; i < len; i++ )
        dst[i] = 1/std::sqrt(src[i]);
}


تهی invSqrt64f(ثابت دوگانه* src, دوگانه* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();
    صحیح i = 0;

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ثابت صحیح VECSZ = VTraits<v_float64>::vlanes();
    برای ( ; i < len; i += VECSZ*2)
    {
        اگر( i + VECSZ*2 > len )
        {
            اگر( i == 0 || src == dst )
                بشکن;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
#endif

    برای( ; i < len; i++ )
        dst[i] = 1/std::sqrt(src[i]);
}


تهی sqrt32f(ثابت اعشاری* src, اعشاری* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();

    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    برای( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            اگر( i == 0 || src == dst )
                بشکن;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    برای( ; i < len; i++ )
        dst[i] = std::sqrt(src[i]);
}


تهی sqrt64f(ثابت دوگانه* src, دوگانه* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();

    صحیح i = 0;

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ثابت صحیح VECSZ = VTraits<v_float64>::vlanes();
    برای( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            اگر( i == 0 || src == dst )
                بشکن;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    برای( ; i < len; i++ )
        dst[i] = std::sqrt(src[i]);
}

// Workaround for ICE in MSVS 2015 update 3 (issue #7795)
// CV_AVX is not used here, because generated code is faster in non-AVX mode.
// (tested with disabled IPP on i5-6300U)
#اگر (defined _MSC_VER && _MSC_VER >= 1900 && (defined(_M_IX86) || defined(_M_X64))) || defined(__EMSCRIPTEN__)
تهی exp32f(ثابت اعشاری *src, اعشاری *dst, صحیح n)
{
    CV_INSTRUMENT_REGION();

    برای (صحیح i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

تهی exp64f(ثابت دوگانه *src, دوگانه *dst, صحیح n)
{
    CV_INSTRUMENT_REGION();

    برای (صحیح i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

تهی log32f(ثابت اعشاری *src, اعشاری *dst, صحیح n)
{
    CV_INSTRUMENT_REGION();

    برای (صحیح i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
تهی log64f(ثابت دوگانه *src, دوگانه *dst, صحیح n)
{
    CV_INSTRUMENT_REGION();

    برای (صحیح i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
#وگرنه

////////////////////////////////////// EXP /////////////////////////////////////


#endif // issue 7795

اعشاری fastAtan2( اعشاری y, اعشاری x )
{
    بازگشت atan_f32(y, x);
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

}} // namespace cv::hal
