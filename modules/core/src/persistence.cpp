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

#ವ್ಯಾಖ್ಯಾನಿಸು CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

ನಾಮಾಕಾಶ cv
{

ನಾಮಾಕಾಶ fs
{

ಪೂರ್ಣಾಂಕ strcasecmp(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* s1, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* s2)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dummy="";
    ಆದರೆ(!s1) s1=dummy;
    ಆದರೆ(!s2) s2=dummy;

    size_t len1 = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(s1);
    size_t len2 = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(s2);
    size_t i, len = std::min(len1, len2);
    ಗಾಗಿ( i = 0; i < len; i++ )
    {
        ಪೂರ್ಣಾಂಕ d = tolower((ಪೂರ್ಣಾಂಕ)s1[i]) - tolower((ಪೂರ್ಣಾಂಕ)s2[i]);
        ಆದರೆ( d != 0 )
            ಹಿಂತಿರುಗಿ d;
    }
    ಹಿಂತಿರುಗಿ len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

ಅಕ್ಷರ* itoa( ಪೂರ್ಣಾಂಕ _val, ಅಕ್ಷರ* ಬಫರ್, ಪೂರ್ಣಾಂಕ /*radix*/ )
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ radix = 10;
    ಅಕ್ಷರ* ptr=ಬಫರ್ + 23 /* enough even for 64-bit integers */;
    ಚಿಹ್ನೆರಹಿತ val = ನಿರಪೇಕ್ಷ(_val);

    *ptr = '\0';
    ಮಾಡು
    {
        ಚಿಹ್ನೆರಹಿತ r = val / radix;
        *--ptr = (ಅಕ್ಷರ)(val - (r*radix) + '0');
        val = r;
    }
    ಆಗ( val != 0 );

    ಆದರೆ( _val < 0 )
        *--ptr = '-';

    ಹಿಂತಿರುಗಿ ptr;
}

ಅಕ್ಷರ* itoa( int64_t _val, ಅಕ್ಷರ* ಬಫರ್, ಪೂರ್ಣಾಂಕ /*radix*/, ತಾರ್ಕಿಕ _signed)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ radix = 10;
    ಅಕ್ಷರ* ptr=ಬಫರ್ + 23 /* enough even for 64-bit integers */;
    ಪೂರ್ಣಾಂಕ sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : ನಿರಪೇಕ್ಷ(_val);

    *ptr = '\0';
    ಮಾಡು
    {
        uint64_t r = val / radix;
        *--ptr = (ಅಕ್ಷರ)(val - (r*radix) + '0');
        val = r;
    }
    ಆಗ( val != 0 );

    ಆದರೆ( sign < 0 )
        *--ptr = '-';

    ಹಿಂತಿರುಗಿ ptr;
}

ಅಕ್ಷರ* double_ಗಮ್ಯ_String( ಅಕ್ಷರ* buf, size_t buf_ಗಾತ್ರ, ದ್ವಿಗುಣ value, ತಾರ್ಕಿಕ explicitZero )
{
    Cv64suf val;
    ಚಿಹ್ನೆರಹಿತ ieee754_hi;

    val.f = value;
    ieee754_hi = (ಚಿಹ್ನೆರಹಿತ)(val.u >> 32);

    ಆದರೆ( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        ಪೂರ್ಣಾಂಕ ivalue = cv_ಸುತ್ತು(value);
        ಆದರೆ( ivalue == value )
        {
            ಆದರೆ( explicitZero )
                snprintf( buf, buf_ಗಾತ್ರ, "%d.0", ivalue );
            ಇಲ್ಲದಿದ್ದರೆ
                snprintf( buf, buf_ಗಾತ್ರ, "%d.", ivalue );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, buf_ಗಾತ್ರ, "%.17g", value );

            ಅಕ್ಷರ* ptr = buf;
            ಆದರೆ( *ptr == '+' || *ptr == '-' )
                ptr++;
            ಗಾಗಿ( ; cv_isdigit(*ptr); ptr++ )
                ;
            ಆದರೆ( *ptr == ',' )
                *ptr = '.';
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಚಿಹ್ನೆರಹಿತ ieee754_lo = (ಚಿಹ್ನೆರಹಿತ)val.u;
        ಆದರೆ( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        ಇಲ್ಲದಿದ್ದರೆ
            strcpy( buf, (ಪೂರ್ಣಾಂಕ)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    ಹಿಂತಿರುಗಿ buf;
}

ಅಕ್ಷರ* float_ಗಮ್ಯ_String( ಅಕ್ಷರ* buf, size_t buf_ಗಾತ್ರ, ದಶಮಾಂಶ value, ತಾರ್ಕಿಕ halfprecision, ತಾರ್ಕಿಕ explicitZero )
{
    Cv32suf val;
    ಚಿಹ್ನೆರಹಿತ ieee754;
    val.f = value;
    ieee754 = val.u;

    ಆದರೆ( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        ಪೂರ್ಣಾಂಕ ivalue = cv_ಸುತ್ತು(value);
        ಆದರೆ( ivalue == value )
        {
            ಆದರೆ( explicitZero )
                snprintf( buf, buf_ಗಾತ್ರ, "%d.0", ivalue );
            ಇಲ್ಲದಿದ್ದರೆ
                snprintf( buf, buf_ಗಾತ್ರ, "%d.", ivalue );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, buf_ಗಾತ್ರ, "%.5g", value);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, buf_ಗಾತ್ರ, "%.9g", value);
            }

            ಅಕ್ಷರ* ptr = buf;
            ಆದರೆ( *ptr == '+' || *ptr == '-' )
                ptr++;
            ಗಾಗಿ( ; cv_isdigit(*ptr); ptr++ )
                ;
            ಆದರೆ( *ptr == ',' )
                *ptr = '.';
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        ಇಲ್ಲದಿದ್ದರೆ
            strcpy( buf, (ಪೂರ್ಣಾಂಕ)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    ಹಿಂತಿರುಗಿ buf;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ symbols[] = "ucwsifdhHbUIneE";

ಸ್ಥಿರ ಅಕ್ಷರ type_ಚಿಹ್ನೆ(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_Check_ಆಳ(ಆಳ, ಆಳ >= 0 && ಆಳ <= CV_8F_E4M3FNUZ, "");
    ಹಿಂತಿರುಗಿ symbols[ಆಳ];
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಚಿಹ್ನೆ_ಗಮ್ಯ_Type(ಅಕ್ಷರ c)
{
    ಆದರೆ (c == 'r')
        ಹಿಂತಿರುಗಿ CV_SEQ_ELTYPE_PTR;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* pos = strchr( symbols, c );
    ಆದರೆ( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    ಹಿಂತಿರುಗಿ static_cast<ಪೂರ್ಣಾಂಕ>(pos - symbols);
}

ಅಕ್ಷರ* encodeFormat(ಪೂರ್ಣಾಂಕ elem_type, ಅಕ್ಷರ* dt, size_t dt_len)
{
    ಪೂರ್ಣಾಂಕ cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    ಅಕ್ಷರ ಚಿಹ್ನೆ = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_ಚಿಹ್ನೆ(CV_MAT_ಆಳ(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, ಚಿಹ್ನೆ);
    ಹಿಂತಿರುಗಿ dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
ಅಕ್ಷರ* encodeFormat(ಪೂರ್ಣಾಂಕ elem_type, ಅಕ್ಷರ* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    ಹಿಂತಿರುಗಿ encodeFormat(elem_type, dt, max);
}

ಪೂರ್ಣಾಂಕ decodeFormat( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt, ಪೂರ್ಣಾಂಕ* fmt_ಜೋಡಿಗಳು, ಪೂರ್ಣಾಂಕ max_len )
{
    ಪೂರ್ಣಾಂಕ fmt_pair_ಎಣಿಕೆ = 0;
    ಪೂರ್ಣಾಂಕ i = 0, k = 0, len = dt ? (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(dt) : 0;

    ಆದರೆ( !dt || !len )
        ಹಿಂತಿರುಗಿ 0;

    CV_Assert( fmt_ಜೋಡಿಗಳು != 0 && max_len > 0 );
    fmt_ಜೋಡಿಗಳು[0] = 0;
    max_len *= 2;

    ಗಾಗಿ( ; k < len; k++ )
    {
        ಅಕ್ಷರ c = dt[k];

        ಆದರೆ( cv_isdigit(c) )
        {
            ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 = c - '0';
            ಆದರೆ( cv_isdigit(dt[k+1]) )
            {
                ಅಕ್ಷರ* endptr = 0;
                ಎಣಿಕೆ_3 = (ಪೂರ್ಣಾಂಕ)strtol( dt+k, &endptr, 10 );
                k = (ಪೂರ್ಣಾಂಕ)(endptr - dt) - 1;
            }

            ಆದರೆ( ಎಣಿಕೆ_3 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_ಜೋಡಿಗಳು[i] = ಎಣಿಕೆ_3;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಪೂರ್ಣಾಂಕ ಆಳ = ಚಿಹ್ನೆ_ಗಮ್ಯ_Type(c);
            ಆದರೆ( fmt_ಜೋಡಿಗಳು[i] == 0 )
                fmt_ಜೋಡಿಗಳು[i] = 1;
            fmt_ಜೋಡಿಗಳು[i+1] = ಆಳ;
            ಆದರೆ( i > 0 && fmt_ಜೋಡಿಗಳು[i+1] == fmt_ಜೋಡಿಗಳು[i-1] )
                fmt_ಜೋಡಿಗಳು[i-2] += fmt_ಜೋಡಿಗಳು[i];
            ಇಲ್ಲದಿದ್ದರೆ
            {
                i += 2;
                ಆದರೆ( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_ಜೋಡಿಗಳು[i] = 0;
        }
    }

    fmt_pair_ಎಣಿಕೆ = i/2;
    ಹಿಂತಿರುಗಿ fmt_pair_ಎಣಿಕೆ;
}

ಪೂರ್ಣಾಂಕ calc_Elem_ಗಾತ್ರ( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt, ಪೂರ್ಣಾಂಕ initial_ಗಾತ್ರ )
{
    ಪೂರ್ಣಾಂಕ size = 0;
    ಪೂರ್ಣಾಂಕ fmt_ಜೋಡಿಗಳು[CV_FS_MAX_FMT_ಜೋಡಿಗಳು*2], i, fmt_pair_ಎಣಿಕೆ;
    ಪೂರ್ಣಾಂಕ comp_ಗಾತ್ರ;

    fmt_pair_ಎಣಿಕೆ = decodeFormat( dt, fmt_ಜೋಡಿಗಳು, CV_FS_MAX_FMT_ಜೋಡಿಗಳು );
    fmt_pair_ಎಣಿಕೆ *= 2;
    ಗಾಗಿ( i = 0, size = initial_ಗಾತ್ರ; i < fmt_pair_ಎಣಿಕೆ; i += 2 )
    {
        comp_ಗಾತ್ರ = CV_ELEM_ಗಾತ್ರ(fmt_ಜೋಡಿಗಳು[i+1]);
        size = cvAlign( size, comp_ಗಾತ್ರ );
        size += comp_ಗಾತ್ರ * fmt_ಜೋಡಿಗಳು[i];
    }
    ಆದರೆ( initial_ಗಾತ್ರ == 0 )
    {
        comp_ಗಾತ್ರ = CV_ELEM_ಗಾತ್ರ(fmt_ಜೋಡಿಗಳು[1]);
        size = cvAlign( size, comp_ಗಾತ್ರ );
    }
    ಹಿಂತಿರುಗಿ size;
}


ಪೂರ್ಣಾಂಕ calc_Struct_ಗಾತ್ರ( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt, ಪೂರ್ಣಾಂಕ initial_ಗಾತ್ರ )
{
    ಪೂರ್ಣಾಂಕ size = calc_Elem_ಗಾತ್ರ( dt, initial_ಗಾತ್ರ );
    size_t elem_max_ಗಾತ್ರ = 0;
    ಗಾಗಿ ( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * type = dt; *type != '\0'; type++ )
    {
        ಅಕ್ಷರ v = *type;
        ಆದರೆ (v >= '0' && v <= '9')
            ಮುಂದುವರಿ;  // skip vector size
        ಆಯ್ಕೆ (v)
        {
        ಸ್ಥಿತಿ 'u': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(uchar ) ); ನಿಲ್ಲಿಸು; }
        ಸ್ಥಿತಿ 'b': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(ತಾರ್ಕಿಕ  ) ); ನಿಲ್ಲಿಸು; }
        ಸ್ಥಿತಿ 'c': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(schar ) ); ನಿಲ್ಲಿಸು; }
        ಸ್ಥಿತಿ 'w': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(ushort) ); ನಿಲ್ಲಿಸು; }
        ಸ್ಥಿತಿ 's': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(ಗಿಡ್ಡ ) ); ನಿಲ್ಲಿಸು; }
        ಸ್ಥಿತಿ 'i': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ   ) ); ನಿಲ್ಲಿಸು; }
        ಸ್ಥಿತಿ 'n': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ) ); ನಿಲ್ಲಿಸು; }
        ಸ್ಥಿತಿ 'f': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(ದಶಮಾಂಶ ) ); ನಿಲ್ಲಿಸು; }
        ಸ್ಥಿತಿ 'd': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(ದ್ವಿಗುಣ) ); ನಿಲ್ಲಿಸು; }
        ಸ್ಥಿತಿ 'h': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(hfloat)); ನಿಲ್ಲಿಸು; }
        ಸ್ಥಿತಿ 'H': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(bfloat)); ನಿಲ್ಲಿಸು; }
        ಸ್ಥಿತಿ 'e': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(fp8_t) ); ನಿಲ್ಲಿಸು; }
        ಸ್ಥಿತಿ 'E': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(fp8a_t)); ನಿಲ್ಲಿಸು; }
        ಸ್ಥಿತಿ 'I': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(int64_t)); ನಿಲ್ಲಿಸು; }
        ಸ್ಥಿತಿ 'U': { elem_max_ಗಾತ್ರ = std::max( elem_max_ಗಾತ್ರ, ಗಾತ್ರ(uint64_t)); ನಿಲ್ಲಿಸು; }
        ಪೂರ್ವನಿಯೋಜಿತ:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (ಅಕ್ಷರ)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<ಪೂರ್ಣಾಂಕ>(elem_max_ಗಾತ್ರ) );
    ಹಿಂತಿರುಗಿ size;
}

ಪೂರ್ಣಾಂಕ decodeSimpleFormat( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt )
{
    ಪೂರ್ಣಾಂಕ elem_type = -1;
    ಪೂರ್ಣಾಂಕ fmt_ಜೋಡಿಗಳು[CV_FS_MAX_FMT_ಜೋಡಿಗಳು*2], fmt_pair_ಎಣಿಕೆ;

    fmt_pair_ಎಣಿಕೆ = decodeFormat( dt, fmt_ಜೋಡಿಗಳು, CV_FS_MAX_FMT_ಜೋಡಿಗಳು );
    ಆದರೆ( fmt_pair_ಎಣಿಕೆ != 1 || fmt_ಜೋಡಿಗಳು[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_ಜೋಡಿಗಳು[1], fmt_ಜೋಡಿಗಳು[0] );

    ಹಿಂತಿರುಗಿ elem_type;
}

}

#ಆದರೆ defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LITTLE_ENDIAN_MEM_ACCESS 1
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LITTLE_ENDIAN_MEM_ACCESS 0
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ readInt(ಸ್ಥಿರಾಂಕ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ಆದರೆ CV_LITTLE_ENDIAN_MEM_ACCESS
    ಪೂರ್ಣಾಂಕ val;
    ಸ್ಮರಣೆ_ನಕಲು(&val, p, ಗಾತ್ರ(val));
    ಹಿಂತಿರುಗಿ val;
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ val = (ಪೂರ್ಣಾಂಕ)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    ಹಿಂತಿರುಗಿ val;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ int64_t readLong(ಸ್ಥಿರಾಂಕ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ಆದರೆ CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    ಸ್ಮರಣೆ_ನಕಲು(&val, p, ಗಾತ್ರ(val));
    ಹಿಂತಿರುಗಿ val;
#ಇಲ್ಲದಿದ್ದರೆ
    ಚಿಹ್ನೆರಹಿತ val0 = (ಚಿಹ್ನೆರಹಿತ)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    ಚಿಹ್ನೆರಹಿತ val1 = (ಚಿಹ್ನೆರಹಿತ)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    ಹಿಂತಿರುಗಿ val0 | ((int64_t)val1 << 32);
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ readReal(ಸ್ಥಿರಾಂಕ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ಆದರೆ CV_LITTLE_ENDIAN_MEM_ACCESS
    ದ್ವಿಗುಣ val;
    ಸ್ಮರಣೆ_ನಕಲು(&val, p, ಗಾತ್ರ(val));
    ಹಿಂತಿರುಗಿ val;
#ಇಲ್ಲದಿದ್ದರೆ
    ಚಿಹ್ನೆರಹಿತ val0 = (ಚಿಹ್ನೆರಹಿತ)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    ಚಿಹ್ನೆರಹಿತ val1 = (ಚಿಹ್ನೆರಹಿತ)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    ಹಿಂತಿರುಗಿ val.f;
#ಆದರೆ_ಅಂತ್ಯ
}

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ಆದರೆ CV_LITTLE_ENDIAN_MEM_ACCESS
    ಸ್ಮರಣೆ_ನಕಲು(p, &ival, ಗಾತ್ರ(ival));
#ಇಲ್ಲದಿದ್ದರೆ
    ಗಾಗಿ (size_t i = 0, j = 0; i < ಗಾತ್ರ(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ writeReal(uchar* p, ದ್ವಿಗುಣ fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ಆದರೆ CV_LITTLE_ENDIAN_MEM_ACCESS
    ಸ್ಮರಣೆ_ನಕಲು(p, &fval, ಗಾತ್ರ(fval));
#ಇಲ್ಲದಿದ್ದರೆ
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
#ಆದರೆ_ಅಂತ್ಯ
}



ಶೂನ್ಯ FileStorage::Impl::init() {
    flags = 0;
    ಬಫರ್.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = ಸುಳ್ಳು;
    state_ದ_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = ಸುಳ್ಳು;
    delayed_struct_ಕೀಲಿ = ಶೂನ್ಯಸೂಚಕ;
    delayed_struct_flags = 0;
    delayed_type_name = ಶೂನ್ಯಸೂಚಕ;
    base64_writer = ಶೂನ್ಯಸೂಚಕ;
    is_opened = ಸುಳ್ಳು;
    dummy_eof = ಸುಳ್ಳು;
    write_ಬಹುಲಕ = ಸುಳ್ಳು;
    mem_ಬಹುಲಕ = ಸುಳ್ಳು;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    ಖಾಲಿಗಳು_ಹರಿವು = ನಿಜ;

    strbufv.clear();
    strbuf = 0;
    strbufsize = strbufpos = 0;
    roots.clear();

    fs_ದತ್ತಾಂಶ.clear();
    fs_ದತ್ತಾಂಶ_ptrs.clear();
    fs_ದತ್ತಾಂಶ_blksz.clear();
    freeSpaceOfs = 0;

    str_hash.clear();
    str_hash_ದತ್ತಾಂಶ.clear();
    str_hash_ದತ್ತಾಂಶ.resize(1);
    str_hash_ದತ್ತಾಂಶ[0] = '\0';

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
    ಆದರೆ (is_opened) {
        ಆದರೆ (out)
            out->clear();
        ಆದರೆ (write_ಬಹುಲಕ) {
            ಆಗ (write_ರಾಶಿ.size() > 1) {
                ಕೊನೆ_Write_Struct();
            }
            flush();
            ಆದರೆ (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        ಆದರೆ (mem_ಬಹುಲಕ && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

ಶೂನ್ಯ FileStorage::Impl::analyze_file_name(ಸ್ಥಿರಾಂಕ std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ not_file_name = '\n';
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ parameter_begin = '?';
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ parameter_separator = '&';

    ಆದರೆ (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        ಹಿಂತಿರುಗಿ;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    ಆದರೆ (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        ಗಾಗಿ (size_t param_beg = beg, param_ಕೊನೆ = beg;
             param_ಕೊನೆ < end;
             param_beg = param_ಕೊನೆ + 1) {
            param_ಕೊನೆ = file_name.find_first_of(parameter_separator, param_beg);
            ಆದರೆ ((param_ಕೊನೆ == std::string::npos || param_ಕೊನೆ != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_ಕೊನೆ - param_beg));
            }
        }
    }
}

ತಾರ್ಕಿಕ FileStorage::Impl::open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename_or_buf, ಪೂರ್ಣಾಂಕ _flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *encoding) {
    ತಾರ್ಕಿಕ ok = ನಿಜ;
    release();

    ತಾರ್ಕಿಕ append = (_flags & 3) == FileStorage::APPEND;
    mem_ಬಹುಲಕ = (_flags & FileStorage::ಸ್ಮರಣೆ) != 0;

    write_ಬಹುಲಕ = (_flags & 3) != 0;
    ತಾರ್ಕಿಕ write_base64 = (write_ಬಹುಲಕ || append) && (_flags & FileStorage::BASE64) != 0;

    ತಾರ್ಕಿಕ isGZ = ಸುಳ್ಳು;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        ಆದರೆ (!params.empty())
            filename = params[0];

        ಆದರೆ (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_ಬಹುಲಕ || append);
    }

    ಆದರೆ (filename.size() == 0 && !mem_ಬಹುಲಕ && !write_ಬಹುಲಕ)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    ಆದರೆ (mem_ಬಹುಲಕ && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    ಆದರೆ (!mem_ಬಹುಲಕ) {
        size_t dot_idx = filename.find_last_of('.');
        ಅಕ್ಷರ compression = '\0';

        ಆದರೆ (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            ಆದರೆ (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                ಆದರೆ (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = ನಿಜ;
                }

                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = ನಿಜ;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        ಆದರೆ (!isGZ) {
            file = fopen(filename.c_str(), !write_ಬಹುಲಕ ? "rt" : !append ? "wt" : "a+t");
            ಆದರೆ (!file)
            {
                CV_LOG_ERROR(ಖಾಲಿ, "Can't open file: '" << filename << "' in " << (!write_ಬಹುಲಕ ? "read" : !append ? "write" : "append") << " mode");
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
            }
        } ಇಲ್ಲದಿದ್ದರೆ {
#ಆದರೆ USE_ZLIB
            ಆದರೆ (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            ಅಕ್ಷರ ಬಹುಲಕ[] = {write_ಬಹುಲಕ ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), ಬಹುಲಕ);
            ಆದರೆ (!gzfile)
            {
                CV_LOG_ERROR(ಖಾಲಿ, "Can't open archive: '" << filename << "' mode=" << ಬಹುಲಕ);
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
            }
#ಇಲ್ಲದಿದ್ದರೆ
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#ಆದರೆ_ಅಂತ್ಯ
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_ದತ್ತಾಂಶ.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    ಆದರೆ (write_ಬಹುಲಕ) {
        fmt = flags & FileStorage::FORMAT_ಮುಖವಾಡ;

        ಆದರೆ (mem_ಬಹುಲಕ)
            outbuf.clear();

        ಆದರೆ (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dot_pos = ಖಾಲಿ;
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dot_pos2 = ಖಾಲಿ;
            // like strrchr() implementation, but save two last positions simultaneously
            ಗಾಗಿ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pos = &filename[0]; pos[0] != 0; pos++) {
                ಆದರೆ (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            ಆದರೆ (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != ಖಾಲಿ) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        ಪೂರ್ಣಾಂಕ buf_ಗಾತ್ರ_2 = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        ಆದರೆ (append) {
            fseek(file, 0, SEEK_END);
            ಆದರೆ (ftell(file) == 0)
                append = ಸುಳ್ಳು;
        }

        write_ರಾಶಿ.clear();
        ಖಾಲಿಗಳು_ಹರಿವು = ನಿಜ;
        write_ರಾಶಿ.push_back(FStruct_ದತ್ತಾಂಶ("", File_ಗಂಟು::MAP | File_ಗಂಟು::ಖಾಲಿಗಳು_2, 0));
        ಬಫರ್.reserve(buf_ಗಾತ್ರ_2 + 1024);
        ಬಫರ್.resize(buf_ಗಾತ್ರ_2);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_ದ_writing_base64 = FileStorage_API::Base64State::Uncertain;

        ಆದರೆ (fmt == FileStorage::FORMAT_XML) {
            size_t file_ಗಾತ್ರ_2 = file ? (size_t) ftell(file) : (size_t) 0;
            ಆದರೆ (!append || file_ಗಾತ್ರ_2 == 0) {
                ಆದರೆ (encoding && *encoding != '\0') {
                    ಆದರೆ (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(encoding) < 1000);
                    ಅಕ್ಷರ buf[1100];
                    snprintf(buf, ಗಾತ್ರ(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } ಇಲ್ಲದಿದ್ದರೆ
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಪೂರ್ಣಾಂಕ xml_buf_ಗಾತ್ರ = 1 << 10;
                ಅಕ್ಷರ substr[] = "</opencv_storage>";
                ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_occurrence = -1;
                xml_buf_ಗಾತ್ರ = MIN(xml_buf_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ(file_ಗಾತ್ರ_2));
                fseek(file, -xml_buf_ಗಾತ್ರ, SEEK_END);
                // find the last occurrence of </opencv_storage>
                ಗಾಗಿ (;;) {
                    ಪೂರ್ಣಾಂಕ ಸಾಲು_ಸ್ಥಾನಾಂತರ = (ಪೂರ್ಣಾಂಕ) ftell(file);
                    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ptr0 = ಇದು->gets(xml_buf_ಗಾತ್ರ);
                    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ptr = ಖಾಲಿ;
                    ಆದರೆ (!ptr0)
                        ನಿಲ್ಲಿಸು;
                    ptr = ptr0;
                    ಗಾಗಿ (;;) {
                        ptr = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(ptr, substr);
                        ಆದರೆ (!ptr)
                            ನಿಲ್ಲಿಸು;
                        ಕೊನೆಯ_occurrence = ಸಾಲು_ಸ್ಥಾನಾಂತರ + (ಪೂರ್ಣಾಂಕ) (ptr - ptr0);
                        ptr += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(substr);
                    }
                }
                ಆದರೆ (ಕೊನೆಯ_occurrence < 0) {
                    release();
                    CV_Error(cv::Error::StsError, "Could not find </opencv_storage> in the end of file.\n");
                }
                closeFile();
                file = fopen(filename.c_str(), "r+t");
                CV_Assert(file != 0);
                fseek(file, ಕೊನೆಯ_occurrence, SEEK_SET);
                // replace the last "</opencv_storage>" with " <!-- resumed -->", which has the same length
                puts(" <!-- resumed -->");
                fseek(file, 0, SEEK_END);
                puts("\n");
            }

            emitter_do_not_use_ನೇರ_dereference = createXMLEmitter(ಇದು);
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                ಆದರೆ (!append) {
                    ಆದರೆ (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    ಇಲ್ಲದಿದ್ದರೆ
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            ಇಲ್ಲದಿದ್ದರೆ
                puts("...\n---\n");

            emitter_do_not_use_ನೇರ_dereference = createYAMLEmitter(ಇದು);
        } ಇಲ್ಲದಿದ್ದರೆ {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            ಆದರೆ (!append)
                puts("{\n");
            ಇಲ್ಲದಿದ್ದರೆ {
                ತಾರ್ಕಿಕ valid = ಸುಳ್ಳು;
                ದೀರ್ಘ roffset = 0;
                ಗಾಗಿ (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಕೊನೆ_mark = '}';
                    ಆದರೆ (fgetc(file) == ಕೊನೆ_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = ನಿಜ;
                        ನಿಲ್ಲಿಸು;
                    }
                }

                ಆದರೆ (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } ಇಲ್ಲದಿದ್ದರೆ {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_ರಾಶಿ.back().indent = 4;
            emitter_do_not_use_ನೇರ_dereference = createJSONEmitter(ಇದು);
        }
        is_opened = ನಿಜ;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಸ್ಥಿರಾಂಕ size_t buf_size0 = 40;
        ಬಫರ್.resize(buf_size0);
        ಆದರೆ (mem_ಬಹುಲಕ) {
            strbuf = (ಅಕ್ಷರ *) filename_or_buf;
            strbufsize = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(strbuf);
        }

        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *yaml_signature = "%YAML";
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *json_signature = "{";
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *xml_signature = "<?xml";
        ಅಕ್ಷರ *buf = ಇದು->gets(16);
        CV_Assert(buf);
        ಅಕ್ಷರ *bufPtr = cv_skip_BOM(buf);
        size_t buf_ಸ್ಥಾನಾಂತರ = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(bufPtr, yaml_signature, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(bufPtr, json_signature, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(bufPtr, xml_signature, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        ಇಲ್ಲದಿದ್ದರೆ
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = buf_ಸ್ಥಾನಾಂತರ;
        bufofs = 0;

        ಪ್ರಯತ್ನಿಸು {
            ಅಕ್ಷರ *ptr = ಬಫರ್_ಆರಂಭ();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_ಗಂಟು ಬೇರು_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_ಗಂಟು_Space(ಬೇರು_nodes, 9);
            *rptr = File_ಗಂಟು::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            ಆಯ್ಕೆ (fmt) {
                ಸ್ಥಿತಿ FileStorage::FORMAT_XML:
                    parser_do_not_use_ನೇರ_dereference = createXMLParser(ಇದು);
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ FileStorage::FORMAT_YAML:
                ಸ್ಥಿತಿ FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_ನೇರ_dereference = createYAMLParser(ಇದು);
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ FileStorage::FORMAT_JSON:
                    parser_do_not_use_ನೇರ_dereference = createJSONParser(ಇದು);
                    ನಿಲ್ಲಿಸು;
                ಪೂರ್ವನಿಯೋಜಿತ:
                    parser_do_not_use_ನೇರ_dereference = Ptr<FileStorageParser>();
            }

            ಆದರೆ (!parser_do_not_use_ನೇರ_dereference.empty()) {
                ok = getParser().parse(ptr);
                ಆದರೆ (ok) {
                    finalizeCollection(ಬೇರು_nodes);

                    CV_Assert(!fs_ದತ್ತಾಂಶ_ptrs.empty());
                    File_ಗಂಟು roots_ಗಂಟು(fs_ext, 0, 0);
                    size_t i, nroots = roots_ಗಂಟು.size();
                    File_ಗಂಟು_Iterator it = roots_ಗಂಟು.begin();

                    ಗಾಗಿ (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        ಹಿಡಿ (...)
        {
            // FIXIT log error message
            is_opened = ನಿಜ;
            release();
            ಎಸೆ;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = ನಿಜ;
        std::vector<ಅಕ್ಷರ> tmpbuf;
        std::swap(ಬಫರ್, tmpbuf);
        bufofs = 0;
    }
    ಹಿಂತಿರುಗಿ ok;
}

ಶೂನ್ಯ FileStorage::Impl::puts(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str) {
    CV_Assert(write_ಬಹುಲಕ);
    ಆದರೆ (mem_ಬಹುಲಕ)
        std::copy(str, str + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(str), std::back_inserter(outbuf));
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (file)
        fputs(str, file);
#ಆದರೆ USE_ZLIB
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (gzfile)
        gzputs(gzfile, str);
#ಆದರೆ_ಅಂತ್ಯ
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

ಅಕ್ಷರ *FileStorage::Impl::getsFromFile(ಅಕ್ಷರ *buf, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3) {
    ಆದರೆ (file)
        ಹಿಂತಿರುಗಿ fgets(buf, ಎಣಿಕೆ_3, file);
#ಆದರೆ USE_ZLIB
    ಆದರೆ (gzfile)
        ಹಿಂತಿರುಗಿ gzgets(gzfile, buf, ಎಣಿಕೆ_3);
#ಆದರೆ_ಅಂತ್ಯ
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

ಅಕ್ಷರ *FileStorage::Impl::gets(size_t max_ಎಣಿಕೆ) {
    ಆದರೆ (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *instr = strbuf;
        ಗಾಗಿ (; i < len; i++) {
            ಅಕ್ಷರ c = instr[i];
            ಆದರೆ (c == '\0' || c == '\n') {
                ಆದರೆ (c == '\n')
                    i++;
                ನಿಲ್ಲಿಸು;
            }
        }
        size_t ಎಣಿಕೆ_3 = i - strbufpos;
        ಆದರೆ (max_ಎಣಿಕೆ == 0 || max_ಎಣಿಕೆ > ಎಣಿಕೆ_3)
            max_ಎಣಿಕೆ = ಎಣಿಕೆ_3;
        ಬಫರ್.resize(std::max(ಬಫರ್.size(), max_ಎಣಿಕೆ + 8));
        ಸ್ಮರಣೆ_ನಕಲು(&ಬಫರ್[0], instr + strbufpos, max_ಎಣಿಕೆ);
        ಬಫರ್[max_ಎಣಿಕೆ] = '\0';
        strbufpos = i;
        ಹಿಂತಿರುಗಿ max_ಎಣಿಕೆ > 0 ? &ಬಫರ್[0] : 0;
    }

    ಸ್ಥಿರಾಂಕ size_t MAX_ಖಂಡ_ಗಾತ್ರ = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ / 2; // hopefully, that will be enough
    ಆದರೆ (max_ಎಣಿಕೆ == 0)
        max_ಎಣಿಕೆ = MAX_ಖಂಡ_ಗಾತ್ರ;
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Assert(max_ಎಣಿಕೆ < MAX_ಖಂಡ_ಗಾತ್ರ);
    size_t ofs = 0;

    ಗಾಗಿ (;;) {
        ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 = (ಪೂರ್ಣಾಂಕ) std::min(ಬಫರ್.size() - ofs - 16, max_ಎಣಿಕೆ);
        ಅಕ್ಷರ *ptr = getsFromFile(&ಬಫರ್[ofs], ಎಣಿಕೆ_3 + 1);
        ಆದರೆ (!ptr)
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ಣಾಂಕ delta = (ಪೂರ್ಣಾಂಕ) ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ptr);
        ofs += delta;
        max_ಎಣಿಕೆ -= delta;
        ಆದರೆ (delta == 0 || ptr[delta - 1] == '\n' || max_ಎಣಿಕೆ == 0)
            ನಿಲ್ಲಿಸು;
        ಆದರೆ (delta == ಎಣಿಕೆ_3)
            ಬಫರ್.resize((size_t) (ಬಫರ್.size() * 1.5));
    }
    ಹಿಂತಿರುಗಿ ofs > 0 ? &ಬಫರ್[0] : 0;
}

ಅಕ್ಷರ *FileStorage::Impl::gets() {
    ಅಕ್ಷರ *ptr = ಇದು->gets(0);
    ಆದರೆ (!ptr) {
        ptr = ಬಫರ್_ಆರಂಭ();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        ಗಣ_Eof();
        ಹಿಂತಿರುಗಿ 0;
    } ಇಲ್ಲದಿದ್ದರೆ {
        size_t l = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ptr);
        ಆದರೆ (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    ಹಿಂತಿರುಗಿ ptr;
}

ತಾರ್ಕಿಕ FileStorage::Impl::eof() {
    ಆದರೆ (dummy_eof)
        ಹಿಂತಿರುಗಿ ನಿಜ;
    ಆದರೆ (strbuf)
        ಹಿಂತಿರುಗಿ strbufpos >= strbufsize;
    ಆದರೆ (file)
        ಹಿಂತಿರುಗಿ feof(file) != 0;
#ಆದರೆ USE_ZLIB
    ಆದರೆ (gzfile)
        ಹಿಂತಿರುಗಿ gzeof(gzfile) != 0;
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

ಶೂನ್ಯ FileStorage::Impl::ಗಣ_Eof() {
    dummy_eof = ನಿಜ;
}

ಶೂನ್ಯ FileStorage::Impl::closeFile() {
    ಆದರೆ (file)
        fclose(file);
#ಆದರೆ USE_ZLIB
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (gzfile)
        gzclose(gzfile);
#ಆದರೆ_ಅಂತ್ಯ
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = ಸುಳ್ಳು;
}

ಶೂನ್ಯ FileStorage::Impl::rewind() {
    ಆದರೆ (file)
        ::rewind(file);
#ಆದರೆ USE_ZLIB
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (gzfile)
        gzrewind(gzfile);
#ಆದರೆ_ಅಂತ್ಯ
    strbufpos = 0;
}

ಅಕ್ಷರ *FileStorage::Impl::resize_Write_ಬಫರ್(ಅಕ್ಷರ *ptr, ಪೂರ್ಣಾಂಕ len) {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್_ಕೊನೆ = &ಬಫರ್[0] + ಬಫರ್.size();
    ಆದರೆ (ptr + len < ಬಫರ್_ಕೊನೆ)
        ಹಿಂತಿರುಗಿ ptr;

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್_ಆರಂಭ_2 = &ಬಫರ್[0];
    ಪೂರ್ಣಾಂಕ written_len = (ಪೂರ್ಣಾಂಕ) (ptr - ಬಫರ್_ಆರಂಭ_2);

    CV_Assert(written_len <= (ಪೂರ್ಣಾಂಕ) ಬಫರ್.size());
    ಪೂರ್ಣಾಂಕ new_ಗಾತ್ರ_2 = (ಪೂರ್ಣಾಂಕ) ((ಬಫರ್_ಕೊನೆ - ಬಫರ್_ಆರಂಭ_2) * 3 / 2);
    new_ಗಾತ್ರ_2 = MAX(written_len + len, new_ಗಾತ್ರ_2);
    ಬಫರ್.reserve(new_ಗಾತ್ರ_2 + 256);
    ಬಫರ್.resize(new_ಗಾತ್ರ_2);
    bufofs = written_len;
    ಹಿಂತಿರುಗಿ &ಬಫರ್[0] + bufofs;
}

ಅಕ್ಷರ *FileStorage::Impl::flush() {
    ಅಕ್ಷರ *ಬಫರ್_ಆರಂಭ_2 = &ಬಫರ್[0];
    ಅಕ್ಷರ *ptr = ಬಫರ್_ಆರಂಭ_2 + bufofs;

    ಆದರೆ (ptr > ಬಫರ್_ಆರಂಭ_2 + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(ಬಫರ್_ಆರಂಭ_2);
        bufofs = 0;
    }

    ಪೂರ್ಣಾಂಕ indent = write_ರಾಶಿ.back().indent;

    ಆದರೆ (space != indent) {
        memset(ಬಫರ್_ಆರಂಭ_2, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = ಬಫರ್_ಆರಂಭ_2 + bufofs;

    ಹಿಂತಿರುಗಿ ptr;
}

ಶೂನ್ಯ FileStorage::Impl::ಕೊನೆ_Write_Struct() {
    CV_Assert(write_ಬಹುಲಕ);

    check_if_write_struct_is_delayed(ಸುಳ್ಳು);
    ಆದರೆ (state_ದ_writing_base64 != FileStorage_API::Uncertain)
        switch_ಗಮ್ಯ_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_ರಾಶಿ.empty());

    FStruct_ದತ್ತಾಂಶ &current_struct = write_ರಾಶಿ.back();
    ಆದರೆ (fmt == FileStorage::FORMAT_JSON && !File_ಗಂಟು::isFlow(current_struct.flags) && write_ರಾಶಿ.size() > 1)
        current_struct.indent = write_ರಾಶಿ[write_ರಾಶಿ.size() - 2].indent;

    getEmitter().ಕೊನೆ_Write_Struct(current_struct);

    write_ರಾಶಿ.pop_back();
    ಆದರೆ (!write_ರಾಶಿ.empty())
        write_ರಾಶಿ.back().flags &= ~File_ಗಂಟು::ಖಾಲಿಗಳು_2;
}

ಶೂನ್ಯ FileStorage::Impl::ಆರಂಭ_Write_Struct_helper(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಪೂರ್ಣಾಂಕ struct_flags,
                                                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type_name) {
    CV_Assert(write_ಬಹುಲಕ);

    struct_flags = (struct_flags & (File_ಗಂಟು::TYPE_ಮುಖವಾಡ | File_ಗಂಟು::FLOW)) | File_ಗಂಟು::ಖಾಲಿಗಳು_2;
    ಆದರೆ (!File_ಗಂಟು::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    ಆದರೆ (type_name && type_name[0] == '\0')
        type_name = 0;

    FStruct_ದತ್ತಾಂಶ s = getEmitter().ಆರಂಭ_Write_Struct(write_ರಾಶಿ.back(), ಕೀಲಿ, struct_flags, type_name);

    write_ರಾಶಿ.push_back(s);
    size_t write_ರಾಶಿ_ಗಾತ್ರ = write_ರಾಶಿ.size();
    ಆದರೆ (write_ರಾಶಿ_ಗಾತ್ರ > 1)
        write_ರಾಶಿ[write_ರಾಶಿ_ಗಾತ್ರ - 2].flags &= ~File_ಗಂಟು::ಖಾಲಿಗಳು_2;

    ಆದರೆ (fmt != FileStorage::FORMAT_JSON && !File_ಗಂಟು::isFlow(s.flags))
        flush();

    ಆದರೆ (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_ಗಂಟು::isMap(struct_flags)) {
        getEmitter().ಲಿಖ("type_id", type_name, ಸುಳ್ಳು);
    }
}

ಶೂನ್ಯ FileStorage::Impl::ಆರಂಭ_Write_Struct(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಪೂರ್ಣಾಂಕ struct_flags,
                                         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type_name) {
    check_if_write_struct_is_delayed(ಸುಳ್ಳು);
    ಆದರೆ (state_ದ_writing_base64 == FileStorage_API::NotUse)
        switch_ಗಮ್ಯ_Base64_state(FileStorage_API::Uncertain);

    ಆದರೆ (state_ದ_writing_base64 == FileStorage_API::Uncertain && File_ಗಂಟು::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(ಕೀಲಿ, struct_flags, type_name);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        ಆದರೆ ((File_ಗಂಟು::TYPE_ಮುಖವಾಡ & struct_flags) != File_ಗಂಟು::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (state_ದ_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        ಆರಂಭ_Write_Struct_helper(ಕೀಲಿ, struct_flags, "binary");

        ಆದರೆ (state_ದ_writing_base64 != FileStorage_API::Uncertain)
            switch_ಗಮ್ಯ_Base64_state(FileStorage_API::Uncertain);
        switch_ಗಮ್ಯ_Base64_state(FileStorage_API::ಒಳ_Use);
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Won't output Base64 data */
        ಆದರೆ (state_ದ_writing_base64 == FileStorage_API::ಒಳ_Use)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        ಆರಂಭ_Write_Struct_helper(ಕೀಲಿ, struct_flags, type_name);

        ಆದರೆ (state_ದ_writing_base64 != FileStorage_API::Uncertain)
            switch_ಗಮ್ಯ_Base64_state(FileStorage_API::Uncertain);
        switch_ಗಮ್ಯ_Base64_state(FileStorage_API::NotUse);
    }
}

ಶೂನ್ಯ FileStorage::Impl::writeComment(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *comment, ತಾರ್ಕಿಕ eol_comment) {
    CV_Assert(write_ಬಹುಲಕ);
    getEmitter().writeComment(comment, eol_comment);
}

ಶೂನ್ಯ FileStorage::Impl::ಆರಂಭ_ಮುಂದಿನ_ಹರಿವು() {
    CV_Assert(write_ಬಹುಲಕ);
    ಆದರೆ (!ಖಾಲಿಗಳು_ಹರಿವು) {
        ಆಗ (!write_ರಾಶಿ.empty())
            ಕೊನೆ_Write_Struct();
        flush();
        getEmitter().ಆರಂಭ_ಮುಂದಿನ_ಹರಿವು();
        ಖಾಲಿಗಳು_ಹರಿವು = ನಿಜ;
        write_ರಾಶಿ.push_back(FStruct_ದತ್ತಾಂಶ("", File_ಗಂಟು::ಖಾಲಿಗಳು_2, 0));
        bufofs = 0;
    }
}

ಶೂನ್ಯ FileStorage::Impl::ಲಿಖ(ಸ್ಥಿರಾಂಕ String &ಕೀಲಿ, ಪೂರ್ಣಾಂಕ value) {
    CV_Assert(write_ಬಹುಲಕ);
    getEmitter().ಲಿಖ(ಕೀಲಿ.c_str(), value);
}

ಶೂನ್ಯ FileStorage::Impl::ಲಿಖ(ಸ್ಥಿರಾಂಕ String &ಕೀಲಿ, int64_t value) {
    CV_Assert(write_ಬಹುಲಕ);
    getEmitter().ಲಿಖ(ಕೀಲಿ.c_str(), value);
}

ಶೂನ್ಯ FileStorage::Impl::ಲಿಖ(ಸ್ಥಿರಾಂಕ String &ಕೀಲಿ, ದ್ವಿಗುಣ value) {
    CV_Assert(write_ಬಹುಲಕ);
    getEmitter().ಲಿಖ(ಕೀಲಿ.c_str(), value);
}

ಶೂನ್ಯ FileStorage::Impl::ಲಿಖ(ಸ್ಥಿರಾಂಕ String &ಕೀಲಿ, ಸ್ಥಿರಾಂಕ String &value) {
    CV_Assert(write_ಬಹುಲಕ);
    getEmitter().ಲಿಖ(ಕೀಲಿ.c_str(), value.c_str(), ಸುಳ್ಳು);
}

ಶೂನ್ಯ FileStorage::Impl::write_Raw_ದತ್ತಾಂಶ(ಸ್ಥಿರಾಂಕ std::string &dt, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ_3, size_t len) {
    CV_Assert(write_ಬಹುಲಕ);

    ಆದರೆ (is_using_base64 || state_ದ_writing_base64 == FileStorage_API::Base64State::ಒಳ_Use) {
        write_Raw_ದತ್ತಾಂಶ_Base64(ದತ್ತಾಂಶ_3, len, dt.c_str());
        ಹಿಂತಿರುಗಿ;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (state_ದ_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_ಗಮ್ಯ_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elem_ಗಾತ್ರ = fs::calc_Struct_ಗಾತ್ರ(dt.c_str(), 0);
    CV_Assert(elem_ಗಾತ್ರ);
    CV_Assert(len % elem_ಗಾತ್ರ == 0);
    len /= elem_ಗಾತ್ರ;

    ತಾರ್ಕಿಕ explicitZero = fmt == FileStorage::FORMAT_JSON;
    ಸ್ಥಿರಾಂಕ uchar *data0 = (ಸ್ಥಿರಾಂಕ uchar *) ದತ್ತಾಂಶ_3;
    ಪೂರ್ಣಾಂಕ fmt_ಜೋಡಿಗಳು[CV_FS_MAX_FMT_ಜೋಡಿಗಳು * 2], k, fmt_pair_ಎಣಿಕೆ;
    ಅಕ್ಷರ buf[256] = "";

    fmt_pair_ಎಣಿಕೆ = fs::decodeFormat(dt.c_str(), fmt_ಜೋಡಿಗಳು, CV_FS_MAX_FMT_ಜೋಡಿಗಳು);

    ಆದರೆ (!len)
        ಹಿಂತಿರುಗಿ;

    ಆದರೆ (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    ಆದರೆ (fmt_pair_ಎಣಿಕೆ == 1) {
        fmt_ಜೋಡಿಗಳು[0] *= (ಪೂರ್ಣಾಂಕ) len;
        len = 1;
    }

    ಗಾಗಿ (; len--; data0 += elem_ಗಾತ್ರ) {
        ಪೂರ್ಣಾಂಕ offset = 0;
        ಗಾಗಿ (k = 0; k < fmt_pair_ಎಣಿಕೆ; k++) {
            ಪೂರ್ಣಾಂಕ i, ಎಣಿಕೆ_3 = fmt_ಜೋಡಿಗಳು[k * 2];
            ಪೂರ್ಣಾಂಕ elem_type = fmt_ಜೋಡಿಗಳು[k * 2 + 1];
            ಪೂರ್ಣಾಂಕ elem_ಗಾತ್ರ_2 = CV_ELEM_ಗಾತ್ರ(elem_type);
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ptr;

            offset = cvAlign(offset, elem_ಗಾತ್ರ_2);
            ಸ್ಥಿರಾಂಕ uchar *ದತ್ತಾಂಶ = data0 + offset;

            ಗಾಗಿ (i = 0; i < ಎಣಿಕೆ_3; i++) {
                ಆಯ್ಕೆ (elem_type) {
                    ಸ್ಥಿತಿ CV_8U:
                        ptr = fs::itoa(*(uchar *) ದತ್ತಾಂಶ, buf, 10);
                        ದತ್ತಾಂಶ++;
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_Bool:
                        ptr = fs::itoa(*(uchar *) ದತ್ತಾಂಶ != 0, buf, 10);
                        ದತ್ತಾಂಶ++;
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_8S:
                        ptr = fs::itoa(*(ಅಕ್ಷರ *) ದತ್ತಾಂಶ, buf, 10);
                        ದತ್ತಾಂಶ++;
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_16U:
                        ptr = fs::itoa(*(ushort *) ದತ್ತಾಂಶ, buf, 10);
                        ದತ್ತಾಂಶ += ಗಾತ್ರ(ushort);
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_16S:
                        ptr = fs::itoa(*(ಗಿಡ್ಡ *) ದತ್ತಾಂಶ, buf, 10);
                        ದತ್ತಾಂಶ += ಗಾತ್ರ(ಗಿಡ್ಡ);
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_32U:
                        ptr = fs::itoa((int64_t)*(ಚಿಹ್ನೆರಹಿತ*) ದತ್ತಾಂಶ, buf, 10, ಸುಳ್ಳು);
                        ದತ್ತಾಂಶ += ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ);
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_32S:
                        ptr = fs::itoa(*(ಪೂರ್ಣಾಂಕ *) ದತ್ತಾಂಶ, buf, 10);
                        ದತ್ತಾಂಶ += ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ);
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_64U:
                        ptr = fs::itoa(*(uint64_t*) ದತ್ತಾಂಶ, buf, 10, ಸುಳ್ಳು);
                        ದತ್ತಾಂಶ += ಗಾತ್ರ(uint64_t);
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_64S:
                        ptr = fs::itoa(*(int64_t*) ದತ್ತಾಂಶ, buf, 10, ನಿಜ);
                        ದತ್ತಾಂಶ += ಗಾತ್ರ(int64_t);
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_32F:
                        ptr = fs::float_ಗಮ್ಯ_String(buf, ಗಾತ್ರ(buf), *(ದಶಮಾಂಶ *) ದತ್ತಾಂಶ, ಸುಳ್ಳು, explicitZero);
                        ದತ್ತಾಂಶ += ಗಾತ್ರ(ದಶಮಾಂಶ);
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_64F:
                        ptr = fs::double_ಗಮ್ಯ_String(buf, ಗಾತ್ರ(buf), *(ದ್ವಿಗುಣ *) ದತ್ತಾಂಶ, explicitZero);
                        ದತ್ತಾಂಶ += ಗಾತ್ರ(ದ್ವಿಗುಣ);
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_16F:
                        ptr = fs::float_ಗಮ್ಯ_String(buf, ಗಾತ್ರ(buf), (ದಶಮಾಂಶ) *(hfloat *) ದತ್ತಾಂಶ, ನಿಜ, explicitZero);
                        ದತ್ತಾಂಶ += ಗಾತ್ರ(hfloat);
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_16BF:
                        ptr = fs::float_ಗಮ್ಯ_String(buf, ಗಾತ್ರ(buf), (ದಶಮಾಂಶ) *(bfloat *) ದತ್ತಾಂಶ, ನಿಜ, explicitZero);
                        ದತ್ತಾಂಶ += ಗಾತ್ರ(bfloat);
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_8F_E4M3FN:
                        ptr = fs::float_ಗಮ್ಯ_String(buf, ಗಾತ್ರ(buf), (ದಶಮಾಂಶ) *(fp8_t *) ದತ್ತಾಂಶ, ನಿಜ, explicitZero);
                        ದತ್ತಾಂಶ += ಗಾತ್ರ(fp8_t);
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_8F_E4M3FNUZ:
                        ptr = fs::float_ಗಮ್ಯ_String(buf, ಗಾತ್ರ(buf), (ದಶಮಾಂಶ) *(fp8a_t *) ದತ್ತಾಂಶ, ನಿಜ, explicitZero);
                        ದತ್ತಾಂಶ += ಗಾತ್ರ(fp8a_t);
                        ನಿಲ್ಲಿಸು;
                    ಪೂರ್ವನಿಯೋಜಿತ:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        ಹಿಂತಿರುಗಿ;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (ಪೂರ್ಣಾಂಕ) (ದತ್ತಾಂಶ - data0);
        }
    }
}

ಶೂನ್ಯ FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(ಸುಳ್ಳು);

    ಆದರೆ (state_ದ_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_ಗಮ್ಯ_Base64_state(FileStorage_API::Base64State::Uncertain);
}

ಶೂನ್ಯ FileStorage::Impl::switch_ಗಮ್ಯ_Base64_state(FileStorage_API::Base64State new_state) {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *err_unable_ಗಮ್ಯ_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    ಆಯ್ಕೆ (state_ದ_writing_base64) {
        ಸ್ಥಿತಿ FileStorage_API::Base64State::Uncertain:
            ಆಯ್ಕೆ (new_state) {
                ಸ್ಥಿತಿ FileStorage_API::Base64State::ಒಳ_Use:
                {
                    CV_DbgAssert(base64_writer == 0);
                    ತಾರ್ಕಿಕ can_indent = (fmt != cv::FileStorage::ಬಹುಲಕ_2::FORMAT_JSON);
                    base64_writer = ಹೊಸ base64::Base64Writer(*ಇದು, can_indent);
                    ಆದರೆ (!can_indent) {
                        ಅಕ್ಷರ *ptr = ಬಫರ್_Ptr();
                        *ptr++ = '\0';
                        puts(ಬಫರ್_ಆರಂಭ());
                        ಗಣ_ಬಫರ್_Ptr(ಬಫರ್_ಆರಂಭ());
                        memset(ಬಫರ್_ಆರಂಭ(), 0, static_cast<ಪೂರ್ಣಾಂಕ>(space));
                        puts("\"$base64$");
                    }
                    ನಿಲ್ಲಿಸು;
                }
                ಸ್ಥಿತಿ FileStorage_API::Base64State::Uncertain:
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ FileStorage_API::Base64State::NotUse:
                    ನಿಲ್ಲಿಸು;
                ಪೂರ್ವನಿಯೋಜಿತ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ನಿಲ್ಲಿಸು;
            }
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ FileStorage_API::Base64State::ಒಳ_Use:
            ಆಯ್ಕೆ (new_state) {
                ಸ್ಥಿತಿ FileStorage_API::Base64State::ಒಳ_Use:
                ಸ್ಥಿತಿ FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_ಗಮ್ಯ_switch);
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ FileStorage_API::Base64State::Uncertain:
                    ಅಳಿಸು base64_writer;
                    base64_writer = 0;
                    ಆದರೆ ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        ಗಣ_ಬಫರ್_Ptr(ಬಫರ್_ಆರಂಭ());
                        flush();
                        memset(ಬಫರ್_ಆರಂಭ(), 0, static_cast<ಪೂರ್ಣಾಂಕ>(space) );
                        ಗಣ_ಬಫರ್_Ptr(ಬಫರ್_ಆರಂಭ());
                    }
                    ನಿಲ್ಲಿಸು;
                ಪೂರ್ವನಿಯೋಜಿತ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ನಿಲ್ಲಿಸು;
            }
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ FileStorage_API::Base64State::NotUse:
            ಆಯ್ಕೆ (new_state) {
                ಸ್ಥಿತಿ FileStorage_API::Base64State::ಒಳ_Use:
                ಸ್ಥಿತಿ FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_ಗಮ್ಯ_switch);
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ FileStorage_API::Base64State::Uncertain:
                    ನಿಲ್ಲಿಸು;
                ಪೂರ್ವನಿಯೋಜಿತ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ನಿಲ್ಲಿಸು;
            }
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            ನಿಲ್ಲಿಸು;
    }

    state_ದ_writing_base64 = new_state;
}

ಶೂನ್ಯ FileStorage::Impl::make_write_struct_delayed(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಪೂರ್ಣಾಂಕ struct_flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type_name) {
    CV_Assert(is_write_struct_delayed == ಸುಳ್ಳು);
    CV_DbgAssert(delayed_struct_ಕೀಲಿ == ಶೂನ್ಯಸೂಚಕ);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == ಶೂನ್ಯಸೂಚಕ);

    delayed_struct_flags = struct_flags;

    ಆದರೆ (ಕೀಲಿ != ಶೂನ್ಯಸೂಚಕ) {
        delayed_struct_ಕೀಲಿ = ಹೊಸ ಅಕ್ಷರ[ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಕೀಲಿ) + 1U];
        strcpy(delayed_struct_ಕೀಲಿ, ಕೀಲಿ);
    }

    ಆದರೆ (type_name != ಶೂನ್ಯಸೂಚಕ) {
        delayed_type_name = ಹೊಸ ಅಕ್ಷರ[ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = ನಿಜ;
}

ಶೂನ್ಯ FileStorage::Impl::check_if_write_struct_is_delayed(ತಾರ್ಕಿಕ change_type_ಗಮ್ಯ_base64) {
    ಆದರೆ (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_ಕೀಲಿ;
        std::string type_name;
        ಪೂರ್ಣಾಂಕ struct_flags = delayed_struct_flags;

        ಆದರೆ (delayed_struct_ಕೀಲಿ != ಶೂನ್ಯಸೂಚಕ && *delayed_struct_ಕೀಲಿ != '\0') {
            struct_ಕೀಲಿ.assign(delayed_struct_ಕೀಲಿ);
        }
        ಆದರೆ (delayed_type_name != ಶೂನ್ಯಸೂಚಕ && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        ಅಳಿಸು[] delayed_struct_ಕೀಲಿ;
        ಅಳಿಸು[] delayed_type_name;
        delayed_struct_ಕೀಲಿ = ಶೂನ್ಯಸೂಚಕ;
        delayed_struct_flags = 0;
        delayed_type_name = ಶೂನ್ಯಸೂಚಕ;

        is_write_struct_delayed = ಸುಳ್ಳು;

        /* call */
        ಆದರೆ (change_type_ಗಮ್ಯ_base64) {
            ಆರಂಭ_Write_Struct_helper(struct_ಕೀಲಿ.c_str(), struct_flags, "binary");
            ಆದರೆ (state_ದ_writing_base64 != FileStorage_API::Uncertain)
                switch_ಗಮ್ಯ_Base64_state(FileStorage_API::Uncertain);
            switch_ಗಮ್ಯ_Base64_state(FileStorage_API::ಒಳ_Use);
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಆರಂಭ_Write_Struct_helper(struct_ಕೀಲಿ.c_str(), struct_flags, type_name.c_str());
            ಆದರೆ (state_ದ_writing_base64 != FileStorage_API::Uncertain)
                switch_ಗಮ್ಯ_Base64_state(FileStorage_API::Uncertain);
            switch_ಗಮ್ಯ_Base64_state(FileStorage_API::NotUse);
        }
    }
}

ಶೂನ್ಯ FileStorage::Impl::write_Raw_ದತ್ತಾಂಶ_Base64(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ_3, size_t len, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dt) {
    CV_Assert(write_ಬಹುಲಕ);

    check_if_write_struct_is_delayed(ನಿಜ);

    ಆದರೆ (state_ದ_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_ಗಮ್ಯ_Base64_state(FileStorage_API::Base64State::ಒಳ_Use);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (state_ದ_writing_base64 != FileStorage_API::Base64State::ಒಳ_Use) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->ಲಿಖ(ದತ್ತಾಂಶ_3, len, dt);
}

File_ಗಂಟು FileStorage::Impl::get_ಮೊದಲ_ತುದಿ_Level_ಗಂಟು() ಸ್ಥಿರಾಂಕ {
    ಹಿಂತಿರುಗಿ roots.empty() ? File_ಗಂಟು() : roots[0];
}

File_ಗಂಟು FileStorage::Impl::ಬೇರು(ಪೂರ್ಣಾಂಕ ಹರಿವು_Idx) ಸ್ಥಿರಾಂಕ {
    ಹಿಂತಿರುಗಿ ಹರಿವು_Idx >= 0 && ಹರಿವು_Idx < (ಪೂರ್ಣಾಂಕ) roots.size() ? roots[ಹರಿವು_Idx] : File_ಗಂಟು();
}

File_ಗಂಟು FileStorage::Impl::ಕ್ರಿಯಾಕಾರಕ[](ಸ್ಥಿರಾಂಕ String &nodename) ಸ್ಥಿರಾಂಕ {
    ಹಿಂತಿರುಗಿ ಇದು->ಕ್ರಿಯಾಕಾರಕ[](nodename.c_str());
}

File_ಗಂಟು FileStorage::Impl::ಕ್ರಿಯಾಕಾರಕ[](ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * /*nodename*/) ಸ್ಥಿರಾಂಕ {
    ಹಿಂತಿರುಗಿ File_ಗಂಟು();
}

ಪೂರ್ಣಾಂಕ FileStorage::Impl::getFormat() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ fmt; }

ಅಕ್ಷರ *FileStorage::Impl::ಬಫರ್_Ptr() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ *) (&ಬಫರ್[0] + bufofs); }

ಅಕ್ಷರ *FileStorage::Impl::ಬಫರ್_ಆರಂಭ() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ *) &ಬಫರ್[0]; }

ಅಕ್ಷರ *FileStorage::Impl::ಬಫರ್_ಕೊನೆ_2() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ *) (&ಬಫರ್[0] + ಬಫರ್.size()); }

ಶೂನ್ಯ FileStorage::Impl::ಗಣ_ಬಫರ್_Ptr(ಅಕ್ಷರ *ptr) {
    ಅಕ್ಷರ *bufferstart = ಬಫರ್_ಆರಂಭ();
    CV_Assert(ptr >= bufferstart && ptr <= ಬಫರ್_ಕೊನೆ_2());
    bufofs = ptr - bufferstart;
}

ಪೂರ್ಣಾಂಕ FileStorage::Impl::wrapMargin() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ wrap_margin; }

FStruct_ದತ್ತಾಂಶ &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_ರಾಶಿ.empty());
    ಹಿಂತಿರುಗಿ write_ರಾಶಿ.back();
}

ಶೂನ್ಯ FileStorage::Impl::ಗಣ_Non_ಖಾಲಿಗಳು() {
    ಖಾಲಿಗಳು_ಹರಿವು = ಸುಳ್ಳು;
}

ಶೂನ್ಯ FileStorage::Impl::processSpecialDouble(ಅಕ್ಷರ *buf, ದ್ವಿಗುಣ *value, ಅಕ್ಷರ **endptr) {
    FileStorage_API *fs = ಇದು;
    ಅಕ್ಷರ c = buf[0];
    ಪೂರ್ಣಾಂಕ inf_hi = 0x7ff00000;

    ಆದರೆ (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    ಆದರೆ (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    ಆದರೆ (ದೊಡ್ಡ_ಅಕ್ಷರ(buf[1]) == 'I' && ದೊಡ್ಡ_ಅಕ್ಷರ(buf[2]) == 'N' && ದೊಡ್ಡ_ಅಕ್ಷರ(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ದೊಡ್ಡ_ಅಕ್ಷರ(buf[1]) == 'N' && ದೊಡ್ಡ_ಅಕ್ಷರ(buf[2]) == 'A' && ದೊಡ್ಡ_ಅಕ್ಷರ(buf[3]) == 'N')
        v.u = (uint64) -1;
    ಇಲ್ಲದಿದ್ದರೆ
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

ದ್ವಿಗುಣ FileStorage::Impl::strtod(ಅಕ್ಷರ *ptr, ಅಕ್ಷರ **endptr) {
    ದ್ವಿಗುಣ fval = ::strtod(ptr, endptr);
    ಆದರೆ (**endptr == '.') {
        ಅಕ್ಷರ *dot_pos = *endptr;
        *dot_pos = ',';
        ದ್ವಿಗುಣ fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        ಆದರೆ (*endptr > dot_pos)
            fval = fval2;
        ಇಲ್ಲದಿದ್ದರೆ
            *endptr = dot_pos;
    }

    ಆದರೆ (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    ಹಿಂತಿರುಗಿ fval;
}

ಶೂನ್ಯ FileStorage::Impl::convert_ಗಮ್ಯ_Collection(ಪೂರ್ಣಾಂಕ type, File_ಗಂಟು &ಗಂಟು) {
    CV_Assert(type == File_ಗಂಟು::SEQ || type == File_ಗಂಟು::MAP);

    ಪೂರ್ಣಾಂಕ ಗಂಟು_type = ಗಂಟು.type();
    ಆದರೆ (ಗಂಟು_type == type)
        ಹಿಂತಿರುಗಿ;

    ತಾರ್ಕಿಕ named = ಗಂಟು.isNamed();
    uchar *ptr = ಗಂಟು.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    ದ್ವಿಗುಣ fval = 0;
    std::string sval;
    ತಾರ್ಕಿಕ ಕೂಡಿಸು_ಮೊದಲ_scalar = ಸುಳ್ಳು;

    ಆದರೆ (ಗಂಟು_type != File_ಗಂಟು::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_ಗಂಟು::SEQ);
        ಆದರೆ (ಗಂಟು_type == File_ಗಂಟು::INT) {
            ival = readLong(ptr);
            ಕೂಡಿಸು_ಮೊದಲ_scalar = ನಿಜ;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಂಟು_type == File_ಗಂಟು::REAL) {
            fval = readReal(ptr);
            ಕೂಡಿಸು_ಮೊದಲ_scalar = ನಿಜ;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಂಟು_type == File_ಗಂಟು::STRING) {
            sval = std::string(ಗಂಟು);
            ಕೂಡಿಸು_ಮೊದಲ_scalar = ನಿಜ;
        } ಇಲ್ಲದಿದ್ದರೆ
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", ಗಂಟು_type));
    }

    ptr = reserve_ಗಂಟು_Space(ಗಂಟು, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_ಗಂಟು::NAMED : 0));
    // name has been copied automatically
    ಆದರೆ (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    ಆದರೆ (ಕೂಡಿಸು_ಮೊದಲ_scalar)
        ಕೂಡಿಸು_ಗಂಟು(ಗಂಟು, std::string(), ಗಂಟು_type,
                ಗಂಟು_type == File_ಗಂಟು::INT ? (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &ival :
                ಗಂಟು_type == File_ಗಂಟು::REAL ? (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) &fval :
                ಗಂಟು_type == File_ಗಂಟು::STRING ? (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *) sval.c_str() : 0,
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
    ತಾರ್ಕಿಕ shrink_ಖಂಡ = ಸುಳ್ಳು;
    size_t shrink_ಖಂಡ_Idx = 0, shrink_ಗಾತ್ರ = 0;

    uchar *ptr = 0, *ಖಂಡ_ಕೊನೆ = 0;

    ಆದರೆ (!fs_ದತ್ತಾಂಶ_ptrs.empty()) {
        size_t ಖಂಡ_Idx = ಗಂಟು.ಖಂಡ_Idx;
        size_t ofs = ಗಂಟು.ofs;
        CV_Assert(ಖಂಡ_Idx == fs_ದತ್ತಾಂಶ_ptrs.size() - 1);
        CV_Assert(ofs <= fs_ದತ್ತಾಂಶ_blksz[ಖಂಡ_Idx]);
        CV_Assert(freeSpaceOfs <= fs_ದತ್ತಾಂಶ_blksz[ಖಂಡ_Idx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_ದತ್ತಾಂಶ_ptrs[ಖಂಡ_Idx] + ofs;
        ಖಂಡ_ಕೊನೆ = fs_ದತ್ತಾಂಶ_ptrs[ಖಂಡ_Idx] + fs_ದತ್ತಾಂಶ_blksz[ಖಂಡ_Idx];

        CV_Assert(ptr >= fs_ದತ್ತಾಂಶ_ptrs[ಖಂಡ_Idx] && ptr <= ಖಂಡ_ಕೊನೆ);
        ಆದರೆ (ptr + sz <= ಖಂಡ_ಕೊನೆ) {
            freeSpaceOfs = ofs + sz;
            ಹಿಂತಿರುಗಿ ptr;
        }

        ಆದರೆ (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_ದತ್ತಾಂಶ[ಖಂಡ_Idx]->resize(sz);
            ptr = &fs_ದತ್ತಾಂಶ[ಖಂಡ_Idx]->at(0);
            fs_ದತ್ತಾಂಶ_ptrs[ಖಂಡ_Idx] = ptr;
            fs_ದತ್ತಾಂಶ_blksz[ಖಂಡ_Idx] = sz;
            freeSpaceOfs = sz;
            ಹಿಂತಿರುಗಿ ptr;
        }

        shrink_ಖಂಡ = ನಿಜ;
        shrink_ಖಂಡ_Idx = ಖಂಡ_Idx;
        shrink_ಗಾತ್ರ = ofs;
    }

    size_t ಖಂಡ_ಗಾತ್ರ_2 = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(ಖಂಡ_ಗಾತ್ರ_2);
    fs_ದತ್ತಾಂಶ.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_ದತ್ತಾಂಶ_ptrs.push_back(new_ptr);
    fs_ದತ್ತಾಂಶ_blksz.push_back(ಖಂಡ_ಗಾತ್ರ_2);
    ಗಂಟು.ಖಂಡ_Idx = fs_ದತ್ತಾಂಶ_ptrs.size() - 1;
    ಗಂಟು.ofs = 0;
    freeSpaceOfs = sz;

    ಆದರೆ (ptr && ptr + 5 <= ಖಂಡ_ಕೊನೆ) {
        new_ptr[0] = ptr[0];
        ಆದರೆ (ptr[0] & File_ಗಂಟು::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    ಆದರೆ (shrink_ಖಂಡ) {
        fs_ದತ್ತಾಂಶ[shrink_ಖಂಡ_Idx]->resize(shrink_ಗಾತ್ರ);
        fs_ದತ್ತಾಂಶ_blksz[shrink_ಖಂಡ_Idx] = shrink_ಗಾತ್ರ;
    }

    ಹಿಂತಿರುಗಿ new_ptr;
}

ಚಿಹ್ನೆರಹಿತ FileStorage::Impl::getStringOfs(ಸ್ಥಿರಾಂಕ std::string &ಕೀಲಿ) ಸ್ಥಿರಾಂಕ {
    str_hash_t::const_iterator it = str_hash.find(ಕೀಲಿ);
    ಹಿಂತಿರುಗಿ it != str_hash.end() ? it->second : 0;
}

File_ಗಂಟು FileStorage::Impl::ಕೂಡಿಸು_ಗಂಟು(File_ಗಂಟು &collection, ಸ್ಥಿರಾಂಕ std::string &ಕೀಲಿ,
                                    ಪೂರ್ಣಾಂಕ elem_type, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *value, ಪೂರ್ಣಾಂಕ len) {
    FileStorage_API *fs = ಇದು;
    ತಾರ್ಕಿಕ noname = ಕೀಲಿ.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(ಕೀಲಿ.c_str(), "_") == 0);
    convert_ಗಮ್ಯ_Collection(noname ? File_ಗಂಟು::SEQ : File_ಗಂಟು::MAP, collection);

    ತಾರ್ಕಿಕ isseq = collection.empty() ? ಸುಳ್ಳು : collection.isSeq();
    ಆದರೆ (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    ಚಿಹ್ನೆರಹಿತ strofs = 0;
    ಆದರೆ (!noname) {
        strofs = getStringOfs(ಕೀಲಿ);
        ಆದರೆ (!strofs) {
            strofs = (ಚಿಹ್ನೆರಹಿತ) str_hash_ದತ್ತಾಂಶ.size();
            size_t keysize = ಕೀಲಿ.size() + 1;
            str_hash_ದತ್ತಾಂಶ.resize(strofs + keysize);
            ಸ್ಮರಣೆ_ನಕಲು(&str_hash_ದತ್ತಾಂಶ[0] + strofs, &ಕೀಲಿ[0], keysize);
            str_hash.insert(std::make_pair(ಕೀಲಿ, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t ಖಂಡ_Idx = fs_ದತ್ತಾಂಶ_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_ಗಂಟು ಗಂಟು(fs_ext, ಖಂಡ_Idx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_ಗಂಟು_Space(ಗಂಟು, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_ಗಂಟು::NAMED));
    ಆದರೆ (elem_type == File_ಗಂಟು::NONE)
        freeSpaceOfs -= 8;

    ಆದರೆ (!noname) {
        writeInt(ptr, (ಪೂರ್ಣಾಂಕ) strofs);
        ptr += 4;
    }

    ಆದರೆ (elem_type == File_ಗಂಟು::SEQ || elem_type == File_ಗಂಟು::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    ಆದರೆ (value)
        ಗಂಟು.ಗಣ_ಮೌಲ್ಯ(elem_type, value, len);

    ಆದರೆ (collection.isNamed())
        cp += 4;
    ಪೂರ್ಣಾಂಕ nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    ಹಿಂತಿರುಗಿ ಗಂಟು;
}

ಶೂನ್ಯ FileStorage::Impl::finalizeCollection(File_ಗಂಟು &collection) {
    ಆದರೆ (!collection.isSeq() && !collection.isMap())
        ಹಿಂತಿರುಗಿ;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    ಆದರೆ (*ptr0 & File_ಗಂಟು::NAMED)
        ptr += 4;
    size_t ಖಂಡ_Idx = collection.ಖಂಡ_Idx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t raw_ಗಾತ್ರ = 4;
    ಚಿಹ್ನೆರಹಿತ sz = (ಚಿಹ್ನೆರಹಿತ) readInt(ptr + 4);
    ಆದರೆ (sz > 0) {
        size_t ಕೊನೆಯ_ಖಂಡ_Idx = fs_ದತ್ತಾಂಶ_ptrs.size() - 1;

        ಗಾಗಿ (; ಖಂಡ_Idx < ಕೊನೆಯ_ಖಂಡ_Idx; ಖಂಡ_Idx++) {
            raw_ಗಾತ್ರ += fs_ದತ್ತಾಂಶ_blksz[ಖಂಡ_Idx] - ofs;
            ofs = 0;
        }
    }
    raw_ಗಾತ್ರ += freeSpaceOfs - ofs;
    writeInt(ptr, (ಪೂರ್ಣಾಂಕ) raw_ಗಾತ್ರ);
}

ಶೂನ್ಯ FileStorage::Impl::normalize_ಗಂಟು_Ofs(size_t &ಖಂಡ_Idx, size_t &ofs) ಸ್ಥಿರಾಂಕ {
    ಆಗ (ofs >= fs_ದತ್ತಾಂಶ_blksz[ಖಂಡ_Idx]) {
        ಆದರೆ (ಖಂಡ_Idx == fs_ದತ್ತಾಂಶ_blksz.size() - 1) {
            CV_Assert(ofs == fs_ದತ್ತಾಂಶ_blksz[ಖಂಡ_Idx]);
            ನಿಲ್ಲಿಸು;
        }
        ofs -= fs_ದತ್ತಾಂಶ_blksz[ಖಂಡ_Idx];
        ಖಂಡ_Idx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_ದ_writing_base64() {
    ಹಿಂತಿರುಗಿ state_ದ_writing_base64;
}

ಪೂರ್ಣಾಂಕ FileStorage::Impl::get_space() {
    ಹಿಂತಿರುಗಿ space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = ನಿಜ;
}

ಶೂನ್ಯ FileStorage::Impl::Base64Decoder::init(ಸ್ಥಿರಾಂಕ Ptr<FileStorageParser> &_parser, ಅಕ್ಷರ *_ptr, ಪೂರ್ಣಾಂಕ _indent) {
    parser_do_not_use_ನೇರ_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = ಸುಳ್ಳು;
}

ತಾರ್ಕಿಕ FileStorage::Impl::Base64Decoder::read_ಇನ್ನಷ್ಟು(ಪೂರ್ಣಾಂಕ needed) {
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uchar base64tab[] =
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

    ಆದರೆ (eos)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    ಗಾಗಿ (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    ಅಕ್ಷರ *beg = 0, *end = 0;
    ತಾರ್ಕಿಕ ok = getParser().get_Base64_ಅಡ್ಡಸಾಲು(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    ಆದರೆ (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = ನಿಜ;
        size_t tc = totalchars;
        ಗಾಗಿ (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    ಪೂರ್ಣಾಂಕ i = 0, j, n = (ಪೂರ್ಣಾಂಕ) encoded.size();
    ಆದರೆ (n > 0) {
        ಸ್ಥಿರಾಂಕ uchar *tab = base64tab;
        ಅಕ್ಷರ *src = &encoded[0];

        ಗಾಗಿ (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(ಪೂರ್ಣಾಂಕ) (uchar) src[i]], c = tab[(ಪೂರ್ಣಾಂಕ) (uchar) src[i + 1]];
            uchar b = tab[(ಪೂರ್ಣಾಂಕ) (uchar) src[i + 2]], a = tab[(ಪೂರ್ಣಾಂಕ) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    ಆದರೆ (i > 0 && encoded[i - 1] == '=') {
        ಆದರೆ (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        ಆದರೆ (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    ಗಾಗಿ (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    ಆದರೆ (ofs >= sz && !read_ಇನ್ನಷ್ಟು(1))
        ಹಿಂತಿರುಗಿ (uchar) 0;
    ಹಿಂತಿರುಗಿ decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    ಆದರೆ (ofs + 2 > sz && !read_ಇನ್ನಷ್ಟು(2))
        ಹಿಂತಿರುಗಿ (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    ಹಿಂತಿರುಗಿ val;
}

ಪೂರ್ಣಾಂಕ FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    ಆದರೆ (ofs + 4 > sz && !read_ಇನ್ನಷ್ಟು(4))
        ಹಿಂತಿರುಗಿ 0;
    ಪೂರ್ಣಾಂಕ ival = readInt(&decoded[ofs]);
    ofs += 4;
    ಹಿಂತಿರುಗಿ ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    ಆದರೆ (ofs + 8 > sz && !read_ಇನ್ನಷ್ಟು(8))
        ಹಿಂತಿರುಗಿ 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    ಹಿಂತಿರುಗಿ ival;
}

ದ್ವಿಗುಣ FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    ಆದರೆ (ofs + 8 > sz && !read_ಇನ್ನಷ್ಟು(8))
        ಹಿಂತಿರುಗಿ 0;
    ದ್ವಿಗುಣ fval = readReal(&decoded[ofs]);
    ofs += 8;
    ಹಿಂತಿರುಗಿ fval;
}

ತಾರ್ಕಿಕ FileStorage::Impl::Base64Decoder::ಕೊನೆ_ದ_ಹರಿವು() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ eos; }

ಅಕ್ಷರ *FileStorage::Impl::Base64Decoder::getPtr() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ ptr; }


ಅಕ್ಷರ *FileStorage::Impl::parseBase64(ಅಕ್ಷರ *ptr, ಪೂರ್ಣಾಂಕ indent, File_ಗಂಟು &collection) {
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ BASE64_HDR_ಗಾತ್ರ = 24;
    ಅಕ್ಷರ dt[BASE64_HDR_ಗಾತ್ರ + 1] = {0};
    base64decoder.init(parser_do_not_use_ನೇರ_dereference, ptr, indent);

    ಪೂರ್ಣಾಂಕ i, k;

    ಗಾಗಿ (i = 0; i < BASE64_HDR_ಗಾತ್ರ; i++)
        dt[i] = (ಅಕ್ಷರ) base64decoder.getUInt8();
    ಗಾಗಿ (i = 0; i < BASE64_HDR_ಗಾತ್ರ; i++)
        ಆದರೆ (ಖಾಲಿಯೇ(dt[i]))
            ನಿಲ್ಲಿಸು;
    dt[i] = '\0';

    CV_Assert(!base64decoder.ಕೊನೆ_ದ_ಹರಿವು());

    ಪೂರ್ಣಾಂಕ fmt_ಜೋಡಿಗಳು[CV_FS_MAX_FMT_ಜೋಡಿಗಳು * 2];
    ಪೂರ್ಣಾಂಕ fmt_pair_ಎಣಿಕೆ = fs::decodeFormat(dt, fmt_ಜೋಡಿಗಳು, CV_FS_MAX_FMT_ಜೋಡಿಗಳು);
    int64_t ival = 0;
    ದ್ವಿಗುಣ fval = 0;

    ಗಾಗಿ (;;) {
        ಗಾಗಿ (k = 0; k < fmt_pair_ಎಣಿಕೆ; k++) {
            ಪೂರ್ಣಾಂಕ elem_type = fmt_ಜೋಡಿಗಳು[k * 2 + 1];
            ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 = fmt_ಜೋಡಿಗಳು[k * 2];

            ಗಾಗಿ (i = 0; i < ಎಣಿಕೆ_3; i++) {
                ಪೂರ್ಣಾಂಕ ಗಂಟು_type = File_ಗಂಟು::INT;
                ಆಯ್ಕೆ (elem_type) {
                    ಸ್ಥಿತಿ CV_8U:
                        ival = base64decoder.getUInt8();
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_8S:
                        ival = (ಅಕ್ಷರ) base64decoder.getUInt8();
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_16U:
                        ival = base64decoder.getUInt16();
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_16S:
                        ival = (ಗಿಡ್ಡ) base64decoder.getUInt16();
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_32U:
                        ival = base64decoder.getInt32();
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_32S:
                        ival = base64decoder.getInt32();
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_64U:
                        ival = base64decoder.getInt64();
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_64S:
                        ival = base64decoder.getInt64();
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        ಗಂಟು_type = File_ಗಂಟು::REAL;
                    }
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_64F:
                        fval = base64decoder.getFloat64();
                        ಗಂಟು_type = File_ಗಂಟು::REAL;
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_16F:
                        fval = ದಶಮಾಂಶ(hfloat_From_ಬಿಟ್ಟುಗಳು(base64decoder.getUInt16()));
                        ಗಂಟು_type = File_ಗಂಟು::REAL;
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        ಗಂಟು_type = File_ಗಂಟು::REAL;
                        ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        ಗಂಟು_type = File_ಗಂಟು::REAL;
                        ನಿಲ್ಲಿಸು;
                    ಪೂರ್ವನಿಯೋಜಿತ:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                ಆದರೆ (base64decoder.ಕೊನೆ_ದ_ಹರಿವು())
                    ನಿಲ್ಲಿಸು;
                ಕೂಡಿಸು_ಗಂಟು(collection, std::string(), ಗಂಟು_type,
                        ಗಂಟು_type == File_ಗಂಟು::INT ? (ಶೂನ್ಯ *) &ival : (ಶೂನ್ಯ *) &fval, -1);
            }
        }
        ಆದರೆ (base64decoder.ಕೊನೆ_ದ_ಹರಿವು())
            ನಿಲ್ಲಿಸು;
    }

    finalizeCollection(collection);
    ಹಿಂತಿರುಗಿ base64decoder.getPtr();
}

ಶೂನ್ಯ FileStorage::Impl::parseError(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *func_name, ಸ್ಥಿರಾಂಕ std::string &err_ಸಂದೇಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೂಲ_file,
                                   ಪೂರ್ಣಾಂಕ ಮೂಲ_ಸಾಲು) {
    std::string ಸಂದೇಶ = format("%s(%d): %s", filename.c_str(), lineno, err_ಸಂದೇಶ.c_str());
    error(Error::StsParseError, func_name, ಸಂದೇಶ.c_str(), ಮೂಲ_file, ಮೂಲ_ಸಾಲು);
}

ಸ್ಥಿರಾಂಕ uchar *FileStorage::Impl::get_ಗಂಟು_Ptr(size_t ಖಂಡ_Idx, size_t ofs) ಸ್ಥಿರಾಂಕ {
    CV_Assert(ಖಂಡ_Idx < fs_ದತ್ತಾಂಶ_ptrs.size());
    CV_Assert(ofs < fs_ದತ್ತಾಂಶ_blksz[ಖಂಡ_Idx]);

    ಹಿಂತಿರುಗಿ fs_ದತ್ತಾಂಶ_ptrs[ಖಂಡ_Idx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) ಸ್ಥಿರಾಂಕ {
    CV_Assert(nameofs < str_hash_ದತ್ತಾಂಶ.size());
    ಹಿಂತಿರುಗಿ std::string(&str_hash_ದತ್ತಾಂಶ[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { ಹಿಂತಿರುಗಿ fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(ಇದು);
}

FileStorage::FileStorage(ಸ್ಥಿರಾಂಕ String& filename, ಪೂರ್ಣಾಂಕ flags, ಸ್ಥಿರಾಂಕ String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(ಇದು);
    ತಾರ್ಕಿಕ ok = p->open(filename.c_str(), flags, encoding.c_str());
    ಆದರೆ(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

ಶೂನ್ಯ FileStorage::ಆರಂಭ_Write_Struct(ಸ್ಥಿರಾಂಕ String& name, ಪೂರ್ಣಾಂಕ struct_flags, ಸ್ಥಿರಾಂಕ String& typeName)
{
    p->ಆರಂಭ_Write_Struct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    ಆದರೆ ((struct_flags & File_ಗಂಟು::TYPE_ಮುಖವಾಡ) == File_ಗಂಟು::SEQ)
        state = FileStorage::ಮೌಲ್ಯ_EXPECTED;
    ಇಲ್ಲದಿದ್ದರೆ
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

ಶೂನ್ಯ FileStorage::ಕೊನೆ_Write_Struct()
{
    p->ಕೊನೆ_Write_Struct();
    state = p->write_ರಾಶಿ.empty() || File_ಗಂಟು::isMap(p->write_ರಾಶಿ.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::ಮೌಲ್ಯ_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

ತಾರ್ಕಿಕ FileStorage::open(ಸ್ಥಿರಾಂಕ String& filename, ಪೂರ್ಣಾಂಕ flags, ಸ್ಥಿರಾಂಕ String& encoding)
{
    ಪ್ರಯತ್ನಿಸು
    {
        ತಾರ್ಕಿಕ ok = p->open(filename.c_str(), flags, encoding.c_str());
        ಆದರೆ(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        ಹಿಂತಿರುಗಿ ok;
    }
    ಹಿಡಿ (...)
    {
        release();
        ಎಸೆ;  // re-throw
    }
}

ತಾರ್ಕಿಕ FileStorage::isOpened() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ p->is_opened; }

ಶೂನ್ಯ FileStorage::release()
{
    p->release();
}

File_ಗಂಟು FileStorage::ಬೇರು(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ( p.empty() || p->roots.empty() || i < 0 || i >= (ಪೂರ್ಣಾಂಕ)p->roots.size() )
        ಹಿಂತಿರುಗಿ File_ಗಂಟು();

    ಹಿಂತಿರುಗಿ p->roots[i];
}

File_ಗಂಟು FileStorage::get_ಮೊದಲ_ತುದಿ_Level_ಗಂಟು() ಸ್ಥಿರಾಂಕ
{
    File_ಗಂಟು r = ಬೇರು();
    File_ಗಂಟು_Iterator it = r.begin();
    ಹಿಂತಿರುಗಿ it != r.end() ? *it : File_ಗಂಟು();
}

std::string FileStorage::getDefaultObjectName(ಸ್ಥಿರಾಂಕ std::string& _filename)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* stubname = "unnamed";
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* filename = _filename.c_str();
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ptr2 = filename + _filename.size();
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ptr = ptr2 - 1;
    cv::Auto_ಬಫರ್<ಅಕ್ಷರ> name_buf(_filename.size()+1);

    ಆಗ( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        ಆದರೆ( *ptr == '.' && (!*ptr2 || ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    ಆದರೆ( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    ಅಕ್ಷರ* name = name_buf.ದತ್ತಾಂಶ();

    // name must start with letter or '_'
    ಆದರೆ( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    ಆಗ( ptr < ptr2 )
    {
        ಅಕ್ಷರ c = *ptr++;
        ಆದರೆ( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.ದತ್ತಾಂಶ();
    ಆದರೆ( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    ಹಿಂತಿರುಗಿ name;
}


ಪೂರ್ಣಾಂಕ FileStorage::getFormat() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ p->fmt;
}

File_ಗಂಟು FileStorage::ಕ್ರಿಯಾಕಾರಕ [](ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ ಇದು->ಕ್ರಿಯಾಕಾರಕ[](std::string(ಕೀಲಿ));
}

File_ಗಂಟು FileStorage::ಕ್ರಿಯಾಕಾರಕ [](ಸ್ಥಿರಾಂಕ std::string& ಕೀಲಿ) ಸ್ಥಿರಾಂಕ
{
    File_ಗಂಟು res;
    ಗಾಗಿ (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][ಕೀಲಿ];
        ಆದರೆ (!res.empty())
            ನಿಲ್ಲಿಸು;
    }
    ಹಿಂತಿರುಗಿ res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    ಹಿಂತಿರುಗಿ buf;
}

ಶೂನ್ಯ FileStorage::writeRaw( ಸ್ಥಿರಾಂಕ String& fmt, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* vec, size_t len )
{
    p->write_Raw_ದತ್ತಾಂಶ(fmt, (ಸ್ಥಿರಾಂಕ uchar*)vec, len);
}

ಶೂನ್ಯ FileStorage::writeComment( ಸ್ಥಿರಾಂಕ String& comment, ತಾರ್ಕಿಕ eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

ಶೂನ್ಯ writeScalar( FileStorage& fs, ಪೂರ್ಣಾಂಕ value )
{
    fs.p->ಲಿಖ(String(), value);
}

ಶೂನ್ಯ writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->ಲಿಖ(String(), value);
}

ಶೂನ್ಯ writeScalar( FileStorage& fs, ದಶಮಾಂಶ value )
{
    fs.p->ಲಿಖ(String(), (ದ್ವಿಗುಣ)value);
}

ಶೂನ್ಯ writeScalar( FileStorage& fs, ದ್ವಿಗುಣ value )
{
    fs.p->ಲಿಖ(String(), value);
}

ಶೂನ್ಯ writeScalar( FileStorage& fs, ಸ್ಥಿರಾಂಕ String& value )
{
    fs.p->ಲಿಖ(String(), value);
}

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಸ್ಥಿರಾಂಕ String& name, ಪೂರ್ಣಾಂಕ value )
{
    fs.p->ಲಿಖ(name, value);
}

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಸ್ಥಿರಾಂಕ String& name, int64_t value )
{
    fs.p->ಲಿಖ(name, value);
}

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಸ್ಥಿರಾಂಕ String& name, ದಶಮಾಂಶ value )
{
    fs.p->ಲಿಖ(name, (ದ್ವಿಗುಣ)value);
}

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಸ್ಥಿರಾಂಕ String& name, ದ್ವಿಗುಣ value )
{
    fs.p->ಲಿಖ(name, value);
}

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಸ್ಥಿರಾಂಕ String& name, ಸ್ಥಿರಾಂಕ String& value )
{
    fs.p->ಲಿಖ(name, value);
}

ಶೂನ್ಯ ಲಿಖ( FileStorage& fs, ಸ್ಥಿರಾಂಕ String& name, ತಾರ್ಕಿಕ value )
{
    fs.p->ಲಿಖ(name, value);
}

ಶೂನ್ಯ FileStorage::ಲಿಖ(ಸ್ಥಿರಾಂಕ String& name, ಪೂರ್ಣಾಂಕ val) { p->ಲಿಖ(name, val); }
ಶೂನ್ಯ FileStorage::ಲಿಖ(ಸ್ಥಿರಾಂಕ String& name, ತಾರ್ಕಿಕ val) { p->ಲಿಖ(name, val); }
ಶೂನ್ಯ FileStorage::ಲಿಖ(ಸ್ಥಿರಾಂಕ String& name, int64_t val) { p->ಲಿಖ(name, val); }
ಶೂನ್ಯ FileStorage::ಲಿಖ(ಸ್ಥಿರಾಂಕ String& name, ದ್ವಿಗುಣ val) { p->ಲಿಖ(name, val); }
ಶೂನ್ಯ FileStorage::ಲಿಖ(ಸ್ಥಿರಾಂಕ String& name, ಸ್ಥಿರಾಂಕ String& val) { p->ಲಿಖ(name, val); }
ಶೂನ್ಯ FileStorage::ಲಿಖ(ಸ್ಥಿರಾಂಕ String& name, ಸ್ಥಿರಾಂಕ Mat& val) { cv::ಲಿಖ(*ಇದು, name, val); }
ಶೂನ್ಯ FileStorage::ಲಿಖ(ಸ್ಥಿರಾಂಕ String& name, ಸ್ಥಿರಾಂಕ std::vector<String>& val) { cv::ಲಿಖ(*ಇದು, name, val); }

FileStorage& ಕ್ರಿಯಾಕಾರಕ << (FileStorage& fs, ಸ್ಥಿರಾಂಕ String& str)
{
    ಎಣಿಕೆ { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        ಮೌಲ್ಯ_EXPECTED = FileStorage::ಮೌಲ್ಯ_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* _str = str.c_str();
    ಆದರೆ( !fs.isOpened() || !_str )
        ಹಿಂತಿರುಗಿ fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    ಅಕ್ಷರ c = *_str;

    ಆದರೆ( c == '}' || c == ']' )
    {
        ಆದರೆ( fs_impl->write_ರಾಶಿ.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        ಪೂರ್ಣಾಂಕ struct_flags = fs_impl->write_ರಾಶಿ.back().flags;
        ಅಕ್ಷರ expected_bracket = File_ಗಂಟು::isMap(struct_flags) ? '}' : ']';
        ಆದರೆ( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->ಕೊನೆ_Write_Struct();
        CV_Assert(!fs_impl->write_ರಾಶಿ.empty());
        struct_flags = fs_impl->write_ರಾಶಿ.back().flags;
        fs.state = File_ಗಂಟು::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : ಮೌಲ್ಯ_EXPECTED;
        fs.elname = String();
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        ಆದರೆ (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = ಮೌಲ್ಯ_EXPECTED + INSIDE_MAP;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( (fs.state & 3) == ಮೌಲ್ಯ_EXPECTED )
    {
        ಆದರೆ( c == '{' || c == '[' )
        {
            ಪೂರ್ಣಾಂಕ struct_flags = c == '{' ? File_ಗಂಟು::MAP : File_ಗಂಟು::SEQ;
            fs.state = struct_flags == File_ಗಂಟು::MAP ? INSIDE_MAP + NAME_EXPECTED : ಮೌಲ್ಯ_EXPECTED;
            _str++;
            ಆದರೆ( *_str == ':' )
            {
                _str++;
                ಆದರೆ( !*_str )
                    struct_flags |= File_ಗಂಟು::FLOW;
            }
            fs_impl->ಆರಂಭ_Write_Struct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಲಿಖ( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            ಆದರೆ( fs.state == INSIDE_MAP + ಮೌಲ್ಯ_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    ಹಿಂತಿರುಗಿ fs;
}


File_ಗಂಟು::File_ಗಂಟು()
    : fs(ಖಾಲಿ)
{
    ಖಂಡ_Idx = ofs = 0;
}

File_ಗಂಟು::File_ಗಂಟು(FileStorage::Impl* _fs, size_t ಖಂಡ_Idx_2, size_t _ofs)
    : fs(_fs)
{
    ಖಂಡ_Idx = ಖಂಡ_Idx_2;
    ofs = _ofs;
}

File_ಗಂಟು::File_ಗಂಟು(ಸ್ಥಿರಾಂಕ FileStorage* _fs, size_t ಖಂಡ_Idx_2, size_t _ofs)
    : File_ಗಂಟು(_fs->p.get(), ಖಂಡ_Idx_2, _ofs)
{
    // nothing
}

File_ಗಂಟು::File_ಗಂಟು(ಸ್ಥಿರಾಂಕ File_ಗಂಟು& ಗಂಟು)
{
    fs = ಗಂಟು.fs;
    ಖಂಡ_Idx = ಗಂಟು.ಖಂಡ_Idx;
    ofs = ಗಂಟು.ofs;
}

File_ಗಂಟು& File_ಗಂಟು::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ File_ಗಂಟು& ಗಂಟು)
{
    fs = ಗಂಟು.fs;
    ಖಂಡ_Idx = ಗಂಟು.ಖಂಡ_Idx;
    ofs = ಗಂಟು.ofs;
    ಹಿಂತಿರುಗಿ *ಇದು;
}

File_ಗಂಟು File_ಗಂಟು::ಕ್ರಿಯಾಕಾರಕ[](ಸ್ಥಿರಾಂಕ std::string& nodename) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(!fs)
        ಹಿಂತಿರುಗಿ File_ಗಂಟು();

    CV_Assert( isMap() );

    ಚಿಹ್ನೆರಹಿತ ಕೀಲಿ = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_ಗಂಟು_Iterator it = begin();

    ಗಾಗಿ( i = 0; i < sz; i++, ++it )
    {
        File_ಗಂಟು n = *it;
        ಸ್ಥಿರಾಂಕ uchar* p = n.ptr();
        ಚಿಹ್ನೆರಹಿತ key2 = (ಚಿಹ್ನೆರಹಿತ)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_ದತ್ತಾಂಶ.size() );
        ಆದರೆ( ಕೀಲಿ == key2 )
            ಹಿಂತಿರುಗಿ n;
    }
    ಹಿಂತಿರುಗಿ File_ಗಂಟು();
}

File_ಗಂಟು File_ಗಂಟು::ಕ್ರಿಯಾಕಾರಕ[](ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* nodename) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ ಇದು->ಕ್ರಿಯಾಕಾರಕ[](std::string(nodename));
}

File_ಗಂಟು File_ಗಂಟು::ಕ್ರಿಯಾಕಾರಕ[](ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ(!fs)
        ಹಿಂತಿರುಗಿ File_ಗಂಟು();

    CV_Assert( isSeq() );

    ಪೂರ್ಣಾಂಕ sz = (ಪೂರ್ಣಾಂಕ)size();
    CV_Assert( 0 <= i && i < sz );

    File_ಗಂಟು_Iterator it = begin();
    it += i;

    ಹಿಂತಿರುಗಿ *it;
}

std::vector<String> File_ಗಂಟು::ಕೀಲಿಗಳು() ಸ್ಥಿರಾಂಕ
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    ಗಾಗಿ (File_ಗಂಟು_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    ಹಿಂತಿರುಗಿ res;
}

ಪೂರ್ಣಾಂಕ File_ಗಂಟು::type() ಸ್ಥಿರಾಂಕ
{
    ಸ್ಥಿರಾಂಕ uchar* p = ptr();
    ಆದರೆ(!p)
        ಹಿಂತಿರುಗಿ NONE;
    ಹಿಂತಿರುಗಿ (*p & TYPE_ಮುಖವಾಡ);
}

ತಾರ್ಕಿಕ File_ಗಂಟು::isMap(ಪೂರ್ಣಾಂಕ flags) { ಹಿಂತಿರುಗಿ (flags & TYPE_ಮುಖವಾಡ) == MAP; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isSeq(ಪೂರ್ಣಾಂಕ flags) { ಹಿಂತಿರುಗಿ (flags & TYPE_ಮುಖವಾಡ) == SEQ; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isCollection(ಪೂರ್ಣಾಂಕ flags) { ಹಿಂತಿರುಗಿ isMap(flags) || isSeq(flags); }
ತಾರ್ಕಿಕ File_ಗಂಟು::isFlow(ಪೂರ್ಣಾಂಕ flags) { ಹಿಂತಿರುಗಿ (flags & FLOW) != 0; }
ತಾರ್ಕಿಕ File_ಗಂಟು::is_ಖಾಲಿಗಳು_Collection(ಪೂರ್ಣಾಂಕ flags) { ಹಿಂತಿರುಗಿ (flags & ಖಾಲಿಗಳು_2) != 0; }

ತಾರ್ಕಿಕ File_ಗಂಟು::empty() ಸ್ಥಿರಾಂಕ   { ಹಿಂತಿರುಗಿ fs == 0; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isNone() ಸ್ಥಿರಾಂಕ  { ಹಿಂತಿರುಗಿ type() == NONE; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isSeq() ಸ್ಥಿರಾಂಕ   { ಹಿಂತಿರುಗಿ type() == SEQ; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isMap() ಸ್ಥಿರಾಂಕ   { ಹಿಂತಿರುಗಿ type() == MAP; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isInt() ಸ್ಥಿರಾಂಕ   { ಹಿಂತಿರುಗಿ type() == INT;  }
ತಾರ್ಕಿಕ File_ಗಂಟು::isReal() ಸ್ಥಿರಾಂಕ  { ಹಿಂತಿರುಗಿ type() == REAL; }
ತಾರ್ಕಿಕ File_ಗಂಟು::isString() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ type() == STRING;  }
ತಾರ್ಕಿಕ File_ಗಂಟು::isNamed() ಸ್ಥಿರಾಂಕ
{
    ಸ್ಥಿರಾಂಕ uchar* p = ptr();
    ಆದರೆ(!p)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಹಿಂತಿರುಗಿ (*p & NAMED) != 0;
}

std::string File_ಗಂಟು::name() ಸ್ಥಿರಾಂಕ
{
    ಸ್ಥಿರಾಂಕ uchar* p = ptr();
    ಆದರೆ(!p)
        ಹಿಂತಿರುಗಿ std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    ಹಿಂತಿರುಗಿ fs->getName(nameofs);
}

File_ಗಂಟು::ಕ್ರಿಯಾಕಾರಕ ಪೂರ್ಣಾಂಕ() ಸ್ಥಿರಾಂಕ
{
    ಸ್ಥಿರಾಂಕ uchar* p = ptr();
    ಆದರೆ(!p)
        ಹಿಂತಿರುಗಿ 0;
    ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗ್ = *p;
    ಪೂರ್ಣಾಂಕ type = (ಟ್ಯಾಗ್ & TYPE_ಮುಖವಾಡ);
    p += (ಟ್ಯಾಗ್ & NAMED) ? 5 : 1;

    ಆದರೆ( type == INT )
    {
        ಹಿಂತಿರುಗಿ readInt(p);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == REAL )
    {
        ಹಿಂತಿರುಗಿ cv_ಸುತ್ತು(readReal(p));
    }
    ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ 0x7fffffff;
}

File_ಗಂಟು::ಕ್ರಿಯಾಕಾರಕ int64_t() ಸ್ಥಿರಾಂಕ
{
    ಸ್ಥಿರಾಂಕ uchar* p = ptr();
    ಆದರೆ(!p)
        ಹಿಂತಿರುಗಿ 0;
    ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗ್ = *p;
    ಪೂರ್ಣಾಂಕ type = (ಟ್ಯಾಗ್ & TYPE_ಮುಖವಾಡ);
    p += (ಟ್ಯಾಗ್ & NAMED) ? 5 : 1;

    ಆದರೆ( type == INT )
    {
        ಹಿಂತಿರುಗಿ readLong(p);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == REAL )
    {
        ಹಿಂತಿರುಗಿ cv_ಸುತ್ತು(readReal(p));
    }
    ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ 0x7fffffff;
}

File_ಗಂಟು::ಕ್ರಿಯಾಕಾರಕ ದಶಮಾಂಶ() ಸ್ಥಿರಾಂಕ
{
    ಸ್ಥಿರಾಂಕ uchar* p = ptr();
    ಆದರೆ(!p)
        ಹಿಂತಿರುಗಿ 0.f;
    ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗ್ = *p;
    ಪೂರ್ಣಾಂಕ type = (ಟ್ಯಾಗ್ & TYPE_ಮುಖವಾಡ);
    p += (ಟ್ಯಾಗ್ & NAMED) ? 5 : 1;

    ಆದರೆ( type == INT )
    {
        ಹಿಂತಿರುಗಿ (ದಶಮಾಂಶ)readLong(p);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == REAL )
    {
        ಹಿಂತಿರುಗಿ (ದಶಮಾಂಶ)readReal(p);
    }
    ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ FLT_MAX;
}

File_ಗಂಟು::ಕ್ರಿಯಾಕಾರಕ ದ್ವಿಗುಣ() ಸ್ಥಿರಾಂಕ
{
    ಸ್ಥಿರಾಂಕ uchar* p = ptr();
    ಆದರೆ(!p)
        ಹಿಂತಿರುಗಿ 0.f;
    ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗ್ = *p;
    ಪೂರ್ಣಾಂಕ type = (ಟ್ಯಾಗ್ & TYPE_ಮುಖವಾಡ);
    p += (ಟ್ಯಾಗ್ & NAMED) ? 5 : 1;

    ಆದರೆ( type == INT )
    {
        ಹಿಂತಿರುಗಿ (ದ್ವಿಗುಣ)readLong(p);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == REAL )
    {
        ಹಿಂತಿರುಗಿ readReal(p);
    }
    ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ DBL_MAX;
}

ದ್ವಿಗುಣ File_ಗಂಟು::real() ಸ್ಥಿರಾಂಕ  { ಹಿಂತಿರುಗಿ ದ್ವಿಗುಣ(*ಇದು); }

std::string File_ಗಂಟು::string() ಸ್ಥಿರಾಂಕ
{
    ಸ್ಥಿರಾಂಕ uchar* p = ptr();
    ಆದರೆ( !p || (*p & TYPE_ಮುಖವಾಡ) != STRING )
        ಹಿಂತಿರುಗಿ std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(ಚಿಹ್ನೆರಹಿತ)readInt(p);
    ಹಿಂತಿರುಗಿ std::string((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*)(p + 4), sz - 1);
}
Mat File_ಗಂಟು::mat() ಸ್ಥಿರಾಂಕ { Mat value; ಪಠ(*ಇದು, value, Mat()); ಹಿಂತಿರುಗಿ value; }

File_ಗಂಟು_Iterator File_ಗಂಟು::begin() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ File_ಗಂಟು_Iterator(*ಇದು, ಸುಳ್ಳು); }
File_ಗಂಟು_Iterator File_ಗಂಟು::end() ಸ್ಥಿರಾಂಕ   { ಹಿಂತಿರುಗಿ File_ಗಂಟು_Iterator(*ಇದು, ನಿಜ); }

ಶೂನ್ಯ File_ಗಂಟು::readRaw( ಸ್ಥಿರಾಂಕ std::string& fmt, ಶೂನ್ಯ* vec, size_t len ) ಸ್ಥಿರಾಂಕ
{
    File_ಗಂಟು_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_ಗಂಟು::size() ಸ್ಥಿರಾಂಕ
{
    ಸ್ಥಿರಾಂಕ uchar* p = ptr();
    ಆದರೆ( !p )
        ಹಿಂತಿರುಗಿ 0;
    ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗ್ = *p;
    ಪೂರ್ಣಾಂಕ tp = ಟ್ಯಾಗ್ & TYPE_ಮುಖವಾಡ;
    ಆದರೆ( tp == MAP || tp == SEQ )
    {
        ಆದರೆ( ಟ್ಯಾಗ್ & NAMED )
            p += 4;
        ಹಿಂತಿರುಗಿ (size_t)(ಚಿಹ್ನೆರಹಿತ)readInt(p + 5);
    }
    ಹಿಂತಿರುಗಿ tp != NONE;
}

size_t File_ಗಂಟು::raw_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{
    ಸ್ಥಿರಾಂಕ uchar* p0 = ptr(), *p = p0;
    ಆದರೆ( !p )
        ಹಿಂತಿರುಗಿ 0;
    ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗ್ = *p++;
    ಪೂರ್ಣಾಂಕ tp = ಟ್ಯಾಗ್ & TYPE_ಮುಖವಾಡ;
    ಆದರೆ( ಟ್ಯಾಗ್ & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    ಆದರೆ( tp == INT )
        ಹಿಂತಿರುಗಿ sz0 + 8;
    ಆದರೆ( tp == REAL )
        ಹಿಂತಿರುಗಿ sz0 + 8;
    ಆದರೆ( tp == NONE )
        ಹಿಂತಿರುಗಿ sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    ಹಿಂತಿರುಗಿ sz0 + 4 + readInt(p);
}

uchar* File_ಗಂಟು::ptr()
{
    ಹಿಂತಿರುಗಿ !fs ? 0 : (uchar*)fs->get_ಗಂಟು_Ptr(ಖಂಡ_Idx, ofs);
}

ಸ್ಥಿರಾಂಕ uchar* File_ಗಂಟು::ptr() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ !fs ? 0 : fs->get_ಗಂಟು_Ptr(ಖಂಡ_Idx, ofs);
}

ಶೂನ್ಯ File_ಗಂಟು::ಗಣ_ಮೌಲ್ಯ( ಪೂರ್ಣಾಂಕ type, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* value, ಪೂರ್ಣಾಂಕ len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗ್ = *p;
    ಪೂರ್ಣಾಂಕ current_type = ಟ್ಯಾಗ್ & TYPE_ಮುಖವಾಡ;
    CV_Assert( current_type == NONE || current_type == type );

    ಪೂರ್ಣಾಂಕ sz = 1;

    ಆದರೆ( ಟ್ಯಾಗ್ & NAMED )
        sz += 4;

    ಆದರೆ( type == INT )
        sz += 8;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == REAL )
        sz += 8;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == STRING )
    {
        ಆದರೆ( len < 0 )
            len = (ಪೂರ್ಣಾಂಕ)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_ಗಂಟು_Space(*ಇದು, sz);
    *p++ = (uchar)(type | (ಟ್ಯಾಗ್ & NAMED));
    ಆದರೆ( ಟ್ಯಾಗ್ & NAMED )
        p += 4;

    ಆದರೆ( type == INT )
    {
        int64_t ival = *(ಸ್ಥಿರಾಂಕ int64_t*)value;
        writeInt(p, ival);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == REAL )
    {
        ದ್ವಿಗುಣ dbval = *(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)value;
        writeReal(p, dbval);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == STRING )
    {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* str = (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*)value;
        writeInt(p, len + 1);
        ಸ್ಮರಣೆ_ನಕಲು(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_ಗಂಟು_Iterator::File_ಗಂಟು_Iterator()
{
    fs = 0;
    ಖಂಡ_Idx = 0;
    ofs = 0;
    ಖಂಡ_ಗಾತ್ರ_2 = 0;
    ಗಂಟು_NElems = 0;
    idx = 0;
}

File_ಗಂಟು_Iterator::File_ಗಂಟು_Iterator( ಸ್ಥಿರಾಂಕ File_ಗಂಟು& ಗಂಟು, ತಾರ್ಕಿಕ seek_ಕೊನೆ )
{
    fs = ಗಂಟು.fs;
    idx = 0;
    ಆದರೆ( !fs )
        ಖಂಡ_Idx = ofs = ಖಂಡ_ಗಾತ್ರ_2 = ಗಂಟು_NElems = 0;
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಖಂಡ_Idx = ಗಂಟು.ಖಂಡ_Idx;
        ofs = ಗಂಟು.ofs;

        ತಾರ್ಕಿಕ collection = ಗಂಟು.isSeq() || ಗಂಟು.isMap();
        ಆದರೆ( ಗಂಟು.isNone() )
        {
            ಗಂಟು_NElems = 0;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( !collection )
        {
            ಗಂಟು_NElems = 1;
            ಆದರೆ( seek_ಕೊನೆ )
            {
                idx = 1;
                ofs += ಗಂಟು.raw_ಗಾತ್ರ();
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಗಂಟು_NElems = ಗಂಟು.size();
            ಸ್ಥಿರಾಂಕ uchar* p0 = ಗಂಟು.ptr(), *p = p0 + 1;
            ಆದರೆ(*p0 & File_ಗಂಟು::NAMED )
                p += 4;
            ಆದರೆ( !seek_ಕೊನೆ )
                ofs += (p - p0) + 8;
            ಇಲ್ಲದಿದ್ದರೆ
            {
                size_t rawsz = (size_t)(ಚಿಹ್ನೆರಹಿತ)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = ಗಂಟು_NElems;
            }
        }
        fs->normalize_ಗಂಟು_Ofs(ಖಂಡ_Idx, ofs);
        ಖಂಡ_ಗಾತ್ರ_2 = fs->fs_ದತ್ತಾಂಶ_blksz[ಖಂಡ_Idx];
    }
}

File_ಗಂಟು_Iterator::File_ಗಂಟು_Iterator(ಸ್ಥಿರಾಂಕ File_ಗಂಟು_Iterator& it)
{
    fs = it.fs;
    ಖಂಡ_Idx = it.ಖಂಡ_Idx;
    ofs = it.ofs;
    ಖಂಡ_ಗಾತ್ರ_2 = it.ಖಂಡ_ಗಾತ್ರ_2;
    ಗಂಟು_NElems = it.ಗಂಟು_NElems;
    idx = it.idx;
}

File_ಗಂಟು_Iterator& File_ಗಂಟು_Iterator::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ File_ಗಂಟು_Iterator& it)
{
    fs = it.fs;
    ಖಂಡ_Idx = it.ಖಂಡ_Idx;
    ofs = it.ofs;
    ಖಂಡ_ಗಾತ್ರ_2 = it.ಖಂಡ_ಗಾತ್ರ_2;
    ಗಂಟು_NElems = it.ಗಂಟು_NElems;
    idx = it.idx;
    ಹಿಂತಿರುಗಿ *ಇದು;
}

File_ಗಂಟು File_ಗಂಟು_Iterator::ಕ್ರಿಯಾಕಾರಕ *() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ File_ಗಂಟು(idx < ಗಂಟು_NElems ? fs : ಖಾಲಿ, ಖಂಡ_Idx, ofs);
}

File_ಗಂಟು_Iterator& File_ಗಂಟು_Iterator::ಕ್ರಿಯಾಕಾರಕ ++ ()
{
    ಆದರೆ( idx == ಗಂಟು_NElems || !fs )
        ಹಿಂತಿರುಗಿ *ಇದು;
    idx++;
    File_ಗಂಟು n(fs, ಖಂಡ_Idx, ofs);
    ofs += n.raw_ಗಾತ್ರ();
    ಆದರೆ( ofs >= ಖಂಡ_ಗಾತ್ರ_2 )
    {
        fs->normalize_ಗಂಟು_Ofs(ಖಂಡ_Idx, ofs);
        ಖಂಡ_ಗಾತ್ರ_2 = fs->fs_ದತ್ತಾಂಶ_blksz[ಖಂಡ_Idx];
    }
    ಹಿಂತಿರುಗಿ *ಇದು;
}

File_ಗಂಟು_Iterator File_ಗಂಟು_Iterator::ಕ್ರಿಯಾಕಾರಕ ++ (ಪೂರ್ಣಾಂಕ)
{
    File_ಗಂಟು_Iterator it = *ಇದು;
    ++(*ಇದು);
    ಹಿಂತಿರುಗಿ it;
}

File_ಗಂಟು_Iterator& File_ಗಂಟು_Iterator::ಕ್ರಿಯಾಕಾರಕ += (ಪೂರ್ಣಾಂಕ _ofs)
{
    CV_Assert( _ofs >= 0 );
    ಗಾಗಿ( ; _ofs > 0; _ofs-- )
        ಇದು->ಕ್ರಿಯಾಕಾರಕ ++();
    ಹಿಂತಿರುಗಿ *ಇದು;
}

File_ಗಂಟು_Iterator& File_ಗಂಟು_Iterator::readRaw( ಸ್ಥಿರಾಂಕ String& fmt, ಶೂನ್ಯ* _data0, size_t maxsz)
{
    ಆದರೆ( fs && idx < ಗಂಟು_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        ಪೂರ್ಣಾಂಕ fmt_ಜೋಡಿಗಳು[CV_FS_MAX_FMT_ಜೋಡಿಗಳು*2];
        ಪೂರ್ಣಾಂಕ fmt_pair_ಎಣಿಕೆ = fs::decodeFormat( fmt.c_str(), fmt_ಜೋಡಿಗಳು, CV_FS_MAX_FMT_ಜೋಡಿಗಳು );
        size_t esz = fs::calc_Struct_ಗಾತ್ರ( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        ಗಾಗಿ( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < fmt_pair_ಎಣಿಕೆ; k++ )
            {
                ಪೂರ್ಣಾಂಕ elem_type = fmt_ಜೋಡಿಗಳು[k*2+1];
                ಪೂರ್ಣಾಂಕ elem_ಗಾತ್ರ_2 = CV_ELEM_ಗಾತ್ರ(elem_type);

                ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 = fmt_ಜೋಡಿಗಳು[k*2];
                offset = align_ಗಾತ್ರ( offset, elem_ಗಾತ್ರ_2 );
                uchar* ದತ್ತಾಂಶ = data0 + offset;

                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < ಎಣಿಕೆ_3; i++, ++(*ಇದು) )
                {
                    File_ಗಂಟು ಗಂಟು = *(*ಇದು);
                    ಆದರೆ( ಗಂಟು.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_ಗಾತ್ರ_2 == 8 ? (int64_t)ಗಂಟು : (ಪೂರ್ಣಾಂಕ)ಗಂಟು);
                        ಆಯ್ಕೆ( elem_type )
                        {
                        ಸ್ಥಿತಿ CV_8U:
                            *(uchar*)ದತ್ತಾಂಶ = saturate_cast<uchar>(ival);
                            ದತ್ತಾಂಶ++;
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_8S:
                            *(ಅಕ್ಷರ*)ದತ್ತಾಂಶ = saturate_cast<schar>(ival);
                            ದತ್ತಾಂಶ++;
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_Bool:
                            *(ತಾರ್ಕಿಕ*)ದತ್ತಾಂಶ = ival != 0;
                            ದತ್ತಾಂಶ++;
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_16U:
                            *(ushort*)ದತ್ತಾಂಶ = saturate_cast<ushort>(ival);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(ushort);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_16S:
                            *(ಗಿಡ್ಡ*)ದತ್ತಾಂಶ = saturate_cast<ಗಿಡ್ಡ>(ival);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(ಗಿಡ್ಡ);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_32U:
                            *(ಚಿಹ್ನೆರಹಿತ*)ದತ್ತಾಂಶ = (ಚಿಹ್ನೆರಹಿತ)std::max(ival, (int64_t)0);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_32S:
                            *(ಪೂರ್ಣಾಂಕ*)ದತ್ತಾಂಶ = (ಪೂರ್ಣಾಂಕ)ival;
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_32F:
                            *(ದಶಮಾಂಶ*)ದತ್ತಾಂಶ = (ದಶಮಾಂಶ)ival;
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(ದಶಮಾಂಶ);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_64U:
                            *(uint64_t*)ದತ್ತಾಂಶ = (uint64_t)ival;
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(uint64_t);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_64S:
                            *(int64_t*)ದತ್ತಾಂಶ = (int64_t)ival;
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(int64_t);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_64F:
                            *(ದ್ವಿಗುಣ*)ದತ್ತಾಂಶ = (ದ್ವಿಗುಣ)ival;
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(ದ್ವಿಗುಣ);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_16F:
                            *(hfloat*)ದತ್ತಾಂಶ = hfloat((ದಶಮಾಂಶ)ival);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(hfloat);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_16BF:
                            *(bfloat*)ದತ್ತಾಂಶ = bfloat((ದಶಮಾಂಶ)ival);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(bfloat);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_8F_E4M3FN:
                            *(fp8_t*)ದತ್ತಾಂಶ = fp8_t((ದಶಮಾಂಶ)ival);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(fp8_t);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_8F_E4M3FNUZ:
                            *(fp8a_t*)ದತ್ತಾಂಶ = fp8a_t((ದಶಮಾಂಶ)ival);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(fp8a_t);
                            ನಿಲ್ಲಿಸು;
                        ಪೂರ್ವನಿಯೋಜಿತ:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಗಂಟು.isReal() )
                    {
                        ದ್ವಿಗುಣ fval = (ದ್ವಿಗುಣ)ಗಂಟು;

                        ಆಯ್ಕೆ( elem_type )
                        {
                        ಸ್ಥಿತಿ CV_8U:
                            *(uchar*)ದತ್ತಾಂಶ = saturate_cast<uchar>(fval);
                            ದತ್ತಾಂಶ++;
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_8S:
                            *(ಅಕ್ಷರ*)ದತ್ತಾಂಶ = saturate_cast<schar>(fval);
                            ದತ್ತಾಂಶ++;
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_16U:
                            *(ushort*)ದತ್ತಾಂಶ = saturate_cast<ushort>(fval);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(ushort);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_16S:
                            *(ಗಿಡ್ಡ*)ದತ್ತಾಂಶ = saturate_cast<ಗಿಡ್ಡ>(fval);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(ಗಿಡ್ಡ);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_32U:
                            *(ಪೂರ್ಣಾಂಕ*)ದತ್ತಾಂಶ = saturate_cast<ಚಿಹ್ನೆರಹಿತ>(fval);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_32S:
                            *(ಪೂರ್ಣಾಂಕ*)ದತ್ತಾಂಶ = saturate_cast<ಪೂರ್ಣಾಂಕ>(fval);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_32F:
                            *(ದಶಮಾಂಶ*)ದತ್ತಾಂಶ = (ದಶಮಾಂಶ)fval;
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(ದಶಮಾಂಶ);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_64U:
                            *(uint64_t*)ದತ್ತಾಂಶ = (uint64_t)round(std::max(fval, 0.));
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(uint64_t);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_64S:
                            *(int64_t*)ದತ್ತಾಂಶ = (int64_t)round(std::max(fval, 0.));
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(int64_t);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_64F:
                            *(ದ್ವಿಗುಣ*)ದತ್ತಾಂಶ = fval;
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(ದ್ವಿಗುಣ);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_16F:
                            *(hfloat*)ದತ್ತಾಂಶ = hfloat((ದಶಮಾಂಶ)fval);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(hfloat);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_16BF:
                            *(bfloat*)ದತ್ತಾಂಶ = bfloat((ದಶಮಾಂಶ)fval);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(bfloat);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_8F_E4M3FN:
                            *(fp8_t*)ದತ್ತಾಂಶ = fp8_t((ದಶಮಾಂಶ)fval);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(fp8_t);
                            ನಿಲ್ಲಿಸು;
                        ಸ್ಥಿತಿ CV_8F_E4M3FNUZ:
                            *(fp8a_t*)ದತ್ತಾಂಶ = fp8a_t((ದಶಮಾಂಶ)fval);
                            ದತ್ತಾಂಶ += ಗಾತ್ರ(fp8a_t);
                            ನಿಲ್ಲಿಸು;
                        ಪೂರ್ವನಿಯೋಜಿತ:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (ಪೂರ್ಣಾಂಕ)(ದತ್ತಾಂಶ - data0);
            }
        }
    }

    ಹಿಂತಿರುಗಿ *ಇದು;
}

ತಾರ್ಕಿಕ File_ಗಂಟು_Iterator::equal_ಗಮ್ಯ(ಸ್ಥಿರಾಂಕ File_ಗಂಟು_Iterator& it) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ fs == it.fs && ಖಂಡ_Idx == it.ಖಂಡ_Idx && ofs == it.ofs &&
           idx == it.idx && ಗಂಟು_NElems == it.ಗಂಟು_NElems;
}

size_t File_ಗಂಟು_Iterator::ಉಳಿದ() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ ಗಂಟು_NElems - idx;
}

ತಾರ್ಕಿಕ ಕ್ರಿಯಾಕಾರಕ == ( ಸ್ಥಿರಾಂಕ File_ಗಂಟು_Iterator& it1, ಸ್ಥಿರಾಂಕ File_ಗಂಟು_Iterator& it2 )
{
    ಹಿಂತಿರುಗಿ it1.equal_ಗಮ್ಯ(it2);
}

ತಾರ್ಕಿಕ ಕ್ರಿಯಾಕಾರಕ != ( ಸ್ಥಿರಾಂಕ File_ಗಂಟು_Iterator& it1, ಸ್ಥಿರಾಂಕ File_ಗಂಟು_Iterator& it2 )
{
    ಹಿಂತಿರುಗಿ !it1.equal_ಗಮ್ಯ(it2);
}

ಶೂನ್ಯ ಪಠ(ಸ್ಥಿರಾಂಕ File_ಗಂಟು& ಗಂಟು, ಪೂರ್ಣಾಂಕ& val, ಪೂರ್ಣಾಂಕ default_val)
{
    val = default_val;
    ಆದರೆ( !ಗಂಟು.empty() )
    {
        val = (ಪೂರ್ಣಾಂಕ)ಗಂಟು;
    }
}

ಶೂನ್ಯ ಪಠ(ಸ್ಥಿರಾಂಕ File_ಗಂಟು& ಗಂಟು, int64_t& val, int64_t default_val)
{
    val = default_val;
    ಆದರೆ( !ಗಂಟು.empty() )
    {
        val = (int64_t)ಗಂಟು;
    }
}

ಶೂನ್ಯ ಪಠ(ಸ್ಥಿರಾಂಕ File_ಗಂಟು& ಗಂಟು, ದ್ವಿಗುಣ& val, ದ್ವಿಗುಣ default_val)
{
    val = default_val;
    ಆದರೆ( !ಗಂಟು.empty() )
    {
        val = (ದ್ವಿಗುಣ)ಗಂಟು;
    }
}

ಶೂನ್ಯ ಪಠ(ಸ್ಥಿರಾಂಕ File_ಗಂಟು& ಗಂಟು, ದಶಮಾಂಶ& val, ದಶಮಾಂಶ default_val)
{
    val = default_val;
    ಆದರೆ( !ಗಂಟು.empty() )
    {
        val = (ದಶಮಾಂಶ)ಗಂಟು;
    }
}

ಶೂನ್ಯ ಪಠ(ಸ್ಥಿರಾಂಕ File_ಗಂಟು& ಗಂಟು, std::string& val, ಸ್ಥಿರಾಂಕ std::string& default_val)
{
    val = default_val;
    ಆದರೆ( !ಗಂಟು.empty() )
    {
        val = (std::string)ಗಂಟು;
    }
}
ಶೂನ್ಯ FileStorage::Impl::ಲಿಖ(ಸ್ಥಿರಾಂಕ String &ಕೀಲಿ, ತಾರ್ಕಿಕ value)
{
    CV_Assert(write_ಬಹುಲಕ);
    ಆದರೆ (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().ಲಿಖ(ಕೀಲಿ.c_str(), (ಪೂರ್ಣಾಂಕ)value);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().ಲಿಖ(ಕೀಲಿ.c_str(), value ? "true" : "false", ಸುಳ್ಳು);
    }
}

FileStorage_API::~FileStorage_API() {}

ನಾಮಾಕಾಶ internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, ಸ್ಥಿರಾಂಕ std::string& name,
                                       ಪೂರ್ಣಾಂಕ flags, ಸ್ಥಿರಾಂಕ std::string& typeName)
{
    fs = &_fs;
    fs->ಆರಂಭ_Write_Struct(name, flags, typeName);
}

WriteStructContext::~WriteStructContext()
{
    fs->ಕೊನೆ_Write_Struct();
}

}

}
