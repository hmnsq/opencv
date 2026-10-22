// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "mathfuncs.hpp"

نامخلا cv { نامخلا hal {

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// forward declarations
خالی cart_مقصد_Polar32f(ثابت اعشاری *X, ثابت اعشاری *Y, اعشاری* mag, اعشاری *angle, صحیح len, منطقی angle_اندر_Degrees);
خالی cart_مقصد_Polar64f(ثابت دوہرا *X, ثابت دوہرا *Y, دوہرا* mag, دوہرا *angle, صحیح len, منطقی angle_اندر_Degrees);
خالی polar_مقصد_Cart32f(ثابت اعشاری *mag, ثابت اعشاری *angle, اعشاری *X, اعشاری *Y, صحیح len, منطقی angle_اندر_Degrees);
خالی polar_مقصد_Cart64f(ثابت دوہرا *mag, ثابت دوہرا *angle, دوہرا *X, دوہرا *Y, صحیح len, منطقی angle_اندر_Degrees);
خالی fastAtan32f(ثابت اعشاری *Y, ثابت اعشاری *X, اعشاری *angle, صحیح len, منطقی angle_اندر_Degrees);
خالی fastAtan64f(ثابت دوہرا *Y, ثابت دوہرا *X, دوہرا *angle, صحیح len, منطقی angle_اندر_Degrees);
خالی fastAtan2(ثابت اعشاری *Y, ثابت اعشاری *X, اعشاری *angle, صحیح len, منطقی angle_اندر_Degrees);
خالی magnitude32f(ثابت اعشاری* x, ثابت اعشاری* y, اعشاری* mag, صحیح len);
خالی magnitude64f(ثابت دوہرا* x, ثابت دوہرا* y, دوہرا* mag, صحیح len);
خالی invSqrt32f(ثابت اعشاری* src, اعشاری* dst, صحیح len);
خالی invSqrt64f(ثابت دوہرا* src, دوہرا* dst, صحیح len);
خالی sqrt32f(ثابت اعشاری* src, اعشاری* dst, صحیح len);
خالی sqrt64f(ثابت دوہرا* src, دوہرا* dst, صحیح len);
اعشاری fastAtan2(اعشاری y, اعشاری x);

#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

استعمال نامخلا std;
استعمال نامخلا cv;

نامخلا {

ساکن ثابت اعشاری atan2_p1 = 0.9997878412794807f*(اعشاری)(180/CV_PI);
ساکن ثابت اعشاری atan2_p3 = -0.3258083974640975f*(اعشاری)(180/CV_PI);
ساکن ثابت اعشاری atan2_p5 = 0.1555786518463281f*(اعشاری)(180/CV_PI);
ساکن ثابت اعشاری atan2_p7 = -0.04432655554792128f*(اعشاری)(180/CV_PI);

#اگر_تعریف_شدہ __EMSCRIPTEN__
ساکن اندرونی اعشاری atan_f32(اعشاری y, اعشاری x)
{
    اعشاری a = atan2(y, x) * 180.0f / CV_PI;
    اگر (a < 0.0f)
        a += 360.0f;
    اگر (a >= 360.0f)
        a -= 360.0f;
    واپس a; // range [0; 360)
}
#ورنہ
ساکن اندرونی اعشاری atan_f32(اعشاری y, اعشاری x)
{
    اعشاری ax = std::مطلق_قدر(x), ay = std::مطلق_قدر(y);
    اعشاری a, c, c2;
    اگر( ax >= ay )
    {
        c = ay/(ax + (اعشاری)DBL_EPSILON);
        c2 = c*c;
        a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    ورنہ
    {
        c = ax/(ay + (اعشاری)DBL_EPSILON);
        c2 = c*c;
        a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
    }
    اگر( x < 0 )
        a = 180.f - a;
    اگر( y < 0 )
        a = 360.f - a;
    واپس a;
}
#اگر_ختم

#اگر (CV_SIMD || CV_SIMD_SCALABLE)

v_float32 v_atan_f32(ثابت v_float32& y, ثابت v_float32& x)
{
    v_float32 eps = vx_setall_f32((اعشاری)DBL_EPSILON);
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
    v_float32 c = v_div(v_min(ax, ay), v_جوڑو(v_max(ax, ay), eps));
    v_float32 cc = v_mul(c, c);
    v_float32 a = v_mul(v_fma(v_fma(v_fma(cc, p7, p5), cc, p3), cc, p1), c);
    a = v_select(v_ge(ax, ay), a, v_sub(val90, a));
    a = v_select(v_lt(x, z), v_sub(val180, a), a);
    a = v_select(v_lt(y, z), v_sub(val360, a), a);
    واپس a;
}

#اگر_ختم

} // anonymous::

ساکن خالی cart_مقصد_Polar32f_2(ثابت اعشاری *X, ثابت اعشاری *Y, اعشاری *mag, اعشاری *angle, صحیح len, منطقی angle_اندر_Degrees )
{
    اعشاری scale = angle_اندر_Degrees ? 1.f : (اعشاری)(CV_PI/180);
    صحیح i = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    برائے( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            اگر( i == 0 || angle == X || angle == Y )
                توڑو;
            i = len - VECSZ*2;
        }

        v_float32 x0 = vx_بھرو(X + i);
        v_float32 y0 = vx_بھرو(Y + i);
        v_float32 x1 = vx_بھرو(X + i + VECSZ);
        v_float32 y1 = vx_بھرو(Y + i + VECSZ);

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
#اگر_ختم

    برائے( ; i < len; i++ )
    {
        اعشاری x0 = X[i], y0 = Y[i];
        mag[i] = std::جذر(x0*x0 + y0*y0);
        angle[i] = atan_f32(y0, x0)*scale;
    }
}

خالی cart_مقصد_Polar32f(ثابت اعشاری *X, ثابت اعشاری *Y, اعشاری *mag, اعشاری *angle, صحیح len, منطقی angle_اندر_Degrees )
{
    CV_INSTRUMENT_REGION();
    cart_مقصد_Polar32f_2(X, Y, mag, angle, len, angle_اندر_Degrees );
}

خالی cart_مقصد_Polar64f(ثابت دوہرا *X, ثابت دوہرا *Y, دوہرا *mag, دوہرا *angle, صحیح len, منطقی angle_اندر_Degrees)
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح BLKSZ = 128;
    اعشاری ybuf[BLKSZ], xbuf[BLKSZ], mbuf[BLKSZ], abuf[BLKSZ];
    برائے( صحیح i = 0; i < len; i += BLKSZ )
    {
        صحیح j, blksz = std::min(BLKSZ, len - i);
        برائے( j = 0; j < blksz; j++ )
        {
            xbuf[j] = (اعشاری)X[i + j];
            ybuf[j] = (اعشاری)Y[i + j];
        }
        cart_مقصد_Polar32f_2(xbuf, ybuf, mbuf, abuf, blksz, angle_اندر_Degrees);
        برائے( j = 0; j < blksz; j++ )
            mag[i + j] = mbuf[j];
        برائے( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

نامخلا {

ساکن اندرونی خالی SinCos_32f(ثابت اعشاری* mag, ثابت اعشاری* angle, اعشاری* cosval, اعشاری* sinval, صحیح len, صحیح angle_اندر_degrees)
{
    ثابت صحیح N = 64;

    ساکن ثابت دوہرا sin_جدول[] =
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

    ساکن ثابت دوہرا k2 = (2*CV_PI)/N;

    ساکن ثابت دوہرا sin_a0 = -0.166630293345647*k2*k2*k2;
    ساکن ثابت دوہرا sin_a2 = k2;

    ساکن ثابت دوہرا cos_a0 = -0.499818138450326*k2*k2;
    /*static const double cos_a2 =  1;*/

    دوہرا k1;
    صحیح i = 0;

    اگر( !angle_اندر_degrees )
        k1 = N/(2*CV_PI);
    ورنہ
        k1 = N/360.;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    ثابت v_float32 scale = vx_setall_f32(angle_اندر_degrees ? (اعشاری)CV_PI / 180.f : 1.f);

    برائے( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            اگر( i == 0 || angle == cosval || angle == sinval || mag == cosval || mag == sinval )
                توڑو;
            i = len - VECSZ*2;
        }

        v_float32 r0 = v_mul(vx_بھرو(angle + i), scale);
        v_float32 r1 = v_mul(vx_بھرو(angle + i + VECSZ), scale);

        v_float32 c0, c1, s0, s1;
        v_sincos(r0, s0, c0);
        v_sincos(r1, s1, c1);

        اگر( mag )
        {
            v_float32 m0 = vx_بھرو(mag + i);
            v_float32 m1 = vx_بھرو(mag + i + VECSZ);
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
#اگر_ختم

    برائے( ; i < len; i++ )
    {
        دوہرا t = angle[i]*k1;
        صحیح it = cv_باری(t);
        t -= it;
        صحیح sin_idx = it & (N - 1);
        صحیح cos_idx = (N/4 - sin_idx) & (N - 1);

        دوہرا sin_b = (sin_a0*t*t + sin_a2)*t;
        دوہرا cos_b = cos_a0*t*t + 1;

        دوہرا sin_a = sin_جدول[sin_idx];
        دوہرا cos_a = sin_جدول[cos_idx];

        دوہرا sin_val = sin_a*cos_b + cos_a*sin_b;
        دوہرا cos_val = cos_a*cos_b - sin_a*sin_b;

        اگر (mag)
        {
            دوہرا mag_val = mag[i];
            sin_val *= mag_val;
            cos_val *= mag_val;
        }

        sinval[i] = (اعشاری)sin_val;
        cosval[i] = (اعشاری)cos_val;
    }
}

} // anonymous::

خالی polar_مقصد_Cart32f(ثابت اعشاری *mag, ثابت اعشاری *angle, اعشاری *X, اعشاری *Y, صحیح len, منطقی angle_اندر_Degrees)
{
    CV_INSTRUMENT_REGION();
    SinCos_32f(mag, angle, X, Y, len, angle_اندر_Degrees);
}

خالی polar_مقصد_Cart64f(ثابت دوہرا *mag, ثابت دوہرا *angle, دوہرا *X, دوہرا *Y, صحیح len, منطقی angle_اندر_Degrees)
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح BLKSZ = 128;
    اعشاری ybuf[BLKSZ], xbuf[BLKSZ], _mbuf[BLKSZ], abuf[BLKSZ];
    اعشاری* mbuf = mag ? _mbuf : خالی_اشاریہ;
    برائے( صحیح i = 0; i < len; i += BLKSZ )
    {
        صحیح j, blksz = std::min(BLKSZ, len - i);
        برائے( j = 0; j < blksz; j++ )
        {
            اگر (mbuf)
                mbuf[j] = (اعشاری)mag[i + j];
            abuf[j] = (اعشاری)angle[i + j];
        }
        SinCos_32f(mbuf, abuf, xbuf, ybuf, blksz, angle_اندر_Degrees);
        برائے( j = 0; j < blksz; j++ )
            X[i + j] = xbuf[j];
        برائے( j = 0; j < blksz; j++ )
            Y[i + j] = ybuf[j];
    }
}

ساکن خالی fastAtan32f_(ثابت اعشاری *Y, ثابت اعشاری *X, اعشاری *angle, صحیح len, منطقی angle_اندر_Degrees )
{
    اعشاری scale = angle_اندر_Degrees ? 1.f : (اعشاری)(CV_PI/180);
    صحیح i = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    v_float32 s = vx_setall_f32(scale);

    برائے( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            // if it's inplace operation, we cannot repeatedly process
            // the tail for the second time, so we have to use the
            // scalar code
            اگر( i == 0 || angle == X || angle == Y )
                توڑو;
            i = len - VECSZ*2;
        }

        v_float32 y0 = vx_بھرو(Y + i);
        v_float32 x0 = vx_بھرو(X + i);
        v_float32 y1 = vx_بھرو(Y + i + VECSZ);
        v_float32 x1 = vx_بھرو(X + i + VECSZ);

        v_float32 r0 = v_mul(v_atan_f32(y0, x0), s);
        v_float32 r1 = v_mul(v_atan_f32(y1, x1), s);

        v_store(angle + i, r0);
        v_store(angle + i + VECSZ, r1);
    }
    vx_cleanup();
#اگر_ختم

    برائے( ; i < len; i++ )
        angle[i] = atan_f32(Y[i], X[i])*scale;
}

خالی fastAtan32f(ثابت اعشاری *Y, ثابت اعشاری *X, اعشاری *angle, صحیح len, منطقی angle_اندر_Degrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f_(Y, X, angle, len, angle_اندر_Degrees );
}

خالی fastAtan64f(ثابت دوہرا *Y, ثابت دوہرا *X, دوہرا *angle, صحیح len, منطقی angle_اندر_Degrees)
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح BLKSZ = 128;
    اعشاری ybuf[BLKSZ], xbuf[BLKSZ], abuf[BLKSZ];
    برائے( صحیح i = 0; i < len; i += BLKSZ )
    {
        صحیح j, blksz = std::min(BLKSZ, len - i);
        برائے( j = 0; j < blksz; j++ )
        {
            ybuf[j] = (اعشاری)Y[i + j];
            xbuf[j] = (اعشاری)X[i + j];
        }
        fastAtan32f_(ybuf, xbuf, abuf, blksz, angle_اندر_Degrees);
        برائے( j = 0; j < blksz; j++ )
            angle[i + j] = abuf[j];
    }
}

// deprecated
خالی fastAtan2(ثابت اعشاری *Y, ثابت اعشاری *X, اعشاری *angle, صحیح len, منطقی angle_اندر_Degrees )
{
    CV_INSTRUMENT_REGION();
    fastAtan32f(Y, X, angle, len, angle_اندر_Degrees);
}

خالی magnitude32f(ثابت اعشاری* x, ثابت اعشاری* y, اعشاری* mag, صحیح len)
{
    CV_INSTRUMENT_REGION();

    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    برائے( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            اگر( i == 0 || mag == x || mag == y )
                توڑو;
            i = len - VECSZ*2;
        }
        v_float32 x0 = vx_بھرو(x + i), x1 = vx_بھرو(x + i + VECSZ);
        v_float32 y0 = vx_بھرو(y + i), y1 = vx_بھرو(y + i + VECSZ);
        x0 = v_sqrt(v_muladd(x0, x0, v_mul(y0, y0)));
        x1 = v_sqrt(v_muladd(x1, x1, v_mul(y1, y1)));
        v_store(mag + i, x0);
        v_store(mag + i + VECSZ, x1);
    }
    vx_cleanup();
#اگر_ختم

    برائے( ; i < len; i++ )
    {
        اعشاری x0 = x[i], y0 = y[i];
        mag[i] = std::جذر(x0*x0 + y0*y0);
    }
}

خالی magnitude64f(ثابت دوہرا* x, ثابت دوہرا* y, دوہرا* mag, صحیح len)
{
    CV_INSTRUMENT_REGION();

    صحیح i = 0;

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ثابت صحیح VECSZ = VTraits<v_float64>::vlanes();
    برائے( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            اگر( i == 0 || mag == x || mag == y )
                توڑو;
            i = len - VECSZ*2;
        }
        v_float64 x0 = vx_بھرو(x + i), x1 = vx_بھرو(x + i + VECSZ);
        v_float64 y0 = vx_بھرو(y + i), y1 = vx_بھرو(y + i + VECSZ);
        x0 = v_sqrt(v_muladd(x0, x0, v_mul(y0, y0)));
        x1 = v_sqrt(v_muladd(x1, x1, v_mul(y1, y1)));
        v_store(mag + i, x0);
        v_store(mag + i + VECSZ, x1);
    }
    vx_cleanup();
#اگر_ختم

    برائے( ; i < len; i++ )
    {
        دوہرا x0 = x[i], y0 = y[i];
        mag[i] = std::جذر(x0*x0 + y0*y0);
    }
}


خالی invSqrt32f(ثابت اعشاری* src, اعشاری* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();

    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    برائے( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            اگر( i == 0 || src == dst )
                توڑو;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_بھرو(src + i), t1 = vx_بھرو(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#اگر_ختم

    برائے( ; i < len; i++ )
        dst[i] = 1/std::جذر(src[i]);
}


خالی invSqrt64f(ثابت دوہرا* src, دوہرا* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();
    صحیح i = 0;

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ثابت صحیح VECSZ = VTraits<v_float64>::vlanes();
    برائے ( ; i < len; i += VECSZ*2)
    {
        اگر( i + VECSZ*2 > len )
        {
            اگر( i == 0 || src == dst )
                توڑو;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_بھرو(src + i), t1 = vx_بھرو(src + i + VECSZ);
        t0 = v_invsqrt(t0);
        t1 = v_invsqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
#اگر_ختم

    برائے( ; i < len; i++ )
        dst[i] = 1/std::جذر(src[i]);
}


خالی sqrt32f(ثابت اعشاری* src, اعشاری* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();

    صحیح i = 0;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    برائے( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            اگر( i == 0 || src == dst )
                توڑو;
            i = len - VECSZ*2;
        }
        v_float32 t0 = vx_بھرو(src + i), t1 = vx_بھرو(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#اگر_ختم

    برائے( ; i < len; i++ )
        dst[i] = std::جذر(src[i]);
}


خالی sqrt64f(ثابت دوہرا* src, دوہرا* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();

    صحیح i = 0;

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
    ثابت صحیح VECSZ = VTraits<v_float64>::vlanes();
    برائے( ; i < len; i += VECSZ*2 )
    {
        اگر( i + VECSZ*2 > len )
        {
            اگر( i == 0 || src == dst )
                توڑو;
            i = len - VECSZ*2;
        }
        v_float64 t0 = vx_بھرو(src + i), t1 = vx_بھرو(src + i + VECSZ);
        t0 = v_sqrt(t0);
        t1 = v_sqrt(t1);
        v_store(dst + i, t0); v_store(dst + i + VECSZ, t1);
    }
    vx_cleanup();
#اگر_ختم

    برائے( ; i < len; i++ )
        dst[i] = std::جذر(src[i]);
}

// Workaround for ICE in MSVS 2015 update 3 (issue #7795)
// CV_AVX is not used here, because generated code is faster in non-AVX mode.
// (tested with disabled IPP on i5-6300U)
#اگر (defined _MSC_VER && _MSC_VER >= 1900 && (defined(_M_IX86) || defined(_M_X64))) || defined(__EMSCRIPTEN__)
خالی exp32f(ثابت اعشاری *src, اعشاری *dst, صحیح n)
{
    CV_INSTRUMENT_REGION();

    برائے (صحیح i = 0; i < n; i++)
    {
        dst[i] = std::قوت_نما(src[i]);
    }
}

خالی exp64f(ثابت دوہرا *src, دوہرا *dst, صحیح n)
{
    CV_INSTRUMENT_REGION();

    برائے (صحیح i = 0; i < n; i++)
    {
        dst[i] = std::قوت_نما(src[i]);
    }
}

خالی log32f(ثابت اعشاری *src, اعشاری *dst, صحیح n)
{
    CV_INSTRUMENT_REGION();

    برائے (صحیح i = 0; i < n; i++)
    {
        dst[i] = std::لوگارتھم(src[i]);
    }
}
خالی log64f(ثابت دوہرا *src, دوہرا *dst, صحیح n)
{
    CV_INSTRUMENT_REGION();

    برائے (صحیح i = 0; i < n; i++)
    {
        dst[i] = std::لوگارتھم(src[i]);
    }
}
#ورنہ

////////////////////////////////////// EXP /////////////////////////////////////


#اگر_ختم // issue 7795

اعشاری fastAtan2( اعشاری y, اعشاری x )
{
    واپس atan_f32(y, x);
}

#اگر_ختم // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_انتہا

}} // namespace cv::hal
