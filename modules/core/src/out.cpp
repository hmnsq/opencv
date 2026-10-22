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

#ضمن "precomp.hpp"

نطاق cv
{
    صنف FormattedImpl CV_FINAL : عام Formatted
    {
        تعداد { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_قيمة, STATE_FINISHED,
               STATE_سطر_SEPARATOR, STATE_CN_SEPARATOR, STATE_قيمة_SEPARATOR };
        تعداد {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        حرف floatFormat[8];
        حرف buf[32];   // enough for double with precision up to 20

        Mat mtx;
        صحيح mcn; // == mtx.channels()
        منطقي single_سطر;
        منطقي alignOrder;    // true when cn first order

        صحيح state;
        صحيح row;
        صحيح col;
        صحيح cn;

        String prologue;
        String epilogue;
        حرف braces[5];

        فراغ (FormattedImpl::*قيمة_To_Str)();
        فراغ قيمة_To_Str_Bool() { snprintf(buf, حجم(buf), "%d", (صحيح)mtx.ptr<uchar>(row, col)[cn] != 0); }
        فراغ قيمة_To_Str8u()  { snprintf(buf, حجم(buf), "%3d", (صحيح)mtx.ptr<uchar>(row, col)[cn]); }
        فراغ قيمة_To_Str8s()  { snprintf(buf, حجم(buf), "%3d", (صحيح)mtx.ptr<schar>(row, col)[cn]); }
        فراغ قيمة_To_Str16u() { snprintf(buf, حجم(buf), "%d", (صحيح)mtx.ptr<ushort>(row, col)[cn]); }
        فراغ قيمة_To_Str16s() { snprintf(buf, حجم(buf), "%d", (صحيح)mtx.ptr<short>(row, col)[cn]); }
        فراغ قيمة_To_Str32u() { snprintf(buf, حجم(buf), "%u", mtx.ptr<unsigned>(row, col)[cn]); }
        فراغ قيمة_To_Str32s() { snprintf(buf, حجم(buf), "%d", mtx.ptr<صحيح>(row, col)[cn]); }
        فراغ قيمة_To_Str32f() { snprintf(buf, حجم(buf), floatFormat, mtx.ptr<عائم>(row, col)[cn]); }
        فراغ قيمة_To_Str64f() { snprintf(buf, حجم(buf), floatFormat, mtx.ptr<مزدوج>(row, col)[cn]); }
        فراغ قيمة_To_Str64u() { snprintf(buf, حجم(buf), "%llu", (unsigned long long)mtx.ptr<uint64_t>(row, col)[cn]); }
        فراغ قيمة_To_Str64s() { snprintf(buf, حجم(buf), "%lld", (long long)mtx.ptr<int64_t>(row, col)[cn]); }
        فراغ قيمة_To_Str16f() { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<hfloat>(row, col)[cn]); }
        فراغ قيمة_To_Str16bf() { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<bfloat>(row, col)[cn]); }
        فراغ قيمة_To_Str8f()   { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<fp8_t>(row, col)[cn]); }
        فراغ قيمة_To_Str8fnuz() { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<fp8a_t>(row, col)[cn]); }
        فراغ قيمة_To_Str_Other() { buf[0] = 0; }

    عام:

        FormattedImpl(String pl, String el, Mat m, حرف br[5], منطقي s_سطر, منطقي aOrder, صحيح precision)
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

            إذا (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            وإلا
            {
                cv_snprintf(floatFormat, حجم(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            بدل(mtx.depth())
            {
                حالة CV_8U:  قيمة_To_Str = &FormattedImpl::قيمة_To_Str8u; اكسر;
                حالة CV_8S:  قيمة_To_Str = &FormattedImpl::قيمة_To_Str8s; اكسر;
                حالة CV_Bool: قيمة_To_Str = &FormattedImpl::قيمة_To_Str_Bool; اكسر;
                حالة CV_16U: قيمة_To_Str = &FormattedImpl::قيمة_To_Str16u; اكسر;
                حالة CV_16S: قيمة_To_Str = &FormattedImpl::قيمة_To_Str16s; اكسر;
                حالة CV_32U: قيمة_To_Str = &FormattedImpl::قيمة_To_Str32u; اكسر;
                حالة CV_32S: قيمة_To_Str = &FormattedImpl::قيمة_To_Str32s; اكسر;
                حالة CV_32F: قيمة_To_Str = &FormattedImpl::قيمة_To_Str32f; اكسر;
                حالة CV_64F: قيمة_To_Str = &FormattedImpl::قيمة_To_Str64f; اكسر;
                حالة CV_64U: قيمة_To_Str = &FormattedImpl::قيمة_To_Str64u; اكسر;
                حالة CV_64S: قيمة_To_Str = &FormattedImpl::قيمة_To_Str64s; اكسر;
                حالة CV_16F: قيمة_To_Str = &FormattedImpl::قيمة_To_Str16f; اكسر;
                حالة CV_16BF: قيمة_To_Str = &FormattedImpl::قيمة_To_Str16bf; اكسر;
                حالة CV_8F_E4M3FN:   قيمة_To_Str = &FormattedImpl::قيمة_To_Str8f; اكسر;
                حالة CV_8F_E4M3FNUZ: قيمة_To_Str = &FormattedImpl::قيمة_To_Str8fnuz; اكسر;
                افتراضي:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.depth()));
            }
        }

        فراغ reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        ثابت حرف* next() CV_OVERRIDE
        {
            بدل(state)
            {
                حالة STATE_PROLOGUE:
                    row = 0;
                    إذا (mtx.empty())
                        state = STATE_EPILOGUE;
                    وإلا إذا (alignOrder)
                        state = STATE_INTERLUDE;
                    وإلا
                        state = STATE_ROW_OPEN;
                    أرجع prologue.c_str();
                حالة STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    إذا (row >= mtx.rows)
                    {
                        إذا (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            أرجع buf;
                        }
                        وإلا
                            row = 0;
                        snprintf(buf, حجم(buf), "\n(:, :, %d) = \n", cn+1);
                        أرجع buf;
                    }
                    snprintf(buf, حجم(buf), "(:, :, %d) = \n", cn+1);
                    أرجع buf;
                حالة STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    أرجع epilogue.c_str();
                حالة STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        إذا (row > 0)
                            طالما(pos < prologue.size() && pos < حجم(buf) - 2)
                                buf[pos++] = ' ';
                        إذا (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        إذا(!pos)
                            أرجع next();
                        buf[pos] = 0;
                    }
                    أرجع buf;
                حالة STATE_ROW_CLOSE:
                    state = STATE_سطر_SEPARATOR;
                    ++row;
                    إذا (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        أرجع buf;
                    }
                    وإلا إذا(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        أرجع buf;
                    }
                    أرجع next();
                حالة STATE_CN_OPEN:
                    state = STATE_قيمة;
                    إذا (!alignOrder)
                        cn = 0;
                    إذا (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        أرجع buf;
                    }
                    أرجع next();
                حالة STATE_CN_CLOSE:
                    ++col;
                    إذا (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    وإلا
                        state = STATE_CN_SEPARATOR;
                    إذا (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        أرجع buf;
                    }
                    أرجع next();
                حالة STATE_قيمة:
                    (هذا->*قيمة_To_Str)();
                    state = STATE_CN_CLOSE;
                    إذا (alignOrder)
                        أرجع buf;
                    إذا (++cn < mcn)
                        state = STATE_قيمة_SEPARATOR;
                    أرجع buf;
                حالة STATE_FINISHED:
                    أرجع 0;
                حالة STATE_سطر_SEPARATOR:
                    إذا (row >= mtx.rows)
                    {
                        إذا (alignOrder)
                            state = STATE_INTERLUDE;
                        وإلا
                            state = STATE_EPILOGUE;
                        أرجع next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = single_سطر ? ' ' : '\n';
                    buf[1] = 0;
                    أرجع buf;
                حالة STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    أرجع buf;
                حالة STATE_قيمة_SEPARATOR:
                    state = STATE_قيمة;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    أرجع buf;
            }
            أرجع 0;
        }
    };

    صنف FormatterBase : عام Formatter
    {
    عام:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(true) {}

        فراغ set16fPrecision(صحيح p) CV_OVERRIDE
        {
            prec16f = p;
        }

        فراغ set32fPrecision(صحيح p) CV_OVERRIDE
        {
            prec32f = p;
        }

        فراغ set64fPrecision(صحيح p) CV_OVERRIDE
        {
            prec64f = p;
        }

        فراغ setMultiline(منطقي ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    محمي:
        صحيح prec16f;
        صحيح prec32f;
        صحيح prec64f;
        صحيح multiline;
    };

    صنف DefaultFormatter CV_FINAL : عام FormatterBase
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ';', '\0', '\0'};
            أرجع makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, خطأ, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف MatlabFormatter CV_FINAL : عام FormatterBase
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ';', '\0', '\0'};
            أرجع makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, true, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف PythonFormatter CV_FINAL : عام FormatterBase
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'[', ']', ',', '[', ']'};
            إذا (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            أرجع makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, خطأ, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف NumpyFormatter CV_FINAL : عام FormatterBase
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            ساكن ثابت حرف* numpyTypes[CV_DEPTH_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            حرف braces[5] = {'[', ']', ',', '[', ']'};
            إذا (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            أرجع makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.depth()]), mtx, &*braces,
                mtx.rows == 1 || !multiline, خطأ, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف CSVFormatter CV_FINAL : عام FormatterBase
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            أرجع makePtr<FormattedImpl>(String(),
                mtx.rows > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.rows == 1 || !multiline, خطأ, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف CFormatter CV_FINAL : عام FormatterBase
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ',', '\0', '\0'};
            أرجع makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.rows == 1 || !multiline, خطأ, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        بدل(fmt)
        {
            حالة FMT_DEFAULT:
                أرجع makePtr<DefaultFormatter>();
            حالة FMT_MATLAB:
                أرجع makePtr<MatlabFormatter>();
            حالة FMT_CSV:
                أرجع makePtr<CSVFormatter>();
            حالة FMT_PYTHON:
                أرجع makePtr<PythonFormatter>();
            حالة FMT_NUMPY:
                أرجع makePtr<NumpyFormatter>();
            حالة FMT_C:
                أرجع makePtr<CFormatter>();
        }
        أرجع makePtr<DefaultFormatter>();
    }

    template<typename _Tp> بنية Fmt
    {
        تعريفنوع صحيح temp_type;
        ساكن ثابت حرف* fmt() { أرجع "%d"; }
    };

    template<> بنية Fmt<uint32_t>
    {
        تعريفنوع unsigned temp_type;
        ساكن ثابت حرف* fmt() { أرجع "%u"; }
    };

    template<> بنية Fmt<int64_t>
    {
        تعريفنوع long long temp_type;
        ساكن ثابت حرف* fmt() { أرجع "%lld"; }
    };

    template<> بنية Fmt<uint64_t>
    {
        تعريفنوع unsigned long long temp_type;
        ساكن ثابت حرف* fmt() { أرجع "%llu"; }
    };

    template<> بنية Fmt<عائم>
    {
        تعريفنوع عائم temp_type;
        ساكن ثابت حرف* fmt() { أرجع "%.5g"; }
    };

    template<> بنية Fmt<مزدوج>
    {
        تعريفنوع مزدوج temp_type;
        ساكن ثابت حرف* fmt() { أرجع "%.5g"; }
    };

    template<> بنية Fmt<hfloat>
    {
        تعريفنوع عائم temp_type;
        ساكن ثابت حرف* fmt() { أرجع "%.5g"; }
    };

    template<> بنية Fmt<bfloat>
    {
        تعريفنوع عائم temp_type;
        ساكن ثابت حرف* fmt() { أرجع "%.4g"; }
    };

    template<> بنية Fmt<fp8_t>
    {
        تعريفنوع عائم temp_type;
        ساكن ثابت حرف* fmt() { أرجع "%.4g"; }
    };

    template<> بنية Fmt<fp8a_t>
    {
        تعريفنوع عائم temp_type;
        ساكن ثابت حرف* fmt() { أرجع "%.4g"; }
    };

    template <typename _Tp>
    ساكن فراغ pprintRow(std::ostream& strm, ثابت _Tp* ptr, صحيح n, size_t ofs, صحيح edge)
    {
        حرف buf[128];
        ثابت حرف* fmt = Fmt<_Tp>::fmt();
        صحيح i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        إذا (edge == 0)
            edge = ndump;
        لكل (i = 0; i < ndump; i++) {
            صحيح j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            إذا (i > 0)
                strm << ", ";
            إذا (j >= 0) {
                snprintf(buf, حجم(buf), fmt, (typename Fmt<_Tp>::temp_type)ptr[ofs + j]);
                strm << buf;
            } وإلا
                strm << "... ";
        }
    }

    ساكن فراغ pprintSlice(std::ostream& strm, ثابت Mat& tensor,
                            ثابت size_t* step, صحيح d,
                            size_t ofs, صحيح edge)
    {
        MatShape shape = tensor.shape();
        صحيح ndims = shape.dims;
        صحيح n = d >= ndims ? 1 : shape[d];
        إذا (d >= ndims - 1) {
            صحيح typ = tensor.depth();
            فراغ* data = tensor.data;
            CV_Assert(data);
            n *= tensor.channels();
            إذا (typ == CV_8U)
                pprintRow(strm, (ثابت uint8_t*)data, n, ofs, edge);
            وإلا إذا (typ == CV_8S)
                pprintRow(strm, (ثابت int8_t*)data, n, ofs, edge);
            وإلا إذا (typ == CV_16U)
                pprintRow(strm, (ثابت uint16_t*)data, n, ofs, edge);
            وإلا إذا (typ == CV_16S)
                pprintRow(strm, (ثابت int16_t*)data, n, ofs, edge);
            وإلا إذا (typ == CV_32U)
                pprintRow(strm, (ثابت unsigned*)data, n, ofs, edge);
            وإلا إذا (typ == CV_32S)
                pprintRow(strm, (ثابت صحيح*)data, n, ofs, edge);
            وإلا إذا (typ == CV_64U)
                pprintRow(strm, (ثابت uint64_t*)data, n, ofs, edge);
            وإلا إذا (typ == CV_64S)
                pprintRow(strm, (ثابت int64_t*)data, n, ofs, edge);
            وإلا إذا (typ == CV_32F)
                pprintRow(strm, (ثابت عائم*)data, n, ofs, edge);
            وإلا إذا (typ == CV_64F)
                pprintRow(strm, (ثابت مزدوج*)data, n, ofs, edge);
            وإلا إذا (typ == CV_16F)
                pprintRow(strm, (ثابت hfloat*)data, n, ofs, edge);
            وإلا إذا (typ == CV_16BF)
                pprintRow(strm, (ثابت bfloat*)data, n, ofs, edge);
            وإلا إذا (typ == CV_Bool)
                pprintRow(strm, (ثابت منطقي*)data, n, ofs, edge);
            وإلا إذا (typ == CV_8F_E4M3FN)
                pprintRow(strm, (ثابت fp8_t*)data, n, ofs, edge);
            وإلا إذا (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (ثابت fp8a_t*)data, n, ofs, edge);
            وإلا {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } وإلا {
            صحيح i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            منطقي dots = خطأ;
            لكل (i = 0; i < ndump; i++) {
                إذا (i > 0 && !dots) {
                    صحيح nempty_أسطر = ndims - 2 - d;
                    لكل (صحيح k = 0; k < nempty_أسطر; k++)
                        strm << "\n";
                }
                إذا (i > 0)
                    strm << "\n";
                صحيح j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                إذا (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                وإلا
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, مدخل_Array array,
                         صحيح /*indent*/, صحيح edge_,
                         صحيح wholeTensorThreshold,
                         حرف parens)
    {
        حرف oparen = parens;
        حرف cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        صحيح edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        إذا (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat temp;
            tensor.copyTo(temp);
            tensor = temp;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.total();

        إذا (parens)
            strm << oparen;
        إذا (sz_all == 0) {
            إذا (!parens)
                strm << "<empty>";
        } وإلا {
            إذا (sz_all <= (size_t)wholeTensorThreshold)
                edge = 0;

            صحيح ndims = shape.dims;
            صحيح cn = tensor.channels();
            size_t step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            لكل (صحيح i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        إذا (parens)
            strm << cparen;
        أرجع strm;
    }

} // cv
