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

#অন্তর্ভাব "precomp.hpp"

নামাকাশ cv
{
    শ্রেণী FormattedImpl CV_FINAL : সর্বজনীন Formatted
    {
        গণনা { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_মান, STATE_FINISHED,
               STATE_সারি_SEPARATOR, STATE_CN_SEPARATOR, STATE_মান_SEPARATOR };
        গণনা {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        অক্ষর floatFormat[8];
        অক্ষর buf[32];   // enough for double with precision up to 20

        Mat mtx;
        পূর্ণসংখ্যা mcn; // == mtx.channels()
        যৌক্তিক single_সারি;
        যৌক্তিক alignOrder;    // true when cn first order

        পূর্ণসংখ্যা state;
        পূর্ণসংখ্যা row;
        পূর্ণসংখ্যা col;
        পূর্ণসংখ্যা cn;

        String prologue;
        String epilogue;
        অক্ষর braces[5];

        শূন্য (FormattedImpl::*মান_To_Str)();
        শূন্য মান_To_Str_Bool() { snprintf(buf, আকার(buf), "%d", (পূর্ণসংখ্যা)mtx.ptr<uchar>(row, col)[cn] != 0); }
        শূন্য মান_To_Str8u()  { snprintf(buf, আকার(buf), "%3d", (পূর্ণসংখ্যা)mtx.ptr<uchar>(row, col)[cn]); }
        শূন্য মান_To_Str8s()  { snprintf(buf, আকার(buf), "%3d", (পূর্ণসংখ্যা)mtx.ptr<schar>(row, col)[cn]); }
        শূন্য মান_To_Str16u() { snprintf(buf, আকার(buf), "%d", (পূর্ণসংখ্যা)mtx.ptr<ushort>(row, col)[cn]); }
        শূন্য মান_To_Str16s() { snprintf(buf, আকার(buf), "%d", (পূর্ণসংখ্যা)mtx.ptr<short>(row, col)[cn]); }
        শূন্য মান_To_Str32u() { snprintf(buf, আকার(buf), "%u", mtx.ptr<unsigned>(row, col)[cn]); }
        শূন্য মান_To_Str32s() { snprintf(buf, আকার(buf), "%d", mtx.ptr<পূর্ণসংখ্যা>(row, col)[cn]); }
        শূন্য মান_To_Str32f() { snprintf(buf, আকার(buf), floatFormat, mtx.ptr<দশমিক>(row, col)[cn]); }
        শূন্য মান_To_Str64f() { snprintf(buf, আকার(buf), floatFormat, mtx.ptr<দ্বিগুণ>(row, col)[cn]); }
        শূন্য মান_To_Str64u() { snprintf(buf, আকার(buf), "%llu", (unsigned long long)mtx.ptr<uint64_t>(row, col)[cn]); }
        শূন্য মান_To_Str64s() { snprintf(buf, আকার(buf), "%lld", (long long)mtx.ptr<int64_t>(row, col)[cn]); }
        শূন্য মান_To_Str16f() { snprintf(buf, আকার(buf), floatFormat, (দশমিক)mtx.ptr<hfloat>(row, col)[cn]); }
        শূন্য মান_To_Str16bf() { snprintf(buf, আকার(buf), floatFormat, (দশমিক)mtx.ptr<bfloat>(row, col)[cn]); }
        শূন্য মান_To_Str8f()   { snprintf(buf, আকার(buf), floatFormat, (দশমিক)mtx.ptr<fp8_t>(row, col)[cn]); }
        শূন্য মান_To_Str8fnuz() { snprintf(buf, আকার(buf), floatFormat, (দশমিক)mtx.ptr<fp8a_t>(row, col)[cn]); }
        শূন্য মান_To_Str_Other() { buf[0] = 0; }

    সর্বজনীন:

        FormattedImpl(String pl, String el, Mat m, অক্ষর br[5], যৌক্তিক s_সারি, যৌক্তিক aOrder, পূর্ণসংখ্যা precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            memcpy(braces, br, 5);
            state = STATE_PROLOGUE;
            single_সারি = s_সারি;
            alignOrder = aOrder;
            row = col = cn =0;

            যদি (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            অন্যথা
            {
                cv_snprintf(floatFormat, আকার(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            নির্বাচন(mtx.depth())
            {
                ক্ষেত্র CV_8U:  মান_To_Str = &FormattedImpl::মান_To_Str8u; বিরতি;
                ক্ষেত্র CV_8S:  মান_To_Str = &FormattedImpl::মান_To_Str8s; বিরতি;
                ক্ষেত্র CV_Bool: মান_To_Str = &FormattedImpl::মান_To_Str_Bool; বিরতি;
                ক্ষেত্র CV_16U: মান_To_Str = &FormattedImpl::মান_To_Str16u; বিরতি;
                ক্ষেত্র CV_16S: মান_To_Str = &FormattedImpl::মান_To_Str16s; বিরতি;
                ক্ষেত্র CV_32U: মান_To_Str = &FormattedImpl::মান_To_Str32u; বিরতি;
                ক্ষেত্র CV_32S: মান_To_Str = &FormattedImpl::মান_To_Str32s; বিরতি;
                ক্ষেত্র CV_32F: মান_To_Str = &FormattedImpl::মান_To_Str32f; বিরতি;
                ক্ষেত্র CV_64F: মান_To_Str = &FormattedImpl::মান_To_Str64f; বিরতি;
                ক্ষেত্র CV_64U: মান_To_Str = &FormattedImpl::মান_To_Str64u; বিরতি;
                ক্ষেত্র CV_64S: মান_To_Str = &FormattedImpl::মান_To_Str64s; বিরতি;
                ক্ষেত্র CV_16F: মান_To_Str = &FormattedImpl::মান_To_Str16f; বিরতি;
                ক্ষেত্র CV_16BF: মান_To_Str = &FormattedImpl::মান_To_Str16bf; বিরতি;
                ক্ষেত্র CV_8F_E4M3FN:   মান_To_Str = &FormattedImpl::মান_To_Str8f; বিরতি;
                ক্ষেত্র CV_8F_E4M3FNUZ: মান_To_Str = &FormattedImpl::মান_To_Str8fnuz; বিরতি;
                পূর্বনির্ধারিত:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.depth()));
            }
        }

        শূন্য reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        ধ্রুবক অক্ষর* next() CV_OVERRIDE
        {
            নির্বাচন(state)
            {
                ক্ষেত্র STATE_PROLOGUE:
                    row = 0;
                    যদি (mtx.empty())
                        state = STATE_EPILOGUE;
                    অন্যথা যদি (alignOrder)
                        state = STATE_INTERLUDE;
                    অন্যথা
                        state = STATE_ROW_OPEN;
                    ফেরত prologue.c_str();
                ক্ষেত্র STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    যদি (row >= mtx.rows)
                    {
                        যদি (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            ফেরত buf;
                        }
                        অন্যথা
                            row = 0;
                        snprintf(buf, আকার(buf), "\n(:, :, %d) = \n", cn+1);
                        ফেরত buf;
                    }
                    snprintf(buf, আকার(buf), "(:, :, %d) = \n", cn+1);
                    ফেরত buf;
                ক্ষেত্র STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    ফেরত epilogue.c_str();
                ক্ষেত্র STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        যদি (row > 0)
                            যখন(pos < prologue.size() && pos < আকার(buf) - 2)
                                buf[pos++] = ' ';
                        যদি (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        যদি(!pos)
                            ফেরত next();
                        buf[pos] = 0;
                    }
                    ফেরত buf;
                ক্ষেত্র STATE_ROW_CLOSE:
                    state = STATE_সারি_SEPARATOR;
                    ++row;
                    যদি (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        ফেরত buf;
                    }
                    অন্যথা যদি(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        ফেরত buf;
                    }
                    ফেরত next();
                ক্ষেত্র STATE_CN_OPEN:
                    state = STATE_মান;
                    যদি (!alignOrder)
                        cn = 0;
                    যদি (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        ফেরত buf;
                    }
                    ফেরত next();
                ক্ষেত্র STATE_CN_CLOSE:
                    ++col;
                    যদি (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    অন্যথা
                        state = STATE_CN_SEPARATOR;
                    যদি (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        ফেরত buf;
                    }
                    ফেরত next();
                ক্ষেত্র STATE_মান:
                    (এই->*মান_To_Str)();
                    state = STATE_CN_CLOSE;
                    যদি (alignOrder)
                        ফেরত buf;
                    যদি (++cn < mcn)
                        state = STATE_মান_SEPARATOR;
                    ফেরত buf;
                ক্ষেত্র STATE_FINISHED:
                    ফেরত 0;
                ক্ষেত্র STATE_সারি_SEPARATOR:
                    যদি (row >= mtx.rows)
                    {
                        যদি (alignOrder)
                            state = STATE_INTERLUDE;
                        অন্যথা
                            state = STATE_EPILOGUE;
                        ফেরত next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = single_সারি ? ' ' : '\n';
                    buf[1] = 0;
                    ফেরত buf;
                ক্ষেত্র STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    ফেরত buf;
                ক্ষেত্র STATE_মান_SEPARATOR:
                    state = STATE_মান;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    ফেরত buf;
            }
            ফেরত 0;
        }
    };

    শ্রেণী FormatterBase : সর্বজনীন Formatter
    {
    সর্বজনীন:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(সত্য) {}

        শূন্য set16fPrecision(পূর্ণসংখ্যা p) CV_OVERRIDE
        {
            prec16f = p;
        }

        শূন্য set32fPrecision(পূর্ণসংখ্যা p) CV_OVERRIDE
        {
            prec32f = p;
        }

        শূন্য set64fPrecision(পূর্ণসংখ্যা p) CV_OVERRIDE
        {
            prec64f = p;
        }

        শূন্য setMultiline(যৌক্তিক ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    সুরক্ষিত:
        পূর্ণসংখ্যা prec16f;
        পূর্ণসংখ্যা prec32f;
        পূর্ণসংখ্যা prec64f;
        পূর্ণসংখ্যা multiline;
    };

    শ্রেণী DefaultFormatter CV_FINAL : সর্বজনীন FormatterBase
    {
    সর্বজনীন:

        Ptr<Formatted> format(ধ্রুবক Mat& mtx) ধ্রুবক CV_OVERRIDE
        {
            অক্ষর braces[5] = {'\0', '\0', ';', '\0', '\0'};
            ফেরত makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, মিথ্যা, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    শ্রেণী MatlabFormatter CV_FINAL : সর্বজনীন FormatterBase
    {
    সর্বজনীন:

        Ptr<Formatted> format(ধ্রুবক Mat& mtx) ধ্রুবক CV_OVERRIDE
        {
            অক্ষর braces[5] = {'\0', '\0', ';', '\0', '\0'};
            ফেরত makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, সত্য, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    শ্রেণী PythonFormatter CV_FINAL : সর্বজনীন FormatterBase
    {
    সর্বজনীন:

        Ptr<Formatted> format(ধ্রুবক Mat& mtx) ধ্রুবক CV_OVERRIDE
        {
            অক্ষর braces[5] = {'[', ']', ',', '[', ']'};
            যদি (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            ফেরত makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, মিথ্যা, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    শ্রেণী NumpyFormatter CV_FINAL : সর্বজনীন FormatterBase
    {
    সর্বজনীন:

        Ptr<Formatted> format(ধ্রুবক Mat& mtx) ধ্রুবক CV_OVERRIDE
        {
            স্থির ধ্রুবক অক্ষর* numpyTypes[CV_DEPTH_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            অক্ষর braces[5] = {'[', ']', ',', '[', ']'};
            যদি (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            ফেরত makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.depth()]), mtx, &*braces,
                mtx.rows == 1 || !multiline, মিথ্যা, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    শ্রেণী CSVFormatter CV_FINAL : সর্বজনীন FormatterBase
    {
    সর্বজনীন:

        Ptr<Formatted> format(ধ্রুবক Mat& mtx) ধ্রুবক CV_OVERRIDE
        {
            অক্ষর braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            ফেরত makePtr<FormattedImpl>(String(),
                mtx.rows > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.rows == 1 || !multiline, মিথ্যা, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    শ্রেণী CFormatter CV_FINAL : সর্বজনীন FormatterBase
    {
    সর্বজনীন:

        Ptr<Formatted> format(ধ্রুবক Mat& mtx) ধ্রুবক CV_OVERRIDE
        {
            অক্ষর braces[5] = {'\0', '\0', ',', '\0', '\0'};
            ফেরত makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.rows == 1 || !multiline, মিথ্যা, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        নির্বাচন(fmt)
        {
            ক্ষেত্র FMT_DEFAULT:
                ফেরত makePtr<DefaultFormatter>();
            ক্ষেত্র FMT_MATLAB:
                ফেরত makePtr<MatlabFormatter>();
            ক্ষেত্র FMT_CSV:
                ফেরত makePtr<CSVFormatter>();
            ক্ষেত্র FMT_PYTHON:
                ফেরত makePtr<PythonFormatter>();
            ক্ষেত্র FMT_NUMPY:
                ফেরত makePtr<NumpyFormatter>();
            ক্ষেত্র FMT_C:
                ফেরত makePtr<CFormatter>();
        }
        ফেরত makePtr<DefaultFormatter>();
    }

    template<typename _Tp> কাঠামো Fmt
    {
        প্রকারলেখ পূর্ণসংখ্যা অস্থায়ী_type;
        স্থির ধ্রুবক অক্ষর* fmt() { ফেরত "%d"; }
    };

    template<> কাঠামো Fmt<uint32_t>
    {
        প্রকারলেখ unsigned অস্থায়ী_type;
        স্থির ধ্রুবক অক্ষর* fmt() { ফেরত "%u"; }
    };

    template<> কাঠামো Fmt<int64_t>
    {
        প্রকারলেখ long long অস্থায়ী_type;
        স্থির ধ্রুবক অক্ষর* fmt() { ফেরত "%lld"; }
    };

    template<> কাঠামো Fmt<uint64_t>
    {
        প্রকারলেখ unsigned long long অস্থায়ী_type;
        স্থির ধ্রুবক অক্ষর* fmt() { ফেরত "%llu"; }
    };

    template<> কাঠামো Fmt<দশমিক>
    {
        প্রকারলেখ দশমিক অস্থায়ী_type;
        স্থির ধ্রুবক অক্ষর* fmt() { ফেরত "%.5g"; }
    };

    template<> কাঠামো Fmt<দ্বিগুণ>
    {
        প্রকারলেখ দ্বিগুণ অস্থায়ী_type;
        স্থির ধ্রুবক অক্ষর* fmt() { ফেরত "%.5g"; }
    };

    template<> কাঠামো Fmt<hfloat>
    {
        প্রকারলেখ দশমিক অস্থায়ী_type;
        স্থির ধ্রুবক অক্ষর* fmt() { ফেরত "%.5g"; }
    };

    template<> কাঠামো Fmt<bfloat>
    {
        প্রকারলেখ দশমিক অস্থায়ী_type;
        স্থির ধ্রুবক অক্ষর* fmt() { ফেরত "%.4g"; }
    };

    template<> কাঠামো Fmt<fp8_t>
    {
        প্রকারলেখ দশমিক অস্থায়ী_type;
        স্থির ধ্রুবক অক্ষর* fmt() { ফেরত "%.4g"; }
    };

    template<> কাঠামো Fmt<fp8a_t>
    {
        প্রকারলেখ দশমিক অস্থায়ী_type;
        স্থির ধ্রুবক অক্ষর* fmt() { ফেরত "%.4g"; }
    };

    template <typename _Tp>
    স্থির শূন্য pprintRow(std::ostream& strm, ধ্রুবক _Tp* ptr, পূর্ণসংখ্যা n, size_t ofs, পূর্ণসংখ্যা edge)
    {
        অক্ষর buf[128];
        ধ্রুবক অক্ষর* fmt = Fmt<_Tp>::fmt();
        পূর্ণসংখ্যা i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        যদি (edge == 0)
            edge = ndump;
        জন্য (i = 0; i < ndump; i++) {
            পূর্ণসংখ্যা j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            যদি (i > 0)
                strm << ", ";
            যদি (j >= 0) {
                snprintf(buf, আকার(buf), fmt, (typename Fmt<_Tp>::অস্থায়ী_type)ptr[ofs + j]);
                strm << buf;
            } অন্যথা
                strm << "... ";
        }
    }

    স্থির শূন্য pprintSlice(std::ostream& strm, ধ্রুবক Mat& tensor,
                            ধ্রুবক size_t* step, পূর্ণসংখ্যা d,
                            size_t ofs, পূর্ণসংখ্যা edge)
    {
        MatShape shape = tensor.shape();
        পূর্ণসংখ্যা ndims = shape.dims;
        পূর্ণসংখ্যা n = d >= ndims ? 1 : shape[d];
        যদি (d >= ndims - 1) {
            পূর্ণসংখ্যা typ = tensor.depth();
            শূন্য* তথ্য = tensor.তথ্য;
            CV_Assert(তথ্য);
            n *= tensor.channels();
            যদি (typ == CV_8U)
                pprintRow(strm, (ধ্রুবক uint8_t*)তথ্য, n, ofs, edge);
            অন্যথা যদি (typ == CV_8S)
                pprintRow(strm, (ধ্রুবক int8_t*)তথ্য, n, ofs, edge);
            অন্যথা যদি (typ == CV_16U)
                pprintRow(strm, (ধ্রুবক uint16_t*)তথ্য, n, ofs, edge);
            অন্যথা যদি (typ == CV_16S)
                pprintRow(strm, (ধ্রুবক int16_t*)তথ্য, n, ofs, edge);
            অন্যথা যদি (typ == CV_32U)
                pprintRow(strm, (ধ্রুবক unsigned*)তথ্য, n, ofs, edge);
            অন্যথা যদি (typ == CV_32S)
                pprintRow(strm, (ধ্রুবক পূর্ণসংখ্যা*)তথ্য, n, ofs, edge);
            অন্যথা যদি (typ == CV_64U)
                pprintRow(strm, (ধ্রুবক uint64_t*)তথ্য, n, ofs, edge);
            অন্যথা যদি (typ == CV_64S)
                pprintRow(strm, (ধ্রুবক int64_t*)তথ্য, n, ofs, edge);
            অন্যথা যদি (typ == CV_32F)
                pprintRow(strm, (ধ্রুবক দশমিক*)তথ্য, n, ofs, edge);
            অন্যথা যদি (typ == CV_64F)
                pprintRow(strm, (ধ্রুবক দ্বিগুণ*)তথ্য, n, ofs, edge);
            অন্যথা যদি (typ == CV_16F)
                pprintRow(strm, (ধ্রুবক hfloat*)তথ্য, n, ofs, edge);
            অন্যথা যদি (typ == CV_16BF)
                pprintRow(strm, (ধ্রুবক bfloat*)তথ্য, n, ofs, edge);
            অন্যথা যদি (typ == CV_Bool)
                pprintRow(strm, (ধ্রুবক যৌক্তিক*)তথ্য, n, ofs, edge);
            অন্যথা যদি (typ == CV_8F_E4M3FN)
                pprintRow(strm, (ধ্রুবক fp8_t*)তথ্য, n, ofs, edge);
            অন্যথা যদি (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (ধ্রুবক fp8a_t*)তথ্য, n, ofs, edge);
            অন্যথা {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } অন্যথা {
            পূর্ণসংখ্যা i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            যৌক্তিক dots = মিথ্যা;
            জন্য (i = 0; i < ndump; i++) {
                যদি (i > 0 && !dots) {
                    পূর্ণসংখ্যা nempty_সারিগুলি = ndims - 2 - d;
                    জন্য (পূর্ণসংখ্যা k = 0; k < nempty_সারিগুলি; k++)
                        strm << "\n";
                }
                যদি (i > 0)
                    strm << "\n";
                পূর্ণসংখ্যা j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                যদি (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                অন্যথা
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, প্রবেশ_Array array,
                         পূর্ণসংখ্যা /*indent*/, পূর্ণসংখ্যা edge_,
                         পূর্ণসংখ্যা wholeTensorThreshold,
                         অক্ষর parens)
    {
        অক্ষর oparen = parens;
        অক্ষর cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        পূর্ণসংখ্যা edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        যদি (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat অস্থায়ী_2;
            tensor.copyTo(অস্থায়ী_2);
            tensor = অস্থায়ী_2;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.যোগ();

        যদি (parens)
            strm << oparen;
        যদি (sz_all == 0) {
            যদি (!parens)
                strm << "<empty>";
        } অন্যথা {
            যদি (sz_all <= (size_t)wholeTensorThreshold)
                edge = 0;

            পূর্ণসংখ্যা ndims = shape.dims;
            পূর্ণসংখ্যা cn = tensor.channels();
            size_t step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            জন্য (পূর্ণসংখ্যা i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        যদি (parens)
            strm << cparen;
        ফেরত strm;
    }

} // cv
