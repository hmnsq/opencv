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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ನಾಮಾಕಾಶ cv
{
    ವರ್ಗ FormattedImpl CV_FINAL : ಸಾರ್ವಜನಿಕ Formatted
    {
        ಎಣಿಕೆ { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ಅಡ್ಡಸಾಲು_OPEN, STATE_ಅಡ್ಡಸಾಲು_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_ಮೌಲ್ಯ, STATE_ಪೂರ್ಣಗೊಂಡ,
               STATE_ಸಾಲು_SEPARATOR, STATE_CN_SEPARATOR, STATE_ಮೌಲ್ಯ_SEPARATOR };
        ಎಣಿಕೆ {BRACE_ಅಡ್ಡಸಾಲು_OPEN = 0, BRACE_ಅಡ್ಡಸಾಲು_CLOSE = 1, BRACE_ಅಡ್ಡಸಾಲು_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        ಅಕ್ಷರ floatFormat[8];
        ಅಕ್ಷರ buf[32];   // enough for double with precision up to 20

        Mat mtx;
        ಪೂರ್ಣಾಂಕ mcn; // == mtx.channels()
        ತಾರ್ಕಿಕ single_ಸಾಲು;
        ತಾರ್ಕಿಕ align_ಕ್ರಮ;    // true when cn first order

        ಪೂರ್ಣಾಂಕ state;
        ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು;
        ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು;
        ಪೂರ್ಣಾಂಕ cn;

        String prologue;
        String epilogue;
        ಅಕ್ಷರ braces[5];

        ಶೂನ್ಯ (FormattedImpl::*ಮೌಲ್ಯ_ಗಮ್ಯ_Str)();
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str_Bool() { snprintf(buf, ಗಾತ್ರ(buf), "%d", (ಪೂರ್ಣಾಂಕ)mtx.ptr<uchar>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn] != 0); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str8u()  { snprintf(buf, ಗಾತ್ರ(buf), "%3d", (ಪೂರ್ಣಾಂಕ)mtx.ptr<uchar>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str8s()  { snprintf(buf, ಗಾತ್ರ(buf), "%3d", (ಪೂರ್ಣಾಂಕ)mtx.ptr<schar>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str16u() { snprintf(buf, ಗಾತ್ರ(buf), "%d", (ಪೂರ್ಣಾಂಕ)mtx.ptr<ushort>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str16s() { snprintf(buf, ಗಾತ್ರ(buf), "%d", (ಪೂರ್ಣಾಂಕ)mtx.ptr<ಗಿಡ್ಡ>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str32u() { snprintf(buf, ಗಾತ್ರ(buf), "%u", mtx.ptr<ಚಿಹ್ನೆರಹಿತ>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str32s() { snprintf(buf, ಗಾತ್ರ(buf), "%d", mtx.ptr<ಪೂರ್ಣಾಂಕ>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str32f() { snprintf(buf, ಗಾತ್ರ(buf), floatFormat, mtx.ptr<ದಶಮಾಂಶ>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str64f() { snprintf(buf, ಗಾತ್ರ(buf), floatFormat, mtx.ptr<ದ್ವಿಗುಣ>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str64u() { snprintf(buf, ಗಾತ್ರ(buf), "%llu", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ)mtx.ptr<uint64_t>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str64s() { snprintf(buf, ಗಾತ್ರ(buf), "%lld", (ದೀರ್ಘ ದೀರ್ಘ)mtx.ptr<int64_t>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str16f() { snprintf(buf, ಗಾತ್ರ(buf), floatFormat, (ದಶಮಾಂಶ)mtx.ptr<hfloat>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str16bf() { snprintf(buf, ಗಾತ್ರ(buf), floatFormat, (ದಶಮಾಂಶ)mtx.ptr<bfloat>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str8f()   { snprintf(buf, ಗಾತ್ರ(buf), floatFormat, (ದಶಮಾಂಶ)mtx.ptr<fp8_t>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str8fnuz() { snprintf(buf, ಗಾತ್ರ(buf), floatFormat, (ದಶಮಾಂಶ)mtx.ptr<fp8a_t>(ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_ಗಮ್ಯ_Str_Other() { buf[0] = 0; }

    ಸಾರ್ವಜನಿಕ:

        FormattedImpl(String pl, String el, Mat m, ಅಕ್ಷರ br[5], ತಾರ್ಕಿಕ s_ಸಾಲು, ತಾರ್ಕಿಕ a_ಕ್ರಮ, ಪೂರ್ಣಾಂಕ precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            ಸ್ಮರಣೆ_ನಕಲು(braces, br, 5);
            state = STATE_PROLOGUE;
            single_ಸಾಲು = s_ಸಾಲು;
            align_ಕ್ರಮ = a_ಕ್ರಮ;
            ಅಡ್ಡಸಾಲು = ಲಂಬಸಾಲು = cn =0;

            ಆದರೆ (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                cv_snprintf(floatFormat, ಗಾತ್ರ(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            ಆಯ್ಕೆ(mtx.ಆಳ())
            {
                ಸ್ಥಿತಿ CV_8U:  ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str8u; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ CV_8S:  ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str8s; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ CV_Bool: ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str_Bool; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ CV_16U: ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str16u; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ CV_16S: ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str16s; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ CV_32U: ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str32u; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ CV_32S: ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str32s; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ CV_32F: ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str32f; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ CV_64F: ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str64f; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ CV_64U: ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str64u; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ CV_64S: ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str64s; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ CV_16F: ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str16f; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ CV_16BF: ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str16bf; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ CV_8F_E4M3FN:   ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str8f; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ CV_8F_E4M3FNUZ: ಮೌಲ್ಯ_ಗಮ್ಯ_Str = &FormattedImpl::ಮೌಲ್ಯ_ಗಮ್ಯ_Str8fnuz; ನಿಲ್ಲಿಸು;
                ಪೂರ್ವನಿಯೋಜಿತ:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.ಆಳ()));
            }
        }

        ಶೂನ್ಯ reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* next() CV_OVERRIDE
        {
            ಆಯ್ಕೆ(state)
            {
                ಸ್ಥಿತಿ STATE_PROLOGUE:
                    ಅಡ್ಡಸಾಲು = 0;
                    ಆದರೆ (mtx.empty())
                        state = STATE_EPILOGUE;
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (align_ಕ್ರಮ)
                        state = STATE_INTERLUDE;
                    ಇಲ್ಲದಿದ್ದರೆ
                        state = STATE_ಅಡ್ಡಸಾಲು_OPEN;
                    ಹಿಂತಿರುಗಿ prologue.c_str();
                ಸ್ಥಿತಿ STATE_INTERLUDE:
                    state = STATE_ಅಡ್ಡಸಾಲು_OPEN;
                    ಆದರೆ (ಅಡ್ಡಸಾಲು >= mtx.ಅಡ್ಡಸಾಲುಗಳು)
                    {
                        ಆದರೆ (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            ಹಿಂತಿರುಗಿ buf;
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                            ಅಡ್ಡಸಾಲು = 0;
                        snprintf(buf, ಗಾತ್ರ(buf), "\n(:, :, %d) = \n", cn+1);
                        ಹಿಂತಿರುಗಿ buf;
                    }
                    snprintf(buf, ಗಾತ್ರ(buf), "(:, :, %d) = \n", cn+1);
                    ಹಿಂತಿರುಗಿ buf;
                ಸ್ಥಿತಿ STATE_EPILOGUE:
                    state = STATE_ಪೂರ್ಣಗೊಂಡ;
                    ಹಿಂತಿರುಗಿ epilogue.c_str();
                ಸ್ಥಿತಿ STATE_ಅಡ್ಡಸಾಲು_OPEN:
                    ಲಂಬಸಾಲು = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        ಆದರೆ (ಅಡ್ಡಸಾಲು > 0)
                            ಆಗ(pos < prologue.size() && pos < ಗಾತ್ರ(buf) - 2)
                                buf[pos++] = ' ';
                        ಆದರೆ (braces[BRACE_ಅಡ್ಡಸಾಲು_OPEN])
                            buf[pos++] = braces[BRACE_ಅಡ್ಡಸಾಲು_OPEN];
                        ಆದರೆ(!pos)
                            ಹಿಂತಿರುಗಿ next();
                        buf[pos] = 0;
                    }
                    ಹಿಂತಿರುಗಿ buf;
                ಸ್ಥಿತಿ STATE_ಅಡ್ಡಸಾಲು_CLOSE:
                    state = STATE_ಸಾಲು_SEPARATOR;
                    ++ಅಡ್ಡಸಾಲು;
                    ಆದರೆ (braces[BRACE_ಅಡ್ಡಸಾಲು_CLOSE])
                    {
                        buf[0] = braces[BRACE_ಅಡ್ಡಸಾಲು_CLOSE];
                        buf[1] = ಅಡ್ಡಸಾಲು < mtx.ಅಡ್ಡಸಾಲುಗಳು ? ',' : '\0';
                        buf[2] = 0;
                        ಹಿಂತಿರುಗಿ buf;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(braces[BRACE_ಅಡ್ಡಸಾಲು_SEP] && ಅಡ್ಡಸಾಲು < mtx.ಅಡ್ಡಸಾಲುಗಳು)
                    {
                        buf[0] = braces[BRACE_ಅಡ್ಡಸಾಲು_SEP];
                        buf[1] = 0;
                        ಹಿಂತಿರುಗಿ buf;
                    }
                    ಹಿಂತಿರುಗಿ next();
                ಸ್ಥಿತಿ STATE_CN_OPEN:
                    state = STATE_ಮೌಲ್ಯ;
                    ಆದರೆ (!align_ಕ್ರಮ)
                        cn = 0;
                    ಆದರೆ (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        ಹಿಂತಿರುಗಿ buf;
                    }
                    ಹಿಂತಿರುಗಿ next();
                ಸ್ಥಿತಿ STATE_CN_CLOSE:
                    ++ಲಂಬಸಾಲು;
                    ಆದರೆ (ಲಂಬಸಾಲು >= mtx.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)
                        state = STATE_ಅಡ್ಡಸಾಲು_CLOSE;
                    ಇಲ್ಲದಿದ್ದರೆ
                        state = STATE_CN_SEPARATOR;
                    ಆದರೆ (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        ಹಿಂತಿರುಗಿ buf;
                    }
                    ಹಿಂತಿರುಗಿ next();
                ಸ್ಥಿತಿ STATE_ಮೌಲ್ಯ:
                    (ಇದು->*ಮೌಲ್ಯ_ಗಮ್ಯ_Str)();
                    state = STATE_CN_CLOSE;
                    ಆದರೆ (align_ಕ್ರಮ)
                        ಹಿಂತಿರುಗಿ buf;
                    ಆದರೆ (++cn < mcn)
                        state = STATE_ಮೌಲ್ಯ_SEPARATOR;
                    ಹಿಂತಿರುಗಿ buf;
                ಸ್ಥಿತಿ STATE_ಪೂರ್ಣಗೊಂಡ:
                    ಹಿಂತಿರುಗಿ 0;
                ಸ್ಥಿತಿ STATE_ಸಾಲು_SEPARATOR:
                    ಆದರೆ (ಅಡ್ಡಸಾಲು >= mtx.ಅಡ್ಡಸಾಲುಗಳು)
                    {
                        ಆದರೆ (align_ಕ್ರಮ)
                            state = STATE_INTERLUDE;
                        ಇಲ್ಲದಿದ್ದರೆ
                            state = STATE_EPILOGUE;
                        ಹಿಂತಿರುಗಿ next();
                    }
                    state = STATE_ಅಡ್ಡಸಾಲು_OPEN;
                    buf[0] = single_ಸಾಲು ? ' ' : '\n';
                    buf[1] = 0;
                    ಹಿಂತಿರುಗಿ buf;
                ಸ್ಥಿತಿ STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    ಹಿಂತಿರುಗಿ buf;
                ಸ್ಥಿತಿ STATE_ಮೌಲ್ಯ_SEPARATOR:
                    state = STATE_ಮೌಲ್ಯ;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    ಹಿಂತಿರುಗಿ buf;
            }
            ಹಿಂತಿರುಗಿ 0;
        }
    };

    ವರ್ಗ Formatter_ತಳ : ಸಾರ್ವಜನಿಕ Formatter
    {
    ಸಾರ್ವಜನಿಕ:
        Formatter_ತಳ() : prec16f(4), prec32f(8), prec64f(16), multiline(ನಿಜ) {}

        ಶೂನ್ಯ set16fPrecision(ಪೂರ್ಣಾಂಕ p) CV_OVERRIDE
        {
            prec16f = p;
        }

        ಶೂನ್ಯ set32fPrecision(ಪೂರ್ಣಾಂಕ p) CV_OVERRIDE
        {
            prec32f = p;
        }

        ಶೂನ್ಯ set64fPrecision(ಪೂರ್ಣಾಂಕ p) CV_OVERRIDE
        {
            prec64f = p;
        }

        ಶೂನ್ಯ ಗಣ_Multiline(ತಾರ್ಕಿಕ ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    ಸಂರಕ್ಷಿತ:
        ಪೂರ್ಣಾಂಕ prec16f;
        ಪೂರ್ಣಾಂಕ prec32f;
        ಪೂರ್ಣಾಂಕ prec64f;
        ಪೂರ್ಣಾಂಕ multiline;
    };

    ವರ್ಗ DefaultFormatter CV_FINAL : ಸಾರ್ವಜನಿಕ Formatter_ತಳ
    {
    ಸಾರ್ವಜನಿಕ:

        Ptr<Formatted> format(ಸ್ಥಿರಾಂಕ Mat& mtx) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
        {
            ಅಕ್ಷರ braces[5] = {'\0', '\0', ';', '\0', '\0'};
            ಹಿಂತಿರುಗಿ makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.ಅಡ್ಡಸಾಲುಗಳು == 1 || !multiline, ಸುಳ್ಳು, mtx.ಆಳ() == CV_64F ? prec64f : prec32f );
        }
    };

    ವರ್ಗ MatlabFormatter CV_FINAL : ಸಾರ್ವಜನಿಕ Formatter_ತಳ
    {
    ಸಾರ್ವಜನಿಕ:

        Ptr<Formatted> format(ಸ್ಥಿರಾಂಕ Mat& mtx) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
        {
            ಅಕ್ಷರ braces[5] = {'\0', '\0', ';', '\0', '\0'};
            ಹಿಂತಿರುಗಿ makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.ಅಡ್ಡಸಾಲುಗಳು == 1 || !multiline, ನಿಜ, mtx.ಆಳ() == CV_64F ? prec64f : prec32f );
        }
    };

    ವರ್ಗ PythonFormatter CV_FINAL : ಸಾರ್ವಜನಿಕ Formatter_ತಳ
    {
    ಸಾರ್ವಜನಿಕ:

        Ptr<Formatted> format(ಸ್ಥಿರಾಂಕ Mat& mtx) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
        {
            ಅಕ್ಷರ braces[5] = {'[', ']', ',', '[', ']'};
            ಆದರೆ (mtx.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1)
                braces[0] = braces[1] = '\0';
            ಹಿಂತಿರುಗಿ makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.ಅಡ್ಡಸಾಲುಗಳು == 1 || !multiline, ಸುಳ್ಳು, mtx.ಆಳ() == CV_64F ? prec64f : prec32f );
        }
    };

    ವರ್ಗ NumpyFormatter CV_FINAL : ಸಾರ್ವಜನಿಕ Formatter_ತಳ
    {
    ಸಾರ್ವಜನಿಕ:

        Ptr<Formatted> format(ಸ್ಥಿರಾಂಕ Mat& mtx) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
        {
            ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* numpyTypes[CV_ಆಳ_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            ಅಕ್ಷರ braces[5] = {'[', ']', ',', '[', ']'};
            ಆದರೆ (mtx.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1)
                braces[0] = braces[1] = '\0';
            ಹಿಂತಿರುಗಿ makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.ಆಳ()]), mtx, &*braces,
                mtx.ಅಡ್ಡಸಾಲುಗಳು == 1 || !multiline, ಸುಳ್ಳು, mtx.ಆಳ() == CV_64F ? prec64f : prec32f );
        }
    };

    ವರ್ಗ CSVFormatter CV_FINAL : ಸಾರ್ವಜನಿಕ Formatter_ತಳ
    {
    ಸಾರ್ವಜನಿಕ:

        Ptr<Formatted> format(ಸ್ಥಿರಾಂಕ Mat& mtx) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
        {
            ಅಕ್ಷರ braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            ಹಿಂತಿರುಗಿ makePtr<FormattedImpl>(String(),
                mtx.ಅಡ್ಡಸಾಲುಗಳು > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.ಅಡ್ಡಸಾಲುಗಳು == 1 || !multiline, ಸುಳ್ಳು, mtx.ಆಳ() == CV_64F ? prec64f : prec32f );
        }
    };

    ವರ್ಗ CFormatter CV_FINAL : ಸಾರ್ವಜನಿಕ Formatter_ತಳ
    {
    ಸಾರ್ವಜನಿಕ:

        Ptr<Formatted> format(ಸ್ಥಿರಾಂಕ Mat& mtx) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
        {
            ಅಕ್ಷರ braces[5] = {'\0', '\0', ',', '\0', '\0'};
            ಹಿಂತಿರುಗಿ makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.ಅಡ್ಡಸಾಲುಗಳು == 1 || !multiline, ಸುಳ್ಳು, mtx.ಆಳ() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        ಆಯ್ಕೆ(fmt)
        {
            ಸ್ಥಿತಿ FMT_DEFAULT:
                ಹಿಂತಿರುಗಿ makePtr<DefaultFormatter>();
            ಸ್ಥಿತಿ FMT_MATLAB:
                ಹಿಂತಿರುಗಿ makePtr<MatlabFormatter>();
            ಸ್ಥಿತಿ FMT_CSV:
                ಹಿಂತಿರುಗಿ makePtr<CSVFormatter>();
            ಸ್ಥಿತಿ FMT_PYTHON:
                ಹಿಂತಿರುಗಿ makePtr<PythonFormatter>();
            ಸ್ಥಿತಿ FMT_NUMPY:
                ಹಿಂತಿರುಗಿ makePtr<NumpyFormatter>();
            ಸ್ಥಿತಿ FMT_C:
                ಹಿಂತಿರುಗಿ makePtr<CFormatter>();
        }
        ಹಿಂತಿರುಗಿ makePtr<DefaultFormatter>();
    }

    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp> ರಚನೆ Fmt
    {
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ ತಾತ್ಕಾಲಿಕ_type;
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt() { ಹಿಂತಿರುಗಿ "%d"; }
    };

    ಮಾದರಿ<> ರಚನೆ Fmt<uint32_t>
    {
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ತಾತ್ಕಾಲಿಕ_type;
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt() { ಹಿಂತಿರುಗಿ "%u"; }
    };

    ಮಾದರಿ<> ರಚನೆ Fmt<int64_t>
    {
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದೀರ್ಘ ದೀರ್ಘ ತಾತ್ಕಾಲಿಕ_type;
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt() { ಹಿಂತಿರುಗಿ "%lld"; }
    };

    ಮಾದರಿ<> ರಚನೆ Fmt<uint64_t>
    {
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ತಾತ್ಕಾಲಿಕ_type;
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt() { ಹಿಂತಿರುಗಿ "%llu"; }
    };

    ಮಾದರಿ<> ರಚನೆ Fmt<ದಶಮಾಂಶ>
    {
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದಶಮಾಂಶ ತಾತ್ಕಾಲಿಕ_type;
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt() { ಹಿಂತಿರುಗಿ "%.5g"; }
    };

    ಮಾದರಿ<> ರಚನೆ Fmt<ದ್ವಿಗುಣ>
    {
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದ್ವಿಗುಣ ತಾತ್ಕಾಲಿಕ_type;
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt() { ಹಿಂತಿರುಗಿ "%.5g"; }
    };

    ಮಾದರಿ<> ರಚನೆ Fmt<hfloat>
    {
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದಶಮಾಂಶ ತಾತ್ಕಾಲಿಕ_type;
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt() { ಹಿಂತಿರುಗಿ "%.5g"; }
    };

    ಮಾದರಿ<> ರಚನೆ Fmt<bfloat>
    {
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದಶಮಾಂಶ ತಾತ್ಕಾಲಿಕ_type;
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt() { ಹಿಂತಿರುಗಿ "%.4g"; }
    };

    ಮಾದರಿ<> ರಚನೆ Fmt<fp8_t>
    {
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದಶಮಾಂಶ ತಾತ್ಕಾಲಿಕ_type;
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt() { ಹಿಂತಿರುಗಿ "%.4g"; }
    };

    ಮಾದರಿ<> ರಚನೆ Fmt<fp8a_t>
    {
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದಶಮಾಂಶ ತಾತ್ಕಾಲಿಕ_type;
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt() { ಹಿಂತಿರುಗಿ "%.4g"; }
    };

    ಮಾದರಿ <ಬಗೆ_ಹೆಸರು _Tp>
    ಸ್ಥಿರ ಶೂನ್ಯ pprint_ಅಡ್ಡಸಾಲು(std::ostream& strm, ಸ್ಥಿರಾಂಕ _Tp* ptr, ಪೂರ್ಣಾಂಕ n, size_t ofs, ಪೂರ್ಣಾಂಕ ಅಂಚು)
    {
        ಅಕ್ಷರ buf[128];
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt = Fmt<_Tp>::fmt();
        ಪೂರ್ಣಾಂಕ i, ndump = ಅಂಚು > 0 ? std::min(n, ಅಂಚು*2+1) : n;
        ಆದರೆ (ಅಂಚು == 0)
            ಅಂಚು = ndump;
        ಗಾಗಿ (i = 0; i < ndump; i++) {
            ಪೂರ್ಣಾಂಕ j = n == ndump || i < ಅಂಚು ? i : i == ಅಂಚು ? -1 : n-ಅಂಚು*2-1+i;
            ಆದರೆ (i > 0)
                strm << ", ";
            ಆದರೆ (j >= 0) {
                snprintf(buf, ಗಾತ್ರ(buf), fmt, (ಬಗೆ_ಹೆಸರು Fmt<_Tp>::ತಾತ್ಕಾಲಿಕ_type)ptr[ofs + j]);
                strm << buf;
            } ಇಲ್ಲದಿದ್ದರೆ
                strm << "... ";
        }
    }

    ಸ್ಥಿರ ಶೂನ್ಯ pprint_ತುಂಡು(std::ostream& strm, ಸ್ಥಿರಾಂಕ Mat& tensor,
                            ಸ್ಥಿರಾಂಕ size_t* ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ d,
                            size_t ofs, ಪೂರ್ಣಾಂಕ ಅಂಚು)
    {
        MatShape shape = tensor.shape();
        ಪೂರ್ಣಾಂಕ ndims = shape.dims;
        ಪೂರ್ಣಾಂಕ n = d >= ndims ? 1 : shape[d];
        ಆದರೆ (d >= ndims - 1) {
            ಪೂರ್ಣಾಂಕ typ = tensor.ಆಳ();
            ಶೂನ್ಯ* ದತ್ತಾಂಶ = tensor.ದತ್ತಾಂಶ;
            CV_Assert(ದತ್ತಾಂಶ);
            n *= tensor.channels();
            ಆದರೆ (typ == CV_8U)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ uint8_t*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typ == CV_8S)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ int8_t*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typ == CV_16U)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ uint16_t*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typ == CV_16S)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ int16_t*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typ == CV_32U)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typ == CV_32S)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typ == CV_64U)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ uint64_t*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typ == CV_64S)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ int64_t*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typ == CV_32F)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typ == CV_64F)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typ == CV_16F)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ hfloat*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typ == CV_16BF)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ bfloat*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typ == CV_Bool)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typ == CV_8F_E4M3FN)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ fp8_t*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (typ == CV_8F_E4M3FNUZ)
                pprint_ಅಡ್ಡಸಾಲು(strm, (ಸ್ಥಿರಾಂಕ fp8a_t*)ದತ್ತಾಂಶ, n, ofs, ಅಂಚು);
            ಇಲ್ಲದಿದ್ದರೆ {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಪೂರ್ಣಾಂಕ i, ndump = ಅಂಚು > 0 ? std::min(n, ಅಂಚು*2+1) : n;
            ತಾರ್ಕಿಕ dots = ಸುಳ್ಳು;
            ಗಾಗಿ (i = 0; i < ndump; i++) {
                ಆದರೆ (i > 0 && !dots) {
                    ಪೂರ್ಣಾಂಕ nempty_ಸಾಲುಗಳು = ndims - 2 - d;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < nempty_ಸಾಲುಗಳು; k++)
                        strm << "\n";
                }
                ಆದರೆ (i > 0)
                    strm << "\n";
                ಪೂರ್ಣಾಂಕ j = n == ndump || i < ಅಂಚು ? i :
                        i == ಅಂಚು ? -1 :
                        n - ಅಂಚು*2 - 1 + i;
                dots = j < 0;
                ಆದರೆ (!dots)
                    pprint_ತುಂಡು(strm, tensor, ಹೆಜ್ಜೆ, d+1, ofs + j*ಹೆಜ್ಜೆ[d], ಅಂಚು);
                ಇಲ್ಲದಿದ್ದರೆ
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, ಒಳಹರಿವು_Array array,
                         ಪೂರ್ಣಾಂಕ /*indent*/, ಪೂರ್ಣಾಂಕ ಅಂಚು_2,
                         ಪೂರ್ಣಾಂಕ ಸಂಪೂರ್ಣ_Tensor_ಮಿತಿ,
                         ಅಕ್ಷರ parens)
    {
        ಅಕ್ಷರ oparen = parens;
        ಅಕ್ಷರ cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        ಪೂರ್ಣಾಂಕ ಅಂಚು = ಅಂಚು_2 > 0 ? ಅಂಚು_2 : 3;
        ಸಂಪೂರ್ಣ_Tensor_ಮಿತಿ = ಸಂಪೂರ್ಣ_Tensor_ಮಿತಿ > 0 ? ಸಂಪೂರ್ಣ_Tensor_ಮಿತಿ : 100;

        Mat tensor = array.getMat();
        ಆದರೆ (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat ತಾತ್ಕಾಲಿಕ_2;
            tensor.copy_ಗಮ್ಯ(ತಾತ್ಕಾಲಿಕ_2);
            tensor = ತಾತ್ಕಾಲಿಕ_2;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.ಒಟ್ಟು();

        ಆದರೆ (parens)
            strm << oparen;
        ಆದರೆ (sz_all == 0) {
            ಆದರೆ (!parens)
                strm << "<empty>";
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (sz_all <= (size_t)ಸಂಪೂರ್ಣ_Tensor_ಮಿತಿ)
                ಅಂಚು = 0;

            ಪೂರ್ಣಾಂಕ ndims = shape.dims;
            ಪೂರ್ಣಾಂಕ cn = tensor.channels();
            size_t ಹೆಜ್ಜೆ[MatShape::MAX_DIMS];
            ಹೆಜ್ಜೆ[std::max(ndims-1, 0)] = 1;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ndims-2; i >= 0; i--) {
                ಹೆಜ್ಜೆ[i] = ಹೆಜ್ಜೆ[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprint_ತುಂಡು(strm, tensor, ಹೆಜ್ಜೆ, 0, 0, ಅಂಚು);
        }
        ಆದರೆ (parens)
            strm << cparen;
        ಹಿಂತಿರುಗಿ strm;
    }

} // cv
