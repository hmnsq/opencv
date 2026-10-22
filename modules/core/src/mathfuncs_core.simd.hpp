// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#અન્તર્ભાવ "mathfuncs.hpp"

નામાકાશ cv { નામાકાશ hal {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
શૂન્ય cartToPolar32f(અચલ દશાંશ *X, અચલ દશાંશ *Y, દશાંશ* mag, દશાંશ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees);
શૂન્ય cartToPolar64f(અચલ દ્વિગુણ *X, અચલ દ્વિગુણ *Y, દ્વિગુણ* mag, દ્વિગુણ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees);
શૂન્ય polarToCart32f(અચલ દશાંશ *mag, અચલ દશાંશ *angle, દશાંશ *X, દશાંશ *Y, પૂર્ણાંક len, તાર્કિક angleInDegrees);
શૂન્ય polarToCart64f(અચલ દ્વિગુણ *mag, અચલ દ્વિગુણ *angle, દ્વિગુણ *X, દ્વિગુણ *Y, પૂર્ણાંક len, તાર્કિક angleInDegrees);
શૂન્ય fastAtan32f(અચલ દશાંશ *Y, અચલ દશાંશ *X, દશાંશ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees);
શૂન્ય fastAtan64f(અચલ દ્વિગુણ *Y, અચલ દ્વિગુણ *X, દ્વિગુણ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees);
શૂન્ય fastAtan2(અચલ દશાંશ *Y, અચલ દશાંશ *X, દશાંશ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees);
શૂન્ય magnitude32f(અચલ દશાંશ* x, અચલ દશાંશ* y, દશાંશ* mag, પૂર્ણાંક len);
શૂન્ય magnitude64f(અચલ દ્વિગુણ* x, અચલ દ્વિગુણ* y, દ્વિગુણ* mag, પૂર્ણાંક len);
શૂન્ય invSqrt32f(અચલ દશાંશ* src, દશાંશ* dst, પૂર્ણાંક len);
શૂન્ય invSqrt64f(અચલ દ્વિગુણ* src, દ્વિગુણ* dst, પૂર્ણાંક len);
શૂન્ય sqrt32f(અચલ દશાંશ* src, દશાંશ* dst, પૂર્ણાંક len);
શૂન્ય sqrt64f(અચલ દ્વિગુણ* src, દ્વિગુણ* dst, પૂર્ણાંક len);
દશાંશ fastAtan2(દશાંશ y, દશાંશ x);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

using નામાકાશ std;
using નામાકાશ cv;

નામાકાશ {

સ્થિર અચલ દશાંશ atan2_p1 = 0.9997878412794807f*(દશાંશ)(180/CV_PI);
સ્થિર અચલ દશાંશ atan2_p3 = -0.3258083974640975f*(દશાંશ)(180/CV_PI);
સ્થિર અચલ દશાંશ atan2_p5 = 0.1555786518463281f*(દશાંશ)(180/CV_PI);
સ્થિર અચલ દશાંશ atan2_p7 = -0.04432655554792128f*(દશાંશ)(180/CV_PI);

#ifdef __EMSCRIPTEN__
સ્થિર અંતઃરેખ દશાંશ atan_f32(દશાંશ y, દશાંશ x)
{
    દશાંશ a = atan2(y, x) * 180.0f / CV_PI;
    જો (a < 0.0f)
        a += 360.0f;
    જો (a >= 360.0f)
        a -= 360.0f;
    પાછા a; // range [0; 360)
}
#નહીંતર
સ્થિર અંતઃરેખ દશાંશ atan_f32(દશાંશ y, દશાંશ x)
{
    દશાંશ ax = std::abs(x), ay = std::abs(y);
    દશાંશ a, c, c2;
    જો( ax >= ay )
    {
        c = ay/(ax + (દશાંશ)DBL_EPSILON);
        c2 = c*c;
        a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    નહીંતર
    {
        c = ax/(ay + (દશાંશ)DBL_EPSILON);
        c2 = c*c;
        a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    જો( x < 0 )
        a = 180.f - a;
    જો( y < 0 )
        a = 360.f - a;
    પાછા a;
}
#endif

#જો (CV_SIMD || CV_SIMD_SCALABLE)

v_float32 v_atan_f32(અચલ v_float32& y, અચલ v_float32& x)
{
    v_float32 eps = vx_setall_f32((દશાંશ)DBL_EPSILON);
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
    પાછા a;
}

#endif

} // anonymous::

સ્થિર શૂન્ય cartToPolar32f_(અચલ દશાંશ *X, અચલ દશાંશ *Y, દશાંશ *mag, દશાંશ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees )
{
    દશાંશ scale = angleInDegrees ? 1.f : (દશાંશ)(CV_PI/180);
    પૂર્ણાંક i = 0;
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    અચલ પૂર્ણાંક VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    માટે( ; i < len; i += VECSZ*2 )
    {
        જો( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            જો( i == 0 || angle == X || angle == Y )
                તોડો;
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

    માટે( ; i < len; i++ )
    {
        દશાંશ x0 = X[i], y0 = Y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
        angle[i] = atan_f32(y0, x0)*scale;
    }
}

શૂન્ય cartToPolar32f(અચલ દશાંશ *X, અચલ દશાંશ *Y, દશાંશ *mag, દશાંશ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    cartToPolar32f_(X, Y, mag, angle, len, angleInDegrees );
}

શૂન્ય cartToPolar64f(અચલ દ્વિગુણ *X, અચલ દ્વિગુણ *Y, દ્વિગુણ *mag, દ્વિગુણ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    અચલ પૂર્ણાંક BLKSZ = 128;
    દશાંશ ybuf[BLKSZ], xbuf[BLKSZ], mbuf[BLKSZ], abuf[BLKSZ];
    માટે( પૂર્ણાંક i = 0; i < len; i += BLKSZ )
    {
        પૂર્ણાંક j, blksz = std::min(BLKSZ, len - i);
        માટે( j = 0; j < blksz; j++ )
        {
            xbuf[j] = (દશાંશ)X[i + j];
            ybuf[j] = (દશાંશ)Y[i + j];
        }
        cartToPolar32f_(xbuf, ybuf, mbuf, abuf, blksz, angleInDegrees);
        માટે( j = 0; j < blksz; j++ )
            mag[i + j] = mbuf[j];
        માટે( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

નામાકાશ {

સ્થિર અંતઃરેખ શૂન્ય SinCos_32f(અચલ દશાંશ* mag, અચલ દશાંશ* angle, દશાંશ* cosval, દશાંશ* sinval, પૂર્ણાંક len, પૂર્ણાંક angle_in_degrees)
{
    અચલ પૂર્ણાંક N = 64;

    સ્થિર અચલ દ્વિગુણ sin_table[] =
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

    સ્થિર અચલ દ્વિગુણ k2 = (2*CV_PI)/N;

    સ્થિર અચલ દ્વિગુણ sin_a0 = -0.166630293345647*k2*k2*k2;
    સ્થિર અચલ દ્વિગુણ sin_a2 = k2;

    સ્થિર અચલ દ્વિગુણ cos_a0 = -0.499818138450326*k2*k2;
    /*static const double cos_a2 =  1;*/

    દ્વિગુણ k1;
    પૂર્ણાંક i = 0;

    જો( !angle_in_degrees )
        k1 = N/(2*CV_PI);
    નહીંતર
        k1 = N/360.;

#જો (CV_SIMD || CV_SIMD_SCALABLE)
    અચલ પૂર્ણાંક VECSZ = VTraits<v_float32>::vlanes();
    અચલ v_float32 scale = vx_setall_f32(angle_in_degrees ? (દશાંશ)CV_PI / 180.f : 1.f);

    માટે( ; i < len; i += VECSZ*2 )
    {
        જો( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            જો( i == 0 || angle == cosval || angle == sinval || mag == cosval || mag == sinval )
                તોડો;
            i = len - VECSZ*2;
        }

        v_float32 r0 = v_mul(vx_load(angle + i), scale);
        v_float32 r1 = v_mul(vx_load(angle + i + VECSZ), scale);

        v_float32 c0, c1, s0, s1;
        v_sincos(r0, s0, c0);
        v_sincos(r1, s1, c1);

        જો( mag )
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

    માટે( ; i < len; i++ )
    {
        દ્વિગુણ t = angle[i]*k1;
        પૂર્ણાંક it = cvRound(t);
        t -= it;
        પૂર્ણાંક sin_idx = it & (N - 1);
        પૂર્ણાંક cos_idx = (N/4 - sin_idx) & (N - 1);

        દ્વિગુણ sin_b = (sin_a0*t*t + sin_a2)*t;
        દ્વિગુણ cos_b = cos_a0*t*t + 1;

        દ્વિગુણ sin_a = sin_table[sin_idx];
        દ્વિગુણ cos_a = sin_table[cos_idx];

        દ્વિગુણ sin_val = sin_a*cos_b + cos_a*sin_b;
        દ્વિગુણ cos_val = cos_a*cos_b - sin_a*sin_b;

        જો (mag)
        {
            દ્વિગુણ mag_val = mag[i];
            sin_val *= mag_val;
            cos_val *= mag_val;
        }

        sinval[i] = (દશાંશ)sin_val;
        cosval[i] = (દશાંશ)cos_val;
    }
}

} // anonymous::

શૂન્ય polarToCart32f(અચલ દશાંશ *mag, અચલ દશાંશ *angle, દશાંશ *X, દશાંશ *Y, પૂર્ણાંક len, તાર્કિક angleInDegrees)
{
    CV_INSTRUMENT_REGION();
    SinCos_32f(mag, angle, X, Y, len, angleInDegrees);
}

શૂન્ય polarToCart64f(અચલ દ્વિગુણ *mag, અચલ દ્વિગુણ *angle, દ્વિગુણ *X, દ્વિગુણ *Y, પૂર્ણાંક len, તાર્કિક angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    અચલ પૂર્ણાંક BLKSZ = 128;
    દશાંશ ybuf[BLKSZ], xbuf[BLKSZ], _mbuf[BLKSZ], abuf[BLKSZ];
    દશાંશ* mbuf = mag ? _mbuf : nullptr;
    માટે( પૂર્ણાંક i = 0; i < len; i += BLKSZ )
    {
        પૂર્ણાંક j, blksz = std::min(BLKSZ, len - i);
        માટે( j = 0; j < blksz; j++ )
        {
            જો (mbuf)
                mbuf[j] = (દશાંશ)mag[i + j];
            abuf[j] = (દશાંશ)angle[i + j];
        }
        SinCos_32f(mbuf, abuf, xbuf, ybuf, blksz, angleInDegrees);
        માટે( j = 0; j < blksz; j++ )
            X[i + j] = xbuf[j];
        માટે( j = 0; j < blksz; j++ )
            Y[i + j] = ybuf[j];
    }
}

સ્થિર શૂન્ય fastAtan32f_(અચલ દશાંશ *Y, અચલ દશાંશ *X, દશાંશ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees )
{
    દશાંશ scale = angleInDegrees ? 1.f : (દશાંશ)(CV_PI/180);
    પૂર્ણાંક i = 0;
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    અચલ પૂર્ણાંક VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    માટે( ; i < len; i += VECSZ*2 )
    {
        જો( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            જો( i == 0 || angle == X || angle == Y )
                તોડો;
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

    માટે( ; i < len; i++ )
        angle[i] = atan_f32(Y[i], X[i])*scale;
}

શૂન્ય fastAtan32f(અચલ દશાંશ *Y, અચલ દશાંશ *X, દશાંશ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f_(Y, X, angle, len, angleInDegrees );
}

શૂન્ય fastAtan64f(અચલ દ્વિગુણ *Y, અચલ દ્વિગુણ *X, દ્વિગુણ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    અચલ પૂર્ણાંક BLKSZ = 128;
    દશાંશ ybuf[BLKSZ], xbuf[BLKSZ], abuf[BLKSZ];
    માટે( પૂર્ણાંક i = 0; i < len; i += BLKSZ )
    {
        પૂર્ણાંક j, blksz = std::min(BLKSZ, len - i);
        માટે( j = 0; j < blksz; j++ )
        {
            ybuf[j] = (દશાંશ)Y[i + j];
            xbuf[j] = (દશાંશ)X[i + j];
        }
        fastAtan32f_(ybuf, xbuf, abuf, blksz, angleInDegrees);
        માટે( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

// deprecated
શૂન્ય fastAtan2(અચલ દશાંશ *Y, અચલ દશાંશ *X, દશાંશ *angle, પૂર્ણાંક len, તાર્કિક angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

શૂન્ય magnitude32f(અચલ દશાંશ* x, અચલ દશાંશ* y, દશાંશ* mag, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક i = 0;

#જો (CV_SIMD || CV_SIMD_SCALABLE)
    અચલ પૂર્ણાંક VECSZ = VTraits<v_float32>::vlanes();
    માટે( ; i < len; i += VECSZ*2 )
    {
        જો( i + VECSZ*2 > len )
        {
            જો( i == 0 || mag == x || mag == y )
                તોડો;
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

    માટે( ; i < len; i++ )
    {
        દશાંશ x0 = x[i], y0 = y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
    }
}

શૂન્ય magnitude64f(અચલ દ્વિગુણ* x, અચલ દ્વિગુણ* y, દ્વિગુણ* mag, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક i = 0;

#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    અચલ પૂર્ણાંક VECSZ = VTraits<v_float64>::vlanes();
    માટે( ; i < len; i += VECSZ*2 )
    {
        જો( i + VECSZ*2 > len )
        {
            જો( i == 0 || mag == x || mag == y )
                તોડો;
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

    માટે( ; i < len; i++ )
    {
        દ્વિગુણ x0 = x[i], y0 = y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
    }
}


શૂન્ય invSqrt32f(અચલ દશાંશ* src, દશાંશ* dst, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક i = 0;

#જો (CV_SIMD || CV_SIMD_SCALABLE)
    અચલ પૂર્ણાંક VECSZ = VTraits<v_float32>::vlanes();
    માટે( ; i < len; i += VECSZ*2 )
    {
        જો( i + VECSZ*2 > len )
        {
            જો( i == 0 || src == dst )
                તોડો;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    માટે( ; i < len; i++ )
        dst[i] = 1/std::sqrt(src[i]);
}


શૂન્ય invSqrt64f(અચલ દ્વિગુણ* src, દ્વિગુણ* dst, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();
    પૂર્ણાંક i = 0;

#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    અચલ પૂર્ણાંક VECSZ = VTraits<v_float64>::vlanes();
    માટે ( ; i < len; i += VECSZ*2)
    {
        જો( i + VECSZ*2 > len )
        {
            જો( i == 0 || src == dst )
                તોડો;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
#endif

    માટે( ; i < len; i++ )
        dst[i] = 1/std::sqrt(src[i]);
}


શૂન્ય sqrt32f(અચલ દશાંશ* src, દશાંશ* dst, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક i = 0;

#જો (CV_SIMD || CV_SIMD_SCALABLE)
    અચલ પૂર્ણાંક VECSZ = VTraits<v_float32>::vlanes();
    માટે( ; i < len; i += VECSZ*2 )
    {
        જો( i + VECSZ*2 > len )
        {
            જો( i == 0 || src == dst )
                તોડો;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    માટે( ; i < len; i++ )
        dst[i] = std::sqrt(src[i]);
}


શૂન્ય sqrt64f(અચલ દ્વિગુણ* src, દ્વિગુણ* dst, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક i = 0;

#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    અચલ પૂર્ણાંક VECSZ = VTraits<v_float64>::vlanes();
    માટે( ; i < len; i += VECSZ*2 )
    {
        જો( i + VECSZ*2 > len )
        {
            જો( i == 0 || src == dst )
                તોડો;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    માટે( ; i < len; i++ )
        dst[i] = std::sqrt(src[i]);
}

// Workaround for ICE in MSVS 2015 update 3 (issue #7795)
// CV_AVX is not used here, because generated code is faster in non-AVX mode.
// (tested with disabled IPP on i5-6300U)
#જો (defined _MSC_VER && _MSC_VER >= 1900 && (defined(_M_IX86) || defined(_M_X64))) || defined(__EMSCRIPTEN__)
શૂન્ય exp32f(અચલ દશાંશ *src, દશાંશ *dst, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    માટે (પૂર્ણાંક i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

શૂન્ય exp64f(અચલ દ્વિગુણ *src, દ્વિગુણ *dst, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    માટે (પૂર્ણાંક i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

શૂન્ય log32f(અચલ દશાંશ *src, દશાંશ *dst, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    માટે (પૂર્ણાંક i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
શૂન્ય log64f(અચલ દ્વિગુણ *src, દ્વિગુણ *dst, પૂર્ણાંક n)
{
    CV_INSTRUMENT_REGION();

    માટે (પૂર્ણાંક i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
#નહીંતર

////////////////////////////////////// EXP /////////////////////////////////////


#endif // issue 7795

દશાંશ fastAtan2( દશાંશ y, દશાંશ x )
{
    પાછા atan_f32(y, x);
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

}} // namespace cv::hal
