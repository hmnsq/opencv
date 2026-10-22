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

#समावेश "precomp.hpp"

namespace cv
{
    श्रेणी FormattedImpl CV_FINAL : खुला Formatted
    {
        क्रमागत { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_मूल्य, STATE_FINISHED,
               STATE_पंक्ति_SEPARATOR, STATE_CN_SEPARATOR, STATE_मूल्य_SEPARATOR };
        क्रमागत {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        अक्षर floatFormat[8];
        अक्षर buf[32];   // enough for double with precision up to 20

        Mat mtx;
        पूर्णांक mcn; // == mtx.channels()
        bool single_पंक्ति;
        bool alignOrder;    // true when cn first order

        पूर्णांक state;
        पूर्णांक row;
        पूर्णांक col;
        पूर्णांक cn;

        String prologue;
        String epilogue;
        अक्षर braces[5];

        व्योम (FormattedImpl::*मूल्य_To_Str)();
        व्योम मूल्य_To_Str_Bool() { snprintf(buf, माप(buf), "%d", (पूर्णांक)mtx.ptr<uchar>(row, col)[cn] != 0); }
        व्योम मूल्य_To_Str8u()  { snprintf(buf, माप(buf), "%3d", (पूर्णांक)mtx.ptr<uchar>(row, col)[cn]); }
        व्योम मूल्य_To_Str8s()  { snprintf(buf, माप(buf), "%3d", (पूर्णांक)mtx.ptr<schar>(row, col)[cn]); }
        व्योम मूल्य_To_Str16u() { snprintf(buf, माप(buf), "%d", (पूर्णांक)mtx.ptr<ushort>(row, col)[cn]); }
        व्योम मूल्य_To_Str16s() { snprintf(buf, माप(buf), "%d", (पूर्णांक)mtx.ptr<लघु>(row, col)[cn]); }
        व्योम मूल्य_To_Str32u() { snprintf(buf, माप(buf), "%u", mtx.ptr<अचिन्हित>(row, col)[cn]); }
        व्योम मूल्य_To_Str32s() { snprintf(buf, माप(buf), "%d", mtx.ptr<पूर्णांक>(row, col)[cn]); }
        व्योम मूल्य_To_Str32f() { snprintf(buf, माप(buf), floatFormat, mtx.ptr<भग्न>(row, col)[cn]); }
        व्योम मूल्य_To_Str64f() { snprintf(buf, माप(buf), floatFormat, mtx.ptr<द्विगुन>(row, col)[cn]); }
        व्योम मूल्य_To_Str64u() { snprintf(buf, माप(buf), "%llu", (अचिन्हित दीर्घ दीर्घ)mtx.ptr<uint64_t>(row, col)[cn]); }
        व्योम मूल्य_To_Str64s() { snprintf(buf, माप(buf), "%lld", (दीर्घ दीर्घ)mtx.ptr<int64_t>(row, col)[cn]); }
        व्योम मूल्य_To_Str16f() { snprintf(buf, माप(buf), floatFormat, (भग्न)mtx.ptr<hfloat>(row, col)[cn]); }
        व्योम मूल्य_To_Str16bf() { snprintf(buf, माप(buf), floatFormat, (भग्न)mtx.ptr<bfloat>(row, col)[cn]); }
        व्योम मूल्य_To_Str8f()   { snprintf(buf, माप(buf), floatFormat, (भग्न)mtx.ptr<fp8_t>(row, col)[cn]); }
        व्योम मूल्य_To_Str8fnuz() { snprintf(buf, माप(buf), floatFormat, (भग्न)mtx.ptr<fp8a_t>(row, col)[cn]); }
        व्योम मूल्य_To_Str_Other() { buf[0] = 0; }

    खुला:

        FormattedImpl(String pl, String el, Mat m, अक्षर br[5], bool s_पंक्ति, bool aOrder, पूर्णांक precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            स_नकल(braces, br, 5);
            state = STATE_PROLOGUE;
            single_पंक्ति = s_पंक्ति;
            alignOrder = aOrder;
            row = col = cn =0;

            अगर (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            अन्यथा
            {
                cv_snprintf(floatFormat, माप(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            चयन(mtx.depth())
            {
                हाल CV_8U:  मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str8u; अवरोध;
                हाल CV_8S:  मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str8s; अवरोध;
                हाल CV_Bool: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str_Bool; अवरोध;
                हाल CV_16U: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str16u; अवरोध;
                हाल CV_16S: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str16s; अवरोध;
                हाल CV_32U: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str32u; अवरोध;
                हाल CV_32S: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str32s; अवरोध;
                हाल CV_32F: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str32f; अवरोध;
                हाल CV_64F: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str64f; अवरोध;
                हाल CV_64U: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str64u; अवरोध;
                हाल CV_64S: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str64s; अवरोध;
                हाल CV_16F: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str16f; अवरोध;
                हाल CV_16BF: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str16bf; अवरोध;
                हाल CV_8F_E4M3FN:   मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str8f; अवरोध;
                हाल CV_8F_E4M3FNUZ: मूल्य_To_Str = &FormattedImpl::मूल्य_To_Str8fnuz; अवरोध;
                शेष:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.depth()));
            }
        }

        व्योम reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        स्थिर अक्षर* next() CV_OVERRIDE
        {
            चयन(state)
            {
                हाल STATE_PROLOGUE:
                    row = 0;
                    अगर (mtx.empty())
                        state = STATE_EPILOGUE;
                    अन्यथा अगर (alignOrder)
                        state = STATE_INTERLUDE;
                    अन्यथा
                        state = STATE_ROW_OPEN;
                    वापस prologue.c_str();
                हाल STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    अगर (row >= mtx.rows)
                    {
                        अगर (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            वापस buf;
                        }
                        अन्यथा
                            row = 0;
                        snprintf(buf, माप(buf), "\n(:, :, %d) = \n", cn+1);
                        वापस buf;
                    }
                    snprintf(buf, माप(buf), "(:, :, %d) = \n", cn+1);
                    वापस buf;
                हाल STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    वापस epilogue.c_str();
                हाल STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        माप_प्रकार pos = 0;
                        अगर (row > 0)
                            जबतक(pos < prologue.size() && pos < माप(buf) - 2)
                                buf[pos++] = ' ';
                        अगर (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        अगर(!pos)
                            वापस next();
                        buf[pos] = 0;
                    }
                    वापस buf;
                हाल STATE_ROW_CLOSE:
                    state = STATE_पंक्ति_SEPARATOR;
                    ++row;
                    अगर (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        वापस buf;
                    }
                    अन्यथा अगर(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        वापस buf;
                    }
                    वापस next();
                हाल STATE_CN_OPEN:
                    state = STATE_मूल्य;
                    अगर (!alignOrder)
                        cn = 0;
                    अगर (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        वापस buf;
                    }
                    वापस next();
                हाल STATE_CN_CLOSE:
                    ++col;
                    अगर (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    अन्यथा
                        state = STATE_CN_SEPARATOR;
                    अगर (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        वापस buf;
                    }
                    वापस next();
                हाल STATE_मूल्य:
                    (यह->*मूल्य_To_Str)();
                    state = STATE_CN_CLOSE;
                    अगर (alignOrder)
                        वापस buf;
                    अगर (++cn < mcn)
                        state = STATE_मूल्य_SEPARATOR;
                    वापस buf;
                हाल STATE_FINISHED:
                    वापस 0;
                हाल STATE_पंक्ति_SEPARATOR:
                    अगर (row >= mtx.rows)
                    {
                        अगर (alignOrder)
                            state = STATE_INTERLUDE;
                        अन्यथा
                            state = STATE_EPILOGUE;
                        वापस next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = single_पंक्ति ? ' ' : '\n';
                    buf[1] = 0;
                    वापस buf;
                हाल STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    वापस buf;
                हाल STATE_मूल्य_SEPARATOR:
                    state = STATE_मूल्य;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    वापस buf;
            }
            वापस 0;
        }
    };

    श्रेणी FormatterBase : खुला Formatter
    {
    खुला:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(true) {}

        व्योम set16fPrecision(पूर्णांक p) CV_OVERRIDE
        {
            prec16f = p;
        }

        व्योम set32fPrecision(पूर्णांक p) CV_OVERRIDE
        {
            prec32f = p;
        }

        व्योम set64fPrecision(पूर्णांक p) CV_OVERRIDE
        {
            prec64f = p;
        }

        व्योम setMultiline(bool ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    रक्षित:
        पूर्णांक prec16f;
        पूर्णांक prec32f;
        पूर्णांक prec64f;
        पूर्णांक multiline;
    };

    श्रेणी DefaultFormatter CV_FINAL : खुला FormatterBase
    {
    खुला:

        Ptr<Formatted> format(स्थिर Mat& mtx) स्थिर CV_OVERRIDE
        {
            अक्षर braces[5] = {'\0', '\0', ';', '\0', '\0'};
            वापस makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    श्रेणी MatlabFormatter CV_FINAL : खुला FormatterBase
    {
    खुला:

        Ptr<Formatted> format(स्थिर Mat& mtx) स्थिर CV_OVERRIDE
        {
            अक्षर braces[5] = {'\0', '\0', ';', '\0', '\0'};
            वापस makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, true, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    श्रेणी PythonFormatter CV_FINAL : खुला FormatterBase
    {
    खुला:

        Ptr<Formatted> format(स्थिर Mat& mtx) स्थिर CV_OVERRIDE
        {
            अक्षर braces[5] = {'[', ']', ',', '[', ']'};
            अगर (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            वापस makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    श्रेणी NumpyFormatter CV_FINAL : खुला FormatterBase
    {
    खुला:

        Ptr<Formatted> format(स्थिर Mat& mtx) स्थिर CV_OVERRIDE
        {
            अटल स्थिर अक्षर* numpyTypes[CV_DEPTH_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            अक्षर braces[5] = {'[', ']', ',', '[', ']'};
            अगर (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            वापस makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.depth()]), mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    श्रेणी CSVFormatter CV_FINAL : खुला FormatterBase
    {
    खुला:

        Ptr<Formatted> format(स्थिर Mat& mtx) स्थिर CV_OVERRIDE
        {
            अक्षर braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            वापस makePtr<FormattedImpl>(String(),
                mtx.rows > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    श्रेणी CFormatter CV_FINAL : खुला FormatterBase
    {
    खुला:

        Ptr<Formatted> format(स्थिर Mat& mtx) स्थिर CV_OVERRIDE
        {
            अक्षर braces[5] = {'\0', '\0', ',', '\0', '\0'};
            वापस makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::लो(Formatter::FormatType fmt)
    {
        चयन(fmt)
        {
            हाल FMT_DEFAULT:
                वापस makePtr<DefaultFormatter>();
            हाल FMT_MATLAB:
                वापस makePtr<MatlabFormatter>();
            हाल FMT_CSV:
                वापस makePtr<CSVFormatter>();
            हाल FMT_PYTHON:
                वापस makePtr<PythonFormatter>();
            हाल FMT_NUMPY:
                वापस makePtr<NumpyFormatter>();
            हाल FMT_C:
                वापस makePtr<CFormatter>();
        }
        वापस makePtr<DefaultFormatter>();
    }

    ढाँचा<typename _Tp> काष्ठा Fmt
    {
        प्रकार पूर्णांक अस्थायी_type;
        अटल स्थिर अक्षर* fmt() { वापस "%d"; }
    };

    ढाँचा<> काष्ठा Fmt<uint32_t>
    {
        प्रकार अचिन्हित अस्थायी_type;
        अटल स्थिर अक्षर* fmt() { वापस "%u"; }
    };

    ढाँचा<> काष्ठा Fmt<int64_t>
    {
        प्रकार दीर्घ दीर्घ अस्थायी_type;
        अटल स्थिर अक्षर* fmt() { वापस "%lld"; }
    };

    ढाँचा<> काष्ठा Fmt<uint64_t>
    {
        प्रकार अचिन्हित दीर्घ दीर्घ अस्थायी_type;
        अटल स्थिर अक्षर* fmt() { वापस "%llu"; }
    };

    ढाँचा<> काष्ठा Fmt<भग्न>
    {
        प्रकार भग्न अस्थायी_type;
        अटल स्थिर अक्षर* fmt() { वापस "%.5g"; }
    };

    ढाँचा<> काष्ठा Fmt<द्विगुन>
    {
        प्रकार द्विगुन अस्थायी_type;
        अटल स्थिर अक्षर* fmt() { वापस "%.5g"; }
    };

    ढाँचा<> काष्ठा Fmt<hfloat>
    {
        प्रकार भग्न अस्थायी_type;
        अटल स्थिर अक्षर* fmt() { वापस "%.5g"; }
    };

    ढाँचा<> काष्ठा Fmt<bfloat>
    {
        प्रकार भग्न अस्थायी_type;
        अटल स्थिर अक्षर* fmt() { वापस "%.4g"; }
    };

    ढाँचा<> काष्ठा Fmt<fp8_t>
    {
        प्रकार भग्न अस्थायी_type;
        अटल स्थिर अक्षर* fmt() { वापस "%.4g"; }
    };

    ढाँचा<> काष्ठा Fmt<fp8a_t>
    {
        प्रकार भग्न अस्थायी_type;
        अटल स्थिर अक्षर* fmt() { वापस "%.4g"; }
    };

    ढाँचा <typename _Tp>
    अटल व्योम pprintRow(std::ब_प्रवाह& strm, स्थिर _Tp* ptr, पूर्णांक n, माप_प्रकार ofs, पूर्णांक edge)
    {
        अक्षर buf[128];
        स्थिर अक्षर* fmt = Fmt<_Tp>::fmt();
        पूर्णांक i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        अगर (edge == 0)
            edge = ndump;
        क्रम (i = 0; i < ndump; i++) {
            पूर्णांक j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            अगर (i > 0)
                strm << ", ";
            अगर (j >= 0) {
                snprintf(buf, माप(buf), fmt, (typename Fmt<_Tp>::अस्थायी_type)ptr[ofs + j]);
                strm << buf;
            } अन्यथा
                strm << "... ";
        }
    }

    अटल व्योम pprintSlice(std::ब_प्रवाह& strm, स्थिर Mat& tensor,
                            स्थिर माप_प्रकार* step, पूर्णांक d,
                            माप_प्रकार ofs, पूर्णांक edge)
    {
        MatShape shape = tensor.shape();
        पूर्णांक ndims = shape.dims;
        पूर्णांक n = d >= ndims ? 1 : shape[d];
        अगर (d >= ndims - 1) {
            पूर्णांक typ = tensor.depth();
            व्योम* तथ्य = tensor.तथ्य;
            CV_Assert(तथ्य);
            n *= tensor.channels();
            अगर (typ == CV_8U)
                pprintRow(strm, (स्थिर uint8_t*)तथ्य, n, ofs, edge);
            अन्यथा अगर (typ == CV_8S)
                pprintRow(strm, (स्थिर int8_t*)तथ्य, n, ofs, edge);
            अन्यथा अगर (typ == CV_16U)
                pprintRow(strm, (स्थिर uint16_t*)तथ्य, n, ofs, edge);
            अन्यथा अगर (typ == CV_16S)
                pprintRow(strm, (स्थिर int16_t*)तथ्य, n, ofs, edge);
            अन्यथा अगर (typ == CV_32U)
                pprintRow(strm, (स्थिर अचिन्हित*)तथ्य, n, ofs, edge);
            अन्यथा अगर (typ == CV_32S)
                pprintRow(strm, (स्थिर पूर्णांक*)तथ्य, n, ofs, edge);
            अन्यथा अगर (typ == CV_64U)
                pprintRow(strm, (स्थिर uint64_t*)तथ्य, n, ofs, edge);
            अन्यथा अगर (typ == CV_64S)
                pprintRow(strm, (स्थिर int64_t*)तथ्य, n, ofs, edge);
            अन्यथा अगर (typ == CV_32F)
                pprintRow(strm, (स्थिर भग्न*)तथ्य, n, ofs, edge);
            अन्यथा अगर (typ == CV_64F)
                pprintRow(strm, (स्थिर द्विगुन*)तथ्य, n, ofs, edge);
            अन्यथा अगर (typ == CV_16F)
                pprintRow(strm, (स्थिर hfloat*)तथ्य, n, ofs, edge);
            अन्यथा अगर (typ == CV_16BF)
                pprintRow(strm, (स्थिर bfloat*)तथ्य, n, ofs, edge);
            अन्यथा अगर (typ == CV_Bool)
                pprintRow(strm, (स्थिर bool*)तथ्य, n, ofs, edge);
            अन्यथा अगर (typ == CV_8F_E4M3FN)
                pprintRow(strm, (स्थिर fp8_t*)तथ्य, n, ofs, edge);
            अन्यथा अगर (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (स्थिर fp8a_t*)तथ्य, n, ofs, edge);
            अन्यथा {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } अन्यथा {
            पूर्णांक i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            bool dots = false;
            क्रम (i = 0; i < ndump; i++) {
                अगर (i > 0 && !dots) {
                    पूर्णांक nempty_पंक्तियाँ = ndims - 2 - d;
                    क्रम (पूर्णांक k = 0; k < nempty_पंक्तियाँ; k++)
                        strm << "\n";
                }
                अगर (i > 0)
                    strm << "\n";
                पूर्णांक j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                अगर (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                अन्यथा
                    strm << "...";
            }
        }
    }

    std::ब_प्रवाह& pprint(std::ब_प्रवाह& strm, निवेश_Array array,
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
        अगर (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat अस्थायी_2;
            tensor.copyTo(अस्थायी_2);
            tensor = अस्थायी_2;
        }

        MatShape shape = tensor.shape();
        माप_प्रकार sz_all = tensor.योग();

        अगर (parens)
            strm << oparen;
        अगर (sz_all == 0) {
            अगर (!parens)
                strm << "<empty>";
        } अन्यथा {
            अगर (sz_all <= (माप_प्रकार)wholeTensorThreshold)
                edge = 0;

            पूर्णांक ndims = shape.dims;
            पूर्णांक cn = tensor.channels();
            माप_प्रकार step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            क्रम (पूर्णांक i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        अगर (parens)
            strm << cparen;
        वापस strm;
    }

} // cv
