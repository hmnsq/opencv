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

#அந்தர்பாவ "precomp.hpp"

நாமாகாஶ cv
{
    வகுப்பு FormattedImpl CV_FINAL : பொது Formatted
    {
        எண்ணிக்கை { STATE_PROLOGUE, STATE_EPILOGUE, STATE_INTERLUDE,
               STATE_ROW_OPEN, STATE_ROW_CLOSE, STATE_CN_OPEN, STATE_CN_CLOSE, STATE_மதிப்பு, STATE_FINISHED,
               STATE_வரி_SEPARATOR, STATE_CN_SEPARATOR, STATE_மதிப்பு_SEPARATOR };
        எண்ணிக்கை {BRACE_ROW_OPEN = 0, BRACE_ROW_CLOSE = 1, BRACE_ROW_SEP=2, BRACE_CN_OPEN=3, BRACE_CN_CLOSE=4 };

        எழுத்து floatFormat[8];
        எழுத்து buf[32];   // enough for double with precision up to 20

        Mat mtx;
        முழு mcn; // == mtx.channels()
        தருக்க single_வரி;
        தருக்க alignOrder;    // true when cn first order

        முழு state;
        முழு row;
        முழு col;
        முழு cn;

        String prologue;
        String epilogue;
        எழுத்து braces[5];

        வெறுமை (FormattedImpl::*மதிப்பு_To_Str)();
        வெறுமை மதிப்பு_To_Str_Bool() { snprintf(buf, அளவு(buf), "%d", (முழு)mtx.ptr<uchar>(row, col)[cn] != 0); }
        வெறுமை மதிப்பு_To_Str8u()  { snprintf(buf, அளவு(buf), "%3d", (முழு)mtx.ptr<uchar>(row, col)[cn]); }
        வெறுமை மதிப்பு_To_Str8s()  { snprintf(buf, அளவு(buf), "%3d", (முழு)mtx.ptr<schar>(row, col)[cn]); }
        வெறுமை மதிப்பு_To_Str16u() { snprintf(buf, அளவு(buf), "%d", (முழு)mtx.ptr<ushort>(row, col)[cn]); }
        வெறுமை மதிப்பு_To_Str16s() { snprintf(buf, அளவு(buf), "%d", (முழு)mtx.ptr<short>(row, col)[cn]); }
        வெறுமை மதிப்பு_To_Str32u() { snprintf(buf, அளவு(buf), "%u", mtx.ptr<unsigned>(row, col)[cn]); }
        வெறுமை மதிப்பு_To_Str32s() { snprintf(buf, அளவு(buf), "%d", mtx.ptr<முழு>(row, col)[cn]); }
        வெறுமை மதிப்பு_To_Str32f() { snprintf(buf, அளவு(buf), floatFormat, mtx.ptr<தசம>(row, col)[cn]); }
        வெறுமை மதிப்பு_To_Str64f() { snprintf(buf, அளவு(buf), floatFormat, mtx.ptr<இரட்டை>(row, col)[cn]); }
        வெறுமை மதிப்பு_To_Str64u() { snprintf(buf, அளவு(buf), "%llu", (unsigned long long)mtx.ptr<uint64_t>(row, col)[cn]); }
        வெறுமை மதிப்பு_To_Str64s() { snprintf(buf, அளவு(buf), "%lld", (long long)mtx.ptr<int64_t>(row, col)[cn]); }
        வெறுமை மதிப்பு_To_Str16f() { snprintf(buf, அளவு(buf), floatFormat, (தசம)mtx.ptr<hfloat>(row, col)[cn]); }
        வெறுமை மதிப்பு_To_Str16bf() { snprintf(buf, அளவு(buf), floatFormat, (தசம)mtx.ptr<bfloat>(row, col)[cn]); }
        வெறுமை மதிப்பு_To_Str8f()   { snprintf(buf, அளவு(buf), floatFormat, (தசம)mtx.ptr<fp8_t>(row, col)[cn]); }
        வெறுமை மதிப்பு_To_Str8fnuz() { snprintf(buf, அளவு(buf), floatFormat, (தசம)mtx.ptr<fp8a_t>(row, col)[cn]); }
        வெறுமை மதிப்பு_To_Str_Other() { buf[0] = 0; }

    பொது:

        FormattedImpl(String pl, String el, Mat m, எழுத்து br[5], தருக்க s_வரி, தருக்க aOrder, முழு precision)
        {
            CV_Assert(m.dims <= 2);

            prologue = pl;
            epilogue = el;
            mtx = m;
            mcn = m.channels();
            memcpy(braces, br, 5);
            state = STATE_PROLOGUE;
            single_வரி = s_வரி;
            alignOrder = aOrder;
            row = col = cn =0;

            எனில் (precision < 0)
            {
                floatFormat[0] = '%';
                floatFormat[1] = 'a';
                floatFormat[2] = 0;
            }
            இல்லையெனில்
            {
                cv_snprintf(floatFormat, அளவு(floatFormat), "%%.%dg", std::min(precision, 20));
            }

            தேர்வு(mtx.depth())
            {
                நிலை CV_8U:  மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str8u; நிறுத்து;
                நிலை CV_8S:  மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str8s; நிறுத்து;
                நிலை CV_Bool: மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str_Bool; நிறுத்து;
                நிலை CV_16U: மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str16u; நிறுத்து;
                நிலை CV_16S: மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str16s; நிறுத்து;
                நிலை CV_32U: மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str32u; நிறுத்து;
                நிலை CV_32S: மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str32s; நிறுத்து;
                நிலை CV_32F: மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str32f; நிறுத்து;
                நிலை CV_64F: மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str64f; நிறுத்து;
                நிலை CV_64U: மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str64u; நிறுத்து;
                நிலை CV_64S: மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str64s; நிறுத்து;
                நிலை CV_16F: மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str16f; நிறுத்து;
                நிலை CV_16BF: மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str16bf; நிறுத்து;
                நிலை CV_8F_E4M3FN:   மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str8f; நிறுத்து;
                நிலை CV_8F_E4M3FNUZ: மதிப்பு_To_Str = &FormattedImpl::மதிப்பு_To_Str8fnuz; நிறுத்து;
                இயல்பு:
                    CV_Error_(Error::StsError, ("unsupported matrix type %d\n", mtx.depth()));
            }
        }

        வெறுமை reset() CV_OVERRIDE
        {
            state = STATE_PROLOGUE;
        }

        மாறிலி எழுத்து* next() CV_OVERRIDE
        {
            தேர்வு(state)
            {
                நிலை STATE_PROLOGUE:
                    row = 0;
                    எனில் (mtx.empty())
                        state = STATE_EPILOGUE;
                    இல்லையெனில் எனில் (alignOrder)
                        state = STATE_INTERLUDE;
                    இல்லையெனில்
                        state = STATE_ROW_OPEN;
                    திரும்பு prologue.c_str();
                நிலை STATE_INTERLUDE:
                    state = STATE_ROW_OPEN;
                    எனில் (row >= mtx.rows)
                    {
                        எனில் (++cn >= mcn)
                        {
                            state = STATE_EPILOGUE;
                            buf[0] = 0;
                            திரும்பு buf;
                        }
                        இல்லையெனில்
                            row = 0;
                        snprintf(buf, அளவு(buf), "\n(:, :, %d) = \n", cn+1);
                        திரும்பு buf;
                    }
                    snprintf(buf, அளவு(buf), "(:, :, %d) = \n", cn+1);
                    திரும்பு buf;
                நிலை STATE_EPILOGUE:
                    state = STATE_FINISHED;
                    திரும்பு epilogue.c_str();
                நிலை STATE_ROW_OPEN:
                    col = 0;
                    state = STATE_CN_OPEN;
                    {
                        size_t pos = 0;
                        எனில் (row > 0)
                            போது(pos < prologue.size() && pos < அளவு(buf) - 2)
                                buf[pos++] = ' ';
                        எனில் (braces[BRACE_ROW_OPEN])
                            buf[pos++] = braces[BRACE_ROW_OPEN];
                        எனில்(!pos)
                            திரும்பு next();
                        buf[pos] = 0;
                    }
                    திரும்பு buf;
                நிலை STATE_ROW_CLOSE:
                    state = STATE_வரி_SEPARATOR;
                    ++row;
                    எனில் (braces[BRACE_ROW_CLOSE])
                    {
                        buf[0] = braces[BRACE_ROW_CLOSE];
                        buf[1] = row < mtx.rows ? ',' : '\0';
                        buf[2] = 0;
                        திரும்பு buf;
                    }
                    இல்லையெனில் எனில்(braces[BRACE_ROW_SEP] && row < mtx.rows)
                    {
                        buf[0] = braces[BRACE_ROW_SEP];
                        buf[1] = 0;
                        திரும்பு buf;
                    }
                    திரும்பு next();
                நிலை STATE_CN_OPEN:
                    state = STATE_மதிப்பு;
                    எனில் (!alignOrder)
                        cn = 0;
                    எனில் (mcn > 1 && braces[BRACE_CN_OPEN])
                    {
                        buf[0] = braces[BRACE_CN_OPEN];
                        buf[1] = 0;
                        திரும்பு buf;
                    }
                    திரும்பு next();
                நிலை STATE_CN_CLOSE:
                    ++col;
                    எனில் (col >= mtx.cols)
                        state = STATE_ROW_CLOSE;
                    இல்லையெனில்
                        state = STATE_CN_SEPARATOR;
                    எனில் (mcn > 1 && braces[BRACE_CN_CLOSE])
                    {
                        buf[0] = braces[BRACE_CN_CLOSE];
                        buf[1] = 0;
                        திரும்பு buf;
                    }
                    திரும்பு next();
                நிலை STATE_மதிப்பு:
                    (இது->*மதிப்பு_To_Str)();
                    state = STATE_CN_CLOSE;
                    எனில் (alignOrder)
                        திரும்பு buf;
                    எனில் (++cn < mcn)
                        state = STATE_மதிப்பு_SEPARATOR;
                    திரும்பு buf;
                நிலை STATE_FINISHED:
                    திரும்பு 0;
                நிலை STATE_வரி_SEPARATOR:
                    எனில் (row >= mtx.rows)
                    {
                        எனில் (alignOrder)
                            state = STATE_INTERLUDE;
                        இல்லையெனில்
                            state = STATE_EPILOGUE;
                        திரும்பு next();
                    }
                    state = STATE_ROW_OPEN;
                    buf[0] = single_வரி ? ' ' : '\n';
                    buf[1] = 0;
                    திரும்பு buf;
                நிலை STATE_CN_SEPARATOR:
                    state = STATE_CN_OPEN;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    திரும்பு buf;
                நிலை STATE_மதிப்பு_SEPARATOR:
                    state = STATE_மதிப்பு;
                    buf[0] = ',';
                    buf[1] = ' ';
                    buf[2] = 0;
                    திரும்பு buf;
            }
            திரும்பு 0;
        }
    };

    வகுப்பு FormatterBase : பொது Formatter
    {
    பொது:
        FormatterBase() : prec16f(4), prec32f(8), prec64f(16), multiline(உண்மை) {}

        வெறுமை set16fPrecision(முழு p) CV_OVERRIDE
        {
            prec16f = p;
        }

        வெறுமை set32fPrecision(முழு p) CV_OVERRIDE
        {
            prec32f = p;
        }

        வெறுமை set64fPrecision(முழு p) CV_OVERRIDE
        {
            prec64f = p;
        }

        வெறுமை setMultiline(தருக்க ml) CV_OVERRIDE
        {
            multiline = ml;
        }

    பாதுகாக்கப்பட்ட:
        முழு prec16f;
        முழு prec32f;
        முழு prec64f;
        முழு multiline;
    };

    வகுப்பு DefaultFormatter CV_FINAL : பொது FormatterBase
    {
    பொது:

        Ptr<Formatted> format(மாறிலி Mat& mtx) மாறிலி CV_OVERRIDE
        {
            எழுத்து braces[5] = {'\0', '\0', ';', '\0', '\0'};
            திரும்பு makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, பொய், mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    வகுப்பு MatlabFormatter CV_FINAL : பொது FormatterBase
    {
    பொது:

        Ptr<Formatted> format(மாறிலி Mat& mtx) மாறிலி CV_OVERRIDE
        {
            எழுத்து braces[5] = {'\0', '\0', ';', '\0', '\0'};
            திரும்பு makePtr<FormattedImpl>("", "", mtx, &*braces,
                mtx.rows == 1 || !multiline, உண்மை, mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    வகுப்பு PythonFormatter CV_FINAL : பொது FormatterBase
    {
    பொது:

        Ptr<Formatted> format(மாறிலி Mat& mtx) மாறிலி CV_OVERRIDE
        {
            எழுத்து braces[5] = {'[', ']', ',', '[', ']'};
            எனில் (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            திரும்பு makePtr<FormattedImpl>("[", "]", mtx, &*braces,
                mtx.rows == 1 || !multiline, பொய், mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    வகுப்பு NumpyFormatter CV_FINAL : பொது FormatterBase
    {
    பொது:

        Ptr<Formatted> format(மாறிலி Mat& mtx) மாறிலி CV_OVERRIDE
        {
            நிலையான மாறிலி எழுத்து* numpyTypes[CV_DEPTH_MAX] =
            {
                "uint8", "int8", "uint16", "int16", "int32", "float32", "float64",
                "float16", "bfloat16", "bool", "uint64", "int64", "uint32",
                "float8_e4m3fn", "float8_e4m3fnuz"
            };
            எழுத்து braces[5] = {'[', ']', ',', '[', ']'};
            எனில் (mtx.cols == 1)
                braces[0] = braces[1] = '\0';
            திரும்பு makePtr<FormattedImpl>("array([",
                cv::format("], dtype='%s')", numpyTypes[mtx.depth()]), mtx, &*braces,
                mtx.rows == 1 || !multiline, பொய், mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    வகுப்பு CSVFormatter CV_FINAL : பொது FormatterBase
    {
    பொது:

        Ptr<Formatted> format(மாறிலி Mat& mtx) மாறிலி CV_OVERRIDE
        {
            எழுத்து braces[5] = {'\0', '\0', '\0', '\0', '\0'};
            திரும்பு makePtr<FormattedImpl>(String(),
                mtx.rows > 1 ? String("\n") : String(), mtx, &*braces,
                mtx.rows == 1 || !multiline, பொய், mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    வகுப்பு CFormatter CV_FINAL : பொது FormatterBase
    {
    பொது:

        Ptr<Formatted> format(மாறிலி Mat& mtx) மாறிலி CV_OVERRIDE
        {
            எழுத்து braces[5] = {'\0', '\0', ',', '\0', '\0'};
            திரும்பு makePtr<FormattedImpl>("{", "}", mtx, &*braces,
                mtx.rows == 1 || !multiline, பொய், mtx.depth() == CV_64F ? prec64f : prec32f );
        }
    };

    Formatted::~Formatted() {}
    Formatter::~Formatter() {}

    Ptr<Formatter> Formatter::get(Formatter::FormatType fmt)
    {
        தேர்வு(fmt)
        {
            நிலை FMT_DEFAULT:
                திரும்பு makePtr<DefaultFormatter>();
            நிலை FMT_MATLAB:
                திரும்பு makePtr<MatlabFormatter>();
            நிலை FMT_CSV:
                திரும்பு makePtr<CSVFormatter>();
            நிலை FMT_PYTHON:
                திரும்பு makePtr<PythonFormatter>();
            நிலை FMT_NUMPY:
                திரும்பு makePtr<NumpyFormatter>();
            நிலை FMT_C:
                திரும்பு makePtr<CFormatter>();
        }
        திரும்பு makePtr<DefaultFormatter>();
    }

    template<typename _Tp> கட்டமைப்பு Fmt
    {
        வகைவரை முழு temp_type;
        நிலையான மாறிலி எழுத்து* fmt() { திரும்பு "%d"; }
    };

    template<> கட்டமைப்பு Fmt<uint32_t>
    {
        வகைவரை unsigned temp_type;
        நிலையான மாறிலி எழுத்து* fmt() { திரும்பு "%u"; }
    };

    template<> கட்டமைப்பு Fmt<int64_t>
    {
        வகைவரை long long temp_type;
        நிலையான மாறிலி எழுத்து* fmt() { திரும்பு "%lld"; }
    };

    template<> கட்டமைப்பு Fmt<uint64_t>
    {
        வகைவரை unsigned long long temp_type;
        நிலையான மாறிலி எழுத்து* fmt() { திரும்பு "%llu"; }
    };

    template<> கட்டமைப்பு Fmt<தசம>
    {
        வகைவரை தசம temp_type;
        நிலையான மாறிலி எழுத்து* fmt() { திரும்பு "%.5g"; }
    };

    template<> கட்டமைப்பு Fmt<இரட்டை>
    {
        வகைவரை இரட்டை temp_type;
        நிலையான மாறிலி எழுத்து* fmt() { திரும்பு "%.5g"; }
    };

    template<> கட்டமைப்பு Fmt<hfloat>
    {
        வகைவரை தசம temp_type;
        நிலையான மாறிலி எழுத்து* fmt() { திரும்பு "%.5g"; }
    };

    template<> கட்டமைப்பு Fmt<bfloat>
    {
        வகைவரை தசம temp_type;
        நிலையான மாறிலி எழுத்து* fmt() { திரும்பு "%.4g"; }
    };

    template<> கட்டமைப்பு Fmt<fp8_t>
    {
        வகைவரை தசம temp_type;
        நிலையான மாறிலி எழுத்து* fmt() { திரும்பு "%.4g"; }
    };

    template<> கட்டமைப்பு Fmt<fp8a_t>
    {
        வகைவரை தசம temp_type;
        நிலையான மாறிலி எழுத்து* fmt() { திரும்பு "%.4g"; }
    };

    template <typename _Tp>
    நிலையான வெறுமை pprintRow(std::ostream& strm, மாறிலி _Tp* ptr, முழு n, size_t ofs, முழு edge)
    {
        எழுத்து buf[128];
        மாறிலி எழுத்து* fmt = Fmt<_Tp>::fmt();
        முழு i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
        எனில் (edge == 0)
            edge = ndump;
        க்கு (i = 0; i < ndump; i++) {
            முழு j = n == ndump || i < edge ? i : i == edge ? -1 : n-edge*2-1+i;
            எனில் (i > 0)
                strm << ", ";
            எனில் (j >= 0) {
                snprintf(buf, அளவு(buf), fmt, (typename Fmt<_Tp>::temp_type)ptr[ofs + j]);
                strm << buf;
            } இல்லையெனில்
                strm << "... ";
        }
    }

    நிலையான வெறுமை pprintSlice(std::ostream& strm, மாறிலி Mat& tensor,
                            மாறிலி size_t* step, முழு d,
                            size_t ofs, முழு edge)
    {
        MatShape shape = tensor.shape();
        முழு ndims = shape.dims;
        முழு n = d >= ndims ? 1 : shape[d];
        எனில் (d >= ndims - 1) {
            முழு typ = tensor.depth();
            வெறுமை* data = tensor.data;
            CV_Assert(data);
            n *= tensor.channels();
            எனில் (typ == CV_8U)
                pprintRow(strm, (மாறிலி uint8_t*)data, n, ofs, edge);
            இல்லையெனில் எனில் (typ == CV_8S)
                pprintRow(strm, (மாறிலி int8_t*)data, n, ofs, edge);
            இல்லையெனில் எனில் (typ == CV_16U)
                pprintRow(strm, (மாறிலி uint16_t*)data, n, ofs, edge);
            இல்லையெனில் எனில் (typ == CV_16S)
                pprintRow(strm, (மாறிலி int16_t*)data, n, ofs, edge);
            இல்லையெனில் எனில் (typ == CV_32U)
                pprintRow(strm, (மாறிலி unsigned*)data, n, ofs, edge);
            இல்லையெனில் எனில் (typ == CV_32S)
                pprintRow(strm, (மாறிலி முழு*)data, n, ofs, edge);
            இல்லையெனில் எனில் (typ == CV_64U)
                pprintRow(strm, (மாறிலி uint64_t*)data, n, ofs, edge);
            இல்லையெனில் எனில் (typ == CV_64S)
                pprintRow(strm, (மாறிலி int64_t*)data, n, ofs, edge);
            இல்லையெனில் எனில் (typ == CV_32F)
                pprintRow(strm, (மாறிலி தசம*)data, n, ofs, edge);
            இல்லையெனில் எனில் (typ == CV_64F)
                pprintRow(strm, (மாறிலி இரட்டை*)data, n, ofs, edge);
            இல்லையெனில் எனில் (typ == CV_16F)
                pprintRow(strm, (மாறிலி hfloat*)data, n, ofs, edge);
            இல்லையெனில் எனில் (typ == CV_16BF)
                pprintRow(strm, (மாறிலி bfloat*)data, n, ofs, edge);
            இல்லையெனில் எனில் (typ == CV_Bool)
                pprintRow(strm, (மாறிலி தருக்க*)data, n, ofs, edge);
            இல்லையெனில் எனில் (typ == CV_8F_E4M3FN)
                pprintRow(strm, (மாறிலி fp8_t*)data, n, ofs, edge);
            இல்லையெனில் எனில் (typ == CV_8F_E4M3FNUZ)
                pprintRow(strm, (மாறிலி fp8a_t*)data, n, ofs, edge);
            இல்லையெனில் {
                CV_Error(Error::StsNotImplemented, "unsupported type");
            }
        } இல்லையெனில் {
            முழு i, ndump = edge > 0 ? std::min(n, edge*2+1) : n;
            தருக்க dots = பொய்;
            க்கு (i = 0; i < ndump; i++) {
                எனில் (i > 0 && !dots) {
                    முழு nempty_வரிகள் = ndims - 2 - d;
                    க்கு (முழு k = 0; k < nempty_வரிகள்; k++)
                        strm << "\n";
                }
                எனில் (i > 0)
                    strm << "\n";
                முழு j = n == ndump || i < edge ? i :
                        i == edge ? -1 :
                        n - edge*2 - 1 + i;
                dots = j < 0;
                எனில் (!dots)
                    pprintSlice(strm, tensor, step, d+1, ofs + j*step[d], edge);
                இல்லையெனில்
                    strm << "...";
            }
        }
    }

    std::ostream& pprint(std::ostream& strm, உள்ளீடு_Array array,
                         முழு /*indent*/, முழு edge_,
                         முழு wholeTensorThreshold,
                         எழுத்து parens)
    {
        எழுத்து oparen = parens;
        எழுத்து cparen = parens == '(' ? ')' :
                      parens == '[' ? ']' :
                      parens == '{' ? '}' :
                      parens == '<' ? '>' :
                      parens;
        முழு edge = edge_ > 0 ? edge_ : 3;
        wholeTensorThreshold = wholeTensorThreshold > 0 ? wholeTensorThreshold : 100;

        Mat tensor = array.getMat();
        எனில் (!tensor.isContinuous()) {
            // [TODO] print non-continous arrays without copy
            Mat temp;
            tensor.copyTo(temp);
            tensor = temp;
        }

        MatShape shape = tensor.shape();
        size_t sz_all = tensor.total();

        எனில் (parens)
            strm << oparen;
        எனில் (sz_all == 0) {
            எனில் (!parens)
                strm << "<empty>";
        } இல்லையெனில் {
            எனில் (sz_all <= (size_t)wholeTensorThreshold)
                edge = 0;

            முழு ndims = shape.dims;
            முழு cn = tensor.channels();
            size_t step[MatShape::MAX_DIMS];
            step[std::max(ndims-1, 0)] = 1;
            க்கு (முழு i = ndims-2; i >= 0; i--) {
                step[i] = step[i+1]*shape[i+1]*cn;
                cn = 1;
            }
            pprintSlice(strm, tensor, step, 0, 0, edge);
        }
        எனில் (parens)
            strm << cparen;
        திரும்பு strm;
    }

} // cv
