// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "mathfuncs.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
ಶೂನ್ಯ cart_ಗಮ್ಯ_Polar32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *X, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *Y, ದಶಮಾಂಶ* mag, ದಶಮಾಂಶ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees);
ಶೂನ್ಯ cart_ಗಮ್ಯ_Polar64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *X, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *Y, ದ್ವಿಗುಣ* mag, ದ್ವಿಗುಣ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees);
ಶೂನ್ಯ polar_ಗಮ್ಯ_Cart32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *mag, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *angle, ದಶಮಾಂಶ *X, ದಶಮಾಂಶ *Y, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees);
ಶೂನ್ಯ polar_ಗಮ್ಯ_Cart64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *mag, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *angle, ದ್ವಿಗುಣ *X, ದ್ವಿಗುಣ *Y, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees);
ಶೂನ್ಯ fastAtan32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *Y, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *X, ದಶಮಾಂಶ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees);
ಶೂನ್ಯ fastAtan64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *Y, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *X, ದ್ವಿಗುಣ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees);
ಶೂನ್ಯ fastAtan2(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *Y, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *X, ದಶಮಾಂಶ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees);
ಶೂನ್ಯ magnitude32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* x, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* y, ದಶಮಾಂಶ* mag, ಪೂರ್ಣಾಂಕ len);
ಶೂನ್ಯ magnitude64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* x, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* y, ದ್ವಿಗುಣ* mag, ಪೂರ್ಣಾಂಕ len);
ಶೂನ್ಯ invSqrt32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len);
ಶೂನ್ಯ invSqrt64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len);
ಶೂನ್ಯ sqrt32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len);
ಶೂನ್ಯ sqrt64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len);
ದಶಮಾಂಶ fastAtan2(ದಶಮಾಂಶ y, ದಶಮಾಂಶ x);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ಬಳಸಿ ನಾಮಾಕಾಶ std;
ಬಳಸಿ ನಾಮಾಕಾಶ cv;

ನಾಮಾಕಾಶ {

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ atan2_p1 = 0.9997878412794807f*(ದಶಮಾಂಶ)(180/CV_PI);
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ atan2_p3 = -0.3258083974640975f*(ದಶಮಾಂಶ)(180/CV_PI);
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ atan2_p5 = 0.1555786518463281f*(ದಶಮಾಂಶ)(180/CV_PI);
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ atan2_p7 = -0.04432655554792128f*(ದಶಮಾಂಶ)(180/CV_PI);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __EMSCRIPTEN__
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದಶಮಾಂಶ atan_f32(ದಶಮಾಂಶ y, ದಶಮಾಂಶ x)
{
    ದಶಮಾಂಶ a = atan2(y, x) * 180.0f / CV_PI;
    ಆದರೆ (a < 0.0f)
        a += 360.0f;
    ಆದರೆ (a >= 360.0f)
        a -= 360.0f;
    ಹಿಂತಿರುಗಿ a; // range [0; 360)
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದಶಮಾಂಶ atan_f32(ದಶಮಾಂಶ y, ದಶಮಾಂಶ x)
{
    ದಶಮಾಂಶ ax = std::ನಿರಪೇಕ್ಷ(x), ay = std::ನಿರಪೇಕ್ಷ(y);
    ದಶಮಾಂಶ a, c, c2;
    ಆದರೆ( ax >= ay )
    {
        c = ay/(ax + (ದಶಮಾಂಶ)DBL_EPSILON);
        c2 = c*c;
        a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        c = ax/(ay + (ದಶಮಾಂಶ)DBL_EPSILON);
        c2 = c*c;
        a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    ಆದರೆ( x < 0 )
        a = 180.f - a;
    ಆದರೆ( y < 0 )
        a = 360.f - a;
    ಹಿಂತಿರುಗಿ a;
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)

v_float32 v_atan_f32(ಸ್ಥಿರಾಂಕ v_float32& y, ಸ್ಥಿರಾಂಕ v_float32& x)
{
    v_float32 eps = vx_setall_f32((ದಶಮಾಂಶ)DBL_EPSILON);
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
    v_float32 c = v_div(v_min(ax, ay), v_ಕೂಡಿಸು(v_max(ax, ay), eps));
    v_float32 cc = v_mul(c, c);
    v_float32 a = v_mul(v_fma(v_fma(v_fma(cc, p7, p5), cc, p3), cc, p1), c);
    a = v_select(v_ge(ax, ay), a, v_sub(val90, a));
    a = v_select(v_lt(x, z), v_sub(val180, a), a);
    a = v_select(v_lt(y, z), v_sub(val360, a), a);
    ಹಿಂತಿರುಗಿ a;
}

#ಆದರೆ_ಅಂತ್ಯ

} // anonymous::

ಸ್ಥಿರ ಶೂನ್ಯ cart_ಗಮ್ಯ_Polar32f_2(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *X, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *Y, ದಶಮಾಂಶ *mag, ದಶಮಾಂಶ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees )
{
    ದಶಮಾಂಶ scale = angle_ಒಳ_Degrees ? 1.f : (ದಶಮಾಂಶ)(CV_PI/180);
    ಪೂರ್ಣಾಂಕ i = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    ಗಾಗಿ( ; i < len; i += VECSZ*2 )
    {
        ಆದರೆ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            ಆದರೆ( i == 0 || angle == X || angle == Y )
                ನಿಲ್ಲಿಸು;
            i = len - VECSZ*2;
        }

        v_float32 x0 = vx_ತುಂಬು(X + i);
        v_float32 y0 = vx_ತುಂಬು(Y + i);
        v_float32 x1 = vx_ತುಂಬು(X + i + VECSZ);
        v_float32 y1 = vx_ತುಂಬು(Y + i + VECSZ);

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
#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ( ; i < len; i++ )
    {
        ದಶಮಾಂಶ x0 = X[i], y0 = Y[i];
        mag[i] = std::ವರ್ಗಮೂಲ(x0*x0 + y0*y0);
        angle[i] = atan_f32(y0, x0)*scale;
    }
}

ಶೂನ್ಯ cart_ಗಮ್ಯ_Polar32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *X, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *Y, ದಶಮಾಂಶ *mag, ದಶಮಾಂಶ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees )
{
    CV_INSTRUMENT_REGION();
    cart_ಗಮ್ಯ_Polar32f_2(X, Y, mag, angle, len, angle_ಒಳ_Degrees );
}

ಶೂನ್ಯ cart_ಗಮ್ಯ_Polar64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *X, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *Y, ದ್ವಿಗುಣ *mag, ದ್ವಿಗುಣ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees)
{
    CV_INSTRUMENT_REGION();

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ BLKSZ = 128;
    ದಶಮಾಂಶ ybuf[BLKSZ], xbuf[BLKSZ], mbuf[BLKSZ], abuf[BLKSZ];
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i += BLKSZ )
    {
        ಪೂರ್ಣಾಂಕ j, blksz = std::min(BLKSZ, len - i);
        ಗಾಗಿ( j = 0; j < blksz; j++ )
        {
            xbuf[j] = (ದಶಮಾಂಶ)X[i + j];
            ybuf[j] = (ದಶಮಾಂಶ)Y[i + j];
        }
        cart_ಗಮ್ಯ_Polar32f_2(xbuf, ybuf, mbuf, abuf, blksz, angle_ಒಳ_Degrees);
        ಗಾಗಿ( j = 0; j < blksz; j++ )
            mag[i + j] = mbuf[j];
        ಗಾಗಿ( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

ನಾಮಾಕಾಶ {

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ SinCos_32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* mag, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* angle, ದಶಮಾಂಶ* cosval, ದಶಮಾಂಶ* sinval, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ angle_ಒಳ_degrees)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ N = 64;

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ sin_ಕೋಷ್ಟಕ[] =
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

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ k2 = (2*CV_PI)/N;

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ sin_a0 = -0.166630293345647*k2*k2*k2;
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ sin_a2 = k2;

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ cos_a0 = -0.499818138450326*k2*k2;
    /*static const double cos_a2 =  1;*/

    ದ್ವಿಗುಣ k1;
    ಪೂರ್ಣಾಂಕ i = 0;

    ಆದರೆ( !angle_ಒಳ_degrees )
        k1 = N/(2*CV_PI);
    ಇಲ್ಲದಿದ್ದರೆ
        k1 = N/360.;

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
    ಸ್ಥಿರಾಂಕ v_float32 scale = vx_setall_f32(angle_ಒಳ_degrees ? (ದಶಮಾಂಶ)CV_PI / 180.f : 1.f);

    ಗಾಗಿ( ; i < len; i += VECSZ*2 )
    {
        ಆದರೆ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            ಆದರೆ( i == 0 || angle == cosval || angle == sinval || mag == cosval || mag == sinval )
                ನಿಲ್ಲಿಸು;
            i = len - VECSZ*2;
        }

        v_float32 r0 = v_mul(vx_ತುಂಬು(angle + i), scale);
        v_float32 r1 = v_mul(vx_ತುಂಬು(angle + i + VECSZ), scale);

        v_float32 c0, c1, s0, s1;
        v_sincos(r0, s0, c0);
        v_sincos(r1, s1, c1);

        ಆದರೆ( mag )
        {
            v_float32 m0 = vx_ತುಂಬು(mag + i);
            v_float32 m1 = vx_ತುಂಬು(mag + i + VECSZ);
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
#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ( ; i < len; i++ )
    {
        ದ್ವಿಗುಣ t = angle[i]*k1;
        ಪೂರ್ಣಾಂಕ it = cv_ಸುತ್ತು(t);
        t -= it;
        ಪೂರ್ಣಾಂಕ sin_idx = it & (N - 1);
        ಪೂರ್ಣಾಂಕ cos_idx = (N/4 - sin_idx) & (N - 1);

        ದ್ವಿಗುಣ sin_b = (sin_a0*t*t + sin_a2)*t;
        ದ್ವಿಗುಣ cos_b = cos_a0*t*t + 1;

        ದ್ವಿಗುಣ sin_a = sin_ಕೋಷ್ಟಕ[sin_idx];
        ದ್ವಿಗುಣ cos_a = sin_ಕೋಷ್ಟಕ[cos_idx];

        ದ್ವಿಗುಣ sin_val = sin_a*cos_b + cos_a*sin_b;
        ದ್ವಿಗುಣ cos_val = cos_a*cos_b - sin_a*sin_b;

        ಆದರೆ (mag)
        {
            ದ್ವಿಗುಣ mag_val = mag[i];
            sin_val *= mag_val;
            cos_val *= mag_val;
        }

        sinval[i] = (ದಶಮಾಂಶ)sin_val;
        cosval[i] = (ದಶಮಾಂಶ)cos_val;
    }
}

} // anonymous::

ಶೂನ್ಯ polar_ಗಮ್ಯ_Cart32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *mag, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *angle, ದಶಮಾಂಶ *X, ದಶಮಾಂಶ *Y, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees)
{
    CV_INSTRUMENT_REGION();
    SinCos_32f(mag, angle, X, Y, len, angle_ಒಳ_Degrees);
}

ಶೂನ್ಯ polar_ಗಮ್ಯ_Cart64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *mag, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *angle, ದ್ವಿಗುಣ *X, ದ್ವಿಗುಣ *Y, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees)
{
    CV_INSTRUMENT_REGION();

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ BLKSZ = 128;
    ದಶಮಾಂಶ ybuf[BLKSZ], xbuf[BLKSZ], _mbuf[BLKSZ], abuf[BLKSZ];
    ದಶಮಾಂಶ* mbuf = mag ? _mbuf : ಶೂನ್ಯಸೂಚಕ;
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i += BLKSZ )
    {
        ಪೂರ್ಣಾಂಕ j, blksz = std::min(BLKSZ, len - i);
        ಗಾಗಿ( j = 0; j < blksz; j++ )
        {
            ಆದರೆ (mbuf)
                mbuf[j] = (ದಶಮಾಂಶ)mag[i + j];
            abuf[j] = (ದಶಮಾಂಶ)angle[i + j];
        }
        SinCos_32f(mbuf, abuf, xbuf, ybuf, blksz, angle_ಒಳ_Degrees);
        ಗಾಗಿ( j = 0; j < blksz; j++ )
            X[i + j] = xbuf[j];
        ಗಾಗಿ( j = 0; j < blksz; j++ )
            Y[i + j] = ybuf[j];
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ fastAtan32f_(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *Y, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *X, ದಶಮಾಂಶ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees )
{
    ದಶಮಾಂಶ scale = angle_ಒಳ_Degrees ? 1.f : (ದಶಮಾಂಶ)(CV_PI/180);
    ಪೂರ್ಣಾಂಕ i = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    ಗಾಗಿ( ; i < len; i += VECSZ*2 )
    {
        ಆದರೆ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            ಆದರೆ( i == 0 || angle == X || angle == Y )
                ನಿಲ್ಲಿಸು;
            i = len - VECSZ*2;
        }

        v_float32 y0 = vx_ತುಂಬು(Y + i);
        v_float32 x0 = vx_ತುಂಬು(X + i);
        v_float32 y1 = vx_ತುಂಬು(Y + i + VECSZ);
        v_float32 x1 = vx_ತುಂಬು(X + i + VECSZ);

        v_float32 r0 = v_mul(v_atan_f32(y0, x0), s);
        v_float32 r1 = v_mul(v_atan_f32(y1, x1), s);

        v_store(angle + i, r0);
        v_store(angle + i + VECSZ, r1);
    }
    vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ( ; i < len; i++ )
        angle[i] = atan_f32(Y[i], X[i])*scale;
}

ಶೂನ್ಯ fastAtan32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *Y, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *X, ದಶಮಾಂಶ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f_(Y, X, angle, len, angle_ಒಳ_Degrees );
}

ಶೂನ್ಯ fastAtan64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *Y, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *X, ದ್ವಿಗುಣ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees)
{
    CV_INSTRUMENT_REGION();

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ BLKSZ = 128;
    ದಶಮಾಂಶ ybuf[BLKSZ], xbuf[BLKSZ], abuf[BLKSZ];
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i += BLKSZ )
    {
        ಪೂರ್ಣಾಂಕ j, blksz = std::min(BLKSZ, len - i);
        ಗಾಗಿ( j = 0; j < blksz; j++ )
        {
            ybuf[j] = (ದಶಮಾಂಶ)Y[i + j];
            xbuf[j] = (ದಶಮಾಂಶ)X[i + j];
        }
        fastAtan32f_(ybuf, xbuf, abuf, blksz, angle_ಒಳ_Degrees);
        ಗಾಗಿ( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

// deprecated
ಶೂನ್ಯ fastAtan2(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *Y, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *X, ದಶಮಾಂಶ *angle, ಪೂರ್ಣಾಂಕ len, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f(Y, X, angle, len, angle_ಒಳ_Degrees);
}

ಶೂನ್ಯ magnitude32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* x, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* y, ದಶಮಾಂಶ* mag, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ i = 0;

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
    ಗಾಗಿ( ; i < len; i += VECSZ*2 )
    {
        ಆದರೆ( i + VECSZ*2 > len )
        {
            ಆದರೆ( i == 0 || mag == x || mag == y )
                ನಿಲ್ಲಿಸು;
            i = len - VECSZ*2;
        }
        v_float32 x0 = vx_ತುಂಬು(x + i), x1 = vx_ತುಂಬು(x + i + VECSZ);
        v_float32 y0 = vx_ತುಂಬು(y + i), y1 = vx_ತುಂಬು(y + i + VECSZ);
        x0 = v_sqrt(v_muladd(x0, x0, v_mul(y0, y0)));
        x1 = v_sqrt(v_muladd(x1, x1, v_mul(y1, y1)));
        v_store(mag + i, x0);
        v_store(mag + i + VECSZ, x1);
    }
    vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ( ; i < len; i++ )
    {
        ದಶಮಾಂಶ x0 = x[i], y0 = y[i];
        mag[i] = std::ವರ್ಗಮೂಲ(x0*x0 + y0*y0);
    }
}

ಶೂನ್ಯ magnitude64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* x, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* y, ದ್ವಿಗುಣ* mag, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ i = 0;

#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float64>::vlanes();
    ಗಾಗಿ( ; i < len; i += VECSZ*2 )
    {
        ಆದರೆ( i + VECSZ*2 > len )
        {
            ಆದರೆ( i == 0 || mag == x || mag == y )
                ನಿಲ್ಲಿಸು;
            i = len - VECSZ*2;
        }
        v_float64 x0 = vx_ತುಂಬು(x + i), x1 = vx_ತುಂಬು(x + i + VECSZ);
        v_float64 y0 = vx_ತುಂಬು(y + i), y1 = vx_ತುಂಬು(y + i + VECSZ);
        x0 = v_sqrt(v_muladd(x0, x0, v_mul(y0, y0)));
        x1 = v_sqrt(v_muladd(x1, x1, v_mul(y1, y1)));
        v_store(mag + i, x0);
        v_store(mag + i + VECSZ, x1);
    }
    vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ( ; i < len; i++ )
    {
        ದ್ವಿಗುಣ x0 = x[i], y0 = y[i];
        mag[i] = std::ವರ್ಗಮೂಲ(x0*x0 + y0*y0);
    }
}


ಶೂನ್ಯ invSqrt32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ i = 0;

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
    ಗಾಗಿ( ; i < len; i += VECSZ*2 )
    {
        ಆದರೆ( i + VECSZ*2 > len )
        {
            ಆದರೆ( i == 0 || src == dst )
                ನಿಲ್ಲಿಸು;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_ತುಂಬು(src + i), t1 = vx_ತುಂಬು(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ( ; i < len; i++ )
        dst[i] = 1/std::ವರ್ಗಮೂಲ(src[i]);
}


ಶೂನ್ಯ invSqrt64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();
    ಪೂರ್ಣಾಂಕ i = 0;

#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float64>::vlanes();
    ಗಾಗಿ ( ; i < len; i += VECSZ*2)
    {
        ಆದರೆ( i + VECSZ*2 > len )
        {
            ಆದರೆ( i == 0 || src == dst )
                ನಿಲ್ಲಿಸು;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_ತುಂಬು(src + i), t1 = vx_ತುಂಬು(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ( ; i < len; i++ )
        dst[i] = 1/std::ವರ್ಗಮೂಲ(src[i]);
}


ಶೂನ್ಯ sqrt32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ i = 0;

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
    ಗಾಗಿ( ; i < len; i += VECSZ*2 )
    {
        ಆದರೆ( i + VECSZ*2 > len )
        {
            ಆದರೆ( i == 0 || src == dst )
                ನಿಲ್ಲಿಸು;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_ತುಂಬು(src + i), t1 = vx_ತುಂಬು(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ( ; i < len; i++ )
        dst[i] = std::ವರ್ಗಮೂಲ(src[i]);
}


ಶೂನ್ಯ sqrt64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src, ದ್ವಿಗುಣ* dst, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ i = 0;

#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float64>::vlanes();
    ಗಾಗಿ( ; i < len; i += VECSZ*2 )
    {
        ಆದರೆ( i + VECSZ*2 > len )
        {
            ಆದರೆ( i == 0 || src == dst )
                ನಿಲ್ಲಿಸು;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_ತುಂಬು(src + i), t1 = vx_ತುಂಬು(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ

    ಗಾಗಿ( ; i < len; i++ )
        dst[i] = std::ವರ್ಗಮೂಲ(src[i]);
}

// Workaround for ICE in MSVS 2015 update 3 (issue #7795)
// CV_AVX is not used here, because generated code is faster in non-AVX mode.
// (tested with disabled IPP on i5-6300U)
#ಆದರೆ (defined _MSC_VER && _MSC_VER >= 1900 && (defined(_M_IX86) || defined(_M_X64))) || defined(__EMSCRIPTEN__)
ಶೂನ್ಯ exp32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src, ದಶಮಾಂಶ *dst, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
    {
        dst[i] = std::ಘಾತಾಂಕ(src[i]);
    }
}

ಶೂನ್ಯ exp64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src, ದ್ವಿಗುಣ *dst, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
    {
        dst[i] = std::ಘಾತಾಂಕ(src[i]);
    }
}

ಶೂನ್ಯ log32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *src, ದಶಮಾಂಶ *dst, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
    {
        dst[i] = std::ಲಘುಗಣಕ(src[i]);
    }
}
ಶೂನ್ಯ log64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *src, ದ್ವಿಗುಣ *dst, ಪೂರ್ಣಾಂಕ n)
{
    CV_INSTRUMENT_REGION();

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
    {
        dst[i] = std::ಲಘುಗಣಕ(src[i]);
    }
}
#ಇಲ್ಲದಿದ್ದರೆ

////////////////////////////////////// EXP /////////////////////////////////////


#ಆದರೆ_ಅಂತ್ಯ // issue 7795

ದಶಮಾಂಶ fastAtan2( ದಶಮಾಂಶ y, ದಶಮಾಂಶ x )
{
    ಹಿಂತಿರುಗಿ atan_f32(y, x);
}

#ಆದರೆ_ಅಂತ್ಯ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ

}} // namespace cv::hal
