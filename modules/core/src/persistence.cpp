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

số_nguyên strcasecmp(const char* s1, const char* s2)
{
    const char* dummy="";
    nếu(!s1) s1=dummy;
    nếu(!s2) s2=dummy;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t i, len = std::min(len1, len2);
    cho( i = 0; i < len; i++ )
    {
        số_nguyên d = tolower((số_nguyên)s1[i]) - tolower((số_nguyên)s2[i]);
        nếu( d != 0 )
            trả_về d;
    }
    trả_về len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

char* itoa( số_nguyên _val, char* buffer, số_nguyên /*radix*/ )
{
    const số_nguyên radix = 10;
    char* ptr=buffer + 23 /* enough even for 64-bit integers */;
    unsigned val = abs(_val);

    *ptr = '\0';
    do
    {
        unsigned r = val / radix;
        *--ptr = (char)(val - (r*radix) + '0');
        val = r;
    }
    trong_khi( val != 0 );

    nếu( _val < 0 )
        *--ptr = '-';

    trả_về ptr;
}

char* itoa( int64_t _val, char* buffer, số_nguyên /*radix*/, bool _signed)
{
    const số_nguyên radix = 10;
    char* ptr=buffer + 23 /* enough even for 64-bit integers */;
    số_nguyên sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : abs(_val);

    *ptr = '\0';
    do
    {
        uint64_t r = val / radix;
        *--ptr = (char)(val - (r*radix) + '0');
        val = r;
    }
    trong_khi( val != 0 );

    nếu( sign < 0 )
        *--ptr = '-';

    trả_về ptr;
}

char* doubleToString( char* buf, size_t bufSize, double value, bool explicitZero )
{
    Cv64suf val;
    unsigned ieee754_hi;

    val.f = value;
    ieee754_hi = (unsigned)(val.u >> 32);

    nếu( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        số_nguyên ivalue = cvRound(value);
        nếu( ivalue == value )
        {
            nếu( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            không_thì
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        không_thì
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, bufSize, "%.17g", value );

            char* ptr = buf;
            nếu( *ptr == '+' || *ptr == '-' )
                ptr++;
            cho( ; cv_isdigit(*ptr); ptr++ )
                ;
            nếu( *ptr == ',' )
                *ptr = '.';
        }
    }
    không_thì
    {
        unsigned ieee754_lo = (unsigned)val.u;
        nếu( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        không_thì
            strcpy( buf, (số_nguyên)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    trả_về buf;
}

char* floatToString( char* buf, size_t bufSize, float value, bool halfprecision, bool explicitZero )
{
    Cv32suf val;
    unsigned ieee754;
    val.f = value;
    ieee754 = val.u;

    nếu( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        số_nguyên ivalue = cvRound(value);
        nếu( ivalue == value )
        {
            nếu( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            không_thì
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        không_thì
        {
            nếu (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.5g", value);
            }
            không_thì
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.9g", value);
            }

            char* ptr = buf;
            nếu( *ptr == '+' || *ptr == '-' )
                ptr++;
            cho( ; cv_isdigit(*ptr); ptr++ )
                ;
            nếu( *ptr == ',' )
                *ptr = '.';
        }
    }
    không_thì
    {
        nếu( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        không_thì
            strcpy( buf, (số_nguyên)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    trả_về buf;
}

static const char symbols[] = "ucwsifdhHbUIneE";

static char typeSymbol(số_nguyên depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    trả_về symbols[depth];
}

static số_nguyên symbolToType(char c)
{
    nếu (c == 'r')
        trả_về CV_SEQ_ELTYPE_PTR;
    const char* pos = strchr( symbols, c );
    nếu( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    trả_về static_cast<số_nguyên>(pos - symbols);
}

char* encodeFormat(số_nguyên elem_type, char* dt, size_t dt_len)
{
    số_nguyên cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    char symbol = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : typeSymbol(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, symbol);
    trả_về dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
char* encodeFormat(số_nguyên elem_type, char* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    trả_về encodeFormat(elem_type, dt, max);
}

số_nguyên decodeFormat( const char* dt, số_nguyên* fmt_pairs, số_nguyên max_len )
{
    số_nguyên fmt_pair_count = 0;
    số_nguyên i = 0, k = 0, len = dt ? (số_nguyên)strlen(dt) : 0;

    nếu( !dt || !len )
        trả_về 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    cho( ; k < len; k++ )
    {
        char c = dt[k];

        nếu( cv_isdigit(c) )
        {
            số_nguyên count = c - '0';
            nếu( cv_isdigit(dt[k+1]) )
            {
                char* endptr = 0;
                count = (số_nguyên)strtol( dt+k, &endptr, 10 );
                k = (số_nguyên)(endptr - dt) - 1;
            }

            nếu( count <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = count;
        }
        không_thì
        {
            số_nguyên depth = symbolToType(c);
            nếu( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = depth;
            nếu( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            không_thì
            {
                i += 2;
                nếu( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_count = i/2;
    trả_về fmt_pair_count;
}

số_nguyên calcElemSize( const char* dt, số_nguyên initial_size )
{
    số_nguyên size = 0;
    số_nguyên fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_count;
    số_nguyên comp_size;

    fmt_pair_count = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_count *= 2;
    cho( i = 0, size = initial_size; i < fmt_pair_count; i += 2 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[i+1]);
        size = cvAlign( size, comp_size );
        size += comp_size * fmt_pairs[i];
    }
    nếu( initial_size == 0 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[1]);
        size = cvAlign( size, comp_size );
    }
    trả_về size;
}


số_nguyên calcStructSize( const char* dt, số_nguyên initial_size )
{
    số_nguyên size = calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    cho ( const char * type = dt; *type != '\0'; type++ )
    {
        char v = *type;
        nếu (v >= '0' && v <= '9')
            continue;  // skip vector size
        switch (v)
        {
        case 'u': { elem_max_size = std::max( elem_max_size, sizeof(uchar ) ); break; }
        case 'b': { elem_max_size = std::max( elem_max_size, sizeof(bool  ) ); break; }
        case 'c': { elem_max_size = std::max( elem_max_size, sizeof(schar ) ); break; }
        case 'w': { elem_max_size = std::max( elem_max_size, sizeof(ushort) ); break; }
        case 's': { elem_max_size = std::max( elem_max_size, sizeof(short ) ); break; }
        case 'i': { elem_max_size = std::max( elem_max_size, sizeof(số_nguyên   ) ); break; }
        case 'n': { elem_max_size = std::max( elem_max_size, sizeof(unsigned) ); break; }
        case 'f': { elem_max_size = std::max( elem_max_size, sizeof(float ) ); break; }
        case 'd': { elem_max_size = std::max( elem_max_size, sizeof(double) ); break; }
        case 'h': { elem_max_size = std::max( elem_max_size, sizeof(hfloat)); break; }
        case 'H': { elem_max_size = std::max( elem_max_size, sizeof(bfloat)); break; }
        case 'e': { elem_max_size = std::max( elem_max_size, sizeof(fp8_t) ); break; }
        case 'E': { elem_max_size = std::max( elem_max_size, sizeof(fp8a_t)); break; }
        case 'I': { elem_max_size = std::max( elem_max_size, sizeof(int64_t)); break; }
        case 'U': { elem_max_size = std::max( elem_max_size, sizeof(uint64_t)); break; }
        default:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (char)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<số_nguyên>(elem_max_size) );
    trả_về size;
}

số_nguyên decodeSimpleFormat( const char* dt )
{
    số_nguyên elem_type = -1;
    số_nguyên fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_count;

    fmt_pair_count = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    nếu( fmt_pair_count != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    trả_về elem_type;
}

}

#nếu defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#define CV_LITTLE_ENDIAN_MEM_ACCESS 1
#không_thì
#define CV_LITTLE_ENDIAN_MEM_ACCESS 0
#endif

static inline số_nguyên readInt(const uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#nếu CV_LITTLE_ENDIAN_MEM_ACCESS
    số_nguyên val;
    memcpy(&val, p, sizeof(val));
    trả_về val;
#không_thì
    số_nguyên val = (số_nguyên)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    trả_về val;
#endif
}

static inline int64_t readLong(const uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#nếu CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    memcpy(&val, p, sizeof(val));
    trả_về val;
#không_thì
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    trả_về val0 | ((int64_t)val1 << 32);
#endif
}

static inline double readReal(const uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#nếu CV_LITTLE_ENDIAN_MEM_ACCESS
    double val;
    memcpy(&val, p, sizeof(val));
    trả_về val;
#không_thì
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    trả_về val.f;
#endif
}

template <typename T>
static inline void writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#nếu CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &ival, sizeof(ival));
#không_thì
    cho (size_t i = 0, j = 0; i < sizeof(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#endif
}

static inline void writeReal(uchar* p, double fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#nếu CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &fval, sizeof(fval));
#không_thì
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
    nếu (is_opened) {
        nếu (out)
            out->clear();
        nếu (write_mode) {
            trong_khi (write_stack.size() > 1) {
                endWriteStruct();
            }
            flush();
            nếu (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            không_thì nếu (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        nếu (mem_mode && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

void FileStorage::Impl::analyze_file_name(const std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    static const char not_file_name = '\n';
    static const char parameter_begin = '?';
    static const char parameter_separator = '&';

    nếu (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        trả_về;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    nếu (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        cho (size_t param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            nếu ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

bool FileStorage::Impl::open(const char *filename_or_buf, số_nguyên _flags, const char *encoding) {
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
        nếu (!params.empty())
            filename = params[0];

        nếu (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_mode || append);
    }

    nếu (filename.size() == 0 && !mem_mode && !write_mode)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    nếu (mem_mode && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    nếu (!mem_mode) {
        size_t dot_idx = filename.find_last_of('.');
        char compression = '\0';

        nếu (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            nếu (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                nếu (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = true;
                }

                không_thì nếu(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = true;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        nếu (!isGZ) {
            file = fopen(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            nếu (!file)
            {
                CV_LOG_ERROR(NULL, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                trả_về false;
            }
        } không_thì {
#nếu USE_ZLIB
            nếu (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            char mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            nếu (!gzfile)
            {
                CV_LOG_ERROR(NULL, "Can't open archive: '" << filename << "' mode=" << mode);
                trả_về false;
            }
#không_thì
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#endif
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_data.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    nếu (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        nếu (mem_mode)
            outbuf.clear();

        nếu (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            const char *dot_pos = NULL;
            const char *dot_pos2 = NULL;
            // like strrchr() implementation, but save two last positions simultaneously
            cho (const char *pos = &filename[0]; pos[0] != 0; pos++) {
                nếu (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            nếu (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != NULL) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } không_thì nếu (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        số_nguyên buf_size = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        nếu (append) {
            fseek(file, 0, SEEK_END);
            nếu (ftell(file) == 0)
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

        nếu (fmt == FileStorage::FORMAT_XML) {
            size_t file_size = file ? (size_t) ftell(file) : (size_t) 0;
            nếu (!append || file_size == 0) {
                nếu (encoding && *encoding != '\0') {
                    nếu (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(strlen(encoding) < 1000);
                    char buf[1100];
                    snprintf(buf, sizeof(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } không_thì
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } không_thì {
                số_nguyên xml_buf_size = 1 << 10;
                char substr[] = "</opencv_storage>";
                số_nguyên last_occurrence = -1;
                xml_buf_size = MIN(xml_buf_size, số_nguyên(file_size));
                fseek(file, -xml_buf_size, SEEK_END);
                // find the last occurrence of </opencv_storage>
                cho (;;) {
                    số_nguyên line_offset = (số_nguyên) ftell(file);
                    const char *ptr0 = this->gets(xml_buf_size);
                    const char *ptr = NULL;
                    nếu (!ptr0)
                        break;
                    ptr = ptr0;
                    cho (;;) {
                        ptr = strstr(ptr, substr);
                        nếu (!ptr)
                            break;
                        last_occurrence = line_offset + (số_nguyên) (ptr - ptr0);
                        ptr += strlen(substr);
                    }
                }
                nếu (last_occurrence < 0) {
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
        } không_thì nếu (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                nếu (!append) {
                    nếu (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    không_thì
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            không_thì
                puts("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(this);
        } không_thì {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            nếu (!append)
                puts("{\n");
            không_thì {
                bool valid = false;
                long roffset = 0;
                cho (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    const char end_mark = '}';
                    nếu (fgetc(file) == end_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = true;
                        break;
                    }
                }

                nếu (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } không_thì {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_stack.back().indent = 4;
            emitter_do_not_use_direct_dereference = createJSONEmitter(this);
        }
        is_opened = true;
    } không_thì {
        const size_t buf_size0 = 40;
        buffer.resize(buf_size0);
        nếu (mem_mode) {
            strbuf = (char *) filename_or_buf;
            strbufsize = strlen(strbuf);
        }

        const char *yaml_signature = "%YAML";
        const char *json_signature = "{";
        const char *xml_signature = "<?xml";
        char *buf = this->gets(16);
        CV_Assert(buf);
        char *bufPtr = cv_skip_BOM(buf);
        size_t bufOffset = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        nếu (strncmp(bufPtr, yaml_signature, strlen(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        không_thì nếu (strncmp(bufPtr, json_signature, strlen(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        không_thì nếu (strncmp(bufPtr, xml_signature, strlen(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        không_thì
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = bufOffset;
        bufofs = 0;

        try {
            char *ptr = bufferStart();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            FileNode root_nodes(fs_ext, 0, 0);

            uchar *rptr = reserveNodeSpace(root_nodes, 9);
            *rptr = FileNode::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            switch (fmt) {
                case FileStorage::FORMAT_XML:
                    parser_do_not_use_direct_dereference = createXMLParser(this);
                    break;
                case FileStorage::FORMAT_YAML:
                case FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_direct_dereference = createYAMLParser(this);
                    break;
                case FileStorage::FORMAT_JSON:
                    parser_do_not_use_direct_dereference = createJSONParser(this);
                    break;
                default:
                    parser_do_not_use_direct_dereference = Ptr<FileStorageParser>();
            }

            nếu (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                nếu (ok) {
                    finalizeCollection(root_nodes);

                    CV_Assert(!fs_data_ptrs.empty());
                    FileNode roots_node(fs_ext, 0, 0);
                    size_t i, nroots = roots_node.size();
                    FileNodeIterator it = roots_node.begin();

                    cho (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        catch (...)
        {
            // FIXIT log error message
            is_opened = true;
            release();
            throw;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = true;
        std::vector<char> tmpbuf;
        std::swap(buffer, tmpbuf);
        bufofs = 0;
    }
    trả_về ok;
}

void FileStorage::Impl::puts(const char *str) {
    CV_Assert(write_mode);
    nếu (mem_mode)
        std::copy(str, str + strlen(str), std::back_inserter(outbuf));
    không_thì nếu (file)
        fputs(str, file);
#nếu USE_ZLIB
    không_thì nếu (gzfile)
        gzputs(gzfile, str);
#endif
    không_thì
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

char *FileStorage::Impl::getsFromFile(char *buf, số_nguyên count) {
    nếu (file)
        trả_về fgets(buf, count, file);
#nếu USE_ZLIB
    nếu (gzfile)
        trả_về gzgets(gzfile, buf, count);
#endif
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

char *FileStorage::Impl::gets(size_t maxCount) {
    nếu (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        const char *instr = strbuf;
        cho (; i < len; i++) {
            char c = instr[i];
            nếu (c == '\0' || c == '\n') {
                nếu (c == '\n')
                    i++;
                break;
            }
        }
        size_t count = i - strbufpos;
        nếu (maxCount == 0 || maxCount > count)
            maxCount = count;
        buffer.resize(std::max(buffer.size(), maxCount + 8));
        memcpy(&buffer[0], instr + strbufpos, maxCount);
        buffer[maxCount] = '\0';
        strbufpos = i;
        trả_về maxCount > 0 ? &buffer[0] : 0;
    }

    const size_t MAX_BLOCK_SIZE = INT_MAX / 2; // hopefully, that will be enough
    nếu (maxCount == 0)
        maxCount = MAX_BLOCK_SIZE;
    không_thì
        CV_Assert(maxCount < MAX_BLOCK_SIZE);
    size_t ofs = 0;

    cho (;;) {
        số_nguyên count = (số_nguyên) std::min(buffer.size() - ofs - 16, maxCount);
        char *ptr = getsFromFile(&buffer[ofs], count + 1);
        nếu (!ptr)
            break;
        số_nguyên delta = (số_nguyên) strlen(ptr);
        ofs += delta;
        maxCount -= delta;
        nếu (delta == 0 || ptr[delta - 1] == '\n' || maxCount == 0)
            break;
        nếu (delta == count)
            buffer.resize((size_t) (buffer.size() * 1.5));
    }
    trả_về ofs > 0 ? &buffer[0] : 0;
}

char *FileStorage::Impl::gets() {
    char *ptr = this->gets(0);
    nếu (!ptr) {
        ptr = bufferStart();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        trả_về 0;
    } không_thì {
        size_t l = strlen(ptr);
        nếu (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    trả_về ptr;
}

bool FileStorage::Impl::eof() {
    nếu (dummy_eof)
        trả_về true;
    nếu (strbuf)
        trả_về strbufpos >= strbufsize;
    nếu (file)
        trả_về feof(file) != 0;
#nếu USE_ZLIB
    nếu (gzfile)
        trả_về gzeof(gzfile) != 0;
#endif
    trả_về false;
}

void FileStorage::Impl::setEof() {
    dummy_eof = true;
}

void FileStorage::Impl::closeFile() {
    nếu (file)
        fclose(file);
#nếu USE_ZLIB
    không_thì nếu (gzfile)
        gzclose(gzfile);
#endif
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = false;
}

void FileStorage::Impl::rewind() {
    nếu (file)
        ::rewind(file);
#nếu USE_ZLIB
    không_thì nếu (gzfile)
        gzrewind(gzfile);
#endif
    strbufpos = 0;
}

char *FileStorage::Impl::resizeWriteBuffer(char *ptr, số_nguyên len) {
    const char *buffer_end = &buffer[0] + buffer.size();
    nếu (ptr + len < buffer_end)
        trả_về ptr;

    const char *buffer_start = &buffer[0];
    số_nguyên written_len = (số_nguyên) (ptr - buffer_start);

    CV_Assert(written_len <= (số_nguyên) buffer.size());
    số_nguyên new_size = (số_nguyên) ((buffer_end - buffer_start) * 3 / 2);
    new_size = MAX(written_len + len, new_size);
    buffer.reserve(new_size + 256);
    buffer.resize(new_size);
    bufofs = written_len;
    trả_về &buffer[0] + bufofs;
}

char *FileStorage::Impl::flush() {
    char *buffer_start = &buffer[0];
    char *ptr = buffer_start + bufofs;

    nếu (ptr > buffer_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(buffer_start);
        bufofs = 0;
    }

    số_nguyên indent = write_stack.back().indent;

    nếu (space != indent) {
        memset(buffer_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = buffer_start + bufofs;

    trả_về ptr;
}

void FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(false);
    nếu (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_stack.empty());

    FStructData &current_struct = write_stack.back();
    nếu (fmt == FileStorage::FORMAT_JSON && !FileNode::isFlow(current_struct.flags) && write_stack.size() > 1)
        current_struct.indent = write_stack[write_stack.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_stack.pop_back();
    nếu (!write_stack.empty())
        write_stack.back().flags &= ~FileNode::EMPTY;
}

void FileStorage::Impl::startWriteStruct_helper(const char *key, số_nguyên struct_flags,
                                                const char *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (FileNode::TYPE_MASK | FileNode::FLOW)) | FileNode::EMPTY;
    nếu (!FileNode::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    nếu (type_name && type_name[0] == '\0')
        type_name = 0;

    FStructData s = getEmitter().startWriteStruct(write_stack.back(), key, struct_flags, type_name);

    write_stack.push_back(s);
    size_t write_stack_size = write_stack.size();
    nếu (write_stack_size > 1)
        write_stack[write_stack_size - 2].flags &= ~FileNode::EMPTY;

    nếu (fmt != FileStorage::FORMAT_JSON && !FileNode::isFlow(s.flags))
        flush();

    nếu (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && FileNode::isMap(struct_flags)) {
        getEmitter().write("type_id", type_name, false);
    }
}

void FileStorage::Impl::startWriteStruct(const char *key, số_nguyên struct_flags,
                                         const char *type_name) {
    check_if_write_struct_is_delayed(false);
    nếu (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    nếu (state_of_writing_base64 == FileStorage_API::Uncertain && FileNode::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(key, struct_flags, type_name);
    } không_thì nếu (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        nếu ((FileNode::TYPE_MASK & struct_flags) != FileNode::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        không_thì nếu (state_of_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(key, struct_flags, "binary");

        nếu (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::InUse);
    } không_thì {
        /* Won't output Base64 data */
        nếu (state_of_writing_base64 == FileStorage_API::InUse)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(key, struct_flags, type_name);

        nếu (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::NotUse);
    }
}

void FileStorage::Impl::writeComment(const char *comment, bool eol_comment) {
    CV_Assert(write_mode);
    getEmitter().writeComment(comment, eol_comment);
}

void FileStorage::Impl::startNextStream() {
    CV_Assert(write_mode);
    nếu (!empty_stream) {
        trong_khi (!write_stack.empty())
            endWriteStruct();
        flush();
        getEmitter().startNextStream();
        empty_stream = true;
        write_stack.push_back(FStructData("", FileNode::EMPTY, 0));
        bufofs = 0;
    }
}

void FileStorage::Impl::write(const String &key, số_nguyên value) {
    CV_Assert(write_mode);
    getEmitter().write(key.c_str(), value);
}

void FileStorage::Impl::write(const String &key, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().write(key.c_str(), value);
}

void FileStorage::Impl::write(const String &key, double value) {
    CV_Assert(write_mode);
    getEmitter().write(key.c_str(), value);
}

void FileStorage::Impl::write(const String &key, const String &value) {
    CV_Assert(write_mode);
    getEmitter().write(key.c_str(), value.c_str(), false);
}

void FileStorage::Impl::writeRawData(const std::string &dt, const void *_data, size_t len) {
    CV_Assert(write_mode);

    nếu (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        writeRawDataBase64(_data, len, dt.c_str());
        trả_về;
    } không_thì nếu (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elemSize = fs::calcStructSize(dt.c_str(), 0);
    CV_Assert(elemSize);
    CV_Assert(len % elemSize == 0);
    len /= elemSize;

    bool explicitZero = fmt == FileStorage::FORMAT_JSON;
    const uchar *data0 = (const uchar *) _data;
    số_nguyên fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_count;
    char buf[256] = "";

    fmt_pair_count = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    nếu (!len)
        trả_về;

    nếu (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    nếu (fmt_pair_count == 1) {
        fmt_pairs[0] *= (số_nguyên) len;
        len = 1;
    }

    cho (; len--; data0 += elemSize) {
        số_nguyên offset = 0;
        cho (k = 0; k < fmt_pair_count; k++) {
            số_nguyên i, count = fmt_pairs[k * 2];
            số_nguyên elem_type = fmt_pairs[k * 2 + 1];
            số_nguyên elem_size = CV_ELEM_SIZE(elem_type);
            const char *ptr;

            offset = cvAlign(offset, elem_size);
            const uchar *data = data0 + offset;

            cho (i = 0; i < count; i++) {
                switch (elem_type) {
                    case CV_8U:
                        ptr = fs::itoa(*(uchar *) data, buf, 10);
                        data++;
                        break;
                    case CV_Bool:
                        ptr = fs::itoa(*(uchar *) data != 0, buf, 10);
                        data++;
                        break;
                    case CV_8S:
                        ptr = fs::itoa(*(char *) data, buf, 10);
                        data++;
                        break;
                    case CV_16U:
                        ptr = fs::itoa(*(ushort *) data, buf, 10);
                        data += sizeof(ushort);
                        break;
                    case CV_16S:
                        ptr = fs::itoa(*(short *) data, buf, 10);
                        data += sizeof(short);
                        break;
                    case CV_32U:
                        ptr = fs::itoa((int64_t)*(unsigned*) data, buf, 10, false);
                        data += sizeof(unsigned);
                        break;
                    case CV_32S:
                        ptr = fs::itoa(*(số_nguyên *) data, buf, 10);
                        data += sizeof(số_nguyên);
                        break;
                    case CV_64U:
                        ptr = fs::itoa(*(uint64_t*) data, buf, 10, false);
                        data += sizeof(uint64_t);
                        break;
                    case CV_64S:
                        ptr = fs::itoa(*(int64_t*) data, buf, 10, true);
                        data += sizeof(int64_t);
                        break;
                    case CV_32F:
                        ptr = fs::floatToString(buf, sizeof(buf), *(float *) data, false, explicitZero);
                        data += sizeof(float);
                        break;
                    case CV_64F:
                        ptr = fs::doubleToString(buf, sizeof(buf), *(double *) data, explicitZero);
                        data += sizeof(double);
                        break;
                    case CV_16F:
                        ptr = fs::floatToString(buf, sizeof(buf), (float) *(hfloat *) data, true, explicitZero);
                        data += sizeof(hfloat);
                        break;
                    case CV_16BF:
                        ptr = fs::floatToString(buf, sizeof(buf), (float) *(bfloat *) data, true, explicitZero);
                        data += sizeof(bfloat);
                        break;
                    case CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, sizeof(buf), (float) *(fp8_t *) data, true, explicitZero);
                        data += sizeof(fp8_t);
                        break;
                    case CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, sizeof(buf), (float) *(fp8a_t *) data, true, explicitZero);
                        data += sizeof(fp8a_t);
                        break;
                    default:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        trả_về;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (số_nguyên) (data - data0);
        }
    }
}

void FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(false);

    nếu (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_to_Base64_state(FileStorage_API::Base64State::Uncertain);
}

void FileStorage::Impl::switch_to_Base64_state(FileStorage_API::Base64State new_state) {
    const char *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    const char *err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    switch (state_of_writing_base64) {
        case FileStorage_API::Base64State::Uncertain:
            switch (new_state) {
                case FileStorage_API::Base64State::InUse:
                {
                    CV_DbgAssert(base64_writer == 0);
                    bool can_indent = (fmt != cv::FileStorage::Mode::FORMAT_JSON);
                    base64_writer = new base64::Base64Writer(*this, can_indent);
                    nếu (!can_indent) {
                        char *ptr = bufferPtr();
                        *ptr++ = '\0';
                        puts(bufferStart());
                        setBufferPtr(bufferStart());
                        memset(bufferStart(), 0, static_cast<số_nguyên>(space));
                        puts("\"$base64$");
                    }
                    break;
                }
                case FileStorage_API::Base64State::Uncertain:
                    break;
                case FileStorage_API::Base64State::NotUse:
                    break;
                default:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    break;
            }
            break;
        case FileStorage_API::Base64State::InUse:
            switch (new_state) {
                case FileStorage_API::Base64State::InUse:
                case FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    break;
                case FileStorage_API::Base64State::Uncertain:
                    delete base64_writer;
                    base64_writer = 0;
                    nếu ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        setBufferPtr(bufferStart());
                        flush();
                        memset(bufferStart(), 0, static_cast<số_nguyên>(space) );
                        setBufferPtr(bufferStart());
                    }
                    break;
                default:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    break;
            }
            break;
        case FileStorage_API::Base64State::NotUse:
            switch (new_state) {
                case FileStorage_API::Base64State::InUse:
                case FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    break;
                case FileStorage_API::Base64State::Uncertain:
                    break;
                default:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    break;
            }
            break;
        default:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            break;
    }

    state_of_writing_base64 = new_state;
}

void FileStorage::Impl::make_write_struct_delayed(const char *key, số_nguyên struct_flags, const char *type_name) {
    CV_Assert(is_write_struct_delayed == false);
    CV_DbgAssert(delayed_struct_key == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    nếu (key != nullptr) {
        delayed_struct_key = new char[strlen(key) + 1U];
        strcpy(delayed_struct_key, key);
    }

    nếu (type_name != nullptr) {
        delayed_type_name = new char[strlen(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = true;
}

void FileStorage::Impl::check_if_write_struct_is_delayed(bool change_type_to_base64) {
    nếu (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_key;
        std::string type_name;
        số_nguyên struct_flags = delayed_struct_flags;

        nếu (delayed_struct_key != nullptr && *delayed_struct_key != '\0') {
            struct_key.assign(delayed_struct_key);
        }
        nếu (delayed_type_name != nullptr && *delayed_type_name != '\0') {
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
        nếu (change_type_to_base64) {
            startWriteStruct_helper(struct_key.c_str(), struct_flags, "binary");
            nếu (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::InUse);
        } không_thì {
            startWriteStruct_helper(struct_key.c_str(), struct_flags, type_name.c_str());
            nếu (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::NotUse);
        }
    }
}

void FileStorage::Impl::writeRawDataBase64(const void *_data, size_t len, const char *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(true);

    nếu (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } không_thì nếu (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->write(_data, len, dt);
}

FileNode FileStorage::Impl::getFirstTopLevelNode() const {
    trả_về roots.empty() ? FileNode() : roots[0];
}

FileNode FileStorage::Impl::root(số_nguyên streamIdx) const {
    trả_về streamIdx >= 0 && streamIdx < (số_nguyên) roots.size() ? roots[streamIdx] : FileNode();
}

FileNode FileStorage::Impl::operator[](const String &nodename) const {
    trả_về this->operator[](nodename.c_str());
}

FileNode FileStorage::Impl::operator[](const char * /*nodename*/) const {
    trả_về FileNode();
}

số_nguyên FileStorage::Impl::getFormat() const { trả_về fmt; }

char *FileStorage::Impl::bufferPtr() const { trả_về (char *) (&buffer[0] + bufofs); }

char *FileStorage::Impl::bufferStart() const { trả_về (char *) &buffer[0]; }

char *FileStorage::Impl::bufferEnd() const { trả_về (char *) (&buffer[0] + buffer.size()); }

void FileStorage::Impl::setBufferPtr(char *ptr) {
    char *bufferstart = bufferStart();
    CV_Assert(ptr >= bufferstart && ptr <= bufferEnd());
    bufofs = ptr - bufferstart;
}

số_nguyên FileStorage::Impl::wrapMargin() const { trả_về wrap_margin; }

FStructData &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_stack.empty());
    trả_về write_stack.back();
}

void FileStorage::Impl::setNonEmpty() {
    empty_stream = false;
}

void FileStorage::Impl::processSpecialDouble(char *buf, double *value, char **endptr) {
    FileStorage_API *fs = this;
    char c = buf[0];
    số_nguyên inf_hi = 0x7ff00000;

    nếu (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    nếu (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    nếu (toupper(buf[1]) == 'I' && toupper(buf[2]) == 'N' && toupper(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    không_thì nếu (toupper(buf[1]) == 'N' && toupper(buf[2]) == 'A' && toupper(buf[3]) == 'N')
        v.u = (uint64) -1;
    không_thì
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

double FileStorage::Impl::strtod(char *ptr, char **endptr) {
    double fval = ::strtod(ptr, endptr);
    nếu (**endptr == '.') {
        char *dot_pos = *endptr;
        *dot_pos = ',';
        double fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        nếu (*endptr > dot_pos)
            fval = fval2;
        không_thì
            *endptr = dot_pos;
    }

    nếu (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    trả_về fval;
}

void FileStorage::Impl::convertToCollection(số_nguyên type, FileNode &node) {
    CV_Assert(type == FileNode::SEQ || type == FileNode::MAP);

    số_nguyên node_type = node.type();
    nếu (node_type == type)
        trả_về;

    bool named = node.isNamed();
    uchar *ptr = node.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    double fval = 0;
    std::string sval;
    bool add_first_scalar = false;

    nếu (node_type != FileNode::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == FileNode::SEQ);
        nếu (node_type == FileNode::INT) {
            ival = readLong(ptr);
            add_first_scalar = true;
        } không_thì nếu (node_type == FileNode::REAL) {
            fval = readReal(ptr);
            add_first_scalar = true;
        } không_thì nếu (node_type == FileNode::STRING) {
            sval = std::string(node);
            add_first_scalar = true;
        } không_thì
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", node_type));
    }

    ptr = reserveNodeSpace(node, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? FileNode::NAMED : 0));
    // name has been copied automatically
    nếu (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    nếu (add_first_scalar)
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

    nếu (!fs_data_ptrs.empty()) {
        size_t blockIdx = node.blockIdx;
        size_t ofs = node.ofs;
        CV_Assert(blockIdx == fs_data_ptrs.size() - 1);
        CV_Assert(ofs <= fs_data_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_data_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_data_ptrs[blockIdx] + ofs;
        blockEnd = fs_data_ptrs[blockIdx] + fs_data_blksz[blockIdx];

        CV_Assert(ptr >= fs_data_ptrs[blockIdx] && ptr <= blockEnd);
        nếu (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            trả_về ptr;
        }

        nếu (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_data[blockIdx]->resize(sz);
            ptr = &fs_data[blockIdx]->at(0);
            fs_data_ptrs[blockIdx] = ptr;
            fs_data_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            trả_về ptr;
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

    nếu (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        nếu (ptr[0] & FileNode::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    nếu (shrinkBlock) {
        fs_data[shrinkBlockIdx]->resize(shrinkSize);
        fs_data_blksz[shrinkBlockIdx] = shrinkSize;
    }

    trả_về new_ptr;
}

unsigned FileStorage::Impl::getStringOfs(const std::string &key) const {
    str_hash_t::const_iterator it = str_hash.find(key);
    trả_về it != str_hash.end() ? it->second : 0;
}

FileNode FileStorage::Impl::addNode(FileNode &collection, const std::string &key,
                                    số_nguyên elem_type, const void *value, số_nguyên len) {
    FileStorage_API *fs = this;
    bool noname = key.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(key.c_str(), "_") == 0);
    convertToCollection(noname ? FileNode::SEQ : FileNode::MAP, collection);

    bool isseq = collection.empty() ? false : collection.isSeq();
    nếu (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    unsigned strofs = 0;
    nếu (!noname) {
        strofs = getStringOfs(key);
        nếu (!strofs) {
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
    nếu (elem_type == FileNode::NONE)
        freeSpaceOfs -= 8;

    nếu (!noname) {
        writeInt(ptr, (số_nguyên) strofs);
        ptr += 4;
    }

    nếu (elem_type == FileNode::SEQ || elem_type == FileNode::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    nếu (value)
        node.setValue(elem_type, value, len);

    nếu (collection.isNamed())
        cp += 4;
    số_nguyên nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    trả_về node;
}

void FileStorage::Impl::finalizeCollection(FileNode &collection) {
    nếu (!collection.isSeq() && !collection.isMap())
        trả_về;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    nếu (*ptr0 & FileNode::NAMED)
        ptr += 4;
    size_t blockIdx = collection.blockIdx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t rawSize = 4;
    unsigned sz = (unsigned) readInt(ptr + 4);
    nếu (sz > 0) {
        size_t lastBlockIdx = fs_data_ptrs.size() - 1;

        cho (; blockIdx < lastBlockIdx; blockIdx++) {
            rawSize += fs_data_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    rawSize += freeSpaceOfs - ofs;
    writeInt(ptr, (số_nguyên) rawSize);
}

void FileStorage::Impl::normalizeNodeOfs(size_t &blockIdx, size_t &ofs) const {
    trong_khi (ofs >= fs_data_blksz[blockIdx]) {
        nếu (blockIdx == fs_data_blksz.size() - 1) {
            CV_Assert(ofs == fs_data_blksz[blockIdx]);
            break;
        }
        ofs -= fs_data_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    trả_về state_of_writing_base64;
}

số_nguyên FileStorage::Impl::get_space() {
    trả_về space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = true;
}

void FileStorage::Impl::Base64Decoder::init(const Ptr<FileStorageParser> &_parser, char *_ptr, số_nguyên _indent) {
    parser_do_not_use_direct_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = false;
}

bool FileStorage::Impl::Base64Decoder::readMore(số_nguyên needed) {
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

    nếu (eos)
        trả_về false;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    cho (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    char *beg = 0, *end = 0;
    bool ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    nếu (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = true;
        size_t tc = totalchars;
        cho (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    số_nguyên i = 0, j, n = (số_nguyên) encoded.size();
    nếu (n > 0) {
        const uchar *tab = base64tab;
        char *src = &encoded[0];

        cho (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(số_nguyên) (uchar) src[i]], c = tab[(số_nguyên) (uchar) src[i + 1]];
            uchar b = tab[(số_nguyên) (uchar) src[i + 2]], a = tab[(số_nguyên) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    nếu (i > 0 && encoded[i - 1] == '=') {
        nếu (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        nếu (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    cho (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    trả_về (số_nguyên) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    nếu (ofs >= sz && !readMore(1))
        trả_về (uchar) 0;
    trả_về decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    nếu (ofs + 2 > sz && !readMore(2))
        trả_về (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    trả_về val;
}

số_nguyên FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    nếu (ofs + 4 > sz && !readMore(4))
        trả_về 0;
    số_nguyên ival = readInt(&decoded[ofs]);
    ofs += 4;
    trả_về ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    nếu (ofs + 8 > sz && !readMore(8))
        trả_về 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    trả_về ival;
}

double FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    nếu (ofs + 8 > sz && !readMore(8))
        trả_về 0;
    double fval = readReal(&decoded[ofs]);
    ofs += 8;
    trả_về fval;
}

bool FileStorage::Impl::Base64Decoder::endOfStream() const { trả_về eos; }

char *FileStorage::Impl::Base64Decoder::getPtr() const { trả_về ptr; }


char *FileStorage::Impl::parseBase64(char *ptr, số_nguyên indent, FileNode &collection) {
    const số_nguyên BASE64_HDR_SIZE = 24;
    char dt[BASE64_HDR_SIZE + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    số_nguyên i, k;

    cho (i = 0; i < BASE64_HDR_SIZE; i++)
        dt[i] = (char) base64decoder.getUInt8();
    cho (i = 0; i < BASE64_HDR_SIZE; i++)
        nếu (isspace(dt[i]))
            break;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    số_nguyên fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    số_nguyên fmt_pair_count = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    double fval = 0;

    cho (;;) {
        cho (k = 0; k < fmt_pair_count; k++) {
            số_nguyên elem_type = fmt_pairs[k * 2 + 1];
            số_nguyên count = fmt_pairs[k * 2];

            cho (i = 0; i < count; i++) {
                số_nguyên node_type = FileNode::INT;
                switch (elem_type) {
                    case CV_8U:
                        ival = base64decoder.getUInt8();
                        break;
                    case CV_8S:
                        ival = (char) base64decoder.getUInt8();
                        break;
                    case CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        break;
                    case CV_16U:
                        ival = base64decoder.getUInt16();
                        break;
                    case CV_16S:
                        ival = (short) base64decoder.getUInt16();
                        break;
                    case CV_32U:
                        ival = base64decoder.getInt32();
                        break;
                    case CV_32S:
                        ival = base64decoder.getInt32();
                        break;
                    case CV_64U:
                        ival = base64decoder.getInt64();
                        break;
                    case CV_64S:
                        ival = base64decoder.getInt64();
                        break;
                    case CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        node_type = FileNode::REAL;
                    }
                        break;
                    case CV_64F:
                        fval = base64decoder.getFloat64();
                        node_type = FileNode::REAL;
                        break;
                    case CV_16F:
                        fval = float(hfloatFromBits(base64decoder.getUInt16()));
                        node_type = FileNode::REAL;
                        break;
                    case CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        node_type = FileNode::REAL;
                        break;
                    case CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        node_type = FileNode::REAL;
                        break;
                    default:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                nếu (base64decoder.endOfStream())
                    break;
                addNode(collection, std::string(), node_type,
                        node_type == FileNode::INT ? (void *) &ival : (void *) &fval, -1);
            }
        }
        nếu (base64decoder.endOfStream())
            break;
    }

    finalizeCollection(collection);
    trả_về base64decoder.getPtr();
}

void FileStorage::Impl::parseError(const char *func_name, const std::string &err_msg, const char *source_file,
                                   số_nguyên source_line) {
    std::string msg = format("%s(%d): %s", filename.c_str(), lineno, err_msg.c_str());
    error(Error::StsParseError, func_name, msg.c_str(), source_file, source_line);
}

const uchar *FileStorage::Impl::getNodePtr(size_t blockIdx, size_t ofs) const {
    CV_Assert(blockIdx < fs_data_ptrs.size());
    CV_Assert(ofs < fs_data_blksz[blockIdx]);

    trả_về fs_data_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) const {
    CV_Assert(nameofs < str_hash_data.size());
    trả_về std::string(&str_hash_data[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { trả_về fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(this);
}

FileStorage::FileStorage(const String& filename, số_nguyên flags, const String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(this);
    bool ok = p->open(filename.c_str(), flags, encoding.c_str());
    nếu(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

void FileStorage::startWriteStruct(const String& name, số_nguyên struct_flags, const String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    nếu ((struct_flags & FileNode::TYPE_MASK) == FileNode::SEQ)
        state = FileStorage::VALUE_EXPECTED;
    không_thì
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

bool FileStorage::open(const String& filename, số_nguyên flags, const String& encoding)
{
    try
    {
        bool ok = p->open(filename.c_str(), flags, encoding.c_str());
        nếu(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        trả_về ok;
    }
    catch (...)
    {
        release();
        throw;  // re-throw
    }
}

bool FileStorage::isOpened() const { trả_về p->is_opened; }

void FileStorage::release()
{
    p->release();
}

FileNode FileStorage::root(số_nguyên i) const
{
    nếu( p.empty() || p->roots.empty() || i < 0 || i >= (số_nguyên)p->roots.size() )
        trả_về FileNode();

    trả_về p->roots[i];
}

FileNode FileStorage::getFirstTopLevelNode() const
{
    FileNode r = root();
    FileNodeIterator it = r.begin();
    trả_về it != r.end() ? *it : FileNode();
}

std::string FileStorage::getDefaultObjectName(const std::string& _filename)
{
    static const char* stubname = "unnamed";
    const char* filename = _filename.c_str();
    const char* ptr2 = filename + _filename.size();
    const char* ptr = ptr2 - 1;
    cv::AutoBuffer<char> name_buf(_filename.size()+1);

    trong_khi( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        nếu( *ptr == '.' && (!*ptr2 || strncmp(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    nếu( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    char* name = name_buf.data();

    // name must start with letter or '_'
    nếu( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    trong_khi( ptr < ptr2 )
    {
        char c = *ptr++;
        nếu( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.data();
    nếu( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    trả_về name;
}


số_nguyên FileStorage::getFormat() const
{
    trả_về p->fmt;
}

FileNode FileStorage::operator [](const char* key) const
{
    trả_về this->operator[](std::string(key));
}

FileNode FileStorage::operator [](const std::string& key) const
{
    FileNode res;
    cho (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][key];
        nếu (!res.empty())
            break;
    }
    trả_về res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    trả_về buf;
}

void FileStorage::writeRaw( const String& fmt, const void* vec, size_t len )
{
    p->writeRawData(fmt, (const uchar*)vec, len);
}

void FileStorage::writeComment( const String& comment, bool eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

void writeScalar( FileStorage& fs, số_nguyên value )
{
    fs.p->write(String(), value);
}

void writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->write(String(), value);
}

void writeScalar( FileStorage& fs, float value )
{
    fs.p->write(String(), (double)value);
}

void writeScalar( FileStorage& fs, double value )
{
    fs.p->write(String(), value);
}

void writeScalar( FileStorage& fs, const String& value )
{
    fs.p->write(String(), value);
}

void write( FileStorage& fs, const String& name, số_nguyên value )
{
    fs.p->write(name, value);
}

void write( FileStorage& fs, const String& name, int64_t value )
{
    fs.p->write(name, value);
}

void write( FileStorage& fs, const String& name, float value )
{
    fs.p->write(name, (double)value);
}

void write( FileStorage& fs, const String& name, double value )
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

void FileStorage::write(const String& name, số_nguyên val) { p->write(name, val); }
void FileStorage::write(const String& name, bool val) { p->write(name, val); }
void FileStorage::write(const String& name, int64_t val) { p->write(name, val); }
void FileStorage::write(const String& name, double val) { p->write(name, val); }
void FileStorage::write(const String& name, const String& val) { p->write(name, val); }
void FileStorage::write(const String& name, const Mat& val) { cv::write(*this, name, val); }
void FileStorage::write(const String& name, const std::vector<String>& val) { cv::write(*this, name, val); }

FileStorage& operator << (FileStorage& fs, const String& str)
{
    enum { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        VALUE_EXPECTED = FileStorage::VALUE_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    const char* _str = str.c_str();
    nếu( !fs.isOpened() || !_str )
        trả_về fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    char c = *_str;

    nếu( c == '}' || c == ']' )
    {
        nếu( fs_impl->write_stack.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        số_nguyên struct_flags = fs_impl->write_stack.back().flags;
        char expected_bracket = FileNode::isMap(struct_flags) ? '}' : ']';
        nếu( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_stack.empty());
        struct_flags = fs_impl->write_stack.back().flags;
        fs.state = FileNode::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : VALUE_EXPECTED;
        fs.elname = String();
    }
    không_thì nếu( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        nếu (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = VALUE_EXPECTED + INSIDE_MAP;
    }
    không_thì nếu( (fs.state & 3) == VALUE_EXPECTED )
    {
        nếu( c == '{' || c == '[' )
        {
            số_nguyên struct_flags = c == '{' ? FileNode::MAP : FileNode::SEQ;
            fs.state = struct_flags == FileNode::MAP ? INSIDE_MAP + NAME_EXPECTED : VALUE_EXPECTED;
            _str++;
            nếu( *_str == ':' )
            {
                _str++;
                nếu( !*_str )
                    struct_flags |= FileNode::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        không_thì
        {
            write( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            nếu( fs.state == INSIDE_MAP + VALUE_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    không_thì
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    trả_về fs;
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
    trả_về *this;
}

FileNode FileNode::operator[](const std::string& nodename) const
{
    nếu(!fs)
        trả_về FileNode();

    CV_Assert( isMap() );

    unsigned key = fs->getStringOfs(nodename);
    size_t i, sz = size();
    FileNodeIterator it = begin();

    cho( i = 0; i < sz; i++, ++it )
    {
        FileNode n = *it;
        const uchar* p = n.ptr();
        unsigned key2 = (unsigned)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_data.size() );
        nếu( key == key2 )
            trả_về n;
    }
    trả_về FileNode();
}

FileNode FileNode::operator[](const char* nodename) const
{
    trả_về this->operator[](std::string(nodename));
}

FileNode FileNode::operator[](số_nguyên i) const
{
    nếu(!fs)
        trả_về FileNode();

    CV_Assert( isSeq() );

    số_nguyên sz = (số_nguyên)size();
    CV_Assert( 0 <= i && i < sz );

    FileNodeIterator it = begin();
    it += i;

    trả_về *it;
}

std::vector<String> FileNode::keys() const
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    cho (FileNodeIterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    trả_về res;
}

số_nguyên FileNode::type() const
{
    const uchar* p = ptr();
    nếu(!p)
        trả_về NONE;
    trả_về (*p & TYPE_MASK);
}

bool FileNode::isMap(số_nguyên flags) { trả_về (flags & TYPE_MASK) == MAP; }
bool FileNode::isSeq(số_nguyên flags) { trả_về (flags & TYPE_MASK) == SEQ; }
bool FileNode::isCollection(số_nguyên flags) { trả_về isMap(flags) || isSeq(flags); }
bool FileNode::isFlow(số_nguyên flags) { trả_về (flags & FLOW) != 0; }
bool FileNode::isEmptyCollection(số_nguyên flags) { trả_về (flags & EMPTY) != 0; }

bool FileNode::empty() const   { trả_về fs == 0; }
bool FileNode::isNone() const  { trả_về type() == NONE; }
bool FileNode::isSeq() const   { trả_về type() == SEQ; }
bool FileNode::isMap() const   { trả_về type() == MAP; }
bool FileNode::isInt() const   { trả_về type() == INT;  }
bool FileNode::isReal() const  { trả_về type() == REAL; }
bool FileNode::isString() const { trả_về type() == STRING;  }
bool FileNode::isNamed() const
{
    const uchar* p = ptr();
    nếu(!p)
        trả_về false;
    trả_về (*p & NAMED) != 0;
}

std::string FileNode::name() const
{
    const uchar* p = ptr();
    nếu(!p)
        trả_về std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    trả_về fs->getName(nameofs);
}

FileNode::operator số_nguyên() const
{
    const uchar* p = ptr();
    nếu(!p)
        trả_về 0;
    số_nguyên tag = *p;
    số_nguyên type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    nếu( type == INT )
    {
        trả_về readInt(p);
    }
    không_thì nếu( type == REAL )
    {
        trả_về cvRound(readReal(p));
    }
    không_thì
        trả_về 0x7fffffff;
}

FileNode::operator int64_t() const
{
    const uchar* p = ptr();
    nếu(!p)
        trả_về 0;
    số_nguyên tag = *p;
    số_nguyên type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    nếu( type == INT )
    {
        trả_về readLong(p);
    }
    không_thì nếu( type == REAL )
    {
        trả_về cvRound(readReal(p));
    }
    không_thì
        trả_về 0x7fffffff;
}

FileNode::operator float() const
{
    const uchar* p = ptr();
    nếu(!p)
        trả_về 0.f;
    số_nguyên tag = *p;
    số_nguyên type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    nếu( type == INT )
    {
        trả_về (float)readLong(p);
    }
    không_thì nếu( type == REAL )
    {
        trả_về (float)readReal(p);
    }
    không_thì
        trả_về FLT_MAX;
}

FileNode::operator double() const
{
    const uchar* p = ptr();
    nếu(!p)
        trả_về 0.f;
    số_nguyên tag = *p;
    số_nguyên type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    nếu( type == INT )
    {
        trả_về (double)readLong(p);
    }
    không_thì nếu( type == REAL )
    {
        trả_về readReal(p);
    }
    không_thì
        trả_về DBL_MAX;
}

double FileNode::real() const  { trả_về double(*this); }

std::string FileNode::string() const
{
    const uchar* p = ptr();
    nếu( !p || (*p & TYPE_MASK) != STRING )
        trả_về std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(unsigned)readInt(p);
    trả_về std::string((const char*)(p + 4), sz - 1);
}
Mat FileNode::mat() const { Mat value; read(*this, value, Mat()); trả_về value; }

FileNodeIterator FileNode::begin() const { trả_về FileNodeIterator(*this, false); }
FileNodeIterator FileNode::end() const   { trả_về FileNodeIterator(*this, true); }

void FileNode::readRaw( const std::string& fmt, void* vec, size_t len ) const
{
    FileNodeIterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t FileNode::size() const
{
    const uchar* p = ptr();
    nếu( !p )
        trả_về 0;
    số_nguyên tag = *p;
    số_nguyên tp = tag & TYPE_MASK;
    nếu( tp == MAP || tp == SEQ )
    {
        nếu( tag & NAMED )
            p += 4;
        trả_về (size_t)(unsigned)readInt(p + 5);
    }
    trả_về tp != NONE;
}

size_t FileNode::rawSize() const
{
    const uchar* p0 = ptr(), *p = p0;
    nếu( !p )
        trả_về 0;
    số_nguyên tag = *p++;
    số_nguyên tp = tag & TYPE_MASK;
    nếu( tag & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    nếu( tp == INT )
        trả_về sz0 + 8;
    nếu( tp == REAL )
        trả_về sz0 + 8;
    nếu( tp == NONE )
        trả_về sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    trả_về sz0 + 4 + readInt(p);
}

uchar* FileNode::ptr()
{
    trả_về !fs ? 0 : (uchar*)fs->getNodePtr(blockIdx, ofs);
}

const uchar* FileNode::ptr() const
{
    trả_về !fs ? 0 : fs->getNodePtr(blockIdx, ofs);
}

void FileNode::setValue( số_nguyên type, const void* value, số_nguyên len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    số_nguyên tag = *p;
    số_nguyên current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    số_nguyên sz = 1;

    nếu( tag & NAMED )
        sz += 4;

    nếu( type == INT )
        sz += 8;
    không_thì nếu( type == REAL )
        sz += 8;
    không_thì nếu( type == STRING )
    {
        nếu( len < 0 )
            len = (số_nguyên)strlen((const char*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    không_thì
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserveNodeSpace(*this, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    nếu( tag & NAMED )
        p += 4;

    nếu( type == INT )
    {
        int64_t ival = *(const int64_t*)value;
        writeInt(p, ival);
    }
    không_thì nếu( type == REAL )
    {
        double dbval = *(const double*)value;
        writeReal(p, dbval);
    }
    không_thì nếu( type == STRING )
    {
        const char* str = (const char*)value;
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
    nếu( !fs )
        blockIdx = ofs = blockSize = nodeNElems = 0;
    không_thì
    {
        blockIdx = node.blockIdx;
        ofs = node.ofs;

        bool collection = node.isSeq() || node.isMap();
        nếu( node.isNone() )
        {
            nodeNElems = 0;
        }
        không_thì nếu( !collection )
        {
            nodeNElems = 1;
            nếu( seekEnd )
            {
                idx = 1;
                ofs += node.rawSize();
            }
        }
        không_thì
        {
            nodeNElems = node.size();
            const uchar* p0 = node.ptr(), *p = p0 + 1;
            nếu(*p0 & FileNode::NAMED )
                p += 4;
            nếu( !seekEnd )
                ofs += (p - p0) + 8;
            không_thì
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
    trả_về *this;
}

FileNode FileNodeIterator::operator *() const
{
    trả_về FileNode(idx < nodeNElems ? fs : NULL, blockIdx, ofs);
}

FileNodeIterator& FileNodeIterator::operator ++ ()
{
    nếu( idx == nodeNElems || !fs )
        trả_về *this;
    idx++;
    FileNode n(fs, blockIdx, ofs);
    ofs += n.rawSize();
    nếu( ofs >= blockSize )
    {
        fs->normalizeNodeOfs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
    trả_về *this;
}

FileNodeIterator FileNodeIterator::operator ++ (số_nguyên)
{
    FileNodeIterator it = *this;
    ++(*this);
    trả_về it;
}

FileNodeIterator& FileNodeIterator::operator += (số_nguyên _ofs)
{
    CV_Assert( _ofs >= 0 );
    cho( ; _ofs > 0; _ofs-- )
        this->operator ++();
    trả_về *this;
}

FileNodeIterator& FileNodeIterator::readRaw( const String& fmt, void* _data0, size_t maxsz)
{
    nếu( fs && idx < nodeNElems )
    {
        uchar* data0 = (uchar*)_data0;
        số_nguyên fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        số_nguyên fmt_pair_count = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        size_t esz = fs::calcStructSize( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        cho( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            cho( số_nguyên k = 0; k < fmt_pair_count; k++ )
            {
                số_nguyên elem_type = fmt_pairs[k*2+1];
                số_nguyên elem_size = CV_ELEM_SIZE(elem_type);

                số_nguyên count = fmt_pairs[k*2];
                offset = alignSize( offset, elem_size );
                uchar* data = data0 + offset;

                cho( số_nguyên i = 0; i < count; i++, ++(*this) )
                {
                    FileNode node = *(*this);
                    nếu( node.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_size == 8 ? (int64_t)node : (số_nguyên)node);
                        switch( elem_type )
                        {
                        case CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(ival);
                            data++;
                            break;
                        case CV_8S:
                            *(char*)data = saturate_cast<schar>(ival);
                            data++;
                            break;
                        case CV_Bool:
                            *(bool*)data = ival != 0;
                            data++;
                            break;
                        case CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(ival);
                            data += sizeof(ushort);
                            break;
                        case CV_16S:
                            *(short*)data = saturate_cast<short>(ival);
                            data += sizeof(short);
                            break;
                        case CV_32U:
                            *(unsigned*)data = (unsigned)std::max(ival, (int64_t)0);
                            data += sizeof(unsigned);
                            break;
                        case CV_32S:
                            *(số_nguyên*)data = (số_nguyên)ival;
                            data += sizeof(số_nguyên);
                            break;
                        case CV_32F:
                            *(float*)data = (float)ival;
                            data += sizeof(float);
                            break;
                        case CV_64U:
                            *(uint64_t*)data = (uint64_t)ival;
                            data += sizeof(uint64_t);
                            break;
                        case CV_64S:
                            *(int64_t*)data = (int64_t)ival;
                            data += sizeof(int64_t);
                            break;
                        case CV_64F:
                            *(double*)data = (double)ival;
                            data += sizeof(double);
                            break;
                        case CV_16F:
                            *(hfloat*)data = hfloat((float)ival);
                            data += sizeof(hfloat);
                            break;
                        case CV_16BF:
                            *(bfloat*)data = bfloat((float)ival);
                            data += sizeof(bfloat);
                            break;
                        case CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((float)ival);
                            data += sizeof(fp8_t);
                            break;
                        case CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((float)ival);
                            data += sizeof(fp8a_t);
                            break;
                        default:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    không_thì nếu( node.isReal() )
                    {
                        double fval = (double)node;

                        switch( elem_type )
                        {
                        case CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(fval);
                            data++;
                            break;
                        case CV_8S:
                            *(char*)data = saturate_cast<schar>(fval);
                            data++;
                            break;
                        case CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(fval);
                            data += sizeof(ushort);
                            break;
                        case CV_16S:
                            *(short*)data = saturate_cast<short>(fval);
                            data += sizeof(short);
                            break;
                        case CV_32U:
                            *(số_nguyên*)data = saturate_cast<unsigned>(fval);
                            data += sizeof(số_nguyên);
                            break;
                        case CV_32S:
                            *(số_nguyên*)data = saturate_cast<số_nguyên>(fval);
                            data += sizeof(số_nguyên);
                            break;
                        case CV_32F:
                            *(float*)data = (float)fval;
                            data += sizeof(float);
                            break;
                        case CV_64U:
                            *(uint64_t*)data = (uint64_t)round(std::max(fval, 0.));
                            data += sizeof(uint64_t);
                            break;
                        case CV_64S:
                            *(int64_t*)data = (int64_t)round(std::max(fval, 0.));
                            data += sizeof(int64_t);
                            break;
                        case CV_64F:
                            *(double*)data = fval;
                            data += sizeof(double);
                            break;
                        case CV_16F:
                            *(hfloat*)data = hfloat((float)fval);
                            data += sizeof(hfloat);
                            break;
                        case CV_16BF:
                            *(bfloat*)data = bfloat((float)fval);
                            data += sizeof(bfloat);
                            break;
                        case CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((float)fval);
                            data += sizeof(fp8_t);
                            break;
                        case CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((float)fval);
                            data += sizeof(fp8a_t);
                            break;
                        default:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    không_thì
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (số_nguyên)(data - data0);
            }
        }
    }

    trả_về *this;
}

bool FileNodeIterator::equalTo(const FileNodeIterator& it) const
{
    trả_về fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && nodeNElems == it.nodeNElems;
}

size_t FileNodeIterator::remaining() const
{
    trả_về nodeNElems - idx;
}

bool operator == ( const FileNodeIterator& it1, const FileNodeIterator& it2 )
{
    trả_về it1.equalTo(it2);
}

bool operator != ( const FileNodeIterator& it1, const FileNodeIterator& it2 )
{
    trả_về !it1.equalTo(it2);
}

void read(const FileNode& node, số_nguyên& val, số_nguyên default_val)
{
    val = default_val;
    nếu( !node.empty() )
    {
        val = (số_nguyên)node;
    }
}

void read(const FileNode& node, int64_t& val, int64_t default_val)
{
    val = default_val;
    nếu( !node.empty() )
    {
        val = (int64_t)node;
    }
}

void read(const FileNode& node, double& val, double default_val)
{
    val = default_val;
    nếu( !node.empty() )
    {
        val = (double)node;
    }
}

void read(const FileNode& node, float& val, float default_val)
{
    val = default_val;
    nếu( !node.empty() )
    {
        val = (float)node;
    }
}

void read(const FileNode& node, std::string& val, const std::string& default_val)
{
    val = default_val;
    nếu( !node.empty() )
    {
        val = (std::string)node;
    }
}
void FileStorage::Impl::write(const String &key, bool value)
{
    CV_Assert(write_mode);
    nếu (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().write(key.c_str(), (số_nguyên)value);
    }
    không_thì
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().write(key.c_str(), value ? "true" : "false", false);
    }
}

FileStorage_API::~FileStorage_API() {}

namespace internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, const std::string& name,
                                       số_nguyên flags, const std::string& typeName)
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
