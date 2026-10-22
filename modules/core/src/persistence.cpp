// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "persistence.hpp"
#شامل "persistence_impl.hpp"
#شامل "persistence_base64_encoding.hpp"
#شامل <unordered_map>
#شامل <iterator>

#شامل <opencv2/core/utils/logger.hpp>

#define CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

فضاینام cv
{

فضاینام fs
{

صحیح strcasecmp(ثابت نویسه* s1, ثابت نویسه* s2)
{
    ثابت نویسه* dummy="";
    اگر(!s1) s1=dummy;
    اگر(!s2) s2=dummy;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t i, len = std::min(len1, len2);
    برای( i = 0; i < len; i++ )
    {
        صحیح d = tolower((صحیح)s1[i]) - tolower((صحیح)s2[i]);
        اگر( d != 0 )
            بازگشت d;
    }
    بازگشت len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

نویسه* itoa( صحیح _val, نویسه* buffer, صحیح /*radix*/ )
{
    ثابت صحیح radix = 10;
    نویسه* ptr=buffer + 23 /* enough even for 64-bit integers */;
    unsigned val = abs(_val);

    *ptr = '\0';
    انجامبده
    {
        unsigned r = val / radix;
        *--ptr = (نویسه)(val - (r*radix) + '0');
        val = r;
    }
    وقتی( val != 0 );

    اگر( _val < 0 )
        *--ptr = '-';

    بازگشت ptr;
}

نویسه* itoa( int64_t _val, نویسه* buffer, صحیح /*radix*/, منطقی _signed)
{
    ثابت صحیح radix = 10;
    نویسه* ptr=buffer + 23 /* enough even for 64-bit integers */;
    صحیح sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : abs(_val);

    *ptr = '\0';
    انجامبده
    {
        uint64_t r = val / radix;
        *--ptr = (نویسه)(val - (r*radix) + '0');
        val = r;
    }
    وقتی( val != 0 );

    اگر( sign < 0 )
        *--ptr = '-';

    بازگشت ptr;
}

نویسه* doubleToString( نویسه* buf, size_t bufSize, دوگانه value, منطقی explicitZero )
{
    Cv64suf val;
    unsigned ieee754_hi;

    val.f = value;
    ieee754_hi = (unsigned)(val.u >> 32);

    اگر( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        صحیح ivalue = cvRound(value);
        اگر( ivalue == value )
        {
            اگر( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            وگرنه
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        وگرنه
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, bufSize, "%.17g", value );

            نویسه* ptr = buf;
            اگر( *ptr == '+' || *ptr == '-' )
                ptr++;
            برای( ; cv_isdigit(*ptr); ptr++ )
                ;
            اگر( *ptr == ',' )
                *ptr = '.';
        }
    }
    وگرنه
    {
        unsigned ieee754_lo = (unsigned)val.u;
        اگر( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        وگرنه
            strcpy( buf, (صحیح)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    بازگشت buf;
}

نویسه* floatToString( نویسه* buf, size_t bufSize, اعشاری value, منطقی halfprecision, منطقی explicitZero )
{
    Cv32suf val;
    unsigned ieee754;
    val.f = value;
    ieee754 = val.u;

    اگر( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        صحیح ivalue = cvRound(value);
        اگر( ivalue == value )
        {
            اگر( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            وگرنه
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        وگرنه
        {
            اگر (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.5g", value);
            }
            وگرنه
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.9g", value);
            }

            نویسه* ptr = buf;
            اگر( *ptr == '+' || *ptr == '-' )
                ptr++;
            برای( ; cv_isdigit(*ptr); ptr++ )
                ;
            اگر( *ptr == ',' )
                *ptr = '.';
        }
    }
    وگرنه
    {
        اگر( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        وگرنه
            strcpy( buf, (صحیح)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    بازگشت buf;
}

ایستا ثابت نویسه symbols[] = "ucwsifdhHbUIneE";

ایستا نویسه type_نماد(صحیح depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    بازگشت symbols[depth];
}

ایستا صحیح نماد_To_Type(نویسه c)
{
    اگر (c == 'r')
        بازگشت CV_SEQ_ELTYPE_PTR;
    ثابت نویسه* pos = strchr( symbols, c );
    اگر( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    بازگشت static_cast<صحیح>(pos - symbols);
}

نویسه* encodeFormat(صحیح elem_type, نویسه* dt, size_t dt_len)
{
    صحیح cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    نویسه نماد = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_نماد(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, نماد);
    بازگشت dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
نویسه* encodeFormat(صحیح elem_type, نویسه* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    بازگشت encodeFormat(elem_type, dt, max);
}

صحیح decodeFormat( ثابت نویسه* dt, صحیح* fmt_pairs, صحیح max_len )
{
    صحیح fmt_pair_شمار = 0;
    صحیح i = 0, k = 0, len = dt ? (صحیح)strlen(dt) : 0;

    اگر( !dt || !len )
        بازگشت 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    برای( ; k < len; k++ )
    {
        نویسه c = dt[k];

        اگر( cv_isdigit(c) )
        {
            صحیح شمار_2 = c - '0';
            اگر( cv_isdigit(dt[k+1]) )
            {
                نویسه* endptr = 0;
                شمار_2 = (صحیح)strtol( dt+k, &endptr, 10 );
                k = (صحیح)(endptr - dt) - 1;
            }

            اگر( شمار_2 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = شمار_2;
        }
        وگرنه
        {
            صحیح depth = نماد_To_Type(c);
            اگر( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = depth;
            اگر( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            وگرنه
            {
                i += 2;
                اگر( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_شمار = i/2;
    بازگشت fmt_pair_شمار;
}

صحیح calcElemSize( ثابت نویسه* dt, صحیح initial_size )
{
    صحیح size = 0;
    صحیح fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_شمار;
    صحیح comp_size;

    fmt_pair_شمار = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_شمار *= 2;
    برای( i = 0, size = initial_size; i < fmt_pair_شمار; i += 2 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[i+1]);
        size = cvAlign( size, comp_size );
        size += comp_size * fmt_pairs[i];
    }
    اگر( initial_size == 0 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[1]);
        size = cvAlign( size, comp_size );
    }
    بازگشت size;
}


صحیح calcStructSize( ثابت نویسه* dt, صحیح initial_size )
{
    صحیح size = calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    برای ( ثابت نویسه * type = dt; *type != '\0'; type++ )
    {
        نویسه v = *type;
        اگر (v >= '0' && v <= '9')
            ادامه;  // skip vector size
        انتخاب (v)
        {
        حالت 'u': { elem_max_size = std::max( elem_max_size, اندازه(uchar ) ); بشکن; }
        حالت 'b': { elem_max_size = std::max( elem_max_size, اندازه(منطقی  ) ); بشکن; }
        حالت 'c': { elem_max_size = std::max( elem_max_size, اندازه(schar ) ); بشکن; }
        حالت 'w': { elem_max_size = std::max( elem_max_size, اندازه(ushort) ); بشکن; }
        حالت 's': { elem_max_size = std::max( elem_max_size, اندازه(short ) ); بشکن; }
        حالت 'i': { elem_max_size = std::max( elem_max_size, اندازه(صحیح   ) ); بشکن; }
        حالت 'n': { elem_max_size = std::max( elem_max_size, اندازه(unsigned) ); بشکن; }
        حالت 'f': { elem_max_size = std::max( elem_max_size, اندازه(اعشاری ) ); بشکن; }
        حالت 'd': { elem_max_size = std::max( elem_max_size, اندازه(دوگانه) ); بشکن; }
        حالت 'h': { elem_max_size = std::max( elem_max_size, اندازه(hfloat)); بشکن; }
        حالت 'H': { elem_max_size = std::max( elem_max_size, اندازه(bfloat)); بشکن; }
        حالت 'e': { elem_max_size = std::max( elem_max_size, اندازه(fp8_t) ); بشکن; }
        حالت 'E': { elem_max_size = std::max( elem_max_size, اندازه(fp8a_t)); بشکن; }
        حالت 'I': { elem_max_size = std::max( elem_max_size, اندازه(int64_t)); بشکن; }
        حالت 'U': { elem_max_size = std::max( elem_max_size, اندازه(uint64_t)); بشکن; }
        پیشفرض:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (نویسه)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<صحیح>(elem_max_size) );
    بازگشت size;
}

صحیح decodeSimpleFormat( ثابت نویسه* dt )
{
    صحیح elem_type = -1;
    صحیح fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_شمار;

    fmt_pair_شمار = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    اگر( fmt_pair_شمار != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    بازگشت elem_type;
}

}

#اگر defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#define CV_LITTLE_ENDIAN_MEM_ACCESS 1
#وگرنه
#define CV_LITTLE_ENDIAN_MEM_ACCESS 0
#endif

ایستا درونخطی صحیح readInt(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#اگر CV_LITTLE_ENDIAN_MEM_ACCESS
    صحیح val;
    memcpy(&val, p, اندازه(val));
    بازگشت val;
#وگرنه
    صحیح val = (صحیح)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    بازگشت val;
#endif
}

ایستا درونخطی int64_t readLong(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#اگر CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    memcpy(&val, p, اندازه(val));
    بازگشت val;
#وگرنه
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    بازگشت val0 | ((int64_t)val1 << 32);
#endif
}

ایستا درونخطی دوگانه readReal(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#اگر CV_LITTLE_ENDIAN_MEM_ACCESS
    دوگانه val;
    memcpy(&val, p, اندازه(val));
    بازگشت val;
#وگرنه
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    بازگشت val.f;
#endif
}

template <typename T>
ایستا درونخطی تهی writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#اگر CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &ival, اندازه(ival));
#وگرنه
    برای (size_t i = 0, j = 0; i < اندازه(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#endif
}

ایستا درونخطی تهی writeReal(uchar* p, دوگانه fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#اگر CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &fval, اندازه(fval));
#وگرنه
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



تهی FileStorage::Impl::init() {
    flags = 0;
    buffer.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = نادرست;
    state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = نادرست;
    delayed_struct_کلید = nullptr;
    delayed_struct_flags = 0;
    delayed_type_name = nullptr;
    base64_writer = nullptr;
    is_opened = نادرست;
    dummy_eof = نادرست;
    write_mode = نادرست;
    mem_mode = نادرست;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    empty_stream = درست;

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

تهی FileStorage::Impl::release(String *out) {
    اگر (is_opened) {
        اگر (out)
            out->clear();
        اگر (write_mode) {
            وقتی (write_پشته.size() > 1) {
                endWriteStruct();
            }
            flush();
            اگر (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            وگرنه اگر (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        اگر (mem_mode && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

تهی FileStorage::Impl::analyze_file_name(ثابت std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    ایستا ثابت نویسه not_file_name = '\n';
    ایستا ثابت نویسه parameter_begin = '?';
    ایستا ثابت نویسه parameter_separator = '&';

    اگر (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        بازگشت;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    اگر (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        برای (size_t param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            اگر ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

منطقی FileStorage::Impl::open(ثابت نویسه *filename_or_buf, صحیح _flags, ثابت نویسه *encoding) {
    منطقی ok = درست;
    release();

    منطقی append = (_flags & 3) == FileStorage::APPEND;
    mem_mode = (_flags & FileStorage::MEMORY) != 0;

    write_mode = (_flags & 3) != 0;
    منطقی write_base64 = (write_mode || append) && (_flags & FileStorage::BASE64) != 0;

    منطقی isGZ = نادرست;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        اگر (!params.empty())
            filename = params[0];

        اگر (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_mode || append);
    }

    اگر (filename.size() == 0 && !mem_mode && !write_mode)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    اگر (mem_mode && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    اگر (!mem_mode) {
        size_t dot_idx = filename.find_last_of('.');
        نویسه compression = '\0';

        اگر (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            اگر (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                اگر (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = درست;
                }

                وگرنه اگر(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = درست;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        اگر (!isGZ) {
            file = fopen(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            اگر (!file)
            {
                CV_LOG_ERROR(پوچ, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                بازگشت نادرست;
            }
        } وگرنه {
#اگر USE_ZLIB
            اگر (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            نویسه mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            اگر (!gzfile)
            {
                CV_LOG_ERROR(پوچ, "Can't open archive: '" << filename << "' mode=" << mode);
                بازگشت نادرست;
            }
#وگرنه
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#endif
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_data.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    اگر (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        اگر (mem_mode)
            outbuf.clear();

        اگر (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            ثابت نویسه *dot_pos = پوچ;
            ثابت نویسه *dot_pos2 = پوچ;
            // like strrchr() implementation, but save two last positions simultaneously
            برای (ثابت نویسه *pos = &filename[0]; pos[0] != 0; pos++) {
                اگر (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            اگر (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != پوچ) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } وگرنه اگر (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        صحیح buf_size = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        اگر (append) {
            fseek(file, 0, SEEK_END);
            اگر (ftell(file) == 0)
                append = نادرست;
        }

        write_پشته.clear();
        empty_stream = درست;
        write_پشته.push_back(FStructData("", File_گره::MAP | File_گره::EMPTY, 0));
        buffer.reserve(buf_size + 1024);
        buffer.resize(buf_size);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;

        اگر (fmt == FileStorage::FORMAT_XML) {
            size_t file_size = file ? (size_t) ftell(file) : (size_t) 0;
            اگر (!append || file_size == 0) {
                اگر (encoding && *encoding != '\0') {
                    اگر (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(strlen(encoding) < 1000);
                    نویسه buf[1100];
                    snprintf(buf, اندازه(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } وگرنه
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } وگرنه {
                صحیح xml_buf_size = 1 << 10;
                نویسه substr[] = "</opencv_storage>";
                صحیح last_occurrence = -1;
                xml_buf_size = MIN(xml_buf_size, صحیح(file_size));
                fseek(file, -xml_buf_size, SEEK_END);
                // find the last occurrence of </opencv_storage>
                برای (;;) {
                    صحیح سطر_offset = (صحیح) ftell(file);
                    ثابت نویسه *ptr0 = این->gets(xml_buf_size);
                    ثابت نویسه *ptr = پوچ;
                    اگر (!ptr0)
                        بشکن;
                    ptr = ptr0;
                    برای (;;) {
                        ptr = strstr(ptr, substr);
                        اگر (!ptr)
                            بشکن;
                        last_occurrence = سطر_offset + (صحیح) (ptr - ptr0);
                        ptr += strlen(substr);
                    }
                }
                اگر (last_occurrence < 0) {
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

            emitter_do_not_use_direct_dereference = createXMLEmitter(این);
        } وگرنه اگر (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                اگر (!append) {
                    اگر (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    وگرنه
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            وگرنه
                puts("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(این);
        } وگرنه {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            اگر (!append)
                puts("{\n");
            وگرنه {
                منطقی valid = نادرست;
                long roffset = 0;
                برای (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    ثابت نویسه end_mark = '}';
                    اگر (fgetc(file) == end_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = درست;
                        بشکن;
                    }
                }

                اگر (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } وگرنه {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_پشته.back().indent = 4;
            emitter_do_not_use_direct_dereference = createJSONEmitter(این);
        }
        is_opened = درست;
    } وگرنه {
        ثابت size_t buf_size0 = 40;
        buffer.resize(buf_size0);
        اگر (mem_mode) {
            strbuf = (نویسه *) filename_or_buf;
            strbufsize = strlen(strbuf);
        }

        ثابت نویسه *yaml_signature = "%YAML";
        ثابت نویسه *json_signature = "{";
        ثابت نویسه *xml_signature = "<?xml";
        نویسه *buf = این->gets(16);
        CV_Assert(buf);
        نویسه *bufPtr = cv_skip_BOM(buf);
        size_t bufOffset = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        اگر (strncmp(bufPtr, yaml_signature, strlen(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        وگرنه اگر (strncmp(bufPtr, json_signature, strlen(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        وگرنه اگر (strncmp(bufPtr, xml_signature, strlen(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        وگرنه
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = bufOffset;
        bufofs = 0;

        تلاش {
            نویسه *ptr = bufferStart();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_گره ریشه_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_گره_Space(ریشه_nodes, 9);
            *rptr = File_گره::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            انتخاب (fmt) {
                حالت FileStorage::FORMAT_XML:
                    parser_do_not_use_direct_dereference = createXMLParser(این);
                    بشکن;
                حالت FileStorage::FORMAT_YAML:
                حالت FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_direct_dereference = createYAMLParser(این);
                    بشکن;
                حالت FileStorage::FORMAT_JSON:
                    parser_do_not_use_direct_dereference = createJSONParser(این);
                    بشکن;
                پیشفرض:
                    parser_do_not_use_direct_dereference = Ptr<FileStorageParser>();
            }

            اگر (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                اگر (ok) {
                    finalizeCollection(ریشه_nodes);

                    CV_Assert(!fs_data_ptrs.empty());
                    File_گره roots_گره(fs_ext, 0, 0);
                    size_t i, nroots = roots_گره.size();
                    File_گره_Iterator it = roots_گره.begin();

                    برای (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        بگیر (...)
        {
            // FIXIT log error message
            is_opened = درست;
            release();
            پرتابکن;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = درست;
        std::vector<نویسه> tmpbuf;
        std::swap(buffer, tmpbuf);
        bufofs = 0;
    }
    بازگشت ok;
}

تهی FileStorage::Impl::puts(ثابت نویسه *str) {
    CV_Assert(write_mode);
    اگر (mem_mode)
        std::copy(str, str + strlen(str), std::back_inserter(outbuf));
    وگرنه اگر (file)
        fputs(str, file);
#اگر USE_ZLIB
    وگرنه اگر (gzfile)
        gzputs(gzfile, str);
#endif
    وگرنه
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

نویسه *FileStorage::Impl::getsFromFile(نویسه *buf, صحیح شمار_2) {
    اگر (file)
        بازگشت fgets(buf, شمار_2, file);
#اگر USE_ZLIB
    اگر (gzfile)
        بازگشت gzgets(gzfile, buf, شمار_2);
#endif
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

نویسه *FileStorage::Impl::gets(size_t max_شمار) {
    اگر (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        ثابت نویسه *instr = strbuf;
        برای (; i < len; i++) {
            نویسه c = instr[i];
            اگر (c == '\0' || c == '\n') {
                اگر (c == '\n')
                    i++;
                بشکن;
            }
        }
        size_t شمار_2 = i - strbufpos;
        اگر (max_شمار == 0 || max_شمار > شمار_2)
            max_شمار = شمار_2;
        buffer.resize(std::max(buffer.size(), max_شمار + 8));
        memcpy(&buffer[0], instr + strbufpos, max_شمار);
        buffer[max_شمار] = '\0';
        strbufpos = i;
        بازگشت max_شمار > 0 ? &buffer[0] : 0;
    }

    ثابت size_t MAX_BLOCK_SIZE = INT_MAX / 2; // hopefully, that will be enough
    اگر (max_شمار == 0)
        max_شمار = MAX_BLOCK_SIZE;
    وگرنه
        CV_Assert(max_شمار < MAX_BLOCK_SIZE);
    size_t ofs = 0;

    برای (;;) {
        صحیح شمار_2 = (صحیح) std::min(buffer.size() - ofs - 16, max_شمار);
        نویسه *ptr = getsFromFile(&buffer[ofs], شمار_2 + 1);
        اگر (!ptr)
            بشکن;
        صحیح delta = (صحیح) strlen(ptr);
        ofs += delta;
        max_شمار -= delta;
        اگر (delta == 0 || ptr[delta - 1] == '\n' || max_شمار == 0)
            بشکن;
        اگر (delta == شمار_2)
            buffer.resize((size_t) (buffer.size() * 1.5));
    }
    بازگشت ofs > 0 ? &buffer[0] : 0;
}

نویسه *FileStorage::Impl::gets() {
    نویسه *ptr = این->gets(0);
    اگر (!ptr) {
        ptr = bufferStart();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        بازگشت 0;
    } وگرنه {
        size_t l = strlen(ptr);
        اگر (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    بازگشت ptr;
}

منطقی FileStorage::Impl::eof() {
    اگر (dummy_eof)
        بازگشت درست;
    اگر (strbuf)
        بازگشت strbufpos >= strbufsize;
    اگر (file)
        بازگشت feof(file) != 0;
#اگر USE_ZLIB
    اگر (gzfile)
        بازگشت gzeof(gzfile) != 0;
#endif
    بازگشت نادرست;
}

تهی FileStorage::Impl::setEof() {
    dummy_eof = درست;
}

تهی FileStorage::Impl::closeFile() {
    اگر (file)
        fclose(file);
#اگر USE_ZLIB
    وگرنه اگر (gzfile)
        gzclose(gzfile);
#endif
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = نادرست;
}

تهی FileStorage::Impl::rewind() {
    اگر (file)
        ::rewind(file);
#اگر USE_ZLIB
    وگرنه اگر (gzfile)
        gzrewind(gzfile);
#endif
    strbufpos = 0;
}

نویسه *FileStorage::Impl::resizeWriteBuffer(نویسه *ptr, صحیح len) {
    ثابت نویسه *buffer_end = &buffer[0] + buffer.size();
    اگر (ptr + len < buffer_end)
        بازگشت ptr;

    ثابت نویسه *buffer_start = &buffer[0];
    صحیح written_len = (صحیح) (ptr - buffer_start);

    CV_Assert(written_len <= (صحیح) buffer.size());
    صحیح new_size = (صحیح) ((buffer_end - buffer_start) * 3 / 2);
    new_size = MAX(written_len + len, new_size);
    buffer.reserve(new_size + 256);
    buffer.resize(new_size);
    bufofs = written_len;
    بازگشت &buffer[0] + bufofs;
}

نویسه *FileStorage::Impl::flush() {
    نویسه *buffer_start = &buffer[0];
    نویسه *ptr = buffer_start + bufofs;

    اگر (ptr > buffer_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(buffer_start);
        bufofs = 0;
    }

    صحیح indent = write_پشته.back().indent;

    اگر (space != indent) {
        memset(buffer_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = buffer_start + bufofs;

    بازگشت ptr;
}

تهی FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(نادرست);
    اگر (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_پشته.empty());

    FStructData &current_struct = write_پشته.back();
    اگر (fmt == FileStorage::FORMAT_JSON && !File_گره::isFlow(current_struct.flags) && write_پشته.size() > 1)
        current_struct.indent = write_پشته[write_پشته.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_پشته.pop_back();
    اگر (!write_پشته.empty())
        write_پشته.back().flags &= ~File_گره::EMPTY;
}

تهی FileStorage::Impl::startWriteStruct_helper(ثابت نویسه *کلید, صحیح struct_flags,
                                                ثابت نویسه *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (File_گره::TYPE_MASK | File_گره::FLOW)) | File_گره::EMPTY;
    اگر (!File_گره::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    اگر (type_name && type_name[0] == '\0')
        type_name = 0;

    FStructData s = getEmitter().startWriteStruct(write_پشته.back(), کلید, struct_flags, type_name);

    write_پشته.push_back(s);
    size_t write_پشته_size = write_پشته.size();
    اگر (write_پشته_size > 1)
        write_پشته[write_پشته_size - 2].flags &= ~File_گره::EMPTY;

    اگر (fmt != FileStorage::FORMAT_JSON && !File_گره::isFlow(s.flags))
        flush();

    اگر (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_گره::isMap(struct_flags)) {
        getEmitter().اكتب("type_id", type_name, نادرست);
    }
}

تهی FileStorage::Impl::startWriteStruct(ثابت نویسه *کلید, صحیح struct_flags,
                                         ثابت نویسه *type_name) {
    check_if_write_struct_is_delayed(نادرست);
    اگر (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    اگر (state_of_writing_base64 == FileStorage_API::Uncertain && File_گره::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(کلید, struct_flags, type_name);
    } وگرنه اگر (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        اگر ((File_گره::TYPE_MASK & struct_flags) != File_گره::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        وگرنه اگر (state_of_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(کلید, struct_flags, "binary");

        اگر (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::InUse);
    } وگرنه {
        /* Won't output Base64 data */
        اگر (state_of_writing_base64 == FileStorage_API::InUse)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(کلید, struct_flags, type_name);

        اگر (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::NotUse);
    }
}

تهی FileStorage::Impl::writeComment(ثابت نویسه *comment, منطقی eol_comment) {
    CV_Assert(write_mode);
    getEmitter().writeComment(comment, eol_comment);
}

تهی FileStorage::Impl::startNextStream() {
    CV_Assert(write_mode);
    اگر (!empty_stream) {
        وقتی (!write_پشته.empty())
            endWriteStruct();
        flush();
        getEmitter().startNextStream();
        empty_stream = درست;
        write_پشته.push_back(FStructData("", File_گره::EMPTY, 0));
        bufofs = 0;
    }
}

تهی FileStorage::Impl::اكتب(ثابت String &کلید, صحیح value) {
    CV_Assert(write_mode);
    getEmitter().اكتب(کلید.c_str(), value);
}

تهی FileStorage::Impl::اكتب(ثابت String &کلید, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().اكتب(کلید.c_str(), value);
}

تهی FileStorage::Impl::اكتب(ثابت String &کلید, دوگانه value) {
    CV_Assert(write_mode);
    getEmitter().اكتب(کلید.c_str(), value);
}

تهی FileStorage::Impl::اكتب(ثابت String &کلید, ثابت String &value) {
    CV_Assert(write_mode);
    getEmitter().اكتب(کلید.c_str(), value.c_str(), نادرست);
}

تهی FileStorage::Impl::writeRawData(ثابت std::string &dt, ثابت تهی *_data, size_t len) {
    CV_Assert(write_mode);

    اگر (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        writeRawDataBase64(_data, len, dt.c_str());
        بازگشت;
    } وگرنه اگر (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elemSize = fs::calcStructSize(dt.c_str(), 0);
    CV_Assert(elemSize);
    CV_Assert(len % elemSize == 0);
    len /= elemSize;

    منطقی explicitZero = fmt == FileStorage::FORMAT_JSON;
    ثابت uchar *data0 = (ثابت uchar *) _data;
    صحیح fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_شمار;
    نویسه buf[256] = "";

    fmt_pair_شمار = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    اگر (!len)
        بازگشت;

    اگر (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    اگر (fmt_pair_شمار == 1) {
        fmt_pairs[0] *= (صحیح) len;
        len = 1;
    }

    برای (; len--; data0 += elemSize) {
        صحیح offset = 0;
        برای (k = 0; k < fmt_pair_شمار; k++) {
            صحیح i, شمار_2 = fmt_pairs[k * 2];
            صحیح elem_type = fmt_pairs[k * 2 + 1];
            صحیح elem_size = CV_ELEM_SIZE(elem_type);
            ثابت نویسه *ptr;

            offset = cvAlign(offset, elem_size);
            ثابت uchar *data = data0 + offset;

            برای (i = 0; i < شمار_2; i++) {
                انتخاب (elem_type) {
                    حالت CV_8U:
                        ptr = fs::itoa(*(uchar *) data, buf, 10);
                        data++;
                        بشکن;
                    حالت CV_Bool:
                        ptr = fs::itoa(*(uchar *) data != 0, buf, 10);
                        data++;
                        بشکن;
                    حالت CV_8S:
                        ptr = fs::itoa(*(نویسه *) data, buf, 10);
                        data++;
                        بشکن;
                    حالت CV_16U:
                        ptr = fs::itoa(*(ushort *) data, buf, 10);
                        data += اندازه(ushort);
                        بشکن;
                    حالت CV_16S:
                        ptr = fs::itoa(*(short *) data, buf, 10);
                        data += اندازه(short);
                        بشکن;
                    حالت CV_32U:
                        ptr = fs::itoa((int64_t)*(unsigned*) data, buf, 10, نادرست);
                        data += اندازه(unsigned);
                        بشکن;
                    حالت CV_32S:
                        ptr = fs::itoa(*(صحیح *) data, buf, 10);
                        data += اندازه(صحیح);
                        بشکن;
                    حالت CV_64U:
                        ptr = fs::itoa(*(uint64_t*) data, buf, 10, نادرست);
                        data += اندازه(uint64_t);
                        بشکن;
                    حالت CV_64S:
                        ptr = fs::itoa(*(int64_t*) data, buf, 10, درست);
                        data += اندازه(int64_t);
                        بشکن;
                    حالت CV_32F:
                        ptr = fs::floatToString(buf, اندازه(buf), *(اعشاری *) data, نادرست, explicitZero);
                        data += اندازه(اعشاری);
                        بشکن;
                    حالت CV_64F:
                        ptr = fs::doubleToString(buf, اندازه(buf), *(دوگانه *) data, explicitZero);
                        data += اندازه(دوگانه);
                        بشکن;
                    حالت CV_16F:
                        ptr = fs::floatToString(buf, اندازه(buf), (اعشاری) *(hfloat *) data, درست, explicitZero);
                        data += اندازه(hfloat);
                        بشکن;
                    حالت CV_16BF:
                        ptr = fs::floatToString(buf, اندازه(buf), (اعشاری) *(bfloat *) data, درست, explicitZero);
                        data += اندازه(bfloat);
                        بشکن;
                    حالت CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, اندازه(buf), (اعشاری) *(fp8_t *) data, درست, explicitZero);
                        data += اندازه(fp8_t);
                        بشکن;
                    حالت CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, اندازه(buf), (اعشاری) *(fp8a_t *) data, درست, explicitZero);
                        data += اندازه(fp8a_t);
                        بشکن;
                    پیشفرض:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        بازگشت;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (صحیح) (data - data0);
        }
    }
}

تهی FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(نادرست);

    اگر (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_to_Base64_state(FileStorage_API::Base64State::Uncertain);
}

تهی FileStorage::Impl::switch_to_Base64_state(FileStorage_API::Base64State new_state) {
    ثابت نویسه *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    ثابت نویسه *err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    انتخاب (state_of_writing_base64) {
        حالت FileStorage_API::Base64State::Uncertain:
            انتخاب (new_state) {
                حالت FileStorage_API::Base64State::InUse:
                {
                    CV_DbgAssert(base64_writer == 0);
                    منطقی can_indent = (fmt != cv::FileStorage::Mode::FORMAT_JSON);
                    base64_writer = جدید base64::Base64Writer(*این, can_indent);
                    اگر (!can_indent) {
                        نویسه *ptr = bufferPtr();
                        *ptr++ = '\0';
                        puts(bufferStart());
                        setBufferPtr(bufferStart());
                        memset(bufferStart(), 0, static_cast<صحیح>(space));
                        puts("\"$base64$");
                    }
                    بشکن;
                }
                حالت FileStorage_API::Base64State::Uncertain:
                    بشکن;
                حالت FileStorage_API::Base64State::NotUse:
                    بشکن;
                پیشفرض:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    بشکن;
            }
            بشکن;
        حالت FileStorage_API::Base64State::InUse:
            انتخاب (new_state) {
                حالت FileStorage_API::Base64State::InUse:
                حالت FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    بشکن;
                حالت FileStorage_API::Base64State::Uncertain:
                    delete base64_writer;
                    base64_writer = 0;
                    اگر ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        setBufferPtr(bufferStart());
                        flush();
                        memset(bufferStart(), 0, static_cast<صحیح>(space) );
                        setBufferPtr(bufferStart());
                    }
                    بشکن;
                پیشفرض:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    بشکن;
            }
            بشکن;
        حالت FileStorage_API::Base64State::NotUse:
            انتخاب (new_state) {
                حالت FileStorage_API::Base64State::InUse:
                حالت FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    بشکن;
                حالت FileStorage_API::Base64State::Uncertain:
                    بشکن;
                پیشفرض:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    بشکن;
            }
            بشکن;
        پیشفرض:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            بشکن;
    }

    state_of_writing_base64 = new_state;
}

تهی FileStorage::Impl::make_write_struct_delayed(ثابت نویسه *کلید, صحیح struct_flags, ثابت نویسه *type_name) {
    CV_Assert(is_write_struct_delayed == نادرست);
    CV_DbgAssert(delayed_struct_کلید == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    اگر (کلید != nullptr) {
        delayed_struct_کلید = جدید نویسه[strlen(کلید) + 1U];
        strcpy(delayed_struct_کلید, کلید);
    }

    اگر (type_name != nullptr) {
        delayed_type_name = جدید نویسه[strlen(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = درست;
}

تهی FileStorage::Impl::check_if_write_struct_is_delayed(منطقی change_type_to_base64) {
    اگر (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_کلید;
        std::string type_name;
        صحیح struct_flags = delayed_struct_flags;

        اگر (delayed_struct_کلید != nullptr && *delayed_struct_کلید != '\0') {
            struct_کلید.assign(delayed_struct_کلید);
        }
        اگر (delayed_type_name != nullptr && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        delete[] delayed_struct_کلید;
        delete[] delayed_type_name;
        delayed_struct_کلید = nullptr;
        delayed_struct_flags = 0;
        delayed_type_name = nullptr;

        is_write_struct_delayed = نادرست;

        /* call */
        اگر (change_type_to_base64) {
            startWriteStruct_helper(struct_کلید.c_str(), struct_flags, "binary");
            اگر (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::InUse);
        } وگرنه {
            startWriteStruct_helper(struct_کلید.c_str(), struct_flags, type_name.c_str());
            اگر (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::NotUse);
        }
    }
}

تهی FileStorage::Impl::writeRawDataBase64(ثابت تهی *_data, size_t len, ثابت نویسه *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(درست);

    اگر (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } وگرنه اگر (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->اكتب(_data, len, dt);
}

File_گره FileStorage::Impl::get_First_بالاترین_Level_گره() ثابت {
    بازگشت roots.empty() ? File_گره() : roots[0];
}

File_گره FileStorage::Impl::ریشه(صحیح streamIdx) ثابت {
    بازگشت streamIdx >= 0 && streamIdx < (صحیح) roots.size() ? roots[streamIdx] : File_گره();
}

File_گره FileStorage::Impl::operator[](ثابت String &nodename) ثابت {
    بازگشت این->operator[](nodename.c_str());
}

File_گره FileStorage::Impl::operator[](ثابت نویسه * /*nodename*/) ثابت {
    بازگشت File_گره();
}

صحیح FileStorage::Impl::getFormat() ثابت { بازگشت fmt; }

نویسه *FileStorage::Impl::bufferPtr() ثابت { بازگشت (نویسه *) (&buffer[0] + bufofs); }

نویسه *FileStorage::Impl::bufferStart() ثابت { بازگشت (نویسه *) &buffer[0]; }

نویسه *FileStorage::Impl::bufferEnd() ثابت { بازگشت (نویسه *) (&buffer[0] + buffer.size()); }

تهی FileStorage::Impl::setBufferPtr(نویسه *ptr) {
    نویسه *bufferstart = bufferStart();
    CV_Assert(ptr >= bufferstart && ptr <= bufferEnd());
    bufofs = ptr - bufferstart;
}

صحیح FileStorage::Impl::wrapMargin() ثابت { بازگشت wrap_margin; }

FStructData &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_پشته.empty());
    بازگشت write_پشته.back();
}

تهی FileStorage::Impl::setNonEmpty() {
    empty_stream = نادرست;
}

تهی FileStorage::Impl::processSpecialDouble(نویسه *buf, دوگانه *value, نویسه **endptr) {
    FileStorage_API *fs = این;
    نویسه c = buf[0];
    صحیح inf_hi = 0x7ff00000;

    اگر (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    اگر (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    اگر (toupper(buf[1]) == 'I' && toupper(buf[2]) == 'N' && toupper(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    وگرنه اگر (toupper(buf[1]) == 'N' && toupper(buf[2]) == 'A' && toupper(buf[3]) == 'N')
        v.u = (uint64) -1;
    وگرنه
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

دوگانه FileStorage::Impl::strtod(نویسه *ptr, نویسه **endptr) {
    دوگانه fval = ::strtod(ptr, endptr);
    اگر (**endptr == '.') {
        نویسه *dot_pos = *endptr;
        *dot_pos = ',';
        دوگانه fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        اگر (*endptr > dot_pos)
            fval = fval2;
        وگرنه
            *endptr = dot_pos;
    }

    اگر (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    بازگشت fval;
}

تهی FileStorage::Impl::convertToCollection(صحیح type, File_گره &گره) {
    CV_Assert(type == File_گره::SEQ || type == File_گره::MAP);

    صحیح گره_type = گره.type();
    اگر (گره_type == type)
        بازگشت;

    منطقی named = گره.isNamed();
    uchar *ptr = گره.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    دوگانه fval = 0;
    std::string sval;
    منطقی add_first_scalar = نادرست;

    اگر (گره_type != File_گره::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_گره::SEQ);
        اگر (گره_type == File_گره::INT) {
            ival = readLong(ptr);
            add_first_scalar = درست;
        } وگرنه اگر (گره_type == File_گره::REAL) {
            fval = readReal(ptr);
            add_first_scalar = درست;
        } وگرنه اگر (گره_type == File_گره::STRING) {
            sval = std::string(گره);
            add_first_scalar = درست;
        } وگرنه
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", گره_type));
    }

    ptr = reserve_گره_Space(گره, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_گره::NAMED : 0));
    // name has been copied automatically
    اگر (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    اگر (add_first_scalar)
        add_گره(گره, std::string(), گره_type,
                گره_type == File_گره::INT ? (ثابت تهی *) &ival :
                گره_type == File_گره::REAL ? (ثابت تهی *) &fval :
                گره_type == File_گره::STRING ? (ثابت تهی *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_گره_Space(File_گره &گره, size_t sz) {
    منطقی shrinkBlock = نادرست;
    size_t shrinkBlockIdx = 0, shrinkSize = 0;

    uchar *ptr = 0, *blockEnd = 0;

    اگر (!fs_data_ptrs.empty()) {
        size_t blockIdx = گره.blockIdx;
        size_t ofs = گره.ofs;
        CV_Assert(blockIdx == fs_data_ptrs.size() - 1);
        CV_Assert(ofs <= fs_data_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_data_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_data_ptrs[blockIdx] + ofs;
        blockEnd = fs_data_ptrs[blockIdx] + fs_data_blksz[blockIdx];

        CV_Assert(ptr >= fs_data_ptrs[blockIdx] && ptr <= blockEnd);
        اگر (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            بازگشت ptr;
        }

        اگر (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_data[blockIdx]->resize(sz);
            ptr = &fs_data[blockIdx]->at(0);
            fs_data_ptrs[blockIdx] = ptr;
            fs_data_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            بازگشت ptr;
        }

        shrinkBlock = درست;
        shrinkBlockIdx = blockIdx;
        shrinkSize = ofs;
    }

    size_t blockSize = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(blockSize);
    fs_data.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_data_ptrs.push_back(new_ptr);
    fs_data_blksz.push_back(blockSize);
    گره.blockIdx = fs_data_ptrs.size() - 1;
    گره.ofs = 0;
    freeSpaceOfs = sz;

    اگر (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        اگر (ptr[0] & File_گره::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    اگر (shrinkBlock) {
        fs_data[shrinkBlockIdx]->resize(shrinkSize);
        fs_data_blksz[shrinkBlockIdx] = shrinkSize;
    }

    بازگشت new_ptr;
}

unsigned FileStorage::Impl::getStringOfs(ثابت std::string &کلید) ثابت {
    str_hash_t::const_iterator it = str_hash.find(کلید);
    بازگشت it != str_hash.end() ? it->second : 0;
}

File_گره FileStorage::Impl::add_گره(File_گره &collection, ثابت std::string &کلید,
                                    صحیح elem_type, ثابت تهی *value, صحیح len) {
    FileStorage_API *fs = این;
    منطقی noname = کلید.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(کلید.c_str(), "_") == 0);
    convertToCollection(noname ? File_گره::SEQ : File_گره::MAP, collection);

    منطقی isseq = collection.empty() ? نادرست : collection.isSeq();
    اگر (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    unsigned strofs = 0;
    اگر (!noname) {
        strofs = getStringOfs(کلید);
        اگر (!strofs) {
            strofs = (unsigned) str_hash_data.size();
            size_t keysize = کلید.size() + 1;
            str_hash_data.resize(strofs + keysize);
            memcpy(&str_hash_data[0] + strofs, &کلید[0], keysize);
            str_hash.insert(std::make_pair(کلید, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t blockIdx = fs_data_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_گره گره(fs_ext, blockIdx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_گره_Space(گره, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_گره::NAMED));
    اگر (elem_type == File_گره::NONE)
        freeSpaceOfs -= 8;

    اگر (!noname) {
        writeInt(ptr, (صحیح) strofs);
        ptr += 4;
    }

    اگر (elem_type == File_گره::SEQ || elem_type == File_گره::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    اگر (value)
        گره.set_مقدار(elem_type, value, len);

    اگر (collection.isNamed())
        cp += 4;
    صحیح nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    بازگشت گره;
}

تهی FileStorage::Impl::finalizeCollection(File_گره &collection) {
    اگر (!collection.isSeq() && !collection.isMap())
        بازگشت;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    اگر (*ptr0 & File_گره::NAMED)
        ptr += 4;
    size_t blockIdx = collection.blockIdx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t rawSize = 4;
    unsigned sz = (unsigned) readInt(ptr + 4);
    اگر (sz > 0) {
        size_t lastBlockIdx = fs_data_ptrs.size() - 1;

        برای (; blockIdx < lastBlockIdx; blockIdx++) {
            rawSize += fs_data_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    rawSize += freeSpaceOfs - ofs;
    writeInt(ptr, (صحیح) rawSize);
}

تهی FileStorage::Impl::normalize_گره_Ofs(size_t &blockIdx, size_t &ofs) ثابت {
    وقتی (ofs >= fs_data_blksz[blockIdx]) {
        اگر (blockIdx == fs_data_blksz.size() - 1) {
            CV_Assert(ofs == fs_data_blksz[blockIdx]);
            بشکن;
        }
        ofs -= fs_data_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    بازگشت state_of_writing_base64;
}

صحیح FileStorage::Impl::get_space() {
    بازگشت space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = درست;
}

تهی FileStorage::Impl::Base64Decoder::init(ثابت Ptr<FileStorageParser> &_parser, نویسه *_ptr, صحیح _indent) {
    parser_do_not_use_direct_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = نادرست;
}

منطقی FileStorage::Impl::Base64Decoder::readMore(صحیح needed) {
    ایستا ثابت uchar base64tab[] =
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

    اگر (eos)
        بازگشت نادرست;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    برای (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    نویسه *beg = 0, *end = 0;
    منطقی ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    اگر (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = درست;
        size_t tc = totalchars;
        برای (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    صحیح i = 0, j, n = (صحیح) encoded.size();
    اگر (n > 0) {
        ثابت uchar *tab = base64tab;
        نویسه *src = &encoded[0];

        برای (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(صحیح) (uchar) src[i]], c = tab[(صحیح) (uchar) src[i + 1]];
            uchar b = tab[(صحیح) (uchar) src[i + 2]], a = tab[(صحیح) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    اگر (i > 0 && encoded[i - 1] == '=') {
        اگر (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        اگر (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    برای (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    بازگشت (صحیح) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    اگر (ofs >= sz && !readMore(1))
        بازگشت (uchar) 0;
    بازگشت decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    اگر (ofs + 2 > sz && !readMore(2))
        بازگشت (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    بازگشت val;
}

صحیح FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    اگر (ofs + 4 > sz && !readMore(4))
        بازگشت 0;
    صحیح ival = readInt(&decoded[ofs]);
    ofs += 4;
    بازگشت ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    اگر (ofs + 8 > sz && !readMore(8))
        بازگشت 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    بازگشت ival;
}

دوگانه FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    اگر (ofs + 8 > sz && !readMore(8))
        بازگشت 0;
    دوگانه fval = readReal(&decoded[ofs]);
    ofs += 8;
    بازگشت fval;
}

منطقی FileStorage::Impl::Base64Decoder::endOfStream() ثابت { بازگشت eos; }

نویسه *FileStorage::Impl::Base64Decoder::getPtr() ثابت { بازگشت ptr; }


نویسه *FileStorage::Impl::parseBase64(نویسه *ptr, صحیح indent, File_گره &collection) {
    ثابت صحیح BASE64_HDR_SIZE = 24;
    نویسه dt[BASE64_HDR_SIZE + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    صحیح i, k;

    برای (i = 0; i < BASE64_HDR_SIZE; i++)
        dt[i] = (نویسه) base64decoder.getUInt8();
    برای (i = 0; i < BASE64_HDR_SIZE; i++)
        اگر (isspace(dt[i]))
            بشکن;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    صحیح fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    صحیح fmt_pair_شمار = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    دوگانه fval = 0;

    برای (;;) {
        برای (k = 0; k < fmt_pair_شمار; k++) {
            صحیح elem_type = fmt_pairs[k * 2 + 1];
            صحیح شمار_2 = fmt_pairs[k * 2];

            برای (i = 0; i < شمار_2; i++) {
                صحیح گره_type = File_گره::INT;
                انتخاب (elem_type) {
                    حالت CV_8U:
                        ival = base64decoder.getUInt8();
                        بشکن;
                    حالت CV_8S:
                        ival = (نویسه) base64decoder.getUInt8();
                        بشکن;
                    حالت CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        بشکن;
                    حالت CV_16U:
                        ival = base64decoder.getUInt16();
                        بشکن;
                    حالت CV_16S:
                        ival = (short) base64decoder.getUInt16();
                        بشکن;
                    حالت CV_32U:
                        ival = base64decoder.getInt32();
                        بشکن;
                    حالت CV_32S:
                        ival = base64decoder.getInt32();
                        بشکن;
                    حالت CV_64U:
                        ival = base64decoder.getInt64();
                        بشکن;
                    حالت CV_64S:
                        ival = base64decoder.getInt64();
                        بشکن;
                    حالت CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        گره_type = File_گره::REAL;
                    }
                        بشکن;
                    حالت CV_64F:
                        fval = base64decoder.getFloat64();
                        گره_type = File_گره::REAL;
                        بشکن;
                    حالت CV_16F:
                        fval = اعشاری(hfloatFromBits(base64decoder.getUInt16()));
                        گره_type = File_گره::REAL;
                        بشکن;
                    حالت CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        گره_type = File_گره::REAL;
                        بشکن;
                    حالت CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        گره_type = File_گره::REAL;
                        بشکن;
                    پیشفرض:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                اگر (base64decoder.endOfStream())
                    بشکن;
                add_گره(collection, std::string(), گره_type,
                        گره_type == File_گره::INT ? (تهی *) &ival : (تهی *) &fval, -1);
            }
        }
        اگر (base64decoder.endOfStream())
            بشکن;
    }

    finalizeCollection(collection);
    بازگشت base64decoder.getPtr();
}

تهی FileStorage::Impl::parseError(ثابت نویسه *func_name, ثابت std::string &err_پیام, ثابت نویسه *source_file,
                                   صحیح source_سطر) {
    std::string پیام = format("%s(%d): %s", filename.c_str(), lineno, err_پیام.c_str());
    error(Error::StsParseError, func_name, پیام.c_str(), source_file, source_سطر);
}

ثابت uchar *FileStorage::Impl::get_گره_Ptr(size_t blockIdx, size_t ofs) ثابت {
    CV_Assert(blockIdx < fs_data_ptrs.size());
    CV_Assert(ofs < fs_data_blksz[blockIdx]);

    بازگشت fs_data_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) ثابت {
    CV_Assert(nameofs < str_hash_data.size());
    بازگشت std::string(&str_hash_data[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { بازگشت fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(این);
}

FileStorage::FileStorage(ثابت String& filename, صحیح flags, ثابت String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(این);
    منطقی ok = p->open(filename.c_str(), flags, encoding.c_str());
    اگر(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

تهی FileStorage::startWriteStruct(ثابت String& name, صحیح struct_flags, ثابت String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    اگر ((struct_flags & File_گره::TYPE_MASK) == File_گره::SEQ)
        state = FileStorage::مقدار_EXPECTED;
    وگرنه
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

تهی FileStorage::endWriteStruct()
{
    p->endWriteStruct();
    state = p->write_پشته.empty() || File_گره::isMap(p->write_پشته.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::مقدار_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

منطقی FileStorage::open(ثابت String& filename, صحیح flags, ثابت String& encoding)
{
    تلاش
    {
        منطقی ok = p->open(filename.c_str(), flags, encoding.c_str());
        اگر(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        بازگشت ok;
    }
    بگیر (...)
    {
        release();
        پرتابکن;  // re-throw
    }
}

منطقی FileStorage::isOpened() ثابت { بازگشت p->is_opened; }

تهی FileStorage::release()
{
    p->release();
}

File_گره FileStorage::ریشه(صحیح i) ثابت
{
    اگر( p.empty() || p->roots.empty() || i < 0 || i >= (صحیح)p->roots.size() )
        بازگشت File_گره();

    بازگشت p->roots[i];
}

File_گره FileStorage::get_First_بالاترین_Level_گره() ثابت
{
    File_گره r = ریشه();
    File_گره_Iterator it = r.begin();
    بازگشت it != r.end() ? *it : File_گره();
}

std::string FileStorage::getDefaultObjectName(ثابت std::string& _filename)
{
    ایستا ثابت نویسه* stubname = "unnamed";
    ثابت نویسه* filename = _filename.c_str();
    ثابت نویسه* ptr2 = filename + _filename.size();
    ثابت نویسه* ptr = ptr2 - 1;
    cv::AutoBuffer<نویسه> name_buf(_filename.size()+1);

    وقتی( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        اگر( *ptr == '.' && (!*ptr2 || strncmp(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    اگر( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    نویسه* name = name_buf.data();

    // name must start with letter or '_'
    اگر( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    وقتی( ptr < ptr2 )
    {
        نویسه c = *ptr++;
        اگر( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.data();
    اگر( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    بازگشت name;
}


صحیح FileStorage::getFormat() ثابت
{
    بازگشت p->fmt;
}

File_گره FileStorage::operator [](ثابت نویسه* کلید) ثابت
{
    بازگشت این->operator[](std::string(کلید));
}

File_گره FileStorage::operator [](ثابت std::string& کلید) ثابت
{
    File_گره res;
    برای (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][کلید];
        اگر (!res.empty())
            بشکن;
    }
    بازگشت res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    بازگشت buf;
}

تهی FileStorage::writeRaw( ثابت String& fmt, ثابت تهی* vec, size_t len )
{
    p->writeRawData(fmt, (ثابت uchar*)vec, len);
}

تهی FileStorage::writeComment( ثابت String& comment, منطقی eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

تهی writeScalar( FileStorage& fs, صحیح value )
{
    fs.p->اكتب(String(), value);
}

تهی writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->اكتب(String(), value);
}

تهی writeScalar( FileStorage& fs, اعشاری value )
{
    fs.p->اكتب(String(), (دوگانه)value);
}

تهی writeScalar( FileStorage& fs, دوگانه value )
{
    fs.p->اكتب(String(), value);
}

تهی writeScalar( FileStorage& fs, ثابت String& value )
{
    fs.p->اكتب(String(), value);
}

تهی اكتب( FileStorage& fs, ثابت String& name, صحیح value )
{
    fs.p->اكتب(name, value);
}

تهی اكتب( FileStorage& fs, ثابت String& name, int64_t value )
{
    fs.p->اكتب(name, value);
}

تهی اكتب( FileStorage& fs, ثابت String& name, اعشاری value )
{
    fs.p->اكتب(name, (دوگانه)value);
}

تهی اكتب( FileStorage& fs, ثابت String& name, دوگانه value )
{
    fs.p->اكتب(name, value);
}

تهی اكتب( FileStorage& fs, ثابت String& name, ثابت String& value )
{
    fs.p->اكتب(name, value);
}

تهی اكتب( FileStorage& fs, ثابت String& name, منطقی value )
{
    fs.p->اكتب(name, value);
}

تهی FileStorage::اكتب(ثابت String& name, صحیح val) { p->اكتب(name, val); }
تهی FileStorage::اكتب(ثابت String& name, منطقی val) { p->اكتب(name, val); }
تهی FileStorage::اكتب(ثابت String& name, int64_t val) { p->اكتب(name, val); }
تهی FileStorage::اكتب(ثابت String& name, دوگانه val) { p->اكتب(name, val); }
تهی FileStorage::اكتب(ثابت String& name, ثابت String& val) { p->اكتب(name, val); }
تهی FileStorage::اكتب(ثابت String& name, ثابت Mat& val) { cv::اكتب(*این, name, val); }
تهی FileStorage::اكتب(ثابت String& name, ثابت std::vector<String>& val) { cv::اكتب(*این, name, val); }

FileStorage& operator << (FileStorage& fs, ثابت String& str)
{
    شمارش { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        مقدار_EXPECTED = FileStorage::مقدار_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    ثابت نویسه* _str = str.c_str();
    اگر( !fs.isOpened() || !_str )
        بازگشت fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    نویسه c = *_str;

    اگر( c == '}' || c == ']' )
    {
        اگر( fs_impl->write_پشته.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        صحیح struct_flags = fs_impl->write_پشته.back().flags;
        نویسه expected_bracket = File_گره::isMap(struct_flags) ? '}' : ']';
        اگر( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_پشته.empty());
        struct_flags = fs_impl->write_پشته.back().flags;
        fs.state = File_گره::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : مقدار_EXPECTED;
        fs.elname = String();
    }
    وگرنه اگر( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        اگر (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = مقدار_EXPECTED + INSIDE_MAP;
    }
    وگرنه اگر( (fs.state & 3) == مقدار_EXPECTED )
    {
        اگر( c == '{' || c == '[' )
        {
            صحیح struct_flags = c == '{' ? File_گره::MAP : File_گره::SEQ;
            fs.state = struct_flags == File_گره::MAP ? INSIDE_MAP + NAME_EXPECTED : مقدار_EXPECTED;
            _str++;
            اگر( *_str == ':' )
            {
                _str++;
                اگر( !*_str )
                    struct_flags |= File_گره::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        وگرنه
        {
            اكتب( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            اگر( fs.state == INSIDE_MAP + مقدار_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    وگرنه
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    بازگشت fs;
}


File_گره::File_گره()
    : fs(پوچ)
{
    blockIdx = ofs = 0;
}

File_گره::File_گره(FileStorage::Impl* _fs, size_t _blockIdx, size_t _ofs)
    : fs(_fs)
{
    blockIdx = _blockIdx;
    ofs = _ofs;
}

File_گره::File_گره(ثابت FileStorage* _fs, size_t _blockIdx, size_t _ofs)
    : File_گره(_fs->p.get(), _blockIdx, _ofs)
{
    // nothing
}

File_گره::File_گره(ثابت File_گره& گره)
{
    fs = گره.fs;
    blockIdx = گره.blockIdx;
    ofs = گره.ofs;
}

File_گره& File_گره::operator=(ثابت File_گره& گره)
{
    fs = گره.fs;
    blockIdx = گره.blockIdx;
    ofs = گره.ofs;
    بازگشت *این;
}

File_گره File_گره::operator[](ثابت std::string& nodename) ثابت
{
    اگر(!fs)
        بازگشت File_گره();

    CV_Assert( isMap() );

    unsigned کلید = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_گره_Iterator it = begin();

    برای( i = 0; i < sz; i++, ++it )
    {
        File_گره n = *it;
        ثابت uchar* p = n.ptr();
        unsigned key2 = (unsigned)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_data.size() );
        اگر( کلید == key2 )
            بازگشت n;
    }
    بازگشت File_گره();
}

File_گره File_گره::operator[](ثابت نویسه* nodename) ثابت
{
    بازگشت این->operator[](std::string(nodename));
}

File_گره File_گره::operator[](صحیح i) ثابت
{
    اگر(!fs)
        بازگشت File_گره();

    CV_Assert( isSeq() );

    صحیح sz = (صحیح)size();
    CV_Assert( 0 <= i && i < sz );

    File_گره_Iterator it = begin();
    it += i;

    بازگشت *it;
}

std::vector<String> File_گره::کلیدها() ثابت
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    برای (File_گره_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    بازگشت res;
}

صحیح File_گره::type() ثابت
{
    ثابت uchar* p = ptr();
    اگر(!p)
        بازگشت NONE;
    بازگشت (*p & TYPE_MASK);
}

منطقی File_گره::isMap(صحیح flags) { بازگشت (flags & TYPE_MASK) == MAP; }
منطقی File_گره::isSeq(صحیح flags) { بازگشت (flags & TYPE_MASK) == SEQ; }
منطقی File_گره::isCollection(صحیح flags) { بازگشت isMap(flags) || isSeq(flags); }
منطقی File_گره::isFlow(صحیح flags) { بازگشت (flags & FLOW) != 0; }
منطقی File_گره::isEmptyCollection(صحیح flags) { بازگشت (flags & EMPTY) != 0; }

منطقی File_گره::empty() ثابت   { بازگشت fs == 0; }
منطقی File_گره::isNone() ثابت  { بازگشت type() == NONE; }
منطقی File_گره::isSeq() ثابت   { بازگشت type() == SEQ; }
منطقی File_گره::isMap() ثابت   { بازگشت type() == MAP; }
منطقی File_گره::isInt() ثابت   { بازگشت type() == INT;  }
منطقی File_گره::isReal() ثابت  { بازگشت type() == REAL; }
منطقی File_گره::isString() ثابت { بازگشت type() == STRING;  }
منطقی File_گره::isNamed() ثابت
{
    ثابت uchar* p = ptr();
    اگر(!p)
        بازگشت نادرست;
    بازگشت (*p & NAMED) != 0;
}

std::string File_گره::name() ثابت
{
    ثابت uchar* p = ptr();
    اگر(!p)
        بازگشت std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    بازگشت fs->getName(nameofs);
}

File_گره::operator صحیح() ثابت
{
    ثابت uchar* p = ptr();
    اگر(!p)
        بازگشت 0;
    صحیح tag = *p;
    صحیح type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    اگر( type == INT )
    {
        بازگشت readInt(p);
    }
    وگرنه اگر( type == REAL )
    {
        بازگشت cvRound(readReal(p));
    }
    وگرنه
        بازگشت 0x7fffffff;
}

File_گره::operator int64_t() ثابت
{
    ثابت uchar* p = ptr();
    اگر(!p)
        بازگشت 0;
    صحیح tag = *p;
    صحیح type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    اگر( type == INT )
    {
        بازگشت readLong(p);
    }
    وگرنه اگر( type == REAL )
    {
        بازگشت cvRound(readReal(p));
    }
    وگرنه
        بازگشت 0x7fffffff;
}

File_گره::operator اعشاری() ثابت
{
    ثابت uchar* p = ptr();
    اگر(!p)
        بازگشت 0.f;
    صحیح tag = *p;
    صحیح type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    اگر( type == INT )
    {
        بازگشت (اعشاری)readLong(p);
    }
    وگرنه اگر( type == REAL )
    {
        بازگشت (اعشاری)readReal(p);
    }
    وگرنه
        بازگشت FLT_MAX;
}

File_گره::operator دوگانه() ثابت
{
    ثابت uchar* p = ptr();
    اگر(!p)
        بازگشت 0.f;
    صحیح tag = *p;
    صحیح type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    اگر( type == INT )
    {
        بازگشت (دوگانه)readLong(p);
    }
    وگرنه اگر( type == REAL )
    {
        بازگشت readReal(p);
    }
    وگرنه
        بازگشت DBL_MAX;
}

دوگانه File_گره::real() ثابت  { بازگشت دوگانه(*این); }

std::string File_گره::string() ثابت
{
    ثابت uchar* p = ptr();
    اگر( !p || (*p & TYPE_MASK) != STRING )
        بازگشت std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(unsigned)readInt(p);
    بازگشت std::string((ثابت نویسه*)(p + 4), sz - 1);
}
Mat File_گره::mat() ثابت { Mat value; اقرأ(*این, value, Mat()); بازگشت value; }

File_گره_Iterator File_گره::begin() ثابت { بازگشت File_گره_Iterator(*این, نادرست); }
File_گره_Iterator File_گره::end() ثابت   { بازگشت File_گره_Iterator(*این, درست); }

تهی File_گره::readRaw( ثابت std::string& fmt, تهی* vec, size_t len ) ثابت
{
    File_گره_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_گره::size() ثابت
{
    ثابت uchar* p = ptr();
    اگر( !p )
        بازگشت 0;
    صحیح tag = *p;
    صحیح tp = tag & TYPE_MASK;
    اگر( tp == MAP || tp == SEQ )
    {
        اگر( tag & NAMED )
            p += 4;
        بازگشت (size_t)(unsigned)readInt(p + 5);
    }
    بازگشت tp != NONE;
}

size_t File_گره::rawSize() ثابت
{
    ثابت uchar* p0 = ptr(), *p = p0;
    اگر( !p )
        بازگشت 0;
    صحیح tag = *p++;
    صحیح tp = tag & TYPE_MASK;
    اگر( tag & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    اگر( tp == INT )
        بازگشت sz0 + 8;
    اگر( tp == REAL )
        بازگشت sz0 + 8;
    اگر( tp == NONE )
        بازگشت sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    بازگشت sz0 + 4 + readInt(p);
}

uchar* File_گره::ptr()
{
    بازگشت !fs ? 0 : (uchar*)fs->get_گره_Ptr(blockIdx, ofs);
}

ثابت uchar* File_گره::ptr() ثابت
{
    بازگشت !fs ? 0 : fs->get_گره_Ptr(blockIdx, ofs);
}

تهی File_گره::set_مقدار( صحیح type, ثابت تهی* value, صحیح len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    صحیح tag = *p;
    صحیح current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    صحیح sz = 1;

    اگر( tag & NAMED )
        sz += 4;

    اگر( type == INT )
        sz += 8;
    وگرنه اگر( type == REAL )
        sz += 8;
    وگرنه اگر( type == STRING )
    {
        اگر( len < 0 )
            len = (صحیح)strlen((ثابت نویسه*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    وگرنه
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_گره_Space(*این, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    اگر( tag & NAMED )
        p += 4;

    اگر( type == INT )
    {
        int64_t ival = *(ثابت int64_t*)value;
        writeInt(p, ival);
    }
    وگرنه اگر( type == REAL )
    {
        دوگانه dbval = *(ثابت دوگانه*)value;
        writeReal(p, dbval);
    }
    وگرنه اگر( type == STRING )
    {
        ثابت نویسه* str = (ثابت نویسه*)value;
        writeInt(p, len + 1);
        memcpy(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_گره_Iterator::File_گره_Iterator()
{
    fs = 0;
    blockIdx = 0;
    ofs = 0;
    blockSize = 0;
    گره_NElems = 0;
    idx = 0;
}

File_گره_Iterator::File_گره_Iterator( ثابت File_گره& گره, منطقی seekEnd )
{
    fs = گره.fs;
    idx = 0;
    اگر( !fs )
        blockIdx = ofs = blockSize = گره_NElems = 0;
    وگرنه
    {
        blockIdx = گره.blockIdx;
        ofs = گره.ofs;

        منطقی collection = گره.isSeq() || گره.isMap();
        اگر( گره.isNone() )
        {
            گره_NElems = 0;
        }
        وگرنه اگر( !collection )
        {
            گره_NElems = 1;
            اگر( seekEnd )
            {
                idx = 1;
                ofs += گره.rawSize();
            }
        }
        وگرنه
        {
            گره_NElems = گره.size();
            ثابت uchar* p0 = گره.ptr(), *p = p0 + 1;
            اگر(*p0 & File_گره::NAMED )
                p += 4;
            اگر( !seekEnd )
                ofs += (p - p0) + 8;
            وگرنه
            {
                size_t rawsz = (size_t)(unsigned)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = گره_NElems;
            }
        }
        fs->normalize_گره_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
}

File_گره_Iterator::File_گره_Iterator(ثابت File_گره_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    گره_NElems = it.گره_NElems;
    idx = it.idx;
}

File_گره_Iterator& File_گره_Iterator::operator=(ثابت File_گره_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    گره_NElems = it.گره_NElems;
    idx = it.idx;
    بازگشت *این;
}

File_گره File_گره_Iterator::operator *() ثابت
{
    بازگشت File_گره(idx < گره_NElems ? fs : پوچ, blockIdx, ofs);
}

File_گره_Iterator& File_گره_Iterator::operator ++ ()
{
    اگر( idx == گره_NElems || !fs )
        بازگشت *این;
    idx++;
    File_گره n(fs, blockIdx, ofs);
    ofs += n.rawSize();
    اگر( ofs >= blockSize )
    {
        fs->normalize_گره_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
    بازگشت *این;
}

File_گره_Iterator File_گره_Iterator::operator ++ (صحیح)
{
    File_گره_Iterator it = *این;
    ++(*این);
    بازگشت it;
}

File_گره_Iterator& File_گره_Iterator::operator += (صحیح _ofs)
{
    CV_Assert( _ofs >= 0 );
    برای( ; _ofs > 0; _ofs-- )
        این->operator ++();
    بازگشت *این;
}

File_گره_Iterator& File_گره_Iterator::readRaw( ثابت String& fmt, تهی* _data0, size_t maxsz)
{
    اگر( fs && idx < گره_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        صحیح fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        صحیح fmt_pair_شمار = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        size_t esz = fs::calcStructSize( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        برای( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            برای( صحیح k = 0; k < fmt_pair_شمار; k++ )
            {
                صحیح elem_type = fmt_pairs[k*2+1];
                صحیح elem_size = CV_ELEM_SIZE(elem_type);

                صحیح شمار_2 = fmt_pairs[k*2];
                offset = alignSize( offset, elem_size );
                uchar* data = data0 + offset;

                برای( صحیح i = 0; i < شمار_2; i++, ++(*این) )
                {
                    File_گره گره = *(*این);
                    اگر( گره.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_size == 8 ? (int64_t)گره : (صحیح)گره);
                        انتخاب( elem_type )
                        {
                        حالت CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(ival);
                            data++;
                            بشکن;
                        حالت CV_8S:
                            *(نویسه*)data = saturate_cast<schar>(ival);
                            data++;
                            بشکن;
                        حالت CV_Bool:
                            *(منطقی*)data = ival != 0;
                            data++;
                            بشکن;
                        حالت CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(ival);
                            data += اندازه(ushort);
                            بشکن;
                        حالت CV_16S:
                            *(short*)data = saturate_cast<short>(ival);
                            data += اندازه(short);
                            بشکن;
                        حالت CV_32U:
                            *(unsigned*)data = (unsigned)std::max(ival, (int64_t)0);
                            data += اندازه(unsigned);
                            بشکن;
                        حالت CV_32S:
                            *(صحیح*)data = (صحیح)ival;
                            data += اندازه(صحیح);
                            بشکن;
                        حالت CV_32F:
                            *(اعشاری*)data = (اعشاری)ival;
                            data += اندازه(اعشاری);
                            بشکن;
                        حالت CV_64U:
                            *(uint64_t*)data = (uint64_t)ival;
                            data += اندازه(uint64_t);
                            بشکن;
                        حالت CV_64S:
                            *(int64_t*)data = (int64_t)ival;
                            data += اندازه(int64_t);
                            بشکن;
                        حالت CV_64F:
                            *(دوگانه*)data = (دوگانه)ival;
                            data += اندازه(دوگانه);
                            بشکن;
                        حالت CV_16F:
                            *(hfloat*)data = hfloat((اعشاری)ival);
                            data += اندازه(hfloat);
                            بشکن;
                        حالت CV_16BF:
                            *(bfloat*)data = bfloat((اعشاری)ival);
                            data += اندازه(bfloat);
                            بشکن;
                        حالت CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((اعشاری)ival);
                            data += اندازه(fp8_t);
                            بشکن;
                        حالت CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((اعشاری)ival);
                            data += اندازه(fp8a_t);
                            بشکن;
                        پیشفرض:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    وگرنه اگر( گره.isReal() )
                    {
                        دوگانه fval = (دوگانه)گره;

                        انتخاب( elem_type )
                        {
                        حالت CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(fval);
                            data++;
                            بشکن;
                        حالت CV_8S:
                            *(نویسه*)data = saturate_cast<schar>(fval);
                            data++;
                            بشکن;
                        حالت CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(fval);
                            data += اندازه(ushort);
                            بشکن;
                        حالت CV_16S:
                            *(short*)data = saturate_cast<short>(fval);
                            data += اندازه(short);
                            بشکن;
                        حالت CV_32U:
                            *(صحیح*)data = saturate_cast<unsigned>(fval);
                            data += اندازه(صحیح);
                            بشکن;
                        حالت CV_32S:
                            *(صحیح*)data = saturate_cast<صحیح>(fval);
                            data += اندازه(صحیح);
                            بشکن;
                        حالت CV_32F:
                            *(اعشاری*)data = (اعشاری)fval;
                            data += اندازه(اعشاری);
                            بشکن;
                        حالت CV_64U:
                            *(uint64_t*)data = (uint64_t)round(std::max(fval, 0.));
                            data += اندازه(uint64_t);
                            بشکن;
                        حالت CV_64S:
                            *(int64_t*)data = (int64_t)round(std::max(fval, 0.));
                            data += اندازه(int64_t);
                            بشکن;
                        حالت CV_64F:
                            *(دوگانه*)data = fval;
                            data += اندازه(دوگانه);
                            بشکن;
                        حالت CV_16F:
                            *(hfloat*)data = hfloat((اعشاری)fval);
                            data += اندازه(hfloat);
                            بشکن;
                        حالت CV_16BF:
                            *(bfloat*)data = bfloat((اعشاری)fval);
                            data += اندازه(bfloat);
                            بشکن;
                        حالت CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((اعشاری)fval);
                            data += اندازه(fp8_t);
                            بشکن;
                        حالت CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((اعشاری)fval);
                            data += اندازه(fp8a_t);
                            بشکن;
                        پیشفرض:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    وگرنه
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (صحیح)(data - data0);
            }
        }
    }

    بازگشت *این;
}

منطقی File_گره_Iterator::equalTo(ثابت File_گره_Iterator& it) ثابت
{
    بازگشت fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && گره_NElems == it.گره_NElems;
}

size_t File_گره_Iterator::remaining() ثابت
{
    بازگشت گره_NElems - idx;
}

منطقی operator == ( ثابت File_گره_Iterator& it1, ثابت File_گره_Iterator& it2 )
{
    بازگشت it1.equalTo(it2);
}

منطقی operator != ( ثابت File_گره_Iterator& it1, ثابت File_گره_Iterator& it2 )
{
    بازگشت !it1.equalTo(it2);
}

تهی اقرأ(ثابت File_گره& گره, صحیح& val, صحیح default_val)
{
    val = default_val;
    اگر( !گره.empty() )
    {
        val = (صحیح)گره;
    }
}

تهی اقرأ(ثابت File_گره& گره, int64_t& val, int64_t default_val)
{
    val = default_val;
    اگر( !گره.empty() )
    {
        val = (int64_t)گره;
    }
}

تهی اقرأ(ثابت File_گره& گره, دوگانه& val, دوگانه default_val)
{
    val = default_val;
    اگر( !گره.empty() )
    {
        val = (دوگانه)گره;
    }
}

تهی اقرأ(ثابت File_گره& گره, اعشاری& val, اعشاری default_val)
{
    val = default_val;
    اگر( !گره.empty() )
    {
        val = (اعشاری)گره;
    }
}

تهی اقرأ(ثابت File_گره& گره, std::string& val, ثابت std::string& default_val)
{
    val = default_val;
    اگر( !گره.empty() )
    {
        val = (std::string)گره;
    }
}
تهی FileStorage::Impl::اكتب(ثابت String &کلید, منطقی value)
{
    CV_Assert(write_mode);
    اگر (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().اكتب(کلید.c_str(), (صحیح)value);
    }
    وگرنه
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().اكتب(کلید.c_str(), value ? "true" : "false", نادرست);
    }
}

FileStorage_API::~FileStorage_API() {}

فضاینام internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, ثابت std::string& name,
                                       صحیح flags, ثابت std::string& typeName)
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
