// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "persistence.hpp"
#அந்தர்பாவ "persistence_impl.hpp"
#அந்தர்பாவ "persistence_base64_encoding.hpp"
#அந்தர்பாவ <unordered_map>
#அந்தர்பாவ <iterator>

#அந்தர்பாவ <opencv2/core/utils/logger.hpp>

#define CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

நாமாகாஶ cv
{

நாமாகாஶ fs
{

முழு strcasecmp(மாறிலி எழுத்து* s1, மாறிலி எழுத்து* s2)
{
    மாறிலி எழுத்து* dummy="";
    எனில்(!s1) s1=dummy;
    எனில்(!s2) s2=dummy;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t i, len = std::min(len1, len2);
    க்கு( i = 0; i < len; i++ )
    {
        முழு d = tolower((முழு)s1[i]) - tolower((முழு)s2[i]);
        எனில்( d != 0 )
            திரும்பு d;
    }
    திரும்பு len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

எழுத்து* itoa( முழு _val, எழுத்து* buffer, முழு /*radix*/ )
{
    மாறிலி முழு radix = 10;
    எழுத்து* ptr=buffer + 23 /* enough even for 64-bit integers */;
    unsigned val = abs(_val);

    *ptr = '\0';
    செய்
    {
        unsigned r = val / radix;
        *--ptr = (எழுத்து)(val - (r*radix) + '0');
        val = r;
    }
    போது( val != 0 );

    எனில்( _val < 0 )
        *--ptr = '-';

    திரும்பு ptr;
}

எழுத்து* itoa( int64_t _val, எழுத்து* buffer, முழு /*radix*/, தருக்க _signed)
{
    மாறிலி முழு radix = 10;
    எழுத்து* ptr=buffer + 23 /* enough even for 64-bit integers */;
    முழு sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : abs(_val);

    *ptr = '\0';
    செய்
    {
        uint64_t r = val / radix;
        *--ptr = (எழுத்து)(val - (r*radix) + '0');
        val = r;
    }
    போது( val != 0 );

    எனில்( sign < 0 )
        *--ptr = '-';

    திரும்பு ptr;
}

எழுத்து* doubleToString( எழுத்து* buf, size_t bufSize, இரட்டை value, தருக்க explicitZero )
{
    Cv64suf val;
    unsigned ieee754_hi;

    val.f = value;
    ieee754_hi = (unsigned)(val.u >> 32);

    எனில்( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        முழு ivalue = cvRound(value);
        எனில்( ivalue == value )
        {
            எனில்( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            இல்லையெனில்
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        இல்லையெனில்
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, bufSize, "%.17g", value );

            எழுத்து* ptr = buf;
            எனில்( *ptr == '+' || *ptr == '-' )
                ptr++;
            க்கு( ; cv_isdigit(*ptr); ptr++ )
                ;
            எனில்( *ptr == ',' )
                *ptr = '.';
        }
    }
    இல்லையெனில்
    {
        unsigned ieee754_lo = (unsigned)val.u;
        எனில்( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        இல்லையெனில்
            strcpy( buf, (முழு)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    திரும்பு buf;
}

எழுத்து* floatToString( எழுத்து* buf, size_t bufSize, தசம value, தருக்க halfprecision, தருக்க explicitZero )
{
    Cv32suf val;
    unsigned ieee754;
    val.f = value;
    ieee754 = val.u;

    எனில்( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        முழு ivalue = cvRound(value);
        எனில்( ivalue == value )
        {
            எனில்( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            இல்லையெனில்
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        இல்லையெனில்
        {
            எனில் (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.5g", value);
            }
            இல்லையெனில்
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.9g", value);
            }

            எழுத்து* ptr = buf;
            எனில்( *ptr == '+' || *ptr == '-' )
                ptr++;
            க்கு( ; cv_isdigit(*ptr); ptr++ )
                ;
            எனில்( *ptr == ',' )
                *ptr = '.';
        }
    }
    இல்லையெனில்
    {
        எனில்( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        இல்லையெனில்
            strcpy( buf, (முழு)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    திரும்பு buf;
}

நிலையான மாறிலி எழுத்து symbols[] = "ucwsifdhHbUIneE";

நிலையான எழுத்து type_குறியீடு(முழு depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    திரும்பு symbols[depth];
}

நிலையான முழு குறியீடு_To_Type(எழுத்து c)
{
    எனில் (c == 'r')
        திரும்பு CV_SEQ_ELTYPE_PTR;
    மாறிலி எழுத்து* pos = strchr( symbols, c );
    எனில்( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    திரும்பு static_cast<முழு>(pos - symbols);
}

எழுத்து* encodeFormat(முழு elem_type, எழுத்து* dt, size_t dt_len)
{
    முழு cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    எழுத்து குறியீடு = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_குறியீடு(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, குறியீடு);
    திரும்பு dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
எழுத்து* encodeFormat(முழு elem_type, எழுத்து* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    திரும்பு encodeFormat(elem_type, dt, max);
}

முழு decodeFormat( மாறிலி எழுத்து* dt, முழு* fmt_pairs, முழு max_len )
{
    முழு fmt_pair_மொத்தம் = 0;
    முழு i = 0, k = 0, len = dt ? (முழு)strlen(dt) : 0;

    எனில்( !dt || !len )
        திரும்பு 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    க்கு( ; k < len; k++ )
    {
        எழுத்து c = dt[k];

        எனில்( cv_isdigit(c) )
        {
            முழு மொத்தம்_2 = c - '0';
            எனில்( cv_isdigit(dt[k+1]) )
            {
                எழுத்து* endptr = 0;
                மொத்தம்_2 = (முழு)strtol( dt+k, &endptr, 10 );
                k = (முழு)(endptr - dt) - 1;
            }

            எனில்( மொத்தம்_2 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = மொத்தம்_2;
        }
        இல்லையெனில்
        {
            முழு depth = குறியீடு_To_Type(c);
            எனில்( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = depth;
            எனில்( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            இல்லையெனில்
            {
                i += 2;
                எனில்( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_மொத்தம் = i/2;
    திரும்பு fmt_pair_மொத்தம்;
}

முழு calcElemSize( மாறிலி எழுத்து* dt, முழு initial_size )
{
    முழு size = 0;
    முழு fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_மொத்தம்;
    முழு comp_size;

    fmt_pair_மொத்தம் = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_மொத்தம் *= 2;
    க்கு( i = 0, size = initial_size; i < fmt_pair_மொத்தம்; i += 2 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[i+1]);
        size = cvAlign( size, comp_size );
        size += comp_size * fmt_pairs[i];
    }
    எனில்( initial_size == 0 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[1]);
        size = cvAlign( size, comp_size );
    }
    திரும்பு size;
}


முழு calcStructSize( மாறிலி எழுத்து* dt, முழு initial_size )
{
    முழு size = calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    க்கு ( மாறிலி எழுத்து * type = dt; *type != '\0'; type++ )
    {
        எழுத்து v = *type;
        எனில் (v >= '0' && v <= '9')
            தொடர்;  // skip vector size
        தேர்வு (v)
        {
        நிலை 'u': { elem_max_size = std::max( elem_max_size, அளவு(uchar ) ); நிறுத்து; }
        நிலை 'b': { elem_max_size = std::max( elem_max_size, அளவு(தருக்க  ) ); நிறுத்து; }
        நிலை 'c': { elem_max_size = std::max( elem_max_size, அளவு(schar ) ); நிறுத்து; }
        நிலை 'w': { elem_max_size = std::max( elem_max_size, அளவு(ushort) ); நிறுத்து; }
        நிலை 's': { elem_max_size = std::max( elem_max_size, அளவு(short ) ); நிறுத்து; }
        நிலை 'i': { elem_max_size = std::max( elem_max_size, அளவு(முழு   ) ); நிறுத்து; }
        நிலை 'n': { elem_max_size = std::max( elem_max_size, அளவு(unsigned) ); நிறுத்து; }
        நிலை 'f': { elem_max_size = std::max( elem_max_size, அளவு(தசம ) ); நிறுத்து; }
        நிலை 'd': { elem_max_size = std::max( elem_max_size, அளவு(இரட்டை) ); நிறுத்து; }
        நிலை 'h': { elem_max_size = std::max( elem_max_size, அளவு(hfloat)); நிறுத்து; }
        நிலை 'H': { elem_max_size = std::max( elem_max_size, அளவு(bfloat)); நிறுத்து; }
        நிலை 'e': { elem_max_size = std::max( elem_max_size, அளவு(fp8_t) ); நிறுத்து; }
        நிலை 'E': { elem_max_size = std::max( elem_max_size, அளவு(fp8a_t)); நிறுத்து; }
        நிலை 'I': { elem_max_size = std::max( elem_max_size, அளவு(int64_t)); நிறுத்து; }
        நிலை 'U': { elem_max_size = std::max( elem_max_size, அளவு(uint64_t)); நிறுத்து; }
        இயல்பு:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (எழுத்து)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<முழு>(elem_max_size) );
    திரும்பு size;
}

முழு decodeSimpleFormat( மாறிலி எழுத்து* dt )
{
    முழு elem_type = -1;
    முழு fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_மொத்தம்;

    fmt_pair_மொத்தம் = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    எனில்( fmt_pair_மொத்தம் != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    திரும்பு elem_type;
}

}

#எனில் defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#define CV_LITTLE_ENDIAN_MEM_ACCESS 1
#இல்லையெனில்
#define CV_LITTLE_ENDIAN_MEM_ACCESS 0
#endif

நிலையான உள்வரி முழு readInt(மாறிலி uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#எனில் CV_LITTLE_ENDIAN_MEM_ACCESS
    முழு val;
    memcpy(&val, p, அளவு(val));
    திரும்பு val;
#இல்லையெனில்
    முழு val = (முழு)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    திரும்பு val;
#endif
}

நிலையான உள்வரி int64_t readLong(மாறிலி uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#எனில் CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    memcpy(&val, p, அளவு(val));
    திரும்பு val;
#இல்லையெனில்
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    திரும்பு val0 | ((int64_t)val1 << 32);
#endif
}

நிலையான உள்வரி இரட்டை readReal(மாறிலி uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#எனில் CV_LITTLE_ENDIAN_MEM_ACCESS
    இரட்டை val;
    memcpy(&val, p, அளவு(val));
    திரும்பு val;
#இல்லையெனில்
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    திரும்பு val.f;
#endif
}

template <typename T>
நிலையான உள்வரி வெறுமை writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#எனில் CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &ival, அளவு(ival));
#இல்லையெனில்
    க்கு (size_t i = 0, j = 0; i < அளவு(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#endif
}

நிலையான உள்வரி வெறுமை writeReal(uchar* p, இரட்டை fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#எனில் CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &fval, அளவு(fval));
#இல்லையெனில்
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



வெறுமை FileStorage::Impl::init() {
    flags = 0;
    buffer.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = பொய்;
    state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = பொய்;
    delayed_struct_சாவி = nullptr;
    delayed_struct_flags = 0;
    delayed_type_name = nullptr;
    base64_writer = nullptr;
    is_opened = பொய்;
    dummy_eof = பொய்;
    write_mode = பொய்;
    mem_mode = பொய்;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    empty_stream = உண்மை;

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

வெறுமை FileStorage::Impl::release(String *out) {
    எனில் (is_opened) {
        எனில் (out)
            out->clear();
        எனில் (write_mode) {
            போது (write_அடுக்கு.size() > 1) {
                endWriteStruct();
            }
            flush();
            எனில் (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            இல்லையெனில் எனில் (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        எனில் (mem_mode && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

வெறுமை FileStorage::Impl::analyze_file_name(மாறிலி std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    நிலையான மாறிலி எழுத்து not_file_name = '\n';
    நிலையான மாறிலி எழுத்து parameter_begin = '?';
    நிலையான மாறிலி எழுத்து parameter_separator = '&';

    எனில் (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        திரும்பு;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    எனில் (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        க்கு (size_t param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            எனில் ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

தருக்க FileStorage::Impl::open(மாறிலி எழுத்து *filename_or_buf, முழு _flags, மாறிலி எழுத்து *encoding) {
    தருக்க ok = உண்மை;
    release();

    தருக்க append = (_flags & 3) == FileStorage::APPEND;
    mem_mode = (_flags & FileStorage::MEMORY) != 0;

    write_mode = (_flags & 3) != 0;
    தருக்க write_base64 = (write_mode || append) && (_flags & FileStorage::BASE64) != 0;

    தருக்க isGZ = பொய்;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        எனில் (!params.empty())
            filename = params[0];

        எனில் (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_mode || append);
    }

    எனில் (filename.size() == 0 && !mem_mode && !write_mode)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    எனில் (mem_mode && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    எனில் (!mem_mode) {
        size_t dot_idx = filename.find_last_of('.');
        எழுத்து compression = '\0';

        எனில் (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            எனில் (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                எனில் (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = உண்மை;
                }

                இல்லையெனில் எனில்(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = உண்மை;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        எனில் (!isGZ) {
            file = fopen(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            எனில் (!file)
            {
                CV_LOG_ERROR(வெற்று, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                திரும்பு பொய்;
            }
        } இல்லையெனில் {
#எனில் USE_ZLIB
            எனில் (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            எழுத்து mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            எனில் (!gzfile)
            {
                CV_LOG_ERROR(வெற்று, "Can't open archive: '" << filename << "' mode=" << mode);
                திரும்பு பொய்;
            }
#இல்லையெனில்
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#endif
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_data.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    எனில் (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        எனில் (mem_mode)
            outbuf.clear();

        எனில் (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            மாறிலி எழுத்து *dot_pos = வெற்று;
            மாறிலி எழுத்து *dot_pos2 = வெற்று;
            // like strrchr() implementation, but save two last positions simultaneously
            க்கு (மாறிலி எழுத்து *pos = &filename[0]; pos[0] != 0; pos++) {
                எனில் (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            எனில் (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != வெற்று) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } இல்லையெனில் எனில் (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        முழு buf_size = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        எனில் (append) {
            fseek(file, 0, SEEK_END);
            எனில் (ftell(file) == 0)
                append = பொய்;
        }

        write_அடுக்கு.clear();
        empty_stream = உண்மை;
        write_அடுக்கு.push_back(FStructData("", File_முனை::MAP | File_முனை::EMPTY, 0));
        buffer.reserve(buf_size + 1024);
        buffer.resize(buf_size);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;

        எனில் (fmt == FileStorage::FORMAT_XML) {
            size_t file_size = file ? (size_t) ftell(file) : (size_t) 0;
            எனில் (!append || file_size == 0) {
                எனில் (encoding && *encoding != '\0') {
                    எனில் (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(strlen(encoding) < 1000);
                    எழுத்து buf[1100];
                    snprintf(buf, அளவு(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } இல்லையெனில்
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } இல்லையெனில் {
                முழு xml_buf_size = 1 << 10;
                எழுத்து substr[] = "</opencv_storage>";
                முழு last_occurrence = -1;
                xml_buf_size = MIN(xml_buf_size, முழு(file_size));
                fseek(file, -xml_buf_size, SEEK_END);
                // find the last occurrence of </opencv_storage>
                க்கு (;;) {
                    முழு வரி_offset = (முழு) ftell(file);
                    மாறிலி எழுத்து *ptr0 = இது->gets(xml_buf_size);
                    மாறிலி எழுத்து *ptr = வெற்று;
                    எனில் (!ptr0)
                        நிறுத்து;
                    ptr = ptr0;
                    க்கு (;;) {
                        ptr = strstr(ptr, substr);
                        எனில் (!ptr)
                            நிறுத்து;
                        last_occurrence = வரி_offset + (முழு) (ptr - ptr0);
                        ptr += strlen(substr);
                    }
                }
                எனில் (last_occurrence < 0) {
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

            emitter_do_not_use_direct_dereference = createXMLEmitter(இது);
        } இல்லையெனில் எனில் (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                எனில் (!append) {
                    எனில் (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    இல்லையெனில்
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            இல்லையெனில்
                puts("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(இது);
        } இல்லையெனில் {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            எனில் (!append)
                puts("{\n");
            இல்லையெனில் {
                தருக்க valid = பொய்;
                long roffset = 0;
                க்கு (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    மாறிலி எழுத்து end_mark = '}';
                    எனில் (fgetc(file) == end_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = உண்மை;
                        நிறுத்து;
                    }
                }

                எனில் (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } இல்லையெனில் {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_அடுக்கு.back().indent = 4;
            emitter_do_not_use_direct_dereference = createJSONEmitter(இது);
        }
        is_opened = உண்மை;
    } இல்லையெனில் {
        மாறிலி size_t buf_size0 = 40;
        buffer.resize(buf_size0);
        எனில் (mem_mode) {
            strbuf = (எழுத்து *) filename_or_buf;
            strbufsize = strlen(strbuf);
        }

        மாறிலி எழுத்து *yaml_signature = "%YAML";
        மாறிலி எழுத்து *json_signature = "{";
        மாறிலி எழுத்து *xml_signature = "<?xml";
        எழுத்து *buf = இது->gets(16);
        CV_Assert(buf);
        எழுத்து *bufPtr = cv_skip_BOM(buf);
        size_t bufOffset = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        எனில் (strncmp(bufPtr, yaml_signature, strlen(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        இல்லையெனில் எனில் (strncmp(bufPtr, json_signature, strlen(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        இல்லையெனில் எனில் (strncmp(bufPtr, xml_signature, strlen(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        இல்லையெனில்
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = bufOffset;
        bufofs = 0;

        முயல் {
            எழுத்து *ptr = bufferStart();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_முனை வேர்_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_முனை_Space(வேர்_nodes, 9);
            *rptr = File_முனை::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            தேர்வு (fmt) {
                நிலை FileStorage::FORMAT_XML:
                    parser_do_not_use_direct_dereference = createXMLParser(இது);
                    நிறுத்து;
                நிலை FileStorage::FORMAT_YAML:
                நிலை FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_direct_dereference = createYAMLParser(இது);
                    நிறுத்து;
                நிலை FileStorage::FORMAT_JSON:
                    parser_do_not_use_direct_dereference = createJSONParser(இது);
                    நிறுத்து;
                இயல்பு:
                    parser_do_not_use_direct_dereference = Ptr<FileStorageParser>();
            }

            எனில் (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                எனில் (ok) {
                    finalizeCollection(வேர்_nodes);

                    CV_Assert(!fs_data_ptrs.empty());
                    File_முனை roots_முனை(fs_ext, 0, 0);
                    size_t i, nroots = roots_முனை.size();
                    File_முனை_Iterator it = roots_முனை.begin();

                    க்கு (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        பிடி (...)
        {
            // FIXIT log error message
            is_opened = உண்மை;
            release();
            எறி;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = உண்மை;
        std::vector<எழுத்து> tmpbuf;
        std::swap(buffer, tmpbuf);
        bufofs = 0;
    }
    திரும்பு ok;
}

வெறுமை FileStorage::Impl::puts(மாறிலி எழுத்து *str) {
    CV_Assert(write_mode);
    எனில் (mem_mode)
        std::copy(str, str + strlen(str), std::back_inserter(outbuf));
    இல்லையெனில் எனில் (file)
        fputs(str, file);
#எனில் USE_ZLIB
    இல்லையெனில் எனில் (gzfile)
        gzputs(gzfile, str);
#endif
    இல்லையெனில்
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

எழுத்து *FileStorage::Impl::getsFromFile(எழுத்து *buf, முழு மொத்தம்_2) {
    எனில் (file)
        திரும்பு fgets(buf, மொத்தம்_2, file);
#எனில் USE_ZLIB
    எனில் (gzfile)
        திரும்பு gzgets(gzfile, buf, மொத்தம்_2);
#endif
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

எழுத்து *FileStorage::Impl::gets(size_t max_மொத்தம்) {
    எனில் (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        மாறிலி எழுத்து *instr = strbuf;
        க்கு (; i < len; i++) {
            எழுத்து c = instr[i];
            எனில் (c == '\0' || c == '\n') {
                எனில் (c == '\n')
                    i++;
                நிறுத்து;
            }
        }
        size_t மொத்தம்_2 = i - strbufpos;
        எனில் (max_மொத்தம் == 0 || max_மொத்தம் > மொத்தம்_2)
            max_மொத்தம் = மொத்தம்_2;
        buffer.resize(std::max(buffer.size(), max_மொத்தம் + 8));
        memcpy(&buffer[0], instr + strbufpos, max_மொத்தம்);
        buffer[max_மொத்தம்] = '\0';
        strbufpos = i;
        திரும்பு max_மொத்தம் > 0 ? &buffer[0] : 0;
    }

    மாறிலி size_t MAX_BLOCK_SIZE = INT_MAX / 2; // hopefully, that will be enough
    எனில் (max_மொத்தம் == 0)
        max_மொத்தம் = MAX_BLOCK_SIZE;
    இல்லையெனில்
        CV_Assert(max_மொத்தம் < MAX_BLOCK_SIZE);
    size_t ofs = 0;

    க்கு (;;) {
        முழு மொத்தம்_2 = (முழு) std::min(buffer.size() - ofs - 16, max_மொத்தம்);
        எழுத்து *ptr = getsFromFile(&buffer[ofs], மொத்தம்_2 + 1);
        எனில் (!ptr)
            நிறுத்து;
        முழு delta = (முழு) strlen(ptr);
        ofs += delta;
        max_மொத்தம் -= delta;
        எனில் (delta == 0 || ptr[delta - 1] == '\n' || max_மொத்தம் == 0)
            நிறுத்து;
        எனில் (delta == மொத்தம்_2)
            buffer.resize((size_t) (buffer.size() * 1.5));
    }
    திரும்பு ofs > 0 ? &buffer[0] : 0;
}

எழுத்து *FileStorage::Impl::gets() {
    எழுத்து *ptr = இது->gets(0);
    எனில் (!ptr) {
        ptr = bufferStart();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        திரும்பு 0;
    } இல்லையெனில் {
        size_t l = strlen(ptr);
        எனில் (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    திரும்பு ptr;
}

தருக்க FileStorage::Impl::eof() {
    எனில் (dummy_eof)
        திரும்பு உண்மை;
    எனில் (strbuf)
        திரும்பு strbufpos >= strbufsize;
    எனில் (file)
        திரும்பு feof(file) != 0;
#எனில் USE_ZLIB
    எனில் (gzfile)
        திரும்பு gzeof(gzfile) != 0;
#endif
    திரும்பு பொய்;
}

வெறுமை FileStorage::Impl::setEof() {
    dummy_eof = உண்மை;
}

வெறுமை FileStorage::Impl::closeFile() {
    எனில் (file)
        fclose(file);
#எனில் USE_ZLIB
    இல்லையெனில் எனில் (gzfile)
        gzclose(gzfile);
#endif
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = பொய்;
}

வெறுமை FileStorage::Impl::rewind() {
    எனில் (file)
        ::rewind(file);
#எனில் USE_ZLIB
    இல்லையெனில் எனில் (gzfile)
        gzrewind(gzfile);
#endif
    strbufpos = 0;
}

எழுத்து *FileStorage::Impl::resizeWriteBuffer(எழுத்து *ptr, முழு len) {
    மாறிலி எழுத்து *buffer_end = &buffer[0] + buffer.size();
    எனில் (ptr + len < buffer_end)
        திரும்பு ptr;

    மாறிலி எழுத்து *buffer_start = &buffer[0];
    முழு written_len = (முழு) (ptr - buffer_start);

    CV_Assert(written_len <= (முழு) buffer.size());
    முழு new_size = (முழு) ((buffer_end - buffer_start) * 3 / 2);
    new_size = MAX(written_len + len, new_size);
    buffer.reserve(new_size + 256);
    buffer.resize(new_size);
    bufofs = written_len;
    திரும்பு &buffer[0] + bufofs;
}

எழுத்து *FileStorage::Impl::flush() {
    எழுத்து *buffer_start = &buffer[0];
    எழுத்து *ptr = buffer_start + bufofs;

    எனில் (ptr > buffer_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(buffer_start);
        bufofs = 0;
    }

    முழு indent = write_அடுக்கு.back().indent;

    எனில் (space != indent) {
        memset(buffer_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = buffer_start + bufofs;

    திரும்பு ptr;
}

வெறுமை FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(பொய்);
    எனில் (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_அடுக்கு.empty());

    FStructData &current_struct = write_அடுக்கு.back();
    எனில் (fmt == FileStorage::FORMAT_JSON && !File_முனை::isFlow(current_struct.flags) && write_அடுக்கு.size() > 1)
        current_struct.indent = write_அடுக்கு[write_அடுக்கு.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_அடுக்கு.pop_back();
    எனில் (!write_அடுக்கு.empty())
        write_அடுக்கு.back().flags &= ~File_முனை::EMPTY;
}

வெறுமை FileStorage::Impl::startWriteStruct_helper(மாறிலி எழுத்து *சாவி, முழு struct_flags,
                                                மாறிலி எழுத்து *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (File_முனை::TYPE_MASK | File_முனை::FLOW)) | File_முனை::EMPTY;
    எனில் (!File_முனை::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    எனில் (type_name && type_name[0] == '\0')
        type_name = 0;

    FStructData s = getEmitter().startWriteStruct(write_அடுக்கு.back(), சாவி, struct_flags, type_name);

    write_அடுக்கு.push_back(s);
    size_t write_அடுக்கு_size = write_அடுக்கு.size();
    எனில் (write_அடுக்கு_size > 1)
        write_அடுக்கு[write_அடுக்கு_size - 2].flags &= ~File_முனை::EMPTY;

    எனில் (fmt != FileStorage::FORMAT_JSON && !File_முனை::isFlow(s.flags))
        flush();

    எனில் (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_முனை::isMap(struct_flags)) {
        getEmitter().லிக("type_id", type_name, பொய்);
    }
}

வெறுமை FileStorage::Impl::startWriteStruct(மாறிலி எழுத்து *சாவி, முழு struct_flags,
                                         மாறிலி எழுத்து *type_name) {
    check_if_write_struct_is_delayed(பொய்);
    எனில் (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    எனில் (state_of_writing_base64 == FileStorage_API::Uncertain && File_முனை::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(சாவி, struct_flags, type_name);
    } இல்லையெனில் எனில் (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        எனில் ((File_முனை::TYPE_MASK & struct_flags) != File_முனை::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        இல்லையெனில் எனில் (state_of_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(சாவி, struct_flags, "binary");

        எனில் (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::InUse);
    } இல்லையெனில் {
        /* Won't output Base64 data */
        எனில் (state_of_writing_base64 == FileStorage_API::InUse)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(சாவி, struct_flags, type_name);

        எனில் (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::NotUse);
    }
}

வெறுமை FileStorage::Impl::writeComment(மாறிலி எழுத்து *comment, தருக்க eol_comment) {
    CV_Assert(write_mode);
    getEmitter().writeComment(comment, eol_comment);
}

வெறுமை FileStorage::Impl::startNextStream() {
    CV_Assert(write_mode);
    எனில் (!empty_stream) {
        போது (!write_அடுக்கு.empty())
            endWriteStruct();
        flush();
        getEmitter().startNextStream();
        empty_stream = உண்மை;
        write_அடுக்கு.push_back(FStructData("", File_முனை::EMPTY, 0));
        bufofs = 0;
    }
}

வெறுமை FileStorage::Impl::லிக(மாறிலி String &சாவி, முழு value) {
    CV_Assert(write_mode);
    getEmitter().லிக(சாவி.c_str(), value);
}

வெறுமை FileStorage::Impl::லிக(மாறிலி String &சாவி, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().லிக(சாவி.c_str(), value);
}

வெறுமை FileStorage::Impl::லிக(மாறிலி String &சாவி, இரட்டை value) {
    CV_Assert(write_mode);
    getEmitter().லிக(சாவி.c_str(), value);
}

வெறுமை FileStorage::Impl::லிக(மாறிலி String &சாவி, மாறிலி String &value) {
    CV_Assert(write_mode);
    getEmitter().லிக(சாவி.c_str(), value.c_str(), பொய்);
}

வெறுமை FileStorage::Impl::writeRawData(மாறிலி std::string &dt, மாறிலி வெறுமை *_data, size_t len) {
    CV_Assert(write_mode);

    எனில் (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        writeRawDataBase64(_data, len, dt.c_str());
        திரும்பு;
    } இல்லையெனில் எனில் (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elemSize = fs::calcStructSize(dt.c_str(), 0);
    CV_Assert(elemSize);
    CV_Assert(len % elemSize == 0);
    len /= elemSize;

    தருக்க explicitZero = fmt == FileStorage::FORMAT_JSON;
    மாறிலி uchar *data0 = (மாறிலி uchar *) _data;
    முழு fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_மொத்தம்;
    எழுத்து buf[256] = "";

    fmt_pair_மொத்தம் = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    எனில் (!len)
        திரும்பு;

    எனில் (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    எனில் (fmt_pair_மொத்தம் == 1) {
        fmt_pairs[0] *= (முழு) len;
        len = 1;
    }

    க்கு (; len--; data0 += elemSize) {
        முழு offset = 0;
        க்கு (k = 0; k < fmt_pair_மொத்தம்; k++) {
            முழு i, மொத்தம்_2 = fmt_pairs[k * 2];
            முழு elem_type = fmt_pairs[k * 2 + 1];
            முழு elem_size = CV_ELEM_SIZE(elem_type);
            மாறிலி எழுத்து *ptr;

            offset = cvAlign(offset, elem_size);
            மாறிலி uchar *data = data0 + offset;

            க்கு (i = 0; i < மொத்தம்_2; i++) {
                தேர்வு (elem_type) {
                    நிலை CV_8U:
                        ptr = fs::itoa(*(uchar *) data, buf, 10);
                        data++;
                        நிறுத்து;
                    நிலை CV_Bool:
                        ptr = fs::itoa(*(uchar *) data != 0, buf, 10);
                        data++;
                        நிறுத்து;
                    நிலை CV_8S:
                        ptr = fs::itoa(*(எழுத்து *) data, buf, 10);
                        data++;
                        நிறுத்து;
                    நிலை CV_16U:
                        ptr = fs::itoa(*(ushort *) data, buf, 10);
                        data += அளவு(ushort);
                        நிறுத்து;
                    நிலை CV_16S:
                        ptr = fs::itoa(*(short *) data, buf, 10);
                        data += அளவு(short);
                        நிறுத்து;
                    நிலை CV_32U:
                        ptr = fs::itoa((int64_t)*(unsigned*) data, buf, 10, பொய்);
                        data += அளவு(unsigned);
                        நிறுத்து;
                    நிலை CV_32S:
                        ptr = fs::itoa(*(முழு *) data, buf, 10);
                        data += அளவு(முழு);
                        நிறுத்து;
                    நிலை CV_64U:
                        ptr = fs::itoa(*(uint64_t*) data, buf, 10, பொய்);
                        data += அளவு(uint64_t);
                        நிறுத்து;
                    நிலை CV_64S:
                        ptr = fs::itoa(*(int64_t*) data, buf, 10, உண்மை);
                        data += அளவு(int64_t);
                        நிறுத்து;
                    நிலை CV_32F:
                        ptr = fs::floatToString(buf, அளவு(buf), *(தசம *) data, பொய், explicitZero);
                        data += அளவு(தசம);
                        நிறுத்து;
                    நிலை CV_64F:
                        ptr = fs::doubleToString(buf, அளவு(buf), *(இரட்டை *) data, explicitZero);
                        data += அளவு(இரட்டை);
                        நிறுத்து;
                    நிலை CV_16F:
                        ptr = fs::floatToString(buf, அளவு(buf), (தசம) *(hfloat *) data, உண்மை, explicitZero);
                        data += அளவு(hfloat);
                        நிறுத்து;
                    நிலை CV_16BF:
                        ptr = fs::floatToString(buf, அளவு(buf), (தசம) *(bfloat *) data, உண்மை, explicitZero);
                        data += அளவு(bfloat);
                        நிறுத்து;
                    நிலை CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, அளவு(buf), (தசம) *(fp8_t *) data, உண்மை, explicitZero);
                        data += அளவு(fp8_t);
                        நிறுத்து;
                    நிலை CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, அளவு(buf), (தசம) *(fp8a_t *) data, உண்மை, explicitZero);
                        data += அளவு(fp8a_t);
                        நிறுத்து;
                    இயல்பு:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        திரும்பு;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (முழு) (data - data0);
        }
    }
}

வெறுமை FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(பொய்);

    எனில் (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_to_Base64_state(FileStorage_API::Base64State::Uncertain);
}

வெறுமை FileStorage::Impl::switch_to_Base64_state(FileStorage_API::Base64State new_state) {
    மாறிலி எழுத்து *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    மாறிலி எழுத்து *err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    தேர்வு (state_of_writing_base64) {
        நிலை FileStorage_API::Base64State::Uncertain:
            தேர்வு (new_state) {
                நிலை FileStorage_API::Base64State::InUse:
                {
                    CV_DbgAssert(base64_writer == 0);
                    தருக்க can_indent = (fmt != cv::FileStorage::Mode::FORMAT_JSON);
                    base64_writer = புதிய base64::Base64Writer(*இது, can_indent);
                    எனில் (!can_indent) {
                        எழுத்து *ptr = bufferPtr();
                        *ptr++ = '\0';
                        puts(bufferStart());
                        setBufferPtr(bufferStart());
                        memset(bufferStart(), 0, static_cast<முழு>(space));
                        puts("\"$base64$");
                    }
                    நிறுத்து;
                }
                நிலை FileStorage_API::Base64State::Uncertain:
                    நிறுத்து;
                நிலை FileStorage_API::Base64State::NotUse:
                    நிறுத்து;
                இயல்பு:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    நிறுத்து;
            }
            நிறுத்து;
        நிலை FileStorage_API::Base64State::InUse:
            தேர்வு (new_state) {
                நிலை FileStorage_API::Base64State::InUse:
                நிலை FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    நிறுத்து;
                நிலை FileStorage_API::Base64State::Uncertain:
                    delete base64_writer;
                    base64_writer = 0;
                    எனில் ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        setBufferPtr(bufferStart());
                        flush();
                        memset(bufferStart(), 0, static_cast<முழு>(space) );
                        setBufferPtr(bufferStart());
                    }
                    நிறுத்து;
                இயல்பு:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    நிறுத்து;
            }
            நிறுத்து;
        நிலை FileStorage_API::Base64State::NotUse:
            தேர்வு (new_state) {
                நிலை FileStorage_API::Base64State::InUse:
                நிலை FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    நிறுத்து;
                நிலை FileStorage_API::Base64State::Uncertain:
                    நிறுத்து;
                இயல்பு:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    நிறுத்து;
            }
            நிறுத்து;
        இயல்பு:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            நிறுத்து;
    }

    state_of_writing_base64 = new_state;
}

வெறுமை FileStorage::Impl::make_write_struct_delayed(மாறிலி எழுத்து *சாவி, முழு struct_flags, மாறிலி எழுத்து *type_name) {
    CV_Assert(is_write_struct_delayed == பொய்);
    CV_DbgAssert(delayed_struct_சாவி == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    எனில் (சாவி != nullptr) {
        delayed_struct_சாவி = புதிய எழுத்து[strlen(சாவி) + 1U];
        strcpy(delayed_struct_சாவி, சாவி);
    }

    எனில் (type_name != nullptr) {
        delayed_type_name = புதிய எழுத்து[strlen(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = உண்மை;
}

வெறுமை FileStorage::Impl::check_if_write_struct_is_delayed(தருக்க change_type_to_base64) {
    எனில் (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_சாவி;
        std::string type_name;
        முழு struct_flags = delayed_struct_flags;

        எனில் (delayed_struct_சாவி != nullptr && *delayed_struct_சாவி != '\0') {
            struct_சாவி.assign(delayed_struct_சாவி);
        }
        எனில் (delayed_type_name != nullptr && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        delete[] delayed_struct_சாவி;
        delete[] delayed_type_name;
        delayed_struct_சாவி = nullptr;
        delayed_struct_flags = 0;
        delayed_type_name = nullptr;

        is_write_struct_delayed = பொய்;

        /* call */
        எனில் (change_type_to_base64) {
            startWriteStruct_helper(struct_சாவி.c_str(), struct_flags, "binary");
            எனில் (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::InUse);
        } இல்லையெனில் {
            startWriteStruct_helper(struct_சாவி.c_str(), struct_flags, type_name.c_str());
            எனில் (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::NotUse);
        }
    }
}

வெறுமை FileStorage::Impl::writeRawDataBase64(மாறிலி வெறுமை *_data, size_t len, மாறிலி எழுத்து *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(உண்மை);

    எனில் (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } இல்லையெனில் எனில் (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->லிக(_data, len, dt);
}

File_முனை FileStorage::Impl::get_First_உச்சி_Level_முனை() மாறிலி {
    திரும்பு roots.empty() ? File_முனை() : roots[0];
}

File_முனை FileStorage::Impl::வேர்(முழு streamIdx) மாறிலி {
    திரும்பு streamIdx >= 0 && streamIdx < (முழு) roots.size() ? roots[streamIdx] : File_முனை();
}

File_முனை FileStorage::Impl::operator[](மாறிலி String &nodename) மாறிலி {
    திரும்பு இது->operator[](nodename.c_str());
}

File_முனை FileStorage::Impl::operator[](மாறிலி எழுத்து * /*nodename*/) மாறிலி {
    திரும்பு File_முனை();
}

முழு FileStorage::Impl::getFormat() மாறிலி { திரும்பு fmt; }

எழுத்து *FileStorage::Impl::bufferPtr() மாறிலி { திரும்பு (எழுத்து *) (&buffer[0] + bufofs); }

எழுத்து *FileStorage::Impl::bufferStart() மாறிலி { திரும்பு (எழுத்து *) &buffer[0]; }

எழுத்து *FileStorage::Impl::bufferEnd() மாறிலி { திரும்பு (எழுத்து *) (&buffer[0] + buffer.size()); }

வெறுமை FileStorage::Impl::setBufferPtr(எழுத்து *ptr) {
    எழுத்து *bufferstart = bufferStart();
    CV_Assert(ptr >= bufferstart && ptr <= bufferEnd());
    bufofs = ptr - bufferstart;
}

முழு FileStorage::Impl::wrapMargin() மாறிலி { திரும்பு wrap_margin; }

FStructData &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_அடுக்கு.empty());
    திரும்பு write_அடுக்கு.back();
}

வெறுமை FileStorage::Impl::setNonEmpty() {
    empty_stream = பொய்;
}

வெறுமை FileStorage::Impl::processSpecialDouble(எழுத்து *buf, இரட்டை *value, எழுத்து **endptr) {
    FileStorage_API *fs = இது;
    எழுத்து c = buf[0];
    முழு inf_hi = 0x7ff00000;

    எனில் (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    எனில் (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    எனில் (toupper(buf[1]) == 'I' && toupper(buf[2]) == 'N' && toupper(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    இல்லையெனில் எனில் (toupper(buf[1]) == 'N' && toupper(buf[2]) == 'A' && toupper(buf[3]) == 'N')
        v.u = (uint64) -1;
    இல்லையெனில்
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

இரட்டை FileStorage::Impl::strtod(எழுத்து *ptr, எழுத்து **endptr) {
    இரட்டை fval = ::strtod(ptr, endptr);
    எனில் (**endptr == '.') {
        எழுத்து *dot_pos = *endptr;
        *dot_pos = ',';
        இரட்டை fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        எனில் (*endptr > dot_pos)
            fval = fval2;
        இல்லையெனில்
            *endptr = dot_pos;
    }

    எனில் (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    திரும்பு fval;
}

வெறுமை FileStorage::Impl::convertToCollection(முழு type, File_முனை &முனை) {
    CV_Assert(type == File_முனை::SEQ || type == File_முனை::MAP);

    முழு முனை_type = முனை.type();
    எனில் (முனை_type == type)
        திரும்பு;

    தருக்க named = முனை.isNamed();
    uchar *ptr = முனை.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    இரட்டை fval = 0;
    std::string sval;
    தருக்க add_first_scalar = பொய்;

    எனில் (முனை_type != File_முனை::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_முனை::SEQ);
        எனில் (முனை_type == File_முனை::INT) {
            ival = readLong(ptr);
            add_first_scalar = உண்மை;
        } இல்லையெனில் எனில் (முனை_type == File_முனை::REAL) {
            fval = readReal(ptr);
            add_first_scalar = உண்மை;
        } இல்லையெனில் எனில் (முனை_type == File_முனை::STRING) {
            sval = std::string(முனை);
            add_first_scalar = உண்மை;
        } இல்லையெனில்
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", முனை_type));
    }

    ptr = reserve_முனை_Space(முனை, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_முனை::NAMED : 0));
    // name has been copied automatically
    எனில் (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    எனில் (add_first_scalar)
        add_முனை(முனை, std::string(), முனை_type,
                முனை_type == File_முனை::INT ? (மாறிலி வெறுமை *) &ival :
                முனை_type == File_முனை::REAL ? (மாறிலி வெறுமை *) &fval :
                முனை_type == File_முனை::STRING ? (மாறிலி வெறுமை *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_முனை_Space(File_முனை &முனை, size_t sz) {
    தருக்க shrinkBlock = பொய்;
    size_t shrinkBlockIdx = 0, shrinkSize = 0;

    uchar *ptr = 0, *blockEnd = 0;

    எனில் (!fs_data_ptrs.empty()) {
        size_t blockIdx = முனை.blockIdx;
        size_t ofs = முனை.ofs;
        CV_Assert(blockIdx == fs_data_ptrs.size() - 1);
        CV_Assert(ofs <= fs_data_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_data_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_data_ptrs[blockIdx] + ofs;
        blockEnd = fs_data_ptrs[blockIdx] + fs_data_blksz[blockIdx];

        CV_Assert(ptr >= fs_data_ptrs[blockIdx] && ptr <= blockEnd);
        எனில் (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            திரும்பு ptr;
        }

        எனில் (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_data[blockIdx]->resize(sz);
            ptr = &fs_data[blockIdx]->at(0);
            fs_data_ptrs[blockIdx] = ptr;
            fs_data_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            திரும்பு ptr;
        }

        shrinkBlock = உண்மை;
        shrinkBlockIdx = blockIdx;
        shrinkSize = ofs;
    }

    size_t blockSize = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(blockSize);
    fs_data.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_data_ptrs.push_back(new_ptr);
    fs_data_blksz.push_back(blockSize);
    முனை.blockIdx = fs_data_ptrs.size() - 1;
    முனை.ofs = 0;
    freeSpaceOfs = sz;

    எனில் (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        எனில் (ptr[0] & File_முனை::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    எனில் (shrinkBlock) {
        fs_data[shrinkBlockIdx]->resize(shrinkSize);
        fs_data_blksz[shrinkBlockIdx] = shrinkSize;
    }

    திரும்பு new_ptr;
}

unsigned FileStorage::Impl::getStringOfs(மாறிலி std::string &சாவி) மாறிலி {
    str_hash_t::const_iterator it = str_hash.find(சாவி);
    திரும்பு it != str_hash.end() ? it->second : 0;
}

File_முனை FileStorage::Impl::add_முனை(File_முனை &collection, மாறிலி std::string &சாவி,
                                    முழு elem_type, மாறிலி வெறுமை *value, முழு len) {
    FileStorage_API *fs = இது;
    தருக்க noname = சாவி.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(சாவி.c_str(), "_") == 0);
    convertToCollection(noname ? File_முனை::SEQ : File_முனை::MAP, collection);

    தருக்க isseq = collection.empty() ? பொய் : collection.isSeq();
    எனில் (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    unsigned strofs = 0;
    எனில் (!noname) {
        strofs = getStringOfs(சாவி);
        எனில் (!strofs) {
            strofs = (unsigned) str_hash_data.size();
            size_t keysize = சாவி.size() + 1;
            str_hash_data.resize(strofs + keysize);
            memcpy(&str_hash_data[0] + strofs, &சாவி[0], keysize);
            str_hash.insert(std::make_pair(சாவி, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t blockIdx = fs_data_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_முனை முனை(fs_ext, blockIdx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_முனை_Space(முனை, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_முனை::NAMED));
    எனில் (elem_type == File_முனை::NONE)
        freeSpaceOfs -= 8;

    எனில் (!noname) {
        writeInt(ptr, (முழு) strofs);
        ptr += 4;
    }

    எனில் (elem_type == File_முனை::SEQ || elem_type == File_முனை::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    எனில் (value)
        முனை.set_மதிப்பு(elem_type, value, len);

    எனில் (collection.isNamed())
        cp += 4;
    முழு nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    திரும்பு முனை;
}

வெறுமை FileStorage::Impl::finalizeCollection(File_முனை &collection) {
    எனில் (!collection.isSeq() && !collection.isMap())
        திரும்பு;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    எனில் (*ptr0 & File_முனை::NAMED)
        ptr += 4;
    size_t blockIdx = collection.blockIdx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t rawSize = 4;
    unsigned sz = (unsigned) readInt(ptr + 4);
    எனில் (sz > 0) {
        size_t lastBlockIdx = fs_data_ptrs.size() - 1;

        க்கு (; blockIdx < lastBlockIdx; blockIdx++) {
            rawSize += fs_data_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    rawSize += freeSpaceOfs - ofs;
    writeInt(ptr, (முழு) rawSize);
}

வெறுமை FileStorage::Impl::normalize_முனை_Ofs(size_t &blockIdx, size_t &ofs) மாறிலி {
    போது (ofs >= fs_data_blksz[blockIdx]) {
        எனில் (blockIdx == fs_data_blksz.size() - 1) {
            CV_Assert(ofs == fs_data_blksz[blockIdx]);
            நிறுத்து;
        }
        ofs -= fs_data_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    திரும்பு state_of_writing_base64;
}

முழு FileStorage::Impl::get_space() {
    திரும்பு space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = உண்மை;
}

வெறுமை FileStorage::Impl::Base64Decoder::init(மாறிலி Ptr<FileStorageParser> &_parser, எழுத்து *_ptr, முழு _indent) {
    parser_do_not_use_direct_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = பொய்;
}

தருக்க FileStorage::Impl::Base64Decoder::readMore(முழு needed) {
    நிலையான மாறிலி uchar base64tab[] =
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

    எனில் (eos)
        திரும்பு பொய்;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    க்கு (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    எழுத்து *beg = 0, *end = 0;
    தருக்க ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    எனில் (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = உண்மை;
        size_t tc = totalchars;
        க்கு (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    முழு i = 0, j, n = (முழு) encoded.size();
    எனில் (n > 0) {
        மாறிலி uchar *tab = base64tab;
        எழுத்து *src = &encoded[0];

        க்கு (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(முழு) (uchar) src[i]], c = tab[(முழு) (uchar) src[i + 1]];
            uchar b = tab[(முழு) (uchar) src[i + 2]], a = tab[(முழு) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    எனில் (i > 0 && encoded[i - 1] == '=') {
        எனில் (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        எனில் (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    க்கு (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    திரும்பு (முழு) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    எனில் (ofs >= sz && !readMore(1))
        திரும்பு (uchar) 0;
    திரும்பு decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    எனில் (ofs + 2 > sz && !readMore(2))
        திரும்பு (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    திரும்பு val;
}

முழு FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    எனில் (ofs + 4 > sz && !readMore(4))
        திரும்பு 0;
    முழு ival = readInt(&decoded[ofs]);
    ofs += 4;
    திரும்பு ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    எனில் (ofs + 8 > sz && !readMore(8))
        திரும்பு 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    திரும்பு ival;
}

இரட்டை FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    எனில் (ofs + 8 > sz && !readMore(8))
        திரும்பு 0;
    இரட்டை fval = readReal(&decoded[ofs]);
    ofs += 8;
    திரும்பு fval;
}

தருக்க FileStorage::Impl::Base64Decoder::endOfStream() மாறிலி { திரும்பு eos; }

எழுத்து *FileStorage::Impl::Base64Decoder::getPtr() மாறிலி { திரும்பு ptr; }


எழுத்து *FileStorage::Impl::parseBase64(எழுத்து *ptr, முழு indent, File_முனை &collection) {
    மாறிலி முழு BASE64_HDR_SIZE = 24;
    எழுத்து dt[BASE64_HDR_SIZE + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    முழு i, k;

    க்கு (i = 0; i < BASE64_HDR_SIZE; i++)
        dt[i] = (எழுத்து) base64decoder.getUInt8();
    க்கு (i = 0; i < BASE64_HDR_SIZE; i++)
        எனில் (isspace(dt[i]))
            நிறுத்து;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    முழு fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    முழு fmt_pair_மொத்தம் = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    இரட்டை fval = 0;

    க்கு (;;) {
        க்கு (k = 0; k < fmt_pair_மொத்தம்; k++) {
            முழு elem_type = fmt_pairs[k * 2 + 1];
            முழு மொத்தம்_2 = fmt_pairs[k * 2];

            க்கு (i = 0; i < மொத்தம்_2; i++) {
                முழு முனை_type = File_முனை::INT;
                தேர்வு (elem_type) {
                    நிலை CV_8U:
                        ival = base64decoder.getUInt8();
                        நிறுத்து;
                    நிலை CV_8S:
                        ival = (எழுத்து) base64decoder.getUInt8();
                        நிறுத்து;
                    நிலை CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        நிறுத்து;
                    நிலை CV_16U:
                        ival = base64decoder.getUInt16();
                        நிறுத்து;
                    நிலை CV_16S:
                        ival = (short) base64decoder.getUInt16();
                        நிறுத்து;
                    நிலை CV_32U:
                        ival = base64decoder.getInt32();
                        நிறுத்து;
                    நிலை CV_32S:
                        ival = base64decoder.getInt32();
                        நிறுத்து;
                    நிலை CV_64U:
                        ival = base64decoder.getInt64();
                        நிறுத்து;
                    நிலை CV_64S:
                        ival = base64decoder.getInt64();
                        நிறுத்து;
                    நிலை CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        முனை_type = File_முனை::REAL;
                    }
                        நிறுத்து;
                    நிலை CV_64F:
                        fval = base64decoder.getFloat64();
                        முனை_type = File_முனை::REAL;
                        நிறுத்து;
                    நிலை CV_16F:
                        fval = தசம(hfloatFromBits(base64decoder.getUInt16()));
                        முனை_type = File_முனை::REAL;
                        நிறுத்து;
                    நிலை CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        முனை_type = File_முனை::REAL;
                        நிறுத்து;
                    நிலை CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        முனை_type = File_முனை::REAL;
                        நிறுத்து;
                    இயல்பு:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                எனில் (base64decoder.endOfStream())
                    நிறுத்து;
                add_முனை(collection, std::string(), முனை_type,
                        முனை_type == File_முனை::INT ? (வெறுமை *) &ival : (வெறுமை *) &fval, -1);
            }
        }
        எனில் (base64decoder.endOfStream())
            நிறுத்து;
    }

    finalizeCollection(collection);
    திரும்பு base64decoder.getPtr();
}

வெறுமை FileStorage::Impl::parseError(மாறிலி எழுத்து *func_name, மாறிலி std::string &err_செய்தி, மாறிலி எழுத்து *source_file,
                                   முழு source_வரி) {
    std::string செய்தி = format("%s(%d): %s", filename.c_str(), lineno, err_செய்தி.c_str());
    error(Error::StsParseError, func_name, செய்தி.c_str(), source_file, source_வரி);
}

மாறிலி uchar *FileStorage::Impl::get_முனை_Ptr(size_t blockIdx, size_t ofs) மாறிலி {
    CV_Assert(blockIdx < fs_data_ptrs.size());
    CV_Assert(ofs < fs_data_blksz[blockIdx]);

    திரும்பு fs_data_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) மாறிலி {
    CV_Assert(nameofs < str_hash_data.size());
    திரும்பு std::string(&str_hash_data[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { திரும்பு fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(இது);
}

FileStorage::FileStorage(மாறிலி String& filename, முழு flags, மாறிலி String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(இது);
    தருக்க ok = p->open(filename.c_str(), flags, encoding.c_str());
    எனில்(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

வெறுமை FileStorage::startWriteStruct(மாறிலி String& name, முழு struct_flags, மாறிலி String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    எனில் ((struct_flags & File_முனை::TYPE_MASK) == File_முனை::SEQ)
        state = FileStorage::மதிப்பு_EXPECTED;
    இல்லையெனில்
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

வெறுமை FileStorage::endWriteStruct()
{
    p->endWriteStruct();
    state = p->write_அடுக்கு.empty() || File_முனை::isMap(p->write_அடுக்கு.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::மதிப்பு_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

தருக்க FileStorage::open(மாறிலி String& filename, முழு flags, மாறிலி String& encoding)
{
    முயல்
    {
        தருக்க ok = p->open(filename.c_str(), flags, encoding.c_str());
        எனில்(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        திரும்பு ok;
    }
    பிடி (...)
    {
        release();
        எறி;  // re-throw
    }
}

தருக்க FileStorage::isOpened() மாறிலி { திரும்பு p->is_opened; }

வெறுமை FileStorage::release()
{
    p->release();
}

File_முனை FileStorage::வேர்(முழு i) மாறிலி
{
    எனில்( p.empty() || p->roots.empty() || i < 0 || i >= (முழு)p->roots.size() )
        திரும்பு File_முனை();

    திரும்பு p->roots[i];
}

File_முனை FileStorage::get_First_உச்சி_Level_முனை() மாறிலி
{
    File_முனை r = வேர்();
    File_முனை_Iterator it = r.begin();
    திரும்பு it != r.end() ? *it : File_முனை();
}

std::string FileStorage::getDefaultObjectName(மாறிலி std::string& _filename)
{
    நிலையான மாறிலி எழுத்து* stubname = "unnamed";
    மாறிலி எழுத்து* filename = _filename.c_str();
    மாறிலி எழுத்து* ptr2 = filename + _filename.size();
    மாறிலி எழுத்து* ptr = ptr2 - 1;
    cv::AutoBuffer<எழுத்து> name_buf(_filename.size()+1);

    போது( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        எனில்( *ptr == '.' && (!*ptr2 || strncmp(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    எனில்( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    எழுத்து* name = name_buf.data();

    // name must start with letter or '_'
    எனில்( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    போது( ptr < ptr2 )
    {
        எழுத்து c = *ptr++;
        எனில்( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.data();
    எனில்( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    திரும்பு name;
}


முழு FileStorage::getFormat() மாறிலி
{
    திரும்பு p->fmt;
}

File_முனை FileStorage::operator [](மாறிலி எழுத்து* சாவி) மாறிலி
{
    திரும்பு இது->operator[](std::string(சாவி));
}

File_முனை FileStorage::operator [](மாறிலி std::string& சாவி) மாறிலி
{
    File_முனை res;
    க்கு (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][சாவி];
        எனில் (!res.empty())
            நிறுத்து;
    }
    திரும்பு res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    திரும்பு buf;
}

வெறுமை FileStorage::writeRaw( மாறிலி String& fmt, மாறிலி வெறுமை* vec, size_t len )
{
    p->writeRawData(fmt, (மாறிலி uchar*)vec, len);
}

வெறுமை FileStorage::writeComment( மாறிலி String& comment, தருக்க eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

வெறுமை writeScalar( FileStorage& fs, முழு value )
{
    fs.p->லிக(String(), value);
}

வெறுமை writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->லிக(String(), value);
}

வெறுமை writeScalar( FileStorage& fs, தசம value )
{
    fs.p->லிக(String(), (இரட்டை)value);
}

வெறுமை writeScalar( FileStorage& fs, இரட்டை value )
{
    fs.p->லிக(String(), value);
}

வெறுமை writeScalar( FileStorage& fs, மாறிலி String& value )
{
    fs.p->லிக(String(), value);
}

வெறுமை லிக( FileStorage& fs, மாறிலி String& name, முழு value )
{
    fs.p->லிக(name, value);
}

வெறுமை லிக( FileStorage& fs, மாறிலி String& name, int64_t value )
{
    fs.p->லிக(name, value);
}

வெறுமை லிக( FileStorage& fs, மாறிலி String& name, தசம value )
{
    fs.p->லிக(name, (இரட்டை)value);
}

வெறுமை லிக( FileStorage& fs, மாறிலி String& name, இரட்டை value )
{
    fs.p->லிக(name, value);
}

வெறுமை லிக( FileStorage& fs, மாறிலி String& name, மாறிலி String& value )
{
    fs.p->லிக(name, value);
}

வெறுமை லிக( FileStorage& fs, மாறிலி String& name, தருக்க value )
{
    fs.p->லிக(name, value);
}

வெறுமை FileStorage::லிக(மாறிலி String& name, முழு val) { p->லிக(name, val); }
வெறுமை FileStorage::லிக(மாறிலி String& name, தருக்க val) { p->லிக(name, val); }
வெறுமை FileStorage::லிக(மாறிலி String& name, int64_t val) { p->லிக(name, val); }
வெறுமை FileStorage::லிக(மாறிலி String& name, இரட்டை val) { p->லிக(name, val); }
வெறுமை FileStorage::லிக(மாறிலி String& name, மாறிலி String& val) { p->லிக(name, val); }
வெறுமை FileStorage::லிக(மாறிலி String& name, மாறிலி Mat& val) { cv::லிக(*இது, name, val); }
வெறுமை FileStorage::லிக(மாறிலி String& name, மாறிலி std::vector<String>& val) { cv::லிக(*இது, name, val); }

FileStorage& operator << (FileStorage& fs, மாறிலி String& str)
{
    எண்ணிக்கை { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        மதிப்பு_EXPECTED = FileStorage::மதிப்பு_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    மாறிலி எழுத்து* _str = str.c_str();
    எனில்( !fs.isOpened() || !_str )
        திரும்பு fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    எழுத்து c = *_str;

    எனில்( c == '}' || c == ']' )
    {
        எனில்( fs_impl->write_அடுக்கு.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        முழு struct_flags = fs_impl->write_அடுக்கு.back().flags;
        எழுத்து expected_bracket = File_முனை::isMap(struct_flags) ? '}' : ']';
        எனில்( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_அடுக்கு.empty());
        struct_flags = fs_impl->write_அடுக்கு.back().flags;
        fs.state = File_முனை::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : மதிப்பு_EXPECTED;
        fs.elname = String();
    }
    இல்லையெனில் எனில்( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        எனில் (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = மதிப்பு_EXPECTED + INSIDE_MAP;
    }
    இல்லையெனில் எனில்( (fs.state & 3) == மதிப்பு_EXPECTED )
    {
        எனில்( c == '{' || c == '[' )
        {
            முழு struct_flags = c == '{' ? File_முனை::MAP : File_முனை::SEQ;
            fs.state = struct_flags == File_முனை::MAP ? INSIDE_MAP + NAME_EXPECTED : மதிப்பு_EXPECTED;
            _str++;
            எனில்( *_str == ':' )
            {
                _str++;
                எனில்( !*_str )
                    struct_flags |= File_முனை::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        இல்லையெனில்
        {
            லிக( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            எனில்( fs.state == INSIDE_MAP + மதிப்பு_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    இல்லையெனில்
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    திரும்பு fs;
}


File_முனை::File_முனை()
    : fs(வெற்று)
{
    blockIdx = ofs = 0;
}

File_முனை::File_முனை(FileStorage::Impl* _fs, size_t _blockIdx, size_t _ofs)
    : fs(_fs)
{
    blockIdx = _blockIdx;
    ofs = _ofs;
}

File_முனை::File_முனை(மாறிலி FileStorage* _fs, size_t _blockIdx, size_t _ofs)
    : File_முனை(_fs->p.get(), _blockIdx, _ofs)
{
    // nothing
}

File_முனை::File_முனை(மாறிலி File_முனை& முனை)
{
    fs = முனை.fs;
    blockIdx = முனை.blockIdx;
    ofs = முனை.ofs;
}

File_முனை& File_முனை::operator=(மாறிலி File_முனை& முனை)
{
    fs = முனை.fs;
    blockIdx = முனை.blockIdx;
    ofs = முனை.ofs;
    திரும்பு *இது;
}

File_முனை File_முனை::operator[](மாறிலி std::string& nodename) மாறிலி
{
    எனில்(!fs)
        திரும்பு File_முனை();

    CV_Assert( isMap() );

    unsigned சாவி = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_முனை_Iterator it = begin();

    க்கு( i = 0; i < sz; i++, ++it )
    {
        File_முனை n = *it;
        மாறிலி uchar* p = n.ptr();
        unsigned key2 = (unsigned)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_data.size() );
        எனில்( சாவி == key2 )
            திரும்பு n;
    }
    திரும்பு File_முனை();
}

File_முனை File_முனை::operator[](மாறிலி எழுத்து* nodename) மாறிலி
{
    திரும்பு இது->operator[](std::string(nodename));
}

File_முனை File_முனை::operator[](முழு i) மாறிலி
{
    எனில்(!fs)
        திரும்பு File_முனை();

    CV_Assert( isSeq() );

    முழு sz = (முழு)size();
    CV_Assert( 0 <= i && i < sz );

    File_முனை_Iterator it = begin();
    it += i;

    திரும்பு *it;
}

std::vector<String> File_முனை::சாவிகள்() மாறிலி
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    க்கு (File_முனை_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    திரும்பு res;
}

முழு File_முனை::type() மாறிலி
{
    மாறிலி uchar* p = ptr();
    எனில்(!p)
        திரும்பு NONE;
    திரும்பு (*p & TYPE_MASK);
}

தருக்க File_முனை::isMap(முழு flags) { திரும்பு (flags & TYPE_MASK) == MAP; }
தருக்க File_முனை::isSeq(முழு flags) { திரும்பு (flags & TYPE_MASK) == SEQ; }
தருக்க File_முனை::isCollection(முழு flags) { திரும்பு isMap(flags) || isSeq(flags); }
தருக்க File_முனை::isFlow(முழு flags) { திரும்பு (flags & FLOW) != 0; }
தருக்க File_முனை::isEmptyCollection(முழு flags) { திரும்பு (flags & EMPTY) != 0; }

தருக்க File_முனை::empty() மாறிலி   { திரும்பு fs == 0; }
தருக்க File_முனை::isNone() மாறிலி  { திரும்பு type() == NONE; }
தருக்க File_முனை::isSeq() மாறிலி   { திரும்பு type() == SEQ; }
தருக்க File_முனை::isMap() மாறிலி   { திரும்பு type() == MAP; }
தருக்க File_முனை::isInt() மாறிலி   { திரும்பு type() == INT;  }
தருக்க File_முனை::isReal() மாறிலி  { திரும்பு type() == REAL; }
தருக்க File_முனை::isString() மாறிலி { திரும்பு type() == STRING;  }
தருக்க File_முனை::isNamed() மாறிலி
{
    மாறிலி uchar* p = ptr();
    எனில்(!p)
        திரும்பு பொய்;
    திரும்பு (*p & NAMED) != 0;
}

std::string File_முனை::name() மாறிலி
{
    மாறிலி uchar* p = ptr();
    எனில்(!p)
        திரும்பு std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    திரும்பு fs->getName(nameofs);
}

File_முனை::operator முழு() மாறிலி
{
    மாறிலி uchar* p = ptr();
    எனில்(!p)
        திரும்பு 0;
    முழு tag = *p;
    முழு type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    எனில்( type == INT )
    {
        திரும்பு readInt(p);
    }
    இல்லையெனில் எனில்( type == REAL )
    {
        திரும்பு cvRound(readReal(p));
    }
    இல்லையெனில்
        திரும்பு 0x7fffffff;
}

File_முனை::operator int64_t() மாறிலி
{
    மாறிலி uchar* p = ptr();
    எனில்(!p)
        திரும்பு 0;
    முழு tag = *p;
    முழு type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    எனில்( type == INT )
    {
        திரும்பு readLong(p);
    }
    இல்லையெனில் எனில்( type == REAL )
    {
        திரும்பு cvRound(readReal(p));
    }
    இல்லையெனில்
        திரும்பு 0x7fffffff;
}

File_முனை::operator தசம() மாறிலி
{
    மாறிலி uchar* p = ptr();
    எனில்(!p)
        திரும்பு 0.f;
    முழு tag = *p;
    முழு type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    எனில்( type == INT )
    {
        திரும்பு (தசம)readLong(p);
    }
    இல்லையெனில் எனில்( type == REAL )
    {
        திரும்பு (தசம)readReal(p);
    }
    இல்லையெனில்
        திரும்பு FLT_MAX;
}

File_முனை::operator இரட்டை() மாறிலி
{
    மாறிலி uchar* p = ptr();
    எனில்(!p)
        திரும்பு 0.f;
    முழு tag = *p;
    முழு type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    எனில்( type == INT )
    {
        திரும்பு (இரட்டை)readLong(p);
    }
    இல்லையெனில் எனில்( type == REAL )
    {
        திரும்பு readReal(p);
    }
    இல்லையெனில்
        திரும்பு DBL_MAX;
}

இரட்டை File_முனை::real() மாறிலி  { திரும்பு இரட்டை(*இது); }

std::string File_முனை::string() மாறிலி
{
    மாறிலி uchar* p = ptr();
    எனில்( !p || (*p & TYPE_MASK) != STRING )
        திரும்பு std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(unsigned)readInt(p);
    திரும்பு std::string((மாறிலி எழுத்து*)(p + 4), sz - 1);
}
Mat File_முனை::mat() மாறிலி { Mat value; பட(*இது, value, Mat()); திரும்பு value; }

File_முனை_Iterator File_முனை::begin() மாறிலி { திரும்பு File_முனை_Iterator(*இது, பொய்); }
File_முனை_Iterator File_முனை::end() மாறிலி   { திரும்பு File_முனை_Iterator(*இது, உண்மை); }

வெறுமை File_முனை::readRaw( மாறிலி std::string& fmt, வெறுமை* vec, size_t len ) மாறிலி
{
    File_முனை_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_முனை::size() மாறிலி
{
    மாறிலி uchar* p = ptr();
    எனில்( !p )
        திரும்பு 0;
    முழு tag = *p;
    முழு tp = tag & TYPE_MASK;
    எனில்( tp == MAP || tp == SEQ )
    {
        எனில்( tag & NAMED )
            p += 4;
        திரும்பு (size_t)(unsigned)readInt(p + 5);
    }
    திரும்பு tp != NONE;
}

size_t File_முனை::rawSize() மாறிலி
{
    மாறிலி uchar* p0 = ptr(), *p = p0;
    எனில்( !p )
        திரும்பு 0;
    முழு tag = *p++;
    முழு tp = tag & TYPE_MASK;
    எனில்( tag & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    எனில்( tp == INT )
        திரும்பு sz0 + 8;
    எனில்( tp == REAL )
        திரும்பு sz0 + 8;
    எனில்( tp == NONE )
        திரும்பு sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    திரும்பு sz0 + 4 + readInt(p);
}

uchar* File_முனை::ptr()
{
    திரும்பு !fs ? 0 : (uchar*)fs->get_முனை_Ptr(blockIdx, ofs);
}

மாறிலி uchar* File_முனை::ptr() மாறிலி
{
    திரும்பு !fs ? 0 : fs->get_முனை_Ptr(blockIdx, ofs);
}

வெறுமை File_முனை::set_மதிப்பு( முழு type, மாறிலி வெறுமை* value, முழு len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    முழு tag = *p;
    முழு current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    முழு sz = 1;

    எனில்( tag & NAMED )
        sz += 4;

    எனில்( type == INT )
        sz += 8;
    இல்லையெனில் எனில்( type == REAL )
        sz += 8;
    இல்லையெனில் எனில்( type == STRING )
    {
        எனில்( len < 0 )
            len = (முழு)strlen((மாறிலி எழுத்து*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_முனை_Space(*இது, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    எனில்( tag & NAMED )
        p += 4;

    எனில்( type == INT )
    {
        int64_t ival = *(மாறிலி int64_t*)value;
        writeInt(p, ival);
    }
    இல்லையெனில் எனில்( type == REAL )
    {
        இரட்டை dbval = *(மாறிலி இரட்டை*)value;
        writeReal(p, dbval);
    }
    இல்லையெனில் எனில்( type == STRING )
    {
        மாறிலி எழுத்து* str = (மாறிலி எழுத்து*)value;
        writeInt(p, len + 1);
        memcpy(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_முனை_Iterator::File_முனை_Iterator()
{
    fs = 0;
    blockIdx = 0;
    ofs = 0;
    blockSize = 0;
    முனை_NElems = 0;
    idx = 0;
}

File_முனை_Iterator::File_முனை_Iterator( மாறிலி File_முனை& முனை, தருக்க seekEnd )
{
    fs = முனை.fs;
    idx = 0;
    எனில்( !fs )
        blockIdx = ofs = blockSize = முனை_NElems = 0;
    இல்லையெனில்
    {
        blockIdx = முனை.blockIdx;
        ofs = முனை.ofs;

        தருக்க collection = முனை.isSeq() || முனை.isMap();
        எனில்( முனை.isNone() )
        {
            முனை_NElems = 0;
        }
        இல்லையெனில் எனில்( !collection )
        {
            முனை_NElems = 1;
            எனில்( seekEnd )
            {
                idx = 1;
                ofs += முனை.rawSize();
            }
        }
        இல்லையெனில்
        {
            முனை_NElems = முனை.size();
            மாறிலி uchar* p0 = முனை.ptr(), *p = p0 + 1;
            எனில்(*p0 & File_முனை::NAMED )
                p += 4;
            எனில்( !seekEnd )
                ofs += (p - p0) + 8;
            இல்லையெனில்
            {
                size_t rawsz = (size_t)(unsigned)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = முனை_NElems;
            }
        }
        fs->normalize_முனை_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
}

File_முனை_Iterator::File_முனை_Iterator(மாறிலி File_முனை_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    முனை_NElems = it.முனை_NElems;
    idx = it.idx;
}

File_முனை_Iterator& File_முனை_Iterator::operator=(மாறிலி File_முனை_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    முனை_NElems = it.முனை_NElems;
    idx = it.idx;
    திரும்பு *இது;
}

File_முனை File_முனை_Iterator::operator *() மாறிலி
{
    திரும்பு File_முனை(idx < முனை_NElems ? fs : வெற்று, blockIdx, ofs);
}

File_முனை_Iterator& File_முனை_Iterator::operator ++ ()
{
    எனில்( idx == முனை_NElems || !fs )
        திரும்பு *இது;
    idx++;
    File_முனை n(fs, blockIdx, ofs);
    ofs += n.rawSize();
    எனில்( ofs >= blockSize )
    {
        fs->normalize_முனை_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
    திரும்பு *இது;
}

File_முனை_Iterator File_முனை_Iterator::operator ++ (முழு)
{
    File_முனை_Iterator it = *இது;
    ++(*இது);
    திரும்பு it;
}

File_முனை_Iterator& File_முனை_Iterator::operator += (முழு _ofs)
{
    CV_Assert( _ofs >= 0 );
    க்கு( ; _ofs > 0; _ofs-- )
        இது->operator ++();
    திரும்பு *இது;
}

File_முனை_Iterator& File_முனை_Iterator::readRaw( மாறிலி String& fmt, வெறுமை* _data0, size_t maxsz)
{
    எனில்( fs && idx < முனை_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        முழு fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        முழு fmt_pair_மொத்தம் = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        size_t esz = fs::calcStructSize( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        க்கு( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            க்கு( முழு k = 0; k < fmt_pair_மொத்தம்; k++ )
            {
                முழு elem_type = fmt_pairs[k*2+1];
                முழு elem_size = CV_ELEM_SIZE(elem_type);

                முழு மொத்தம்_2 = fmt_pairs[k*2];
                offset = alignSize( offset, elem_size );
                uchar* data = data0 + offset;

                க்கு( முழு i = 0; i < மொத்தம்_2; i++, ++(*இது) )
                {
                    File_முனை முனை = *(*இது);
                    எனில்( முனை.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_size == 8 ? (int64_t)முனை : (முழு)முனை);
                        தேர்வு( elem_type )
                        {
                        நிலை CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(ival);
                            data++;
                            நிறுத்து;
                        நிலை CV_8S:
                            *(எழுத்து*)data = saturate_cast<schar>(ival);
                            data++;
                            நிறுத்து;
                        நிலை CV_Bool:
                            *(தருக்க*)data = ival != 0;
                            data++;
                            நிறுத்து;
                        நிலை CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(ival);
                            data += அளவு(ushort);
                            நிறுத்து;
                        நிலை CV_16S:
                            *(short*)data = saturate_cast<short>(ival);
                            data += அளவு(short);
                            நிறுத்து;
                        நிலை CV_32U:
                            *(unsigned*)data = (unsigned)std::max(ival, (int64_t)0);
                            data += அளவு(unsigned);
                            நிறுத்து;
                        நிலை CV_32S:
                            *(முழு*)data = (முழு)ival;
                            data += அளவு(முழு);
                            நிறுத்து;
                        நிலை CV_32F:
                            *(தசம*)data = (தசம)ival;
                            data += அளவு(தசம);
                            நிறுத்து;
                        நிலை CV_64U:
                            *(uint64_t*)data = (uint64_t)ival;
                            data += அளவு(uint64_t);
                            நிறுத்து;
                        நிலை CV_64S:
                            *(int64_t*)data = (int64_t)ival;
                            data += அளவு(int64_t);
                            நிறுத்து;
                        நிலை CV_64F:
                            *(இரட்டை*)data = (இரட்டை)ival;
                            data += அளவு(இரட்டை);
                            நிறுத்து;
                        நிலை CV_16F:
                            *(hfloat*)data = hfloat((தசம)ival);
                            data += அளவு(hfloat);
                            நிறுத்து;
                        நிலை CV_16BF:
                            *(bfloat*)data = bfloat((தசம)ival);
                            data += அளவு(bfloat);
                            நிறுத்து;
                        நிலை CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((தசம)ival);
                            data += அளவு(fp8_t);
                            நிறுத்து;
                        நிலை CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((தசம)ival);
                            data += அளவு(fp8a_t);
                            நிறுத்து;
                        இயல்பு:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    இல்லையெனில் எனில்( முனை.isReal() )
                    {
                        இரட்டை fval = (இரட்டை)முனை;

                        தேர்வு( elem_type )
                        {
                        நிலை CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(fval);
                            data++;
                            நிறுத்து;
                        நிலை CV_8S:
                            *(எழுத்து*)data = saturate_cast<schar>(fval);
                            data++;
                            நிறுத்து;
                        நிலை CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(fval);
                            data += அளவு(ushort);
                            நிறுத்து;
                        நிலை CV_16S:
                            *(short*)data = saturate_cast<short>(fval);
                            data += அளவு(short);
                            நிறுத்து;
                        நிலை CV_32U:
                            *(முழு*)data = saturate_cast<unsigned>(fval);
                            data += அளவு(முழு);
                            நிறுத்து;
                        நிலை CV_32S:
                            *(முழு*)data = saturate_cast<முழு>(fval);
                            data += அளவு(முழு);
                            நிறுத்து;
                        நிலை CV_32F:
                            *(தசம*)data = (தசம)fval;
                            data += அளவு(தசம);
                            நிறுத்து;
                        நிலை CV_64U:
                            *(uint64_t*)data = (uint64_t)round(std::max(fval, 0.));
                            data += அளவு(uint64_t);
                            நிறுத்து;
                        நிலை CV_64S:
                            *(int64_t*)data = (int64_t)round(std::max(fval, 0.));
                            data += அளவு(int64_t);
                            நிறுத்து;
                        நிலை CV_64F:
                            *(இரட்டை*)data = fval;
                            data += அளவு(இரட்டை);
                            நிறுத்து;
                        நிலை CV_16F:
                            *(hfloat*)data = hfloat((தசம)fval);
                            data += அளவு(hfloat);
                            நிறுத்து;
                        நிலை CV_16BF:
                            *(bfloat*)data = bfloat((தசம)fval);
                            data += அளவு(bfloat);
                            நிறுத்து;
                        நிலை CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((தசம)fval);
                            data += அளவு(fp8_t);
                            நிறுத்து;
                        நிலை CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((தசம)fval);
                            data += அளவு(fp8a_t);
                            நிறுத்து;
                        இயல்பு:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    இல்லையெனில்
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (முழு)(data - data0);
            }
        }
    }

    திரும்பு *இது;
}

தருக்க File_முனை_Iterator::equalTo(மாறிலி File_முனை_Iterator& it) மாறிலி
{
    திரும்பு fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && முனை_NElems == it.முனை_NElems;
}

size_t File_முனை_Iterator::remaining() மாறிலி
{
    திரும்பு முனை_NElems - idx;
}

தருக்க operator == ( மாறிலி File_முனை_Iterator& it1, மாறிலி File_முனை_Iterator& it2 )
{
    திரும்பு it1.equalTo(it2);
}

தருக்க operator != ( மாறிலி File_முனை_Iterator& it1, மாறிலி File_முனை_Iterator& it2 )
{
    திரும்பு !it1.equalTo(it2);
}

வெறுமை பட(மாறிலி File_முனை& முனை, முழு& val, முழு default_val)
{
    val = default_val;
    எனில்( !முனை.empty() )
    {
        val = (முழு)முனை;
    }
}

வெறுமை பட(மாறிலி File_முனை& முனை, int64_t& val, int64_t default_val)
{
    val = default_val;
    எனில்( !முனை.empty() )
    {
        val = (int64_t)முனை;
    }
}

வெறுமை பட(மாறிலி File_முனை& முனை, இரட்டை& val, இரட்டை default_val)
{
    val = default_val;
    எனில்( !முனை.empty() )
    {
        val = (இரட்டை)முனை;
    }
}

வெறுமை பட(மாறிலி File_முனை& முனை, தசம& val, தசம default_val)
{
    val = default_val;
    எனில்( !முனை.empty() )
    {
        val = (தசம)முனை;
    }
}

வெறுமை பட(மாறிலி File_முனை& முனை, std::string& val, மாறிலி std::string& default_val)
{
    val = default_val;
    எனில்( !முனை.empty() )
    {
        val = (std::string)முனை;
    }
}
வெறுமை FileStorage::Impl::லிக(மாறிலி String &சாவி, தருக்க value)
{
    CV_Assert(write_mode);
    எனில் (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().லிக(சாவி.c_str(), (முழு)value);
    }
    இல்லையெனில்
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().லிக(சாவி.c_str(), value ? "true" : "false", பொய்);
    }
}

FileStorage_API::~FileStorage_API() {}

நாமாகாஶ internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, மாறிலி std::string& name,
                                       முழு flags, மாறிலி std::string& typeName)
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
