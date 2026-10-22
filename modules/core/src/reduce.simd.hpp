// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਨਾਮਾਕਾਸ਼ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*Reduce_ਜੋੜ_Func)(ਅਚਲ Mat& src, Mat& dst);
Reduce_ਜੋੜ_Func getReduceCSumFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth);
Reduce_ਜੋੜ_Func getReduceRSumFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth);
Reduce_ਜੋੜ_Func getReduceRSum2Func(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// =====================================================================
//  Col reduce SUM (dim=1): sum each row into cn output values
// =====================================================================

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)

// --- uchar → int ---
// Uses u16 intermediate accumulator to halve the number of widen operations:
// u8→u16 per iteration, u16→u32 flush every 128 iterations (max u16 value: 128*255=32640 < 65535)
ਸਥਿਰ ਖਾਲੀ reduce_ਖਾਨਾ_ਜੋੜ_8u32s(ਅਚਲ Mat& srcmat, Mat& dstmat)
{
    ਅਚਲ ਪੂਰਨ cn = srcmat.channels();
    ਅਚਲ ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ = srcmat.ਖਾਨੇ_ਗਿਣਤੀ;
    ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = ਖਾਨੇ_ਗਿਣਤੀ * cn;

    ਸਵੈਚਾਲਿਤ body = [&](ਅਚਲ Range& range) {
        ਲਈ (ਪੂਰਨ y = range.ਸ਼ੁਰੂ; y < range.end; y++)
        {
            ਅਚਲ uchar* src = srcmat.ptr<uchar>(y);
            ਪੂਰਨ* dst = dstmat.ptr<ਪੂਰਨ>(y);

            ਜੇ (cn == 1)
            {
#ਜੇ defined(CV_NEON_DOT)
                // ARMv8.2 DOTPROD: vdotq_u32(acc, src, ones) — single instruction byte sum
                // u32 lane max 1020/iter, overflow at ~4.2M iter (67MB) — safe without flush
                ਅਚਲ uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t v_ਜੋੜ = vdupq_n_u32(0);
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਚੌੜਾਈ - 16; x += 16)
                    v_ਜੋੜ = vdotq_u32(v_ਜੋੜ, vld1q_u8(src + x), ones);
                ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)vaddvq_u32(v_ਜੋੜ);
                ਲਈ (; x < ਚੌੜਾਈ; x++)
                    ਕੁੱਲ += (ਪੂਰਨ)src[x];
                dst[0] = ਕੁੱਲ;
#ਜਾਂਜੇ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                // AArch64 fallback: vpaddlq chain
                uint32x4_t v_ਜੋੜ = vdupq_n_u32(0);
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਚੌੜਾਈ - 16; x += 16)
                    v_ਜੋੜ = vaddq_u32(v_ਜੋੜ, vpaddlq_u16(vpaddlq_u8(vld1q_u8(src + x))));
                ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)vaddvq_u32(v_ਜੋੜ);
                ਲਈ (; x < ਚੌੜਾਈ; x++)
                    ਕੁੱਲ += (ਪੂਰਨ)src[x];
                dst[0] = ਕੁੱਲ;
#ਜਾਂਜੇ CV_AVX2
                // Intel AVX2: _mm256_sad_epu8 — 32 bytes → 4×u64 sum, 1 cycle
                ਅਚਲ __m256i zero = _mm256_setzero_si256();
                __m256i vsum = zero;
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਚੌੜਾਈ - 32; x += 32)
                    vsum = mm256_ਜੋੜੋ_epi64(vsum, _mm256_sad_epu8(
                        _mm256_loadu_si256((ਅਚਲ __m256i*)(src + x)), zero));
                // 8-byte tail: reduce max scalar tail from 31 to 7
                // Use _mm256_inserti128_si256 instead of _mm256_castsi128_si256:
                // the latter leaves upper 128 bits undefined per Intel spec,
                // causing MSVC to expose stale register data to _mm256_sad_epu8.
                ਲਈ (; x <= ਚੌੜਾਈ - 8; x += 8)
                    vsum = mm256_ਜੋੜੋ_epi64(vsum, _mm256_sad_epu8(
                        _mm256_inserti128_si256(_mm256_setzero_si256(), _mm_loadl_epi64((ਅਚਲ __m128i*)(src + x)), 0), zero));
                __m128i lo128 = _mm256_castsi256_si128(vsum);
                __m128i hi128 = _mm256_extracti128_si256(vsum, 1);
                __m128i s = mm_ਜੋੜੋ_epi64(lo128, hi128);
                s = mm_ਜੋੜੋ_epi64(s, _mm_unpackhi_epi64(s, s));
                ਪੂਰਨ ਕੁੱਲ = _mm_cvtsi128_si32(s);
                ਲਈ (; x < ਚੌੜਾਈ; x++)
                    ਕੁੱਲ += (ਪੂਰਨ)src[x];
                dst[0] = ਕੁੱਲ;
#ਜਾਂਜੇ CV_SSE2
                // Intel SSE2: _mm_sad_epu8 — 16 bytes → 2×u64 sum
                ਅਚਲ __m128i zero = _mm_setzero_si128();
                __m128i vsum = zero;
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਚੌੜਾਈ - 16; x += 16)
                    vsum = mm_ਜੋੜੋ_epi64(vsum, _mm_sad_epu8(
                        _mm_loadu_si128((ਅਚਲ __m128i*)(src + x)), zero));
                __m128i s = mm_ਜੋੜੋ_epi64(vsum, _mm_unpackhi_epi64(vsum, vsum));
                ਪੂਰਨ ਕੁੱਲ = _mm_cvtsi128_si32(s);
                ਲਈ (; x < ਚੌੜਾਈ; x++)
                    ਕੁੱਲ += (ਪੂਰਨ)src[x];
                dst[0] = ਕੁੱਲ;
#ਨਹੀਂਤਾਂ
                ਅਚਲ ਪੂਰਨ vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 v_ਜੋੜ = vx_setzero_u32();
                v_uint16 v_sum16 = vx_setzero_u16();
                ਪੂਰਨ x = 0, batch = 0;
                ਅਚਲ ਪੂਰਨ flush_at = 128;

                ਲਈ (; x <= ਚੌੜਾਈ - vlanes8; x += vlanes8)
                {
                    v_uint16 lo, hi;
                    v_expand(vx_ਲੱਦੋ(src + x), lo, hi);
                    v_sum16 = v_ਜੋੜੋ(v_sum16, v_ਜੋੜੋ(lo, hi));

                    ਜੇ (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(v_sum16, a, b);
                        v_ਜੋੜ = v_ਜੋੜੋ(v_ਜੋੜ, v_ਜੋੜੋ(a, b));
                        v_sum16 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(v_sum16, a, b);
                    v_ਜੋੜ = v_ਜੋੜੋ(v_ਜੋੜ, v_ਜੋੜੋ(a, b));
                }

                ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)v_reduce_ਜੋੜ(v_ਜੋੜ);
                ਲਈ (; x < ਚੌੜਾਈ; x++)
                    ਕੁੱਲ += (ਪੂਰਨ)src[x];
                dst[0] = ਕੁੱਲ;
#ਜੇ_ਅੰਤ
            }
            ਨਹੀਂਤਾਂ ਜੇ (cn == 4)
            {
#ਜੇ defined(CV_NEON_DOT)
                // ARMv8.2 DOTPROD: vld4q_u8 deinterleave + vdotq_u32 per-channel
                // vld4q_u8 reads 16 pixels (64 bytes), deinterleaves into 4 × u8x16
                // vdotq_u32 sums each channel's 16 bytes into 4 partial u32 sums
                ਅਚਲ uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t acc0 = vdupq_n_u32(0), acc1 = vdupq_n_u32(0);
                uint32x4_t acc2 = vdupq_n_u32(0), acc3 = vdupq_n_u32(0);
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਖਾਨੇ_ਗਿਣਤੀ - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    acc0 = vdotq_u32(acc0, v.val[0], ones);
                    acc1 = vdotq_u32(acc1, v.val[1], ones);
                    acc2 = vdotq_u32(acc2, v.val[2], ones);
                    acc3 = vdotq_u32(acc3, v.val[3], ones);
                }
                dst[0] = (ਪੂਰਨ)vaddvq_u32(acc0);
                dst[1] = (ਪੂਰਨ)vaddvq_u32(acc1);
                dst[2] = (ਪੂਰਨ)vaddvq_u32(acc2);
                dst[3] = (ਪੂਰਨ)vaddvq_u32(acc3);
                ਲਈ (; x < ਖਾਨੇ_ਗਿਣਤੀ; x++)
                {
                    dst[0] += (ਪੂਰਨ)src[x * 4];
                    dst[1] += (ਪੂਰਨ)src[x * 4 + 1];
                    dst[2] += (ਪੂਰਨ)src[x * 4 + 2];
                    dst[3] += (ਪੂਰਨ)src[x * 4 + 3];
                }
#ਜਾਂਜੇ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                // AArch64 fallback: vld4q_u8 deinterleave + vaddlvq_u8 per-channel
                ਪੂਰਨ sums[4] = {0, 0, 0, 0};
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਖਾਨੇ_ਗਿਣਤੀ - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    sums[0] += (ਪੂਰਨ)vaddlvq_u8(v.val[0]);
                    sums[1] += (ਪੂਰਨ)vaddlvq_u8(v.val[1]);
                    sums[2] += (ਪੂਰਨ)vaddlvq_u8(v.val[2]);
                    sums[3] += (ਪੂਰਨ)vaddlvq_u8(v.val[3]);
                }
                dst[0] = sums[0]; dst[1] = sums[1];
                dst[2] = sums[2]; dst[3] = sums[3];
                ਲਈ (; x < ਖਾਨੇ_ਗਿਣਤੀ; x++)
                {
                    dst[0] += (ਪੂਰਨ)src[x * 4];
                    dst[1] += (ਪੂਰਨ)src[x * 4 + 1];
                    dst[2] += (ਪੂਰਨ)src[x * 4 + 2];
                    dst[3] += (ਪੂਰਨ)src[x * 4 + 3];
                }
#ਜਾਂਜੇ CV_AVX2
                // Intel AVX2: shuffle + unpack to group channels, full-register SAD
                ਅਚਲ __m256i zero = _mm256_setzero_si256();
                ਅਚਲ __m256i shuf_ਨਕਾਬ = _mm256_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                ਪੂਰਨ x = 0;
                // 16 pixels/iter with unpack (full-register SAD)
                {
                    __m256i ਜੋੜ_bg = zero, ਜੋੜ_ra = zero;
                    ਲਈ (; x <= ਖਾਨੇ_ਗਿਣਤੀ - 16; x += 16)
                    {
                        __m256i v0 = _mm256_loadu_si256((ਅਚਲ __m256i*)(src + x * 4));
                        __m256i v1 = _mm256_loadu_si256((ਅਚਲ __m256i*)(src + x * 4 + 32));
                        __m256i g0 = _mm256_shuffle_epi8(v0, shuf_ਨਕਾਬ);
                        __m256i g1 = _mm256_shuffle_epi8(v1, shuf_ਨਕਾਬ);
                        __m256i bg = _mm256_unpacklo_epi32(g0, g1);
                        __m256i ra = _mm256_unpackhi_epi32(g0, g1);
                        ਜੋੜ_bg = mm256_ਜੋੜੋ_epi64(ਜੋੜ_bg, _mm256_sad_epu8(bg, zero));
                        ਜੋੜ_ra = mm256_ਜੋੜੋ_epi64(ਜੋੜ_ra, _mm256_sad_epu8(ra, zero));
                    }
                    __m128i bg_lo = _mm256_castsi256_si128(ਜੋੜ_bg);
                    __m128i bg_hi = _mm256_extracti128_si256(ਜੋੜ_bg, 1);
                    __m128i bg_s = mm_ਜੋੜੋ_epi64(bg_lo, bg_hi);
                    __m128i ra_lo = _mm256_castsi256_si128(ਜੋੜ_ra);
                    __m128i ra_hi = _mm256_extracti128_si256(ਜੋੜ_ra, 1);
                    __m128i ra_s = mm_ਜੋੜੋ_epi64(ra_lo, ra_hi);
                    dst[0] = _mm_cvtsi128_si32(bg_s);                     // B
                    dst[1] = _mm_cvtsi128_si32(_mm_srli_si128(bg_s, 8));  // G
                    dst[2] = _mm_cvtsi128_si32(ra_s);                     // R
                    dst[3] = _mm_cvtsi128_si32(_mm_srli_si128(ra_s, 8));  // A
                }
                // 8-pixel tail with and/srli
                {
                    ਅਚਲ __m256i ਨਕਾਬ_lo32 = _mm256_set1_epi64x(0x00000000FFFFFFFFLL);
                    __m256i ਜੋੜ_br = zero, ਜੋੜ_ga = zero;
                    ਲਈ (; x <= ਖਾਨੇ_ਗਿਣਤੀ - 8; x += 8)
                    {
                        __m256i v = _mm256_loadu_si256((ਅਚਲ __m256i*)(src + x * 4));
                        __m256i grouped = _mm256_shuffle_epi8(v, shuf_ਨਕਾਬ);
                        __m256i br = _mm256_and_si256(grouped, ਨਕਾਬ_lo32);
                        __m256i ga = _mm256_srli_epi64(grouped, 32);
                        ਜੋੜ_br = mm256_ਜੋੜੋ_epi64(ਜੋੜ_br, _mm256_sad_epu8(br, zero));
                        ਜੋੜ_ga = mm256_ਜੋੜੋ_epi64(ਜੋੜ_ga, _mm256_sad_epu8(ga, zero));
                    }
                    __m128i br_lo = _mm256_castsi256_si128(ਜੋੜ_br);
                    __m128i br_hi = _mm256_extracti128_si256(ਜੋੜ_br, 1);
                    __m128i br_s = mm_ਜੋੜੋ_epi64(br_lo, br_hi);
                    __m128i ga_lo = _mm256_castsi256_si128(ਜੋੜ_ga);
                    __m128i ga_hi = _mm256_extracti128_si256(ਜੋੜ_ga, 1);
                    __m128i ga_s = mm_ਜੋੜੋ_epi64(ga_lo, ga_hi);
                    dst[0] += _mm_cvtsi128_si32(br_s);
                    dst[1] += _mm_cvtsi128_si32(ga_s);
                    dst[2] += _mm_cvtsi128_si32(_mm_srli_si128(br_s, 8));
                    dst[3] += _mm_cvtsi128_si32(_mm_srli_si128(ga_s, 8));
                }
                ਲਈ (; x < ਖਾਨੇ_ਗਿਣਤੀ; x++)
                {
                    dst[0] += (ਪੂਰਨ)src[x * 4];
                    dst[1] += (ਪੂਰਨ)src[x * 4 + 1];
                    dst[2] += (ਪੂਰਨ)src[x * 4 + 2];
                    dst[3] += (ਪੂਰਨ)src[x * 4 + 3];
                }
#ਜਾਂਜੇ CV_SSSE3
                // Intel SSSE3: _mm_shuffle_epi8 deinterleave + _mm_sad_epu8
                // 16 bytes = 4 pixels × 4 channels per iteration
                ਅਚਲ __m128i zero = _mm_setzero_si128();
                ਅਚਲ __m128i shuf_ਨਕਾਬ = _mm_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                ਅਚਲ __m128i ਨਕਾਬ_lo32 = _mm_set1_epi64x(0x00000000FFFFFFFFLL);
                __m128i ਜੋੜ_br = zero, ਜੋੜ_ga = zero;
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਖਾਨੇ_ਗਿਣਤੀ - 4; x += 4)
                {
                    __m128i v = _mm_loadu_si128((ਅਚਲ __m128i*)(src + x * 4));
                    __m128i grouped = _mm_shuffle_epi8(v, shuf_ਨਕਾਬ);
                    __m128i br = _mm_and_si128(grouped, ਨਕਾਬ_lo32);
                    __m128i ga = _mm_srli_epi64(grouped, 32);
                    ਜੋੜ_br = mm_ਜੋੜੋ_epi64(ਜੋੜ_br, _mm_sad_epu8(br, zero));
                    ਜੋੜ_ga = mm_ਜੋੜੋ_epi64(ਜੋੜ_ga, _mm_sad_epu8(ga, zero));
                }
                dst[0] = _mm_cvtsi128_si32(ਜੋੜ_br);
                dst[1] = _mm_cvtsi128_si32(ਜੋੜ_ga);
                dst[2] = _mm_cvtsi128_si32(_mm_srli_si128(ਜੋੜ_br, 8));
                dst[3] = _mm_cvtsi128_si32(_mm_srli_si128(ਜੋੜ_ga, 8));
                ਲਈ (; x < ਖਾਨੇ_ਗਿਣਤੀ; x++)
                {
                    dst[0] += (ਪੂਰਨ)src[x * 4];
                    dst[1] += (ਪੂਰਨ)src[x * 4 + 1];
                    dst[2] += (ਪੂਰਨ)src[x * 4 + 2];
                    dst[3] += (ਪੂਰਨ)src[x * 4 + 3];
                }
#ਨਹੀਂਤਾਂ
                ਅਚਲ ਪੂਰਨ vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 vsum0 = vx_setzero_u32(), vsum1 = vx_setzero_u32();
                v_uint32 vsum2 = vx_setzero_u32(), vsum3 = vx_setzero_u32();
                v_uint16 vs16_0 = vx_setzero_u16(), vs16_1 = vx_setzero_u16();
                v_uint16 vs16_2 = vx_setzero_u16(), vs16_3 = vx_setzero_u16();
                ਪੂਰਨ x = 0, batch = 0;
                ਅਚਲ ਪੂਰਨ flush_at = 128;

                ਲਈ (; x <= ਖਾਨੇ_ਗਿਣਤੀ - vlanes8; x += vlanes8)
                {
                    v_uint8 ch0, ch1, ch2, ch3;
                    v_ਲੱਦੋ_deinterleave(src + x * 4, ch0, ch1, ch2, ch3);

                    v_uint16 lo, hi;
                    v_expand(ch0, lo, hi); vs16_0 = v_ਜੋੜੋ(vs16_0, v_ਜੋੜੋ(lo, hi));
                    v_expand(ch1, lo, hi); vs16_1 = v_ਜੋੜੋ(vs16_1, v_ਜੋੜੋ(lo, hi));
                    v_expand(ch2, lo, hi); vs16_2 = v_ਜੋੜੋ(vs16_2, v_ਜੋੜੋ(lo, hi));
                    v_expand(ch3, lo, hi); vs16_3 = v_ਜੋੜੋ(vs16_3, v_ਜੋੜੋ(lo, hi));

                    ਜੇ (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(vs16_0, a, b); vsum0 = v_ਜੋੜੋ(vsum0, v_ਜੋੜੋ(a, b));
                        v_expand(vs16_1, a, b); vsum1 = v_ਜੋੜੋ(vsum1, v_ਜੋੜੋ(a, b));
                        v_expand(vs16_2, a, b); vsum2 = v_ਜੋੜੋ(vsum2, v_ਜੋੜੋ(a, b));
                        v_expand(vs16_3, a, b); vsum3 = v_ਜੋੜੋ(vsum3, v_ਜੋੜੋ(a, b));
                        vs16_0 = vx_setzero_u16(); vs16_1 = vx_setzero_u16();
                        vs16_2 = vx_setzero_u16(); vs16_3 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(vs16_0, a, b); vsum0 = v_ਜੋੜੋ(vsum0, v_ਜੋੜੋ(a, b));
                    v_expand(vs16_1, a, b); vsum1 = v_ਜੋੜੋ(vsum1, v_ਜੋੜੋ(a, b));
                    v_expand(vs16_2, a, b); vsum2 = v_ਜੋੜੋ(vsum2, v_ਜੋੜੋ(a, b));
                    v_expand(vs16_3, a, b); vsum3 = v_ਜੋੜੋ(vsum3, v_ਜੋੜੋ(a, b));
                }

                dst[0] = (ਪੂਰਨ)v_reduce_ਜੋੜ(vsum0);
                dst[1] = (ਪੂਰਨ)v_reduce_ਜੋੜ(vsum1);
                dst[2] = (ਪੂਰਨ)v_reduce_ਜੋੜ(vsum2);
                dst[3] = (ਪੂਰਨ)v_reduce_ਜੋੜ(vsum3);

                ਲਈ (; x < ਖਾਨੇ_ਗਿਣਤੀ; x++)
                {
                    dst[0] += (ਪੂਰਨ)src[x * 4];
                    dst[1] += (ਪੂਰਨ)src[x * 4 + 1];
                    dst[2] += (ਪੂਰਨ)src[x * 4 + 2];
                    dst[3] += (ਪੂਰਨ)src[x * 4 + 3];
                }
#ਜੇ_ਅੰਤ
            }
            ਨਹੀਂਤਾਂ
            {
                // generic cn: scalar fallback
                ਲਈ (ਪੂਰਨ c = 0; c < cn; c++)
                    dst[c] = (ਪੂਰਨ)src[c];
                ਲਈ (ਪੂਰਨ x = cn; x < ਚੌੜਾਈ; x += cn)
                    ਲਈ (ਪੂਰਨ c = 0; c < cn; c++)
                        dst[c] += (ਪੂਰਨ)src[x + c];
            }
        }
    };

    parallel_for_(Range(0, srcmat.ਪੰਕਤੀਆਂ), body);
    v_cleanup();
}

// --- uchar → float ---
ਸਥਿਰ ਖਾਲੀ reduce_ਖਾਨਾ_ਜੋੜ_8u32f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{
    ਅਚਲ ਪੂਰਨ cn = srcmat.channels();
    ਅਚਲ ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ = srcmat.ਖਾਨੇ_ਗਿਣਤੀ;
    ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = ਖਾਨੇ_ਗਿਣਤੀ * cn;

    ਸਵੈਚਾਲਿਤ body = [&](ਅਚਲ Range& range) {
        ਲਈ (ਪੂਰਨ y = range.ਸ਼ੁਰੂ; y < range.end; y++)
        {
            ਅਚਲ uchar* src = srcmat.ptr<uchar>(y);
            ਦਸ਼ਮਲਵ* dst = dstmat.ptr<ਦਸ਼ਮਲਵ>(y);

            // compute in int, then convert to float
            Auto_ਬਫਰ<ਪੂਰਨ> ibuf(cn);
            ਪੂਰਨ* sums = ibuf.ਅੰਕੜੇ();
            ਲਈ (ਪੂਰਨ c = 0; c < cn; c++)
                sums[c] = 0;

            ਜੇ (cn == 1)
            {
#ਜੇ defined(CV_NEON_DOT)
                ਅਚਲ uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t v_ਜੋੜ = vdupq_n_u32(0);
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਚੌੜਾਈ - 16; x += 16)
                    v_ਜੋੜ = vdotq_u32(v_ਜੋੜ, vld1q_u8(src + x), ones);
                sums[0] = (ਪੂਰਨ)vaddvq_u32(v_ਜੋੜ);
                ਲਈ (; x < ਚੌੜਾਈ; x++)
                    sums[0] += (ਪੂਰਨ)src[x];
#ਜਾਂਜੇ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                uint32x4_t v_ਜੋੜ = vdupq_n_u32(0);
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਚੌੜਾਈ - 16; x += 16)
                    v_ਜੋੜ = vaddq_u32(v_ਜੋੜ, vpaddlq_u16(vpaddlq_u8(vld1q_u8(src + x))));
                sums[0] = (ਪੂਰਨ)vaddvq_u32(v_ਜੋੜ);
                ਲਈ (; x < ਚੌੜਾਈ; x++)
                    sums[0] += (ਪੂਰਨ)src[x];
#ਜਾਂਜੇ CV_AVX2
                ਅਚਲ __m256i zero = _mm256_setzero_si256();
                __m256i vsum = zero;
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਚੌੜਾਈ - 32; x += 32)
                    vsum = mm256_ਜੋੜੋ_epi64(vsum, _mm256_sad_epu8(
                        _mm256_loadu_si256((ਅਚਲ __m256i*)(src + x)), zero));
                // 8-byte tail: reduce max scalar tail from 31 to 7
                // Use _mm256_inserti128_si256 instead of _mm256_castsi128_si256:
                // the latter leaves upper 128 bits undefined per Intel spec,
                // causing MSVC to expose stale register data to _mm256_sad_epu8.
                ਲਈ (; x <= ਚੌੜਾਈ - 8; x += 8)
                    vsum = mm256_ਜੋੜੋ_epi64(vsum, _mm256_sad_epu8(
                        _mm256_inserti128_si256(_mm256_setzero_si256(), _mm_loadl_epi64((ਅਚਲ __m128i*)(src + x)), 0), zero));
                __m128i lo128 = _mm256_castsi256_si128(vsum);
                __m128i hi128 = _mm256_extracti128_si256(vsum, 1);
                __m128i s = mm_ਜੋੜੋ_epi64(lo128, hi128);
                s = mm_ਜੋੜੋ_epi64(s, _mm_unpackhi_epi64(s, s));
                sums[0] = _mm_cvtsi128_si32(s);
                ਲਈ (; x < ਚੌੜਾਈ; x++)
                    sums[0] += (ਪੂਰਨ)src[x];
#ਜਾਂਜੇ CV_SSE2
                ਅਚਲ __m128i zero = _mm_setzero_si128();
                __m128i vsum = zero;
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਚੌੜਾਈ - 16; x += 16)
                    vsum = mm_ਜੋੜੋ_epi64(vsum, _mm_sad_epu8(
                        _mm_loadu_si128((ਅਚਲ __m128i*)(src + x)), zero));
                __m128i s = mm_ਜੋੜੋ_epi64(vsum, _mm_unpackhi_epi64(vsum, vsum));
                sums[0] = _mm_cvtsi128_si32(s);
                ਲਈ (; x < ਚੌੜਾਈ; x++)
                    sums[0] += (ਪੂਰਨ)src[x];
#ਨਹੀਂਤਾਂ
                ਅਚਲ ਪੂਰਨ vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 v_ਜੋੜ = vx_setzero_u32();
                v_uint16 v_sum16 = vx_setzero_u16();
                ਪੂਰਨ x = 0, batch = 0;

                ਲਈ (; x <= ਚੌੜਾਈ - vlanes8; x += vlanes8)
                {
                    v_uint16 lo, hi;
                    v_expand(vx_ਲੱਦੋ(src + x), lo, hi);
                    v_sum16 = v_ਜੋੜੋ(v_sum16, v_ਜੋੜੋ(lo, hi));
                    ਜੇ (++batch >= 128)
                    {
                        v_uint32 a, b;
                        v_expand(v_sum16, a, b);
                        v_ਜੋੜ = v_ਜੋੜੋ(v_ਜੋੜ, v_ਜੋੜੋ(a, b));
                        v_sum16 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                v_uint32 a, b;
                v_expand(v_sum16, a, b);
                v_ਜੋੜ = v_ਜੋੜੋ(v_ਜੋੜ, v_ਜੋੜੋ(a, b));

                sums[0] = (ਪੂਰਨ)v_reduce_ਜੋੜ(v_ਜੋੜ);
                ਲਈ (; x < ਚੌੜਾਈ; x++)
                    sums[0] += (ਪੂਰਨ)src[x];
#ਜੇ_ਅੰਤ
            }
            ਨਹੀਂਤਾਂ ਜੇ (cn == 4)
            {
#ਜੇ defined(CV_NEON_DOT)
                ਅਚਲ uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t acc0 = vdupq_n_u32(0), acc1 = vdupq_n_u32(0);
                uint32x4_t acc2 = vdupq_n_u32(0), acc3 = vdupq_n_u32(0);
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਖਾਨੇ_ਗਿਣਤੀ - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    acc0 = vdotq_u32(acc0, v.val[0], ones);
                    acc1 = vdotq_u32(acc1, v.val[1], ones);
                    acc2 = vdotq_u32(acc2, v.val[2], ones);
                    acc3 = vdotq_u32(acc3, v.val[3], ones);
                }
                sums[0] = (ਪੂਰਨ)vaddvq_u32(acc0);
                sums[1] = (ਪੂਰਨ)vaddvq_u32(acc1);
                sums[2] = (ਪੂਰਨ)vaddvq_u32(acc2);
                sums[3] = (ਪੂਰਨ)vaddvq_u32(acc3);
                ਲਈ (; x < ਖਾਨੇ_ਗਿਣਤੀ; x++)
                {
                    sums[0] += (ਪੂਰਨ)src[x * 4];
                    sums[1] += (ਪੂਰਨ)src[x * 4 + 1];
                    sums[2] += (ਪੂਰਨ)src[x * 4 + 2];
                    sums[3] += (ਪੂਰਨ)src[x * 4 + 3];
                }
#ਜਾਂਜੇ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਖਾਨੇ_ਗਿਣਤੀ - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    sums[0] += (ਪੂਰਨ)vaddlvq_u8(v.val[0]);
                    sums[1] += (ਪੂਰਨ)vaddlvq_u8(v.val[1]);
                    sums[2] += (ਪੂਰਨ)vaddlvq_u8(v.val[2]);
                    sums[3] += (ਪੂਰਨ)vaddlvq_u8(v.val[3]);
                }
                ਲਈ (; x < ਖਾਨੇ_ਗਿਣਤੀ; x++)
                {
                    sums[0] += (ਪੂਰਨ)src[x * 4];
                    sums[1] += (ਪੂਰਨ)src[x * 4 + 1];
                    sums[2] += (ਪੂਰਨ)src[x * 4 + 2];
                    sums[3] += (ਪੂਰਨ)src[x * 4 + 3];
                }
#ਜਾਂਜੇ CV_AVX2
                // Intel AVX2: shuffle + unpack to group channels, full-register SAD
                ਅਚਲ __m256i zero = _mm256_setzero_si256();
                ਅਚਲ __m256i shuf_ਨਕਾਬ = _mm256_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                ਪੂਰਨ x = 0;
                // 16 pixels/iter with unpack (full-register SAD)
                {
                    __m256i ਜੋੜ_bg = zero, ਜੋੜ_ra = zero;
                    ਲਈ (; x <= ਖਾਨੇ_ਗਿਣਤੀ - 16; x += 16)
                    {
                        __m256i v0 = _mm256_loadu_si256((ਅਚਲ __m256i*)(src + x * 4));
                        __m256i v1 = _mm256_loadu_si256((ਅਚਲ __m256i*)(src + x * 4 + 32));
                        __m256i g0 = _mm256_shuffle_epi8(v0, shuf_ਨਕਾਬ);
                        __m256i g1 = _mm256_shuffle_epi8(v1, shuf_ਨਕਾਬ);
                        __m256i bg = _mm256_unpacklo_epi32(g0, g1);
                        __m256i ra = _mm256_unpackhi_epi32(g0, g1);
                        ਜੋੜ_bg = mm256_ਜੋੜੋ_epi64(ਜੋੜ_bg, _mm256_sad_epu8(bg, zero));
                        ਜੋੜ_ra = mm256_ਜੋੜੋ_epi64(ਜੋੜ_ra, _mm256_sad_epu8(ra, zero));
                    }
                    __m128i bg_lo = _mm256_castsi256_si128(ਜੋੜ_bg);
                    __m128i bg_hi = _mm256_extracti128_si256(ਜੋੜ_bg, 1);
                    __m128i bg_s = mm_ਜੋੜੋ_epi64(bg_lo, bg_hi);
                    __m128i ra_lo = _mm256_castsi256_si128(ਜੋੜ_ra);
                    __m128i ra_hi = _mm256_extracti128_si256(ਜੋੜ_ra, 1);
                    __m128i ra_s = mm_ਜੋੜੋ_epi64(ra_lo, ra_hi);
                    sums[0] = _mm_cvtsi128_si32(bg_s);                     // B
                    sums[1] = _mm_cvtsi128_si32(_mm_srli_si128(bg_s, 8));  // G
                    sums[2] = _mm_cvtsi128_si32(ra_s);                     // R
                    sums[3] = _mm_cvtsi128_si32(_mm_srli_si128(ra_s, 8));  // A
                }
                // 8-pixel tail with and/srli
                {
                    ਅਚਲ __m256i ਨਕਾਬ_lo32 = _mm256_set1_epi64x(0x00000000FFFFFFFFLL);
                    __m256i ਜੋੜ_br = zero, ਜੋੜ_ga = zero;
                    ਲਈ (; x <= ਖਾਨੇ_ਗਿਣਤੀ - 8; x += 8)
                    {
                        __m256i v = _mm256_loadu_si256((ਅਚਲ __m256i*)(src + x * 4));
                        __m256i grouped = _mm256_shuffle_epi8(v, shuf_ਨਕਾਬ);
                        __m256i br = _mm256_and_si256(grouped, ਨਕਾਬ_lo32);
                        __m256i ga = _mm256_srli_epi64(grouped, 32);
                        ਜੋੜ_br = mm256_ਜੋੜੋ_epi64(ਜੋੜ_br, _mm256_sad_epu8(br, zero));
                        ਜੋੜ_ga = mm256_ਜੋੜੋ_epi64(ਜੋੜ_ga, _mm256_sad_epu8(ga, zero));
                    }
                    __m128i br_lo = _mm256_castsi256_si128(ਜੋੜ_br);
                    __m128i br_hi = _mm256_extracti128_si256(ਜੋੜ_br, 1);
                    __m128i br_s = mm_ਜੋੜੋ_epi64(br_lo, br_hi);
                    __m128i ga_lo = _mm256_castsi256_si128(ਜੋੜ_ga);
                    __m128i ga_hi = _mm256_extracti128_si256(ਜੋੜ_ga, 1);
                    __m128i ga_s = mm_ਜੋੜੋ_epi64(ga_lo, ga_hi);
                    sums[0] += _mm_cvtsi128_si32(br_s);
                    sums[1] += _mm_cvtsi128_si32(ga_s);
                    sums[2] += _mm_cvtsi128_si32(_mm_srli_si128(br_s, 8));
                    sums[3] += _mm_cvtsi128_si32(_mm_srli_si128(ga_s, 8));
                }
                ਲਈ (; x < ਖਾਨੇ_ਗਿਣਤੀ; x++)
                {
                    sums[0] += (ਪੂਰਨ)src[x * 4];
                    sums[1] += (ਪੂਰਨ)src[x * 4 + 1];
                    sums[2] += (ਪੂਰਨ)src[x * 4 + 2];
                    sums[3] += (ਪੂਰਨ)src[x * 4 + 3];
                }
#ਜਾਂਜੇ CV_SSSE3
                // Intel SSSE3: _mm_shuffle_epi8 deinterleave + _mm_sad_epu8
                ਅਚਲ __m128i zero = _mm_setzero_si128();
                ਅਚਲ __m128i shuf_ਨਕਾਬ = _mm_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                ਅਚਲ __m128i ਨਕਾਬ_lo32 = _mm_set1_epi64x(0x00000000FFFFFFFFLL);
                __m128i ਜੋੜ_br = zero, ਜੋੜ_ga = zero;
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਖਾਨੇ_ਗਿਣਤੀ - 4; x += 4)
                {
                    __m128i v = _mm_loadu_si128((ਅਚਲ __m128i*)(src + x * 4));
                    __m128i grouped = _mm_shuffle_epi8(v, shuf_ਨਕਾਬ);
                    __m128i br = _mm_and_si128(grouped, ਨਕਾਬ_lo32);
                    __m128i ga = _mm_srli_epi64(grouped, 32);
                    ਜੋੜ_br = mm_ਜੋੜੋ_epi64(ਜੋੜ_br, _mm_sad_epu8(br, zero));
                    ਜੋੜ_ga = mm_ਜੋੜੋ_epi64(ਜੋੜ_ga, _mm_sad_epu8(ga, zero));
                }
                sums[0] = _mm_cvtsi128_si32(ਜੋੜ_br);
                sums[1] = _mm_cvtsi128_si32(ਜੋੜ_ga);
                sums[2] = _mm_cvtsi128_si32(_mm_srli_si128(ਜੋੜ_br, 8));
                sums[3] = _mm_cvtsi128_si32(_mm_srli_si128(ਜੋੜ_ga, 8));
                ਲਈ (; x < ਖਾਨੇ_ਗਿਣਤੀ; x++)
                {
                    sums[0] += (ਪੂਰਨ)src[x * 4];
                    sums[1] += (ਪੂਰਨ)src[x * 4 + 1];
                    sums[2] += (ਪੂਰਨ)src[x * 4 + 2];
                    sums[3] += (ਪੂਰਨ)src[x * 4 + 3];
                }
#ਨਹੀਂਤਾਂ
                ਅਚਲ ਪੂਰਨ vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 vsum0 = vx_setzero_u32(), vsum1 = vx_setzero_u32();
                v_uint32 vsum2 = vx_setzero_u32(), vsum3 = vx_setzero_u32();
                v_uint16 vs16_0 = vx_setzero_u16(), vs16_1 = vx_setzero_u16();
                v_uint16 vs16_2 = vx_setzero_u16(), vs16_3 = vx_setzero_u16();
                ਪੂਰਨ x = 0, batch = 0;
                ਅਚਲ ਪੂਰਨ flush_at = 128;

                ਲਈ (; x <= ਖਾਨੇ_ਗਿਣਤੀ - vlanes8; x += vlanes8)
                {
                    v_uint8 ch0, ch1, ch2, ch3;
                    v_ਲੱਦੋ_deinterleave(src + x * 4, ch0, ch1, ch2, ch3);

                    v_uint16 lo, hi;
                    v_expand(ch0, lo, hi); vs16_0 = v_ਜੋੜੋ(vs16_0, v_ਜੋੜੋ(lo, hi));
                    v_expand(ch1, lo, hi); vs16_1 = v_ਜੋੜੋ(vs16_1, v_ਜੋੜੋ(lo, hi));
                    v_expand(ch2, lo, hi); vs16_2 = v_ਜੋੜੋ(vs16_2, v_ਜੋੜੋ(lo, hi));
                    v_expand(ch3, lo, hi); vs16_3 = v_ਜੋੜੋ(vs16_3, v_ਜੋੜੋ(lo, hi));

                    ਜੇ (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(vs16_0, a, b); vsum0 = v_ਜੋੜੋ(vsum0, v_ਜੋੜੋ(a, b));
                        v_expand(vs16_1, a, b); vsum1 = v_ਜੋੜੋ(vsum1, v_ਜੋੜੋ(a, b));
                        v_expand(vs16_2, a, b); vsum2 = v_ਜੋੜੋ(vsum2, v_ਜੋੜੋ(a, b));
                        v_expand(vs16_3, a, b); vsum3 = v_ਜੋੜੋ(vsum3, v_ਜੋੜੋ(a, b));
                        vs16_0 = vx_setzero_u16(); vs16_1 = vx_setzero_u16();
                        vs16_2 = vx_setzero_u16(); vs16_3 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(vs16_0, a, b); vsum0 = v_ਜੋੜੋ(vsum0, v_ਜੋੜੋ(a, b));
                    v_expand(vs16_1, a, b); vsum1 = v_ਜੋੜੋ(vsum1, v_ਜੋੜੋ(a, b));
                    v_expand(vs16_2, a, b); vsum2 = v_ਜੋੜੋ(vsum2, v_ਜੋੜੋ(a, b));
                    v_expand(vs16_3, a, b); vsum3 = v_ਜੋੜੋ(vsum3, v_ਜੋੜੋ(a, b));
                }

                sums[0] = (ਪੂਰਨ)v_reduce_ਜੋੜ(vsum0);
                sums[1] = (ਪੂਰਨ)v_reduce_ਜੋੜ(vsum1);
                sums[2] = (ਪੂਰਨ)v_reduce_ਜੋੜ(vsum2);
                sums[3] = (ਪੂਰਨ)v_reduce_ਜੋੜ(vsum3);

                ਲਈ (; x < ਖਾਨੇ_ਗਿਣਤੀ; x++)
                {
                    sums[0] += (ਪੂਰਨ)src[x * 4];
                    sums[1] += (ਪੂਰਨ)src[x * 4 + 1];
                    sums[2] += (ਪੂਰਨ)src[x * 4 + 2];
                    sums[3] += (ਪੂਰਨ)src[x * 4 + 3];
                }
#ਜੇ_ਅੰਤ
            }
            ਨਹੀਂਤਾਂ
            {
                ਲਈ (ਪੂਰਨ x = 0, c = 0; x < ਚੌੜਾਈ; x++, c = (c+1)&-(c < cn-1))
                    sums[c] += (ਪੂਰਨ)src[x];
            }
            ਲਈ (ਪੂਰਨ c = 0; c < cn; c++)
                dst[c] = (ਦਸ਼ਮਲਵ)sums[c];
        }
    };
    parallel_for_(Range(0, srcmat.ਪੰਕਤੀਆਂ), body);
    v_cleanup();
}

// --- 16-bit (ushort/short) → float ---
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ SrcT, ਕਿਸਮ_ਨਾਂ VecSrc, ਕਿਸਮ_ਨਾਂ VecDst>
ਸਥਿਰ ਖਾਲੀ reduce_ਖਾਨਾ_ਜੋੜ_16_32f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{
    ਅਚਲ ਪੂਰਨ cn = srcmat.channels();
    ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = srcmat.ਖਾਨੇ_ਗਿਣਤੀ * cn;
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<VecSrc>::vlanes();

    ਸਵੈਚਾਲਿਤ body = [&](ਅਚਲ Range& range) {
        ਲਈ (ਪੂਰਨ y = range.ਸ਼ੁਰੂ; y < range.end; y++)
        {
            ਅਚਲ SrcT* src = srcmat.ptr<SrcT>(y);
            ਦਸ਼ਮਲਵ* dst = dstmat.ptr<ਦਸ਼ਮਲਵ>(y);

            ਜੇ (cn == 1)
            {
                VecDst v_sum0 = v_setzero_<VecDst>(), v_sum1 = v_setzero_<VecDst>();
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਚੌੜਾਈ - vlanes; x += vlanes)
                {
                    VecDst lo, hi;
                    v_expand(vx_ਲੱਦੋ(src + x), lo, hi);
                    v_sum0 = v_ਜੋੜੋ(v_sum0, lo);
                    v_sum1 = v_ਜੋੜੋ(v_sum1, hi);
                }
                dst[0] = (ਦਸ਼ਮਲਵ)(v_reduce_ਜੋੜ(v_sum0) + v_reduce_ਜੋੜ(v_sum1));
                ਲਈ (; x < ਚੌੜਾਈ; x++)
                    dst[0] += (ਦਸ਼ਮਲਵ)src[x];
            }
            ਨਹੀਂਤਾਂ
            {
                ਲਈ (ਪੂਰਨ c = 0; c < cn; c++)
                    dst[c] = 0;
                ਲਈ (ਪੂਰਨ x = 0, c = 0; x < ਚੌੜਾਈ; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += (ਦਸ਼ਮਲਵ)src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.ਪੰਕਤੀਆਂ), body);
    v_cleanup();
}

ਸਥਿਰ ਖਾਲੀ reduce_ਖਾਨਾ_ਜੋੜ_16u32f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{ reduce_ਖਾਨਾ_ਜੋੜ_16_32f<ushort, v_uint16, v_uint32>(srcmat, dstmat); }

ਸਥਿਰ ਖਾਲੀ reduce_ਖਾਨਾ_ਜੋੜ_16s32f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{ reduce_ਖਾਨਾ_ਜੋੜ_16_32f<ਛੋਟਾ, v_int16, v_int32>(srcmat, dstmat); }

// --- float/double → same type ---
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VecT>
ਸਥਿਰ ਖਾਲੀ reduce_ਖਾਨਾ_ਜੋੜ_FP(ਅਚਲ Mat& srcmat, Mat& dstmat)
{
    ਅਚਲ ਪੂਰਨ cn = srcmat.channels();
    ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = srcmat.ਖਾਨੇ_ਗਿਣਤੀ * cn;
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<VecT>::vlanes();

    ਸਵੈਚਾਲਿਤ body = [&](ਅਚਲ Range& range) {
        ਲਈ (ਪੂਰਨ y = range.ਸ਼ੁਰੂ; y < range.end; y++)
        {
            ਅਚਲ T* src = srcmat.ptr<T>(y);
            T* dst = dstmat.ptr<T>(y);

            ਜੇ (cn == 1)
            {
                VecT s0 = v_setzero_<VecT>(), s1 = v_setzero_<VecT>();
                VecT s2 = v_setzero_<VecT>(), s3 = v_setzero_<VecT>();
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਚੌੜਾਈ - vlanes * 4; x += vlanes * 4)
                {
                    s0 = v_ਜੋੜੋ(s0, vx_ਲੱਦੋ(src + x));
                    s1 = v_ਜੋੜੋ(s1, vx_ਲੱਦੋ(src + x + vlanes));
                    s2 = v_ਜੋੜੋ(s2, vx_ਲੱਦੋ(src + x + vlanes * 2));
                    s3 = v_ਜੋੜੋ(s3, vx_ਲੱਦੋ(src + x + vlanes * 3));
                }
                s0 = v_ਜੋੜੋ(v_ਜੋੜੋ(s0, s1), v_ਜੋੜੋ(s2, s3));
                ਲਈ (; x <= ਚੌੜਾਈ - vlanes; x += vlanes)
                    s0 = v_ਜੋੜੋ(s0, vx_ਲੱਦੋ(src + x));
                T ਕੁੱਲ = (T)v_reduce_ਜੋੜ(s0);
                ਲਈ (; x < ਚੌੜਾਈ; x++)
                    ਕੁੱਲ += src[x];
                dst[0] = ਕੁੱਲ;
            }
            ਨਹੀਂਤਾਂ
            {
                ਲਈ (ਪੂਰਨ c = 0; c < cn; c++)
                    dst[c] = 0;
                ਲਈ (ਪੂਰਨ x = 0, c = 0; x < ਚੌੜਾਈ; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.ਪੰਕਤੀਆਂ), body);
    v_cleanup();
}

ਸਥਿਰ ਖਾਲੀ reduce_ਖਾਨਾ_ਜੋੜ_32f32f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{ reduce_ਖਾਨਾ_ਜੋੜ_FP<ਦਸ਼ਮਲਵ, v_float32>(srcmat, dstmat); }

#ਜੇ CV_SIMD_64F
// --- float → double ---
ਸਥਿਰ ਖਾਲੀ reduce_ਖਾਨਾ_ਜੋੜ_32f64f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{
    ਅਚਲ ਪੂਰਨ cn = srcmat.channels();
    ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = srcmat.ਖਾਨੇ_ਗਿਣਤੀ * cn;
    ਅਚਲ ਪੂਰਨ vlanes32 = VTraits<v_float32>::vlanes();

    ਸਵੈਚਾਲਿਤ body = [&](ਅਚਲ Range& range) {
        ਲਈ (ਪੂਰਨ y = range.ਸ਼ੁਰੂ; y < range.end; y++)
        {
            ਅਚਲ ਦਸ਼ਮਲਵ* src = srcmat.ptr<ਦਸ਼ਮਲਵ>(y);
            ਦੁੱਗਣਾ* dst = dstmat.ptr<ਦੁੱਗਣਾ>(y);

            ਜੇ (cn == 1)
            {
                v_float64 v_sum0 = vx_setzero_f64();
                v_float64 v_sum1 = vx_setzero_f64();
                ਪੂਰਨ x = 0;
                ਲਈ (; x <= ਚੌੜਾਈ - vlanes32; x += vlanes32)
                {
                    v_float32 v = vx_ਲੱਦੋ(src + x);
                    v_sum0 = v_ਜੋੜੋ(v_sum0, v_cvt_f64(v));
                    v_sum1 = v_ਜੋੜੋ(v_sum1, v_cvt_f64_ਉੱਚਾ(v));
                }
                v_float64 v_ਕੁੱਲ = v_ਜੋੜੋ(v_sum0, v_sum1);

                ਦੁੱਗਣਾ ਕੁੱਲ = v_reduce_ਜੋੜ(v_ਕੁੱਲ);
                ਲਈ (; x < ਚੌੜਾਈ; x++)
                    ਕੁੱਲ += (ਦੁੱਗਣਾ)src[x];
                dst[0] = ਕੁੱਲ;
            }
            ਨਹੀਂਤਾਂ
            {
                ਲਈ (ਪੂਰਨ c = 0; c < cn; c++)
                    dst[c] = 0;
                ਲਈ (ਪੂਰਨ x = 0, c = 0; x < ਚੌੜਾਈ; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += (ਦੁੱਗਣਾ)src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.ਪੰਕਤੀਆਂ), body);
    v_cleanup();
}

ਸਥਿਰ ਖਾਲੀ reduce_ਖਾਨਾ_ਜੋੜ_64f64f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{ reduce_ਖਾਨਾ_ਜੋੜ_FP<ਦੁੱਗਣਾ, v_float64>(srcmat, dstmat); }
#ਜੇ_ਅੰਤ // CV_SIMD_64F

// =====================================================================
//  Row reduce SUM (dim=0): sum each column across all rows
// =====================================================================

// --- uchar → int ---
// Uses u16 intermediate accumulator for vertical sum: accumulate u8→u16 per row,
// flush u16→u32 every 256 rows (max u16 value: 256*255=65280 < 65535)
ਸਥਿਰ ਖਾਲੀ reduce_ਪੰਕਤੀ_ਜੋੜ_8u32s(ਅਚਲ Mat& srcmat, Mat& dstmat)
{
    ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ_cn = srcmat.ਖਾਨੇ_ਗਿਣਤੀ * srcmat.channels();
    ਅਚਲ ਪੂਰਨ ਉਚਾਈ = srcmat.ਪੰਕਤੀਆਂ;
    ਅਚਲ ਪੂਰਨ vlanes16 = VTraits<v_uint16>::vlanes();
    ਅਚਲ ਪੂਰਨ vlanes32 = VTraits<v_int32>::vlanes();

    ਸਵੈਚਾਲਿਤ body = [&](ਅਚਲ Range& range) {
        ਅਚਲ ਪੂਰਨ ਸ਼ੁਰੂ = range.ਸ਼ੁਰੂ;
        ਅਚਲ ਪੂਰਨ end = range.end;
        ਅਚਲ ਪੂਰਨ len = end - ਸ਼ੁਰੂ;

        Auto_ਬਫਰ<ਪੂਰਨ> buf32_storage(len);
        Auto_ਬਫਰ<ushort> buf16_storage(len);
        ਪੂਰਨ* buf32 = buf32_storage.ਅੰਕੜੇ();
        ushort* buf16 = buf16_storage.ਅੰਕੜੇ();

        // init from first row
        ਅਚਲ uchar* src0 = srcmat.ptr<uchar>(0) + ਸ਼ੁਰੂ;
        ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
            buf32[i] = (ਪੂਰਨ)src0[i];

        ਜੇ (ਉਚਾਈ <= 1)
        {
            ਪੂਰਨ* dst = dstmat.ptr<ਪੂਰਨ>(0) + ਸ਼ੁਰੂ;
            ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
                dst[i] = buf32[i];
            ਵਾਪਸ;
        }

        memset(buf16, 0, len * ਆਕਾਰ(ushort));
        ਅਚਲ ਪੂਰਨ flush_at = 256; // 256*255 = 65280 < 65535 (u16 safe)
        ਪੂਰਨ flush_ਗਿਣਤੀ = 0;

        ਲਈ (ਪੂਰਨ ਪੰਕਤੀ = 1; ਪੰਕਤੀ < ਉਚਾਈ; ਪੰਕਤੀ++)
        {
            ਅਚਲ uchar* src = srcmat.ptr<uchar>(ਪੰਕਤੀ) + ਸ਼ੁਰੂ;
            ਪੂਰਨ i = 0;

#ਜੇ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
            // AArch64: vaddw_u8 — single-instruction widening add (u16 += u8)
            ਲਈ (; i <= len - 16; i += 16)
            {
                uint8x16_t v = vld1q_u8(src + i);
                uint16x8_t acc_lo = vld1q_u16(buf16 + i);
                uint16x8_t acc_hi = vld1q_u16(buf16 + i + 8);
                vst1q_u16(buf16 + i,     vaddw_u8(acc_lo, vget_ਨੀਵਾਂ_u8(v)));
                vst1q_u16(buf16 + i + 8, vaddw_ਉੱਚਾ_u8(acc_hi, v));
            }
#ਜਾਂਜੇ CV_RVV
            ਅਚਲ ਪੂਰਨ vl = (ਪੂਰਨ)__riscv_vsetvlmax_e8m2();
            ਲਈ (; i <= len - vl; i += vl)
            {
                vuint8m2_t vs = __riscv_vle8_v_u8m2(src + i, vl);
                vuint16m4_t va = __riscv_vle16_v_u16m4(buf16 + i, vl);
                va = __riscv_vwaddu_wv_u16m4(va, vs, vl);
                __riscv_vse16_v_u16m4(buf16 + i, va, vl);
            }
#ਨਹੀਂਤਾਂ
            ਅਚਲ ਪੂਰਨ vlanes8 = VTraits<v_uint8>::vlanes();
            ਲਈ (; i <= len - vlanes8; i += vlanes8)
            {
                v_uint16 lo, hi;
                v_expand(vx_ਲੱਦੋ(src + i), lo, hi);
                v_store(buf16 + i, v_ਜੋੜੋ(vx_ਲੱਦੋ(buf16 + i), lo));
                v_store(buf16 + i + vlanes16, v_ਜੋੜੋ(vx_ਲੱਦੋ(buf16 + i + vlanes16), hi));
            }
#ਜੇ_ਅੰਤ
            ਲਈ (; i < len; i++)
                buf16[i] += (ushort)src[i];

            ਜੇ (++flush_ਗਿਣਤੀ >= flush_at || ਪੰਕਤੀ == ਉਚਾਈ - 1)
            {
                // flush u16 → u32
                ਪੂਰਨ j = 0;
                ਲਈ (; j <= len - vlanes16; j += vlanes16)
                {
                    v_uint32 lo, hi;
                    v_expand(vx_ਲੱਦੋ(buf16 + j), lo, hi);
                    v_store(buf32 + j, v_ਜੋੜੋ(vx_ਲੱਦੋ(buf32 + j), v_reinterpret_as_s32(lo)));
                    v_store(buf32 + j + vlanes32, v_ਜੋੜੋ(vx_ਲੱਦੋ(buf32 + j + vlanes32), v_reinterpret_as_s32(hi)));
                }
                ਲਈ (; j < len; j++)
                    buf32[j] += (ਪੂਰਨ)buf16[j];
                memset(buf16, 0, len * ਆਕਾਰ(ushort));
                flush_ਗਿਣਤੀ = 0;
            }
        }

        ਪੂਰਨ* dst = dstmat.ptr<ਪੂਰਨ>(0) + ਸ਼ੁਰੂ;
        ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
            dst[i] = buf32[i];
    };

    parallel_for_(Range(0, ਚੌੜਾਈ_cn), body, ਚੌੜਾਈ_cn * CV_ELEM_ਮਾਪ(srcmat.ਡੂੰਘਾਈ()) / 64);
    v_cleanup();
}

// --- uchar → float ---
ਸਥਿਰ ਖਾਲੀ reduce_ਪੰਕਤੀ_ਜੋੜ_8u32f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{
    // compute in int, then convert to float
    Mat ਆਰਜ਼ੀ_2(dstmat.ਪੰਕਤੀਆਂ, dstmat.ਖਾਨੇ_ਗਿਣਤੀ, CV_32SC(srcmat.channels()));
    reduce_ਪੰਕਤੀ_ਜੋੜ_8u32s(srcmat, ਆਰਜ਼ੀ_2);
    ਆਰਜ਼ੀ_2.convert_ਮੰਜ਼ਿਲ(dstmat, dstmat.type());
}

// --- 16-bit (ushort/short) → float row reduce ---
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ SrcT, ਕਿਸਮ_ਨਾਂ VecSrc, ਕਿਸਮ_ਨਾਂ VecDst>
ਸਥਿਰ ਖਾਲੀ reduce_ਪੰਕਤੀ_ਜੋੜ_16_32f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{
    ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ_cn = srcmat.ਖਾਨੇ_ਗਿਣਤੀ * srcmat.channels();
    ਅਚਲ ਪੂਰਨ ਉਚਾਈ = srcmat.ਪੰਕਤੀਆਂ;
    ਅਚਲ ਪੂਰਨ vlanes16 = VTraits<VecSrc>::vlanes();
    ਅਚਲ ਪੂਰਨ vlanes32 = vlanes16 / 2;

    ਸਵੈਚਾਲਿਤ body = [&](ਅਚਲ Range& range) {
        ਅਚਲ ਪੂਰਨ ਸ਼ੁਰੂ = range.ਸ਼ੁਰੂ;
        ਅਚਲ ਪੂਰਨ end = range.end;
        ਅਚਲ ਪੂਰਨ len = end - ਸ਼ੁਰੂ;
        ਦਸ਼ਮਲਵ* dst = dstmat.ptr<ਦਸ਼ਮਲਵ>(0) + ਸ਼ੁਰੂ;

        ਪੂਰਨ i = 0;
        ਲਈ (; i <= len - vlanes16 * 4; i += vlanes16 * 4)
        {
            ਅਚਲ SrcT* src0 = srcmat.ptr<SrcT>(0) + ਸ਼ੁਰੂ + i;
            VecDst a0, a1, a2, a3, a4, a5, a6, a7;
            v_expand(vx_ਲੱਦੋ(src0), a0, a1);
            v_expand(vx_ਲੱਦੋ(src0 + vlanes16), a2, a3);
            v_expand(vx_ਲੱਦੋ(src0 + vlanes16*2), a4, a5);
            v_expand(vx_ਲੱਦੋ(src0 + vlanes16*3), a6, a7);
            ਲਈ (ਪੂਰਨ ਪੰਕਤੀ = 1; ਪੰਕਤੀ < ਉਚਾਈ; ਪੰਕਤੀ++)
            {
                ਅਚਲ SrcT* src = srcmat.ptr<SrcT>(ਪੰਕਤੀ) + ਸ਼ੁਰੂ + i;
                VecDst lo, hi;
                v_expand(vx_ਲੱਦੋ(src), lo, hi);
                a0 = v_ਜੋੜੋ(a0, lo); a1 = v_ਜੋੜੋ(a1, hi);
                v_expand(vx_ਲੱਦੋ(src + vlanes16), lo, hi);
                a2 = v_ਜੋੜੋ(a2, lo); a3 = v_ਜੋੜੋ(a3, hi);
                v_expand(vx_ਲੱਦੋ(src + vlanes16*2), lo, hi);
                a4 = v_ਜੋੜੋ(a4, lo); a5 = v_ਜੋੜੋ(a5, hi);
                v_expand(vx_ਲੱਦੋ(src + vlanes16*3), lo, hi);
                a6 = v_ਜੋੜੋ(a6, lo); a7 = v_ਜੋੜੋ(a7, hi);
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
        ਲਈ (; i <= len - vlanes16; i += vlanes16)
        {
            VecDst a0, a1;
            v_expand(vx_ਲੱਦੋ(srcmat.ptr<SrcT>(0) + ਸ਼ੁਰੂ + i), a0, a1);
            ਲਈ (ਪੂਰਨ ਪੰਕਤੀ = 1; ਪੰਕਤੀ < ਉਚਾਈ; ਪੰਕਤੀ++)
            {
                VecDst lo, hi;
                v_expand(vx_ਲੱਦੋ(srcmat.ptr<SrcT>(ਪੰਕਤੀ) + ਸ਼ੁਰੂ + i), lo, hi);
                a0 = v_ਜੋੜੋ(a0, lo); a1 = v_ਜੋੜੋ(a1, hi);
            }
            v_store(dst + i, v_cvt_f32(v_reinterpret_as_s32(a0)));
            v_store(dst + i + vlanes32, v_cvt_f32(v_reinterpret_as_s32(a1)));
        }
        ਲਈ (; i < len; i++)
        {
            ਪੂਰਨ val = (ਪੂਰਨ)*(srcmat.ptr<SrcT>(0) + ਸ਼ੁਰੂ + i);
            ਲਈ (ਪੂਰਨ ਪੰਕਤੀ = 1; ਪੰਕਤੀ < ਉਚਾਈ; ਪੰਕਤੀ++)
                val += (ਪੂਰਨ)*(srcmat.ptr<SrcT>(ਪੰਕਤੀ) + ਸ਼ੁਰੂ + i);
            dst[i] = (ਦਸ਼ਮਲਵ)val;
        }
    };

    parallel_for_(Range(0, ਚੌੜਾਈ_cn), body, ਚੌੜਾਈ_cn * CV_ELEM_ਮਾਪ(srcmat.ਡੂੰਘਾਈ()) / 64);
    v_cleanup();
}

ਸਥਿਰ ਖਾਲੀ reduce_ਪੰਕਤੀ_ਜੋੜ_16u32f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{ reduce_ਪੰਕਤੀ_ਜੋੜ_16_32f<ushort, v_uint16, v_uint32>(srcmat, dstmat); }

ਸਥਿਰ ਖਾਲੀ reduce_ਪੰਕਤੀ_ਜੋੜ_16s32f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{ reduce_ਪੰਕਤੀ_ਜੋੜ_16_32f<ਛੋਟਾ, v_int16, v_int32>(srcmat, dstmat); }

// --- float/double → same type row reduce ---
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ VecT>
ਸਥਿਰ ਖਾਲੀ reduce_ਪੰਕਤੀ_ਜੋੜ_FP(ਅਚਲ Mat& srcmat, Mat& dstmat)
{
    ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ_cn = srcmat.ਖਾਨੇ_ਗਿਣਤੀ * srcmat.channels();
    ਅਚਲ ਪੂਰਨ ਉਚਾਈ = srcmat.ਪੰਕਤੀਆਂ;
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<VecT>::vlanes();

    ਸਵੈਚਾਲਿਤ body = [&](ਅਚਲ Range& range) {
        ਅਚਲ ਪੂਰਨ ਸ਼ੁਰੂ = range.ਸ਼ੁਰੂ;
        ਅਚਲ ਪੂਰਨ end = range.end;
        ਅਚਲ ਪੂਰਨ len = end - ਸ਼ੁਰੂ;
        T* dst = dstmat.ptr<T>(0) + ਸ਼ੁਰੂ;

        ਪੂਰਨ i = 0;
        ਲਈ (; i <= len - vlanes * 8; i += vlanes * 8)
        {
            ਅਚਲ T* src0 = srcmat.ptr<T>(0) + ਸ਼ੁਰੂ + i;
            VecT s0 = vx_ਲੱਦੋ(src0), s1 = vx_ਲੱਦੋ(src0 + vlanes);
            VecT s2 = vx_ਲੱਦੋ(src0 + vlanes*2), s3 = vx_ਲੱਦੋ(src0 + vlanes*3);
            VecT s4 = vx_ਲੱਦੋ(src0 + vlanes*4), s5 = vx_ਲੱਦੋ(src0 + vlanes*5);
            VecT s6 = vx_ਲੱਦੋ(src0 + vlanes*6), s7 = vx_ਲੱਦੋ(src0 + vlanes*7);
            ਲਈ (ਪੂਰਨ ਪੰਕਤੀ = 1; ਪੰਕਤੀ < ਉਚਾਈ; ਪੰਕਤੀ++)
            {
                ਅਚਲ T* src = srcmat.ptr<T>(ਪੰਕਤੀ) + ਸ਼ੁਰੂ + i;
                s0 = v_ਜੋੜੋ(s0, vx_ਲੱਦੋ(src));
                s1 = v_ਜੋੜੋ(s1, vx_ਲੱਦੋ(src + vlanes));
                s2 = v_ਜੋੜੋ(s2, vx_ਲੱਦੋ(src + vlanes*2));
                s3 = v_ਜੋੜੋ(s3, vx_ਲੱਦੋ(src + vlanes*3));
                s4 = v_ਜੋੜੋ(s4, vx_ਲੱਦੋ(src + vlanes*4));
                s5 = v_ਜੋੜੋ(s5, vx_ਲੱਦੋ(src + vlanes*5));
                s6 = v_ਜੋੜੋ(s6, vx_ਲੱਦੋ(src + vlanes*6));
                s7 = v_ਜੋੜੋ(s7, vx_ਲੱਦੋ(src + vlanes*7));
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
        ਲਈ (; i <= len - vlanes; i += vlanes)
        {
            VecT s0 = vx_ਲੱਦੋ(srcmat.ptr<T>(0) + ਸ਼ੁਰੂ + i);
            ਲਈ (ਪੂਰਨ ਪੰਕਤੀ = 1; ਪੰਕਤੀ < ਉਚਾਈ; ਪੰਕਤੀ++)
                s0 = v_ਜੋੜੋ(s0, vx_ਲੱਦੋ(srcmat.ptr<T>(ਪੰਕਤੀ) + ਸ਼ੁਰੂ + i));
            v_store(dst + i, s0);
        }
        ਲਈ (; i < len; i++)
        {
            T val = *(srcmat.ptr<T>(0) + ਸ਼ੁਰੂ + i);
            ਲਈ (ਪੂਰਨ ਪੰਕਤੀ = 1; ਪੰਕਤੀ < ਉਚਾਈ; ਪੰਕਤੀ++)
                val += *(srcmat.ptr<T>(ਪੰਕਤੀ) + ਸ਼ੁਰੂ + i);
            dst[i] = val;
        }
    };

    parallel_for_(Range(0, ਚੌੜਾਈ_cn), body, ਚੌੜਾਈ_cn * CV_ELEM_ਮਾਪ(srcmat.ਡੂੰਘਾਈ()) / 64);
    v_cleanup();
}

ਸਥਿਰ ਖਾਲੀ reduce_ਪੰਕਤੀ_ਜੋੜ_32f32f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{ reduce_ਪੰਕਤੀ_ਜੋੜ_FP<ਦਸ਼ਮਲਵ, v_float32>(srcmat, dstmat); }

#ਜੇ CV_SIMD_64F
// --- float → double ---
ਸਥਿਰ ਖਾਲੀ reduce_ਪੰਕਤੀ_ਜੋੜ_32f64f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{
    ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ_cn = srcmat.ਖਾਨੇ_ਗਿਣਤੀ * srcmat.channels();
    ਅਚਲ ਪੂਰਨ ਉਚਾਈ = srcmat.ਪੰਕਤੀਆਂ;
    ਅਚਲ ਪੂਰਨ vlanes32 = VTraits<v_float32>::vlanes();
    ਅਚਲ ਪੂਰਨ vlanes64 = VTraits<v_float64>::vlanes();

    ਸਵੈਚਾਲਿਤ body = [&](ਅਚਲ Range& range) {
        ਅਚਲ ਪੂਰਨ ਸ਼ੁਰੂ = range.ਸ਼ੁਰੂ;
        ਅਚਲ ਪੂਰਨ end = range.end;
        ਅਚਲ ਪੂਰਨ len = end - ਸ਼ੁਰੂ;

        Auto_ਬਫਰ<ਦੁੱਗਣਾ> buf_storage(len);
        ਦੁੱਗਣਾ* buf = buf_storage.ਅੰਕੜੇ();

        ਅਚਲ ਦਸ਼ਮਲਵ* src0 = srcmat.ptr<ਦਸ਼ਮਲਵ>(0) + ਸ਼ੁਰੂ;
        ਲਈ (ਪੂਰਨ i = 0; i < len; i++)
            buf[i] = (ਦੁੱਗਣਾ)src0[i];

        ਲਈ (ਪੂਰਨ ਪੰਕਤੀ = 1; ਪੰਕਤੀ < ਉਚਾਈ; ਪੰਕਤੀ++)
        {
            ਅਚਲ ਦਸ਼ਮਲਵ* src = srcmat.ptr<ਦਸ਼ਮਲਵ>(ਪੰਕਤੀ) + ਸ਼ੁਰੂ;
            ਪੂਰਨ i = 0;
            ਲਈ (; i <= len - vlanes32; i += vlanes32)
            {
                v_float32 v = vx_ਲੱਦੋ(src + i);
                v_store(buf + i, v_ਜੋੜੋ(vx_ਲੱਦੋ(buf + i), v_cvt_f64(v)));
                v_store(buf + i + vlanes64, v_ਜੋੜੋ(vx_ਲੱਦੋ(buf + i + vlanes64), v_cvt_f64_ਉੱਚਾ(v)));
            }
            ਲਈ (; i < len; i++)
                buf[i] += (ਦੁੱਗਣਾ)src[i];
        }

        ਦੁੱਗਣਾ* dst = dstmat.ptr<ਦੁੱਗਣਾ>(0) + ਸ਼ੁਰੂ;
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst, buf, len * ਆਕਾਰ(ਦੁੱਗਣਾ));
    };

    parallel_for_(Range(0, ਚੌੜਾਈ_cn), body, ਚੌੜਾਈ_cn * CV_ELEM_ਮਾਪ(srcmat.ਡੂੰਘਾਈ()) / 64);
    v_cleanup();
}

ਸਥਿਰ ਖਾਲੀ reduce_ਪੰਕਤੀ_ਜੋੜ_64f64f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{ reduce_ਪੰਕਤੀ_ਜੋੜ_FP<ਦੁੱਗਣਾ, v_float64>(srcmat, dstmat); }
#ਜੇ_ਅੰਤ // CV_SIMD_64F

#ਜੇ_ਅੰਤ // CV_SIMD || CV_SIMD_SCALABLE

#ਜੇ CV_SIMD_SCALABLE
ਸਥਿਰ ਸਤਰਅੰਦਰ v_float32 reduce_ਪੰਕਤੀ_Sum2_Acc4(ਅਚਲ v_float32& acc,
                                           ਅਚਲ ਦਸ਼ਮਲਵ* src0, ਅਚਲ ਦਸ਼ਮਲਵ* src1,
                                           ਅਚਲ ਦਸ਼ਮਲਵ* src2, ਅਚਲ ਦਸ਼ਮਲਵ* src3)
{
    v_float32 ਜੋੜ = acc;
    v_float32 x = vx_ਲੱਦੋ(src0);
    ਜੋੜ = v_fma(x, x, ਜੋੜ);
    x = vx_ਲੱਦੋ(src1);
    ਜੋੜ = v_fma(x, x, ਜੋੜ);
    x = vx_ਲੱਦੋ(src2);
    ਜੋੜ = v_fma(x, x, ਜੋੜ);
    x = vx_ਲੱਦੋ(src3);
    ਵਾਪਸ v_fma(x, x, ਜੋੜ);
}

// dim=0, REDUCE_SUM2 for CV_32F using scalable universal intrinsics.
ਸਥਿਰ ਖਾਲੀ reduce_ਪੰਕਤੀ_Sum2_32f32f(ਅਚਲ Mat& srcmat, Mat& dstmat)
{
    ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = srcmat.ਖਾਨੇ_ਗਿਣਤੀ * srcmat.channels();
    ਅਚਲ ਪੂਰਨ ਉਚਾਈ = srcmat.ਪੰਕਤੀਆਂ;
    ਅਚਲ ਪੂਰਨ vlanes = VTraits<v_float32>::vlanes();
    ਅਚਲ ਪੂਰਨ ਖੰਡ_2 = vlanes * 4;
    Auto_ਬਫਰ<ਦਸ਼ਮਲਵ> ਬਫਰ(ਚੌੜਾਈ);
    ਦਸ਼ਮਲਵ* buf = ਬਫਰ.ਅੰਕੜੇ();

    ਅਚਲ ਦਸ਼ਮਲਵ* src = srcmat.ptr<ਦਸ਼ਮਲਵ>(0);
    ਪੂਰਨ i = 0;
    ਲਈ (; i <= ਚੌੜਾਈ - ਖੰਡ_2; i += ਖੰਡ_2)
    {
        v_float32 x0 = vx_ਲੱਦੋ(src + i);
        v_float32 x1 = vx_ਲੱਦੋ(src + i + vlanes);
        v_float32 x2 = vx_ਲੱਦੋ(src + i + vlanes * 2);
        v_float32 x3 = vx_ਲੱਦੋ(src + i + vlanes * 3);
        v_store(buf + i, v_mul(x0, x0));
        v_store(buf + i + vlanes, v_mul(x1, x1));
        v_store(buf + i + vlanes * 2, v_mul(x2, x2));
        v_store(buf + i + vlanes * 3, v_mul(x3, x3));
    }
    ਲਈ (; i <= ਚੌੜਾਈ - vlanes; i += vlanes)
    {
        v_float32 x = vx_ਲੱਦੋ(src + i);
        v_store(buf + i, v_mul(x, x));
    }
    ਲਈ (; i < ਚੌੜਾਈ; i++)
        buf[i] = src[i] * src[i];

    ਪੂਰਨ ਪੰਕਤੀ = 1;
    // Four rows per pass amortize buffer traffic while preserving source-row
    // accumulation order. The outer loop stays serial: splitting width into
    // tiny parallel stripes under-fills scalable vectors and adds scheduling
    // overhead for this memory-streaming kernel.
    ਲਈ (; ਪੰਕਤੀ <= ਉਚਾਈ - 4; ਪੰਕਤੀ += 4)
    {
        ਅਚਲ ਦਸ਼ਮਲਵ* src0 = srcmat.ptr<ਦਸ਼ਮਲਵ>(ਪੰਕਤੀ);
        ਅਚਲ ਦਸ਼ਮਲਵ* src1 = srcmat.ptr<ਦਸ਼ਮਲਵ>(ਪੰਕਤੀ + 1);
        ਅਚਲ ਦਸ਼ਮਲਵ* src2 = srcmat.ptr<ਦਸ਼ਮਲਵ>(ਪੰਕਤੀ + 2);
        ਅਚਲ ਦਸ਼ਮਲਵ* src3 = srcmat.ptr<ਦਸ਼ਮਲਵ>(ਪੰਕਤੀ + 3);

        i = 0;
        ਲਈ (; i <= ਚੌੜਾਈ - ਖੰਡ_2; i += ਖੰਡ_2)
        {
            v_float32 sum0 = vx_ਲੱਦੋ(buf + i);
            v_float32 sum1 = vx_ਲੱਦੋ(buf + i + vlanes);
            v_float32 sum2 = vx_ਲੱਦੋ(buf + i + vlanes * 2);
            v_float32 sum3 = vx_ਲੱਦੋ(buf + i + vlanes * 3);
            sum0 = reduce_ਪੰਕਤੀ_Sum2_Acc4(sum0, src0 + i, src1 + i, src2 + i, src3 + i);
            sum1 = reduce_ਪੰਕਤੀ_Sum2_Acc4(sum1, src0 + i + vlanes, src1 + i + vlanes,
                                     src2 + i + vlanes, src3 + i + vlanes);
            sum2 = reduce_ਪੰਕਤੀ_Sum2_Acc4(sum2, src0 + i + vlanes * 2, src1 + i + vlanes * 2,
                                     src2 + i + vlanes * 2, src3 + i + vlanes * 2);
            sum3 = reduce_ਪੰਕਤੀ_Sum2_Acc4(sum3, src0 + i + vlanes * 3, src1 + i + vlanes * 3,
                                     src2 + i + vlanes * 3, src3 + i + vlanes * 3);
            v_store(buf + i, sum0);
            v_store(buf + i + vlanes, sum1);
            v_store(buf + i + vlanes * 2, sum2);
            v_store(buf + i + vlanes * 3, sum3);
        }
        ਲਈ (; i <= ਚੌੜਾਈ - vlanes; i += vlanes)
        {
            v_float32 ਜੋੜ = vx_ਲੱਦੋ(buf + i);
            ਜੋੜ = reduce_ਪੰਕਤੀ_Sum2_Acc4(ਜੋੜ, src0 + i, src1 + i, src2 + i, src3 + i);
            v_store(buf + i, ਜੋੜ);
        }
        ਲਈ (; i < ਚੌੜਾਈ; i++)
        {
            ਦਸ਼ਮਲਵ ਜੋੜ = buf[i];
            ਦਸ਼ਮਲਵ x = src0[i]; ਜੋੜ += x * x;
            x = src1[i]; ਜੋੜ += x * x;
            x = src2[i]; ਜੋੜ += x * x;
            x = src3[i]; ਜੋੜ += x * x;
            buf[i] = ਜੋੜ;
        }
    }

    ਲਈ (; ਪੰਕਤੀ < ਉਚਾਈ; ਪੰਕਤੀ++)
    {
        src = srcmat.ptr<ਦਸ਼ਮਲਵ>(ਪੰਕਤੀ);
        i = 0;
        ਲਈ (; i <= ਚੌੜਾਈ - ਖੰਡ_2; i += ਖੰਡ_2)
        {
            v_float32 sum0 = vx_ਲੱਦੋ(buf + i);
            v_float32 sum1 = vx_ਲੱਦੋ(buf + i + vlanes);
            v_float32 sum2 = vx_ਲੱਦੋ(buf + i + vlanes * 2);
            v_float32 sum3 = vx_ਲੱਦੋ(buf + i + vlanes * 3);
            v_float32 x0 = vx_ਲੱਦੋ(src + i);
            v_float32 x1 = vx_ਲੱਦੋ(src + i + vlanes);
            v_float32 x2 = vx_ਲੱਦੋ(src + i + vlanes * 2);
            v_float32 x3 = vx_ਲੱਦੋ(src + i + vlanes * 3);
            v_store(buf + i, v_fma(x0, x0, sum0));
            v_store(buf + i + vlanes, v_fma(x1, x1, sum1));
            v_store(buf + i + vlanes * 2, v_fma(x2, x2, sum2));
            v_store(buf + i + vlanes * 3, v_fma(x3, x3, sum3));
        }
        ਲਈ (; i <= ਚੌੜਾਈ - vlanes; i += vlanes)
        {
            v_float32 x = vx_ਲੱਦੋ(src + i);
            v_store(buf + i, v_fma(x, x, vx_ਲੱਦੋ(buf + i)));
        }
        ਲਈ (; i < ਚੌੜਾਈ; i++)
            buf[i] += src[i] * src[i];
    }

    // Delay output stores until all source rows are consumed, preserving the
    // existing overlap-safe behavior of the proposed optimized path.
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(dstmat.ptr<ਦਸ਼ਮਲਵ>(), buf, ਚੌੜਾਈ * ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    v_cleanup();
}
#ਜੇ_ਅੰਤ

// =====================================================================
//  Dispatchers
// =====================================================================

Reduce_ਜੋੜ_Func getReduceCSumFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth)
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ (sdepth == CV_8U && ddepth == CV_32S) ਵਾਪਸ reduce_ਖਾਨਾ_ਜੋੜ_8u32s;
    ਜੇ (sdepth == CV_8U && ddepth == CV_32F) ਵਾਪਸ reduce_ਖਾਨਾ_ਜੋੜ_8u32f;
    ਜੇ (sdepth == CV_16U && ddepth == CV_32F) ਵਾਪਸ reduce_ਖਾਨਾ_ਜੋੜ_16u32f;
    ਜੇ (sdepth == CV_16S && ddepth == CV_32F) ਵਾਪਸ reduce_ਖਾਨਾ_ਜੋੜ_16s32f;
    ਜੇ (sdepth == CV_32F && ddepth == CV_32F) ਵਾਪਸ reduce_ਖਾਨਾ_ਜੋੜ_32f32f;
#ਜੇ CV_SIMD_64F
    ਜੇ (sdepth == CV_32F && ddepth == CV_64F) ਵਾਪਸ reduce_ਖਾਨਾ_ਜੋੜ_32f64f;
    ਜੇ (sdepth == CV_64F && ddepth == CV_64F) ਵਾਪਸ reduce_ਖਾਨਾ_ਜੋੜ_64f64f;
#ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(sdepth);
    CV_ਅਣਵਰਤਿਆ(ddepth);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
}

Reduce_ਜੋੜ_Func getReduceRSumFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth)
{
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ (sdepth == CV_8U && ddepth == CV_32S) ਵਾਪਸ reduce_ਪੰਕਤੀ_ਜੋੜ_8u32s;
    ਜੇ (sdepth == CV_8U && ddepth == CV_32F) ਵਾਪਸ reduce_ਪੰਕਤੀ_ਜੋੜ_8u32f;
    ਜੇ (sdepth == CV_16U && ddepth == CV_32F) ਵਾਪਸ reduce_ਪੰਕਤੀ_ਜੋੜ_16u32f;
    ਜੇ (sdepth == CV_16S && ddepth == CV_32F) ਵਾਪਸ reduce_ਪੰਕਤੀ_ਜੋੜ_16s32f;
    ਜੇ (sdepth == CV_32F && ddepth == CV_32F) ਵਾਪਸ reduce_ਪੰਕਤੀ_ਜੋੜ_32f32f;
#ਜੇ CV_SIMD_64F
    ਜੇ (sdepth == CV_32F && ddepth == CV_64F) ਵਾਪਸ reduce_ਪੰਕਤੀ_ਜੋੜ_32f64f;
    ਜੇ (sdepth == CV_64F && ddepth == CV_64F) ਵਾਪਸ reduce_ਪੰਕਤੀ_ਜੋੜ_64f64f;
#ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(sdepth);
    CV_ਅਣਵਰਤਿਆ(ddepth);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
}

Reduce_ਜੋੜ_Func getReduceRSum2Func(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth)
{
#ਜੇ CV_SIMD_SCALABLE
    ਜੇ (sdepth == CV_32F && ddepth == CV_32F) ਵਾਪਸ reduce_ਪੰਕਤੀ_Sum2_32f32f;
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(sdepth);
    CV_ਅਣਵਰਤਿਆ(ddepth);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
}

#ਜੇ_ਅੰਤ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
} // namespace cv
