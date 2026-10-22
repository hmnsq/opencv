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
               STATE_لیکه_OPEN, STATE_لیکه_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_ارزښت, STATE_پای_ته_رسېدلی,
               STATE_کرښه_SEPARATOR, STATE_CN_SEPARATOR, STATE_ارزښت_SEPARATOR };
        تعداد {BRACE_لیکه_OPEN = 0, BRACE_لیکه_CLOSE = 1, BRACE_لیکه_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        حرف floatFormat[8];
        حرف buf[32];   // enough for double with precision up to 20

        Mat mtx;
        صحيح mcn; // == mtx.channels()
        منطقي single_کرښه;
        منطقي align_ترتیب;    // true when cn first order

        صحيح state;
        صحيح لیکه;
        صحيح ستون;
        صحيح cn;

        String prologue;
        String epilogue;
        حرف braces[5];

        فراغ (FormattedImpl::*ارزښت_منزل_Str)();
        فراغ ارزښت_منزل_Str_Bool() { snprintf(buf, حجم(buf), "%d", (صحيح)mtx.ptr<uchar>(لیکه, ستون)[cn] != 0); }
        فراغ ارزښت_منزل_Str8u()  { snprintf(buf, حجم(buf), "%3d", (صحيح)mtx.ptr<uchar>(لیکه, ستون)[cn]); }
        فراغ ارزښت_منزل_Str8s()  { snprintf(buf, حجم(buf), "%3d", (صحيح)mtx.ptr<schar>(لیکه, ستون)[cn]); }
        فراغ ارزښت_منزل_Str16u() { snprintf(buf, حجم(buf), "%d", (صحيح)mtx.ptr<ushort>(لیکه, ستون)[cn]); }
        فراغ ارزښت_منزل_Str16s() { snprintf(buf, حجم(buf), "%d", (صحيح)mtx.ptr<لنډ>(لیکه, ستون)[cn]); }
        فراغ ارزښت_منزل_Str32u() { snprintf(buf, حجم(buf), "%u", mtx.ptr<بینښه>(لیکه, ستون)[cn]); }
        فراغ ارزښت_منزل_Str32s() { snprintf(buf, حجم(buf), "%d", mtx.ptr<صحيح>(لیکه, ستون)[cn]); }
        فراغ ارزښت_منزل_Str32f() { snprintf(buf, حجم(buf), floatFormat, mtx.ptr<عائم>(لیکه, ستون)[cn]); }
        فراغ ارزښت_منزل_Str64f() { snprintf(buf, حجم(buf), floatFormat, mtx.ptr<مزدوج>(لیکه, ستون)[cn]); }
        فراغ ارزښت_منزل_Str64u() { snprintf(buf, حجم(buf), "%llu", (بینښه اوږد اوږد)mtx.ptr<uint64_t>(لیکه, ستون)[cn]); }
        فراغ ارزښت_منزل_Str64s() { snprintf(buf, حجم(buf), "%lld", (اوږد اوږد)mtx.ptr<int64_t>(لیکه, ستون)[cn]); }
        فراغ ارزښت_منزل_Str16f() { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<hfloat>(لیکه, ستون)[cn]); }
        فراغ ارزښت_منزل_Str16bf() { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<bfloat>(لیکه, ستون)[cn]); }
        فراغ ارزښت_منزل_Str8f()   { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<fp8_t>(لیکه, ستون)[cn]); }
        فراغ ارزښت_منزل_Str8fnuz() { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<fp8a_t>(لیکه, ستون)[cn]); }
        فراغ ارزښت_منزل_Str_Other() { buf[0] = 0; }

    عام:

        FormattedImpl(String pl, String el, Mat m, حرف br[5], منطقي s_کرښه, منطقي a_ترتیب, صحيح precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            حافظه_نقل(braces, br, 5);
            state = STATE_PROLOGUE;
            single_کرښه = s_کرښه;
            align_ترتیب = a_ترتیب;
            لیکه = ستون = cn =0;

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

            بدل(mtx.ژوروالی())
            {
                حالة CV_8U:  ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str8u; اكسر;
                حالة CV_8S:  ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str8s; اكسر;
                حالة CV_Bool: ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str_Bool; اكسر;
                حالة CV_16U: ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str16u; اكسر;
                حالة CV_16S: ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str16s; اكسر;
                حالة CV_32U: ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str32u; اكسر;
                حالة CV_32S: ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str32s; اكسر;
                حالة CV_32F: ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str32f; اكسر;
                حالة CV_64F: ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str64f; اكسر;
                حالة CV_64U: ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str64u; اكسر;
                حالة CV_64S: ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str64s; اكسر;
                حالة CV_16F: ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str16f; اكسر;
                حالة CV_16BF: ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str16bf; اكسر;
                حالة CV_8F_E4M3FN:   ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str8f; اكسر;
                حالة CV_8F_E4M3FNUZ: ارزښت_منزل_Str = &FormattedImpl::ارزښت_منزل_Str8fnuz; اكسر;
                افتراضي:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.ژوروالی()));
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
                    لیکه = 0;
                    إذا (mtx.empty())
                        state = STATE_EPILOGUE;
                    وإلا إذا (align_ترتیب)
                        state = STATE_INTERLUDE;
                    وإلا
                        state = STATE_لیکه_OPEN;
                    أرجع prologue.c_str();
                حالة STATE_INTERLUDE:
                    state = STATE_لیکه_OPEN;
                    إذا (لیکه >= mtx.لیکې)
                    {
                        إذا (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            أرجع buf;
                        }
                        وإلا
                            لیکه = 0;
                        snprintf(buf, حجم(buf), "\n(:, :, %d) = \n", cn+1);
                        أرجع buf;
                    }
                    snprintf(buf, حجم(buf), "(:, :, %d) = \n", cn+1);
                    أرجع buf;
                حالة STATE_EPILOGUE:
                    state = STATE_پای_ته_رسېدلی;
                    أرجع epilogue.c_str();
                حالة STATE_لیکه_OPEN:
                    ستون = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        إذا (لیکه > 0)
                            طالما(pos < prologue.size() && pos < حجم(buf) - 2)
                                buf[pos++] = ' ';
                        إذا (braces[BRACE_لیکه_OPEN])
                            buf[pos++] = braces[BRACE_لیکه_OPEN];
                        إذا(!pos)
                            أرجع next();
                        buf[pos] = 0;
                    }
                    أرجع buf;
                حالة STATE_لیکه_CLOSE:
                    state = STATE_کرښه_SEPARATOR;
                    ++لیکه;
                    إذا (braces[BRACE_لیکه_CLOSE])
                    {
                        buf[0] = braces[BRACE_لیکه_CLOSE];
                        buf[1] = لیکه < mtx.لیکې ? ',' : '\0';
                        buf[2] = 0;
                        أرجع buf;
                    }
                    وإلا إذا(braces[BRACE_لیکه_SEP] && لیکه < mtx.لیکې)
                    {
                        buf[0] = braces[BRACE_لیکه_SEP];
                        buf[1] = 0;
                        أرجع buf;
                    }
                    أرجع next();
                حالة STATE_CN_OPEN:
                    state = STATE_ارزښت;
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
                    إذا (ستون >= mtx.ستون_شمېر)
                        state = STATE_لیکه_CLOSE;
                    وإلا
                        state = STATE_CN_SEPARATOR;
                    إذا (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        أرجع buf;
                    }
                    أرجع next();
                حالة STATE_ارزښت:
                    (هذا->*ارزښت_منزل_Str)();
                    state = STATE_CN_CLOSE;
                    إذا (align_ترتیب)
                        أرجع buf;
                    إذا (++cn < mcn)
                        state = STATE_ارزښت_SEPARATOR;
                    أرجع buf;
                حالة STATE_پای_ته_رسېدلی:
                    أرجع 0;
                حالة STATE_کرښه_SEPARATOR:
                    إذا (لیکه >= mtx.لیکې)
                    {
                        إذا (align_ترتیب)
                            state = STATE_INTERLUDE;
                        وإلا
                            state = STATE_EPILOGUE;
                        أرجع next();
                    }
                    state = STATE_لیکه_OPEN;
                    buf[0] = single_کرښه ? ' ' : '\n';
                    buf[1] = 0;
                    أرجع buf;
                حالة STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    أرجع buf;
                حالة STATE_ارزښت_SEPARATOR:
                    state = STATE_ارزښت;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    أرجع buf;
            }
            أرجع 0;
        }
    };

    صنف Formatter_بنسټ : عام Formatter
    {
    عام:
        Formatter_بنسټ() : prec16f(4), prec32f(8), prec64f(16), multiline(ریښتیا) {}

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

        فراغ سېټ_Multiline(منطقي ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    محمي:
        صحيح prec16f;
        صحيح prec32f;
        صحيح prec64f;
        صحيح multiline;
    };

    صنف DefaultFormatter CV_FINAL : عام Formatter_بنسټ
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ';', '\0', '\0'};
            أرجع makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.لیکې == 1 || !multiline, خطأ, mtx.ژوروالی() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف MatlabFormatter CV_FINAL : عام Formatter_بنسټ
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ';', '\0', '\0'};
            أرجع makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.لیکې == 1 || !multiline, ریښتیا, mtx.ژوروالی() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف PythonFormatter CV_FINAL : عام Formatter_بنسټ
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'[', ']', ',', '[', ']'};
            إذا (mtx.ستون_شمېر == 1)
                braces[0] = braces[1] = '\0';
            أرجع makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.لیکې == 1 || !multiline, خطأ, mtx.ژوروالی() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف NumpyFormatter CV_FINAL : عام Formatter_بنسټ
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            ساكن ثابت حرف* numpyTypes[CV_ژوروالی_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            حرف braces[5] = {'[', ']', ',', '[', ']'};
            إذا (mtx.ستون_شمېر == 1)
                braces[0] = braces[1] = '\0';
            أرجع makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.ژوروالی()]), mtx, &*braces,
                mtx.لیکې == 1 || !multiline, خطأ, mtx.ژوروالی() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف CSVFormatter CV_FINAL : عام Formatter_بنسټ
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            أرجع makePtr<FormattedImpl>(String(),
                mtx.لیکې > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.لیکې == 1 || !multiline, خطأ, mtx.ژوروالی() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف CFormatter CV_FINAL : عام Formatter_بنسټ
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ',', '\0', '\0'};
            أرجع makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.لیکې == 1 || !multiline, خطأ, mtx.ژوروالی() == CV_64F ? prec64f : prec32f );
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

    کينډۍ<ډول_نوم _Tp> بنية Fmt
    {
        تعريفنوع صحيح لنډمهاله_type;
        ساكن ثابت حرف* fmt() { أرجع "%d"; }
    };

    کينډۍ<> بنية Fmt<uint32_t>
    {
        تعريفنوع بینښه لنډمهاله_type;
        ساكن ثابت حرف* fmt() { أرجع "%u"; }
    };

    کينډۍ<> بنية Fmt<int64_t>
    {
        تعريفنوع اوږد اوږد لنډمهاله_type;
        ساكن ثابت حرف* fmt() { أرجع "%lld"; }
    };

    کينډۍ<> بنية Fmt<uint64_t>
    {
        تعريفنوع بینښه اوږد اوږد لنډمهاله_type;
        ساكن ثابت حرف* fmt() { أرجع "%llu"; }
    };

    کينډۍ<> بنية Fmt<عائم>
    {
        تعريفنوع عائم لنډمهاله_type;
        ساكن ثابت حرف* fmt() { أرجع "%.5g"; }
    };

    کينډۍ<> بنية Fmt<مزدوج>
    {
        تعريفنوع مزدوج لنډمهاله_type;
        ساكن ثابت حرف* fmt() { أرجع "%.5g"; }
    };

    کينډۍ<> بنية Fmt<hfloat>
    {
        تعريفنوع عائم لنډمهاله_type;
        ساكن ثابت حرف* fmt() { أرجع "%.5g"; }
    };

    کينډۍ<> بنية Fmt<bfloat>
    {
        تعريفنوع عائم لنډمهاله_type;
        ساكن ثابت حرف* fmt() { أرجع "%.4g"; }
    };

    کينډۍ<> بنية Fmt<fp8_t>
    {
        تعريفنوع عائم لنډمهاله_type;
        ساكن ثابت حرف* fmt() { أرجع "%.4g"; }
    };

    کينډۍ<> بنية Fmt<fp8a_t>
    {
        تعريفنوع عائم لنډمهاله_type;
        ساكن ثابت حرف* fmt() { أرجع "%.4g"; }
    };

    کينډۍ <ډول_نوم _Tp>
    ساكن فراغ pprint_لیکه(std::ostream& strm, ثابت _Tp* ptr, صحيح n, size_t ofs, صحيح څنډه)
    {
        حرف buf[128];
        ثابت حرف* fmt = Fmt<_Tp>::fmt();
        صحيح i, ndump = څنډه > 0 ? std::min(n, څنډه*2+1) : n;
        إذا (څنډه == 0)
            څنډه = ndump;
        لكل (i = 0; i < ndump; i++) {
            صحيح j = n == ndump || i < څنډه ? i : i == څنډه ? -1 : n-څنډه*2-1+i;
            إذا (i > 0)
                strm << ", ";
            إذا (j >= 0) {
                snprintf(buf, حجم(buf), fmt, (ډول_نوم Fmt<_Tp>::لنډمهاله_type)ptr[ofs + j]);
                strm << buf;
            } وإلا
                strm << "... ";
        }
    }

    ساكن فراغ pprint_ټوټه(std::ostream& strm, ثابت Mat& tensor,
                            ثابت size_t* ګام, صحيح d,
                            size_t ofs, صحيح څنډه)
    {
        MatShape shape = tensor.shape();
        صحيح ndims = shape.dims;
        صحيح n = d >= ndims ? 1 : shape[d];
        إذا (d >= ndims - 1) {
            صحيح typ = tensor.ژوروالی();
            فراغ* معلومات = tensor.معلومات;
            CV_Assert(معلومات);
            n *= tensor.channels();
            إذا (typ == CV_8U)
                pprint_لیکه(strm, (ثابت uint8_t*)معلومات, n, ofs, څنډه);
            وإلا إذا (typ == CV_8S)
                pprint_لیکه(strm, (ثابت int8_t*)معلومات, n, ofs, څنډه);
            وإلا إذا (typ == CV_16U)
                pprint_لیکه(strm, (ثابت uint16_t*)معلومات, n, ofs, څنډه);
            وإلا إذا (typ == CV_16S)
                pprint_لیکه(strm, (ثابت int16_t*)معلومات, n, ofs, څنډه);
            وإلا إذا (typ == CV_32U)
                pprint_لیکه(strm, (ثابت بینښه*)معلومات, n, ofs, څنډه);
            وإلا إذا (typ == CV_32S)
                pprint_لیکه(strm, (ثابت صحيح*)معلومات, n, ofs, څنډه);
            وإلا إذا (typ == CV_64U)
                pprint_لیکه(strm, (ثابت uint64_t*)معلومات, n, ofs, څنډه);
            وإلا إذا (typ == CV_64S)
                pprint_لیکه(strm, (ثابت int64_t*)معلومات, n, ofs, څنډه);
            وإلا إذا (typ == CV_32F)
                pprint_لیکه(strm, (ثابت عائم*)معلومات, n, ofs, څنډه);
            وإلا إذا (typ == CV_64F)
                pprint_لیکه(strm, (ثابت مزدوج*)معلومات, n, ofs, څنډه);
            وإلا إذا (typ == CV_16F)
                pprint_لیکه(strm, (ثابت hfloat*)معلومات, n, ofs, څنډه);
            وإلا إذا (typ == CV_16BF)
                pprint_لیکه(strm, (ثابت bfloat*)معلومات, n, ofs, څنډه);
            وإلا إذا (typ == CV_Bool)
                pprint_لیکه(strm, (ثابت منطقي*)معلومات, n, ofs, څنډه);
            وإلا إذا (typ == CV_8F_E4M3FN)
                pprint_لیکه(strm, (ثابت fp8_t*)معلومات, n, ofs, څنډه);
            وإلا إذا (typ == CV_8F_E4M3FNUZ)
                pprint_لیکه(strm, (ثابت fp8a_t*)معلومات, n, ofs, څنډه);
            وإلا {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } وإلا {
            صحيح i, ndump = څنډه > 0 ? std::min(n, څنډه*2+1) : n;
            منطقي dots = خطأ;
            لكل (i = 0; i < ndump; i++) {
                إذا (i > 0 && !dots) {
                    صحيح nempty_کرښې = ndims - 2 - d;
                    لكل (صحيح k = 0; k < nempty_کرښې; k++)
                        strm << "\n";
                }
                إذا (i > 0)
                    strm << "\n";
                صحيح j = n == ndump || i < څنډه ? i :
                        i == څنډه ? -1 :
                        n - څنډه*2 - 1 + i;
                dots = j < 0;
                إذا (!dots)
                    pprint_ټوټه(strm, tensor, ګام, d+1, ofs + j*ګام[d], څنډه);
                وإلا
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, ننوت_Array array,
                         صحيح /*indent*/, صحيح څنډه_2,
                         صحيح ټولیز_Tensor_پوله,
                         حرف parens)
    {
        حرف oparen = parens;
        حرف cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        صحيح څنډه = څنډه_2 > 0 ? څنډه_2 : 3;
        ټولیز_Tensor_پوله = ټولیز_Tensor_پوله > 0 ? ټولیز_Tensor_پوله : 100;

        Mat tensor = array.getMat();
        إذا (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat لنډمهاله_2;
            tensor.copy_منزل(لنډمهاله_2);
            tensor = لنډمهاله_2;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.ټول();

        إذا (parens)
            strm << oparen;
        إذا (sz_all == 0) {
            إذا (!parens)
                strm << "<empty>";
        } وإلا {
            إذا (sz_all <= (size_t)ټولیز_Tensor_پوله)
                څنډه = 0;

            صحيح ndims = shape.dims;
            صحيح cn = tensor.channels();
            size_t ګام[MatShape::MAX_DIMS];
            ګام[std::max(ndims-1, 0)] = 1;
            لكل (صحيح i = ndims-2; i >= 0; i--) {
                ګام[i] = ګام[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprint_ټوټه(strm, tensor, ګام, 0, 0, څنډه);
        }
        إذا (parens)
            strm << cparen;
        أرجع strm;
    }

} // cv
