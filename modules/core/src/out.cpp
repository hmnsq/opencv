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

#अन्तर्भाव "precomp.hpp"

नामाकाश cv
{
    वग्ग FormattedImpl CV_FINAL : सार्वजनिक Formatted
    {
        गणना { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_मानम्, STATE_FINISHED,
               STATE_पङ्क्तिः_SEPARATOR, STATE_CN_SEPARATOR, STATE_मानम्_SEPARATOR };
        गणना {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        वर्ण floatFormat[8];
        वर्ण buf[32];   // enough for double with precision up to 20

        Mat mtx;
        पूर्णाङ्क mcn; // == mtx.channels()
        तार्किक single_पङ्क्तिः;
        तार्किक alignOrder;    // true when cn first order

        पूर्णाङ्क state;
        पूर्णाङ्क row;
        पूर्णाङ्क col;
        पूर्णाङ्क cn;

        String prologue;
        String epilogue;
        वर्ण braces[5];

        शून्य (FormattedImpl::*मानम्_To_Str)();
        शून्य मानम्_To_Str_Bool() { snprintf(buf, परिमाण(buf), "%d", (पूर्णाङ्क)mtx.ptr<uchar>(row, col)[cn] != 0); }
        शून्य मानम्_To_Str8u()  { snprintf(buf, परिमाण(buf), "%3d", (पूर्णाङ्क)mtx.ptr<uchar>(row, col)[cn]); }
        शून्य मानम्_To_Str8s()  { snprintf(buf, परिमाण(buf), "%3d", (पूर्णाङ्क)mtx.ptr<schar>(row, col)[cn]); }
        शून्य मानम्_To_Str16u() { snprintf(buf, परिमाण(buf), "%d", (पूर्णाङ्क)mtx.ptr<ushort>(row, col)[cn]); }
        शून्य मानम्_To_Str16s() { snprintf(buf, परिमाण(buf), "%d", (पूर्णाङ्क)mtx.ptr<short>(row, col)[cn]); }
        शून्य मानम्_To_Str32u() { snprintf(buf, परिमाण(buf), "%u", mtx.ptr<unsigned>(row, col)[cn]); }
        शून्य मानम्_To_Str32s() { snprintf(buf, परिमाण(buf), "%d", mtx.ptr<पूर्णाङ्क>(row, col)[cn]); }
        शून्य मानम्_To_Str32f() { snprintf(buf, परिमाण(buf), floatFormat, mtx.ptr<भिन्न>(row, col)[cn]); }
        शून्य मानम्_To_Str64f() { snprintf(buf, परिमाण(buf), floatFormat, mtx.ptr<द्विगुण>(row, col)[cn]); }
        शून्य मानम्_To_Str64u() { snprintf(buf, परिमाण(buf), "%llu", (unsigned long long)mtx.ptr<uint64_t>(row, col)[cn]); }
        शून्य मानम्_To_Str64s() { snprintf(buf, परिमाण(buf), "%lld", (long long)mtx.ptr<int64_t>(row, col)[cn]); }
        शून्य मानम्_To_Str16f() { snprintf(buf, परिमाण(buf), floatFormat, (भिन्न)mtx.ptr<hfloat>(row, col)[cn]); }
        शून्य मानम्_To_Str16bf() { snprintf(buf, परिमाण(buf), floatFormat, (भिन्न)mtx.ptr<bfloat>(row, col)[cn]); }
        शून्य मानम्_To_Str8f()   { snprintf(buf, परिमाण(buf), floatFormat, (भिन्न)mtx.ptr<fp8_t>(row, col)[cn]); }
        शून्य मानम्_To_Str8fnuz() { snprintf(buf, परिमाण(buf), floatFormat, (भिन्न)mtx.ptr<fp8a_t>(row, col)[cn]); }
        शून्य मानम्_To_Str_Other() { buf[0] = 0; }

    सार्वजनिक:

        FormattedImpl(String pl, String el, Mat m, वर्ण br[5], तार्किक s_पङ्क्तिः, तार्किक aOrder, पूर्णाङ्क precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            memcpy(braces, br, 5);
            state = STATE_PROLOGUE;
            single_पङ्क्तिः = s_पङ्क्तिः;
            alignOrder = aOrder;
            row = col = cn =0;

            सचे (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            अञ्ञथा
            {
                cv_snprintf(floatFormat, परिमाण(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            विकल्प(mtx.depth())
            {
                पक्ष CV_8U:  मानम्_To_Str = &FormattedImpl::मानम्_To_Str8u; भङ्ग;
                पक्ष CV_8S:  मानम्_To_Str = &FormattedImpl::मानम्_To_Str8s; भङ्ग;
                पक्ष CV_Bool: मानम्_To_Str = &FormattedImpl::मानम्_To_Str_Bool; भङ्ग;
                पक्ष CV_16U: मानम्_To_Str = &FormattedImpl::मानम्_To_Str16u; भङ्ग;
                पक्ष CV_16S: मानम्_To_Str = &FormattedImpl::मानम्_To_Str16s; भङ्ग;
                पक्ष CV_32U: मानम्_To_Str = &FormattedImpl::मानम्_To_Str32u; भङ्ग;
                पक्ष CV_32S: मानम्_To_Str = &FormattedImpl::मानम्_To_Str32s; भङ्ग;
                पक्ष CV_32F: मानम्_To_Str = &FormattedImpl::मानम्_To_Str32f; भङ्ग;
                पक्ष CV_64F: मानम्_To_Str = &FormattedImpl::मानम्_To_Str64f; भङ्ग;
                पक्ष CV_64U: मानम्_To_Str = &FormattedImpl::मानम्_To_Str64u; भङ्ग;
                पक्ष CV_64S: मानम्_To_Str = &FormattedImpl::मानम्_To_Str64s; भङ्ग;
                पक्ष CV_16F: मानम्_To_Str = &FormattedImpl::मानम्_To_Str16f; भङ्ग;
                पक्ष CV_16BF: मानम्_To_Str = &FormattedImpl::मानम्_To_Str16bf; भङ्ग;
                पक्ष CV_8F_E4M3FN:   मानम्_To_Str = &FormattedImpl::मानम्_To_Str8f; भङ्ग;
                पक्ष CV_8F_E4M3FNUZ: मानम्_To_Str = &FormattedImpl::मानम्_To_Str8fnuz; भङ्ग;
                स्वभाव:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.depth()));
            }
        }

        शून्य reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        ध्रुव वर्ण* next() CV_OVERRIDE
        {
            विकल्प(state)
            {
                पक्ष STATE_PROLOGUE:
                    row = 0;
                    सचे (mtx.empty())
                        state = STATE_EPILOGUE;
                    अञ्ञथा सचे (alignOrder)
                        state = STATE_INTERLUDE;
                    अञ्ञथा
                        state = STATE_ROW_OPEN;
                    पटिनिवत्त prologue.c_str();
                पक्ष STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    सचे (row >= mtx.rows)
                    {
                        सचे (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            पटिनिवत्त buf;
                        }
                        अञ्ञथा
                            row = 0;
                        snprintf(buf, परिमाण(buf), "\n(:, :, %d) = \n", cn+1);
                        पटिनिवत्त buf;
                    }
                    snprintf(buf, परिमाण(buf), "(:, :, %d) = \n", cn+1);
                    पटिनिवत्त buf;
                पक्ष STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    पटिनिवत्त epilogue.c_str();
                पक्ष STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        सचे (row > 0)
                            याव(pos < prologue.size() && pos < परिमाण(buf) - 2)
                                buf[pos++] = ' ';
                        सचे (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        सचे(!pos)
                            पटिनिवत्त next();
                        buf[pos] = 0;
                    }
                    पटिनिवत्त buf;
                पक्ष STATE_ROW_CLOSE:
                    state = STATE_पङ्क्तिः_SEPARATOR;
                    ++row;
                    सचे (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        पटिनिवत्त buf;
                    }
                    अञ्ञथा सचे(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        पटिनिवत्त buf;
                    }
                    पटिनिवत्त next();
                पक्ष STATE_CN_OPEN:
                    state = STATE_मानम्;
                    सचे (!alignOrder)
                        cn = 0;
                    सचे (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        पटिनिवत्त buf;
                    }
                    पटिनिवत्त next();
                पक्ष STATE_CN_CLOSE:
                    ++col;
                    सचे (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    अञ्ञथा
                        state = STATE_CN_SEPARATOR;
                    सचे (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        पटिनिवत्त buf;
                    }
                    पटिनिवत्त next();
                पक्ष STATE_मानम्:
                    (एतत्->*मानम्_To_Str)();
                    state = STATE_CN_CLOSE;
                    सचे (alignOrder)
                        पटिनिवत्त buf;
                    सचे (++cn < mcn)
                        state = STATE_मानम्_SEPARATOR;
                    पटिनिवत्त buf;
                पक्ष STATE_FINISHED:
                    पटिनिवत्त 0;
                पक्ष STATE_पङ्क्तिः_SEPARATOR:
                    सचे (row >= mtx.rows)
                    {
                        सचे (alignOrder)
                            state = STATE_INTERLUDE;
                        अञ्ञथा
                            state = STATE_EPILOGUE;
                        पटिनिवत्त next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = single_पङ्क्तिः ? ' ' : '\n';
                    buf[1] = 0;
                    पटिनिवत्त buf;
                पक्ष STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    पटिनिवत्त buf;
                पक्ष STATE_मानम्_SEPARATOR:
                    state = STATE_मानम्;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    पटिनिवत्त buf;
            }
            पटिनिवत्त 0;
        }
    };

    वग्ग FormatterBase : सार्वजनिक Formatter
    {
    सार्वजनिक:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(सच्च) {}

        शून्य set16fPrecision(पूर्णाङ्क p) CV_OVERRIDE
        {
            prec16f = p;
        }

        शून्य set32fPrecision(पूर्णाङ्क p) CV_OVERRIDE
        {
            prec32f = p;
        }

        शून्य set64fPrecision(पूर्णाङ्क p) CV_OVERRIDE
        {
            prec64f = p;
        }

        शून्य setMultiline(तार्किक ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    रक्षित:
        पूर्णाङ्क prec16f;
        पूर्णाङ्क prec32f;
        पूर्णाङ्क prec64f;
        पूर्णाङ्क multiline;
    };

    वग्ग DefaultFormatter CV_FINAL : सार्वजनिक FormatterBase
    {
    सार्वजनिक:

        Ptr<Formatted> format(ध्रुव Mat& mtx) ध्रुव CV_OVERRIDE
        {
            वर्ण braces[5] = {'\0', '\0', ';', '\0', '\0'};
            पटिनिवत्त makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, मिच्छा, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    वग्ग MatlabFormatter CV_FINAL : सार्वजनिक FormatterBase
    {
    सार्वजनिक:

        Ptr<Formatted> format(ध्रुव Mat& mtx) ध्रुव CV_OVERRIDE
        {
            वर्ण braces[5] = {'\0', '\0', ';', '\0', '\0'};
            पटिनिवत्त makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, सच्च, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    वग्ग PythonFormatter CV_FINAL : सार्वजनिक FormatterBase
    {
    सार्वजनिक:

        Ptr<Formatted> format(ध्रुव Mat& mtx) ध्रुव CV_OVERRIDE
        {
            वर्ण braces[5] = {'[', ']', ',', '[', ']'};
            सचे (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            पटिनिवत्त makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, मिच्छा, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    वग्ग NumpyFormatter CV_FINAL : सार्वजनिक FormatterBase
    {
    सार्वजनिक:

        Ptr<Formatted> format(ध्रुव Mat& mtx) ध्रुव CV_OVERRIDE
        {
            स्थिर ध्रुव वर्ण* numpyTypes[CV_DEPTH_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            वर्ण braces[5] = {'[', ']', ',', '[', ']'};
            सचे (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            पटिनिवत्त makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.depth()]), mtx, &*braces,
                mtx.rows == 1 || !multiline, मिच्छा, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    वग्ग CSVFormatter CV_FINAL : सार्वजनिक FormatterBase
    {
    सार्वजनिक:

        Ptr<Formatted> format(ध्रुव Mat& mtx) ध्रुव CV_OVERRIDE
        {
            वर्ण braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            पटिनिवत्त makePtr<FormattedImpl>(String(),
                mtx.rows > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.rows == 1 || !multiline, मिच्छा, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    वग्ग CFormatter CV_FINAL : सार्वजनिक FormatterBase
    {
    सार्वजनिक:

        Ptr<Formatted> format(ध्रुव Mat& mtx) ध्रुव CV_OVERRIDE
        {
            वर्ण braces[5] = {'\0', '\0', ',', '\0', '\0'};
            पटिनिवत्त makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.rows == 1 || !multiline, मिच्छा, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        विकल्प(fmt)
        {
            पक्ष FMT_DEFAULT:
                पटिनिवत्त makePtr<DefaultFormatter>();
            पक्ष FMT_MATLAB:
                पटिनिवत्त makePtr<MatlabFormatter>();
            पक्ष FMT_CSV:
                पटिनिवत्त makePtr<CSVFormatter>();
            पक्ष FMT_PYTHON:
                पटिनिवत्त makePtr<PythonFormatter>();
            पक्ष FMT_NUMPY:
                पटिनिवत्त makePtr<NumpyFormatter>();
            पक्ष FMT_C:
                पटिनिवत्त makePtr<CFormatter>();
        }
        पटिनिवत्त makePtr<DefaultFormatter>();
    }

    template<typename _Tp> संरचना Fmt
    {
        प्रकारनिर्देश पूर्णाङ्क temp_type;
        स्थिर ध्रुव वर्ण* fmt() { पटिनिवत्त "%d"; }
    };

    template<> संरचना Fmt<uint32_t>
    {
        प्रकारनिर्देश unsigned temp_type;
        स्थिर ध्रुव वर्ण* fmt() { पटिनिवत्त "%u"; }
    };

    template<> संरचना Fmt<int64_t>
    {
        प्रकारनिर्देश long long temp_type;
        स्थिर ध्रुव वर्ण* fmt() { पटिनिवत्त "%lld"; }
    };

    template<> संरचना Fmt<uint64_t>
    {
        प्रकारनिर्देश unsigned long long temp_type;
        स्थिर ध्रुव वर्ण* fmt() { पटिनिवत्त "%llu"; }
    };

    template<> संरचना Fmt<भिन्न>
    {
        प्रकारनिर्देश भिन्न temp_type;
        स्थिर ध्रुव वर्ण* fmt() { पटिनिवत्त "%.5g"; }
    };

    template<> संरचना Fmt<द्विगुण>
    {
        प्रकारनिर्देश द्विगुण temp_type;
        स्थिर ध्रुव वर्ण* fmt() { पटिनिवत्त "%.5g"; }
    };

    template<> संरचना Fmt<hfloat>
    {
        प्रकारनिर्देश भिन्न temp_type;
        स्थिर ध्रुव वर्ण* fmt() { पटिनिवत्त "%.5g"; }
    };

    template<> संरचना Fmt<bfloat>
    {
        प्रकारनिर्देश भिन्न temp_type;
        स्थिर ध्रुव वर्ण* fmt() { पटिनिवत्त "%.4g"; }
    };

    template<> संरचना Fmt<fp8_t>
    {
        प्रकारनिर्देश भिन्न temp_type;
        स्थिर ध्रुव वर्ण* fmt() { पटिनिवत्त "%.4g"; }
    };

    template<> संरचना Fmt<fp8a_t>
    {
        प्रकारनिर्देश भिन्न temp_type;
        स्थिर ध्रुव वर्ण* fmt() { पटिनिवत्त "%.4g"; }
    };

    template <typename _Tp>
    स्थिर शून्य pprintRow(std::ostream& strm, ध्रुव _Tp* ptr, पूर्णाङ्क n, size_t ofs, पूर्णाङ्क edge)
    {
        वर्ण buf[128];
        ध्रुव वर्ण* fmt = Fmt<_Tp>::fmt();
        पूर्णाङ्क i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        सचे (edge == 0)
            edge = ndump;
        अत्थाय (i = 0; i < ndump; i++) {
            पूर्णाङ्क j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            सचे (i > 0)
                strm << ", ";
            सचे (j >= 0) {
                snprintf(buf, परिमाण(buf), fmt, (typename Fmt<_Tp>::temp_type)ptr[ofs + j]);
                strm << buf;
            } अञ्ञथा
                strm << "... ";
        }
    }

    स्थिर शून्य pprintSlice(std::ostream& strm, ध्रुव Mat& tensor,
                            ध्रुव size_t* step, पूर्णाङ्क d,
                            size_t ofs, पूर्णाङ्क edge)
    {
        MatShape shape = tensor.shape();
        पूर्णाङ्क ndims = shape.dims;
        पूर्णाङ्क n = d >= ndims ? 1 : shape[d];
        सचे (d >= ndims - 1) {
            पूर्णाङ्क typ = tensor.depth();
            शून्य* data = tensor.data;
            CV_Assert(data);
            n *= tensor.channels();
            सचे (typ == CV_8U)
                pprintRow(strm, (ध्रुव uint8_t*)data, n, ofs, edge);
            अञ्ञथा सचे (typ == CV_8S)
                pprintRow(strm, (ध्रुव int8_t*)data, n, ofs, edge);
            अञ्ञथा सचे (typ == CV_16U)
                pprintRow(strm, (ध्रुव uint16_t*)data, n, ofs, edge);
            अञ्ञथा सचे (typ == CV_16S)
                pprintRow(strm, (ध्रुव int16_t*)data, n, ofs, edge);
            अञ्ञथा सचे (typ == CV_32U)
                pprintRow(strm, (ध्रुव unsigned*)data, n, ofs, edge);
            अञ्ञथा सचे (typ == CV_32S)
                pprintRow(strm, (ध्रुव पूर्णाङ्क*)data, n, ofs, edge);
            अञ्ञथा सचे (typ == CV_64U)
                pprintRow(strm, (ध्रुव uint64_t*)data, n, ofs, edge);
            अञ्ञथा सचे (typ == CV_64S)
                pprintRow(strm, (ध्रुव int64_t*)data, n, ofs, edge);
            अञ्ञथा सचे (typ == CV_32F)
                pprintRow(strm, (ध्रुव भिन्न*)data, n, ofs, edge);
            अञ्ञथा सचे (typ == CV_64F)
                pprintRow(strm, (ध्रुव द्विगुण*)data, n, ofs, edge);
            अञ्ञथा सचे (typ == CV_16F)
                pprintRow(strm, (ध्रुव hfloat*)data, n, ofs, edge);
            अञ्ञथा सचे (typ == CV_16BF)
                pprintRow(strm, (ध्रुव bfloat*)data, n, ofs, edge);
            अञ्ञथा सचे (typ == CV_Bool)
                pprintRow(strm, (ध्रुव तार्किक*)data, n, ofs, edge);
            अञ्ञथा सचे (typ == CV_8F_E4M3FN)
                pprintRow(strm, (ध्रुव fp8_t*)data, n, ofs, edge);
            अञ्ञथा सचे (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (ध्रुव fp8a_t*)data, n, ofs, edge);
            अञ्ञथा {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } अञ्ञथा {
            पूर्णाङ्क i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            तार्किक dots = मिच्छा;
            अत्थाय (i = 0; i < ndump; i++) {
                सचे (i > 0 && !dots) {
                    पूर्णाङ्क nempty_पङ्क्तयः = ndims - 2 - d;
                    अत्थाय (पूर्णाङ्क k = 0; k < nempty_पङ्क्तयः; k++)
                        strm << "\n";
                }
                सचे (i > 0)
                    strm << "\n";
                पूर्णाङ्क j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                सचे (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                अञ्ञथा
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, निवेशः_Array array,
                         पूर्णाङ्क /*indent*/, पूर्णाङ्क edge_,
                         पूर्णाङ्क wholeTensorThreshold,
                         वर्ण parens)
    {
        वर्ण oparen = parens;
        वर्ण cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        पूर्णाङ्क edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        सचे (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat temp;
            tensor.copyTo(temp);
            tensor = temp;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.total();

        सचे (parens)
            strm << oparen;
        सचे (sz_all == 0) {
            सचे (!parens)
                strm << "<empty>";
        } अञ्ञथा {
            सचे (sz_all <= (size_t)wholeTensorThreshold)
                edge = 0;

            पूर्णाङ्क ndims = shape.dims;
            पूर्णाङ्क cn = tensor.channels();
            size_t step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            अत्थाय (पूर्णाङ्क i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        सचे (parens)
            strm << cparen;
        पटिनिवत्त strm;
    }

} // cv
