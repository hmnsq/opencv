// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "persistence.hpp"
#অন্তর্ভাব "persistence_impl.hpp"
#অন্তর্ভাব "persistence_base64_encoding.hpp"
#অন্তর্ভাব <unordered_map>
#অন্তর্ভাব <iterator>

#অন্তর্ভাব <opencv2/core/utils/logger.hpp>

#define CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

নামাকাশ cv
{

নামাকাশ fs
{

পূর্ণসংখ্যা strcasecmp(ধ্রুবক অক্ষর* s1, ধ্রুবক অক্ষর* s2)
{
    ধ্রুবক অক্ষর* dummy="";
    যদি(!s1) s1=dummy;
    যদি(!s2) s2=dummy;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t i, len = std::min(len1, len2);
    জন্য( i = 0; i < len; i++ )
    {
        পূর্ণসংখ্যা d = tolower((পূর্ণসংখ্যা)s1[i]) - tolower((পূর্ণসংখ্যা)s2[i]);
        যদি( d != 0 )
            ফেরত d;
    }
    ফেরত len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

অক্ষর* itoa( পূর্ণসংখ্যা _val, অক্ষর* বাফার, পূর্ণসংখ্যা /*radix*/ )
{
    ধ্রুবক পূর্ণসংখ্যা radix = 10;
    অক্ষর* ptr=বাফার + 23 /* enough even for 64-bit integers */;
    unsigned val = abs(_val);

    *ptr = '\0';
    করো
    {
        unsigned r = val / radix;
        *--ptr = (অক্ষর)(val - (r*radix) + '0');
        val = r;
    }
    যখন( val != 0 );

    যদি( _val < 0 )
        *--ptr = '-';

    ফেরত ptr;
}

অক্ষর* itoa( int64_t _val, অক্ষর* বাফার, পূর্ণসংখ্যা /*radix*/, যৌক্তিক _signed)
{
    ধ্রুবক পূর্ণসংখ্যা radix = 10;
    অক্ষর* ptr=বাফার + 23 /* enough even for 64-bit integers */;
    পূর্ণসংখ্যা sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : abs(_val);

    *ptr = '\0';
    করো
    {
        uint64_t r = val / radix;
        *--ptr = (অক্ষর)(val - (r*radix) + '0');
        val = r;
    }
    যখন( val != 0 );

    যদি( sign < 0 )
        *--ptr = '-';

    ফেরত ptr;
}

অক্ষর* doubleToString( অক্ষর* buf, size_t buf_আকার, দ্বিগুণ value, যৌক্তিক explicitZero )
{
    Cv64suf val;
    unsigned ieee754_hi;

    val.f = value;
    ieee754_hi = (unsigned)(val.u >> 32);

    যদি( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        পূর্ণসংখ্যা ivalue = cvRound(value);
        যদি( ivalue == value )
        {
            যদি( explicitZero )
                snprintf( buf, buf_আকার, "%d.0", ivalue );
            অন্যথা
                snprintf( buf, buf_আকার, "%d.", ivalue );
        }
        অন্যথা
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, buf_আকার, "%.17g", value );

            অক্ষর* ptr = buf;
            যদি( *ptr == '+' || *ptr == '-' )
                ptr++;
            জন্য( ; cv_isdigit(*ptr); ptr++ )
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
            strcpy( buf, (পূর্ণসংখ্যা)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    ফেরত buf;
}

অক্ষর* floatToString( অক্ষর* buf, size_t buf_আকার, দশমিক value, যৌক্তিক halfprecision, যৌক্তিক explicitZero )
{
    Cv32suf val;
    unsigned ieee754;
    val.f = value;
    ieee754 = val.u;

    যদি( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        পূর্ণসংখ্যা ivalue = cvRound(value);
        যদি( ivalue == value )
        {
            যদি( explicitZero )
                snprintf( buf, buf_আকার, "%d.0", ivalue );
            অন্যথা
                snprintf( buf, buf_আকার, "%d.", ivalue );
        }
        অন্যথা
        {
            যদি (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, buf_আকার, "%.5g", value);
            }
            অন্যথা
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, buf_আকার, "%.9g", value);
            }

            অক্ষর* ptr = buf;
            যদি( *ptr == '+' || *ptr == '-' )
                ptr++;
            জন্য( ; cv_isdigit(*ptr); ptr++ )
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
            strcpy( buf, (পূর্ণসংখ্যা)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    ফেরত buf;
}

স্থির ধ্রুবক অক্ষর symbols[] = "ucwsifdhHbUIneE";

স্থির অক্ষর type_প্রতীক(পূর্ণসংখ্যা depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    ফেরত symbols[depth];
}

স্থির পূর্ণসংখ্যা প্রতীক_To_Type(অক্ষর c)
{
    যদি (c == 'r')
        ফেরত CV_SEQ_ELTYPE_PTR;
    ধ্রুবক অক্ষর* pos = strchr( symbols, c );
    যদি( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    ফেরত static_cast<পূর্ণসংখ্যা>(pos - symbols);
}

অক্ষর* encodeFormat(পূর্ণসংখ্যা elem_type, অক্ষর* dt, size_t dt_len)
{
    পূর্ণসংখ্যা cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    অক্ষর প্রতীক = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_প্রতীক(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, প্রতীক);
    ফেরত dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
অক্ষর* encodeFormat(পূর্ণসংখ্যা elem_type, অক্ষর* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    ফেরত encodeFormat(elem_type, dt, max);
}

পূর্ণসংখ্যা decodeFormat( ধ্রুবক অক্ষর* dt, পূর্ণসংখ্যা* fmt_pairs, পূর্ণসংখ্যা max_len )
{
    পূর্ণসংখ্যা fmt_pair_গুনতি = 0;
    পূর্ণসংখ্যা i = 0, k = 0, len = dt ? (পূর্ণসংখ্যা)strlen(dt) : 0;

    যদি( !dt || !len )
        ফেরত 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    জন্য( ; k < len; k++ )
    {
        অক্ষর c = dt[k];

        যদি( cv_isdigit(c) )
        {
            পূর্ণসংখ্যা গুনতি_2 = c - '0';
            যদি( cv_isdigit(dt[k+1]) )
            {
                অক্ষর* endptr = 0;
                গুনতি_2 = (পূর্ণসংখ্যা)strtol( dt+k, &endptr, 10 );
                k = (পূর্ণসংখ্যা)(endptr - dt) - 1;
            }

            যদি( গুনতি_2 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = গুনতি_2;
        }
        অন্যথা
        {
            পূর্ণসংখ্যা depth = প্রতীক_To_Type(c);
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
    ফেরত fmt_pair_গুনতি;
}

পূর্ণসংখ্যা calc_Elem_আকার( ধ্রুবক অক্ষর* dt, পূর্ণসংখ্যা initial_আকার )
{
    পূর্ণসংখ্যা size = 0;
    পূর্ণসংখ্যা fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_গুনতি;
    পূর্ণসংখ্যা comp_আকার;

    fmt_pair_গুনতি = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_গুনতি *= 2;
    জন্য( i = 0, size = initial_আকার; i < fmt_pair_গুনতি; i += 2 )
    {
        comp_আকার = CV_ELEM_আকার(fmt_pairs[i+1]);
        size = cvAlign( size, comp_আকার );
        size += comp_আকার * fmt_pairs[i];
    }
    যদি( initial_আকার == 0 )
    {
        comp_আকার = CV_ELEM_আকার(fmt_pairs[1]);
        size = cvAlign( size, comp_আকার );
    }
    ফেরত size;
}


পূর্ণসংখ্যা calc_Struct_আকার( ধ্রুবক অক্ষর* dt, পূর্ণসংখ্যা initial_আকার )
{
    পূর্ণসংখ্যা size = calc_Elem_আকার( dt, initial_আকার );
    size_t elem_max_আকার = 0;
    জন্য ( ধ্রুবক অক্ষর * type = dt; *type != '\0'; type++ )
    {
        অক্ষর v = *type;
        যদি (v >= '0' && v <= '9')
            চালু;  // skip vector size
        নির্বাচন (v)
        {
        ক্ষেত্র 'u': { elem_max_আকার = std::max( elem_max_আকার, আকার(uchar ) ); বিরতি; }
        ক্ষেত্র 'b': { elem_max_আকার = std::max( elem_max_আকার, আকার(যৌক্তিক  ) ); বিরতি; }
        ক্ষেত্র 'c': { elem_max_আকার = std::max( elem_max_আকার, আকার(schar ) ); বিরতি; }
        ক্ষেত্র 'w': { elem_max_আকার = std::max( elem_max_আকার, আকার(ushort) ); বিরতি; }
        ক্ষেত্র 's': { elem_max_আকার = std::max( elem_max_আকার, আকার(short ) ); বিরতি; }
        ক্ষেত্র 'i': { elem_max_আকার = std::max( elem_max_আকার, আকার(পূর্ণসংখ্যা   ) ); বিরতি; }
        ক্ষেত্র 'n': { elem_max_আকার = std::max( elem_max_আকার, আকার(unsigned) ); বিরতি; }
        ক্ষেত্র 'f': { elem_max_আকার = std::max( elem_max_আকার, আকার(দশমিক ) ); বিরতি; }
        ক্ষেত্র 'd': { elem_max_আকার = std::max( elem_max_আকার, আকার(দ্বিগুণ) ); বিরতি; }
        ক্ষেত্র 'h': { elem_max_আকার = std::max( elem_max_আকার, আকার(hfloat)); বিরতি; }
        ক্ষেত্র 'H': { elem_max_আকার = std::max( elem_max_আকার, আকার(bfloat)); বিরতি; }
        ক্ষেত্র 'e': { elem_max_আকার = std::max( elem_max_আকার, আকার(fp8_t) ); বিরতি; }
        ক্ষেত্র 'E': { elem_max_আকার = std::max( elem_max_আকার, আকার(fp8a_t)); বিরতি; }
        ক্ষেত্র 'I': { elem_max_আকার = std::max( elem_max_আকার, আকার(int64_t)); বিরতি; }
        ক্ষেত্র 'U': { elem_max_আকার = std::max( elem_max_আকার, আকার(uint64_t)); বিরতি; }
        পূর্বনির্ধারিত:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (অক্ষর)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<পূর্ণসংখ্যা>(elem_max_আকার) );
    ফেরত size;
}

পূর্ণসংখ্যা decodeSimpleFormat( ধ্রুবক অক্ষর* dt )
{
    পূর্ণসংখ্যা elem_type = -1;
    পূর্ণসংখ্যা fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_গুনতি;

    fmt_pair_গুনতি = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    যদি( fmt_pair_গুনতি != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    ফেরত elem_type;
}

}

#যদি defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#define CV_LITTLE_ENDIAN_MEM_ACCESS 1
#অন্যথা
#define CV_LITTLE_ENDIAN_MEM_ACCESS 0
#endif

স্থির অন্তঃরেখ পূর্ণসংখ্যা readInt(ধ্রুবক uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#যদি CV_LITTLE_ENDIAN_MEM_ACCESS
    পূর্ণসংখ্যা val;
    memcpy(&val, p, আকার(val));
    ফেরত val;
#অন্যথা
    পূর্ণসংখ্যা val = (পূর্ণসংখ্যা)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    ফেরত val;
#endif
}

স্থির অন্তঃরেখ int64_t readLong(ধ্রুবক uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#যদি CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    memcpy(&val, p, আকার(val));
    ফেরত val;
#অন্যথা
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    ফেরত val0 | ((int64_t)val1 << 32);
#endif
}

স্থির অন্তঃরেখ দ্বিগুণ readReal(ধ্রুবক uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#যদি CV_LITTLE_ENDIAN_MEM_ACCESS
    দ্বিগুণ val;
    memcpy(&val, p, আকার(val));
    ফেরত val;
#অন্যথা
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    ফেরত val.f;
#endif
}

template <typename T>
স্থির অন্তঃরেখ শূন্য writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#যদি CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &ival, আকার(ival));
#অন্যথা
    জন্য (size_t i = 0, j = 0; i < আকার(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#endif
}

স্থির অন্তঃরেখ শূন্য writeReal(uchar* p, দ্বিগুণ fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#যদি CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &fval, আকার(fval));
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
    বাফার.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = মিথ্যা;
    state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = মিথ্যা;
    delayed_struct_চাবি = nullptr;
    delayed_struct_flags = 0;
    delayed_type_name = nullptr;
    base64_writer = nullptr;
    is_opened = মিথ্যা;
    dummy_eof = মিথ্যা;
    write_mode = মিথ্যা;
    mem_mode = মিথ্যা;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    empty_stream = সত্য;

    strbufv.clear();
    strbuf = 0;
    strbufsize = strbufpos = 0;
    roots.clear();

    fs_তথ্য.clear();
    fs_তথ্য_ptrs.clear();
    fs_তথ্য_blksz.clear();
    freeSpaceOfs = 0;

    str_hash.clear();
    str_hash_তথ্য.clear();
    str_hash_তথ্য.resize(1);
    str_hash_তথ্য[0] = '\0';

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
            যখন (write_স্তূপ.size() > 1) {
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

শূন্য FileStorage::Impl::analyze_file_name(ধ্রুবক std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    স্থির ধ্রুবক অক্ষর not_file_name = '\n';
    স্থির ধ্রুবক অক্ষর parameter_begin = '?';
    স্থির ধ্রুবক অক্ষর parameter_separator = '&';

    যদি (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        ফেরত;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    যদি (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        জন্য (size_t param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            যদি ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

যৌক্তিক FileStorage::Impl::open(ধ্রুবক অক্ষর *filename_or_buf, পূর্ণসংখ্যা _flags, ধ্রুবক অক্ষর *encoding) {
    যৌক্তিক ok = সত্য;
    release();

    যৌক্তিক append = (_flags & 3) == FileStorage::APPEND;
    mem_mode = (_flags & FileStorage::MEMORY) != 0;

    write_mode = (_flags & 3) != 0;
    যৌক্তিক write_base64 = (write_mode || append) && (_flags & FileStorage::BASE64) != 0;

    যৌক্তিক isGZ = মিথ্যা;

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
        অক্ষর compression = '\0';

        যদি (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            যদি (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                যদি (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = সত্য;
                }

                অন্যথা যদি(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = সত্য;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        যদি (!isGZ) {
            file = fopen(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            যদি (!file)
            {
                CV_LOG_ERROR(ফাঁকা, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                ফেরত মিথ্যা;
            }
        } অন্যথা {
#যদি USE_ZLIB
            যদি (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            অক্ষর mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            যদি (!gzfile)
            {
                CV_LOG_ERROR(ফাঁকা, "Can't open archive: '" << filename << "' mode=" << mode);
                ফেরত মিথ্যা;
            }
#অন্যথা
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#endif
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_তথ্য.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    যদি (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        যদি (mem_mode)
            outbuf.clear();

        যদি (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            ধ্রুবক অক্ষর *dot_pos = ফাঁকা;
            ধ্রুবক অক্ষর *dot_pos2 = ফাঁকা;
            // like strrchr() implementation, but save two last positions simultaneously
            জন্য (ধ্রুবক অক্ষর *pos = &filename[0]; pos[0] != 0; pos++) {
                যদি (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            যদি (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != ফাঁকা) {
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
        পূর্ণসংখ্যা buf_আকার_2 = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        যদি (append) {
            fseek(file, 0, SEEK_END);
            যদি (ftell(file) == 0)
                append = মিথ্যা;
        }

        write_স্তূপ.clear();
        empty_stream = সত্য;
        write_স্তূপ.push_back(FStruct_তথ্য("", File_গ্রন্থি::MAP | File_গ্রন্থি::EMPTY, 0));
        বাফার.reserve(buf_আকার_2 + 1024);
        বাফার.resize(buf_আকার_2);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;

        যদি (fmt == FileStorage::FORMAT_XML) {
            size_t file_আকার_2 = file ? (size_t) ftell(file) : (size_t) 0;
            যদি (!append || file_আকার_2 == 0) {
                যদি (encoding && *encoding != '\0') {
                    যদি (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(strlen(encoding) < 1000);
                    অক্ষর buf[1100];
                    snprintf(buf, আকার(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } অন্যথা
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } অন্যথা {
                পূর্ণসংখ্যা xml_buf_আকার = 1 << 10;
                অক্ষর substr[] = "</opencv_storage>";
                পূর্ণসংখ্যা last_occurrence = -1;
                xml_buf_আকার = MIN(xml_buf_আকার, পূর্ণসংখ্যা(file_আকার_2));
                fseek(file, -xml_buf_আকার, SEEK_END);
                // find the last occurrence of </opencv_storage>
                জন্য (;;) {
                    পূর্ণসংখ্যা সারি_offset = (পূর্ণসংখ্যা) ftell(file);
                    ধ্রুবক অক্ষর *ptr0 = এই->gets(xml_buf_আকার);
                    ধ্রুবক অক্ষর *ptr = ফাঁকা;
                    যদি (!ptr0)
                        বিরতি;
                    ptr = ptr0;
                    জন্য (;;) {
                        ptr = strstr(ptr, substr);
                        যদি (!ptr)
                            বিরতি;
                        last_occurrence = সারি_offset + (পূর্ণসংখ্যা) (ptr - ptr0);
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

            emitter_do_not_use_direct_dereference = createXMLEmitter(এই);
        } অন্যথা যদি (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                যদি (!append) {
                    যদি (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    অন্যথা
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            অন্যথা
                puts("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(এই);
        } অন্যথা {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            যদি (!append)
                puts("{\n");
            অন্যথা {
                যৌক্তিক valid = মিথ্যা;
                long roffset = 0;
                জন্য (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    ধ্রুবক অক্ষর end_mark = '}';
                    যদি (fgetc(file) == end_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = সত্য;
                        বিরতি;
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
            emitter_do_not_use_direct_dereference = createJSONEmitter(এই);
        }
        is_opened = সত্য;
    } অন্যথা {
        ধ্রুবক size_t buf_size0 = 40;
        বাফার.resize(buf_size0);
        যদি (mem_mode) {
            strbuf = (অক্ষর *) filename_or_buf;
            strbufsize = strlen(strbuf);
        }

        ধ্রুবক অক্ষর *yaml_signature = "%YAML";
        ধ্রুবক অক্ষর *json_signature = "{";
        ধ্রুবক অক্ষর *xml_signature = "<?xml";
        অক্ষর *buf = এই->gets(16);
        CV_Assert(buf);
        অক্ষর *bufPtr = cv_skip_BOM(buf);
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

        চেষ্টা {
            অক্ষর *ptr = বাফার_Start();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_গ্রন্থি মূল_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_গ্রন্থি_Space(মূল_nodes, 9);
            *rptr = File_গ্রন্থি::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            নির্বাচন (fmt) {
                ক্ষেত্র FileStorage::FORMAT_XML:
                    parser_do_not_use_direct_dereference = createXMLParser(এই);
                    বিরতি;
                ক্ষেত্র FileStorage::FORMAT_YAML:
                ক্ষেত্র FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_direct_dereference = createYAMLParser(এই);
                    বিরতি;
                ক্ষেত্র FileStorage::FORMAT_JSON:
                    parser_do_not_use_direct_dereference = createJSONParser(এই);
                    বিরতি;
                পূর্বনির্ধারিত:
                    parser_do_not_use_direct_dereference = Ptr<FileStorageParser>();
            }

            যদি (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                যদি (ok) {
                    finalizeCollection(মূল_nodes);

                    CV_Assert(!fs_তথ্য_ptrs.empty());
                    File_গ্রন্থি roots_গ্রন্থি(fs_ext, 0, 0);
                    size_t i, nroots = roots_গ্রন্থি.size();
                    File_গ্রন্থি_Iterator it = roots_গ্রন্থি.begin();

                    জন্য (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        ধরো (...)
        {
            // FIXIT log error message
            is_opened = সত্য;
            release();
            ছোড়ো;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = সত্য;
        std::vector<অক্ষর> tmpbuf;
        std::swap(বাফার, tmpbuf);
        bufofs = 0;
    }
    ফেরত ok;
}

শূন্য FileStorage::Impl::puts(ধ্রুবক অক্ষর *str) {
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

অক্ষর *FileStorage::Impl::getsFromFile(অক্ষর *buf, পূর্ণসংখ্যা গুনতি_2) {
    যদি (file)
        ফেরত fgets(buf, গুনতি_2, file);
#যদি USE_ZLIB
    যদি (gzfile)
        ফেরত gzgets(gzfile, buf, গুনতি_2);
#endif
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

অক্ষর *FileStorage::Impl::gets(size_t max_গুনতি) {
    যদি (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        ধ্রুবক অক্ষর *instr = strbuf;
        জন্য (; i < len; i++) {
            অক্ষর c = instr[i];
            যদি (c == '\0' || c == '\n') {
                যদি (c == '\n')
                    i++;
                বিরতি;
            }
        }
        size_t গুনতি_2 = i - strbufpos;
        যদি (max_গুনতি == 0 || max_গুনতি > গুনতি_2)
            max_গুনতি = গুনতি_2;
        বাফার.resize(std::max(বাফার.size(), max_গুনতি + 8));
        memcpy(&বাফার[0], instr + strbufpos, max_গুনতি);
        বাফার[max_গুনতি] = '\0';
        strbufpos = i;
        ফেরত max_গুনতি > 0 ? &বাফার[0] : 0;
    }

    ধ্রুবক size_t MAX_BLOCK_আকার = INT_MAX / 2; // hopefully, that will be enough
    যদি (max_গুনতি == 0)
        max_গুনতি = MAX_BLOCK_আকার;
    অন্যথা
        CV_Assert(max_গুনতি < MAX_BLOCK_আকার);
    size_t ofs = 0;

    জন্য (;;) {
        পূর্ণসংখ্যা গুনতি_2 = (পূর্ণসংখ্যা) std::min(বাফার.size() - ofs - 16, max_গুনতি);
        অক্ষর *ptr = getsFromFile(&বাফার[ofs], গুনতি_2 + 1);
        যদি (!ptr)
            বিরতি;
        পূর্ণসংখ্যা delta = (পূর্ণসংখ্যা) strlen(ptr);
        ofs += delta;
        max_গুনতি -= delta;
        যদি (delta == 0 || ptr[delta - 1] == '\n' || max_গুনতি == 0)
            বিরতি;
        যদি (delta == গুনতি_2)
            বাফার.resize((size_t) (বাফার.size() * 1.5));
    }
    ফেরত ofs > 0 ? &বাফার[0] : 0;
}

অক্ষর *FileStorage::Impl::gets() {
    অক্ষর *ptr = এই->gets(0);
    যদি (!ptr) {
        ptr = বাফার_Start();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        ফেরত 0;
    } অন্যথা {
        size_t l = strlen(ptr);
        যদি (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    ফেরত ptr;
}

যৌক্তিক FileStorage::Impl::eof() {
    যদি (dummy_eof)
        ফেরত সত্য;
    যদি (strbuf)
        ফেরত strbufpos >= strbufsize;
    যদি (file)
        ফেরত feof(file) != 0;
#যদি USE_ZLIB
    যদি (gzfile)
        ফেরত gzeof(gzfile) != 0;
#endif
    ফেরত মিথ্যা;
}

শূন্য FileStorage::Impl::setEof() {
    dummy_eof = সত্য;
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
    is_opened = মিথ্যা;
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

অক্ষর *FileStorage::Impl::resize_Write_বাফার(অক্ষর *ptr, পূর্ণসংখ্যা len) {
    ধ্রুবক অক্ষর *বাফার_end = &বাফার[0] + বাফার.size();
    যদি (ptr + len < বাফার_end)
        ফেরত ptr;

    ধ্রুবক অক্ষর *বাফার_start = &বাফার[0];
    পূর্ণসংখ্যা written_len = (পূর্ণসংখ্যা) (ptr - বাফার_start);

    CV_Assert(written_len <= (পূর্ণসংখ্যা) বাফার.size());
    পূর্ণসংখ্যা new_আকার_2 = (পূর্ণসংখ্যা) ((বাফার_end - বাফার_start) * 3 / 2);
    new_আকার_2 = MAX(written_len + len, new_আকার_2);
    বাফার.reserve(new_আকার_2 + 256);
    বাফার.resize(new_আকার_2);
    bufofs = written_len;
    ফেরত &বাফার[0] + bufofs;
}

অক্ষর *FileStorage::Impl::flush() {
    অক্ষর *বাফার_start = &বাফার[0];
    অক্ষর *ptr = বাফার_start + bufofs;

    যদি (ptr > বাফার_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(বাফার_start);
        bufofs = 0;
    }

    পূর্ণসংখ্যা indent = write_স্তূপ.back().indent;

    যদি (space != indent) {
        memset(বাফার_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = বাফার_start + bufofs;

    ফেরত ptr;
}

শূন্য FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(মিথ্যা);
    যদি (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_স্তূপ.empty());

    FStruct_তথ্য &current_struct = write_স্তূপ.back();
    যদি (fmt == FileStorage::FORMAT_JSON && !File_গ্রন্থি::isFlow(current_struct.flags) && write_স্তূপ.size() > 1)
        current_struct.indent = write_স্তূপ[write_স্তূপ.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_স্তূপ.pop_back();
    যদি (!write_স্তূপ.empty())
        write_স্তূপ.back().flags &= ~File_গ্রন্থি::EMPTY;
}

শূন্য FileStorage::Impl::startWriteStruct_helper(ধ্রুবক অক্ষর *চাবি, পূর্ণসংখ্যা struct_flags,
                                                ধ্রুবক অক্ষর *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (File_গ্রন্থি::TYPE_MASK | File_গ্রন্থি::FLOW)) | File_গ্রন্থি::EMPTY;
    যদি (!File_গ্রন্থি::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    যদি (type_name && type_name[0] == '\0')
        type_name = 0;

    FStruct_তথ্য s = getEmitter().startWriteStruct(write_স্তূপ.back(), চাবি, struct_flags, type_name);

    write_স্তূপ.push_back(s);
    size_t write_স্তূপ_আকার = write_স্তূপ.size();
    যদি (write_স্তূপ_আকার > 1)
        write_স্তূপ[write_স্তূপ_আকার - 2].flags &= ~File_গ্রন্থি::EMPTY;

    যদি (fmt != FileStorage::FORMAT_JSON && !File_গ্রন্থি::isFlow(s.flags))
        flush();

    যদি (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_গ্রন্থি::isMap(struct_flags)) {
        getEmitter().লিখ("type_id", type_name, মিথ্যা);
    }
}

শূন্য FileStorage::Impl::startWriteStruct(ধ্রুবক অক্ষর *চাবি, পূর্ণসংখ্যা struct_flags,
                                         ধ্রুবক অক্ষর *type_name) {
    check_if_write_struct_is_delayed(মিথ্যা);
    যদি (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    যদি (state_of_writing_base64 == FileStorage_API::Uncertain && File_গ্রন্থি::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(চাবি, struct_flags, type_name);
    } অন্যথা যদি (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        যদি ((File_গ্রন্থি::TYPE_MASK & struct_flags) != File_গ্রন্থি::SEQ)
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

শূন্য FileStorage::Impl::writeComment(ধ্রুবক অক্ষর *comment, যৌক্তিক eol_comment) {
    CV_Assert(write_mode);
    getEmitter().writeComment(comment, eol_comment);
}

শূন্য FileStorage::Impl::start_পরবর্তী_Stream() {
    CV_Assert(write_mode);
    যদি (!empty_stream) {
        যখন (!write_স্তূপ.empty())
            endWriteStruct();
        flush();
        getEmitter().start_পরবর্তী_Stream();
        empty_stream = সত্য;
        write_স্তূপ.push_back(FStruct_তথ্য("", File_গ্রন্থি::EMPTY, 0));
        bufofs = 0;
    }
}

শূন্য FileStorage::Impl::লিখ(ধ্রুবক String &চাবি, পূর্ণসংখ্যা value) {
    CV_Assert(write_mode);
    getEmitter().লিখ(চাবি.c_str(), value);
}

শূন্য FileStorage::Impl::লিখ(ধ্রুবক String &চাবি, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().লিখ(চাবি.c_str(), value);
}

শূন্য FileStorage::Impl::লিখ(ধ্রুবক String &চাবি, দ্বিগুণ value) {
    CV_Assert(write_mode);
    getEmitter().লিখ(চাবি.c_str(), value);
}

শূন্য FileStorage::Impl::লিখ(ধ্রুবক String &চাবি, ধ্রুবক String &value) {
    CV_Assert(write_mode);
    getEmitter().লিখ(চাবি.c_str(), value.c_str(), মিথ্যা);
}

শূন্য FileStorage::Impl::write_Raw_তথ্য(ধ্রুবক std::string &dt, ধ্রুবক শূন্য *তথ্য_3, size_t len) {
    CV_Assert(write_mode);

    যদি (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        write_Raw_তথ্য_Base64(তথ্য_3, len, dt.c_str());
        ফেরত;
    } অন্যথা যদি (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elem_আকার = fs::calc_Struct_আকার(dt.c_str(), 0);
    CV_Assert(elem_আকার);
    CV_Assert(len % elem_আকার == 0);
    len /= elem_আকার;

    যৌক্তিক explicitZero = fmt == FileStorage::FORMAT_JSON;
    ধ্রুবক uchar *data0 = (ধ্রুবক uchar *) তথ্য_3;
    পূর্ণসংখ্যা fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_গুনতি;
    অক্ষর buf[256] = "";

    fmt_pair_গুনতি = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    যদি (!len)
        ফেরত;

    যদি (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    যদি (fmt_pair_গুনতি == 1) {
        fmt_pairs[0] *= (পূর্ণসংখ্যা) len;
        len = 1;
    }

    জন্য (; len--; data0 += elem_আকার) {
        পূর্ণসংখ্যা offset = 0;
        জন্য (k = 0; k < fmt_pair_গুনতি; k++) {
            পূর্ণসংখ্যা i, গুনতি_2 = fmt_pairs[k * 2];
            পূর্ণসংখ্যা elem_type = fmt_pairs[k * 2 + 1];
            পূর্ণসংখ্যা elem_আকার_2 = CV_ELEM_আকার(elem_type);
            ধ্রুবক অক্ষর *ptr;

            offset = cvAlign(offset, elem_আকার_2);
            ধ্রুবক uchar *তথ্য = data0 + offset;

            জন্য (i = 0; i < গুনতি_2; i++) {
                নির্বাচন (elem_type) {
                    ক্ষেত্র CV_8U:
                        ptr = fs::itoa(*(uchar *) তথ্য, buf, 10);
                        তথ্য++;
                        বিরতি;
                    ক্ষেত্র CV_Bool:
                        ptr = fs::itoa(*(uchar *) তথ্য != 0, buf, 10);
                        তথ্য++;
                        বিরতি;
                    ক্ষেত্র CV_8S:
                        ptr = fs::itoa(*(অক্ষর *) তথ্য, buf, 10);
                        তথ্য++;
                        বিরতি;
                    ক্ষেত্র CV_16U:
                        ptr = fs::itoa(*(ushort *) তথ্য, buf, 10);
                        তথ্য += আকার(ushort);
                        বিরতি;
                    ক্ষেত্র CV_16S:
                        ptr = fs::itoa(*(short *) তথ্য, buf, 10);
                        তথ্য += আকার(short);
                        বিরতি;
                    ক্ষেত্র CV_32U:
                        ptr = fs::itoa((int64_t)*(unsigned*) তথ্য, buf, 10, মিথ্যা);
                        তথ্য += আকার(unsigned);
                        বিরতি;
                    ক্ষেত্র CV_32S:
                        ptr = fs::itoa(*(পূর্ণসংখ্যা *) তথ্য, buf, 10);
                        তথ্য += আকার(পূর্ণসংখ্যা);
                        বিরতি;
                    ক্ষেত্র CV_64U:
                        ptr = fs::itoa(*(uint64_t*) তথ্য, buf, 10, মিথ্যা);
                        তথ্য += আকার(uint64_t);
                        বিরতি;
                    ক্ষেত্র CV_64S:
                        ptr = fs::itoa(*(int64_t*) তথ্য, buf, 10, সত্য);
                        তথ্য += আকার(int64_t);
                        বিরতি;
                    ক্ষেত্র CV_32F:
                        ptr = fs::floatToString(buf, আকার(buf), *(দশমিক *) তথ্য, মিথ্যা, explicitZero);
                        তথ্য += আকার(দশমিক);
                        বিরতি;
                    ক্ষেত্র CV_64F:
                        ptr = fs::doubleToString(buf, আকার(buf), *(দ্বিগুণ *) তথ্য, explicitZero);
                        তথ্য += আকার(দ্বিগুণ);
                        বিরতি;
                    ক্ষেত্র CV_16F:
                        ptr = fs::floatToString(buf, আকার(buf), (দশমিক) *(hfloat *) তথ্য, সত্য, explicitZero);
                        তথ্য += আকার(hfloat);
                        বিরতি;
                    ক্ষেত্র CV_16BF:
                        ptr = fs::floatToString(buf, আকার(buf), (দশমিক) *(bfloat *) তথ্য, সত্য, explicitZero);
                        তথ্য += আকার(bfloat);
                        বিরতি;
                    ক্ষেত্র CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, আকার(buf), (দশমিক) *(fp8_t *) তথ্য, সত্য, explicitZero);
                        তথ্য += আকার(fp8_t);
                        বিরতি;
                    ক্ষেত্র CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, আকার(buf), (দশমিক) *(fp8a_t *) তথ্য, সত্য, explicitZero);
                        তথ্য += আকার(fp8a_t);
                        বিরতি;
                    পূর্বনির্ধারিত:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        ফেরত;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (পূর্ণসংখ্যা) (তথ্য - data0);
        }
    }
}

শূন্য FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(মিথ্যা);

    যদি (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_to_Base64_state(FileStorage_API::Base64State::Uncertain);
}

শূন্য FileStorage::Impl::switch_to_Base64_state(FileStorage_API::Base64State new_state) {
    ধ্রুবক অক্ষর *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    ধ্রুবক অক্ষর *err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    নির্বাচন (state_of_writing_base64) {
        ক্ষেত্র FileStorage_API::Base64State::Uncertain:
            নির্বাচন (new_state) {
                ক্ষেত্র FileStorage_API::Base64State::InUse:
                {
                    CV_DbgAssert(base64_writer == 0);
                    যৌক্তিক can_indent = (fmt != cv::FileStorage::Mode::FORMAT_JSON);
                    base64_writer = নতুন base64::Base64Writer(*এই, can_indent);
                    যদি (!can_indent) {
                        অক্ষর *ptr = বাফার_Ptr();
                        *ptr++ = '\0';
                        puts(বাফার_Start());
                        set_বাফার_Ptr(বাফার_Start());
                        memset(বাফার_Start(), 0, static_cast<পূর্ণসংখ্যা>(space));
                        puts("\"$base64$");
                    }
                    বিরতি;
                }
                ক্ষেত্র FileStorage_API::Base64State::Uncertain:
                    বিরতি;
                ক্ষেত্র FileStorage_API::Base64State::NotUse:
                    বিরতি;
                পূর্বনির্ধারিত:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    বিরতি;
            }
            বিরতি;
        ক্ষেত্র FileStorage_API::Base64State::InUse:
            নির্বাচন (new_state) {
                ক্ষেত্র FileStorage_API::Base64State::InUse:
                ক্ষেত্র FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    বিরতি;
                ক্ষেত্র FileStorage_API::Base64State::Uncertain:
                    delete base64_writer;
                    base64_writer = 0;
                    যদি ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        set_বাফার_Ptr(বাফার_Start());
                        flush();
                        memset(বাফার_Start(), 0, static_cast<পূর্ণসংখ্যা>(space) );
                        set_বাফার_Ptr(বাফার_Start());
                    }
                    বিরতি;
                পূর্বনির্ধারিত:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    বিরতি;
            }
            বিরতি;
        ক্ষেত্র FileStorage_API::Base64State::NotUse:
            নির্বাচন (new_state) {
                ক্ষেত্র FileStorage_API::Base64State::InUse:
                ক্ষেত্র FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    বিরতি;
                ক্ষেত্র FileStorage_API::Base64State::Uncertain:
                    বিরতি;
                পূর্বনির্ধারিত:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    বিরতি;
            }
            বিরতি;
        পূর্বনির্ধারিত:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            বিরতি;
    }

    state_of_writing_base64 = new_state;
}

শূন্য FileStorage::Impl::make_write_struct_delayed(ধ্রুবক অক্ষর *চাবি, পূর্ণসংখ্যা struct_flags, ধ্রুবক অক্ষর *type_name) {
    CV_Assert(is_write_struct_delayed == মিথ্যা);
    CV_DbgAssert(delayed_struct_চাবি == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    যদি (চাবি != nullptr) {
        delayed_struct_চাবি = নতুন অক্ষর[strlen(চাবি) + 1U];
        strcpy(delayed_struct_চাবি, চাবি);
    }

    যদি (type_name != nullptr) {
        delayed_type_name = নতুন অক্ষর[strlen(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = সত্য;
}

শূন্য FileStorage::Impl::check_if_write_struct_is_delayed(যৌক্তিক change_type_to_base64) {
    যদি (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_চাবি;
        std::string type_name;
        পূর্ণসংখ্যা struct_flags = delayed_struct_flags;

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

        is_write_struct_delayed = মিথ্যা;

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

শূন্য FileStorage::Impl::write_Raw_তথ্য_Base64(ধ্রুবক শূন্য *তথ্য_3, size_t len, ধ্রুবক অক্ষর *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(সত্য);

    যদি (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } অন্যথা যদি (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->লিখ(তথ্য_3, len, dt);
}

File_গ্রন্থি FileStorage::Impl::get_First_শীর্ষ_Level_গ্রন্থি() ধ্রুবক {
    ফেরত roots.empty() ? File_গ্রন্থি() : roots[0];
}

File_গ্রন্থি FileStorage::Impl::মূল(পূর্ণসংখ্যা streamIdx) ধ্রুবক {
    ফেরত streamIdx >= 0 && streamIdx < (পূর্ণসংখ্যা) roots.size() ? roots[streamIdx] : File_গ্রন্থি();
}

File_গ্রন্থি FileStorage::Impl::operator[](ধ্রুবক String &nodename) ধ্রুবক {
    ফেরত এই->operator[](nodename.c_str());
}

File_গ্রন্থি FileStorage::Impl::operator[](ধ্রুবক অক্ষর * /*nodename*/) ধ্রুবক {
    ফেরত File_গ্রন্থি();
}

পূর্ণসংখ্যা FileStorage::Impl::getFormat() ধ্রুবক { ফেরত fmt; }

অক্ষর *FileStorage::Impl::বাফার_Ptr() ধ্রুবক { ফেরত (অক্ষর *) (&বাফার[0] + bufofs); }

অক্ষর *FileStorage::Impl::বাফার_Start() ধ্রুবক { ফেরত (অক্ষর *) &বাফার[0]; }

অক্ষর *FileStorage::Impl::বাফার_End() ধ্রুবক { ফেরত (অক্ষর *) (&বাফার[0] + বাফার.size()); }

শূন্য FileStorage::Impl::set_বাফার_Ptr(অক্ষর *ptr) {
    অক্ষর *bufferstart = বাফার_Start();
    CV_Assert(ptr >= bufferstart && ptr <= বাফার_End());
    bufofs = ptr - bufferstart;
}

পূর্ণসংখ্যা FileStorage::Impl::wrapMargin() ধ্রুবক { ফেরত wrap_margin; }

FStruct_তথ্য &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_স্তূপ.empty());
    ফেরত write_স্তূপ.back();
}

শূন্য FileStorage::Impl::setNonEmpty() {
    empty_stream = মিথ্যা;
}

শূন্য FileStorage::Impl::processSpecialDouble(অক্ষর *buf, দ্বিগুণ *value, অক্ষর **endptr) {
    FileStorage_API *fs = এই;
    অক্ষর c = buf[0];
    পূর্ণসংখ্যা inf_hi = 0x7ff00000;

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

দ্বিগুণ FileStorage::Impl::strtod(অক্ষর *ptr, অক্ষর **endptr) {
    দ্বিগুণ fval = ::strtod(ptr, endptr);
    যদি (**endptr == '.') {
        অক্ষর *dot_pos = *endptr;
        *dot_pos = ',';
        দ্বিগুণ fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        যদি (*endptr > dot_pos)
            fval = fval2;
        অন্যথা
            *endptr = dot_pos;
    }

    যদি (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    ফেরত fval;
}

শূন্য FileStorage::Impl::convertToCollection(পূর্ণসংখ্যা type, File_গ্রন্থি &গ্রন্থি) {
    CV_Assert(type == File_গ্রন্থি::SEQ || type == File_গ্রন্থি::MAP);

    পূর্ণসংখ্যা গ্রন্থি_type = গ্রন্থি.type();
    যদি (গ্রন্থি_type == type)
        ফেরত;

    যৌক্তিক named = গ্রন্থি.isNamed();
    uchar *ptr = গ্রন্থি.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    দ্বিগুণ fval = 0;
    std::string sval;
    যৌক্তিক add_first_scalar = মিথ্যা;

    যদি (গ্রন্থি_type != File_গ্রন্থি::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_গ্রন্থি::SEQ);
        যদি (গ্রন্থি_type == File_গ্রন্থি::INT) {
            ival = readLong(ptr);
            add_first_scalar = সত্য;
        } অন্যথা যদি (গ্রন্থি_type == File_গ্রন্থি::REAL) {
            fval = readReal(ptr);
            add_first_scalar = সত্য;
        } অন্যথা যদি (গ্রন্থি_type == File_গ্রন্থি::STRING) {
            sval = std::string(গ্রন্থি);
            add_first_scalar = সত্য;
        } অন্যথা
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", গ্রন্থি_type));
    }

    ptr = reserve_গ্রন্থি_Space(গ্রন্থি, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_গ্রন্থি::NAMED : 0));
    // name has been copied automatically
    যদি (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    যদি (add_first_scalar)
        add_গ্রন্থি(গ্রন্থি, std::string(), গ্রন্থি_type,
                গ্রন্থি_type == File_গ্রন্থি::INT ? (ধ্রুবক শূন্য *) &ival :
                গ্রন্থি_type == File_গ্রন্থি::REAL ? (ধ্রুবক শূন্য *) &fval :
                গ্রন্থি_type == File_গ্রন্থি::STRING ? (ধ্রুবক শূন্য *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_গ্রন্থি_Space(File_গ্রন্থি &গ্রন্থি, size_t sz) {
    যৌক্তিক shrinkBlock = মিথ্যা;
    size_t shrinkBlockIdx = 0, shrink_আকার = 0;

    uchar *ptr = 0, *blockEnd = 0;

    যদি (!fs_তথ্য_ptrs.empty()) {
        size_t blockIdx = গ্রন্থি.blockIdx;
        size_t ofs = গ্রন্থি.ofs;
        CV_Assert(blockIdx == fs_তথ্য_ptrs.size() - 1);
        CV_Assert(ofs <= fs_তথ্য_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_তথ্য_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_তথ্য_ptrs[blockIdx] + ofs;
        blockEnd = fs_তথ্য_ptrs[blockIdx] + fs_তথ্য_blksz[blockIdx];

        CV_Assert(ptr >= fs_তথ্য_ptrs[blockIdx] && ptr <= blockEnd);
        যদি (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            ফেরত ptr;
        }

        যদি (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_তথ্য[blockIdx]->resize(sz);
            ptr = &fs_তথ্য[blockIdx]->at(0);
            fs_তথ্য_ptrs[blockIdx] = ptr;
            fs_তথ্য_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            ফেরত ptr;
        }

        shrinkBlock = সত্য;
        shrinkBlockIdx = blockIdx;
        shrink_আকার = ofs;
    }

    size_t block_আকার = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(block_আকার);
    fs_তথ্য.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_তথ্য_ptrs.push_back(new_ptr);
    fs_তথ্য_blksz.push_back(block_আকার);
    গ্রন্থি.blockIdx = fs_তথ্য_ptrs.size() - 1;
    গ্রন্থি.ofs = 0;
    freeSpaceOfs = sz;

    যদি (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        যদি (ptr[0] & File_গ্রন্থি::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    যদি (shrinkBlock) {
        fs_তথ্য[shrinkBlockIdx]->resize(shrink_আকার);
        fs_তথ্য_blksz[shrinkBlockIdx] = shrink_আকার;
    }

    ফেরত new_ptr;
}

unsigned FileStorage::Impl::getStringOfs(ধ্রুবক std::string &চাবি) ধ্রুবক {
    str_hash_t::const_iterator it = str_hash.find(চাবি);
    ফেরত it != str_hash.end() ? it->second : 0;
}

File_গ্রন্থি FileStorage::Impl::add_গ্রন্থি(File_গ্রন্থি &collection, ধ্রুবক std::string &চাবি,
                                    পূর্ণসংখ্যা elem_type, ধ্রুবক শূন্য *value, পূর্ণসংখ্যা len) {
    FileStorage_API *fs = এই;
    যৌক্তিক noname = চাবি.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(চাবি.c_str(), "_") == 0);
    convertToCollection(noname ? File_গ্রন্থি::SEQ : File_গ্রন্থি::MAP, collection);

    যৌক্তিক isseq = collection.empty() ? মিথ্যা : collection.isSeq();
    যদি (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    unsigned strofs = 0;
    যদি (!noname) {
        strofs = getStringOfs(চাবি);
        যদি (!strofs) {
            strofs = (unsigned) str_hash_তথ্য.size();
            size_t keysize = চাবি.size() + 1;
            str_hash_তথ্য.resize(strofs + keysize);
            memcpy(&str_hash_তথ্য[0] + strofs, &চাবি[0], keysize);
            str_hash.insert(std::make_pair(চাবি, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t blockIdx = fs_তথ্য_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_গ্রন্থি গ্রন্থি(fs_ext, blockIdx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_গ্রন্থি_Space(গ্রন্থি, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_গ্রন্থি::NAMED));
    যদি (elem_type == File_গ্রন্থি::NONE)
        freeSpaceOfs -= 8;

    যদি (!noname) {
        writeInt(ptr, (পূর্ণসংখ্যা) strofs);
        ptr += 4;
    }

    যদি (elem_type == File_গ্রন্থি::SEQ || elem_type == File_গ্রন্থি::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    যদি (value)
        গ্রন্থি.set_মান(elem_type, value, len);

    যদি (collection.isNamed())
        cp += 4;
    পূর্ণসংখ্যা nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    ফেরত গ্রন্থি;
}

শূন্য FileStorage::Impl::finalizeCollection(File_গ্রন্থি &collection) {
    যদি (!collection.isSeq() && !collection.isMap())
        ফেরত;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    যদি (*ptr0 & File_গ্রন্থি::NAMED)
        ptr += 4;
    size_t blockIdx = collection.blockIdx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t raw_আকার = 4;
    unsigned sz = (unsigned) readInt(ptr + 4);
    যদি (sz > 0) {
        size_t lastBlockIdx = fs_তথ্য_ptrs.size() - 1;

        জন্য (; blockIdx < lastBlockIdx; blockIdx++) {
            raw_আকার += fs_তথ্য_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    raw_আকার += freeSpaceOfs - ofs;
    writeInt(ptr, (পূর্ণসংখ্যা) raw_আকার);
}

শূন্য FileStorage::Impl::normalize_গ্রন্থি_Ofs(size_t &blockIdx, size_t &ofs) ধ্রুবক {
    যখন (ofs >= fs_তথ্য_blksz[blockIdx]) {
        যদি (blockIdx == fs_তথ্য_blksz.size() - 1) {
            CV_Assert(ofs == fs_তথ্য_blksz[blockIdx]);
            বিরতি;
        }
        ofs -= fs_তথ্য_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    ফেরত state_of_writing_base64;
}

পূর্ণসংখ্যা FileStorage::Impl::get_space() {
    ফেরত space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = সত্য;
}

শূন্য FileStorage::Impl::Base64Decoder::init(ধ্রুবক Ptr<FileStorageParser> &_parser, অক্ষর *_ptr, পূর্ণসংখ্যা _indent) {
    parser_do_not_use_direct_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = মিথ্যা;
}

যৌক্তিক FileStorage::Impl::Base64Decoder::readMore(পূর্ণসংখ্যা needed) {
    স্থির ধ্রুবক uchar base64tab[] =
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
        ফেরত মিথ্যা;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    জন্য (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    অক্ষর *beg = 0, *end = 0;
    যৌক্তিক ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    যদি (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = সত্য;
        size_t tc = totalchars;
        জন্য (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    পূর্ণসংখ্যা i = 0, j, n = (পূর্ণসংখ্যা) encoded.size();
    যদি (n > 0) {
        ধ্রুবক uchar *tab = base64tab;
        অক্ষর *src = &encoded[0];

        জন্য (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(পূর্ণসংখ্যা) (uchar) src[i]], c = tab[(পূর্ণসংখ্যা) (uchar) src[i + 1]];
            uchar b = tab[(পূর্ণসংখ্যা) (uchar) src[i + 2]], a = tab[(পূর্ণসংখ্যা) (uchar) src[i + 3]];

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
    জন্য (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    ফেরত (পূর্ণসংখ্যা) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    যদি (ofs >= sz && !readMore(1))
        ফেরত (uchar) 0;
    ফেরত decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    যদি (ofs + 2 > sz && !readMore(2))
        ফেরত (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    ফেরত val;
}

পূর্ণসংখ্যা FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    যদি (ofs + 4 > sz && !readMore(4))
        ফেরত 0;
    পূর্ণসংখ্যা ival = readInt(&decoded[ofs]);
    ofs += 4;
    ফেরত ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    যদি (ofs + 8 > sz && !readMore(8))
        ফেরত 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    ফেরত ival;
}

দ্বিগুণ FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    যদি (ofs + 8 > sz && !readMore(8))
        ফেরত 0;
    দ্বিগুণ fval = readReal(&decoded[ofs]);
    ofs += 8;
    ফেরত fval;
}

যৌক্তিক FileStorage::Impl::Base64Decoder::endOfStream() ধ্রুবক { ফেরত eos; }

অক্ষর *FileStorage::Impl::Base64Decoder::getPtr() ধ্রুবক { ফেরত ptr; }


অক্ষর *FileStorage::Impl::parseBase64(অক্ষর *ptr, পূর্ণসংখ্যা indent, File_গ্রন্থি &collection) {
    ধ্রুবক পূর্ণসংখ্যা BASE64_HDR_আকার = 24;
    অক্ষর dt[BASE64_HDR_আকার + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    পূর্ণসংখ্যা i, k;

    জন্য (i = 0; i < BASE64_HDR_আকার; i++)
        dt[i] = (অক্ষর) base64decoder.getUInt8();
    জন্য (i = 0; i < BASE64_HDR_আকার; i++)
        যদি (isspace(dt[i]))
            বিরতি;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    পূর্ণসংখ্যা fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    পূর্ণসংখ্যা fmt_pair_গুনতি = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    দ্বিগুণ fval = 0;

    জন্য (;;) {
        জন্য (k = 0; k < fmt_pair_গুনতি; k++) {
            পূর্ণসংখ্যা elem_type = fmt_pairs[k * 2 + 1];
            পূর্ণসংখ্যা গুনতি_2 = fmt_pairs[k * 2];

            জন্য (i = 0; i < গুনতি_2; i++) {
                পূর্ণসংখ্যা গ্রন্থি_type = File_গ্রন্থি::INT;
                নির্বাচন (elem_type) {
                    ক্ষেত্র CV_8U:
                        ival = base64decoder.getUInt8();
                        বিরতি;
                    ক্ষেত্র CV_8S:
                        ival = (অক্ষর) base64decoder.getUInt8();
                        বিরতি;
                    ক্ষেত্র CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        বিরতি;
                    ক্ষেত্র CV_16U:
                        ival = base64decoder.getUInt16();
                        বিরতি;
                    ক্ষেত্র CV_16S:
                        ival = (short) base64decoder.getUInt16();
                        বিরতি;
                    ক্ষেত্র CV_32U:
                        ival = base64decoder.getInt32();
                        বিরতি;
                    ক্ষেত্র CV_32S:
                        ival = base64decoder.getInt32();
                        বিরতি;
                    ক্ষেত্র CV_64U:
                        ival = base64decoder.getInt64();
                        বিরতি;
                    ক্ষেত্র CV_64S:
                        ival = base64decoder.getInt64();
                        বিরতি;
                    ক্ষেত্র CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        গ্রন্থি_type = File_গ্রন্থি::REAL;
                    }
                        বিরতি;
                    ক্ষেত্র CV_64F:
                        fval = base64decoder.getFloat64();
                        গ্রন্থি_type = File_গ্রন্থি::REAL;
                        বিরতি;
                    ক্ষেত্র CV_16F:
                        fval = দশমিক(hfloatFromBits(base64decoder.getUInt16()));
                        গ্রন্থি_type = File_গ্রন্থি::REAL;
                        বিরতি;
                    ক্ষেত্র CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        গ্রন্থি_type = File_গ্রন্থি::REAL;
                        বিরতি;
                    ক্ষেত্র CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        গ্রন্থি_type = File_গ্রন্থি::REAL;
                        বিরতি;
                    পূর্বনির্ধারিত:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                যদি (base64decoder.endOfStream())
                    বিরতি;
                add_গ্রন্থি(collection, std::string(), গ্রন্থি_type,
                        গ্রন্থি_type == File_গ্রন্থি::INT ? (শূন্য *) &ival : (শূন্য *) &fval, -1);
            }
        }
        যদি (base64decoder.endOfStream())
            বিরতি;
    }

    finalizeCollection(collection);
    ফেরত base64decoder.getPtr();
}

শূন্য FileStorage::Impl::parseError(ধ্রুবক অক্ষর *func_name, ধ্রুবক std::string &err_বার্তা, ধ্রুবক অক্ষর *source_file,
                                   পূর্ণসংখ্যা source_সারি) {
    std::string বার্তা = format("%s(%d): %s", filename.c_str(), lineno, err_বার্তা.c_str());
    error(Error::StsParseError, func_name, বার্তা.c_str(), source_file, source_সারি);
}

ধ্রুবক uchar *FileStorage::Impl::get_গ্রন্থি_Ptr(size_t blockIdx, size_t ofs) ধ্রুবক {
    CV_Assert(blockIdx < fs_তথ্য_ptrs.size());
    CV_Assert(ofs < fs_তথ্য_blksz[blockIdx]);

    ফেরত fs_তথ্য_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) ধ্রুবক {
    CV_Assert(nameofs < str_hash_তথ্য.size());
    ফেরত std::string(&str_hash_তথ্য[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { ফেরত fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(এই);
}

FileStorage::FileStorage(ধ্রুবক String& filename, পূর্ণসংখ্যা flags, ধ্রুবক String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(এই);
    যৌক্তিক ok = p->open(filename.c_str(), flags, encoding.c_str());
    যদি(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

শূন্য FileStorage::startWriteStruct(ধ্রুবক String& name, পূর্ণসংখ্যা struct_flags, ধ্রুবক String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    যদি ((struct_flags & File_গ্রন্থি::TYPE_MASK) == File_গ্রন্থি::SEQ)
        state = FileStorage::মান_EXPECTED;
    অন্যথা
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

শূন্য FileStorage::endWriteStruct()
{
    p->endWriteStruct();
    state = p->write_স্তূপ.empty() || File_গ্রন্থি::isMap(p->write_স্তূপ.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::মান_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

যৌক্তিক FileStorage::open(ধ্রুবক String& filename, পূর্ণসংখ্যা flags, ধ্রুবক String& encoding)
{
    চেষ্টা
    {
        যৌক্তিক ok = p->open(filename.c_str(), flags, encoding.c_str());
        যদি(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        ফেরত ok;
    }
    ধরো (...)
    {
        release();
        ছোড়ো;  // re-throw
    }
}

যৌক্তিক FileStorage::isOpened() ধ্রুবক { ফেরত p->is_opened; }

শূন্য FileStorage::release()
{
    p->release();
}

File_গ্রন্থি FileStorage::মূল(পূর্ণসংখ্যা i) ধ্রুবক
{
    যদি( p.empty() || p->roots.empty() || i < 0 || i >= (পূর্ণসংখ্যা)p->roots.size() )
        ফেরত File_গ্রন্থি();

    ফেরত p->roots[i];
}

File_গ্রন্থি FileStorage::get_First_শীর্ষ_Level_গ্রন্থি() ধ্রুবক
{
    File_গ্রন্থি r = মূল();
    File_গ্রন্থি_Iterator it = r.begin();
    ফেরত it != r.end() ? *it : File_গ্রন্থি();
}

std::string FileStorage::getDefaultObjectName(ধ্রুবক std::string& _filename)
{
    স্থির ধ্রুবক অক্ষর* stubname = "unnamed";
    ধ্রুবক অক্ষর* filename = _filename.c_str();
    ধ্রুবক অক্ষর* ptr2 = filename + _filename.size();
    ধ্রুবক অক্ষর* ptr = ptr2 - 1;
    cv::Auto_বাফার<অক্ষর> name_buf(_filename.size()+1);

    যখন( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        যদি( *ptr == '.' && (!*ptr2 || strncmp(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    যদি( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    অক্ষর* name = name_buf.তথ্য();

    // name must start with letter or '_'
    যদি( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    যখন( ptr < ptr2 )
    {
        অক্ষর c = *ptr++;
        যদি( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.তথ্য();
    যদি( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    ফেরত name;
}


পূর্ণসংখ্যা FileStorage::getFormat() ধ্রুবক
{
    ফেরত p->fmt;
}

File_গ্রন্থি FileStorage::operator [](ধ্রুবক অক্ষর* চাবি) ধ্রুবক
{
    ফেরত এই->operator[](std::string(চাবি));
}

File_গ্রন্থি FileStorage::operator [](ধ্রুবক std::string& চাবি) ধ্রুবক
{
    File_গ্রন্থি res;
    জন্য (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][চাবি];
        যদি (!res.empty())
            বিরতি;
    }
    ফেরত res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    ফেরত buf;
}

শূন্য FileStorage::writeRaw( ধ্রুবক String& fmt, ধ্রুবক শূন্য* vec, size_t len )
{
    p->write_Raw_তথ্য(fmt, (ধ্রুবক uchar*)vec, len);
}

শূন্য FileStorage::writeComment( ধ্রুবক String& comment, যৌক্তিক eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

শূন্য writeScalar( FileStorage& fs, পূর্ণসংখ্যা value )
{
    fs.p->লিখ(String(), value);
}

শূন্য writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->লিখ(String(), value);
}

শূন্য writeScalar( FileStorage& fs, দশমিক value )
{
    fs.p->লিখ(String(), (দ্বিগুণ)value);
}

শূন্য writeScalar( FileStorage& fs, দ্বিগুণ value )
{
    fs.p->লিখ(String(), value);
}

শূন্য writeScalar( FileStorage& fs, ধ্রুবক String& value )
{
    fs.p->লিখ(String(), value);
}

শূন্য লিখ( FileStorage& fs, ধ্রুবক String& name, পূর্ণসংখ্যা value )
{
    fs.p->লিখ(name, value);
}

শূন্য লিখ( FileStorage& fs, ধ্রুবক String& name, int64_t value )
{
    fs.p->লিখ(name, value);
}

শূন্য লিখ( FileStorage& fs, ধ্রুবক String& name, দশমিক value )
{
    fs.p->লিখ(name, (দ্বিগুণ)value);
}

শূন্য লিখ( FileStorage& fs, ধ্রুবক String& name, দ্বিগুণ value )
{
    fs.p->লিখ(name, value);
}

শূন্য লিখ( FileStorage& fs, ধ্রুবক String& name, ধ্রুবক String& value )
{
    fs.p->লিখ(name, value);
}

শূন্য লিখ( FileStorage& fs, ধ্রুবক String& name, যৌক্তিক value )
{
    fs.p->লিখ(name, value);
}

শূন্য FileStorage::লিখ(ধ্রুবক String& name, পূর্ণসংখ্যা val) { p->লিখ(name, val); }
শূন্য FileStorage::লিখ(ধ্রুবক String& name, যৌক্তিক val) { p->লিখ(name, val); }
শূন্য FileStorage::লিখ(ধ্রুবক String& name, int64_t val) { p->লিখ(name, val); }
শূন্য FileStorage::লিখ(ধ্রুবক String& name, দ্বিগুণ val) { p->লিখ(name, val); }
শূন্য FileStorage::লিখ(ধ্রুবক String& name, ধ্রুবক String& val) { p->লিখ(name, val); }
শূন্য FileStorage::লিখ(ধ্রুবক String& name, ধ্রুবক Mat& val) { cv::লিখ(*এই, name, val); }
শূন্য FileStorage::লিখ(ধ্রুবক String& name, ধ্রুবক std::vector<String>& val) { cv::লিখ(*এই, name, val); }

FileStorage& operator << (FileStorage& fs, ধ্রুবক String& str)
{
    গণনা { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        মান_EXPECTED = FileStorage::মান_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    ধ্রুবক অক্ষর* _str = str.c_str();
    যদি( !fs.isOpened() || !_str )
        ফেরত fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    অক্ষর c = *_str;

    যদি( c == '}' || c == ']' )
    {
        যদি( fs_impl->write_স্তূপ.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        পূর্ণসংখ্যা struct_flags = fs_impl->write_স্তূপ.back().flags;
        অক্ষর expected_bracket = File_গ্রন্থি::isMap(struct_flags) ? '}' : ']';
        যদি( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_স্তূপ.empty());
        struct_flags = fs_impl->write_স্তূপ.back().flags;
        fs.state = File_গ্রন্থি::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : মান_EXPECTED;
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
            পূর্ণসংখ্যা struct_flags = c == '{' ? File_গ্রন্থি::MAP : File_গ্রন্থি::SEQ;
            fs.state = struct_flags == File_গ্রন্থি::MAP ? INSIDE_MAP + NAME_EXPECTED : মান_EXPECTED;
            _str++;
            যদি( *_str == ':' )
            {
                _str++;
                যদি( !*_str )
                    struct_flags |= File_গ্রন্থি::FLOW;
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
    ফেরত fs;
}


File_গ্রন্থি::File_গ্রন্থি()
    : fs(ফাঁকা)
{
    blockIdx = ofs = 0;
}

File_গ্রন্থি::File_গ্রন্থি(FileStorage::Impl* _fs, size_t _blockIdx, size_t _ofs)
    : fs(_fs)
{
    blockIdx = _blockIdx;
    ofs = _ofs;
}

File_গ্রন্থি::File_গ্রন্থি(ধ্রুবক FileStorage* _fs, size_t _blockIdx, size_t _ofs)
    : File_গ্রন্থি(_fs->p.get(), _blockIdx, _ofs)
{
    // nothing
}

File_গ্রন্থি::File_গ্রন্থি(ধ্রুবক File_গ্রন্থি& গ্রন্থি)
{
    fs = গ্রন্থি.fs;
    blockIdx = গ্রন্থি.blockIdx;
    ofs = গ্রন্থি.ofs;
}

File_গ্রন্থি& File_গ্রন্থি::operator=(ধ্রুবক File_গ্রন্থি& গ্রন্থি)
{
    fs = গ্রন্থি.fs;
    blockIdx = গ্রন্থি.blockIdx;
    ofs = গ্রন্থি.ofs;
    ফেরত *এই;
}

File_গ্রন্থি File_গ্রন্থি::operator[](ধ্রুবক std::string& nodename) ধ্রুবক
{
    যদি(!fs)
        ফেরত File_গ্রন্থি();

    CV_Assert( isMap() );

    unsigned চাবি = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_গ্রন্থি_Iterator it = begin();

    জন্য( i = 0; i < sz; i++, ++it )
    {
        File_গ্রন্থি n = *it;
        ধ্রুবক uchar* p = n.ptr();
        unsigned key2 = (unsigned)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_তথ্য.size() );
        যদি( চাবি == key2 )
            ফেরত n;
    }
    ফেরত File_গ্রন্থি();
}

File_গ্রন্থি File_গ্রন্থি::operator[](ধ্রুবক অক্ষর* nodename) ধ্রুবক
{
    ফেরত এই->operator[](std::string(nodename));
}

File_গ্রন্থি File_গ্রন্থি::operator[](পূর্ণসংখ্যা i) ধ্রুবক
{
    যদি(!fs)
        ফেরত File_গ্রন্থি();

    CV_Assert( isSeq() );

    পূর্ণসংখ্যা sz = (পূর্ণসংখ্যা)size();
    CV_Assert( 0 <= i && i < sz );

    File_গ্রন্থি_Iterator it = begin();
    it += i;

    ফেরত *it;
}

std::vector<String> File_গ্রন্থি::চাবিগুলি() ধ্রুবক
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    জন্য (File_গ্রন্থি_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    ফেরত res;
}

পূর্ণসংখ্যা File_গ্রন্থি::type() ধ্রুবক
{
    ধ্রুবক uchar* p = ptr();
    যদি(!p)
        ফেরত NONE;
    ফেরত (*p & TYPE_MASK);
}

যৌক্তিক File_গ্রন্থি::isMap(পূর্ণসংখ্যা flags) { ফেরত (flags & TYPE_MASK) == MAP; }
যৌক্তিক File_গ্রন্থি::isSeq(পূর্ণসংখ্যা flags) { ফেরত (flags & TYPE_MASK) == SEQ; }
যৌক্তিক File_গ্রন্থি::isCollection(পূর্ণসংখ্যা flags) { ফেরত isMap(flags) || isSeq(flags); }
যৌক্তিক File_গ্রন্থি::isFlow(পূর্ণসংখ্যা flags) { ফেরত (flags & FLOW) != 0; }
যৌক্তিক File_গ্রন্থি::isEmptyCollection(পূর্ণসংখ্যা flags) { ফেরত (flags & EMPTY) != 0; }

যৌক্তিক File_গ্রন্থি::empty() ধ্রুবক   { ফেরত fs == 0; }
যৌক্তিক File_গ্রন্থি::isNone() ধ্রুবক  { ফেরত type() == NONE; }
যৌক্তিক File_গ্রন্থি::isSeq() ধ্রুবক   { ফেরত type() == SEQ; }
যৌক্তিক File_গ্রন্থি::isMap() ধ্রুবক   { ফেরত type() == MAP; }
যৌক্তিক File_গ্রন্থি::isInt() ধ্রুবক   { ফেরত type() == INT;  }
যৌক্তিক File_গ্রন্থি::isReal() ধ্রুবক  { ফেরত type() == REAL; }
যৌক্তিক File_গ্রন্থি::isString() ধ্রুবক { ফেরত type() == STRING;  }
যৌক্তিক File_গ্রন্থি::isNamed() ধ্রুবক
{
    ধ্রুবক uchar* p = ptr();
    যদি(!p)
        ফেরত মিথ্যা;
    ফেরত (*p & NAMED) != 0;
}

std::string File_গ্রন্থি::name() ধ্রুবক
{
    ধ্রুবক uchar* p = ptr();
    যদি(!p)
        ফেরত std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    ফেরত fs->getName(nameofs);
}

File_গ্রন্থি::operator পূর্ণসংখ্যা() ধ্রুবক
{
    ধ্রুবক uchar* p = ptr();
    যদি(!p)
        ফেরত 0;
    পূর্ণসংখ্যা tag = *p;
    পূর্ণসংখ্যা type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    যদি( type == INT )
    {
        ফেরত readInt(p);
    }
    অন্যথা যদি( type == REAL )
    {
        ফেরত cvRound(readReal(p));
    }
    অন্যথা
        ফেরত 0x7fffffff;
}

File_গ্রন্থি::operator int64_t() ধ্রুবক
{
    ধ্রুবক uchar* p = ptr();
    যদি(!p)
        ফেরত 0;
    পূর্ণসংখ্যা tag = *p;
    পূর্ণসংখ্যা type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    যদি( type == INT )
    {
        ফেরত readLong(p);
    }
    অন্যথা যদি( type == REAL )
    {
        ফেরত cvRound(readReal(p));
    }
    অন্যথা
        ফেরত 0x7fffffff;
}

File_গ্রন্থি::operator দশমিক() ধ্রুবক
{
    ধ্রুবক uchar* p = ptr();
    যদি(!p)
        ফেরত 0.f;
    পূর্ণসংখ্যা tag = *p;
    পূর্ণসংখ্যা type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    যদি( type == INT )
    {
        ফেরত (দশমিক)readLong(p);
    }
    অন্যথা যদি( type == REAL )
    {
        ফেরত (দশমিক)readReal(p);
    }
    অন্যথা
        ফেরত FLT_MAX;
}

File_গ্রন্থি::operator দ্বিগুণ() ধ্রুবক
{
    ধ্রুবক uchar* p = ptr();
    যদি(!p)
        ফেরত 0.f;
    পূর্ণসংখ্যা tag = *p;
    পূর্ণসংখ্যা type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    যদি( type == INT )
    {
        ফেরত (দ্বিগুণ)readLong(p);
    }
    অন্যথা যদি( type == REAL )
    {
        ফেরত readReal(p);
    }
    অন্যথা
        ফেরত DBL_MAX;
}

দ্বিগুণ File_গ্রন্থি::real() ধ্রুবক  { ফেরত দ্বিগুণ(*এই); }

std::string File_গ্রন্থি::string() ধ্রুবক
{
    ধ্রুবক uchar* p = ptr();
    যদি( !p || (*p & TYPE_MASK) != STRING )
        ফেরত std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(unsigned)readInt(p);
    ফেরত std::string((ধ্রুবক অক্ষর*)(p + 4), sz - 1);
}
Mat File_গ্রন্থি::mat() ধ্রুবক { Mat value; পঠ(*এই, value, Mat()); ফেরত value; }

File_গ্রন্থি_Iterator File_গ্রন্থি::begin() ধ্রুবক { ফেরত File_গ্রন্থি_Iterator(*এই, মিথ্যা); }
File_গ্রন্থি_Iterator File_গ্রন্থি::end() ধ্রুবক   { ফেরত File_গ্রন্থি_Iterator(*এই, সত্য); }

শূন্য File_গ্রন্থি::readRaw( ধ্রুবক std::string& fmt, শূন্য* vec, size_t len ) ধ্রুবক
{
    File_গ্রন্থি_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_গ্রন্থি::size() ধ্রুবক
{
    ধ্রুবক uchar* p = ptr();
    যদি( !p )
        ফেরত 0;
    পূর্ণসংখ্যা tag = *p;
    পূর্ণসংখ্যা tp = tag & TYPE_MASK;
    যদি( tp == MAP || tp == SEQ )
    {
        যদি( tag & NAMED )
            p += 4;
        ফেরত (size_t)(unsigned)readInt(p + 5);
    }
    ফেরত tp != NONE;
}

size_t File_গ্রন্থি::raw_আকার() ধ্রুবক
{
    ধ্রুবক uchar* p0 = ptr(), *p = p0;
    যদি( !p )
        ফেরত 0;
    পূর্ণসংখ্যা tag = *p++;
    পূর্ণসংখ্যা tp = tag & TYPE_MASK;
    যদি( tag & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    যদি( tp == INT )
        ফেরত sz0 + 8;
    যদি( tp == REAL )
        ফেরত sz0 + 8;
    যদি( tp == NONE )
        ফেরত sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    ফেরত sz0 + 4 + readInt(p);
}

uchar* File_গ্রন্থি::ptr()
{
    ফেরত !fs ? 0 : (uchar*)fs->get_গ্রন্থি_Ptr(blockIdx, ofs);
}

ধ্রুবক uchar* File_গ্রন্থি::ptr() ধ্রুবক
{
    ফেরত !fs ? 0 : fs->get_গ্রন্থি_Ptr(blockIdx, ofs);
}

শূন্য File_গ্রন্থি::set_মান( পূর্ণসংখ্যা type, ধ্রুবক শূন্য* value, পূর্ণসংখ্যা len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    পূর্ণসংখ্যা tag = *p;
    পূর্ণসংখ্যা current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    পূর্ণসংখ্যা sz = 1;

    যদি( tag & NAMED )
        sz += 4;

    যদি( type == INT )
        sz += 8;
    অন্যথা যদি( type == REAL )
        sz += 8;
    অন্যথা যদি( type == STRING )
    {
        যদি( len < 0 )
            len = (পূর্ণসংখ্যা)strlen((ধ্রুবক অক্ষর*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    অন্যথা
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_গ্রন্থি_Space(*এই, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    যদি( tag & NAMED )
        p += 4;

    যদি( type == INT )
    {
        int64_t ival = *(ধ্রুবক int64_t*)value;
        writeInt(p, ival);
    }
    অন্যথা যদি( type == REAL )
    {
        দ্বিগুণ dbval = *(ধ্রুবক দ্বিগুণ*)value;
        writeReal(p, dbval);
    }
    অন্যথা যদি( type == STRING )
    {
        ধ্রুবক অক্ষর* str = (ধ্রুবক অক্ষর*)value;
        writeInt(p, len + 1);
        memcpy(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_গ্রন্থি_Iterator::File_গ্রন্থি_Iterator()
{
    fs = 0;
    blockIdx = 0;
    ofs = 0;
    block_আকার = 0;
    গ্রন্থি_NElems = 0;
    idx = 0;
}

File_গ্রন্থি_Iterator::File_গ্রন্থি_Iterator( ধ্রুবক File_গ্রন্থি& গ্রন্থি, যৌক্তিক seekEnd )
{
    fs = গ্রন্থি.fs;
    idx = 0;
    যদি( !fs )
        blockIdx = ofs = block_আকার = গ্রন্থি_NElems = 0;
    অন্যথা
    {
        blockIdx = গ্রন্থি.blockIdx;
        ofs = গ্রন্থি.ofs;

        যৌক্তিক collection = গ্রন্থি.isSeq() || গ্রন্থি.isMap();
        যদি( গ্রন্থি.isNone() )
        {
            গ্রন্থি_NElems = 0;
        }
        অন্যথা যদি( !collection )
        {
            গ্রন্থি_NElems = 1;
            যদি( seekEnd )
            {
                idx = 1;
                ofs += গ্রন্থি.raw_আকার();
            }
        }
        অন্যথা
        {
            গ্রন্থি_NElems = গ্রন্থি.size();
            ধ্রুবক uchar* p0 = গ্রন্থি.ptr(), *p = p0 + 1;
            যদি(*p0 & File_গ্রন্থি::NAMED )
                p += 4;
            যদি( !seekEnd )
                ofs += (p - p0) + 8;
            অন্যথা
            {
                size_t rawsz = (size_t)(unsigned)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = গ্রন্থি_NElems;
            }
        }
        fs->normalize_গ্রন্থি_Ofs(blockIdx, ofs);
        block_আকার = fs->fs_তথ্য_blksz[blockIdx];
    }
}

File_গ্রন্থি_Iterator::File_গ্রন্থি_Iterator(ধ্রুবক File_গ্রন্থি_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    block_আকার = it.block_আকার;
    গ্রন্থি_NElems = it.গ্রন্থি_NElems;
    idx = it.idx;
}

File_গ্রন্থি_Iterator& File_গ্রন্থি_Iterator::operator=(ধ্রুবক File_গ্রন্থি_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    block_আকার = it.block_আকার;
    গ্রন্থি_NElems = it.গ্রন্থি_NElems;
    idx = it.idx;
    ফেরত *এই;
}

File_গ্রন্থি File_গ্রন্থি_Iterator::operator *() ধ্রুবক
{
    ফেরত File_গ্রন্থি(idx < গ্রন্থি_NElems ? fs : ফাঁকা, blockIdx, ofs);
}

File_গ্রন্থি_Iterator& File_গ্রন্থি_Iterator::operator ++ ()
{
    যদি( idx == গ্রন্থি_NElems || !fs )
        ফেরত *এই;
    idx++;
    File_গ্রন্থি n(fs, blockIdx, ofs);
    ofs += n.raw_আকার();
    যদি( ofs >= block_আকার )
    {
        fs->normalize_গ্রন্থি_Ofs(blockIdx, ofs);
        block_আকার = fs->fs_তথ্য_blksz[blockIdx];
    }
    ফেরত *এই;
}

File_গ্রন্থি_Iterator File_গ্রন্থি_Iterator::operator ++ (পূর্ণসংখ্যা)
{
    File_গ্রন্থি_Iterator it = *এই;
    ++(*এই);
    ফেরত it;
}

File_গ্রন্থি_Iterator& File_গ্রন্থি_Iterator::operator += (পূর্ণসংখ্যা _ofs)
{
    CV_Assert( _ofs >= 0 );
    জন্য( ; _ofs > 0; _ofs-- )
        এই->operator ++();
    ফেরত *এই;
}

File_গ্রন্থি_Iterator& File_গ্রন্থি_Iterator::readRaw( ধ্রুবক String& fmt, শূন্য* _data0, size_t maxsz)
{
    যদি( fs && idx < গ্রন্থি_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        পূর্ণসংখ্যা fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        পূর্ণসংখ্যা fmt_pair_গুনতি = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        size_t esz = fs::calc_Struct_আকার( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        জন্য( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            জন্য( পূর্ণসংখ্যা k = 0; k < fmt_pair_গুনতি; k++ )
            {
                পূর্ণসংখ্যা elem_type = fmt_pairs[k*2+1];
                পূর্ণসংখ্যা elem_আকার_2 = CV_ELEM_আকার(elem_type);

                পূর্ণসংখ্যা গুনতি_2 = fmt_pairs[k*2];
                offset = align_আকার( offset, elem_আকার_2 );
                uchar* তথ্য = data0 + offset;

                জন্য( পূর্ণসংখ্যা i = 0; i < গুনতি_2; i++, ++(*এই) )
                {
                    File_গ্রন্থি গ্রন্থি = *(*এই);
                    যদি( গ্রন্থি.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_আকার_2 == 8 ? (int64_t)গ্রন্থি : (পূর্ণসংখ্যা)গ্রন্থি);
                        নির্বাচন( elem_type )
                        {
                        ক্ষেত্র CV_8U:
                            *(uchar*)তথ্য = saturate_cast<uchar>(ival);
                            তথ্য++;
                            বিরতি;
                        ক্ষেত্র CV_8S:
                            *(অক্ষর*)তথ্য = saturate_cast<schar>(ival);
                            তথ্য++;
                            বিরতি;
                        ক্ষেত্র CV_Bool:
                            *(যৌক্তিক*)তথ্য = ival != 0;
                            তথ্য++;
                            বিরতি;
                        ক্ষেত্র CV_16U:
                            *(ushort*)তথ্য = saturate_cast<ushort>(ival);
                            তথ্য += আকার(ushort);
                            বিরতি;
                        ক্ষেত্র CV_16S:
                            *(short*)তথ্য = saturate_cast<short>(ival);
                            তথ্য += আকার(short);
                            বিরতি;
                        ক্ষেত্র CV_32U:
                            *(unsigned*)তথ্য = (unsigned)std::max(ival, (int64_t)0);
                            তথ্য += আকার(unsigned);
                            বিরতি;
                        ক্ষেত্র CV_32S:
                            *(পূর্ণসংখ্যা*)তথ্য = (পূর্ণসংখ্যা)ival;
                            তথ্য += আকার(পূর্ণসংখ্যা);
                            বিরতি;
                        ক্ষেত্র CV_32F:
                            *(দশমিক*)তথ্য = (দশমিক)ival;
                            তথ্য += আকার(দশমিক);
                            বিরতি;
                        ক্ষেত্র CV_64U:
                            *(uint64_t*)তথ্য = (uint64_t)ival;
                            তথ্য += আকার(uint64_t);
                            বিরতি;
                        ক্ষেত্র CV_64S:
                            *(int64_t*)তথ্য = (int64_t)ival;
                            তথ্য += আকার(int64_t);
                            বিরতি;
                        ক্ষেত্র CV_64F:
                            *(দ্বিগুণ*)তথ্য = (দ্বিগুণ)ival;
                            তথ্য += আকার(দ্বিগুণ);
                            বিরতি;
                        ক্ষেত্র CV_16F:
                            *(hfloat*)তথ্য = hfloat((দশমিক)ival);
                            তথ্য += আকার(hfloat);
                            বিরতি;
                        ক্ষেত্র CV_16BF:
                            *(bfloat*)তথ্য = bfloat((দশমিক)ival);
                            তথ্য += আকার(bfloat);
                            বিরতি;
                        ক্ষেত্র CV_8F_E4M3FN:
                            *(fp8_t*)তথ্য = fp8_t((দশমিক)ival);
                            তথ্য += আকার(fp8_t);
                            বিরতি;
                        ক্ষেত্র CV_8F_E4M3FNUZ:
                            *(fp8a_t*)তথ্য = fp8a_t((দশমিক)ival);
                            তথ্য += আকার(fp8a_t);
                            বিরতি;
                        পূর্বনির্ধারিত:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    অন্যথা যদি( গ্রন্থি.isReal() )
                    {
                        দ্বিগুণ fval = (দ্বিগুণ)গ্রন্থি;

                        নির্বাচন( elem_type )
                        {
                        ক্ষেত্র CV_8U:
                            *(uchar*)তথ্য = saturate_cast<uchar>(fval);
                            তথ্য++;
                            বিরতি;
                        ক্ষেত্র CV_8S:
                            *(অক্ষর*)তথ্য = saturate_cast<schar>(fval);
                            তথ্য++;
                            বিরতি;
                        ক্ষেত্র CV_16U:
                            *(ushort*)তথ্য = saturate_cast<ushort>(fval);
                            তথ্য += আকার(ushort);
                            বিরতি;
                        ক্ষেত্র CV_16S:
                            *(short*)তথ্য = saturate_cast<short>(fval);
                            তথ্য += আকার(short);
                            বিরতি;
                        ক্ষেত্র CV_32U:
                            *(পূর্ণসংখ্যা*)তথ্য = saturate_cast<unsigned>(fval);
                            তথ্য += আকার(পূর্ণসংখ্যা);
                            বিরতি;
                        ক্ষেত্র CV_32S:
                            *(পূর্ণসংখ্যা*)তথ্য = saturate_cast<পূর্ণসংখ্যা>(fval);
                            তথ্য += আকার(পূর্ণসংখ্যা);
                            বিরতি;
                        ক্ষেত্র CV_32F:
                            *(দশমিক*)তথ্য = (দশমিক)fval;
                            তথ্য += আকার(দশমিক);
                            বিরতি;
                        ক্ষেত্র CV_64U:
                            *(uint64_t*)তথ্য = (uint64_t)round(std::max(fval, 0.));
                            তথ্য += আকার(uint64_t);
                            বিরতি;
                        ক্ষেত্র CV_64S:
                            *(int64_t*)তথ্য = (int64_t)round(std::max(fval, 0.));
                            তথ্য += আকার(int64_t);
                            বিরতি;
                        ক্ষেত্র CV_64F:
                            *(দ্বিগুণ*)তথ্য = fval;
                            তথ্য += আকার(দ্বিগুণ);
                            বিরতি;
                        ক্ষেত্র CV_16F:
                            *(hfloat*)তথ্য = hfloat((দশমিক)fval);
                            তথ্য += আকার(hfloat);
                            বিরতি;
                        ক্ষেত্র CV_16BF:
                            *(bfloat*)তথ্য = bfloat((দশমিক)fval);
                            তথ্য += আকার(bfloat);
                            বিরতি;
                        ক্ষেত্র CV_8F_E4M3FN:
                            *(fp8_t*)তথ্য = fp8_t((দশমিক)fval);
                            তথ্য += আকার(fp8_t);
                            বিরতি;
                        ক্ষেত্র CV_8F_E4M3FNUZ:
                            *(fp8a_t*)তথ্য = fp8a_t((দশমিক)fval);
                            তথ্য += আকার(fp8a_t);
                            বিরতি;
                        পূর্বনির্ধারিত:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    অন্যথা
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (পূর্ণসংখ্যা)(তথ্য - data0);
            }
        }
    }

    ফেরত *এই;
}

যৌক্তিক File_গ্রন্থি_Iterator::equalTo(ধ্রুবক File_গ্রন্থি_Iterator& it) ধ্রুবক
{
    ফেরত fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && গ্রন্থি_NElems == it.গ্রন্থি_NElems;
}

size_t File_গ্রন্থি_Iterator::remaining() ধ্রুবক
{
    ফেরত গ্রন্থি_NElems - idx;
}

যৌক্তিক operator == ( ধ্রুবক File_গ্রন্থি_Iterator& it1, ধ্রুবক File_গ্রন্থি_Iterator& it2 )
{
    ফেরত it1.equalTo(it2);
}

যৌক্তিক operator != ( ধ্রুবক File_গ্রন্থি_Iterator& it1, ধ্রুবক File_গ্রন্থি_Iterator& it2 )
{
    ফেরত !it1.equalTo(it2);
}

শূন্য পঠ(ধ্রুবক File_গ্রন্থি& গ্রন্থি, পূর্ণসংখ্যা& val, পূর্ণসংখ্যা default_val)
{
    val = default_val;
    যদি( !গ্রন্থি.empty() )
    {
        val = (পূর্ণসংখ্যা)গ্রন্থি;
    }
}

শূন্য পঠ(ধ্রুবক File_গ্রন্থি& গ্রন্থি, int64_t& val, int64_t default_val)
{
    val = default_val;
    যদি( !গ্রন্থি.empty() )
    {
        val = (int64_t)গ্রন্থি;
    }
}

শূন্য পঠ(ধ্রুবক File_গ্রন্থি& গ্রন্থি, দ্বিগুণ& val, দ্বিগুণ default_val)
{
    val = default_val;
    যদি( !গ্রন্থি.empty() )
    {
        val = (দ্বিগুণ)গ্রন্থি;
    }
}

শূন্য পঠ(ধ্রুবক File_গ্রন্থি& গ্রন্থি, দশমিক& val, দশমিক default_val)
{
    val = default_val;
    যদি( !গ্রন্থি.empty() )
    {
        val = (দশমিক)গ্রন্থি;
    }
}

শূন্য পঠ(ধ্রুবক File_গ্রন্থি& গ্রন্থি, std::string& val, ধ্রুবক std::string& default_val)
{
    val = default_val;
    যদি( !গ্রন্থি.empty() )
    {
        val = (std::string)গ্রন্থি;
    }
}
শূন্য FileStorage::Impl::লিখ(ধ্রুবক String &চাবি, যৌক্তিক value)
{
    CV_Assert(write_mode);
    যদি (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().লিখ(চাবি.c_str(), (পূর্ণসংখ্যা)value);
    }
    অন্যথা
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().লিখ(চাবি.c_str(), value ? "true" : "false", মিথ্যা);
    }
}

FileStorage_API::~FileStorage_API() {}

নামাকাশ internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, ধ্রুবক std::string& name,
                                       পূর্ণসংখ্যা flags, ধ্রুবক std::string& typeName)
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
