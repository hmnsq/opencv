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

#അന്തര്ഭാവ "precomp.hpp"

നാമാകാശ cv
{
    വർഗ്ഗം FormattedImpl CV_FINAL : പൊതു Formatted
    {
        എണ്ണൽ { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_മൂല്യം, STATE_FINISHED,
               STATE_വരി_SEPARATOR, STATE_CN_SEPARATOR, STATE_മൂല്യം_SEPARATOR };
        എണ്ണൽ {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        അക്ഷരം floatFormat[8];
        അക്ഷരം buf[32];   // enough for double with precision up to 20

        Mat mtx;
        പൂർണ്ണസംഖ്യ mcn; // == mtx.channels()
        തർക്കം single_വരി;
        തർക്കം alignOrder;    // true when cn first order

        പൂർണ്ണസംഖ്യ state;
        പൂർണ്ണസംഖ്യ row;
        പൂർണ്ണസംഖ്യ col;
        പൂർണ്ണസംഖ്യ cn;

        String prologue;
        String epilogue;
        അക്ഷരം braces[5];

        ശൂന്യം (FormattedImpl::*മൂല്യം_To_Str)();
        ശൂന്യം മൂല്യം_To_Str_Bool() { snprintf(buf, വലുപ്പം(buf), "%d", (പൂർണ്ണസംഖ്യ)mtx.ptr<uchar>(row, col)[cn] != 0); }
        ശൂന്യം മൂല്യം_To_Str8u()  { snprintf(buf, വലുപ്പം(buf), "%3d", (പൂർണ്ണസംഖ്യ)mtx.ptr<uchar>(row, col)[cn]); }
        ശൂന്യം മൂല്യം_To_Str8s()  { snprintf(buf, വലുപ്പം(buf), "%3d", (പൂർണ്ണസംഖ്യ)mtx.ptr<schar>(row, col)[cn]); }
        ശൂന്യം മൂല്യം_To_Str16u() { snprintf(buf, വലുപ്പം(buf), "%d", (പൂർണ്ണസംഖ്യ)mtx.ptr<ushort>(row, col)[cn]); }
        ശൂന്യം മൂല്യം_To_Str16s() { snprintf(buf, വലുപ്പം(buf), "%d", (പൂർണ്ണസംഖ്യ)mtx.ptr<short>(row, col)[cn]); }
        ശൂന്യം മൂല്യം_To_Str32u() { snprintf(buf, വലുപ്പം(buf), "%u", mtx.ptr<unsigned>(row, col)[cn]); }
        ശൂന്യം മൂല്യം_To_Str32s() { snprintf(buf, വലുപ്പം(buf), "%d", mtx.ptr<പൂർണ്ണസംഖ്യ>(row, col)[cn]); }
        ശൂന്യം മൂല്യം_To_Str32f() { snprintf(buf, വലുപ്പം(buf), floatFormat, mtx.ptr<ദശാംശം>(row, col)[cn]); }
        ശൂന്യം മൂല്യം_To_Str64f() { snprintf(buf, വലുപ്പം(buf), floatFormat, mtx.ptr<ഇരട്ട>(row, col)[cn]); }
        ശൂന്യം മൂല്യം_To_Str64u() { snprintf(buf, വലുപ്പം(buf), "%llu", (unsigned long long)mtx.ptr<uint64_t>(row, col)[cn]); }
        ശൂന്യം മൂല്യം_To_Str64s() { snprintf(buf, വലുപ്പം(buf), "%lld", (long long)mtx.ptr<int64_t>(row, col)[cn]); }
        ശൂന്യം മൂല്യം_To_Str16f() { snprintf(buf, വലുപ്പം(buf), floatFormat, (ദശാംശം)mtx.ptr<hfloat>(row, col)[cn]); }
        ശൂന്യം മൂല്യം_To_Str16bf() { snprintf(buf, വലുപ്പം(buf), floatFormat, (ദശാംശം)mtx.ptr<bfloat>(row, col)[cn]); }
        ശൂന്യം മൂല്യം_To_Str8f()   { snprintf(buf, വലുപ്പം(buf), floatFormat, (ദശാംശം)mtx.ptr<fp8_t>(row, col)[cn]); }
        ശൂന്യം മൂല്യം_To_Str8fnuz() { snprintf(buf, വലുപ്പം(buf), floatFormat, (ദശാംശം)mtx.ptr<fp8a_t>(row, col)[cn]); }
        ശൂന്യം മൂല്യം_To_Str_Other() { buf[0] = 0; }

    പൊതു:

        FormattedImpl(String pl, String el, Mat m, അക്ഷരം br[5], തർക്കം s_വരി, തർക്കം aOrder, പൂർണ്ണസംഖ്യ precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            memcpy(braces, br, 5);
            state = STATE_PROLOGUE;
            single_വരി = s_വരി;
            alignOrder = aOrder;
            row = col = cn =0;

            എങ്കിൽ (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            അല്ലെങ്കിൽ
            {
                cv_snprintf(floatFormat, വലുപ്പം(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            തിരഞ്ഞെടുപ്പ്(mtx.depth())
            {
                അവസ്ഥ CV_8U:  മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str8u; നിർത്തുക;
                അവസ്ഥ CV_8S:  മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str8s; നിർത്തുക;
                അവസ്ഥ CV_Bool: മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str_Bool; നിർത്തുക;
                അവസ്ഥ CV_16U: മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str16u; നിർത്തുക;
                അവസ്ഥ CV_16S: മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str16s; നിർത്തുക;
                അവസ്ഥ CV_32U: മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str32u; നിർത്തുക;
                അവസ്ഥ CV_32S: മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str32s; നിർത്തുക;
                അവസ്ഥ CV_32F: മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str32f; നിർത്തുക;
                അവസ്ഥ CV_64F: മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str64f; നിർത്തുക;
                അവസ്ഥ CV_64U: മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str64u; നിർത്തുക;
                അവസ്ഥ CV_64S: മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str64s; നിർത്തുക;
                അവസ്ഥ CV_16F: മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str16f; നിർത്തുക;
                അവസ്ഥ CV_16BF: മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str16bf; നിർത്തുക;
                അവസ്ഥ CV_8F_E4M3FN:   മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str8f; നിർത്തുക;
                അവസ്ഥ CV_8F_E4M3FNUZ: മൂല്യം_To_Str = &FormattedImpl::മൂല്യം_To_Str8fnuz; നിർത്തുക;
                സ്ഥിരസ്ഥിതി:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.depth()));
            }
        }

        ശൂന്യം reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        സ്ഥിരം അക്ഷരം* next() CV_OVERRIDE
        {
            തിരഞ്ഞെടുപ്പ്(state)
            {
                അവസ്ഥ STATE_PROLOGUE:
                    row = 0;
                    എങ്കിൽ (mtx.empty())
                        state = STATE_EPILOGUE;
                    അല്ലെങ്കിൽ എങ്കിൽ (alignOrder)
                        state = STATE_INTERLUDE;
                    അല്ലെങ്കിൽ
                        state = STATE_ROW_OPEN;
                    തിരികെ prologue.c_str();
                അവസ്ഥ STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    എങ്കിൽ (row >= mtx.rows)
                    {
                        എങ്കിൽ (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            തിരികെ buf;
                        }
                        അല്ലെങ്കിൽ
                            row = 0;
                        snprintf(buf, വലുപ്പം(buf), "\n(:, :, %d) = \n", cn+1);
                        തിരികെ buf;
                    }
                    snprintf(buf, വലുപ്പം(buf), "(:, :, %d) = \n", cn+1);
                    തിരികെ buf;
                അവസ്ഥ STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    തിരികെ epilogue.c_str();
                അവസ്ഥ STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        എങ്കിൽ (row > 0)
                            ആകുമ്പോൾ(pos < prologue.size() && pos < വലുപ്പം(buf) - 2)
                                buf[pos++] = ' ';
                        എങ്കിൽ (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        എങ്കിൽ(!pos)
                            തിരികെ next();
                        buf[pos] = 0;
                    }
                    തിരികെ buf;
                അവസ്ഥ STATE_ROW_CLOSE:
                    state = STATE_വരി_SEPARATOR;
                    ++row;
                    എങ്കിൽ (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        തിരികെ buf;
                    }
                    അല്ലെങ്കിൽ എങ്കിൽ(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        തിരികെ buf;
                    }
                    തിരികെ next();
                അവസ്ഥ STATE_CN_OPEN:
                    state = STATE_മൂല്യം;
                    എങ്കിൽ (!alignOrder)
                        cn = 0;
                    എങ്കിൽ (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        തിരികെ buf;
                    }
                    തിരികെ next();
                അവസ്ഥ STATE_CN_CLOSE:
                    ++col;
                    എങ്കിൽ (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    അല്ലെങ്കിൽ
                        state = STATE_CN_SEPARATOR;
                    എങ്കിൽ (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        തിരികെ buf;
                    }
                    തിരികെ next();
                അവസ്ഥ STATE_മൂല്യം:
                    (ഇത്->*മൂല്യം_To_Str)();
                    state = STATE_CN_CLOSE;
                    എങ്കിൽ (alignOrder)
                        തിരികെ buf;
                    എങ്കിൽ (++cn < mcn)
                        state = STATE_മൂല്യം_SEPARATOR;
                    തിരികെ buf;
                അവസ്ഥ STATE_FINISHED:
                    തിരികെ 0;
                അവസ്ഥ STATE_വരി_SEPARATOR:
                    എങ്കിൽ (row >= mtx.rows)
                    {
                        എങ്കിൽ (alignOrder)
                            state = STATE_INTERLUDE;
                        അല്ലെങ്കിൽ
                            state = STATE_EPILOGUE;
                        തിരികെ next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = single_വരി ? ' ' : '\n';
                    buf[1] = 0;
                    തിരികെ buf;
                അവസ്ഥ STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    തിരികെ buf;
                അവസ്ഥ STATE_മൂല്യം_SEPARATOR:
                    state = STATE_മൂല്യം;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    തിരികെ buf;
            }
            തിരികെ 0;
        }
    };

    വർഗ്ഗം FormatterBase : പൊതു Formatter
    {
    പൊതു:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(സത്യം) {}

        ശൂന്യം set16fPrecision(പൂർണ്ണസംഖ്യ p) CV_OVERRIDE
        {
            prec16f = p;
        }

        ശൂന്യം set32fPrecision(പൂർണ്ണസംഖ്യ p) CV_OVERRIDE
        {
            prec32f = p;
        }

        ശൂന്യം set64fPrecision(പൂർണ്ണസംഖ്യ p) CV_OVERRIDE
        {
            prec64f = p;
        }

        ശൂന്യം setMultiline(തർക്കം ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    സംരക്ഷിത:
        പൂർണ്ണസംഖ്യ prec16f;
        പൂർണ്ണസംഖ്യ prec32f;
        പൂർണ്ണസംഖ്യ prec64f;
        പൂർണ്ണസംഖ്യ multiline;
    };

    വർഗ്ഗം DefaultFormatter CV_FINAL : പൊതു FormatterBase
    {
    പൊതു:

        Ptr<Formatted> format(സ്ഥിരം Mat& mtx) സ്ഥിരം CV_OVERRIDE
        {
            അക്ഷരം braces[5] = {'\0', '\0', ';', '\0', '\0'};
            തിരികെ makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, അസത്യം, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    വർഗ്ഗം MatlabFormatter CV_FINAL : പൊതു FormatterBase
    {
    പൊതു:

        Ptr<Formatted> format(സ്ഥിരം Mat& mtx) സ്ഥിരം CV_OVERRIDE
        {
            അക്ഷരം braces[5] = {'\0', '\0', ';', '\0', '\0'};
            തിരികെ makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, സത്യം, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    വർഗ്ഗം PythonFormatter CV_FINAL : പൊതു FormatterBase
    {
    പൊതു:

        Ptr<Formatted> format(സ്ഥിരം Mat& mtx) സ്ഥിരം CV_OVERRIDE
        {
            അക്ഷരം braces[5] = {'[', ']', ',', '[', ']'};
            എങ്കിൽ (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            തിരികെ makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, അസത്യം, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    വർഗ്ഗം NumpyFormatter CV_FINAL : പൊതു FormatterBase
    {
    പൊതു:

        Ptr<Formatted> format(സ്ഥിരം Mat& mtx) സ്ഥിരം CV_OVERRIDE
        {
            സ്ഥിതം സ്ഥിരം അക്ഷരം* numpyTypes[CV_DEPTH_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            അക്ഷരം braces[5] = {'[', ']', ',', '[', ']'};
            എങ്കിൽ (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            തിരികെ makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.depth()]), mtx, &*braces,
                mtx.rows == 1 || !multiline, അസത്യം, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    വർഗ്ഗം CSVFormatter CV_FINAL : പൊതു FormatterBase
    {
    പൊതു:

        Ptr<Formatted> format(സ്ഥിരം Mat& mtx) സ്ഥിരം CV_OVERRIDE
        {
            അക്ഷരം braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            തിരികെ makePtr<FormattedImpl>(String(),
                mtx.rows > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.rows == 1 || !multiline, അസത്യം, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    വർഗ്ഗം CFormatter CV_FINAL : പൊതു FormatterBase
    {
    പൊതു:

        Ptr<Formatted> format(സ്ഥിരം Mat& mtx) സ്ഥിരം CV_OVERRIDE
        {
            അക്ഷരം braces[5] = {'\0', '\0', ',', '\0', '\0'};
            തിരികെ makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.rows == 1 || !multiline, അസത്യം, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        തിരഞ്ഞെടുപ്പ്(fmt)
        {
            അവസ്ഥ FMT_DEFAULT:
                തിരികെ makePtr<DefaultFormatter>();
            അവസ്ഥ FMT_MATLAB:
                തിരികെ makePtr<MatlabFormatter>();
            അവസ്ഥ FMT_CSV:
                തിരികെ makePtr<CSVFormatter>();
            അവസ്ഥ FMT_PYTHON:
                തിരികെ makePtr<PythonFormatter>();
            അവസ്ഥ FMT_NUMPY:
                തിരികെ makePtr<NumpyFormatter>();
            അവസ്ഥ FMT_C:
                തിരികെ makePtr<CFormatter>();
        }
        തിരികെ makePtr<DefaultFormatter>();
    }

    template<typename _Tp> ഘടന Fmt
    {
        തരനിർവചനം പൂർണ്ണസംഖ്യ temp_type;
        സ്ഥിതം സ്ഥിരം അക്ഷരം* fmt() { തിരികെ "%d"; }
    };

    template<> ഘടന Fmt<uint32_t>
    {
        തരനിർവചനം unsigned temp_type;
        സ്ഥിതം സ്ഥിരം അക്ഷരം* fmt() { തിരികെ "%u"; }
    };

    template<> ഘടന Fmt<int64_t>
    {
        തരനിർവചനം long long temp_type;
        സ്ഥിതം സ്ഥിരം അക്ഷരം* fmt() { തിരികെ "%lld"; }
    };

    template<> ഘടന Fmt<uint64_t>
    {
        തരനിർവചനം unsigned long long temp_type;
        സ്ഥിതം സ്ഥിരം അക്ഷരം* fmt() { തിരികെ "%llu"; }
    };

    template<> ഘടന Fmt<ദശാംശം>
    {
        തരനിർവചനം ദശാംശം temp_type;
        സ്ഥിതം സ്ഥിരം അക്ഷരം* fmt() { തിരികെ "%.5g"; }
    };

    template<> ഘടന Fmt<ഇരട്ട>
    {
        തരനിർവചനം ഇരട്ട temp_type;
        സ്ഥിതം സ്ഥിരം അക്ഷരം* fmt() { തിരികെ "%.5g"; }
    };

    template<> ഘടന Fmt<hfloat>
    {
        തരനിർവചനം ദശാംശം temp_type;
        സ്ഥിതം സ്ഥിരം അക്ഷരം* fmt() { തിരികെ "%.5g"; }
    };

    template<> ഘടന Fmt<bfloat>
    {
        തരനിർവചനം ദശാംശം temp_type;
        സ്ഥിതം സ്ഥിരം അക്ഷരം* fmt() { തിരികെ "%.4g"; }
    };

    template<> ഘടന Fmt<fp8_t>
    {
        തരനിർവചനം ദശാംശം temp_type;
        സ്ഥിതം സ്ഥിരം അക്ഷരം* fmt() { തിരികെ "%.4g"; }
    };

    template<> ഘടന Fmt<fp8a_t>
    {
        തരനിർവചനം ദശാംശം temp_type;
        സ്ഥിതം സ്ഥിരം അക്ഷരം* fmt() { തിരികെ "%.4g"; }
    };

    template <typename _Tp>
    സ്ഥിതം ശൂന്യം pprintRow(std::ostream& strm, സ്ഥിരം _Tp* ptr, പൂർണ്ണസംഖ്യ n, size_t ofs, പൂർണ്ണസംഖ്യ edge)
    {
        അക്ഷരം buf[128];
        സ്ഥിരം അക്ഷരം* fmt = Fmt<_Tp>::fmt();
        പൂർണ്ണസംഖ്യ i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        എങ്കിൽ (edge == 0)
            edge = ndump;
        വേണ്ടി (i = 0; i < ndump; i++) {
            പൂർണ്ണസംഖ്യ j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            എങ്കിൽ (i > 0)
                strm << ", ";
            എങ്കിൽ (j >= 0) {
                snprintf(buf, വലുപ്പം(buf), fmt, (typename Fmt<_Tp>::temp_type)ptr[ofs + j]);
                strm << buf;
            } അല്ലെങ്കിൽ
                strm << "... ";
        }
    }

    സ്ഥിതം ശൂന്യം pprintSlice(std::ostream& strm, സ്ഥിരം Mat& tensor,
                            സ്ഥിരം size_t* step, പൂർണ്ണസംഖ്യ d,
                            size_t ofs, പൂർണ്ണസംഖ്യ edge)
    {
        MatShape shape = tensor.shape();
        പൂർണ്ണസംഖ്യ ndims = shape.dims;
        പൂർണ്ണസംഖ്യ n = d >= ndims ? 1 : shape[d];
        എങ്കിൽ (d >= ndims - 1) {
            പൂർണ്ണസംഖ്യ typ = tensor.depth();
            ശൂന്യം* data = tensor.data;
            CV_Assert(data);
            n *= tensor.channels();
            എങ്കിൽ (typ == CV_8U)
                pprintRow(strm, (സ്ഥിരം uint8_t*)data, n, ofs, edge);
            അല്ലെങ്കിൽ എങ്കിൽ (typ == CV_8S)
                pprintRow(strm, (സ്ഥിരം int8_t*)data, n, ofs, edge);
            അല്ലെങ്കിൽ എങ്കിൽ (typ == CV_16U)
                pprintRow(strm, (സ്ഥിരം uint16_t*)data, n, ofs, edge);
            അല്ലെങ്കിൽ എങ്കിൽ (typ == CV_16S)
                pprintRow(strm, (സ്ഥിരം int16_t*)data, n, ofs, edge);
            അല്ലെങ്കിൽ എങ്കിൽ (typ == CV_32U)
                pprintRow(strm, (സ്ഥിരം unsigned*)data, n, ofs, edge);
            അല്ലെങ്കിൽ എങ്കിൽ (typ == CV_32S)
                pprintRow(strm, (സ്ഥിരം പൂർണ്ണസംഖ്യ*)data, n, ofs, edge);
            അല്ലെങ്കിൽ എങ്കിൽ (typ == CV_64U)
                pprintRow(strm, (സ്ഥിരം uint64_t*)data, n, ofs, edge);
            അല്ലെങ്കിൽ എങ്കിൽ (typ == CV_64S)
                pprintRow(strm, (സ്ഥിരം int64_t*)data, n, ofs, edge);
            അല്ലെങ്കിൽ എങ്കിൽ (typ == CV_32F)
                pprintRow(strm, (സ്ഥിരം ദശാംശം*)data, n, ofs, edge);
            അല്ലെങ്കിൽ എങ്കിൽ (typ == CV_64F)
                pprintRow(strm, (സ്ഥിരം ഇരട്ട*)data, n, ofs, edge);
            അല്ലെങ്കിൽ എങ്കിൽ (typ == CV_16F)
                pprintRow(strm, (സ്ഥിരം hfloat*)data, n, ofs, edge);
            അല്ലെങ്കിൽ എങ്കിൽ (typ == CV_16BF)
                pprintRow(strm, (സ്ഥിരം bfloat*)data, n, ofs, edge);
            അല്ലെങ്കിൽ എങ്കിൽ (typ == CV_Bool)
                pprintRow(strm, (സ്ഥിരം തർക്കം*)data, n, ofs, edge);
            അല്ലെങ്കിൽ എങ്കിൽ (typ == CV_8F_E4M3FN)
                pprintRow(strm, (സ്ഥിരം fp8_t*)data, n, ofs, edge);
            അല്ലെങ്കിൽ എങ്കിൽ (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (സ്ഥിരം fp8a_t*)data, n, ofs, edge);
            അല്ലെങ്കിൽ {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } അല്ലെങ്കിൽ {
            പൂർണ്ണസംഖ്യ i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            തർക്കം dots = അസത്യം;
            വേണ്ടി (i = 0; i < ndump; i++) {
                എങ്കിൽ (i > 0 && !dots) {
                    പൂർണ്ണസംഖ്യ nempty_വരികൾ = ndims - 2 - d;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < nempty_വരികൾ; k++)
                        strm << "\n";
                }
                എങ്കിൽ (i > 0)
                    strm << "\n";
                പൂർണ്ണസംഖ്യ j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                എങ്കിൽ (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                അല്ലെങ്കിൽ
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, നിവേശം_Array array,
                         പൂർണ്ണസംഖ്യ /*indent*/, പൂർണ്ണസംഖ്യ edge_,
                         പൂർണ്ണസംഖ്യ wholeTensorThreshold,
                         അക്ഷരം parens)
    {
        അക്ഷരം oparen = parens;
        അക്ഷരം cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        പൂർണ്ണസംഖ്യ edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        എങ്കിൽ (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat temp;
            tensor.copyTo(temp);
            tensor = temp;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.total();

        എങ്കിൽ (parens)
            strm << oparen;
        എങ്കിൽ (sz_all == 0) {
            എങ്കിൽ (!parens)
                strm << "<empty>";
        } അല്ലെങ്കിൽ {
            എങ്കിൽ (sz_all <= (size_t)wholeTensorThreshold)
                edge = 0;

            പൂർണ്ണസംഖ്യ ndims = shape.dims;
            പൂർണ്ണസംഖ്യ cn = tensor.channels();
            size_t step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        എങ്കിൽ (parens)
            strm << cparen;
        തിരികെ strm;
    }

} // cv
