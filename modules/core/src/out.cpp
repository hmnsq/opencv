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
        ចំនួនគត់ mcn; // == mtx.channels()
        bool singleLine;
        bool alignOrder;    // true when cn first order

        ចំនួនគត់ state;
        ចំនួនគត់ row;
        ចំនួនគត់ col;
        ចំនួនគត់ cn;

        String prologue;
        String epilogue;
        var_na braces[5];

        void (FormattedImpl::*valueToStr)();
        void valueToStrBool() { snprintf(buf, parimaa_na(buf), "%d", (ចំនួនគត់)mtx.ptr<uchar>(row, col)[cn] != 0); }
        void valueToStr8u()  { snprintf(buf, parimaa_na(buf), "%3d", (ចំនួនគត់)mtx.ptr<uchar>(row, col)[cn]); }
        void valueToStr8s()  { snprintf(buf, parimaa_na(buf), "%3d", (ចំនួនគត់)mtx.ptr<schar>(row, col)[cn]); }
        void valueToStr16u() { snprintf(buf, parimaa_na(buf), "%d", (ចំនួនគត់)mtx.ptr<ushort>(row, col)[cn]); }
        void valueToStr16s() { snprintf(buf, parimaa_na(buf), "%d", (ចំនួនគត់)mtx.ptr<short>(row, col)[cn]); }
        void valueToStr32u() { snprintf(buf, parimaa_na(buf), "%u", mtx.ptr<unsigned>(row, col)[cn]); }
        void valueToStr32s() { snprintf(buf, parimaa_na(buf), "%d", mtx.ptr<ចំនួនគត់>(row, col)[cn]); }
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

        FormattedImpl(String pl, String el, Mat m, var_na br[5], bool sLine, bool aOrder, ចំនួនគត់ precision)
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

            ប្រសិនបើ (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            បើមិនដូច្នេះ
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
                    ប្រសិនបើ (mtx.empty())
                        state = STATE_EPILOGUE;
                    បើមិនដូច្នេះ ប្រសិនបើ (alignOrder)
                        state = STATE_INTERLUDE;
                    បើមិនដូច្នេះ
                        state = STATE_ROW_OPEN;
                    ត្រឡប់ prologue.c_str();
                pak_sa STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    ប្រសិនបើ (row >= mtx.rows)
                    {
                        ប្រសិនបើ (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            ត្រឡប់ buf;
                        }
                        បើមិនដូច្នេះ
                            row = 0;
                        snprintf(buf, parimaa_na(buf), "\n(:, :, %d) = \n", cn+1);
                        ត្រឡប់ buf;
                    }
                    snprintf(buf, parimaa_na(buf), "(:, :, %d) = \n", cn+1);
                    ត្រឡប់ buf;
                pak_sa STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    ត្រឡប់ epilogue.c_str();
                pak_sa STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        ប្រសិនបើ (row > 0)
                            ខណៈពេល(pos < prologue.size() && pos < parimaa_na(buf) - 2)
                                buf[pos++] = ' ';
                        ប្រសិនបើ (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        ប្រសិនបើ(!pos)
                            ត្រឡប់ next();
                        buf[pos] = 0;
                    }
                    ត្រឡប់ buf;
                pak_sa STATE_ROW_CLOSE:
                    state = STATE_LINE_SEPARATOR;
                    ++row;
                    ប្រសិនបើ (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        ត្រឡប់ buf;
                    }
                    បើមិនដូច្នេះ ប្រសិនបើ(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        ត្រឡប់ buf;
                    }
                    ត្រឡប់ next();
                pak_sa STATE_CN_OPEN:
                    state = STATE_VALUE;
                    ប្រសិនបើ (!alignOrder)
                        cn = 0;
                    ប្រសិនបើ (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        ត្រឡប់ buf;
                    }
                    ត្រឡប់ next();
                pak_sa STATE_CN_CLOSE:
                    ++col;
                    ប្រសិនបើ (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    បើមិនដូច្នេះ
                        state = STATE_CN_SEPARATOR;
                    ប្រសិនបើ (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        ត្រឡប់ buf;
                    }
                    ត្រឡប់ next();
                pak_sa STATE_VALUE:
                    (this->*valueToStr)();
                    state = STATE_CN_CLOSE;
                    ប្រសិនបើ (alignOrder)
                        ត្រឡប់ buf;
                    ប្រសិនបើ (++cn < mcn)
                        state = STATE_VALUE_SEPARATOR;
                    ត្រឡប់ buf;
                pak_sa STATE_FINISHED:
                    ត្រឡប់ 0;
                pak_sa STATE_LINE_SEPARATOR:
                    ប្រសិនបើ (row >= mtx.rows)
                    {
                        ប្រសិនបើ (alignOrder)
                            state = STATE_INTERLUDE;
                        បើមិនដូច្នេះ
                            state = STATE_EPILOGUE;
                        ត្រឡប់ next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = singleLine ? ' ' : '\n';
                    buf[1] = 0;
                    ត្រឡប់ buf;
                pak_sa STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    ត្រឡប់ buf;
                pak_sa STATE_VALUE_SEPARATOR:
                    state = STATE_VALUE;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    ត្រឡប់ buf;
            }
            ត្រឡប់ 0;
        }
    };

    class FormatterBase : public Formatter
    {
    public:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(true) {}

        void set16fPrecision(ចំនួនគត់ p) CV_OVERRIDE
        {
            prec16f = p;
        }

        void set32fPrecision(ចំនួនគត់ p) CV_OVERRIDE
        {
            prec32f = p;
        }

        void set64fPrecision(ចំនួនគត់ p) CV_OVERRIDE
        {
            prec64f = p;
        }

        void setMultiline(bool ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    rak_sita:
        ចំនួនគត់ prec16f;
        ចំនួនគត់ prec32f;
        ចំនួនគត់ prec64f;
        ចំនួនគត់ multiline;
    };

    class DefaultFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            var_na braces[5] = {'\0', '\0', ';', '\0', '\0'};
            ត្រឡប់ makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    class MatlabFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            var_na braces[5] = {'\0', '\0', ';', '\0', '\0'};
            ត្រឡប់ makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, true, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    class PythonFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            var_na braces[5] = {'[', ']', ',', '[', ']'};
            ប្រសិនបើ (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            ត្រឡប់ makePtr<FormattedImpl>("[", "]", mtx, &*braces,
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
            ប្រសិនបើ (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            ត្រឡប់ makePtr<FormattedImpl>("array([",
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
            ត្រឡប់ makePtr<FormattedImpl>(String(),
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
            ត្រឡប់ makePtr<FormattedImpl>("{", "}", mtx, &*braces,
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
                ត្រឡប់ makePtr<DefaultFormatter>();
            pak_sa FMT_MATLAB:
                ត្រឡប់ makePtr<MatlabFormatter>();
            pak_sa FMT_CSV:
                ត្រឡប់ makePtr<CSVFormatter>();
            pak_sa FMT_PYTHON:
                ត្រឡប់ makePtr<PythonFormatter>();
            pak_sa FMT_NUMPY:
                ត្រឡប់ makePtr<NumpyFormatter>();
            pak_sa FMT_C:
                ត្រឡប់ makePtr<CFormatter>();
        }
        ត្រឡប់ makePtr<DefaultFormatter>();
    }

    template<typename _Tp> sa_nracanaa Fmt
    {
        typedef ចំនួនគត់ temp_type;
        static const var_na* fmt() { ត្រឡប់ "%d"; }
    };

    template<> sa_nracanaa Fmt<uint32_t>
    {
        typedef unsigned temp_type;
        static const var_na* fmt() { ត្រឡប់ "%u"; }
    };

    template<> sa_nracanaa Fmt<int64_t>
    {
        typedef long long temp_type;
        static const var_na* fmt() { ត្រឡប់ "%lld"; }
    };

    template<> sa_nracanaa Fmt<uint64_t>
    {
        typedef unsigned long long temp_type;
        static const var_na* fmt() { ត្រឡប់ "%llu"; }
    };

    template<> sa_nracanaa Fmt<float>
    {
        typedef float temp_type;
        static const var_na* fmt() { ត្រឡប់ "%.5g"; }
    };

    template<> sa_nracanaa Fmt<dviguu_na>
    {
        typedef dviguu_na temp_type;
        static const var_na* fmt() { ត្រឡប់ "%.5g"; }
    };

    template<> sa_nracanaa Fmt<hfloat>
    {
        typedef float temp_type;
        static const var_na* fmt() { ត្រឡប់ "%.5g"; }
    };

    template<> sa_nracanaa Fmt<bfloat>
    {
        typedef float temp_type;
        static const var_na* fmt() { ត្រឡប់ "%.4g"; }
    };

    template<> sa_nracanaa Fmt<fp8_t>
    {
        typedef float temp_type;
        static const var_na* fmt() { ត្រឡប់ "%.4g"; }
    };

    template<> sa_nracanaa Fmt<fp8a_t>
    {
        typedef float temp_type;
        static const var_na* fmt() { ត្រឡប់ "%.4g"; }
    };

    template <typename _Tp>
    static void pprintRow(std::ostream& strm, const _Tp* ptr, ចំនួនគត់ n, size_t ofs, ចំនួនគត់ edge)
    {
        var_na buf[128];
        const var_na* fmt = Fmt<_Tp>::fmt();
        ចំនួនគត់ i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        ប្រសិនបើ (edge == 0)
            edge = ndump;
        សម្រាប់ (i = 0; i < ndump; i++) {
            ចំនួនគត់ j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            ប្រសិនបើ (i > 0)
                strm << ", ";
            ប្រសិនបើ (j >= 0) {
                snprintf(buf, parimaa_na(buf), fmt, (typename Fmt<_Tp>::temp_type)ptr[ofs + j]);
                strm << buf;
            } បើមិនដូច្នេះ
                strm << "... ";
        }
    }

    static void pprintSlice(std::ostream& strm, const Mat& tensor,
                            const size_t* step, ចំនួនគត់ d,
                            size_t ofs, ចំនួនគត់ edge)
    {
        MatShape shape = tensor.shape();
        ចំនួនគត់ ndims = shape.dims;
        ចំនួនគត់ n = d >= ndims ? 1 : shape[d];
        ប្រសិនបើ (d >= ndims - 1) {
            ចំនួនគត់ typ = tensor.depth();
            void* data = tensor.data;
            CV_Assert(data);
            n *= tensor.channels();
            ប្រសិនបើ (typ == CV_8U)
                pprintRow(strm, (const uint8_t*)data, n, ofs, edge);
            បើមិនដូច្នេះ ប្រសិនបើ (typ == CV_8S)
                pprintRow(strm, (const int8_t*)data, n, ofs, edge);
            បើមិនដូច្នេះ ប្រសិនបើ (typ == CV_16U)
                pprintRow(strm, (const uint16_t*)data, n, ofs, edge);
            បើមិនដូច្នេះ ប្រសិនបើ (typ == CV_16S)
                pprintRow(strm, (const int16_t*)data, n, ofs, edge);
            បើមិនដូច្នេះ ប្រសិនបើ (typ == CV_32U)
                pprintRow(strm, (const unsigned*)data, n, ofs, edge);
            បើមិនដូច្នេះ ប្រសិនបើ (typ == CV_32S)
                pprintRow(strm, (const ចំនួនគត់*)data, n, ofs, edge);
            បើមិនដូច្នេះ ប្រសិនបើ (typ == CV_64U)
                pprintRow(strm, (const uint64_t*)data, n, ofs, edge);
            បើមិនដូច្នេះ ប្រសិនបើ (typ == CV_64S)
                pprintRow(strm, (const int64_t*)data, n, ofs, edge);
            បើមិនដូច្នេះ ប្រសិនបើ (typ == CV_32F)
                pprintRow(strm, (const float*)data, n, ofs, edge);
            បើមិនដូច្នេះ ប្រសិនបើ (typ == CV_64F)
                pprintRow(strm, (const dviguu_na*)data, n, ofs, edge);
            បើមិនដូច្នេះ ប្រសិនបើ (typ == CV_16F)
                pprintRow(strm, (const hfloat*)data, n, ofs, edge);
            បើមិនដូច្នេះ ប្រសិនបើ (typ == CV_16BF)
                pprintRow(strm, (const bfloat*)data, n, ofs, edge);
            បើមិនដូច្នេះ ប្រសិនបើ (typ == CV_Bool)
                pprintRow(strm, (const bool*)data, n, ofs, edge);
            បើមិនដូច្នេះ ប្រសិនបើ (typ == CV_8F_E4M3FN)
                pprintRow(strm, (const fp8_t*)data, n, ofs, edge);
            បើមិនដូច្នេះ ប្រសិនបើ (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (const fp8a_t*)data, n, ofs, edge);
            បើមិនដូច្នេះ {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } បើមិនដូច្នេះ {
            ចំនួនគត់ i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            bool dots = false;
            សម្រាប់ (i = 0; i < ndump; i++) {
                ប្រសិនបើ (i > 0 && !dots) {
                    ចំនួនគត់ nempty_lines = ndims - 2 - d;
                    សម្រាប់ (ចំនួនគត់ k = 0; k < nempty_lines; k++)
                        strm << "\n";
                }
                ប្រសិនបើ (i > 0)
                    strm << "\n";
                ចំនួនគត់ j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                ប្រសិនបើ (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                បើមិនដូច្នេះ
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, InputArray array,
                         ចំនួនគត់ /*indent*/, ចំនួនគត់ edge_,
                         ចំនួនគត់ wholeTensorThreshold,
                         var_na parens)
    {
        var_na oparen = parens;
        var_na cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        ចំនួនគត់ edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        ប្រសិនបើ (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat temp;
            tensor.copyTo(temp);
            tensor = temp;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.total();

        ប្រសិនបើ (parens)
            strm << oparen;
        ប្រសិនបើ (sz_all == 0) {
            ប្រសិនបើ (!parens)
                strm << "<empty>";
        } បើមិនដូច្នេះ {
            ប្រសិនបើ (sz_all <= (size_t)wholeTensorThreshold)
                edge = 0;

            ចំនួនគត់ ndims = shape.dims;
            ចំនួនគត់ cn = tensor.channels();
            size_t step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            សម្រាប់ (ចំនួនគត់ i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        ប្រសិនបើ (parens)
            strm << cparen;
        ត្រឡប់ strm;
    }

} // cv
