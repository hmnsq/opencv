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
        ಗಣನಾ { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_ಮೌಲ್ಯ, STATE_FINISHED,
               STATE_ಸಾಲು_SEPARATOR, STATE_CN_SEPARATOR, STATE_ಮೌಲ್ಯ_SEPARATOR };
        ಗಣನಾ {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        ವರ್ಣ floatFormat[8];
        ವರ್ಣ buf[32];   // enough for double with precision up to 20

        Mat mtx;
        ಪೂರ್ಣಾಙ್ಕ mcn; // == mtx.channels()
        ತಾರ್ಕಿಕ single_ಸಾಲು;
        ತಾರ್ಕಿಕ alignOrder;    // true when cn first order

        ಪೂರ್ಣಾಙ್ಕ state;
        ಪೂರ್ಣಾಙ್ಕ row;
        ಪೂರ್ಣಾಙ್ಕ col;
        ಪೂರ್ಣಾಙ್ಕ cn;

        String prologue;
        String epilogue;
        ವರ್ಣ braces[5];

        ಶೂನ್ಯ (FormattedImpl::*ಮೌಲ್ಯ_To_Str)();
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str_Bool() { snprintf(buf, ಪರಿಮಾಣ(buf), "%d", (ಪೂರ್ಣಾಙ್ಕ)mtx.ptr<uchar>(row, col)[cn] != 0); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str8u()  { snprintf(buf, ಪರಿಮಾಣ(buf), "%3d", (ಪೂರ್ಣಾಙ್ಕ)mtx.ptr<uchar>(row, col)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str8s()  { snprintf(buf, ಪರಿಮಾಣ(buf), "%3d", (ಪೂರ್ಣಾಙ್ಕ)mtx.ptr<schar>(row, col)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str16u() { snprintf(buf, ಪರಿಮಾಣ(buf), "%d", (ಪೂರ್ಣಾಙ್ಕ)mtx.ptr<ushort>(row, col)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str16s() { snprintf(buf, ಪರಿಮಾಣ(buf), "%d", (ಪೂರ್ಣಾಙ್ಕ)mtx.ptr<short>(row, col)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str32u() { snprintf(buf, ಪರಿಮಾಣ(buf), "%u", mtx.ptr<unsigned>(row, col)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str32s() { snprintf(buf, ಪರಿಮಾಣ(buf), "%d", mtx.ptr<ಪೂರ್ಣಾಙ್ಕ>(row, col)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str32f() { snprintf(buf, ಪರಿಮಾಣ(buf), floatFormat, mtx.ptr<ಭಿನ್ನ>(row, col)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str64f() { snprintf(buf, ಪರಿಮಾಣ(buf), floatFormat, mtx.ptr<ದ್ವಿಗುಣ>(row, col)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str64u() { snprintf(buf, ಪರಿಮಾಣ(buf), "%llu", (unsigned long long)mtx.ptr<uint64_t>(row, col)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str64s() { snprintf(buf, ಪರಿಮಾಣ(buf), "%lld", (long long)mtx.ptr<int64_t>(row, col)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str16f() { snprintf(buf, ಪರಿಮಾಣ(buf), floatFormat, (ಭಿನ್ನ)mtx.ptr<hfloat>(row, col)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str16bf() { snprintf(buf, ಪರಿಮಾಣ(buf), floatFormat, (ಭಿನ್ನ)mtx.ptr<bfloat>(row, col)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str8f()   { snprintf(buf, ಪರಿಮಾಣ(buf), floatFormat, (ಭಿನ್ನ)mtx.ptr<fp8_t>(row, col)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str8fnuz() { snprintf(buf, ಪರಿಮಾಣ(buf), floatFormat, (ಭಿನ್ನ)mtx.ptr<fp8a_t>(row, col)[cn]); }
        ಶೂನ್ಯ ಮೌಲ್ಯ_To_Str_Other() { buf[0] = 0; }

    ಸಾರ್ವಜನಿಕ:

        FormattedImpl(String pl, String el, Mat m, ವರ್ಣ br[5], ತಾರ್ಕಿಕ s_ಸಾಲು, ತಾರ್ಕಿಕ aOrder, ಪೂರ್ಣಾಙ್ಕ precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            memcpy(braces, br, 5);
            state = STATE_PROLOGUE;
            single_ಸಾಲು = s_ಸಾಲು;
            alignOrder = aOrder;
            row = col = cn =0;

            ಯದಿ (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            ಅನ್ಯಥಾ
            {
                cv_snprintf(floatFormat, ಪರಿಮಾಣ(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            ವಿಕಲ್ಪ(mtx.depth())
            {
                ಪಕ್ಷ CV_8U:  ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str8u; ಭಙ್ಗ;
                ಪಕ್ಷ CV_8S:  ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str8s; ಭಙ್ಗ;
                ಪಕ್ಷ CV_Bool: ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str_Bool; ಭಙ್ಗ;
                ಪಕ್ಷ CV_16U: ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str16u; ಭಙ್ಗ;
                ಪಕ್ಷ CV_16S: ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str16s; ಭಙ್ಗ;
                ಪಕ್ಷ CV_32U: ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str32u; ಭಙ್ಗ;
                ಪಕ್ಷ CV_32S: ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str32s; ಭಙ್ಗ;
                ಪಕ್ಷ CV_32F: ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str32f; ಭಙ್ಗ;
                ಪಕ್ಷ CV_64F: ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str64f; ಭಙ್ಗ;
                ಪಕ್ಷ CV_64U: ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str64u; ಭಙ್ಗ;
                ಪಕ್ಷ CV_64S: ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str64s; ಭಙ್ಗ;
                ಪಕ್ಷ CV_16F: ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str16f; ಭಙ್ಗ;
                ಪಕ್ಷ CV_16BF: ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str16bf; ಭಙ್ಗ;
                ಪಕ್ಷ CV_8F_E4M3FN:   ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str8f; ಭಙ್ಗ;
                ಪಕ್ಷ CV_8F_E4M3FNUZ: ಮೌಲ್ಯ_To_Str = &FormattedImpl::ಮೌಲ್ಯ_To_Str8fnuz; ಭಙ್ಗ;
                ಸ್ವಭಾವ:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.depth()));
            }
        }

        ಶೂನ್ಯ reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        ಧ್ರುವ ವರ್ಣ* next() CV_OVERRIDE
        {
            ವಿಕಲ್ಪ(state)
            {
                ಪಕ್ಷ STATE_PROLOGUE:
                    row = 0;
                    ಯದಿ (mtx.empty())
                        state = STATE_EPILOGUE;
                    ಅನ್ಯಥಾ ಯದಿ (alignOrder)
                        state = STATE_INTERLUDE;
                    ಅನ್ಯಥಾ
                        state = STATE_ROW_OPEN;
                    ಪ್ರತ್ಯಾಗಮ prologue.c_str();
                ಪಕ್ಷ STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    ಯದಿ (row >= mtx.rows)
                    {
                        ಯದಿ (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            ಪ್ರತ್ಯಾಗಮ buf;
                        }
                        ಅನ್ಯಥಾ
                            row = 0;
                        snprintf(buf, ಪರಿಮಾಣ(buf), "\n(:, :, %d) = \n", cn+1);
                        ಪ್ರತ್ಯಾಗಮ buf;
                    }
                    snprintf(buf, ಪರಿಮಾಣ(buf), "(:, :, %d) = \n", cn+1);
                    ಪ್ರತ್ಯಾಗಮ buf;
                ಪಕ್ಷ STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    ಪ್ರತ್ಯಾಗಮ epilogue.c_str();
                ಪಕ್ಷ STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        ಯದಿ (row > 0)
                            ಯಾವತ್(pos < prologue.size() && pos < ಪರಿಮಾಣ(buf) - 2)
                                buf[pos++] = ' ';
                        ಯದಿ (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        ಯದಿ(!pos)
                            ಪ್ರತ್ಯಾಗಮ next();
                        buf[pos] = 0;
                    }
                    ಪ್ರತ್ಯಾಗಮ buf;
                ಪಕ್ಷ STATE_ROW_CLOSE:
                    state = STATE_ಸಾಲು_SEPARATOR;
                    ++row;
                    ಯದಿ (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        ಪ್ರತ್ಯಾಗಮ buf;
                    }
                    ಅನ್ಯಥಾ ಯದಿ(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        ಪ್ರತ್ಯಾಗಮ buf;
                    }
                    ಪ್ರತ್ಯಾಗಮ next();
                ಪಕ್ಷ STATE_CN_OPEN:
                    state = STATE_ಮೌಲ್ಯ;
                    ಯದಿ (!alignOrder)
                        cn = 0;
                    ಯದಿ (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        ಪ್ರತ್ಯಾಗಮ buf;
                    }
                    ಪ್ರತ್ಯಾಗಮ next();
                ಪಕ್ಷ STATE_CN_CLOSE:
                    ++col;
                    ಯದಿ (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    ಅನ್ಯಥಾ
                        state = STATE_CN_SEPARATOR;
                    ಯದಿ (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        ಪ್ರತ್ಯಾಗಮ buf;
                    }
                    ಪ್ರತ್ಯಾಗಮ next();
                ಪಕ್ಷ STATE_ಮೌಲ್ಯ:
                    (ಏತತ್->*ಮೌಲ್ಯ_To_Str)();
                    state = STATE_CN_CLOSE;
                    ಯದಿ (alignOrder)
                        ಪ್ರತ್ಯಾಗಮ buf;
                    ಯದಿ (++cn < mcn)
                        state = STATE_ಮೌಲ್ಯ_SEPARATOR;
                    ಪ್ರತ್ಯಾಗಮ buf;
                ಪಕ್ಷ STATE_FINISHED:
                    ಪ್ರತ್ಯಾಗಮ 0;
                ಪಕ್ಷ STATE_ಸಾಲು_SEPARATOR:
                    ಯದಿ (row >= mtx.rows)
                    {
                        ಯದಿ (alignOrder)
                            state = STATE_INTERLUDE;
                        ಅನ್ಯಥಾ
                            state = STATE_EPILOGUE;
                        ಪ್ರತ್ಯಾಗಮ next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = single_ಸಾಲು ? ' ' : '\n';
                    buf[1] = 0;
                    ಪ್ರತ್ಯಾಗಮ buf;
                ಪಕ್ಷ STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    ಪ್ರತ್ಯಾಗಮ buf;
                ಪಕ್ಷ STATE_ಮೌಲ್ಯ_SEPARATOR:
                    state = STATE_ಮೌಲ್ಯ;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    ಪ್ರತ್ಯಾಗಮ buf;
            }
            ಪ್ರತ್ಯಾಗಮ 0;
        }
    };

    ವರ್ಗ FormatterBase : ಸಾರ್ವಜನಿಕ Formatter
    {
    ಸಾರ್ವಜನಿಕ:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(ಸತ್ಯ) {}

        ಶೂನ್ಯ set16fPrecision(ಪೂರ್ಣಾಙ್ಕ p) CV_OVERRIDE
        {
            prec16f = p;
        }

        ಶೂನ್ಯ set32fPrecision(ಪೂರ್ಣಾಙ್ಕ p) CV_OVERRIDE
        {
            prec32f = p;
        }

        ಶೂನ್ಯ set64fPrecision(ಪೂರ್ಣಾಙ್ಕ p) CV_OVERRIDE
        {
            prec64f = p;
        }

        ಶೂನ್ಯ setMultiline(ತಾರ್ಕಿಕ ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    ರಕ್ಷಿತ:
        ಪೂರ್ಣಾಙ್ಕ prec16f;
        ಪೂರ್ಣಾಙ್ಕ prec32f;
        ಪೂರ್ಣಾಙ್ಕ prec64f;
        ಪೂರ್ಣಾಙ್ಕ multiline;
    };

    ವರ್ಗ DefaultFormatter CV_FINAL : ಸಾರ್ವಜನಿಕ FormatterBase
    {
    ಸಾರ್ವಜನಿಕ:

        Ptr<Formatted> format(ಧ್ರುವ Mat& mtx) ಧ್ರುವ CV_OVERRIDE
        {
            ವರ್ಣ braces[5] = {'\0', '\0', ';', '\0', '\0'};
            ಪ್ರತ್ಯಾಗಮ makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, ಅಸತ್ಯ, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    ವರ್ಗ MatlabFormatter CV_FINAL : ಸಾರ್ವಜನಿಕ FormatterBase
    {
    ಸಾರ್ವಜನಿಕ:

        Ptr<Formatted> format(ಧ್ರುವ Mat& mtx) ಧ್ರುವ CV_OVERRIDE
        {
            ವರ್ಣ braces[5] = {'\0', '\0', ';', '\0', '\0'};
            ಪ್ರತ್ಯಾಗಮ makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, ಸತ್ಯ, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    ವರ್ಗ PythonFormatter CV_FINAL : ಸಾರ್ವಜನಿಕ FormatterBase
    {
    ಸಾರ್ವಜನಿಕ:

        Ptr<Formatted> format(ಧ್ರುವ Mat& mtx) ಧ್ರುವ CV_OVERRIDE
        {
            ವರ್ಣ braces[5] = {'[', ']', ',', '[', ']'};
            ಯದಿ (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            ಪ್ರತ್ಯಾಗಮ makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, ಅಸತ್ಯ, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    ವರ್ಗ NumpyFormatter CV_FINAL : ಸಾರ್ವಜನಿಕ FormatterBase
    {
    ಸಾರ್ವಜನಿಕ:

        Ptr<Formatted> format(ಧ್ರುವ Mat& mtx) ಧ್ರುವ CV_OVERRIDE
        {
            ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* numpyTypes[CV_DEPTH_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            ವರ್ಣ braces[5] = {'[', ']', ',', '[', ']'};
            ಯದಿ (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            ಪ್ರತ್ಯಾಗಮ makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.depth()]), mtx, &*braces,
                mtx.rows == 1 || !multiline, ಅಸತ್ಯ, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    ವರ್ಗ CSVFormatter CV_FINAL : ಸಾರ್ವಜನಿಕ FormatterBase
    {
    ಸಾರ್ವಜನಿಕ:

        Ptr<Formatted> format(ಧ್ರುವ Mat& mtx) ಧ್ರುವ CV_OVERRIDE
        {
            ವರ್ಣ braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            ಪ್ರತ್ಯಾಗಮ makePtr<FormattedImpl>(String(),
                mtx.rows > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.rows == 1 || !multiline, ಅಸತ್ಯ, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    ವರ್ಗ CFormatter CV_FINAL : ಸಾರ್ವಜನಿಕ FormatterBase
    {
    ಸಾರ್ವಜನಿಕ:

        Ptr<Formatted> format(ಧ್ರುವ Mat& mtx) ಧ್ರುವ CV_OVERRIDE
        {
            ವರ್ಣ braces[5] = {'\0', '\0', ',', '\0', '\0'};
            ಪ್ರತ್ಯಾಗಮ makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.rows == 1 || !multiline, ಅಸತ್ಯ, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        ವಿಕಲ್ಪ(fmt)
        {
            ಪಕ್ಷ FMT_DEFAULT:
                ಪ್ರತ್ಯಾಗಮ makePtr<DefaultFormatter>();
            ಪಕ್ಷ FMT_MATLAB:
                ಪ್ರತ್ಯಾಗಮ makePtr<MatlabFormatter>();
            ಪಕ್ಷ FMT_CSV:
                ಪ್ರತ್ಯಾಗಮ makePtr<CSVFormatter>();
            ಪಕ್ಷ FMT_PYTHON:
                ಪ್ರತ್ಯಾಗಮ makePtr<PythonFormatter>();
            ಪಕ್ಷ FMT_NUMPY:
                ಪ್ರತ್ಯಾಗಮ makePtr<NumpyFormatter>();
            ಪಕ್ಷ FMT_C:
                ಪ್ರತ್ಯಾಗಮ makePtr<CFormatter>();
        }
        ಪ್ರತ್ಯಾಗಮ makePtr<DefaultFormatter>();
    }

    template<typename _Tp> ಸಂರಚನಾ Fmt
    {
        ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ temp_type;
        ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* fmt() { ಪ್ರತ್ಯಾಗಮ "%d"; }
    };

    template<> ಸಂರಚನಾ Fmt<uint32_t>
    {
        ಪ್ರಕಾರನಿರ್ದೇಶ unsigned temp_type;
        ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* fmt() { ಪ್ರತ್ಯಾಗಮ "%u"; }
    };

    template<> ಸಂರಚನಾ Fmt<int64_t>
    {
        ಪ್ರಕಾರನಿರ್ದೇಶ long long temp_type;
        ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* fmt() { ಪ್ರತ್ಯಾಗಮ "%lld"; }
    };

    template<> ಸಂರಚನಾ Fmt<uint64_t>
    {
        ಪ್ರಕಾರನಿರ್ದೇಶ unsigned long long temp_type;
        ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* fmt() { ಪ್ರತ್ಯಾಗಮ "%llu"; }
    };

    template<> ಸಂರಚನಾ Fmt<ಭಿನ್ನ>
    {
        ಪ್ರಕಾರನಿರ್ದೇಶ ಭಿನ್ನ temp_type;
        ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* fmt() { ಪ್ರತ್ಯಾಗಮ "%.5g"; }
    };

    template<> ಸಂರಚನಾ Fmt<ದ್ವಿಗುಣ>
    {
        ಪ್ರಕಾರನಿರ್ದೇಶ ದ್ವಿಗುಣ temp_type;
        ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* fmt() { ಪ್ರತ್ಯಾಗಮ "%.5g"; }
    };

    template<> ಸಂರಚನಾ Fmt<hfloat>
    {
        ಪ್ರಕಾರನಿರ್ದೇಶ ಭಿನ್ನ temp_type;
        ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* fmt() { ಪ್ರತ್ಯಾಗಮ "%.5g"; }
    };

    template<> ಸಂರಚನಾ Fmt<bfloat>
    {
        ಪ್ರಕಾರನಿರ್ದೇಶ ಭಿನ್ನ temp_type;
        ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* fmt() { ಪ್ರತ್ಯಾಗಮ "%.4g"; }
    };

    template<> ಸಂರಚನಾ Fmt<fp8_t>
    {
        ಪ್ರಕಾರನಿರ್ದೇಶ ಭಿನ್ನ temp_type;
        ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* fmt() { ಪ್ರತ್ಯಾಗಮ "%.4g"; }
    };

    template<> ಸಂರಚನಾ Fmt<fp8a_t>
    {
        ಪ್ರಕಾರನಿರ್ದೇಶ ಭಿನ್ನ temp_type;
        ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* fmt() { ಪ್ರತ್ಯಾಗಮ "%.4g"; }
    };

    template <typename _Tp>
    ಸ್ಥಿರ ಶೂನ್ಯ pprintRow(std::ostream& strm, ಧ್ರುವ _Tp* ptr, ಪೂರ್ಣಾಙ್ಕ n, size_t ofs, ಪೂರ್ಣಾಙ್ಕ edge)
    {
        ವರ್ಣ buf[128];
        ಧ್ರುವ ವರ್ಣ* fmt = Fmt<_Tp>::fmt();
        ಪೂರ್ಣಾಙ್ಕ i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        ಯದಿ (edge == 0)
            edge = ndump;
        ಪ್ರತಿ (i = 0; i < ndump; i++) {
            ಪೂರ್ಣಾಙ್ಕ j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            ಯದಿ (i > 0)
                strm << ", ";
            ಯದಿ (j >= 0) {
                snprintf(buf, ಪರಿಮಾಣ(buf), fmt, (typename Fmt<_Tp>::temp_type)ptr[ofs + j]);
                strm << buf;
            } ಅನ್ಯಥಾ
                strm << "... ";
        }
    }

    ಸ್ಥಿರ ಶೂನ್ಯ pprintSlice(std::ostream& strm, ಧ್ರುವ Mat& tensor,
                            ಧ್ರುವ size_t* step, ಪೂರ್ಣಾಙ್ಕ d,
                            size_t ofs, ಪೂರ್ಣಾಙ್ಕ edge)
    {
        MatShape shape = tensor.shape();
        ಪೂರ್ಣಾಙ್ಕ ndims = shape.dims;
        ಪೂರ್ಣಾಙ್ಕ n = d >= ndims ? 1 : shape[d];
        ಯದಿ (d >= ndims - 1) {
            ಪೂರ್ಣಾಙ್ಕ typ = tensor.depth();
            ಶೂನ್ಯ* data = tensor.data;
            CV_Assert(data);
            n *= tensor.channels();
            ಯದಿ (typ == CV_8U)
                pprintRow(strm, (ಧ್ರುವ uint8_t*)data, n, ofs, edge);
            ಅನ್ಯಥಾ ಯದಿ (typ == CV_8S)
                pprintRow(strm, (ಧ್ರುವ int8_t*)data, n, ofs, edge);
            ಅನ್ಯಥಾ ಯದಿ (typ == CV_16U)
                pprintRow(strm, (ಧ್ರುವ uint16_t*)data, n, ofs, edge);
            ಅನ್ಯಥಾ ಯದಿ (typ == CV_16S)
                pprintRow(strm, (ಧ್ರುವ int16_t*)data, n, ofs, edge);
            ಅನ್ಯಥಾ ಯದಿ (typ == CV_32U)
                pprintRow(strm, (ಧ್ರುವ unsigned*)data, n, ofs, edge);
            ಅನ್ಯಥಾ ಯದಿ (typ == CV_32S)
                pprintRow(strm, (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)data, n, ofs, edge);
            ಅನ್ಯಥಾ ಯದಿ (typ == CV_64U)
                pprintRow(strm, (ಧ್ರುವ uint64_t*)data, n, ofs, edge);
            ಅನ್ಯಥಾ ಯದಿ (typ == CV_64S)
                pprintRow(strm, (ಧ್ರುವ int64_t*)data, n, ofs, edge);
            ಅನ್ಯಥಾ ಯದಿ (typ == CV_32F)
                pprintRow(strm, (ಧ್ರುವ ಭಿನ್ನ*)data, n, ofs, edge);
            ಅನ್ಯಥಾ ಯದಿ (typ == CV_64F)
                pprintRow(strm, (ಧ್ರುವ ದ್ವಿಗುಣ*)data, n, ofs, edge);
            ಅನ್ಯಥಾ ಯದಿ (typ == CV_16F)
                pprintRow(strm, (ಧ್ರುವ hfloat*)data, n, ofs, edge);
            ಅನ್ಯಥಾ ಯದಿ (typ == CV_16BF)
                pprintRow(strm, (ಧ್ರುವ bfloat*)data, n, ofs, edge);
            ಅನ್ಯಥಾ ಯದಿ (typ == CV_Bool)
                pprintRow(strm, (ಧ್ರುವ ತಾರ್ಕಿಕ*)data, n, ofs, edge);
            ಅನ್ಯಥಾ ಯದಿ (typ == CV_8F_E4M3FN)
                pprintRow(strm, (ಧ್ರುವ fp8_t*)data, n, ofs, edge);
            ಅನ್ಯಥಾ ಯದಿ (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (ಧ್ರುವ fp8a_t*)data, n, ofs, edge);
            ಅನ್ಯಥಾ {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } ಅನ್ಯಥಾ {
            ಪೂರ್ಣಾಙ್ಕ i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            ತಾರ್ಕಿಕ dots = ಅಸತ್ಯ;
            ಪ್ರತಿ (i = 0; i < ndump; i++) {
                ಯದಿ (i > 0 && !dots) {
                    ಪೂರ್ಣಾಙ್ಕ nempty_ಸಾಲುಗಳು = ndims - 2 - d;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < nempty_ಸಾಲುಗಳು; k++)
                        strm << "\n";
                }
                ಯದಿ (i > 0)
                    strm << "\n";
                ಪೂರ್ಣಾಙ್ಕ j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                ಯದಿ (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                ಅನ್ಯಥಾ
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, ಒಳಹರಿವು_Array array,
                         ಪೂರ್ಣಾಙ್ಕ /*indent*/, ಪೂರ್ಣಾಙ್ಕ edge_,
                         ಪೂರ್ಣಾಙ್ಕ wholeTensorThreshold,
                         ವರ್ಣ parens)
    {
        ವರ್ಣ oparen = parens;
        ವರ್ಣ cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        ಪೂರ್ಣಾಙ್ಕ edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        ಯದಿ (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat temp;
            tensor.copyTo(temp);
            tensor = temp;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.total();

        ಯದಿ (parens)
            strm << oparen;
        ಯದಿ (sz_all == 0) {
            ಯದಿ (!parens)
                strm << "<empty>";
        } ಅನ್ಯಥಾ {
            ಯದಿ (sz_all <= (size_t)wholeTensorThreshold)
                edge = 0;

            ಪೂರ್ಣಾಙ್ಕ ndims = shape.dims;
            ಪೂರ್ಣಾಙ್ಕ cn = tensor.channels();
            size_t step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        ಯದಿ (parens)
            strm << cparen;
        ಪ್ರತ್ಯಾಗಮ strm;
    }

} // cv
