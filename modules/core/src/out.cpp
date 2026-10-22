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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

ନାମାକାଶ cv
{
    ଶ୍ରେଣୀ FormattedImpl CV_FINAL : ସର୍ବସାଧାରଣ Formatted
    {
        ଗଣନା { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_ମୂଲ୍ୟ, STATE_FINISHED,
               STATE_ଧାଡ଼ି_SEPARATOR, STATE_CN_SEPARATOR, STATE_ମୂଲ୍ୟ_SEPARATOR };
        ଗଣନା {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        ଅକ୍ଷର floatFormat[8];
        ଅକ୍ଷର buf[32];   // enough for double with precision up to 20

        Mat mtx;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା mcn; // == mtx.channels()
        ତାର୍କିକ single_ଧାଡ଼ି;
        ତାର୍କିକ alignOrder;    // true when cn first order

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା state;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା row;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା col;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn;

        String prologue;
        String epilogue;
        ଅକ୍ଷର braces[5];

        ଶୂନ୍ୟ (FormattedImpl::*ମୂଲ୍ୟ_To_Str)();
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str_Bool() { snprintf(buf, ଆକାର(buf), "%d", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)mtx.ptr<uchar>(row, col)[cn] != 0); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str8u()  { snprintf(buf, ଆକାର(buf), "%3d", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)mtx.ptr<uchar>(row, col)[cn]); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str8s()  { snprintf(buf, ଆକାର(buf), "%3d", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)mtx.ptr<schar>(row, col)[cn]); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str16u() { snprintf(buf, ଆକାର(buf), "%d", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)mtx.ptr<ushort>(row, col)[cn]); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str16s() { snprintf(buf, ଆକାର(buf), "%d", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)mtx.ptr<short>(row, col)[cn]); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str32u() { snprintf(buf, ଆକାର(buf), "%u", mtx.ptr<unsigned>(row, col)[cn]); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str32s() { snprintf(buf, ଆକାର(buf), "%d", mtx.ptr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(row, col)[cn]); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str32f() { snprintf(buf, ଆକାର(buf), floatFormat, mtx.ptr<ଦଶମିକ>(row, col)[cn]); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str64f() { snprintf(buf, ଆକାର(buf), floatFormat, mtx.ptr<ଦ୍ୱିଗୁଣ>(row, col)[cn]); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str64u() { snprintf(buf, ଆକାର(buf), "%llu", (unsigned long long)mtx.ptr<uint64_t>(row, col)[cn]); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str64s() { snprintf(buf, ଆକାର(buf), "%lld", (long long)mtx.ptr<int64_t>(row, col)[cn]); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str16f() { snprintf(buf, ଆକାର(buf), floatFormat, (ଦଶମିକ)mtx.ptr<hfloat>(row, col)[cn]); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str16bf() { snprintf(buf, ଆକାର(buf), floatFormat, (ଦଶମିକ)mtx.ptr<bfloat>(row, col)[cn]); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str8f()   { snprintf(buf, ଆକାର(buf), floatFormat, (ଦଶମିକ)mtx.ptr<fp8_t>(row, col)[cn]); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str8fnuz() { snprintf(buf, ଆକାର(buf), floatFormat, (ଦଶମିକ)mtx.ptr<fp8a_t>(row, col)[cn]); }
        ଶୂନ୍ୟ ମୂଲ୍ୟ_To_Str_Other() { buf[0] = 0; }

    ସର୍ବସାଧାରଣ:

        FormattedImpl(String pl, String el, Mat m, ଅକ୍ଷର br[5], ତାର୍କିକ s_ଧାଡ଼ି, ତାର୍କିକ aOrder, ପୂର୍ଣ୍ଣସଂଖ୍ୟା precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            memcpy(braces, br, 5);
            state = STATE_PROLOGUE;
            single_ଧାଡ଼ି = s_ଧାଡ଼ି;
            alignOrder = aOrder;
            row = col = cn =0;

            ଯଦି (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            ଅନ୍ୟଥା
            {
                cv_snprintf(floatFormat, ଆକାର(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            ବାଛ(mtx.depth())
            {
                ସ୍ଥିତି CV_8U:  ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str8u; ଭାଙ୍ଗ;
                ସ୍ଥିତି CV_8S:  ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str8s; ଭାଙ୍ଗ;
                ସ୍ଥିତି CV_Bool: ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str_Bool; ଭାଙ୍ଗ;
                ସ୍ଥିତି CV_16U: ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str16u; ଭାଙ୍ଗ;
                ସ୍ଥିତି CV_16S: ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str16s; ଭାଙ୍ଗ;
                ସ୍ଥିତି CV_32U: ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str32u; ଭାଙ୍ଗ;
                ସ୍ଥିତି CV_32S: ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str32s; ଭାଙ୍ଗ;
                ସ୍ଥିତି CV_32F: ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str32f; ଭାଙ୍ଗ;
                ସ୍ଥିତି CV_64F: ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str64f; ଭାଙ୍ଗ;
                ସ୍ଥିତି CV_64U: ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str64u; ଭାଙ୍ଗ;
                ସ୍ଥିତି CV_64S: ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str64s; ଭାଙ୍ଗ;
                ସ୍ଥିତି CV_16F: ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str16f; ଭାଙ୍ଗ;
                ସ୍ଥିତି CV_16BF: ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str16bf; ଭାଙ୍ଗ;
                ସ୍ଥିତି CV_8F_E4M3FN:   ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str8f; ଭାଙ୍ଗ;
                ସ୍ଥିତି CV_8F_E4M3FNUZ: ମୂଲ୍ୟ_To_Str = &FormattedImpl::ମୂଲ୍ୟ_To_Str8fnuz; ଭାଙ୍ଗ;
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.depth()));
            }
        }

        ଶୂନ୍ୟ reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        ଅଚଳ ଅକ୍ଷର* next() CV_OVERRIDE
        {
            ବାଛ(state)
            {
                ସ୍ଥିତି STATE_PROLOGUE:
                    row = 0;
                    ଯଦି (mtx.empty())
                        state = STATE_EPILOGUE;
                    ଅନ୍ୟଥା ଯଦି (alignOrder)
                        state = STATE_INTERLUDE;
                    ଅନ୍ୟଥା
                        state = STATE_ROW_OPEN;
                    ଫେରନ୍ତୁ prologue.c_str();
                ସ୍ଥିତି STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    ଯଦି (row >= mtx.rows)
                    {
                        ଯଦି (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            ଫେରନ୍ତୁ buf;
                        }
                        ଅନ୍ୟଥା
                            row = 0;
                        snprintf(buf, ଆକାର(buf), "\n(:, :, %d) = \n", cn+1);
                        ଫେରନ୍ତୁ buf;
                    }
                    snprintf(buf, ଆକାର(buf), "(:, :, %d) = \n", cn+1);
                    ଫେରନ୍ତୁ buf;
                ସ୍ଥିତି STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    ଫେରନ୍ତୁ epilogue.c_str();
                ସ୍ଥିତି STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        ଯଦି (row > 0)
                            ଯେତେବେଳେ(pos < prologue.size() && pos < ଆକାର(buf) - 2)
                                buf[pos++] = ' ';
                        ଯଦି (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        ଯଦି(!pos)
                            ଫେରନ୍ତୁ next();
                        buf[pos] = 0;
                    }
                    ଫେରନ୍ତୁ buf;
                ସ୍ଥିତି STATE_ROW_CLOSE:
                    state = STATE_ଧାଡ଼ି_SEPARATOR;
                    ++row;
                    ଯଦି (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        ଫେରନ୍ତୁ buf;
                    }
                    ଅନ୍ୟଥା ଯଦି(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        ଫେରନ୍ତୁ buf;
                    }
                    ଫେରନ୍ତୁ next();
                ସ୍ଥିତି STATE_CN_OPEN:
                    state = STATE_ମୂଲ୍ୟ;
                    ଯଦି (!alignOrder)
                        cn = 0;
                    ଯଦି (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        ଫେରନ୍ତୁ buf;
                    }
                    ଫେରନ୍ତୁ next();
                ସ୍ଥିତି STATE_CN_CLOSE:
                    ++col;
                    ଯଦି (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    ଅନ୍ୟଥା
                        state = STATE_CN_SEPARATOR;
                    ଯଦି (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        ଫେରନ୍ତୁ buf;
                    }
                    ଫେରନ୍ତୁ next();
                ସ୍ଥିତି STATE_ମୂଲ୍ୟ:
                    (ଏହି->*ମୂଲ୍ୟ_To_Str)();
                    state = STATE_CN_CLOSE;
                    ଯଦି (alignOrder)
                        ଫେରନ୍ତୁ buf;
                    ଯଦି (++cn < mcn)
                        state = STATE_ମୂଲ୍ୟ_SEPARATOR;
                    ଫେରନ୍ତୁ buf;
                ସ୍ଥିତି STATE_FINISHED:
                    ଫେରନ୍ତୁ 0;
                ସ୍ଥିତି STATE_ଧାଡ଼ି_SEPARATOR:
                    ଯଦି (row >= mtx.rows)
                    {
                        ଯଦି (alignOrder)
                            state = STATE_INTERLUDE;
                        ଅନ୍ୟଥା
                            state = STATE_EPILOGUE;
                        ଫେରନ୍ତୁ next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = single_ଧାଡ଼ି ? ' ' : '\n';
                    buf[1] = 0;
                    ଫେରନ୍ତୁ buf;
                ସ୍ଥିତି STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    ଫେରନ୍ତୁ buf;
                ସ୍ଥିତି STATE_ମୂଲ୍ୟ_SEPARATOR:
                    state = STATE_ମୂଲ୍ୟ;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    ଫେରନ୍ତୁ buf;
            }
            ଫେରନ୍ତୁ 0;
        }
    };

    ଶ୍ରେଣୀ FormatterBase : ସର୍ବସାଧାରଣ Formatter
    {
    ସର୍ବସାଧାରଣ:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(ସତ୍ୟ) {}

        ଶୂନ୍ୟ set16fPrecision(ପୂର୍ଣ୍ଣସଂଖ୍ୟା p) CV_OVERRIDE
        {
            prec16f = p;
        }

        ଶୂନ୍ୟ set32fPrecision(ପୂର୍ଣ୍ଣସଂଖ୍ୟା p) CV_OVERRIDE
        {
            prec32f = p;
        }

        ଶୂନ୍ୟ set64fPrecision(ପୂର୍ଣ୍ଣସଂଖ୍ୟା p) CV_OVERRIDE
        {
            prec64f = p;
        }

        ଶୂନ୍ୟ setMultiline(ତାର୍କିକ ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    ସୁରକ୍ଷିତ:
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା prec16f;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା prec32f;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା prec64f;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା multiline;
    };

    ଶ୍ରେଣୀ DefaultFormatter CV_FINAL : ସର୍ବସାଧାରଣ FormatterBase
    {
    ସର୍ବସାଧାରଣ:

        Ptr<Formatted> format(ଅଚଳ Mat& mtx) ଅଚଳ CV_OVERRIDE
        {
            ଅକ୍ଷର braces[5] = {'\0', '\0', ';', '\0', '\0'};
            ଫେରନ୍ତୁ makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, ମିଥ୍ୟା, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    ଶ୍ରେଣୀ MatlabFormatter CV_FINAL : ସର୍ବସାଧାରଣ FormatterBase
    {
    ସର୍ବସାଧାରଣ:

        Ptr<Formatted> format(ଅଚଳ Mat& mtx) ଅଚଳ CV_OVERRIDE
        {
            ଅକ୍ଷର braces[5] = {'\0', '\0', ';', '\0', '\0'};
            ଫେରନ୍ତୁ makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, ସତ୍ୟ, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    ଶ୍ରେଣୀ PythonFormatter CV_FINAL : ସର୍ବସାଧାରଣ FormatterBase
    {
    ସର୍ବସାଧାରଣ:

        Ptr<Formatted> format(ଅଚଳ Mat& mtx) ଅଚଳ CV_OVERRIDE
        {
            ଅକ୍ଷର braces[5] = {'[', ']', ',', '[', ']'};
            ଯଦି (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            ଫେରନ୍ତୁ makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, ମିଥ୍ୟା, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    ଶ୍ରେଣୀ NumpyFormatter CV_FINAL : ସର୍ବସାଧାରଣ FormatterBase
    {
    ସର୍ବସାଧାରଣ:

        Ptr<Formatted> format(ଅଚଳ Mat& mtx) ଅଚଳ CV_OVERRIDE
        {
            ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* numpyTypes[CV_DEPTH_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            ଅକ୍ଷର braces[5] = {'[', ']', ',', '[', ']'};
            ଯଦି (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            ଫେରନ୍ତୁ makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.depth()]), mtx, &*braces,
                mtx.rows == 1 || !multiline, ମିଥ୍ୟା, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    ଶ୍ରେଣୀ CSVFormatter CV_FINAL : ସର୍ବସାଧାରଣ FormatterBase
    {
    ସର୍ବସାଧାରଣ:

        Ptr<Formatted> format(ଅଚଳ Mat& mtx) ଅଚଳ CV_OVERRIDE
        {
            ଅକ୍ଷର braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            ଫେରନ୍ତୁ makePtr<FormattedImpl>(String(),
                mtx.rows > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.rows == 1 || !multiline, ମିଥ୍ୟା, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    ଶ୍ରେଣୀ CFormatter CV_FINAL : ସର୍ବସାଧାରଣ FormatterBase
    {
    ସର୍ବସାଧାରଣ:

        Ptr<Formatted> format(ଅଚଳ Mat& mtx) ଅଚଳ CV_OVERRIDE
        {
            ଅକ୍ଷର braces[5] = {'\0', '\0', ',', '\0', '\0'};
            ଫେରନ୍ତୁ makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.rows == 1 || !multiline, ମିଥ୍ୟା, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        ବାଛ(fmt)
        {
            ସ୍ଥିତି FMT_DEFAULT:
                ଫେରନ୍ତୁ makePtr<DefaultFormatter>();
            ସ୍ଥିତି FMT_MATLAB:
                ଫେରନ୍ତୁ makePtr<MatlabFormatter>();
            ସ୍ଥିତି FMT_CSV:
                ଫେରନ୍ତୁ makePtr<CSVFormatter>();
            ସ୍ଥିତି FMT_PYTHON:
                ଫେରନ୍ତୁ makePtr<PythonFormatter>();
            ସ୍ଥିତି FMT_NUMPY:
                ଫେରନ୍ତୁ makePtr<NumpyFormatter>();
            ସ୍ଥିତି FMT_C:
                ଫେରନ୍ତୁ makePtr<CFormatter>();
        }
        ଫେରନ୍ତୁ makePtr<DefaultFormatter>();
    }

    template<typename _Tp> ସଂରଚନା Fmt
    {
        ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା temp_type;
        ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* fmt() { ଫେରନ୍ତୁ "%d"; }
    };

    template<> ସଂରଚନା Fmt<uint32_t>
    {
        ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned temp_type;
        ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* fmt() { ଫେରନ୍ତୁ "%u"; }
    };

    template<> ସଂରଚନା Fmt<int64_t>
    {
        ପ୍ରକାରବ୍ୟାଖ୍ୟା long long temp_type;
        ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* fmt() { ଫେରନ୍ତୁ "%lld"; }
    };

    template<> ସଂରଚନା Fmt<uint64_t>
    {
        ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned long long temp_type;
        ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* fmt() { ଫେରନ୍ତୁ "%llu"; }
    };

    template<> ସଂରଚନା Fmt<ଦଶମିକ>
    {
        ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦଶମିକ temp_type;
        ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* fmt() { ଫେରନ୍ତୁ "%.5g"; }
    };

    template<> ସଂରଚନା Fmt<ଦ୍ୱିଗୁଣ>
    {
        ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦ୍ୱିଗୁଣ temp_type;
        ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* fmt() { ଫେରନ୍ତୁ "%.5g"; }
    };

    template<> ସଂରଚନା Fmt<hfloat>
    {
        ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦଶମିକ temp_type;
        ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* fmt() { ଫେରନ୍ତୁ "%.5g"; }
    };

    template<> ସଂରଚନା Fmt<bfloat>
    {
        ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦଶମିକ temp_type;
        ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* fmt() { ଫେରନ୍ତୁ "%.4g"; }
    };

    template<> ସଂରଚନା Fmt<fp8_t>
    {
        ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦଶମିକ temp_type;
        ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* fmt() { ଫେରନ୍ତୁ "%.4g"; }
    };

    template<> ସଂରଚନା Fmt<fp8a_t>
    {
        ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦଶମିକ temp_type;
        ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* fmt() { ଫେରନ୍ତୁ "%.4g"; }
    };

    template <typename _Tp>
    ସ୍ଥିର ଶୂନ୍ୟ pprintRow(std::ostream& strm, ଅଚଳ _Tp* ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, size_t ofs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା edge)
    {
        ଅକ୍ଷର buf[128];
        ଅଚଳ ଅକ୍ଷର* fmt = Fmt<_Tp>::fmt();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        ଯଦି (edge == 0)
            edge = ndump;
        ପାଇଁ (i = 0; i < ndump; i++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            ଯଦି (i > 0)
                strm << ", ";
            ଯଦି (j >= 0) {
                snprintf(buf, ଆକାର(buf), fmt, (typename Fmt<_Tp>::temp_type)ptr[ofs + j]);
                strm << buf;
            } ଅନ୍ୟଥା
                strm << "... ";
        }
    }

    ସ୍ଥିର ଶୂନ୍ୟ pprintSlice(std::ostream& strm, ଅଚଳ Mat& tensor,
                            ଅଚଳ size_t* step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା d,
                            size_t ofs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା edge)
    {
        MatShape shape = tensor.shape();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndims = shape.dims;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = d >= ndims ? 1 : shape[d];
        ଯଦି (d >= ndims - 1) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା typ = tensor.depth();
            ଶୂନ୍ୟ* data = tensor.data;
            CV_Assert(data);
            n *= tensor.channels();
            ଯଦି (typ == CV_8U)
                pprintRow(strm, (ଅଚଳ uint8_t*)data, n, ofs, edge);
            ଅନ୍ୟଥା ଯଦି (typ == CV_8S)
                pprintRow(strm, (ଅଚଳ int8_t*)data, n, ofs, edge);
            ଅନ୍ୟଥା ଯଦି (typ == CV_16U)
                pprintRow(strm, (ଅଚଳ uint16_t*)data, n, ofs, edge);
            ଅନ୍ୟଥା ଯଦି (typ == CV_16S)
                pprintRow(strm, (ଅଚଳ int16_t*)data, n, ofs, edge);
            ଅନ୍ୟଥା ଯଦି (typ == CV_32U)
                pprintRow(strm, (ଅଚଳ unsigned*)data, n, ofs, edge);
            ଅନ୍ୟଥା ଯଦି (typ == CV_32S)
                pprintRow(strm, (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)data, n, ofs, edge);
            ଅନ୍ୟଥା ଯଦି (typ == CV_64U)
                pprintRow(strm, (ଅଚଳ uint64_t*)data, n, ofs, edge);
            ଅନ୍ୟଥା ଯଦି (typ == CV_64S)
                pprintRow(strm, (ଅଚଳ int64_t*)data, n, ofs, edge);
            ଅନ୍ୟଥା ଯଦି (typ == CV_32F)
                pprintRow(strm, (ଅଚଳ ଦଶମିକ*)data, n, ofs, edge);
            ଅନ୍ୟଥା ଯଦି (typ == CV_64F)
                pprintRow(strm, (ଅଚଳ ଦ୍ୱିଗୁଣ*)data, n, ofs, edge);
            ଅନ୍ୟଥା ଯଦି (typ == CV_16F)
                pprintRow(strm, (ଅଚଳ hfloat*)data, n, ofs, edge);
            ଅନ୍ୟଥା ଯଦି (typ == CV_16BF)
                pprintRow(strm, (ଅଚଳ bfloat*)data, n, ofs, edge);
            ଅନ୍ୟଥା ଯଦି (typ == CV_Bool)
                pprintRow(strm, (ଅଚଳ ତାର୍କିକ*)data, n, ofs, edge);
            ଅନ୍ୟଥା ଯଦି (typ == CV_8F_E4M3FN)
                pprintRow(strm, (ଅଚଳ fp8_t*)data, n, ofs, edge);
            ଅନ୍ୟଥା ଯଦି (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (ଅଚଳ fp8a_t*)data, n, ofs, edge);
            ଅନ୍ୟଥା {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } ଅନ୍ୟଥା {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            ତାର୍କିକ dots = ମିଥ୍ୟା;
            ପାଇଁ (i = 0; i < ndump; i++) {
                ଯଦି (i > 0 && !dots) {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nempty_ଧାଡ଼ିଗୁଡ଼ିକ = ndims - 2 - d;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < nempty_ଧାଡ଼ିଗୁଡ଼ିକ; k++)
                        strm << "\n";
                }
                ଯଦି (i > 0)
                    strm << "\n";
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                ଯଦି (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                ଅନ୍ୟଥା
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, ନିବେଶ_Array array,
                         ପୂର୍ଣ୍ଣସଂଖ୍ୟା /*indent*/, ପୂର୍ଣ୍ଣସଂଖ୍ୟା edge_,
                         ପୂର୍ଣ୍ଣସଂଖ୍ୟା wholeTensorThreshold,
                         ଅକ୍ଷର parens)
    {
        ଅକ୍ଷର oparen = parens;
        ଅକ୍ଷର cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        ଯଦି (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat temp;
            tensor.copyTo(temp);
            tensor = temp;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.total();

        ଯଦି (parens)
            strm << oparen;
        ଯଦି (sz_all == 0) {
            ଯଦି (!parens)
                strm << "<empty>";
        } ଅନ୍ୟଥା {
            ଯଦି (sz_all <= (size_t)wholeTensorThreshold)
                edge = 0;

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndims = shape.dims;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = tensor.channels();
            size_t step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        ଯଦି (parens)
            strm << cparen;
        ଫେରନ୍ତୁ strm;
    }

} // cv
