// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "persistence.hpp"
#ଅନ୍ତର୍ଭାଵ "persistence_impl.hpp"
#ଅନ୍ତର୍ଭାଵ "persistence_base64_encoding.hpp"
#ଅନ୍ତର୍ଭାଵ <unordered_map>
#ଅନ୍ତର୍ଭାଵ <iterator>

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.hpp>

#define CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

ନାମାକାଶ cv
{

ନାମାକାଶ fs
{

ପୂର୍ଣ୍ଣସଂଖ୍ୟା strcasecmp(ଅଚଳ ଅକ୍ଷର* s1, ଅଚଳ ଅକ୍ଷର* s2)
{
    ଅଚଳ ଅକ୍ଷର* dummy="";
    ଯଦି(!s1) s1=dummy;
    ଯଦି(!s2) s2=dummy;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t i, len = std::min(len1, len2);
    ପାଇଁ( i = 0; i < len; i++ )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = tolower((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)s1[i]) - tolower((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)s2[i]);
        ଯଦି( d != 0 )
            ଫେରନ୍ତୁ d;
    }
    ଫେରନ୍ତୁ len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

ଅକ୍ଷର* itoa( ପୂର୍ଣ୍ଣସଂଖ୍ୟା _val, ଅକ୍ଷର* buffer, ପୂର୍ଣ୍ଣସଂଖ୍ୟା /*radix*/ )
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା radix = 10;
    ଅକ୍ଷର* ptr=buffer + 23 /* enough even for 64-bit integers */;
    unsigned val = abs(_val);

    *ptr = '\0';
    କର
    {
        unsigned r = val / radix;
        *--ptr = (ଅକ୍ଷର)(val - (r*radix) + '0');
        val = r;
    }
    ଯେତେବେଳେ( val != 0 );

    ଯଦି( _val < 0 )
        *--ptr = '-';

    ଫେରନ୍ତୁ ptr;
}

ଅକ୍ଷର* itoa( int64_t _val, ଅକ୍ଷର* buffer, ପୂର୍ଣ୍ଣସଂଖ୍ୟା /*radix*/, ତାର୍କିକ _signed)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା radix = 10;
    ଅକ୍ଷର* ptr=buffer + 23 /* enough even for 64-bit integers */;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : abs(_val);

    *ptr = '\0';
    କର
    {
        uint64_t r = val / radix;
        *--ptr = (ଅକ୍ଷର)(val - (r*radix) + '0');
        val = r;
    }
    ଯେତେବେଳେ( val != 0 );

    ଯଦି( sign < 0 )
        *--ptr = '-';

    ଫେରନ୍ତୁ ptr;
}

ଅକ୍ଷର* doubleToString( ଅକ୍ଷର* buf, size_t bufSize, ଦ୍ୱିଗୁଣ value, ତାର୍କିକ explicitZero )
{
    Cv64suf val;
    unsigned ieee754_hi;

    val.f = value;
    ieee754_hi = (unsigned)(val.u >> 32);

    ଯଦି( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ivalue = cvRound(value);
        ଯଦି( ivalue == value )
        {
            ଯଦି( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            ଅନ୍ୟଥା
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        ଅନ୍ୟଥା
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, bufSize, "%.17g", value );

            ଅକ୍ଷର* ptr = buf;
            ଯଦି( *ptr == '+' || *ptr == '-' )
                ptr++;
            ପାଇଁ( ; cv_isdigit(*ptr); ptr++ )
                ;
            ଯଦି( *ptr == ',' )
                *ptr = '.';
        }
    }
    ଅନ୍ୟଥା
    {
        unsigned ieee754_lo = (unsigned)val.u;
        ଯଦି( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        ଅନ୍ୟଥା
            strcpy( buf, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    ଫେରନ୍ତୁ buf;
}

ଅକ୍ଷର* floatToString( ଅକ୍ଷର* buf, size_t bufSize, ଦଶମିକ value, ତାର୍କିକ halfprecision, ତାର୍କିକ explicitZero )
{
    Cv32suf val;
    unsigned ieee754;
    val.f = value;
    ieee754 = val.u;

    ଯଦି( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ivalue = cvRound(value);
        ଯଦି( ivalue == value )
        {
            ଯଦି( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            ଅନ୍ୟଥା
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        ଅନ୍ୟଥା
        {
            ଯଦି (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.5g", value);
            }
            ଅନ୍ୟଥା
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.9g", value);
            }

            ଅକ୍ଷର* ptr = buf;
            ଯଦି( *ptr == '+' || *ptr == '-' )
                ptr++;
            ପାଇଁ( ; cv_isdigit(*ptr); ptr++ )
                ;
            ଯଦି( *ptr == ',' )
                *ptr = '.';
        }
    }
    ଅନ୍ୟଥା
    {
        ଯଦି( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        ଅନ୍ୟଥା
            strcpy( buf, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    ଫେରନ୍ତୁ buf;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର symbols[] = "ucwsifdhHbUIneE";

ସ୍ଥିର ଅକ୍ଷର type_ପ୍ରତୀକ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    ଫେରନ୍ତୁ symbols[depth];
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ପ୍ରତୀକ_To_Type(ଅକ୍ଷର c)
{
    ଯଦି (c == 'r')
        ଫେରନ୍ତୁ CV_SEQ_ELTYPE_PTR;
    ଅଚଳ ଅକ୍ଷର* pos = strchr( symbols, c );
    ଯଦି( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    ଫେରନ୍ତୁ static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(pos - symbols);
}

ଅକ୍ଷର* encodeFormat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_type, ଅକ୍ଷର* dt, size_t dt_len)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    ଅକ୍ଷର ପ୍ରତୀକ = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_ପ୍ରତୀକ(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, ପ୍ରତୀକ);
    ଫେରନ୍ତୁ dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
ଅକ୍ଷର* encodeFormat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_type, ଅକ୍ଷର* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    ଫେରନ୍ତୁ encodeFormat(elem_type, dt, max);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା decodeFormat( ଅଚଳ ଅକ୍ଷର* dt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* fmt_pairs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_len )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fmt_pair_ଗଣନା = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0, k = 0, len = dt ? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strlen(dt) : 0;

    ଯଦି( !dt || !len )
        ଫେରନ୍ତୁ 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    ପାଇଁ( ; k < len; k++ )
    {
        ଅକ୍ଷର c = dt[k];

        ଯଦି( cv_isdigit(c) )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3 = c - '0';
            ଯଦି( cv_isdigit(dt[k+1]) )
            {
                ଅକ୍ଷର* endptr = 0;
                ଗଣନା_3 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strtol( dt+k, &endptr, 10 );
                k = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(endptr - dt) - 1;
            }

            ଯଦି( ଗଣନା_3 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = ଗଣନା_3;
        }
        ଅନ୍ୟଥା
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = ପ୍ରତୀକ_To_Type(c);
            ଯଦି( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = depth;
            ଯଦି( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            ଅନ୍ୟଥା
            {
                i += 2;
                ଯଦି( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_ଗଣନା = i/2;
    ଫେରନ୍ତୁ fmt_pair_ଗଣନା;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା calcElemSize( ଅଚଳ ଅକ୍ଷର* dt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା initial_size )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_ଗଣନା;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା comp_size;

    fmt_pair_ଗଣନା = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_ଗଣନା *= 2;
    ପାଇଁ( i = 0, size = initial_size; i < fmt_pair_ଗଣନା; i += 2 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[i+1]);
        size = cvAlign( size, comp_size );
        size += comp_size * fmt_pairs[i];
    }
    ଯଦି( initial_size == 0 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[1]);
        size = cvAlign( size, comp_size );
    }
    ଫେରନ୍ତୁ size;
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା calcStructSize( ଅଚଳ ଅକ୍ଷର* dt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା initial_size )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    ପାଇଁ ( ଅଚଳ ଅକ୍ଷର * type = dt; *type != '\0'; type++ )
    {
        ଅକ୍ଷର v = *type;
        ଯଦି (v >= '0' && v <= '9')
            ଜାରି;  // skip vector size
        ବାଛ (v)
        {
        ସ୍ଥିତି 'u': { elem_max_size = std::max( elem_max_size, ଆକାର(uchar ) ); ଭାଙ୍ଗ; }
        ସ୍ଥିତି 'b': { elem_max_size = std::max( elem_max_size, ଆକାର(ତାର୍କିକ  ) ); ଭାଙ୍ଗ; }
        ସ୍ଥିତି 'c': { elem_max_size = std::max( elem_max_size, ଆକାର(schar ) ); ଭାଙ୍ଗ; }
        ସ୍ଥିତି 'w': { elem_max_size = std::max( elem_max_size, ଆକାର(ushort) ); ଭାଙ୍ଗ; }
        ସ୍ଥିତି 's': { elem_max_size = std::max( elem_max_size, ଆକାର(short ) ); ଭାଙ୍ଗ; }
        ସ୍ଥିତି 'i': { elem_max_size = std::max( elem_max_size, ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା   ) ); ଭାଙ୍ଗ; }
        ସ୍ଥିତି 'n': { elem_max_size = std::max( elem_max_size, ଆକାର(unsigned) ); ଭାଙ୍ଗ; }
        ସ୍ଥିତି 'f': { elem_max_size = std::max( elem_max_size, ଆକାର(ଦଶମିକ ) ); ଭାଙ୍ଗ; }
        ସ୍ଥିତି 'd': { elem_max_size = std::max( elem_max_size, ଆକାର(ଦ୍ୱିଗୁଣ) ); ଭାଙ୍ଗ; }
        ସ୍ଥିତି 'h': { elem_max_size = std::max( elem_max_size, ଆକାର(hfloat)); ଭାଙ୍ଗ; }
        ସ୍ଥିତି 'H': { elem_max_size = std::max( elem_max_size, ଆକାର(bfloat)); ଭାଙ୍ଗ; }
        ସ୍ଥିତି 'e': { elem_max_size = std::max( elem_max_size, ଆକାର(fp8_t) ); ଭାଙ୍ଗ; }
        ସ୍ଥିତି 'E': { elem_max_size = std::max( elem_max_size, ଆକାର(fp8a_t)); ଭାଙ୍ଗ; }
        ସ୍ଥିତି 'I': { elem_max_size = std::max( elem_max_size, ଆକାର(int64_t)); ଭାଙ୍ଗ; }
        ସ୍ଥିତି 'U': { elem_max_size = std::max( elem_max_size, ଆକାର(uint64_t)); ଭାଙ୍ଗ; }
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (ଅକ୍ଷର)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(elem_max_size) );
    ଫେରନ୍ତୁ size;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା decodeSimpleFormat( ଅଚଳ ଅକ୍ଷର* dt )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_type = -1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_ଗଣନା;

    fmt_pair_ଗଣନା = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    ଯଦି( fmt_pair_ଗଣନା != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    ଫେରନ୍ତୁ elem_type;
}

}

#ଯଦି defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#define CV_LITTLE_ENDIAN_MEM_ACCESS 1
#ଅନ୍ୟଥା
#define CV_LITTLE_ENDIAN_MEM_ACCESS 0
#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା readInt(ଅଚଳ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ଯଦି CV_LITTLE_ENDIAN_MEM_ACCESS
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା val;
    memcpy(&val, p, ଆକାର(val));
    ଫେରନ୍ତୁ val;
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା val = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    ଫେରନ୍ତୁ val;
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ int64_t readLong(ଅଚଳ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ଯଦି CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    memcpy(&val, p, ଆକାର(val));
    ଫେରନ୍ତୁ val;
#ଅନ୍ୟଥା
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    ଫେରନ୍ତୁ val0 | ((int64_t)val1 << 32);
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ readReal(ଅଚଳ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ଯଦି CV_LITTLE_ENDIAN_MEM_ACCESS
    ଦ୍ୱିଗୁଣ val;
    memcpy(&val, p, ଆକାର(val));
    ଫେରନ୍ତୁ val;
#ଅନ୍ୟଥା
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    ଫେରନ୍ତୁ val.f;
#endif
}

template <typename T>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ଯଦି CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &ival, ଆକାର(ival));
#ଅନ୍ୟଥା
    ପାଇଁ (size_t i = 0, j = 0; i < ଆକାର(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ writeReal(uchar* p, ଦ୍ୱିଗୁଣ fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ଯଦି CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &fval, ଆକାର(fval));
#ଅନ୍ୟଥା
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



ଶୂନ୍ୟ FileStorage::Impl::init() {
    flags = 0;
    buffer.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = ମିଥ୍ୟା;
    state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = ମିଥ୍ୟା;
    delayed_struct_ଚାବି = nullptr;
    delayed_struct_flags = 0;
    delayed_type_name = nullptr;
    base64_writer = nullptr;
    is_opened = ମିଥ୍ୟା;
    dummy_eof = ମିଥ୍ୟା;
    write_mode = ମିଥ୍ୟା;
    mem_mode = ମିଥ୍ୟା;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    empty_stream = ସତ୍ୟ;

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

ଶୂନ୍ୟ FileStorage::Impl::release(String *out) {
    ଯଦି (is_opened) {
        ଯଦି (out)
            out->clear();
        ଯଦି (write_mode) {
            ଯେତେବେଳେ (write_ଗଦା.size() > 1) {
                endWriteStruct();
            }
            flush();
            ଯଦି (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            ଅନ୍ୟଥା ଯଦି (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        ଯଦି (mem_mode && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

ଶୂନ୍ୟ FileStorage::Impl::analyze_file_name(ଅଚଳ std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର not_file_name = '\n';
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର parameter_begin = '?';
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର parameter_separator = '&';

    ଯଦି (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        ଫେରନ୍ତୁ;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    ଯଦି (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        ପାଇଁ (size_t param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            ଯଦି ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

ତାର୍କିକ FileStorage::Impl::open(ଅଚଳ ଅକ୍ଷର *filename_or_buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _flags, ଅଚଳ ଅକ୍ଷର *encoding) {
    ତାର୍କିକ ok = ସତ୍ୟ;
    release();

    ତାର୍କିକ append = (_flags & 3) == FileStorage::APPEND;
    mem_mode = (_flags & FileStorage::MEMORY) != 0;

    write_mode = (_flags & 3) != 0;
    ତାର୍କିକ write_base64 = (write_mode || append) && (_flags & FileStorage::BASE64) != 0;

    ତାର୍କିକ isGZ = ମିଥ୍ୟା;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        ଯଦି (!params.empty())
            filename = params[0];

        ଯଦି (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_mode || append);
    }

    ଯଦି (filename.size() == 0 && !mem_mode && !write_mode)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    ଯଦି (mem_mode && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    ଯଦି (!mem_mode) {
        size_t dot_idx = filename.find_last_of('.');
        ଅକ୍ଷର compression = '\0';

        ଯଦି (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            ଯଦି (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                ଯଦି (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = ସତ୍ୟ;
                }

                ଅନ୍ୟଥା ଯଦି(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = ସତ୍ୟ;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        ଯଦି (!isGZ) {
            file = fopen(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            ଯଦି (!file)
            {
                CV_LOG_ERROR(ଖାଲି, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
            }
        } ଅନ୍ୟଥା {
#ଯଦି USE_ZLIB
            ଯଦି (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            ଅକ୍ଷର mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            ଯଦି (!gzfile)
            {
                CV_LOG_ERROR(ଖାଲି, "Can't open archive: '" << filename << "' mode=" << mode);
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
            }
#ଅନ୍ୟଥା
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#endif
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_data.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    ଯଦି (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        ଯଦି (mem_mode)
            outbuf.clear();

        ଯଦି (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            ଅଚଳ ଅକ୍ଷର *dot_pos = ଖାଲି;
            ଅଚଳ ଅକ୍ଷର *dot_pos2 = ଖାଲି;
            // like strrchr() implementation, but save two last positions simultaneously
            ପାଇଁ (ଅଚଳ ଅକ୍ଷର *pos = &filename[0]; pos[0] != 0; pos++) {
                ଯଦି (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            ଯଦି (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != ଖାଲି) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } ଅନ୍ୟଥା ଯଦି (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା buf_size = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        ଯଦି (append) {
            fseek(file, 0, SEEK_END);
            ଯଦି (ftell(file) == 0)
                append = ମିଥ୍ୟା;
        }

        write_ଗଦା.clear();
        empty_stream = ସତ୍ୟ;
        write_ଗଦା.push_back(FStructData("", File_ଗଣ୍ଠି::MAP | File_ଗଣ୍ଠି::EMPTY, 0));
        buffer.reserve(buf_size + 1024);
        buffer.resize(buf_size);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;

        ଯଦି (fmt == FileStorage::FORMAT_XML) {
            size_t file_size = file ? (size_t) ftell(file) : (size_t) 0;
            ଯଦି (!append || file_size == 0) {
                ଯଦି (encoding && *encoding != '\0') {
                    ଯଦି (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(strlen(encoding) < 1000);
                    ଅକ୍ଷର buf[1100];
                    snprintf(buf, ଆକାର(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } ଅନ୍ୟଥା
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } ଅନ୍ୟଥା {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା xml_buf_size = 1 << 10;
                ଅକ୍ଷର substr[] = "</opencv_storage>";
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_occurrence = -1;
                xml_buf_size = MIN(xml_buf_size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା(file_size));
                fseek(file, -xml_buf_size, SEEK_END);
                // find the last occurrence of </opencv_storage>
                ପାଇଁ (;;) {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_offset = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ftell(file);
                    ଅଚଳ ଅକ୍ଷର *ptr0 = ଏହି->gets(xml_buf_size);
                    ଅଚଳ ଅକ୍ଷର *ptr = ଖାଲି;
                    ଯଦି (!ptr0)
                        ଭାଙ୍ଗ;
                    ptr = ptr0;
                    ପାଇଁ (;;) {
                        ptr = strstr(ptr, substr);
                        ଯଦି (!ptr)
                            ଭାଙ୍ଗ;
                        last_occurrence = ଧାଡ଼ି_offset + (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (ptr - ptr0);
                        ptr += strlen(substr);
                    }
                }
                ଯଦି (last_occurrence < 0) {
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

            emitter_do_not_use_direct_dereference = createXMLEmitter(ଏହି);
        } ଅନ୍ୟଥା ଯଦି (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                ଯଦି (!append) {
                    ଯଦି (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    ଅନ୍ୟଥା
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            ଅନ୍ୟଥା
                puts("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(ଏହି);
        } ଅନ୍ୟଥା {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            ଯଦି (!append)
                puts("{\n");
            ଅନ୍ୟଥା {
                ତାର୍କିକ valid = ମିଥ୍ୟା;
                long roffset = 0;
                ପାଇଁ (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    ଅଚଳ ଅକ୍ଷର end_mark = '}';
                    ଯଦି (fgetc(file) == end_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = ସତ୍ୟ;
                        ଭାଙ୍ଗ;
                    }
                }

                ଯଦି (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } ଅନ୍ୟଥା {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_ଗଦା.back().indent = 4;
            emitter_do_not_use_direct_dereference = createJSONEmitter(ଏହି);
        }
        is_opened = ସତ୍ୟ;
    } ଅନ୍ୟଥା {
        ଅଚଳ size_t buf_size0 = 40;
        buffer.resize(buf_size0);
        ଯଦି (mem_mode) {
            strbuf = (ଅକ୍ଷର *) filename_or_buf;
            strbufsize = strlen(strbuf);
        }

        ଅଚଳ ଅକ୍ଷର *yaml_signature = "%YAML";
        ଅଚଳ ଅକ୍ଷର *json_signature = "{";
        ଅଚଳ ଅକ୍ଷର *xml_signature = "<?xml";
        ଅକ୍ଷର *buf = ଏହି->gets(16);
        CV_Assert(buf);
        ଅକ୍ଷର *bufPtr = cv_skip_BOM(buf);
        size_t bufOffset = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        ଯଦି (strncmp(bufPtr, yaml_signature, strlen(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        ଅନ୍ୟଥା ଯଦି (strncmp(bufPtr, json_signature, strlen(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        ଅନ୍ୟଥା ଯଦି (strncmp(bufPtr, xml_signature, strlen(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        ଅନ୍ୟଥା
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = bufOffset;
        bufofs = 0;

        ଚେଷ୍ଟା {
            ଅକ୍ଷର *ptr = bufferStart();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_ଗଣ୍ଠି ମୂଳ_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_ଗଣ୍ଠି_Space(ମୂଳ_nodes, 9);
            *rptr = File_ଗଣ୍ଠି::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            ବାଛ (fmt) {
                ସ୍ଥିତି FileStorage::FORMAT_XML:
                    parser_do_not_use_direct_dereference = createXMLParser(ଏହି);
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି FileStorage::FORMAT_YAML:
                ସ୍ଥିତି FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_direct_dereference = createYAMLParser(ଏହି);
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି FileStorage::FORMAT_JSON:
                    parser_do_not_use_direct_dereference = createJSONParser(ଏହି);
                    ଭାଙ୍ଗ;
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                    parser_do_not_use_direct_dereference = Ptr<FileStorageParser>();
            }

            ଯଦି (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                ଯଦି (ok) {
                    finalizeCollection(ମୂଳ_nodes);

                    CV_Assert(!fs_data_ptrs.empty());
                    File_ଗଣ୍ଠି roots_ଗଣ୍ଠି(fs_ext, 0, 0);
                    size_t i, nroots = roots_ଗଣ୍ଠି.size();
                    File_ଗଣ୍ଠି_Iterator it = roots_ଗଣ୍ଠି.begin();

                    ପାଇଁ (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        ଧର (...)
        {
            // FIXIT log error message
            is_opened = ସତ୍ୟ;
            release();
            ଫିଙ୍ଗ;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = ସତ୍ୟ;
        std::vector<ଅକ୍ଷର> tmpbuf;
        std::swap(buffer, tmpbuf);
        bufofs = 0;
    }
    ଫେରନ୍ତୁ ok;
}

ଶୂନ୍ୟ FileStorage::Impl::puts(ଅଚଳ ଅକ୍ଷର *str) {
    CV_Assert(write_mode);
    ଯଦି (mem_mode)
        std::copy(str, str + strlen(str), std::back_inserter(outbuf));
    ଅନ୍ୟଥା ଯଦି (file)
        fputs(str, file);
#ଯଦି USE_ZLIB
    ଅନ୍ୟଥା ଯଦି (gzfile)
        gzputs(gzfile, str);
#endif
    ଅନ୍ୟଥା
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

ଅକ୍ଷର *FileStorage::Impl::getsFromFile(ଅକ୍ଷର *buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3) {
    ଯଦି (file)
        ଫେରନ୍ତୁ fgets(buf, ଗଣନା_3, file);
#ଯଦି USE_ZLIB
    ଯଦି (gzfile)
        ଫେରନ୍ତୁ gzgets(gzfile, buf, ଗଣନା_3);
#endif
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

ଅକ୍ଷର *FileStorage::Impl::gets(size_t max_ଗଣନା) {
    ଯଦି (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        ଅଚଳ ଅକ୍ଷର *instr = strbuf;
        ପାଇଁ (; i < len; i++) {
            ଅକ୍ଷର c = instr[i];
            ଯଦି (c == '\0' || c == '\n') {
                ଯଦି (c == '\n')
                    i++;
                ଭାଙ୍ଗ;
            }
        }
        size_t ଗଣନା_3 = i - strbufpos;
        ଯଦି (max_ଗଣନା == 0 || max_ଗଣନା > ଗଣନା_3)
            max_ଗଣନା = ଗଣନା_3;
        buffer.resize(std::max(buffer.size(), max_ଗଣନା + 8));
        memcpy(&buffer[0], instr + strbufpos, max_ଗଣନା);
        buffer[max_ଗଣନା] = '\0';
        strbufpos = i;
        ଫେରନ୍ତୁ max_ଗଣନା > 0 ? &buffer[0] : 0;
    }

    ଅଚଳ size_t MAX_BLOCK_SIZE = INT_MAX / 2; // hopefully, that will be enough
    ଯଦି (max_ଗଣନା == 0)
        max_ଗଣନା = MAX_BLOCK_SIZE;
    ଅନ୍ୟଥା
        CV_Assert(max_ଗଣନା < MAX_BLOCK_SIZE);
    size_t ofs = 0;

    ପାଇଁ (;;) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) std::min(buffer.size() - ofs - 16, max_ଗଣନା);
        ଅକ୍ଷର *ptr = getsFromFile(&buffer[ofs], ଗଣନା_3 + 1);
        ଯଦି (!ptr)
            ଭାଙ୍ଗ;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) strlen(ptr);
        ofs += delta;
        max_ଗଣନା -= delta;
        ଯଦି (delta == 0 || ptr[delta - 1] == '\n' || max_ଗଣନା == 0)
            ଭାଙ୍ଗ;
        ଯଦି (delta == ଗଣନା_3)
            buffer.resize((size_t) (buffer.size() * 1.5));
    }
    ଫେରନ୍ତୁ ofs > 0 ? &buffer[0] : 0;
}

ଅକ୍ଷର *FileStorage::Impl::gets() {
    ଅକ୍ଷର *ptr = ଏହି->gets(0);
    ଯଦି (!ptr) {
        ptr = bufferStart();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        ଫେରନ୍ତୁ 0;
    } ଅନ୍ୟଥା {
        size_t l = strlen(ptr);
        ଯଦି (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    ଫେରନ୍ତୁ ptr;
}

ତାର୍କିକ FileStorage::Impl::eof() {
    ଯଦି (dummy_eof)
        ଫେରନ୍ତୁ ସତ୍ୟ;
    ଯଦି (strbuf)
        ଫେରନ୍ତୁ strbufpos >= strbufsize;
    ଯଦି (file)
        ଫେରନ୍ତୁ feof(file) != 0;
#ଯଦି USE_ZLIB
    ଯଦି (gzfile)
        ଫେରନ୍ତୁ gzeof(gzfile) != 0;
#endif
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ଶୂନ୍ୟ FileStorage::Impl::setEof() {
    dummy_eof = ସତ୍ୟ;
}

ଶୂନ୍ୟ FileStorage::Impl::closeFile() {
    ଯଦି (file)
        fclose(file);
#ଯଦି USE_ZLIB
    ଅନ୍ୟଥା ଯଦି (gzfile)
        gzclose(gzfile);
#endif
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = ମିଥ୍ୟା;
}

ଶୂନ୍ୟ FileStorage::Impl::rewind() {
    ଯଦି (file)
        ::rewind(file);
#ଯଦି USE_ZLIB
    ଅନ୍ୟଥା ଯଦି (gzfile)
        gzrewind(gzfile);
#endif
    strbufpos = 0;
}

ଅକ୍ଷର *FileStorage::Impl::resizeWriteBuffer(ଅକ୍ଷର *ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) {
    ଅଚଳ ଅକ୍ଷର *buffer_end = &buffer[0] + buffer.size();
    ଯଦି (ptr + len < buffer_end)
        ଫେରନ୍ତୁ ptr;

    ଅଚଳ ଅକ୍ଷର *buffer_start = &buffer[0];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା written_len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (ptr - buffer_start);

    CV_Assert(written_len <= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) buffer.size());
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_size = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ((buffer_end - buffer_start) * 3 / 2);
    new_size = MAX(written_len + len, new_size);
    buffer.reserve(new_size + 256);
    buffer.resize(new_size);
    bufofs = written_len;
    ଫେରନ୍ତୁ &buffer[0] + bufofs;
}

ଅକ୍ଷର *FileStorage::Impl::flush() {
    ଅକ୍ଷର *buffer_start = &buffer[0];
    ଅକ୍ଷର *ptr = buffer_start + bufofs;

    ଯଦି (ptr > buffer_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(buffer_start);
        bufofs = 0;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା indent = write_ଗଦା.back().indent;

    ଯଦି (space != indent) {
        memset(buffer_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = buffer_start + bufofs;

    ଫେରନ୍ତୁ ptr;
}

ଶୂନ୍ୟ FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(ମିଥ୍ୟା);
    ଯଦି (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_ଗଦା.empty());

    FStructData &current_struct = write_ଗଦା.back();
    ଯଦି (fmt == FileStorage::FORMAT_JSON && !File_ଗଣ୍ଠି::isFlow(current_struct.flags) && write_ଗଦା.size() > 1)
        current_struct.indent = write_ଗଦା[write_ଗଦା.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_ଗଦା.pop_back();
    ଯଦି (!write_ଗଦା.empty())
        write_ଗଦା.back().flags &= ~File_ଗଣ୍ଠି::EMPTY;
}

ଶୂନ୍ୟ FileStorage::Impl::startWriteStruct_helper(ଅଚଳ ଅକ୍ଷର *ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags,
                                                ଅଚଳ ଅକ୍ଷର *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (File_ଗଣ୍ଠି::TYPE_MASK | File_ଗଣ୍ଠି::FLOW)) | File_ଗଣ୍ଠି::EMPTY;
    ଯଦି (!File_ଗଣ୍ଠି::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    ଯଦି (type_name && type_name[0] == '\0')
        type_name = 0;

    FStructData s = getEmitter().startWriteStruct(write_ଗଦା.back(), ଚାବି, struct_flags, type_name);

    write_ଗଦା.push_back(s);
    size_t write_ଗଦା_size = write_ଗଦା.size();
    ଯଦି (write_ଗଦା_size > 1)
        write_ଗଦା[write_ଗଦା_size - 2].flags &= ~File_ଗଣ୍ଠି::EMPTY;

    ଯଦି (fmt != FileStorage::FORMAT_JSON && !File_ଗଣ୍ଠି::isFlow(s.flags))
        flush();

    ଯଦି (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_ଗଣ୍ଠି::isMap(struct_flags)) {
        getEmitter().ଲିଖ("type_id", type_name, ମିଥ୍ୟା);
    }
}

ଶୂନ୍ୟ FileStorage::Impl::startWriteStruct(ଅଚଳ ଅକ୍ଷର *ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags,
                                         ଅଚଳ ଅକ୍ଷର *type_name) {
    check_if_write_struct_is_delayed(ମିଥ୍ୟା);
    ଯଦି (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    ଯଦି (state_of_writing_base64 == FileStorage_API::Uncertain && File_ଗଣ୍ଠି::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(ଚାବି, struct_flags, type_name);
    } ଅନ୍ୟଥା ଯଦି (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        ଯଦି ((File_ଗଣ୍ଠି::TYPE_MASK & struct_flags) != File_ଗଣ୍ଠି::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        ଅନ୍ୟଥା ଯଦି (state_of_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(ଚାବି, struct_flags, "binary");

        ଯଦି (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::InUse);
    } ଅନ୍ୟଥା {
        /* Won't output Base64 data */
        ଯଦି (state_of_writing_base64 == FileStorage_API::InUse)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(ଚାବି, struct_flags, type_name);

        ଯଦି (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::NotUse);
    }
}

ଶୂନ୍ୟ FileStorage::Impl::writeComment(ଅଚଳ ଅକ୍ଷର *comment, ତାର୍କିକ eol_comment) {
    CV_Assert(write_mode);
    getEmitter().writeComment(comment, eol_comment);
}

ଶୂନ୍ୟ FileStorage::Impl::startNextStream() {
    CV_Assert(write_mode);
    ଯଦି (!empty_stream) {
        ଯେତେବେଳେ (!write_ଗଦା.empty())
            endWriteStruct();
        flush();
        getEmitter().startNextStream();
        empty_stream = ସତ୍ୟ;
        write_ଗଦା.push_back(FStructData("", File_ଗଣ୍ଠି::EMPTY, 0));
        bufofs = 0;
    }
}

ଶୂନ୍ୟ FileStorage::Impl::ଲିଖ(ଅଚଳ String &ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value) {
    CV_Assert(write_mode);
    getEmitter().ଲିଖ(ଚାବି.c_str(), value);
}

ଶୂନ୍ୟ FileStorage::Impl::ଲିଖ(ଅଚଳ String &ଚାବି, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().ଲିଖ(ଚାବି.c_str(), value);
}

ଶୂନ୍ୟ FileStorage::Impl::ଲିଖ(ଅଚଳ String &ଚାବି, ଦ୍ୱିଗୁଣ value) {
    CV_Assert(write_mode);
    getEmitter().ଲିଖ(ଚାବି.c_str(), value);
}

ଶୂନ୍ୟ FileStorage::Impl::ଲିଖ(ଅଚଳ String &ଚାବି, ଅଚଳ String &value) {
    CV_Assert(write_mode);
    getEmitter().ଲିଖ(ଚାବି.c_str(), value.c_str(), ମିଥ୍ୟା);
}

ଶୂନ୍ୟ FileStorage::Impl::writeRawData(ଅଚଳ std::string &dt, ଅଚଳ ଶୂନ୍ୟ *_data, size_t len) {
    CV_Assert(write_mode);

    ଯଦି (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        writeRawDataBase64(_data, len, dt.c_str());
        ଫେରନ୍ତୁ;
    } ଅନ୍ୟଥା ଯଦି (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elemSize = fs::calcStructSize(dt.c_str(), 0);
    CV_Assert(elemSize);
    CV_Assert(len % elemSize == 0);
    len /= elemSize;

    ତାର୍କିକ explicitZero = fmt == FileStorage::FORMAT_JSON;
    ଅଚଳ uchar *data0 = (ଅଚଳ uchar *) _data;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_ଗଣନା;
    ଅକ୍ଷର buf[256] = "";

    fmt_pair_ଗଣନା = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    ଯଦି (!len)
        ଫେରନ୍ତୁ;

    ଯଦି (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    ଯଦି (fmt_pair_ଗଣନା == 1) {
        fmt_pairs[0] *= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) len;
        len = 1;
    }

    ପାଇଁ (; len--; data0 += elemSize) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = 0;
        ପାଇଁ (k = 0; k < fmt_pair_ଗଣନା; k++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ଗଣନା_3 = fmt_pairs[k * 2];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_type = fmt_pairs[k * 2 + 1];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_size = CV_ELEM_SIZE(elem_type);
            ଅଚଳ ଅକ୍ଷର *ptr;

            offset = cvAlign(offset, elem_size);
            ଅଚଳ uchar *data = data0 + offset;

            ପାଇଁ (i = 0; i < ଗଣନା_3; i++) {
                ବାଛ (elem_type) {
                    ସ୍ଥିତି CV_8U:
                        ptr = fs::itoa(*(uchar *) data, buf, 10);
                        data++;
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_Bool:
                        ptr = fs::itoa(*(uchar *) data != 0, buf, 10);
                        data++;
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_8S:
                        ptr = fs::itoa(*(ଅକ୍ଷର *) data, buf, 10);
                        data++;
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_16U:
                        ptr = fs::itoa(*(ushort *) data, buf, 10);
                        data += ଆକାର(ushort);
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_16S:
                        ptr = fs::itoa(*(short *) data, buf, 10);
                        data += ଆକାର(short);
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_32U:
                        ptr = fs::itoa((int64_t)*(unsigned*) data, buf, 10, ମିଥ୍ୟା);
                        data += ଆକାର(unsigned);
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_32S:
                        ptr = fs::itoa(*(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *) data, buf, 10);
                        data += ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_64U:
                        ptr = fs::itoa(*(uint64_t*) data, buf, 10, ମିଥ୍ୟା);
                        data += ଆକାର(uint64_t);
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_64S:
                        ptr = fs::itoa(*(int64_t*) data, buf, 10, ସତ୍ୟ);
                        data += ଆକାର(int64_t);
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_32F:
                        ptr = fs::floatToString(buf, ଆକାର(buf), *(ଦଶମିକ *) data, ମିଥ୍ୟା, explicitZero);
                        data += ଆକାର(ଦଶମିକ);
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_64F:
                        ptr = fs::doubleToString(buf, ଆକାର(buf), *(ଦ୍ୱିଗୁଣ *) data, explicitZero);
                        data += ଆକାର(ଦ୍ୱିଗୁଣ);
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_16F:
                        ptr = fs::floatToString(buf, ଆକାର(buf), (ଦଶମିକ) *(hfloat *) data, ସତ୍ୟ, explicitZero);
                        data += ଆକାର(hfloat);
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_16BF:
                        ptr = fs::floatToString(buf, ଆକାର(buf), (ଦଶମିକ) *(bfloat *) data, ସତ୍ୟ, explicitZero);
                        data += ଆକାର(bfloat);
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, ଆକାର(buf), (ଦଶମିକ) *(fp8_t *) data, ସତ୍ୟ, explicitZero);
                        data += ଆକାର(fp8_t);
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, ଆକାର(buf), (ଦଶମିକ) *(fp8a_t *) data, ସତ୍ୟ, explicitZero);
                        data += ଆକାର(fp8a_t);
                        ଭାଙ୍ଗ;
                    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        ଫେରନ୍ତୁ;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (data - data0);
        }
    }
}

ଶୂନ୍ୟ FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(ମିଥ୍ୟା);

    ଯଦି (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_to_Base64_state(FileStorage_API::Base64State::Uncertain);
}

ଶୂନ୍ୟ FileStorage::Impl::switch_to_Base64_state(FileStorage_API::Base64State new_state) {
    ଅଚଳ ଅକ୍ଷର *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    ଅଚଳ ଅକ୍ଷର *err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    ବାଛ (state_of_writing_base64) {
        ସ୍ଥିତି FileStorage_API::Base64State::Uncertain:
            ବାଛ (new_state) {
                ସ୍ଥିତି FileStorage_API::Base64State::InUse:
                {
                    CV_DbgAssert(base64_writer == 0);
                    ତାର୍କିକ can_indent = (fmt != cv::FileStorage::Mode::FORMAT_JSON);
                    base64_writer = ନୂତନ base64::Base64Writer(*ଏହି, can_indent);
                    ଯଦି (!can_indent) {
                        ଅକ୍ଷର *ptr = bufferPtr();
                        *ptr++ = '\0';
                        puts(bufferStart());
                        setBufferPtr(bufferStart());
                        memset(bufferStart(), 0, static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(space));
                        puts("\"$base64$");
                    }
                    ଭାଙ୍ଗ;
                }
                ସ୍ଥିତି FileStorage_API::Base64State::Uncertain:
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି FileStorage_API::Base64State::NotUse:
                    ଭାଙ୍ଗ;
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ଭାଙ୍ଗ;
            }
            ଭାଙ୍ଗ;
        ସ୍ଥିତି FileStorage_API::Base64State::InUse:
            ବାଛ (new_state) {
                ସ୍ଥିତି FileStorage_API::Base64State::InUse:
                ସ୍ଥିତି FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି FileStorage_API::Base64State::Uncertain:
                    delete base64_writer;
                    base64_writer = 0;
                    ଯଦି ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        setBufferPtr(bufferStart());
                        flush();
                        memset(bufferStart(), 0, static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(space) );
                        setBufferPtr(bufferStart());
                    }
                    ଭାଙ୍ଗ;
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ଭାଙ୍ଗ;
            }
            ଭାଙ୍ଗ;
        ସ୍ଥିତି FileStorage_API::Base64State::NotUse:
            ବାଛ (new_state) {
                ସ୍ଥିତି FileStorage_API::Base64State::InUse:
                ସ୍ଥିତି FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    ଭାଙ୍ଗ;
                ସ୍ଥିତି FileStorage_API::Base64State::Uncertain:
                    ଭାଙ୍ଗ;
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ଭାଙ୍ଗ;
            }
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            ଭାଙ୍ଗ;
    }

    state_of_writing_base64 = new_state;
}

ଶୂନ୍ୟ FileStorage::Impl::make_write_struct_delayed(ଅଚଳ ଅକ୍ଷର *ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags, ଅଚଳ ଅକ୍ଷର *type_name) {
    CV_Assert(is_write_struct_delayed == ମିଥ୍ୟା);
    CV_DbgAssert(delayed_struct_ଚାବି == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    ଯଦି (ଚାବି != nullptr) {
        delayed_struct_ଚାବି = ନୂତନ ଅକ୍ଷର[strlen(ଚାବି) + 1U];
        strcpy(delayed_struct_ଚାବି, ଚାବି);
    }

    ଯଦି (type_name != nullptr) {
        delayed_type_name = ନୂତନ ଅକ୍ଷର[strlen(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = ସତ୍ୟ;
}

ଶୂନ୍ୟ FileStorage::Impl::check_if_write_struct_is_delayed(ତାର୍କିକ change_type_to_base64) {
    ଯଦି (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_ଚାବି;
        std::string type_name;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags = delayed_struct_flags;

        ଯଦି (delayed_struct_ଚାବି != nullptr && *delayed_struct_ଚାବି != '\0') {
            struct_ଚାବି.assign(delayed_struct_ଚାବି);
        }
        ଯଦି (delayed_type_name != nullptr && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        delete[] delayed_struct_ଚାବି;
        delete[] delayed_type_name;
        delayed_struct_ଚାବି = nullptr;
        delayed_struct_flags = 0;
        delayed_type_name = nullptr;

        is_write_struct_delayed = ମିଥ୍ୟା;

        /* call */
        ଯଦି (change_type_to_base64) {
            startWriteStruct_helper(struct_ଚାବି.c_str(), struct_flags, "binary");
            ଯଦି (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::InUse);
        } ଅନ୍ୟଥା {
            startWriteStruct_helper(struct_ଚାବି.c_str(), struct_flags, type_name.c_str());
            ଯଦି (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::NotUse);
        }
    }
}

ଶୂନ୍ୟ FileStorage::Impl::writeRawDataBase64(ଅଚଳ ଶୂନ୍ୟ *_data, size_t len, ଅଚଳ ଅକ୍ଷର *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(ସତ୍ୟ);

    ଯଦି (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } ଅନ୍ୟଥା ଯଦି (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->ଲିଖ(_data, len, dt);
}

File_ଗଣ୍ଠି FileStorage::Impl::get_First_ଶୀର୍ଷ_Level_ଗଣ୍ଠି() ଅଚଳ {
    ଫେରନ୍ତୁ roots.empty() ? File_ଗଣ୍ଠି() : roots[0];
}

File_ଗଣ୍ଠି FileStorage::Impl::ମୂଳ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା streamIdx) ଅଚଳ {
    ଫେରନ୍ତୁ streamIdx >= 0 && streamIdx < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roots.size() ? roots[streamIdx] : File_ଗଣ୍ଠି();
}

File_ଗଣ୍ଠି FileStorage::Impl::operator[](ଅଚଳ String &nodename) ଅଚଳ {
    ଫେରନ୍ତୁ ଏହି->operator[](nodename.c_str());
}

File_ଗଣ୍ଠି FileStorage::Impl::operator[](ଅଚଳ ଅକ୍ଷର * /*nodename*/) ଅଚଳ {
    ଫେରନ୍ତୁ File_ଗଣ୍ଠି();
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା FileStorage::Impl::getFormat() ଅଚଳ { ଫେରନ୍ତୁ fmt; }

ଅକ୍ଷର *FileStorage::Impl::bufferPtr() ଅଚଳ { ଫେରନ୍ତୁ (ଅକ୍ଷର *) (&buffer[0] + bufofs); }

ଅକ୍ଷର *FileStorage::Impl::bufferStart() ଅଚଳ { ଫେରନ୍ତୁ (ଅକ୍ଷର *) &buffer[0]; }

ଅକ୍ଷର *FileStorage::Impl::bufferEnd() ଅଚଳ { ଫେରନ୍ତୁ (ଅକ୍ଷର *) (&buffer[0] + buffer.size()); }

ଶୂନ୍ୟ FileStorage::Impl::setBufferPtr(ଅକ୍ଷର *ptr) {
    ଅକ୍ଷର *bufferstart = bufferStart();
    CV_Assert(ptr >= bufferstart && ptr <= bufferEnd());
    bufofs = ptr - bufferstart;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା FileStorage::Impl::wrapMargin() ଅଚଳ { ଫେରନ୍ତୁ wrap_margin; }

FStructData &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_ଗଦା.empty());
    ଫେରନ୍ତୁ write_ଗଦା.back();
}

ଶୂନ୍ୟ FileStorage::Impl::setNonEmpty() {
    empty_stream = ମିଥ୍ୟା;
}

ଶୂନ୍ୟ FileStorage::Impl::processSpecialDouble(ଅକ୍ଷର *buf, ଦ୍ୱିଗୁଣ *value, ଅକ୍ଷର **endptr) {
    FileStorage_API *fs = ଏହି;
    ଅକ୍ଷର c = buf[0];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା inf_hi = 0x7ff00000;

    ଯଦି (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    ଯଦି (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    ଯଦି (toupper(buf[1]) == 'I' && toupper(buf[2]) == 'N' && toupper(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    ଅନ୍ୟଥା ଯଦି (toupper(buf[1]) == 'N' && toupper(buf[2]) == 'A' && toupper(buf[3]) == 'N')
        v.u = (uint64) -1;
    ଅନ୍ୟଥା
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

ଦ୍ୱିଗୁଣ FileStorage::Impl::strtod(ଅକ୍ଷର *ptr, ଅକ୍ଷର **endptr) {
    ଦ୍ୱିଗୁଣ fval = ::strtod(ptr, endptr);
    ଯଦି (**endptr == '.') {
        ଅକ୍ଷର *dot_pos = *endptr;
        *dot_pos = ',';
        ଦ୍ୱିଗୁଣ fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        ଯଦି (*endptr > dot_pos)
            fval = fval2;
        ଅନ୍ୟଥା
            *endptr = dot_pos;
    }

    ଯଦି (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    ଫେରନ୍ତୁ fval;
}

ଶୂନ୍ୟ FileStorage::Impl::convertToCollection(ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, File_ଗଣ୍ଠି &ଗଣ୍ଠି) {
    CV_Assert(type == File_ଗଣ୍ଠି::SEQ || type == File_ଗଣ୍ଠି::MAP);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି_type = ଗଣ୍ଠି.type();
    ଯଦି (ଗଣ୍ଠି_type == type)
        ଫେରନ୍ତୁ;

    ତାର୍କିକ named = ଗଣ୍ଠି.isNamed();
    uchar *ptr = ଗଣ୍ଠି.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    ଦ୍ୱିଗୁଣ fval = 0;
    std::string sval;
    ତାର୍କିକ add_first_scalar = ମିଥ୍ୟା;

    ଯଦି (ଗଣ୍ଠି_type != File_ଗଣ୍ଠି::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_ଗଣ୍ଠି::SEQ);
        ଯଦି (ଗଣ୍ଠି_type == File_ଗଣ୍ଠି::INT) {
            ival = readLong(ptr);
            add_first_scalar = ସତ୍ୟ;
        } ଅନ୍ୟଥା ଯଦି (ଗଣ୍ଠି_type == File_ଗଣ୍ଠି::REAL) {
            fval = readReal(ptr);
            add_first_scalar = ସତ୍ୟ;
        } ଅନ୍ୟଥା ଯଦି (ଗଣ୍ଠି_type == File_ଗଣ୍ଠି::STRING) {
            sval = std::string(ଗଣ୍ଠି);
            add_first_scalar = ସତ୍ୟ;
        } ଅନ୍ୟଥା
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", ଗଣ୍ଠି_type));
    }

    ptr = reserve_ଗଣ୍ଠି_Space(ଗଣ୍ଠି, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_ଗଣ୍ଠି::NAMED : 0));
    // name has been copied automatically
    ଯଦି (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    ଯଦି (add_first_scalar)
        add_ଗଣ୍ଠି(ଗଣ୍ଠି, std::string(), ଗଣ୍ଠି_type,
                ଗଣ୍ଠି_type == File_ଗଣ୍ଠି::INT ? (ଅଚଳ ଶୂନ୍ୟ *) &ival :
                ଗଣ୍ଠି_type == File_ଗଣ୍ଠି::REAL ? (ଅଚଳ ଶୂନ୍ୟ *) &fval :
                ଗଣ୍ଠି_type == File_ଗଣ୍ଠି::STRING ? (ଅଚଳ ଶୂନ୍ୟ *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_ଗଣ୍ଠି_Space(File_ଗଣ୍ଠି &ଗଣ୍ଠି, size_t sz) {
    ତାର୍କିକ shrinkBlock = ମିଥ୍ୟା;
    size_t shrinkBlockIdx = 0, shrinkSize = 0;

    uchar *ptr = 0, *blockEnd = 0;

    ଯଦି (!fs_data_ptrs.empty()) {
        size_t blockIdx = ଗଣ୍ଠି.blockIdx;
        size_t ofs = ଗଣ୍ଠି.ofs;
        CV_Assert(blockIdx == fs_data_ptrs.size() - 1);
        CV_Assert(ofs <= fs_data_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_data_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_data_ptrs[blockIdx] + ofs;
        blockEnd = fs_data_ptrs[blockIdx] + fs_data_blksz[blockIdx];

        CV_Assert(ptr >= fs_data_ptrs[blockIdx] && ptr <= blockEnd);
        ଯଦି (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            ଫେରନ୍ତୁ ptr;
        }

        ଯଦି (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_data[blockIdx]->resize(sz);
            ptr = &fs_data[blockIdx]->at(0);
            fs_data_ptrs[blockIdx] = ptr;
            fs_data_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            ଫେରନ୍ତୁ ptr;
        }

        shrinkBlock = ସତ୍ୟ;
        shrinkBlockIdx = blockIdx;
        shrinkSize = ofs;
    }

    size_t blockSize = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(blockSize);
    fs_data.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_data_ptrs.push_back(new_ptr);
    fs_data_blksz.push_back(blockSize);
    ଗଣ୍ଠି.blockIdx = fs_data_ptrs.size() - 1;
    ଗଣ୍ଠି.ofs = 0;
    freeSpaceOfs = sz;

    ଯଦି (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        ଯଦି (ptr[0] & File_ଗଣ୍ଠି::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    ଯଦି (shrinkBlock) {
        fs_data[shrinkBlockIdx]->resize(shrinkSize);
        fs_data_blksz[shrinkBlockIdx] = shrinkSize;
    }

    ଫେରନ୍ତୁ new_ptr;
}

unsigned FileStorage::Impl::getStringOfs(ଅଚଳ std::string &ଚାବି) ଅଚଳ {
    str_hash_t::const_iterator it = str_hash.find(ଚାବି);
    ଫେରନ୍ତୁ it != str_hash.end() ? it->second : 0;
}

File_ଗଣ୍ଠି FileStorage::Impl::add_ଗଣ୍ଠି(File_ଗଣ୍ଠି &collection, ଅଚଳ std::string &ଚାବି,
                                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_type, ଅଚଳ ଶୂନ୍ୟ *value, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len) {
    FileStorage_API *fs = ଏହି;
    ତାର୍କିକ noname = ଚାବି.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(ଚାବି.c_str(), "_") == 0);
    convertToCollection(noname ? File_ଗଣ୍ଠି::SEQ : File_ଗଣ୍ଠି::MAP, collection);

    ତାର୍କିକ isseq = collection.empty() ? ମିଥ୍ୟା : collection.isSeq();
    ଯଦି (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    unsigned strofs = 0;
    ଯଦି (!noname) {
        strofs = getStringOfs(ଚାବି);
        ଯଦି (!strofs) {
            strofs = (unsigned) str_hash_data.size();
            size_t keysize = ଚାବି.size() + 1;
            str_hash_data.resize(strofs + keysize);
            memcpy(&str_hash_data[0] + strofs, &ଚାବି[0], keysize);
            str_hash.insert(std::make_pair(ଚାବି, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t blockIdx = fs_data_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_ଗଣ୍ଠି ଗଣ୍ଠି(fs_ext, blockIdx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_ଗଣ୍ଠି_Space(ଗଣ୍ଠି, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_ଗଣ୍ଠି::NAMED));
    ଯଦି (elem_type == File_ଗଣ୍ଠି::NONE)
        freeSpaceOfs -= 8;

    ଯଦି (!noname) {
        writeInt(ptr, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) strofs);
        ptr += 4;
    }

    ଯଦି (elem_type == File_ଗଣ୍ଠି::SEQ || elem_type == File_ଗଣ୍ଠି::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    ଯଦି (value)
        ଗଣ୍ଠି.set_ମୂଲ୍ୟ(elem_type, value, len);

    ଯଦି (collection.isNamed())
        cp += 4;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    ଫେରନ୍ତୁ ଗଣ୍ଠି;
}

ଶୂନ୍ୟ FileStorage::Impl::finalizeCollection(File_ଗଣ୍ଠି &collection) {
    ଯଦି (!collection.isSeq() && !collection.isMap())
        ଫେରନ୍ତୁ;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    ଯଦି (*ptr0 & File_ଗଣ୍ଠି::NAMED)
        ptr += 4;
    size_t blockIdx = collection.blockIdx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t rawSize = 4;
    unsigned sz = (unsigned) readInt(ptr + 4);
    ଯଦି (sz > 0) {
        size_t lastBlockIdx = fs_data_ptrs.size() - 1;

        ପାଇଁ (; blockIdx < lastBlockIdx; blockIdx++) {
            rawSize += fs_data_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    rawSize += freeSpaceOfs - ofs;
    writeInt(ptr, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) rawSize);
}

ଶୂନ୍ୟ FileStorage::Impl::normalize_ଗଣ୍ଠି_Ofs(size_t &blockIdx, size_t &ofs) ଅଚଳ {
    ଯେତେବେଳେ (ofs >= fs_data_blksz[blockIdx]) {
        ଯଦି (blockIdx == fs_data_blksz.size() - 1) {
            CV_Assert(ofs == fs_data_blksz[blockIdx]);
            ଭାଙ୍ଗ;
        }
        ofs -= fs_data_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    ଫେରନ୍ତୁ state_of_writing_base64;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା FileStorage::Impl::get_space() {
    ଫେରନ୍ତୁ space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = ସତ୍ୟ;
}

ଶୂନ୍ୟ FileStorage::Impl::Base64Decoder::init(ଅଚଳ Ptr<FileStorageParser> &_parser, ଅକ୍ଷର *_ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _indent) {
    parser_do_not_use_direct_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = ମିଥ୍ୟା;
}

ତାର୍କିକ FileStorage::Impl::Base64Decoder::readMore(ପୂର୍ଣ୍ଣସଂଖ୍ୟା needed) {
    ସ୍ଥିର ଅଚଳ uchar base64tab[] =
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

    ଯଦି (eos)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    ପାଇଁ (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    ଅକ୍ଷର *beg = 0, *end = 0;
    ତାର୍କିକ ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    ଯଦି (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = ସତ୍ୟ;
        size_t tc = totalchars;
        ପାଇଁ (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0, j, n = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) encoded.size();
    ଯଦି (n > 0) {
        ଅଚଳ uchar *tab = base64tab;
        ଅକ୍ଷର *src = &encoded[0];

        ପାଇଁ (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (uchar) src[i]], c = tab[(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (uchar) src[i + 1]];
            uchar b = tab[(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (uchar) src[i + 2]], a = tab[(ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    ଯଦି (i > 0 && encoded[i - 1] == '=') {
        ଯଦି (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        ଯଦି (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    ପାଇଁ (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    ଯଦି (ofs >= sz && !readMore(1))
        ଫେରନ୍ତୁ (uchar) 0;
    ଫେରନ୍ତୁ decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    ଯଦି (ofs + 2 > sz && !readMore(2))
        ଫେରନ୍ତୁ (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    ଫେରନ୍ତୁ val;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    ଯଦି (ofs + 4 > sz && !readMore(4))
        ଫେରନ୍ତୁ 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ival = readInt(&decoded[ofs]);
    ofs += 4;
    ଫେରନ୍ତୁ ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    ଯଦି (ofs + 8 > sz && !readMore(8))
        ଫେରନ୍ତୁ 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    ଫେରନ୍ତୁ ival;
}

ଦ୍ୱିଗୁଣ FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    ଯଦି (ofs + 8 > sz && !readMore(8))
        ଫେରନ୍ତୁ 0;
    ଦ୍ୱିଗୁଣ fval = readReal(&decoded[ofs]);
    ofs += 8;
    ଫେରନ୍ତୁ fval;
}

ତାର୍କିକ FileStorage::Impl::Base64Decoder::endOfStream() ଅଚଳ { ଫେରନ୍ତୁ eos; }

ଅକ୍ଷର *FileStorage::Impl::Base64Decoder::getPtr() ଅଚଳ { ଫେରନ୍ତୁ ptr; }


ଅକ୍ଷର *FileStorage::Impl::parseBase64(ଅକ୍ଷର *ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା indent, File_ଗଣ୍ଠି &collection) {
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା BASE64_HDR_SIZE = 24;
    ଅକ୍ଷର dt[BASE64_HDR_SIZE + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, k;

    ପାଇଁ (i = 0; i < BASE64_HDR_SIZE; i++)
        dt[i] = (ଅକ୍ଷର) base64decoder.getUInt8();
    ପାଇଁ (i = 0; i < BASE64_HDR_SIZE; i++)
        ଯଦି (isspace(dt[i]))
            ଭାଙ୍ଗ;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା fmt_pair_ଗଣନା = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    ଦ୍ୱିଗୁଣ fval = 0;

    ପାଇଁ (;;) {
        ପାଇଁ (k = 0; k < fmt_pair_ଗଣନା; k++) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_type = fmt_pairs[k * 2 + 1];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3 = fmt_pairs[k * 2];

            ପାଇଁ (i = 0; i < ଗଣନା_3; i++) {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି_type = File_ଗଣ୍ଠି::INT;
                ବାଛ (elem_type) {
                    ସ୍ଥିତି CV_8U:
                        ival = base64decoder.getUInt8();
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_8S:
                        ival = (ଅକ୍ଷର) base64decoder.getUInt8();
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_16U:
                        ival = base64decoder.getUInt16();
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_16S:
                        ival = (short) base64decoder.getUInt16();
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_32U:
                        ival = base64decoder.getInt32();
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_32S:
                        ival = base64decoder.getInt32();
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_64U:
                        ival = base64decoder.getInt64();
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_64S:
                        ival = base64decoder.getInt64();
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        ଗଣ୍ଠି_type = File_ଗଣ୍ଠି::REAL;
                    }
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_64F:
                        fval = base64decoder.getFloat64();
                        ଗଣ୍ଠି_type = File_ଗଣ୍ଠି::REAL;
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_16F:
                        fval = ଦଶମିକ(hfloatFromBits(base64decoder.getUInt16()));
                        ଗଣ୍ଠି_type = File_ଗଣ୍ଠି::REAL;
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        ଗଣ୍ଠି_type = File_ଗଣ୍ଠି::REAL;
                        ଭାଙ୍ଗ;
                    ସ୍ଥିତି CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        ଗଣ୍ଠି_type = File_ଗଣ୍ଠି::REAL;
                        ଭାଙ୍ଗ;
                    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                ଯଦି (base64decoder.endOfStream())
                    ଭାଙ୍ଗ;
                add_ଗଣ୍ଠି(collection, std::string(), ଗଣ୍ଠି_type,
                        ଗଣ୍ଠି_type == File_ଗଣ୍ଠି::INT ? (ଶୂନ୍ୟ *) &ival : (ଶୂନ୍ୟ *) &fval, -1);
            }
        }
        ଯଦି (base64decoder.endOfStream())
            ଭାଙ୍ଗ;
    }

    finalizeCollection(collection);
    ଫେରନ୍ତୁ base64decoder.getPtr();
}

ଶୂନ୍ୟ FileStorage::Impl::parseError(ଅଚଳ ଅକ୍ଷର *func_name, ଅଚଳ std::string &err_ସନ୍ଦେଶ, ଅଚଳ ଅକ୍ଷର *source_file,
                                   ପୂର୍ଣ୍ଣସଂଖ୍ୟା source_ଧାଡ଼ି) {
    std::string ସନ୍ଦେଶ = format("%s(%d): %s", filename.c_str(), lineno, err_ସନ୍ଦେଶ.c_str());
    error(Error::StsParseError, func_name, ସନ୍ଦେଶ.c_str(), source_file, source_ଧାଡ଼ି);
}

ଅଚଳ uchar *FileStorage::Impl::get_ଗଣ୍ଠି_Ptr(size_t blockIdx, size_t ofs) ଅଚଳ {
    CV_Assert(blockIdx < fs_data_ptrs.size());
    CV_Assert(ofs < fs_data_blksz[blockIdx]);

    ଫେରନ୍ତୁ fs_data_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) ଅଚଳ {
    CV_Assert(nameofs < str_hash_data.size());
    ଫେରନ୍ତୁ std::string(&str_hash_data[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { ଫେରନ୍ତୁ fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(ଏହି);
}

FileStorage::FileStorage(ଅଚଳ String& filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ଅଚଳ String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(ଏହି);
    ତାର୍କିକ ok = p->open(filename.c_str(), flags, encoding.c_str());
    ଯଦି(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

ଶୂନ୍ୟ FileStorage::startWriteStruct(ଅଚଳ String& name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags, ଅଚଳ String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    ଯଦି ((struct_flags & File_ଗଣ୍ଠି::TYPE_MASK) == File_ଗଣ୍ଠି::SEQ)
        state = FileStorage::ମୂଲ୍ୟ_EXPECTED;
    ଅନ୍ୟଥା
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

ଶୂନ୍ୟ FileStorage::endWriteStruct()
{
    p->endWriteStruct();
    state = p->write_ଗଦା.empty() || File_ଗଣ୍ଠି::isMap(p->write_ଗଦା.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::ମୂଲ୍ୟ_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

ତାର୍କିକ FileStorage::open(ଅଚଳ String& filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ଅଚଳ String& encoding)
{
    ଚେଷ୍ଟା
    {
        ତାର୍କିକ ok = p->open(filename.c_str(), flags, encoding.c_str());
        ଯଦି(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        ଫେରନ୍ତୁ ok;
    }
    ଧର (...)
    {
        release();
        ଫିଙ୍ଗ;  // re-throw
    }
}

ତାର୍କିକ FileStorage::isOpened() ଅଚଳ { ଫେରନ୍ତୁ p->is_opened; }

ଶୂନ୍ୟ FileStorage::release()
{
    p->release();
}

File_ଗଣ୍ଠି FileStorage::ମୂଳ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ଯଦି( p.empty() || p->roots.empty() || i < 0 || i >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)p->roots.size() )
        ଫେରନ୍ତୁ File_ଗଣ୍ଠି();

    ଫେରନ୍ତୁ p->roots[i];
}

File_ଗଣ୍ଠି FileStorage::get_First_ଶୀର୍ଷ_Level_ଗଣ୍ଠି() ଅଚଳ
{
    File_ଗଣ୍ଠି r = ମୂଳ();
    File_ଗଣ୍ଠି_Iterator it = r.begin();
    ଫେରନ୍ତୁ it != r.end() ? *it : File_ଗଣ୍ଠି();
}

std::string FileStorage::getDefaultObjectName(ଅଚଳ std::string& _filename)
{
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* stubname = "unnamed";
    ଅଚଳ ଅକ୍ଷର* filename = _filename.c_str();
    ଅଚଳ ଅକ୍ଷର* ptr2 = filename + _filename.size();
    ଅଚଳ ଅକ୍ଷର* ptr = ptr2 - 1;
    cv::AutoBuffer<ଅକ୍ଷର> name_buf(_filename.size()+1);

    ଯେତେବେଳେ( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        ଯଦି( *ptr == '.' && (!*ptr2 || strncmp(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    ଯଦି( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    ଅକ୍ଷର* name = name_buf.data();

    // name must start with letter or '_'
    ଯଦି( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    ଯେତେବେଳେ( ptr < ptr2 )
    {
        ଅକ୍ଷର c = *ptr++;
        ଯଦି( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.data();
    ଯଦି( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    ଫେରନ୍ତୁ name;
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା FileStorage::getFormat() ଅଚଳ
{
    ଫେରନ୍ତୁ p->fmt;
}

File_ଗଣ୍ଠି FileStorage::operator [](ଅଚଳ ଅକ୍ଷର* ଚାବି) ଅଚଳ
{
    ଫେରନ୍ତୁ ଏହି->operator[](std::string(ଚାବି));
}

File_ଗଣ୍ଠି FileStorage::operator [](ଅଚଳ std::string& ଚାବି) ଅଚଳ
{
    File_ଗଣ୍ଠି res;
    ପାଇଁ (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][ଚାବି];
        ଯଦି (!res.empty())
            ଭାଙ୍ଗ;
    }
    ଫେରନ୍ତୁ res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    ଫେରନ୍ତୁ buf;
}

ଶୂନ୍ୟ FileStorage::writeRaw( ଅଚଳ String& fmt, ଅଚଳ ଶୂନ୍ୟ* vec, size_t len )
{
    p->writeRawData(fmt, (ଅଚଳ uchar*)vec, len);
}

ଶୂନ୍ୟ FileStorage::writeComment( ଅଚଳ String& comment, ତାର୍କିକ eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

ଶୂନ୍ୟ writeScalar( FileStorage& fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value )
{
    fs.p->ଲିଖ(String(), value);
}

ଶୂନ୍ୟ writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->ଲିଖ(String(), value);
}

ଶୂନ୍ୟ writeScalar( FileStorage& fs, ଦଶମିକ value )
{
    fs.p->ଲିଖ(String(), (ଦ୍ୱିଗୁଣ)value);
}

ଶୂନ୍ୟ writeScalar( FileStorage& fs, ଦ୍ୱିଗୁଣ value )
{
    fs.p->ଲିଖ(String(), value);
}

ଶୂନ୍ୟ writeScalar( FileStorage& fs, ଅଚଳ String& value )
{
    fs.p->ଲିଖ(String(), value);
}

ଶୂନ୍ୟ ଲିଖ( FileStorage& fs, ଅଚଳ String& name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value )
{
    fs.p->ଲିଖ(name, value);
}

ଶୂନ୍ୟ ଲିଖ( FileStorage& fs, ଅଚଳ String& name, int64_t value )
{
    fs.p->ଲିଖ(name, value);
}

ଶୂନ୍ୟ ଲିଖ( FileStorage& fs, ଅଚଳ String& name, ଦଶମିକ value )
{
    fs.p->ଲିଖ(name, (ଦ୍ୱିଗୁଣ)value);
}

ଶୂନ୍ୟ ଲିଖ( FileStorage& fs, ଅଚଳ String& name, ଦ୍ୱିଗୁଣ value )
{
    fs.p->ଲିଖ(name, value);
}

ଶୂନ୍ୟ ଲିଖ( FileStorage& fs, ଅଚଳ String& name, ଅଚଳ String& value )
{
    fs.p->ଲିଖ(name, value);
}

ଶୂନ୍ୟ ଲିଖ( FileStorage& fs, ଅଚଳ String& name, ତାର୍କିକ value )
{
    fs.p->ଲିଖ(name, value);
}

ଶୂନ୍ୟ FileStorage::ଲିଖ(ଅଚଳ String& name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା val) { p->ଲିଖ(name, val); }
ଶୂନ୍ୟ FileStorage::ଲିଖ(ଅଚଳ String& name, ତାର୍କିକ val) { p->ଲିଖ(name, val); }
ଶୂନ୍ୟ FileStorage::ଲିଖ(ଅଚଳ String& name, int64_t val) { p->ଲିଖ(name, val); }
ଶୂନ୍ୟ FileStorage::ଲିଖ(ଅଚଳ String& name, ଦ୍ୱିଗୁଣ val) { p->ଲିଖ(name, val); }
ଶୂନ୍ୟ FileStorage::ଲିଖ(ଅଚଳ String& name, ଅଚଳ String& val) { p->ଲିଖ(name, val); }
ଶୂନ୍ୟ FileStorage::ଲିଖ(ଅଚଳ String& name, ଅଚଳ Mat& val) { cv::ଲିଖ(*ଏହି, name, val); }
ଶୂନ୍ୟ FileStorage::ଲିଖ(ଅଚଳ String& name, ଅଚଳ std::vector<String>& val) { cv::ଲିଖ(*ଏହି, name, val); }

FileStorage& operator << (FileStorage& fs, ଅଚଳ String& str)
{
    ଗଣନା { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        ମୂଲ୍ୟ_EXPECTED = FileStorage::ମୂଲ୍ୟ_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    ଅଚଳ ଅକ୍ଷର* _str = str.c_str();
    ଯଦି( !fs.isOpened() || !_str )
        ଫେରନ୍ତୁ fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    ଅକ୍ଷର c = *_str;

    ଯଦି( c == '}' || c == ']' )
    {
        ଯଦି( fs_impl->write_ଗଦା.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags = fs_impl->write_ଗଦା.back().flags;
        ଅକ୍ଷର expected_bracket = File_ଗଣ୍ଠି::isMap(struct_flags) ? '}' : ']';
        ଯଦି( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_ଗଦା.empty());
        struct_flags = fs_impl->write_ଗଦା.back().flags;
        fs.state = File_ଗଣ୍ଠି::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : ମୂଲ୍ୟ_EXPECTED;
        fs.elname = String();
    }
    ଅନ୍ୟଥା ଯଦି( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        ଯଦି (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = ମୂଲ୍ୟ_EXPECTED + INSIDE_MAP;
    }
    ଅନ୍ୟଥା ଯଦି( (fs.state & 3) == ମୂଲ୍ୟ_EXPECTED )
    {
        ଯଦି( c == '{' || c == '[' )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags = c == '{' ? File_ଗଣ୍ଠି::MAP : File_ଗଣ୍ଠି::SEQ;
            fs.state = struct_flags == File_ଗଣ୍ଠି::MAP ? INSIDE_MAP + NAME_EXPECTED : ମୂଲ୍ୟ_EXPECTED;
            _str++;
            ଯଦି( *_str == ':' )
            {
                _str++;
                ଯଦି( !*_str )
                    struct_flags |= File_ଗଣ୍ଠି::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        ଅନ୍ୟଥା
        {
            ଲିଖ( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            ଯଦି( fs.state == INSIDE_MAP + ମୂଲ୍ୟ_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    ଅନ୍ୟଥା
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    ଫେରନ୍ତୁ fs;
}


File_ଗଣ୍ଠି::File_ଗଣ୍ଠି()
    : fs(ଖାଲି)
{
    blockIdx = ofs = 0;
}

File_ଗଣ୍ଠି::File_ଗଣ୍ଠି(FileStorage::Impl* _fs, size_t _blockIdx, size_t _ofs)
    : fs(_fs)
{
    blockIdx = _blockIdx;
    ofs = _ofs;
}

File_ଗଣ୍ଠି::File_ଗଣ୍ଠି(ଅଚଳ FileStorage* _fs, size_t _blockIdx, size_t _ofs)
    : File_ଗଣ୍ଠି(_fs->p.get(), _blockIdx, _ofs)
{
    // nothing
}

File_ଗଣ୍ଠି::File_ଗଣ୍ଠି(ଅଚଳ File_ଗଣ୍ଠି& ଗଣ୍ଠି)
{
    fs = ଗଣ୍ଠି.fs;
    blockIdx = ଗଣ୍ଠି.blockIdx;
    ofs = ଗଣ୍ଠି.ofs;
}

File_ଗଣ୍ଠି& File_ଗଣ୍ଠି::operator=(ଅଚଳ File_ଗଣ୍ଠି& ଗଣ୍ଠି)
{
    fs = ଗଣ୍ଠି.fs;
    blockIdx = ଗଣ୍ଠି.blockIdx;
    ofs = ଗଣ୍ଠି.ofs;
    ଫେରନ୍ତୁ *ଏହି;
}

File_ଗଣ୍ଠି File_ଗଣ୍ଠି::operator[](ଅଚଳ std::string& nodename) ଅଚଳ
{
    ଯଦି(!fs)
        ଫେରନ୍ତୁ File_ଗଣ୍ଠି();

    CV_Assert( isMap() );

    unsigned ଚାବି = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_ଗଣ୍ଠି_Iterator it = begin();

    ପାଇଁ( i = 0; i < sz; i++, ++it )
    {
        File_ଗଣ୍ଠି n = *it;
        ଅଚଳ uchar* p = n.ptr();
        unsigned key2 = (unsigned)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_data.size() );
        ଯଦି( ଚାବି == key2 )
            ଫେରନ୍ତୁ n;
    }
    ଫେରନ୍ତୁ File_ଗଣ୍ଠି();
}

File_ଗଣ୍ଠି File_ଗଣ୍ଠି::operator[](ଅଚଳ ଅକ୍ଷର* nodename) ଅଚଳ
{
    ଫେରନ୍ତୁ ଏହି->operator[](std::string(nodename));
}

File_ଗଣ୍ଠି File_ଗଣ୍ଠି::operator[](ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ଯଦି(!fs)
        ଫେରନ୍ତୁ File_ଗଣ୍ଠି();

    CV_Assert( isSeq() );

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)size();
    CV_Assert( 0 <= i && i < sz );

    File_ଗଣ୍ଠି_Iterator it = begin();
    it += i;

    ଫେରନ୍ତୁ *it;
}

std::vector<String> File_ଗଣ୍ଠି::ଚାବିଗୁଡ଼ିକ() ଅଚଳ
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    ପାଇଁ (File_ଗଣ୍ଠି_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    ଫେରନ୍ତୁ res;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା File_ଗଣ୍ଠି::type() ଅଚଳ
{
    ଅଚଳ uchar* p = ptr();
    ଯଦି(!p)
        ଫେରନ୍ତୁ NONE;
    ଫେରନ୍ତୁ (*p & TYPE_MASK);
}

ତାର୍କିକ File_ଗଣ୍ଠି::isMap(ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags) { ଫେରନ୍ତୁ (flags & TYPE_MASK) == MAP; }
ତାର୍କିକ File_ଗଣ୍ଠି::isSeq(ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags) { ଫେରନ୍ତୁ (flags & TYPE_MASK) == SEQ; }
ତାର୍କିକ File_ଗଣ୍ଠି::isCollection(ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags) { ଫେରନ୍ତୁ isMap(flags) || isSeq(flags); }
ତାର୍କିକ File_ଗଣ୍ଠି::isFlow(ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags) { ଫେରନ୍ତୁ (flags & FLOW) != 0; }
ତାର୍କିକ File_ଗଣ୍ଠି::isEmptyCollection(ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags) { ଫେରନ୍ତୁ (flags & EMPTY) != 0; }

ତାର୍କିକ File_ଗଣ୍ଠି::empty() ଅଚଳ   { ଫେରନ୍ତୁ fs == 0; }
ତାର୍କିକ File_ଗଣ୍ଠି::isNone() ଅଚଳ  { ଫେରନ୍ତୁ type() == NONE; }
ତାର୍କିକ File_ଗଣ୍ଠି::isSeq() ଅଚଳ   { ଫେରନ୍ତୁ type() == SEQ; }
ତାର୍କିକ File_ଗଣ୍ଠି::isMap() ଅଚଳ   { ଫେରନ୍ତୁ type() == MAP; }
ତାର୍କିକ File_ଗଣ୍ଠି::isInt() ଅଚଳ   { ଫେରନ୍ତୁ type() == INT;  }
ତାର୍କିକ File_ଗଣ୍ଠି::isReal() ଅଚଳ  { ଫେରନ୍ତୁ type() == REAL; }
ତାର୍କିକ File_ଗଣ୍ଠି::isString() ଅଚଳ { ଫେରନ୍ତୁ type() == STRING;  }
ତାର୍କିକ File_ଗଣ୍ଠି::isNamed() ଅଚଳ
{
    ଅଚଳ uchar* p = ptr();
    ଯଦି(!p)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ (*p & NAMED) != 0;
}

std::string File_ଗଣ୍ଠି::name() ଅଚଳ
{
    ଅଚଳ uchar* p = ptr();
    ଯଦି(!p)
        ଫେରନ୍ତୁ std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    ଫେରନ୍ତୁ fs->getName(nameofs);
}

File_ଗଣ୍ଠି::operator ପୂର୍ଣ୍ଣସଂଖ୍ୟା() ଅଚଳ
{
    ଅଚଳ uchar* p = ptr();
    ଯଦି(!p)
        ଫେରନ୍ତୁ 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag = *p;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    ଯଦି( type == INT )
    {
        ଫେରନ୍ତୁ readInt(p);
    }
    ଅନ୍ୟଥା ଯଦି( type == REAL )
    {
        ଫେରନ୍ତୁ cvRound(readReal(p));
    }
    ଅନ୍ୟଥା
        ଫେରନ୍ତୁ 0x7fffffff;
}

File_ଗଣ୍ଠି::operator int64_t() ଅଚଳ
{
    ଅଚଳ uchar* p = ptr();
    ଯଦି(!p)
        ଫେରନ୍ତୁ 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag = *p;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    ଯଦି( type == INT )
    {
        ଫେରନ୍ତୁ readLong(p);
    }
    ଅନ୍ୟଥା ଯଦି( type == REAL )
    {
        ଫେରନ୍ତୁ cvRound(readReal(p));
    }
    ଅନ୍ୟଥା
        ଫେରନ୍ତୁ 0x7fffffff;
}

File_ଗଣ୍ଠି::operator ଦଶମିକ() ଅଚଳ
{
    ଅଚଳ uchar* p = ptr();
    ଯଦି(!p)
        ଫେରନ୍ତୁ 0.f;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag = *p;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    ଯଦି( type == INT )
    {
        ଫେରନ୍ତୁ (ଦଶମିକ)readLong(p);
    }
    ଅନ୍ୟଥା ଯଦି( type == REAL )
    {
        ଫେରନ୍ତୁ (ଦଶମିକ)readReal(p);
    }
    ଅନ୍ୟଥା
        ଫେରନ୍ତୁ FLT_MAX;
}

File_ଗଣ୍ଠି::operator ଦ୍ୱିଗୁଣ() ଅଚଳ
{
    ଅଚଳ uchar* p = ptr();
    ଯଦି(!p)
        ଫେରନ୍ତୁ 0.f;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag = *p;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    ଯଦି( type == INT )
    {
        ଫେରନ୍ତୁ (ଦ୍ୱିଗୁଣ)readLong(p);
    }
    ଅନ୍ୟଥା ଯଦି( type == REAL )
    {
        ଫେରନ୍ତୁ readReal(p);
    }
    ଅନ୍ୟଥା
        ଫେରନ୍ତୁ DBL_MAX;
}

ଦ୍ୱିଗୁଣ File_ଗଣ୍ଠି::real() ଅଚଳ  { ଫେରନ୍ତୁ ଦ୍ୱିଗୁଣ(*ଏହି); }

std::string File_ଗଣ୍ଠି::string() ଅଚଳ
{
    ଅଚଳ uchar* p = ptr();
    ଯଦି( !p || (*p & TYPE_MASK) != STRING )
        ଫେରନ୍ତୁ std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(unsigned)readInt(p);
    ଫେରନ୍ତୁ std::string((ଅଚଳ ଅକ୍ଷର*)(p + 4), sz - 1);
}
Mat File_ଗଣ୍ଠି::mat() ଅଚଳ { Mat value; ପଠ(*ଏହି, value, Mat()); ଫେରନ୍ତୁ value; }

File_ଗଣ୍ଠି_Iterator File_ଗଣ୍ଠି::begin() ଅଚଳ { ଫେରନ୍ତୁ File_ଗଣ୍ଠି_Iterator(*ଏହି, ମିଥ୍ୟା); }
File_ଗଣ୍ଠି_Iterator File_ଗଣ୍ଠି::end() ଅଚଳ   { ଫେରନ୍ତୁ File_ଗଣ୍ଠି_Iterator(*ଏହି, ସତ୍ୟ); }

ଶୂନ୍ୟ File_ଗଣ୍ଠି::readRaw( ଅଚଳ std::string& fmt, ଶୂନ୍ୟ* vec, size_t len ) ଅଚଳ
{
    File_ଗଣ୍ଠି_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_ଗଣ୍ଠି::size() ଅଚଳ
{
    ଅଚଳ uchar* p = ptr();
    ଯଦି( !p )
        ଫେରନ୍ତୁ 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag = *p;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tp = tag & TYPE_MASK;
    ଯଦି( tp == MAP || tp == SEQ )
    {
        ଯଦି( tag & NAMED )
            p += 4;
        ଫେରନ୍ତୁ (size_t)(unsigned)readInt(p + 5);
    }
    ଫେରନ୍ତୁ tp != NONE;
}

size_t File_ଗଣ୍ଠି::rawSize() ଅଚଳ
{
    ଅଚଳ uchar* p0 = ptr(), *p = p0;
    ଯଦି( !p )
        ଫେରନ୍ତୁ 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag = *p++;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tp = tag & TYPE_MASK;
    ଯଦି( tag & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    ଯଦି( tp == INT )
        ଫେରନ୍ତୁ sz0 + 8;
    ଯଦି( tp == REAL )
        ଫେରନ୍ତୁ sz0 + 8;
    ଯଦି( tp == NONE )
        ଫେରନ୍ତୁ sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    ଫେରନ୍ତୁ sz0 + 4 + readInt(p);
}

uchar* File_ଗଣ୍ଠି::ptr()
{
    ଫେରନ୍ତୁ !fs ? 0 : (uchar*)fs->get_ଗଣ୍ଠି_Ptr(blockIdx, ofs);
}

ଅଚଳ uchar* File_ଗଣ୍ଠି::ptr() ଅଚଳ
{
    ଫେରନ୍ତୁ !fs ? 0 : fs->get_ଗଣ୍ଠି_Ptr(blockIdx, ofs);
}

ଶୂନ୍ୟ File_ଗଣ୍ଠି::set_ମୂଲ୍ୟ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ଅଚଳ ଶୂନ୍ୟ* value, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag = *p;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz = 1;

    ଯଦି( tag & NAMED )
        sz += 4;

    ଯଦି( type == INT )
        sz += 8;
    ଅନ୍ୟଥା ଯଦି( type == REAL )
        sz += 8;
    ଅନ୍ୟଥା ଯଦି( type == STRING )
    {
        ଯଦି( len < 0 )
            len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)strlen((ଅଚଳ ଅକ୍ଷର*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_ଗଣ୍ଠି_Space(*ଏହି, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    ଯଦି( tag & NAMED )
        p += 4;

    ଯଦି( type == INT )
    {
        int64_t ival = *(ଅଚଳ int64_t*)value;
        writeInt(p, ival);
    }
    ଅନ୍ୟଥା ଯଦି( type == REAL )
    {
        ଦ୍ୱିଗୁଣ dbval = *(ଅଚଳ ଦ୍ୱିଗୁଣ*)value;
        writeReal(p, dbval);
    }
    ଅନ୍ୟଥା ଯଦି( type == STRING )
    {
        ଅଚଳ ଅକ୍ଷର* str = (ଅଚଳ ଅକ୍ଷର*)value;
        writeInt(p, len + 1);
        memcpy(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_ଗଣ୍ଠି_Iterator::File_ଗଣ୍ଠି_Iterator()
{
    fs = 0;
    blockIdx = 0;
    ofs = 0;
    blockSize = 0;
    ଗଣ୍ଠି_NElems = 0;
    idx = 0;
}

File_ଗଣ୍ଠି_Iterator::File_ଗଣ୍ଠି_Iterator( ଅଚଳ File_ଗଣ୍ଠି& ଗଣ୍ଠି, ତାର୍କିକ seekEnd )
{
    fs = ଗଣ୍ଠି.fs;
    idx = 0;
    ଯଦି( !fs )
        blockIdx = ofs = blockSize = ଗଣ୍ଠି_NElems = 0;
    ଅନ୍ୟଥା
    {
        blockIdx = ଗଣ୍ଠି.blockIdx;
        ofs = ଗଣ୍ଠି.ofs;

        ତାର୍କିକ collection = ଗଣ୍ଠି.isSeq() || ଗଣ୍ଠି.isMap();
        ଯଦି( ଗଣ୍ଠି.isNone() )
        {
            ଗଣ୍ଠି_NElems = 0;
        }
        ଅନ୍ୟଥା ଯଦି( !collection )
        {
            ଗଣ୍ଠି_NElems = 1;
            ଯଦି( seekEnd )
            {
                idx = 1;
                ofs += ଗଣ୍ଠି.rawSize();
            }
        }
        ଅନ୍ୟଥା
        {
            ଗଣ୍ଠି_NElems = ଗଣ୍ଠି.size();
            ଅଚଳ uchar* p0 = ଗଣ୍ଠି.ptr(), *p = p0 + 1;
            ଯଦି(*p0 & File_ଗଣ୍ଠି::NAMED )
                p += 4;
            ଯଦି( !seekEnd )
                ofs += (p - p0) + 8;
            ଅନ୍ୟଥା
            {
                size_t rawsz = (size_t)(unsigned)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = ଗଣ୍ଠି_NElems;
            }
        }
        fs->normalize_ଗଣ୍ଠି_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
}

File_ଗଣ୍ଠି_Iterator::File_ଗଣ୍ଠି_Iterator(ଅଚଳ File_ଗଣ୍ଠି_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    ଗଣ୍ଠି_NElems = it.ଗଣ୍ଠି_NElems;
    idx = it.idx;
}

File_ଗଣ୍ଠି_Iterator& File_ଗଣ୍ଠି_Iterator::operator=(ଅଚଳ File_ଗଣ୍ଠି_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    ଗଣ୍ଠି_NElems = it.ଗଣ୍ଠି_NElems;
    idx = it.idx;
    ଫେରନ୍ତୁ *ଏହି;
}

File_ଗଣ୍ଠି File_ଗଣ୍ଠି_Iterator::operator *() ଅଚଳ
{
    ଫେରନ୍ତୁ File_ଗଣ୍ଠି(idx < ଗଣ୍ଠି_NElems ? fs : ଖାଲି, blockIdx, ofs);
}

File_ଗଣ୍ଠି_Iterator& File_ଗଣ୍ଠି_Iterator::operator ++ ()
{
    ଯଦି( idx == ଗଣ୍ଠି_NElems || !fs )
        ଫେରନ୍ତୁ *ଏହି;
    idx++;
    File_ଗଣ୍ଠି n(fs, blockIdx, ofs);
    ofs += n.rawSize();
    ଯଦି( ofs >= blockSize )
    {
        fs->normalize_ଗଣ୍ଠି_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
    ଫେରନ୍ତୁ *ଏହି;
}

File_ଗଣ୍ଠି_Iterator File_ଗଣ୍ଠି_Iterator::operator ++ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
{
    File_ଗଣ୍ଠି_Iterator it = *ଏହି;
    ++(*ଏହି);
    ଫେରନ୍ତୁ it;
}

File_ଗଣ୍ଠି_Iterator& File_ଗଣ୍ଠି_Iterator::operator += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା _ofs)
{
    CV_Assert( _ofs >= 0 );
    ପାଇଁ( ; _ofs > 0; _ofs-- )
        ଏହି->operator ++();
    ଫେରନ୍ତୁ *ଏହି;
}

File_ଗଣ୍ଠି_Iterator& File_ଗଣ୍ଠି_Iterator::readRaw( ଅଚଳ String& fmt, ଶୂନ୍ୟ* _data0, size_t maxsz)
{
    ଯଦି( fs && idx < ଗଣ୍ଠି_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା fmt_pair_ଗଣନା = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        size_t esz = fs::calcStructSize( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        ପାଇଁ( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < fmt_pair_ଗଣନା; k++ )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_type = fmt_pairs[k*2+1];
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_size = CV_ELEM_SIZE(elem_type);

                ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3 = fmt_pairs[k*2];
                offset = alignSize( offset, elem_size );
                uchar* data = data0 + offset;

                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_3; i++, ++(*ଏହି) )
                {
                    File_ଗଣ୍ଠି ଗଣ୍ଠି = *(*ଏହି);
                    ଯଦି( ଗଣ୍ଠି.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_size == 8 ? (int64_t)ଗଣ୍ଠି : (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଗଣ୍ଠି);
                        ବାଛ( elem_type )
                        {
                        ସ୍ଥିତି CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(ival);
                            data++;
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_8S:
                            *(ଅକ୍ଷର*)data = saturate_cast<schar>(ival);
                            data++;
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_Bool:
                            *(ତାର୍କିକ*)data = ival != 0;
                            data++;
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(ival);
                            data += ଆକାର(ushort);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_16S:
                            *(short*)data = saturate_cast<short>(ival);
                            data += ଆକାର(short);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_32U:
                            *(unsigned*)data = (unsigned)std::max(ival, (int64_t)0);
                            data += ଆକାର(unsigned);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_32S:
                            *(ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)data = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ival;
                            data += ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_32F:
                            *(ଦଶମିକ*)data = (ଦଶମିକ)ival;
                            data += ଆକାର(ଦଶମିକ);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_64U:
                            *(uint64_t*)data = (uint64_t)ival;
                            data += ଆକାର(uint64_t);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_64S:
                            *(int64_t*)data = (int64_t)ival;
                            data += ଆକାର(int64_t);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_64F:
                            *(ଦ୍ୱିଗୁଣ*)data = (ଦ୍ୱିଗୁଣ)ival;
                            data += ଆକାର(ଦ୍ୱିଗୁଣ);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_16F:
                            *(hfloat*)data = hfloat((ଦଶମିକ)ival);
                            data += ଆକାର(hfloat);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_16BF:
                            *(bfloat*)data = bfloat((ଦଶମିକ)ival);
                            data += ଆକାର(bfloat);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((ଦଶମିକ)ival);
                            data += ଆକାର(fp8_t);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((ଦଶମିକ)ival);
                            data += ଆକାର(fp8a_t);
                            ଭାଙ୍ଗ;
                        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    ଅନ୍ୟଥା ଯଦି( ଗଣ୍ଠି.isReal() )
                    {
                        ଦ୍ୱିଗୁଣ fval = (ଦ୍ୱିଗୁଣ)ଗଣ୍ଠି;

                        ବାଛ( elem_type )
                        {
                        ସ୍ଥିତି CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(fval);
                            data++;
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_8S:
                            *(ଅକ୍ଷର*)data = saturate_cast<schar>(fval);
                            data++;
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(fval);
                            data += ଆକାର(ushort);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_16S:
                            *(short*)data = saturate_cast<short>(fval);
                            data += ଆକାର(short);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_32U:
                            *(ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)data = saturate_cast<unsigned>(fval);
                            data += ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_32S:
                            *(ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)data = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(fval);
                            data += ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_32F:
                            *(ଦଶମିକ*)data = (ଦଶମିକ)fval;
                            data += ଆକାର(ଦଶମିକ);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_64U:
                            *(uint64_t*)data = (uint64_t)round(std::max(fval, 0.));
                            data += ଆକାର(uint64_t);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_64S:
                            *(int64_t*)data = (int64_t)round(std::max(fval, 0.));
                            data += ଆକାର(int64_t);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_64F:
                            *(ଦ୍ୱିଗୁଣ*)data = fval;
                            data += ଆକାର(ଦ୍ୱିଗୁଣ);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_16F:
                            *(hfloat*)data = hfloat((ଦଶମିକ)fval);
                            data += ଆକାର(hfloat);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_16BF:
                            *(bfloat*)data = bfloat((ଦଶମିକ)fval);
                            data += ଆକାର(bfloat);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((ଦଶମିକ)fval);
                            data += ଆକାର(fp8_t);
                            ଭାଙ୍ଗ;
                        ସ୍ଥିତି CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((ଦଶମିକ)fval);
                            data += ଆକାର(fp8a_t);
                            ଭାଙ୍ଗ;
                        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    ଅନ୍ୟଥା
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(data - data0);
            }
        }
    }

    ଫେରନ୍ତୁ *ଏହି;
}

ତାର୍କିକ File_ଗଣ୍ଠି_Iterator::equalTo(ଅଚଳ File_ଗଣ୍ଠି_Iterator& it) ଅଚଳ
{
    ଫେରନ୍ତୁ fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && ଗଣ୍ଠି_NElems == it.ଗଣ୍ଠି_NElems;
}

size_t File_ଗଣ୍ଠି_Iterator::remaining() ଅଚଳ
{
    ଫେରନ୍ତୁ ଗଣ୍ଠି_NElems - idx;
}

ତାର୍କିକ operator == ( ଅଚଳ File_ଗଣ୍ଠି_Iterator& it1, ଅଚଳ File_ଗଣ୍ଠି_Iterator& it2 )
{
    ଫେରନ୍ତୁ it1.equalTo(it2);
}

ତାର୍କିକ operator != ( ଅଚଳ File_ଗଣ୍ଠି_Iterator& it1, ଅଚଳ File_ଗଣ୍ଠି_Iterator& it2 )
{
    ଫେରନ୍ତୁ !it1.equalTo(it2);
}

ଶୂନ୍ୟ ପଠ(ଅଚଳ File_ଗଣ୍ଠି& ଗଣ୍ଠି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& val, ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_val)
{
    val = default_val;
    ଯଦି( !ଗଣ୍ଠି.empty() )
    {
        val = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଗଣ୍ଠି;
    }
}

ଶୂନ୍ୟ ପଠ(ଅଚଳ File_ଗଣ୍ଠି& ଗଣ୍ଠି, int64_t& val, int64_t default_val)
{
    val = default_val;
    ଯଦି( !ଗଣ୍ଠି.empty() )
    {
        val = (int64_t)ଗଣ୍ଠି;
    }
}

ଶୂନ୍ୟ ପଠ(ଅଚଳ File_ଗଣ୍ଠି& ଗଣ୍ଠି, ଦ୍ୱିଗୁଣ& val, ଦ୍ୱିଗୁଣ default_val)
{
    val = default_val;
    ଯଦି( !ଗଣ୍ଠି.empty() )
    {
        val = (ଦ୍ୱିଗୁଣ)ଗଣ୍ଠି;
    }
}

ଶୂନ୍ୟ ପଠ(ଅଚଳ File_ଗଣ୍ଠି& ଗଣ୍ଠି, ଦଶମିକ& val, ଦଶମିକ default_val)
{
    val = default_val;
    ଯଦି( !ଗଣ୍ଠି.empty() )
    {
        val = (ଦଶମିକ)ଗଣ୍ଠି;
    }
}

ଶୂନ୍ୟ ପଠ(ଅଚଳ File_ଗଣ୍ଠି& ଗଣ୍ଠି, std::string& val, ଅଚଳ std::string& default_val)
{
    val = default_val;
    ଯଦି( !ଗଣ୍ଠି.empty() )
    {
        val = (std::string)ଗଣ୍ଠି;
    }
}
ଶୂନ୍ୟ FileStorage::Impl::ଲିଖ(ଅଚଳ String &ଚାବି, ତାର୍କିକ value)
{
    CV_Assert(write_mode);
    ଯଦି (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().ଲିଖ(ଚାବି.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)value);
    }
    ଅନ୍ୟଥା
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().ଲିଖ(ଚାବି.c_str(), value ? "true" : "false", ମିଥ୍ୟା);
    }
}

FileStorage_API::~FileStorage_API() {}

ନାମାକାଶ internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, ଅଚଳ std::string& name,
                                       ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ଅଚଳ std::string& typeName)
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
