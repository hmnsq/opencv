// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাৱ "mathfuncs.hpp"

নামাকাশ cv { নামাকাশ hal {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
শূন্য cartToPolar32f(ধ্রুৱ ভিন্ন *X, ধ্রুৱ ভিন্ন *Y, ভিন্ন* mag, ভিন্ন *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees);
শূন্য cartToPolar64f(ধ্রুৱ দ্ৱিগুণ *X, ধ্রুৱ দ্ৱিগুণ *Y, দ্ৱিগুণ* mag, দ্ৱিগুণ *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees);
শূন্য polarToCart32f(ধ্রুৱ ভিন্ন *mag, ধ্রুৱ ভিন্ন *angle, ভিন্ন *X, ভিন্ন *Y, পূর্ণাঙ্ক len, তার্কিক angleInDegrees);
শূন্য polarToCart64f(ধ্রুৱ দ্ৱিগুণ *mag, ধ্রুৱ দ্ৱিগুণ *angle, দ্ৱিগুণ *X, দ্ৱিগুণ *Y, পূর্ণাঙ্ক len, তার্কিক angleInDegrees);
শূন্য fastAtan32f(ধ্রুৱ ভিন্ন *Y, ধ্রুৱ ভিন্ন *X, ভিন্ন *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees);
শূন্য fastAtan64f(ধ্রুৱ দ্ৱিগুণ *Y, ধ্রুৱ দ্ৱিগুণ *X, দ্ৱিগুণ *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees);
শূন্য fastAtan2(ধ্রুৱ ভিন্ন *Y, ধ্রুৱ ভিন্ন *X, ভিন্ন *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees);
শূন্য magnitude32f(ধ্রুৱ ভিন্ন* x, ধ্রুৱ ভিন্ন* y, ভিন্ন* mag, পূর্ণাঙ্ক len);
শূন্য magnitude64f(ধ্রুৱ দ্ৱিগুণ* x, ধ্রুৱ দ্ৱিগুণ* y, দ্ৱিগুণ* mag, পূর্ণাঙ্ক len);
শূন্য invSqrt32f(ধ্রুৱ ভিন্ন* src, ভিন্ন* dst, পূর্ণাঙ্ক len);
শূন্য invSqrt64f(ধ্রুৱ দ্ৱিগুণ* src, দ্ৱিগুণ* dst, পূর্ণাঙ্ক len);
শূন্য sqrt32f(ধ্রুৱ ভিন্ন* src, ভিন্ন* dst, পূর্ণাঙ্ক len);
শূন্য sqrt64f(ধ্রুৱ দ্ৱিগুণ* src, দ্ৱিগুণ* dst, পূর্ণাঙ্ক len);
ভিন্ন fastAtan2(ভিন্ন y, ভিন্ন x);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

using নামাকাশ std;
using নামাকাশ cv;

নামাকাশ {

স্থির ধ্রুৱ ভিন্ন atan2_p1 = 0.9997878412794807f*(ভিন্ন)(180/CV_PI);
স্থির ধ্রুৱ ভিন্ন atan2_p3 = -0.3258083974640975f*(ভিন্ন)(180/CV_PI);
স্থির ধ্রুৱ ভিন্ন atan2_p5 = 0.1555786518463281f*(ভিন্ন)(180/CV_PI);
স্থির ধ্রুৱ ভিন্ন atan2_p7 = -0.04432655554792128f*(ভিন্ন)(180/CV_PI);

#ifdef __EMSCRIPTEN__
স্থির অন্তঃস্থ ভিন্ন atan_f32(ভিন্ন y, ভিন্ন x)
{
    ভিন্ন a = atan2(y, x) * 180.0f / CV_PI;
    যদি (a < 0.0f)
        a += 360.0f;
    যদি (a >= 360.0f)
        a -= 360.0f;
    ঘূৰাই a; // range [0; 360)
}
#অন্যথা
স্থির অন্তঃস্থ ভিন্ন atan_f32(ভিন্ন y, ভিন্ন x)
{
    ভিন্ন ax = std::abs(x), ay = std::abs(y);
    ভিন্ন a, c, c2;
    যদি( ax >= ay )
    {
        c = ay/(ax + (ভিন্ন)DBL_EPSILON);
        c2 = c*c;
        a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    অন্যথা
    {
        c = ax/(ay + (ভিন্ন)DBL_EPSILON);
        c2 = c*c;
        a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    যদি( x < 0 )
        a = 180.f - a;
    যদি( y < 0 )
        a = 360.f - a;
    ঘূৰাই a;
}
#endif

#যদি (CV_SIMD || CV_SIMD_SCALABLE)

v_float32 v_atan_f32(ধ্রুৱ v_float32& y, ধ্রুৱ v_float32& x)
{
    v_float32 eps = vx_setall_f32((ভিন্ন)DBL_EPSILON);
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
    ঘূৰাই a;
}

#endif

} // anonymous::

স্থির শূন্য cartToPolar32f_(ধ্রুৱ ভিন্ন *X, ধ্রুৱ ভিন্ন *Y, ভিন্ন *mag, ভিন্ন *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees )
{
    ভিন্ন scale = angleInDegrees ? 1.f : (ভিন্ন)(CV_PI/180);
    পূর্ণাঙ্ক i = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুৱ পূর্ণাঙ্ক VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    বাবে( ; i < len; i += VECSZ*2 )
    {
        যদি( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            যদি( i == 0 || angle == X || angle == Y )
                ভঙ্গ;
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

    বাবে( ; i < len; i++ )
    {
        ভিন্ন x0 = X[i], y0 = Y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
        angle[i] = atan_f32(y0, x0)*scale;
    }
}

শূন্য cartToPolar32f(ধ্রুৱ ভিন্ন *X, ধ্রুৱ ভিন্ন *Y, ভিন্ন *mag, ভিন্ন *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    cartToPolar32f_(X, Y, mag, angle, len, angleInDegrees );
}

শূন্য cartToPolar64f(ধ্রুৱ দ্ৱিগুণ *X, ধ্রুৱ দ্ৱিগুণ *Y, দ্ৱিগুণ *mag, দ্ৱিগুণ *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    ধ্রুৱ পূর্ণাঙ্ক BLKSZ = 128;
    ভিন্ন ybuf[BLKSZ], xbuf[BLKSZ], mbuf[BLKSZ], abuf[BLKSZ];
    বাবে( পূর্ণাঙ্ক i = 0; i < len; i += BLKSZ )
    {
        পূর্ণাঙ্ক j, blksz = std::min(BLKSZ, len - i);
        বাবে( j = 0; j < blksz; j++ )
        {
            xbuf[j] = (ভিন্ন)X[i + j];
            ybuf[j] = (ভিন্ন)Y[i + j];
        }
        cartToPolar32f_(xbuf, ybuf, mbuf, abuf, blksz, angleInDegrees);
        বাবে( j = 0; j < blksz; j++ )
            mag[i + j] = mbuf[j];
        বাবে( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

নামাকাশ {

স্থির অন্তঃস্থ শূন্য SinCos_32f(ধ্রুৱ ভিন্ন* mag, ধ্রুৱ ভিন্ন* angle, ভিন্ন* cosval, ভিন্ন* sinval, পূর্ণাঙ্ক len, পূর্ণাঙ্ক angle_in_degrees)
{
    ধ্রুৱ পূর্ণাঙ্ক N = 64;

    স্থির ধ্রুৱ দ্ৱিগুণ sin_table[] =
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

    স্থির ধ্রুৱ দ্ৱিগুণ k2 = (2*CV_PI)/N;

    স্থির ধ্রুৱ দ্ৱিগুণ sin_a0 = -0.166630293345647*k2*k2*k2;
    স্থির ধ্রুৱ দ্ৱিগুণ sin_a2 = k2;

    স্থির ধ্রুৱ দ্ৱিগুণ cos_a0 = -0.499818138450326*k2*k2;
    /*static const double cos_a2 =  1;*/

    দ্ৱিগুণ k1;
    পূর্ণাঙ্ক i = 0;

    যদি( !angle_in_degrees )
        k1 = N/(2*CV_PI);
    অন্যথা
        k1 = N/360.;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুৱ পূর্ণাঙ্ক VECSZ = VTraits<v_float32>::vlanes();
    ধ্রুৱ v_float32 scale = vx_setall_f32(angle_in_degrees ? (ভিন্ন)CV_PI / 180.f : 1.f);

    বাবে( ; i < len; i += VECSZ*2 )
    {
        যদি( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            যদি( i == 0 || angle == cosval || angle == sinval || mag == cosval || mag == sinval )
                ভঙ্গ;
            i = len - VECSZ*2;
        }

        v_float32 r0 = v_mul(vx_load(angle + i), scale);
        v_float32 r1 = v_mul(vx_load(angle + i + VECSZ), scale);

        v_float32 c0, c1, s0, s1;
        v_sincos(r0, s0, c0);
        v_sincos(r1, s1, c1);

        যদি( mag )
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

    বাবে( ; i < len; i++ )
    {
        দ্ৱিগুণ t = angle[i]*k1;
        পূর্ণাঙ্ক it = cvRound(t);
        t -= it;
        পূর্ণাঙ্ক sin_idx = it & (N - 1);
        পূর্ণাঙ্ক cos_idx = (N/4 - sin_idx) & (N - 1);

        দ্ৱিগুণ sin_b = (sin_a0*t*t + sin_a2)*t;
        দ্ৱিগুণ cos_b = cos_a0*t*t + 1;

        দ্ৱিগুণ sin_a = sin_table[sin_idx];
        দ্ৱিগুণ cos_a = sin_table[cos_idx];

        দ্ৱিগুণ sin_val = sin_a*cos_b + cos_a*sin_b;
        দ্ৱিগুণ cos_val = cos_a*cos_b - sin_a*sin_b;

        যদি (mag)
        {
            দ্ৱিগুণ mag_val = mag[i];
            sin_val *= mag_val;
            cos_val *= mag_val;
        }

        sinval[i] = (ভিন্ন)sin_val;
        cosval[i] = (ভিন্ন)cos_val;
    }
}

} // anonymous::

শূন্য polarToCart32f(ধ্রুৱ ভিন্ন *mag, ধ্রুৱ ভিন্ন *angle, ভিন্ন *X, ভিন্ন *Y, পূর্ণাঙ্ক len, তার্কিক angleInDegrees)
{
    CV_INSTRUMENT_REGION();
    SinCos_32f(mag, angle, X, Y, len, angleInDegrees);
}

শূন্য polarToCart64f(ধ্রুৱ দ্ৱিগুণ *mag, ধ্রুৱ দ্ৱিগুণ *angle, দ্ৱিগুণ *X, দ্ৱিগুণ *Y, পূর্ণাঙ্ক len, তার্কিক angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    ধ্রুৱ পূর্ণাঙ্ক BLKSZ = 128;
    ভিন্ন ybuf[BLKSZ], xbuf[BLKSZ], _mbuf[BLKSZ], abuf[BLKSZ];
    ভিন্ন* mbuf = mag ? _mbuf : nullptr;
    বাবে( পূর্ণাঙ্ক i = 0; i < len; i += BLKSZ )
    {
        পূর্ণাঙ্ক j, blksz = std::min(BLKSZ, len - i);
        বাবে( j = 0; j < blksz; j++ )
        {
            যদি (mbuf)
                mbuf[j] = (ভিন্ন)mag[i + j];
            abuf[j] = (ভিন্ন)angle[i + j];
        }
        SinCos_32f(mbuf, abuf, xbuf, ybuf, blksz, angleInDegrees);
        বাবে( j = 0; j < blksz; j++ )
            X[i + j] = xbuf[j];
        বাবে( j = 0; j < blksz; j++ )
            Y[i + j] = ybuf[j];
    }
}

স্থির শূন্য fastAtan32f_(ধ্রুৱ ভিন্ন *Y, ধ্রুৱ ভিন্ন *X, ভিন্ন *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees )
{
    ভিন্ন scale = angleInDegrees ? 1.f : (ভিন্ন)(CV_PI/180);
    পূর্ণাঙ্ক i = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুৱ পূর্ণাঙ্ক VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    বাবে( ; i < len; i += VECSZ*2 )
    {
        যদি( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            যদি( i == 0 || angle == X || angle == Y )
                ভঙ্গ;
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

    বাবে( ; i < len; i++ )
        angle[i] = atan_f32(Y[i], X[i])*scale;
}

শূন্য fastAtan32f(ধ্রুৱ ভিন্ন *Y, ধ্রুৱ ভিন্ন *X, ভিন্ন *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f_(Y, X, angle, len, angleInDegrees );
}

শূন্য fastAtan64f(ধ্রুৱ দ্ৱিগুণ *Y, ধ্রুৱ দ্ৱিগুণ *X, দ্ৱিগুণ *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees)
{
    CV_INSTRUMENT_REGION();

    ধ্রুৱ পূর্ণাঙ্ক BLKSZ = 128;
    ভিন্ন ybuf[BLKSZ], xbuf[BLKSZ], abuf[BLKSZ];
    বাবে( পূর্ণাঙ্ক i = 0; i < len; i += BLKSZ )
    {
        পূর্ণাঙ্ক j, blksz = std::min(BLKSZ, len - i);
        বাবে( j = 0; j < blksz; j++ )
        {
            ybuf[j] = (ভিন্ন)Y[i + j];
            xbuf[j] = (ভিন্ন)X[i + j];
        }
        fastAtan32f_(ybuf, xbuf, abuf, blksz, angleInDegrees);
        বাবে( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

// deprecated
শূন্য fastAtan2(ধ্রুৱ ভিন্ন *Y, ধ্রুৱ ভিন্ন *X, ভিন্ন *angle, পূর্ণাঙ্ক len, তার্কিক angleInDegrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f(Y, X, angle, len, angleInDegrees);
}

শূন্য magnitude32f(ধ্রুৱ ভিন্ন* x, ধ্রুৱ ভিন্ন* y, ভিন্ন* mag, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক i = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুৱ পূর্ণাঙ্ক VECSZ = VTraits<v_float32>::vlanes();
    বাবে( ; i < len; i += VECSZ*2 )
    {
        যদি( i + VECSZ*2 > len )
        {
            যদি( i == 0 || mag == x || mag == y )
                ভঙ্গ;
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

    বাবে( ; i < len; i++ )
    {
        ভিন্ন x0 = x[i], y0 = y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
    }
}

শূন্য magnitude64f(ধ্রুৱ দ্ৱিগুণ* x, ধ্রুৱ দ্ৱিগুণ* y, দ্ৱিগুণ* mag, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক i = 0;

#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ধ্রুৱ পূর্ণাঙ্ক VECSZ = VTraits<v_float64>::vlanes();
    বাবে( ; i < len; i += VECSZ*2 )
    {
        যদি( i + VECSZ*2 > len )
        {
            যদি( i == 0 || mag == x || mag == y )
                ভঙ্গ;
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

    বাবে( ; i < len; i++ )
    {
        দ্ৱিগুণ x0 = x[i], y0 = y[i];
        mag[i] = std::sqrt(x0*x0 + y0*y0);
    }
}


শূন্য invSqrt32f(ধ্রুৱ ভিন্ন* src, ভিন্ন* dst, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক i = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুৱ পূর্ণাঙ্ক VECSZ = VTraits<v_float32>::vlanes();
    বাবে( ; i < len; i += VECSZ*2 )
    {
        যদি( i + VECSZ*2 > len )
        {
            যদি( i == 0 || src == dst )
                ভঙ্গ;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    বাবে( ; i < len; i++ )
        dst[i] = 1/std::sqrt(src[i]);
}


শূন্য invSqrt64f(ধ্রুৱ দ্ৱিগুণ* src, দ্ৱিগুণ* dst, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();
    পূর্ণাঙ্ক i = 0;

#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ধ্রুৱ পূর্ণাঙ্ক VECSZ = VTraits<v_float64>::vlanes();
    বাবে ( ; i < len; i += VECSZ*2)
    {
        যদি( i + VECSZ*2 > len )
        {
            যদি( i == 0 || src == dst )
                ভঙ্গ;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
#endif

    বাবে( ; i < len; i++ )
        dst[i] = 1/std::sqrt(src[i]);
}


শূন্য sqrt32f(ধ্রুৱ ভিন্ন* src, ভিন্ন* dst, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক i = 0;

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
    ধ্রুৱ পূর্ণাঙ্ক VECSZ = VTraits<v_float32>::vlanes();
    বাবে( ; i < len; i += VECSZ*2 )
    {
        যদি( i + VECSZ*2 > len )
        {
            যদি( i == 0 || src == dst )
                ভঙ্গ;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    বাবে( ; i < len; i++ )
        dst[i] = std::sqrt(src[i]);
}


শূন্য sqrt64f(ধ্রুৱ দ্ৱিগুণ* src, দ্ৱিগুণ* dst, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক i = 0;

#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ধ্রুৱ পূর্ণাঙ্ক VECSZ = VTraits<v_float64>::vlanes();
    বাবে( ; i < len; i += VECSZ*2 )
    {
        যদি( i + VECSZ*2 > len )
        {
            যদি( i == 0 || src == dst )
                ভঙ্গ;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_load(src + i), t1 = vx_load(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#endif

    বাবে( ; i < len; i++ )
        dst[i] = std::sqrt(src[i]);
}

// Workaround for ICE in MSVS 2015 update 3 (issue #7795)
// CV_AVX is not used here, because generated code is faster in non-AVX mode.
// (tested with disabled IPP on i5-6300U)
#যদি (defined _MSC_VER && _MSC_VER >= 1900 && (defined(_M_IX86) || defined(_M_X64))) || defined(__EMSCRIPTEN__)
শূন্য exp32f(ধ্রুৱ ভিন্ন *src, ভিন্ন *dst, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    বাবে (পূর্ণাঙ্ক i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

শূন্য exp64f(ধ্রুৱ দ্ৱিগুণ *src, দ্ৱিগুণ *dst, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    বাবে (পূর্ণাঙ্ক i = 0; i < n; i++)
    {
        dst[i] = std::exp(src[i]);
    }
}

শূন্য log32f(ধ্রুৱ ভিন্ন *src, ভিন্ন *dst, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    বাবে (পূর্ণাঙ্ক i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
শূন্য log64f(ধ্রুৱ দ্ৱিগুণ *src, দ্ৱিগুণ *dst, পূর্ণাঙ্ক n)
{
    CV_INSTRUMENT_REGION();

    বাবে (পূর্ণাঙ্ক i = 0; i < n; i++)
    {
        dst[i] = std::log(src[i]);
    }
}
#অন্যথা

////////////////////////////////////// EXP /////////////////////////////////////


#endif // issue 7795

ভিন্ন fastAtan2( ভিন্ন y, ভিন্ন x )
{
    ঘূৰাই atan_f32(y, x);
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END

}} // namespace cv::hal
