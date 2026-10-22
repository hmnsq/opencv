// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "persistence.hpp"
#അന്തര്ഭാവ "persistence_impl.hpp"
#അന്തര്ഭാവ "persistence_base64_encoding.hpp"
#അന്തര്ഭാവ <unordered_map>
#അന്തര്ഭാവ <iterator>

#അന്തര്ഭാവ <opencv2/core/utils/logger.hpp>

#define CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

നാമാകാശ cv
{

നാമാകാശ fs
{

പൂർണ്ണസംഖ്യ strcasecmp(സ്ഥിരം അക്ഷരം* s1, സ്ഥിരം അക്ഷരം* s2)
{
    സ്ഥിരം അക്ഷരം* dummy="";
    എങ്കിൽ(!s1) s1=dummy;
    എങ്കിൽ(!s2) s2=dummy;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t i, len = std::min(len1, len2);
    വേണ്ടി( i = 0; i < len; i++ )
    {
        പൂർണ്ണസംഖ്യ d = tolower((പൂർണ്ണസംഖ്യ)s1[i]) - tolower((പൂർണ്ണസംഖ്യ)s2[i]);
        എങ്കിൽ( d != 0 )
            തിരികെ d;
    }
    തിരികെ len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

അക്ഷരം* itoa( പൂർണ്ണസംഖ്യ _val, അക്ഷരം* buffer, പൂർണ്ണസംഖ്യ /*radix*/ )
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ radix = 10;
    അക്ഷരം* ptr=buffer + 23 /* enough even for 64-bit integers */;
    unsigned val = abs(_val);

    *ptr = '\0';
    ചെയ്യുക
    {
        unsigned r = val / radix;
        *--ptr = (അക്ഷരം)(val - (r*radix) + '0');
        val = r;
    }
    ആകുമ്പോൾ( val != 0 );

    എങ്കിൽ( _val < 0 )
        *--ptr = '-';

    തിരികെ ptr;
}

അക്ഷരം* itoa( int64_t _val, അക്ഷരം* buffer, പൂർണ്ണസംഖ്യ /*radix*/, തർക്കം _signed)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ radix = 10;
    അക്ഷരം* ptr=buffer + 23 /* enough even for 64-bit integers */;
    പൂർണ്ണസംഖ്യ sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : abs(_val);

    *ptr = '\0';
    ചെയ്യുക
    {
        uint64_t r = val / radix;
        *--ptr = (അക്ഷരം)(val - (r*radix) + '0');
        val = r;
    }
    ആകുമ്പോൾ( val != 0 );

    എങ്കിൽ( sign < 0 )
        *--ptr = '-';

    തിരികെ ptr;
}

അക്ഷരം* doubleToString( അക്ഷരം* buf, size_t bufSize, ഇരട്ട value, തർക്കം explicitZero )
{
    Cv64suf val;
    unsigned ieee754_hi;

    val.f = value;
    ieee754_hi = (unsigned)(val.u >> 32);

    എങ്കിൽ( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        പൂർണ്ണസംഖ്യ ivalue = cvRound(value);
        എങ്കിൽ( ivalue == value )
        {
            എങ്കിൽ( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            അല്ലെങ്കിൽ
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        അല്ലെങ്കിൽ
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, bufSize, "%.17g", value );

            അക്ഷരം* ptr = buf;
            എങ്കിൽ( *ptr == '+' || *ptr == '-' )
                ptr++;
            വേണ്ടി( ; cv_isdigit(*ptr); ptr++ )
                ;
            എങ്കിൽ( *ptr == ',' )
                *ptr = '.';
        }
    }
    അല്ലെങ്കിൽ
    {
        unsigned ieee754_lo = (unsigned)val.u;
        എങ്കിൽ( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        അല്ലെങ്കിൽ
            strcpy( buf, (പൂർണ്ണസംഖ്യ)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    തിരികെ buf;
}

അക്ഷരം* floatToString( അക്ഷരം* buf, size_t bufSize, ദശാംശം value, തർക്കം halfprecision, തർക്കം explicitZero )
{
    Cv32suf val;
    unsigned ieee754;
    val.f = value;
    ieee754 = val.u;

    എങ്കിൽ( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        പൂർണ്ണസംഖ്യ ivalue = cvRound(value);
        എങ്കിൽ( ivalue == value )
        {
            എങ്കിൽ( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            അല്ലെങ്കിൽ
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        അല്ലെങ്കിൽ
        {
            എങ്കിൽ (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.5g", value);
            }
            അല്ലെങ്കിൽ
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.9g", value);
            }

            അക്ഷരം* ptr = buf;
            എങ്കിൽ( *ptr == '+' || *ptr == '-' )
                ptr++;
            വേണ്ടി( ; cv_isdigit(*ptr); ptr++ )
                ;
            എങ്കിൽ( *ptr == ',' )
                *ptr = '.';
        }
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        അല്ലെങ്കിൽ
            strcpy( buf, (പൂർണ്ണസംഖ്യ)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    തിരികെ buf;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം symbols[] = "ucwsifdhHbUIneE";

സ്ഥിതം അക്ഷരം type_ചിഹ്നം(പൂർണ്ണസംഖ്യ depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    തിരികെ symbols[depth];
}

സ്ഥിതം പൂർണ്ണസംഖ്യ ചിഹ്നം_To_Type(അക്ഷരം c)
{
    എങ്കിൽ (c == 'r')
        തിരികെ CV_SEQ_ELTYPE_PTR;
    സ്ഥിരം അക്ഷരം* pos = strchr( symbols, c );
    എങ്കിൽ( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    തിരികെ static_cast<പൂർണ്ണസംഖ്യ>(pos - symbols);
}

അക്ഷരം* encodeFormat(പൂർണ്ണസംഖ്യ elem_type, അക്ഷരം* dt, size_t dt_len)
{
    പൂർണ്ണസംഖ്യ cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    അക്ഷരം ചിഹ്നം = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_ചിഹ്നം(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, ചിഹ്നം);
    തിരികെ dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
അക്ഷരം* encodeFormat(പൂർണ്ണസംഖ്യ elem_type, അക്ഷരം* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    തിരികെ encodeFormat(elem_type, dt, max);
}

പൂർണ്ണസംഖ്യ decodeFormat( സ്ഥിരം അക്ഷരം* dt, പൂർണ്ണസംഖ്യ* fmt_pairs, പൂർണ്ണസംഖ്യ max_len )
{
    പൂർണ്ണസംഖ്യ fmt_pair_എണ്ണം = 0;
    പൂർണ്ണസംഖ്യ i = 0, k = 0, len = dt ? (പൂർണ്ണസംഖ്യ)strlen(dt) : 0;

    എങ്കിൽ( !dt || !len )
        തിരികെ 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    വേണ്ടി( ; k < len; k++ )
    {
        അക്ഷരം c = dt[k];

        എങ്കിൽ( cv_isdigit(c) )
        {
            പൂർണ്ണസംഖ്യ എണ്ണം_2 = c - '0';
            എങ്കിൽ( cv_isdigit(dt[k+1]) )
            {
                അക്ഷരം* endptr = 0;
                എണ്ണം_2 = (പൂർണ്ണസംഖ്യ)strtol( dt+k, &endptr, 10 );
                k = (പൂർണ്ണസംഖ്യ)(endptr - dt) - 1;
            }

            എങ്കിൽ( എണ്ണം_2 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = എണ്ണം_2;
        }
        അല്ലെങ്കിൽ
        {
            പൂർണ്ണസംഖ്യ depth = ചിഹ്നം_To_Type(c);
            എങ്കിൽ( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = depth;
            എങ്കിൽ( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            അല്ലെങ്കിൽ
            {
                i += 2;
                എങ്കിൽ( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_എണ്ണം = i/2;
    തിരികെ fmt_pair_എണ്ണം;
}

പൂർണ്ണസംഖ്യ calcElemSize( സ്ഥിരം അക്ഷരം* dt, പൂർണ്ണസംഖ്യ initial_size )
{
    പൂർണ്ണസംഖ്യ size = 0;
    പൂർണ്ണസംഖ്യ fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_എണ്ണം;
    പൂർണ്ണസംഖ്യ comp_size;

    fmt_pair_എണ്ണം = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_എണ്ണം *= 2;
    വേണ്ടി( i = 0, size = initial_size; i < fmt_pair_എണ്ണം; i += 2 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[i+1]);
        size = cvAlign( size, comp_size );
        size += comp_size * fmt_pairs[i];
    }
    എങ്കിൽ( initial_size == 0 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[1]);
        size = cvAlign( size, comp_size );
    }
    തിരികെ size;
}


പൂർണ്ണസംഖ്യ calcStructSize( സ്ഥിരം അക്ഷരം* dt, പൂർണ്ണസംഖ്യ initial_size )
{
    പൂർണ്ണസംഖ്യ size = calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    വേണ്ടി ( സ്ഥിരം അക്ഷരം * type = dt; *type != '\0'; type++ )
    {
        അക്ഷരം v = *type;
        എങ്കിൽ (v >= '0' && v <= '9')
            തുടരുക;  // skip vector size
        തിരഞ്ഞെടുപ്പ് (v)
        {
        അവസ്ഥ 'u': { elem_max_size = std::max( elem_max_size, വലുപ്പം(uchar ) ); നിർത്തുക; }
        അവസ്ഥ 'b': { elem_max_size = std::max( elem_max_size, വലുപ്പം(തർക്കം  ) ); നിർത്തുക; }
        അവസ്ഥ 'c': { elem_max_size = std::max( elem_max_size, വലുപ്പം(schar ) ); നിർത്തുക; }
        അവസ്ഥ 'w': { elem_max_size = std::max( elem_max_size, വലുപ്പം(ushort) ); നിർത്തുക; }
        അവസ്ഥ 's': { elem_max_size = std::max( elem_max_size, വലുപ്പം(short ) ); നിർത്തുക; }
        അവസ്ഥ 'i': { elem_max_size = std::max( elem_max_size, വലുപ്പം(പൂർണ്ണസംഖ്യ   ) ); നിർത്തുക; }
        അവസ്ഥ 'n': { elem_max_size = std::max( elem_max_size, വലുപ്പം(unsigned) ); നിർത്തുക; }
        അവസ്ഥ 'f': { elem_max_size = std::max( elem_max_size, വലുപ്പം(ദശാംശം ) ); നിർത്തുക; }
        അവസ്ഥ 'd': { elem_max_size = std::max( elem_max_size, വലുപ്പം(ഇരട്ട) ); നിർത്തുക; }
        അവസ്ഥ 'h': { elem_max_size = std::max( elem_max_size, വലുപ്പം(hfloat)); നിർത്തുക; }
        അവസ്ഥ 'H': { elem_max_size = std::max( elem_max_size, വലുപ്പം(bfloat)); നിർത്തുക; }
        അവസ്ഥ 'e': { elem_max_size = std::max( elem_max_size, വലുപ്പം(fp8_t) ); നിർത്തുക; }
        അവസ്ഥ 'E': { elem_max_size = std::max( elem_max_size, വലുപ്പം(fp8a_t)); നിർത്തുക; }
        അവസ്ഥ 'I': { elem_max_size = std::max( elem_max_size, വലുപ്പം(int64_t)); നിർത്തുക; }
        അവസ്ഥ 'U': { elem_max_size = std::max( elem_max_size, വലുപ്പം(uint64_t)); നിർത്തുക; }
        സ്ഥിരസ്ഥിതി:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (അക്ഷരം)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<പൂർണ്ണസംഖ്യ>(elem_max_size) );
    തിരികെ size;
}

പൂർണ്ണസംഖ്യ decodeSimpleFormat( സ്ഥിരം അക്ഷരം* dt )
{
    പൂർണ്ണസംഖ്യ elem_type = -1;
    പൂർണ്ണസംഖ്യ fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_എണ്ണം;

    fmt_pair_എണ്ണം = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    എങ്കിൽ( fmt_pair_എണ്ണം != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    തിരികെ elem_type;
}

}

#എങ്കിൽ defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#define CV_LITTLE_ENDIAN_MEM_ACCESS 1
#അല്ലെങ്കിൽ
#define CV_LITTLE_ENDIAN_MEM_ACCESS 0
#endif

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ readInt(സ്ഥിരം uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#എങ്കിൽ CV_LITTLE_ENDIAN_MEM_ACCESS
    പൂർണ്ണസംഖ്യ val;
    memcpy(&val, p, വലുപ്പം(val));
    തിരികെ val;
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ val = (പൂർണ്ണസംഖ്യ)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    തിരികെ val;
#endif
}

സ്ഥിതം വരിയിൽ int64_t readLong(സ്ഥിരം uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#എങ്കിൽ CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    memcpy(&val, p, വലുപ്പം(val));
    തിരികെ val;
#അല്ലെങ്കിൽ
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    തിരികെ val0 | ((int64_t)val1 << 32);
#endif
}

സ്ഥിതം വരിയിൽ ഇരട്ട readReal(സ്ഥിരം uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#എങ്കിൽ CV_LITTLE_ENDIAN_MEM_ACCESS
    ഇരട്ട val;
    memcpy(&val, p, വലുപ്പം(val));
    തിരികെ val;
#അല്ലെങ്കിൽ
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    തിരികെ val.f;
#endif
}

template <typename T>
സ്ഥിതം വരിയിൽ ശൂന്യം writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#എങ്കിൽ CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &ival, വലുപ്പം(ival));
#അല്ലെങ്കിൽ
    വേണ്ടി (size_t i = 0, j = 0; i < വലുപ്പം(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#endif
}

സ്ഥിതം വരിയിൽ ശൂന്യം writeReal(uchar* p, ഇരട്ട fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#എങ്കിൽ CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &fval, വലുപ്പം(fval));
#അല്ലെങ്കിൽ
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



ശൂന്യം FileStorage::Impl::init() {
    flags = 0;
    buffer.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = അസത്യം;
    state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = അസത്യം;
    delayed_struct_താക്കോൽ = nullptr;
    delayed_struct_flags = 0;
    delayed_type_name = nullptr;
    base64_writer = nullptr;
    is_opened = അസത്യം;
    dummy_eof = അസത്യം;
    write_mode = അസത്യം;
    mem_mode = അസത്യം;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    empty_stream = സത്യം;

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

ശൂന്യം FileStorage::Impl::release(String *out) {
    എങ്കിൽ (is_opened) {
        എങ്കിൽ (out)
            out->clear();
        എങ്കിൽ (write_mode) {
            ആകുമ്പോൾ (write_അട്ടി.size() > 1) {
                endWriteStruct();
            }
            flush();
            എങ്കിൽ (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            അല്ലെങ്കിൽ എങ്കിൽ (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        എങ്കിൽ (mem_mode && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

ശൂന്യം FileStorage::Impl::analyze_file_name(സ്ഥിരം std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    സ്ഥിതം സ്ഥിരം അക്ഷരം not_file_name = '\n';
    സ്ഥിതം സ്ഥിരം അക്ഷരം parameter_begin = '?';
    സ്ഥിതം സ്ഥിരം അക്ഷരം parameter_separator = '&';

    എങ്കിൽ (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        തിരികെ;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    എങ്കിൽ (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        വേണ്ടി (size_t param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            എങ്കിൽ ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

തർക്കം FileStorage::Impl::open(സ്ഥിരം അക്ഷരം *filename_or_buf, പൂർണ്ണസംഖ്യ _flags, സ്ഥിരം അക്ഷരം *encoding) {
    തർക്കം ok = സത്യം;
    release();

    തർക്കം append = (_flags & 3) == FileStorage::APPEND;
    mem_mode = (_flags & FileStorage::MEMORY) != 0;

    write_mode = (_flags & 3) != 0;
    തർക്കം write_base64 = (write_mode || append) && (_flags & FileStorage::BASE64) != 0;

    തർക്കം isGZ = അസത്യം;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        എങ്കിൽ (!params.empty())
            filename = params[0];

        എങ്കിൽ (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_mode || append);
    }

    എങ്കിൽ (filename.size() == 0 && !mem_mode && !write_mode)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    എങ്കിൽ (mem_mode && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    എങ്കിൽ (!mem_mode) {
        size_t dot_idx = filename.find_last_of('.');
        അക്ഷരം compression = '\0';

        എങ്കിൽ (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            എങ്കിൽ (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                എങ്കിൽ (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = സത്യം;
                }

                അല്ലെങ്കിൽ എങ്കിൽ(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = സത്യം;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        എങ്കിൽ (!isGZ) {
            file = fopen(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            എങ്കിൽ (!file)
            {
                CV_LOG_ERROR(ശൂന്യ, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                തിരികെ അസത്യം;
            }
        } അല്ലെങ്കിൽ {
#എങ്കിൽ USE_ZLIB
            എങ്കിൽ (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            അക്ഷരം mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            എങ്കിൽ (!gzfile)
            {
                CV_LOG_ERROR(ശൂന്യ, "Can't open archive: '" << filename << "' mode=" << mode);
                തിരികെ അസത്യം;
            }
#അല്ലെങ്കിൽ
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#endif
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_data.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    എങ്കിൽ (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        എങ്കിൽ (mem_mode)
            outbuf.clear();

        എങ്കിൽ (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            സ്ഥിരം അക്ഷരം *dot_pos = ശൂന്യ;
            സ്ഥിരം അക്ഷരം *dot_pos2 = ശൂന്യ;
            // like strrchr() implementation, but save two last positions simultaneously
            വേണ്ടി (സ്ഥിരം അക്ഷരം *pos = &filename[0]; pos[0] != 0; pos++) {
                എങ്കിൽ (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            എങ്കിൽ (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != ശൂന്യ) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } അല്ലെങ്കിൽ എങ്കിൽ (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        പൂർണ്ണസംഖ്യ buf_size = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        എങ്കിൽ (append) {
            fseek(file, 0, SEEK_END);
            എങ്കിൽ (ftell(file) == 0)
                append = അസത്യം;
        }

        write_അട്ടി.clear();
        empty_stream = സത്യം;
        write_അട്ടി.push_back(FStructData("", File_കണ്ണി::MAP | File_കണ്ണി::EMPTY, 0));
        buffer.reserve(buf_size + 1024);
        buffer.resize(buf_size);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;

        എങ്കിൽ (fmt == FileStorage::FORMAT_XML) {
            size_t file_size = file ? (size_t) ftell(file) : (size_t) 0;
            എങ്കിൽ (!append || file_size == 0) {
                എങ്കിൽ (encoding && *encoding != '\0') {
                    എങ്കിൽ (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(strlen(encoding) < 1000);
                    അക്ഷരം buf[1100];
                    snprintf(buf, വലുപ്പം(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } അല്ലെങ്കിൽ
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } അല്ലെങ്കിൽ {
                പൂർണ്ണസംഖ്യ xml_buf_size = 1 << 10;
                അക്ഷരം substr[] = "</opencv_storage>";
                പൂർണ്ണസംഖ്യ last_occurrence = -1;
                xml_buf_size = MIN(xml_buf_size, പൂർണ്ണസംഖ്യ(file_size));
                fseek(file, -xml_buf_size, SEEK_END);
                // find the last occurrence of </opencv_storage>
                വേണ്ടി (;;) {
                    പൂർണ്ണസംഖ്യ വരി_offset = (പൂർണ്ണസംഖ്യ) ftell(file);
                    സ്ഥിരം അക്ഷരം *ptr0 = ഇത്->gets(xml_buf_size);
                    സ്ഥിരം അക്ഷരം *ptr = ശൂന്യ;
                    എങ്കിൽ (!ptr0)
                        നിർത്തുക;
                    ptr = ptr0;
                    വേണ്ടി (;;) {
                        ptr = strstr(ptr, substr);
                        എങ്കിൽ (!ptr)
                            നിർത്തുക;
                        last_occurrence = വരി_offset + (പൂർണ്ണസംഖ്യ) (ptr - ptr0);
                        ptr += strlen(substr);
                    }
                }
                എങ്കിൽ (last_occurrence < 0) {
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

            emitter_do_not_use_direct_dereference = createXMLEmitter(ഇത്);
        } അല്ലെങ്കിൽ എങ്കിൽ (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                എങ്കിൽ (!append) {
                    എങ്കിൽ (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    അല്ലെങ്കിൽ
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            അല്ലെങ്കിൽ
                puts("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(ഇത്);
        } അല്ലെങ്കിൽ {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            എങ്കിൽ (!append)
                puts("{\n");
            അല്ലെങ്കിൽ {
                തർക്കം valid = അസത്യം;
                long roffset = 0;
                വേണ്ടി (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    സ്ഥിരം അക്ഷരം end_mark = '}';
                    എങ്കിൽ (fgetc(file) == end_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = സത്യം;
                        നിർത്തുക;
                    }
                }

                എങ്കിൽ (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } അല്ലെങ്കിൽ {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_അട്ടി.back().indent = 4;
            emitter_do_not_use_direct_dereference = createJSONEmitter(ഇത്);
        }
        is_opened = സത്യം;
    } അല്ലെങ്കിൽ {
        സ്ഥിരം size_t buf_size0 = 40;
        buffer.resize(buf_size0);
        എങ്കിൽ (mem_mode) {
            strbuf = (അക്ഷരം *) filename_or_buf;
            strbufsize = strlen(strbuf);
        }

        സ്ഥിരം അക്ഷരം *yaml_signature = "%YAML";
        സ്ഥിരം അക്ഷരം *json_signature = "{";
        സ്ഥിരം അക്ഷരം *xml_signature = "<?xml";
        അക്ഷരം *buf = ഇത്->gets(16);
        CV_Assert(buf);
        അക്ഷരം *bufPtr = cv_skip_BOM(buf);
        size_t bufOffset = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        എങ്കിൽ (strncmp(bufPtr, yaml_signature, strlen(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        അല്ലെങ്കിൽ എങ്കിൽ (strncmp(bufPtr, json_signature, strlen(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        അല്ലെങ്കിൽ എങ്കിൽ (strncmp(bufPtr, xml_signature, strlen(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        അല്ലെങ്കിൽ
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = bufOffset;
        bufofs = 0;

        ശ്രമിക്കുക {
            അക്ഷരം *ptr = bufferStart();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_കണ്ണി വേര്_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_കണ്ണി_Space(വേര്_nodes, 9);
            *rptr = File_കണ്ണി::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            തിരഞ്ഞെടുപ്പ് (fmt) {
                അവസ്ഥ FileStorage::FORMAT_XML:
                    parser_do_not_use_direct_dereference = createXMLParser(ഇത്);
                    നിർത്തുക;
                അവസ്ഥ FileStorage::FORMAT_YAML:
                അവസ്ഥ FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_direct_dereference = createYAMLParser(ഇത്);
                    നിർത്തുക;
                അവസ്ഥ FileStorage::FORMAT_JSON:
                    parser_do_not_use_direct_dereference = createJSONParser(ഇത്);
                    നിർത്തുക;
                സ്ഥിരസ്ഥിതി:
                    parser_do_not_use_direct_dereference = Ptr<FileStorageParser>();
            }

            എങ്കിൽ (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                എങ്കിൽ (ok) {
                    finalizeCollection(വേര്_nodes);

                    CV_Assert(!fs_data_ptrs.empty());
                    File_കണ്ണി roots_കണ്ണി(fs_ext, 0, 0);
                    size_t i, nroots = roots_കണ്ണി.size();
                    File_കണ്ണി_Iterator it = roots_കണ്ണി.begin();

                    വേണ്ടി (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        പിടിക്കുക (...)
        {
            // FIXIT log error message
            is_opened = സത്യം;
            release();
            എറിയുക;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = സത്യം;
        std::vector<അക്ഷരം> tmpbuf;
        std::swap(buffer, tmpbuf);
        bufofs = 0;
    }
    തിരികെ ok;
}

ശൂന്യം FileStorage::Impl::puts(സ്ഥിരം അക്ഷരം *str) {
    CV_Assert(write_mode);
    എങ്കിൽ (mem_mode)
        std::copy(str, str + strlen(str), std::back_inserter(outbuf));
    അല്ലെങ്കിൽ എങ്കിൽ (file)
        fputs(str, file);
#എങ്കിൽ USE_ZLIB
    അല്ലെങ്കിൽ എങ്കിൽ (gzfile)
        gzputs(gzfile, str);
#endif
    അല്ലെങ്കിൽ
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

അക്ഷരം *FileStorage::Impl::getsFromFile(അക്ഷരം *buf, പൂർണ്ണസംഖ്യ എണ്ണം_2) {
    എങ്കിൽ (file)
        തിരികെ fgets(buf, എണ്ണം_2, file);
#എങ്കിൽ USE_ZLIB
    എങ്കിൽ (gzfile)
        തിരികെ gzgets(gzfile, buf, എണ്ണം_2);
#endif
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

അക്ഷരം *FileStorage::Impl::gets(size_t max_എണ്ണം) {
    എങ്കിൽ (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        സ്ഥിരം അക്ഷരം *instr = strbuf;
        വേണ്ടി (; i < len; i++) {
            അക്ഷരം c = instr[i];
            എങ്കിൽ (c == '\0' || c == '\n') {
                എങ്കിൽ (c == '\n')
                    i++;
                നിർത്തുക;
            }
        }
        size_t എണ്ണം_2 = i - strbufpos;
        എങ്കിൽ (max_എണ്ണം == 0 || max_എണ്ണം > എണ്ണം_2)
            max_എണ്ണം = എണ്ണം_2;
        buffer.resize(std::max(buffer.size(), max_എണ്ണം + 8));
        memcpy(&buffer[0], instr + strbufpos, max_എണ്ണം);
        buffer[max_എണ്ണം] = '\0';
        strbufpos = i;
        തിരികെ max_എണ്ണം > 0 ? &buffer[0] : 0;
    }

    സ്ഥിരം size_t MAX_BLOCK_SIZE = INT_MAX / 2; // hopefully, that will be enough
    എങ്കിൽ (max_എണ്ണം == 0)
        max_എണ്ണം = MAX_BLOCK_SIZE;
    അല്ലെങ്കിൽ
        CV_Assert(max_എണ്ണം < MAX_BLOCK_SIZE);
    size_t ofs = 0;

    വേണ്ടി (;;) {
        പൂർണ്ണസംഖ്യ എണ്ണം_2 = (പൂർണ്ണസംഖ്യ) std::min(buffer.size() - ofs - 16, max_എണ്ണം);
        അക്ഷരം *ptr = getsFromFile(&buffer[ofs], എണ്ണം_2 + 1);
        എങ്കിൽ (!ptr)
            നിർത്തുക;
        പൂർണ്ണസംഖ്യ delta = (പൂർണ്ണസംഖ്യ) strlen(ptr);
        ofs += delta;
        max_എണ്ണം -= delta;
        എങ്കിൽ (delta == 0 || ptr[delta - 1] == '\n' || max_എണ്ണം == 0)
            നിർത്തുക;
        എങ്കിൽ (delta == എണ്ണം_2)
            buffer.resize((size_t) (buffer.size() * 1.5));
    }
    തിരികെ ofs > 0 ? &buffer[0] : 0;
}

അക്ഷരം *FileStorage::Impl::gets() {
    അക്ഷരം *ptr = ഇത്->gets(0);
    എങ്കിൽ (!ptr) {
        ptr = bufferStart();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        തിരികെ 0;
    } അല്ലെങ്കിൽ {
        size_t l = strlen(ptr);
        എങ്കിൽ (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    തിരികെ ptr;
}

തർക്കം FileStorage::Impl::eof() {
    എങ്കിൽ (dummy_eof)
        തിരികെ സത്യം;
    എങ്കിൽ (strbuf)
        തിരികെ strbufpos >= strbufsize;
    എങ്കിൽ (file)
        തിരികെ feof(file) != 0;
#എങ്കിൽ USE_ZLIB
    എങ്കിൽ (gzfile)
        തിരികെ gzeof(gzfile) != 0;
#endif
    തിരികെ അസത്യം;
}

ശൂന്യം FileStorage::Impl::setEof() {
    dummy_eof = സത്യം;
}

ശൂന്യം FileStorage::Impl::closeFile() {
    എങ്കിൽ (file)
        fclose(file);
#എങ്കിൽ USE_ZLIB
    അല്ലെങ്കിൽ എങ്കിൽ (gzfile)
        gzclose(gzfile);
#endif
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = അസത്യം;
}

ശൂന്യം FileStorage::Impl::rewind() {
    എങ്കിൽ (file)
        ::rewind(file);
#എങ്കിൽ USE_ZLIB
    അല്ലെങ്കിൽ എങ്കിൽ (gzfile)
        gzrewind(gzfile);
#endif
    strbufpos = 0;
}

അക്ഷരം *FileStorage::Impl::resizeWriteBuffer(അക്ഷരം *ptr, പൂർണ്ണസംഖ്യ len) {
    സ്ഥിരം അക്ഷരം *buffer_end = &buffer[0] + buffer.size();
    എങ്കിൽ (ptr + len < buffer_end)
        തിരികെ ptr;

    സ്ഥിരം അക്ഷരം *buffer_start = &buffer[0];
    പൂർണ്ണസംഖ്യ written_len = (പൂർണ്ണസംഖ്യ) (ptr - buffer_start);

    CV_Assert(written_len <= (പൂർണ്ണസംഖ്യ) buffer.size());
    പൂർണ്ണസംഖ്യ new_size = (പൂർണ്ണസംഖ്യ) ((buffer_end - buffer_start) * 3 / 2);
    new_size = MAX(written_len + len, new_size);
    buffer.reserve(new_size + 256);
    buffer.resize(new_size);
    bufofs = written_len;
    തിരികെ &buffer[0] + bufofs;
}

അക്ഷരം *FileStorage::Impl::flush() {
    അക്ഷരം *buffer_start = &buffer[0];
    അക്ഷരം *ptr = buffer_start + bufofs;

    എങ്കിൽ (ptr > buffer_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(buffer_start);
        bufofs = 0;
    }

    പൂർണ്ണസംഖ്യ indent = write_അട്ടി.back().indent;

    എങ്കിൽ (space != indent) {
        memset(buffer_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = buffer_start + bufofs;

    തിരികെ ptr;
}

ശൂന്യം FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(അസത്യം);
    എങ്കിൽ (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_അട്ടി.empty());

    FStructData &current_struct = write_അട്ടി.back();
    എങ്കിൽ (fmt == FileStorage::FORMAT_JSON && !File_കണ്ണി::isFlow(current_struct.flags) && write_അട്ടി.size() > 1)
        current_struct.indent = write_അട്ടി[write_അട്ടി.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_അട്ടി.pop_back();
    എങ്കിൽ (!write_അട്ടി.empty())
        write_അട്ടി.back().flags &= ~File_കണ്ണി::EMPTY;
}

ശൂന്യം FileStorage::Impl::startWriteStruct_helper(സ്ഥിരം അക്ഷരം *താക്കോൽ, പൂർണ്ണസംഖ്യ struct_flags,
                                                സ്ഥിരം അക്ഷരം *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (File_കണ്ണി::TYPE_MASK | File_കണ്ണി::FLOW)) | File_കണ്ണി::EMPTY;
    എങ്കിൽ (!File_കണ്ണി::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    എങ്കിൽ (type_name && type_name[0] == '\0')
        type_name = 0;

    FStructData s = getEmitter().startWriteStruct(write_അട്ടി.back(), താക്കോൽ, struct_flags, type_name);

    write_അട്ടി.push_back(s);
    size_t write_അട്ടി_size = write_അട്ടി.size();
    എങ്കിൽ (write_അട്ടി_size > 1)
        write_അട്ടി[write_അട്ടി_size - 2].flags &= ~File_കണ്ണി::EMPTY;

    എങ്കിൽ (fmt != FileStorage::FORMAT_JSON && !File_കണ്ണി::isFlow(s.flags))
        flush();

    എങ്കിൽ (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_കണ്ണി::isMap(struct_flags)) {
        getEmitter().ലിഖ("type_id", type_name, അസത്യം);
    }
}

ശൂന്യം FileStorage::Impl::startWriteStruct(സ്ഥിരം അക്ഷരം *താക്കോൽ, പൂർണ്ണസംഖ്യ struct_flags,
                                         സ്ഥിരം അക്ഷരം *type_name) {
    check_if_write_struct_is_delayed(അസത്യം);
    എങ്കിൽ (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    എങ്കിൽ (state_of_writing_base64 == FileStorage_API::Uncertain && File_കണ്ണി::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(താക്കോൽ, struct_flags, type_name);
    } അല്ലെങ്കിൽ എങ്കിൽ (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        എങ്കിൽ ((File_കണ്ണി::TYPE_MASK & struct_flags) != File_കണ്ണി::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        അല്ലെങ്കിൽ എങ്കിൽ (state_of_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(താക്കോൽ, struct_flags, "binary");

        എങ്കിൽ (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::InUse);
    } അല്ലെങ്കിൽ {
        /* Won't output Base64 data */
        എങ്കിൽ (state_of_writing_base64 == FileStorage_API::InUse)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(താക്കോൽ, struct_flags, type_name);

        എങ്കിൽ (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::NotUse);
    }
}

ശൂന്യം FileStorage::Impl::writeComment(സ്ഥിരം അക്ഷരം *comment, തർക്കം eol_comment) {
    CV_Assert(write_mode);
    getEmitter().writeComment(comment, eol_comment);
}

ശൂന്യം FileStorage::Impl::startNextStream() {
    CV_Assert(write_mode);
    എങ്കിൽ (!empty_stream) {
        ആകുമ്പോൾ (!write_അട്ടി.empty())
            endWriteStruct();
        flush();
        getEmitter().startNextStream();
        empty_stream = സത്യം;
        write_അട്ടി.push_back(FStructData("", File_കണ്ണി::EMPTY, 0));
        bufofs = 0;
    }
}

ശൂന്യം FileStorage::Impl::ലിഖ(സ്ഥിരം String &താക്കോൽ, പൂർണ്ണസംഖ്യ value) {
    CV_Assert(write_mode);
    getEmitter().ലിഖ(താക്കോൽ.c_str(), value);
}

ശൂന്യം FileStorage::Impl::ലിഖ(സ്ഥിരം String &താക്കോൽ, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().ലിഖ(താക്കോൽ.c_str(), value);
}

ശൂന്യം FileStorage::Impl::ലിഖ(സ്ഥിരം String &താക്കോൽ, ഇരട്ട value) {
    CV_Assert(write_mode);
    getEmitter().ലിഖ(താക്കോൽ.c_str(), value);
}

ശൂന്യം FileStorage::Impl::ലിഖ(സ്ഥിരം String &താക്കോൽ, സ്ഥിരം String &value) {
    CV_Assert(write_mode);
    getEmitter().ലിഖ(താക്കോൽ.c_str(), value.c_str(), അസത്യം);
}

ശൂന്യം FileStorage::Impl::writeRawData(സ്ഥിരം std::string &dt, സ്ഥിരം ശൂന്യം *_data, size_t len) {
    CV_Assert(write_mode);

    എങ്കിൽ (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        writeRawDataBase64(_data, len, dt.c_str());
        തിരികെ;
    } അല്ലെങ്കിൽ എങ്കിൽ (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elemSize = fs::calcStructSize(dt.c_str(), 0);
    CV_Assert(elemSize);
    CV_Assert(len % elemSize == 0);
    len /= elemSize;

    തർക്കം explicitZero = fmt == FileStorage::FORMAT_JSON;
    സ്ഥിരം uchar *data0 = (സ്ഥിരം uchar *) _data;
    പൂർണ്ണസംഖ്യ fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_എണ്ണം;
    അക്ഷരം buf[256] = "";

    fmt_pair_എണ്ണം = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    എങ്കിൽ (!len)
        തിരികെ;

    എങ്കിൽ (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    എങ്കിൽ (fmt_pair_എണ്ണം == 1) {
        fmt_pairs[0] *= (പൂർണ്ണസംഖ്യ) len;
        len = 1;
    }

    വേണ്ടി (; len--; data0 += elemSize) {
        പൂർണ്ണസംഖ്യ offset = 0;
        വേണ്ടി (k = 0; k < fmt_pair_എണ്ണം; k++) {
            പൂർണ്ണസംഖ്യ i, എണ്ണം_2 = fmt_pairs[k * 2];
            പൂർണ്ണസംഖ്യ elem_type = fmt_pairs[k * 2 + 1];
            പൂർണ്ണസംഖ്യ elem_size = CV_ELEM_SIZE(elem_type);
            സ്ഥിരം അക്ഷരം *ptr;

            offset = cvAlign(offset, elem_size);
            സ്ഥിരം uchar *data = data0 + offset;

            വേണ്ടി (i = 0; i < എണ്ണം_2; i++) {
                തിരഞ്ഞെടുപ്പ് (elem_type) {
                    അവസ്ഥ CV_8U:
                        ptr = fs::itoa(*(uchar *) data, buf, 10);
                        data++;
                        നിർത്തുക;
                    അവസ്ഥ CV_Bool:
                        ptr = fs::itoa(*(uchar *) data != 0, buf, 10);
                        data++;
                        നിർത്തുക;
                    അവസ്ഥ CV_8S:
                        ptr = fs::itoa(*(അക്ഷരം *) data, buf, 10);
                        data++;
                        നിർത്തുക;
                    അവസ്ഥ CV_16U:
                        ptr = fs::itoa(*(ushort *) data, buf, 10);
                        data += വലുപ്പം(ushort);
                        നിർത്തുക;
                    അവസ്ഥ CV_16S:
                        ptr = fs::itoa(*(short *) data, buf, 10);
                        data += വലുപ്പം(short);
                        നിർത്തുക;
                    അവസ്ഥ CV_32U:
                        ptr = fs::itoa((int64_t)*(unsigned*) data, buf, 10, അസത്യം);
                        data += വലുപ്പം(unsigned);
                        നിർത്തുക;
                    അവസ്ഥ CV_32S:
                        ptr = fs::itoa(*(പൂർണ്ണസംഖ്യ *) data, buf, 10);
                        data += വലുപ്പം(പൂർണ്ണസംഖ്യ);
                        നിർത്തുക;
                    അവസ്ഥ CV_64U:
                        ptr = fs::itoa(*(uint64_t*) data, buf, 10, അസത്യം);
                        data += വലുപ്പം(uint64_t);
                        നിർത്തുക;
                    അവസ്ഥ CV_64S:
                        ptr = fs::itoa(*(int64_t*) data, buf, 10, സത്യം);
                        data += വലുപ്പം(int64_t);
                        നിർത്തുക;
                    അവസ്ഥ CV_32F:
                        ptr = fs::floatToString(buf, വലുപ്പം(buf), *(ദശാംശം *) data, അസത്യം, explicitZero);
                        data += വലുപ്പം(ദശാംശം);
                        നിർത്തുക;
                    അവസ്ഥ CV_64F:
                        ptr = fs::doubleToString(buf, വലുപ്പം(buf), *(ഇരട്ട *) data, explicitZero);
                        data += വലുപ്പം(ഇരട്ട);
                        നിർത്തുക;
                    അവസ്ഥ CV_16F:
                        ptr = fs::floatToString(buf, വലുപ്പം(buf), (ദശാംശം) *(hfloat *) data, സത്യം, explicitZero);
                        data += വലുപ്പം(hfloat);
                        നിർത്തുക;
                    അവസ്ഥ CV_16BF:
                        ptr = fs::floatToString(buf, വലുപ്പം(buf), (ദശാംശം) *(bfloat *) data, സത്യം, explicitZero);
                        data += വലുപ്പം(bfloat);
                        നിർത്തുക;
                    അവസ്ഥ CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, വലുപ്പം(buf), (ദശാംശം) *(fp8_t *) data, സത്യം, explicitZero);
                        data += വലുപ്പം(fp8_t);
                        നിർത്തുക;
                    അവസ്ഥ CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, വലുപ്പം(buf), (ദശാംശം) *(fp8a_t *) data, സത്യം, explicitZero);
                        data += വലുപ്പം(fp8a_t);
                        നിർത്തുക;
                    സ്ഥിരസ്ഥിതി:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        തിരികെ;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (പൂർണ്ണസംഖ്യ) (data - data0);
        }
    }
}

ശൂന്യം FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(അസത്യം);

    എങ്കിൽ (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_to_Base64_state(FileStorage_API::Base64State::Uncertain);
}

ശൂന്യം FileStorage::Impl::switch_to_Base64_state(FileStorage_API::Base64State new_state) {
    സ്ഥിരം അക്ഷരം *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    സ്ഥിരം അക്ഷരം *err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    തിരഞ്ഞെടുപ്പ് (state_of_writing_base64) {
        അവസ്ഥ FileStorage_API::Base64State::Uncertain:
            തിരഞ്ഞെടുപ്പ് (new_state) {
                അവസ്ഥ FileStorage_API::Base64State::InUse:
                {
                    CV_DbgAssert(base64_writer == 0);
                    തർക്കം can_indent = (fmt != cv::FileStorage::Mode::FORMAT_JSON);
                    base64_writer = പുതിയ base64::Base64Writer(*ഇത്, can_indent);
                    എങ്കിൽ (!can_indent) {
                        അക്ഷരം *ptr = bufferPtr();
                        *ptr++ = '\0';
                        puts(bufferStart());
                        setBufferPtr(bufferStart());
                        memset(bufferStart(), 0, static_cast<പൂർണ്ണസംഖ്യ>(space));
                        puts("\"$base64$");
                    }
                    നിർത്തുക;
                }
                അവസ്ഥ FileStorage_API::Base64State::Uncertain:
                    നിർത്തുക;
                അവസ്ഥ FileStorage_API::Base64State::NotUse:
                    നിർത്തുക;
                സ്ഥിരസ്ഥിതി:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    നിർത്തുക;
            }
            നിർത്തുക;
        അവസ്ഥ FileStorage_API::Base64State::InUse:
            തിരഞ്ഞെടുപ്പ് (new_state) {
                അവസ്ഥ FileStorage_API::Base64State::InUse:
                അവസ്ഥ FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    നിർത്തുക;
                അവസ്ഥ FileStorage_API::Base64State::Uncertain:
                    delete base64_writer;
                    base64_writer = 0;
                    എങ്കിൽ ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        setBufferPtr(bufferStart());
                        flush();
                        memset(bufferStart(), 0, static_cast<പൂർണ്ണസംഖ്യ>(space) );
                        setBufferPtr(bufferStart());
                    }
                    നിർത്തുക;
                സ്ഥിരസ്ഥിതി:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    നിർത്തുക;
            }
            നിർത്തുക;
        അവസ്ഥ FileStorage_API::Base64State::NotUse:
            തിരഞ്ഞെടുപ്പ് (new_state) {
                അവസ്ഥ FileStorage_API::Base64State::InUse:
                അവസ്ഥ FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    നിർത്തുക;
                അവസ്ഥ FileStorage_API::Base64State::Uncertain:
                    നിർത്തുക;
                സ്ഥിരസ്ഥിതി:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    നിർത്തുക;
            }
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            നിർത്തുക;
    }

    state_of_writing_base64 = new_state;
}

ശൂന്യം FileStorage::Impl::make_write_struct_delayed(സ്ഥിരം അക്ഷരം *താക്കോൽ, പൂർണ്ണസംഖ്യ struct_flags, സ്ഥിരം അക്ഷരം *type_name) {
    CV_Assert(is_write_struct_delayed == അസത്യം);
    CV_DbgAssert(delayed_struct_താക്കോൽ == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    എങ്കിൽ (താക്കോൽ != nullptr) {
        delayed_struct_താക്കോൽ = പുതിയ അക്ഷരം[strlen(താക്കോൽ) + 1U];
        strcpy(delayed_struct_താക്കോൽ, താക്കോൽ);
    }

    എങ്കിൽ (type_name != nullptr) {
        delayed_type_name = പുതിയ അക്ഷരം[strlen(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = സത്യം;
}

ശൂന്യം FileStorage::Impl::check_if_write_struct_is_delayed(തർക്കം change_type_to_base64) {
    എങ്കിൽ (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_താക്കോൽ;
        std::string type_name;
        പൂർണ്ണസംഖ്യ struct_flags = delayed_struct_flags;

        എങ്കിൽ (delayed_struct_താക്കോൽ != nullptr && *delayed_struct_താക്കോൽ != '\0') {
            struct_താക്കോൽ.assign(delayed_struct_താക്കോൽ);
        }
        എങ്കിൽ (delayed_type_name != nullptr && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        delete[] delayed_struct_താക്കോൽ;
        delete[] delayed_type_name;
        delayed_struct_താക്കോൽ = nullptr;
        delayed_struct_flags = 0;
        delayed_type_name = nullptr;

        is_write_struct_delayed = അസത്യം;

        /* call */
        എങ്കിൽ (change_type_to_base64) {
            startWriteStruct_helper(struct_താക്കോൽ.c_str(), struct_flags, "binary");
            എങ്കിൽ (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::InUse);
        } അല്ലെങ്കിൽ {
            startWriteStruct_helper(struct_താക്കോൽ.c_str(), struct_flags, type_name.c_str());
            എങ്കിൽ (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::NotUse);
        }
    }
}

ശൂന്യം FileStorage::Impl::writeRawDataBase64(സ്ഥിരം ശൂന്യം *_data, size_t len, സ്ഥിരം അക്ഷരം *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(സത്യം);

    എങ്കിൽ (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } അല്ലെങ്കിൽ എങ്കിൽ (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->ലിഖ(_data, len, dt);
}

File_കണ്ണി FileStorage::Impl::get_First_മുകൾ_Level_കണ്ണി() സ്ഥിരം {
    തിരികെ roots.empty() ? File_കണ്ണി() : roots[0];
}

File_കണ്ണി FileStorage::Impl::വേര്(പൂർണ്ണസംഖ്യ streamIdx) സ്ഥിരം {
    തിരികെ streamIdx >= 0 && streamIdx < (പൂർണ്ണസംഖ്യ) roots.size() ? roots[streamIdx] : File_കണ്ണി();
}

File_കണ്ണി FileStorage::Impl::operator[](സ്ഥിരം String &nodename) സ്ഥിരം {
    തിരികെ ഇത്->operator[](nodename.c_str());
}

File_കണ്ണി FileStorage::Impl::operator[](സ്ഥിരം അക്ഷരം * /*nodename*/) സ്ഥിരം {
    തിരികെ File_കണ്ണി();
}

പൂർണ്ണസംഖ്യ FileStorage::Impl::getFormat() സ്ഥിരം { തിരികെ fmt; }

അക്ഷരം *FileStorage::Impl::bufferPtr() സ്ഥിരം { തിരികെ (അക്ഷരം *) (&buffer[0] + bufofs); }

അക്ഷരം *FileStorage::Impl::bufferStart() സ്ഥിരം { തിരികെ (അക്ഷരം *) &buffer[0]; }

അക്ഷരം *FileStorage::Impl::bufferEnd() സ്ഥിരം { തിരികെ (അക്ഷരം *) (&buffer[0] + buffer.size()); }

ശൂന്യം FileStorage::Impl::setBufferPtr(അക്ഷരം *ptr) {
    അക്ഷരം *bufferstart = bufferStart();
    CV_Assert(ptr >= bufferstart && ptr <= bufferEnd());
    bufofs = ptr - bufferstart;
}

പൂർണ്ണസംഖ്യ FileStorage::Impl::wrapMargin() സ്ഥിരം { തിരികെ wrap_margin; }

FStructData &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_അട്ടി.empty());
    തിരികെ write_അട്ടി.back();
}

ശൂന്യം FileStorage::Impl::setNonEmpty() {
    empty_stream = അസത്യം;
}

ശൂന്യം FileStorage::Impl::processSpecialDouble(അക്ഷരം *buf, ഇരട്ട *value, അക്ഷരം **endptr) {
    FileStorage_API *fs = ഇത്;
    അക്ഷരം c = buf[0];
    പൂർണ്ണസംഖ്യ inf_hi = 0x7ff00000;

    എങ്കിൽ (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    എങ്കിൽ (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    എങ്കിൽ (toupper(buf[1]) == 'I' && toupper(buf[2]) == 'N' && toupper(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    അല്ലെങ്കിൽ എങ്കിൽ (toupper(buf[1]) == 'N' && toupper(buf[2]) == 'A' && toupper(buf[3]) == 'N')
        v.u = (uint64) -1;
    അല്ലെങ്കിൽ
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

ഇരട്ട FileStorage::Impl::strtod(അക്ഷരം *ptr, അക്ഷരം **endptr) {
    ഇരട്ട fval = ::strtod(ptr, endptr);
    എങ്കിൽ (**endptr == '.') {
        അക്ഷരം *dot_pos = *endptr;
        *dot_pos = ',';
        ഇരട്ട fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        എങ്കിൽ (*endptr > dot_pos)
            fval = fval2;
        അല്ലെങ്കിൽ
            *endptr = dot_pos;
    }

    എങ്കിൽ (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    തിരികെ fval;
}

ശൂന്യം FileStorage::Impl::convertToCollection(പൂർണ്ണസംഖ്യ type, File_കണ്ണി &കണ്ണി) {
    CV_Assert(type == File_കണ്ണി::SEQ || type == File_കണ്ണി::MAP);

    പൂർണ്ണസംഖ്യ കണ്ണി_type = കണ്ണി.type();
    എങ്കിൽ (കണ്ണി_type == type)
        തിരികെ;

    തർക്കം named = കണ്ണി.isNamed();
    uchar *ptr = കണ്ണി.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    ഇരട്ട fval = 0;
    std::string sval;
    തർക്കം add_first_scalar = അസത്യം;

    എങ്കിൽ (കണ്ണി_type != File_കണ്ണി::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_കണ്ണി::SEQ);
        എങ്കിൽ (കണ്ണി_type == File_കണ്ണി::INT) {
            ival = readLong(ptr);
            add_first_scalar = സത്യം;
        } അല്ലെങ്കിൽ എങ്കിൽ (കണ്ണി_type == File_കണ്ണി::REAL) {
            fval = readReal(ptr);
            add_first_scalar = സത്യം;
        } അല്ലെങ്കിൽ എങ്കിൽ (കണ്ണി_type == File_കണ്ണി::STRING) {
            sval = std::string(കണ്ണി);
            add_first_scalar = സത്യം;
        } അല്ലെങ്കിൽ
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", കണ്ണി_type));
    }

    ptr = reserve_കണ്ണി_Space(കണ്ണി, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_കണ്ണി::NAMED : 0));
    // name has been copied automatically
    എങ്കിൽ (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    എങ്കിൽ (add_first_scalar)
        add_കണ്ണി(കണ്ണി, std::string(), കണ്ണി_type,
                കണ്ണി_type == File_കണ്ണി::INT ? (സ്ഥിരം ശൂന്യം *) &ival :
                കണ്ണി_type == File_കണ്ണി::REAL ? (സ്ഥിരം ശൂന്യം *) &fval :
                കണ്ണി_type == File_കണ്ണി::STRING ? (സ്ഥിരം ശൂന്യം *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_കണ്ണി_Space(File_കണ്ണി &കണ്ണി, size_t sz) {
    തർക്കം shrinkBlock = അസത്യം;
    size_t shrinkBlockIdx = 0, shrinkSize = 0;

    uchar *ptr = 0, *blockEnd = 0;

    എങ്കിൽ (!fs_data_ptrs.empty()) {
        size_t blockIdx = കണ്ണി.blockIdx;
        size_t ofs = കണ്ണി.ofs;
        CV_Assert(blockIdx == fs_data_ptrs.size() - 1);
        CV_Assert(ofs <= fs_data_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_data_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_data_ptrs[blockIdx] + ofs;
        blockEnd = fs_data_ptrs[blockIdx] + fs_data_blksz[blockIdx];

        CV_Assert(ptr >= fs_data_ptrs[blockIdx] && ptr <= blockEnd);
        എങ്കിൽ (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            തിരികെ ptr;
        }

        എങ്കിൽ (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_data[blockIdx]->resize(sz);
            ptr = &fs_data[blockIdx]->at(0);
            fs_data_ptrs[blockIdx] = ptr;
            fs_data_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            തിരികെ ptr;
        }

        shrinkBlock = സത്യം;
        shrinkBlockIdx = blockIdx;
        shrinkSize = ofs;
    }

    size_t blockSize = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(blockSize);
    fs_data.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_data_ptrs.push_back(new_ptr);
    fs_data_blksz.push_back(blockSize);
    കണ്ണി.blockIdx = fs_data_ptrs.size() - 1;
    കണ്ണി.ofs = 0;
    freeSpaceOfs = sz;

    എങ്കിൽ (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        എങ്കിൽ (ptr[0] & File_കണ്ണി::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    എങ്കിൽ (shrinkBlock) {
        fs_data[shrinkBlockIdx]->resize(shrinkSize);
        fs_data_blksz[shrinkBlockIdx] = shrinkSize;
    }

    തിരികെ new_ptr;
}

unsigned FileStorage::Impl::getStringOfs(സ്ഥിരം std::string &താക്കോൽ) സ്ഥിരം {
    str_hash_t::const_iterator it = str_hash.find(താക്കോൽ);
    തിരികെ it != str_hash.end() ? it->second : 0;
}

File_കണ്ണി FileStorage::Impl::add_കണ്ണി(File_കണ്ണി &collection, സ്ഥിരം std::string &താക്കോൽ,
                                    പൂർണ്ണസംഖ്യ elem_type, സ്ഥിരം ശൂന്യം *value, പൂർണ്ണസംഖ്യ len) {
    FileStorage_API *fs = ഇത്;
    തർക്കം noname = താക്കോൽ.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(താക്കോൽ.c_str(), "_") == 0);
    convertToCollection(noname ? File_കണ്ണി::SEQ : File_കണ്ണി::MAP, collection);

    തർക്കം isseq = collection.empty() ? അസത്യം : collection.isSeq();
    എങ്കിൽ (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    unsigned strofs = 0;
    എങ്കിൽ (!noname) {
        strofs = getStringOfs(താക്കോൽ);
        എങ്കിൽ (!strofs) {
            strofs = (unsigned) str_hash_data.size();
            size_t keysize = താക്കോൽ.size() + 1;
            str_hash_data.resize(strofs + keysize);
            memcpy(&str_hash_data[0] + strofs, &താക്കോൽ[0], keysize);
            str_hash.insert(std::make_pair(താക്കോൽ, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t blockIdx = fs_data_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_കണ്ണി കണ്ണി(fs_ext, blockIdx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_കണ്ണി_Space(കണ്ണി, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_കണ്ണി::NAMED));
    എങ്കിൽ (elem_type == File_കണ്ണി::NONE)
        freeSpaceOfs -= 8;

    എങ്കിൽ (!noname) {
        writeInt(ptr, (പൂർണ്ണസംഖ്യ) strofs);
        ptr += 4;
    }

    എങ്കിൽ (elem_type == File_കണ്ണി::SEQ || elem_type == File_കണ്ണി::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    എങ്കിൽ (value)
        കണ്ണി.set_മൂല്യം(elem_type, value, len);

    എങ്കിൽ (collection.isNamed())
        cp += 4;
    പൂർണ്ണസംഖ്യ nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    തിരികെ കണ്ണി;
}

ശൂന്യം FileStorage::Impl::finalizeCollection(File_കണ്ണി &collection) {
    എങ്കിൽ (!collection.isSeq() && !collection.isMap())
        തിരികെ;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    എങ്കിൽ (*ptr0 & File_കണ്ണി::NAMED)
        ptr += 4;
    size_t blockIdx = collection.blockIdx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t rawSize = 4;
    unsigned sz = (unsigned) readInt(ptr + 4);
    എങ്കിൽ (sz > 0) {
        size_t lastBlockIdx = fs_data_ptrs.size() - 1;

        വേണ്ടി (; blockIdx < lastBlockIdx; blockIdx++) {
            rawSize += fs_data_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    rawSize += freeSpaceOfs - ofs;
    writeInt(ptr, (പൂർണ്ണസംഖ്യ) rawSize);
}

ശൂന്യം FileStorage::Impl::normalize_കണ്ണി_Ofs(size_t &blockIdx, size_t &ofs) സ്ഥിരം {
    ആകുമ്പോൾ (ofs >= fs_data_blksz[blockIdx]) {
        എങ്കിൽ (blockIdx == fs_data_blksz.size() - 1) {
            CV_Assert(ofs == fs_data_blksz[blockIdx]);
            നിർത്തുക;
        }
        ofs -= fs_data_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    തിരികെ state_of_writing_base64;
}

പൂർണ്ണസംഖ്യ FileStorage::Impl::get_space() {
    തിരികെ space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = സത്യം;
}

ശൂന്യം FileStorage::Impl::Base64Decoder::init(സ്ഥിരം Ptr<FileStorageParser> &_parser, അക്ഷരം *_ptr, പൂർണ്ണസംഖ്യ _indent) {
    parser_do_not_use_direct_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = അസത്യം;
}

തർക്കം FileStorage::Impl::Base64Decoder::readMore(പൂർണ്ണസംഖ്യ needed) {
    സ്ഥിതം സ്ഥിരം uchar base64tab[] =
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

    എങ്കിൽ (eos)
        തിരികെ അസത്യം;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    വേണ്ടി (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    അക്ഷരം *beg = 0, *end = 0;
    തർക്കം ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    എങ്കിൽ (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = സത്യം;
        size_t tc = totalchars;
        വേണ്ടി (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    പൂർണ്ണസംഖ്യ i = 0, j, n = (പൂർണ്ണസംഖ്യ) encoded.size();
    എങ്കിൽ (n > 0) {
        സ്ഥിരം uchar *tab = base64tab;
        അക്ഷരം *src = &encoded[0];

        വേണ്ടി (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(പൂർണ്ണസംഖ്യ) (uchar) src[i]], c = tab[(പൂർണ്ണസംഖ്യ) (uchar) src[i + 1]];
            uchar b = tab[(പൂർണ്ണസംഖ്യ) (uchar) src[i + 2]], a = tab[(പൂർണ്ണസംഖ്യ) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    എങ്കിൽ (i > 0 && encoded[i - 1] == '=') {
        എങ്കിൽ (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        എങ്കിൽ (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    വേണ്ടി (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    തിരികെ (പൂർണ്ണസംഖ്യ) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    എങ്കിൽ (ofs >= sz && !readMore(1))
        തിരികെ (uchar) 0;
    തിരികെ decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    എങ്കിൽ (ofs + 2 > sz && !readMore(2))
        തിരികെ (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    തിരികെ val;
}

പൂർണ്ണസംഖ്യ FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    എങ്കിൽ (ofs + 4 > sz && !readMore(4))
        തിരികെ 0;
    പൂർണ്ണസംഖ്യ ival = readInt(&decoded[ofs]);
    ofs += 4;
    തിരികെ ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    എങ്കിൽ (ofs + 8 > sz && !readMore(8))
        തിരികെ 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    തിരികെ ival;
}

ഇരട്ട FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    എങ്കിൽ (ofs + 8 > sz && !readMore(8))
        തിരികെ 0;
    ഇരട്ട fval = readReal(&decoded[ofs]);
    ofs += 8;
    തിരികെ fval;
}

തർക്കം FileStorage::Impl::Base64Decoder::endOfStream() സ്ഥിരം { തിരികെ eos; }

അക്ഷരം *FileStorage::Impl::Base64Decoder::getPtr() സ്ഥിരം { തിരികെ ptr; }


അക്ഷരം *FileStorage::Impl::parseBase64(അക്ഷരം *ptr, പൂർണ്ണസംഖ്യ indent, File_കണ്ണി &collection) {
    സ്ഥിരം പൂർണ്ണസംഖ്യ BASE64_HDR_SIZE = 24;
    അക്ഷരം dt[BASE64_HDR_SIZE + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    പൂർണ്ണസംഖ്യ i, k;

    വേണ്ടി (i = 0; i < BASE64_HDR_SIZE; i++)
        dt[i] = (അക്ഷരം) base64decoder.getUInt8();
    വേണ്ടി (i = 0; i < BASE64_HDR_SIZE; i++)
        എങ്കിൽ (isspace(dt[i]))
            നിർത്തുക;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    പൂർണ്ണസംഖ്യ fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    പൂർണ്ണസംഖ്യ fmt_pair_എണ്ണം = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    ഇരട്ട fval = 0;

    വേണ്ടി (;;) {
        വേണ്ടി (k = 0; k < fmt_pair_എണ്ണം; k++) {
            പൂർണ്ണസംഖ്യ elem_type = fmt_pairs[k * 2 + 1];
            പൂർണ്ണസംഖ്യ എണ്ണം_2 = fmt_pairs[k * 2];

            വേണ്ടി (i = 0; i < എണ്ണം_2; i++) {
                പൂർണ്ണസംഖ്യ കണ്ണി_type = File_കണ്ണി::INT;
                തിരഞ്ഞെടുപ്പ് (elem_type) {
                    അവസ്ഥ CV_8U:
                        ival = base64decoder.getUInt8();
                        നിർത്തുക;
                    അവസ്ഥ CV_8S:
                        ival = (അക്ഷരം) base64decoder.getUInt8();
                        നിർത്തുക;
                    അവസ്ഥ CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        നിർത്തുക;
                    അവസ്ഥ CV_16U:
                        ival = base64decoder.getUInt16();
                        നിർത്തുക;
                    അവസ്ഥ CV_16S:
                        ival = (short) base64decoder.getUInt16();
                        നിർത്തുക;
                    അവസ്ഥ CV_32U:
                        ival = base64decoder.getInt32();
                        നിർത്തുക;
                    അവസ്ഥ CV_32S:
                        ival = base64decoder.getInt32();
                        നിർത്തുക;
                    അവസ്ഥ CV_64U:
                        ival = base64decoder.getInt64();
                        നിർത്തുക;
                    അവസ്ഥ CV_64S:
                        ival = base64decoder.getInt64();
                        നിർത്തുക;
                    അവസ്ഥ CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        കണ്ണി_type = File_കണ്ണി::REAL;
                    }
                        നിർത്തുക;
                    അവസ്ഥ CV_64F:
                        fval = base64decoder.getFloat64();
                        കണ്ണി_type = File_കണ്ണി::REAL;
                        നിർത്തുക;
                    അവസ്ഥ CV_16F:
                        fval = ദശാംശം(hfloatFromBits(base64decoder.getUInt16()));
                        കണ്ണി_type = File_കണ്ണി::REAL;
                        നിർത്തുക;
                    അവസ്ഥ CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        കണ്ണി_type = File_കണ്ണി::REAL;
                        നിർത്തുക;
                    അവസ്ഥ CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        കണ്ണി_type = File_കണ്ണി::REAL;
                        നിർത്തുക;
                    സ്ഥിരസ്ഥിതി:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                എങ്കിൽ (base64decoder.endOfStream())
                    നിർത്തുക;
                add_കണ്ണി(collection, std::string(), കണ്ണി_type,
                        കണ്ണി_type == File_കണ്ണി::INT ? (ശൂന്യം *) &ival : (ശൂന്യം *) &fval, -1);
            }
        }
        എങ്കിൽ (base64decoder.endOfStream())
            നിർത്തുക;
    }

    finalizeCollection(collection);
    തിരികെ base64decoder.getPtr();
}

ശൂന്യം FileStorage::Impl::parseError(സ്ഥിരം അക്ഷരം *func_name, സ്ഥിരം std::string &err_സന്ദേശം, സ്ഥിരം അക്ഷരം *source_file,
                                   പൂർണ്ണസംഖ്യ source_വരി) {
    std::string സന്ദേശം = format("%s(%d): %s", filename.c_str(), lineno, err_സന്ദേശം.c_str());
    error(Error::StsParseError, func_name, സന്ദേശം.c_str(), source_file, source_വരി);
}

സ്ഥിരം uchar *FileStorage::Impl::get_കണ്ണി_Ptr(size_t blockIdx, size_t ofs) സ്ഥിരം {
    CV_Assert(blockIdx < fs_data_ptrs.size());
    CV_Assert(ofs < fs_data_blksz[blockIdx]);

    തിരികെ fs_data_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) സ്ഥിരം {
    CV_Assert(nameofs < str_hash_data.size());
    തിരികെ std::string(&str_hash_data[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { തിരികെ fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(ഇത്);
}

FileStorage::FileStorage(സ്ഥിരം String& filename, പൂർണ്ണസംഖ്യ flags, സ്ഥിരം String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(ഇത്);
    തർക്കം ok = p->open(filename.c_str(), flags, encoding.c_str());
    എങ്കിൽ(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

ശൂന്യം FileStorage::startWriteStruct(സ്ഥിരം String& name, പൂർണ്ണസംഖ്യ struct_flags, സ്ഥിരം String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    എങ്കിൽ ((struct_flags & File_കണ്ണി::TYPE_MASK) == File_കണ്ണി::SEQ)
        state = FileStorage::മൂല്യം_EXPECTED;
    അല്ലെങ്കിൽ
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

ശൂന്യം FileStorage::endWriteStruct()
{
    p->endWriteStruct();
    state = p->write_അട്ടി.empty() || File_കണ്ണി::isMap(p->write_അട്ടി.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::മൂല്യം_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

തർക്കം FileStorage::open(സ്ഥിരം String& filename, പൂർണ്ണസംഖ്യ flags, സ്ഥിരം String& encoding)
{
    ശ്രമിക്കുക
    {
        തർക്കം ok = p->open(filename.c_str(), flags, encoding.c_str());
        എങ്കിൽ(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        തിരികെ ok;
    }
    പിടിക്കുക (...)
    {
        release();
        എറിയുക;  // re-throw
    }
}

തർക്കം FileStorage::isOpened() സ്ഥിരം { തിരികെ p->is_opened; }

ശൂന്യം FileStorage::release()
{
    p->release();
}

File_കണ്ണി FileStorage::വേര്(പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    എങ്കിൽ( p.empty() || p->roots.empty() || i < 0 || i >= (പൂർണ്ണസംഖ്യ)p->roots.size() )
        തിരികെ File_കണ്ണി();

    തിരികെ p->roots[i];
}

File_കണ്ണി FileStorage::get_First_മുകൾ_Level_കണ്ണി() സ്ഥിരം
{
    File_കണ്ണി r = വേര്();
    File_കണ്ണി_Iterator it = r.begin();
    തിരികെ it != r.end() ? *it : File_കണ്ണി();
}

std::string FileStorage::getDefaultObjectName(സ്ഥിരം std::string& _filename)
{
    സ്ഥിതം സ്ഥിരം അക്ഷരം* stubname = "unnamed";
    സ്ഥിരം അക്ഷരം* filename = _filename.c_str();
    സ്ഥിരം അക്ഷരം* ptr2 = filename + _filename.size();
    സ്ഥിരം അക്ഷരം* ptr = ptr2 - 1;
    cv::AutoBuffer<അക്ഷരം> name_buf(_filename.size()+1);

    ആകുമ്പോൾ( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        എങ്കിൽ( *ptr == '.' && (!*ptr2 || strncmp(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    എങ്കിൽ( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    അക്ഷരം* name = name_buf.data();

    // name must start with letter or '_'
    എങ്കിൽ( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    ആകുമ്പോൾ( ptr < ptr2 )
    {
        അക്ഷരം c = *ptr++;
        എങ്കിൽ( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.data();
    എങ്കിൽ( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    തിരികെ name;
}


പൂർണ്ണസംഖ്യ FileStorage::getFormat() സ്ഥിരം
{
    തിരികെ p->fmt;
}

File_കണ്ണി FileStorage::operator [](സ്ഥിരം അക്ഷരം* താക്കോൽ) സ്ഥിരം
{
    തിരികെ ഇത്->operator[](std::string(താക്കോൽ));
}

File_കണ്ണി FileStorage::operator [](സ്ഥിരം std::string& താക്കോൽ) സ്ഥിരം
{
    File_കണ്ണി res;
    വേണ്ടി (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][താക്കോൽ];
        എങ്കിൽ (!res.empty())
            നിർത്തുക;
    }
    തിരികെ res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    തിരികെ buf;
}

ശൂന്യം FileStorage::writeRaw( സ്ഥിരം String& fmt, സ്ഥിരം ശൂന്യം* vec, size_t len )
{
    p->writeRawData(fmt, (സ്ഥിരം uchar*)vec, len);
}

ശൂന്യം FileStorage::writeComment( സ്ഥിരം String& comment, തർക്കം eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

ശൂന്യം writeScalar( FileStorage& fs, പൂർണ്ണസംഖ്യ value )
{
    fs.p->ലിഖ(String(), value);
}

ശൂന്യം writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->ലിഖ(String(), value);
}

ശൂന്യം writeScalar( FileStorage& fs, ദശാംശം value )
{
    fs.p->ലിഖ(String(), (ഇരട്ട)value);
}

ശൂന്യം writeScalar( FileStorage& fs, ഇരട്ട value )
{
    fs.p->ലിഖ(String(), value);
}

ശൂന്യം writeScalar( FileStorage& fs, സ്ഥിരം String& value )
{
    fs.p->ലിഖ(String(), value);
}

ശൂന്യം ലിഖ( FileStorage& fs, സ്ഥിരം String& name, പൂർണ്ണസംഖ്യ value )
{
    fs.p->ലിഖ(name, value);
}

ശൂന്യം ലിഖ( FileStorage& fs, സ്ഥിരം String& name, int64_t value )
{
    fs.p->ലിഖ(name, value);
}

ശൂന്യം ലിഖ( FileStorage& fs, സ്ഥിരം String& name, ദശാംശം value )
{
    fs.p->ലിഖ(name, (ഇരട്ട)value);
}

ശൂന്യം ലിഖ( FileStorage& fs, സ്ഥിരം String& name, ഇരട്ട value )
{
    fs.p->ലിഖ(name, value);
}

ശൂന്യം ലിഖ( FileStorage& fs, സ്ഥിരം String& name, സ്ഥിരം String& value )
{
    fs.p->ലിഖ(name, value);
}

ശൂന്യം ലിഖ( FileStorage& fs, സ്ഥിരം String& name, തർക്കം value )
{
    fs.p->ലിഖ(name, value);
}

ശൂന്യം FileStorage::ലിഖ(സ്ഥിരം String& name, പൂർണ്ണസംഖ്യ val) { p->ലിഖ(name, val); }
ശൂന്യം FileStorage::ലിഖ(സ്ഥിരം String& name, തർക്കം val) { p->ലിഖ(name, val); }
ശൂന്യം FileStorage::ലിഖ(സ്ഥിരം String& name, int64_t val) { p->ലിഖ(name, val); }
ശൂന്യം FileStorage::ലിഖ(സ്ഥിരം String& name, ഇരട്ട val) { p->ലിഖ(name, val); }
ശൂന്യം FileStorage::ലിഖ(സ്ഥിരം String& name, സ്ഥിരം String& val) { p->ലിഖ(name, val); }
ശൂന്യം FileStorage::ലിഖ(സ്ഥിരം String& name, സ്ഥിരം Mat& val) { cv::ലിഖ(*ഇത്, name, val); }
ശൂന്യം FileStorage::ലിഖ(സ്ഥിരം String& name, സ്ഥിരം std::vector<String>& val) { cv::ലിഖ(*ഇത്, name, val); }

FileStorage& operator << (FileStorage& fs, സ്ഥിരം String& str)
{
    എണ്ണൽ { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        മൂല്യം_EXPECTED = FileStorage::മൂല്യം_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    സ്ഥിരം അക്ഷരം* _str = str.c_str();
    എങ്കിൽ( !fs.isOpened() || !_str )
        തിരികെ fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    അക്ഷരം c = *_str;

    എങ്കിൽ( c == '}' || c == ']' )
    {
        എങ്കിൽ( fs_impl->write_അട്ടി.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        പൂർണ്ണസംഖ്യ struct_flags = fs_impl->write_അട്ടി.back().flags;
        അക്ഷരം expected_bracket = File_കണ്ണി::isMap(struct_flags) ? '}' : ']';
        എങ്കിൽ( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_അട്ടി.empty());
        struct_flags = fs_impl->write_അട്ടി.back().flags;
        fs.state = File_കണ്ണി::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : മൂല്യം_EXPECTED;
        fs.elname = String();
    }
    അല്ലെങ്കിൽ എങ്കിൽ( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        എങ്കിൽ (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = മൂല്യം_EXPECTED + INSIDE_MAP;
    }
    അല്ലെങ്കിൽ എങ്കിൽ( (fs.state & 3) == മൂല്യം_EXPECTED )
    {
        എങ്കിൽ( c == '{' || c == '[' )
        {
            പൂർണ്ണസംഖ്യ struct_flags = c == '{' ? File_കണ്ണി::MAP : File_കണ്ണി::SEQ;
            fs.state = struct_flags == File_കണ്ണി::MAP ? INSIDE_MAP + NAME_EXPECTED : മൂല്യം_EXPECTED;
            _str++;
            എങ്കിൽ( *_str == ':' )
            {
                _str++;
                എങ്കിൽ( !*_str )
                    struct_flags |= File_കണ്ണി::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        അല്ലെങ്കിൽ
        {
            ലിഖ( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            എങ്കിൽ( fs.state == INSIDE_MAP + മൂല്യം_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    അല്ലെങ്കിൽ
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    തിരികെ fs;
}


File_കണ്ണി::File_കണ്ണി()
    : fs(ശൂന്യ)
{
    blockIdx = ofs = 0;
}

File_കണ്ണി::File_കണ്ണി(FileStorage::Impl* _fs, size_t _blockIdx, size_t _ofs)
    : fs(_fs)
{
    blockIdx = _blockIdx;
    ofs = _ofs;
}

File_കണ്ണി::File_കണ്ണി(സ്ഥിരം FileStorage* _fs, size_t _blockIdx, size_t _ofs)
    : File_കണ്ണി(_fs->p.get(), _blockIdx, _ofs)
{
    // nothing
}

File_കണ്ണി::File_കണ്ണി(സ്ഥിരം File_കണ്ണി& കണ്ണി)
{
    fs = കണ്ണി.fs;
    blockIdx = കണ്ണി.blockIdx;
    ofs = കണ്ണി.ofs;
}

File_കണ്ണി& File_കണ്ണി::operator=(സ്ഥിരം File_കണ്ണി& കണ്ണി)
{
    fs = കണ്ണി.fs;
    blockIdx = കണ്ണി.blockIdx;
    ofs = കണ്ണി.ofs;
    തിരികെ *ഇത്;
}

File_കണ്ണി File_കണ്ണി::operator[](സ്ഥിരം std::string& nodename) സ്ഥിരം
{
    എങ്കിൽ(!fs)
        തിരികെ File_കണ്ണി();

    CV_Assert( isMap() );

    unsigned താക്കോൽ = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_കണ്ണി_Iterator it = begin();

    വേണ്ടി( i = 0; i < sz; i++, ++it )
    {
        File_കണ്ണി n = *it;
        സ്ഥിരം uchar* p = n.ptr();
        unsigned key2 = (unsigned)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_data.size() );
        എങ്കിൽ( താക്കോൽ == key2 )
            തിരികെ n;
    }
    തിരികെ File_കണ്ണി();
}

File_കണ്ണി File_കണ്ണി::operator[](സ്ഥിരം അക്ഷരം* nodename) സ്ഥിരം
{
    തിരികെ ഇത്->operator[](std::string(nodename));
}

File_കണ്ണി File_കണ്ണി::operator[](പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{
    എങ്കിൽ(!fs)
        തിരികെ File_കണ്ണി();

    CV_Assert( isSeq() );

    പൂർണ്ണസംഖ്യ sz = (പൂർണ്ണസംഖ്യ)size();
    CV_Assert( 0 <= i && i < sz );

    File_കണ്ണി_Iterator it = begin();
    it += i;

    തിരികെ *it;
}

std::vector<String> File_കണ്ണി::താക്കോലുകൾ() സ്ഥിരം
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    വേണ്ടി (File_കണ്ണി_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    തിരികെ res;
}

പൂർണ്ണസംഖ്യ File_കണ്ണി::type() സ്ഥിരം
{
    സ്ഥിരം uchar* p = ptr();
    എങ്കിൽ(!p)
        തിരികെ NONE;
    തിരികെ (*p & TYPE_MASK);
}

തർക്കം File_കണ്ണി::isMap(പൂർണ്ണസംഖ്യ flags) { തിരികെ (flags & TYPE_MASK) == MAP; }
തർക്കം File_കണ്ണി::isSeq(പൂർണ്ണസംഖ്യ flags) { തിരികെ (flags & TYPE_MASK) == SEQ; }
തർക്കം File_കണ്ണി::isCollection(പൂർണ്ണസംഖ്യ flags) { തിരികെ isMap(flags) || isSeq(flags); }
തർക്കം File_കണ്ണി::isFlow(പൂർണ്ണസംഖ്യ flags) { തിരികെ (flags & FLOW) != 0; }
തർക്കം File_കണ്ണി::isEmptyCollection(പൂർണ്ണസംഖ്യ flags) { തിരികെ (flags & EMPTY) != 0; }

തർക്കം File_കണ്ണി::empty() സ്ഥിരം   { തിരികെ fs == 0; }
തർക്കം File_കണ്ണി::isNone() സ്ഥിരം  { തിരികെ type() == NONE; }
തർക്കം File_കണ്ണി::isSeq() സ്ഥിരം   { തിരികെ type() == SEQ; }
തർക്കം File_കണ്ണി::isMap() സ്ഥിരം   { തിരികെ type() == MAP; }
തർക്കം File_കണ്ണി::isInt() സ്ഥിരം   { തിരികെ type() == INT;  }
തർക്കം File_കണ്ണി::isReal() സ്ഥിരം  { തിരികെ type() == REAL; }
തർക്കം File_കണ്ണി::isString() സ്ഥിരം { തിരികെ type() == STRING;  }
തർക്കം File_കണ്ണി::isNamed() സ്ഥിരം
{
    സ്ഥിരം uchar* p = ptr();
    എങ്കിൽ(!p)
        തിരികെ അസത്യം;
    തിരികെ (*p & NAMED) != 0;
}

std::string File_കണ്ണി::name() സ്ഥിരം
{
    സ്ഥിരം uchar* p = ptr();
    എങ്കിൽ(!p)
        തിരികെ std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    തിരികെ fs->getName(nameofs);
}

File_കണ്ണി::operator പൂർണ്ണസംഖ്യ() സ്ഥിരം
{
    സ്ഥിരം uchar* p = ptr();
    എങ്കിൽ(!p)
        തിരികെ 0;
    പൂർണ്ണസംഖ്യ tag = *p;
    പൂർണ്ണസംഖ്യ type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    എങ്കിൽ( type == INT )
    {
        തിരികെ readInt(p);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( type == REAL )
    {
        തിരികെ cvRound(readReal(p));
    }
    അല്ലെങ്കിൽ
        തിരികെ 0x7fffffff;
}

File_കണ്ണി::operator int64_t() സ്ഥിരം
{
    സ്ഥിരം uchar* p = ptr();
    എങ്കിൽ(!p)
        തിരികെ 0;
    പൂർണ്ണസംഖ്യ tag = *p;
    പൂർണ്ണസംഖ്യ type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    എങ്കിൽ( type == INT )
    {
        തിരികെ readLong(p);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( type == REAL )
    {
        തിരികെ cvRound(readReal(p));
    }
    അല്ലെങ്കിൽ
        തിരികെ 0x7fffffff;
}

File_കണ്ണി::operator ദശാംശം() സ്ഥിരം
{
    സ്ഥിരം uchar* p = ptr();
    എങ്കിൽ(!p)
        തിരികെ 0.f;
    പൂർണ്ണസംഖ്യ tag = *p;
    പൂർണ്ണസംഖ്യ type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    എങ്കിൽ( type == INT )
    {
        തിരികെ (ദശാംശം)readLong(p);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( type == REAL )
    {
        തിരികെ (ദശാംശം)readReal(p);
    }
    അല്ലെങ്കിൽ
        തിരികെ FLT_MAX;
}

File_കണ്ണി::operator ഇരട്ട() സ്ഥിരം
{
    സ്ഥിരം uchar* p = ptr();
    എങ്കിൽ(!p)
        തിരികെ 0.f;
    പൂർണ്ണസംഖ്യ tag = *p;
    പൂർണ്ണസംഖ്യ type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    എങ്കിൽ( type == INT )
    {
        തിരികെ (ഇരട്ട)readLong(p);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( type == REAL )
    {
        തിരികെ readReal(p);
    }
    അല്ലെങ്കിൽ
        തിരികെ DBL_MAX;
}

ഇരട്ട File_കണ്ണി::real() സ്ഥിരം  { തിരികെ ഇരട്ട(*ഇത്); }

std::string File_കണ്ണി::string() സ്ഥിരം
{
    സ്ഥിരം uchar* p = ptr();
    എങ്കിൽ( !p || (*p & TYPE_MASK) != STRING )
        തിരികെ std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(unsigned)readInt(p);
    തിരികെ std::string((സ്ഥിരം അക്ഷരം*)(p + 4), sz - 1);
}
Mat File_കണ്ണി::mat() സ്ഥിരം { Mat value; പഠ(*ഇത്, value, Mat()); തിരികെ value; }

File_കണ്ണി_Iterator File_കണ്ണി::begin() സ്ഥിരം { തിരികെ File_കണ്ണി_Iterator(*ഇത്, അസത്യം); }
File_കണ്ണി_Iterator File_കണ്ണി::end() സ്ഥിരം   { തിരികെ File_കണ്ണി_Iterator(*ഇത്, സത്യം); }

ശൂന്യം File_കണ്ണി::readRaw( സ്ഥിരം std::string& fmt, ശൂന്യം* vec, size_t len ) സ്ഥിരം
{
    File_കണ്ണി_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_കണ്ണി::size() സ്ഥിരം
{
    സ്ഥിരം uchar* p = ptr();
    എങ്കിൽ( !p )
        തിരികെ 0;
    പൂർണ്ണസംഖ്യ tag = *p;
    പൂർണ്ണസംഖ്യ tp = tag & TYPE_MASK;
    എങ്കിൽ( tp == MAP || tp == SEQ )
    {
        എങ്കിൽ( tag & NAMED )
            p += 4;
        തിരികെ (size_t)(unsigned)readInt(p + 5);
    }
    തിരികെ tp != NONE;
}

size_t File_കണ്ണി::rawSize() സ്ഥിരം
{
    സ്ഥിരം uchar* p0 = ptr(), *p = p0;
    എങ്കിൽ( !p )
        തിരികെ 0;
    പൂർണ്ണസംഖ്യ tag = *p++;
    പൂർണ്ണസംഖ്യ tp = tag & TYPE_MASK;
    എങ്കിൽ( tag & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    എങ്കിൽ( tp == INT )
        തിരികെ sz0 + 8;
    എങ്കിൽ( tp == REAL )
        തിരികെ sz0 + 8;
    എങ്കിൽ( tp == NONE )
        തിരികെ sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    തിരികെ sz0 + 4 + readInt(p);
}

uchar* File_കണ്ണി::ptr()
{
    തിരികെ !fs ? 0 : (uchar*)fs->get_കണ്ണി_Ptr(blockIdx, ofs);
}

സ്ഥിരം uchar* File_കണ്ണി::ptr() സ്ഥിരം
{
    തിരികെ !fs ? 0 : fs->get_കണ്ണി_Ptr(blockIdx, ofs);
}

ശൂന്യം File_കണ്ണി::set_മൂല്യം( പൂർണ്ണസംഖ്യ type, സ്ഥിരം ശൂന്യം* value, പൂർണ്ണസംഖ്യ len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    പൂർണ്ണസംഖ്യ tag = *p;
    പൂർണ്ണസംഖ്യ current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    പൂർണ്ണസംഖ്യ sz = 1;

    എങ്കിൽ( tag & NAMED )
        sz += 4;

    എങ്കിൽ( type == INT )
        sz += 8;
    അല്ലെങ്കിൽ എങ്കിൽ( type == REAL )
        sz += 8;
    അല്ലെങ്കിൽ എങ്കിൽ( type == STRING )
    {
        എങ്കിൽ( len < 0 )
            len = (പൂർണ്ണസംഖ്യ)strlen((സ്ഥിരം അക്ഷരം*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_കണ്ണി_Space(*ഇത്, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    എങ്കിൽ( tag & NAMED )
        p += 4;

    എങ്കിൽ( type == INT )
    {
        int64_t ival = *(സ്ഥിരം int64_t*)value;
        writeInt(p, ival);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( type == REAL )
    {
        ഇരട്ട dbval = *(സ്ഥിരം ഇരട്ട*)value;
        writeReal(p, dbval);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( type == STRING )
    {
        സ്ഥിരം അക്ഷരം* str = (സ്ഥിരം അക്ഷരം*)value;
        writeInt(p, len + 1);
        memcpy(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_കണ്ണി_Iterator::File_കണ്ണി_Iterator()
{
    fs = 0;
    blockIdx = 0;
    ofs = 0;
    blockSize = 0;
    കണ്ണി_NElems = 0;
    idx = 0;
}

File_കണ്ണി_Iterator::File_കണ്ണി_Iterator( സ്ഥിരം File_കണ്ണി& കണ്ണി, തർക്കം seekEnd )
{
    fs = കണ്ണി.fs;
    idx = 0;
    എങ്കിൽ( !fs )
        blockIdx = ofs = blockSize = കണ്ണി_NElems = 0;
    അല്ലെങ്കിൽ
    {
        blockIdx = കണ്ണി.blockIdx;
        ofs = കണ്ണി.ofs;

        തർക്കം collection = കണ്ണി.isSeq() || കണ്ണി.isMap();
        എങ്കിൽ( കണ്ണി.isNone() )
        {
            കണ്ണി_NElems = 0;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( !collection )
        {
            കണ്ണി_NElems = 1;
            എങ്കിൽ( seekEnd )
            {
                idx = 1;
                ofs += കണ്ണി.rawSize();
            }
        }
        അല്ലെങ്കിൽ
        {
            കണ്ണി_NElems = കണ്ണി.size();
            സ്ഥിരം uchar* p0 = കണ്ണി.ptr(), *p = p0 + 1;
            എങ്കിൽ(*p0 & File_കണ്ണി::NAMED )
                p += 4;
            എങ്കിൽ( !seekEnd )
                ofs += (p - p0) + 8;
            അല്ലെങ്കിൽ
            {
                size_t rawsz = (size_t)(unsigned)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = കണ്ണി_NElems;
            }
        }
        fs->normalize_കണ്ണി_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
}

File_കണ്ണി_Iterator::File_കണ്ണി_Iterator(സ്ഥിരം File_കണ്ണി_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    കണ്ണി_NElems = it.കണ്ണി_NElems;
    idx = it.idx;
}

File_കണ്ണി_Iterator& File_കണ്ണി_Iterator::operator=(സ്ഥിരം File_കണ്ണി_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    കണ്ണി_NElems = it.കണ്ണി_NElems;
    idx = it.idx;
    തിരികെ *ഇത്;
}

File_കണ്ണി File_കണ്ണി_Iterator::operator *() സ്ഥിരം
{
    തിരികെ File_കണ്ണി(idx < കണ്ണി_NElems ? fs : ശൂന്യ, blockIdx, ofs);
}

File_കണ്ണി_Iterator& File_കണ്ണി_Iterator::operator ++ ()
{
    എങ്കിൽ( idx == കണ്ണി_NElems || !fs )
        തിരികെ *ഇത്;
    idx++;
    File_കണ്ണി n(fs, blockIdx, ofs);
    ofs += n.rawSize();
    എങ്കിൽ( ofs >= blockSize )
    {
        fs->normalize_കണ്ണി_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
    തിരികെ *ഇത്;
}

File_കണ്ണി_Iterator File_കണ്ണി_Iterator::operator ++ (പൂർണ്ണസംഖ്യ)
{
    File_കണ്ണി_Iterator it = *ഇത്;
    ++(*ഇത്);
    തിരികെ it;
}

File_കണ്ണി_Iterator& File_കണ്ണി_Iterator::operator += (പൂർണ്ണസംഖ്യ _ofs)
{
    CV_Assert( _ofs >= 0 );
    വേണ്ടി( ; _ofs > 0; _ofs-- )
        ഇത്->operator ++();
    തിരികെ *ഇത്;
}

File_കണ്ണി_Iterator& File_കണ്ണി_Iterator::readRaw( സ്ഥിരം String& fmt, ശൂന്യം* _data0, size_t maxsz)
{
    എങ്കിൽ( fs && idx < കണ്ണി_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        പൂർണ്ണസംഖ്യ fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        പൂർണ്ണസംഖ്യ fmt_pair_എണ്ണം = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        size_t esz = fs::calcStructSize( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        വേണ്ടി( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < fmt_pair_എണ്ണം; k++ )
            {
                പൂർണ്ണസംഖ്യ elem_type = fmt_pairs[k*2+1];
                പൂർണ്ണസംഖ്യ elem_size = CV_ELEM_SIZE(elem_type);

                പൂർണ്ണസംഖ്യ എണ്ണം_2 = fmt_pairs[k*2];
                offset = alignSize( offset, elem_size );
                uchar* data = data0 + offset;

                വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം_2; i++, ++(*ഇത്) )
                {
                    File_കണ്ണി കണ്ണി = *(*ഇത്);
                    എങ്കിൽ( കണ്ണി.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_size == 8 ? (int64_t)കണ്ണി : (പൂർണ്ണസംഖ്യ)കണ്ണി);
                        തിരഞ്ഞെടുപ്പ്( elem_type )
                        {
                        അവസ്ഥ CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(ival);
                            data++;
                            നിർത്തുക;
                        അവസ്ഥ CV_8S:
                            *(അക്ഷരം*)data = saturate_cast<schar>(ival);
                            data++;
                            നിർത്തുക;
                        അവസ്ഥ CV_Bool:
                            *(തർക്കം*)data = ival != 0;
                            data++;
                            നിർത്തുക;
                        അവസ്ഥ CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(ival);
                            data += വലുപ്പം(ushort);
                            നിർത്തുക;
                        അവസ്ഥ CV_16S:
                            *(short*)data = saturate_cast<short>(ival);
                            data += വലുപ്പം(short);
                            നിർത്തുക;
                        അവസ്ഥ CV_32U:
                            *(unsigned*)data = (unsigned)std::max(ival, (int64_t)0);
                            data += വലുപ്പം(unsigned);
                            നിർത്തുക;
                        അവസ്ഥ CV_32S:
                            *(പൂർണ്ണസംഖ്യ*)data = (പൂർണ്ണസംഖ്യ)ival;
                            data += വലുപ്പം(പൂർണ്ണസംഖ്യ);
                            നിർത്തുക;
                        അവസ്ഥ CV_32F:
                            *(ദശാംശം*)data = (ദശാംശം)ival;
                            data += വലുപ്പം(ദശാംശം);
                            നിർത്തുക;
                        അവസ്ഥ CV_64U:
                            *(uint64_t*)data = (uint64_t)ival;
                            data += വലുപ്പം(uint64_t);
                            നിർത്തുക;
                        അവസ്ഥ CV_64S:
                            *(int64_t*)data = (int64_t)ival;
                            data += വലുപ്പം(int64_t);
                            നിർത്തുക;
                        അവസ്ഥ CV_64F:
                            *(ഇരട്ട*)data = (ഇരട്ട)ival;
                            data += വലുപ്പം(ഇരട്ട);
                            നിർത്തുക;
                        അവസ്ഥ CV_16F:
                            *(hfloat*)data = hfloat((ദശാംശം)ival);
                            data += വലുപ്പം(hfloat);
                            നിർത്തുക;
                        അവസ്ഥ CV_16BF:
                            *(bfloat*)data = bfloat((ദശാംശം)ival);
                            data += വലുപ്പം(bfloat);
                            നിർത്തുക;
                        അവസ്ഥ CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((ദശാംശം)ival);
                            data += വലുപ്പം(fp8_t);
                            നിർത്തുക;
                        അവസ്ഥ CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((ദശാംശം)ival);
                            data += വലുപ്പം(fp8a_t);
                            നിർത്തുക;
                        സ്ഥിരസ്ഥിതി:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    അല്ലെങ്കിൽ എങ്കിൽ( കണ്ണി.isReal() )
                    {
                        ഇരട്ട fval = (ഇരട്ട)കണ്ണി;

                        തിരഞ്ഞെടുപ്പ്( elem_type )
                        {
                        അവസ്ഥ CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(fval);
                            data++;
                            നിർത്തുക;
                        അവസ്ഥ CV_8S:
                            *(അക്ഷരം*)data = saturate_cast<schar>(fval);
                            data++;
                            നിർത്തുക;
                        അവസ്ഥ CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(fval);
                            data += വലുപ്പം(ushort);
                            നിർത്തുക;
                        അവസ്ഥ CV_16S:
                            *(short*)data = saturate_cast<short>(fval);
                            data += വലുപ്പം(short);
                            നിർത്തുക;
                        അവസ്ഥ CV_32U:
                            *(പൂർണ്ണസംഖ്യ*)data = saturate_cast<unsigned>(fval);
                            data += വലുപ്പം(പൂർണ്ണസംഖ്യ);
                            നിർത്തുക;
                        അവസ്ഥ CV_32S:
                            *(പൂർണ്ണസംഖ്യ*)data = saturate_cast<പൂർണ്ണസംഖ്യ>(fval);
                            data += വലുപ്പം(പൂർണ്ണസംഖ്യ);
                            നിർത്തുക;
                        അവസ്ഥ CV_32F:
                            *(ദശാംശം*)data = (ദശാംശം)fval;
                            data += വലുപ്പം(ദശാംശം);
                            നിർത്തുക;
                        അവസ്ഥ CV_64U:
                            *(uint64_t*)data = (uint64_t)round(std::max(fval, 0.));
                            data += വലുപ്പം(uint64_t);
                            നിർത്തുക;
                        അവസ്ഥ CV_64S:
                            *(int64_t*)data = (int64_t)round(std::max(fval, 0.));
                            data += വലുപ്പം(int64_t);
                            നിർത്തുക;
                        അവസ്ഥ CV_64F:
                            *(ഇരട്ട*)data = fval;
                            data += വലുപ്പം(ഇരട്ട);
                            നിർത്തുക;
                        അവസ്ഥ CV_16F:
                            *(hfloat*)data = hfloat((ദശാംശം)fval);
                            data += വലുപ്പം(hfloat);
                            നിർത്തുക;
                        അവസ്ഥ CV_16BF:
                            *(bfloat*)data = bfloat((ദശാംശം)fval);
                            data += വലുപ്പം(bfloat);
                            നിർത്തുക;
                        അവസ്ഥ CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((ദശാംശം)fval);
                            data += വലുപ്പം(fp8_t);
                            നിർത്തുക;
                        അവസ്ഥ CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((ദശാംശം)fval);
                            data += വലുപ്പം(fp8a_t);
                            നിർത്തുക;
                        സ്ഥിരസ്ഥിതി:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    അല്ലെങ്കിൽ
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (പൂർണ്ണസംഖ്യ)(data - data0);
            }
        }
    }

    തിരികെ *ഇത്;
}

തർക്കം File_കണ്ണി_Iterator::equalTo(സ്ഥിരം File_കണ്ണി_Iterator& it) സ്ഥിരം
{
    തിരികെ fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && കണ്ണി_NElems == it.കണ്ണി_NElems;
}

size_t File_കണ്ണി_Iterator::remaining() സ്ഥിരം
{
    തിരികെ കണ്ണി_NElems - idx;
}

തർക്കം operator == ( സ്ഥിരം File_കണ്ണി_Iterator& it1, സ്ഥിരം File_കണ്ണി_Iterator& it2 )
{
    തിരികെ it1.equalTo(it2);
}

തർക്കം operator != ( സ്ഥിരം File_കണ്ണി_Iterator& it1, സ്ഥിരം File_കണ്ണി_Iterator& it2 )
{
    തിരികെ !it1.equalTo(it2);
}

ശൂന്യം പഠ(സ്ഥിരം File_കണ്ണി& കണ്ണി, പൂർണ്ണസംഖ്യ& val, പൂർണ്ണസംഖ്യ default_val)
{
    val = default_val;
    എങ്കിൽ( !കണ്ണി.empty() )
    {
        val = (പൂർണ്ണസംഖ്യ)കണ്ണി;
    }
}

ശൂന്യം പഠ(സ്ഥിരം File_കണ്ണി& കണ്ണി, int64_t& val, int64_t default_val)
{
    val = default_val;
    എങ്കിൽ( !കണ്ണി.empty() )
    {
        val = (int64_t)കണ്ണി;
    }
}

ശൂന്യം പഠ(സ്ഥിരം File_കണ്ണി& കണ്ണി, ഇരട്ട& val, ഇരട്ട default_val)
{
    val = default_val;
    എങ്കിൽ( !കണ്ണി.empty() )
    {
        val = (ഇരട്ട)കണ്ണി;
    }
}

ശൂന്യം പഠ(സ്ഥിരം File_കണ്ണി& കണ്ണി, ദശാംശം& val, ദശാംശം default_val)
{
    val = default_val;
    എങ്കിൽ( !കണ്ണി.empty() )
    {
        val = (ദശാംശം)കണ്ണി;
    }
}

ശൂന്യം പഠ(സ്ഥിരം File_കണ്ണി& കണ്ണി, std::string& val, സ്ഥിരം std::string& default_val)
{
    val = default_val;
    എങ്കിൽ( !കണ്ണി.empty() )
    {
        val = (std::string)കണ്ണി;
    }
}
ശൂന്യം FileStorage::Impl::ലിഖ(സ്ഥിരം String &താക്കോൽ, തർക്കം value)
{
    CV_Assert(write_mode);
    എങ്കിൽ (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().ലിഖ(താക്കോൽ.c_str(), (പൂർണ്ണസംഖ്യ)value);
    }
    അല്ലെങ്കിൽ
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().ലിഖ(താക്കോൽ.c_str(), value ? "true" : "false", അസത്യം);
    }
}

FileStorage_API::~FileStorage_API() {}

നാമാകാശ internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, സ്ഥിരം std::string& name,
                                       പൂർണ്ണസംഖ്യ flags, സ്ഥിരം std::string& typeName)
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
