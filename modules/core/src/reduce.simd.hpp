// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"

نامخلا cv {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

قسمتعریف خالی (*Reduce_مجموع_Func)(ثابت Mat& src, Mat& dst);
Reduce_مجموع_Func getReduceCSumFunc(صحیح sdepth, صحیح ddepth);
Reduce_مجموع_Func getReduceRSumFunc(صحیح sdepth, صحیح ddepth);
Reduce_مجموع_Func getReduceRSum2Func(صحیح sdepth, صحیح ddepth);

#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// =====================================================================
//  Col reduce SUM (dim=1): sum each row into cn output values
// =====================================================================

#اگر (CV_SIMD || CV_SIMD_SCALABLE)

// --- uchar → int ---
// Uses u16 intermediate accumulator to halve the number of widen operations:
// u8→u16 per iteration, u16→u32 flush every 128 iterations (max u16 value: 128*255=32640 < 65535)
ساکن خالی reduce_ستون_مجموع_8u32s(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحیح cn = srcmat.channels();
    ثابت صحیح ستون_تعداد = srcmat.ستون_تعداد;
    ثابت صحیح چوڑائی = ستون_تعداد * cn;

    خودکار body = [&](ثابت Range& range) {
        برائے (صحیح y = range.آغاز; y < range.end; y++)
        {
            ثابت uchar* src = srcmat.ptr<uchar>(y);
            صحیح* dst = dstmat.ptr<صحیح>(y);

            اگر (cn == 1)
            {
#اگر defined(CV_NEON_DOT)
                // ARMv8.2 DOTPROD: vdotq_u32(acc, src, ones) — single instruction byte sum
                // u32 lane max 1020/iter, overflow at ~4.2M iter (67MB) — safe without flush
                ثابت uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t v_مجموع = vdupq_n_u32(0);
                صحیح x = 0;
                برائے (; x <= چوڑائی - 16; x += 16)
                    v_مجموع = vdotq_u32(v_مجموع, vld1q_u8(src + x), ones);
                صحیح کل = (صحیح)vaddvq_u32(v_مجموع);
                برائے (; x < چوڑائی; x++)
                    کل += (صحیح)src[x];
                dst[0] = کل;
#وگرنہ_اگر CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                // AArch64 fallback: vpaddlq chain
                uint32x4_t v_مجموع = vdupq_n_u32(0);
                صحیح x = 0;
                برائے (; x <= چوڑائی - 16; x += 16)
                    v_مجموع = vaddq_u32(v_مجموع, vpaddlq_u16(vpaddlq_u8(vld1q_u8(src + x))));
                صحیح کل = (صحیح)vaddvq_u32(v_مجموع);
                برائے (; x < چوڑائی; x++)
                    کل += (صحیح)src[x];
                dst[0] = کل;
#وگرنہ_اگر CV_AVX2
                // Intel AVX2: _mm256_sad_epu8 — 32 bytes → 4×u64 sum, 1 cycle
                ثابت __m256i zero = _mm256_setzero_si256();
                __m256i vsum = zero;
                صحیح x = 0;
                برائے (; x <= چوڑائی - 32; x += 32)
                    vsum = mm256_جوڑو_epi64(vsum, _mm256_sad_epu8(
                        _mm256_loadu_si256((ثابت __m256i*)(src + x)), zero));
                // 8-byte tail: reduce max scalar tail from 31 to 7
                // Use _mm256_inserti128_si256 instead of _mm256_castsi128_si256:
                // the latter leaves upper 128 bits undefined per Intel spec,
                // causing MSVC to expose stale register data to _mm256_sad_epu8.
                برائے (; x <= چوڑائی - 8; x += 8)
                    vsum = mm256_جوڑو_epi64(vsum, _mm256_sad_epu8(
                        _mm256_inserti128_si256(_mm256_setzero_si256(), _mm_loadl_epi64((ثابت __m128i*)(src + x)), 0), zero));
                __m128i lo128 = _mm256_castsi256_si128(vsum);
                __m128i hi128 = _mm256_extracti128_si256(vsum, 1);
                __m128i s = mm_جوڑو_epi64(lo128, hi128);
                s = mm_جوڑو_epi64(s, _mm_unpackhi_epi64(s, s));
                صحیح کل = _mm_cvtsi128_si32(s);
                برائے (; x < چوڑائی; x++)
                    کل += (صحیح)src[x];
                dst[0] = کل;
#وگرنہ_اگر CV_SSE2
                // Intel SSE2: _mm_sad_epu8 — 16 bytes → 2×u64 sum
                ثابت __m128i zero = _mm_setzero_si128();
                __m128i vsum = zero;
                صحیح x = 0;
                برائے (; x <= چوڑائی - 16; x += 16)
                    vsum = mm_جوڑو_epi64(vsum, _mm_sad_epu8(
                        _mm_loadu_si128((ثابت __m128i*)(src + x)), zero));
                __m128i s = mm_جوڑو_epi64(vsum, _mm_unpackhi_epi64(vsum, vsum));
                صحیح کل = _mm_cvtsi128_si32(s);
                برائے (; x < چوڑائی; x++)
                    کل += (صحیح)src[x];
                dst[0] = کل;
#ورنہ
                ثابت صحیح vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 v_مجموع = vx_setzero_u32();
                v_uint16 v_sum16 = vx_setzero_u16();
                صحیح x = 0, batch = 0;
                ثابت صحیح flush_at = 128;

                برائے (; x <= چوڑائی - vlanes8; x += vlanes8)
                {
                    v_uint16 lo, hi;
                    v_expand(vx_بھرو(src + x), lo, hi);
                    v_sum16 = v_جوڑو(v_sum16, v_جوڑو(lo, hi));

                    اگر (++batch >= flush_at)
                    {
                        v_uint32 a, b;
                        v_expand(v_sum16, a, b);
                        v_مجموع = v_جوڑو(v_مجموع, v_جوڑو(a, b));
                        v_sum16 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                {
                    v_uint32 a, b;
                    v_expand(v_sum16, a, b);
                    v_مجموع = v_جوڑو(v_مجموع, v_جوڑو(a, b));
                }

                صحیح کل = (صحیح)v_reduce_مجموع(v_مجموع);
                برائے (; x < چوڑائی; x++)
                    کل += (صحیح)src[x];
                dst[0] = کل;
#اگر_ختم
            }
            ورنہ اگر (cn == 4)
            {
#اگر defined(CV_NEON_DOT)
                // ARMv8.2 DOTPROD: vld4q_u8 deinterleave + vdotq_u32 per-channel
                // vld4q_u8 reads 16 pixels (64 bytes), deinterleaves into 4 × u8x16
                // vdotq_u32 sums each channel's 16 bytes into 4 partial u32 sums
                ثابت uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t acc0 = vdupq_n_u32(0), acc1 = vdupq_n_u32(0);
                uint32x4_t acc2 = vdupq_n_u32(0), acc3 = vdupq_n_u32(0);
                صحیح x = 0;
                برائے (; x <= ستون_تعداد - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    acc0 = vdotq_u32(acc0, v.val[0], ones);
                    acc1 = vdotq_u32(acc1, v.val[1], ones);
                    acc2 = vdotq_u32(acc2, v.val[2], ones);
                    acc3 = vdotq_u32(acc3, v.val[3], ones);
                }
                dst[0] = (صحیح)vaddvq_u32(acc0);
                dst[1] = (صحیح)vaddvq_u32(acc1);
                dst[2] = (صحیح)vaddvq_u32(acc2);
                dst[3] = (صحیح)vaddvq_u32(acc3);
                برائے (; x < ستون_تعداد; x++)
                {
                    dst[0] += (صحیح)src[x * 4];
                    dst[1] += (صحیح)src[x * 4 + 1];
                    dst[2] += (صحیح)src[x * 4 + 2];
                    dst[3] += (صحیح)src[x * 4 + 3];
                }
#وگرنہ_اگر CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                // AArch64 fallback: vld4q_u8 deinterleave + vaddlvq_u8 per-channel
                صحیح sums[4] = {0, 0, 0, 0};
                صحیح x = 0;
                برائے (; x <= ستون_تعداد - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    sums[0] += (صحیح)vaddlvq_u8(v.val[0]);
                    sums[1] += (صحیح)vaddlvq_u8(v.val[1]);
                    sums[2] += (صحیح)vaddlvq_u8(v.val[2]);
                    sums[3] += (صحیح)vaddlvq_u8(v.val[3]);
                }
                dst[0] = sums[0]; dst[1] = sums[1];
                dst[2] = sums[2]; dst[3] = sums[3];
                برائے (; x < ستون_تعداد; x++)
                {
                    dst[0] += (صحیح)src[x * 4];
                    dst[1] += (صحیح)src[x * 4 + 1];
                    dst[2] += (صحیح)src[x * 4 + 2];
                    dst[3] += (صحیح)src[x * 4 + 3];
                }
#وگرنہ_اگر CV_AVX2
                // Intel AVX2: shuffle + unpack to group channels, full-register SAD
                ثابت __m256i zero = _mm256_setzero_si256();
                ثابت __m256i shuf_نقاب = _mm256_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                صحیح x = 0;
                // 16 pixels/iter with unpack (full-register SAD)
                {
                    __m256i مجموع_bg = zero, مجموع_ra = zero;
                    برائے (; x <= ستون_تعداد - 16; x += 16)
                    {
                        __m256i v0 = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4));
                        __m256i v1 = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4 + 32));
                        __m256i g0 = _mm256_shuffle_epi8(v0, shuf_نقاب);
                        __m256i g1 = _mm256_shuffle_epi8(v1, shuf_نقاب);
                        __m256i bg = _mm256_unpacklo_epi32(g0, g1);
                        __m256i ra = _mm256_unpackhi_epi32(g0, g1);
                        مجموع_bg = mm256_جوڑو_epi64(مجموع_bg, _mm256_sad_epu8(bg, zero));
                        مجموع_ra = mm256_جوڑو_epi64(مجموع_ra, _mm256_sad_epu8(ra, zero));
                    }
                    __m128i bg_lo = _mm256_castsi256_si128(مجموع_bg);
                    __m128i bg_hi = _mm256_extracti128_si256(مجموع_bg, 1);
                    __m128i bg_s = mm_جوڑو_epi64(bg_lo, bg_hi);
                    __m128i ra_lo = _mm256_castsi256_si128(مجموع_ra);
                    __m128i ra_hi = _mm256_extracti128_si256(مجموع_ra, 1);
                    __m128i ra_s = mm_جوڑو_epi64(ra_lo, ra_hi);
                    dst[0] = _mm_cvtsi128_si32(bg_s);                     // B
                    dst[1] = _mm_cvtsi128_si32(_mm_srli_si128(bg_s, 8));  // G
                    dst[2] = _mm_cvtsi128_si32(ra_s);                     // R
                    dst[3] = _mm_cvtsi128_si32(_mm_srli_si128(ra_s, 8));  // A
                }
                // 8-pixel tail with and/srli
                {
                    ثابت __m256i نقاب_lo32 = _mm256_set1_epi64x(0x00000000FFFFFFFFLL);
                    __m256i مجموع_br = zero, مجموع_ga = zero;
                    برائے (; x <= ستون_تعداد - 8; x += 8)
                    {
                        __m256i v = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4));
                        __m256i grouped = _mm256_shuffle_epi8(v, shuf_نقاب);
                        __m256i br = _mm256_and_si256(grouped, نقاب_lo32);
                        __m256i ga = _mm256_srli_epi64(grouped, 32);
                        مجموع_br = mm256_جوڑو_epi64(مجموع_br, _mm256_sad_epu8(br, zero));
                        مجموع_ga = mm256_جوڑو_epi64(مجموع_ga, _mm256_sad_epu8(ga, zero));
                    }
                    __m128i br_lo = _mm256_castsi256_si128(مجموع_br);
                    __m128i br_hi = _mm256_extracti128_si256(مجموع_br, 1);
                    __m128i br_s = mm_جوڑو_epi64(br_lo, br_hi);
                    __m128i ga_lo = _mm256_castsi256_si128(مجموع_ga);
                    __m128i ga_hi = _mm256_extracti128_si256(مجموع_ga, 1);
                    __m128i ga_s = mm_جوڑو_epi64(ga_lo, ga_hi);
                    dst[0] += _mm_cvtsi128_si32(br_s);
                    dst[1] += _mm_cvtsi128_si32(ga_s);
                    dst[2] += _mm_cvtsi128_si32(_mm_srli_si128(br_s, 8));
                    dst[3] += _mm_cvtsi128_si32(_mm_srli_si128(ga_s, 8));
                }
                برائے (; x < ستون_تعداد; x++)
                {
                    dst[0] += (صحیح)src[x * 4];
                    dst[1] += (صحیح)src[x * 4 + 1];
                    dst[2] += (صحیح)src[x * 4 + 2];
                    dst[3] += (صحیح)src[x * 4 + 3];
                }
#وگرنہ_اگر CV_SSSE3
                // Intel SSSE3: _mm_shuffle_epi8 deinterleave + _mm_sad_epu8
                // 16 bytes = 4 pixels × 4 channels per iteration
                ثابت __m128i zero = _mm_setzero_si128();
                ثابت __m128i shuf_نقاب = _mm_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                ثابت __m128i نقاب_lo32 = _mm_set1_epi64x(0x00000000FFFFFFFFLL);
                __m128i مجموع_br = zero, مجموع_ga = zero;
                صحیح x = 0;
                برائے (; x <= ستون_تعداد - 4; x += 4)
                {
                    __m128i v = _mm_loadu_si128((ثابت __m128i*)(src + x * 4));
                    __m128i grouped = _mm_shuffle_epi8(v, shuf_نقاب);
                    __m128i br = _mm_and_si128(grouped, نقاب_lo32);
                    __m128i ga = _mm_srli_epi64(grouped, 32);
                    مجموع_br = mm_جوڑو_epi64(مجموع_br, _mm_sad_epu8(br, zero));
                    مجموع_ga = mm_جوڑو_epi64(مجموع_ga, _mm_sad_epu8(ga, zero));
                }
                dst[0] = _mm_cvtsi128_si32(مجموع_br);
                dst[1] = _mm_cvtsi128_si32(مجموع_ga);
                dst[2] = _mm_cvtsi128_si32(_mm_srli_si128(مجموع_br, 8));
                dst[3] = _mm_cvtsi128_si32(_mm_srli_si128(مجموع_ga, 8));
                برائے (; x < ستون_تعداد; x++)
                {
                    dst[0] += (صحیح)src[x * 4];
                    dst[1] += (صحیح)src[x * 4 + 1];
                    dst[2] += (صحیح)src[x * 4 + 2];
                    dst[3] += (صحیح)src[x * 4 + 3];
                }
#ورنہ
                ثابت صحیح vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 vsum0 = vx_setzero_u32(), vsum1 = vx_setzero_u32();
                v_uint32 vsum2 = vx_setzero_u32(), vsum3 = vx_setzero_u32();
                v_uint16 vs16_0 = vx_setzero_u16(), vs16_1 = vx_setzero_u16();
                v_uint16 vs16_2 = vx_setzero_u16(), vs16_3 = vx_setzero_u16();
                صحیح x = 0, batch = 0;
                ثابت صحیح flush_at = 128;

                برائے (; x <= ستون_تعداد - vlanes8; x += vlanes8)
                {
                    v_uint8 ch0, ch1, ch2, ch3;
                    v_بھرو_deinterleave(src + x * 4, ch0, ch1, ch2, ch3);

                    v_uint16 lo, hi;
                    v_expand(ch0, lo, hi); vs16_0 = v_جوڑو(vs16_0, v_جوڑو(lo, hi));
                    v_expand(ch1, lo, hi); vs16_1 = v_جوڑو(vs16_1, v_جوڑو(lo, hi));
                    v_expand(ch2, lo, hi); vs16_2 = v_جوڑو(vs16_2, v_جوڑو(lo, hi));
                    v_expand(ch3, lo, hi); vs16_3 = v_جوڑو(vs16_3, v_جوڑو(lo, hi));

                    اگر (++batch >= flush_at)
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

                dst[0] = (صحیح)v_reduce_مجموع(vsum0);
                dst[1] = (صحیح)v_reduce_مجموع(vsum1);
                dst[2] = (صحیح)v_reduce_مجموع(vsum2);
                dst[3] = (صحیح)v_reduce_مجموع(vsum3);

                برائے (; x < ستون_تعداد; x++)
                {
                    dst[0] += (صحیح)src[x * 4];
                    dst[1] += (صحیح)src[x * 4 + 1];
                    dst[2] += (صحیح)src[x * 4 + 2];
                    dst[3] += (صحیح)src[x * 4 + 3];
                }
#اگر_ختم
            }
            ورنہ
            {
                // generic cn: scalar fallback
                برائے (صحیح c = 0; c < cn; c++)
                    dst[c] = (صحیح)src[c];
                برائے (صحیح x = cn; x < چوڑائی; x += cn)
                    برائے (صحیح c = 0; c < cn; c++)
                        dst[c] += (صحیح)src[x + c];
            }
        }
    };

    parallel_for_(Range(0, srcmat.صفیں), body);
    v_cleanup();
}

// --- uchar → float ---
ساکن خالی reduce_ستون_مجموع_8u32f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحیح cn = srcmat.channels();
    ثابت صحیح ستون_تعداد = srcmat.ستون_تعداد;
    ثابت صحیح چوڑائی = ستون_تعداد * cn;

    خودکار body = [&](ثابت Range& range) {
        برائے (صحیح y = range.آغاز; y < range.end; y++)
        {
            ثابت uchar* src = srcmat.ptr<uchar>(y);
            اعشاری* dst = dstmat.ptr<اعشاری>(y);

            // compute in int, then convert to float
            Auto_بفر<صحیح> ibuf(cn);
            صحیح* sums = ibuf.ڈیٹا();
            برائے (صحیح c = 0; c < cn; c++)
                sums[c] = 0;

            اگر (cn == 1)
            {
#اگر defined(CV_NEON_DOT)
                ثابت uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t v_مجموع = vdupq_n_u32(0);
                صحیح x = 0;
                برائے (; x <= چوڑائی - 16; x += 16)
                    v_مجموع = vdotq_u32(v_مجموع, vld1q_u8(src + x), ones);
                sums[0] = (صحیح)vaddvq_u32(v_مجموع);
                برائے (; x < چوڑائی; x++)
                    sums[0] += (صحیح)src[x];
#وگرنہ_اگر CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                uint32x4_t v_مجموع = vdupq_n_u32(0);
                صحیح x = 0;
                برائے (; x <= چوڑائی - 16; x += 16)
                    v_مجموع = vaddq_u32(v_مجموع, vpaddlq_u16(vpaddlq_u8(vld1q_u8(src + x))));
                sums[0] = (صحیح)vaddvq_u32(v_مجموع);
                برائے (; x < چوڑائی; x++)
                    sums[0] += (صحیح)src[x];
#وگرنہ_اگر CV_AVX2
                ثابت __m256i zero = _mm256_setzero_si256();
                __m256i vsum = zero;
                صحیح x = 0;
                برائے (; x <= چوڑائی - 32; x += 32)
                    vsum = mm256_جوڑو_epi64(vsum, _mm256_sad_epu8(
                        _mm256_loadu_si256((ثابت __m256i*)(src + x)), zero));
                // 8-byte tail: reduce max scalar tail from 31 to 7
                // Use _mm256_inserti128_si256 instead of _mm256_castsi128_si256:
                // the latter leaves upper 128 bits undefined per Intel spec,
                // causing MSVC to expose stale register data to _mm256_sad_epu8.
                برائے (; x <= چوڑائی - 8; x += 8)
                    vsum = mm256_جوڑو_epi64(vsum, _mm256_sad_epu8(
                        _mm256_inserti128_si256(_mm256_setzero_si256(), _mm_loadl_epi64((ثابت __m128i*)(src + x)), 0), zero));
                __m128i lo128 = _mm256_castsi256_si128(vsum);
                __m128i hi128 = _mm256_extracti128_si256(vsum, 1);
                __m128i s = mm_جوڑو_epi64(lo128, hi128);
                s = mm_جوڑو_epi64(s, _mm_unpackhi_epi64(s, s));
                sums[0] = _mm_cvtsi128_si32(s);
                برائے (; x < چوڑائی; x++)
                    sums[0] += (صحیح)src[x];
#وگرنہ_اگر CV_SSE2
                ثابت __m128i zero = _mm_setzero_si128();
                __m128i vsum = zero;
                صحیح x = 0;
                برائے (; x <= چوڑائی - 16; x += 16)
                    vsum = mm_جوڑو_epi64(vsum, _mm_sad_epu8(
                        _mm_loadu_si128((ثابت __m128i*)(src + x)), zero));
                __m128i s = mm_جوڑو_epi64(vsum, _mm_unpackhi_epi64(vsum, vsum));
                sums[0] = _mm_cvtsi128_si32(s);
                برائے (; x < چوڑائی; x++)
                    sums[0] += (صحیح)src[x];
#ورنہ
                ثابت صحیح vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 v_مجموع = vx_setzero_u32();
                v_uint16 v_sum16 = vx_setzero_u16();
                صحیح x = 0, batch = 0;

                برائے (; x <= چوڑائی - vlanes8; x += vlanes8)
                {
                    v_uint16 lo, hi;
                    v_expand(vx_بھرو(src + x), lo, hi);
                    v_sum16 = v_جوڑو(v_sum16, v_جوڑو(lo, hi));
                    اگر (++batch >= 128)
                    {
                        v_uint32 a, b;
                        v_expand(v_sum16, a, b);
                        v_مجموع = v_جوڑو(v_مجموع, v_جوڑو(a, b));
                        v_sum16 = vx_setzero_u16();
                        batch = 0;
                    }
                }
                v_uint32 a, b;
                v_expand(v_sum16, a, b);
                v_مجموع = v_جوڑو(v_مجموع, v_جوڑو(a, b));

                sums[0] = (صحیح)v_reduce_مجموع(v_مجموع);
                برائے (; x < چوڑائی; x++)
                    sums[0] += (صحیح)src[x];
#اگر_ختم
            }
            ورنہ اگر (cn == 4)
            {
#اگر defined(CV_NEON_DOT)
                ثابت uint8x16_t ones = vdupq_n_u8(1);
                uint32x4_t acc0 = vdupq_n_u32(0), acc1 = vdupq_n_u32(0);
                uint32x4_t acc2 = vdupq_n_u32(0), acc3 = vdupq_n_u32(0);
                صحیح x = 0;
                برائے (; x <= ستون_تعداد - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    acc0 = vdotq_u32(acc0, v.val[0], ones);
                    acc1 = vdotq_u32(acc1, v.val[1], ones);
                    acc2 = vdotq_u32(acc2, v.val[2], ones);
                    acc3 = vdotq_u32(acc3, v.val[3], ones);
                }
                sums[0] = (صحیح)vaddvq_u32(acc0);
                sums[1] = (صحیح)vaddvq_u32(acc1);
                sums[2] = (صحیح)vaddvq_u32(acc2);
                sums[3] = (صحیح)vaddvq_u32(acc3);
                برائے (; x < ستون_تعداد; x++)
                {
                    sums[0] += (صحیح)src[x * 4];
                    sums[1] += (صحیح)src[x * 4 + 1];
                    sums[2] += (صحیح)src[x * 4 + 2];
                    sums[3] += (صحیح)src[x * 4 + 3];
                }
#وگرنہ_اگر CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
                صحیح x = 0;
                برائے (; x <= ستون_تعداد - 16; x += 16)
                {
                    uint8x16x4_t v = vld4q_u8(src + x * 4);
                    sums[0] += (صحیح)vaddlvq_u8(v.val[0]);
                    sums[1] += (صحیح)vaddlvq_u8(v.val[1]);
                    sums[2] += (صحیح)vaddlvq_u8(v.val[2]);
                    sums[3] += (صحیح)vaddlvq_u8(v.val[3]);
                }
                برائے (; x < ستون_تعداد; x++)
                {
                    sums[0] += (صحیح)src[x * 4];
                    sums[1] += (صحیح)src[x * 4 + 1];
                    sums[2] += (صحیح)src[x * 4 + 2];
                    sums[3] += (صحیح)src[x * 4 + 3];
                }
#وگرنہ_اگر CV_AVX2
                // Intel AVX2: shuffle + unpack to group channels, full-register SAD
                ثابت __m256i zero = _mm256_setzero_si256();
                ثابت __m256i shuf_نقاب = _mm256_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15,
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                صحیح x = 0;
                // 16 pixels/iter with unpack (full-register SAD)
                {
                    __m256i مجموع_bg = zero, مجموع_ra = zero;
                    برائے (; x <= ستون_تعداد - 16; x += 16)
                    {
                        __m256i v0 = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4));
                        __m256i v1 = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4 + 32));
                        __m256i g0 = _mm256_shuffle_epi8(v0, shuf_نقاب);
                        __m256i g1 = _mm256_shuffle_epi8(v1, shuf_نقاب);
                        __m256i bg = _mm256_unpacklo_epi32(g0, g1);
                        __m256i ra = _mm256_unpackhi_epi32(g0, g1);
                        مجموع_bg = mm256_جوڑو_epi64(مجموع_bg, _mm256_sad_epu8(bg, zero));
                        مجموع_ra = mm256_جوڑو_epi64(مجموع_ra, _mm256_sad_epu8(ra, zero));
                    }
                    __m128i bg_lo = _mm256_castsi256_si128(مجموع_bg);
                    __m128i bg_hi = _mm256_extracti128_si256(مجموع_bg, 1);
                    __m128i bg_s = mm_جوڑو_epi64(bg_lo, bg_hi);
                    __m128i ra_lo = _mm256_castsi256_si128(مجموع_ra);
                    __m128i ra_hi = _mm256_extracti128_si256(مجموع_ra, 1);
                    __m128i ra_s = mm_جوڑو_epi64(ra_lo, ra_hi);
                    sums[0] = _mm_cvtsi128_si32(bg_s);                     // B
                    sums[1] = _mm_cvtsi128_si32(_mm_srli_si128(bg_s, 8));  // G
                    sums[2] = _mm_cvtsi128_si32(ra_s);                     // R
                    sums[3] = _mm_cvtsi128_si32(_mm_srli_si128(ra_s, 8));  // A
                }
                // 8-pixel tail with and/srli
                {
                    ثابت __m256i نقاب_lo32 = _mm256_set1_epi64x(0x00000000FFFFFFFFLL);
                    __m256i مجموع_br = zero, مجموع_ga = zero;
                    برائے (; x <= ستون_تعداد - 8; x += 8)
                    {
                        __m256i v = _mm256_loadu_si256((ثابت __m256i*)(src + x * 4));
                        __m256i grouped = _mm256_shuffle_epi8(v, shuf_نقاب);
                        __m256i br = _mm256_and_si256(grouped, نقاب_lo32);
                        __m256i ga = _mm256_srli_epi64(grouped, 32);
                        مجموع_br = mm256_جوڑو_epi64(مجموع_br, _mm256_sad_epu8(br, zero));
                        مجموع_ga = mm256_جوڑو_epi64(مجموع_ga, _mm256_sad_epu8(ga, zero));
                    }
                    __m128i br_lo = _mm256_castsi256_si128(مجموع_br);
                    __m128i br_hi = _mm256_extracti128_si256(مجموع_br, 1);
                    __m128i br_s = mm_جوڑو_epi64(br_lo, br_hi);
                    __m128i ga_lo = _mm256_castsi256_si128(مجموع_ga);
                    __m128i ga_hi = _mm256_extracti128_si256(مجموع_ga, 1);
                    __m128i ga_s = mm_جوڑو_epi64(ga_lo, ga_hi);
                    sums[0] += _mm_cvtsi128_si32(br_s);
                    sums[1] += _mm_cvtsi128_si32(ga_s);
                    sums[2] += _mm_cvtsi128_si32(_mm_srli_si128(br_s, 8));
                    sums[3] += _mm_cvtsi128_si32(_mm_srli_si128(ga_s, 8));
                }
                برائے (; x < ستون_تعداد; x++)
                {
                    sums[0] += (صحیح)src[x * 4];
                    sums[1] += (صحیح)src[x * 4 + 1];
                    sums[2] += (صحیح)src[x * 4 + 2];
                    sums[3] += (صحیح)src[x * 4 + 3];
                }
#وگرنہ_اگر CV_SSSE3
                // Intel SSSE3: _mm_shuffle_epi8 deinterleave + _mm_sad_epu8
                ثابت __m128i zero = _mm_setzero_si128();
                ثابت __m128i shuf_نقاب = _mm_setr_epi8(
                    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15);
                ثابت __m128i نقاب_lo32 = _mm_set1_epi64x(0x00000000FFFFFFFFLL);
                __m128i مجموع_br = zero, مجموع_ga = zero;
                صحیح x = 0;
                برائے (; x <= ستون_تعداد - 4; x += 4)
                {
                    __m128i v = _mm_loadu_si128((ثابت __m128i*)(src + x * 4));
                    __m128i grouped = _mm_shuffle_epi8(v, shuf_نقاب);
                    __m128i br = _mm_and_si128(grouped, نقاب_lo32);
                    __m128i ga = _mm_srli_epi64(grouped, 32);
                    مجموع_br = mm_جوڑو_epi64(مجموع_br, _mm_sad_epu8(br, zero));
                    مجموع_ga = mm_جوڑو_epi64(مجموع_ga, _mm_sad_epu8(ga, zero));
                }
                sums[0] = _mm_cvtsi128_si32(مجموع_br);
                sums[1] = _mm_cvtsi128_si32(مجموع_ga);
                sums[2] = _mm_cvtsi128_si32(_mm_srli_si128(مجموع_br, 8));
                sums[3] = _mm_cvtsi128_si32(_mm_srli_si128(مجموع_ga, 8));
                برائے (; x < ستون_تعداد; x++)
                {
                    sums[0] += (صحیح)src[x * 4];
                    sums[1] += (صحیح)src[x * 4 + 1];
                    sums[2] += (صحیح)src[x * 4 + 2];
                    sums[3] += (صحیح)src[x * 4 + 3];
                }
#ورنہ
                ثابت صحیح vlanes8 = VTraits<v_uint8>::vlanes();
                v_uint32 vsum0 = vx_setzero_u32(), vsum1 = vx_setzero_u32();
                v_uint32 vsum2 = vx_setzero_u32(), vsum3 = vx_setzero_u32();
                v_uint16 vs16_0 = vx_setzero_u16(), vs16_1 = vx_setzero_u16();
                v_uint16 vs16_2 = vx_setzero_u16(), vs16_3 = vx_setzero_u16();
                صحیح x = 0, batch = 0;
                ثابت صحیح flush_at = 128;

                برائے (; x <= ستون_تعداد - vlanes8; x += vlanes8)
                {
                    v_uint8 ch0, ch1, ch2, ch3;
                    v_بھرو_deinterleave(src + x * 4, ch0, ch1, ch2, ch3);

                    v_uint16 lo, hi;
                    v_expand(ch0, lo, hi); vs16_0 = v_جوڑو(vs16_0, v_جوڑو(lo, hi));
                    v_expand(ch1, lo, hi); vs16_1 = v_جوڑو(vs16_1, v_جوڑو(lo, hi));
                    v_expand(ch2, lo, hi); vs16_2 = v_جوڑو(vs16_2, v_جوڑو(lo, hi));
                    v_expand(ch3, lo, hi); vs16_3 = v_جوڑو(vs16_3, v_جوڑو(lo, hi));

                    اگر (++batch >= flush_at)
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

                sums[0] = (صحیح)v_reduce_مجموع(vsum0);
                sums[1] = (صحیح)v_reduce_مجموع(vsum1);
                sums[2] = (صحیح)v_reduce_مجموع(vsum2);
                sums[3] = (صحیح)v_reduce_مجموع(vsum3);

                برائے (; x < ستون_تعداد; x++)
                {
                    sums[0] += (صحیح)src[x * 4];
                    sums[1] += (صحیح)src[x * 4 + 1];
                    sums[2] += (صحیح)src[x * 4 + 2];
                    sums[3] += (صحیح)src[x * 4 + 3];
                }
#اگر_ختم
            }
            ورنہ
            {
                برائے (صحیح x = 0, c = 0; x < چوڑائی; x++, c = (c+1)&-(c < cn-1))
                    sums[c] += (صحیح)src[x];
            }
            برائے (صحیح c = 0; c < cn; c++)
                dst[c] = (اعشاری)sums[c];
        }
    };
    parallel_for_(Range(0, srcmat.صفیں), body);
    v_cleanup();
}

// --- 16-bit (ushort/short) → float ---
سانچہ<قسم_نام SrcT, قسم_نام VecSrc, قسم_نام VecDst>
ساکن خالی reduce_ستون_مجموع_16_32f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحیح cn = srcmat.channels();
    ثابت صحیح چوڑائی = srcmat.ستون_تعداد * cn;
    ثابت صحیح vlanes = VTraits<VecSrc>::vlanes();

    خودکار body = [&](ثابت Range& range) {
        برائے (صحیح y = range.آغاز; y < range.end; y++)
        {
            ثابت SrcT* src = srcmat.ptr<SrcT>(y);
            اعشاری* dst = dstmat.ptr<اعشاری>(y);

            اگر (cn == 1)
            {
                VecDst v_sum0 = v_setzero_<VecDst>(), v_sum1 = v_setzero_<VecDst>();
                صحیح x = 0;
                برائے (; x <= چوڑائی - vlanes; x += vlanes)
                {
                    VecDst lo, hi;
                    v_expand(vx_بھرو(src + x), lo, hi);
                    v_sum0 = v_جوڑو(v_sum0, lo);
                    v_sum1 = v_جوڑو(v_sum1, hi);
                }
                dst[0] = (اعشاری)(v_reduce_مجموع(v_sum0) + v_reduce_مجموع(v_sum1));
                برائے (; x < چوڑائی; x++)
                    dst[0] += (اعشاری)src[x];
            }
            ورنہ
            {
                برائے (صحیح c = 0; c < cn; c++)
                    dst[c] = 0;
                برائے (صحیح x = 0, c = 0; x < چوڑائی; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += (اعشاری)src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.صفیں), body);
    v_cleanup();
}

ساکن خالی reduce_ستون_مجموع_16u32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_ستون_مجموع_16_32f<ushort, v_uint16, v_uint32>(srcmat, dstmat); }

ساکن خالی reduce_ستون_مجموع_16s32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_ستون_مجموع_16_32f<مختصر, v_int16, v_int32>(srcmat, dstmat); }

// --- float/double → same type ---
سانچہ<قسم_نام T, قسم_نام VecT>
ساکن خالی reduce_ستون_مجموع_FP(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحیح cn = srcmat.channels();
    ثابت صحیح چوڑائی = srcmat.ستون_تعداد * cn;
    ثابت صحیح vlanes = VTraits<VecT>::vlanes();

    خودکار body = [&](ثابت Range& range) {
        برائے (صحیح y = range.آغاز; y < range.end; y++)
        {
            ثابت T* src = srcmat.ptr<T>(y);
            T* dst = dstmat.ptr<T>(y);

            اگر (cn == 1)
            {
                VecT s0 = v_setzero_<VecT>(), s1 = v_setzero_<VecT>();
                VecT s2 = v_setzero_<VecT>(), s3 = v_setzero_<VecT>();
                صحیح x = 0;
                برائے (; x <= چوڑائی - vlanes * 4; x += vlanes * 4)
                {
                    s0 = v_جوڑو(s0, vx_بھرو(src + x));
                    s1 = v_جوڑو(s1, vx_بھرو(src + x + vlanes));
                    s2 = v_جوڑو(s2, vx_بھرو(src + x + vlanes * 2));
                    s3 = v_جوڑو(s3, vx_بھرو(src + x + vlanes * 3));
                }
                s0 = v_جوڑو(v_جوڑو(s0, s1), v_جوڑو(s2, s3));
                برائے (; x <= چوڑائی - vlanes; x += vlanes)
                    s0 = v_جوڑو(s0, vx_بھرو(src + x));
                T کل = (T)v_reduce_مجموع(s0);
                برائے (; x < چوڑائی; x++)
                    کل += src[x];
                dst[0] = کل;
            }
            ورنہ
            {
                برائے (صحیح c = 0; c < cn; c++)
                    dst[c] = 0;
                برائے (صحیح x = 0, c = 0; x < چوڑائی; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.صفیں), body);
    v_cleanup();
}

ساکن خالی reduce_ستون_مجموع_32f32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_ستون_مجموع_FP<اعشاری, v_float32>(srcmat, dstmat); }

#اگر CV_SIMD_64F
// --- float → double ---
ساکن خالی reduce_ستون_مجموع_32f64f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحیح cn = srcmat.channels();
    ثابت صحیح چوڑائی = srcmat.ستون_تعداد * cn;
    ثابت صحیح vlanes32 = VTraits<v_float32>::vlanes();

    خودکار body = [&](ثابت Range& range) {
        برائے (صحیح y = range.آغاز; y < range.end; y++)
        {
            ثابت اعشاری* src = srcmat.ptr<اعشاری>(y);
            دوہرا* dst = dstmat.ptr<دوہرا>(y);

            اگر (cn == 1)
            {
                v_float64 v_sum0 = vx_setzero_f64();
                v_float64 v_sum1 = vx_setzero_f64();
                صحیح x = 0;
                برائے (; x <= چوڑائی - vlanes32; x += vlanes32)
                {
                    v_float32 v = vx_بھرو(src + x);
                    v_sum0 = v_جوڑو(v_sum0, v_cvt_f64(v));
                    v_sum1 = v_جوڑو(v_sum1, v_cvt_f64_بلند(v));
                }
                v_float64 v_کل = v_جوڑو(v_sum0, v_sum1);

                دوہرا کل = v_reduce_مجموع(v_کل);
                برائے (; x < چوڑائی; x++)
                    کل += (دوہرا)src[x];
                dst[0] = کل;
            }
            ورنہ
            {
                برائے (صحیح c = 0; c < cn; c++)
                    dst[c] = 0;
                برائے (صحیح x = 0, c = 0; x < چوڑائی; x++, c = (c+1)&-(c < cn-1))
                    dst[c] += (دوہرا)src[x];
            }
        }
    };
    parallel_for_(Range(0, srcmat.صفیں), body);
    v_cleanup();
}

ساکن خالی reduce_ستون_مجموع_64f64f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_ستون_مجموع_FP<دوہرا, v_float64>(srcmat, dstmat); }
#اگر_ختم // CV_SIMD_64F

// =====================================================================
//  Row reduce SUM (dim=0): sum each column across all rows
// =====================================================================

// --- uchar → int ---
// Uses u16 intermediate accumulator for vertical sum: accumulate u8→u16 per row,
// flush u16→u32 every 256 rows (max u16 value: 256*255=65280 < 65535)
ساکن خالی reduce_صف_مجموع_8u32s(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحیح چوڑائی_cn = srcmat.ستون_تعداد * srcmat.channels();
    ثابت صحیح اونچائی = srcmat.صفیں;
    ثابت صحیح vlanes16 = VTraits<v_uint16>::vlanes();
    ثابت صحیح vlanes32 = VTraits<v_int32>::vlanes();

    خودکار body = [&](ثابت Range& range) {
        ثابت صحیح آغاز = range.آغاز;
        ثابت صحیح end = range.end;
        ثابت صحیح len = end - آغاز;

        Auto_بفر<صحیح> buf32_storage(len);
        Auto_بفر<ushort> buf16_storage(len);
        صحیح* buf32 = buf32_storage.ڈیٹا();
        ushort* buf16 = buf16_storage.ڈیٹا();

        // init from first row
        ثابت uchar* src0 = srcmat.ptr<uchar>(0) + آغاز;
        برائے (صحیح i = 0; i < len; i++)
            buf32[i] = (صحیح)src0[i];

        اگر (اونچائی <= 1)
        {
            صحیح* dst = dstmat.ptr<صحیح>(0) + آغاز;
            برائے (صحیح i = 0; i < len; i++)
                dst[i] = buf32[i];
            واپس;
        }

        memset(buf16, 0, len * حجم(ushort));
        ثابت صحیح flush_at = 256; // 256*255 = 65280 < 65535 (u16 safe)
        صحیح flush_گنتی = 0;

        برائے (صحیح صف = 1; صف < اونچائی; صف++)
        {
            ثابت uchar* src = srcmat.ptr<uchar>(صف) + آغاز;
            صحیح i = 0;

#اگر CV_NEON && (defined(__aarch64__) || defined(_M_ARM64))
            // AArch64: vaddw_u8 — single-instruction widening add (u16 += u8)
            برائے (; i <= len - 16; i += 16)
            {
                uint8x16_t v = vld1q_u8(src + i);
                uint16x8_t acc_lo = vld1q_u16(buf16 + i);
                uint16x8_t acc_hi = vld1q_u16(buf16 + i + 8);
                vst1q_u16(buf16 + i,     vaddw_u8(acc_lo, vget_کم_u8(v)));
                vst1q_u16(buf16 + i + 8, vaddw_بلند_u8(acc_hi, v));
            }
#وگرنہ_اگر CV_RVV
            ثابت صحیح vl = (صحیح)__riscv_vsetvlmax_e8m2();
            برائے (; i <= len - vl; i += vl)
            {
                vuint8m2_t vs = __riscv_vle8_v_u8m2(src + i, vl);
                vuint16m4_t va = __riscv_vle16_v_u16m4(buf16 + i, vl);
                va = __riscv_vwaddu_wv_u16m4(va, vs, vl);
                __riscv_vse16_v_u16m4(buf16 + i, va, vl);
            }
#ورنہ
            ثابت صحیح vlanes8 = VTraits<v_uint8>::vlanes();
            برائے (; i <= len - vlanes8; i += vlanes8)
            {
                v_uint16 lo, hi;
                v_expand(vx_بھرو(src + i), lo, hi);
                v_store(buf16 + i, v_جوڑو(vx_بھرو(buf16 + i), lo));
                v_store(buf16 + i + vlanes16, v_جوڑو(vx_بھرو(buf16 + i + vlanes16), hi));
            }
#اگر_ختم
            برائے (; i < len; i++)
                buf16[i] += (ushort)src[i];

            اگر (++flush_گنتی >= flush_at || صف == اونچائی - 1)
            {
                // flush u16 → u32
                صحیح j = 0;
                برائے (; j <= len - vlanes16; j += vlanes16)
                {
                    v_uint32 lo, hi;
                    v_expand(vx_بھرو(buf16 + j), lo, hi);
                    v_store(buf32 + j, v_جوڑو(vx_بھرو(buf32 + j), v_reinterpret_as_s32(lo)));
                    v_store(buf32 + j + vlanes32, v_جوڑو(vx_بھرو(buf32 + j + vlanes32), v_reinterpret_as_s32(hi)));
                }
                برائے (; j < len; j++)
                    buf32[j] += (صحیح)buf16[j];
                memset(buf16, 0, len * حجم(ushort));
                flush_گنتی = 0;
            }
        }

        صحیح* dst = dstmat.ptr<صحیح>(0) + آغاز;
        برائے (صحیح i = 0; i < len; i++)
            dst[i] = buf32[i];
    };

    parallel_for_(Range(0, چوڑائی_cn), body, چوڑائی_cn * CV_ELEM_سائز(srcmat.گہرائی()) / 64);
    v_cleanup();
}

// --- uchar → float ---
ساکن خالی reduce_صف_مجموع_8u32f(ثابت Mat& srcmat, Mat& dstmat)
{
    // compute in int, then convert to float
    Mat عارضی_2(dstmat.صفیں, dstmat.ستون_تعداد, CV_32SC(srcmat.channels()));
    reduce_صف_مجموع_8u32s(srcmat, عارضی_2);
    عارضی_2.convert_مقصد(dstmat, dstmat.type());
}

// --- 16-bit (ushort/short) → float row reduce ---
سانچہ<قسم_نام SrcT, قسم_نام VecSrc, قسم_نام VecDst>
ساکن خالی reduce_صف_مجموع_16_32f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحیح چوڑائی_cn = srcmat.ستون_تعداد * srcmat.channels();
    ثابت صحیح اونچائی = srcmat.صفیں;
    ثابت صحیح vlanes16 = VTraits<VecSrc>::vlanes();
    ثابت صحیح vlanes32 = vlanes16 / 2;

    خودکار body = [&](ثابت Range& range) {
        ثابت صحیح آغاز = range.آغاز;
        ثابت صحیح end = range.end;
        ثابت صحیح len = end - آغاز;
        اعشاری* dst = dstmat.ptr<اعشاری>(0) + آغاز;

        صحیح i = 0;
        برائے (; i <= len - vlanes16 * 4; i += vlanes16 * 4)
        {
            ثابت SrcT* src0 = srcmat.ptr<SrcT>(0) + آغاز + i;
            VecDst a0, a1, a2, a3, a4, a5, a6, a7;
            v_expand(vx_بھرو(src0), a0, a1);
            v_expand(vx_بھرو(src0 + vlanes16), a2, a3);
            v_expand(vx_بھرو(src0 + vlanes16*2), a4, a5);
            v_expand(vx_بھرو(src0 + vlanes16*3), a6, a7);
            برائے (صحیح صف = 1; صف < اونچائی; صف++)
            {
                ثابت SrcT* src = srcmat.ptr<SrcT>(صف) + آغاز + i;
                VecDst lo, hi;
                v_expand(vx_بھرو(src), lo, hi);
                a0 = v_جوڑو(a0, lo); a1 = v_جوڑو(a1, hi);
                v_expand(vx_بھرو(src + vlanes16), lo, hi);
                a2 = v_جوڑو(a2, lo); a3 = v_جوڑو(a3, hi);
                v_expand(vx_بھرو(src + vlanes16*2), lo, hi);
                a4 = v_جوڑو(a4, lo); a5 = v_جوڑو(a5, hi);
                v_expand(vx_بھرو(src + vlanes16*3), lo, hi);
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
        برائے (; i <= len - vlanes16; i += vlanes16)
        {
            VecDst a0, a1;
            v_expand(vx_بھرو(srcmat.ptr<SrcT>(0) + آغاز + i), a0, a1);
            برائے (صحیح صف = 1; صف < اونچائی; صف++)
            {
                VecDst lo, hi;
                v_expand(vx_بھرو(srcmat.ptr<SrcT>(صف) + آغاز + i), lo, hi);
                a0 = v_جوڑو(a0, lo); a1 = v_جوڑو(a1, hi);
            }
            v_store(dst + i, v_cvt_f32(v_reinterpret_as_s32(a0)));
            v_store(dst + i + vlanes32, v_cvt_f32(v_reinterpret_as_s32(a1)));
        }
        برائے (; i < len; i++)
        {
            صحیح val = (صحیح)*(srcmat.ptr<SrcT>(0) + آغاز + i);
            برائے (صحیح صف = 1; صف < اونچائی; صف++)
                val += (صحیح)*(srcmat.ptr<SrcT>(صف) + آغاز + i);
            dst[i] = (اعشاری)val;
        }
    };

    parallel_for_(Range(0, چوڑائی_cn), body, چوڑائی_cn * CV_ELEM_سائز(srcmat.گہرائی()) / 64);
    v_cleanup();
}

ساکن خالی reduce_صف_مجموع_16u32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_صف_مجموع_16_32f<ushort, v_uint16, v_uint32>(srcmat, dstmat); }

ساکن خالی reduce_صف_مجموع_16s32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_صف_مجموع_16_32f<مختصر, v_int16, v_int32>(srcmat, dstmat); }

// --- float/double → same type row reduce ---
سانچہ<قسم_نام T, قسم_نام VecT>
ساکن خالی reduce_صف_مجموع_FP(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحیح چوڑائی_cn = srcmat.ستون_تعداد * srcmat.channels();
    ثابت صحیح اونچائی = srcmat.صفیں;
    ثابت صحیح vlanes = VTraits<VecT>::vlanes();

    خودکار body = [&](ثابت Range& range) {
        ثابت صحیح آغاز = range.آغاز;
        ثابت صحیح end = range.end;
        ثابت صحیح len = end - آغاز;
        T* dst = dstmat.ptr<T>(0) + آغاز;

        صحیح i = 0;
        برائے (; i <= len - vlanes * 8; i += vlanes * 8)
        {
            ثابت T* src0 = srcmat.ptr<T>(0) + آغاز + i;
            VecT s0 = vx_بھرو(src0), s1 = vx_بھرو(src0 + vlanes);
            VecT s2 = vx_بھرو(src0 + vlanes*2), s3 = vx_بھرو(src0 + vlanes*3);
            VecT s4 = vx_بھرو(src0 + vlanes*4), s5 = vx_بھرو(src0 + vlanes*5);
            VecT s6 = vx_بھرو(src0 + vlanes*6), s7 = vx_بھرو(src0 + vlanes*7);
            برائے (صحیح صف = 1; صف < اونچائی; صف++)
            {
                ثابت T* src = srcmat.ptr<T>(صف) + آغاز + i;
                s0 = v_جوڑو(s0, vx_بھرو(src));
                s1 = v_جوڑو(s1, vx_بھرو(src + vlanes));
                s2 = v_جوڑو(s2, vx_بھرو(src + vlanes*2));
                s3 = v_جوڑو(s3, vx_بھرو(src + vlanes*3));
                s4 = v_جوڑو(s4, vx_بھرو(src + vlanes*4));
                s5 = v_جوڑو(s5, vx_بھرو(src + vlanes*5));
                s6 = v_جوڑو(s6, vx_بھرو(src + vlanes*6));
                s7 = v_جوڑو(s7, vx_بھرو(src + vlanes*7));
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
        برائے (; i <= len - vlanes; i += vlanes)
        {
            VecT s0 = vx_بھرو(srcmat.ptr<T>(0) + آغاز + i);
            برائے (صحیح صف = 1; صف < اونچائی; صف++)
                s0 = v_جوڑو(s0, vx_بھرو(srcmat.ptr<T>(صف) + آغاز + i));
            v_store(dst + i, s0);
        }
        برائے (; i < len; i++)
        {
            T val = *(srcmat.ptr<T>(0) + آغاز + i);
            برائے (صحیح صف = 1; صف < اونچائی; صف++)
                val += *(srcmat.ptr<T>(صف) + آغاز + i);
            dst[i] = val;
        }
    };

    parallel_for_(Range(0, چوڑائی_cn), body, چوڑائی_cn * CV_ELEM_سائز(srcmat.گہرائی()) / 64);
    v_cleanup();
}

ساکن خالی reduce_صف_مجموع_32f32f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_صف_مجموع_FP<اعشاری, v_float32>(srcmat, dstmat); }

#اگر CV_SIMD_64F
// --- float → double ---
ساکن خالی reduce_صف_مجموع_32f64f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحیح چوڑائی_cn = srcmat.ستون_تعداد * srcmat.channels();
    ثابت صحیح اونچائی = srcmat.صفیں;
    ثابت صحیح vlanes32 = VTraits<v_float32>::vlanes();
    ثابت صحیح vlanes64 = VTraits<v_float64>::vlanes();

    خودکار body = [&](ثابت Range& range) {
        ثابت صحیح آغاز = range.آغاز;
        ثابت صحیح end = range.end;
        ثابت صحیح len = end - آغاز;

        Auto_بفر<دوہرا> buf_storage(len);
        دوہرا* buf = buf_storage.ڈیٹا();

        ثابت اعشاری* src0 = srcmat.ptr<اعشاری>(0) + آغاز;
        برائے (صحیح i = 0; i < len; i++)
            buf[i] = (دوہرا)src0[i];

        برائے (صحیح صف = 1; صف < اونچائی; صف++)
        {
            ثابت اعشاری* src = srcmat.ptr<اعشاری>(صف) + آغاز;
            صحیح i = 0;
            برائے (; i <= len - vlanes32; i += vlanes32)
            {
                v_float32 v = vx_بھرو(src + i);
                v_store(buf + i, v_جوڑو(vx_بھرو(buf + i), v_cvt_f64(v)));
                v_store(buf + i + vlanes64, v_جوڑو(vx_بھرو(buf + i + vlanes64), v_cvt_f64_بلند(v)));
            }
            برائے (; i < len; i++)
                buf[i] += (دوہرا)src[i];
        }

        دوہرا* dst = dstmat.ptr<دوہرا>(0) + آغاز;
        حافظہ_نقل(dst, buf, len * حجم(دوہرا));
    };

    parallel_for_(Range(0, چوڑائی_cn), body, چوڑائی_cn * CV_ELEM_سائز(srcmat.گہرائی()) / 64);
    v_cleanup();
}

ساکن خالی reduce_صف_مجموع_64f64f(ثابت Mat& srcmat, Mat& dstmat)
{ reduce_صف_مجموع_FP<دوہرا, v_float64>(srcmat, dstmat); }
#اگر_ختم // CV_SIMD_64F

#اگر_ختم // CV_SIMD || CV_SIMD_SCALABLE

#اگر CV_SIMD_SCALABLE
ساکن اندرونی v_float32 reduce_صف_Sum2_Acc4(ثابت v_float32& acc,
                                           ثابت اعشاری* src0, ثابت اعشاری* src1,
                                           ثابت اعشاری* src2, ثابت اعشاری* src3)
{
    v_float32 مجموع = acc;
    v_float32 x = vx_بھرو(src0);
    مجموع = v_fma(x, x, مجموع);
    x = vx_بھرو(src1);
    مجموع = v_fma(x, x, مجموع);
    x = vx_بھرو(src2);
    مجموع = v_fma(x, x, مجموع);
    x = vx_بھرو(src3);
    واپس v_fma(x, x, مجموع);
}

// dim=0, REDUCE_SUM2 for CV_32F using scalable universal intrinsics.
ساکن خالی reduce_صف_Sum2_32f32f(ثابت Mat& srcmat, Mat& dstmat)
{
    ثابت صحیح چوڑائی = srcmat.ستون_تعداد * srcmat.channels();
    ثابت صحیح اونچائی = srcmat.صفیں;
    ثابت صحیح vlanes = VTraits<v_float32>::vlanes();
    ثابت صحیح بلاک_2 = vlanes * 4;
    Auto_بفر<اعشاری> بفر(چوڑائی);
    اعشاری* buf = بفر.ڈیٹا();

    ثابت اعشاری* src = srcmat.ptr<اعشاری>(0);
    صحیح i = 0;
    برائے (; i <= چوڑائی - بلاک_2; i += بلاک_2)
    {
        v_float32 x0 = vx_بھرو(src + i);
        v_float32 x1 = vx_بھرو(src + i + vlanes);
        v_float32 x2 = vx_بھرو(src + i + vlanes * 2);
        v_float32 x3 = vx_بھرو(src + i + vlanes * 3);
        v_store(buf + i, v_mul(x0, x0));
        v_store(buf + i + vlanes, v_mul(x1, x1));
        v_store(buf + i + vlanes * 2, v_mul(x2, x2));
        v_store(buf + i + vlanes * 3, v_mul(x3, x3));
    }
    برائے (; i <= چوڑائی - vlanes; i += vlanes)
    {
        v_float32 x = vx_بھرو(src + i);
        v_store(buf + i, v_mul(x, x));
    }
    برائے (; i < چوڑائی; i++)
        buf[i] = src[i] * src[i];

    صحیح صف = 1;
    // Four rows per pass amortize buffer traffic while preserving source-row
    // accumulation order. The outer loop stays serial: splitting width into
    // tiny parallel stripes under-fills scalable vectors and adds scheduling
    // overhead for this memory-streaming kernel.
    برائے (; صف <= اونچائی - 4; صف += 4)
    {
        ثابت اعشاری* src0 = srcmat.ptr<اعشاری>(صف);
        ثابت اعشاری* src1 = srcmat.ptr<اعشاری>(صف + 1);
        ثابت اعشاری* src2 = srcmat.ptr<اعشاری>(صف + 2);
        ثابت اعشاری* src3 = srcmat.ptr<اعشاری>(صف + 3);

        i = 0;
        برائے (; i <= چوڑائی - بلاک_2; i += بلاک_2)
        {
            v_float32 sum0 = vx_بھرو(buf + i);
            v_float32 sum1 = vx_بھرو(buf + i + vlanes);
            v_float32 sum2 = vx_بھرو(buf + i + vlanes * 2);
            v_float32 sum3 = vx_بھرو(buf + i + vlanes * 3);
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
        برائے (; i <= چوڑائی - vlanes; i += vlanes)
        {
            v_float32 مجموع = vx_بھرو(buf + i);
            مجموع = reduce_صف_Sum2_Acc4(مجموع, src0 + i, src1 + i, src2 + i, src3 + i);
            v_store(buf + i, مجموع);
        }
        برائے (; i < چوڑائی; i++)
        {
            اعشاری مجموع = buf[i];
            اعشاری x = src0[i]; مجموع += x * x;
            x = src1[i]; مجموع += x * x;
            x = src2[i]; مجموع += x * x;
            x = src3[i]; مجموع += x * x;
            buf[i] = مجموع;
        }
    }

    برائے (; صف < اونچائی; صف++)
    {
        src = srcmat.ptr<اعشاری>(صف);
        i = 0;
        برائے (; i <= چوڑائی - بلاک_2; i += بلاک_2)
        {
            v_float32 sum0 = vx_بھرو(buf + i);
            v_float32 sum1 = vx_بھرو(buf + i + vlanes);
            v_float32 sum2 = vx_بھرو(buf + i + vlanes * 2);
            v_float32 sum3 = vx_بھرو(buf + i + vlanes * 3);
            v_float32 x0 = vx_بھرو(src + i);
            v_float32 x1 = vx_بھرو(src + i + vlanes);
            v_float32 x2 = vx_بھرو(src + i + vlanes * 2);
            v_float32 x3 = vx_بھرو(src + i + vlanes * 3);
            v_store(buf + i, v_fma(x0, x0, sum0));
            v_store(buf + i + vlanes, v_fma(x1, x1, sum1));
            v_store(buf + i + vlanes * 2, v_fma(x2, x2, sum2));
            v_store(buf + i + vlanes * 3, v_fma(x3, x3, sum3));
        }
        برائے (; i <= چوڑائی - vlanes; i += vlanes)
        {
            v_float32 x = vx_بھرو(src + i);
            v_store(buf + i, v_fma(x, x, vx_بھرو(buf + i)));
        }
        برائے (; i < چوڑائی; i++)
            buf[i] += src[i] * src[i];
    }

    // Delay output stores until all source rows are consumed, preserving the
    // existing overlap-safe behavior of the proposed optimized path.
    حافظہ_نقل(dstmat.ptr<اعشاری>(), buf, چوڑائی * حجم(اعشاری));
    v_cleanup();
}
#اگر_ختم

// =====================================================================
//  Dispatchers
// =====================================================================

Reduce_مجموع_Func getReduceCSumFunc(صحیح sdepth, صحیح ddepth)
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر (sdepth == CV_8U && ddepth == CV_32S) واپس reduce_ستون_مجموع_8u32s;
    اگر (sdepth == CV_8U && ddepth == CV_32F) واپس reduce_ستون_مجموع_8u32f;
    اگر (sdepth == CV_16U && ddepth == CV_32F) واپس reduce_ستون_مجموع_16u32f;
    اگر (sdepth == CV_16S && ddepth == CV_32F) واپس reduce_ستون_مجموع_16s32f;
    اگر (sdepth == CV_32F && ddepth == CV_32F) واپس reduce_ستون_مجموع_32f32f;
#اگر CV_SIMD_64F
    اگر (sdepth == CV_32F && ddepth == CV_64F) واپس reduce_ستون_مجموع_32f64f;
    اگر (sdepth == CV_64F && ddepth == CV_64F) واپس reduce_ستون_مجموع_64f64f;
#اگر_ختم
#ورنہ
    CV_غیر_مستعمل(sdepth);
    CV_غیر_مستعمل(ddepth);
#اگر_ختم
    واپس خالی_اشاریہ;
}

Reduce_مجموع_Func getReduceRSumFunc(صحیح sdepth, صحیح ddepth)
{
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر (sdepth == CV_8U && ddepth == CV_32S) واپس reduce_صف_مجموع_8u32s;
    اگر (sdepth == CV_8U && ddepth == CV_32F) واپس reduce_صف_مجموع_8u32f;
    اگر (sdepth == CV_16U && ddepth == CV_32F) واپس reduce_صف_مجموع_16u32f;
    اگر (sdepth == CV_16S && ddepth == CV_32F) واپس reduce_صف_مجموع_16s32f;
    اگر (sdepth == CV_32F && ddepth == CV_32F) واپس reduce_صف_مجموع_32f32f;
#اگر CV_SIMD_64F
    اگر (sdepth == CV_32F && ddepth == CV_64F) واپس reduce_صف_مجموع_32f64f;
    اگر (sdepth == CV_64F && ddepth == CV_64F) واپس reduce_صف_مجموع_64f64f;
#اگر_ختم
#ورنہ
    CV_غیر_مستعمل(sdepth);
    CV_غیر_مستعمل(ddepth);
#اگر_ختم
    واپس خالی_اشاریہ;
}

Reduce_مجموع_Func getReduceRSum2Func(صحیح sdepth, صحیح ddepth)
{
#اگر CV_SIMD_SCALABLE
    اگر (sdepth == CV_32F && ddepth == CV_32F) واپس reduce_صف_Sum2_32f32f;
#ورنہ
    CV_غیر_مستعمل(sdepth);
    CV_غیر_مستعمل(ddepth);
#اگر_ختم
    واپس خالی_اشاریہ;
}

#اگر_ختم // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_انتہا
} // namespace cv
