// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "persistence.hpp"
#અન્તર્ભાવ "persistence_impl.hpp"
#અન્તર્ભાવ "persistence_base64_encoding.hpp"
#અન્તર્ભાવ <unordered_map>
#અન્તર્ભાવ <iterator>

#અન્તર્ભાવ <opencv2/core/utils/logger.hpp>

#define CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

નામાકાશ cv
{

નામાકાશ fs
{

પૂર્ણાંક strcasecmp(અચલ અક્ષર* s1, અચલ અક્ષર* s2)
{
    અચલ અક્ષર* dummy="";
    જો(!s1) s1=dummy;
    જો(!s2) s2=dummy;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t i, len = std::min(len1, len2);
    માટે( i = 0; i < len; i++ )
    {
        પૂર્ણાંક d = tolower((પૂર્ણાંક)s1[i]) - tolower((પૂર્ણાંક)s2[i]);
        જો( d != 0 )
            પાછા d;
    }
    પાછા len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

અક્ષર* itoa( પૂર્ણાંક _val, અક્ષર* buffer, પૂર્ણાંક /*radix*/ )
{
    અચલ પૂર્ણાંક radix = 10;
    અક્ષર* ptr=buffer + 23 /* enough even for 64-bit integers */;
    unsigned val = abs(_val);

    *ptr = '\0';
    કરો
    {
        unsigned r = val / radix;
        *--ptr = (અક્ષર)(val - (r*radix) + '0');
        val = r;
    }
    જ્યારે( val != 0 );

    જો( _val < 0 )
        *--ptr = '-';

    પાછા ptr;
}

અક્ષર* itoa( int64_t _val, અક્ષર* buffer, પૂર્ણાંક /*radix*/, તાર્કિક _signed)
{
    અચલ પૂર્ણાંક radix = 10;
    અક્ષર* ptr=buffer + 23 /* enough even for 64-bit integers */;
    પૂર્ણાંક sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : abs(_val);

    *ptr = '\0';
    કરો
    {
        uint64_t r = val / radix;
        *--ptr = (અક્ષર)(val - (r*radix) + '0');
        val = r;
    }
    જ્યારે( val != 0 );

    જો( sign < 0 )
        *--ptr = '-';

    પાછા ptr;
}

અક્ષર* doubleToString( અક્ષર* buf, size_t bufSize, દ્વિગુણ value, તાર્કિક explicitZero )
{
    Cv64suf val;
    unsigned ieee754_hi;

    val.f = value;
    ieee754_hi = (unsigned)(val.u >> 32);

    જો( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        પૂર્ણાંક ivalue = cvRound(value);
        જો( ivalue == value )
        {
            જો( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            નહીંતર
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        નહીંતર
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, bufSize, "%.17g", value );

            અક્ષર* ptr = buf;
            જો( *ptr == '+' || *ptr == '-' )
                ptr++;
            માટે( ; cv_isdigit(*ptr); ptr++ )
                ;
            જો( *ptr == ',' )
                *ptr = '.';
        }
    }
    નહીંતર
    {
        unsigned ieee754_lo = (unsigned)val.u;
        જો( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        નહીંતર
            strcpy( buf, (પૂર્ણાંક)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    પાછા buf;
}

અક્ષર* floatToString( અક્ષર* buf, size_t bufSize, દશાંશ value, તાર્કિક halfprecision, તાર્કિક explicitZero )
{
    Cv32suf val;
    unsigned ieee754;
    val.f = value;
    ieee754 = val.u;

    જો( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        પૂર્ણાંક ivalue = cvRound(value);
        જો( ivalue == value )
        {
            જો( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            નહીંતર
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        નહીંતર
        {
            જો (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.5g", value);
            }
            નહીંતર
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.9g", value);
            }

            અક્ષર* ptr = buf;
            જો( *ptr == '+' || *ptr == '-' )
                ptr++;
            માટે( ; cv_isdigit(*ptr); ptr++ )
                ;
            જો( *ptr == ',' )
                *ptr = '.';
        }
    }
    નહીંતર
    {
        જો( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        નહીંતર
            strcpy( buf, (પૂર્ણાંક)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    પાછા buf;
}

સ્થિર અચલ અક્ષર symbols[] = "ucwsifdhHbUIneE";

સ્થિર અક્ષર type_પ્રતીક(પૂર્ણાંક depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    પાછા symbols[depth];
}

સ્થિર પૂર્ણાંક પ્રતીક_To_Type(અક્ષર c)
{
    જો (c == 'r')
        પાછા CV_SEQ_ELTYPE_PTR;
    અચલ અક્ષર* pos = strchr( symbols, c );
    જો( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    પાછા static_cast<પૂર્ણાંક>(pos - symbols);
}

અક્ષર* encodeFormat(પૂર્ણાંક elem_type, અક્ષર* dt, size_t dt_len)
{
    પૂર્ણાંક cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    અક્ષર પ્રતીક = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_પ્રતીક(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, પ્રતીક);
    પાછા dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
અક્ષર* encodeFormat(પૂર્ણાંક elem_type, અક્ષર* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    પાછા encodeFormat(elem_type, dt, max);
}

પૂર્ણાંક decodeFormat( અચલ અક્ષર* dt, પૂર્ણાંક* fmt_pairs, પૂર્ણાંક max_len )
{
    પૂર્ણાંક fmt_pair_ગણતરી = 0;
    પૂર્ણાંક i = 0, k = 0, len = dt ? (પૂર્ણાંક)strlen(dt) : 0;

    જો( !dt || !len )
        પાછા 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    માટે( ; k < len; k++ )
    {
        અક્ષર c = dt[k];

        જો( cv_isdigit(c) )
        {
            પૂર્ણાંક ગણતરી_2 = c - '0';
            જો( cv_isdigit(dt[k+1]) )
            {
                અક્ષર* endptr = 0;
                ગણતરી_2 = (પૂર્ણાંક)strtol( dt+k, &endptr, 10 );
                k = (પૂર્ણાંક)(endptr - dt) - 1;
            }

            જો( ગણતરી_2 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = ગણતરી_2;
        }
        નહીંતર
        {
            પૂર્ણાંક depth = પ્રતીક_To_Type(c);
            જો( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = depth;
            જો( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            નહીંતર
            {
                i += 2;
                જો( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_ગણતરી = i/2;
    પાછા fmt_pair_ગણતરી;
}

પૂર્ણાંક calcElemSize( અચલ અક્ષર* dt, પૂર્ણાંક initial_size )
{
    પૂર્ણાંક size = 0;
    પૂર્ણાંક fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_ગણતરી;
    પૂર્ણાંક comp_size;

    fmt_pair_ગણતરી = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_ગણતરી *= 2;
    માટે( i = 0, size = initial_size; i < fmt_pair_ગણતરી; i += 2 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[i+1]);
        size = cvAlign( size, comp_size );
        size += comp_size * fmt_pairs[i];
    }
    જો( initial_size == 0 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[1]);
        size = cvAlign( size, comp_size );
    }
    પાછા size;
}


પૂર્ણાંક calcStructSize( અચલ અક્ષર* dt, પૂર્ણાંક initial_size )
{
    પૂર્ણાંક size = calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    માટે ( અચલ અક્ષર * type = dt; *type != '\0'; type++ )
    {
        અક્ષર v = *type;
        જો (v >= '0' && v <= '9')
            ચાલુ;  // skip vector size
        પસંદગી (v)
        {
        સ્થિતિ 'u': { elem_max_size = std::max( elem_max_size, કદ(uchar ) ); તોડો; }
        સ્થિતિ 'b': { elem_max_size = std::max( elem_max_size, કદ(તાર્કિક  ) ); તોડો; }
        સ્થિતિ 'c': { elem_max_size = std::max( elem_max_size, કદ(schar ) ); તોડો; }
        સ્થિતિ 'w': { elem_max_size = std::max( elem_max_size, કદ(ushort) ); તોડો; }
        સ્થિતિ 's': { elem_max_size = std::max( elem_max_size, કદ(short ) ); તોડો; }
        સ્થિતિ 'i': { elem_max_size = std::max( elem_max_size, કદ(પૂર્ણાંક   ) ); તોડો; }
        સ્થિતિ 'n': { elem_max_size = std::max( elem_max_size, કદ(unsigned) ); તોડો; }
        સ્થિતિ 'f': { elem_max_size = std::max( elem_max_size, કદ(દશાંશ ) ); તોડો; }
        સ્થિતિ 'd': { elem_max_size = std::max( elem_max_size, કદ(દ્વિગુણ) ); તોડો; }
        સ્થિતિ 'h': { elem_max_size = std::max( elem_max_size, કદ(hfloat)); તોડો; }
        સ્થિતિ 'H': { elem_max_size = std::max( elem_max_size, કદ(bfloat)); તોડો; }
        સ્થિતિ 'e': { elem_max_size = std::max( elem_max_size, કદ(fp8_t) ); તોડો; }
        સ્થિતિ 'E': { elem_max_size = std::max( elem_max_size, કદ(fp8a_t)); તોડો; }
        સ્થિતિ 'I': { elem_max_size = std::max( elem_max_size, કદ(int64_t)); તોડો; }
        સ્થિતિ 'U': { elem_max_size = std::max( elem_max_size, કદ(uint64_t)); તોડો; }
        મૂળભૂત:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (અક્ષર)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<પૂર્ણાંક>(elem_max_size) );
    પાછા size;
}

પૂર્ણાંક decodeSimpleFormat( અચલ અક્ષર* dt )
{
    પૂર્ણાંક elem_type = -1;
    પૂર્ણાંક fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_ગણતરી;

    fmt_pair_ગણતરી = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    જો( fmt_pair_ગણતરી != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    પાછા elem_type;
}

}

#જો defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#define CV_LITTLE_ENDIAN_MEM_ACCESS 1
#નહીંતર
#define CV_LITTLE_ENDIAN_MEM_ACCESS 0
#endif

સ્થિર અંતઃરેખ પૂર્ણાંક readInt(અચલ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#જો CV_LITTLE_ENDIAN_MEM_ACCESS
    પૂર્ણાંક val;
    memcpy(&val, p, કદ(val));
    પાછા val;
#નહીંતર
    પૂર્ણાંક val = (પૂર્ણાંક)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    પાછા val;
#endif
}

સ્થિર અંતઃરેખ int64_t readLong(અચલ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#જો CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    memcpy(&val, p, કદ(val));
    પાછા val;
#નહીંતર
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    પાછા val0 | ((int64_t)val1 << 32);
#endif
}

સ્થિર અંતઃરેખ દ્વિગુણ readReal(અચલ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#જો CV_LITTLE_ENDIAN_MEM_ACCESS
    દ્વિગુણ val;
    memcpy(&val, p, કદ(val));
    પાછા val;
#નહીંતર
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    પાછા val.f;
#endif
}

template <typename T>
સ્થિર અંતઃરેખ શૂન્ય writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#જો CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &ival, કદ(ival));
#નહીંતર
    માટે (size_t i = 0, j = 0; i < કદ(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#endif
}

સ્થિર અંતઃરેખ શૂન્ય writeReal(uchar* p, દ્વિગુણ fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#જો CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &fval, કદ(fval));
#નહીંતર
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



શૂન્ય FileStorage::Impl::init() {
    flags = 0;
    buffer.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = ખોટું;
    state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = ખોટું;
    delayed_struct_ચાવી = nullptr;
    delayed_struct_flags = 0;
    delayed_type_name = nullptr;
    base64_writer = nullptr;
    is_opened = ખોટું;
    dummy_eof = ખોટું;
    write_mode = ખોટું;
    mem_mode = ખોટું;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    empty_stream = સાચું;

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

શૂન્ય FileStorage::Impl::release(String *out) {
    જો (is_opened) {
        જો (out)
            out->clear();
        જો (write_mode) {
            જ્યારે (write_ઢગલો.size() > 1) {
                endWriteStruct();
            }
            flush();
            જો (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            નહીંતર જો (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        જો (mem_mode && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

શૂન્ય FileStorage::Impl::analyze_file_name(અચલ std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    સ્થિર અચલ અક્ષર not_file_name = '\n';
    સ્થિર અચલ અક્ષર parameter_begin = '?';
    સ્થિર અચલ અક્ષર parameter_separator = '&';

    જો (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        પાછા;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    જો (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        માટે (size_t param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            જો ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

તાર્કિક FileStorage::Impl::open(અચલ અક્ષર *filename_or_buf, પૂર્ણાંક _flags, અચલ અક્ષર *encoding) {
    તાર્કિક ok = સાચું;
    release();

    તાર્કિક append = (_flags & 3) == FileStorage::APPEND;
    mem_mode = (_flags & FileStorage::MEMORY) != 0;

    write_mode = (_flags & 3) != 0;
    તાર્કિક write_base64 = (write_mode || append) && (_flags & FileStorage::BASE64) != 0;

    તાર્કિક isGZ = ખોટું;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        જો (!params.empty())
            filename = params[0];

        જો (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_mode || append);
    }

    જો (filename.size() == 0 && !mem_mode && !write_mode)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    જો (mem_mode && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    જો (!mem_mode) {
        size_t dot_idx = filename.find_last_of('.');
        અક્ષર compression = '\0';

        જો (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            જો (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                જો (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = સાચું;
                }

                નહીંતર જો(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = સાચું;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        જો (!isGZ) {
            file = fopen(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            જો (!file)
            {
                CV_LOG_ERROR(ખાલી, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                પાછા ખોટું;
            }
        } નહીંતર {
#જો USE_ZLIB
            જો (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            અક્ષર mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            જો (!gzfile)
            {
                CV_LOG_ERROR(ખાલી, "Can't open archive: '" << filename << "' mode=" << mode);
                પાછા ખોટું;
            }
#નહીંતર
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#endif
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_data.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    જો (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        જો (mem_mode)
            outbuf.clear();

        જો (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            અચલ અક્ષર *dot_pos = ખાલી;
            અચલ અક્ષર *dot_pos2 = ખાલી;
            // like strrchr() implementation, but save two last positions simultaneously
            માટે (અચલ અક્ષર *pos = &filename[0]; pos[0] != 0; pos++) {
                જો (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            જો (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != ખાલી) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } નહીંતર જો (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        પૂર્ણાંક buf_size = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        જો (append) {
            fseek(file, 0, SEEK_END);
            જો (ftell(file) == 0)
                append = ખોટું;
        }

        write_ઢગલો.clear();
        empty_stream = સાચું;
        write_ઢગલો.push_back(FStructData("", File_ગાંઠ::MAP | File_ગાંઠ::EMPTY, 0));
        buffer.reserve(buf_size + 1024);
        buffer.resize(buf_size);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;

        જો (fmt == FileStorage::FORMAT_XML) {
            size_t file_size = file ? (size_t) ftell(file) : (size_t) 0;
            જો (!append || file_size == 0) {
                જો (encoding && *encoding != '\0') {
                    જો (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(strlen(encoding) < 1000);
                    અક્ષર buf[1100];
                    snprintf(buf, કદ(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } નહીંતર
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } નહીંતર {
                પૂર્ણાંક xml_buf_size = 1 << 10;
                અક્ષર substr[] = "</opencv_storage>";
                પૂર્ણાંક last_occurrence = -1;
                xml_buf_size = MIN(xml_buf_size, પૂર્ણાંક(file_size));
                fseek(file, -xml_buf_size, SEEK_END);
                // find the last occurrence of </opencv_storage>
                માટે (;;) {
                    પૂર્ણાંક લીટી_offset = (પૂર્ણાંક) ftell(file);
                    અચલ અક્ષર *ptr0 = આ->gets(xml_buf_size);
                    અચલ અક્ષર *ptr = ખાલી;
                    જો (!ptr0)
                        તોડો;
                    ptr = ptr0;
                    માટે (;;) {
                        ptr = strstr(ptr, substr);
                        જો (!ptr)
                            તોડો;
                        last_occurrence = લીટી_offset + (પૂર્ણાંક) (ptr - ptr0);
                        ptr += strlen(substr);
                    }
                }
                જો (last_occurrence < 0) {
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

            emitter_do_not_use_direct_dereference = createXMLEmitter(આ);
        } નહીંતર જો (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                જો (!append) {
                    જો (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    નહીંતર
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            નહીંતર
                puts("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(આ);
        } નહીંતર {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            જો (!append)
                puts("{\n");
            નહીંતર {
                તાર્કિક valid = ખોટું;
                long roffset = 0;
                માટે (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    અચલ અક્ષર end_mark = '}';
                    જો (fgetc(file) == end_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = સાચું;
                        તોડો;
                    }
                }

                જો (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } નહીંતર {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_ઢગલો.back().indent = 4;
            emitter_do_not_use_direct_dereference = createJSONEmitter(આ);
        }
        is_opened = સાચું;
    } નહીંતર {
        અચલ size_t buf_size0 = 40;
        buffer.resize(buf_size0);
        જો (mem_mode) {
            strbuf = (અક્ષર *) filename_or_buf;
            strbufsize = strlen(strbuf);
        }

        અચલ અક્ષર *yaml_signature = "%YAML";
        અચલ અક્ષર *json_signature = "{";
        અચલ અક્ષર *xml_signature = "<?xml";
        અક્ષર *buf = આ->gets(16);
        CV_Assert(buf);
        અક્ષર *bufPtr = cv_skip_BOM(buf);
        size_t bufOffset = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        જો (strncmp(bufPtr, yaml_signature, strlen(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        નહીંતર જો (strncmp(bufPtr, json_signature, strlen(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        નહીંતર જો (strncmp(bufPtr, xml_signature, strlen(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        નહીંતર
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = bufOffset;
        bufofs = 0;

        પ્રયાસ {
            અક્ષર *ptr = bufferStart();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_ગાંઠ મૂળ_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_ગાંઠ_Space(મૂળ_nodes, 9);
            *rptr = File_ગાંઠ::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            પસંદગી (fmt) {
                સ્થિતિ FileStorage::FORMAT_XML:
                    parser_do_not_use_direct_dereference = createXMLParser(આ);
                    તોડો;
                સ્થિતિ FileStorage::FORMAT_YAML:
                સ્થિતિ FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_direct_dereference = createYAMLParser(આ);
                    તોડો;
                સ્થિતિ FileStorage::FORMAT_JSON:
                    parser_do_not_use_direct_dereference = createJSONParser(આ);
                    તોડો;
                મૂળભૂત:
                    parser_do_not_use_direct_dereference = Ptr<FileStorageParser>();
            }

            જો (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                જો (ok) {
                    finalizeCollection(મૂળ_nodes);

                    CV_Assert(!fs_data_ptrs.empty());
                    File_ગાંઠ roots_ગાંઠ(fs_ext, 0, 0);
                    size_t i, nroots = roots_ગાંઠ.size();
                    File_ગાંઠ_Iterator it = roots_ગાંઠ.begin();

                    માટે (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        પકડો (...)
        {
            // FIXIT log error message
            is_opened = સાચું;
            release();
            ફેંકો;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = સાચું;
        std::vector<અક્ષર> tmpbuf;
        std::swap(buffer, tmpbuf);
        bufofs = 0;
    }
    પાછા ok;
}

શૂન્ય FileStorage::Impl::puts(અચલ અક્ષર *str) {
    CV_Assert(write_mode);
    જો (mem_mode)
        std::copy(str, str + strlen(str), std::back_inserter(outbuf));
    નહીંતર જો (file)
        fputs(str, file);
#જો USE_ZLIB
    નહીંતર જો (gzfile)
        gzputs(gzfile, str);
#endif
    નહીંતર
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

અક્ષર *FileStorage::Impl::getsFromFile(અક્ષર *buf, પૂર્ણાંક ગણતરી_2) {
    જો (file)
        પાછા fgets(buf, ગણતરી_2, file);
#જો USE_ZLIB
    જો (gzfile)
        પાછા gzgets(gzfile, buf, ગણતરી_2);
#endif
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

અક્ષર *FileStorage::Impl::gets(size_t max_ગણતરી) {
    જો (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        અચલ અક્ષર *instr = strbuf;
        માટે (; i < len; i++) {
            અક્ષર c = instr[i];
            જો (c == '\0' || c == '\n') {
                જો (c == '\n')
                    i++;
                તોડો;
            }
        }
        size_t ગણતરી_2 = i - strbufpos;
        જો (max_ગણતરી == 0 || max_ગણતરી > ગણતરી_2)
            max_ગણતરી = ગણતરી_2;
        buffer.resize(std::max(buffer.size(), max_ગણતરી + 8));
        memcpy(&buffer[0], instr + strbufpos, max_ગણતરી);
        buffer[max_ગણતરી] = '\0';
        strbufpos = i;
        પાછા max_ગણતરી > 0 ? &buffer[0] : 0;
    }

    અચલ size_t MAX_BLOCK_SIZE = INT_MAX / 2; // hopefully, that will be enough
    જો (max_ગણતરી == 0)
        max_ગણતરી = MAX_BLOCK_SIZE;
    નહીંતર
        CV_Assert(max_ગણતરી < MAX_BLOCK_SIZE);
    size_t ofs = 0;

    માટે (;;) {
        પૂર્ણાંક ગણતરી_2 = (પૂર્ણાંક) std::min(buffer.size() - ofs - 16, max_ગણતરી);
        અક્ષર *ptr = getsFromFile(&buffer[ofs], ગણતરી_2 + 1);
        જો (!ptr)
            તોડો;
        પૂર્ણાંક delta = (પૂર્ણાંક) strlen(ptr);
        ofs += delta;
        max_ગણતરી -= delta;
        જો (delta == 0 || ptr[delta - 1] == '\n' || max_ગણતરી == 0)
            તોડો;
        જો (delta == ગણતરી_2)
            buffer.resize((size_t) (buffer.size() * 1.5));
    }
    પાછા ofs > 0 ? &buffer[0] : 0;
}

અક્ષર *FileStorage::Impl::gets() {
    અક્ષર *ptr = આ->gets(0);
    જો (!ptr) {
        ptr = bufferStart();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        પાછા 0;
    } નહીંતર {
        size_t l = strlen(ptr);
        જો (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    પાછા ptr;
}

તાર્કિક FileStorage::Impl::eof() {
    જો (dummy_eof)
        પાછા સાચું;
    જો (strbuf)
        પાછા strbufpos >= strbufsize;
    જો (file)
        પાછા feof(file) != 0;
#જો USE_ZLIB
    જો (gzfile)
        પાછા gzeof(gzfile) != 0;
#endif
    પાછા ખોટું;
}

શૂન્ય FileStorage::Impl::setEof() {
    dummy_eof = સાચું;
}

શૂન્ય FileStorage::Impl::closeFile() {
    જો (file)
        fclose(file);
#જો USE_ZLIB
    નહીંતર જો (gzfile)
        gzclose(gzfile);
#endif
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = ખોટું;
}

શૂન્ય FileStorage::Impl::rewind() {
    જો (file)
        ::rewind(file);
#જો USE_ZLIB
    નહીંતર જો (gzfile)
        gzrewind(gzfile);
#endif
    strbufpos = 0;
}

અક્ષર *FileStorage::Impl::resizeWriteBuffer(અક્ષર *ptr, પૂર્ણાંક len) {
    અચલ અક્ષર *buffer_end = &buffer[0] + buffer.size();
    જો (ptr + len < buffer_end)
        પાછા ptr;

    અચલ અક્ષર *buffer_start = &buffer[0];
    પૂર્ણાંક written_len = (પૂર્ણાંક) (ptr - buffer_start);

    CV_Assert(written_len <= (પૂર્ણાંક) buffer.size());
    પૂર્ણાંક new_size = (પૂર્ણાંક) ((buffer_end - buffer_start) * 3 / 2);
    new_size = MAX(written_len + len, new_size);
    buffer.reserve(new_size + 256);
    buffer.resize(new_size);
    bufofs = written_len;
    પાછા &buffer[0] + bufofs;
}

અક્ષર *FileStorage::Impl::flush() {
    અક્ષર *buffer_start = &buffer[0];
    અક્ષર *ptr = buffer_start + bufofs;

    જો (ptr > buffer_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(buffer_start);
        bufofs = 0;
    }

    પૂર્ણાંક indent = write_ઢગલો.back().indent;

    જો (space != indent) {
        memset(buffer_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = buffer_start + bufofs;

    પાછા ptr;
}

શૂન્ય FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(ખોટું);
    જો (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_ઢગલો.empty());

    FStructData &current_struct = write_ઢગલો.back();
    જો (fmt == FileStorage::FORMAT_JSON && !File_ગાંઠ::isFlow(current_struct.flags) && write_ઢગલો.size() > 1)
        current_struct.indent = write_ઢગલો[write_ઢગલો.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_ઢગલો.pop_back();
    જો (!write_ઢગલો.empty())
        write_ઢગલો.back().flags &= ~File_ગાંઠ::EMPTY;
}

શૂન્ય FileStorage::Impl::startWriteStruct_helper(અચલ અક્ષર *ચાવી, પૂર્ણાંક struct_flags,
                                                અચલ અક્ષર *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (File_ગાંઠ::TYPE_MASK | File_ગાંઠ::FLOW)) | File_ગાંઠ::EMPTY;
    જો (!File_ગાંઠ::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    જો (type_name && type_name[0] == '\0')
        type_name = 0;

    FStructData s = getEmitter().startWriteStruct(write_ઢગલો.back(), ચાવી, struct_flags, type_name);

    write_ઢગલો.push_back(s);
    size_t write_ઢગલો_size = write_ઢગલો.size();
    જો (write_ઢગલો_size > 1)
        write_ઢગલો[write_ઢગલો_size - 2].flags &= ~File_ગાંઠ::EMPTY;

    જો (fmt != FileStorage::FORMAT_JSON && !File_ગાંઠ::isFlow(s.flags))
        flush();

    જો (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_ગાંઠ::isMap(struct_flags)) {
        getEmitter().લિખ("type_id", type_name, ખોટું);
    }
}

શૂન્ય FileStorage::Impl::startWriteStruct(અચલ અક્ષર *ચાવી, પૂર્ણાંક struct_flags,
                                         અચલ અક્ષર *type_name) {
    check_if_write_struct_is_delayed(ખોટું);
    જો (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    જો (state_of_writing_base64 == FileStorage_API::Uncertain && File_ગાંઠ::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(ચાવી, struct_flags, type_name);
    } નહીંતર જો (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        જો ((File_ગાંઠ::TYPE_MASK & struct_flags) != File_ગાંઠ::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        નહીંતર જો (state_of_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(ચાવી, struct_flags, "binary");

        જો (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::InUse);
    } નહીંતર {
        /* Won't output Base64 data */
        જો (state_of_writing_base64 == FileStorage_API::InUse)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(ચાવી, struct_flags, type_name);

        જો (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::NotUse);
    }
}

શૂન્ય FileStorage::Impl::writeComment(અચલ અક્ષર *comment, તાર્કિક eol_comment) {
    CV_Assert(write_mode);
    getEmitter().writeComment(comment, eol_comment);
}

શૂન્ય FileStorage::Impl::startNextStream() {
    CV_Assert(write_mode);
    જો (!empty_stream) {
        જ્યારે (!write_ઢગલો.empty())
            endWriteStruct();
        flush();
        getEmitter().startNextStream();
        empty_stream = સાચું;
        write_ઢગલો.push_back(FStructData("", File_ગાંઠ::EMPTY, 0));
        bufofs = 0;
    }
}

શૂન્ય FileStorage::Impl::લિખ(અચલ String &ચાવી, પૂર્ણાંક value) {
    CV_Assert(write_mode);
    getEmitter().લિખ(ચાવી.c_str(), value);
}

શૂન્ય FileStorage::Impl::લિખ(અચલ String &ચાવી, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().લિખ(ચાવી.c_str(), value);
}

શૂન્ય FileStorage::Impl::લિખ(અચલ String &ચાવી, દ્વિગુણ value) {
    CV_Assert(write_mode);
    getEmitter().લિખ(ચાવી.c_str(), value);
}

શૂન્ય FileStorage::Impl::લિખ(અચલ String &ચાવી, અચલ String &value) {
    CV_Assert(write_mode);
    getEmitter().લિખ(ચાવી.c_str(), value.c_str(), ખોટું);
}

શૂન્ય FileStorage::Impl::writeRawData(અચલ std::string &dt, અચલ શૂન્ય *_data, size_t len) {
    CV_Assert(write_mode);

    જો (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        writeRawDataBase64(_data, len, dt.c_str());
        પાછા;
    } નહીંતર જો (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elemSize = fs::calcStructSize(dt.c_str(), 0);
    CV_Assert(elemSize);
    CV_Assert(len % elemSize == 0);
    len /= elemSize;

    તાર્કિક explicitZero = fmt == FileStorage::FORMAT_JSON;
    અચલ uchar *data0 = (અચલ uchar *) _data;
    પૂર્ણાંક fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_ગણતરી;
    અક્ષર buf[256] = "";

    fmt_pair_ગણતરી = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    જો (!len)
        પાછા;

    જો (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    જો (fmt_pair_ગણતરી == 1) {
        fmt_pairs[0] *= (પૂર્ણાંક) len;
        len = 1;
    }

    માટે (; len--; data0 += elemSize) {
        પૂર્ણાંક offset = 0;
        માટે (k = 0; k < fmt_pair_ગણતરી; k++) {
            પૂર્ણાંક i, ગણતરી_2 = fmt_pairs[k * 2];
            પૂર્ણાંક elem_type = fmt_pairs[k * 2 + 1];
            પૂર્ણાંક elem_size = CV_ELEM_SIZE(elem_type);
            અચલ અક્ષર *ptr;

            offset = cvAlign(offset, elem_size);
            અચલ uchar *data = data0 + offset;

            માટે (i = 0; i < ગણતરી_2; i++) {
                પસંદગી (elem_type) {
                    સ્થિતિ CV_8U:
                        ptr = fs::itoa(*(uchar *) data, buf, 10);
                        data++;
                        તોડો;
                    સ્થિતિ CV_Bool:
                        ptr = fs::itoa(*(uchar *) data != 0, buf, 10);
                        data++;
                        તોડો;
                    સ્થિતિ CV_8S:
                        ptr = fs::itoa(*(અક્ષર *) data, buf, 10);
                        data++;
                        તોડો;
                    સ્થિતિ CV_16U:
                        ptr = fs::itoa(*(ushort *) data, buf, 10);
                        data += કદ(ushort);
                        તોડો;
                    સ્થિતિ CV_16S:
                        ptr = fs::itoa(*(short *) data, buf, 10);
                        data += કદ(short);
                        તોડો;
                    સ્થિતિ CV_32U:
                        ptr = fs::itoa((int64_t)*(unsigned*) data, buf, 10, ખોટું);
                        data += કદ(unsigned);
                        તોડો;
                    સ્થિતિ CV_32S:
                        ptr = fs::itoa(*(પૂર્ણાંક *) data, buf, 10);
                        data += કદ(પૂર્ણાંક);
                        તોડો;
                    સ્થિતિ CV_64U:
                        ptr = fs::itoa(*(uint64_t*) data, buf, 10, ખોટું);
                        data += કદ(uint64_t);
                        તોડો;
                    સ્થિતિ CV_64S:
                        ptr = fs::itoa(*(int64_t*) data, buf, 10, સાચું);
                        data += કદ(int64_t);
                        તોડો;
                    સ્થિતિ CV_32F:
                        ptr = fs::floatToString(buf, કદ(buf), *(દશાંશ *) data, ખોટું, explicitZero);
                        data += કદ(દશાંશ);
                        તોડો;
                    સ્થિતિ CV_64F:
                        ptr = fs::doubleToString(buf, કદ(buf), *(દ્વિગુણ *) data, explicitZero);
                        data += કદ(દ્વિગુણ);
                        તોડો;
                    સ્થિતિ CV_16F:
                        ptr = fs::floatToString(buf, કદ(buf), (દશાંશ) *(hfloat *) data, સાચું, explicitZero);
                        data += કદ(hfloat);
                        તોડો;
                    સ્થિતિ CV_16BF:
                        ptr = fs::floatToString(buf, કદ(buf), (દશાંશ) *(bfloat *) data, સાચું, explicitZero);
                        data += કદ(bfloat);
                        તોડો;
                    સ્થિતિ CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, કદ(buf), (દશાંશ) *(fp8_t *) data, સાચું, explicitZero);
                        data += કદ(fp8_t);
                        તોડો;
                    સ્થિતિ CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, કદ(buf), (દશાંશ) *(fp8a_t *) data, સાચું, explicitZero);
                        data += કદ(fp8a_t);
                        તોડો;
                    મૂળભૂત:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        પાછા;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (પૂર્ણાંક) (data - data0);
        }
    }
}

શૂન્ય FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(ખોટું);

    જો (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_to_Base64_state(FileStorage_API::Base64State::Uncertain);
}

શૂન્ય FileStorage::Impl::switch_to_Base64_state(FileStorage_API::Base64State new_state) {
    અચલ અક્ષર *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    અચલ અક્ષર *err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    પસંદગી (state_of_writing_base64) {
        સ્થિતિ FileStorage_API::Base64State::Uncertain:
            પસંદગી (new_state) {
                સ્થિતિ FileStorage_API::Base64State::InUse:
                {
                    CV_DbgAssert(base64_writer == 0);
                    તાર્કિક can_indent = (fmt != cv::FileStorage::Mode::FORMAT_JSON);
                    base64_writer = નવું base64::Base64Writer(*આ, can_indent);
                    જો (!can_indent) {
                        અક્ષર *ptr = bufferPtr();
                        *ptr++ = '\0';
                        puts(bufferStart());
                        setBufferPtr(bufferStart());
                        memset(bufferStart(), 0, static_cast<પૂર્ણાંક>(space));
                        puts("\"$base64$");
                    }
                    તોડો;
                }
                સ્થિતિ FileStorage_API::Base64State::Uncertain:
                    તોડો;
                સ્થિતિ FileStorage_API::Base64State::NotUse:
                    તોડો;
                મૂળભૂત:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    તોડો;
            }
            તોડો;
        સ્થિતિ FileStorage_API::Base64State::InUse:
            પસંદગી (new_state) {
                સ્થિતિ FileStorage_API::Base64State::InUse:
                સ્થિતિ FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    તોડો;
                સ્થિતિ FileStorage_API::Base64State::Uncertain:
                    delete base64_writer;
                    base64_writer = 0;
                    જો ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        setBufferPtr(bufferStart());
                        flush();
                        memset(bufferStart(), 0, static_cast<પૂર્ણાંક>(space) );
                        setBufferPtr(bufferStart());
                    }
                    તોડો;
                મૂળભૂત:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    તોડો;
            }
            તોડો;
        સ્થિતિ FileStorage_API::Base64State::NotUse:
            પસંદગી (new_state) {
                સ્થિતિ FileStorage_API::Base64State::InUse:
                સ્થિતિ FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    તોડો;
                સ્થિતિ FileStorage_API::Base64State::Uncertain:
                    તોડો;
                મૂળભૂત:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    તોડો;
            }
            તોડો;
        મૂળભૂત:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            તોડો;
    }

    state_of_writing_base64 = new_state;
}

શૂન્ય FileStorage::Impl::make_write_struct_delayed(અચલ અક્ષર *ચાવી, પૂર્ણાંક struct_flags, અચલ અક્ષર *type_name) {
    CV_Assert(is_write_struct_delayed == ખોટું);
    CV_DbgAssert(delayed_struct_ચાવી == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    જો (ચાવી != nullptr) {
        delayed_struct_ચાવી = નવું અક્ષર[strlen(ચાવી) + 1U];
        strcpy(delayed_struct_ચાવી, ચાવી);
    }

    જો (type_name != nullptr) {
        delayed_type_name = નવું અક્ષર[strlen(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = સાચું;
}

શૂન્ય FileStorage::Impl::check_if_write_struct_is_delayed(તાર્કિક change_type_to_base64) {
    જો (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_ચાવી;
        std::string type_name;
        પૂર્ણાંક struct_flags = delayed_struct_flags;

        જો (delayed_struct_ચાવી != nullptr && *delayed_struct_ચાવી != '\0') {
            struct_ચાવી.assign(delayed_struct_ચાવી);
        }
        જો (delayed_type_name != nullptr && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        delete[] delayed_struct_ચાવી;
        delete[] delayed_type_name;
        delayed_struct_ચાવી = nullptr;
        delayed_struct_flags = 0;
        delayed_type_name = nullptr;

        is_write_struct_delayed = ખોટું;

        /* call */
        જો (change_type_to_base64) {
            startWriteStruct_helper(struct_ચાવી.c_str(), struct_flags, "binary");
            જો (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::InUse);
        } નહીંતર {
            startWriteStruct_helper(struct_ચાવી.c_str(), struct_flags, type_name.c_str());
            જો (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::NotUse);
        }
    }
}

શૂન્ય FileStorage::Impl::writeRawDataBase64(અચલ શૂન્ય *_data, size_t len, અચલ અક્ષર *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(સાચું);

    જો (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } નહીંતર જો (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->લિખ(_data, len, dt);
}

File_ગાંઠ FileStorage::Impl::get_First_ટોચ_Level_ગાંઠ() અચલ {
    પાછા roots.empty() ? File_ગાંઠ() : roots[0];
}

File_ગાંઠ FileStorage::Impl::મૂળ(પૂર્ણાંક streamIdx) અચલ {
    પાછા streamIdx >= 0 && streamIdx < (પૂર્ણાંક) roots.size() ? roots[streamIdx] : File_ગાંઠ();
}

File_ગાંઠ FileStorage::Impl::operator[](અચલ String &nodename) અચલ {
    પાછા આ->operator[](nodename.c_str());
}

File_ગાંઠ FileStorage::Impl::operator[](અચલ અક્ષર * /*nodename*/) અચલ {
    પાછા File_ગાંઠ();
}

પૂર્ણાંક FileStorage::Impl::getFormat() અચલ { પાછા fmt; }

અક્ષર *FileStorage::Impl::bufferPtr() અચલ { પાછા (અક્ષર *) (&buffer[0] + bufofs); }

અક્ષર *FileStorage::Impl::bufferStart() અચલ { પાછા (અક્ષર *) &buffer[0]; }

અક્ષર *FileStorage::Impl::bufferEnd() અચલ { પાછા (અક્ષર *) (&buffer[0] + buffer.size()); }

શૂન્ય FileStorage::Impl::setBufferPtr(અક્ષર *ptr) {
    અક્ષર *bufferstart = bufferStart();
    CV_Assert(ptr >= bufferstart && ptr <= bufferEnd());
    bufofs = ptr - bufferstart;
}

પૂર્ણાંક FileStorage::Impl::wrapMargin() અચલ { પાછા wrap_margin; }

FStructData &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_ઢગલો.empty());
    પાછા write_ઢગલો.back();
}

શૂન્ય FileStorage::Impl::setNonEmpty() {
    empty_stream = ખોટું;
}

શૂન્ય FileStorage::Impl::processSpecialDouble(અક્ષર *buf, દ્વિગુણ *value, અક્ષર **endptr) {
    FileStorage_API *fs = આ;
    અક્ષર c = buf[0];
    પૂર્ણાંક inf_hi = 0x7ff00000;

    જો (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    જો (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    જો (toupper(buf[1]) == 'I' && toupper(buf[2]) == 'N' && toupper(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    નહીંતર જો (toupper(buf[1]) == 'N' && toupper(buf[2]) == 'A' && toupper(buf[3]) == 'N')
        v.u = (uint64) -1;
    નહીંતર
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

દ્વિગુણ FileStorage::Impl::strtod(અક્ષર *ptr, અક્ષર **endptr) {
    દ્વિગુણ fval = ::strtod(ptr, endptr);
    જો (**endptr == '.') {
        અક્ષર *dot_pos = *endptr;
        *dot_pos = ',';
        દ્વિગુણ fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        જો (*endptr > dot_pos)
            fval = fval2;
        નહીંતર
            *endptr = dot_pos;
    }

    જો (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    પાછા fval;
}

શૂન્ય FileStorage::Impl::convertToCollection(પૂર્ણાંક type, File_ગાંઠ &ગાંઠ) {
    CV_Assert(type == File_ગાંઠ::SEQ || type == File_ગાંઠ::MAP);

    પૂર્ણાંક ગાંઠ_type = ગાંઠ.type();
    જો (ગાંઠ_type == type)
        પાછા;

    તાર્કિક named = ગાંઠ.isNamed();
    uchar *ptr = ગાંઠ.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    દ્વિગુણ fval = 0;
    std::string sval;
    તાર્કિક add_first_scalar = ખોટું;

    જો (ગાંઠ_type != File_ગાંઠ::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_ગાંઠ::SEQ);
        જો (ગાંઠ_type == File_ગાંઠ::INT) {
            ival = readLong(ptr);
            add_first_scalar = સાચું;
        } નહીંતર જો (ગાંઠ_type == File_ગાંઠ::REAL) {
            fval = readReal(ptr);
            add_first_scalar = સાચું;
        } નહીંતર જો (ગાંઠ_type == File_ગાંઠ::STRING) {
            sval = std::string(ગાંઠ);
            add_first_scalar = સાચું;
        } નહીંતર
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", ગાંઠ_type));
    }

    ptr = reserve_ગાંઠ_Space(ગાંઠ, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_ગાંઠ::NAMED : 0));
    // name has been copied automatically
    જો (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    જો (add_first_scalar)
        add_ગાંઠ(ગાંઠ, std::string(), ગાંઠ_type,
                ગાંઠ_type == File_ગાંઠ::INT ? (અચલ શૂન્ય *) &ival :
                ગાંઠ_type == File_ગાંઠ::REAL ? (અચલ શૂન્ય *) &fval :
                ગાંઠ_type == File_ગાંઠ::STRING ? (અચલ શૂન્ય *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_ગાંઠ_Space(File_ગાંઠ &ગાંઠ, size_t sz) {
    તાર્કિક shrinkBlock = ખોટું;
    size_t shrinkBlockIdx = 0, shrinkSize = 0;

    uchar *ptr = 0, *blockEnd = 0;

    જો (!fs_data_ptrs.empty()) {
        size_t blockIdx = ગાંઠ.blockIdx;
        size_t ofs = ગાંઠ.ofs;
        CV_Assert(blockIdx == fs_data_ptrs.size() - 1);
        CV_Assert(ofs <= fs_data_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_data_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_data_ptrs[blockIdx] + ofs;
        blockEnd = fs_data_ptrs[blockIdx] + fs_data_blksz[blockIdx];

        CV_Assert(ptr >= fs_data_ptrs[blockIdx] && ptr <= blockEnd);
        જો (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            પાછા ptr;
        }

        જો (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_data[blockIdx]->resize(sz);
            ptr = &fs_data[blockIdx]->at(0);
            fs_data_ptrs[blockIdx] = ptr;
            fs_data_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            પાછા ptr;
        }

        shrinkBlock = સાચું;
        shrinkBlockIdx = blockIdx;
        shrinkSize = ofs;
    }

    size_t blockSize = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(blockSize);
    fs_data.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_data_ptrs.push_back(new_ptr);
    fs_data_blksz.push_back(blockSize);
    ગાંઠ.blockIdx = fs_data_ptrs.size() - 1;
    ગાંઠ.ofs = 0;
    freeSpaceOfs = sz;

    જો (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        જો (ptr[0] & File_ગાંઠ::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    જો (shrinkBlock) {
        fs_data[shrinkBlockIdx]->resize(shrinkSize);
        fs_data_blksz[shrinkBlockIdx] = shrinkSize;
    }

    પાછા new_ptr;
}

unsigned FileStorage::Impl::getStringOfs(અચલ std::string &ચાવી) અચલ {
    str_hash_t::const_iterator it = str_hash.find(ચાવી);
    પાછા it != str_hash.end() ? it->second : 0;
}

File_ગાંઠ FileStorage::Impl::add_ગાંઠ(File_ગાંઠ &collection, અચલ std::string &ચાવી,
                                    પૂર્ણાંક elem_type, અચલ શૂન્ય *value, પૂર્ણાંક len) {
    FileStorage_API *fs = આ;
    તાર્કિક noname = ચાવી.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(ચાવી.c_str(), "_") == 0);
    convertToCollection(noname ? File_ગાંઠ::SEQ : File_ગાંઠ::MAP, collection);

    તાર્કિક isseq = collection.empty() ? ખોટું : collection.isSeq();
    જો (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    unsigned strofs = 0;
    જો (!noname) {
        strofs = getStringOfs(ચાવી);
        જો (!strofs) {
            strofs = (unsigned) str_hash_data.size();
            size_t keysize = ચાવી.size() + 1;
            str_hash_data.resize(strofs + keysize);
            memcpy(&str_hash_data[0] + strofs, &ચાવી[0], keysize);
            str_hash.insert(std::make_pair(ચાવી, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t blockIdx = fs_data_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_ગાંઠ ગાંઠ(fs_ext, blockIdx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_ગાંઠ_Space(ગાંઠ, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_ગાંઠ::NAMED));
    જો (elem_type == File_ગાંઠ::NONE)
        freeSpaceOfs -= 8;

    જો (!noname) {
        writeInt(ptr, (પૂર્ણાંક) strofs);
        ptr += 4;
    }

    જો (elem_type == File_ગાંઠ::SEQ || elem_type == File_ગાંઠ::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    જો (value)
        ગાંઠ.set_કિંમત(elem_type, value, len);

    જો (collection.isNamed())
        cp += 4;
    પૂર્ણાંક nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    પાછા ગાંઠ;
}

શૂન્ય FileStorage::Impl::finalizeCollection(File_ગાંઠ &collection) {
    જો (!collection.isSeq() && !collection.isMap())
        પાછા;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    જો (*ptr0 & File_ગાંઠ::NAMED)
        ptr += 4;
    size_t blockIdx = collection.blockIdx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t rawSize = 4;
    unsigned sz = (unsigned) readInt(ptr + 4);
    જો (sz > 0) {
        size_t lastBlockIdx = fs_data_ptrs.size() - 1;

        માટે (; blockIdx < lastBlockIdx; blockIdx++) {
            rawSize += fs_data_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    rawSize += freeSpaceOfs - ofs;
    writeInt(ptr, (પૂર્ણાંક) rawSize);
}

શૂન્ય FileStorage::Impl::normalize_ગાંઠ_Ofs(size_t &blockIdx, size_t &ofs) અચલ {
    જ્યારે (ofs >= fs_data_blksz[blockIdx]) {
        જો (blockIdx == fs_data_blksz.size() - 1) {
            CV_Assert(ofs == fs_data_blksz[blockIdx]);
            તોડો;
        }
        ofs -= fs_data_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    પાછા state_of_writing_base64;
}

પૂર્ણાંક FileStorage::Impl::get_space() {
    પાછા space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = સાચું;
}

શૂન્ય FileStorage::Impl::Base64Decoder::init(અચલ Ptr<FileStorageParser> &_parser, અક્ષર *_ptr, પૂર્ણાંક _indent) {
    parser_do_not_use_direct_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = ખોટું;
}

તાર્કિક FileStorage::Impl::Base64Decoder::readMore(પૂર્ણાંક needed) {
    સ્થિર અચલ uchar base64tab[] =
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

    જો (eos)
        પાછા ખોટું;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    માટે (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    અક્ષર *beg = 0, *end = 0;
    તાર્કિક ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    જો (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = સાચું;
        size_t tc = totalchars;
        માટે (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    પૂર્ણાંક i = 0, j, n = (પૂર્ણાંક) encoded.size();
    જો (n > 0) {
        અચલ uchar *tab = base64tab;
        અક્ષર *src = &encoded[0];

        માટે (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(પૂર્ણાંક) (uchar) src[i]], c = tab[(પૂર્ણાંક) (uchar) src[i + 1]];
            uchar b = tab[(પૂર્ણાંક) (uchar) src[i + 2]], a = tab[(પૂર્ણાંક) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    જો (i > 0 && encoded[i - 1] == '=') {
        જો (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        જો (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    માટે (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    પાછા (પૂર્ણાંક) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    જો (ofs >= sz && !readMore(1))
        પાછા (uchar) 0;
    પાછા decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    જો (ofs + 2 > sz && !readMore(2))
        પાછા (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    પાછા val;
}

પૂર્ણાંક FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    જો (ofs + 4 > sz && !readMore(4))
        પાછા 0;
    પૂર્ણાંક ival = readInt(&decoded[ofs]);
    ofs += 4;
    પાછા ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    જો (ofs + 8 > sz && !readMore(8))
        પાછા 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    પાછા ival;
}

દ્વિગુણ FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    જો (ofs + 8 > sz && !readMore(8))
        પાછા 0;
    દ્વિગુણ fval = readReal(&decoded[ofs]);
    ofs += 8;
    પાછા fval;
}

તાર્કિક FileStorage::Impl::Base64Decoder::endOfStream() અચલ { પાછા eos; }

અક્ષર *FileStorage::Impl::Base64Decoder::getPtr() અચલ { પાછા ptr; }


અક્ષર *FileStorage::Impl::parseBase64(અક્ષર *ptr, પૂર્ણાંક indent, File_ગાંઠ &collection) {
    અચલ પૂર્ણાંક BASE64_HDR_SIZE = 24;
    અક્ષર dt[BASE64_HDR_SIZE + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    પૂર્ણાંક i, k;

    માટે (i = 0; i < BASE64_HDR_SIZE; i++)
        dt[i] = (અક્ષર) base64decoder.getUInt8();
    માટે (i = 0; i < BASE64_HDR_SIZE; i++)
        જો (isspace(dt[i]))
            તોડો;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    પૂર્ણાંક fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    પૂર્ણાંક fmt_pair_ગણતરી = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    દ્વિગુણ fval = 0;

    માટે (;;) {
        માટે (k = 0; k < fmt_pair_ગણતરી; k++) {
            પૂર્ણાંક elem_type = fmt_pairs[k * 2 + 1];
            પૂર્ણાંક ગણતરી_2 = fmt_pairs[k * 2];

            માટે (i = 0; i < ગણતરી_2; i++) {
                પૂર્ણાંક ગાંઠ_type = File_ગાંઠ::INT;
                પસંદગી (elem_type) {
                    સ્થિતિ CV_8U:
                        ival = base64decoder.getUInt8();
                        તોડો;
                    સ્થિતિ CV_8S:
                        ival = (અક્ષર) base64decoder.getUInt8();
                        તોડો;
                    સ્થિતિ CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        તોડો;
                    સ્થિતિ CV_16U:
                        ival = base64decoder.getUInt16();
                        તોડો;
                    સ્થિતિ CV_16S:
                        ival = (short) base64decoder.getUInt16();
                        તોડો;
                    સ્થિતિ CV_32U:
                        ival = base64decoder.getInt32();
                        તોડો;
                    સ્થિતિ CV_32S:
                        ival = base64decoder.getInt32();
                        તોડો;
                    સ્થિતિ CV_64U:
                        ival = base64decoder.getInt64();
                        તોડો;
                    સ્થિતિ CV_64S:
                        ival = base64decoder.getInt64();
                        તોડો;
                    સ્થિતિ CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        ગાંઠ_type = File_ગાંઠ::REAL;
                    }
                        તોડો;
                    સ્થિતિ CV_64F:
                        fval = base64decoder.getFloat64();
                        ગાંઠ_type = File_ગાંઠ::REAL;
                        તોડો;
                    સ્થિતિ CV_16F:
                        fval = દશાંશ(hfloatFromBits(base64decoder.getUInt16()));
                        ગાંઠ_type = File_ગાંઠ::REAL;
                        તોડો;
                    સ્થિતિ CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        ગાંઠ_type = File_ગાંઠ::REAL;
                        તોડો;
                    સ્થિતિ CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        ગાંઠ_type = File_ગાંઠ::REAL;
                        તોડો;
                    મૂળભૂત:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                જો (base64decoder.endOfStream())
                    તોડો;
                add_ગાંઠ(collection, std::string(), ગાંઠ_type,
                        ગાંઠ_type == File_ગાંઠ::INT ? (શૂન્ય *) &ival : (શૂન્ય *) &fval, -1);
            }
        }
        જો (base64decoder.endOfStream())
            તોડો;
    }

    finalizeCollection(collection);
    પાછા base64decoder.getPtr();
}

શૂન્ય FileStorage::Impl::parseError(અચલ અક્ષર *func_name, અચલ std::string &err_સંદેશ, અચલ અક્ષર *source_file,
                                   પૂર્ણાંક source_લીટી) {
    std::string સંદેશ = format("%s(%d): %s", filename.c_str(), lineno, err_સંદેશ.c_str());
    error(Error::StsParseError, func_name, સંદેશ.c_str(), source_file, source_લીટી);
}

અચલ uchar *FileStorage::Impl::get_ગાંઠ_Ptr(size_t blockIdx, size_t ofs) અચલ {
    CV_Assert(blockIdx < fs_data_ptrs.size());
    CV_Assert(ofs < fs_data_blksz[blockIdx]);

    પાછા fs_data_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) અચલ {
    CV_Assert(nameofs < str_hash_data.size());
    પાછા std::string(&str_hash_data[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { પાછા fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(આ);
}

FileStorage::FileStorage(અચલ String& filename, પૂર્ણાંક flags, અચલ String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(આ);
    તાર્કિક ok = p->open(filename.c_str(), flags, encoding.c_str());
    જો(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

શૂન્ય FileStorage::startWriteStruct(અચલ String& name, પૂર્ણાંક struct_flags, અચલ String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    જો ((struct_flags & File_ગાંઠ::TYPE_MASK) == File_ગાંઠ::SEQ)
        state = FileStorage::કિંમત_EXPECTED;
    નહીંતર
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

શૂન્ય FileStorage::endWriteStruct()
{
    p->endWriteStruct();
    state = p->write_ઢગલો.empty() || File_ગાંઠ::isMap(p->write_ઢગલો.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::કિંમત_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

તાર્કિક FileStorage::open(અચલ String& filename, પૂર્ણાંક flags, અચલ String& encoding)
{
    પ્રયાસ
    {
        તાર્કિક ok = p->open(filename.c_str(), flags, encoding.c_str());
        જો(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        પાછા ok;
    }
    પકડો (...)
    {
        release();
        ફેંકો;  // re-throw
    }
}

તાર્કિક FileStorage::isOpened() અચલ { પાછા p->is_opened; }

શૂન્ય FileStorage::release()
{
    p->release();
}

File_ગાંઠ FileStorage::મૂળ(પૂર્ણાંક i) અચલ
{
    જો( p.empty() || p->roots.empty() || i < 0 || i >= (પૂર્ણાંક)p->roots.size() )
        પાછા File_ગાંઠ();

    પાછા p->roots[i];
}

File_ગાંઠ FileStorage::get_First_ટોચ_Level_ગાંઠ() અચલ
{
    File_ગાંઠ r = મૂળ();
    File_ગાંઠ_Iterator it = r.begin();
    પાછા it != r.end() ? *it : File_ગાંઠ();
}

std::string FileStorage::getDefaultObjectName(અચલ std::string& _filename)
{
    સ્થિર અચલ અક્ષર* stubname = "unnamed";
    અચલ અક્ષર* filename = _filename.c_str();
    અચલ અક્ષર* ptr2 = filename + _filename.size();
    અચલ અક્ષર* ptr = ptr2 - 1;
    cv::AutoBuffer<અક્ષર> name_buf(_filename.size()+1);

    જ્યારે( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        જો( *ptr == '.' && (!*ptr2 || strncmp(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    જો( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    અક્ષર* name = name_buf.data();

    // name must start with letter or '_'
    જો( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    જ્યારે( ptr < ptr2 )
    {
        અક્ષર c = *ptr++;
        જો( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.data();
    જો( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    પાછા name;
}


પૂર્ણાંક FileStorage::getFormat() અચલ
{
    પાછા p->fmt;
}

File_ગાંઠ FileStorage::operator [](અચલ અક્ષર* ચાવી) અચલ
{
    પાછા આ->operator[](std::string(ચાવી));
}

File_ગાંઠ FileStorage::operator [](અચલ std::string& ચાવી) અચલ
{
    File_ગાંઠ res;
    માટે (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][ચાવી];
        જો (!res.empty())
            તોડો;
    }
    પાછા res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    પાછા buf;
}

શૂન્ય FileStorage::writeRaw( અચલ String& fmt, અચલ શૂન્ય* vec, size_t len )
{
    p->writeRawData(fmt, (અચલ uchar*)vec, len);
}

શૂન્ય FileStorage::writeComment( અચલ String& comment, તાર્કિક eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

શૂન્ય writeScalar( FileStorage& fs, પૂર્ણાંક value )
{
    fs.p->લિખ(String(), value);
}

શૂન્ય writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->લિખ(String(), value);
}

શૂન્ય writeScalar( FileStorage& fs, દશાંશ value )
{
    fs.p->લિખ(String(), (દ્વિગુણ)value);
}

શૂન્ય writeScalar( FileStorage& fs, દ્વિગુણ value )
{
    fs.p->લિખ(String(), value);
}

શૂન્ય writeScalar( FileStorage& fs, અચલ String& value )
{
    fs.p->લિખ(String(), value);
}

શૂન્ય લિખ( FileStorage& fs, અચલ String& name, પૂર્ણાંક value )
{
    fs.p->લિખ(name, value);
}

શૂન્ય લિખ( FileStorage& fs, અચલ String& name, int64_t value )
{
    fs.p->લિખ(name, value);
}

શૂન્ય લિખ( FileStorage& fs, અચલ String& name, દશાંશ value )
{
    fs.p->લિખ(name, (દ્વિગુણ)value);
}

શૂન્ય લિખ( FileStorage& fs, અચલ String& name, દ્વિગુણ value )
{
    fs.p->લિખ(name, value);
}

શૂન્ય લિખ( FileStorage& fs, અચલ String& name, અચલ String& value )
{
    fs.p->લિખ(name, value);
}

શૂન્ય લિખ( FileStorage& fs, અચલ String& name, તાર્કિક value )
{
    fs.p->લિખ(name, value);
}

શૂન્ય FileStorage::લિખ(અચલ String& name, પૂર્ણાંક val) { p->લિખ(name, val); }
શૂન્ય FileStorage::લિખ(અચલ String& name, તાર્કિક val) { p->લિખ(name, val); }
શૂન્ય FileStorage::લિખ(અચલ String& name, int64_t val) { p->લિખ(name, val); }
શૂન્ય FileStorage::લિખ(અચલ String& name, દ્વિગુણ val) { p->લિખ(name, val); }
શૂન્ય FileStorage::લિખ(અચલ String& name, અચલ String& val) { p->લિખ(name, val); }
શૂન્ય FileStorage::લિખ(અચલ String& name, અચલ Mat& val) { cv::લિખ(*આ, name, val); }
શૂન્ય FileStorage::લિખ(અચલ String& name, અચલ std::vector<String>& val) { cv::લિખ(*આ, name, val); }

FileStorage& operator << (FileStorage& fs, અચલ String& str)
{
    ગણના { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        કિંમત_EXPECTED = FileStorage::કિંમત_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    અચલ અક્ષર* _str = str.c_str();
    જો( !fs.isOpened() || !_str )
        પાછા fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    અક્ષર c = *_str;

    જો( c == '}' || c == ']' )
    {
        જો( fs_impl->write_ઢગલો.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        પૂર્ણાંક struct_flags = fs_impl->write_ઢગલો.back().flags;
        અક્ષર expected_bracket = File_ગાંઠ::isMap(struct_flags) ? '}' : ']';
        જો( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_ઢગલો.empty());
        struct_flags = fs_impl->write_ઢગલો.back().flags;
        fs.state = File_ગાંઠ::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : કિંમત_EXPECTED;
        fs.elname = String();
    }
    નહીંતર જો( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        જો (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = કિંમત_EXPECTED + INSIDE_MAP;
    }
    નહીંતર જો( (fs.state & 3) == કિંમત_EXPECTED )
    {
        જો( c == '{' || c == '[' )
        {
            પૂર્ણાંક struct_flags = c == '{' ? File_ગાંઠ::MAP : File_ગાંઠ::SEQ;
            fs.state = struct_flags == File_ગાંઠ::MAP ? INSIDE_MAP + NAME_EXPECTED : કિંમત_EXPECTED;
            _str++;
            જો( *_str == ':' )
            {
                _str++;
                જો( !*_str )
                    struct_flags |= File_ગાંઠ::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        નહીંતર
        {
            લિખ( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            જો( fs.state == INSIDE_MAP + કિંમત_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    નહીંતર
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    પાછા fs;
}


File_ગાંઠ::File_ગાંઠ()
    : fs(ખાલી)
{
    blockIdx = ofs = 0;
}

File_ગાંઠ::File_ગાંઠ(FileStorage::Impl* _fs, size_t _blockIdx, size_t _ofs)
    : fs(_fs)
{
    blockIdx = _blockIdx;
    ofs = _ofs;
}

File_ગાંઠ::File_ગાંઠ(અચલ FileStorage* _fs, size_t _blockIdx, size_t _ofs)
    : File_ગાંઠ(_fs->p.get(), _blockIdx, _ofs)
{
    // nothing
}

File_ગાંઠ::File_ગાંઠ(અચલ File_ગાંઠ& ગાંઠ)
{
    fs = ગાંઠ.fs;
    blockIdx = ગાંઠ.blockIdx;
    ofs = ગાંઠ.ofs;
}

File_ગાંઠ& File_ગાંઠ::operator=(અચલ File_ગાંઠ& ગાંઠ)
{
    fs = ગાંઠ.fs;
    blockIdx = ગાંઠ.blockIdx;
    ofs = ગાંઠ.ofs;
    પાછા *આ;
}

File_ગાંઠ File_ગાંઠ::operator[](અચલ std::string& nodename) અચલ
{
    જો(!fs)
        પાછા File_ગાંઠ();

    CV_Assert( isMap() );

    unsigned ચાવી = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_ગાંઠ_Iterator it = begin();

    માટે( i = 0; i < sz; i++, ++it )
    {
        File_ગાંઠ n = *it;
        અચલ uchar* p = n.ptr();
        unsigned key2 = (unsigned)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_data.size() );
        જો( ચાવી == key2 )
            પાછા n;
    }
    પાછા File_ગાંઠ();
}

File_ગાંઠ File_ગાંઠ::operator[](અચલ અક્ષર* nodename) અચલ
{
    પાછા આ->operator[](std::string(nodename));
}

File_ગાંઠ File_ગાંઠ::operator[](પૂર્ણાંક i) અચલ
{
    જો(!fs)
        પાછા File_ગાંઠ();

    CV_Assert( isSeq() );

    પૂર્ણાંક sz = (પૂર્ણાંક)size();
    CV_Assert( 0 <= i && i < sz );

    File_ગાંઠ_Iterator it = begin();
    it += i;

    પાછા *it;
}

std::vector<String> File_ગાંઠ::ચાવીઓ() અચલ
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    માટે (File_ગાંઠ_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    પાછા res;
}

પૂર્ણાંક File_ગાંઠ::type() અચલ
{
    અચલ uchar* p = ptr();
    જો(!p)
        પાછા NONE;
    પાછા (*p & TYPE_MASK);
}

તાર્કિક File_ગાંઠ::isMap(પૂર્ણાંક flags) { પાછા (flags & TYPE_MASK) == MAP; }
તાર્કિક File_ગાંઠ::isSeq(પૂર્ણાંક flags) { પાછા (flags & TYPE_MASK) == SEQ; }
તાર્કિક File_ગાંઠ::isCollection(પૂર્ણાંક flags) { પાછા isMap(flags) || isSeq(flags); }
તાર્કિક File_ગાંઠ::isFlow(પૂર્ણાંક flags) { પાછા (flags & FLOW) != 0; }
તાર્કિક File_ગાંઠ::isEmptyCollection(પૂર્ણાંક flags) { પાછા (flags & EMPTY) != 0; }

તાર્કિક File_ગાંઠ::empty() અચલ   { પાછા fs == 0; }
તાર્કિક File_ગાંઠ::isNone() અચલ  { પાછા type() == NONE; }
તાર્કિક File_ગાંઠ::isSeq() અચલ   { પાછા type() == SEQ; }
તાર્કિક File_ગાંઠ::isMap() અચલ   { પાછા type() == MAP; }
તાર્કિક File_ગાંઠ::isInt() અચલ   { પાછા type() == INT;  }
તાર્કિક File_ગાંઠ::isReal() અચલ  { પાછા type() == REAL; }
તાર્કિક File_ગાંઠ::isString() અચલ { પાછા type() == STRING;  }
તાર્કિક File_ગાંઠ::isNamed() અચલ
{
    અચલ uchar* p = ptr();
    જો(!p)
        પાછા ખોટું;
    પાછા (*p & NAMED) != 0;
}

std::string File_ગાંઠ::name() અચલ
{
    અચલ uchar* p = ptr();
    જો(!p)
        પાછા std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    પાછા fs->getName(nameofs);
}

File_ગાંઠ::operator પૂર્ણાંક() અચલ
{
    અચલ uchar* p = ptr();
    જો(!p)
        પાછા 0;
    પૂર્ણાંક tag = *p;
    પૂર્ણાંક type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    જો( type == INT )
    {
        પાછા readInt(p);
    }
    નહીંતર જો( type == REAL )
    {
        પાછા cvRound(readReal(p));
    }
    નહીંતર
        પાછા 0x7fffffff;
}

File_ગાંઠ::operator int64_t() અચલ
{
    અચલ uchar* p = ptr();
    જો(!p)
        પાછા 0;
    પૂર્ણાંક tag = *p;
    પૂર્ણાંક type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    જો( type == INT )
    {
        પાછા readLong(p);
    }
    નહીંતર જો( type == REAL )
    {
        પાછા cvRound(readReal(p));
    }
    નહીંતર
        પાછા 0x7fffffff;
}

File_ગાંઠ::operator દશાંશ() અચલ
{
    અચલ uchar* p = ptr();
    જો(!p)
        પાછા 0.f;
    પૂર્ણાંક tag = *p;
    પૂર્ણાંક type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    જો( type == INT )
    {
        પાછા (દશાંશ)readLong(p);
    }
    નહીંતર જો( type == REAL )
    {
        પાછા (દશાંશ)readReal(p);
    }
    નહીંતર
        પાછા FLT_MAX;
}

File_ગાંઠ::operator દ્વિગુણ() અચલ
{
    અચલ uchar* p = ptr();
    જો(!p)
        પાછા 0.f;
    પૂર્ણાંક tag = *p;
    પૂર્ણાંક type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    જો( type == INT )
    {
        પાછા (દ્વિગુણ)readLong(p);
    }
    નહીંતર જો( type == REAL )
    {
        પાછા readReal(p);
    }
    નહીંતર
        પાછા DBL_MAX;
}

દ્વિગુણ File_ગાંઠ::real() અચલ  { પાછા દ્વિગુણ(*આ); }

std::string File_ગાંઠ::string() અચલ
{
    અચલ uchar* p = ptr();
    જો( !p || (*p & TYPE_MASK) != STRING )
        પાછા std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(unsigned)readInt(p);
    પાછા std::string((અચલ અક્ષર*)(p + 4), sz - 1);
}
Mat File_ગાંઠ::mat() અચલ { Mat value; પઠ(*આ, value, Mat()); પાછા value; }

File_ગાંઠ_Iterator File_ગાંઠ::begin() અચલ { પાછા File_ગાંઠ_Iterator(*આ, ખોટું); }
File_ગાંઠ_Iterator File_ગાંઠ::end() અચલ   { પાછા File_ગાંઠ_Iterator(*આ, સાચું); }

શૂન્ય File_ગાંઠ::readRaw( અચલ std::string& fmt, શૂન્ય* vec, size_t len ) અચલ
{
    File_ગાંઠ_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_ગાંઠ::size() અચલ
{
    અચલ uchar* p = ptr();
    જો( !p )
        પાછા 0;
    પૂર્ણાંક tag = *p;
    પૂર્ણાંક tp = tag & TYPE_MASK;
    જો( tp == MAP || tp == SEQ )
    {
        જો( tag & NAMED )
            p += 4;
        પાછા (size_t)(unsigned)readInt(p + 5);
    }
    પાછા tp != NONE;
}

size_t File_ગાંઠ::rawSize() અચલ
{
    અચલ uchar* p0 = ptr(), *p = p0;
    જો( !p )
        પાછા 0;
    પૂર્ણાંક tag = *p++;
    પૂર્ણાંક tp = tag & TYPE_MASK;
    જો( tag & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    જો( tp == INT )
        પાછા sz0 + 8;
    જો( tp == REAL )
        પાછા sz0 + 8;
    જો( tp == NONE )
        પાછા sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    પાછા sz0 + 4 + readInt(p);
}

uchar* File_ગાંઠ::ptr()
{
    પાછા !fs ? 0 : (uchar*)fs->get_ગાંઠ_Ptr(blockIdx, ofs);
}

અચલ uchar* File_ગાંઠ::ptr() અચલ
{
    પાછા !fs ? 0 : fs->get_ગાંઠ_Ptr(blockIdx, ofs);
}

શૂન્ય File_ગાંઠ::set_કિંમત( પૂર્ણાંક type, અચલ શૂન્ય* value, પૂર્ણાંક len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    પૂર્ણાંક tag = *p;
    પૂર્ણાંક current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    પૂર્ણાંક sz = 1;

    જો( tag & NAMED )
        sz += 4;

    જો( type == INT )
        sz += 8;
    નહીંતર જો( type == REAL )
        sz += 8;
    નહીંતર જો( type == STRING )
    {
        જો( len < 0 )
            len = (પૂર્ણાંક)strlen((અચલ અક્ષર*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    નહીંતર
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_ગાંઠ_Space(*આ, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    જો( tag & NAMED )
        p += 4;

    જો( type == INT )
    {
        int64_t ival = *(અચલ int64_t*)value;
        writeInt(p, ival);
    }
    નહીંતર જો( type == REAL )
    {
        દ્વિગુણ dbval = *(અચલ દ્વિગુણ*)value;
        writeReal(p, dbval);
    }
    નહીંતર જો( type == STRING )
    {
        અચલ અક્ષર* str = (અચલ અક્ષર*)value;
        writeInt(p, len + 1);
        memcpy(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_ગાંઠ_Iterator::File_ગાંઠ_Iterator()
{
    fs = 0;
    blockIdx = 0;
    ofs = 0;
    blockSize = 0;
    ગાંઠ_NElems = 0;
    idx = 0;
}

File_ગાંઠ_Iterator::File_ગાંઠ_Iterator( અચલ File_ગાંઠ& ગાંઠ, તાર્કિક seekEnd )
{
    fs = ગાંઠ.fs;
    idx = 0;
    જો( !fs )
        blockIdx = ofs = blockSize = ગાંઠ_NElems = 0;
    નહીંતર
    {
        blockIdx = ગાંઠ.blockIdx;
        ofs = ગાંઠ.ofs;

        તાર્કિક collection = ગાંઠ.isSeq() || ગાંઠ.isMap();
        જો( ગાંઠ.isNone() )
        {
            ગાંઠ_NElems = 0;
        }
        નહીંતર જો( !collection )
        {
            ગાંઠ_NElems = 1;
            જો( seekEnd )
            {
                idx = 1;
                ofs += ગાંઠ.rawSize();
            }
        }
        નહીંતર
        {
            ગાંઠ_NElems = ગાંઠ.size();
            અચલ uchar* p0 = ગાંઠ.ptr(), *p = p0 + 1;
            જો(*p0 & File_ગાંઠ::NAMED )
                p += 4;
            જો( !seekEnd )
                ofs += (p - p0) + 8;
            નહીંતર
            {
                size_t rawsz = (size_t)(unsigned)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = ગાંઠ_NElems;
            }
        }
        fs->normalize_ગાંઠ_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
}

File_ગાંઠ_Iterator::File_ગાંઠ_Iterator(અચલ File_ગાંઠ_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    ગાંઠ_NElems = it.ગાંઠ_NElems;
    idx = it.idx;
}

File_ગાંઠ_Iterator& File_ગાંઠ_Iterator::operator=(અચલ File_ગાંઠ_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    ગાંઠ_NElems = it.ગાંઠ_NElems;
    idx = it.idx;
    પાછા *આ;
}

File_ગાંઠ File_ગાંઠ_Iterator::operator *() અચલ
{
    પાછા File_ગાંઠ(idx < ગાંઠ_NElems ? fs : ખાલી, blockIdx, ofs);
}

File_ગાંઠ_Iterator& File_ગાંઠ_Iterator::operator ++ ()
{
    જો( idx == ગાંઠ_NElems || !fs )
        પાછા *આ;
    idx++;
    File_ગાંઠ n(fs, blockIdx, ofs);
    ofs += n.rawSize();
    જો( ofs >= blockSize )
    {
        fs->normalize_ગાંઠ_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
    પાછા *આ;
}

File_ગાંઠ_Iterator File_ગાંઠ_Iterator::operator ++ (પૂર્ણાંક)
{
    File_ગાંઠ_Iterator it = *આ;
    ++(*આ);
    પાછા it;
}

File_ગાંઠ_Iterator& File_ગાંઠ_Iterator::operator += (પૂર્ણાંક _ofs)
{
    CV_Assert( _ofs >= 0 );
    માટે( ; _ofs > 0; _ofs-- )
        આ->operator ++();
    પાછા *આ;
}

File_ગાંઠ_Iterator& File_ગાંઠ_Iterator::readRaw( અચલ String& fmt, શૂન્ય* _data0, size_t maxsz)
{
    જો( fs && idx < ગાંઠ_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        પૂર્ણાંક fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        પૂર્ણાંક fmt_pair_ગણતરી = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        size_t esz = fs::calcStructSize( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        માટે( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            માટે( પૂર્ણાંક k = 0; k < fmt_pair_ગણતરી; k++ )
            {
                પૂર્ણાંક elem_type = fmt_pairs[k*2+1];
                પૂર્ણાંક elem_size = CV_ELEM_SIZE(elem_type);

                પૂર્ણાંક ગણતરી_2 = fmt_pairs[k*2];
                offset = alignSize( offset, elem_size );
                uchar* data = data0 + offset;

                માટે( પૂર્ણાંક i = 0; i < ગણતરી_2; i++, ++(*આ) )
                {
                    File_ગાંઠ ગાંઠ = *(*આ);
                    જો( ગાંઠ.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_size == 8 ? (int64_t)ગાંઠ : (પૂર્ણાંક)ગાંઠ);
                        પસંદગી( elem_type )
                        {
                        સ્થિતિ CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(ival);
                            data++;
                            તોડો;
                        સ્થિતિ CV_8S:
                            *(અક્ષર*)data = saturate_cast<schar>(ival);
                            data++;
                            તોડો;
                        સ્થિતિ CV_Bool:
                            *(તાર્કિક*)data = ival != 0;
                            data++;
                            તોડો;
                        સ્થિતિ CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(ival);
                            data += કદ(ushort);
                            તોડો;
                        સ્થિતિ CV_16S:
                            *(short*)data = saturate_cast<short>(ival);
                            data += કદ(short);
                            તોડો;
                        સ્થિતિ CV_32U:
                            *(unsigned*)data = (unsigned)std::max(ival, (int64_t)0);
                            data += કદ(unsigned);
                            તોડો;
                        સ્થિતિ CV_32S:
                            *(પૂર્ણાંક*)data = (પૂર્ણાંક)ival;
                            data += કદ(પૂર્ણાંક);
                            તોડો;
                        સ્થિતિ CV_32F:
                            *(દશાંશ*)data = (દશાંશ)ival;
                            data += કદ(દશાંશ);
                            તોડો;
                        સ્થિતિ CV_64U:
                            *(uint64_t*)data = (uint64_t)ival;
                            data += કદ(uint64_t);
                            તોડો;
                        સ્થિતિ CV_64S:
                            *(int64_t*)data = (int64_t)ival;
                            data += કદ(int64_t);
                            તોડો;
                        સ્થિતિ CV_64F:
                            *(દ્વિગુણ*)data = (દ્વિગુણ)ival;
                            data += કદ(દ્વિગુણ);
                            તોડો;
                        સ્થિતિ CV_16F:
                            *(hfloat*)data = hfloat((દશાંશ)ival);
                            data += કદ(hfloat);
                            તોડો;
                        સ્થિતિ CV_16BF:
                            *(bfloat*)data = bfloat((દશાંશ)ival);
                            data += કદ(bfloat);
                            તોડો;
                        સ્થિતિ CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((દશાંશ)ival);
                            data += કદ(fp8_t);
                            તોડો;
                        સ્થિતિ CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((દશાંશ)ival);
                            data += કદ(fp8a_t);
                            તોડો;
                        મૂળભૂત:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    નહીંતર જો( ગાંઠ.isReal() )
                    {
                        દ્વિગુણ fval = (દ્વિગુણ)ગાંઠ;

                        પસંદગી( elem_type )
                        {
                        સ્થિતિ CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(fval);
                            data++;
                            તોડો;
                        સ્થિતિ CV_8S:
                            *(અક્ષર*)data = saturate_cast<schar>(fval);
                            data++;
                            તોડો;
                        સ્થિતિ CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(fval);
                            data += કદ(ushort);
                            તોડો;
                        સ્થિતિ CV_16S:
                            *(short*)data = saturate_cast<short>(fval);
                            data += કદ(short);
                            તોડો;
                        સ્થિતિ CV_32U:
                            *(પૂર્ણાંક*)data = saturate_cast<unsigned>(fval);
                            data += કદ(પૂર્ણાંક);
                            તોડો;
                        સ્થિતિ CV_32S:
                            *(પૂર્ણાંક*)data = saturate_cast<પૂર્ણાંક>(fval);
                            data += કદ(પૂર્ણાંક);
                            તોડો;
                        સ્થિતિ CV_32F:
                            *(દશાંશ*)data = (દશાંશ)fval;
                            data += કદ(દશાંશ);
                            તોડો;
                        સ્થિતિ CV_64U:
                            *(uint64_t*)data = (uint64_t)round(std::max(fval, 0.));
                            data += કદ(uint64_t);
                            તોડો;
                        સ્થિતિ CV_64S:
                            *(int64_t*)data = (int64_t)round(std::max(fval, 0.));
                            data += કદ(int64_t);
                            તોડો;
                        સ્થિતિ CV_64F:
                            *(દ્વિગુણ*)data = fval;
                            data += કદ(દ્વિગુણ);
                            તોડો;
                        સ્થિતિ CV_16F:
                            *(hfloat*)data = hfloat((દશાંશ)fval);
                            data += કદ(hfloat);
                            તોડો;
                        સ્થિતિ CV_16BF:
                            *(bfloat*)data = bfloat((દશાંશ)fval);
                            data += કદ(bfloat);
                            તોડો;
                        સ્થિતિ CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((દશાંશ)fval);
                            data += કદ(fp8_t);
                            તોડો;
                        સ્થિતિ CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((દશાંશ)fval);
                            data += કદ(fp8a_t);
                            તોડો;
                        મૂળભૂત:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    નહીંતર
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (પૂર્ણાંક)(data - data0);
            }
        }
    }

    પાછા *આ;
}

તાર્કિક File_ગાંઠ_Iterator::equalTo(અચલ File_ગાંઠ_Iterator& it) અચલ
{
    પાછા fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && ગાંઠ_NElems == it.ગાંઠ_NElems;
}

size_t File_ગાંઠ_Iterator::remaining() અચલ
{
    પાછા ગાંઠ_NElems - idx;
}

તાર્કિક operator == ( અચલ File_ગાંઠ_Iterator& it1, અચલ File_ગાંઠ_Iterator& it2 )
{
    પાછા it1.equalTo(it2);
}

તાર્કિક operator != ( અચલ File_ગાંઠ_Iterator& it1, અચલ File_ગાંઠ_Iterator& it2 )
{
    પાછા !it1.equalTo(it2);
}

શૂન્ય પઠ(અચલ File_ગાંઠ& ગાંઠ, પૂર્ણાંક& val, પૂર્ણાંક default_val)
{
    val = default_val;
    જો( !ગાંઠ.empty() )
    {
        val = (પૂર્ણાંક)ગાંઠ;
    }
}

શૂન્ય પઠ(અચલ File_ગાંઠ& ગાંઠ, int64_t& val, int64_t default_val)
{
    val = default_val;
    જો( !ગાંઠ.empty() )
    {
        val = (int64_t)ગાંઠ;
    }
}

શૂન્ય પઠ(અચલ File_ગાંઠ& ગાંઠ, દ્વિગુણ& val, દ્વિગુણ default_val)
{
    val = default_val;
    જો( !ગાંઠ.empty() )
    {
        val = (દ્વિગુણ)ગાંઠ;
    }
}

શૂન્ય પઠ(અચલ File_ગાંઠ& ગાંઠ, દશાંશ& val, દશાંશ default_val)
{
    val = default_val;
    જો( !ગાંઠ.empty() )
    {
        val = (દશાંશ)ગાંઠ;
    }
}

શૂન્ય પઠ(અચલ File_ગાંઠ& ગાંઠ, std::string& val, અચલ std::string& default_val)
{
    val = default_val;
    જો( !ગાંઠ.empty() )
    {
        val = (std::string)ગાંઠ;
    }
}
શૂન્ય FileStorage::Impl::લિખ(અચલ String &ચાવી, તાર્કિક value)
{
    CV_Assert(write_mode);
    જો (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().લિખ(ચાવી.c_str(), (પૂર્ણાંક)value);
    }
    નહીંતર
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().લિખ(ચાવી.c_str(), value ? "true" : "false", ખોટું);
    }
}

FileStorage_API::~FileStorage_API() {}

નામાકાશ internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, અચલ std::string& name,
                                       પૂર્ણાંક flags, અચલ std::string& typeName)
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
