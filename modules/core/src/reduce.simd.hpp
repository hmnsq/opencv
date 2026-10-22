// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"

నామాకాశ cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

రకనిర్వచనం శూన్య (*Reduce_కూడిక_Func)(స్థిరరాశి Mat& src, Mat& dst);
Reduce_కూడిక_Func getReduceCSumFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth);
Reduce_కూడిక_Func getReduceRSumFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth);
Reduce_కూడిక_Func getReduceRSum2Func(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth);

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// =====================================================================
//  Col reduce SUM (dim=1): sum each row into cn output values
// =====================================================================

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)

// --- uchar → int ---
// Uses u16 intermediate accumulator to halve the number of widen operations:
// u8→u16 per iteration, u16→u32 flush every 128 iterations (max u16 value: 128*255=32640 < 65535)
స్థిర శూన్య reduce_నిలువు_కూడిక_8u32s(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{
    స్థిరరాశి పూర్ణాంకం cn = srcmat.channels();
    స్థిరరాశి పూర్ణాంకం నిలువుల_సంఖ్య = srcmat.నిలువుల_సంఖ్య;
    స్థిరరాశి పూర్ణాంకం వెడల్పు = నిలువుల_సంఖ్య * cn;

    స్వయంచాలక body = [&](స్థిరరాశి Range& range) {
        కొరకు (పూర్ణాంకం y = range.ప్రారంభం; y < range.end; y++)
        {
            స్థిరరాశి uchar* src = srcmat.ptr<uchar>(y);
            పూర్ణాంకం* dst = dstmat.ptr<పూర్ణాంకం>(y);

            ఒకవేళ (cn == 1)
            {
#ఒకవేళ defined(CV_NEON_DOT)
                // ARMv8.2 DOTPROD: vdotq_u32(acc, src, ones) — single instruction byte sum
                // u32 lane max 1020/iter, overflow at ~4.2M iter (67MB) — safe without flush
                స్థిరరాశి uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t v_కూడిక = vdupq_n_u32(0);
                పూర్ణాంకం x = 0;
                కొరకు (; x <= వెడల్పు - 16; x += 16)
                    v_కూడిక = vdotq_u32(v_కూడిక, vld1q_u8(src + x), ones);
                పూర్ణాంకం మొత్తం = (పూర్ణాంకం)vaddvq_u32(v_కూడిక);
                కొరకు (; x < వెడల్పు; x++)
                    మొత్తం += (పూర్ణాంకం)src[x];
                dst[0] = మొత్తం;
#లేదావేళ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                // AArch64 fallback: vpaddlq chain
                uint32x4_t v_కూడిక = vdupq_n_u32(0);
                పూర్ణాంకం x = 0;
                కొరకు (; x <= వెడల్పు - 16; x += 16)
                    v_కూడిక = vaddq_u32(v_కూడిక, vpaddlq_u16(vpaddlq_u8(vld1q_u8(src + x))));
                పూర్ణాంకం మొత్తం = (పూర్ణాంకం)vaddvq_u32(v_కూడిక);
                కొరకు (; x < వెడల్పు; x++)
                    మొత్తం += (పూర్ణాంకం)src[x];
                dst[0] = మొత్తం;
#లేదావేళ CV_AVX2
                // Intel AVX2: _mm256_sad_epu8 — 32 bytes → 4×u64 sum, 1 cycle
                స్థిరరాశి __m256i zero = _mm256_setzero_si256();
                __m256i vsum = zero;
                పూర్ణాంకం x = 0;
                కొరకు (; x <= వెడల్పు - 32; x += 32)
                    vsum = mm256_కూడు_epi64(vsum, _mm256_sad_epu8(
                        _mm256_loadu_si256((స్థిరరాశి __m256i*)(src + x)), zero));
                // 8-byte tail: reduce max scalar tail from 31 to 7
                // Use _mm256_inserti128_si256 instead of _mm256_castsi128_si256:
                // the latter leaves upper 128 bits undefined per Intel spec,
                // causing MSVC to expose stale register data to _mm256_sad_epu8.
                కొరకు (; x <= వెడల్పు - 8; x += 8)
                    vsum = mm256_కూడు_epi64(vsum, _mm256_sad_epu8(
                        _mm256_inserti128_si256(_mm256_setzero_si256(), _mm_loadl_epi64((స్థిరరాశి __m128i*)(src + x)), 0), zero));
                __m128i lo128 = _mm256_castsi256_si128(vsum);
                __m128i hi128 = _mm256_extracti128_si256(vsum, 1);
                __m128i s = mm_కూడు_epi64(lo128, hi128);
                s = mm_కూడు_epi64(s, _mm_unpackhi_epi64(s, s));
                పూర్ణాంకం మొత్తం = _mm_cvtsi128_si32(s);
                కొరకు (; x < వెడల్పు; x++)
                    మొత్తం += (పూర్ణాంకం)src[x];
                dst[0] = మొత్తం;
#లేదావేళ CV_SSE2
                // Intel SSE2: _mm_sad_epu8 — 16 bytes → 2×u64 sum
                స్థిరరాశి __m128i zero = _mm_setzero_si128();
                __m128i vsum = zero;
                పూర్ణాంకం x = 0;
                కొరకు (; x <= వెడల్పు - 16; x += 16)
                    vsum = mm_కూడు_epi64(vsum, _mm_sad_epu8(
                        _mm_loadu_si128((స్థిరరాశి __m128i*)(src + x)), zero));
                __m128i s = mm_కూడు_epi64(vsum, _mm_unpackhi_epi64(vsum, vsum));
                పూర్ణాంకం మొత్తం = _mm_cvtsi128_si32(s);
                కొరకు (; x < వెడల్పు; x++)
                    మొత్తం += (పూర్ణాంకం)src[x];
                dst[0] = మొత్తం;
#లేకపోతే
                స్థిరరాశి పూర్ణాంకం vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 v_కూడిక = vx_setzero_u32();
                v_uint16 v_sum16 = vx_setzero_u16();
                పూర్ణాంకం x = 0, batch = 0;
                స్థిరరాశి పూర్ణాంకం flush_at = 128;

                కొరకు (; x <= వెడల్పు - vlanes8; x += vlanes8)
                {
                    v_uint16 lo, hi;
                    v_expand(vx_ఎక్కించు(src + x), lo, hi);
                    v_sum16 = v_కూడు(v_sum16, v_కూడు(lo, hi));

                    ఒకవేళ (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(v_sum16, a, b);
                        v_కూడిక = v_కూడు(v_కూడిక, v_కూడు(a, b));
                        v_sum16 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(v_sum16, a, b);
                    v_కూడిక = v_కూడు(v_కూడిక, v_కూడు(a, b));
                }

                పూర్ణాంకం మొత్తం = (పూర్ణాంకం)v_reduce_కూడిక(v_కూడిక);
                కొరకు (; x < వెడల్పు; x++)
                    మొత్తం += (పూర్ణాంకం)src[x];
                dst[0] = మొత్తం;
#అయితే_ముగింపు
            }
            లేకపోతే ఒకవేళ (cn == 4)
            {
#ఒకవేళ defined(CV_NEON_DOT)
                // ARMv8.2 DOTPROD: vld4q_u8 deinterleave + vdotq_u32 per-channel
                // vld4q_u8 reads 16 pixels (64 bytes), deinterleaves into 4 × u8x16
                // vdotq_u32 sums each channel's 16 bytes into 4 partial u32 sums
                స్థిరరాశి uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t acc0 = vdupq_n_u32(0), acc1 = vdupq_n_u32(0);
                uint32x4_t acc2 = vdupq_n_u32(0), acc3 = vdupq_n_u32(0);
                పూర్ణాంకం x = 0;
                కొరకు (; x <= నిలువుల_సంఖ్య - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    acc0 = vdotq_u32(acc0, v.val[0], ones);
                    acc1 = vdotq_u32(acc1, v.val[1], ones);
                    acc2 = vdotq_u32(acc2, v.val[2], ones);
                    acc3 = vdotq_u32(acc3, v.val[3], ones);
                }
                dst[0] = (పూర్ణాంకం)vaddvq_u32(acc0);
                dst[1] = (పూర్ణాంకం)vaddvq_u32(acc1);
                dst[2] = (పూర్ణాంకం)vaddvq_u32(acc2);
                dst[3] = (పూర్ణాంకం)vaddvq_u32(acc3);
                కొరకు (; x < నిలువుల_సంఖ్య; x++)
                {
                    dst[0] += (పూర్ణాంకం)src[x * 4];
                    dst[1] += (పూర్ణాంకం)src[x * 4 + 1];
                    dst[2] += (పూర్ణాంకం)src[x * 4 + 2];
                    dst[3] += (పూర్ణాంకం)src[x * 4 + 3];
                }
#లేదావేళ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                // AArch64 fallback: vld4q_u8 deinterleave + vaddlvq_u8 per-channel
                పూర్ణాంకం sums[4] = {0, 0, 0, 0};
                పూర్ణాంకం x = 0;
                కొరకు (; x <= నిలువుల_సంఖ్య - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    sums[0] += (పూర్ణాంకం)vaddlvq_u8(v.val[0]);
                    sums[1] += (పూర్ణాంకం)vaddlvq_u8(v.val[1]);
                    sums[2] += (పూర్ణాంకం)vaddlvq_u8(v.val[2]);
                    sums[3] += (పూర్ణాంకం)vaddlvq_u8(v.val[3]);
                }
                dst[0] = sums[0]; dst[1] = sums[1];
                dst[2] = sums[2]; dst[3] = sums[3];
                కొరకు (; x < నిలువుల_సంఖ్య; x++)
                {
                    dst[0] += (పూర్ణాంకం)src[x * 4];
                    dst[1] += (పూర్ణాంకం)src[x * 4 + 1];
                    dst[2] += (పూర్ణాంకం)src[x * 4 + 2];
                    dst[3] += (పూర్ణాంకం)src[x * 4 + 3];
                }
#లేదావేళ CV_AVX2
                // Intel AVX2: shuffle + unpack to group channels, full-register SAD
                స్థిరరాశి __m256i zero = _mm256_setzero_si256();
                స్థిరరాశి __m256i shuf_ముసుగు = _mm256_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                పూర్ణాంకం x = 0;
                // 16 pixels/iter with unpack (full-register SAD)
                {
                    __m256i కూడిక_bg = zero, కూడిక_ra = zero;
                    కొరకు (; x <= నిలువుల_సంఖ్య - 16; x += 16)
                    {
                        __m256i v0 = _mm256_loadu_si256((స్థిరరాశి __m256i*)(src + x * 4));
                        __m256i v1 = _mm256_loadu_si256((స్థిరరాశి __m256i*)(src + x * 4 + 32));
                        __m256i g0 = _mm256_shuffle_epi8(v0, shuf_ముసుగు);
                        __m256i g1 = _mm256_shuffle_epi8(v1, shuf_ముసుగు);
                        __m256i bg = _mm256_unpacklo_epi32(g0, g1);
                        __m256i ra = _mm256_unpackhi_epi32(g0, g1);
                        కూడిక_bg = mm256_కూడు_epi64(కూడిక_bg, _mm256_sad_epu8(bg, zero));
                        కూడిక_ra = mm256_కూడు_epi64(కూడిక_ra, _mm256_sad_epu8(ra, zero));
                    }
                    __m128i bg_lo = _mm256_castsi256_si128(కూడిక_bg);
                    __m128i bg_hi = _mm256_extracti128_si256(కూడిక_bg, 1);
                    __m128i bg_s = mm_కూడు_epi64(bg_lo, bg_hi);
                    __m128i ra_lo = _mm256_castsi256_si128(కూడిక_ra);
                    __m128i ra_hi = _mm256_extracti128_si256(కూడిక_ra, 1);
                    __m128i ra_s = mm_కూడు_epi64(ra_lo, ra_hi);
                    dst[0] = _mm_cvtsi128_si32(bg_s);                     // B
                    dst[1] = _mm_cvtsi128_si32(_mm_srli_si128(bg_s, 8));  // G
                    dst[2] = _mm_cvtsi128_si32(ra_s);                     // R
                    dst[3] = _mm_cvtsi128_si32(_mm_srli_si128(ra_s, 8));  // A
                }
                // 8-pixel tail with and/srli
                {
                    స్థిరరాశి __m256i ముసుగు_lo32 = _mm256_set1_epi64x(0x00000000FFFFFFFFLL);
                    __m256i కూడిక_br = zero, కూడిక_ga = zero;
                    కొరకు (; x <= నిలువుల_సంఖ్య - 8; x += 8)
                    {
                        __m256i v = _mm256_loadu_si256((స్థిరరాశి __m256i*)(src + x * 4));
                        __m256i grouped = _mm256_shuffle_epi8(v, shuf_ముసుగు);
                        __m256i br = _mm256_and_si256(grouped, ముసుగు_lo32);
                        __m256i ga = _mm256_srli_epi64(grouped, 32);
                        కూడిక_br = mm256_కూడు_epi64(కూడిక_br, _mm256_sad_epu8(br, zero));
                        కూడిక_ga = mm256_కూడు_epi64(కూడిక_ga, _mm256_sad_epu8(ga, zero));
                    }
                    __m128i br_lo = _mm256_castsi256_si128(కూడిక_br);
                    __m128i br_hi = _mm256_extracti128_si256(కూడిక_br, 1);
                    __m128i br_s = mm_కూడు_epi64(br_lo, br_hi);
                    __m128i ga_lo = _mm256_castsi256_si128(కూడిక_ga);
                    __m128i ga_hi = _mm256_extracti128_si256(కూడిక_ga, 1);
                    __m128i ga_s = mm_కూడు_epi64(ga_lo, ga_hi);
                    dst[0] += _mm_cvtsi128_si32(br_s);
                    dst[1] += _mm_cvtsi128_si32(ga_s);
                    dst[2] += _mm_cvtsi128_si32(_mm_srli_si128(br_s, 8));
                    dst[3] += _mm_cvtsi128_si32(_mm_srli_si128(ga_s, 8));
                }
                కొరకు (; x < నిలువుల_సంఖ్య; x++)
                {
                    dst[0] += (పూర్ణాంకం)src[x * 4];
                    dst[1] += (పూర్ణాంకం)src[x * 4 + 1];
                    dst[2] += (పూర్ణాంకం)src[x * 4 + 2];
                    dst[3] += (పూర్ణాంకం)src[x * 4 + 3];
                }
#లేదావేళ CV_SSSE3
                // Intel SSSE3: _mm_shuffle_epi8 deinterleave + _mm_sad_epu8
                // 16 bytes = 4 pixels × 4 channels per iteration
                స్థిరరాశి __m128i zero = _mm_setzero_si128();
                స్థిరరాశి __m128i shuf_ముసుగు = _mm_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                స్థిరరాశి __m128i ముసుగు_lo32 = _mm_set1_epi64x(0x00000000FFFFFFFFLL);
                __m128i కూడిక_br = zero, కూడిక_ga = zero;
                పూర్ణాంకం x = 0;
                కొరకు (; x <= నిలువుల_సంఖ్య - 4; x += 4)
                {
                    __m128i v = _mm_loadu_si128((స్థిరరాశి __m128i*)(src + x * 4));
                    __m128i grouped = _mm_shuffle_epi8(v, shuf_ముసుగు);
                    __m128i br = _mm_and_si128(grouped, ముసుగు_lo32);
                    __m128i ga = _mm_srli_epi64(grouped, 32);
                    కూడిక_br = mm_కూడు_epi64(కూడిక_br, _mm_sad_epu8(br, zero));
                    కూడిక_ga = mm_కూడు_epi64(కూడిక_ga, _mm_sad_epu8(ga, zero));
                }
                dst[0] = _mm_cvtsi128_si32(కూడిక_br);
                dst[1] = _mm_cvtsi128_si32(కూడిక_ga);
                dst[2] = _mm_cvtsi128_si32(_mm_srli_si128(కూడిక_br, 8));
                dst[3] = _mm_cvtsi128_si32(_mm_srli_si128(కూడిక_ga, 8));
                కొరకు (; x < నిలువుల_సంఖ్య; x++)
                {
                    dst[0] += (పూర్ణాంకం)src[x * 4];
                    dst[1] += (పూర్ణాంకం)src[x * 4 + 1];
                    dst[2] += (పూర్ణాంకం)src[x * 4 + 2];
                    dst[3] += (పూర్ణాంకం)src[x * 4 + 3];
                }
#లేకపోతే
                స్థిరరాశి పూర్ణాంకం vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 vsum0 = vx_setzero_u32(), vsum1 = vx_setzero_u32();
                v_uint32 vsum2 = vx_setzero_u32(), vsum3 = vx_setzero_u32();
                v_uint16 vs16_0 = vx_setzero_u16(), vs16_1 = vx_setzero_u16();
                v_uint16 vs16_2 = vx_setzero_u16(), vs16_3 = vx_setzero_u16();
                పూర్ణాంకం x = 0, batch = 0;
                స్థిరరాశి పూర్ణాంకం flush_at = 128;

                కొరకు (; x <= నిలువుల_సంఖ్య - vlanes8; x += vlanes8)
                {
                    v_uint8 ch0, ch1, ch2, ch3;
                    v_ఎక్కించు_deinterleave(src + x * 4, ch0, ch1, ch2, ch3);

                    v_uint16 lo, hi;
                    v_expand(ch0, lo, hi); vs16_0 = v_కూడు(vs16_0, v_కూడు(lo, hi));
                    v_expand(ch1, lo, hi); vs16_1 = v_కూడు(vs16_1, v_కూడు(lo, hi));
                    v_expand(ch2, lo, hi); vs16_2 = v_కూడు(vs16_2, v_కూడు(lo, hi));
                    v_expand(ch3, lo, hi); vs16_3 = v_కూడు(vs16_3, v_కూడు(lo, hi));

                    ఒకవేళ (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(vs16_0, a, b); vsum0 = v_కూడు(vsum0, v_కూడు(a, b));
                        v_expand(vs16_1, a, b); vsum1 = v_కూడు(vsum1, v_కూడు(a, b));
                        v_expand(vs16_2, a, b); vsum2 = v_కూడు(vsum2, v_కూడు(a, b));
                        v_expand(vs16_3, a, b); vsum3 = v_కూడు(vsum3, v_కూడు(a, b));
                        vs16_0 = vx_setzero_u16(); vs16_1 = vx_setzero_u16();
                        vs16_2 = vx_setzero_u16(); vs16_3 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(vs16_0, a, b); vsum0 = v_కూడు(vsum0, v_కూడు(a, b));
                    v_expand(vs16_1, a, b); vsum1 = v_కూడు(vsum1, v_కూడు(a, b));
                    v_expand(vs16_2, a, b); vsum2 = v_కూడు(vsum2, v_కూడు(a, b));
                    v_expand(vs16_3, a, b); vsum3 = v_కూడు(vsum3, v_కూడు(a, b));
                }

                dst[0] = (పూర్ణాంకం)v_reduce_కూడిక(vsum0);
                dst[1] = (పూర్ణాంకం)v_reduce_కూడిక(vsum1);
                dst[2] = (పూర్ణాంకం)v_reduce_కూడిక(vsum2);
                dst[3] = (పూర్ణాంకం)v_reduce_కూడిక(vsum3);

                కొరకు (; x < నిలువుల_సంఖ్య; x++)
                {
                    dst[0] += (పూర్ణాంకం)src[x * 4];
                    dst[1] += (పూర్ణాంకం)src[x * 4 + 1];
                    dst[2] += (పూర్ణాంకం)src[x * 4 + 2];
                    dst[3] += (పూర్ణాంకం)src[x * 4 + 3];
                }
#అయితే_ముగింపు
            }
            లేకపోతే
            {
                // generic cn: scalar fallback
                కొరకు (పూర్ణాంకం c = 0; c < cn; c++)
                    dst[c] = (పూర్ణాంకం)src[c];
                కొరకు (పూర్ణాంకం x = cn; x < వెడల్పు; x += cn)
                    కొరకు (పూర్ణాంకం c = 0; c < cn; c++)
                        dst[c] += (పూర్ణాంకం)src[x + c];
            }
        }
    };

    parallel_for_(Range(0, srcmat.అడ్డువరుసలు), body);
    v_cleanup();
}

// --- uchar → float ---
స్థిర శూన్య reduce_నిలువు_కూడిక_8u32f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{
    స్థిరరాశి పూర్ణాంకం cn = srcmat.channels();
    స్థిరరాశి పూర్ణాంకం నిలువుల_సంఖ్య = srcmat.నిలువుల_సంఖ్య;
    స్థిరరాశి పూర్ణాంకం వెడల్పు = నిలువుల_సంఖ్య * cn;

    స్వయంచాలక body = [&](స్థిరరాశి Range& range) {
        కొరకు (పూర్ణాంకం y = range.ప్రారంభం; y < range.end; y++)
        {
            స్థిరరాశి uchar* src = srcmat.ptr<uchar>(y);
            దశాంశం* dst = dstmat.ptr<దశాంశం>(y);

            // compute in int, then convert to float
            Auto_బఫరు<పూర్ణాంకం> ibuf(cn);
            పూర్ణాంకం* sums = ibuf.దత్తాంశం();
            కొరకు (పూర్ణాంకం c = 0; c < cn; c++)
                sums[c] = 0;

            ఒకవేళ (cn == 1)
            {
#ఒకవేళ defined(CV_NEON_DOT)
                స్థిరరాశి uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t v_కూడిక = vdupq_n_u32(0);
                పూర్ణాంకం x = 0;
                కొరకు (; x <= వెడల్పు - 16; x += 16)
                    v_కూడిక = vdotq_u32(v_కూడిక, vld1q_u8(src + x), ones);
                sums[0] = (పూర్ణాంకం)vaddvq_u32(v_కూడిక);
                కొరకు (; x < వెడల్పు; x++)
                    sums[0] += (పూర్ణాంకం)src[x];
#లేదావేళ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                uint32x4_t v_కూడిక = vdupq_n_u32(0);
                పూర్ణాంకం x = 0;
                కొరకు (; x <= వెడల్పు - 16; x += 16)
                    v_కూడిక = vaddq_u32(v_కూడిక, vpaddlq_u16(vpaddlq_u8(vld1q_u8(src + x))));
                sums[0] = (పూర్ణాంకం)vaddvq_u32(v_కూడిక);
                కొరకు (; x < వెడల్పు; x++)
                    sums[0] += (పూర్ణాంకం)src[x];
#లేదావేళ CV_AVX2
                స్థిరరాశి __m256i zero = _mm256_setzero_si256();
                __m256i vsum = zero;
                పూర్ణాంకం x = 0;
                కొరకు (; x <= వెడల్పు - 32; x += 32)
                    vsum = mm256_కూడు_epi64(vsum, _mm256_sad_epu8(
                        _mm256_loadu_si256((స్థిరరాశి __m256i*)(src + x)), zero));
                // 8-byte tail: reduce max scalar tail from 31 to 7
                // Use _mm256_inserti128_si256 instead of _mm256_castsi128_si256:
                // the latter leaves upper 128 bits undefined per Intel spec,
                // causing MSVC to expose stale register data to _mm256_sad_epu8.
                కొరకు (; x <= వెడల్పు - 8; x += 8)
                    vsum = mm256_కూడు_epi64(vsum, _mm256_sad_epu8(
                        _mm256_inserti128_si256(_mm256_setzero_si256(), _mm_loadl_epi64((స్థిరరాశి __m128i*)(src + x)), 0), zero));
                __m128i lo128 = _mm256_castsi256_si128(vsum);
                __m128i hi128 = _mm256_extracti128_si256(vsum, 1);
                __m128i s = mm_కూడు_epi64(lo128, hi128);
                s = mm_కూడు_epi64(s, _mm_unpackhi_epi64(s, s));
                sums[0] = _mm_cvtsi128_si32(s);
                కొరకు (; x < వెడల్పు; x++)
                    sums[0] += (పూర్ణాంకం)src[x];
#లేదావేళ CV_SSE2
                స్థిరరాశి __m128i zero = _mm_setzero_si128();
                __m128i vsum = zero;
                పూర్ణాంకం x = 0;
                కొరకు (; x <= వెడల్పు - 16; x += 16)
                    vsum = mm_కూడు_epi64(vsum, _mm_sad_epu8(
                        _mm_loadu_si128((స్థిరరాశి __m128i*)(src + x)), zero));
                __m128i s = mm_కూడు_epi64(vsum, _mm_unpackhi_epi64(vsum, vsum));
                sums[0] = _mm_cvtsi128_si32(s);
                కొరకు (; x < వెడల్పు; x++)
                    sums[0] += (పూర్ణాంకం)src[x];
#లేకపోతే
                స్థిరరాశి పూర్ణాంకం vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 v_కూడిక = vx_setzero_u32();
                v_uint16 v_sum16 = vx_setzero_u16();
                పూర్ణాంకం x = 0, batch = 0;

                కొరకు (; x <= వెడల్పు - vlanes8; x += vlanes8)
                {
                    v_uint16 lo, hi;
                    v_expand(vx_ఎక్కించు(src + x), lo, hi);
                    v_sum16 = v_కూడు(v_sum16, v_కూడు(lo, hi));
                    ఒకవేళ (++batch >= 128)
                    {
                        v_uint32 a, b;
                        v_expand(v_sum16, a, b);
                        v_కూడిక = v_కూడు(v_కూడిక, v_కూడు(a, b));
                        v_sum16 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                v_uint32 a, b;
                v_expand(v_sum16, a, b);
                v_కూడిక = v_కూడు(v_కూడిక, v_కూడు(a, b));

                sums[0] = (పూర్ణాంకం)v_reduce_కూడిక(v_కూడిక);
                కొరకు (; x < వెడల్పు; x++)
                    sums[0] += (పూర్ణాంకం)src[x];
#అయితే_ముగింపు
            }
            లేకపోతే ఒకవేళ (cn == 4)
            {
#ఒకవేళ defined(CV_NEON_DOT)
                స్థిరరాశి uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t acc0 = vdupq_n_u32(0), acc1 = vdupq_n_u32(0);
                uint32x4_t acc2 = vdupq_n_u32(0), acc3 = vdupq_n_u32(0);
                పూర్ణాంకం x = 0;
                కొరకు (; x <= నిలువుల_సంఖ్య - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    acc0 = vdotq_u32(acc0, v.val[0], ones);
                    acc1 = vdotq_u32(acc1, v.val[1], ones);
                    acc2 = vdotq_u32(acc2, v.val[2], ones);
                    acc3 = vdotq_u32(acc3, v.val[3], ones);
                }
                sums[0] = (పూర్ణాంకం)vaddvq_u32(acc0);
                sums[1] = (పూర్ణాంకం)vaddvq_u32(acc1);
                sums[2] = (పూర్ణాంకం)vaddvq_u32(acc2);
                sums[3] = (పూర్ణాంకం)vaddvq_u32(acc3);
                కొరకు (; x < నిలువుల_సంఖ్య; x++)
                {
                    sums[0] += (పూర్ణాంకం)src[x * 4];
                    sums[1] += (పూర్ణాంకం)src[x * 4 + 1];
                    sums[2] += (పూర్ణాంకం)src[x * 4 + 2];
                    sums[3] += (పూర్ణాంకం)src[x * 4 + 3];
                }
#లేదావేళ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                పూర్ణాంకం x = 0;
                కొరకు (; x <= నిలువుల_సంఖ్య - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    sums[0] += (పూర్ణాంకం)vaddlvq_u8(v.val[0]);
                    sums[1] += (పూర్ణాంకం)vaddlvq_u8(v.val[1]);
                    sums[2] += (పూర్ణాంకం)vaddlvq_u8(v.val[2]);
                    sums[3] += (పూర్ణాంకం)vaddlvq_u8(v.val[3]);
                }
                కొరకు (; x < నిలువుల_సంఖ్య; x++)
                {
                    sums[0] += (పూర్ణాంకం)src[x * 4];
                    sums[1] += (పూర్ణాంకం)src[x * 4 + 1];
                    sums[2] += (పూర్ణాంకం)src[x * 4 + 2];
                    sums[3] += (పూర్ణాంకం)src[x * 4 + 3];
                }
#లేదావేళ CV_AVX2
                // Intel AVX2: shuffle + unpack to group channels, full-register SAD
                స్థిరరాశి __m256i zero = _mm256_setzero_si256();
                స్థిరరాశి __m256i shuf_ముసుగు = _mm256_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                పూర్ణాంకం x = 0;
                // 16 pixels/iter with unpack (full-register SAD)
                {
                    __m256i కూడిక_bg = zero, కూడిక_ra = zero;
                    కొరకు (; x <= నిలువుల_సంఖ్య - 16; x += 16)
                    {
                        __m256i v0 = _mm256_loadu_si256((స్థిరరాశి __m256i*)(src + x * 4));
                        __m256i v1 = _mm256_loadu_si256((స్థిరరాశి __m256i*)(src + x * 4 + 32));
                        __m256i g0 = _mm256_shuffle_epi8(v0, shuf_ముసుగు);
                        __m256i g1 = _mm256_shuffle_epi8(v1, shuf_ముసుగు);
                        __m256i bg = _mm256_unpacklo_epi32(g0, g1);
                        __m256i ra = _mm256_unpackhi_epi32(g0, g1);
                        కూడిక_bg = mm256_కూడు_epi64(కూడిక_bg, _mm256_sad_epu8(bg, zero));
                        కూడిక_ra = mm256_కూడు_epi64(కూడిక_ra, _mm256_sad_epu8(ra, zero));
                    }
                    __m128i bg_lo = _mm256_castsi256_si128(కూడిక_bg);
                    __m128i bg_hi = _mm256_extracti128_si256(కూడిక_bg, 1);
                    __m128i bg_s = mm_కూడు_epi64(bg_lo, bg_hi);
                    __m128i ra_lo = _mm256_castsi256_si128(కూడిక_ra);
                    __m128i ra_hi = _mm256_extracti128_si256(కూడిక_ra, 1);
                    __m128i ra_s = mm_కూడు_epi64(ra_lo, ra_hi);
                    sums[0] = _mm_cvtsi128_si32(bg_s);                     // B
                    sums[1] = _mm_cvtsi128_si32(_mm_srli_si128(bg_s, 8));  // G
                    sums[2] = _mm_cvtsi128_si32(ra_s);                     // R
                    sums[3] = _mm_cvtsi128_si32(_mm_srli_si128(ra_s, 8));  // A
                }
                // 8-pixel tail with and/srli
                {
                    స్థిరరాశి __m256i ముసుగు_lo32 = _mm256_set1_epi64x(0x00000000FFFFFFFFLL);
                    __m256i కూడిక_br = zero, కూడిక_ga = zero;
                    కొరకు (; x <= నిలువుల_సంఖ్య - 8; x += 8)
                    {
                        __m256i v = _mm256_loadu_si256((స్థిరరాశి __m256i*)(src + x * 4));
                        __m256i grouped = _mm256_shuffle_epi8(v, shuf_ముసుగు);
                        __m256i br = _mm256_and_si256(grouped, ముసుగు_lo32);
                        __m256i ga = _mm256_srli_epi64(grouped, 32);
                        కూడిక_br = mm256_కూడు_epi64(కూడిక_br, _mm256_sad_epu8(br, zero));
                        కూడిక_ga = mm256_కూడు_epi64(కూడిక_ga, _mm256_sad_epu8(ga, zero));
                    }
                    __m128i br_lo = _mm256_castsi256_si128(కూడిక_br);
                    __m128i br_hi = _mm256_extracti128_si256(కూడిక_br, 1);
                    __m128i br_s = mm_కూడు_epi64(br_lo, br_hi);
                    __m128i ga_lo = _mm256_castsi256_si128(కూడిక_ga);
                    __m128i ga_hi = _mm256_extracti128_si256(కూడిక_ga, 1);
                    __m128i ga_s = mm_కూడు_epi64(ga_lo, ga_hi);
                    sums[0] += _mm_cvtsi128_si32(br_s);
                    sums[1] += _mm_cvtsi128_si32(ga_s);
                    sums[2] += _mm_cvtsi128_si32(_mm_srli_si128(br_s, 8));
                    sums[3] += _mm_cvtsi128_si32(_mm_srli_si128(ga_s, 8));
                }
                కొరకు (; x < నిలువుల_సంఖ్య; x++)
                {
                    sums[0] += (పూర్ణాంకం)src[x * 4];
                    sums[1] += (పూర్ణాంకం)src[x * 4 + 1];
                    sums[2] += (పూర్ణాంకం)src[x * 4 + 2];
                    sums[3] += (పూర్ణాంకం)src[x * 4 + 3];
                }
#లేదావేళ CV_SSSE3
                // Intel SSSE3: _mm_shuffle_epi8 deinterleave + _mm_sad_epu8
                స్థిరరాశి __m128i zero = _mm_setzero_si128();
                స్థిరరాశి __m128i shuf_ముసుగు = _mm_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                స్థిరరాశి __m128i ముసుగు_lo32 = _mm_set1_epi64x(0x00000000FFFFFFFFLL);
                __m128i కూడిక_br = zero, కూడిక_ga = zero;
                పూర్ణాంకం x = 0;
                కొరకు (; x <= నిలువుల_సంఖ్య - 4; x += 4)
                {
                    __m128i v = _mm_loadu_si128((స్థిరరాశి __m128i*)(src + x * 4));
                    __m128i grouped = _mm_shuffle_epi8(v, shuf_ముసుగు);
                    __m128i br = _mm_and_si128(grouped, ముసుగు_lo32);
                    __m128i ga = _mm_srli_epi64(grouped, 32);
                    కూడిక_br = mm_కూడు_epi64(కూడిక_br, _mm_sad_epu8(br, zero));
                    కూడిక_ga = mm_కూడు_epi64(కూడిక_ga, _mm_sad_epu8(ga, zero));
                }
                sums[0] = _mm_cvtsi128_si32(కూడిక_br);
                sums[1] = _mm_cvtsi128_si32(కూడిక_ga);
                sums[2] = _mm_cvtsi128_si32(_mm_srli_si128(కూడిక_br, 8));
                sums[3] = _mm_cvtsi128_si32(_mm_srli_si128(కూడిక_ga, 8));
                కొరకు (; x < నిలువుల_సంఖ్య; x++)
                {
                    sums[0] += (పూర్ణాంకం)src[x * 4];
                    sums[1] += (పూర్ణాంకం)src[x * 4 + 1];
                    sums[2] += (పూర్ణాంకం)src[x * 4 + 2];
                    sums[3] += (పూర్ణాంకం)src[x * 4 + 3];
                }
#లేకపోతే
                స్థిరరాశి పూర్ణాంకం vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 vsum0 = vx_setzero_u32(), vsum1 = vx_setzero_u32();
                v_uint32 vsum2 = vx_setzero_u32(), vsum3 = vx_setzero_u32();
                v_uint16 vs16_0 = vx_setzero_u16(), vs16_1 = vx_setzero_u16();
                v_uint16 vs16_2 = vx_setzero_u16(), vs16_3 = vx_setzero_u16();
                పూర్ణాంకం x = 0, batch = 0;
                స్థిరరాశి పూర్ణాంకం flush_at = 128;

                కొరకు (; x <= నిలువుల_సంఖ్య - vlanes8; x += vlanes8)
                {
                    v_uint8 ch0, ch1, ch2, ch3;
                    v_ఎక్కించు_deinterleave(src + x * 4, ch0, ch1, ch2, ch3);

                    v_uint16 lo, hi;
                    v_expand(ch0, lo, hi); vs16_0 = v_కూడు(vs16_0, v_కూడు(lo, hi));
                    v_expand(ch1, lo, hi); vs16_1 = v_కూడు(vs16_1, v_కూడు(lo, hi));
                    v_expand(ch2, lo, hi); vs16_2 = v_కూడు(vs16_2, v_కూడు(lo, hi));
                    v_expand(ch3, lo, hi); vs16_3 = v_కూడు(vs16_3, v_కూడు(lo, hi));

                    ఒకవేళ (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(vs16_0, a, b); vsum0 = v_కూడు(vsum0, v_కూడు(a, b));
                        v_expand(vs16_1, a, b); vsum1 = v_కూడు(vsum1, v_కూడు(a, b));
                        v_expand(vs16_2, a, b); vsum2 = v_కూడు(vsum2, v_కూడు(a, b));
                        v_expand(vs16_3, a, b); vsum3 = v_కూడు(vsum3, v_కూడు(a, b));
                        vs16_0 = vx_setzero_u16(); vs16_1 = vx_setzero_u16();
                        vs16_2 = vx_setzero_u16(); vs16_3 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(vs16_0, a, b); vsum0 = v_కూడు(vsum0, v_కూడు(a, b));
                    v_expand(vs16_1, a, b); vsum1 = v_కూడు(vsum1, v_కూడు(a, b));
                    v_expand(vs16_2, a, b); vsum2 = v_కూడు(vsum2, v_కూడు(a, b));
                    v_expand(vs16_3, a, b); vsum3 = v_కూడు(vsum3, v_కూడు(a, b));
                }

                sums[0] = (పూర్ణాంకం)v_reduce_కూడిక(vsum0);
                sums[1] = (పూర్ణాంకం)v_reduce_కూడిక(vsum1);
                sums[2] = (పూర్ణాంకం)v_reduce_కూడిక(vsum2);
                sums[3] = (పూర్ణాంకం)v_reduce_కూడిక(vsum3);

                కొరకు (; x < నిలువుల_సంఖ్య; x++)
                {
                    sums[0] += (పూర్ణాంకం)src[x * 4];
                    sums[1] += (పూర్ణాంకం)src[x * 4 + 1];
                    sums[2] += (పూర్ణాంకం)src[x * 4 + 2];
                    sums[3] += (పూర్ణాంకం)src[x * 4 + 3];
                }
#అయితే_ముగింపు
            }
            లేకపోతే
            {
                కొరకు (పూర్ణాంకం x = 0, c = 0; x < వెడల్పు; x++, c = (c+1)&-(c < cn-1))
                    sums[c] += (పూర్ణాంకం)src[x];
            }
            కొరకు (పూర్ణాంకం c = 0; c < cn; c++)
                dst[c] = (దశాంశం)sums[c];
        }
    };
    parallel_for_(Range(0, srcmat.అడ్డువరుసలు), body);
    v_cleanup();
}

// --- 16-bit (ushort/short) → float ---
మూస<రకం_పేరు SrcT, రకం_పేరు VecSrc, రకం_పేరు VecDst>
స్థిర శూన్య reduce_నిలువు_కూడిక_16_32f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{
    స్థిరరాశి పూర్ణాంకం cn = srcmat.channels();
    స్థిరరాశి పూర్ణాంకం వెడల్పు = srcmat.నిలువుల_సంఖ్య * cn;
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<VecSrc>::vlanes();

    స్వయంచాలక body = [&](స్థిరరాశి Range& range) {
        కొరకు (పూర్ణాంకం y = range.ప్రారంభం; y < range.end; y++)
        {
            స్థిరరాశి SrcT* src = srcmat.ptr<SrcT>(y);
            దశాంశం* dst = dstmat.ptr<దశాంశం>(y);

            ఒకవేళ (cn == 1)
            {
                VecDst v_sum0 = v_setzero_<VecDst>(), v_sum1 = v_setzero_<VecDst>();
                పూర్ణాంకం x = 0;
                కొరకు (; x <= వెడల్పు - vlanes; x += vlanes)
                {
                    VecDst lo, hi;
                    v_expand(vx_ఎక్కించు(src + x), lo, hi);
                    v_sum0 = v_కూడు(v_sum0, lo);
                    v_sum1 = v_కూడు(v_sum1, hi);
                }
                dst[0] = (దశాంశం)(v_reduce_కూడిక(v_sum0) + v_reduce_కూడిక(v_sum1));
                కొరకు (; x < వెడల్పు; x++)
                    dst[0] += (దశాంశం)src[x];
            }
            లేకపోతే
            {
                కొరకు (పూర్ణాంకం c = 0; c < cn; c++)
                    dst[c] = 0;
                కొరకు (పూర్ణాంకం x = 0, c = 0; x < వెడల్పు; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += (దశాంశం)src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.అడ్డువరుసలు), body);
    v_cleanup();
}

స్థిర శూన్య reduce_నిలువు_కూడిక_16u32f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{ reduce_నిలువు_కూడిక_16_32f<ushort, v_uint16, v_uint32>(srcmat, dstmat); }

స్థిర శూన్య reduce_నిలువు_కూడిక_16s32f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{ reduce_నిలువు_కూడిక_16_32f<పొట్టి, v_int16, v_int32>(srcmat, dstmat); }

// --- float/double → same type ---
మూస<రకం_పేరు T, రకం_పేరు VecT>
స్థిర శూన్య reduce_నిలువు_కూడిక_FP(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{
    స్థిరరాశి పూర్ణాంకం cn = srcmat.channels();
    స్థిరరాశి పూర్ణాంకం వెడల్పు = srcmat.నిలువుల_సంఖ్య * cn;
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<VecT>::vlanes();

    స్వయంచాలక body = [&](స్థిరరాశి Range& range) {
        కొరకు (పూర్ణాంకం y = range.ప్రారంభం; y < range.end; y++)
        {
            స్థిరరాశి T* src = srcmat.ptr<T>(y);
            T* dst = dstmat.ptr<T>(y);

            ఒకవేళ (cn == 1)
            {
                VecT s0 = v_setzero_<VecT>(), s1 = v_setzero_<VecT>();
                VecT s2 = v_setzero_<VecT>(), s3 = v_setzero_<VecT>();
                పూర్ణాంకం x = 0;
                కొరకు (; x <= వెడల్పు - vlanes * 4; x += vlanes * 4)
                {
                    s0 = v_కూడు(s0, vx_ఎక్కించు(src + x));
                    s1 = v_కూడు(s1, vx_ఎక్కించు(src + x + vlanes));
                    s2 = v_కూడు(s2, vx_ఎక్కించు(src + x + vlanes * 2));
                    s3 = v_కూడు(s3, vx_ఎక్కించు(src + x + vlanes * 3));
                }
                s0 = v_కూడు(v_కూడు(s0, s1), v_కూడు(s2, s3));
                కొరకు (; x <= వెడల్పు - vlanes; x += vlanes)
                    s0 = v_కూడు(s0, vx_ఎక్కించు(src + x));
                T మొత్తం = (T)v_reduce_కూడిక(s0);
                కొరకు (; x < వెడల్పు; x++)
                    మొత్తం += src[x];
                dst[0] = మొత్తం;
            }
            లేకపోతే
            {
                కొరకు (పూర్ణాంకం c = 0; c < cn; c++)
                    dst[c] = 0;
                కొరకు (పూర్ణాంకం x = 0, c = 0; x < వెడల్పు; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.అడ్డువరుసలు), body);
    v_cleanup();
}

స్థిర శూన్య reduce_నిలువు_కూడిక_32f32f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{ reduce_నిలువు_కూడిక_FP<దశాంశం, v_float32>(srcmat, dstmat); }

#ఒకవేళ CV_SIMD_64F
// --- float → double ---
స్థిర శూన్య reduce_నిలువు_కూడిక_32f64f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{
    స్థిరరాశి పూర్ణాంకం cn = srcmat.channels();
    స్థిరరాశి పూర్ణాంకం వెడల్పు = srcmat.నిలువుల_సంఖ్య * cn;
    స్థిరరాశి పూర్ణాంకం vlanes32 = VTraits<v_float32>::vlanes();

    స్వయంచాలక body = [&](స్థిరరాశి Range& range) {
        కొరకు (పూర్ణాంకం y = range.ప్రారంభం; y < range.end; y++)
        {
            స్థిరరాశి దశాంశం* src = srcmat.ptr<దశాంశం>(y);
            ద్విగుణ* dst = dstmat.ptr<ద్విగుణ>(y);

            ఒకవేళ (cn == 1)
            {
                v_float64 v_sum0 = vx_setzero_f64();
                v_float64 v_sum1 = vx_setzero_f64();
                పూర్ణాంకం x = 0;
                కొరకు (; x <= వెడల్పు - vlanes32; x += vlanes32)
                {
                    v_float32 v = vx_ఎక్కించు(src + x);
                    v_sum0 = v_కూడు(v_sum0, v_cvt_f64(v));
                    v_sum1 = v_కూడు(v_sum1, v_cvt_f64_ఎగువ(v));
                }
                v_float64 v_మొత్తం = v_కూడు(v_sum0, v_sum1);

                ద్విగుణ మొత్తం = v_reduce_కూడిక(v_మొత్తం);
                కొరకు (; x < వెడల్పు; x++)
                    మొత్తం += (ద్విగుణ)src[x];
                dst[0] = మొత్తం;
            }
            లేకపోతే
            {
                కొరకు (పూర్ణాంకం c = 0; c < cn; c++)
                    dst[c] = 0;
                కొరకు (పూర్ణాంకం x = 0, c = 0; x < వెడల్పు; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += (ద్విగుణ)src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.అడ్డువరుసలు), body);
    v_cleanup();
}

స్థిర శూన్య reduce_నిలువు_కూడిక_64f64f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{ reduce_నిలువు_కూడిక_FP<ద్విగుణ, v_float64>(srcmat, dstmat); }
#అయితే_ముగింపు // CV_SIMD_64F

// =====================================================================
//  Row reduce SUM (dim=0): sum each column across all rows
// =====================================================================

// --- uchar → int ---
// Uses u16 intermediate accumulator for vertical sum: accumulate u8→u16 per row,
// flush u16→u32 every 256 rows (max u16 value: 256*255=65280 < 65535)
స్థిర శూన్య reduce_అడ్డువరుస_కూడిక_8u32s(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{
    స్థిరరాశి పూర్ణాంకం వెడల్పు_cn = srcmat.నిలువుల_సంఖ్య * srcmat.channels();
    స్థిరరాశి పూర్ణాంకం ఎత్తు = srcmat.అడ్డువరుసలు;
    స్థిరరాశి పూర్ణాంకం vlanes16 = VTraits<v_uint16>::vlanes();
    స్థిరరాశి పూర్ణాంకం vlanes32 = VTraits<v_int32>::vlanes();

    స్వయంచాలక body = [&](స్థిరరాశి Range& range) {
        స్థిరరాశి పూర్ణాంకం ప్రారంభం = range.ప్రారంభం;
        స్థిరరాశి పూర్ణాంకం end = range.end;
        స్థిరరాశి పూర్ణాంకం len = end - ప్రారంభం;

        Auto_బఫరు<పూర్ణాంకం> buf32_storage(len);
        Auto_బఫరు<ushort> buf16_storage(len);
        పూర్ణాంకం* buf32 = buf32_storage.దత్తాంశం();
        ushort* buf16 = buf16_storage.దత్తాంశం();

        // init from first row
        స్థిరరాశి uchar* src0 = srcmat.ptr<uchar>(0) + ప్రారంభం;
        కొరకు (పూర్ణాంకం i = 0; i < len; i++)
            buf32[i] = (పూర్ణాంకం)src0[i];

        ఒకవేళ (ఎత్తు <= 1)
        {
            పూర్ణాంకం* dst = dstmat.ptr<పూర్ణాంకం>(0) + ప్రారంభం;
            కొరకు (పూర్ణాంకం i = 0; i < len; i++)
                dst[i] = buf32[i];
            తిరిగి;
        }

        memset(buf16, 0, len * పరిమాణం(ushort));
        స్థిరరాశి పూర్ణాంకం flush_at = 256; // 256*255 = 65280 < 65535 (u16 safe)
        పూర్ణాంకం flush_లెక్క = 0;

        కొరకు (పూర్ణాంకం అడ్డువరుస = 1; అడ్డువరుస < ఎత్తు; అడ్డువరుస++)
        {
            స్థిరరాశి uchar* src = srcmat.ptr<uchar>(అడ్డువరుస) + ప్రారంభం;
            పూర్ణాంకం i = 0;

#ఒకవేళ CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
            // AArch64: vaddw_u8 — single-instruction widening add (u16 += u8)
            కొరకు (; i <= len - 16; i += 16)
            {
                uint8x16_t v = vld1q_u8(src + i);
                uint16x8_t acc_lo = vld1q_u16(buf16 + i);
                uint16x8_t acc_hi = vld1q_u16(buf16 + i + 8);
                vst1q_u16(buf16 + i,     vaddw_u8(acc_lo, vget_దిగువ_u8(v)));
                vst1q_u16(buf16 + i + 8, vaddw_ఎగువ_u8(acc_hi, v));
            }
#లేదావేళ CV_RVV
            స్థిరరాశి పూర్ణాంకం vl = (పూర్ణాంకం)__riscv_vsetvlmax_e8m2();
            కొరకు (; i <= len - vl; i += vl)
            {
                vuint8m2_t vs = __riscv_vle8_v_u8m2(src + i, vl);
                vuint16m4_t va = __riscv_vle16_v_u16m4(buf16 + i, vl);
                va = __riscv_vwaddu_wv_u16m4(va, vs, vl);
                __riscv_vse16_v_u16m4(buf16 + i, va, vl);
            }
#లేకపోతే
            స్థిరరాశి పూర్ణాంకం vlanes8 = VTraits<v_uint8>::vlanes();
            కొరకు (; i <= len - vlanes8; i += vlanes8)
            {
                v_uint16 lo, hi;
                v_expand(vx_ఎక్కించు(src + i), lo, hi);
                v_store(buf16 + i, v_కూడు(vx_ఎక్కించు(buf16 + i), lo));
                v_store(buf16 + i + vlanes16, v_కూడు(vx_ఎక్కించు(buf16 + i + vlanes16), hi));
            }
#అయితే_ముగింపు
            కొరకు (; i < len; i++)
                buf16[i] += (ushort)src[i];

            ఒకవేళ (++flush_లెక్క >= flush_at || అడ్డువరుస == ఎత్తు - 1)
            {
                // flush u16 → u32
                పూర్ణాంకం j = 0;
                కొరకు (; j <= len - vlanes16; j += vlanes16)
                {
                    v_uint32 lo, hi;
                    v_expand(vx_ఎక్కించు(buf16 + j), lo, hi);
                    v_store(buf32 + j, v_కూడు(vx_ఎక్కించు(buf32 + j), v_reinterpret_as_s32(lo)));
                    v_store(buf32 + j + vlanes32, v_కూడు(vx_ఎక్కించు(buf32 + j + vlanes32), v_reinterpret_as_s32(hi)));
                }
                కొరకు (; j < len; j++)
                    buf32[j] += (పూర్ణాంకం)buf16[j];
                memset(buf16, 0, len * పరిమాణం(ushort));
                flush_లెక్క = 0;
            }
        }

        పూర్ణాంకం* dst = dstmat.ptr<పూర్ణాంకం>(0) + ప్రారంభం;
        కొరకు (పూర్ణాంకం i = 0; i < len; i++)
            dst[i] = buf32[i];
    };

    parallel_for_(Range(0, వెడల్పు_cn), body, వెడల్పు_cn * CV_ELEM_కొలత(srcmat.లోతు()) / 64);
    v_cleanup();
}

// --- uchar → float ---
స్థిర శూన్య reduce_అడ్డువరుస_కూడిక_8u32f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{
    // compute in int, then convert to float
    Mat తాత్కాలిక_2(dstmat.అడ్డువరుసలు, dstmat.నిలువుల_సంఖ్య, CV_32SC(srcmat.channels()));
    reduce_అడ్డువరుస_కూడిక_8u32s(srcmat, తాత్కాలిక_2);
    తాత్కాలిక_2.convert_గమ్యం(dstmat, dstmat.type());
}

// --- 16-bit (ushort/short) → float row reduce ---
మూస<రకం_పేరు SrcT, రకం_పేరు VecSrc, రకం_పేరు VecDst>
స్థిర శూన్య reduce_అడ్డువరుస_కూడిక_16_32f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{
    స్థిరరాశి పూర్ణాంకం వెడల్పు_cn = srcmat.నిలువుల_సంఖ్య * srcmat.channels();
    స్థిరరాశి పూర్ణాంకం ఎత్తు = srcmat.అడ్డువరుసలు;
    స్థిరరాశి పూర్ణాంకం vlanes16 = VTraits<VecSrc>::vlanes();
    స్థిరరాశి పూర్ణాంకం vlanes32 = vlanes16 / 2;

    స్వయంచాలక body = [&](స్థిరరాశి Range& range) {
        స్థిరరాశి పూర్ణాంకం ప్రారంభం = range.ప్రారంభం;
        స్థిరరాశి పూర్ణాంకం end = range.end;
        స్థిరరాశి పూర్ణాంకం len = end - ప్రారంభం;
        దశాంశం* dst = dstmat.ptr<దశాంశం>(0) + ప్రారంభం;

        పూర్ణాంకం i = 0;
        కొరకు (; i <= len - vlanes16 * 4; i += vlanes16 * 4)
        {
            స్థిరరాశి SrcT* src0 = srcmat.ptr<SrcT>(0) + ప్రారంభం + i;
            VecDst a0, a1, a2, a3, a4, a5, a6, a7;
            v_expand(vx_ఎక్కించు(src0), a0, a1);
            v_expand(vx_ఎక్కించు(src0 + vlanes16), a2, a3);
            v_expand(vx_ఎక్కించు(src0 + vlanes16*2), a4, a5);
            v_expand(vx_ఎక్కించు(src0 + vlanes16*3), a6, a7);
            కొరకు (పూర్ణాంకం అడ్డువరుస = 1; అడ్డువరుస < ఎత్తు; అడ్డువరుస++)
            {
                స్థిరరాశి SrcT* src = srcmat.ptr<SrcT>(అడ్డువరుస) + ప్రారంభం + i;
                VecDst lo, hi;
                v_expand(vx_ఎక్కించు(src), lo, hi);
                a0 = v_కూడు(a0, lo); a1 = v_కూడు(a1, hi);
                v_expand(vx_ఎక్కించు(src + vlanes16), lo, hi);
                a2 = v_కూడు(a2, lo); a3 = v_కూడు(a3, hi);
                v_expand(vx_ఎక్కించు(src + vlanes16*2), lo, hi);
                a4 = v_కూడు(a4, lo); a5 = v_కూడు(a5, hi);
                v_expand(vx_ఎక్కించు(src + vlanes16*3), lo, hi);
                a6 = v_కూడు(a6, lo); a7 = v_కూడు(a7, hi);
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
        కొరకు (; i <= len - vlanes16; i += vlanes16)
        {
            VecDst a0, a1;
            v_expand(vx_ఎక్కించు(srcmat.ptr<SrcT>(0) + ప్రారంభం + i), a0, a1);
            కొరకు (పూర్ణాంకం అడ్డువరుస = 1; అడ్డువరుస < ఎత్తు; అడ్డువరుస++)
            {
                VecDst lo, hi;
                v_expand(vx_ఎక్కించు(srcmat.ptr<SrcT>(అడ్డువరుస) + ప్రారంభం + i), lo, hi);
                a0 = v_కూడు(a0, lo); a1 = v_కూడు(a1, hi);
            }
            v_store(dst + i, v_cvt_f32(v_reinterpret_as_s32(a0)));
            v_store(dst + i + vlanes32, v_cvt_f32(v_reinterpret_as_s32(a1)));
        }
        కొరకు (; i < len; i++)
        {
            పూర్ణాంకం val = (పూర్ణాంకం)*(srcmat.ptr<SrcT>(0) + ప్రారంభం + i);
            కొరకు (పూర్ణాంకం అడ్డువరుస = 1; అడ్డువరుస < ఎత్తు; అడ్డువరుస++)
                val += (పూర్ణాంకం)*(srcmat.ptr<SrcT>(అడ్డువరుస) + ప్రారంభం + i);
            dst[i] = (దశాంశం)val;
        }
    };

    parallel_for_(Range(0, వెడల్పు_cn), body, వెడల్పు_cn * CV_ELEM_కొలత(srcmat.లోతు()) / 64);
    v_cleanup();
}

స్థిర శూన్య reduce_అడ్డువరుస_కూడిక_16u32f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{ reduce_అడ్డువరుస_కూడిక_16_32f<ushort, v_uint16, v_uint32>(srcmat, dstmat); }

స్థిర శూన్య reduce_అడ్డువరుస_కూడిక_16s32f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{ reduce_అడ్డువరుస_కూడిక_16_32f<పొట్టి, v_int16, v_int32>(srcmat, dstmat); }

// --- float/double → same type row reduce ---
మూస<రకం_పేరు T, రకం_పేరు VecT>
స్థిర శూన్య reduce_అడ్డువరుస_కూడిక_FP(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{
    స్థిరరాశి పూర్ణాంకం వెడల్పు_cn = srcmat.నిలువుల_సంఖ్య * srcmat.channels();
    స్థిరరాశి పూర్ణాంకం ఎత్తు = srcmat.అడ్డువరుసలు;
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<VecT>::vlanes();

    స్వయంచాలక body = [&](స్థిరరాశి Range& range) {
        స్థిరరాశి పూర్ణాంకం ప్రారంభం = range.ప్రారంభం;
        స్థిరరాశి పూర్ణాంకం end = range.end;
        స్థిరరాశి పూర్ణాంకం len = end - ప్రారంభం;
        T* dst = dstmat.ptr<T>(0) + ప్రారంభం;

        పూర్ణాంకం i = 0;
        కొరకు (; i <= len - vlanes * 8; i += vlanes * 8)
        {
            స్థిరరాశి T* src0 = srcmat.ptr<T>(0) + ప్రారంభం + i;
            VecT s0 = vx_ఎక్కించు(src0), s1 = vx_ఎక్కించు(src0 + vlanes);
            VecT s2 = vx_ఎక్కించు(src0 + vlanes*2), s3 = vx_ఎక్కించు(src0 + vlanes*3);
            VecT s4 = vx_ఎక్కించు(src0 + vlanes*4), s5 = vx_ఎక్కించు(src0 + vlanes*5);
            VecT s6 = vx_ఎక్కించు(src0 + vlanes*6), s7 = vx_ఎక్కించు(src0 + vlanes*7);
            కొరకు (పూర్ణాంకం అడ్డువరుస = 1; అడ్డువరుస < ఎత్తు; అడ్డువరుస++)
            {
                స్థిరరాశి T* src = srcmat.ptr<T>(అడ్డువరుస) + ప్రారంభం + i;
                s0 = v_కూడు(s0, vx_ఎక్కించు(src));
                s1 = v_కూడు(s1, vx_ఎక్కించు(src + vlanes));
                s2 = v_కూడు(s2, vx_ఎక్కించు(src + vlanes*2));
                s3 = v_కూడు(s3, vx_ఎక్కించు(src + vlanes*3));
                s4 = v_కూడు(s4, vx_ఎక్కించు(src + vlanes*4));
                s5 = v_కూడు(s5, vx_ఎక్కించు(src + vlanes*5));
                s6 = v_కూడు(s6, vx_ఎక్కించు(src + vlanes*6));
                s7 = v_కూడు(s7, vx_ఎక్కించు(src + vlanes*7));
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
        కొరకు (; i <= len - vlanes; i += vlanes)
        {
            VecT s0 = vx_ఎక్కించు(srcmat.ptr<T>(0) + ప్రారంభం + i);
            కొరకు (పూర్ణాంకం అడ్డువరుస = 1; అడ్డువరుస < ఎత్తు; అడ్డువరుస++)
                s0 = v_కూడు(s0, vx_ఎక్కించు(srcmat.ptr<T>(అడ్డువరుస) + ప్రారంభం + i));
            v_store(dst + i, s0);
        }
        కొరకు (; i < len; i++)
        {
            T val = *(srcmat.ptr<T>(0) + ప్రారంభం + i);
            కొరకు (పూర్ణాంకం అడ్డువరుస = 1; అడ్డువరుస < ఎత్తు; అడ్డువరుస++)
                val += *(srcmat.ptr<T>(అడ్డువరుస) + ప్రారంభం + i);
            dst[i] = val;
        }
    };

    parallel_for_(Range(0, వెడల్పు_cn), body, వెడల్పు_cn * CV_ELEM_కొలత(srcmat.లోతు()) / 64);
    v_cleanup();
}

స్థిర శూన్య reduce_అడ్డువరుస_కూడిక_32f32f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{ reduce_అడ్డువరుస_కూడిక_FP<దశాంశం, v_float32>(srcmat, dstmat); }

#ఒకవేళ CV_SIMD_64F
// --- float → double ---
స్థిర శూన్య reduce_అడ్డువరుస_కూడిక_32f64f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{
    స్థిరరాశి పూర్ణాంకం వెడల్పు_cn = srcmat.నిలువుల_సంఖ్య * srcmat.channels();
    స్థిరరాశి పూర్ణాంకం ఎత్తు = srcmat.అడ్డువరుసలు;
    స్థిరరాశి పూర్ణాంకం vlanes32 = VTraits<v_float32>::vlanes();
    స్థిరరాశి పూర్ణాంకం vlanes64 = VTraits<v_float64>::vlanes();

    స్వయంచాలక body = [&](స్థిరరాశి Range& range) {
        స్థిరరాశి పూర్ణాంకం ప్రారంభం = range.ప్రారంభం;
        స్థిరరాశి పూర్ణాంకం end = range.end;
        స్థిరరాశి పూర్ణాంకం len = end - ప్రారంభం;

        Auto_బఫరు<ద్విగుణ> buf_storage(len);
        ద్విగుణ* buf = buf_storage.దత్తాంశం();

        స్థిరరాశి దశాంశం* src0 = srcmat.ptr<దశాంశం>(0) + ప్రారంభం;
        కొరకు (పూర్ణాంకం i = 0; i < len; i++)
            buf[i] = (ద్విగుణ)src0[i];

        కొరకు (పూర్ణాంకం అడ్డువరుస = 1; అడ్డువరుస < ఎత్తు; అడ్డువరుస++)
        {
            స్థిరరాశి దశాంశం* src = srcmat.ptr<దశాంశం>(అడ్డువరుస) + ప్రారంభం;
            పూర్ణాంకం i = 0;
            కొరకు (; i <= len - vlanes32; i += vlanes32)
            {
                v_float32 v = vx_ఎక్కించు(src + i);
                v_store(buf + i, v_కూడు(vx_ఎక్కించు(buf + i), v_cvt_f64(v)));
                v_store(buf + i + vlanes64, v_కూడు(vx_ఎక్కించు(buf + i + vlanes64), v_cvt_f64_ఎగువ(v)));
            }
            కొరకు (; i < len; i++)
                buf[i] += (ద్విగుణ)src[i];
        }

        ద్విగుణ* dst = dstmat.ptr<ద్విగుణ>(0) + ప్రారంభం;
        జ్ఞాపకం_నకలు(dst, buf, len * పరిమాణం(ద్విగుణ));
    };

    parallel_for_(Range(0, వెడల్పు_cn), body, వెడల్పు_cn * CV_ELEM_కొలత(srcmat.లోతు()) / 64);
    v_cleanup();
}

స్థిర శూన్య reduce_అడ్డువరుస_కూడిక_64f64f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{ reduce_అడ్డువరుస_కూడిక_FP<ద్విగుణ, v_float64>(srcmat, dstmat); }
#అయితే_ముగింపు // CV_SIMD_64F

#అయితే_ముగింపు // CV_SIMD || CV_SIMD_SCALABLE

#ఒకవేళ CV_SIMD_SCALABLE
స్థిర అంతఃవరుస v_float32 reduce_అడ్డువరుస_Sum2_Acc4(స్థిరరాశి v_float32& acc,
                                           స్థిరరాశి దశాంశం* src0, స్థిరరాశి దశాంశం* src1,
                                           స్థిరరాశి దశాంశం* src2, స్థిరరాశి దశాంశం* src3)
{
    v_float32 కూడిక = acc;
    v_float32 x = vx_ఎక్కించు(src0);
    కూడిక = v_fma(x, x, కూడిక);
    x = vx_ఎక్కించు(src1);
    కూడిక = v_fma(x, x, కూడిక);
    x = vx_ఎక్కించు(src2);
    కూడిక = v_fma(x, x, కూడిక);
    x = vx_ఎక్కించు(src3);
    తిరిగి v_fma(x, x, కూడిక);
}

// dim=0, REDUCE_SUM2 for CV_32F using scalable universal intrinsics.
స్థిర శూన్య reduce_అడ్డువరుస_Sum2_32f32f(స్థిరరాశి Mat& srcmat, Mat& dstmat)
{
    స్థిరరాశి పూర్ణాంకం వెడల్పు = srcmat.నిలువుల_సంఖ్య * srcmat.channels();
    స్థిరరాశి పూర్ణాంకం ఎత్తు = srcmat.అడ్డువరుసలు;
    స్థిరరాశి పూర్ణాంకం vlanes = VTraits<v_float32>::vlanes();
    స్థిరరాశి పూర్ణాంకం ఖండం_2 = vlanes * 4;
    Auto_బఫరు<దశాంశం> బఫరు(వెడల్పు);
    దశాంశం* buf = బఫరు.దత్తాంశం();

    స్థిరరాశి దశాంశం* src = srcmat.ptr<దశాంశం>(0);
    పూర్ణాంకం i = 0;
    కొరకు (; i <= వెడల్పు - ఖండం_2; i += ఖండం_2)
    {
        v_float32 x0 = vx_ఎక్కించు(src + i);
        v_float32 x1 = vx_ఎక్కించు(src + i + vlanes);
        v_float32 x2 = vx_ఎక్కించు(src + i + vlanes * 2);
        v_float32 x3 = vx_ఎక్కించు(src + i + vlanes * 3);
        v_store(buf + i, v_mul(x0, x0));
        v_store(buf + i + vlanes, v_mul(x1, x1));
        v_store(buf + i + vlanes * 2, v_mul(x2, x2));
        v_store(buf + i + vlanes * 3, v_mul(x3, x3));
    }
    కొరకు (; i <= వెడల్పు - vlanes; i += vlanes)
    {
        v_float32 x = vx_ఎక్కించు(src + i);
        v_store(buf + i, v_mul(x, x));
    }
    కొరకు (; i < వెడల్పు; i++)
        buf[i] = src[i] * src[i];

    పూర్ణాంకం అడ్డువరుస = 1;
    // Four rows per pass amortize buffer traffic while preserving source-row
    // accumulation order. The outer loop stays serial: splitting width into
    // tiny parallel stripes under-fills scalable vectors and adds scheduling
    // overhead for this memory-streaming kernel.
    కొరకు (; అడ్డువరుస <= ఎత్తు - 4; అడ్డువరుస += 4)
    {
        స్థిరరాశి దశాంశం* src0 = srcmat.ptr<దశాంశం>(అడ్డువరుస);
        స్థిరరాశి దశాంశం* src1 = srcmat.ptr<దశాంశం>(అడ్డువరుస + 1);
        స్థిరరాశి దశాంశం* src2 = srcmat.ptr<దశాంశం>(అడ్డువరుస + 2);
        స్థిరరాశి దశాంశం* src3 = srcmat.ptr<దశాంశం>(అడ్డువరుస + 3);

        i = 0;
        కొరకు (; i <= వెడల్పు - ఖండం_2; i += ఖండం_2)
        {
            v_float32 sum0 = vx_ఎక్కించు(buf + i);
            v_float32 sum1 = vx_ఎక్కించు(buf + i + vlanes);
            v_float32 sum2 = vx_ఎక్కించు(buf + i + vlanes * 2);
            v_float32 sum3 = vx_ఎక్కించు(buf + i + vlanes * 3);
            sum0 = reduce_అడ్డువరుస_Sum2_Acc4(sum0, src0 + i, src1 + i, src2 + i, src3 + i);
            sum1 = reduce_అడ్డువరుస_Sum2_Acc4(sum1, src0 + i + vlanes, src1 + i + vlanes,
                                     src2 + i + vlanes, src3 + i + vlanes);
            sum2 = reduce_అడ్డువరుస_Sum2_Acc4(sum2, src0 + i + vlanes * 2, src1 + i + vlanes * 2,
                                     src2 + i + vlanes * 2, src3 + i + vlanes * 2);
            sum3 = reduce_అడ్డువరుస_Sum2_Acc4(sum3, src0 + i + vlanes * 3, src1 + i + vlanes * 3,
                                     src2 + i + vlanes * 3, src3 + i + vlanes * 3);
            v_store(buf + i, sum0);
            v_store(buf + i + vlanes, sum1);
            v_store(buf + i + vlanes * 2, sum2);
            v_store(buf + i + vlanes * 3, sum3);
        }
        కొరకు (; i <= వెడల్పు - vlanes; i += vlanes)
        {
            v_float32 కూడిక = vx_ఎక్కించు(buf + i);
            కూడిక = reduce_అడ్డువరుస_Sum2_Acc4(కూడిక, src0 + i, src1 + i, src2 + i, src3 + i);
            v_store(buf + i, కూడిక);
        }
        కొరకు (; i < వెడల్పు; i++)
        {
            దశాంశం కూడిక = buf[i];
            దశాంశం x = src0[i]; కూడిక += x * x;
            x = src1[i]; కూడిక += x * x;
            x = src2[i]; కూడిక += x * x;
            x = src3[i]; కూడిక += x * x;
            buf[i] = కూడిక;
        }
    }

    కొరకు (; అడ్డువరుస < ఎత్తు; అడ్డువరుస++)
    {
        src = srcmat.ptr<దశాంశం>(అడ్డువరుస);
        i = 0;
        కొరకు (; i <= వెడల్పు - ఖండం_2; i += ఖండం_2)
        {
            v_float32 sum0 = vx_ఎక్కించు(buf + i);
            v_float32 sum1 = vx_ఎక్కించు(buf + i + vlanes);
            v_float32 sum2 = vx_ఎక్కించు(buf + i + vlanes * 2);
            v_float32 sum3 = vx_ఎక్కించు(buf + i + vlanes * 3);
            v_float32 x0 = vx_ఎక్కించు(src + i);
            v_float32 x1 = vx_ఎక్కించు(src + i + vlanes);
            v_float32 x2 = vx_ఎక్కించు(src + i + vlanes * 2);
            v_float32 x3 = vx_ఎక్కించు(src + i + vlanes * 3);
            v_store(buf + i, v_fma(x0, x0, sum0));
            v_store(buf + i + vlanes, v_fma(x1, x1, sum1));
            v_store(buf + i + vlanes * 2, v_fma(x2, x2, sum2));
            v_store(buf + i + vlanes * 3, v_fma(x3, x3, sum3));
        }
        కొరకు (; i <= వెడల్పు - vlanes; i += vlanes)
        {
            v_float32 x = vx_ఎక్కించు(src + i);
            v_store(buf + i, v_fma(x, x, vx_ఎక్కించు(buf + i)));
        }
        కొరకు (; i < వెడల్పు; i++)
            buf[i] += src[i] * src[i];
    }

    // Delay output stores until all source rows are consumed, preserving the
    // existing overlap-safe behavior of the proposed optimized path.
    జ్ఞాపకం_నకలు(dstmat.ptr<దశాంశం>(), buf, వెడల్పు * పరిమాణం(దశాంశం));
    v_cleanup();
}
#అయితే_ముగింపు

// =====================================================================
//  Dispatchers
// =====================================================================

Reduce_కూడిక_Func getReduceCSumFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth)
{
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ (sdepth == CV_8U && ddepth == CV_32S) తిరిగి reduce_నిలువు_కూడిక_8u32s;
    ఒకవేళ (sdepth == CV_8U && ddepth == CV_32F) తిరిగి reduce_నిలువు_కూడిక_8u32f;
    ఒకవేళ (sdepth == CV_16U && ddepth == CV_32F) తిరిగి reduce_నిలువు_కూడిక_16u32f;
    ఒకవేళ (sdepth == CV_16S && ddepth == CV_32F) తిరిగి reduce_నిలువు_కూడిక_16s32f;
    ఒకవేళ (sdepth == CV_32F && ddepth == CV_32F) తిరిగి reduce_నిలువు_కూడిక_32f32f;
#ఒకవేళ CV_SIMD_64F
    ఒకవేళ (sdepth == CV_32F && ddepth == CV_64F) తిరిగి reduce_నిలువు_కూడిక_32f64f;
    ఒకవేళ (sdepth == CV_64F && ddepth == CV_64F) తిరిగి reduce_నిలువు_కూడిక_64f64f;
#అయితే_ముగింపు
#లేకపోతే
    CV_వాడని(sdepth);
    CV_వాడని(ddepth);
#అయితే_ముగింపు
    తిరిగి శూన్యసూచిక;
}

Reduce_కూడిక_Func getReduceRSumFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth)
{
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
    ఒకవేళ (sdepth == CV_8U && ddepth == CV_32S) తిరిగి reduce_అడ్డువరుస_కూడిక_8u32s;
    ఒకవేళ (sdepth == CV_8U && ddepth == CV_32F) తిరిగి reduce_అడ్డువరుస_కూడిక_8u32f;
    ఒకవేళ (sdepth == CV_16U && ddepth == CV_32F) తిరిగి reduce_అడ్డువరుస_కూడిక_16u32f;
    ఒకవేళ (sdepth == CV_16S && ddepth == CV_32F) తిరిగి reduce_అడ్డువరుస_కూడిక_16s32f;
    ఒకవేళ (sdepth == CV_32F && ddepth == CV_32F) తిరిగి reduce_అడ్డువరుస_కూడిక_32f32f;
#ఒకవేళ CV_SIMD_64F
    ఒకవేళ (sdepth == CV_32F && ddepth == CV_64F) తిరిగి reduce_అడ్డువరుస_కూడిక_32f64f;
    ఒకవేళ (sdepth == CV_64F && ddepth == CV_64F) తిరిగి reduce_అడ్డువరుస_కూడిక_64f64f;
#అయితే_ముగింపు
#లేకపోతే
    CV_వాడని(sdepth);
    CV_వాడని(ddepth);
#అయితే_ముగింపు
    తిరిగి శూన్యసూచిక;
}

Reduce_కూడిక_Func getReduceRSum2Func(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth)
{
#ఒకవేళ CV_SIMD_SCALABLE
    ఒకవేళ (sdepth == CV_32F && ddepth == CV_32F) తిరిగి reduce_అడ్డువరుస_Sum2_32f32f;
#లేకపోతే
    CV_వాడని(sdepth);
    CV_వాడని(ddepth);
#అయితే_ముగింపు
    తిరిగి శూన్యసూచిక;
}

#అయితే_ముగింపు // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_చివర
} // namespace cv
