// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "persistence.hpp"
#অন্তর্ভাৱ "persistence_impl.hpp"
#অন্তর্ভাৱ "persistence_base64_encoding.hpp"
#অন্তর্ভাৱ <unordered_map>
#অন্তর্ভাৱ <iterator>

#অন্তর্ভাৱ <opencv2/core/utils/logger.hpp>

#define CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

নামাকাশ cv
{

নামাকাশ fs
{

পূর্ণাঙ্ক strcasecmp(ধ্রুৱ ৱর্ণ* s1, ধ্রুৱ ৱর্ণ* s2)
{
    ধ্রুৱ ৱর্ণ* dummy="";
    যদি(!s1) s1=dummy;
    যদি(!s2) s2=dummy;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t i, len = std::min(len1, len2);
    বাবে( i = 0; i < len; i++ )
    {
        পূর্ণাঙ্ক d = tolower((পূর্ণাঙ্ক)s1[i]) - tolower((পূর্ণাঙ্ক)s2[i]);
        যদি( d != 0 )
            ঘূৰাই d;
    }
    ঘূৰাই len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

ৱর্ণ* itoa( পূর্ণাঙ্ক _val, ৱর্ণ* buffer, পূর্ণাঙ্ক /*radix*/ )
{
    ধ্রুৱ পূর্ণাঙ্ক radix = 10;
    ৱর্ণ* ptr=buffer + 23 /* enough even for 64-bit integers */;
    unsigned val = abs(_val);

    *ptr = '\0';
    কৰা
    {
        unsigned r = val / radix;
        *--ptr = (ৱর্ণ)(val - (r*radix) + '0');
        val = r;
    }
    যেতিয়ালৈ( val != 0 );

    যদি( _val < 0 )
        *--ptr = '-';

    ঘূৰাই ptr;
}

ৱর্ণ* itoa( int64_t _val, ৱর্ণ* buffer, পূর্ণাঙ্ক /*radix*/, তার্কিক _signed)
{
    ধ্রুৱ পূর্ণাঙ্ক radix = 10;
    ৱর্ণ* ptr=buffer + 23 /* enough even for 64-bit integers */;
    পূর্ণাঙ্ক sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : abs(_val);

    *ptr = '\0';
    কৰা
    {
        uint64_t r = val / radix;
        *--ptr = (ৱর্ণ)(val - (r*radix) + '0');
        val = r;
    }
    যেতিয়ালৈ( val != 0 );

    যদি( sign < 0 )
        *--ptr = '-';

    ঘূৰাই ptr;
}

ৱর্ণ* doubleToString( ৱর্ণ* buf, size_t bufSize, দ্ৱিগুণ value, তার্কিক explicitZero )
{
    Cv64suf val;
    unsigned ieee754_hi;

    val.f = value;
    ieee754_hi = (unsigned)(val.u >> 32);

    যদি( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        পূর্ণাঙ্ক ivalue = cvRound(value);
        যদি( ivalue == value )
        {
            যদি( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            অন্যথা
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        অন্যথা
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, bufSize, "%.17g", value );

            ৱর্ণ* ptr = buf;
            যদি( *ptr == '+' || *ptr == '-' )
                ptr++;
            বাবে( ; cv_isdigit(*ptr); ptr++ )
                ;
            যদি( *ptr == ',' )
                *ptr = '.';
        }
    }
    অন্যথা
    {
        unsigned ieee754_lo = (unsigned)val.u;
        যদি( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        অন্যথা
            strcpy( buf, (পূর্ণাঙ্ক)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    ঘূৰাই buf;
}

ৱর্ণ* floatToString( ৱর্ণ* buf, size_t bufSize, ভিন্ন value, তার্কিক halfprecision, তার্কিক explicitZero )
{
    Cv32suf val;
    unsigned ieee754;
    val.f = value;
    ieee754 = val.u;

    যদি( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        পূর্ণাঙ্ক ivalue = cvRound(value);
        যদি( ivalue == value )
        {
            যদি( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            অন্যথা
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        অন্যথা
        {
            যদি (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.5g", value);
            }
            অন্যথা
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.9g", value);
            }

            ৱর্ণ* ptr = buf;
            যদি( *ptr == '+' || *ptr == '-' )
                ptr++;
            বাবে( ; cv_isdigit(*ptr); ptr++ )
                ;
            যদি( *ptr == ',' )
                *ptr = '.';
        }
    }
    অন্যথা
    {
        যদি( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        অন্যথা
            strcpy( buf, (পূর্ণাঙ্ক)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    ঘূৰাই buf;
}

স্থির ধ্রুৱ ৱর্ণ symbols[] = "ucwsifdhHbUIneE";

স্থির ৱর্ণ type_প্ৰতীক(পূর্ণাঙ্ক depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    ঘূৰাই symbols[depth];
}

স্থির পূর্ণাঙ্ক প্ৰতীক_To_Type(ৱর্ণ c)
{
    যদি (c == 'r')
        ঘূৰাই CV_SEQ_ELTYPE_PTR;
    ধ্রুৱ ৱর্ণ* pos = strchr( symbols, c );
    যদি( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    ঘূৰাই static_cast<পূর্ণাঙ্ক>(pos - symbols);
}

ৱর্ণ* encodeFormat(পূর্ণাঙ্ক elem_type, ৱর্ণ* dt, size_t dt_len)
{
    পূর্ণাঙ্ক cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    ৱর্ণ প্ৰতীক = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_প্ৰতীক(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, প্ৰতীক);
    ঘূৰাই dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
ৱর্ণ* encodeFormat(পূর্ণাঙ্ক elem_type, ৱর্ণ* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    ঘূৰাই encodeFormat(elem_type, dt, max);
}

পূর্ণাঙ্ক decodeFormat( ধ্রুৱ ৱর্ণ* dt, পূর্ণাঙ্ক* fmt_pairs, পূর্ণাঙ্ক max_len )
{
    পূর্ণাঙ্ক fmt_pair_গুনতি = 0;
    পূর্ণাঙ্ক i = 0, k = 0, len = dt ? (পূর্ণাঙ্ক)strlen(dt) : 0;

    যদি( !dt || !len )
        ঘূৰাই 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    বাবে( ; k < len; k++ )
    {
        ৱর্ণ c = dt[k];

        যদি( cv_isdigit(c) )
        {
            পূর্ণাঙ্ক গুনতি_2 = c - '0';
            যদি( cv_isdigit(dt[k+1]) )
            {
                ৱর্ণ* endptr = 0;
                গুনতি_2 = (পূর্ণাঙ্ক)strtol( dt+k, &endptr, 10 );
                k = (পূর্ণাঙ্ক)(endptr - dt) - 1;
            }

            যদি( গুনতি_2 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = গুনতি_2;
        }
        অন্যথা
        {
            পূর্ণাঙ্ক depth = প্ৰতীক_To_Type(c);
            যদি( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = depth;
            যদি( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            অন্যথা
            {
                i += 2;
                যদি( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_গুনতি = i/2;
    ঘূৰাই fmt_pair_গুনতি;
}

পূর্ণাঙ্ক calcElemSize( ধ্রুৱ ৱর্ণ* dt, পূর্ণাঙ্ক initial_size )
{
    পূর্ণাঙ্ক size = 0;
    পূর্ণাঙ্ক fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_গুনতি;
    পূর্ণাঙ্ক comp_size;

    fmt_pair_গুনতি = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_গুনতি *= 2;
    বাবে( i = 0, size = initial_size; i < fmt_pair_গুনতি; i += 2 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[i+1]);
        size = cvAlign( size, comp_size );
        size += comp_size * fmt_pairs[i];
    }
    যদি( initial_size == 0 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[1]);
        size = cvAlign( size, comp_size );
    }
    ঘূৰাই size;
}


পূর্ণাঙ্ক calcStructSize( ধ্রুৱ ৱর্ণ* dt, পূর্ণাঙ্ক initial_size )
{
    পূর্ণাঙ্ক size = calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    বাবে ( ধ্রুৱ ৱর্ণ * type = dt; *type != '\0'; type++ )
    {
        ৱর্ণ v = *type;
        যদি (v >= '0' && v <= '9')
            অনুৱর্ত;  // skip vector size
        ৱিকল্প (v)
        {
        পক্ষ 'u': { elem_max_size = std::max( elem_max_size, পরিমাণ(uchar ) ); ভঙ্গ; }
        পক্ষ 'b': { elem_max_size = std::max( elem_max_size, পরিমাণ(তার্কিক  ) ); ভঙ্গ; }
        পক্ষ 'c': { elem_max_size = std::max( elem_max_size, পরিমাণ(schar ) ); ভঙ্গ; }
        পক্ষ 'w': { elem_max_size = std::max( elem_max_size, পরিমাণ(ushort) ); ভঙ্গ; }
        পক্ষ 's': { elem_max_size = std::max( elem_max_size, পরিমাণ(short ) ); ভঙ্গ; }
        পক্ষ 'i': { elem_max_size = std::max( elem_max_size, পরিমাণ(পূর্ণাঙ্ক   ) ); ভঙ্গ; }
        পক্ষ 'n': { elem_max_size = std::max( elem_max_size, পরিমাণ(unsigned) ); ভঙ্গ; }
        পক্ষ 'f': { elem_max_size = std::max( elem_max_size, পরিমাণ(ভিন্ন ) ); ভঙ্গ; }
        পক্ষ 'd': { elem_max_size = std::max( elem_max_size, পরিমাণ(দ্ৱিগুণ) ); ভঙ্গ; }
        পক্ষ 'h': { elem_max_size = std::max( elem_max_size, পরিমাণ(hfloat)); ভঙ্গ; }
        পক্ষ 'H': { elem_max_size = std::max( elem_max_size, পরিমাণ(bfloat)); ভঙ্গ; }
        পক্ষ 'e': { elem_max_size = std::max( elem_max_size, পরিমাণ(fp8_t) ); ভঙ্গ; }
        পক্ষ 'E': { elem_max_size = std::max( elem_max_size, পরিমাণ(fp8a_t)); ভঙ্গ; }
        পক্ষ 'I': { elem_max_size = std::max( elem_max_size, পরিমাণ(int64_t)); ভঙ্গ; }
        পক্ষ 'U': { elem_max_size = std::max( elem_max_size, পরিমাণ(uint64_t)); ভঙ্গ; }
        স্ৱভাৱ:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (ৱর্ণ)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<পূর্ণাঙ্ক>(elem_max_size) );
    ঘূৰাই size;
}

পূর্ণাঙ্ক decodeSimpleFormat( ধ্রুৱ ৱর্ণ* dt )
{
    পূর্ণাঙ্ক elem_type = -1;
    পূর্ণাঙ্ক fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_গুনতি;

    fmt_pair_গুনতি = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    যদি( fmt_pair_গুনতি != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    ঘূৰাই elem_type;
}

}

#যদি defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#define CV_LITTLE_ENDIAN_MEM_ACCESS 1
#অন্যথা
#define CV_LITTLE_ENDIAN_MEM_ACCESS 0
#endif

স্থির অন্তঃস্থ পূর্ণাঙ্ক readInt(ধ্রুৱ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#যদি CV_LITTLE_ENDIAN_MEM_ACCESS
    পূর্ণাঙ্ক val;
    memcpy(&val, p, পরিমাণ(val));
    ঘূৰাই val;
#অন্যথা
    পূর্ণাঙ্ক val = (পূর্ণাঙ্ক)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    ঘূৰাই val;
#endif
}

স্থির অন্তঃস্থ int64_t readLong(ধ্রুৱ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#যদি CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    memcpy(&val, p, পরিমাণ(val));
    ঘূৰাই val;
#অন্যথা
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    ঘূৰাই val0 | ((int64_t)val1 << 32);
#endif
}

স্থির অন্তঃস্থ দ্ৱিগুণ readReal(ধ্রুৱ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#যদি CV_LITTLE_ENDIAN_MEM_ACCESS
    দ্ৱিগুণ val;
    memcpy(&val, p, পরিমাণ(val));
    ঘূৰাই val;
#অন্যথা
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    ঘূৰাই val.f;
#endif
}

template <typename T>
স্থির অন্তঃস্থ শূন্য writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#যদি CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &ival, পরিমাণ(ival));
#অন্যথা
    বাবে (size_t i = 0, j = 0; i < পরিমাণ(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#endif
}

স্থির অন্তঃস্থ শূন্য writeReal(uchar* p, দ্ৱিগুণ fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#যদি CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &fval, পরিমাণ(fval));
#অন্যথা
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



শূন্য FileStorage::Impl::init() {
    flags = 0;
    buffer.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = মিছা;
    state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = মিছা;
    delayed_struct_চাবি = nullptr;
    delayed_struct_flags = 0;
    delayed_type_name = nullptr;
    base64_writer = nullptr;
    is_opened = মিছা;
    dummy_eof = মিছা;
    write_mode = মিছা;
    mem_mode = মিছা;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    empty_stream = সঁচা;

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

শূন্য FileStorage::Impl::release(String *out) {
    যদি (is_opened) {
        যদি (out)
            out->clear();
        যদি (write_mode) {
            যেতিয়ালৈ (write_স্তূপ.size() > 1) {
                endWriteStruct();
            }
            flush();
            যদি (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            অন্যথা যদি (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        যদি (mem_mode && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

শূন্য FileStorage::Impl::analyze_file_name(ধ্রুৱ std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    স্থির ধ্রুৱ ৱর্ণ not_file_name = '\n';
    স্থির ধ্রুৱ ৱর্ণ parameter_begin = '?';
    স্থির ধ্রুৱ ৱর্ণ parameter_separator = '&';

    যদি (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        ঘূৰাই;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    যদি (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        বাবে (size_t param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            যদি ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

তার্কিক FileStorage::Impl::open(ধ্রুৱ ৱর্ণ *filename_or_buf, পূর্ণাঙ্ক _flags, ধ্রুৱ ৱর্ণ *encoding) {
    তার্কিক ok = সঁচা;
    release();

    তার্কিক append = (_flags & 3) == FileStorage::APPEND;
    mem_mode = (_flags & FileStorage::MEMORY) != 0;

    write_mode = (_flags & 3) != 0;
    তার্কিক write_base64 = (write_mode || append) && (_flags & FileStorage::BASE64) != 0;

    তার্কিক isGZ = মিছা;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        যদি (!params.empty())
            filename = params[0];

        যদি (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_mode || append);
    }

    যদি (filename.size() == 0 && !mem_mode && !write_mode)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    যদি (mem_mode && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    যদি (!mem_mode) {
        size_t dot_idx = filename.find_last_of('.');
        ৱর্ণ compression = '\0';

        যদি (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            যদি (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                যদি (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = সঁচা;
                }

                অন্যথা যদি(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = সঁচা;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        যদি (!isGZ) {
            file = fopen(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            যদি (!file)
            {
                CV_LOG_ERROR(রিক্ত, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                ঘূৰাই মিছা;
            }
        } অন্যথা {
#যদি USE_ZLIB
            যদি (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            ৱর্ণ mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            যদি (!gzfile)
            {
                CV_LOG_ERROR(রিক্ত, "Can't open archive: '" << filename << "' mode=" << mode);
                ঘূৰাই মিছা;
            }
#অন্যথা
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#endif
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_data.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    যদি (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        যদি (mem_mode)
            outbuf.clear();

        যদি (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            ধ্রুৱ ৱর্ণ *dot_pos = রিক্ত;
            ধ্রুৱ ৱর্ণ *dot_pos2 = রিক্ত;
            // like strrchr() implementation, but save two last positions simultaneously
            বাবে (ধ্রুৱ ৱর্ণ *pos = &filename[0]; pos[0] != 0; pos++) {
                যদি (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            যদি (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != রিক্ত) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } অন্যথা যদি (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        পূর্ণাঙ্ক buf_size = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        যদি (append) {
            fseek(file, 0, SEEK_END);
            যদি (ftell(file) == 0)
                append = মিছা;
        }

        write_স্তূপ.clear();
        empty_stream = সঁচা;
        write_স্তূপ.push_back(FStructData("", File_গ্ৰন্থি::MAP | File_গ্ৰন্থি::EMPTY, 0));
        buffer.reserve(buf_size + 1024);
        buffer.resize(buf_size);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;

        যদি (fmt == FileStorage::FORMAT_XML) {
            size_t file_size = file ? (size_t) ftell(file) : (size_t) 0;
            যদি (!append || file_size == 0) {
                যদি (encoding && *encoding != '\0') {
                    যদি (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(strlen(encoding) < 1000);
                    ৱর্ণ buf[1100];
                    snprintf(buf, পরিমাণ(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } অন্যথা
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } অন্যথা {
                পূর্ণাঙ্ক xml_buf_size = 1 << 10;
                ৱর্ণ substr[] = "</opencv_storage>";
                পূর্ণাঙ্ক last_occurrence = -1;
                xml_buf_size = MIN(xml_buf_size, পূর্ণাঙ্ক(file_size));
                fseek(file, -xml_buf_size, SEEK_END);
                // find the last occurrence of </opencv_storage>
                বাবে (;;) {
                    পূর্ণাঙ্ক সাৰি_offset = (পূর্ণাঙ্ক) ftell(file);
                    ধ্রুৱ ৱর্ণ *ptr0 = এতত্->gets(xml_buf_size);
                    ধ্রুৱ ৱর্ণ *ptr = রিক্ত;
                    যদি (!ptr0)
                        ভঙ্গ;
                    ptr = ptr0;
                    বাবে (;;) {
                        ptr = strstr(ptr, substr);
                        যদি (!ptr)
                            ভঙ্গ;
                        last_occurrence = সাৰি_offset + (পূর্ণাঙ্ক) (ptr - ptr0);
                        ptr += strlen(substr);
                    }
                }
                যদি (last_occurrence < 0) {
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

            emitter_do_not_use_direct_dereference = createXMLEmitter(এতত্);
        } অন্যথা যদি (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                যদি (!append) {
                    যদি (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    অন্যথা
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            অন্যথা
                puts("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(এতত্);
        } অন্যথা {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            যদি (!append)
                puts("{\n");
            অন্যথা {
                তার্কিক valid = মিছা;
                long roffset = 0;
                বাবে (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    ধ্রুৱ ৱর্ণ end_mark = '}';
                    যদি (fgetc(file) == end_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = সঁচা;
                        ভঙ্গ;
                    }
                }

                যদি (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } অন্যথা {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_স্তূপ.back().indent = 4;
            emitter_do_not_use_direct_dereference = createJSONEmitter(এতত্);
        }
        is_opened = সঁচা;
    } অন্যথা {
        ধ্রুৱ size_t buf_size0 = 40;
        buffer.resize(buf_size0);
        যদি (mem_mode) {
            strbuf = (ৱর্ণ *) filename_or_buf;
            strbufsize = strlen(strbuf);
        }

        ধ্রুৱ ৱর্ণ *yaml_signature = "%YAML";
        ধ্রুৱ ৱর্ণ *json_signature = "{";
        ধ্রুৱ ৱর্ণ *xml_signature = "<?xml";
        ৱর্ণ *buf = এতত্->gets(16);
        CV_Assert(buf);
        ৱর্ণ *bufPtr = cv_skip_BOM(buf);
        size_t bufOffset = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        যদি (strncmp(bufPtr, yaml_signature, strlen(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        অন্যথা যদি (strncmp(bufPtr, json_signature, strlen(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        অন্যথা যদি (strncmp(bufPtr, xml_signature, strlen(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        অন্যথা
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = bufOffset;
        bufofs = 0;

        যত্ন {
            ৱর্ণ *ptr = bufferStart();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_গ্ৰন্থি মূল_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_গ্ৰন্থি_Space(মূল_nodes, 9);
            *rptr = File_গ্ৰন্থি::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            ৱিকল্প (fmt) {
                পক্ষ FileStorage::FORMAT_XML:
                    parser_do_not_use_direct_dereference = createXMLParser(এতত্);
                    ভঙ্গ;
                পক্ষ FileStorage::FORMAT_YAML:
                পক্ষ FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_direct_dereference = createYAMLParser(এতত্);
                    ভঙ্গ;
                পক্ষ FileStorage::FORMAT_JSON:
                    parser_do_not_use_direct_dereference = createJSONParser(এতত্);
                    ভঙ্গ;
                স্ৱভাৱ:
                    parser_do_not_use_direct_dereference = Ptr<FileStorageParser>();
            }

            যদি (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                যদি (ok) {
                    finalizeCollection(মূল_nodes);

                    CV_Assert(!fs_data_ptrs.empty());
                    File_গ্ৰন্থি roots_গ্ৰন্থি(fs_ext, 0, 0);
                    size_t i, nroots = roots_গ্ৰন্থি.size();
                    File_গ্ৰন্থি_Iterator it = roots_গ্ৰন্থি.begin();

                    বাবে (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        গৃহাণ (...)
        {
            // FIXIT log error message
            is_opened = সঁচা;
            release();
            ক্ষিপ;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = সঁচা;
        std::vector<ৱর্ণ> tmpbuf;
        std::swap(buffer, tmpbuf);
        bufofs = 0;
    }
    ঘূৰাই ok;
}

শূন্য FileStorage::Impl::puts(ধ্রুৱ ৱর্ণ *str) {
    CV_Assert(write_mode);
    যদি (mem_mode)
        std::copy(str, str + strlen(str), std::back_inserter(outbuf));
    অন্যথা যদি (file)
        fputs(str, file);
#যদি USE_ZLIB
    অন্যথা যদি (gzfile)
        gzputs(gzfile, str);
#endif
    অন্যথা
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

ৱর্ণ *FileStorage::Impl::getsFromFile(ৱর্ণ *buf, পূর্ণাঙ্ক গুনতি_2) {
    যদি (file)
        ঘূৰাই fgets(buf, গুনতি_2, file);
#যদি USE_ZLIB
    যদি (gzfile)
        ঘূৰাই gzgets(gzfile, buf, গুনতি_2);
#endif
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

ৱর্ণ *FileStorage::Impl::gets(size_t max_গুনতি) {
    যদি (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        ধ্রুৱ ৱর্ণ *instr = strbuf;
        বাবে (; i < len; i++) {
            ৱর্ণ c = instr[i];
            যদি (c == '\0' || c == '\n') {
                যদি (c == '\n')
                    i++;
                ভঙ্গ;
            }
        }
        size_t গুনতি_2 = i - strbufpos;
        যদি (max_গুনতি == 0 || max_গুনতি > গুনতি_2)
            max_গুনতি = গুনতি_2;
        buffer.resize(std::max(buffer.size(), max_গুনতি + 8));
        memcpy(&buffer[0], instr + strbufpos, max_গুনতি);
        buffer[max_গুনতি] = '\0';
        strbufpos = i;
        ঘূৰাই max_গুনতি > 0 ? &buffer[0] : 0;
    }

    ধ্রুৱ size_t MAX_BLOCK_SIZE = INT_MAX / 2; // hopefully, that will be enough
    যদি (max_গুনতি == 0)
        max_গুনতি = MAX_BLOCK_SIZE;
    অন্যথা
        CV_Assert(max_গুনতি < MAX_BLOCK_SIZE);
    size_t ofs = 0;

    বাবে (;;) {
        পূর্ণাঙ্ক গুনতি_2 = (পূর্ণাঙ্ক) std::min(buffer.size() - ofs - 16, max_গুনতি);
        ৱর্ণ *ptr = getsFromFile(&buffer[ofs], গুনতি_2 + 1);
        যদি (!ptr)
            ভঙ্গ;
        পূর্ণাঙ্ক delta = (পূর্ণাঙ্ক) strlen(ptr);
        ofs += delta;
        max_গুনতি -= delta;
        যদি (delta == 0 || ptr[delta - 1] == '\n' || max_গুনতি == 0)
            ভঙ্গ;
        যদি (delta == গুনতি_2)
            buffer.resize((size_t) (buffer.size() * 1.5));
    }
    ঘূৰাই ofs > 0 ? &buffer[0] : 0;
}

ৱর্ণ *FileStorage::Impl::gets() {
    ৱর্ণ *ptr = এতত্->gets(0);
    যদি (!ptr) {
        ptr = bufferStart();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        ঘূৰাই 0;
    } অন্যথা {
        size_t l = strlen(ptr);
        যদি (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    ঘূৰাই ptr;
}

তার্কিক FileStorage::Impl::eof() {
    যদি (dummy_eof)
        ঘূৰাই সঁচা;
    যদি (strbuf)
        ঘূৰাই strbufpos >= strbufsize;
    যদি (file)
        ঘূৰাই feof(file) != 0;
#যদি USE_ZLIB
    যদি (gzfile)
        ঘূৰাই gzeof(gzfile) != 0;
#endif
    ঘূৰাই মিছা;
}

শূন্য FileStorage::Impl::setEof() {
    dummy_eof = সঁচা;
}

শূন্য FileStorage::Impl::closeFile() {
    যদি (file)
        fclose(file);
#যদি USE_ZLIB
    অন্যথা যদি (gzfile)
        gzclose(gzfile);
#endif
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = মিছা;
}

শূন্য FileStorage::Impl::rewind() {
    যদি (file)
        ::rewind(file);
#যদি USE_ZLIB
    অন্যথা যদি (gzfile)
        gzrewind(gzfile);
#endif
    strbufpos = 0;
}

ৱর্ণ *FileStorage::Impl::resizeWriteBuffer(ৱর্ণ *ptr, পূর্ণাঙ্ক len) {
    ধ্রুৱ ৱর্ণ *buffer_end = &buffer[0] + buffer.size();
    যদি (ptr + len < buffer_end)
        ঘূৰাই ptr;

    ধ্রুৱ ৱর্ণ *buffer_start = &buffer[0];
    পূর্ণাঙ্ক written_len = (পূর্ণাঙ্ক) (ptr - buffer_start);

    CV_Assert(written_len <= (পূর্ণাঙ্ক) buffer.size());
    পূর্ণাঙ্ক new_size = (পূর্ণাঙ্ক) ((buffer_end - buffer_start) * 3 / 2);
    new_size = MAX(written_len + len, new_size);
    buffer.reserve(new_size + 256);
    buffer.resize(new_size);
    bufofs = written_len;
    ঘূৰাই &buffer[0] + bufofs;
}

ৱর্ণ *FileStorage::Impl::flush() {
    ৱর্ণ *buffer_start = &buffer[0];
    ৱর্ণ *ptr = buffer_start + bufofs;

    যদি (ptr > buffer_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(buffer_start);
        bufofs = 0;
    }

    পূর্ণাঙ্ক indent = write_স্তূপ.back().indent;

    যদি (space != indent) {
        memset(buffer_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = buffer_start + bufofs;

    ঘূৰাই ptr;
}

শূন্য FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(মিছা);
    যদি (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_স্তূপ.empty());

    FStructData &current_struct = write_স্তূপ.back();
    যদি (fmt == FileStorage::FORMAT_JSON && !File_গ্ৰন্থি::isFlow(current_struct.flags) && write_স্তূপ.size() > 1)
        current_struct.indent = write_স্তূপ[write_স্তূপ.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_স্তূপ.pop_back();
    যদি (!write_স্তূপ.empty())
        write_স্তূপ.back().flags &= ~File_গ্ৰন্থি::EMPTY;
}

শূন্য FileStorage::Impl::startWriteStruct_helper(ধ্রুৱ ৱর্ণ *চাবি, পূর্ণাঙ্ক struct_flags,
                                                ধ্রুৱ ৱর্ণ *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (File_গ্ৰন্থি::TYPE_MASK | File_গ্ৰন্থি::FLOW)) | File_গ্ৰন্থি::EMPTY;
    যদি (!File_গ্ৰন্থি::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    যদি (type_name && type_name[0] == '\0')
        type_name = 0;

    FStructData s = getEmitter().startWriteStruct(write_স্তূপ.back(), চাবি, struct_flags, type_name);

    write_স্তূপ.push_back(s);
    size_t write_স্তূপ_size = write_স্তূপ.size();
    যদি (write_স্তূপ_size > 1)
        write_স্তূপ[write_স্তূপ_size - 2].flags &= ~File_গ্ৰন্থি::EMPTY;

    যদি (fmt != FileStorage::FORMAT_JSON && !File_গ্ৰন্থি::isFlow(s.flags))
        flush();

    যদি (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_গ্ৰন্থি::isMap(struct_flags)) {
        getEmitter().লিখ("type_id", type_name, মিছা);
    }
}

শূন্য FileStorage::Impl::startWriteStruct(ধ্রুৱ ৱর্ণ *চাবি, পূর্ণাঙ্ক struct_flags,
                                         ধ্রুৱ ৱর্ণ *type_name) {
    check_if_write_struct_is_delayed(মিছা);
    যদি (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    যদি (state_of_writing_base64 == FileStorage_API::Uncertain && File_গ্ৰন্থি::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(চাবি, struct_flags, type_name);
    } অন্যথা যদি (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        যদি ((File_গ্ৰন্থি::TYPE_MASK & struct_flags) != File_গ্ৰন্থি::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        অন্যথা যদি (state_of_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(চাবি, struct_flags, "binary");

        যদি (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::InUse);
    } অন্যথা {
        /* Won't output Base64 data */
        যদি (state_of_writing_base64 == FileStorage_API::InUse)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(চাবি, struct_flags, type_name);

        যদি (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::NotUse);
    }
}

শূন্য FileStorage::Impl::writeComment(ধ্রুৱ ৱর্ণ *comment, তার্কিক eol_comment) {
    CV_Assert(write_mode);
    getEmitter().writeComment(comment, eol_comment);
}

শূন্য FileStorage::Impl::startNextStream() {
    CV_Assert(write_mode);
    যদি (!empty_stream) {
        যেতিয়ালৈ (!write_স্তূপ.empty())
            endWriteStruct();
        flush();
        getEmitter().startNextStream();
        empty_stream = সঁচা;
        write_স্তূপ.push_back(FStructData("", File_গ্ৰন্থি::EMPTY, 0));
        bufofs = 0;
    }
}

শূন্য FileStorage::Impl::লিখ(ধ্রুৱ String &চাবি, পূর্ণাঙ্ক value) {
    CV_Assert(write_mode);
    getEmitter().লিখ(চাবি.c_str(), value);
}

শূন্য FileStorage::Impl::লিখ(ধ্রুৱ String &চাবি, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().লিখ(চাবি.c_str(), value);
}

শূন্য FileStorage::Impl::লিখ(ধ্রুৱ String &চাবি, দ্ৱিগুণ value) {
    CV_Assert(write_mode);
    getEmitter().লিখ(চাবি.c_str(), value);
}

শূন্য FileStorage::Impl::লিখ(ধ্রুৱ String &চাবি, ধ্রুৱ String &value) {
    CV_Assert(write_mode);
    getEmitter().লিখ(চাবি.c_str(), value.c_str(), মিছা);
}

শূন্য FileStorage::Impl::writeRawData(ধ্রুৱ std::string &dt, ধ্রুৱ শূন্য *_data, size_t len) {
    CV_Assert(write_mode);

    যদি (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        writeRawDataBase64(_data, len, dt.c_str());
        ঘূৰাই;
    } অন্যথা যদি (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elemSize = fs::calcStructSize(dt.c_str(), 0);
    CV_Assert(elemSize);
    CV_Assert(len % elemSize == 0);
    len /= elemSize;

    তার্কিক explicitZero = fmt == FileStorage::FORMAT_JSON;
    ধ্রুৱ uchar *data0 = (ধ্রুৱ uchar *) _data;
    পূর্ণাঙ্ক fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_গুনতি;
    ৱর্ণ buf[256] = "";

    fmt_pair_গুনতি = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    যদি (!len)
        ঘূৰাই;

    যদি (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    যদি (fmt_pair_গুনতি == 1) {
        fmt_pairs[0] *= (পূর্ণাঙ্ক) len;
        len = 1;
    }

    বাবে (; len--; data0 += elemSize) {
        পূর্ণাঙ্ক offset = 0;
        বাবে (k = 0; k < fmt_pair_গুনতি; k++) {
            পূর্ণাঙ্ক i, গুনতি_2 = fmt_pairs[k * 2];
            পূর্ণাঙ্ক elem_type = fmt_pairs[k * 2 + 1];
            পূর্ণাঙ্ক elem_size = CV_ELEM_SIZE(elem_type);
            ধ্রুৱ ৱর্ণ *ptr;

            offset = cvAlign(offset, elem_size);
            ধ্রুৱ uchar *data = data0 + offset;

            বাবে (i = 0; i < গুনতি_2; i++) {
                ৱিকল্প (elem_type) {
                    পক্ষ CV_8U:
                        ptr = fs::itoa(*(uchar *) data, buf, 10);
                        data++;
                        ভঙ্গ;
                    পক্ষ CV_Bool:
                        ptr = fs::itoa(*(uchar *) data != 0, buf, 10);
                        data++;
                        ভঙ্গ;
                    পক্ষ CV_8S:
                        ptr = fs::itoa(*(ৱর্ণ *) data, buf, 10);
                        data++;
                        ভঙ্গ;
                    পক্ষ CV_16U:
                        ptr = fs::itoa(*(ushort *) data, buf, 10);
                        data += পরিমাণ(ushort);
                        ভঙ্গ;
                    পক্ষ CV_16S:
                        ptr = fs::itoa(*(short *) data, buf, 10);
                        data += পরিমাণ(short);
                        ভঙ্গ;
                    পক্ষ CV_32U:
                        ptr = fs::itoa((int64_t)*(unsigned*) data, buf, 10, মিছা);
                        data += পরিমাণ(unsigned);
                        ভঙ্গ;
                    পক্ষ CV_32S:
                        ptr = fs::itoa(*(পূর্ণাঙ্ক *) data, buf, 10);
                        data += পরিমাণ(পূর্ণাঙ্ক);
                        ভঙ্গ;
                    পক্ষ CV_64U:
                        ptr = fs::itoa(*(uint64_t*) data, buf, 10, মিছা);
                        data += পরিমাণ(uint64_t);
                        ভঙ্গ;
                    পক্ষ CV_64S:
                        ptr = fs::itoa(*(int64_t*) data, buf, 10, সঁচা);
                        data += পরিমাণ(int64_t);
                        ভঙ্গ;
                    পক্ষ CV_32F:
                        ptr = fs::floatToString(buf, পরিমাণ(buf), *(ভিন্ন *) data, মিছা, explicitZero);
                        data += পরিমাণ(ভিন্ন);
                        ভঙ্গ;
                    পক্ষ CV_64F:
                        ptr = fs::doubleToString(buf, পরিমাণ(buf), *(দ্ৱিগুণ *) data, explicitZero);
                        data += পরিমাণ(দ্ৱিগুণ);
                        ভঙ্গ;
                    পক্ষ CV_16F:
                        ptr = fs::floatToString(buf, পরিমাণ(buf), (ভিন্ন) *(hfloat *) data, সঁচা, explicitZero);
                        data += পরিমাণ(hfloat);
                        ভঙ্গ;
                    পক্ষ CV_16BF:
                        ptr = fs::floatToString(buf, পরিমাণ(buf), (ভিন্ন) *(bfloat *) data, সঁচা, explicitZero);
                        data += পরিমাণ(bfloat);
                        ভঙ্গ;
                    পক্ষ CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, পরিমাণ(buf), (ভিন্ন) *(fp8_t *) data, সঁচা, explicitZero);
                        data += পরিমাণ(fp8_t);
                        ভঙ্গ;
                    পক্ষ CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, পরিমাণ(buf), (ভিন্ন) *(fp8a_t *) data, সঁচা, explicitZero);
                        data += পরিমাণ(fp8a_t);
                        ভঙ্গ;
                    স্ৱভাৱ:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        ঘূৰাই;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (পূর্ণাঙ্ক) (data - data0);
        }
    }
}

শূন্য FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(মিছা);

    যদি (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_to_Base64_state(FileStorage_API::Base64State::Uncertain);
}

শূন্য FileStorage::Impl::switch_to_Base64_state(FileStorage_API::Base64State new_state) {
    ধ্রুৱ ৱর্ণ *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    ধ্রুৱ ৱর্ণ *err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    ৱিকল্প (state_of_writing_base64) {
        পক্ষ FileStorage_API::Base64State::Uncertain:
            ৱিকল্প (new_state) {
                পক্ষ FileStorage_API::Base64State::InUse:
                {
                    CV_DbgAssert(base64_writer == 0);
                    তার্কিক can_indent = (fmt != cv::FileStorage::Mode::FORMAT_JSON);
                    base64_writer = নূতন base64::Base64Writer(*এতত্, can_indent);
                    যদি (!can_indent) {
                        ৱর্ণ *ptr = bufferPtr();
                        *ptr++ = '\0';
                        puts(bufferStart());
                        setBufferPtr(bufferStart());
                        memset(bufferStart(), 0, static_cast<পূর্ণাঙ্ক>(space));
                        puts("\"$base64$");
                    }
                    ভঙ্গ;
                }
                পক্ষ FileStorage_API::Base64State::Uncertain:
                    ভঙ্গ;
                পক্ষ FileStorage_API::Base64State::NotUse:
                    ভঙ্গ;
                স্ৱভাৱ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ভঙ্গ;
            }
            ভঙ্গ;
        পক্ষ FileStorage_API::Base64State::InUse:
            ৱিকল্প (new_state) {
                পক্ষ FileStorage_API::Base64State::InUse:
                পক্ষ FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    ভঙ্গ;
                পক্ষ FileStorage_API::Base64State::Uncertain:
                    delete base64_writer;
                    base64_writer = 0;
                    যদি ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        setBufferPtr(bufferStart());
                        flush();
                        memset(bufferStart(), 0, static_cast<পূর্ণাঙ্ক>(space) );
                        setBufferPtr(bufferStart());
                    }
                    ভঙ্গ;
                স্ৱভাৱ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ভঙ্গ;
            }
            ভঙ্গ;
        পক্ষ FileStorage_API::Base64State::NotUse:
            ৱিকল্প (new_state) {
                পক্ষ FileStorage_API::Base64State::InUse:
                পক্ষ FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    ভঙ্গ;
                পক্ষ FileStorage_API::Base64State::Uncertain:
                    ভঙ্গ;
                স্ৱভাৱ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ভঙ্গ;
            }
            ভঙ্গ;
        স্ৱভাৱ:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            ভঙ্গ;
    }

    state_of_writing_base64 = new_state;
}

শূন্য FileStorage::Impl::make_write_struct_delayed(ধ্রুৱ ৱর্ণ *চাবি, পূর্ণাঙ্ক struct_flags, ধ্রুৱ ৱর্ণ *type_name) {
    CV_Assert(is_write_struct_delayed == মিছা);
    CV_DbgAssert(delayed_struct_চাবি == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    যদি (চাবি != nullptr) {
        delayed_struct_চাবি = নূতন ৱর্ণ[strlen(চাবি) + 1U];
        strcpy(delayed_struct_চাবি, চাবি);
    }

    যদি (type_name != nullptr) {
        delayed_type_name = নূতন ৱর্ণ[strlen(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = সঁচা;
}

শূন্য FileStorage::Impl::check_if_write_struct_is_delayed(তার্কিক change_type_to_base64) {
    যদি (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_চাবি;
        std::string type_name;
        পূর্ণাঙ্ক struct_flags = delayed_struct_flags;

        যদি (delayed_struct_চাবি != nullptr && *delayed_struct_চাবি != '\0') {
            struct_চাবি.assign(delayed_struct_চাবি);
        }
        যদি (delayed_type_name != nullptr && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        delete[] delayed_struct_চাবি;
        delete[] delayed_type_name;
        delayed_struct_চাবি = nullptr;
        delayed_struct_flags = 0;
        delayed_type_name = nullptr;

        is_write_struct_delayed = মিছা;

        /* call */
        যদি (change_type_to_base64) {
            startWriteStruct_helper(struct_চাবি.c_str(), struct_flags, "binary");
            যদি (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::InUse);
        } অন্যথা {
            startWriteStruct_helper(struct_চাবি.c_str(), struct_flags, type_name.c_str());
            যদি (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::NotUse);
        }
    }
}

শূন্য FileStorage::Impl::writeRawDataBase64(ধ্রুৱ শূন্য *_data, size_t len, ধ্রুৱ ৱর্ণ *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(সঁচা);

    যদি (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } অন্যথা যদি (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->লিখ(_data, len, dt);
}

File_গ্ৰন্থি FileStorage::Impl::get_First_শীৰ্ষ_Level_গ্ৰন্থি() ধ্রুৱ {
    ঘূৰাই roots.empty() ? File_গ্ৰন্থি() : roots[0];
}

File_গ্ৰন্থি FileStorage::Impl::মূল(পূর্ণাঙ্ক streamIdx) ধ্রুৱ {
    ঘূৰাই streamIdx >= 0 && streamIdx < (পূর্ণাঙ্ক) roots.size() ? roots[streamIdx] : File_গ্ৰন্থি();
}

File_গ্ৰন্থি FileStorage::Impl::operator[](ধ্রুৱ String &nodename) ধ্রুৱ {
    ঘূৰাই এতত্->operator[](nodename.c_str());
}

File_গ্ৰন্থি FileStorage::Impl::operator[](ধ্রুৱ ৱর্ণ * /*nodename*/) ধ্রুৱ {
    ঘূৰাই File_গ্ৰন্থি();
}

পূর্ণাঙ্ক FileStorage::Impl::getFormat() ধ্রুৱ { ঘূৰাই fmt; }

ৱর্ণ *FileStorage::Impl::bufferPtr() ধ্রুৱ { ঘূৰাই (ৱর্ণ *) (&buffer[0] + bufofs); }

ৱর্ণ *FileStorage::Impl::bufferStart() ধ্রুৱ { ঘূৰাই (ৱর্ণ *) &buffer[0]; }

ৱর্ণ *FileStorage::Impl::bufferEnd() ধ্রুৱ { ঘূৰাই (ৱর্ণ *) (&buffer[0] + buffer.size()); }

শূন্য FileStorage::Impl::setBufferPtr(ৱর্ণ *ptr) {
    ৱর্ণ *bufferstart = bufferStart();
    CV_Assert(ptr >= bufferstart && ptr <= bufferEnd());
    bufofs = ptr - bufferstart;
}

পূর্ণাঙ্ক FileStorage::Impl::wrapMargin() ধ্রুৱ { ঘূৰাই wrap_margin; }

FStructData &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_স্তূপ.empty());
    ঘূৰাই write_স্তূপ.back();
}

শূন্য FileStorage::Impl::setNonEmpty() {
    empty_stream = মিছা;
}

শূন্য FileStorage::Impl::processSpecialDouble(ৱর্ণ *buf, দ্ৱিগুণ *value, ৱর্ণ **endptr) {
    FileStorage_API *fs = এতত্;
    ৱর্ণ c = buf[0];
    পূর্ণাঙ্ক inf_hi = 0x7ff00000;

    যদি (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    যদি (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    যদি (toupper(buf[1]) == 'I' && toupper(buf[2]) == 'N' && toupper(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    অন্যথা যদি (toupper(buf[1]) == 'N' && toupper(buf[2]) == 'A' && toupper(buf[3]) == 'N')
        v.u = (uint64) -1;
    অন্যথা
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

দ্ৱিগুণ FileStorage::Impl::strtod(ৱর্ণ *ptr, ৱর্ণ **endptr) {
    দ্ৱিগুণ fval = ::strtod(ptr, endptr);
    যদি (**endptr == '.') {
        ৱর্ণ *dot_pos = *endptr;
        *dot_pos = ',';
        দ্ৱিগুণ fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        যদি (*endptr > dot_pos)
            fval = fval2;
        অন্যথা
            *endptr = dot_pos;
    }

    যদি (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    ঘূৰাই fval;
}

শূন্য FileStorage::Impl::convertToCollection(পূর্ণাঙ্ক type, File_গ্ৰন্থি &গ্ৰন্থি) {
    CV_Assert(type == File_গ্ৰন্থি::SEQ || type == File_গ্ৰন্থি::MAP);

    পূর্ণাঙ্ক গ্ৰন্থি_type = গ্ৰন্থি.type();
    যদি (গ্ৰন্থি_type == type)
        ঘূৰাই;

    তার্কিক named = গ্ৰন্থি.isNamed();
    uchar *ptr = গ্ৰন্থি.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    দ্ৱিগুণ fval = 0;
    std::string sval;
    তার্কিক add_first_scalar = মিছা;

    যদি (গ্ৰন্থি_type != File_গ্ৰন্থি::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_গ্ৰন্থি::SEQ);
        যদি (গ্ৰন্থি_type == File_গ্ৰন্থি::INT) {
            ival = readLong(ptr);
            add_first_scalar = সঁচা;
        } অন্যথা যদি (গ্ৰন্থি_type == File_গ্ৰন্থি::REAL) {
            fval = readReal(ptr);
            add_first_scalar = সঁচা;
        } অন্যথা যদি (গ্ৰন্থি_type == File_গ্ৰন্থি::STRING) {
            sval = std::string(গ্ৰন্থি);
            add_first_scalar = সঁচা;
        } অন্যথা
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", গ্ৰন্থি_type));
    }

    ptr = reserve_গ্ৰন্থি_Space(গ্ৰন্থি, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_গ্ৰন্থি::NAMED : 0));
    // name has been copied automatically
    যদি (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    যদি (add_first_scalar)
        add_গ্ৰন্থি(গ্ৰন্থি, std::string(), গ্ৰন্থি_type,
                গ্ৰন্থি_type == File_গ্ৰন্থি::INT ? (ধ্রুৱ শূন্য *) &ival :
                গ্ৰন্থি_type == File_গ্ৰন্থি::REAL ? (ধ্রুৱ শূন্য *) &fval :
                গ্ৰন্থি_type == File_গ্ৰন্থি::STRING ? (ধ্রুৱ শূন্য *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_গ্ৰন্থি_Space(File_গ্ৰন্থি &গ্ৰন্থি, size_t sz) {
    তার্কিক shrinkBlock = মিছা;
    size_t shrinkBlockIdx = 0, shrinkSize = 0;

    uchar *ptr = 0, *blockEnd = 0;

    যদি (!fs_data_ptrs.empty()) {
        size_t blockIdx = গ্ৰন্থি.blockIdx;
        size_t ofs = গ্ৰন্থি.ofs;
        CV_Assert(blockIdx == fs_data_ptrs.size() - 1);
        CV_Assert(ofs <= fs_data_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_data_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_data_ptrs[blockIdx] + ofs;
        blockEnd = fs_data_ptrs[blockIdx] + fs_data_blksz[blockIdx];

        CV_Assert(ptr >= fs_data_ptrs[blockIdx] && ptr <= blockEnd);
        যদি (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            ঘূৰাই ptr;
        }

        যদি (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_data[blockIdx]->resize(sz);
            ptr = &fs_data[blockIdx]->at(0);
            fs_data_ptrs[blockIdx] = ptr;
            fs_data_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            ঘূৰাই ptr;
        }

        shrinkBlock = সঁচা;
        shrinkBlockIdx = blockIdx;
        shrinkSize = ofs;
    }

    size_t blockSize = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(blockSize);
    fs_data.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_data_ptrs.push_back(new_ptr);
    fs_data_blksz.push_back(blockSize);
    গ্ৰন্থি.blockIdx = fs_data_ptrs.size() - 1;
    গ্ৰন্থি.ofs = 0;
    freeSpaceOfs = sz;

    যদি (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        যদি (ptr[0] & File_গ্ৰন্থি::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    যদি (shrinkBlock) {
        fs_data[shrinkBlockIdx]->resize(shrinkSize);
        fs_data_blksz[shrinkBlockIdx] = shrinkSize;
    }

    ঘূৰাই new_ptr;
}

unsigned FileStorage::Impl::getStringOfs(ধ্রুৱ std::string &চাবি) ধ্রুৱ {
    str_hash_t::const_iterator it = str_hash.find(চাবি);
    ঘূৰাই it != str_hash.end() ? it->second : 0;
}

File_গ্ৰন্থি FileStorage::Impl::add_গ্ৰন্থি(File_গ্ৰন্থি &collection, ধ্রুৱ std::string &চাবি,
                                    পূর্ণাঙ্ক elem_type, ধ্রুৱ শূন্য *value, পূর্ণাঙ্ক len) {
    FileStorage_API *fs = এতত্;
    তার্কিক noname = চাবি.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(চাবি.c_str(), "_") == 0);
    convertToCollection(noname ? File_গ্ৰন্থি::SEQ : File_গ্ৰন্থি::MAP, collection);

    তার্কিক isseq = collection.empty() ? মিছা : collection.isSeq();
    যদি (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    unsigned strofs = 0;
    যদি (!noname) {
        strofs = getStringOfs(চাবি);
        যদি (!strofs) {
            strofs = (unsigned) str_hash_data.size();
            size_t keysize = চাবি.size() + 1;
            str_hash_data.resize(strofs + keysize);
            memcpy(&str_hash_data[0] + strofs, &চাবি[0], keysize);
            str_hash.insert(std::make_pair(চাবি, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t blockIdx = fs_data_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_গ্ৰন্থি গ্ৰন্থি(fs_ext, blockIdx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_গ্ৰন্থি_Space(গ্ৰন্থি, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_গ্ৰন্থি::NAMED));
    যদি (elem_type == File_গ্ৰন্থি::NONE)
        freeSpaceOfs -= 8;

    যদি (!noname) {
        writeInt(ptr, (পূর্ণাঙ্ক) strofs);
        ptr += 4;
    }

    যদি (elem_type == File_গ্ৰন্থি::SEQ || elem_type == File_গ্ৰন্থি::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    যদি (value)
        গ্ৰন্থি.set_মান(elem_type, value, len);

    যদি (collection.isNamed())
        cp += 4;
    পূর্ণাঙ্ক nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    ঘূৰাই গ্ৰন্থি;
}

শূন্য FileStorage::Impl::finalizeCollection(File_গ্ৰন্থি &collection) {
    যদি (!collection.isSeq() && !collection.isMap())
        ঘূৰাই;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    যদি (*ptr0 & File_গ্ৰন্থি::NAMED)
        ptr += 4;
    size_t blockIdx = collection.blockIdx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t rawSize = 4;
    unsigned sz = (unsigned) readInt(ptr + 4);
    যদি (sz > 0) {
        size_t lastBlockIdx = fs_data_ptrs.size() - 1;

        বাবে (; blockIdx < lastBlockIdx; blockIdx++) {
            rawSize += fs_data_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    rawSize += freeSpaceOfs - ofs;
    writeInt(ptr, (পূর্ণাঙ্ক) rawSize);
}

শূন্য FileStorage::Impl::normalize_গ্ৰন্থি_Ofs(size_t &blockIdx, size_t &ofs) ধ্রুৱ {
    যেতিয়ালৈ (ofs >= fs_data_blksz[blockIdx]) {
        যদি (blockIdx == fs_data_blksz.size() - 1) {
            CV_Assert(ofs == fs_data_blksz[blockIdx]);
            ভঙ্গ;
        }
        ofs -= fs_data_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    ঘূৰাই state_of_writing_base64;
}

পূর্ণাঙ্ক FileStorage::Impl::get_space() {
    ঘূৰাই space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = সঁচা;
}

শূন্য FileStorage::Impl::Base64Decoder::init(ধ্রুৱ Ptr<FileStorageParser> &_parser, ৱর্ণ *_ptr, পূর্ণাঙ্ক _indent) {
    parser_do_not_use_direct_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = মিছা;
}

তার্কিক FileStorage::Impl::Base64Decoder::readMore(পূর্ণাঙ্ক needed) {
    স্থির ধ্রুৱ uchar base64tab[] =
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

    যদি (eos)
        ঘূৰাই মিছা;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    বাবে (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    ৱর্ণ *beg = 0, *end = 0;
    তার্কিক ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    যদি (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = সঁচা;
        size_t tc = totalchars;
        বাবে (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    পূর্ণাঙ্ক i = 0, j, n = (পূর্ণাঙ্ক) encoded.size();
    যদি (n > 0) {
        ধ্রুৱ uchar *tab = base64tab;
        ৱর্ণ *src = &encoded[0];

        বাবে (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(পূর্ণাঙ্ক) (uchar) src[i]], c = tab[(পূর্ণাঙ্ক) (uchar) src[i + 1]];
            uchar b = tab[(পূর্ণাঙ্ক) (uchar) src[i + 2]], a = tab[(পূর্ণাঙ্ক) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    যদি (i > 0 && encoded[i - 1] == '=') {
        যদি (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        যদি (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    বাবে (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    ঘূৰাই (পূর্ণাঙ্ক) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    যদি (ofs >= sz && !readMore(1))
        ঘূৰাই (uchar) 0;
    ঘূৰাই decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    যদি (ofs + 2 > sz && !readMore(2))
        ঘূৰাই (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    ঘূৰাই val;
}

পূর্ণাঙ্ক FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    যদি (ofs + 4 > sz && !readMore(4))
        ঘূৰাই 0;
    পূর্ণাঙ্ক ival = readInt(&decoded[ofs]);
    ofs += 4;
    ঘূৰাই ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    যদি (ofs + 8 > sz && !readMore(8))
        ঘূৰাই 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    ঘূৰাই ival;
}

দ্ৱিগুণ FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    যদি (ofs + 8 > sz && !readMore(8))
        ঘূৰাই 0;
    দ্ৱিগুণ fval = readReal(&decoded[ofs]);
    ofs += 8;
    ঘূৰাই fval;
}

তার্কিক FileStorage::Impl::Base64Decoder::endOfStream() ধ্রুৱ { ঘূৰাই eos; }

ৱর্ণ *FileStorage::Impl::Base64Decoder::getPtr() ধ্রুৱ { ঘূৰাই ptr; }


ৱর্ণ *FileStorage::Impl::parseBase64(ৱর্ণ *ptr, পূর্ণাঙ্ক indent, File_গ্ৰন্থি &collection) {
    ধ্রুৱ পূর্ণাঙ্ক BASE64_HDR_SIZE = 24;
    ৱর্ণ dt[BASE64_HDR_SIZE + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    পূর্ণাঙ্ক i, k;

    বাবে (i = 0; i < BASE64_HDR_SIZE; i++)
        dt[i] = (ৱর্ণ) base64decoder.getUInt8();
    বাবে (i = 0; i < BASE64_HDR_SIZE; i++)
        যদি (isspace(dt[i]))
            ভঙ্গ;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    পূর্ণাঙ্ক fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    পূর্ণাঙ্ক fmt_pair_গুনতি = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    দ্ৱিগুণ fval = 0;

    বাবে (;;) {
        বাবে (k = 0; k < fmt_pair_গুনতি; k++) {
            পূর্ণাঙ্ক elem_type = fmt_pairs[k * 2 + 1];
            পূর্ণাঙ্ক গুনতি_2 = fmt_pairs[k * 2];

            বাবে (i = 0; i < গুনতি_2; i++) {
                পূর্ণাঙ্ক গ্ৰন্থি_type = File_গ্ৰন্থি::INT;
                ৱিকল্প (elem_type) {
                    পক্ষ CV_8U:
                        ival = base64decoder.getUInt8();
                        ভঙ্গ;
                    পক্ষ CV_8S:
                        ival = (ৱর্ণ) base64decoder.getUInt8();
                        ভঙ্গ;
                    পক্ষ CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        ভঙ্গ;
                    পক্ষ CV_16U:
                        ival = base64decoder.getUInt16();
                        ভঙ্গ;
                    পক্ষ CV_16S:
                        ival = (short) base64decoder.getUInt16();
                        ভঙ্গ;
                    পক্ষ CV_32U:
                        ival = base64decoder.getInt32();
                        ভঙ্গ;
                    পক্ষ CV_32S:
                        ival = base64decoder.getInt32();
                        ভঙ্গ;
                    পক্ষ CV_64U:
                        ival = base64decoder.getInt64();
                        ভঙ্গ;
                    পক্ষ CV_64S:
                        ival = base64decoder.getInt64();
                        ভঙ্গ;
                    পক্ষ CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        গ্ৰন্থি_type = File_গ্ৰন্থি::REAL;
                    }
                        ভঙ্গ;
                    পক্ষ CV_64F:
                        fval = base64decoder.getFloat64();
                        গ্ৰন্থি_type = File_গ্ৰন্থি::REAL;
                        ভঙ্গ;
                    পক্ষ CV_16F:
                        fval = ভিন্ন(hfloatFromBits(base64decoder.getUInt16()));
                        গ্ৰন্থি_type = File_গ্ৰন্থি::REAL;
                        ভঙ্গ;
                    পক্ষ CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        গ্ৰন্থি_type = File_গ্ৰন্থি::REAL;
                        ভঙ্গ;
                    পক্ষ CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        গ্ৰন্থি_type = File_গ্ৰন্থি::REAL;
                        ভঙ্গ;
                    স্ৱভাৱ:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                যদি (base64decoder.endOfStream())
                    ভঙ্গ;
                add_গ্ৰন্থি(collection, std::string(), গ্ৰন্থি_type,
                        গ্ৰন্থি_type == File_গ্ৰন্থি::INT ? (শূন্য *) &ival : (শূন্য *) &fval, -1);
            }
        }
        যদি (base64decoder.endOfStream())
            ভঙ্গ;
    }

    finalizeCollection(collection);
    ঘূৰাই base64decoder.getPtr();
}

শূন্য FileStorage::Impl::parseError(ধ্রুৱ ৱর্ণ *func_name, ধ্রুৱ std::string &err_বাৰ্তা, ধ্রুৱ ৱর্ণ *source_file,
                                   পূর্ণাঙ্ক source_সাৰি) {
    std::string বাৰ্তা = format("%s(%d): %s", filename.c_str(), lineno, err_বাৰ্তা.c_str());
    error(Error::StsParseError, func_name, বাৰ্তা.c_str(), source_file, source_সাৰি);
}

ধ্রুৱ uchar *FileStorage::Impl::get_গ্ৰন্থি_Ptr(size_t blockIdx, size_t ofs) ধ্রুৱ {
    CV_Assert(blockIdx < fs_data_ptrs.size());
    CV_Assert(ofs < fs_data_blksz[blockIdx]);

    ঘূৰাই fs_data_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) ধ্রুৱ {
    CV_Assert(nameofs < str_hash_data.size());
    ঘূৰাই std::string(&str_hash_data[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { ঘূৰাই fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(এতত্);
}

FileStorage::FileStorage(ধ্রুৱ String& filename, পূর্ণাঙ্ক flags, ধ্রুৱ String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(এতত্);
    তার্কিক ok = p->open(filename.c_str(), flags, encoding.c_str());
    যদি(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

শূন্য FileStorage::startWriteStruct(ধ্রুৱ String& name, পূর্ণাঙ্ক struct_flags, ধ্রুৱ String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    যদি ((struct_flags & File_গ্ৰন্থি::TYPE_MASK) == File_গ্ৰন্থি::SEQ)
        state = FileStorage::মান_EXPECTED;
    অন্যথা
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

শূন্য FileStorage::endWriteStruct()
{
    p->endWriteStruct();
    state = p->write_স্তূপ.empty() || File_গ্ৰন্থি::isMap(p->write_স্তূপ.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::মান_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

তার্কিক FileStorage::open(ধ্রুৱ String& filename, পূর্ণাঙ্ক flags, ধ্রুৱ String& encoding)
{
    যত্ন
    {
        তার্কিক ok = p->open(filename.c_str(), flags, encoding.c_str());
        যদি(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        ঘূৰাই ok;
    }
    গৃহাণ (...)
    {
        release();
        ক্ষিপ;  // re-throw
    }
}

তার্কিক FileStorage::isOpened() ধ্রুৱ { ঘূৰাই p->is_opened; }

শূন্য FileStorage::release()
{
    p->release();
}

File_গ্ৰন্থি FileStorage::মূল(পূর্ণাঙ্ক i) ধ্রুৱ
{
    যদি( p.empty() || p->roots.empty() || i < 0 || i >= (পূর্ণাঙ্ক)p->roots.size() )
        ঘূৰাই File_গ্ৰন্থি();

    ঘূৰাই p->roots[i];
}

File_গ্ৰন্থি FileStorage::get_First_শীৰ্ষ_Level_গ্ৰন্থি() ধ্রুৱ
{
    File_গ্ৰন্থি r = মূল();
    File_গ্ৰন্থি_Iterator it = r.begin();
    ঘূৰাই it != r.end() ? *it : File_গ্ৰন্থি();
}

std::string FileStorage::getDefaultObjectName(ধ্রুৱ std::string& _filename)
{
    স্থির ধ্রুৱ ৱর্ণ* stubname = "unnamed";
    ধ্রুৱ ৱর্ণ* filename = _filename.c_str();
    ধ্রুৱ ৱর্ণ* ptr2 = filename + _filename.size();
    ধ্রুৱ ৱর্ণ* ptr = ptr2 - 1;
    cv::AutoBuffer<ৱর্ণ> name_buf(_filename.size()+1);

    যেতিয়ালৈ( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        যদি( *ptr == '.' && (!*ptr2 || strncmp(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    যদি( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    ৱর্ণ* name = name_buf.data();

    // name must start with letter or '_'
    যদি( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    যেতিয়ালৈ( ptr < ptr2 )
    {
        ৱর্ণ c = *ptr++;
        যদি( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.data();
    যদি( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    ঘূৰাই name;
}


পূর্ণাঙ্ক FileStorage::getFormat() ধ্রুৱ
{
    ঘূৰাই p->fmt;
}

File_গ্ৰন্থি FileStorage::operator [](ধ্রুৱ ৱর্ণ* চাবি) ধ্রুৱ
{
    ঘূৰাই এতত্->operator[](std::string(চাবি));
}

File_গ্ৰন্থি FileStorage::operator [](ধ্রুৱ std::string& চাবি) ধ্রুৱ
{
    File_গ্ৰন্থি res;
    বাবে (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][চাবি];
        যদি (!res.empty())
            ভঙ্গ;
    }
    ঘূৰাই res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    ঘূৰাই buf;
}

শূন্য FileStorage::writeRaw( ধ্রুৱ String& fmt, ধ্রুৱ শূন্য* vec, size_t len )
{
    p->writeRawData(fmt, (ধ্রুৱ uchar*)vec, len);
}

শূন্য FileStorage::writeComment( ধ্রুৱ String& comment, তার্কিক eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

শূন্য writeScalar( FileStorage& fs, পূর্ণাঙ্ক value )
{
    fs.p->লিখ(String(), value);
}

শূন্য writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->লিখ(String(), value);
}

শূন্য writeScalar( FileStorage& fs, ভিন্ন value )
{
    fs.p->লিখ(String(), (দ্ৱিগুণ)value);
}

শূন্য writeScalar( FileStorage& fs, দ্ৱিগুণ value )
{
    fs.p->লিখ(String(), value);
}

শূন্য writeScalar( FileStorage& fs, ধ্রুৱ String& value )
{
    fs.p->লিখ(String(), value);
}

শূন্য লিখ( FileStorage& fs, ধ্রুৱ String& name, পূর্ণাঙ্ক value )
{
    fs.p->লিখ(name, value);
}

শূন্য লিখ( FileStorage& fs, ধ্রুৱ String& name, int64_t value )
{
    fs.p->লিখ(name, value);
}

শূন্য লিখ( FileStorage& fs, ধ্রুৱ String& name, ভিন্ন value )
{
    fs.p->লিখ(name, (দ্ৱিগুণ)value);
}

শূন্য লিখ( FileStorage& fs, ধ্রুৱ String& name, দ্ৱিগুণ value )
{
    fs.p->লিখ(name, value);
}

শূন্য লিখ( FileStorage& fs, ধ্রুৱ String& name, ধ্রুৱ String& value )
{
    fs.p->লিখ(name, value);
}

শূন্য লিখ( FileStorage& fs, ধ্রুৱ String& name, তার্কিক value )
{
    fs.p->লিখ(name, value);
}

শূন্য FileStorage::লিখ(ধ্রুৱ String& name, পূর্ণাঙ্ক val) { p->লিখ(name, val); }
শূন্য FileStorage::লিখ(ধ্রুৱ String& name, তার্কিক val) { p->লিখ(name, val); }
শূন্য FileStorage::লিখ(ধ্রুৱ String& name, int64_t val) { p->লিখ(name, val); }
শূন্য FileStorage::লিখ(ধ্রুৱ String& name, দ্ৱিগুণ val) { p->লিখ(name, val); }
শূন্য FileStorage::লিখ(ধ্রুৱ String& name, ধ্রুৱ String& val) { p->লিখ(name, val); }
শূন্য FileStorage::লিখ(ধ্রুৱ String& name, ধ্রুৱ Mat& val) { cv::লিখ(*এতত্, name, val); }
শূন্য FileStorage::লিখ(ধ্রুৱ String& name, ধ্রুৱ std::vector<String>& val) { cv::লিখ(*এতত্, name, val); }

FileStorage& operator << (FileStorage& fs, ধ্রুৱ String& str)
{
    গণনা { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        মান_EXPECTED = FileStorage::মান_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    ধ্রুৱ ৱর্ণ* _str = str.c_str();
    যদি( !fs.isOpened() || !_str )
        ঘূৰাই fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    ৱর্ণ c = *_str;

    যদি( c == '}' || c == ']' )
    {
        যদি( fs_impl->write_স্তূপ.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        পূর্ণাঙ্ক struct_flags = fs_impl->write_স্তূপ.back().flags;
        ৱর্ণ expected_bracket = File_গ্ৰন্থি::isMap(struct_flags) ? '}' : ']';
        যদি( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_স্তূপ.empty());
        struct_flags = fs_impl->write_স্তূপ.back().flags;
        fs.state = File_গ্ৰন্থি::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : মান_EXPECTED;
        fs.elname = String();
    }
    অন্যথা যদি( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        যদি (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = মান_EXPECTED + INSIDE_MAP;
    }
    অন্যথা যদি( (fs.state & 3) == মান_EXPECTED )
    {
        যদি( c == '{' || c == '[' )
        {
            পূর্ণাঙ্ক struct_flags = c == '{' ? File_গ্ৰন্থি::MAP : File_গ্ৰন্থি::SEQ;
            fs.state = struct_flags == File_গ্ৰন্থি::MAP ? INSIDE_MAP + NAME_EXPECTED : মান_EXPECTED;
            _str++;
            যদি( *_str == ':' )
            {
                _str++;
                যদি( !*_str )
                    struct_flags |= File_গ্ৰন্থি::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        অন্যথা
        {
            লিখ( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            যদি( fs.state == INSIDE_MAP + মান_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    অন্যথা
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    ঘূৰাই fs;
}


File_গ্ৰন্থি::File_গ্ৰন্থি()
    : fs(রিক্ত)
{
    blockIdx = ofs = 0;
}

File_গ্ৰন্থি::File_গ্ৰন্থি(FileStorage::Impl* _fs, size_t _blockIdx, size_t _ofs)
    : fs(_fs)
{
    blockIdx = _blockIdx;
    ofs = _ofs;
}

File_গ্ৰন্থি::File_গ্ৰন্থি(ধ্রুৱ FileStorage* _fs, size_t _blockIdx, size_t _ofs)
    : File_গ্ৰন্থি(_fs->p.get(), _blockIdx, _ofs)
{
    // nothing
}

File_গ্ৰন্থি::File_গ্ৰন্থি(ধ্রুৱ File_গ্ৰন্থি& গ্ৰন্থি)
{
    fs = গ্ৰন্থি.fs;
    blockIdx = গ্ৰন্থি.blockIdx;
    ofs = গ্ৰন্থি.ofs;
}

File_গ্ৰন্থি& File_গ্ৰন্থি::operator=(ধ্রুৱ File_গ্ৰন্থি& গ্ৰন্থি)
{
    fs = গ্ৰন্থি.fs;
    blockIdx = গ্ৰন্থি.blockIdx;
    ofs = গ্ৰন্থি.ofs;
    ঘূৰাই *এতত্;
}

File_গ্ৰন্থি File_গ্ৰন্থি::operator[](ধ্রুৱ std::string& nodename) ধ্রুৱ
{
    যদি(!fs)
        ঘূৰাই File_গ্ৰন্থি();

    CV_Assert( isMap() );

    unsigned চাবি = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_গ্ৰন্থি_Iterator it = begin();

    বাবে( i = 0; i < sz; i++, ++it )
    {
        File_গ্ৰন্থি n = *it;
        ধ্রুৱ uchar* p = n.ptr();
        unsigned key2 = (unsigned)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_data.size() );
        যদি( চাবি == key2 )
            ঘূৰাই n;
    }
    ঘূৰাই File_গ্ৰন্থি();
}

File_গ্ৰন্থি File_গ্ৰন্থি::operator[](ধ্রুৱ ৱর্ণ* nodename) ধ্রুৱ
{
    ঘূৰাই এতত্->operator[](std::string(nodename));
}

File_গ্ৰন্থি File_গ্ৰন্থি::operator[](পূর্ণাঙ্ক i) ধ্রুৱ
{
    যদি(!fs)
        ঘূৰাই File_গ্ৰন্থি();

    CV_Assert( isSeq() );

    পূর্ণাঙ্ক sz = (পূর্ণাঙ্ক)size();
    CV_Assert( 0 <= i && i < sz );

    File_গ্ৰন্থি_Iterator it = begin();
    it += i;

    ঘূৰাই *it;
}

std::vector<String> File_গ্ৰন্থি::চাবিগুলি() ধ্রুৱ
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    বাবে (File_গ্ৰন্থি_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    ঘূৰাই res;
}

পূর্ণাঙ্ক File_গ্ৰন্থি::type() ধ্রুৱ
{
    ধ্রুৱ uchar* p = ptr();
    যদি(!p)
        ঘূৰাই NONE;
    ঘূৰাই (*p & TYPE_MASK);
}

তার্কিক File_গ্ৰন্থি::isMap(পূর্ণাঙ্ক flags) { ঘূৰাই (flags & TYPE_MASK) == MAP; }
তার্কিক File_গ্ৰন্থি::isSeq(পূর্ণাঙ্ক flags) { ঘূৰাই (flags & TYPE_MASK) == SEQ; }
তার্কিক File_গ্ৰন্থি::isCollection(পূর্ণাঙ্ক flags) { ঘূৰাই isMap(flags) || isSeq(flags); }
তার্কিক File_গ্ৰন্থি::isFlow(পূর্ণাঙ্ক flags) { ঘূৰাই (flags & FLOW) != 0; }
তার্কিক File_গ্ৰন্থি::isEmptyCollection(পূর্ণাঙ্ক flags) { ঘূৰাই (flags & EMPTY) != 0; }

তার্কিক File_গ্ৰন্থি::empty() ধ্রুৱ   { ঘূৰাই fs == 0; }
তার্কিক File_গ্ৰন্থি::isNone() ধ্রুৱ  { ঘূৰাই type() == NONE; }
তার্কিক File_গ্ৰন্থি::isSeq() ধ্রুৱ   { ঘূৰাই type() == SEQ; }
তার্কিক File_গ্ৰন্থি::isMap() ধ্রুৱ   { ঘূৰাই type() == MAP; }
তার্কিক File_গ্ৰন্থি::isInt() ধ্রুৱ   { ঘূৰাই type() == INT;  }
তার্কিক File_গ্ৰন্থি::isReal() ধ্রুৱ  { ঘূৰাই type() == REAL; }
তার্কিক File_গ্ৰন্থি::isString() ধ্রুৱ { ঘূৰাই type() == STRING;  }
তার্কিক File_গ্ৰন্থি::isNamed() ধ্রুৱ
{
    ধ্রুৱ uchar* p = ptr();
    যদি(!p)
        ঘূৰাই মিছা;
    ঘূৰাই (*p & NAMED) != 0;
}

std::string File_গ্ৰন্থি::name() ধ্রুৱ
{
    ধ্রুৱ uchar* p = ptr();
    যদি(!p)
        ঘূৰাই std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    ঘূৰাই fs->getName(nameofs);
}

File_গ্ৰন্থি::operator পূর্ণাঙ্ক() ধ্রুৱ
{
    ধ্রুৱ uchar* p = ptr();
    যদি(!p)
        ঘূৰাই 0;
    পূর্ণাঙ্ক tag = *p;
    পূর্ণাঙ্ক type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    যদি( type == INT )
    {
        ঘূৰাই readInt(p);
    }
    অন্যথা যদি( type == REAL )
    {
        ঘূৰাই cvRound(readReal(p));
    }
    অন্যথা
        ঘূৰাই 0x7fffffff;
}

File_গ্ৰন্থি::operator int64_t() ধ্রুৱ
{
    ধ্রুৱ uchar* p = ptr();
    যদি(!p)
        ঘূৰাই 0;
    পূর্ণাঙ্ক tag = *p;
    পূর্ণাঙ্ক type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    যদি( type == INT )
    {
        ঘূৰাই readLong(p);
    }
    অন্যথা যদি( type == REAL )
    {
        ঘূৰাই cvRound(readReal(p));
    }
    অন্যথা
        ঘূৰাই 0x7fffffff;
}

File_গ্ৰন্থি::operator ভিন্ন() ধ্রুৱ
{
    ধ্রুৱ uchar* p = ptr();
    যদি(!p)
        ঘূৰাই 0.f;
    পূর্ণাঙ্ক tag = *p;
    পূর্ণাঙ্ক type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    যদি( type == INT )
    {
        ঘূৰাই (ভিন্ন)readLong(p);
    }
    অন্যথা যদি( type == REAL )
    {
        ঘূৰাই (ভিন্ন)readReal(p);
    }
    অন্যথা
        ঘূৰাই FLT_MAX;
}

File_গ্ৰন্থি::operator দ্ৱিগুণ() ধ্রুৱ
{
    ধ্রুৱ uchar* p = ptr();
    যদি(!p)
        ঘূৰাই 0.f;
    পূর্ণাঙ্ক tag = *p;
    পূর্ণাঙ্ক type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    যদি( type == INT )
    {
        ঘূৰাই (দ্ৱিগুণ)readLong(p);
    }
    অন্যথা যদি( type == REAL )
    {
        ঘূৰাই readReal(p);
    }
    অন্যথা
        ঘূৰাই DBL_MAX;
}

দ্ৱিগুণ File_গ্ৰন্থি::real() ধ্রুৱ  { ঘূৰাই দ্ৱিগুণ(*এতত্); }

std::string File_গ্ৰন্থি::string() ধ্রুৱ
{
    ধ্রুৱ uchar* p = ptr();
    যদি( !p || (*p & TYPE_MASK) != STRING )
        ঘূৰাই std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(unsigned)readInt(p);
    ঘূৰাই std::string((ধ্রুৱ ৱর্ণ*)(p + 4), sz - 1);
}
Mat File_গ্ৰন্থি::mat() ধ্রুৱ { Mat value; পঠ(*এতত্, value, Mat()); ঘূৰাই value; }

File_গ্ৰন্থি_Iterator File_গ্ৰন্থি::begin() ধ্রুৱ { ঘূৰাই File_গ্ৰন্থি_Iterator(*এতত্, মিছা); }
File_গ্ৰন্থি_Iterator File_গ্ৰন্থি::end() ধ্রুৱ   { ঘূৰাই File_গ্ৰন্থি_Iterator(*এতত্, সঁচা); }

শূন্য File_গ্ৰন্থি::readRaw( ধ্রুৱ std::string& fmt, শূন্য* vec, size_t len ) ধ্রুৱ
{
    File_গ্ৰন্থি_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_গ্ৰন্থি::size() ধ্রুৱ
{
    ধ্রুৱ uchar* p = ptr();
    যদি( !p )
        ঘূৰাই 0;
    পূর্ণাঙ্ক tag = *p;
    পূর্ণাঙ্ক tp = tag & TYPE_MASK;
    যদি( tp == MAP || tp == SEQ )
    {
        যদি( tag & NAMED )
            p += 4;
        ঘূৰাই (size_t)(unsigned)readInt(p + 5);
    }
    ঘূৰাই tp != NONE;
}

size_t File_গ্ৰন্থি::rawSize() ধ্রুৱ
{
    ধ্রুৱ uchar* p0 = ptr(), *p = p0;
    যদি( !p )
        ঘূৰাই 0;
    পূর্ণাঙ্ক tag = *p++;
    পূর্ণাঙ্ক tp = tag & TYPE_MASK;
    যদি( tag & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    যদি( tp == INT )
        ঘূৰাই sz0 + 8;
    যদি( tp == REAL )
        ঘূৰাই sz0 + 8;
    যদি( tp == NONE )
        ঘূৰাই sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    ঘূৰাই sz0 + 4 + readInt(p);
}

uchar* File_গ্ৰন্থি::ptr()
{
    ঘূৰাই !fs ? 0 : (uchar*)fs->get_গ্ৰন্থি_Ptr(blockIdx, ofs);
}

ধ্রুৱ uchar* File_গ্ৰন্থি::ptr() ধ্রুৱ
{
    ঘূৰাই !fs ? 0 : fs->get_গ্ৰন্থি_Ptr(blockIdx, ofs);
}

শূন্য File_গ্ৰন্থি::set_মান( পূর্ণাঙ্ক type, ধ্রুৱ শূন্য* value, পূর্ণাঙ্ক len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    পূর্ণাঙ্ক tag = *p;
    পূর্ণাঙ্ক current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    পূর্ণাঙ্ক sz = 1;

    যদি( tag & NAMED )
        sz += 4;

    যদি( type == INT )
        sz += 8;
    অন্যথা যদি( type == REAL )
        sz += 8;
    অন্যথা যদি( type == STRING )
    {
        যদি( len < 0 )
            len = (পূর্ণাঙ্ক)strlen((ধ্রুৱ ৱর্ণ*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    অন্যথা
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_গ্ৰন্থি_Space(*এতত্, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    যদি( tag & NAMED )
        p += 4;

    যদি( type == INT )
    {
        int64_t ival = *(ধ্রুৱ int64_t*)value;
        writeInt(p, ival);
    }
    অন্যথা যদি( type == REAL )
    {
        দ্ৱিগুণ dbval = *(ধ্রুৱ দ্ৱিগুণ*)value;
        writeReal(p, dbval);
    }
    অন্যথা যদি( type == STRING )
    {
        ধ্রুৱ ৱর্ণ* str = (ধ্রুৱ ৱর্ণ*)value;
        writeInt(p, len + 1);
        memcpy(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_গ্ৰন্থি_Iterator::File_গ্ৰন্থি_Iterator()
{
    fs = 0;
    blockIdx = 0;
    ofs = 0;
    blockSize = 0;
    গ্ৰন্থি_NElems = 0;
    idx = 0;
}

File_গ্ৰন্থি_Iterator::File_গ্ৰন্থি_Iterator( ধ্রুৱ File_গ্ৰন্থি& গ্ৰন্থি, তার্কিক seekEnd )
{
    fs = গ্ৰন্থি.fs;
    idx = 0;
    যদি( !fs )
        blockIdx = ofs = blockSize = গ্ৰন্থি_NElems = 0;
    অন্যথা
    {
        blockIdx = গ্ৰন্থি.blockIdx;
        ofs = গ্ৰন্থি.ofs;

        তার্কিক collection = গ্ৰন্থি.isSeq() || গ্ৰন্থি.isMap();
        যদি( গ্ৰন্থি.isNone() )
        {
            গ্ৰন্থি_NElems = 0;
        }
        অন্যথা যদি( !collection )
        {
            গ্ৰন্থি_NElems = 1;
            যদি( seekEnd )
            {
                idx = 1;
                ofs += গ্ৰন্থি.rawSize();
            }
        }
        অন্যথা
        {
            গ্ৰন্থি_NElems = গ্ৰন্থি.size();
            ধ্রুৱ uchar* p0 = গ্ৰন্থি.ptr(), *p = p0 + 1;
            যদি(*p0 & File_গ্ৰন্থি::NAMED )
                p += 4;
            যদি( !seekEnd )
                ofs += (p - p0) + 8;
            অন্যথা
            {
                size_t rawsz = (size_t)(unsigned)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = গ্ৰন্থি_NElems;
            }
        }
        fs->normalize_গ্ৰন্থি_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
}

File_গ্ৰন্থি_Iterator::File_গ্ৰন্থি_Iterator(ধ্রুৱ File_গ্ৰন্থি_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    গ্ৰন্থি_NElems = it.গ্ৰন্থি_NElems;
    idx = it.idx;
}

File_গ্ৰন্থি_Iterator& File_গ্ৰন্থি_Iterator::operator=(ধ্রুৱ File_গ্ৰন্থি_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    গ্ৰন্থি_NElems = it.গ্ৰন্থি_NElems;
    idx = it.idx;
    ঘূৰাই *এতত্;
}

File_গ্ৰন্থি File_গ্ৰন্থি_Iterator::operator *() ধ্রুৱ
{
    ঘূৰাই File_গ্ৰন্থি(idx < গ্ৰন্থি_NElems ? fs : রিক্ত, blockIdx, ofs);
}

File_গ্ৰন্থি_Iterator& File_গ্ৰন্থি_Iterator::operator ++ ()
{
    যদি( idx == গ্ৰন্থি_NElems || !fs )
        ঘূৰাই *এতত্;
    idx++;
    File_গ্ৰন্থি n(fs, blockIdx, ofs);
    ofs += n.rawSize();
    যদি( ofs >= blockSize )
    {
        fs->normalize_গ্ৰন্থি_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
    ঘূৰাই *এতত্;
}

File_গ্ৰন্থি_Iterator File_গ্ৰন্থি_Iterator::operator ++ (পূর্ণাঙ্ক)
{
    File_গ্ৰন্থি_Iterator it = *এতত্;
    ++(*এতত্);
    ঘূৰাই it;
}

File_গ্ৰন্থি_Iterator& File_গ্ৰন্থি_Iterator::operator += (পূর্ণাঙ্ক _ofs)
{
    CV_Assert( _ofs >= 0 );
    বাবে( ; _ofs > 0; _ofs-- )
        এতত্->operator ++();
    ঘূৰাই *এতত্;
}

File_গ্ৰন্থি_Iterator& File_গ্ৰন্থি_Iterator::readRaw( ধ্রুৱ String& fmt, শূন্য* _data0, size_t maxsz)
{
    যদি( fs && idx < গ্ৰন্থি_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        পূর্ণাঙ্ক fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        পূর্ণাঙ্ক fmt_pair_গুনতি = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        size_t esz = fs::calcStructSize( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        বাবে( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            বাবে( পূর্ণাঙ্ক k = 0; k < fmt_pair_গুনতি; k++ )
            {
                পূর্ণাঙ্ক elem_type = fmt_pairs[k*2+1];
                পূর্ণাঙ্ক elem_size = CV_ELEM_SIZE(elem_type);

                পূর্ণাঙ্ক গুনতি_2 = fmt_pairs[k*2];
                offset = alignSize( offset, elem_size );
                uchar* data = data0 + offset;

                বাবে( পূর্ণাঙ্ক i = 0; i < গুনতি_2; i++, ++(*এতত্) )
                {
                    File_গ্ৰন্থি গ্ৰন্থি = *(*এতত্);
                    যদি( গ্ৰন্থি.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_size == 8 ? (int64_t)গ্ৰন্থি : (পূর্ণাঙ্ক)গ্ৰন্থি);
                        ৱিকল্প( elem_type )
                        {
                        পক্ষ CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(ival);
                            data++;
                            ভঙ্গ;
                        পক্ষ CV_8S:
                            *(ৱর্ণ*)data = saturate_cast<schar>(ival);
                            data++;
                            ভঙ্গ;
                        পক্ষ CV_Bool:
                            *(তার্কিক*)data = ival != 0;
                            data++;
                            ভঙ্গ;
                        পক্ষ CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(ival);
                            data += পরিমাণ(ushort);
                            ভঙ্গ;
                        পক্ষ CV_16S:
                            *(short*)data = saturate_cast<short>(ival);
                            data += পরিমাণ(short);
                            ভঙ্গ;
                        পক্ষ CV_32U:
                            *(unsigned*)data = (unsigned)std::max(ival, (int64_t)0);
                            data += পরিমাণ(unsigned);
                            ভঙ্গ;
                        পক্ষ CV_32S:
                            *(পূর্ণাঙ্ক*)data = (পূর্ণাঙ্ক)ival;
                            data += পরিমাণ(পূর্ণাঙ্ক);
                            ভঙ্গ;
                        পক্ষ CV_32F:
                            *(ভিন্ন*)data = (ভিন্ন)ival;
                            data += পরিমাণ(ভিন্ন);
                            ভঙ্গ;
                        পক্ষ CV_64U:
                            *(uint64_t*)data = (uint64_t)ival;
                            data += পরিমাণ(uint64_t);
                            ভঙ্গ;
                        পক্ষ CV_64S:
                            *(int64_t*)data = (int64_t)ival;
                            data += পরিমাণ(int64_t);
                            ভঙ্গ;
                        পক্ষ CV_64F:
                            *(দ্ৱিগুণ*)data = (দ্ৱিগুণ)ival;
                            data += পরিমাণ(দ্ৱিগুণ);
                            ভঙ্গ;
                        পক্ষ CV_16F:
                            *(hfloat*)data = hfloat((ভিন্ন)ival);
                            data += পরিমাণ(hfloat);
                            ভঙ্গ;
                        পক্ষ CV_16BF:
                            *(bfloat*)data = bfloat((ভিন্ন)ival);
                            data += পরিমাণ(bfloat);
                            ভঙ্গ;
                        পক্ষ CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((ভিন্ন)ival);
                            data += পরিমাণ(fp8_t);
                            ভঙ্গ;
                        পক্ষ CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((ভিন্ন)ival);
                            data += পরিমাণ(fp8a_t);
                            ভঙ্গ;
                        স্ৱভাৱ:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    অন্যথা যদি( গ্ৰন্থি.isReal() )
                    {
                        দ্ৱিগুণ fval = (দ্ৱিগুণ)গ্ৰন্থি;

                        ৱিকল্প( elem_type )
                        {
                        পক্ষ CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(fval);
                            data++;
                            ভঙ্গ;
                        পক্ষ CV_8S:
                            *(ৱর্ণ*)data = saturate_cast<schar>(fval);
                            data++;
                            ভঙ্গ;
                        পক্ষ CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(fval);
                            data += পরিমাণ(ushort);
                            ভঙ্গ;
                        পক্ষ CV_16S:
                            *(short*)data = saturate_cast<short>(fval);
                            data += পরিমাণ(short);
                            ভঙ্গ;
                        পক্ষ CV_32U:
                            *(পূর্ণাঙ্ক*)data = saturate_cast<unsigned>(fval);
                            data += পরিমাণ(পূর্ণাঙ্ক);
                            ভঙ্গ;
                        পক্ষ CV_32S:
                            *(পূর্ণাঙ্ক*)data = saturate_cast<পূর্ণাঙ্ক>(fval);
                            data += পরিমাণ(পূর্ণাঙ্ক);
                            ভঙ্গ;
                        পক্ষ CV_32F:
                            *(ভিন্ন*)data = (ভিন্ন)fval;
                            data += পরিমাণ(ভিন্ন);
                            ভঙ্গ;
                        পক্ষ CV_64U:
                            *(uint64_t*)data = (uint64_t)round(std::max(fval, 0.));
                            data += পরিমাণ(uint64_t);
                            ভঙ্গ;
                        পক্ষ CV_64S:
                            *(int64_t*)data = (int64_t)round(std::max(fval, 0.));
                            data += পরিমাণ(int64_t);
                            ভঙ্গ;
                        পক্ষ CV_64F:
                            *(দ্ৱিগুণ*)data = fval;
                            data += পরিমাণ(দ্ৱিগুণ);
                            ভঙ্গ;
                        পক্ষ CV_16F:
                            *(hfloat*)data = hfloat((ভিন্ন)fval);
                            data += পরিমাণ(hfloat);
                            ভঙ্গ;
                        পক্ষ CV_16BF:
                            *(bfloat*)data = bfloat((ভিন্ন)fval);
                            data += পরিমাণ(bfloat);
                            ভঙ্গ;
                        পক্ষ CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((ভিন্ন)fval);
                            data += পরিমাণ(fp8_t);
                            ভঙ্গ;
                        পক্ষ CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((ভিন্ন)fval);
                            data += পরিমাণ(fp8a_t);
                            ভঙ্গ;
                        স্ৱভাৱ:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    অন্যথা
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (পূর্ণাঙ্ক)(data - data0);
            }
        }
    }

    ঘূৰাই *এতত্;
}

তার্কিক File_গ্ৰন্থি_Iterator::equalTo(ধ্রুৱ File_গ্ৰন্থি_Iterator& it) ধ্রুৱ
{
    ঘূৰাই fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && গ্ৰন্থি_NElems == it.গ্ৰন্থি_NElems;
}

size_t File_গ্ৰন্থি_Iterator::remaining() ধ্রুৱ
{
    ঘূৰাই গ্ৰন্থি_NElems - idx;
}

তার্কিক operator == ( ধ্রুৱ File_গ্ৰন্থি_Iterator& it1, ধ্রুৱ File_গ্ৰন্থি_Iterator& it2 )
{
    ঘূৰাই it1.equalTo(it2);
}

তার্কিক operator != ( ধ্রুৱ File_গ্ৰন্থি_Iterator& it1, ধ্রুৱ File_গ্ৰন্থি_Iterator& it2 )
{
    ঘূৰাই !it1.equalTo(it2);
}

শূন্য পঠ(ধ্রুৱ File_গ্ৰন্থি& গ্ৰন্থি, পূর্ণাঙ্ক& val, পূর্ণাঙ্ক default_val)
{
    val = default_val;
    যদি( !গ্ৰন্থি.empty() )
    {
        val = (পূর্ণাঙ্ক)গ্ৰন্থি;
    }
}

শূন্য পঠ(ধ্রুৱ File_গ্ৰন্থি& গ্ৰন্থি, int64_t& val, int64_t default_val)
{
    val = default_val;
    যদি( !গ্ৰন্থি.empty() )
    {
        val = (int64_t)গ্ৰন্থি;
    }
}

শূন্য পঠ(ধ্রুৱ File_গ্ৰন্থি& গ্ৰন্থি, দ্ৱিগুণ& val, দ্ৱিগুণ default_val)
{
    val = default_val;
    যদি( !গ্ৰন্থি.empty() )
    {
        val = (দ্ৱিগুণ)গ্ৰন্থি;
    }
}

শূন্য পঠ(ধ্রুৱ File_গ্ৰন্থি& গ্ৰন্থি, ভিন্ন& val, ভিন্ন default_val)
{
    val = default_val;
    যদি( !গ্ৰন্থি.empty() )
    {
        val = (ভিন্ন)গ্ৰন্থি;
    }
}

শূন্য পঠ(ধ্রুৱ File_গ্ৰন্থি& গ্ৰন্থি, std::string& val, ধ্রুৱ std::string& default_val)
{
    val = default_val;
    যদি( !গ্ৰন্থি.empty() )
    {
        val = (std::string)গ্ৰন্থি;
    }
}
শূন্য FileStorage::Impl::লিখ(ধ্রুৱ String &চাবি, তার্কিক value)
{
    CV_Assert(write_mode);
    যদি (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().লিখ(চাবি.c_str(), (পূর্ণাঙ্ক)value);
    }
    অন্যথা
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().লিখ(চাবি.c_str(), value ? "true" : "false", মিছা);
    }
}

FileStorage_API::~FileStorage_API() {}

নামাকাশ internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, ধ্রুৱ std::string& name,
                                       পূর্ণাঙ্ক flags, ধ্রুৱ std::string& typeName)
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
