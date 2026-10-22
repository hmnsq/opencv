// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"

نطاق cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

تعريفنوع فراغ (*Reduce_جمع_Func)(ثابت Mat& src, Mat& dst);
Reduce_جمع_Func getReduceCSumFunc(صحيح sdepth, صحيح ddepth);
Reduce_جمع_Func getReduceRSumFunc(صحيح sdepth, صحيح ddepth);
Reduce_جمع_Func getReduceRSum2Func(صحيح sdepth, صحيح ddepth);

#اگر_تعریف_نشده CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// =====================================================================
//  Col reduce SUM (dim=1): sum each row into cn output values
// =====================================================================

#إذا (CV_SIMD || CV_SIMD_SCALABLE)

// --- uchar → int ---
// Uses u16 intermediate accumulator to halve the number of widen operations:
// u8→u16 per iteration, u16→u32 flush every 128 iterations (max u16 value: 128*255=32640 < 65535)
ساكن فراغ reduce_ستون_جمع_8u32s(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح cn = srcmat.channels();
    ثابت صحيح تعداد_ستون = srcmat.تعداد_ستون;
    ثابت صحيح عرض = تعداد_ستون * cn;

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
                uint32x4_t v_جمع = vdupq_n_u32(0);
                صحيح x = 0;
                لكل (; x <= عرض - 16; x += 16)
                    v_جمع = vdotq_u32(v_جمع, vld1q_u8(src + x), ones);
                صحيح مجموع = (صحيح)vaddvq_u32(v_جمع);
                لكل (; x < عرض; x++)
                    مجموع += (صحيح)src[x];
                dst[0] = مجموع;
#وإلاإذا CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                // AArch64 fallback: vpaddlq chain
                uint32x4_t v_جمع = vdupq_n_u32(0);
                صحيح x = 0;
                لكل (; x <= عرض - 16; x += 16)
                    v_جمع = vaddq_u32(v_جمع, vpaddlq_u16(vpaddlq_u8(vld1q_u8(src + x))));
                صحيح مجموع = (صحيح)vaddvq_u32(v_جمع);
                لكل (; x < عرض; x++)
                    مجموع += (صحيح)src[x];
                dst[0] = مجموع;
#وإلاإذا CV_AVX2
                // Intel AVX2: _mm256_sad_epu8 — 32 bytes → 4×u64 sum, 1 cycle
                ثابت __m256i zero = _mm256_setzero_si256();
                __m256i vsum = zero;
                صحيح x = 0;
                لكل (; x <= عرض - 32; x += 32)
                    vsum = mm256_جمع_کن_epi64(vsum, _mm256_sad_epu8(
                        _mm256_loadu_si256((ثابت __m256i*)(src + x)), zero));
                // 8-byte tail: reduce max scalar tail from 31 to 7
                // Use _mm256_inserti128_si256 instead of _mm256_castsi128_si256:
                // the latter leaves upper 128 bits undefined per Intel spec,
                // causing MSVC to expose stale register data to _mm256_sad_epu8.
                لكل (; x <= عرض - 8; x += 8)
                    vsum = mm256_جمع_کن_epi64(vsum, _mm256_sad_epu8(
                        _mm256_inserti128_si256(_mm256_setzero_si256(), _mm_loadl_epi64((ثابت __m128i*)(src + x)), 0), zero));
                __m128i lo128 = _mm256_castsi256_si128(vsum);
                __m128i hi128 = _mm256_extracti128_si256(vsum, 1);
                __m128i s = mm_جمع_کن_epi64(lo128, hi128);
                s = mm_جمع_کن_epi64(s, _mm_unpackhi_epi64(s, s));
                صحيح مجموع = _mm_cvtsi128_si32(s);
                لكل (; x < عرض; x++)
                    مجموع += (صحيح)src[x];
                dst[0] = مجموع;
#وإلاإذا CV_SSE2
                // Intel SSE2: _mm_sad_epu8 — 16 bytes → 2×u64 sum
                ثابت __m128i zero = _mm_setzero_si128();
                __m128i vsum = zero;
                صحيح x = 0;
                لكل (; x <= عرض - 16; x += 16)
                    vsum = mm_جمع_کن_epi64(vsum, _mm_sad_epu8(
                        _mm_loadu_si128((ثابت __m128i*)(src + x)), zero));
                __m128i s = mm_جمع_کن_epi64(vsum, _mm_unpackhi_epi64(vsum, vsum));
                صحيح مجموع = _mm_cvtsi128_si32(s);
                لكل (; x < عرض; x++)
                    مجموع += (صحيح)src[x];
                dst[0] = مجموع;
#وإلا
                ثابت صحيح vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 v_جمع = vx_setzero_u32();
                v_uint16 v_sum16 = vx_setzero_u16();
                صحيح x = 0, batch = 0;
                ثابت صحيح flush_at = 128;

                لكل (; x <= عرض - vlanes8; x += vlanes8)
                {
                    v_uint16 lo, hi;
                    v_expand(vx_بارگیری(src + x), lo, hi);
                    v_sum16 = v_جمع_کن(v_sum16, v_جمع_کن(lo, hi));

                    إذا (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(v_sum16, a, b);
                        v_جمع = v_جمع_کن(v_جمع, v_جمع_کن(a, b));
                        v_sum16 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(v_sum16, a, b);
                    v_جمع = v_جمع_کن(v_جمع, v_جمع_کن(a, b));
                }

                صحيح مجموع = (صحيح)v_reduce_جمع(v_جمع);
                لكل (; x < عرض; x++)
                    مجموع += (صحيح)src[x];
                dst[0] = مجموع;
#پایان_اگر
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
                لكل (; x <= تعداد_ستون - 16; x += 16)
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
                لكل (; x < تعداد_ستون; x++)
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
                لكل (; x <= تعداد_ستون - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    sums[0] += (صحيح)vaddlvq_u8(v.val[0]);
                    sums[1] += (صحيح)vaddlvq_u8(v.val[1]);
                    sums[2] += (صحيح)vaddlvq_u8(v.val[2]);
                    sums[3] += (صحيح)vaddlvq_u8(v.val[3]);
                }
                dst[0] = sums[0]; dst[1] = sums[1];
                dst[2] = sums[2]; dst[3] = sums[3];
                لكل (; x < تعداد_ستون; x++)
                {
                    dst[0] += (صحيح)src[x * 4];
                    dst[1] += (صحيح)src[x * 4 + 1];
                    dst[2] += (صحيح)src[x * 4 + 2];
                    dst[3] += (صحيح)src[x * 4 + 3];
                }
#وإلاإذا CV_AVX2
                // Intel AVX2: shuffle + unpack to group channels, full-register SAD
                ثابت __m256i zero = _mm256_setzero_si256();
                ثابت __m256i shuf_ماسک = _mm256_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                صحيح x = 0;
                // 16 pixels/iter with unpack (full-register SAD)
                {
                    __m256i جمع_bg = zero, جمع_ra = zero;
                    لكل (; x <= تعداد_ستون - 16; x += 16)
                    {
                        __m256i v0 = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4));
                        __m256i v1 = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4 + 32));
                        __m256i g0 = _mm256_shuffle_epi8(v0, shuf_ماسک);
                        __m256i g1 = _mm256_shuffle_epi8(v1, shuf_ماسک);
                        __m256i bg = _mm256_unpacklo_epi32(g0, g1);
                        __m256i ra = _mm256_unpackhi_epi32(g0, g1);
                        جمع_bg = mm256_جمع_کن_epi64(جمع_bg, _mm256_sad_epu8(bg, zero));
                        جمع_ra = mm256_جمع_کن_epi64(جمع_ra, _mm256_sad_epu8(ra, zero));
                    }
                    __m128i bg_lo = _mm256_castsi256_si128(جمع_bg);
                    __m128i bg_hi = _mm256_extracti128_si256(جمع_bg, 1);
                    __m128i bg_s = mm_جمع_کن_epi64(bg_lo, bg_hi);
                    __m128i ra_lo = _mm256_castsi256_si128(جمع_ra);
                    __m128i ra_hi = _mm256_extracti128_si256(جمع_ra, 1);
                    __m128i ra_s = mm_جمع_کن_epi64(ra_lo, ra_hi);
                    dst[0] = _mm_cvtsi128_si32(bg_s);                     // B
                    dst[1] = _mm_cvtsi128_si32(_mm_srli_si128(bg_s, 8));  // G
                    dst[2] = _mm_cvtsi128_si32(ra_s);                     // R
                    dst[3] = _mm_cvtsi128_si32(_mm_srli_si128(ra_s, 8));  // A
                }
                // 8-pixel tail with and/srli
                {
                    ثابت __m256i ماسک_lo32 = _mm256_set1_epi64x(0x00000000FFFFFFFFLL);
                    __m256i جمع_br = zero, جمع_ga = zero;
                    لكل (; x <= تعداد_ستون - 8; x += 8)
                    {
                        __m256i v = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4));
                        __m256i grouped = _mm256_shuffle_epi8(v, shuf_ماسک);
                        __m256i br = _mm256_and_si256(grouped, ماسک_lo32);
                        __m256i ga = _mm256_srli_epi64(grouped, 32);
                        جمع_br = mm256_جمع_کن_epi64(جمع_br, _mm256_sad_epu8(br, zero));
                        جمع_ga = mm256_جمع_کن_epi64(جمع_ga, _mm256_sad_epu8(ga, zero));
                    }
                    __m128i br_lo = _mm256_castsi256_si128(جمع_br);
                    __m128i br_hi = _mm256_extracti128_si256(جمع_br, 1);
                    __m128i br_s = mm_جمع_کن_epi64(br_lo, br_hi);
                    __m128i ga_lo = _mm256_castsi256_si128(جمع_ga);
                    __m128i ga_hi = _mm256_extracti128_si256(جمع_ga, 1);
                    __m128i ga_s = mm_جمع_کن_epi64(ga_lo, ga_hi);
                    dst[0] += _mm_cvtsi128_si32(br_s);
                    dst[1] += _mm_cvtsi128_si32(ga_s);
                    dst[2] += _mm_cvtsi128_si32(_mm_srli_si128(br_s, 8));
                    dst[3] += _mm_cvtsi128_si32(_mm_srli_si128(ga_s, 8));
                }
                لكل (; x < تعداد_ستون; x++)
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
                ثابت __m128i shuf_ماسک = _mm_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                ثابت __m128i ماسک_lo32 = _mm_set1_epi64x(0x00000000FFFFFFFFLL);
                __m128i جمع_br = zero, جمع_ga = zero;
                صحيح x = 0;
                لكل (; x <= تعداد_ستون - 4; x += 4)
                {
                    __m128i v = _mm_loadu_si128((ثابت __m128i*)(src + x * 4));
                    __m128i grouped = _mm_shuffle_epi8(v, shuf_ماسک);
                    __m128i br = _mm_and_si128(grouped, ماسک_lo32);
                    __m128i ga = _mm_srli_epi64(grouped, 32);
                    جمع_br = mm_جمع_کن_epi64(جمع_br, _mm_sad_epu8(br, zero));
                    جمع_ga = mm_جمع_کن_epi64(جمع_ga, _mm_sad_epu8(ga, zero));
                }
                dst[0] = _mm_cvtsi128_si32(جمع_br);
                dst[1] = _mm_cvtsi128_si32(جمع_ga);
                dst[2] = _mm_cvtsi128_si32(_mm_srli_si128(جمع_br, 8));
                dst[3] = _mm_cvtsi128_si32(_mm_srli_si128(جمع_ga, 8));
                لكل (; x < تعداد_ستون; x++)
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

                لكل (; x <= تعداد_ستون - vlanes8; x += vlanes8)
                {
                    v_uint8 ch0, ch1, ch2, ch3;
                    v_بارگیری_deinterleave(src + x * 4, ch0, ch1, ch2, ch3);

                    v_uint16 lo, hi;
                    v_expand(ch0, lo, hi); vs16_0 = v_جمع_کن(vs16_0, v_جمع_کن(lo, hi));
                    v_expand(ch1, lo, hi); vs16_1 = v_جمع_کن(vs16_1, v_جمع_کن(lo, hi));
                    v_expand(ch2, lo, hi); vs16_2 = v_جمع_کن(vs16_2, v_جمع_کن(lo, hi));
                    v_expand(ch3, lo, hi); vs16_3 = v_جمع_کن(vs16_3, v_جمع_کن(lo, hi));

                    إذا (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(vs16_0, a, b); vsum0 = v_جمع_کن(vsum0, v_جمع_کن(a, b));
                        v_expand(vs16_1, a, b); vsum1 = v_جمع_کن(vsum1, v_جمع_کن(a, b));
                        v_expand(vs16_2, a, b); vsum2 = v_جمع_کن(vsum2, v_جمع_کن(a, b));
                        v_expand(vs16_3, a, b); vsum3 = v_جمع_کن(vsum3, v_جمع_کن(a, b));
                        vs16_0 = vx_setzero_u16(); vs16_1 = vx_setzero_u16();
                        vs16_2 = vx_setzero_u16(); vs16_3 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(vs16_0, a, b); vsum0 = v_جمع_کن(vsum0, v_جمع_کن(a, b));
                    v_expand(vs16_1, a, b); vsum1 = v_جمع_کن(vsum1, v_جمع_کن(a, b));
                    v_expand(vs16_2, a, b); vsum2 = v_جمع_کن(vsum2, v_جمع_کن(a, b));
                    v_expand(vs16_3, a, b); vsum3 = v_جمع_کن(vsum3, v_جمع_کن(a, b));
                }

                dst[0] = (صحيح)v_reduce_جمع(vsum0);
                dst[1] = (صحيح)v_reduce_جمع(vsum1);
                dst[2] = (صحيح)v_reduce_جمع(vsum2);
                dst[3] = (صحيح)v_reduce_جمع(vsum3);

                لكل (; x < تعداد_ستون; x++)
                {
                    dst[0] += (صحيح)src[x * 4];
                    dst[1] += (صحيح)src[x * 4 + 1];
                    dst[2] += (صحيح)src[x * 4 + 2];
                    dst[3] += (صحيح)src[x * 4 + 3];
                }
#پایان_اگر
            }
            وإلا
            {
                // generic cn: scalar fallback
                لكل (صحيح c = 0; c < cn; c++)
                    dst[c] = (صحيح)src[c];
                لكل (صحيح x = cn; x < عرض; x += cn)
                    لكل (صحيح c = 0; c < cn; c++)
                        dst[c] += (صحيح)src[x + c];
            }
        }
    };

    parallel_for_(Range(0, srcmat.ردیفها), body);
    v_cleanup();
}

// --- uchar → float ---
ساكن فراغ reduce_ستون_جمع_8u32f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح cn = srcmat.channels();
    ثابت صحيح تعداد_ستون = srcmat.تعداد_ستون;
    ثابت صحيح عرض = تعداد_ستون * cn;

    خودکار body = [&](ثابت Range& range) {
        لكل (صحيح y = range.شروع; y < range.end; y++)
        {
            ثابت uchar* src = srcmat.ptr<uchar>(y);
            عائم* dst = dstmat.ptr<عائم>(y);

            // compute in int, then convert to float
            Auto_میانگیر<صحيح> ibuf(cn);
            صحيح* sums = ibuf.داده();
            لكل (صحيح c = 0; c < cn; c++)
                sums[c] = 0;

            إذا (cn == 1)
            {
#إذا defined(CV_NEON_DOT)
                ثابت uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t v_جمع = vdupq_n_u32(0);
                صحيح x = 0;
                لكل (; x <= عرض - 16; x += 16)
                    v_جمع = vdotq_u32(v_جمع, vld1q_u8(src + x), ones);
                sums[0] = (صحيح)vaddvq_u32(v_جمع);
                لكل (; x < عرض; x++)
                    sums[0] += (صحيح)src[x];
#وإلاإذا CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                uint32x4_t v_جمع = vdupq_n_u32(0);
                صحيح x = 0;
                لكل (; x <= عرض - 16; x += 16)
                    v_جمع = vaddq_u32(v_جمع, vpaddlq_u16(vpaddlq_u8(vld1q_u8(src + x))));
                sums[0] = (صحيح)vaddvq_u32(v_جمع);
                لكل (; x < عرض; x++)
                    sums[0] += (صحيح)src[x];
#وإلاإذا CV_AVX2
                ثابت __m256i zero = _mm256_setzero_si256();
                __m256i vsum = zero;
                صحيح x = 0;
                لكل (; x <= عرض - 32; x += 32)
                    vsum = mm256_جمع_کن_epi64(vsum, _mm256_sad_epu8(
                        _mm256_loadu_si256((ثابت __m256i*)(src + x)), zero));
                // 8-byte tail: reduce max scalar tail from 31 to 7
                // Use _mm256_inserti128_si256 instead of _mm256_castsi128_si256:
                // the latter leaves upper 128 bits undefined per Intel spec,
                // causing MSVC to expose stale register data to _mm256_sad_epu8.
                لكل (; x <= عرض - 8; x += 8)
                    vsum = mm256_جمع_کن_epi64(vsum, _mm256_sad_epu8(
                        _mm256_inserti128_si256(_mm256_setzero_si256(), _mm_loadl_epi64((ثابت __m128i*)(src + x)), 0), zero));
                __m128i lo128 = _mm256_castsi256_si128(vsum);
                __m128i hi128 = _mm256_extracti128_si256(vsum, 1);
                __m128i s = mm_جمع_کن_epi64(lo128, hi128);
                s = mm_جمع_کن_epi64(s, _mm_unpackhi_epi64(s, s));
                sums[0] = _mm_cvtsi128_si32(s);
                لكل (; x < عرض; x++)
                    sums[0] += (صحيح)src[x];
#وإلاإذا CV_SSE2
                ثابت __m128i zero = _mm_setzero_si128();
                __m128i vsum = zero;
                صحيح x = 0;
                لكل (; x <= عرض - 16; x += 16)
                    vsum = mm_جمع_کن_epi64(vsum, _mm_sad_epu8(
                        _mm_loadu_si128((ثابت __m128i*)(src + x)), zero));
                __m128i s = mm_جمع_کن_epi64(vsum, _mm_unpackhi_epi64(vsum, vsum));
                sums[0] = _mm_cvtsi128_si32(s);
                لكل (; x < عرض; x++)
                    sums[0] += (صحيح)src[x];
#وإلا
                ثابت صحيح vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 v_جمع = vx_setzero_u32();
                v_uint16 v_sum16 = vx_setzero_u16();
                صحيح x = 0, batch = 0;

                لكل (; x <= عرض - vlanes8; x += vlanes8)
                {
                    v_uint16 lo, hi;
                    v_expand(vx_بارگیری(src + x), lo, hi);
                    v_sum16 = v_جمع_کن(v_sum16, v_جمع_کن(lo, hi));
                    إذا (++batch >= 128)
                    {
                        v_uint32 a, b;
                        v_expand(v_sum16, a, b);
                        v_جمع = v_جمع_کن(v_جمع, v_جمع_کن(a, b));
                        v_sum16 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                v_uint32 a, b;
                v_expand(v_sum16, a, b);
                v_جمع = v_جمع_کن(v_جمع, v_جمع_کن(a, b));

                sums[0] = (صحيح)v_reduce_جمع(v_جمع);
                لكل (; x < عرض; x++)
                    sums[0] += (صحيح)src[x];
#پایان_اگر
            }
            وإلا إذا (cn == 4)
            {
#إذا defined(CV_NEON_DOT)
                ثابت uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t acc0 = vdupq_n_u32(0), acc1 = vdupq_n_u32(0);
                uint32x4_t acc2 = vdupq_n_u32(0), acc3 = vdupq_n_u32(0);
                صحيح x = 0;
                لكل (; x <= تعداد_ستون - 16; x += 16)
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
                لكل (; x < تعداد_ستون; x++)
                {
                    sums[0] += (صحيح)src[x * 4];
                    sums[1] += (صحيح)src[x * 4 + 1];
                    sums[2] += (صحيح)src[x * 4 + 2];
                    sums[3] += (صحيح)src[x * 4 + 3];
                }
#وإلاإذا CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                صحيح x = 0;
                لكل (; x <= تعداد_ستون - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    sums[0] += (صحيح)vaddlvq_u8(v.val[0]);
                    sums[1] += (صحيح)vaddlvq_u8(v.val[1]);
                    sums[2] += (صحيح)vaddlvq_u8(v.val[2]);
                    sums[3] += (صحيح)vaddlvq_u8(v.val[3]);
                }
                لكل (; x < تعداد_ستون; x++)
                {
                    sums[0] += (صحيح)src[x * 4];
                    sums[1] += (صحيح)src[x * 4 + 1];
                    sums[2] += (صحيح)src[x * 4 + 2];
                    sums[3] += (صحيح)src[x * 4 + 3];
                }
#وإلاإذا CV_AVX2
                // Intel AVX2: shuffle + unpack to group channels, full-register SAD
                ثابت __m256i zero = _mm256_setzero_si256();
                ثابت __m256i shuf_ماسک = _mm256_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                صحيح x = 0;
                // 16 pixels/iter with unpack (full-register SAD)
                {
                    __m256i جمع_bg = zero, جمع_ra = zero;
                    لكل (; x <= تعداد_ستون - 16; x += 16)
                    {
                        __m256i v0 = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4));
                        __m256i v1 = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4 + 32));
                        __m256i g0 = _mm256_shuffle_epi8(v0, shuf_ماسک);
                        __m256i g1 = _mm256_shuffle_epi8(v1, shuf_ماسک);
                        __m256i bg = _mm256_unpacklo_epi32(g0, g1);
                        __m256i ra = _mm256_unpackhi_epi32(g0, g1);
                        جمع_bg = mm256_جمع_کن_epi64(جمع_bg, _mm256_sad_epu8(bg, zero));
                        جمع_ra = mm256_جمع_کن_epi64(جمع_ra, _mm256_sad_epu8(ra, zero));
                    }
                    __m128i bg_lo = _mm256_castsi256_si128(جمع_bg);
                    __m128i bg_hi = _mm256_extracti128_si256(جمع_bg, 1);
                    __m128i bg_s = mm_جمع_کن_epi64(bg_lo, bg_hi);
                    __m128i ra_lo = _mm256_castsi256_si128(جمع_ra);
                    __m128i ra_hi = _mm256_extracti128_si256(جمع_ra, 1);
                    __m128i ra_s = mm_جمع_کن_epi64(ra_lo, ra_hi);
                    sums[0] = _mm_cvtsi128_si32(bg_s);                     // B
                    sums[1] = _mm_cvtsi128_si32(_mm_srli_si128(bg_s, 8));  // G
                    sums[2] = _mm_cvtsi128_si32(ra_s);                     // R
                    sums[3] = _mm_cvtsi128_si32(_mm_srli_si128(ra_s, 8));  // A
                }
                // 8-pixel tail with and/srli
                {
                    ثابت __m256i ماسک_lo32 = _mm256_set1_epi64x(0x00000000FFFFFFFFLL);
                    __m256i جمع_br = zero, جمع_ga = zero;
                    لكل (; x <= تعداد_ستون - 8; x += 8)
                    {
                        __m256i v = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4));
                        __m256i grouped = _mm256_shuffle_epi8(v, shuf_ماسک);
                        __m256i br = _mm256_and_si256(grouped, ماسک_lo32);
                        __m256i ga = _mm256_srli_epi64(grouped, 32);
                        جمع_br = mm256_جمع_کن_epi64(جمع_br, _mm256_sad_epu8(br, zero));
                        جمع_ga = mm256_جمع_کن_epi64(جمع_ga, _mm256_sad_epu8(ga, zero));
                    }
                    __m128i br_lo = _mm256_castsi256_si128(جمع_br);
                    __m128i br_hi = _mm256_extracti128_si256(جمع_br, 1);
                    __m128i br_s = mm_جمع_کن_epi64(br_lo, br_hi);
                    __m128i ga_lo = _mm256_castsi256_si128(جمع_ga);
                    __m128i ga_hi = _mm256_extracti128_si256(جمع_ga, 1);
                    __m128i ga_s = mm_جمع_کن_epi64(ga_lo, ga_hi);
                    sums[0] += _mm_cvtsi128_si32(br_s);
                    sums[1] += _mm_cvtsi128_si32(ga_s);
                    sums[2] += _mm_cvtsi128_si32(_mm_srli_si128(br_s, 8));
                    sums[3] += _mm_cvtsi128_si32(_mm_srli_si128(ga_s, 8));
                }
                لكل (; x < تعداد_ستون; x++)
                {
                    sums[0] += (صحيح)src[x * 4];
                    sums[1] += (صحيح)src[x * 4 + 1];
                    sums[2] += (صحيح)src[x * 4 + 2];
                    sums[3] += (صحيح)src[x * 4 + 3];
                }
#وإلاإذا CV_SSSE3
                // Intel SSSE3: _mm_shuffle_epi8 deinterleave + _mm_sad_epu8
                ثابت __m128i zero = _mm_setzero_si128();
                ثابت __m128i shuf_ماسک = _mm_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                ثابت __m128i ماسک_lo32 = _mm_set1_epi64x(0x00000000FFFFFFFFLL);
                __m128i جمع_br = zero, جمع_ga = zero;
                صحيح x = 0;
                لكل (; x <= تعداد_ستون - 4; x += 4)
                {
                    __m128i v = _mm_loadu_si128((ثابت __m128i*)(src + x * 4));
                    __m128i grouped = _mm_shuffle_epi8(v, shuf_ماسک);
                    __m128i br = _mm_and_si128(grouped, ماسک_lo32);
                    __m128i ga = _mm_srli_epi64(grouped, 32);
                    جمع_br = mm_جمع_کن_epi64(جمع_br, _mm_sad_epu8(br, zero));
                    جمع_ga = mm_جمع_کن_epi64(جمع_ga, _mm_sad_epu8(ga, zero));
                }
                sums[0] = _mm_cvtsi128_si32(جمع_br);
                sums[1] = _mm_cvtsi128_si32(جمع_ga);
                sums[2] = _mm_cvtsi128_si32(_mm_srli_si128(جمع_br, 8));
                sums[3] = _mm_cvtsi128_si32(_mm_srli_si128(جمع_ga, 8));
                لكل (; x < تعداد_ستون; x++)
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

                لكل (; x <= تعداد_ستون - vlanes8; x += vlanes8)
                {
                    v_uint8 ch0, ch1, ch2, ch3;
                    v_بارگیری_deinterleave(src + x * 4, ch0, ch1, ch2, ch3);

                    v_uint16 lo, hi;
                    v_expand(ch0, lo, hi); vs16_0 = v_جمع_کن(vs16_0, v_جمع_کن(lo, hi));
                    v_expand(ch1, lo, hi); vs16_1 = v_جمع_کن(vs16_1, v_جمع_کن(lo, hi));
                    v_expand(ch2, lo, hi); vs16_2 = v_جمع_کن(vs16_2, v_جمع_کن(lo, hi));
                    v_expand(ch3, lo, hi); vs16_3 = v_جمع_کن(vs16_3, v_جمع_کن(lo, hi));

                    إذا (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(vs16_0, a, b); vsum0 = v_جمع_کن(vsum0, v_جمع_کن(a, b));
                        v_expand(vs16_1, a, b); vsum1 = v_جمع_کن(vsum1, v_جمع_کن(a, b));
                        v_expand(vs16_2, a, b); vsum2 = v_جمع_کن(vsum2, v_جمع_کن(a, b));
                        v_expand(vs16_3, a, b); vsum3 = v_جمع_کن(vsum3, v_جمع_کن(a, b));
                        vs16_0 = vx_setzero_u16(); vs16_1 = vx_setzero_u16();
                        vs16_2 = vx_setzero_u16(); vs16_3 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(vs16_0, a, b); vsum0 = v_جمع_کن(vsum0, v_جمع_کن(a, b));
                    v_expand(vs16_1, a, b); vsum1 = v_جمع_کن(vsum1, v_جمع_کن(a, b));
                    v_expand(vs16_2, a, b); vsum2 = v_جمع_کن(vsum2, v_جمع_کن(a, b));
                    v_expand(vs16_3, a, b); vsum3 = v_جمع_کن(vsum3, v_جمع_کن(a, b));
                }

                sums[0] = (صحيح)v_reduce_جمع(vsum0);
                sums[1] = (صحيح)v_reduce_جمع(vsum1);
                sums[2] = (صحيح)v_reduce_جمع(vsum2);
                sums[3] = (صحيح)v_reduce_جمع(vsum3);

                لكل (; x < تعداد_ستون; x++)
                {
                    sums[0] += (صحيح)src[x * 4];
                    sums[1] += (صحيح)src[x * 4 + 1];
                    sums[2] += (صحيح)src[x * 4 + 2];
                    sums[3] += (صحيح)src[x * 4 + 3];
                }
#پایان_اگر
            }
            وإلا
            {
                لكل (صحيح x = 0, c = 0; x < عرض; x++, c = (c+1)&-(c < cn-1))
                    sums[c] += (صحيح)src[x];
            }
            لكل (صحيح c = 0; c < cn; c++)
                dst[c] = (عائم)sums[c];
        }
    };
    parallel_for_(Range(0, srcmat.ردیفها), body);
    v_cleanup();
}

// --- 16-bit (ushort/short) → float ---
قالب<نام_نوع SrcT, نام_نوع VecSrc, نام_نوع VecDst>
ساكن فراغ reduce_ستون_جمع_16_32f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح cn = srcmat.channels();
    ثابت صحيح عرض = srcmat.تعداد_ستون * cn;
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
                لكل (; x <= عرض - vlanes; x += vlanes)
                {
                    VecDst lo, hi;
                    v_expand(vx_بارگیری(src + x), lo, hi);
                    v_sum0 = v_جمع_کن(v_sum0, lo);
                    v_sum1 = v_جمع_کن(v_sum1, hi);
                }
                dst[0] = (عائم)(v_reduce_جمع(v_sum0) + v_reduce_جمع(v_sum1));
                لكل (; x < عرض; x++)
                    dst[0] += (عائم)src[x];
            }
            وإلا
            {
                لكل (صحيح c = 0; c < cn; c++)
                    dst[c] = 0;
                لكل (صحيح x = 0, c = 0; x < عرض; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += (عائم)src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.ردیفها), body);
    v_cleanup();
}

ساكن فراغ reduce_ستون_جمع_16u32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_ستون_جمع_16_32f<ushort, v_uint16, v_uint32>(srcmat, dstmat); }

ساكن فراغ reduce_ستون_جمع_16s32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_ستون_جمع_16_32f<کوتاه, v_int16, v_int32>(srcmat, dstmat); }

// --- float/double → same type ---
قالب<نام_نوع T, نام_نوع VecT>
ساكن فراغ reduce_ستون_جمع_FP(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح cn = srcmat.channels();
    ثابت صحيح عرض = srcmat.تعداد_ستون * cn;
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
                لكل (; x <= عرض - vlanes * 4; x += vlanes * 4)
                {
                    s0 = v_جمع_کن(s0, vx_بارگیری(src + x));
                    s1 = v_جمع_کن(s1, vx_بارگیری(src + x + vlanes));
                    s2 = v_جمع_کن(s2, vx_بارگیری(src + x + vlanes * 2));
                    s3 = v_جمع_کن(s3, vx_بارگیری(src + x + vlanes * 3));
                }
                s0 = v_جمع_کن(v_جمع_کن(s0, s1), v_جمع_کن(s2, s3));
                لكل (; x <= عرض - vlanes; x += vlanes)
                    s0 = v_جمع_کن(s0, vx_بارگیری(src + x));
                T مجموع = (T)v_reduce_جمع(s0);
                لكل (; x < عرض; x++)
                    مجموع += src[x];
                dst[0] = مجموع;
            }
            وإلا
            {
                لكل (صحيح c = 0; c < cn; c++)
                    dst[c] = 0;
                لكل (صحيح x = 0, c = 0; x < عرض; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.ردیفها), body);
    v_cleanup();
}

ساكن فراغ reduce_ستون_جمع_32f32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_ستون_جمع_FP<عائم, v_float32>(srcmat, dstmat); }

#إذا CV_SIMD_64F
// --- float → double ---
ساكن فراغ reduce_ستون_جمع_32f64f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح cn = srcmat.channels();
    ثابت صحيح عرض = srcmat.تعداد_ستون * cn;
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
                لكل (; x <= عرض - vlanes32; x += vlanes32)
                {
                    v_float32 v = vx_بارگیری(src + x);
                    v_sum0 = v_جمع_کن(v_sum0, v_cvt_f64(v));
                    v_sum1 = v_جمع_کن(v_sum1, v_cvt_f64_بالا(v));
                }
                v_float64 v_مجموع = v_جمع_کن(v_sum0, v_sum1);

                مزدوج مجموع = v_reduce_جمع(v_مجموع);
                لكل (; x < عرض; x++)
                    مجموع += (مزدوج)src[x];
                dst[0] = مجموع;
            }
            وإلا
            {
                لكل (صحيح c = 0; c < cn; c++)
                    dst[c] = 0;
                لكل (صحيح x = 0, c = 0; x < عرض; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += (مزدوج)src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.ردیفها), body);
    v_cleanup();
}

ساكن فراغ reduce_ستون_جمع_64f64f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_ستون_جمع_FP<مزدوج, v_float64>(srcmat, dstmat); }
#پایان_اگر // CV_SIMD_64F

// =====================================================================
//  Row reduce SUM (dim=0): sum each column across all rows
// =====================================================================

// --- uchar → int ---
// Uses u16 intermediate accumulator for vertical sum: accumulate u8→u16 per row,
// flush u16→u32 every 256 rows (max u16 value: 256*255=65280 < 65535)
ساكن فراغ reduce_ردیف_جمع_8u32s(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح عرض_cn = srcmat.تعداد_ستون * srcmat.channels();
    ثابت صحيح ارتفاع = srcmat.ردیفها;
    ثابت صحيح vlanes16 = VTraits<v_uint16>::vlanes();
    ثابت صحيح vlanes32 = VTraits<v_int32>::vlanes();

    خودکار body = [&](ثابت Range& range) {
        ثابت صحيح شروع = range.شروع;
        ثابت صحيح end = range.end;
        ثابت صحيح len = end - شروع;

        Auto_میانگیر<صحيح> buf32_storage(len);
        Auto_میانگیر<ushort> buf16_storage(len);
        صحيح* buf32 = buf32_storage.داده();
        ushort* buf16 = buf16_storage.داده();

        // init from first row
        ثابت uchar* src0 = srcmat.ptr<uchar>(0) + شروع;
        لكل (صحيح i = 0; i < len; i++)
            buf32[i] = (صحيح)src0[i];

        إذا (ارتفاع <= 1)
        {
            صحيح* dst = dstmat.ptr<صحيح>(0) + شروع;
            لكل (صحيح i = 0; i < len; i++)
                dst[i] = buf32[i];
            أرجع;
        }

        memset(buf16, 0, len * حجم(ushort));
        ثابت صحيح flush_at = 256; // 256*255 = 65280 < 65535 (u16 safe)
        صحيح flush_شمار = 0;

        لكل (صحيح ردیف = 1; ردیف < ارتفاع; ردیف++)
        {
            ثابت uchar* src = srcmat.ptr<uchar>(ردیف) + شروع;
            صحيح i = 0;

#إذا CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
            // AArch64: vaddw_u8 — single-instruction widening add (u16 += u8)
            لكل (; i <= len - 16; i += 16)
            {
                uint8x16_t v = vld1q_u8(src + i);
                uint16x8_t acc_lo = vld1q_u16(buf16 + i);
                uint16x8_t acc_hi = vld1q_u16(buf16 + i + 8);
                vst1q_u16(buf16 + i,     vaddw_u8(acc_lo, vget_کم_u8(v)));
                vst1q_u16(buf16 + i + 8, vaddw_بالا_u8(acc_hi, v));
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
                v_expand(vx_بارگیری(src + i), lo, hi);
                v_store(buf16 + i, v_جمع_کن(vx_بارگیری(buf16 + i), lo));
                v_store(buf16 + i + vlanes16, v_جمع_کن(vx_بارگیری(buf16 + i + vlanes16), hi));
            }
#پایان_اگر
            لكل (; i < len; i++)
                buf16[i] += (ushort)src[i];

            إذا (++flush_شمار >= flush_at || ردیف == ارتفاع - 1)
            {
                // flush u16 → u32
                صحيح j = 0;
                لكل (; j <= len - vlanes16; j += vlanes16)
                {
                    v_uint32 lo, hi;
                    v_expand(vx_بارگیری(buf16 + j), lo, hi);
                    v_store(buf32 + j, v_جمع_کن(vx_بارگیری(buf32 + j), v_reinterpret_as_s32(lo)));
                    v_store(buf32 + j + vlanes32, v_جمع_کن(vx_بارگیری(buf32 + j + vlanes32), v_reinterpret_as_s32(hi)));
                }
                لكل (; j < len; j++)
                    buf32[j] += (صحيح)buf16[j];
                memset(buf16, 0, len * حجم(ushort));
                flush_شمار = 0;
            }
        }

        صحيح* dst = dstmat.ptr<صحيح>(0) + شروع;
        لكل (صحيح i = 0; i < len; i++)
            dst[i] = buf32[i];
    };

    parallel_for_(Range(0, عرض_cn), body, عرض_cn * CV_ELEM_اندازه(srcmat.عمق()) / 64);
    v_cleanup();
}

// --- uchar → float ---
ساكن فراغ reduce_ردیف_جمع_8u32f(ثابت Mat& srcmat, Mat& dstmat)
{
    // compute in int, then convert to float
    Mat موقت_2(dstmat.ردیفها, dstmat.تعداد_ستون, CV_32SC(srcmat.channels()));
    reduce_ردیف_جمع_8u32s(srcmat, موقت_2);
    موقت_2.convert_مقصد(dstmat, dstmat.type());
}

// --- 16-bit (ushort/short) → float row reduce ---
قالب<نام_نوع SrcT, نام_نوع VecSrc, نام_نوع VecDst>
ساكن فراغ reduce_ردیف_جمع_16_32f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح عرض_cn = srcmat.تعداد_ستون * srcmat.channels();
    ثابت صحيح ارتفاع = srcmat.ردیفها;
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
            v_expand(vx_بارگیری(src0), a0, a1);
            v_expand(vx_بارگیری(src0 + vlanes16), a2, a3);
            v_expand(vx_بارگیری(src0 + vlanes16*2), a4, a5);
            v_expand(vx_بارگیری(src0 + vlanes16*3), a6, a7);
            لكل (صحيح ردیف = 1; ردیف < ارتفاع; ردیف++)
            {
                ثابت SrcT* src = srcmat.ptr<SrcT>(ردیف) + شروع + i;
                VecDst lo, hi;
                v_expand(vx_بارگیری(src), lo, hi);
                a0 = v_جمع_کن(a0, lo); a1 = v_جمع_کن(a1, hi);
                v_expand(vx_بارگیری(src + vlanes16), lo, hi);
                a2 = v_جمع_کن(a2, lo); a3 = v_جمع_کن(a3, hi);
                v_expand(vx_بارگیری(src + vlanes16*2), lo, hi);
                a4 = v_جمع_کن(a4, lo); a5 = v_جمع_کن(a5, hi);
                v_expand(vx_بارگیری(src + vlanes16*3), lo, hi);
                a6 = v_جمع_کن(a6, lo); a7 = v_جمع_کن(a7, hi);
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
            v_expand(vx_بارگیری(srcmat.ptr<SrcT>(0) + شروع + i), a0, a1);
            لكل (صحيح ردیف = 1; ردیف < ارتفاع; ردیف++)
            {
                VecDst lo, hi;
                v_expand(vx_بارگیری(srcmat.ptr<SrcT>(ردیف) + شروع + i), lo, hi);
                a0 = v_جمع_کن(a0, lo); a1 = v_جمع_کن(a1, hi);
            }
            v_store(dst + i, v_cvt_f32(v_reinterpret_as_s32(a0)));
            v_store(dst + i + vlanes32, v_cvt_f32(v_reinterpret_as_s32(a1)));
        }
        لكل (; i < len; i++)
        {
            صحيح val = (صحيح)*(srcmat.ptr<SrcT>(0) + شروع + i);
            لكل (صحيح ردیف = 1; ردیف < ارتفاع; ردیف++)
                val += (صحيح)*(srcmat.ptr<SrcT>(ردیف) + شروع + i);
            dst[i] = (عائم)val;
        }
    };

    parallel_for_(Range(0, عرض_cn), body, عرض_cn * CV_ELEM_اندازه(srcmat.عمق()) / 64);
    v_cleanup();
}

ساكن فراغ reduce_ردیف_جمع_16u32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_ردیف_جمع_16_32f<ushort, v_uint16, v_uint32>(srcmat, dstmat); }

ساكن فراغ reduce_ردیف_جمع_16s32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_ردیف_جمع_16_32f<کوتاه, v_int16, v_int32>(srcmat, dstmat); }

// --- float/double → same type row reduce ---
قالب<نام_نوع T, نام_نوع VecT>
ساكن فراغ reduce_ردیف_جمع_FP(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح عرض_cn = srcmat.تعداد_ستون * srcmat.channels();
    ثابت صحيح ارتفاع = srcmat.ردیفها;
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
            VecT s0 = vx_بارگیری(src0), s1 = vx_بارگیری(src0 + vlanes);
            VecT s2 = vx_بارگیری(src0 + vlanes*2), s3 = vx_بارگیری(src0 + vlanes*3);
            VecT s4 = vx_بارگیری(src0 + vlanes*4), s5 = vx_بارگیری(src0 + vlanes*5);
            VecT s6 = vx_بارگیری(src0 + vlanes*6), s7 = vx_بارگیری(src0 + vlanes*7);
            لكل (صحيح ردیف = 1; ردیف < ارتفاع; ردیف++)
            {
                ثابت T* src = srcmat.ptr<T>(ردیف) + شروع + i;
                s0 = v_جمع_کن(s0, vx_بارگیری(src));
                s1 = v_جمع_کن(s1, vx_بارگیری(src + vlanes));
                s2 = v_جمع_کن(s2, vx_بارگیری(src + vlanes*2));
                s3 = v_جمع_کن(s3, vx_بارگیری(src + vlanes*3));
                s4 = v_جمع_کن(s4, vx_بارگیری(src + vlanes*4));
                s5 = v_جمع_کن(s5, vx_بارگیری(src + vlanes*5));
                s6 = v_جمع_کن(s6, vx_بارگیری(src + vlanes*6));
                s7 = v_جمع_کن(s7, vx_بارگیری(src + vlanes*7));
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
            VecT s0 = vx_بارگیری(srcmat.ptr<T>(0) + شروع + i);
            لكل (صحيح ردیف = 1; ردیف < ارتفاع; ردیف++)
                s0 = v_جمع_کن(s0, vx_بارگیری(srcmat.ptr<T>(ردیف) + شروع + i));
            v_store(dst + i, s0);
        }
        لكل (; i < len; i++)
        {
            T val = *(srcmat.ptr<T>(0) + شروع + i);
            لكل (صحيح ردیف = 1; ردیف < ارتفاع; ردیف++)
                val += *(srcmat.ptr<T>(ردیف) + شروع + i);
            dst[i] = val;
        }
    };

    parallel_for_(Range(0, عرض_cn), body, عرض_cn * CV_ELEM_اندازه(srcmat.عمق()) / 64);
    v_cleanup();
}

ساكن فراغ reduce_ردیف_جمع_32f32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_ردیف_جمع_FP<عائم, v_float32>(srcmat, dstmat); }

#إذا CV_SIMD_64F
// --- float → double ---
ساكن فراغ reduce_ردیف_جمع_32f64f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح عرض_cn = srcmat.تعداد_ستون * srcmat.channels();
    ثابت صحيح ارتفاع = srcmat.ردیفها;
    ثابت صحيح vlanes32 = VTraits<v_float32>::vlanes();
    ثابت صحيح vlanes64 = VTraits<v_float64>::vlanes();

    خودکار body = [&](ثابت Range& range) {
        ثابت صحيح شروع = range.شروع;
        ثابت صحيح end = range.end;
        ثابت صحيح len = end - شروع;

        Auto_میانگیر<مزدوج> buf_storage(len);
        مزدوج* buf = buf_storage.داده();

        ثابت عائم* src0 = srcmat.ptr<عائم>(0) + شروع;
        لكل (صحيح i = 0; i < len; i++)
            buf[i] = (مزدوج)src0[i];

        لكل (صحيح ردیف = 1; ردیف < ارتفاع; ردیف++)
        {
            ثابت عائم* src = srcmat.ptr<عائم>(ردیف) + شروع;
            صحيح i = 0;
            لكل (; i <= len - vlanes32; i += vlanes32)
            {
                v_float32 v = vx_بارگیری(src + i);
                v_store(buf + i, v_جمع_کن(vx_بارگیری(buf + i), v_cvt_f64(v)));
                v_store(buf + i + vlanes64, v_جمع_کن(vx_بارگیری(buf + i + vlanes64), v_cvt_f64_بالا(v)));
            }
            لكل (; i < len; i++)
                buf[i] += (مزدوج)src[i];
        }

        مزدوج* dst = dstmat.ptr<مزدوج>(0) + شروع;
        کپی_حافظه(dst, buf, len * حجم(مزدوج));
    };

    parallel_for_(Range(0, عرض_cn), body, عرض_cn * CV_ELEM_اندازه(srcmat.عمق()) / 64);
    v_cleanup();
}

ساكن فراغ reduce_ردیف_جمع_64f64f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_ردیف_جمع_FP<مزدوج, v_float64>(srcmat, dstmat); }
#پایان_اگر // CV_SIMD_64F

#پایان_اگر // CV_SIMD || CV_SIMD_SCALABLE

#إذا CV_SIMD_SCALABLE
ساكن مضمن v_float32 reduce_ردیف_Sum2_Acc4(ثابت v_float32& acc,
                                           ثابت عائم* src0, ثابت عائم* src1,
                                           ثابت عائم* src2, ثابت عائم* src3)
{
    v_float32 جمع = acc;
    v_float32 x = vx_بارگیری(src0);
    جمع = v_fma(x, x, جمع);
    x = vx_بارگیری(src1);
    جمع = v_fma(x, x, جمع);
    x = vx_بارگیری(src2);
    جمع = v_fma(x, x, جمع);
    x = vx_بارگیری(src3);
    أرجع v_fma(x, x, جمع);
}

// dim=0, REDUCE_SUM2 for CV_32F using scalable universal intrinsics.
ساكن فراغ reduce_ردیف_Sum2_32f32f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحيح عرض = srcmat.تعداد_ستون * srcmat.channels();
    ثابت صحيح ارتفاع = srcmat.ردیفها;
    ثابت صحيح vlanes = VTraits<v_float32>::vlanes();
    ثابت صحيح بلوک_2 = vlanes * 4;
    Auto_میانگیر<عائم> میانگیر(عرض);
    عائم* buf = میانگیر.داده();

    ثابت عائم* src = srcmat.ptr<عائم>(0);
    صحيح i = 0;
    لكل (; i <= عرض - بلوک_2; i += بلوک_2)
    {
        v_float32 x0 = vx_بارگیری(src + i);
        v_float32 x1 = vx_بارگیری(src + i + vlanes);
        v_float32 x2 = vx_بارگیری(src + i + vlanes * 2);
        v_float32 x3 = vx_بارگیری(src + i + vlanes * 3);
        v_store(buf + i, v_mul(x0, x0));
        v_store(buf + i + vlanes, v_mul(x1, x1));
        v_store(buf + i + vlanes * 2, v_mul(x2, x2));
        v_store(buf + i + vlanes * 3, v_mul(x3, x3));
    }
    لكل (; i <= عرض - vlanes; i += vlanes)
    {
        v_float32 x = vx_بارگیری(src + i);
        v_store(buf + i, v_mul(x, x));
    }
    لكل (; i < عرض; i++)
        buf[i] = src[i] * src[i];

    صحيح ردیف = 1;
    // Four rows per pass amortize buffer traffic while preserving source-row
    // accumulation order. The outer loop stays serial: splitting width into
    // tiny parallel stripes under-fills scalable vectors and adds scheduling
    // overhead for this memory-streaming kernel.
    لكل (; ردیف <= ارتفاع - 4; ردیف += 4)
    {
        ثابت عائم* src0 = srcmat.ptr<عائم>(ردیف);
        ثابت عائم* src1 = srcmat.ptr<عائم>(ردیف + 1);
        ثابت عائم* src2 = srcmat.ptr<عائم>(ردیف + 2);
        ثابت عائم* src3 = srcmat.ptr<عائم>(ردیف + 3);

        i = 0;
        لكل (; i <= عرض - بلوک_2; i += بلوک_2)
        {
            v_float32 sum0 = vx_بارگیری(buf + i);
            v_float32 sum1 = vx_بارگیری(buf + i + vlanes);
            v_float32 sum2 = vx_بارگیری(buf + i + vlanes * 2);
            v_float32 sum3 = vx_بارگیری(buf + i + vlanes * 3);
            sum0 = reduce_ردیف_Sum2_Acc4(sum0, src0 + i, src1 + i, src2 + i, src3 + i);
            sum1 = reduce_ردیف_Sum2_Acc4(sum1, src0 + i + vlanes, src1 + i + vlanes,
                                     src2 + i + vlanes, src3 + i + vlanes);
            sum2 = reduce_ردیف_Sum2_Acc4(sum2, src0 + i + vlanes * 2, src1 + i + vlanes * 2,
                                     src2 + i + vlanes * 2, src3 + i + vlanes * 2);
            sum3 = reduce_ردیف_Sum2_Acc4(sum3, src0 + i + vlanes * 3, src1 + i + vlanes * 3,
                                     src2 + i + vlanes * 3, src3 + i + vlanes * 3);
            v_store(buf + i, sum0);
            v_store(buf + i + vlanes, sum1);
            v_store(buf + i + vlanes * 2, sum2);
            v_store(buf + i + vlanes * 3, sum3);
        }
        لكل (; i <= عرض - vlanes; i += vlanes)
        {
            v_float32 جمع = vx_بارگیری(buf + i);
            جمع = reduce_ردیف_Sum2_Acc4(جمع, src0 + i, src1 + i, src2 + i, src3 + i);
            v_store(buf + i, جمع);
        }
        لكل (; i < عرض; i++)
        {
            عائم جمع = buf[i];
            عائم x = src0[i]; جمع += x * x;
            x = src1[i]; جمع += x * x;
            x = src2[i]; جمع += x * x;
            x = src3[i]; جمع += x * x;
            buf[i] = جمع;
        }
    }

    لكل (; ردیف < ارتفاع; ردیف++)
    {
        src = srcmat.ptr<عائم>(ردیف);
        i = 0;
        لكل (; i <= عرض - بلوک_2; i += بلوک_2)
        {
            v_float32 sum0 = vx_بارگیری(buf + i);
            v_float32 sum1 = vx_بارگیری(buf + i + vlanes);
            v_float32 sum2 = vx_بارگیری(buf + i + vlanes * 2);
            v_float32 sum3 = vx_بارگیری(buf + i + vlanes * 3);
            v_float32 x0 = vx_بارگیری(src + i);
            v_float32 x1 = vx_بارگیری(src + i + vlanes);
            v_float32 x2 = vx_بارگیری(src + i + vlanes * 2);
            v_float32 x3 = vx_بارگیری(src + i + vlanes * 3);
            v_store(buf + i, v_fma(x0, x0, sum0));
            v_store(buf + i + vlanes, v_fma(x1, x1, sum1));
            v_store(buf + i + vlanes * 2, v_fma(x2, x2, sum2));
            v_store(buf + i + vlanes * 3, v_fma(x3, x3, sum3));
        }
        لكل (; i <= عرض - vlanes; i += vlanes)
        {
            v_float32 x = vx_بارگیری(src + i);
            v_store(buf + i, v_fma(x, x, vx_بارگیری(buf + i)));
        }
        لكل (; i < عرض; i++)
            buf[i] += src[i] * src[i];
    }

    // Delay output stores until all source rows are consumed, preserving the
    // existing overlap-safe behavior of the proposed optimized path.
    کپی_حافظه(dstmat.ptr<عائم>(), buf, عرض * حجم(عائم));
    v_cleanup();
}
#پایان_اگر

// =====================================================================
//  Dispatchers
// =====================================================================

Reduce_جمع_Func getReduceCSumFunc(صحيح sdepth, صحيح ddepth)
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا (sdepth == CV_8U && ddepth == CV_32S) أرجع reduce_ستون_جمع_8u32s;
    إذا (sdepth == CV_8U && ddepth == CV_32F) أرجع reduce_ستون_جمع_8u32f;
    إذا (sdepth == CV_16U && ddepth == CV_32F) أرجع reduce_ستون_جمع_16u32f;
    إذا (sdepth == CV_16S && ddepth == CV_32F) أرجع reduce_ستون_جمع_16s32f;
    إذا (sdepth == CV_32F && ddepth == CV_32F) أرجع reduce_ستون_جمع_32f32f;
#إذا CV_SIMD_64F
    إذا (sdepth == CV_32F && ddepth == CV_64F) أرجع reduce_ستون_جمع_32f64f;
    إذا (sdepth == CV_64F && ddepth == CV_64F) أرجع reduce_ستون_جمع_64f64f;
#پایان_اگر
#وإلا
    CV_استفادهنشده(sdepth);
    CV_استفادهنشده(ddepth);
#پایان_اگر
    أرجع اشاره_تهی;
}

Reduce_جمع_Func getReduceRSumFunc(صحيح sdepth, صحيح ddepth)
{
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
    إذا (sdepth == CV_8U && ddepth == CV_32S) أرجع reduce_ردیف_جمع_8u32s;
    إذا (sdepth == CV_8U && ddepth == CV_32F) أرجع reduce_ردیف_جمع_8u32f;
    إذا (sdepth == CV_16U && ddepth == CV_32F) أرجع reduce_ردیف_جمع_16u32f;
    إذا (sdepth == CV_16S && ddepth == CV_32F) أرجع reduce_ردیف_جمع_16s32f;
    إذا (sdepth == CV_32F && ddepth == CV_32F) أرجع reduce_ردیف_جمع_32f32f;
#إذا CV_SIMD_64F
    إذا (sdepth == CV_32F && ddepth == CV_64F) أرجع reduce_ردیف_جمع_32f64f;
    إذا (sdepth == CV_64F && ddepth == CV_64F) أرجع reduce_ردیف_جمع_64f64f;
#پایان_اگر
#وإلا
    CV_استفادهنشده(sdepth);
    CV_استفادهنشده(ddepth);
#پایان_اگر
    أرجع اشاره_تهی;
}

Reduce_جمع_Func getReduceRSum2Func(صحيح sdepth, صحيح ddepth)
{
#إذا CV_SIMD_SCALABLE
    إذا (sdepth == CV_32F && ddepth == CV_32F) أرجع reduce_ردیف_Sum2_32f32f;
#وإلا
    CV_استفادهنشده(sdepth);
    CV_استفادهنشده(ddepth);
#پایان_اگر
    أرجع اشاره_تهی;
}

#پایان_اگر // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_انتها
} // namespace cv
