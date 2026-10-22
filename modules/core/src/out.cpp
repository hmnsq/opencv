/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਨਾਮਾਕਾਸ਼ cv
{
    ਵਰਗ FormattedImpl CV_FINAL : ਜਨਤਕ Formatted
    {
        ਗਿਣਤੀ { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ਪੰਕਤੀ_OPEN, STATE_ਪੰਕਤੀ_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_ਮੁੱਲ, STATE_ਮੁਕੰਮਲ,
               STATE_ਸਤਰ_SEPARATOR, STATE_CN_SEPARATOR, STATE_ਮੁੱਲ_SEPARATOR };
        ਗਿਣਤੀ {BRACE_ਪੰਕਤੀ_OPEN = 0, BRACE_ਪੰਕਤੀ_CLOSE = 1, BRACE_ਪੰਕਤੀ_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        ਅੱਖਰ floatFormat[8];
        ਅੱਖਰ buf[32];   // enough for double with precision up to 20

        Mat mtx;
        ਪੂਰਨ mcn; // == mtx.channels()
        ਤਾਰਕਿਕ single_ਸਤਰ;
        ਤਾਰਕਿਕ align_ਕ੍ਰਮ;    // true when cn first order

        ਪੂਰਨ state;
        ਪੂਰਨ ਪੰਕਤੀ;
        ਪੂਰਨ ਖਾਨਾ;
        ਪੂਰਨ cn;

        String prologue;
        String epilogue;
        ਅੱਖਰ braces[5];

        ਖਾਲੀ (FormattedImpl::*ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str)();
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str_Bool() { snprintf(buf, ਆਕਾਰ(buf), "%d", (ਪੂਰਨ)mtx.ptr<uchar>(ਪੰਕਤੀ, ਖਾਨਾ)[cn] != 0); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str8u()  { snprintf(buf, ਆਕਾਰ(buf), "%3d", (ਪੂਰਨ)mtx.ptr<uchar>(ਪੰਕਤੀ, ਖਾਨਾ)[cn]); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str8s()  { snprintf(buf, ਆਕਾਰ(buf), "%3d", (ਪੂਰਨ)mtx.ptr<schar>(ਪੰਕਤੀ, ਖਾਨਾ)[cn]); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str16u() { snprintf(buf, ਆਕਾਰ(buf), "%d", (ਪੂਰਨ)mtx.ptr<ushort>(ਪੰਕਤੀ, ਖਾਨਾ)[cn]); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str16s() { snprintf(buf, ਆਕਾਰ(buf), "%d", (ਪੂਰਨ)mtx.ptr<ਛੋਟਾ>(ਪੰਕਤੀ, ਖਾਨਾ)[cn]); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str32u() { snprintf(buf, ਆਕਾਰ(buf), "%u", mtx.ptr<ਬਿਨਾਨਿਸ਼ਾਨੀ>(ਪੰਕਤੀ, ਖਾਨਾ)[cn]); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str32s() { snprintf(buf, ਆਕਾਰ(buf), "%d", mtx.ptr<ਪੂਰਨ>(ਪੰਕਤੀ, ਖਾਨਾ)[cn]); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str32f() { snprintf(buf, ਆਕਾਰ(buf), floatFormat, mtx.ptr<ਦਸ਼ਮਲਵ>(ਪੰਕਤੀ, ਖਾਨਾ)[cn]); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str64f() { snprintf(buf, ਆਕਾਰ(buf), floatFormat, mtx.ptr<ਦੁੱਗਣਾ>(ਪੰਕਤੀ, ਖਾਨਾ)[cn]); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str64u() { snprintf(buf, ਆਕਾਰ(buf), "%llu", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ)mtx.ptr<uint64_t>(ਪੰਕਤੀ, ਖਾਨਾ)[cn]); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str64s() { snprintf(buf, ਆਕਾਰ(buf), "%lld", (ਲੰਮਾ ਲੰਮਾ)mtx.ptr<int64_t>(ਪੰਕਤੀ, ਖਾਨਾ)[cn]); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str16f() { snprintf(buf, ਆਕਾਰ(buf), floatFormat, (ਦਸ਼ਮਲਵ)mtx.ptr<hfloat>(ਪੰਕਤੀ, ਖਾਨਾ)[cn]); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str16bf() { snprintf(buf, ਆਕਾਰ(buf), floatFormat, (ਦਸ਼ਮਲਵ)mtx.ptr<bfloat>(ਪੰਕਤੀ, ਖਾਨਾ)[cn]); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str8f()   { snprintf(buf, ਆਕਾਰ(buf), floatFormat, (ਦਸ਼ਮਲਵ)mtx.ptr<fp8_t>(ਪੰਕਤੀ, ਖਾਨਾ)[cn]); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str8fnuz() { snprintf(buf, ਆਕਾਰ(buf), floatFormat, (ਦਸ਼ਮਲਵ)mtx.ptr<fp8a_t>(ਪੰਕਤੀ, ਖਾਨਾ)[cn]); }
        ਖਾਲੀ ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str_Other() { buf[0] = 0; }

    ਜਨਤਕ:

        FormattedImpl(String pl, String el, Mat m, ਅੱਖਰ br[5], ਤਾਰਕਿਕ s_ਸਤਰ, ਤਾਰਕਿਕ a_ਕ੍ਰਮ, ਪੂਰਨ precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(braces, br, 5);
            state = STATE_PROLOGUE;
            single_ਸਤਰ = s_ਸਤਰ;
            align_ਕ੍ਰਮ = a_ਕ੍ਰਮ;
            ਪੰਕਤੀ = ਖਾਨਾ = cn =0;

            ਜੇ (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            ਨਹੀਂਤਾਂ
            {
                cv_snprintf(floatFormat, ਆਕਾਰ(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            ਚੋਣ(mtx.ਡੂੰਘਾਈ())
            {
                ਹਾਲਤ CV_8U:  ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str8u; ਰੋਕੋ;
                ਹਾਲਤ CV_8S:  ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str8s; ਰੋਕੋ;
                ਹਾਲਤ CV_Bool: ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str_Bool; ਰੋਕੋ;
                ਹਾਲਤ CV_16U: ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str16u; ਰੋਕੋ;
                ਹਾਲਤ CV_16S: ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str16s; ਰੋਕੋ;
                ਹਾਲਤ CV_32U: ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str32u; ਰੋਕੋ;
                ਹਾਲਤ CV_32S: ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str32s; ਰੋਕੋ;
                ਹਾਲਤ CV_32F: ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str32f; ਰੋਕੋ;
                ਹਾਲਤ CV_64F: ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str64f; ਰੋਕੋ;
                ਹਾਲਤ CV_64U: ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str64u; ਰੋਕੋ;
                ਹਾਲਤ CV_64S: ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str64s; ਰੋਕੋ;
                ਹਾਲਤ CV_16F: ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str16f; ਰੋਕੋ;
                ਹਾਲਤ CV_16BF: ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str16bf; ਰੋਕੋ;
                ਹਾਲਤ CV_8F_E4M3FN:   ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str8f; ਰੋਕੋ;
                ਹਾਲਤ CV_8F_E4M3FNUZ: ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str = &FormattedImpl::ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str8fnuz; ਰੋਕੋ;
                ਮੂਲ:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.ਡੂੰਘਾਈ()));
            }
        }

        ਖਾਲੀ reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        ਅਚਲ ਅੱਖਰ* next() CV_OVERRIDE
        {
            ਚੋਣ(state)
            {
                ਹਾਲਤ STATE_PROLOGUE:
                    ਪੰਕਤੀ = 0;
                    ਜੇ (mtx.empty())
                        state = STATE_EPILOGUE;
                    ਨਹੀਂਤਾਂ ਜੇ (align_ਕ੍ਰਮ)
                        state = STATE_INTERLUDE;
                    ਨਹੀਂਤਾਂ
                        state = STATE_ਪੰਕਤੀ_OPEN;
                    ਵਾਪਸ prologue.c_str();
                ਹਾਲਤ STATE_INTERLUDE:
                    state = STATE_ਪੰਕਤੀ_OPEN;
                    ਜੇ (ਪੰਕਤੀ >= mtx.ਪੰਕਤੀਆਂ)
                    {
                        ਜੇ (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            ਵਾਪਸ buf;
                        }
                        ਨਹੀਂਤਾਂ
                            ਪੰਕਤੀ = 0;
                        snprintf(buf, ਆਕਾਰ(buf), "\n(:, :, %d) = \n", cn+1);
                        ਵਾਪਸ buf;
                    }
                    snprintf(buf, ਆਕਾਰ(buf), "(:, :, %d) = \n", cn+1);
                    ਵਾਪਸ buf;
                ਹਾਲਤ STATE_EPILOGUE:
                    state = STATE_ਮੁਕੰਮਲ;
                    ਵਾਪਸ epilogue.c_str();
                ਹਾਲਤ STATE_ਪੰਕਤੀ_OPEN:
                    ਖਾਨਾ = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        ਜੇ (ਪੰਕਤੀ > 0)
                            ਜਦੋਂ(pos < prologue.size() && pos < ਆਕਾਰ(buf) - 2)
                                buf[pos++] = ' ';
                        ਜੇ (braces[BRACE_ਪੰਕਤੀ_OPEN])
                            buf[pos++] = braces[BRACE_ਪੰਕਤੀ_OPEN];
                        ਜੇ(!pos)
                            ਵਾਪਸ next();
                        buf[pos] = 0;
                    }
                    ਵਾਪਸ buf;
                ਹਾਲਤ STATE_ਪੰਕਤੀ_CLOSE:
                    state = STATE_ਸਤਰ_SEPARATOR;
                    ++ਪੰਕਤੀ;
                    ਜੇ (braces[BRACE_ਪੰਕਤੀ_CLOSE])
                    {
                        buf[0] = braces[BRACE_ਪੰਕਤੀ_CLOSE];
                        buf[1] = ਪੰਕਤੀ < mtx.ਪੰਕਤੀਆਂ ? ',' : '\0';
                        buf[2] = 0;
                        ਵਾਪਸ buf;
                    }
                    ਨਹੀਂਤਾਂ ਜੇ(braces[BRACE_ਪੰਕਤੀ_SEP] && ਪੰਕਤੀ < mtx.ਪੰਕਤੀਆਂ)
                    {
                        buf[0] = braces[BRACE_ਪੰਕਤੀ_SEP];
                        buf[1] = 0;
                        ਵਾਪਸ buf;
                    }
                    ਵਾਪਸ next();
                ਹਾਲਤ STATE_CN_OPEN:
                    state = STATE_ਮੁੱਲ;
                    ਜੇ (!align_ਕ੍ਰਮ)
                        cn = 0;
                    ਜੇ (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        ਵਾਪਸ buf;
                    }
                    ਵਾਪਸ next();
                ਹਾਲਤ STATE_CN_CLOSE:
                    ++ਖਾਨਾ;
                    ਜੇ (ਖਾਨਾ >= mtx.ਖਾਨੇ_ਗਿਣਤੀ)
                        state = STATE_ਪੰਕਤੀ_CLOSE;
                    ਨਹੀਂਤਾਂ
                        state = STATE_CN_SEPARATOR;
                    ਜੇ (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        ਵਾਪਸ buf;
                    }
                    ਵਾਪਸ next();
                ਹਾਲਤ STATE_ਮੁੱਲ:
                    (ਇਹ->*ਮੁੱਲ_ਮੰਜ਼ਿਲ_Str)();
                    state = STATE_CN_CLOSE;
                    ਜੇ (align_ਕ੍ਰਮ)
                        ਵਾਪਸ buf;
                    ਜੇ (++cn < mcn)
                        state = STATE_ਮੁੱਲ_SEPARATOR;
                    ਵਾਪਸ buf;
                ਹਾਲਤ STATE_ਮੁਕੰਮਲ:
                    ਵਾਪਸ 0;
                ਹਾਲਤ STATE_ਸਤਰ_SEPARATOR:
                    ਜੇ (ਪੰਕਤੀ >= mtx.ਪੰਕਤੀਆਂ)
                    {
                        ਜੇ (align_ਕ੍ਰਮ)
                            state = STATE_INTERLUDE;
                        ਨਹੀਂਤਾਂ
                            state = STATE_EPILOGUE;
                        ਵਾਪਸ next();
                    }
                    state = STATE_ਪੰਕਤੀ_OPEN;
                    buf[0] = single_ਸਤਰ ? ' ' : '\n';
                    buf[1] = 0;
                    ਵਾਪਸ buf;
                ਹਾਲਤ STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    ਵਾਪਸ buf;
                ਹਾਲਤ STATE_ਮੁੱਲ_SEPARATOR:
                    state = STATE_ਮੁੱਲ;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    ਵਾਪਸ buf;
            }
            ਵਾਪਸ 0;
        }
    };

    ਵਰਗ Formatter_ਅਧਾਰ : ਜਨਤਕ Formatter
    {
    ਜਨਤਕ:
        Formatter_ਅਧਾਰ() : prec16f(4), prec32f(8), prec64f(16), multiline(ਸੱਚ) {}

        ਖਾਲੀ set16fPrecision(ਪੂਰਨ p) CV_OVERRIDE
        {
            prec16f = p;
        }

        ਖਾਲੀ set32fPrecision(ਪੂਰਨ p) CV_OVERRIDE
        {
            prec32f = p;
        }

        ਖਾਲੀ set64fPrecision(ਪੂਰਨ p) CV_OVERRIDE
        {
            prec64f = p;
        }

        ਖਾਲੀ ਸੈੱਟ_Multiline(ਤਾਰਕਿਕ ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    ਸੁਰੱਖਿਅਤ:
        ਪੂਰਨ prec16f;
        ਪੂਰਨ prec32f;
        ਪੂਰਨ prec64f;
        ਪੂਰਨ multiline;
    };

    ਵਰਗ DefaultFormatter CV_FINAL : ਜਨਤਕ Formatter_ਅਧਾਰ
    {
    ਜਨਤਕ:

        Ptr<Formatted> format(ਅਚਲ Mat& mtx) ਅਚਲ CV_OVERRIDE
        {
            ਅੱਖਰ braces[5] = {'\0', '\0', ';', '\0', '\0'};
            ਵਾਪਸ makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.ਪੰਕਤੀਆਂ == 1 || !multiline, ਝੂਠ, mtx.ਡੂੰਘਾਈ() == CV_64F ? prec64f : prec32f );
        }
    };

    ਵਰਗ MatlabFormatter CV_FINAL : ਜਨਤਕ Formatter_ਅਧਾਰ
    {
    ਜਨਤਕ:

        Ptr<Formatted> format(ਅਚਲ Mat& mtx) ਅਚਲ CV_OVERRIDE
        {
            ਅੱਖਰ braces[5] = {'\0', '\0', ';', '\0', '\0'};
            ਵਾਪਸ makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.ਪੰਕਤੀਆਂ == 1 || !multiline, ਸੱਚ, mtx.ਡੂੰਘਾਈ() == CV_64F ? prec64f : prec32f );
        }
    };

    ਵਰਗ PythonFormatter CV_FINAL : ਜਨਤਕ Formatter_ਅਧਾਰ
    {
    ਜਨਤਕ:

        Ptr<Formatted> format(ਅਚਲ Mat& mtx) ਅਚਲ CV_OVERRIDE
        {
            ਅੱਖਰ braces[5] = {'[', ']', ',', '[', ']'};
            ਜੇ (mtx.ਖਾਨੇ_ਗਿਣਤੀ == 1)
                braces[0] = braces[1] = '\0';
            ਵਾਪਸ makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.ਪੰਕਤੀਆਂ == 1 || !multiline, ਝੂਠ, mtx.ਡੂੰਘਾਈ() == CV_64F ? prec64f : prec32f );
        }
    };

    ਵਰਗ NumpyFormatter CV_FINAL : ਜਨਤਕ Formatter_ਅਧਾਰ
    {
    ਜਨਤਕ:

        Ptr<Formatted> format(ਅਚਲ Mat& mtx) ਅਚਲ CV_OVERRIDE
        {
            ਸਥਿਰ ਅਚਲ ਅੱਖਰ* numpyTypes[CV_ਡੂੰਘਾਈ_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            ਅੱਖਰ braces[5] = {'[', ']', ',', '[', ']'};
            ਜੇ (mtx.ਖਾਨੇ_ਗਿਣਤੀ == 1)
                braces[0] = braces[1] = '\0';
            ਵਾਪਸ makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.ਡੂੰਘਾਈ()]), mtx, &*braces,
                mtx.ਪੰਕਤੀਆਂ == 1 || !multiline, ਝੂਠ, mtx.ਡੂੰਘਾਈ() == CV_64F ? prec64f : prec32f );
        }
    };

    ਵਰਗ CSVFormatter CV_FINAL : ਜਨਤਕ Formatter_ਅਧਾਰ
    {
    ਜਨਤਕ:

        Ptr<Formatted> format(ਅਚਲ Mat& mtx) ਅਚਲ CV_OVERRIDE
        {
            ਅੱਖਰ braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            ਵਾਪਸ makePtr<FormattedImpl>(String(),
                mtx.ਪੰਕਤੀਆਂ > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.ਪੰਕਤੀਆਂ == 1 || !multiline, ਝੂਠ, mtx.ਡੂੰਘਾਈ() == CV_64F ? prec64f : prec32f );
        }
    };

    ਵਰਗ CFormatter CV_FINAL : ਜਨਤਕ Formatter_ਅਧਾਰ
    {
    ਜਨਤਕ:

        Ptr<Formatted> format(ਅਚਲ Mat& mtx) ਅਚਲ CV_OVERRIDE
        {
            ਅੱਖਰ braces[5] = {'\0', '\0', ',', '\0', '\0'};
            ਵਾਪਸ makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.ਪੰਕਤੀਆਂ == 1 || !multiline, ਝੂਠ, mtx.ਡੂੰਘਾਈ() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        ਚੋਣ(fmt)
        {
            ਹਾਲਤ FMT_DEFAULT:
                ਵਾਪਸ makePtr<DefaultFormatter>();
            ਹਾਲਤ FMT_MATLAB:
                ਵਾਪਸ makePtr<MatlabFormatter>();
            ਹਾਲਤ FMT_CSV:
                ਵਾਪਸ makePtr<CSVFormatter>();
            ਹਾਲਤ FMT_PYTHON:
                ਵਾਪਸ makePtr<PythonFormatter>();
            ਹਾਲਤ FMT_NUMPY:
                ਵਾਪਸ makePtr<NumpyFormatter>();
            ਹਾਲਤ FMT_C:
                ਵਾਪਸ makePtr<CFormatter>();
        }
        ਵਾਪਸ makePtr<DefaultFormatter>();
    }

    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp> ਬਣਤਰ Fmt
    {
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ ਆਰਜ਼ੀ_type;
        ਸਥਿਰ ਅਚਲ ਅੱਖਰ* fmt() { ਵਾਪਸ "%d"; }
    };

    ਸਾਂਚਾ<> ਬਣਤਰ Fmt<uint32_t>
    {
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਰਜ਼ੀ_type;
        ਸਥਿਰ ਅਚਲ ਅੱਖਰ* fmt() { ਵਾਪਸ "%u"; }
    };

    ਸਾਂਚਾ<> ਬਣਤਰ Fmt<int64_t>
    {
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਲੰਮਾ ਲੰਮਾ ਆਰਜ਼ੀ_type;
        ਸਥਿਰ ਅਚਲ ਅੱਖਰ* fmt() { ਵਾਪਸ "%lld"; }
    };

    ਸਾਂਚਾ<> ਬਣਤਰ Fmt<uint64_t>
    {
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਆਰਜ਼ੀ_type;
        ਸਥਿਰ ਅਚਲ ਅੱਖਰ* fmt() { ਵਾਪਸ "%llu"; }
    };

    ਸਾਂਚਾ<> ਬਣਤਰ Fmt<ਦਸ਼ਮਲਵ>
    {
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦਸ਼ਮਲਵ ਆਰਜ਼ੀ_type;
        ਸਥਿਰ ਅਚਲ ਅੱਖਰ* fmt() { ਵਾਪਸ "%.5g"; }
    };

    ਸਾਂਚਾ<> ਬਣਤਰ Fmt<ਦੁੱਗਣਾ>
    {
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦੁੱਗਣਾ ਆਰਜ਼ੀ_type;
        ਸਥਿਰ ਅਚਲ ਅੱਖਰ* fmt() { ਵਾਪਸ "%.5g"; }
    };

    ਸਾਂਚਾ<> ਬਣਤਰ Fmt<hfloat>
    {
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦਸ਼ਮਲਵ ਆਰਜ਼ੀ_type;
        ਸਥਿਰ ਅਚਲ ਅੱਖਰ* fmt() { ਵਾਪਸ "%.5g"; }
    };

    ਸਾਂਚਾ<> ਬਣਤਰ Fmt<bfloat>
    {
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦਸ਼ਮਲਵ ਆਰਜ਼ੀ_type;
        ਸਥਿਰ ਅਚਲ ਅੱਖਰ* fmt() { ਵਾਪਸ "%.4g"; }
    };

    ਸਾਂਚਾ<> ਬਣਤਰ Fmt<fp8_t>
    {
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦਸ਼ਮਲਵ ਆਰਜ਼ੀ_type;
        ਸਥਿਰ ਅਚਲ ਅੱਖਰ* fmt() { ਵਾਪਸ "%.4g"; }
    };

    ਸਾਂਚਾ<> ਬਣਤਰ Fmt<fp8a_t>
    {
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦਸ਼ਮਲਵ ਆਰਜ਼ੀ_type;
        ਸਥਿਰ ਅਚਲ ਅੱਖਰ* fmt() { ਵਾਪਸ "%.4g"; }
    };

    ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ _Tp>
    ਸਥਿਰ ਖਾਲੀ pprint_ਪੰਕਤੀ(std::ostream& strm, ਅਚਲ _Tp* ptr, ਪੂਰਨ n, size_t ofs, ਪੂਰਨ ਕਿਨਾਰਾ)
    {
        ਅੱਖਰ buf[128];
        ਅਚਲ ਅੱਖਰ* fmt = Fmt<_Tp>::fmt();
        ਪੂਰਨ i, ndump = ਕਿਨਾਰਾ > 0 ? std::min(n, ਕਿਨਾਰਾ*2+1) : n;
        ਜੇ (ਕਿਨਾਰਾ == 0)
            ਕਿਨਾਰਾ = ndump;
        ਲਈ (i = 0; i < ndump; i++) {
            ਪੂਰਨ j = n == ndump || i < ਕਿਨਾਰਾ ? i : i == ਕਿਨਾਰਾ ? -1 : n-ਕਿਨਾਰਾ*2-1+i;
            ਜੇ (i > 0)
                strm << ", ";
            ਜੇ (j >= 0) {
                snprintf(buf, ਆਕਾਰ(buf), fmt, (ਕਿਸਮ_ਨਾਂ Fmt<_Tp>::ਆਰਜ਼ੀ_type)ptr[ofs + j]);
                strm << buf;
            } ਨਹੀਂਤਾਂ
                strm << "... ";
        }
    }

    ਸਥਿਰ ਖਾਲੀ pprint_ਟੁਕੜਾ(std::ostream& strm, ਅਚਲ Mat& tensor,
                            ਅਚਲ size_t* ਕਦਮ, ਪੂਰਨ d,
                            size_t ofs, ਪੂਰਨ ਕਿਨਾਰਾ)
    {
        MatShape shape = tensor.shape();
        ਪੂਰਨ ndims = shape.dims;
        ਪੂਰਨ n = d >= ndims ? 1 : shape[d];
        ਜੇ (d >= ndims - 1) {
            ਪੂਰਨ typ = tensor.ਡੂੰਘਾਈ();
            ਖਾਲੀ* ਅੰਕੜੇ = tensor.ਅੰਕੜੇ;
            CV_Assert(ਅੰਕੜੇ);
            n *= tensor.channels();
            ਜੇ (typ == CV_8U)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ uint8_t*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ ਜੇ (typ == CV_8S)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ int8_t*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ ਜੇ (typ == CV_16U)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ uint16_t*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ ਜੇ (typ == CV_16S)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ int16_t*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ ਜੇ (typ == CV_32U)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ ਜੇ (typ == CV_32S)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ ਪੂਰਨ*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ ਜੇ (typ == CV_64U)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ uint64_t*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ ਜੇ (typ == CV_64S)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ int64_t*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ ਜੇ (typ == CV_32F)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ ਦਸ਼ਮਲਵ*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ ਜੇ (typ == CV_64F)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ ਦੁੱਗਣਾ*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ ਜੇ (typ == CV_16F)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ hfloat*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ ਜੇ (typ == CV_16BF)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ bfloat*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ ਜੇ (typ == CV_Bool)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ ਤਾਰਕਿਕ*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ ਜੇ (typ == CV_8F_E4M3FN)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ fp8_t*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ ਜੇ (typ == CV_8F_E4M3FNUZ)
                pprint_ਪੰਕਤੀ(strm, (ਅਚਲ fp8a_t*)ਅੰਕੜੇ, n, ofs, ਕਿਨਾਰਾ);
            ਨਹੀਂਤਾਂ {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } ਨਹੀਂਤਾਂ {
            ਪੂਰਨ i, ndump = ਕਿਨਾਰਾ > 0 ? std::min(n, ਕਿਨਾਰਾ*2+1) : n;
            ਤਾਰਕਿਕ dots = ਝੂਠ;
            ਲਈ (i = 0; i < ndump; i++) {
                ਜੇ (i > 0 && !dots) {
                    ਪੂਰਨ nempty_ਸਤਰਾਂ = ndims - 2 - d;
                    ਲਈ (ਪੂਰਨ k = 0; k < nempty_ਸਤਰਾਂ; k++)
                        strm << "\n";
                }
                ਜੇ (i > 0)
                    strm << "\n";
                ਪੂਰਨ j = n == ndump || i < ਕਿਨਾਰਾ ? i :
                        i == ਕਿਨਾਰਾ ? -1 :
                        n - ਕਿਨਾਰਾ*2 - 1 + i;
                dots = j < 0;
                ਜੇ (!dots)
                    pprint_ਟੁਕੜਾ(strm, tensor, ਕਦਮ, d+1, ofs + j*ਕਦਮ[d], ਕਿਨਾਰਾ);
                ਨਹੀਂਤਾਂ
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, ਨਿਵੇਸ਼_Array array,
                         ਪੂਰਨ /*indent*/, ਪੂਰਨ ਕਿਨਾਰਾ_2,
                         ਪੂਰਨ ਸਾਰਾ_Tensor_ਹੱਦ,
                         ਅੱਖਰ parens)
    {
        ਅੱਖਰ oparen = parens;
        ਅੱਖਰ cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        ਪੂਰਨ ਕਿਨਾਰਾ = ਕਿਨਾਰਾ_2 > 0 ? ਕਿਨਾਰਾ_2 : 3;
        ਸਾਰਾ_Tensor_ਹੱਦ = ਸਾਰਾ_Tensor_ਹੱਦ > 0 ? ਸਾਰਾ_Tensor_ਹੱਦ : 100;

        Mat tensor = array.getMat();
        ਜੇ (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat ਆਰਜ਼ੀ_2;
            tensor.copy_ਮੰਜ਼ਿਲ(ਆਰਜ਼ੀ_2);
            tensor = ਆਰਜ਼ੀ_2;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.ਕੁੱਲ();

        ਜੇ (parens)
            strm << oparen;
        ਜੇ (sz_all == 0) {
            ਜੇ (!parens)
                strm << "<empty>";
        } ਨਹੀਂਤਾਂ {
            ਜੇ (sz_all <= (size_t)ਸਾਰਾ_Tensor_ਹੱਦ)
                ਕਿਨਾਰਾ = 0;

            ਪੂਰਨ ndims = shape.dims;
            ਪੂਰਨ cn = tensor.channels();
            size_t ਕਦਮ[MatShape::MAX_DIMS];
            ਕਦਮ[std::max(ndims-1, 0)] = 1;
            ਲਈ (ਪੂਰਨ i = ndims-2; i >= 0; i--) {
                ਕਦਮ[i] = ਕਦਮ[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprint_ਟੁਕੜਾ(strm, tensor, ਕਦਮ, 0, 0, ਕਿਨਾਰਾ);
        }
        ਜੇ (parens)
            strm << cparen;
        ਵਾਪਸ strm;
    }

} // cv
