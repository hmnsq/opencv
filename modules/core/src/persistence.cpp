// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "persistence.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "persistence_impl.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "persistence_base64_encoding.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <unordered_map>
#ਅਨ੍ਤਰ੍ਭਾਵ <iterator>

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.hpp>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

ਨਾਮਾਕਾਸ਼ cv
{

ਨਾਮਾਕਾਸ਼ fs
{

ਪੂਰਨ strcasecmp(ਅਚਲ ਅੱਖਰ* s1, ਅਚਲ ਅੱਖਰ* s2)
{
    ਅਚਲ ਅੱਖਰ* dummy="";
    ਜੇ(!s1) s1=dummy;
    ਜੇ(!s2) s2=dummy;

    size_t len1 = ਲੜੀ_ਲੰਬਾਈ(s1);
    size_t len2 = ਲੜੀ_ਲੰਬਾਈ(s2);
    size_t i, len = std::min(len1, len2);
    ਲਈ( i = 0; i < len; i++ )
    {
        ਪੂਰਨ d = tolower((ਪੂਰਨ)s1[i]) - tolower((ਪੂਰਨ)s2[i]);
        ਜੇ( d != 0 )
            ਵਾਪਸ d;
    }
    ਵਾਪਸ len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

ਅੱਖਰ* itoa( ਪੂਰਨ _val, ਅੱਖਰ* ਬਫਰ, ਪੂਰਨ /*radix*/ )
{
    ਅਚਲ ਪੂਰਨ radix = 10;
    ਅੱਖਰ* ptr=ਬਫਰ + 23 /* enough even for 64-bit integers */;
    ਬਿਨਾਨਿਸ਼ਾਨੀ val = ਨਿਰਪੇਖ(_val);

    *ptr = '\0';
    ਕਰੋ
    {
        ਬਿਨਾਨਿਸ਼ਾਨੀ r = val / radix;
        *--ptr = (ਅੱਖਰ)(val - (r*radix) + '0');
        val = r;
    }
    ਜਦੋਂ( val != 0 );

    ਜੇ( _val < 0 )
        *--ptr = '-';

    ਵਾਪਸ ptr;
}

ਅੱਖਰ* itoa( int64_t _val, ਅੱਖਰ* ਬਫਰ, ਪੂਰਨ /*radix*/, ਤਾਰਕਿਕ _signed)
{
    ਅਚਲ ਪੂਰਨ radix = 10;
    ਅੱਖਰ* ptr=ਬਫਰ + 23 /* enough even for 64-bit integers */;
    ਪੂਰਨ sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : ਨਿਰਪੇਖ(_val);

    *ptr = '\0';
    ਕਰੋ
    {
        uint64_t r = val / radix;
        *--ptr = (ਅੱਖਰ)(val - (r*radix) + '0');
        val = r;
    }
    ਜਦੋਂ( val != 0 );

    ਜੇ( sign < 0 )
        *--ptr = '-';

    ਵਾਪਸ ptr;
}

ਅੱਖਰ* double_ਮੰਜ਼ਿਲ_String( ਅੱਖਰ* buf, size_t buf_ਮਾਪ, ਦੁੱਗਣਾ value, ਤਾਰਕਿਕ explicitZero )
{
    Cv64suf val;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ieee754_hi;

    val.f = value;
    ieee754_hi = (ਬਿਨਾਨਿਸ਼ਾਨੀ)(val.u >> 32);

    ਜੇ( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        ਪੂਰਨ ivalue = cv_ਗੇੜਾ(value);
        ਜੇ( ivalue == value )
        {
            ਜੇ( explicitZero )
                snprintf( buf, buf_ਮਾਪ, "%d.0", ivalue );
            ਨਹੀਂਤਾਂ
                snprintf( buf, buf_ਮਾਪ, "%d.", ivalue );
        }
        ਨਹੀਂਤਾਂ
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, buf_ਮਾਪ, "%.17g", value );

            ਅੱਖਰ* ptr = buf;
            ਜੇ( *ptr == '+' || *ptr == '-' )
                ptr++;
            ਲਈ( ; cv_isdigit(*ptr); ptr++ )
                ;
            ਜੇ( *ptr == ',' )
                *ptr = '.';
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ieee754_lo = (ਬਿਨਾਨਿਸ਼ਾਨੀ)val.u;
        ਜੇ( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        ਨਹੀਂਤਾਂ
            strcpy( buf, (ਪੂਰਨ)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    ਵਾਪਸ buf;
}

ਅੱਖਰ* float_ਮੰਜ਼ਿਲ_String( ਅੱਖਰ* buf, size_t buf_ਮਾਪ, ਦਸ਼ਮਲਵ value, ਤਾਰਕਿਕ halfprecision, ਤਾਰਕਿਕ explicitZero )
{
    Cv32suf val;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ieee754;
    val.f = value;
    ieee754 = val.u;

    ਜੇ( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        ਪੂਰਨ ivalue = cv_ਗੇੜਾ(value);
        ਜੇ( ivalue == value )
        {
            ਜੇ( explicitZero )
                snprintf( buf, buf_ਮਾਪ, "%d.0", ivalue );
            ਨਹੀਂਤਾਂ
                snprintf( buf, buf_ਮਾਪ, "%d.", ivalue );
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, buf_ਮਾਪ, "%.5g", value);
            }
            ਨਹੀਂਤਾਂ
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, buf_ਮਾਪ, "%.9g", value);
            }

            ਅੱਖਰ* ptr = buf;
            ਜੇ( *ptr == '+' || *ptr == '-' )
                ptr++;
            ਲਈ( ; cv_isdigit(*ptr); ptr++ )
                ;
            ਜੇ( *ptr == ',' )
                *ptr = '.';
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        ਨਹੀਂਤਾਂ
            strcpy( buf, (ਪੂਰਨ)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    ਵਾਪਸ buf;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ symbols[] = "ucwsifdhHbUIneE";

ਸਥਿਰ ਅੱਖਰ type_ਚਿੰਨ੍ਹ(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_Check_ਡੂੰਘਾਈ(ਡੂੰਘਾਈ, ਡੂੰਘਾਈ >= 0 && ਡੂੰਘਾਈ <= CV_8F_E4M3FNUZ, "");
    ਵਾਪਸ symbols[ਡੂੰਘਾਈ];
}

ਸਥਿਰ ਪੂਰਨ ਚਿੰਨ੍ਹ_ਮੰਜ਼ਿਲ_Type(ਅੱਖਰ c)
{
    ਜੇ (c == 'r')
        ਵਾਪਸ CV_SEQ_ELTYPE_PTR;
    ਅਚਲ ਅੱਖਰ* pos = strchr( symbols, c );
    ਜੇ( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    ਵਾਪਸ static_cast<ਪੂਰਨ>(pos - symbols);
}

ਅੱਖਰ* encodeFormat(ਪੂਰਨ elem_type, ਅੱਖਰ* dt, size_t dt_len)
{
    ਪੂਰਨ cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    ਅੱਖਰ ਚਿੰਨ੍ਹ = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_ਚਿੰਨ੍ਹ(CV_MAT_ਡੂੰਘਾਈ(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, ਚਿੰਨ੍ਹ);
    ਵਾਪਸ dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
ਅੱਖਰ* encodeFormat(ਪੂਰਨ elem_type, ਅੱਖਰ* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    ਵਾਪਸ encodeFormat(elem_type, dt, max);
}

ਪੂਰਨ decodeFormat( ਅਚਲ ਅੱਖਰ* dt, ਪੂਰਨ* fmt_ਜੋੜੇ, ਪੂਰਨ max_len )
{
    ਪੂਰਨ fmt_pair_ਗਿਣਤੀ = 0;
    ਪੂਰਨ i = 0, k = 0, len = dt ? (ਪੂਰਨ)ਲੜੀ_ਲੰਬਾਈ(dt) : 0;

    ਜੇ( !dt || !len )
        ਵਾਪਸ 0;

    CV_Assert( fmt_ਜੋੜੇ != 0 && max_len > 0 );
    fmt_ਜੋੜੇ[0] = 0;
    max_len *= 2;

    ਲਈ( ; k < len; k++ )
    {
        ਅੱਖਰ c = dt[k];

        ਜੇ( cv_isdigit(c) )
        {
            ਪੂਰਨ ਗਿਣਤੀ_3 = c - '0';
            ਜੇ( cv_isdigit(dt[k+1]) )
            {
                ਅੱਖਰ* endptr = 0;
                ਗਿਣਤੀ_3 = (ਪੂਰਨ)strtol( dt+k, &endptr, 10 );
                k = (ਪੂਰਨ)(endptr - dt) - 1;
            }

            ਜੇ( ਗਿਣਤੀ_3 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_ਜੋੜੇ[i] = ਗਿਣਤੀ_3;
        }
        ਨਹੀਂਤਾਂ
        {
            ਪੂਰਨ ਡੂੰਘਾਈ = ਚਿੰਨ੍ਹ_ਮੰਜ਼ਿਲ_Type(c);
            ਜੇ( fmt_ਜੋੜੇ[i] == 0 )
                fmt_ਜੋੜੇ[i] = 1;
            fmt_ਜੋੜੇ[i+1] = ਡੂੰਘਾਈ;
            ਜੇ( i > 0 && fmt_ਜੋੜੇ[i+1] == fmt_ਜੋੜੇ[i-1] )
                fmt_ਜੋੜੇ[i-2] += fmt_ਜੋੜੇ[i];
            ਨਹੀਂਤਾਂ
            {
                i += 2;
                ਜੇ( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_ਜੋੜੇ[i] = 0;
        }
    }

    fmt_pair_ਗਿਣਤੀ = i/2;
    ਵਾਪਸ fmt_pair_ਗਿਣਤੀ;
}

ਪੂਰਨ calc_Elem_ਮਾਪ( ਅਚਲ ਅੱਖਰ* dt, ਪੂਰਨ initial_ਮਾਪ )
{
    ਪੂਰਨ size = 0;
    ਪੂਰਨ fmt_ਜੋੜੇ[CV_FS_MAX_FMT_ਜੋੜੇ*2], i, fmt_pair_ਗਿਣਤੀ;
    ਪੂਰਨ comp_ਮਾਪ;

    fmt_pair_ਗਿਣਤੀ = decodeFormat( dt, fmt_ਜੋੜੇ, CV_FS_MAX_FMT_ਜੋੜੇ );
    fmt_pair_ਗਿਣਤੀ *= 2;
    ਲਈ( i = 0, size = initial_ਮਾਪ; i < fmt_pair_ਗਿਣਤੀ; i += 2 )
    {
        comp_ਮਾਪ = CV_ELEM_ਮਾਪ(fmt_ਜੋੜੇ[i+1]);
        size = cvAlign( size, comp_ਮਾਪ );
        size += comp_ਮਾਪ * fmt_ਜੋੜੇ[i];
    }
    ਜੇ( initial_ਮਾਪ == 0 )
    {
        comp_ਮਾਪ = CV_ELEM_ਮਾਪ(fmt_ਜੋੜੇ[1]);
        size = cvAlign( size, comp_ਮਾਪ );
    }
    ਵਾਪਸ size;
}


ਪੂਰਨ calc_Struct_ਮਾਪ( ਅਚਲ ਅੱਖਰ* dt, ਪੂਰਨ initial_ਮਾਪ )
{
    ਪੂਰਨ size = calc_Elem_ਮਾਪ( dt, initial_ਮਾਪ );
    size_t elem_max_ਮਾਪ = 0;
    ਲਈ ( ਅਚਲ ਅੱਖਰ * type = dt; *type != '\0'; type++ )
    {
        ਅੱਖਰ v = *type;
        ਜੇ (v >= '0' && v <= '9')
            ਜਾਰੀ;  // skip vector size
        ਚੋਣ (v)
        {
        ਹਾਲਤ 'u': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(uchar ) ); ਰੋਕੋ; }
        ਹਾਲਤ 'b': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(ਤਾਰਕਿਕ  ) ); ਰੋਕੋ; }
        ਹਾਲਤ 'c': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(schar ) ); ਰੋਕੋ; }
        ਹਾਲਤ 'w': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(ushort) ); ਰੋਕੋ; }
        ਹਾਲਤ 's': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(ਛੋਟਾ ) ); ਰੋਕੋ; }
        ਹਾਲਤ 'i': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(ਪੂਰਨ   ) ); ਰੋਕੋ; }
        ਹਾਲਤ 'n': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ) ); ਰੋਕੋ; }
        ਹਾਲਤ 'f': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(ਦਸ਼ਮਲਵ ) ); ਰੋਕੋ; }
        ਹਾਲਤ 'd': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(ਦੁੱਗਣਾ) ); ਰੋਕੋ; }
        ਹਾਲਤ 'h': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(hfloat)); ਰੋਕੋ; }
        ਹਾਲਤ 'H': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(bfloat)); ਰੋਕੋ; }
        ਹਾਲਤ 'e': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(fp8_t) ); ਰੋਕੋ; }
        ਹਾਲਤ 'E': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(fp8a_t)); ਰੋਕੋ; }
        ਹਾਲਤ 'I': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(int64_t)); ਰੋਕੋ; }
        ਹਾਲਤ 'U': { elem_max_ਮਾਪ = std::max( elem_max_ਮਾਪ, ਆਕਾਰ(uint64_t)); ਰੋਕੋ; }
        ਮੂਲ:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (ਅੱਖਰ)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<ਪੂਰਨ>(elem_max_ਮਾਪ) );
    ਵਾਪਸ size;
}

ਪੂਰਨ decodeSimpleFormat( ਅਚਲ ਅੱਖਰ* dt )
{
    ਪੂਰਨ elem_type = -1;
    ਪੂਰਨ fmt_ਜੋੜੇ[CV_FS_MAX_FMT_ਜੋੜੇ*2], fmt_pair_ਗਿਣਤੀ;

    fmt_pair_ਗਿਣਤੀ = decodeFormat( dt, fmt_ਜੋੜੇ, CV_FS_MAX_FMT_ਜੋੜੇ );
    ਜੇ( fmt_pair_ਗਿਣਤੀ != 1 || fmt_ਜੋੜੇ[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_ਜੋੜੇ[1], fmt_ਜੋੜੇ[0] );

    ਵਾਪਸ elem_type;
}

}

#ਜੇ defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LITTLE_ENDIAN_MEM_ACCESS 1
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LITTLE_ENDIAN_MEM_ACCESS 0
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ readInt(ਅਚਲ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ਜੇ CV_LITTLE_ENDIAN_MEM_ACCESS
    ਪੂਰਨ val;
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(&val, p, ਆਕਾਰ(val));
    ਵਾਪਸ val;
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ val = (ਪੂਰਨ)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    ਵਾਪਸ val;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ int64_t readLong(ਅਚਲ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ਜੇ CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(&val, p, ਆਕਾਰ(val));
    ਵਾਪਸ val;
#ਨਹੀਂਤਾਂ
    ਬਿਨਾਨਿਸ਼ਾਨੀ val0 = (ਬਿਨਾਨਿਸ਼ਾਨੀ)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    ਬਿਨਾਨਿਸ਼ਾਨੀ val1 = (ਬਿਨਾਨਿਸ਼ਾਨੀ)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    ਵਾਪਸ val0 | ((int64_t)val1 << 32);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ readReal(ਅਚਲ uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ਜੇ CV_LITTLE_ENDIAN_MEM_ACCESS
    ਦੁੱਗਣਾ val;
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(&val, p, ਆਕਾਰ(val));
    ਵਾਪਸ val;
#ਨਹੀਂਤਾਂ
    ਬਿਨਾਨਿਸ਼ਾਨੀ val0 = (ਬਿਨਾਨਿਸ਼ਾਨੀ)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    ਬਿਨਾਨਿਸ਼ਾਨੀ val1 = (ਬਿਨਾਨਿਸ਼ਾਨੀ)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    ਵਾਪਸ val.f;
#ਜੇ_ਅੰਤ
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ਜੇ CV_LITTLE_ENDIAN_MEM_ACCESS
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(p, &ival, ਆਕਾਰ(ival));
#ਨਹੀਂਤਾਂ
    ਲਈ (size_t i = 0, j = 0; i < ਆਕਾਰ(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ writeReal(uchar* p, ਦੁੱਗਣਾ fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ਜੇ CV_LITTLE_ENDIAN_MEM_ACCESS
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(p, &fval, ਆਕਾਰ(fval));
#ਨਹੀਂਤਾਂ
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
#ਜੇ_ਅੰਤ
}



ਖਾਲੀ FileStorage::Impl::init() {
    flags = 0;
    ਬਫਰ.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = ਝੂਠ;
    state_ਦਾ_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = ਝੂਠ;
    delayed_struct_ਕੁੰਜੀ = ਖਾਲੀ_ਸੂਚਕ;
    delayed_struct_flags = 0;
    delayed_type_name = ਖਾਲੀ_ਸੂਚਕ;
    base64_writer = ਖਾਲੀ_ਸੂਚਕ;
    is_opened = ਝੂਠ;
    dummy_eof = ਝੂਠ;
    write_ਬਹੁਲਕ = ਝੂਠ;
    mem_ਬਹੁਲਕ = ਝੂਠ;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    ਖਾਲੀ_ਥਾਵਾਂ_ਧਾਰਾ = ਸੱਚ;

    strbufv.clear();
    strbuf = 0;
    strbufsize = strbufpos = 0;
    roots.clear();

    fs_ਅੰਕੜੇ.clear();
    fs_ਅੰਕੜੇ_ptrs.clear();
    fs_ਅੰਕੜੇ_blksz.clear();
    freeSpaceOfs = 0;

    str_hash.clear();
    str_hash_ਅੰਕੜੇ.clear();
    str_hash_ਅੰਕੜੇ.resize(1);
    str_hash_ਅੰਕੜੇ[0] = '\0';

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

ਖਾਲੀ FileStorage::Impl::release(String *out) {
    ਜੇ (is_opened) {
        ਜੇ (out)
            out->clear();
        ਜੇ (write_ਬਹੁਲਕ) {
            ਜਦੋਂ (write_ਢੇਰ.size() > 1) {
                ਸਿਰਾ_Write_Struct();
            }
            flush();
            ਜੇ (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            ਨਹੀਂਤਾਂ ਜੇ (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        ਜੇ (mem_ਬਹੁਲਕ && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

ਖਾਲੀ FileStorage::Impl::analyze_file_name(ਅਚਲ std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ not_file_name = '\n';
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ parameter_begin = '?';
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ parameter_separator = '&';

    ਜੇ (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        ਵਾਪਸ;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    ਜੇ (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        ਲਈ (size_t param_beg = beg, param_ਸਿਰਾ = beg;
             param_ਸਿਰਾ < end;
             param_beg = param_ਸਿਰਾ + 1) {
            param_ਸਿਰਾ = file_name.find_first_of(parameter_separator, param_beg);
            ਜੇ ((param_ਸਿਰਾ == std::string::npos || param_ਸਿਰਾ != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_ਸਿਰਾ - param_beg));
            }
        }
    }
}

ਤਾਰਕਿਕ FileStorage::Impl::open(ਅਚਲ ਅੱਖਰ *filename_or_buf, ਪੂਰਨ _flags, ਅਚਲ ਅੱਖਰ *encoding) {
    ਤਾਰਕਿਕ ok = ਸੱਚ;
    release();

    ਤਾਰਕਿਕ append = (_flags & 3) == FileStorage::APPEND;
    mem_ਬਹੁਲਕ = (_flags & FileStorage::ਸਮ੍ਰਿਤੀ) != 0;

    write_ਬਹੁਲਕ = (_flags & 3) != 0;
    ਤਾਰਕਿਕ write_base64 = (write_ਬਹੁਲਕ || append) && (_flags & FileStorage::BASE64) != 0;

    ਤਾਰਕਿਕ isGZ = ਝੂਠ;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        ਜੇ (!params.empty())
            filename = params[0];

        ਜੇ (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_ਬਹੁਲਕ || append);
    }

    ਜੇ (filename.size() == 0 && !mem_ਬਹੁਲਕ && !write_ਬਹੁਲਕ)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    ਜੇ (mem_ਬਹੁਲਕ && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    ਜੇ (!mem_ਬਹੁਲਕ) {
        size_t dot_idx = filename.find_last_of('.');
        ਅੱਖਰ compression = '\0';

        ਜੇ (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            ਜੇ (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                ਜੇ (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = ਸੱਚ;
                }

                ਨਹੀਂਤਾਂ ਜੇ(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = ਸੱਚ;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        ਜੇ (!isGZ) {
            file = fopen(filename.c_str(), !write_ਬਹੁਲਕ ? "rt" : !append ? "wt" : "a+t");
            ਜੇ (!file)
            {
                CV_LOG_ERROR(ਨਿਰਾ, "Can't open file: '" << filename << "' in " << (!write_ਬਹੁਲਕ ? "read" : !append ? "write" : "append") << " mode");
                ਵਾਪਸ ਝੂਠ;
            }
        } ਨਹੀਂਤਾਂ {
#ਜੇ USE_ZLIB
            ਜੇ (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            ਅੱਖਰ ਬਹੁਲਕ[] = {write_ਬਹੁਲਕ ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), ਬਹੁਲਕ);
            ਜੇ (!gzfile)
            {
                CV_LOG_ERROR(ਨਿਰਾ, "Can't open archive: '" << filename << "' mode=" << ਬਹੁਲਕ);
                ਵਾਪਸ ਝੂਠ;
            }
#ਨਹੀਂਤਾਂ
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#ਜੇ_ਅੰਤ
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_ਅੰਕੜੇ.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    ਜੇ (write_ਬਹੁਲਕ) {
        fmt = flags & FileStorage::FORMAT_ਨਕਾਬ;

        ਜੇ (mem_ਬਹੁਲਕ)
            outbuf.clear();

        ਜੇ (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            ਅਚਲ ਅੱਖਰ *dot_pos = ਨਿਰਾ;
            ਅਚਲ ਅੱਖਰ *dot_pos2 = ਨਿਰਾ;
            // like strrchr() implementation, but save two last positions simultaneously
            ਲਈ (ਅਚਲ ਅੱਖਰ *pos = &filename[0]; pos[0] != 0; pos++) {
                ਜੇ (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            ਜੇ (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != ਨਿਰਾ) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } ਨਹੀਂਤਾਂ ਜੇ (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        ਪੂਰਨ buf_ਮਾਪ_2 = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        ਜੇ (append) {
            fseek(file, 0, SEEK_END);
            ਜੇ (ftell(file) == 0)
                append = ਝੂਠ;
        }

        write_ਢੇਰ.clear();
        ਖਾਲੀ_ਥਾਵਾਂ_ਧਾਰਾ = ਸੱਚ;
        write_ਢੇਰ.push_back(FStruct_ਅੰਕੜੇ("", File_ਗੰਢ::MAP | File_ਗੰਢ::ਖਾਲੀ_ਥਾਵਾਂ_2, 0));
        ਬਫਰ.reserve(buf_ਮਾਪ_2 + 1024);
        ਬਫਰ.resize(buf_ਮਾਪ_2);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_ਦਾ_writing_base64 = FileStorage_API::Base64State::Uncertain;

        ਜੇ (fmt == FileStorage::FORMAT_XML) {
            size_t file_ਮਾਪ_2 = file ? (size_t) ftell(file) : (size_t) 0;
            ਜੇ (!append || file_ਮਾਪ_2 == 0) {
                ਜੇ (encoding && *encoding != '\0') {
                    ਜੇ (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(ਲੜੀ_ਲੰਬਾਈ(encoding) < 1000);
                    ਅੱਖਰ buf[1100];
                    snprintf(buf, ਆਕਾਰ(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } ਨਹੀਂਤਾਂ
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } ਨਹੀਂਤਾਂ {
                ਪੂਰਨ xml_buf_ਮਾਪ = 1 << 10;
                ਅੱਖਰ substr[] = "</opencv_storage>";
                ਪੂਰਨ ਆਖਰੀ_occurrence = -1;
                xml_buf_ਮਾਪ = MIN(xml_buf_ਮਾਪ, ਪੂਰਨ(file_ਮਾਪ_2));
                fseek(file, -xml_buf_ਮਾਪ, SEEK_END);
                // find the last occurrence of </opencv_storage>
                ਲਈ (;;) {
                    ਪੂਰਨ ਸਤਰ_ਵਿੱਥ = (ਪੂਰਨ) ftell(file);
                    ਅਚਲ ਅੱਖਰ *ptr0 = ਇਹ->gets(xml_buf_ਮਾਪ);
                    ਅਚਲ ਅੱਖਰ *ptr = ਨਿਰਾ;
                    ਜੇ (!ptr0)
                        ਰੋਕੋ;
                    ptr = ptr0;
                    ਲਈ (;;) {
                        ptr = ਲੜੀ_ਖੋਜ(ptr, substr);
                        ਜੇ (!ptr)
                            ਰੋਕੋ;
                        ਆਖਰੀ_occurrence = ਸਤਰ_ਵਿੱਥ + (ਪੂਰਨ) (ptr - ptr0);
                        ptr += ਲੜੀ_ਲੰਬਾਈ(substr);
                    }
                }
                ਜੇ (ਆਖਰੀ_occurrence < 0) {
                    release();
                    CV_Error(cv::Error::StsError, "Could not find </opencv_storage> in the end of file.\n");
                }
                closeFile();
                file = fopen(filename.c_str(), "r+t");
                CV_Assert(file != 0);
                fseek(file, ਆਖਰੀ_occurrence, SEEK_SET);
                // replace the last "</opencv_storage>" with " <!-- resumed -->", which has the same length
                puts(" <!-- resumed -->");
                fseek(file, 0, SEEK_END);
                puts("\n");
            }

            emitter_do_not_use_ਸਿੱਧਾ_dereference = createXMLEmitter(ਇਹ);
        } ਨਹੀਂਤਾਂ ਜੇ (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                ਜੇ (!append) {
                    ਜੇ (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    ਨਹੀਂਤਾਂ
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            ਨਹੀਂਤਾਂ
                puts("...\n---\n");

            emitter_do_not_use_ਸਿੱਧਾ_dereference = createYAMLEmitter(ਇਹ);
        } ਨਹੀਂਤਾਂ {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            ਜੇ (!append)
                puts("{\n");
            ਨਹੀਂਤਾਂ {
                ਤਾਰਕਿਕ valid = ਝੂਠ;
                ਲੰਮਾ roffset = 0;
                ਲਈ (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    ਅਚਲ ਅੱਖਰ ਸਿਰਾ_mark = '}';
                    ਜੇ (fgetc(file) == ਸਿਰਾ_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = ਸੱਚ;
                        ਰੋਕੋ;
                    }
                }

                ਜੇ (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } ਨਹੀਂਤਾਂ {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_ਢੇਰ.back().indent = 4;
            emitter_do_not_use_ਸਿੱਧਾ_dereference = createJSONEmitter(ਇਹ);
        }
        is_opened = ਸੱਚ;
    } ਨਹੀਂਤਾਂ {
        ਅਚਲ size_t buf_size0 = 40;
        ਬਫਰ.resize(buf_size0);
        ਜੇ (mem_ਬਹੁਲਕ) {
            strbuf = (ਅੱਖਰ *) filename_or_buf;
            strbufsize = ਲੜੀ_ਲੰਬਾਈ(strbuf);
        }

        ਅਚਲ ਅੱਖਰ *yaml_signature = "%YAML";
        ਅਚਲ ਅੱਖਰ *json_signature = "{";
        ਅਚਲ ਅੱਖਰ *xml_signature = "<?xml";
        ਅੱਖਰ *buf = ਇਹ->gets(16);
        CV_Assert(buf);
        ਅੱਖਰ *bufPtr = cv_skip_BOM(buf);
        size_t buf_ਵਿੱਥ = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        ਜੇ (ਲੜੀ_ਤੁਲਨਾ(bufPtr, yaml_signature, ਲੜੀ_ਲੰਬਾਈ(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        ਨਹੀਂਤਾਂ ਜੇ (ਲੜੀ_ਤੁਲਨਾ(bufPtr, json_signature, ਲੜੀ_ਲੰਬਾਈ(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        ਨਹੀਂਤਾਂ ਜੇ (ਲੜੀ_ਤੁਲਨਾ(bufPtr, xml_signature, ਲੜੀ_ਲੰਬਾਈ(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        ਨਹੀਂਤਾਂ
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = buf_ਵਿੱਥ;
        bufofs = 0;

        ਕੋਸ਼ਿਸ਼ {
            ਅੱਖਰ *ptr = ਬਫਰ_ਸ਼ੁਰੂ();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_ਗੰਢ ਜੜ੍ਹ_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_ਗੰਢ_Space(ਜੜ੍ਹ_nodes, 9);
            *rptr = File_ਗੰਢ::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            ਚੋਣ (fmt) {
                ਹਾਲਤ FileStorage::FORMAT_XML:
                    parser_do_not_use_ਸਿੱਧਾ_dereference = createXMLParser(ਇਹ);
                    ਰੋਕੋ;
                ਹਾਲਤ FileStorage::FORMAT_YAML:
                ਹਾਲਤ FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_ਸਿੱਧਾ_dereference = createYAMLParser(ਇਹ);
                    ਰੋਕੋ;
                ਹਾਲਤ FileStorage::FORMAT_JSON:
                    parser_do_not_use_ਸਿੱਧਾ_dereference = createJSONParser(ਇਹ);
                    ਰੋਕੋ;
                ਮੂਲ:
                    parser_do_not_use_ਸਿੱਧਾ_dereference = Ptr<FileStorageParser>();
            }

            ਜੇ (!parser_do_not_use_ਸਿੱਧਾ_dereference.empty()) {
                ok = getParser().parse(ptr);
                ਜੇ (ok) {
                    finalizeCollection(ਜੜ੍ਹ_nodes);

                    CV_Assert(!fs_ਅੰਕੜੇ_ptrs.empty());
                    File_ਗੰਢ roots_ਗੰਢ(fs_ext, 0, 0);
                    size_t i, nroots = roots_ਗੰਢ.size();
                    File_ਗੰਢ_Iterator it = roots_ਗੰਢ.begin();

                    ਲਈ (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        ਫੜੋ (...)
        {
            // FIXIT log error message
            is_opened = ਸੱਚ;
            release();
            ਸੁੱਟੋ;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = ਸੱਚ;
        std::vector<ਅੱਖਰ> tmpbuf;
        std::swap(ਬਫਰ, tmpbuf);
        bufofs = 0;
    }
    ਵਾਪਸ ok;
}

ਖਾਲੀ FileStorage::Impl::puts(ਅਚਲ ਅੱਖਰ *str) {
    CV_Assert(write_ਬਹੁਲਕ);
    ਜੇ (mem_ਬਹੁਲਕ)
        std::copy(str, str + ਲੜੀ_ਲੰਬਾਈ(str), std::back_inserter(outbuf));
    ਨਹੀਂਤਾਂ ਜੇ (file)
        fputs(str, file);
#ਜੇ USE_ZLIB
    ਨਹੀਂਤਾਂ ਜੇ (gzfile)
        gzputs(gzfile, str);
#ਜੇ_ਅੰਤ
    ਨਹੀਂਤਾਂ
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

ਅੱਖਰ *FileStorage::Impl::getsFromFile(ਅੱਖਰ *buf, ਪੂਰਨ ਗਿਣਤੀ_3) {
    ਜੇ (file)
        ਵਾਪਸ fgets(buf, ਗਿਣਤੀ_3, file);
#ਜੇ USE_ZLIB
    ਜੇ (gzfile)
        ਵਾਪਸ gzgets(gzfile, buf, ਗਿਣਤੀ_3);
#ਜੇ_ਅੰਤ
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

ਅੱਖਰ *FileStorage::Impl::gets(size_t max_ਗਿਣਤੀ) {
    ਜੇ (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        ਅਚਲ ਅੱਖਰ *instr = strbuf;
        ਲਈ (; i < len; i++) {
            ਅੱਖਰ c = instr[i];
            ਜੇ (c == '\0' || c == '\n') {
                ਜੇ (c == '\n')
                    i++;
                ਰੋਕੋ;
            }
        }
        size_t ਗਿਣਤੀ_3 = i - strbufpos;
        ਜੇ (max_ਗਿਣਤੀ == 0 || max_ਗਿਣਤੀ > ਗਿਣਤੀ_3)
            max_ਗਿਣਤੀ = ਗਿਣਤੀ_3;
        ਬਫਰ.resize(std::max(ਬਫਰ.size(), max_ਗਿਣਤੀ + 8));
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(&ਬਫਰ[0], instr + strbufpos, max_ਗਿਣਤੀ);
        ਬਫਰ[max_ਗਿਣਤੀ] = '\0';
        strbufpos = i;
        ਵਾਪਸ max_ਗਿਣਤੀ > 0 ? &ਬਫਰ[0] : 0;
    }

    ਅਚਲ size_t MAX_ਖੰਡ_ਮਾਪ = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ / 2; // hopefully, that will be enough
    ਜੇ (max_ਗਿਣਤੀ == 0)
        max_ਗਿਣਤੀ = MAX_ਖੰਡ_ਮਾਪ;
    ਨਹੀਂਤਾਂ
        CV_Assert(max_ਗਿਣਤੀ < MAX_ਖੰਡ_ਮਾਪ);
    size_t ofs = 0;

    ਲਈ (;;) {
        ਪੂਰਨ ਗਿਣਤੀ_3 = (ਪੂਰਨ) std::min(ਬਫਰ.size() - ofs - 16, max_ਗਿਣਤੀ);
        ਅੱਖਰ *ptr = getsFromFile(&ਬਫਰ[ofs], ਗਿਣਤੀ_3 + 1);
        ਜੇ (!ptr)
            ਰੋਕੋ;
        ਪੂਰਨ delta = (ਪੂਰਨ) ਲੜੀ_ਲੰਬਾਈ(ptr);
        ofs += delta;
        max_ਗਿਣਤੀ -= delta;
        ਜੇ (delta == 0 || ptr[delta - 1] == '\n' || max_ਗਿਣਤੀ == 0)
            ਰੋਕੋ;
        ਜੇ (delta == ਗਿਣਤੀ_3)
            ਬਫਰ.resize((size_t) (ਬਫਰ.size() * 1.5));
    }
    ਵਾਪਸ ofs > 0 ? &ਬਫਰ[0] : 0;
}

ਅੱਖਰ *FileStorage::Impl::gets() {
    ਅੱਖਰ *ptr = ਇਹ->gets(0);
    ਜੇ (!ptr) {
        ptr = ਬਫਰ_ਸ਼ੁਰੂ();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        ਸੈੱਟ_Eof();
        ਵਾਪਸ 0;
    } ਨਹੀਂਤਾਂ {
        size_t l = ਲੜੀ_ਲੰਬਾਈ(ptr);
        ਜੇ (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    ਵਾਪਸ ptr;
}

ਤਾਰਕਿਕ FileStorage::Impl::eof() {
    ਜੇ (dummy_eof)
        ਵਾਪਸ ਸੱਚ;
    ਜੇ (strbuf)
        ਵਾਪਸ strbufpos >= strbufsize;
    ਜੇ (file)
        ਵਾਪਸ feof(file) != 0;
#ਜੇ USE_ZLIB
    ਜੇ (gzfile)
        ਵਾਪਸ gzeof(gzfile) != 0;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਝੂਠ;
}

ਖਾਲੀ FileStorage::Impl::ਸੈੱਟ_Eof() {
    dummy_eof = ਸੱਚ;
}

ਖਾਲੀ FileStorage::Impl::closeFile() {
    ਜੇ (file)
        fclose(file);
#ਜੇ USE_ZLIB
    ਨਹੀਂਤਾਂ ਜੇ (gzfile)
        gzclose(gzfile);
#ਜੇ_ਅੰਤ
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = ਝੂਠ;
}

ਖਾਲੀ FileStorage::Impl::rewind() {
    ਜੇ (file)
        ::rewind(file);
#ਜੇ USE_ZLIB
    ਨਹੀਂਤਾਂ ਜੇ (gzfile)
        gzrewind(gzfile);
#ਜੇ_ਅੰਤ
    strbufpos = 0;
}

ਅੱਖਰ *FileStorage::Impl::resize_Write_ਬਫਰ(ਅੱਖਰ *ptr, ਪੂਰਨ len) {
    ਅਚਲ ਅੱਖਰ *ਬਫਰ_ਸਿਰਾ = &ਬਫਰ[0] + ਬਫਰ.size();
    ਜੇ (ptr + len < ਬਫਰ_ਸਿਰਾ)
        ਵਾਪਸ ptr;

    ਅਚਲ ਅੱਖਰ *ਬਫਰ_ਸ਼ੁਰੂ_2 = &ਬਫਰ[0];
    ਪੂਰਨ written_len = (ਪੂਰਨ) (ptr - ਬਫਰ_ਸ਼ੁਰੂ_2);

    CV_Assert(written_len <= (ਪੂਰਨ) ਬਫਰ.size());
    ਪੂਰਨ new_ਮਾਪ_2 = (ਪੂਰਨ) ((ਬਫਰ_ਸਿਰਾ - ਬਫਰ_ਸ਼ੁਰੂ_2) * 3 / 2);
    new_ਮਾਪ_2 = MAX(written_len + len, new_ਮਾਪ_2);
    ਬਫਰ.reserve(new_ਮਾਪ_2 + 256);
    ਬਫਰ.resize(new_ਮਾਪ_2);
    bufofs = written_len;
    ਵਾਪਸ &ਬਫਰ[0] + bufofs;
}

ਅੱਖਰ *FileStorage::Impl::flush() {
    ਅੱਖਰ *ਬਫਰ_ਸ਼ੁਰੂ_2 = &ਬਫਰ[0];
    ਅੱਖਰ *ptr = ਬਫਰ_ਸ਼ੁਰੂ_2 + bufofs;

    ਜੇ (ptr > ਬਫਰ_ਸ਼ੁਰੂ_2 + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(ਬਫਰ_ਸ਼ੁਰੂ_2);
        bufofs = 0;
    }

    ਪੂਰਨ indent = write_ਢੇਰ.back().indent;

    ਜੇ (space != indent) {
        memset(ਬਫਰ_ਸ਼ੁਰੂ_2, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = ਬਫਰ_ਸ਼ੁਰੂ_2 + bufofs;

    ਵਾਪਸ ptr;
}

ਖਾਲੀ FileStorage::Impl::ਸਿਰਾ_Write_Struct() {
    CV_Assert(write_ਬਹੁਲਕ);

    check_if_write_struct_is_delayed(ਝੂਠ);
    ਜੇ (state_ਦਾ_writing_base64 != FileStorage_API::Uncertain)
        switch_ਮੰਜ਼ਿਲ_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_ਢੇਰ.empty());

    FStruct_ਅੰਕੜੇ &current_struct = write_ਢੇਰ.back();
    ਜੇ (fmt == FileStorage::FORMAT_JSON && !File_ਗੰਢ::isFlow(current_struct.flags) && write_ਢੇਰ.size() > 1)
        current_struct.indent = write_ਢੇਰ[write_ਢੇਰ.size() - 2].indent;

    getEmitter().ਸਿਰਾ_Write_Struct(current_struct);

    write_ਢੇਰ.pop_back();
    ਜੇ (!write_ਢੇਰ.empty())
        write_ਢੇਰ.back().flags &= ~File_ਗੰਢ::ਖਾਲੀ_ਥਾਵਾਂ_2;
}

ਖਾਲੀ FileStorage::Impl::ਸ਼ੁਰੂ_Write_Struct_helper(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਪੂਰਨ struct_flags,
                                                ਅਚਲ ਅੱਖਰ *type_name) {
    CV_Assert(write_ਬਹੁਲਕ);

    struct_flags = (struct_flags & (File_ਗੰਢ::TYPE_ਨਕਾਬ | File_ਗੰਢ::FLOW)) | File_ਗੰਢ::ਖਾਲੀ_ਥਾਵਾਂ_2;
    ਜੇ (!File_ਗੰਢ::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    ਜੇ (type_name && type_name[0] == '\0')
        type_name = 0;

    FStruct_ਅੰਕੜੇ s = getEmitter().ਸ਼ੁਰੂ_Write_Struct(write_ਢੇਰ.back(), ਕੁੰਜੀ, struct_flags, type_name);

    write_ਢੇਰ.push_back(s);
    size_t write_ਢੇਰ_ਮਾਪ = write_ਢੇਰ.size();
    ਜੇ (write_ਢੇਰ_ਮਾਪ > 1)
        write_ਢੇਰ[write_ਢੇਰ_ਮਾਪ - 2].flags &= ~File_ਗੰਢ::ਖਾਲੀ_ਥਾਵਾਂ_2;

    ਜੇ (fmt != FileStorage::FORMAT_JSON && !File_ਗੰਢ::isFlow(s.flags))
        flush();

    ਜੇ (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_ਗੰਢ::isMap(struct_flags)) {
        getEmitter().ਲਿਖ("type_id", type_name, ਝੂਠ);
    }
}

ਖਾਲੀ FileStorage::Impl::ਸ਼ੁਰੂ_Write_Struct(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਪੂਰਨ struct_flags,
                                         ਅਚਲ ਅੱਖਰ *type_name) {
    check_if_write_struct_is_delayed(ਝੂਠ);
    ਜੇ (state_ਦਾ_writing_base64 == FileStorage_API::NotUse)
        switch_ਮੰਜ਼ਿਲ_Base64_state(FileStorage_API::Uncertain);

    ਜੇ (state_ਦਾ_writing_base64 == FileStorage_API::Uncertain && File_ਗੰਢ::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(ਕੁੰਜੀ, struct_flags, type_name);
    } ਨਹੀਂਤਾਂ ਜੇ (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        ਜੇ ((File_ਗੰਢ::TYPE_ਨਕਾਬ & struct_flags) != File_ਗੰਢ::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        ਨਹੀਂਤਾਂ ਜੇ (state_ਦਾ_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        ਸ਼ੁਰੂ_Write_Struct_helper(ਕੁੰਜੀ, struct_flags, "binary");

        ਜੇ (state_ਦਾ_writing_base64 != FileStorage_API::Uncertain)
            switch_ਮੰਜ਼ਿਲ_Base64_state(FileStorage_API::Uncertain);
        switch_ਮੰਜ਼ਿਲ_Base64_state(FileStorage_API::ਵਿੱਚ_Use);
    } ਨਹੀਂਤਾਂ {
        /* Won't output Base64 data */
        ਜੇ (state_ਦਾ_writing_base64 == FileStorage_API::ਵਿੱਚ_Use)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        ਸ਼ੁਰੂ_Write_Struct_helper(ਕੁੰਜੀ, struct_flags, type_name);

        ਜੇ (state_ਦਾ_writing_base64 != FileStorage_API::Uncertain)
            switch_ਮੰਜ਼ਿਲ_Base64_state(FileStorage_API::Uncertain);
        switch_ਮੰਜ਼ਿਲ_Base64_state(FileStorage_API::NotUse);
    }
}

ਖਾਲੀ FileStorage::Impl::writeComment(ਅਚਲ ਅੱਖਰ *comment, ਤਾਰਕਿਕ eol_comment) {
    CV_Assert(write_ਬਹੁਲਕ);
    getEmitter().writeComment(comment, eol_comment);
}

ਖਾਲੀ FileStorage::Impl::ਸ਼ੁਰੂ_ਅਗਲਾ_ਧਾਰਾ() {
    CV_Assert(write_ਬਹੁਲਕ);
    ਜੇ (!ਖਾਲੀ_ਥਾਵਾਂ_ਧਾਰਾ) {
        ਜਦੋਂ (!write_ਢੇਰ.empty())
            ਸਿਰਾ_Write_Struct();
        flush();
        getEmitter().ਸ਼ੁਰੂ_ਅਗਲਾ_ਧਾਰਾ();
        ਖਾਲੀ_ਥਾਵਾਂ_ਧਾਰਾ = ਸੱਚ;
        write_ਢੇਰ.push_back(FStruct_ਅੰਕੜੇ("", File_ਗੰਢ::ਖਾਲੀ_ਥਾਵਾਂ_2, 0));
        bufofs = 0;
    }
}

ਖਾਲੀ FileStorage::Impl::ਲਿਖ(ਅਚਲ String &ਕੁੰਜੀ, ਪੂਰਨ value) {
    CV_Assert(write_ਬਹੁਲਕ);
    getEmitter().ਲਿਖ(ਕੁੰਜੀ.c_str(), value);
}

ਖਾਲੀ FileStorage::Impl::ਲਿਖ(ਅਚਲ String &ਕੁੰਜੀ, int64_t value) {
    CV_Assert(write_ਬਹੁਲਕ);
    getEmitter().ਲਿਖ(ਕੁੰਜੀ.c_str(), value);
}

ਖਾਲੀ FileStorage::Impl::ਲਿਖ(ਅਚਲ String &ਕੁੰਜੀ, ਦੁੱਗਣਾ value) {
    CV_Assert(write_ਬਹੁਲਕ);
    getEmitter().ਲਿਖ(ਕੁੰਜੀ.c_str(), value);
}

ਖਾਲੀ FileStorage::Impl::ਲਿਖ(ਅਚਲ String &ਕੁੰਜੀ, ਅਚਲ String &value) {
    CV_Assert(write_ਬਹੁਲਕ);
    getEmitter().ਲਿਖ(ਕੁੰਜੀ.c_str(), value.c_str(), ਝੂਠ);
}

ਖਾਲੀ FileStorage::Impl::write_Raw_ਅੰਕੜੇ(ਅਚਲ std::string &dt, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ_3, size_t len) {
    CV_Assert(write_ਬਹੁਲਕ);

    ਜੇ (is_using_base64 || state_ਦਾ_writing_base64 == FileStorage_API::Base64State::ਵਿੱਚ_Use) {
        write_Raw_ਅੰਕੜੇ_Base64(ਅੰਕੜੇ_3, len, dt.c_str());
        ਵਾਪਸ;
    } ਨਹੀਂਤਾਂ ਜੇ (state_ਦਾ_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_ਮੰਜ਼ਿਲ_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elem_ਮਾਪ = fs::calc_Struct_ਮਾਪ(dt.c_str(), 0);
    CV_Assert(elem_ਮਾਪ);
    CV_Assert(len % elem_ਮਾਪ == 0);
    len /= elem_ਮਾਪ;

    ਤਾਰਕਿਕ explicitZero = fmt == FileStorage::FORMAT_JSON;
    ਅਚਲ uchar *data0 = (ਅਚਲ uchar *) ਅੰਕੜੇ_3;
    ਪੂਰਨ fmt_ਜੋੜੇ[CV_FS_MAX_FMT_ਜੋੜੇ * 2], k, fmt_pair_ਗਿਣਤੀ;
    ਅੱਖਰ buf[256] = "";

    fmt_pair_ਗਿਣਤੀ = fs::decodeFormat(dt.c_str(), fmt_ਜੋੜੇ, CV_FS_MAX_FMT_ਜੋੜੇ);

    ਜੇ (!len)
        ਵਾਪਸ;

    ਜੇ (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    ਜੇ (fmt_pair_ਗਿਣਤੀ == 1) {
        fmt_ਜੋੜੇ[0] *= (ਪੂਰਨ) len;
        len = 1;
    }

    ਲਈ (; len--; data0 += elem_ਮਾਪ) {
        ਪੂਰਨ offset = 0;
        ਲਈ (k = 0; k < fmt_pair_ਗਿਣਤੀ; k++) {
            ਪੂਰਨ i, ਗਿਣਤੀ_3 = fmt_ਜੋੜੇ[k * 2];
            ਪੂਰਨ elem_type = fmt_ਜੋੜੇ[k * 2 + 1];
            ਪੂਰਨ elem_ਮਾਪ_2 = CV_ELEM_ਮਾਪ(elem_type);
            ਅਚਲ ਅੱਖਰ *ptr;

            offset = cvAlign(offset, elem_ਮਾਪ_2);
            ਅਚਲ uchar *ਅੰਕੜੇ = data0 + offset;

            ਲਈ (i = 0; i < ਗਿਣਤੀ_3; i++) {
                ਚੋਣ (elem_type) {
                    ਹਾਲਤ CV_8U:
                        ptr = fs::itoa(*(uchar *) ਅੰਕੜੇ, buf, 10);
                        ਅੰਕੜੇ++;
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_Bool:
                        ptr = fs::itoa(*(uchar *) ਅੰਕੜੇ != 0, buf, 10);
                        ਅੰਕੜੇ++;
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_8S:
                        ptr = fs::itoa(*(ਅੱਖਰ *) ਅੰਕੜੇ, buf, 10);
                        ਅੰਕੜੇ++;
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_16U:
                        ptr = fs::itoa(*(ushort *) ਅੰਕੜੇ, buf, 10);
                        ਅੰਕੜੇ += ਆਕਾਰ(ushort);
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_16S:
                        ptr = fs::itoa(*(ਛੋਟਾ *) ਅੰਕੜੇ, buf, 10);
                        ਅੰਕੜੇ += ਆਕਾਰ(ਛੋਟਾ);
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_32U:
                        ptr = fs::itoa((int64_t)*(ਬਿਨਾਨਿਸ਼ਾਨੀ*) ਅੰਕੜੇ, buf, 10, ਝੂਠ);
                        ਅੰਕੜੇ += ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ);
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_32S:
                        ptr = fs::itoa(*(ਪੂਰਨ *) ਅੰਕੜੇ, buf, 10);
                        ਅੰਕੜੇ += ਆਕਾਰ(ਪੂਰਨ);
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_64U:
                        ptr = fs::itoa(*(uint64_t*) ਅੰਕੜੇ, buf, 10, ਝੂਠ);
                        ਅੰਕੜੇ += ਆਕਾਰ(uint64_t);
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_64S:
                        ptr = fs::itoa(*(int64_t*) ਅੰਕੜੇ, buf, 10, ਸੱਚ);
                        ਅੰਕੜੇ += ਆਕਾਰ(int64_t);
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_32F:
                        ptr = fs::float_ਮੰਜ਼ਿਲ_String(buf, ਆਕਾਰ(buf), *(ਦਸ਼ਮਲਵ *) ਅੰਕੜੇ, ਝੂਠ, explicitZero);
                        ਅੰਕੜੇ += ਆਕਾਰ(ਦਸ਼ਮਲਵ);
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_64F:
                        ptr = fs::double_ਮੰਜ਼ਿਲ_String(buf, ਆਕਾਰ(buf), *(ਦੁੱਗਣਾ *) ਅੰਕੜੇ, explicitZero);
                        ਅੰਕੜੇ += ਆਕਾਰ(ਦੁੱਗਣਾ);
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_16F:
                        ptr = fs::float_ਮੰਜ਼ਿਲ_String(buf, ਆਕਾਰ(buf), (ਦਸ਼ਮਲਵ) *(hfloat *) ਅੰਕੜੇ, ਸੱਚ, explicitZero);
                        ਅੰਕੜੇ += ਆਕਾਰ(hfloat);
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_16BF:
                        ptr = fs::float_ਮੰਜ਼ਿਲ_String(buf, ਆਕਾਰ(buf), (ਦਸ਼ਮਲਵ) *(bfloat *) ਅੰਕੜੇ, ਸੱਚ, explicitZero);
                        ਅੰਕੜੇ += ਆਕਾਰ(bfloat);
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_8F_E4M3FN:
                        ptr = fs::float_ਮੰਜ਼ਿਲ_String(buf, ਆਕਾਰ(buf), (ਦਸ਼ਮਲਵ) *(fp8_t *) ਅੰਕੜੇ, ਸੱਚ, explicitZero);
                        ਅੰਕੜੇ += ਆਕਾਰ(fp8_t);
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_8F_E4M3FNUZ:
                        ptr = fs::float_ਮੰਜ਼ਿਲ_String(buf, ਆਕਾਰ(buf), (ਦਸ਼ਮਲਵ) *(fp8a_t *) ਅੰਕੜੇ, ਸੱਚ, explicitZero);
                        ਅੰਕੜੇ += ਆਕਾਰ(fp8a_t);
                        ਰੋਕੋ;
                    ਮੂਲ:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        ਵਾਪਸ;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (ਪੂਰਨ) (ਅੰਕੜੇ - data0);
        }
    }
}

ਖਾਲੀ FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(ਝੂਠ);

    ਜੇ (state_ਦਾ_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_ਮੰਜ਼ਿਲ_Base64_state(FileStorage_API::Base64State::Uncertain);
}

ਖਾਲੀ FileStorage::Impl::switch_ਮੰਜ਼ਿਲ_Base64_state(FileStorage_API::Base64State new_state) {
    ਅਚਲ ਅੱਖਰ *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    ਅਚਲ ਅੱਖਰ *err_unable_ਮੰਜ਼ਿਲ_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    ਚੋਣ (state_ਦਾ_writing_base64) {
        ਹਾਲਤ FileStorage_API::Base64State::Uncertain:
            ਚੋਣ (new_state) {
                ਹਾਲਤ FileStorage_API::Base64State::ਵਿੱਚ_Use:
                {
                    CV_DbgAssert(base64_writer == 0);
                    ਤਾਰਕਿਕ can_indent = (fmt != cv::FileStorage::ਬਹੁਲਕ_2::FORMAT_JSON);
                    base64_writer = ਨਵਾਂ base64::Base64Writer(*ਇਹ, can_indent);
                    ਜੇ (!can_indent) {
                        ਅੱਖਰ *ptr = ਬਫਰ_Ptr();
                        *ptr++ = '\0';
                        puts(ਬਫਰ_ਸ਼ੁਰੂ());
                        ਸੈੱਟ_ਬਫਰ_Ptr(ਬਫਰ_ਸ਼ੁਰੂ());
                        memset(ਬਫਰ_ਸ਼ੁਰੂ(), 0, static_cast<ਪੂਰਨ>(space));
                        puts("\"$base64$");
                    }
                    ਰੋਕੋ;
                }
                ਹਾਲਤ FileStorage_API::Base64State::Uncertain:
                    ਰੋਕੋ;
                ਹਾਲਤ FileStorage_API::Base64State::NotUse:
                    ਰੋਕੋ;
                ਮੂਲ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ਰੋਕੋ;
            }
            ਰੋਕੋ;
        ਹਾਲਤ FileStorage_API::Base64State::ਵਿੱਚ_Use:
            ਚੋਣ (new_state) {
                ਹਾਲਤ FileStorage_API::Base64State::ਵਿੱਚ_Use:
                ਹਾਲਤ FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_ਮੰਜ਼ਿਲ_switch);
                    ਰੋਕੋ;
                ਹਾਲਤ FileStorage_API::Base64State::Uncertain:
                    ਮਿਟਾਓ base64_writer;
                    base64_writer = 0;
                    ਜੇ ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        ਸੈੱਟ_ਬਫਰ_Ptr(ਬਫਰ_ਸ਼ੁਰੂ());
                        flush();
                        memset(ਬਫਰ_ਸ਼ੁਰੂ(), 0, static_cast<ਪੂਰਨ>(space) );
                        ਸੈੱਟ_ਬਫਰ_Ptr(ਬਫਰ_ਸ਼ੁਰੂ());
                    }
                    ਰੋਕੋ;
                ਮੂਲ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ਰੋਕੋ;
            }
            ਰੋਕੋ;
        ਹਾਲਤ FileStorage_API::Base64State::NotUse:
            ਚੋਣ (new_state) {
                ਹਾਲਤ FileStorage_API::Base64State::ਵਿੱਚ_Use:
                ਹਾਲਤ FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_ਮੰਜ਼ਿਲ_switch);
                    ਰੋਕੋ;
                ਹਾਲਤ FileStorage_API::Base64State::Uncertain:
                    ਰੋਕੋ;
                ਮੂਲ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ਰੋਕੋ;
            }
            ਰੋਕੋ;
        ਮੂਲ:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            ਰੋਕੋ;
    }

    state_ਦਾ_writing_base64 = new_state;
}

ਖਾਲੀ FileStorage::Impl::make_write_struct_delayed(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਪੂਰਨ struct_flags, ਅਚਲ ਅੱਖਰ *type_name) {
    CV_Assert(is_write_struct_delayed == ਝੂਠ);
    CV_DbgAssert(delayed_struct_ਕੁੰਜੀ == ਖਾਲੀ_ਸੂਚਕ);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == ਖਾਲੀ_ਸੂਚਕ);

    delayed_struct_flags = struct_flags;

    ਜੇ (ਕੁੰਜੀ != ਖਾਲੀ_ਸੂਚਕ) {
        delayed_struct_ਕੁੰਜੀ = ਨਵਾਂ ਅੱਖਰ[ਲੜੀ_ਲੰਬਾਈ(ਕੁੰਜੀ) + 1U];
        strcpy(delayed_struct_ਕੁੰਜੀ, ਕੁੰਜੀ);
    }

    ਜੇ (type_name != ਖਾਲੀ_ਸੂਚਕ) {
        delayed_type_name = ਨਵਾਂ ਅੱਖਰ[ਲੜੀ_ਲੰਬਾਈ(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = ਸੱਚ;
}

ਖਾਲੀ FileStorage::Impl::check_if_write_struct_is_delayed(ਤਾਰਕਿਕ change_type_ਮੰਜ਼ਿਲ_base64) {
    ਜੇ (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_ਕੁੰਜੀ;
        std::string type_name;
        ਪੂਰਨ struct_flags = delayed_struct_flags;

        ਜੇ (delayed_struct_ਕੁੰਜੀ != ਖਾਲੀ_ਸੂਚਕ && *delayed_struct_ਕੁੰਜੀ != '\0') {
            struct_ਕੁੰਜੀ.assign(delayed_struct_ਕੁੰਜੀ);
        }
        ਜੇ (delayed_type_name != ਖਾਲੀ_ਸੂਚਕ && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        ਮਿਟਾਓ[] delayed_struct_ਕੁੰਜੀ;
        ਮਿਟਾਓ[] delayed_type_name;
        delayed_struct_ਕੁੰਜੀ = ਖਾਲੀ_ਸੂਚਕ;
        delayed_struct_flags = 0;
        delayed_type_name = ਖਾਲੀ_ਸੂਚਕ;

        is_write_struct_delayed = ਝੂਠ;

        /* call */
        ਜੇ (change_type_ਮੰਜ਼ਿਲ_base64) {
            ਸ਼ੁਰੂ_Write_Struct_helper(struct_ਕੁੰਜੀ.c_str(), struct_flags, "binary");
            ਜੇ (state_ਦਾ_writing_base64 != FileStorage_API::Uncertain)
                switch_ਮੰਜ਼ਿਲ_Base64_state(FileStorage_API::Uncertain);
            switch_ਮੰਜ਼ਿਲ_Base64_state(FileStorage_API::ਵਿੱਚ_Use);
        } ਨਹੀਂਤਾਂ {
            ਸ਼ੁਰੂ_Write_Struct_helper(struct_ਕੁੰਜੀ.c_str(), struct_flags, type_name.c_str());
            ਜੇ (state_ਦਾ_writing_base64 != FileStorage_API::Uncertain)
                switch_ਮੰਜ਼ਿਲ_Base64_state(FileStorage_API::Uncertain);
            switch_ਮੰਜ਼ਿਲ_Base64_state(FileStorage_API::NotUse);
        }
    }
}

ਖਾਲੀ FileStorage::Impl::write_Raw_ਅੰਕੜੇ_Base64(ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ_3, size_t len, ਅਚਲ ਅੱਖਰ *dt) {
    CV_Assert(write_ਬਹੁਲਕ);

    check_if_write_struct_is_delayed(ਸੱਚ);

    ਜੇ (state_ਦਾ_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_ਮੰਜ਼ਿਲ_Base64_state(FileStorage_API::Base64State::ਵਿੱਚ_Use);
    } ਨਹੀਂਤਾਂ ਜੇ (state_ਦਾ_writing_base64 != FileStorage_API::Base64State::ਵਿੱਚ_Use) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->ਲਿਖ(ਅੰਕੜੇ_3, len, dt);
}

File_ਗੰਢ FileStorage::Impl::get_ਪਹਿਲਾ_ਸਿਖਰ_Level_ਗੰਢ() ਅਚਲ {
    ਵਾਪਸ roots.empty() ? File_ਗੰਢ() : roots[0];
}

File_ਗੰਢ FileStorage::Impl::ਜੜ੍ਹ(ਪੂਰਨ ਧਾਰਾ_Idx) ਅਚਲ {
    ਵਾਪਸ ਧਾਰਾ_Idx >= 0 && ਧਾਰਾ_Idx < (ਪੂਰਨ) roots.size() ? roots[ਧਾਰਾ_Idx] : File_ਗੰਢ();
}

File_ਗੰਢ FileStorage::Impl::ਸੰਚਾਲਕ[](ਅਚਲ String &nodename) ਅਚਲ {
    ਵਾਪਸ ਇਹ->ਸੰਚਾਲਕ[](nodename.c_str());
}

File_ਗੰਢ FileStorage::Impl::ਸੰਚਾਲਕ[](ਅਚਲ ਅੱਖਰ * /*nodename*/) ਅਚਲ {
    ਵਾਪਸ File_ਗੰਢ();
}

ਪੂਰਨ FileStorage::Impl::getFormat() ਅਚਲ { ਵਾਪਸ fmt; }

ਅੱਖਰ *FileStorage::Impl::ਬਫਰ_Ptr() ਅਚਲ { ਵਾਪਸ (ਅੱਖਰ *) (&ਬਫਰ[0] + bufofs); }

ਅੱਖਰ *FileStorage::Impl::ਬਫਰ_ਸ਼ੁਰੂ() ਅਚਲ { ਵਾਪਸ (ਅੱਖਰ *) &ਬਫਰ[0]; }

ਅੱਖਰ *FileStorage::Impl::ਬਫਰ_ਸਿਰਾ_2() ਅਚਲ { ਵਾਪਸ (ਅੱਖਰ *) (&ਬਫਰ[0] + ਬਫਰ.size()); }

ਖਾਲੀ FileStorage::Impl::ਸੈੱਟ_ਬਫਰ_Ptr(ਅੱਖਰ *ptr) {
    ਅੱਖਰ *bufferstart = ਬਫਰ_ਸ਼ੁਰੂ();
    CV_Assert(ptr >= bufferstart && ptr <= ਬਫਰ_ਸਿਰਾ_2());
    bufofs = ptr - bufferstart;
}

ਪੂਰਨ FileStorage::Impl::wrapMargin() ਅਚਲ { ਵਾਪਸ wrap_margin; }

FStruct_ਅੰਕੜੇ &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_ਢੇਰ.empty());
    ਵਾਪਸ write_ਢੇਰ.back();
}

ਖਾਲੀ FileStorage::Impl::ਸੈੱਟ_Non_ਖਾਲੀ_ਥਾਵਾਂ() {
    ਖਾਲੀ_ਥਾਵਾਂ_ਧਾਰਾ = ਝੂਠ;
}

ਖਾਲੀ FileStorage::Impl::processSpecialDouble(ਅੱਖਰ *buf, ਦੁੱਗਣਾ *value, ਅੱਖਰ **endptr) {
    FileStorage_API *fs = ਇਹ;
    ਅੱਖਰ c = buf[0];
    ਪੂਰਨ inf_hi = 0x7ff00000;

    ਜੇ (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    ਜੇ (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    ਜੇ (ਵੱਡਾ_ਅੱਖਰ(buf[1]) == 'I' && ਵੱਡਾ_ਅੱਖਰ(buf[2]) == 'N' && ਵੱਡਾ_ਅੱਖਰ(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    ਨਹੀਂਤਾਂ ਜੇ (ਵੱਡਾ_ਅੱਖਰ(buf[1]) == 'N' && ਵੱਡਾ_ਅੱਖਰ(buf[2]) == 'A' && ਵੱਡਾ_ਅੱਖਰ(buf[3]) == 'N')
        v.u = (uint64) -1;
    ਨਹੀਂਤਾਂ
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

ਦੁੱਗਣਾ FileStorage::Impl::strtod(ਅੱਖਰ *ptr, ਅੱਖਰ **endptr) {
    ਦੁੱਗਣਾ fval = ::strtod(ptr, endptr);
    ਜੇ (**endptr == '.') {
        ਅੱਖਰ *dot_pos = *endptr;
        *dot_pos = ',';
        ਦੁੱਗਣਾ fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        ਜੇ (*endptr > dot_pos)
            fval = fval2;
        ਨਹੀਂਤਾਂ
            *endptr = dot_pos;
    }

    ਜੇ (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    ਵਾਪਸ fval;
}

ਖਾਲੀ FileStorage::Impl::convert_ਮੰਜ਼ਿਲ_Collection(ਪੂਰਨ type, File_ਗੰਢ &ਗੰਢ) {
    CV_Assert(type == File_ਗੰਢ::SEQ || type == File_ਗੰਢ::MAP);

    ਪੂਰਨ ਗੰਢ_type = ਗੰਢ.type();
    ਜੇ (ਗੰਢ_type == type)
        ਵਾਪਸ;

    ਤਾਰਕਿਕ named = ਗੰਢ.isNamed();
    uchar *ptr = ਗੰਢ.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    ਦੁੱਗਣਾ fval = 0;
    std::string sval;
    ਤਾਰਕਿਕ ਜੋੜੋ_ਪਹਿਲਾ_scalar = ਝੂਠ;

    ਜੇ (ਗੰਢ_type != File_ਗੰਢ::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_ਗੰਢ::SEQ);
        ਜੇ (ਗੰਢ_type == File_ਗੰਢ::INT) {
            ival = readLong(ptr);
            ਜੋੜੋ_ਪਹਿਲਾ_scalar = ਸੱਚ;
        } ਨਹੀਂਤਾਂ ਜੇ (ਗੰਢ_type == File_ਗੰਢ::REAL) {
            fval = readReal(ptr);
            ਜੋੜੋ_ਪਹਿਲਾ_scalar = ਸੱਚ;
        } ਨਹੀਂਤਾਂ ਜੇ (ਗੰਢ_type == File_ਗੰਢ::STRING) {
            sval = std::string(ਗੰਢ);
            ਜੋੜੋ_ਪਹਿਲਾ_scalar = ਸੱਚ;
        } ਨਹੀਂਤਾਂ
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", ਗੰਢ_type));
    }

    ptr = reserve_ਗੰਢ_Space(ਗੰਢ, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_ਗੰਢ::NAMED : 0));
    // name has been copied automatically
    ਜੇ (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    ਜੇ (ਜੋੜੋ_ਪਹਿਲਾ_scalar)
        ਜੋੜੋ_ਗੰਢ(ਗੰਢ, std::string(), ਗੰਢ_type,
                ਗੰਢ_type == File_ਗੰਢ::INT ? (ਅਚਲ ਖਾਲੀ *) &ival :
                ਗੰਢ_type == File_ਗੰਢ::REAL ? (ਅਚਲ ਖਾਲੀ *) &fval :
                ਗੰਢ_type == File_ਗੰਢ::STRING ? (ਅਚਲ ਖਾਲੀ *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_ਗੰਢ_Space(File_ਗੰਢ &ਗੰਢ, size_t sz) {
    ਤਾਰਕਿਕ shrink_ਖੰਡ = ਝੂਠ;
    size_t shrink_ਖੰਡ_Idx = 0, shrink_ਮਾਪ = 0;

    uchar *ptr = 0, *ਖੰਡ_ਸਿਰਾ = 0;

    ਜੇ (!fs_ਅੰਕੜੇ_ptrs.empty()) {
        size_t ਖੰਡ_Idx = ਗੰਢ.ਖੰਡ_Idx;
        size_t ofs = ਗੰਢ.ofs;
        CV_Assert(ਖੰਡ_Idx == fs_ਅੰਕੜੇ_ptrs.size() - 1);
        CV_Assert(ofs <= fs_ਅੰਕੜੇ_blksz[ਖੰਡ_Idx]);
        CV_Assert(freeSpaceOfs <= fs_ਅੰਕੜੇ_blksz[ਖੰਡ_Idx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_ਅੰਕੜੇ_ptrs[ਖੰਡ_Idx] + ofs;
        ਖੰਡ_ਸਿਰਾ = fs_ਅੰਕੜੇ_ptrs[ਖੰਡ_Idx] + fs_ਅੰਕੜੇ_blksz[ਖੰਡ_Idx];

        CV_Assert(ptr >= fs_ਅੰਕੜੇ_ptrs[ਖੰਡ_Idx] && ptr <= ਖੰਡ_ਸਿਰਾ);
        ਜੇ (ptr + sz <= ਖੰਡ_ਸਿਰਾ) {
            freeSpaceOfs = ofs + sz;
            ਵਾਪਸ ptr;
        }

        ਜੇ (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_ਅੰਕੜੇ[ਖੰਡ_Idx]->resize(sz);
            ptr = &fs_ਅੰਕੜੇ[ਖੰਡ_Idx]->at(0);
            fs_ਅੰਕੜੇ_ptrs[ਖੰਡ_Idx] = ptr;
            fs_ਅੰਕੜੇ_blksz[ਖੰਡ_Idx] = sz;
            freeSpaceOfs = sz;
            ਵਾਪਸ ptr;
        }

        shrink_ਖੰਡ = ਸੱਚ;
        shrink_ਖੰਡ_Idx = ਖੰਡ_Idx;
        shrink_ਮਾਪ = ofs;
    }

    size_t ਖੰਡ_ਮਾਪ_2 = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(ਖੰਡ_ਮਾਪ_2);
    fs_ਅੰਕੜੇ.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_ਅੰਕੜੇ_ptrs.push_back(new_ptr);
    fs_ਅੰਕੜੇ_blksz.push_back(ਖੰਡ_ਮਾਪ_2);
    ਗੰਢ.ਖੰਡ_Idx = fs_ਅੰਕੜੇ_ptrs.size() - 1;
    ਗੰਢ.ofs = 0;
    freeSpaceOfs = sz;

    ਜੇ (ptr && ptr + 5 <= ਖੰਡ_ਸਿਰਾ) {
        new_ptr[0] = ptr[0];
        ਜੇ (ptr[0] & File_ਗੰਢ::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    ਜੇ (shrink_ਖੰਡ) {
        fs_ਅੰਕੜੇ[shrink_ਖੰਡ_Idx]->resize(shrink_ਮਾਪ);
        fs_ਅੰਕੜੇ_blksz[shrink_ਖੰਡ_Idx] = shrink_ਮਾਪ;
    }

    ਵਾਪਸ new_ptr;
}

ਬਿਨਾਨਿਸ਼ਾਨੀ FileStorage::Impl::getStringOfs(ਅਚਲ std::string &ਕੁੰਜੀ) ਅਚਲ {
    str_hash_t::const_iterator it = str_hash.find(ਕੁੰਜੀ);
    ਵਾਪਸ it != str_hash.end() ? it->second : 0;
}

File_ਗੰਢ FileStorage::Impl::ਜੋੜੋ_ਗੰਢ(File_ਗੰਢ &collection, ਅਚਲ std::string &ਕੁੰਜੀ,
                                    ਪੂਰਨ elem_type, ਅਚਲ ਖਾਲੀ *value, ਪੂਰਨ len) {
    FileStorage_API *fs = ਇਹ;
    ਤਾਰਕਿਕ noname = ਕੁੰਜੀ.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(ਕੁੰਜੀ.c_str(), "_") == 0);
    convert_ਮੰਜ਼ਿਲ_Collection(noname ? File_ਗੰਢ::SEQ : File_ਗੰਢ::MAP, collection);

    ਤਾਰਕਿਕ isseq = collection.empty() ? ਝੂਠ : collection.isSeq();
    ਜੇ (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    ਬਿਨਾਨਿਸ਼ਾਨੀ strofs = 0;
    ਜੇ (!noname) {
        strofs = getStringOfs(ਕੁੰਜੀ);
        ਜੇ (!strofs) {
            strofs = (ਬਿਨਾਨਿਸ਼ਾਨੀ) str_hash_ਅੰਕੜੇ.size();
            size_t keysize = ਕੁੰਜੀ.size() + 1;
            str_hash_ਅੰਕੜੇ.resize(strofs + keysize);
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(&str_hash_ਅੰਕੜੇ[0] + strofs, &ਕੁੰਜੀ[0], keysize);
            str_hash.insert(std::make_pair(ਕੁੰਜੀ, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t ਖੰਡ_Idx = fs_ਅੰਕੜੇ_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_ਗੰਢ ਗੰਢ(fs_ext, ਖੰਡ_Idx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_ਗੰਢ_Space(ਗੰਢ, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_ਗੰਢ::NAMED));
    ਜੇ (elem_type == File_ਗੰਢ::NONE)
        freeSpaceOfs -= 8;

    ਜੇ (!noname) {
        writeInt(ptr, (ਪੂਰਨ) strofs);
        ptr += 4;
    }

    ਜੇ (elem_type == File_ਗੰਢ::SEQ || elem_type == File_ਗੰਢ::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    ਜੇ (value)
        ਗੰਢ.ਸੈੱਟ_ਮੁੱਲ(elem_type, value, len);

    ਜੇ (collection.isNamed())
        cp += 4;
    ਪੂਰਨ nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    ਵਾਪਸ ਗੰਢ;
}

ਖਾਲੀ FileStorage::Impl::finalizeCollection(File_ਗੰਢ &collection) {
    ਜੇ (!collection.isSeq() && !collection.isMap())
        ਵਾਪਸ;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    ਜੇ (*ptr0 & File_ਗੰਢ::NAMED)
        ptr += 4;
    size_t ਖੰਡ_Idx = collection.ਖੰਡ_Idx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t raw_ਮਾਪ = 4;
    ਬਿਨਾਨਿਸ਼ਾਨੀ sz = (ਬਿਨਾਨਿਸ਼ਾਨੀ) readInt(ptr + 4);
    ਜੇ (sz > 0) {
        size_t ਆਖਰੀ_ਖੰਡ_Idx = fs_ਅੰਕੜੇ_ptrs.size() - 1;

        ਲਈ (; ਖੰਡ_Idx < ਆਖਰੀ_ਖੰਡ_Idx; ਖੰਡ_Idx++) {
            raw_ਮਾਪ += fs_ਅੰਕੜੇ_blksz[ਖੰਡ_Idx] - ofs;
            ofs = 0;
        }
    }
    raw_ਮਾਪ += freeSpaceOfs - ofs;
    writeInt(ptr, (ਪੂਰਨ) raw_ਮਾਪ);
}

ਖਾਲੀ FileStorage::Impl::normalize_ਗੰਢ_Ofs(size_t &ਖੰਡ_Idx, size_t &ofs) ਅਚਲ {
    ਜਦੋਂ (ofs >= fs_ਅੰਕੜੇ_blksz[ਖੰਡ_Idx]) {
        ਜੇ (ਖੰਡ_Idx == fs_ਅੰਕੜੇ_blksz.size() - 1) {
            CV_Assert(ofs == fs_ਅੰਕੜੇ_blksz[ਖੰਡ_Idx]);
            ਰੋਕੋ;
        }
        ofs -= fs_ਅੰਕੜੇ_blksz[ਖੰਡ_Idx];
        ਖੰਡ_Idx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_ਦਾ_writing_base64() {
    ਵਾਪਸ state_ਦਾ_writing_base64;
}

ਪੂਰਨ FileStorage::Impl::get_space() {
    ਵਾਪਸ space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = ਸੱਚ;
}

ਖਾਲੀ FileStorage::Impl::Base64Decoder::init(ਅਚਲ Ptr<FileStorageParser> &_parser, ਅੱਖਰ *_ptr, ਪੂਰਨ _indent) {
    parser_do_not_use_ਸਿੱਧਾ_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = ਝੂਠ;
}

ਤਾਰਕਿਕ FileStorage::Impl::Base64Decoder::read_ਹੋਰ(ਪੂਰਨ needed) {
    ਸਥਿਰ ਅਚਲ uchar base64tab[] =
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

    ਜੇ (eos)
        ਵਾਪਸ ਝੂਠ;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    ਲਈ (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    ਅੱਖਰ *beg = 0, *end = 0;
    ਤਾਰਕਿਕ ok = getParser().get_Base64_ਪੰਕਤੀ(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    ਜੇ (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = ਸੱਚ;
        size_t tc = totalchars;
        ਲਈ (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    ਪੂਰਨ i = 0, j, n = (ਪੂਰਨ) encoded.size();
    ਜੇ (n > 0) {
        ਅਚਲ uchar *tab = base64tab;
        ਅੱਖਰ *src = &encoded[0];

        ਲਈ (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(ਪੂਰਨ) (uchar) src[i]], c = tab[(ਪੂਰਨ) (uchar) src[i + 1]];
            uchar b = tab[(ਪੂਰਨ) (uchar) src[i + 2]], a = tab[(ਪੂਰਨ) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    ਜੇ (i > 0 && encoded[i - 1] == '=') {
        ਜੇ (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        ਜੇ (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    ਲਈ (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    ਵਾਪਸ (ਪੂਰਨ) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    ਜੇ (ofs >= sz && !read_ਹੋਰ(1))
        ਵਾਪਸ (uchar) 0;
    ਵਾਪਸ decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    ਜੇ (ofs + 2 > sz && !read_ਹੋਰ(2))
        ਵਾਪਸ (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    ਵਾਪਸ val;
}

ਪੂਰਨ FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    ਜੇ (ofs + 4 > sz && !read_ਹੋਰ(4))
        ਵਾਪਸ 0;
    ਪੂਰਨ ival = readInt(&decoded[ofs]);
    ofs += 4;
    ਵਾਪਸ ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    ਜੇ (ofs + 8 > sz && !read_ਹੋਰ(8))
        ਵਾਪਸ 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    ਵਾਪਸ ival;
}

ਦੁੱਗਣਾ FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    ਜੇ (ofs + 8 > sz && !read_ਹੋਰ(8))
        ਵਾਪਸ 0;
    ਦੁੱਗਣਾ fval = readReal(&decoded[ofs]);
    ofs += 8;
    ਵਾਪਸ fval;
}

ਤਾਰਕਿਕ FileStorage::Impl::Base64Decoder::ਸਿਰਾ_ਦਾ_ਧਾਰਾ() ਅਚਲ { ਵਾਪਸ eos; }

ਅੱਖਰ *FileStorage::Impl::Base64Decoder::getPtr() ਅਚਲ { ਵਾਪਸ ptr; }


ਅੱਖਰ *FileStorage::Impl::parseBase64(ਅੱਖਰ *ptr, ਪੂਰਨ indent, File_ਗੰਢ &collection) {
    ਅਚਲ ਪੂਰਨ BASE64_HDR_ਮਾਪ = 24;
    ਅੱਖਰ dt[BASE64_HDR_ਮਾਪ + 1] = {0};
    base64decoder.init(parser_do_not_use_ਸਿੱਧਾ_dereference, ptr, indent);

    ਪੂਰਨ i, k;

    ਲਈ (i = 0; i < BASE64_HDR_ਮਾਪ; i++)
        dt[i] = (ਅੱਖਰ) base64decoder.getUInt8();
    ਲਈ (i = 0; i < BASE64_HDR_ਮਾਪ; i++)
        ਜੇ (ਖਾਲੀ_ਥਾਂ_ਹੈ(dt[i]))
            ਰੋਕੋ;
    dt[i] = '\0';

    CV_Assert(!base64decoder.ਸਿਰਾ_ਦਾ_ਧਾਰਾ());

    ਪੂਰਨ fmt_ਜੋੜੇ[CV_FS_MAX_FMT_ਜੋੜੇ * 2];
    ਪੂਰਨ fmt_pair_ਗਿਣਤੀ = fs::decodeFormat(dt, fmt_ਜੋੜੇ, CV_FS_MAX_FMT_ਜੋੜੇ);
    int64_t ival = 0;
    ਦੁੱਗਣਾ fval = 0;

    ਲਈ (;;) {
        ਲਈ (k = 0; k < fmt_pair_ਗਿਣਤੀ; k++) {
            ਪੂਰਨ elem_type = fmt_ਜੋੜੇ[k * 2 + 1];
            ਪੂਰਨ ਗਿਣਤੀ_3 = fmt_ਜੋੜੇ[k * 2];

            ਲਈ (i = 0; i < ਗਿਣਤੀ_3; i++) {
                ਪੂਰਨ ਗੰਢ_type = File_ਗੰਢ::INT;
                ਚੋਣ (elem_type) {
                    ਹਾਲਤ CV_8U:
                        ival = base64decoder.getUInt8();
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_8S:
                        ival = (ਅੱਖਰ) base64decoder.getUInt8();
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_16U:
                        ival = base64decoder.getUInt16();
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_16S:
                        ival = (ਛੋਟਾ) base64decoder.getUInt16();
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_32U:
                        ival = base64decoder.getInt32();
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_32S:
                        ival = base64decoder.getInt32();
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_64U:
                        ival = base64decoder.getInt64();
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_64S:
                        ival = base64decoder.getInt64();
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        ਗੰਢ_type = File_ਗੰਢ::REAL;
                    }
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_64F:
                        fval = base64decoder.getFloat64();
                        ਗੰਢ_type = File_ਗੰਢ::REAL;
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_16F:
                        fval = ਦਸ਼ਮਲਵ(hfloat_From_ਬਿੱਟ(base64decoder.getUInt16()));
                        ਗੰਢ_type = File_ਗੰਢ::REAL;
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        ਗੰਢ_type = File_ਗੰਢ::REAL;
                        ਰੋਕੋ;
                    ਹਾਲਤ CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        ਗੰਢ_type = File_ਗੰਢ::REAL;
                        ਰੋਕੋ;
                    ਮੂਲ:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                ਜੇ (base64decoder.ਸਿਰਾ_ਦਾ_ਧਾਰਾ())
                    ਰੋਕੋ;
                ਜੋੜੋ_ਗੰਢ(collection, std::string(), ਗੰਢ_type,
                        ਗੰਢ_type == File_ਗੰਢ::INT ? (ਖਾਲੀ *) &ival : (ਖਾਲੀ *) &fval, -1);
            }
        }
        ਜੇ (base64decoder.ਸਿਰਾ_ਦਾ_ਧਾਰਾ())
            ਰੋਕੋ;
    }

    finalizeCollection(collection);
    ਵਾਪਸ base64decoder.getPtr();
}

ਖਾਲੀ FileStorage::Impl::parseError(ਅਚਲ ਅੱਖਰ *func_name, ਅਚਲ std::string &err_ਸੁਨੇਹਾ, ਅਚਲ ਅੱਖਰ *ਸਰੋਤ_file,
                                   ਪੂਰਨ ਸਰੋਤ_ਸਤਰ) {
    std::string ਸੁਨੇਹਾ = format("%s(%d): %s", filename.c_str(), lineno, err_ਸੁਨੇਹਾ.c_str());
    error(Error::StsParseError, func_name, ਸੁਨੇਹਾ.c_str(), ਸਰੋਤ_file, ਸਰੋਤ_ਸਤਰ);
}

ਅਚਲ uchar *FileStorage::Impl::get_ਗੰਢ_Ptr(size_t ਖੰਡ_Idx, size_t ofs) ਅਚਲ {
    CV_Assert(ਖੰਡ_Idx < fs_ਅੰਕੜੇ_ptrs.size());
    CV_Assert(ofs < fs_ਅੰਕੜੇ_blksz[ਖੰਡ_Idx]);

    ਵਾਪਸ fs_ਅੰਕੜੇ_ptrs[ਖੰਡ_Idx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) ਅਚਲ {
    CV_Assert(nameofs < str_hash_ਅੰਕੜੇ.size());
    ਵਾਪਸ std::string(&str_hash_ਅੰਕੜੇ[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { ਵਾਪਸ fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(ਇਹ);
}

FileStorage::FileStorage(ਅਚਲ String& filename, ਪੂਰਨ flags, ਅਚਲ String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(ਇਹ);
    ਤਾਰਕਿਕ ok = p->open(filename.c_str(), flags, encoding.c_str());
    ਜੇ(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

ਖਾਲੀ FileStorage::ਸ਼ੁਰੂ_Write_Struct(ਅਚਲ String& name, ਪੂਰਨ struct_flags, ਅਚਲ String& typeName)
{
    p->ਸ਼ੁਰੂ_Write_Struct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    ਜੇ ((struct_flags & File_ਗੰਢ::TYPE_ਨਕਾਬ) == File_ਗੰਢ::SEQ)
        state = FileStorage::ਮੁੱਲ_EXPECTED;
    ਨਹੀਂਤਾਂ
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

ਖਾਲੀ FileStorage::ਸਿਰਾ_Write_Struct()
{
    p->ਸਿਰਾ_Write_Struct();
    state = p->write_ਢੇਰ.empty() || File_ਗੰਢ::isMap(p->write_ਢੇਰ.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::ਮੁੱਲ_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

ਤਾਰਕਿਕ FileStorage::open(ਅਚਲ String& filename, ਪੂਰਨ flags, ਅਚਲ String& encoding)
{
    ਕੋਸ਼ਿਸ਼
    {
        ਤਾਰਕਿਕ ok = p->open(filename.c_str(), flags, encoding.c_str());
        ਜੇ(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        ਵਾਪਸ ok;
    }
    ਫੜੋ (...)
    {
        release();
        ਸੁੱਟੋ;  // re-throw
    }
}

ਤਾਰਕਿਕ FileStorage::isOpened() ਅਚਲ { ਵਾਪਸ p->is_opened; }

ਖਾਲੀ FileStorage::release()
{
    p->release();
}

File_ਗੰਢ FileStorage::ਜੜ੍ਹ(ਪੂਰਨ i) ਅਚਲ
{
    ਜੇ( p.empty() || p->roots.empty() || i < 0 || i >= (ਪੂਰਨ)p->roots.size() )
        ਵਾਪਸ File_ਗੰਢ();

    ਵਾਪਸ p->roots[i];
}

File_ਗੰਢ FileStorage::get_ਪਹਿਲਾ_ਸਿਖਰ_Level_ਗੰਢ() ਅਚਲ
{
    File_ਗੰਢ r = ਜੜ੍ਹ();
    File_ਗੰਢ_Iterator it = r.begin();
    ਵਾਪਸ it != r.end() ? *it : File_ਗੰਢ();
}

std::string FileStorage::getDefaultObjectName(ਅਚਲ std::string& _filename)
{
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ* stubname = "unnamed";
    ਅਚਲ ਅੱਖਰ* filename = _filename.c_str();
    ਅਚਲ ਅੱਖਰ* ptr2 = filename + _filename.size();
    ਅਚਲ ਅੱਖਰ* ptr = ptr2 - 1;
    cv::Auto_ਬਫਰ<ਅੱਖਰ> name_buf(_filename.size()+1);

    ਜਦੋਂ( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        ਜੇ( *ptr == '.' && (!*ptr2 || ਲੜੀ_ਤੁਲਨਾ(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    ਜੇ( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    ਅੱਖਰ* name = name_buf.ਅੰਕੜੇ();

    // name must start with letter or '_'
    ਜੇ( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    ਜਦੋਂ( ptr < ptr2 )
    {
        ਅੱਖਰ c = *ptr++;
        ਜੇ( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.ਅੰਕੜੇ();
    ਜੇ( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    ਵਾਪਸ name;
}


ਪੂਰਨ FileStorage::getFormat() ਅਚਲ
{
    ਵਾਪਸ p->fmt;
}

File_ਗੰਢ FileStorage::ਸੰਚਾਲਕ [](ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ) ਅਚਲ
{
    ਵਾਪਸ ਇਹ->ਸੰਚਾਲਕ[](std::string(ਕੁੰਜੀ));
}

File_ਗੰਢ FileStorage::ਸੰਚਾਲਕ [](ਅਚਲ std::string& ਕੁੰਜੀ) ਅਚਲ
{
    File_ਗੰਢ res;
    ਲਈ (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][ਕੁੰਜੀ];
        ਜੇ (!res.empty())
            ਰੋਕੋ;
    }
    ਵਾਪਸ res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    ਵਾਪਸ buf;
}

ਖਾਲੀ FileStorage::writeRaw( ਅਚਲ String& fmt, ਅਚਲ ਖਾਲੀ* vec, size_t len )
{
    p->write_Raw_ਅੰਕੜੇ(fmt, (ਅਚਲ uchar*)vec, len);
}

ਖਾਲੀ FileStorage::writeComment( ਅਚਲ String& comment, ਤਾਰਕਿਕ eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

ਖਾਲੀ writeScalar( FileStorage& fs, ਪੂਰਨ value )
{
    fs.p->ਲਿਖ(String(), value);
}

ਖਾਲੀ writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->ਲਿਖ(String(), value);
}

ਖਾਲੀ writeScalar( FileStorage& fs, ਦਸ਼ਮਲਵ value )
{
    fs.p->ਲਿਖ(String(), (ਦੁੱਗਣਾ)value);
}

ਖਾਲੀ writeScalar( FileStorage& fs, ਦੁੱਗਣਾ value )
{
    fs.p->ਲਿਖ(String(), value);
}

ਖਾਲੀ writeScalar( FileStorage& fs, ਅਚਲ String& value )
{
    fs.p->ਲਿਖ(String(), value);
}

ਖਾਲੀ ਲਿਖ( FileStorage& fs, ਅਚਲ String& name, ਪੂਰਨ value )
{
    fs.p->ਲਿਖ(name, value);
}

ਖਾਲੀ ਲਿਖ( FileStorage& fs, ਅਚਲ String& name, int64_t value )
{
    fs.p->ਲਿਖ(name, value);
}

ਖਾਲੀ ਲਿਖ( FileStorage& fs, ਅਚਲ String& name, ਦਸ਼ਮਲਵ value )
{
    fs.p->ਲਿਖ(name, (ਦੁੱਗਣਾ)value);
}

ਖਾਲੀ ਲਿਖ( FileStorage& fs, ਅਚਲ String& name, ਦੁੱਗਣਾ value )
{
    fs.p->ਲਿਖ(name, value);
}

ਖਾਲੀ ਲਿਖ( FileStorage& fs, ਅਚਲ String& name, ਅਚਲ String& value )
{
    fs.p->ਲਿਖ(name, value);
}

ਖਾਲੀ ਲਿਖ( FileStorage& fs, ਅਚਲ String& name, ਤਾਰਕਿਕ value )
{
    fs.p->ਲਿਖ(name, value);
}

ਖਾਲੀ FileStorage::ਲਿਖ(ਅਚਲ String& name, ਪੂਰਨ val) { p->ਲਿਖ(name, val); }
ਖਾਲੀ FileStorage::ਲਿਖ(ਅਚਲ String& name, ਤਾਰਕਿਕ val) { p->ਲਿਖ(name, val); }
ਖਾਲੀ FileStorage::ਲਿਖ(ਅਚਲ String& name, int64_t val) { p->ਲਿਖ(name, val); }
ਖਾਲੀ FileStorage::ਲਿਖ(ਅਚਲ String& name, ਦੁੱਗਣਾ val) { p->ਲਿਖ(name, val); }
ਖਾਲੀ FileStorage::ਲਿਖ(ਅਚਲ String& name, ਅਚਲ String& val) { p->ਲਿਖ(name, val); }
ਖਾਲੀ FileStorage::ਲਿਖ(ਅਚਲ String& name, ਅਚਲ Mat& val) { cv::ਲਿਖ(*ਇਹ, name, val); }
ਖਾਲੀ FileStorage::ਲਿਖ(ਅਚਲ String& name, ਅਚਲ std::vector<String>& val) { cv::ਲਿਖ(*ਇਹ, name, val); }

FileStorage& ਸੰਚਾਲਕ << (FileStorage& fs, ਅਚਲ String& str)
{
    ਗਿਣਤੀ { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        ਮੁੱਲ_EXPECTED = FileStorage::ਮੁੱਲ_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    ਅਚਲ ਅੱਖਰ* _str = str.c_str();
    ਜੇ( !fs.isOpened() || !_str )
        ਵਾਪਸ fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    ਅੱਖਰ c = *_str;

    ਜੇ( c == '}' || c == ']' )
    {
        ਜੇ( fs_impl->write_ਢੇਰ.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        ਪੂਰਨ struct_flags = fs_impl->write_ਢੇਰ.back().flags;
        ਅੱਖਰ expected_bracket = File_ਗੰਢ::isMap(struct_flags) ? '}' : ']';
        ਜੇ( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->ਸਿਰਾ_Write_Struct();
        CV_Assert(!fs_impl->write_ਢੇਰ.empty());
        struct_flags = fs_impl->write_ਢੇਰ.back().flags;
        fs.state = File_ਗੰਢ::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : ਮੁੱਲ_EXPECTED;
        fs.elname = String();
    }
    ਨਹੀਂਤਾਂ ਜੇ( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        ਜੇ (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = ਮੁੱਲ_EXPECTED + INSIDE_MAP;
    }
    ਨਹੀਂਤਾਂ ਜੇ( (fs.state & 3) == ਮੁੱਲ_EXPECTED )
    {
        ਜੇ( c == '{' || c == '[' )
        {
            ਪੂਰਨ struct_flags = c == '{' ? File_ਗੰਢ::MAP : File_ਗੰਢ::SEQ;
            fs.state = struct_flags == File_ਗੰਢ::MAP ? INSIDE_MAP + NAME_EXPECTED : ਮੁੱਲ_EXPECTED;
            _str++;
            ਜੇ( *_str == ':' )
            {
                _str++;
                ਜੇ( !*_str )
                    struct_flags |= File_ਗੰਢ::FLOW;
            }
            fs_impl->ਸ਼ੁਰੂ_Write_Struct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        ਨਹੀਂਤਾਂ
        {
            ਲਿਖ( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            ਜੇ( fs.state == INSIDE_MAP + ਮੁੱਲ_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    ਨਹੀਂਤਾਂ
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    ਵਾਪਸ fs;
}


File_ਗੰਢ::File_ਗੰਢ()
    : fs(ਨਿਰਾ)
{
    ਖੰਡ_Idx = ofs = 0;
}

File_ਗੰਢ::File_ਗੰਢ(FileStorage::Impl* _fs, size_t ਖੰਡ_Idx_2, size_t _ofs)
    : fs(_fs)
{
    ਖੰਡ_Idx = ਖੰਡ_Idx_2;
    ofs = _ofs;
}

File_ਗੰਢ::File_ਗੰਢ(ਅਚਲ FileStorage* _fs, size_t ਖੰਡ_Idx_2, size_t _ofs)
    : File_ਗੰਢ(_fs->p.get(), ਖੰਡ_Idx_2, _ofs)
{
    // nothing
}

File_ਗੰਢ::File_ਗੰਢ(ਅਚਲ File_ਗੰਢ& ਗੰਢ)
{
    fs = ਗੰਢ.fs;
    ਖੰਡ_Idx = ਗੰਢ.ਖੰਡ_Idx;
    ofs = ਗੰਢ.ofs;
}

File_ਗੰਢ& File_ਗੰਢ::ਸੰਚਾਲਕ=(ਅਚਲ File_ਗੰਢ& ਗੰਢ)
{
    fs = ਗੰਢ.fs;
    ਖੰਡ_Idx = ਗੰਢ.ਖੰਡ_Idx;
    ofs = ਗੰਢ.ofs;
    ਵਾਪਸ *ਇਹ;
}

File_ਗੰਢ File_ਗੰਢ::ਸੰਚਾਲਕ[](ਅਚਲ std::string& nodename) ਅਚਲ
{
    ਜੇ(!fs)
        ਵਾਪਸ File_ਗੰਢ();

    CV_Assert( isMap() );

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੰਜੀ = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_ਗੰਢ_Iterator it = begin();

    ਲਈ( i = 0; i < sz; i++, ++it )
    {
        File_ਗੰਢ n = *it;
        ਅਚਲ uchar* p = n.ptr();
        ਬਿਨਾਨਿਸ਼ਾਨੀ key2 = (ਬਿਨਾਨਿਸ਼ਾਨੀ)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_ਅੰਕੜੇ.size() );
        ਜੇ( ਕੁੰਜੀ == key2 )
            ਵਾਪਸ n;
    }
    ਵਾਪਸ File_ਗੰਢ();
}

File_ਗੰਢ File_ਗੰਢ::ਸੰਚਾਲਕ[](ਅਚਲ ਅੱਖਰ* nodename) ਅਚਲ
{
    ਵਾਪਸ ਇਹ->ਸੰਚਾਲਕ[](std::string(nodename));
}

File_ਗੰਢ File_ਗੰਢ::ਸੰਚਾਲਕ[](ਪੂਰਨ i) ਅਚਲ
{
    ਜੇ(!fs)
        ਵਾਪਸ File_ਗੰਢ();

    CV_Assert( isSeq() );

    ਪੂਰਨ sz = (ਪੂਰਨ)size();
    CV_Assert( 0 <= i && i < sz );

    File_ਗੰਢ_Iterator it = begin();
    it += i;

    ਵਾਪਸ *it;
}

std::vector<String> File_ਗੰਢ::ਕੁੰਜੀਆਂ() ਅਚਲ
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    ਲਈ (File_ਗੰਢ_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    ਵਾਪਸ res;
}

ਪੂਰਨ File_ਗੰਢ::type() ਅਚਲ
{
    ਅਚਲ uchar* p = ptr();
    ਜੇ(!p)
        ਵਾਪਸ NONE;
    ਵਾਪਸ (*p & TYPE_ਨਕਾਬ);
}

ਤਾਰਕਿਕ File_ਗੰਢ::isMap(ਪੂਰਨ flags) { ਵਾਪਸ (flags & TYPE_ਨਕਾਬ) == MAP; }
ਤਾਰਕਿਕ File_ਗੰਢ::isSeq(ਪੂਰਨ flags) { ਵਾਪਸ (flags & TYPE_ਨਕਾਬ) == SEQ; }
ਤਾਰਕਿਕ File_ਗੰਢ::isCollection(ਪੂਰਨ flags) { ਵਾਪਸ isMap(flags) || isSeq(flags); }
ਤਾਰਕਿਕ File_ਗੰਢ::isFlow(ਪੂਰਨ flags) { ਵਾਪਸ (flags & FLOW) != 0; }
ਤਾਰਕਿਕ File_ਗੰਢ::is_ਖਾਲੀ_ਥਾਵਾਂ_Collection(ਪੂਰਨ flags) { ਵਾਪਸ (flags & ਖਾਲੀ_ਥਾਵਾਂ_2) != 0; }

ਤਾਰਕਿਕ File_ਗੰਢ::empty() ਅਚਲ   { ਵਾਪਸ fs == 0; }
ਤਾਰਕਿਕ File_ਗੰਢ::isNone() ਅਚਲ  { ਵਾਪਸ type() == NONE; }
ਤਾਰਕਿਕ File_ਗੰਢ::isSeq() ਅਚਲ   { ਵਾਪਸ type() == SEQ; }
ਤਾਰਕਿਕ File_ਗੰਢ::isMap() ਅਚਲ   { ਵਾਪਸ type() == MAP; }
ਤਾਰਕਿਕ File_ਗੰਢ::isInt() ਅਚਲ   { ਵਾਪਸ type() == INT;  }
ਤਾਰਕਿਕ File_ਗੰਢ::isReal() ਅਚਲ  { ਵਾਪਸ type() == REAL; }
ਤਾਰਕਿਕ File_ਗੰਢ::isString() ਅਚਲ { ਵਾਪਸ type() == STRING;  }
ਤਾਰਕਿਕ File_ਗੰਢ::isNamed() ਅਚਲ
{
    ਅਚਲ uchar* p = ptr();
    ਜੇ(!p)
        ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ (*p & NAMED) != 0;
}

std::string File_ਗੰਢ::name() ਅਚਲ
{
    ਅਚਲ uchar* p = ptr();
    ਜੇ(!p)
        ਵਾਪਸ std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    ਵਾਪਸ fs->getName(nameofs);
}

File_ਗੰਢ::ਸੰਚਾਲਕ ਪੂਰਨ() ਅਚਲ
{
    ਅਚਲ uchar* p = ptr();
    ਜੇ(!p)
        ਵਾਪਸ 0;
    ਪੂਰਨ ਟੈਗ = *p;
    ਪੂਰਨ type = (ਟੈਗ & TYPE_ਨਕਾਬ);
    p += (ਟੈਗ & NAMED) ? 5 : 1;

    ਜੇ( type == INT )
    {
        ਵਾਪਸ readInt(p);
    }
    ਨਹੀਂਤਾਂ ਜੇ( type == REAL )
    {
        ਵਾਪਸ cv_ਗੇੜਾ(readReal(p));
    }
    ਨਹੀਂਤਾਂ
        ਵਾਪਸ 0x7fffffff;
}

File_ਗੰਢ::ਸੰਚਾਲਕ int64_t() ਅਚਲ
{
    ਅਚਲ uchar* p = ptr();
    ਜੇ(!p)
        ਵਾਪਸ 0;
    ਪੂਰਨ ਟੈਗ = *p;
    ਪੂਰਨ type = (ਟੈਗ & TYPE_ਨਕਾਬ);
    p += (ਟੈਗ & NAMED) ? 5 : 1;

    ਜੇ( type == INT )
    {
        ਵਾਪਸ readLong(p);
    }
    ਨਹੀਂਤਾਂ ਜੇ( type == REAL )
    {
        ਵਾਪਸ cv_ਗੇੜਾ(readReal(p));
    }
    ਨਹੀਂਤਾਂ
        ਵਾਪਸ 0x7fffffff;
}

File_ਗੰਢ::ਸੰਚਾਲਕ ਦਸ਼ਮਲਵ() ਅਚਲ
{
    ਅਚਲ uchar* p = ptr();
    ਜੇ(!p)
        ਵਾਪਸ 0.f;
    ਪੂਰਨ ਟੈਗ = *p;
    ਪੂਰਨ type = (ਟੈਗ & TYPE_ਨਕਾਬ);
    p += (ਟੈਗ & NAMED) ? 5 : 1;

    ਜੇ( type == INT )
    {
        ਵਾਪਸ (ਦਸ਼ਮਲਵ)readLong(p);
    }
    ਨਹੀਂਤਾਂ ਜੇ( type == REAL )
    {
        ਵਾਪਸ (ਦਸ਼ਮਲਵ)readReal(p);
    }
    ਨਹੀਂਤਾਂ
        ਵਾਪਸ FLT_MAX;
}

File_ਗੰਢ::ਸੰਚਾਲਕ ਦੁੱਗਣਾ() ਅਚਲ
{
    ਅਚਲ uchar* p = ptr();
    ਜੇ(!p)
        ਵਾਪਸ 0.f;
    ਪੂਰਨ ਟੈਗ = *p;
    ਪੂਰਨ type = (ਟੈਗ & TYPE_ਨਕਾਬ);
    p += (ਟੈਗ & NAMED) ? 5 : 1;

    ਜੇ( type == INT )
    {
        ਵਾਪਸ (ਦੁੱਗਣਾ)readLong(p);
    }
    ਨਹੀਂਤਾਂ ਜੇ( type == REAL )
    {
        ਵਾਪਸ readReal(p);
    }
    ਨਹੀਂਤਾਂ
        ਵਾਪਸ DBL_MAX;
}

ਦੁੱਗਣਾ File_ਗੰਢ::real() ਅਚਲ  { ਵਾਪਸ ਦੁੱਗਣਾ(*ਇਹ); }

std::string File_ਗੰਢ::string() ਅਚਲ
{
    ਅਚਲ uchar* p = ptr();
    ਜੇ( !p || (*p & TYPE_ਨਕਾਬ) != STRING )
        ਵਾਪਸ std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(ਬਿਨਾਨਿਸ਼ਾਨੀ)readInt(p);
    ਵਾਪਸ std::string((ਅਚਲ ਅੱਖਰ*)(p + 4), sz - 1);
}
Mat File_ਗੰਢ::mat() ਅਚਲ { Mat value; ਪਠ(*ਇਹ, value, Mat()); ਵਾਪਸ value; }

File_ਗੰਢ_Iterator File_ਗੰਢ::begin() ਅਚਲ { ਵਾਪਸ File_ਗੰਢ_Iterator(*ਇਹ, ਝੂਠ); }
File_ਗੰਢ_Iterator File_ਗੰਢ::end() ਅਚਲ   { ਵਾਪਸ File_ਗੰਢ_Iterator(*ਇਹ, ਸੱਚ); }

ਖਾਲੀ File_ਗੰਢ::readRaw( ਅਚਲ std::string& fmt, ਖਾਲੀ* vec, size_t len ) ਅਚਲ
{
    File_ਗੰਢ_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_ਗੰਢ::size() ਅਚਲ
{
    ਅਚਲ uchar* p = ptr();
    ਜੇ( !p )
        ਵਾਪਸ 0;
    ਪੂਰਨ ਟੈਗ = *p;
    ਪੂਰਨ tp = ਟੈਗ & TYPE_ਨਕਾਬ;
    ਜੇ( tp == MAP || tp == SEQ )
    {
        ਜੇ( ਟੈਗ & NAMED )
            p += 4;
        ਵਾਪਸ (size_t)(ਬਿਨਾਨਿਸ਼ਾਨੀ)readInt(p + 5);
    }
    ਵਾਪਸ tp != NONE;
}

size_t File_ਗੰਢ::raw_ਮਾਪ() ਅਚਲ
{
    ਅਚਲ uchar* p0 = ptr(), *p = p0;
    ਜੇ( !p )
        ਵਾਪਸ 0;
    ਪੂਰਨ ਟੈਗ = *p++;
    ਪੂਰਨ tp = ਟੈਗ & TYPE_ਨਕਾਬ;
    ਜੇ( ਟੈਗ & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    ਜੇ( tp == INT )
        ਵਾਪਸ sz0 + 8;
    ਜੇ( tp == REAL )
        ਵਾਪਸ sz0 + 8;
    ਜੇ( tp == NONE )
        ਵਾਪਸ sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    ਵਾਪਸ sz0 + 4 + readInt(p);
}

uchar* File_ਗੰਢ::ptr()
{
    ਵਾਪਸ !fs ? 0 : (uchar*)fs->get_ਗੰਢ_Ptr(ਖੰਡ_Idx, ofs);
}

ਅਚਲ uchar* File_ਗੰਢ::ptr() ਅਚਲ
{
    ਵਾਪਸ !fs ? 0 : fs->get_ਗੰਢ_Ptr(ਖੰਡ_Idx, ofs);
}

ਖਾਲੀ File_ਗੰਢ::ਸੈੱਟ_ਮੁੱਲ( ਪੂਰਨ type, ਅਚਲ ਖਾਲੀ* value, ਪੂਰਨ len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    ਪੂਰਨ ਟੈਗ = *p;
    ਪੂਰਨ current_type = ਟੈਗ & TYPE_ਨਕਾਬ;
    CV_Assert( current_type == NONE || current_type == type );

    ਪੂਰਨ sz = 1;

    ਜੇ( ਟੈਗ & NAMED )
        sz += 4;

    ਜੇ( type == INT )
        sz += 8;
    ਨਹੀਂਤਾਂ ਜੇ( type == REAL )
        sz += 8;
    ਨਹੀਂਤਾਂ ਜੇ( type == STRING )
    {
        ਜੇ( len < 0 )
            len = (ਪੂਰਨ)ਲੜੀ_ਲੰਬਾਈ((ਅਚਲ ਅੱਖਰ*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_ਗੰਢ_Space(*ਇਹ, sz);
    *p++ = (uchar)(type | (ਟੈਗ & NAMED));
    ਜੇ( ਟੈਗ & NAMED )
        p += 4;

    ਜੇ( type == INT )
    {
        int64_t ival = *(ਅਚਲ int64_t*)value;
        writeInt(p, ival);
    }
    ਨਹੀਂਤਾਂ ਜੇ( type == REAL )
    {
        ਦੁੱਗਣਾ dbval = *(ਅਚਲ ਦੁੱਗਣਾ*)value;
        writeReal(p, dbval);
    }
    ਨਹੀਂਤਾਂ ਜੇ( type == STRING )
    {
        ਅਚਲ ਅੱਖਰ* str = (ਅਚਲ ਅੱਖਰ*)value;
        writeInt(p, len + 1);
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_ਗੰਢ_Iterator::File_ਗੰਢ_Iterator()
{
    fs = 0;
    ਖੰਡ_Idx = 0;
    ofs = 0;
    ਖੰਡ_ਮਾਪ_2 = 0;
    ਗੰਢ_NElems = 0;
    idx = 0;
}

File_ਗੰਢ_Iterator::File_ਗੰਢ_Iterator( ਅਚਲ File_ਗੰਢ& ਗੰਢ, ਤਾਰਕਿਕ seek_ਸਿਰਾ )
{
    fs = ਗੰਢ.fs;
    idx = 0;
    ਜੇ( !fs )
        ਖੰਡ_Idx = ofs = ਖੰਡ_ਮਾਪ_2 = ਗੰਢ_NElems = 0;
    ਨਹੀਂਤਾਂ
    {
        ਖੰਡ_Idx = ਗੰਢ.ਖੰਡ_Idx;
        ofs = ਗੰਢ.ofs;

        ਤਾਰਕਿਕ collection = ਗੰਢ.isSeq() || ਗੰਢ.isMap();
        ਜੇ( ਗੰਢ.isNone() )
        {
            ਗੰਢ_NElems = 0;
        }
        ਨਹੀਂਤਾਂ ਜੇ( !collection )
        {
            ਗੰਢ_NElems = 1;
            ਜੇ( seek_ਸਿਰਾ )
            {
                idx = 1;
                ofs += ਗੰਢ.raw_ਮਾਪ();
            }
        }
        ਨਹੀਂਤਾਂ
        {
            ਗੰਢ_NElems = ਗੰਢ.size();
            ਅਚਲ uchar* p0 = ਗੰਢ.ptr(), *p = p0 + 1;
            ਜੇ(*p0 & File_ਗੰਢ::NAMED )
                p += 4;
            ਜੇ( !seek_ਸਿਰਾ )
                ofs += (p - p0) + 8;
            ਨਹੀਂਤਾਂ
            {
                size_t rawsz = (size_t)(ਬਿਨਾਨਿਸ਼ਾਨੀ)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = ਗੰਢ_NElems;
            }
        }
        fs->normalize_ਗੰਢ_Ofs(ਖੰਡ_Idx, ofs);
        ਖੰਡ_ਮਾਪ_2 = fs->fs_ਅੰਕੜੇ_blksz[ਖੰਡ_Idx];
    }
}

File_ਗੰਢ_Iterator::File_ਗੰਢ_Iterator(ਅਚਲ File_ਗੰਢ_Iterator& it)
{
    fs = it.fs;
    ਖੰਡ_Idx = it.ਖੰਡ_Idx;
    ofs = it.ofs;
    ਖੰਡ_ਮਾਪ_2 = it.ਖੰਡ_ਮਾਪ_2;
    ਗੰਢ_NElems = it.ਗੰਢ_NElems;
    idx = it.idx;
}

File_ਗੰਢ_Iterator& File_ਗੰਢ_Iterator::ਸੰਚਾਲਕ=(ਅਚਲ File_ਗੰਢ_Iterator& it)
{
    fs = it.fs;
    ਖੰਡ_Idx = it.ਖੰਡ_Idx;
    ofs = it.ofs;
    ਖੰਡ_ਮਾਪ_2 = it.ਖੰਡ_ਮਾਪ_2;
    ਗੰਢ_NElems = it.ਗੰਢ_NElems;
    idx = it.idx;
    ਵਾਪਸ *ਇਹ;
}

File_ਗੰਢ File_ਗੰਢ_Iterator::ਸੰਚਾਲਕ *() ਅਚਲ
{
    ਵਾਪਸ File_ਗੰਢ(idx < ਗੰਢ_NElems ? fs : ਨਿਰਾ, ਖੰਡ_Idx, ofs);
}

File_ਗੰਢ_Iterator& File_ਗੰਢ_Iterator::ਸੰਚਾਲਕ ++ ()
{
    ਜੇ( idx == ਗੰਢ_NElems || !fs )
        ਵਾਪਸ *ਇਹ;
    idx++;
    File_ਗੰਢ n(fs, ਖੰਡ_Idx, ofs);
    ofs += n.raw_ਮਾਪ();
    ਜੇ( ofs >= ਖੰਡ_ਮਾਪ_2 )
    {
        fs->normalize_ਗੰਢ_Ofs(ਖੰਡ_Idx, ofs);
        ਖੰਡ_ਮਾਪ_2 = fs->fs_ਅੰਕੜੇ_blksz[ਖੰਡ_Idx];
    }
    ਵਾਪਸ *ਇਹ;
}

File_ਗੰਢ_Iterator File_ਗੰਢ_Iterator::ਸੰਚਾਲਕ ++ (ਪੂਰਨ)
{
    File_ਗੰਢ_Iterator it = *ਇਹ;
    ++(*ਇਹ);
    ਵਾਪਸ it;
}

File_ਗੰਢ_Iterator& File_ਗੰਢ_Iterator::ਸੰਚਾਲਕ += (ਪੂਰਨ _ofs)
{
    CV_Assert( _ofs >= 0 );
    ਲਈ( ; _ofs > 0; _ofs-- )
        ਇਹ->ਸੰਚਾਲਕ ++();
    ਵਾਪਸ *ਇਹ;
}

File_ਗੰਢ_Iterator& File_ਗੰਢ_Iterator::readRaw( ਅਚਲ String& fmt, ਖਾਲੀ* _data0, size_t maxsz)
{
    ਜੇ( fs && idx < ਗੰਢ_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        ਪੂਰਨ fmt_ਜੋੜੇ[CV_FS_MAX_FMT_ਜੋੜੇ*2];
        ਪੂਰਨ fmt_pair_ਗਿਣਤੀ = fs::decodeFormat( fmt.c_str(), fmt_ਜੋੜੇ, CV_FS_MAX_FMT_ਜੋੜੇ );
        size_t esz = fs::calc_Struct_ਮਾਪ( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        ਲਈ( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            ਲਈ( ਪੂਰਨ k = 0; k < fmt_pair_ਗਿਣਤੀ; k++ )
            {
                ਪੂਰਨ elem_type = fmt_ਜੋੜੇ[k*2+1];
                ਪੂਰਨ elem_ਮਾਪ_2 = CV_ELEM_ਮਾਪ(elem_type);

                ਪੂਰਨ ਗਿਣਤੀ_3 = fmt_ਜੋੜੇ[k*2];
                offset = align_ਮਾਪ( offset, elem_ਮਾਪ_2 );
                uchar* ਅੰਕੜੇ = data0 + offset;

                ਲਈ( ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_3; i++, ++(*ਇਹ) )
                {
                    File_ਗੰਢ ਗੰਢ = *(*ਇਹ);
                    ਜੇ( ਗੰਢ.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_ਮਾਪ_2 == 8 ? (int64_t)ਗੰਢ : (ਪੂਰਨ)ਗੰਢ);
                        ਚੋਣ( elem_type )
                        {
                        ਹਾਲਤ CV_8U:
                            *(uchar*)ਅੰਕੜੇ = saturate_cast<uchar>(ival);
                            ਅੰਕੜੇ++;
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_8S:
                            *(ਅੱਖਰ*)ਅੰਕੜੇ = saturate_cast<schar>(ival);
                            ਅੰਕੜੇ++;
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_Bool:
                            *(ਤਾਰਕਿਕ*)ਅੰਕੜੇ = ival != 0;
                            ਅੰਕੜੇ++;
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_16U:
                            *(ushort*)ਅੰਕੜੇ = saturate_cast<ushort>(ival);
                            ਅੰਕੜੇ += ਆਕਾਰ(ushort);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_16S:
                            *(ਛੋਟਾ*)ਅੰਕੜੇ = saturate_cast<ਛੋਟਾ>(ival);
                            ਅੰਕੜੇ += ਆਕਾਰ(ਛੋਟਾ);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_32U:
                            *(ਬਿਨਾਨਿਸ਼ਾਨੀ*)ਅੰਕੜੇ = (ਬਿਨਾਨਿਸ਼ਾਨੀ)std::max(ival, (int64_t)0);
                            ਅੰਕੜੇ += ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_32S:
                            *(ਪੂਰਨ*)ਅੰਕੜੇ = (ਪੂਰਨ)ival;
                            ਅੰਕੜੇ += ਆਕਾਰ(ਪੂਰਨ);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_32F:
                            *(ਦਸ਼ਮਲਵ*)ਅੰਕੜੇ = (ਦਸ਼ਮਲਵ)ival;
                            ਅੰਕੜੇ += ਆਕਾਰ(ਦਸ਼ਮਲਵ);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_64U:
                            *(uint64_t*)ਅੰਕੜੇ = (uint64_t)ival;
                            ਅੰਕੜੇ += ਆਕਾਰ(uint64_t);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_64S:
                            *(int64_t*)ਅੰਕੜੇ = (int64_t)ival;
                            ਅੰਕੜੇ += ਆਕਾਰ(int64_t);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_64F:
                            *(ਦੁੱਗਣਾ*)ਅੰਕੜੇ = (ਦੁੱਗਣਾ)ival;
                            ਅੰਕੜੇ += ਆਕਾਰ(ਦੁੱਗਣਾ);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_16F:
                            *(hfloat*)ਅੰਕੜੇ = hfloat((ਦਸ਼ਮਲਵ)ival);
                            ਅੰਕੜੇ += ਆਕਾਰ(hfloat);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_16BF:
                            *(bfloat*)ਅੰਕੜੇ = bfloat((ਦਸ਼ਮਲਵ)ival);
                            ਅੰਕੜੇ += ਆਕਾਰ(bfloat);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_8F_E4M3FN:
                            *(fp8_t*)ਅੰਕੜੇ = fp8_t((ਦਸ਼ਮਲਵ)ival);
                            ਅੰਕੜੇ += ਆਕਾਰ(fp8_t);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_8F_E4M3FNUZ:
                            *(fp8a_t*)ਅੰਕੜੇ = fp8a_t((ਦਸ਼ਮਲਵ)ival);
                            ਅੰਕੜੇ += ਆਕਾਰ(fp8a_t);
                            ਰੋਕੋ;
                        ਮੂਲ:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    ਨਹੀਂਤਾਂ ਜੇ( ਗੰਢ.isReal() )
                    {
                        ਦੁੱਗਣਾ fval = (ਦੁੱਗਣਾ)ਗੰਢ;

                        ਚੋਣ( elem_type )
                        {
                        ਹਾਲਤ CV_8U:
                            *(uchar*)ਅੰਕੜੇ = saturate_cast<uchar>(fval);
                            ਅੰਕੜੇ++;
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_8S:
                            *(ਅੱਖਰ*)ਅੰਕੜੇ = saturate_cast<schar>(fval);
                            ਅੰਕੜੇ++;
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_16U:
                            *(ushort*)ਅੰਕੜੇ = saturate_cast<ushort>(fval);
                            ਅੰਕੜੇ += ਆਕਾਰ(ushort);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_16S:
                            *(ਛੋਟਾ*)ਅੰਕੜੇ = saturate_cast<ਛੋਟਾ>(fval);
                            ਅੰਕੜੇ += ਆਕਾਰ(ਛੋਟਾ);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_32U:
                            *(ਪੂਰਨ*)ਅੰਕੜੇ = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ>(fval);
                            ਅੰਕੜੇ += ਆਕਾਰ(ਪੂਰਨ);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_32S:
                            *(ਪੂਰਨ*)ਅੰਕੜੇ = saturate_cast<ਪੂਰਨ>(fval);
                            ਅੰਕੜੇ += ਆਕਾਰ(ਪੂਰਨ);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_32F:
                            *(ਦਸ਼ਮਲਵ*)ਅੰਕੜੇ = (ਦਸ਼ਮਲਵ)fval;
                            ਅੰਕੜੇ += ਆਕਾਰ(ਦਸ਼ਮਲਵ);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_64U:
                            *(uint64_t*)ਅੰਕੜੇ = (uint64_t)round(std::max(fval, 0.));
                            ਅੰਕੜੇ += ਆਕਾਰ(uint64_t);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_64S:
                            *(int64_t*)ਅੰਕੜੇ = (int64_t)round(std::max(fval, 0.));
                            ਅੰਕੜੇ += ਆਕਾਰ(int64_t);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_64F:
                            *(ਦੁੱਗਣਾ*)ਅੰਕੜੇ = fval;
                            ਅੰਕੜੇ += ਆਕਾਰ(ਦੁੱਗਣਾ);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_16F:
                            *(hfloat*)ਅੰਕੜੇ = hfloat((ਦਸ਼ਮਲਵ)fval);
                            ਅੰਕੜੇ += ਆਕਾਰ(hfloat);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_16BF:
                            *(bfloat*)ਅੰਕੜੇ = bfloat((ਦਸ਼ਮਲਵ)fval);
                            ਅੰਕੜੇ += ਆਕਾਰ(bfloat);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_8F_E4M3FN:
                            *(fp8_t*)ਅੰਕੜੇ = fp8_t((ਦਸ਼ਮਲਵ)fval);
                            ਅੰਕੜੇ += ਆਕਾਰ(fp8_t);
                            ਰੋਕੋ;
                        ਹਾਲਤ CV_8F_E4M3FNUZ:
                            *(fp8a_t*)ਅੰਕੜੇ = fp8a_t((ਦਸ਼ਮਲਵ)fval);
                            ਅੰਕੜੇ += ਆਕਾਰ(fp8a_t);
                            ਰੋਕੋ;
                        ਮੂਲ:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    ਨਹੀਂਤਾਂ
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (ਪੂਰਨ)(ਅੰਕੜੇ - data0);
            }
        }
    }

    ਵਾਪਸ *ਇਹ;
}

ਤਾਰਕਿਕ File_ਗੰਢ_Iterator::equal_ਮੰਜ਼ਿਲ(ਅਚਲ File_ਗੰਢ_Iterator& it) ਅਚਲ
{
    ਵਾਪਸ fs == it.fs && ਖੰਡ_Idx == it.ਖੰਡ_Idx && ofs == it.ofs &&
           idx == it.idx && ਗੰਢ_NElems == it.ਗੰਢ_NElems;
}

size_t File_ਗੰਢ_Iterator::ਬਚਿਆ() ਅਚਲ
{
    ਵਾਪਸ ਗੰਢ_NElems - idx;
}

ਤਾਰਕਿਕ ਸੰਚਾਲਕ == ( ਅਚਲ File_ਗੰਢ_Iterator& it1, ਅਚਲ File_ਗੰਢ_Iterator& it2 )
{
    ਵਾਪਸ it1.equal_ਮੰਜ਼ਿਲ(it2);
}

ਤਾਰਕਿਕ ਸੰਚਾਲਕ != ( ਅਚਲ File_ਗੰਢ_Iterator& it1, ਅਚਲ File_ਗੰਢ_Iterator& it2 )
{
    ਵਾਪਸ !it1.equal_ਮੰਜ਼ਿਲ(it2);
}

ਖਾਲੀ ਪਠ(ਅਚਲ File_ਗੰਢ& ਗੰਢ, ਪੂਰਨ& val, ਪੂਰਨ default_val)
{
    val = default_val;
    ਜੇ( !ਗੰਢ.empty() )
    {
        val = (ਪੂਰਨ)ਗੰਢ;
    }
}

ਖਾਲੀ ਪਠ(ਅਚਲ File_ਗੰਢ& ਗੰਢ, int64_t& val, int64_t default_val)
{
    val = default_val;
    ਜੇ( !ਗੰਢ.empty() )
    {
        val = (int64_t)ਗੰਢ;
    }
}

ਖਾਲੀ ਪਠ(ਅਚਲ File_ਗੰਢ& ਗੰਢ, ਦੁੱਗਣਾ& val, ਦੁੱਗਣਾ default_val)
{
    val = default_val;
    ਜੇ( !ਗੰਢ.empty() )
    {
        val = (ਦੁੱਗਣਾ)ਗੰਢ;
    }
}

ਖਾਲੀ ਪਠ(ਅਚਲ File_ਗੰਢ& ਗੰਢ, ਦਸ਼ਮਲਵ& val, ਦਸ਼ਮਲਵ default_val)
{
    val = default_val;
    ਜੇ( !ਗੰਢ.empty() )
    {
        val = (ਦਸ਼ਮਲਵ)ਗੰਢ;
    }
}

ਖਾਲੀ ਪਠ(ਅਚਲ File_ਗੰਢ& ਗੰਢ, std::string& val, ਅਚਲ std::string& default_val)
{
    val = default_val;
    ਜੇ( !ਗੰਢ.empty() )
    {
        val = (std::string)ਗੰਢ;
    }
}
ਖਾਲੀ FileStorage::Impl::ਲਿਖ(ਅਚਲ String &ਕੁੰਜੀ, ਤਾਰਕਿਕ value)
{
    CV_Assert(write_ਬਹੁਲਕ);
    ਜੇ (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().ਲਿਖ(ਕੁੰਜੀ.c_str(), (ਪੂਰਨ)value);
    }
    ਨਹੀਂਤਾਂ
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().ਲਿਖ(ਕੁੰਜੀ.c_str(), value ? "true" : "false", ਝੂਠ);
    }
}

FileStorage_API::~FileStorage_API() {}

ਨਾਮਾਕਾਸ਼ internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, ਅਚਲ std::string& name,
                                       ਪੂਰਨ flags, ਅਚਲ std::string& typeName)
{
    fs = &_fs;
    fs->ਸ਼ੁਰੂ_Write_Struct(name, flags, typeName);
}

WriteStructContext::~WriteStructContext()
{
    fs->ਸਿਰਾ_Write_Struct();
}

}

}
