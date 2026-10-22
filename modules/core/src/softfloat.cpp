// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

// This file is based on files from packages softfloat and fdlibm
// issued with the following licenses:

/*============================================================================

This C source file is part of the SoftFloat IEEE Floating-Point Arithmetic
Package, Release 3c, by John R. Hauser.

Copyright 2011, 2012, 2013, 2014, 2015, 2016, 2017 The Regents of the
University of California.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions, and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions, and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

 3. Neither the name of the University nor the names of its contributors may
    be used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS "AS IS", AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ARE
DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/

// FDLIBM licenses:

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
 * ====================================================
 * Copyright (C) 2004 by Sun Microsystems, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core/softfloat.hpp"

ನಾಮಾಕಾಶ cv
{

/*----------------------------------------------------------------------------
| Software floating-point underflow tininess-detection mode.
*----------------------------------------------------------------------------*/
ಗಣನಾ {
    tininess_beforeRounding = 0,
    tininess_afterRounding  = 1
};
//fixed to make softfloat code stateless
ಸ್ಥಿರ ಧ್ರುವ uint_fast8_t globalDetectTininess = tininess_afterRounding;

/*----------------------------------------------------------------------------
| Software floating-point exception flags.
*----------------------------------------------------------------------------*/
ಗಣನಾ {
    flag_inexact   =  1,
    flag_underflow =  2,
    flag_overflow  =  4,
    flag_infinite  =  8,
    flag_invalid   = 16
};

// Disabled to make softfloat code stateless
// This function may be changed in the future for better error handling
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ raiseFlags( uint_fast8_t /* flags */)
{
    //exceptionFlags |= flags;
}

/*----------------------------------------------------------------------------
| Software floating-point rounding mode.
*----------------------------------------------------------------------------*/
ಗಣನಾ {
    round_near_even   = 0, // round to nearest, with ties to even
    round_minMag      = 1, // round to minimum magnitude (toward zero)
    round_min         = 2, // round to minimum (down)
    round_max         = 3, // round to maximum (up)
    round_near_maxMag = 4, // round to nearest, with ties to maximum magnitude (away from zero)
    round_odd         = 5  // round to odd (jamming)
};
/* What is round_odd (from SoftFloat manual):
 * If supported, mode round_odd first rounds a floating-point result to minimum magnitude,
 * the same as round_minMag, and then, if the result is inexact, the least-significant bit
 * of the result is set to 1. This rounding mode is also known as jamming.
 */

//fixed to make softfloat code stateless
ಸ್ಥಿರ ಧ್ರುವ uint_fast8_t globalRoundingMode = round_near_even;

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/

#define signF32UI( a ) (((uint32_t) (a)>>31) != 0)
#define expF32UI( a ) ((int_fast16_t) ((a)>>23) & 0xFF)
#define fracF32UI( a ) ((a) & 0x007FFFFF)
#define packToF32UI( sign, exp, sig ) (((uint32_t) (sign)<<31) + ((uint32_t) (exp)<<23) + (sig))

#define isNaNF32UI( a ) (((~(a) & 0x7F800000) == 0) && ((a) & 0x007FFFFF))

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/

#define signF64UI( a ) (((uint64_t) (a)>>63) != 0)
#define expF64UI( a ) ((int_fast16_t) ((a)>>52) & 0x7FF)
#define fracF64UI( a ) ((a) & UINT64_C( 0x000FFFFFFFFFFFFF ))
#define packToF64UI( sign, exp, sig ) ((uint64_t) (((uint_fast64_t) (sign)<<63) + ((uint_fast64_t) (exp)<<52) + (sig)))

#define isNaNF64UI( a ) (((~(a) & UINT64_C( 0x7FF0000000000000 )) == 0) && ((a) & UINT64_C( 0x000FFFFFFFFFFFFF )))

/*----------------------------------------------------------------------------
| Types used to pass 32-bit and 64-bit floating-point
| arguments and results to/from functions.  These types must be exactly
| 32 bits and 64 bits in size, respectively.  Where a
| platform has "native" support for IEEE-Standard floating-point formats,
| the types below may, if desired, be defined as aliases for the native types
| (typically 'float' and 'double').
*----------------------------------------------------------------------------*/
ಪ್ರಕಾರನಿರ್ದೇಶ softfloat float32_t;
ಪ್ರಕಾರನಿರ್ದೇಶ softdouble float64_t;

/*----------------------------------------------------------------------------
| Integer-to-floating-point conversion routines.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ float32_t ui32_to_f32( uint32_t );
ಸ್ಥಿರ float64_t ui32_to_f64( uint32_t );
ಸ್ಥಿರ float32_t ui64_to_f32( uint64_t );
ಸ್ಥಿರ float64_t ui64_to_f64( uint64_t );
ಸ್ಥಿರ float32_t i32_to_f32( int32_t );
ಸ್ಥಿರ float64_t i32_to_f64( int32_t );
ಸ್ಥಿರ float32_t i64_to_f32( int64_t );
ಸ್ಥಿರ float64_t i64_to_f64( int64_t );

/*----------------------------------------------------------------------------
| 32-bit (single-precision) floating-point operations.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ int_fast32_t f32_to_i32( float32_t, uint_fast8_t, ತಾರ್ಕಿಕ );
ಸ್ಥಿರ int_fast32_t f32_to_i32_r_minMag( float32_t, ತಾರ್ಕಿಕ );
ಸ್ಥಿರ float64_t f32_to_f64( float32_t );
ಸ್ಥಿರ float32_t f32_roundToInt( float32_t, uint_fast8_t, ತಾರ್ಕಿಕ );
ಸ್ಥಿರ float32_t f32_add( float32_t, float32_t );
ಸ್ಥಿರ float32_t f32_sub( float32_t, float32_t );
ಸ್ಥಿರ float32_t f32_mul( float32_t, float32_t );
ಸ್ಥಿರ float32_t f32_mulAdd( float32_t, float32_t, float32_t );
ಸ್ಥಿರ float32_t f32_div( float32_t, float32_t );
ಸ್ಥಿರ float32_t f32_rem( float32_t, float32_t );
ಸ್ಥಿರ float32_t f32_sqrt( float32_t );
ಸ್ಥಿರ ತಾರ್ಕಿಕ f32_eq( float32_t, float32_t );
ಸ್ಥಿರ ತಾರ್ಕಿಕ f32_le( float32_t, float32_t );
ಸ್ಥಿರ ತಾರ್ಕಿಕ f32_lt( float32_t, float32_t );

/*----------------------------------------------------------------------------
| 64-bit (double-precision) floating-point operations.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ int_fast32_t f64_to_i32( float64_t, uint_fast8_t, ತಾರ್ಕಿಕ );
ಸ್ಥಿರ int_fast64_t f64_to_i64( float64_t, uint_fast8_t, ತಾರ್ಕಿಕ );
ಸ್ಥಿರ int_fast32_t f64_to_i32_r_minMag( float64_t, ತಾರ್ಕಿಕ );
ಸ್ಥಿರ float32_t f64_to_f32( float64_t );
ಸ್ಥಿರ float64_t f64_roundToInt( float64_t, uint_fast8_t, ತಾರ್ಕಿಕ );
ಸ್ಥಿರ float64_t f64_add( float64_t, float64_t );
ಸ್ಥಿರ float64_t f64_sub( float64_t, float64_t );
ಸ್ಥಿರ float64_t f64_mul( float64_t, float64_t );
ಸ್ಥಿರ float64_t f64_mulAdd( float64_t, float64_t, float64_t );
ಸ್ಥಿರ float64_t f64_div( float64_t, float64_t );
ಸ್ಥಿರ float64_t f64_rem( float64_t, float64_t );
ಸ್ಥಿರ float64_t f64_sqrt( float64_t );
ಸ್ಥಿರ ತಾರ್ಕಿಕ f64_eq( float64_t, float64_t );
ಸ್ಥಿರ ತಾರ್ಕಿಕ f64_le( float64_t, float64_t );
ಸ್ಥಿರ ತಾರ್ಕಿಕ f64_lt( float64_t, float64_t );

/*----------------------------------------------------------------------------
| Ported from OpenCV and fdlibm and added for usability
*----------------------------------------------------------------------------*/

ಸ್ಥಿರ float32_t f32_powi( float32_t x, ಪೂರ್ಣಾಙ್ಕ y);
ಸ್ಥಿರ float64_t f64_powi( float64_t x, ಪೂರ್ಣಾಙ್ಕ y);
ಸ್ಥಿರ float64_t f64_sin_kernel(float64_t x);
ಸ್ಥಿರ float64_t f64_cos_kernel(float64_t x);
ಸ್ಥಿರ ಶೂನ್ಯ f64_sincos_reduce(ಧ್ರುವ float64_t& x, float64_t& y, ಪೂರ್ಣಾಙ್ಕ& n);

ಸ್ಥಿರ float32_t f32_exp( float32_t x);
ಸ್ಥಿರ float64_t f64_exp(float64_t x);
ಸ್ಥಿರ float32_t f32_log(float32_t x);
ಸ್ಥಿರ float64_t f64_log(float64_t x);
ಸ್ಥಿರ float32_t f32_cbrt(float32_t x);
ಸ್ಥಿರ float32_t f32_pow( float32_t x, float32_t y);
ಸ್ಥಿರ float64_t f64_pow( float64_t x, float64_t y);
ಸ್ಥಿರ float64_t f64_sin( float64_t x );
ಸ್ಥಿರ float64_t f64_cos( float64_t x );

/*----------------------------------------------------------------------------
| softfloat and softdouble methods and members
*----------------------------------------------------------------------------*/

softfloat::softfloat( ಧ್ರುವ uint32_t a ) { *ಏತತ್ = ui32_to_f32(a); }
softfloat::softfloat( ಧ್ರುವ uint64_t a ) { *ಏತತ್ = ui64_to_f32(a); }
softfloat::softfloat( ಧ್ರುವ  int32_t a ) { *ಏತತ್ =  i32_to_f32(a); }
softfloat::softfloat( ಧ್ರುವ  int64_t a ) { *ಏತತ್ =  i64_to_f32(a); }

softfloat::operator softdouble() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ f32_to_f64(*ಏತತ್); }

softfloat softfloat::operator + (ಧ್ರುವ softfloat& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ f32_add(*ಏತತ್, a); }
softfloat softfloat::operator - (ಧ್ರುವ softfloat& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ f32_sub(*ಏತತ್, a); }
softfloat softfloat::operator * (ಧ್ರುವ softfloat& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ f32_mul(*ಏತತ್, a); }
softfloat softfloat::operator / (ಧ್ರುವ softfloat& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ f32_div(*ಏತತ್, a); }
softfloat softfloat::operator % (ಧ್ರುವ softfloat& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ f32_rem(*ಏತತ್, a); }

ತಾರ್ಕಿಕ softfloat::operator == ( ಧ್ರುವ softfloat& a ) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ  f32_eq(*ಏತತ್, a); }
ತಾರ್ಕಿಕ softfloat::operator != ( ಧ್ರುವ softfloat& a ) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !f32_eq(*ಏತತ್, a); }
ತಾರ್ಕಿಕ softfloat::operator >  ( ಧ್ರುವ softfloat& a ) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ  f32_lt(a, *ಏತತ್); }
ತಾರ್ಕಿಕ softfloat::operator >= ( ಧ್ರುವ softfloat& a ) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ  f32_le(a, *ಏತತ್); }
ತಾರ್ಕಿಕ softfloat::operator <  ( ಧ್ರುವ softfloat& a ) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ  f32_lt(*ಏತತ್, a); }
ತಾರ್ಕಿಕ softfloat::operator <= ( ಧ್ರುವ softfloat& a ) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ  f32_le(*ಏತತ್, a); }

softdouble::softdouble( ಧ್ರುವ uint32_t a ) { *ಏತತ್ = ui32_to_f64(a); }
softdouble::softdouble( ಧ್ರುವ uint64_t a ) { *ಏತತ್ = ui64_to_f64(a); }
softdouble::softdouble( ಧ್ರುವ  int32_t a ) { *ಏತತ್ =  i32_to_f64(a); }
softdouble::softdouble( ಧ್ರುವ  int64_t a ) { *ಏತತ್ =  i64_to_f64(a); }

}

ಪೂರ್ಣಾಙ್ಕ cvTrunc(ಧ್ರುವ cv::softfloat& a) { ಪ್ರತ್ಯಾಗಮ cv::f32_to_i32_r_minMag(a, ಅಸತ್ಯ); }
ಪೂರ್ಣಾಙ್ಕ cvRound(ಧ್ರುವ cv::softfloat& a) { ಪ್ರತ್ಯಾಗಮ cv::f32_to_i32(a, cv::round_near_even, ಅಸತ್ಯ); }
ಪೂರ್ಣಾಙ್ಕ cvFloor(ಧ್ರುವ cv::softfloat& a) { ಪ್ರತ್ಯಾಗಮ cv::f32_to_i32(a, cv::round_min, ಅಸತ್ಯ); }
ಪೂರ್ಣಾಙ್ಕ cvCeil (ಧ್ರುವ cv::softfloat& a) { ಪ್ರತ್ಯಾಗಮ cv::f32_to_i32(a, cv::round_max, ಅಸತ್ಯ); }

ಪೂರ್ಣಾಙ್ಕ cvTrunc(ಧ್ರುವ cv::softdouble& a) { ಪ್ರತ್ಯಾಗಮ cv::f64_to_i32_r_minMag(a, ಅಸತ್ಯ); }
ಪೂರ್ಣಾಙ್ಕ cvRound(ಧ್ರುವ cv::softdouble& a) { ಪ್ರತ್ಯಾಗಮ cv::f64_to_i32(a, cv::round_near_even, ಅಸತ್ಯ); }
ಪೂರ್ಣಾಙ್ಕ cvFloor(ಧ್ರುವ cv::softdouble& a) { ಪ್ರತ್ಯಾಗಮ cv::f64_to_i32(a, cv::round_min, ಅಸತ್ಯ); }
ಪೂರ್ಣಾಙ್ಕ cvCeil (ಧ್ರುವ cv::softdouble& a) { ಪ್ರತ್ಯಾಗಮ cv::f64_to_i32(a, cv::round_max, ಅಸತ್ಯ); }

int64_t cvRound64(ಧ್ರುವ cv::softdouble& a) { ಪ್ರತ್ಯಾಗಮ cv::f64_to_i64(a, cv::round_near_even, ಅಸತ್ಯ); }

ನಾಮಾಕಾಶ cv
{
softdouble::operator softfloat() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ f64_to_f32(*ಏತತ್); }

softdouble softdouble::operator + (ಧ್ರುವ softdouble& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ f64_add(*ಏತತ್, a); }
softdouble softdouble::operator - (ಧ್ರುವ softdouble& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ f64_sub(*ಏತತ್, a); }
softdouble softdouble::operator * (ಧ್ರುವ softdouble& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ f64_mul(*ಏತತ್, a); }
softdouble softdouble::operator / (ಧ್ರುವ softdouble& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ f64_div(*ಏತತ್, a); }
softdouble softdouble::operator % (ಧ್ರುವ softdouble& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ f64_rem(*ಏತತ್, a); }

ತಾರ್ಕಿಕ softdouble::operator == (ಧ್ರುವ softdouble& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ  f64_eq(*ಏತತ್, a); }
ತಾರ್ಕಿಕ softdouble::operator != (ಧ್ರುವ softdouble& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !f64_eq(*ಏತತ್, a); }
ತಾರ್ಕಿಕ softdouble::operator >  (ಧ್ರುವ softdouble& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ  f64_lt(a, *ಏತತ್); }
ತಾರ್ಕಿಕ softdouble::operator >= (ಧ್ರುವ softdouble& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ  f64_le(a, *ಏತತ್); }
ತಾರ್ಕಿಕ softdouble::operator <  (ಧ್ರುವ softdouble& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ  f64_lt(*ಏತತ್, a); }
ತಾರ್ಕಿಕ softdouble::operator <= (ಧ್ರುವ softdouble& a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ  f64_le(*ಏತತ್, a); }

/*----------------------------------------------------------------------------
| Overloads for math functions
*----------------------------------------------------------------------------*/

softfloat  mulAdd( ಧ್ರುವ softfloat&  a, ಧ್ರುವ softfloat&  b, ಧ್ರುವ softfloat & c) { ಪ್ರತ್ಯಾಗಮ f32_mulAdd(a, b, c); }
softdouble mulAdd( ಧ್ರುವ softdouble& a, ಧ್ರುವ softdouble& b, ಧ್ರುವ softdouble& c) { ಪ್ರತ್ಯಾಗಮ f64_mulAdd(a, b, c); }

softfloat  sqrt( ಧ್ರುವ softfloat&  a ) { ಪ್ರತ್ಯಾಗಮ f32_sqrt(a); }
softdouble sqrt( ಧ್ರುವ softdouble& a ) { ಪ್ರತ್ಯಾಗಮ f64_sqrt(a); }

softfloat  exp( ಧ್ರುವ softfloat&  a) { ಪ್ರತ್ಯಾಗಮ f32_exp(a); }
softdouble exp( ಧ್ರುವ softdouble& a) { ಪ್ರತ್ಯಾಗಮ f64_exp(a); }

softfloat  log( ಧ್ರುವ softfloat&  a ) { ಪ್ರತ್ಯಾಗಮ f32_log(a); }
softdouble log( ಧ್ರುವ softdouble& a ) { ಪ್ರತ್ಯಾಗಮ f64_log(a); }

softfloat  pow( ಧ್ರುವ softfloat&  a, ಧ್ರುವ softfloat&  b) { ಪ್ರತ್ಯಾಗಮ f32_pow(a, b); }
softdouble pow( ಧ್ರುವ softdouble& a, ಧ್ರುವ softdouble& b) { ಪ್ರತ್ಯಾಗಮ f64_pow(a, b); }

softfloat cbrt(ಧ್ರುವ softfloat& a) { ಪ್ರತ್ಯಾಗಮ f32_cbrt(a); }

softdouble sin(ಧ್ರುವ softdouble& a) { ಪ್ರತ್ಯಾಗಮ f64_sin(a); }
softdouble cos(ಧ್ರುವ softdouble& a) { ಪ್ರತ್ಯಾಗಮ f64_cos(a); }

/*----------------------------------------------------------------------------
| The values to return on conversions to 32-bit integer formats that raise an
| invalid exception.
*----------------------------------------------------------------------------*/
#define i32_fromPosOverflow  0x7FFFFFFF
#define i32_fromNegOverflow  (-0x7FFFFFFF - 1)
#define i32_fromNaN          0x7FFFFFFF

/*----------------------------------------------------------------------------
| The values to return on conversions to 64-bit integer formats that raise an
| invalid exception.
*----------------------------------------------------------------------------*/
#define i64_fromPosOverflow  UINT64_C( 0x7FFFFFFFFFFFFFFF )
//fixed unsigned unary minus: -x == ~x + 1
//#define i64_fromNegOverflow (-UINT64_C( 0x7FFFFFFFFFFFFFFF ) - 1)
#define i64_fromNegOverflow  (~UINT64_C( 0x7FFFFFFFFFFFFFFF ) + 1 - 1)
#define i64_fromNaN          UINT64_C( 0x7FFFFFFFFFFFFFFF )

/*----------------------------------------------------------------------------
| "Common NaN" structure, used to transfer NaN representations from one format
| to another.
*----------------------------------------------------------------------------*/
ಸಂರಚನಾ commonNaN {
    ತಾರ್ಕಿಕ sign;
#ifndef ಪದಗಳು_BIGENDIAN
    uint64_t v0, v64;
#ಅನ್ಯಥಾ
    uint64_t v64, v0;
#endif
};

/*----------------------------------------------------------------------------
| The bit pattern for a default generated 32-bit floating-point NaN.
*----------------------------------------------------------------------------*/
#define defaultNaNF32UI 0xFFC00000

/*----------------------------------------------------------------------------
| Returns true when 32-bit unsigned integer `uiA' has the bit pattern of a
| 32-bit floating-point signaling NaN.
| Note:  This macro evaluates its argument more than once.
*----------------------------------------------------------------------------*/
#define softfloat_isSigNaNF32UI( uiA ) ((((uiA) & 0x7FC00000) == 0x7F800000) && ((uiA) & 0x003FFFFF))

/*----------------------------------------------------------------------------
| Assuming `uiA' has the bit pattern of a 32-bit floating-point NaN, converts
| this NaN to the common NaN form, and stores the resulting common NaN at the
| location pointed to by `zPtr'.  If the NaN is a signaling NaN, the invalid
| exception is raised.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ ಶೂನ್ಯ softfloat_f32UIToCommonNaN( uint_fast32_t uiA, ಸಂರಚನಾ commonNaN *zPtr );

/*----------------------------------------------------------------------------
| Converts the common NaN pointed to by `aPtr' into a 32-bit floating-point
| NaN, and returns the bit pattern of this value as an unsigned integer.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ uint_fast32_t softfloat_commonNaNToF32UI( ಧ್ರುವ ಸಂರಚನಾ commonNaN *aPtr );

/*----------------------------------------------------------------------------
| Interpreting `uiA' and `uiB' as the bit patterns of two 32-bit floating-
| point values, at least one of which is a NaN, returns the bit pattern of
| the combined NaN result.  If either `uiA' or `uiB' has the pattern of a
| signaling NaN, the invalid exception is raised.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ uint_fast32_t softfloat_propagateNaNF32UI( uint_fast32_t uiA, uint_fast32_t uiB );

/*----------------------------------------------------------------------------
| The bit pattern for a default generated 64-bit floating-point NaN.
*----------------------------------------------------------------------------*/
#define defaultNaNF64UI UINT64_C( 0xFFF8000000000000 )

/*----------------------------------------------------------------------------
| Returns true when 64-bit unsigned integer `uiA' has the bit pattern of a
| 64-bit floating-point signaling NaN.
| Note:  This macro evaluates its argument more than once.
*----------------------------------------------------------------------------*/
#define softfloat_isSigNaNF64UI( uiA ) \
    ((((uiA) & UINT64_C( 0x7FF8000000000000 )) == UINT64_C( 0x7FF0000000000000 )) && \
      ((uiA) & UINT64_C( 0x0007FFFFFFFFFFFF )))

/*----------------------------------------------------------------------------
| Assuming `uiA' has the bit pattern of a 64-bit floating-point NaN, converts
| this NaN to the common NaN form, and stores the resulting common NaN at the
| location pointed to by `zPtr'.  If the NaN is a signaling NaN, the invalid
| exception is raised.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ ಶೂನ್ಯ softfloat_f64UIToCommonNaN( uint_fast64_t uiA, ಸಂರಚನಾ commonNaN *zPtr );

/*----------------------------------------------------------------------------
| Converts the common NaN pointed to by `aPtr' into a 64-bit floating-point
| NaN, and returns the bit pattern of this value as an unsigned integer.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ uint_fast64_t softfloat_commonNaNToF64UI( ಧ್ರುವ ಸಂರಚನಾ commonNaN *aPtr );

/*----------------------------------------------------------------------------
| Interpreting `uiA' and `uiB' as the bit patterns of two 64-bit floating-
| point values, at least one of which is a NaN, returns the bit pattern of
| the combined NaN result.  If either `uiA' or `uiB' has the pattern of a
| signaling NaN, the invalid exception is raised.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ uint_fast64_t softfloat_propagateNaNF64UI( uint_fast64_t uiA, uint_fast64_t uiB );

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/

#ifndef ಪದಗಳು_BIGENDIAN
ಸಂರಚನಾ uint128 { uint64_t v0, v64; };
ಸಂರಚನಾ uint64_extra { uint64_t extra, v; };
ಸಂರಚನಾ uint128_extra { uint64_t extra; ಸಂರಚನಾ uint128 v; };
#ಅನ್ಯಥಾ
ಸಂರಚನಾ uint128 { uint64_t v64, v0; };
ಸಂರಚನಾ uint64_extra { uint64_t v, extra; };
ಸಂರಚನಾ uint128_extra { ಸಂರಚನಾ uint128 v; uint64_t extra; };
#endif

ಗಣನಾ {
    softfloat_mulAdd_subC    = 1,
    softfloat_mulAdd_subProd = 2
};

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ int_fast32_t softfloat_roundToI32( ತಾರ್ಕಿಕ, uint_fast64_t, uint_fast8_t, ತಾರ್ಕಿಕ );

ಸಂರಚನಾ exp16_sig32 { int_fast16_t exp; uint_fast32_t sig; };
ಸ್ಥಿರ ಸಂರಚನಾ exp16_sig32 softfloat_normSubnormalF32Sig( uint_fast32_t );

ಸ್ಥಿರ float32_t softfloat_roundPackToF32( ತಾರ್ಕಿಕ, int_fast16_t, uint_fast32_t );
ಸ್ಥಿರ float32_t softfloat_normRoundPackToF32( ತಾರ್ಕಿಕ, int_fast16_t, uint_fast32_t );

ಸ್ಥಿರ float32_t softfloat_addMagsF32( uint_fast32_t, uint_fast32_t );
ಸ್ಥಿರ float32_t softfloat_subMagsF32( uint_fast32_t, uint_fast32_t );
ಸ್ಥಿರ float32_t softfloat_mulAddF32(uint_fast32_t, uint_fast32_t, uint_fast32_t, uint_fast8_t );

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ int_fast64_t softfloat_roundToI64( ತಾರ್ಕಿಕ, uint_fast64_t, uint_fast64_t, uint_fast8_t, ತಾರ್ಕಿಕ);

ಸಂರಚನಾ exp16_sig64 { int_fast16_t exp; uint_fast64_t sig; };
ಸ್ಥಿರ ಸಂರಚನಾ exp16_sig64 softfloat_normSubnormalF64Sig( uint_fast64_t );

ಸ್ಥಿರ float64_t softfloat_roundPackToF64( ತಾರ್ಕಿಕ, int_fast16_t, uint_fast64_t );
ಸ್ಥಿರ float64_t softfloat_normRoundPackToF64( ತಾರ್ಕಿಕ, int_fast16_t, uint_fast64_t );

ಸ್ಥಿರ float64_t softfloat_addMagsF64( uint_fast64_t, uint_fast64_t, ತಾರ್ಕಿಕ );
ಸ್ಥಿರ float64_t softfloat_subMagsF64( uint_fast64_t, uint_fast64_t, ತಾರ್ಕಿಕ );
ಸ್ಥಿರ float64_t softfloat_mulAddF64( uint_fast64_t, uint_fast64_t, uint_fast64_t, uint_fast8_t );

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
| Shifts 'a' right by the number of bits given in 'dist', which must be in
| the range 1 to 63.  If any nonzero bits are shifted off, they are "jammed"
| into the least-significant bit of the shifted value by setting the least-
| significant bit to 1.  This shifted-and-jammed value is returned.
*----------------------------------------------------------------------------*/

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint64_t softfloat_short_Shift_ಬಲ_Jam64( uint64_t a, uint_fast8_t dist )
{ ಪ್ರತ್ಯಾಗಮ a>>dist | ((a & (((uint_fast64_t) 1<<dist) - 1)) != 0); }

/*----------------------------------------------------------------------------
| Shifts 'a' right by the number of bits given in 'dist', which must not
| be zero.  If any nonzero bits are shifted off, they are "jammed" into the
| least-significant bit of the shifted value by setting the least-significant
| bit to 1.  This shifted-and-jammed value is returned.
|   The value of 'dist' can be arbitrarily large.  In particular, if 'dist' is
| greater than 32, the result will be either 0 or 1, depending on whether 'a'
| is zero or nonzero.
*----------------------------------------------------------------------------*/

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint32_t softfloat_shift_ಬಲ_Jam32( uint32_t a, uint_fast16_t dist )
{
    //fixed unsigned unary minus: -x == ~x + 1
    ಪ್ರತ್ಯಾಗಮ (dist < 31) ? a>>dist | ((uint32_t) (a<<((~dist + 1) & 31)) != 0) : (a != 0);
}

/*----------------------------------------------------------------------------
| Shifts 'a' right by the number of bits given in 'dist', which must not
| be zero.  If any nonzero bits are shifted off, they are "jammed" into the
| least-significant bit of the shifted value by setting the least-significant
| bit to 1.  This shifted-and-jammed value is returned.
|   The value of 'dist' can be arbitrarily large.  In particular, if 'dist' is
| greater than 64, the result will be either 0 or 1, depending on whether 'a'
| is zero or nonzero.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint64_t softfloat_shift_ಬಲ_Jam64( uint64_t a, uint_fast32_t dist )
{
    //fixed unsigned unary minus: -x == ~x + 1
    ಪ್ರತ್ಯಾಗಮ (dist < 63) ? a>>dist | ((uint64_t) (a<<((~dist + 1) & 63)) != 0) : (a != 0);
}

/*----------------------------------------------------------------------------
| A constant table that translates an 8-bit unsigned integer (the array index)
| into the number of leading 0 bits before the most-significant 1 of that
| integer.  For integer zero (index 0), the corresponding table element is 8.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ ಧ್ರುವ uint_least8_t softfloat_ಎಣಿಕೆ_Leading_Zeros8[256] = {
    8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*----------------------------------------------------------------------------
| Returns the number of leading 0 bits before the most-significant 1 bit of
| 'a'.  If 'a' is zero, 32 is returned.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint_fast8_t softfloat_ಎಣಿಕೆ_Leading_Zeros32( uint32_t a )
{
    uint_fast8_t ಎಣಿಕೆ_2 = 0;
    ಯದಿ ( a < 0x10000 ) {
        ಎಣಿಕೆ_2 = 16;
        a <<= 16;
    }
    ಯದಿ ( a < 0x1000000 ) {
        ಎಣಿಕೆ_2 += 8;
        a <<= 8;
    }
    ಎಣಿಕೆ_2 += softfloat_ಎಣಿಕೆ_Leading_Zeros8[a>>24];
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ_2;
}

/*----------------------------------------------------------------------------
| Returns the number of leading 0 bits before the most-significant 1 bit of
| 'a'.  If 'a' is zero, 64 is returned.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ uint_fast8_t softfloat_ಎಣಿಕೆ_Leading_Zeros64( uint64_t a );

/*----------------------------------------------------------------------------
| Returns an approximation to the reciprocal of the number represented by 'a',
| where 'a' is interpreted as an unsigned fixed-point number with one integer
| bit and 31 fraction bits.  The 'a' input must be "normalized", meaning that
| its most-significant bit (bit 31) must be 1.  Thus, if A is the value of
| the fixed-point interpretation of 'a', then 1 <= A < 2.  The returned value
| is interpreted as a pure unsigned fraction, having no integer bits and 32
| fraction bits.  The approximation returned is never greater than the true
| reciprocal 1/A, and it differs from the true reciprocal by at most 2.006 ulp
| (units in the last place).
*----------------------------------------------------------------------------*/
#define softfloat_approxRecip32_1( a ) ((uint32_t) (UINT64_C( 0x7FFFFFFFFFFFFFFF ) / (uint32_t) (a)))

/*----------------------------------------------------------------------------
| Returns an approximation to the reciprocal of the square root of the number
| represented by 'a', where 'a' is interpreted as an unsigned fixed-point
| number either with one integer bit and 31 fraction bits or with two integer
| bits and 30 fraction bits.  The format of 'a' is determined by 'oddExpA',
| which must be either 0 or 1.  If 'oddExpA' is 1, 'a' is interpreted as
| having one integer bit, and if 'oddExpA' is 0, 'a' is interpreted as having
| two integer bits.  The 'a' input must be "normalized", meaning that its
| most-significant bit (bit 31) must be 1.  Thus, if A is the value of the
| fixed-point interpretation of 'a', it follows that 1 <= A < 2 when 'oddExpA'
| is 1, and 2 <= A < 4 when 'oddExpA' is 0.
|   The returned value is interpreted as a pure unsigned fraction, having
| no integer bits and 32 fraction bits.  The approximation returned is never
| greater than the true reciprocal 1/sqrt(A), and it differs from the true
| reciprocal by at most 2.06 ulp (units in the last place).  The approximation
| returned is also always within the range 0.5 to 1; thus, the most-
| significant bit of the result is always set.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ uint32_t softfloat_approxRecipSqrt32_1( unsigned ಪೂರ್ಣಾಙ್ಕ oddExpA, uint32_t a );

ಸ್ಥಿರ ಧ್ರುವ uint16_t softfloat_approxRecipSqrt_1k0s[16] = {
    0xB4C9, 0xFFAB, 0xAA7D, 0xF11C, 0xA1C5, 0xE4C7, 0x9A43, 0xDA29,
    0x93B5, 0xD0E5, 0x8DED, 0xC8B7, 0x88C6, 0xC16D, 0x8424, 0xBAE1
};
ಸ್ಥಿರ ಧ್ರುವ uint16_t softfloat_approxRecipSqrt_1k1s[16] = {
    0xA5A5, 0xEA42, 0x8C21, 0xC62D, 0x788F, 0xAA7F, 0x6928, 0x94B6,
    0x5CC7, 0x8335, 0x52A6, 0x74E2, 0x4A3E, 0x68FE, 0x432B, 0x5EFD
};

/*----------------------------------------------------------------------------
| Shifts the 128 bits formed by concatenating 'a64' and 'a0' left by the
| number of bits given in 'dist', which must be in the range 1 to 63.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ uint128 softfloat_shortShiftLeft128( uint64_t a64, uint64_t a0, uint_fast8_t dist )
{
    ಸಂರಚನಾ uint128 z;
    z.v64 = a64<<dist | a0>>(-dist & 63);
    z.v0 = a0<<dist;
    ಪ್ರತ್ಯಾಗಮ z;
}

/*----------------------------------------------------------------------------
| Shifts the 128 bits formed by concatenating 'a64' and 'a0' right by the
| number of bits given in 'dist', which must be in the range 1 to 63.  If any
| nonzero bits are shifted off, they are "jammed" into the least-significant
| bit of the shifted value by setting the least-significant bit to 1.  This
| shifted-and-jammed value is returned.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ uint128 softfloat_short_Shift_ಬಲ_Jam128(uint64_t a64, uint64_t a0, uint_fast8_t dist )
{
    uint_fast8_t negDist = -dist;
    ಸಂರಚನಾ uint128 z;
    z.v64 = a64>>dist;
    z.v0 =
        a64<<(negDist & 63) | a0>>dist
            | ((uint64_t) (a0<<(negDist & 63)) != 0);
    ಪ್ರತ್ಯಾಗಮ z;
}

/*----------------------------------------------------------------------------
| Shifts the 128 bits formed by concatenating 'a64' and 'a0' right by the
| number of bits given in 'dist', which must not be zero.  If any nonzero bits
| are shifted off, they are "jammed" into the least-significant bit of the
| shifted value by setting the least-significant bit to 1.  This shifted-and-
| jammed value is returned.
|   The value of 'dist' can be arbitrarily large.  In particular, if 'dist' is
| greater than 128, the result will be either 0 or 1, depending on whether the
| original 128 bits are all zeros.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ ಸಂರಚನಾ uint128 softfloat_shift_ಬಲ_Jam128( uint64_t a64, uint64_t a0, uint_fast32_t dist );

/*----------------------------------------------------------------------------
| Returns the sum of the 128-bit integer formed by concatenating 'a64' and
| 'a0' and the 128-bit integer formed by concatenating 'b64' and 'b0'.  The
| addition is modulo 2^128, so any carry out is lost.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ uint128 softfloat_add128( uint64_t a64, uint64_t a0, uint64_t b64, uint64_t b0 )
{
    ಸಂರಚನಾ uint128 z;
    z.v0 = a0 + b0;
    z.v64 = a64 + b64 + (z.v0 < a0);
    ಪ್ರತ್ಯಾಗಮ z;
}

/*----------------------------------------------------------------------------
| Returns the difference of the 128-bit integer formed by concatenating 'a64'
| and 'a0' and the 128-bit integer formed by concatenating 'b64' and 'b0'.
| The subtraction is modulo 2^128, so any borrow out (carry out) is lost.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ uint128 softfloat_sub128( uint64_t a64, uint64_t a0, uint64_t b64, uint64_t b0 )
{
    ಸಂರಚನಾ uint128 z;
    z.v0 = a0 - b0;
    z.v64 = a64 - b64;
    z.v64 -= (a0 < b0);
    ಪ್ರತ್ಯಾಗಮ z;
}

/*----------------------------------------------------------------------------
| Returns the 128-bit product of 'a' and 'b'.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ ಸಂರಚನಾ uint128 softfloat_mul64To128( uint64_t a, uint64_t b );

/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
*----------------------------------------------------------------------------*/

ಸ್ಥಿರ float32_t f32_add( float32_t a, float32_t b )
{
    uint_fast32_t uiA = a.v;
    uint_fast32_t uiB = b.v;

    ಯದಿ ( signF32UI( uiA ^ uiB ) ) {
        ಪ್ರತ್ಯಾಗಮ softfloat_subMagsF32( uiA, uiB );
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ softfloat_addMagsF32( uiA, uiB );
    }
}

ಸ್ಥಿರ float32_t f32_div( float32_t a, float32_t b )
{
    uint_fast32_t uiA;
    ತಾರ್ಕಿಕ signA;
    int_fast16_t expA;
    uint_fast32_t sigA;
    uint_fast32_t uiB;
    ತಾರ್ಕಿಕ signB;
    int_fast16_t expB;
    uint_fast32_t sigB;
    ತಾರ್ಕಿಕ signZ;
    ಸಂರಚನಾ exp16_sig32 normExpSig;
    int_fast16_t expZ;
    uint_fast64_t sig64A;
    uint_fast32_t sigZ;
    uint_fast32_t uiZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    signA = signF32UI( uiA );
    expA  = expF32UI( uiA );
    sigA  = fracF32UI( uiA );
    uiB = b.v;
    signB = signF32UI( uiB );
    expB  = expF32UI( uiB );
    sigB  = fracF32UI( uiB );
    signZ = signA ^ signB;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( expA == 0xFF ) {
        ಯದಿ ( sigA ) ಗಚ್ಛ propagateNaN;
        ಯದಿ ( expB == 0xFF ) {
            ಯದಿ ( sigB ) ಗಚ್ಛ propagateNaN;
            ಗಚ್ಛ invalid;
        }
        ಗಚ್ಛ infinity;
    }
    ಯದಿ ( expB == 0xFF ) {
        ಯದಿ ( sigB ) ಗಚ್ಛ propagateNaN;
        ಗಚ್ಛ zero;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( ! expB ) {
        ಯದಿ ( ! sigB ) {
            ಯದಿ ( ! (expA | sigA) ) ಗಚ್ಛ invalid;
            raiseFlags( flag_infinite );
            ಗಚ್ಛ infinity;
        }
        normExpSig = softfloat_normSubnormalF32Sig( sigB );
        expB = normExpSig.exp;
        sigB = normExpSig.sig;
    }
    ಯದಿ ( ! expA ) {
        ಯದಿ ( ! sigA ) ಗಚ್ಛ zero;
        normExpSig = softfloat_normSubnormalF32Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expZ = expA - expB + 0x7E;
    sigA |= 0x00800000;
    sigB |= 0x00800000;
    ಯದಿ ( sigA < sigB ) {
        --expZ;
        sig64A = (uint_fast64_t) sigA<<31;
    } ಅನ್ಯಥಾ {
        sig64A = (uint_fast64_t) sigA<<30;
    }
    sigZ = (uint_fast32_t)(sig64A / sigB); // fixed warning on type cast
    ಯದಿ ( ! (sigZ & 0x3F) ) sigZ |= ((uint_fast64_t) sigB * sigZ != sig64A);
    ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF32( signZ, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = softfloat_propagateNaNF32UI( uiA, uiB );
    ಗಚ್ಛ uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 invalid:
    raiseFlags( flag_invalid );
    uiZ = defaultNaNF32UI;
    ಗಚ್ಛ uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 infinity:
    uiZ = packToF32UI( signZ, 0xFF, 0 );
    ಗಚ್ಛ uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 zero:
    uiZ = packToF32UI( signZ, 0, 0 );
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(uiZ);
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ f32_eq( float32_t a, float32_t b )
{
    uint_fast32_t uiA;
    uint_fast32_t uiB;

    uiA = a.v;
    uiB = b.v;
    ಯದಿ ( isNaNF32UI( uiA ) || isNaNF32UI( uiB ) )
    {
        ಯದಿ (softfloat_isSigNaNF32UI( uiA ) || softfloat_isSigNaNF32UI( uiB ) )
            raiseFlags( flag_invalid );
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ (uiA == uiB) || ! (uint32_t) ((uiA | uiB)<<1);
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ f32_le( float32_t a, float32_t b )
{
    uint_fast32_t uiA;
    uint_fast32_t uiB;
    ತಾರ್ಕಿಕ signA, signB;

    uiA = a.v;
    uiB = b.v;
    ಯದಿ ( isNaNF32UI( uiA ) || isNaNF32UI( uiB ) )
    {
        raiseFlags( flag_invalid );
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    signA = signF32UI( uiA );
    signB = signF32UI( uiB );
    ಪ್ರತ್ಯಾಗಮ (signA != signB) ? signA || ! (uint32_t) ((uiA | uiB)<<1)
                            : (uiA == uiB) || (signA ^ (uiA < uiB));
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ f32_lt( float32_t a, float32_t b )
{
    uint_fast32_t uiA;
    uint_fast32_t uiB;
    ತಾರ್ಕಿಕ signA, signB;

    uiA = a.v; uiB = b.v;
    ಯದಿ ( isNaNF32UI( uiA ) || isNaNF32UI( uiB ) )
    {
        raiseFlags( flag_invalid );
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    signA = signF32UI( uiA );
    signB = signF32UI( uiB );
    ಪ್ರತ್ಯಾಗಮ (signA != signB) ? signA && ((uint32_t) ((uiA | uiB)<<1) != 0)
                            : (uiA != uiB) && (signA ^ (uiA < uiB));
}

ಸ್ಥಿರ float32_t f32_mulAdd( float32_t a, float32_t b, float32_t c )
{
    uint_fast32_t uiA;
    uint_fast32_t uiB;
    uint_fast32_t uiC;

    uiA = a.v;
    uiB = b.v;
    uiC = c.v;
    ಪ್ರತ್ಯಾಗಮ softfloat_mulAddF32( uiA, uiB, uiC, 0 );
}

ಸ್ಥಿರ float32_t f32_mul( float32_t a, float32_t b )
{
    uint_fast32_t uiA;
    ತಾರ್ಕಿಕ signA;
    int_fast16_t expA;
    uint_fast32_t sigA;
    uint_fast32_t uiB;
    ತಾರ್ಕಿಕ signB;
    int_fast16_t expB;
    uint_fast32_t sigB;
    ತಾರ್ಕಿಕ signZ;
    uint_fast32_t magBits;
    ಸಂರಚನಾ exp16_sig32 normExpSig;
    int_fast16_t expZ;
    uint_fast32_t sigZ, uiZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    signA = signF32UI( uiA );
    expA  = expF32UI( uiA );
    sigA  = fracF32UI( uiA );
    uiB = b.v;
    signB = signF32UI( uiB );
    expB  = expF32UI( uiB );
    sigB  = fracF32UI( uiB );
    signZ = signA ^ signB;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( expA == 0xFF ) {
        ಯದಿ ( sigA || ((expB == 0xFF) && sigB) ) ಗಚ್ಛ propagateNaN;
        magBits = expB | sigB;
        ಗಚ್ಛ infArg;
    }
    ಯದಿ ( expB == 0xFF ) {
        ಯದಿ ( sigB ) ಗಚ್ಛ propagateNaN;
        magBits = expA | sigA;
        ಗಚ್ಛ infArg;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( ! expA ) {
        ಯದಿ ( ! sigA ) ಗಚ್ಛ zero;
        normExpSig = softfloat_normSubnormalF32Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }
    ಯದಿ ( ! expB ) {
        ಯದಿ ( ! sigB ) ಗಚ್ಛ zero;
        normExpSig = softfloat_normSubnormalF32Sig( sigB );
        expB = normExpSig.exp;
        sigB = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expZ = expA + expB - 0x7F;
    sigA = (sigA | 0x00800000)<<7;
    sigB = (sigB | 0x00800000)<<8;
    sigZ = (uint_fast32_t)softfloat_short_Shift_ಬಲ_Jam64( (uint_fast64_t) sigA * sigB, 32 ); //fixed warning on type cast
    ಯದಿ ( sigZ < 0x40000000 ) {
        --expZ;
        sigZ <<= 1;
    }
    ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF32( signZ, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = softfloat_propagateNaNF32UI( uiA, uiB );
    ಗಚ್ಛ uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 infArg:
    ಯದಿ ( ! magBits ) {
        raiseFlags( flag_invalid );
        uiZ = defaultNaNF32UI;
    } ಅನ್ಯಥಾ {
        uiZ = packToF32UI( signZ, 0xFF, 0 );
    }
    ಗಚ್ಛ uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 zero:
    uiZ = packToF32UI( signZ, 0, 0 );
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float32_t f32_rem( float32_t a, float32_t b )
{
    uint_fast32_t uiA;
    ತಾರ್ಕಿಕ signA;
    int_fast16_t expA;
    uint_fast32_t sigA;
    uint_fast32_t uiB;
    int_fast16_t expB;
    uint_fast32_t sigB;
    ಸಂರಚನಾ exp16_sig32 normExpSig;
    uint32_t rem;
    int_fast16_t expDiff;
    uint32_t q, recip32, altRem, meanRem;
    ತಾರ್ಕಿಕ signRem;
    uint_fast32_t uiZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    signA = signF32UI( uiA );
    expA  = expF32UI( uiA );
    sigA  = fracF32UI( uiA );
    uiB = b.v;
    expB = expF32UI( uiB );
    sigB = fracF32UI( uiB );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( expA == 0xFF ) {
        ಯದಿ ( sigA || ((expB == 0xFF) && sigB) ) ಗಚ್ಛ propagateNaN;
        ಗಚ್ಛ invalid;
    }
    ಯದಿ ( expB == 0xFF ) {
        ಯದಿ ( sigB ) ಗಚ್ಛ propagateNaN;
        ಪ್ರತ್ಯಾಗಮ a;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( ! expB ) {
        ಯದಿ ( ! sigB ) ಗಚ್ಛ invalid;
        normExpSig = softfloat_normSubnormalF32Sig( sigB );
        expB = normExpSig.exp;
        sigB = normExpSig.sig;
    }
    ಯದಿ ( ! expA ) {
        ಯದಿ ( ! sigA ) ಪ್ರತ್ಯಾಗಮ a;
        normExpSig = softfloat_normSubnormalF32Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    rem = sigA | 0x00800000;
    sigB |= 0x00800000;
    expDiff = expA - expB;
    ಯದಿ ( expDiff < 1 ) {
        ಯದಿ ( expDiff < -1 ) ಪ್ರತ್ಯಾಗಮ a;
        sigB <<= 6;
        ಯದಿ ( expDiff ) {
            rem <<= 5;
            q = 0;
        } ಅನ್ಯಥಾ {
            rem <<= 6;
            q = (sigB <= rem);
            ಯದಿ ( q ) rem -= sigB;
        }
    } ಅನ್ಯಥಾ {
        recip32 = softfloat_approxRecip32_1( sigB<<8 );
        /*--------------------------------------------------------------------
        | Changing the shift of `rem' here requires also changing the initial
        | subtraction from `expDiff'.
        *--------------------------------------------------------------------*/
        rem <<= 7;
        expDiff -= 31;
        /*--------------------------------------------------------------------
        | The scale of `sigB' affects how many bits are obtained during each
        | cycle of the loop.  Currently this is 29 bits per loop iteration,
        | which is believed to be the maximum possible.
        *--------------------------------------------------------------------*/
        sigB <<= 6;
        ಪ್ರತಿ (;;) {
            q = (rem * (uint_fast64_t) recip32)>>32;
            ಯದಿ ( expDiff < 0 ) ಭಙ್ಗ;
            //fixed unsigned unary minus: -x == ~x + 1
            rem = ~(q * (uint32_t) sigB) + 1;
            expDiff -= 29;
        }
        /*--------------------------------------------------------------------
        | (`expDiff' cannot be less than -30 here.)
        *--------------------------------------------------------------------*/
        q >>= ~expDiff & 31;
        rem = (rem<<(expDiff + 30)) - q * (uint32_t) sigB;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಕುರು {
        altRem = rem;
        ++q;
        rem -= sigB;
    } ಯಾವತ್ ( ! (rem & 0x80000000) );
    meanRem = rem + altRem;
    ಯದಿ ( (meanRem & 0x80000000) || (! meanRem && (q & 1)) ) rem = altRem;
    signRem = signA;
    ಯದಿ ( 0x80000000 <= rem ) {
        signRem = ! signRem;
        //fixed unsigned unary minus: -x == ~x + 1
        rem = ~rem + 1;
    }
    ಪ್ರತ್ಯಾಗಮ softfloat_normRoundPackToF32( signRem, expB, rem );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = softfloat_propagateNaNF32UI( uiA, uiB );
    ಗಚ್ಛ uiZ;
 invalid:
    raiseFlags( flag_invalid );
    uiZ = defaultNaNF32UI;
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float32_t f32_roundToInt( float32_t a, uint_fast8_t roundingMode, ತಾರ್ಕಿಕ exact )
{
    uint_fast32_t uiA;
    int_fast16_t exp;
    uint_fast32_t uiZ, lastBitMask, roundBitsMask;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    exp = expF32UI( uiA );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( exp <= 0x7E ) {
        ಯದಿ ( ! (uint32_t) (uiA<<1) ) ಪ್ರತ್ಯಾಗಮ a;
        ಯದಿ ( exact ) raiseFlags(flag_inexact);
        uiZ = uiA & packToF32UI( 1, 0, 0 );
        ವಿಕಲ್ಪ ( roundingMode ) {
         ಪಕ್ಷ round_near_even:
            ಯದಿ ( ! fracF32UI( uiA ) ) ಭಙ್ಗ;
            [[fallthrough]];
         ಪಕ್ಷ round_near_maxMag:
            ಯದಿ ( exp == 0x7E ) uiZ |= packToF32UI( 0, 0x7F, 0 );
            ಭಙ್ಗ;
         ಪಕ್ಷ round_min:
            ಯದಿ ( uiZ ) uiZ = packToF32UI( 1, 0x7F, 0 );
            ಭಙ್ಗ;
         ಪಕ್ಷ round_max:
            ಯದಿ ( ! uiZ ) uiZ = packToF32UI( 0, 0x7F, 0 );
            ಭಙ್ಗ;
        }
        ಗಚ್ಛ uiZ;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( 0x96 <= exp ) {
        ಯದಿ ( (exp == 0xFF) && fracF32UI( uiA ) ) {
            uiZ = softfloat_propagateNaNF32UI( uiA, 0 );
            ಗಚ್ಛ uiZ;
        }
        ಪ್ರತ್ಯಾಗಮ a;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiZ = uiA;
    lastBitMask = (uint_fast32_t) 1<<(0x96 - exp);
    roundBitsMask = lastBitMask - 1;
    ಯದಿ ( roundingMode == round_near_maxMag ) {
        uiZ += lastBitMask>>1;
    } ಅನ್ಯಥಾ ಯದಿ ( roundingMode == round_near_even ) {
        uiZ += lastBitMask>>1;
        ಯದಿ ( ! (uiZ & roundBitsMask) ) uiZ &= ~lastBitMask;
    } ಅನ್ಯಥಾ ಯದಿ (
        roundingMode
            == (signF32UI( uiZ ) ? round_min : round_max)
    ) {
        uiZ += roundBitsMask;
    }
    uiZ &= ~roundBitsMask;
    ಯದಿ ( exact && (uiZ != uiA) ) {
        raiseFlags(flag_inexact);
    }
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float32_t f32_sqrt( float32_t a )
{
    uint_fast32_t uiA;
    ತಾರ್ಕಿಕ signA;
    int_fast16_t expA;
    uint_fast32_t sigA, uiZ;
    ಸಂರಚನಾ exp16_sig32 normExpSig;
    int_fast16_t expZ;
    uint_fast32_t sigZ, shiftedSigZ;
    uint32_t negRem;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    signA = signF32UI( uiA );
    expA  = expF32UI( uiA );
    sigA  = fracF32UI( uiA );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( expA == 0xFF ) {
        ಯದಿ ( sigA ) {
            uiZ = softfloat_propagateNaNF32UI( uiA, 0 );
            ಗಚ್ಛ uiZ;
        }
        ಯದಿ ( ! signA ) ಪ್ರತ್ಯಾಗಮ a;
        ಗಚ್ಛ invalid;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( signA ) {
        ಯದಿ ( ! (expA | sigA) ) ಪ್ರತ್ಯಾಗಮ a;
        ಗಚ್ಛ invalid;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( ! expA ) {
        ಯದಿ ( ! sigA ) ಪ್ರತ್ಯಾಗಮ a;
        normExpSig = softfloat_normSubnormalF32Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expZ = ((expA - 0x7F)>>1) + 0x7E;
    expA &= 1;
    sigA = (sigA | 0x00800000)<<8;
    sigZ =
        ((uint_fast64_t) sigA * softfloat_approxRecipSqrt32_1( expA, sigA ))
            >>32;
    ಯದಿ ( expA ) sigZ >>= 1;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    sigZ += 2;
    ಯದಿ ( (sigZ & 0x3F) < 2 ) {
        shiftedSigZ = sigZ>>2;
        negRem = shiftedSigZ * shiftedSigZ;
        sigZ &= ~3;
        ಯದಿ ( negRem & 0x80000000 ) {
            sigZ |= 1;
        } ಅನ್ಯಥಾ {
            ಯದಿ ( negRem ) --sigZ;
        }
    }
    ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF32( 0, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 invalid:
    raiseFlags( flag_invalid );
    uiZ = defaultNaNF32UI;
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float32_t f32_sub( float32_t a, float32_t b )
{
    uint_fast32_t uiA;
    uint_fast32_t uiB;

    uiA = a.v;
    uiB = b.v;
    ಯದಿ ( signF32UI( uiA ^ uiB ) ) {
        ಪ್ರತ್ಯಾಗಮ softfloat_addMagsF32( uiA, uiB );
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ softfloat_subMagsF32( uiA, uiB );
    }
}

ಸ್ಥಿರ float64_t f32_to_f64( float32_t a )
{
    uint_fast32_t uiA;
    ತಾರ್ಕಿಕ sign;
    int_fast16_t exp;
    uint_fast32_t frac;
    ಸಂರಚನಾ commonNaN commonNaN;
    uint_fast64_t uiZ;
    ಸಂರಚನಾ exp16_sig32 normExpSig;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    sign = signF32UI( uiA );
    exp  = expF32UI( uiA );
    frac = fracF32UI( uiA );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( exp == 0xFF ) {
        ಯದಿ ( frac ) {
            softfloat_f32UIToCommonNaN( uiA, &commonNaN );
            uiZ = softfloat_commonNaNToF64UI( &commonNaN );
        } ಅನ್ಯಥಾ {
            uiZ = packToF64UI( sign, 0x7FF, 0 );
        }
        ಗಚ್ಛ uiZ;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( ! exp ) {
        ಯದಿ ( ! frac ) {
            uiZ = packToF64UI( sign, 0, 0 );
            ಗಚ್ಛ uiZ;
        }
        normExpSig = softfloat_normSubnormalF32Sig( frac );
        exp = normExpSig.exp - 1;
        frac = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiZ = packToF64UI( sign, exp + 0x380, (uint_fast64_t) frac<<29 );
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(uiZ);
}

ಸ್ಥಿರ int_fast32_t f32_to_i32( float32_t a, uint_fast8_t roundingMode, ತಾರ್ಕಿಕ exact )
{
    uint_fast32_t uiA;
    ತಾರ್ಕಿಕ sign;
    int_fast16_t exp;
    uint_fast32_t sig;
    uint_fast64_t sig64;
    int_fast16_t shiftDist;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    sign = signF32UI( uiA );
    exp  = expF32UI( uiA );
    sig  = fracF32UI( uiA );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
#ಯದಿ (i32_fromNaN != i32_fromPosOverflow) || (i32_fromNaN != i32_fromNegOverflow)
    ಯದಿ ( (exp == 0xFF) && sig ) {
#ಯದಿ (i32_fromNaN == i32_fromPosOverflow)
        sign = 0;
#ನೋಚೇದ್ಯದಿ (i32_fromNaN == i32_fromNegOverflow)
        sign = 1;
#ಅನ್ಯಥಾ
        raiseFlags( flag_invalid );
        ಪ್ರತ್ಯಾಗಮ i32_fromNaN;
#endif
    }
#endif
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( exp ) sig |= 0x00800000;
    sig64 = (uint_fast64_t) sig<<32;
    shiftDist = 0xAA - exp;
    ಯದಿ ( 0 < shiftDist ) sig64 = softfloat_shift_ಬಲ_Jam64( sig64, shiftDist );
    ಪ್ರತ್ಯಾಗಮ softfloat_roundToI32( sign, sig64, roundingMode, exact );
}

ಸ್ಥಿರ int_fast32_t f32_to_i32_r_minMag( float32_t a, ತಾರ್ಕಿಕ exact )
{
    uint_fast32_t uiA;
    int_fast16_t exp;
    uint_fast32_t sig;
    int_fast16_t shiftDist;
    ತಾರ್ಕಿಕ sign;
    int_fast32_t absZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    exp = expF32UI( uiA );
    sig = fracF32UI( uiA );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    shiftDist = 0x9E - exp;
    ಯದಿ ( 32 <= shiftDist ) {
        ಯದಿ ( exact && (exp | sig) ) {
            raiseFlags(flag_inexact);
        }
        ಪ್ರತ್ಯಾಗಮ 0;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    sign = signF32UI( uiA );
    ಯದಿ ( shiftDist <= 0 ) {
        ಯದಿ ( uiA == packToF32UI( 1, 0x9E, 0 ) ) ಪ್ರತ್ಯಾಗಮ -0x7FFFFFFF - 1;
        raiseFlags( flag_invalid );
        ಪ್ರತ್ಯಾಗಮ
            (exp == 0xFF) && sig ? i32_fromNaN
                : sign ? i32_fromNegOverflow : i32_fromPosOverflow;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    sig = (sig | 0x00800000)<<8;
    absZ = sig>>shiftDist;
    ಯದಿ ( exact && ((uint_fast32_t) absZ<<shiftDist != sig) ) {
        raiseFlags(flag_inexact);
    }
    ಪ್ರತ್ಯಾಗಮ sign ? -absZ : absZ;
}

ಸ್ಥಿರ float64_t f64_add( float64_t a, float64_t b )
{
    uint_fast64_t uiA;
    ತಾರ್ಕಿಕ signA;
    uint_fast64_t uiB;
    ತಾರ್ಕಿಕ signB;

    uiA = a.v;
    signA = signF64UI( uiA );
    uiB = b.v;
    signB = signF64UI( uiB );
    ಯದಿ ( signA == signB ) {
        ಪ್ರತ್ಯಾಗಮ softfloat_addMagsF64( uiA, uiB, signA );
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ softfloat_subMagsF64( uiA, uiB, signA );
    }
}

ಸ್ಥಿರ float64_t f64_div( float64_t a, float64_t b )
{
    uint_fast64_t uiA;
    ತಾರ್ಕಿಕ signA;
    int_fast16_t expA;
    uint_fast64_t sigA;
    uint_fast64_t uiB;
    ತಾರ್ಕಿಕ signB;
    int_fast16_t expB;
    uint_fast64_t sigB;
    ತಾರ್ಕಿಕ signZ;
    ಸಂರಚನಾ exp16_sig64 normExpSig;
    int_fast16_t expZ;
    uint32_t recip32, sig32Z, doubleTerm;
    uint_fast64_t rem;
    uint32_t q;
    uint_fast64_t sigZ;
    uint_fast64_t uiZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    signA = signF64UI( uiA );
    expA  = expF64UI( uiA );
    sigA  = fracF64UI( uiA );
    uiB = b.v;
    signB = signF64UI( uiB );
    expB  = expF64UI( uiB );
    sigB  = fracF64UI( uiB );
    signZ = signA ^ signB;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( expA == 0x7FF ) {
        ಯದಿ ( sigA ) ಗಚ್ಛ propagateNaN;
        ಯದಿ ( expB == 0x7FF ) {
            ಯದಿ ( sigB ) ಗಚ್ಛ propagateNaN;
            ಗಚ್ಛ invalid;
        }
        ಗಚ್ಛ infinity;
    }
    ಯದಿ ( expB == 0x7FF ) {
        ಯದಿ ( sigB ) ಗಚ್ಛ propagateNaN;
        ಗಚ್ಛ zero;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( ! expB ) {
        ಯದಿ ( ! sigB ) {
            ಯದಿ ( ! (expA | sigA) ) ಗಚ್ಛ invalid;
            raiseFlags( flag_infinite );
            ಗಚ್ಛ infinity;
        }
        normExpSig = softfloat_normSubnormalF64Sig( sigB );
        expB = normExpSig.exp;
        sigB = normExpSig.sig;
    }
    ಯದಿ ( ! expA ) {
        ಯದಿ ( ! sigA ) ಗಚ್ಛ zero;
        normExpSig = softfloat_normSubnormalF64Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expZ = expA - expB + 0x3FE;
    sigA |= UINT64_C( 0x0010000000000000 );
    sigB |= UINT64_C( 0x0010000000000000 );
    ಯದಿ ( sigA < sigB ) {
        --expZ;
        sigA <<= 11;
    } ಅನ್ಯಥಾ {
        sigA <<= 10;
    }
    sigB <<= 11;
    recip32 = softfloat_approxRecip32_1( sigB>>32 ) - 2;
    sig32Z = ((uint32_t) (sigA>>32) * (uint_fast64_t) recip32)>>32;
    doubleTerm = sig32Z<<1;
    rem =
        ((sigA - (uint_fast64_t) doubleTerm * (uint32_t) (sigB>>32))<<28)
            - (uint_fast64_t) doubleTerm * ((uint32_t) sigB>>4);
    q = (((uint32_t) (rem>>32) * (uint_fast64_t) recip32)>>32) + 4;
    sigZ = ((uint_fast64_t) sig32Z<<32) + ((uint_fast64_t) q<<4);
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( (sigZ & 0x1FF) < 4<<4 ) {
        q &= ~7;
        sigZ &= ~(uint_fast64_t) 0x7F;
        doubleTerm = q<<1;
        rem =
            ((rem - (uint_fast64_t) doubleTerm * (uint32_t) (sigB>>32))<<28)
                - (uint_fast64_t) doubleTerm * ((uint32_t) sigB>>4);
        ಯದಿ ( rem & UINT64_C( 0x8000000000000000 ) ) {
            sigZ -= 1<<7;
        } ಅನ್ಯಥಾ {
            ಯದಿ ( rem ) sigZ |= 1;
        }
    }
    ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF64( signZ, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = softfloat_propagateNaNF64UI( uiA, uiB );
    ಗಚ್ಛ uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 invalid:
    raiseFlags( flag_invalid );
    uiZ = defaultNaNF64UI;
    ಗಚ್ಛ uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 infinity:
    uiZ = packToF64UI( signZ, 0x7FF, 0 );
    ಗಚ್ಛ uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 zero:
    uiZ = packToF64UI( signZ, 0, 0 );
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(uiZ);
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ f64_eq( float64_t a, float64_t b )
{
    uint_fast64_t uiA;
    uint_fast64_t uiB;

    uiA = a.v;
    uiB = b.v;
    ಯದಿ ( isNaNF64UI( uiA ) || isNaNF64UI( uiB ) )
    {
        ಯದಿ ( softfloat_isSigNaNF64UI( uiA ) || softfloat_isSigNaNF64UI( uiB ) )
            raiseFlags( flag_invalid );
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ (uiA == uiB) || ! ((uiA | uiB) & UINT64_C( 0x7FFFFFFFFFFFFFFF ));
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ f64_le( float64_t a, float64_t b )
{
    uint_fast64_t uiA;
    uint_fast64_t uiB;
    ತಾರ್ಕಿಕ signA, signB;

    uiA = a.v;
    uiB = b.v;
    ಯದಿ ( isNaNF64UI( uiA ) || isNaNF64UI( uiB ) ) {
        raiseFlags( flag_invalid );
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    signA = signF64UI( uiA );
    signB = signF64UI( uiB );
    ಪ್ರತ್ಯಾಗಮ (signA != signB) ? signA || ! ((uiA | uiB) & UINT64_C( 0x7FFFFFFFFFFFFFFF ))
                            : (uiA == uiB) || (signA ^ (uiA < uiB));
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ f64_lt( float64_t a, float64_t b )
{
    uint_fast64_t uiA;
    uint_fast64_t uiB;
    ತಾರ್ಕಿಕ signA, signB;

    uiA = a.v;
    uiB = b.v;
    ಯದಿ ( isNaNF64UI( uiA ) || isNaNF64UI( uiB ) ) {
        raiseFlags( flag_invalid );
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    signA = signF64UI( uiA );
    signB = signF64UI( uiB );
    ಪ್ರತ್ಯಾಗಮ (signA != signB) ? signA && ((uiA | uiB) & UINT64_C( 0x7FFFFFFFFFFFFFFF ))
                            : (uiA != uiB) && (signA ^ (uiA < uiB));
}

ಸ್ಥಿರ float64_t f64_mulAdd( float64_t a, float64_t b, float64_t c )
{
    uint_fast64_t uiA;
    uint_fast64_t uiB;
    uint_fast64_t uiC;

    uiA = a.v;
    uiB = b.v;
    uiC = c.v;
    ಪ್ರತ್ಯಾಗಮ softfloat_mulAddF64( uiA, uiB, uiC, 0 );
}

ಸ್ಥಿರ float64_t f64_mul( float64_t a, float64_t b )
{
    uint_fast64_t uiA;
    ತಾರ್ಕಿಕ signA;
    int_fast16_t expA;
    uint_fast64_t sigA;
    uint_fast64_t uiB;
    ತಾರ್ಕಿಕ signB;
    int_fast16_t expB;
    uint_fast64_t sigB;
    ತಾರ್ಕಿಕ signZ;
    uint_fast64_t magBits;
    ಸಂರಚನಾ exp16_sig64 normExpSig;
    int_fast16_t expZ;
    ಸಂರಚನಾ uint128 sig128Z;
    uint_fast64_t sigZ, uiZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    signA = signF64UI( uiA );
    expA  = expF64UI( uiA );
    sigA  = fracF64UI( uiA );
    uiB = b.v;
    signB = signF64UI( uiB );
    expB  = expF64UI( uiB );
    sigB  = fracF64UI( uiB );
    signZ = signA ^ signB;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( expA == 0x7FF ) {
        ಯದಿ ( sigA || ((expB == 0x7FF) && sigB) ) ಗಚ್ಛ propagateNaN;
        magBits = expB | sigB;
        ಗಚ್ಛ infArg;
    }
    ಯದಿ ( expB == 0x7FF ) {
        ಯದಿ ( sigB ) ಗಚ್ಛ propagateNaN;
        magBits = expA | sigA;
        ಗಚ್ಛ infArg;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( ! expA ) {
        ಯದಿ ( ! sigA ) ಗಚ್ಛ zero;
        normExpSig = softfloat_normSubnormalF64Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }
    ಯದಿ ( ! expB ) {
        ಯದಿ ( ! sigB ) ಗಚ್ಛ zero;
        normExpSig = softfloat_normSubnormalF64Sig( sigB );
        expB = normExpSig.exp;
        sigB = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expZ = expA + expB - 0x3FF;
    sigA = (sigA | UINT64_C( 0x0010000000000000 ))<<10;
    sigB = (sigB | UINT64_C( 0x0010000000000000 ))<<11;
    sig128Z = softfloat_mul64To128( sigA, sigB );
    sigZ = sig128Z.v64 | (sig128Z.v0 != 0);

    ಯದಿ ( sigZ < UINT64_C( 0x4000000000000000 ) ) {
        --expZ;
        sigZ <<= 1;
    }
    ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF64( signZ, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = softfloat_propagateNaNF64UI( uiA, uiB );
    ಗಚ್ಛ uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 infArg:
    ಯದಿ ( ! magBits ) {
        raiseFlags( flag_invalid );
        uiZ = defaultNaNF64UI;
    } ಅನ್ಯಥಾ {
        uiZ = packToF64UI( signZ, 0x7FF, 0 );
    }
    ಗಚ್ಛ uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 zero:
    uiZ = packToF64UI( signZ, 0, 0 );
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float64_t f64_rem( float64_t a, float64_t b )
{
    uint_fast64_t uiA;
    ತಾರ್ಕಿಕ signA;
    int_fast16_t expA;
    uint_fast64_t sigA;
    uint_fast64_t uiB;
    int_fast16_t expB;
    uint_fast64_t sigB;
    ಸಂರಚನಾ exp16_sig64 normExpSig;
    uint64_t rem;
    int_fast16_t expDiff;
    uint32_t q, recip32;
    uint_fast64_t q64;
    uint64_t altRem, meanRem;
    ತಾರ್ಕಿಕ signRem;
    uint_fast64_t uiZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    signA = signF64UI( uiA );
    expA  = expF64UI( uiA );
    sigA  = fracF64UI( uiA );
    uiB = b.v;
    expB = expF64UI( uiB );
    sigB = fracF64UI( uiB );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( expA == 0x7FF ) {
        ಯದಿ ( sigA || ((expB == 0x7FF) && sigB) ) ಗಚ್ಛ propagateNaN;
        ಗಚ್ಛ invalid;
    }
    ಯದಿ ( expB == 0x7FF ) {
        ಯದಿ ( sigB ) ಗಚ್ಛ propagateNaN;
        ಪ್ರತ್ಯಾಗಮ a;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( expA < expB - 1 ) ಪ್ರತ್ಯಾಗಮ a;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( ! expB ) {
        ಯದಿ ( ! sigB ) ಗಚ್ಛ invalid;
        normExpSig = softfloat_normSubnormalF64Sig( sigB );
        expB = normExpSig.exp;
        sigB = normExpSig.sig;
    }
    ಯದಿ ( ! expA ) {
        ಯದಿ ( ! sigA ) ಪ್ರತ್ಯಾಗಮ a;
        normExpSig = softfloat_normSubnormalF64Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    rem = sigA | UINT64_C( 0x0010000000000000 );
    sigB |= UINT64_C( 0x0010000000000000 );
    expDiff = expA - expB;
    ಯದಿ ( expDiff < 1 ) {
        ಯದಿ ( expDiff < -1 ) ಪ್ರತ್ಯಾಗಮ a;
        sigB <<= 9;
        ಯದಿ ( expDiff ) {
            rem <<= 8;
            q = 0;
        } ಅನ್ಯಥಾ {
            rem <<= 9;
            q = (sigB <= rem);
            ಯದಿ ( q ) rem -= sigB;
        }
    } ಅನ್ಯಥಾ {
        recip32 = softfloat_approxRecip32_1( sigB>>21 );
        /*--------------------------------------------------------------------
        | Changing the shift of `rem' here requires also changing the initial
        | subtraction from `expDiff'.
        *--------------------------------------------------------------------*/
        rem <<= 9;
        expDiff -= 30;
        /*--------------------------------------------------------------------
        | The scale of `sigB' affects how many bits are obtained during each
        | cycle of the loop.  Currently this is 29 bits per loop iteration,
        | the maximum possible.
        *--------------------------------------------------------------------*/
        sigB <<= 9;
        ಪ್ರತಿ (;;) {
            q64 = (uint32_t) (rem>>32) * (uint_fast64_t) recip32;
            ಯದಿ ( expDiff < 0 ) ಭಙ್ಗ;
            q = (q64 + 0x80000000)>>32;
            rem <<= 29;
            rem -= q * (uint64_t) sigB;
            ಯದಿ ( rem & UINT64_C( 0x8000000000000000 ) ) rem += sigB;
            expDiff -= 29;
        }
        /*--------------------------------------------------------------------
        | (`expDiff' cannot be less than -29 here.)
        *--------------------------------------------------------------------*/
        q = (uint32_t) (q64>>32)>>(~expDiff & 31);
        rem = (rem<<(expDiff + 30)) - q * (uint64_t) sigB;
        ಯದಿ ( rem & UINT64_C( 0x8000000000000000 ) ) {
            altRem = rem + sigB;
            ಗಚ್ಛ selectRem;
        }
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಕುರು {
        altRem = rem;
        ++q;
        rem -= sigB;
    } ಯಾವತ್ ( ! (rem & UINT64_C( 0x8000000000000000 )) );
 selectRem:
    meanRem = rem + altRem;
    ಯದಿ (
        (meanRem & UINT64_C( 0x8000000000000000 )) || (! meanRem && (q & 1))
    ) {
        rem = altRem;
    }
    signRem = signA;
    ಯದಿ ( rem & UINT64_C( 0x8000000000000000 ) ) {
        signRem = ! signRem;
        //fixed unsigned unary minus: -x == ~x + 1
        rem = ~rem + 1;
    }
    ಪ್ರತ್ಯಾಗಮ softfloat_normRoundPackToF64( signRem, expB, rem );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = softfloat_propagateNaNF64UI( uiA, uiB );
    ಗಚ್ಛ uiZ;
 invalid:
    raiseFlags( flag_invalid );
    uiZ = defaultNaNF64UI;
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float64_t f64_roundToInt( float64_t a, uint_fast8_t roundingMode, ತಾರ್ಕಿಕ exact )
{
    uint_fast64_t uiA;
    int_fast16_t exp;
    uint_fast64_t uiZ, lastBitMask, roundBitsMask;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    exp = expF64UI( uiA );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( exp <= 0x3FE ) {
        ಯದಿ ( ! (uiA & UINT64_C( 0x7FFFFFFFFFFFFFFF )) ) ಪ್ರತ್ಯಾಗಮ a;
        ಯದಿ ( exact ) raiseFlags(flag_inexact);
        uiZ = uiA & packToF64UI( 1, 0, 0 );
        ವಿಕಲ್ಪ ( roundingMode ) {
         ಪಕ್ಷ round_near_even:
            ಯದಿ ( ! fracF64UI( uiA ) ) ಭಙ್ಗ;
            [[fallthrough]];
         ಪಕ್ಷ round_near_maxMag:
            ಯದಿ ( exp == 0x3FE ) uiZ |= packToF64UI( 0, 0x3FF, 0 );
            ಭಙ್ಗ;
         ಪಕ್ಷ round_min:
            ಯದಿ ( uiZ ) uiZ = packToF64UI( 1, 0x3FF, 0 );
            ಭಙ್ಗ;
         ಪಕ್ಷ round_max:
            ಯದಿ ( ! uiZ ) uiZ = packToF64UI( 0, 0x3FF, 0 );
            ಭಙ್ಗ;
        }
        ಗಚ್ಛ uiZ;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( 0x433 <= exp ) {
        ಯದಿ ( (exp == 0x7FF) && fracF64UI( uiA ) ) {
            uiZ = softfloat_propagateNaNF64UI( uiA, 0 );
            ಗಚ್ಛ uiZ;
        }
        ಪ್ರತ್ಯಾಗಮ a;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiZ = uiA;
    lastBitMask = (uint_fast64_t) 1<<(0x433 - exp);
    roundBitsMask = lastBitMask - 1;
    ಯದಿ ( roundingMode == round_near_maxMag ) {
        uiZ += lastBitMask>>1;
    } ಅನ್ಯಥಾ ಯದಿ ( roundingMode == round_near_even ) {
        uiZ += lastBitMask>>1;
        ಯದಿ ( ! (uiZ & roundBitsMask) ) uiZ &= ~lastBitMask;
    } ಅನ್ಯಥಾ ಯದಿ (
        roundingMode
            == (signF64UI( uiZ ) ? round_min : round_max)
    ) {
        uiZ += roundBitsMask;
    }
    uiZ &= ~roundBitsMask;
    ಯದಿ ( exact && (uiZ != uiA) ) {
        raiseFlags(flag_inexact);
    }
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float64_t f64_sqrt( float64_t a )
{
    uint_fast64_t uiA;
    ತಾರ್ಕಿಕ signA;
    int_fast16_t expA;
    uint_fast64_t sigA, uiZ;
    ಸಂರಚನಾ exp16_sig64 normExpSig;
    int_fast16_t expZ;
    uint32_t sig32A, recipSqrt32, sig32Z;
    uint_fast64_t rem;
    uint32_t q;
    uint_fast64_t sigZ, shiftedSigZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    signA = signF64UI( uiA );
    expA  = expF64UI( uiA );
    sigA  = fracF64UI( uiA );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( expA == 0x7FF ) {
        ಯದಿ ( sigA ) {
            uiZ = softfloat_propagateNaNF64UI( uiA, 0 );
            ಗಚ್ಛ uiZ;
        }
        ಯದಿ ( ! signA ) ಪ್ರತ್ಯಾಗಮ a;
        ಗಚ್ಛ invalid;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( signA ) {
        ಯದಿ ( ! (expA | sigA) ) ಪ್ರತ್ಯಾಗಮ a;
        ಗಚ್ಛ invalid;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( ! expA ) {
        ಯದಿ ( ! sigA ) ಪ್ರತ್ಯಾಗಮ a;
        normExpSig = softfloat_normSubnormalF64Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    | (`sig32Z' is guaranteed to be a lower bound on the square root of
    | `sig32A', which makes `sig32Z' also a lower bound on the square root of
    | `sigA'.)
    *------------------------------------------------------------------------*/
    expZ = ((expA - 0x3FF)>>1) + 0x3FE;
    expA &= 1;
    sigA |= UINT64_C( 0x0010000000000000 );
    sig32A = (uint32_t)(sigA>>21); //fixed warning on type cast
    recipSqrt32 = softfloat_approxRecipSqrt32_1( expA, sig32A );
    sig32Z = ((uint_fast64_t) sig32A * recipSqrt32)>>32;
    ಯದಿ ( expA ) {
        sigA <<= 8;
        sig32Z >>= 1;
    } ಅನ್ಯಥಾ {
        sigA <<= 9;
    }
    rem = sigA - (uint_fast64_t) sig32Z * sig32Z;
    q = ((uint32_t) (rem>>2) * (uint_fast64_t) recipSqrt32)>>32;
    sigZ = ((uint_fast64_t) sig32Z<<32 | 1<<5) + ((uint_fast64_t) q<<3);
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( (sigZ & 0x1FF) < 0x22 ) {
        sigZ &= ~(uint_fast64_t) 0x3F;
        shiftedSigZ = sigZ>>6;
        rem = (sigA<<52) - shiftedSigZ * shiftedSigZ;
        ಯದಿ ( rem & UINT64_C( 0x8000000000000000 ) ) {
            --sigZ;
        } ಅನ್ಯಥಾ {
            ಯದಿ ( rem ) sigZ |= 1;
        }
    }
    ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF64( 0, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 invalid:
    raiseFlags( flag_invalid );
    uiZ = defaultNaNF64UI;
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float64_t f64_sub( float64_t a, float64_t b )
{
    uint_fast64_t uiA;
    ತಾರ್ಕಿಕ signA;
    uint_fast64_t uiB;
    ತಾರ್ಕಿಕ signB;

    uiA = a.v;
    signA = signF64UI( uiA );
    uiB = b.v;
    signB = signF64UI( uiB );

    ಯದಿ ( signA == signB ) {
        ಪ್ರತ್ಯಾಗಮ softfloat_subMagsF64( uiA, uiB, signA );
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ softfloat_addMagsF64( uiA, uiB, signA );
    }
}

ಸ್ಥಿರ float32_t f64_to_f32( float64_t a )
{
    uint_fast64_t uiA;
    ತಾರ್ಕಿಕ sign;
    int_fast16_t exp;
    uint_fast64_t frac;
    ಸಂರಚನಾ commonNaN commonNaN;
    uint_fast32_t uiZ, frac32;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    sign = signF64UI( uiA );
    exp  = expF64UI( uiA );
    frac = fracF64UI( uiA );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( exp == 0x7FF ) {
        ಯದಿ ( frac ) {
            softfloat_f64UIToCommonNaN( uiA, &commonNaN );
            uiZ = softfloat_commonNaNToF32UI( &commonNaN );
        } ಅನ್ಯಥಾ {
            uiZ = packToF32UI( sign, 0xFF, 0 );
        }
        ಗಚ್ಛ uiZ;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    frac32 = (uint_fast32_t)softfloat_short_Shift_ಬಲ_Jam64( frac, 22 ); //fixed warning on type cast
    ಯದಿ ( ! (exp | frac32) ) {
        uiZ = packToF32UI( sign, 0, 0 );
        ಗಚ್ಛ uiZ;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF32( sign, exp - 0x381, frac32 | 0x40000000 );
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(uiZ);
}

ಸ್ಥಿರ int_fast32_t f64_to_i32( float64_t a, uint_fast8_t roundingMode, ತಾರ್ಕಿಕ exact )
{
    uint_fast64_t uiA;
    ತಾರ್ಕಿಕ sign;
    int_fast16_t exp;
    uint_fast64_t sig;
    int_fast16_t shiftDist;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    sign = signF64UI( uiA );
    exp  = expF64UI( uiA );
    sig  = fracF64UI( uiA );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
#ಯದಿ (i32_fromNaN != i32_fromPosOverflow) || (i32_fromNaN != i32_fromNegOverflow)
    ಯದಿ ( (exp == 0x7FF) && sig ) {
#ಯದಿ (i32_fromNaN == i32_fromPosOverflow)
        sign = 0;
#ನೋಚೇದ್ಯದಿ (i32_fromNaN == i32_fromNegOverflow)
        sign = 1;
#ಅನ್ಯಥಾ
        raiseFlags( flag_invalid );
        ಪ್ರತ್ಯಾಗಮ i32_fromNaN;
#endif
    }
#endif
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( exp ) sig |= UINT64_C( 0x0010000000000000 );
    shiftDist = 0x427 - exp;
    ಯದಿ ( 0 < shiftDist ) sig = softfloat_shift_ಬಲ_Jam64( sig, shiftDist );
    ಪ್ರತ್ಯಾಗಮ softfloat_roundToI32( sign, sig, roundingMode, exact );
}

ಸ್ಥಿರ int_fast64_t f64_to_i64(float64_t a, uint_fast8_t roundingMode, ತಾರ್ಕಿಕ exact )
{
    uint_fast64_t uiA;
    ತಾರ್ಕಿಕ sign;
    int_fast16_t exp;
    uint_fast64_t sig;
    int_fast16_t shiftDist;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    sign = signF64UI(uiA);
    exp = expF64UI(uiA);
    sig = fracF64UI(uiA);
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
#ಯದಿ (i64_fromNaN != i64_fromPosOverflow) || (i64_fromNaN != i64_fromNegOverflow)
    ಯದಿ ((exp == 0x7FF) && sig) {
#ಯದಿ (i64_fromNaN == i64_fromPosOverflow)
        sign = 0;
#ನೋಚೇದ್ಯದಿ (i64_fromNaN == i64_fromNegOverflow)
        sign = 1;
#ಅನ್ಯಥಾ
        raiseFlags(flag_invalid);
        ಪ್ರತ್ಯಾಗಮ i64_fromNaN;
#endif
    }
#endif
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ (exp) sig |= UINT64_C(0x0010000000000000);
    shiftDist = 0x433 - exp;
    ಯದಿ (shiftDist <= 0) {
        ತಾರ್ಕಿಕ isValid = shiftDist >= -11;
        ಯದಿ (isValid)
        {
            uint_fast64_t z = sig << -shiftDist;
            ಯದಿ (0 == (z & UINT64_C(0x8000000000000000)))
            {
                ಪ್ರತ್ಯಾಗಮ sign ? -(int_fast64_t)z : (int_fast64_t)z;
            }
        }
        raiseFlags(flag_invalid);
        ಪ್ರತ್ಯಾಗಮ sign ? i64_fromNegOverflow : i64_fromPosOverflow;
    }
    ಅನ್ಯಥಾ {
        ಯದಿ (shiftDist < 64)
            ಪ್ರತ್ಯಾಗಮ
                softfloat_roundToI64(
                    sign, sig >> shiftDist, sig << (-shiftDist & 63), roundingMode, exact);
        ಅನ್ಯಥಾ
            ಪ್ರತ್ಯಾಗಮ
                softfloat_roundToI64(
                    sign, 0, (shiftDist == 64) ? sig : (sig != 0), roundingMode, exact);
    }
}

ಸ್ಥಿರ int_fast32_t f64_to_i32_r_minMag( float64_t a, ತಾರ್ಕಿಕ exact )
{
    uint_fast64_t uiA;
    int_fast16_t exp;
    uint_fast64_t sig;
    int_fast16_t shiftDist;
    ತಾರ್ಕಿಕ sign;
    int_fast32_t absZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uiA = a.v;
    exp = expF64UI( uiA );
    sig = fracF64UI( uiA );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    shiftDist = 0x433 - exp;
    ಯದಿ ( 53 <= shiftDist ) {
        ಯದಿ ( exact && (exp | sig) ) {
            raiseFlags(flag_inexact);
        }
        ಪ್ರತ್ಯಾಗಮ 0;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    sign = signF64UI( uiA );
    ಯದಿ ( shiftDist < 22 ) {
        ಯದಿ (
            sign && (exp == 0x41E) && (sig < UINT64_C( 0x0000000000200000 ))
        ) {
            ಯದಿ ( exact && sig ) {
                raiseFlags(flag_inexact);
            }
            ಪ್ರತ್ಯಾಗಮ -0x7FFFFFFF - 1;
        }
        raiseFlags( flag_invalid );
        ಪ್ರತ್ಯಾಗಮ
            (exp == 0x7FF) && sig ? i32_fromNaN
                : sign ? i32_fromNegOverflow : i32_fromPosOverflow;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    sig |= UINT64_C( 0x0010000000000000 );
    absZ = (int_fast32_t)(sig>>shiftDist); //fixed warning on type cast
    ಯದಿ ( exact && ((uint_fast64_t) (uint_fast32_t) absZ<<shiftDist != sig) ) {
        raiseFlags(flag_inexact);
    }
    ಪ್ರತ್ಯಾಗಮ sign ? -absZ : absZ;
}

ಸ್ಥಿರ float32_t i32_to_f32( int32_t a )
{
    ತಾರ್ಕಿಕ sign;
    uint_fast32_t absA;

    sign = (a < 0);
    ಯದಿ ( ! (a & 0x7FFFFFFF) ) {
        ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(sign ? packToF32UI( 1, 0x9E, 0 ) : 0);
    }
    //fixed unsigned unary minus: -x == ~x + 1
    absA = sign ? (~(uint_fast32_t) a + 1) : (uint_fast32_t) a;
    ಪ್ರತ್ಯಾಗಮ softfloat_normRoundPackToF32( sign, 0x9C, absA );
}

ಸ್ಥಿರ float64_t i32_to_f64( int32_t a )
{
    uint_fast64_t uiZ;
    ತಾರ್ಕಿಕ sign;
    uint_fast32_t absA;
    int_fast8_t shiftDist;

    ಯದಿ ( ! a ) {
        uiZ = 0;
    } ಅನ್ಯಥಾ {
        sign = (a < 0);
        //fixed unsigned unary minus: -x == ~x + 1
        absA = sign ? (~(uint_fast32_t) a + 1) : (uint_fast32_t) a;
        shiftDist = softfloat_ಎಣಿಕೆ_Leading_Zeros32( absA ) + 21;
        uiZ =
            packToF64UI(
                sign, 0x432 - shiftDist, (uint_fast64_t) absA<<shiftDist );
    }
    ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float32_t i64_to_f32( int64_t a )
{
    ತಾರ್ಕಿಕ sign;
    uint_fast64_t absA;
    int_fast8_t shiftDist;
    uint_fast32_t sig;

    sign = (a < 0);
    //fixed unsigned unary minus: -x == ~x + 1
    absA = sign ? (~(uint_fast64_t) a + 1) : (uint_fast64_t) a;
    shiftDist = softfloat_ಎಣಿಕೆ_Leading_Zeros64( absA ) - 40;
    ಯದಿ ( 0 <= shiftDist ) {
        ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(a ? packToF32UI(sign, 0x95 - shiftDist, (uint_fast32_t) absA<<shiftDist ) : 0);
    } ಅನ್ಯಥಾ {
        shiftDist += 7;
        sig =
            (shiftDist < 0)
                ? (uint_fast32_t) softfloat_short_Shift_ಬಲ_Jam64( absA, -shiftDist ) //fixed warning on type cast
                : (uint_fast32_t) absA<<shiftDist;
        ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF32( sign, 0x9C - shiftDist, sig );
    }
}

ಸ್ಥಿರ float64_t i64_to_f64( int64_t a )
{
    ತಾರ್ಕಿಕ sign;
    uint_fast64_t absA;

    sign = (a < 0);
    ಯದಿ ( ! (a & UINT64_C( 0x7FFFFFFFFFFFFFFF )) ) {
        ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(sign ? packToF64UI( 1, 0x43E, 0 ) : 0);
    }
    //fixed unsigned unary minus: -x == ~x + 1
    absA = sign ? (~(uint_fast64_t) a + 1) : (uint_fast64_t) a;
    ಪ್ರತ್ಯಾಗಮ softfloat_normRoundPackToF64( sign, 0x43C, absA );
}

ಸ್ಥಿರ float32_t softfloat_addMagsF32( uint_fast32_t uiA, uint_fast32_t uiB )
{
    int_fast16_t expA;
    uint_fast32_t sigA;
    int_fast16_t expB;
    uint_fast32_t sigB;
    int_fast16_t expDiff;
    uint_fast32_t uiZ;
    ತಾರ್ಕಿಕ signZ;
    int_fast16_t expZ;
    uint_fast32_t sigZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expA = expF32UI( uiA );
    sigA = fracF32UI( uiA );
    expB = expF32UI( uiB );
    sigB = fracF32UI( uiB );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expDiff = expA - expB;
    ಯದಿ ( ! expDiff ) {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        ಯದಿ ( ! expA ) {
            uiZ = uiA + sigB;
            ಗಚ್ಛ uiZ;
        }
        ಯದಿ ( expA == 0xFF ) {
            ಯದಿ ( sigA | sigB ) ಗಚ್ಛ propagateNaN;
            uiZ = uiA;
            ಗಚ್ಛ uiZ;
        }
        signZ = signF32UI( uiA );
        expZ = expA;
        sigZ = 0x01000000 + sigA + sigB;
        ಯದಿ ( ! (sigZ & 1) && (expZ < 0xFE) ) {
            uiZ = packToF32UI( signZ, expZ, sigZ>>1 );
            ಗಚ್ಛ uiZ;
        }
        sigZ <<= 6;
    } ಅನ್ಯಥಾ {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        signZ = signF32UI( uiA );
        sigA <<= 6;
        sigB <<= 6;
        ಯದಿ ( expDiff < 0 ) {
            ಯದಿ ( expB == 0xFF ) {
                ಯದಿ ( sigB ) ಗಚ್ಛ propagateNaN;
                uiZ = packToF32UI( signZ, 0xFF, 0 );
                ಗಚ್ಛ uiZ;
            }
            expZ = expB;
            sigA += expA ? 0x20000000 : sigA;
            sigA = softfloat_shift_ಬಲ_Jam32( sigA, -expDiff );
        } ಅನ್ಯಥಾ {
            ಯದಿ ( expA == 0xFF ) {
                ಯದಿ ( sigA ) ಗಚ್ಛ propagateNaN;
                uiZ = uiA;
                ಗಚ್ಛ uiZ;
            }
            expZ = expA;
            sigB += expB ? 0x20000000 : sigB;
            sigB = softfloat_shift_ಬಲ_Jam32( sigB, expDiff );
        }
        sigZ = 0x20000000 + sigA + sigB;
        ಯದಿ ( sigZ < 0x40000000 ) {
            --expZ;
            sigZ <<= 1;
        }
    }
    ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF32( signZ, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = softfloat_propagateNaNF32UI( uiA, uiB );
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float64_t
 softfloat_addMagsF64( uint_fast64_t uiA, uint_fast64_t uiB, ತಾರ್ಕಿಕ signZ )
{
    int_fast16_t expA;
    uint_fast64_t sigA;
    int_fast16_t expB;
    uint_fast64_t sigB;
    int_fast16_t expDiff;
    uint_fast64_t uiZ;
    int_fast16_t expZ;
    uint_fast64_t sigZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expA = expF64UI( uiA );
    sigA = fracF64UI( uiA );
    expB = expF64UI( uiB );
    sigB = fracF64UI( uiB );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expDiff = expA - expB;
    ಯದಿ ( ! expDiff ) {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        ಯದಿ ( ! expA ) {
            uiZ = uiA + sigB;
            ಗಚ್ಛ uiZ;
        }
        ಯದಿ ( expA == 0x7FF ) {
            ಯದಿ ( sigA | sigB ) ಗಚ್ಛ propagateNaN;
            uiZ = uiA;
            ಗಚ್ಛ uiZ;
        }
        expZ = expA;
        sigZ = UINT64_C( 0x0020000000000000 ) + sigA + sigB;
        sigZ <<= 9;
    } ಅನ್ಯಥಾ {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        sigA <<= 9;
        sigB <<= 9;
        ಯದಿ ( expDiff < 0 ) {
            ಯದಿ ( expB == 0x7FF ) {
                ಯದಿ ( sigB ) ಗಚ್ಛ propagateNaN;
                uiZ = packToF64UI( signZ, 0x7FF, 0 );
                ಗಚ್ಛ uiZ;
            }
            expZ = expB;
            ಯದಿ ( expA ) {
                sigA += UINT64_C( 0x2000000000000000 );
            } ಅನ್ಯಥಾ {
                sigA <<= 1;
            }
            sigA = softfloat_shift_ಬಲ_Jam64( sigA, -expDiff );
        } ಅನ್ಯಥಾ {
            ಯದಿ ( expA == 0x7FF ) {
                ಯದಿ ( sigA ) ಗಚ್ಛ propagateNaN;
                uiZ = uiA;
                ಗಚ್ಛ uiZ;
            }
            expZ = expA;
            ಯದಿ ( expB ) {
                sigB += UINT64_C( 0x2000000000000000 );
            } ಅನ್ಯಥಾ {
                sigB <<= 1;
            }
            sigB = softfloat_shift_ಬಲ_Jam64( sigB, expDiff );
        }
        sigZ = UINT64_C( 0x2000000000000000 ) + sigA + sigB;
        ಯದಿ ( sigZ < UINT64_C( 0x4000000000000000 ) ) {
            --expZ;
            sigZ <<= 1;
        }
    }
    ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF64( signZ, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = softfloat_propagateNaNF64UI( uiA, uiB );
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(uiZ);
}

ಸ್ಥಿರ uint32_t softfloat_approxRecipSqrt32_1( unsigned ಪೂರ್ಣಾಙ್ಕ oddExpA, uint32_t a )
{
    ಪೂರ್ಣಾಙ್ಕ index;
    uint16_t eps, r0;
    uint_fast32_t ESqrR0;
    uint32_t sigma0;
    uint_fast32_t r;
    uint32_t sqrSigma0;

    index = (a>>27 & 0xE) + oddExpA;
    eps = (uint16_t) (a>>12);
    r0 = softfloat_approxRecipSqrt_1k0s[index]
             - ((softfloat_approxRecipSqrt_1k1s[index] * (uint_fast32_t) eps)
                    >>20);
    ESqrR0 = (uint_fast32_t) r0 * r0;
    ಯದಿ ( ! oddExpA ) ESqrR0 <<= 1;
    sigma0 = ~(uint_fast32_t) (((uint32_t) ESqrR0 * (uint_fast64_t) a)>>23);
    r = (uint_fast32_t)(((uint_fast32_t) r0<<16) + ((r0 * (uint_fast64_t) sigma0)>>25)); //fixed warning on type cast
    sqrSigma0 = ((uint_fast64_t) sigma0 * sigma0)>>32;
    r += ((uint32_t) ((r>>1) + (r>>3) - ((uint_fast32_t) r0<<14))
              * (uint_fast64_t) sqrSigma0)
             >>48;
    ಯದಿ ( ! (r & 0x80000000) ) r = 0x80000000;
    ಪ್ರತ್ಯಾಗಮ r;
}

/*----------------------------------------------------------------------------
| Converts the common NaN pointed to by `aPtr' into a 32-bit floating-point
| NaN, and returns the bit pattern of this value as an unsigned integer.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ uint_fast32_t softfloat_commonNaNToF32UI( ಧ್ರುವ ಸಂರಚನಾ commonNaN *aPtr )
{
    ಪ್ರತ್ಯಾಗಮ (uint_fast32_t) aPtr->sign<<31 | 0x7FC00000 | aPtr->v64>>41;
}

/*----------------------------------------------------------------------------
| Converts the common NaN pointed to by `aPtr' into a 64-bit floating-point
| NaN, and returns the bit pattern of this value as an unsigned integer.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ uint_fast64_t softfloat_commonNaNToF64UI( ಧ್ರುವ ಸಂರಚನಾ commonNaN *aPtr )
{
    ಪ್ರತ್ಯಾಗಮ
        (uint_fast64_t) aPtr->sign<<63 | UINT64_C( 0x7FF8000000000000 )
            | aPtr->v64>>12;
}

ಸ್ಥಿರ uint_fast8_t softfloat_ಎಣಿಕೆ_Leading_Zeros64( uint64_t a )
{
    uint_fast8_t ಎಣಿಕೆ_2;
    uint32_t a32;

    ಎಣಿಕೆ_2 = 0;
    a32 = a>>32;
    ಯದಿ ( ! a32 ) {
        ಎಣಿಕೆ_2 = 32;
        a32 = (uint32_t) a; //fixed warning on type cast
    }
    /*------------------------------------------------------------------------
    | From here, result is current count + count leading zeros of `a32'.
    *------------------------------------------------------------------------*/
    ಯದಿ ( a32 < 0x10000 ) {
        ಎಣಿಕೆ_2 += 16;
        a32 <<= 16;
    }
    ಯದಿ ( a32 < 0x1000000 ) {
        ಎಣಿಕೆ_2 += 8;
        a32 <<= 8;
    }
    ಎಣಿಕೆ_2 += softfloat_ಎಣಿಕೆ_Leading_Zeros8[a32>>24];
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ_2;
}

/*----------------------------------------------------------------------------
| Assuming `uiA' has the bit pattern of a 32-bit floating-point NaN, converts
| this NaN to the common NaN form, and stores the resulting common NaN at the
| location pointed to by `zPtr'.  If the NaN is a signaling NaN, the invalid
| exception is raised.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ ಶೂನ್ಯ softfloat_f32UIToCommonNaN( uint_fast32_t uiA, ಸಂರಚನಾ commonNaN *zPtr )
{
    ಯದಿ ( softfloat_isSigNaNF32UI( uiA ) ) {
        raiseFlags( flag_invalid );
    }
    zPtr->sign = (uiA>>31) != 0;
    zPtr->v64  = (uint_fast64_t) uiA<<41;
    zPtr->v0   = 0;
}

/*----------------------------------------------------------------------------
| Assuming `uiA' has the bit pattern of a 64-bit floating-point NaN, converts
| this NaN to the common NaN form, and stores the resulting common NaN at the
| location pointed to by `zPtr'.  If the NaN is a signaling NaN, the invalid
| exception is raised.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ ಶೂನ್ಯ softfloat_f64UIToCommonNaN( uint_fast64_t uiA, ಸಂರಚನಾ commonNaN *zPtr )
{
    ಯದಿ ( softfloat_isSigNaNF64UI( uiA ) ) {
        raiseFlags( flag_invalid );
    }
    zPtr->sign = (uiA>>63) != 0;
    zPtr->v64  = uiA<<12;
    zPtr->v0   = 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ uint128 softfloat_mul64To128( uint64_t a, uint64_t b )
{
    uint32_t a32, a0, b32, b0;
    ಸಂರಚನಾ uint128 z;
    uint64_t mid1, mid;

    a32 = a>>32;
    a0 = (uint32_t)a; //fixed warning on type cast
    b32 = b>>32;
    b0 = (uint32_t) b; //fixed warning on type cast
    z.v0 = (uint_fast64_t) a0 * b0;
    mid1 = (uint_fast64_t) a32 * b0;
    mid = mid1 + (uint_fast64_t) a0 * b32;
    z.v64 = (uint_fast64_t) a32 * b32;
    z.v64 += (uint_fast64_t) (mid < mid1)<<32 | mid>>32;
    mid <<= 32;
    z.v0 += mid;
    z.v64 += (z.v0 < mid);
    ಪ್ರತ್ಯಾಗಮ z;
}

ಸ್ಥಿರ float32_t
 softfloat_mulAddF32(
     uint_fast32_t uiA, uint_fast32_t uiB, uint_fast32_t uiC, uint_fast8_t op )
{
    ತಾರ್ಕಿಕ signA;
    int_fast16_t expA;
    uint_fast32_t sigA;
    ತಾರ್ಕಿಕ signB;
    int_fast16_t expB;
    uint_fast32_t sigB;
    ತಾರ್ಕಿಕ signC;
    int_fast16_t expC;
    uint_fast32_t sigC;
    ತಾರ್ಕಿಕ signProd;
    uint_fast32_t magBits, uiZ;
    ಸಂರಚನಾ exp16_sig32 normExpSig;
    int_fast16_t expProd;
    uint_fast64_t sigProd;
    ತಾರ್ಕಿಕ signZ;
    int_fast16_t expZ;
    uint_fast32_t sigZ;
    int_fast16_t expDiff;
    uint_fast64_t sig64Z, sig64C;
    int_fast8_t shiftDist;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    signA = signF32UI( uiA );
    expA  = expF32UI( uiA );
    sigA  = fracF32UI( uiA );
    signB = signF32UI( uiB );
    expB  = expF32UI( uiB );
    sigB  = fracF32UI( uiB );
    signC = signF32UI( uiC ) ^ (op == softfloat_mulAdd_subC);
    expC  = expF32UI( uiC );
    sigC  = fracF32UI( uiC );
    signProd = signA ^ signB ^ (op == softfloat_mulAdd_subProd);
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( expA == 0xFF ) {
        ಯದಿ ( sigA || ((expB == 0xFF) && sigB) ) ಗಚ್ಛ propagateNaN_ABC;
        magBits = expB | sigB;
        ಗಚ್ಛ infProdArg;
    }
    ಯದಿ ( expB == 0xFF ) {
        ಯದಿ ( sigB ) ಗಚ್ಛ propagateNaN_ABC;
        magBits = expA | sigA;
        ಗಚ್ಛ infProdArg;
    }
    ಯದಿ ( expC == 0xFF ) {
        ಯದಿ ( sigC ) {
            uiZ = 0;
            ಗಚ್ಛ propagateNaN_ZC;
        }
        uiZ = uiC;
        ಗಚ್ಛ uiZ;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( ! expA ) {
        ಯದಿ ( ! sigA ) ಗಚ್ಛ zeroProd;
        normExpSig = softfloat_normSubnormalF32Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }
    ಯದಿ ( ! expB ) {
        ಯದಿ ( ! sigB ) ಗಚ್ಛ zeroProd;
        normExpSig = softfloat_normSubnormalF32Sig( sigB );
        expB = normExpSig.exp;
        sigB = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expProd = expA + expB - 0x7E;
    sigA = (sigA | 0x00800000)<<7;
    sigB = (sigB | 0x00800000)<<7;
    sigProd = (uint_fast64_t) sigA * sigB;
    ಯದಿ ( sigProd < UINT64_C( 0x2000000000000000 ) ) {
        --expProd;
        sigProd <<= 1;
    }
    signZ = signProd;
    ಯದಿ ( ! expC ) {
        ಯದಿ ( ! sigC ) {
            expZ = expProd - 1;
            sigZ = (uint_fast32_t) softfloat_short_Shift_ಬಲ_Jam64( sigProd, 31 ); //fixed warning on type cast
            ಗಚ್ಛ roundPack;
        }
        normExpSig = softfloat_normSubnormalF32Sig( sigC );
        expC = normExpSig.exp;
        sigC = normExpSig.sig;
    }
    sigC = (sigC | 0x00800000)<<6;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expDiff = expProd - expC;
    ಯದಿ ( signProd == signC ) {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        ಯದಿ ( expDiff <= 0 ) {
            expZ = expC;
            sigZ = sigC + (uint_fast32_t) softfloat_shift_ಬಲ_Jam64( sigProd, 32 - expDiff ); //fixed warning on type cast
        } ಅನ್ಯಥಾ {
            expZ = expProd;
            sig64Z =
                sigProd
                    + softfloat_shift_ಬಲ_Jam64(
                          (uint_fast64_t) sigC<<32, expDiff );
            sigZ = (uint_fast32_t) softfloat_short_Shift_ಬಲ_Jam64( sig64Z, 32 ); //fixed warning on type cast
        }
        ಯದಿ ( sigZ < 0x40000000 ) {
            --expZ;
            sigZ <<= 1;
        }
    } ಅನ್ಯಥಾ {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        sig64C = (uint_fast64_t) sigC<<32;
        ಯದಿ ( expDiff < 0 ) {
            signZ = signC;
            expZ = expC;
            sig64Z = sig64C - softfloat_shift_ಬಲ_Jam64( sigProd, -expDiff );
        } ಅನ್ಯಥಾ ಯದಿ ( ! expDiff ) {
            expZ = expProd;
            sig64Z = sigProd - sig64C;
            ಯದಿ ( ! sig64Z ) ಗಚ್ಛ completeCancellation;
            ಯದಿ ( sig64Z & UINT64_C( 0x8000000000000000 ) ) {
                signZ = ! signZ;
                //fixed unsigned unary minus: -x == ~x + 1
                sig64Z = ~sig64Z + 1;
            }
        } ಅನ್ಯಥಾ {
            expZ = expProd;
            sig64Z = sigProd - softfloat_shift_ಬಲ_Jam64( sig64C, expDiff );
        }
        shiftDist = softfloat_ಎಣಿಕೆ_Leading_Zeros64( sig64Z ) - 1;
        expZ -= shiftDist;
        shiftDist -= 32;
        ಯದಿ ( shiftDist < 0 ) {
            sigZ = (uint_fast32_t) softfloat_short_Shift_ಬಲ_Jam64( sig64Z, -shiftDist ); //fixed warning on type cast
        } ಅನ್ಯಥಾ {
            sigZ = (uint_fast32_t) sig64Z<<shiftDist;
        }
    }
 roundPack:
    ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF32( signZ, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN_ABC:
    uiZ = softfloat_propagateNaNF32UI( uiA, uiB );
    ಗಚ್ಛ propagateNaN_ZC;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 infProdArg:
    ಯದಿ ( magBits ) {
        uiZ = packToF32UI( signProd, 0xFF, 0 );
        ಯದಿ ( expC != 0xFF ) ಗಚ್ಛ uiZ;
        ಯದಿ ( sigC ) ಗಚ್ಛ propagateNaN_ZC;
        ಯದಿ ( signProd == signC ) ಗಚ್ಛ uiZ;
    }
    raiseFlags( flag_invalid );
    uiZ = defaultNaNF32UI;
 propagateNaN_ZC:
    uiZ = softfloat_propagateNaNF32UI( uiZ, uiC );
    ಗಚ್ಛ uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 zeroProd:
    uiZ = uiC;
    ಯದಿ ( ! (expC | sigC) && (signProd != signC) ) {
 completeCancellation:
        uiZ =
            packToF32UI((globalRoundingMode == round_min), 0, 0 );
    }
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float64_t
 softfloat_mulAddF64(
     uint_fast64_t uiA, uint_fast64_t uiB, uint_fast64_t uiC, uint_fast8_t op )
{
    ತಾರ್ಕಿಕ signA;
    int_fast16_t expA;
    uint_fast64_t sigA;
    ತಾರ್ಕಿಕ signB;
    int_fast16_t expB;
    uint_fast64_t sigB;
    ತಾರ್ಕಿಕ signC;
    int_fast16_t expC;
    uint_fast64_t sigC;
    ತಾರ್ಕಿಕ signZ;
    uint_fast64_t magBits, uiZ;
    ಸಂರಚನಾ exp16_sig64 normExpSig;
    int_fast16_t expZ;
    ಸಂರಚನಾ uint128 sig128Z;
    uint_fast64_t sigZ;
    int_fast16_t expDiff;
    ಸಂರಚನಾ uint128 sig128C;
    int_fast8_t shiftDist;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    signA = signF64UI( uiA );
    expA  = expF64UI( uiA );
    sigA  = fracF64UI( uiA );
    signB = signF64UI( uiB );
    expB  = expF64UI( uiB );
    sigB  = fracF64UI( uiB );
    signC = signF64UI( uiC ) ^ (op == softfloat_mulAdd_subC);
    expC  = expF64UI( uiC );
    sigC  = fracF64UI( uiC );
    signZ = signA ^ signB ^ (op == softfloat_mulAdd_subProd);
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( expA == 0x7FF ) {
        ಯದಿ ( sigA || ((expB == 0x7FF) && sigB) ) ಗಚ್ಛ propagateNaN_ABC;
        magBits = expB | sigB;
        ಗಚ್ಛ infProdArg;
    }
    ಯದಿ ( expB == 0x7FF ) {
        ಯದಿ ( sigB ) ಗಚ್ಛ propagateNaN_ABC;
        magBits = expA | sigA;
        ಗಚ್ಛ infProdArg;
    }
    ಯದಿ ( expC == 0x7FF ) {
        ಯದಿ ( sigC ) {
            uiZ = 0;
            ಗಚ್ಛ propagateNaN_ZC;
        }
        uiZ = uiC;
        ಗಚ್ಛ uiZ;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( ! expA ) {
        ಯದಿ ( ! sigA ) ಗಚ್ಛ zeroProd;
        normExpSig = softfloat_normSubnormalF64Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }
    ಯದಿ ( ! expB ) {
        ಯದಿ ( ! sigB ) ಗಚ್ಛ zeroProd;
        normExpSig = softfloat_normSubnormalF64Sig( sigB );
        expB = normExpSig.exp;
        sigB = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expZ = expA + expB - 0x3FE;
    sigA = (sigA | UINT64_C( 0x0010000000000000 ))<<10;
    sigB = (sigB | UINT64_C( 0x0010000000000000 ))<<10;
    sig128Z = softfloat_mul64To128( sigA, sigB );
    ಯದಿ ( sig128Z.v64 < UINT64_C( 0x2000000000000000 ) ) {
        --expZ;
        sig128Z =
            softfloat_add128(
                sig128Z.v64, sig128Z.v0, sig128Z.v64, sig128Z.v0 );
    }
    ಯದಿ ( ! expC ) {
        ಯದಿ ( ! sigC ) {
            --expZ;
            sigZ = sig128Z.v64<<1 | (sig128Z.v0 != 0);
            ಗಚ್ಛ roundPack;
        }
        normExpSig = softfloat_normSubnormalF64Sig( sigC );
        expC = normExpSig.exp;
        sigC = normExpSig.sig;
    }
    sigC = (sigC | UINT64_C( 0x0010000000000000 ))<<9;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    //fixed initialization
    sig128C.v0 = sig128C.v64 = 0;
    expDiff = expZ - expC;
    ಯದಿ ( expDiff < 0 ) {
        expZ = expC;
        ಯದಿ ( (signZ == signC) || (expDiff < -1) ) {
            sig128Z.v64 = softfloat_shift_ಬಲ_Jam64( sig128Z.v64, -expDiff );
        } ಅನ್ಯಥಾ {
            sig128Z =
                softfloat_short_Shift_ಬಲ_Jam128( sig128Z.v64, sig128Z.v0, 1 );
        }
    } ಅನ್ಯಥಾ ಯದಿ ( expDiff ) {
        sig128C = softfloat_shift_ಬಲ_Jam128( sigC, 0, expDiff );
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( signZ == signC ) {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        ಯದಿ ( expDiff <= 0 ) {
            sigZ = (sigC + sig128Z.v64) | (sig128Z.v0 != 0);
        } ಅನ್ಯಥಾ {
            sig128Z =
                softfloat_add128(
                    sig128Z.v64, sig128Z.v0, sig128C.v64, sig128C.v0 );
            sigZ = sig128Z.v64 | (sig128Z.v0 != 0);
        }
        ಯದಿ ( sigZ < UINT64_C( 0x4000000000000000 ) ) {
            --expZ;
            sigZ <<= 1;
        }
    } ಅನ್ಯಥಾ {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        ಯದಿ ( expDiff < 0 ) {
            signZ = signC;
            sig128Z = softfloat_sub128( sigC, 0, sig128Z.v64, sig128Z.v0 );
        } ಅನ್ಯಥಾ ಯದಿ ( ! expDiff ) {
            sig128Z.v64 = sig128Z.v64 - sigC;
            ಯದಿ ( ! (sig128Z.v64 | sig128Z.v0) ) ಗಚ್ಛ completeCancellation;
            ಯದಿ ( sig128Z.v64 & UINT64_C( 0x8000000000000000 ) ) {
                signZ = ! signZ;
                sig128Z = softfloat_sub128( 0, 0, sig128Z.v64, sig128Z.v0 );
            }
        } ಅನ್ಯಥಾ {
            sig128Z =
                softfloat_sub128(
                    sig128Z.v64, sig128Z.v0, sig128C.v64, sig128C.v0 );
        }
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        ಯದಿ ( ! sig128Z.v64 ) {
            expZ -= 64;
            sig128Z.v64 = sig128Z.v0;
            sig128Z.v0 = 0;
        }
        shiftDist = softfloat_ಎಣಿಕೆ_Leading_Zeros64( sig128Z.v64 ) - 1;
        expZ -= shiftDist;
        ಯದಿ ( shiftDist < 0 ) {
            sigZ = softfloat_short_Shift_ಬಲ_Jam64( sig128Z.v64, -shiftDist );
        } ಅನ್ಯಥಾ {
            sig128Z =
                softfloat_shortShiftLeft128(
                    sig128Z.v64, sig128Z.v0, shiftDist );
            sigZ = sig128Z.v64;
        }
        sigZ |= (sig128Z.v0 != 0);
    }
 roundPack:
    ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF64( signZ, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN_ABC:
    uiZ = softfloat_propagateNaNF64UI( uiA, uiB );
    ಗಚ್ಛ propagateNaN_ZC;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 infProdArg:
    ಯದಿ ( magBits ) {
        uiZ = packToF64UI( signZ, 0x7FF, 0 );
        ಯದಿ ( expC != 0x7FF ) ಗಚ್ಛ uiZ;
        ಯದಿ ( sigC ) ಗಚ್ಛ propagateNaN_ZC;
        ಯದಿ ( signZ == signC ) ಗಚ್ಛ uiZ;
    }
    raiseFlags( flag_invalid );
    uiZ = defaultNaNF64UI;
 propagateNaN_ZC:
    uiZ = softfloat_propagateNaNF64UI( uiZ, uiC );
    ಗಚ್ಛ uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 zeroProd:
    uiZ = uiC;
    ಯದಿ ( ! (expC | sigC) && (signZ != signC) ) {
 completeCancellation:
        uiZ =
            packToF64UI((globalRoundingMode == round_min), 0, 0 );
    }
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float32_t
 softfloat_normRoundPackToF32( ತಾರ್ಕಿಕ sign, int_fast16_t exp, uint_fast32_t sig )
{
    int_fast8_t shiftDist;

    shiftDist = softfloat_ಎಣಿಕೆ_Leading_Zeros32( sig ) - 1;
    exp -= shiftDist;
    ಯದಿ ( (7 <= shiftDist) && ((unsigned ಪೂರ್ಣಾಙ್ಕ) exp < 0xFD) ) {
        ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(packToF32UI( sign, sig ? exp : 0, sig<<(shiftDist - 7) ));
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF32( sign, exp, sig<<shiftDist );
    }
}

ಸ್ಥಿರ float64_t
 softfloat_normRoundPackToF64( ತಾರ್ಕಿಕ sign, int_fast16_t exp, uint_fast64_t sig )
{
    int_fast8_t shiftDist;

    shiftDist = softfloat_ಎಣಿಕೆ_Leading_Zeros64( sig ) - 1;
    exp -= shiftDist;
    ಯದಿ ( (10 <= shiftDist) && ((unsigned ಪೂರ್ಣಾಙ್ಕ) exp < 0x7FD) ) {
        ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(packToF64UI( sign, sig ? exp : 0, sig<<(shiftDist - 10) ));
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF64( sign, exp, sig<<shiftDist );
    }
}

ಸ್ಥಿರ ಸಂರಚನಾ exp16_sig32 softfloat_normSubnormalF32Sig( uint_fast32_t sig )
{
    int_fast8_t shiftDist;
    ಸಂರಚನಾ exp16_sig32 z;

    shiftDist = softfloat_ಎಣಿಕೆ_Leading_Zeros32( sig ) - 8;
    z.exp = 1 - shiftDist;
    z.sig = sig<<shiftDist;
    ಪ್ರತ್ಯಾಗಮ z;
}

ಸ್ಥಿರ ಸಂರಚನಾ exp16_sig64 softfloat_normSubnormalF64Sig( uint_fast64_t sig )
{
    int_fast8_t shiftDist;
    ಸಂರಚನಾ exp16_sig64 z;

    shiftDist = softfloat_ಎಣಿಕೆ_Leading_Zeros64( sig ) - 11;
    z.exp = 1 - shiftDist;
    z.sig = sig<<shiftDist;
    ಪ್ರತ್ಯಾಗಮ z;
}

/*----------------------------------------------------------------------------
| Interpreting `uiA' and `uiB' as the bit patterns of two 32-bit floating-
| point values, at least one of which is a NaN, returns the bit pattern of
| the combined NaN result.  If either `uiA' or `uiB' has the pattern of a
| signaling NaN, the invalid exception is raised.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ uint_fast32_t
 softfloat_propagateNaNF32UI( uint_fast32_t uiA, uint_fast32_t uiB )
{
    ತಾರ್ಕಿಕ isSigNaNA;

    isSigNaNA = softfloat_isSigNaNF32UI( uiA );
    ಯದಿ ( isSigNaNA || softfloat_isSigNaNF32UI( uiB ) ) {
        raiseFlags( flag_invalid );
        ಯದಿ ( isSigNaNA ) ಪ್ರತ್ಯಾಗಮ uiA | 0x00400000;
    }
    ಪ್ರತ್ಯಾಗಮ (isNaNF32UI( uiA ) ? uiA : uiB) | 0x00400000;
}

/*----------------------------------------------------------------------------
| Interpreting `uiA' and `uiB' as the bit patterns of two 64-bit floating-
| point values, at least one of which is a NaN, returns the bit pattern of
| the combined NaN result.  If either `uiA' or `uiB' has the pattern of a
| signaling NaN, the invalid exception is raised.
*----------------------------------------------------------------------------*/
ಸ್ಥಿರ uint_fast64_t
 softfloat_propagateNaNF64UI( uint_fast64_t uiA, uint_fast64_t uiB )
{
    ತಾರ್ಕಿಕ isSigNaNA;

    isSigNaNA = softfloat_isSigNaNF64UI( uiA );
    ಯದಿ ( isSigNaNA || softfloat_isSigNaNF64UI( uiB ) ) {
        raiseFlags( flag_invalid );
        ಯದಿ ( isSigNaNA ) ಪ್ರತ್ಯಾಗಮ uiA | UINT64_C( 0x0008000000000000 );
    }
    ಪ್ರತ್ಯಾಗಮ (isNaNF64UI( uiA ) ? uiA : uiB) | UINT64_C( 0x0008000000000000 );
}

ಸ್ಥಿರ float32_t
 softfloat_roundPackToF32( ತಾರ್ಕಿಕ sign, int_fast16_t exp, uint_fast32_t sig )
{
    uint_fast8_t roundingMode;
    ತಾರ್ಕಿಕ roundNearEven;
    uint_fast8_t roundIncrement, roundBits;
    ತಾರ್ಕಿಕ isTiny;
    uint_fast32_t uiZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    roundingMode = globalRoundingMode;
    roundNearEven = (roundingMode == round_near_even);
    roundIncrement = 0x40;
    ಯದಿ ( ! roundNearEven && (roundingMode != round_near_maxMag) ) {
        roundIncrement =
            (roundingMode
                 == (sign ? round_min : round_max))
                ? 0x7F
                : 0;
    }
    roundBits = sig & 0x7F;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( 0xFD <= (unsigned ಪೂರ್ಣಾಙ್ಕ) exp ) {
        ಯದಿ ( exp < 0 ) {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            isTiny =
                (globalDetectTininess == tininess_beforeRounding)
                    || (exp < -1) || (sig + roundIncrement < 0x80000000);
            sig = softfloat_shift_ಬಲ_Jam32( sig, -exp );
            exp = 0;
            roundBits = sig & 0x7F;
            ಯದಿ ( isTiny && roundBits ) {
                raiseFlags( flag_underflow );
            }
        } ಅನ್ಯಥಾ ಯದಿ ( (0xFD < exp) || (0x80000000 <= sig + roundIncrement) ) {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            raiseFlags(
                flag_overflow | flag_inexact );
            uiZ = packToF32UI( sign, 0xFF, 0 ) - ! roundIncrement;
            ಗಚ್ಛ uiZ;
        }
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    sig = (sig + roundIncrement)>>7;
    ಯದಿ ( roundBits ) {
        raiseFlags(flag_inexact);
        ಯದಿ ( roundingMode == round_odd ) {
            sig |= 1;
            ಗಚ್ಛ packReturn;
        }
    }
    sig &= ~(uint_fast32_t) (! (roundBits ^ 0x40) & roundNearEven);
    ಯದಿ ( ! sig ) exp = 0;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 packReturn:
    uiZ = packToF32UI( sign, exp, sig );
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float64_t
 softfloat_roundPackToF64( ತಾರ್ಕಿಕ sign, int_fast16_t exp, uint_fast64_t sig )
{
    uint_fast8_t roundingMode;
    ತಾರ್ಕಿಕ roundNearEven;
    uint_fast16_t roundIncrement, roundBits;
    ತಾರ್ಕಿಕ isTiny;
    uint_fast64_t uiZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    roundingMode = globalRoundingMode;
    roundNearEven = (roundingMode == round_near_even);
    roundIncrement = 0x200;
    ಯದಿ ( ! roundNearEven && (roundingMode != round_near_maxMag) ) {
        roundIncrement =
            (roundingMode
                 == (sign ? round_min : round_max))
                ? 0x3FF
                : 0;
    }
    roundBits = sig & 0x3FF;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    ಯದಿ ( 0x7FD <= (uint16_t) exp ) {
        ಯದಿ ( exp < 0 ) {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            isTiny =
                (globalDetectTininess == tininess_beforeRounding)
                    || (exp < -1)
                    || (sig + roundIncrement < UINT64_C( 0x8000000000000000 ));
            sig = softfloat_shift_ಬಲ_Jam64( sig, -exp );
            exp = 0;
            roundBits = sig & 0x3FF;
            ಯದಿ ( isTiny && roundBits ) {
                raiseFlags( flag_underflow );
            }
        } ಅನ್ಯಥಾ ಯದಿ (
            (0x7FD < exp)
                || (UINT64_C( 0x8000000000000000 ) <= sig + roundIncrement)
        ) {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            raiseFlags(
                flag_overflow | flag_inexact );
            uiZ = packToF64UI( sign, 0x7FF, 0 ) - ! roundIncrement;
            ಗಚ್ಛ uiZ;
        }
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    sig = (sig + roundIncrement)>>10;
    ಯದಿ ( roundBits ) {
        raiseFlags(flag_inexact);
        ಯದಿ ( roundingMode == round_odd ) {
            sig |= 1;
            ಗಚ್ಛ packReturn;
        }
    }
    sig &= ~(uint_fast64_t) (! (roundBits ^ 0x200) & roundNearEven);
    ಯದಿ ( ! sig ) exp = 0;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 packReturn:
    uiZ = packToF64UI( sign, exp, sig );
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(uiZ);
}

ಸ್ಥಿರ int_fast32_t
 softfloat_roundToI32(
     ತಾರ್ಕಿಕ sign, uint_fast64_t sig, uint_fast8_t roundingMode, ತಾರ್ಕಿಕ exact )
{
    ತಾರ್ಕಿಕ roundNearEven;
    uint_fast16_t roundIncrement, roundBits;
    uint_fast32_t sig32;
    ಸಙ್ಘ { uint32_t ui; int32_t i; } uZ;
    int_fast32_t z;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    roundNearEven = (roundingMode == round_near_even);
    roundIncrement = 0x800;
    ಯದಿ ( ! roundNearEven && (roundingMode != round_near_maxMag) ) {
        roundIncrement =
            (roundingMode
                 == (sign ? round_min : round_max))
                ? 0xFFF
                : 0;
    }
    roundBits = sig & 0xFFF;
    sig += roundIncrement;
    ಯದಿ ( sig & UINT64_C( 0xFFFFF00000000000 ) ) ಗಚ್ಛ invalid;
    sig32 = (uint_fast32_t)(sig>>12); //fixed warning on type cast
    sig32 &= ~(uint_fast32_t) (! (roundBits ^ 0x800) & roundNearEven);
    //fixed unsigned unary minus: -x == ~x + 1
    uZ.ui = sign ? (~sig32 + 1) : sig32;
    z = uZ.i;
    ಯದಿ ( z && ((z < 0) ^ sign) ) ಗಚ್ಛ invalid;
    ಯದಿ ( exact && roundBits ) {
        raiseFlags(flag_inexact);
    }
    ಪ್ರತ್ಯಾಗಮ z;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 invalid:
    raiseFlags( flag_invalid );
    ಪ್ರತ್ಯಾಗಮ sign ? i32_fromNegOverflow : i32_fromPosOverflow;
}

ಸ್ಥಿರ int_fast64_t
 softfloat_roundToI64(
    ತಾರ್ಕಿಕ sign, uint_fast64_t sig, uint_fast64_t sigExtra, uint_fast8_t roundingMode, ತಾರ್ಕಿಕ exact )
{
    ತಾರ್ಕಿಕ roundNearEven, doIncrement;
    ಸಙ್ಘ { uint64_t ui; int64_t i; } uZ;
    int_fast64_t z;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    roundNearEven = (roundingMode == round_near_even);
    doIncrement = (UINT64_C(0x8000000000000000) <= sigExtra);
    ಯದಿ (!roundNearEven && (roundingMode != round_near_maxMag)) {
        doIncrement =
            (roundingMode
                == (sign ? round_min : round_max))
            && sigExtra;
    }
    ಯದಿ (doIncrement) {
        ++sig;
        ಯದಿ (!sig) ಗಚ್ಛ invalid;
        sig &=
            ~(uint_fast64_t)
            (!(sigExtra & UINT64_C(0x7FFFFFFFFFFFFFFF))
                & roundNearEven);
    }
    uZ.ui = sign ? (~sig + 1) : sig;
    z = uZ.i;
    ಯದಿ (z && ((z < 0) ^ sign)) ಗಚ್ಛ invalid;
    ಯದಿ (exact && sigExtra) {
        raiseFlags(flag_inexact);
    }
    ಪ್ರತ್ಯಾಗಮ z;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
invalid:
    raiseFlags(flag_invalid);
    ಪ್ರತ್ಯಾಗಮ sign ? i64_fromNegOverflow : i64_fromPosOverflow;
}

ಸ್ಥಿರ ಸಂರಚನಾ uint128
 softfloat_shift_ಬಲ_Jam128( uint64_t a64, uint64_t a0, uint_fast32_t dist )
{
    uint_fast8_t u8NegDist;
    ಸಂರಚನಾ uint128 z;

    ಯದಿ ( dist < 64 ) {
        //fixed unsigned unary minus: -x == ~x + 1 , fixed type cast
        u8NegDist = (uint_fast8_t)(~dist + 1);
        z.v64 = a64>>dist;
        z.v0 =
            a64<<(u8NegDist & 63) | a0>>dist
                | ((uint64_t) (a0<<(u8NegDist & 63)) != 0);
    } ಅನ್ಯಥಾ {
        z.v64 = 0;
        z.v0 =
            (dist < 127)
                ? a64>>(dist & 63)
                      | (((a64 & (((uint_fast64_t) 1<<(dist & 63)) - 1)) | a0)
                             != 0)
                : ((a64 | a0) != 0);
    }
    ಪ್ರತ್ಯಾಗಮ z;
}

ಸ್ಥಿರ float32_t softfloat_subMagsF32( uint_fast32_t uiA, uint_fast32_t uiB )
{
    int_fast16_t expA;
    uint_fast32_t sigA;
    int_fast16_t expB;
    uint_fast32_t sigB;
    int_fast16_t expDiff;
    uint_fast32_t uiZ;
    int_fast32_t sigDiff;
    ತಾರ್ಕಿಕ signZ;
    int_fast8_t shiftDist;
    int_fast16_t expZ;
    uint_fast32_t sigX, sigY;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expA = expF32UI( uiA );
    sigA = fracF32UI( uiA );
    expB = expF32UI( uiB );
    sigB = fracF32UI( uiB );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expDiff = expA - expB;
    ಯದಿ ( ! expDiff ) {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        ಯದಿ ( expA == 0xFF ) {
            ಯದಿ ( sigA | sigB ) ಗಚ್ಛ propagateNaN;
            raiseFlags( flag_invalid );
            uiZ = defaultNaNF32UI;
            ಗಚ್ಛ uiZ;
        }
        sigDiff = sigA - sigB;
        ಯದಿ ( ! sigDiff ) {
            uiZ =
                packToF32UI(
                    (globalRoundingMode == round_min), 0, 0 );
            ಗಚ್ಛ uiZ;
        }
        ಯದಿ ( expA ) --expA;
        signZ = signF32UI( uiA );
        ಯದಿ ( sigDiff < 0 ) {
            signZ = ! signZ;
            sigDiff = -sigDiff;
        }
        shiftDist = softfloat_ಎಣಿಕೆ_Leading_Zeros32( sigDiff ) - 8;
        expZ = expA - shiftDist;
        ಯದಿ ( expZ < 0 ) {
            shiftDist = (int_fast8_t)expA; //fixed type cast
            expZ = 0;
        }
        uiZ = packToF32UI( signZ, expZ, sigDiff<<shiftDist );
        ಗಚ್ಛ uiZ;
    } ಅನ್ಯಥಾ {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        signZ = signF32UI( uiA );
        sigA <<= 7;
        sigB <<= 7;
        ಯದಿ ( expDiff < 0 ) {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            signZ = ! signZ;
            ಯದಿ ( expB == 0xFF ) {
                ಯದಿ ( sigB ) ಗಚ್ಛ propagateNaN;
                uiZ = packToF32UI( signZ, 0xFF, 0 );
                ಗಚ್ಛ uiZ;
            }
            expZ = expB - 1;
            sigX = sigB | 0x40000000;
            sigY = sigA + (expA ? 0x40000000 : sigA);
            expDiff = -expDiff;
        } ಅನ್ಯಥಾ {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            ಯದಿ ( expA == 0xFF ) {
                ಯದಿ ( sigA ) ಗಚ್ಛ propagateNaN;
                uiZ = uiA;
                ಗಚ್ಛ uiZ;
            }
            expZ = expA - 1;
            sigX = sigA | 0x40000000;
            sigY = sigB + (expB ? 0x40000000 : sigB);
        }
        ಪ್ರತ್ಯಾಗಮ
            softfloat_normRoundPackToF32(
                signZ, expZ, sigX - softfloat_shift_ಬಲ_Jam32( sigY, expDiff )
            );
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = softfloat_propagateNaNF32UI( uiA, uiB );
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float64_t
 softfloat_subMagsF64( uint_fast64_t uiA, uint_fast64_t uiB, ತಾರ್ಕಿಕ signZ )
{
    int_fast16_t expA;
    uint_fast64_t sigA;
    int_fast16_t expB;
    uint_fast64_t sigB;
    int_fast16_t expDiff;
    uint_fast64_t uiZ;
    int_fast64_t sigDiff;
    int_fast8_t shiftDist;
    int_fast16_t expZ;
    uint_fast64_t sigZ;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expA = expF64UI( uiA );
    sigA = fracF64UI( uiA );
    expB = expF64UI( uiB );
    sigB = fracF64UI( uiB );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expDiff = expA - expB;
    ಯದಿ ( ! expDiff ) {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        ಯದಿ ( expA == 0x7FF ) {
            ಯದಿ ( sigA | sigB ) ಗಚ್ಛ propagateNaN;
            raiseFlags( flag_invalid );
            uiZ = defaultNaNF64UI;
            ಗಚ್ಛ uiZ;
        }
        sigDiff = sigA - sigB;
        ಯದಿ ( ! sigDiff ) {
            uiZ =
                packToF64UI(
                    (globalRoundingMode == round_min), 0, 0 );
            ಗಚ್ಛ uiZ;
        }
        ಯದಿ ( expA ) --expA;
        ಯದಿ ( sigDiff < 0 ) {
            signZ = ! signZ;
            sigDiff = -sigDiff;
        }
        shiftDist = softfloat_ಎಣಿಕೆ_Leading_Zeros64( sigDiff ) - 11;
        expZ = expA - shiftDist;
        ಯದಿ ( expZ < 0 ) {
            shiftDist = (int_fast8_t)expA; //fixed type cast
            expZ = 0;
        }
        uiZ = packToF64UI( signZ, expZ, sigDiff<<shiftDist );
        ಗಚ್ಛ uiZ;
    } ಅನ್ಯಥಾ {
        /*--------------------------------------------------------------------
        *--------------------------------------------------------------------*/
        sigA <<= 10;
        sigB <<= 10;
        ಯದಿ ( expDiff < 0 ) {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            signZ = ! signZ;
            ಯದಿ ( expB == 0x7FF ) {
                ಯದಿ ( sigB ) ಗಚ್ಛ propagateNaN;
                uiZ = packToF64UI( signZ, 0x7FF, 0 );
                ಗಚ್ಛ uiZ;
            }
            sigA += expA ? UINT64_C( 0x4000000000000000 ) : sigA;
            sigA = softfloat_shift_ಬಲ_Jam64( sigA, -expDiff );
            sigB |= UINT64_C( 0x4000000000000000 );
            expZ = expB;
            sigZ = sigB - sigA;
        } ಅನ್ಯಥಾ {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            ಯದಿ ( expA == 0x7FF ) {
                ಯದಿ ( sigA ) ಗಚ್ಛ propagateNaN;
                uiZ = uiA;
                ಗಚ್ಛ uiZ;
            }
            sigB += expB ? UINT64_C( 0x4000000000000000 ) : sigB;
            sigB = softfloat_shift_ಬಲ_Jam64( sigB, expDiff );
            sigA |= UINT64_C( 0x4000000000000000 );
            expZ = expA;
            sigZ = sigA - sigB;
        }
        ಪ್ರತ್ಯಾಗಮ softfloat_normRoundPackToF64( signZ, expZ - 1, sigZ );
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = softfloat_propagateNaNF64UI( uiA, uiB );
 uiZ:
    ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float32_t ui32_to_f32( uint32_t a )
{
    ಯದಿ ( ! a ) {
        ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(0);
    }
    ಯದಿ ( a & 0x80000000 ) {
        ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF32( 0, 0x9D, a>>1 | (a & 1) );
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ softfloat_normRoundPackToF32( 0, 0x9C, a );
    }
}

ಸ್ಥಿರ float64_t ui32_to_f64( uint32_t a )
{
    uint_fast64_t uiZ;
    int_fast8_t shiftDist;

    ಯದಿ ( ! a ) {
        uiZ = 0;
    } ಅನ್ಯಥಾ {
        shiftDist = softfloat_ಎಣಿಕೆ_Leading_Zeros32( a ) + 21;
        uiZ =
            packToF64UI( 0, 0x432 - shiftDist, (uint_fast64_t) a<<shiftDist );
    }
    ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(uiZ);
}

ಸ್ಥಿರ float32_t ui64_to_f32( uint64_t a )
{
    int_fast8_t shiftDist;
    uint_fast32_t sig;

    shiftDist = softfloat_ಎಣಿಕೆ_Leading_Zeros64( a ) - 40;
    ಯದಿ ( 0 <= shiftDist ) {
        ಪ್ರತ್ಯಾಗಮ float32_t::fromRaw(a ? packToF32UI(0, 0x95 - shiftDist, (uint_fast32_t) a<<shiftDist ) : 0);
    } ಅನ್ಯಥಾ {
        shiftDist += 7;
        sig =
            (shiftDist < 0) ? (uint_fast32_t) softfloat_short_Shift_ಬಲ_Jam64( a, -shiftDist ) //fixed warning on type cast
                : (uint_fast32_t) a<<shiftDist;
        ಪ್ರತ್ಯಾಗಮ softfloat_roundPackToF32( 0, 0x9C - shiftDist, sig );
    }
}

ಸ್ಥಿರ float64_t ui64_to_f64( uint64_t a )
{
    ಯದಿ ( ! a ) {
        ಪ್ರತ್ಯಾಗಮ float64_t::fromRaw(0);
    }
    ಯದಿ ( a & UINT64_C( 0x8000000000000000 ) ) {
        ಪ್ರತ್ಯಾಗಮ
            softfloat_roundPackToF64(
                0, 0x43D, softfloat_short_Shift_ಬಲ_Jam64( a, 1 ) );
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ softfloat_normRoundPackToF64( 0, 0x43C, a );
    }
}

/*----------------------------------------------------------------------------
| Ported from OpenCV.
*----------------------------------------------------------------------------*/

////////////////////////////////////// EXP /////////////////////////////////////

#define EXPTAB_SCALE 6
#define EXPTAB_MASK  ((1 << EXPTAB_SCALE) - 1)

// .9670371139572337719125840413672004409288e-2
ಸ್ಥಿರ ಧ್ರುವ softdouble EXPPOLY_32F_A0 = float64_t::fromRaw(0x3f83ce0f3e46f431);

ಸ್ಥಿರ ಧ್ರುವ uint64_t expTab[] = {
    0x3ff0000000000000, // 1.000000
    0x3ff02c9a3e778061, // 1.010889
    0x3ff059b0d3158574, // 1.021897
    0x3ff0874518759bc8, // 1.033025
    0x3ff0b5586cf9890f, // 1.044274
    0x3ff0e3ec32d3d1a2, // 1.055645
    0x3ff11301d0125b51, // 1.067140
    0x3ff1429aaea92de0, // 1.078761
    0x3ff172b83c7d517b, // 1.090508
    0x3ff1a35beb6fcb75, // 1.102383
    0x3ff1d4873168b9aa, // 1.114387
    0x3ff2063b88628cd6, // 1.126522
    0x3ff2387a6e756238, // 1.138789
    0x3ff26b4565e27cdd, // 1.151189
    0x3ff29e9df51fdee1, // 1.163725
    0x3ff2d285a6e4030b, // 1.176397
    0x3ff306fe0a31b715, // 1.189207
    0x3ff33c08b26416ff, // 1.202157
    0x3ff371a7373aa9cb, // 1.215247
    0x3ff3a7db34e59ff7, // 1.228481
    0x3ff3dea64c123422, // 1.241858
    0x3ff4160a21f72e2a, // 1.255381
    0x3ff44e086061892d, // 1.269051
    0x3ff486a2b5c13cd0, // 1.282870
    0x3ff4bfdad5362a27, // 1.296840
    0x3ff4f9b2769d2ca7, // 1.310961
    0x3ff5342b569d4f82, // 1.325237
    0x3ff56f4736b527da, // 1.339668
    0x3ff5ab07dd485429, // 1.354256
    0x3ff5e76f15ad2148, // 1.369002
    0x3ff6247eb03a5585, // 1.383910
    0x3ff6623882552225, // 1.398980
    0x3ff6a09e667f3bcd, // 1.414214
    0x3ff6dfb23c651a2f, // 1.429613
    0x3ff71f75e8ec5f74, // 1.445181
    0x3ff75feb564267c9, // 1.460918
    0x3ff7a11473eb0187, // 1.476826
    0x3ff7e2f336cf4e62, // 1.492908
    0x3ff82589994cce13, // 1.509164
    0x3ff868d99b4492ed, // 1.525598
    0x3ff8ace5422aa0db, // 1.542211
    0x3ff8f1ae99157736, // 1.559004
    0x3ff93737b0cdc5e5, // 1.575981
    0x3ff97d829fde4e50, // 1.593142
    0x3ff9c49182a3f090, // 1.610490
    0x3ffa0c667b5de565, // 1.628027
    0x3ffa5503b23e255d, // 1.645755
    0x3ffa9e6b5579fdbf, // 1.663677
    0x3ffae89f995ad3ad, // 1.681793
    0x3ffb33a2b84f15fb, // 1.700106
    0x3ffb7f76f2fb5e47, // 1.718619
    0x3ffbcc1e904bc1d2, // 1.737334
    0x3ffc199bdd85529c, // 1.756252
    0x3ffc67f12e57d14b, // 1.775376
    0x3ffcb720dcef9069, // 1.794709
    0x3ffd072d4a07897c, // 1.814252
    0x3ffd5818dcfba487, // 1.834008
    0x3ffda9e603db3285, // 1.853979
    0x3ffdfc97337b9b5f, // 1.874168
    0x3ffe502ee78b3ff6, // 1.894576
    0x3ffea4afa2a490da, // 1.915207
    0x3ffefa1bee615a27, // 1.936062
    0x3fff50765b6e4540, // 1.957144
    0x3fffa7c1819e90d8, // 1.978456
};

// 1 / ln(2) * (1 << EXPTAB_SCALE) == 1.4426950408889634073599246810019 * (1 << EXPTAB_SCALE)
ಸ್ಥಿರ ಧ್ರುವ float64_t exp_prescale = float64_t::fromRaw(0x3ff71547652b82fe) * float64_t(1 << EXPTAB_SCALE);
ಸ್ಥಿರ ಧ್ರುವ float64_t exp_postscale = float64_t::one()/float64_t(1 << EXPTAB_SCALE);
ಸ್ಥಿರ ಧ್ರುವ float64_t exp_max_val(3000*(1 << EXPTAB_SCALE)); // log10(DBL_MAX) < 3000

ಸ್ಥಿರ float32_t f32_exp( float32_t x)
{
    //special cases
    ಯದಿ(x.isNaN()) ಪ್ರತ್ಯಾಗಮ float32_t::nan();
    ಯದಿ(x.isInf()) ಪ್ರತ್ಯಾಗಮ (x == float32_t::inf()) ? x : float32_t::zero();

    ಸ್ಥಿರ ಧ್ರುವ float64_t
        A4 = float64_t::one() / EXPPOLY_32F_A0,
        A3 = float64_t::fromRaw(0x3fe62e42fef9277b) / EXPPOLY_32F_A0, // .6931471805521448196800669615864773144641 / EXPPOLY_32F_A0,
        A2 = float64_t::fromRaw(0x3fcebfbe081585e7) / EXPPOLY_32F_A0, // .2402265109513301490103372422686535526573 / EXPPOLY_32F_A0,
        A1 = float64_t::fromRaw(0x3fac6af0d93cf576) / EXPPOLY_32F_A0; // .5550339366753125211915322047004666939128e-1 / EXPPOLY_32F_A0;

    float64_t x0;
    ಯದಿ(expF32UI(x.v) > 127 + 10)
        x0 = signF32UI(x.v) ? -exp_max_val : exp_max_val;
    ಅನ್ಯಥಾ
        x0 = f32_to_f64(x) * exp_prescale;

    ಪೂರ್ಣಾಙ್ಕ val0 = f64_to_i32(x0, round_near_even, ಅಸತ್ಯ);
    ಪೂರ್ಣಾಙ್ಕ t = (val0 >> EXPTAB_SCALE) + 1023;
    t = t < 0 ? 0 : (t > 2047 ? 2047 : t);
    float64_t buf; buf.v = packToF64UI(0, t, 0);

    x0 = (x0 - f64_roundToInt(x0, round_near_even, ಅಸತ್ಯ)) * exp_postscale;

    ಪ್ರತ್ಯಾಗಮ (buf * EXPPOLY_32F_A0 * float64_t::fromRaw(expTab[val0 & EXPTAB_MASK]) * ((((x0 + A1)*x0 + A2)*x0 + A3)*x0 + A4));
}

ಸ್ಥಿರ float64_t f64_exp(float64_t x)
{
    //special cases
    ಯದಿ(x.isNaN()) ಪ್ರತ್ಯಾಗಮ float64_t::nan();
    ಯದಿ(x.isInf()) ಪ್ರತ್ಯಾಗಮ (x == float64_t::inf()) ? x : float64_t::zero();

    ಸ್ಥಿರ ಧ್ರುವ float64_t
        A5 = float64_t::one() / EXPPOLY_32F_A0,
        A4 = float64_t::fromRaw(0x3fe62e42fefa39f1) / EXPPOLY_32F_A0, // .69314718055994546743029643825322 / EXPPOLY_32F_A0
        A3 = float64_t::fromRaw(0x3fcebfbdff82a45a) / EXPPOLY_32F_A0, // .24022650695886477918181338054308 / EXPPOLY_32F_A0
        A2 = float64_t::fromRaw(0x3fac6b08d81fec75) / EXPPOLY_32F_A0, // .55504108793649567998466049042729e-1 / EXPPOLY_32F_A0
        A1 = float64_t::fromRaw(0x3f83b2a72b4f3cd3) / EXPPOLY_32F_A0, // .96180973140732918010002372686186e-2 / EXPPOLY_32F_A0
        A0 = float64_t::fromRaw(0x3f55e7aa1566c2a4) / EXPPOLY_32F_A0; // .13369713757180123244806654839424e-2 / EXPPOLY_32F_A0

    float64_t x0;
    ಯದಿ(expF64UI(x.v) > 1023 + 10)
        x0 = signF64UI(x.v) ? -exp_max_val : exp_max_val;
    ಅನ್ಯಥಾ
        x0 = x * exp_prescale;

    ಪೂರ್ಣಾಙ್ಕ val0 = cvRound(x0);
    ಪೂರ್ಣಾಙ್ಕ t = (val0 >> EXPTAB_SCALE) + 1023;
    t = t < 0 ? 0 : (t > 2047 ? 2047 : t);
    float64_t buf; buf.v = packToF64UI(0, t, 0);

    x0 = (x0 - f64_roundToInt(x0, round_near_even, ಅಸತ್ಯ)) * exp_postscale;

    ಪ್ರತ್ಯಾಗಮ buf * EXPPOLY_32F_A0 * float64_t::fromRaw(expTab[val0 & EXPTAB_MASK]) * (((((A0*x0 + A1)*x0 + A2)*x0 + A3)*x0 + A4)*x0 + A5);
}

#undef EXPTAB_SCALE
#undef EXPTAB_MASK
#undef EXPPOLY_32F_A0

/////////////////////////////////////////// LOG ///////////////////////////////////////

#define LOGTAB_SCALE    8

ಸ್ಥಿರ ಧ್ರುವ uint64_t CV_DECL_ALIGNED(16) icvLogTab[] = {
    0, 0x3ff0000000000000, // 0.000000, 1.000000
    0x3f6ff00aa2b10bc0, 0x3fefe01fe01fe020, // 0.003899, 0.996109
    0x3f7fe02a6b106788, 0x3fefc07f01fc07f0, // 0.007782, 0.992248
    0x3f87dc475f810a76, 0x3fefa11caa01fa12, // 0.011651, 0.988417
    0x3f8fc0a8b0fc03e3, 0x3fef81f81f81f820, // 0.015504, 0.984615
    0x3f93cea44346a574, 0x3fef6310aca0dbb5, // 0.019343, 0.980843
    0x3f97b91b07d5b11a, 0x3fef44659e4a4271, // 0.023167, 0.977099
    0x3f9b9fc027af9197, 0x3fef25f644230ab5, // 0.026977, 0.973384
    0x3f9f829b0e783300, 0x3fef07c1f07c1f08, // 0.030772, 0.969697
    0x3fa1b0d98923d97f, 0x3feee9c7f8458e02, // 0.034552, 0.966038
    0x3fa39e87b9febd5f, 0x3feecc07b301ecc0, // 0.038319, 0.962406
    0x3fa58a5bafc8e4d4, 0x3feeae807aba01eb, // 0.042071, 0.958801
    0x3fa77458f632dcfc, 0x3fee9131abf0b767, // 0.045810, 0.955224
    0x3fa95c830ec8e3eb, 0x3fee741aa59750e4, // 0.049534, 0.951673
    0x3fab42dd711971be, 0x3fee573ac901e574, // 0.053245, 0.948148
    0x3fad276b8adb0b52, 0x3fee3a9179dc1a73, // 0.056941, 0.944649
    0x3faf0a30c01162a6, 0x3fee1e1e1e1e1e1e, // 0.060625, 0.941176
    0x3fb075983598e471, 0x3fee01e01e01e01e, // 0.064294, 0.937729
    0x3fb16536eea37ae0, 0x3fede5d6e3f8868a, // 0.067951, 0.934307
    0x3fb253f62f0a1416, 0x3fedca01dca01dca, // 0.071594, 0.930909
    0x3fb341d7961bd1d0, 0x3fedae6076b981db, // 0.075223, 0.927536
    0x3fb42edcbea646f0, 0x3fed92f2231e7f8a, // 0.078840, 0.924188
    0x3fb51b073f06183f, 0x3fed77b654b82c34, // 0.082444, 0.920863
    0x3fb60658a93750c3, 0x3fed5cac807572b2, // 0.086034, 0.917563
    0x3fb6f0d28ae56b4b, 0x3fed41d41d41d41d, // 0.089612, 0.914286
    0x3fb7da766d7b12cc, 0x3fed272ca3fc5b1a, // 0.093177, 0.911032
    0x3fb8c345d6319b20, 0x3fed0cb58f6ec074, // 0.096730, 0.907801
    0x3fb9ab42462033ac, 0x3fecf26e5c44bfc6, // 0.100269, 0.904594
    0x3fba926d3a4ad563, 0x3fecd85689039b0b, // 0.103797, 0.901408
    0x3fbb78c82bb0eda1, 0x3fecbe6d9601cbe7, // 0.107312, 0.898246
    0x3fbc5e548f5bc743, 0x3feca4b3055ee191, // 0.110814, 0.895105
    0x3fbd4313d66cb35d, 0x3fec8b265afb8a42, // 0.114305, 0.891986
    0x3fbe27076e2af2e5, 0x3fec71c71c71c71c, // 0.117783, 0.888889
    0x3fbf0a30c01162a6, 0x3fec5894d10d4986, // 0.121249, 0.885813
    0x3fbfec9131dbeaba, 0x3fec3f8f01c3f8f0, // 0.124703, 0.882759
    0x3fc0671512ca596e, 0x3fec26b5392ea01c, // 0.128146, 0.879725
    0x3fc0d77e7cd08e59, 0x3fec0e070381c0e0, // 0.131576, 0.876712
    0x3fc14785846742ac, 0x3febf583ee868d8b, // 0.134995, 0.873720
    0x3fc1b72ad52f67a0, 0x3febdd2b899406f7, // 0.138402, 0.870748
    0x3fc2266f190a5acb, 0x3febc4fd65883e7b, // 0.141798, 0.867797
    0x3fc29552f81ff523, 0x3febacf914c1bad0, // 0.145182, 0.864865
    0x3fc303d718e47fd2, 0x3feb951e2b18ff23, // 0.148555, 0.861953
    0x3fc371fc201e8f74, 0x3feb7d6c3dda338b, // 0.151916, 0.859060
    0x3fc3dfc2b0ecc629, 0x3feb65e2e3beee05, // 0.155266, 0.856187
    0x3fc44d2b6ccb7d1e, 0x3feb4e81b4e81b4f, // 0.158605, 0.853333
    0x3fc4ba36f39a55e5, 0x3feb37484ad806ce, // 0.161933, 0.850498
    0x3fc526e5e3a1b437, 0x3feb2036406c80d9, // 0.165250, 0.847682
    0x3fc59338d9982085, 0x3feb094b31d922a4, // 0.168555, 0.844884
    0x3fc5ff3070a793d3, 0x3feaf286bca1af28, // 0.171850, 0.842105
    0x3fc66acd4272ad50, 0x3feadbe87f94905e, // 0.175134, 0.839344
    0x3fc6d60fe719d21c, 0x3feac5701ac5701b, // 0.178408, 0.836601
    0x3fc740f8f54037a4, 0x3feaaf1d2f87ebfd, // 0.181670, 0.833876
    0x3fc7ab890210d909, 0x3fea98ef606a63be, // 0.184922, 0.831169
    0x3fc815c0a14357ea, 0x3fea82e65130e159, // 0.188164, 0.828479
    0x3fc87fa06520c910, 0x3fea6d01a6d01a6d, // 0.191395, 0.825806
    0x3fc8e928de886d40, 0x3fea574107688a4a, // 0.194615, 0.823151
    0x3fc9525a9cf456b4, 0x3fea41a41a41a41a, // 0.197826, 0.820513
    0x3fc9bb362e7dfb83, 0x3fea2c2a87c51ca0, // 0.201026, 0.817891
    0x3fca23bc1fe2b563, 0x3fea16d3f97a4b02, // 0.204216, 0.815287
    0x3fca8becfc882f18, 0x3fea01a01a01a01a, // 0.207395, 0.812698
    0x3fcaf3c94e80bff2, 0x3fe9ec8e951033d9, // 0.210565, 0.810127
    0x3fcb5b519e8fb5a4, 0x3fe9d79f176b682d, // 0.213724, 0.807571
    0x3fcbc286742d8cd6, 0x3fe9c2d14ee4a102, // 0.216874, 0.805031
    0x3fcc2968558c18c0, 0x3fe9ae24ea5510da, // 0.220014, 0.802508
    0x3fcc8ff7c79a9a21, 0x3fe999999999999a, // 0.223144, 0.800000
    0x3fccf6354e09c5dc, 0x3fe9852f0d8ec0ff, // 0.226264, 0.797508
    0x3fcd5c216b4fbb91, 0x3fe970e4f80cb872, // 0.229374, 0.795031
    0x3fcdc1bca0abec7d, 0x3fe95cbb0be377ae, // 0.232475, 0.792570
    0x3fce27076e2af2e5, 0x3fe948b0fcd6e9e0, // 0.235566, 0.790123
    0x3fce8c0252aa5a5f, 0x3fe934c67f9b2ce6, // 0.238648, 0.787692
    0x3fcef0adcbdc5936, 0x3fe920fb49d0e229, // 0.241720, 0.785276
    0x3fcf550a564b7b37, 0x3fe90d4f120190d5, // 0.244783, 0.782875
    0x3fcfb9186d5e3e2a, 0x3fe8f9c18f9c18fa, // 0.247836, 0.780488
    0x3fd00e6c45ad501c, 0x3fe8e6527af1373f, // 0.250880, 0.778116
    0x3fd0402594b4d040, 0x3fe8d3018d3018d3, // 0.253915, 0.775758
    0x3fd071b85fcd590d, 0x3fe8bfce8062ff3a, // 0.256941, 0.773414
    0x3fd0a324e27390e3, 0x3fe8acb90f6bf3aa, // 0.259958, 0.771084
    0x3fd0d46b579ab74b, 0x3fe899c0f601899c, // 0.262965, 0.768769
    0x3fd1058bf9ae4ad5, 0x3fe886e5f0abb04a, // 0.265964, 0.766467
    0x3fd136870293a8b0, 0x3fe87427bcc092b9, // 0.268953, 0.764179
    0x3fd1675cababa60e, 0x3fe8618618618618, // 0.271934, 0.761905
    0x3fd1980d2dd4236f, 0x3fe84f00c2780614, // 0.274905, 0.759644
    0x3fd1c898c16999fa, 0x3fe83c977ab2bedd, // 0.277868, 0.757396
    0x3fd1f8ff9e48a2f2, 0x3fe82a4a0182a4a0, // 0.280823, 0.755162
    0x3fd22941fbcf7965, 0x3fe8181818181818, // 0.283768, 0.752941
    0x3fd2596010df7639, 0x3fe8060180601806, // 0.286705, 0.750733
    0x3fd2895a13de86a3, 0x3fe7f405fd017f40, // 0.289633, 0.748538
    0x3fd2b9303ab89d24, 0x3fe7e225515a4f1d, // 0.292553, 0.746356
    0x3fd2e8e2bae11d30, 0x3fe7d05f417d05f4, // 0.295464, 0.744186
    0x3fd31871c9544184, 0x3fe7beb3922e017c, // 0.298367, 0.742029
    0x3fd347dd9a987d54, 0x3fe7ad2208e0ecc3, // 0.301261, 0.739884
    0x3fd3772662bfd85a, 0x3fe79baa6bb6398b, // 0.304147, 0.737752
    0x3fd3a64c556945e9, 0x3fe78a4c8178a4c8, // 0.307025, 0.735632
    0x3fd3d54fa5c1f70f, 0x3fe77908119ac60d, // 0.309894, 0.733524
    0x3fd404308686a7e3, 0x3fe767dce434a9b1, // 0.312756, 0.731429
    0x3fd432ef2a04e813, 0x3fe756cac201756d, // 0.315609, 0.729345
    0x3fd4618bc21c5ec2, 0x3fe745d1745d1746, // 0.318454, 0.727273
    0x3fd49006804009d0, 0x3fe734f0c541fe8d, // 0.321291, 0.725212
    0x3fd4be5f957778a0, 0x3fe724287f46debc, // 0.324119, 0.723164
    0x3fd4ec9732600269, 0x3fe713786d9c7c09, // 0.326940, 0.721127
    0x3fd51aad872df82d, 0x3fe702e05c0b8170, // 0.329753, 0.719101
    0x3fd548a2c3add262, 0x3fe6f26016f26017, // 0.332558, 0.717087
    0x3fd5767717455a6c, 0x3fe6e1f76b4337c7, // 0.335356, 0.715084
    0x3fd5a42ab0f4cfe1, 0x3fe6d1a62681c861, // 0.338145, 0.713092
    0x3fd5d1bdbf5809ca, 0x3fe6c16c16c16c17, // 0.340927, 0.711111
    0x3fd5ff3070a793d3, 0x3fe6b1490aa31a3d, // 0.343701, 0.709141
    0x3fd62c82f2b9c795, 0x3fe6a13cd1537290, // 0.346467, 0.707182
    0x3fd659b57303e1f2, 0x3fe691473a88d0c0, // 0.349225, 0.705234
    0x3fd686c81e9b14ae, 0x3fe6816816816817, // 0.351976, 0.703297
    0x3fd6b3bb2235943d, 0x3fe6719f3601671a, // 0.354720, 0.701370
    0x3fd6e08eaa2ba1e3, 0x3fe661ec6a5122f9, // 0.357456, 0.699454
    0x3fd70d42e2789235, 0x3fe6524f853b4aa3, // 0.360184, 0.697548
    0x3fd739d7f6bbd006, 0x3fe642c8590b2164, // 0.362905, 0.695652
    0x3fd7664e1239dbce, 0x3fe63356b88ac0de, // 0.365619, 0.693767
    0x3fd792a55fdd47a2, 0x3fe623fa77016240, // 0.368326, 0.691892
    0x3fd7bede0a37afbf, 0x3fe614b36831ae94, // 0.371025, 0.690027
    0x3fd7eaf83b82afc3, 0x3fe6058160581606, // 0.373716, 0.688172
    0x3fd816f41da0d495, 0x3fe5f66434292dfc, // 0.376401, 0.686327
    0x3fd842d1da1e8b17, 0x3fe5e75bb8d015e7, // 0.379078, 0.684492
    0x3fd86e919a330ba0, 0x3fe5d867c3ece2a5, // 0.381749, 0.682667
    0x3fd89a3386c1425a, 0x3fe5c9882b931057, // 0.384412, 0.680851
    0x3fd8c5b7c858b48a, 0x3fe5babcc647fa91, // 0.387068, 0.679045
    0x3fd8f11e873662c7, 0x3fe5ac056b015ac0, // 0.389717, 0.677249
    0x3fd91c67eb45a83d, 0x3fe59d61f123ccaa, // 0.392359, 0.675462
    0x3fd947941c2116fa, 0x3fe58ed2308158ed, // 0.394994, 0.673684
    0x3fd972a341135158, 0x3fe5805601580560, // 0.397622, 0.671916
    0x3fd99d958117e08a, 0x3fe571ed3c506b3a, // 0.400243, 0.670157
    0x3fd9c86b02dc0862, 0x3fe56397ba7c52e2, // 0.402858, 0.668407
    0x3fd9f323ecbf984b, 0x3fe5555555555555, // 0.405465, 0.666667
    0x3fda1dc064d5b995, 0x3fe54725e6bb82fe, // 0.408066, 0.664935
    0x3fda484090e5bb0a, 0x3fe5390948f40feb, // 0.410660, 0.663212
    0x3fda72a4966bd9ea, 0x3fe52aff56a8054b, // 0.413247, 0.661499
    0x3fda9cec9a9a0849, 0x3fe51d07eae2f815, // 0.415828, 0.659794
    0x3fdac718c258b0e4, 0x3fe50f22e111c4c5, // 0.418402, 0.658098
    0x3fdaf1293247786b, 0x3fe5015015015015, // 0.420969, 0.656410
    0x3fdb1b1e0ebdfc5b, 0x3fe4f38f62dd4c9b, // 0.423530, 0.654731
    0x3fdb44f77bcc8f62, 0x3fe4e5e0a72f0539, // 0.426084, 0.653061
    0x3fdb6eb59d3cf35d, 0x3fe4d843bedc2c4c, // 0.428632, 0.651399
    0x3fdb9858969310fb, 0x3fe4cab88725af6e, // 0.431173, 0.649746
    0x3fdbc1e08b0dad0a, 0x3fe4bd3edda68fe1, // 0.433708, 0.648101
    0x3fdbeb4d9da71b7b, 0x3fe4afd6a052bf5b, // 0.436237, 0.646465
    0x3fdc149ff115f026, 0x3fe4a27fad76014a, // 0.438759, 0.644836
    0x3fdc3dd7a7cdad4d, 0x3fe49539e3b2d067, // 0.441275, 0.643216
    0x3fdc66f4e3ff6ff7, 0x3fe4880522014880, // 0.443784, 0.641604
    0x3fdc8ff7c79a9a21, 0x3fe47ae147ae147b, // 0.446287, 0.640000
    0x3fdcb8e0744d7ac9, 0x3fe46dce34596066, // 0.448784, 0.638404
    0x3fdce1af0b85f3eb, 0x3fe460cbc7f5cf9a, // 0.451275, 0.636816
    0x3fdd0a63ae721e64, 0x3fe453d9e2c776ca, // 0.453759, 0.635236
    0x3fdd32fe7e00ebd5, 0x3fe446f86562d9fb, // 0.456237, 0.633663
    0x3fdd5b7f9ae2c683, 0x3fe43a2730abee4d, // 0.458710, 0.632099
    0x3fdd83e7258a2f3e, 0x3fe42d6625d51f87, // 0.461176, 0.630542
    0x3fddac353e2c5954, 0x3fe420b5265e5951, // 0.463636, 0.628993
    0x3fddd46a04c1c4a0, 0x3fe4141414141414, // 0.466090, 0.627451
    0x3fddfc859906d5b5, 0x3fe40782d10e6566, // 0.468538, 0.625917
    0x3fde24881a7c6c26, 0x3fe3fb013fb013fb, // 0.470980, 0.624390
    0x3fde4c71a8687704, 0x3fe3ee8f42a5af07, // 0.473416, 0.622871
    0x3fde744261d68787, 0x3fe3e22cbce4a902, // 0.475846, 0.621359
    0x3fde9bfa659861f5, 0x3fe3d5d991aa75c6, // 0.478270, 0.619855
    0x3fdec399d2468cc0, 0x3fe3c995a47babe7, // 0.480689, 0.618357
    0x3fdeeb20c640ddf4, 0x3fe3bd60d9232955, // 0.483101, 0.616867
    0x3fdf128f5faf06ec, 0x3fe3b13b13b13b14, // 0.485508, 0.615385
    0x3fdf39e5bc811e5b, 0x3fe3a524387ac822, // 0.487909, 0.613909
    0x3fdf6123fa7028ac, 0x3fe3991c2c187f63, // 0.490304, 0.612440
    0x3fdf884a36fe9ec2, 0x3fe38d22d366088e, // 0.492693, 0.610979
    0x3fdfaf588f78f31e, 0x3fe3813813813814, // 0.495077, 0.609524
    0x3fdfd64f20f61571, 0x3fe3755bd1c945ee, // 0.497455, 0.608076
    0x3fdffd2e0857f498, 0x3fe3698df3de0748, // 0.499828, 0.606635
    0x3fe011fab125ff8a, 0x3fe35dce5f9f2af8, // 0.502195, 0.605201
    0x3fe02552a5a5d0fe, 0x3fe3521cfb2b78c1, // 0.504556, 0.603774
    0x3fe0389eefce633b, 0x3fe34679ace01346, // 0.506912, 0.602353
    0x3fe04bdf9da926d2, 0x3fe33ae45b57bcb2, // 0.509262, 0.600939
    0x3fe05f14bd26459c, 0x3fe32f5ced6a1dfa, // 0.511607, 0.599532
    0x3fe0723e5c1cdf40, 0x3fe323e34a2b10bf, // 0.513946, 0.598131
    0x3fe0855c884b450e, 0x3fe3187758e9ebb6, // 0.516279, 0.596737
    0x3fe0986f4f573520, 0x3fe30d190130d190, // 0.518608, 0.595349
    0x3fe0ab76bece14d1, 0x3fe301c82ac40260, // 0.520931, 0.593968
    0x3fe0be72e4252a82, 0x3fe2f684bda12f68, // 0.523248, 0.592593
    0x3fe0d163ccb9d6b7, 0x3fe2eb4ea1fed14b, // 0.525560, 0.591224
    0x3fe0e44985d1cc8b, 0x3fe2e025c04b8097, // 0.527867, 0.589862
    0x3fe0f7241c9b497d, 0x3fe2d50a012d50a0, // 0.530169, 0.588506
    0x3fe109f39e2d4c96, 0x3fe2c9fb4d812ca0, // 0.532465, 0.587156
    0x3fe11cb81787ccf8, 0x3fe2bef98e5a3711, // 0.534756, 0.585812
    0x3fe12f719593efbc, 0x3fe2b404ad012b40, // 0.537041, 0.584475
    0x3fe1422025243d44, 0x3fe2a91c92f3c105, // 0.539322, 0.583144
    0x3fe154c3d2f4d5e9, 0x3fe29e4129e4129e, // 0.541597, 0.581818
    0x3fe1675cababa60e, 0x3fe293725bb804a5, // 0.543867, 0.580499
    0x3fe179eabbd899a0, 0x3fe288b01288b013, // 0.546132, 0.579186
    0x3fe18c6e0ff5cf06, 0x3fe27dfa38a1ce4d, // 0.548392, 0.577878
    0x3fe19ee6b467c96e, 0x3fe27350b8812735, // 0.550647, 0.576577
    0x3fe1b154b57da29e, 0x3fe268b37cd60127, // 0.552897, 0.575281
    0x3fe1c3b81f713c24, 0x3fe25e22708092f1, // 0.555142, 0.573991
    0x3fe1d610fe677003, 0x3fe2539d7e9177b2, // 0.557381, 0.572707
    0x3fe1e85f5e7040d0, 0x3fe2492492492492, // 0.559616, 0.571429
    0x3fe1faa34b87094c, 0x3fe23eb79717605b, // 0.561845, 0.570156
    0x3fe20cdcd192ab6d, 0x3fe23456789abcdf, // 0.564070, 0.568889
    0x3fe21f0bfc65beeb, 0x3fe22a0122a0122a, // 0.566290, 0.567627
    0x3fe23130d7bebf42, 0x3fe21fb78121fb78, // 0.568505, 0.566372
    0x3fe2434b6f483933, 0x3fe21579804855e6, // 0.570715, 0.565121
    0x3fe2555bce98f7cb, 0x3fe20b470c67c0d9, // 0.572920, 0.563877
    0x3fe26762013430df, 0x3fe2012012012012, // 0.575120, 0.562637
    0x3fe2795e1289b11a, 0x3fe1f7047dc11f70, // 0.577315, 0.561404
    0x3fe28b500df60782, 0x3fe1ecf43c7fb84c, // 0.579506, 0.560175
    0x3fe29d37fec2b08a, 0x3fe1e2ef3b3fb874, // 0.581692, 0.558952
    0x3fe2af15f02640ad, 0x3fe1d8f5672e4abd, // 0.583873, 0.557734
    0x3fe2c0e9ed448e8b, 0x3fe1cf06ada2811d, // 0.586049, 0.556522
    0x3fe2d2b4012edc9d, 0x3fe1c522fc1ce059, // 0.588221, 0.555315
    0x3fe2e47436e40268, 0x3fe1bb4a4046ed29, // 0.590387, 0.554113
    0x3fe2f62a99509546, 0x3fe1b17c67f2bae3, // 0.592550, 0.552916
    0x3fe307d7334f10be, 0x3fe1a7b9611a7b96, // 0.594707, 0.551724
    0x3fe3197a0fa7fe6a, 0x3fe19e0119e0119e, // 0.596860, 0.550538
    0x3fe32b1339121d71, 0x3fe19453808ca29c, // 0.599008, 0.549356
    0x3fe33ca2ba328994, 0x3fe18ab083902bdb, // 0.601152, 0.548180
    0x3fe34e289d9ce1d3, 0x3fe1811811811812, // 0.603291, 0.547009
    0x3fe35fa4edd36ea0, 0x3fe1778a191bd684, // 0.605425, 0.545842
    0x3fe37117b54747b5, 0x3fe16e0689427379, // 0.607555, 0.544681
    0x3fe38280fe58797e, 0x3fe1648d50fc3201, // 0.609681, 0.543524
    0x3fe393e0d3562a19, 0x3fe15b1e5f75270d, // 0.611802, 0.542373
    0x3fe3a5373e7ebdf9, 0x3fe151b9a3fdd5c9, // 0.613918, 0.541226
    0x3fe3b68449fffc22, 0x3fe1485f0e0acd3b, // 0.616030, 0.540084
    0x3fe3c7c7fff73205, 0x3fe13f0e8d344724, // 0.618137, 0.538947
    0x3fe3d9026a7156fa, 0x3fe135c81135c811, // 0.620240, 0.537815
    0x3fe3ea33936b2f5b, 0x3fe12c8b89edc0ac, // 0.622339, 0.536688
    0x3fe3fb5b84d16f42, 0x3fe12358e75d3033, // 0.624433, 0.535565
    0x3fe40c7a4880dce9, 0x3fe11a3019a74826, // 0.626523, 0.534447
    0x3fe41d8fe84672ae, 0x3fe1111111111111, // 0.628609, 0.533333
    0x3fe42e9c6ddf80bf, 0x3fe107fbbe011080, // 0.630690, 0.532225
    0x3fe43f9fe2f9ce67, 0x3fe0fef010fef011, // 0.632767, 0.531120
    0x3fe4509a5133bb0a, 0x3fe0f5edfab325a2, // 0.634839, 0.530021
    0x3fe4618bc21c5ec2, 0x3fe0ecf56be69c90, // 0.636907, 0.528926
    0x3fe472743f33aaad, 0x3fe0e40655826011, // 0.638971, 0.527835
    0x3fe48353d1ea88df, 0x3fe0db20a88f4696, // 0.641031, 0.526749
    0x3fe4942a83a2fc07, 0x3fe0d24456359e3a, // 0.643087, 0.525667
    0x3fe4a4f85db03ebb, 0x3fe0c9714fbcda3b, // 0.645138, 0.524590
    0x3fe4b5bd6956e273, 0x3fe0c0a7868b4171, // 0.647185, 0.523517
    0x3fe4c679afccee39, 0x3fe0b7e6ec259dc8, // 0.649228, 0.522449
    0x3fe4d72d3a39fd00, 0x3fe0af2f722eecb5, // 0.651267, 0.521385
    0x3fe4e7d811b75bb0, 0x3fe0a6810a6810a7, // 0.653301, 0.520325
    0x3fe4f87a3f5026e8, 0x3fe09ddba6af8360, // 0.655332, 0.519270
    0x3fe50913cc01686b, 0x3fe0953f39010954, // 0.657358, 0.518219
    0x3fe519a4c0ba3446, 0x3fe08cabb37565e2, // 0.659380, 0.517172
    0x3fe52a2d265bc5aa, 0x3fe0842108421084, // 0.661398, 0.516129
    0x3fe53aad05b99b7c, 0x3fe07b9f29b8eae2, // 0.663413, 0.515091
    0x3fe54b2467999497, 0x3fe073260a47f7c6, // 0.665423, 0.514056
    0x3fe55b9354b40bcd, 0x3fe06ab59c7912fb, // 0.667429, 0.513026
    0x3fe56bf9d5b3f399, 0x3fe0624dd2f1a9fc, // 0.669431, 0.512000
    0x3fe57c57f336f190, 0x3fe059eea0727586, // 0.671429, 0.510978
    0x3fe58cadb5cd7989, 0x3fe05197f7d73404, // 0.673423, 0.509960
    0x3fe59cfb25fae87d, 0x3fe04949cc1664c5, // 0.675413, 0.508946
    0x3fe5ad404c359f2c, 0x3fe0410410410410, // 0.677399, 0.507937
    0x3fe5bd7d30e71c73, 0x3fe038c6b78247fc, // 0.679381, 0.506931
    0x3fe5cdb1dc6c1764, 0x3fe03091b51f5e1a, // 0.681359, 0.505929
    0x3fe5ddde57149923, 0x3fe02864fc7729e9, // 0.683334, 0.504931
    0x3fe5ee02a9241675, 0x3fe0204081020408, // 0.685304, 0.503937
    0x3fe5fe1edad18918, 0x3fe0182436517a37, // 0.687271, 0.502947
    0x3fe60e32f44788d8, 0x3fe0101010101010, // 0.689233, 0.501961
    0x3fe62e42fefa39ef, 0x3fe0000000000000, // 0.693147, 0.500000
};

// 0.69314718055994530941723212145818
ಸ್ಥಿರ ಧ್ರುವ float64_t ln_2 = float64_t::fromRaw(0x3fe62e42fefa39ef);

ಸ್ಥಿರ float32_t f32_log(float32_t x)
{
    //special cases
    ಯದಿ(x.isNaN() || x < float32_t::zero()) ಪ್ರತ್ಯಾಗಮ float32_t::nan();
    ಯದಿ(x == float32_t::zero()) ಪ್ರತ್ಯಾಗಮ -float32_t::inf();

    //first 8 bits of mantissa
    ಪೂರ್ಣಾಙ್ಕ h0 = (x.v >> (23 - LOGTAB_SCALE)) & ((1 << LOGTAB_SCALE) - 1);
    //buf == 0.00000000_the_rest_mantissa_bits
    float64_t buf; buf.v = packToF64UI(0, 1023, ((uint64_t)x.v << 29) & ((1LL << (52 - LOGTAB_SCALE)) - 1));
    buf -= float64_t::one();

    float64_t tab0 = float64_t::fromRaw(icvLogTab[2*h0]);
    float64_t tab1 = float64_t::fromRaw(icvLogTab[2*h0+1]);

    float64_t x0 = buf * tab1;
    //if last elements of icvLogTab
    ಯದಿ(h0 == 255) x0 += float64_t(-float64_t::one() / float64_t(512));

    float64_t y0 = ln_2 * float64_t(expF32UI(x.v) - 127) + tab0 + x0*x0*x0/float64_t(3) - x0*x0/float64_t(2) + x0;

    ಪ್ರತ್ಯಾಗಮ y0;
}

ಸ್ಥಿರ float64_t f64_log(float64_t x)
{
    //special cases
    ಯದಿ(x.isNaN() || x < float64_t::zero()) ಪ್ರತ್ಯಾಗಮ float64_t::nan();
    ಯದಿ(x == float64_t::zero()) ಪ್ರತ್ಯಾಗಮ -float64_t::inf();

    ಸ್ಥಿರ ಧ್ರುವ float64_t
    A7(1),
    A6(-float64_t::one() / float64_t(2)),
    A5( float64_t::one() / float64_t(3)),
    A4(-float64_t::one() / float64_t(4)),
    A3( float64_t::one() / float64_t(5)),
    A2(-float64_t::one() / float64_t(6)),
    A1( float64_t::one() / float64_t(7)),
    A0(-float64_t::one() / float64_t(8));

    //first 8 bits of mantissa
    ಪೂರ್ಣಾಙ್ಕ h0 = (x.v >> (52 - LOGTAB_SCALE)) & ((1 << LOGTAB_SCALE) - 1);
    //buf == 0.00000000_the_rest_mantissa_bits
    float64_t buf; buf.v = packToF64UI(0, 1023, x.v & ((1LL << (52 - LOGTAB_SCALE)) - 1));
    buf -= float64_t::one();

    float64_t tab0 = float64_t::fromRaw(icvLogTab[2*h0]);
    float64_t tab1 = float64_t::fromRaw(icvLogTab[2*h0 + 1]);

    float64_t x0 = buf * tab1;
    //if last elements of icvLogTab
    ಯದಿ(h0 == 255) x0 += float64_t(-float64_t::one()/float64_t(512));
    float64_t xq = x0*x0;

    ಪ್ರತ್ಯಾಗಮ ln_2 * float64_t( expF64UI(x.v) - 1023) + tab0 + (((A0*xq + A2)*xq + A4)*xq + A6)*xq +
           (((A1*xq + A3)*xq + A5)*xq + A7)*x0;
}

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
ಸ್ಥಿರ float32_t f32_cbrt(float32_t x)
{
    //special cases
    ಯದಿ (x.isNaN()) ಪ್ರತ್ಯಾಗಮ float32_t::nan();
    ಯದಿ (x.isInf()) ಪ್ರತ್ಯಾಗಮ x;

    ಪೂರ್ಣಾಙ್ಕ s = signF32UI(x.v);
    ಪೂರ್ಣಾಙ್ಕ ex = expF32UI(x.v) - 127;
    ಪೂರ್ಣಾಙ್ಕ shx = ex % 3;
    shx -= shx >= 0 ? 3 : 0;
    ex = (ex - shx) / 3 - 1; /* exponent of cube root */
    float64_t fr; fr.v = packToF64UI(0, shx + 1023, ((uint64_t)fracF32UI(x.v)) << 29);

    /* 0.125 <= fr < 1.0 */
    /* Use quartic rational polynomial with error < 2^(-24) */
    ಧ್ರುವ float64_t A1 = float64_t::fromRaw(0x4046a09e6653ba70); //  45.2548339756803022511987494
    ಧ್ರುವ float64_t A2 = float64_t::fromRaw(0x406808f46c6116e0); // 192.2798368355061050458134625
    ಧ್ರುವ float64_t A3 = float64_t::fromRaw(0x405dca97439cae14); // 119.1654824285581628956914143
    ಧ್ರುವ float64_t A4 = float64_t::fromRaw(0x402add70d2827500); //  13.43250139086239872172837314
    ಧ್ರುವ float64_t A5 = float64_t::fromRaw(0x3fc4f15f83f55d2d); //   0.1636161226585754240958355063
    ಧ್ರುವ float64_t A6 = float64_t::fromRaw(0x402d9e20660edb21); //  14.80884093219134573786480845
    ಧ್ರುವ float64_t A7 = float64_t::fromRaw(0x4062ff15c0285815); // 151.9714051044435648658557668
    ಧ್ರುವ float64_t A8 = float64_t::fromRaw(0x406510d06a8112ce); // 168.5254414101568283957668343
    ಧ್ರುವ float64_t A9 = float64_t::fromRaw(0x4040fecbc9e2c375); //  33.9905941350215598754191872
    ಧ್ರುವ float64_t A10 = float64_t::one();

    fr = ((((A1 * fr + A2) * fr + A3) * fr + A4) * fr + A5)/
         ((((A6 * fr + A7) * fr + A8) * fr + A9) * fr + A10);
    /* fr *= 2^ex * sign */

    // checks for "+0" and "-0", reset sign bit
    float32_t y; y.v = ((x.v & ((1u << 31) - 1)) != 0) ? packToF32UI(s, ex + 127, (uint32_t)(fracF64UI(fr.v) >> 29)) : 0;
    ಪ್ರತ್ಯಾಗಮ y;
}

/// POW functions ///

ಸ್ಥಿರ float32_t f32_pow( float32_t x, float32_t y)
{
    ಸ್ಥಿರ ಧ್ರುವ float32_t zero = float32_t::zero(), one = float32_t::one(), inf = float32_t::inf(), nan = float32_t::nan();
    ತಾರ್ಕಿಕ xinf = x.isInf(), yinf = y.isInf(), xnan = x.isNaN(), ynan = y.isNaN();
    float32_t ax = abs(x);
    ತಾರ್ಕಿಕ useInf = (y > zero) == (ax > one);
    float32_t v;
    //special cases
    ಯದಿ(ynan) v = nan;
    ಅನ್ಯಥಾ ಯದಿ(yinf) v = (ax == one || xnan) ? nan : (useInf ? inf : zero);
    ಅನ್ಯಥಾ ಯದಿ(y == zero) v = one;
    ಅನ್ಯಥಾ ಯದಿ(y == one ) v = x;
    ಅನ್ಯಥಾ //here y is ok
    {
        ಯದಿ(xnan) v = nan;
        ಅನ್ಯಥಾ ಯದಿ(xinf) v = (y < zero) ? zero : inf;
        ಅನ್ಯಥಾ ಯದಿ(y == f32_roundToInt(y, round_near_even, ಅಸತ್ಯ)) v = f32_powi(x, f32_to_i32(y, round_near_even, ಅಸತ್ಯ));
        ಅನ್ಯಥಾ ಯದಿ(x  < zero) v = nan;
        // (0 ** 0) == 1
        ಅನ್ಯಥಾ ಯದಿ(x == zero) v = (y < zero) ? inf : (y == zero ? one : zero);
        // here x and y are ok
        ಅನ್ಯಥಾ v = f32_exp(y * f32_log(x));
    }

    ಪ್ರತ್ಯಾಗಮ v;
}

ಸ್ಥಿರ float64_t f64_pow( float64_t x, float64_t y)
{
    ಸ್ಥಿರ ಧ್ರುವ float64_t zero = float64_t::zero(), one = float64_t::one(), inf = float64_t::inf(), nan = float64_t::nan();
    ತಾರ್ಕಿಕ xinf = x.isInf(), yinf = y.isInf(), xnan = x.isNaN(), ynan = y.isNaN();
    float64_t ax = abs(x);
    ತಾರ್ಕಿಕ useInf = (y > zero) == (ax > one);
    float64_t v;
    //special cases
    ಯದಿ(ynan) v = nan;
    ಅನ್ಯಥಾ ಯದಿ(yinf) v = (ax == one || xnan) ? nan : (useInf ? inf : zero);
    ಅನ್ಯಥಾ ಯದಿ(y == zero) v = one;
    ಅನ್ಯಥಾ ಯದಿ(y == one ) v = x;
    ಅನ್ಯಥಾ //here y is ok
    {
        ಯದಿ(xnan) v = nan;
        ಅನ್ಯಥಾ ಯದಿ(xinf) v = (y < zero) ? zero : inf;
        ಅನ್ಯಥಾ ಯದಿ(y == f64_roundToInt(y, round_near_even, ಅಸತ್ಯ)) v = f64_powi(x, f64_to_i32(y, round_near_even, ಅಸತ್ಯ));
        ಅನ್ಯಥಾ ಯದಿ(x  < zero) v = nan;
        // (0 ** 0) == 1
        ಅನ್ಯಥಾ ಯದಿ(x == zero) v = (y < zero) ? inf : (y == zero ? one : zero);
        // here x and y are ok
        ಅನ್ಯಥಾ v = f64_exp(y * f64_log(x));
    }

    ಪ್ರತ್ಯಾಗಮ v;
}

// These functions are for internal use only

ಸ್ಥಿರ float32_t f32_powi( float32_t x, ಪೂರ್ಣಾಙ್ಕ y)
{
    float32_t v;
    //special case: (0 ** 0) == 1
    ಯದಿ(x == float32_t::zero())
        v = (y < 0) ? float32_t::inf() : (y == 0 ? float32_t::one() : float32_t::zero());
    // here x and y are ok
    ಅನ್ಯಥಾ
    {
        float32_t a = float32_t::one(), b = x;
        ಪೂರ್ಣಾಙ್ಕ p = std::abs(y);
        ಯದಿ( y < 0 )
            b = float32_t::one()/b;
        ಯಾವತ್( p > 1 )
        {
            ಯದಿ( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }
        v = a * b;
    }

    ಪ್ರತ್ಯಾಗಮ v;
}

ಸ್ಥಿರ float64_t f64_powi( float64_t x, ಪೂರ್ಣಾಙ್ಕ y)
{
    float64_t v;
    //special case: (0 ** 0) == 1
    ಯದಿ(x == float64_t::zero())
        v = (y < 0) ? float64_t::inf() : (y == 0 ? float64_t::one() : float64_t::zero());
    // here x and y are ok
    ಅನ್ಯಥಾ
    {
        float64_t a = float64_t::one(), b = x;
        ಪೂರ್ಣಾಙ್ಕ p = std::abs(y);
        ಯದಿ( y < 0 )
            b = float64_t::one()/b;
        ಯಾವತ್( p > 1 )
        {
            ಯದಿ( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }
        v = a * b;
    }

    ಪ್ರತ್ಯಾಗಮ v;
}

/*
 * sin and cos functions taken from fdlibm with original comments
 * (edited where needed)
 */

ಸ್ಥಿರ ಧ್ರುವ float64_t pi2   = float64_t::pi().setExp(2);
ಸ್ಥಿರ ಧ್ರುವ float64_t piby2 = float64_t::pi().setExp(0);
ಸ್ಥಿರ ಧ್ರುವ float64_t piby4 = float64_t::pi().setExp(-1);
ಸ್ಥಿರ ಧ್ರುವ float64_t half  = float64_t::one()/float64_t(2);
ಸ್ಥಿರ ಧ್ರುವ float64_t third = float64_t::one()/float64_t(3);

/* __kernel_sin( x, y, iy)
 * N.B. from OpenCV side: y and iy removed, simplified to polynomial
 *
 * kernel sin function on [-pi/4, pi/4], pi/4 ~ 0.7854
 * Input x is assumed to be bounded by ~pi/4 in magnitude.
 * Input y is the tail of x.
 * Input iy indicates whether y is 0. (if iy=0, y assume to be 0).
 *
 * Algorithm
 *  1. Since sin(-x) = -sin(x), we need only to consider positive x.
 *  2. if x < 2^-27 (hx<0x3e400000 0), return x with inexact if x!=0.
 *  3. sin(x) is approximated by a polynomial of degree 13 on
 *     [0,pi/4]
 *                       3            13
 *      sin(x) ~ x + S1*x + ... + S6*x
 *     where
 *
 *  |sin(x)         2     4     6     8     10     12  |     -58
 *  |----- - (1+S1*x +S2*x +S3*x +S4*x +S5*x  +S6*x   )| <= 2
 *  |  x                                               |
 *
 *  4. sin(x+y) = sin(x) + sin'(x')*y
 *          ~ sin(x) + (1-x*x/2)*y
 *     For better accuracy, let
 *           3      2      2      2      2
 *      r = x *(S2+x *(S3+x *(S4+x *(S5+x *S6))))
 *     then                   3    2
 *      sin(x) = x + (S1*x + (x *(r-y/2)+y))
 */

ಸ್ಥಿರ ಧ್ರುವ float64_t
// -1/3!  = -1/6
S1  = float64_t::fromRaw( 0xBFC5555555555549 ),
//  1/5!  =  1/120
S2  = float64_t::fromRaw( 0x3F8111111110F8A6 ),
// -1/7!  = -1/5040
S3  = float64_t::fromRaw( 0xBF2A01A019C161D5 ),
//  1/9!  =  1/362880
S4  = float64_t::fromRaw( 0x3EC71DE357B1FE7D ),
// -1/11! = -1/39916800
S5  = float64_t::fromRaw( 0xBE5AE5E68A2B9CEB ),
//  1/13! =  1/6227020800
S6  = float64_t::fromRaw( 0x3DE5D93A5ACFD57C );

ಸ್ಥಿರ float64_t f64_sin_kernel(float64_t x)
{
    ಯದಿ(x.getExp() < -27)
    {
        ಯದಿ(x != x.zero()) raiseFlags(flag_inexact);
        ಪ್ರತ್ಯಾಗಮ x;
    }

    float64_t z = x*x;
    ಪ್ರತ್ಯಾಗಮ x*mulAdd(z, mulAdd(z, mulAdd(z, mulAdd(z, mulAdd(z, mulAdd(z,
                    S6, S5), S4), S3), S2), S1), x.one());
}

/*
 * __kernel_cos( x,  y )
 * N.B. from OpenCV's side: y removed, simplified to one polynomial
 *
 * kernel cos function on [-pi/4, pi/4], pi/4 ~ 0.785398164
 * Input x is assumed to be bounded by ~pi/4 in magnitude.
 * Input y is the tail of x.
 *
 * Algorithm
 *  1. Since cos(-x) = cos(x), we need only to consider positive x.
 *  2. if x < 2^-27 (hx<0x3e400000 0), return 1 with inexact if x!=0.
 *  3. cos(x) is approximated by a polynomial of degree 14 on
 *     [0,pi/4]
 *                           4            14
 *      cos(x) ~ 1 - x*x/2 + C1*x + ... + C6*x
 *     where the remez error is
 *
 *  |              2     4     6     8     10    12     14 |     -58
 *  |cos(x)-(1-.5*x +C1*x +C2*x +C3*x +C4*x +C5*x  +C6*x  )| <= 2
 *  |                                                      |
 *
 *                 4     6     8     10    12     14
 *  4. let r = C1*x +C2*x +C3*x +C4*x +C5*x  +C6*x  , then
 *         cos(x) = 1 - x*x/2 + r
 *     since cos(x+y) ~ cos(x) - sin(x)*y
 *            ~ cos(x) - x*y,
 *     a correction term is necessary in cos(x) and hence
 *      cos(x+y) = 1 - (x*x/2 - (r - x*y))
 *
 * N.B. The following part was removed since we have enough precision
 *
 *     For better accuracy when x > 0.3, let qx = |x|/4 with
 *     the last 32 bits mask off, and if x > 0.78125, let qx = 0.28125.
 *     Then
 *      cos(x+y) = (1-qx) - ((x*x/2-qx) - (r-x*y)).
 *         Note that 1-qx and (x*x/2-qx) is EXACT here, and the
 *     magnitude of the latter is at least a quarter of x*x/2,
 *     thus, reducing the rounding error in the subtraction.
 */

ಸ್ಥಿರ ಧ್ರುವ float64_t
//  1/4!  =  1/24
C1  = float64_t::fromRaw( 0x3FA555555555554C ),
// -1/6!  = -1/720
C2  = float64_t::fromRaw( 0xBF56C16C16C15177 ),
//  1/8!  =  1/40320
C3  = float64_t::fromRaw( 0x3EFA01A019CB1590 ),
// -1/10! = -1/3628800
C4  = float64_t::fromRaw( 0xBE927E4F809C52AD ),
//  1/12! =  1/479001600
C5  = float64_t::fromRaw( 0x3E21EE9EBDB4B1C4 ),
// -1/14! = -1/87178291200
C6  = float64_t::fromRaw( 0xBDA8FAE9BE8838D4 );

ಸ್ಥಿರ float64_t f64_cos_kernel(float64_t x)
{
    ಯದಿ(x.getExp() < -27)
    {
        ಯದಿ(x != x.zero()) raiseFlags(flag_inexact);
        ಪ್ರತ್ಯಾಗಮ x.one();
    }

    float64_t z = x*x;
    ಪ್ರತ್ಯಾಗಮ mulAdd(mulAdd(z, mulAdd(z, mulAdd(z, mulAdd(z, mulAdd(z, mulAdd(z,
                  C6, C5), C4), C3), C2), C1), -half), z, x.one());
}

ಸ್ಥಿರ ಶೂನ್ಯ f64_sincos_reduce(ಧ್ರುವ float64_t& x, float64_t& y, ಪೂರ್ಣಾಙ್ಕ& n)
{
    ಯದಿ(abs(x) < piby4)
    {
        n = 0, y = x;
    }
    ಅನ್ಯಥಾ
    {
        /* argument reduction needed */
        float64_t p = f64_rem(x, pi2);
        float64_t v = p - float64_t::eps().setExp(-10);
        ಯದಿ(abs(v) <= piby4)
        {
            n = 0; y = p;
        }
        ಅನ್ಯಥಾ ಯದಿ(abs(v) <= (float64_t(3)*piby4))
        {
            n = (p > 0) ? 1 : 3;
            y = (p > 0) ? p - piby2 : p + piby2;
            ಯದಿ(p > 0) n = 1, y = p - piby2;
            ಅನ್ಯಥಾ      n = 3, y = p + piby2;
        }
        ಅನ್ಯಥಾ
        {
            n = 2;
            y = (p > 0) ? p - p.pi() : p + p.pi();
        }
    }
}

/* sin(x)
 * Return sine function of x.
 *
 * kernel function:
 *  __kernel_sin        ... sine function on [-pi/4,pi/4]
 *  __kernel_cos        ... cose function on [-pi/4,pi/4]
 *
 * Method.
 *      Let S,C and T denote the sin, cos and tan respectively on
 *  [-PI/4, +PI/4]. Reduce the argument x to y = x-k*pi/2
 *  in [-pi/4 , +pi/4], and let n = k mod 4.
 *  We have
 *
 *      n        sin(x)    cos(x)    tan(x)
 *     ----------------------------------------------------------
 *      0          S       C         T
 *      1          C      -S        -1/T
 *      2         -S      -C         T
 *      3         -C       S        -1/T
 *     ----------------------------------------------------------
 *
 * Special cases:
 *      Let trig be any of sin, cos, or tan.
 *      trig(+-INF)  is NaN, with signals;
 *      trig(NaN)    is that NaN;
 *
 * Accuracy:
 *  TRIG(x) returns trig(x) nearly rounded
 */

ಸ್ಥಿರ float64_t f64_sin( float64_t x )
{
    ಯದಿ(x.isInf() || x.isNaN()) ಪ್ರತ್ಯಾಗಮ x.nan();

    float64_t y; ಪೂರ್ಣಾಙ್ಕ n;
    f64_sincos_reduce(x, y, n);
    ವಿಕಲ್ಪ (n)
    {
    ಪಕ್ಷ 0:  ಪ್ರತ್ಯಾಗಮ  f64_sin_kernel(y);
    ಪಕ್ಷ 1:  ಪ್ರತ್ಯಾಗಮ  f64_cos_kernel(y);
    ಪಕ್ಷ 2:  ಪ್ರತ್ಯಾಗಮ -f64_sin_kernel(y);
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ -f64_cos_kernel(y);
    }
}

/* cos(x)
 * Return cosine function of x.
 *
 * kernel function:
 *  __kernel_sin        ... sine function on [-pi/4,pi/4]
 *  __kernel_cos        ... cosine function on [-pi/4,pi/4]
 *
 * Method.
 *      Let S,C and T denote the sin, cos and tan respectively on
 *  [-PI/4, +PI/4]. Reduce the argument x to y = x-k*pi/2
 *  in [-pi/4 , +pi/4], and let n = k mod 4.
 *  We have
 *
 *      n       sin(x)      cos(x)       tan(x)
 *     ----------------------------------------------------------
 *      0       S            C           T
 *      1       C           -S          -1/T
 *      2      -S           -C           T
 *      3      -C            S          -1/T
 *     ----------------------------------------------------------
 *
 * Special cases:
 *      Let trig be any of sin, cos, or tan.
 *      trig(+-INF)  is NaN, with signals;
 *      trig(NaN)    is that NaN;
 *
 * Accuracy:
 *  TRIG(x) returns trig(x) nearly rounded
 */

ಸ್ಥಿರ float64_t f64_cos( float64_t x )
{
    ಯದಿ(x.isInf() || x.isNaN()) ಪ್ರತ್ಯಾಗಮ x.nan();

    float64_t y; ಪೂರ್ಣಾಙ್ಕ n;
    f64_sincos_reduce(x, y, n);
    ವಿಕಲ್ಪ (n)
    {
    ಪಕ್ಷ 0:  ಪ್ರತ್ಯಾಗಮ  f64_cos_kernel(y);
    ಪಕ್ಷ 1:  ಪ್ರತ್ಯಾಗಮ -f64_sin_kernel(y);
    ಪಕ್ಷ 2:  ಪ್ರತ್ಯಾಗಮ -f64_cos_kernel(y);
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ  f64_sin_kernel(y);
    }
}

}
