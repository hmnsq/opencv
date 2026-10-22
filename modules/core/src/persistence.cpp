// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "persistence.hpp"
#ಅನ್ತರ್ಭಾವ "persistence_impl.hpp"
#ಅನ್ತರ್ಭಾವ "persistence_base64_encoding.hpp"
#ಅನ್ತರ್ಭಾವ <unordered_map>
#ಅನ್ತರ್ಭಾವ <iterator>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>

#define CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

ನಾಮಾಕಾಶ cv
{

ನಾಮಾಕಾಶ fs
{

ಪೂರ್ಣಾಙ್ಕ strcasecmp(ಧ್ರುವ ವರ್ಣ* s1, ಧ್ರುವ ವರ್ಣ* s2)
{
    ಧ್ರುವ ವರ್ಣ* dummy="";
    ಯದಿ(!s1) s1=dummy;
    ಯದಿ(!s2) s2=dummy;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t i, len = std::min(len1, len2);
    ಪ್ರತಿ( i = 0; i < len; i++ )
    {
        ಪೂರ್ಣಾಙ್ಕ d = tolower((ಪೂರ್ಣಾಙ್ಕ)s1[i]) - tolower((ಪೂರ್ಣಾಙ್ಕ)s2[i]);
        ಯದಿ( d != 0 )
            ಪ್ರತ್ಯಾಗಮ d;
    }
    ಪ್ರತ್ಯಾಗಮ len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

ವರ್ಣ* itoa( ಪೂರ್ಣಾಙ್ಕ _val, ವರ್ಣ* buffer, ಪೂರ್ಣಾಙ್ಕ /*radix*/ )
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ radix = 10;
    ವರ್ಣ* ptr=buffer + 23 /* enough even for 64-bit integers */;
    unsigned val = abs(_val);

    *ptr = '\0';
    ಕುರು
    {
        unsigned r = val / radix;
        *--ptr = (ವರ್ಣ)(val - (r*radix) + '0');
        val = r;
    }
    ಯಾವತ್( val != 0 );

    ಯದಿ( _val < 0 )
        *--ptr = '-';

    ಪ್ರತ್ಯಾಗಮ ptr;
}

ವರ್ಣ* itoa( int64_t _val, ವರ್ಣ* buffer, ಪೂರ್ಣಾಙ್ಕ /*radix*/, ತಾರ್ಕಿಕ _signed)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ radix = 10;
    ವರ್ಣ* ptr=buffer + 23 /* enough even for 64-bit integers */;
    ಪೂರ್ಣಾಙ್ಕ sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : abs(_val);

    *ptr = '\0';
    ಕುರು
    {
        uint64_t r = val / radix;
        *--ptr = (ವರ್ಣ)(val - (r*radix) + '0');
        val = r;
    }
    ಯಾವತ್( val != 0 );

    ಯದಿ( sign < 0 )
        *--ptr = '-';

    ಪ್ರತ್ಯಾಗಮ ptr;
}

ವರ್ಣ* doubleToString( ವರ್ಣ* buf, size_t bufSize, ದ್ವಿಗುಣ value, ತಾರ್ಕಿಕ explicitZero )
{
    Cv64suf val;
    unsigned ieee754_hi;

    val.f = value;
    ieee754_hi = (unsigned)(val.u >> 32);

    ಯದಿ( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        ಪೂರ್ಣಾಙ್ಕ ivalue = cvRound(value);
        ಯದಿ( ivalue == value )
        {
            ಯದಿ( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            ಅನ್ಯಥಾ
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        ಅನ್ಯಥಾ
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, bufSize, "%.17g", value );

            ವರ್ಣ* ptr = buf;
            ಯದಿ( *ptr == '+' || *ptr == '-' )
                ptr++;
            ಪ್ರತಿ( ; cv_isdigit(*ptr); ptr++ )
                ;
            ಯದಿ( *ptr == ',' )
                *ptr = '.';
        }
    }
    ಅನ್ಯಥಾ
    {
        unsigned ieee754_lo = (unsigned)val.u;
        ಯದಿ( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        ಅನ್ಯಥಾ
            strcpy( buf, (ಪೂರ್ಣಾಙ್ಕ)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    ಪ್ರತ್ಯಾಗಮ buf;
}

ವರ್ಣ* floatToString( ವರ್ಣ* buf, size_t bufSize, ಭಿನ್ನ value, ತಾರ್ಕಿಕ halfprecision, ತಾರ್ಕಿಕ explicitZero )
{
    Cv32suf val;
    unsigned ieee754;
    val.f = value;
    ieee754 = val.u;

    ಯದಿ( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        ಪೂರ್ಣಾಙ್ಕ ivalue = cvRound(value);
        ಯದಿ( ivalue == value )
        {
            ಯದಿ( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            ಅನ್ಯಥಾ
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        ಅನ್ಯಥಾ
        {
            ಯದಿ (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.5g", value);
            }
            ಅನ್ಯಥಾ
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.9g", value);
            }

            ವರ್ಣ* ptr = buf;
            ಯದಿ( *ptr == '+' || *ptr == '-' )
                ptr++;
            ಪ್ರತಿ( ; cv_isdigit(*ptr); ptr++ )
                ;
            ಯದಿ( *ptr == ',' )
                *ptr = '.';
        }
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        ಅನ್ಯಥಾ
            strcpy( buf, (ಪೂರ್ಣಾಙ್ಕ)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    ಪ್ರತ್ಯಾಗಮ buf;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ symbols[] = "ucwsifdhHbUIneE";

ಸ್ಥಿರ ವರ್ಣ type_ಚಿಹ್ನೆ(ಪೂರ್ಣಾಙ್ಕ depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    ಪ್ರತ್ಯಾಗಮ symbols[depth];
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಚಿಹ್ನೆ_To_Type(ವರ್ಣ c)
{
    ಯದಿ (c == 'r')
        ಪ್ರತ್ಯಾಗಮ CV_SEQ_ELTYPE_PTR;
    ಧ್ರುವ ವರ್ಣ* pos = strchr( symbols, c );
    ಯದಿ( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    ಪ್ರತ್ಯಾಗಮ static_cast<ಪೂರ್ಣಾಙ್ಕ>(pos - symbols);
}

ವರ್ಣ* encodeFormat(ಪೂರ್ಣಾಙ್ಕ elem_type, ವರ್ಣ* dt, size_t dt_len)
{
    ಪೂರ್ಣಾಙ್ಕ cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    ವರ್ಣ ಚಿಹ್ನೆ = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_ಚಿಹ್ನೆ(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, ಚಿಹ್ನೆ);
    ಪ್ರತ್ಯಾಗಮ dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
ವರ್ಣ* encodeFormat(ಪೂರ್ಣಾಙ್ಕ elem_type, ವರ್ಣ* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    ಪ್ರತ್ಯಾಗಮ encodeFormat(elem_type, dt, max);
}

ಪೂರ್ಣಾಙ್ಕ decodeFormat( ಧ್ರುವ ವರ್ಣ* dt, ಪೂರ್ಣಾಙ್ಕ* fmt_pairs, ಪೂರ್ಣಾಙ್ಕ max_len )
{
    ಪೂರ್ಣಾಙ್ಕ fmt_pair_ಎಣಿಕೆ = 0;
    ಪೂರ್ಣಾಙ್ಕ i = 0, k = 0, len = dt ? (ಪೂರ್ಣಾಙ್ಕ)strlen(dt) : 0;

    ಯದಿ( !dt || !len )
        ಪ್ರತ್ಯಾಗಮ 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    ಪ್ರತಿ( ; k < len; k++ )
    {
        ವರ್ಣ c = dt[k];

        ಯದಿ( cv_isdigit(c) )
        {
            ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2 = c - '0';
            ಯದಿ( cv_isdigit(dt[k+1]) )
            {
                ವರ್ಣ* endptr = 0;
                ಎಣಿಕೆ_2 = (ಪೂರ್ಣಾಙ್ಕ)strtol( dt+k, &endptr, 10 );
                k = (ಪೂರ್ಣಾಙ್ಕ)(endptr - dt) - 1;
            }

            ಯದಿ( ಎಣಿಕೆ_2 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = ಎಣಿಕೆ_2;
        }
        ಅನ್ಯಥಾ
        {
            ಪೂರ್ಣಾಙ್ಕ depth = ಚಿಹ್ನೆ_To_Type(c);
            ಯದಿ( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = depth;
            ಯದಿ( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            ಅನ್ಯಥಾ
            {
                i += 2;
                ಯದಿ( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_ಎಣಿಕೆ = i/2;
    ಪ್ರತ್ಯಾಗಮ fmt_pair_ಎಣಿಕೆ;
}

ಪೂರ್ಣಾಙ್ಕ calcElemSize( ಧ್ರುವ ವರ್ಣ* dt, ಪೂರ್ಣಾಙ್ಕ initial_size )
{
    ಪೂರ್ಣಾಙ್ಕ size = 0;
    ಪೂರ್ಣಾಙ್ಕ fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_ಎಣಿಕೆ;
    ಪೂರ್ಣಾಙ್ಕ comp_size;

    fmt_pair_ಎಣಿಕೆ = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_ಎಣಿಕೆ *= 2;
    ಪ್ರತಿ( i = 0, size = initial_size; i < fmt_pair_ಎಣಿಕೆ; i += 2 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[i+1]);
        size = cvAlign( size, comp_size );
        size += comp_size * fmt_pairs[i];
    }
    ಯದಿ( initial_size == 0 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[1]);
        size = cvAlign( size, comp_size );
    }
    ಪ್ರತ್ಯಾಗಮ size;
}


ಪೂರ್ಣಾಙ್ಕ calcStructSize( ಧ್ರುವ ವರ್ಣ* dt, ಪೂರ್ಣಾಙ್ಕ initial_size )
{
    ಪೂರ್ಣಾಙ್ಕ size = calcElemSize( dt, initial_size );
    size_t elem_max_size = 0;
    ಪ್ರತಿ ( ಧ್ರುವ ವರ್ಣ * type = dt; *type != '\0'; type++ )
    {
        ವರ್ಣ v = *type;
        ಯದಿ (v >= '0' && v <= '9')
            ಅನುವರ್ತ;  // skip vector size
        ವಿಕಲ್ಪ (v)
        {
        ಪಕ್ಷ 'u': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(uchar ) ); ಭಙ್ಗ; }
        ಪಕ್ಷ 'b': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(ತಾರ್ಕಿಕ  ) ); ಭಙ್ಗ; }
        ಪಕ್ಷ 'c': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(schar ) ); ಭಙ್ಗ; }
        ಪಕ್ಷ 'w': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(ushort) ); ಭಙ್ಗ; }
        ಪಕ್ಷ 's': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(short ) ); ಭಙ್ಗ; }
        ಪಕ್ಷ 'i': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ   ) ); ಭಙ್ಗ; }
        ಪಕ್ಷ 'n': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(unsigned) ); ಭಙ್ಗ; }
        ಪಕ್ಷ 'f': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(ಭಿನ್ನ ) ); ಭಙ್ಗ; }
        ಪಕ್ಷ 'd': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(ದ್ವಿಗುಣ) ); ಭಙ್ಗ; }
        ಪಕ್ಷ 'h': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(hfloat)); ಭಙ್ಗ; }
        ಪಕ್ಷ 'H': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(bfloat)); ಭಙ್ಗ; }
        ಪಕ್ಷ 'e': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(fp8_t) ); ಭಙ್ಗ; }
        ಪಕ್ಷ 'E': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(fp8a_t)); ಭಙ್ಗ; }
        ಪಕ್ಷ 'I': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(int64_t)); ಭಙ್ಗ; }
        ಪಕ್ಷ 'U': { elem_max_size = std::max( elem_max_size, ಪರಿಮಾಣ(uint64_t)); ಭಙ್ಗ; }
        ಸ್ವಭಾವ:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (ವರ್ಣ)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<ಪೂರ್ಣಾಙ್ಕ>(elem_max_size) );
    ಪ್ರತ್ಯಾಗಮ size;
}

ಪೂರ್ಣಾಙ್ಕ decodeSimpleFormat( ಧ್ರುವ ವರ್ಣ* dt )
{
    ಪೂರ್ಣಾಙ್ಕ elem_type = -1;
    ಪೂರ್ಣಾಙ್ಕ fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_ಎಣಿಕೆ;

    fmt_pair_ಎಣಿಕೆ = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    ಯದಿ( fmt_pair_ಎಣಿಕೆ != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    ಪ್ರತ್ಯಾಗಮ elem_type;
}

}

#ಯದಿ defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#define CV_LITTLE_ENDIAN_MEM_ACCESS 1
#ಅನ್ಯಥಾ
#define CV_LITTLE_ENDIAN_MEM_ACCESS 0
#endif

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ readInt(ಧ್ರುವ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ಯದಿ CV_LITTLE_ENDIAN_MEM_ACCESS
    ಪೂರ್ಣಾಙ್ಕ val;
    memcpy(&val, p, ಪರಿಮಾಣ(val));
    ಪ್ರತ್ಯಾಗಮ val;
#ಅನ್ಯಥಾ
    ಪೂರ್ಣಾಙ್ಕ val = (ಪೂರ್ಣಾಙ್ಕ)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    ಪ್ರತ್ಯಾಗಮ val;
#endif
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ int64_t readLong(ಧ್ರುವ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ಯದಿ CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    memcpy(&val, p, ಪರಿಮಾಣ(val));
    ಪ್ರತ್ಯಾಗಮ val;
#ಅನ್ಯಥಾ
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    ಪ್ರತ್ಯಾಗಮ val0 | ((int64_t)val1 << 32);
#endif
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ readReal(ಧ್ರುವ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ಯದಿ CV_LITTLE_ENDIAN_MEM_ACCESS
    ದ್ವಿಗುಣ val;
    memcpy(&val, p, ಪರಿಮಾಣ(val));
    ಪ್ರತ್ಯಾಗಮ val;
#ಅನ್ಯಥಾ
    unsigned val0 = (unsigned)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    unsigned val1 = (unsigned)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    ಪ್ರತ್ಯಾಗಮ val.f;
#endif
}

template <typename T>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ಯದಿ CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &ival, ಪರಿಮಾಣ(ival));
#ಅನ್ಯಥಾ
    ಪ್ರತಿ (size_t i = 0, j = 0; i < ಪರಿಮಾಣ(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#endif
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ writeReal(uchar* p, ದ್ವಿಗುಣ fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ಯದಿ CV_LITTLE_ENDIAN_MEM_ACCESS
    memcpy(p, &fval, ಪರಿಮಾಣ(fval));
#ಅನ್ಯಥಾ
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



ಶೂನ್ಯ FileStorage::Impl::init() {
    flags = 0;
    buffer.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = ಅಸತ್ಯ;
    state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = ಅಸತ್ಯ;
    delayed_struct_ಕೀಲಿ = nullptr;
    delayed_struct_flags = 0;
    delayed_type_name = nullptr;
    base64_writer = nullptr;
    is_opened = ಅಸತ್ಯ;
    dummy_eof = ಅಸತ್ಯ;
    write_mode = ಅಸತ್ಯ;
    mem_mode = ಅಸತ್ಯ;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    empty_stream = ಸತ್ಯ;

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

ಶೂನ್ಯ FileStorage::Impl::release(String *out) {
    ಯದಿ (is_opened) {
        ಯದಿ (out)
            out->clear();
        ಯದಿ (write_mode) {
            ಯಾವತ್ (write_ರಾಶಿ.size() > 1) {
                endWriteStruct();
            }
            flush();
            ಯದಿ (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            ಅನ್ಯಥಾ ಯದಿ (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        ಯದಿ (mem_mode && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

ಶೂನ್ಯ FileStorage::Impl::analyze_file_name(ಧ್ರುವ std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ not_file_name = '\n';
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ parameter_begin = '?';
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ parameter_separator = '&';

    ಯದಿ (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        ಪ್ರತ್ಯಾಗಮ;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    ಯದಿ (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        ಪ್ರತಿ (size_t param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            ಯದಿ ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

ತಾರ್ಕಿಕ FileStorage::Impl::open(ಧ್ರುವ ವರ್ಣ *filename_or_buf, ಪೂರ್ಣಾಙ್ಕ _flags, ಧ್ರುವ ವರ್ಣ *encoding) {
    ತಾರ್ಕಿಕ ok = ಸತ್ಯ;
    release();

    ತಾರ್ಕಿಕ append = (_flags & 3) == FileStorage::APPEND;
    mem_mode = (_flags & FileStorage::MEMORY) != 0;

    write_mode = (_flags & 3) != 0;
    ತಾರ್ಕಿಕ write_base64 = (write_mode || append) && (_flags & FileStorage::BASE64) != 0;

    ತಾರ್ಕಿಕ isGZ = ಅಸತ್ಯ;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        ಯದಿ (!params.empty())
            filename = params[0];

        ಯದಿ (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_mode || append);
    }

    ಯದಿ (filename.size() == 0 && !mem_mode && !write_mode)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    ಯದಿ (mem_mode && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    ಯದಿ (!mem_mode) {
        size_t dot_idx = filename.find_last_of('.');
        ವರ್ಣ compression = '\0';

        ಯದಿ (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            ಯದಿ (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                ಯದಿ (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = ಸತ್ಯ;
                }

                ಅನ್ಯಥಾ ಯದಿ(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = ಸತ್ಯ;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        ಯದಿ (!isGZ) {
            file = fopen(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            ಯದಿ (!file)
            {
                CV_LOG_ERROR(ರಿಕ್ತ, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            }
        } ಅನ್ಯಥಾ {
#ಯದಿ USE_ZLIB
            ಯದಿ (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            ವರ್ಣ mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            ಯದಿ (!gzfile)
            {
                CV_LOG_ERROR(ರಿಕ್ತ, "Can't open archive: '" << filename << "' mode=" << mode);
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            }
#ಅನ್ಯಥಾ
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#endif
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_data.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    ಯದಿ (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        ಯದಿ (mem_mode)
            outbuf.clear();

        ಯದಿ (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            ಧ್ರುವ ವರ್ಣ *dot_pos = ರಿಕ್ತ;
            ಧ್ರುವ ವರ್ಣ *dot_pos2 = ರಿಕ್ತ;
            // like strrchr() implementation, but save two last positions simultaneously
            ಪ್ರತಿ (ಧ್ರುವ ವರ್ಣ *pos = &filename[0]; pos[0] != 0; pos++) {
                ಯದಿ (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            ಯದಿ (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != ರಿಕ್ತ) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } ಅನ್ಯಥಾ ಯದಿ (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        ಪೂರ್ಣಾಙ್ಕ buf_size = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        ಯದಿ (append) {
            fseek(file, 0, SEEK_END);
            ಯದಿ (ftell(file) == 0)
                append = ಅಸತ್ಯ;
        }

        write_ರಾಶಿ.clear();
        empty_stream = ಸತ್ಯ;
        write_ರಾಶಿ.push_back(FStructData("", File_ಗಂಟು::MAP | File_ಗಂಟು::EMPTY, 0));
        buffer.reserve(buf_size + 1024);
        buffer.resize(buf_size);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;

        ಯದಿ (fmt == FileStorage::FORMAT_XML) {
            size_t file_size = file ? (size_t) ftell(file) : (size_t) 0;
            ಯದಿ (!append || file_size == 0) {
                ಯದಿ (encoding && *encoding != '\0') {
                    ಯದಿ (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(strlen(encoding) < 1000);
                    ವರ್ಣ buf[1100];
                    snprintf(buf, ಪರಿಮಾಣ(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } ಅನ್ಯಥಾ
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } ಅನ್ಯಥಾ {
                ಪೂರ್ಣಾಙ್ಕ xml_buf_size = 1 << 10;
                ವರ್ಣ substr[] = "</opencv_storage>";
                ಪೂರ್ಣಾಙ್ಕ last_occurrence = -1;
                xml_buf_size = MIN(xml_buf_size, ಪೂರ್ಣಾಙ್ಕ(file_size));
                fseek(file, -xml_buf_size, SEEK_END);
                // find the last occurrence of </opencv_storage>
                ಪ್ರತಿ (;;) {
                    ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_offset = (ಪೂರ್ಣಾಙ್ಕ) ftell(file);
                    ಧ್ರುವ ವರ್ಣ *ptr0 = ಏತತ್->gets(xml_buf_size);
                    ಧ್ರುವ ವರ್ಣ *ptr = ರಿಕ್ತ;
                    ಯದಿ (!ptr0)
                        ಭಙ್ಗ;
                    ptr = ptr0;
                    ಪ್ರತಿ (;;) {
                        ptr = strstr(ptr, substr);
                        ಯದಿ (!ptr)
                            ಭಙ್ಗ;
                        last_occurrence = ಸಾಲು_offset + (ಪೂರ್ಣಾಙ್ಕ) (ptr - ptr0);
                        ptr += strlen(substr);
                    }
                }
                ಯದಿ (last_occurrence < 0) {
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

            emitter_do_not_use_direct_dereference = createXMLEmitter(ಏತತ್);
        } ಅನ್ಯಥಾ ಯದಿ (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                ಯದಿ (!append) {
                    ಯದಿ (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    ಅನ್ಯಥಾ
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            ಅನ್ಯಥಾ
                puts("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(ಏತತ್);
        } ಅನ್ಯಥಾ {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            ಯದಿ (!append)
                puts("{\n");
            ಅನ್ಯಥಾ {
                ತಾರ್ಕಿಕ valid = ಅಸತ್ಯ;
                long roffset = 0;
                ಪ್ರತಿ (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    ಧ್ರುವ ವರ್ಣ end_mark = '}';
                    ಯದಿ (fgetc(file) == end_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = ಸತ್ಯ;
                        ಭಙ್ಗ;
                    }
                }

                ಯದಿ (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } ಅನ್ಯಥಾ {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_ರಾಶಿ.back().indent = 4;
            emitter_do_not_use_direct_dereference = createJSONEmitter(ಏತತ್);
        }
        is_opened = ಸತ್ಯ;
    } ಅನ್ಯಥಾ {
        ಧ್ರುವ size_t buf_size0 = 40;
        buffer.resize(buf_size0);
        ಯದಿ (mem_mode) {
            strbuf = (ವರ್ಣ *) filename_or_buf;
            strbufsize = strlen(strbuf);
        }

        ಧ್ರುವ ವರ್ಣ *yaml_signature = "%YAML";
        ಧ್ರುವ ವರ್ಣ *json_signature = "{";
        ಧ್ರುವ ವರ್ಣ *xml_signature = "<?xml";
        ವರ್ಣ *buf = ಏತತ್->gets(16);
        CV_Assert(buf);
        ವರ್ಣ *bufPtr = cv_skip_BOM(buf);
        size_t bufOffset = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        ಯದಿ (strncmp(bufPtr, yaml_signature, strlen(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        ಅನ್ಯಥಾ ಯದಿ (strncmp(bufPtr, json_signature, strlen(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        ಅನ್ಯಥಾ ಯದಿ (strncmp(bufPtr, xml_signature, strlen(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        ಅನ್ಯಥಾ
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = bufOffset;
        bufofs = 0;

        ಯತ್ನ {
            ವರ್ಣ *ptr = bufferStart();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_ಗಂಟು ಬೇರು_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_ಗಂಟು_Space(ಬೇರು_nodes, 9);
            *rptr = File_ಗಂಟು::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            ವಿಕಲ್ಪ (fmt) {
                ಪಕ್ಷ FileStorage::FORMAT_XML:
                    parser_do_not_use_direct_dereference = createXMLParser(ಏತತ್);
                    ಭಙ್ಗ;
                ಪಕ್ಷ FileStorage::FORMAT_YAML:
                ಪಕ್ಷ FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_direct_dereference = createYAMLParser(ಏತತ್);
                    ಭಙ್ಗ;
                ಪಕ್ಷ FileStorage::FORMAT_JSON:
                    parser_do_not_use_direct_dereference = createJSONParser(ಏತತ್);
                    ಭಙ್ಗ;
                ಸ್ವಭಾವ:
                    parser_do_not_use_direct_dereference = Ptr<FileStorageParser>();
            }

            ಯದಿ (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                ಯದಿ (ok) {
                    finalizeCollection(ಬೇರು_nodes);

                    CV_Assert(!fs_data_ptrs.empty());
                    File_ಗಂಟು roots_ಗಂಟು(fs_ext, 0, 0);
                    size_t i, nroots = roots_ಗಂಟು.size();
                    File_ಗಂಟು_Iterator it = roots_ಗಂಟು.begin();

                    ಪ್ರತಿ (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        ಗೃಹಾಣ (...)
        {
            // FIXIT log error message
            is_opened = ಸತ್ಯ;
            release();
            ಕ್ಷಿಪ;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = ಸತ್ಯ;
        std::vector<ವರ್ಣ> tmpbuf;
        std::swap(buffer, tmpbuf);
        bufofs = 0;
    }
    ಪ್ರತ್ಯಾಗಮ ok;
}

ಶೂನ್ಯ FileStorage::Impl::puts(ಧ್ರುವ ವರ್ಣ *str) {
    CV_Assert(write_mode);
    ಯದಿ (mem_mode)
        std::copy(str, str + strlen(str), std::back_inserter(outbuf));
    ಅನ್ಯಥಾ ಯದಿ (file)
        fputs(str, file);
#ಯದಿ USE_ZLIB
    ಅನ್ಯಥಾ ಯದಿ (gzfile)
        gzputs(gzfile, str);
#endif
    ಅನ್ಯಥಾ
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

ವರ್ಣ *FileStorage::Impl::getsFromFile(ವರ್ಣ *buf, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2) {
    ಯದಿ (file)
        ಪ್ರತ್ಯಾಗಮ fgets(buf, ಎಣಿಕೆ_2, file);
#ಯದಿ USE_ZLIB
    ಯದಿ (gzfile)
        ಪ್ರತ್ಯಾಗಮ gzgets(gzfile, buf, ಎಣಿಕೆ_2);
#endif
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

ವರ್ಣ *FileStorage::Impl::gets(size_t max_ಎಣಿಕೆ) {
    ಯದಿ (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        ಧ್ರುವ ವರ್ಣ *instr = strbuf;
        ಪ್ರತಿ (; i < len; i++) {
            ವರ್ಣ c = instr[i];
            ಯದಿ (c == '\0' || c == '\n') {
                ಯದಿ (c == '\n')
                    i++;
                ಭಙ್ಗ;
            }
        }
        size_t ಎಣಿಕೆ_2 = i - strbufpos;
        ಯದಿ (max_ಎಣಿಕೆ == 0 || max_ಎಣಿಕೆ > ಎಣಿಕೆ_2)
            max_ಎಣಿಕೆ = ಎಣಿಕೆ_2;
        buffer.resize(std::max(buffer.size(), max_ಎಣಿಕೆ + 8));
        memcpy(&buffer[0], instr + strbufpos, max_ಎಣಿಕೆ);
        buffer[max_ಎಣಿಕೆ] = '\0';
        strbufpos = i;
        ಪ್ರತ್ಯಾಗಮ max_ಎಣಿಕೆ > 0 ? &buffer[0] : 0;
    }

    ಧ್ರುವ size_t MAX_BLOCK_SIZE = INT_MAX / 2; // hopefully, that will be enough
    ಯದಿ (max_ಎಣಿಕೆ == 0)
        max_ಎಣಿಕೆ = MAX_BLOCK_SIZE;
    ಅನ್ಯಥಾ
        CV_Assert(max_ಎಣಿಕೆ < MAX_BLOCK_SIZE);
    size_t ofs = 0;

    ಪ್ರತಿ (;;) {
        ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2 = (ಪೂರ್ಣಾಙ್ಕ) std::min(buffer.size() - ofs - 16, max_ಎಣಿಕೆ);
        ವರ್ಣ *ptr = getsFromFile(&buffer[ofs], ಎಣಿಕೆ_2 + 1);
        ಯದಿ (!ptr)
            ಭಙ್ಗ;
        ಪೂರ್ಣಾಙ್ಕ delta = (ಪೂರ್ಣಾಙ್ಕ) strlen(ptr);
        ofs += delta;
        max_ಎಣಿಕೆ -= delta;
        ಯದಿ (delta == 0 || ptr[delta - 1] == '\n' || max_ಎಣಿಕೆ == 0)
            ಭಙ್ಗ;
        ಯದಿ (delta == ಎಣಿಕೆ_2)
            buffer.resize((size_t) (buffer.size() * 1.5));
    }
    ಪ್ರತ್ಯಾಗಮ ofs > 0 ? &buffer[0] : 0;
}

ವರ್ಣ *FileStorage::Impl::gets() {
    ವರ್ಣ *ptr = ಏತತ್->gets(0);
    ಯದಿ (!ptr) {
        ptr = bufferStart();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        ಪ್ರತ್ಯಾಗಮ 0;
    } ಅನ್ಯಥಾ {
        size_t l = strlen(ptr);
        ಯದಿ (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    ಪ್ರತ್ಯಾಗಮ ptr;
}

ತಾರ್ಕಿಕ FileStorage::Impl::eof() {
    ಯದಿ (dummy_eof)
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಯದಿ (strbuf)
        ಪ್ರತ್ಯಾಗಮ strbufpos >= strbufsize;
    ಯದಿ (file)
        ಪ್ರತ್ಯಾಗಮ feof(file) != 0;
#ಯದಿ USE_ZLIB
    ಯದಿ (gzfile)
        ಪ್ರತ್ಯಾಗಮ gzeof(gzfile) != 0;
#endif
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ಶೂನ್ಯ FileStorage::Impl::setEof() {
    dummy_eof = ಸತ್ಯ;
}

ಶೂನ್ಯ FileStorage::Impl::closeFile() {
    ಯದಿ (file)
        fclose(file);
#ಯದಿ USE_ZLIB
    ಅನ್ಯಥಾ ಯದಿ (gzfile)
        gzclose(gzfile);
#endif
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = ಅಸತ್ಯ;
}

ಶೂನ್ಯ FileStorage::Impl::rewind() {
    ಯದಿ (file)
        ::rewind(file);
#ಯದಿ USE_ZLIB
    ಅನ್ಯಥಾ ಯದಿ (gzfile)
        gzrewind(gzfile);
#endif
    strbufpos = 0;
}

ವರ್ಣ *FileStorage::Impl::resizeWriteBuffer(ವರ್ಣ *ptr, ಪೂರ್ಣಾಙ್ಕ len) {
    ಧ್ರುವ ವರ್ಣ *buffer_end = &buffer[0] + buffer.size();
    ಯದಿ (ptr + len < buffer_end)
        ಪ್ರತ್ಯಾಗಮ ptr;

    ಧ್ರುವ ವರ್ಣ *buffer_start = &buffer[0];
    ಪೂರ್ಣಾಙ್ಕ written_len = (ಪೂರ್ಣಾಙ್ಕ) (ptr - buffer_start);

    CV_Assert(written_len <= (ಪೂರ್ಣಾಙ್ಕ) buffer.size());
    ಪೂರ್ಣಾಙ್ಕ new_size = (ಪೂರ್ಣಾಙ್ಕ) ((buffer_end - buffer_start) * 3 / 2);
    new_size = MAX(written_len + len, new_size);
    buffer.reserve(new_size + 256);
    buffer.resize(new_size);
    bufofs = written_len;
    ಪ್ರತ್ಯಾಗಮ &buffer[0] + bufofs;
}

ವರ್ಣ *FileStorage::Impl::flush() {
    ವರ್ಣ *buffer_start = &buffer[0];
    ವರ್ಣ *ptr = buffer_start + bufofs;

    ಯದಿ (ptr > buffer_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(buffer_start);
        bufofs = 0;
    }

    ಪೂರ್ಣಾಙ್ಕ indent = write_ರಾಶಿ.back().indent;

    ಯದಿ (space != indent) {
        memset(buffer_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = buffer_start + bufofs;

    ಪ್ರತ್ಯಾಗಮ ptr;
}

ಶೂನ್ಯ FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(ಅಸತ್ಯ);
    ಯದಿ (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_ರಾಶಿ.empty());

    FStructData &current_struct = write_ರಾಶಿ.back();
    ಯದಿ (fmt == FileStorage::FORMAT_JSON && !File_ಗಂಟು::isFlow(current_struct.flags) && write_ರಾಶಿ.size() > 1)
        current_struct.indent = write_ರಾಶಿ[write_ರಾಶಿ.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_ರಾಶಿ.pop_back();
    ಯದಿ (!write_ರಾಶಿ.empty())
        write_ರಾಶಿ.back().flags &= ~File_ಗಂಟು::EMPTY;
}

ಶೂನ್ಯ FileStorage::Impl::startWriteStruct_helper(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಪೂರ್ಣಾಙ್ಕ struct_flags,
                                                ಧ್ರುವ ವರ್ಣ *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (File_ಗಂಟು::TYPE_MASK | File_ಗಂಟು::FLOW)) | File_ಗಂಟು::EMPTY;
    ಯದಿ (!File_ಗಂಟು::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    ಯದಿ (type_name && type_name[0] == '\0')
        type_name = 0;

    FStructData s = getEmitter().startWriteStruct(write_ರಾಶಿ.back(), ಕೀಲಿ, struct_flags, type_name);

    write_ರಾಶಿ.push_back(s);
    size_t write_ರಾಶಿ_size = write_ರಾಶಿ.size();
    ಯದಿ (write_ರಾಶಿ_size > 1)
        write_ರಾಶಿ[write_ರಾಶಿ_size - 2].flags &= ~File_ಗಂಟು::EMPTY;

    ಯದಿ (fmt != FileStorage::FORMAT_JSON && !File_ಗಂಟು::isFlow(s.flags))
        flush();

    ಯದಿ (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_ಗಂಟು::isMap(struct_flags)) {
        getEmitter().ಲಿಖ("type_id", type_name, ಅಸತ್ಯ);
    }
}

ಶೂನ್ಯ FileStorage::Impl::startWriteStruct(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಪೂರ್ಣಾಙ್ಕ struct_flags,
                                         ಧ್ರುವ ವರ್ಣ *type_name) {
    check_if_write_struct_is_delayed(ಅಸತ್ಯ);
    ಯದಿ (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    ಯದಿ (state_of_writing_base64 == FileStorage_API::Uncertain && File_ಗಂಟು::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(ಕೀಲಿ, struct_flags, type_name);
    } ಅನ್ಯಥಾ ಯದಿ (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        ಯದಿ ((File_ಗಂಟು::TYPE_MASK & struct_flags) != File_ಗಂಟು::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        ಅನ್ಯಥಾ ಯದಿ (state_of_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(ಕೀಲಿ, struct_flags, "binary");

        ಯದಿ (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::InUse);
    } ಅನ್ಯಥಾ {
        /* Won't output Base64 data */
        ಯದಿ (state_of_writing_base64 == FileStorage_API::InUse)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(ಕೀಲಿ, struct_flags, type_name);

        ಯದಿ (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::NotUse);
    }
}

ಶೂನ್ಯ FileStorage::Impl::writeComment(ಧ್ರುವ ವರ್ಣ *comment, ತಾರ್ಕಿಕ eol_comment) {
    CV_Assert(write_mode);
    getEmitter().writeComment(comment, eol_comment);
}

ಶೂನ್ಯ FileStorage::Impl::startNextStream() {
    CV_Assert(write_mode);
    ಯದಿ (!empty_stream) {
        ಯಾವತ್ (!write_ರಾಶಿ.empty())
            endWriteStruct();
        flush();
        getEmitter().startNextStream();
        empty_stream = ಸತ್ಯ;
        write_ರಾಶಿ.push_back(FStructData("", File_ಗಂಟು::EMPTY, 0));
        bufofs = 0;
    }
}

ಶೂನ್ಯ FileStorage::Impl::ಲಿಖ(ಧ್ರುವ String &ಕೀಲಿ, ಪೂರ್ಣಾಙ್ಕ value) {
    CV_Assert(write_mode);
    getEmitter().ಲಿಖ(ಕೀಲಿ.c_str(), value);
}

ಶೂನ್ಯ FileStorage::Impl::ಲಿಖ(ಧ್ರುವ String &ಕೀಲಿ, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().ಲಿಖ(ಕೀಲಿ.c_str(), value);
}

ಶೂನ್ಯ FileStorage::Impl::ಲಿಖ(ಧ್ರುವ String &ಕೀಲಿ, ದ್ವಿಗುಣ value) {
    CV_Assert(write_mode);
    getEmitter().ಲಿಖ(ಕೀಲಿ.c_str(), value);
}

ಶೂನ್ಯ FileStorage::Impl::ಲಿಖ(ಧ್ರುವ String &ಕೀಲಿ, ಧ್ರುವ String &value) {
    CV_Assert(write_mode);
    getEmitter().ಲಿಖ(ಕೀಲಿ.c_str(), value.c_str(), ಅಸತ್ಯ);
}

ಶೂನ್ಯ FileStorage::Impl::writeRawData(ಧ್ರುವ std::string &dt, ಧ್ರುವ ಶೂನ್ಯ *_data, size_t len) {
    CV_Assert(write_mode);

    ಯದಿ (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        writeRawDataBase64(_data, len, dt.c_str());
        ಪ್ರತ್ಯಾಗಮ;
    } ಅನ್ಯಥಾ ಯದಿ (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elemSize = fs::calcStructSize(dt.c_str(), 0);
    CV_Assert(elemSize);
    CV_Assert(len % elemSize == 0);
    len /= elemSize;

    ತಾರ್ಕಿಕ explicitZero = fmt == FileStorage::FORMAT_JSON;
    ಧ್ರುವ uchar *data0 = (ಧ್ರುವ uchar *) _data;
    ಪೂರ್ಣಾಙ್ಕ fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_ಎಣಿಕೆ;
    ವರ್ಣ buf[256] = "";

    fmt_pair_ಎಣಿಕೆ = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    ಯದಿ (!len)
        ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    ಯದಿ (fmt_pair_ಎಣಿಕೆ == 1) {
        fmt_pairs[0] *= (ಪೂರ್ಣಾಙ್ಕ) len;
        len = 1;
    }

    ಪ್ರತಿ (; len--; data0 += elemSize) {
        ಪೂರ್ಣಾಙ್ಕ offset = 0;
        ಪ್ರತಿ (k = 0; k < fmt_pair_ಎಣಿಕೆ; k++) {
            ಪೂರ್ಣಾಙ್ಕ i, ಎಣಿಕೆ_2 = fmt_pairs[k * 2];
            ಪೂರ್ಣಾಙ್ಕ elem_type = fmt_pairs[k * 2 + 1];
            ಪೂರ್ಣಾಙ್ಕ elem_size = CV_ELEM_SIZE(elem_type);
            ಧ್ರುವ ವರ್ಣ *ptr;

            offset = cvAlign(offset, elem_size);
            ಧ್ರುವ uchar *data = data0 + offset;

            ಪ್ರತಿ (i = 0; i < ಎಣಿಕೆ_2; i++) {
                ವಿಕಲ್ಪ (elem_type) {
                    ಪಕ್ಷ CV_8U:
                        ptr = fs::itoa(*(uchar *) data, buf, 10);
                        data++;
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_Bool:
                        ptr = fs::itoa(*(uchar *) data != 0, buf, 10);
                        data++;
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_8S:
                        ptr = fs::itoa(*(ವರ್ಣ *) data, buf, 10);
                        data++;
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_16U:
                        ptr = fs::itoa(*(ushort *) data, buf, 10);
                        data += ಪರಿಮಾಣ(ushort);
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_16S:
                        ptr = fs::itoa(*(short *) data, buf, 10);
                        data += ಪರಿಮಾಣ(short);
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_32U:
                        ptr = fs::itoa((int64_t)*(unsigned*) data, buf, 10, ಅಸತ್ಯ);
                        data += ಪರಿಮಾಣ(unsigned);
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_32S:
                        ptr = fs::itoa(*(ಪೂರ್ಣಾಙ್ಕ *) data, buf, 10);
                        data += ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ);
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_64U:
                        ptr = fs::itoa(*(uint64_t*) data, buf, 10, ಅಸತ್ಯ);
                        data += ಪರಿಮಾಣ(uint64_t);
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_64S:
                        ptr = fs::itoa(*(int64_t*) data, buf, 10, ಸತ್ಯ);
                        data += ಪರಿಮಾಣ(int64_t);
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_32F:
                        ptr = fs::floatToString(buf, ಪರಿಮಾಣ(buf), *(ಭಿನ್ನ *) data, ಅಸತ್ಯ, explicitZero);
                        data += ಪರಿಮಾಣ(ಭಿನ್ನ);
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_64F:
                        ptr = fs::doubleToString(buf, ಪರಿಮಾಣ(buf), *(ದ್ವಿಗುಣ *) data, explicitZero);
                        data += ಪರಿಮಾಣ(ದ್ವಿಗುಣ);
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_16F:
                        ptr = fs::floatToString(buf, ಪರಿಮಾಣ(buf), (ಭಿನ್ನ) *(hfloat *) data, ಸತ್ಯ, explicitZero);
                        data += ಪರಿಮಾಣ(hfloat);
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_16BF:
                        ptr = fs::floatToString(buf, ಪರಿಮಾಣ(buf), (ಭಿನ್ನ) *(bfloat *) data, ಸತ್ಯ, explicitZero);
                        data += ಪರಿಮಾಣ(bfloat);
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, ಪರಿಮಾಣ(buf), (ಭಿನ್ನ) *(fp8_t *) data, ಸತ್ಯ, explicitZero);
                        data += ಪರಿಮಾಣ(fp8_t);
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, ಪರಿಮಾಣ(buf), (ಭಿನ್ನ) *(fp8a_t *) data, ಸತ್ಯ, explicitZero);
                        data += ಪರಿಮಾಣ(fp8a_t);
                        ಭಙ್ಗ;
                    ಸ್ವಭಾವ:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        ಪ್ರತ್ಯಾಗಮ;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (ಪೂರ್ಣಾಙ್ಕ) (data - data0);
        }
    }
}

ಶೂನ್ಯ FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(ಅಸತ್ಯ);

    ಯದಿ (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_to_Base64_state(FileStorage_API::Base64State::Uncertain);
}

ಶೂನ್ಯ FileStorage::Impl::switch_to_Base64_state(FileStorage_API::Base64State new_state) {
    ಧ್ರುವ ವರ್ಣ *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    ಧ್ರುವ ವರ್ಣ *err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    ವಿಕಲ್ಪ (state_of_writing_base64) {
        ಪಕ್ಷ FileStorage_API::Base64State::Uncertain:
            ವಿಕಲ್ಪ (new_state) {
                ಪಕ್ಷ FileStorage_API::Base64State::InUse:
                {
                    CV_DbgAssert(base64_writer == 0);
                    ತಾರ್ಕಿಕ can_indent = (fmt != cv::FileStorage::Mode::FORMAT_JSON);
                    base64_writer = ನೂತನ base64::Base64Writer(*ಏತತ್, can_indent);
                    ಯದಿ (!can_indent) {
                        ವರ್ಣ *ptr = bufferPtr();
                        *ptr++ = '\0';
                        puts(bufferStart());
                        setBufferPtr(bufferStart());
                        memset(bufferStart(), 0, static_cast<ಪೂರ್ಣಾಙ್ಕ>(space));
                        puts("\"$base64$");
                    }
                    ಭಙ್ಗ;
                }
                ಪಕ್ಷ FileStorage_API::Base64State::Uncertain:
                    ಭಙ್ಗ;
                ಪಕ್ಷ FileStorage_API::Base64State::NotUse:
                    ಭಙ್ಗ;
                ಸ್ವಭಾವ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ಭಙ್ಗ;
            }
            ಭಙ್ಗ;
        ಪಕ್ಷ FileStorage_API::Base64State::InUse:
            ವಿಕಲ್ಪ (new_state) {
                ಪಕ್ಷ FileStorage_API::Base64State::InUse:
                ಪಕ್ಷ FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    ಭಙ್ಗ;
                ಪಕ್ಷ FileStorage_API::Base64State::Uncertain:
                    delete base64_writer;
                    base64_writer = 0;
                    ಯದಿ ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        setBufferPtr(bufferStart());
                        flush();
                        memset(bufferStart(), 0, static_cast<ಪೂರ್ಣಾಙ್ಕ>(space) );
                        setBufferPtr(bufferStart());
                    }
                    ಭಙ್ಗ;
                ಸ್ವಭಾವ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ಭಙ್ಗ;
            }
            ಭಙ್ಗ;
        ಪಕ್ಷ FileStorage_API::Base64State::NotUse:
            ವಿಕಲ್ಪ (new_state) {
                ಪಕ್ಷ FileStorage_API::Base64State::InUse:
                ಪಕ್ಷ FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    ಭಙ್ಗ;
                ಪಕ್ಷ FileStorage_API::Base64State::Uncertain:
                    ಭಙ್ಗ;
                ಸ್ವಭಾವ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ಭಙ್ಗ;
            }
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            ಭಙ್ಗ;
    }

    state_of_writing_base64 = new_state;
}

ಶೂನ್ಯ FileStorage::Impl::make_write_struct_delayed(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಪೂರ್ಣಾಙ್ಕ struct_flags, ಧ್ರುವ ವರ್ಣ *type_name) {
    CV_Assert(is_write_struct_delayed == ಅಸತ್ಯ);
    CV_DbgAssert(delayed_struct_ಕೀಲಿ == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    ಯದಿ (ಕೀಲಿ != nullptr) {
        delayed_struct_ಕೀಲಿ = ನೂತನ ವರ್ಣ[strlen(ಕೀಲಿ) + 1U];
        strcpy(delayed_struct_ಕೀಲಿ, ಕೀಲಿ);
    }

    ಯದಿ (type_name != nullptr) {
        delayed_type_name = ನೂತನ ವರ್ಣ[strlen(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = ಸತ್ಯ;
}

ಶೂನ್ಯ FileStorage::Impl::check_if_write_struct_is_delayed(ತಾರ್ಕಿಕ change_type_to_base64) {
    ಯದಿ (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_ಕೀಲಿ;
        std::string type_name;
        ಪೂರ್ಣಾಙ್ಕ struct_flags = delayed_struct_flags;

        ಯದಿ (delayed_struct_ಕೀಲಿ != nullptr && *delayed_struct_ಕೀಲಿ != '\0') {
            struct_ಕೀಲಿ.assign(delayed_struct_ಕೀಲಿ);
        }
        ಯದಿ (delayed_type_name != nullptr && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        delete[] delayed_struct_ಕೀಲಿ;
        delete[] delayed_type_name;
        delayed_struct_ಕೀಲಿ = nullptr;
        delayed_struct_flags = 0;
        delayed_type_name = nullptr;

        is_write_struct_delayed = ಅಸತ್ಯ;

        /* call */
        ಯದಿ (change_type_to_base64) {
            startWriteStruct_helper(struct_ಕೀಲಿ.c_str(), struct_flags, "binary");
            ಯದಿ (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::InUse);
        } ಅನ್ಯಥಾ {
            startWriteStruct_helper(struct_ಕೀಲಿ.c_str(), struct_flags, type_name.c_str());
            ಯದಿ (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::NotUse);
        }
    }
}

ಶೂನ್ಯ FileStorage::Impl::writeRawDataBase64(ಧ್ರುವ ಶೂನ್ಯ *_data, size_t len, ಧ್ರುವ ವರ್ಣ *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(ಸತ್ಯ);

    ಯದಿ (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } ಅನ್ಯಥಾ ಯದಿ (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->ಲಿಖ(_data, len, dt);
}

File_ಗಂಟು FileStorage::Impl::get_First_ತುದಿ_Level_ಗಂಟು() ಧ್ರುವ {
    ಪ್ರತ್ಯಾಗಮ roots.empty() ? File_ಗಂಟು() : roots[0];
}

File_ಗಂಟು FileStorage::Impl::ಬೇರು(ಪೂರ್ಣಾಙ್ಕ streamIdx) ಧ್ರುವ {
    ಪ್ರತ್ಯಾಗಮ streamIdx >= 0 && streamIdx < (ಪೂರ್ಣಾಙ್ಕ) roots.size() ? roots[streamIdx] : File_ಗಂಟು();
}

File_ಗಂಟು FileStorage::Impl::operator[](ಧ್ರುವ String &nodename) ಧ್ರುವ {
    ಪ್ರತ್ಯಾಗಮ ಏತತ್->operator[](nodename.c_str());
}

File_ಗಂಟು FileStorage::Impl::operator[](ಧ್ರುವ ವರ್ಣ * /*nodename*/) ಧ್ರುವ {
    ಪ್ರತ್ಯಾಗಮ File_ಗಂಟು();
}

ಪೂರ್ಣಾಙ್ಕ FileStorage::Impl::getFormat() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ fmt; }

ವರ್ಣ *FileStorage::Impl::bufferPtr() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ *) (&buffer[0] + bufofs); }

ವರ್ಣ *FileStorage::Impl::bufferStart() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ *) &buffer[0]; }

ವರ್ಣ *FileStorage::Impl::bufferEnd() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ *) (&buffer[0] + buffer.size()); }

ಶೂನ್ಯ FileStorage::Impl::setBufferPtr(ವರ್ಣ *ptr) {
    ವರ್ಣ *bufferstart = bufferStart();
    CV_Assert(ptr >= bufferstart && ptr <= bufferEnd());
    bufofs = ptr - bufferstart;
}

ಪೂರ್ಣಾಙ್ಕ FileStorage::Impl::wrapMargin() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ wrap_margin; }

FStructData &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_ರಾಶಿ.empty());
    ಪ್ರತ್ಯಾಗಮ write_ರಾಶಿ.back();
}

ಶೂನ್ಯ FileStorage::Impl::setNonEmpty() {
    empty_stream = ಅಸತ್ಯ;
}

ಶೂನ್ಯ FileStorage::Impl::processSpecialDouble(ವರ್ಣ *buf, ದ್ವಿಗುಣ *value, ವರ್ಣ **endptr) {
    FileStorage_API *fs = ಏತತ್;
    ವರ್ಣ c = buf[0];
    ಪೂರ್ಣಾಙ್ಕ inf_hi = 0x7ff00000;

    ಯದಿ (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    ಯದಿ (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    ಯದಿ (toupper(buf[1]) == 'I' && toupper(buf[2]) == 'N' && toupper(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    ಅನ್ಯಥಾ ಯದಿ (toupper(buf[1]) == 'N' && toupper(buf[2]) == 'A' && toupper(buf[3]) == 'N')
        v.u = (uint64) -1;
    ಅನ್ಯಥಾ
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

ದ್ವಿಗುಣ FileStorage::Impl::strtod(ವರ್ಣ *ptr, ವರ್ಣ **endptr) {
    ದ್ವಿಗುಣ fval = ::strtod(ptr, endptr);
    ಯದಿ (**endptr == '.') {
        ವರ್ಣ *dot_pos = *endptr;
        *dot_pos = ',';
        ದ್ವಿಗುಣ fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        ಯದಿ (*endptr > dot_pos)
            fval = fval2;
        ಅನ್ಯಥಾ
            *endptr = dot_pos;
    }

    ಯದಿ (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    ಪ್ರತ್ಯಾಗಮ fval;
}

ಶೂನ್ಯ FileStorage::Impl::convertToCollection(ಪೂರ್ಣಾಙ್ಕ type, File_ಗಂಟು &ಗಂಟು) {
    CV_Assert(type == File_ಗಂಟು::SEQ || type == File_ಗಂಟು::MAP);

    ಪೂರ್ಣಾಙ್ಕ ಗಂಟು_type = ಗಂಟು.type();
    ಯದಿ (ಗಂಟು_type == type)
        ಪ್ರತ್ಯಾಗಮ;

    ತಾರ್ಕಿಕ named = ಗಂಟು.isNamed();
    uchar *ptr = ಗಂಟು.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    ದ್ವಿಗುಣ fval = 0;
    std::string sval;
    ತಾರ್ಕಿಕ add_first_scalar = ಅಸತ್ಯ;

    ಯದಿ (ಗಂಟು_type != File_ಗಂಟು::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_ಗಂಟು::SEQ);
        ಯದಿ (ಗಂಟು_type == File_ಗಂಟು::INT) {
            ival = readLong(ptr);
            add_first_scalar = ಸತ್ಯ;
        } ಅನ್ಯಥಾ ಯದಿ (ಗಂಟು_type == File_ಗಂಟು::REAL) {
            fval = readReal(ptr);
            add_first_scalar = ಸತ್ಯ;
        } ಅನ್ಯಥಾ ಯದಿ (ಗಂಟು_type == File_ಗಂಟು::STRING) {
            sval = std::string(ಗಂಟು);
            add_first_scalar = ಸತ್ಯ;
        } ಅನ್ಯಥಾ
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", ಗಂಟು_type));
    }

    ptr = reserve_ಗಂಟು_Space(ಗಂಟು, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_ಗಂಟು::NAMED : 0));
    // name has been copied automatically
    ಯದಿ (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    ಯದಿ (add_first_scalar)
        add_ಗಂಟು(ಗಂಟು, std::string(), ಗಂಟು_type,
                ಗಂಟು_type == File_ಗಂಟು::INT ? (ಧ್ರುವ ಶೂನ್ಯ *) &ival :
                ಗಂಟು_type == File_ಗಂಟು::REAL ? (ಧ್ರುವ ಶೂನ್ಯ *) &fval :
                ಗಂಟು_type == File_ಗಂಟು::STRING ? (ಧ್ರುವ ಶೂನ್ಯ *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_ಗಂಟು_Space(File_ಗಂಟು &ಗಂಟು, size_t sz) {
    ತಾರ್ಕಿಕ shrinkBlock = ಅಸತ್ಯ;
    size_t shrinkBlockIdx = 0, shrinkSize = 0;

    uchar *ptr = 0, *blockEnd = 0;

    ಯದಿ (!fs_data_ptrs.empty()) {
        size_t blockIdx = ಗಂಟು.blockIdx;
        size_t ofs = ಗಂಟು.ofs;
        CV_Assert(blockIdx == fs_data_ptrs.size() - 1);
        CV_Assert(ofs <= fs_data_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_data_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_data_ptrs[blockIdx] + ofs;
        blockEnd = fs_data_ptrs[blockIdx] + fs_data_blksz[blockIdx];

        CV_Assert(ptr >= fs_data_ptrs[blockIdx] && ptr <= blockEnd);
        ಯದಿ (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            ಪ್ರತ್ಯಾಗಮ ptr;
        }

        ಯದಿ (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_data[blockIdx]->resize(sz);
            ptr = &fs_data[blockIdx]->at(0);
            fs_data_ptrs[blockIdx] = ptr;
            fs_data_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            ಪ್ರತ್ಯಾಗಮ ptr;
        }

        shrinkBlock = ಸತ್ಯ;
        shrinkBlockIdx = blockIdx;
        shrinkSize = ofs;
    }

    size_t blockSize = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(blockSize);
    fs_data.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_data_ptrs.push_back(new_ptr);
    fs_data_blksz.push_back(blockSize);
    ಗಂಟು.blockIdx = fs_data_ptrs.size() - 1;
    ಗಂಟು.ofs = 0;
    freeSpaceOfs = sz;

    ಯದಿ (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        ಯದಿ (ptr[0] & File_ಗಂಟು::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    ಯದಿ (shrinkBlock) {
        fs_data[shrinkBlockIdx]->resize(shrinkSize);
        fs_data_blksz[shrinkBlockIdx] = shrinkSize;
    }

    ಪ್ರತ್ಯಾಗಮ new_ptr;
}

unsigned FileStorage::Impl::getStringOfs(ಧ್ರುವ std::string &ಕೀಲಿ) ಧ್ರುವ {
    str_hash_t::const_iterator it = str_hash.find(ಕೀಲಿ);
    ಪ್ರತ್ಯಾಗಮ it != str_hash.end() ? it->second : 0;
}

File_ಗಂಟು FileStorage::Impl::add_ಗಂಟು(File_ಗಂಟು &collection, ಧ್ರುವ std::string &ಕೀಲಿ,
                                    ಪೂರ್ಣಾಙ್ಕ elem_type, ಧ್ರುವ ಶೂನ್ಯ *value, ಪೂರ್ಣಾಙ್ಕ len) {
    FileStorage_API *fs = ಏತತ್;
    ತಾರ್ಕಿಕ noname = ಕೀಲಿ.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(ಕೀಲಿ.c_str(), "_") == 0);
    convertToCollection(noname ? File_ಗಂಟು::SEQ : File_ಗಂಟು::MAP, collection);

    ತಾರ್ಕಿಕ isseq = collection.empty() ? ಅಸತ್ಯ : collection.isSeq();
    ಯದಿ (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    unsigned strofs = 0;
    ಯದಿ (!noname) {
        strofs = getStringOfs(ಕೀಲಿ);
        ಯದಿ (!strofs) {
            strofs = (unsigned) str_hash_data.size();
            size_t keysize = ಕೀಲಿ.size() + 1;
            str_hash_data.resize(strofs + keysize);
            memcpy(&str_hash_data[0] + strofs, &ಕೀಲಿ[0], keysize);
            str_hash.insert(std::make_pair(ಕೀಲಿ, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t blockIdx = fs_data_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_ಗಂಟು ಗಂಟು(fs_ext, blockIdx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_ಗಂಟು_Space(ಗಂಟು, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_ಗಂಟು::NAMED));
    ಯದಿ (elem_type == File_ಗಂಟು::NONE)
        freeSpaceOfs -= 8;

    ಯದಿ (!noname) {
        writeInt(ptr, (ಪೂರ್ಣಾಙ್ಕ) strofs);
        ptr += 4;
    }

    ಯದಿ (elem_type == File_ಗಂಟು::SEQ || elem_type == File_ಗಂಟು::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    ಯದಿ (value)
        ಗಂಟು.set_ಮೌಲ್ಯ(elem_type, value, len);

    ಯದಿ (collection.isNamed())
        cp += 4;
    ಪೂರ್ಣಾಙ್ಕ nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    ಪ್ರತ್ಯಾಗಮ ಗಂಟು;
}

ಶೂನ್ಯ FileStorage::Impl::finalizeCollection(File_ಗಂಟು &collection) {
    ಯದಿ (!collection.isSeq() && !collection.isMap())
        ಪ್ರತ್ಯಾಗಮ;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    ಯದಿ (*ptr0 & File_ಗಂಟು::NAMED)
        ptr += 4;
    size_t blockIdx = collection.blockIdx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t rawSize = 4;
    unsigned sz = (unsigned) readInt(ptr + 4);
    ಯದಿ (sz > 0) {
        size_t lastBlockIdx = fs_data_ptrs.size() - 1;

        ಪ್ರತಿ (; blockIdx < lastBlockIdx; blockIdx++) {
            rawSize += fs_data_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    rawSize += freeSpaceOfs - ofs;
    writeInt(ptr, (ಪೂರ್ಣಾಙ್ಕ) rawSize);
}

ಶೂನ್ಯ FileStorage::Impl::normalize_ಗಂಟು_Ofs(size_t &blockIdx, size_t &ofs) ಧ್ರುವ {
    ಯಾವತ್ (ofs >= fs_data_blksz[blockIdx]) {
        ಯದಿ (blockIdx == fs_data_blksz.size() - 1) {
            CV_Assert(ofs == fs_data_blksz[blockIdx]);
            ಭಙ್ಗ;
        }
        ofs -= fs_data_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    ಪ್ರತ್ಯಾಗಮ state_of_writing_base64;
}

ಪೂರ್ಣಾಙ್ಕ FileStorage::Impl::get_space() {
    ಪ್ರತ್ಯಾಗಮ space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = ಸತ್ಯ;
}

ಶೂನ್ಯ FileStorage::Impl::Base64Decoder::init(ಧ್ರುವ Ptr<FileStorageParser> &_parser, ವರ್ಣ *_ptr, ಪೂರ್ಣಾಙ್ಕ _indent) {
    parser_do_not_use_direct_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = ಅಸತ್ಯ;
}

ತಾರ್ಕಿಕ FileStorage::Impl::Base64Decoder::readMore(ಪೂರ್ಣಾಙ್ಕ needed) {
    ಸ್ಥಿರ ಧ್ರುವ uchar base64tab[] =
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

    ಯದಿ (eos)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    ಪ್ರತಿ (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    ವರ್ಣ *beg = 0, *end = 0;
    ತಾರ್ಕಿಕ ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    ಯದಿ (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = ಸತ್ಯ;
        size_t tc = totalchars;
        ಪ್ರತಿ (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    ಪೂರ್ಣಾಙ್ಕ i = 0, j, n = (ಪೂರ್ಣಾಙ್ಕ) encoded.size();
    ಯದಿ (n > 0) {
        ಧ್ರುವ uchar *tab = base64tab;
        ವರ್ಣ *src = &encoded[0];

        ಪ್ರತಿ (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(ಪೂರ್ಣಾಙ್ಕ) (uchar) src[i]], c = tab[(ಪೂರ್ಣಾಙ್ಕ) (uchar) src[i + 1]];
            uchar b = tab[(ಪೂರ್ಣಾಙ್ಕ) (uchar) src[i + 2]], a = tab[(ಪೂರ್ಣಾಙ್ಕ) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    ಯದಿ (i > 0 && encoded[i - 1] == '=') {
        ಯದಿ (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        ಯದಿ (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    ಪ್ರತಿ (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    ಯದಿ (ofs >= sz && !readMore(1))
        ಪ್ರತ್ಯಾಗಮ (uchar) 0;
    ಪ್ರತ್ಯಾಗಮ decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    ಯದಿ (ofs + 2 > sz && !readMore(2))
        ಪ್ರತ್ಯಾಗಮ (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    ಪ್ರತ್ಯಾಗಮ val;
}

ಪೂರ್ಣಾಙ್ಕ FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    ಯದಿ (ofs + 4 > sz && !readMore(4))
        ಪ್ರತ್ಯಾಗಮ 0;
    ಪೂರ್ಣಾಙ್ಕ ival = readInt(&decoded[ofs]);
    ofs += 4;
    ಪ್ರತ್ಯಾಗಮ ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    ಯದಿ (ofs + 8 > sz && !readMore(8))
        ಪ್ರತ್ಯಾಗಮ 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    ಪ್ರತ್ಯಾಗಮ ival;
}

ದ್ವಿಗುಣ FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    ಯದಿ (ofs + 8 > sz && !readMore(8))
        ಪ್ರತ್ಯಾಗಮ 0;
    ದ್ವಿಗುಣ fval = readReal(&decoded[ofs]);
    ofs += 8;
    ಪ್ರತ್ಯಾಗಮ fval;
}

ತಾರ್ಕಿಕ FileStorage::Impl::Base64Decoder::endOfStream() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ eos; }

ವರ್ಣ *FileStorage::Impl::Base64Decoder::getPtr() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ptr; }


ವರ್ಣ *FileStorage::Impl::parseBase64(ವರ್ಣ *ptr, ಪೂರ್ಣಾಙ್ಕ indent, File_ಗಂಟು &collection) {
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ BASE64_HDR_SIZE = 24;
    ವರ್ಣ dt[BASE64_HDR_SIZE + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    ಪೂರ್ಣಾಙ್ಕ i, k;

    ಪ್ರತಿ (i = 0; i < BASE64_HDR_SIZE; i++)
        dt[i] = (ವರ್ಣ) base64decoder.getUInt8();
    ಪ್ರತಿ (i = 0; i < BASE64_HDR_SIZE; i++)
        ಯದಿ (isspace(dt[i]))
            ಭಙ್ಗ;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    ಪೂರ್ಣಾಙ್ಕ fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    ಪೂರ್ಣಾಙ್ಕ fmt_pair_ಎಣಿಕೆ = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    ದ್ವಿಗುಣ fval = 0;

    ಪ್ರತಿ (;;) {
        ಪ್ರತಿ (k = 0; k < fmt_pair_ಎಣಿಕೆ; k++) {
            ಪೂರ್ಣಾಙ್ಕ elem_type = fmt_pairs[k * 2 + 1];
            ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2 = fmt_pairs[k * 2];

            ಪ್ರತಿ (i = 0; i < ಎಣಿಕೆ_2; i++) {
                ಪೂರ್ಣಾಙ್ಕ ಗಂಟು_type = File_ಗಂಟು::INT;
                ವಿಕಲ್ಪ (elem_type) {
                    ಪಕ್ಷ CV_8U:
                        ival = base64decoder.getUInt8();
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_8S:
                        ival = (ವರ್ಣ) base64decoder.getUInt8();
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_16U:
                        ival = base64decoder.getUInt16();
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_16S:
                        ival = (short) base64decoder.getUInt16();
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_32U:
                        ival = base64decoder.getInt32();
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_32S:
                        ival = base64decoder.getInt32();
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_64U:
                        ival = base64decoder.getInt64();
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_64S:
                        ival = base64decoder.getInt64();
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        ಗಂಟು_type = File_ಗಂಟು::REAL;
                    }
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_64F:
                        fval = base64decoder.getFloat64();
                        ಗಂಟು_type = File_ಗಂಟು::REAL;
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_16F:
                        fval = ಭಿನ್ನ(hfloatFromBits(base64decoder.getUInt16()));
                        ಗಂಟು_type = File_ಗಂಟು::REAL;
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        ಗಂಟು_type = File_ಗಂಟು::REAL;
                        ಭಙ್ಗ;
                    ಪಕ್ಷ CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        ಗಂಟು_type = File_ಗಂಟು::REAL;
                        ಭಙ್ಗ;
                    ಸ್ವಭಾವ:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                ಯದಿ (base64decoder.endOfStream())
                    ಭಙ್ಗ;
                add_ಗಂಟು(collection, std::string(), ಗಂಟು_type,
                        ಗಂಟು_type == File_ಗಂಟು::INT ? (ಶೂನ್ಯ *) &ival : (ಶೂನ್ಯ *) &fval, -1);
            }
        }
        ಯದಿ (base64decoder.endOfStream())
            ಭಙ್ಗ;
    }

    finalizeCollection(collection);
    ಪ್ರತ್ಯಾಗಮ base64decoder.getPtr();
}

ಶೂನ್ಯ FileStorage::Impl::parseError(ಧ್ರುವ ವರ್ಣ *func_name, ಧ್ರುವ std::string &err_ಸಂದೇಶ, ಧ್ರುವ ವರ್ಣ *source_file,
                                   ಪೂರ್ಣಾಙ್ಕ source_ಸಾಲು) {
    std::string ಸಂದೇಶ = format("%s(%d): %s", filename.c_str(), lineno, err_ಸಂದೇಶ.c_str());
    error(Error::StsParseError, func_name, ಸಂದೇಶ.c_str(), source_file, source_ಸಾಲು);
}

ಧ್ರುವ uchar *FileStorage::Impl::get_ಗಂಟು_Ptr(size_t blockIdx, size_t ofs) ಧ್ರುವ {
    CV_Assert(blockIdx < fs_data_ptrs.size());
    CV_Assert(ofs < fs_data_blksz[blockIdx]);

    ಪ್ರತ್ಯಾಗಮ fs_data_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) ಧ್ರುವ {
    CV_Assert(nameofs < str_hash_data.size());
    ಪ್ರತ್ಯಾಗಮ std::string(&str_hash_data[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { ಪ್ರತ್ಯಾಗಮ fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(ಏತತ್);
}

FileStorage::FileStorage(ಧ್ರುವ String& filename, ಪೂರ್ಣಾಙ್ಕ flags, ಧ್ರುವ String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(ಏತತ್);
    ತಾರ್ಕಿಕ ok = p->open(filename.c_str(), flags, encoding.c_str());
    ಯದಿ(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

ಶೂನ್ಯ FileStorage::startWriteStruct(ಧ್ರುವ String& name, ಪೂರ್ಣಾಙ್ಕ struct_flags, ಧ್ರುವ String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    ಯದಿ ((struct_flags & File_ಗಂಟು::TYPE_MASK) == File_ಗಂಟು::SEQ)
        state = FileStorage::ಮೌಲ್ಯ_EXPECTED;
    ಅನ್ಯಥಾ
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

ಶೂನ್ಯ FileStorage::endWriteStruct()
{
    p->endWriteStruct();
    state = p->write_ರಾಶಿ.empty() || File_ಗಂಟು::isMap(p->write_ರಾಶಿ.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::ಮೌಲ್ಯ_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

ತಾರ್ಕಿಕ FileStorage::open(ಧ್ರುವ String& filename, ಪೂರ್ಣಾಙ್ಕ flags, ಧ್ರುವ String& encoding)
{
    ಯತ್ನ
    {
        ತಾರ್ಕಿಕ ok = p->open(filename.c_str(), flags, encoding.c_str());
        ಯದಿ(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        ಪ್ರತ್ಯಾಗಮ ok;
    }
    ಗೃಹಾಣ (...)
    {
        release();
        ಕ್ಷಿಪ;  // re-throw
    }
}

ತಾರ್ಕಿಕ FileStorage::isOpened() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ p->is_opened; }

ಶೂನ್ಯ FileStorage::release()
{
    p->release();
}

File_ಗಂಟು FileStorage::ಬೇರು(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಯದಿ( p.empty() || p->roots.empty() || i < 0 || i >= (ಪೂರ್ಣಾಙ್ಕ)p->roots.size() )
        ಪ್ರತ್ಯಾಗಮ File_ಗಂಟು();

    ಪ್ರತ್ಯಾಗಮ p->roots[i];
}

File_ಗಂಟು FileStorage::get_First_ತುದಿ_Level_ಗಂಟು() ಧ್ರುವ
{
    File_ಗಂಟು r = ಬೇರು();
    File_ಗಂಟು_Iterator it = r.begin();
    ಪ್ರತ್ಯಾಗಮ it != r.end() ? *it : File_ಗಂಟು();
}

std::string FileStorage::getDefaultObjectName(ಧ್ರುವ std::string& _filename)
{
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* stubname = "unnamed";
    ಧ್ರುವ ವರ್ಣ* filename = _filename.c_str();
    ಧ್ರುವ ವರ್ಣ* ptr2 = filename + _filename.size();
    ಧ್ರುವ ವರ್ಣ* ptr = ptr2 - 1;
    cv::AutoBuffer<ವರ್ಣ> name_buf(_filename.size()+1);

    ಯಾವತ್( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        ಯದಿ( *ptr == '.' && (!*ptr2 || strncmp(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    ಯದಿ( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    ವರ್ಣ* name = name_buf.data();

    // name must start with letter or '_'
    ಯದಿ( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    ಯಾವತ್( ptr < ptr2 )
    {
        ವರ್ಣ c = *ptr++;
        ಯದಿ( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.data();
    ಯದಿ( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    ಪ್ರತ್ಯಾಗಮ name;
}


ಪೂರ್ಣಾಙ್ಕ FileStorage::getFormat() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ p->fmt;
}

File_ಗಂಟು FileStorage::operator [](ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ ಏತತ್->operator[](std::string(ಕೀಲಿ));
}

File_ಗಂಟು FileStorage::operator [](ಧ್ರುವ std::string& ಕೀಲಿ) ಧ್ರುವ
{
    File_ಗಂಟು res;
    ಪ್ರತಿ (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][ಕೀಲಿ];
        ಯದಿ (!res.empty())
            ಭಙ್ಗ;
    }
    ಪ್ರತ್ಯಾಗಮ res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    ಪ್ರತ್ಯಾಗಮ buf;
}

ಶೂನ್ಯ FileStorage::writeRaw( ಧ್ರುವ String& fmt, ಧ್ರುವ ಶೂನ್ಯ* vec, size_t len )
{
    p->writeRawData(fmt, (ಧ್ರುವ uchar*)vec, len);
}

ಶೂನ್ಯ FileStorage::writeComment( ಧ್ರುವ String& comment, ತಾರ್ಕಿಕ eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

ಶೂನ್ಯ writeScalar( FileStorage& fs, ಪೂರ್ಣಾಙ್ಕ value )
{
    fs.p->ಲಿಖ(String(), value);
}

ಶೂನ್ಯ writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->ಲಿಖ(String(), value);
}

ಶೂನ್ಯ writeScalar( FileStorage& fs, ಭಿನ್ನ value )
{
    fs.p->ಲಿಖ(String(), (ದ್ವಿಗುಣ)value);
}

ಶೂನ್ಯ writeScalar( FileStorage& fs, ದ್ವಿಗುಣ value )
{
    fs.p->ಲಿಖ(String(), value);
}

ಶೂನ್ಯ writeScalar( FileStorage& fs, ಧ್ರುವ String& value )
{
    fs.p->ಲಿಖ(String(), value);
}

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಧ್ರುವ String& name, ಪೂರ್ಣಾಙ್ಕ value )
{
    fs.p->ಲಿಖ(name, value);
}

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಧ್ರುವ String& name, int64_t value )
{
    fs.p->ಲಿಖ(name, value);
}

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಧ್ರುವ String& name, ಭಿನ್ನ value )
{
    fs.p->ಲಿಖ(name, (ದ್ವಿಗುಣ)value);
}

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಧ್ರುವ String& name, ದ್ವಿಗುಣ value )
{
    fs.p->ಲಿಖ(name, value);
}

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಧ್ರುವ String& name, ಧ್ರುವ String& value )
{
    fs.p->ಲಿಖ(name, value);
}

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಧ್ರುವ String& name, ತಾರ್ಕಿಕ value )
{
    fs.p->ಲಿಖ(name, value);
}

ಶೂನ್ಯ FileStorage::ಲಿಖ(ಧ್ರುವ String& name, ಪೂರ್ಣಾಙ್ಕ val) { p->ಲಿಖ(name, val); }
ಶೂನ್ಯ FileStorage::ಲಿಖ(ಧ್ರುವ String& name, ತಾರ್ಕಿಕ val) { p->ಲಿಖ(name, val); }
ಶೂನ್ಯ FileStorage::ಲಿಖ(ಧ್ರುವ String& name, int64_t val) { p->ಲಿಖ(name, val); }
ಶೂನ್ಯ FileStorage::ಲಿಖ(ಧ್ರುವ String& name, ದ್ವಿಗುಣ val) { p->ಲಿಖ(name, val); }
ಶೂನ್ಯ FileStorage::ಲಿಖ(ಧ್ರುವ String& name, ಧ್ರುವ String& val) { p->ಲಿಖ(name, val); }
ಶೂನ್ಯ FileStorage::ಲಿಖ(ಧ್ರುವ String& name, ಧ್ರುವ Mat& val) { cv::ಲಿಖ(*ಏತತ್, name, val); }
ಶೂನ್ಯ FileStorage::ಲಿಖ(ಧ್ರುವ String& name, ಧ್ರುವ std::vector<String>& val) { cv::ಲಿಖ(*ಏತತ್, name, val); }

FileStorage& operator << (FileStorage& fs, ಧ್ರುವ String& str)
{
    ಗಣನಾ { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        ಮೌಲ್ಯ_EXPECTED = FileStorage::ಮೌಲ್ಯ_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    ಧ್ರುವ ವರ್ಣ* _str = str.c_str();
    ಯದಿ( !fs.isOpened() || !_str )
        ಪ್ರತ್ಯಾಗಮ fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    ವರ್ಣ c = *_str;

    ಯದಿ( c == '}' || c == ']' )
    {
        ಯದಿ( fs_impl->write_ರಾಶಿ.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        ಪೂರ್ಣಾಙ್ಕ struct_flags = fs_impl->write_ರಾಶಿ.back().flags;
        ವರ್ಣ expected_bracket = File_ಗಂಟು::isMap(struct_flags) ? '}' : ']';
        ಯದಿ( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_ರಾಶಿ.empty());
        struct_flags = fs_impl->write_ರಾಶಿ.back().flags;
        fs.state = File_ಗಂಟು::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : ಮೌಲ್ಯ_EXPECTED;
        fs.elname = String();
    }
    ಅನ್ಯಥಾ ಯದಿ( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        ಯದಿ (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = ಮೌಲ್ಯ_EXPECTED + INSIDE_MAP;
    }
    ಅನ್ಯಥಾ ಯದಿ( (fs.state & 3) == ಮೌಲ್ಯ_EXPECTED )
    {
        ಯದಿ( c == '{' || c == '[' )
        {
            ಪೂರ್ಣಾಙ್ಕ struct_flags = c == '{' ? File_ಗಂಟು::MAP : File_ಗಂಟು::SEQ;
            fs.state = struct_flags == File_ಗಂಟು::MAP ? INSIDE_MAP + NAME_EXPECTED : ಮೌಲ್ಯ_EXPECTED;
            _str++;
            ಯದಿ( *_str == ':' )
            {
                _str++;
                ಯದಿ( !*_str )
                    struct_flags |= File_ಗಂಟು::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        ಅನ್ಯಥಾ
        {
            ಲಿಖ( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            ಯದಿ( fs.state == INSIDE_MAP + ಮೌಲ್ಯ_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    ಅನ್ಯಥಾ
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    ಪ್ರತ್ಯಾಗಮ fs;
}


File_ಗಂಟು::File_ಗಂಟು()
    : fs(ರಿಕ್ತ)
{
    blockIdx = ofs = 0;
}

File_ಗಂಟು::File_ಗಂಟು(FileStorage::Impl* _fs, size_t _blockIdx, size_t _ofs)
    : fs(_fs)
{
    blockIdx = _blockIdx;
    ofs = _ofs;
}

File_ಗಂಟು::File_ಗಂಟು(ಧ್ರುವ FileStorage* _fs, size_t _blockIdx, size_t _ofs)
    : File_ಗಂಟು(_fs->p.get(), _blockIdx, _ofs)
{
    // nothing
}

File_ಗಂಟು::File_ಗಂಟು(ಧ್ರುವ File_ಗಂಟು& ಗಂಟು)
{
    fs = ಗಂಟು.fs;
    blockIdx = ಗಂಟು.blockIdx;
    ofs = ಗಂಟು.ofs;
}

File_ಗಂಟು& File_ಗಂಟು::operator=(ಧ್ರುವ File_ಗಂಟು& ಗಂಟು)
{
    fs = ಗಂಟು.fs;
    blockIdx = ಗಂಟು.blockIdx;
    ofs = ಗಂಟು.ofs;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

File_ಗಂಟು File_ಗಂಟು::operator[](ಧ್ರುವ std::string& nodename) ಧ್ರುವ
{
    ಯದಿ(!fs)
        ಪ್ರತ್ಯಾಗಮ File_ಗಂಟು();

    CV_Assert( isMap() );

    unsigned ಕೀಲಿ = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_ಗಂಟು_Iterator it = begin();

    ಪ್ರತಿ( i = 0; i < sz; i++, ++it )
    {
        File_ಗಂಟು n = *it;
        ಧ್ರುವ uchar* p = n.ptr();
        unsigned key2 = (unsigned)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_data.size() );
        ಯದಿ( ಕೀಲಿ == key2 )
            ಪ್ರತ್ಯಾಗಮ n;
    }
    ಪ್ರತ್ಯಾಗಮ File_ಗಂಟು();
}

File_ಗಂಟು File_ಗಂಟು::operator[](ಧ್ರುವ ವರ್ಣ* nodename) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ ಏತತ್->operator[](std::string(nodename));
}

File_ಗಂಟು File_ಗಂಟು::operator[](ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಯದಿ(!fs)
        ಪ್ರತ್ಯಾಗಮ File_ಗಂಟು();

    CV_Assert( isSeq() );

    ಪೂರ್ಣಾಙ್ಕ sz = (ಪೂರ್ಣಾಙ್ಕ)size();
    CV_Assert( 0 <= i && i < sz );

    File_ಗಂಟು_Iterator it = begin();
    it += i;

    ಪ್ರತ್ಯಾಗಮ *it;
}

std::vector<String> File_ಗಂಟು::ಕೀಲಿಗಳು() ಧ್ರುವ
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    ಪ್ರತಿ (File_ಗಂಟು_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    ಪ್ರತ್ಯಾಗಮ res;
}

ಪೂರ್ಣಾಙ್ಕ File_ಗಂಟು::type() ಧ್ರುವ
{
    ಧ್ರುವ uchar* p = ptr();
    ಯದಿ(!p)
        ಪ್ರತ್ಯಾಗಮ NONE;
    ಪ್ರತ್ಯಾಗಮ (*p & TYPE_MASK);
}

ತಾರ್ಕಿಕ File_ಗಂಟು::isMap(ಪೂರ್ಣಾಙ್ಕ flags) { ಪ್ರತ್ಯಾಗಮ (flags & TYPE_MASK) == MAP; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isSeq(ಪೂರ್ಣಾಙ್ಕ flags) { ಪ್ರತ್ಯಾಗಮ (flags & TYPE_MASK) == SEQ; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isCollection(ಪೂರ್ಣಾಙ್ಕ flags) { ಪ್ರತ್ಯಾಗಮ isMap(flags) || isSeq(flags); }
ತಾರ್ಕಿಕ File_ಗಂಟು::isFlow(ಪೂರ್ಣಾಙ್ಕ flags) { ಪ್ರತ್ಯಾಗಮ (flags & FLOW) != 0; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isEmptyCollection(ಪೂರ್ಣಾಙ್ಕ flags) { ಪ್ರತ್ಯಾಗಮ (flags & EMPTY) != 0; }

ತಾರ್ಕಿಕ File_ಗಂಟು::empty() ಧ್ರುವ   { ಪ್ರತ್ಯಾಗಮ fs == 0; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isNone() ಧ್ರುವ  { ಪ್ರತ್ಯಾಗಮ type() == NONE; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isSeq() ಧ್ರುವ   { ಪ್ರತ್ಯಾಗಮ type() == SEQ; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isMap() ಧ್ರುವ   { ಪ್ರತ್ಯಾಗಮ type() == MAP; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isInt() ಧ್ರುವ   { ಪ್ರತ್ಯಾಗಮ type() == INT;  }
ತಾರ್ಕಿಕ File_ಗಂಟು::isReal() ಧ್ರುವ  { ಪ್ರತ್ಯಾಗಮ type() == REAL; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isString() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ type() == STRING;  }
ತಾರ್ಕಿಕ File_ಗಂಟು::isNamed() ಧ್ರುವ
{
    ಧ್ರುವ uchar* p = ptr();
    ಯದಿ(!p)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ (*p & NAMED) != 0;
}

std::string File_ಗಂಟು::name() ಧ್ರುವ
{
    ಧ್ರುವ uchar* p = ptr();
    ಯದಿ(!p)
        ಪ್ರತ್ಯಾಗಮ std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    ಪ್ರತ್ಯಾಗಮ fs->getName(nameofs);
}

File_ಗಂಟು::operator ಪೂರ್ಣಾಙ್ಕ() ಧ್ರುವ
{
    ಧ್ರುವ uchar* p = ptr();
    ಯದಿ(!p)
        ಪ್ರತ್ಯಾಗಮ 0;
    ಪೂರ್ಣಾಙ್ಕ tag = *p;
    ಪೂರ್ಣಾಙ್ಕ type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    ಯದಿ( type == INT )
    {
        ಪ್ರತ್ಯಾಗಮ readInt(p);
    }
    ಅನ್ಯಥಾ ಯದಿ( type == REAL )
    {
        ಪ್ರತ್ಯಾಗಮ cvRound(readReal(p));
    }
    ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ 0x7fffffff;
}

File_ಗಂಟು::operator int64_t() ಧ್ರುವ
{
    ಧ್ರುವ uchar* p = ptr();
    ಯದಿ(!p)
        ಪ್ರತ್ಯಾಗಮ 0;
    ಪೂರ್ಣಾಙ್ಕ tag = *p;
    ಪೂರ್ಣಾಙ್ಕ type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    ಯದಿ( type == INT )
    {
        ಪ್ರತ್ಯಾಗಮ readLong(p);
    }
    ಅನ್ಯಥಾ ಯದಿ( type == REAL )
    {
        ಪ್ರತ್ಯಾಗಮ cvRound(readReal(p));
    }
    ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ 0x7fffffff;
}

File_ಗಂಟು::operator ಭಿನ್ನ() ಧ್ರುವ
{
    ಧ್ರುವ uchar* p = ptr();
    ಯದಿ(!p)
        ಪ್ರತ್ಯಾಗಮ 0.f;
    ಪೂರ್ಣಾಙ್ಕ tag = *p;
    ಪೂರ್ಣಾಙ್ಕ type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    ಯದಿ( type == INT )
    {
        ಪ್ರತ್ಯಾಗಮ (ಭಿನ್ನ)readLong(p);
    }
    ಅನ್ಯಥಾ ಯದಿ( type == REAL )
    {
        ಪ್ರತ್ಯಾಗಮ (ಭಿನ್ನ)readReal(p);
    }
    ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ FLT_MAX;
}

File_ಗಂಟು::operator ದ್ವಿಗುಣ() ಧ್ರುವ
{
    ಧ್ರುವ uchar* p = ptr();
    ಯದಿ(!p)
        ಪ್ರತ್ಯಾಗಮ 0.f;
    ಪೂರ್ಣಾಙ್ಕ tag = *p;
    ಪೂರ್ಣಾಙ್ಕ type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    ಯದಿ( type == INT )
    {
        ಪ್ರತ್ಯಾಗಮ (ದ್ವಿಗುಣ)readLong(p);
    }
    ಅನ್ಯಥಾ ಯದಿ( type == REAL )
    {
        ಪ್ರತ್ಯಾಗಮ readReal(p);
    }
    ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ DBL_MAX;
}

ದ್ವಿಗುಣ File_ಗಂಟು::real() ಧ್ರುವ  { ಪ್ರತ್ಯಾಗಮ ದ್ವಿಗುಣ(*ಏತತ್); }

std::string File_ಗಂಟು::string() ಧ್ರುವ
{
    ಧ್ರುವ uchar* p = ptr();
    ಯದಿ( !p || (*p & TYPE_MASK) != STRING )
        ಪ್ರತ್ಯಾಗಮ std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(unsigned)readInt(p);
    ಪ್ರತ್ಯಾಗಮ std::string((ಧ್ರುವ ವರ್ಣ*)(p + 4), sz - 1);
}
Mat File_ಗಂಟು::mat() ಧ್ರುವ { Mat value; ಪಠ(*ಏತತ್, value, Mat()); ಪ್ರತ್ಯಾಗಮ value; }

File_ಗಂಟು_Iterator File_ಗಂಟು::begin() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ File_ಗಂಟು_Iterator(*ಏತತ್, ಅಸತ್ಯ); }
File_ಗಂಟು_Iterator File_ಗಂಟು::end() ಧ್ರುವ   { ಪ್ರತ್ಯಾಗಮ File_ಗಂಟು_Iterator(*ಏತತ್, ಸತ್ಯ); }

ಶೂನ್ಯ File_ಗಂಟು::readRaw( ಧ್ರುವ std::string& fmt, ಶೂನ್ಯ* vec, size_t len ) ಧ್ರುವ
{
    File_ಗಂಟು_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_ಗಂಟು::size() ಧ್ರುವ
{
    ಧ್ರುವ uchar* p = ptr();
    ಯದಿ( !p )
        ಪ್ರತ್ಯಾಗಮ 0;
    ಪೂರ್ಣಾಙ್ಕ tag = *p;
    ಪೂರ್ಣಾಙ್ಕ tp = tag & TYPE_MASK;
    ಯದಿ( tp == MAP || tp == SEQ )
    {
        ಯದಿ( tag & NAMED )
            p += 4;
        ಪ್ರತ್ಯಾಗಮ (size_t)(unsigned)readInt(p + 5);
    }
    ಪ್ರತ್ಯಾಗಮ tp != NONE;
}

size_t File_ಗಂಟು::rawSize() ಧ್ರುವ
{
    ಧ್ರುವ uchar* p0 = ptr(), *p = p0;
    ಯದಿ( !p )
        ಪ್ರತ್ಯಾಗಮ 0;
    ಪೂರ್ಣಾಙ್ಕ tag = *p++;
    ಪೂರ್ಣಾಙ್ಕ tp = tag & TYPE_MASK;
    ಯದಿ( tag & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    ಯದಿ( tp == INT )
        ಪ್ರತ್ಯಾಗಮ sz0 + 8;
    ಯದಿ( tp == REAL )
        ಪ್ರತ್ಯಾಗಮ sz0 + 8;
    ಯದಿ( tp == NONE )
        ಪ್ರತ್ಯಾಗಮ sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    ಪ್ರತ್ಯಾಗಮ sz0 + 4 + readInt(p);
}

uchar* File_ಗಂಟು::ptr()
{
    ಪ್ರತ್ಯಾಗಮ !fs ? 0 : (uchar*)fs->get_ಗಂಟು_Ptr(blockIdx, ofs);
}

ಧ್ರುವ uchar* File_ಗಂಟು::ptr() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ !fs ? 0 : fs->get_ಗಂಟು_Ptr(blockIdx, ofs);
}

ಶೂನ್ಯ File_ಗಂಟು::set_ಮೌಲ್ಯ( ಪೂರ್ಣಾಙ್ಕ type, ಧ್ರುವ ಶೂನ್ಯ* value, ಪೂರ್ಣಾಙ್ಕ len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    ಪೂರ್ಣಾಙ್ಕ tag = *p;
    ಪೂರ್ಣಾಙ್ಕ current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    ಪೂರ್ಣಾಙ್ಕ sz = 1;

    ಯದಿ( tag & NAMED )
        sz += 4;

    ಯದಿ( type == INT )
        sz += 8;
    ಅನ್ಯಥಾ ಯದಿ( type == REAL )
        sz += 8;
    ಅನ್ಯಥಾ ಯದಿ( type == STRING )
    {
        ಯದಿ( len < 0 )
            len = (ಪೂರ್ಣಾಙ್ಕ)strlen((ಧ್ರುವ ವರ್ಣ*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_ಗಂಟು_Space(*ಏತತ್, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    ಯದಿ( tag & NAMED )
        p += 4;

    ಯದಿ( type == INT )
    {
        int64_t ival = *(ಧ್ರುವ int64_t*)value;
        writeInt(p, ival);
    }
    ಅನ್ಯಥಾ ಯದಿ( type == REAL )
    {
        ದ್ವಿಗುಣ dbval = *(ಧ್ರುವ ದ್ವಿಗುಣ*)value;
        writeReal(p, dbval);
    }
    ಅನ್ಯಥಾ ಯದಿ( type == STRING )
    {
        ಧ್ರುವ ವರ್ಣ* str = (ಧ್ರುವ ವರ್ಣ*)value;
        writeInt(p, len + 1);
        memcpy(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_ಗಂಟು_Iterator::File_ಗಂಟು_Iterator()
{
    fs = 0;
    blockIdx = 0;
    ofs = 0;
    blockSize = 0;
    ಗಂಟು_NElems = 0;
    idx = 0;
}

File_ಗಂಟು_Iterator::File_ಗಂಟು_Iterator( ಧ್ರುವ File_ಗಂಟು& ಗಂಟು, ತಾರ್ಕಿಕ seekEnd )
{
    fs = ಗಂಟು.fs;
    idx = 0;
    ಯದಿ( !fs )
        blockIdx = ofs = blockSize = ಗಂಟು_NElems = 0;
    ಅನ್ಯಥಾ
    {
        blockIdx = ಗಂಟು.blockIdx;
        ofs = ಗಂಟು.ofs;

        ತಾರ್ಕಿಕ collection = ಗಂಟು.isSeq() || ಗಂಟು.isMap();
        ಯದಿ( ಗಂಟು.isNone() )
        {
            ಗಂಟು_NElems = 0;
        }
        ಅನ್ಯಥಾ ಯದಿ( !collection )
        {
            ಗಂಟು_NElems = 1;
            ಯದಿ( seekEnd )
            {
                idx = 1;
                ofs += ಗಂಟು.rawSize();
            }
        }
        ಅನ್ಯಥಾ
        {
            ಗಂಟು_NElems = ಗಂಟು.size();
            ಧ್ರುವ uchar* p0 = ಗಂಟು.ptr(), *p = p0 + 1;
            ಯದಿ(*p0 & File_ಗಂಟು::NAMED )
                p += 4;
            ಯದಿ( !seekEnd )
                ofs += (p - p0) + 8;
            ಅನ್ಯಥಾ
            {
                size_t rawsz = (size_t)(unsigned)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = ಗಂಟು_NElems;
            }
        }
        fs->normalize_ಗಂಟು_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
}

File_ಗಂಟು_Iterator::File_ಗಂಟು_Iterator(ಧ್ರುವ File_ಗಂಟು_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    ಗಂಟು_NElems = it.ಗಂಟು_NElems;
    idx = it.idx;
}

File_ಗಂಟು_Iterator& File_ಗಂಟು_Iterator::operator=(ಧ್ರುವ File_ಗಂಟು_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    ಗಂಟು_NElems = it.ಗಂಟು_NElems;
    idx = it.idx;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

File_ಗಂಟು File_ಗಂಟು_Iterator::operator *() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ File_ಗಂಟು(idx < ಗಂಟು_NElems ? fs : ರಿಕ್ತ, blockIdx, ofs);
}

File_ಗಂಟು_Iterator& File_ಗಂಟು_Iterator::operator ++ ()
{
    ಯದಿ( idx == ಗಂಟು_NElems || !fs )
        ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
    idx++;
    File_ಗಂಟು n(fs, blockIdx, ofs);
    ofs += n.rawSize();
    ಯದಿ( ofs >= blockSize )
    {
        fs->normalize_ಗಂಟು_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

File_ಗಂಟು_Iterator File_ಗಂಟು_Iterator::operator ++ (ಪೂರ್ಣಾಙ್ಕ)
{
    File_ಗಂಟು_Iterator it = *ಏತತ್;
    ++(*ಏತತ್);
    ಪ್ರತ್ಯಾಗಮ it;
}

File_ಗಂಟು_Iterator& File_ಗಂಟು_Iterator::operator += (ಪೂರ್ಣಾಙ್ಕ _ofs)
{
    CV_Assert( _ofs >= 0 );
    ಪ್ರತಿ( ; _ofs > 0; _ofs-- )
        ಏತತ್->operator ++();
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

File_ಗಂಟು_Iterator& File_ಗಂಟು_Iterator::readRaw( ಧ್ರುವ String& fmt, ಶೂನ್ಯ* _data0, size_t maxsz)
{
    ಯದಿ( fs && idx < ಗಂಟು_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        ಪೂರ್ಣಾಙ್ಕ fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        ಪೂರ್ಣಾಙ್ಕ fmt_pair_ಎಣಿಕೆ = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        size_t esz = fs::calcStructSize( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        ಪ್ರತಿ( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ k = 0; k < fmt_pair_ಎಣಿಕೆ; k++ )
            {
                ಪೂರ್ಣಾಙ್ಕ elem_type = fmt_pairs[k*2+1];
                ಪೂರ್ಣಾಙ್ಕ elem_size = CV_ELEM_SIZE(elem_type);

                ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2 = fmt_pairs[k*2];
                offset = alignSize( offset, elem_size );
                uchar* data = data0 + offset;

                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ_2; i++, ++(*ಏತತ್) )
                {
                    File_ಗಂಟು ಗಂಟು = *(*ಏತತ್);
                    ಯದಿ( ಗಂಟು.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_size == 8 ? (int64_t)ಗಂಟು : (ಪೂರ್ಣಾಙ್ಕ)ಗಂಟು);
                        ವಿಕಲ್ಪ( elem_type )
                        {
                        ಪಕ್ಷ CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(ival);
                            data++;
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_8S:
                            *(ವರ್ಣ*)data = saturate_cast<schar>(ival);
                            data++;
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_Bool:
                            *(ತಾರ್ಕಿಕ*)data = ival != 0;
                            data++;
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(ival);
                            data += ಪರಿಮಾಣ(ushort);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_16S:
                            *(short*)data = saturate_cast<short>(ival);
                            data += ಪರಿಮಾಣ(short);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_32U:
                            *(unsigned*)data = (unsigned)std::max(ival, (int64_t)0);
                            data += ಪರಿಮಾಣ(unsigned);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_32S:
                            *(ಪೂರ್ಣಾಙ್ಕ*)data = (ಪೂರ್ಣಾಙ್ಕ)ival;
                            data += ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_32F:
                            *(ಭಿನ್ನ*)data = (ಭಿನ್ನ)ival;
                            data += ಪರಿಮಾಣ(ಭಿನ್ನ);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_64U:
                            *(uint64_t*)data = (uint64_t)ival;
                            data += ಪರಿಮಾಣ(uint64_t);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_64S:
                            *(int64_t*)data = (int64_t)ival;
                            data += ಪರಿಮಾಣ(int64_t);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_64F:
                            *(ದ್ವಿಗುಣ*)data = (ದ್ವಿಗುಣ)ival;
                            data += ಪರಿಮಾಣ(ದ್ವಿಗುಣ);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_16F:
                            *(hfloat*)data = hfloat((ಭಿನ್ನ)ival);
                            data += ಪರಿಮಾಣ(hfloat);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_16BF:
                            *(bfloat*)data = bfloat((ಭಿನ್ನ)ival);
                            data += ಪರಿಮಾಣ(bfloat);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((ಭಿನ್ನ)ival);
                            data += ಪರಿಮಾಣ(fp8_t);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((ಭಿನ್ನ)ival);
                            data += ಪರಿಮಾಣ(fp8a_t);
                            ಭಙ್ಗ;
                        ಸ್ವಭಾವ:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    ಅನ್ಯಥಾ ಯದಿ( ಗಂಟು.isReal() )
                    {
                        ದ್ವಿಗುಣ fval = (ದ್ವಿಗುಣ)ಗಂಟು;

                        ವಿಕಲ್ಪ( elem_type )
                        {
                        ಪಕ್ಷ CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(fval);
                            data++;
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_8S:
                            *(ವರ್ಣ*)data = saturate_cast<schar>(fval);
                            data++;
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(fval);
                            data += ಪರಿಮಾಣ(ushort);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_16S:
                            *(short*)data = saturate_cast<short>(fval);
                            data += ಪರಿಮಾಣ(short);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_32U:
                            *(ಪೂರ್ಣಾಙ್ಕ*)data = saturate_cast<unsigned>(fval);
                            data += ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_32S:
                            *(ಪೂರ್ಣಾಙ್ಕ*)data = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(fval);
                            data += ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_32F:
                            *(ಭಿನ್ನ*)data = (ಭಿನ್ನ)fval;
                            data += ಪರಿಮಾಣ(ಭಿನ್ನ);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_64U:
                            *(uint64_t*)data = (uint64_t)round(std::max(fval, 0.));
                            data += ಪರಿಮಾಣ(uint64_t);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_64S:
                            *(int64_t*)data = (int64_t)round(std::max(fval, 0.));
                            data += ಪರಿಮಾಣ(int64_t);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_64F:
                            *(ದ್ವಿಗುಣ*)data = fval;
                            data += ಪರಿಮಾಣ(ದ್ವಿಗುಣ);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_16F:
                            *(hfloat*)data = hfloat((ಭಿನ್ನ)fval);
                            data += ಪರಿಮಾಣ(hfloat);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_16BF:
                            *(bfloat*)data = bfloat((ಭಿನ್ನ)fval);
                            data += ಪರಿಮಾಣ(bfloat);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((ಭಿನ್ನ)fval);
                            data += ಪರಿಮಾಣ(fp8_t);
                            ಭಙ್ಗ;
                        ಪಕ್ಷ CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((ಭಿನ್ನ)fval);
                            data += ಪರಿಮಾಣ(fp8a_t);
                            ಭಙ್ಗ;
                        ಸ್ವಭಾವ:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    ಅನ್ಯಥಾ
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (ಪೂರ್ಣಾಙ್ಕ)(data - data0);
            }
        }
    }

    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

ತಾರ್ಕಿಕ File_ಗಂಟು_Iterator::equalTo(ಧ್ರುವ File_ಗಂಟು_Iterator& it) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && ಗಂಟು_NElems == it.ಗಂಟು_NElems;
}

size_t File_ಗಂಟು_Iterator::remaining() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ ಗಂಟು_NElems - idx;
}

ತಾರ್ಕಿಕ operator == ( ಧ್ರುವ File_ಗಂಟು_Iterator& it1, ಧ್ರುವ File_ಗಂಟು_Iterator& it2 )
{
    ಪ್ರತ್ಯಾಗಮ it1.equalTo(it2);
}

ತಾರ್ಕಿಕ operator != ( ಧ್ರುವ File_ಗಂಟು_Iterator& it1, ಧ್ರುವ File_ಗಂಟು_Iterator& it2 )
{
    ಪ್ರತ್ಯಾಗಮ !it1.equalTo(it2);
}

ಶೂನ್ಯ ಪಠ(ಧ್ರುವ File_ಗಂಟು& ಗಂಟು, ಪೂರ್ಣಾಙ್ಕ& val, ಪೂರ್ಣಾಙ್ಕ default_val)
{
    val = default_val;
    ಯದಿ( !ಗಂಟು.empty() )
    {
        val = (ಪೂರ್ಣಾಙ್ಕ)ಗಂಟು;
    }
}

ಶೂನ್ಯ ಪಠ(ಧ್ರುವ File_ಗಂಟು& ಗಂಟು, int64_t& val, int64_t default_val)
{
    val = default_val;
    ಯದಿ( !ಗಂಟು.empty() )
    {
        val = (int64_t)ಗಂಟು;
    }
}

ಶೂನ್ಯ ಪಠ(ಧ್ರುವ File_ಗಂಟು& ಗಂಟು, ದ್ವಿಗುಣ& val, ದ್ವಿಗುಣ default_val)
{
    val = default_val;
    ಯದಿ( !ಗಂಟು.empty() )
    {
        val = (ದ್ವಿಗುಣ)ಗಂಟು;
    }
}

ಶೂನ್ಯ ಪಠ(ಧ್ರುವ File_ಗಂಟು& ಗಂಟು, ಭಿನ್ನ& val, ಭಿನ್ನ default_val)
{
    val = default_val;
    ಯದಿ( !ಗಂಟು.empty() )
    {
        val = (ಭಿನ್ನ)ಗಂಟು;
    }
}

ಶೂನ್ಯ ಪಠ(ಧ್ರುವ File_ಗಂಟು& ಗಂಟು, std::string& val, ಧ್ರುವ std::string& default_val)
{
    val = default_val;
    ಯದಿ( !ಗಂಟು.empty() )
    {
        val = (std::string)ಗಂಟು;
    }
}
ಶೂನ್ಯ FileStorage::Impl::ಲಿಖ(ಧ್ರುವ String &ಕೀಲಿ, ತಾರ್ಕಿಕ value)
{
    CV_Assert(write_mode);
    ಯದಿ (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().ಲಿಖ(ಕೀಲಿ.c_str(), (ಪೂರ್ಣಾಙ್ಕ)value);
    }
    ಅನ್ಯಥಾ
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().ಲಿಖ(ಕೀಲಿ.c_str(), value ? "true" : "false", ಅಸತ್ಯ);
    }
}

FileStorage_API::~FileStorage_API() {}

ನಾಮಾಕಾಶ internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, ಧ್ರುವ std::string& name,
                                       ಪೂರ್ಣಾಙ್ಕ flags, ಧ್ರುವ std::string& typeName)
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
