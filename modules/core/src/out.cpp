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

#include "precomp.hpp"

namespace cv
{
    class FormattedImpl CV_FINAL : public Formatted
    {
        ga_nanaa { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_VALUE, STATE_FINISHED,
               STATE_LINE_SEPARATOR, STATE_CN_SEPARATOR, STATE_VALUE_SEPARATOR };
        ga_nanaa {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        var_na floatFormat[8];
        var_na buf[32];   // enough for double with precision up to 20

        Mat mtx;
        puur_naa_nka mcn; // == mtx.channels()
        bool singleLine;
        bool alignOrder;    // true when cn first order

        puur_naa_nka state;
        puur_naa_nka row;
        puur_naa_nka col;
        puur_naa_nka cn;

        String prologue;
        String epilogue;
        var_na braces[5];

        void (FormattedImpl::*valueToStr)();
        void valueToStrBool() { snprintf(buf, parimaa_na(buf), "%d", (puur_naa_nka)mtx.ptr<uchar>(row, col)[cn] != 0); }
        void valueToStr8u()  { snprintf(buf, parimaa_na(buf), "%3d", (puur_naa_nka)mtx.ptr<uchar>(row, col)[cn]); }
        void valueToStr8s()  { snprintf(buf, parimaa_na(buf), "%3d", (puur_naa_nka)mtx.ptr<schar>(row, col)[cn]); }
        void valueToStr16u() { snprintf(buf, parimaa_na(buf), "%d", (puur_naa_nka)mtx.ptr<ushort>(row, col)[cn]); }
        void valueToStr16s() { snprintf(buf, parimaa_na(buf), "%d", (puur_naa_nka)mtx.ptr<short>(row, col)[cn]); }
        void valueToStr32u() { snprintf(buf, parimaa_na(buf), "%u", mtx.ptr<unsigned>(row, col)[cn]); }
        void valueToStr32s() { snprintf(buf, parimaa_na(buf), "%d", mtx.ptr<puur_naa_nka>(row, col)[cn]); }
        void valueToStr32f() { snprintf(buf, parimaa_na(buf), floatFormat, mtx.ptr<float>(row, col)[cn]); }
        void valueToStr64f() { snprintf(buf, parimaa_na(buf), floatFormat, mtx.ptr<dviguu_na>(row, col)[cn]); }
        void valueToStr64u() { snprintf(buf, parimaa_na(buf), "%llu", (unsigned long long)mtx.ptr<uint64_t>(row, col)[cn]); }
        void valueToStr64s() { snprintf(buf, parimaa_na(buf), "%lld", (long long)mtx.ptr<int64_t>(row, col)[cn]); }
        void valueToStr16f() { snprintf(buf, parimaa_na(buf), floatFormat, (float)mtx.ptr<hfloat>(row, col)[cn]); }
        void valueToStr16bf() { snprintf(buf, parimaa_na(buf), floatFormat, (float)mtx.ptr<bfloat>(row, col)[cn]); }
        void valueToStr8f()   { snprintf(buf, parimaa_na(buf), floatFormat, (float)mtx.ptr<fp8_t>(row, col)[cn]); }
        void valueToStr8fnuz() { snprintf(buf, parimaa_na(buf), floatFormat, (float)mtx.ptr<fp8a_t>(row, col)[cn]); }
        void valueToStrOther() { buf[0] = 0; }

    public:

        FormattedImpl(String pl, String el, Mat m, var_na br[5], bool sLine, bool aOrder, puur_naa_nka precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            memcpy(braces, br, 5);
            state = STATE_PROLOGUE;
            singleLine = sLine;
            alignOrder = aOrder;
            row = col = cn =0;

            ᱡᱩᱫᱤ (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            ᱵᱟᱝ
            {
                cv_snprintf(floatFormat, parimaa_na(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            switch(mtx.depth())
            {
                pak_sa CV_8U:  valueToStr = &FormattedImpl::valueToStr8u; bha_nga;
                pak_sa CV_8S:  valueToStr = &FormattedImpl::valueToStr8s; bha_nga;
                pak_sa CV_Bool: valueToStr = &FormattedImpl::valueToStrBool; bha_nga;
                pak_sa CV_16U: valueToStr = &FormattedImpl::valueToStr16u; bha_nga;
                pak_sa CV_16S: valueToStr = &FormattedImpl::valueToStr16s; bha_nga;
                pak_sa CV_32U: valueToStr = &FormattedImpl::valueToStr32u; bha_nga;
                pak_sa CV_32S: valueToStr = &FormattedImpl::valueToStr32s; bha_nga;
                pak_sa CV_32F: valueToStr = &FormattedImpl::valueToStr32f; bha_nga;
                pak_sa CV_64F: valueToStr = &FormattedImpl::valueToStr64f; bha_nga;
                pak_sa CV_64U: valueToStr = &FormattedImpl::valueToStr64u; bha_nga;
                pak_sa CV_64S: valueToStr = &FormattedImpl::valueToStr64s; bha_nga;
                pak_sa CV_16F: valueToStr = &FormattedImpl::valueToStr16f; bha_nga;
                pak_sa CV_16BF: valueToStr = &FormattedImpl::valueToStr16bf; bha_nga;
                pak_sa CV_8F_E4M3FN:   valueToStr = &FormattedImpl::valueToStr8f; bha_nga;
                pak_sa CV_8F_E4M3FNUZ: valueToStr = &FormattedImpl::valueToStr8fnuz; bha_nga;
                default:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.depth()));
            }
        }

        void reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        const var_na* next() CV_OVERRIDE
        {
            switch(state)
            {
                pak_sa STATE_PROLOGUE:
                    row = 0;
                    ᱡᱩᱫᱤ (mtx.empty())
                        state = STATE_EPILOGUE;
                    ᱵᱟᱝ ᱡᱩᱫᱤ (alignOrder)
                        state = STATE_INTERLUDE;
                    ᱵᱟᱝ
                        state = STATE_ROW_OPEN;
                    ᱨᱩᱣᱟᱹᱲ prologue.c_str();
                pak_sa STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    ᱡᱩᱫᱤ (row >= mtx.rows)
                    {
                        ᱡᱩᱫᱤ (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            ᱨᱩᱣᱟᱹᱲ buf;
                        }
                        ᱵᱟᱝ
                            row = 0;
                        snprintf(buf, parimaa_na(buf), "\n(:, :, %d) = \n", cn+1);
                        ᱨᱩᱣᱟᱹᱲ buf;
                    }
                    snprintf(buf, parimaa_na(buf), "(:, :, %d) = \n", cn+1);
                    ᱨᱩᱣᱟᱹᱲ buf;
                pak_sa STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    ᱨᱩᱣᱟᱹᱲ epilogue.c_str();
                pak_sa STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        ᱡᱩᱫᱤ (row > 0)
                            ᱡᱚᱠᱷᱚᱱ(pos < prologue.size() && pos < parimaa_na(buf) - 2)
                                buf[pos++] = ' ';
                        ᱡᱩᱫᱤ (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        ᱡᱩᱫᱤ(!pos)
                            ᱨᱩᱣᱟᱹᱲ next();
                        buf[pos] = 0;
                    }
                    ᱨᱩᱣᱟᱹᱲ buf;
                pak_sa STATE_ROW_CLOSE:
                    state = STATE_LINE_SEPARATOR;
                    ++row;
                    ᱡᱩᱫᱤ (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        ᱨᱩᱣᱟᱹᱲ buf;
                    }
                    ᱵᱟᱝ ᱡᱩᱫᱤ(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        ᱨᱩᱣᱟᱹᱲ buf;
                    }
                    ᱨᱩᱣᱟᱹᱲ next();
                pak_sa STATE_CN_OPEN:
                    state = STATE_VALUE;
                    ᱡᱩᱫᱤ (!alignOrder)
                        cn = 0;
                    ᱡᱩᱫᱤ (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        ᱨᱩᱣᱟᱹᱲ buf;
                    }
                    ᱨᱩᱣᱟᱹᱲ next();
                pak_sa STATE_CN_CLOSE:
                    ++col;
                    ᱡᱩᱫᱤ (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    ᱵᱟᱝ
                        state = STATE_CN_SEPARATOR;
                    ᱡᱩᱫᱤ (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        ᱨᱩᱣᱟᱹᱲ buf;
                    }
                    ᱨᱩᱣᱟᱹᱲ next();
                pak_sa STATE_VALUE:
                    (this->*valueToStr)();
                    state = STATE_CN_CLOSE;
                    ᱡᱩᱫᱤ (alignOrder)
                        ᱨᱩᱣᱟᱹᱲ buf;
                    ᱡᱩᱫᱤ (++cn < mcn)
                        state = STATE_VALUE_SEPARATOR;
                    ᱨᱩᱣᱟᱹᱲ buf;
                pak_sa STATE_FINISHED:
                    ᱨᱩᱣᱟᱹᱲ 0;
                pak_sa STATE_LINE_SEPARATOR:
                    ᱡᱩᱫᱤ (row >= mtx.rows)
                    {
                        ᱡᱩᱫᱤ (alignOrder)
                            state = STATE_INTERLUDE;
                        ᱵᱟᱝ
                            state = STATE_EPILOGUE;
                        ᱨᱩᱣᱟᱹᱲ next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = singleLine ? ' ' : '\n';
                    buf[1] = 0;
                    ᱨᱩᱣᱟᱹᱲ buf;
                pak_sa STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    ᱨᱩᱣᱟᱹᱲ buf;
                pak_sa STATE_VALUE_SEPARATOR:
                    state = STATE_VALUE;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    ᱨᱩᱣᱟᱹᱲ buf;
            }
            ᱨᱩᱣᱟᱹᱲ 0;
        }
    };

    class FormatterBase : public Formatter
    {
    public:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(true) {}

        void set16fPrecision(puur_naa_nka p) CV_OVERRIDE
        {
            prec16f = p;
        }

        void set32fPrecision(puur_naa_nka p) CV_OVERRIDE
        {
            prec32f = p;
        }

        void set64fPrecision(puur_naa_nka p) CV_OVERRIDE
        {
            prec64f = p;
        }

        void setMultiline(bool ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    rak_sita:
        puur_naa_nka prec16f;
        puur_naa_nka prec32f;
        puur_naa_nka prec64f;
        puur_naa_nka multiline;
    };

    class DefaultFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            var_na braces[5] = {'\0', '\0', ';', '\0', '\0'};
            ᱨᱩᱣᱟᱹᱲ makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    class MatlabFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            var_na braces[5] = {'\0', '\0', ';', '\0', '\0'};
            ᱨᱩᱣᱟᱹᱲ makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, true, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    class PythonFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            var_na braces[5] = {'[', ']', ',', '[', ']'};
            ᱡᱩᱫᱤ (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            ᱨᱩᱣᱟᱹᱲ makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    class NumpyFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            static const var_na* numpyTypes[CV_DEPTH_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            var_na braces[5] = {'[', ']', ',', '[', ']'};
            ᱡᱩᱫᱤ (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            ᱨᱩᱣᱟᱹᱲ makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.depth()]), mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    class CSVFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            var_na braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            ᱨᱩᱣᱟᱹᱲ makePtr<FormattedImpl>(String(),
                mtx.rows > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    class CFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            var_na braces[5] = {'\0', '\0', ',', '\0', '\0'};
            ᱨᱩᱣᱟᱹᱲ makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        switch(fmt)
        {
            pak_sa FMT_DEFAULT:
                ᱨᱩᱣᱟᱹᱲ makePtr<DefaultFormatter>();
            pak_sa FMT_MATLAB:
                ᱨᱩᱣᱟᱹᱲ makePtr<MatlabFormatter>();
            pak_sa FMT_CSV:
                ᱨᱩᱣᱟᱹᱲ makePtr<CSVFormatter>();
            pak_sa FMT_PYTHON:
                ᱨᱩᱣᱟᱹᱲ makePtr<PythonFormatter>();
            pak_sa FMT_NUMPY:
                ᱨᱩᱣᱟᱹᱲ makePtr<NumpyFormatter>();
            pak_sa FMT_C:
                ᱨᱩᱣᱟᱹᱲ makePtr<CFormatter>();
        }
        ᱨᱩᱣᱟᱹᱲ makePtr<DefaultFormatter>();
    }

    template<typename _Tp> sa_nracanaa Fmt
    {
        typedef puur_naa_nka temp_type;
        static const var_na* fmt() { ᱨᱩᱣᱟᱹᱲ "%d"; }
    };

    template<> sa_nracanaa Fmt<uint32_t>
    {
        typedef unsigned temp_type;
        static const var_na* fmt() { ᱨᱩᱣᱟᱹᱲ "%u"; }
    };

    template<> sa_nracanaa Fmt<int64_t>
    {
        typedef long long temp_type;
        static const var_na* fmt() { ᱨᱩᱣᱟᱹᱲ "%lld"; }
    };

    template<> sa_nracanaa Fmt<uint64_t>
    {
        typedef unsigned long long temp_type;
        static const var_na* fmt() { ᱨᱩᱣᱟᱹᱲ "%llu"; }
    };

    template<> sa_nracanaa Fmt<float>
    {
        typedef float temp_type;
        static const var_na* fmt() { ᱨᱩᱣᱟᱹᱲ "%.5g"; }
    };

    template<> sa_nracanaa Fmt<dviguu_na>
    {
        typedef dviguu_na temp_type;
        static const var_na* fmt() { ᱨᱩᱣᱟᱹᱲ "%.5g"; }
    };

    template<> sa_nracanaa Fmt<hfloat>
    {
        typedef float temp_type;
        static const var_na* fmt() { ᱨᱩᱣᱟᱹᱲ "%.5g"; }
    };

    template<> sa_nracanaa Fmt<bfloat>
    {
        typedef float temp_type;
        static const var_na* fmt() { ᱨᱩᱣᱟᱹᱲ "%.4g"; }
    };

    template<> sa_nracanaa Fmt<fp8_t>
    {
        typedef float temp_type;
        static const var_na* fmt() { ᱨᱩᱣᱟᱹᱲ "%.4g"; }
    };

    template<> sa_nracanaa Fmt<fp8a_t>
    {
        typedef float temp_type;
        static const var_na* fmt() { ᱨᱩᱣᱟᱹᱲ "%.4g"; }
    };

    template <typename _Tp>
    static void pprintRow(std::ostream& strm, const _Tp* ptr, puur_naa_nka n, size_t ofs, puur_naa_nka edge)
    {
        var_na buf[128];
        const var_na* fmt = Fmt<_Tp>::fmt();
        puur_naa_nka i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        ᱡᱩᱫᱤ (edge == 0)
            edge = ndump;
        for (i = 0; i < ndump; i++) {
            puur_naa_nka j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            ᱡᱩᱫᱤ (i > 0)
                strm << ", ";
            ᱡᱩᱫᱤ (j >= 0) {
                snprintf(buf, parimaa_na(buf), fmt, (typename Fmt<_Tp>::temp_type)ptr[ofs + j]);
                strm << buf;
            } ᱵᱟᱝ
                strm << "... ";
        }
    }

    static void pprintSlice(std::ostream& strm, const Mat& tensor,
                            const size_t* step, puur_naa_nka d,
                            size_t ofs, puur_naa_nka edge)
    {
        MatShape shape = tensor.shape();
        puur_naa_nka ndims = shape.dims;
        puur_naa_nka n = d >= ndims ? 1 : shape[d];
        ᱡᱩᱫᱤ (d >= ndims - 1) {
            puur_naa_nka typ = tensor.depth();
            void* data = tensor.data;
            CV_Assert(data);
            n *= tensor.channels();
            ᱡᱩᱫᱤ (typ == CV_8U)
                pprintRow(strm, (const uint8_t*)data, n, ofs, edge);
            ᱵᱟᱝ ᱡᱩᱫᱤ (typ == CV_8S)
                pprintRow(strm, (const int8_t*)data, n, ofs, edge);
            ᱵᱟᱝ ᱡᱩᱫᱤ (typ == CV_16U)
                pprintRow(strm, (const uint16_t*)data, n, ofs, edge);
            ᱵᱟᱝ ᱡᱩᱫᱤ (typ == CV_16S)
                pprintRow(strm, (const int16_t*)data, n, ofs, edge);
            ᱵᱟᱝ ᱡᱩᱫᱤ (typ == CV_32U)
                pprintRow(strm, (const unsigned*)data, n, ofs, edge);
            ᱵᱟᱝ ᱡᱩᱫᱤ (typ == CV_32S)
                pprintRow(strm, (const puur_naa_nka*)data, n, ofs, edge);
            ᱵᱟᱝ ᱡᱩᱫᱤ (typ == CV_64U)
                pprintRow(strm, (const uint64_t*)data, n, ofs, edge);
            ᱵᱟᱝ ᱡᱩᱫᱤ (typ == CV_64S)
                pprintRow(strm, (const int64_t*)data, n, ofs, edge);
            ᱵᱟᱝ ᱡᱩᱫᱤ (typ == CV_32F)
                pprintRow(strm, (const float*)data, n, ofs, edge);
            ᱵᱟᱝ ᱡᱩᱫᱤ (typ == CV_64F)
                pprintRow(strm, (const dviguu_na*)data, n, ofs, edge);
            ᱵᱟᱝ ᱡᱩᱫᱤ (typ == CV_16F)
                pprintRow(strm, (const hfloat*)data, n, ofs, edge);
            ᱵᱟᱝ ᱡᱩᱫᱤ (typ == CV_16BF)
                pprintRow(strm, (const bfloat*)data, n, ofs, edge);
            ᱵᱟᱝ ᱡᱩᱫᱤ (typ == CV_Bool)
                pprintRow(strm, (const bool*)data, n, ofs, edge);
            ᱵᱟᱝ ᱡᱩᱫᱤ (typ == CV_8F_E4M3FN)
                pprintRow(strm, (const fp8_t*)data, n, ofs, edge);
            ᱵᱟᱝ ᱡᱩᱫᱤ (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (const fp8a_t*)data, n, ofs, edge);
            ᱵᱟᱝ {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } ᱵᱟᱝ {
            puur_naa_nka i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            bool dots = false;
            for (i = 0; i < ndump; i++) {
                ᱡᱩᱫᱤ (i > 0 && !dots) {
                    puur_naa_nka nempty_lines = ndims - 2 - d;
                    for (puur_naa_nka k = 0; k < nempty_lines; k++)
                        strm << "\n";
                }
                ᱡᱩᱫᱤ (i > 0)
                    strm << "\n";
                puur_naa_nka j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                ᱡᱩᱫᱤ (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                ᱵᱟᱝ
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, InputArray array,
                         puur_naa_nka /*indent*/, puur_naa_nka edge_,
                         puur_naa_nka wholeTensorThreshold,
                         var_na parens)
    {
        var_na oparen = parens;
        var_na cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        puur_naa_nka edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        ᱡᱩᱫᱤ (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat temp;
            tensor.copyTo(temp);
            tensor = temp;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.total();

        ᱡᱩᱫᱤ (parens)
            strm << oparen;
        ᱡᱩᱫᱤ (sz_all == 0) {
            ᱡᱩᱫᱤ (!parens)
                strm << "<empty>";
        } ᱵᱟᱝ {
            ᱡᱩᱫᱤ (sz_all <= (size_t)wholeTensorThreshold)
                edge = 0;

            puur_naa_nka ndims = shape.dims;
            puur_naa_nka cn = tensor.channels();
            size_t step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            for (puur_naa_nka i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        ᱡᱩᱫᱤ (parens)
            strm << cparen;
        ᱨᱩᱣᱟᱹᱲ strm;
    }

} // cv
