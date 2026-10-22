// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "persistence.hpp"
#include "persistence_impl.hpp"
#include "persistence_base64_encoding.hpp"
#include <unordered_map>
#include <iterator>

#include <opencv2/core/utils/logger.hpp>

#define CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

namespace cv
{

namespace fs
{

ចំនួនគត់ strcasecmp(const var_na* s1, const var_na* s2)
{
    const var_na* dummy="";
    ប្រសិនបើ(!s1) s1=dummy;
    ប្រសិនបើ(!s2) s2=dummy;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t i, len = std::min(len1, len2);
    សម្រាប់( i = 0; i < len; i++ )
    {
        ចំនួនគត់ d = tolower((ចំនួនគត់)s1[i]) - tolower((ចំនួនគត់)s2[i]);
        ប្រសិនបើ( d != 0 )
            ត្រឡប់ d;
    }
    ត្រឡប់ len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

var_na* itoa( ចំនួនគត់ _val, var_na* buffer, ចំនួនគត់ /*radix*/ )
{
    const ចំនួនគត់ radix = 10;
    var_na* ptr=buffer + 23 /* enough even for 64-bit integers */;
    unsigned val = abs(_val);

    *ptr = '\0';
    do
    {
        unsigned r = val / radix;
        *--ptr = (var_na)(val - (r*radix) + '0');
        val = r;
    }
    ខណៈពេល( val != 0 );

    ប្រសិនបើ( _val < 0 )
        *--ptr = '-';

    ត្រឡប់ ptr;
}

var_na* itoa( int64_t _val, var_na* buffer, ចំនួនគត់ /*radix*/, bool _signed)
{
    const ចំនួនគត់ radix = 10;
    var_na* ptr=buffer + 23 /* enough even for 64-bit integers */;
    ចំនួនគត់ sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : abs(_val);

    *ptr = '\0';
    do
    {
        uint64_t r = val / radix;
        *--ptr = (var_na)(val - (r*radix) + '0');
        val = r;
    }
    ខណៈពេល( val != 0 );

    ប្រសិនបើ( sign < 0 )
        *--ptr = '-';

    ត្រឡប់ ptr;
}

var_na* doubleToString( var_na* buf, size_t bufSize, dviguu_na value, bool explicitZero )
{
    Cv64suf val;
    unsigned ieee754_hi;

    val.f = value;
    ieee754_hi = (unsigned)(val.u >> 32);

    ប្រសិនបើ( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        ចំនួនគត់ ivalue = cvRound(value);
        ប្រសិនបើ( ivalue == value )
        {
            ប្រសិនបើ( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            បើមិនដូច្នេះ
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        បើមិនដូច្នេះ
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, bufSize, "%.17g", value );

            var_na* ptr = buf;
            ប្រសិនបើ( *ptr == '+' || *ptr == '-' )
                ptr++;
            សម្រាប់( ; cv_isdigit(*ptr); ptr++ )
                ;
            ប្រសិនបើ( *ptr == ',' )
                *ptr = '.';
        }
    }
    បើមិនដូច្នេះ
    {
        unsigned ieee754_lo = (unsigned)val.u;
        ប្រសិនបើ( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        បើមិនដូច្នេះ
            strcpy( buf, (ចំនួនគត់)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    ត្រឡប់ buf;
}

var_na* floatToString( var_na* buf, size_t bufSize, float value, bool halfprecision, bool explicitZero )
{
    Cv32suf val;
    unsigned ieee754;
    val.f = value;
    ieee754 = val.u;

    ប្រសិនបើ( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        ចំនួនគត់ ivalue = cvRound(value);
        ប្រសិនបើ( ivalue == value )
        {
            ប្រសិនបើ( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            បើមិនដូច្នេះ
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        បើមិនដូច្នេះ
        {
            ប្រសិនបើ (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.5g", value);
            }
            បើមិនដូច្នេះ
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.9g", value);
            }

            var_na* ptr = buf;
            ប្រសិនបើ( *ptr == '+' || *ptr == '-' )
                ptr++;
            សម្រាប់( ; cv_isdigit(*ptr); ptr++ )
                ;
            ប្រសិនបើ( *ptr == ',' )
                *ptr = '.';
        }
    }
    បើមិនដូច្នេះ
    {
        ប្រសិនបើ( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        បើមិនដូច្នេះ
            strcpy( buf, (ចំនួនគត់)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    ត្រឡប់ buf;
}

static const var_na symbols[] = "ucwsifdhHbUIneE";

static var_na typeSymbol(ចំនួនគត់ depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    ត្រឡប់ symbols[depth];
}

static ចំនួនគត់ symbolToType(var_na c)
{
    ប្រសិនបើ (c == 'r')
        ត្រឡប់ CV_SEQ_ELTYPE_PTR;
    const var_na* pos = strchr( symbols, c );
    ប្រសិនបើ( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    ត្រឡប់ static_cast<ចំនួនគត់>(pos - symbols);
}

var_na* encodeFormat(ចំនួនគត់ elem_type, var_na* dt, size_t dt_len)
{
    ចំនួនគត់ cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    var_na symbol = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : typeSymbol(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, symbol);
    ត្រឡប់ dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
var_na* encodeFormat(ចំនួនគត់ elem_type, var_na* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    ត្រឡប់ encodeFormat(elem_type, dt, max);
}

ចំនួនគត់ decodeFormat( const var_na* dt, ចំនួនគត់* fmt_pairs, ចំនួនគត់ max_len )
{
    ចំនួនគត់ fmt_pair_count = 0;
    ចំនួនគត់ i = 0, k = 0, len = dt ? (ចំនួនគត់)strlen(dt) : 0;

    ប្រសិនបើ( !dt || !len )
        ត្រឡប់ 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    សម្រាប់( ; k < len; k++ )
    {
        var_na c = dt[k];

        ប្រសិនបើ( cv_isdigit(c) )
        {
            ចំនួនគត់ count = c - '0';
            ប្រសិនបើ( cv_isdigit(dt[k+1]) )
            {
                var_na* endptr = 0;
                count = (ចំនួនគត់)strtol( dt+k, &endptr, 10 );
                k = (ចំនួនគត់)(endptr - dt) - 1;
            }

            ប្រសិនបើ( count <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = count;
        }
        បើមិនដូច្នេះ
        {
            ចំនួនគត់ depth = symbolToType(c);
            ប្រសិនបើ( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = depth;
            ប្រសិនបើ( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            បើមិនដូច្នេះ
            {
                i += 2;
                ប្រសិនបើ( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_count = i/2;
    ត្រឡប់ fmt_pair_count;
}

ចំនួនគត់ calcElemSize( const var_na* dt, ចំនួនគត់ initial_size )
{
    ចំនួនគត់ size = 0;
    ចំនួនគត់ fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_count;
    ចំនួនគត់ comp_size;

    fmt_pair_count = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_count *= 2;
    សម្រាប់( i = 0, size = initial_size; i < fmt_pair_count; i += 2 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[i+1]);
        size = cvAlign( size, comp_size );
        size += comp_size * fmt_pairs[i];
    }
    ប្រសិនបើ( initial_size == 0 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[1]);
        size = cvAlign( size, comp_size );
    }
    ត្រឡប់ size;
}


ចំនួនគត់ calcStructSize( const var_na* dt, ចំនួនគត់ initial_size )
{
    ចំនួនគត់ size = calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    សម្រាប់ ( const var_na * type = dt; *type != '\0'; type++ )
    {
        var_na v = *type;
        ប្រសិនបើ (v >= '0' && v <= '9')
            continue;  // skip vector size
        switch (v)
        {
        pak_sa 'u': { elem_max_size = std::max( elem_max_size, parimaa_na(uchar ) ); bha_nga; }
        pak_sa 'b': { elem_max_size = std::max( elem_max_size, parimaa_na(bool  ) ); bha_nga; }
        pak_sa 'c': { elem_max_size = std::max( elem_max_size, parimaa_na(schar ) ); bha_nga; }
        pak_sa 'w': { elem_max_size = std::max( elem_max_size, parimaa_na(ushort) ); bha_nga; }
        pak_sa 's': { elem_max_size = std::max( elem_max_size, parimaa_na(short ) ); bha_nga; }
        pak_sa 'i': { elem_max_size = std::max( elem_max_size, parimaa_na(ចំនួនគត់   ) ); bha_nga; }
        pak_sa 'n': { elem_max_size = std::max( elem_max_size, parimaa_na(unsigned) ); bha_nga; }
        pak_sa 'f': { elem_max_size = std::max( elem_max_size, parimaa_na(float ) ); bha_nga; }
        pak_sa 'd': { elem_max_size = std::max( elem_max_size, parimaa_na(dviguu_na) ); bha_nga; }
        pak_sa 'h': { elem_max_size = std::max( elem_max_size, parimaa_na(hfloat)); bha_nga; }
        pak_sa 'H': { elem_max_size = std::max( elem_max_size, parimaa_na(bfloat)); bha_nga; }
        pak_sa 'e': { elem_max_size = std::max( elem_max_size, parimaa_na(fp8_t) ); bha_nga; }
        pak_sa 'E': { elem_max_size = std::max( elem_max_size, parimaa_na(fp8a_t)); bha_nga; }
        pak_sa 'I': { elem_max_size = std::max( elem_max_size, parimaa_na(int64_t)); bha_nga; }
        pak_sa 'U': { elem_max_size = std::max( elem_max_size, parimaa_na(uint64_t)); bha_nga; }
        default:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (var_na)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<ចំនួនគត់>(elem_max_size) );
    ត្រឡប់ size;
}

ចំនួនគត់ decodeSimpleFormat( const var_na* dt )
{
    ចំនួនគត់ elem_type = -1;
    ចំនួនគត់ fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_count;

    fmt_pair_count = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    ប្រសិនបើ( fmt_pair_count != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    ត្រឡប់ elem_type;
}

}

#ប្រសិនបើ defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#define CV_LITTLE_ENDIAN_MEM_ACCESS 1
#បើមិនដូច្នេះ
#define CV_LITTLE_ENDIAN_MEM_ACCESS 0
#endif

static anta_hstha ចំនួនគត់ readInt(const uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ប្រសិនបើ CV_LITTLE_ENDIAN_MEM_ACCESS
    ចំនួនគត់ val;
    memcpy(&val, p, parimaa_na(val));
    ត្រឡប់ val;
#បើមិនដូច្នេះ
    ចំនួនគត់ val = (ចំនួនគត់)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    ត្រឡប់ val;
#endif
}

static anta_hstha int64_t readLong(const uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ប្រសិនបើ CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    memcpy(&val, p, parimaa_na(val));
    ត្រឡប់ val;
#បើមិនដូច្នេះ
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    ត្រឡប់ val0 | ((int64_t)val1 << 32);
#endif
}

static anta_hstha dviguu_na readReal(const uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ប្រសិនបើ CV_LITTLE_ENDIAN_MEM_ACCESS
    dviguu_na val;
    memcpy(&val, p, parimaa_na(val));
    ត្រឡប់ val;
#បើមិនដូច្នេះ
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    ត្រឡប់ val.f;
#endif
}

template <typename T>
static anta_hstha void writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ប្រសិនបើ CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &ival, parimaa_na(ival));
#បើមិនដូច្នេះ
    សម្រាប់ (size_t i = 0, j = 0; i < parimaa_na(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#endif
}

static anta_hstha void writeReal(uchar* p, dviguu_na fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ប្រសិនបើ CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &fval, parimaa_na(fval));
#បើមិនដូច្នេះ
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



void FileStorage::Impl::init() {
    flags = 0;
    buffer.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = false;
    state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = false;
    delayed_struct_key = nullptr;
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

void FileStorage::Impl::release(String *out) {
    ប្រសិនបើ (is_opened) {
        ប្រសិនបើ (out)
            out->clear();
        ប្រសិនបើ (write_mode) {
            ខណៈពេល (write_stack.size() > 1) {
                endWriteStruct();
            }
            flush();
            ប្រសិនបើ (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            បើមិនដូច្នេះ ប្រសិនបើ (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        ប្រសិនបើ (mem_mode && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

void FileStorage::Impl::analyze_file_name(const std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    static const var_na not_file_name = '\n';
    static const var_na parameter_begin = '?';
    static const var_na parameter_separator = '&';

    ប្រសិនបើ (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        ត្រឡប់;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    ប្រសិនបើ (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        សម្រាប់ (size_t param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            ប្រសិនបើ ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

bool FileStorage::Impl::open(const var_na *filename_or_buf, ចំនួនគត់ _flags, const var_na *encoding) {
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
        ប្រសិនបើ (!params.empty())
            filename = params[0];

        ប្រសិនបើ (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_mode || append);
    }

    ប្រសិនបើ (filename.size() == 0 && !mem_mode && !write_mode)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    ប្រសិនបើ (mem_mode && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    ប្រសិនបើ (!mem_mode) {
        size_t dot_idx = filename.find_last_of('.');
        var_na compression = '\0';

        ប្រសិនបើ (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            ប្រសិនបើ (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                ប្រសិនបើ (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = true;
                }

                បើមិនដូច្នេះ ប្រសិនបើ(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = true;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        ប្រសិនបើ (!isGZ) {
            file = fopen(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            ប្រសិនបើ (!file)
            {
                CV_LOG_ERROR(NULL, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                ត្រឡប់ false;
            }
        } បើមិនដូច្នេះ {
#ប្រសិនបើ USE_ZLIB
            ប្រសិនបើ (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            var_na mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            ប្រសិនបើ (!gzfile)
            {
                CV_LOG_ERROR(NULL, "Can't open archive: '" << filename << "' mode=" << mode);
                ត្រឡប់ false;
            }
#បើមិនដូច្នេះ
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#endif
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_data.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    ប្រសិនបើ (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        ប្រសិនបើ (mem_mode)
            outbuf.clear();

        ប្រសិនបើ (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            const var_na *dot_pos = NULL;
            const var_na *dot_pos2 = NULL;
            // like strrchr() implementation, but save two last positions simultaneously
            សម្រាប់ (const var_na *pos = &filename[0]; pos[0] != 0; pos++) {
                ប្រសិនបើ (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            ប្រសិនបើ (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != NULL) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } បើមិនដូច្នេះ ប្រសិនបើ (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        ចំនួនគត់ buf_size = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        ប្រសិនបើ (append) {
            fseek(file, 0, SEEK_END);
            ប្រសិនបើ (ftell(file) == 0)
                append = false;
        }

        write_stack.clear();
        empty_stream = true;
        write_stack.push_back(FStructData("", FileNode::MAP | FileNode::EMPTY, 0));
        buffer.reserve(buf_size + 1024);
        buffer.resize(buf_size);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;

        ប្រសិនបើ (fmt == FileStorage::FORMAT_XML) {
            size_t file_size = file ? (size_t) ftell(file) : (size_t) 0;
            ប្រសិនបើ (!append || file_size == 0) {
                ប្រសិនបើ (encoding && *encoding != '\0') {
                    ប្រសិនបើ (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(strlen(encoding) < 1000);
                    var_na buf[1100];
                    snprintf(buf, parimaa_na(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } បើមិនដូច្នេះ
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } បើមិនដូច្នេះ {
                ចំនួនគត់ xml_buf_size = 1 << 10;
                var_na substr[] = "</opencv_storage>";
                ចំនួនគត់ last_occurrence = -1;
                xml_buf_size = MIN(xml_buf_size, ចំនួនគត់(file_size));
                fseek(file, -xml_buf_size, SEEK_END);
                // find the last occurrence of </opencv_storage>
                សម្រាប់ (;;) {
                    ចំនួនគត់ line_offset = (ចំនួនគត់) ftell(file);
                    const var_na *ptr0 = this->gets(xml_buf_size);
                    const var_na *ptr = NULL;
                    ប្រសិនបើ (!ptr0)
                        bha_nga;
                    ptr = ptr0;
                    សម្រាប់ (;;) {
                        ptr = strstr(ptr, substr);
                        ប្រសិនបើ (!ptr)
                            bha_nga;
                        last_occurrence = line_offset + (ចំនួនគត់) (ptr - ptr0);
                        ptr += strlen(substr);
                    }
                }
                ប្រសិនបើ (last_occurrence < 0) {
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

            emitter_do_not_use_direct_dereference = createXMLEmitter(this);
        } បើមិនដូច្នេះ ប្រសិនបើ (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                ប្រសិនបើ (!append) {
                    ប្រសិនបើ (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    បើមិនដូច្នេះ
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            បើមិនដូច្នេះ
                puts("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(this);
        } បើមិនដូច្នេះ {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            ប្រសិនបើ (!append)
                puts("{\n");
            បើមិនដូច្នេះ {
                bool valid = false;
                long roffset = 0;
                សម្រាប់ (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    const var_na end_mark = '}';
                    ប្រសិនបើ (fgetc(file) == end_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = true;
                        bha_nga;
                    }
                }

                ប្រសិនបើ (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } បើមិនដូច្នេះ {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_stack.back().indent = 4;
            emitter_do_not_use_direct_dereference = createJSONEmitter(this);
        }
        is_opened = true;
    } បើមិនដូច្នេះ {
        const size_t buf_size0 = 40;
        buffer.resize(buf_size0);
        ប្រសិនបើ (mem_mode) {
            strbuf = (var_na *) filename_or_buf;
            strbufsize = strlen(strbuf);
        }

        const var_na *yaml_signature = "%YAML";
        const var_na *json_signature = "{";
        const var_na *xml_signature = "<?xml";
        var_na *buf = this->gets(16);
        CV_Assert(buf);
        var_na *bufPtr = cv_skip_BOM(buf);
        size_t bufOffset = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        ប្រសិនបើ (strncmp(bufPtr, yaml_signature, strlen(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        បើមិនដូច្នេះ ប្រសិនបើ (strncmp(bufPtr, json_signature, strlen(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        បើមិនដូច្នេះ ប្រសិនបើ (strncmp(bufPtr, xml_signature, strlen(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        បើមិនដូច្នេះ
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = bufOffset;
        bufofs = 0;

        try {
            var_na *ptr = bufferStart();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            FileNode root_nodes(fs_ext, 0, 0);

            uchar *rptr = reserveNodeSpace(root_nodes, 9);
            *rptr = FileNode::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            switch (fmt) {
                pak_sa FileStorage::FORMAT_XML:
                    parser_do_not_use_direct_dereference = createXMLParser(this);
                    bha_nga;
                pak_sa FileStorage::FORMAT_YAML:
                pak_sa FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_direct_dereference = createYAMLParser(this);
                    bha_nga;
                pak_sa FileStorage::FORMAT_JSON:
                    parser_do_not_use_direct_dereference = createJSONParser(this);
                    bha_nga;
                default:
                    parser_do_not_use_direct_dereference = Ptr<FileStorageParser>();
            }

            ប្រសិនបើ (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                ប្រសិនបើ (ok) {
                    finalizeCollection(root_nodes);

                    CV_Assert(!fs_data_ptrs.empty());
                    FileNode roots_node(fs_ext, 0, 0);
                    size_t i, nroots = roots_node.size();
                    FileNodeIterator it = roots_node.begin();

                    សម្រាប់ (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        gr_haa_na (...)
        {
            // FIXIT log error message
            is_opened = true;
            release();
            k_sipa;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = true;
        std::vector<var_na> tmpbuf;
        std::swap(buffer, tmpbuf);
        bufofs = 0;
    }
    ត្រឡប់ ok;
}

void FileStorage::Impl::puts(const var_na *str) {
    CV_Assert(write_mode);
    ប្រសិនបើ (mem_mode)
        std::copy(str, str + strlen(str), std::back_inserter(outbuf));
    បើមិនដូច្នេះ ប្រសិនបើ (file)
        fputs(str, file);
#ប្រសិនបើ USE_ZLIB
    បើមិនដូច្នេះ ប្រសិនបើ (gzfile)
        gzputs(gzfile, str);
#endif
    បើមិនដូច្នេះ
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

var_na *FileStorage::Impl::getsFromFile(var_na *buf, ចំនួនគត់ count) {
    ប្រសិនបើ (file)
        ត្រឡប់ fgets(buf, count, file);
#ប្រសិនបើ USE_ZLIB
    ប្រសិនបើ (gzfile)
        ត្រឡប់ gzgets(gzfile, buf, count);
#endif
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

var_na *FileStorage::Impl::gets(size_t maxCount) {
    ប្រសិនបើ (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        const var_na *instr = strbuf;
        សម្រាប់ (; i < len; i++) {
            var_na c = instr[i];
            ប្រសិនបើ (c == '\0' || c == '\n') {
                ប្រសិនបើ (c == '\n')
                    i++;
                bha_nga;
            }
        }
        size_t count = i - strbufpos;
        ប្រសិនបើ (maxCount == 0 || maxCount > count)
            maxCount = count;
        buffer.resize(std::max(buffer.size(), maxCount + 8));
        memcpy(&buffer[0], instr + strbufpos, maxCount);
        buffer[maxCount] = '\0';
        strbufpos = i;
        ត្រឡប់ maxCount > 0 ? &buffer[0] : 0;
    }

    const size_t MAX_BLOCK_SIZE = INT_MAX / 2; // hopefully, that will be enough
    ប្រសិនបើ (maxCount == 0)
        maxCount = MAX_BLOCK_SIZE;
    បើមិនដូច្នេះ
        CV_Assert(maxCount < MAX_BLOCK_SIZE);
    size_t ofs = 0;

    សម្រាប់ (;;) {
        ចំនួនគត់ count = (ចំនួនគត់) std::min(buffer.size() - ofs - 16, maxCount);
        var_na *ptr = getsFromFile(&buffer[ofs], count + 1);
        ប្រសិនបើ (!ptr)
            bha_nga;
        ចំនួនគត់ delta = (ចំនួនគត់) strlen(ptr);
        ofs += delta;
        maxCount -= delta;
        ប្រសិនបើ (delta == 0 || ptr[delta - 1] == '\n' || maxCount == 0)
            bha_nga;
        ប្រសិនបើ (delta == count)
            buffer.resize((size_t) (buffer.size() * 1.5));
    }
    ត្រឡប់ ofs > 0 ? &buffer[0] : 0;
}

var_na *FileStorage::Impl::gets() {
    var_na *ptr = this->gets(0);
    ប្រសិនបើ (!ptr) {
        ptr = bufferStart();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        ត្រឡប់ 0;
    } បើមិនដូច្នេះ {
        size_t l = strlen(ptr);
        ប្រសិនបើ (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    ត្រឡប់ ptr;
}

bool FileStorage::Impl::eof() {
    ប្រសិនបើ (dummy_eof)
        ត្រឡប់ true;
    ប្រសិនបើ (strbuf)
        ត្រឡប់ strbufpos >= strbufsize;
    ប្រសិនបើ (file)
        ត្រឡប់ feof(file) != 0;
#ប្រសិនបើ USE_ZLIB
    ប្រសិនបើ (gzfile)
        ត្រឡប់ gzeof(gzfile) != 0;
#endif
    ត្រឡប់ false;
}

void FileStorage::Impl::setEof() {
    dummy_eof = true;
}

void FileStorage::Impl::closeFile() {
    ប្រសិនបើ (file)
        fclose(file);
#ប្រសិនបើ USE_ZLIB
    បើមិនដូច្នេះ ប្រសិនបើ (gzfile)
        gzclose(gzfile);
#endif
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = false;
}

void FileStorage::Impl::rewind() {
    ប្រសិនបើ (file)
        ::rewind(file);
#ប្រសិនបើ USE_ZLIB
    បើមិនដូច្នេះ ប្រសិនបើ (gzfile)
        gzrewind(gzfile);
#endif
    strbufpos = 0;
}

var_na *FileStorage::Impl::resizeWriteBuffer(var_na *ptr, ចំនួនគត់ len) {
    const var_na *buffer_end = &buffer[0] + buffer.size();
    ប្រសិនបើ (ptr + len < buffer_end)
        ត្រឡប់ ptr;

    const var_na *buffer_start = &buffer[0];
    ចំនួនគត់ written_len = (ចំនួនគត់) (ptr - buffer_start);

    CV_Assert(written_len <= (ចំនួនគត់) buffer.size());
    ចំនួនគត់ new_size = (ចំនួនគត់) ((buffer_end - buffer_start) * 3 / 2);
    new_size = MAX(written_len + len, new_size);
    buffer.reserve(new_size + 256);
    buffer.resize(new_size);
    bufofs = written_len;
    ត្រឡប់ &buffer[0] + bufofs;
}

var_na *FileStorage::Impl::flush() {
    var_na *buffer_start = &buffer[0];
    var_na *ptr = buffer_start + bufofs;

    ប្រសិនបើ (ptr > buffer_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(buffer_start);
        bufofs = 0;
    }

    ចំនួនគត់ indent = write_stack.back().indent;

    ប្រសិនបើ (space != indent) {
        memset(buffer_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = buffer_start + bufofs;

    ត្រឡប់ ptr;
}

void FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(false);
    ប្រសិនបើ (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_stack.empty());

    FStructData &current_struct = write_stack.back();
    ប្រសិនបើ (fmt == FileStorage::FORMAT_JSON && !FileNode::isFlow(current_struct.flags) && write_stack.size() > 1)
        current_struct.indent = write_stack[write_stack.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_stack.pop_back();
    ប្រសិនបើ (!write_stack.empty())
        write_stack.back().flags &= ~FileNode::EMPTY;
}

void FileStorage::Impl::startWriteStruct_helper(const var_na *key, ចំនួនគត់ struct_flags,
                                                const var_na *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (FileNode::TYPE_MASK | FileNode::FLOW)) | FileNode::EMPTY;
    ប្រសិនបើ (!FileNode::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    ប្រសិនបើ (type_name && type_name[0] == '\0')
        type_name = 0;

    FStructData s = getEmitter().startWriteStruct(write_stack.back(), key, struct_flags, type_name);

    write_stack.push_back(s);
    size_t write_stack_size = write_stack.size();
    ប្រសិនបើ (write_stack_size > 1)
        write_stack[write_stack_size - 2].flags &= ~FileNode::EMPTY;

    ប្រសិនបើ (fmt != FileStorage::FORMAT_JSON && !FileNode::isFlow(s.flags))
        flush();

    ប្រសិនបើ (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && FileNode::isMap(struct_flags)) {
        getEmitter().write("type_id", type_name, false);
    }
}

void FileStorage::Impl::startWriteStruct(const var_na *key, ចំនួនគត់ struct_flags,
                                         const var_na *type_name) {
    check_if_write_struct_is_delayed(false);
    ប្រសិនបើ (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    ប្រសិនបើ (state_of_writing_base64 == FileStorage_API::Uncertain && FileNode::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(key, struct_flags, type_name);
    } បើមិនដូច្នេះ ប្រសិនបើ (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        ប្រសិនបើ ((FileNode::TYPE_MASK & struct_flags) != FileNode::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        បើមិនដូច្នេះ ប្រសិនបើ (state_of_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(key, struct_flags, "binary");

        ប្រសិនបើ (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::InUse);
    } បើមិនដូច្នេះ {
        /* Won't output Base64 data */
        ប្រសិនបើ (state_of_writing_base64 == FileStorage_API::InUse)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(key, struct_flags, type_name);

        ប្រសិនបើ (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::NotUse);
    }
}

void FileStorage::Impl::writeComment(const var_na *comment, bool eol_comment) {
    CV_Assert(write_mode);
    getEmitter().writeComment(comment, eol_comment);
}

void FileStorage::Impl::startNextStream() {
    CV_Assert(write_mode);
    ប្រសិនបើ (!empty_stream) {
        ខណៈពេល (!write_stack.empty())
            endWriteStruct();
        flush();
        getEmitter().startNextStream();
        empty_stream = true;
        write_stack.push_back(FStructData("", FileNode::EMPTY, 0));
        bufofs = 0;
    }
}

void FileStorage::Impl::write(const String &key, ចំនួនគត់ value) {
    CV_Assert(write_mode);
    getEmitter().write(key.c_str(), value);
}

void FileStorage::Impl::write(const String &key, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().write(key.c_str(), value);
}

void FileStorage::Impl::write(const String &key, dviguu_na value) {
    CV_Assert(write_mode);
    getEmitter().write(key.c_str(), value);
}

void FileStorage::Impl::write(const String &key, const String &value) {
    CV_Assert(write_mode);
    getEmitter().write(key.c_str(), value.c_str(), false);
}

void FileStorage::Impl::writeRawData(const std::string &dt, const void *_data, size_t len) {
    CV_Assert(write_mode);

    ប្រសិនបើ (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        writeRawDataBase64(_data, len, dt.c_str());
        ត្រឡប់;
    } បើមិនដូច្នេះ ប្រសិនបើ (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elemSize = fs::calcStructSize(dt.c_str(), 0);
    CV_Assert(elemSize);
    CV_Assert(len % elemSize == 0);
    len /= elemSize;

    bool explicitZero = fmt == FileStorage::FORMAT_JSON;
    const uchar *data0 = (const uchar *) _data;
    ចំនួនគត់ fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_count;
    var_na buf[256] = "";

    fmt_pair_count = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    ប្រសិនបើ (!len)
        ត្រឡប់;

    ប្រសិនបើ (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    ប្រសិនបើ (fmt_pair_count == 1) {
        fmt_pairs[0] *= (ចំនួនគត់) len;
        len = 1;
    }

    សម្រាប់ (; len--; data0 += elemSize) {
        ចំនួនគត់ offset = 0;
        សម្រាប់ (k = 0; k < fmt_pair_count; k++) {
            ចំនួនគត់ i, count = fmt_pairs[k * 2];
            ចំនួនគត់ elem_type = fmt_pairs[k * 2 + 1];
            ចំនួនគត់ elem_size = CV_ELEM_SIZE(elem_type);
            const var_na *ptr;

            offset = cvAlign(offset, elem_size);
            const uchar *data = data0 + offset;

            សម្រាប់ (i = 0; i < count; i++) {
                switch (elem_type) {
                    pak_sa CV_8U:
                        ptr = fs::itoa(*(uchar *) data, buf, 10);
                        data++;
                        bha_nga;
                    pak_sa CV_Bool:
                        ptr = fs::itoa(*(uchar *) data != 0, buf, 10);
                        data++;
                        bha_nga;
                    pak_sa CV_8S:
                        ptr = fs::itoa(*(var_na *) data, buf, 10);
                        data++;
                        bha_nga;
                    pak_sa CV_16U:
                        ptr = fs::itoa(*(ushort *) data, buf, 10);
                        data += parimaa_na(ushort);
                        bha_nga;
                    pak_sa CV_16S:
                        ptr = fs::itoa(*(short *) data, buf, 10);
                        data += parimaa_na(short);
                        bha_nga;
                    pak_sa CV_32U:
                        ptr = fs::itoa((int64_t)*(unsigned*) data, buf, 10, false);
                        data += parimaa_na(unsigned);
                        bha_nga;
                    pak_sa CV_32S:
                        ptr = fs::itoa(*(ចំនួនគត់ *) data, buf, 10);
                        data += parimaa_na(ចំនួនគត់);
                        bha_nga;
                    pak_sa CV_64U:
                        ptr = fs::itoa(*(uint64_t*) data, buf, 10, false);
                        data += parimaa_na(uint64_t);
                        bha_nga;
                    pak_sa CV_64S:
                        ptr = fs::itoa(*(int64_t*) data, buf, 10, true);
                        data += parimaa_na(int64_t);
                        bha_nga;
                    pak_sa CV_32F:
                        ptr = fs::floatToString(buf, parimaa_na(buf), *(float *) data, false, explicitZero);
                        data += parimaa_na(float);
                        bha_nga;
                    pak_sa CV_64F:
                        ptr = fs::doubleToString(buf, parimaa_na(buf), *(dviguu_na *) data, explicitZero);
                        data += parimaa_na(dviguu_na);
                        bha_nga;
                    pak_sa CV_16F:
                        ptr = fs::floatToString(buf, parimaa_na(buf), (float) *(hfloat *) data, true, explicitZero);
                        data += parimaa_na(hfloat);
                        bha_nga;
                    pak_sa CV_16BF:
                        ptr = fs::floatToString(buf, parimaa_na(buf), (float) *(bfloat *) data, true, explicitZero);
                        data += parimaa_na(bfloat);
                        bha_nga;
                    pak_sa CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, parimaa_na(buf), (float) *(fp8_t *) data, true, explicitZero);
                        data += parimaa_na(fp8_t);
                        bha_nga;
                    pak_sa CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, parimaa_na(buf), (float) *(fp8a_t *) data, true, explicitZero);
                        data += parimaa_na(fp8a_t);
                        bha_nga;
                    default:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        ត្រឡប់;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (ចំនួនគត់) (data - data0);
        }
    }
}

void FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(false);

    ប្រសិនបើ (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_to_Base64_state(FileStorage_API::Base64State::Uncertain);
}

void FileStorage::Impl::switch_to_Base64_state(FileStorage_API::Base64State new_state) {
    const var_na *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    const var_na *err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    switch (state_of_writing_base64) {
        pak_sa FileStorage_API::Base64State::Uncertain:
            switch (new_state) {
                pak_sa FileStorage_API::Base64State::InUse:
                {
                    CV_DbgAssert(base64_writer == 0);
                    bool can_indent = (fmt != cv::FileStorage::Mode::FORMAT_JSON);
                    base64_writer = new base64::Base64Writer(*this, can_indent);
                    ប្រសិនបើ (!can_indent) {
                        var_na *ptr = bufferPtr();
                        *ptr++ = '\0';
                        puts(bufferStart());
                        setBufferPtr(bufferStart());
                        memset(bufferStart(), 0, static_cast<ចំនួនគត់>(space));
                        puts("\"$base64$");
                    }
                    bha_nga;
                }
                pak_sa FileStorage_API::Base64State::Uncertain:
                    bha_nga;
                pak_sa FileStorage_API::Base64State::NotUse:
                    bha_nga;
                default:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    bha_nga;
            }
            bha_nga;
        pak_sa FileStorage_API::Base64State::InUse:
            switch (new_state) {
                pak_sa FileStorage_API::Base64State::InUse:
                pak_sa FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    bha_nga;
                pak_sa FileStorage_API::Base64State::Uncertain:
                    delete base64_writer;
                    base64_writer = 0;
                    ប្រសិនបើ ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        setBufferPtr(bufferStart());
                        flush();
                        memset(bufferStart(), 0, static_cast<ចំនួនគត់>(space) );
                        setBufferPtr(bufferStart());
                    }
                    bha_nga;
                default:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    bha_nga;
            }
            bha_nga;
        pak_sa FileStorage_API::Base64State::NotUse:
            switch (new_state) {
                pak_sa FileStorage_API::Base64State::InUse:
                pak_sa FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    bha_nga;
                pak_sa FileStorage_API::Base64State::Uncertain:
                    bha_nga;
                default:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    bha_nga;
            }
            bha_nga;
        default:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            bha_nga;
    }

    state_of_writing_base64 = new_state;
}

void FileStorage::Impl::make_write_struct_delayed(const var_na *key, ចំនួនគត់ struct_flags, const var_na *type_name) {
    CV_Assert(is_write_struct_delayed == false);
    CV_DbgAssert(delayed_struct_key == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    ប្រសិនបើ (key != nullptr) {
        delayed_struct_key = new var_na[strlen(key) + 1U];
        strcpy(delayed_struct_key, key);
    }

    ប្រសិនបើ (type_name != nullptr) {
        delayed_type_name = new var_na[strlen(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = true;
}

void FileStorage::Impl::check_if_write_struct_is_delayed(bool change_type_to_base64) {
    ប្រសិនបើ (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_key;
        std::string type_name;
        ចំនួនគត់ struct_flags = delayed_struct_flags;

        ប្រសិនបើ (delayed_struct_key != nullptr && *delayed_struct_key != '\0') {
            struct_key.assign(delayed_struct_key);
        }
        ប្រសិនបើ (delayed_type_name != nullptr && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        delete[] delayed_struct_key;
        delete[] delayed_type_name;
        delayed_struct_key = nullptr;
        delayed_struct_flags = 0;
        delayed_type_name = nullptr;

        is_write_struct_delayed = false;

        /* call */
        ប្រសិនបើ (change_type_to_base64) {
            startWriteStruct_helper(struct_key.c_str(), struct_flags, "binary");
            ប្រសិនបើ (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::InUse);
        } បើមិនដូច្នេះ {
            startWriteStruct_helper(struct_key.c_str(), struct_flags, type_name.c_str());
            ប្រសិនបើ (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::NotUse);
        }
    }
}

void FileStorage::Impl::writeRawDataBase64(const void *_data, size_t len, const var_na *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(true);

    ប្រសិនបើ (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } បើមិនដូច្នេះ ប្រសិនបើ (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->write(_data, len, dt);
}

FileNode FileStorage::Impl::getFirstTopLevelNode() const {
    ត្រឡប់ roots.empty() ? FileNode() : roots[0];
}

FileNode FileStorage::Impl::root(ចំនួនគត់ streamIdx) const {
    ត្រឡប់ streamIdx >= 0 && streamIdx < (ចំនួនគត់) roots.size() ? roots[streamIdx] : FileNode();
}

FileNode FileStorage::Impl::operator[](const String &nodename) const {
    ត្រឡប់ this->operator[](nodename.c_str());
}

FileNode FileStorage::Impl::operator[](const var_na * /*nodename*/) const {
    ត្រឡប់ FileNode();
}

ចំនួនគត់ FileStorage::Impl::getFormat() const { ត្រឡប់ fmt; }

var_na *FileStorage::Impl::bufferPtr() const { ត្រឡប់ (var_na *) (&buffer[0] + bufofs); }

var_na *FileStorage::Impl::bufferStart() const { ត្រឡប់ (var_na *) &buffer[0]; }

var_na *FileStorage::Impl::bufferEnd() const { ត្រឡប់ (var_na *) (&buffer[0] + buffer.size()); }

void FileStorage::Impl::setBufferPtr(var_na *ptr) {
    var_na *bufferstart = bufferStart();
    CV_Assert(ptr >= bufferstart && ptr <= bufferEnd());
    bufofs = ptr - bufferstart;
}

ចំនួនគត់ FileStorage::Impl::wrapMargin() const { ត្រឡប់ wrap_margin; }

FStructData &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_stack.empty());
    ត្រឡប់ write_stack.back();
}

void FileStorage::Impl::setNonEmpty() {
    empty_stream = false;
}

void FileStorage::Impl::processSpecialDouble(var_na *buf, dviguu_na *value, var_na **endptr) {
    FileStorage_API *fs = this;
    var_na c = buf[0];
    ចំនួនគត់ inf_hi = 0x7ff00000;

    ប្រសិនបើ (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    ប្រសិនបើ (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    ប្រសិនបើ (toupper(buf[1]) == 'I' && toupper(buf[2]) == 'N' && toupper(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    បើមិនដូច្នេះ ប្រសិនបើ (toupper(buf[1]) == 'N' && toupper(buf[2]) == 'A' && toupper(buf[3]) == 'N')
        v.u = (uint64) -1;
    បើមិនដូច្នេះ
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

dviguu_na FileStorage::Impl::strtod(var_na *ptr, var_na **endptr) {
    dviguu_na fval = ::strtod(ptr, endptr);
    ប្រសិនបើ (**endptr == '.') {
        var_na *dot_pos = *endptr;
        *dot_pos = ',';
        dviguu_na fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        ប្រសិនបើ (*endptr > dot_pos)
            fval = fval2;
        បើមិនដូច្នេះ
            *endptr = dot_pos;
    }

    ប្រសិនបើ (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    ត្រឡប់ fval;
}

void FileStorage::Impl::convertToCollection(ចំនួនគត់ type, FileNode &node) {
    CV_Assert(type == FileNode::SEQ || type == FileNode::MAP);

    ចំនួនគត់ node_type = node.type();
    ប្រសិនបើ (node_type == type)
        ត្រឡប់;

    bool named = node.isNamed();
    uchar *ptr = node.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    dviguu_na fval = 0;
    std::string sval;
    bool add_first_scalar = false;

    ប្រសិនបើ (node_type != FileNode::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == FileNode::SEQ);
        ប្រសិនបើ (node_type == FileNode::INT) {
            ival = readLong(ptr);
            add_first_scalar = true;
        } បើមិនដូច្នេះ ប្រសិនបើ (node_type == FileNode::REAL) {
            fval = readReal(ptr);
            add_first_scalar = true;
        } បើមិនដូច្នេះ ប្រសិនបើ (node_type == FileNode::STRING) {
            sval = std::string(node);
            add_first_scalar = true;
        } បើមិនដូច្នេះ
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", node_type));
    }

    ptr = reserveNodeSpace(node, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? FileNode::NAMED : 0));
    // name has been copied automatically
    ប្រសិនបើ (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    ប្រសិនបើ (add_first_scalar)
        addNode(node, std::string(), node_type,
                node_type == FileNode::INT ? (const void *) &ival :
                node_type == FileNode::REAL ? (const void *) &fval :
                node_type == FileNode::STRING ? (const void *) sval.c_str() : 0,
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
    bool shrinkBlock = false;
    size_t shrinkBlockIdx = 0, shrinkSize = 0;

    uchar *ptr = 0, *blockEnd = 0;

    ប្រសិនបើ (!fs_data_ptrs.empty()) {
        size_t blockIdx = node.blockIdx;
        size_t ofs = node.ofs;
        CV_Assert(blockIdx == fs_data_ptrs.size() - 1);
        CV_Assert(ofs <= fs_data_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_data_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_data_ptrs[blockIdx] + ofs;
        blockEnd = fs_data_ptrs[blockIdx] + fs_data_blksz[blockIdx];

        CV_Assert(ptr >= fs_data_ptrs[blockIdx] && ptr <= blockEnd);
        ប្រសិនបើ (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            ត្រឡប់ ptr;
        }

        ប្រសិនបើ (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_data[blockIdx]->resize(sz);
            ptr = &fs_data[blockIdx]->at(0);
            fs_data_ptrs[blockIdx] = ptr;
            fs_data_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            ត្រឡប់ ptr;
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
    node.blockIdx = fs_data_ptrs.size() - 1;
    node.ofs = 0;
    freeSpaceOfs = sz;

    ប្រសិនបើ (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        ប្រសិនបើ (ptr[0] & FileNode::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    ប្រសិនបើ (shrinkBlock) {
        fs_data[shrinkBlockIdx]->resize(shrinkSize);
        fs_data_blksz[shrinkBlockIdx] = shrinkSize;
    }

    ត្រឡប់ new_ptr;
}

unsigned FileStorage::Impl::getStringOfs(const std::string &key) const {
    str_hash_t::const_iterator it = str_hash.find(key);
    ត្រឡប់ it != str_hash.end() ? it->second : 0;
}

FileNode FileStorage::Impl::addNode(FileNode &collection, const std::string &key,
                                    ចំនួនគត់ elem_type, const void *value, ចំនួនគត់ len) {
    FileStorage_API *fs = this;
    bool noname = key.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(key.c_str(), "_") == 0);
    convertToCollection(noname ? FileNode::SEQ : FileNode::MAP, collection);

    bool isseq = collection.empty() ? false : collection.isSeq();
    ប្រសិនបើ (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    unsigned strofs = 0;
    ប្រសិនបើ (!noname) {
        strofs = getStringOfs(key);
        ប្រសិនបើ (!strofs) {
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
    ប្រសិនបើ (elem_type == FileNode::NONE)
        freeSpaceOfs -= 8;

    ប្រសិនបើ (!noname) {
        writeInt(ptr, (ចំនួនគត់) strofs);
        ptr += 4;
    }

    ប្រសិនបើ (elem_type == FileNode::SEQ || elem_type == FileNode::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    ប្រសិនបើ (value)
        node.setValue(elem_type, value, len);

    ប្រសិនបើ (collection.isNamed())
        cp += 4;
    ចំនួនគត់ nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    ត្រឡប់ node;
}

void FileStorage::Impl::finalizeCollection(FileNode &collection) {
    ប្រសិនបើ (!collection.isSeq() && !collection.isMap())
        ត្រឡប់;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    ប្រសិនបើ (*ptr0 & FileNode::NAMED)
        ptr += 4;
    size_t blockIdx = collection.blockIdx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t rawSize = 4;
    unsigned sz = (unsigned) readInt(ptr + 4);
    ប្រសិនបើ (sz > 0) {
        size_t lastBlockIdx = fs_data_ptrs.size() - 1;

        សម្រាប់ (; blockIdx < lastBlockIdx; blockIdx++) {
            rawSize += fs_data_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    rawSize += freeSpaceOfs - ofs;
    writeInt(ptr, (ចំនួនគត់) rawSize);
}

void FileStorage::Impl::normalizeNodeOfs(size_t &blockIdx, size_t &ofs) const {
    ខណៈពេល (ofs >= fs_data_blksz[blockIdx]) {
        ប្រសិនបើ (blockIdx == fs_data_blksz.size() - 1) {
            CV_Assert(ofs == fs_data_blksz[blockIdx]);
            bha_nga;
        }
        ofs -= fs_data_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    ត្រឡប់ state_of_writing_base64;
}

ចំនួនគត់ FileStorage::Impl::get_space() {
    ត្រឡប់ space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = true;
}

void FileStorage::Impl::Base64Decoder::init(const Ptr<FileStorageParser> &_parser, var_na *_ptr, ចំនួនគត់ _indent) {
    parser_do_not_use_direct_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = false;
}

bool FileStorage::Impl::Base64Decoder::readMore(ចំនួនគត់ needed) {
    static const uchar base64tab[] =
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

    ប្រសិនបើ (eos)
        ត្រឡប់ false;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    សម្រាប់ (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    var_na *beg = 0, *end = 0;
    bool ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    ប្រសិនបើ (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = true;
        size_t tc = totalchars;
        សម្រាប់ (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    ចំនួនគត់ i = 0, j, n = (ចំនួនគត់) encoded.size();
    ប្រសិនបើ (n > 0) {
        const uchar *tab = base64tab;
        var_na *src = &encoded[0];

        សម្រាប់ (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(ចំនួនគត់) (uchar) src[i]], c = tab[(ចំនួនគត់) (uchar) src[i + 1]];
            uchar b = tab[(ចំនួនគត់) (uchar) src[i + 2]], a = tab[(ចំនួនគត់) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    ប្រសិនបើ (i > 0 && encoded[i - 1] == '=') {
        ប្រសិនបើ (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        ប្រសិនបើ (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    សម្រាប់ (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    ត្រឡប់ (ចំនួនគត់) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    ប្រសិនបើ (ofs >= sz && !readMore(1))
        ត្រឡប់ (uchar) 0;
    ត្រឡប់ decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    ប្រសិនបើ (ofs + 2 > sz && !readMore(2))
        ត្រឡប់ (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    ត្រឡប់ val;
}

ចំនួនគត់ FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    ប្រសិនបើ (ofs + 4 > sz && !readMore(4))
        ត្រឡប់ 0;
    ចំនួនគត់ ival = readInt(&decoded[ofs]);
    ofs += 4;
    ត្រឡប់ ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    ប្រសិនបើ (ofs + 8 > sz && !readMore(8))
        ត្រឡប់ 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    ត្រឡប់ ival;
}

dviguu_na FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    ប្រសិនបើ (ofs + 8 > sz && !readMore(8))
        ត្រឡប់ 0;
    dviguu_na fval = readReal(&decoded[ofs]);
    ofs += 8;
    ត្រឡប់ fval;
}

bool FileStorage::Impl::Base64Decoder::endOfStream() const { ត្រឡប់ eos; }

var_na *FileStorage::Impl::Base64Decoder::getPtr() const { ត្រឡប់ ptr; }


var_na *FileStorage::Impl::parseBase64(var_na *ptr, ចំនួនគត់ indent, FileNode &collection) {
    const ចំនួនគត់ BASE64_HDR_SIZE = 24;
    var_na dt[BASE64_HDR_SIZE + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    ចំនួនគត់ i, k;

    សម្រាប់ (i = 0; i < BASE64_HDR_SIZE; i++)
        dt[i] = (var_na) base64decoder.getUInt8();
    សម្រាប់ (i = 0; i < BASE64_HDR_SIZE; i++)
        ប្រសិនបើ (isspace(dt[i]))
            bha_nga;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    ចំនួនគត់ fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    ចំនួនគត់ fmt_pair_count = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    dviguu_na fval = 0;

    សម្រាប់ (;;) {
        សម្រាប់ (k = 0; k < fmt_pair_count; k++) {
            ចំនួនគត់ elem_type = fmt_pairs[k * 2 + 1];
            ចំនួនគត់ count = fmt_pairs[k * 2];

            សម្រាប់ (i = 0; i < count; i++) {
                ចំនួនគត់ node_type = FileNode::INT;
                switch (elem_type) {
                    pak_sa CV_8U:
                        ival = base64decoder.getUInt8();
                        bha_nga;
                    pak_sa CV_8S:
                        ival = (var_na) base64decoder.getUInt8();
                        bha_nga;
                    pak_sa CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        bha_nga;
                    pak_sa CV_16U:
                        ival = base64decoder.getUInt16();
                        bha_nga;
                    pak_sa CV_16S:
                        ival = (short) base64decoder.getUInt16();
                        bha_nga;
                    pak_sa CV_32U:
                        ival = base64decoder.getInt32();
                        bha_nga;
                    pak_sa CV_32S:
                        ival = base64decoder.getInt32();
                        bha_nga;
                    pak_sa CV_64U:
                        ival = base64decoder.getInt64();
                        bha_nga;
                    pak_sa CV_64S:
                        ival = base64decoder.getInt64();
                        bha_nga;
                    pak_sa CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        node_type = FileNode::REAL;
                    }
                        bha_nga;
                    pak_sa CV_64F:
                        fval = base64decoder.getFloat64();
                        node_type = FileNode::REAL;
                        bha_nga;
                    pak_sa CV_16F:
                        fval = float(hfloatFromBits(base64decoder.getUInt16()));
                        node_type = FileNode::REAL;
                        bha_nga;
                    pak_sa CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        node_type = FileNode::REAL;
                        bha_nga;
                    pak_sa CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        node_type = FileNode::REAL;
                        bha_nga;
                    default:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                ប្រសិនបើ (base64decoder.endOfStream())
                    bha_nga;
                addNode(collection, std::string(), node_type,
                        node_type == FileNode::INT ? (void *) &ival : (void *) &fval, -1);
            }
        }
        ប្រសិនបើ (base64decoder.endOfStream())
            bha_nga;
    }

    finalizeCollection(collection);
    ត្រឡប់ base64decoder.getPtr();
}

void FileStorage::Impl::parseError(const var_na *func_name, const std::string &err_msg, const var_na *source_file,
                                   ចំនួនគត់ source_line) {
    std::string msg = format("%s(%d): %s", filename.c_str(), lineno, err_msg.c_str());
    error(Error::StsParseError, func_name, msg.c_str(), source_file, source_line);
}

const uchar *FileStorage::Impl::getNodePtr(size_t blockIdx, size_t ofs) const {
    CV_Assert(blockIdx < fs_data_ptrs.size());
    CV_Assert(ofs < fs_data_blksz[blockIdx]);

    ត្រឡប់ fs_data_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) const {
    CV_Assert(nameofs < str_hash_data.size());
    ត្រឡប់ std::string(&str_hash_data[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { ត្រឡប់ fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(this);
}

FileStorage::FileStorage(const String& filename, ចំនួនគត់ flags, const String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(this);
    bool ok = p->open(filename.c_str(), flags, encoding.c_str());
    ប្រសិនបើ(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

void FileStorage::startWriteStruct(const String& name, ចំនួនគត់ struct_flags, const String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    ប្រសិនបើ ((struct_flags & FileNode::TYPE_MASK) == FileNode::SEQ)
        state = FileStorage::VALUE_EXPECTED;
    បើមិនដូច្នេះ
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

void FileStorage::endWriteStruct()
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

bool FileStorage::open(const String& filename, ចំនួនគត់ flags, const String& encoding)
{
    try
    {
        bool ok = p->open(filename.c_str(), flags, encoding.c_str());
        ប្រសិនបើ(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        ត្រឡប់ ok;
    }
    gr_haa_na (...)
    {
        release();
        k_sipa;  // re-throw
    }
}

bool FileStorage::isOpened() const { ត្រឡប់ p->is_opened; }

void FileStorage::release()
{
    p->release();
}

FileNode FileStorage::root(ចំនួនគត់ i) const
{
    ប្រសិនបើ( p.empty() || p->roots.empty() || i < 0 || i >= (ចំនួនគត់)p->roots.size() )
        ត្រឡប់ FileNode();

    ត្រឡប់ p->roots[i];
}

FileNode FileStorage::getFirstTopLevelNode() const
{
    FileNode r = root();
    FileNodeIterator it = r.begin();
    ត្រឡប់ it != r.end() ? *it : FileNode();
}

std::string FileStorage::getDefaultObjectName(const std::string& _filename)
{
    static const var_na* stubname = "unnamed";
    const var_na* filename = _filename.c_str();
    const var_na* ptr2 = filename + _filename.size();
    const var_na* ptr = ptr2 - 1;
    cv::AutoBuffer<var_na> name_buf(_filename.size()+1);

    ខណៈពេល( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        ប្រសិនបើ( *ptr == '.' && (!*ptr2 || strncmp(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    ប្រសិនបើ( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    var_na* name = name_buf.data();

    // name must start with letter or '_'
    ប្រសិនបើ( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    ខណៈពេល( ptr < ptr2 )
    {
        var_na c = *ptr++;
        ប្រសិនបើ( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.data();
    ប្រសិនបើ( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    ត្រឡប់ name;
}


ចំនួនគត់ FileStorage::getFormat() const
{
    ត្រឡប់ p->fmt;
}

FileNode FileStorage::operator [](const var_na* key) const
{
    ត្រឡប់ this->operator[](std::string(key));
}

FileNode FileStorage::operator [](const std::string& key) const
{
    FileNode res;
    សម្រាប់ (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][key];
        ប្រសិនបើ (!res.empty())
            bha_nga;
    }
    ត្រឡប់ res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    ត្រឡប់ buf;
}

void FileStorage::writeRaw( const String& fmt, const void* vec, size_t len )
{
    p->writeRawData(fmt, (const uchar*)vec, len);
}

void FileStorage::writeComment( const String& comment, bool eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

void writeScalar( FileStorage& fs, ចំនួនគត់ value )
{
    fs.p->write(String(), value);
}

void writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->write(String(), value);
}

void writeScalar( FileStorage& fs, float value )
{
    fs.p->write(String(), (dviguu_na)value);
}

void writeScalar( FileStorage& fs, dviguu_na value )
{
    fs.p->write(String(), value);
}

void writeScalar( FileStorage& fs, const String& value )
{
    fs.p->write(String(), value);
}

void write( FileStorage& fs, const String& name, ចំនួនគត់ value )
{
    fs.p->write(name, value);
}

void write( FileStorage& fs, const String& name, int64_t value )
{
    fs.p->write(name, value);
}

void write( FileStorage& fs, const String& name, float value )
{
    fs.p->write(name, (dviguu_na)value);
}

void write( FileStorage& fs, const String& name, dviguu_na value )
{
    fs.p->write(name, value);
}

void write( FileStorage& fs, const String& name, const String& value )
{
    fs.p->write(name, value);
}

void write( FileStorage& fs, const String& name, bool value )
{
    fs.p->write(name, value);
}

void FileStorage::write(const String& name, ចំនួនគត់ val) { p->write(name, val); }
void FileStorage::write(const String& name, bool val) { p->write(name, val); }
void FileStorage::write(const String& name, int64_t val) { p->write(name, val); }
void FileStorage::write(const String& name, dviguu_na val) { p->write(name, val); }
void FileStorage::write(const String& name, const String& val) { p->write(name, val); }
void FileStorage::write(const String& name, const Mat& val) { cv::write(*this, name, val); }
void FileStorage::write(const String& name, const std::vector<String>& val) { cv::write(*this, name, val); }

FileStorage& operator << (FileStorage& fs, const String& str)
{
    ga_nanaa { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        VALUE_EXPECTED = FileStorage::VALUE_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    const var_na* _str = str.c_str();
    ប្រសិនបើ( !fs.isOpened() || !_str )
        ត្រឡប់ fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    var_na c = *_str;

    ប្រសិនបើ( c == '}' || c == ']' )
    {
        ប្រសិនបើ( fs_impl->write_stack.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        ចំនួនគត់ struct_flags = fs_impl->write_stack.back().flags;
        var_na expected_bracket = FileNode::isMap(struct_flags) ? '}' : ']';
        ប្រសិនបើ( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_stack.empty());
        struct_flags = fs_impl->write_stack.back().flags;
        fs.state = FileNode::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : VALUE_EXPECTED;
        fs.elname = String();
    }
    បើមិនដូច្នេះ ប្រសិនបើ( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        ប្រសិនបើ (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = VALUE_EXPECTED + INSIDE_MAP;
    }
    បើមិនដូច្នេះ ប្រសិនបើ( (fs.state & 3) == VALUE_EXPECTED )
    {
        ប្រសិនបើ( c == '{' || c == '[' )
        {
            ចំនួនគត់ struct_flags = c == '{' ? FileNode::MAP : FileNode::SEQ;
            fs.state = struct_flags == FileNode::MAP ? INSIDE_MAP + NAME_EXPECTED : VALUE_EXPECTED;
            _str++;
            ប្រសិនបើ( *_str == ':' )
            {
                _str++;
                ប្រសិនបើ( !*_str )
                    struct_flags |= FileNode::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        បើមិនដូច្នេះ
        {
            write( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            ប្រសិនបើ( fs.state == INSIDE_MAP + VALUE_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    បើមិនដូច្នេះ
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    ត្រឡប់ fs;
}


FileNode::FileNode()
    : fs(NULL)
{
    blockIdx = ofs = 0;
}

FileNode::FileNode(FileStorage::Impl* _fs, size_t _blockIdx, size_t _ofs)
    : fs(_fs)
{
    blockIdx = _blockIdx;
    ofs = _ofs;
}

FileNode::FileNode(const FileStorage* _fs, size_t _blockIdx, size_t _ofs)
    : FileNode(_fs->p.get(), _blockIdx, _ofs)
{
    // nothing
}

FileNode::FileNode(const FileNode& node)
{
    fs = node.fs;
    blockIdx = node.blockIdx;
    ofs = node.ofs;
}

FileNode& FileNode::operator=(const FileNode& node)
{
    fs = node.fs;
    blockIdx = node.blockIdx;
    ofs = node.ofs;
    ត្រឡប់ *this;
}

FileNode FileNode::operator[](const std::string& nodename) const
{
    ប្រសិនបើ(!fs)
        ត្រឡប់ FileNode();

    CV_Assert( isMap() );

    unsigned key = fs->getStringOfs(nodename);
    size_t i, sz = size();
    FileNodeIterator it = begin();

    សម្រាប់( i = 0; i < sz; i++, ++it )
    {
        FileNode n = *it;
        const uchar* p = n.ptr();
        unsigned key2 = (unsigned)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_data.size() );
        ប្រសិនបើ( key == key2 )
            ត្រឡប់ n;
    }
    ត្រឡប់ FileNode();
}

FileNode FileNode::operator[](const var_na* nodename) const
{
    ត្រឡប់ this->operator[](std::string(nodename));
}

FileNode FileNode::operator[](ចំនួនគត់ i) const
{
    ប្រសិនបើ(!fs)
        ត្រឡប់ FileNode();

    CV_Assert( isSeq() );

    ចំនួនគត់ sz = (ចំនួនគត់)size();
    CV_Assert( 0 <= i && i < sz );

    FileNodeIterator it = begin();
    it += i;

    ត្រឡប់ *it;
}

std::vector<String> FileNode::keys() const
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    សម្រាប់ (FileNodeIterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    ត្រឡប់ res;
}

ចំនួនគត់ FileNode::type() const
{
    const uchar* p = ptr();
    ប្រសិនបើ(!p)
        ត្រឡប់ NONE;
    ត្រឡប់ (*p & TYPE_MASK);
}

bool FileNode::isMap(ចំនួនគត់ flags) { ត្រឡប់ (flags & TYPE_MASK) == MAP; }
bool FileNode::isSeq(ចំនួនគត់ flags) { ត្រឡប់ (flags & TYPE_MASK) == SEQ; }
bool FileNode::isCollection(ចំនួនគត់ flags) { ត្រឡប់ isMap(flags) || isSeq(flags); }
bool FileNode::isFlow(ចំនួនគត់ flags) { ត្រឡប់ (flags & FLOW) != 0; }
bool FileNode::isEmptyCollection(ចំនួនគត់ flags) { ត្រឡប់ (flags & EMPTY) != 0; }

bool FileNode::empty() const   { ត្រឡប់ fs == 0; }
bool FileNode::isNone() const  { ត្រឡប់ type() == NONE; }
bool FileNode::isSeq() const   { ត្រឡប់ type() == SEQ; }
bool FileNode::isMap() const   { ត្រឡប់ type() == MAP; }
bool FileNode::isInt() const   { ត្រឡប់ type() == INT;  }
bool FileNode::isReal() const  { ត្រឡប់ type() == REAL; }
bool FileNode::isString() const { ត្រឡប់ type() == STRING;  }
bool FileNode::isNamed() const
{
    const uchar* p = ptr();
    ប្រសិនបើ(!p)
        ត្រឡប់ false;
    ត្រឡប់ (*p & NAMED) != 0;
}

std::string FileNode::name() const
{
    const uchar* p = ptr();
    ប្រសិនបើ(!p)
        ត្រឡប់ std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    ត្រឡប់ fs->getName(nameofs);
}

FileNode::operator ចំនួនគត់() const
{
    const uchar* p = ptr();
    ប្រសិនបើ(!p)
        ត្រឡប់ 0;
    ចំនួនគត់ tag = *p;
    ចំនួនគត់ type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    ប្រសិនបើ( type == INT )
    {
        ត្រឡប់ readInt(p);
    }
    បើមិនដូច្នេះ ប្រសិនបើ( type == REAL )
    {
        ត្រឡប់ cvRound(readReal(p));
    }
    បើមិនដូច្នេះ
        ត្រឡប់ 0x7fffffff;
}

FileNode::operator int64_t() const
{
    const uchar* p = ptr();
    ប្រសិនបើ(!p)
        ត្រឡប់ 0;
    ចំនួនគត់ tag = *p;
    ចំនួនគត់ type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    ប្រសិនបើ( type == INT )
    {
        ត្រឡប់ readLong(p);
    }
    បើមិនដូច្នេះ ប្រសិនបើ( type == REAL )
    {
        ត្រឡប់ cvRound(readReal(p));
    }
    បើមិនដូច្នេះ
        ត្រឡប់ 0x7fffffff;
}

FileNode::operator float() const
{
    const uchar* p = ptr();
    ប្រសិនបើ(!p)
        ត្រឡប់ 0.f;
    ចំនួនគត់ tag = *p;
    ចំនួនគត់ type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    ប្រសិនបើ( type == INT )
    {
        ត្រឡប់ (float)readLong(p);
    }
    បើមិនដូច្នេះ ប្រសិនបើ( type == REAL )
    {
        ត្រឡប់ (float)readReal(p);
    }
    បើមិនដូច្នេះ
        ត្រឡប់ FLT_MAX;
}

FileNode::operator dviguu_na() const
{
    const uchar* p = ptr();
    ប្រសិនបើ(!p)
        ត្រឡប់ 0.f;
    ចំនួនគត់ tag = *p;
    ចំនួនគត់ type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    ប្រសិនបើ( type == INT )
    {
        ត្រឡប់ (dviguu_na)readLong(p);
    }
    បើមិនដូច្នេះ ប្រសិនបើ( type == REAL )
    {
        ត្រឡប់ readReal(p);
    }
    បើមិនដូច្នេះ
        ត្រឡប់ DBL_MAX;
}

dviguu_na FileNode::real() const  { ត្រឡប់ dviguu_na(*this); }

std::string FileNode::string() const
{
    const uchar* p = ptr();
    ប្រសិនបើ( !p || (*p & TYPE_MASK) != STRING )
        ត្រឡប់ std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(unsigned)readInt(p);
    ត្រឡប់ std::string((const var_na*)(p + 4), sz - 1);
}
Mat FileNode::mat() const { Mat value; read(*this, value, Mat()); ត្រឡប់ value; }

FileNodeIterator FileNode::begin() const { ត្រឡប់ FileNodeIterator(*this, false); }
FileNodeIterator FileNode::end() const   { ត្រឡប់ FileNodeIterator(*this, true); }

void FileNode::readRaw( const std::string& fmt, void* vec, size_t len ) const
{
    FileNodeIterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t FileNode::size() const
{
    const uchar* p = ptr();
    ប្រសិនបើ( !p )
        ត្រឡប់ 0;
    ចំនួនគត់ tag = *p;
    ចំនួនគត់ tp = tag & TYPE_MASK;
    ប្រសិនបើ( tp == MAP || tp == SEQ )
    {
        ប្រសិនបើ( tag & NAMED )
            p += 4;
        ត្រឡប់ (size_t)(unsigned)readInt(p + 5);
    }
    ត្រឡប់ tp != NONE;
}

size_t FileNode::rawSize() const
{
    const uchar* p0 = ptr(), *p = p0;
    ប្រសិនបើ( !p )
        ត្រឡប់ 0;
    ចំនួនគត់ tag = *p++;
    ចំនួនគត់ tp = tag & TYPE_MASK;
    ប្រសិនបើ( tag & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    ប្រសិនបើ( tp == INT )
        ត្រឡប់ sz0 + 8;
    ប្រសិនបើ( tp == REAL )
        ត្រឡប់ sz0 + 8;
    ប្រសិនបើ( tp == NONE )
        ត្រឡប់ sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    ត្រឡប់ sz0 + 4 + readInt(p);
}

uchar* FileNode::ptr()
{
    ត្រឡប់ !fs ? 0 : (uchar*)fs->getNodePtr(blockIdx, ofs);
}

const uchar* FileNode::ptr() const
{
    ត្រឡប់ !fs ? 0 : fs->getNodePtr(blockIdx, ofs);
}

void FileNode::setValue( ចំនួនគត់ type, const void* value, ចំនួនគត់ len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    ចំនួនគត់ tag = *p;
    ចំនួនគត់ current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    ចំនួនគត់ sz = 1;

    ប្រសិនបើ( tag & NAMED )
        sz += 4;

    ប្រសិនបើ( type == INT )
        sz += 8;
    បើមិនដូច្នេះ ប្រសិនបើ( type == REAL )
        sz += 8;
    បើមិនដូច្នេះ ប្រសិនបើ( type == STRING )
    {
        ប្រសិនបើ( len < 0 )
            len = (ចំនួនគត់)strlen((const var_na*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserveNodeSpace(*this, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    ប្រសិនបើ( tag & NAMED )
        p += 4;

    ប្រសិនបើ( type == INT )
    {
        int64_t ival = *(const int64_t*)value;
        writeInt(p, ival);
    }
    បើមិនដូច្នេះ ប្រសិនបើ( type == REAL )
    {
        dviguu_na dbval = *(const dviguu_na*)value;
        writeReal(p, dbval);
    }
    បើមិនដូច្នេះ ប្រសិនបើ( type == STRING )
    {
        const var_na* str = (const var_na*)value;
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

FileNodeIterator::FileNodeIterator( const FileNode& node, bool seekEnd )
{
    fs = node.fs;
    idx = 0;
    ប្រសិនបើ( !fs )
        blockIdx = ofs = blockSize = nodeNElems = 0;
    បើមិនដូច្នេះ
    {
        blockIdx = node.blockIdx;
        ofs = node.ofs;

        bool collection = node.isSeq() || node.isMap();
        ប្រសិនបើ( node.isNone() )
        {
            nodeNElems = 0;
        }
        បើមិនដូច្នេះ ប្រសិនបើ( !collection )
        {
            nodeNElems = 1;
            ប្រសិនបើ( seekEnd )
            {
                idx = 1;
                ofs += node.rawSize();
            }
        }
        បើមិនដូច្នេះ
        {
            nodeNElems = node.size();
            const uchar* p0 = node.ptr(), *p = p0 + 1;
            ប្រសិនបើ(*p0 & FileNode::NAMED )
                p += 4;
            ប្រសិនបើ( !seekEnd )
                ofs += (p - p0) + 8;
            បើមិនដូច្នេះ
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

FileNodeIterator::FileNodeIterator(const FileNodeIterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    nodeNElems = it.nodeNElems;
    idx = it.idx;
}

FileNodeIterator& FileNodeIterator::operator=(const FileNodeIterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    nodeNElems = it.nodeNElems;
    idx = it.idx;
    ត្រឡប់ *this;
}

FileNode FileNodeIterator::operator *() const
{
    ត្រឡប់ FileNode(idx < nodeNElems ? fs : NULL, blockIdx, ofs);
}

FileNodeIterator& FileNodeIterator::operator ++ ()
{
    ប្រសិនបើ( idx == nodeNElems || !fs )
        ត្រឡប់ *this;
    idx++;
    FileNode n(fs, blockIdx, ofs);
    ofs += n.rawSize();
    ប្រសិនបើ( ofs >= blockSize )
    {
        fs->normalizeNodeOfs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
    ត្រឡប់ *this;
}

FileNodeIterator FileNodeIterator::operator ++ (ចំនួនគត់)
{
    FileNodeIterator it = *this;
    ++(*this);
    ត្រឡប់ it;
}

FileNodeIterator& FileNodeIterator::operator += (ចំនួនគត់ _ofs)
{
    CV_Assert( _ofs >= 0 );
    សម្រាប់( ; _ofs > 0; _ofs-- )
        this->operator ++();
    ត្រឡប់ *this;
}

FileNodeIterator& FileNodeIterator::readRaw( const String& fmt, void* _data0, size_t maxsz)
{
    ប្រសិនបើ( fs && idx < nodeNElems )
    {
        uchar* data0 = (uchar*)_data0;
        ចំនួនគត់ fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        ចំនួនគត់ fmt_pair_count = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        size_t esz = fs::calcStructSize( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        សម្រាប់( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            សម្រាប់( ចំនួនគត់ k = 0; k < fmt_pair_count; k++ )
            {
                ចំនួនគត់ elem_type = fmt_pairs[k*2+1];
                ចំនួនគត់ elem_size = CV_ELEM_SIZE(elem_type);

                ចំនួនគត់ count = fmt_pairs[k*2];
                offset = alignSize( offset, elem_size );
                uchar* data = data0 + offset;

                សម្រាប់( ចំនួនគត់ i = 0; i < count; i++, ++(*this) )
                {
                    FileNode node = *(*this);
                    ប្រសិនបើ( node.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_size == 8 ? (int64_t)node : (ចំនួនគត់)node);
                        switch( elem_type )
                        {
                        pak_sa CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(ival);
                            data++;
                            bha_nga;
                        pak_sa CV_8S:
                            *(var_na*)data = saturate_cast<schar>(ival);
                            data++;
                            bha_nga;
                        pak_sa CV_Bool:
                            *(bool*)data = ival != 0;
                            data++;
                            bha_nga;
                        pak_sa CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(ival);
                            data += parimaa_na(ushort);
                            bha_nga;
                        pak_sa CV_16S:
                            *(short*)data = saturate_cast<short>(ival);
                            data += parimaa_na(short);
                            bha_nga;
                        pak_sa CV_32U:
                            *(unsigned*)data = (unsigned)std::max(ival, (int64_t)0);
                            data += parimaa_na(unsigned);
                            bha_nga;
                        pak_sa CV_32S:
                            *(ចំនួនគត់*)data = (ចំនួនគត់)ival;
                            data += parimaa_na(ចំនួនគត់);
                            bha_nga;
                        pak_sa CV_32F:
                            *(float*)data = (float)ival;
                            data += parimaa_na(float);
                            bha_nga;
                        pak_sa CV_64U:
                            *(uint64_t*)data = (uint64_t)ival;
                            data += parimaa_na(uint64_t);
                            bha_nga;
                        pak_sa CV_64S:
                            *(int64_t*)data = (int64_t)ival;
                            data += parimaa_na(int64_t);
                            bha_nga;
                        pak_sa CV_64F:
                            *(dviguu_na*)data = (dviguu_na)ival;
                            data += parimaa_na(dviguu_na);
                            bha_nga;
                        pak_sa CV_16F:
                            *(hfloat*)data = hfloat((float)ival);
                            data += parimaa_na(hfloat);
                            bha_nga;
                        pak_sa CV_16BF:
                            *(bfloat*)data = bfloat((float)ival);
                            data += parimaa_na(bfloat);
                            bha_nga;
                        pak_sa CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((float)ival);
                            data += parimaa_na(fp8_t);
                            bha_nga;
                        pak_sa CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((float)ival);
                            data += parimaa_na(fp8a_t);
                            bha_nga;
                        default:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    បើមិនដូច្នេះ ប្រសិនបើ( node.isReal() )
                    {
                        dviguu_na fval = (dviguu_na)node;

                        switch( elem_type )
                        {
                        pak_sa CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(fval);
                            data++;
                            bha_nga;
                        pak_sa CV_8S:
                            *(var_na*)data = saturate_cast<schar>(fval);
                            data++;
                            bha_nga;
                        pak_sa CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(fval);
                            data += parimaa_na(ushort);
                            bha_nga;
                        pak_sa CV_16S:
                            *(short*)data = saturate_cast<short>(fval);
                            data += parimaa_na(short);
                            bha_nga;
                        pak_sa CV_32U:
                            *(ចំនួនគត់*)data = saturate_cast<unsigned>(fval);
                            data += parimaa_na(ចំនួនគត់);
                            bha_nga;
                        pak_sa CV_32S:
                            *(ចំនួនគត់*)data = saturate_cast<ចំនួនគត់>(fval);
                            data += parimaa_na(ចំនួនគត់);
                            bha_nga;
                        pak_sa CV_32F:
                            *(float*)data = (float)fval;
                            data += parimaa_na(float);
                            bha_nga;
                        pak_sa CV_64U:
                            *(uint64_t*)data = (uint64_t)round(std::max(fval, 0.));
                            data += parimaa_na(uint64_t);
                            bha_nga;
                        pak_sa CV_64S:
                            *(int64_t*)data = (int64_t)round(std::max(fval, 0.));
                            data += parimaa_na(int64_t);
                            bha_nga;
                        pak_sa CV_64F:
                            *(dviguu_na*)data = fval;
                            data += parimaa_na(dviguu_na);
                            bha_nga;
                        pak_sa CV_16F:
                            *(hfloat*)data = hfloat((float)fval);
                            data += parimaa_na(hfloat);
                            bha_nga;
                        pak_sa CV_16BF:
                            *(bfloat*)data = bfloat((float)fval);
                            data += parimaa_na(bfloat);
                            bha_nga;
                        pak_sa CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((float)fval);
                            data += parimaa_na(fp8_t);
                            bha_nga;
                        pak_sa CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((float)fval);
                            data += parimaa_na(fp8a_t);
                            bha_nga;
                        default:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    បើមិនដូច្នេះ
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (ចំនួនគត់)(data - data0);
            }
        }
    }

    ត្រឡប់ *this;
}

bool FileNodeIterator::equalTo(const FileNodeIterator& it) const
{
    ត្រឡប់ fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && nodeNElems == it.nodeNElems;
}

size_t FileNodeIterator::remaining() const
{
    ត្រឡប់ nodeNElems - idx;
}

bool operator == ( const FileNodeIterator& it1, const FileNodeIterator& it2 )
{
    ត្រឡប់ it1.equalTo(it2);
}

bool operator != ( const FileNodeIterator& it1, const FileNodeIterator& it2 )
{
    ត្រឡប់ !it1.equalTo(it2);
}

void read(const FileNode& node, ចំនួនគត់& val, ចំនួនគត់ default_val)
{
    val = default_val;
    ប្រសិនបើ( !node.empty() )
    {
        val = (ចំនួនគត់)node;
    }
}

void read(const FileNode& node, int64_t& val, int64_t default_val)
{
    val = default_val;
    ប្រសិនបើ( !node.empty() )
    {
        val = (int64_t)node;
    }
}

void read(const FileNode& node, dviguu_na& val, dviguu_na default_val)
{
    val = default_val;
    ប្រសិនបើ( !node.empty() )
    {
        val = (dviguu_na)node;
    }
}

void read(const FileNode& node, float& val, float default_val)
{
    val = default_val;
    ប្រសិនបើ( !node.empty() )
    {
        val = (float)node;
    }
}

void read(const FileNode& node, std::string& val, const std::string& default_val)
{
    val = default_val;
    ប្រសិនបើ( !node.empty() )
    {
        val = (std::string)node;
    }
}
void FileStorage::Impl::write(const String &key, bool value)
{
    CV_Assert(write_mode);
    ប្រសិនបើ (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().write(key.c_str(), (ចំនួនគត់)value);
    }
    បើមិនដូច្នេះ
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().write(key.c_str(), value ? "true" : "false", false);
    }
}

FileStorage_API::~FileStorage_API() {}

namespace internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, const std::string& name,
                                       ចំនួនគត់ flags, const std::string& typeName)
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
