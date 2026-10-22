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
        ціле mcn; // == mtx.channels()
        bool singleLine;
        bool alignOrder;    // true when cn first order

        ціле state;
        ціле row;
        ціле col;
        ціле cn;

        String prologue;
        String epilogue;
        char braces[5];

        void (FormattedImpl::*valueToStr)();
        void valueToStrBool() { snprintf(buf, sizeof(buf), "%d", (ціле)mtx.ptr<uchar>(row, col)[cn] != 0); }
        void valueToStr8u()  { snprintf(buf, sizeof(buf), "%3d", (ціле)mtx.ptr<uchar>(row, col)[cn]); }
        void valueToStr8s()  { snprintf(buf, sizeof(buf), "%3d", (ціле)mtx.ptr<schar>(row, col)[cn]); }
        void valueToStr16u() { snprintf(buf, sizeof(buf), "%d", (ціле)mtx.ptr<ushort>(row, col)[cn]); }
        void valueToStr16s() { snprintf(buf, sizeof(buf), "%d", (ціле)mtx.ptr<short>(row, col)[cn]); }
        void valueToStr32u() { snprintf(buf, sizeof(buf), "%u", mtx.ptr<unsigned>(row, col)[cn]); }
        void valueToStr32s() { snprintf(buf, sizeof(buf), "%d", mtx.ptr<ціле>(row, col)[cn]); }
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

        FormattedImpl(String pl, String el, Mat m, char br[5], bool sLine, bool aOrder, ціле precision)
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

            якщо (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            інакше
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
                    якщо (mtx.empty())
                        state = STATE_EPILOGUE;
                    інакше якщо (alignOrder)
                        state = STATE_INTERLUDE;
                    інакше
                        state = STATE_ROW_OPEN;
                    повернути prologue.c_str();
                case STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    якщо (row >= mtx.rows)
                    {
                        якщо (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            повернути buf;
                        }
                        інакше
                            row = 0;
                        snprintf(buf, sizeof(buf), "\n(:, :, %d) = \n", cn+1);
                        повернути buf;
                    }
                    snprintf(buf, sizeof(buf), "(:, :, %d) = \n", cn+1);
                    повернути buf;
                case STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    повернути epilogue.c_str();
                case STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        якщо (row > 0)
                            поки(pos < prologue.size() && pos < sizeof(buf) - 2)
                                buf[pos++] = ' ';
                        якщо (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        якщо(!pos)
                            повернути next();
                        buf[pos] = 0;
                    }
                    повернути buf;
                case STATE_ROW_CLOSE:
                    state = STATE_LINE_SEPARATOR;
                    ++row;
                    якщо (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        повернути buf;
                    }
                    інакше якщо(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        повернути buf;
                    }
                    повернути next();
                case STATE_CN_OPEN:
                    state = STATE_VALUE;
                    якщо (!alignOrder)
                        cn = 0;
                    якщо (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        повернути buf;
                    }
                    повернути next();
                case STATE_CN_CLOSE:
                    ++col;
                    якщо (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    інакше
                        state = STATE_CN_SEPARATOR;
                    якщо (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        повернути buf;
                    }
                    повернути next();
                case STATE_VALUE:
                    (this->*valueToStr)();
                    state = STATE_CN_CLOSE;
                    якщо (alignOrder)
                        повернути buf;
                    якщо (++cn < mcn)
                        state = STATE_VALUE_SEPARATOR;
                    повернути buf;
                case STATE_FINISHED:
                    повернути 0;
                case STATE_LINE_SEPARATOR:
                    якщо (row >= mtx.rows)
                    {
                        якщо (alignOrder)
                            state = STATE_INTERLUDE;
                        інакше
                            state = STATE_EPILOGUE;
                        повернути next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = singleLine ? ' ' : '\n';
                    buf[1] = 0;
                    повернути buf;
                case STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    повернути buf;
                case STATE_VALUE_SEPARATOR:
                    state = STATE_VALUE;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    повернути buf;
            }
            повернути 0;
        }
    };

    class FormatterBase : public Formatter
    {
    public:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(true) {}

        void set16fPrecision(ціле p) CV_OVERRIDE
        {
            prec16f = p;
        }

        void set32fPrecision(ціле p) CV_OVERRIDE
        {
            prec32f = p;
        }

        void set64fPrecision(ціле p) CV_OVERRIDE
        {
            prec64f = p;
        }

        void setMultiline(bool ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    protected:
        ціле prec16f;
        ціле prec32f;
        ціле prec64f;
        ціле multiline;
    };

    class DefaultFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            char braces[5] = {'\0', '\0', ';', '\0', '\0'};
            повернути makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, false, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    class MatlabFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            char braces[5] = {'\0', '\0', ';', '\0', '\0'};
            повернути makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, true, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    class PythonFormatter CV_FINAL : public FormatterBase
    {
    public:

        Ptr<Formatted> format(const Mat& mtx) const CV_OVERRIDE
        {
            char braces[5] = {'[', ']', ',', '[', ']'};
            якщо (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            повернути makePtr<FormattedImpl>("[", "]", mtx, &*braces,
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
            якщо (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            повернути makePtr<FormattedImpl>("array([",
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
            повернути makePtr<FormattedImpl>(String(),
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
            повернути makePtr<FormattedImpl>("{", "}", mtx, &*braces,
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
                повернути makePtr<DefaultFormatter>();
            case FMT_MATLAB:
                повернути makePtr<MatlabFormatter>();
            case FMT_CSV:
                повернути makePtr<CSVFormatter>();
            case FMT_PYTHON:
                повернути makePtr<PythonFormatter>();
            case FMT_NUMPY:
                повернути makePtr<NumpyFormatter>();
            case FMT_C:
                повернути makePtr<CFormatter>();
        }
        повернути makePtr<DefaultFormatter>();
    }

    template<typename _Tp> struct Fmt
    {
        typedef ціле temp_type;
        static const char* fmt() { повернути "%d"; }
    };

    template<> struct Fmt<uint32_t>
    {
        typedef unsigned temp_type;
        static const char* fmt() { повернути "%u"; }
    };

    template<> struct Fmt<int64_t>
    {
        typedef long long temp_type;
        static const char* fmt() { повернути "%lld"; }
    };

    template<> struct Fmt<uint64_t>
    {
        typedef unsigned long long temp_type;
        static const char* fmt() { повернути "%llu"; }
    };

    template<> struct Fmt<float>
    {
        typedef float temp_type;
        static const char* fmt() { повернути "%.5g"; }
    };

    template<> struct Fmt<double>
    {
        typedef double temp_type;
        static const char* fmt() { повернути "%.5g"; }
    };

    template<> struct Fmt<hfloat>
    {
        typedef float temp_type;
        static const char* fmt() { повернути "%.5g"; }
    };

    template<> struct Fmt<bfloat>
    {
        typedef float temp_type;
        static const char* fmt() { повернути "%.4g"; }
    };

    template<> struct Fmt<fp8_t>
    {
        typedef float temp_type;
        static const char* fmt() { повернути "%.4g"; }
    };

    template<> struct Fmt<fp8a_t>
    {
        typedef float temp_type;
        static const char* fmt() { повернути "%.4g"; }
    };

    template <typename _Tp>
    static void pprintRow(std::ostream& strm, const _Tp* ptr, ціле n, size_t ofs, ціле edge)
    {
        char buf[128];
        const char* fmt = Fmt<_Tp>::fmt();
        ціле i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        якщо (edge == 0)
            edge = ndump;
        для (i = 0; i < ndump; i++) {
            ціле j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            якщо (i > 0)
                strm << ", ";
            якщо (j >= 0) {
                snprintf(buf, sizeof(buf), fmt, (typename Fmt<_Tp>::temp_type)ptr[ofs + j]);
                strm << buf;
            } інакше
                strm << "... ";
        }
    }

    static void pprintSlice(std::ostream& strm, const Mat& tensor,
                            const size_t* step, ціле d,
                            size_t ofs, ціле edge)
    {
        MatShape shape = tensor.shape();
        ціле ndims = shape.dims;
        ціле n = d >= ndims ? 1 : shape[d];
        якщо (d >= ndims - 1) {
            ціле typ = tensor.depth();
            void* data = tensor.data;
            CV_Assert(data);
            n *= tensor.channels();
            якщо (typ == CV_8U)
                pprintRow(strm, (const uint8_t*)data, n, ofs, edge);
            інакше якщо (typ == CV_8S)
                pprintRow(strm, (const int8_t*)data, n, ofs, edge);
            інакше якщо (typ == CV_16U)
                pprintRow(strm, (const uint16_t*)data, n, ofs, edge);
            інакше якщо (typ == CV_16S)
                pprintRow(strm, (const int16_t*)data, n, ofs, edge);
            інакше якщо (typ == CV_32U)
                pprintRow(strm, (const unsigned*)data, n, ofs, edge);
            інакше якщо (typ == CV_32S)
                pprintRow(strm, (const ціле*)data, n, ofs, edge);
            інакше якщо (typ == CV_64U)
                pprintRow(strm, (const uint64_t*)data, n, ofs, edge);
            інакше якщо (typ == CV_64S)
                pprintRow(strm, (const int64_t*)data, n, ofs, edge);
            інакше якщо (typ == CV_32F)
                pprintRow(strm, (const float*)data, n, ofs, edge);
            інакше якщо (typ == CV_64F)
                pprintRow(strm, (const double*)data, n, ofs, edge);
            інакше якщо (typ == CV_16F)
                pprintRow(strm, (const hfloat*)data, n, ofs, edge);
            інакше якщо (typ == CV_16BF)
                pprintRow(strm, (const bfloat*)data, n, ofs, edge);
            інакше якщо (typ == CV_Bool)
                pprintRow(strm, (const bool*)data, n, ofs, edge);
            інакше якщо (typ == CV_8F_E4M3FN)
                pprintRow(strm, (const fp8_t*)data, n, ofs, edge);
            інакше якщо (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (const fp8a_t*)data, n, ofs, edge);
            інакше {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } інакше {
            ціле i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            bool dots = false;
            для (i = 0; i < ndump; i++) {
                якщо (i > 0 && !dots) {
                    ціле nempty_lines = ndims - 2 - d;
                    для (ціле k = 0; k < nempty_lines; k++)
                        strm << "\n";
                }
                якщо (i > 0)
                    strm << "\n";
                ціле j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                якщо (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                інакше
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, InputArray array,
                         ціле /*indent*/, ціле edge_,
                         ціле wholeTensorThreshold,
                         char parens)
    {
        char oparen = parens;
        char cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        ціле edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        якщо (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat temp;
            tensor.copyTo(temp);
            tensor = temp;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.total();

        якщо (parens)
            strm << oparen;
        якщо (sz_all == 0) {
            якщо (!parens)
                strm << "<empty>";
        } інакше {
            якщо (sz_all <= (size_t)wholeTensorThreshold)
                edge = 0;

            ціле ndims = shape.dims;
            ціле cn = tensor.channels();
            size_t step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            для (ціле i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        якщо (parens)
            strm << cparen;
        повернути strm;
    }

} // cv
