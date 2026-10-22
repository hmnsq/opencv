// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"

نطاق cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

تعريفنوع فراغ (*Reduce_جوڑ_Func)(ثابت Mat& src, Mat& dst);
Reduce_جوڑ_Func getReduceCSumFunc(صحيح sdepth, صحيح ddepth);
Reduce_جوڑ_Func getReduceRSumFunc(صحيح sdepth, صحيح ddepth);
Reduce_جوڑ_Func getReduceRSum2Func(صحيح sdepth, صحيح ddepth);

#جے_بنا_تعریف CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// =====================================================================
//  Col reduce SUM (dim=1): sum each row into cn output values
// =====================================================================

#إذا (CV_SIMD || CV_SIMD_SCALABLE)

// --- uchar → int ---
// Uses u16 intermediate accumulator to halve the number of widen operations:
// u8→u16 per iteration, u16→u32 flush every 128 iterations (max u16 value: 128*255=32640 < 65535)
ساكن فراغ reduce_خانہ_جوڑ_8u32s(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح cn = srcmat.channels();
    ثابت صحيح خانے_گنتی = srcmat.خانے_گنتی;
    ثابت صحيح چوڑائی = خانے_گنتی * cn;

    خودکار body = [&](ثابت Range& range) {
        لكل (صحيح y = range.شروع; y < range.end; y++)
        {
            ثابت uchar* src = srcmat.ptr<uchar>(y);
            صحيح* dst = dstmat.ptr<صحيح>(y);

            إذا (cn == 1)
            {
#إذا defined(CV_NEON_DOT)
                // ARMv8.2 DOTPROD: vdotq_u32(acc, src, ones) — single instruction byte sum
                // u32 lane max 1020/iter, overflow at ~4.2M iter (67MB) — safe without flush
                ثابت uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t v_جوڑ = vdupq_n_u32(0);
                صحيح x = 0;
                لكل (; x <= چوڑائی - 16; x += 16)
                    v_جوڑ = vdotq_u32(v_جوڑ, vld1q_u8(src + x), ones);
                صحيح کل = (صحيح)vaddvq_u32(v_جوڑ);
                لكل (; x < چوڑائی; x++)
                    کل += (صحيح)src[x];
                dst[0] = کل;
#وإلاإذا CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                // AArch64 fallback: vpaddlq chain
                uint32x4_t v_جوڑ = vdupq_n_u32(0);
                صحيح x = 0;
                لكل (; x <= چوڑائی - 16; x += 16)
                    v_جوڑ = vaddq_u32(v_جوڑ, vpaddlq_u16(vpaddlq_u8(vld1q_u8(src + x))));
                صحيح کل = (صحيح)vaddvq_u32(v_جوڑ);
                لكل (; x < چوڑائی; x++)
                    کل += (صحيح)src[x];
                dst[0] = کل;
#وإلاإذا CV_AVX2
                // Intel AVX2: _mm256_sad_epu8 — 32 bytes → 4×u64 sum, 1 cycle
                ثابت __m256i zero = _mm256_setzero_si256();
                __m256i vsum = zero;
                صحيح x = 0;
                لكل (; x <= چوڑائی - 32; x += 32)
                    vsum = mm256_جوڑو_epi64(vsum, _mm256_sad_epu8(
                        _mm256_loadu_si256((ثابت __m256i*)(src + x)), zero));
                // 8-byte tail: reduce max scalar tail from 31 to 7
                // Use _mm256_inserti128_si256 instead of _mm256_castsi128_si256:
                // the latter leaves upper 128 bits undefined per Intel spec,
                // causing MSVC to expose stale register data to _mm256_sad_epu8.
                لكل (; x <= چوڑائی - 8; x += 8)
                    vsum = mm256_جوڑو_epi64(vsum, _mm256_sad_epu8(
                        _mm256_inserti128_si256(_mm256_setzero_si256(), _mm_loadl_epi64((ثابت __m128i*)(src + x)), 0), zero));
                __m128i lo128 = _mm256_castsi256_si128(vsum);
                __m128i hi128 = _mm256_extracti128_si256(vsum, 1);
                __m128i s = mm_جوڑو_epi64(lo128, hi128);
                s = mm_جوڑو_epi64(s, _mm_unpackhi_epi64(s, s));
                صحيح کل = _mm_cvtsi128_si32(s);
                لكل (; x < چوڑائی; x++)
                    کل += (صحيح)src[x];
                dst[0] = کل;
#وإلاإذا CV_SSE2
                // Intel SSE2: _mm_sad_epu8 — 16 bytes → 2×u64 sum
                ثابت __m128i zero = _mm_setzero_si128();
                __m128i vsum = zero;
                صحيح x = 0;
                لكل (; x <= چوڑائی - 16; x += 16)
                    vsum = mm_جوڑو_epi64(vsum, _mm_sad_epu8(
                        _mm_loadu_si128((ثابت __m128i*)(src + x)), zero));
                __m128i s = mm_جوڑو_epi64(vsum, _mm_unpackhi_epi64(vsum, vsum));
                صحيح کل = _mm_cvtsi128_si32(s);
                لكل (; x < چوڑائی; x++)
                    کل += (صحيح)src[x];
                dst[0] = کل;
#وإلا
                ثابت صحيح vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 v_جوڑ = vx_setzero_u32();
                v_uint16 v_sum16 = vx_setzero_u16();
                صحيح x = 0, batch = 0;
                ثابت صحيح flush_at = 128;

                لكل (; x <= چوڑائی - vlanes8; x += vlanes8)
                {
                    v_uint16 lo, hi;
                    v_expand(vx_لدو(src + x), lo, hi);
                    v_sum16 = v_جوڑو(v_sum16, v_جوڑو(lo, hi));

                    إذا (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(v_sum16, a, b);
                        v_جوڑ = v_جوڑو(v_جوڑ, v_جوڑو(a, b));
                        v_sum16 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(v_sum16, a, b);
                    v_جوڑ = v_جوڑو(v_جوڑ, v_جوڑو(a, b));
                }

                صحيح کل = (صحيح)v_reduce_جوڑ(v_جوڑ);
                لكل (; x < چوڑائی; x++)
                    کل += (صحيح)src[x];
                dst[0] = کل;
#جے_انت
            }
            وإلا إذا (cn == 4)
            {
#إذا defined(CV_NEON_DOT)
                // ARMv8.2 DOTPROD: vld4q_u8 deinterleave + vdotq_u32 per-channel
                // vld4q_u8 reads 16 pixels (64 bytes), deinterleaves into 4 × u8x16
                // vdotq_u32 sums each channel's 16 bytes into 4 partial u32 sums
                ثابت uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t acc0 = vdupq_n_u32(0), acc1 = vdupq_n_u32(0);
                uint32x4_t acc2 = vdupq_n_u32(0), acc3 = vdupq_n_u32(0);
                صحيح x = 0;
                لكل (; x <= خانے_گنتی - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    acc0 = vdotq_u32(acc0, v.val[0], ones);
                    acc1 = vdotq_u32(acc1, v.val[1], ones);
                    acc2 = vdotq_u32(acc2, v.val[2], ones);
                    acc3 = vdotq_u32(acc3, v.val[3], ones);
                }
                dst[0] = (صحيح)vaddvq_u32(acc0);
                dst[1] = (صحيح)vaddvq_u32(acc1);
                dst[2] = (صحيح)vaddvq_u32(acc2);
                dst[3] = (صحيح)vaddvq_u32(acc3);
                لكل (; x < خانے_گنتی; x++)
                {
                    dst[0] += (صحيح)src[x * 4];
                    dst[1] += (صحيح)src[x * 4 + 1];
                    dst[2] += (صحيح)src[x * 4 + 2];
                    dst[3] += (صحيح)src[x * 4 + 3];
                }
#وإلاإذا CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                // AArch64 fallback: vld4q_u8 deinterleave + vaddlvq_u8 per-channel
                صحيح sums[4] = {0, 0, 0, 0};
                صحيح x = 0;
                لكل (; x <= خانے_گنتی - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    sums[0] += (صحيح)vaddlvq_u8(v.val[0]);
                    sums[1] += (صحيح)vaddlvq_u8(v.val[1]);
                    sums[2] += (صحيح)vaddlvq_u8(v.val[2]);
                    sums[3] += (صحيح)vaddlvq_u8(v.val[3]);
                }
                dst[0] = sums[0]; dst[1] = sums[1];
                dst[2] = sums[2]; dst[3] = sums[3];
                لكل (; x < خانے_گنتی; x++)
                {
                    dst[0] += (صحيح)src[x * 4];
                    dst[1] += (صحيح)src[x * 4 + 1];
                    dst[2] += (صحيح)src[x * 4 + 2];
                    dst[3] += (صحيح)src[x * 4 + 3];
                }
#وإلاإذا CV_AVX2
                // Intel AVX2: shuffle + unpack to group channels, full-register SAD
                ثابت __m256i zero = _mm256_setzero_si256();
                ثابت __m256i shuf_نقاب = _mm256_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                صحيح x = 0;
                // 16 pixels/iter with unpack (full-register SAD)
                {
                    __m256i جوڑ_bg = zero, جوڑ_ra = zero;
                    لكل (; x <= خانے_گنتی - 16; x += 16)
                    {
                        __m256i v0 = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4));
                        __m256i v1 = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4 + 32));
                        __m256i g0 = _mm256_shuffle_epi8(v0, shuf_نقاب);
                        __m256i g1 = _mm256_shuffle_epi8(v1, shuf_نقاب);
                        __m256i bg = _mm256_unpacklo_epi32(g0, g1);
                        __m256i ra = _mm256_unpackhi_epi32(g0, g1);
                        جوڑ_bg = mm256_جوڑو_epi64(جوڑ_bg, _mm256_sad_epu8(bg, zero));
                        جوڑ_ra = mm256_جوڑو_epi64(جوڑ_ra, _mm256_sad_epu8(ra, zero));
                    }
                    __m128i bg_lo = _mm256_castsi256_si128(جوڑ_bg);
                    __m128i bg_hi = _mm256_extracti128_si256(جوڑ_bg, 1);
                    __m128i bg_s = mm_جوڑو_epi64(bg_lo, bg_hi);
                    __m128i ra_lo = _mm256_castsi256_si128(جوڑ_ra);
                    __m128i ra_hi = _mm256_extracti128_si256(جوڑ_ra, 1);
                    __m128i ra_s = mm_جوڑو_epi64(ra_lo, ra_hi);
                    dst[0] = _mm_cvtsi128_si32(bg_s);                     // B
                    dst[1] = _mm_cvtsi128_si32(_mm_srli_si128(bg_s, 8));  // G
                    dst[2] = _mm_cvtsi128_si32(ra_s);                     // R
                    dst[3] = _mm_cvtsi128_si32(_mm_srli_si128(ra_s, 8));  // A
                }
                // 8-pixel tail with and/srli
                {
                    ثابت __m256i نقاب_lo32 = _mm256_set1_epi64x(0x00000000FFFFFFFFLL);
                    __m256i جوڑ_br = zero, جوڑ_ga = zero;
                    لكل (; x <= خانے_گنتی - 8; x += 8)
                    {
                        __m256i v = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4));
                        __m256i grouped = _mm256_shuffle_epi8(v, shuf_نقاب);
                        __m256i br = _mm256_and_si256(grouped, نقاب_lo32);
                        __m256i ga = _mm256_srli_epi64(grouped, 32);
                        جوڑ_br = mm256_جوڑو_epi64(جوڑ_br, _mm256_sad_epu8(br, zero));
                        جوڑ_ga = mm256_جوڑو_epi64(جوڑ_ga, _mm256_sad_epu8(ga, zero));
                    }
                    __m128i br_lo = _mm256_castsi256_si128(جوڑ_br);
                    __m128i br_hi = _mm256_extracti128_si256(جوڑ_br, 1);
                    __m128i br_s = mm_جوڑو_epi64(br_lo, br_hi);
                    __m128i ga_lo = _mm256_castsi256_si128(جوڑ_ga);
                    __m128i ga_hi = _mm256_extracti128_si256(جوڑ_ga, 1);
                    __m128i ga_s = mm_جوڑو_epi64(ga_lo, ga_hi);
                    dst[0] += _mm_cvtsi128_si32(br_s);
                    dst[1] += _mm_cvtsi128_si32(ga_s);
                    dst[2] += _mm_cvtsi128_si32(_mm_srli_si128(br_s, 8));
                    dst[3] += _mm_cvtsi128_si32(_mm_srli_si128(ga_s, 8));
                }
                لكل (; x < خانے_گنتی; x++)
                {
                    dst[0] += (صحيح)src[x * 4];
                    dst[1] += (صحيح)src[x * 4 + 1];
                    dst[2] += (صحيح)src[x * 4 + 2];
                    dst[3] += (صحيح)src[x * 4 + 3];
                }
#وإلاإذا CV_SSSE3
                // Intel SSSE3: _mm_shuffle_epi8 deinterleave + _mm_sad_epu8
                // 16 bytes = 4 pixels × 4 channels per iteration
                ثابت __m128i zero = _mm_setzero_si128();
                ثابت __m128i shuf_نقاب = _mm_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                ثابت __m128i نقاب_lo32 = _mm_set1_epi64x(0x00000000FFFFFFFFLL);
                __m128i جوڑ_br = zero, جوڑ_ga = zero;
                صحيح x = 0;
                لكل (; x <= خانے_گنتی - 4; x += 4)
                {
                    __m128i v = _mm_loadu_si128((ثابت __m128i*)(src + x * 4));
                    __m128i grouped = _mm_shuffle_epi8(v, shuf_نقاب);
                    __m128i br = _mm_and_si128(grouped, نقاب_lo32);
                    __m128i ga = _mm_srli_epi64(grouped, 32);
                    جوڑ_br = mm_جوڑو_epi64(جوڑ_br, _mm_sad_epu8(br, zero));
                    جوڑ_ga = mm_جوڑو_epi64(جوڑ_ga, _mm_sad_epu8(ga, zero));
                }
                dst[0] = _mm_cvtsi128_si32(جوڑ_br);
                dst[1] = _mm_cvtsi128_si32(جوڑ_ga);
                dst[2] = _mm_cvtsi128_si32(_mm_srli_si128(جوڑ_br, 8));
                dst[3] = _mm_cvtsi128_si32(_mm_srli_si128(جوڑ_ga, 8));
                لكل (; x < خانے_گنتی; x++)
                {
                    dst[0] += (صحيح)src[x * 4];
                    dst[1] += (صحيح)src[x * 4 + 1];
                    dst[2] += (صحيح)src[x * 4 + 2];
                    dst[3] += (صحيح)src[x * 4 + 3];
                }
#وإلا
                ثابت صحيح vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 vsum0 = vx_setzero_u32(), vsum1 = vx_setzero_u32();
                v_uint32 vsum2 = vx_setzero_u32(), vsum3 = vx_setzero_u32();
                v_uint16 vs16_0 = vx_setzero_u16(), vs16_1 = vx_setzero_u16();
                v_uint16 vs16_2 = vx_setzero_u16(), vs16_3 = vx_setzero_u16();
                صحيح x = 0, batch = 0;
                ثابت صحيح flush_at = 128;

                لكل (; x <= خانے_گنتی - vlanes8; x += vlanes8)
                {
                    v_uint8 ch0, ch1, ch2, ch3;
                    v_لدو_deinterleave(src + x * 4, ch0, ch1, ch2, ch3);

                    v_uint16 lo, hi;
                    v_expand(ch0, lo, hi); vs16_0 = v_جوڑو(vs16_0, v_جوڑو(lo, hi));
                    v_expand(ch1, lo, hi); vs16_1 = v_جوڑو(vs16_1, v_جوڑو(lo, hi));
                    v_expand(ch2, lo, hi); vs16_2 = v_جوڑو(vs16_2, v_جوڑو(lo, hi));
                    v_expand(ch3, lo, hi); vs16_3 = v_جوڑو(vs16_3, v_جوڑو(lo, hi));

                    إذا (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(vs16_0, a, b); vsum0 = v_جوڑو(vsum0, v_جوڑو(a, b));
                        v_expand(vs16_1, a, b); vsum1 = v_جوڑو(vsum1, v_جوڑو(a, b));
                        v_expand(vs16_2, a, b); vsum2 = v_جوڑو(vsum2, v_جوڑو(a, b));
                        v_expand(vs16_3, a, b); vsum3 = v_جوڑو(vsum3, v_جوڑو(a, b));
                        vs16_0 = vx_setzero_u16(); vs16_1 = vx_setzero_u16();
                        vs16_2 = vx_setzero_u16(); vs16_3 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(vs16_0, a, b); vsum0 = v_جوڑو(vsum0, v_جوڑو(a, b));
                    v_expand(vs16_1, a, b); vsum1 = v_جوڑو(vsum1, v_جوڑو(a, b));
                    v_expand(vs16_2, a, b); vsum2 = v_جوڑو(vsum2, v_جوڑو(a, b));
                    v_expand(vs16_3, a, b); vsum3 = v_جوڑو(vsum3, v_جوڑو(a, b));
                }

                dst[0] = (صحيح)v_reduce_جوڑ(vsum0);
                dst[1] = (صحيح)v_reduce_جوڑ(vsum1);
                dst[2] = (صحيح)v_reduce_جوڑ(vsum2);
                dst[3] = (صحيح)v_reduce_جوڑ(vsum3);

                لكل (; x < خانے_گنتی; x++)
                {
                    dst[0] += (صحيح)src[x * 4];
                    dst[1] += (صحيح)src[x * 4 + 1];
                    dst[2] += (صحيح)src[x * 4 + 2];
                    dst[3] += (صحيح)src[x * 4 + 3];
                }
#جے_انت
            }
            وإلا
            {
                // generic cn: scalar fallback
                لكل (صحيح c = 0; c < cn; c++)
                    dst[c] = (صحيح)src[c];
                لكل (صحيح x = cn; x < چوڑائی; x += cn)
                    لكل (صحيح c = 0; c < cn; c++)
                        dst[c] += (صحيح)src[x + c];
            }
        }
    };

    parallel_for_(Range(0, srcmat.صفاں), body);
    v_cleanup();
}

// --- uchar → float ---
ساكن فراغ reduce_خانہ_جوڑ_8u32f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح cn = srcmat.channels();
    ثابت صحيح خانے_گنتی = srcmat.خانے_گنتی;
    ثابت صحيح چوڑائی = خانے_گنتی * cn;

    خودکار body = [&](ثابت Range& range) {
        لكل (صحيح y = range.شروع; y < range.end; y++)
        {
            ثابت uchar* src = srcmat.ptr<uchar>(y);
            عائم* dst = dstmat.ptr<عائم>(y);

            // compute in int, then convert to float
            Auto_بفر<صحيح> ibuf(cn);
            صحيح* sums = ibuf.انکڑے();
            لكل (صحيح c = 0; c < cn; c++)
                sums[c] = 0;

            إذا (cn == 1)
            {
#إذا defined(CV_NEON_DOT)
                ثابت uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t v_جوڑ = vdupq_n_u32(0);
                صحيح x = 0;
                لكل (; x <= چوڑائی - 16; x += 16)
                    v_جوڑ = vdotq_u32(v_جوڑ, vld1q_u8(src + x), ones);
                sums[0] = (صحيح)vaddvq_u32(v_جوڑ);
                لكل (; x < چوڑائی; x++)
                    sums[0] += (صحيح)src[x];
#وإلاإذا CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                uint32x4_t v_جوڑ = vdupq_n_u32(0);
                صحيح x = 0;
                لكل (; x <= چوڑائی - 16; x += 16)
                    v_جوڑ = vaddq_u32(v_جوڑ, vpaddlq_u16(vpaddlq_u8(vld1q_u8(src + x))));
                sums[0] = (صحيح)vaddvq_u32(v_جوڑ);
                لكل (; x < چوڑائی; x++)
                    sums[0] += (صحيح)src[x];
#وإلاإذا CV_AVX2
                ثابت __m256i zero = _mm256_setzero_si256();
                __m256i vsum = zero;
                صحيح x = 0;
                لكل (; x <= چوڑائی - 32; x += 32)
                    vsum = mm256_جوڑو_epi64(vsum, _mm256_sad_epu8(
                        _mm256_loadu_si256((ثابت __m256i*)(src + x)), zero));
                // 8-byte tail: reduce max scalar tail from 31 to 7
                // Use _mm256_inserti128_si256 instead of _mm256_castsi128_si256:
                // the latter leaves upper 128 bits undefined per Intel spec,
                // causing MSVC to expose stale register data to _mm256_sad_epu8.
                لكل (; x <= چوڑائی - 8; x += 8)
                    vsum = mm256_جوڑو_epi64(vsum, _mm256_sad_epu8(
                        _mm256_inserti128_si256(_mm256_setzero_si256(), _mm_loadl_epi64((ثابت __m128i*)(src + x)), 0), zero));
                __m128i lo128 = _mm256_castsi256_si128(vsum);
                __m128i hi128 = _mm256_extracti128_si256(vsum, 1);
                __m128i s = mm_جوڑو_epi64(lo128, hi128);
                s = mm_جوڑو_epi64(s, _mm_unpackhi_epi64(s, s));
                sums[0] = _mm_cvtsi128_si32(s);
                لكل (; x < چوڑائی; x++)
                    sums[0] += (صحيح)src[x];
#وإلاإذا CV_SSE2
                ثابت __m128i zero = _mm_setzero_si128();
                __m128i vsum = zero;
                صحيح x = 0;
                لكل (; x <= چوڑائی - 16; x += 16)
                    vsum = mm_جوڑو_epi64(vsum, _mm_sad_epu8(
                        _mm_loadu_si128((ثابت __m128i*)(src + x)), zero));
                __m128i s = mm_جوڑو_epi64(vsum, _mm_unpackhi_epi64(vsum, vsum));
                sums[0] = _mm_cvtsi128_si32(s);
                لكل (; x < چوڑائی; x++)
                    sums[0] += (صحيح)src[x];
#وإلا
                ثابت صحيح vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 v_جوڑ = vx_setzero_u32();
                v_uint16 v_sum16 = vx_setzero_u16();
                صحيح x = 0, batch = 0;

                لكل (; x <= چوڑائی - vlanes8; x += vlanes8)
                {
                    v_uint16 lo, hi;
                    v_expand(vx_لدو(src + x), lo, hi);
                    v_sum16 = v_جوڑو(v_sum16, v_جوڑو(lo, hi));
                    إذا (++batch >= 128)
                    {
                        v_uint32 a, b;
                        v_expand(v_sum16, a, b);
                        v_جوڑ = v_جوڑو(v_جوڑ, v_جوڑو(a, b));
                        v_sum16 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                v_uint32 a, b;
                v_expand(v_sum16, a, b);
                v_جوڑ = v_جوڑو(v_جوڑ, v_جوڑو(a, b));

                sums[0] = (صحيح)v_reduce_جوڑ(v_جوڑ);
                لكل (; x < چوڑائی; x++)
                    sums[0] += (صحيح)src[x];
#جے_انت
            }
            وإلا إذا (cn == 4)
            {
#إذا defined(CV_NEON_DOT)
                ثابت uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t acc0 = vdupq_n_u32(0), acc1 = vdupq_n_u32(0);
                uint32x4_t acc2 = vdupq_n_u32(0), acc3 = vdupq_n_u32(0);
                صحيح x = 0;
                لكل (; x <= خانے_گنتی - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    acc0 = vdotq_u32(acc0, v.val[0], ones);
                    acc1 = vdotq_u32(acc1, v.val[1], ones);
                    acc2 = vdotq_u32(acc2, v.val[2], ones);
                    acc3 = vdotq_u32(acc3, v.val[3], ones);
                }
                sums[0] = (صحيح)vaddvq_u32(acc0);
                sums[1] = (صحيح)vaddvq_u32(acc1);
                sums[2] = (صحيح)vaddvq_u32(acc2);
                sums[3] = (صحيح)vaddvq_u32(acc3);
                لكل (; x < خانے_گنتی; x++)
                {
                    sums[0] += (صحيح)src[x * 4];
                    sums[1] += (صحيح)src[x * 4 + 1];
                    sums[2] += (صحيح)src[x * 4 + 2];
                    sums[3] += (صحيح)src[x * 4 + 3];
                }
#وإلاإذا CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                صحيح x = 0;
                لكل (; x <= خانے_گنتی - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    sums[0] += (صحيح)vaddlvq_u8(v.val[0]);
                    sums[1] += (صحيح)vaddlvq_u8(v.val[1]);
                    sums[2] += (صحيح)vaddlvq_u8(v.val[2]);
                    sums[3] += (صحيح)vaddlvq_u8(v.val[3]);
                }
                لكل (; x < خانے_گنتی; x++)
                {
                    sums[0] += (صحيح)src[x * 4];
                    sums[1] += (صحيح)src[x * 4 + 1];
                    sums[2] += (صحيح)src[x * 4 + 2];
                    sums[3] += (صحيح)src[x * 4 + 3];
                }
#وإلاإذا CV_AVX2
                // Intel AVX2: shuffle + unpack to group channels, full-register SAD
                ثابت __m256i zero = _mm256_setzero_si256();
                ثابت __m256i shuf_نقاب = _mm256_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                صحيح x = 0;
                // 16 pixels/iter with unpack (full-register SAD)
                {
                    __m256i جوڑ_bg = zero, جوڑ_ra = zero;
                    لكل (; x <= خانے_گنتی - 16; x += 16)
                    {
                        __m256i v0 = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4));
                        __m256i v1 = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4 + 32));
                        __m256i g0 = _mm256_shuffle_epi8(v0, shuf_نقاب);
                        __m256i g1 = _mm256_shuffle_epi8(v1, shuf_نقاب);
                        __m256i bg = _mm256_unpacklo_epi32(g0, g1);
                        __m256i ra = _mm256_unpackhi_epi32(g0, g1);
                        جوڑ_bg = mm256_جوڑو_epi64(جوڑ_bg, _mm256_sad_epu8(bg, zero));
                        جوڑ_ra = mm256_جوڑو_epi64(جوڑ_ra, _mm256_sad_epu8(ra, zero));
                    }
                    __m128i bg_lo = _mm256_castsi256_si128(جوڑ_bg);
                    __m128i bg_hi = _mm256_extracti128_si256(جوڑ_bg, 1);
                    __m128i bg_s = mm_جوڑو_epi64(bg_lo, bg_hi);
                    __m128i ra_lo = _mm256_castsi256_si128(جوڑ_ra);
                    __m128i ra_hi = _mm256_extracti128_si256(جوڑ_ra, 1);
                    __m128i ra_s = mm_جوڑو_epi64(ra_lo, ra_hi);
                    sums[0] = _mm_cvtsi128_si32(bg_s);                     // B
                    sums[1] = _mm_cvtsi128_si32(_mm_srli_si128(bg_s, 8));  // G
                    sums[2] = _mm_cvtsi128_si32(ra_s);                     // R
                    sums[3] = _mm_cvtsi128_si32(_mm_srli_si128(ra_s, 8));  // A
                }
                // 8-pixel tail with and/srli
                {
                    ثابت __m256i نقاب_lo32 = _mm256_set1_epi64x(0x00000000FFFFFFFFLL);
                    __m256i جوڑ_br = zero, جوڑ_ga = zero;
                    لكل (; x <= خانے_گنتی - 8; x += 8)
                    {
                        __m256i v = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4));
                        __m256i grouped = _mm256_shuffle_epi8(v, shuf_نقاب);
                        __m256i br = _mm256_and_si256(grouped, نقاب_lo32);
                        __m256i ga = _mm256_srli_epi64(grouped, 32);
                        جوڑ_br = mm256_جوڑو_epi64(جوڑ_br, _mm256_sad_epu8(br, zero));
                        جوڑ_ga = mm256_جوڑو_epi64(جوڑ_ga, _mm256_sad_epu8(ga, zero));
                    }
                    __m128i br_lo = _mm256_castsi256_si128(جوڑ_br);
                    __m128i br_hi = _mm256_extracti128_si256(جوڑ_br, 1);
                    __m128i br_s = mm_جوڑو_epi64(br_lo, br_hi);
                    __m128i ga_lo = _mm256_castsi256_si128(جوڑ_ga);
                    __m128i ga_hi = _mm256_extracti128_si256(جوڑ_ga, 1);
                    __m128i ga_s = mm_جوڑو_epi64(ga_lo, ga_hi);
                    sums[0] += _mm_cvtsi128_si32(br_s);
                    sums[1] += _mm_cvtsi128_si32(ga_s);
                    sums[2] += _mm_cvtsi128_si32(_mm_srli_si128(br_s, 8));
                    sums[3] += _mm_cvtsi128_si32(_mm_srli_si128(ga_s, 8));
                }
                لكل (; x < خانے_گنتی; x++)
                {
                    sums[0] += (صحيح)src[x * 4];
                    sums[1] += (صحيح)src[x * 4 + 1];
                    sums[2] += (صحيح)src[x * 4 + 2];
                    sums[3] += (صحيح)src[x * 4 + 3];
                }
#وإلاإذا CV_SSSE3
                // Intel SSSE3: _mm_shuffle_epi8 deinterleave + _mm_sad_epu8
                ثابت __m128i zero = _mm_setzero_si128();
                ثابت __m128i shuf_نقاب = _mm_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                ثابت __m128i نقاب_lo32 = _mm_set1_epi64x(0x00000000FFFFFFFFLL);
                __m128i جوڑ_br = zero, جوڑ_ga = zero;
                صحيح x = 0;
                لكل (; x <= خانے_گنتی - 4; x += 4)
                {
                    __m128i v = _mm_loadu_si128((ثابت __m128i*)(src + x * 4));
                    __m128i grouped = _mm_shuffle_epi8(v, shuf_نقاب);
                    __m128i br = _mm_and_si128(grouped, نقاب_lo32);
                    __m128i ga = _mm_srli_epi64(grouped, 32);
                    جوڑ_br = mm_جوڑو_epi64(جوڑ_br, _mm_sad_epu8(br, zero));
                    جوڑ_ga = mm_جوڑو_epi64(جوڑ_ga, _mm_sad_epu8(ga, zero));
                }
                sums[0] = _mm_cvtsi128_si32(جوڑ_br);
                sums[1] = _mm_cvtsi128_si32(جوڑ_ga);
                sums[2] = _mm_cvtsi128_si32(_mm_srli_si128(جوڑ_br, 8));
                sums[3] = _mm_cvtsi128_si32(_mm_srli_si128(جوڑ_ga, 8));
                لكل (; x < خانے_گنتی; x++)
                {
                    sums[0] += (صحيح)src[x * 4];
                    sums[1] += (صحيح)src[x * 4 + 1];
                    sums[2] += (صحيح)src[x * 4 + 2];
                    sums[3] += (صحيح)src[x * 4 + 3];
                }
#وإلا
                ثابت صحيح vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 vsum0 = vx_setzero_u32(), vsum1 = vx_setzero_u32();
                v_uint32 vsum2 = vx_setzero_u32(), vsum3 = vx_setzero_u32();
                v_uint16 vs16_0 = vx_setzero_u16(), vs16_1 = vx_setzero_u16();
                v_uint16 vs16_2 = vx_setzero_u16(), vs16_3 = vx_setzero_u16();
                صحيح x = 0, batch = 0;
                ثابت صحيح flush_at = 128;

                لكل (; x <= خانے_گنتی - vlanes8; x += vlanes8)
                {
                    v_uint8 ch0, ch1, ch2, ch3;
                    v_لدو_deinterleave(src + x * 4, ch0, ch1, ch2, ch3);

                    v_uint16 lo, hi;
                    v_expand(ch0, lo, hi); vs16_0 = v_جوڑو(vs16_0, v_جوڑو(lo, hi));
                    v_expand(ch1, lo, hi); vs16_1 = v_جوڑو(vs16_1, v_جوڑو(lo, hi));
                    v_expand(ch2, lo, hi); vs16_2 = v_جوڑو(vs16_2, v_جوڑو(lo, hi));
                    v_expand(ch3, lo, hi); vs16_3 = v_جوڑو(vs16_3, v_جوڑو(lo, hi));

                    إذا (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(vs16_0, a, b); vsum0 = v_جوڑو(vsum0, v_جوڑو(a, b));
                        v_expand(vs16_1, a, b); vsum1 = v_جوڑو(vsum1, v_جوڑو(a, b));
                        v_expand(vs16_2, a, b); vsum2 = v_جوڑو(vsum2, v_جوڑو(a, b));
                        v_expand(vs16_3, a, b); vsum3 = v_جوڑو(vsum3, v_جوڑو(a, b));
                        vs16_0 = vx_setzero_u16(); vs16_1 = vx_setzero_u16();
                        vs16_2 = vx_setzero_u16(); vs16_3 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(vs16_0, a, b); vsum0 = v_جوڑو(vsum0, v_جوڑو(a, b));
                    v_expand(vs16_1, a, b); vsum1 = v_جوڑو(vsum1, v_جوڑو(a, b));
                    v_expand(vs16_2, a, b); vsum2 = v_جوڑو(vsum2, v_جوڑو(a, b));
                    v_expand(vs16_3, a, b); vsum3 = v_جوڑو(vsum3, v_جوڑو(a, b));
                }

                sums[0] = (صحيح)v_reduce_جوڑ(vsum0);
                sums[1] = (صحيح)v_reduce_جوڑ(vsum1);
                sums[2] = (صحيح)v_reduce_جوڑ(vsum2);
                sums[3] = (صحيح)v_reduce_جوڑ(vsum3);

                لكل (; x < خانے_گنتی; x++)
                {
                    sums[0] += (صحيح)src[x * 4];
                    sums[1] += (صحيح)src[x * 4 + 1];
                    sums[2] += (صحيح)src[x * 4 + 2];
                    sums[3] += (صحيح)src[x * 4 + 3];
                }
#جے_انت
            }
            وإلا
            {
                لكل (صحيح x = 0, c = 0; x < چوڑائی; x++, c = (c+1)&-(c < cn-1))
                    sums[c] += (صحيح)src[x];
            }
            لكل (صحيح c = 0; c < cn; c++)
                dst[c] = (عائم)sums[c];
        }
    };
    parallel_for_(Range(0, srcmat.صفاں), body);
    v_cleanup();
}

// --- 16-bit (ushort/short) → float ---
سانچا<قسم_ناں SrcT, قسم_ناں VecSrc, قسم_ناں VecDst>
ساكن فراغ reduce_خانہ_جوڑ_16_32f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح cn = srcmat.channels();
    ثابت صحيح چوڑائی = srcmat.خانے_گنتی * cn;
    ثابت صحيح vlanes = VTraits<VecSrc>::vlanes();

    خودکار body = [&](ثابت Range& range) {
        لكل (صحيح y = range.شروع; y < range.end; y++)
        {
            ثابت SrcT* src = srcmat.ptr<SrcT>(y);
            عائم* dst = dstmat.ptr<عائم>(y);

            إذا (cn == 1)
            {
                VecDst v_sum0 = v_setzero_<VecDst>(), v_sum1 = v_setzero_<VecDst>();
                صحيح x = 0;
                لكل (; x <= چوڑائی - vlanes; x += vlanes)
                {
                    VecDst lo, hi;
                    v_expand(vx_لدو(src + x), lo, hi);
                    v_sum0 = v_جوڑو(v_sum0, lo);
                    v_sum1 = v_جوڑو(v_sum1, hi);
                }
                dst[0] = (عائم)(v_reduce_جوڑ(v_sum0) + v_reduce_جوڑ(v_sum1));
                لكل (; x < چوڑائی; x++)
                    dst[0] += (عائم)src[x];
            }
            وإلا
            {
                لكل (صحيح c = 0; c < cn; c++)
                    dst[c] = 0;
                لكل (صحيح x = 0, c = 0; x < چوڑائی; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += (عائم)src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.صفاں), body);
    v_cleanup();
}

ساكن فراغ reduce_خانہ_جوڑ_16u32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_خانہ_جوڑ_16_32f<ushort, v_uint16, v_uint32>(srcmat, dstmat); }

ساكن فراغ reduce_خانہ_جوڑ_16s32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_خانہ_جوڑ_16_32f<چھوٹا, v_int16, v_int32>(srcmat, dstmat); }

// --- float/double → same type ---
سانچا<قسم_ناں T, قسم_ناں VecT>
ساكن فراغ reduce_خانہ_جوڑ_FP(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح cn = srcmat.channels();
    ثابت صحيح چوڑائی = srcmat.خانے_گنتی * cn;
    ثابت صحيح vlanes = VTraits<VecT>::vlanes();

    خودکار body = [&](ثابت Range& range) {
        لكل (صحيح y = range.شروع; y < range.end; y++)
        {
            ثابت T* src = srcmat.ptr<T>(y);
            T* dst = dstmat.ptr<T>(y);

            إذا (cn == 1)
            {
                VecT s0 = v_setzero_<VecT>(), s1 = v_setzero_<VecT>();
                VecT s2 = v_setzero_<VecT>(), s3 = v_setzero_<VecT>();
                صحيح x = 0;
                لكل (; x <= چوڑائی - vlanes * 4; x += vlanes * 4)
                {
                    s0 = v_جوڑو(s0, vx_لدو(src + x));
                    s1 = v_جوڑو(s1, vx_لدو(src + x + vlanes));
                    s2 = v_جوڑو(s2, vx_لدو(src + x + vlanes * 2));
                    s3 = v_جوڑو(s3, vx_لدو(src + x + vlanes * 3));
                }
                s0 = v_جوڑو(v_جوڑو(s0, s1), v_جوڑو(s2, s3));
                لكل (; x <= چوڑائی - vlanes; x += vlanes)
                    s0 = v_جوڑو(s0, vx_لدو(src + x));
                T کل = (T)v_reduce_جوڑ(s0);
                لكل (; x < چوڑائی; x++)
                    کل += src[x];
                dst[0] = کل;
            }
            وإلا
            {
                لكل (صحيح c = 0; c < cn; c++)
                    dst[c] = 0;
                لكل (صحيح x = 0, c = 0; x < چوڑائی; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.صفاں), body);
    v_cleanup();
}

ساكن فراغ reduce_خانہ_جوڑ_32f32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_خانہ_جوڑ_FP<عائم, v_float32>(srcmat, dstmat); }

#إذا CV_SIMD_64F
// --- float → double ---
ساكن فراغ reduce_خانہ_جوڑ_32f64f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح cn = srcmat.channels();
    ثابت صحيح چوڑائی = srcmat.خانے_گنتی * cn;
    ثابت صحيح vlanes32 = VTraits<v_float32>::vlanes();

    خودکار body = [&](ثابت Range& range) {
        لكل (صحيح y = range.شروع; y < range.end; y++)
        {
            ثابت عائم* src = srcmat.ptr<عائم>(y);
            مزدوج* dst = dstmat.ptr<مزدوج>(y);

            إذا (cn == 1)
            {
                v_float64 v_sum0 = vx_setzero_f64();
                v_float64 v_sum1 = vx_setzero_f64();
                صحيح x = 0;
                لكل (; x <= چوڑائی - vlanes32; x += vlanes32)
                {
                    v_float32 v = vx_لدو(src + x);
                    v_sum0 = v_جوڑو(v_sum0, v_cvt_f64(v));
                    v_sum1 = v_جوڑو(v_sum1, v_cvt_f64_بلند(v));
                }
                v_float64 v_کل = v_جوڑو(v_sum0, v_sum1);

                مزدوج کل = v_reduce_جوڑ(v_کل);
                لكل (; x < چوڑائی; x++)
                    کل += (مزدوج)src[x];
                dst[0] = کل;
            }
            وإلا
            {
                لكل (صحيح c = 0; c < cn; c++)
                    dst[c] = 0;
                لكل (صحيح x = 0, c = 0; x < چوڑائی; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += (مزدوج)src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.صفاں), body);
    v_cleanup();
}

ساكن فراغ reduce_خانہ_جوڑ_64f64f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_خانہ_جوڑ_FP<مزدوج, v_float64>(srcmat, dstmat); }
#جے_انت // CV_SIMD_64F

// =====================================================================
//  Row reduce SUM (dim=0): sum each column across all rows
// =====================================================================

// --- uchar → int ---
// Uses u16 intermediate accumulator for vertical sum: accumulate u8→u16 per row,
// flush u16→u32 every 256 rows (max u16 value: 256*255=65280 < 65535)
ساكن فراغ reduce_صف_جوڑ_8u32s(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح چوڑائی_cn = srcmat.خانے_گنتی * srcmat.channels();
    ثابت صحيح اچائی = srcmat.صفاں;
    ثابت صحيح vlanes16 = VTraits<v_uint16>::vlanes();
    ثابت صحيح vlanes32 = VTraits<v_int32>::vlanes();

    خودکار body = [&](ثابت Range& range) {
        ثابت صحيح شروع = range.شروع;
        ثابت صحيح end = range.end;
        ثابت صحيح len = end - شروع;

        Auto_بفر<صحيح> buf32_storage(len);
        Auto_بفر<ushort> buf16_storage(len);
        صحيح* buf32 = buf32_storage.انکڑے();
        ushort* buf16 = buf16_storage.انکڑے();

        // init from first row
        ثابت uchar* src0 = srcmat.ptr<uchar>(0) + شروع;
        لكل (صحيح i = 0; i < len; i++)
            buf32[i] = (صحيح)src0[i];

        إذا (اچائی <= 1)
        {
            صحيح* dst = dstmat.ptr<صحيح>(0) + شروع;
            لكل (صحيح i = 0; i < len; i++)
                dst[i] = buf32[i];
            أرجع;
        }

        memset(buf16, 0, len * حجم(ushort));
        ثابت صحيح flush_at = 256; // 256*255 = 65280 < 65535 (u16 safe)
        صحيح flush_گنتی = 0;

        لكل (صحيح صف = 1; صف < اچائی; صف++)
        {
            ثابت uchar* src = srcmat.ptr<uchar>(صف) + شروع;
            صحيح i = 0;

#إذا CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
            // AArch64: vaddw_u8 — single-instruction widening add (u16 += u8)
            لكل (; i <= len - 16; i += 16)
            {
                uint8x16_t v = vld1q_u8(src + i);
                uint16x8_t acc_lo = vld1q_u16(buf16 + i);
                uint16x8_t acc_hi = vld1q_u16(buf16 + i + 8);
                vst1q_u16(buf16 + i,     vaddw_u8(acc_lo, vget_کم_u8(v)));
                vst1q_u16(buf16 + i + 8, vaddw_بلند_u8(acc_hi, v));
            }
#وإلاإذا CV_RVV
            ثابت صحيح vl = (صحيح)__riscv_vsetvlmax_e8m2();
            لكل (; i <= len - vl; i += vl)
            {
                vuint8m2_t vs = __riscv_vle8_v_u8m2(src + i, vl);
                vuint16m4_t va = __riscv_vle16_v_u16m4(buf16 + i, vl);
                va = __riscv_vwaddu_wv_u16m4(va, vs, vl);
                __riscv_vse16_v_u16m4(buf16 + i, va, vl);
            }
#وإلا
            ثابت صحيح vlanes8 = VTraits<v_uint8>::vlanes();
            لكل (; i <= len - vlanes8; i += vlanes8)
            {
                v_uint16 lo, hi;
                v_expand(vx_لدو(src + i), lo, hi);
                v_store(buf16 + i, v_جوڑو(vx_لدو(buf16 + i), lo));
                v_store(buf16 + i + vlanes16, v_جوڑو(vx_لدو(buf16 + i + vlanes16), hi));
            }
#جے_انت
            لكل (; i < len; i++)
                buf16[i] += (ushort)src[i];

            إذا (++flush_گنتی >= flush_at || صف == اچائی - 1)
            {
                // flush u16 → u32
                صحيح j = 0;
                لكل (; j <= len - vlanes16; j += vlanes16)
                {
                    v_uint32 lo, hi;
                    v_expand(vx_لدو(buf16 + j), lo, hi);
                    v_store(buf32 + j, v_جوڑو(vx_لدو(buf32 + j), v_reinterpret_as_s32(lo)));
                    v_store(buf32 + j + vlanes32, v_جوڑو(vx_لدو(buf32 + j + vlanes32), v_reinterpret_as_s32(hi)));
                }
                لكل (; j < len; j++)
                    buf32[j] += (صحيح)buf16[j];
                memset(buf16, 0, len * حجم(ushort));
                flush_گنتی = 0;
            }
        }

        صحيح* dst = dstmat.ptr<صحيح>(0) + شروع;
        لكل (صحيح i = 0; i < len; i++)
            dst[i] = buf32[i];
    };

    parallel_for_(Range(0, چوڑائی_cn), body, چوڑائی_cn * CV_ELEM_ناپ(srcmat.ڈونگھائی()) / 64);
    v_cleanup();
}

// --- uchar → float ---
ساكن فراغ reduce_صف_جوڑ_8u32f(ثابت Mat& srcmat, Mat& dstmat)
{
    // compute in int, then convert to float
    Mat عارضی_2(dstmat.صفاں, dstmat.خانے_گنتی, CV_32SC(srcmat.channels()));
    reduce_صف_جوڑ_8u32s(srcmat, عارضی_2);
    عارضی_2.convert_منزل(dstmat, dstmat.type());
}

// --- 16-bit (ushort/short) → float row reduce ---
سانچا<قسم_ناں SrcT, قسم_ناں VecSrc, قسم_ناں VecDst>
ساكن فراغ reduce_صف_جوڑ_16_32f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح چوڑائی_cn = srcmat.خانے_گنتی * srcmat.channels();
    ثابت صحيح اچائی = srcmat.صفاں;
    ثابت صحيح vlanes16 = VTraits<VecSrc>::vlanes();
    ثابت صحيح vlanes32 = vlanes16 / 2;

    خودکار body = [&](ثابت Range& range) {
        ثابت صحيح شروع = range.شروع;
        ثابت صحيح end = range.end;
        ثابت صحيح len = end - شروع;
        عائم* dst = dstmat.ptr<عائم>(0) + شروع;

        صحيح i = 0;
        لكل (; i <= len - vlanes16 * 4; i += vlanes16 * 4)
        {
            ثابت SrcT* src0 = srcmat.ptr<SrcT>(0) + شروع + i;
            VecDst a0, a1, a2, a3, a4, a5, a6, a7;
            v_expand(vx_لدو(src0), a0, a1);
            v_expand(vx_لدو(src0 + vlanes16), a2, a3);
            v_expand(vx_لدو(src0 + vlanes16*2), a4, a5);
            v_expand(vx_لدو(src0 + vlanes16*3), a6, a7);
            لكل (صحيح صف = 1; صف < اچائی; صف++)
            {
                ثابت SrcT* src = srcmat.ptr<SrcT>(صف) + شروع + i;
                VecDst lo, hi;
                v_expand(vx_لدو(src), lo, hi);
                a0 = v_جوڑو(a0, lo); a1 = v_جوڑو(a1, hi);
                v_expand(vx_لدو(src + vlanes16), lo, hi);
                a2 = v_جوڑو(a2, lo); a3 = v_جوڑو(a3, hi);
                v_expand(vx_لدو(src + vlanes16*2), lo, hi);
                a4 = v_جوڑو(a4, lo); a5 = v_جوڑو(a5, hi);
                v_expand(vx_لدو(src + vlanes16*3), lo, hi);
                a6 = v_جوڑو(a6, lo); a7 = v_جوڑو(a7, hi);
            }
            v_store(dst + i, v_cvt_f32(v_reinterpret_as_s32(a0)));
            v_store(dst + i + vlanes32, v_cvt_f32(v_reinterpret_as_s32(a1)));
            v_store(dst + i + vlanes32*2, v_cvt_f32(v_reinterpret_as_s32(a2)));
            v_store(dst + i + vlanes32*3, v_cvt_f32(v_reinterpret_as_s32(a3)));
            v_store(dst + i + vlanes32*4, v_cvt_f32(v_reinterpret_as_s32(a4)));
            v_store(dst + i + vlanes32*5, v_cvt_f32(v_reinterpret_as_s32(a5)));
            v_store(dst + i + vlanes32*6, v_cvt_f32(v_reinterpret_as_s32(a6)));
            v_store(dst + i + vlanes32*7, v_cvt_f32(v_reinterpret_as_s32(a7)));
        }
        لكل (; i <= len - vlanes16; i += vlanes16)
        {
            VecDst a0, a1;
            v_expand(vx_لدو(srcmat.ptr<SrcT>(0) + شروع + i), a0, a1);
            لكل (صحيح صف = 1; صف < اچائی; صف++)
            {
                VecDst lo, hi;
                v_expand(vx_لدو(srcmat.ptr<SrcT>(صف) + شروع + i), lo, hi);
                a0 = v_جوڑو(a0, lo); a1 = v_جوڑو(a1, hi);
            }
            v_store(dst + i, v_cvt_f32(v_reinterpret_as_s32(a0)));
            v_store(dst + i + vlanes32, v_cvt_f32(v_reinterpret_as_s32(a1)));
        }
        لكل (; i < len; i++)
        {
            صحيح val = (صحيح)*(srcmat.ptr<SrcT>(0) + شروع + i);
            لكل (صحيح صف = 1; صف < اچائی; صف++)
                val += (صحيح)*(srcmat.ptr<SrcT>(صف) + شروع + i);
            dst[i] = (عائم)val;
        }
    };

    parallel_for_(Range(0, چوڑائی_cn), body, چوڑائی_cn * CV_ELEM_ناپ(srcmat.ڈونگھائی()) / 64);
    v_cleanup();
}

ساكن فراغ reduce_صف_جوڑ_16u32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_صف_جوڑ_16_32f<ushort, v_uint16, v_uint32>(srcmat, dstmat); }

ساكن فراغ reduce_صف_جوڑ_16s32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_صف_جوڑ_16_32f<چھوٹا, v_int16, v_int32>(srcmat, dstmat); }

// --- float/double → same type row reduce ---
سانچا<قسم_ناں T, قسم_ناں VecT>
ساكن فراغ reduce_صف_جوڑ_FP(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح چوڑائی_cn = srcmat.خانے_گنتی * srcmat.channels();
    ثابت صحيح اچائی = srcmat.صفاں;
    ثابت صحيح vlanes = VTraits<VecT>::vlanes();

    خودکار body = [&](ثابت Range& range) {
        ثابت صحيح شروع = range.شروع;
        ثابت صحيح end = range.end;
        ثابت صحيح len = end - شروع;
        T* dst = dstmat.ptr<T>(0) + شروع;

        صحيح i = 0;
        لكل (; i <= len - vlanes * 8; i += vlanes * 8)
        {
            ثابت T* src0 = srcmat.ptr<T>(0) + شروع + i;
            VecT s0 = vx_لدو(src0), s1 = vx_لدو(src0 + vlanes);
            VecT s2 = vx_لدو(src0 + vlanes*2), s3 = vx_لدو(src0 + vlanes*3);
            VecT s4 = vx_لدو(src0 + vlanes*4), s5 = vx_لدو(src0 + vlanes*5);
            VecT s6 = vx_لدو(src0 + vlanes*6), s7 = vx_لدو(src0 + vlanes*7);
            لكل (صحيح صف = 1; صف < اچائی; صف++)
            {
                ثابت T* src = srcmat.ptr<T>(صف) + شروع + i;
                s0 = v_جوڑو(s0, vx_لدو(src));
                s1 = v_جوڑو(s1, vx_لدو(src + vlanes));
                s2 = v_جوڑو(s2, vx_لدو(src + vlanes*2));
                s3 = v_جوڑو(s3, vx_لدو(src + vlanes*3));
                s4 = v_جوڑو(s4, vx_لدو(src + vlanes*4));
                s5 = v_جوڑو(s5, vx_لدو(src + vlanes*5));
                s6 = v_جوڑو(s6, vx_لدو(src + vlanes*6));
                s7 = v_جوڑو(s7, vx_لدو(src + vlanes*7));
            }
            v_store(dst + i, s0);
            v_store(dst + i + vlanes, s1);
            v_store(dst + i + vlanes*2, s2);
            v_store(dst + i + vlanes*3, s3);
            v_store(dst + i + vlanes*4, s4);
            v_store(dst + i + vlanes*5, s5);
            v_store(dst + i + vlanes*6, s6);
            v_store(dst + i + vlanes*7, s7);
        }
        لكل (; i <= len - vlanes; i += vlanes)
        {
            VecT s0 = vx_لدو(srcmat.ptr<T>(0) + شروع + i);
            لكل (صحيح صف = 1; صف < اچائی; صف++)
                s0 = v_جوڑو(s0, vx_لدو(srcmat.ptr<T>(صف) + شروع + i));
            v_store(dst + i, s0);
        }
        لكل (; i < len; i++)
        {
            T val = *(srcmat.ptr<T>(0) + شروع + i);
            لكل (صحيح صف = 1; صف < اچائی; صف++)
                val += *(srcmat.ptr<T>(صف) + شروع + i);
            dst[i] = val;
        }
    };

    parallel_for_(Range(0, چوڑائی_cn), body, چوڑائی_cn * CV_ELEM_ناپ(srcmat.ڈونگھائی()) / 64);
    v_cleanup();
}

ساكن فراغ reduce_صف_جوڑ_32f32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_صف_جوڑ_FP<عائم, v_float32>(srcmat, dstmat); }

#إذا CV_SIMD_64F
// --- float → double ---
ساكن فراغ reduce_صف_جوڑ_32f64f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح چوڑائی_cn = srcmat.خانے_گنتی * srcmat.channels();
    ثابت صحيح اچائی = srcmat.صفاں;
    ثابت صحيح vlanes32 = VTraits<v_float32>::vlanes();
    ثابت صحيح vlanes64 = VTraits<v_float64>::vlanes();

    خودکار body = [&](ثابت Range& range) {
        ثابت صحيح شروع = range.شروع;
        ثابت صحيح end = range.end;
        ثابت صحيح len = end - شروع;

        Auto_بفر<مزدوج> buf_storage(len);
        مزدوج* buf = buf_storage.انکڑے();

        ثابت عائم* src0 = srcmat.ptr<عائم>(0) + شروع;
        لكل (صحيح i = 0; i < len; i++)
            buf[i] = (مزدوج)src0[i];

        لكل (صحيح صف = 1; صف < اچائی; صف++)
        {
            ثابت عائم* src = srcmat.ptr<عائم>(صف) + شروع;
            صحيح i = 0;
            لكل (; i <= len - vlanes32; i += vlanes32)
            {
                v_float32 v = vx_لدو(src + i);
                v_store(buf + i, v_جوڑو(vx_لدو(buf + i), v_cvt_f64(v)));
                v_store(buf + i + vlanes64, v_جوڑو(vx_لدو(buf + i + vlanes64), v_cvt_f64_بلند(v)));
            }
            لكل (; i < len; i++)
                buf[i] += (مزدوج)src[i];
        }

        مزدوج* dst = dstmat.ptr<مزدوج>(0) + شروع;
        حافظہ_نقل(dst, buf, len * حجم(مزدوج));
    };

    parallel_for_(Range(0, چوڑائی_cn), body, چوڑائی_cn * CV_ELEM_ناپ(srcmat.ڈونگھائی()) / 64);
    v_cleanup();
}

ساكن فراغ reduce_صف_جوڑ_64f64f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_صف_جوڑ_FP<مزدوج, v_float64>(srcmat, dstmat); }
#جے_انت // CV_SIMD_64F

#جے_انت // CV_SIMD || CV_SIMD_SCALABLE

#إذا CV_SIMD_SCALABLE
ساكن مضمن v_float32 reduce_صف_Sum2_Acc4(ثابت v_float32& acc,
                                           ثابت عائم* src0, ثابت عائم* src1,
                                           ثابت عائم* src2, ثابت عائم* src3)
{
    v_float32 جوڑ = acc;
    v_float32 x = vx_لدو(src0);
    جوڑ = v_fma(x, x, جوڑ);
    x = vx_لدو(src1);
    جوڑ = v_fma(x, x, جوڑ);
    x = vx_لدو(src2);
    جوڑ = v_fma(x, x, جوڑ);
    x = vx_لدو(src3);
    أرجع v_fma(x, x, جوڑ);
}

// dim=0, REDUCE_SUM2 for CV_32F using scalable universal intrinsics.
ساكن فراغ reduce_صف_Sum2_32f32f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح چوڑائی = srcmat.خانے_گنتی * srcmat.channels();
    ثابت صحيح اچائی = srcmat.صفاں;
    ثابت صحيح vlanes = VTraits<v_float32>::vlanes();
    ثابت صحيح بلاک_2 = vlanes * 4;
    Auto_بفر<عائم> بفر(چوڑائی);
    عائم* buf = بفر.انکڑے();

    ثابت عائم* src = srcmat.ptr<عائم>(0);
    صحيح i = 0;
    لكل (; i <= چوڑائی - بلاک_2; i += بلاک_2)
    {
        v_float32 x0 = vx_لدو(src + i);
        v_float32 x1 = vx_لدو(src + i + vlanes);
        v_float32 x2 = vx_لدو(src + i + vlanes * 2);
        v_float32 x3 = vx_لدو(src + i + vlanes * 3);
        v_store(buf + i, v_mul(x0, x0));
        v_store(buf + i + vlanes, v_mul(x1, x1));
        v_store(buf + i + vlanes * 2, v_mul(x2, x2));
        v_store(buf + i + vlanes * 3, v_mul(x3, x3));
    }
    لكل (; i <= چوڑائی - vlanes; i += vlanes)
    {
        v_float32 x = vx_لدو(src + i);
        v_store(buf + i, v_mul(x, x));
    }
    لكل (; i < چوڑائی; i++)
        buf[i] = src[i] * src[i];

    صحيح صف = 1;
    // Four rows per pass amortize buffer traffic while preserving source-row
    // accumulation order. The outer loop stays serial: splitting width into
    // tiny parallel stripes under-fills scalable vectors and adds scheduling
    // overhead for this memory-streaming kernel.
    لكل (; صف <= اچائی - 4; صف += 4)
    {
        ثابت عائم* src0 = srcmat.ptr<عائم>(صف);
        ثابت عائم* src1 = srcmat.ptr<عائم>(صف + 1);
        ثابت عائم* src2 = srcmat.ptr<عائم>(صف + 2);
        ثابت عائم* src3 = srcmat.ptr<عائم>(صف + 3);

        i = 0;
        لكل (; i <= چوڑائی - بلاک_2; i += بلاک_2)
        {
            v_float32 sum0 = vx_لدو(buf + i);
            v_float32 sum1 = vx_لدو(buf + i + vlanes);
            v_float32 sum2 = vx_لدو(buf + i + vlanes * 2);
            v_float32 sum3 = vx_لدو(buf + i + vlanes * 3);
            sum0 = reduce_صف_Sum2_Acc4(sum0, src0 + i, src1 + i, src2 + i, src3 + i);
            sum1 = reduce_صف_Sum2_Acc4(sum1, src0 + i + vlanes, src1 + i + vlanes,
                                     src2 + i + vlanes, src3 + i + vlanes);
            sum2 = reduce_صف_Sum2_Acc4(sum2, src0 + i + vlanes * 2, src1 + i + vlanes * 2,
                                     src2 + i + vlanes * 2, src3 + i + vlanes * 2);
            sum3 = reduce_صف_Sum2_Acc4(sum3, src0 + i + vlanes * 3, src1 + i + vlanes * 3,
                                     src2 + i + vlanes * 3, src3 + i + vlanes * 3);
            v_store(buf + i, sum0);
            v_store(buf + i + vlanes, sum1);
            v_store(buf + i + vlanes * 2, sum2);
            v_store(buf + i + vlanes * 3, sum3);
        }
        لكل (; i <= چوڑائی - vlanes; i += vlanes)
        {
            v_float32 جوڑ = vx_لدو(buf + i);
            جوڑ = reduce_صف_Sum2_Acc4(جوڑ, src0 + i, src1 + i, src2 + i, src3 + i);
            v_store(buf + i, جوڑ);
        }
        لكل (; i < چوڑائی; i++)
        {
            عائم جوڑ = buf[i];
            عائم x = src0[i]; جوڑ += x * x;
            x = src1[i]; جوڑ += x * x;
            x = src2[i]; جوڑ += x * x;
            x = src3[i]; جوڑ += x * x;
            buf[i] = جوڑ;
        }
    }

    لكل (; صف < اچائی; صف++)
    {
        src = srcmat.ptr<عائم>(صف);
        i = 0;
        لكل (; i <= چوڑائی - بلاک_2; i += بلاک_2)
        {
            v_float32 sum0 = vx_لدو(buf + i);
            v_float32 sum1 = vx_لدو(buf + i + vlanes);
            v_float32 sum2 = vx_لدو(buf + i + vlanes * 2);
            v_float32 sum3 = vx_لدو(buf + i + vlanes * 3);
            v_float32 x0 = vx_لدو(src + i);
            v_float32 x1 = vx_لدو(src + i + vlanes);
            v_float32 x2 = vx_لدو(src + i + vlanes * 2);
            v_float32 x3 = vx_لدو(src + i + vlanes * 3);
            v_store(buf + i, v_fma(x0, x0, sum0));
            v_store(buf + i + vlanes, v_fma(x1, x1, sum1));
            v_store(buf + i + vlanes * 2, v_fma(x2, x2, sum2));
            v_store(buf + i + vlanes * 3, v_fma(x3, x3, sum3));
        }
        لكل (; i <= چوڑائی - vlanes; i += vlanes)
        {
            v_float32 x = vx_لدو(src + i);
            v_store(buf + i, v_fma(x, x, vx_لدو(buf + i)));
        }
        لكل (; i < چوڑائی; i++)
            buf[i] += src[i] * src[i];
    }

    // Delay output stores until all source rows are consumed, preserving the
    // existing overlap-safe behavior of the proposed optimized path.
    حافظہ_نقل(dstmat.ptr<عائم>(), buf, چوڑائی * حجم(عائم));
    v_cleanup();
}
#جے_انت

// =====================================================================
//  Dispatchers
// =====================================================================

Reduce_جوڑ_Func getReduceCSumFunc(صحيح sdepth, صحيح ddepth)
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا (sdepth == CV_8U && ddepth == CV_32S) أرجع reduce_خانہ_جوڑ_8u32s;
    إذا (sdepth == CV_8U && ddepth == CV_32F) أرجع reduce_خانہ_جوڑ_8u32f;
    إذا (sdepth == CV_16U && ddepth == CV_32F) أرجع reduce_خانہ_جوڑ_16u32f;
    إذا (sdepth == CV_16S && ddepth == CV_32F) أرجع reduce_خانہ_جوڑ_16s32f;
    إذا (sdepth == CV_32F && ddepth == CV_32F) أرجع reduce_خانہ_جوڑ_32f32f;
#إذا CV_SIMD_64F
    إذا (sdepth == CV_32F && ddepth == CV_64F) أرجع reduce_خانہ_جوڑ_32f64f;
    إذا (sdepth == CV_64F && ddepth == CV_64F) أرجع reduce_خانہ_جوڑ_64f64f;
#جے_انت
#وإلا
    CV_بن_ورتیا(sdepth);
    CV_بن_ورتیا(ddepth);
#جے_انت
    أرجع خالی_سوچک;
}

Reduce_جوڑ_Func getReduceRSumFunc(صحيح sdepth, صحيح ddepth)
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا (sdepth == CV_8U && ddepth == CV_32S) أرجع reduce_صف_جوڑ_8u32s;
    إذا (sdepth == CV_8U && ddepth == CV_32F) أرجع reduce_صف_جوڑ_8u32f;
    إذا (sdepth == CV_16U && ddepth == CV_32F) أرجع reduce_صف_جوڑ_16u32f;
    إذا (sdepth == CV_16S && ddepth == CV_32F) أرجع reduce_صف_جوڑ_16s32f;
    إذا (sdepth == CV_32F && ddepth == CV_32F) أرجع reduce_صف_جوڑ_32f32f;
#إذا CV_SIMD_64F
    إذا (sdepth == CV_32F && ddepth == CV_64F) أرجع reduce_صف_جوڑ_32f64f;
    إذا (sdepth == CV_64F && ddepth == CV_64F) أرجع reduce_صف_جوڑ_64f64f;
#جے_انت
#وإلا
    CV_بن_ورتیا(sdepth);
    CV_بن_ورتیا(ddepth);
#جے_انت
    أرجع خالی_سوچک;
}

Reduce_جوڑ_Func getReduceRSum2Func(صحيح sdepth, صحيح ddepth)
{
#إذا CV_SIMD_SCALABLE
    إذا (sdepth == CV_32F && ddepth == CV_32F) أرجع reduce_صف_Sum2_32f32f;
#وإلا
    CV_بن_ورتیا(sdepth);
    CV_بن_ورتیا(ddepth);
#جے_انت
    أرجع خالی_سوچک;
}

#جے_انت // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_سرا
} // namespace cv
