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

نامخلا cv
{
    جماعت FormattedImpl CV_FINAL : عوامی Formatted
    {
        شمار { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_صف_OPEN, STATE_صف_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_قدر, STATE_ختم_شدہ,
               STATE_سطر_SEPARATOR, STATE_CN_SEPARATOR, STATE_قدر_SEPARATOR };
        شمار {BRACE_صف_OPEN = 0, BRACE_صف_CLOSE = 1, BRACE_صف_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        حرف floatFormat[8];
        حرف buf[32];   // enough for double with precision up to 20

        Mat mtx;
        صحیح mcn; // == mtx.channels()
        منطقی single_سطر;
        منطقی align_ترتیب;    // true when cn first order

        صحیح state;
        صحیح صف;
        صحیح ستون;
        صحیح cn;

        String prologue;
        String epilogue;
        حرف braces[5];

        خالی (FormattedImpl::*قدر_مقصد_Str)();
        خالی قدر_مقصد_Str_Bool() { snprintf(buf, حجم(buf), "%d", (صحیح)mtx.ptr<uchar>(صف, ستون)[cn] != 0); }
        خالی قدر_مقصد_Str8u()  { snprintf(buf, حجم(buf), "%3d", (صحیح)mtx.ptr<uchar>(صف, ستون)[cn]); }
        خالی قدر_مقصد_Str8s()  { snprintf(buf, حجم(buf), "%3d", (صحیح)mtx.ptr<schar>(صف, ستون)[cn]); }
        خالی قدر_مقصد_Str16u() { snprintf(buf, حجم(buf), "%d", (صحیح)mtx.ptr<ushort>(صف, ستون)[cn]); }
        خالی قدر_مقصد_Str16s() { snprintf(buf, حجم(buf), "%d", (صحیح)mtx.ptr<مختصر>(صف, ستون)[cn]); }
        خالی قدر_مقصد_Str32u() { snprintf(buf, حجم(buf), "%u", mtx.ptr<غیرعلامتی>(صف, ستون)[cn]); }
        خالی قدر_مقصد_Str32s() { snprintf(buf, حجم(buf), "%d", mtx.ptr<صحیح>(صف, ستون)[cn]); }
        خالی قدر_مقصد_Str32f() { snprintf(buf, حجم(buf), floatFormat, mtx.ptr<اعشاری>(صف, ستون)[cn]); }
        خالی قدر_مقصد_Str64f() { snprintf(buf, حجم(buf), floatFormat, mtx.ptr<دوہرا>(صف, ستون)[cn]); }
        خالی قدر_مقصد_Str64u() { snprintf(buf, حجم(buf), "%llu", (غیرعلامتی طویل طویل)mtx.ptr<uint64_t>(صف, ستون)[cn]); }
        خالی قدر_مقصد_Str64s() { snprintf(buf, حجم(buf), "%lld", (طویل طویل)mtx.ptr<int64_t>(صف, ستون)[cn]); }
        خالی قدر_مقصد_Str16f() { snprintf(buf, حجم(buf), floatFormat, (اعشاری)mtx.ptr<hfloat>(صف, ستون)[cn]); }
        خالی قدر_مقصد_Str16bf() { snprintf(buf, حجم(buf), floatFormat, (اعشاری)mtx.ptr<bfloat>(صف, ستون)[cn]); }
        خالی قدر_مقصد_Str8f()   { snprintf(buf, حجم(buf), floatFormat, (اعشاری)mtx.ptr<fp8_t>(صف, ستون)[cn]); }
        خالی قدر_مقصد_Str8fnuz() { snprintf(buf, حجم(buf), floatFormat, (اعشاری)mtx.ptr<fp8a_t>(صف, ستون)[cn]); }
        خالی قدر_مقصد_Str_Other() { buf[0] = 0; }

    عوامی:

        FormattedImpl(String pl, String el, Mat m, حرف br[5], منطقی s_سطر, منطقی a_ترتیب, صحیح precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            حافظہ_نقل(braces, br, 5);
            state = STATE_PROLOGUE;
            single_سطر = s_سطر;
            align_ترتیب = a_ترتیب;
            صف = ستون = cn =0;

            اگر (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            ورنہ
            {
                cv_snprintf(floatFormat, حجم(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            انتخاب(mtx.گہرائی())
            {
                حالت CV_8U:  قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str8u; توڑو;
                حالت CV_8S:  قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str8s; توڑو;
                حالت CV_Bool: قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str_Bool; توڑو;
                حالت CV_16U: قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str16u; توڑو;
                حالت CV_16S: قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str16s; توڑو;
                حالت CV_32U: قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str32u; توڑو;
                حالت CV_32S: قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str32s; توڑو;
                حالت CV_32F: قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str32f; توڑو;
                حالت CV_64F: قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str64f; توڑو;
                حالت CV_64U: قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str64u; توڑو;
                حالت CV_64S: قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str64s; توڑو;
                حالت CV_16F: قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str16f; توڑو;
                حالت CV_16BF: قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str16bf; توڑو;
                حالت CV_8F_E4M3FN:   قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str8f; توڑو;
                حالت CV_8F_E4M3FNUZ: قدر_مقصد_Str = &FormattedImpl::قدر_مقصد_Str8fnuz; توڑو;
                طےشدہ:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.گہرائی()));
            }
        }

        خالی reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        ثابت حرف* next() CV_OVERRIDE
        {
            انتخاب(state)
            {
                حالت STATE_PROLOGUE:
                    صف = 0;
                    اگر (mtx.empty())
                        state = STATE_EPILOGUE;
                    ورنہ اگر (align_ترتیب)
                        state = STATE_INTERLUDE;
                    ورنہ
                        state = STATE_صف_OPEN;
                    واپس prologue.c_str();
                حالت STATE_INTERLUDE:
                    state = STATE_صف_OPEN;
                    اگر (صف >= mtx.صفیں)
                    {
                        اگر (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            واپس buf;
                        }
                        ورنہ
                            صف = 0;
                        snprintf(buf, حجم(buf), "\n(:, :, %d) = \n", cn+1);
                        واپس buf;
                    }
                    snprintf(buf, حجم(buf), "(:, :, %d) = \n", cn+1);
                    واپس buf;
                حالت STATE_EPILOGUE:
                    state = STATE_ختم_شدہ;
                    واپس epilogue.c_str();
                حالت STATE_صف_OPEN:
                    ستون = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        اگر (صف > 0)
                            جب(pos < prologue.size() && pos < حجم(buf) - 2)
                                buf[pos++] = ' ';
                        اگر (braces[BRACE_صف_OPEN])
                            buf[pos++] = braces[BRACE_صف_OPEN];
                        اگر(!pos)
                            واپس next();
                        buf[pos] = 0;
                    }
                    واپس buf;
                حالت STATE_صف_CLOSE:
                    state = STATE_سطر_SEPARATOR;
                    ++صف;
                    اگر (braces[BRACE_صف_CLOSE])
                    {
                        buf[0] = braces[BRACE_صف_CLOSE];
                        buf[1] = صف < mtx.صفیں ? ',' : '\0';
                        buf[2] = 0;
                        واپس buf;
                    }
                    ورنہ اگر(braces[BRACE_صف_SEP] && صف < mtx.صفیں)
                    {
                        buf[0] = braces[BRACE_صف_SEP];
                        buf[1] = 0;
                        واپس buf;
                    }
                    واپس next();
                حالت STATE_CN_OPEN:
                    state = STATE_قدر;
                    اگر (!align_ترتیب)
                        cn = 0;
                    اگر (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        واپس buf;
                    }
                    واپس next();
                حالت STATE_CN_CLOSE:
                    ++ستون;
                    اگر (ستون >= mtx.ستون_تعداد)
                        state = STATE_صف_CLOSE;
                    ورنہ
                        state = STATE_CN_SEPARATOR;
                    اگر (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        واپس buf;
                    }
                    واپس next();
                حالت STATE_قدر:
                    (یہ->*قدر_مقصد_Str)();
                    state = STATE_CN_CLOSE;
                    اگر (align_ترتیب)
                        واپس buf;
                    اگر (++cn < mcn)
                        state = STATE_قدر_SEPARATOR;
                    واپس buf;
                حالت STATE_ختم_شدہ:
                    واپس 0;
                حالت STATE_سطر_SEPARATOR:
                    اگر (صف >= mtx.صفیں)
                    {
                        اگر (align_ترتیب)
                            state = STATE_INTERLUDE;
                        ورنہ
                            state = STATE_EPILOGUE;
                        واپس next();
                    }
                    state = STATE_صف_OPEN;
                    buf[0] = single_سطر ? ' ' : '\n';
                    buf[1] = 0;
                    واپس buf;
                حالت STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    واپس buf;
                حالت STATE_قدر_SEPARATOR:
                    state = STATE_قدر;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    واپس buf;
            }
            واپس 0;
        }
    };

    جماعت Formatter_بنیاد : عوامی Formatter
    {
    عوامی:
        Formatter_بنیاد() : prec16f(4), prec32f(8), prec64f(16), multiline(سچ) {}

        خالی set16fPrecision(صحیح p) CV_OVERRIDE
        {
            prec16f = p;
        }

        خالی set32fPrecision(صحیح p) CV_OVERRIDE
        {
            prec32f = p;
        }

        خالی set64fPrecision(صحیح p) CV_OVERRIDE
        {
            prec64f = p;
        }

        خالی سیٹ_Multiline(منطقی ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    محفوظ:
        صحیح prec16f;
        صحیح prec32f;
        صحیح prec64f;
        صحیح multiline;
    };

    جماعت DefaultFormatter CV_FINAL : عوامی Formatter_بنیاد
    {
    عوامی:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ';', '\0', '\0'};
            واپس makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.صفیں == 1 || !multiline, جھوٹ, mtx.گہرائی() == CV_64F ? prec64f : prec32f );
        }
    };

    جماعت MatlabFormatter CV_FINAL : عوامی Formatter_بنیاد
    {
    عوامی:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ';', '\0', '\0'};
            واپس makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.صفیں == 1 || !multiline, سچ, mtx.گہرائی() == CV_64F ? prec64f : prec32f );
        }
    };

    جماعت PythonFormatter CV_FINAL : عوامی Formatter_بنیاد
    {
    عوامی:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'[', ']', ',', '[', ']'};
            اگر (mtx.ستون_تعداد == 1)
                braces[0] = braces[1] = '\0';
            واپس makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.صفیں == 1 || !multiline, جھوٹ, mtx.گہرائی() == CV_64F ? prec64f : prec32f );
        }
    };

    جماعت NumpyFormatter CV_FINAL : عوامی Formatter_بنیاد
    {
    عوامی:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            ساکن ثابت حرف* numpyTypes[CV_گہرائی_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            حرف braces[5] = {'[', ']', ',', '[', ']'};
            اگر (mtx.ستون_تعداد == 1)
                braces[0] = braces[1] = '\0';
            واپس makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.گہرائی()]), mtx, &*braces,
                mtx.صفیں == 1 || !multiline, جھوٹ, mtx.گہرائی() == CV_64F ? prec64f : prec32f );
        }
    };

    جماعت CSVFormatter CV_FINAL : عوامی Formatter_بنیاد
    {
    عوامی:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            واپس makePtr<FormattedImpl>(String(),
                mtx.صفیں > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.صفیں == 1 || !multiline, جھوٹ, mtx.گہرائی() == CV_64F ? prec64f : prec32f );
        }
    };

    جماعت CFormatter CV_FINAL : عوامی Formatter_بنیاد
    {
    عوامی:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ',', '\0', '\0'};
            واپس makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.صفیں == 1 || !multiline, جھوٹ, mtx.گہرائی() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        انتخاب(fmt)
        {
            حالت FMT_DEFAULT:
                واپس makePtr<DefaultFormatter>();
            حالت FMT_MATLAB:
                واپس makePtr<MatlabFormatter>();
            حالت FMT_CSV:
                واپس makePtr<CSVFormatter>();
            حالت FMT_PYTHON:
                واپس makePtr<PythonFormatter>();
            حالت FMT_NUMPY:
                واپس makePtr<NumpyFormatter>();
            حالت FMT_C:
                واپس makePtr<CFormatter>();
        }
        واپس makePtr<DefaultFormatter>();
    }

    سانچہ<قسم_نام _Tp> ساخت Fmt
    {
        قسمتعریف صحیح عارضی_type;
        ساکن ثابت حرف* fmt() { واپس "%d"; }
    };

    سانچہ<> ساخت Fmt<uint32_t>
    {
        قسمتعریف غیرعلامتی عارضی_type;
        ساکن ثابت حرف* fmt() { واپس "%u"; }
    };

    سانچہ<> ساخت Fmt<int64_t>
    {
        قسمتعریف طویل طویل عارضی_type;
        ساکن ثابت حرف* fmt() { واپس "%lld"; }
    };

    سانچہ<> ساخت Fmt<uint64_t>
    {
        قسمتعریف غیرعلامتی طویل طویل عارضی_type;
        ساکن ثابت حرف* fmt() { واپس "%llu"; }
    };

    سانچہ<> ساخت Fmt<اعشاری>
    {
        قسمتعریف اعشاری عارضی_type;
        ساکن ثابت حرف* fmt() { واپس "%.5g"; }
    };

    سانچہ<> ساخت Fmt<دوہرا>
    {
        قسمتعریف دوہرا عارضی_type;
        ساکن ثابت حرف* fmt() { واپس "%.5g"; }
    };

    سانچہ<> ساخت Fmt<hfloat>
    {
        قسمتعریف اعشاری عارضی_type;
        ساکن ثابت حرف* fmt() { واپس "%.5g"; }
    };

    سانچہ<> ساخت Fmt<bfloat>
    {
        قسمتعریف اعشاری عارضی_type;
        ساکن ثابت حرف* fmt() { واپس "%.4g"; }
    };

    سانچہ<> ساخت Fmt<fp8_t>
    {
        قسمتعریف اعشاری عارضی_type;
        ساکن ثابت حرف* fmt() { واپس "%.4g"; }
    };

    سانچہ<> ساخت Fmt<fp8a_t>
    {
        قسمتعریف اعشاری عارضی_type;
        ساکن ثابت حرف* fmt() { واپس "%.4g"; }
    };

    سانچہ <قسم_نام _Tp>
    ساکن خالی pprint_صف(std::ostream& strm, ثابت _Tp* ptr, صحیح n, size_t ofs, صحیح کنارہ)
    {
        حرف buf[128];
        ثابت حرف* fmt = Fmt<_Tp>::fmt();
        صحیح i, ndump = کنارہ > 0 ? std::min(n, کنارہ*2+1) : n;
        اگر (کنارہ == 0)
            کنارہ = ndump;
        برائے (i = 0; i < ndump; i++) {
            صحیح j = n == ndump || i < کنارہ ? i : i == کنارہ ? -1 : n-کنارہ*2-1+i;
            اگر (i > 0)
                strm << ", ";
            اگر (j >= 0) {
                snprintf(buf, حجم(buf), fmt, (قسم_نام Fmt<_Tp>::عارضی_type)ptr[ofs + j]);
                strm << buf;
            } ورنہ
                strm << "... ";
        }
    }

    ساکن خالی pprint_قاش(std::ostream& strm, ثابت Mat& tensor,
                            ثابت size_t* قدم, صحیح d,
                            size_t ofs, صحیح کنارہ)
    {
        MatShape shape = tensor.shape();
        صحیح ndims = shape.dims;
        صحیح n = d >= ndims ? 1 : shape[d];
        اگر (d >= ndims - 1) {
            صحیح typ = tensor.گہرائی();
            خالی* ڈیٹا = tensor.ڈیٹا;
            CV_Assert(ڈیٹا);
            n *= tensor.channels();
            اگر (typ == CV_8U)
                pprint_صف(strm, (ثابت uint8_t*)ڈیٹا, n, ofs, کنارہ);
            ورنہ اگر (typ == CV_8S)
                pprint_صف(strm, (ثابت int8_t*)ڈیٹا, n, ofs, کنارہ);
            ورنہ اگر (typ == CV_16U)
                pprint_صف(strm, (ثابت uint16_t*)ڈیٹا, n, ofs, کنارہ);
            ورنہ اگر (typ == CV_16S)
                pprint_صف(strm, (ثابت int16_t*)ڈیٹا, n, ofs, کنارہ);
            ورنہ اگر (typ == CV_32U)
                pprint_صف(strm, (ثابت غیرعلامتی*)ڈیٹا, n, ofs, کنارہ);
            ورنہ اگر (typ == CV_32S)
                pprint_صف(strm, (ثابت صحیح*)ڈیٹا, n, ofs, کنارہ);
            ورنہ اگر (typ == CV_64U)
                pprint_صف(strm, (ثابت uint64_t*)ڈیٹا, n, ofs, کنارہ);
            ورنہ اگر (typ == CV_64S)
                pprint_صف(strm, (ثابت int64_t*)ڈیٹا, n, ofs, کنارہ);
            ورنہ اگر (typ == CV_32F)
                pprint_صف(strm, (ثابت اعشاری*)ڈیٹا, n, ofs, کنارہ);
            ورنہ اگر (typ == CV_64F)
                pprint_صف(strm, (ثابت دوہرا*)ڈیٹا, n, ofs, کنارہ);
            ورنہ اگر (typ == CV_16F)
                pprint_صف(strm, (ثابت hfloat*)ڈیٹا, n, ofs, کنارہ);
            ورنہ اگر (typ == CV_16BF)
                pprint_صف(strm, (ثابت bfloat*)ڈیٹا, n, ofs, کنارہ);
            ورنہ اگر (typ == CV_Bool)
                pprint_صف(strm, (ثابت منطقی*)ڈیٹا, n, ofs, کنارہ);
            ورنہ اگر (typ == CV_8F_E4M3FN)
                pprint_صف(strm, (ثابت fp8_t*)ڈیٹا, n, ofs, کنارہ);
            ورنہ اگر (typ == CV_8F_E4M3FNUZ)
                pprint_صف(strm, (ثابت fp8a_t*)ڈیٹا, n, ofs, کنارہ);
            ورنہ {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } ورنہ {
            صحیح i, ndump = کنارہ > 0 ? std::min(n, کنارہ*2+1) : n;
            منطقی dots = جھوٹ;
            برائے (i = 0; i < ndump; i++) {
                اگر (i > 0 && !dots) {
                    صحیح nempty_سطریں = ndims - 2 - d;
                    برائے (صحیح k = 0; k < nempty_سطریں; k++)
                        strm << "\n";
                }
                اگر (i > 0)
                    strm << "\n";
                صحیح j = n == ndump || i < کنارہ ? i :
                        i == کنارہ ? -1 :
                        n - کنارہ*2 - 1 + i;
                dots = j < 0;
                اگر (!dots)
                    pprint_قاش(strm, tensor, قدم, d+1, ofs + j*قدم[d], کنارہ);
                ورنہ
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, داخلہ_Array array,
                         صحیح /*indent*/, صحیح کنارہ_2,
                         صحیح پورا_Tensor_حد,
                         حرف parens)
    {
        حرف oparen = parens;
        حرف cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        صحیح کنارہ = کنارہ_2 > 0 ? کنارہ_2 : 3;
        پورا_Tensor_حد = پورا_Tensor_حد > 0 ? پورا_Tensor_حد : 100;

        Mat tensor = array.getMat();
        اگر (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat عارضی_2;
            tensor.copy_مقصد(عارضی_2);
            tensor = عارضی_2;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.کل();

        اگر (parens)
            strm << oparen;
        اگر (sz_all == 0) {
            اگر (!parens)
                strm << "<empty>";
        } ورنہ {
            اگر (sz_all <= (size_t)پورا_Tensor_حد)
                کنارہ = 0;

            صحیح ndims = shape.dims;
            صحیح cn = tensor.channels();
            size_t قدم[MatShape::MAX_DIMS];
            قدم[std::max(ndims-1, 0)] = 1;
            برائے (صحیح i = ndims-2; i >= 0; i--) {
                قدم[i] = قدم[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprint_قاش(strm, tensor, قدم, 0, 0, کنارہ);
        }
        اگر (parens)
            strm << cparen;
        واپس strm;
    }

} // cv
