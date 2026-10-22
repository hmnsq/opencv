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
    वर्ग FormattedImpl CV_FINAL : सार्वजनिक Formatted
    {
        गणना { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_मूल्य, STATE_FINISHED,
               STATE_ओळ_SEPARATOR, STATE_CN_SEPARATOR, STATE_मूल्य_SEPARATOR };
        गणना {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        अक्षर floatFormat[8];
        अक्षर buf[32];   // enough for double with precision up to 20

        Mat mtx;
        पूर्णांक mcn; // == mtx.channels()
        तार्किक single_ओळ;
        तार्किक alignOrder;    // true when cn first order

        पूर्णांक state;
        पूर्णांक row;
        पूर्णांक col;
        पूर्णांक cn;

        String prologue;
        String epilogue;
        अक्षर braces[5];

        शून्य (FormattedImpl::*मूल्य_To_Str)();
        शून्य मूल्य_To_Str_Bool() { snprintf(buf, आकार(buf), "%d", (पूर्णांक)mtx.ptr<uchar>(row, col)[cn] != 0); }
        शून्य मूल्य_To_Str8u()  { snprintf(buf, आकार(buf), "%3d", (पूर्णांक)mtx.ptr<uchar>(row, col)[cn]); }
        शून्य मूल्य_To_Str8s()  { snprintf(buf, आकार(buf), "%3d", (पूर्णांक)mtx.ptr<schar>(row, col)[cn]); }
        शून्य मूल्य_To_Str16u() { snprintf(buf, आकार(buf), "%d", (पूर्णांक)mtx.ptr<ushort>(row, col)[cn]); }
        शून्य मूल्य_To_Str16s() { snprintf(buf, आकार(buf), "%d", (पूर्णांक)mtx.ptr<लघु>(row, col)[cn]); }
        शून्य मूल्य_To_Str32u() { snprintf(buf, आकार(buf), "%u", mtx.ptr<अचिन्हित>(row, col)[cn]); }
        शून्य मूल्य_To_Str32s() { snprintf(buf, आकार(buf), "%d", mtx.ptr<पूर्णांक>(row, col)[cn]); }
        शून्य मूल्य_To_Str32f() { snprintf(buf, आकार(buf), floatFormat, mtx.ptr<दशांश>(row, col)[cn]); }
        शून्य मूल्य_To_Str64f() { snprintf(buf, आकार(buf), floatFormat, mtx.ptr<द्विगुण>(row, col)[cn]); }
        शून्य मूल्य_To_Str64u() { snprintf(buf, आकार(buf), "%llu", (अचिन्हित दीर्घ दीर्घ)mtx.ptr<uint64_t>(row, col)[cn]); }
        शून्य मूल्य_To_Str64s() { snprintf(buf, आकार(buf), "%lld", (दीर्घ दीर्घ)mtx.ptr<int64_t>(row, col)[cn]); }
        शून्य मूल्य_To_Str16f() { snprintf(buf, आकार(buf), floatFormat, (दशांश)mtx.ptr<hfloat>(row, col)[cn]); }
        शून्य मूल्य_To_Str16bf() { snprintf(buf, आकार(buf), floatFormat, (दशांश)mtx.ptr<bfloat>(row, col)[cn]); }
        शून्य मूल्य_To_Str8f()   { snprintf(buf, आकार(buf), floatFormat, (दशांश)mtx.ptr<fp8_t>(row, col)[cn]); }
        शून्य मूल्य_To_Str8fnuz() { snprintf(buf, आकार(buf), floatFormat, (दशांश)mtx.ptr<fp8a_t>(row, col)[cn]); }
        शून्य मूल्य_To_Str_Other() { buf[0] = 0; }

    सार्वजनिक:

        FormattedImpl(String pl, String el, Mat m, अक्षर br[5], तार्किक s_ओळ, तार्किक aOrder, पूर्णांक precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            स_नकल(braces, br, 5);
            state = STATE_PROLOGUE;
            single_ओळ = s_ओळ;
            alignOrder = aOrder;
            row = col = cn =0;

            जर (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            अन्यथा
            {
                cv_snprintf(floatFormat, आकार(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            निवड(mtx.depth())
            {
                स्थिती CV_8U:  मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str8u; थांबा;
                स्थिती CV_8S:  मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str8s; थांबा;
                स्थिती CV_Bool: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str_Bool; थांबा;
                स्थिती CV_16U: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str16u; थांबा;
                स्थिती CV_16S: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str16s; थांबा;
                स्थिती CV_32U: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str32u; थांबा;
                स्थिती CV_32S: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str32s; थांबा;
                स्थिती CV_32F: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str32f; थांबा;
                स्थिती CV_64F: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str64f; थांबा;
                स्थिती CV_64U: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str64u; थांबा;
                स्थिती CV_64S: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str64s; थांबा;
                स्थिती CV_16F: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str16f; थांबा;
                स्थिती CV_16BF: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str16bf; थांबा;
                स्थिती CV_8F_E4M3FN:   मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str8f; थांबा;
                स्थिती CV_8F_E4M3FNUZ: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str8fnuz; थांबा;
                पूर्वनिर्धारित:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.depth()));
            }
        }

        शून्य reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        स्थिरांक अक्षर* next() CV_OVERRIDE
        {
            निवड(state)
            {
                स्थिती STATE_PROLOGUE:
                    row = 0;
                    जर (mtx.empty())
                        state = STATE_EPILOGUE;
                    अन्यथा जर (alignOrder)
                        state = STATE_INTERLUDE;
                    अन्यथा
                        state = STATE_ROW_OPEN;
                    परत prologue.c_str();
                स्थिती STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    जर (row >= mtx.rows)
                    {
                        जर (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            परत buf;
                        }
                        अन्यथा
                            row = 0;
                        snprintf(buf, आकार(buf), "\n(:, :, %d) = \n", cn+1);
                        परत buf;
                    }
                    snprintf(buf, आकार(buf), "(:, :, %d) = \n", cn+1);
                    परत buf;
                स्थिती STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    परत epilogue.c_str();
                स्थिती STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        माप_प्रकार pos = 0;
                        जर (row > 0)
                            पर्यंत(pos < prologue.size() && pos < आकार(buf) - 2)
                                buf[pos++] = ' ';
                        जर (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        जर(!pos)
                            परत next();
                        buf[pos] = 0;
                    }
                    परत buf;
                स्थिती STATE_ROW_CLOSE:
                    state = STATE_ओळ_SEPARATOR;
                    ++row;
                    जर (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        परत buf;
                    }
                    अन्यथा जर(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        परत buf;
                    }
                    परत next();
                स्थिती STATE_CN_OPEN:
                    state = STATE_मूल्य;
                    जर (!alignOrder)
                        cn = 0;
                    जर (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        परत buf;
                    }
                    परत next();
                स्थिती STATE_CN_CLOSE:
                    ++col;
                    जर (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    अन्यथा
                        state = STATE_CN_SEPARATOR;
                    जर (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        परत buf;
                    }
                    परत next();
                स्थिती STATE_मूल्य:
                    (हे->*मूल्य_To_Str)();
                    state = STATE_CN_CLOSE;
                    जर (alignOrder)
                        परत buf;
                    जर (++cn < mcn)
                        state = STATE_मूल्य_SEPARATOR;
                    परत buf;
                स्थिती STATE_FINISHED:
                    परत 0;
                स्थिती STATE_ओळ_SEPARATOR:
                    जर (row >= mtx.rows)
                    {
                        जर (alignOrder)
                            state = STATE_INTERLUDE;
                        अन्यथा
                            state = STATE_EPILOGUE;
                        परत next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = single_ओळ ? ' ' : '\n';
                    buf[1] = 0;
                    परत buf;
                स्थिती STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    परत buf;
                स्थिती STATE_मूल्य_SEPARATOR:
                    state = STATE_मूल्य;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    परत buf;
            }
            परत 0;
        }
    };

    वर्ग FormatterBase : सार्वजनिक Formatter
    {
    सार्वजनिक:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(सत्य) {}

        शून्य set16fPrecision(पूर्णांक p) CV_OVERRIDE
        {
            prec16f = p;
        }

        शून्य set32fPrecision(पूर्णांक p) CV_OVERRIDE
        {
            prec32f = p;
        }

        शून्य set64fPrecision(पूर्णांक p) CV_OVERRIDE
        {
            prec64f = p;
        }

        शून्य setMultiline(तार्किक ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    संरक्षित:
        पूर्णांक prec16f;
        पूर्णांक prec32f;
        पूर्णांक prec64f;
        पूर्णांक multiline;
    };

    वर्ग DefaultFormatter CV_FINAL : सार्वजनिक FormatterBase
    {
    सार्वजनिक:

        Ptr<Formatted> format(स्थिरांक Mat& mtx) स्थिरांक CV_OVERRIDE
        {
            अक्षर braces[5] = {'\0', '\0', ';', '\0', '\0'};
            परत makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, असत्य, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    वर्ग MatlabFormatter CV_FINAL : सार्वजनिक FormatterBase
    {
    सार्वजनिक:

        Ptr<Formatted> format(स्थिरांक Mat& mtx) स्थिरांक CV_OVERRIDE
        {
            अक्षर braces[5] = {'\0', '\0', ';', '\0', '\0'};
            परत makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, सत्य, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    वर्ग PythonFormatter CV_FINAL : सार्वजनिक FormatterBase
    {
    सार्वजनिक:

        Ptr<Formatted> format(स्थिरांक Mat& mtx) स्थिरांक CV_OVERRIDE
        {
            अक्षर braces[5] = {'[', ']', ',', '[', ']'};
            जर (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            परत makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, असत्य, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    वर्ग NumpyFormatter CV_FINAL : सार्वजनिक FormatterBase
    {
    सार्वजनिक:

        Ptr<Formatted> format(स्थिरांक Mat& mtx) स्थिरांक CV_OVERRIDE
        {
            स्थिर स्थिरांक अक्षर* numpyTypes[CV_DEPTH_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            अक्षर braces[5] = {'[', ']', ',', '[', ']'};
            जर (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            परत makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.depth()]), mtx, &*braces,
                mtx.rows == 1 || !multiline, असत्य, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    वर्ग CSVFormatter CV_FINAL : सार्वजनिक FormatterBase
    {
    सार्वजनिक:

        Ptr<Formatted> format(स्थिरांक Mat& mtx) स्थिरांक CV_OVERRIDE
        {
            अक्षर braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            परत makePtr<FormattedImpl>(String(),
                mtx.rows > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.rows == 1 || !multiline, असत्य, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    वर्ग CFormatter CV_FINAL : सार्वजनिक FormatterBase
    {
    सार्वजनिक:

        Ptr<Formatted> format(स्थिरांक Mat& mtx) स्थिरांक CV_OVERRIDE
        {
            अक्षर braces[5] = {'\0', '\0', ',', '\0', '\0'};
            परत makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.rows == 1 || !multiline, असत्य, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        निवड(fmt)
        {
            स्थिती FMT_DEFAULT:
                परत makePtr<DefaultFormatter>();
            स्थिती FMT_MATLAB:
                परत makePtr<MatlabFormatter>();
            स्थिती FMT_CSV:
                परत makePtr<CSVFormatter>();
            स्थिती FMT_PYTHON:
                परत makePtr<PythonFormatter>();
            स्थिती FMT_NUMPY:
                परत makePtr<NumpyFormatter>();
            स्थिती FMT_C:
                परत makePtr<CFormatter>();
        }
        परत makePtr<DefaultFormatter>();
    }

    ढाँचा<typename _Tp> संरचना Fmt
    {
        प्रकारव्याख्या पूर्णांक temp_type;
        स्थिर स्थिरांक अक्षर* fmt() { परत "%d"; }
    };

    ढाँचा<> संरचना Fmt<uint32_t>
    {
        प्रकारव्याख्या अचिन्हित temp_type;
        स्थिर स्थिरांक अक्षर* fmt() { परत "%u"; }
    };

    ढाँचा<> संरचना Fmt<int64_t>
    {
        प्रकारव्याख्या दीर्घ दीर्घ temp_type;
        स्थिर स्थिरांक अक्षर* fmt() { परत "%lld"; }
    };

    ढाँचा<> संरचना Fmt<uint64_t>
    {
        प्रकारव्याख्या अचिन्हित दीर्घ दीर्घ temp_type;
        स्थिर स्थिरांक अक्षर* fmt() { परत "%llu"; }
    };

    ढाँचा<> संरचना Fmt<दशांश>
    {
        प्रकारव्याख्या दशांश temp_type;
        स्थिर स्थिरांक अक्षर* fmt() { परत "%.5g"; }
    };

    ढाँचा<> संरचना Fmt<द्विगुण>
    {
        प्रकारव्याख्या द्विगुण temp_type;
        स्थिर स्थिरांक अक्षर* fmt() { परत "%.5g"; }
    };

    ढाँचा<> संरचना Fmt<hfloat>
    {
        प्रकारव्याख्या दशांश temp_type;
        स्थिर स्थिरांक अक्षर* fmt() { परत "%.5g"; }
    };

    ढाँचा<> संरचना Fmt<bfloat>
    {
        प्रकारव्याख्या दशांश temp_type;
        स्थिर स्थिरांक अक्षर* fmt() { परत "%.4g"; }
    };

    ढाँचा<> संरचना Fmt<fp8_t>
    {
        प्रकारव्याख्या दशांश temp_type;
        स्थिर स्थिरांक अक्षर* fmt() { परत "%.4g"; }
    };

    ढाँचा<> संरचना Fmt<fp8a_t>
    {
        प्रकारव्याख्या दशांश temp_type;
        स्थिर स्थिरांक अक्षर* fmt() { परत "%.4g"; }
    };

    ढाँचा <typename _Tp>
    स्थिर शून्य pprintRow(std::ostream& strm, स्थिरांक _Tp* ptr, पूर्णांक n, माप_प्रकार ofs, पूर्णांक edge)
    {
        अक्षर buf[128];
        स्थिरांक अक्षर* fmt = Fmt<_Tp>::fmt();
        पूर्णांक i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        जर (edge == 0)
            edge = ndump;
        साठी (i = 0; i < ndump; i++) {
            पूर्णांक j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            जर (i > 0)
                strm << ", ";
            जर (j >= 0) {
                snprintf(buf, आकार(buf), fmt, (typename Fmt<_Tp>::temp_type)ptr[ofs + j]);
                strm << buf;
            } अन्यथा
                strm << "... ";
        }
    }

    स्थिर शून्य pprintSlice(std::ostream& strm, स्थिरांक Mat& tensor,
                            स्थिरांक माप_प्रकार* step, पूर्णांक d,
                            माप_प्रकार ofs, पूर्णांक edge)
    {
        MatShape shape = tensor.shape();
        पूर्णांक ndims = shape.dims;
        पूर्णांक n = d >= ndims ? 1 : shape[d];
        जर (d >= ndims - 1) {
            पूर्णांक typ = tensor.depth();
            शून्य* data = tensor.data;
            CV_Assert(data);
            n *= tensor.channels();
            जर (typ == CV_8U)
                pprintRow(strm, (स्थिरांक uint8_t*)data, n, ofs, edge);
            अन्यथा जर (typ == CV_8S)
                pprintRow(strm, (स्थिरांक int8_t*)data, n, ofs, edge);
            अन्यथा जर (typ == CV_16U)
                pprintRow(strm, (स्थिरांक uint16_t*)data, n, ofs, edge);
            अन्यथा जर (typ == CV_16S)
                pprintRow(strm, (स्थिरांक int16_t*)data, n, ofs, edge);
            अन्यथा जर (typ == CV_32U)
                pprintRow(strm, (स्थिरांक अचिन्हित*)data, n, ofs, edge);
            अन्यथा जर (typ == CV_32S)
                pprintRow(strm, (स्थिरांक पूर्णांक*)data, n, ofs, edge);
            अन्यथा जर (typ == CV_64U)
                pprintRow(strm, (स्थिरांक uint64_t*)data, n, ofs, edge);
            अन्यथा जर (typ == CV_64S)
                pprintRow(strm, (स्थिरांक int64_t*)data, n, ofs, edge);
            अन्यथा जर (typ == CV_32F)
                pprintRow(strm, (स्थिरांक दशांश*)data, n, ofs, edge);
            अन्यथा जर (typ == CV_64F)
                pprintRow(strm, (स्थिरांक द्विगुण*)data, n, ofs, edge);
            अन्यथा जर (typ == CV_16F)
                pprintRow(strm, (स्थिरांक hfloat*)data, n, ofs, edge);
            अन्यथा जर (typ == CV_16BF)
                pprintRow(strm, (स्थिरांक bfloat*)data, n, ofs, edge);
            अन्यथा जर (typ == CV_Bool)
                pprintRow(strm, (स्थिरांक तार्किक*)data, n, ofs, edge);
            अन्यथा जर (typ == CV_8F_E4M3FN)
                pprintRow(strm, (स्थिरांक fp8_t*)data, n, ofs, edge);
            अन्यथा जर (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (स्थिरांक fp8a_t*)data, n, ofs, edge);
            अन्यथा {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } अन्यथा {
            पूर्णांक i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            तार्किक dots = असत्य;
            साठी (i = 0; i < ndump; i++) {
                जर (i > 0 && !dots) {
                    पूर्णांक nempty_ओळी = ndims - 2 - d;
                    साठी (पूर्णांक k = 0; k < nempty_ओळी; k++)
                        strm << "\n";
                }
                जर (i > 0)
                    strm << "\n";
                पूर्णांक j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                जर (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                अन्यथा
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, आदान_Array array,
                         पूर्णांक /*indent*/, पूर्णांक edge_,
                         पूर्णांक wholeTensorThreshold,
                         अक्षर parens)
    {
        अक्षर oparen = parens;
        अक्षर cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        पूर्णांक edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        जर (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat temp;
            tensor.copyTo(temp);
            tensor = temp;
        }

        MatShape shape = tensor.shape();
        माप_प्रकार sz_all = tensor.total();

        जर (parens)
            strm << oparen;
        जर (sz_all == 0) {
            जर (!parens)
                strm << "<empty>";
        } अन्यथा {
            जर (sz_all <= (माप_प्रकार)wholeTensorThreshold)
                edge = 0;

            पूर्णांक ndims = shape.dims;
            पूर्णांक cn = tensor.channels();
            माप_प्रकार step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            साठी (पूर्णांक i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        जर (parens)
            strm << cparen;
        परत strm;
    }

} // cv
