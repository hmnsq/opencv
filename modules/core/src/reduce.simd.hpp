// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*Reduce_ಮೊತ್ತ_Func)(ಸ್ಥಿರಾಂಕ Mat& src, Mat& dst);
Reduce_ಮೊತ್ತ_Func getReduceCSumFunc(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth);
Reduce_ಮೊತ್ತ_Func getReduceRSumFunc(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth);
Reduce_ಮೊತ್ತ_Func getReduceRSum2Func(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// =====================================================================
//  Col reduce SUM (dim=1): sum each row into cn output values
// =====================================================================

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)

// --- uchar → int ---
// Uses u16 intermediate accumulator to halve the number of widen operations:
// u8→u16 per iteration, u16→u32 flush every 128 iterations (max u16 value: 128*255=32640 < 65535)
ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_8u32s(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = srcmat.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = srcmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn;

    ಸ್ವಯಂಚಾಲಿತ body = [&](ಸ್ಥಿರಾಂಕ Range& range) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = range.ಆರಂಭ; y < range.end; y++)
        {
            ಸ್ಥಿರಾಂಕ uchar* src = srcmat.ptr<uchar>(y);
            ಪೂರ್ಣಾಂಕ* dst = dstmat.ptr<ಪೂರ್ಣಾಂಕ>(y);

            ಆದರೆ (cn == 1)
            {
#ಆದರೆ defined(CV_NEON_DOT)
                // ARMv8.2 DOTPROD: vdotq_u32(acc, src, ones) — single instruction byte sum
                // u32 lane max 1020/iter, overflow at ~4.2M iter (67MB) — safe without flush
                ಸ್ಥಿರಾಂಕ uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t v_ಮೊತ್ತ = vdupq_n_u32(0);
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಅಗಲ - 16; x += 16)
                    v_ಮೊತ್ತ = vdotq_u32(v_ಮೊತ್ತ, vld1q_u8(src + x), ones);
                ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)vaddvq_u32(v_ಮೊತ್ತ);
                ಗಾಗಿ (; x < ಅಗಲ; x++)
                    ಒಟ್ಟು += (ಪೂರ್ಣಾಂಕ)src[x];
                dst[0] = ಒಟ್ಟು;
#ಅಥವಾಆದರೆ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                // AArch64 fallback: vpaddlq chain
                uint32x4_t v_ಮೊತ್ತ = vdupq_n_u32(0);
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಅಗಲ - 16; x += 16)
                    v_ಮೊತ್ತ = vaddq_u32(v_ಮೊತ್ತ, vpaddlq_u16(vpaddlq_u8(vld1q_u8(src + x))));
                ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)vaddvq_u32(v_ಮೊತ್ತ);
                ಗಾಗಿ (; x < ಅಗಲ; x++)
                    ಒಟ್ಟು += (ಪೂರ್ಣಾಂಕ)src[x];
                dst[0] = ಒಟ್ಟು;
#ಅಥವಾಆದರೆ CV_AVX2
                // Intel AVX2: _mm256_sad_epu8 — 32 bytes → 4×u64 sum, 1 cycle
                ಸ್ಥಿರಾಂಕ __m256i zero = _mm256_setzero_si256();
                __m256i vsum = zero;
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಅಗಲ - 32; x += 32)
                    vsum = mm256_ಕೂಡಿಸು_epi64(vsum, _mm256_sad_epu8(
                        _mm256_loadu_si256((ಸ್ಥಿರಾಂಕ __m256i*)(src + x)), zero));
                // 8-byte tail: reduce max scalar tail from 31 to 7
                // Use _mm256_inserti128_si256 instead of _mm256_castsi128_si256:
                // the latter leaves upper 128 bits undefined per Intel spec,
                // causing MSVC to expose stale register data to _mm256_sad_epu8.
                ಗಾಗಿ (; x <= ಅಗಲ - 8; x += 8)
                    vsum = mm256_ಕೂಡಿಸು_epi64(vsum, _mm256_sad_epu8(
                        _mm256_inserti128_si256(_mm256_setzero_si256(), _mm_loadl_epi64((ಸ್ಥಿರಾಂಕ __m128i*)(src + x)), 0), zero));
                __m128i lo128 = _mm256_castsi256_si128(vsum);
                __m128i hi128 = _mm256_extracti128_si256(vsum, 1);
                __m128i s = mm_ಕೂಡಿಸು_epi64(lo128, hi128);
                s = mm_ಕೂಡಿಸು_epi64(s, _mm_unpackhi_epi64(s, s));
                ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = _mm_cvtsi128_si32(s);
                ಗಾಗಿ (; x < ಅಗಲ; x++)
                    ಒಟ್ಟು += (ಪೂರ್ಣಾಂಕ)src[x];
                dst[0] = ಒಟ್ಟು;
#ಅಥವಾಆದರೆ CV_SSE2
                // Intel SSE2: _mm_sad_epu8 — 16 bytes → 2×u64 sum
                ಸ್ಥಿರಾಂಕ __m128i zero = _mm_setzero_si128();
                __m128i vsum = zero;
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಅಗಲ - 16; x += 16)
                    vsum = mm_ಕೂಡಿಸು_epi64(vsum, _mm_sad_epu8(
                        _mm_loadu_si128((ಸ್ಥಿರಾಂಕ __m128i*)(src + x)), zero));
                __m128i s = mm_ಕೂಡಿಸು_epi64(vsum, _mm_unpackhi_epi64(vsum, vsum));
                ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = _mm_cvtsi128_si32(s);
                ಗಾಗಿ (; x < ಅಗಲ; x++)
                    ಒಟ್ಟು += (ಪೂರ್ಣಾಂಕ)src[x];
                dst[0] = ಒಟ್ಟು;
#ಇಲ್ಲದಿದ್ದರೆ
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 v_ಮೊತ್ತ = vx_setzero_u32();
                v_uint16 v_sum16 = vx_setzero_u16();
                ಪೂರ್ಣಾಂಕ x = 0, batch = 0;
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ flush_at = 128;

                ಗಾಗಿ (; x <= ಅಗಲ - vlanes8; x += vlanes8)
                {
                    v_uint16 lo, hi;
                    v_expand(vx_ತುಂಬು(src + x), lo, hi);
                    v_sum16 = v_ಕೂಡಿಸು(v_sum16, v_ಕೂಡಿಸು(lo, hi));

                    ಆದರೆ (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(v_sum16, a, b);
                        v_ಮೊತ್ತ = v_ಕೂಡಿಸು(v_ಮೊತ್ತ, v_ಕೂಡಿಸು(a, b));
                        v_sum16 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(v_sum16, a, b);
                    v_ಮೊತ್ತ = v_ಕೂಡಿಸು(v_ಮೊತ್ತ, v_ಕೂಡಿಸು(a, b));
                }

                ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(v_ಮೊತ್ತ);
                ಗಾಗಿ (; x < ಅಗಲ; x++)
                    ಒಟ್ಟು += (ಪೂರ್ಣಾಂಕ)src[x];
                dst[0] = ಒಟ್ಟು;
#ಆದರೆ_ಅಂತ್ಯ
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn == 4)
            {
#ಆದರೆ defined(CV_NEON_DOT)
                // ARMv8.2 DOTPROD: vld4q_u8 deinterleave + vdotq_u32 per-channel
                // vld4q_u8 reads 16 pixels (64 bytes), deinterleaves into 4 × u8x16
                // vdotq_u32 sums each channel's 16 bytes into 4 partial u32 sums
                ಸ್ಥಿರಾಂಕ uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t acc0 = vdupq_n_u32(0), acc1 = vdupq_n_u32(0);
                uint32x4_t acc2 = vdupq_n_u32(0), acc3 = vdupq_n_u32(0);
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    acc0 = vdotq_u32(acc0, v.val[0], ones);
                    acc1 = vdotq_u32(acc1, v.val[1], ones);
                    acc2 = vdotq_u32(acc2, v.val[2], ones);
                    acc3 = vdotq_u32(acc3, v.val[3], ones);
                }
                dst[0] = (ಪೂರ್ಣಾಂಕ)vaddvq_u32(acc0);
                dst[1] = (ಪೂರ್ಣಾಂಕ)vaddvq_u32(acc1);
                dst[2] = (ಪೂರ್ಣಾಂಕ)vaddvq_u32(acc2);
                dst[3] = (ಪೂರ್ಣಾಂಕ)vaddvq_u32(acc3);
                ಗಾಗಿ (; x < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; x++)
                {
                    dst[0] += (ಪೂರ್ಣಾಂಕ)src[x * 4];
                    dst[1] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 1];
                    dst[2] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 2];
                    dst[3] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 3];
                }
#ಅಥವಾಆದರೆ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                // AArch64 fallback: vld4q_u8 deinterleave + vaddlvq_u8 per-channel
                ಪೂರ್ಣಾಂಕ sums[4] = {0, 0, 0, 0};
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    sums[0] += (ಪೂರ್ಣಾಂಕ)vaddlvq_u8(v.val[0]);
                    sums[1] += (ಪೂರ್ಣಾಂಕ)vaddlvq_u8(v.val[1]);
                    sums[2] += (ಪೂರ್ಣಾಂಕ)vaddlvq_u8(v.val[2]);
                    sums[3] += (ಪೂರ್ಣಾಂಕ)vaddlvq_u8(v.val[3]);
                }
                dst[0] = sums[0]; dst[1] = sums[1];
                dst[2] = sums[2]; dst[3] = sums[3];
                ಗಾಗಿ (; x < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; x++)
                {
                    dst[0] += (ಪೂರ್ಣಾಂಕ)src[x * 4];
                    dst[1] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 1];
                    dst[2] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 2];
                    dst[3] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 3];
                }
#ಅಥವಾಆದರೆ CV_AVX2
                // Intel AVX2: shuffle + unpack to group channels, full-register SAD
                ಸ್ಥಿರಾಂಕ __m256i zero = _mm256_setzero_si256();
                ಸ್ಥಿರಾಂಕ __m256i shuf_ಮುಖವಾಡ = _mm256_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                ಪೂರ್ಣಾಂಕ x = 0;
                // 16 pixels/iter with unpack (full-register SAD)
                {
                    __m256i ಮೊತ್ತ_bg = zero, ಮೊತ್ತ_ra = zero;
                    ಗಾಗಿ (; x <= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 16; x += 16)
                    {
                        __m256i v0 = _mm256_loadu_si256((ಸ್ಥಿರಾಂಕ __m256i*)(src + x * 4));
                        __m256i v1 = _mm256_loadu_si256((ಸ್ಥಿರಾಂಕ __m256i*)(src + x * 4 + 32));
                        __m256i g0 = _mm256_shuffle_epi8(v0, shuf_ಮುಖವಾಡ);
                        __m256i g1 = _mm256_shuffle_epi8(v1, shuf_ಮುಖವಾಡ);
                        __m256i bg = _mm256_unpacklo_epi32(g0, g1);
                        __m256i ra = _mm256_unpackhi_epi32(g0, g1);
                        ಮೊತ್ತ_bg = mm256_ಕೂಡಿಸು_epi64(ಮೊತ್ತ_bg, _mm256_sad_epu8(bg, zero));
                        ಮೊತ್ತ_ra = mm256_ಕೂಡಿಸು_epi64(ಮೊತ್ತ_ra, _mm256_sad_epu8(ra, zero));
                    }
                    __m128i bg_lo = _mm256_castsi256_si128(ಮೊತ್ತ_bg);
                    __m128i bg_hi = _mm256_extracti128_si256(ಮೊತ್ತ_bg, 1);
                    __m128i bg_s = mm_ಕೂಡಿಸು_epi64(bg_lo, bg_hi);
                    __m128i ra_lo = _mm256_castsi256_si128(ಮೊತ್ತ_ra);
                    __m128i ra_hi = _mm256_extracti128_si256(ಮೊತ್ತ_ra, 1);
                    __m128i ra_s = mm_ಕೂಡಿಸು_epi64(ra_lo, ra_hi);
                    dst[0] = _mm_cvtsi128_si32(bg_s);                     // B
                    dst[1] = _mm_cvtsi128_si32(_mm_srli_si128(bg_s, 8));  // G
                    dst[2] = _mm_cvtsi128_si32(ra_s);                     // R
                    dst[3] = _mm_cvtsi128_si32(_mm_srli_si128(ra_s, 8));  // A
                }
                // 8-pixel tail with and/srli
                {
                    ಸ್ಥಿರಾಂಕ __m256i ಮುಖವಾಡ_lo32 = _mm256_set1_epi64x(0x00000000FFFFFFFFLL);
                    __m256i ಮೊತ್ತ_br = zero, ಮೊತ್ತ_ga = zero;
                    ಗಾಗಿ (; x <= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 8; x += 8)
                    {
                        __m256i v = _mm256_loadu_si256((ಸ್ಥಿರಾಂಕ __m256i*)(src + x * 4));
                        __m256i grouped = _mm256_shuffle_epi8(v, shuf_ಮುಖವಾಡ);
                        __m256i br = _mm256_and_si256(grouped, ಮುಖವಾಡ_lo32);
                        __m256i ga = _mm256_srli_epi64(grouped, 32);
                        ಮೊತ್ತ_br = mm256_ಕೂಡಿಸು_epi64(ಮೊತ್ತ_br, _mm256_sad_epu8(br, zero));
                        ಮೊತ್ತ_ga = mm256_ಕೂಡಿಸು_epi64(ಮೊತ್ತ_ga, _mm256_sad_epu8(ga, zero));
                    }
                    __m128i br_lo = _mm256_castsi256_si128(ಮೊತ್ತ_br);
                    __m128i br_hi = _mm256_extracti128_si256(ಮೊತ್ತ_br, 1);
                    __m128i br_s = mm_ಕೂಡಿಸು_epi64(br_lo, br_hi);
                    __m128i ga_lo = _mm256_castsi256_si128(ಮೊತ್ತ_ga);
                    __m128i ga_hi = _mm256_extracti128_si256(ಮೊತ್ತ_ga, 1);
                    __m128i ga_s = mm_ಕೂಡಿಸು_epi64(ga_lo, ga_hi);
                    dst[0] += _mm_cvtsi128_si32(br_s);
                    dst[1] += _mm_cvtsi128_si32(ga_s);
                    dst[2] += _mm_cvtsi128_si32(_mm_srli_si128(br_s, 8));
                    dst[3] += _mm_cvtsi128_si32(_mm_srli_si128(ga_s, 8));
                }
                ಗಾಗಿ (; x < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; x++)
                {
                    dst[0] += (ಪೂರ್ಣಾಂಕ)src[x * 4];
                    dst[1] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 1];
                    dst[2] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 2];
                    dst[3] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 3];
                }
#ಅಥವಾಆದರೆ CV_SSSE3
                // Intel SSSE3: _mm_shuffle_epi8 deinterleave + _mm_sad_epu8
                // 16 bytes = 4 pixels × 4 channels per iteration
                ಸ್ಥಿರಾಂಕ __m128i zero = _mm_setzero_si128();
                ಸ್ಥಿರಾಂಕ __m128i shuf_ಮುಖವಾಡ = _mm_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                ಸ್ಥಿರಾಂಕ __m128i ಮುಖವಾಡ_lo32 = _mm_set1_epi64x(0x00000000FFFFFFFFLL);
                __m128i ಮೊತ್ತ_br = zero, ಮೊತ್ತ_ga = zero;
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 4; x += 4)
                {
                    __m128i v = _mm_loadu_si128((ಸ್ಥಿರಾಂಕ __m128i*)(src + x * 4));
                    __m128i grouped = _mm_shuffle_epi8(v, shuf_ಮುಖವಾಡ);
                    __m128i br = _mm_and_si128(grouped, ಮುಖವಾಡ_lo32);
                    __m128i ga = _mm_srli_epi64(grouped, 32);
                    ಮೊತ್ತ_br = mm_ಕೂಡಿಸು_epi64(ಮೊತ್ತ_br, _mm_sad_epu8(br, zero));
                    ಮೊತ್ತ_ga = mm_ಕೂಡಿಸು_epi64(ಮೊತ್ತ_ga, _mm_sad_epu8(ga, zero));
                }
                dst[0] = _mm_cvtsi128_si32(ಮೊತ್ತ_br);
                dst[1] = _mm_cvtsi128_si32(ಮೊತ್ತ_ga);
                dst[2] = _mm_cvtsi128_si32(_mm_srli_si128(ಮೊತ್ತ_br, 8));
                dst[3] = _mm_cvtsi128_si32(_mm_srli_si128(ಮೊತ್ತ_ga, 8));
                ಗಾಗಿ (; x < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; x++)
                {
                    dst[0] += (ಪೂರ್ಣಾಂಕ)src[x * 4];
                    dst[1] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 1];
                    dst[2] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 2];
                    dst[3] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 3];
                }
#ಇಲ್ಲದಿದ್ದರೆ
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 vsum0 = vx_setzero_u32(), vsum1 = vx_setzero_u32();
                v_uint32 vsum2 = vx_setzero_u32(), vsum3 = vx_setzero_u32();
                v_uint16 vs16_0 = vx_setzero_u16(), vs16_1 = vx_setzero_u16();
                v_uint16 vs16_2 = vx_setzero_u16(), vs16_3 = vx_setzero_u16();
                ಪೂರ್ಣಾಂಕ x = 0, batch = 0;
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ flush_at = 128;

                ಗಾಗಿ (; x <= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - vlanes8; x += vlanes8)
                {
                    v_uint8 ch0, ch1, ch2, ch3;
                    v_ತುಂಬು_deinterleave(src + x * 4, ch0, ch1, ch2, ch3);

                    v_uint16 lo, hi;
                    v_expand(ch0, lo, hi); vs16_0 = v_ಕೂಡಿಸು(vs16_0, v_ಕೂಡಿಸು(lo, hi));
                    v_expand(ch1, lo, hi); vs16_1 = v_ಕೂಡಿಸು(vs16_1, v_ಕೂಡಿಸು(lo, hi));
                    v_expand(ch2, lo, hi); vs16_2 = v_ಕೂಡಿಸು(vs16_2, v_ಕೂಡಿಸು(lo, hi));
                    v_expand(ch3, lo, hi); vs16_3 = v_ಕೂಡಿಸು(vs16_3, v_ಕೂಡಿಸು(lo, hi));

                    ಆದರೆ (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(vs16_0, a, b); vsum0 = v_ಕೂಡಿಸು(vsum0, v_ಕೂಡಿಸು(a, b));
                        v_expand(vs16_1, a, b); vsum1 = v_ಕೂಡಿಸು(vsum1, v_ಕೂಡಿಸು(a, b));
                        v_expand(vs16_2, a, b); vsum2 = v_ಕೂಡಿಸು(vsum2, v_ಕೂಡಿಸು(a, b));
                        v_expand(vs16_3, a, b); vsum3 = v_ಕೂಡಿಸು(vsum3, v_ಕೂಡಿಸು(a, b));
                        vs16_0 = vx_setzero_u16(); vs16_1 = vx_setzero_u16();
                        vs16_2 = vx_setzero_u16(); vs16_3 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(vs16_0, a, b); vsum0 = v_ಕೂಡಿಸು(vsum0, v_ಕೂಡಿಸು(a, b));
                    v_expand(vs16_1, a, b); vsum1 = v_ಕೂಡಿಸು(vsum1, v_ಕೂಡಿಸು(a, b));
                    v_expand(vs16_2, a, b); vsum2 = v_ಕೂಡಿಸು(vsum2, v_ಕೂಡಿಸು(a, b));
                    v_expand(vs16_3, a, b); vsum3 = v_ಕೂಡಿಸು(vsum3, v_ಕೂಡಿಸು(a, b));
                }

                dst[0] = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(vsum0);
                dst[1] = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(vsum1);
                dst[2] = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(vsum2);
                dst[3] = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(vsum3);

                ಗಾಗಿ (; x < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; x++)
                {
                    dst[0] += (ಪೂರ್ಣಾಂಕ)src[x * 4];
                    dst[1] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 1];
                    dst[2] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 2];
                    dst[3] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 3];
                }
#ಆದರೆ_ಅಂತ್ಯ
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                // generic cn: scalar fallback
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < cn; c++)
                    dst[c] = (ಪೂರ್ಣಾಂಕ)src[c];
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = cn; x < ಅಗಲ; x += cn)
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < cn; c++)
                        dst[c] += (ಪೂರ್ಣಾಂಕ)src[x + c];
            }
        }
    };

    parallel_for_(Range(0, srcmat.ಅಡ್ಡಸಾಲುಗಳು), body);
    v_cleanup();
}

// --- uchar → float ---
ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_8u32f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = srcmat.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = srcmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn;

    ಸ್ವಯಂಚಾಲಿತ body = [&](ಸ್ಥಿರಾಂಕ Range& range) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = range.ಆರಂಭ; y < range.end; y++)
        {
            ಸ್ಥಿರಾಂಕ uchar* src = srcmat.ptr<uchar>(y);
            ದಶಮಾಂಶ* dst = dstmat.ptr<ದಶಮಾಂಶ>(y);

            // compute in int, then convert to float
            Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ> ibuf(cn);
            ಪೂರ್ಣಾಂಕ* sums = ibuf.ದತ್ತಾಂಶ();
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < cn; c++)
                sums[c] = 0;

            ಆದರೆ (cn == 1)
            {
#ಆದರೆ defined(CV_NEON_DOT)
                ಸ್ಥಿರಾಂಕ uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t v_ಮೊತ್ತ = vdupq_n_u32(0);
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಅಗಲ - 16; x += 16)
                    v_ಮೊತ್ತ = vdotq_u32(v_ಮೊತ್ತ, vld1q_u8(src + x), ones);
                sums[0] = (ಪೂರ್ಣಾಂಕ)vaddvq_u32(v_ಮೊತ್ತ);
                ಗಾಗಿ (; x < ಅಗಲ; x++)
                    sums[0] += (ಪೂರ್ಣಾಂಕ)src[x];
#ಅಥವಾಆದರೆ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                uint32x4_t v_ಮೊತ್ತ = vdupq_n_u32(0);
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಅಗಲ - 16; x += 16)
                    v_ಮೊತ್ತ = vaddq_u32(v_ಮೊತ್ತ, vpaddlq_u16(vpaddlq_u8(vld1q_u8(src + x))));
                sums[0] = (ಪೂರ್ಣಾಂಕ)vaddvq_u32(v_ಮೊತ್ತ);
                ಗಾಗಿ (; x < ಅಗಲ; x++)
                    sums[0] += (ಪೂರ್ಣಾಂಕ)src[x];
#ಅಥವಾಆದರೆ CV_AVX2
                ಸ್ಥಿರಾಂಕ __m256i zero = _mm256_setzero_si256();
                __m256i vsum = zero;
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಅಗಲ - 32; x += 32)
                    vsum = mm256_ಕೂಡಿಸು_epi64(vsum, _mm256_sad_epu8(
                        _mm256_loadu_si256((ಸ್ಥಿರಾಂಕ __m256i*)(src + x)), zero));
                // 8-byte tail: reduce max scalar tail from 31 to 7
                // Use _mm256_inserti128_si256 instead of _mm256_castsi128_si256:
                // the latter leaves upper 128 bits undefined per Intel spec,
                // causing MSVC to expose stale register data to _mm256_sad_epu8.
                ಗಾಗಿ (; x <= ಅಗಲ - 8; x += 8)
                    vsum = mm256_ಕೂಡಿಸು_epi64(vsum, _mm256_sad_epu8(
                        _mm256_inserti128_si256(_mm256_setzero_si256(), _mm_loadl_epi64((ಸ್ಥಿರಾಂಕ __m128i*)(src + x)), 0), zero));
                __m128i lo128 = _mm256_castsi256_si128(vsum);
                __m128i hi128 = _mm256_extracti128_si256(vsum, 1);
                __m128i s = mm_ಕೂಡಿಸು_epi64(lo128, hi128);
                s = mm_ಕೂಡಿಸು_epi64(s, _mm_unpackhi_epi64(s, s));
                sums[0] = _mm_cvtsi128_si32(s);
                ಗಾಗಿ (; x < ಅಗಲ; x++)
                    sums[0] += (ಪೂರ್ಣಾಂಕ)src[x];
#ಅಥವಾಆದರೆ CV_SSE2
                ಸ್ಥಿರಾಂಕ __m128i zero = _mm_setzero_si128();
                __m128i vsum = zero;
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಅಗಲ - 16; x += 16)
                    vsum = mm_ಕೂಡಿಸು_epi64(vsum, _mm_sad_epu8(
                        _mm_loadu_si128((ಸ್ಥಿರಾಂಕ __m128i*)(src + x)), zero));
                __m128i s = mm_ಕೂಡಿಸು_epi64(vsum, _mm_unpackhi_epi64(vsum, vsum));
                sums[0] = _mm_cvtsi128_si32(s);
                ಗಾಗಿ (; x < ಅಗಲ; x++)
                    sums[0] += (ಪೂರ್ಣಾಂಕ)src[x];
#ಇಲ್ಲದಿದ್ದರೆ
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 v_ಮೊತ್ತ = vx_setzero_u32();
                v_uint16 v_sum16 = vx_setzero_u16();
                ಪೂರ್ಣಾಂಕ x = 0, batch = 0;

                ಗಾಗಿ (; x <= ಅಗಲ - vlanes8; x += vlanes8)
                {
                    v_uint16 lo, hi;
                    v_expand(vx_ತುಂಬು(src + x), lo, hi);
                    v_sum16 = v_ಕೂಡಿಸು(v_sum16, v_ಕೂಡಿಸು(lo, hi));
                    ಆದರೆ (++batch >= 128)
                    {
                        v_uint32 a, b;
                        v_expand(v_sum16, a, b);
                        v_ಮೊತ್ತ = v_ಕೂಡಿಸು(v_ಮೊತ್ತ, v_ಕೂಡಿಸು(a, b));
                        v_sum16 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                v_uint32 a, b;
                v_expand(v_sum16, a, b);
                v_ಮೊತ್ತ = v_ಕೂಡಿಸು(v_ಮೊತ್ತ, v_ಕೂಡಿಸು(a, b));

                sums[0] = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(v_ಮೊತ್ತ);
                ಗಾಗಿ (; x < ಅಗಲ; x++)
                    sums[0] += (ಪೂರ್ಣಾಂಕ)src[x];
#ಆದರೆ_ಅಂತ್ಯ
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cn == 4)
            {
#ಆದರೆ defined(CV_NEON_DOT)
                ಸ್ಥಿರಾಂಕ uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t acc0 = vdupq_n_u32(0), acc1 = vdupq_n_u32(0);
                uint32x4_t acc2 = vdupq_n_u32(0), acc3 = vdupq_n_u32(0);
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    acc0 = vdotq_u32(acc0, v.val[0], ones);
                    acc1 = vdotq_u32(acc1, v.val[1], ones);
                    acc2 = vdotq_u32(acc2, v.val[2], ones);
                    acc3 = vdotq_u32(acc3, v.val[3], ones);
                }
                sums[0] = (ಪೂರ್ಣಾಂಕ)vaddvq_u32(acc0);
                sums[1] = (ಪೂರ್ಣಾಂಕ)vaddvq_u32(acc1);
                sums[2] = (ಪೂರ್ಣಾಂಕ)vaddvq_u32(acc2);
                sums[3] = (ಪೂರ್ಣಾಂಕ)vaddvq_u32(acc3);
                ಗಾಗಿ (; x < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; x++)
                {
                    sums[0] += (ಪೂರ್ಣಾಂಕ)src[x * 4];
                    sums[1] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 1];
                    sums[2] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 2];
                    sums[3] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 3];
                }
#ಅಥವಾಆದರೆ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    sums[0] += (ಪೂರ್ಣಾಂಕ)vaddlvq_u8(v.val[0]);
                    sums[1] += (ಪೂರ್ಣಾಂಕ)vaddlvq_u8(v.val[1]);
                    sums[2] += (ಪೂರ್ಣಾಂಕ)vaddlvq_u8(v.val[2]);
                    sums[3] += (ಪೂರ್ಣಾಂಕ)vaddlvq_u8(v.val[3]);
                }
                ಗಾಗಿ (; x < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; x++)
                {
                    sums[0] += (ಪೂರ್ಣಾಂಕ)src[x * 4];
                    sums[1] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 1];
                    sums[2] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 2];
                    sums[3] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 3];
                }
#ಅಥವಾಆದರೆ CV_AVX2
                // Intel AVX2: shuffle + unpack to group channels, full-register SAD
                ಸ್ಥಿರಾಂಕ __m256i zero = _mm256_setzero_si256();
                ಸ್ಥಿರಾಂಕ __m256i shuf_ಮುಖವಾಡ = _mm256_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                ಪೂರ್ಣಾಂಕ x = 0;
                // 16 pixels/iter with unpack (full-register SAD)
                {
                    __m256i ಮೊತ್ತ_bg = zero, ಮೊತ್ತ_ra = zero;
                    ಗಾಗಿ (; x <= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 16; x += 16)
                    {
                        __m256i v0 = _mm256_loadu_si256((ಸ್ಥಿರಾಂಕ __m256i*)(src + x * 4));
                        __m256i v1 = _mm256_loadu_si256((ಸ್ಥಿರಾಂಕ __m256i*)(src + x * 4 + 32));
                        __m256i g0 = _mm256_shuffle_epi8(v0, shuf_ಮುಖವಾಡ);
                        __m256i g1 = _mm256_shuffle_epi8(v1, shuf_ಮುಖವಾಡ);
                        __m256i bg = _mm256_unpacklo_epi32(g0, g1);
                        __m256i ra = _mm256_unpackhi_epi32(g0, g1);
                        ಮೊತ್ತ_bg = mm256_ಕೂಡಿಸು_epi64(ಮೊತ್ತ_bg, _mm256_sad_epu8(bg, zero));
                        ಮೊತ್ತ_ra = mm256_ಕೂಡಿಸು_epi64(ಮೊತ್ತ_ra, _mm256_sad_epu8(ra, zero));
                    }
                    __m128i bg_lo = _mm256_castsi256_si128(ಮೊತ್ತ_bg);
                    __m128i bg_hi = _mm256_extracti128_si256(ಮೊತ್ತ_bg, 1);
                    __m128i bg_s = mm_ಕೂಡಿಸು_epi64(bg_lo, bg_hi);
                    __m128i ra_lo = _mm256_castsi256_si128(ಮೊತ್ತ_ra);
                    __m128i ra_hi = _mm256_extracti128_si256(ಮೊತ್ತ_ra, 1);
                    __m128i ra_s = mm_ಕೂಡಿಸು_epi64(ra_lo, ra_hi);
                    sums[0] = _mm_cvtsi128_si32(bg_s);                     // B
                    sums[1] = _mm_cvtsi128_si32(_mm_srli_si128(bg_s, 8));  // G
                    sums[2] = _mm_cvtsi128_si32(ra_s);                     // R
                    sums[3] = _mm_cvtsi128_si32(_mm_srli_si128(ra_s, 8));  // A
                }
                // 8-pixel tail with and/srli
                {
                    ಸ್ಥಿರಾಂಕ __m256i ಮುಖವಾಡ_lo32 = _mm256_set1_epi64x(0x00000000FFFFFFFFLL);
                    __m256i ಮೊತ್ತ_br = zero, ಮೊತ್ತ_ga = zero;
                    ಗಾಗಿ (; x <= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 8; x += 8)
                    {
                        __m256i v = _mm256_loadu_si256((ಸ್ಥಿರಾಂಕ __m256i*)(src + x * 4));
                        __m256i grouped = _mm256_shuffle_epi8(v, shuf_ಮುಖವಾಡ);
                        __m256i br = _mm256_and_si256(grouped, ಮುಖವಾಡ_lo32);
                        __m256i ga = _mm256_srli_epi64(grouped, 32);
                        ಮೊತ್ತ_br = mm256_ಕೂಡಿಸು_epi64(ಮೊತ್ತ_br, _mm256_sad_epu8(br, zero));
                        ಮೊತ್ತ_ga = mm256_ಕೂಡಿಸು_epi64(ಮೊತ್ತ_ga, _mm256_sad_epu8(ga, zero));
                    }
                    __m128i br_lo = _mm256_castsi256_si128(ಮೊತ್ತ_br);
                    __m128i br_hi = _mm256_extracti128_si256(ಮೊತ್ತ_br, 1);
                    __m128i br_s = mm_ಕೂಡಿಸು_epi64(br_lo, br_hi);
                    __m128i ga_lo = _mm256_castsi256_si128(ಮೊತ್ತ_ga);
                    __m128i ga_hi = _mm256_extracti128_si256(ಮೊತ್ತ_ga, 1);
                    __m128i ga_s = mm_ಕೂಡಿಸು_epi64(ga_lo, ga_hi);
                    sums[0] += _mm_cvtsi128_si32(br_s);
                    sums[1] += _mm_cvtsi128_si32(ga_s);
                    sums[2] += _mm_cvtsi128_si32(_mm_srli_si128(br_s, 8));
                    sums[3] += _mm_cvtsi128_si32(_mm_srli_si128(ga_s, 8));
                }
                ಗಾಗಿ (; x < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; x++)
                {
                    sums[0] += (ಪೂರ್ಣಾಂಕ)src[x * 4];
                    sums[1] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 1];
                    sums[2] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 2];
                    sums[3] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 3];
                }
#ಅಥವಾಆದರೆ CV_SSSE3
                // Intel SSSE3: _mm_shuffle_epi8 deinterleave + _mm_sad_epu8
                ಸ್ಥಿರಾಂಕ __m128i zero = _mm_setzero_si128();
                ಸ್ಥಿರಾಂಕ __m128i shuf_ಮುಖವಾಡ = _mm_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                ಸ್ಥಿರಾಂಕ __m128i ಮುಖವಾಡ_lo32 = _mm_set1_epi64x(0x00000000FFFFFFFFLL);
                __m128i ಮೊತ್ತ_br = zero, ಮೊತ್ತ_ga = zero;
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 4; x += 4)
                {
                    __m128i v = _mm_loadu_si128((ಸ್ಥಿರಾಂಕ __m128i*)(src + x * 4));
                    __m128i grouped = _mm_shuffle_epi8(v, shuf_ಮುಖವಾಡ);
                    __m128i br = _mm_and_si128(grouped, ಮುಖವಾಡ_lo32);
                    __m128i ga = _mm_srli_epi64(grouped, 32);
                    ಮೊತ್ತ_br = mm_ಕೂಡಿಸು_epi64(ಮೊತ್ತ_br, _mm_sad_epu8(br, zero));
                    ಮೊತ್ತ_ga = mm_ಕೂಡಿಸು_epi64(ಮೊತ್ತ_ga, _mm_sad_epu8(ga, zero));
                }
                sums[0] = _mm_cvtsi128_si32(ಮೊತ್ತ_br);
                sums[1] = _mm_cvtsi128_si32(ಮೊತ್ತ_ga);
                sums[2] = _mm_cvtsi128_si32(_mm_srli_si128(ಮೊತ್ತ_br, 8));
                sums[3] = _mm_cvtsi128_si32(_mm_srli_si128(ಮೊತ್ತ_ga, 8));
                ಗಾಗಿ (; x < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; x++)
                {
                    sums[0] += (ಪೂರ್ಣಾಂಕ)src[x * 4];
                    sums[1] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 1];
                    sums[2] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 2];
                    sums[3] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 3];
                }
#ಇಲ್ಲದಿದ್ದರೆ
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 vsum0 = vx_setzero_u32(), vsum1 = vx_setzero_u32();
                v_uint32 vsum2 = vx_setzero_u32(), vsum3 = vx_setzero_u32();
                v_uint16 vs16_0 = vx_setzero_u16(), vs16_1 = vx_setzero_u16();
                v_uint16 vs16_2 = vx_setzero_u16(), vs16_3 = vx_setzero_u16();
                ಪೂರ್ಣಾಂಕ x = 0, batch = 0;
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ flush_at = 128;

                ಗಾಗಿ (; x <= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - vlanes8; x += vlanes8)
                {
                    v_uint8 ch0, ch1, ch2, ch3;
                    v_ತುಂಬು_deinterleave(src + x * 4, ch0, ch1, ch2, ch3);

                    v_uint16 lo, hi;
                    v_expand(ch0, lo, hi); vs16_0 = v_ಕೂಡಿಸು(vs16_0, v_ಕೂಡಿಸು(lo, hi));
                    v_expand(ch1, lo, hi); vs16_1 = v_ಕೂಡಿಸು(vs16_1, v_ಕೂಡಿಸು(lo, hi));
                    v_expand(ch2, lo, hi); vs16_2 = v_ಕೂಡಿಸು(vs16_2, v_ಕೂಡಿಸು(lo, hi));
                    v_expand(ch3, lo, hi); vs16_3 = v_ಕೂಡಿಸು(vs16_3, v_ಕೂಡಿಸು(lo, hi));

                    ಆದರೆ (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(vs16_0, a, b); vsum0 = v_ಕೂಡಿಸು(vsum0, v_ಕೂಡಿಸು(a, b));
                        v_expand(vs16_1, a, b); vsum1 = v_ಕೂಡಿಸು(vsum1, v_ಕೂಡಿಸು(a, b));
                        v_expand(vs16_2, a, b); vsum2 = v_ಕೂಡಿಸು(vsum2, v_ಕೂಡಿಸು(a, b));
                        v_expand(vs16_3, a, b); vsum3 = v_ಕೂಡಿಸು(vsum3, v_ಕೂಡಿಸು(a, b));
                        vs16_0 = vx_setzero_u16(); vs16_1 = vx_setzero_u16();
                        vs16_2 = vx_setzero_u16(); vs16_3 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(vs16_0, a, b); vsum0 = v_ಕೂಡಿಸು(vsum0, v_ಕೂಡಿಸು(a, b));
                    v_expand(vs16_1, a, b); vsum1 = v_ಕೂಡಿಸು(vsum1, v_ಕೂಡಿಸು(a, b));
                    v_expand(vs16_2, a, b); vsum2 = v_ಕೂಡಿಸು(vsum2, v_ಕೂಡಿಸು(a, b));
                    v_expand(vs16_3, a, b); vsum3 = v_ಕೂಡಿಸು(vsum3, v_ಕೂಡಿಸು(a, b));
                }

                sums[0] = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(vsum0);
                sums[1] = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(vsum1);
                sums[2] = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(vsum2);
                sums[3] = (ಪೂರ್ಣಾಂಕ)v_reduce_ಮೊತ್ತ(vsum3);

                ಗಾಗಿ (; x < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; x++)
                {
                    sums[0] += (ಪೂರ್ಣಾಂಕ)src[x * 4];
                    sums[1] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 1];
                    sums[2] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 2];
                    sums[3] += (ಪೂರ್ಣಾಂಕ)src[x * 4 + 3];
                }
#ಆದರೆ_ಅಂತ್ಯ
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0, c = 0; x < ಅಗಲ; x++, c = (c+1)&-(c < cn-1))
                    sums[c] += (ಪೂರ್ಣಾಂಕ)src[x];
            }
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < cn; c++)
                dst[c] = (ದಶಮಾಂಶ)sums[c];
        }
    };
    parallel_for_(Range(0, srcmat.ಅಡ್ಡಸಾಲುಗಳು), body);
    v_cleanup();
}

// --- 16-bit (ushort/short) → float ---
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು SrcT, ಬಗೆ_ಹೆಸರು VecSrc, ಬಗೆ_ಹೆಸರು VecDst>
ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_16_32f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = srcmat.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = srcmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<VecSrc>::vlanes();

    ಸ್ವಯಂಚಾಲಿತ body = [&](ಸ್ಥಿರಾಂಕ Range& range) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = range.ಆರಂಭ; y < range.end; y++)
        {
            ಸ್ಥಿರಾಂಕ SrcT* src = srcmat.ptr<SrcT>(y);
            ದಶಮಾಂಶ* dst = dstmat.ptr<ದಶಮಾಂಶ>(y);

            ಆದರೆ (cn == 1)
            {
                VecDst v_sum0 = v_setzero_<VecDst>(), v_sum1 = v_setzero_<VecDst>();
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಅಗಲ - vlanes; x += vlanes)
                {
                    VecDst lo, hi;
                    v_expand(vx_ತುಂಬು(src + x), lo, hi);
                    v_sum0 = v_ಕೂಡಿಸು(v_sum0, lo);
                    v_sum1 = v_ಕೂಡಿಸು(v_sum1, hi);
                }
                dst[0] = (ದಶಮಾಂಶ)(v_reduce_ಮೊತ್ತ(v_sum0) + v_reduce_ಮೊತ್ತ(v_sum1));
                ಗಾಗಿ (; x < ಅಗಲ; x++)
                    dst[0] += (ದಶಮಾಂಶ)src[x];
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < cn; c++)
                    dst[c] = 0;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0, c = 0; x < ಅಗಲ; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += (ದಶಮಾಂಶ)src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.ಅಡ್ಡಸಾಲುಗಳು), body);
    v_cleanup();
}

ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_16u32f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_16_32f<ushort, v_uint16, v_uint32>(srcmat, dstmat); }

ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_16s32f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_16_32f<ಗಿಡ್ಡ, v_int16, v_int32>(srcmat, dstmat); }

// --- float/double → same type ---
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು VecT>
ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_FP(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = srcmat.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = srcmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<VecT>::vlanes();

    ಸ್ವಯಂಚಾಲಿತ body = [&](ಸ್ಥಿರಾಂಕ Range& range) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = range.ಆರಂಭ; y < range.end; y++)
        {
            ಸ್ಥಿರಾಂಕ T* src = srcmat.ptr<T>(y);
            T* dst = dstmat.ptr<T>(y);

            ಆದರೆ (cn == 1)
            {
                VecT s0 = v_setzero_<VecT>(), s1 = v_setzero_<VecT>();
                VecT s2 = v_setzero_<VecT>(), s3 = v_setzero_<VecT>();
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಅಗಲ - vlanes * 4; x += vlanes * 4)
                {
                    s0 = v_ಕೂಡಿಸು(s0, vx_ತುಂಬು(src + x));
                    s1 = v_ಕೂಡಿಸು(s1, vx_ತುಂಬು(src + x + vlanes));
                    s2 = v_ಕೂಡಿಸು(s2, vx_ತುಂಬು(src + x + vlanes * 2));
                    s3 = v_ಕೂಡಿಸು(s3, vx_ತುಂಬು(src + x + vlanes * 3));
                }
                s0 = v_ಕೂಡಿಸು(v_ಕೂಡಿಸು(s0, s1), v_ಕೂಡಿಸು(s2, s3));
                ಗಾಗಿ (; x <= ಅಗಲ - vlanes; x += vlanes)
                    s0 = v_ಕೂಡಿಸು(s0, vx_ತುಂಬು(src + x));
                T ಒಟ್ಟು = (T)v_reduce_ಮೊತ್ತ(s0);
                ಗಾಗಿ (; x < ಅಗಲ; x++)
                    ಒಟ್ಟು += src[x];
                dst[0] = ಒಟ್ಟು;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < cn; c++)
                    dst[c] = 0;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0, c = 0; x < ಅಗಲ; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.ಅಡ್ಡಸಾಲುಗಳು), body);
    v_cleanup();
}

ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_32f32f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_FP<ದಶಮಾಂಶ, v_float32>(srcmat, dstmat); }

#ಆದರೆ CV_SIMD_64F
// --- float → double ---
ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_32f64f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = srcmat.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = srcmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes32 = VTraits<v_float32>::vlanes();

    ಸ್ವಯಂಚಾಲಿತ body = [&](ಸ್ಥಿರಾಂಕ Range& range) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = range.ಆರಂಭ; y < range.end; y++)
        {
            ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src = srcmat.ptr<ದಶಮಾಂಶ>(y);
            ದ್ವಿಗುಣ* dst = dstmat.ptr<ದ್ವಿಗುಣ>(y);

            ಆದರೆ (cn == 1)
            {
                v_float64 v_sum0 = vx_setzero_f64();
                v_float64 v_sum1 = vx_setzero_f64();
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x <= ಅಗಲ - vlanes32; x += vlanes32)
                {
                    v_float32 v = vx_ತುಂಬು(src + x);
                    v_sum0 = v_ಕೂಡಿಸು(v_sum0, v_cvt_f64(v));
                    v_sum1 = v_ಕೂಡಿಸು(v_sum1, v_cvt_f64_ಮೇಲಿನ(v));
                }
                v_float64 v_ಒಟ್ಟು = v_ಕೂಡಿಸು(v_sum0, v_sum1);

                ದ್ವಿಗುಣ ಒಟ್ಟು = v_reduce_ಮೊತ್ತ(v_ಒಟ್ಟು);
                ಗಾಗಿ (; x < ಅಗಲ; x++)
                    ಒಟ್ಟು += (ದ್ವಿಗುಣ)src[x];
                dst[0] = ಒಟ್ಟು;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < cn; c++)
                    dst[c] = 0;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0, c = 0; x < ಅಗಲ; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += (ದ್ವಿಗುಣ)src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.ಅಡ್ಡಸಾಲುಗಳು), body);
    v_cleanup();
}

ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_64f64f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_FP<ದ್ವಿಗುಣ, v_float64>(srcmat, dstmat); }
#ಆದರೆ_ಅಂತ್ಯ // CV_SIMD_64F

// =====================================================================
//  Row reduce SUM (dim=0): sum each column across all rows
// =====================================================================

// --- uchar → int ---
// Uses u16 intermediate accumulator for vertical sum: accumulate u8→u16 per row,
// flush u16→u32 every 256 rows (max u16 value: 256*255=65280 < 65535)
ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_8u32s(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ_cn = srcmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * srcmat.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎತ್ತರ = srcmat.ಅಡ್ಡಸಾಲುಗಳು;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes16 = VTraits<v_uint16>::vlanes();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes32 = VTraits<v_int32>::vlanes();

    ಸ್ವಯಂಚಾಲಿತ body = [&](ಸ್ಥಿರಾಂಕ Range& range) {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಆರಂಭ = range.ಆರಂಭ;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ end = range.end;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ len = end - ಆರಂಭ;

        Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ> buf32_storage(len);
        Auto_ಬಫರ್<ushort> buf16_storage(len);
        ಪೂರ್ಣಾಂಕ* buf32 = buf32_storage.ದತ್ತಾಂಶ();
        ushort* buf16 = buf16_storage.ದತ್ತಾಂಶ();

        // init from first row
        ಸ್ಥಿರಾಂಕ uchar* src0 = srcmat.ptr<uchar>(0) + ಆರಂಭ;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
            buf32[i] = (ಪೂರ್ಣಾಂಕ)src0[i];

        ಆದರೆ (ಎತ್ತರ <= 1)
        {
            ಪೂರ್ಣಾಂಕ* dst = dstmat.ptr<ಪೂರ್ಣಾಂಕ>(0) + ಆರಂಭ;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
                dst[i] = buf32[i];
            ಹಿಂತಿರುಗಿ;
        }

        memset(buf16, 0, len * ಗಾತ್ರ(ushort));
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ flush_at = 256; // 256*255 = 65280 < 65535 (u16 safe)
        ಪೂರ್ಣಾಂಕ flush_ಎಣಿಕೆ = 0;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = 1; ಅಡ್ಡಸಾಲು < ಎತ್ತರ; ಅಡ್ಡಸಾಲು++)
        {
            ಸ್ಥಿರಾಂಕ uchar* src = srcmat.ptr<uchar>(ಅಡ್ಡಸಾಲು) + ಆರಂಭ;
            ಪೂರ್ಣಾಂಕ i = 0;

#ಆದರೆ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
            // AArch64: vaddw_u8 — single-instruction widening add (u16 += u8)
            ಗಾಗಿ (; i <= len - 16; i += 16)
            {
                uint8x16_t v = vld1q_u8(src + i);
                uint16x8_t acc_lo = vld1q_u16(buf16 + i);
                uint16x8_t acc_hi = vld1q_u16(buf16 + i + 8);
                vst1q_u16(buf16 + i,     vaddw_u8(acc_lo, vget_ಕೆಳಗಿನ_u8(v)));
                vst1q_u16(buf16 + i + 8, vaddw_ಮೇಲಿನ_u8(acc_hi, v));
            }
#ಅಥವಾಆದರೆ CV_RVV
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vl = (ಪೂರ್ಣಾಂಕ)__riscv_vsetvlmax_e8m2();
            ಗಾಗಿ (; i <= len - vl; i += vl)
            {
                vuint8m2_t vs = __riscv_vle8_v_u8m2(src + i, vl);
                vuint16m4_t va = __riscv_vle16_v_u16m4(buf16 + i, vl);
                va = __riscv_vwaddu_wv_u16m4(va, vs, vl);
                __riscv_vse16_v_u16m4(buf16 + i, va, vl);
            }
#ಇಲ್ಲದಿದ್ದರೆ
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes8 = VTraits<v_uint8>::vlanes();
            ಗಾಗಿ (; i <= len - vlanes8; i += vlanes8)
            {
                v_uint16 lo, hi;
                v_expand(vx_ತುಂಬು(src + i), lo, hi);
                v_store(buf16 + i, v_ಕೂಡಿಸು(vx_ತುಂಬು(buf16 + i), lo));
                v_store(buf16 + i + vlanes16, v_ಕೂಡಿಸು(vx_ತುಂಬು(buf16 + i + vlanes16), hi));
            }
#ಆದರೆ_ಅಂತ್ಯ
            ಗಾಗಿ (; i < len; i++)
                buf16[i] += (ushort)src[i];

            ಆದರೆ (++flush_ಎಣಿಕೆ >= flush_at || ಅಡ್ಡಸಾಲು == ಎತ್ತರ - 1)
            {
                // flush u16 → u32
                ಪೂರ್ಣಾಂಕ j = 0;
                ಗಾಗಿ (; j <= len - vlanes16; j += vlanes16)
                {
                    v_uint32 lo, hi;
                    v_expand(vx_ತುಂಬು(buf16 + j), lo, hi);
                    v_store(buf32 + j, v_ಕೂಡಿಸು(vx_ತುಂಬು(buf32 + j), v_reinterpret_as_s32(lo)));
                    v_store(buf32 + j + vlanes32, v_ಕೂಡಿಸು(vx_ತುಂಬು(buf32 + j + vlanes32), v_reinterpret_as_s32(hi)));
                }
                ಗಾಗಿ (; j < len; j++)
                    buf32[j] += (ಪೂರ್ಣಾಂಕ)buf16[j];
                memset(buf16, 0, len * ಗಾತ್ರ(ushort));
                flush_ಎಣಿಕೆ = 0;
            }
        }

        ಪೂರ್ಣಾಂಕ* dst = dstmat.ptr<ಪೂರ್ಣಾಂಕ>(0) + ಆರಂಭ;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
            dst[i] = buf32[i];
    };

    parallel_for_(Range(0, ಅಗಲ_cn), body, ಅಗಲ_cn * CV_ELEM_ಗಾತ್ರ(srcmat.ಆಳ()) / 64);
    v_cleanup();
}

// --- uchar → float ---
ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_8u32f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{
    // compute in int, then convert to float
    Mat ತಾತ್ಕಾಲಿಕ_2(dstmat.ಅಡ್ಡಸಾಲುಗಳು, dstmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, CV_32SC(srcmat.channels()));
    reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_8u32s(srcmat, ತಾತ್ಕಾಲಿಕ_2);
    ತಾತ್ಕಾಲಿಕ_2.convert_ಗಮ್ಯ(dstmat, dstmat.type());
}

// --- 16-bit (ushort/short) → float row reduce ---
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು SrcT, ಬಗೆ_ಹೆಸರು VecSrc, ಬಗೆ_ಹೆಸರು VecDst>
ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_16_32f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ_cn = srcmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * srcmat.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎತ್ತರ = srcmat.ಅಡ್ಡಸಾಲುಗಳು;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes16 = VTraits<VecSrc>::vlanes();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes32 = vlanes16 / 2;

    ಸ್ವಯಂಚಾಲಿತ body = [&](ಸ್ಥಿರಾಂಕ Range& range) {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಆರಂಭ = range.ಆರಂಭ;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ end = range.end;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ len = end - ಆರಂಭ;
        ದಶಮಾಂಶ* dst = dstmat.ptr<ದಶಮಾಂಶ>(0) + ಆರಂಭ;

        ಪೂರ್ಣಾಂಕ i = 0;
        ಗಾಗಿ (; i <= len - vlanes16 * 4; i += vlanes16 * 4)
        {
            ಸ್ಥಿರಾಂಕ SrcT* src0 = srcmat.ptr<SrcT>(0) + ಆರಂಭ + i;
            VecDst a0, a1, a2, a3, a4, a5, a6, a7;
            v_expand(vx_ತುಂಬು(src0), a0, a1);
            v_expand(vx_ತುಂಬು(src0 + vlanes16), a2, a3);
            v_expand(vx_ತುಂಬು(src0 + vlanes16*2), a4, a5);
            v_expand(vx_ತುಂಬು(src0 + vlanes16*3), a6, a7);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = 1; ಅಡ್ಡಸಾಲು < ಎತ್ತರ; ಅಡ್ಡಸಾಲು++)
            {
                ಸ್ಥಿರಾಂಕ SrcT* src = srcmat.ptr<SrcT>(ಅಡ್ಡಸಾಲು) + ಆರಂಭ + i;
                VecDst lo, hi;
                v_expand(vx_ತುಂಬು(src), lo, hi);
                a0 = v_ಕೂಡಿಸು(a0, lo); a1 = v_ಕೂಡಿಸು(a1, hi);
                v_expand(vx_ತುಂಬು(src + vlanes16), lo, hi);
                a2 = v_ಕೂಡಿಸು(a2, lo); a3 = v_ಕೂಡಿಸು(a3, hi);
                v_expand(vx_ತುಂಬು(src + vlanes16*2), lo, hi);
                a4 = v_ಕೂಡಿಸು(a4, lo); a5 = v_ಕೂಡಿಸು(a5, hi);
                v_expand(vx_ತುಂಬು(src + vlanes16*3), lo, hi);
                a6 = v_ಕೂಡಿಸು(a6, lo); a7 = v_ಕೂಡಿಸು(a7, hi);
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
        ಗಾಗಿ (; i <= len - vlanes16; i += vlanes16)
        {
            VecDst a0, a1;
            v_expand(vx_ತುಂಬು(srcmat.ptr<SrcT>(0) + ಆರಂಭ + i), a0, a1);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = 1; ಅಡ್ಡಸಾಲು < ಎತ್ತರ; ಅಡ್ಡಸಾಲು++)
            {
                VecDst lo, hi;
                v_expand(vx_ತುಂಬು(srcmat.ptr<SrcT>(ಅಡ್ಡಸಾಲು) + ಆರಂಭ + i), lo, hi);
                a0 = v_ಕೂಡಿಸು(a0, lo); a1 = v_ಕೂಡಿಸು(a1, hi);
            }
            v_store(dst + i, v_cvt_f32(v_reinterpret_as_s32(a0)));
            v_store(dst + i + vlanes32, v_cvt_f32(v_reinterpret_as_s32(a1)));
        }
        ಗಾಗಿ (; i < len; i++)
        {
            ಪೂರ್ಣಾಂಕ val = (ಪೂರ್ಣಾಂಕ)*(srcmat.ptr<SrcT>(0) + ಆರಂಭ + i);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = 1; ಅಡ್ಡಸಾಲು < ಎತ್ತರ; ಅಡ್ಡಸಾಲು++)
                val += (ಪೂರ್ಣಾಂಕ)*(srcmat.ptr<SrcT>(ಅಡ್ಡಸಾಲು) + ಆರಂಭ + i);
            dst[i] = (ದಶಮಾಂಶ)val;
        }
    };

    parallel_for_(Range(0, ಅಗಲ_cn), body, ಅಗಲ_cn * CV_ELEM_ಗಾತ್ರ(srcmat.ಆಳ()) / 64);
    v_cleanup();
}

ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_16u32f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_16_32f<ushort, v_uint16, v_uint32>(srcmat, dstmat); }

ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_16s32f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_16_32f<ಗಿಡ್ಡ, v_int16, v_int32>(srcmat, dstmat); }

// --- float/double → same type row reduce ---
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು VecT>
ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_FP(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ_cn = srcmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * srcmat.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎತ್ತರ = srcmat.ಅಡ್ಡಸಾಲುಗಳು;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<VecT>::vlanes();

    ಸ್ವಯಂಚಾಲಿತ body = [&](ಸ್ಥಿರಾಂಕ Range& range) {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಆರಂಭ = range.ಆರಂಭ;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ end = range.end;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ len = end - ಆರಂಭ;
        T* dst = dstmat.ptr<T>(0) + ಆರಂಭ;

        ಪೂರ್ಣಾಂಕ i = 0;
        ಗಾಗಿ (; i <= len - vlanes * 8; i += vlanes * 8)
        {
            ಸ್ಥಿರಾಂಕ T* src0 = srcmat.ptr<T>(0) + ಆರಂಭ + i;
            VecT s0 = vx_ತುಂಬು(src0), s1 = vx_ತುಂಬು(src0 + vlanes);
            VecT s2 = vx_ತುಂಬು(src0 + vlanes*2), s3 = vx_ತುಂಬು(src0 + vlanes*3);
            VecT s4 = vx_ತುಂಬು(src0 + vlanes*4), s5 = vx_ತುಂಬು(src0 + vlanes*5);
            VecT s6 = vx_ತುಂಬು(src0 + vlanes*6), s7 = vx_ತುಂಬು(src0 + vlanes*7);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = 1; ಅಡ್ಡಸಾಲು < ಎತ್ತರ; ಅಡ್ಡಸಾಲು++)
            {
                ಸ್ಥಿರಾಂಕ T* src = srcmat.ptr<T>(ಅಡ್ಡಸಾಲು) + ಆರಂಭ + i;
                s0 = v_ಕೂಡಿಸು(s0, vx_ತುಂಬು(src));
                s1 = v_ಕೂಡಿಸು(s1, vx_ತುಂಬು(src + vlanes));
                s2 = v_ಕೂಡಿಸು(s2, vx_ತುಂಬು(src + vlanes*2));
                s3 = v_ಕೂಡಿಸು(s3, vx_ತುಂಬು(src + vlanes*3));
                s4 = v_ಕೂಡಿಸು(s4, vx_ತುಂಬು(src + vlanes*4));
                s5 = v_ಕೂಡಿಸು(s5, vx_ತುಂಬು(src + vlanes*5));
                s6 = v_ಕೂಡಿಸು(s6, vx_ತುಂಬು(src + vlanes*6));
                s7 = v_ಕೂಡಿಸು(s7, vx_ತುಂಬು(src + vlanes*7));
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
        ಗಾಗಿ (; i <= len - vlanes; i += vlanes)
        {
            VecT s0 = vx_ತುಂಬು(srcmat.ptr<T>(0) + ಆರಂಭ + i);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = 1; ಅಡ್ಡಸಾಲು < ಎತ್ತರ; ಅಡ್ಡಸಾಲು++)
                s0 = v_ಕೂಡಿಸು(s0, vx_ತುಂಬು(srcmat.ptr<T>(ಅಡ್ಡಸಾಲು) + ಆರಂಭ + i));
            v_store(dst + i, s0);
        }
        ಗಾಗಿ (; i < len; i++)
        {
            T val = *(srcmat.ptr<T>(0) + ಆರಂಭ + i);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = 1; ಅಡ್ಡಸಾಲು < ಎತ್ತರ; ಅಡ್ಡಸಾಲು++)
                val += *(srcmat.ptr<T>(ಅಡ್ಡಸಾಲು) + ಆರಂಭ + i);
            dst[i] = val;
        }
    };

    parallel_for_(Range(0, ಅಗಲ_cn), body, ಅಗಲ_cn * CV_ELEM_ಗಾತ್ರ(srcmat.ಆಳ()) / 64);
    v_cleanup();
}

ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_32f32f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_FP<ದಶಮಾಂಶ, v_float32>(srcmat, dstmat); }

#ಆದರೆ CV_SIMD_64F
// --- float → double ---
ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_32f64f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ_cn = srcmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * srcmat.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎತ್ತರ = srcmat.ಅಡ್ಡಸಾಲುಗಳು;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes32 = VTraits<v_float32>::vlanes();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes64 = VTraits<v_float64>::vlanes();

    ಸ್ವಯಂಚಾಲಿತ body = [&](ಸ್ಥಿರಾಂಕ Range& range) {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಆರಂಭ = range.ಆರಂಭ;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ end = range.end;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ len = end - ಆರಂಭ;

        Auto_ಬಫರ್<ದ್ವಿಗುಣ> buf_storage(len);
        ದ್ವಿಗುಣ* buf = buf_storage.ದತ್ತಾಂಶ();

        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src0 = srcmat.ptr<ದಶಮಾಂಶ>(0) + ಆರಂಭ;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < len; i++)
            buf[i] = (ದ್ವಿಗುಣ)src0[i];

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = 1; ಅಡ್ಡಸಾಲು < ಎತ್ತರ; ಅಡ್ಡಸಾಲು++)
        {
            ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src = srcmat.ptr<ದಶಮಾಂಶ>(ಅಡ್ಡಸಾಲು) + ಆರಂಭ;
            ಪೂರ್ಣಾಂಕ i = 0;
            ಗಾಗಿ (; i <= len - vlanes32; i += vlanes32)
            {
                v_float32 v = vx_ತುಂಬು(src + i);
                v_store(buf + i, v_ಕೂಡಿಸು(vx_ತುಂಬು(buf + i), v_cvt_f64(v)));
                v_store(buf + i + vlanes64, v_ಕೂಡಿಸು(vx_ತುಂಬು(buf + i + vlanes64), v_cvt_f64_ಮೇಲಿನ(v)));
            }
            ಗಾಗಿ (; i < len; i++)
                buf[i] += (ದ್ವಿಗುಣ)src[i];
        }

        ದ್ವಿಗುಣ* dst = dstmat.ptr<ದ್ವಿಗುಣ>(0) + ಆರಂಭ;
        ಸ್ಮರಣೆ_ನಕಲು(dst, buf, len * ಗಾತ್ರ(ದ್ವಿಗುಣ));
    };

    parallel_for_(Range(0, ಅಗಲ_cn), body, ಅಗಲ_cn * CV_ELEM_ಗಾತ್ರ(srcmat.ಆಳ()) / 64);
    v_cleanup();
}

ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_64f64f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_FP<ದ್ವಿಗುಣ, v_float64>(srcmat, dstmat); }
#ಆದರೆ_ಅಂತ್ಯ // CV_SIMD_64F

#ಆದರೆ_ಅಂತ್ಯ // CV_SIMD || CV_SIMD_SCALABLE

#ಆದರೆ CV_SIMD_SCALABLE
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_float32 reduce_ಅಡ್ಡಸಾಲು_Sum2_Acc4(ಸ್ಥಿರಾಂಕ v_float32& acc,
                                           ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src0, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1,
                                           ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src3)
{
    v_float32 ಮೊತ್ತ = acc;
    v_float32 x = vx_ತುಂಬು(src0);
    ಮೊತ್ತ = v_fma(x, x, ಮೊತ್ತ);
    x = vx_ತುಂಬು(src1);
    ಮೊತ್ತ = v_fma(x, x, ಮೊತ್ತ);
    x = vx_ತುಂಬು(src2);
    ಮೊತ್ತ = v_fma(x, x, ಮೊತ್ತ);
    x = vx_ತುಂಬು(src3);
    ಹಿಂತಿರುಗಿ v_fma(x, x, ಮೊತ್ತ);
}

// dim=0, REDUCE_SUM2 for CV_32F using scalable universal intrinsics.
ಸ್ಥಿರ ಶೂನ್ಯ reduce_ಅಡ್ಡಸಾಲು_Sum2_32f32f(ಸ್ಥಿರಾಂಕ Mat& srcmat, Mat& dstmat)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಗಲ = srcmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * srcmat.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎತ್ತರ = srcmat.ಅಡ್ಡಸಾಲುಗಳು;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<v_float32>::vlanes();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಖಂಡ_2 = vlanes * 4;
    Auto_ಬಫರ್<ದಶಮಾಂಶ> ಬಫರ್(ಅಗಲ);
    ದಶಮಾಂಶ* buf = ಬಫರ್.ದತ್ತಾಂಶ();

    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src = srcmat.ptr<ದಶಮಾಂಶ>(0);
    ಪೂರ್ಣಾಂಕ i = 0;
    ಗಾಗಿ (; i <= ಅಗಲ - ಖಂಡ_2; i += ಖಂಡ_2)
    {
        v_float32 x0 = vx_ತುಂಬು(src + i);
        v_float32 x1 = vx_ತುಂಬು(src + i + vlanes);
        v_float32 x2 = vx_ತುಂಬು(src + i + vlanes * 2);
        v_float32 x3 = vx_ತುಂಬು(src + i + vlanes * 3);
        v_store(buf + i, v_mul(x0, x0));
        v_store(buf + i + vlanes, v_mul(x1, x1));
        v_store(buf + i + vlanes * 2, v_mul(x2, x2));
        v_store(buf + i + vlanes * 3, v_mul(x3, x3));
    }
    ಗಾಗಿ (; i <= ಅಗಲ - vlanes; i += vlanes)
    {
        v_float32 x = vx_ತುಂಬು(src + i);
        v_store(buf + i, v_mul(x, x));
    }
    ಗಾಗಿ (; i < ಅಗಲ; i++)
        buf[i] = src[i] * src[i];

    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = 1;
    // Four rows per pass amortize buffer traffic while preserving source-row
    // accumulation order. The outer loop stays serial: splitting width into
    // tiny parallel stripes under-fills scalable vectors and adds scheduling
    // overhead for this memory-streaming kernel.
    ಗಾಗಿ (; ಅಡ್ಡಸಾಲು <= ಎತ್ತರ - 4; ಅಡ್ಡಸಾಲು += 4)
    {
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src0 = srcmat.ptr<ದಶಮಾಂಶ>(ಅಡ್ಡಸಾಲು);
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1 = srcmat.ptr<ದಶಮಾಂಶ>(ಅಡ್ಡಸಾಲು + 1);
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2 = srcmat.ptr<ದಶಮಾಂಶ>(ಅಡ್ಡಸಾಲು + 2);
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src3 = srcmat.ptr<ದಶಮಾಂಶ>(ಅಡ್ಡಸಾಲು + 3);

        i = 0;
        ಗಾಗಿ (; i <= ಅಗಲ - ಖಂಡ_2; i += ಖಂಡ_2)
        {
            v_float32 sum0 = vx_ತುಂಬು(buf + i);
            v_float32 sum1 = vx_ತುಂಬು(buf + i + vlanes);
            v_float32 sum2 = vx_ತುಂಬು(buf + i + vlanes * 2);
            v_float32 sum3 = vx_ತುಂಬು(buf + i + vlanes * 3);
            sum0 = reduce_ಅಡ್ಡಸಾಲು_Sum2_Acc4(sum0, src0 + i, src1 + i, src2 + i, src3 + i);
            sum1 = reduce_ಅಡ್ಡಸಾಲು_Sum2_Acc4(sum1, src0 + i + vlanes, src1 + i + vlanes,
                                     src2 + i + vlanes, src3 + i + vlanes);
            sum2 = reduce_ಅಡ್ಡಸಾಲು_Sum2_Acc4(sum2, src0 + i + vlanes * 2, src1 + i + vlanes * 2,
                                     src2 + i + vlanes * 2, src3 + i + vlanes * 2);
            sum3 = reduce_ಅಡ್ಡಸಾಲು_Sum2_Acc4(sum3, src0 + i + vlanes * 3, src1 + i + vlanes * 3,
                                     src2 + i + vlanes * 3, src3 + i + vlanes * 3);
            v_store(buf + i, sum0);
            v_store(buf + i + vlanes, sum1);
            v_store(buf + i + vlanes * 2, sum2);
            v_store(buf + i + vlanes * 3, sum3);
        }
        ಗಾಗಿ (; i <= ಅಗಲ - vlanes; i += vlanes)
        {
            v_float32 ಮೊತ್ತ = vx_ತುಂಬು(buf + i);
            ಮೊತ್ತ = reduce_ಅಡ್ಡಸಾಲು_Sum2_Acc4(ಮೊತ್ತ, src0 + i, src1 + i, src2 + i, src3 + i);
            v_store(buf + i, ಮೊತ್ತ);
        }
        ಗಾಗಿ (; i < ಅಗಲ; i++)
        {
            ದಶಮಾಂಶ ಮೊತ್ತ = buf[i];
            ದಶಮಾಂಶ x = src0[i]; ಮೊತ್ತ += x * x;
            x = src1[i]; ಮೊತ್ತ += x * x;
            x = src2[i]; ಮೊತ್ತ += x * x;
            x = src3[i]; ಮೊತ್ತ += x * x;
            buf[i] = ಮೊತ್ತ;
        }
    }

    ಗಾಗಿ (; ಅಡ್ಡಸಾಲು < ಎತ್ತರ; ಅಡ್ಡಸಾಲು++)
    {
        src = srcmat.ptr<ದಶಮಾಂಶ>(ಅಡ್ಡಸಾಲು);
        i = 0;
        ಗಾಗಿ (; i <= ಅಗಲ - ಖಂಡ_2; i += ಖಂಡ_2)
        {
            v_float32 sum0 = vx_ತುಂಬು(buf + i);
            v_float32 sum1 = vx_ತುಂಬು(buf + i + vlanes);
            v_float32 sum2 = vx_ತುಂಬು(buf + i + vlanes * 2);
            v_float32 sum3 = vx_ತುಂಬು(buf + i + vlanes * 3);
            v_float32 x0 = vx_ತುಂಬು(src + i);
            v_float32 x1 = vx_ತುಂಬು(src + i + vlanes);
            v_float32 x2 = vx_ತುಂಬು(src + i + vlanes * 2);
            v_float32 x3 = vx_ತುಂಬು(src + i + vlanes * 3);
            v_store(buf + i, v_fma(x0, x0, sum0));
            v_store(buf + i + vlanes, v_fma(x1, x1, sum1));
            v_store(buf + i + vlanes * 2, v_fma(x2, x2, sum2));
            v_store(buf + i + vlanes * 3, v_fma(x3, x3, sum3));
        }
        ಗಾಗಿ (; i <= ಅಗಲ - vlanes; i += vlanes)
        {
            v_float32 x = vx_ತುಂಬು(src + i);
            v_store(buf + i, v_fma(x, x, vx_ತುಂಬು(buf + i)));
        }
        ಗಾಗಿ (; i < ಅಗಲ; i++)
            buf[i] += src[i] * src[i];
    }

    // Delay output stores until all source rows are consumed, preserving the
    // existing overlap-safe behavior of the proposed optimized path.
    ಸ್ಮರಣೆ_ನಕಲು(dstmat.ptr<ದಶಮಾಂಶ>(), buf, ಅಗಲ * ಗಾತ್ರ(ದಶಮಾಂಶ));
    v_cleanup();
}
#ಆದರೆ_ಅಂತ್ಯ

// =====================================================================
//  Dispatchers
// =====================================================================

Reduce_ಮೊತ್ತ_Func getReduceCSumFunc(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth)
{
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ (sdepth == CV_8U && ddepth == CV_32S) ಹಿಂತಿರುಗಿ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_8u32s;
    ಆದರೆ (sdepth == CV_8U && ddepth == CV_32F) ಹಿಂತಿರುಗಿ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_8u32f;
    ಆದರೆ (sdepth == CV_16U && ddepth == CV_32F) ಹಿಂತಿರುಗಿ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_16u32f;
    ಆದರೆ (sdepth == CV_16S && ddepth == CV_32F) ಹಿಂತಿರುಗಿ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_16s32f;
    ಆದರೆ (sdepth == CV_32F && ddepth == CV_32F) ಹಿಂತಿರುಗಿ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_32f32f;
#ಆದರೆ CV_SIMD_64F
    ಆದರೆ (sdepth == CV_32F && ddepth == CV_64F) ಹಿಂತಿರುಗಿ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_32f64f;
    ಆದರೆ (sdepth == CV_64F && ddepth == CV_64F) ಹಿಂತಿರುಗಿ reduce_ಲಂಬಸಾಲು_ಮೊತ್ತ_64f64f;
#ಆದರೆ_ಅಂತ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(sdepth);
    CV_ಬಳಸದ(ddepth);
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;
}

Reduce_ಮೊತ್ತ_Func getReduceRSumFunc(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth)
{
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ (sdepth == CV_8U && ddepth == CV_32S) ಹಿಂತಿರುಗಿ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_8u32s;
    ಆದರೆ (sdepth == CV_8U && ddepth == CV_32F) ಹಿಂತಿರುಗಿ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_8u32f;
    ಆದರೆ (sdepth == CV_16U && ddepth == CV_32F) ಹಿಂತಿರುಗಿ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_16u32f;
    ಆದರೆ (sdepth == CV_16S && ddepth == CV_32F) ಹಿಂತಿರುಗಿ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_16s32f;
    ಆದರೆ (sdepth == CV_32F && ddepth == CV_32F) ಹಿಂತಿರುಗಿ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_32f32f;
#ಆದರೆ CV_SIMD_64F
    ಆದರೆ (sdepth == CV_32F && ddepth == CV_64F) ಹಿಂತಿರುಗಿ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_32f64f;
    ಆದರೆ (sdepth == CV_64F && ddepth == CV_64F) ಹಿಂತಿರುಗಿ reduce_ಅಡ್ಡಸಾಲು_ಮೊತ್ತ_64f64f;
#ಆದರೆ_ಅಂತ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(sdepth);
    CV_ಬಳಸದ(ddepth);
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;
}

Reduce_ಮೊತ್ತ_Func getReduceRSum2Func(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth)
{
#ಆದರೆ CV_SIMD_SCALABLE
    ಆದರೆ (sdepth == CV_32F && ddepth == CV_32F) ಹಿಂತಿರುಗಿ reduce_ಅಡ್ಡಸಾಲು_Sum2_32f32f;
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(sdepth);
    CV_ಬಳಸದ(ddepth);
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;
}

#ಆದರೆ_ಅಂತ್ಯ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
} // namespace cv
