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

#تعریف_کړه CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

نطاق cv
{

نطاق fs
{

صحيح strcasecmp(ثابت حرف* s1, ثابت حرف* s2)
{
    ثابت حرف* dummy="";
    إذا(!s1) s1=dummy;
    إذا(!s2) s2=dummy;

    size_t len1 = مزي_اوږدوالی(s1);
    size_t len2 = مزي_اوږدوالی(s2);
    size_t i, len = std::min(len1, len2);
    لكل( i = 0; i < len; i++ )
    {
        صحيح d = tolower((صحيح)s1[i]) - tolower((صحيح)s2[i]);
        إذا( d != 0 )
            أرجع d;
    }
    أرجع len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

حرف* itoa( صحيح _val, حرف* بفر, صحيح /*radix*/ )
{
    ثابت صحيح radix = 10;
    حرف* ptr=بفر + 23 /* enough even for 64-bit integers */;
    بینښه val = مطلق_قیمت(_val);

    *ptr = '\0';
    افعل
    {
        بینښه r = val / radix;
        *--ptr = (حرف)(val - (r*radix) + '0');
        val = r;
    }
    طالما( val != 0 );

    إذا( _val < 0 )
        *--ptr = '-';

    أرجع ptr;
}

حرف* itoa( int64_t _val, حرف* بفر, صحيح /*radix*/, منطقي _signed)
{
    ثابت صحيح radix = 10;
    حرف* ptr=بفر + 23 /* enough even for 64-bit integers */;
    صحيح sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : مطلق_قیمت(_val);

    *ptr = '\0';
    افعل
    {
        uint64_t r = val / radix;
        *--ptr = (حرف)(val - (r*radix) + '0');
        val = r;
    }
    طالما( val != 0 );

    إذا( sign < 0 )
        *--ptr = '-';

    أرجع ptr;
}

حرف* double_منزل_String( حرف* buf, size_t buf_اندازه, مزدوج value, منطقي explicitZero )
{
    Cv64suf val;
    بینښه ieee754_hi;

    val.f = value;
    ieee754_hi = (بینښه)(val.u >> 32);

    إذا( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        صحيح ivalue = cv_پړاو(value);
        إذا( ivalue == value )
        {
            إذا( explicitZero )
                snprintf( buf, buf_اندازه, "%d.0", ivalue );
            وإلا
                snprintf( buf, buf_اندازه, "%d.", ivalue );
        }
        وإلا
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, buf_اندازه, "%.17g", value );

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
        بینښه ieee754_lo = (بینښه)val.u;
        إذا( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        وإلا
            strcpy( buf, (صحيح)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    أرجع buf;
}

حرف* float_منزل_String( حرف* buf, size_t buf_اندازه, عائم value, منطقي halfprecision, منطقي explicitZero )
{
    Cv32suf val;
    بینښه ieee754;
    val.f = value;
    ieee754 = val.u;

    إذا( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        صحيح ivalue = cv_پړاو(value);
        إذا( ivalue == value )
        {
            إذا( explicitZero )
                snprintf( buf, buf_اندازه, "%d.0", ivalue );
            وإلا
                snprintf( buf, buf_اندازه, "%d.", ivalue );
        }
        وإلا
        {
            إذا (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, buf_اندازه, "%.5g", value);
            }
            وإلا
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, buf_اندازه, "%.9g", value);
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

    أرجع buf;
}

ساكن ثابت حرف symbols[] = "ucwsifdhHbUIneE";

ساكن حرف type_نښه(صحيح ژوروالی)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_Check_ژوروالی(ژوروالی, ژوروالی >= 0 && ژوروالی <= CV_8F_E4M3FNUZ, "");
    أرجع symbols[ژوروالی];
}

ساكن صحيح نښه_منزل_Type(حرف c)
{
    إذا (c == 'r')
        أرجع CV_SEQ_ELTYPE_PTR;
    ثابت حرف* pos = strchr( symbols, c );
    إذا( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    أرجع static_cast<صحيح>(pos - symbols);
}

حرف* encodeFormat(صحيح elem_type, حرف* dt, size_t dt_len)
{
    صحيح cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    حرف نښه = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_نښه(CV_MAT_ژوروالی(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, نښه);
    أرجع dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
حرف* encodeFormat(صحيح elem_type, حرف* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    أرجع encodeFormat(elem_type, dt, max);
}

صحيح decodeFormat( ثابت حرف* dt, صحيح* fmt_جوړې, صحيح max_len )
{
    صحيح fmt_pair_شمېر = 0;
    صحيح i = 0, k = 0, len = dt ? (صحيح)مزي_اوږدوالی(dt) : 0;

    إذا( !dt || !len )
        أرجع 0;

    CV_Assert( fmt_جوړې != 0 && max_len > 0 );
    fmt_جوړې[0] = 0;
    max_len *= 2;

    لكل( ; k < len; k++ )
    {
        حرف c = dt[k];

        إذا( cv_isdigit(c) )
        {
            صحيح شمېر_2 = c - '0';
            إذا( cv_isdigit(dt[k+1]) )
            {
                حرف* endptr = 0;
                شمېر_2 = (صحيح)strtol( dt+k, &endptr, 10 );
                k = (صحيح)(endptr - dt) - 1;
            }

            إذا( شمېر_2 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_جوړې[i] = شمېر_2;
        }
        وإلا
        {
            صحيح ژوروالی = نښه_منزل_Type(c);
            إذا( fmt_جوړې[i] == 0 )
                fmt_جوړې[i] = 1;
            fmt_جوړې[i+1] = ژوروالی;
            إذا( i > 0 && fmt_جوړې[i+1] == fmt_جوړې[i-1] )
                fmt_جوړې[i-2] += fmt_جوړې[i];
            وإلا
            {
                i += 2;
                إذا( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_جوړې[i] = 0;
        }
    }

    fmt_pair_شمېر = i/2;
    أرجع fmt_pair_شمېر;
}

صحيح calc_Elem_اندازه( ثابت حرف* dt, صحيح initial_اندازه )
{
    صحيح size = 0;
    صحيح fmt_جوړې[CV_FS_MAX_FMT_جوړې*2], i, fmt_pair_شمېر;
    صحيح comp_اندازه;

    fmt_pair_شمېر = decodeFormat( dt, fmt_جوړې, CV_FS_MAX_FMT_جوړې );
    fmt_pair_شمېر *= 2;
    لكل( i = 0, size = initial_اندازه; i < fmt_pair_شمېر; i += 2 )
    {
        comp_اندازه = CV_ELEM_اندازه(fmt_جوړې[i+1]);
        size = cvAlign( size, comp_اندازه );
        size += comp_اندازه * fmt_جوړې[i];
    }
    إذا( initial_اندازه == 0 )
    {
        comp_اندازه = CV_ELEM_اندازه(fmt_جوړې[1]);
        size = cvAlign( size, comp_اندازه );
    }
    أرجع size;
}


صحيح calc_Struct_اندازه( ثابت حرف* dt, صحيح initial_اندازه )
{
    صحيح size = calc_Elem_اندازه( dt, initial_اندازه );
    size_t elem_max_اندازه = 0;
    لكل ( ثابت حرف * type = dt; *type != '\0'; type++ )
    {
        حرف v = *type;
        إذا (v >= '0' && v <= '9')
            استمر;  // skip vector size
        بدل (v)
        {
        حالة 'u': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(uchar ) ); اكسر; }
        حالة 'b': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(منطقي  ) ); اكسر; }
        حالة 'c': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(schar ) ); اكسر; }
        حالة 'w': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(ushort) ); اكسر; }
        حالة 's': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(لنډ ) ); اكسر; }
        حالة 'i': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(صحيح   ) ); اكسر; }
        حالة 'n': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(بینښه) ); اكسر; }
        حالة 'f': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(عائم ) ); اكسر; }
        حالة 'd': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(مزدوج) ); اكسر; }
        حالة 'h': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(hfloat)); اكسر; }
        حالة 'H': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(bfloat)); اكسر; }
        حالة 'e': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(fp8_t) ); اكسر; }
        حالة 'E': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(fp8a_t)); اكسر; }
        حالة 'I': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(int64_t)); اكسر; }
        حالة 'U': { elem_max_اندازه = std::max( elem_max_اندازه, حجم(uint64_t)); اكسر; }
        افتراضي:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (حرف)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<صحيح>(elem_max_اندازه) );
    أرجع size;
}

صحيح decodeSimpleFormat( ثابت حرف* dt )
{
    صحيح elem_type = -1;
    صحيح fmt_جوړې[CV_FS_MAX_FMT_جوړې*2], fmt_pair_شمېر;

    fmt_pair_شمېر = decodeFormat( dt, fmt_جوړې, CV_FS_MAX_FMT_جوړې );
    إذا( fmt_pair_شمېر != 1 || fmt_جوړې[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_جوړې[1], fmt_جوړې[0] );

    أرجع elem_type;
}

}

#إذا defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#تعریف_کړه CV_LITTLE_ENDIAN_MEM_ACCESS 1
#وإلا
#تعریف_کړه CV_LITTLE_ENDIAN_MEM_ACCESS 0
#د_که_پای

ساكن مضمن صحيح readInt(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#إذا CV_LITTLE_ENDIAN_MEM_ACCESS
    صحيح val;
    حافظه_نقل(&val, p, حجم(val));
    أرجع val;
#وإلا
    صحيح val = (صحيح)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    أرجع val;
#د_که_پای
}

ساكن مضمن int64_t readLong(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#إذا CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    حافظه_نقل(&val, p, حجم(val));
    أرجع val;
#وإلا
    بینښه val0 = (بینښه)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    بینښه val1 = (بینښه)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    أرجع val0 | ((int64_t)val1 << 32);
#د_که_پای
}

ساكن مضمن مزدوج readReal(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#إذا CV_LITTLE_ENDIAN_MEM_ACCESS
    مزدوج val;
    حافظه_نقل(&val, p, حجم(val));
    أرجع val;
#وإلا
    بینښه val0 = (بینښه)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    بینښه val1 = (بینښه)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    أرجع val.f;
#د_که_پای
}

کينډۍ <ډول_نوم T>
ساكن مضمن فراغ writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#إذا CV_LITTLE_ENDIAN_MEM_ACCESS
    حافظه_نقل(p, &ival, حجم(ival));
#وإلا
    لكل (size_t i = 0, j = 0; i < حجم(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#د_که_پای
}

ساكن مضمن فراغ writeReal(uchar* p, مزدوج fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#إذا CV_LITTLE_ENDIAN_MEM_ACCESS
    حافظه_نقل(p, &fval, حجم(fval));
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
#د_که_پای
}



فراغ FileStorage::Impl::init() {
    flags = 0;
    بفر.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = خطأ;
    state_د_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = خطأ;
    delayed_struct_کلی = تش_اشاره;
    delayed_struct_flags = 0;
    delayed_type_name = تش_اشاره;
    base64_writer = تش_اشاره;
    is_opened = خطأ;
    dummy_eof = خطأ;
    write_ډېرپېښ = خطأ;
    mem_ډېرپېښ = خطأ;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    تشه_بهیر = ریښتیا;

    strbufv.clear();
    strbuf = 0;
    strbufsize = strbufpos = 0;
    roots.clear();

    fs_معلومات.clear();
    fs_معلومات_ptrs.clear();
    fs_معلومات_blksz.clear();
    freeSpaceOfs = 0;

    str_hash.clear();
    str_hash_معلومات.clear();
    str_hash_معلومات.resize(1);
    str_hash_معلومات[0] = '\0';

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
        إذا (write_ډېرپېښ) {
            طالما (write_ډېری.size() > 1) {
                پای_ټکی_Write_Struct();
            }
            flush();
            إذا (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            وإلا إذا (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        إذا (mem_ډېرپېښ && out) {
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
        أرجع;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    إذا (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        لكل (size_t param_beg = beg, param_پای_ټکی = beg;
             param_پای_ټکی < end;
             param_beg = param_پای_ټکی + 1) {
            param_پای_ټکی = file_name.find_first_of(parameter_separator, param_beg);
            إذا ((param_پای_ټکی == std::string::npos || param_پای_ټکی != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_پای_ټکی - param_beg));
            }
        }
    }
}

منطقي FileStorage::Impl::open(ثابت حرف *filename_or_buf, صحيح _flags, ثابت حرف *encoding) {
    منطقي ok = ریښتیا;
    release();

    منطقي append = (_flags & 3) == FileStorage::APPEND;
    mem_ډېرپېښ = (_flags & FileStorage::حافظه) != 0;

    write_ډېرپېښ = (_flags & 3) != 0;
    منطقي write_base64 = (write_ډېرپېښ || append) && (_flags & FileStorage::BASE64) != 0;

    منطقي isGZ = خطأ;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        إذا (!params.empty())
            filename = params[0];

        إذا (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_ډېرپېښ || append);
    }

    إذا (filename.size() == 0 && !mem_ډېرپېښ && !write_ډېرپېښ)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    إذا (mem_ډېرپېښ && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    إذا (!mem_ډېرپېښ) {
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
                    isGZ = ریښتیا;
                }

                وإلا إذا(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = ریښتیا;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        إذا (!isGZ) {
            file = fopen(filename.c_str(), !write_ډېرپېښ ? "rt" : !append ? "wt" : "a+t");
            إذا (!file)
            {
                CV_LOG_ERROR(عدم, "Can't open file: '" << filename << "' in " << (!write_ډېرپېښ ? "read" : !append ? "write" : "append") << " mode");
                أرجع خطأ;
            }
        } وإلا {
#إذا USE_ZLIB
            إذا (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            حرف ډېرپېښ[] = {write_ډېرپېښ ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), ډېرپېښ);
            إذا (!gzfile)
            {
                CV_LOG_ERROR(عدم, "Can't open archive: '" << filename << "' mode=" << ډېرپېښ);
                أرجع خطأ;
            }
#وإلا
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#د_که_پای
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_معلومات.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    إذا (write_ډېرپېښ) {
        fmt = flags & FileStorage::FORMAT_ماسک;

        إذا (mem_ډېرپېښ)
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
        صحيح buf_اندازه_2 = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        إذا (append) {
            fseek(file, 0, SEEK_END);
            إذا (ftell(file) == 0)
                append = خطأ;
        }

        write_ډېری.clear();
        تشه_بهیر = ریښتیا;
        write_ډېری.push_back(FStruct_معلومات("", File_غوټه::MAP | File_غوټه::تشه_2, 0));
        بفر.reserve(buf_اندازه_2 + 1024);
        بفر.resize(buf_اندازه_2);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_د_writing_base64 = FileStorage_API::Base64State::Uncertain;

        إذا (fmt == FileStorage::FORMAT_XML) {
            size_t file_اندازه_2 = file ? (size_t) ftell(file) : (size_t) 0;
            إذا (!append || file_اندازه_2 == 0) {
                إذا (encoding && *encoding != '\0') {
                    إذا (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(مزي_اوږدوالی(encoding) < 1000);
                    حرف buf[1100];
                    snprintf(buf, حجم(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } وإلا
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } وإلا {
                صحيح xml_buf_اندازه = 1 << 10;
                حرف substr[] = "</opencv_storage>";
                صحيح وروستی_occurrence = -1;
                xml_buf_اندازه = MIN(xml_buf_اندازه, صحيح(file_اندازه_2));
                fseek(file, -xml_buf_اندازه, SEEK_END);
                // find the last occurrence of </opencv_storage>
                لكل (;;) {
                    صحيح کرښه_دننه_واټن = (صحيح) ftell(file);
                    ثابت حرف *ptr0 = هذا->gets(xml_buf_اندازه);
                    ثابت حرف *ptr = عدم;
                    إذا (!ptr0)
                        اكسر;
                    ptr = ptr0;
                    لكل (;;) {
                        ptr = مزي_پلټنه(ptr, substr);
                        إذا (!ptr)
                            اكسر;
                        وروستی_occurrence = کرښه_دننه_واټن + (صحيح) (ptr - ptr0);
                        ptr += مزي_اوږدوالی(substr);
                    }
                }
                إذا (وروستی_occurrence < 0) {
                    release();
                    CV_Error(cv::Error::StsError, "Could not find </opencv_storage> in the end of file.\n");
                }
                closeFile();
                file = fopen(filename.c_str(), "r+t");
                CV_Assert(file != 0);
                fseek(file, وروستی_occurrence, SEEK_SET);
                // replace the last "</opencv_storage>" with " <!-- resumed -->", which has the same length
                puts(" <!-- resumed -->");
                fseek(file, 0, SEEK_END);
                puts("\n");
            }

            emitter_do_not_use_مستقیم_dereference = createXMLEmitter(هذا);
        } وإلا إذا (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                إذا (!append) {
                    إذا (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    وإلا
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            وإلا
                puts("...\n---\n");

            emitter_do_not_use_مستقیم_dereference = createYAMLEmitter(هذا);
        } وإلا {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            إذا (!append)
                puts("{\n");
            وإلا {
                منطقي valid = خطأ;
                اوږد roffset = 0;
                لكل (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    ثابت حرف پای_ټکی_mark = '}';
                    إذا (fgetc(file) == پای_ټکی_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = ریښتیا;
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
            write_ډېری.back().indent = 4;
            emitter_do_not_use_مستقیم_dereference = createJSONEmitter(هذا);
        }
        is_opened = ریښتیا;
    } وإلا {
        ثابت size_t buf_size0 = 40;
        بفر.resize(buf_size0);
        إذا (mem_ډېرپېښ) {
            strbuf = (حرف *) filename_or_buf;
            strbufsize = مزي_اوږدوالی(strbuf);
        }

        ثابت حرف *yaml_signature = "%YAML";
        ثابت حرف *json_signature = "{";
        ثابت حرف *xml_signature = "<?xml";
        حرف *buf = هذا->gets(16);
        CV_Assert(buf);
        حرف *bufPtr = cv_skip_BOM(buf);
        size_t buf_دننه_واټن = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        إذا (مزي_پرتله(bufPtr, yaml_signature, مزي_اوږدوالی(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        وإلا إذا (مزي_پرتله(bufPtr, json_signature, مزي_اوږدوالی(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        وإلا إذا (مزي_پرتله(bufPtr, xml_signature, مزي_اوږدوالی(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        وإلا
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = buf_دننه_واټن;
        bufofs = 0;

        حاول {
            حرف *ptr = بفر_پیل();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_غوټه ریښه_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_غوټه_Space(ریښه_nodes, 9);
            *rptr = File_غوټه::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            بدل (fmt) {
                حالة FileStorage::FORMAT_XML:
                    parser_do_not_use_مستقیم_dereference = createXMLParser(هذا);
                    اكسر;
                حالة FileStorage::FORMAT_YAML:
                حالة FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_مستقیم_dereference = createYAMLParser(هذا);
                    اكسر;
                حالة FileStorage::FORMAT_JSON:
                    parser_do_not_use_مستقیم_dereference = createJSONParser(هذا);
                    اكسر;
                افتراضي:
                    parser_do_not_use_مستقیم_dereference = Ptr<FileStorageParser>();
            }

            إذا (!parser_do_not_use_مستقیم_dereference.empty()) {
                ok = getParser().parse(ptr);
                إذا (ok) {
                    finalizeCollection(ریښه_nodes);

                    CV_Assert(!fs_معلومات_ptrs.empty());
                    File_غوټه roots_غوټه(fs_ext, 0, 0);
                    size_t i, nroots = roots_غوټه.size();
                    File_غوټه_Iterator it = roots_غوټه.begin();

                    لكل (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        امسك (...)
        {
            // FIXIT log error message
            is_opened = ریښتیا;
            release();
            ارم;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = ریښتیا;
        std::vector<حرف> tmpbuf;
        std::swap(بفر, tmpbuf);
        bufofs = 0;
    }
    أرجع ok;
}

فراغ FileStorage::Impl::puts(ثابت حرف *str) {
    CV_Assert(write_ډېرپېښ);
    إذا (mem_ډېرپېښ)
        std::copy(str, str + مزي_اوږدوالی(str), std::back_inserter(outbuf));
    وإلا إذا (file)
        fputs(str, file);
#إذا USE_ZLIB
    وإلا إذا (gzfile)
        gzputs(gzfile, str);
#د_که_پای
    وإلا
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

حرف *FileStorage::Impl::getsFromFile(حرف *buf, صحيح شمېر_2) {
    إذا (file)
        أرجع fgets(buf, شمېر_2, file);
#إذا USE_ZLIB
    إذا (gzfile)
        أرجع gzgets(gzfile, buf, شمېر_2);
#د_که_پای
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

حرف *FileStorage::Impl::gets(size_t max_شمېر) {
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
        size_t شمېر_2 = i - strbufpos;
        إذا (max_شمېر == 0 || max_شمېر > شمېر_2)
            max_شمېر = شمېر_2;
        بفر.resize(std::max(بفر.size(), max_شمېر + 8));
        حافظه_نقل(&بفر[0], instr + strbufpos, max_شمېر);
        بفر[max_شمېر] = '\0';
        strbufpos = i;
        أرجع max_شمېر > 0 ? &بفر[0] : 0;
    }

    ثابت size_t MAX_بلاک_اندازه = لوی_عدد / 2; // hopefully, that will be enough
    إذا (max_شمېر == 0)
        max_شمېر = MAX_بلاک_اندازه;
    وإلا
        CV_Assert(max_شمېر < MAX_بلاک_اندازه);
    size_t ofs = 0;

    لكل (;;) {
        صحيح شمېر_2 = (صحيح) std::min(بفر.size() - ofs - 16, max_شمېر);
        حرف *ptr = getsFromFile(&بفر[ofs], شمېر_2 + 1);
        إذا (!ptr)
            اكسر;
        صحيح delta = (صحيح) مزي_اوږدوالی(ptr);
        ofs += delta;
        max_شمېر -= delta;
        إذا (delta == 0 || ptr[delta - 1] == '\n' || max_شمېر == 0)
            اكسر;
        إذا (delta == شمېر_2)
            بفر.resize((size_t) (بفر.size() * 1.5));
    }
    أرجع ofs > 0 ? &بفر[0] : 0;
}

حرف *FileStorage::Impl::gets() {
    حرف *ptr = هذا->gets(0);
    إذا (!ptr) {
        ptr = بفر_پیل();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        سېټ_Eof();
        أرجع 0;
    } وإلا {
        size_t l = مزي_اوږدوالی(ptr);
        إذا (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    أرجع ptr;
}

منطقي FileStorage::Impl::eof() {
    إذا (dummy_eof)
        أرجع ریښتیا;
    إذا (strbuf)
        أرجع strbufpos >= strbufsize;
    إذا (file)
        أرجع feof(file) != 0;
#إذا USE_ZLIB
    إذا (gzfile)
        أرجع gzeof(gzfile) != 0;
#د_که_پای
    أرجع خطأ;
}

فراغ FileStorage::Impl::سېټ_Eof() {
    dummy_eof = ریښتیا;
}

فراغ FileStorage::Impl::closeFile() {
    إذا (file)
        fclose(file);
#إذا USE_ZLIB
    وإلا إذا (gzfile)
        gzclose(gzfile);
#د_که_پای
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
#د_که_پای
    strbufpos = 0;
}

حرف *FileStorage::Impl::resize_Write_بفر(حرف *ptr, صحيح len) {
    ثابت حرف *بفر_پای_ټکی = &بفر[0] + بفر.size();
    إذا (ptr + len < بفر_پای_ټکی)
        أرجع ptr;

    ثابت حرف *بفر_پیل_2 = &بفر[0];
    صحيح written_len = (صحيح) (ptr - بفر_پیل_2);

    CV_Assert(written_len <= (صحيح) بفر.size());
    صحيح new_اندازه_2 = (صحيح) ((بفر_پای_ټکی - بفر_پیل_2) * 3 / 2);
    new_اندازه_2 = MAX(written_len + len, new_اندازه_2);
    بفر.reserve(new_اندازه_2 + 256);
    بفر.resize(new_اندازه_2);
    bufofs = written_len;
    أرجع &بفر[0] + bufofs;
}

حرف *FileStorage::Impl::flush() {
    حرف *بفر_پیل_2 = &بفر[0];
    حرف *ptr = بفر_پیل_2 + bufofs;

    إذا (ptr > بفر_پیل_2 + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(بفر_پیل_2);
        bufofs = 0;
    }

    صحيح indent = write_ډېری.back().indent;

    إذا (space != indent) {
        memset(بفر_پیل_2, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = بفر_پیل_2 + bufofs;

    أرجع ptr;
}

فراغ FileStorage::Impl::پای_ټکی_Write_Struct() {
    CV_Assert(write_ډېرپېښ);

    check_if_write_struct_is_delayed(خطأ);
    إذا (state_د_writing_base64 != FileStorage_API::Uncertain)
        switch_منزل_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_ډېری.empty());

    FStruct_معلومات &current_struct = write_ډېری.back();
    إذا (fmt == FileStorage::FORMAT_JSON && !File_غوټه::isFlow(current_struct.flags) && write_ډېری.size() > 1)
        current_struct.indent = write_ډېری[write_ډېری.size() - 2].indent;

    getEmitter().پای_ټکی_Write_Struct(current_struct);

    write_ډېری.pop_back();
    إذا (!write_ډېری.empty())
        write_ډېری.back().flags &= ~File_غوټه::تشه_2;
}

فراغ FileStorage::Impl::پیل_Write_Struct_helper(ثابت حرف *کلی, صحيح struct_flags,
                                                ثابت حرف *type_name) {
    CV_Assert(write_ډېرپېښ);

    struct_flags = (struct_flags & (File_غوټه::TYPE_ماسک | File_غوټه::FLOW)) | File_غوټه::تشه_2;
    إذا (!File_غوټه::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    إذا (type_name && type_name[0] == '\0')
        type_name = 0;

    FStruct_معلومات s = getEmitter().پیل_Write_Struct(write_ډېری.back(), کلی, struct_flags, type_name);

    write_ډېری.push_back(s);
    size_t write_ډېری_اندازه = write_ډېری.size();
    إذا (write_ډېری_اندازه > 1)
        write_ډېری[write_ډېری_اندازه - 2].flags &= ~File_غوټه::تشه_2;

    إذا (fmt != FileStorage::FORMAT_JSON && !File_غوټه::isFlow(s.flags))
        flush();

    إذا (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_غوټه::isMap(struct_flags)) {
        getEmitter().اكتب("type_id", type_name, خطأ);
    }
}

فراغ FileStorage::Impl::پیل_Write_Struct(ثابت حرف *کلی, صحيح struct_flags,
                                         ثابت حرف *type_name) {
    check_if_write_struct_is_delayed(خطأ);
    إذا (state_د_writing_base64 == FileStorage_API::NotUse)
        switch_منزل_Base64_state(FileStorage_API::Uncertain);

    إذا (state_د_writing_base64 == FileStorage_API::Uncertain && File_غوټه::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(کلی, struct_flags, type_name);
    } وإلا إذا (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        إذا ((File_غوټه::TYPE_ماسک & struct_flags) != File_غوټه::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        وإلا إذا (state_د_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        پیل_Write_Struct_helper(کلی, struct_flags, "binary");

        إذا (state_د_writing_base64 != FileStorage_API::Uncertain)
            switch_منزل_Base64_state(FileStorage_API::Uncertain);
        switch_منزل_Base64_state(FileStorage_API::دننه_Use);
    } وإلا {
        /* Won't output Base64 data */
        إذا (state_د_writing_base64 == FileStorage_API::دننه_Use)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        پیل_Write_Struct_helper(کلی, struct_flags, type_name);

        إذا (state_د_writing_base64 != FileStorage_API::Uncertain)
            switch_منزل_Base64_state(FileStorage_API::Uncertain);
        switch_منزل_Base64_state(FileStorage_API::NotUse);
    }
}

فراغ FileStorage::Impl::writeComment(ثابت حرف *comment, منطقي eol_comment) {
    CV_Assert(write_ډېرپېښ);
    getEmitter().writeComment(comment, eol_comment);
}

فراغ FileStorage::Impl::پیل_بل_بهیر() {
    CV_Assert(write_ډېرپېښ);
    إذا (!تشه_بهیر) {
        طالما (!write_ډېری.empty())
            پای_ټکی_Write_Struct();
        flush();
        getEmitter().پیل_بل_بهیر();
        تشه_بهیر = ریښتیا;
        write_ډېری.push_back(FStruct_معلومات("", File_غوټه::تشه_2, 0));
        bufofs = 0;
    }
}

فراغ FileStorage::Impl::اكتب(ثابت String &کلی, صحيح value) {
    CV_Assert(write_ډېرپېښ);
    getEmitter().اكتب(کلی.c_str(), value);
}

فراغ FileStorage::Impl::اكتب(ثابت String &کلی, int64_t value) {
    CV_Assert(write_ډېرپېښ);
    getEmitter().اكتب(کلی.c_str(), value);
}

فراغ FileStorage::Impl::اكتب(ثابت String &کلی, مزدوج value) {
    CV_Assert(write_ډېرپېښ);
    getEmitter().اكتب(کلی.c_str(), value);
}

فراغ FileStorage::Impl::اكتب(ثابت String &کلی, ثابت String &value) {
    CV_Assert(write_ډېرپېښ);
    getEmitter().اكتب(کلی.c_str(), value.c_str(), خطأ);
}

فراغ FileStorage::Impl::write_Raw_معلومات(ثابت std::string &dt, ثابت فراغ *معلومات_3, size_t len) {
    CV_Assert(write_ډېرپېښ);

    إذا (is_using_base64 || state_د_writing_base64 == FileStorage_API::Base64State::دننه_Use) {
        write_Raw_معلومات_Base64(معلومات_3, len, dt.c_str());
        أرجع;
    } وإلا إذا (state_د_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_منزل_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elem_اندازه = fs::calc_Struct_اندازه(dt.c_str(), 0);
    CV_Assert(elem_اندازه);
    CV_Assert(len % elem_اندازه == 0);
    len /= elem_اندازه;

    منطقي explicitZero = fmt == FileStorage::FORMAT_JSON;
    ثابت uchar *data0 = (ثابت uchar *) معلومات_3;
    صحيح fmt_جوړې[CV_FS_MAX_FMT_جوړې * 2], k, fmt_pair_شمېر;
    حرف buf[256] = "";

    fmt_pair_شمېر = fs::decodeFormat(dt.c_str(), fmt_جوړې, CV_FS_MAX_FMT_جوړې);

    إذا (!len)
        أرجع;

    إذا (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    إذا (fmt_pair_شمېر == 1) {
        fmt_جوړې[0] *= (صحيح) len;
        len = 1;
    }

    لكل (; len--; data0 += elem_اندازه) {
        صحيح offset = 0;
        لكل (k = 0; k < fmt_pair_شمېر; k++) {
            صحيح i, شمېر_2 = fmt_جوړې[k * 2];
            صحيح elem_type = fmt_جوړې[k * 2 + 1];
            صحيح elem_اندازه_2 = CV_ELEM_اندازه(elem_type);
            ثابت حرف *ptr;

            offset = cvAlign(offset, elem_اندازه_2);
            ثابت uchar *معلومات = data0 + offset;

            لكل (i = 0; i < شمېر_2; i++) {
                بدل (elem_type) {
                    حالة CV_8U:
                        ptr = fs::itoa(*(uchar *) معلومات, buf, 10);
                        معلومات++;
                        اكسر;
                    حالة CV_Bool:
                        ptr = fs::itoa(*(uchar *) معلومات != 0, buf, 10);
                        معلومات++;
                        اكسر;
                    حالة CV_8S:
                        ptr = fs::itoa(*(حرف *) معلومات, buf, 10);
                        معلومات++;
                        اكسر;
                    حالة CV_16U:
                        ptr = fs::itoa(*(ushort *) معلومات, buf, 10);
                        معلومات += حجم(ushort);
                        اكسر;
                    حالة CV_16S:
                        ptr = fs::itoa(*(لنډ *) معلومات, buf, 10);
                        معلومات += حجم(لنډ);
                        اكسر;
                    حالة CV_32U:
                        ptr = fs::itoa((int64_t)*(بینښه*) معلومات, buf, 10, خطأ);
                        معلومات += حجم(بینښه);
                        اكسر;
                    حالة CV_32S:
                        ptr = fs::itoa(*(صحيح *) معلومات, buf, 10);
                        معلومات += حجم(صحيح);
                        اكسر;
                    حالة CV_64U:
                        ptr = fs::itoa(*(uint64_t*) معلومات, buf, 10, خطأ);
                        معلومات += حجم(uint64_t);
                        اكسر;
                    حالة CV_64S:
                        ptr = fs::itoa(*(int64_t*) معلومات, buf, 10, ریښتیا);
                        معلومات += حجم(int64_t);
                        اكسر;
                    حالة CV_32F:
                        ptr = fs::float_منزل_String(buf, حجم(buf), *(عائم *) معلومات, خطأ, explicitZero);
                        معلومات += حجم(عائم);
                        اكسر;
                    حالة CV_64F:
                        ptr = fs::double_منزل_String(buf, حجم(buf), *(مزدوج *) معلومات, explicitZero);
                        معلومات += حجم(مزدوج);
                        اكسر;
                    حالة CV_16F:
                        ptr = fs::float_منزل_String(buf, حجم(buf), (عائم) *(hfloat *) معلومات, ریښتیا, explicitZero);
                        معلومات += حجم(hfloat);
                        اكسر;
                    حالة CV_16BF:
                        ptr = fs::float_منزل_String(buf, حجم(buf), (عائم) *(bfloat *) معلومات, ریښتیا, explicitZero);
                        معلومات += حجم(bfloat);
                        اكسر;
                    حالة CV_8F_E4M3FN:
                        ptr = fs::float_منزل_String(buf, حجم(buf), (عائم) *(fp8_t *) معلومات, ریښتیا, explicitZero);
                        معلومات += حجم(fp8_t);
                        اكسر;
                    حالة CV_8F_E4M3FNUZ:
                        ptr = fs::float_منزل_String(buf, حجم(buf), (عائم) *(fp8a_t *) معلومات, ریښتیا, explicitZero);
                        معلومات += حجم(fp8a_t);
                        اكسر;
                    افتراضي:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        أرجع;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (صحيح) (معلومات - data0);
        }
    }
}

فراغ FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(خطأ);

    إذا (state_د_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_منزل_Base64_state(FileStorage_API::Base64State::Uncertain);
}

فراغ FileStorage::Impl::switch_منزل_Base64_state(FileStorage_API::Base64State new_state) {
    ثابت حرف *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    ثابت حرف *err_unable_منزل_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    بدل (state_د_writing_base64) {
        حالة FileStorage_API::Base64State::Uncertain:
            بدل (new_state) {
                حالة FileStorage_API::Base64State::دننه_Use:
                {
                    CV_DbgAssert(base64_writer == 0);
                    منطقي can_indent = (fmt != cv::FileStorage::ډېرپېښ_2::FORMAT_JSON);
                    base64_writer = جديد base64::Base64Writer(*هذا, can_indent);
                    إذا (!can_indent) {
                        حرف *ptr = بفر_Ptr();
                        *ptr++ = '\0';
                        puts(بفر_پیل());
                        سېټ_بفر_Ptr(بفر_پیل());
                        memset(بفر_پیل(), 0, static_cast<صحيح>(space));
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
        حالة FileStorage_API::Base64State::دننه_Use:
            بدل (new_state) {
                حالة FileStorage_API::Base64State::دننه_Use:
                حالة FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_منزل_switch);
                    اكسر;
                حالة FileStorage_API::Base64State::Uncertain:
                    ړنګول base64_writer;
                    base64_writer = 0;
                    إذا ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        سېټ_بفر_Ptr(بفر_پیل());
                        flush();
                        memset(بفر_پیل(), 0, static_cast<صحيح>(space) );
                        سېټ_بفر_Ptr(بفر_پیل());
                    }
                    اكسر;
                افتراضي:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    اكسر;
            }
            اكسر;
        حالة FileStorage_API::Base64State::NotUse:
            بدل (new_state) {
                حالة FileStorage_API::Base64State::دننه_Use:
                حالة FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_منزل_switch);
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

    state_د_writing_base64 = new_state;
}

فراغ FileStorage::Impl::make_write_struct_delayed(ثابت حرف *کلی, صحيح struct_flags, ثابت حرف *type_name) {
    CV_Assert(is_write_struct_delayed == خطأ);
    CV_DbgAssert(delayed_struct_کلی == تش_اشاره);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == تش_اشاره);

    delayed_struct_flags = struct_flags;

    إذا (کلی != تش_اشاره) {
        delayed_struct_کلی = جديد حرف[مزي_اوږدوالی(کلی) + 1U];
        strcpy(delayed_struct_کلی, کلی);
    }

    إذا (type_name != تش_اشاره) {
        delayed_type_name = جديد حرف[مزي_اوږدوالی(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = ریښتیا;
}

فراغ FileStorage::Impl::check_if_write_struct_is_delayed(منطقي change_type_منزل_base64) {
    إذا (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_کلی;
        std::string type_name;
        صحيح struct_flags = delayed_struct_flags;

        إذا (delayed_struct_کلی != تش_اشاره && *delayed_struct_کلی != '\0') {
            struct_کلی.assign(delayed_struct_کلی);
        }
        إذا (delayed_type_name != تش_اشاره && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        ړنګول[] delayed_struct_کلی;
        ړنګول[] delayed_type_name;
        delayed_struct_کلی = تش_اشاره;
        delayed_struct_flags = 0;
        delayed_type_name = تش_اشاره;

        is_write_struct_delayed = خطأ;

        /* call */
        إذا (change_type_منزل_base64) {
            پیل_Write_Struct_helper(struct_کلی.c_str(), struct_flags, "binary");
            إذا (state_د_writing_base64 != FileStorage_API::Uncertain)
                switch_منزل_Base64_state(FileStorage_API::Uncertain);
            switch_منزل_Base64_state(FileStorage_API::دننه_Use);
        } وإلا {
            پیل_Write_Struct_helper(struct_کلی.c_str(), struct_flags, type_name.c_str());
            إذا (state_د_writing_base64 != FileStorage_API::Uncertain)
                switch_منزل_Base64_state(FileStorage_API::Uncertain);
            switch_منزل_Base64_state(FileStorage_API::NotUse);
        }
    }
}

فراغ FileStorage::Impl::write_Raw_معلومات_Base64(ثابت فراغ *معلومات_3, size_t len, ثابت حرف *dt) {
    CV_Assert(write_ډېرپېښ);

    check_if_write_struct_is_delayed(ریښتیا);

    إذا (state_د_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_منزل_Base64_state(FileStorage_API::Base64State::دننه_Use);
    } وإلا إذا (state_د_writing_base64 != FileStorage_API::Base64State::دننه_Use) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->اكتب(معلومات_3, len, dt);
}

File_غوټه FileStorage::Impl::get_لومړی_سر_Level_غوټه() ثابت {
    أرجع roots.empty() ? File_غوټه() : roots[0];
}

File_غوټه FileStorage::Impl::ریښه(صحيح بهیر_Idx) ثابت {
    أرجع بهیر_Idx >= 0 && بهیر_Idx < (صحيح) roots.size() ? roots[بهیر_Idx] : File_غوټه();
}

File_غوټه FileStorage::Impl::چلوونکی[](ثابت String &nodename) ثابت {
    أرجع هذا->چلوونکی[](nodename.c_str());
}

File_غوټه FileStorage::Impl::چلوونکی[](ثابت حرف * /*nodename*/) ثابت {
    أرجع File_غوټه();
}

صحيح FileStorage::Impl::getFormat() ثابت { أرجع fmt; }

حرف *FileStorage::Impl::بفر_Ptr() ثابت { أرجع (حرف *) (&بفر[0] + bufofs); }

حرف *FileStorage::Impl::بفر_پیل() ثابت { أرجع (حرف *) &بفر[0]; }

حرف *FileStorage::Impl::بفر_پای_ټکی_2() ثابت { أرجع (حرف *) (&بفر[0] + بفر.size()); }

فراغ FileStorage::Impl::سېټ_بفر_Ptr(حرف *ptr) {
    حرف *bufferstart = بفر_پیل();
    CV_Assert(ptr >= bufferstart && ptr <= بفر_پای_ټکی_2());
    bufofs = ptr - bufferstart;
}

صحيح FileStorage::Impl::wrapMargin() ثابت { أرجع wrap_margin; }

FStruct_معلومات &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_ډېری.empty());
    أرجع write_ډېری.back();
}

فراغ FileStorage::Impl::سېټ_Non_تشه() {
    تشه_بهیر = خطأ;
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
    إذا (لوی_توری(buf[1]) == 'I' && لوی_توری(buf[2]) == 'N' && لوی_توری(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    وإلا إذا (لوی_توری(buf[1]) == 'N' && لوی_توری(buf[2]) == 'A' && لوی_توری(buf[3]) == 'N')
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

    أرجع fval;
}

فراغ FileStorage::Impl::convert_منزل_Collection(صحيح type, File_غوټه &غوټه) {
    CV_Assert(type == File_غوټه::SEQ || type == File_غوټه::MAP);

    صحيح غوټه_type = غوټه.type();
    إذا (غوټه_type == type)
        أرجع;

    منطقي named = غوټه.isNamed();
    uchar *ptr = غوټه.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    مزدوج fval = 0;
    std::string sval;
    منطقي جمع_کړه_لومړی_scalar = خطأ;

    إذا (غوټه_type != File_غوټه::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_غوټه::SEQ);
        إذا (غوټه_type == File_غوټه::INT) {
            ival = readLong(ptr);
            جمع_کړه_لومړی_scalar = ریښتیا;
        } وإلا إذا (غوټه_type == File_غوټه::REAL) {
            fval = readReal(ptr);
            جمع_کړه_لومړی_scalar = ریښتیا;
        } وإلا إذا (غوټه_type == File_غوټه::STRING) {
            sval = std::string(غوټه);
            جمع_کړه_لومړی_scalar = ریښتیا;
        } وإلا
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", غوټه_type));
    }

    ptr = reserve_غوټه_Space(غوټه, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_غوټه::NAMED : 0));
    // name has been copied automatically
    إذا (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    إذا (جمع_کړه_لومړی_scalar)
        جمع_کړه_غوټه(غوټه, std::string(), غوټه_type,
                غوټه_type == File_غوټه::INT ? (ثابت فراغ *) &ival :
                غوټه_type == File_غوټه::REAL ? (ثابت فراغ *) &fval :
                غوټه_type == File_غوټه::STRING ? (ثابت فراغ *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_غوټه_Space(File_غوټه &غوټه, size_t sz) {
    منطقي shrink_بلاک = خطأ;
    size_t shrink_بلاک_Idx = 0, shrink_اندازه = 0;

    uchar *ptr = 0, *بلاک_پای_ټکی = 0;

    إذا (!fs_معلومات_ptrs.empty()) {
        size_t بلاک_Idx = غوټه.بلاک_Idx;
        size_t ofs = غوټه.ofs;
        CV_Assert(بلاک_Idx == fs_معلومات_ptrs.size() - 1);
        CV_Assert(ofs <= fs_معلومات_blksz[بلاک_Idx]);
        CV_Assert(freeSpaceOfs <= fs_معلومات_blksz[بلاک_Idx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_معلومات_ptrs[بلاک_Idx] + ofs;
        بلاک_پای_ټکی = fs_معلومات_ptrs[بلاک_Idx] + fs_معلومات_blksz[بلاک_Idx];

        CV_Assert(ptr >= fs_معلومات_ptrs[بلاک_Idx] && ptr <= بلاک_پای_ټکی);
        إذا (ptr + sz <= بلاک_پای_ټکی) {
            freeSpaceOfs = ofs + sz;
            أرجع ptr;
        }

        إذا (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_معلومات[بلاک_Idx]->resize(sz);
            ptr = &fs_معلومات[بلاک_Idx]->at(0);
            fs_معلومات_ptrs[بلاک_Idx] = ptr;
            fs_معلومات_blksz[بلاک_Idx] = sz;
            freeSpaceOfs = sz;
            أرجع ptr;
        }

        shrink_بلاک = ریښتیا;
        shrink_بلاک_Idx = بلاک_Idx;
        shrink_اندازه = ofs;
    }

    size_t بلاک_اندازه_2 = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(بلاک_اندازه_2);
    fs_معلومات.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_معلومات_ptrs.push_back(new_ptr);
    fs_معلومات_blksz.push_back(بلاک_اندازه_2);
    غوټه.بلاک_Idx = fs_معلومات_ptrs.size() - 1;
    غوټه.ofs = 0;
    freeSpaceOfs = sz;

    إذا (ptr && ptr + 5 <= بلاک_پای_ټکی) {
        new_ptr[0] = ptr[0];
        إذا (ptr[0] & File_غوټه::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    إذا (shrink_بلاک) {
        fs_معلومات[shrink_بلاک_Idx]->resize(shrink_اندازه);
        fs_معلومات_blksz[shrink_بلاک_Idx] = shrink_اندازه;
    }

    أرجع new_ptr;
}

بینښه FileStorage::Impl::getStringOfs(ثابت std::string &کلی) ثابت {
    str_hash_t::const_iterator it = str_hash.find(کلی);
    أرجع it != str_hash.end() ? it->second : 0;
}

File_غوټه FileStorage::Impl::جمع_کړه_غوټه(File_غوټه &collection, ثابت std::string &کلی,
                                    صحيح elem_type, ثابت فراغ *value, صحيح len) {
    FileStorage_API *fs = هذا;
    منطقي noname = کلی.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(کلی.c_str(), "_") == 0);
    convert_منزل_Collection(noname ? File_غوټه::SEQ : File_غوټه::MAP, collection);

    منطقي isseq = collection.empty() ? خطأ : collection.isSeq();
    إذا (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    بینښه strofs = 0;
    إذا (!noname) {
        strofs = getStringOfs(کلی);
        إذا (!strofs) {
            strofs = (بینښه) str_hash_معلومات.size();
            size_t keysize = کلی.size() + 1;
            str_hash_معلومات.resize(strofs + keysize);
            حافظه_نقل(&str_hash_معلومات[0] + strofs, &کلی[0], keysize);
            str_hash.insert(std::make_pair(کلی, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t بلاک_Idx = fs_معلومات_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_غوټه غوټه(fs_ext, بلاک_Idx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_غوټه_Space(غوټه, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_غوټه::NAMED));
    إذا (elem_type == File_غوټه::NONE)
        freeSpaceOfs -= 8;

    إذا (!noname) {
        writeInt(ptr, (صحيح) strofs);
        ptr += 4;
    }

    إذا (elem_type == File_غوټه::SEQ || elem_type == File_غوټه::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    إذا (value)
        غوټه.سېټ_ارزښت(elem_type, value, len);

    إذا (collection.isNamed())
        cp += 4;
    صحيح nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    أرجع غوټه;
}

فراغ FileStorage::Impl::finalizeCollection(File_غوټه &collection) {
    إذا (!collection.isSeq() && !collection.isMap())
        أرجع;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    إذا (*ptr0 & File_غوټه::NAMED)
        ptr += 4;
    size_t بلاک_Idx = collection.بلاک_Idx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t raw_اندازه = 4;
    بینښه sz = (بینښه) readInt(ptr + 4);
    إذا (sz > 0) {
        size_t وروستی_بلاک_Idx = fs_معلومات_ptrs.size() - 1;

        لكل (; بلاک_Idx < وروستی_بلاک_Idx; بلاک_Idx++) {
            raw_اندازه += fs_معلومات_blksz[بلاک_Idx] - ofs;
            ofs = 0;
        }
    }
    raw_اندازه += freeSpaceOfs - ofs;
    writeInt(ptr, (صحيح) raw_اندازه);
}

فراغ FileStorage::Impl::normalize_غوټه_Ofs(size_t &بلاک_Idx, size_t &ofs) ثابت {
    طالما (ofs >= fs_معلومات_blksz[بلاک_Idx]) {
        إذا (بلاک_Idx == fs_معلومات_blksz.size() - 1) {
            CV_Assert(ofs == fs_معلومات_blksz[بلاک_Idx]);
            اكسر;
        }
        ofs -= fs_معلومات_blksz[بلاک_Idx];
        بلاک_Idx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_د_writing_base64() {
    أرجع state_د_writing_base64;
}

صحيح FileStorage::Impl::get_space() {
    أرجع space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = ریښتیا;
}

فراغ FileStorage::Impl::Base64Decoder::init(ثابت Ptr<FileStorageParser> &_parser, حرف *_ptr, صحيح _indent) {
    parser_do_not_use_مستقیم_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = خطأ;
}

منطقي FileStorage::Impl::Base64Decoder::read_نور(صحيح needed) {
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
    منطقي ok = getParser().get_Base64_لیکه(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    إذا (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = ریښتیا;
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

    أرجع (صحيح) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    إذا (ofs >= sz && !read_نور(1))
        أرجع (uchar) 0;
    أرجع decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    إذا (ofs + 2 > sz && !read_نور(2))
        أرجع (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    أرجع val;
}

صحيح FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    إذا (ofs + 4 > sz && !read_نور(4))
        أرجع 0;
    صحيح ival = readInt(&decoded[ofs]);
    ofs += 4;
    أرجع ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    إذا (ofs + 8 > sz && !read_نور(8))
        أرجع 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    أرجع ival;
}

مزدوج FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    إذا (ofs + 8 > sz && !read_نور(8))
        أرجع 0;
    مزدوج fval = readReal(&decoded[ofs]);
    ofs += 8;
    أرجع fval;
}

منطقي FileStorage::Impl::Base64Decoder::پای_ټکی_د_بهیر() ثابت { أرجع eos; }

حرف *FileStorage::Impl::Base64Decoder::getPtr() ثابت { أرجع ptr; }


حرف *FileStorage::Impl::parseBase64(حرف *ptr, صحيح indent, File_غوټه &collection) {
    ثابت صحيح BASE64_HDR_اندازه = 24;
    حرف dt[BASE64_HDR_اندازه + 1] = {0};
    base64decoder.init(parser_do_not_use_مستقیم_dereference, ptr, indent);

    صحيح i, k;

    لكل (i = 0; i < BASE64_HDR_اندازه; i++)
        dt[i] = (حرف) base64decoder.getUInt8();
    لكل (i = 0; i < BASE64_HDR_اندازه; i++)
        إذا (که_تشه(dt[i]))
            اكسر;
    dt[i] = '\0';

    CV_Assert(!base64decoder.پای_ټکی_د_بهیر());

    صحيح fmt_جوړې[CV_FS_MAX_FMT_جوړې * 2];
    صحيح fmt_pair_شمېر = fs::decodeFormat(dt, fmt_جوړې, CV_FS_MAX_FMT_جوړې);
    int64_t ival = 0;
    مزدوج fval = 0;

    لكل (;;) {
        لكل (k = 0; k < fmt_pair_شمېر; k++) {
            صحيح elem_type = fmt_جوړې[k * 2 + 1];
            صحيح شمېر_2 = fmt_جوړې[k * 2];

            لكل (i = 0; i < شمېر_2; i++) {
                صحيح غوټه_type = File_غوټه::INT;
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
                        ival = (لنډ) base64decoder.getUInt16();
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
                        غوټه_type = File_غوټه::REAL;
                    }
                        اكسر;
                    حالة CV_64F:
                        fval = base64decoder.getFloat64();
                        غوټه_type = File_غوټه::REAL;
                        اكسر;
                    حالة CV_16F:
                        fval = عائم(hfloat_From_بټونه(base64decoder.getUInt16()));
                        غوټه_type = File_غوټه::REAL;
                        اكسر;
                    حالة CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        غوټه_type = File_غوټه::REAL;
                        اكسر;
                    حالة CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        غوټه_type = File_غوټه::REAL;
                        اكسر;
                    افتراضي:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                إذا (base64decoder.پای_ټکی_د_بهیر())
                    اكسر;
                جمع_کړه_غوټه(collection, std::string(), غوټه_type,
                        غوټه_type == File_غوټه::INT ? (فراغ *) &ival : (فراغ *) &fval, -1);
            }
        }
        إذا (base64decoder.پای_ټکی_د_بهیر())
            اكسر;
    }

    finalizeCollection(collection);
    أرجع base64decoder.getPtr();
}

فراغ FileStorage::Impl::parseError(ثابت حرف *func_name, ثابت std::string &err_پیغام, ثابت حرف *سرچینه_file,
                                   صحيح سرچینه_کرښه) {
    std::string پیغام = format("%s(%d): %s", filename.c_str(), lineno, err_پیغام.c_str());
    error(Error::StsParseError, func_name, پیغام.c_str(), سرچینه_file, سرچینه_کرښه);
}

ثابت uchar *FileStorage::Impl::get_غوټه_Ptr(size_t بلاک_Idx, size_t ofs) ثابت {
    CV_Assert(بلاک_Idx < fs_معلومات_ptrs.size());
    CV_Assert(ofs < fs_معلومات_blksz[بلاک_Idx]);

    أرجع fs_معلومات_ptrs[بلاک_Idx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) ثابت {
    CV_Assert(nameofs < str_hash_معلومات.size());
    أرجع std::string(&str_hash_معلومات[nameofs]);
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
    إذا(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

فراغ FileStorage::پیل_Write_Struct(ثابت String& name, صحيح struct_flags, ثابت String& typeName)
{
    p->پیل_Write_Struct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    إذا ((struct_flags & File_غوټه::TYPE_ماسک) == File_غوټه::SEQ)
        state = FileStorage::ارزښت_EXPECTED;
    وإلا
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

فراغ FileStorage::پای_ټکی_Write_Struct()
{
    p->پای_ټکی_Write_Struct();
    state = p->write_ډېری.empty() || File_غوټه::isMap(p->write_ډېری.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::ارزښت_EXPECTED;
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

File_غوټه FileStorage::ریښه(صحيح i) ثابت
{
    إذا( p.empty() || p->roots.empty() || i < 0 || i >= (صحيح)p->roots.size() )
        أرجع File_غوټه();

    أرجع p->roots[i];
}

File_غوټه FileStorage::get_لومړی_سر_Level_غوټه() ثابت
{
    File_غوټه r = ریښه();
    File_غوټه_Iterator it = r.begin();
    أرجع it != r.end() ? *it : File_غوټه();
}

std::string FileStorage::getDefaultObjectName(ثابت std::string& _filename)
{
    ساكن ثابت حرف* stubname = "unnamed";
    ثابت حرف* filename = _filename.c_str();
    ثابت حرف* ptr2 = filename + _filename.size();
    ثابت حرف* ptr = ptr2 - 1;
    cv::Auto_بفر<حرف> name_buf(_filename.size()+1);

    طالما( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        إذا( *ptr == '.' && (!*ptr2 || مزي_پرتله(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    إذا( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    حرف* name = name_buf.معلومات();

    // name must start with letter or '_'
    إذا( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    طالما( ptr < ptr2 )
    {
        حرف c = *ptr++;
        إذا( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.معلومات();
    إذا( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    أرجع name;
}


صحيح FileStorage::getFormat() ثابت
{
    أرجع p->fmt;
}

File_غوټه FileStorage::چلوونکی [](ثابت حرف* کلی) ثابت
{
    أرجع هذا->چلوونکی[](std::string(کلی));
}

File_غوټه FileStorage::چلوونکی [](ثابت std::string& کلی) ثابت
{
    File_غوټه res;
    لكل (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][کلی];
        إذا (!res.empty())
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
    p->write_Raw_معلومات(fmt, (ثابت uchar*)vec, len);
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

FileStorage& چلوونکی << (FileStorage& fs, ثابت String& str)
{
    تعداد { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        ارزښت_EXPECTED = FileStorage::ارزښت_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    ثابت حرف* _str = str.c_str();
    إذا( !fs.isOpened() || !_str )
        أرجع fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    حرف c = *_str;

    إذا( c == '}' || c == ']' )
    {
        إذا( fs_impl->write_ډېری.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        صحيح struct_flags = fs_impl->write_ډېری.back().flags;
        حرف expected_bracket = File_غوټه::isMap(struct_flags) ? '}' : ']';
        إذا( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->پای_ټکی_Write_Struct();
        CV_Assert(!fs_impl->write_ډېری.empty());
        struct_flags = fs_impl->write_ډېری.back().flags;
        fs.state = File_غوټه::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : ارزښت_EXPECTED;
        fs.elname = String();
    }
    وإلا إذا( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        إذا (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = ارزښت_EXPECTED + INSIDE_MAP;
    }
    وإلا إذا( (fs.state & 3) == ارزښت_EXPECTED )
    {
        إذا( c == '{' || c == '[' )
        {
            صحيح struct_flags = c == '{' ? File_غوټه::MAP : File_غوټه::SEQ;
            fs.state = struct_flags == File_غوټه::MAP ? INSIDE_MAP + NAME_EXPECTED : ارزښت_EXPECTED;
            _str++;
            إذا( *_str == ':' )
            {
                _str++;
                إذا( !*_str )
                    struct_flags |= File_غوټه::FLOW;
            }
            fs_impl->پیل_Write_Struct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        وإلا
        {
            اكتب( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            إذا( fs.state == INSIDE_MAP + ارزښت_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    وإلا
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    أرجع fs;
}


File_غوټه::File_غوټه()
    : fs(عدم)
{
    بلاک_Idx = ofs = 0;
}

File_غوټه::File_غوټه(FileStorage::Impl* _fs, size_t بلاک_Idx_2, size_t _ofs)
    : fs(_fs)
{
    بلاک_Idx = بلاک_Idx_2;
    ofs = _ofs;
}

File_غوټه::File_غوټه(ثابت FileStorage* _fs, size_t بلاک_Idx_2, size_t _ofs)
    : File_غوټه(_fs->p.get(), بلاک_Idx_2, _ofs)
{
    // nothing
}

File_غوټه::File_غوټه(ثابت File_غوټه& غوټه)
{
    fs = غوټه.fs;
    بلاک_Idx = غوټه.بلاک_Idx;
    ofs = غوټه.ofs;
}

File_غوټه& File_غوټه::چلوونکی=(ثابت File_غوټه& غوټه)
{
    fs = غوټه.fs;
    بلاک_Idx = غوټه.بلاک_Idx;
    ofs = غوټه.ofs;
    أرجع *هذا;
}

File_غوټه File_غوټه::چلوونکی[](ثابت std::string& nodename) ثابت
{
    إذا(!fs)
        أرجع File_غوټه();

    CV_Assert( isMap() );

    بینښه کلی = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_غوټه_Iterator it = begin();

    لكل( i = 0; i < sz; i++, ++it )
    {
        File_غوټه n = *it;
        ثابت uchar* p = n.ptr();
        بینښه key2 = (بینښه)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_معلومات.size() );
        إذا( کلی == key2 )
            أرجع n;
    }
    أرجع File_غوټه();
}

File_غوټه File_غوټه::چلوونکی[](ثابت حرف* nodename) ثابت
{
    أرجع هذا->چلوونکی[](std::string(nodename));
}

File_غوټه File_غوټه::چلوونکی[](صحيح i) ثابت
{
    إذا(!fs)
        أرجع File_غوټه();

    CV_Assert( isSeq() );

    صحيح sz = (صحيح)size();
    CV_Assert( 0 <= i && i < sz );

    File_غوټه_Iterator it = begin();
    it += i;

    أرجع *it;
}

std::vector<String> File_غوټه::کلیګانې() ثابت
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    لكل (File_غوټه_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    أرجع res;
}

صحيح File_غوټه::type() ثابت
{
    ثابت uchar* p = ptr();
    إذا(!p)
        أرجع NONE;
    أرجع (*p & TYPE_ماسک);
}

منطقي File_غوټه::isMap(صحيح flags) { أرجع (flags & TYPE_ماسک) == MAP; }
منطقي File_غوټه::isSeq(صحيح flags) { أرجع (flags & TYPE_ماسک) == SEQ; }
منطقي File_غوټه::isCollection(صحيح flags) { أرجع isMap(flags) || isSeq(flags); }
منطقي File_غوټه::isFlow(صحيح flags) { أرجع (flags & FLOW) != 0; }
منطقي File_غوټه::is_تشه_Collection(صحيح flags) { أرجع (flags & تشه_2) != 0; }

منطقي File_غوټه::empty() ثابت   { أرجع fs == 0; }
منطقي File_غوټه::isNone() ثابت  { أرجع type() == NONE; }
منطقي File_غوټه::isSeq() ثابت   { أرجع type() == SEQ; }
منطقي File_غوټه::isMap() ثابت   { أرجع type() == MAP; }
منطقي File_غوټه::isInt() ثابت   { أرجع type() == INT;  }
منطقي File_غوټه::isReal() ثابت  { أرجع type() == REAL; }
منطقي File_غوټه::isString() ثابت { أرجع type() == STRING;  }
منطقي File_غوټه::isNamed() ثابت
{
    ثابت uchar* p = ptr();
    إذا(!p)
        أرجع خطأ;
    أرجع (*p & NAMED) != 0;
}

std::string File_غوټه::name() ثابت
{
    ثابت uchar* p = ptr();
    إذا(!p)
        أرجع std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    أرجع fs->getName(nameofs);
}

File_غوټه::چلوونکی صحيح() ثابت
{
    ثابت uchar* p = ptr();
    إذا(!p)
        أرجع 0;
    صحيح نښان = *p;
    صحيح type = (نښان & TYPE_ماسک);
    p += (نښان & NAMED) ? 5 : 1;

    إذا( type == INT )
    {
        أرجع readInt(p);
    }
    وإلا إذا( type == REAL )
    {
        أرجع cv_پړاو(readReal(p));
    }
    وإلا
        أرجع 0x7fffffff;
}

File_غوټه::چلوونکی int64_t() ثابت
{
    ثابت uchar* p = ptr();
    إذا(!p)
        أرجع 0;
    صحيح نښان = *p;
    صحيح type = (نښان & TYPE_ماسک);
    p += (نښان & NAMED) ? 5 : 1;

    إذا( type == INT )
    {
        أرجع readLong(p);
    }
    وإلا إذا( type == REAL )
    {
        أرجع cv_پړاو(readReal(p));
    }
    وإلا
        أرجع 0x7fffffff;
}

File_غوټه::چلوونکی عائم() ثابت
{
    ثابت uchar* p = ptr();
    إذا(!p)
        أرجع 0.f;
    صحيح نښان = *p;
    صحيح type = (نښان & TYPE_ماسک);
    p += (نښان & NAMED) ? 5 : 1;

    إذا( type == INT )
    {
        أرجع (عائم)readLong(p);
    }
    وإلا إذا( type == REAL )
    {
        أرجع (عائم)readReal(p);
    }
    وإلا
        أرجع FLT_MAX;
}

File_غوټه::چلوونکی مزدوج() ثابت
{
    ثابت uchar* p = ptr();
    إذا(!p)
        أرجع 0.f;
    صحيح نښان = *p;
    صحيح type = (نښان & TYPE_ماسک);
    p += (نښان & NAMED) ? 5 : 1;

    إذا( type == INT )
    {
        أرجع (مزدوج)readLong(p);
    }
    وإلا إذا( type == REAL )
    {
        أرجع readReal(p);
    }
    وإلا
        أرجع DBL_MAX;
}

مزدوج File_غوټه::real() ثابت  { أرجع مزدوج(*هذا); }

std::string File_غوټه::string() ثابت
{
    ثابت uchar* p = ptr();
    إذا( !p || (*p & TYPE_ماسک) != STRING )
        أرجع std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(بینښه)readInt(p);
    أرجع std::string((ثابت حرف*)(p + 4), sz - 1);
}
Mat File_غوټه::mat() ثابت { Mat value; اقرأ(*هذا, value, Mat()); أرجع value; }

File_غوټه_Iterator File_غوټه::begin() ثابت { أرجع File_غوټه_Iterator(*هذا, خطأ); }
File_غوټه_Iterator File_غوټه::end() ثابت   { أرجع File_غوټه_Iterator(*هذا, ریښتیا); }

فراغ File_غوټه::readRaw( ثابت std::string& fmt, فراغ* vec, size_t len ) ثابت
{
    File_غوټه_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_غوټه::size() ثابت
{
    ثابت uchar* p = ptr();
    إذا( !p )
        أرجع 0;
    صحيح نښان = *p;
    صحيح tp = نښان & TYPE_ماسک;
    إذا( tp == MAP || tp == SEQ )
    {
        إذا( نښان & NAMED )
            p += 4;
        أرجع (size_t)(بینښه)readInt(p + 5);
    }
    أرجع tp != NONE;
}

size_t File_غوټه::raw_اندازه() ثابت
{
    ثابت uchar* p0 = ptr(), *p = p0;
    إذا( !p )
        أرجع 0;
    صحيح نښان = *p++;
    صحيح tp = نښان & TYPE_ماسک;
    إذا( نښان & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    إذا( tp == INT )
        أرجع sz0 + 8;
    إذا( tp == REAL )
        أرجع sz0 + 8;
    إذا( tp == NONE )
        أرجع sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    أرجع sz0 + 4 + readInt(p);
}

uchar* File_غوټه::ptr()
{
    أرجع !fs ? 0 : (uchar*)fs->get_غوټه_Ptr(بلاک_Idx, ofs);
}

ثابت uchar* File_غوټه::ptr() ثابت
{
    أرجع !fs ? 0 : fs->get_غوټه_Ptr(بلاک_Idx, ofs);
}

فراغ File_غوټه::سېټ_ارزښت( صحيح type, ثابت فراغ* value, صحيح len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    صحيح نښان = *p;
    صحيح current_type = نښان & TYPE_ماسک;
    CV_Assert( current_type == NONE || current_type == type );

    صحيح sz = 1;

    إذا( نښان & NAMED )
        sz += 4;

    إذا( type == INT )
        sz += 8;
    وإلا إذا( type == REAL )
        sz += 8;
    وإلا إذا( type == STRING )
    {
        إذا( len < 0 )
            len = (صحيح)مزي_اوږدوالی((ثابت حرف*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    وإلا
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_غوټه_Space(*هذا, sz);
    *p++ = (uchar)(type | (نښان & NAMED));
    إذا( نښان & NAMED )
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
        حافظه_نقل(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_غوټه_Iterator::File_غوټه_Iterator()
{
    fs = 0;
    بلاک_Idx = 0;
    ofs = 0;
    بلاک_اندازه_2 = 0;
    غوټه_NElems = 0;
    idx = 0;
}

File_غوټه_Iterator::File_غوټه_Iterator( ثابت File_غوټه& غوټه, منطقي seek_پای_ټکی )
{
    fs = غوټه.fs;
    idx = 0;
    إذا( !fs )
        بلاک_Idx = ofs = بلاک_اندازه_2 = غوټه_NElems = 0;
    وإلا
    {
        بلاک_Idx = غوټه.بلاک_Idx;
        ofs = غوټه.ofs;

        منطقي collection = غوټه.isSeq() || غوټه.isMap();
        إذا( غوټه.isNone() )
        {
            غوټه_NElems = 0;
        }
        وإلا إذا( !collection )
        {
            غوټه_NElems = 1;
            إذا( seek_پای_ټکی )
            {
                idx = 1;
                ofs += غوټه.raw_اندازه();
            }
        }
        وإلا
        {
            غوټه_NElems = غوټه.size();
            ثابت uchar* p0 = غوټه.ptr(), *p = p0 + 1;
            إذا(*p0 & File_غوټه::NAMED )
                p += 4;
            إذا( !seek_پای_ټکی )
                ofs += (p - p0) + 8;
            وإلا
            {
                size_t rawsz = (size_t)(بینښه)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = غوټه_NElems;
            }
        }
        fs->normalize_غوټه_Ofs(بلاک_Idx, ofs);
        بلاک_اندازه_2 = fs->fs_معلومات_blksz[بلاک_Idx];
    }
}

File_غوټه_Iterator::File_غوټه_Iterator(ثابت File_غوټه_Iterator& it)
{
    fs = it.fs;
    بلاک_Idx = it.بلاک_Idx;
    ofs = it.ofs;
    بلاک_اندازه_2 = it.بلاک_اندازه_2;
    غوټه_NElems = it.غوټه_NElems;
    idx = it.idx;
}

File_غوټه_Iterator& File_غوټه_Iterator::چلوونکی=(ثابت File_غوټه_Iterator& it)
{
    fs = it.fs;
    بلاک_Idx = it.بلاک_Idx;
    ofs = it.ofs;
    بلاک_اندازه_2 = it.بلاک_اندازه_2;
    غوټه_NElems = it.غوټه_NElems;
    idx = it.idx;
    أرجع *هذا;
}

File_غوټه File_غوټه_Iterator::چلوونکی *() ثابت
{
    أرجع File_غوټه(idx < غوټه_NElems ? fs : عدم, بلاک_Idx, ofs);
}

File_غوټه_Iterator& File_غوټه_Iterator::چلوونکی ++ ()
{
    إذا( idx == غوټه_NElems || !fs )
        أرجع *هذا;
    idx++;
    File_غوټه n(fs, بلاک_Idx, ofs);
    ofs += n.raw_اندازه();
    إذا( ofs >= بلاک_اندازه_2 )
    {
        fs->normalize_غوټه_Ofs(بلاک_Idx, ofs);
        بلاک_اندازه_2 = fs->fs_معلومات_blksz[بلاک_Idx];
    }
    أرجع *هذا;
}

File_غوټه_Iterator File_غوټه_Iterator::چلوونکی ++ (صحيح)
{
    File_غوټه_Iterator it = *هذا;
    ++(*هذا);
    أرجع it;
}

File_غوټه_Iterator& File_غوټه_Iterator::چلوونکی += (صحيح _ofs)
{
    CV_Assert( _ofs >= 0 );
    لكل( ; _ofs > 0; _ofs-- )
        هذا->چلوونکی ++();
    أرجع *هذا;
}

File_غوټه_Iterator& File_غوټه_Iterator::readRaw( ثابت String& fmt, فراغ* _data0, size_t maxsz)
{
    إذا( fs && idx < غوټه_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        صحيح fmt_جوړې[CV_FS_MAX_FMT_جوړې*2];
        صحيح fmt_pair_شمېر = fs::decodeFormat( fmt.c_str(), fmt_جوړې, CV_FS_MAX_FMT_جوړې );
        size_t esz = fs::calc_Struct_اندازه( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        لكل( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            لكل( صحيح k = 0; k < fmt_pair_شمېر; k++ )
            {
                صحيح elem_type = fmt_جوړې[k*2+1];
                صحيح elem_اندازه_2 = CV_ELEM_اندازه(elem_type);

                صحيح شمېر_2 = fmt_جوړې[k*2];
                offset = align_اندازه( offset, elem_اندازه_2 );
                uchar* معلومات = data0 + offset;

                لكل( صحيح i = 0; i < شمېر_2; i++, ++(*هذا) )
                {
                    File_غوټه غوټه = *(*هذا);
                    إذا( غوټه.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_اندازه_2 == 8 ? (int64_t)غوټه : (صحيح)غوټه);
                        بدل( elem_type )
                        {
                        حالة CV_8U:
                            *(uchar*)معلومات = saturate_cast<uchar>(ival);
                            معلومات++;
                            اكسر;
                        حالة CV_8S:
                            *(حرف*)معلومات = saturate_cast<schar>(ival);
                            معلومات++;
                            اكسر;
                        حالة CV_Bool:
                            *(منطقي*)معلومات = ival != 0;
                            معلومات++;
                            اكسر;
                        حالة CV_16U:
                            *(ushort*)معلومات = saturate_cast<ushort>(ival);
                            معلومات += حجم(ushort);
                            اكسر;
                        حالة CV_16S:
                            *(لنډ*)معلومات = saturate_cast<لنډ>(ival);
                            معلومات += حجم(لنډ);
                            اكسر;
                        حالة CV_32U:
                            *(بینښه*)معلومات = (بینښه)std::max(ival, (int64_t)0);
                            معلومات += حجم(بینښه);
                            اكسر;
                        حالة CV_32S:
                            *(صحيح*)معلومات = (صحيح)ival;
                            معلومات += حجم(صحيح);
                            اكسر;
                        حالة CV_32F:
                            *(عائم*)معلومات = (عائم)ival;
                            معلومات += حجم(عائم);
                            اكسر;
                        حالة CV_64U:
                            *(uint64_t*)معلومات = (uint64_t)ival;
                            معلومات += حجم(uint64_t);
                            اكسر;
                        حالة CV_64S:
                            *(int64_t*)معلومات = (int64_t)ival;
                            معلومات += حجم(int64_t);
                            اكسر;
                        حالة CV_64F:
                            *(مزدوج*)معلومات = (مزدوج)ival;
                            معلومات += حجم(مزدوج);
                            اكسر;
                        حالة CV_16F:
                            *(hfloat*)معلومات = hfloat((عائم)ival);
                            معلومات += حجم(hfloat);
                            اكسر;
                        حالة CV_16BF:
                            *(bfloat*)معلومات = bfloat((عائم)ival);
                            معلومات += حجم(bfloat);
                            اكسر;
                        حالة CV_8F_E4M3FN:
                            *(fp8_t*)معلومات = fp8_t((عائم)ival);
                            معلومات += حجم(fp8_t);
                            اكسر;
                        حالة CV_8F_E4M3FNUZ:
                            *(fp8a_t*)معلومات = fp8a_t((عائم)ival);
                            معلومات += حجم(fp8a_t);
                            اكسر;
                        افتراضي:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    وإلا إذا( غوټه.isReal() )
                    {
                        مزدوج fval = (مزدوج)غوټه;

                        بدل( elem_type )
                        {
                        حالة CV_8U:
                            *(uchar*)معلومات = saturate_cast<uchar>(fval);
                            معلومات++;
                            اكسر;
                        حالة CV_8S:
                            *(حرف*)معلومات = saturate_cast<schar>(fval);
                            معلومات++;
                            اكسر;
                        حالة CV_16U:
                            *(ushort*)معلومات = saturate_cast<ushort>(fval);
                            معلومات += حجم(ushort);
                            اكسر;
                        حالة CV_16S:
                            *(لنډ*)معلومات = saturate_cast<لنډ>(fval);
                            معلومات += حجم(لنډ);
                            اكسر;
                        حالة CV_32U:
                            *(صحيح*)معلومات = saturate_cast<بینښه>(fval);
                            معلومات += حجم(صحيح);
                            اكسر;
                        حالة CV_32S:
                            *(صحيح*)معلومات = saturate_cast<صحيح>(fval);
                            معلومات += حجم(صحيح);
                            اكسر;
                        حالة CV_32F:
                            *(عائم*)معلومات = (عائم)fval;
                            معلومات += حجم(عائم);
                            اكسر;
                        حالة CV_64U:
                            *(uint64_t*)معلومات = (uint64_t)round(std::max(fval, 0.));
                            معلومات += حجم(uint64_t);
                            اكسر;
                        حالة CV_64S:
                            *(int64_t*)معلومات = (int64_t)round(std::max(fval, 0.));
                            معلومات += حجم(int64_t);
                            اكسر;
                        حالة CV_64F:
                            *(مزدوج*)معلومات = fval;
                            معلومات += حجم(مزدوج);
                            اكسر;
                        حالة CV_16F:
                            *(hfloat*)معلومات = hfloat((عائم)fval);
                            معلومات += حجم(hfloat);
                            اكسر;
                        حالة CV_16BF:
                            *(bfloat*)معلومات = bfloat((عائم)fval);
                            معلومات += حجم(bfloat);
                            اكسر;
                        حالة CV_8F_E4M3FN:
                            *(fp8_t*)معلومات = fp8_t((عائم)fval);
                            معلومات += حجم(fp8_t);
                            اكسر;
                        حالة CV_8F_E4M3FNUZ:
                            *(fp8a_t*)معلومات = fp8a_t((عائم)fval);
                            معلومات += حجم(fp8a_t);
                            اكسر;
                        افتراضي:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    وإلا
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (صحيح)(معلومات - data0);
            }
        }
    }

    أرجع *هذا;
}

منطقي File_غوټه_Iterator::equal_منزل(ثابت File_غوټه_Iterator& it) ثابت
{
    أرجع fs == it.fs && بلاک_Idx == it.بلاک_Idx && ofs == it.ofs &&
           idx == it.idx && غوټه_NElems == it.غوټه_NElems;
}

size_t File_غوټه_Iterator::پاتې_شوی() ثابت
{
    أرجع غوټه_NElems - idx;
}

منطقي چلوونکی == ( ثابت File_غوټه_Iterator& it1, ثابت File_غوټه_Iterator& it2 )
{
    أرجع it1.equal_منزل(it2);
}

منطقي چلوونکی != ( ثابت File_غوټه_Iterator& it1, ثابت File_غوټه_Iterator& it2 )
{
    أرجع !it1.equal_منزل(it2);
}

فراغ اقرأ(ثابت File_غوټه& غوټه, صحيح& val, صحيح default_val)
{
    val = default_val;
    إذا( !غوټه.empty() )
    {
        val = (صحيح)غوټه;
    }
}

فراغ اقرأ(ثابت File_غوټه& غوټه, int64_t& val, int64_t default_val)
{
    val = default_val;
    إذا( !غوټه.empty() )
    {
        val = (int64_t)غوټه;
    }
}

فراغ اقرأ(ثابت File_غوټه& غوټه, مزدوج& val, مزدوج default_val)
{
    val = default_val;
    إذا( !غوټه.empty() )
    {
        val = (مزدوج)غوټه;
    }
}

فراغ اقرأ(ثابت File_غوټه& غوټه, عائم& val, عائم default_val)
{
    val = default_val;
    إذا( !غوټه.empty() )
    {
        val = (عائم)غوټه;
    }
}

فراغ اقرأ(ثابت File_غوټه& غوټه, std::string& val, ثابت std::string& default_val)
{
    val = default_val;
    إذا( !غوټه.empty() )
    {
        val = (std::string)غوټه;
    }
}
فراغ FileStorage::Impl::اكتب(ثابت String &کلی, منطقي value)
{
    CV_Assert(write_ډېرپېښ);
    إذا (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().اكتب(کلی.c_str(), (صحيح)value);
    }
    وإلا
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().اكتب(کلی.c_str(), value ? "true" : "false", خطأ);
    }
}

FileStorage_API::~FileStorage_API() {}

نطاق internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, ثابت std::string& name,
                                       صحيح flags, ثابت std::string& typeName)
{
    fs = &_fs;
    fs->پیل_Write_Struct(name, flags, typeName);
}

WriteStructContext::~WriteStructContext()
{
    fs->پای_ټکی_Write_Struct();
}

}

}
