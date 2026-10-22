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
               STATE_ردیف_OPEN, STATE_ردیف_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_مقدار, STATE_پایانیافته,
               STATE_سطر_SEPARATOR, STATE_CN_SEPARATOR, STATE_مقدار_SEPARATOR };
        تعداد {BRACE_ردیف_OPEN = 0, BRACE_ردیف_CLOSE = 1, BRACE_ردیف_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        حرف floatFormat[8];
        حرف buf[32];   // enough for double with precision up to 20

        Mat mtx;
        صحيح mcn; // == mtx.channels()
        منطقي single_سطر;
        منطقي align_ترتیب;    // true when cn first order

        صحيح state;
        صحيح ردیف;
        صحيح ستون;
        صحيح cn;

        String prologue;
        String epilogue;
        حرف braces[5];

        فراغ (FormattedImpl::*مقدار_مقصد_Str)();
        فراغ مقدار_مقصد_Str_Bool() { snprintf(buf, حجم(buf), "%d", (صحيح)mtx.ptr<uchar>(ردیف, ستون)[cn] != 0); }
        فراغ مقدار_مقصد_Str8u()  { snprintf(buf, حجم(buf), "%3d", (صحيح)mtx.ptr<uchar>(ردیف, ستون)[cn]); }
        فراغ مقدار_مقصد_Str8s()  { snprintf(buf, حجم(buf), "%3d", (صحيح)mtx.ptr<schar>(ردیف, ستون)[cn]); }
        فراغ مقدار_مقصد_Str16u() { snprintf(buf, حجم(buf), "%d", (صحيح)mtx.ptr<ushort>(ردیف, ستون)[cn]); }
        فراغ مقدار_مقصد_Str16s() { snprintf(buf, حجم(buf), "%d", (صحيح)mtx.ptr<کوتاه>(ردیف, ستون)[cn]); }
        فراغ مقدار_مقصد_Str32u() { snprintf(buf, حجم(buf), "%u", mtx.ptr<بیعلامت>(ردیف, ستون)[cn]); }
        فراغ مقدار_مقصد_Str32s() { snprintf(buf, حجم(buf), "%d", mtx.ptr<صحيح>(ردیف, ستون)[cn]); }
        فراغ مقدار_مقصد_Str32f() { snprintf(buf, حجم(buf), floatFormat, mtx.ptr<عائم>(ردیف, ستون)[cn]); }
        فراغ مقدار_مقصد_Str64f() { snprintf(buf, حجم(buf), floatFormat, mtx.ptr<مزدوج>(ردیف, ستون)[cn]); }
        فراغ مقدار_مقصد_Str64u() { snprintf(buf, حجم(buf), "%llu", (بیعلامت دراز دراز)mtx.ptr<uint64_t>(ردیف, ستون)[cn]); }
        فراغ مقدار_مقصد_Str64s() { snprintf(buf, حجم(buf), "%lld", (دراز دراز)mtx.ptr<int64_t>(ردیف, ستون)[cn]); }
        فراغ مقدار_مقصد_Str16f() { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<hfloat>(ردیف, ستون)[cn]); }
        فراغ مقدار_مقصد_Str16bf() { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<bfloat>(ردیف, ستون)[cn]); }
        فراغ مقدار_مقصد_Str8f()   { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<fp8_t>(ردیف, ستون)[cn]); }
        فراغ مقدار_مقصد_Str8fnuz() { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<fp8a_t>(ردیف, ستون)[cn]); }
        فراغ مقدار_مقصد_Str_Other() { buf[0] = 0; }

    عام:

        FormattedImpl(String pl, String el, Mat m, حرف br[5], منطقي s_سطر, منطقي a_ترتیب, صحيح precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            کپی_حافظه(braces, br, 5);
            state = STATE_PROLOGUE;
            single_سطر = s_سطر;
            align_ترتیب = a_ترتیب;
            ردیف = ستون = cn =0;

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

            بدل(mtx.عمق())
            {
                حالة CV_8U:  مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str8u; اكسر;
                حالة CV_8S:  مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str8s; اكسر;
                حالة CV_Bool: مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str_Bool; اكسر;
                حالة CV_16U: مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str16u; اكسر;
                حالة CV_16S: مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str16s; اكسر;
                حالة CV_32U: مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str32u; اكسر;
                حالة CV_32S: مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str32s; اكسر;
                حالة CV_32F: مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str32f; اكسر;
                حالة CV_64F: مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str64f; اكسر;
                حالة CV_64U: مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str64u; اكسر;
                حالة CV_64S: مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str64s; اكسر;
                حالة CV_16F: مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str16f; اكسر;
                حالة CV_16BF: مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str16bf; اكسر;
                حالة CV_8F_E4M3FN:   مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str8f; اكسر;
                حالة CV_8F_E4M3FNUZ: مقدار_مقصد_Str = &FormattedImpl::مقدار_مقصد_Str8fnuz; اكسر;
                افتراضي:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.عمق()));
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
                    ردیف = 0;
                    إذا (mtx.empty())
                        state = STATE_EPILOGUE;
                    وإلا إذا (align_ترتیب)
                        state = STATE_INTERLUDE;
                    وإلا
                        state = STATE_ردیف_OPEN;
                    أرجع prologue.c_str();
                حالة STATE_INTERLUDE:
                    state = STATE_ردیف_OPEN;
                    إذا (ردیف >= mtx.ردیفها)
                    {
                        إذا (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            أرجع buf;
                        }
                        وإلا
                            ردیف = 0;
                        snprintf(buf, حجم(buf), "\n(:, :, %d) = \n", cn+1);
                        أرجع buf;
                    }
                    snprintf(buf, حجم(buf), "(:, :, %d) = \n", cn+1);
                    أرجع buf;
                حالة STATE_EPILOGUE:
                    state = STATE_پایانیافته;
                    أرجع epilogue.c_str();
                حالة STATE_ردیف_OPEN:
                    ستون = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        إذا (ردیف > 0)
                            طالما(pos < prologue.size() && pos < حجم(buf) - 2)
                                buf[pos++] = ' ';
                        إذا (braces[BRACE_ردیف_OPEN])
                            buf[pos++] = braces[BRACE_ردیف_OPEN];
                        إذا(!pos)
                            أرجع next();
                        buf[pos] = 0;
                    }
                    أرجع buf;
                حالة STATE_ردیف_CLOSE:
                    state = STATE_سطر_SEPARATOR;
                    ++ردیف;
                    إذا (braces[BRACE_ردیف_CLOSE])
                    {
                        buf[0] = braces[BRACE_ردیف_CLOSE];
                        buf[1] = ردیف < mtx.ردیفها ? ',' : '\0';
                        buf[2] = 0;
                        أرجع buf;
                    }
                    وإلا إذا(braces[BRACE_ردیف_SEP] && ردیف < mtx.ردیفها)
                    {
                        buf[0] = braces[BRACE_ردیف_SEP];
                        buf[1] = 0;
                        أرجع buf;
                    }
                    أرجع next();
                حالة STATE_CN_OPEN:
                    state = STATE_مقدار;
                    إذا (!align_ترتیب)
                        cn = 0;
                    إذا (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        أرجع buf;
                    }
                    أرجع next();
                حالة STATE_CN_CLOSE:
                    ++ستون;
                    إذا (ستون >= mtx.تعداد_ستون)
                        state = STATE_ردیف_CLOSE;
                    وإلا
                        state = STATE_CN_SEPARATOR;
                    إذا (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        أرجع buf;
                    }
                    أرجع next();
                حالة STATE_مقدار:
                    (هذا->*مقدار_مقصد_Str)();
                    state = STATE_CN_CLOSE;
                    إذا (align_ترتیب)
                        أرجع buf;
                    إذا (++cn < mcn)
                        state = STATE_مقدار_SEPARATOR;
                    أرجع buf;
                حالة STATE_پایانیافته:
                    أرجع 0;
                حالة STATE_سطر_SEPARATOR:
                    إذا (ردیف >= mtx.ردیفها)
                    {
                        إذا (align_ترتیب)
                            state = STATE_INTERLUDE;
                        وإلا
                            state = STATE_EPILOGUE;
                        أرجع next();
                    }
                    state = STATE_ردیف_OPEN;
                    buf[0] = single_سطر ? ' ' : '\n';
                    buf[1] = 0;
                    أرجع buf;
                حالة STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    أرجع buf;
                حالة STATE_مقدار_SEPARATOR:
                    state = STATE_مقدار;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    أرجع buf;
            }
            أرجع 0;
        }
    };

    صنف Formatter_پایه : عام Formatter
    {
    عام:
        Formatter_پایه() : prec16f(4), prec32f(8), prec64f(16), multiline(درست) {}

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

        فراغ مجموعه_Multiline(منطقي ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    محمي:
        صحيح prec16f;
        صحيح prec32f;
        صحيح prec64f;
        صحيح multiline;
    };

    صنف DefaultFormatter CV_FINAL : عام Formatter_پایه
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ';', '\0', '\0'};
            أرجع makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.ردیفها == 1 || !multiline, خطأ, mtx.عمق() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف MatlabFormatter CV_FINAL : عام Formatter_پایه
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ';', '\0', '\0'};
            أرجع makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.ردیفها == 1 || !multiline, درست, mtx.عمق() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف PythonFormatter CV_FINAL : عام Formatter_پایه
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'[', ']', ',', '[', ']'};
            إذا (mtx.تعداد_ستون == 1)
                braces[0] = braces[1] = '\0';
            أرجع makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.ردیفها == 1 || !multiline, خطأ, mtx.عمق() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف NumpyFormatter CV_FINAL : عام Formatter_پایه
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            ساكن ثابت حرف* numpyTypes[CV_عمق_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            حرف braces[5] = {'[', ']', ',', '[', ']'};
            إذا (mtx.تعداد_ستون == 1)
                braces[0] = braces[1] = '\0';
            أرجع makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.عمق()]), mtx, &*braces,
                mtx.ردیفها == 1 || !multiline, خطأ, mtx.عمق() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف CSVFormatter CV_FINAL : عام Formatter_پایه
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            أرجع makePtr<FormattedImpl>(String(),
                mtx.ردیفها > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.ردیفها == 1 || !multiline, خطأ, mtx.عمق() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف CFormatter CV_FINAL : عام Formatter_پایه
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ',', '\0', '\0'};
            أرجع makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.ردیفها == 1 || !multiline, خطأ, mtx.عمق() == CV_64F ? prec64f : prec32f );
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

    قالب<نام_نوع _Tp> بنية Fmt
    {
        تعريفنوع صحيح موقت_type;
        ساكن ثابت حرف* fmt() { أرجع "%d"; }
    };

    قالب<> بنية Fmt<uint32_t>
    {
        تعريفنوع بیعلامت موقت_type;
        ساكن ثابت حرف* fmt() { أرجع "%u"; }
    };

    قالب<> بنية Fmt<int64_t>
    {
        تعريفنوع دراز دراز موقت_type;
        ساكن ثابت حرف* fmt() { أرجع "%lld"; }
    };

    قالب<> بنية Fmt<uint64_t>
    {
        تعريفنوع بیعلامت دراز دراز موقت_type;
        ساكن ثابت حرف* fmt() { أرجع "%llu"; }
    };

    قالب<> بنية Fmt<عائم>
    {
        تعريفنوع عائم موقت_type;
        ساكن ثابت حرف* fmt() { أرجع "%.5g"; }
    };

    قالب<> بنية Fmt<مزدوج>
    {
        تعريفنوع مزدوج موقت_type;
        ساكن ثابت حرف* fmt() { أرجع "%.5g"; }
    };

    قالب<> بنية Fmt<hfloat>
    {
        تعريفنوع عائم موقت_type;
        ساكن ثابت حرف* fmt() { أرجع "%.5g"; }
    };

    قالب<> بنية Fmt<bfloat>
    {
        تعريفنوع عائم موقت_type;
        ساكن ثابت حرف* fmt() { أرجع "%.4g"; }
    };

    قالب<> بنية Fmt<fp8_t>
    {
        تعريفنوع عائم موقت_type;
        ساكن ثابت حرف* fmt() { أرجع "%.4g"; }
    };

    قالب<> بنية Fmt<fp8a_t>
    {
        تعريفنوع عائم موقت_type;
        ساكن ثابت حرف* fmt() { أرجع "%.4g"; }
    };

    قالب <نام_نوع _Tp>
    ساكن فراغ pprint_ردیف(std::ostream& strm, ثابت _Tp* ptr, صحيح n, size_t ofs, صحيح یال)
    {
        حرف buf[128];
        ثابت حرف* fmt = Fmt<_Tp>::fmt();
        صحيح i, ndump = یال > 0 ? std::min(n, یال*2+1) : n;
        إذا (یال == 0)
            یال = ndump;
        لكل (i = 0; i < ndump; i++) {
            صحيح j = n == ndump || i < یال ? i : i == یال ? -1 : n-یال*2-1+i;
            إذا (i > 0)
                strm << ", ";
            إذا (j >= 0) {
                snprintf(buf, حجم(buf), fmt, (نام_نوع Fmt<_Tp>::موقت_type)ptr[ofs + j]);
                strm << buf;
            } وإلا
                strm << "... ";
        }
    }

    ساكن فراغ pprint_برش(std::ostream& strm, ثابت Mat& tensor,
                            ثابت size_t* گام, صحيح d,
                            size_t ofs, صحيح یال)
    {
        MatShape shape = tensor.shape();
        صحيح ndims = shape.dims;
        صحيح n = d >= ndims ? 1 : shape[d];
        إذا (d >= ndims - 1) {
            صحيح typ = tensor.عمق();
            فراغ* داده = tensor.داده;
            CV_Assert(داده);
            n *= tensor.channels();
            إذا (typ == CV_8U)
                pprint_ردیف(strm, (ثابت uint8_t*)داده, n, ofs, یال);
            وإلا إذا (typ == CV_8S)
                pprint_ردیف(strm, (ثابت int8_t*)داده, n, ofs, یال);
            وإلا إذا (typ == CV_16U)
                pprint_ردیف(strm, (ثابت uint16_t*)داده, n, ofs, یال);
            وإلا إذا (typ == CV_16S)
                pprint_ردیف(strm, (ثابت int16_t*)داده, n, ofs, یال);
            وإلا إذا (typ == CV_32U)
                pprint_ردیف(strm, (ثابت بیعلامت*)داده, n, ofs, یال);
            وإلا إذا (typ == CV_32S)
                pprint_ردیف(strm, (ثابت صحيح*)داده, n, ofs, یال);
            وإلا إذا (typ == CV_64U)
                pprint_ردیف(strm, (ثابت uint64_t*)داده, n, ofs, یال);
            وإلا إذا (typ == CV_64S)
                pprint_ردیف(strm, (ثابت int64_t*)داده, n, ofs, یال);
            وإلا إذا (typ == CV_32F)
                pprint_ردیف(strm, (ثابت عائم*)داده, n, ofs, یال);
            وإلا إذا (typ == CV_64F)
                pprint_ردیف(strm, (ثابت مزدوج*)داده, n, ofs, یال);
            وإلا إذا (typ == CV_16F)
                pprint_ردیف(strm, (ثابت hfloat*)داده, n, ofs, یال);
            وإلا إذا (typ == CV_16BF)
                pprint_ردیف(strm, (ثابت bfloat*)داده, n, ofs, یال);
            وإلا إذا (typ == CV_Bool)
                pprint_ردیف(strm, (ثابت منطقي*)داده, n, ofs, یال);
            وإلا إذا (typ == CV_8F_E4M3FN)
                pprint_ردیف(strm, (ثابت fp8_t*)داده, n, ofs, یال);
            وإلا إذا (typ == CV_8F_E4M3FNUZ)
                pprint_ردیف(strm, (ثابت fp8a_t*)داده, n, ofs, یال);
            وإلا {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } وإلا {
            صحيح i, ndump = یال > 0 ? std::min(n, یال*2+1) : n;
            منطقي dots = خطأ;
            لكل (i = 0; i < ndump; i++) {
                إذا (i > 0 && !dots) {
                    صحيح nempty_سطرها = ndims - 2 - d;
                    لكل (صحيح k = 0; k < nempty_سطرها; k++)
                        strm << "\n";
                }
                إذا (i > 0)
                    strm << "\n";
                صحيح j = n == ndump || i < یال ? i :
                        i == یال ? -1 :
                        n - یال*2 - 1 + i;
                dots = j < 0;
                إذا (!dots)
                    pprint_برش(strm, tensor, گام, d+1, ofs + j*گام[d], یال);
                وإلا
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, درون_داد_Array array,
                         صحيح /*indent*/, صحيح یال_2,
                         صحيح کامل_Tensor_آستانه,
                         حرف parens)
    {
        حرف oparen = parens;
        حرف cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        صحيح یال = یال_2 > 0 ? یال_2 : 3;
        کامل_Tensor_آستانه = کامل_Tensor_آستانه > 0 ? کامل_Tensor_آستانه : 100;

        Mat tensor = array.getMat();
        إذا (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat موقت_2;
            tensor.copy_مقصد(موقت_2);
            tensor = موقت_2;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.مجموع();

        إذا (parens)
            strm << oparen;
        إذا (sz_all == 0) {
            إذا (!parens)
                strm << "<empty>";
        } وإلا {
            إذا (sz_all <= (size_t)کامل_Tensor_آستانه)
                یال = 0;

            صحيح ndims = shape.dims;
            صحيح cn = tensor.channels();
            size_t گام[MatShape::MAX_DIMS];
            گام[std::max(ndims-1, 0)] = 1;
            لكل (صحيح i = ndims-2; i >= 0; i--) {
                گام[i] = گام[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprint_برش(strm, tensor, گام, 0, 0, یال);
        }
        إذا (parens)
            strm << cparen;
        أرجع strm;
    }

} // cv
