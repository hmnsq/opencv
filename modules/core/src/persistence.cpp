// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"
#समावेश "persistence.hpp"
#समावेश "persistence_impl.hpp"
#समावेश "persistence_base64_encoding.hpp"
#समावेश <unordered_map>
#समावेश <iterator>

#समावेश <opencv2/core/utils/logger.hpp>

#घोषणा CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

namespace cv
{

namespace fs
{

पूर्णांक strcasecmp(स्थिर अक्षर* s1, स्थिर अक्षर* s2)
{
    स्थिर अक्षर* dummy="";
    अगर(!s1) s1=dummy;
    अगर(!s2) s2=dummy;

    माप_प्रकार len1 = म_माप(s1);
    माप_प्रकार len2 = म_माप(s2);
    माप_प्रकार i, len = std::min(len1, len2);
    क्रम( i = 0; i < len; i++ )
    {
        पूर्णांक d = छोटे((पूर्णांक)s1[i]) - छोटे((पूर्णांक)s2[i]);
        अगर( d != 0 )
            वापस d;
    }
    वापस len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

अक्षर* itoa( पूर्णांक _val, अक्षर* बफर, पूर्णांक /*radix*/ )
{
    स्थिर पूर्णांक radix = 10;
    अक्षर* ptr=बफर + 23 /* enough even for 64-bit integers */;
    अचिन्हित val = असल(_val);

    *ptr = '\0';
    करो
    {
        अचिन्हित r = val / radix;
        *--ptr = (अक्षर)(val - (r*radix) + '0');
        val = r;
    }
    जबतक( val != 0 );

    अगर( _val < 0 )
        *--ptr = '-';

    वापस ptr;
}

अक्षर* itoa( int64_t _val, अक्षर* बफर, पूर्णांक /*radix*/, bool _signed)
{
    स्थिर पूर्णांक radix = 10;
    अक्षर* ptr=बफर + 23 /* enough even for 64-bit integers */;
    पूर्णांक sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : असल(_val);

    *ptr = '\0';
    करो
    {
        uint64_t r = val / radix;
        *--ptr = (अक्षर)(val - (r*radix) + '0');
        val = r;
    }
    जबतक( val != 0 );

    अगर( sign < 0 )
        *--ptr = '-';

    वापस ptr;
}

अक्षर* doubleToString( अक्षर* buf, माप_प्रकार buf_आकार, द्विगुन value, bool explicitZero )
{
    Cv64suf val;
    अचिन्हित ieee754_hi;

    val.f = value;
    ieee754_hi = (अचिन्हित)(val.u >> 32);

    अगर( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        पूर्णांक ivalue = cvRound(value);
        अगर( ivalue == value )
        {
            अगर( explicitZero )
                snprintf( buf, buf_आकार, "%d.0", ivalue );
            अन्यथा
                snprintf( buf, buf_आकार, "%d.", ivalue );
        }
        अन्यथा
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, buf_आकार, "%.17g", value );

            अक्षर* ptr = buf;
            अगर( *ptr == '+' || *ptr == '-' )
                ptr++;
            क्रम( ; cv_isdigit(*ptr); ptr++ )
                ;
            अगर( *ptr == ',' )
                *ptr = '.';
        }
    }
    अन्यथा
    {
        अचिन्हित ieee754_lo = (अचिन्हित)val.u;
        अगर( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            म_नकल( buf, ".Nan" );
        अन्यथा
            म_नकल( buf, (पूर्णांक)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    वापस buf;
}

अक्षर* floatToString( अक्षर* buf, माप_प्रकार buf_आकार, भग्न value, bool halfprecision, bool explicitZero )
{
    Cv32suf val;
    अचिन्हित ieee754;
    val.f = value;
    ieee754 = val.u;

    अगर( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        पूर्णांक ivalue = cvRound(value);
        अगर( ivalue == value )
        {
            अगर( explicitZero )
                snprintf( buf, buf_आकार, "%d.0", ivalue );
            अन्यथा
                snprintf( buf, buf_आकार, "%d.", ivalue );
        }
        अन्यथा
        {
            अगर (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, buf_आकार, "%.5g", value);
            }
            अन्यथा
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, buf_आकार, "%.9g", value);
            }

            अक्षर* ptr = buf;
            अगर( *ptr == '+' || *ptr == '-' )
                ptr++;
            क्रम( ; cv_isdigit(*ptr); ptr++ )
                ;
            अगर( *ptr == ',' )
                *ptr = '.';
        }
    }
    अन्यथा
    {
        अगर( (ieee754 & 0x7fffffff) != 0x7f800000 )
            म_नकल( buf, ".Nan" );
        अन्यथा
            म_नकल( buf, (पूर्णांक)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    वापस buf;
}

अटल स्थिर अक्षर symbols[] = "ucwsifdhHbUIneE";

अटल अक्षर type_प्रतीक(पूर्णांक depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    वापस symbols[depth];
}

अटल पूर्णांक प्रतीक_To_Type(अक्षर c)
{
    अगर (c == 'r')
        वापस CV_SEQ_ELTYPE_PTR;
    स्थिर अक्षर* pos = म_अक्षर( symbols, c );
    अगर( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    वापस static_cast<पूर्णांक>(pos - symbols);
}

अक्षर* encodeFormat(पूर्णांक elem_type, अक्षर* dt, माप_प्रकार dt_len)
{
    पूर्णांक cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    अक्षर प्रतीक = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_प्रतीक(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, प्रतीक);
    वापस dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
अक्षर* encodeFormat(पूर्णांक elem_type, अक्षर* dt)
{
    constexpr माप_प्रकार max = 20+1+1; // UINT64_MAX + one char + nul termination.
    वापस encodeFormat(elem_type, dt, max);
}

पूर्णांक decodeFormat( स्थिर अक्षर* dt, पूर्णांक* fmt_pairs, पूर्णांक max_len )
{
    पूर्णांक fmt_pair_गिनती = 0;
    पूर्णांक i = 0, k = 0, len = dt ? (पूर्णांक)म_माप(dt) : 0;

    अगर( !dt || !len )
        वापस 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    क्रम( ; k < len; k++ )
    {
        अक्षर c = dt[k];

        अगर( cv_isdigit(c) )
        {
            पूर्णांक गिनती_2 = c - '0';
            अगर( cv_isdigit(dt[k+1]) )
            {
                अक्षर* endptr = 0;
                गिनती_2 = (पूर्णांक)म_से_दीर्घ( dt+k, &endptr, 10 );
                k = (पूर्णांक)(endptr - dt) - 1;
            }

            अगर( गिनती_2 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = गिनती_2;
        }
        अन्यथा
        {
            पूर्णांक depth = प्रतीक_To_Type(c);
            अगर( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = depth;
            अगर( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            अन्यथा
            {
                i += 2;
                अगर( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_गिनती = i/2;
    वापस fmt_pair_गिनती;
}

पूर्णांक calc_Elem_आकार( स्थिर अक्षर* dt, पूर्णांक initial_आकार )
{
    पूर्णांक size = 0;
    पूर्णांक fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_गिनती;
    पूर्णांक comp_आकार;

    fmt_pair_गिनती = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_गिनती *= 2;
    क्रम( i = 0, size = initial_आकार; i < fmt_pair_गिनती; i += 2 )
    {
        comp_आकार = CV_ELEM_आकार(fmt_pairs[i+1]);
        size = cvAlign( size, comp_आकार );
        size += comp_आकार * fmt_pairs[i];
    }
    अगर( initial_आकार == 0 )
    {
        comp_आकार = CV_ELEM_आकार(fmt_pairs[1]);
        size = cvAlign( size, comp_आकार );
    }
    वापस size;
}


पूर्णांक calc_Struct_आकार( स्थिर अक्षर* dt, पूर्णांक initial_आकार )
{
    पूर्णांक size = calc_Elem_आकार( dt, initial_आकार );
    माप_प्रकार elem_max_आकार = 0;
    क्रम ( स्थिर अक्षर * type = dt; *type != '\0'; type++ )
    {
        अक्षर v = *type;
        अगर (v >= '0' && v <= '9')
            जारी;  // skip vector size
        चयन (v)
        {
        हाल 'u': { elem_max_आकार = std::max( elem_max_आकार, माप(uchar ) ); अवरोध; }
        हाल 'b': { elem_max_आकार = std::max( elem_max_आकार, माप(bool  ) ); अवरोध; }
        हाल 'c': { elem_max_आकार = std::max( elem_max_आकार, माप(schar ) ); अवरोध; }
        हाल 'w': { elem_max_आकार = std::max( elem_max_आकार, माप(ushort) ); अवरोध; }
        हाल 's': { elem_max_आकार = std::max( elem_max_आकार, माप(लघु ) ); अवरोध; }
        हाल 'i': { elem_max_आकार = std::max( elem_max_आकार, माप(पूर्णांक   ) ); अवरोध; }
        हाल 'n': { elem_max_आकार = std::max( elem_max_आकार, माप(अचिन्हित) ); अवरोध; }
        हाल 'f': { elem_max_आकार = std::max( elem_max_आकार, माप(भग्न ) ); अवरोध; }
        हाल 'd': { elem_max_आकार = std::max( elem_max_आकार, माप(द्विगुन) ); अवरोध; }
        हाल 'h': { elem_max_आकार = std::max( elem_max_आकार, माप(hfloat)); अवरोध; }
        हाल 'H': { elem_max_आकार = std::max( elem_max_आकार, माप(bfloat)); अवरोध; }
        हाल 'e': { elem_max_आकार = std::max( elem_max_आकार, माप(fp8_t) ); अवरोध; }
        हाल 'E': { elem_max_आकार = std::max( elem_max_आकार, माप(fp8a_t)); अवरोध; }
        हाल 'I': { elem_max_आकार = std::max( elem_max_आकार, माप(int64_t)); अवरोध; }
        हाल 'U': { elem_max_आकार = std::max( elem_max_आकार, माप(uint64_t)); अवरोध; }
        शेष:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (अक्षर)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<पूर्णांक>(elem_max_आकार) );
    वापस size;
}

पूर्णांक decodeSimpleFormat( स्थिर अक्षर* dt )
{
    पूर्णांक elem_type = -1;
    पूर्णांक fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_गिनती;

    fmt_pair_गिनती = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    अगर( fmt_pair_गिनती != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    वापस elem_type;
}

}

#अगर defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#घोषणा CV_LITTLE_ENDIAN_MEM_ACCESS 1
#अन्यथा
#घोषणा CV_LITTLE_ENDIAN_MEM_ACCESS 0
#पूर्ण_अगर

अटल अंतरभूत पूर्णांक readInt(स्थिर uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#अगर CV_LITTLE_ENDIAN_MEM_ACCESS
    पूर्णांक val;
    स_नकल(&val, p, माप(val));
    वापस val;
#अन्यथा
    पूर्णांक val = (पूर्णांक)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    वापस val;
#पूर्ण_अगर
}

अटल अंतरभूत int64_t readLong(स्थिर uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#अगर CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    स_नकल(&val, p, माप(val));
    वापस val;
#अन्यथा
    अचिन्हित val0 = (अचिन्हित)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    अचिन्हित val1 = (अचिन्हित)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    वापस val0 | ((int64_t)val1 << 32);
#पूर्ण_अगर
}

अटल अंतरभूत द्विगुन readReal(स्थिर uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#अगर CV_LITTLE_ENDIAN_MEM_ACCESS
    द्विगुन val;
    स_नकल(&val, p, माप(val));
    वापस val;
#अन्यथा
    अचिन्हित val0 = (अचिन्हित)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    अचिन्हित val1 = (अचिन्हित)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    वापस val.f;
#पूर्ण_अगर
}

ढाँचा <typename T>
अटल अंतरभूत व्योम writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#अगर CV_LITTLE_ENDIAN_MEM_ACCESS
    स_नकल(p, &ival, माप(ival));
#अन्यथा
    क्रम (माप_प्रकार i = 0, j = 0; i < माप(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#पूर्ण_अगर
}

अटल अंतरभूत व्योम writeReal(uchar* p, द्विगुन fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#अगर CV_LITTLE_ENDIAN_MEM_ACCESS
    स_नकल(p, &fval, माप(fval));
#अन्यथा
    Cv64suf v;
    v.f = fval;
    p[0] = (uchar)v.u;
    p[1] = (uchar)(v.u >> 8);
    p[2] = (uchar)(v.u >> 16);
    p[3] = (uchar)(v.u >> 24);
    p[4] = (uchar)(v.u >> 32);
    p[5] = (uchar)(v.u >> 40);
    p[6] = (uchar)(v.u >> 48);
    p[7] = (uchar)(v.u >> 56);
#पूर्ण_अगर
}



व्योम FileStorage::Impl::init() {
    flags = 0;
    बफर.सफा();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = false;
    state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = false;
    delayed_struct_कुंजी = nullptr;
    delayed_struct_flags = 0;
    delayed_type_name = nullptr;
    base64_writer = nullptr;
    is_opened = false;
    dummy_eof = false;
    write_mode = false;
    mem_mode = false;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    empty_stream = true;

    strbufv.सफा();
    strbuf = 0;
    strbufsize = strbufpos = 0;
    roots.सफा();

    fs_तथ्य.सफा();
    fs_तथ्य_ptrs.सफा();
    fs_तथ्य_blksz.सफा();
    freeSpaceOfs = 0;

    str_hash.सफा();
    str_hash_तथ्य.सफा();
    str_hash_तथ्य.resize(1);
    str_hash_तथ्य[0] = '\0';

    filename.सफा();
    lineno = 0;
}

FileStorage::Impl::Impl(FileStorage *_fs) {
    fs_ext = _fs;
    init();
}

FileStorage::Impl::~Impl() {
    release();
}

व्योम FileStorage::Impl::release(String *out) {
    अगर (is_opened) {
        अगर (out)
            out->सफा();
        अगर (write_mode) {
            जबतक (write_ढेर.size() > 1) {
                endWriteStruct();
            }
            सफाई();
            अगर (fmt == FileStorage::FORMAT_XML)
                माला_दो("</opencv_storage>\n");
            अन्यथा अगर (fmt == FileStorage::FORMAT_JSON)
                माला_दो("}\n");
        }
        अगर (mem_mode && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

व्योम FileStorage::Impl::analyze_file_name(स्थिर std::string &file_name, std::vector<std::string> &params) {
    params.सफा();
    अटल स्थिर अक्षर not_file_name = '\n';
    अटल स्थिर अक्षर parameter_begin = '?';
    अटल स्थिर अक्षर parameter_separator = '&';

    अगर (file_name.find(not_file_name, (माप_प्रकार) 0) != std::string::npos)
        वापस;

    माप_प्रकार beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((माप_प्रकार) 0, beg));

    अगर (beg != std::string::npos) {
        माप_प्रकार end = file_name.size();
        beg++;
        क्रम (माप_प्रकार param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            अगर ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

bool FileStorage::Impl::खोलो(स्थिर अक्षर *filename_or_buf, पूर्णांक _flags, स्थिर अक्षर *encoding) {
    bool ok = true;
    release();

    bool append = (_flags & 3) == FileStorage::APPEND;
    mem_mode = (_flags & FileStorage::MEMORY) != 0;

    write_mode = (_flags & 3) != 0;
    bool write_base64 = (write_mode || append) && (_flags & FileStorage::BASE64) != 0;

    bool isGZ = false;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        अगर (!params.empty())
            filename = params[0];

        अगर (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_mode || append);
    }

    अगर (filename.size() == 0 && !mem_mode && !write_mode)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    अगर (mem_mode && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    अगर (!mem_mode) {
        माप_प्रकार dot_idx = filename.find_last_of('.');
        अक्षर compression = '\0';

        अगर (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            अगर (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                अगर (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = true;
                }

                अन्यथा अगर(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = true;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        अगर (!isGZ) {
            file = ख_खोलो(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            अगर (!file)
            {
                CV_LOG_ERROR(शून्य, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                वापस false;
            }
        } अन्यथा {
#अगर USE_ZLIB
            अगर (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            अक्षर mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            अगर (!gzfile)
            {
                CV_LOG_ERROR(शून्य, "Can't open archive: '" << filename << "' mode=" << mode);
                वापस false;
            }
#अन्यथा
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#पूर्ण_अगर
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.सफा();
    fs_तथ्य.सफा();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    अगर (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        अगर (mem_mode)
            outbuf.सफा();

        अगर (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            स्थिर अक्षर *dot_pos = शून्य;
            स्थिर अक्षर *dot_pos2 = शून्य;
            // like strrchr() implementation, but save two last positions simultaneously
            क्रम (स्थिर अक्षर *pos = &filename[0]; pos[0] != 0; pos++) {
                अगर (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            अगर (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != शून्य) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } अन्यथा अगर (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        पूर्णांक buf_आकार_2 = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        अगर (append) {
            ख_जाओ(file, 0, अंत_से);
            अगर (ख_बताओ(file) == 0)
                append = false;
        }

        write_ढेर.सफा();
        empty_stream = true;
        write_ढेर.push_back(FStruct_तथ्य("", File_गाँठ::MAP | File_गाँठ::EMPTY, 0));
        बफर.reserve(buf_आकार_2 + 1024);
        बफर.resize(buf_आकार_2);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;

        अगर (fmt == FileStorage::FORMAT_XML) {
            माप_प्रकार file_आकार_2 = file ? (माप_प्रकार) ख_बताओ(file) : (माप_प्रकार) 0;
            अगर (!append || file_आकार_2 == 0) {
                अगर (encoding && *encoding != '\0') {
                    अगर (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(म_माप(encoding) < 1000);
                    अक्षर buf[1100];
                    snprintf(buf, माप(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    माला_दो(buf);
                } अन्यथा
                    माला_दो("<?xml version=\"1.0\"?>\n");
                माला_दो("<opencv_storage>\n");
            } अन्यथा {
                पूर्णांक xml_buf_आकार = 1 << 10;
                अक्षर substr[] = "</opencv_storage>";
                पूर्णांक last_occurrence = -1;
                xml_buf_आकार = MIN(xml_buf_आकार, पूर्णांक(file_आकार_2));
                ख_जाओ(file, -xml_buf_आकार, अंत_से);
                // find the last occurrence of </opencv_storage>
                क्रम (;;) {
                    पूर्णांक पंक्ति_offset = (पूर्णांक) ख_बताओ(file);
                    स्थिर अक्षर *ptr0 = यह->माला_लो(xml_buf_आकार);
                    स्थिर अक्षर *ptr = शून्य;
                    अगर (!ptr0)
                        अवरोध;
                    ptr = ptr0;
                    क्रम (;;) {
                        ptr = म_माला(ptr, substr);
                        अगर (!ptr)
                            अवरोध;
                        last_occurrence = पंक्ति_offset + (पूर्णांक) (ptr - ptr0);
                        ptr += म_माप(substr);
                    }
                }
                अगर (last_occurrence < 0) {
                    release();
                    CV_Error(cv::Error::StsError, "Could not find </opencv_storage> in the end of file.\n");
                }
                closeFile();
                file = ख_खोलो(filename.c_str(), "r+t");
                CV_Assert(file != 0);
                ख_जाओ(file, last_occurrence, शुरू_से);
                // replace the last "</opencv_storage>" with " <!-- resumed -->", which has the same length
                माला_दो(" <!-- resumed -->");
                ख_जाओ(file, 0, अंत_से);
                माला_दो("\n");
            }

            emitter_do_not_use_direct_dereference = createXMLEmitter(यह);
        } अन्यथा अगर (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                अगर (!append) {
                    अगर (fmt == FileStorage::FORMAT_YAML_1_0)
                        माला_दो("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    अन्यथा
                        माला_दो("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            अन्यथा
                माला_दो("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(यह);
        } अन्यथा {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            अगर (!append)
                माला_दो("{\n");
            अन्यथा {
                bool valid = false;
                दीर्घ roffset = 0;
                क्रम (;
                        ख_जाओ(file, roffset, अंत_से) == 0;
                        roffset -= 1) {
                    स्थिर अक्षर end_mark = '}';
                    अगर (ख_अक्षर_लो(file) == end_mark) {
                        ख_जाओ(file, roffset, अंत_से);
                        valid = true;
                        अवरोध;
                    }
                }

                अगर (valid) {
                    closeFile();
                    file = ख_खोलो(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    ख_जाओ(file, roffset, अंत_से);
                    ख_माला_दो(",", file);
                } अन्यथा {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_ढेर.back().indent = 4;
            emitter_do_not_use_direct_dereference = createJSONEmitter(यह);
        }
        is_opened = true;
    } अन्यथा {
        स्थिर माप_प्रकार buf_size0 = 40;
        बफर.resize(buf_size0);
        अगर (mem_mode) {
            strbuf = (अक्षर *) filename_or_buf;
            strbufsize = म_माप(strbuf);
        }

        स्थिर अक्षर *yaml_signature = "%YAML";
        स्थिर अक्षर *json_signature = "{";
        स्थिर अक्षर *xml_signature = "<?xml";
        अक्षर *buf = यह->माला_लो(16);
        CV_Assert(buf);
        अक्षर *bufPtr = cv_skip_BOM(buf);
        माप_प्रकार bufOffset = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        अगर (म_भेदन(bufPtr, yaml_signature, म_माप(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        अन्यथा अगर (म_भेदन(bufPtr, json_signature, म_माप(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        अन्यथा अगर (म_भेदन(bufPtr, xml_signature, म_माप(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        अन्यथा
            fmt = FileStorage::FORMAT_YAML;

        शुरुआत();
        strbufpos = bufOffset;
        bufofs = 0;

        प्रयास {
            अक्षर *ptr = बफर_Start();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_गाँठ मूल_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_गाँठ_Space(मूल_nodes, 9);
            *rptr = File_गाँठ::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.सफा();

            चयन (fmt) {
                हाल FileStorage::FORMAT_XML:
                    parser_do_not_use_direct_dereference = createXMLParser(यह);
                    अवरोध;
                हाल FileStorage::FORMAT_YAML:
                हाल FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_direct_dereference = createYAMLParser(यह);
                    अवरोध;
                हाल FileStorage::FORMAT_JSON:
                    parser_do_not_use_direct_dereference = createJSONParser(यह);
                    अवरोध;
                शेष:
                    parser_do_not_use_direct_dereference = Ptr<FileStorageParser>();
            }

            अगर (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                अगर (ok) {
                    finalizeCollection(मूल_nodes);

                    CV_Assert(!fs_तथ्य_ptrs.empty());
                    File_गाँठ roots_गाँठ(fs_ext, 0, 0);
                    माप_प्रकार i, nroots = roots_गाँठ.size();
                    File_गाँठ_Iterator it = roots_गाँठ.begin();

                    क्रम (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        पकड़ो (...)
        {
            // FIXIT log error message
            is_opened = true;
            release();
            फेंको;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = true;
        std::vector<अक्षर> tmpbuf;
        std::swap(बफर, tmpbuf);
        bufofs = 0;
    }
    वापस ok;
}

व्योम FileStorage::Impl::माला_दो(स्थिर अक्षर *str) {
    CV_Assert(write_mode);
    अगर (mem_mode)
        std::copy(str, str + म_माप(str), std::back_inserter(outbuf));
    अन्यथा अगर (file)
        ख_माला_दो(str, file);
#अगर USE_ZLIB
    अन्यथा अगर (gzfile)
        gzputs(gzfile, str);
#पूर्ण_अगर
    अन्यथा
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

अक्षर *FileStorage::Impl::getsFromFile(अक्षर *buf, पूर्णांक गिनती_2) {
    अगर (file)
        वापस ख_माला_लो(buf, गिनती_2, file);
#अगर USE_ZLIB
    अगर (gzfile)
        वापस gzgets(gzfile, buf, गिनती_2);
#पूर्ण_अगर
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

अक्षर *FileStorage::Impl::माला_लो(माप_प्रकार max_गिनती) {
    अगर (strbuf) {
        माप_प्रकार i = strbufpos, len = strbufsize;
        स्थिर अक्षर *instr = strbuf;
        क्रम (; i < len; i++) {
            अक्षर c = instr[i];
            अगर (c == '\0' || c == '\n') {
                अगर (c == '\n')
                    i++;
                अवरोध;
            }
        }
        माप_प्रकार गिनती_2 = i - strbufpos;
        अगर (max_गिनती == 0 || max_गिनती > गिनती_2)
            max_गिनती = गिनती_2;
        बफर.resize(std::max(बफर.size(), max_गिनती + 8));
        स_नकल(&बफर[0], instr + strbufpos, max_गिनती);
        बफर[max_गिनती] = '\0';
        strbufpos = i;
        वापस max_गिनती > 0 ? &बफर[0] : 0;
    }

    स्थिर माप_प्रकार MAX_BLOCK_आकार = पूर्णांक_उच्च / 2; // hopefully, that will be enough
    अगर (max_गिनती == 0)
        max_गिनती = MAX_BLOCK_आकार;
    अन्यथा
        CV_Assert(max_गिनती < MAX_BLOCK_आकार);
    माप_प्रकार ofs = 0;

    क्रम (;;) {
        पूर्णांक गिनती_2 = (पूर्णांक) std::min(बफर.size() - ofs - 16, max_गिनती);
        अक्षर *ptr = getsFromFile(&बफर[ofs], गिनती_2 + 1);
        अगर (!ptr)
            अवरोध;
        पूर्णांक delta = (पूर्णांक) म_माप(ptr);
        ofs += delta;
        max_गिनती -= delta;
        अगर (delta == 0 || ptr[delta - 1] == '\n' || max_गिनती == 0)
            अवरोध;
        अगर (delta == गिनती_2)
            बफर.resize((माप_प्रकार) (बफर.size() * 1.5));
    }
    वापस ofs > 0 ? &बफर[0] : 0;
}

अक्षर *FileStorage::Impl::माला_लो() {
    अक्षर *ptr = यह->माला_लो(0);
    अगर (!ptr) {
        ptr = बफर_Start();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        वापस 0;
    } अन्यथा {
        माप_प्रकार l = म_माप(ptr);
        अगर (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !खप()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    वापस ptr;
}

bool FileStorage::Impl::खप() {
    अगर (dummy_eof)
        वापस true;
    अगर (strbuf)
        वापस strbufpos >= strbufsize;
    अगर (file)
        वापस ख_पूर्ण(file) != 0;
#अगर USE_ZLIB
    अगर (gzfile)
        वापस gzeof(gzfile) != 0;
#पूर्ण_अगर
    वापस false;
}

व्योम FileStorage::Impl::setEof() {
    dummy_eof = true;
}

व्योम FileStorage::Impl::closeFile() {
    अगर (file)
        ख_बंद(file);
#अगर USE_ZLIB
    अन्यथा अगर (gzfile)
        gzclose(gzfile);
#पूर्ण_अगर
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = false;
}

व्योम FileStorage::Impl::शुरुआत() {
    अगर (file)
        ::शुरुआत(file);
#अगर USE_ZLIB
    अन्यथा अगर (gzfile)
        gzrewind(gzfile);
#पूर्ण_अगर
    strbufpos = 0;
}

अक्षर *FileStorage::Impl::resize_Write_बफर(अक्षर *ptr, पूर्णांक len) {
    स्थिर अक्षर *बफर_end = &बफर[0] + बफर.size();
    अगर (ptr + len < बफर_end)
        वापस ptr;

    स्थिर अक्षर *बफर_start = &बफर[0];
    पूर्णांक written_len = (पूर्णांक) (ptr - बफर_start);

    CV_Assert(written_len <= (पूर्णांक) बफर.size());
    पूर्णांक new_आकार_2 = (पूर्णांक) ((बफर_end - बफर_start) * 3 / 2);
    new_आकार_2 = MAX(written_len + len, new_आकार_2);
    बफर.reserve(new_आकार_2 + 256);
    बफर.resize(new_आकार_2);
    bufofs = written_len;
    वापस &बफर[0] + bufofs;
}

अक्षर *FileStorage::Impl::सफाई() {
    अक्षर *बफर_start = &बफर[0];
    अक्षर *ptr = बफर_start + bufofs;

    अगर (ptr > बफर_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        माला_दो(बफर_start);
        bufofs = 0;
    }

    पूर्णांक indent = write_ढेर.back().indent;

    अगर (space != indent) {
        स_रखो(बफर_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = बफर_start + bufofs;

    वापस ptr;
}

व्योम FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(false);
    अगर (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_ढेर.empty());

    FStruct_तथ्य &current_struct = write_ढेर.back();
    अगर (fmt == FileStorage::FORMAT_JSON && !File_गाँठ::isFlow(current_struct.flags) && write_ढेर.size() > 1)
        current_struct.indent = write_ढेर[write_ढेर.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_ढेर.pop_back();
    अगर (!write_ढेर.empty())
        write_ढेर.back().flags &= ~File_गाँठ::EMPTY;
}

व्योम FileStorage::Impl::startWriteStruct_helper(स्थिर अक्षर *कुंजी, पूर्णांक struct_flags,
                                                स्थिर अक्षर *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (File_गाँठ::TYPE_MASK | File_गाँठ::FLOW)) | File_गाँठ::EMPTY;
    अगर (!File_गाँठ::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    अगर (type_name && type_name[0] == '\0')
        type_name = 0;

    FStruct_तथ्य s = getEmitter().startWriteStruct(write_ढेर.back(), कुंजी, struct_flags, type_name);

    write_ढेर.push_back(s);
    माप_प्रकार write_ढेर_आकार = write_ढेर.size();
    अगर (write_ढेर_आकार > 1)
        write_ढेर[write_ढेर_आकार - 2].flags &= ~File_गाँठ::EMPTY;

    अगर (fmt != FileStorage::FORMAT_JSON && !File_गाँठ::isFlow(s.flags))
        सफाई();

    अगर (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_गाँठ::isMap(struct_flags)) {
        getEmitter().ग_लिखो("type_id", type_name, false);
    }
}

व्योम FileStorage::Impl::startWriteStruct(स्थिर अक्षर *कुंजी, पूर्णांक struct_flags,
                                         स्थिर अक्षर *type_name) {
    check_if_write_struct_is_delayed(false);
    अगर (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    अगर (state_of_writing_base64 == FileStorage_API::Uncertain && File_गाँठ::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(कुंजी, struct_flags, type_name);
    } अन्यथा अगर (type_name && स_भेद(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        अगर ((File_गाँठ::TYPE_MASK & struct_flags) != File_गाँठ::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        अन्यथा अगर (state_of_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(कुंजी, struct_flags, "binary");

        अगर (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::InUse);
    } अन्यथा {
        /* Won't output Base64 data */
        अगर (state_of_writing_base64 == FileStorage_API::InUse)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(कुंजी, struct_flags, type_name);

        अगर (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::NotUse);
    }
}

व्योम FileStorage::Impl::writeComment(स्थिर अक्षर *comment, bool eol_comment) {
    CV_Assert(write_mode);
    getEmitter().writeComment(comment, eol_comment);
}

व्योम FileStorage::Impl::start_अग्र_Stream() {
    CV_Assert(write_mode);
    अगर (!empty_stream) {
        जबतक (!write_ढेर.empty())
            endWriteStruct();
        सफाई();
        getEmitter().start_अग्र_Stream();
        empty_stream = true;
        write_ढेर.push_back(FStruct_तथ्य("", File_गाँठ::EMPTY, 0));
        bufofs = 0;
    }
}

व्योम FileStorage::Impl::ग_लिखो(स्थिर String &कुंजी, पूर्णांक value) {
    CV_Assert(write_mode);
    getEmitter().ग_लिखो(कुंजी.c_str(), value);
}

व्योम FileStorage::Impl::ग_लिखो(स्थिर String &कुंजी, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().ग_लिखो(कुंजी.c_str(), value);
}

व्योम FileStorage::Impl::ग_लिखो(स्थिर String &कुंजी, द्विगुन value) {
    CV_Assert(write_mode);
    getEmitter().ग_लिखो(कुंजी.c_str(), value);
}

व्योम FileStorage::Impl::ग_लिखो(स्थिर String &कुंजी, स्थिर String &value) {
    CV_Assert(write_mode);
    getEmitter().ग_लिखो(कुंजी.c_str(), value.c_str(), false);
}

व्योम FileStorage::Impl::write_Raw_तथ्य(स्थिर std::string &dt, स्थिर व्योम *तथ्य_3, माप_प्रकार len) {
    CV_Assert(write_mode);

    अगर (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        write_Raw_तथ्य_Base64(तथ्य_3, len, dt.c_str());
        वापस;
    } अन्यथा अगर (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    माप_प्रकार elem_आकार = fs::calc_Struct_आकार(dt.c_str(), 0);
    CV_Assert(elem_आकार);
    CV_Assert(len % elem_आकार == 0);
    len /= elem_आकार;

    bool explicitZero = fmt == FileStorage::FORMAT_JSON;
    स्थिर uchar *data0 = (स्थिर uchar *) तथ्य_3;
    पूर्णांक fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_गिनती;
    अक्षर buf[256] = "";

    fmt_pair_गिनती = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    अगर (!len)
        वापस;

    अगर (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    अगर (fmt_pair_गिनती == 1) {
        fmt_pairs[0] *= (पूर्णांक) len;
        len = 1;
    }

    क्रम (; len--; data0 += elem_आकार) {
        पूर्णांक offset = 0;
        क्रम (k = 0; k < fmt_pair_गिनती; k++) {
            पूर्णांक i, गिनती_2 = fmt_pairs[k * 2];
            पूर्णांक elem_type = fmt_pairs[k * 2 + 1];
            पूर्णांक elem_आकार_2 = CV_ELEM_आकार(elem_type);
            स्थिर अक्षर *ptr;

            offset = cvAlign(offset, elem_आकार_2);
            स्थिर uchar *तथ्य = data0 + offset;

            क्रम (i = 0; i < गिनती_2; i++) {
                चयन (elem_type) {
                    हाल CV_8U:
                        ptr = fs::itoa(*(uchar *) तथ्य, buf, 10);
                        तथ्य++;
                        अवरोध;
                    हाल CV_Bool:
                        ptr = fs::itoa(*(uchar *) तथ्य != 0, buf, 10);
                        तथ्य++;
                        अवरोध;
                    हाल CV_8S:
                        ptr = fs::itoa(*(अक्षर *) तथ्य, buf, 10);
                        तथ्य++;
                        अवरोध;
                    हाल CV_16U:
                        ptr = fs::itoa(*(ushort *) तथ्य, buf, 10);
                        तथ्य += माप(ushort);
                        अवरोध;
                    हाल CV_16S:
                        ptr = fs::itoa(*(लघु *) तथ्य, buf, 10);
                        तथ्य += माप(लघु);
                        अवरोध;
                    हाल CV_32U:
                        ptr = fs::itoa((int64_t)*(अचिन्हित*) तथ्य, buf, 10, false);
                        तथ्य += माप(अचिन्हित);
                        अवरोध;
                    हाल CV_32S:
                        ptr = fs::itoa(*(पूर्णांक *) तथ्य, buf, 10);
                        तथ्य += माप(पूर्णांक);
                        अवरोध;
                    हाल CV_64U:
                        ptr = fs::itoa(*(uint64_t*) तथ्य, buf, 10, false);
                        तथ्य += माप(uint64_t);
                        अवरोध;
                    हाल CV_64S:
                        ptr = fs::itoa(*(int64_t*) तथ्य, buf, 10, true);
                        तथ्य += माप(int64_t);
                        अवरोध;
                    हाल CV_32F:
                        ptr = fs::floatToString(buf, माप(buf), *(भग्न *) तथ्य, false, explicitZero);
                        तथ्य += माप(भग्न);
                        अवरोध;
                    हाल CV_64F:
                        ptr = fs::doubleToString(buf, माप(buf), *(द्विगुन *) तथ्य, explicitZero);
                        तथ्य += माप(द्विगुन);
                        अवरोध;
                    हाल CV_16F:
                        ptr = fs::floatToString(buf, माप(buf), (भग्न) *(hfloat *) तथ्य, true, explicitZero);
                        तथ्य += माप(hfloat);
                        अवरोध;
                    हाल CV_16BF:
                        ptr = fs::floatToString(buf, माप(buf), (भग्न) *(bfloat *) तथ्य, true, explicitZero);
                        तथ्य += माप(bfloat);
                        अवरोध;
                    हाल CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, माप(buf), (भग्न) *(fp8_t *) तथ्य, true, explicitZero);
                        तथ्य += माप(fp8_t);
                        अवरोध;
                    हाल CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, माप(buf), (भग्न) *(fp8a_t *) तथ्य, true, explicitZero);
                        तथ्य += माप(fp8a_t);
                        अवरोध;
                    शेष:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        वापस;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (पूर्णांक) (तथ्य - data0);
        }
    }
}

व्योम FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(false);

    अगर (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_to_Base64_state(FileStorage_API::Base64State::Uncertain);
}

व्योम FileStorage::Impl::switch_to_Base64_state(FileStorage_API::Base64State new_state) {
    स्थिर अक्षर *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    स्थिर अक्षर *err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    चयन (state_of_writing_base64) {
        हाल FileStorage_API::Base64State::Uncertain:
            चयन (new_state) {
                हाल FileStorage_API::Base64State::InUse:
                {
                    CV_DbgAssert(base64_writer == 0);
                    bool can_indent = (fmt != cv::FileStorage::Mode::FORMAT_JSON);
                    base64_writer = नया base64::Base64Writer(*यह, can_indent);
                    अगर (!can_indent) {
                        अक्षर *ptr = बफर_Ptr();
                        *ptr++ = '\0';
                        माला_दो(बफर_Start());
                        set_बफर_Ptr(बफर_Start());
                        स_रखो(बफर_Start(), 0, static_cast<पूर्णांक>(space));
                        माला_दो("\"$base64$");
                    }
                    अवरोध;
                }
                हाल FileStorage_API::Base64State::Uncertain:
                    अवरोध;
                हाल FileStorage_API::Base64State::NotUse:
                    अवरोध;
                शेष:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    अवरोध;
            }
            अवरोध;
        हाल FileStorage_API::Base64State::InUse:
            चयन (new_state) {
                हाल FileStorage_API::Base64State::InUse:
                हाल FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    अवरोध;
                हाल FileStorage_API::Base64State::Uncertain:
                    मिटाओ base64_writer;
                    base64_writer = 0;
                    अगर ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        माला_दो("\"");
                        set_बफर_Ptr(बफर_Start());
                        सफाई();
                        स_रखो(बफर_Start(), 0, static_cast<पूर्णांक>(space) );
                        set_बफर_Ptr(बफर_Start());
                    }
                    अवरोध;
                शेष:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    अवरोध;
            }
            अवरोध;
        हाल FileStorage_API::Base64State::NotUse:
            चयन (new_state) {
                हाल FileStorage_API::Base64State::InUse:
                हाल FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    अवरोध;
                हाल FileStorage_API::Base64State::Uncertain:
                    अवरोध;
                शेष:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    अवरोध;
            }
            अवरोध;
        शेष:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            अवरोध;
    }

    state_of_writing_base64 = new_state;
}

व्योम FileStorage::Impl::make_write_struct_delayed(स्थिर अक्षर *कुंजी, पूर्णांक struct_flags, स्थिर अक्षर *type_name) {
    CV_Assert(is_write_struct_delayed == false);
    CV_DbgAssert(delayed_struct_कुंजी == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    अगर (कुंजी != nullptr) {
        delayed_struct_कुंजी = नया अक्षर[म_माप(कुंजी) + 1U];
        म_नकल(delayed_struct_कुंजी, कुंजी);
    }

    अगर (type_name != nullptr) {
        delayed_type_name = नया अक्षर[म_माप(type_name) + 1U];
        म_नकल(delayed_type_name, type_name);
    }

    is_write_struct_delayed = true;
}

व्योम FileStorage::Impl::check_if_write_struct_is_delayed(bool change_type_to_base64) {
    अगर (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_कुंजी;
        std::string type_name;
        पूर्णांक struct_flags = delayed_struct_flags;

        अगर (delayed_struct_कुंजी != nullptr && *delayed_struct_कुंजी != '\0') {
            struct_कुंजी.assign(delayed_struct_कुंजी);
        }
        अगर (delayed_type_name != nullptr && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        मिटाओ[] delayed_struct_कुंजी;
        मिटाओ[] delayed_type_name;
        delayed_struct_कुंजी = nullptr;
        delayed_struct_flags = 0;
        delayed_type_name = nullptr;

        is_write_struct_delayed = false;

        /* call */
        अगर (change_type_to_base64) {
            startWriteStruct_helper(struct_कुंजी.c_str(), struct_flags, "binary");
            अगर (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::InUse);
        } अन्यथा {
            startWriteStruct_helper(struct_कुंजी.c_str(), struct_flags, type_name.c_str());
            अगर (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::NotUse);
        }
    }
}

व्योम FileStorage::Impl::write_Raw_तथ्य_Base64(स्थिर व्योम *तथ्य_3, माप_प्रकार len, स्थिर अक्षर *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(true);

    अगर (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } अन्यथा अगर (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->ग_लिखो(तथ्य_3, len, dt);
}

File_गाँठ FileStorage::Impl::get_First_शीर्ष_Level_गाँठ() स्थिर {
    वापस roots.empty() ? File_गाँठ() : roots[0];
}

File_गाँठ FileStorage::Impl::मूल(पूर्णांक streamIdx) स्थिर {
    वापस streamIdx >= 0 && streamIdx < (पूर्णांक) roots.size() ? roots[streamIdx] : File_गाँठ();
}

File_गाँठ FileStorage::Impl::चालक[](स्थिर String &nodename) स्थिर {
    वापस यह->चालक[](nodename.c_str());
}

File_गाँठ FileStorage::Impl::चालक[](स्थिर अक्षर * /*nodename*/) स्थिर {
    वापस File_गाँठ();
}

पूर्णांक FileStorage::Impl::getFormat() स्थिर { वापस fmt; }

अक्षर *FileStorage::Impl::बफर_Ptr() स्थिर { वापस (अक्षर *) (&बफर[0] + bufofs); }

अक्षर *FileStorage::Impl::बफर_Start() स्थिर { वापस (अक्षर *) &बफर[0]; }

अक्षर *FileStorage::Impl::बफर_End() स्थिर { वापस (अक्षर *) (&बफर[0] + बफर.size()); }

व्योम FileStorage::Impl::set_बफर_Ptr(अक्षर *ptr) {
    अक्षर *bufferstart = बफर_Start();
    CV_Assert(ptr >= bufferstart && ptr <= बफर_End());
    bufofs = ptr - bufferstart;
}

पूर्णांक FileStorage::Impl::wrapMargin() स्थिर { वापस wrap_margin; }

FStruct_तथ्य &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_ढेर.empty());
    वापस write_ढेर.back();
}

व्योम FileStorage::Impl::setNonEmpty() {
    empty_stream = false;
}

व्योम FileStorage::Impl::processSpecialDouble(अक्षर *buf, द्विगुन *value, अक्षर **endptr) {
    FileStorage_API *fs = यह;
    अक्षर c = buf[0];
    पूर्णांक inf_hi = 0x7ff00000;

    अगर (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    अगर (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    अगर (बड़े(buf[1]) == 'I' && बड़े(buf[2]) == 'N' && बड़े(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    अन्यथा अगर (बड़े(buf[1]) == 'N' && बड़े(buf[2]) == 'A' && बड़े(buf[3]) == 'N')
        v.u = (uint64) -1;
    अन्यथा
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

द्विगुन FileStorage::Impl::म_से_भग्न(अक्षर *ptr, अक्षर **endptr) {
    द्विगुन fval = ::म_से_भग्न(ptr, endptr);
    अगर (**endptr == '.') {
        अक्षर *dot_pos = *endptr;
        *dot_pos = ',';
        द्विगुन fval2 = ::म_से_भग्न(ptr, endptr);
        *dot_pos = '.';
        अगर (*endptr > dot_pos)
            fval = fval2;
        अन्यथा
            *endptr = dot_pos;
    }

    अगर (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    वापस fval;
}

व्योम FileStorage::Impl::convertToCollection(पूर्णांक type, File_गाँठ &गाँठ) {
    CV_Assert(type == File_गाँठ::SEQ || type == File_गाँठ::MAP);

    पूर्णांक गाँठ_type = गाँठ.type();
    अगर (गाँठ_type == type)
        वापस;

    bool named = गाँठ.isNamed();
    uchar *ptr = गाँठ.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    द्विगुन fval = 0;
    std::string sval;
    bool add_first_scalar = false;

    अगर (गाँठ_type != File_गाँठ::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_गाँठ::SEQ);
        अगर (गाँठ_type == File_गाँठ::INT) {
            ival = readLong(ptr);
            add_first_scalar = true;
        } अन्यथा अगर (गाँठ_type == File_गाँठ::REAL) {
            fval = readReal(ptr);
            add_first_scalar = true;
        } अन्यथा अगर (गाँठ_type == File_गाँठ::STRING) {
            sval = std::string(गाँठ);
            add_first_scalar = true;
        } अन्यथा
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", गाँठ_type));
    }

    ptr = reserve_गाँठ_Space(गाँठ, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_गाँठ::NAMED : 0));
    // name has been copied automatically
    अगर (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    अगर (add_first_scalar)
        add_गाँठ(गाँठ, std::string(), गाँठ_type,
                गाँठ_type == File_गाँठ::INT ? (स्थिर व्योम *) &ival :
                गाँठ_type == File_गाँठ::REAL ? (स्थिर व्योम *) &fval :
                गाँठ_type == File_गाँठ::STRING ? (स्थिर व्योम *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_गाँठ_Space(File_गाँठ &गाँठ, माप_प्रकार sz) {
    bool shrinkBlock = false;
    माप_प्रकार shrinkBlockIdx = 0, shrink_आकार = 0;

    uchar *ptr = 0, *blockEnd = 0;

    अगर (!fs_तथ्य_ptrs.empty()) {
        माप_प्रकार blockIdx = गाँठ.blockIdx;
        माप_प्रकार ofs = गाँठ.ofs;
        CV_Assert(blockIdx == fs_तथ्य_ptrs.size() - 1);
        CV_Assert(ofs <= fs_तथ्य_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_तथ्य_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_तथ्य_ptrs[blockIdx] + ofs;
        blockEnd = fs_तथ्य_ptrs[blockIdx] + fs_तथ्य_blksz[blockIdx];

        CV_Assert(ptr >= fs_तथ्य_ptrs[blockIdx] && ptr <= blockEnd);
        अगर (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            वापस ptr;
        }

        अगर (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_तथ्य[blockIdx]->resize(sz);
            ptr = &fs_तथ्य[blockIdx]->at(0);
            fs_तथ्य_ptrs[blockIdx] = ptr;
            fs_तथ्य_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            वापस ptr;
        }

        shrinkBlock = true;
        shrinkBlockIdx = blockIdx;
        shrink_आकार = ofs;
    }

    माप_प्रकार block_आकार = std::max((माप_प्रकार) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(block_आकार);
    fs_तथ्य.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_तथ्य_ptrs.push_back(new_ptr);
    fs_तथ्य_blksz.push_back(block_आकार);
    गाँठ.blockIdx = fs_तथ्य_ptrs.size() - 1;
    गाँठ.ofs = 0;
    freeSpaceOfs = sz;

    अगर (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        अगर (ptr[0] & File_गाँठ::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    अगर (shrinkBlock) {
        fs_तथ्य[shrinkBlockIdx]->resize(shrink_आकार);
        fs_तथ्य_blksz[shrinkBlockIdx] = shrink_आकार;
    }

    वापस new_ptr;
}

अचिन्हित FileStorage::Impl::getStringOfs(स्थिर std::string &कुंजी) स्थिर {
    str_hash_t::const_iterator it = str_hash.find(कुंजी);
    वापस it != str_hash.end() ? it->second : 0;
}

File_गाँठ FileStorage::Impl::add_गाँठ(File_गाँठ &collection, स्थिर std::string &कुंजी,
                                    पूर्णांक elem_type, स्थिर व्योम *value, पूर्णांक len) {
    FileStorage_API *fs = यह;
    bool noname = कुंजी.empty() || (fmt == FileStorage::FORMAT_XML && म_भेद(कुंजी.c_str(), "_") == 0);
    convertToCollection(noname ? File_गाँठ::SEQ : File_गाँठ::MAP, collection);

    bool isseq = collection.empty() ? false : collection.isSeq();
    अगर (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    अचिन्हित strofs = 0;
    अगर (!noname) {
        strofs = getStringOfs(कुंजी);
        अगर (!strofs) {
            strofs = (अचिन्हित) str_hash_तथ्य.size();
            माप_प्रकार keysize = कुंजी.size() + 1;
            str_hash_तथ्य.resize(strofs + keysize);
            स_नकल(&str_hash_तथ्य[0] + strofs, &कुंजी[0], keysize);
            str_hash.insert(std::make_pair(कुंजी, strofs));
        }
    }

    uchar *cp = collection.ptr();

    माप_प्रकार blockIdx = fs_तथ्य_ptrs.size() - 1;
    माप_प्रकार ofs = freeSpaceOfs;
    File_गाँठ गाँठ(fs_ext, blockIdx, ofs);

    माप_प्रकार sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_गाँठ_Space(गाँठ, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_गाँठ::NAMED));
    अगर (elem_type == File_गाँठ::NONE)
        freeSpaceOfs -= 8;

    अगर (!noname) {
        writeInt(ptr, (पूर्णांक) strofs);
        ptr += 4;
    }

    अगर (elem_type == File_गाँठ::SEQ || elem_type == File_गाँठ::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    अगर (value)
        गाँठ.set_मूल्य(elem_type, value, len);

    अगर (collection.isNamed())
        cp += 4;
    पूर्णांक nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    वापस गाँठ;
}

व्योम FileStorage::Impl::finalizeCollection(File_गाँठ &collection) {
    अगर (!collection.isSeq() && !collection.isMap())
        वापस;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    अगर (*ptr0 & File_गाँठ::NAMED)
        ptr += 4;
    माप_प्रकार blockIdx = collection.blockIdx;
    माप_प्रकार ofs = collection.ofs + (माप_प्रकार) (ptr + 8 - ptr0);
    माप_प्रकार raw_आकार = 4;
    अचिन्हित sz = (अचिन्हित) readInt(ptr + 4);
    अगर (sz > 0) {
        माप_प्रकार lastBlockIdx = fs_तथ्य_ptrs.size() - 1;

        क्रम (; blockIdx < lastBlockIdx; blockIdx++) {
            raw_आकार += fs_तथ्य_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    raw_आकार += freeSpaceOfs - ofs;
    writeInt(ptr, (पूर्णांक) raw_आकार);
}

व्योम FileStorage::Impl::normalize_गाँठ_Ofs(माप_प्रकार &blockIdx, माप_प्रकार &ofs) स्थिर {
    जबतक (ofs >= fs_तथ्य_blksz[blockIdx]) {
        अगर (blockIdx == fs_तथ्य_blksz.size() - 1) {
            CV_Assert(ofs == fs_तथ्य_blksz[blockIdx]);
            अवरोध;
        }
        ofs -= fs_तथ्य_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    वापस state_of_writing_base64;
}

पूर्णांक FileStorage::Impl::get_space() {
    वापस space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = true;
}

व्योम FileStorage::Impl::Base64Decoder::init(स्थिर Ptr<FileStorageParser> &_parser, अक्षर *_ptr, पूर्णांक _indent) {
    parser_do_not_use_direct_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.सफा();
    decoded.सफा();
    ofs = 0;
    totalchars = 0;
    eos = false;
}

bool FileStorage::Impl::Base64Decoder::readMore(पूर्णांक needed) {
    अटल स्थिर uchar base64tab[] =
            {
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0, 0, 63,
                    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0,
                    0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 0,
                    0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            };

    अगर (eos)
        वापस false;

    माप_प्रकार sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    क्रम (माप_प्रकार i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    अक्षर *beg = 0, *end = 0;
    bool ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    अगर (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = true;
        माप_प्रकार tc = totalchars;
        क्रम (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    पूर्णांक i = 0, j, n = (पूर्णांक) encoded.size();
    अगर (n > 0) {
        स्थिर uchar *tab = base64tab;
        अक्षर *src = &encoded[0];

        क्रम (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(पूर्णांक) (uchar) src[i]], c = tab[(पूर्णांक) (uchar) src[i + 1]];
            uchar b = tab[(पूर्णांक) (uchar) src[i + 2]], a = tab[(पूर्णांक) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    अगर (i > 0 && encoded[i - 1] == '=') {
        अगर (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        अगर (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    क्रम (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    वापस (पूर्णांक) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    माप_प्रकार sz = decoded.size();
    अगर (ofs >= sz && !readMore(1))
        वापस (uchar) 0;
    वापस decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    माप_प्रकार sz = decoded.size();
    अगर (ofs + 2 > sz && !readMore(2))
        वापस (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    वापस val;
}

पूर्णांक FileStorage::Impl::Base64Decoder::getInt32() {
    माप_प्रकार sz = decoded.size();
    अगर (ofs + 4 > sz && !readMore(4))
        वापस 0;
    पूर्णांक ival = readInt(&decoded[ofs]);
    ofs += 4;
    वापस ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    माप_प्रकार sz = decoded.size();
    अगर (ofs + 8 > sz && !readMore(8))
        वापस 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    वापस ival;
}

द्विगुन FileStorage::Impl::Base64Decoder::getFloat64() {
    माप_प्रकार sz = decoded.size();
    अगर (ofs + 8 > sz && !readMore(8))
        वापस 0;
    द्विगुन fval = readReal(&decoded[ofs]);
    ofs += 8;
    वापस fval;
}

bool FileStorage::Impl::Base64Decoder::endOfStream() स्थिर { वापस eos; }

अक्षर *FileStorage::Impl::Base64Decoder::getPtr() स्थिर { वापस ptr; }


अक्षर *FileStorage::Impl::parseBase64(अक्षर *ptr, पूर्णांक indent, File_गाँठ &collection) {
    स्थिर पूर्णांक BASE64_HDR_आकार = 24;
    अक्षर dt[BASE64_HDR_आकार + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    पूर्णांक i, k;

    क्रम (i = 0; i < BASE64_HDR_आकार; i++)
        dt[i] = (अक्षर) base64decoder.getUInt8();
    क्रम (i = 0; i < BASE64_HDR_आकार; i++)
        अगर (है_खाली(dt[i]))
            अवरोध;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    पूर्णांक fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    पूर्णांक fmt_pair_गिनती = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    द्विगुन fval = 0;

    क्रम (;;) {
        क्रम (k = 0; k < fmt_pair_गिनती; k++) {
            पूर्णांक elem_type = fmt_pairs[k * 2 + 1];
            पूर्णांक गिनती_2 = fmt_pairs[k * 2];

            क्रम (i = 0; i < गिनती_2; i++) {
                पूर्णांक गाँठ_type = File_गाँठ::INT;
                चयन (elem_type) {
                    हाल CV_8U:
                        ival = base64decoder.getUInt8();
                        अवरोध;
                    हाल CV_8S:
                        ival = (अक्षर) base64decoder.getUInt8();
                        अवरोध;
                    हाल CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        अवरोध;
                    हाल CV_16U:
                        ival = base64decoder.getUInt16();
                        अवरोध;
                    हाल CV_16S:
                        ival = (लघु) base64decoder.getUInt16();
                        अवरोध;
                    हाल CV_32U:
                        ival = base64decoder.getInt32();
                        अवरोध;
                    हाल CV_32S:
                        ival = base64decoder.getInt32();
                        अवरोध;
                    हाल CV_64U:
                        ival = base64decoder.getInt64();
                        अवरोध;
                    हाल CV_64S:
                        ival = base64decoder.getInt64();
                        अवरोध;
                    हाल CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        गाँठ_type = File_गाँठ::REAL;
                    }
                        अवरोध;
                    हाल CV_64F:
                        fval = base64decoder.getFloat64();
                        गाँठ_type = File_गाँठ::REAL;
                        अवरोध;
                    हाल CV_16F:
                        fval = भग्न(hfloatFromBits(base64decoder.getUInt16()));
                        गाँठ_type = File_गाँठ::REAL;
                        अवरोध;
                    हाल CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        गाँठ_type = File_गाँठ::REAL;
                        अवरोध;
                    हाल CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        गाँठ_type = File_गाँठ::REAL;
                        अवरोध;
                    शेष:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                अगर (base64decoder.endOfStream())
                    अवरोध;
                add_गाँठ(collection, std::string(), गाँठ_type,
                        गाँठ_type == File_गाँठ::INT ? (व्योम *) &ival : (व्योम *) &fval, -1);
            }
        }
        अगर (base64decoder.endOfStream())
            अवरोध;
    }

    finalizeCollection(collection);
    वापस base64decoder.getPtr();
}

व्योम FileStorage::Impl::parseError(स्थिर अक्षर *func_name, स्थिर std::string &err_संदेश, स्थिर अक्षर *source_file,
                                   पूर्णांक source_पंक्ति) {
    std::string संदेश = format("%s(%d): %s", filename.c_str(), lineno, err_संदेश.c_str());
    error(Error::StsParseError, func_name, संदेश.c_str(), source_file, source_पंक्ति);
}

स्थिर uchar *FileStorage::Impl::get_गाँठ_Ptr(माप_प्रकार blockIdx, माप_प्रकार ofs) स्थिर {
    CV_Assert(blockIdx < fs_तथ्य_ptrs.size());
    CV_Assert(ofs < fs_तथ्य_blksz[blockIdx]);

    वापस fs_तथ्य_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(माप_प्रकार nameofs) स्थिर {
    CV_Assert(nameofs < str_hash_तथ्य.size());
    वापस std::string(&str_hash_तथ्य[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { वापस fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(यह);
}

FileStorage::FileStorage(स्थिर String& filename, पूर्णांक flags, स्थिर String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(यह);
    bool ok = p->खोलो(filename.c_str(), flags, encoding.c_str());
    अगर(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

व्योम FileStorage::startWriteStruct(स्थिर String& name, पूर्णांक struct_flags, स्थिर String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    अगर ((struct_flags & File_गाँठ::TYPE_MASK) == File_गाँठ::SEQ)
        state = FileStorage::मूल्य_EXPECTED;
    अन्यथा
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

व्योम FileStorage::endWriteStruct()
{
    p->endWriteStruct();
    state = p->write_ढेर.empty() || File_गाँठ::isMap(p->write_ढेर.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::मूल्य_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

bool FileStorage::खोलो(स्थिर String& filename, पूर्णांक flags, स्थिर String& encoding)
{
    प्रयास
    {
        bool ok = p->खोलो(filename.c_str(), flags, encoding.c_str());
        अगर(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        वापस ok;
    }
    पकड़ो (...)
    {
        release();
        फेंको;  // re-throw
    }
}

bool FileStorage::isOpened() स्थिर { वापस p->is_opened; }

व्योम FileStorage::release()
{
    p->release();
}

File_गाँठ FileStorage::मूल(पूर्णांक i) स्थिर
{
    अगर( p.empty() || p->roots.empty() || i < 0 || i >= (पूर्णांक)p->roots.size() )
        वापस File_गाँठ();

    वापस p->roots[i];
}

File_गाँठ FileStorage::get_First_शीर्ष_Level_गाँठ() स्थिर
{
    File_गाँठ r = मूल();
    File_गाँठ_Iterator it = r.begin();
    वापस it != r.end() ? *it : File_गाँठ();
}

std::string FileStorage::getDefaultObjectName(स्थिर std::string& _filename)
{
    अटल स्थिर अक्षर* stubname = "unnamed";
    स्थिर अक्षर* filename = _filename.c_str();
    स्थिर अक्षर* ptr2 = filename + _filename.size();
    स्थिर अक्षर* ptr = ptr2 - 1;
    cv::Auto_बफर<अक्षर> name_buf(_filename.size()+1);

    जबतक( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        अगर( *ptr == '.' && (!*ptr2 || म_भेदन(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    अगर( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    अक्षर* name = name_buf.तथ्य();

    // name must start with letter or '_'
    अगर( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    जबतक( ptr < ptr2 )
    {
        अक्षर c = *ptr++;
        अगर( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.तथ्य();
    अगर( म_भेद( name, "_" ) == 0 )
        म_नकल( name, stubname );
    वापस name;
}


पूर्णांक FileStorage::getFormat() स्थिर
{
    वापस p->fmt;
}

File_गाँठ FileStorage::चालक [](स्थिर अक्षर* कुंजी) स्थिर
{
    वापस यह->चालक[](std::string(कुंजी));
}

File_गाँठ FileStorage::चालक [](स्थिर std::string& कुंजी) स्थिर
{
    File_गाँठ res;
    क्रम (माप_प्रकार i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][कुंजी];
        अगर (!res.empty())
            अवरोध;
    }
    वापस res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    वापस buf;
}

व्योम FileStorage::writeRaw( स्थिर String& fmt, स्थिर व्योम* vec, माप_प्रकार len )
{
    p->write_Raw_तथ्य(fmt, (स्थिर uchar*)vec, len);
}

व्योम FileStorage::writeComment( स्थिर String& comment, bool eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

व्योम writeScalar( FileStorage& fs, पूर्णांक value )
{
    fs.p->ग_लिखो(String(), value);
}

व्योम writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->ग_लिखो(String(), value);
}

व्योम writeScalar( FileStorage& fs, भग्न value )
{
    fs.p->ग_लिखो(String(), (द्विगुन)value);
}

व्योम writeScalar( FileStorage& fs, द्विगुन value )
{
    fs.p->ग_लिखो(String(), value);
}

व्योम writeScalar( FileStorage& fs, स्थिर String& value )
{
    fs.p->ग_लिखो(String(), value);
}

व्योम ग_लिखो( FileStorage& fs, स्थिर String& name, पूर्णांक value )
{
    fs.p->ग_लिखो(name, value);
}

व्योम ग_लिखो( FileStorage& fs, स्थिर String& name, int64_t value )
{
    fs.p->ग_लिखो(name, value);
}

व्योम ग_लिखो( FileStorage& fs, स्थिर String& name, भग्न value )
{
    fs.p->ग_लिखो(name, (द्विगुन)value);
}

व्योम ग_लिखो( FileStorage& fs, स्थिर String& name, द्विगुन value )
{
    fs.p->ग_लिखो(name, value);
}

व्योम ग_लिखो( FileStorage& fs, स्थिर String& name, स्थिर String& value )
{
    fs.p->ग_लिखो(name, value);
}

व्योम ग_लिखो( FileStorage& fs, स्थिर String& name, bool value )
{
    fs.p->ग_लिखो(name, value);
}

व्योम FileStorage::ग_लिखो(स्थिर String& name, पूर्णांक val) { p->ग_लिखो(name, val); }
व्योम FileStorage::ग_लिखो(स्थिर String& name, bool val) { p->ग_लिखो(name, val); }
व्योम FileStorage::ग_लिखो(स्थिर String& name, int64_t val) { p->ग_लिखो(name, val); }
व्योम FileStorage::ग_लिखो(स्थिर String& name, द्विगुन val) { p->ग_लिखो(name, val); }
व्योम FileStorage::ग_लिखो(स्थिर String& name, स्थिर String& val) { p->ग_लिखो(name, val); }
व्योम FileStorage::ग_लिखो(स्थिर String& name, स्थिर Mat& val) { cv::ग_लिखो(*यह, name, val); }
व्योम FileStorage::ग_लिखो(स्थिर String& name, स्थिर std::vector<String>& val) { cv::ग_लिखो(*यह, name, val); }

FileStorage& चालक << (FileStorage& fs, स्थिर String& str)
{
    क्रमागत { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        मूल्य_EXPECTED = FileStorage::मूल्य_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    स्थिर अक्षर* _str = str.c_str();
    अगर( !fs.isOpened() || !_str )
        वापस fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    अक्षर c = *_str;

    अगर( c == '}' || c == ']' )
    {
        अगर( fs_impl->write_ढेर.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        पूर्णांक struct_flags = fs_impl->write_ढेर.back().flags;
        अक्षर expected_bracket = File_गाँठ::isMap(struct_flags) ? '}' : ']';
        अगर( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_ढेर.empty());
        struct_flags = fs_impl->write_ढेर.back().flags;
        fs.state = File_गाँठ::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : मूल्य_EXPECTED;
        fs.elname = String();
    }
    अन्यथा अगर( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        अगर (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = मूल्य_EXPECTED + INSIDE_MAP;
    }
    अन्यथा अगर( (fs.state & 3) == मूल्य_EXPECTED )
    {
        अगर( c == '{' || c == '[' )
        {
            पूर्णांक struct_flags = c == '{' ? File_गाँठ::MAP : File_गाँठ::SEQ;
            fs.state = struct_flags == File_गाँठ::MAP ? INSIDE_MAP + NAME_EXPECTED : मूल्य_EXPECTED;
            _str++;
            अगर( *_str == ':' )
            {
                _str++;
                अगर( !*_str )
                    struct_flags |= File_गाँठ::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        अन्यथा
        {
            ग_लिखो( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            अगर( fs.state == INSIDE_MAP + मूल्य_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    अन्यथा
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    वापस fs;
}


File_गाँठ::File_गाँठ()
    : fs(शून्य)
{
    blockIdx = ofs = 0;
}

File_गाँठ::File_गाँठ(FileStorage::Impl* _fs, माप_प्रकार _blockIdx, माप_प्रकार _ofs)
    : fs(_fs)
{
    blockIdx = _blockIdx;
    ofs = _ofs;
}

File_गाँठ::File_गाँठ(स्थिर FileStorage* _fs, माप_प्रकार _blockIdx, माप_प्रकार _ofs)
    : File_गाँठ(_fs->p.लो(), _blockIdx, _ofs)
{
    // nothing
}

File_गाँठ::File_गाँठ(स्थिर File_गाँठ& गाँठ)
{
    fs = गाँठ.fs;
    blockIdx = गाँठ.blockIdx;
    ofs = गाँठ.ofs;
}

File_गाँठ& File_गाँठ::चालक=(स्थिर File_गाँठ& गाँठ)
{
    fs = गाँठ.fs;
    blockIdx = गाँठ.blockIdx;
    ofs = गाँठ.ofs;
    वापस *यह;
}

File_गाँठ File_गाँठ::चालक[](स्थिर std::string& nodename) स्थिर
{
    अगर(!fs)
        वापस File_गाँठ();

    CV_Assert( isMap() );

    अचिन्हित कुंजी = fs->getStringOfs(nodename);
    माप_प्रकार i, sz = size();
    File_गाँठ_Iterator it = begin();

    क्रम( i = 0; i < sz; i++, ++it )
    {
        File_गाँठ n = *it;
        स्थिर uchar* p = n.ptr();
        अचिन्हित key2 = (अचिन्हित)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_तथ्य.size() );
        अगर( कुंजी == key2 )
            वापस n;
    }
    वापस File_गाँठ();
}

File_गाँठ File_गाँठ::चालक[](स्थिर अक्षर* nodename) स्थिर
{
    वापस यह->चालक[](std::string(nodename));
}

File_गाँठ File_गाँठ::चालक[](पूर्णांक i) स्थिर
{
    अगर(!fs)
        वापस File_गाँठ();

    CV_Assert( isSeq() );

    पूर्णांक sz = (पूर्णांक)size();
    CV_Assert( 0 <= i && i < sz );

    File_गाँठ_Iterator it = begin();
    it += i;

    वापस *it;
}

std::vector<String> File_गाँठ::कुंजियाँ() स्थिर
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    क्रम (File_गाँठ_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    वापस res;
}

पूर्णांक File_गाँठ::type() स्थिर
{
    स्थिर uchar* p = ptr();
    अगर(!p)
        वापस NONE;
    वापस (*p & TYPE_MASK);
}

bool File_गाँठ::isMap(पूर्णांक flags) { वापस (flags & TYPE_MASK) == MAP; }
bool File_गाँठ::isSeq(पूर्णांक flags) { वापस (flags & TYPE_MASK) == SEQ; }
bool File_गाँठ::isCollection(पूर्णांक flags) { वापस isMap(flags) || isSeq(flags); }
bool File_गाँठ::isFlow(पूर्णांक flags) { वापस (flags & FLOW) != 0; }
bool File_गाँठ::isEmptyCollection(पूर्णांक flags) { वापस (flags & EMPTY) != 0; }

bool File_गाँठ::empty() स्थिर   { वापस fs == 0; }
bool File_गाँठ::isNone() स्थिर  { वापस type() == NONE; }
bool File_गाँठ::isSeq() स्थिर   { वापस type() == SEQ; }
bool File_गाँठ::isMap() स्थिर   { वापस type() == MAP; }
bool File_गाँठ::isInt() स्थिर   { वापस type() == INT;  }
bool File_गाँठ::isReal() स्थिर  { वापस type() == REAL; }
bool File_गाँठ::isString() स्थिर { वापस type() == STRING;  }
bool File_गाँठ::isNamed() स्थिर
{
    स्थिर uchar* p = ptr();
    अगर(!p)
        वापस false;
    वापस (*p & NAMED) != 0;
}

std::string File_गाँठ::name() स्थिर
{
    स्थिर uchar* p = ptr();
    अगर(!p)
        वापस std::string();
    माप_प्रकार nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    वापस fs->getName(nameofs);
}

File_गाँठ::चालक पूर्णांक() स्थिर
{
    स्थिर uchar* p = ptr();
    अगर(!p)
        वापस 0;
    पूर्णांक tag = *p;
    पूर्णांक type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    अगर( type == INT )
    {
        वापस readInt(p);
    }
    अन्यथा अगर( type == REAL )
    {
        वापस cvRound(readReal(p));
    }
    अन्यथा
        वापस 0x7fffffff;
}

File_गाँठ::चालक int64_t() स्थिर
{
    स्थिर uchar* p = ptr();
    अगर(!p)
        वापस 0;
    पूर्णांक tag = *p;
    पूर्णांक type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    अगर( type == INT )
    {
        वापस readLong(p);
    }
    अन्यथा अगर( type == REAL )
    {
        वापस cvRound(readReal(p));
    }
    अन्यथा
        वापस 0x7fffffff;
}

File_गाँठ::चालक भग्न() स्थिर
{
    स्थिर uchar* p = ptr();
    अगर(!p)
        वापस 0.f;
    पूर्णांक tag = *p;
    पूर्णांक type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    अगर( type == INT )
    {
        वापस (भग्न)readLong(p);
    }
    अन्यथा अगर( type == REAL )
    {
        वापस (भग्न)readReal(p);
    }
    अन्यथा
        वापस भग्न_उच्च;
}

File_गाँठ::चालक द्विगुन() स्थिर
{
    स्थिर uchar* p = ptr();
    अगर(!p)
        वापस 0.f;
    पूर्णांक tag = *p;
    पूर्णांक type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    अगर( type == INT )
    {
        वापस (द्विगुन)readLong(p);
    }
    अन्यथा अगर( type == REAL )
    {
        वापस readReal(p);
    }
    अन्यथा
        वापस द्विग_उच्च;
}

द्विगुन File_गाँठ::real() स्थिर  { वापस द्विगुन(*यह); }

std::string File_गाँठ::string() स्थिर
{
    स्थिर uchar* p = ptr();
    अगर( !p || (*p & TYPE_MASK) != STRING )
        वापस std::string();
    p += (*p & NAMED) ? 5 : 1;
    माप_प्रकार sz = (माप_प्रकार)(अचिन्हित)readInt(p);
    वापस std::string((स्थिर अक्षर*)(p + 4), sz - 1);
}
Mat File_गाँठ::mat() स्थिर { Mat value; पढ़ो(*यह, value, Mat()); वापस value; }

File_गाँठ_Iterator File_गाँठ::begin() स्थिर { वापस File_गाँठ_Iterator(*यह, false); }
File_गाँठ_Iterator File_गाँठ::end() स्थिर   { वापस File_गाँठ_Iterator(*यह, true); }

व्योम File_गाँठ::readRaw( स्थिर std::string& fmt, व्योम* vec, माप_प्रकार len ) स्थिर
{
    File_गाँठ_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

माप_प्रकार File_गाँठ::size() स्थिर
{
    स्थिर uchar* p = ptr();
    अगर( !p )
        वापस 0;
    पूर्णांक tag = *p;
    पूर्णांक tp = tag & TYPE_MASK;
    अगर( tp == MAP || tp == SEQ )
    {
        अगर( tag & NAMED )
            p += 4;
        वापस (माप_प्रकार)(अचिन्हित)readInt(p + 5);
    }
    वापस tp != NONE;
}

माप_प्रकार File_गाँठ::raw_आकार() स्थिर
{
    स्थिर uchar* p0 = ptr(), *p = p0;
    अगर( !p )
        वापस 0;
    पूर्णांक tag = *p++;
    पूर्णांक tp = tag & TYPE_MASK;
    अगर( tag & NAMED )
        p += 4;
    माप_प्रकार sz0 = (माप_प्रकार)(p - p0);
    अगर( tp == INT )
        वापस sz0 + 8;
    अगर( tp == REAL )
        वापस sz0 + 8;
    अगर( tp == NONE )
        वापस sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    वापस sz0 + 4 + readInt(p);
}

uchar* File_गाँठ::ptr()
{
    वापस !fs ? 0 : (uchar*)fs->get_गाँठ_Ptr(blockIdx, ofs);
}

स्थिर uchar* File_गाँठ::ptr() स्थिर
{
    वापस !fs ? 0 : fs->get_गाँठ_Ptr(blockIdx, ofs);
}

व्योम File_गाँठ::set_मूल्य( पूर्णांक type, स्थिर व्योम* value, पूर्णांक len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    पूर्णांक tag = *p;
    पूर्णांक current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    पूर्णांक sz = 1;

    अगर( tag & NAMED )
        sz += 4;

    अगर( type == INT )
        sz += 8;
    अन्यथा अगर( type == REAL )
        sz += 8;
    अन्यथा अगर( type == STRING )
    {
        अगर( len < 0 )
            len = (पूर्णांक)म_माप((स्थिर अक्षर*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    अन्यथा
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_गाँठ_Space(*यह, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    अगर( tag & NAMED )
        p += 4;

    अगर( type == INT )
    {
        int64_t ival = *(स्थिर int64_t*)value;
        writeInt(p, ival);
    }
    अन्यथा अगर( type == REAL )
    {
        द्विगुन dbval = *(स्थिर द्विगुन*)value;
        writeReal(p, dbval);
    }
    अन्यथा अगर( type == STRING )
    {
        स्थिर अक्षर* str = (स्थिर अक्षर*)value;
        writeInt(p, len + 1);
        स_नकल(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_गाँठ_Iterator::File_गाँठ_Iterator()
{
    fs = 0;
    blockIdx = 0;
    ofs = 0;
    block_आकार = 0;
    गाँठ_NElems = 0;
    idx = 0;
}

File_गाँठ_Iterator::File_गाँठ_Iterator( स्थिर File_गाँठ& गाँठ, bool seekEnd )
{
    fs = गाँठ.fs;
    idx = 0;
    अगर( !fs )
        blockIdx = ofs = block_आकार = गाँठ_NElems = 0;
    अन्यथा
    {
        blockIdx = गाँठ.blockIdx;
        ofs = गाँठ.ofs;

        bool collection = गाँठ.isSeq() || गाँठ.isMap();
        अगर( गाँठ.isNone() )
        {
            गाँठ_NElems = 0;
        }
        अन्यथा अगर( !collection )
        {
            गाँठ_NElems = 1;
            अगर( seekEnd )
            {
                idx = 1;
                ofs += गाँठ.raw_आकार();
            }
        }
        अन्यथा
        {
            गाँठ_NElems = गाँठ.size();
            स्थिर uchar* p0 = गाँठ.ptr(), *p = p0 + 1;
            अगर(*p0 & File_गाँठ::NAMED )
                p += 4;
            अगर( !seekEnd )
                ofs += (p - p0) + 8;
            अन्यथा
            {
                माप_प्रकार rawsz = (माप_प्रकार)(अचिन्हित)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = गाँठ_NElems;
            }
        }
        fs->normalize_गाँठ_Ofs(blockIdx, ofs);
        block_आकार = fs->fs_तथ्य_blksz[blockIdx];
    }
}

File_गाँठ_Iterator::File_गाँठ_Iterator(स्थिर File_गाँठ_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    block_आकार = it.block_आकार;
    गाँठ_NElems = it.गाँठ_NElems;
    idx = it.idx;
}

File_गाँठ_Iterator& File_गाँठ_Iterator::चालक=(स्थिर File_गाँठ_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    block_आकार = it.block_आकार;
    गाँठ_NElems = it.गाँठ_NElems;
    idx = it.idx;
    वापस *यह;
}

File_गाँठ File_गाँठ_Iterator::चालक *() स्थिर
{
    वापस File_गाँठ(idx < गाँठ_NElems ? fs : शून्य, blockIdx, ofs);
}

File_गाँठ_Iterator& File_गाँठ_Iterator::चालक ++ ()
{
    अगर( idx == गाँठ_NElems || !fs )
        वापस *यह;
    idx++;
    File_गाँठ n(fs, blockIdx, ofs);
    ofs += n.raw_आकार();
    अगर( ofs >= block_आकार )
    {
        fs->normalize_गाँठ_Ofs(blockIdx, ofs);
        block_आकार = fs->fs_तथ्य_blksz[blockIdx];
    }
    वापस *यह;
}

File_गाँठ_Iterator File_गाँठ_Iterator::चालक ++ (पूर्णांक)
{
    File_गाँठ_Iterator it = *यह;
    ++(*यह);
    वापस it;
}

File_गाँठ_Iterator& File_गाँठ_Iterator::चालक += (पूर्णांक _ofs)
{
    CV_Assert( _ofs >= 0 );
    क्रम( ; _ofs > 0; _ofs-- )
        यह->चालक ++();
    वापस *यह;
}

File_गाँठ_Iterator& File_गाँठ_Iterator::readRaw( स्थिर String& fmt, व्योम* _data0, माप_प्रकार maxsz)
{
    अगर( fs && idx < गाँठ_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        पूर्णांक fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        पूर्णांक fmt_pair_गिनती = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        माप_प्रकार esz = fs::calc_Struct_आकार( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        क्रम( ; maxsz > 0; maxsz--, data0 += esz )
        {
            माप_प्रकार offset = 0;
            क्रम( पूर्णांक k = 0; k < fmt_pair_गिनती; k++ )
            {
                पूर्णांक elem_type = fmt_pairs[k*2+1];
                पूर्णांक elem_आकार_2 = CV_ELEM_आकार(elem_type);

                पूर्णांक गिनती_2 = fmt_pairs[k*2];
                offset = align_आकार( offset, elem_आकार_2 );
                uchar* तथ्य = data0 + offset;

                क्रम( पूर्णांक i = 0; i < गिनती_2; i++, ++(*यह) )
                {
                    File_गाँठ गाँठ = *(*यह);
                    अगर( गाँठ.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_आकार_2 == 8 ? (int64_t)गाँठ : (पूर्णांक)गाँठ);
                        चयन( elem_type )
                        {
                        हाल CV_8U:
                            *(uchar*)तथ्य = saturate_cast<uchar>(ival);
                            तथ्य++;
                            अवरोध;
                        हाल CV_8S:
                            *(अक्षर*)तथ्य = saturate_cast<schar>(ival);
                            तथ्य++;
                            अवरोध;
                        हाल CV_Bool:
                            *(bool*)तथ्य = ival != 0;
                            तथ्य++;
                            अवरोध;
                        हाल CV_16U:
                            *(ushort*)तथ्य = saturate_cast<ushort>(ival);
                            तथ्य += माप(ushort);
                            अवरोध;
                        हाल CV_16S:
                            *(लघु*)तथ्य = saturate_cast<लघु>(ival);
                            तथ्य += माप(लघु);
                            अवरोध;
                        हाल CV_32U:
                            *(अचिन्हित*)तथ्य = (अचिन्हित)std::max(ival, (int64_t)0);
                            तथ्य += माप(अचिन्हित);
                            अवरोध;
                        हाल CV_32S:
                            *(पूर्णांक*)तथ्य = (पूर्णांक)ival;
                            तथ्य += माप(पूर्णांक);
                            अवरोध;
                        हाल CV_32F:
                            *(भग्न*)तथ्य = (भग्न)ival;
                            तथ्य += माप(भग्न);
                            अवरोध;
                        हाल CV_64U:
                            *(uint64_t*)तथ्य = (uint64_t)ival;
                            तथ्य += माप(uint64_t);
                            अवरोध;
                        हाल CV_64S:
                            *(int64_t*)तथ्य = (int64_t)ival;
                            तथ्य += माप(int64_t);
                            अवरोध;
                        हाल CV_64F:
                            *(द्विगुन*)तथ्य = (द्विगुन)ival;
                            तथ्य += माप(द्विगुन);
                            अवरोध;
                        हाल CV_16F:
                            *(hfloat*)तथ्य = hfloat((भग्न)ival);
                            तथ्य += माप(hfloat);
                            अवरोध;
                        हाल CV_16BF:
                            *(bfloat*)तथ्य = bfloat((भग्न)ival);
                            तथ्य += माप(bfloat);
                            अवरोध;
                        हाल CV_8F_E4M3FN:
                            *(fp8_t*)तथ्य = fp8_t((भग्न)ival);
                            तथ्य += माप(fp8_t);
                            अवरोध;
                        हाल CV_8F_E4M3FNUZ:
                            *(fp8a_t*)तथ्य = fp8a_t((भग्न)ival);
                            तथ्य += माप(fp8a_t);
                            अवरोध;
                        शेष:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    अन्यथा अगर( गाँठ.isReal() )
                    {
                        द्विगुन fval = (द्विगुन)गाँठ;

                        चयन( elem_type )
                        {
                        हाल CV_8U:
                            *(uchar*)तथ्य = saturate_cast<uchar>(fval);
                            तथ्य++;
                            अवरोध;
                        हाल CV_8S:
                            *(अक्षर*)तथ्य = saturate_cast<schar>(fval);
                            तथ्य++;
                            अवरोध;
                        हाल CV_16U:
                            *(ushort*)तथ्य = saturate_cast<ushort>(fval);
                            तथ्य += माप(ushort);
                            अवरोध;
                        हाल CV_16S:
                            *(लघु*)तथ्य = saturate_cast<लघु>(fval);
                            तथ्य += माप(लघु);
                            अवरोध;
                        हाल CV_32U:
                            *(पूर्णांक*)तथ्य = saturate_cast<अचिन्हित>(fval);
                            तथ्य += माप(पूर्णांक);
                            अवरोध;
                        हाल CV_32S:
                            *(पूर्णांक*)तथ्य = saturate_cast<पूर्णांक>(fval);
                            तथ्य += माप(पूर्णांक);
                            अवरोध;
                        हाल CV_32F:
                            *(भग्न*)तथ्य = (भग्न)fval;
                            तथ्य += माप(भग्न);
                            अवरोध;
                        हाल CV_64U:
                            *(uint64_t*)तथ्य = (uint64_t)round(std::max(fval, 0.));
                            तथ्य += माप(uint64_t);
                            अवरोध;
                        हाल CV_64S:
                            *(int64_t*)तथ्य = (int64_t)round(std::max(fval, 0.));
                            तथ्य += माप(int64_t);
                            अवरोध;
                        हाल CV_64F:
                            *(द्विगुन*)तथ्य = fval;
                            तथ्य += माप(द्विगुन);
                            अवरोध;
                        हाल CV_16F:
                            *(hfloat*)तथ्य = hfloat((भग्न)fval);
                            तथ्य += माप(hfloat);
                            अवरोध;
                        हाल CV_16BF:
                            *(bfloat*)तथ्य = bfloat((भग्न)fval);
                            तथ्य += माप(bfloat);
                            अवरोध;
                        हाल CV_8F_E4M3FN:
                            *(fp8_t*)तथ्य = fp8_t((भग्न)fval);
                            तथ्य += माप(fp8_t);
                            अवरोध;
                        हाल CV_8F_E4M3FNUZ:
                            *(fp8a_t*)तथ्य = fp8a_t((भग्न)fval);
                            तथ्य += माप(fp8a_t);
                            अवरोध;
                        शेष:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    अन्यथा
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (पूर्णांक)(तथ्य - data0);
            }
        }
    }

    वापस *यह;
}

bool File_गाँठ_Iterator::equalTo(स्थिर File_गाँठ_Iterator& it) स्थिर
{
    वापस fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && गाँठ_NElems == it.गाँठ_NElems;
}

माप_प्रकार File_गाँठ_Iterator::remaining() स्थिर
{
    वापस गाँठ_NElems - idx;
}

bool चालक == ( स्थिर File_गाँठ_Iterator& it1, स्थिर File_गाँठ_Iterator& it2 )
{
    वापस it1.equalTo(it2);
}

bool चालक != ( स्थिर File_गाँठ_Iterator& it1, स्थिर File_गाँठ_Iterator& it2 )
{
    वापस !it1.equalTo(it2);
}

व्योम पढ़ो(स्थिर File_गाँठ& गाँठ, पूर्णांक& val, पूर्णांक default_val)
{
    val = default_val;
    अगर( !गाँठ.empty() )
    {
        val = (पूर्णांक)गाँठ;
    }
}

व्योम पढ़ो(स्थिर File_गाँठ& गाँठ, int64_t& val, int64_t default_val)
{
    val = default_val;
    अगर( !गाँठ.empty() )
    {
        val = (int64_t)गाँठ;
    }
}

व्योम पढ़ो(स्थिर File_गाँठ& गाँठ, द्विगुन& val, द्विगुन default_val)
{
    val = default_val;
    अगर( !गाँठ.empty() )
    {
        val = (द्विगुन)गाँठ;
    }
}

व्योम पढ़ो(स्थिर File_गाँठ& गाँठ, भग्न& val, भग्न default_val)
{
    val = default_val;
    अगर( !गाँठ.empty() )
    {
        val = (भग्न)गाँठ;
    }
}

व्योम पढ़ो(स्थिर File_गाँठ& गाँठ, std::string& val, स्थिर std::string& default_val)
{
    val = default_val;
    अगर( !गाँठ.empty() )
    {
        val = (std::string)गाँठ;
    }
}
व्योम FileStorage::Impl::ग_लिखो(स्थिर String &कुंजी, bool value)
{
    CV_Assert(write_mode);
    अगर (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().ग_लिखो(कुंजी.c_str(), (पूर्णांक)value);
    }
    अन्यथा
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().ग_लिखो(कुंजी.c_str(), value ? "true" : "false", false);
    }
}

FileStorage_API::~FileStorage_API() {}

namespace internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, स्थिर std::string& name,
                                       पूर्णांक flags, स्थिर std::string& typeName)
{
    fs = &_fs;
    fs->startWriteStruct(name, flags, typeName);
}

WriteStructContext::~WriteStructContext()
{
    fs->endWriteStruct();
}

}

}
