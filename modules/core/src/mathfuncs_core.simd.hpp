// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "mathfuncs.hpp"

نطاق cv { نطاق hal {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
فراغ cartToPolar32f(ثابت عائم *X, ثابت عائم *Y, عائم* mag, عائم *angle, صحيح len, منطقي angleInDegrees);
فراغ cartToPolar64f(ثابت مزدوج *X, ثابت مزدوج *Y, مزدوج* mag, مزدوج *angle, صحيح len, منطقي angleInDegrees);
فراغ polarToCart32f(ثابت عائم *mag, ثابت عائم *angle, عائم *X, عائم *Y, صحيح len, منطقي angleInDegrees);
فراغ polarToCart64f(ثابت مزدوج *mag, ثابت مزدوج *angle, مزدوج *X, مزدوج *Y, صحيح len, منطقي angleInDegrees);
فراغ fastAtan32f(ثابت عائم *Y, ثابت عائم *X, عائم *angle, صحيح len, منطقي angleInDegrees);
فراغ fastAtan64f(ثابت مزدوج *Y, ثابت مزدوج *X, مزدوج *angle, صحيح len, منطقي angleInDegrees);
فراغ fastAtan2(ثابت عائم *Y, ثابت عائم *X, عائم *angle, صحيح len, منطقي angleInDegrees);
فراغ magnitude32f(ثابت عائم* x, ثابت عائم* y, عائم* mag, صحيح len);
فراغ magnitude64f(ثابت مزدوج* x, ثابت مزدوج* y, مزدوج* mag, صحيح len);
فراغ invSqrt32f(ثابت عائم* src, عائم* dst, صحيح len);
فراغ invSqrt64f(ثابت مزدوج* src, مزدوج* dst, صحيح len);
فراغ sqrt32f(ثابت عائم* src, عائم* dst, صحيح len);
فراغ sqrt64f(ثابت مزدوج* src, مزدوج* dst, صحيح len);
عائم fastAtan2(عائم y, عائم x);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

using نطاق std;
using نطاق cv;

نطاق {

ساكن ثابت عائم atan2_p1 = 0.9997878412794807f*(عائم)(180/CV_PI);
ساكن ثابت عائم atan2_p3 = -0.3258083974640975f*(عائم)(180/CV_PI);
ساكن ثابت عائم atan2_p5 = 0.1555786518463281f*(عائم)(180/CV_PI);
ساكن ثابت عائم atan2_p7 = -0.04432655554792128f*(عائم)(180/CV_PI);

#ifdef __EMSCRIPTEN__
ساكن مضمن عائم atan_f32(عائم y, عائم x)
{
    عائم a = atan2(y, x) * 180.0f / CV_PI;
    𐡄𐡍 (a < 0.0f)
        a += 360.0f;
    𐡄𐡍 (a >= 360.0f)
        a -= 360.0f;
    أرجع a; // range [0; 360)
}
#وإلا
ساكن مضمن عائم atan_f32(عائم y, عائم x)
{
    عائم ax = std::abs(x), ay = std::abs(y);
    عائم a, c, c2;
    𐡄𐡍( ax >= ay )
    {
        c = ay/(ax + (عائم)DBL_EPSILON);
        c2 = c*c;
        a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    وإلا
    {
        c = ax/(ay + (عائم)DBL_EPSILON);
        c2 = c*c;
        a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    𐡄𐡍( x < 0 )
        a = 180.f - a;
    𐡄𐡍( y < 0 )
        a = 360.f - a;
    أرجع a;
}
#endif

#𐡄𐡍 (CV_SIMD || CV_SIMD_SCALABLE)

v_float32 v_atan_f32(ثابت v_float32& y, ثابت v_float32& x)
{
    v_float32 eps = vx_setall_f32((عائم)DBL_EPSILON);
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
    أرجع a;
}

#endif

} // anonymous::

ساكن فراغ cartToPolar32f_(ثابت عائم *X, ثابت عائم *Y, عائم *mag, عائم *angle, صحيح len, منطقي angleInDegrees )
{
    عائم scale = angleInDegrees ? 1.f : (عائم)(CV_PI/180);
    صحيح i = 0;
#𐡄𐡍 (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    لكل( ; i < len; i += VECSZ*2 )
    {
        𐡄𐡍( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            𐡄𐡍( i == 0 || angle == X || angle == Y )
                اكسر;
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

    لكل( ; i < len; i++ )
    {
        عائم x0 = X[i], y0 = Y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
        angle[i] = atan_f32(y0, x0)*scale;
    }
}

فراغ cartToPolar32f(ثابت عائم *X, ثابت عائم *Y, عائم *mag, عائم *angle, صحيح len, منطقي angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    cartToPolar32f_(X, Y, mag, angle, len, angleInDegrees );
}

فراغ cartToPolar64f(ثابت مزدوج *X, ثابت مزدوج *Y, مزدوج *mag, مزدوج *angle, صحيح len, منطقي angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    ثابت صحيح BLKSZ = 128;
    عائم ybuf[BLKSZ], xbuf[BLKSZ], mbuf[BLKSZ], abuf[BLKSZ];
    لكل( صحيح i = 0; i < len; i += BLKSZ )
    {
        صحيح j, blksz = std::min(BLKSZ, len - i);
        لكل( j = 0; j < blksz; j++ )
        {
            xbuf[j] = (عائم)X[i + j];
            ybuf[j] = (عائم)Y[i + j];
        }
        cartToPolar32f_(xbuf, ybuf, mbuf, abuf, blksz, angleInDegrees);
        لكل( j = 0; j < blksz; j++ )
            mag[i + j] = mbuf[j];
        لكل( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

نطاق {

ساكن مضمن فراغ SinCos_32f(ثابت عائم* mag, ثابت عائم* angle, عائم* cosval, عائم* sinval, صحيح len, صحيح angle_in_degrees)
{
    ثابت صحيح N = 64;

    ساكن ثابت مزدوج sin_table[] =
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

    ساكن ثابت مزدوج k2 = (2*CV_PI)/N;

    ساكن ثابت مزدوج sin_a0 = -0.166630293345647*k2*k2*k2;
    ساكن ثابت مزدوج sin_a2 = k2;

    ساكن ثابت مزدوج cos_a0 = -0.499818138450326*k2*k2;
    /*static const double cos_a2 =  1;*/

    مزدوج k1;
    صحيح i = 0;

    𐡄𐡍( !angle_in_degrees )
        k1 = N/(2*CV_PI);
    وإلا
        k1 = N/360.;

#𐡄𐡍 (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح VECSZ = VTraits<v_float32>::vlanes();
    ثابت v_float32 scale = vx_setall_f32(angle_in_degrees ? (عائم)CV_PI / 180.f : 1.f);

    لكل( ; i < len; i += VECSZ*2 )
    {
        𐡄𐡍( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            𐡄𐡍( i == 0 || angle == cosval || angle == sinval || mag == cosval || mag == sinval )
                اكسر;
            i = len - VECSZ*2;
        }

        v_float32 r0 = v_mul(vx_load(angle + i), scale);
        v_float32 r1 = v_mul(vx_load(angle + i + VECSZ), scale);

        v_float32 c0, c1, s0, s1;
        v_sincos(r0, s0, c0);
        v_sincos(r1, s1, c1);

        𐡄𐡍( mag )
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

    لكل( ; i < len; i++ )
    {
        مزدوج t = angle[i]*k1;
        صحيح it = cvRound(t);
        t -= it;
        صحيح sin_idx = it & (N - 1);
        صحيح cos_idx = (N/4 - sin_idx) & (N - 1);

        مزدوج sin_b = (sin_a0*t*t + sin_a2)*t;
        مزدوج cos_b = cos_a0*t*t + 1;

        مزدوج sin_a = sin_table[sin_idx];
        مزدوج cos_a = sin_table[cos_idx];

        مزدوج sin_val = sin_a*cos_b + cos_a*sin_b;
        مزدوج cos_val = cos_a*cos_b - sin_a*sin_b;

        𐡄𐡍 (mag)
        {
            مزدوج mag_val = mag[i];
            sin_val *= mag_val;
            cos_val *= mag_val;
        }

        sinval[i] = (عائم)sin_val;
        cosval[i] = (عائم)cos_val;
    }
}

} // anonymous::

فراغ polarToCart32f(ثابت عائم *mag, ثابت عائم *angle, عائم *X, عائم *Y, صحيح len, منطقي angleInDegrees)
{
    CV_INSTRUMENT_REGION();
    SinCos_32f(mag, angle, X, Y, len, angleInDegrees);
}

فراغ polarToCart64f(ثابت مزدوج *mag, ثابت مزدوج *angle, مزدوج *X, مزدوج *Y, صحيح len, منطقي angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    ثابت صحيح BLKSZ = 128;
    عائم ybuf[BLKSZ], xbuf[BLKSZ], _mbuf[BLKSZ], abuf[BLKSZ];
    عائم* mbuf = mag ? _mbuf : nullptr;
    لكل( صحيح i = 0; i < len; i += BLKSZ )
    {
        صحيح j, blksz = std::min(BLKSZ, len - i);
        لكل( j = 0; j < blksz; j++ )
        {
            𐡄𐡍 (mbuf)
                mbuf[j] = (عائم)mag[i + j];
            abuf[j] = (عائم)angle[i + j];
        }
        SinCos_32f(mbuf, abuf, xbuf, ybuf, blksz, angleInDegrees);
        لكل( j = 0; j < blksz; j++ )
            X[i + j] = xbuf[j];
        لكل( j = 0; j < blksz; j++ )
            Y[i + j] = ybuf[j];
    }
}

ساكن فراغ fastAtan32f_(ثابت عائم *Y, ثابت عائم *X, عائم *angle, صحيح len, منطقي angleInDegrees )
{
    عائم scale = angleInDegrees ? 1.f : (عائم)(CV_PI/180);
    صحيح i = 0;
#𐡄𐡍 (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    لكل( ; i < len; i += VECSZ*2 )
    {
        𐡄𐡍( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            𐡄𐡍( i == 0 || angle == X || angle == Y )
                اكسر;
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

    لكل( ; i < len; i++ )
        angle[i] = atan_f32(Y[i], X[i])*scale;
}

فراغ fastAtan32f(ثابت عائم *Y, ثابت عائم *X, عائم *angle, صحيح len, منطقي angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f_(Y, X, angle, len, angleInDegrees );
}

فراغ fastAtan64f(ثابت مزدوج *Y, ثابت مزدوج *X, مزدوج *angle, صحيح len, منطقي angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    ثابت صحيح BLKSZ = 128;
    عائم ybuf[BLKSZ], xbuf[BLKSZ], abuf[BLKSZ];
    لكل( صحيح i = 0; i < len; i += BLKSZ )
    {
        صحيح j, blksz = std::min(BLKSZ, len - i);
        لكل( j = 0; j < blksz; j++ )
        {
            ybuf[j] = (عائم)Y[i + j];
            xbuf[j] = (عائم)X[i + j];
        }
        fastAtan32f_(ybuf, xbuf, abuf, blksz, angleInDegrees);
        لكل( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

// deprecated
فراغ fastAtan2(ثابت عائم *Y, ثابت عائم *X, عائم *angle, صحيح len, منطقي angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

فراغ magnitude32f(ثابت عائم* x, ثابت عائم* y, عائم* mag, صحيح len)
{
    CV_INSTRUMENT_REGION();

    صحيح i = 0;

#𐡄𐡍 (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح VECSZ = VTraits<v_float32>::vlanes();
    لكل( ; i < len; i += VECSZ*2 )
    {
        𐡄𐡍( i + VECSZ*2 > len )
        {
            𐡄𐡍( i == 0 || mag == x || mag == y )
                اكسر;
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

    لكل( ; i < len; i++ )
    {
        عائم x0 = x[i], y0 = y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
    }
}

فراغ magnitude64f(ثابت مزدوج* x, ثابت مزدوج* y, مزدوج* mag, صحيح len)
{
    CV_INSTRUMENT_REGION();

    صحيح i = 0;

#𐡄𐡍 (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ثابت صحيح VECSZ = VTraits<v_float64>::vlanes();
    لكل( ; i < len; i += VECSZ*2 )
    {
        𐡄𐡍( i + VECSZ*2 > len )
        {
            𐡄𐡍( i == 0 || mag == x || mag == y )
                اكسر;
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

    لكل( ; i < len; i++ )
    {
        مزدوج x0 = x[i], y0 = y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
    }
}


فراغ invSqrt32f(ثابت عائم* src, عائم* dst, صحيح len)
{
    CV_INSTRUMENT_REGION();

    صحيح i = 0;

#𐡄𐡍 (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح VECSZ = VTraits<v_float32>::vlanes();
    لكل( ; i < len; i += VECSZ*2 )
    {
        𐡄𐡍( i + VECSZ*2 > len )
        {
            𐡄𐡍( i == 0 || src == dst )
                اكسر;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    لكل( ; i < len; i++ )
        dst[i] = 1/std::sqrt(src[i]);
}


فراغ invSqrt64f(ثابت مزدوج* src, مزدوج* dst, صحيح len)
{
    CV_INSTRUMENT_REGION();
    صحيح i = 0;

#𐡄𐡍 (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ثابت صحيح VECSZ = VTraits<v_float64>::vlanes();
    لكل ( ; i < len; i += VECSZ*2)
    {
        𐡄𐡍( i + VECSZ*2 > len )
        {
            𐡄𐡍( i == 0 || src == dst )
                اكسر;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
#endif

    لكل( ; i < len; i++ )
        dst[i] = 1/std::sqrt(src[i]);
}


فراغ sqrt32f(ثابت عائم* src, عائم* dst, صحيح len)
{
    CV_INSTRUMENT_REGION();

    صحيح i = 0;

#𐡄𐡍 (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحيح VECSZ = VTraits<v_float32>::vlanes();
    لكل( ; i < len; i += VECSZ*2 )
    {
        𐡄𐡍( i + VECSZ*2 > len )
        {
            𐡄𐡍( i == 0 || src == dst )
                اكسر;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    لكل( ; i < len; i++ )
        dst[i] = std::sqrt(src[i]);
}


فراغ sqrt64f(ثابت مزدوج* src, مزدوج* dst, صحيح len)
{
    CV_INSTRUMENT_REGION();

    صحيح i = 0;

#𐡄𐡍 (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ثابت صحيح VECSZ = VTraits<v_float64>::vlanes();
    لكل( ; i < len; i += VECSZ*2 )
    {
        𐡄𐡍( i + VECSZ*2 > len )
        {
            𐡄𐡍( i == 0 || src == dst )
                اكسر;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    لكل( ; i < len; i++ )
        dst[i] = std::sqrt(src[i]);
}

// Workaround for ICE in MSVS 2015 update 3 (issue #7795)
// CV_AVX is not used here, because generated code is faster in non-AVX mode.
// (tested with disabled IPP on i5-6300U)
#𐡄𐡍 (defined _MSC_VER && _MSC_VER >= 1900 && (defined(_M_IX86) || defined(_M_X64))) || defined(__EMSCRIPTEN__)
فراغ exp32f(ثابت عائم *src, عائم *dst, صحيح n)
{
    CV_INSTRUMENT_REGION();

    لكل (صحيح i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

فراغ exp64f(ثابت مزدوج *src, مزدوج *dst, صحيح n)
{
    CV_INSTRUMENT_REGION();

    لكل (صحيح i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

فراغ log32f(ثابت عائم *src, عائم *dst, صحيح n)
{
    CV_INSTRUMENT_REGION();

    لكل (صحيح i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
فراغ log64f(ثابت مزدوج *src, مزدوج *dst, صحيح n)
{
    CV_INSTRUMENT_REGION();

    لكل (صحيح i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
#وإلا

////////////////////////////////////// EXP /////////////////////////////////////


#endif // issue 7795

عائم fastAtan2( عائم y, عائم x )
{
    أرجع atan_f32(y, x);
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

}} // namespace cv::hal
