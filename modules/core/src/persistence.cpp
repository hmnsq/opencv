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
    إذا(!s1) s1=dummy;
    إذا(!s2) s2=dummy;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t i, len = std::min(len1, len2);
    لكل( i = 0; i < len; i++ )
    {
        صحيح d = tolower((صحيح)s1[i]) - tolower((صحيح)s2[i]);
        إذا( d != 0 )
            إرجاع d;
    }
    إرجاع len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

حرف* itoa( صحيح _val, حرف* buffer, صحيح /*radix*/ )
{
    ثابت صحيح radix = 10;
    حرف* ptr=buffer + 23 /* enough even for 64-bit integers */;
    unsigned val = abs(_val);

    *ptr = '\0';
    افعل
    {
        unsigned r = val / radix;
        *--ptr = (حرف)(val - (r*radix) + '0');
        val = r;
    }
    بينما( val != 0 );

    إذا( _val < 0 )
        *--ptr = '-';

    إرجاع ptr;
}

حرف* itoa( int64_t _val, حرف* buffer, صحيح /*radix*/, منطقي _signed)
{
    ثابت صحيح radix = 10;
    حرف* ptr=buffer + 23 /* enough even for 64-bit integers */;
    صحيح sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : abs(_val);

    *ptr = '\0';
    افعل
    {
        uint64_t r = val / radix;
        *--ptr = (حرف)(val - (r*radix) + '0');
        val = r;
    }
    بينما( val != 0 );

    إذا( sign < 0 )
        *--ptr = '-';

    إرجاع ptr;
}

حرف* doubleToString( حرف* buf, size_t bufSize, مزدوج value, منطقي explicitZero )
{
    Cv64suf val;
    unsigned ieee754_hi;

    val.f = value;
    ieee754_hi = (unsigned)(val.u >> 32);

    إذا( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        صحيح ivalue = cvRound(value);
        إذا( ivalue == value )
        {
            إذا( explicitZero )
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
            إذا( *ptr == '+' || *ptr == '-' )
                ptr++;
            لكل( ; cv_isdigit(*ptr); ptr++ )
                ;
            إذا( *ptr == ',' )
                *ptr = '.';
        }
    }
    وإلا
    {
        unsigned ieee754_lo = (unsigned)val.u;
        إذا( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        وإلا
            strcpy( buf, (صحيح)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    إرجاع buf;
}

حرف* floatToString( حرف* buf, size_t bufSize, عائم value, منطقي halfprecision, منطقي explicitZero )
{
    Cv32suf val;
    unsigned ieee754;
    val.f = value;
    ieee754 = val.u;

    إذا( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        صحيح ivalue = cvRound(value);
        إذا( ivalue == value )
        {
            إذا( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            وإلا
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        وإلا
        {
            إذا (halfprecision)
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
            إذا( *ptr == '+' || *ptr == '-' )
                ptr++;
            لكل( ; cv_isdigit(*ptr); ptr++ )
                ;
            إذا( *ptr == ',' )
                *ptr = '.';
        }
    }
    وإلا
    {
        إذا( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        وإلا
            strcpy( buf, (صحيح)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    إرجاع buf;
}

ساكن ثابت حرف symbols[] = "ucwsifdhHbUIneE";

ساكن حرف type_رمز(صحيح depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    إرجاع symbols[depth];
}

ساكن صحيح رمز_To_Type(حرف c)
{
    إذا (c == 'r')
        إرجاع CV_SEQ_ELTYPE_PTR;
    ثابت حرف* pos = strchr( symbols, c );
    إذا( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    إرجاع static_cast<صحيح>(pos - symbols);
}

حرف* encodeFormat(صحيح elem_type, حرف* dt, size_t dt_len)
{
    صحيح cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    حرف رمز = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_رمز(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, رمز);
    إرجاع dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
حرف* encodeFormat(صحيح elem_type, حرف* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    إرجاع encodeFormat(elem_type, dt, max);
}

صحيح decodeFormat( ثابت حرف* dt, صحيح* fmt_pairs, صحيح max_len )
{
    صحيح fmt_pair_تعداد = 0;
    صحيح i = 0, k = 0, len = dt ? (صحيح)strlen(dt) : 0;

    إذا( !dt || !len )
        إرجاع 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    لكل( ; k < len; k++ )
    {
        حرف c = dt[k];

        إذا( cv_isdigit(c) )
        {
            صحيح تعداد_3 = c - '0';
            إذا( cv_isdigit(dt[k+1]) )
            {
                حرف* endptr = 0;
                تعداد_3 = (صحيح)strtol( dt+k, &endptr, 10 );
                k = (صحيح)(endptr - dt) - 1;
            }

            إذا( تعداد_3 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = تعداد_3;
        }
        وإلا
        {
            صحيح depth = رمز_To_Type(c);
            إذا( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = depth;
            إذا( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            وإلا
            {
                i += 2;
                إذا( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_تعداد = i/2;
    إرجاع fmt_pair_تعداد;
}

صحيح calcElemSize( ثابت حرف* dt, صحيح initial_size )
{
    صحيح size = 0;
    صحيح fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_تعداد;
    صحيح comp_size;

    fmt_pair_تعداد = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_تعداد *= 2;
    لكل( i = 0, size = initial_size; i < fmt_pair_تعداد; i += 2 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[i+1]);
        size = cvAlign( size, comp_size );
        size += comp_size * fmt_pairs[i];
    }
    إذا( initial_size == 0 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[1]);
        size = cvAlign( size, comp_size );
    }
    إرجاع size;
}


صحيح calcStructSize( ثابت حرف* dt, صحيح initial_size )
{
    صحيح size = calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    لكل ( ثابت حرف * type = dt; *type != '\0'; type++ )
    {
        حرف v = *type;
        إذا (v >= '0' && v <= '9')
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
    إرجاع size;
}

صحيح decodeSimpleFormat( ثابت حرف* dt )
{
    صحيح elem_type = -1;
    صحيح fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_تعداد;

    fmt_pair_تعداد = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    إذا( fmt_pair_تعداد != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    إرجاع elem_type;
}

}

#إذا defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#define CV_LITTLE_ENDIAN_MEM_ACCESS 1
#وإلا
#define CV_LITTLE_ENDIAN_MEM_ACCESS 0
#endif

ساكن مضمن صحيح readInt(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#إذا CV_LITTLE_ENDIAN_MEM_ACCESS
    صحيح val;
    memcpy(&val, p, حجم(val));
    إرجاع val;
#وإلا
    صحيح val = (صحيح)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    إرجاع val;
#endif
}

ساكن مضمن int64_t readLong(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#إذا CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    memcpy(&val, p, حجم(val));
    إرجاع val;
#وإلا
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    إرجاع val0 | ((int64_t)val1 << 32);
#endif
}

ساكن مضمن مزدوج readReal(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#إذا CV_LITTLE_ENDIAN_MEM_ACCESS
    مزدوج val;
    memcpy(&val, p, حجم(val));
    إرجاع val;
#وإلا
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    إرجاع val.f;
#endif
}

template <typename T>
ساكن مضمن فراغ writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#إذا CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &ival, حجم(ival));
#وإلا
    لكل (size_t i = 0, j = 0; i < حجم(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#endif
}

ساكن مضمن فراغ writeReal(uchar* p, مزدوج fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#إذا CV_LITTLE_ENDIAN_MEM_ACCESS
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
    delayed_struct_مفتاح = nullptr;
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
    empty_stream = true;

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
    إذا (is_opened) {
        إذا (out)
            out->clear();
        إذا (write_mode) {
            بينما (write_مكدس.size() > 1) {
                endWriteStruct();
            }
            flush();
            إذا (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            وإلا إذا (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        إذا (mem_mode && out) {
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

    إذا (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        إرجاع;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    إذا (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        لكل (size_t param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            إذا ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

منطقي FileStorage::Impl::open(ثابت حرف *filename_or_buf, صحيح _flags, ثابت حرف *encoding) {
    منطقي ok = true;
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
        إذا (!params.empty())
            filename = params[0];

        إذا (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_mode || append);
    }

    إذا (filename.size() == 0 && !mem_mode && !write_mode)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    إذا (mem_mode && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    إذا (!mem_mode) {
        size_t dot_idx = filename.find_last_of('.');
        حرف compression = '\0';

        إذا (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            إذا (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                إذا (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = true;
                }

                وإلا إذا(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = true;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        إذا (!isGZ) {
            file = fopen(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            إذا (!file)
            {
                CV_LOG_ERROR(عدم, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                إرجاع خطأ;
            }
        } وإلا {
#إذا USE_ZLIB
            إذا (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            حرف mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            إذا (!gzfile)
            {
                CV_LOG_ERROR(عدم, "Can't open archive: '" << filename << "' mode=" << mode);
                إرجاع خطأ;
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

    إذا (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        إذا (mem_mode)
            outbuf.clear();

        إذا (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            ثابت حرف *dot_pos = عدم;
            ثابت حرف *dot_pos2 = عدم;
            // like strrchr() implementation, but save two last positions simultaneously
            لكل (ثابت حرف *pos = &filename[0]; pos[0] != 0; pos++) {
                إذا (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            إذا (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != عدم) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } وإلا إذا (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        صحيح buf_size = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        إذا (append) {
            fseek(file, 0, SEEK_END);
            إذا (ftell(file) == 0)
                append = خطأ;
        }

        write_مكدس.clear();
        empty_stream = true;
        write_مكدس.push_back(FStructData("", File_عقدة::MAP | File_عقدة::EMPTY, 0));
        buffer.reserve(buf_size + 1024);
        buffer.resize(buf_size);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;

        إذا (fmt == FileStorage::FORMAT_XML) {
            size_t file_size = file ? (size_t) ftell(file) : (size_t) 0;
            إذا (!append || file_size == 0) {
                إذا (encoding && *encoding != '\0') {
                    إذا (fs::strcasecmp(encoding, "UTF-16") == 0) {
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
                    صحيح سطر_offset = (صحيح) ftell(file);
                    ثابت حرف *ptr0 = هذا->gets(xml_buf_size);
                    ثابت حرف *ptr = عدم;
                    إذا (!ptr0)
                        اكسر;
                    ptr = ptr0;
                    لكل (;;) {
                        ptr = strstr(ptr, substr);
                        إذا (!ptr)
                            اكسر;
                        last_occurrence = سطر_offset + (صحيح) (ptr - ptr0);
                        ptr += strlen(substr);
                    }
                }
                إذا (last_occurrence < 0) {
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
        } وإلا إذا (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                إذا (!append) {
                    إذا (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    وإلا
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            وإلا
                puts("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(هذا);
        } وإلا {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            إذا (!append)
                puts("{\n");
            وإلا {
                منطقي valid = خطأ;
                long roffset = 0;
                لكل (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    ثابت حرف end_mark = '}';
                    إذا (fgetc(file) == end_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = true;
                        اكسر;
                    }
                }

                إذا (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } وإلا {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_مكدس.back().indent = 4;
            emitter_do_not_use_direct_dereference = createJSONEmitter(هذا);
        }
        is_opened = true;
    } وإلا {
        ثابت size_t buf_size0 = 40;
        buffer.resize(buf_size0);
        إذا (mem_mode) {
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
        إذا (strncmp(bufPtr, yaml_signature, strlen(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        وإلا إذا (strncmp(bufPtr, json_signature, strlen(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        وإلا إذا (strncmp(bufPtr, xml_signature, strlen(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        وإلا
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = bufOffset;
        bufofs = 0;

        حاول {
            حرف *ptr = bufferStart();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_عقدة جذر_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_عقدة_Space(جذر_nodes, 9);
            *rptr = File_عقدة::SEQ;
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

            إذا (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                إذا (ok) {
                    finalizeCollection(جذر_nodes);

                    CV_Assert(!fs_data_ptrs.empty());
                    File_عقدة roots_عقدة(fs_ext, 0, 0);
                    size_t i, nroots = roots_عقدة.size();
                    File_عقدة_Iterator it = roots_عقدة.begin();

                    لكل (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        امسك (...)
        {
            // FIXIT log error message
            is_opened = true;
            release();
            ارم;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = true;
        std::vector<حرف> tmpbuf;
        std::swap(buffer, tmpbuf);
        bufofs = 0;
    }
    إرجاع ok;
}

فراغ FileStorage::Impl::puts(ثابت حرف *str) {
    CV_Assert(write_mode);
    إذا (mem_mode)
        std::copy(str, str + strlen(str), std::back_inserter(outbuf));
    وإلا إذا (file)
        fputs(str, file);
#إذا USE_ZLIB
    وإلا إذا (gzfile)
        gzputs(gzfile, str);
#endif
    وإلا
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

حرف *FileStorage::Impl::getsFromFile(حرف *buf, صحيح تعداد_3) {
    إذا (file)
        إرجاع fgets(buf, تعداد_3, file);
#إذا USE_ZLIB
    إذا (gzfile)
        إرجاع gzgets(gzfile, buf, تعداد_3);
#endif
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

حرف *FileStorage::Impl::gets(size_t max_تعداد) {
    إذا (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        ثابت حرف *instr = strbuf;
        لكل (; i < len; i++) {
            حرف c = instr[i];
            إذا (c == '\0' || c == '\n') {
                إذا (c == '\n')
                    i++;
                اكسر;
            }
        }
        size_t تعداد_3 = i - strbufpos;
        إذا (max_تعداد == 0 || max_تعداد > تعداد_3)
            max_تعداد = تعداد_3;
        buffer.resize(std::max(buffer.size(), max_تعداد + 8));
        memcpy(&buffer[0], instr + strbufpos, max_تعداد);
        buffer[max_تعداد] = '\0';
        strbufpos = i;
        إرجاع max_تعداد > 0 ? &buffer[0] : 0;
    }

    ثابت size_t MAX_BLOCK_SIZE = INT_MAX / 2; // hopefully, that will be enough
    إذا (max_تعداد == 0)
        max_تعداد = MAX_BLOCK_SIZE;
    وإلا
        CV_Assert(max_تعداد < MAX_BLOCK_SIZE);
    size_t ofs = 0;

    لكل (;;) {
        صحيح تعداد_3 = (صحيح) std::min(buffer.size() - ofs - 16, max_تعداد);
        حرف *ptr = getsFromFile(&buffer[ofs], تعداد_3 + 1);
        إذا (!ptr)
            اكسر;
        صحيح delta = (صحيح) strlen(ptr);
        ofs += delta;
        max_تعداد -= delta;
        إذا (delta == 0 || ptr[delta - 1] == '\n' || max_تعداد == 0)
            اكسر;
        إذا (delta == تعداد_3)
            buffer.resize((size_t) (buffer.size() * 1.5));
    }
    إرجاع ofs > 0 ? &buffer[0] : 0;
}

حرف *FileStorage::Impl::gets() {
    حرف *ptr = هذا->gets(0);
    إذا (!ptr) {
        ptr = bufferStart();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        إرجاع 0;
    } وإلا {
        size_t l = strlen(ptr);
        إذا (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    إرجاع ptr;
}

منطقي FileStorage::Impl::eof() {
    إذا (dummy_eof)
        إرجاع true;
    إذا (strbuf)
        إرجاع strbufpos >= strbufsize;
    إذا (file)
        إرجاع feof(file) != 0;
#إذا USE_ZLIB
    إذا (gzfile)
        إرجاع gzeof(gzfile) != 0;
#endif
    إرجاع خطأ;
}

فراغ FileStorage::Impl::setEof() {
    dummy_eof = true;
}

فراغ FileStorage::Impl::closeFile() {
    إذا (file)
        fclose(file);
#إذا USE_ZLIB
    وإلا إذا (gzfile)
        gzclose(gzfile);
#endif
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = خطأ;
}

فراغ FileStorage::Impl::rewind() {
    إذا (file)
        ::rewind(file);
#إذا USE_ZLIB
    وإلا إذا (gzfile)
        gzrewind(gzfile);
#endif
    strbufpos = 0;
}

حرف *FileStorage::Impl::resizeWriteBuffer(حرف *ptr, صحيح len) {
    ثابت حرف *buffer_end = &buffer[0] + buffer.size();
    إذا (ptr + len < buffer_end)
        إرجاع ptr;

    ثابت حرف *buffer_start = &buffer[0];
    صحيح written_len = (صحيح) (ptr - buffer_start);

    CV_Assert(written_len <= (صحيح) buffer.size());
    صحيح new_size = (صحيح) ((buffer_end - buffer_start) * 3 / 2);
    new_size = MAX(written_len + len, new_size);
    buffer.reserve(new_size + 256);
    buffer.resize(new_size);
    bufofs = written_len;
    إرجاع &buffer[0] + bufofs;
}

حرف *FileStorage::Impl::flush() {
    حرف *buffer_start = &buffer[0];
    حرف *ptr = buffer_start + bufofs;

    إذا (ptr > buffer_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(buffer_start);
        bufofs = 0;
    }

    صحيح indent = write_مكدس.back().indent;

    إذا (space != indent) {
        memset(buffer_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = buffer_start + bufofs;

    إرجاع ptr;
}

فراغ FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(خطأ);
    إذا (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_مكدس.empty());

    FStructData &current_struct = write_مكدس.back();
    إذا (fmt == FileStorage::FORMAT_JSON && !File_عقدة::isFlow(current_struct.flags) && write_مكدس.size() > 1)
        current_struct.indent = write_مكدس[write_مكدس.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_مكدس.pop_back();
    إذا (!write_مكدس.empty())
        write_مكدس.back().flags &= ~File_عقدة::EMPTY;
}

فراغ FileStorage::Impl::startWriteStruct_helper(ثابت حرف *مفتاح, صحيح struct_flags,
                                                ثابت حرف *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (File_عقدة::TYPE_MASK | File_عقدة::FLOW)) | File_عقدة::EMPTY;
    إذا (!File_عقدة::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    إذا (type_name && type_name[0] == '\0')
        type_name = 0;

    FStructData s = getEmitter().startWriteStruct(write_مكدس.back(), مفتاح, struct_flags, type_name);

    write_مكدس.push_back(s);
    size_t write_مكدس_size = write_مكدس.size();
    إذا (write_مكدس_size > 1)
        write_مكدس[write_مكدس_size - 2].flags &= ~File_عقدة::EMPTY;

    إذا (fmt != FileStorage::FORMAT_JSON && !File_عقدة::isFlow(s.flags))
        flush();

    إذا (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_عقدة::isMap(struct_flags)) {
        getEmitter().اكتب("type_id", type_name, خطأ);
    }
}

فراغ FileStorage::Impl::startWriteStruct(ثابت حرف *مفتاح, صحيح struct_flags,
                                         ثابت حرف *type_name) {
    check_if_write_struct_is_delayed(خطأ);
    إذا (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    إذا (state_of_writing_base64 == FileStorage_API::Uncertain && File_عقدة::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(مفتاح, struct_flags, type_name);
    } وإلا إذا (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        إذا ((File_عقدة::TYPE_MASK & struct_flags) != File_عقدة::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        وإلا إذا (state_of_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(مفتاح, struct_flags, "binary");

        إذا (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::InUse);
    } وإلا {
        /* Won't output Base64 data */
        إذا (state_of_writing_base64 == FileStorage_API::InUse)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(مفتاح, struct_flags, type_name);

        إذا (state_of_writing_base64 != FileStorage_API::Uncertain)
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
    إذا (!empty_stream) {
        بينما (!write_مكدس.empty())
            endWriteStruct();
        flush();
        getEmitter().startNextStream();
        empty_stream = true;
        write_مكدس.push_back(FStructData("", File_عقدة::EMPTY, 0));
        bufofs = 0;
    }
}

فراغ FileStorage::Impl::اكتب(ثابت String &مفتاح, صحيح value) {
    CV_Assert(write_mode);
    getEmitter().اكتب(مفتاح.c_str(), value);
}

فراغ FileStorage::Impl::اكتب(ثابت String &مفتاح, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().اكتب(مفتاح.c_str(), value);
}

فراغ FileStorage::Impl::اكتب(ثابت String &مفتاح, مزدوج value) {
    CV_Assert(write_mode);
    getEmitter().اكتب(مفتاح.c_str(), value);
}

فراغ FileStorage::Impl::اكتب(ثابت String &مفتاح, ثابت String &value) {
    CV_Assert(write_mode);
    getEmitter().اكتب(مفتاح.c_str(), value.c_str(), خطأ);
}

فراغ FileStorage::Impl::writeRawData(ثابت std::string &dt, ثابت فراغ *_data, size_t len) {
    CV_Assert(write_mode);

    إذا (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        writeRawDataBase64(_data, len, dt.c_str());
        إرجاع;
    } وإلا إذا (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elemSize = fs::calcStructSize(dt.c_str(), 0);
    CV_Assert(elemSize);
    CV_Assert(len % elemSize == 0);
    len /= elemSize;

    منطقي explicitZero = fmt == FileStorage::FORMAT_JSON;
    ثابت uchar *data0 = (ثابت uchar *) _data;
    صحيح fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_تعداد;
    حرف buf[256] = "";

    fmt_pair_تعداد = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    إذا (!len)
        إرجاع;

    إذا (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    إذا (fmt_pair_تعداد == 1) {
        fmt_pairs[0] *= (صحيح) len;
        len = 1;
    }

    لكل (; len--; data0 += elemSize) {
        صحيح offset = 0;
        لكل (k = 0; k < fmt_pair_تعداد; k++) {
            صحيح i, تعداد_3 = fmt_pairs[k * 2];
            صحيح elem_type = fmt_pairs[k * 2 + 1];
            صحيح elem_size = CV_ELEM_SIZE(elem_type);
            ثابت حرف *ptr;

            offset = cvAlign(offset, elem_size);
            ثابت uchar *data = data0 + offset;

            لكل (i = 0; i < تعداد_3; i++) {
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
                        ptr = fs::itoa(*(int64_t*) data, buf, 10, true);
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
                        ptr = fs::floatToString(buf, حجم(buf), (عائم) *(hfloat *) data, true, explicitZero);
                        data += حجم(hfloat);
                        اكسر;
                    حالة CV_16BF:
                        ptr = fs::floatToString(buf, حجم(buf), (عائم) *(bfloat *) data, true, explicitZero);
                        data += حجم(bfloat);
                        اكسر;
                    حالة CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, حجم(buf), (عائم) *(fp8_t *) data, true, explicitZero);
                        data += حجم(fp8_t);
                        اكسر;
                    حالة CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, حجم(buf), (عائم) *(fp8a_t *) data, true, explicitZero);
                        data += حجم(fp8a_t);
                        اكسر;
                    افتراضي:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        إرجاع;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (صحيح) (data - data0);
        }
    }
}

فراغ FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(خطأ);

    إذا (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
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
                    إذا (!can_indent) {
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
                    إذا ( fmt == cv::FileStorage::FORMAT_JSON )
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

فراغ FileStorage::Impl::make_write_struct_delayed(ثابت حرف *مفتاح, صحيح struct_flags, ثابت حرف *type_name) {
    CV_Assert(is_write_struct_delayed == خطأ);
    CV_DbgAssert(delayed_struct_مفتاح == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    إذا (مفتاح != nullptr) {
        delayed_struct_مفتاح = جديد حرف[strlen(مفتاح) + 1U];
        strcpy(delayed_struct_مفتاح, مفتاح);
    }

    إذا (type_name != nullptr) {
        delayed_type_name = جديد حرف[strlen(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = true;
}

فراغ FileStorage::Impl::check_if_write_struct_is_delayed(منطقي change_type_to_base64) {
    إذا (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_مفتاح;
        std::string type_name;
        صحيح struct_flags = delayed_struct_flags;

        إذا (delayed_struct_مفتاح != nullptr && *delayed_struct_مفتاح != '\0') {
            struct_مفتاح.assign(delayed_struct_مفتاح);
        }
        إذا (delayed_type_name != nullptr && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        delete[] delayed_struct_مفتاح;
        delete[] delayed_type_name;
        delayed_struct_مفتاح = nullptr;
        delayed_struct_flags = 0;
        delayed_type_name = nullptr;

        is_write_struct_delayed = خطأ;

        /* call */
        إذا (change_type_to_base64) {
            startWriteStruct_helper(struct_مفتاح.c_str(), struct_flags, "binary");
            إذا (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::InUse);
        } وإلا {
            startWriteStruct_helper(struct_مفتاح.c_str(), struct_flags, type_name.c_str());
            إذا (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::NotUse);
        }
    }
}

فراغ FileStorage::Impl::writeRawDataBase64(ثابت فراغ *_data, size_t len, ثابت حرف *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(true);

    إذا (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } وإلا إذا (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->اكتب(_data, len, dt);
}

File_عقدة FileStorage::Impl::get_First_قمة_Level_عقدة() ثابت {
    إرجاع roots.empty() ? File_عقدة() : roots[0];
}

File_عقدة FileStorage::Impl::جذر(صحيح streamIdx) ثابت {
    إرجاع streamIdx >= 0 && streamIdx < (صحيح) roots.size() ? roots[streamIdx] : File_عقدة();
}

File_عقدة FileStorage::Impl::operator[](ثابت String &nodename) ثابت {
    إرجاع هذا->operator[](nodename.c_str());
}

File_عقدة FileStorage::Impl::operator[](ثابت حرف * /*nodename*/) ثابت {
    إرجاع File_عقدة();
}

صحيح FileStorage::Impl::getFormat() ثابت { إرجاع fmt; }

حرف *FileStorage::Impl::bufferPtr() ثابت { إرجاع (حرف *) (&buffer[0] + bufofs); }

حرف *FileStorage::Impl::bufferStart() ثابت { إرجاع (حرف *) &buffer[0]; }

حرف *FileStorage::Impl::bufferEnd() ثابت { إرجاع (حرف *) (&buffer[0] + buffer.size()); }

فراغ FileStorage::Impl::setBufferPtr(حرف *ptr) {
    حرف *bufferstart = bufferStart();
    CV_Assert(ptr >= bufferstart && ptr <= bufferEnd());
    bufofs = ptr - bufferstart;
}

صحيح FileStorage::Impl::wrapMargin() ثابت { إرجاع wrap_margin; }

FStructData &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_مكدس.empty());
    إرجاع write_مكدس.back();
}

فراغ FileStorage::Impl::setNonEmpty() {
    empty_stream = خطأ;
}

فراغ FileStorage::Impl::processSpecialDouble(حرف *buf, مزدوج *value, حرف **endptr) {
    FileStorage_API *fs = هذا;
    حرف c = buf[0];
    صحيح inf_hi = 0x7ff00000;

    إذا (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    إذا (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    إذا (toupper(buf[1]) == 'I' && toupper(buf[2]) == 'N' && toupper(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    وإلا إذا (toupper(buf[1]) == 'N' && toupper(buf[2]) == 'A' && toupper(buf[3]) == 'N')
        v.u = (uint64) -1;
    وإلا
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

مزدوج FileStorage::Impl::strtod(حرف *ptr, حرف **endptr) {
    مزدوج fval = ::strtod(ptr, endptr);
    إذا (**endptr == '.') {
        حرف *dot_pos = *endptr;
        *dot_pos = ',';
        مزدوج fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        إذا (*endptr > dot_pos)
            fval = fval2;
        وإلا
            *endptr = dot_pos;
    }

    إذا (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    إرجاع fval;
}

فراغ FileStorage::Impl::convertToCollection(صحيح type, File_عقدة &عقدة) {
    CV_Assert(type == File_عقدة::SEQ || type == File_عقدة::MAP);

    صحيح عقدة_type = عقدة.type();
    إذا (عقدة_type == type)
        إرجاع;

    منطقي named = عقدة.isNamed();
    uchar *ptr = عقدة.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    مزدوج fval = 0;
    std::string sval;
    منطقي add_first_scalar = خطأ;

    إذا (عقدة_type != File_عقدة::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_عقدة::SEQ);
        إذا (عقدة_type == File_عقدة::INT) {
            ival = readLong(ptr);
            add_first_scalar = true;
        } وإلا إذا (عقدة_type == File_عقدة::REAL) {
            fval = readReal(ptr);
            add_first_scalar = true;
        } وإلا إذا (عقدة_type == File_عقدة::STRING) {
            sval = std::string(عقدة);
            add_first_scalar = true;
        } وإلا
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", عقدة_type));
    }

    ptr = reserve_عقدة_Space(عقدة, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_عقدة::NAMED : 0));
    // name has been copied automatically
    إذا (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    إذا (add_first_scalar)
        add_عقدة(عقدة, std::string(), عقدة_type,
                عقدة_type == File_عقدة::INT ? (ثابت فراغ *) &ival :
                عقدة_type == File_عقدة::REAL ? (ثابت فراغ *) &fval :
                عقدة_type == File_عقدة::STRING ? (ثابت فراغ *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_عقدة_Space(File_عقدة &عقدة, size_t sz) {
    منطقي shrinkBlock = خطأ;
    size_t shrinkBlockIdx = 0, shrinkSize = 0;

    uchar *ptr = 0, *blockEnd = 0;

    إذا (!fs_data_ptrs.empty()) {
        size_t blockIdx = عقدة.blockIdx;
        size_t ofs = عقدة.ofs;
        CV_Assert(blockIdx == fs_data_ptrs.size() - 1);
        CV_Assert(ofs <= fs_data_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_data_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_data_ptrs[blockIdx] + ofs;
        blockEnd = fs_data_ptrs[blockIdx] + fs_data_blksz[blockIdx];

        CV_Assert(ptr >= fs_data_ptrs[blockIdx] && ptr <= blockEnd);
        إذا (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            إرجاع ptr;
        }

        إذا (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_data[blockIdx]->resize(sz);
            ptr = &fs_data[blockIdx]->at(0);
            fs_data_ptrs[blockIdx] = ptr;
            fs_data_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            إرجاع ptr;
        }

        shrinkBlock = true;
        shrinkBlockIdx = blockIdx;
        shrinkSize = ofs;
    }

    size_t blockSize = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(blockSize);
    fs_data.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_data_ptrs.push_back(new_ptr);
    fs_data_blksz.push_back(blockSize);
    عقدة.blockIdx = fs_data_ptrs.size() - 1;
    عقدة.ofs = 0;
    freeSpaceOfs = sz;

    إذا (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        إذا (ptr[0] & File_عقدة::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    إذا (shrinkBlock) {
        fs_data[shrinkBlockIdx]->resize(shrinkSize);
        fs_data_blksz[shrinkBlockIdx] = shrinkSize;
    }

    إرجاع new_ptr;
}

unsigned FileStorage::Impl::getStringOfs(ثابت std::string &مفتاح) ثابت {
    str_hash_t::const_iterator it = str_hash.find(مفتاح);
    إرجاع it != str_hash.end() ? it->second : 0;
}

File_عقدة FileStorage::Impl::add_عقدة(File_عقدة &collection, ثابت std::string &مفتاح,
                                    صحيح elem_type, ثابت فراغ *value, صحيح len) {
    FileStorage_API *fs = هذا;
    منطقي noname = مفتاح.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(مفتاح.c_str(), "_") == 0);
    convertToCollection(noname ? File_عقدة::SEQ : File_عقدة::MAP, collection);

    منطقي isseq = collection.empty() ? خطأ : collection.isSeq();
    إذا (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    unsigned strofs = 0;
    إذا (!noname) {
        strofs = getStringOfs(مفتاح);
        إذا (!strofs) {
            strofs = (unsigned) str_hash_data.size();
            size_t keysize = مفتاح.size() + 1;
            str_hash_data.resize(strofs + keysize);
            memcpy(&str_hash_data[0] + strofs, &مفتاح[0], keysize);
            str_hash.insert(std::make_pair(مفتاح, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t blockIdx = fs_data_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_عقدة عقدة(fs_ext, blockIdx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_عقدة_Space(عقدة, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_عقدة::NAMED));
    إذا (elem_type == File_عقدة::NONE)
        freeSpaceOfs -= 8;

    إذا (!noname) {
        writeInt(ptr, (صحيح) strofs);
        ptr += 4;
    }

    إذا (elem_type == File_عقدة::SEQ || elem_type == File_عقدة::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    إذا (value)
        عقدة.set_قيمة(elem_type, value, len);

    إذا (collection.isNamed())
        cp += 4;
    صحيح nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    إرجاع عقدة;
}

فراغ FileStorage::Impl::finalizeCollection(File_عقدة &collection) {
    إذا (!collection.isSeq() && !collection.isMap())
        إرجاع;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    إذا (*ptr0 & File_عقدة::NAMED)
        ptr += 4;
    size_t blockIdx = collection.blockIdx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t rawSize = 4;
    unsigned sz = (unsigned) readInt(ptr + 4);
    إذا (sz > 0) {
        size_t lastBlockIdx = fs_data_ptrs.size() - 1;

        لكل (; blockIdx < lastBlockIdx; blockIdx++) {
            rawSize += fs_data_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    rawSize += freeSpaceOfs - ofs;
    writeInt(ptr, (صحيح) rawSize);
}

فراغ FileStorage::Impl::normalize_عقدة_Ofs(size_t &blockIdx, size_t &ofs) ثابت {
    بينما (ofs >= fs_data_blksz[blockIdx]) {
        إذا (blockIdx == fs_data_blksz.size() - 1) {
            CV_Assert(ofs == fs_data_blksz[blockIdx]);
            اكسر;
        }
        ofs -= fs_data_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    إرجاع state_of_writing_base64;
}

صحيح FileStorage::Impl::get_space() {
    إرجاع space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = true;
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

    إذا (eos)
        إرجاع خطأ;

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

    إذا (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = true;
        size_t tc = totalchars;
        لكل (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    صحيح i = 0, j, n = (صحيح) encoded.size();
    إذا (n > 0) {
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

    إذا (i > 0 && encoded[i - 1] == '=') {
        إذا (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        إذا (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    لكل (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    إرجاع (صحيح) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    إذا (ofs >= sz && !readMore(1))
        إرجاع (uchar) 0;
    إرجاع decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    إذا (ofs + 2 > sz && !readMore(2))
        إرجاع (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    إرجاع val;
}

صحيح FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    إذا (ofs + 4 > sz && !readMore(4))
        إرجاع 0;
    صحيح ival = readInt(&decoded[ofs]);
    ofs += 4;
    إرجاع ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    إذا (ofs + 8 > sz && !readMore(8))
        إرجاع 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    إرجاع ival;
}

مزدوج FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    إذا (ofs + 8 > sz && !readMore(8))
        إرجاع 0;
    مزدوج fval = readReal(&decoded[ofs]);
    ofs += 8;
    إرجاع fval;
}

منطقي FileStorage::Impl::Base64Decoder::endOfStream() ثابت { إرجاع eos; }

حرف *FileStorage::Impl::Base64Decoder::getPtr() ثابت { إرجاع ptr; }


حرف *FileStorage::Impl::parseBase64(حرف *ptr, صحيح indent, File_عقدة &collection) {
    ثابت صحيح BASE64_HDR_SIZE = 24;
    حرف dt[BASE64_HDR_SIZE + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    صحيح i, k;

    لكل (i = 0; i < BASE64_HDR_SIZE; i++)
        dt[i] = (حرف) base64decoder.getUInt8();
    لكل (i = 0; i < BASE64_HDR_SIZE; i++)
        إذا (isspace(dt[i]))
            اكسر;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    صحيح fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    صحيح fmt_pair_تعداد = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    مزدوج fval = 0;

    لكل (;;) {
        لكل (k = 0; k < fmt_pair_تعداد; k++) {
            صحيح elem_type = fmt_pairs[k * 2 + 1];
            صحيح تعداد_3 = fmt_pairs[k * 2];

            لكل (i = 0; i < تعداد_3; i++) {
                صحيح عقدة_type = File_عقدة::INT;
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
                        عقدة_type = File_عقدة::REAL;
                    }
                        اكسر;
                    حالة CV_64F:
                        fval = base64decoder.getFloat64();
                        عقدة_type = File_عقدة::REAL;
                        اكسر;
                    حالة CV_16F:
                        fval = عائم(hfloatFromBits(base64decoder.getUInt16()));
                        عقدة_type = File_عقدة::REAL;
                        اكسر;
                    حالة CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        عقدة_type = File_عقدة::REAL;
                        اكسر;
                    حالة CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        عقدة_type = File_عقدة::REAL;
                        اكسر;
                    افتراضي:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                إذا (base64decoder.endOfStream())
                    اكسر;
                add_عقدة(collection, std::string(), عقدة_type,
                        عقدة_type == File_عقدة::INT ? (فراغ *) &ival : (فراغ *) &fval, -1);
            }
        }
        إذا (base64decoder.endOfStream())
            اكسر;
    }

    finalizeCollection(collection);
    إرجاع base64decoder.getPtr();
}

فراغ FileStorage::Impl::parseError(ثابت حرف *func_name, ثابت std::string &err_رسالة, ثابت حرف *source_file,
                                   صحيح source_سطر) {
    std::string رسالة = format("%s(%d): %s", filename.c_str(), lineno, err_رسالة.c_str());
    error(Error::StsParseError, func_name, رسالة.c_str(), source_file, source_سطر);
}

ثابت uchar *FileStorage::Impl::get_عقدة_Ptr(size_t blockIdx, size_t ofs) ثابت {
    CV_Assert(blockIdx < fs_data_ptrs.size());
    CV_Assert(ofs < fs_data_blksz[blockIdx]);

    إرجاع fs_data_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) ثابت {
    CV_Assert(nameofs < str_hash_data.size());
    إرجاع std::string(&str_hash_data[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { إرجاع fs_ext; }


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
    إذا(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

فراغ FileStorage::startWriteStruct(ثابت String& name, صحيح struct_flags, ثابت String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    إذا ((struct_flags & File_عقدة::TYPE_MASK) == File_عقدة::SEQ)
        state = FileStorage::قيمة_EXPECTED;
    وإلا
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

فراغ FileStorage::endWriteStruct()
{
    p->endWriteStruct();
    state = p->write_مكدس.empty() || File_عقدة::isMap(p->write_مكدس.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::قيمة_EXPECTED;
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
        إذا(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        إرجاع ok;
    }
    امسك (...)
    {
        release();
        ارم;  // re-throw
    }
}

منطقي FileStorage::isOpened() ثابت { إرجاع p->is_opened; }

فراغ FileStorage::release()
{
    p->release();
}

File_عقدة FileStorage::جذر(صحيح i) ثابت
{
    إذا( p.empty() || p->roots.empty() || i < 0 || i >= (صحيح)p->roots.size() )
        إرجاع File_عقدة();

    إرجاع p->roots[i];
}

File_عقدة FileStorage::get_First_قمة_Level_عقدة() ثابت
{
    File_عقدة r = جذر();
    File_عقدة_Iterator it = r.begin();
    إرجاع it != r.end() ? *it : File_عقدة();
}

std::string FileStorage::getDefaultObjectName(ثابت std::string& _filename)
{
    ساكن ثابت حرف* stubname = "unnamed";
    ثابت حرف* filename = _filename.c_str();
    ثابت حرف* ptr2 = filename + _filename.size();
    ثابت حرف* ptr = ptr2 - 1;
    cv::AutoBuffer<حرف> name_buf(_filename.size()+1);

    بينما( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        إذا( *ptr == '.' && (!*ptr2 || strncmp(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    إذا( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    حرف* name = name_buf.data();

    // name must start with letter or '_'
    إذا( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    بينما( ptr < ptr2 )
    {
        حرف c = *ptr++;
        إذا( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.data();
    إذا( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    إرجاع name;
}


صحيح FileStorage::getFormat() ثابت
{
    إرجاع p->fmt;
}

File_عقدة FileStorage::operator [](ثابت حرف* مفتاح) ثابت
{
    إرجاع هذا->operator[](std::string(مفتاح));
}

File_عقدة FileStorage::operator [](ثابت std::string& مفتاح) ثابت
{
    File_عقدة res;
    لكل (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][مفتاح];
        إذا (!res.empty())
            اكسر;
    }
    إرجاع res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    إرجاع buf;
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
        قيمة_EXPECTED = FileStorage::قيمة_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    ثابت حرف* _str = str.c_str();
    إذا( !fs.isOpened() || !_str )
        إرجاع fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    حرف c = *_str;

    إذا( c == '}' || c == ']' )
    {
        إذا( fs_impl->write_مكدس.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        صحيح struct_flags = fs_impl->write_مكدس.back().flags;
        حرف expected_bracket = File_عقدة::isMap(struct_flags) ? '}' : ']';
        إذا( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_مكدس.empty());
        struct_flags = fs_impl->write_مكدس.back().flags;
        fs.state = File_عقدة::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : قيمة_EXPECTED;
        fs.elname = String();
    }
    وإلا إذا( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        إذا (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = قيمة_EXPECTED + INSIDE_MAP;
    }
    وإلا إذا( (fs.state & 3) == قيمة_EXPECTED )
    {
        إذا( c == '{' || c == '[' )
        {
            صحيح struct_flags = c == '{' ? File_عقدة::MAP : File_عقدة::SEQ;
            fs.state = struct_flags == File_عقدة::MAP ? INSIDE_MAP + NAME_EXPECTED : قيمة_EXPECTED;
            _str++;
            إذا( *_str == ':' )
            {
                _str++;
                إذا( !*_str )
                    struct_flags |= File_عقدة::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        وإلا
        {
            اكتب( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            إذا( fs.state == INSIDE_MAP + قيمة_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    وإلا
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    إرجاع fs;
}


File_عقدة::File_عقدة()
    : fs(عدم)
{
    blockIdx = ofs = 0;
}

File_عقدة::File_عقدة(FileStorage::Impl* _fs, size_t _blockIdx, size_t _ofs)
    : fs(_fs)
{
    blockIdx = _blockIdx;
    ofs = _ofs;
}

File_عقدة::File_عقدة(ثابت FileStorage* _fs, size_t _blockIdx, size_t _ofs)
    : File_عقدة(_fs->p.get(), _blockIdx, _ofs)
{
    // nothing
}

File_عقدة::File_عقدة(ثابت File_عقدة& عقدة)
{
    fs = عقدة.fs;
    blockIdx = عقدة.blockIdx;
    ofs = عقدة.ofs;
}

File_عقدة& File_عقدة::operator=(ثابت File_عقدة& عقدة)
{
    fs = عقدة.fs;
    blockIdx = عقدة.blockIdx;
    ofs = عقدة.ofs;
    إرجاع *هذا;
}

File_عقدة File_عقدة::operator[](ثابت std::string& nodename) ثابت
{
    إذا(!fs)
        إرجاع File_عقدة();

    CV_Assert( isMap() );

    unsigned مفتاح = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_عقدة_Iterator it = begin();

    لكل( i = 0; i < sz; i++, ++it )
    {
        File_عقدة n = *it;
        ثابت uchar* p = n.ptr();
        unsigned key2 = (unsigned)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_data.size() );
        إذا( مفتاح == key2 )
            إرجاع n;
    }
    إرجاع File_عقدة();
}

File_عقدة File_عقدة::operator[](ثابت حرف* nodename) ثابت
{
    إرجاع هذا->operator[](std::string(nodename));
}

File_عقدة File_عقدة::operator[](صحيح i) ثابت
{
    إذا(!fs)
        إرجاع File_عقدة();

    CV_Assert( isSeq() );

    صحيح sz = (صحيح)size();
    CV_Assert( 0 <= i && i < sz );

    File_عقدة_Iterator it = begin();
    it += i;

    إرجاع *it;
}

std::vector<String> File_عقدة::مفاتيح() ثابت
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    لكل (File_عقدة_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    إرجاع res;
}

صحيح File_عقدة::type() ثابت
{
    ثابت uchar* p = ptr();
    إذا(!p)
        إرجاع NONE;
    إرجاع (*p & TYPE_MASK);
}

منطقي File_عقدة::isMap(صحيح flags) { إرجاع (flags & TYPE_MASK) == MAP; }
منطقي File_عقدة::isSeq(صحيح flags) { إرجاع (flags & TYPE_MASK) == SEQ; }
منطقي File_عقدة::isCollection(صحيح flags) { إرجاع isMap(flags) || isSeq(flags); }
منطقي File_عقدة::isFlow(صحيح flags) { إرجاع (flags & FLOW) != 0; }
منطقي File_عقدة::isEmptyCollection(صحيح flags) { إرجاع (flags & EMPTY) != 0; }

منطقي File_عقدة::empty() ثابت   { إرجاع fs == 0; }
منطقي File_عقدة::isNone() ثابت  { إرجاع type() == NONE; }
منطقي File_عقدة::isSeq() ثابت   { إرجاع type() == SEQ; }
منطقي File_عقدة::isMap() ثابت   { إرجاع type() == MAP; }
منطقي File_عقدة::isInt() ثابت   { إرجاع type() == INT;  }
منطقي File_عقدة::isReal() ثابت  { إرجاع type() == REAL; }
منطقي File_عقدة::isString() ثابت { إرجاع type() == STRING;  }
منطقي File_عقدة::isNamed() ثابت
{
    ثابت uchar* p = ptr();
    إذا(!p)
        إرجاع خطأ;
    إرجاع (*p & NAMED) != 0;
}

std::string File_عقدة::name() ثابت
{
    ثابت uchar* p = ptr();
    إذا(!p)
        إرجاع std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    إرجاع fs->getName(nameofs);
}

File_عقدة::operator صحيح() ثابت
{
    ثابت uchar* p = ptr();
    إذا(!p)
        إرجاع 0;
    صحيح tag = *p;
    صحيح type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    إذا( type == INT )
    {
        إرجاع readInt(p);
    }
    وإلا إذا( type == REAL )
    {
        إرجاع cvRound(readReal(p));
    }
    وإلا
        إرجاع 0x7fffffff;
}

File_عقدة::operator int64_t() ثابت
{
    ثابت uchar* p = ptr();
    إذا(!p)
        إرجاع 0;
    صحيح tag = *p;
    صحيح type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    إذا( type == INT )
    {
        إرجاع readLong(p);
    }
    وإلا إذا( type == REAL )
    {
        إرجاع cvRound(readReal(p));
    }
    وإلا
        إرجاع 0x7fffffff;
}

File_عقدة::operator عائم() ثابت
{
    ثابت uchar* p = ptr();
    إذا(!p)
        إرجاع 0.f;
    صحيح tag = *p;
    صحيح type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    إذا( type == INT )
    {
        إرجاع (عائم)readLong(p);
    }
    وإلا إذا( type == REAL )
    {
        إرجاع (عائم)readReal(p);
    }
    وإلا
        إرجاع FLT_MAX;
}

File_عقدة::operator مزدوج() ثابت
{
    ثابت uchar* p = ptr();
    إذا(!p)
        إرجاع 0.f;
    صحيح tag = *p;
    صحيح type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    إذا( type == INT )
    {
        إرجاع (مزدوج)readLong(p);
    }
    وإلا إذا( type == REAL )
    {
        إرجاع readReal(p);
    }
    وإلا
        إرجاع DBL_MAX;
}

مزدوج File_عقدة::real() ثابت  { إرجاع مزدوج(*هذا); }

std::string File_عقدة::string() ثابت
{
    ثابت uchar* p = ptr();
    إذا( !p || (*p & TYPE_MASK) != STRING )
        إرجاع std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(unsigned)readInt(p);
    إرجاع std::string((ثابت حرف*)(p + 4), sz - 1);
}
Mat File_عقدة::mat() ثابت { Mat value; اقرأ(*هذا, value, Mat()); إرجاع value; }

File_عقدة_Iterator File_عقدة::begin() ثابت { إرجاع File_عقدة_Iterator(*هذا, خطأ); }
File_عقدة_Iterator File_عقدة::end() ثابت   { إرجاع File_عقدة_Iterator(*هذا, true); }

فراغ File_عقدة::readRaw( ثابت std::string& fmt, فراغ* vec, size_t len ) ثابت
{
    File_عقدة_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_عقدة::size() ثابت
{
    ثابت uchar* p = ptr();
    إذا( !p )
        إرجاع 0;
    صحيح tag = *p;
    صحيح tp = tag & TYPE_MASK;
    إذا( tp == MAP || tp == SEQ )
    {
        إذا( tag & NAMED )
            p += 4;
        إرجاع (size_t)(unsigned)readInt(p + 5);
    }
    إرجاع tp != NONE;
}

size_t File_عقدة::rawSize() ثابت
{
    ثابت uchar* p0 = ptr(), *p = p0;
    إذا( !p )
        إرجاع 0;
    صحيح tag = *p++;
    صحيح tp = tag & TYPE_MASK;
    إذا( tag & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    إذا( tp == INT )
        إرجاع sz0 + 8;
    إذا( tp == REAL )
        إرجاع sz0 + 8;
    إذا( tp == NONE )
        إرجاع sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    إرجاع sz0 + 4 + readInt(p);
}

uchar* File_عقدة::ptr()
{
    إرجاع !fs ? 0 : (uchar*)fs->get_عقدة_Ptr(blockIdx, ofs);
}

ثابت uchar* File_عقدة::ptr() ثابت
{
    إرجاع !fs ? 0 : fs->get_عقدة_Ptr(blockIdx, ofs);
}

فراغ File_عقدة::set_قيمة( صحيح type, ثابت فراغ* value, صحيح len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    صحيح tag = *p;
    صحيح current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    صحيح sz = 1;

    إذا( tag & NAMED )
        sz += 4;

    إذا( type == INT )
        sz += 8;
    وإلا إذا( type == REAL )
        sz += 8;
    وإلا إذا( type == STRING )
    {
        إذا( len < 0 )
            len = (صحيح)strlen((ثابت حرف*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    وإلا
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_عقدة_Space(*هذا, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    إذا( tag & NAMED )
        p += 4;

    إذا( type == INT )
    {
        int64_t ival = *(ثابت int64_t*)value;
        writeInt(p, ival);
    }
    وإلا إذا( type == REAL )
    {
        مزدوج dbval = *(ثابت مزدوج*)value;
        writeReal(p, dbval);
    }
    وإلا إذا( type == STRING )
    {
        ثابت حرف* str = (ثابت حرف*)value;
        writeInt(p, len + 1);
        memcpy(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_عقدة_Iterator::File_عقدة_Iterator()
{
    fs = 0;
    blockIdx = 0;
    ofs = 0;
    blockSize = 0;
    عقدة_NElems = 0;
    idx = 0;
}

File_عقدة_Iterator::File_عقدة_Iterator( ثابت File_عقدة& عقدة, منطقي seekEnd )
{
    fs = عقدة.fs;
    idx = 0;
    إذا( !fs )
        blockIdx = ofs = blockSize = عقدة_NElems = 0;
    وإلا
    {
        blockIdx = عقدة.blockIdx;
        ofs = عقدة.ofs;

        منطقي collection = عقدة.isSeq() || عقدة.isMap();
        إذا( عقدة.isNone() )
        {
            عقدة_NElems = 0;
        }
        وإلا إذا( !collection )
        {
            عقدة_NElems = 1;
            إذا( seekEnd )
            {
                idx = 1;
                ofs += عقدة.rawSize();
            }
        }
        وإلا
        {
            عقدة_NElems = عقدة.size();
            ثابت uchar* p0 = عقدة.ptr(), *p = p0 + 1;
            إذا(*p0 & File_عقدة::NAMED )
                p += 4;
            إذا( !seekEnd )
                ofs += (p - p0) + 8;
            وإلا
            {
                size_t rawsz = (size_t)(unsigned)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = عقدة_NElems;
            }
        }
        fs->normalize_عقدة_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
}

File_عقدة_Iterator::File_عقدة_Iterator(ثابت File_عقدة_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    عقدة_NElems = it.عقدة_NElems;
    idx = it.idx;
}

File_عقدة_Iterator& File_عقدة_Iterator::operator=(ثابت File_عقدة_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    عقدة_NElems = it.عقدة_NElems;
    idx = it.idx;
    إرجاع *هذا;
}

File_عقدة File_عقدة_Iterator::operator *() ثابت
{
    إرجاع File_عقدة(idx < عقدة_NElems ? fs : عدم, blockIdx, ofs);
}

File_عقدة_Iterator& File_عقدة_Iterator::operator ++ ()
{
    إذا( idx == عقدة_NElems || !fs )
        إرجاع *هذا;
    idx++;
    File_عقدة n(fs, blockIdx, ofs);
    ofs += n.rawSize();
    إذا( ofs >= blockSize )
    {
        fs->normalize_عقدة_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
    إرجاع *هذا;
}

File_عقدة_Iterator File_عقدة_Iterator::operator ++ (صحيح)
{
    File_عقدة_Iterator it = *هذا;
    ++(*هذا);
    إرجاع it;
}

File_عقدة_Iterator& File_عقدة_Iterator::operator += (صحيح _ofs)
{
    CV_Assert( _ofs >= 0 );
    لكل( ; _ofs > 0; _ofs-- )
        هذا->operator ++();
    إرجاع *هذا;
}

File_عقدة_Iterator& File_عقدة_Iterator::readRaw( ثابت String& fmt, فراغ* _data0, size_t maxsz)
{
    إذا( fs && idx < عقدة_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        صحيح fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        صحيح fmt_pair_تعداد = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        size_t esz = fs::calcStructSize( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        لكل( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            لكل( صحيح k = 0; k < fmt_pair_تعداد; k++ )
            {
                صحيح elem_type = fmt_pairs[k*2+1];
                صحيح elem_size = CV_ELEM_SIZE(elem_type);

                صحيح تعداد_3 = fmt_pairs[k*2];
                offset = alignSize( offset, elem_size );
                uchar* data = data0 + offset;

                لكل( صحيح i = 0; i < تعداد_3; i++, ++(*هذا) )
                {
                    File_عقدة عقدة = *(*هذا);
                    إذا( عقدة.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_size == 8 ? (int64_t)عقدة : (صحيح)عقدة);
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
                    وإلا إذا( عقدة.isReal() )
                    {
                        مزدوج fval = (مزدوج)عقدة;

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

    إرجاع *هذا;
}

منطقي File_عقدة_Iterator::equalTo(ثابت File_عقدة_Iterator& it) ثابت
{
    إرجاع fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && عقدة_NElems == it.عقدة_NElems;
}

size_t File_عقدة_Iterator::remaining() ثابت
{
    إرجاع عقدة_NElems - idx;
}

منطقي operator == ( ثابت File_عقدة_Iterator& it1, ثابت File_عقدة_Iterator& it2 )
{
    إرجاع it1.equalTo(it2);
}

منطقي operator != ( ثابت File_عقدة_Iterator& it1, ثابت File_عقدة_Iterator& it2 )
{
    إرجاع !it1.equalTo(it2);
}

فراغ اقرأ(ثابت File_عقدة& عقدة, صحيح& val, صحيح default_val)
{
    val = default_val;
    إذا( !عقدة.empty() )
    {
        val = (صحيح)عقدة;
    }
}

فراغ اقرأ(ثابت File_عقدة& عقدة, int64_t& val, int64_t default_val)
{
    val = default_val;
    إذا( !عقدة.empty() )
    {
        val = (int64_t)عقدة;
    }
}

فراغ اقرأ(ثابت File_عقدة& عقدة, مزدوج& val, مزدوج default_val)
{
    val = default_val;
    إذا( !عقدة.empty() )
    {
        val = (مزدوج)عقدة;
    }
}

فراغ اقرأ(ثابت File_عقدة& عقدة, عائم& val, عائم default_val)
{
    val = default_val;
    إذا( !عقدة.empty() )
    {
        val = (عائم)عقدة;
    }
}

فراغ اقرأ(ثابت File_عقدة& عقدة, std::string& val, ثابت std::string& default_val)
{
    val = default_val;
    إذا( !عقدة.empty() )
    {
        val = (std::string)عقدة;
    }
}
فراغ FileStorage::Impl::اكتب(ثابت String &مفتاح, منطقي value)
{
    CV_Assert(write_mode);
    إذا (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().اكتب(مفتاح.c_str(), (صحيح)value);
    }
    وإلا
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().اكتب(مفتاح.c_str(), value ? "true" : "false", خطأ);
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
