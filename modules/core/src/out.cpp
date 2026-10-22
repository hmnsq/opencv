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

#شامل "precomp.hpp"

فضاینام cv
{
    رده FormattedImpl CV_FINAL : عمومی Formatted
    {
        شمارش { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_مقدار, STATE_FINISHED,
               STATE_سطر_SEPARATOR, STATE_CN_SEPARATOR, STATE_مقدار_SEPARATOR };
        شمارش {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        نویسه floatFormat[8];
        نویسه buf[32];   // enough for double with precision up to 20

        Mat mtx;
        صحیح mcn; // == mtx.channels()
        منطقی single_سطر;
        منطقی alignOrder;    // true when cn first order

        صحیح state;
        صحیح row;
        صحیح col;
        صحیح cn;

        String prologue;
        String epilogue;
        نویسه braces[5];

        تهی (FormattedImpl::*مقدار_To_Str)();
        تهی مقدار_To_Str_Bool() { snprintf(buf, اندازه(buf), "%d", (صحیح)mtx.ptr<uchar>(row, col)[cn] != 0); }
        تهی مقدار_To_Str8u()  { snprintf(buf, اندازه(buf), "%3d", (صحیح)mtx.ptr<uchar>(row, col)[cn]); }
        تهی مقدار_To_Str8s()  { snprintf(buf, اندازه(buf), "%3d", (صحیح)mtx.ptr<schar>(row, col)[cn]); }
        تهی مقدار_To_Str16u() { snprintf(buf, اندازه(buf), "%d", (صحیح)mtx.ptr<ushort>(row, col)[cn]); }
        تهی مقدار_To_Str16s() { snprintf(buf, اندازه(buf), "%d", (صحیح)mtx.ptr<short>(row, col)[cn]); }
        تهی مقدار_To_Str32u() { snprintf(buf, اندازه(buf), "%u", mtx.ptr<unsigned>(row, col)[cn]); }
        تهی مقدار_To_Str32s() { snprintf(buf, اندازه(buf), "%d", mtx.ptr<صحیح>(row, col)[cn]); }
        تهی مقدار_To_Str32f() { snprintf(buf, اندازه(buf), floatFormat, mtx.ptr<اعشاری>(row, col)[cn]); }
        تهی مقدار_To_Str64f() { snprintf(buf, اندازه(buf), floatFormat, mtx.ptr<دوگانه>(row, col)[cn]); }
        تهی مقدار_To_Str64u() { snprintf(buf, اندازه(buf), "%llu", (unsigned long long)mtx.ptr<uint64_t>(row, col)[cn]); }
        تهی مقدار_To_Str64s() { snprintf(buf, اندازه(buf), "%lld", (long long)mtx.ptr<int64_t>(row, col)[cn]); }
        تهی مقدار_To_Str16f() { snprintf(buf, اندازه(buf), floatFormat, (اعشاری)mtx.ptr<hfloat>(row, col)[cn]); }
        تهی مقدار_To_Str16bf() { snprintf(buf, اندازه(buf), floatFormat, (اعشاری)mtx.ptr<bfloat>(row, col)[cn]); }
        تهی مقدار_To_Str8f()   { snprintf(buf, اندازه(buf), floatFormat, (اعشاری)mtx.ptr<fp8_t>(row, col)[cn]); }
        تهی مقدار_To_Str8fnuz() { snprintf(buf, اندازه(buf), floatFormat, (اعشاری)mtx.ptr<fp8a_t>(row, col)[cn]); }
        تهی مقدار_To_Str_Other() { buf[0] = 0; }

    عمومی:

        FormattedImpl(String pl, String el, Mat m, نویسه br[5], منطقی s_سطر, منطقی aOrder, صحیح precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            memcpy(braces, br, 5);
            state = STATE_PROLOGUE;
            single_سطر = s_سطر;
            alignOrder = aOrder;
            row = col = cn =0;

            اگر (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            وگرنه
            {
                cv_snprintf(floatFormat, اندازه(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            انتخاب(mtx.depth())
            {
                حالت CV_8U:  مقدار_To_Str = &FormattedImpl::مقدار_To_Str8u; بشکن;
                حالت CV_8S:  مقدار_To_Str = &FormattedImpl::مقدار_To_Str8s; بشکن;
                حالت CV_Bool: مقدار_To_Str = &FormattedImpl::مقدار_To_Str_Bool; بشکن;
                حالت CV_16U: مقدار_To_Str = &FormattedImpl::مقدار_To_Str16u; بشکن;
                حالت CV_16S: مقدار_To_Str = &FormattedImpl::مقدار_To_Str16s; بشکن;
                حالت CV_32U: مقدار_To_Str = &FormattedImpl::مقدار_To_Str32u; بشکن;
                حالت CV_32S: مقدار_To_Str = &FormattedImpl::مقدار_To_Str32s; بشکن;
                حالت CV_32F: مقدار_To_Str = &FormattedImpl::مقدار_To_Str32f; بشکن;
                حالت CV_64F: مقدار_To_Str = &FormattedImpl::مقدار_To_Str64f; بشکن;
                حالت CV_64U: مقدار_To_Str = &FormattedImpl::مقدار_To_Str64u; بشکن;
                حالت CV_64S: مقدار_To_Str = &FormattedImpl::مقدار_To_Str64s; بشکن;
                حالت CV_16F: مقدار_To_Str = &FormattedImpl::مقدار_To_Str16f; بشکن;
                حالت CV_16BF: مقدار_To_Str = &FormattedImpl::مقدار_To_Str16bf; بشکن;
                حالت CV_8F_E4M3FN:   مقدار_To_Str = &FormattedImpl::مقدار_To_Str8f; بشکن;
                حالت CV_8F_E4M3FNUZ: مقدار_To_Str = &FormattedImpl::مقدار_To_Str8fnuz; بشکن;
                پیشفرض:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.depth()));
            }
        }

        تهی reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        ثابت نویسه* next() CV_OVERRIDE
        {
            انتخاب(state)
            {
                حالت STATE_PROLOGUE:
                    row = 0;
                    اگر (mtx.empty())
                        state = STATE_EPILOGUE;
                    وگرنه اگر (alignOrder)
                        state = STATE_INTERLUDE;
                    وگرنه
                        state = STATE_ROW_OPEN;
                    بازگشت prologue.c_str();
                حالت STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    اگر (row >= mtx.rows)
                    {
                        اگر (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            بازگشت buf;
                        }
                        وگرنه
                            row = 0;
                        snprintf(buf, اندازه(buf), "\n(:, :, %d) = \n", cn+1);
                        بازگشت buf;
                    }
                    snprintf(buf, اندازه(buf), "(:, :, %d) = \n", cn+1);
                    بازگشت buf;
                حالت STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    بازگشت epilogue.c_str();
                حالت STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        اگر (row > 0)
                            وقتی(pos < prologue.size() && pos < اندازه(buf) - 2)
                                buf[pos++] = ' ';
                        اگر (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        اگر(!pos)
                            بازگشت next();
                        buf[pos] = 0;
                    }
                    بازگشت buf;
                حالت STATE_ROW_CLOSE:
                    state = STATE_سطر_SEPARATOR;
                    ++row;
                    اگر (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        بازگشت buf;
                    }
                    وگرنه اگر(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        بازگشت buf;
                    }
                    بازگشت next();
                حالت STATE_CN_OPEN:
                    state = STATE_مقدار;
                    اگر (!alignOrder)
                        cn = 0;
                    اگر (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        بازگشت buf;
                    }
                    بازگشت next();
                حالت STATE_CN_CLOSE:
                    ++col;
                    اگر (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    وگرنه
                        state = STATE_CN_SEPARATOR;
                    اگر (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        بازگشت buf;
                    }
                    بازگشت next();
                حالت STATE_مقدار:
                    (این->*مقدار_To_Str)();
                    state = STATE_CN_CLOSE;
                    اگر (alignOrder)
                        بازگشت buf;
                    اگر (++cn < mcn)
                        state = STATE_مقدار_SEPARATOR;
                    بازگشت buf;
                حالت STATE_FINISHED:
                    بازگشت 0;
                حالت STATE_سطر_SEPARATOR:
                    اگر (row >= mtx.rows)
                    {
                        اگر (alignOrder)
                            state = STATE_INTERLUDE;
                        وگرنه
                            state = STATE_EPILOGUE;
                        بازگشت next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = single_سطر ? ' ' : '\n';
                    buf[1] = 0;
                    بازگشت buf;
                حالت STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    بازگشت buf;
                حالت STATE_مقدار_SEPARATOR:
                    state = STATE_مقدار;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    بازگشت buf;
            }
            بازگشت 0;
        }
    };

    رده FormatterBase : عمومی Formatter
    {
    عمومی:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(درست) {}

        تهی set16fPrecision(صحیح p) CV_OVERRIDE
        {
            prec16f = p;
        }

        تهی set32fPrecision(صحیح p) CV_OVERRIDE
        {
            prec32f = p;
        }

        تهی set64fPrecision(صحیح p) CV_OVERRIDE
        {
            prec64f = p;
        }

        تهی setMultiline(منطقی ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    محافظتشده:
        صحیح prec16f;
        صحیح prec32f;
        صحیح prec64f;
        صحیح multiline;
    };

    رده DefaultFormatter CV_FINAL : عمومی FormatterBase
    {
    عمومی:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            نویسه braces[5] = {'\0', '\0', ';', '\0', '\0'};
            بازگشت makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, نادرست, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    رده MatlabFormatter CV_FINAL : عمومی FormatterBase
    {
    عمومی:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            نویسه braces[5] = {'\0', '\0', ';', '\0', '\0'};
            بازگشت makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, درست, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    رده PythonFormatter CV_FINAL : عمومی FormatterBase
    {
    عمومی:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            نویسه braces[5] = {'[', ']', ',', '[', ']'};
            اگر (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            بازگشت makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, نادرست, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    رده NumpyFormatter CV_FINAL : عمومی FormatterBase
    {
    عمومی:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            ایستا ثابت نویسه* numpyTypes[CV_DEPTH_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            نویسه braces[5] = {'[', ']', ',', '[', ']'};
            اگر (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            بازگشت makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.depth()]), mtx, &*braces,
                mtx.rows == 1 || !multiline, نادرست, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    رده CSVFormatter CV_FINAL : عمومی FormatterBase
    {
    عمومی:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            نویسه braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            بازگشت makePtr<FormattedImpl>(String(),
                mtx.rows > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.rows == 1 || !multiline, نادرست, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    رده CFormatter CV_FINAL : عمومی FormatterBase
    {
    عمومی:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            نویسه braces[5] = {'\0', '\0', ',', '\0', '\0'};
            بازگشت makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.rows == 1 || !multiline, نادرست, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        انتخاب(fmt)
        {
            حالت FMT_DEFAULT:
                بازگشت makePtr<DefaultFormatter>();
            حالت FMT_MATLAB:
                بازگشت makePtr<MatlabFormatter>();
            حالت FMT_CSV:
                بازگشت makePtr<CSVFormatter>();
            حالت FMT_PYTHON:
                بازگشت makePtr<PythonFormatter>();
            حالت FMT_NUMPY:
                بازگشت makePtr<NumpyFormatter>();
            حالت FMT_C:
                بازگشت makePtr<CFormatter>();
        }
        بازگشت makePtr<DefaultFormatter>();
    }

    template<typename _Tp> ساختار Fmt
    {
        تعریفنوع صحیح temp_type;
        ایستا ثابت نویسه* fmt() { بازگشت "%d"; }
    };

    template<> ساختار Fmt<uint32_t>
    {
        تعریفنوع unsigned temp_type;
        ایستا ثابت نویسه* fmt() { بازگشت "%u"; }
    };

    template<> ساختار Fmt<int64_t>
    {
        تعریفنوع long long temp_type;
        ایستا ثابت نویسه* fmt() { بازگشت "%lld"; }
    };

    template<> ساختار Fmt<uint64_t>
    {
        تعریفنوع unsigned long long temp_type;
        ایستا ثابت نویسه* fmt() { بازگشت "%llu"; }
    };

    template<> ساختار Fmt<اعشاری>
    {
        تعریفنوع اعشاری temp_type;
        ایستا ثابت نویسه* fmt() { بازگشت "%.5g"; }
    };

    template<> ساختار Fmt<دوگانه>
    {
        تعریفنوع دوگانه temp_type;
        ایستا ثابت نویسه* fmt() { بازگشت "%.5g"; }
    };

    template<> ساختار Fmt<hfloat>
    {
        تعریفنوع اعشاری temp_type;
        ایستا ثابت نویسه* fmt() { بازگشت "%.5g"; }
    };

    template<> ساختار Fmt<bfloat>
    {
        تعریفنوع اعشاری temp_type;
        ایستا ثابت نویسه* fmt() { بازگشت "%.4g"; }
    };

    template<> ساختار Fmt<fp8_t>
    {
        تعریفنوع اعشاری temp_type;
        ایستا ثابت نویسه* fmt() { بازگشت "%.4g"; }
    };

    template<> ساختار Fmt<fp8a_t>
    {
        تعریفنوع اعشاری temp_type;
        ایستا ثابت نویسه* fmt() { بازگشت "%.4g"; }
    };

    template <typename _Tp>
    ایستا تهی pprintRow(std::ostream& strm, ثابت _Tp* ptr, صحیح n, size_t ofs, صحیح edge)
    {
        نویسه buf[128];
        ثابت نویسه* fmt = Fmt<_Tp>::fmt();
        صحیح i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        اگر (edge == 0)
            edge = ndump;
        برای (i = 0; i < ndump; i++) {
            صحیح j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            اگر (i > 0)
                strm << ", ";
            اگر (j >= 0) {
                snprintf(buf, اندازه(buf), fmt, (typename Fmt<_Tp>::temp_type)ptr[ofs + j]);
                strm << buf;
            } وگرنه
                strm << "... ";
        }
    }

    ایستا تهی pprintSlice(std::ostream& strm, ثابت Mat& tensor,
                            ثابت size_t* step, صحیح d,
                            size_t ofs, صحیح edge)
    {
        MatShape shape = tensor.shape();
        صحیح ndims = shape.dims;
        صحیح n = d >= ndims ? 1 : shape[d];
        اگر (d >= ndims - 1) {
            صحیح typ = tensor.depth();
            تهی* data = tensor.data;
            CV_Assert(data);
            n *= tensor.channels();
            اگر (typ == CV_8U)
                pprintRow(strm, (ثابت uint8_t*)data, n, ofs, edge);
            وگرنه اگر (typ == CV_8S)
                pprintRow(strm, (ثابت int8_t*)data, n, ofs, edge);
            وگرنه اگر (typ == CV_16U)
                pprintRow(strm, (ثابت uint16_t*)data, n, ofs, edge);
            وگرنه اگر (typ == CV_16S)
                pprintRow(strm, (ثابت int16_t*)data, n, ofs, edge);
            وگرنه اگر (typ == CV_32U)
                pprintRow(strm, (ثابت unsigned*)data, n, ofs, edge);
            وگرنه اگر (typ == CV_32S)
                pprintRow(strm, (ثابت صحیح*)data, n, ofs, edge);
            وگرنه اگر (typ == CV_64U)
                pprintRow(strm, (ثابت uint64_t*)data, n, ofs, edge);
            وگرنه اگر (typ == CV_64S)
                pprintRow(strm, (ثابت int64_t*)data, n, ofs, edge);
            وگرنه اگر (typ == CV_32F)
                pprintRow(strm, (ثابت اعشاری*)data, n, ofs, edge);
            وگرنه اگر (typ == CV_64F)
                pprintRow(strm, (ثابت دوگانه*)data, n, ofs, edge);
            وگرنه اگر (typ == CV_16F)
                pprintRow(strm, (ثابت hfloat*)data, n, ofs, edge);
            وگرنه اگر (typ == CV_16BF)
                pprintRow(strm, (ثابت bfloat*)data, n, ofs, edge);
            وگرنه اگر (typ == CV_Bool)
                pprintRow(strm, (ثابت منطقی*)data, n, ofs, edge);
            وگرنه اگر (typ == CV_8F_E4M3FN)
                pprintRow(strm, (ثابت fp8_t*)data, n, ofs, edge);
            وگرنه اگر (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (ثابت fp8a_t*)data, n, ofs, edge);
            وگرنه {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } وگرنه {
            صحیح i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            منطقی dots = نادرست;
            برای (i = 0; i < ndump; i++) {
                اگر (i > 0 && !dots) {
                    صحیح nempty_سطرها = ndims - 2 - d;
                    برای (صحیح k = 0; k < nempty_سطرها; k++)
                        strm << "\n";
                }
                اگر (i > 0)
                    strm << "\n";
                صحیح j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                اگر (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                وگرنه
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, درون_داد_Array array,
                         صحیح /*indent*/, صحیح edge_,
                         صحیح wholeTensorThreshold,
                         نویسه parens)
    {
        نویسه oparen = parens;
        نویسه cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        صحیح edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        اگر (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat temp;
            tensor.copyTo(temp);
            tensor = temp;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.total();

        اگر (parens)
            strm << oparen;
        اگر (sz_all == 0) {
            اگر (!parens)
                strm << "<empty>";
        } وگرنه {
            اگر (sz_all <= (size_t)wholeTensorThreshold)
                edge = 0;

            صحیح ndims = shape.dims;
            صحیح cn = tensor.channels();
            size_t step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            برای (صحیح i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        اگر (parens)
            strm << cparen;
        بازگشت strm;
    }

} // cv
