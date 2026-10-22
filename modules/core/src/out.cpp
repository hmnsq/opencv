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
        enum { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_VALUE, STATE_FINISHED,
               STATE_LINE_SEPARATOR, STATE_CN_SEPARATOR, STATE_VALUE_SEPARATOR };
        enum {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        char floatFormat[8];
        char buf[32];   // enough for double with precision up to 20

        Mat mtx;
        цяло mcn; // == mtx.channels()
        bool singleLine;
        bool alignOrder;    // true when cn first order

        цяло state;
        цяло row;
        цяло col;
        цяло cn;

        String prologue;
        String epilogue;
        char braces[5];

        void (FormattedImpl::*valueToStr)();
        void valueToStrBool() { snprintf(buf, sizeof(buf), "%d", (цяло)mtx.ptr<uchar>(row, col)[cn] != 0); }
        void valueToStr8u()  { snprintf(buf, sizeof(buf), "%3d", (цяло)mtx.ptr<uchar>(row, col)[cn]); }
        void valueToStr8s()  { snprintf(buf, sizeof(buf), "%3d", (цяло)mtx.ptr<schar>(row, col)[cn]); }
        void valueToStr16u() { snprintf(buf, sizeof(buf), "%d", (цяло)mtx.ptr<ushort>(row, col)[cn]); }
        void valueToStr16s() { snprintf(buf, sizeof(buf), "%d", (цяло)mtx.ptr<short>(row, col)[cn]); }
        void valueToStr32u() { snprintf(buf, sizeof(buf), "%u", mtx.ptr<unsigned>(row, col)[cn]); }
        void valueToStr32s() { snprintf(buf, sizeof(buf), "%d", mtx.ptr<цяло>(row, col)[cn]); }
        void valueToStr32f() { snprintf(buf, sizeof(buf), floatFormat, mtx.ptr<float>(row, col)[cn]); }
        void valueToStr64f() { snprintf(buf, sizeof(buf), floatFormat, mtx.ptr<double>(row, col)[cn]); }
        void valueToStr64u() { snprintf(buf, sizeof(buf), "%llu", (unsigned long long)mtx.ptr<uint64_t>(row, col)[cn]); }
        void valueToStr64s() { snprintf(buf, sizeof(buf), "%lld", (long long)mtx.ptr<int64_t>(row, col)[cn]); }
        void valueToStr16f() { snprintf(buf, sizeof(buf), floatFormat, (float)mtx.ptr<hfloat>(row, col)[cn]); }
        void valueToStr16bf() { snprintf(buf, sizeof(buf), floatFormat, (float)mtx.ptr<bfloat>(row, col)[cn]); }
        void valueToStr8f()   { snprintf(buf, sizeof(buf), floatFormat, (float)mtx.ptr<fp8_t>(row, col)[cn]); }
        void valueToStr8fnuz() { snprintf(buf, sizeof(buf), floatFormat, (float)mtx.ptr<fp8a_t>(row, col)[cn]); }
        void valueToStrOther() { buf[0] = 0; }

    public:

        FormattedImpl(String pl, String el, Mat m, char br[5], bool sLine, bool aOrder, цяло precision)
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

            ако (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            иначе
            {
                cv_snprintf(floatFormat, sizeof(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            switch(mtx.depth())
            {
                case CV_8U:  valueToStr = &FormattedImpl::valueToStr8u; break;
                case CV_8S:  valueToStr = &FormattedImpl::valueToStr8s; break;
                case CV_Bool: valueToStr = &FormattedImpl::valueToStrBool; break;
                case CV_16U: valueToStr = &FormattedImpl::valueToStr16u; break;
                case CV_16S: valueToStr = &FormattedImpl::valueToStr16s; break;
                case CV_32U: valueToStr = &FormattedImpl::valueToStr32u; break;
                case CV_32S: valueToStr = &FormattedImpl::valueToStr32s; break;
                case CV_32F: valueToStr = &FormattedImpl::valueToStr32f; break;
                case CV_64F: valueToStr = &FormattedImpl::valueToStr64f; break;
                case CV_64U: valueToStr = &FormattedImpl::valueToStr64u; break;
                case CV_64S: valueToStr = &FormattedImpl::valueToStr64s; break;
                case CV_16F: valueToStr = &FormattedImpl::valueToStr16f; break;
                case CV_16BF: valueToStr = &FormattedImpl::valueToStr16bf; break;
                case CV_8F_E4M3FN:   valueToStr = &FormattedImpl::valueToStr8f; break;
                case CV_8F_E4M3FNUZ: valueToStr = &FormattedImpl::valueToStr8fnuz; break;
                default:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.depth()));
            }
        }

        void reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        const char* next() CV_OVERRIDE
        {
            switch(state)
            {
                case STATE_PROLOGUE:
                    row = 0;
                    ако (mtx.empty())
                        state = STATE_EPILOGUE;
                    иначе ако (alignOrder)
                        state = STATE_INTERLUDE;
                    иначе
                        state = STATE_ROW_OPEN;
                    върни prologue.c_str();
                case STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    ако (row >= mtx.rows)
                    {
                        ако (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            върни buf;
                        }
                        иначе
                            row = 0;
                        snprintf(buf, sizeof(buf), "\n(:, :, %d) = \n", cn+1);
                        върни buf;
                    }
                    snprintf(buf, sizeof(buf), "(:, :, %d) = \n", cn+1);
                    върни buf;
                case STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    върни epilogue.c_str();
                case STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        ако (row > 0)
                            докато(pos < prologue.size() && pos < sizeof(buf) - 2)
                                buf[pos++] = ' ';
                        ако (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        ако(!pos)
                            върни next();
                        buf[pos] = 0;
                    }
                    върни buf;
                case STATE_ROW_CLOSE:
                    state = STATE_LINE_SEPARATOR;
                    ++row;
                    ако (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        върни buf;
                    }
                    иначе ако(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        върни buf;
                    }
                    върни next();
                case STATE_CN_OPEN:
                    state = STATE_VALUE;
                    ако (!alignOrder)
                        cn = 0;
                    ако (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        върни buf;
                    }
                    върни next();
                case STATE_CN_CLOSE:
                    ++col;
                    ако (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    иначе
                        state = STATE_CN_SEPARATOR;
                    ако (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        върни buf;
                    }
                    върни next();
                case STATE_VALUE:
                    (this->*valueToStr)();
                    state = STATE_CN_CLOSE;
                    ако (alignOrder)
                        върни buf;
                    ако (++cn < mcn)
                        state = STATE_VALUE_SEPARATOR;
                    върни buf;
                case STATE_FINISHED:
                    върни 0;
                case STATE_LINE_SEPARATOR:
                    ако (row >= mtx.rows)
                    {
                        ако (alignOrder)
                            state = STATE_INTERLUDE;
                        иначе
                            state = STATE_EPILOGUE;
                        върни next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = singleLine ? ' ' : '\n';
                    buf[1] = 0;
                    върни buf;
                case STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    върни buf;
                case STATE_VALUE_SEPARATOR:
                    state = STATE_VALUE;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    върни buf;
            }
            върни 0;
        }
    };

    class FormatterBase : public Formatter
    {
    public:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(true) {}

        void set16fPrecision(цяло p) CV_OVERRIDE
        {
            prec16f = p;
        }

        void set32fPrecision(цяло p) CV_OVERRIDE
        {
            prec32f = p;
        }

        void set64fPrecision(цяло p) CV_OVERRIDE
        {
            prec64f = p;
        }

        void setMultiline(bool ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    protected:
        цяло prec16f;
        цяло prec32f;
        цяло prec64f;
        цяло multiline;
    };

    class DefaultFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            char braces[5] = {'\0', '\0', ';', '\0', '\0'};
            върни makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    class MatlabFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            char braces[5] = {'\0', '\0', ';', '\0', '\0'};
            върни makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, true, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    class PythonFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            char braces[5] = {'[', ']', ',', '[', ']'};
            ако (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            върни makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    class NumpyFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            static const char* numpyTypes[CV_DEPTH_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            char braces[5] = {'[', ']', ',', '[', ']'};
            ако (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            върни makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.depth()]), mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    class CSVFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            char braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            върни makePtr<FormattedImpl>(String(),
                mtx.rows > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    class CFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            char braces[5] = {'\0', '\0', ',', '\0', '\0'};
            върни makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        switch(fmt)
        {
            case FMT_DEFAULT:
                върни makePtr<DefaultFormatter>();
            case FMT_MATLAB:
                върни makePtr<MatlabFormatter>();
            case FMT_CSV:
                върни makePtr<CSVFormatter>();
            case FMT_PYTHON:
                върни makePtr<PythonFormatter>();
            case FMT_NUMPY:
                върни makePtr<NumpyFormatter>();
            case FMT_C:
                върни makePtr<CFormatter>();
        }
        върни makePtr<DefaultFormatter>();
    }

    template<typename _Tp> struct Fmt
    {
        typedef цяло temp_type;
        static const char* fmt() { върни "%d"; }
    };

    template<> struct Fmt<uint32_t>
    {
        typedef unsigned temp_type;
        static const char* fmt() { върни "%u"; }
    };

    template<> struct Fmt<int64_t>
    {
        typedef long long temp_type;
        static const char* fmt() { върни "%lld"; }
    };

    template<> struct Fmt<uint64_t>
    {
        typedef unsigned long long temp_type;
        static const char* fmt() { върни "%llu"; }
    };

    template<> struct Fmt<float>
    {
        typedef float temp_type;
        static const char* fmt() { върни "%.5g"; }
    };

    template<> struct Fmt<double>
    {
        typedef double temp_type;
        static const char* fmt() { върни "%.5g"; }
    };

    template<> struct Fmt<hfloat>
    {
        typedef float temp_type;
        static const char* fmt() { върни "%.5g"; }
    };

    template<> struct Fmt<bfloat>
    {
        typedef float temp_type;
        static const char* fmt() { върни "%.4g"; }
    };

    template<> struct Fmt<fp8_t>
    {
        typedef float temp_type;
        static const char* fmt() { върни "%.4g"; }
    };

    template<> struct Fmt<fp8a_t>
    {
        typedef float temp_type;
        static const char* fmt() { върни "%.4g"; }
    };

    template <typename _Tp>
    static void pprintRow(std::ostream& strm, const _Tp* ptr, цяло n, size_t ofs, цяло edge)
    {
        char buf[128];
        const char* fmt = Fmt<_Tp>::fmt();
        цяло i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        ако (edge == 0)
            edge = ndump;
        за (i = 0; i < ndump; i++) {
            цяло j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            ако (i > 0)
                strm << ", ";
            ако (j >= 0) {
                snprintf(buf, sizeof(buf), fmt, (typename Fmt<_Tp>::temp_type)ptr[ofs + j]);
                strm << buf;
            } иначе
                strm << "... ";
        }
    }

    static void pprintSlice(std::ostream& strm, const Mat& tensor,
                            const size_t* step, цяло d,
                            size_t ofs, цяло edge)
    {
        MatShape shape = tensor.shape();
        цяло ndims = shape.dims;
        цяло n = d >= ndims ? 1 : shape[d];
        ако (d >= ndims - 1) {
            цяло typ = tensor.depth();
            void* data = tensor.data;
            CV_Assert(data);
            n *= tensor.channels();
            ако (typ == CV_8U)
                pprintRow(strm, (const uint8_t*)data, n, ofs, edge);
            иначе ако (typ == CV_8S)
                pprintRow(strm, (const int8_t*)data, n, ofs, edge);
            иначе ако (typ == CV_16U)
                pprintRow(strm, (const uint16_t*)data, n, ofs, edge);
            иначе ако (typ == CV_16S)
                pprintRow(strm, (const int16_t*)data, n, ofs, edge);
            иначе ако (typ == CV_32U)
                pprintRow(strm, (const unsigned*)data, n, ofs, edge);
            иначе ако (typ == CV_32S)
                pprintRow(strm, (const цяло*)data, n, ofs, edge);
            иначе ако (typ == CV_64U)
                pprintRow(strm, (const uint64_t*)data, n, ofs, edge);
            иначе ако (typ == CV_64S)
                pprintRow(strm, (const int64_t*)data, n, ofs, edge);
            иначе ако (typ == CV_32F)
                pprintRow(strm, (const float*)data, n, ofs, edge);
            иначе ако (typ == CV_64F)
                pprintRow(strm, (const double*)data, n, ofs, edge);
            иначе ако (typ == CV_16F)
                pprintRow(strm, (const hfloat*)data, n, ofs, edge);
            иначе ако (typ == CV_16BF)
                pprintRow(strm, (const bfloat*)data, n, ofs, edge);
            иначе ако (typ == CV_Bool)
                pprintRow(strm, (const bool*)data, n, ofs, edge);
            иначе ако (typ == CV_8F_E4M3FN)
                pprintRow(strm, (const fp8_t*)data, n, ofs, edge);
            иначе ако (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (const fp8a_t*)data, n, ofs, edge);
            иначе {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } иначе {
            цяло i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            bool dots = false;
            за (i = 0; i < ndump; i++) {
                ако (i > 0 && !dots) {
                    цяло nempty_lines = ndims - 2 - d;
                    за (цяло k = 0; k < nempty_lines; k++)
                        strm << "\n";
                }
                ако (i > 0)
                    strm << "\n";
                цяло j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                ако (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                иначе
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, InputArray array,
                         цяло /*indent*/, цяло edge_,
                         цяло wholeTensorThreshold,
                         char parens)
    {
        char oparen = parens;
        char cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        цяло edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        ако (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat temp;
            tensor.copyTo(temp);
            tensor = temp;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.total();

        ако (parens)
            strm << oparen;
        ако (sz_all == 0) {
            ако (!parens)
                strm << "<empty>";
        } иначе {
            ако (sz_all <= (size_t)wholeTensorThreshold)
                edge = 0;

            цяло ndims = shape.dims;
            цяло cn = tensor.channels();
            size_t step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            за (цяло i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        ако (parens)
            strm << cparen;
        върни strm;
    }

} // cv
