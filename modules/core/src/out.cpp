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

#అన్తర్భావ "precomp.hpp"

నామాకాశ cv
{
    తరగతి FormattedImpl CV_FINAL : బహిరంగ Formatted
    {
        గణన { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_అడ్డువరుస_OPEN, STATE_అడ్డువరుస_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_విలువ, STATE_ముగిసిన,
               STATE_పంక్తి_SEPARATOR, STATE_CN_SEPARATOR, STATE_విలువ_SEPARATOR };
        గణన {BRACE_అడ్డువరుస_OPEN = 0, BRACE_అడ్డువరుస_CLOSE = 1, BRACE_అడ్డువరుస_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        అక్షరం floatFormat[8];
        అక్షరం buf[32];   // enough for double with precision up to 20

        Mat mtx;
        పూర్ణాంకం mcn; // == mtx.channels()
        తార్కిక single_పంక్తి;
        తార్కిక align_క్రమం;    // true when cn first order

        పూర్ణాంకం state;
        పూర్ణాంకం అడ్డువరుస;
        పూర్ణాంకం నిలువు;
        పూర్ణాంకం cn;

        String prologue;
        String epilogue;
        అక్షరం braces[5];

        శూన్య (FormattedImpl::*విలువ_గమ్యం_Str)();
        శూన్య విలువ_గమ్యం_Str_Bool() { snprintf(buf, పరిమాణం(buf), "%d", (పూర్ణాంకం)mtx.ptr<uchar>(అడ్డువరుస, నిలువు)[cn] != 0); }
        శూన్య విలువ_గమ్యం_Str8u()  { snprintf(buf, పరిమాణం(buf), "%3d", (పూర్ణాంకం)mtx.ptr<uchar>(అడ్డువరుస, నిలువు)[cn]); }
        శూన్య విలువ_గమ్యం_Str8s()  { snprintf(buf, పరిమాణం(buf), "%3d", (పూర్ణాంకం)mtx.ptr<schar>(అడ్డువరుస, నిలువు)[cn]); }
        శూన్య విలువ_గమ్యం_Str16u() { snprintf(buf, పరిమాణం(buf), "%d", (పూర్ణాంకం)mtx.ptr<ushort>(అడ్డువరుస, నిలువు)[cn]); }
        శూన్య విలువ_గమ్యం_Str16s() { snprintf(buf, పరిమాణం(buf), "%d", (పూర్ణాంకం)mtx.ptr<పొట్టి>(అడ్డువరుస, నిలువు)[cn]); }
        శూన్య విలువ_గమ్యం_Str32u() { snprintf(buf, పరిమాణం(buf), "%u", mtx.ptr<గుర్తులేని>(అడ్డువరుస, నిలువు)[cn]); }
        శూన్య విలువ_గమ్యం_Str32s() { snprintf(buf, పరిమాణం(buf), "%d", mtx.ptr<పూర్ణాంకం>(అడ్డువరుస, నిలువు)[cn]); }
        శూన్య విలువ_గమ్యం_Str32f() { snprintf(buf, పరిమాణం(buf), floatFormat, mtx.ptr<దశాంశం>(అడ్డువరుస, నిలువు)[cn]); }
        శూన్య విలువ_గమ్యం_Str64f() { snprintf(buf, పరిమాణం(buf), floatFormat, mtx.ptr<ద్విగుణ>(అడ్డువరుస, నిలువు)[cn]); }
        శూన్య విలువ_గమ్యం_Str64u() { snprintf(buf, పరిమాణం(buf), "%llu", (గుర్తులేని దీర్ఘ దీర్ఘ)mtx.ptr<uint64_t>(అడ్డువరుస, నిలువు)[cn]); }
        శూన్య విలువ_గమ్యం_Str64s() { snprintf(buf, పరిమాణం(buf), "%lld", (దీర్ఘ దీర్ఘ)mtx.ptr<int64_t>(అడ్డువరుస, నిలువు)[cn]); }
        శూన్య విలువ_గమ్యం_Str16f() { snprintf(buf, పరిమాణం(buf), floatFormat, (దశాంశం)mtx.ptr<hfloat>(అడ్డువరుస, నిలువు)[cn]); }
        శూన్య విలువ_గమ్యం_Str16bf() { snprintf(buf, పరిమాణం(buf), floatFormat, (దశాంశం)mtx.ptr<bfloat>(అడ్డువరుస, నిలువు)[cn]); }
        శూన్య విలువ_గమ్యం_Str8f()   { snprintf(buf, పరిమాణం(buf), floatFormat, (దశాంశం)mtx.ptr<fp8_t>(అడ్డువరుస, నిలువు)[cn]); }
        శూన్య విలువ_గమ్యం_Str8fnuz() { snprintf(buf, పరిమాణం(buf), floatFormat, (దశాంశం)mtx.ptr<fp8a_t>(అడ్డువరుస, నిలువు)[cn]); }
        శూన్య విలువ_గమ్యం_Str_Other() { buf[0] = 0; }

    బహిరంగ:

        FormattedImpl(String pl, String el, Mat m, అక్షరం br[5], తార్కిక s_పంక్తి, తార్కిక a_క్రమం, పూర్ణాంకం precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            జ్ఞాపకం_నకలు(braces, br, 5);
            state = STATE_PROLOGUE;
            single_పంక్తి = s_పంక్తి;
            align_క్రమం = a_క్రమం;
            అడ్డువరుస = నిలువు = cn =0;

            ఒకవేళ (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            లేకపోతే
            {
                cv_snprintf(floatFormat, పరిమాణం(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            ఎంపిక(mtx.లోతు())
            {
                స్థితి CV_8U:  విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str8u; ఆపు;
                స్థితి CV_8S:  విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str8s; ఆపు;
                స్థితి CV_Bool: విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str_Bool; ఆపు;
                స్థితి CV_16U: విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str16u; ఆపు;
                స్థితి CV_16S: విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str16s; ఆపు;
                స్థితి CV_32U: విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str32u; ఆపు;
                స్థితి CV_32S: విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str32s; ఆపు;
                స్థితి CV_32F: విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str32f; ఆపు;
                స్థితి CV_64F: విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str64f; ఆపు;
                స్థితి CV_64U: విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str64u; ఆపు;
                స్థితి CV_64S: విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str64s; ఆపు;
                స్థితి CV_16F: విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str16f; ఆపు;
                స్థితి CV_16BF: విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str16bf; ఆపు;
                స్థితి CV_8F_E4M3FN:   విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str8f; ఆపు;
                స్థితి CV_8F_E4M3FNUZ: విలువ_గమ్యం_Str = &FormattedImpl::విలువ_గమ్యం_Str8fnuz; ఆపు;
                అప్రమేయ:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.లోతు()));
            }
        }

        శూన్య reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        స్థిరరాశి అక్షరం* next() CV_OVERRIDE
        {
            ఎంపిక(state)
            {
                స్థితి STATE_PROLOGUE:
                    అడ్డువరుస = 0;
                    ఒకవేళ (mtx.empty())
                        state = STATE_EPILOGUE;
                    లేకపోతే ఒకవేళ (align_క్రమం)
                        state = STATE_INTERLUDE;
                    లేకపోతే
                        state = STATE_అడ్డువరుస_OPEN;
                    తిరిగి prologue.c_str();
                స్థితి STATE_INTERLUDE:
                    state = STATE_అడ్డువరుస_OPEN;
                    ఒకవేళ (అడ్డువరుస >= mtx.అడ్డువరుసలు)
                    {
                        ఒకవేళ (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            తిరిగి buf;
                        }
                        లేకపోతే
                            అడ్డువరుస = 0;
                        snprintf(buf, పరిమాణం(buf), "\n(:, :, %d) = \n", cn+1);
                        తిరిగి buf;
                    }
                    snprintf(buf, పరిమాణం(buf), "(:, :, %d) = \n", cn+1);
                    తిరిగి buf;
                స్థితి STATE_EPILOGUE:
                    state = STATE_ముగిసిన;
                    తిరిగి epilogue.c_str();
                స్థితి STATE_అడ్డువరుస_OPEN:
                    నిలువు = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        ఒకవేళ (అడ్డువరుస > 0)
                            అయితే(pos < prologue.size() && pos < పరిమాణం(buf) - 2)
                                buf[pos++] = ' ';
                        ఒకవేళ (braces[BRACE_అడ్డువరుస_OPEN])
                            buf[pos++] = braces[BRACE_అడ్డువరుస_OPEN];
                        ఒకవేళ(!pos)
                            తిరిగి next();
                        buf[pos] = 0;
                    }
                    తిరిగి buf;
                స్థితి STATE_అడ్డువరుస_CLOSE:
                    state = STATE_పంక్తి_SEPARATOR;
                    ++అడ్డువరుస;
                    ఒకవేళ (braces[BRACE_అడ్డువరుస_CLOSE])
                    {
                        buf[0] = braces[BRACE_అడ్డువరుస_CLOSE];
                        buf[1] = అడ్డువరుస < mtx.అడ్డువరుసలు ? ',' : '\0';
                        buf[2] = 0;
                        తిరిగి buf;
                    }
                    లేకపోతే ఒకవేళ(braces[BRACE_అడ్డువరుస_SEP] && అడ్డువరుస < mtx.అడ్డువరుసలు)
                    {
                        buf[0] = braces[BRACE_అడ్డువరుస_SEP];
                        buf[1] = 0;
                        తిరిగి buf;
                    }
                    తిరిగి next();
                స్థితి STATE_CN_OPEN:
                    state = STATE_విలువ;
                    ఒకవేళ (!align_క్రమం)
                        cn = 0;
                    ఒకవేళ (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        తిరిగి buf;
                    }
                    తిరిగి next();
                స్థితి STATE_CN_CLOSE:
                    ++నిలువు;
                    ఒకవేళ (నిలువు >= mtx.నిలువుల_సంఖ్య)
                        state = STATE_అడ్డువరుస_CLOSE;
                    లేకపోతే
                        state = STATE_CN_SEPARATOR;
                    ఒకవేళ (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        తిరిగి buf;
                    }
                    తిరిగి next();
                స్థితి STATE_విలువ:
                    (ఇది->*విలువ_గమ్యం_Str)();
                    state = STATE_CN_CLOSE;
                    ఒకవేళ (align_క్రమం)
                        తిరిగి buf;
                    ఒకవేళ (++cn < mcn)
                        state = STATE_విలువ_SEPARATOR;
                    తిరిగి buf;
                స్థితి STATE_ముగిసిన:
                    తిరిగి 0;
                స్థితి STATE_పంక్తి_SEPARATOR:
                    ఒకవేళ (అడ్డువరుస >= mtx.అడ్డువరుసలు)
                    {
                        ఒకవేళ (align_క్రమం)
                            state = STATE_INTERLUDE;
                        లేకపోతే
                            state = STATE_EPILOGUE;
                        తిరిగి next();
                    }
                    state = STATE_అడ్డువరుస_OPEN;
                    buf[0] = single_పంక్తి ? ' ' : '\n';
                    buf[1] = 0;
                    తిరిగి buf;
                స్థితి STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    తిరిగి buf;
                స్థితి STATE_విలువ_SEPARATOR:
                    state = STATE_విలువ;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    తిరిగి buf;
            }
            తిరిగి 0;
        }
    };

    తరగతి Formatter_పునాది : బహిరంగ Formatter
    {
    బహిరంగ:
        Formatter_పునాది() : prec16f(4), prec32f(8), prec64f(16), multiline(నిజం) {}

        శూన్య set16fPrecision(పూర్ణాంకం p) CV_OVERRIDE
        {
            prec16f = p;
        }

        శూన్య set32fPrecision(పూర్ణాంకం p) CV_OVERRIDE
        {
            prec32f = p;
        }

        శూన్య set64fPrecision(పూర్ణాంకం p) CV_OVERRIDE
        {
            prec64f = p;
        }

        శూన్య సమితి_Multiline(తార్కిక ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    రక్షిత:
        పూర్ణాంకం prec16f;
        పూర్ణాంకం prec32f;
        పూర్ణాంకం prec64f;
        పూర్ణాంకం multiline;
    };

    తరగతి DefaultFormatter CV_FINAL : బహిరంగ Formatter_పునాది
    {
    బహిరంగ:

        Ptr<Formatted> format(స్థిరరాశి Mat& mtx) స్థిరరాశి CV_OVERRIDE
        {
            అక్షరం braces[5] = {'\0', '\0', ';', '\0', '\0'};
            తిరిగి makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.అడ్డువరుసలు == 1 || !multiline, అబద్ధం, mtx.లోతు() == CV_64F ? prec64f : prec32f );
        }
    };

    తరగతి MatlabFormatter CV_FINAL : బహిరంగ Formatter_పునాది
    {
    బహిరంగ:

        Ptr<Formatted> format(స్థిరరాశి Mat& mtx) స్థిరరాశి CV_OVERRIDE
        {
            అక్షరం braces[5] = {'\0', '\0', ';', '\0', '\0'};
            తిరిగి makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.అడ్డువరుసలు == 1 || !multiline, నిజం, mtx.లోతు() == CV_64F ? prec64f : prec32f );
        }
    };

    తరగతి PythonFormatter CV_FINAL : బహిరంగ Formatter_పునాది
    {
    బహిరంగ:

        Ptr<Formatted> format(స్థిరరాశి Mat& mtx) స్థిరరాశి CV_OVERRIDE
        {
            అక్షరం braces[5] = {'[', ']', ',', '[', ']'};
            ఒకవేళ (mtx.నిలువుల_సంఖ్య == 1)
                braces[0] = braces[1] = '\0';
            తిరిగి makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.అడ్డువరుసలు == 1 || !multiline, అబద్ధం, mtx.లోతు() == CV_64F ? prec64f : prec32f );
        }
    };

    తరగతి NumpyFormatter CV_FINAL : బహిరంగ Formatter_పునాది
    {
    బహిరంగ:

        Ptr<Formatted> format(స్థిరరాశి Mat& mtx) స్థిరరాశి CV_OVERRIDE
        {
            స్థిర స్థిరరాశి అక్షరం* numpyTypes[CV_లోతు_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            అక్షరం braces[5] = {'[', ']', ',', '[', ']'};
            ఒకవేళ (mtx.నిలువుల_సంఖ్య == 1)
                braces[0] = braces[1] = '\0';
            తిరిగి makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.లోతు()]), mtx, &*braces,
                mtx.అడ్డువరుసలు == 1 || !multiline, అబద్ధం, mtx.లోతు() == CV_64F ? prec64f : prec32f );
        }
    };

    తరగతి CSVFormatter CV_FINAL : బహిరంగ Formatter_పునాది
    {
    బహిరంగ:

        Ptr<Formatted> format(స్థిరరాశి Mat& mtx) స్థిరరాశి CV_OVERRIDE
        {
            అక్షరం braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            తిరిగి makePtr<FormattedImpl>(String(),
                mtx.అడ్డువరుసలు > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.అడ్డువరుసలు == 1 || !multiline, అబద్ధం, mtx.లోతు() == CV_64F ? prec64f : prec32f );
        }
    };

    తరగతి CFormatter CV_FINAL : బహిరంగ Formatter_పునాది
    {
    బహిరంగ:

        Ptr<Formatted> format(స్థిరరాశి Mat& mtx) స్థిరరాశి CV_OVERRIDE
        {
            అక్షరం braces[5] = {'\0', '\0', ',', '\0', '\0'};
            తిరిగి makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.అడ్డువరుసలు == 1 || !multiline, అబద్ధం, mtx.లోతు() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        ఎంపిక(fmt)
        {
            స్థితి FMT_DEFAULT:
                తిరిగి makePtr<DefaultFormatter>();
            స్థితి FMT_MATLAB:
                తిరిగి makePtr<MatlabFormatter>();
            స్థితి FMT_CSV:
                తిరిగి makePtr<CSVFormatter>();
            స్థితి FMT_PYTHON:
                తిరిగి makePtr<PythonFormatter>();
            స్థితి FMT_NUMPY:
                తిరిగి makePtr<NumpyFormatter>();
            స్థితి FMT_C:
                తిరిగి makePtr<CFormatter>();
        }
        తిరిగి makePtr<DefaultFormatter>();
    }

    మూస<రకం_పేరు _Tp> నిర్మాణం Fmt
    {
        రకనిర్వచనం పూర్ణాంకం తాత్కాలిక_type;
        స్థిర స్థిరరాశి అక్షరం* fmt() { తిరిగి "%d"; }
    };

    మూస<> నిర్మాణం Fmt<uint32_t>
    {
        రకనిర్వచనం గుర్తులేని తాత్కాలిక_type;
        స్థిర స్థిరరాశి అక్షరం* fmt() { తిరిగి "%u"; }
    };

    మూస<> నిర్మాణం Fmt<int64_t>
    {
        రకనిర్వచనం దీర్ఘ దీర్ఘ తాత్కాలిక_type;
        స్థిర స్థిరరాశి అక్షరం* fmt() { తిరిగి "%lld"; }
    };

    మూస<> నిర్మాణం Fmt<uint64_t>
    {
        రకనిర్వచనం గుర్తులేని దీర్ఘ దీర్ఘ తాత్కాలిక_type;
        స్థిర స్థిరరాశి అక్షరం* fmt() { తిరిగి "%llu"; }
    };

    మూస<> నిర్మాణం Fmt<దశాంశం>
    {
        రకనిర్వచనం దశాంశం తాత్కాలిక_type;
        స్థిర స్థిరరాశి అక్షరం* fmt() { తిరిగి "%.5g"; }
    };

    మూస<> నిర్మాణం Fmt<ద్విగుణ>
    {
        రకనిర్వచనం ద్విగుణ తాత్కాలిక_type;
        స్థిర స్థిరరాశి అక్షరం* fmt() { తిరిగి "%.5g"; }
    };

    మూస<> నిర్మాణం Fmt<hfloat>
    {
        రకనిర్వచనం దశాంశం తాత్కాలిక_type;
        స్థిర స్థిరరాశి అక్షరం* fmt() { తిరిగి "%.5g"; }
    };

    మూస<> నిర్మాణం Fmt<bfloat>
    {
        రకనిర్వచనం దశాంశం తాత్కాలిక_type;
        స్థిర స్థిరరాశి అక్షరం* fmt() { తిరిగి "%.4g"; }
    };

    మూస<> నిర్మాణం Fmt<fp8_t>
    {
        రకనిర్వచనం దశాంశం తాత్కాలిక_type;
        స్థిర స్థిరరాశి అక్షరం* fmt() { తిరిగి "%.4g"; }
    };

    మూస<> నిర్మాణం Fmt<fp8a_t>
    {
        రకనిర్వచనం దశాంశం తాత్కాలిక_type;
        స్థిర స్థిరరాశి అక్షరం* fmt() { తిరిగి "%.4g"; }
    };

    మూస <రకం_పేరు _Tp>
    స్థిర శూన్య pprint_అడ్డువరుస(std::ostream& strm, స్థిరరాశి _Tp* ptr, పూర్ణాంకం n, size_t ofs, పూర్ణాంకం అంచు)
    {
        అక్షరం buf[128];
        స్థిరరాశి అక్షరం* fmt = Fmt<_Tp>::fmt();
        పూర్ణాంకం i, ndump = అంచు > 0 ? std::min(n, అంచు*2+1) : n;
        ఒకవేళ (అంచు == 0)
            అంచు = ndump;
        కొరకు (i = 0; i < ndump; i++) {
            పూర్ణాంకం j = n == ndump || i < అంచు ? i : i == అంచు ? -1 : n-అంచు*2-1+i;
            ఒకవేళ (i > 0)
                strm << ", ";
            ఒకవేళ (j >= 0) {
                snprintf(buf, పరిమాణం(buf), fmt, (రకం_పేరు Fmt<_Tp>::తాత్కాలిక_type)ptr[ofs + j]);
                strm << buf;
            } లేకపోతే
                strm << "... ";
        }
    }

    స్థిర శూన్య pprint_ముక్క(std::ostream& strm, స్థిరరాశి Mat& tensor,
                            స్థిరరాశి size_t* అడుగు, పూర్ణాంకం d,
                            size_t ofs, పూర్ణాంకం అంచు)
    {
        MatShape shape = tensor.shape();
        పూర్ణాంకం ndims = shape.dims;
        పూర్ణాంకం n = d >= ndims ? 1 : shape[d];
        ఒకవేళ (d >= ndims - 1) {
            పూర్ణాంకం typ = tensor.లోతు();
            శూన్య* దత్తాంశం = tensor.దత్తాంశం;
            CV_Assert(దత్తాంశం);
            n *= tensor.channels();
            ఒకవేళ (typ == CV_8U)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి uint8_t*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే ఒకవేళ (typ == CV_8S)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి int8_t*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే ఒకవేళ (typ == CV_16U)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి uint16_t*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే ఒకవేళ (typ == CV_16S)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి int16_t*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే ఒకవేళ (typ == CV_32U)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి గుర్తులేని*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే ఒకవేళ (typ == CV_32S)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి పూర్ణాంకం*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే ఒకవేళ (typ == CV_64U)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి uint64_t*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే ఒకవేళ (typ == CV_64S)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి int64_t*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే ఒకవేళ (typ == CV_32F)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి దశాంశం*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే ఒకవేళ (typ == CV_64F)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి ద్విగుణ*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే ఒకవేళ (typ == CV_16F)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి hfloat*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే ఒకవేళ (typ == CV_16BF)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి bfloat*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే ఒకవేళ (typ == CV_Bool)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి తార్కిక*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే ఒకవేళ (typ == CV_8F_E4M3FN)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి fp8_t*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే ఒకవేళ (typ == CV_8F_E4M3FNUZ)
                pprint_అడ్డువరుస(strm, (స్థిరరాశి fp8a_t*)దత్తాంశం, n, ofs, అంచు);
            లేకపోతే {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } లేకపోతే {
            పూర్ణాంకం i, ndump = అంచు > 0 ? std::min(n, అంచు*2+1) : n;
            తార్కిక dots = అబద్ధం;
            కొరకు (i = 0; i < ndump; i++) {
                ఒకవేళ (i > 0 && !dots) {
                    పూర్ణాంకం nempty_పంక్తులు = ndims - 2 - d;
                    కొరకు (పూర్ణాంకం k = 0; k < nempty_పంక్తులు; k++)
                        strm << "\n";
                }
                ఒకవేళ (i > 0)
                    strm << "\n";
                పూర్ణాంకం j = n == ndump || i < అంచు ? i :
                        i == అంచు ? -1 :
                        n - అంచు*2 - 1 + i;
                dots = j < 0;
                ఒకవేళ (!dots)
                    pprint_ముక్క(strm, tensor, అడుగు, d+1, ofs + j*అడుగు[d], అంచు);
                లేకపోతే
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, ఆదానం_Array array,
                         పూర్ణాంకం /*indent*/, పూర్ణాంకం అంచు_2,
                         పూర్ణాంకం సంపూర్ణ_Tensor_పరిమితి,
                         అక్షరం parens)
    {
        అక్షరం oparen = parens;
        అక్షరం cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        పూర్ణాంకం అంచు = అంచు_2 > 0 ? అంచు_2 : 3;
        సంపూర్ణ_Tensor_పరిమితి = సంపూర్ణ_Tensor_పరిమితి > 0 ? సంపూర్ణ_Tensor_పరిమితి : 100;

        Mat tensor = array.getMat();
        ఒకవేళ (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat తాత్కాలిక_2;
            tensor.copy_గమ్యం(తాత్కాలిక_2);
            tensor = తాత్కాలిక_2;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.మొత్తం();

        ఒకవేళ (parens)
            strm << oparen;
        ఒకవేళ (sz_all == 0) {
            ఒకవేళ (!parens)
                strm << "<empty>";
        } లేకపోతే {
            ఒకవేళ (sz_all <= (size_t)సంపూర్ణ_Tensor_పరిమితి)
                అంచు = 0;

            పూర్ణాంకం ndims = shape.dims;
            పూర్ణాంకం cn = tensor.channels();
            size_t అడుగు[MatShape::MAX_DIMS];
            అడుగు[std::max(ndims-1, 0)] = 1;
            కొరకు (పూర్ణాంకం i = ndims-2; i >= 0; i--) {
                అడుగు[i] = అడుగు[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprint_ముక్క(strm, tensor, అడుగు, 0, 0, అంచు);
        }
        ఒకవేళ (parens)
            strm << cparen;
        తిరిగి strm;
    }

} // cv
