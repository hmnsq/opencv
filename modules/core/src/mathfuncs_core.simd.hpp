// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "mathfuncs.hpp"

నామాకాశ cv { నామాకాశ hal {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
శూన్య cart_గమ్యం_Polar32f(స్థిరరాశి దశాంశం *X, స్థిరరాశి దశాంశం *Y, దశాంశం* mag, దశాంశం *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees);
శూన్య cart_గమ్యం_Polar64f(స్థిరరాశి ద్విగుణ *X, స్థిరరాశి ద్విగుణ *Y, ద్విగుణ* mag, ద్విగుణ *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees);
శూన్య polar_గమ్యం_Cart32f(స్థిరరాశి దశాంశం *mag, స్థిరరాశి దశాంశం *angle, దశాంశం *X, దశాంశం *Y, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees);
శూన్య polar_గమ్యం_Cart64f(స్థిరరాశి ద్విగుణ *mag, స్థిరరాశి ద్విగుణ *angle, ద్విగుణ *X, ద్విగుణ *Y, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees);
శూన్య fastAtan32f(స్థిరరాశి దశాంశం *Y, స్థిరరాశి దశాంశం *X, దశాంశం *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees);
శూన్య fastAtan64f(స్థిరరాశి ద్విగుణ *Y, స్థిరరాశి ద్విగుణ *X, ద్విగుణ *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees);
శూన్య fastAtan2(స్థిరరాశి దశాంశం *Y, స్థిరరాశి దశాంశం *X, దశాంశం *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees);
శూన్య magnitude32f(స్థిరరాశి దశాంశం* x, స్థిరరాశి దశాంశం* y, దశాంశం* mag, పూర్ణాంకం len);
శూన్య magnitude64f(స్థిరరాశి ద్విగుణ* x, స్థిరరాశి ద్విగుణ* y, ద్విగుణ* mag, పూర్ణాంకం len);
శూన్య invSqrt32f(స్థిరరాశి దశాంశం* src, దశాంశం* dst, పూర్ణాంకం len);
శూన్య invSqrt64f(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, పూర్ణాంకం len);
శూన్య sqrt32f(స్థిరరాశి దశాంశం* src, దశాంశం* dst, పూర్ణాంకం len);
శూన్య sqrt64f(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, పూర్ణాంకం len);
దశాంశం fastAtan2(దశాంశం y, దశాంశం x);

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ఉపయోగించి నామాకాశ std;
ఉపయోగించి నామాకాశ cv;

నామాకాశ {

స్థిర స్థిరరాశి దశాంశం atan2_p1 = 0.9997878412794807f*(దశాంశం)(180/CV_PI);
స్థిర స్థిరరాశి దశాంశం atan2_p3 = -0.3258083974640975f*(దశాంశం)(180/CV_PI);
స్థిర స్థిరరాశి దశాంశం atan2_p5 = 0.1555786518463281f*(దశాంశం)(180/CV_PI);
స్థిర స్థిరరాశి దశాంశం atan2_p7 = -0.04432655554792128f*(దశాంశం)(180/CV_PI);

#నిర్వచితమైతే __EMSCRIPTEN__
స్థిర అంతఃవరుస దశాంశం atan_f32(దశాంశం y, దశాంశం x)
{
    దశాంశం a = atan2(y, x) * 180.0f / CV_PI;
    ఒకవేళ (a < 0.0f)
        a += 360.0f;
    ఒకవేళ (a >= 360.0f)
        a -= 360.0f;
    తిరిగి a; // range [0; 360)
}
#లేకపోతే
స్థిర అంతఃవరుస దశాంశం atan_f32(దశాంశం y, దశాంశం x)
{
    దశాంశం ax = std::నిరపేక్ష(x), ay = std::నిరపేక్ష(y);
    దశాంశం a, c, c2;
    ఒకవేళ( ax >= ay )
    {
        c = ay/(ax + (దశాంశం)DBL_EPSILON);
        c2 = c*c;
        a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    లేకపోతే
    {
        c = ax/(ay + (దశాంశం)DBL_EPSILON);
        c2 = c*c;
        a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    ఒకవేళ( x < 0 )
        a = 180.f - a;
    ఒకవేళ( y < 0 )
        a = 360.f - a;
    తిరిగి a;
}
#అయితే_ముగింపు

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)

v_float32 v_atan_f32(స్థిరరాశి v_float32& y, స్థిరరాశి v_float32& x)
{
    v_float32 eps = vx_setall_f32((దశాంశం)DBL_EPSILON);
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
    v_float32 c = v_div(v_min(ax, ay), v_కూడు(v_max(ax, ay), eps));
    v_float32 cc = v_mul(c, c);
    v_float32 a = v_mul(v_fma(v_fma(v_fma(cc, p7, p5), cc, p3), cc, p1), c);
    a = v_select(v_ge(ax, ay), a, v_sub(val90, a));
    a = v_select(v_lt(x, z), v_sub(val180, a), a);
    a = v_select(v_lt(y, z), v_sub(val360, a), a);
    తిరిగి a;
}

#అయితే_ముగింపు

} // anonymous::

స్థిర శూన్య cart_గమ్యం_Polar32f_2(స్థిరరాశి దశాంశం *X, స్థిరరాశి దశాంశం *Y, దశాంశం *mag, దశాంశం *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees )
{
    దశాంశం scale = angle_లోపల_Degrees ? 1.f : (దశాంశం)(CV_PI/180);
    పూర్ణాంకం i = 0;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    కొరకు( ; i < len; i += VECSZ*2 )
    {
        ఒకవేళ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            ఒకవేళ( i == 0 || angle == X || angle == Y )
                ఆపు;
            i = len - VECSZ*2;
        }

        v_float32 x0 = vx_ఎక్కించు(X + i);
        v_float32 y0 = vx_ఎక్కించు(Y + i);
        v_float32 x1 = vx_ఎక్కించు(X + i + VECSZ);
        v_float32 y1 = vx_ఎక్కించు(Y + i + VECSZ);

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
#అయితే_ముగింపు

    కొరకు( ; i < len; i++ )
    {
        దశాంశం x0 = X[i], y0 = Y[i];
        mag[i] = std::వర్గమూలం(x0*x0 + y0*y0);
        angle[i] = atan_f32(y0, x0)*scale;
    }
}

శూన్య cart_గమ్యం_Polar32f(స్థిరరాశి దశాంశం *X, స్థిరరాశి దశాంశం *Y, దశాంశం *mag, దశాంశం *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees )
{
    CV_INSTRUMENT_REGION();
    cart_గమ్యం_Polar32f_2(X, Y, mag, angle, len, angle_లోపల_Degrees );
}

శూన్య cart_గమ్యం_Polar64f(స్థిరరాశి ద్విగుణ *X, స్థిరరాశి ద్విగుణ *Y, ద్విగుణ *mag, ద్విగుణ *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees)
{
    CV_INSTRUMENT_REGION();

    స్థిరరాశి పూర్ణాంకం BLKSZ = 128;
    దశాంశం ybuf[BLKSZ], xbuf[BLKSZ], mbuf[BLKSZ], abuf[BLKSZ];
    కొరకు( పూర్ణాంకం i = 0; i < len; i += BLKSZ )
    {
        పూర్ణాంకం j, blksz = std::min(BLKSZ, len - i);
        కొరకు( j = 0; j < blksz; j++ )
        {
            xbuf[j] = (దశాంశం)X[i + j];
            ybuf[j] = (దశాంశం)Y[i + j];
        }
        cart_గమ్యం_Polar32f_2(xbuf, ybuf, mbuf, abuf, blksz, angle_లోపల_Degrees);
        కొరకు( j = 0; j < blksz; j++ )
            mag[i + j] = mbuf[j];
        కొరకు( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

నామాకాశ {

స్థిర అంతఃవరుస శూన్య SinCos_32f(స్థిరరాశి దశాంశం* mag, స్థిరరాశి దశాంశం* angle, దశాంశం* cosval, దశాంశం* sinval, పూర్ణాంకం len, పూర్ణాంకం angle_లోపల_degrees)
{
    స్థిరరాశి పూర్ణాంకం N = 64;

    స్థిర స్థిరరాశి ద్విగుణ sin_పట్టిక[] =
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

    స్థిర స్థిరరాశి ద్విగుణ k2 = (2*CV_PI)/N;

    స్థిర స్థిరరాశి ద్విగుణ sin_a0 = -0.166630293345647*k2*k2*k2;
    స్థిర స్థిరరాశి ద్విగుణ sin_a2 = k2;

    స్థిర స్థిరరాశి ద్విగుణ cos_a0 = -0.499818138450326*k2*k2;
    /*static const double cos_a2 =  1;*/

    ద్విగుణ k1;
    పూర్ణాంకం i = 0;

    ఒకవేళ( !angle_లోపల_degrees )
        k1 = N/(2*CV_PI);
    లేకపోతే
        k1 = N/360.;

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<v_float32>::vlanes();
    స్థిరరాశి v_float32 scale = vx_setall_f32(angle_లోపల_degrees ? (దశాంశం)CV_PI / 180.f : 1.f);

    కొరకు( ; i < len; i += VECSZ*2 )
    {
        ఒకవేళ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            ఒకవేళ( i == 0 || angle == cosval || angle == sinval || mag == cosval || mag == sinval )
                ఆపు;
            i = len - VECSZ*2;
        }

        v_float32 r0 = v_mul(vx_ఎక్కించు(angle + i), scale);
        v_float32 r1 = v_mul(vx_ఎక్కించు(angle + i + VECSZ), scale);

        v_float32 c0, c1, s0, s1;
        v_sincos(r0, s0, c0);
        v_sincos(r1, s1, c1);

        ఒకవేళ( mag )
        {
            v_float32 m0 = vx_ఎక్కించు(mag + i);
            v_float32 m1 = vx_ఎక్కించు(mag + i + VECSZ);
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
#అయితే_ముగింపు

    కొరకు( ; i < len; i++ )
    {
        ద్విగుణ t = angle[i]*k1;
        పూర్ణాంకం it = cv_విడత(t);
        t -= it;
        పూర్ణాంకం sin_idx = it & (N - 1);
        పూర్ణాంకం cos_idx = (N/4 - sin_idx) & (N - 1);

        ద్విగుణ sin_b = (sin_a0*t*t + sin_a2)*t;
        ద్విగుణ cos_b = cos_a0*t*t + 1;

        ద్విగుణ sin_a = sin_పట్టిక[sin_idx];
        ద్విగుణ cos_a = sin_పట్టిక[cos_idx];

        ద్విగుణ sin_val = sin_a*cos_b + cos_a*sin_b;
        ద్విగుణ cos_val = cos_a*cos_b - sin_a*sin_b;

        ఒకవేళ (mag)
        {
            ద్విగుణ mag_val = mag[i];
            sin_val *= mag_val;
            cos_val *= mag_val;
        }

        sinval[i] = (దశాంశం)sin_val;
        cosval[i] = (దశాంశం)cos_val;
    }
}

} // anonymous::

శూన్య polar_గమ్యం_Cart32f(స్థిరరాశి దశాంశం *mag, స్థిరరాశి దశాంశం *angle, దశాంశం *X, దశాంశం *Y, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees)
{
    CV_INSTRUMENT_REGION();
    SinCos_32f(mag, angle, X, Y, len, angle_లోపల_Degrees);
}

శూన్య polar_గమ్యం_Cart64f(స్థిరరాశి ద్విగుణ *mag, స్థిరరాశి ద్విగుణ *angle, ద్విగుణ *X, ద్విగుణ *Y, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees)
{
    CV_INSTRUMENT_REGION();

    స్థిరరాశి పూర్ణాంకం BLKSZ = 128;
    దశాంశం ybuf[BLKSZ], xbuf[BLKSZ], _mbuf[BLKSZ], abuf[BLKSZ];
    దశాంశం* mbuf = mag ? _mbuf : శూన్యసూచిక;
    కొరకు( పూర్ణాంకం i = 0; i < len; i += BLKSZ )
    {
        పూర్ణాంకం j, blksz = std::min(BLKSZ, len - i);
        కొరకు( j = 0; j < blksz; j++ )
        {
            ఒకవేళ (mbuf)
                mbuf[j] = (దశాంశం)mag[i + j];
            abuf[j] = (దశాంశం)angle[i + j];
        }
        SinCos_32f(mbuf, abuf, xbuf, ybuf, blksz, angle_లోపల_Degrees);
        కొరకు( j = 0; j < blksz; j++ )
            X[i + j] = xbuf[j];
        కొరకు( j = 0; j < blksz; j++ )
            Y[i + j] = ybuf[j];
    }
}

స్థిర శూన్య fastAtan32f_(స్థిరరాశి దశాంశం *Y, స్థిరరాశి దశాంశం *X, దశాంశం *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees )
{
    దశాంశం scale = angle_లోపల_Degrees ? 1.f : (దశాంశం)(CV_PI/180);
    పూర్ణాంకం i = 0;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    కొరకు( ; i < len; i += VECSZ*2 )
    {
        ఒకవేళ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            ఒకవేళ( i == 0 || angle == X || angle == Y )
                ఆపు;
            i = len - VECSZ*2;
        }

        v_float32 y0 = vx_ఎక్కించు(Y + i);
        v_float32 x0 = vx_ఎక్కించు(X + i);
        v_float32 y1 = vx_ఎక్కించు(Y + i + VECSZ);
        v_float32 x1 = vx_ఎక్కించు(X + i + VECSZ);

        v_float32 r0 = v_mul(v_atan_f32(y0, x0), s);
        v_float32 r1 = v_mul(v_atan_f32(y1, x1), s);

        v_store(angle + i, r0);
        v_store(angle + i + VECSZ, r1);
    }
    vx_cleanup();
#అయితే_ముగింపు

    కొరకు( ; i < len; i++ )
        angle[i] = atan_f32(Y[i], X[i])*scale;
}

శూన్య fastAtan32f(స్థిరరాశి దశాంశం *Y, స్థిరరాశి దశాంశం *X, దశాంశం *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f_(Y, X, angle, len, angle_లోపల_Degrees );
}

శూన్య fastAtan64f(స్థిరరాశి ద్విగుణ *Y, స్థిరరాశి ద్విగుణ *X, ద్విగుణ *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees)
{
    CV_INSTRUMENT_REGION();

    స్థిరరాశి పూర్ణాంకం BLKSZ = 128;
    దశాంశం ybuf[BLKSZ], xbuf[BLKSZ], abuf[BLKSZ];
    కొరకు( పూర్ణాంకం i = 0; i < len; i += BLKSZ )
    {
        పూర్ణాంకం j, blksz = std::min(BLKSZ, len - i);
        కొరకు( j = 0; j < blksz; j++ )
        {
            ybuf[j] = (దశాంశం)Y[i + j];
            xbuf[j] = (దశాంశం)X[i + j];
        }
        fastAtan32f_(ybuf, xbuf, abuf, blksz, angle_లోపల_Degrees);
        కొరకు( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

// deprecated
శూన్య fastAtan2(స్థిరరాశి దశాంశం *Y, స్థిరరాశి దశాంశం *X, దశాంశం *angle, పూర్ణాంకం len, తార్కిక angle_లోపల_Degrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f(Y, X, angle, len, angle_లోపల_Degrees);
}

శూన్య magnitude32f(స్థిరరాశి దశాంశం* x, స్థిరరాశి దశాంశం* y, దశాంశం* mag, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం i = 0;

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<v_float32>::vlanes();
    కొరకు( ; i < len; i += VECSZ*2 )
    {
        ఒకవేళ( i + VECSZ*2 > len )
        {
            ఒకవేళ( i == 0 || mag == x || mag == y )
                ఆపు;
            i = len - VECSZ*2;
        }
        v_float32 x0 = vx_ఎక్కించు(x + i), x1 = vx_ఎక్కించు(x + i + VECSZ);
        v_float32 y0 = vx_ఎక్కించు(y + i), y1 = vx_ఎక్కించు(y + i + VECSZ);
        x0 = v_sqrt(v_muladd(x0, x0, v_mul(y0, y0)));
        x1 = v_sqrt(v_muladd(x1, x1, v_mul(y1, y1)));
        v_store(mag + i, x0);
        v_store(mag + i + VECSZ, x1);
    }
    vx_cleanup();
#అయితే_ముగింపు

    కొరకు( ; i < len; i++ )
    {
        దశాంశం x0 = x[i], y0 = y[i];
        mag[i] = std::వర్గమూలం(x0*x0 + y0*y0);
    }
}

శూన్య magnitude64f(స్థిరరాశి ద్విగుణ* x, స్థిరరాశి ద్విగుణ* y, ద్విగుణ* mag, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం i = 0;

#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<v_float64>::vlanes();
    కొరకు( ; i < len; i += VECSZ*2 )
    {
        ఒకవేళ( i + VECSZ*2 > len )
        {
            ఒకవేళ( i == 0 || mag == x || mag == y )
                ఆపు;
            i = len - VECSZ*2;
        }
        v_float64 x0 = vx_ఎక్కించు(x + i), x1 = vx_ఎక్కించు(x + i + VECSZ);
        v_float64 y0 = vx_ఎక్కించు(y + i), y1 = vx_ఎక్కించు(y + i + VECSZ);
        x0 = v_sqrt(v_muladd(x0, x0, v_mul(y0, y0)));
        x1 = v_sqrt(v_muladd(x1, x1, v_mul(y1, y1)));
        v_store(mag + i, x0);
        v_store(mag + i + VECSZ, x1);
    }
    vx_cleanup();
#అయితే_ముగింపు

    కొరకు( ; i < len; i++ )
    {
        ద్విగుణ x0 = x[i], y0 = y[i];
        mag[i] = std::వర్గమూలం(x0*x0 + y0*y0);
    }
}


శూన్య invSqrt32f(స్థిరరాశి దశాంశం* src, దశాంశం* dst, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం i = 0;

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<v_float32>::vlanes();
    కొరకు( ; i < len; i += VECSZ*2 )
    {
        ఒకవేళ( i + VECSZ*2 > len )
        {
            ఒకవేళ( i == 0 || src == dst )
                ఆపు;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_ఎక్కించు(src + i), t1 = vx_ఎక్కించు(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#అయితే_ముగింపు

    కొరకు( ; i < len; i++ )
        dst[i] = 1/std::వర్గమూలం(src[i]);
}


శూన్య invSqrt64f(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();
    పూర్ణాంకం i = 0;

#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<v_float64>::vlanes();
    కొరకు ( ; i < len; i += VECSZ*2)
    {
        ఒకవేళ( i + VECSZ*2 > len )
        {
            ఒకవేళ( i == 0 || src == dst )
                ఆపు;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_ఎక్కించు(src + i), t1 = vx_ఎక్కించు(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
#అయితే_ముగింపు

    కొరకు( ; i < len; i++ )
        dst[i] = 1/std::వర్గమూలం(src[i]);
}


శూన్య sqrt32f(స్థిరరాశి దశాంశం* src, దశాంశం* dst, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం i = 0;

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<v_float32>::vlanes();
    కొరకు( ; i < len; i += VECSZ*2 )
    {
        ఒకవేళ( i + VECSZ*2 > len )
        {
            ఒకవేళ( i == 0 || src == dst )
                ఆపు;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_ఎక్కించు(src + i), t1 = vx_ఎక్కించు(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#అయితే_ముగింపు

    కొరకు( ; i < len; i++ )
        dst[i] = std::వర్గమూలం(src[i]);
}


శూన్య sqrt64f(స్థిరరాశి ద్విగుణ* src, ద్విగుణ* dst, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం i = 0;

#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<v_float64>::vlanes();
    కొరకు( ; i < len; i += VECSZ*2 )
    {
        ఒకవేళ( i + VECSZ*2 > len )
        {
            ఒకవేళ( i == 0 || src == dst )
                ఆపు;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_ఎక్కించు(src + i), t1 = vx_ఎక్కించు(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#అయితే_ముగింపు

    కొరకు( ; i < len; i++ )
        dst[i] = std::వర్గమూలం(src[i]);
}

// Workaround for ICE in MSVS 2015 update 3 (issue #7795)
// CV_AVX is not used here, because generated code is faster in non-AVX mode.
// (tested with disabled IPP on i5-6300U)
#ఒకవేళ (defined _MSC_VER && _MSC_VER >= 1900 && (defined(_M_IX86) || defined(_M_X64))) || defined(__EMSCRIPTEN__)
శూన్య exp32f(స్థిరరాశి దశాంశం *src, దశాంశం *dst, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    కొరకు (పూర్ణాంకం i = 0; i < n; i++)
    {
        dst[i] = std::ఘాతాంకం(src[i]);
    }
}

శూన్య exp64f(స్థిరరాశి ద్విగుణ *src, ద్విగుణ *dst, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    కొరకు (పూర్ణాంకం i = 0; i < n; i++)
    {
        dst[i] = std::ఘాతాంకం(src[i]);
    }
}

శూన్య log32f(స్థిరరాశి దశాంశం *src, దశాంశం *dst, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    కొరకు (పూర్ణాంకం i = 0; i < n; i++)
    {
        dst[i] = std::సంవర్గమానం(src[i]);
    }
}
శూన్య log64f(స్థిరరాశి ద్విగుణ *src, ద్విగుణ *dst, పూర్ణాంకం n)
{
    CV_INSTRUMENT_REGION();

    కొరకు (పూర్ణాంకం i = 0; i < n; i++)
    {
        dst[i] = std::సంవర్గమానం(src[i]);
    }
}
#లేకపోతే

////////////////////////////////////// EXP /////////////////////////////////////


#అయితే_ముగింపు // issue 7795

దశాంశం fastAtan2( దశాంశం y, దశాంశం x )
{
    తిరిగి atan_f32(y, x);
}

#అయితే_ముగింపు // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_చివర

}} // namespace cv::hal
