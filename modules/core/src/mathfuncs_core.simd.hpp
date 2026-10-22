// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#அந்தர்பாவ "mathfuncs.hpp"

நாமாகாஶ cv { நாமாகாஶ hal {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
வெறுமை cartToPolar32f(மாறிலி தசம *X, மாறிலி தசம *Y, தசம* mag, தசம *angle, முழு len, தருக்க angleInDegrees);
வெறுமை cartToPolar64f(மாறிலி இரட்டை *X, மாறிலி இரட்டை *Y, இரட்டை* mag, இரட்டை *angle, முழு len, தருக்க angleInDegrees);
வெறுமை polarToCart32f(மாறிலி தசம *mag, மாறிலி தசம *angle, தசம *X, தசம *Y, முழு len, தருக்க angleInDegrees);
வெறுமை polarToCart64f(மாறிலி இரட்டை *mag, மாறிலி இரட்டை *angle, இரட்டை *X, இரட்டை *Y, முழு len, தருக்க angleInDegrees);
வெறுமை fastAtan32f(மாறிலி தசம *Y, மாறிலி தசம *X, தசம *angle, முழு len, தருக்க angleInDegrees);
வெறுமை fastAtan64f(மாறிலி இரட்டை *Y, மாறிலி இரட்டை *X, இரட்டை *angle, முழு len, தருக்க angleInDegrees);
வெறுமை fastAtan2(மாறிலி தசம *Y, மாறிலி தசம *X, தசம *angle, முழு len, தருக்க angleInDegrees);
வெறுமை magnitude32f(மாறிலி தசம* x, மாறிலி தசம* y, தசம* mag, முழு len);
வெறுமை magnitude64f(மாறிலி இரட்டை* x, மாறிலி இரட்டை* y, இரட்டை* mag, முழு len);
வெறுமை invSqrt32f(மாறிலி தசம* src, தசம* dst, முழு len);
வெறுமை invSqrt64f(மாறிலி இரட்டை* src, இரட்டை* dst, முழு len);
வெறுமை sqrt32f(மாறிலி தசம* src, தசம* dst, முழு len);
வெறுமை sqrt64f(மாறிலி இரட்டை* src, இரட்டை* dst, முழு len);
தசம fastAtan2(தசம y, தசம x);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

using நாமாகாஶ std;
using நாமாகாஶ cv;

நாமாகாஶ {

நிலையான மாறிலி தசம atan2_p1 = 0.9997878412794807f*(தசம)(180/CV_PI);
நிலையான மாறிலி தசம atan2_p3 = -0.3258083974640975f*(தசம)(180/CV_PI);
நிலையான மாறிலி தசம atan2_p5 = 0.1555786518463281f*(தசம)(180/CV_PI);
நிலையான மாறிலி தசம atan2_p7 = -0.04432655554792128f*(தசம)(180/CV_PI);

#ifdef __EMSCRIPTEN__
நிலையான உள்வரி தசம atan_f32(தசம y, தசம x)
{
    தசம a = atan2(y, x) * 180.0f / CV_PI;
    எனில் (a < 0.0f)
        a += 360.0f;
    எனில் (a >= 360.0f)
        a -= 360.0f;
    திரும்பு a; // range [0; 360)
}
#இல்லையெனில்
நிலையான உள்வரி தசம atan_f32(தசம y, தசம x)
{
    தசம ax = std::abs(x), ay = std::abs(y);
    தசம a, c, c2;
    எனில்( ax >= ay )
    {
        c = ay/(ax + (தசம)DBL_EPSILON);
        c2 = c*c;
        a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    இல்லையெனில்
    {
        c = ax/(ay + (தசம)DBL_EPSILON);
        c2 = c*c;
        a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    எனில்( x < 0 )
        a = 180.f - a;
    எனில்( y < 0 )
        a = 360.f - a;
    திரும்பு a;
}
#endif

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)

v_float32 v_atan_f32(மாறிலி v_float32& y, மாறிலி v_float32& x)
{
    v_float32 eps = vx_setall_f32((தசம)DBL_EPSILON);
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
    திரும்பு a;
}

#endif

} // anonymous::

நிலையான வெறுமை cartToPolar32f_(மாறிலி தசம *X, மாறிலி தசம *Y, தசம *mag, தசம *angle, முழு len, தருக்க angleInDegrees )
{
    தசம scale = angleInDegrees ? 1.f : (தசம)(CV_PI/180);
    முழு i = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    மாறிலி முழு VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    க்கு( ; i < len; i += VECSZ*2 )
    {
        எனில்( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            எனில்( i == 0 || angle == X || angle == Y )
                நிறுத்து;
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

    க்கு( ; i < len; i++ )
    {
        தசம x0 = X[i], y0 = Y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
        angle[i] = atan_f32(y0, x0)*scale;
    }
}

வெறுமை cartToPolar32f(மாறிலி தசம *X, மாறிலி தசம *Y, தசம *mag, தசம *angle, முழு len, தருக்க angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    cartToPolar32f_(X, Y, mag, angle, len, angleInDegrees );
}

வெறுமை cartToPolar64f(மாறிலி இரட்டை *X, மாறிலி இரட்டை *Y, இரட்டை *mag, இரட்டை *angle, முழு len, தருக்க angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    மாறிலி முழு BLKSZ = 128;
    தசம ybuf[BLKSZ], xbuf[BLKSZ], mbuf[BLKSZ], abuf[BLKSZ];
    க்கு( முழு i = 0; i < len; i += BLKSZ )
    {
        முழு j, blksz = std::min(BLKSZ, len - i);
        க்கு( j = 0; j < blksz; j++ )
        {
            xbuf[j] = (தசம)X[i + j];
            ybuf[j] = (தசம)Y[i + j];
        }
        cartToPolar32f_(xbuf, ybuf, mbuf, abuf, blksz, angleInDegrees);
        க்கு( j = 0; j < blksz; j++ )
            mag[i + j] = mbuf[j];
        க்கு( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

நாமாகாஶ {

நிலையான உள்வரி வெறுமை SinCos_32f(மாறிலி தசம* mag, மாறிலி தசம* angle, தசம* cosval, தசம* sinval, முழு len, முழு angle_in_degrees)
{
    மாறிலி முழு N = 64;

    நிலையான மாறிலி இரட்டை sin_table[] =
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

    நிலையான மாறிலி இரட்டை k2 = (2*CV_PI)/N;

    நிலையான மாறிலி இரட்டை sin_a0 = -0.166630293345647*k2*k2*k2;
    நிலையான மாறிலி இரட்டை sin_a2 = k2;

    நிலையான மாறிலி இரட்டை cos_a0 = -0.499818138450326*k2*k2;
    /*static const double cos_a2 =  1;*/

    இரட்டை k1;
    முழு i = 0;

    எனில்( !angle_in_degrees )
        k1 = N/(2*CV_PI);
    இல்லையெனில்
        k1 = N/360.;

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    மாறிலி முழு VECSZ = VTraits<v_float32>::vlanes();
    மாறிலி v_float32 scale = vx_setall_f32(angle_in_degrees ? (தசம)CV_PI / 180.f : 1.f);

    க்கு( ; i < len; i += VECSZ*2 )
    {
        எனில்( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            எனில்( i == 0 || angle == cosval || angle == sinval || mag == cosval || mag == sinval )
                நிறுத்து;
            i = len - VECSZ*2;
        }

        v_float32 r0 = v_mul(vx_load(angle + i), scale);
        v_float32 r1 = v_mul(vx_load(angle + i + VECSZ), scale);

        v_float32 c0, c1, s0, s1;
        v_sincos(r0, s0, c0);
        v_sincos(r1, s1, c1);

        எனில்( mag )
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

    க்கு( ; i < len; i++ )
    {
        இரட்டை t = angle[i]*k1;
        முழு it = cvRound(t);
        t -= it;
        முழு sin_idx = it & (N - 1);
        முழு cos_idx = (N/4 - sin_idx) & (N - 1);

        இரட்டை sin_b = (sin_a0*t*t + sin_a2)*t;
        இரட்டை cos_b = cos_a0*t*t + 1;

        இரட்டை sin_a = sin_table[sin_idx];
        இரட்டை cos_a = sin_table[cos_idx];

        இரட்டை sin_val = sin_a*cos_b + cos_a*sin_b;
        இரட்டை cos_val = cos_a*cos_b - sin_a*sin_b;

        எனில் (mag)
        {
            இரட்டை mag_val = mag[i];
            sin_val *= mag_val;
            cos_val *= mag_val;
        }

        sinval[i] = (தசம)sin_val;
        cosval[i] = (தசம)cos_val;
    }
}

} // anonymous::

வெறுமை polarToCart32f(மாறிலி தசம *mag, மாறிலி தசம *angle, தசம *X, தசம *Y, முழு len, தருக்க angleInDegrees)
{
    CV_INSTRUMENT_REGION();
    SinCos_32f(mag, angle, X, Y, len, angleInDegrees);
}

வெறுமை polarToCart64f(மாறிலி இரட்டை *mag, மாறிலி இரட்டை *angle, இரட்டை *X, இரட்டை *Y, முழு len, தருக்க angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    மாறிலி முழு BLKSZ = 128;
    தசம ybuf[BLKSZ], xbuf[BLKSZ], _mbuf[BLKSZ], abuf[BLKSZ];
    தசம* mbuf = mag ? _mbuf : nullptr;
    க்கு( முழு i = 0; i < len; i += BLKSZ )
    {
        முழு j, blksz = std::min(BLKSZ, len - i);
        க்கு( j = 0; j < blksz; j++ )
        {
            எனில் (mbuf)
                mbuf[j] = (தசம)mag[i + j];
            abuf[j] = (தசம)angle[i + j];
        }
        SinCos_32f(mbuf, abuf, xbuf, ybuf, blksz, angleInDegrees);
        க்கு( j = 0; j < blksz; j++ )
            X[i + j] = xbuf[j];
        க்கு( j = 0; j < blksz; j++ )
            Y[i + j] = ybuf[j];
    }
}

நிலையான வெறுமை fastAtan32f_(மாறிலி தசம *Y, மாறிலி தசம *X, தசம *angle, முழு len, தருக்க angleInDegrees )
{
    தசம scale = angleInDegrees ? 1.f : (தசம)(CV_PI/180);
    முழு i = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    மாறிலி முழு VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    க்கு( ; i < len; i += VECSZ*2 )
    {
        எனில்( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            எனில்( i == 0 || angle == X || angle == Y )
                நிறுத்து;
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

    க்கு( ; i < len; i++ )
        angle[i] = atan_f32(Y[i], X[i])*scale;
}

வெறுமை fastAtan32f(மாறிலி தசம *Y, மாறிலி தசம *X, தசம *angle, முழு len, தருக்க angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f_(Y, X, angle, len, angleInDegrees );
}

வெறுமை fastAtan64f(மாறிலி இரட்டை *Y, மாறிலி இரட்டை *X, இரட்டை *angle, முழு len, தருக்க angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    மாறிலி முழு BLKSZ = 128;
    தசம ybuf[BLKSZ], xbuf[BLKSZ], abuf[BLKSZ];
    க்கு( முழு i = 0; i < len; i += BLKSZ )
    {
        முழு j, blksz = std::min(BLKSZ, len - i);
        க்கு( j = 0; j < blksz; j++ )
        {
            ybuf[j] = (தசம)Y[i + j];
            xbuf[j] = (தசம)X[i + j];
        }
        fastAtan32f_(ybuf, xbuf, abuf, blksz, angleInDegrees);
        க்கு( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

// deprecated
வெறுமை fastAtan2(மாறிலி தசம *Y, மாறிலி தசம *X, தசம *angle, முழு len, தருக்க angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

வெறுமை magnitude32f(மாறிலி தசம* x, மாறிலி தசம* y, தசம* mag, முழு len)
{
    CV_INSTRUMENT_REGION();

    முழு i = 0;

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    மாறிலி முழு VECSZ = VTraits<v_float32>::vlanes();
    க்கு( ; i < len; i += VECSZ*2 )
    {
        எனில்( i + VECSZ*2 > len )
        {
            எனில்( i == 0 || mag == x || mag == y )
                நிறுத்து;
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

    க்கு( ; i < len; i++ )
    {
        தசம x0 = x[i], y0 = y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
    }
}

வெறுமை magnitude64f(மாறிலி இரட்டை* x, மாறிலி இரட்டை* y, இரட்டை* mag, முழு len)
{
    CV_INSTRUMENT_REGION();

    முழு i = 0;

#எனில் (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    மாறிலி முழு VECSZ = VTraits<v_float64>::vlanes();
    க்கு( ; i < len; i += VECSZ*2 )
    {
        எனில்( i + VECSZ*2 > len )
        {
            எனில்( i == 0 || mag == x || mag == y )
                நிறுத்து;
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

    க்கு( ; i < len; i++ )
    {
        இரட்டை x0 = x[i], y0 = y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
    }
}


வெறுமை invSqrt32f(மாறிலி தசம* src, தசம* dst, முழு len)
{
    CV_INSTRUMENT_REGION();

    முழு i = 0;

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    மாறிலி முழு VECSZ = VTraits<v_float32>::vlanes();
    க்கு( ; i < len; i += VECSZ*2 )
    {
        எனில்( i + VECSZ*2 > len )
        {
            எனில்( i == 0 || src == dst )
                நிறுத்து;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    க்கு( ; i < len; i++ )
        dst[i] = 1/std::sqrt(src[i]);
}


வெறுமை invSqrt64f(மாறிலி இரட்டை* src, இரட்டை* dst, முழு len)
{
    CV_INSTRUMENT_REGION();
    முழு i = 0;

#எனில் (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    மாறிலி முழு VECSZ = VTraits<v_float64>::vlanes();
    க்கு ( ; i < len; i += VECSZ*2)
    {
        எனில்( i + VECSZ*2 > len )
        {
            எனில்( i == 0 || src == dst )
                நிறுத்து;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
#endif

    க்கு( ; i < len; i++ )
        dst[i] = 1/std::sqrt(src[i]);
}


வெறுமை sqrt32f(மாறிலி தசம* src, தசம* dst, முழு len)
{
    CV_INSTRUMENT_REGION();

    முழு i = 0;

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    மாறிலி முழு VECSZ = VTraits<v_float32>::vlanes();
    க்கு( ; i < len; i += VECSZ*2 )
    {
        எனில்( i + VECSZ*2 > len )
        {
            எனில்( i == 0 || src == dst )
                நிறுத்து;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    க்கு( ; i < len; i++ )
        dst[i] = std::sqrt(src[i]);
}


வெறுமை sqrt64f(மாறிலி இரட்டை* src, இரட்டை* dst, முழு len)
{
    CV_INSTRUMENT_REGION();

    முழு i = 0;

#எனில் (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    மாறிலி முழு VECSZ = VTraits<v_float64>::vlanes();
    க்கு( ; i < len; i += VECSZ*2 )
    {
        எனில்( i + VECSZ*2 > len )
        {
            எனில்( i == 0 || src == dst )
                நிறுத்து;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    க்கு( ; i < len; i++ )
        dst[i] = std::sqrt(src[i]);
}

// Workaround for ICE in MSVS 2015 update 3 (issue #7795)
// CV_AVX is not used here, because generated code is faster in non-AVX mode.
// (tested with disabled IPP on i5-6300U)
#எனில் (defined _MSC_VER && _MSC_VER >= 1900 && (defined(_M_IX86) || defined(_M_X64))) || defined(__EMSCRIPTEN__)
வெறுமை exp32f(மாறிலி தசம *src, தசம *dst, முழு n)
{
    CV_INSTRUMENT_REGION();

    க்கு (முழு i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

வெறுமை exp64f(மாறிலி இரட்டை *src, இரட்டை *dst, முழு n)
{
    CV_INSTRUMENT_REGION();

    க்கு (முழு i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

வெறுமை log32f(மாறிலி தசம *src, தசம *dst, முழு n)
{
    CV_INSTRUMENT_REGION();

    க்கு (முழு i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
வெறுமை log64f(மாறிலி இரட்டை *src, இரட்டை *dst, முழு n)
{
    CV_INSTRUMENT_REGION();

    க்கு (முழு i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
#இல்லையெனில்

////////////////////////////////////// EXP /////////////////////////////////////


#endif // issue 7795

தசம fastAtan2( தசம y, தசம x )
{
    திரும்பு atan_f32(y, x);
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

}} // namespace cv::hal
