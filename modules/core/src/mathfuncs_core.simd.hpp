// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "mathfuncs.hpp"

नामाकाश cv { नामाकाश hal {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
शून्य cartToPolar32f(स्थिरांक दशांश *X, स्थिरांक दशांश *Y, दशांश* mag, दशांश *angle, पूर्णांक len, तार्किक angleInDegrees);
शून्य cartToPolar64f(स्थिरांक द्विगुण *X, स्थिरांक द्विगुण *Y, द्विगुण* mag, द्विगुण *angle, पूर्णांक len, तार्किक angleInDegrees);
शून्य polarToCart32f(स्थिरांक दशांश *mag, स्थिरांक दशांश *angle, दशांश *X, दशांश *Y, पूर्णांक len, तार्किक angleInDegrees);
शून्य polarToCart64f(स्थिरांक द्विगुण *mag, स्थिरांक द्विगुण *angle, द्विगुण *X, द्विगुण *Y, पूर्णांक len, तार्किक angleInDegrees);
शून्य fastAtan32f(स्थिरांक दशांश *Y, स्थिरांक दशांश *X, दशांश *angle, पूर्णांक len, तार्किक angleInDegrees);
शून्य fastAtan64f(स्थिरांक द्विगुण *Y, स्थिरांक द्विगुण *X, द्विगुण *angle, पूर्णांक len, तार्किक angleInDegrees);
शून्य fastAtan2(स्थिरांक दशांश *Y, स्थिरांक दशांश *X, दशांश *angle, पूर्णांक len, तार्किक angleInDegrees);
शून्य magnitude32f(स्थिरांक दशांश* x, स्थिरांक दशांश* y, दशांश* mag, पूर्णांक len);
शून्य magnitude64f(स्थिरांक द्विगुण* x, स्थिरांक द्विगुण* y, द्विगुण* mag, पूर्णांक len);
शून्य invSqrt32f(स्थिरांक दशांश* src, दशांश* dst, पूर्णांक len);
शून्य invSqrt64f(स्थिरांक द्विगुण* src, द्विगुण* dst, पूर्णांक len);
शून्य sqrt32f(स्थिरांक दशांश* src, दशांश* dst, पूर्णांक len);
शून्य sqrt64f(स्थिरांक द्विगुण* src, द्विगुण* dst, पूर्णांक len);
दशांश fastAtan2(दशांश y, दशांश x);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

using नामाकाश std;
using नामाकाश cv;

नामाकाश {

स्थिर स्थिरांक दशांश atan2_p1 = 0.9997878412794807f*(दशांश)(180/CV_PI);
स्थिर स्थिरांक दशांश atan2_p3 = -0.3258083974640975f*(दशांश)(180/CV_PI);
स्थिर स्थिरांक दशांश atan2_p5 = 0.1555786518463281f*(दशांश)(180/CV_PI);
स्थिर स्थिरांक दशांश atan2_p7 = -0.04432655554792128f*(दशांश)(180/CV_PI);

#ifdef __EMSCRIPTEN__
स्थिर ओळीत दशांश atan_f32(दशांश y, दशांश x)
{
    दशांश a = atan2(y, x) * 180.0f / CV_PI;
    जर (a < 0.0f)
        a += 360.0f;
    जर (a >= 360.0f)
        a -= 360.0f;
    परत a; // range [0; 360)
}
#अन्यथा
स्थिर ओळीत दशांश atan_f32(दशांश y, दशांश x)
{
    दशांश ax = std::असल(x), ay = std::असल(y);
    दशांश a, c, c2;
    जर( ax >= ay )
    {
        c = ay/(ax + (दशांश)द्विग_अंतर);
        c2 = c*c;
        a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    अन्यथा
    {
        c = ax/(ay + (दशांश)द्विग_अंतर);
        c2 = c*c;
        a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    जर( x < 0 )
        a = 180.f - a;
    जर( y < 0 )
        a = 360.f - a;
    परत a;
}
#endif

#जर (CV_SIMD || CV_SIMD_SCALABLE)

v_float32 v_atan_f32(स्थिरांक v_float32& y, स्थिरांक v_float32& x)
{
    v_float32 eps = vx_setall_f32((दशांश)द्विग_अंतर);
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
    परत a;
}

#endif

} // anonymous::

स्थिर शून्य cartToPolar32f_(स्थिरांक दशांश *X, स्थिरांक दशांश *Y, दशांश *mag, दशांश *angle, पूर्णांक len, तार्किक angleInDegrees )
{
    दशांश scale = angleInDegrees ? 1.f : (दशांश)(CV_PI/180);
    पूर्णांक i = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिरांक पूर्णांक VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    साठी( ; i < len; i += VECSZ*2 )
    {
        जर( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            जर( i == 0 || angle == X || angle == Y )
                थांबा;
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

    साठी( ; i < len; i++ )
    {
        दशांश x0 = X[i], y0 = Y[i];
        mag[i] = std::वर्ग_मूल(x0*x0 + y0*y0);
        angle[i] = atan_f32(y0, x0)*scale;
    }
}

शून्य cartToPolar32f(स्थिरांक दशांश *X, स्थिरांक दशांश *Y, दशांश *mag, दशांश *angle, पूर्णांक len, तार्किक angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    cartToPolar32f_(X, Y, mag, angle, len, angleInDegrees );
}

शून्य cartToPolar64f(स्थिरांक द्विगुण *X, स्थिरांक द्विगुण *Y, द्विगुण *mag, द्विगुण *angle, पूर्णांक len, तार्किक angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    स्थिरांक पूर्णांक BLKSZ = 128;
    दशांश ybuf[BLKSZ], xbuf[BLKSZ], mbuf[BLKSZ], abuf[BLKSZ];
    साठी( पूर्णांक i = 0; i < len; i += BLKSZ )
    {
        पूर्णांक j, blksz = std::min(BLKSZ, len - i);
        साठी( j = 0; j < blksz; j++ )
        {
            xbuf[j] = (दशांश)X[i + j];
            ybuf[j] = (दशांश)Y[i + j];
        }
        cartToPolar32f_(xbuf, ybuf, mbuf, abuf, blksz, angleInDegrees);
        साठी( j = 0; j < blksz; j++ )
            mag[i + j] = mbuf[j];
        साठी( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

नामाकाश {

स्थिर ओळीत शून्य SinCos_32f(स्थिरांक दशांश* mag, स्थिरांक दशांश* angle, दशांश* cosval, दशांश* sinval, पूर्णांक len, पूर्णांक angle_in_degrees)
{
    स्थिरांक पूर्णांक N = 64;

    स्थिर स्थिरांक द्विगुण sin_table[] =
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

    स्थिर स्थिरांक द्विगुण k2 = (2*CV_PI)/N;

    स्थिर स्थिरांक द्विगुण sin_a0 = -0.166630293345647*k2*k2*k2;
    स्थिर स्थिरांक द्विगुण sin_a2 = k2;

    स्थिर स्थिरांक द्विगुण cos_a0 = -0.499818138450326*k2*k2;
    /*static const double cos_a2 =  1;*/

    द्विगुण k1;
    पूर्णांक i = 0;

    जर( !angle_in_degrees )
        k1 = N/(2*CV_PI);
    अन्यथा
        k1 = N/360.;

#जर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिरांक पूर्णांक VECSZ = VTraits<v_float32>::vlanes();
    स्थिरांक v_float32 scale = vx_setall_f32(angle_in_degrees ? (दशांश)CV_PI / 180.f : 1.f);

    साठी( ; i < len; i += VECSZ*2 )
    {
        जर( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            जर( i == 0 || angle == cosval || angle == sinval || mag == cosval || mag == sinval )
                थांबा;
            i = len - VECSZ*2;
        }

        v_float32 r0 = v_mul(vx_load(angle + i), scale);
        v_float32 r1 = v_mul(vx_load(angle + i + VECSZ), scale);

        v_float32 c0, c1, s0, s1;
        v_sincos(r0, s0, c0);
        v_sincos(r1, s1, c1);

        जर( mag )
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

    साठी( ; i < len; i++ )
    {
        द्विगुण t = angle[i]*k1;
        पूर्णांक it = cvRound(t);
        t -= it;
        पूर्णांक sin_idx = it & (N - 1);
        पूर्णांक cos_idx = (N/4 - sin_idx) & (N - 1);

        द्विगुण sin_b = (sin_a0*t*t + sin_a2)*t;
        द्विगुण cos_b = cos_a0*t*t + 1;

        द्विगुण sin_a = sin_table[sin_idx];
        द्विगुण cos_a = sin_table[cos_idx];

        द्विगुण sin_val = sin_a*cos_b + cos_a*sin_b;
        द्विगुण cos_val = cos_a*cos_b - sin_a*sin_b;

        जर (mag)
        {
            द्विगुण mag_val = mag[i];
            sin_val *= mag_val;
            cos_val *= mag_val;
        }

        sinval[i] = (दशांश)sin_val;
        cosval[i] = (दशांश)cos_val;
    }
}

} // anonymous::

शून्य polarToCart32f(स्थिरांक दशांश *mag, स्थिरांक दशांश *angle, दशांश *X, दशांश *Y, पूर्णांक len, तार्किक angleInDegrees)
{
    CV_INSTRUMENT_REGION();
    SinCos_32f(mag, angle, X, Y, len, angleInDegrees);
}

शून्य polarToCart64f(स्थिरांक द्विगुण *mag, स्थिरांक द्विगुण *angle, द्विगुण *X, द्विगुण *Y, पूर्णांक len, तार्किक angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    स्थिरांक पूर्णांक BLKSZ = 128;
    दशांश ybuf[BLKSZ], xbuf[BLKSZ], _mbuf[BLKSZ], abuf[BLKSZ];
    दशांश* mbuf = mag ? _mbuf : nullptr;
    साठी( पूर्णांक i = 0; i < len; i += BLKSZ )
    {
        पूर्णांक j, blksz = std::min(BLKSZ, len - i);
        साठी( j = 0; j < blksz; j++ )
        {
            जर (mbuf)
                mbuf[j] = (दशांश)mag[i + j];
            abuf[j] = (दशांश)angle[i + j];
        }
        SinCos_32f(mbuf, abuf, xbuf, ybuf, blksz, angleInDegrees);
        साठी( j = 0; j < blksz; j++ )
            X[i + j] = xbuf[j];
        साठी( j = 0; j < blksz; j++ )
            Y[i + j] = ybuf[j];
    }
}

स्थिर शून्य fastAtan32f_(स्थिरांक दशांश *Y, स्थिरांक दशांश *X, दशांश *angle, पूर्णांक len, तार्किक angleInDegrees )
{
    दशांश scale = angleInDegrees ? 1.f : (दशांश)(CV_PI/180);
    पूर्णांक i = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिरांक पूर्णांक VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    साठी( ; i < len; i += VECSZ*2 )
    {
        जर( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            जर( i == 0 || angle == X || angle == Y )
                थांबा;
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

    साठी( ; i < len; i++ )
        angle[i] = atan_f32(Y[i], X[i])*scale;
}

शून्य fastAtan32f(स्थिरांक दशांश *Y, स्थिरांक दशांश *X, दशांश *angle, पूर्णांक len, तार्किक angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f_(Y, X, angle, len, angleInDegrees );
}

शून्य fastAtan64f(स्थिरांक द्विगुण *Y, स्थिरांक द्विगुण *X, द्विगुण *angle, पूर्णांक len, तार्किक angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    स्थिरांक पूर्णांक BLKSZ = 128;
    दशांश ybuf[BLKSZ], xbuf[BLKSZ], abuf[BLKSZ];
    साठी( पूर्णांक i = 0; i < len; i += BLKSZ )
    {
        पूर्णांक j, blksz = std::min(BLKSZ, len - i);
        साठी( j = 0; j < blksz; j++ )
        {
            ybuf[j] = (दशांश)Y[i + j];
            xbuf[j] = (दशांश)X[i + j];
        }
        fastAtan32f_(ybuf, xbuf, abuf, blksz, angleInDegrees);
        साठी( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

// deprecated
शून्य fastAtan2(स्थिरांक दशांश *Y, स्थिरांक दशांश *X, दशांश *angle, पूर्णांक len, तार्किक angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

शून्य magnitude32f(स्थिरांक दशांश* x, स्थिरांक दशांश* y, दशांश* mag, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक i = 0;

#जर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिरांक पूर्णांक VECSZ = VTraits<v_float32>::vlanes();
    साठी( ; i < len; i += VECSZ*2 )
    {
        जर( i + VECSZ*2 > len )
        {
            जर( i == 0 || mag == x || mag == y )
                थांबा;
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

    साठी( ; i < len; i++ )
    {
        दशांश x0 = x[i], y0 = y[i];
        mag[i] = std::वर्ग_मूल(x0*x0 + y0*y0);
    }
}

शून्य magnitude64f(स्थिरांक द्विगुण* x, स्थिरांक द्विगुण* y, द्विगुण* mag, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक i = 0;

#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    स्थिरांक पूर्णांक VECSZ = VTraits<v_float64>::vlanes();
    साठी( ; i < len; i += VECSZ*2 )
    {
        जर( i + VECSZ*2 > len )
        {
            जर( i == 0 || mag == x || mag == y )
                थांबा;
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

    साठी( ; i < len; i++ )
    {
        द्विगुण x0 = x[i], y0 = y[i];
        mag[i] = std::वर्ग_मूल(x0*x0 + y0*y0);
    }
}


शून्य invSqrt32f(स्थिरांक दशांश* src, दशांश* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक i = 0;

#जर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिरांक पूर्णांक VECSZ = VTraits<v_float32>::vlanes();
    साठी( ; i < len; i += VECSZ*2 )
    {
        जर( i + VECSZ*2 > len )
        {
            जर( i == 0 || src == dst )
                थांबा;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    साठी( ; i < len; i++ )
        dst[i] = 1/std::वर्ग_मूल(src[i]);
}


शून्य invSqrt64f(स्थिरांक द्विगुण* src, द्विगुण* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    पूर्णांक i = 0;

#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    स्थिरांक पूर्णांक VECSZ = VTraits<v_float64>::vlanes();
    साठी ( ; i < len; i += VECSZ*2)
    {
        जर( i + VECSZ*2 > len )
        {
            जर( i == 0 || src == dst )
                थांबा;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
#endif

    साठी( ; i < len; i++ )
        dst[i] = 1/std::वर्ग_मूल(src[i]);
}


शून्य sqrt32f(स्थिरांक दशांश* src, दशांश* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक i = 0;

#जर (CV_SIMD || CV_SIMD_SCALABLE)
    स्थिरांक पूर्णांक VECSZ = VTraits<v_float32>::vlanes();
    साठी( ; i < len; i += VECSZ*2 )
    {
        जर( i + VECSZ*2 > len )
        {
            जर( i == 0 || src == dst )
                थांबा;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    साठी( ; i < len; i++ )
        dst[i] = std::वर्ग_मूल(src[i]);
}


शून्य sqrt64f(स्थिरांक द्विगुण* src, द्विगुण* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक i = 0;

#जर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    स्थिरांक पूर्णांक VECSZ = VTraits<v_float64>::vlanes();
    साठी( ; i < len; i += VECSZ*2 )
    {
        जर( i + VECSZ*2 > len )
        {
            जर( i == 0 || src == dst )
                थांबा;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    साठी( ; i < len; i++ )
        dst[i] = std::वर्ग_मूल(src[i]);
}

// Workaround for ICE in MSVS 2015 update 3 (issue #7795)
// CV_AVX is not used here, because generated code is faster in non-AVX mode.
// (tested with disabled IPP on i5-6300U)
#जर (defined _MSC_VER && _MSC_VER >= 1900 && (defined(_M_IX86) || defined(_M_X64))) || defined(__EMSCRIPTEN__)
शून्य exp32f(स्थिरांक दशांश *src, दशांश *dst, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    साठी (पूर्णांक i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

शून्य exp64f(स्थिरांक द्विगुण *src, द्विगुण *dst, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    साठी (पूर्णांक i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

शून्य log32f(स्थिरांक दशांश *src, दशांश *dst, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    साठी (पूर्णांक i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
शून्य log64f(स्थिरांक द्विगुण *src, द्विगुण *dst, पूर्णांक n)
{
    CV_INSTRUMENT_REGION();

    साठी (पूर्णांक i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
#अन्यथा

////////////////////////////////////// EXP /////////////////////////////////////


#endif // issue 7795

दशांश fastAtan2( दशांश y, दशांश x )
{
    परत atan_f32(y, x);
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

}} // namespace cv::hal
