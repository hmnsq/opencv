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
               STATE_صف_OPEN, STATE_صف_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_قدر, STATE_ختم,
               STATE_سٽ_SEPARATOR, STATE_CN_SEPARATOR, STATE_قدر_SEPARATOR };
        تعداد {BRACE_صف_OPEN = 0, BRACE_صف_CLOSE = 1, BRACE_صف_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        حرف floatFormat[8];
        حرف buf[32];   // enough for double with precision up to 20

        Mat mtx;
        صحيح mcn; // == mtx.channels()
        منطقي single_سٽ;
        منطقي align_ترتيب;    // true when cn first order

        صحيح state;
        صحيح صف;
        صحيح کانو;
        صحيح cn;

        String prologue;
        String epilogue;
        حرف braces[5];

        فراغ (FormattedImpl::*قدر_منزل_Str)();
        فراغ قدر_منزل_Str_Bool() { snprintf(buf, حجم(buf), "%d", (صحيح)mtx.ptr<uchar>(صف, کانو)[cn] != 0); }
        فراغ قدر_منزل_Str8u()  { snprintf(buf, حجم(buf), "%3d", (صحيح)mtx.ptr<uchar>(صف, کانو)[cn]); }
        فراغ قدر_منزل_Str8s()  { snprintf(buf, حجم(buf), "%3d", (صحيح)mtx.ptr<schar>(صف, کانو)[cn]); }
        فراغ قدر_منزل_Str16u() { snprintf(buf, حجم(buf), "%d", (صحيح)mtx.ptr<ushort>(صف, کانو)[cn]); }
        فراغ قدر_منزل_Str16s() { snprintf(buf, حجم(buf), "%d", (صحيح)mtx.ptr<ننڍو>(صف, کانو)[cn]); }
        فراغ قدر_منزل_Str32u() { snprintf(buf, حجم(buf), "%u", mtx.ptr<بنانشان>(صف, کانو)[cn]); }
        فراغ قدر_منزل_Str32s() { snprintf(buf, حجم(buf), "%d", mtx.ptr<صحيح>(صف, کانو)[cn]); }
        فراغ قدر_منزل_Str32f() { snprintf(buf, حجم(buf), floatFormat, mtx.ptr<عائم>(صف, کانو)[cn]); }
        فراغ قدر_منزل_Str64f() { snprintf(buf, حجم(buf), floatFormat, mtx.ptr<مزدوج>(صف, کانو)[cn]); }
        فراغ قدر_منزل_Str64u() { snprintf(buf, حجم(buf), "%llu", (بنانشان ڊگهو ڊگهو)mtx.ptr<uint64_t>(صف, کانو)[cn]); }
        فراغ قدر_منزل_Str64s() { snprintf(buf, حجم(buf), "%lld", (ڊگهو ڊگهو)mtx.ptr<int64_t>(صف, کانو)[cn]); }
        فراغ قدر_منزل_Str16f() { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<hfloat>(صف, کانو)[cn]); }
        فراغ قدر_منزل_Str16bf() { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<bfloat>(صف, کانو)[cn]); }
        فراغ قدر_منزل_Str8f()   { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<fp8_t>(صف, کانو)[cn]); }
        فراغ قدر_منزل_Str8fnuz() { snprintf(buf, حجم(buf), floatFormat, (عائم)mtx.ptr<fp8a_t>(صف, کانو)[cn]); }
        فراغ قدر_منزل_Str_Other() { buf[0] = 0; }

    عام:

        FormattedImpl(String pl, String el, Mat m, حرف br[5], منطقي s_سٽ, منطقي a_ترتيب, صحيح precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            يادگيري_نقل(braces, br, 5);
            state = STATE_PROLOGUE;
            single_سٽ = s_سٽ;
            align_ترتيب = a_ترتيب;
            صف = کانو = cn =0;

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

            بدل(mtx.اونهائي())
            {
                حالة CV_8U:  قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str8u; اكسر;
                حالة CV_8S:  قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str8s; اكسر;
                حالة CV_Bool: قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str_Bool; اكسر;
                حالة CV_16U: قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str16u; اكسر;
                حالة CV_16S: قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str16s; اكسر;
                حالة CV_32U: قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str32u; اكسر;
                حالة CV_32S: قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str32s; اكسر;
                حالة CV_32F: قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str32f; اكسر;
                حالة CV_64F: قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str64f; اكسر;
                حالة CV_64U: قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str64u; اكسر;
                حالة CV_64S: قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str64s; اكسر;
                حالة CV_16F: قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str16f; اكسر;
                حالة CV_16BF: قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str16bf; اكسر;
                حالة CV_8F_E4M3FN:   قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str8f; اكسر;
                حالة CV_8F_E4M3FNUZ: قدر_منزل_Str = &FormattedImpl::قدر_منزل_Str8fnuz; اكسر;
                افتراضي:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.اونهائي()));
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
                    صف = 0;
                    إذا (mtx.empty())
                        state = STATE_EPILOGUE;
                    وإلا إذا (align_ترتيب)
                        state = STATE_INTERLUDE;
                    وإلا
                        state = STATE_صف_OPEN;
                    أرجع prologue.c_str();
                حالة STATE_INTERLUDE:
                    state = STATE_صف_OPEN;
                    إذا (صف >= mtx.صفون)
                    {
                        إذا (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            أرجع buf;
                        }
                        وإلا
                            صف = 0;
                        snprintf(buf, حجم(buf), "\n(:, :, %d) = \n", cn+1);
                        أرجع buf;
                    }
                    snprintf(buf, حجم(buf), "(:, :, %d) = \n", cn+1);
                    أرجع buf;
                حالة STATE_EPILOGUE:
                    state = STATE_ختم;
                    أرجع epilogue.c_str();
                حالة STATE_صف_OPEN:
                    کانو = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        إذا (صف > 0)
                            طالما(pos < prologue.size() && pos < حجم(buf) - 2)
                                buf[pos++] = ' ';
                        إذا (braces[BRACE_صف_OPEN])
                            buf[pos++] = braces[BRACE_صف_OPEN];
                        إذا(!pos)
                            أرجع next();
                        buf[pos] = 0;
                    }
                    أرجع buf;
                حالة STATE_صف_CLOSE:
                    state = STATE_سٽ_SEPARATOR;
                    ++صف;
                    إذا (braces[BRACE_صف_CLOSE])
                    {
                        buf[0] = braces[BRACE_صف_CLOSE];
                        buf[1] = صف < mtx.صفون ? ',' : '\0';
                        buf[2] = 0;
                        أرجع buf;
                    }
                    وإلا إذا(braces[BRACE_صف_SEP] && صف < mtx.صفون)
                    {
                        buf[0] = braces[BRACE_صف_SEP];
                        buf[1] = 0;
                        أرجع buf;
                    }
                    أرجع next();
                حالة STATE_CN_OPEN:
                    state = STATE_قدر;
                    إذا (!align_ترتيب)
                        cn = 0;
                    إذا (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        أرجع buf;
                    }
                    أرجع next();
                حالة STATE_CN_CLOSE:
                    ++کانو;
                    إذا (کانو >= mtx.کانن_ڳڻپ)
                        state = STATE_صف_CLOSE;
                    وإلا
                        state = STATE_CN_SEPARATOR;
                    إذا (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        أرجع buf;
                    }
                    أرجع next();
                حالة STATE_قدر:
                    (هذا->*قدر_منزل_Str)();
                    state = STATE_CN_CLOSE;
                    إذا (align_ترتيب)
                        أرجع buf;
                    إذا (++cn < mcn)
                        state = STATE_قدر_SEPARATOR;
                    أرجع buf;
                حالة STATE_ختم:
                    أرجع 0;
                حالة STATE_سٽ_SEPARATOR:
                    إذا (صف >= mtx.صفون)
                    {
                        إذا (align_ترتيب)
                            state = STATE_INTERLUDE;
                        وإلا
                            state = STATE_EPILOGUE;
                        أرجع next();
                    }
                    state = STATE_صف_OPEN;
                    buf[0] = single_سٽ ? ' ' : '\n';
                    buf[1] = 0;
                    أرجع buf;
                حالة STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    أرجع buf;
                حالة STATE_قدر_SEPARATOR:
                    state = STATE_قدر;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    أرجع buf;
            }
            أرجع 0;
        }
    };

    صنف Formatter_بنياد : عام Formatter
    {
    عام:
        Formatter_بنياد() : prec16f(4), prec32f(8), prec64f(16), multiline(سچ) {}

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

        فراغ سيٽ_Multiline(منطقي ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    محمي:
        صحيح prec16f;
        صحيح prec32f;
        صحيح prec64f;
        صحيح multiline;
    };

    صنف DefaultFormatter CV_FINAL : عام Formatter_بنياد
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ';', '\0', '\0'};
            أرجع makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.صفون == 1 || !multiline, خطأ, mtx.اونهائي() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف MatlabFormatter CV_FINAL : عام Formatter_بنياد
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ';', '\0', '\0'};
            أرجع makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.صفون == 1 || !multiline, سچ, mtx.اونهائي() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف PythonFormatter CV_FINAL : عام Formatter_بنياد
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'[', ']', ',', '[', ']'};
            إذا (mtx.کانن_ڳڻپ == 1)
                braces[0] = braces[1] = '\0';
            أرجع makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.صفون == 1 || !multiline, خطأ, mtx.اونهائي() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف NumpyFormatter CV_FINAL : عام Formatter_بنياد
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            ساكن ثابت حرف* numpyTypes[CV_اونهائي_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            حرف braces[5] = {'[', ']', ',', '[', ']'};
            إذا (mtx.کانن_ڳڻپ == 1)
                braces[0] = braces[1] = '\0';
            أرجع makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.اونهائي()]), mtx, &*braces,
                mtx.صفون == 1 || !multiline, خطأ, mtx.اونهائي() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف CSVFormatter CV_FINAL : عام Formatter_بنياد
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            أرجع makePtr<FormattedImpl>(String(),
                mtx.صفون > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.صفون == 1 || !multiline, خطأ, mtx.اونهائي() == CV_64F ? prec64f : prec32f );
        }
    };

    صنف CFormatter CV_FINAL : عام Formatter_بنياد
    {
    عام:

        Ptr<Formatted> format(ثابت Mat& mtx) ثابت CV_OVERRIDE
        {
            حرف braces[5] = {'\0', '\0', ',', '\0', '\0'};
            أرجع makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.صفون == 1 || !multiline, خطأ, mtx.اونهائي() == CV_64F ? prec64f : prec32f );
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

    سانچو<قسم_نالو _Tp> بنية Fmt
    {
        تعريفنوع صحيح عارضي_type;
        ساكن ثابت حرف* fmt() { أرجع "%d"; }
    };

    سانچو<> بنية Fmt<uint32_t>
    {
        تعريفنوع بنانشان عارضي_type;
        ساكن ثابت حرف* fmt() { أرجع "%u"; }
    };

    سانچو<> بنية Fmt<int64_t>
    {
        تعريفنوع ڊگهو ڊگهو عارضي_type;
        ساكن ثابت حرف* fmt() { أرجع "%lld"; }
    };

    سانچو<> بنية Fmt<uint64_t>
    {
        تعريفنوع بنانشان ڊگهو ڊگهو عارضي_type;
        ساكن ثابت حرف* fmt() { أرجع "%llu"; }
    };

    سانچو<> بنية Fmt<عائم>
    {
        تعريفنوع عائم عارضي_type;
        ساكن ثابت حرف* fmt() { أرجع "%.5g"; }
    };

    سانچو<> بنية Fmt<مزدوج>
    {
        تعريفنوع مزدوج عارضي_type;
        ساكن ثابت حرف* fmt() { أرجع "%.5g"; }
    };

    سانچو<> بنية Fmt<hfloat>
    {
        تعريفنوع عائم عارضي_type;
        ساكن ثابت حرف* fmt() { أرجع "%.5g"; }
    };

    سانچو<> بنية Fmt<bfloat>
    {
        تعريفنوع عائم عارضي_type;
        ساكن ثابت حرف* fmt() { أرجع "%.4g"; }
    };

    سانچو<> بنية Fmt<fp8_t>
    {
        تعريفنوع عائم عارضي_type;
        ساكن ثابت حرف* fmt() { أرجع "%.4g"; }
    };

    سانچو<> بنية Fmt<fp8a_t>
    {
        تعريفنوع عائم عارضي_type;
        ساكن ثابت حرف* fmt() { أرجع "%.4g"; }
    };

    سانچو <قسم_نالو _Tp>
    ساكن فراغ pprint_صف(std::ostream& strm, ثابت _Tp* ptr, صحيح n, size_t ofs, صحيح ڪنارو)
    {
        حرف buf[128];
        ثابت حرف* fmt = Fmt<_Tp>::fmt();
        صحيح i, ndump = ڪنارو > 0 ? std::min(n, ڪنارو*2+1) : n;
        إذا (ڪنارو == 0)
            ڪنارو = ndump;
        لكل (i = 0; i < ndump; i++) {
            صحيح j = n == ndump || i < ڪنارو ? i : i == ڪنارو ? -1 : n-ڪنارو*2-1+i;
            إذا (i > 0)
                strm << ", ";
            إذا (j >= 0) {
                snprintf(buf, حجم(buf), fmt, (قسم_نالو Fmt<_Tp>::عارضي_type)ptr[ofs + j]);
                strm << buf;
            } وإلا
                strm << "... ";
        }
    }

    ساكن فراغ pprint_ٽڪرو(std::ostream& strm, ثابت Mat& tensor,
                            ثابت size_t* قدم, صحيح d,
                            size_t ofs, صحيح ڪنارو)
    {
        MatShape shape = tensor.shape();
        صحيح ndims = shape.dims;
        صحيح n = d >= ndims ? 1 : shape[d];
        إذا (d >= ndims - 1) {
            صحيح typ = tensor.اونهائي();
            فراغ* معلومات = tensor.معلومات;
            CV_Assert(معلومات);
            n *= tensor.channels();
            إذا (typ == CV_8U)
                pprint_صف(strm, (ثابت uint8_t*)معلومات, n, ofs, ڪنارو);
            وإلا إذا (typ == CV_8S)
                pprint_صف(strm, (ثابت int8_t*)معلومات, n, ofs, ڪنارو);
            وإلا إذا (typ == CV_16U)
                pprint_صف(strm, (ثابت uint16_t*)معلومات, n, ofs, ڪنارو);
            وإلا إذا (typ == CV_16S)
                pprint_صف(strm, (ثابت int16_t*)معلومات, n, ofs, ڪنارو);
            وإلا إذا (typ == CV_32U)
                pprint_صف(strm, (ثابت بنانشان*)معلومات, n, ofs, ڪنارو);
            وإلا إذا (typ == CV_32S)
                pprint_صف(strm, (ثابت صحيح*)معلومات, n, ofs, ڪنارو);
            وإلا إذا (typ == CV_64U)
                pprint_صف(strm, (ثابت uint64_t*)معلومات, n, ofs, ڪنارو);
            وإلا إذا (typ == CV_64S)
                pprint_صف(strm, (ثابت int64_t*)معلومات, n, ofs, ڪنارو);
            وإلا إذا (typ == CV_32F)
                pprint_صف(strm, (ثابت عائم*)معلومات, n, ofs, ڪنارو);
            وإلا إذا (typ == CV_64F)
                pprint_صف(strm, (ثابت مزدوج*)معلومات, n, ofs, ڪنارو);
            وإلا إذا (typ == CV_16F)
                pprint_صف(strm, (ثابت hfloat*)معلومات, n, ofs, ڪنارو);
            وإلا إذا (typ == CV_16BF)
                pprint_صف(strm, (ثابت bfloat*)معلومات, n, ofs, ڪنارو);
            وإلا إذا (typ == CV_Bool)
                pprint_صف(strm, (ثابت منطقي*)معلومات, n, ofs, ڪنارو);
            وإلا إذا (typ == CV_8F_E4M3FN)
                pprint_صف(strm, (ثابت fp8_t*)معلومات, n, ofs, ڪنارو);
            وإلا إذا (typ == CV_8F_E4M3FNUZ)
                pprint_صف(strm, (ثابت fp8a_t*)معلومات, n, ofs, ڪنارو);
            وإلا {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } وإلا {
            صحيح i, ndump = ڪنارو > 0 ? std::min(n, ڪنارو*2+1) : n;
            منطقي dots = خطأ;
            لكل (i = 0; i < ndump; i++) {
                إذا (i > 0 && !dots) {
                    صحيح nempty_سٽون = ndims - 2 - d;
                    لكل (صحيح k = 0; k < nempty_سٽون; k++)
                        strm << "\n";
                }
                إذا (i > 0)
                    strm << "\n";
                صحيح j = n == ndump || i < ڪنارو ? i :
                        i == ڪنارو ? -1 :
                        n - ڪنارو*2 - 1 + i;
                dots = j < 0;
                إذا (!dots)
                    pprint_ٽڪرو(strm, tensor, قدم, d+1, ofs + j*قدم[d], ڪنارو);
                وإلا
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, داخلا_Array array,
                         صحيح /*indent*/, صحيح ڪنارو_2,
                         صحيح سڄو_Tensor_حد,
                         حرف parens)
    {
        حرف oparen = parens;
        حرف cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        صحيح ڪنارو = ڪنارو_2 > 0 ? ڪنارو_2 : 3;
        سڄو_Tensor_حد = سڄو_Tensor_حد > 0 ? سڄو_Tensor_حد : 100;

        Mat tensor = array.getMat();
        إذا (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat عارضي_2;
            tensor.copy_منزل(عارضي_2);
            tensor = عارضي_2;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.ڪل();

        إذا (parens)
            strm << oparen;
        إذا (sz_all == 0) {
            إذا (!parens)
                strm << "<empty>";
        } وإلا {
            إذا (sz_all <= (size_t)سڄو_Tensor_حد)
                ڪنارو = 0;

            صحيح ndims = shape.dims;
            صحيح cn = tensor.channels();
            size_t قدم[MatShape::MAX_DIMS];
            قدم[std::max(ndims-1, 0)] = 1;
            لكل (صحيح i = ndims-2; i >= 0; i--) {
                قدم[i] = قدم[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprint_ٽڪرو(strm, tensor, قدم, 0, 0, ڪنارو);
        }
        إذا (parens)
            strm << cparen;
        أرجع strm;
    }

} // cv
