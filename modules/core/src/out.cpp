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

#અન્તર્ભાવ "precomp.hpp"

નામાકાશ cv
{
    વર્ગ FormattedImpl CV_FINAL : જાહેર Formatted
    {
        ગણના { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_કિંમત, STATE_FINISHED,
               STATE_લીટી_SEPARATOR, STATE_CN_SEPARATOR, STATE_કિંમત_SEPARATOR };
        ગણના {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        અક્ષર floatFormat[8];
        અક્ષર buf[32];   // enough for double with precision up to 20

        Mat mtx;
        પૂર્ણાંક mcn; // == mtx.channels()
        તાર્કિક single_લીટી;
        તાર્કિક alignOrder;    // true when cn first order

        પૂર્ણાંક state;
        પૂર્ણાંક row;
        પૂર્ણાંક col;
        પૂર્ણાંક cn;

        String prologue;
        String epilogue;
        અક્ષર braces[5];

        શૂન્ય (FormattedImpl::*કિંમત_To_Str)();
        શૂન્ય કિંમત_To_Str_Bool() { snprintf(buf, કદ(buf), "%d", (પૂર્ણાંક)mtx.ptr<uchar>(row, col)[cn] != 0); }
        શૂન્ય કિંમત_To_Str8u()  { snprintf(buf, કદ(buf), "%3d", (પૂર્ણાંક)mtx.ptr<uchar>(row, col)[cn]); }
        શૂન્ય કિંમત_To_Str8s()  { snprintf(buf, કદ(buf), "%3d", (પૂર્ણાંક)mtx.ptr<schar>(row, col)[cn]); }
        શૂન્ય કિંમત_To_Str16u() { snprintf(buf, કદ(buf), "%d", (પૂર્ણાંક)mtx.ptr<ushort>(row, col)[cn]); }
        શૂન્ય કિંમત_To_Str16s() { snprintf(buf, કદ(buf), "%d", (પૂર્ણાંક)mtx.ptr<short>(row, col)[cn]); }
        શૂન્ય કિંમત_To_Str32u() { snprintf(buf, કદ(buf), "%u", mtx.ptr<unsigned>(row, col)[cn]); }
        શૂન્ય કિંમત_To_Str32s() { snprintf(buf, કદ(buf), "%d", mtx.ptr<પૂર્ણાંક>(row, col)[cn]); }
        શૂન્ય કિંમત_To_Str32f() { snprintf(buf, કદ(buf), floatFormat, mtx.ptr<દશાંશ>(row, col)[cn]); }
        શૂન્ય કિંમત_To_Str64f() { snprintf(buf, કદ(buf), floatFormat, mtx.ptr<દ્વિગુણ>(row, col)[cn]); }
        શૂન્ય કિંમત_To_Str64u() { snprintf(buf, કદ(buf), "%llu", (unsigned long long)mtx.ptr<uint64_t>(row, col)[cn]); }
        શૂન્ય કિંમત_To_Str64s() { snprintf(buf, કદ(buf), "%lld", (long long)mtx.ptr<int64_t>(row, col)[cn]); }
        શૂન્ય કિંમત_To_Str16f() { snprintf(buf, કદ(buf), floatFormat, (દશાંશ)mtx.ptr<hfloat>(row, col)[cn]); }
        શૂન્ય કિંમત_To_Str16bf() { snprintf(buf, કદ(buf), floatFormat, (દશાંશ)mtx.ptr<bfloat>(row, col)[cn]); }
        શૂન્ય કિંમત_To_Str8f()   { snprintf(buf, કદ(buf), floatFormat, (દશાંશ)mtx.ptr<fp8_t>(row, col)[cn]); }
        શૂન્ય કિંમત_To_Str8fnuz() { snprintf(buf, કદ(buf), floatFormat, (દશાંશ)mtx.ptr<fp8a_t>(row, col)[cn]); }
        શૂન્ય કિંમત_To_Str_Other() { buf[0] = 0; }

    જાહેર:

        FormattedImpl(String pl, String el, Mat m, અક્ષર br[5], તાર્કિક s_લીટી, તાર્કિક aOrder, પૂર્ણાંક precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            memcpy(braces, br, 5);
            state = STATE_PROLOGUE;
            single_લીટી = s_લીટી;
            alignOrder = aOrder;
            row = col = cn =0;

            જો (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            નહીંતર
            {
                cv_snprintf(floatFormat, કદ(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            પસંદગી(mtx.depth())
            {
                સ્થિતિ CV_8U:  કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str8u; તોડો;
                સ્થિતિ CV_8S:  કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str8s; તોડો;
                સ્થિતિ CV_Bool: કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str_Bool; તોડો;
                સ્થિતિ CV_16U: કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str16u; તોડો;
                સ્થિતિ CV_16S: કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str16s; તોડો;
                સ્થિતિ CV_32U: કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str32u; તોડો;
                સ્થિતિ CV_32S: કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str32s; તોડો;
                સ્થિતિ CV_32F: કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str32f; તોડો;
                સ્થિતિ CV_64F: કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str64f; તોડો;
                સ્થિતિ CV_64U: કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str64u; તોડો;
                સ્થિતિ CV_64S: કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str64s; તોડો;
                સ્થિતિ CV_16F: કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str16f; તોડો;
                સ્થિતિ CV_16BF: કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str16bf; તોડો;
                સ્થિતિ CV_8F_E4M3FN:   કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str8f; તોડો;
                સ્થિતિ CV_8F_E4M3FNUZ: કિંમત_To_Str = &FormattedImpl::કિંમત_To_Str8fnuz; તોડો;
                મૂળભૂત:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.depth()));
            }
        }

        શૂન્ય reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        અચલ અક્ષર* next() CV_OVERRIDE
        {
            પસંદગી(state)
            {
                સ્થિતિ STATE_PROLOGUE:
                    row = 0;
                    જો (mtx.empty())
                        state = STATE_EPILOGUE;
                    નહીંતર જો (alignOrder)
                        state = STATE_INTERLUDE;
                    નહીંતર
                        state = STATE_ROW_OPEN;
                    પાછા prologue.c_str();
                સ્થિતિ STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    જો (row >= mtx.rows)
                    {
                        જો (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            પાછા buf;
                        }
                        નહીંતર
                            row = 0;
                        snprintf(buf, કદ(buf), "\n(:, :, %d) = \n", cn+1);
                        પાછા buf;
                    }
                    snprintf(buf, કદ(buf), "(:, :, %d) = \n", cn+1);
                    પાછા buf;
                સ્થિતિ STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    પાછા epilogue.c_str();
                સ્થિતિ STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        જો (row > 0)
                            જ્યારે(pos < prologue.size() && pos < કદ(buf) - 2)
                                buf[pos++] = ' ';
                        જો (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        જો(!pos)
                            પાછા next();
                        buf[pos] = 0;
                    }
                    પાછા buf;
                સ્થિતિ STATE_ROW_CLOSE:
                    state = STATE_લીટી_SEPARATOR;
                    ++row;
                    જો (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        પાછા buf;
                    }
                    નહીંતર જો(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        પાછા buf;
                    }
                    પાછા next();
                સ્થિતિ STATE_CN_OPEN:
                    state = STATE_કિંમત;
                    જો (!alignOrder)
                        cn = 0;
                    જો (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        પાછા buf;
                    }
                    પાછા next();
                સ્થિતિ STATE_CN_CLOSE:
                    ++col;
                    જો (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    નહીંતર
                        state = STATE_CN_SEPARATOR;
                    જો (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        પાછા buf;
                    }
                    પાછા next();
                સ્થિતિ STATE_કિંમત:
                    (આ->*કિંમત_To_Str)();
                    state = STATE_CN_CLOSE;
                    જો (alignOrder)
                        પાછા buf;
                    જો (++cn < mcn)
                        state = STATE_કિંમત_SEPARATOR;
                    પાછા buf;
                સ્થિતિ STATE_FINISHED:
                    પાછા 0;
                સ્થિતિ STATE_લીટી_SEPARATOR:
                    જો (row >= mtx.rows)
                    {
                        જો (alignOrder)
                            state = STATE_INTERLUDE;
                        નહીંતર
                            state = STATE_EPILOGUE;
                        પાછા next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = single_લીટી ? ' ' : '\n';
                    buf[1] = 0;
                    પાછા buf;
                સ્થિતિ STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    પાછા buf;
                સ્થિતિ STATE_કિંમત_SEPARATOR:
                    state = STATE_કિંમત;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    પાછા buf;
            }
            પાછા 0;
        }
    };

    વર્ગ FormatterBase : જાહેર Formatter
    {
    જાહેર:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(સાચું) {}

        શૂન્ય set16fPrecision(પૂર્ણાંક p) CV_OVERRIDE
        {
            prec16f = p;
        }

        શૂન્ય set32fPrecision(પૂર્ણાંક p) CV_OVERRIDE
        {
            prec32f = p;
        }

        શૂન્ય set64fPrecision(પૂર્ણાંક p) CV_OVERRIDE
        {
            prec64f = p;
        }

        શૂન્ય setMultiline(તાર્કિક ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    સુરક્ષિત:
        પૂર્ણાંક prec16f;
        પૂર્ણાંક prec32f;
        પૂર્ણાંક prec64f;
        પૂર્ણાંક multiline;
    };

    વર્ગ DefaultFormatter CV_FINAL : જાહેર FormatterBase
    {
    જાહેર:

        Ptr<Formatted> format(અચલ Mat& mtx) અચલ CV_OVERRIDE
        {
            અક્ષર braces[5] = {'\0', '\0', ';', '\0', '\0'};
            પાછા makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, ખોટું, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    વર્ગ MatlabFormatter CV_FINAL : જાહેર FormatterBase
    {
    જાહેર:

        Ptr<Formatted> format(અચલ Mat& mtx) અચલ CV_OVERRIDE
        {
            અક્ષર braces[5] = {'\0', '\0', ';', '\0', '\0'};
            પાછા makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, સાચું, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    વર્ગ PythonFormatter CV_FINAL : જાહેર FormatterBase
    {
    જાહેર:

        Ptr<Formatted> format(અચલ Mat& mtx) અચલ CV_OVERRIDE
        {
            અક્ષર braces[5] = {'[', ']', ',', '[', ']'};
            જો (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            પાછા makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, ખોટું, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    વર્ગ NumpyFormatter CV_FINAL : જાહેર FormatterBase
    {
    જાહેર:

        Ptr<Formatted> format(અચલ Mat& mtx) અચલ CV_OVERRIDE
        {
            સ્થિર અચલ અક્ષર* numpyTypes[CV_DEPTH_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            અક્ષર braces[5] = {'[', ']', ',', '[', ']'};
            જો (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            પાછા makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.depth()]), mtx, &*braces,
                mtx.rows == 1 || !multiline, ખોટું, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    વર્ગ CSVFormatter CV_FINAL : જાહેર FormatterBase
    {
    જાહેર:

        Ptr<Formatted> format(અચલ Mat& mtx) અચલ CV_OVERRIDE
        {
            અક્ષર braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            પાછા makePtr<FormattedImpl>(String(),
                mtx.rows > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.rows == 1 || !multiline, ખોટું, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    વર્ગ CFormatter CV_FINAL : જાહેર FormatterBase
    {
    જાહેર:

        Ptr<Formatted> format(અચલ Mat& mtx) અચલ CV_OVERRIDE
        {
            અક્ષર braces[5] = {'\0', '\0', ',', '\0', '\0'};
            પાછા makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.rows == 1 || !multiline, ખોટું, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        પસંદગી(fmt)
        {
            સ્થિતિ FMT_DEFAULT:
                પાછા makePtr<DefaultFormatter>();
            સ્થિતિ FMT_MATLAB:
                પાછા makePtr<MatlabFormatter>();
            સ્થિતિ FMT_CSV:
                પાછા makePtr<CSVFormatter>();
            સ્થિતિ FMT_PYTHON:
                પાછા makePtr<PythonFormatter>();
            સ્થિતિ FMT_NUMPY:
                પાછા makePtr<NumpyFormatter>();
            સ્થિતિ FMT_C:
                પાછા makePtr<CFormatter>();
        }
        પાછા makePtr<DefaultFormatter>();
    }

    template<typename _Tp> સંરચના Fmt
    {
        પ્રકારવ્યાખ્યા પૂર્ણાંક temp_type;
        સ્થિર અચલ અક્ષર* fmt() { પાછા "%d"; }
    };

    template<> સંરચના Fmt<uint32_t>
    {
        પ્રકારવ્યાખ્યા unsigned temp_type;
        સ્થિર અચલ અક્ષર* fmt() { પાછા "%u"; }
    };

    template<> સંરચના Fmt<int64_t>
    {
        પ્રકારવ્યાખ્યા long long temp_type;
        સ્થિર અચલ અક્ષર* fmt() { પાછા "%lld"; }
    };

    template<> સંરચના Fmt<uint64_t>
    {
        પ્રકારવ્યાખ્યા unsigned long long temp_type;
        સ્થિર અચલ અક્ષર* fmt() { પાછા "%llu"; }
    };

    template<> સંરચના Fmt<દશાંશ>
    {
        પ્રકારવ્યાખ્યા દશાંશ temp_type;
        સ્થિર અચલ અક્ષર* fmt() { પાછા "%.5g"; }
    };

    template<> સંરચના Fmt<દ્વિગુણ>
    {
        પ્રકારવ્યાખ્યા દ્વિગુણ temp_type;
        સ્થિર અચલ અક્ષર* fmt() { પાછા "%.5g"; }
    };

    template<> સંરચના Fmt<hfloat>
    {
        પ્રકારવ્યાખ્યા દશાંશ temp_type;
        સ્થિર અચલ અક્ષર* fmt() { પાછા "%.5g"; }
    };

    template<> સંરચના Fmt<bfloat>
    {
        પ્રકારવ્યાખ્યા દશાંશ temp_type;
        સ્થિર અચલ અક્ષર* fmt() { પાછા "%.4g"; }
    };

    template<> સંરચના Fmt<fp8_t>
    {
        પ્રકારવ્યાખ્યા દશાંશ temp_type;
        સ્થિર અચલ અક્ષર* fmt() { પાછા "%.4g"; }
    };

    template<> સંરચના Fmt<fp8a_t>
    {
        પ્રકારવ્યાખ્યા દશાંશ temp_type;
        સ્થિર અચલ અક્ષર* fmt() { પાછા "%.4g"; }
    };

    template <typename _Tp>
    સ્થિર શૂન્ય pprintRow(std::ostream& strm, અચલ _Tp* ptr, પૂર્ણાંક n, size_t ofs, પૂર્ણાંક edge)
    {
        અક્ષર buf[128];
        અચલ અક્ષર* fmt = Fmt<_Tp>::fmt();
        પૂર્ણાંક i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        જો (edge == 0)
            edge = ndump;
        માટે (i = 0; i < ndump; i++) {
            પૂર્ણાંક j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            જો (i > 0)
                strm << ", ";
            જો (j >= 0) {
                snprintf(buf, કદ(buf), fmt, (typename Fmt<_Tp>::temp_type)ptr[ofs + j]);
                strm << buf;
            } નહીંતર
                strm << "... ";
        }
    }

    સ્થિર શૂન્ય pprintSlice(std::ostream& strm, અચલ Mat& tensor,
                            અચલ size_t* step, પૂર્ણાંક d,
                            size_t ofs, પૂર્ણાંક edge)
    {
        MatShape shape = tensor.shape();
        પૂર્ણાંક ndims = shape.dims;
        પૂર્ણાંક n = d >= ndims ? 1 : shape[d];
        જો (d >= ndims - 1) {
            પૂર્ણાંક typ = tensor.depth();
            શૂન્ય* data = tensor.data;
            CV_Assert(data);
            n *= tensor.channels();
            જો (typ == CV_8U)
                pprintRow(strm, (અચલ uint8_t*)data, n, ofs, edge);
            નહીંતર જો (typ == CV_8S)
                pprintRow(strm, (અચલ int8_t*)data, n, ofs, edge);
            નહીંતર જો (typ == CV_16U)
                pprintRow(strm, (અચલ uint16_t*)data, n, ofs, edge);
            નહીંતર જો (typ == CV_16S)
                pprintRow(strm, (અચલ int16_t*)data, n, ofs, edge);
            નહીંતર જો (typ == CV_32U)
                pprintRow(strm, (અચલ unsigned*)data, n, ofs, edge);
            નહીંતર જો (typ == CV_32S)
                pprintRow(strm, (અચલ પૂર્ણાંક*)data, n, ofs, edge);
            નહીંતર જો (typ == CV_64U)
                pprintRow(strm, (અચલ uint64_t*)data, n, ofs, edge);
            નહીંતર જો (typ == CV_64S)
                pprintRow(strm, (અચલ int64_t*)data, n, ofs, edge);
            નહીંતર જો (typ == CV_32F)
                pprintRow(strm, (અચલ દશાંશ*)data, n, ofs, edge);
            નહીંતર જો (typ == CV_64F)
                pprintRow(strm, (અચલ દ્વિગુણ*)data, n, ofs, edge);
            નહીંતર જો (typ == CV_16F)
                pprintRow(strm, (અચલ hfloat*)data, n, ofs, edge);
            નહીંતર જો (typ == CV_16BF)
                pprintRow(strm, (અચલ bfloat*)data, n, ofs, edge);
            નહીંતર જો (typ == CV_Bool)
                pprintRow(strm, (અચલ તાર્કિક*)data, n, ofs, edge);
            નહીંતર જો (typ == CV_8F_E4M3FN)
                pprintRow(strm, (અચલ fp8_t*)data, n, ofs, edge);
            નહીંતર જો (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (અચલ fp8a_t*)data, n, ofs, edge);
            નહીંતર {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } નહીંતર {
            પૂર્ણાંક i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            તાર્કિક dots = ખોટું;
            માટે (i = 0; i < ndump; i++) {
                જો (i > 0 && !dots) {
                    પૂર્ણાંક nempty_લીટીઓ = ndims - 2 - d;
                    માટે (પૂર્ણાંક k = 0; k < nempty_લીટીઓ; k++)
                        strm << "\n";
                }
                જો (i > 0)
                    strm << "\n";
                પૂર્ણાંક j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                જો (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                નહીંતર
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, આગત_Array array,
                         પૂર્ણાંક /*indent*/, પૂર્ણાંક edge_,
                         પૂર્ણાંક wholeTensorThreshold,
                         અક્ષર parens)
    {
        અક્ષર oparen = parens;
        અક્ષર cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        પૂર્ણાંક edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        જો (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat temp;
            tensor.copyTo(temp);
            tensor = temp;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.total();

        જો (parens)
            strm << oparen;
        જો (sz_all == 0) {
            જો (!parens)
                strm << "<empty>";
        } નહીંતર {
            જો (sz_all <= (size_t)wholeTensorThreshold)
                edge = 0;

            પૂર્ણાંક ndims = shape.dims;
            પૂર્ણાંક cn = tensor.channels();
            size_t step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            માટે (પૂર્ણાંક i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        જો (parens)
            strm << cparen;
        પાછા strm;
    }

} // cv
