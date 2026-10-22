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
    বর্গ FormattedImpl CV_FINAL : সার্বজনিক Formatted
    {
        গণনা { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_মান, STATE_FINISHED,
               STATE_সারি_SEPARATOR, STATE_CN_SEPARATOR, STATE_মান_SEPARATOR };
        গণনা {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        বর্ণ floatFormat[8];
        বর্ণ buf[32];   // enough for double with precision up to 20

        Mat mtx;
        পূর্ণাঙ্ক mcn; // == mtx.channels()
        তার্কিক single_সারি;
        তার্কিক alignOrder;    // true when cn first order

        পূর্ণাঙ্ক state;
        পূর্ণাঙ্ক row;
        পূর্ণাঙ্ক col;
        পূর্ণাঙ্ক cn;

        String prologue;
        String epilogue;
        বর্ণ braces[5];

        শূন্য (FormattedImpl::*মান_To_Str)();
        শূন্য মান_To_Str_Bool() { snprintf(buf, পরিমাণ(buf), "%d", (পূর্ণাঙ্ক)mtx.ptr<uchar>(row, col)[cn] != 0); }
        শূন্য মান_To_Str8u()  { snprintf(buf, পরিমাণ(buf), "%3d", (পূর্ণাঙ্ক)mtx.ptr<uchar>(row, col)[cn]); }
        শূন্য মান_To_Str8s()  { snprintf(buf, পরিমাণ(buf), "%3d", (পূর্ণাঙ্ক)mtx.ptr<schar>(row, col)[cn]); }
        শূন্য মান_To_Str16u() { snprintf(buf, পরিমাণ(buf), "%d", (পূর্ণাঙ্ক)mtx.ptr<ushort>(row, col)[cn]); }
        শূন্য মান_To_Str16s() { snprintf(buf, পরিমাণ(buf), "%d", (পূর্ণাঙ্ক)mtx.ptr<short>(row, col)[cn]); }
        শূন্য মান_To_Str32u() { snprintf(buf, পরিমাণ(buf), "%u", mtx.ptr<unsigned>(row, col)[cn]); }
        শূন্য মান_To_Str32s() { snprintf(buf, পরিমাণ(buf), "%d", mtx.ptr<পূর্ণাঙ্ক>(row, col)[cn]); }
        শূন্য মান_To_Str32f() { snprintf(buf, পরিমাণ(buf), floatFormat, mtx.ptr<ভিন্ন>(row, col)[cn]); }
        শূন্য মান_To_Str64f() { snprintf(buf, পরিমাণ(buf), floatFormat, mtx.ptr<দ্বিগুণ>(row, col)[cn]); }
        শূন্য মান_To_Str64u() { snprintf(buf, পরিমাণ(buf), "%llu", (unsigned long long)mtx.ptr<uint64_t>(row, col)[cn]); }
        শূন্য মান_To_Str64s() { snprintf(buf, পরিমাণ(buf), "%lld", (long long)mtx.ptr<int64_t>(row, col)[cn]); }
        শূন্য মান_To_Str16f() { snprintf(buf, পরিমাণ(buf), floatFormat, (ভিন্ন)mtx.ptr<hfloat>(row, col)[cn]); }
        শূন্য মান_To_Str16bf() { snprintf(buf, পরিমাণ(buf), floatFormat, (ভিন্ন)mtx.ptr<bfloat>(row, col)[cn]); }
        শূন্য মান_To_Str8f()   { snprintf(buf, পরিমাণ(buf), floatFormat, (ভিন্ন)mtx.ptr<fp8_t>(row, col)[cn]); }
        শূন্য মান_To_Str8fnuz() { snprintf(buf, পরিমাণ(buf), floatFormat, (ভিন্ন)mtx.ptr<fp8a_t>(row, col)[cn]); }
        শূন্য মান_To_Str_Other() { buf[0] = 0; }

    সার্বজনিক:

        FormattedImpl(String pl, String el, Mat m, বর্ণ br[5], তার্কিক s_সারি, তার্কিক aOrder, পূর্ণাঙ্ক precision)
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
                cv_snprintf(floatFormat, পরিমাণ(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            বিকল্প(mtx.depth())
            {
                পক্ষ CV_8U:  মান_To_Str = &FormattedImpl::মান_To_Str8u; ভঙ্গ;
                পক্ষ CV_8S:  মান_To_Str = &FormattedImpl::মান_To_Str8s; ভঙ্গ;
                পক্ষ CV_Bool: মান_To_Str = &FormattedImpl::মান_To_Str_Bool; ভঙ্গ;
                পক্ষ CV_16U: মান_To_Str = &FormattedImpl::মান_To_Str16u; ভঙ্গ;
                পক্ষ CV_16S: মান_To_Str = &FormattedImpl::মান_To_Str16s; ভঙ্গ;
                পক্ষ CV_32U: মান_To_Str = &FormattedImpl::মান_To_Str32u; ভঙ্গ;
                পক্ষ CV_32S: মান_To_Str = &FormattedImpl::মান_To_Str32s; ভঙ্গ;
                পক্ষ CV_32F: মান_To_Str = &FormattedImpl::মান_To_Str32f; ভঙ্গ;
                পক্ষ CV_64F: মান_To_Str = &FormattedImpl::মান_To_Str64f; ভঙ্গ;
                পক্ষ CV_64U: মান_To_Str = &FormattedImpl::মান_To_Str64u; ভঙ্গ;
                পক্ষ CV_64S: মান_To_Str = &FormattedImpl::মান_To_Str64s; ভঙ্গ;
                পক্ষ CV_16F: মান_To_Str = &FormattedImpl::মান_To_Str16f; ভঙ্গ;
                পক্ষ CV_16BF: মান_To_Str = &FormattedImpl::মান_To_Str16bf; ভঙ্গ;
                পক্ষ CV_8F_E4M3FN:   মান_To_Str = &FormattedImpl::মান_To_Str8f; ভঙ্গ;
                পক্ষ CV_8F_E4M3FNUZ: মান_To_Str = &FormattedImpl::মান_To_Str8fnuz; ভঙ্গ;
                স্বভাব:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.depth()));
            }
        }

        শূন্য reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        ধ্রুব বর্ণ* next() CV_OVERRIDE
        {
            বিকল্প(state)
            {
                পক্ষ STATE_PROLOGUE:
                    row = 0;
                    যদি (mtx.empty())
                        state = STATE_EPILOGUE;
                    অন্যথা যদি (alignOrder)
                        state = STATE_INTERLUDE;
                    অন্যথা
                        state = STATE_ROW_OPEN;
                    প্রত্যাগম prologue.c_str();
                পক্ষ STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    যদি (row >= mtx.rows)
                    {
                        যদি (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            প্রত্যাগম buf;
                        }
                        অন্যথা
                            row = 0;
                        snprintf(buf, পরিমাণ(buf), "\n(:, :, %d) = \n", cn+1);
                        প্রত্যাগম buf;
                    }
                    snprintf(buf, পরিমাণ(buf), "(:, :, %d) = \n", cn+1);
                    প্রত্যাগম buf;
                পক্ষ STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    প্রত্যাগম epilogue.c_str();
                পক্ষ STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        যদি (row > 0)
                            যাবত্(pos < prologue.size() && pos < পরিমাণ(buf) - 2)
                                buf[pos++] = ' ';
                        যদি (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        যদি(!pos)
                            প্রত্যাগম next();
                        buf[pos] = 0;
                    }
                    প্রত্যাগম buf;
                পক্ষ STATE_ROW_CLOSE:
                    state = STATE_সারি_SEPARATOR;
                    ++row;
                    যদি (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        প্রত্যাগম buf;
                    }
                    অন্যথা যদি(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        প্রত্যাগম buf;
                    }
                    প্রত্যাগম next();
                পক্ষ STATE_CN_OPEN:
                    state = STATE_মান;
                    যদি (!alignOrder)
                        cn = 0;
                    যদি (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        প্রত্যাগম buf;
                    }
                    প্রত্যাগম next();
                পক্ষ STATE_CN_CLOSE:
                    ++col;
                    যদি (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    অন্যথা
                        state = STATE_CN_SEPARATOR;
                    যদি (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        প্রত্যাগম buf;
                    }
                    প্রত্যাগম next();
                পক্ষ STATE_মান:
                    (এতত্->*মান_To_Str)();
                    state = STATE_CN_CLOSE;
                    যদি (alignOrder)
                        প্রত্যাগম buf;
                    যদি (++cn < mcn)
                        state = STATE_মান_SEPARATOR;
                    প্রত্যাগম buf;
                পক্ষ STATE_FINISHED:
                    প্রত্যাগম 0;
                পক্ষ STATE_সারি_SEPARATOR:
                    যদি (row >= mtx.rows)
                    {
                        যদি (alignOrder)
                            state = STATE_INTERLUDE;
                        অন্যথা
                            state = STATE_EPILOGUE;
                        প্রত্যাগম next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = single_সারি ? ' ' : '\n';
                    buf[1] = 0;
                    প্রত্যাগম buf;
                পক্ষ STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    প্রত্যাগম buf;
                পক্ষ STATE_মান_SEPARATOR:
                    state = STATE_মান;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    প্রত্যাগম buf;
            }
            প্রত্যাগম 0;
        }
    };

    বর্গ FormatterBase : সার্বজনিক Formatter
    {
    সার্বজনিক:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(সত্য) {}

        শূন্য set16fPrecision(পূর্ণাঙ্ক p) CV_OVERRIDE
        {
            prec16f = p;
        }

        শূন্য set32fPrecision(পূর্ণাঙ্ক p) CV_OVERRIDE
        {
            prec32f = p;
        }

        শূন্য set64fPrecision(পূর্ণাঙ্ক p) CV_OVERRIDE
        {
            prec64f = p;
        }

        শূন্য setMultiline(তার্কিক ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    রক্ষিত:
        পূর্ণাঙ্ক prec16f;
        পূর্ণাঙ্ক prec32f;
        পূর্ণাঙ্ক prec64f;
        পূর্ণাঙ্ক multiline;
    };

    বর্গ DefaultFormatter CV_FINAL : সার্বজনিক FormatterBase
    {
    সার্বজনিক:

        Ptr<Formatted> format(ধ্রুব Mat& mtx) ধ্রুব CV_OVERRIDE
        {
            বর্ণ braces[5] = {'\0', '\0', ';', '\0', '\0'};
            প্রত্যাগম makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, অসত্য, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    বর্গ MatlabFormatter CV_FINAL : সার্বজনিক FormatterBase
    {
    সার্বজনিক:

        Ptr<Formatted> format(ধ্রুব Mat& mtx) ধ্রুব CV_OVERRIDE
        {
            বর্ণ braces[5] = {'\0', '\0', ';', '\0', '\0'};
            প্রত্যাগম makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, সত্য, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    বর্গ PythonFormatter CV_FINAL : সার্বজনিক FormatterBase
    {
    সার্বজনিক:

        Ptr<Formatted> format(ধ্রুব Mat& mtx) ধ্রুব CV_OVERRIDE
        {
            বর্ণ braces[5] = {'[', ']', ',', '[', ']'};
            যদি (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            প্রত্যাগম makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, অসত্য, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    বর্গ NumpyFormatter CV_FINAL : সার্বজনিক FormatterBase
    {
    সার্বজনিক:

        Ptr<Formatted> format(ধ্রুব Mat& mtx) ধ্রুব CV_OVERRIDE
        {
            স্থির ধ্রুব বর্ণ* numpyTypes[CV_DEPTH_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            বর্ণ braces[5] = {'[', ']', ',', '[', ']'};
            যদি (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            প্রত্যাগম makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.depth()]), mtx, &*braces,
                mtx.rows == 1 || !multiline, অসত্য, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    বর্গ CSVFormatter CV_FINAL : সার্বজনিক FormatterBase
    {
    সার্বজনিক:

        Ptr<Formatted> format(ধ্রুব Mat& mtx) ধ্রুব CV_OVERRIDE
        {
            বর্ণ braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            প্রত্যাগম makePtr<FormattedImpl>(String(),
                mtx.rows > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.rows == 1 || !multiline, অসত্য, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    বর্গ CFormatter CV_FINAL : সার্বজনিক FormatterBase
    {
    সার্বজনিক:

        Ptr<Formatted> format(ধ্রুব Mat& mtx) ধ্রুব CV_OVERRIDE
        {
            বর্ণ braces[5] = {'\0', '\0', ',', '\0', '\0'};
            প্রত্যাগম makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.rows == 1 || !multiline, অসত্য, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        বিকল্প(fmt)
        {
            পক্ষ FMT_DEFAULT:
                প্রত্যাগম makePtr<DefaultFormatter>();
            পক্ষ FMT_MATLAB:
                প্রত্যাগম makePtr<MatlabFormatter>();
            পক্ষ FMT_CSV:
                প্রত্যাগম makePtr<CSVFormatter>();
            পক্ষ FMT_PYTHON:
                প্রত্যাগম makePtr<PythonFormatter>();
            পক্ষ FMT_NUMPY:
                প্রত্যাগম makePtr<NumpyFormatter>();
            পক্ষ FMT_C:
                প্রত্যাগম makePtr<CFormatter>();
        }
        প্রত্যাগম makePtr<DefaultFormatter>();
    }

    template<typename _Tp> সংরচনা Fmt
    {
        প্রকারনির্দেশ পূর্ণাঙ্ক temp_type;
        স্থির ধ্রুব বর্ণ* fmt() { প্রত্যাগম "%d"; }
    };

    template<> সংরচনা Fmt<uint32_t>
    {
        প্রকারনির্দেশ unsigned temp_type;
        স্থির ধ্রুব বর্ণ* fmt() { প্রত্যাগম "%u"; }
    };

    template<> সংরচনা Fmt<int64_t>
    {
        প্রকারনির্দেশ long long temp_type;
        স্থির ধ্রুব বর্ণ* fmt() { প্রত্যাগম "%lld"; }
    };

    template<> সংরচনা Fmt<uint64_t>
    {
        প্রকারনির্দেশ unsigned long long temp_type;
        স্থির ধ্রুব বর্ণ* fmt() { প্রত্যাগম "%llu"; }
    };

    template<> সংরচনা Fmt<ভিন্ন>
    {
        প্রকারনির্দেশ ভিন্ন temp_type;
        স্থির ধ্রুব বর্ণ* fmt() { প্রত্যাগম "%.5g"; }
    };

    template<> সংরচনা Fmt<দ্বিগুণ>
    {
        প্রকারনির্দেশ দ্বিগুণ temp_type;
        স্থির ধ্রুব বর্ণ* fmt() { প্রত্যাগম "%.5g"; }
    };

    template<> সংরচনা Fmt<hfloat>
    {
        প্রকারনির্দেশ ভিন্ন temp_type;
        স্থির ধ্রুব বর্ণ* fmt() { প্রত্যাগম "%.5g"; }
    };

    template<> সংরচনা Fmt<bfloat>
    {
        প্রকারনির্দেশ ভিন্ন temp_type;
        স্থির ধ্রুব বর্ণ* fmt() { প্রত্যাগম "%.4g"; }
    };

    template<> সংরচনা Fmt<fp8_t>
    {
        প্রকারনির্দেশ ভিন্ন temp_type;
        স্থির ধ্রুব বর্ণ* fmt() { প্রত্যাগম "%.4g"; }
    };

    template<> সংরচনা Fmt<fp8a_t>
    {
        প্রকারনির্দেশ ভিন্ন temp_type;
        স্থির ধ্রুব বর্ণ* fmt() { প্রত্যাগম "%.4g"; }
    };

    template <typename _Tp>
    স্থির শূন্য pprintRow(std::ostream& strm, ধ্রুব _Tp* ptr, পূর্ণাঙ্ক n, size_t ofs, পূর্ণাঙ্ক edge)
    {
        বর্ণ buf[128];
        ধ্রুব বর্ণ* fmt = Fmt<_Tp>::fmt();
        পূর্ণাঙ্ক i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        যদি (edge == 0)
            edge = ndump;
        প্রতি (i = 0; i < ndump; i++) {
            পূর্ণাঙ্ক j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            যদি (i > 0)
                strm << ", ";
            যদি (j >= 0) {
                snprintf(buf, পরিমাণ(buf), fmt, (typename Fmt<_Tp>::temp_type)ptr[ofs + j]);
                strm << buf;
            } অন্যথা
                strm << "... ";
        }
    }

    স্থির শূন্য pprintSlice(std::ostream& strm, ধ্রুব Mat& tensor,
                            ধ্রুব size_t* step, পূর্ণাঙ্ক d,
                            size_t ofs, পূর্ণাঙ্ক edge)
    {
        MatShape shape = tensor.shape();
        পূর্ণাঙ্ক ndims = shape.dims;
        পূর্ণাঙ্ক n = d >= ndims ? 1 : shape[d];
        যদি (d >= ndims - 1) {
            পূর্ণাঙ্ক typ = tensor.depth();
            শূন্য* data = tensor.data;
            CV_Assert(data);
            n *= tensor.channels();
            যদি (typ == CV_8U)
                pprintRow(strm, (ধ্রুব uint8_t*)data, n, ofs, edge);
            অন্যথা যদি (typ == CV_8S)
                pprintRow(strm, (ধ্রুব int8_t*)data, n, ofs, edge);
            অন্যথা যদি (typ == CV_16U)
                pprintRow(strm, (ধ্রুব uint16_t*)data, n, ofs, edge);
            অন্যথা যদি (typ == CV_16S)
                pprintRow(strm, (ধ্রুব int16_t*)data, n, ofs, edge);
            অন্যথা যদি (typ == CV_32U)
                pprintRow(strm, (ধ্রুব unsigned*)data, n, ofs, edge);
            অন্যথা যদি (typ == CV_32S)
                pprintRow(strm, (ধ্রুব পূর্ণাঙ্ক*)data, n, ofs, edge);
            অন্যথা যদি (typ == CV_64U)
                pprintRow(strm, (ধ্রুব uint64_t*)data, n, ofs, edge);
            অন্যথা যদি (typ == CV_64S)
                pprintRow(strm, (ধ্রুব int64_t*)data, n, ofs, edge);
            অন্যথা যদি (typ == CV_32F)
                pprintRow(strm, (ধ্রুব ভিন্ন*)data, n, ofs, edge);
            অন্যথা যদি (typ == CV_64F)
                pprintRow(strm, (ধ্রুব দ্বিগুণ*)data, n, ofs, edge);
            অন্যথা যদি (typ == CV_16F)
                pprintRow(strm, (ধ্রুব hfloat*)data, n, ofs, edge);
            অন্যথা যদি (typ == CV_16BF)
                pprintRow(strm, (ধ্রুব bfloat*)data, n, ofs, edge);
            অন্যথা যদি (typ == CV_Bool)
                pprintRow(strm, (ধ্রুব তার্কিক*)data, n, ofs, edge);
            অন্যথা যদি (typ == CV_8F_E4M3FN)
                pprintRow(strm, (ধ্রুব fp8_t*)data, n, ofs, edge);
            অন্যথা যদি (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (ধ্রুব fp8a_t*)data, n, ofs, edge);
            অন্যথা {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } অন্যথা {
            পূর্ণাঙ্ক i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            তার্কিক dots = অসত্য;
            প্রতি (i = 0; i < ndump; i++) {
                যদি (i > 0 && !dots) {
                    পূর্ণাঙ্ক nempty_সারিগুলি = ndims - 2 - d;
                    প্রতি (পূর্ণাঙ্ক k = 0; k < nempty_সারিগুলি; k++)
                        strm << "\n";
                }
                যদি (i > 0)
                    strm << "\n";
                পূর্ণাঙ্ক j = n == ndump || i < edge ? i :
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
                         পূর্ণাঙ্ক /*indent*/, পূর্ণাঙ্ক edge_,
                         পূর্ণাঙ্ক wholeTensorThreshold,
                         বর্ণ parens)
    {
        বর্ণ oparen = parens;
        বর্ণ cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        পূর্ণাঙ্ক edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        যদি (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat temp;
            tensor.copyTo(temp);
            tensor = temp;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.total();

        যদি (parens)
            strm << oparen;
        যদি (sz_all == 0) {
            যদি (!parens)
                strm << "<empty>";
        } অন্যথা {
            যদি (sz_all <= (size_t)wholeTensorThreshold)
                edge = 0;

            পূর্ণাঙ্ক ndims = shape.dims;
            পূর্ণাঙ্ক cn = tensor.channels();
            size_t step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            প্রতি (পূর্ণাঙ্ক i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        যদি (parens)
            strm << cparen;
        প্রত্যাগম strm;
    }

} // cv
