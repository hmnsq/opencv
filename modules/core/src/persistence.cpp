// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "persistence.hpp"
#अन्तर्भाव "persistence_impl.hpp"
#अन्तर्भाव "persistence_base64_encoding.hpp"
#अन्तर्भाव <unordered_map>
#अन्तर्भाव <iterator>

#अन्तर्भाव <opencv2/core/utils/logger.hpp>

#define CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

नामाकाश cv
{

नामाकाश fs
{

पूर्णाङ्क strcasecmp(ध्रुव वर्ण* s1, ध्रुव वर्ण* s2)
{
    ध्रुव वर्ण* dummy="";
    जइ(!s1) s1=dummy;
    जइ(!s2) s2=dummy;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t i, len = std::min(len1, len2);
    प्रति( i = 0; i < len; i++ )
    {
        पूर्णाङ्क d = tolower((पूर्णाङ्क)s1[i]) - tolower((पूर्णाङ्क)s2[i]);
        जइ( d != 0 )
            प्रत्यागम d;
    }
    प्रत्यागम len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

वर्ण* itoa( पूर्णाङ्क _val, वर्ण* buffer, पूर्णाङ्क /*radix*/ )
{
    ध्रुव पूर्णाङ्क radix = 10;
    वर्ण* ptr=buffer + 23 /* enough even for 64-bit integers */;
    unsigned val = abs(_val);

    *ptr = '\0';
    करेहि
    {
        unsigned r = val / radix;
        *--ptr = (वर्ण)(val - (r*radix) + '0');
        val = r;
    }
    जाव( val != 0 );

    जइ( _val < 0 )
        *--ptr = '-';

    प्रत्यागम ptr;
}

वर्ण* itoa( int64_t _val, वर्ण* buffer, पूर्णाङ्क /*radix*/, तार्किक _signed)
{
    ध्रुव पूर्णाङ्क radix = 10;
    वर्ण* ptr=buffer + 23 /* enough even for 64-bit integers */;
    पूर्णाङ्क sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : abs(_val);

    *ptr = '\0';
    करेहि
    {
        uint64_t r = val / radix;
        *--ptr = (वर्ण)(val - (r*radix) + '0');
        val = r;
    }
    जाव( val != 0 );

    जइ( sign < 0 )
        *--ptr = '-';

    प्रत्यागम ptr;
}

वर्ण* doubleToString( वर्ण* buf, size_t bufSize, द्विगुण value, तार्किक explicitZero )
{
    Cv64suf val;
    unsigned ieee754_hi;

    val.f = value;
    ieee754_hi = (unsigned)(val.u >> 32);

    जइ( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        पूर्णाङ्क ivalue = cvRound(value);
        जइ( ivalue == value )
        {
            जइ( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            अण्णहा
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        अण्णहा
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, bufSize, "%.17g", value );

            वर्ण* ptr = buf;
            जइ( *ptr == '+' || *ptr == '-' )
                ptr++;
            प्रति( ; cv_isdigit(*ptr); ptr++ )
                ;
            जइ( *ptr == ',' )
                *ptr = '.';
        }
    }
    अण्णहा
    {
        unsigned ieee754_lo = (unsigned)val.u;
        जइ( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        अण्णहा
            strcpy( buf, (पूर्णाङ्क)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    प्रत्यागम buf;
}

वर्ण* floatToString( वर्ण* buf, size_t bufSize, भिन्न value, तार्किक halfprecision, तार्किक explicitZero )
{
    Cv32suf val;
    unsigned ieee754;
    val.f = value;
    ieee754 = val.u;

    जइ( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        पूर्णाङ्क ivalue = cvRound(value);
        जइ( ivalue == value )
        {
            जइ( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            अण्णहा
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        अण्णहा
        {
            जइ (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.5g", value);
            }
            अण्णहा
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.9g", value);
            }

            वर्ण* ptr = buf;
            जइ( *ptr == '+' || *ptr == '-' )
                ptr++;
            प्रति( ; cv_isdigit(*ptr); ptr++ )
                ;
            जइ( *ptr == ',' )
                *ptr = '.';
        }
    }
    अण्णहा
    {
        जइ( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        अण्णहा
            strcpy( buf, (पूर्णाङ्क)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    प्रत्यागम buf;
}

स्थिर ध्रुव वर्ण symbols[] = "ucwsifdhHbUIneE";

स्थिर वर्ण type_चिह्नम्(पूर्णाङ्क depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    प्रत्यागम symbols[depth];
}

स्थिर पूर्णाङ्क चिह्नम्_To_Type(वर्ण c)
{
    जइ (c == 'r')
        प्रत्यागम CV_SEQ_ELTYPE_PTR;
    ध्रुव वर्ण* pos = strchr( symbols, c );
    जइ( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    प्रत्यागम static_cast<पूर्णाङ्क>(pos - symbols);
}

वर्ण* encodeFormat(पूर्णाङ्क elem_type, वर्ण* dt, size_t dt_len)
{
    पूर्णाङ्क cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    वर्ण चिह्नम् = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_चिह्नम्(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, चिह्नम्);
    प्रत्यागम dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
वर्ण* encodeFormat(पूर्णाङ्क elem_type, वर्ण* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    प्रत्यागम encodeFormat(elem_type, dt, max);
}

पूर्णाङ्क decodeFormat( ध्रुव वर्ण* dt, पूर्णाङ्क* fmt_pairs, पूर्णाङ्क max_len )
{
    पूर्णाङ्क fmt_pair_गणनम् = 0;
    पूर्णाङ्क i = 0, k = 0, len = dt ? (पूर्णाङ्क)strlen(dt) : 0;

    जइ( !dt || !len )
        प्रत्यागम 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    प्रति( ; k < len; k++ )
    {
        वर्ण c = dt[k];

        जइ( cv_isdigit(c) )
        {
            पूर्णाङ्क गणनम्_2 = c - '0';
            जइ( cv_isdigit(dt[k+1]) )
            {
                वर्ण* endptr = 0;
                गणनम्_2 = (पूर्णाङ्क)strtol( dt+k, &endptr, 10 );
                k = (पूर्णाङ्क)(endptr - dt) - 1;
            }

            जइ( गणनम्_2 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = गणनम्_2;
        }
        अण्णहा
        {
            पूर्णाङ्क depth = चिह्नम्_To_Type(c);
            जइ( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = depth;
            जइ( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            अण्णहा
            {
                i += 2;
                जइ( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_गणनम् = i/2;
    प्रत्यागम fmt_pair_गणनम्;
}

पूर्णाङ्क calcElemSize( ध्रुव वर्ण* dt, पूर्णाङ्क initial_size )
{
    पूर्णाङ्क size = 0;
    पूर्णाङ्क fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_गणनम्;
    पूर्णाङ्क comp_size;

    fmt_pair_गणनम् = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_गणनम् *= 2;
    प्रति( i = 0, size = initial_size; i < fmt_pair_गणनम्; i += 2 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[i+1]);
        size = cvAlign( size, comp_size );
        size += comp_size * fmt_pairs[i];
    }
    जइ( initial_size == 0 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[1]);
        size = cvAlign( size, comp_size );
    }
    प्रत्यागम size;
}


पूर्णाङ्क calcStructSize( ध्रुव वर्ण* dt, पूर्णाङ्क initial_size )
{
    पूर्णाङ्क size = calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    प्रति ( ध्रुव वर्ण * type = dt; *type != '\0'; type++ )
    {
        वर्ण v = *type;
        जइ (v >= '0' && v <= '9')
            अनुवर्त;  // skip vector size
        विकल्प (v)
        {
        पक्ष 'u': { elem_max_size = std::max( elem_max_size, परिमाण(uchar ) ); भङ्ग; }
        पक्ष 'b': { elem_max_size = std::max( elem_max_size, परिमाण(तार्किक  ) ); भङ्ग; }
        पक्ष 'c': { elem_max_size = std::max( elem_max_size, परिमाण(schar ) ); भङ्ग; }
        पक्ष 'w': { elem_max_size = std::max( elem_max_size, परिमाण(ushort) ); भङ्ग; }
        पक्ष 's': { elem_max_size = std::max( elem_max_size, परिमाण(short ) ); भङ्ग; }
        पक्ष 'i': { elem_max_size = std::max( elem_max_size, परिमाण(पूर्णाङ्क   ) ); भङ्ग; }
        पक्ष 'n': { elem_max_size = std::max( elem_max_size, परिमाण(unsigned) ); भङ्ग; }
        पक्ष 'f': { elem_max_size = std::max( elem_max_size, परिमाण(भिन्न ) ); भङ्ग; }
        पक्ष 'd': { elem_max_size = std::max( elem_max_size, परिमाण(द्विगुण) ); भङ्ग; }
        पक्ष 'h': { elem_max_size = std::max( elem_max_size, परिमाण(hfloat)); भङ्ग; }
        पक्ष 'H': { elem_max_size = std::max( elem_max_size, परिमाण(bfloat)); भङ्ग; }
        पक्ष 'e': { elem_max_size = std::max( elem_max_size, परिमाण(fp8_t) ); भङ्ग; }
        पक्ष 'E': { elem_max_size = std::max( elem_max_size, परिमाण(fp8a_t)); भङ्ग; }
        पक्ष 'I': { elem_max_size = std::max( elem_max_size, परिमाण(int64_t)); भङ्ग; }
        पक्ष 'U': { elem_max_size = std::max( elem_max_size, परिमाण(uint64_t)); भङ्ग; }
        स्वभाव:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (वर्ण)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<पूर्णाङ्क>(elem_max_size) );
    प्रत्यागम size;
}

पूर्णाङ्क decodeSimpleFormat( ध्रुव वर्ण* dt )
{
    पूर्णाङ्क elem_type = -1;
    पूर्णाङ्क fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_गणनम्;

    fmt_pair_गणनम् = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    जइ( fmt_pair_गणनम् != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    प्रत्यागम elem_type;
}

}

#जइ defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#define CV_LITTLE_ENDIAN_MEM_ACCESS 1
#अण्णहा
#define CV_LITTLE_ENDIAN_MEM_ACCESS 0
#endif

स्थिर अन्तःस्थ पूर्णाङ्क readInt(ध्रुव uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#जइ CV_LITTLE_ENDIAN_MEM_ACCESS
    पूर्णाङ्क val;
    memcpy(&val, p, परिमाण(val));
    प्रत्यागम val;
#अण्णहा
    पूर्णाङ्क val = (पूर्णाङ्क)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    प्रत्यागम val;
#endif
}

स्थिर अन्तःस्थ int64_t readLong(ध्रुव uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#जइ CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    memcpy(&val, p, परिमाण(val));
    प्रत्यागम val;
#अण्णहा
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    प्रत्यागम val0 | ((int64_t)val1 << 32);
#endif
}

स्थिर अन्तःस्थ द्विगुण readReal(ध्रुव uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#जइ CV_LITTLE_ENDIAN_MEM_ACCESS
    द्विगुण val;
    memcpy(&val, p, परिमाण(val));
    प्रत्यागम val;
#अण्णहा
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    प्रत्यागम val.f;
#endif
}

template <typename T>
स्थिर अन्तःस्थ शून्य writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#जइ CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &ival, परिमाण(ival));
#अण्णहा
    प्रति (size_t i = 0, j = 0; i < परिमाण(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#endif
}

स्थिर अन्तःस्थ शून्य writeReal(uchar* p, द्विगुण fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#जइ CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &fval, परिमाण(fval));
#अण्णहा
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
#endif
}



शून्य FileStorage::Impl::init() {
    flags = 0;
    buffer.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = मिच्छ;
    state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = मिच्छ;
    delayed_struct_कुञ्चिका = nullptr;
    delayed_struct_flags = 0;
    delayed_type_name = nullptr;
    base64_writer = nullptr;
    is_opened = मिच्छ;
    dummy_eof = मिच्छ;
    write_mode = मिच्छ;
    mem_mode = मिच्छ;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    empty_stream = सच्च;

    strbufv.clear();
    strbuf = 0;
    strbufsize = strbufpos = 0;
    roots.clear();

    fs_data.clear();
    fs_data_ptrs.clear();
    fs_data_blksz.clear();
    freeSpaceOfs = 0;

    str_hash.clear();
    str_hash_data.clear();
    str_hash_data.resize(1);
    str_hash_data[0] = '\0';

    filename.clear();
    lineno = 0;
}

FileStorage::Impl::Impl(FileStorage *_fs) {
    fs_ext = _fs;
    init();
}

FileStorage::Impl::~Impl() {
    release();
}

शून्य FileStorage::Impl::release(String *out) {
    जइ (is_opened) {
        जइ (out)
            out->clear();
        जइ (write_mode) {
            जाव (write_राशिः.size() > 1) {
                endWriteStruct();
            }
            flush();
            जइ (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            अण्णहा जइ (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        जइ (mem_mode && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

शून्य FileStorage::Impl::analyze_file_name(ध्रुव std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    स्थिर ध्रुव वर्ण not_file_name = '\n';
    स्थिर ध्रुव वर्ण parameter_begin = '?';
    स्थिर ध्रुव वर्ण parameter_separator = '&';

    जइ (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        प्रत्यागम;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    जइ (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        प्रति (size_t param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            जइ ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

तार्किक FileStorage::Impl::open(ध्रुव वर्ण *filename_or_buf, पूर्णाङ्क _flags, ध्रुव वर्ण *encoding) {
    तार्किक ok = सच्च;
    release();

    तार्किक append = (_flags & 3) == FileStorage::APPEND;
    mem_mode = (_flags & FileStorage::MEMORY) != 0;

    write_mode = (_flags & 3) != 0;
    तार्किक write_base64 = (write_mode || append) && (_flags & FileStorage::BASE64) != 0;

    तार्किक isGZ = मिच्छ;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        जइ (!params.empty())
            filename = params[0];

        जइ (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_mode || append);
    }

    जइ (filename.size() == 0 && !mem_mode && !write_mode)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    जइ (mem_mode && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    जइ (!mem_mode) {
        size_t dot_idx = filename.find_last_of('.');
        वर्ण compression = '\0';

        जइ (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            जइ (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                जइ (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = सच्च;
                }

                अण्णहा जइ(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = सच्च;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        जइ (!isGZ) {
            file = fopen(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            जइ (!file)
            {
                CV_LOG_ERROR(रिक्त, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                प्रत्यागम मिच्छ;
            }
        } अण्णहा {
#जइ USE_ZLIB
            जइ (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            वर्ण mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            जइ (!gzfile)
            {
                CV_LOG_ERROR(रिक्त, "Can't open archive: '" << filename << "' mode=" << mode);
                प्रत्यागम मिच्छ;
            }
#अण्णहा
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#endif
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_data.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    जइ (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        जइ (mem_mode)
            outbuf.clear();

        जइ (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            ध्रुव वर्ण *dot_pos = रिक्त;
            ध्रुव वर्ण *dot_pos2 = रिक्त;
            // like strrchr() implementation, but save two last positions simultaneously
            प्रति (ध्रुव वर्ण *pos = &filename[0]; pos[0] != 0; pos++) {
                जइ (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            जइ (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != रिक्त) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } अण्णहा जइ (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        पूर्णाङ्क buf_size = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        जइ (append) {
            fseek(file, 0, SEEK_END);
            जइ (ftell(file) == 0)
                append = मिच्छ;
        }

        write_राशिः.clear();
        empty_stream = सच्च;
        write_राशिः.push_back(FStructData("", File_ग्रन्थिः::MAP | File_ग्रन्थिः::EMPTY, 0));
        buffer.reserve(buf_size + 1024);
        buffer.resize(buf_size);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;

        जइ (fmt == FileStorage::FORMAT_XML) {
            size_t file_size = file ? (size_t) ftell(file) : (size_t) 0;
            जइ (!append || file_size == 0) {
                जइ (encoding && *encoding != '\0') {
                    जइ (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(strlen(encoding) < 1000);
                    वर्ण buf[1100];
                    snprintf(buf, परिमाण(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } अण्णहा
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } अण्णहा {
                पूर्णाङ्क xml_buf_size = 1 << 10;
                वर्ण substr[] = "</opencv_storage>";
                पूर्णाङ्क last_occurrence = -1;
                xml_buf_size = MIN(xml_buf_size, पूर्णाङ्क(file_size));
                fseek(file, -xml_buf_size, SEEK_END);
                // find the last occurrence of </opencv_storage>
                प्रति (;;) {
                    पूर्णाङ्क पङ्क्तिः_offset = (पूर्णाङ्क) ftell(file);
                    ध्रुव वर्ण *ptr0 = एतत्->gets(xml_buf_size);
                    ध्रुव वर्ण *ptr = रिक्त;
                    जइ (!ptr0)
                        भङ्ग;
                    ptr = ptr0;
                    प्रति (;;) {
                        ptr = strstr(ptr, substr);
                        जइ (!ptr)
                            भङ्ग;
                        last_occurrence = पङ्क्तिः_offset + (पूर्णाङ्क) (ptr - ptr0);
                        ptr += strlen(substr);
                    }
                }
                जइ (last_occurrence < 0) {
                    release();
                    CV_Error(cv::Error::StsError, "Could not find </opencv_storage> in the end of file.\n");
                }
                closeFile();
                file = fopen(filename.c_str(), "r+t");
                CV_Assert(file != 0);
                fseek(file, last_occurrence, SEEK_SET);
                // replace the last "</opencv_storage>" with " <!-- resumed -->", which has the same length
                puts(" <!-- resumed -->");
                fseek(file, 0, SEEK_END);
                puts("\n");
            }

            emitter_do_not_use_direct_dereference = createXMLEmitter(एतत्);
        } अण्णहा जइ (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                जइ (!append) {
                    जइ (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    अण्णहा
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            अण्णहा
                puts("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(एतत्);
        } अण्णहा {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            जइ (!append)
                puts("{\n");
            अण्णहा {
                तार्किक valid = मिच्छ;
                long roffset = 0;
                प्रति (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    ध्रुव वर्ण end_mark = '}';
                    जइ (fgetc(file) == end_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = सच्च;
                        भङ्ग;
                    }
                }

                जइ (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } अण्णहा {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_राशिः.back().indent = 4;
            emitter_do_not_use_direct_dereference = createJSONEmitter(एतत्);
        }
        is_opened = सच्च;
    } अण्णहा {
        ध्रुव size_t buf_size0 = 40;
        buffer.resize(buf_size0);
        जइ (mem_mode) {
            strbuf = (वर्ण *) filename_or_buf;
            strbufsize = strlen(strbuf);
        }

        ध्रुव वर्ण *yaml_signature = "%YAML";
        ध्रुव वर्ण *json_signature = "{";
        ध्रुव वर्ण *xml_signature = "<?xml";
        वर्ण *buf = एतत्->gets(16);
        CV_Assert(buf);
        वर्ण *bufPtr = cv_skip_BOM(buf);
        size_t bufOffset = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        जइ (strncmp(bufPtr, yaml_signature, strlen(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        अण्णहा जइ (strncmp(bufPtr, json_signature, strlen(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        अण्णहा जइ (strncmp(bufPtr, xml_signature, strlen(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        अण्णहा
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = bufOffset;
        bufofs = 0;

        यत्न {
            वर्ण *ptr = bufferStart();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_ग्रन्थिः मूलम्_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_ग्रन्थिः_Space(मूलम्_nodes, 9);
            *rptr = File_ग्रन्थिः::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            विकल्प (fmt) {
                पक्ष FileStorage::FORMAT_XML:
                    parser_do_not_use_direct_dereference = createXMLParser(एतत्);
                    भङ्ग;
                पक्ष FileStorage::FORMAT_YAML:
                पक्ष FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_direct_dereference = createYAMLParser(एतत्);
                    भङ्ग;
                पक्ष FileStorage::FORMAT_JSON:
                    parser_do_not_use_direct_dereference = createJSONParser(एतत्);
                    भङ्ग;
                स्वभाव:
                    parser_do_not_use_direct_dereference = Ptr<FileStorageParser>();
            }

            जइ (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                जइ (ok) {
                    finalizeCollection(मूलम्_nodes);

                    CV_Assert(!fs_data_ptrs.empty());
                    File_ग्रन्थिः roots_ग्रन्थिः(fs_ext, 0, 0);
                    size_t i, nroots = roots_ग्रन्थिः.size();
                    File_ग्रन्थिः_Iterator it = roots_ग्रन्थिः.begin();

                    प्रति (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        गृहाण (...)
        {
            // FIXIT log error message
            is_opened = सच्च;
            release();
            क्षिप;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = सच्च;
        std::vector<वर्ण> tmpbuf;
        std::swap(buffer, tmpbuf);
        bufofs = 0;
    }
    प्रत्यागम ok;
}

शून्य FileStorage::Impl::puts(ध्रुव वर्ण *str) {
    CV_Assert(write_mode);
    जइ (mem_mode)
        std::copy(str, str + strlen(str), std::back_inserter(outbuf));
    अण्णहा जइ (file)
        fputs(str, file);
#जइ USE_ZLIB
    अण्णहा जइ (gzfile)
        gzputs(gzfile, str);
#endif
    अण्णहा
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

वर्ण *FileStorage::Impl::getsFromFile(वर्ण *buf, पूर्णाङ्क गणनम्_2) {
    जइ (file)
        प्रत्यागम fgets(buf, गणनम्_2, file);
#जइ USE_ZLIB
    जइ (gzfile)
        प्रत्यागम gzgets(gzfile, buf, गणनम्_2);
#endif
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

वर्ण *FileStorage::Impl::gets(size_t max_गणनम्) {
    जइ (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        ध्रुव वर्ण *instr = strbuf;
        प्रति (; i < len; i++) {
            वर्ण c = instr[i];
            जइ (c == '\0' || c == '\n') {
                जइ (c == '\n')
                    i++;
                भङ्ग;
            }
        }
        size_t गणनम्_2 = i - strbufpos;
        जइ (max_गणनम् == 0 || max_गणनम् > गणनम्_2)
            max_गणनम् = गणनम्_2;
        buffer.resize(std::max(buffer.size(), max_गणनम् + 8));
        memcpy(&buffer[0], instr + strbufpos, max_गणनम्);
        buffer[max_गणनम्] = '\0';
        strbufpos = i;
        प्रत्यागम max_गणनम् > 0 ? &buffer[0] : 0;
    }

    ध्रुव size_t MAX_BLOCK_SIZE = INT_MAX / 2; // hopefully, that will be enough
    जइ (max_गणनम् == 0)
        max_गणनम् = MAX_BLOCK_SIZE;
    अण्णहा
        CV_Assert(max_गणनम् < MAX_BLOCK_SIZE);
    size_t ofs = 0;

    प्रति (;;) {
        पूर्णाङ्क गणनम्_2 = (पूर्णाङ्क) std::min(buffer.size() - ofs - 16, max_गणनम्);
        वर्ण *ptr = getsFromFile(&buffer[ofs], गणनम्_2 + 1);
        जइ (!ptr)
            भङ्ग;
        पूर्णाङ्क delta = (पूर्णाङ्क) strlen(ptr);
        ofs += delta;
        max_गणनम् -= delta;
        जइ (delta == 0 || ptr[delta - 1] == '\n' || max_गणनम् == 0)
            भङ्ग;
        जइ (delta == गणनम्_2)
            buffer.resize((size_t) (buffer.size() * 1.5));
    }
    प्रत्यागम ofs > 0 ? &buffer[0] : 0;
}

वर्ण *FileStorage::Impl::gets() {
    वर्ण *ptr = एतत्->gets(0);
    जइ (!ptr) {
        ptr = bufferStart();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        प्रत्यागम 0;
    } अण्णहा {
        size_t l = strlen(ptr);
        जइ (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    प्रत्यागम ptr;
}

तार्किक FileStorage::Impl::eof() {
    जइ (dummy_eof)
        प्रत्यागम सच्च;
    जइ (strbuf)
        प्रत्यागम strbufpos >= strbufsize;
    जइ (file)
        प्रत्यागम feof(file) != 0;
#जइ USE_ZLIB
    जइ (gzfile)
        प्रत्यागम gzeof(gzfile) != 0;
#endif
    प्रत्यागम मिच्छ;
}

शून्य FileStorage::Impl::setEof() {
    dummy_eof = सच्च;
}

शून्य FileStorage::Impl::closeFile() {
    जइ (file)
        fclose(file);
#जइ USE_ZLIB
    अण्णहा जइ (gzfile)
        gzclose(gzfile);
#endif
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = मिच्छ;
}

शून्य FileStorage::Impl::rewind() {
    जइ (file)
        ::rewind(file);
#जइ USE_ZLIB
    अण्णहा जइ (gzfile)
        gzrewind(gzfile);
#endif
    strbufpos = 0;
}

वर्ण *FileStorage::Impl::resizeWriteBuffer(वर्ण *ptr, पूर्णाङ्क len) {
    ध्रुव वर्ण *buffer_end = &buffer[0] + buffer.size();
    जइ (ptr + len < buffer_end)
        प्रत्यागम ptr;

    ध्रुव वर्ण *buffer_start = &buffer[0];
    पूर्णाङ्क written_len = (पूर्णाङ्क) (ptr - buffer_start);

    CV_Assert(written_len <= (पूर्णाङ्क) buffer.size());
    पूर्णाङ्क new_size = (पूर्णाङ्क) ((buffer_end - buffer_start) * 3 / 2);
    new_size = MAX(written_len + len, new_size);
    buffer.reserve(new_size + 256);
    buffer.resize(new_size);
    bufofs = written_len;
    प्रत्यागम &buffer[0] + bufofs;
}

वर्ण *FileStorage::Impl::flush() {
    वर्ण *buffer_start = &buffer[0];
    वर्ण *ptr = buffer_start + bufofs;

    जइ (ptr > buffer_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(buffer_start);
        bufofs = 0;
    }

    पूर्णाङ्क indent = write_राशिः.back().indent;

    जइ (space != indent) {
        memset(buffer_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = buffer_start + bufofs;

    प्रत्यागम ptr;
}

शून्य FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(मिच्छ);
    जइ (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_राशिः.empty());

    FStructData &current_struct = write_राशिः.back();
    जइ (fmt == FileStorage::FORMAT_JSON && !File_ग्रन्थिः::isFlow(current_struct.flags) && write_राशिः.size() > 1)
        current_struct.indent = write_राशिः[write_राशिः.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_राशिः.pop_back();
    जइ (!write_राशिः.empty())
        write_राशिः.back().flags &= ~File_ग्रन्थिः::EMPTY;
}

शून्य FileStorage::Impl::startWriteStruct_helper(ध्रुव वर्ण *कुञ्चिका, पूर्णाङ्क struct_flags,
                                                ध्रुव वर्ण *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (File_ग्रन्थिः::TYPE_MASK | File_ग्रन्थिः::FLOW)) | File_ग्रन्थिः::EMPTY;
    जइ (!File_ग्रन्थिः::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    जइ (type_name && type_name[0] == '\0')
        type_name = 0;

    FStructData s = getEmitter().startWriteStruct(write_राशिः.back(), कुञ्चिका, struct_flags, type_name);

    write_राशिः.push_back(s);
    size_t write_राशिः_size = write_राशिः.size();
    जइ (write_राशिः_size > 1)
        write_राशिः[write_राशिः_size - 2].flags &= ~File_ग्रन्थिः::EMPTY;

    जइ (fmt != FileStorage::FORMAT_JSON && !File_ग्रन्थिः::isFlow(s.flags))
        flush();

    जइ (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_ग्रन्थिः::isMap(struct_flags)) {
        getEmitter().लिख("type_id", type_name, मिच्छ);
    }
}

शून्य FileStorage::Impl::startWriteStruct(ध्रुव वर्ण *कुञ्चिका, पूर्णाङ्क struct_flags,
                                         ध्रुव वर्ण *type_name) {
    check_if_write_struct_is_delayed(मिच्छ);
    जइ (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    जइ (state_of_writing_base64 == FileStorage_API::Uncertain && File_ग्रन्थिः::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(कुञ्चिका, struct_flags, type_name);
    } अण्णहा जइ (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        जइ ((File_ग्रन्थिः::TYPE_MASK & struct_flags) != File_ग्रन्थिः::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        अण्णहा जइ (state_of_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(कुञ्चिका, struct_flags, "binary");

        जइ (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::InUse);
    } अण्णहा {
        /* Won't output Base64 data */
        जइ (state_of_writing_base64 == FileStorage_API::InUse)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(कुञ्चिका, struct_flags, type_name);

        जइ (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::NotUse);
    }
}

शून्य FileStorage::Impl::writeComment(ध्रुव वर्ण *comment, तार्किक eol_comment) {
    CV_Assert(write_mode);
    getEmitter().writeComment(comment, eol_comment);
}

शून्य FileStorage::Impl::startNextStream() {
    CV_Assert(write_mode);
    जइ (!empty_stream) {
        जाव (!write_राशिः.empty())
            endWriteStruct();
        flush();
        getEmitter().startNextStream();
        empty_stream = सच्च;
        write_राशिः.push_back(FStructData("", File_ग्रन्थिः::EMPTY, 0));
        bufofs = 0;
    }
}

शून्य FileStorage::Impl::लिख(ध्रुव String &कुञ्चिका, पूर्णाङ्क value) {
    CV_Assert(write_mode);
    getEmitter().लिख(कुञ्चिका.c_str(), value);
}

शून्य FileStorage::Impl::लिख(ध्रुव String &कुञ्चिका, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().लिख(कुञ्चिका.c_str(), value);
}

शून्य FileStorage::Impl::लिख(ध्रुव String &कुञ्चिका, द्विगुण value) {
    CV_Assert(write_mode);
    getEmitter().लिख(कुञ्चिका.c_str(), value);
}

शून्य FileStorage::Impl::लिख(ध्रुव String &कुञ्चिका, ध्रुव String &value) {
    CV_Assert(write_mode);
    getEmitter().लिख(कुञ्चिका.c_str(), value.c_str(), मिच्छ);
}

शून्य FileStorage::Impl::writeRawData(ध्रुव std::string &dt, ध्रुव शून्य *_data, size_t len) {
    CV_Assert(write_mode);

    जइ (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        writeRawDataBase64(_data, len, dt.c_str());
        प्रत्यागम;
    } अण्णहा जइ (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elemSize = fs::calcStructSize(dt.c_str(), 0);
    CV_Assert(elemSize);
    CV_Assert(len % elemSize == 0);
    len /= elemSize;

    तार्किक explicitZero = fmt == FileStorage::FORMAT_JSON;
    ध्रुव uchar *data0 = (ध्रुव uchar *) _data;
    पूर्णाङ्क fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_गणनम्;
    वर्ण buf[256] = "";

    fmt_pair_गणनम् = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    जइ (!len)
        प्रत्यागम;

    जइ (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    जइ (fmt_pair_गणनम् == 1) {
        fmt_pairs[0] *= (पूर्णाङ्क) len;
        len = 1;
    }

    प्रति (; len--; data0 += elemSize) {
        पूर्णाङ्क offset = 0;
        प्रति (k = 0; k < fmt_pair_गणनम्; k++) {
            पूर्णाङ्क i, गणनम्_2 = fmt_pairs[k * 2];
            पूर्णाङ्क elem_type = fmt_pairs[k * 2 + 1];
            पूर्णाङ्क elem_size = CV_ELEM_SIZE(elem_type);
            ध्रुव वर्ण *ptr;

            offset = cvAlign(offset, elem_size);
            ध्रुव uchar *data = data0 + offset;

            प्रति (i = 0; i < गणनम्_2; i++) {
                विकल्प (elem_type) {
                    पक्ष CV_8U:
                        ptr = fs::itoa(*(uchar *) data, buf, 10);
                        data++;
                        भङ्ग;
                    पक्ष CV_Bool:
                        ptr = fs::itoa(*(uchar *) data != 0, buf, 10);
                        data++;
                        भङ्ग;
                    पक्ष CV_8S:
                        ptr = fs::itoa(*(वर्ण *) data, buf, 10);
                        data++;
                        भङ्ग;
                    पक्ष CV_16U:
                        ptr = fs::itoa(*(ushort *) data, buf, 10);
                        data += परिमाण(ushort);
                        भङ्ग;
                    पक्ष CV_16S:
                        ptr = fs::itoa(*(short *) data, buf, 10);
                        data += परिमाण(short);
                        भङ्ग;
                    पक्ष CV_32U:
                        ptr = fs::itoa((int64_t)*(unsigned*) data, buf, 10, मिच्छ);
                        data += परिमाण(unsigned);
                        भङ्ग;
                    पक्ष CV_32S:
                        ptr = fs::itoa(*(पूर्णाङ्क *) data, buf, 10);
                        data += परिमाण(पूर्णाङ्क);
                        भङ्ग;
                    पक्ष CV_64U:
                        ptr = fs::itoa(*(uint64_t*) data, buf, 10, मिच्छ);
                        data += परिमाण(uint64_t);
                        भङ्ग;
                    पक्ष CV_64S:
                        ptr = fs::itoa(*(int64_t*) data, buf, 10, सच्च);
                        data += परिमाण(int64_t);
                        भङ्ग;
                    पक्ष CV_32F:
                        ptr = fs::floatToString(buf, परिमाण(buf), *(भिन्न *) data, मिच्छ, explicitZero);
                        data += परिमाण(भिन्न);
                        भङ्ग;
                    पक्ष CV_64F:
                        ptr = fs::doubleToString(buf, परिमाण(buf), *(द्विगुण *) data, explicitZero);
                        data += परिमाण(द्विगुण);
                        भङ्ग;
                    पक्ष CV_16F:
                        ptr = fs::floatToString(buf, परिमाण(buf), (भिन्न) *(hfloat *) data, सच्च, explicitZero);
                        data += परिमाण(hfloat);
                        भङ्ग;
                    पक्ष CV_16BF:
                        ptr = fs::floatToString(buf, परिमाण(buf), (भिन्न) *(bfloat *) data, सच्च, explicitZero);
                        data += परिमाण(bfloat);
                        भङ्ग;
                    पक्ष CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, परिमाण(buf), (भिन्न) *(fp8_t *) data, सच्च, explicitZero);
                        data += परिमाण(fp8_t);
                        भङ्ग;
                    पक्ष CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, परिमाण(buf), (भिन्न) *(fp8a_t *) data, सच्च, explicitZero);
                        data += परिमाण(fp8a_t);
                        भङ्ग;
                    स्वभाव:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        प्रत्यागम;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (पूर्णाङ्क) (data - data0);
        }
    }
}

शून्य FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(मिच्छ);

    जइ (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_to_Base64_state(FileStorage_API::Base64State::Uncertain);
}

शून्य FileStorage::Impl::switch_to_Base64_state(FileStorage_API::Base64State new_state) {
    ध्रुव वर्ण *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    ध्रुव वर्ण *err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    विकल्प (state_of_writing_base64) {
        पक्ष FileStorage_API::Base64State::Uncertain:
            विकल्प (new_state) {
                पक्ष FileStorage_API::Base64State::InUse:
                {
                    CV_DbgAssert(base64_writer == 0);
                    तार्किक can_indent = (fmt != cv::FileStorage::Mode::FORMAT_JSON);
                    base64_writer = नूतन base64::Base64Writer(*एतत्, can_indent);
                    जइ (!can_indent) {
                        वर्ण *ptr = bufferPtr();
                        *ptr++ = '\0';
                        puts(bufferStart());
                        setBufferPtr(bufferStart());
                        memset(bufferStart(), 0, static_cast<पूर्णाङ्क>(space));
                        puts("\"$base64$");
                    }
                    भङ्ग;
                }
                पक्ष FileStorage_API::Base64State::Uncertain:
                    भङ्ग;
                पक्ष FileStorage_API::Base64State::NotUse:
                    भङ्ग;
                स्वभाव:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    भङ्ग;
            }
            भङ्ग;
        पक्ष FileStorage_API::Base64State::InUse:
            विकल्प (new_state) {
                पक्ष FileStorage_API::Base64State::InUse:
                पक्ष FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    भङ्ग;
                पक्ष FileStorage_API::Base64State::Uncertain:
                    delete base64_writer;
                    base64_writer = 0;
                    जइ ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        setBufferPtr(bufferStart());
                        flush();
                        memset(bufferStart(), 0, static_cast<पूर्णाङ्क>(space) );
                        setBufferPtr(bufferStart());
                    }
                    भङ्ग;
                स्वभाव:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    भङ्ग;
            }
            भङ्ग;
        पक्ष FileStorage_API::Base64State::NotUse:
            विकल्प (new_state) {
                पक्ष FileStorage_API::Base64State::InUse:
                पक्ष FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    भङ्ग;
                पक्ष FileStorage_API::Base64State::Uncertain:
                    भङ्ग;
                स्वभाव:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    भङ्ग;
            }
            भङ्ग;
        स्वभाव:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            भङ्ग;
    }

    state_of_writing_base64 = new_state;
}

शून्य FileStorage::Impl::make_write_struct_delayed(ध्रुव वर्ण *कुञ्चिका, पूर्णाङ्क struct_flags, ध्रुव वर्ण *type_name) {
    CV_Assert(is_write_struct_delayed == मिच्छ);
    CV_DbgAssert(delayed_struct_कुञ्चिका == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    जइ (कुञ्चिका != nullptr) {
        delayed_struct_कुञ्चिका = नूतन वर्ण[strlen(कुञ्चिका) + 1U];
        strcpy(delayed_struct_कुञ्चिका, कुञ्चिका);
    }

    जइ (type_name != nullptr) {
        delayed_type_name = नूतन वर्ण[strlen(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = सच्च;
}

शून्य FileStorage::Impl::check_if_write_struct_is_delayed(तार्किक change_type_to_base64) {
    जइ (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_कुञ्चिका;
        std::string type_name;
        पूर्णाङ्क struct_flags = delayed_struct_flags;

        जइ (delayed_struct_कुञ्चिका != nullptr && *delayed_struct_कुञ्चिका != '\0') {
            struct_कुञ्चिका.assign(delayed_struct_कुञ्चिका);
        }
        जइ (delayed_type_name != nullptr && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        delete[] delayed_struct_कुञ्चिका;
        delete[] delayed_type_name;
        delayed_struct_कुञ्चिका = nullptr;
        delayed_struct_flags = 0;
        delayed_type_name = nullptr;

        is_write_struct_delayed = मिच्छ;

        /* call */
        जइ (change_type_to_base64) {
            startWriteStruct_helper(struct_कुञ्चिका.c_str(), struct_flags, "binary");
            जइ (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::InUse);
        } अण्णहा {
            startWriteStruct_helper(struct_कुञ्चिका.c_str(), struct_flags, type_name.c_str());
            जइ (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::NotUse);
        }
    }
}

शून्य FileStorage::Impl::writeRawDataBase64(ध्रुव शून्य *_data, size_t len, ध्रुव वर्ण *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(सच्च);

    जइ (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } अण्णहा जइ (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->लिख(_data, len, dt);
}

File_ग्रन्थिः FileStorage::Impl::get_First_शिखरम्_Level_ग्रन्थिः() ध्रुव {
    प्रत्यागम roots.empty() ? File_ग्रन्थिः() : roots[0];
}

File_ग्रन्थिः FileStorage::Impl::मूलम्(पूर्णाङ्क streamIdx) ध्रुव {
    प्रत्यागम streamIdx >= 0 && streamIdx < (पूर्णाङ्क) roots.size() ? roots[streamIdx] : File_ग्रन्थिः();
}

File_ग्रन्थिः FileStorage::Impl::operator[](ध्रुव String &nodename) ध्रुव {
    प्रत्यागम एतत्->operator[](nodename.c_str());
}

File_ग्रन्थिः FileStorage::Impl::operator[](ध्रुव वर्ण * /*nodename*/) ध्रुव {
    प्रत्यागम File_ग्रन्थिः();
}

पूर्णाङ्क FileStorage::Impl::getFormat() ध्रुव { प्रत्यागम fmt; }

वर्ण *FileStorage::Impl::bufferPtr() ध्रुव { प्रत्यागम (वर्ण *) (&buffer[0] + bufofs); }

वर्ण *FileStorage::Impl::bufferStart() ध्रुव { प्रत्यागम (वर्ण *) &buffer[0]; }

वर्ण *FileStorage::Impl::bufferEnd() ध्रुव { प्रत्यागम (वर्ण *) (&buffer[0] + buffer.size()); }

शून्य FileStorage::Impl::setBufferPtr(वर्ण *ptr) {
    वर्ण *bufferstart = bufferStart();
    CV_Assert(ptr >= bufferstart && ptr <= bufferEnd());
    bufofs = ptr - bufferstart;
}

पूर्णाङ्क FileStorage::Impl::wrapMargin() ध्रुव { प्रत्यागम wrap_margin; }

FStructData &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_राशिः.empty());
    प्रत्यागम write_राशिः.back();
}

शून्य FileStorage::Impl::setNonEmpty() {
    empty_stream = मिच्छ;
}

शून्य FileStorage::Impl::processSpecialDouble(वर्ण *buf, द्विगुण *value, वर्ण **endptr) {
    FileStorage_API *fs = एतत्;
    वर्ण c = buf[0];
    पूर्णाङ्क inf_hi = 0x7ff00000;

    जइ (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    जइ (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    जइ (toupper(buf[1]) == 'I' && toupper(buf[2]) == 'N' && toupper(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    अण्णहा जइ (toupper(buf[1]) == 'N' && toupper(buf[2]) == 'A' && toupper(buf[3]) == 'N')
        v.u = (uint64) -1;
    अण्णहा
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

द्विगुण FileStorage::Impl::strtod(वर्ण *ptr, वर्ण **endptr) {
    द्विगुण fval = ::strtod(ptr, endptr);
    जइ (**endptr == '.') {
        वर्ण *dot_pos = *endptr;
        *dot_pos = ',';
        द्विगुण fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        जइ (*endptr > dot_pos)
            fval = fval2;
        अण्णहा
            *endptr = dot_pos;
    }

    जइ (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    प्रत्यागम fval;
}

शून्य FileStorage::Impl::convertToCollection(पूर्णाङ्क type, File_ग्रन्थिः &ग्रन्थिः) {
    CV_Assert(type == File_ग्रन्थिः::SEQ || type == File_ग्रन्थिः::MAP);

    पूर्णाङ्क ग्रन्थिः_type = ग्रन्थिः.type();
    जइ (ग्रन्थिः_type == type)
        प्रत्यागम;

    तार्किक named = ग्रन्थिः.isNamed();
    uchar *ptr = ग्रन्थिः.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    द्विगुण fval = 0;
    std::string sval;
    तार्किक add_first_scalar = मिच्छ;

    जइ (ग्रन्थिः_type != File_ग्रन्थिः::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_ग्रन्थिः::SEQ);
        जइ (ग्रन्थिः_type == File_ग्रन्थिः::INT) {
            ival = readLong(ptr);
            add_first_scalar = सच्च;
        } अण्णहा जइ (ग्रन्थिः_type == File_ग्रन्थिः::REAL) {
            fval = readReal(ptr);
            add_first_scalar = सच्च;
        } अण्णहा जइ (ग्रन्थिः_type == File_ग्रन्थिः::STRING) {
            sval = std::string(ग्रन्थिः);
            add_first_scalar = सच्च;
        } अण्णहा
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", ग्रन्थिः_type));
    }

    ptr = reserve_ग्रन्थिः_Space(ग्रन्थिः, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_ग्रन्थिः::NAMED : 0));
    // name has been copied automatically
    जइ (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    जइ (add_first_scalar)
        add_ग्रन्थिः(ग्रन्थिः, std::string(), ग्रन्थिः_type,
                ग्रन्थिः_type == File_ग्रन्थिः::INT ? (ध्रुव शून्य *) &ival :
                ग्रन्थिः_type == File_ग्रन्थिः::REAL ? (ध्रुव शून्य *) &fval :
                ग्रन्थिः_type == File_ग्रन्थिः::STRING ? (ध्रुव शून्य *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_ग्रन्थिः_Space(File_ग्रन्थिः &ग्रन्थिः, size_t sz) {
    तार्किक shrinkBlock = मिच्छ;
    size_t shrinkBlockIdx = 0, shrinkSize = 0;

    uchar *ptr = 0, *blockEnd = 0;

    जइ (!fs_data_ptrs.empty()) {
        size_t blockIdx = ग्रन्थिः.blockIdx;
        size_t ofs = ग्रन्थिः.ofs;
        CV_Assert(blockIdx == fs_data_ptrs.size() - 1);
        CV_Assert(ofs <= fs_data_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_data_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_data_ptrs[blockIdx] + ofs;
        blockEnd = fs_data_ptrs[blockIdx] + fs_data_blksz[blockIdx];

        CV_Assert(ptr >= fs_data_ptrs[blockIdx] && ptr <= blockEnd);
        जइ (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            प्रत्यागम ptr;
        }

        जइ (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_data[blockIdx]->resize(sz);
            ptr = &fs_data[blockIdx]->at(0);
            fs_data_ptrs[blockIdx] = ptr;
            fs_data_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            प्रत्यागम ptr;
        }

        shrinkBlock = सच्च;
        shrinkBlockIdx = blockIdx;
        shrinkSize = ofs;
    }

    size_t blockSize = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(blockSize);
    fs_data.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_data_ptrs.push_back(new_ptr);
    fs_data_blksz.push_back(blockSize);
    ग्रन्थिः.blockIdx = fs_data_ptrs.size() - 1;
    ग्रन्थिः.ofs = 0;
    freeSpaceOfs = sz;

    जइ (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        जइ (ptr[0] & File_ग्रन्थिः::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    जइ (shrinkBlock) {
        fs_data[shrinkBlockIdx]->resize(shrinkSize);
        fs_data_blksz[shrinkBlockIdx] = shrinkSize;
    }

    प्रत्यागम new_ptr;
}

unsigned FileStorage::Impl::getStringOfs(ध्रुव std::string &कुञ्चिका) ध्रुव {
    str_hash_t::const_iterator it = str_hash.find(कुञ्चिका);
    प्रत्यागम it != str_hash.end() ? it->second : 0;
}

File_ग्रन्थिः FileStorage::Impl::add_ग्रन्थिः(File_ग्रन्थिः &collection, ध्रुव std::string &कुञ्चिका,
                                    पूर्णाङ्क elem_type, ध्रुव शून्य *value, पूर्णाङ्क len) {
    FileStorage_API *fs = एतत्;
    तार्किक noname = कुञ्चिका.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(कुञ्चिका.c_str(), "_") == 0);
    convertToCollection(noname ? File_ग्रन्थिः::SEQ : File_ग्रन्थिः::MAP, collection);

    तार्किक isseq = collection.empty() ? मिच्छ : collection.isSeq();
    जइ (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    unsigned strofs = 0;
    जइ (!noname) {
        strofs = getStringOfs(कुञ्चिका);
        जइ (!strofs) {
            strofs = (unsigned) str_hash_data.size();
            size_t keysize = कुञ्चिका.size() + 1;
            str_hash_data.resize(strofs + keysize);
            memcpy(&str_hash_data[0] + strofs, &कुञ्चिका[0], keysize);
            str_hash.insert(std::make_pair(कुञ्चिका, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t blockIdx = fs_data_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_ग्रन्थिः ग्रन्थिः(fs_ext, blockIdx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_ग्रन्थिः_Space(ग्रन्थिः, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_ग्रन्थिः::NAMED));
    जइ (elem_type == File_ग्रन्थिः::NONE)
        freeSpaceOfs -= 8;

    जइ (!noname) {
        writeInt(ptr, (पूर्णाङ्क) strofs);
        ptr += 4;
    }

    जइ (elem_type == File_ग्रन्थिः::SEQ || elem_type == File_ग्रन्थिः::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    जइ (value)
        ग्रन्थिः.set_मानम्(elem_type, value, len);

    जइ (collection.isNamed())
        cp += 4;
    पूर्णाङ्क nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    प्रत्यागम ग्रन्थिः;
}

शून्य FileStorage::Impl::finalizeCollection(File_ग्रन्थिः &collection) {
    जइ (!collection.isSeq() && !collection.isMap())
        प्रत्यागम;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    जइ (*ptr0 & File_ग्रन्थिः::NAMED)
        ptr += 4;
    size_t blockIdx = collection.blockIdx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t rawSize = 4;
    unsigned sz = (unsigned) readInt(ptr + 4);
    जइ (sz > 0) {
        size_t lastBlockIdx = fs_data_ptrs.size() - 1;

        प्रति (; blockIdx < lastBlockIdx; blockIdx++) {
            rawSize += fs_data_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    rawSize += freeSpaceOfs - ofs;
    writeInt(ptr, (पूर्णाङ्क) rawSize);
}

शून्य FileStorage::Impl::normalize_ग्रन्थिः_Ofs(size_t &blockIdx, size_t &ofs) ध्रुव {
    जाव (ofs >= fs_data_blksz[blockIdx]) {
        जइ (blockIdx == fs_data_blksz.size() - 1) {
            CV_Assert(ofs == fs_data_blksz[blockIdx]);
            भङ्ग;
        }
        ofs -= fs_data_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    प्रत्यागम state_of_writing_base64;
}

पूर्णाङ्क FileStorage::Impl::get_space() {
    प्रत्यागम space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = सच्च;
}

शून्य FileStorage::Impl::Base64Decoder::init(ध्रुव Ptr<FileStorageParser> &_parser, वर्ण *_ptr, पूर्णाङ्क _indent) {
    parser_do_not_use_direct_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = मिच्छ;
}

तार्किक FileStorage::Impl::Base64Decoder::readMore(पूर्णाङ्क needed) {
    स्थिर ध्रुव uchar base64tab[] =
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

    जइ (eos)
        प्रत्यागम मिच्छ;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    प्रति (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    वर्ण *beg = 0, *end = 0;
    तार्किक ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    जइ (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = सच्च;
        size_t tc = totalchars;
        प्रति (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    पूर्णाङ्क i = 0, j, n = (पूर्णाङ्क) encoded.size();
    जइ (n > 0) {
        ध्रुव uchar *tab = base64tab;
        वर्ण *src = &encoded[0];

        प्रति (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(पूर्णाङ्क) (uchar) src[i]], c = tab[(पूर्णाङ्क) (uchar) src[i + 1]];
            uchar b = tab[(पूर्णाङ्क) (uchar) src[i + 2]], a = tab[(पूर्णाङ्क) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    जइ (i > 0 && encoded[i - 1] == '=') {
        जइ (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        जइ (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    प्रति (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    प्रत्यागम (पूर्णाङ्क) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    जइ (ofs >= sz && !readMore(1))
        प्रत्यागम (uchar) 0;
    प्रत्यागम decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    जइ (ofs + 2 > sz && !readMore(2))
        प्रत्यागम (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    प्रत्यागम val;
}

पूर्णाङ्क FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    जइ (ofs + 4 > sz && !readMore(4))
        प्रत्यागम 0;
    पूर्णाङ्क ival = readInt(&decoded[ofs]);
    ofs += 4;
    प्रत्यागम ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    जइ (ofs + 8 > sz && !readMore(8))
        प्रत्यागम 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    प्रत्यागम ival;
}

द्विगुण FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    जइ (ofs + 8 > sz && !readMore(8))
        प्रत्यागम 0;
    द्विगुण fval = readReal(&decoded[ofs]);
    ofs += 8;
    प्रत्यागम fval;
}

तार्किक FileStorage::Impl::Base64Decoder::endOfStream() ध्रुव { प्रत्यागम eos; }

वर्ण *FileStorage::Impl::Base64Decoder::getPtr() ध्रुव { प्रत्यागम ptr; }


वर्ण *FileStorage::Impl::parseBase64(वर्ण *ptr, पूर्णाङ्क indent, File_ग्रन्थिः &collection) {
    ध्रुव पूर्णाङ्क BASE64_HDR_SIZE = 24;
    वर्ण dt[BASE64_HDR_SIZE + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    पूर्णाङ्क i, k;

    प्रति (i = 0; i < BASE64_HDR_SIZE; i++)
        dt[i] = (वर्ण) base64decoder.getUInt8();
    प्रति (i = 0; i < BASE64_HDR_SIZE; i++)
        जइ (isspace(dt[i]))
            भङ्ग;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    पूर्णाङ्क fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    पूर्णाङ्क fmt_pair_गणनम् = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    द्विगुण fval = 0;

    प्रति (;;) {
        प्रति (k = 0; k < fmt_pair_गणनम्; k++) {
            पूर्णाङ्क elem_type = fmt_pairs[k * 2 + 1];
            पूर्णाङ्क गणनम्_2 = fmt_pairs[k * 2];

            प्रति (i = 0; i < गणनम्_2; i++) {
                पूर्णाङ्क ग्रन्थिः_type = File_ग्रन्थिः::INT;
                विकल्प (elem_type) {
                    पक्ष CV_8U:
                        ival = base64decoder.getUInt8();
                        भङ्ग;
                    पक्ष CV_8S:
                        ival = (वर्ण) base64decoder.getUInt8();
                        भङ्ग;
                    पक्ष CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        भङ्ग;
                    पक्ष CV_16U:
                        ival = base64decoder.getUInt16();
                        भङ्ग;
                    पक्ष CV_16S:
                        ival = (short) base64decoder.getUInt16();
                        भङ्ग;
                    पक्ष CV_32U:
                        ival = base64decoder.getInt32();
                        भङ्ग;
                    पक्ष CV_32S:
                        ival = base64decoder.getInt32();
                        भङ्ग;
                    पक्ष CV_64U:
                        ival = base64decoder.getInt64();
                        भङ्ग;
                    पक्ष CV_64S:
                        ival = base64decoder.getInt64();
                        भङ्ग;
                    पक्ष CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        ग्रन्थिः_type = File_ग्रन्थिः::REAL;
                    }
                        भङ्ग;
                    पक्ष CV_64F:
                        fval = base64decoder.getFloat64();
                        ग्रन्थिः_type = File_ग्रन्थिः::REAL;
                        भङ्ग;
                    पक्ष CV_16F:
                        fval = भिन्न(hfloatFromBits(base64decoder.getUInt16()));
                        ग्रन्थिः_type = File_ग्रन्थिः::REAL;
                        भङ्ग;
                    पक्ष CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        ग्रन्थिः_type = File_ग्रन्थिः::REAL;
                        भङ्ग;
                    पक्ष CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        ग्रन्थिः_type = File_ग्रन्थिः::REAL;
                        भङ्ग;
                    स्वभाव:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                जइ (base64decoder.endOfStream())
                    भङ्ग;
                add_ग्रन्थिः(collection, std::string(), ग्रन्थिः_type,
                        ग्रन्थिः_type == File_ग्रन्थिः::INT ? (शून्य *) &ival : (शून्य *) &fval, -1);
            }
        }
        जइ (base64decoder.endOfStream())
            भङ्ग;
    }

    finalizeCollection(collection);
    प्रत्यागम base64decoder.getPtr();
}

शून्य FileStorage::Impl::parseError(ध्रुव वर्ण *func_name, ध्रुव std::string &err_सन्देशः, ध्रुव वर्ण *source_file,
                                   पूर्णाङ्क source_पङ्क्तिः) {
    std::string सन्देशः = format("%s(%d): %s", filename.c_str(), lineno, err_सन्देशः.c_str());
    error(Error::StsParseError, func_name, सन्देशः.c_str(), source_file, source_पङ्क्तिः);
}

ध्रुव uchar *FileStorage::Impl::get_ग्रन्थिः_Ptr(size_t blockIdx, size_t ofs) ध्रुव {
    CV_Assert(blockIdx < fs_data_ptrs.size());
    CV_Assert(ofs < fs_data_blksz[blockIdx]);

    प्रत्यागम fs_data_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) ध्रुव {
    CV_Assert(nameofs < str_hash_data.size());
    प्रत्यागम std::string(&str_hash_data[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { प्रत्यागम fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(एतत्);
}

FileStorage::FileStorage(ध्रुव String& filename, पूर्णाङ्क flags, ध्रुव String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(एतत्);
    तार्किक ok = p->open(filename.c_str(), flags, encoding.c_str());
    जइ(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

शून्य FileStorage::startWriteStruct(ध्रुव String& name, पूर्णाङ्क struct_flags, ध्रुव String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    जइ ((struct_flags & File_ग्रन्थिः::TYPE_MASK) == File_ग्रन्थिः::SEQ)
        state = FileStorage::मानम्_EXPECTED;
    अण्णहा
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

शून्य FileStorage::endWriteStruct()
{
    p->endWriteStruct();
    state = p->write_राशिः.empty() || File_ग्रन्थिः::isMap(p->write_राशिः.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::मानम्_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

तार्किक FileStorage::open(ध्रुव String& filename, पूर्णाङ्क flags, ध्रुव String& encoding)
{
    यत्न
    {
        तार्किक ok = p->open(filename.c_str(), flags, encoding.c_str());
        जइ(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        प्रत्यागम ok;
    }
    गृहाण (...)
    {
        release();
        क्षिप;  // re-throw
    }
}

तार्किक FileStorage::isOpened() ध्रुव { प्रत्यागम p->is_opened; }

शून्य FileStorage::release()
{
    p->release();
}

File_ग्रन्थिः FileStorage::मूलम्(पूर्णाङ्क i) ध्रुव
{
    जइ( p.empty() || p->roots.empty() || i < 0 || i >= (पूर्णाङ्क)p->roots.size() )
        प्रत्यागम File_ग्रन्थिः();

    प्रत्यागम p->roots[i];
}

File_ग्रन्थिः FileStorage::get_First_शिखरम्_Level_ग्रन्थिः() ध्रुव
{
    File_ग्रन्थिः r = मूलम्();
    File_ग्रन्थिः_Iterator it = r.begin();
    प्रत्यागम it != r.end() ? *it : File_ग्रन्थिः();
}

std::string FileStorage::getDefaultObjectName(ध्रुव std::string& _filename)
{
    स्थिर ध्रुव वर्ण* stubname = "unnamed";
    ध्रुव वर्ण* filename = _filename.c_str();
    ध्रुव वर्ण* ptr2 = filename + _filename.size();
    ध्रुव वर्ण* ptr = ptr2 - 1;
    cv::AutoBuffer<वर्ण> name_buf(_filename.size()+1);

    जाव( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        जइ( *ptr == '.' && (!*ptr2 || strncmp(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    जइ( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    वर्ण* name = name_buf.data();

    // name must start with letter or '_'
    जइ( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    जाव( ptr < ptr2 )
    {
        वर्ण c = *ptr++;
        जइ( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.data();
    जइ( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    प्रत्यागम name;
}


पूर्णाङ्क FileStorage::getFormat() ध्रुव
{
    प्रत्यागम p->fmt;
}

File_ग्रन्थिः FileStorage::operator [](ध्रुव वर्ण* कुञ्चिका) ध्रुव
{
    प्रत्यागम एतत्->operator[](std::string(कुञ्चिका));
}

File_ग्रन्थिः FileStorage::operator [](ध्रुव std::string& कुञ्चिका) ध्रुव
{
    File_ग्रन्थिः res;
    प्रति (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][कुञ्चिका];
        जइ (!res.empty())
            भङ्ग;
    }
    प्रत्यागम res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    प्रत्यागम buf;
}

शून्य FileStorage::writeRaw( ध्रुव String& fmt, ध्रुव शून्य* vec, size_t len )
{
    p->writeRawData(fmt, (ध्रुव uchar*)vec, len);
}

शून्य FileStorage::writeComment( ध्रुव String& comment, तार्किक eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

शून्य writeScalar( FileStorage& fs, पूर्णाङ्क value )
{
    fs.p->लिख(String(), value);
}

शून्य writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->लिख(String(), value);
}

शून्य writeScalar( FileStorage& fs, भिन्न value )
{
    fs.p->लिख(String(), (द्विगुण)value);
}

शून्य writeScalar( FileStorage& fs, द्विगुण value )
{
    fs.p->लिख(String(), value);
}

शून्य writeScalar( FileStorage& fs, ध्रुव String& value )
{
    fs.p->लिख(String(), value);
}

शून्य लिख( FileStorage& fs, ध्रुव String& name, पूर्णाङ्क value )
{
    fs.p->लिख(name, value);
}

शून्य लिख( FileStorage& fs, ध्रुव String& name, int64_t value )
{
    fs.p->लिख(name, value);
}

शून्य लिख( FileStorage& fs, ध्रुव String& name, भिन्न value )
{
    fs.p->लिख(name, (द्विगुण)value);
}

शून्य लिख( FileStorage& fs, ध्रुव String& name, द्विगुण value )
{
    fs.p->लिख(name, value);
}

शून्य लिख( FileStorage& fs, ध्रुव String& name, ध्रुव String& value )
{
    fs.p->लिख(name, value);
}

शून्य लिख( FileStorage& fs, ध्रुव String& name, तार्किक value )
{
    fs.p->लिख(name, value);
}

शून्य FileStorage::लिख(ध्रुव String& name, पूर्णाङ्क val) { p->लिख(name, val); }
शून्य FileStorage::लिख(ध्रुव String& name, तार्किक val) { p->लिख(name, val); }
शून्य FileStorage::लिख(ध्रुव String& name, int64_t val) { p->लिख(name, val); }
शून्य FileStorage::लिख(ध्रुव String& name, द्विगुण val) { p->लिख(name, val); }
शून्य FileStorage::लिख(ध्रुव String& name, ध्रुव String& val) { p->लिख(name, val); }
शून्य FileStorage::लिख(ध्रुव String& name, ध्रुव Mat& val) { cv::लिख(*एतत्, name, val); }
शून्य FileStorage::लिख(ध्रुव String& name, ध्रुव std::vector<String>& val) { cv::लिख(*एतत्, name, val); }

FileStorage& operator << (FileStorage& fs, ध्रुव String& str)
{
    गणना { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        मानम्_EXPECTED = FileStorage::मानम्_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    ध्रुव वर्ण* _str = str.c_str();
    जइ( !fs.isOpened() || !_str )
        प्रत्यागम fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    वर्ण c = *_str;

    जइ( c == '}' || c == ']' )
    {
        जइ( fs_impl->write_राशिः.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        पूर्णाङ्क struct_flags = fs_impl->write_राशिः.back().flags;
        वर्ण expected_bracket = File_ग्रन्थिः::isMap(struct_flags) ? '}' : ']';
        जइ( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_राशिः.empty());
        struct_flags = fs_impl->write_राशिः.back().flags;
        fs.state = File_ग्रन्थिः::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : मानम्_EXPECTED;
        fs.elname = String();
    }
    अण्णहा जइ( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        जइ (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = मानम्_EXPECTED + INSIDE_MAP;
    }
    अण्णहा जइ( (fs.state & 3) == मानम्_EXPECTED )
    {
        जइ( c == '{' || c == '[' )
        {
            पूर्णाङ्क struct_flags = c == '{' ? File_ग्रन्थिः::MAP : File_ग्रन्थिः::SEQ;
            fs.state = struct_flags == File_ग्रन्थिः::MAP ? INSIDE_MAP + NAME_EXPECTED : मानम्_EXPECTED;
            _str++;
            जइ( *_str == ':' )
            {
                _str++;
                जइ( !*_str )
                    struct_flags |= File_ग्रन्थिः::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        अण्णहा
        {
            लिख( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            जइ( fs.state == INSIDE_MAP + मानम्_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    अण्णहा
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    प्रत्यागम fs;
}


File_ग्रन्थिः::File_ग्रन्थिः()
    : fs(रिक्त)
{
    blockIdx = ofs = 0;
}

File_ग्रन्थिः::File_ग्रन्थिः(FileStorage::Impl* _fs, size_t _blockIdx, size_t _ofs)
    : fs(_fs)
{
    blockIdx = _blockIdx;
    ofs = _ofs;
}

File_ग्रन्थिः::File_ग्रन्थिः(ध्रुव FileStorage* _fs, size_t _blockIdx, size_t _ofs)
    : File_ग्रन्थिः(_fs->p.get(), _blockIdx, _ofs)
{
    // nothing
}

File_ग्रन्थिः::File_ग्रन्थिः(ध्रुव File_ग्रन्थिः& ग्रन्थिः)
{
    fs = ग्रन्थिः.fs;
    blockIdx = ग्रन्थिः.blockIdx;
    ofs = ग्रन्थिः.ofs;
}

File_ग्रन्थिः& File_ग्रन्थिः::operator=(ध्रुव File_ग्रन्थिः& ग्रन्थिः)
{
    fs = ग्रन्थिः.fs;
    blockIdx = ग्रन्थिः.blockIdx;
    ofs = ग्रन्थिः.ofs;
    प्रत्यागम *एतत्;
}

File_ग्रन्थिः File_ग्रन्थिः::operator[](ध्रुव std::string& nodename) ध्रुव
{
    जइ(!fs)
        प्रत्यागम File_ग्रन्थिः();

    CV_Assert( isMap() );

    unsigned कुञ्चिका = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_ग्रन्थिः_Iterator it = begin();

    प्रति( i = 0; i < sz; i++, ++it )
    {
        File_ग्रन्थिः n = *it;
        ध्रुव uchar* p = n.ptr();
        unsigned key2 = (unsigned)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_data.size() );
        जइ( कुञ्चिका == key2 )
            प्रत्यागम n;
    }
    प्रत्यागम File_ग्रन्थिः();
}

File_ग्रन्थिः File_ग्रन्थिः::operator[](ध्रुव वर्ण* nodename) ध्रुव
{
    प्रत्यागम एतत्->operator[](std::string(nodename));
}

File_ग्रन्थिः File_ग्रन्थिः::operator[](पूर्णाङ्क i) ध्रुव
{
    जइ(!fs)
        प्रत्यागम File_ग्रन्थिः();

    CV_Assert( isSeq() );

    पूर्णाङ्क sz = (पूर्णाङ्क)size();
    CV_Assert( 0 <= i && i < sz );

    File_ग्रन्थिः_Iterator it = begin();
    it += i;

    प्रत्यागम *it;
}

std::vector<String> File_ग्रन्थिः::कुञ्चिकाः() ध्रुव
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    प्रति (File_ग्रन्थिः_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    प्रत्यागम res;
}

पूर्णाङ्क File_ग्रन्थिः::type() ध्रुव
{
    ध्रुव uchar* p = ptr();
    जइ(!p)
        प्रत्यागम NONE;
    प्रत्यागम (*p & TYPE_MASK);
}

तार्किक File_ग्रन्थिः::isMap(पूर्णाङ्क flags) { प्रत्यागम (flags & TYPE_MASK) == MAP; }
तार्किक File_ग्रन्थिः::isSeq(पूर्णाङ्क flags) { प्रत्यागम (flags & TYPE_MASK) == SEQ; }
तार्किक File_ग्रन्थिः::isCollection(पूर्णाङ्क flags) { प्रत्यागम isMap(flags) || isSeq(flags); }
तार्किक File_ग्रन्थिः::isFlow(पूर्णाङ्क flags) { प्रत्यागम (flags & FLOW) != 0; }
तार्किक File_ग्रन्थिः::isEmptyCollection(पूर्णाङ्क flags) { प्रत्यागम (flags & EMPTY) != 0; }

तार्किक File_ग्रन्थिः::empty() ध्रुव   { प्रत्यागम fs == 0; }
तार्किक File_ग्रन्थिः::isNone() ध्रुव  { प्रत्यागम type() == NONE; }
तार्किक File_ग्रन्थिः::isSeq() ध्रुव   { प्रत्यागम type() == SEQ; }
तार्किक File_ग्रन्थिः::isMap() ध्रुव   { प्रत्यागम type() == MAP; }
तार्किक File_ग्रन्थिः::isInt() ध्रुव   { प्रत्यागम type() == INT;  }
तार्किक File_ग्रन्थिः::isReal() ध्रुव  { प्रत्यागम type() == REAL; }
तार्किक File_ग्रन्थिः::isString() ध्रुव { प्रत्यागम type() == STRING;  }
तार्किक File_ग्रन्थिः::isNamed() ध्रुव
{
    ध्रुव uchar* p = ptr();
    जइ(!p)
        प्रत्यागम मिच्छ;
    प्रत्यागम (*p & NAMED) != 0;
}

std::string File_ग्रन्थिः::name() ध्रुव
{
    ध्रुव uchar* p = ptr();
    जइ(!p)
        प्रत्यागम std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    प्रत्यागम fs->getName(nameofs);
}

File_ग्रन्थिः::operator पूर्णाङ्क() ध्रुव
{
    ध्रुव uchar* p = ptr();
    जइ(!p)
        प्रत्यागम 0;
    पूर्णाङ्क tag = *p;
    पूर्णाङ्क type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    जइ( type == INT )
    {
        प्रत्यागम readInt(p);
    }
    अण्णहा जइ( type == REAL )
    {
        प्रत्यागम cvRound(readReal(p));
    }
    अण्णहा
        प्रत्यागम 0x7fffffff;
}

File_ग्रन्थिः::operator int64_t() ध्रुव
{
    ध्रुव uchar* p = ptr();
    जइ(!p)
        प्रत्यागम 0;
    पूर्णाङ्क tag = *p;
    पूर्णाङ्क type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    जइ( type == INT )
    {
        प्रत्यागम readLong(p);
    }
    अण्णहा जइ( type == REAL )
    {
        प्रत्यागम cvRound(readReal(p));
    }
    अण्णहा
        प्रत्यागम 0x7fffffff;
}

File_ग्रन्थिः::operator भिन्न() ध्रुव
{
    ध्रुव uchar* p = ptr();
    जइ(!p)
        प्रत्यागम 0.f;
    पूर्णाङ्क tag = *p;
    पूर्णाङ्क type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    जइ( type == INT )
    {
        प्रत्यागम (भिन्न)readLong(p);
    }
    अण्णहा जइ( type == REAL )
    {
        प्रत्यागम (भिन्न)readReal(p);
    }
    अण्णहा
        प्रत्यागम FLT_MAX;
}

File_ग्रन्थिः::operator द्विगुण() ध्रुव
{
    ध्रुव uchar* p = ptr();
    जइ(!p)
        प्रत्यागम 0.f;
    पूर्णाङ्क tag = *p;
    पूर्णाङ्क type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    जइ( type == INT )
    {
        प्रत्यागम (द्विगुण)readLong(p);
    }
    अण्णहा जइ( type == REAL )
    {
        प्रत्यागम readReal(p);
    }
    अण्णहा
        प्रत्यागम DBL_MAX;
}

द्विगुण File_ग्रन्थिः::real() ध्रुव  { प्रत्यागम द्विगुण(*एतत्); }

std::string File_ग्रन्थिः::string() ध्रुव
{
    ध्रुव uchar* p = ptr();
    जइ( !p || (*p & TYPE_MASK) != STRING )
        प्रत्यागम std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(unsigned)readInt(p);
    प्रत्यागम std::string((ध्रुव वर्ण*)(p + 4), sz - 1);
}
Mat File_ग्रन्थिः::mat() ध्रुव { Mat value; पठ(*एतत्, value, Mat()); प्रत्यागम value; }

File_ग्रन्थिः_Iterator File_ग्रन्थिः::begin() ध्रुव { प्रत्यागम File_ग्रन्थिः_Iterator(*एतत्, मिच्छ); }
File_ग्रन्थिः_Iterator File_ग्रन्थिः::end() ध्रुव   { प्रत्यागम File_ग्रन्थिः_Iterator(*एतत्, सच्च); }

शून्य File_ग्रन्थिः::readRaw( ध्रुव std::string& fmt, शून्य* vec, size_t len ) ध्रुव
{
    File_ग्रन्थिः_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_ग्रन्थिः::size() ध्रुव
{
    ध्रुव uchar* p = ptr();
    जइ( !p )
        प्रत्यागम 0;
    पूर्णाङ्क tag = *p;
    पूर्णाङ्क tp = tag & TYPE_MASK;
    जइ( tp == MAP || tp == SEQ )
    {
        जइ( tag & NAMED )
            p += 4;
        प्रत्यागम (size_t)(unsigned)readInt(p + 5);
    }
    प्रत्यागम tp != NONE;
}

size_t File_ग्रन्थिः::rawSize() ध्रुव
{
    ध्रुव uchar* p0 = ptr(), *p = p0;
    जइ( !p )
        प्रत्यागम 0;
    पूर्णाङ्क tag = *p++;
    पूर्णाङ्क tp = tag & TYPE_MASK;
    जइ( tag & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    जइ( tp == INT )
        प्रत्यागम sz0 + 8;
    जइ( tp == REAL )
        प्रत्यागम sz0 + 8;
    जइ( tp == NONE )
        प्रत्यागम sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    प्रत्यागम sz0 + 4 + readInt(p);
}

uchar* File_ग्रन्थिः::ptr()
{
    प्रत्यागम !fs ? 0 : (uchar*)fs->get_ग्रन्थिः_Ptr(blockIdx, ofs);
}

ध्रुव uchar* File_ग्रन्थिः::ptr() ध्रुव
{
    प्रत्यागम !fs ? 0 : fs->get_ग्रन्थिः_Ptr(blockIdx, ofs);
}

शून्य File_ग्रन्थिः::set_मानम्( पूर्णाङ्क type, ध्रुव शून्य* value, पूर्णाङ्क len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    पूर्णाङ्क tag = *p;
    पूर्णाङ्क current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    पूर्णाङ्क sz = 1;

    जइ( tag & NAMED )
        sz += 4;

    जइ( type == INT )
        sz += 8;
    अण्णहा जइ( type == REAL )
        sz += 8;
    अण्णहा जइ( type == STRING )
    {
        जइ( len < 0 )
            len = (पूर्णाङ्क)strlen((ध्रुव वर्ण*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    अण्णहा
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_ग्रन्थिः_Space(*एतत्, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    जइ( tag & NAMED )
        p += 4;

    जइ( type == INT )
    {
        int64_t ival = *(ध्रुव int64_t*)value;
        writeInt(p, ival);
    }
    अण्णहा जइ( type == REAL )
    {
        द्विगुण dbval = *(ध्रुव द्विगुण*)value;
        writeReal(p, dbval);
    }
    अण्णहा जइ( type == STRING )
    {
        ध्रुव वर्ण* str = (ध्रुव वर्ण*)value;
        writeInt(p, len + 1);
        memcpy(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_ग्रन्थिः_Iterator::File_ग्रन्थिः_Iterator()
{
    fs = 0;
    blockIdx = 0;
    ofs = 0;
    blockSize = 0;
    ग्रन्थिः_NElems = 0;
    idx = 0;
}

File_ग्रन्थिः_Iterator::File_ग्रन्थिः_Iterator( ध्रुव File_ग्रन्थिः& ग्रन्थिः, तार्किक seekEnd )
{
    fs = ग्रन्थिः.fs;
    idx = 0;
    जइ( !fs )
        blockIdx = ofs = blockSize = ग्रन्थिः_NElems = 0;
    अण्णहा
    {
        blockIdx = ग्रन्थिः.blockIdx;
        ofs = ग्रन्थिः.ofs;

        तार्किक collection = ग्रन्थिः.isSeq() || ग्रन्थिः.isMap();
        जइ( ग्रन्थिः.isNone() )
        {
            ग्रन्थिः_NElems = 0;
        }
        अण्णहा जइ( !collection )
        {
            ग्रन्थिः_NElems = 1;
            जइ( seekEnd )
            {
                idx = 1;
                ofs += ग्रन्थिः.rawSize();
            }
        }
        अण्णहा
        {
            ग्रन्थिः_NElems = ग्रन्थिः.size();
            ध्रुव uchar* p0 = ग्रन्थिः.ptr(), *p = p0 + 1;
            जइ(*p0 & File_ग्रन्थिः::NAMED )
                p += 4;
            जइ( !seekEnd )
                ofs += (p - p0) + 8;
            अण्णहा
            {
                size_t rawsz = (size_t)(unsigned)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = ग्रन्थिः_NElems;
            }
        }
        fs->normalize_ग्रन्थिः_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
}

File_ग्रन्थिः_Iterator::File_ग्रन्थिः_Iterator(ध्रुव File_ग्रन्थिः_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    ग्रन्थिः_NElems = it.ग्रन्थिः_NElems;
    idx = it.idx;
}

File_ग्रन्थिः_Iterator& File_ग्रन्थिः_Iterator::operator=(ध्रुव File_ग्रन्थिः_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    ग्रन्थिः_NElems = it.ग्रन्थिः_NElems;
    idx = it.idx;
    प्रत्यागम *एतत्;
}

File_ग्रन्थिः File_ग्रन्थिः_Iterator::operator *() ध्रुव
{
    प्रत्यागम File_ग्रन्थिः(idx < ग्रन्थिः_NElems ? fs : रिक्त, blockIdx, ofs);
}

File_ग्रन्थिः_Iterator& File_ग्रन्थिः_Iterator::operator ++ ()
{
    जइ( idx == ग्रन्थिः_NElems || !fs )
        प्रत्यागम *एतत्;
    idx++;
    File_ग्रन्थिः n(fs, blockIdx, ofs);
    ofs += n.rawSize();
    जइ( ofs >= blockSize )
    {
        fs->normalize_ग्रन्थिः_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
    प्रत्यागम *एतत्;
}

File_ग्रन्थिः_Iterator File_ग्रन्थिः_Iterator::operator ++ (पूर्णाङ्क)
{
    File_ग्रन्थिः_Iterator it = *एतत्;
    ++(*एतत्);
    प्रत्यागम it;
}

File_ग्रन्थिः_Iterator& File_ग्रन्थिः_Iterator::operator += (पूर्णाङ्क _ofs)
{
    CV_Assert( _ofs >= 0 );
    प्रति( ; _ofs > 0; _ofs-- )
        एतत्->operator ++();
    प्रत्यागम *एतत्;
}

File_ग्रन्थिः_Iterator& File_ग्रन्थिः_Iterator::readRaw( ध्रुव String& fmt, शून्य* _data0, size_t maxsz)
{
    जइ( fs && idx < ग्रन्थिः_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        पूर्णाङ्क fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        पूर्णाङ्क fmt_pair_गणनम् = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        size_t esz = fs::calcStructSize( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        प्रति( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            प्रति( पूर्णाङ्क k = 0; k < fmt_pair_गणनम्; k++ )
            {
                पूर्णाङ्क elem_type = fmt_pairs[k*2+1];
                पूर्णाङ्क elem_size = CV_ELEM_SIZE(elem_type);

                पूर्णाङ्क गणनम्_2 = fmt_pairs[k*2];
                offset = alignSize( offset, elem_size );
                uchar* data = data0 + offset;

                प्रति( पूर्णाङ्क i = 0; i < गणनम्_2; i++, ++(*एतत्) )
                {
                    File_ग्रन्थिः ग्रन्थिः = *(*एतत्);
                    जइ( ग्रन्थिः.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_size == 8 ? (int64_t)ग्रन्थिः : (पूर्णाङ्क)ग्रन्थिः);
                        विकल्प( elem_type )
                        {
                        पक्ष CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(ival);
                            data++;
                            भङ्ग;
                        पक्ष CV_8S:
                            *(वर्ण*)data = saturate_cast<schar>(ival);
                            data++;
                            भङ्ग;
                        पक्ष CV_Bool:
                            *(तार्किक*)data = ival != 0;
                            data++;
                            भङ्ग;
                        पक्ष CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(ival);
                            data += परिमाण(ushort);
                            भङ्ग;
                        पक्ष CV_16S:
                            *(short*)data = saturate_cast<short>(ival);
                            data += परिमाण(short);
                            भङ्ग;
                        पक्ष CV_32U:
                            *(unsigned*)data = (unsigned)std::max(ival, (int64_t)0);
                            data += परिमाण(unsigned);
                            भङ्ग;
                        पक्ष CV_32S:
                            *(पूर्णाङ्क*)data = (पूर्णाङ्क)ival;
                            data += परिमाण(पूर्णाङ्क);
                            भङ्ग;
                        पक्ष CV_32F:
                            *(भिन्न*)data = (भिन्न)ival;
                            data += परिमाण(भिन्न);
                            भङ्ग;
                        पक्ष CV_64U:
                            *(uint64_t*)data = (uint64_t)ival;
                            data += परिमाण(uint64_t);
                            भङ्ग;
                        पक्ष CV_64S:
                            *(int64_t*)data = (int64_t)ival;
                            data += परिमाण(int64_t);
                            भङ्ग;
                        पक्ष CV_64F:
                            *(द्विगुण*)data = (द्विगुण)ival;
                            data += परिमाण(द्विगुण);
                            भङ्ग;
                        पक्ष CV_16F:
                            *(hfloat*)data = hfloat((भिन्न)ival);
                            data += परिमाण(hfloat);
                            भङ्ग;
                        पक्ष CV_16BF:
                            *(bfloat*)data = bfloat((भिन्न)ival);
                            data += परिमाण(bfloat);
                            भङ्ग;
                        पक्ष CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((भिन्न)ival);
                            data += परिमाण(fp8_t);
                            भङ्ग;
                        पक्ष CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((भिन्न)ival);
                            data += परिमाण(fp8a_t);
                            भङ्ग;
                        स्वभाव:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    अण्णहा जइ( ग्रन्थिः.isReal() )
                    {
                        द्विगुण fval = (द्विगुण)ग्रन्थिः;

                        विकल्प( elem_type )
                        {
                        पक्ष CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(fval);
                            data++;
                            भङ्ग;
                        पक्ष CV_8S:
                            *(वर्ण*)data = saturate_cast<schar>(fval);
                            data++;
                            भङ्ग;
                        पक्ष CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(fval);
                            data += परिमाण(ushort);
                            भङ्ग;
                        पक्ष CV_16S:
                            *(short*)data = saturate_cast<short>(fval);
                            data += परिमाण(short);
                            भङ्ग;
                        पक्ष CV_32U:
                            *(पूर्णाङ्क*)data = saturate_cast<unsigned>(fval);
                            data += परिमाण(पूर्णाङ्क);
                            भङ्ग;
                        पक्ष CV_32S:
                            *(पूर्णाङ्क*)data = saturate_cast<पूर्णाङ्क>(fval);
                            data += परिमाण(पूर्णाङ्क);
                            भङ्ग;
                        पक्ष CV_32F:
                            *(भिन्न*)data = (भिन्न)fval;
                            data += परिमाण(भिन्न);
                            भङ्ग;
                        पक्ष CV_64U:
                            *(uint64_t*)data = (uint64_t)round(std::max(fval, 0.));
                            data += परिमाण(uint64_t);
                            भङ्ग;
                        पक्ष CV_64S:
                            *(int64_t*)data = (int64_t)round(std::max(fval, 0.));
                            data += परिमाण(int64_t);
                            भङ्ग;
                        पक्ष CV_64F:
                            *(द्विगुण*)data = fval;
                            data += परिमाण(द्विगुण);
                            भङ्ग;
                        पक्ष CV_16F:
                            *(hfloat*)data = hfloat((भिन्न)fval);
                            data += परिमाण(hfloat);
                            भङ्ग;
                        पक्ष CV_16BF:
                            *(bfloat*)data = bfloat((भिन्न)fval);
                            data += परिमाण(bfloat);
                            भङ्ग;
                        पक्ष CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((भिन्न)fval);
                            data += परिमाण(fp8_t);
                            भङ्ग;
                        पक्ष CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((भिन्न)fval);
                            data += परिमाण(fp8a_t);
                            भङ्ग;
                        स्वभाव:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    अण्णहा
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (पूर्णाङ्क)(data - data0);
            }
        }
    }

    प्रत्यागम *एतत्;
}

तार्किक File_ग्रन्थिः_Iterator::equalTo(ध्रुव File_ग्रन्थिः_Iterator& it) ध्रुव
{
    प्रत्यागम fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && ग्रन्थिः_NElems == it.ग्रन्थिः_NElems;
}

size_t File_ग्रन्थिः_Iterator::remaining() ध्रुव
{
    प्रत्यागम ग्रन्थिः_NElems - idx;
}

तार्किक operator == ( ध्रुव File_ग्रन्थिः_Iterator& it1, ध्रुव File_ग्रन्थिः_Iterator& it2 )
{
    प्रत्यागम it1.equalTo(it2);
}

तार्किक operator != ( ध्रुव File_ग्रन्थिः_Iterator& it1, ध्रुव File_ग्रन्थिः_Iterator& it2 )
{
    प्रत्यागम !it1.equalTo(it2);
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, पूर्णाङ्क& val, पूर्णाङ्क default_val)
{
    val = default_val;
    जइ( !ग्रन्थिः.empty() )
    {
        val = (पूर्णाङ्क)ग्रन्थिः;
    }
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, int64_t& val, int64_t default_val)
{
    val = default_val;
    जइ( !ग्रन्थिः.empty() )
    {
        val = (int64_t)ग्रन्थिः;
    }
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, द्विगुण& val, द्विगुण default_val)
{
    val = default_val;
    जइ( !ग्रन्थिः.empty() )
    {
        val = (द्विगुण)ग्रन्थिः;
    }
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, भिन्न& val, भिन्न default_val)
{
    val = default_val;
    जइ( !ग्रन्थिः.empty() )
    {
        val = (भिन्न)ग्रन्थिः;
    }
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, std::string& val, ध्रुव std::string& default_val)
{
    val = default_val;
    जइ( !ग्रन्थिः.empty() )
    {
        val = (std::string)ग्रन्थिः;
    }
}
शून्य FileStorage::Impl::लिख(ध्रुव String &कुञ्चिका, तार्किक value)
{
    CV_Assert(write_mode);
    जइ (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().लिख(कुञ्चिका.c_str(), (पूर्णाङ्क)value);
    }
    अण्णहा
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().लिख(कुञ्चिका.c_str(), value ? "true" : "false", मिच्छ);
    }
}

FileStorage_API::~FileStorage_API() {}

नामाकाश internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, ध्रुव std::string& name,
                                       पूर्णाङ्क flags, ध्रुव std::string& typeName)
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
