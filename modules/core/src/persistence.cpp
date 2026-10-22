// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "persistence.hpp"
#ضمن "persistence_impl.hpp"
#ضمن "persistence_base64_encoding.hpp"
#ضمن <unordered_map>
#ضمن <iterator>

#ضمن <opencv2/core/utils/logger.hpp>

#define CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

نطاق cv
{

نطاق fs
{

صحيح strcasecmp(ثابت حرف* s1, ثابت حرف* s2)
{
    ثابت حرف* dummy="";
    𐡄𐡍(!s1) s1=dummy;
    𐡄𐡍(!s2) s2=dummy;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t i, len = std::min(len1, len2);
    لكل( i = 0; i < len; i++ )
    {
        صحيح d = tolower((صحيح)s1[i]) - tolower((صحيح)s2[i]);
        𐡄𐡍( d != 0 )
            أرجع d;
    }
    أرجع len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

حرف* itoa( صحيح _val, حرف* buffer, صحيح /*radix*/ )
{
    ثابت صحيح radix = 10;
    حرف* ptr=buffer + 23 /* enough even for 64-bit integers */;
    unsigned val = abs(_val);

    *ptr = '\0';
    𐡏𐡁𐡃
    {
        unsigned r = val / radix;
        *--ptr = (حرف)(val - (r*radix) + '0');
        val = r;
    }
    طالما( val != 0 );

    𐡄𐡍( _val < 0 )
        *--ptr = '-';

    أرجع ptr;
}

حرف* itoa( int64_t _val, حرف* buffer, صحيح /*radix*/, منطقي _signed)
{
    ثابت صحيح radix = 10;
    حرف* ptr=buffer + 23 /* enough even for 64-bit integers */;
    صحيح sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : abs(_val);

    *ptr = '\0';
    𐡏𐡁𐡃
    {
        uint64_t r = val / radix;
        *--ptr = (حرف)(val - (r*radix) + '0');
        val = r;
    }
    طالما( val != 0 );

    𐡄𐡍( sign < 0 )
        *--ptr = '-';

    أرجع ptr;
}

حرف* doubleToString( حرف* buf, size_t bufSize, مزدوج value, منطقي explicitZero )
{
    Cv64suf val;
    unsigned ieee754_hi;

    val.f = value;
    ieee754_hi = (unsigned)(val.u >> 32);

    𐡄𐡍( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        صحيح ivalue = cvRound(value);
        𐡄𐡍( ivalue == value )
        {
            𐡄𐡍( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            وإلا
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        وإلا
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, bufSize, "%.17g", value );

            حرف* ptr = buf;
            𐡄𐡍( *ptr == '+' || *ptr == '-' )
                ptr++;
            لكل( ; cv_isdigit(*ptr); ptr++ )
                ;
            𐡄𐡍( *ptr == ',' )
                *ptr = '.';
        }
    }
    وإلا
    {
        unsigned ieee754_lo = (unsigned)val.u;
        𐡄𐡍( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        وإلا
            strcpy( buf, (صحيح)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    أرجع buf;
}

حرف* floatToString( حرف* buf, size_t bufSize, عائم value, منطقي halfprecision, منطقي explicitZero )
{
    Cv32suf val;
    unsigned ieee754;
    val.f = value;
    ieee754 = val.u;

    𐡄𐡍( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        صحيح ivalue = cvRound(value);
        𐡄𐡍( ivalue == value )
        {
            𐡄𐡍( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            وإلا
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        وإلا
        {
            𐡄𐡍 (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.5g", value);
            }
            وإلا
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.9g", value);
            }

            حرف* ptr = buf;
            𐡄𐡍( *ptr == '+' || *ptr == '-' )
                ptr++;
            لكل( ; cv_isdigit(*ptr); ptr++ )
                ;
            𐡄𐡍( *ptr == ',' )
                *ptr = '.';
        }
    }
    وإلا
    {
        𐡄𐡍( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        وإلا
            strcpy( buf, (صحيح)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    أرجع buf;
}

ساكن ثابت حرف symbols[] = "ucwsifdhHbUIneE";

ساكن حرف typeSymbol(صحيح depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    أرجع symbols[depth];
}

ساكن صحيح symbolToType(حرف c)
{
    𐡄𐡍 (c == 'r')
        أرجع CV_SEQ_ELTYPE_PTR;
    ثابت حرف* pos = strchr( symbols, c );
    𐡄𐡍( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    أرجع static_cast<صحيح>(pos - symbols);
}

حرف* encodeFormat(صحيح elem_type, حرف* dt, size_t dt_len)
{
    صحيح cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    حرف symbol = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : typeSymbol(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, symbol);
    أرجع dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
حرف* encodeFormat(صحيح elem_type, حرف* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    أرجع encodeFormat(elem_type, dt, max);
}

صحيح decodeFormat( ثابت حرف* dt, صحيح* fmt_pairs, صحيح max_len )
{
    صحيح fmt_pair_count = 0;
    صحيح i = 0, k = 0, len = dt ? (صحيح)strlen(dt) : 0;

    𐡄𐡍( !dt || !len )
        أرجع 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    لكل( ; k < len; k++ )
    {
        حرف c = dt[k];

        𐡄𐡍( cv_isdigit(c) )
        {
            صحيح count = c - '0';
            𐡄𐡍( cv_isdigit(dt[k+1]) )
            {
                حرف* endptr = 0;
                count = (صحيح)strtol( dt+k, &endptr, 10 );
                k = (صحيح)(endptr - dt) - 1;
            }

            𐡄𐡍( count <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = count;
        }
        وإلا
        {
            صحيح depth = symbolToType(c);
            𐡄𐡍( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = depth;
            𐡄𐡍( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            وإلا
            {
                i += 2;
                𐡄𐡍( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_count = i/2;
    أرجع fmt_pair_count;
}

صحيح calcElemSize( ثابت حرف* dt, صحيح initial_size )
{
    صحيح size = 0;
    صحيح fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_count;
    صحيح comp_size;

    fmt_pair_count = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_count *= 2;
    لكل( i = 0, size = initial_size; i < fmt_pair_count; i += 2 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[i+1]);
        size = cvAlign( size, comp_size );
        size += comp_size * fmt_pairs[i];
    }
    𐡄𐡍( initial_size == 0 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[1]);
        size = cvAlign( size, comp_size );
    }
    أرجع size;
}


صحيح calcStructSize( ثابت حرف* dt, صحيح initial_size )
{
    صحيح size = calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    لكل ( ثابت حرف * type = dt; *type != '\0'; type++ )
    {
        حرف v = *type;
        𐡄𐡍 (v >= '0' && v <= '9')
            استمر;  // skip vector size
        بدل (v)
        {
        حالة 'u': { elem_max_size = std::max( elem_max_size, حجم(uchar ) ); اكسر; }
        حالة 'b': { elem_max_size = std::max( elem_max_size, حجم(منطقي  ) ); اكسر; }
        حالة 'c': { elem_max_size = std::max( elem_max_size, حجم(schar ) ); اكسر; }
        حالة 'w': { elem_max_size = std::max( elem_max_size, حجم(ushort) ); اكسر; }
        حالة 's': { elem_max_size = std::max( elem_max_size, حجم(short ) ); اكسر; }
        حالة 'i': { elem_max_size = std::max( elem_max_size, حجم(صحيح   ) ); اكسر; }
        حالة 'n': { elem_max_size = std::max( elem_max_size, حجم(unsigned) ); اكسر; }
        حالة 'f': { elem_max_size = std::max( elem_max_size, حجم(عائم ) ); اكسر; }
        حالة 'd': { elem_max_size = std::max( elem_max_size, حجم(مزدوج) ); اكسر; }
        حالة 'h': { elem_max_size = std::max( elem_max_size, حجم(hfloat)); اكسر; }
        حالة 'H': { elem_max_size = std::max( elem_max_size, حجم(bfloat)); اكسر; }
        حالة 'e': { elem_max_size = std::max( elem_max_size, حجم(fp8_t) ); اكسر; }
        حالة 'E': { elem_max_size = std::max( elem_max_size, حجم(fp8a_t)); اكسر; }
        حالة 'I': { elem_max_size = std::max( elem_max_size, حجم(int64_t)); اكسر; }
        حالة 'U': { elem_max_size = std::max( elem_max_size, حجم(uint64_t)); اكسر; }
        افتراضي:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (حرف)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<صحيح>(elem_max_size) );
    أرجع size;
}

صحيح decodeSimpleFormat( ثابت حرف* dt )
{
    صحيح elem_type = -1;
    صحيح fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_count;

    fmt_pair_count = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    𐡄𐡍( fmt_pair_count != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    أرجع elem_type;
}

}

#𐡄𐡍 defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#define CV_LITTLE_ENDIAN_MEM_ACCESS 1
#وإلا
#define CV_LITTLE_ENDIAN_MEM_ACCESS 0
#endif

ساكن مضمن صحيح readInt(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#𐡄𐡍 CV_LITTLE_ENDIAN_MEM_ACCESS
    صحيح val;
    memcpy(&val, p, حجم(val));
    أرجع val;
#وإلا
    صحيح val = (صحيح)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    أرجع val;
#endif
}

ساكن مضمن int64_t readLong(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#𐡄𐡍 CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    memcpy(&val, p, حجم(val));
    أرجع val;
#وإلا
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    أرجع val0 | ((int64_t)val1 << 32);
#endif
}

ساكن مضمن مزدوج readReal(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#𐡄𐡍 CV_LITTLE_ENDIAN_MEM_ACCESS
    مزدوج val;
    memcpy(&val, p, حجم(val));
    أرجع val;
#وإلا
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    أرجع val.f;
#endif
}

template <typename T>
ساكن مضمن فراغ writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#𐡄𐡍 CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &ival, حجم(ival));
#وإلا
    لكل (size_t i = 0, j = 0; i < حجم(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#endif
}

ساكن مضمن فراغ writeReal(uchar* p, مزدوج fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#𐡄𐡍 CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &fval, حجم(fval));
#وإلا
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



فراغ FileStorage::Impl::init() {
    flags = 0;
    buffer.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = خطأ;
    state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = خطأ;
    delayed_struct_key = nullptr;
    delayed_struct_flags = 0;
    delayed_type_name = nullptr;
    base64_writer = nullptr;
    is_opened = خطأ;
    dummy_eof = خطأ;
    write_mode = خطأ;
    mem_mode = خطأ;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    empty_stream = 𐡒𐡔𐡈;

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

فراغ FileStorage::Impl::release(String *out) {
    𐡄𐡍 (is_opened) {
        𐡄𐡍 (out)
            out->clear();
        𐡄𐡍 (write_mode) {
            طالما (write_stack.size() > 1) {
                endWriteStruct();
            }
            flush();
            𐡄𐡍 (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            وإلا 𐡄𐡍 (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        𐡄𐡍 (mem_mode && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

فراغ FileStorage::Impl::analyze_file_name(ثابت std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    ساكن ثابت حرف not_file_name = '\n';
    ساكن ثابت حرف parameter_begin = '?';
    ساكن ثابت حرف parameter_separator = '&';

    𐡄𐡍 (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        أرجع;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    𐡄𐡍 (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        لكل (size_t param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            𐡄𐡍 ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

منطقي FileStorage::Impl::open(ثابت حرف *filename_or_buf, صحيح _flags, ثابت حرف *encoding) {
    منطقي ok = 𐡒𐡔𐡈;
    release();

    منطقي append = (_flags & 3) == FileStorage::APPEND;
    mem_mode = (_flags & FileStorage::MEMORY) != 0;

    write_mode = (_flags & 3) != 0;
    منطقي write_base64 = (write_mode || append) && (_flags & FileStorage::BASE64) != 0;

    منطقي isGZ = خطأ;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        𐡄𐡍 (!params.empty())
            filename = params[0];

        𐡄𐡍 (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_mode || append);
    }

    𐡄𐡍 (filename.size() == 0 && !mem_mode && !write_mode)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    𐡄𐡍 (mem_mode && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    𐡄𐡍 (!mem_mode) {
        size_t dot_idx = filename.find_last_of('.');
        حرف compression = '\0';

        𐡄𐡍 (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            𐡄𐡍 (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                𐡄𐡍 (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = 𐡒𐡔𐡈;
                }

                وإلا 𐡄𐡍(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = 𐡒𐡔𐡈;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        𐡄𐡍 (!isGZ) {
            file = fopen(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            𐡄𐡍 (!file)
            {
                CV_LOG_ERROR(عدم, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                أرجع خطأ;
            }
        } وإلا {
#𐡄𐡍 USE_ZLIB
            𐡄𐡍 (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            حرف mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            𐡄𐡍 (!gzfile)
            {
                CV_LOG_ERROR(عدم, "Can't open archive: '" << filename << "' mode=" << mode);
                أرجع خطأ;
            }
#وإلا
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#endif
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_data.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    𐡄𐡍 (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        𐡄𐡍 (mem_mode)
            outbuf.clear();

        𐡄𐡍 (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            ثابت حرف *dot_pos = عدم;
            ثابت حرف *dot_pos2 = عدم;
            // like strrchr() implementation, but save two last positions simultaneously
            لكل (ثابت حرف *pos = &filename[0]; pos[0] != 0; pos++) {
                𐡄𐡍 (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            𐡄𐡍 (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != عدم) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } وإلا 𐡄𐡍 (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        صحيح buf_size = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        𐡄𐡍 (append) {
            fseek(file, 0, SEEK_END);
            𐡄𐡍 (ftell(file) == 0)
                append = خطأ;
        }

        write_stack.clear();
        empty_stream = 𐡒𐡔𐡈;
        write_stack.push_back(FStructData("", FileNode::MAP | FileNode::EMPTY, 0));
        buffer.reserve(buf_size + 1024);
        buffer.resize(buf_size);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;

        𐡄𐡍 (fmt == FileStorage::FORMAT_XML) {
            size_t file_size = file ? (size_t) ftell(file) : (size_t) 0;
            𐡄𐡍 (!append || file_size == 0) {
                𐡄𐡍 (encoding && *encoding != '\0') {
                    𐡄𐡍 (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(strlen(encoding) < 1000);
                    حرف buf[1100];
                    snprintf(buf, حجم(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } وإلا
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } وإلا {
                صحيح xml_buf_size = 1 << 10;
                حرف substr[] = "</opencv_storage>";
                صحيح last_occurrence = -1;
                xml_buf_size = MIN(xml_buf_size, صحيح(file_size));
                fseek(file, -xml_buf_size, SEEK_END);
                // find the last occurrence of </opencv_storage>
                لكل (;;) {
                    صحيح line_offset = (صحيح) ftell(file);
                    ثابت حرف *ptr0 = هذا->gets(xml_buf_size);
                    ثابت حرف *ptr = عدم;
                    𐡄𐡍 (!ptr0)
                        اكسر;
                    ptr = ptr0;
                    لكل (;;) {
                        ptr = strstr(ptr, substr);
                        𐡄𐡍 (!ptr)
                            اكسر;
                        last_occurrence = line_offset + (صحيح) (ptr - ptr0);
                        ptr += strlen(substr);
                    }
                }
                𐡄𐡍 (last_occurrence < 0) {
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

            emitter_do_not_use_direct_dereference = createXMLEmitter(هذا);
        } وإلا 𐡄𐡍 (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                𐡄𐡍 (!append) {
                    𐡄𐡍 (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    وإلا
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            وإلا
                puts("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(هذا);
        } وإلا {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            𐡄𐡍 (!append)
                puts("{\n");
            وإلا {
                منطقي valid = خطأ;
                long roffset = 0;
                لكل (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    ثابت حرف end_mark = '}';
                    𐡄𐡍 (fgetc(file) == end_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = 𐡒𐡔𐡈;
                        اكسر;
                    }
                }

                𐡄𐡍 (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } وإلا {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_stack.back().indent = 4;
            emitter_do_not_use_direct_dereference = createJSONEmitter(هذا);
        }
        is_opened = 𐡒𐡔𐡈;
    } وإلا {
        ثابت size_t buf_size0 = 40;
        buffer.resize(buf_size0);
        𐡄𐡍 (mem_mode) {
            strbuf = (حرف *) filename_or_buf;
            strbufsize = strlen(strbuf);
        }

        ثابت حرف *yaml_signature = "%YAML";
        ثابت حرف *json_signature = "{";
        ثابت حرف *xml_signature = "<?xml";
        حرف *buf = هذا->gets(16);
        CV_Assert(buf);
        حرف *bufPtr = cv_skip_BOM(buf);
        size_t bufOffset = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        𐡄𐡍 (strncmp(bufPtr, yaml_signature, strlen(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        وإلا 𐡄𐡍 (strncmp(bufPtr, json_signature, strlen(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        وإلا 𐡄𐡍 (strncmp(bufPtr, xml_signature, strlen(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        وإلا
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = bufOffset;
        bufofs = 0;

        حاول {
            حرف *ptr = bufferStart();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            FileNode root_nodes(fs_ext, 0, 0);

            uchar *rptr = reserveNodeSpace(root_nodes, 9);
            *rptr = FileNode::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            بدل (fmt) {
                حالة FileStorage::FORMAT_XML:
                    parser_do_not_use_direct_dereference = createXMLParser(هذا);
                    اكسر;
                حالة FileStorage::FORMAT_YAML:
                حالة FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_direct_dereference = createYAMLParser(هذا);
                    اكسر;
                حالة FileStorage::FORMAT_JSON:
                    parser_do_not_use_direct_dereference = createJSONParser(هذا);
                    اكسر;
                افتراضي:
                    parser_do_not_use_direct_dereference = Ptr<FileStorageParser>();
            }

            𐡄𐡍 (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                𐡄𐡍 (ok) {
                    finalizeCollection(root_nodes);

                    CV_Assert(!fs_data_ptrs.empty());
                    FileNode roots_node(fs_ext, 0, 0);
                    size_t i, nroots = roots_node.size();
                    FileNodeIterator it = roots_node.begin();

                    لكل (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        امسك (...)
        {
            // FIXIT log error message
            is_opened = 𐡒𐡔𐡈;
            release();
            ارم;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = 𐡒𐡔𐡈;
        std::vector<حرف> tmpbuf;
        std::swap(buffer, tmpbuf);
        bufofs = 0;
    }
    أرجع ok;
}

فراغ FileStorage::Impl::puts(ثابت حرف *str) {
    CV_Assert(write_mode);
    𐡄𐡍 (mem_mode)
        std::copy(str, str + strlen(str), std::back_inserter(outbuf));
    وإلا 𐡄𐡍 (file)
        fputs(str, file);
#𐡄𐡍 USE_ZLIB
    وإلا 𐡄𐡍 (gzfile)
        gzputs(gzfile, str);
#endif
    وإلا
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

حرف *FileStorage::Impl::getsFromFile(حرف *buf, صحيح count) {
    𐡄𐡍 (file)
        أرجع fgets(buf, count, file);
#𐡄𐡍 USE_ZLIB
    𐡄𐡍 (gzfile)
        أرجع gzgets(gzfile, buf, count);
#endif
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

حرف *FileStorage::Impl::gets(size_t maxCount) {
    𐡄𐡍 (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        ثابت حرف *instr = strbuf;
        لكل (; i < len; i++) {
            حرف c = instr[i];
            𐡄𐡍 (c == '\0' || c == '\n') {
                𐡄𐡍 (c == '\n')
                    i++;
                اكسر;
            }
        }
        size_t count = i - strbufpos;
        𐡄𐡍 (maxCount == 0 || maxCount > count)
            maxCount = count;
        buffer.resize(std::max(buffer.size(), maxCount + 8));
        memcpy(&buffer[0], instr + strbufpos, maxCount);
        buffer[maxCount] = '\0';
        strbufpos = i;
        أرجع maxCount > 0 ? &buffer[0] : 0;
    }

    ثابت size_t MAX_BLOCK_SIZE = INT_MAX / 2; // hopefully, that will be enough
    𐡄𐡍 (maxCount == 0)
        maxCount = MAX_BLOCK_SIZE;
    وإلا
        CV_Assert(maxCount < MAX_BLOCK_SIZE);
    size_t ofs = 0;

    لكل (;;) {
        صحيح count = (صحيح) std::min(buffer.size() - ofs - 16, maxCount);
        حرف *ptr = getsFromFile(&buffer[ofs], count + 1);
        𐡄𐡍 (!ptr)
            اكسر;
        صحيح delta = (صحيح) strlen(ptr);
        ofs += delta;
        maxCount -= delta;
        𐡄𐡍 (delta == 0 || ptr[delta - 1] == '\n' || maxCount == 0)
            اكسر;
        𐡄𐡍 (delta == count)
            buffer.resize((size_t) (buffer.size() * 1.5));
    }
    أرجع ofs > 0 ? &buffer[0] : 0;
}

حرف *FileStorage::Impl::gets() {
    حرف *ptr = هذا->gets(0);
    𐡄𐡍 (!ptr) {
        ptr = bufferStart();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        أرجع 0;
    } وإلا {
        size_t l = strlen(ptr);
        𐡄𐡍 (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    أرجع ptr;
}

منطقي FileStorage::Impl::eof() {
    𐡄𐡍 (dummy_eof)
        أرجع 𐡒𐡔𐡈;
    𐡄𐡍 (strbuf)
        أرجع strbufpos >= strbufsize;
    𐡄𐡍 (file)
        أرجع feof(file) != 0;
#𐡄𐡍 USE_ZLIB
    𐡄𐡍 (gzfile)
        أرجع gzeof(gzfile) != 0;
#endif
    أرجع خطأ;
}

فراغ FileStorage::Impl::setEof() {
    dummy_eof = 𐡒𐡔𐡈;
}

فراغ FileStorage::Impl::closeFile() {
    𐡄𐡍 (file)
        fclose(file);
#𐡄𐡍 USE_ZLIB
    وإلا 𐡄𐡍 (gzfile)
        gzclose(gzfile);
#endif
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = خطأ;
}

فراغ FileStorage::Impl::rewind() {
    𐡄𐡍 (file)
        ::rewind(file);
#𐡄𐡍 USE_ZLIB
    وإلا 𐡄𐡍 (gzfile)
        gzrewind(gzfile);
#endif
    strbufpos = 0;
}

حرف *FileStorage::Impl::resizeWriteBuffer(حرف *ptr, صحيح len) {
    ثابت حرف *buffer_end = &buffer[0] + buffer.size();
    𐡄𐡍 (ptr + len < buffer_end)
        أرجع ptr;

    ثابت حرف *buffer_start = &buffer[0];
    صحيح written_len = (صحيح) (ptr - buffer_start);

    CV_Assert(written_len <= (صحيح) buffer.size());
    صحيح new_size = (صحيح) ((buffer_end - buffer_start) * 3 / 2);
    new_size = MAX(written_len + len, new_size);
    buffer.reserve(new_size + 256);
    buffer.resize(new_size);
    bufofs = written_len;
    أرجع &buffer[0] + bufofs;
}

حرف *FileStorage::Impl::flush() {
    حرف *buffer_start = &buffer[0];
    حرف *ptr = buffer_start + bufofs;

    𐡄𐡍 (ptr > buffer_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(buffer_start);
        bufofs = 0;
    }

    صحيح indent = write_stack.back().indent;

    𐡄𐡍 (space != indent) {
        memset(buffer_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = buffer_start + bufofs;

    أرجع ptr;
}

فراغ FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(خطأ);
    𐡄𐡍 (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_stack.empty());

    FStructData &current_struct = write_stack.back();
    𐡄𐡍 (fmt == FileStorage::FORMAT_JSON && !FileNode::isFlow(current_struct.flags) && write_stack.size() > 1)
        current_struct.indent = write_stack[write_stack.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_stack.pop_back();
    𐡄𐡍 (!write_stack.empty())
        write_stack.back().flags &= ~FileNode::EMPTY;
}

فراغ FileStorage::Impl::startWriteStruct_helper(ثابت حرف *key, صحيح struct_flags,
                                                ثابت حرف *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (FileNode::TYPE_MASK | FileNode::FLOW)) | FileNode::EMPTY;
    𐡄𐡍 (!FileNode::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    𐡄𐡍 (type_name && type_name[0] == '\0')
        type_name = 0;

    FStructData s = getEmitter().startWriteStruct(write_stack.back(), key, struct_flags, type_name);

    write_stack.push_back(s);
    size_t write_stack_size = write_stack.size();
    𐡄𐡍 (write_stack_size > 1)
        write_stack[write_stack_size - 2].flags &= ~FileNode::EMPTY;

    𐡄𐡍 (fmt != FileStorage::FORMAT_JSON && !FileNode::isFlow(s.flags))
        flush();

    𐡄𐡍 (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && FileNode::isMap(struct_flags)) {
        getEmitter().اكتب("type_id", type_name, خطأ);
    }
}

فراغ FileStorage::Impl::startWriteStruct(ثابت حرف *key, صحيح struct_flags,
                                         ثابت حرف *type_name) {
    check_if_write_struct_is_delayed(خطأ);
    𐡄𐡍 (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    𐡄𐡍 (state_of_writing_base64 == FileStorage_API::Uncertain && FileNode::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(key, struct_flags, type_name);
    } وإلا 𐡄𐡍 (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        𐡄𐡍 ((FileNode::TYPE_MASK & struct_flags) != FileNode::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        وإلا 𐡄𐡍 (state_of_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(key, struct_flags, "binary");

        𐡄𐡍 (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::InUse);
    } وإلا {
        /* Won't output Base64 data */
        𐡄𐡍 (state_of_writing_base64 == FileStorage_API::InUse)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(key, struct_flags, type_name);

        𐡄𐡍 (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::NotUse);
    }
}

فراغ FileStorage::Impl::writeComment(ثابت حرف *comment, منطقي eol_comment) {
    CV_Assert(write_mode);
    getEmitter().writeComment(comment, eol_comment);
}

فراغ FileStorage::Impl::startNextStream() {
    CV_Assert(write_mode);
    𐡄𐡍 (!empty_stream) {
        طالما (!write_stack.empty())
            endWriteStruct();
        flush();
        getEmitter().startNextStream();
        empty_stream = 𐡒𐡔𐡈;
        write_stack.push_back(FStructData("", FileNode::EMPTY, 0));
        bufofs = 0;
    }
}

فراغ FileStorage::Impl::اكتب(ثابت String &key, صحيح value) {
    CV_Assert(write_mode);
    getEmitter().اكتب(key.c_str(), value);
}

فراغ FileStorage::Impl::اكتب(ثابت String &key, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().اكتب(key.c_str(), value);
}

فراغ FileStorage::Impl::اكتب(ثابت String &key, مزدوج value) {
    CV_Assert(write_mode);
    getEmitter().اكتب(key.c_str(), value);
}

فراغ FileStorage::Impl::اكتب(ثابت String &key, ثابت String &value) {
    CV_Assert(write_mode);
    getEmitter().اكتب(key.c_str(), value.c_str(), خطأ);
}

فراغ FileStorage::Impl::writeRawData(ثابت std::string &dt, ثابت فراغ *_data, size_t len) {
    CV_Assert(write_mode);

    𐡄𐡍 (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        writeRawDataBase64(_data, len, dt.c_str());
        أرجع;
    } وإلا 𐡄𐡍 (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elemSize = fs::calcStructSize(dt.c_str(), 0);
    CV_Assert(elemSize);
    CV_Assert(len % elemSize == 0);
    len /= elemSize;

    منطقي explicitZero = fmt == FileStorage::FORMAT_JSON;
    ثابت uchar *data0 = (ثابت uchar *) _data;
    صحيح fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_count;
    حرف buf[256] = "";

    fmt_pair_count = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    𐡄𐡍 (!len)
        أرجع;

    𐡄𐡍 (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    𐡄𐡍 (fmt_pair_count == 1) {
        fmt_pairs[0] *= (صحيح) len;
        len = 1;
    }

    لكل (; len--; data0 += elemSize) {
        صحيح offset = 0;
        لكل (k = 0; k < fmt_pair_count; k++) {
            صحيح i, count = fmt_pairs[k * 2];
            صحيح elem_type = fmt_pairs[k * 2 + 1];
            صحيح elem_size = CV_ELEM_SIZE(elem_type);
            ثابت حرف *ptr;

            offset = cvAlign(offset, elem_size);
            ثابت uchar *data = data0 + offset;

            لكل (i = 0; i < count; i++) {
                بدل (elem_type) {
                    حالة CV_8U:
                        ptr = fs::itoa(*(uchar *) data, buf, 10);
                        data++;
                        اكسر;
                    حالة CV_Bool:
                        ptr = fs::itoa(*(uchar *) data != 0, buf, 10);
                        data++;
                        اكسر;
                    حالة CV_8S:
                        ptr = fs::itoa(*(حرف *) data, buf, 10);
                        data++;
                        اكسر;
                    حالة CV_16U:
                        ptr = fs::itoa(*(ushort *) data, buf, 10);
                        data += حجم(ushort);
                        اكسر;
                    حالة CV_16S:
                        ptr = fs::itoa(*(short *) data, buf, 10);
                        data += حجم(short);
                        اكسر;
                    حالة CV_32U:
                        ptr = fs::itoa((int64_t)*(unsigned*) data, buf, 10, خطأ);
                        data += حجم(unsigned);
                        اكسر;
                    حالة CV_32S:
                        ptr = fs::itoa(*(صحيح *) data, buf, 10);
                        data += حجم(صحيح);
                        اكسر;
                    حالة CV_64U:
                        ptr = fs::itoa(*(uint64_t*) data, buf, 10, خطأ);
                        data += حجم(uint64_t);
                        اكسر;
                    حالة CV_64S:
                        ptr = fs::itoa(*(int64_t*) data, buf, 10, 𐡒𐡔𐡈);
                        data += حجم(int64_t);
                        اكسر;
                    حالة CV_32F:
                        ptr = fs::floatToString(buf, حجم(buf), *(عائم *) data, خطأ, explicitZero);
                        data += حجم(عائم);
                        اكسر;
                    حالة CV_64F:
                        ptr = fs::doubleToString(buf, حجم(buf), *(مزدوج *) data, explicitZero);
                        data += حجم(مزدوج);
                        اكسر;
                    حالة CV_16F:
                        ptr = fs::floatToString(buf, حجم(buf), (عائم) *(hfloat *) data, 𐡒𐡔𐡈, explicitZero);
                        data += حجم(hfloat);
                        اكسر;
                    حالة CV_16BF:
                        ptr = fs::floatToString(buf, حجم(buf), (عائم) *(bfloat *) data, 𐡒𐡔𐡈, explicitZero);
                        data += حجم(bfloat);
                        اكسر;
                    حالة CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, حجم(buf), (عائم) *(fp8_t *) data, 𐡒𐡔𐡈, explicitZero);
                        data += حجم(fp8_t);
                        اكسر;
                    حالة CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, حجم(buf), (عائم) *(fp8a_t *) data, 𐡒𐡔𐡈, explicitZero);
                        data += حجم(fp8a_t);
                        اكسر;
                    افتراضي:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        أرجع;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (صحيح) (data - data0);
        }
    }
}

فراغ FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(خطأ);

    𐡄𐡍 (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_to_Base64_state(FileStorage_API::Base64State::Uncertain);
}

فراغ FileStorage::Impl::switch_to_Base64_state(FileStorage_API::Base64State new_state) {
    ثابت حرف *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    ثابت حرف *err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    بدل (state_of_writing_base64) {
        حالة FileStorage_API::Base64State::Uncertain:
            بدل (new_state) {
                حالة FileStorage_API::Base64State::InUse:
                {
                    CV_DbgAssert(base64_writer == 0);
                    منطقي can_indent = (fmt != cv::FileStorage::Mode::FORMAT_JSON);
                    base64_writer = جديد base64::Base64Writer(*هذا, can_indent);
                    𐡄𐡍 (!can_indent) {
                        حرف *ptr = bufferPtr();
                        *ptr++ = '\0';
                        puts(bufferStart());
                        setBufferPtr(bufferStart());
                        memset(bufferStart(), 0, static_cast<صحيح>(space));
                        puts("\"$base64$");
                    }
                    اكسر;
                }
                حالة FileStorage_API::Base64State::Uncertain:
                    اكسر;
                حالة FileStorage_API::Base64State::NotUse:
                    اكسر;
                افتراضي:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    اكسر;
            }
            اكسر;
        حالة FileStorage_API::Base64State::InUse:
            بدل (new_state) {
                حالة FileStorage_API::Base64State::InUse:
                حالة FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    اكسر;
                حالة FileStorage_API::Base64State::Uncertain:
                    delete base64_writer;
                    base64_writer = 0;
                    𐡄𐡍 ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        setBufferPtr(bufferStart());
                        flush();
                        memset(bufferStart(), 0, static_cast<صحيح>(space) );
                        setBufferPtr(bufferStart());
                    }
                    اكسر;
                افتراضي:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    اكسر;
            }
            اكسر;
        حالة FileStorage_API::Base64State::NotUse:
            بدل (new_state) {
                حالة FileStorage_API::Base64State::InUse:
                حالة FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    اكسر;
                حالة FileStorage_API::Base64State::Uncertain:
                    اكسر;
                افتراضي:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    اكسر;
            }
            اكسر;
        افتراضي:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            اكسر;
    }

    state_of_writing_base64 = new_state;
}

فراغ FileStorage::Impl::make_write_struct_delayed(ثابت حرف *key, صحيح struct_flags, ثابت حرف *type_name) {
    CV_Assert(is_write_struct_delayed == خطأ);
    CV_DbgAssert(delayed_struct_key == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    𐡄𐡍 (key != nullptr) {
        delayed_struct_key = جديد حرف[strlen(key) + 1U];
        strcpy(delayed_struct_key, key);
    }

    𐡄𐡍 (type_name != nullptr) {
        delayed_type_name = جديد حرف[strlen(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = 𐡒𐡔𐡈;
}

فراغ FileStorage::Impl::check_if_write_struct_is_delayed(منطقي change_type_to_base64) {
    𐡄𐡍 (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_key;
        std::string type_name;
        صحيح struct_flags = delayed_struct_flags;

        𐡄𐡍 (delayed_struct_key != nullptr && *delayed_struct_key != '\0') {
            struct_key.assign(delayed_struct_key);
        }
        𐡄𐡍 (delayed_type_name != nullptr && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        delete[] delayed_struct_key;
        delete[] delayed_type_name;
        delayed_struct_key = nullptr;
        delayed_struct_flags = 0;
        delayed_type_name = nullptr;

        is_write_struct_delayed = خطأ;

        /* call */
        𐡄𐡍 (change_type_to_base64) {
            startWriteStruct_helper(struct_key.c_str(), struct_flags, "binary");
            𐡄𐡍 (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::InUse);
        } وإلا {
            startWriteStruct_helper(struct_key.c_str(), struct_flags, type_name.c_str());
            𐡄𐡍 (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::NotUse);
        }
    }
}

فراغ FileStorage::Impl::writeRawDataBase64(ثابت فراغ *_data, size_t len, ثابت حرف *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(𐡒𐡔𐡈);

    𐡄𐡍 (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } وإلا 𐡄𐡍 (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->اكتب(_data, len, dt);
}

FileNode FileStorage::Impl::getFirstTopLevelNode() ثابت {
    أرجع roots.empty() ? FileNode() : roots[0];
}

FileNode FileStorage::Impl::root(صحيح streamIdx) ثابت {
    أرجع streamIdx >= 0 && streamIdx < (صحيح) roots.size() ? roots[streamIdx] : FileNode();
}

FileNode FileStorage::Impl::operator[](ثابت String &nodename) ثابت {
    أرجع هذا->operator[](nodename.c_str());
}

FileNode FileStorage::Impl::operator[](ثابت حرف * /*nodename*/) ثابت {
    أرجع FileNode();
}

صحيح FileStorage::Impl::getFormat() ثابت { أرجع fmt; }

حرف *FileStorage::Impl::bufferPtr() ثابت { أرجع (حرف *) (&buffer[0] + bufofs); }

حرف *FileStorage::Impl::bufferStart() ثابت { أرجع (حرف *) &buffer[0]; }

حرف *FileStorage::Impl::bufferEnd() ثابت { أرجع (حرف *) (&buffer[0] + buffer.size()); }

فراغ FileStorage::Impl::setBufferPtr(حرف *ptr) {
    حرف *bufferstart = bufferStart();
    CV_Assert(ptr >= bufferstart && ptr <= bufferEnd());
    bufofs = ptr - bufferstart;
}

صحيح FileStorage::Impl::wrapMargin() ثابت { أرجع wrap_margin; }

FStructData &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_stack.empty());
    أرجع write_stack.back();
}

فراغ FileStorage::Impl::setNonEmpty() {
    empty_stream = خطأ;
}

فراغ FileStorage::Impl::processSpecialDouble(حرف *buf, مزدوج *value, حرف **endptr) {
    FileStorage_API *fs = هذا;
    حرف c = buf[0];
    صحيح inf_hi = 0x7ff00000;

    𐡄𐡍 (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    𐡄𐡍 (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    𐡄𐡍 (toupper(buf[1]) == 'I' && toupper(buf[2]) == 'N' && toupper(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    وإلا 𐡄𐡍 (toupper(buf[1]) == 'N' && toupper(buf[2]) == 'A' && toupper(buf[3]) == 'N')
        v.u = (uint64) -1;
    وإلا
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

مزدوج FileStorage::Impl::strtod(حرف *ptr, حرف **endptr) {
    مزدوج fval = ::strtod(ptr, endptr);
    𐡄𐡍 (**endptr == '.') {
        حرف *dot_pos = *endptr;
        *dot_pos = ',';
        مزدوج fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        𐡄𐡍 (*endptr > dot_pos)
            fval = fval2;
        وإلا
            *endptr = dot_pos;
    }

    𐡄𐡍 (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    أرجع fval;
}

فراغ FileStorage::Impl::convertToCollection(صحيح type, FileNode &node) {
    CV_Assert(type == FileNode::SEQ || type == FileNode::MAP);

    صحيح node_type = node.type();
    𐡄𐡍 (node_type == type)
        أرجع;

    منطقي named = node.isNamed();
    uchar *ptr = node.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    مزدوج fval = 0;
    std::string sval;
    منطقي add_first_scalar = خطأ;

    𐡄𐡍 (node_type != FileNode::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == FileNode::SEQ);
        𐡄𐡍 (node_type == FileNode::INT) {
            ival = readLong(ptr);
            add_first_scalar = 𐡒𐡔𐡈;
        } وإلا 𐡄𐡍 (node_type == FileNode::REAL) {
            fval = readReal(ptr);
            add_first_scalar = 𐡒𐡔𐡈;
        } وإلا 𐡄𐡍 (node_type == FileNode::STRING) {
            sval = std::string(node);
            add_first_scalar = 𐡒𐡔𐡈;
        } وإلا
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", node_type));
    }

    ptr = reserveNodeSpace(node, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? FileNode::NAMED : 0));
    // name has been copied automatically
    𐡄𐡍 (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    𐡄𐡍 (add_first_scalar)
        addNode(node, std::string(), node_type,
                node_type == FileNode::INT ? (ثابت فراغ *) &ival :
                node_type == FileNode::REAL ? (ثابت فراغ *) &fval :
                node_type == FileNode::STRING ? (ثابت فراغ *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserveNodeSpace(FileNode &node, size_t sz) {
    منطقي shrinkBlock = خطأ;
    size_t shrinkBlockIdx = 0, shrinkSize = 0;

    uchar *ptr = 0, *blockEnd = 0;

    𐡄𐡍 (!fs_data_ptrs.empty()) {
        size_t blockIdx = node.blockIdx;
        size_t ofs = node.ofs;
        CV_Assert(blockIdx == fs_data_ptrs.size() - 1);
        CV_Assert(ofs <= fs_data_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_data_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_data_ptrs[blockIdx] + ofs;
        blockEnd = fs_data_ptrs[blockIdx] + fs_data_blksz[blockIdx];

        CV_Assert(ptr >= fs_data_ptrs[blockIdx] && ptr <= blockEnd);
        𐡄𐡍 (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            أرجع ptr;
        }

        𐡄𐡍 (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_data[blockIdx]->resize(sz);
            ptr = &fs_data[blockIdx]->at(0);
            fs_data_ptrs[blockIdx] = ptr;
            fs_data_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            أرجع ptr;
        }

        shrinkBlock = 𐡒𐡔𐡈;
        shrinkBlockIdx = blockIdx;
        shrinkSize = ofs;
    }

    size_t blockSize = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(blockSize);
    fs_data.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_data_ptrs.push_back(new_ptr);
    fs_data_blksz.push_back(blockSize);
    node.blockIdx = fs_data_ptrs.size() - 1;
    node.ofs = 0;
    freeSpaceOfs = sz;

    𐡄𐡍 (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        𐡄𐡍 (ptr[0] & FileNode::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    𐡄𐡍 (shrinkBlock) {
        fs_data[shrinkBlockIdx]->resize(shrinkSize);
        fs_data_blksz[shrinkBlockIdx] = shrinkSize;
    }

    أرجع new_ptr;
}

unsigned FileStorage::Impl::getStringOfs(ثابت std::string &key) ثابت {
    str_hash_t::const_iterator it = str_hash.find(key);
    أرجع it != str_hash.end() ? it->second : 0;
}

FileNode FileStorage::Impl::addNode(FileNode &collection, ثابت std::string &key,
                                    صحيح elem_type, ثابت فراغ *value, صحيح len) {
    FileStorage_API *fs = هذا;
    منطقي noname = key.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(key.c_str(), "_") == 0);
    convertToCollection(noname ? FileNode::SEQ : FileNode::MAP, collection);

    منطقي isseq = collection.empty() ? خطأ : collection.isSeq();
    𐡄𐡍 (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    unsigned strofs = 0;
    𐡄𐡍 (!noname) {
        strofs = getStringOfs(key);
        𐡄𐡍 (!strofs) {
            strofs = (unsigned) str_hash_data.size();
            size_t keysize = key.size() + 1;
            str_hash_data.resize(strofs + keysize);
            memcpy(&str_hash_data[0] + strofs, &key[0], keysize);
            str_hash.insert(std::make_pair(key, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t blockIdx = fs_data_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    FileNode node(fs_ext, blockIdx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserveNodeSpace(node, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : FileNode::NAMED));
    𐡄𐡍 (elem_type == FileNode::NONE)
        freeSpaceOfs -= 8;

    𐡄𐡍 (!noname) {
        writeInt(ptr, (صحيح) strofs);
        ptr += 4;
    }

    𐡄𐡍 (elem_type == FileNode::SEQ || elem_type == FileNode::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    𐡄𐡍 (value)
        node.setValue(elem_type, value, len);

    𐡄𐡍 (collection.isNamed())
        cp += 4;
    صحيح nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    أرجع node;
}

فراغ FileStorage::Impl::finalizeCollection(FileNode &collection) {
    𐡄𐡍 (!collection.isSeq() && !collection.isMap())
        أرجع;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    𐡄𐡍 (*ptr0 & FileNode::NAMED)
        ptr += 4;
    size_t blockIdx = collection.blockIdx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t rawSize = 4;
    unsigned sz = (unsigned) readInt(ptr + 4);
    𐡄𐡍 (sz > 0) {
        size_t lastBlockIdx = fs_data_ptrs.size() - 1;

        لكل (; blockIdx < lastBlockIdx; blockIdx++) {
            rawSize += fs_data_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    rawSize += freeSpaceOfs - ofs;
    writeInt(ptr, (صحيح) rawSize);
}

فراغ FileStorage::Impl::normalizeNodeOfs(size_t &blockIdx, size_t &ofs) ثابت {
    طالما (ofs >= fs_data_blksz[blockIdx]) {
        𐡄𐡍 (blockIdx == fs_data_blksz.size() - 1) {
            CV_Assert(ofs == fs_data_blksz[blockIdx]);
            اكسر;
        }
        ofs -= fs_data_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    أرجع state_of_writing_base64;
}

صحيح FileStorage::Impl::get_space() {
    أرجع space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = 𐡒𐡔𐡈;
}

فراغ FileStorage::Impl::Base64Decoder::init(ثابت Ptr<FileStorageParser> &_parser, حرف *_ptr, صحيح _indent) {
    parser_do_not_use_direct_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = خطأ;
}

منطقي FileStorage::Impl::Base64Decoder::readMore(صحيح needed) {
    ساكن ثابت uchar base64tab[] =
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

    𐡄𐡍 (eos)
        أرجع خطأ;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    لكل (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    حرف *beg = 0, *end = 0;
    منطقي ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    𐡄𐡍 (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = 𐡒𐡔𐡈;
        size_t tc = totalchars;
        لكل (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    صحيح i = 0, j, n = (صحيح) encoded.size();
    𐡄𐡍 (n > 0) {
        ثابت uchar *tab = base64tab;
        حرف *src = &encoded[0];

        لكل (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(صحيح) (uchar) src[i]], c = tab[(صحيح) (uchar) src[i + 1]];
            uchar b = tab[(صحيح) (uchar) src[i + 2]], a = tab[(صحيح) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    𐡄𐡍 (i > 0 && encoded[i - 1] == '=') {
        𐡄𐡍 (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        𐡄𐡍 (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    لكل (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    أرجع (صحيح) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    𐡄𐡍 (ofs >= sz && !readMore(1))
        أرجع (uchar) 0;
    أرجع decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    𐡄𐡍 (ofs + 2 > sz && !readMore(2))
        أرجع (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    أرجع val;
}

صحيح FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    𐡄𐡍 (ofs + 4 > sz && !readMore(4))
        أرجع 0;
    صحيح ival = readInt(&decoded[ofs]);
    ofs += 4;
    أرجع ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    𐡄𐡍 (ofs + 8 > sz && !readMore(8))
        أرجع 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    أرجع ival;
}

مزدوج FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    𐡄𐡍 (ofs + 8 > sz && !readMore(8))
        أرجع 0;
    مزدوج fval = readReal(&decoded[ofs]);
    ofs += 8;
    أرجع fval;
}

منطقي FileStorage::Impl::Base64Decoder::endOfStream() ثابت { أرجع eos; }

حرف *FileStorage::Impl::Base64Decoder::getPtr() ثابت { أرجع ptr; }


حرف *FileStorage::Impl::parseBase64(حرف *ptr, صحيح indent, FileNode &collection) {
    ثابت صحيح BASE64_HDR_SIZE = 24;
    حرف dt[BASE64_HDR_SIZE + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    صحيح i, k;

    لكل (i = 0; i < BASE64_HDR_SIZE; i++)
        dt[i] = (حرف) base64decoder.getUInt8();
    لكل (i = 0; i < BASE64_HDR_SIZE; i++)
        𐡄𐡍 (isspace(dt[i]))
            اكسر;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    صحيح fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    صحيح fmt_pair_count = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    مزدوج fval = 0;

    لكل (;;) {
        لكل (k = 0; k < fmt_pair_count; k++) {
            صحيح elem_type = fmt_pairs[k * 2 + 1];
            صحيح count = fmt_pairs[k * 2];

            لكل (i = 0; i < count; i++) {
                صحيح node_type = FileNode::INT;
                بدل (elem_type) {
                    حالة CV_8U:
                        ival = base64decoder.getUInt8();
                        اكسر;
                    حالة CV_8S:
                        ival = (حرف) base64decoder.getUInt8();
                        اكسر;
                    حالة CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        اكسر;
                    حالة CV_16U:
                        ival = base64decoder.getUInt16();
                        اكسر;
                    حالة CV_16S:
                        ival = (short) base64decoder.getUInt16();
                        اكسر;
                    حالة CV_32U:
                        ival = base64decoder.getInt32();
                        اكسر;
                    حالة CV_32S:
                        ival = base64decoder.getInt32();
                        اكسر;
                    حالة CV_64U:
                        ival = base64decoder.getInt64();
                        اكسر;
                    حالة CV_64S:
                        ival = base64decoder.getInt64();
                        اكسر;
                    حالة CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        node_type = FileNode::REAL;
                    }
                        اكسر;
                    حالة CV_64F:
                        fval = base64decoder.getFloat64();
                        node_type = FileNode::REAL;
                        اكسر;
                    حالة CV_16F:
                        fval = عائم(hfloatFromBits(base64decoder.getUInt16()));
                        node_type = FileNode::REAL;
                        اكسر;
                    حالة CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        node_type = FileNode::REAL;
                        اكسر;
                    حالة CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        node_type = FileNode::REAL;
                        اكسر;
                    افتراضي:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                𐡄𐡍 (base64decoder.endOfStream())
                    اكسر;
                addNode(collection, std::string(), node_type,
                        node_type == FileNode::INT ? (فراغ *) &ival : (فراغ *) &fval, -1);
            }
        }
        𐡄𐡍 (base64decoder.endOfStream())
            اكسر;
    }

    finalizeCollection(collection);
    أرجع base64decoder.getPtr();
}

فراغ FileStorage::Impl::parseError(ثابت حرف *func_name, ثابت std::string &err_msg, ثابت حرف *source_file,
                                   صحيح source_line) {
    std::string msg = format("%s(%d): %s", filename.c_str(), lineno, err_msg.c_str());
    error(Error::StsParseError, func_name, msg.c_str(), source_file, source_line);
}

ثابت uchar *FileStorage::Impl::getNodePtr(size_t blockIdx, size_t ofs) ثابت {
    CV_Assert(blockIdx < fs_data_ptrs.size());
    CV_Assert(ofs < fs_data_blksz[blockIdx]);

    أرجع fs_data_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) ثابت {
    CV_Assert(nameofs < str_hash_data.size());
    أرجع std::string(&str_hash_data[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { أرجع fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(هذا);
}

FileStorage::FileStorage(ثابت String& filename, صحيح flags, ثابت String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(هذا);
    منطقي ok = p->open(filename.c_str(), flags, encoding.c_str());
    𐡄𐡍(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

فراغ FileStorage::startWriteStruct(ثابت String& name, صحيح struct_flags, ثابت String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    𐡄𐡍 ((struct_flags & FileNode::TYPE_MASK) == FileNode::SEQ)
        state = FileStorage::VALUE_EXPECTED;
    وإلا
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

فراغ FileStorage::endWriteStruct()
{
    p->endWriteStruct();
    state = p->write_stack.empty() || FileNode::isMap(p->write_stack.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::VALUE_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

منطقي FileStorage::open(ثابت String& filename, صحيح flags, ثابت String& encoding)
{
    حاول
    {
        منطقي ok = p->open(filename.c_str(), flags, encoding.c_str());
        𐡄𐡍(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        أرجع ok;
    }
    امسك (...)
    {
        release();
        ارم;  // re-throw
    }
}

منطقي FileStorage::isOpened() ثابت { أرجع p->is_opened; }

فراغ FileStorage::release()
{
    p->release();
}

FileNode FileStorage::root(صحيح i) ثابت
{
    𐡄𐡍( p.empty() || p->roots.empty() || i < 0 || i >= (صحيح)p->roots.size() )
        أرجع FileNode();

    أرجع p->roots[i];
}

FileNode FileStorage::getFirstTopLevelNode() ثابت
{
    FileNode r = root();
    FileNodeIterator it = r.begin();
    أرجع it != r.end() ? *it : FileNode();
}

std::string FileStorage::getDefaultObjectName(ثابت std::string& _filename)
{
    ساكن ثابت حرف* stubname = "unnamed";
    ثابت حرف* filename = _filename.c_str();
    ثابت حرف* ptr2 = filename + _filename.size();
    ثابت حرف* ptr = ptr2 - 1;
    cv::AutoBuffer<حرف> name_buf(_filename.size()+1);

    طالما( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        𐡄𐡍( *ptr == '.' && (!*ptr2 || strncmp(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    𐡄𐡍( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    حرف* name = name_buf.data();

    // name must start with letter or '_'
    𐡄𐡍( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    طالما( ptr < ptr2 )
    {
        حرف c = *ptr++;
        𐡄𐡍( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.data();
    𐡄𐡍( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    أرجع name;
}


صحيح FileStorage::getFormat() ثابت
{
    أرجع p->fmt;
}

FileNode FileStorage::operator [](ثابت حرف* key) ثابت
{
    أرجع هذا->operator[](std::string(key));
}

FileNode FileStorage::operator [](ثابت std::string& key) ثابت
{
    FileNode res;
    لكل (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][key];
        𐡄𐡍 (!res.empty())
            اكسر;
    }
    أرجع res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    أرجع buf;
}

فراغ FileStorage::writeRaw( ثابت String& fmt, ثابت فراغ* vec, size_t len )
{
    p->writeRawData(fmt, (ثابت uchar*)vec, len);
}

فراغ FileStorage::writeComment( ثابت String& comment, منطقي eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

فراغ writeScalar( FileStorage& fs, صحيح value )
{
    fs.p->اكتب(String(), value);
}

فراغ writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->اكتب(String(), value);
}

فراغ writeScalar( FileStorage& fs, عائم value )
{
    fs.p->اكتب(String(), (مزدوج)value);
}

فراغ writeScalar( FileStorage& fs, مزدوج value )
{
    fs.p->اكتب(String(), value);
}

فراغ writeScalar( FileStorage& fs, ثابت String& value )
{
    fs.p->اكتب(String(), value);
}

فراغ اكتب( FileStorage& fs, ثابت String& name, صحيح value )
{
    fs.p->اكتب(name, value);
}

فراغ اكتب( FileStorage& fs, ثابت String& name, int64_t value )
{
    fs.p->اكتب(name, value);
}

فراغ اكتب( FileStorage& fs, ثابت String& name, عائم value )
{
    fs.p->اكتب(name, (مزدوج)value);
}

فراغ اكتب( FileStorage& fs, ثابت String& name, مزدوج value )
{
    fs.p->اكتب(name, value);
}

فراغ اكتب( FileStorage& fs, ثابت String& name, ثابت String& value )
{
    fs.p->اكتب(name, value);
}

فراغ اكتب( FileStorage& fs, ثابت String& name, منطقي value )
{
    fs.p->اكتب(name, value);
}

فراغ FileStorage::اكتب(ثابت String& name, صحيح val) { p->اكتب(name, val); }
فراغ FileStorage::اكتب(ثابت String& name, منطقي val) { p->اكتب(name, val); }
فراغ FileStorage::اكتب(ثابت String& name, int64_t val) { p->اكتب(name, val); }
فراغ FileStorage::اكتب(ثابت String& name, مزدوج val) { p->اكتب(name, val); }
فراغ FileStorage::اكتب(ثابت String& name, ثابت String& val) { p->اكتب(name, val); }
فراغ FileStorage::اكتب(ثابت String& name, ثابت Mat& val) { cv::اكتب(*هذا, name, val); }
فراغ FileStorage::اكتب(ثابت String& name, ثابت std::vector<String>& val) { cv::اكتب(*هذا, name, val); }

FileStorage& operator << (FileStorage& fs, ثابت String& str)
{
    تعداد { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        VALUE_EXPECTED = FileStorage::VALUE_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    ثابت حرف* _str = str.c_str();
    𐡄𐡍( !fs.isOpened() || !_str )
        أرجع fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    حرف c = *_str;

    𐡄𐡍( c == '}' || c == ']' )
    {
        𐡄𐡍( fs_impl->write_stack.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        صحيح struct_flags = fs_impl->write_stack.back().flags;
        حرف expected_bracket = FileNode::isMap(struct_flags) ? '}' : ']';
        𐡄𐡍( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_stack.empty());
        struct_flags = fs_impl->write_stack.back().flags;
        fs.state = FileNode::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : VALUE_EXPECTED;
        fs.elname = String();
    }
    وإلا 𐡄𐡍( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        𐡄𐡍 (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = VALUE_EXPECTED + INSIDE_MAP;
    }
    وإلا 𐡄𐡍( (fs.state & 3) == VALUE_EXPECTED )
    {
        𐡄𐡍( c == '{' || c == '[' )
        {
            صحيح struct_flags = c == '{' ? FileNode::MAP : FileNode::SEQ;
            fs.state = struct_flags == FileNode::MAP ? INSIDE_MAP + NAME_EXPECTED : VALUE_EXPECTED;
            _str++;
            𐡄𐡍( *_str == ':' )
            {
                _str++;
                𐡄𐡍( !*_str )
                    struct_flags |= FileNode::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        وإلا
        {
            اكتب( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            𐡄𐡍( fs.state == INSIDE_MAP + VALUE_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    وإلا
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    أرجع fs;
}


FileNode::FileNode()
    : fs(عدم)
{
    blockIdx = ofs = 0;
}

FileNode::FileNode(FileStorage::Impl* _fs, size_t _blockIdx, size_t _ofs)
    : fs(_fs)
{
    blockIdx = _blockIdx;
    ofs = _ofs;
}

FileNode::FileNode(ثابت FileStorage* _fs, size_t _blockIdx, size_t _ofs)
    : FileNode(_fs->p.get(), _blockIdx, _ofs)
{
    // nothing
}

FileNode::FileNode(ثابت FileNode& node)
{
    fs = node.fs;
    blockIdx = node.blockIdx;
    ofs = node.ofs;
}

FileNode& FileNode::operator=(ثابت FileNode& node)
{
    fs = node.fs;
    blockIdx = node.blockIdx;
    ofs = node.ofs;
    أرجع *هذا;
}

FileNode FileNode::operator[](ثابت std::string& nodename) ثابت
{
    𐡄𐡍(!fs)
        أرجع FileNode();

    CV_Assert( isMap() );

    unsigned key = fs->getStringOfs(nodename);
    size_t i, sz = size();
    FileNodeIterator it = begin();

    لكل( i = 0; i < sz; i++, ++it )
    {
        FileNode n = *it;
        ثابت uchar* p = n.ptr();
        unsigned key2 = (unsigned)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_data.size() );
        𐡄𐡍( key == key2 )
            أرجع n;
    }
    أرجع FileNode();
}

FileNode FileNode::operator[](ثابت حرف* nodename) ثابت
{
    أرجع هذا->operator[](std::string(nodename));
}

FileNode FileNode::operator[](صحيح i) ثابت
{
    𐡄𐡍(!fs)
        أرجع FileNode();

    CV_Assert( isSeq() );

    صحيح sz = (صحيح)size();
    CV_Assert( 0 <= i && i < sz );

    FileNodeIterator it = begin();
    it += i;

    أرجع *it;
}

std::vector<String> FileNode::keys() ثابت
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    لكل (FileNodeIterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    أرجع res;
}

صحيح FileNode::type() ثابت
{
    ثابت uchar* p = ptr();
    𐡄𐡍(!p)
        أرجع NONE;
    أرجع (*p & TYPE_MASK);
}

منطقي FileNode::isMap(صحيح flags) { أرجع (flags & TYPE_MASK) == MAP; }
منطقي FileNode::isSeq(صحيح flags) { أرجع (flags & TYPE_MASK) == SEQ; }
منطقي FileNode::isCollection(صحيح flags) { أرجع isMap(flags) || isSeq(flags); }
منطقي FileNode::isFlow(صحيح flags) { أرجع (flags & FLOW) != 0; }
منطقي FileNode::isEmptyCollection(صحيح flags) { أرجع (flags & EMPTY) != 0; }

منطقي FileNode::empty() ثابت   { أرجع fs == 0; }
منطقي FileNode::isNone() ثابت  { أرجع type() == NONE; }
منطقي FileNode::isSeq() ثابت   { أرجع type() == SEQ; }
منطقي FileNode::isMap() ثابت   { أرجع type() == MAP; }
منطقي FileNode::isInt() ثابت   { أرجع type() == INT;  }
منطقي FileNode::isReal() ثابت  { أرجع type() == REAL; }
منطقي FileNode::isString() ثابت { أرجع type() == STRING;  }
منطقي FileNode::isNamed() ثابت
{
    ثابت uchar* p = ptr();
    𐡄𐡍(!p)
        أرجع خطأ;
    أرجع (*p & NAMED) != 0;
}

std::string FileNode::name() ثابت
{
    ثابت uchar* p = ptr();
    𐡄𐡍(!p)
        أرجع std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    أرجع fs->getName(nameofs);
}

FileNode::operator صحيح() ثابت
{
    ثابت uchar* p = ptr();
    𐡄𐡍(!p)
        أرجع 0;
    صحيح tag = *p;
    صحيح type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    𐡄𐡍( type == INT )
    {
        أرجع readInt(p);
    }
    وإلا 𐡄𐡍( type == REAL )
    {
        أرجع cvRound(readReal(p));
    }
    وإلا
        أرجع 0x7fffffff;
}

FileNode::operator int64_t() ثابت
{
    ثابت uchar* p = ptr();
    𐡄𐡍(!p)
        أرجع 0;
    صحيح tag = *p;
    صحيح type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    𐡄𐡍( type == INT )
    {
        أرجع readLong(p);
    }
    وإلا 𐡄𐡍( type == REAL )
    {
        أرجع cvRound(readReal(p));
    }
    وإلا
        أرجع 0x7fffffff;
}

FileNode::operator عائم() ثابت
{
    ثابت uchar* p = ptr();
    𐡄𐡍(!p)
        أرجع 0.f;
    صحيح tag = *p;
    صحيح type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    𐡄𐡍( type == INT )
    {
        أرجع (عائم)readLong(p);
    }
    وإلا 𐡄𐡍( type == REAL )
    {
        أرجع (عائم)readReal(p);
    }
    وإلا
        أرجع FLT_MAX;
}

FileNode::operator مزدوج() ثابت
{
    ثابت uchar* p = ptr();
    𐡄𐡍(!p)
        أرجع 0.f;
    صحيح tag = *p;
    صحيح type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    𐡄𐡍( type == INT )
    {
        أرجع (مزدوج)readLong(p);
    }
    وإلا 𐡄𐡍( type == REAL )
    {
        أرجع readReal(p);
    }
    وإلا
        أرجع DBL_MAX;
}

مزدوج FileNode::real() ثابت  { أرجع مزدوج(*هذا); }

std::string FileNode::string() ثابت
{
    ثابت uchar* p = ptr();
    𐡄𐡍( !p || (*p & TYPE_MASK) != STRING )
        أرجع std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(unsigned)readInt(p);
    أرجع std::string((ثابت حرف*)(p + 4), sz - 1);
}
Mat FileNode::mat() ثابت { Mat value; اقرأ(*هذا, value, Mat()); أرجع value; }

FileNodeIterator FileNode::begin() ثابت { أرجع FileNodeIterator(*هذا, خطأ); }
FileNodeIterator FileNode::end() ثابت   { أرجع FileNodeIterator(*هذا, 𐡒𐡔𐡈); }

فراغ FileNode::readRaw( ثابت std::string& fmt, فراغ* vec, size_t len ) ثابت
{
    FileNodeIterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t FileNode::size() ثابت
{
    ثابت uchar* p = ptr();
    𐡄𐡍( !p )
        أرجع 0;
    صحيح tag = *p;
    صحيح tp = tag & TYPE_MASK;
    𐡄𐡍( tp == MAP || tp == SEQ )
    {
        𐡄𐡍( tag & NAMED )
            p += 4;
        أرجع (size_t)(unsigned)readInt(p + 5);
    }
    أرجع tp != NONE;
}

size_t FileNode::rawSize() ثابت
{
    ثابت uchar* p0 = ptr(), *p = p0;
    𐡄𐡍( !p )
        أرجع 0;
    صحيح tag = *p++;
    صحيح tp = tag & TYPE_MASK;
    𐡄𐡍( tag & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    𐡄𐡍( tp == INT )
        أرجع sz0 + 8;
    𐡄𐡍( tp == REAL )
        أرجع sz0 + 8;
    𐡄𐡍( tp == NONE )
        أرجع sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    أرجع sz0 + 4 + readInt(p);
}

uchar* FileNode::ptr()
{
    أرجع !fs ? 0 : (uchar*)fs->getNodePtr(blockIdx, ofs);
}

ثابت uchar* FileNode::ptr() ثابت
{
    أرجع !fs ? 0 : fs->getNodePtr(blockIdx, ofs);
}

فراغ FileNode::setValue( صحيح type, ثابت فراغ* value, صحيح len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    صحيح tag = *p;
    صحيح current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    صحيح sz = 1;

    𐡄𐡍( tag & NAMED )
        sz += 4;

    𐡄𐡍( type == INT )
        sz += 8;
    وإلا 𐡄𐡍( type == REAL )
        sz += 8;
    وإلا 𐡄𐡍( type == STRING )
    {
        𐡄𐡍( len < 0 )
            len = (صحيح)strlen((ثابت حرف*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    وإلا
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserveNodeSpace(*هذا, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    𐡄𐡍( tag & NAMED )
        p += 4;

    𐡄𐡍( type == INT )
    {
        int64_t ival = *(ثابت int64_t*)value;
        writeInt(p, ival);
    }
    وإلا 𐡄𐡍( type == REAL )
    {
        مزدوج dbval = *(ثابت مزدوج*)value;
        writeReal(p, dbval);
    }
    وإلا 𐡄𐡍( type == STRING )
    {
        ثابت حرف* str = (ثابت حرف*)value;
        writeInt(p, len + 1);
        memcpy(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

FileNodeIterator::FileNodeIterator()
{
    fs = 0;
    blockIdx = 0;
    ofs = 0;
    blockSize = 0;
    nodeNElems = 0;
    idx = 0;
}

FileNodeIterator::FileNodeIterator( ثابت FileNode& node, منطقي seekEnd )
{
    fs = node.fs;
    idx = 0;
    𐡄𐡍( !fs )
        blockIdx = ofs = blockSize = nodeNElems = 0;
    وإلا
    {
        blockIdx = node.blockIdx;
        ofs = node.ofs;

        منطقي collection = node.isSeq() || node.isMap();
        𐡄𐡍( node.isNone() )
        {
            nodeNElems = 0;
        }
        وإلا 𐡄𐡍( !collection )
        {
            nodeNElems = 1;
            𐡄𐡍( seekEnd )
            {
                idx = 1;
                ofs += node.rawSize();
            }
        }
        وإلا
        {
            nodeNElems = node.size();
            ثابت uchar* p0 = node.ptr(), *p = p0 + 1;
            𐡄𐡍(*p0 & FileNode::NAMED )
                p += 4;
            𐡄𐡍( !seekEnd )
                ofs += (p - p0) + 8;
            وإلا
            {
                size_t rawsz = (size_t)(unsigned)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = nodeNElems;
            }
        }
        fs->normalizeNodeOfs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
}

FileNodeIterator::FileNodeIterator(ثابت FileNodeIterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    nodeNElems = it.nodeNElems;
    idx = it.idx;
}

FileNodeIterator& FileNodeIterator::operator=(ثابت FileNodeIterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    nodeNElems = it.nodeNElems;
    idx = it.idx;
    أرجع *هذا;
}

FileNode FileNodeIterator::operator *() ثابت
{
    أرجع FileNode(idx < nodeNElems ? fs : عدم, blockIdx, ofs);
}

FileNodeIterator& FileNodeIterator::operator ++ ()
{
    𐡄𐡍( idx == nodeNElems || !fs )
        أرجع *هذا;
    idx++;
    FileNode n(fs, blockIdx, ofs);
    ofs += n.rawSize();
    𐡄𐡍( ofs >= blockSize )
    {
        fs->normalizeNodeOfs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
    أرجع *هذا;
}

FileNodeIterator FileNodeIterator::operator ++ (صحيح)
{
    FileNodeIterator it = *هذا;
    ++(*هذا);
    أرجع it;
}

FileNodeIterator& FileNodeIterator::operator += (صحيح _ofs)
{
    CV_Assert( _ofs >= 0 );
    لكل( ; _ofs > 0; _ofs-- )
        هذا->operator ++();
    أرجع *هذا;
}

FileNodeIterator& FileNodeIterator::readRaw( ثابت String& fmt, فراغ* _data0, size_t maxsz)
{
    𐡄𐡍( fs && idx < nodeNElems )
    {
        uchar* data0 = (uchar*)_data0;
        صحيح fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        صحيح fmt_pair_count = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        size_t esz = fs::calcStructSize( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        لكل( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            لكل( صحيح k = 0; k < fmt_pair_count; k++ )
            {
                صحيح elem_type = fmt_pairs[k*2+1];
                صحيح elem_size = CV_ELEM_SIZE(elem_type);

                صحيح count = fmt_pairs[k*2];
                offset = alignSize( offset, elem_size );
                uchar* data = data0 + offset;

                لكل( صحيح i = 0; i < count; i++, ++(*هذا) )
                {
                    FileNode node = *(*هذا);
                    𐡄𐡍( node.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_size == 8 ? (int64_t)node : (صحيح)node);
                        بدل( elem_type )
                        {
                        حالة CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(ival);
                            data++;
                            اكسر;
                        حالة CV_8S:
                            *(حرف*)data = saturate_cast<schar>(ival);
                            data++;
                            اكسر;
                        حالة CV_Bool:
                            *(منطقي*)data = ival != 0;
                            data++;
                            اكسر;
                        حالة CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(ival);
                            data += حجم(ushort);
                            اكسر;
                        حالة CV_16S:
                            *(short*)data = saturate_cast<short>(ival);
                            data += حجم(short);
                            اكسر;
                        حالة CV_32U:
                            *(unsigned*)data = (unsigned)std::max(ival, (int64_t)0);
                            data += حجم(unsigned);
                            اكسر;
                        حالة CV_32S:
                            *(صحيح*)data = (صحيح)ival;
                            data += حجم(صحيح);
                            اكسر;
                        حالة CV_32F:
                            *(عائم*)data = (عائم)ival;
                            data += حجم(عائم);
                            اكسر;
                        حالة CV_64U:
                            *(uint64_t*)data = (uint64_t)ival;
                            data += حجم(uint64_t);
                            اكسر;
                        حالة CV_64S:
                            *(int64_t*)data = (int64_t)ival;
                            data += حجم(int64_t);
                            اكسر;
                        حالة CV_64F:
                            *(مزدوج*)data = (مزدوج)ival;
                            data += حجم(مزدوج);
                            اكسر;
                        حالة CV_16F:
                            *(hfloat*)data = hfloat((عائم)ival);
                            data += حجم(hfloat);
                            اكسر;
                        حالة CV_16BF:
                            *(bfloat*)data = bfloat((عائم)ival);
                            data += حجم(bfloat);
                            اكسر;
                        حالة CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((عائم)ival);
                            data += حجم(fp8_t);
                            اكسر;
                        حالة CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((عائم)ival);
                            data += حجم(fp8a_t);
                            اكسر;
                        افتراضي:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    وإلا 𐡄𐡍( node.isReal() )
                    {
                        مزدوج fval = (مزدوج)node;

                        بدل( elem_type )
                        {
                        حالة CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(fval);
                            data++;
                            اكسر;
                        حالة CV_8S:
                            *(حرف*)data = saturate_cast<schar>(fval);
                            data++;
                            اكسر;
                        حالة CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(fval);
                            data += حجم(ushort);
                            اكسر;
                        حالة CV_16S:
                            *(short*)data = saturate_cast<short>(fval);
                            data += حجم(short);
                            اكسر;
                        حالة CV_32U:
                            *(صحيح*)data = saturate_cast<unsigned>(fval);
                            data += حجم(صحيح);
                            اكسر;
                        حالة CV_32S:
                            *(صحيح*)data = saturate_cast<صحيح>(fval);
                            data += حجم(صحيح);
                            اكسر;
                        حالة CV_32F:
                            *(عائم*)data = (عائم)fval;
                            data += حجم(عائم);
                            اكسر;
                        حالة CV_64U:
                            *(uint64_t*)data = (uint64_t)round(std::max(fval, 0.));
                            data += حجم(uint64_t);
                            اكسر;
                        حالة CV_64S:
                            *(int64_t*)data = (int64_t)round(std::max(fval, 0.));
                            data += حجم(int64_t);
                            اكسر;
                        حالة CV_64F:
                            *(مزدوج*)data = fval;
                            data += حجم(مزدوج);
                            اكسر;
                        حالة CV_16F:
                            *(hfloat*)data = hfloat((عائم)fval);
                            data += حجم(hfloat);
                            اكسر;
                        حالة CV_16BF:
                            *(bfloat*)data = bfloat((عائم)fval);
                            data += حجم(bfloat);
                            اكسر;
                        حالة CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((عائم)fval);
                            data += حجم(fp8_t);
                            اكسر;
                        حالة CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((عائم)fval);
                            data += حجم(fp8a_t);
                            اكسر;
                        افتراضي:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    وإلا
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (صحيح)(data - data0);
            }
        }
    }

    أرجع *هذا;
}

منطقي FileNodeIterator::equalTo(ثابت FileNodeIterator& it) ثابت
{
    أرجع fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && nodeNElems == it.nodeNElems;
}

size_t FileNodeIterator::remaining() ثابت
{
    أرجع nodeNElems - idx;
}

منطقي operator == ( ثابت FileNodeIterator& it1, ثابت FileNodeIterator& it2 )
{
    أرجع it1.equalTo(it2);
}

منطقي operator != ( ثابت FileNodeIterator& it1, ثابت FileNodeIterator& it2 )
{
    أرجع !it1.equalTo(it2);
}

فراغ اقرأ(ثابت FileNode& node, صحيح& val, صحيح default_val)
{
    val = default_val;
    𐡄𐡍( !node.empty() )
    {
        val = (صحيح)node;
    }
}

فراغ اقرأ(ثابت FileNode& node, int64_t& val, int64_t default_val)
{
    val = default_val;
    𐡄𐡍( !node.empty() )
    {
        val = (int64_t)node;
    }
}

فراغ اقرأ(ثابت FileNode& node, مزدوج& val, مزدوج default_val)
{
    val = default_val;
    𐡄𐡍( !node.empty() )
    {
        val = (مزدوج)node;
    }
}

فراغ اقرأ(ثابت FileNode& node, عائم& val, عائم default_val)
{
    val = default_val;
    𐡄𐡍( !node.empty() )
    {
        val = (عائم)node;
    }
}

فراغ اقرأ(ثابت FileNode& node, std::string& val, ثابت std::string& default_val)
{
    val = default_val;
    𐡄𐡍( !node.empty() )
    {
        val = (std::string)node;
    }
}
فراغ FileStorage::Impl::اكتب(ثابت String &key, منطقي value)
{
    CV_Assert(write_mode);
    𐡄𐡍 (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().اكتب(key.c_str(), (صحيح)value);
    }
    وإلا
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().اكتب(key.c_str(), value ? "true" : "false", خطأ);
    }
}

FileStorage_API::~FileStorage_API() {}

نطاق internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, ثابت std::string& name,
                                       صحيح flags, ثابت std::string& typeName)
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
