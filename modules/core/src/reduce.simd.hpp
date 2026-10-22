// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#અન્તર્ભાવ "precomp.hpp"

નામાકાશ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

પ્રકારવ્યાખ્યા શૂન્ય (*ReduceSumFunc)(અચલ Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth);
ReduceSumFunc getReduceRSumFunc(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth);
ReduceSumFunc getReduceRSum2Func(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth);

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// =====================================================================
//  Col reduce SUM (dim=1): sum each row into cn output values
// =====================================================================

#જો (CV_SIMD || CV_SIMD_SCALABLE)

// --- uchar → int ---
// Uses u16 intermediate accumulator to halve the number of widen operations:
// u8→u16 per iteration, u16→u32 flush every 128 iterations (max u16 value: 128*255=32640 < 65535)
સ્થિર શૂન્ય reduceColSum_8u32s(અચલ Mat& srcmat, Mat& dstmat)
{
    અચલ પૂર્ણાંક cn = srcmat.channels();
    અચલ પૂર્ણાંક cols = srcmat.cols;
    અચલ પૂર્ણાંક width = cols * cn;

    auto body = [&](અચલ Range& range) {
        માટે (પૂર્ણાંક y = range.start; y < range.end; y++)
        {
            અચલ uchar* src = srcmat.ptr<uchar>(y);
            પૂર્ણાંક* dst = dstmat.ptr<પૂર્ણાંક>(y);

            જો (cn == 1)
            {
#જો defined(CV_NEON_DOT)
                // ARMv8.2 DOTPROD: vdotq_u32(acc, src, ones) — single instruction byte sum
                // u32 lane max 1020/iter, overflow at ~4.2M iter (67MB) — safe without flush
                અચલ uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t v_sum = vdupq_n_u32(0);
                પૂર્ણાંક x = 0;
                માટે (; x <= width - 16; x += 16)
                    v_sum = vdotq_u32(v_sum, vld1q_u8(src + x), ones);
                પૂર્ણાંક total = (પૂર્ણાંક)vaddvq_u32(v_sum);
                માટે (; x < width; x++)
                    total += (પૂર્ણાંક)src[x];
                dst[0] = total;
#અથવાજો CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                // AArch64 fallback: vpaddlq chain
                uint32x4_t v_sum = vdupq_n_u32(0);
                પૂર્ણાંક x = 0;
                માટે (; x <= width - 16; x += 16)
                    v_sum = vaddq_u32(v_sum, vpaddlq_u16(vpaddlq_u8(vld1q_u8(src + x))));
                પૂર્ણાંક total = (પૂર્ણાંક)vaddvq_u32(v_sum);
                માટે (; x < width; x++)
                    total += (પૂર્ણાંક)src[x];
                dst[0] = total;
#અથવાજો CV_AVX2
                // Intel AVX2: _mm256_sad_epu8 — 32 bytes → 4×u64 sum, 1 cycle
                અચલ __m256i zero = _mm256_setzero_si256();
                __m256i vsum = zero;
                પૂર્ણાંક x = 0;
                માટે (; x <= width - 32; x += 32)
                    vsum = _mm256_add_epi64(vsum, _mm256_sad_epu8(
                        _mm256_loadu_si256((અચલ __m256i*)(src + x)), zero));
                // 8-byte tail: reduce max scalar tail from 31 to 7
                // Use _mm256_inserti128_si256 instead of _mm256_castsi128_si256:
                // the latter leaves upper 128 bits undefined per Intel spec,
                // causing MSVC to expose stale register data to _mm256_sad_epu8.
                માટે (; x <= width - 8; x += 8)
                    vsum = _mm256_add_epi64(vsum, _mm256_sad_epu8(
                        _mm256_inserti128_si256(_mm256_setzero_si256(), _mm_loadl_epi64((અચલ __m128i*)(src + x)), 0), zero));
                __m128i lo128 = _mm256_castsi256_si128(vsum);
                __m128i hi128 = _mm256_extracti128_si256(vsum, 1);
                __m128i s = _mm_add_epi64(lo128, hi128);
                s = _mm_add_epi64(s, _mm_unpackhi_epi64(s, s));
                પૂર્ણાંક total = _mm_cvtsi128_si32(s);
                માટે (; x < width; x++)
                    total += (પૂર્ણાંક)src[x];
                dst[0] = total;
#અથવાજો CV_SSE2
                // Intel SSE2: _mm_sad_epu8 — 16 bytes → 2×u64 sum
                અચલ __m128i zero = _mm_setzero_si128();
                __m128i vsum = zero;
                પૂર્ણાંક x = 0;
                માટે (; x <= width - 16; x += 16)
                    vsum = _mm_add_epi64(vsum, _mm_sad_epu8(
                        _mm_loadu_si128((અચલ __m128i*)(src + x)), zero));
                __m128i s = _mm_add_epi64(vsum, _mm_unpackhi_epi64(vsum, vsum));
                પૂર્ણાંક total = _mm_cvtsi128_si32(s);
                માટે (; x < width; x++)
                    total += (પૂર્ણાંક)src[x];
                dst[0] = total;
#નહીંતર
                અચલ પૂર્ણાંક vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 v_sum = vx_setzero_u32();
                v_uint16 v_sum16 = vx_setzero_u16();
                પૂર્ણાંક x = 0, batch = 0;
                અચલ પૂર્ણાંક flush_at = 128;

                માટે (; x <= width - vlanes8; x += vlanes8)
                {
                    v_uint16 lo, hi;
                    v_expand(vx_load(src + x), lo, hi);
                    v_sum16 = v_add(v_sum16, v_add(lo, hi));

                    જો (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(v_sum16, a, b);
                        v_sum = v_add(v_sum, v_add(a, b));
                        v_sum16 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(v_sum16, a, b);
                    v_sum = v_add(v_sum, v_add(a, b));
                }

                પૂર્ણાંક total = (પૂર્ણાંક)v_reduce_sum(v_sum);
                માટે (; x < width; x++)
                    total += (પૂર્ણાંક)src[x];
                dst[0] = total;
#endif
            }
            નહીંતર જો (cn == 4)
            {
#જો defined(CV_NEON_DOT)
                // ARMv8.2 DOTPROD: vld4q_u8 deinterleave + vdotq_u32 per-channel
                // vld4q_u8 reads 16 pixels (64 bytes), deinterleaves into 4 × u8x16
                // vdotq_u32 sums each channel's 16 bytes into 4 partial u32 sums
                અચલ uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t acc0 = vdupq_n_u32(0), acc1 = vdupq_n_u32(0);
                uint32x4_t acc2 = vdupq_n_u32(0), acc3 = vdupq_n_u32(0);
                પૂર્ણાંક x = 0;
                માટે (; x <= cols - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    acc0 = vdotq_u32(acc0, v.val[0], ones);
                    acc1 = vdotq_u32(acc1, v.val[1], ones);
                    acc2 = vdotq_u32(acc2, v.val[2], ones);
                    acc3 = vdotq_u32(acc3, v.val[3], ones);
                }
                dst[0] = (પૂર્ણાંક)vaddvq_u32(acc0);
                dst[1] = (પૂર્ણાંક)vaddvq_u32(acc1);
                dst[2] = (પૂર્ણાંક)vaddvq_u32(acc2);
                dst[3] = (પૂર્ણાંક)vaddvq_u32(acc3);
                માટે (; x < cols; x++)
                {
                    dst[0] += (પૂર્ણાંક)src[x * 4];
                    dst[1] += (પૂર્ણાંક)src[x * 4 + 1];
                    dst[2] += (પૂર્ણાંક)src[x * 4 + 2];
                    dst[3] += (પૂર્ણાંક)src[x * 4 + 3];
                }
#અથવાજો CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                // AArch64 fallback: vld4q_u8 deinterleave + vaddlvq_u8 per-channel
                પૂર્ણાંક sums[4] = {0, 0, 0, 0};
                પૂર્ણાંક x = 0;
                માટે (; x <= cols - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    sums[0] += (પૂર્ણાંક)vaddlvq_u8(v.val[0]);
                    sums[1] += (પૂર્ણાંક)vaddlvq_u8(v.val[1]);
                    sums[2] += (પૂર્ણાંક)vaddlvq_u8(v.val[2]);
                    sums[3] += (પૂર્ણાંક)vaddlvq_u8(v.val[3]);
                }
                dst[0] = sums[0]; dst[1] = sums[1];
                dst[2] = sums[2]; dst[3] = sums[3];
                માટે (; x < cols; x++)
                {
                    dst[0] += (પૂર્ણાંક)src[x * 4];
                    dst[1] += (પૂર્ણાંક)src[x * 4 + 1];
                    dst[2] += (પૂર્ણાંક)src[x * 4 + 2];
                    dst[3] += (પૂર્ણાંક)src[x * 4 + 3];
                }
#અથવાજો CV_AVX2
                // Intel AVX2: shuffle + unpack to group channels, full-register SAD
                અચલ __m256i zero = _mm256_setzero_si256();
                અચલ __m256i shuf_mask = _mm256_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                પૂર્ણાંક x = 0;
                // 16 pixels/iter with unpack (full-register SAD)
                {
                    __m256i sum_bg = zero, sum_ra = zero;
                    માટે (; x <= cols - 16; x += 16)
                    {
                        __m256i v0 = _mm256_loadu_si256((અચલ __m256i*)(src + x * 4));
                        __m256i v1 = _mm256_loadu_si256((અચલ __m256i*)(src + x * 4 + 32));
                        __m256i g0 = _mm256_shuffle_epi8(v0, shuf_mask);
                        __m256i g1 = _mm256_shuffle_epi8(v1, shuf_mask);
                        __m256i bg = _mm256_unpacklo_epi32(g0, g1);
                        __m256i ra = _mm256_unpackhi_epi32(g0, g1);
                        sum_bg = _mm256_add_epi64(sum_bg, _mm256_sad_epu8(bg, zero));
                        sum_ra = _mm256_add_epi64(sum_ra, _mm256_sad_epu8(ra, zero));
                    }
                    __m128i bg_lo = _mm256_castsi256_si128(sum_bg);
                    __m128i bg_hi = _mm256_extracti128_si256(sum_bg, 1);
                    __m128i bg_s = _mm_add_epi64(bg_lo, bg_hi);
                    __m128i ra_lo = _mm256_castsi256_si128(sum_ra);
                    __m128i ra_hi = _mm256_extracti128_si256(sum_ra, 1);
                    __m128i ra_s = _mm_add_epi64(ra_lo, ra_hi);
                    dst[0] = _mm_cvtsi128_si32(bg_s);                     // B
                    dst[1] = _mm_cvtsi128_si32(_mm_srli_si128(bg_s, 8));  // G
                    dst[2] = _mm_cvtsi128_si32(ra_s);                     // R
                    dst[3] = _mm_cvtsi128_si32(_mm_srli_si128(ra_s, 8));  // A
                }
                // 8-pixel tail with and/srli
                {
                    અચલ __m256i mask_lo32 = _mm256_set1_epi64x(0x00000000FFFFFFFFLL);
                    __m256i sum_br = zero, sum_ga = zero;
                    માટે (; x <= cols - 8; x += 8)
                    {
                        __m256i v = _mm256_loadu_si256((અચલ __m256i*)(src + x * 4));
                        __m256i grouped = _mm256_shuffle_epi8(v, shuf_mask);
                        __m256i br = _mm256_and_si256(grouped, mask_lo32);
                        __m256i ga = _mm256_srli_epi64(grouped, 32);
                        sum_br = _mm256_add_epi64(sum_br, _mm256_sad_epu8(br, zero));
                        sum_ga = _mm256_add_epi64(sum_ga, _mm256_sad_epu8(ga, zero));
                    }
                    __m128i br_lo = _mm256_castsi256_si128(sum_br);
                    __m128i br_hi = _mm256_extracti128_si256(sum_br, 1);
                    __m128i br_s = _mm_add_epi64(br_lo, br_hi);
                    __m128i ga_lo = _mm256_castsi256_si128(sum_ga);
                    __m128i ga_hi = _mm256_extracti128_si256(sum_ga, 1);
                    __m128i ga_s = _mm_add_epi64(ga_lo, ga_hi);
                    dst[0] += _mm_cvtsi128_si32(br_s);
                    dst[1] += _mm_cvtsi128_si32(ga_s);
                    dst[2] += _mm_cvtsi128_si32(_mm_srli_si128(br_s, 8));
                    dst[3] += _mm_cvtsi128_si32(_mm_srli_si128(ga_s, 8));
                }
                માટે (; x < cols; x++)
                {
                    dst[0] += (પૂર્ણાંક)src[x * 4];
                    dst[1] += (પૂર્ણાંક)src[x * 4 + 1];
                    dst[2] += (પૂર્ણાંક)src[x * 4 + 2];
                    dst[3] += (પૂર્ણાંક)src[x * 4 + 3];
                }
#અથવાજો CV_SSSE3
                // Intel SSSE3: _mm_shuffle_epi8 deinterleave + _mm_sad_epu8
                // 16 bytes = 4 pixels × 4 channels per iteration
                અચલ __m128i zero = _mm_setzero_si128();
                અચલ __m128i shuf_mask = _mm_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                અચલ __m128i mask_lo32 = _mm_set1_epi64x(0x00000000FFFFFFFFLL);
                __m128i sum_br = zero, sum_ga = zero;
                પૂર્ણાંક x = 0;
                માટે (; x <= cols - 4; x += 4)
                {
                    __m128i v = _mm_loadu_si128((અચલ __m128i*)(src + x * 4));
                    __m128i grouped = _mm_shuffle_epi8(v, shuf_mask);
                    __m128i br = _mm_and_si128(grouped, mask_lo32);
                    __m128i ga = _mm_srli_epi64(grouped, 32);
                    sum_br = _mm_add_epi64(sum_br, _mm_sad_epu8(br, zero));
                    sum_ga = _mm_add_epi64(sum_ga, _mm_sad_epu8(ga, zero));
                }
                dst[0] = _mm_cvtsi128_si32(sum_br);
                dst[1] = _mm_cvtsi128_si32(sum_ga);
                dst[2] = _mm_cvtsi128_si32(_mm_srli_si128(sum_br, 8));
                dst[3] = _mm_cvtsi128_si32(_mm_srli_si128(sum_ga, 8));
                માટે (; x < cols; x++)
                {
                    dst[0] += (પૂર્ણાંક)src[x * 4];
                    dst[1] += (પૂર્ણાંક)src[x * 4 + 1];
                    dst[2] += (પૂર્ણાંક)src[x * 4 + 2];
                    dst[3] += (પૂર્ણાંક)src[x * 4 + 3];
                }
#નહીંતર
                અચલ પૂર્ણાંક vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 vsum0 = vx_setzero_u32(), vsum1 = vx_setzero_u32();
                v_uint32 vsum2 = vx_setzero_u32(), vsum3 = vx_setzero_u32();
                v_uint16 vs16_0 = vx_setzero_u16(), vs16_1 = vx_setzero_u16();
                v_uint16 vs16_2 = vx_setzero_u16(), vs16_3 = vx_setzero_u16();
                પૂર્ણાંક x = 0, batch = 0;
                અચલ પૂર્ણાંક flush_at = 128;

                માટે (; x <= cols - vlanes8; x += vlanes8)
                {
                    v_uint8 ch0, ch1, ch2, ch3;
                    v_load_deinterleave(src + x * 4, ch0, ch1, ch2, ch3);

                    v_uint16 lo, hi;
                    v_expand(ch0, lo, hi); vs16_0 = v_add(vs16_0, v_add(lo, hi));
                    v_expand(ch1, lo, hi); vs16_1 = v_add(vs16_1, v_add(lo, hi));
                    v_expand(ch2, lo, hi); vs16_2 = v_add(vs16_2, v_add(lo, hi));
                    v_expand(ch3, lo, hi); vs16_3 = v_add(vs16_3, v_add(lo, hi));

                    જો (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(vs16_0, a, b); vsum0 = v_add(vsum0, v_add(a, b));
                        v_expand(vs16_1, a, b); vsum1 = v_add(vsum1, v_add(a, b));
                        v_expand(vs16_2, a, b); vsum2 = v_add(vsum2, v_add(a, b));
                        v_expand(vs16_3, a, b); vsum3 = v_add(vsum3, v_add(a, b));
                        vs16_0 = vx_setzero_u16(); vs16_1 = vx_setzero_u16();
                        vs16_2 = vx_setzero_u16(); vs16_3 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(vs16_0, a, b); vsum0 = v_add(vsum0, v_add(a, b));
                    v_expand(vs16_1, a, b); vsum1 = v_add(vsum1, v_add(a, b));
                    v_expand(vs16_2, a, b); vsum2 = v_add(vsum2, v_add(a, b));
                    v_expand(vs16_3, a, b); vsum3 = v_add(vsum3, v_add(a, b));
                }

                dst[0] = (પૂર્ણાંક)v_reduce_sum(vsum0);
                dst[1] = (પૂર્ણાંક)v_reduce_sum(vsum1);
                dst[2] = (પૂર્ણાંક)v_reduce_sum(vsum2);
                dst[3] = (પૂર્ણાંક)v_reduce_sum(vsum3);

                માટે (; x < cols; x++)
                {
                    dst[0] += (પૂર્ણાંક)src[x * 4];
                    dst[1] += (પૂર્ણાંક)src[x * 4 + 1];
                    dst[2] += (પૂર્ણાંક)src[x * 4 + 2];
                    dst[3] += (પૂર્ણાંક)src[x * 4 + 3];
                }
#endif
            }
            નહીંતર
            {
                // generic cn: scalar fallback
                માટે (પૂર્ણાંક c = 0; c < cn; c++)
                    dst[c] = (પૂર્ણાંક)src[c];
                માટે (પૂર્ણાંક x = cn; x < width; x += cn)
                    માટે (પૂર્ણાંક c = 0; c < cn; c++)
                        dst[c] += (પૂર્ણાંક)src[x + c];
            }
        }
    };

    parallel_for_(Range(0, srcmat.rows), body);
    v_cleanup();
}

// --- uchar → float ---
સ્થિર શૂન્ય reduceColSum_8u32f(અચલ Mat& srcmat, Mat& dstmat)
{
    અચલ પૂર્ણાંક cn = srcmat.channels();
    અચલ પૂર્ણાંક cols = srcmat.cols;
    અચલ પૂર્ણાંક width = cols * cn;

    auto body = [&](અચલ Range& range) {
        માટે (પૂર્ણાંક y = range.start; y < range.end; y++)
        {
            અચલ uchar* src = srcmat.ptr<uchar>(y);
            દશાંશ* dst = dstmat.ptr<દશાંશ>(y);

            // compute in int, then convert to float
            AutoBuffer<પૂર્ણાંક> ibuf(cn);
            પૂર્ણાંક* sums = ibuf.data();
            માટે (પૂર્ણાંક c = 0; c < cn; c++)
                sums[c] = 0;

            જો (cn == 1)
            {
#જો defined(CV_NEON_DOT)
                અચલ uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t v_sum = vdupq_n_u32(0);
                પૂર્ણાંક x = 0;
                માટે (; x <= width - 16; x += 16)
                    v_sum = vdotq_u32(v_sum, vld1q_u8(src + x), ones);
                sums[0] = (પૂર્ણાંક)vaddvq_u32(v_sum);
                માટે (; x < width; x++)
                    sums[0] += (પૂર્ણાંક)src[x];
#અથવાજો CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                uint32x4_t v_sum = vdupq_n_u32(0);
                પૂર્ણાંક x = 0;
                માટે (; x <= width - 16; x += 16)
                    v_sum = vaddq_u32(v_sum, vpaddlq_u16(vpaddlq_u8(vld1q_u8(src + x))));
                sums[0] = (પૂર્ણાંક)vaddvq_u32(v_sum);
                માટે (; x < width; x++)
                    sums[0] += (પૂર્ણાંક)src[x];
#અથવાજો CV_AVX2
                અચલ __m256i zero = _mm256_setzero_si256();
                __m256i vsum = zero;
                પૂર્ણાંક x = 0;
                માટે (; x <= width - 32; x += 32)
                    vsum = _mm256_add_epi64(vsum, _mm256_sad_epu8(
                        _mm256_loadu_si256((અચલ __m256i*)(src + x)), zero));
                // 8-byte tail: reduce max scalar tail from 31 to 7
                // Use _mm256_inserti128_si256 instead of _mm256_castsi128_si256:
                // the latter leaves upper 128 bits undefined per Intel spec,
                // causing MSVC to expose stale register data to _mm256_sad_epu8.
                માટે (; x <= width - 8; x += 8)
                    vsum = _mm256_add_epi64(vsum, _mm256_sad_epu8(
                        _mm256_inserti128_si256(_mm256_setzero_si256(), _mm_loadl_epi64((અચલ __m128i*)(src + x)), 0), zero));
                __m128i lo128 = _mm256_castsi256_si128(vsum);
                __m128i hi128 = _mm256_extracti128_si256(vsum, 1);
                __m128i s = _mm_add_epi64(lo128, hi128);
                s = _mm_add_epi64(s, _mm_unpackhi_epi64(s, s));
                sums[0] = _mm_cvtsi128_si32(s);
                માટે (; x < width; x++)
                    sums[0] += (પૂર્ણાંક)src[x];
#અથવાજો CV_SSE2
                અચલ __m128i zero = _mm_setzero_si128();
                __m128i vsum = zero;
                પૂર્ણાંક x = 0;
                માટે (; x <= width - 16; x += 16)
                    vsum = _mm_add_epi64(vsum, _mm_sad_epu8(
                        _mm_loadu_si128((અચલ __m128i*)(src + x)), zero));
                __m128i s = _mm_add_epi64(vsum, _mm_unpackhi_epi64(vsum, vsum));
                sums[0] = _mm_cvtsi128_si32(s);
                માટે (; x < width; x++)
                    sums[0] += (પૂર્ણાંક)src[x];
#નહીંતર
                અચલ પૂર્ણાંક vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 v_sum = vx_setzero_u32();
                v_uint16 v_sum16 = vx_setzero_u16();
                પૂર્ણાંક x = 0, batch = 0;

                માટે (; x <= width - vlanes8; x += vlanes8)
                {
                    v_uint16 lo, hi;
                    v_expand(vx_load(src + x), lo, hi);
                    v_sum16 = v_add(v_sum16, v_add(lo, hi));
                    જો (++batch >= 128)
                    {
                        v_uint32 a, b;
                        v_expand(v_sum16, a, b);
                        v_sum = v_add(v_sum, v_add(a, b));
                        v_sum16 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                v_uint32 a, b;
                v_expand(v_sum16, a, b);
                v_sum = v_add(v_sum, v_add(a, b));

                sums[0] = (પૂર્ણાંક)v_reduce_sum(v_sum);
                માટે (; x < width; x++)
                    sums[0] += (પૂર્ણાંક)src[x];
#endif
            }
            નહીંતર જો (cn == 4)
            {
#જો defined(CV_NEON_DOT)
                અચલ uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t acc0 = vdupq_n_u32(0), acc1 = vdupq_n_u32(0);
                uint32x4_t acc2 = vdupq_n_u32(0), acc3 = vdupq_n_u32(0);
                પૂર્ણાંક x = 0;
                માટે (; x <= cols - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    acc0 = vdotq_u32(acc0, v.val[0], ones);
                    acc1 = vdotq_u32(acc1, v.val[1], ones);
                    acc2 = vdotq_u32(acc2, v.val[2], ones);
                    acc3 = vdotq_u32(acc3, v.val[3], ones);
                }
                sums[0] = (પૂર્ણાંક)vaddvq_u32(acc0);
                sums[1] = (પૂર્ણાંક)vaddvq_u32(acc1);
                sums[2] = (પૂર્ણાંક)vaddvq_u32(acc2);
                sums[3] = (પૂર્ણાંક)vaddvq_u32(acc3);
                માટે (; x < cols; x++)
                {
                    sums[0] += (પૂર્ણાંક)src[x * 4];
                    sums[1] += (પૂર્ણાંક)src[x * 4 + 1];
                    sums[2] += (પૂર્ણાંક)src[x * 4 + 2];
                    sums[3] += (પૂર્ણાંક)src[x * 4 + 3];
                }
#અથવાજો CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                પૂર્ણાંક x = 0;
                માટે (; x <= cols - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    sums[0] += (પૂર્ણાંક)vaddlvq_u8(v.val[0]);
                    sums[1] += (પૂર્ણાંક)vaddlvq_u8(v.val[1]);
                    sums[2] += (પૂર્ણાંક)vaddlvq_u8(v.val[2]);
                    sums[3] += (પૂર્ણાંક)vaddlvq_u8(v.val[3]);
                }
                માટે (; x < cols; x++)
                {
                    sums[0] += (પૂર્ણાંક)src[x * 4];
                    sums[1] += (પૂર્ણાંક)src[x * 4 + 1];
                    sums[2] += (પૂર્ણાંક)src[x * 4 + 2];
                    sums[3] += (પૂર્ણાંક)src[x * 4 + 3];
                }
#અથવાજો CV_AVX2
                // Intel AVX2: shuffle + unpack to group channels, full-register SAD
                અચલ __m256i zero = _mm256_setzero_si256();
                અચલ __m256i shuf_mask = _mm256_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                પૂર્ણાંક x = 0;
                // 16 pixels/iter with unpack (full-register SAD)
                {
                    __m256i sum_bg = zero, sum_ra = zero;
                    માટે (; x <= cols - 16; x += 16)
                    {
                        __m256i v0 = _mm256_loadu_si256((અચલ __m256i*)(src + x * 4));
                        __m256i v1 = _mm256_loadu_si256((અચલ __m256i*)(src + x * 4 + 32));
                        __m256i g0 = _mm256_shuffle_epi8(v0, shuf_mask);
                        __m256i g1 = _mm256_shuffle_epi8(v1, shuf_mask);
                        __m256i bg = _mm256_unpacklo_epi32(g0, g1);
                        __m256i ra = _mm256_unpackhi_epi32(g0, g1);
                        sum_bg = _mm256_add_epi64(sum_bg, _mm256_sad_epu8(bg, zero));
                        sum_ra = _mm256_add_epi64(sum_ra, _mm256_sad_epu8(ra, zero));
                    }
                    __m128i bg_lo = _mm256_castsi256_si128(sum_bg);
                    __m128i bg_hi = _mm256_extracti128_si256(sum_bg, 1);
                    __m128i bg_s = _mm_add_epi64(bg_lo, bg_hi);
                    __m128i ra_lo = _mm256_castsi256_si128(sum_ra);
                    __m128i ra_hi = _mm256_extracti128_si256(sum_ra, 1);
                    __m128i ra_s = _mm_add_epi64(ra_lo, ra_hi);
                    sums[0] = _mm_cvtsi128_si32(bg_s);                     // B
                    sums[1] = _mm_cvtsi128_si32(_mm_srli_si128(bg_s, 8));  // G
                    sums[2] = _mm_cvtsi128_si32(ra_s);                     // R
                    sums[3] = _mm_cvtsi128_si32(_mm_srli_si128(ra_s, 8));  // A
                }
                // 8-pixel tail with and/srli
                {
                    અચલ __m256i mask_lo32 = _mm256_set1_epi64x(0x00000000FFFFFFFFLL);
                    __m256i sum_br = zero, sum_ga = zero;
                    માટે (; x <= cols - 8; x += 8)
                    {
                        __m256i v = _mm256_loadu_si256((અચલ __m256i*)(src + x * 4));
                        __m256i grouped = _mm256_shuffle_epi8(v, shuf_mask);
                        __m256i br = _mm256_and_si256(grouped, mask_lo32);
                        __m256i ga = _mm256_srli_epi64(grouped, 32);
                        sum_br = _mm256_add_epi64(sum_br, _mm256_sad_epu8(br, zero));
                        sum_ga = _mm256_add_epi64(sum_ga, _mm256_sad_epu8(ga, zero));
                    }
                    __m128i br_lo = _mm256_castsi256_si128(sum_br);
                    __m128i br_hi = _mm256_extracti128_si256(sum_br, 1);
                    __m128i br_s = _mm_add_epi64(br_lo, br_hi);
                    __m128i ga_lo = _mm256_castsi256_si128(sum_ga);
                    __m128i ga_hi = _mm256_extracti128_si256(sum_ga, 1);
                    __m128i ga_s = _mm_add_epi64(ga_lo, ga_hi);
                    sums[0] += _mm_cvtsi128_si32(br_s);
                    sums[1] += _mm_cvtsi128_si32(ga_s);
                    sums[2] += _mm_cvtsi128_si32(_mm_srli_si128(br_s, 8));
                    sums[3] += _mm_cvtsi128_si32(_mm_srli_si128(ga_s, 8));
                }
                માટે (; x < cols; x++)
                {
                    sums[0] += (પૂર્ણાંક)src[x * 4];
                    sums[1] += (પૂર્ણાંક)src[x * 4 + 1];
                    sums[2] += (પૂર્ણાંક)src[x * 4 + 2];
                    sums[3] += (પૂર્ણાંક)src[x * 4 + 3];
                }
#અથવાજો CV_SSSE3
                // Intel SSSE3: _mm_shuffle_epi8 deinterleave + _mm_sad_epu8
                અચલ __m128i zero = _mm_setzero_si128();
                અચલ __m128i shuf_mask = _mm_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                અચલ __m128i mask_lo32 = _mm_set1_epi64x(0x00000000FFFFFFFFLL);
                __m128i sum_br = zero, sum_ga = zero;
                પૂર્ણાંક x = 0;
                માટે (; x <= cols - 4; x += 4)
                {
                    __m128i v = _mm_loadu_si128((અચલ __m128i*)(src + x * 4));
                    __m128i grouped = _mm_shuffle_epi8(v, shuf_mask);
                    __m128i br = _mm_and_si128(grouped, mask_lo32);
                    __m128i ga = _mm_srli_epi64(grouped, 32);
                    sum_br = _mm_add_epi64(sum_br, _mm_sad_epu8(br, zero));
                    sum_ga = _mm_add_epi64(sum_ga, _mm_sad_epu8(ga, zero));
                }
                sums[0] = _mm_cvtsi128_si32(sum_br);
                sums[1] = _mm_cvtsi128_si32(sum_ga);
                sums[2] = _mm_cvtsi128_si32(_mm_srli_si128(sum_br, 8));
                sums[3] = _mm_cvtsi128_si32(_mm_srli_si128(sum_ga, 8));
                માટે (; x < cols; x++)
                {
                    sums[0] += (પૂર્ણાંક)src[x * 4];
                    sums[1] += (પૂર્ણાંક)src[x * 4 + 1];
                    sums[2] += (પૂર્ણાંક)src[x * 4 + 2];
                    sums[3] += (પૂર્ણાંક)src[x * 4 + 3];
                }
#નહીંતર
                અચલ પૂર્ણાંક vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 vsum0 = vx_setzero_u32(), vsum1 = vx_setzero_u32();
                v_uint32 vsum2 = vx_setzero_u32(), vsum3 = vx_setzero_u32();
                v_uint16 vs16_0 = vx_setzero_u16(), vs16_1 = vx_setzero_u16();
                v_uint16 vs16_2 = vx_setzero_u16(), vs16_3 = vx_setzero_u16();
                પૂર્ણાંક x = 0, batch = 0;
                અચલ પૂર્ણાંક flush_at = 128;

                માટે (; x <= cols - vlanes8; x += vlanes8)
                {
                    v_uint8 ch0, ch1, ch2, ch3;
                    v_load_deinterleave(src + x * 4, ch0, ch1, ch2, ch3);

                    v_uint16 lo, hi;
                    v_expand(ch0, lo, hi); vs16_0 = v_add(vs16_0, v_add(lo, hi));
                    v_expand(ch1, lo, hi); vs16_1 = v_add(vs16_1, v_add(lo, hi));
                    v_expand(ch2, lo, hi); vs16_2 = v_add(vs16_2, v_add(lo, hi));
                    v_expand(ch3, lo, hi); vs16_3 = v_add(vs16_3, v_add(lo, hi));

                    જો (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(vs16_0, a, b); vsum0 = v_add(vsum0, v_add(a, b));
                        v_expand(vs16_1, a, b); vsum1 = v_add(vsum1, v_add(a, b));
                        v_expand(vs16_2, a, b); vsum2 = v_add(vsum2, v_add(a, b));
                        v_expand(vs16_3, a, b); vsum3 = v_add(vsum3, v_add(a, b));
                        vs16_0 = vx_setzero_u16(); vs16_1 = vx_setzero_u16();
                        vs16_2 = vx_setzero_u16(); vs16_3 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(vs16_0, a, b); vsum0 = v_add(vsum0, v_add(a, b));
                    v_expand(vs16_1, a, b); vsum1 = v_add(vsum1, v_add(a, b));
                    v_expand(vs16_2, a, b); vsum2 = v_add(vsum2, v_add(a, b));
                    v_expand(vs16_3, a, b); vsum3 = v_add(vsum3, v_add(a, b));
                }

                sums[0] = (પૂર્ણાંક)v_reduce_sum(vsum0);
                sums[1] = (પૂર્ણાંક)v_reduce_sum(vsum1);
                sums[2] = (પૂર્ણાંક)v_reduce_sum(vsum2);
                sums[3] = (પૂર્ણાંક)v_reduce_sum(vsum3);

                માટે (; x < cols; x++)
                {
                    sums[0] += (પૂર્ણાંક)src[x * 4];
                    sums[1] += (પૂર્ણાંક)src[x * 4 + 1];
                    sums[2] += (પૂર્ણાંક)src[x * 4 + 2];
                    sums[3] += (પૂર્ણાંક)src[x * 4 + 3];
                }
#endif
            }
            નહીંતર
            {
                માટે (પૂર્ણાંક x = 0, c = 0; x < width; x++, c = (c+1)&-(c < cn-1))
                    sums[c] += (પૂર્ણાંક)src[x];
            }
            માટે (પૂર્ણાંક c = 0; c < cn; c++)
                dst[c] = (દશાંશ)sums[c];
        }
    };
    parallel_for_(Range(0, srcmat.rows), body);
    v_cleanup();
}

// --- 16-bit (ushort/short) → float ---
template<typename SrcT, typename VecSrc, typename VecDst>
સ્થિર શૂન્ય reduceColSum_16_32f(અચલ Mat& srcmat, Mat& dstmat)
{
    અચલ પૂર્ણાંક cn = srcmat.channels();
    અચલ પૂર્ણાંક width = srcmat.cols * cn;
    અચલ પૂર્ણાંક vlanes = VTraits<VecSrc>::vlanes();

    auto body = [&](અચલ Range& range) {
        માટે (પૂર્ણાંક y = range.start; y < range.end; y++)
        {
            અચલ SrcT* src = srcmat.ptr<SrcT>(y);
            દશાંશ* dst = dstmat.ptr<દશાંશ>(y);

            જો (cn == 1)
            {
                VecDst v_sum0 = v_setzero_<VecDst>(), v_sum1 = v_setzero_<VecDst>();
                પૂર્ણાંક x = 0;
                માટે (; x <= width - vlanes; x += vlanes)
                {
                    VecDst lo, hi;
                    v_expand(vx_load(src + x), lo, hi);
                    v_sum0 = v_add(v_sum0, lo);
                    v_sum1 = v_add(v_sum1, hi);
                }
                dst[0] = (દશાંશ)(v_reduce_sum(v_sum0) + v_reduce_sum(v_sum1));
                માટે (; x < width; x++)
                    dst[0] += (દશાંશ)src[x];
            }
            નહીંતર
            {
                માટે (પૂર્ણાંક c = 0; c < cn; c++)
                    dst[c] = 0;
                માટે (પૂર્ણાંક x = 0, c = 0; x < width; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += (દશાંશ)src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.rows), body);
    v_cleanup();
}

સ્થિર શૂન્ય reduceColSum_16u32f(અચલ Mat& srcmat, Mat& dstmat)
{ reduceColSum_16_32f<ushort, v_uint16, v_uint32>(srcmat, dstmat); }

સ્થિર શૂન્ય reduceColSum_16s32f(અચલ Mat& srcmat, Mat& dstmat)
{ reduceColSum_16_32f<short, v_int16, v_int32>(srcmat, dstmat); }

// --- float/double → same type ---
template<typename T, typename VecT>
સ્થિર શૂન્ય reduceColSum_FP(અચલ Mat& srcmat, Mat& dstmat)
{
    અચલ પૂર્ણાંક cn = srcmat.channels();
    અચલ પૂર્ણાંક width = srcmat.cols * cn;
    અચલ પૂર્ણાંક vlanes = VTraits<VecT>::vlanes();

    auto body = [&](અચલ Range& range) {
        માટે (પૂર્ણાંક y = range.start; y < range.end; y++)
        {
            અચલ T* src = srcmat.ptr<T>(y);
            T* dst = dstmat.ptr<T>(y);

            જો (cn == 1)
            {
                VecT s0 = v_setzero_<VecT>(), s1 = v_setzero_<VecT>();
                VecT s2 = v_setzero_<VecT>(), s3 = v_setzero_<VecT>();
                પૂર્ણાંક x = 0;
                માટે (; x <= width - vlanes * 4; x += vlanes * 4)
                {
                    s0 = v_add(s0, vx_load(src + x));
                    s1 = v_add(s1, vx_load(src + x + vlanes));
                    s2 = v_add(s2, vx_load(src + x + vlanes * 2));
                    s3 = v_add(s3, vx_load(src + x + vlanes * 3));
                }
                s0 = v_add(v_add(s0, s1), v_add(s2, s3));
                માટે (; x <= width - vlanes; x += vlanes)
                    s0 = v_add(s0, vx_load(src + x));
                T total = (T)v_reduce_sum(s0);
                માટે (; x < width; x++)
                    total += src[x];
                dst[0] = total;
            }
            નહીંતર
            {
                માટે (પૂર્ણાંક c = 0; c < cn; c++)
                    dst[c] = 0;
                માટે (પૂર્ણાંક x = 0, c = 0; x < width; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.rows), body);
    v_cleanup();
}

સ્થિર શૂન્ય reduceColSum_32f32f(અચલ Mat& srcmat, Mat& dstmat)
{ reduceColSum_FP<દશાંશ, v_float32>(srcmat, dstmat); }

#જો CV_SIMD_64F
// --- float → double ---
સ્થિર શૂન્ય reduceColSum_32f64f(અચલ Mat& srcmat, Mat& dstmat)
{
    અચલ પૂર્ણાંક cn = srcmat.channels();
    અચલ પૂર્ણાંક width = srcmat.cols * cn;
    અચલ પૂર્ણાંક vlanes32 = VTraits<v_float32>::vlanes();

    auto body = [&](અચલ Range& range) {
        માટે (પૂર્ણાંક y = range.start; y < range.end; y++)
        {
            અચલ દશાંશ* src = srcmat.ptr<દશાંશ>(y);
            દ્વિગુણ* dst = dstmat.ptr<દ્વિગુણ>(y);

            જો (cn == 1)
            {
                v_float64 v_sum0 = vx_setzero_f64();
                v_float64 v_sum1 = vx_setzero_f64();
                પૂર્ણાંક x = 0;
                માટે (; x <= width - vlanes32; x += vlanes32)
                {
                    v_float32 v = vx_load(src + x);
                    v_sum0 = v_add(v_sum0, v_cvt_f64(v));
                    v_sum1 = v_add(v_sum1, v_cvt_f64_ઉપલું(v));
                }
                v_float64 v_total = v_add(v_sum0, v_sum1);

                દ્વિગુણ total = v_reduce_sum(v_total);
                માટે (; x < width; x++)
                    total += (દ્વિગુણ)src[x];
                dst[0] = total;
            }
            નહીંતર
            {
                માટે (પૂર્ણાંક c = 0; c < cn; c++)
                    dst[c] = 0;
                માટે (પૂર્ણાંક x = 0, c = 0; x < width; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += (દ્વિગુણ)src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.rows), body);
    v_cleanup();
}

સ્થિર શૂન્ય reduceColSum_64f64f(અચલ Mat& srcmat, Mat& dstmat)
{ reduceColSum_FP<દ્વિગુણ, v_float64>(srcmat, dstmat); }
#endif // CV_SIMD_64F

// =====================================================================
//  Row reduce SUM (dim=0): sum each column across all rows
// =====================================================================

// --- uchar → int ---
// Uses u16 intermediate accumulator for vertical sum: accumulate u8→u16 per row,
// flush u16→u32 every 256 rows (max u16 value: 256*255=65280 < 65535)
સ્થિર શૂન્ય reduceRowSum_8u32s(અચલ Mat& srcmat, Mat& dstmat)
{
    અચલ પૂર્ણાંક width_cn = srcmat.cols * srcmat.channels();
    અચલ પૂર્ણાંક height = srcmat.rows;
    અચલ પૂર્ણાંક vlanes16 = VTraits<v_uint16>::vlanes();
    અચલ પૂર્ણાંક vlanes32 = VTraits<v_int32>::vlanes();

    auto body = [&](અચલ Range& range) {
        અચલ પૂર્ણાંક start = range.start;
        અચલ પૂર્ણાંક end = range.end;
        અચલ પૂર્ણાંક len = end - start;

        AutoBuffer<પૂર્ણાંક> buf32_storage(len);
        AutoBuffer<ushort> buf16_storage(len);
        પૂર્ણાંક* buf32 = buf32_storage.data();
        ushort* buf16 = buf16_storage.data();

        // init from first row
        અચલ uchar* src0 = srcmat.ptr<uchar>(0) + start;
        માટે (પૂર્ણાંક i = 0; i < len; i++)
            buf32[i] = (પૂર્ણાંક)src0[i];

        જો (height <= 1)
        {
            પૂર્ણાંક* dst = dstmat.ptr<પૂર્ણાંક>(0) + start;
            માટે (પૂર્ણાંક i = 0; i < len; i++)
                dst[i] = buf32[i];
            પાછા;
        }

        memset(buf16, 0, len * કદ(ushort));
        અચલ પૂર્ણાંક flush_at = 256; // 256*255 = 65280 < 65535 (u16 safe)
        પૂર્ણાંક flush_ગણતરી = 0;

        માટે (પૂર્ણાંક row = 1; row < height; row++)
        {
            અચલ uchar* src = srcmat.ptr<uchar>(row) + start;
            પૂર્ણાંક i = 0;

#જો CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
            // AArch64: vaddw_u8 — single-instruction widening add (u16 += u8)
            માટે (; i <= len - 16; i += 16)
            {
                uint8x16_t v = vld1q_u8(src + i);
                uint16x8_t acc_lo = vld1q_u16(buf16 + i);
                uint16x8_t acc_hi = vld1q_u16(buf16 + i + 8);
                vst1q_u16(buf16 + i,     vaddw_u8(acc_lo, vget_નીચલું_u8(v)));
                vst1q_u16(buf16 + i + 8, vaddw_ઉપલું_u8(acc_hi, v));
            }
#અથવાજો CV_RVV
            અચલ પૂર્ણાંક vl = (પૂર્ણાંક)__riscv_vsetvlmax_e8m2();
            માટે (; i <= len - vl; i += vl)
            {
                vuint8m2_t vs = __riscv_vle8_v_u8m2(src + i, vl);
                vuint16m4_t va = __riscv_vle16_v_u16m4(buf16 + i, vl);
                va = __riscv_vwaddu_wv_u16m4(va, vs, vl);
                __riscv_vse16_v_u16m4(buf16 + i, va, vl);
            }
#નહીંતર
            અચલ પૂર્ણાંક vlanes8 = VTraits<v_uint8>::vlanes();
            માટે (; i <= len - vlanes8; i += vlanes8)
            {
                v_uint16 lo, hi;
                v_expand(vx_load(src + i), lo, hi);
                v_store(buf16 + i, v_add(vx_load(buf16 + i), lo));
                v_store(buf16 + i + vlanes16, v_add(vx_load(buf16 + i + vlanes16), hi));
            }
#endif
            માટે (; i < len; i++)
                buf16[i] += (ushort)src[i];

            જો (++flush_ગણતરી >= flush_at || row == height - 1)
            {
                // flush u16 → u32
                પૂર્ણાંક j = 0;
                માટે (; j <= len - vlanes16; j += vlanes16)
                {
                    v_uint32 lo, hi;
                    v_expand(vx_load(buf16 + j), lo, hi);
                    v_store(buf32 + j, v_add(vx_load(buf32 + j), v_reinterpret_as_s32(lo)));
                    v_store(buf32 + j + vlanes32, v_add(vx_load(buf32 + j + vlanes32), v_reinterpret_as_s32(hi)));
                }
                માટે (; j < len; j++)
                    buf32[j] += (પૂર્ણાંક)buf16[j];
                memset(buf16, 0, len * કદ(ushort));
                flush_ગણતરી = 0;
            }
        }

        પૂર્ણાંક* dst = dstmat.ptr<પૂર્ણાંક>(0) + start;
        માટે (પૂર્ણાંક i = 0; i < len; i++)
            dst[i] = buf32[i];
    };

    parallel_for_(Range(0, width_cn), body, width_cn * CV_ELEM_SIZE(srcmat.depth()) / 64);
    v_cleanup();
}

// --- uchar → float ---
સ્થિર શૂન્ય reduceRowSum_8u32f(અચલ Mat& srcmat, Mat& dstmat)
{
    // compute in int, then convert to float
    Mat temp(dstmat.rows, dstmat.cols, CV_32SC(srcmat.channels()));
    reduceRowSum_8u32s(srcmat, temp);
    temp.convertTo(dstmat, dstmat.type());
}

// --- 16-bit (ushort/short) → float row reduce ---
template<typename SrcT, typename VecSrc, typename VecDst>
સ્થિર શૂન્ય reduceRowSum_16_32f(અચલ Mat& srcmat, Mat& dstmat)
{
    અચલ પૂર્ણાંક width_cn = srcmat.cols * srcmat.channels();
    અચલ પૂર્ણાંક height = srcmat.rows;
    અચલ પૂર્ણાંક vlanes16 = VTraits<VecSrc>::vlanes();
    અચલ પૂર્ણાંક vlanes32 = vlanes16 / 2;

    auto body = [&](અચલ Range& range) {
        અચલ પૂર્ણાંક start = range.start;
        અચલ પૂર્ણાંક end = range.end;
        અચલ પૂર્ણાંક len = end - start;
        દશાંશ* dst = dstmat.ptr<દશાંશ>(0) + start;

        પૂર્ણાંક i = 0;
        માટે (; i <= len - vlanes16 * 4; i += vlanes16 * 4)
        {
            અચલ SrcT* src0 = srcmat.ptr<SrcT>(0) + start + i;
            VecDst a0, a1, a2, a3, a4, a5, a6, a7;
            v_expand(vx_load(src0), a0, a1);
            v_expand(vx_load(src0 + vlanes16), a2, a3);
            v_expand(vx_load(src0 + vlanes16*2), a4, a5);
            v_expand(vx_load(src0 + vlanes16*3), a6, a7);
            માટે (પૂર્ણાંક row = 1; row < height; row++)
            {
                અચલ SrcT* src = srcmat.ptr<SrcT>(row) + start + i;
                VecDst lo, hi;
                v_expand(vx_load(src), lo, hi);
                a0 = v_add(a0, lo); a1 = v_add(a1, hi);
                v_expand(vx_load(src + vlanes16), lo, hi);
                a2 = v_add(a2, lo); a3 = v_add(a3, hi);
                v_expand(vx_load(src + vlanes16*2), lo, hi);
                a4 = v_add(a4, lo); a5 = v_add(a5, hi);
                v_expand(vx_load(src + vlanes16*3), lo, hi);
                a6 = v_add(a6, lo); a7 = v_add(a7, hi);
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
        માટે (; i <= len - vlanes16; i += vlanes16)
        {
            VecDst a0, a1;
            v_expand(vx_load(srcmat.ptr<SrcT>(0) + start + i), a0, a1);
            માટે (પૂર્ણાંક row = 1; row < height; row++)
            {
                VecDst lo, hi;
                v_expand(vx_load(srcmat.ptr<SrcT>(row) + start + i), lo, hi);
                a0 = v_add(a0, lo); a1 = v_add(a1, hi);
            }
            v_store(dst + i, v_cvt_f32(v_reinterpret_as_s32(a0)));
            v_store(dst + i + vlanes32, v_cvt_f32(v_reinterpret_as_s32(a1)));
        }
        માટે (; i < len; i++)
        {
            પૂર્ણાંક val = (પૂર્ણાંક)*(srcmat.ptr<SrcT>(0) + start + i);
            માટે (પૂર્ણાંક row = 1; row < height; row++)
                val += (પૂર્ણાંક)*(srcmat.ptr<SrcT>(row) + start + i);
            dst[i] = (દશાંશ)val;
        }
    };

    parallel_for_(Range(0, width_cn), body, width_cn * CV_ELEM_SIZE(srcmat.depth()) / 64);
    v_cleanup();
}

સ્થિર શૂન્ય reduceRowSum_16u32f(અચલ Mat& srcmat, Mat& dstmat)
{ reduceRowSum_16_32f<ushort, v_uint16, v_uint32>(srcmat, dstmat); }

સ્થિર શૂન્ય reduceRowSum_16s32f(અચલ Mat& srcmat, Mat& dstmat)
{ reduceRowSum_16_32f<short, v_int16, v_int32>(srcmat, dstmat); }

// --- float/double → same type row reduce ---
template<typename T, typename VecT>
સ્થિર શૂન્ય reduceRowSum_FP(અચલ Mat& srcmat, Mat& dstmat)
{
    અચલ પૂર્ણાંક width_cn = srcmat.cols * srcmat.channels();
    અચલ પૂર્ણાંક height = srcmat.rows;
    અચલ પૂર્ણાંક vlanes = VTraits<VecT>::vlanes();

    auto body = [&](અચલ Range& range) {
        અચલ પૂર્ણાંક start = range.start;
        અચલ પૂર્ણાંક end = range.end;
        અચલ પૂર્ણાંક len = end - start;
        T* dst = dstmat.ptr<T>(0) + start;

        પૂર્ણાંક i = 0;
        માટે (; i <= len - vlanes * 8; i += vlanes * 8)
        {
            અચલ T* src0 = srcmat.ptr<T>(0) + start + i;
            VecT s0 = vx_load(src0), s1 = vx_load(src0 + vlanes);
            VecT s2 = vx_load(src0 + vlanes*2), s3 = vx_load(src0 + vlanes*3);
            VecT s4 = vx_load(src0 + vlanes*4), s5 = vx_load(src0 + vlanes*5);
            VecT s6 = vx_load(src0 + vlanes*6), s7 = vx_load(src0 + vlanes*7);
            માટે (પૂર્ણાંક row = 1; row < height; row++)
            {
                અચલ T* src = srcmat.ptr<T>(row) + start + i;
                s0 = v_add(s0, vx_load(src));
                s1 = v_add(s1, vx_load(src + vlanes));
                s2 = v_add(s2, vx_load(src + vlanes*2));
                s3 = v_add(s3, vx_load(src + vlanes*3));
                s4 = v_add(s4, vx_load(src + vlanes*4));
                s5 = v_add(s5, vx_load(src + vlanes*5));
                s6 = v_add(s6, vx_load(src + vlanes*6));
                s7 = v_add(s7, vx_load(src + vlanes*7));
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
        માટે (; i <= len - vlanes; i += vlanes)
        {
            VecT s0 = vx_load(srcmat.ptr<T>(0) + start + i);
            માટે (પૂર્ણાંક row = 1; row < height; row++)
                s0 = v_add(s0, vx_load(srcmat.ptr<T>(row) + start + i));
            v_store(dst + i, s0);
        }
        માટે (; i < len; i++)
        {
            T val = *(srcmat.ptr<T>(0) + start + i);
            માટે (પૂર્ણાંક row = 1; row < height; row++)
                val += *(srcmat.ptr<T>(row) + start + i);
            dst[i] = val;
        }
    };

    parallel_for_(Range(0, width_cn), body, width_cn * CV_ELEM_SIZE(srcmat.depth()) / 64);
    v_cleanup();
}

સ્થિર શૂન્ય reduceRowSum_32f32f(અચલ Mat& srcmat, Mat& dstmat)
{ reduceRowSum_FP<દશાંશ, v_float32>(srcmat, dstmat); }

#જો CV_SIMD_64F
// --- float → double ---
સ્થિર શૂન્ય reduceRowSum_32f64f(અચલ Mat& srcmat, Mat& dstmat)
{
    અચલ પૂર્ણાંક width_cn = srcmat.cols * srcmat.channels();
    અચલ પૂર્ણાંક height = srcmat.rows;
    અચલ પૂર્ણાંક vlanes32 = VTraits<v_float32>::vlanes();
    અચલ પૂર્ણાંક vlanes64 = VTraits<v_float64>::vlanes();

    auto body = [&](અચલ Range& range) {
        અચલ પૂર્ણાંક start = range.start;
        અચલ પૂર્ણાંક end = range.end;
        અચલ પૂર્ણાંક len = end - start;

        AutoBuffer<દ્વિગુણ> buf_storage(len);
        દ્વિગુણ* buf = buf_storage.data();

        અચલ દશાંશ* src0 = srcmat.ptr<દશાંશ>(0) + start;
        માટે (પૂર્ણાંક i = 0; i < len; i++)
            buf[i] = (દ્વિગુણ)src0[i];

        માટે (પૂર્ણાંક row = 1; row < height; row++)
        {
            અચલ દશાંશ* src = srcmat.ptr<દશાંશ>(row) + start;
            પૂર્ણાંક i = 0;
            માટે (; i <= len - vlanes32; i += vlanes32)
            {
                v_float32 v = vx_load(src + i);
                v_store(buf + i, v_add(vx_load(buf + i), v_cvt_f64(v)));
                v_store(buf + i + vlanes64, v_add(vx_load(buf + i + vlanes64), v_cvt_f64_ઉપલું(v)));
            }
            માટે (; i < len; i++)
                buf[i] += (દ્વિગુણ)src[i];
        }

        દ્વિગુણ* dst = dstmat.ptr<દ્વિગુણ>(0) + start;
        memcpy(dst, buf, len * કદ(દ્વિગુણ));
    };

    parallel_for_(Range(0, width_cn), body, width_cn * CV_ELEM_SIZE(srcmat.depth()) / 64);
    v_cleanup();
}

સ્થિર શૂન્ય reduceRowSum_64f64f(અચલ Mat& srcmat, Mat& dstmat)
{ reduceRowSum_FP<દ્વિગુણ, v_float64>(srcmat, dstmat); }
#endif // CV_SIMD_64F

#endif // CV_SIMD || CV_SIMD_SCALABLE

#જો CV_SIMD_SCALABLE
સ્થિર અંતઃરેખ v_float32 reduceRowSum2Acc4(અચલ v_float32& acc,
                                           અચલ દશાંશ* src0, અચલ દશાંશ* src1,
                                           અચલ દશાંશ* src2, અચલ દશાંશ* src3)
{
    v_float32 sum = acc;
    v_float32 x = vx_load(src0);
    sum = v_fma(x, x, sum);
    x = vx_load(src1);
    sum = v_fma(x, x, sum);
    x = vx_load(src2);
    sum = v_fma(x, x, sum);
    x = vx_load(src3);
    પાછા v_fma(x, x, sum);
}

// dim=0, REDUCE_SUM2 for CV_32F using scalable universal intrinsics.
સ્થિર શૂન્ય reduceRowSum2_32f32f(અચલ Mat& srcmat, Mat& dstmat)
{
    અચલ પૂર્ણાંક width = srcmat.cols * srcmat.channels();
    અચલ પૂર્ણાંક height = srcmat.rows;
    અચલ પૂર્ણાંક vlanes = VTraits<v_float32>::vlanes();
    અચલ પૂર્ણાંક block = vlanes * 4;
    AutoBuffer<દશાંશ> buffer(width);
    દશાંશ* buf = buffer.data();

    અચલ દશાંશ* src = srcmat.ptr<દશાંશ>(0);
    પૂર્ણાંક i = 0;
    માટે (; i <= width - block; i += block)
    {
        v_float32 x0 = vx_load(src + i);
        v_float32 x1 = vx_load(src + i + vlanes);
        v_float32 x2 = vx_load(src + i + vlanes * 2);
        v_float32 x3 = vx_load(src + i + vlanes * 3);
        v_store(buf + i, v_mul(x0, x0));
        v_store(buf + i + vlanes, v_mul(x1, x1));
        v_store(buf + i + vlanes * 2, v_mul(x2, x2));
        v_store(buf + i + vlanes * 3, v_mul(x3, x3));
    }
    માટે (; i <= width - vlanes; i += vlanes)
    {
        v_float32 x = vx_load(src + i);
        v_store(buf + i, v_mul(x, x));
    }
    માટે (; i < width; i++)
        buf[i] = src[i] * src[i];

    પૂર્ણાંક row = 1;
    // Four rows per pass amortize buffer traffic while preserving source-row
    // accumulation order. The outer loop stays serial: splitting width into
    // tiny parallel stripes under-fills scalable vectors and adds scheduling
    // overhead for this memory-streaming kernel.
    માટે (; row <= height - 4; row += 4)
    {
        અચલ દશાંશ* src0 = srcmat.ptr<દશાંશ>(row);
        અચલ દશાંશ* src1 = srcmat.ptr<દશાંશ>(row + 1);
        અચલ દશાંશ* src2 = srcmat.ptr<દશાંશ>(row + 2);
        અચલ દશાંશ* src3 = srcmat.ptr<દશાંશ>(row + 3);

        i = 0;
        માટે (; i <= width - block; i += block)
        {
            v_float32 sum0 = vx_load(buf + i);
            v_float32 sum1 = vx_load(buf + i + vlanes);
            v_float32 sum2 = vx_load(buf + i + vlanes * 2);
            v_float32 sum3 = vx_load(buf + i + vlanes * 3);
            sum0 = reduceRowSum2Acc4(sum0, src0 + i, src1 + i, src2 + i, src3 + i);
            sum1 = reduceRowSum2Acc4(sum1, src0 + i + vlanes, src1 + i + vlanes,
                                     src2 + i + vlanes, src3 + i + vlanes);
            sum2 = reduceRowSum2Acc4(sum2, src0 + i + vlanes * 2, src1 + i + vlanes * 2,
                                     src2 + i + vlanes * 2, src3 + i + vlanes * 2);
            sum3 = reduceRowSum2Acc4(sum3, src0 + i + vlanes * 3, src1 + i + vlanes * 3,
                                     src2 + i + vlanes * 3, src3 + i + vlanes * 3);
            v_store(buf + i, sum0);
            v_store(buf + i + vlanes, sum1);
            v_store(buf + i + vlanes * 2, sum2);
            v_store(buf + i + vlanes * 3, sum3);
        }
        માટે (; i <= width - vlanes; i += vlanes)
        {
            v_float32 sum = vx_load(buf + i);
            sum = reduceRowSum2Acc4(sum, src0 + i, src1 + i, src2 + i, src3 + i);
            v_store(buf + i, sum);
        }
        માટે (; i < width; i++)
        {
            દશાંશ sum = buf[i];
            દશાંશ x = src0[i]; sum += x * x;
            x = src1[i]; sum += x * x;
            x = src2[i]; sum += x * x;
            x = src3[i]; sum += x * x;
            buf[i] = sum;
        }
    }

    માટે (; row < height; row++)
    {
        src = srcmat.ptr<દશાંશ>(row);
        i = 0;
        માટે (; i <= width - block; i += block)
        {
            v_float32 sum0 = vx_load(buf + i);
            v_float32 sum1 = vx_load(buf + i + vlanes);
            v_float32 sum2 = vx_load(buf + i + vlanes * 2);
            v_float32 sum3 = vx_load(buf + i + vlanes * 3);
            v_float32 x0 = vx_load(src + i);
            v_float32 x1 = vx_load(src + i + vlanes);
            v_float32 x2 = vx_load(src + i + vlanes * 2);
            v_float32 x3 = vx_load(src + i + vlanes * 3);
            v_store(buf + i, v_fma(x0, x0, sum0));
            v_store(buf + i + vlanes, v_fma(x1, x1, sum1));
            v_store(buf + i + vlanes * 2, v_fma(x2, x2, sum2));
            v_store(buf + i + vlanes * 3, v_fma(x3, x3, sum3));
        }
        માટે (; i <= width - vlanes; i += vlanes)
        {
            v_float32 x = vx_load(src + i);
            v_store(buf + i, v_fma(x, x, vx_load(buf + i)));
        }
        માટે (; i < width; i++)
            buf[i] += src[i] * src[i];
    }

    // Delay output stores until all source rows are consumed, preserving the
    // existing overlap-safe behavior of the proposed optimized path.
    memcpy(dstmat.ptr<દશાંશ>(), buf, width * કદ(દશાંશ));
    v_cleanup();
}
#endif

// =====================================================================
//  Dispatchers
// =====================================================================

ReduceSumFunc getReduceCSumFunc(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth)
{
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો (sdepth == CV_8U && ddepth == CV_32S) પાછા reduceColSum_8u32s;
    જો (sdepth == CV_8U && ddepth == CV_32F) પાછા reduceColSum_8u32f;
    જો (sdepth == CV_16U && ddepth == CV_32F) પાછા reduceColSum_16u32f;
    જો (sdepth == CV_16S && ddepth == CV_32F) પાછા reduceColSum_16s32f;
    જો (sdepth == CV_32F && ddepth == CV_32F) પાછા reduceColSum_32f32f;
#જો CV_SIMD_64F
    જો (sdepth == CV_32F && ddepth == CV_64F) પાછા reduceColSum_32f64f;
    જો (sdepth == CV_64F && ddepth == CV_64F) પાછા reduceColSum_64f64f;
#endif
#નહીંતર
    CV_UNUSED(sdepth);
    CV_UNUSED(ddepth);
#endif
    પાછા nullptr;
}

ReduceSumFunc getReduceRSumFunc(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth)
{
#જો (CV_SIMD || CV_SIMD_SCALABLE)
    જો (sdepth == CV_8U && ddepth == CV_32S) પાછા reduceRowSum_8u32s;
    જો (sdepth == CV_8U && ddepth == CV_32F) પાછા reduceRowSum_8u32f;
    જો (sdepth == CV_16U && ddepth == CV_32F) પાછા reduceRowSum_16u32f;
    જો (sdepth == CV_16S && ddepth == CV_32F) પાછા reduceRowSum_16s32f;
    જો (sdepth == CV_32F && ddepth == CV_32F) પાછા reduceRowSum_32f32f;
#જો CV_SIMD_64F
    જો (sdepth == CV_32F && ddepth == CV_64F) પાછા reduceRowSum_32f64f;
    જો (sdepth == CV_64F && ddepth == CV_64F) પાછા reduceRowSum_64f64f;
#endif
#નહીંતર
    CV_UNUSED(sdepth);
    CV_UNUSED(ddepth);
#endif
    પાછા nullptr;
}

ReduceSumFunc getReduceRSum2Func(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth)
{
#જો CV_SIMD_SCALABLE
    જો (sdepth == CV_32F && ddepth == CV_32F) પાછા reduceRowSum2_32f32f;
#નહીંતર
    CV_UNUSED(sdepth);
    CV_UNUSED(ddepth);
#endif
    પાછા nullptr;
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
} // namespace cv
