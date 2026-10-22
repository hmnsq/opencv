// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "mathfuncs.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
ಶೂನ್ಯ cartToPolar32f(ಧ್ರುವ ಭಿನ್ನ *X, ಧ್ರುವ ಭಿನ್ನ *Y, ಭಿನ್ನ* mag, ಭಿನ್ನ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees);
ಶೂನ್ಯ cartToPolar64f(ಧ್ರುವ ದ್ವಿಗುಣ *X, ಧ್ರುವ ದ್ವಿಗುಣ *Y, ದ್ವಿಗುಣ* mag, ದ್ವಿಗುಣ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees);
ಶೂನ್ಯ polarToCart32f(ಧ್ರುವ ಭಿನ್ನ *mag, ಧ್ರುವ ಭಿನ್ನ *angle, ಭಿನ್ನ *X, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees);
ಶೂನ್ಯ polarToCart64f(ಧ್ರುವ ದ್ವಿಗುಣ *mag, ಧ್ರುವ ದ್ವಿಗುಣ *angle, ದ್ವಿಗುಣ *X, ದ್ವಿಗುಣ *Y, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees);
ಶೂನ್ಯ fastAtan32f(ಧ್ರುವ ಭಿನ್ನ *Y, ಧ್ರುವ ಭಿನ್ನ *X, ಭಿನ್ನ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees);
ಶೂನ್ಯ fastAtan64f(ಧ್ರುವ ದ್ವಿಗುಣ *Y, ಧ್ರುವ ದ್ವಿಗುಣ *X, ದ್ವಿಗುಣ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees);
ಶೂನ್ಯ fastAtan2(ಧ್ರುವ ಭಿನ್ನ *Y, ಧ್ರುವ ಭಿನ್ನ *X, ಭಿನ್ನ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees);
ಶೂನ್ಯ magnitude32f(ಧ್ರುವ ಭಿನ್ನ* x, ಧ್ರುವ ಭಿನ್ನ* y, ಭಿನ್ನ* mag, ಪೂರ್ಣಾಙ್ಕ len);
ಶೂನ್ಯ magnitude64f(ಧ್ರುವ ದ್ವಿಗುಣ* x, ಧ್ರುವ ದ್ವಿಗುಣ* y, ದ್ವಿಗುಣ* mag, ಪೂರ್ಣಾಙ್ಕ len);
ಶೂನ್ಯ invSqrt32f(ಧ್ರುವ ಭಿನ್ನ* src, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ len);
ಶೂನ್ಯ invSqrt64f(ಧ್ರುವ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ len);
ಶೂನ್ಯ sqrt32f(ಧ್ರುವ ಭಿನ್ನ* src, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ len);
ಶೂನ್ಯ sqrt64f(ಧ್ರುವ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ len);
ಭಿನ್ನ fastAtan2(ಭಿನ್ನ y, ಭಿನ್ನ x);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

using ನಾಮಾಕಾಶ std;
using ನಾಮಾಕಾಶ cv;

ನಾಮಾಕಾಶ {

ಸ್ಥಿರ ಧ್ರುವ ಭಿನ್ನ atan2_p1 = 0.9997878412794807f*(ಭಿನ್ನ)(180/CV_PI);
ಸ್ಥಿರ ಧ್ರುವ ಭಿನ್ನ atan2_p3 = -0.3258083974640975f*(ಭಿನ್ನ)(180/CV_PI);
ಸ್ಥಿರ ಧ್ರುವ ಭಿನ್ನ atan2_p5 = 0.1555786518463281f*(ಭಿನ್ನ)(180/CV_PI);
ಸ್ಥಿರ ಧ್ರುವ ಭಿನ್ನ atan2_p7 = -0.04432655554792128f*(ಭಿನ್ನ)(180/CV_PI);

#ifdef __EMSCRIPTEN__
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಭಿನ್ನ atan_f32(ಭಿನ್ನ y, ಭಿನ್ನ x)
{
    ಭಿನ್ನ a = atan2(y, x) * 180.0f / CV_PI;
    ಯದಿ (a < 0.0f)
        a += 360.0f;
    ಯದಿ (a >= 360.0f)
        a -= 360.0f;
    ಪ್ರತ್ಯಾಗಮ a; // range [0; 360)
}
#ಅನ್ಯಥಾ
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಭಿನ್ನ atan_f32(ಭಿನ್ನ y, ಭಿನ್ನ x)
{
    ಭಿನ್ನ ax = std::abs(x), ay = std::abs(y);
    ಭಿನ್ನ a, c, c2;
    ಯದಿ( ax >= ay )
    {
        c = ay/(ax + (ಭಿನ್ನ)DBL_EPSILON);
        c2 = c*c;
        a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    ಅನ್ಯಥಾ
    {
        c = ax/(ay + (ಭಿನ್ನ)DBL_EPSILON);
        c2 = c*c;
        a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    ಯದಿ( x < 0 )
        a = 180.f - a;
    ಯದಿ( y < 0 )
        a = 360.f - a;
    ಪ್ರತ್ಯಾಗಮ a;
}
#endif

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)

v_float32 v_atan_f32(ಧ್ರುವ v_float32& y, ಧ್ರುವ v_float32& x)
{
    v_float32 eps = vx_setall_f32((ಭಿನ್ನ)DBL_EPSILON);
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
    ಪ್ರತ್ಯಾಗಮ a;
}

#endif

} // anonymous::

ಸ್ಥಿರ ಶೂನ್ಯ cartToPolar32f_(ಧ್ರುವ ಭಿನ್ನ *X, ಧ್ರುವ ಭಿನ್ನ *Y, ಭಿನ್ನ *mag, ಭಿನ್ನ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees )
{
    ಭಿನ್ನ scale = angleInDegrees ? 1.f : (ಭಿನ್ನ)(CV_PI/180);
    ಪೂರ್ಣಾಙ್ಕ i = 0;
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    ಪ್ರತಿ( ; i < len; i += VECSZ*2 )
    {
        ಯದಿ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            ಯದಿ( i == 0 || angle == X || angle == Y )
                ಭಙ್ಗ;
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

    ಪ್ರತಿ( ; i < len; i++ )
    {
        ಭಿನ್ನ x0 = X[i], y0 = Y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
        angle[i] = atan_f32(y0, x0)*scale;
    }
}

ಶೂನ್ಯ cartToPolar32f(ಧ್ರುವ ಭಿನ್ನ *X, ಧ್ರುವ ಭಿನ್ನ *Y, ಭಿನ್ನ *mag, ಭಿನ್ನ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    cartToPolar32f_(X, Y, mag, angle, len, angleInDegrees );
}

ಶೂನ್ಯ cartToPolar64f(ಧ್ರುವ ದ್ವಿಗುಣ *X, ಧ್ರುವ ದ್ವಿಗುಣ *Y, ದ್ವಿಗುಣ *mag, ದ್ವಿಗುಣ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ BLKSZ = 128;
    ಭಿನ್ನ ybuf[BLKSZ], xbuf[BLKSZ], mbuf[BLKSZ], abuf[BLKSZ];
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i += BLKSZ )
    {
        ಪೂರ್ಣಾಙ್ಕ j, blksz = std::min(BLKSZ, len - i);
        ಪ್ರತಿ( j = 0; j < blksz; j++ )
        {
            xbuf[j] = (ಭಿನ್ನ)X[i + j];
            ybuf[j] = (ಭಿನ್ನ)Y[i + j];
        }
        cartToPolar32f_(xbuf, ybuf, mbuf, abuf, blksz, angleInDegrees);
        ಪ್ರತಿ( j = 0; j < blksz; j++ )
            mag[i + j] = mbuf[j];
        ಪ್ರತಿ( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

ನಾಮಾಕಾಶ {

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ SinCos_32f(ಧ್ರುವ ಭಿನ್ನ* mag, ಧ್ರುವ ಭಿನ್ನ* angle, ಭಿನ್ನ* cosval, ಭಿನ್ನ* sinval, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ angle_in_degrees)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ N = 64;

    ಸ್ಥಿರ ಧ್ರುವ ದ್ವಿಗುಣ sin_table[] =
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

    ಸ್ಥಿರ ಧ್ರುವ ದ್ವಿಗುಣ k2 = (2*CV_PI)/N;

    ಸ್ಥಿರ ಧ್ರುವ ದ್ವಿಗುಣ sin_a0 = -0.166630293345647*k2*k2*k2;
    ಸ್ಥಿರ ಧ್ರುವ ದ್ವಿಗುಣ sin_a2 = k2;

    ಸ್ಥಿರ ಧ್ರುವ ದ್ವಿಗುಣ cos_a0 = -0.499818138450326*k2*k2;
    /*static const double cos_a2 =  1;*/

    ದ್ವಿಗುಣ k1;
    ಪೂರ್ಣಾಙ್ಕ i = 0;

    ಯದಿ( !angle_in_degrees )
        k1 = N/(2*CV_PI);
    ಅನ್ಯಥಾ
        k1 = N/360.;

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ VECSZ = VTraits<v_float32>::vlanes();
    ಧ್ರುವ v_float32 scale = vx_setall_f32(angle_in_degrees ? (ಭಿನ್ನ)CV_PI / 180.f : 1.f);

    ಪ್ರತಿ( ; i < len; i += VECSZ*2 )
    {
        ಯದಿ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            ಯದಿ( i == 0 || angle == cosval || angle == sinval || mag == cosval || mag == sinval )
                ಭಙ್ಗ;
            i = len - VECSZ*2;
        }

        v_float32 r0 = v_mul(vx_load(angle + i), scale);
        v_float32 r1 = v_mul(vx_load(angle + i + VECSZ), scale);

        v_float32 c0, c1, s0, s1;
        v_sincos(r0, s0, c0);
        v_sincos(r1, s1, c1);

        ಯದಿ( mag )
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

    ಪ್ರತಿ( ; i < len; i++ )
    {
        ದ್ವಿಗುಣ t = angle[i]*k1;
        ಪೂರ್ಣಾಙ್ಕ it = cvRound(t);
        t -= it;
        ಪೂರ್ಣಾಙ್ಕ sin_idx = it & (N - 1);
        ಪೂರ್ಣಾಙ್ಕ cos_idx = (N/4 - sin_idx) & (N - 1);

        ದ್ವಿಗುಣ sin_b = (sin_a0*t*t + sin_a2)*t;
        ದ್ವಿಗುಣ cos_b = cos_a0*t*t + 1;

        ದ್ವಿಗುಣ sin_a = sin_table[sin_idx];
        ದ್ವಿಗುಣ cos_a = sin_table[cos_idx];

        ದ್ವಿಗುಣ sin_val = sin_a*cos_b + cos_a*sin_b;
        ದ್ವಿಗುಣ cos_val = cos_a*cos_b - sin_a*sin_b;

        ಯದಿ (mag)
        {
            ದ್ವಿಗುಣ mag_val = mag[i];
            sin_val *= mag_val;
            cos_val *= mag_val;
        }

        sinval[i] = (ಭಿನ್ನ)sin_val;
        cosval[i] = (ಭಿನ್ನ)cos_val;
    }
}

} // anonymous::

ಶೂನ್ಯ polarToCart32f(ಧ್ರುವ ಭಿನ್ನ *mag, ಧ್ರುವ ಭಿನ್ನ *angle, ಭಿನ್ನ *X, ಭಿನ್ನ *Y, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees)
{
    CV_INSTRUMENT_REGION();
    SinCos_32f(mag, angle, X, Y, len, angleInDegrees);
}

ಶೂನ್ಯ polarToCart64f(ಧ್ರುವ ದ್ವಿಗುಣ *mag, ಧ್ರುವ ದ್ವಿಗುಣ *angle, ದ್ವಿಗುಣ *X, ದ್ವಿಗುಣ *Y, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ BLKSZ = 128;
    ಭಿನ್ನ ybuf[BLKSZ], xbuf[BLKSZ], _mbuf[BLKSZ], abuf[BLKSZ];
    ಭಿನ್ನ* mbuf = mag ? _mbuf : nullptr;
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i += BLKSZ )
    {
        ಪೂರ್ಣಾಙ್ಕ j, blksz = std::min(BLKSZ, len - i);
        ಪ್ರತಿ( j = 0; j < blksz; j++ )
        {
            ಯದಿ (mbuf)
                mbuf[j] = (ಭಿನ್ನ)mag[i + j];
            abuf[j] = (ಭಿನ್ನ)angle[i + j];
        }
        SinCos_32f(mbuf, abuf, xbuf, ybuf, blksz, angleInDegrees);
        ಪ್ರತಿ( j = 0; j < blksz; j++ )
            X[i + j] = xbuf[j];
        ಪ್ರತಿ( j = 0; j < blksz; j++ )
            Y[i + j] = ybuf[j];
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ fastAtan32f_(ಧ್ರುವ ಭಿನ್ನ *Y, ಧ್ರುವ ಭಿನ್ನ *X, ಭಿನ್ನ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees )
{
    ಭಿನ್ನ scale = angleInDegrees ? 1.f : (ಭಿನ್ನ)(CV_PI/180);
    ಪೂರ್ಣಾಙ್ಕ i = 0;
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    ಪ್ರತಿ( ; i < len; i += VECSZ*2 )
    {
        ಯದಿ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            ಯದಿ( i == 0 || angle == X || angle == Y )
                ಭಙ್ಗ;
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

    ಪ್ರತಿ( ; i < len; i++ )
        angle[i] = atan_f32(Y[i], X[i])*scale;
}

ಶೂನ್ಯ fastAtan32f(ಧ್ರುವ ಭಿನ್ನ *Y, ಧ್ರುವ ಭಿನ್ನ *X, ಭಿನ್ನ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f_(Y, X, angle, len, angleInDegrees );
}

ಶೂನ್ಯ fastAtan64f(ಧ್ರುವ ದ್ವಿಗುಣ *Y, ಧ್ರುವ ದ್ವಿಗುಣ *X, ದ್ವಿಗುಣ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ BLKSZ = 128;
    ಭಿನ್ನ ybuf[BLKSZ], xbuf[BLKSZ], abuf[BLKSZ];
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i += BLKSZ )
    {
        ಪೂರ್ಣಾಙ್ಕ j, blksz = std::min(BLKSZ, len - i);
        ಪ್ರತಿ( j = 0; j < blksz; j++ )
        {
            ybuf[j] = (ಭಿನ್ನ)Y[i + j];
            xbuf[j] = (ಭಿನ್ನ)X[i + j];
        }
        fastAtan32f_(ybuf, xbuf, abuf, blksz, angleInDegrees);
        ಪ್ರತಿ( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

// deprecated
ಶೂನ್ಯ fastAtan2(ಧ್ರುವ ಭಿನ್ನ *Y, ಧ್ರುವ ಭಿನ್ನ *X, ಭಿನ್ನ *angle, ಪೂರ್ಣಾಙ್ಕ len, ತಾರ್ಕಿಕ angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

ಶೂನ್ಯ magnitude32f(ಧ್ರುವ ಭಿನ್ನ* x, ಧ್ರುವ ಭಿನ್ನ* y, ಭಿನ್ನ* mag, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ i = 0;

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ VECSZ = VTraits<v_float32>::vlanes();
    ಪ್ರತಿ( ; i < len; i += VECSZ*2 )
    {
        ಯದಿ( i + VECSZ*2 > len )
        {
            ಯದಿ( i == 0 || mag == x || mag == y )
                ಭಙ್ಗ;
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

    ಪ್ರತಿ( ; i < len; i++ )
    {
        ಭಿನ್ನ x0 = x[i], y0 = y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
    }
}

ಶೂನ್ಯ magnitude64f(ಧ್ರುವ ದ್ವಿಗುಣ* x, ಧ್ರುವ ದ್ವಿಗುಣ* y, ದ್ವಿಗುಣ* mag, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ i = 0;

#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ VECSZ = VTraits<v_float64>::vlanes();
    ಪ್ರತಿ( ; i < len; i += VECSZ*2 )
    {
        ಯದಿ( i + VECSZ*2 > len )
        {
            ಯದಿ( i == 0 || mag == x || mag == y )
                ಭಙ್ಗ;
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

    ಪ್ರತಿ( ; i < len; i++ )
    {
        ದ್ವಿಗುಣ x0 = x[i], y0 = y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
    }
}


ಶೂನ್ಯ invSqrt32f(ಧ್ರುವ ಭಿನ್ನ* src, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ i = 0;

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ VECSZ = VTraits<v_float32>::vlanes();
    ಪ್ರತಿ( ; i < len; i += VECSZ*2 )
    {
        ಯದಿ( i + VECSZ*2 > len )
        {
            ಯದಿ( i == 0 || src == dst )
                ಭಙ್ಗ;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    ಪ್ರತಿ( ; i < len; i++ )
        dst[i] = 1/std::sqrt(src[i]);
}


ಶೂನ್ಯ invSqrt64f(ಧ್ರುವ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();
    ಪೂರ್ಣಾಙ್ಕ i = 0;

#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ VECSZ = VTraits<v_float64>::vlanes();
    ಪ್ರತಿ ( ; i < len; i += VECSZ*2)
    {
        ಯದಿ( i + VECSZ*2 > len )
        {
            ಯದಿ( i == 0 || src == dst )
                ಭಙ್ಗ;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
#endif

    ಪ್ರತಿ( ; i < len; i++ )
        dst[i] = 1/std::sqrt(src[i]);
}


ಶೂನ್ಯ sqrt32f(ಧ್ರುವ ಭಿನ್ನ* src, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ i = 0;

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ VECSZ = VTraits<v_float32>::vlanes();
    ಪ್ರತಿ( ; i < len; i += VECSZ*2 )
    {
        ಯದಿ( i + VECSZ*2 > len )
        {
            ಯದಿ( i == 0 || src == dst )
                ಭಙ್ಗ;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    ಪ್ರತಿ( ; i < len; i++ )
        dst[i] = std::sqrt(src[i]);
}


ಶೂನ್ಯ sqrt64f(ಧ್ರುವ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ i = 0;

#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ VECSZ = VTraits<v_float64>::vlanes();
    ಪ್ರತಿ( ; i < len; i += VECSZ*2 )
    {
        ಯದಿ( i + VECSZ*2 > len )
        {
            ಯದಿ( i == 0 || src == dst )
                ಭಙ್ಗ;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    ಪ್ರತಿ( ; i < len; i++ )
        dst[i] = std::sqrt(src[i]);
}

// Workaround for ICE in MSVS 2015 update 3 (issue #7795)
// CV_AVX is not used here, because generated code is faster in non-AVX mode.
// (tested with disabled IPP on i5-6300U)
#ಯದಿ (defined _MSC_VER && _MSC_VER >= 1900 && (defined(_M_IX86) || defined(_M_X64))) || defined(__EMSCRIPTEN__)
ಶೂನ್ಯ exp32f(ಧ್ರುವ ಭಿನ್ನ *src, ಭಿನ್ನ *dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

ಶೂನ್ಯ exp64f(ಧ್ರುವ ದ್ವಿಗುಣ *src, ದ್ವಿಗುಣ *dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

ಶೂನ್ಯ log32f(ಧ್ರುವ ಭಿನ್ನ *src, ಭಿನ್ನ *dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
ಶೂನ್ಯ log64f(ಧ್ರುವ ದ್ವಿಗುಣ *src, ದ್ವಿಗುಣ *dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_INSTRUMENT_REGION();

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
#ಅನ್ಯಥಾ

////////////////////////////////////// EXP /////////////////////////////////////


#endif // issue 7795

ಭಿನ್ನ fastAtan2( ಭಿನ್ನ y, ಭಿನ್ನ x )
{
    ಪ್ರತ್ಯಾಗಮ atan_f32(y, x);
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

}} // namespace cv::hal
