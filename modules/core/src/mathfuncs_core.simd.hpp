// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "mathfuncs.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ hal {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
ਖਾਲੀ cart_ਮੰਜ਼ਿਲ_Polar32f(ਅਚਲ ਦਸ਼ਮਲਵ *X, ਅਚਲ ਦਸ਼ਮਲਵ *Y, ਦਸ਼ਮਲਵ* mag, ਦਸ਼ਮਲਵ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees);
ਖਾਲੀ cart_ਮੰਜ਼ਿਲ_Polar64f(ਅਚਲ ਦੁੱਗਣਾ *X, ਅਚਲ ਦੁੱਗਣਾ *Y, ਦੁੱਗਣਾ* mag, ਦੁੱਗਣਾ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees);
ਖਾਲੀ polar_ਮੰਜ਼ਿਲ_Cart32f(ਅਚਲ ਦਸ਼ਮਲਵ *mag, ਅਚਲ ਦਸ਼ਮਲਵ *angle, ਦਸ਼ਮਲਵ *X, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees);
ਖਾਲੀ polar_ਮੰਜ਼ਿਲ_Cart64f(ਅਚਲ ਦੁੱਗਣਾ *mag, ਅਚਲ ਦੁੱਗਣਾ *angle, ਦੁੱਗਣਾ *X, ਦੁੱਗਣਾ *Y, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees);
ਖਾਲੀ fastAtan32f(ਅਚਲ ਦਸ਼ਮਲਵ *Y, ਅਚਲ ਦਸ਼ਮਲਵ *X, ਦਸ਼ਮਲਵ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees);
ਖਾਲੀ fastAtan64f(ਅਚਲ ਦੁੱਗਣਾ *Y, ਅਚਲ ਦੁੱਗਣਾ *X, ਦੁੱਗਣਾ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees);
ਖਾਲੀ fastAtan2(ਅਚਲ ਦਸ਼ਮਲਵ *Y, ਅਚਲ ਦਸ਼ਮਲਵ *X, ਦਸ਼ਮਲਵ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees);
ਖਾਲੀ magnitude32f(ਅਚਲ ਦਸ਼ਮਲਵ* x, ਅਚਲ ਦਸ਼ਮਲਵ* y, ਦਸ਼ਮਲਵ* mag, ਪੂਰਨ len);
ਖਾਲੀ magnitude64f(ਅਚਲ ਦੁੱਗਣਾ* x, ਅਚਲ ਦੁੱਗਣਾ* y, ਦੁੱਗਣਾ* mag, ਪੂਰਨ len);
ਖਾਲੀ invSqrt32f(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len);
ਖਾਲੀ invSqrt64f(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len);
ਖਾਲੀ sqrt32f(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len);
ਖਾਲੀ sqrt64f(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len);
ਦਸ਼ਮਲਵ fastAtan2(ਦਸ਼ਮਲਵ y, ਦਸ਼ਮਲਵ x);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ std;
ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv;

ਨਾਮਾਕਾਸ਼ {

ਸਥਿਰ ਅਚਲ ਦਸ਼ਮਲਵ atan2_p1 = 0.9997878412794807f*(ਦਸ਼ਮਲਵ)(180/CV_PI);
ਸਥਿਰ ਅਚਲ ਦਸ਼ਮਲਵ atan2_p3 = -0.3258083974640975f*(ਦਸ਼ਮਲਵ)(180/CV_PI);
ਸਥਿਰ ਅਚਲ ਦਸ਼ਮਲਵ atan2_p5 = 0.1555786518463281f*(ਦਸ਼ਮਲਵ)(180/CV_PI);
ਸਥਿਰ ਅਚਲ ਦਸ਼ਮਲਵ atan2_p7 = -0.04432655554792128f*(ਦਸ਼ਮਲਵ)(180/CV_PI);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __EMSCRIPTEN__
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ atan_f32(ਦਸ਼ਮਲਵ y, ਦਸ਼ਮਲਵ x)
{
    ਦਸ਼ਮਲਵ a = atan2(y, x) * 180.0f / CV_PI;
    ਜੇ (a < 0.0f)
        a += 360.0f;
    ਜੇ (a >= 360.0f)
        a -= 360.0f;
    ਵਾਪਸ a; // range [0; 360)
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ atan_f32(ਦਸ਼ਮਲਵ y, ਦਸ਼ਮਲਵ x)
{
    ਦਸ਼ਮਲਵ ax = std::ਨਿਰਪੇਖ(x), ay = std::ਨਿਰਪੇਖ(y);
    ਦਸ਼ਮਲਵ a, c, c2;
    ਜੇ( ax >= ay )
    {
        c = ay/(ax + (ਦਸ਼ਮਲਵ)DBL_EPSILON);
        c2 = c*c;
        a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    ਨਹੀਂਤਾਂ
    {
        c = ax/(ay + (ਦਸ਼ਮਲਵ)DBL_EPSILON);
        c2 = c*c;
        a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    ਜੇ( x < 0 )
        a = 180.f - a;
    ਜੇ( y < 0 )
        a = 360.f - a;
    ਵਾਪਸ a;
}
#ਜੇ_ਅੰਤ

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)

v_float32 v_atan_f32(ਅਚਲ v_float32& y, ਅਚਲ v_float32& x)
{
    v_float32 eps = vx_setall_f32((ਦਸ਼ਮਲਵ)DBL_EPSILON);
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
    v_float32 c = v_div(v_min(ax, ay), v_ਜੋੜੋ(v_max(ax, ay), eps));
    v_float32 cc = v_mul(c, c);
    v_float32 a = v_mul(v_fma(v_fma(v_fma(cc, p7, p5), cc, p3), cc, p1), c);
    a = v_select(v_ge(ax, ay), a, v_sub(val90, a));
    a = v_select(v_lt(x, z), v_sub(val180, a), a);
    a = v_select(v_lt(y, z), v_sub(val360, a), a);
    ਵਾਪਸ a;
}

#ਜੇ_ਅੰਤ

} // anonymous::

ਸਥਿਰ ਖਾਲੀ cart_ਮੰਜ਼ਿਲ_Polar32f_2(ਅਚਲ ਦਸ਼ਮਲਵ *X, ਅਚਲ ਦਸ਼ਮਲਵ *Y, ਦਸ਼ਮਲਵ *mag, ਦਸ਼ਮਲਵ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees )
{
    ਦਸ਼ਮਲਵ scale = angle_ਵਿੱਚ_Degrees ? 1.f : (ਦਸ਼ਮਲਵ)(CV_PI/180);
    ਪੂਰਨ i = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    ਲਈ( ; i < len; i += VECSZ*2 )
    {
        ਜੇ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            ਜੇ( i == 0 || angle == X || angle == Y )
                ਰੋਕੋ;
            i = len - VECSZ*2;
        }

        v_float32 x0 = vx_ਲੱਦੋ(X + i);
        v_float32 y0 = vx_ਲੱਦੋ(Y + i);
        v_float32 x1 = vx_ਲੱਦੋ(X + i + VECSZ);
        v_float32 y1 = vx_ਲੱਦੋ(Y + i + VECSZ);

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
#ਜੇ_ਅੰਤ

    ਲਈ( ; i < len; i++ )
    {
        ਦਸ਼ਮਲਵ x0 = X[i], y0 = Y[i];
        mag[i] = std::ਵਰਗਮੂਲ(x0*x0 + y0*y0);
        angle[i] = atan_f32(y0, x0)*scale;
    }
}

ਖਾਲੀ cart_ਮੰਜ਼ਿਲ_Polar32f(ਅਚਲ ਦਸ਼ਮਲਵ *X, ਅਚਲ ਦਸ਼ਮਲਵ *Y, ਦਸ਼ਮਲਵ *mag, ਦਸ਼ਮਲਵ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees )
{
    CV_INSTRUMENT_REGION();
    cart_ਮੰਜ਼ਿਲ_Polar32f_2(X, Y, mag, angle, len, angle_ਵਿੱਚ_Degrees );
}

ਖਾਲੀ cart_ਮੰਜ਼ਿਲ_Polar64f(ਅਚਲ ਦੁੱਗਣਾ *X, ਅਚਲ ਦੁੱਗਣਾ *Y, ਦੁੱਗਣਾ *mag, ਦੁੱਗਣਾ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees)
{
    CV_INSTRUMENT_REGION();

    ਅਚਲ ਪੂਰਨ BLKSZ = 128;
    ਦਸ਼ਮਲਵ ybuf[BLKSZ], xbuf[BLKSZ], mbuf[BLKSZ], abuf[BLKSZ];
    ਲਈ( ਪੂਰਨ i = 0; i < len; i += BLKSZ )
    {
        ਪੂਰਨ j, blksz = std::min(BLKSZ, len - i);
        ਲਈ( j = 0; j < blksz; j++ )
        {
            xbuf[j] = (ਦਸ਼ਮਲਵ)X[i + j];
            ybuf[j] = (ਦਸ਼ਮਲਵ)Y[i + j];
        }
        cart_ਮੰਜ਼ਿਲ_Polar32f_2(xbuf, ybuf, mbuf, abuf, blksz, angle_ਵਿੱਚ_Degrees);
        ਲਈ( j = 0; j < blksz; j++ )
            mag[i + j] = mbuf[j];
        ਲਈ( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

ਨਾਮਾਕਾਸ਼ {

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ SinCos_32f(ਅਚਲ ਦਸ਼ਮਲਵ* mag, ਅਚਲ ਦਸ਼ਮਲਵ* angle, ਦਸ਼ਮਲਵ* cosval, ਦਸ਼ਮਲਵ* sinval, ਪੂਰਨ len, ਪੂਰਨ angle_ਵਿੱਚ_degrees)
{
    ਅਚਲ ਪੂਰਨ N = 64;

    ਸਥਿਰ ਅਚਲ ਦੁੱਗਣਾ sin_ਸਾਰਣੀ[] =
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

    ਸਥਿਰ ਅਚਲ ਦੁੱਗਣਾ k2 = (2*CV_PI)/N;

    ਸਥਿਰ ਅਚਲ ਦੁੱਗਣਾ sin_a0 = -0.166630293345647*k2*k2*k2;
    ਸਥਿਰ ਅਚਲ ਦੁੱਗਣਾ sin_a2 = k2;

    ਸਥਿਰ ਅਚਲ ਦੁੱਗਣਾ cos_a0 = -0.499818138450326*k2*k2;
    /*static const double cos_a2 =  1;*/

    ਦੁੱਗਣਾ k1;
    ਪੂਰਨ i = 0;

    ਜੇ( !angle_ਵਿੱਚ_degrees )
        k1 = N/(2*CV_PI);
    ਨਹੀਂਤਾਂ
        k1 = N/360.;

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float32>::vlanes();
    ਅਚਲ v_float32 scale = vx_setall_f32(angle_ਵਿੱਚ_degrees ? (ਦਸ਼ਮਲਵ)CV_PI / 180.f : 1.f);

    ਲਈ( ; i < len; i += VECSZ*2 )
    {
        ਜੇ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            ਜੇ( i == 0 || angle == cosval || angle == sinval || mag == cosval || mag == sinval )
                ਰੋਕੋ;
            i = len - VECSZ*2;
        }

        v_float32 r0 = v_mul(vx_ਲੱਦੋ(angle + i), scale);
        v_float32 r1 = v_mul(vx_ਲੱਦੋ(angle + i + VECSZ), scale);

        v_float32 c0, c1, s0, s1;
        v_sincos(r0, s0, c0);
        v_sincos(r1, s1, c1);

        ਜੇ( mag )
        {
            v_float32 m0 = vx_ਲੱਦੋ(mag + i);
            v_float32 m1 = vx_ਲੱਦੋ(mag + i + VECSZ);
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
#ਜੇ_ਅੰਤ

    ਲਈ( ; i < len; i++ )
    {
        ਦੁੱਗਣਾ t = angle[i]*k1;
        ਪੂਰਨ it = cv_ਗੇੜਾ(t);
        t -= it;
        ਪੂਰਨ sin_idx = it & (N - 1);
        ਪੂਰਨ cos_idx = (N/4 - sin_idx) & (N - 1);

        ਦੁੱਗਣਾ sin_b = (sin_a0*t*t + sin_a2)*t;
        ਦੁੱਗਣਾ cos_b = cos_a0*t*t + 1;

        ਦੁੱਗਣਾ sin_a = sin_ਸਾਰਣੀ[sin_idx];
        ਦੁੱਗਣਾ cos_a = sin_ਸਾਰਣੀ[cos_idx];

        ਦੁੱਗਣਾ sin_val = sin_a*cos_b + cos_a*sin_b;
        ਦੁੱਗਣਾ cos_val = cos_a*cos_b - sin_a*sin_b;

        ਜੇ (mag)
        {
            ਦੁੱਗਣਾ mag_val = mag[i];
            sin_val *= mag_val;
            cos_val *= mag_val;
        }

        sinval[i] = (ਦਸ਼ਮਲਵ)sin_val;
        cosval[i] = (ਦਸ਼ਮਲਵ)cos_val;
    }
}

} // anonymous::

ਖਾਲੀ polar_ਮੰਜ਼ਿਲ_Cart32f(ਅਚਲ ਦਸ਼ਮਲਵ *mag, ਅਚਲ ਦਸ਼ਮਲਵ *angle, ਦਸ਼ਮਲਵ *X, ਦਸ਼ਮਲਵ *Y, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees)
{
    CV_INSTRUMENT_REGION();
    SinCos_32f(mag, angle, X, Y, len, angle_ਵਿੱਚ_Degrees);
}

ਖਾਲੀ polar_ਮੰਜ਼ਿਲ_Cart64f(ਅਚਲ ਦੁੱਗਣਾ *mag, ਅਚਲ ਦੁੱਗਣਾ *angle, ਦੁੱਗਣਾ *X, ਦੁੱਗਣਾ *Y, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees)
{
    CV_INSTRUMENT_REGION();

    ਅਚਲ ਪੂਰਨ BLKSZ = 128;
    ਦਸ਼ਮਲਵ ybuf[BLKSZ], xbuf[BLKSZ], _mbuf[BLKSZ], abuf[BLKSZ];
    ਦਸ਼ਮਲਵ* mbuf = mag ? _mbuf : ਖਾਲੀ_ਸੂਚਕ;
    ਲਈ( ਪੂਰਨ i = 0; i < len; i += BLKSZ )
    {
        ਪੂਰਨ j, blksz = std::min(BLKSZ, len - i);
        ਲਈ( j = 0; j < blksz; j++ )
        {
            ਜੇ (mbuf)
                mbuf[j] = (ਦਸ਼ਮਲਵ)mag[i + j];
            abuf[j] = (ਦਸ਼ਮਲਵ)angle[i + j];
        }
        SinCos_32f(mbuf, abuf, xbuf, ybuf, blksz, angle_ਵਿੱਚ_Degrees);
        ਲਈ( j = 0; j < blksz; j++ )
            X[i + j] = xbuf[j];
        ਲਈ( j = 0; j < blksz; j++ )
            Y[i + j] = ybuf[j];
    }
}

ਸਥਿਰ ਖਾਲੀ fastAtan32f_(ਅਚਲ ਦਸ਼ਮਲਵ *Y, ਅਚਲ ਦਸ਼ਮਲਵ *X, ਦਸ਼ਮਲਵ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees )
{
    ਦਸ਼ਮਲਵ scale = angle_ਵਿੱਚ_Degrees ? 1.f : (ਦਸ਼ਮਲਵ)(CV_PI/180);
    ਪੂਰਨ i = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    ਲਈ( ; i < len; i += VECSZ*2 )
    {
        ਜੇ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            ਜੇ( i == 0 || angle == X || angle == Y )
                ਰੋਕੋ;
            i = len - VECSZ*2;
        }

        v_float32 y0 = vx_ਲੱਦੋ(Y + i);
        v_float32 x0 = vx_ਲੱਦੋ(X + i);
        v_float32 y1 = vx_ਲੱਦੋ(Y + i + VECSZ);
        v_float32 x1 = vx_ਲੱਦੋ(X + i + VECSZ);

        v_float32 r0 = v_mul(v_atan_f32(y0, x0), s);
        v_float32 r1 = v_mul(v_atan_f32(y1, x1), s);

        v_store(angle + i, r0);
        v_store(angle + i + VECSZ, r1);
    }
    vx_cleanup();
#ਜੇ_ਅੰਤ

    ਲਈ( ; i < len; i++ )
        angle[i] = atan_f32(Y[i], X[i])*scale;
}

ਖਾਲੀ fastAtan32f(ਅਚਲ ਦਸ਼ਮਲਵ *Y, ਅਚਲ ਦਸ਼ਮਲਵ *X, ਦਸ਼ਮਲਵ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f_(Y, X, angle, len, angle_ਵਿੱਚ_Degrees );
}

ਖਾਲੀ fastAtan64f(ਅਚਲ ਦੁੱਗਣਾ *Y, ਅਚਲ ਦੁੱਗਣਾ *X, ਦੁੱਗਣਾ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees)
{
    CV_INSTRUMENT_REGION();

    ਅਚਲ ਪੂਰਨ BLKSZ = 128;
    ਦਸ਼ਮਲਵ ybuf[BLKSZ], xbuf[BLKSZ], abuf[BLKSZ];
    ਲਈ( ਪੂਰਨ i = 0; i < len; i += BLKSZ )
    {
        ਪੂਰਨ j, blksz = std::min(BLKSZ, len - i);
        ਲਈ( j = 0; j < blksz; j++ )
        {
            ybuf[j] = (ਦਸ਼ਮਲਵ)Y[i + j];
            xbuf[j] = (ਦਸ਼ਮਲਵ)X[i + j];
        }
        fastAtan32f_(ybuf, xbuf, abuf, blksz, angle_ਵਿੱਚ_Degrees);
        ਲਈ( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

// deprecated
ਖਾਲੀ fastAtan2(ਅਚਲ ਦਸ਼ਮਲਵ *Y, ਅਚਲ ਦਸ਼ਮਲਵ *X, ਦਸ਼ਮਲਵ *angle, ਪੂਰਨ len, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f(Y, X, angle, len, angle_ਵਿੱਚ_Degrees);
}

ਖਾਲੀ magnitude32f(ਅਚਲ ਦਸ਼ਮਲਵ* x, ਅਚਲ ਦਸ਼ਮਲਵ* y, ਦਸ਼ਮਲਵ* mag, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ i = 0;

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float32>::vlanes();
    ਲਈ( ; i < len; i += VECSZ*2 )
    {
        ਜੇ( i + VECSZ*2 > len )
        {
            ਜੇ( i == 0 || mag == x || mag == y )
                ਰੋਕੋ;
            i = len - VECSZ*2;
        }
        v_float32 x0 = vx_ਲੱਦੋ(x + i), x1 = vx_ਲੱਦੋ(x + i + VECSZ);
        v_float32 y0 = vx_ਲੱਦੋ(y + i), y1 = vx_ਲੱਦੋ(y + i + VECSZ);
        x0 = v_sqrt(v_muladd(x0, x0, v_mul(y0, y0)));
        x1 = v_sqrt(v_muladd(x1, x1, v_mul(y1, y1)));
        v_store(mag + i, x0);
        v_store(mag + i + VECSZ, x1);
    }
    vx_cleanup();
#ਜੇ_ਅੰਤ

    ਲਈ( ; i < len; i++ )
    {
        ਦਸ਼ਮਲਵ x0 = x[i], y0 = y[i];
        mag[i] = std::ਵਰਗਮੂਲ(x0*x0 + y0*y0);
    }
}

ਖਾਲੀ magnitude64f(ਅਚਲ ਦੁੱਗਣਾ* x, ਅਚਲ ਦੁੱਗਣਾ* y, ਦੁੱਗਣਾ* mag, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ i = 0;

#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float64>::vlanes();
    ਲਈ( ; i < len; i += VECSZ*2 )
    {
        ਜੇ( i + VECSZ*2 > len )
        {
            ਜੇ( i == 0 || mag == x || mag == y )
                ਰੋਕੋ;
            i = len - VECSZ*2;
        }
        v_float64 x0 = vx_ਲੱਦੋ(x + i), x1 = vx_ਲੱਦੋ(x + i + VECSZ);
        v_float64 y0 = vx_ਲੱਦੋ(y + i), y1 = vx_ਲੱਦੋ(y + i + VECSZ);
        x0 = v_sqrt(v_muladd(x0, x0, v_mul(y0, y0)));
        x1 = v_sqrt(v_muladd(x1, x1, v_mul(y1, y1)));
        v_store(mag + i, x0);
        v_store(mag + i + VECSZ, x1);
    }
    vx_cleanup();
#ਜੇ_ਅੰਤ

    ਲਈ( ; i < len; i++ )
    {
        ਦੁੱਗਣਾ x0 = x[i], y0 = y[i];
        mag[i] = std::ਵਰਗਮੂਲ(x0*x0 + y0*y0);
    }
}


ਖਾਲੀ invSqrt32f(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ i = 0;

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float32>::vlanes();
    ਲਈ( ; i < len; i += VECSZ*2 )
    {
        ਜੇ( i + VECSZ*2 > len )
        {
            ਜੇ( i == 0 || src == dst )
                ਰੋਕੋ;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_ਲੱਦੋ(src + i), t1 = vx_ਲੱਦੋ(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#ਜੇ_ਅੰਤ

    ਲਈ( ; i < len; i++ )
        dst[i] = 1/std::ਵਰਗਮੂਲ(src[i]);
}


ਖਾਲੀ invSqrt64f(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();
    ਪੂਰਨ i = 0;

#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float64>::vlanes();
    ਲਈ ( ; i < len; i += VECSZ*2)
    {
        ਜੇ( i + VECSZ*2 > len )
        {
            ਜੇ( i == 0 || src == dst )
                ਰੋਕੋ;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_ਲੱਦੋ(src + i), t1 = vx_ਲੱਦੋ(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
#ਜੇ_ਅੰਤ

    ਲਈ( ; i < len; i++ )
        dst[i] = 1/std::ਵਰਗਮੂਲ(src[i]);
}


ਖਾਲੀ sqrt32f(ਅਚਲ ਦਸ਼ਮਲਵ* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ i = 0;

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float32>::vlanes();
    ਲਈ( ; i < len; i += VECSZ*2 )
    {
        ਜੇ( i + VECSZ*2 > len )
        {
            ਜੇ( i == 0 || src == dst )
                ਰੋਕੋ;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_ਲੱਦੋ(src + i), t1 = vx_ਲੱਦੋ(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#ਜੇ_ਅੰਤ

    ਲਈ( ; i < len; i++ )
        dst[i] = std::ਵਰਗਮੂਲ(src[i]);
}


ਖਾਲੀ sqrt64f(ਅਚਲ ਦੁੱਗਣਾ* src, ਦੁੱਗਣਾ* dst, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ i = 0;

#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<v_float64>::vlanes();
    ਲਈ( ; i < len; i += VECSZ*2 )
    {
        ਜੇ( i + VECSZ*2 > len )
        {
            ਜੇ( i == 0 || src == dst )
                ਰੋਕੋ;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_ਲੱਦੋ(src + i), t1 = vx_ਲੱਦੋ(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#ਜੇ_ਅੰਤ

    ਲਈ( ; i < len; i++ )
        dst[i] = std::ਵਰਗਮੂਲ(src[i]);
}

// Workaround for ICE in MSVS 2015 update 3 (issue #7795)
// CV_AVX is not used here, because generated code is faster in non-AVX mode.
// (tested with disabled IPP on i5-6300U)
#ਜੇ (defined _MSC_VER && _MSC_VER >= 1900 && (defined(_M_IX86) || defined(_M_X64))) || defined(__EMSCRIPTEN__)
ਖਾਲੀ exp32f(ਅਚਲ ਦਸ਼ਮਲਵ *src, ਦਸ਼ਮਲਵ *dst, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
    {
        dst[i] = std::ਘਾਤਾਂਕ(src[i]);
    }
}

ਖਾਲੀ exp64f(ਅਚਲ ਦੁੱਗਣਾ *src, ਦੁੱਗਣਾ *dst, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
    {
        dst[i] = std::ਘਾਤਾਂਕ(src[i]);
    }
}

ਖਾਲੀ log32f(ਅਚਲ ਦਸ਼ਮਲਵ *src, ਦਸ਼ਮਲਵ *dst, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
    {
        dst[i] = std::ਲਘੂਗਣਕ(src[i]);
    }
}
ਖਾਲੀ log64f(ਅਚਲ ਦੁੱਗਣਾ *src, ਦੁੱਗਣਾ *dst, ਪੂਰਨ n)
{
    CV_INSTRUMENT_REGION();

    ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
    {
        dst[i] = std::ਲਘੂਗਣਕ(src[i]);
    }
}
#ਨਹੀਂਤਾਂ

////////////////////////////////////// EXP /////////////////////////////////////


#ਜੇ_ਅੰਤ // issue 7795

ਦਸ਼ਮਲਵ fastAtan2( ਦਸ਼ਮਲਵ y, ਦਸ਼ਮਲਵ x )
{
    ਵਾਪਸ atan_f32(y, x);
}

#ਜੇ_ਅੰਤ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ

}} // namespace cv::hal
