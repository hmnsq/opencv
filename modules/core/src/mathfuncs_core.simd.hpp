// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "mathfuncs.hpp"

നാമാകാശ cv { നാമാകാശ hal {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
ശൂന്യം cartToPolar32f(സ്ഥിരം ദശാംശം *X, സ്ഥിരം ദശാംശം *Y, ദശാംശം* mag, ദശാംശം *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees);
ശൂന്യം cartToPolar64f(സ്ഥിരം ഇരട്ട *X, സ്ഥിരം ഇരട്ട *Y, ഇരട്ട* mag, ഇരട്ട *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees);
ശൂന്യം polarToCart32f(സ്ഥിരം ദശാംശം *mag, സ്ഥിരം ദശാംശം *angle, ദശാംശം *X, ദശാംശം *Y, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees);
ശൂന്യം polarToCart64f(സ്ഥിരം ഇരട്ട *mag, സ്ഥിരം ഇരട്ട *angle, ഇരട്ട *X, ഇരട്ട *Y, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees);
ശൂന്യം fastAtan32f(സ്ഥിരം ദശാംശം *Y, സ്ഥിരം ദശാംശം *X, ദശാംശം *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees);
ശൂന്യം fastAtan64f(സ്ഥിരം ഇരട്ട *Y, സ്ഥിരം ഇരട്ട *X, ഇരട്ട *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees);
ശൂന്യം fastAtan2(സ്ഥിരം ദശാംശം *Y, സ്ഥിരം ദശാംശം *X, ദശാംശം *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees);
ശൂന്യം magnitude32f(സ്ഥിരം ദശാംശം* x, സ്ഥിരം ദശാംശം* y, ദശാംശം* mag, പൂർണ്ണസംഖ്യ len);
ശൂന്യം magnitude64f(സ്ഥിരം ഇരട്ട* x, സ്ഥിരം ഇരട്ട* y, ഇരട്ട* mag, പൂർണ്ണസംഖ്യ len);
ശൂന്യം invSqrt32f(സ്ഥിരം ദശാംശം* src, ദശാംശം* dst, പൂർണ്ണസംഖ്യ len);
ശൂന്യം invSqrt64f(സ്ഥിരം ഇരട്ട* src, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ len);
ശൂന്യം sqrt32f(സ്ഥിരം ദശാംശം* src, ദശാംശം* dst, പൂർണ്ണസംഖ്യ len);
ശൂന്യം sqrt64f(സ്ഥിരം ഇരട്ട* src, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ len);
ദശാംശം fastAtan2(ദശാംശം y, ദശാംശം x);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

using നാമാകാശ std;
using നാമാകാശ cv;

നാമാകാശ {

സ്ഥിതം സ്ഥിരം ദശാംശം atan2_p1 = 0.9997878412794807f*(ദശാംശം)(180/CV_PI);
സ്ഥിതം സ്ഥിരം ദശാംശം atan2_p3 = -0.3258083974640975f*(ദശാംശം)(180/CV_PI);
സ്ഥിതം സ്ഥിരം ദശാംശം atan2_p5 = 0.1555786518463281f*(ദശാംശം)(180/CV_PI);
സ്ഥിതം സ്ഥിരം ദശാംശം atan2_p7 = -0.04432655554792128f*(ദശാംശം)(180/CV_PI);

#ifdef __EMSCRIPTEN__
സ്ഥിതം വരിയിൽ ദശാംശം atan_f32(ദശാംശം y, ദശാംശം x)
{
    ദശാംശം a = atan2(y, x) * 180.0f / CV_PI;
    എങ്കിൽ (a < 0.0f)
        a += 360.0f;
    എങ്കിൽ (a >= 360.0f)
        a -= 360.0f;
    തിരികെ a; // range [0; 360)
}
#അല്ലെങ്കിൽ
സ്ഥിതം വരിയിൽ ദശാംശം atan_f32(ദശാംശം y, ദശാംശം x)
{
    ദശാംശം ax = std::abs(x), ay = std::abs(y);
    ദശാംശം a, c, c2;
    എങ്കിൽ( ax >= ay )
    {
        c = ay/(ax + (ദശാംശം)DBL_EPSILON);
        c2 = c*c;
        a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    അല്ലെങ്കിൽ
    {
        c = ax/(ay + (ദശാംശം)DBL_EPSILON);
        c2 = c*c;
        a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    എങ്കിൽ( x < 0 )
        a = 180.f - a;
    എങ്കിൽ( y < 0 )
        a = 360.f - a;
    തിരികെ a;
}
#endif

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)

v_float32 v_atan_f32(സ്ഥിരം v_float32& y, സ്ഥിരം v_float32& x)
{
    v_float32 eps = vx_setall_f32((ദശാംശം)DBL_EPSILON);
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
    തിരികെ a;
}

#endif

} // anonymous::

സ്ഥിതം ശൂന്യം cartToPolar32f_(സ്ഥിരം ദശാംശം *X, സ്ഥിരം ദശാംശം *Y, ദശാംശം *mag, ദശാംശം *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees )
{
    ദശാംശം scale = angleInDegrees ? 1.f : (ദശാംശം)(CV_PI/180);
    പൂർണ്ണസംഖ്യ i = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    വേണ്ടി( ; i < len; i += VECSZ*2 )
    {
        എങ്കിൽ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            എങ്കിൽ( i == 0 || angle == X || angle == Y )
                നിർത്തുക;
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

    വേണ്ടി( ; i < len; i++ )
    {
        ദശാംശം x0 = X[i], y0 = Y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
        angle[i] = atan_f32(y0, x0)*scale;
    }
}

ശൂന്യം cartToPolar32f(സ്ഥിരം ദശാംശം *X, സ്ഥിരം ദശാംശം *Y, ദശാംശം *mag, ദശാംശം *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    cartToPolar32f_(X, Y, mag, angle, len, angleInDegrees );
}

ശൂന്യം cartToPolar64f(സ്ഥിരം ഇരട്ട *X, സ്ഥിരം ഇരട്ട *Y, ഇരട്ട *mag, ഇരട്ട *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    സ്ഥിരം പൂർണ്ണസംഖ്യ BLKSZ = 128;
    ദശാംശം ybuf[BLKSZ], xbuf[BLKSZ], mbuf[BLKSZ], abuf[BLKSZ];
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i += BLKSZ )
    {
        പൂർണ്ണസംഖ്യ j, blksz = std::min(BLKSZ, len - i);
        വേണ്ടി( j = 0; j < blksz; j++ )
        {
            xbuf[j] = (ദശാംശം)X[i + j];
            ybuf[j] = (ദശാംശം)Y[i + j];
        }
        cartToPolar32f_(xbuf, ybuf, mbuf, abuf, blksz, angleInDegrees);
        വേണ്ടി( j = 0; j < blksz; j++ )
            mag[i + j] = mbuf[j];
        വേണ്ടി( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

നാമാകാശ {

സ്ഥിതം വരിയിൽ ശൂന്യം SinCos_32f(സ്ഥിരം ദശാംശം* mag, സ്ഥിരം ദശാംശം* angle, ദശാംശം* cosval, ദശാംശം* sinval, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ angle_in_degrees)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ N = 64;

    സ്ഥിതം സ്ഥിരം ഇരട്ട sin_table[] =
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

    സ്ഥിതം സ്ഥിരം ഇരട്ട k2 = (2*CV_PI)/N;

    സ്ഥിതം സ്ഥിരം ഇരട്ട sin_a0 = -0.166630293345647*k2*k2*k2;
    സ്ഥിതം സ്ഥിരം ഇരട്ട sin_a2 = k2;

    സ്ഥിതം സ്ഥിരം ഇരട്ട cos_a0 = -0.499818138450326*k2*k2;
    /*static const double cos_a2 =  1;*/

    ഇരട്ട k1;
    പൂർണ്ണസംഖ്യ i = 0;

    എങ്കിൽ( !angle_in_degrees )
        k1 = N/(2*CV_PI);
    അല്ലെങ്കിൽ
        k1 = N/360.;

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<v_float32>::vlanes();
    സ്ഥിരം v_float32 scale = vx_setall_f32(angle_in_degrees ? (ദശാംശം)CV_PI / 180.f : 1.f);

    വേണ്ടി( ; i < len; i += VECSZ*2 )
    {
        എങ്കിൽ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            എങ്കിൽ( i == 0 || angle == cosval || angle == sinval || mag == cosval || mag == sinval )
                നിർത്തുക;
            i = len - VECSZ*2;
        }

        v_float32 r0 = v_mul(vx_load(angle + i), scale);
        v_float32 r1 = v_mul(vx_load(angle + i + VECSZ), scale);

        v_float32 c0, c1, s0, s1;
        v_sincos(r0, s0, c0);
        v_sincos(r1, s1, c1);

        എങ്കിൽ( mag )
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

    വേണ്ടി( ; i < len; i++ )
    {
        ഇരട്ട t = angle[i]*k1;
        പൂർണ്ണസംഖ്യ it = cvRound(t);
        t -= it;
        പൂർണ്ണസംഖ്യ sin_idx = it & (N - 1);
        പൂർണ്ണസംഖ്യ cos_idx = (N/4 - sin_idx) & (N - 1);

        ഇരട്ട sin_b = (sin_a0*t*t + sin_a2)*t;
        ഇരട്ട cos_b = cos_a0*t*t + 1;

        ഇരട്ട sin_a = sin_table[sin_idx];
        ഇരട്ട cos_a = sin_table[cos_idx];

        ഇരട്ട sin_val = sin_a*cos_b + cos_a*sin_b;
        ഇരട്ട cos_val = cos_a*cos_b - sin_a*sin_b;

        എങ്കിൽ (mag)
        {
            ഇരട്ട mag_val = mag[i];
            sin_val *= mag_val;
            cos_val *= mag_val;
        }

        sinval[i] = (ദശാംശം)sin_val;
        cosval[i] = (ദശാംശം)cos_val;
    }
}

} // anonymous::

ശൂന്യം polarToCart32f(സ്ഥിരം ദശാംശം *mag, സ്ഥിരം ദശാംശം *angle, ദശാംശം *X, ദശാംശം *Y, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees)
{
    CV_INSTRUMENT_REGION();
    SinCos_32f(mag, angle, X, Y, len, angleInDegrees);
}

ശൂന്യം polarToCart64f(സ്ഥിരം ഇരട്ട *mag, സ്ഥിരം ഇരട്ട *angle, ഇരട്ട *X, ഇരട്ട *Y, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    സ്ഥിരം പൂർണ്ണസംഖ്യ BLKSZ = 128;
    ദശാംശം ybuf[BLKSZ], xbuf[BLKSZ], _mbuf[BLKSZ], abuf[BLKSZ];
    ദശാംശം* mbuf = mag ? _mbuf : nullptr;
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i += BLKSZ )
    {
        പൂർണ്ണസംഖ്യ j, blksz = std::min(BLKSZ, len - i);
        വേണ്ടി( j = 0; j < blksz; j++ )
        {
            എങ്കിൽ (mbuf)
                mbuf[j] = (ദശാംശം)mag[i + j];
            abuf[j] = (ദശാംശം)angle[i + j];
        }
        SinCos_32f(mbuf, abuf, xbuf, ybuf, blksz, angleInDegrees);
        വേണ്ടി( j = 0; j < blksz; j++ )
            X[i + j] = xbuf[j];
        വേണ്ടി( j = 0; j < blksz; j++ )
            Y[i + j] = ybuf[j];
    }
}

സ്ഥിതം ശൂന്യം fastAtan32f_(സ്ഥിരം ദശാംശം *Y, സ്ഥിരം ദശാംശം *X, ദശാംശം *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees )
{
    ദശാംശം scale = angleInDegrees ? 1.f : (ദശാംശം)(CV_PI/180);
    പൂർണ്ണസംഖ്യ i = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    വേണ്ടി( ; i < len; i += VECSZ*2 )
    {
        എങ്കിൽ( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            എങ്കിൽ( i == 0 || angle == X || angle == Y )
                നിർത്തുക;
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

    വേണ്ടി( ; i < len; i++ )
        angle[i] = atan_f32(Y[i], X[i])*scale;
}

ശൂന്യം fastAtan32f(സ്ഥിരം ദശാംശം *Y, സ്ഥിരം ദശാംശം *X, ദശാംശം *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f_(Y, X, angle, len, angleInDegrees );
}

ശൂന്യം fastAtan64f(സ്ഥിരം ഇരട്ട *Y, സ്ഥിരം ഇരട്ട *X, ഇരട്ട *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    സ്ഥിരം പൂർണ്ണസംഖ്യ BLKSZ = 128;
    ദശാംശം ybuf[BLKSZ], xbuf[BLKSZ], abuf[BLKSZ];
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i += BLKSZ )
    {
        പൂർണ്ണസംഖ്യ j, blksz = std::min(BLKSZ, len - i);
        വേണ്ടി( j = 0; j < blksz; j++ )
        {
            ybuf[j] = (ദശാംശം)Y[i + j];
            xbuf[j] = (ദശാംശം)X[i + j];
        }
        fastAtan32f_(ybuf, xbuf, abuf, blksz, angleInDegrees);
        വേണ്ടി( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

// deprecated
ശൂന്യം fastAtan2(സ്ഥിരം ദശാംശം *Y, സ്ഥിരം ദശാംശം *X, ദശാംശം *angle, പൂർണ്ണസംഖ്യ len, തർക്കം angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

ശൂന്യം magnitude32f(സ്ഥിരം ദശാംശം* x, സ്ഥിരം ദശാംശം* y, ദശാംശം* mag, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ i = 0;

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<v_float32>::vlanes();
    വേണ്ടി( ; i < len; i += VECSZ*2 )
    {
        എങ്കിൽ( i + VECSZ*2 > len )
        {
            എങ്കിൽ( i == 0 || mag == x || mag == y )
                നിർത്തുക;
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

    വേണ്ടി( ; i < len; i++ )
    {
        ദശാംശം x0 = x[i], y0 = y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
    }
}

ശൂന്യം magnitude64f(സ്ഥിരം ഇരട്ട* x, സ്ഥിരം ഇരട്ട* y, ഇരട്ട* mag, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ i = 0;

#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<v_float64>::vlanes();
    വേണ്ടി( ; i < len; i += VECSZ*2 )
    {
        എങ്കിൽ( i + VECSZ*2 > len )
        {
            എങ്കിൽ( i == 0 || mag == x || mag == y )
                നിർത്തുക;
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

    വേണ്ടി( ; i < len; i++ )
    {
        ഇരട്ട x0 = x[i], y0 = y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
    }
}


ശൂന്യം invSqrt32f(സ്ഥിരം ദശാംശം* src, ദശാംശം* dst, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ i = 0;

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<v_float32>::vlanes();
    വേണ്ടി( ; i < len; i += VECSZ*2 )
    {
        എങ്കിൽ( i + VECSZ*2 > len )
        {
            എങ്കിൽ( i == 0 || src == dst )
                നിർത്തുക;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    വേണ്ടി( ; i < len; i++ )
        dst[i] = 1/std::sqrt(src[i]);
}


ശൂന്യം invSqrt64f(സ്ഥിരം ഇരട്ട* src, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();
    പൂർണ്ണസംഖ്യ i = 0;

#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<v_float64>::vlanes();
    വേണ്ടി ( ; i < len; i += VECSZ*2)
    {
        എങ്കിൽ( i + VECSZ*2 > len )
        {
            എങ്കിൽ( i == 0 || src == dst )
                നിർത്തുക;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
#endif

    വേണ്ടി( ; i < len; i++ )
        dst[i] = 1/std::sqrt(src[i]);
}


ശൂന്യം sqrt32f(സ്ഥിരം ദശാംശം* src, ദശാംശം* dst, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ i = 0;

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<v_float32>::vlanes();
    വേണ്ടി( ; i < len; i += VECSZ*2 )
    {
        എങ്കിൽ( i + VECSZ*2 > len )
        {
            എങ്കിൽ( i == 0 || src == dst )
                നിർത്തുക;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    വേണ്ടി( ; i < len; i++ )
        dst[i] = std::sqrt(src[i]);
}


ശൂന്യം sqrt64f(സ്ഥിരം ഇരട്ട* src, ഇരട്ട* dst, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ i = 0;

#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<v_float64>::vlanes();
    വേണ്ടി( ; i < len; i += VECSZ*2 )
    {
        എങ്കിൽ( i + VECSZ*2 > len )
        {
            എങ്കിൽ( i == 0 || src == dst )
                നിർത്തുക;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    വേണ്ടി( ; i < len; i++ )
        dst[i] = std::sqrt(src[i]);
}

// Workaround for ICE in MSVS 2015 update 3 (issue #7795)
// CV_AVX is not used here, because generated code is faster in non-AVX mode.
// (tested with disabled IPP on i5-6300U)
#എങ്കിൽ (defined _MSC_VER && _MSC_VER >= 1900 && (defined(_M_IX86) || defined(_M_X64))) || defined(__EMSCRIPTEN__)
ശൂന്യം exp32f(സ്ഥിരം ദശാംശം *src, ദശാംശം *dst, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

ശൂന്യം exp64f(സ്ഥിരം ഇരട്ട *src, ഇരട്ട *dst, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

ശൂന്യം log32f(സ്ഥിരം ദശാംശം *src, ദശാംശം *dst, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
ശൂന്യം log64f(സ്ഥിരം ഇരട്ട *src, ഇരട്ട *dst, പൂർണ്ണസംഖ്യ n)
{
    CV_INSTRUMENT_REGION();

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
#അല്ലെങ്കിൽ

////////////////////////////////////// EXP /////////////////////////////////////


#endif // issue 7795

ദശാംശം fastAtan2( ദശാംശം y, ദശാംശം x )
{
    തിരികെ atan_f32(y, x);
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

}} // namespace cv::hal
