// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "persistence.hpp"
#شامل "persistence_impl.hpp"
#شامل "persistence_base64_encoding.hpp"
#شامل <unordered_map>
#شامل <iterator>

#شامل <opencv2/core/utils/logger.hpp>

#تعریف_کرو CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

نامخلا cv
{

نامخلا fs
{

صحیح strcasecmp(ثابت حرف* s1, ثابت حرف* s2)
{
    ثابت حرف* dummy="";
    اگر(!s1) s1=dummy;
    اگر(!s2) s2=dummy;

    size_t len1 = لڑی_لمبائی(s1);
    size_t len2 = لڑی_لمبائی(s2);
    size_t i, len = std::min(len1, len2);
    برائے( i = 0; i < len; i++ )
    {
        صحیح d = tolower((صحیح)s1[i]) - tolower((صحیح)s2[i]);
        اگر( d != 0 )
            واپس d;
    }
    واپس len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

حرف* itoa( صحیح _val, حرف* بفر, صحیح /*radix*/ )
{
    ثابت صحیح radix = 10;
    حرف* ptr=بفر + 23 /* enough even for 64-bit integers */;
    غیرعلامتی val = مطلق_قدر(_val);

    *ptr = '\0';
    کرو
    {
        غیرعلامتی r = val / radix;
        *--ptr = (حرف)(val - (r*radix) + '0');
        val = r;
    }
    جب( val != 0 );

    اگر( _val < 0 )
        *--ptr = '-';

    واپس ptr;
}

حرف* itoa( int64_t _val, حرف* بفر, صحیح /*radix*/, منطقی _signed)
{
    ثابت صحیح radix = 10;
    حرف* ptr=بفر + 23 /* enough even for 64-bit integers */;
    صحیح sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : مطلق_قدر(_val);

    *ptr = '\0';
    کرو
    {
        uint64_t r = val / radix;
        *--ptr = (حرف)(val - (r*radix) + '0');
        val = r;
    }
    جب( val != 0 );

    اگر( sign < 0 )
        *--ptr = '-';

    واپس ptr;
}

حرف* double_مقصد_String( حرف* buf, size_t buf_سائز, دوہرا value, منطقی explicitZero )
{
    Cv64suf val;
    غیرعلامتی ieee754_hi;

    val.f = value;
    ieee754_hi = (غیرعلامتی)(val.u >> 32);

    اگر( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        صحیح ivalue = cv_باری(value);
        اگر( ivalue == value )
        {
            اگر( explicitZero )
                snprintf( buf, buf_سائز, "%d.0", ivalue );
            ورنہ
                snprintf( buf, buf_سائز, "%d.", ivalue );
        }
        ورنہ
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, buf_سائز, "%.17g", value );

            حرف* ptr = buf;
            اگر( *ptr == '+' || *ptr == '-' )
                ptr++;
            برائے( ; cv_isdigit(*ptr); ptr++ )
                ;
            اگر( *ptr == ',' )
                *ptr = '.';
        }
    }
    ورنہ
    {
        غیرعلامتی ieee754_lo = (غیرعلامتی)val.u;
        اگر( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        ورنہ
            strcpy( buf, (صحیح)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    واپس buf;
}

حرف* float_مقصد_String( حرف* buf, size_t buf_سائز, اعشاری value, منطقی halfprecision, منطقی explicitZero )
{
    Cv32suf val;
    غیرعلامتی ieee754;
    val.f = value;
    ieee754 = val.u;

    اگر( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        صحیح ivalue = cv_باری(value);
        اگر( ivalue == value )
        {
            اگر( explicitZero )
                snprintf( buf, buf_سائز, "%d.0", ivalue );
            ورنہ
                snprintf( buf, buf_سائز, "%d.", ivalue );
        }
        ورنہ
        {
            اگر (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, buf_سائز, "%.5g", value);
            }
            ورنہ
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, buf_سائز, "%.9g", value);
            }

            حرف* ptr = buf;
            اگر( *ptr == '+' || *ptr == '-' )
                ptr++;
            برائے( ; cv_isdigit(*ptr); ptr++ )
                ;
            اگر( *ptr == ',' )
                *ptr = '.';
        }
    }
    ورنہ
    {
        اگر( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        ورنہ
            strcpy( buf, (صحیح)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    واپس buf;
}

ساکن ثابت حرف symbols[] = "ucwsifdhHbUIneE";

ساکن حرف type_علامت(صحیح گہرائی)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_Check_گہرائی(گہرائی, گہرائی >= 0 && گہرائی <= CV_8F_E4M3FNUZ, "");
    واپس symbols[گہرائی];
}

ساکن صحیح علامت_مقصد_Type(حرف c)
{
    اگر (c == 'r')
        واپس CV_SEQ_ELTYPE_PTR;
    ثابت حرف* pos = strchr( symbols, c );
    اگر( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    واپس static_cast<صحیح>(pos - symbols);
}

حرف* encodeFormat(صحیح elem_type, حرف* dt, size_t dt_len)
{
    صحیح cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    حرف علامت = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_علامت(CV_MAT_گہرائی(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, علامت);
    واپس dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
حرف* encodeFormat(صحیح elem_type, حرف* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    واپس encodeFormat(elem_type, dt, max);
}

صحیح decodeFormat( ثابت حرف* dt, صحیح* fmt_جوڑے, صحیح max_len )
{
    صحیح fmt_pair_گنتی = 0;
    صحیح i = 0, k = 0, len = dt ? (صحیح)لڑی_لمبائی(dt) : 0;

    اگر( !dt || !len )
        واپس 0;

    CV_Assert( fmt_جوڑے != 0 && max_len > 0 );
    fmt_جوڑے[0] = 0;
    max_len *= 2;

    برائے( ; k < len; k++ )
    {
        حرف c = dt[k];

        اگر( cv_isdigit(c) )
        {
            صحیح گنتی_2 = c - '0';
            اگر( cv_isdigit(dt[k+1]) )
            {
                حرف* endptr = 0;
                گنتی_2 = (صحیح)strtol( dt+k, &endptr, 10 );
                k = (صحیح)(endptr - dt) - 1;
            }

            اگر( گنتی_2 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_جوڑے[i] = گنتی_2;
        }
        ورنہ
        {
            صحیح گہرائی = علامت_مقصد_Type(c);
            اگر( fmt_جوڑے[i] == 0 )
                fmt_جوڑے[i] = 1;
            fmt_جوڑے[i+1] = گہرائی;
            اگر( i > 0 && fmt_جوڑے[i+1] == fmt_جوڑے[i-1] )
                fmt_جوڑے[i-2] += fmt_جوڑے[i];
            ورنہ
            {
                i += 2;
                اگر( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_جوڑے[i] = 0;
        }
    }

    fmt_pair_گنتی = i/2;
    واپس fmt_pair_گنتی;
}

صحیح calc_Elem_سائز( ثابت حرف* dt, صحیح initial_سائز )
{
    صحیح size = 0;
    صحیح fmt_جوڑے[CV_FS_MAX_FMT_جوڑے*2], i, fmt_pair_گنتی;
    صحیح comp_سائز;

    fmt_pair_گنتی = decodeFormat( dt, fmt_جوڑے, CV_FS_MAX_FMT_جوڑے );
    fmt_pair_گنتی *= 2;
    برائے( i = 0, size = initial_سائز; i < fmt_pair_گنتی; i += 2 )
    {
        comp_سائز = CV_ELEM_سائز(fmt_جوڑے[i+1]);
        size = cvAlign( size, comp_سائز );
        size += comp_سائز * fmt_جوڑے[i];
    }
    اگر( initial_سائز == 0 )
    {
        comp_سائز = CV_ELEM_سائز(fmt_جوڑے[1]);
        size = cvAlign( size, comp_سائز );
    }
    واپس size;
}


صحیح calc_Struct_سائز( ثابت حرف* dt, صحیح initial_سائز )
{
    صحیح size = calc_Elem_سائز( dt, initial_سائز );
    size_t elem_max_سائز = 0;
    برائے ( ثابت حرف * type = dt; *type != '\0'; type++ )
    {
        حرف v = *type;
        اگر (v >= '0' && v <= '9')
            جاری;  // skip vector size
        انتخاب (v)
        {
        حالت 'u': { elem_max_سائز = std::max( elem_max_سائز, حجم(uchar ) ); توڑو; }
        حالت 'b': { elem_max_سائز = std::max( elem_max_سائز, حجم(منطقی  ) ); توڑو; }
        حالت 'c': { elem_max_سائز = std::max( elem_max_سائز, حجم(schar ) ); توڑو; }
        حالت 'w': { elem_max_سائز = std::max( elem_max_سائز, حجم(ushort) ); توڑو; }
        حالت 's': { elem_max_سائز = std::max( elem_max_سائز, حجم(مختصر ) ); توڑو; }
        حالت 'i': { elem_max_سائز = std::max( elem_max_سائز, حجم(صحیح   ) ); توڑو; }
        حالت 'n': { elem_max_سائز = std::max( elem_max_سائز, حجم(غیرعلامتی) ); توڑو; }
        حالت 'f': { elem_max_سائز = std::max( elem_max_سائز, حجم(اعشاری ) ); توڑو; }
        حالت 'd': { elem_max_سائز = std::max( elem_max_سائز, حجم(دوہرا) ); توڑو; }
        حالت 'h': { elem_max_سائز = std::max( elem_max_سائز, حجم(hfloat)); توڑو; }
        حالت 'H': { elem_max_سائز = std::max( elem_max_سائز, حجم(bfloat)); توڑو; }
        حالت 'e': { elem_max_سائز = std::max( elem_max_سائز, حجم(fp8_t) ); توڑو; }
        حالت 'E': { elem_max_سائز = std::max( elem_max_سائز, حجم(fp8a_t)); توڑو; }
        حالت 'I': { elem_max_سائز = std::max( elem_max_سائز, حجم(int64_t)); توڑو; }
        حالت 'U': { elem_max_سائز = std::max( elem_max_سائز, حجم(uint64_t)); توڑو; }
        طےشدہ:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (حرف)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<صحیح>(elem_max_سائز) );
    واپس size;
}

صحیح decodeSimpleFormat( ثابت حرف* dt )
{
    صحیح elem_type = -1;
    صحیح fmt_جوڑے[CV_FS_MAX_FMT_جوڑے*2], fmt_pair_گنتی;

    fmt_pair_گنتی = decodeFormat( dt, fmt_جوڑے, CV_FS_MAX_FMT_جوڑے );
    اگر( fmt_pair_گنتی != 1 || fmt_جوڑے[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_جوڑے[1], fmt_جوڑے[0] );

    واپس elem_type;
}

}

#اگر defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#تعریف_کرو CV_LITTLE_ENDIAN_MEM_ACCESS 1
#ورنہ
#تعریف_کرو CV_LITTLE_ENDIAN_MEM_ACCESS 0
#اگر_ختم

ساکن اندرونی صحیح readInt(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#اگر CV_LITTLE_ENDIAN_MEM_ACCESS
    صحیح val;
    حافظہ_نقل(&val, p, حجم(val));
    واپس val;
#ورنہ
    صحیح val = (صحیح)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    واپس val;
#اگر_ختم
}

ساکن اندرونی int64_t readLong(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#اگر CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    حافظہ_نقل(&val, p, حجم(val));
    واپس val;
#ورنہ
    غیرعلامتی val0 = (غیرعلامتی)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    غیرعلامتی val1 = (غیرعلامتی)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    واپس val0 | ((int64_t)val1 << 32);
#اگر_ختم
}

ساکن اندرونی دوہرا readReal(ثابت uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#اگر CV_LITTLE_ENDIAN_MEM_ACCESS
    دوہرا val;
    حافظہ_نقل(&val, p, حجم(val));
    واپس val;
#ورنہ
    غیرعلامتی val0 = (غیرعلامتی)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    غیرعلامتی val1 = (غیرعلامتی)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    واپس val.f;
#اگر_ختم
}

سانچہ <قسم_نام T>
ساکن اندرونی خالی writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#اگر CV_LITTLE_ENDIAN_MEM_ACCESS
    حافظہ_نقل(p, &ival, حجم(ival));
#ورنہ
    برائے (size_t i = 0, j = 0; i < حجم(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#اگر_ختم
}

ساکن اندرونی خالی writeReal(uchar* p, دوہرا fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#اگر CV_LITTLE_ENDIAN_MEM_ACCESS
    حافظہ_نقل(p, &fval, حجم(fval));
#ورنہ
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
#اگر_ختم
}



خالی FileStorage::Impl::init() {
    flags = 0;
    بفر.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = جھوٹ;
    state_کا_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = جھوٹ;
    delayed_struct_کلید = خالی_اشاریہ;
    delayed_struct_flags = 0;
    delayed_type_name = خالی_اشاریہ;
    base64_writer = خالی_اشاریہ;
    is_opened = جھوٹ;
    dummy_eof = جھوٹ;
    write_منوال = جھوٹ;
    mem_منوال = جھوٹ;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    خالی_خانے_دھارا = سچ;

    strbufv.clear();
    strbuf = 0;
    strbufsize = strbufpos = 0;
    roots.clear();

    fs_ڈیٹا.clear();
    fs_ڈیٹا_ptrs.clear();
    fs_ڈیٹا_blksz.clear();
    freeSpaceOfs = 0;

    str_hash.clear();
    str_hash_ڈیٹا.clear();
    str_hash_ڈیٹا.resize(1);
    str_hash_ڈیٹا[0] = '\0';

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

خالی FileStorage::Impl::release(String *out) {
    اگر (is_opened) {
        اگر (out)
            out->clear();
        اگر (write_منوال) {
            جب (write_ڈھیر.size() > 1) {
                انتہا_Write_Struct();
            }
            flush();
            اگر (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            ورنہ اگر (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        اگر (mem_منوال && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

خالی FileStorage::Impl::analyze_file_name(ثابت std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    ساکن ثابت حرف not_file_name = '\n';
    ساکن ثابت حرف parameter_begin = '?';
    ساکن ثابت حرف parameter_separator = '&';

    اگر (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        واپس;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    اگر (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        برائے (size_t param_beg = beg, param_انتہا = beg;
             param_انتہا < end;
             param_beg = param_انتہا + 1) {
            param_انتہا = file_name.find_first_of(parameter_separator, param_beg);
            اگر ((param_انتہا == std::string::npos || param_انتہا != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_انتہا - param_beg));
            }
        }
    }
}

منطقی FileStorage::Impl::open(ثابت حرف *filename_or_buf, صحیح _flags, ثابت حرف *encoding) {
    منطقی ok = سچ;
    release();

    منطقی append = (_flags & 3) == FileStorage::APPEND;
    mem_منوال = (_flags & FileStorage::حافظہ) != 0;

    write_منوال = (_flags & 3) != 0;
    منطقی write_base64 = (write_منوال || append) && (_flags & FileStorage::BASE64) != 0;

    منطقی isGZ = جھوٹ;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        اگر (!params.empty())
            filename = params[0];

        اگر (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_منوال || append);
    }

    اگر (filename.size() == 0 && !mem_منوال && !write_منوال)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    اگر (mem_منوال && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    اگر (!mem_منوال) {
        size_t dot_idx = filename.find_last_of('.');
        حرف compression = '\0';

        اگر (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            اگر (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                اگر (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = سچ;
                }

                ورنہ اگر(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = سچ;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        اگر (!isGZ) {
            file = fopen(filename.c_str(), !write_منوال ? "rt" : !append ? "wt" : "a+t");
            اگر (!file)
            {
                CV_LOG_ERROR(تہی, "Can't open file: '" << filename << "' in " << (!write_منوال ? "read" : !append ? "write" : "append") << " mode");
                واپس جھوٹ;
            }
        } ورنہ {
#اگر USE_ZLIB
            اگر (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            حرف منوال[] = {write_منوال ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), منوال);
            اگر (!gzfile)
            {
                CV_LOG_ERROR(تہی, "Can't open archive: '" << filename << "' mode=" << منوال);
                واپس جھوٹ;
            }
#ورنہ
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#اگر_ختم
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_ڈیٹا.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    اگر (write_منوال) {
        fmt = flags & FileStorage::FORMAT_نقاب;

        اگر (mem_منوال)
            outbuf.clear();

        اگر (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            ثابت حرف *dot_pos = تہی;
            ثابت حرف *dot_pos2 = تہی;
            // like strrchr() implementation, but save two last positions simultaneously
            برائے (ثابت حرف *pos = &filename[0]; pos[0] != 0; pos++) {
                اگر (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            اگر (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != تہی) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } ورنہ اگر (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        صحیح buf_سائز_2 = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        اگر (append) {
            fseek(file, 0, SEEK_END);
            اگر (ftell(file) == 0)
                append = جھوٹ;
        }

        write_ڈھیر.clear();
        خالی_خانے_دھارا = سچ;
        write_ڈھیر.push_back(FStruct_ڈیٹا("", File_گرہ::MAP | File_گرہ::خالی_خانے_2, 0));
        بفر.reserve(buf_سائز_2 + 1024);
        بفر.resize(buf_سائز_2);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_کا_writing_base64 = FileStorage_API::Base64State::Uncertain;

        اگر (fmt == FileStorage::FORMAT_XML) {
            size_t file_سائز_2 = file ? (size_t) ftell(file) : (size_t) 0;
            اگر (!append || file_سائز_2 == 0) {
                اگر (encoding && *encoding != '\0') {
                    اگر (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(لڑی_لمبائی(encoding) < 1000);
                    حرف buf[1100];
                    snprintf(buf, حجم(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } ورنہ
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } ورنہ {
                صحیح xml_buf_سائز = 1 << 10;
                حرف substr[] = "</opencv_storage>";
                صحیح آخری_occurrence = -1;
                xml_buf_سائز = MIN(xml_buf_سائز, صحیح(file_سائز_2));
                fseek(file, -xml_buf_سائز, SEEK_END);
                // find the last occurrence of </opencv_storage>
                برائے (;;) {
                    صحیح سطر_انحراف = (صحیح) ftell(file);
                    ثابت حرف *ptr0 = یہ->gets(xml_buf_سائز);
                    ثابت حرف *ptr = تہی;
                    اگر (!ptr0)
                        توڑو;
                    ptr = ptr0;
                    برائے (;;) {
                        ptr = لڑی_تلاش(ptr, substr);
                        اگر (!ptr)
                            توڑو;
                        آخری_occurrence = سطر_انحراف + (صحیح) (ptr - ptr0);
                        ptr += لڑی_لمبائی(substr);
                    }
                }
                اگر (آخری_occurrence < 0) {
                    release();
                    CV_Error(cv::Error::StsError, "Could not find </opencv_storage> in the end of file.\n");
                }
                closeFile();
                file = fopen(filename.c_str(), "r+t");
                CV_Assert(file != 0);
                fseek(file, آخری_occurrence, SEEK_SET);
                // replace the last "</opencv_storage>" with " <!-- resumed -->", which has the same length
                puts(" <!-- resumed -->");
                fseek(file, 0, SEEK_END);
                puts("\n");
            }

            emitter_do_not_use_براہ_راست_dereference = createXMLEmitter(یہ);
        } ورنہ اگر (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                اگر (!append) {
                    اگر (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    ورنہ
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            ورنہ
                puts("...\n---\n");

            emitter_do_not_use_براہ_راست_dereference = createYAMLEmitter(یہ);
        } ورنہ {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            اگر (!append)
                puts("{\n");
            ورنہ {
                منطقی valid = جھوٹ;
                طویل roffset = 0;
                برائے (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    ثابت حرف انتہا_mark = '}';
                    اگر (fgetc(file) == انتہا_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = سچ;
                        توڑو;
                    }
                }

                اگر (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } ورنہ {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_ڈھیر.back().indent = 4;
            emitter_do_not_use_براہ_راست_dereference = createJSONEmitter(یہ);
        }
        is_opened = سچ;
    } ورنہ {
        ثابت size_t buf_size0 = 40;
        بفر.resize(buf_size0);
        اگر (mem_منوال) {
            strbuf = (حرف *) filename_or_buf;
            strbufsize = لڑی_لمبائی(strbuf);
        }

        ثابت حرف *yaml_signature = "%YAML";
        ثابت حرف *json_signature = "{";
        ثابت حرف *xml_signature = "<?xml";
        حرف *buf = یہ->gets(16);
        CV_Assert(buf);
        حرف *bufPtr = cv_skip_BOM(buf);
        size_t buf_انحراف = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        اگر (لڑی_موازنہ(bufPtr, yaml_signature, لڑی_لمبائی(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        ورنہ اگر (لڑی_موازنہ(bufPtr, json_signature, لڑی_لمبائی(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        ورنہ اگر (لڑی_موازنہ(bufPtr, xml_signature, لڑی_لمبائی(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        ورنہ
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = buf_انحراف;
        bufofs = 0;

        کوشش {
            حرف *ptr = بفر_آغاز();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_گرہ جڑ_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_گرہ_Space(جڑ_nodes, 9);
            *rptr = File_گرہ::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            انتخاب (fmt) {
                حالت FileStorage::FORMAT_XML:
                    parser_do_not_use_براہ_راست_dereference = createXMLParser(یہ);
                    توڑو;
                حالت FileStorage::FORMAT_YAML:
                حالت FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_براہ_راست_dereference = createYAMLParser(یہ);
                    توڑو;
                حالت FileStorage::FORMAT_JSON:
                    parser_do_not_use_براہ_راست_dereference = createJSONParser(یہ);
                    توڑو;
                طےشدہ:
                    parser_do_not_use_براہ_راست_dereference = Ptr<FileStorageParser>();
            }

            اگر (!parser_do_not_use_براہ_راست_dereference.empty()) {
                ok = getParser().parse(ptr);
                اگر (ok) {
                    finalizeCollection(جڑ_nodes);

                    CV_Assert(!fs_ڈیٹا_ptrs.empty());
                    File_گرہ roots_گرہ(fs_ext, 0, 0);
                    size_t i, nroots = roots_گرہ.size();
                    File_گرہ_Iterator it = roots_گرہ.begin();

                    برائے (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        پکڑو (...)
        {
            // FIXIT log error message
            is_opened = سچ;
            release();
            پھینکو;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = سچ;
        std::vector<حرف> tmpbuf;
        std::swap(بفر, tmpbuf);
        bufofs = 0;
    }
    واپس ok;
}

خالی FileStorage::Impl::puts(ثابت حرف *str) {
    CV_Assert(write_منوال);
    اگر (mem_منوال)
        std::copy(str, str + لڑی_لمبائی(str), std::back_inserter(outbuf));
    ورنہ اگر (file)
        fputs(str, file);
#اگر USE_ZLIB
    ورنہ اگر (gzfile)
        gzputs(gzfile, str);
#اگر_ختم
    ورنہ
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

حرف *FileStorage::Impl::getsFromFile(حرف *buf, صحیح گنتی_2) {
    اگر (file)
        واپس fgets(buf, گنتی_2, file);
#اگر USE_ZLIB
    اگر (gzfile)
        واپس gzgets(gzfile, buf, گنتی_2);
#اگر_ختم
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

حرف *FileStorage::Impl::gets(size_t max_گنتی) {
    اگر (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        ثابت حرف *instr = strbuf;
        برائے (; i < len; i++) {
            حرف c = instr[i];
            اگر (c == '\0' || c == '\n') {
                اگر (c == '\n')
                    i++;
                توڑو;
            }
        }
        size_t گنتی_2 = i - strbufpos;
        اگر (max_گنتی == 0 || max_گنتی > گنتی_2)
            max_گنتی = گنتی_2;
        بفر.resize(std::max(بفر.size(), max_گنتی + 8));
        حافظہ_نقل(&بفر[0], instr + strbufpos, max_گنتی);
        بفر[max_گنتی] = '\0';
        strbufpos = i;
        واپس max_گنتی > 0 ? &بفر[0] : 0;
    }

    ثابت size_t MAX_بلاک_سائز = زیادہ_سے_زیادہ_عدد / 2; // hopefully, that will be enough
    اگر (max_گنتی == 0)
        max_گنتی = MAX_بلاک_سائز;
    ورنہ
        CV_Assert(max_گنتی < MAX_بلاک_سائز);
    size_t ofs = 0;

    برائے (;;) {
        صحیح گنتی_2 = (صحیح) std::min(بفر.size() - ofs - 16, max_گنتی);
        حرف *ptr = getsFromFile(&بفر[ofs], گنتی_2 + 1);
        اگر (!ptr)
            توڑو;
        صحیح delta = (صحیح) لڑی_لمبائی(ptr);
        ofs += delta;
        max_گنتی -= delta;
        اگر (delta == 0 || ptr[delta - 1] == '\n' || max_گنتی == 0)
            توڑو;
        اگر (delta == گنتی_2)
            بفر.resize((size_t) (بفر.size() * 1.5));
    }
    واپس ofs > 0 ? &بفر[0] : 0;
}

حرف *FileStorage::Impl::gets() {
    حرف *ptr = یہ->gets(0);
    اگر (!ptr) {
        ptr = بفر_آغاز();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        سیٹ_Eof();
        واپس 0;
    } ورنہ {
        size_t l = لڑی_لمبائی(ptr);
        اگر (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    واپس ptr;
}

منطقی FileStorage::Impl::eof() {
    اگر (dummy_eof)
        واپس سچ;
    اگر (strbuf)
        واپس strbufpos >= strbufsize;
    اگر (file)
        واپس feof(file) != 0;
#اگر USE_ZLIB
    اگر (gzfile)
        واپس gzeof(gzfile) != 0;
#اگر_ختم
    واپس جھوٹ;
}

خالی FileStorage::Impl::سیٹ_Eof() {
    dummy_eof = سچ;
}

خالی FileStorage::Impl::closeFile() {
    اگر (file)
        fclose(file);
#اگر USE_ZLIB
    ورنہ اگر (gzfile)
        gzclose(gzfile);
#اگر_ختم
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = جھوٹ;
}

خالی FileStorage::Impl::rewind() {
    اگر (file)
        ::rewind(file);
#اگر USE_ZLIB
    ورنہ اگر (gzfile)
        gzrewind(gzfile);
#اگر_ختم
    strbufpos = 0;
}

حرف *FileStorage::Impl::resize_Write_بفر(حرف *ptr, صحیح len) {
    ثابت حرف *بفر_انتہا = &بفر[0] + بفر.size();
    اگر (ptr + len < بفر_انتہا)
        واپس ptr;

    ثابت حرف *بفر_آغاز_2 = &بفر[0];
    صحیح written_len = (صحیح) (ptr - بفر_آغاز_2);

    CV_Assert(written_len <= (صحیح) بفر.size());
    صحیح new_سائز_2 = (صحیح) ((بفر_انتہا - بفر_آغاز_2) * 3 / 2);
    new_سائز_2 = MAX(written_len + len, new_سائز_2);
    بفر.reserve(new_سائز_2 + 256);
    بفر.resize(new_سائز_2);
    bufofs = written_len;
    واپس &بفر[0] + bufofs;
}

حرف *FileStorage::Impl::flush() {
    حرف *بفر_آغاز_2 = &بفر[0];
    حرف *ptr = بفر_آغاز_2 + bufofs;

    اگر (ptr > بفر_آغاز_2 + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(بفر_آغاز_2);
        bufofs = 0;
    }

    صحیح indent = write_ڈھیر.back().indent;

    اگر (space != indent) {
        memset(بفر_آغاز_2, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = بفر_آغاز_2 + bufofs;

    واپس ptr;
}

خالی FileStorage::Impl::انتہا_Write_Struct() {
    CV_Assert(write_منوال);

    check_if_write_struct_is_delayed(جھوٹ);
    اگر (state_کا_writing_base64 != FileStorage_API::Uncertain)
        switch_مقصد_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_ڈھیر.empty());

    FStruct_ڈیٹا &current_struct = write_ڈھیر.back();
    اگر (fmt == FileStorage::FORMAT_JSON && !File_گرہ::isFlow(current_struct.flags) && write_ڈھیر.size() > 1)
        current_struct.indent = write_ڈھیر[write_ڈھیر.size() - 2].indent;

    getEmitter().انتہا_Write_Struct(current_struct);

    write_ڈھیر.pop_back();
    اگر (!write_ڈھیر.empty())
        write_ڈھیر.back().flags &= ~File_گرہ::خالی_خانے_2;
}

خالی FileStorage::Impl::آغاز_Write_Struct_helper(ثابت حرف *کلید, صحیح struct_flags,
                                                ثابت حرف *type_name) {
    CV_Assert(write_منوال);

    struct_flags = (struct_flags & (File_گرہ::TYPE_نقاب | File_گرہ::FLOW)) | File_گرہ::خالی_خانے_2;
    اگر (!File_گرہ::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    اگر (type_name && type_name[0] == '\0')
        type_name = 0;

    FStruct_ڈیٹا s = getEmitter().آغاز_Write_Struct(write_ڈھیر.back(), کلید, struct_flags, type_name);

    write_ڈھیر.push_back(s);
    size_t write_ڈھیر_سائز = write_ڈھیر.size();
    اگر (write_ڈھیر_سائز > 1)
        write_ڈھیر[write_ڈھیر_سائز - 2].flags &= ~File_گرہ::خالی_خانے_2;

    اگر (fmt != FileStorage::FORMAT_JSON && !File_گرہ::isFlow(s.flags))
        flush();

    اگر (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_گرہ::isMap(struct_flags)) {
        getEmitter().اكتب("type_id", type_name, جھوٹ);
    }
}

خالی FileStorage::Impl::آغاز_Write_Struct(ثابت حرف *کلید, صحیح struct_flags,
                                         ثابت حرف *type_name) {
    check_if_write_struct_is_delayed(جھوٹ);
    اگر (state_کا_writing_base64 == FileStorage_API::NotUse)
        switch_مقصد_Base64_state(FileStorage_API::Uncertain);

    اگر (state_کا_writing_base64 == FileStorage_API::Uncertain && File_گرہ::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(کلید, struct_flags, type_name);
    } ورنہ اگر (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        اگر ((File_گرہ::TYPE_نقاب & struct_flags) != File_گرہ::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        ورنہ اگر (state_کا_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        آغاز_Write_Struct_helper(کلید, struct_flags, "binary");

        اگر (state_کا_writing_base64 != FileStorage_API::Uncertain)
            switch_مقصد_Base64_state(FileStorage_API::Uncertain);
        switch_مقصد_Base64_state(FileStorage_API::اندر_Use);
    } ورنہ {
        /* Won't output Base64 data */
        اگر (state_کا_writing_base64 == FileStorage_API::اندر_Use)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        آغاز_Write_Struct_helper(کلید, struct_flags, type_name);

        اگر (state_کا_writing_base64 != FileStorage_API::Uncertain)
            switch_مقصد_Base64_state(FileStorage_API::Uncertain);
        switch_مقصد_Base64_state(FileStorage_API::NotUse);
    }
}

خالی FileStorage::Impl::writeComment(ثابت حرف *comment, منطقی eol_comment) {
    CV_Assert(write_منوال);
    getEmitter().writeComment(comment, eol_comment);
}

خالی FileStorage::Impl::آغاز_اگلا_دھارا() {
    CV_Assert(write_منوال);
    اگر (!خالی_خانے_دھارا) {
        جب (!write_ڈھیر.empty())
            انتہا_Write_Struct();
        flush();
        getEmitter().آغاز_اگلا_دھارا();
        خالی_خانے_دھارا = سچ;
        write_ڈھیر.push_back(FStruct_ڈیٹا("", File_گرہ::خالی_خانے_2, 0));
        bufofs = 0;
    }
}

خالی FileStorage::Impl::اكتب(ثابت String &کلید, صحیح value) {
    CV_Assert(write_منوال);
    getEmitter().اكتب(کلید.c_str(), value);
}

خالی FileStorage::Impl::اكتب(ثابت String &کلید, int64_t value) {
    CV_Assert(write_منوال);
    getEmitter().اكتب(کلید.c_str(), value);
}

خالی FileStorage::Impl::اكتب(ثابت String &کلید, دوہرا value) {
    CV_Assert(write_منوال);
    getEmitter().اكتب(کلید.c_str(), value);
}

خالی FileStorage::Impl::اكتب(ثابت String &کلید, ثابت String &value) {
    CV_Assert(write_منوال);
    getEmitter().اكتب(کلید.c_str(), value.c_str(), جھوٹ);
}

خالی FileStorage::Impl::write_Raw_ڈیٹا(ثابت std::string &dt, ثابت خالی *ڈیٹا_3, size_t len) {
    CV_Assert(write_منوال);

    اگر (is_using_base64 || state_کا_writing_base64 == FileStorage_API::Base64State::اندر_Use) {
        write_Raw_ڈیٹا_Base64(ڈیٹا_3, len, dt.c_str());
        واپس;
    } ورنہ اگر (state_کا_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_مقصد_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elem_سائز = fs::calc_Struct_سائز(dt.c_str(), 0);
    CV_Assert(elem_سائز);
    CV_Assert(len % elem_سائز == 0);
    len /= elem_سائز;

    منطقی explicitZero = fmt == FileStorage::FORMAT_JSON;
    ثابت uchar *data0 = (ثابت uchar *) ڈیٹا_3;
    صحیح fmt_جوڑے[CV_FS_MAX_FMT_جوڑے * 2], k, fmt_pair_گنتی;
    حرف buf[256] = "";

    fmt_pair_گنتی = fs::decodeFormat(dt.c_str(), fmt_جوڑے, CV_FS_MAX_FMT_جوڑے);

    اگر (!len)
        واپس;

    اگر (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    اگر (fmt_pair_گنتی == 1) {
        fmt_جوڑے[0] *= (صحیح) len;
        len = 1;
    }

    برائے (; len--; data0 += elem_سائز) {
        صحیح offset = 0;
        برائے (k = 0; k < fmt_pair_گنتی; k++) {
            صحیح i, گنتی_2 = fmt_جوڑے[k * 2];
            صحیح elem_type = fmt_جوڑے[k * 2 + 1];
            صحیح elem_سائز_2 = CV_ELEM_سائز(elem_type);
            ثابت حرف *ptr;

            offset = cvAlign(offset, elem_سائز_2);
            ثابت uchar *ڈیٹا = data0 + offset;

            برائے (i = 0; i < گنتی_2; i++) {
                انتخاب (elem_type) {
                    حالت CV_8U:
                        ptr = fs::itoa(*(uchar *) ڈیٹا, buf, 10);
                        ڈیٹا++;
                        توڑو;
                    حالت CV_Bool:
                        ptr = fs::itoa(*(uchar *) ڈیٹا != 0, buf, 10);
                        ڈیٹا++;
                        توڑو;
                    حالت CV_8S:
                        ptr = fs::itoa(*(حرف *) ڈیٹا, buf, 10);
                        ڈیٹا++;
                        توڑو;
                    حالت CV_16U:
                        ptr = fs::itoa(*(ushort *) ڈیٹا, buf, 10);
                        ڈیٹا += حجم(ushort);
                        توڑو;
                    حالت CV_16S:
                        ptr = fs::itoa(*(مختصر *) ڈیٹا, buf, 10);
                        ڈیٹا += حجم(مختصر);
                        توڑو;
                    حالت CV_32U:
                        ptr = fs::itoa((int64_t)*(غیرعلامتی*) ڈیٹا, buf, 10, جھوٹ);
                        ڈیٹا += حجم(غیرعلامتی);
                        توڑو;
                    حالت CV_32S:
                        ptr = fs::itoa(*(صحیح *) ڈیٹا, buf, 10);
                        ڈیٹا += حجم(صحیح);
                        توڑو;
                    حالت CV_64U:
                        ptr = fs::itoa(*(uint64_t*) ڈیٹا, buf, 10, جھوٹ);
                        ڈیٹا += حجم(uint64_t);
                        توڑو;
                    حالت CV_64S:
                        ptr = fs::itoa(*(int64_t*) ڈیٹا, buf, 10, سچ);
                        ڈیٹا += حجم(int64_t);
                        توڑو;
                    حالت CV_32F:
                        ptr = fs::float_مقصد_String(buf, حجم(buf), *(اعشاری *) ڈیٹا, جھوٹ, explicitZero);
                        ڈیٹا += حجم(اعشاری);
                        توڑو;
                    حالت CV_64F:
                        ptr = fs::double_مقصد_String(buf, حجم(buf), *(دوہرا *) ڈیٹا, explicitZero);
                        ڈیٹا += حجم(دوہرا);
                        توڑو;
                    حالت CV_16F:
                        ptr = fs::float_مقصد_String(buf, حجم(buf), (اعشاری) *(hfloat *) ڈیٹا, سچ, explicitZero);
                        ڈیٹا += حجم(hfloat);
                        توڑو;
                    حالت CV_16BF:
                        ptr = fs::float_مقصد_String(buf, حجم(buf), (اعشاری) *(bfloat *) ڈیٹا, سچ, explicitZero);
                        ڈیٹا += حجم(bfloat);
                        توڑو;
                    حالت CV_8F_E4M3FN:
                        ptr = fs::float_مقصد_String(buf, حجم(buf), (اعشاری) *(fp8_t *) ڈیٹا, سچ, explicitZero);
                        ڈیٹا += حجم(fp8_t);
                        توڑو;
                    حالت CV_8F_E4M3FNUZ:
                        ptr = fs::float_مقصد_String(buf, حجم(buf), (اعشاری) *(fp8a_t *) ڈیٹا, سچ, explicitZero);
                        ڈیٹا += حجم(fp8a_t);
                        توڑو;
                    طےشدہ:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        واپس;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (صحیح) (ڈیٹا - data0);
        }
    }
}

خالی FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(جھوٹ);

    اگر (state_کا_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_مقصد_Base64_state(FileStorage_API::Base64State::Uncertain);
}

خالی FileStorage::Impl::switch_مقصد_Base64_state(FileStorage_API::Base64State new_state) {
    ثابت حرف *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    ثابت حرف *err_unable_مقصد_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    انتخاب (state_کا_writing_base64) {
        حالت FileStorage_API::Base64State::Uncertain:
            انتخاب (new_state) {
                حالت FileStorage_API::Base64State::اندر_Use:
                {
                    CV_DbgAssert(base64_writer == 0);
                    منطقی can_indent = (fmt != cv::FileStorage::منوال_2::FORMAT_JSON);
                    base64_writer = نیا base64::Base64Writer(*یہ, can_indent);
                    اگر (!can_indent) {
                        حرف *ptr = بفر_Ptr();
                        *ptr++ = '\0';
                        puts(بفر_آغاز());
                        سیٹ_بفر_Ptr(بفر_آغاز());
                        memset(بفر_آغاز(), 0, static_cast<صحیح>(space));
                        puts("\"$base64$");
                    }
                    توڑو;
                }
                حالت FileStorage_API::Base64State::Uncertain:
                    توڑو;
                حالت FileStorage_API::Base64State::NotUse:
                    توڑو;
                طےشدہ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    توڑو;
            }
            توڑو;
        حالت FileStorage_API::Base64State::اندر_Use:
            انتخاب (new_state) {
                حالت FileStorage_API::Base64State::اندر_Use:
                حالت FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_مقصد_switch);
                    توڑو;
                حالت FileStorage_API::Base64State::Uncertain:
                    حذف base64_writer;
                    base64_writer = 0;
                    اگر ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        سیٹ_بفر_Ptr(بفر_آغاز());
                        flush();
                        memset(بفر_آغاز(), 0, static_cast<صحیح>(space) );
                        سیٹ_بفر_Ptr(بفر_آغاز());
                    }
                    توڑو;
                طےشدہ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    توڑو;
            }
            توڑو;
        حالت FileStorage_API::Base64State::NotUse:
            انتخاب (new_state) {
                حالت FileStorage_API::Base64State::اندر_Use:
                حالت FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_مقصد_switch);
                    توڑو;
                حالت FileStorage_API::Base64State::Uncertain:
                    توڑو;
                طےشدہ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    توڑو;
            }
            توڑو;
        طےشدہ:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            توڑو;
    }

    state_کا_writing_base64 = new_state;
}

خالی FileStorage::Impl::make_write_struct_delayed(ثابت حرف *کلید, صحیح struct_flags, ثابت حرف *type_name) {
    CV_Assert(is_write_struct_delayed == جھوٹ);
    CV_DbgAssert(delayed_struct_کلید == خالی_اشاریہ);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == خالی_اشاریہ);

    delayed_struct_flags = struct_flags;

    اگر (کلید != خالی_اشاریہ) {
        delayed_struct_کلید = نیا حرف[لڑی_لمبائی(کلید) + 1U];
        strcpy(delayed_struct_کلید, کلید);
    }

    اگر (type_name != خالی_اشاریہ) {
        delayed_type_name = نیا حرف[لڑی_لمبائی(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = سچ;
}

خالی FileStorage::Impl::check_if_write_struct_is_delayed(منطقی change_type_مقصد_base64) {
    اگر (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_کلید;
        std::string type_name;
        صحیح struct_flags = delayed_struct_flags;

        اگر (delayed_struct_کلید != خالی_اشاریہ && *delayed_struct_کلید != '\0') {
            struct_کلید.assign(delayed_struct_کلید);
        }
        اگر (delayed_type_name != خالی_اشاریہ && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        حذف[] delayed_struct_کلید;
        حذف[] delayed_type_name;
        delayed_struct_کلید = خالی_اشاریہ;
        delayed_struct_flags = 0;
        delayed_type_name = خالی_اشاریہ;

        is_write_struct_delayed = جھوٹ;

        /* call */
        اگر (change_type_مقصد_base64) {
            آغاز_Write_Struct_helper(struct_کلید.c_str(), struct_flags, "binary");
            اگر (state_کا_writing_base64 != FileStorage_API::Uncertain)
                switch_مقصد_Base64_state(FileStorage_API::Uncertain);
            switch_مقصد_Base64_state(FileStorage_API::اندر_Use);
        } ورنہ {
            آغاز_Write_Struct_helper(struct_کلید.c_str(), struct_flags, type_name.c_str());
            اگر (state_کا_writing_base64 != FileStorage_API::Uncertain)
                switch_مقصد_Base64_state(FileStorage_API::Uncertain);
            switch_مقصد_Base64_state(FileStorage_API::NotUse);
        }
    }
}

خالی FileStorage::Impl::write_Raw_ڈیٹا_Base64(ثابت خالی *ڈیٹا_3, size_t len, ثابت حرف *dt) {
    CV_Assert(write_منوال);

    check_if_write_struct_is_delayed(سچ);

    اگر (state_کا_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_مقصد_Base64_state(FileStorage_API::Base64State::اندر_Use);
    } ورنہ اگر (state_کا_writing_base64 != FileStorage_API::Base64State::اندر_Use) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->اكتب(ڈیٹا_3, len, dt);
}

File_گرہ FileStorage::Impl::get_پہلا_چوٹی_Level_گرہ() ثابت {
    واپس roots.empty() ? File_گرہ() : roots[0];
}

File_گرہ FileStorage::Impl::جڑ(صحیح دھارا_Idx) ثابت {
    واپس دھارا_Idx >= 0 && دھارا_Idx < (صحیح) roots.size() ? roots[دھارا_Idx] : File_گرہ();
}

File_گرہ FileStorage::Impl::عامل[](ثابت String &nodename) ثابت {
    واپس یہ->عامل[](nodename.c_str());
}

File_گرہ FileStorage::Impl::عامل[](ثابت حرف * /*nodename*/) ثابت {
    واپس File_گرہ();
}

صحیح FileStorage::Impl::getFormat() ثابت { واپس fmt; }

حرف *FileStorage::Impl::بفر_Ptr() ثابت { واپس (حرف *) (&بفر[0] + bufofs); }

حرف *FileStorage::Impl::بفر_آغاز() ثابت { واپس (حرف *) &بفر[0]; }

حرف *FileStorage::Impl::بفر_انتہا_2() ثابت { واپس (حرف *) (&بفر[0] + بفر.size()); }

خالی FileStorage::Impl::سیٹ_بفر_Ptr(حرف *ptr) {
    حرف *bufferstart = بفر_آغاز();
    CV_Assert(ptr >= bufferstart && ptr <= بفر_انتہا_2());
    bufofs = ptr - bufferstart;
}

صحیح FileStorage::Impl::wrapMargin() ثابت { واپس wrap_margin; }

FStruct_ڈیٹا &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_ڈھیر.empty());
    واپس write_ڈھیر.back();
}

خالی FileStorage::Impl::سیٹ_Non_خالی_خانے() {
    خالی_خانے_دھارا = جھوٹ;
}

خالی FileStorage::Impl::processSpecialDouble(حرف *buf, دوہرا *value, حرف **endptr) {
    FileStorage_API *fs = یہ;
    حرف c = buf[0];
    صحیح inf_hi = 0x7ff00000;

    اگر (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    اگر (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    اگر (بڑا_حرف(buf[1]) == 'I' && بڑا_حرف(buf[2]) == 'N' && بڑا_حرف(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    ورنہ اگر (بڑا_حرف(buf[1]) == 'N' && بڑا_حرف(buf[2]) == 'A' && بڑا_حرف(buf[3]) == 'N')
        v.u = (uint64) -1;
    ورنہ
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

دوہرا FileStorage::Impl::strtod(حرف *ptr, حرف **endptr) {
    دوہرا fval = ::strtod(ptr, endptr);
    اگر (**endptr == '.') {
        حرف *dot_pos = *endptr;
        *dot_pos = ',';
        دوہرا fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        اگر (*endptr > dot_pos)
            fval = fval2;
        ورنہ
            *endptr = dot_pos;
    }

    اگر (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    واپس fval;
}

خالی FileStorage::Impl::convert_مقصد_Collection(صحیح type, File_گرہ &گرہ) {
    CV_Assert(type == File_گرہ::SEQ || type == File_گرہ::MAP);

    صحیح گرہ_type = گرہ.type();
    اگر (گرہ_type == type)
        واپس;

    منطقی named = گرہ.isNamed();
    uchar *ptr = گرہ.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    دوہرا fval = 0;
    std::string sval;
    منطقی جوڑو_پہلا_scalar = جھوٹ;

    اگر (گرہ_type != File_گرہ::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_گرہ::SEQ);
        اگر (گرہ_type == File_گرہ::INT) {
            ival = readLong(ptr);
            جوڑو_پہلا_scalar = سچ;
        } ورنہ اگر (گرہ_type == File_گرہ::REAL) {
            fval = readReal(ptr);
            جوڑو_پہلا_scalar = سچ;
        } ورنہ اگر (گرہ_type == File_گرہ::STRING) {
            sval = std::string(گرہ);
            جوڑو_پہلا_scalar = سچ;
        } ورنہ
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", گرہ_type));
    }

    ptr = reserve_گرہ_Space(گرہ, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_گرہ::NAMED : 0));
    // name has been copied automatically
    اگر (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    اگر (جوڑو_پہلا_scalar)
        جوڑو_گرہ(گرہ, std::string(), گرہ_type,
                گرہ_type == File_گرہ::INT ? (ثابت خالی *) &ival :
                گرہ_type == File_گرہ::REAL ? (ثابت خالی *) &fval :
                گرہ_type == File_گرہ::STRING ? (ثابت خالی *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_گرہ_Space(File_گرہ &گرہ, size_t sz) {
    منطقی shrink_بلاک = جھوٹ;
    size_t shrink_بلاک_Idx = 0, shrink_سائز = 0;

    uchar *ptr = 0, *بلاک_انتہا = 0;

    اگر (!fs_ڈیٹا_ptrs.empty()) {
        size_t بلاک_Idx = گرہ.بلاک_Idx;
        size_t ofs = گرہ.ofs;
        CV_Assert(بلاک_Idx == fs_ڈیٹا_ptrs.size() - 1);
        CV_Assert(ofs <= fs_ڈیٹا_blksz[بلاک_Idx]);
        CV_Assert(freeSpaceOfs <= fs_ڈیٹا_blksz[بلاک_Idx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_ڈیٹا_ptrs[بلاک_Idx] + ofs;
        بلاک_انتہا = fs_ڈیٹا_ptrs[بلاک_Idx] + fs_ڈیٹا_blksz[بلاک_Idx];

        CV_Assert(ptr >= fs_ڈیٹا_ptrs[بلاک_Idx] && ptr <= بلاک_انتہا);
        اگر (ptr + sz <= بلاک_انتہا) {
            freeSpaceOfs = ofs + sz;
            واپس ptr;
        }

        اگر (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_ڈیٹا[بلاک_Idx]->resize(sz);
            ptr = &fs_ڈیٹا[بلاک_Idx]->at(0);
            fs_ڈیٹا_ptrs[بلاک_Idx] = ptr;
            fs_ڈیٹا_blksz[بلاک_Idx] = sz;
            freeSpaceOfs = sz;
            واپس ptr;
        }

        shrink_بلاک = سچ;
        shrink_بلاک_Idx = بلاک_Idx;
        shrink_سائز = ofs;
    }

    size_t بلاک_سائز_2 = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(بلاک_سائز_2);
    fs_ڈیٹا.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_ڈیٹا_ptrs.push_back(new_ptr);
    fs_ڈیٹا_blksz.push_back(بلاک_سائز_2);
    گرہ.بلاک_Idx = fs_ڈیٹا_ptrs.size() - 1;
    گرہ.ofs = 0;
    freeSpaceOfs = sz;

    اگر (ptr && ptr + 5 <= بلاک_انتہا) {
        new_ptr[0] = ptr[0];
        اگر (ptr[0] & File_گرہ::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    اگر (shrink_بلاک) {
        fs_ڈیٹا[shrink_بلاک_Idx]->resize(shrink_سائز);
        fs_ڈیٹا_blksz[shrink_بلاک_Idx] = shrink_سائز;
    }

    واپس new_ptr;
}

غیرعلامتی FileStorage::Impl::getStringOfs(ثابت std::string &کلید) ثابت {
    str_hash_t::const_iterator it = str_hash.find(کلید);
    واپس it != str_hash.end() ? it->second : 0;
}

File_گرہ FileStorage::Impl::جوڑو_گرہ(File_گرہ &collection, ثابت std::string &کلید,
                                    صحیح elem_type, ثابت خالی *value, صحیح len) {
    FileStorage_API *fs = یہ;
    منطقی noname = کلید.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(کلید.c_str(), "_") == 0);
    convert_مقصد_Collection(noname ? File_گرہ::SEQ : File_گرہ::MAP, collection);

    منطقی isseq = collection.empty() ? جھوٹ : collection.isSeq();
    اگر (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    غیرعلامتی strofs = 0;
    اگر (!noname) {
        strofs = getStringOfs(کلید);
        اگر (!strofs) {
            strofs = (غیرعلامتی) str_hash_ڈیٹا.size();
            size_t keysize = کلید.size() + 1;
            str_hash_ڈیٹا.resize(strofs + keysize);
            حافظہ_نقل(&str_hash_ڈیٹا[0] + strofs, &کلید[0], keysize);
            str_hash.insert(std::make_pair(کلید, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t بلاک_Idx = fs_ڈیٹا_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_گرہ گرہ(fs_ext, بلاک_Idx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_گرہ_Space(گرہ, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_گرہ::NAMED));
    اگر (elem_type == File_گرہ::NONE)
        freeSpaceOfs -= 8;

    اگر (!noname) {
        writeInt(ptr, (صحیح) strofs);
        ptr += 4;
    }

    اگر (elem_type == File_گرہ::SEQ || elem_type == File_گرہ::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    اگر (value)
        گرہ.سیٹ_قدر(elem_type, value, len);

    اگر (collection.isNamed())
        cp += 4;
    صحیح nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    واپس گرہ;
}

خالی FileStorage::Impl::finalizeCollection(File_گرہ &collection) {
    اگر (!collection.isSeq() && !collection.isMap())
        واپس;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    اگر (*ptr0 & File_گرہ::NAMED)
        ptr += 4;
    size_t بلاک_Idx = collection.بلاک_Idx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t raw_سائز = 4;
    غیرعلامتی sz = (غیرعلامتی) readInt(ptr + 4);
    اگر (sz > 0) {
        size_t آخری_بلاک_Idx = fs_ڈیٹا_ptrs.size() - 1;

        برائے (; بلاک_Idx < آخری_بلاک_Idx; بلاک_Idx++) {
            raw_سائز += fs_ڈیٹا_blksz[بلاک_Idx] - ofs;
            ofs = 0;
        }
    }
    raw_سائز += freeSpaceOfs - ofs;
    writeInt(ptr, (صحیح) raw_سائز);
}

خالی FileStorage::Impl::normalize_گرہ_Ofs(size_t &بلاک_Idx, size_t &ofs) ثابت {
    جب (ofs >= fs_ڈیٹا_blksz[بلاک_Idx]) {
        اگر (بلاک_Idx == fs_ڈیٹا_blksz.size() - 1) {
            CV_Assert(ofs == fs_ڈیٹا_blksz[بلاک_Idx]);
            توڑو;
        }
        ofs -= fs_ڈیٹا_blksz[بلاک_Idx];
        بلاک_Idx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_کا_writing_base64() {
    واپس state_کا_writing_base64;
}

صحیح FileStorage::Impl::get_space() {
    واپس space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = سچ;
}

خالی FileStorage::Impl::Base64Decoder::init(ثابت Ptr<FileStorageParser> &_parser, حرف *_ptr, صحیح _indent) {
    parser_do_not_use_براہ_راست_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = جھوٹ;
}

منطقی FileStorage::Impl::Base64Decoder::read_مزید(صحیح needed) {
    ساکن ثابت uchar base64tab[] =
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

    اگر (eos)
        واپس جھوٹ;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    برائے (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    حرف *beg = 0, *end = 0;
    منطقی ok = getParser().get_Base64_صف(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    اگر (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = سچ;
        size_t tc = totalchars;
        برائے (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    صحیح i = 0, j, n = (صحیح) encoded.size();
    اگر (n > 0) {
        ثابت uchar *tab = base64tab;
        حرف *src = &encoded[0];

        برائے (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(صحیح) (uchar) src[i]], c = tab[(صحیح) (uchar) src[i + 1]];
            uchar b = tab[(صحیح) (uchar) src[i + 2]], a = tab[(صحیح) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    اگر (i > 0 && encoded[i - 1] == '=') {
        اگر (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        اگر (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    برائے (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    واپس (صحیح) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    اگر (ofs >= sz && !read_مزید(1))
        واپس (uchar) 0;
    واپس decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    اگر (ofs + 2 > sz && !read_مزید(2))
        واپس (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    واپس val;
}

صحیح FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    اگر (ofs + 4 > sz && !read_مزید(4))
        واپس 0;
    صحیح ival = readInt(&decoded[ofs]);
    ofs += 4;
    واپس ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    اگر (ofs + 8 > sz && !read_مزید(8))
        واپس 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    واپس ival;
}

دوہرا FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    اگر (ofs + 8 > sz && !read_مزید(8))
        واپس 0;
    دوہرا fval = readReal(&decoded[ofs]);
    ofs += 8;
    واپس fval;
}

منطقی FileStorage::Impl::Base64Decoder::انتہا_کا_دھارا() ثابت { واپس eos; }

حرف *FileStorage::Impl::Base64Decoder::getPtr() ثابت { واپس ptr; }


حرف *FileStorage::Impl::parseBase64(حرف *ptr, صحیح indent, File_گرہ &collection) {
    ثابت صحیح BASE64_HDR_سائز = 24;
    حرف dt[BASE64_HDR_سائز + 1] = {0};
    base64decoder.init(parser_do_not_use_براہ_راست_dereference, ptr, indent);

    صحیح i, k;

    برائے (i = 0; i < BASE64_HDR_سائز; i++)
        dt[i] = (حرف) base64decoder.getUInt8();
    برائے (i = 0; i < BASE64_HDR_سائز; i++)
        اگر (خلا_ہے(dt[i]))
            توڑو;
    dt[i] = '\0';

    CV_Assert(!base64decoder.انتہا_کا_دھارا());

    صحیح fmt_جوڑے[CV_FS_MAX_FMT_جوڑے * 2];
    صحیح fmt_pair_گنتی = fs::decodeFormat(dt, fmt_جوڑے, CV_FS_MAX_FMT_جوڑے);
    int64_t ival = 0;
    دوہرا fval = 0;

    برائے (;;) {
        برائے (k = 0; k < fmt_pair_گنتی; k++) {
            صحیح elem_type = fmt_جوڑے[k * 2 + 1];
            صحیح گنتی_2 = fmt_جوڑے[k * 2];

            برائے (i = 0; i < گنتی_2; i++) {
                صحیح گرہ_type = File_گرہ::INT;
                انتخاب (elem_type) {
                    حالت CV_8U:
                        ival = base64decoder.getUInt8();
                        توڑو;
                    حالت CV_8S:
                        ival = (حرف) base64decoder.getUInt8();
                        توڑو;
                    حالت CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        توڑو;
                    حالت CV_16U:
                        ival = base64decoder.getUInt16();
                        توڑو;
                    حالت CV_16S:
                        ival = (مختصر) base64decoder.getUInt16();
                        توڑو;
                    حالت CV_32U:
                        ival = base64decoder.getInt32();
                        توڑو;
                    حالت CV_32S:
                        ival = base64decoder.getInt32();
                        توڑو;
                    حالت CV_64U:
                        ival = base64decoder.getInt64();
                        توڑو;
                    حالت CV_64S:
                        ival = base64decoder.getInt64();
                        توڑو;
                    حالت CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        گرہ_type = File_گرہ::REAL;
                    }
                        توڑو;
                    حالت CV_64F:
                        fval = base64decoder.getFloat64();
                        گرہ_type = File_گرہ::REAL;
                        توڑو;
                    حالت CV_16F:
                        fval = اعشاری(hfloat_From_بٹ(base64decoder.getUInt16()));
                        گرہ_type = File_گرہ::REAL;
                        توڑو;
                    حالت CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        گرہ_type = File_گرہ::REAL;
                        توڑو;
                    حالت CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        گرہ_type = File_گرہ::REAL;
                        توڑو;
                    طےشدہ:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                اگر (base64decoder.انتہا_کا_دھارا())
                    توڑو;
                جوڑو_گرہ(collection, std::string(), گرہ_type,
                        گرہ_type == File_گرہ::INT ? (خالی *) &ival : (خالی *) &fval, -1);
            }
        }
        اگر (base64decoder.انتہا_کا_دھارا())
            توڑو;
    }

    finalizeCollection(collection);
    واپس base64decoder.getPtr();
}

خالی FileStorage::Impl::parseError(ثابت حرف *func_name, ثابت std::string &err_پیغام, ثابت حرف *ماخذ_file,
                                   صحیح ماخذ_سطر) {
    std::string پیغام = format("%s(%d): %s", filename.c_str(), lineno, err_پیغام.c_str());
    error(Error::StsParseError, func_name, پیغام.c_str(), ماخذ_file, ماخذ_سطر);
}

ثابت uchar *FileStorage::Impl::get_گرہ_Ptr(size_t بلاک_Idx, size_t ofs) ثابت {
    CV_Assert(بلاک_Idx < fs_ڈیٹا_ptrs.size());
    CV_Assert(ofs < fs_ڈیٹا_blksz[بلاک_Idx]);

    واپس fs_ڈیٹا_ptrs[بلاک_Idx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) ثابت {
    CV_Assert(nameofs < str_hash_ڈیٹا.size());
    واپس std::string(&str_hash_ڈیٹا[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { واپس fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(یہ);
}

FileStorage::FileStorage(ثابت String& filename, صحیح flags, ثابت String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(یہ);
    منطقی ok = p->open(filename.c_str(), flags, encoding.c_str());
    اگر(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

خالی FileStorage::آغاز_Write_Struct(ثابت String& name, صحیح struct_flags, ثابت String& typeName)
{
    p->آغاز_Write_Struct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    اگر ((struct_flags & File_گرہ::TYPE_نقاب) == File_گرہ::SEQ)
        state = FileStorage::قدر_EXPECTED;
    ورنہ
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

خالی FileStorage::انتہا_Write_Struct()
{
    p->انتہا_Write_Struct();
    state = p->write_ڈھیر.empty() || File_گرہ::isMap(p->write_ڈھیر.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::قدر_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

منطقی FileStorage::open(ثابت String& filename, صحیح flags, ثابت String& encoding)
{
    کوشش
    {
        منطقی ok = p->open(filename.c_str(), flags, encoding.c_str());
        اگر(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        واپس ok;
    }
    پکڑو (...)
    {
        release();
        پھینکو;  // re-throw
    }
}

منطقی FileStorage::isOpened() ثابت { واپس p->is_opened; }

خالی FileStorage::release()
{
    p->release();
}

File_گرہ FileStorage::جڑ(صحیح i) ثابت
{
    اگر( p.empty() || p->roots.empty() || i < 0 || i >= (صحیح)p->roots.size() )
        واپس File_گرہ();

    واپس p->roots[i];
}

File_گرہ FileStorage::get_پہلا_چوٹی_Level_گرہ() ثابت
{
    File_گرہ r = جڑ();
    File_گرہ_Iterator it = r.begin();
    واپس it != r.end() ? *it : File_گرہ();
}

std::string FileStorage::getDefaultObjectName(ثابت std::string& _filename)
{
    ساکن ثابت حرف* stubname = "unnamed";
    ثابت حرف* filename = _filename.c_str();
    ثابت حرف* ptr2 = filename + _filename.size();
    ثابت حرف* ptr = ptr2 - 1;
    cv::Auto_بفر<حرف> name_buf(_filename.size()+1);

    جب( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        اگر( *ptr == '.' && (!*ptr2 || لڑی_موازنہ(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    اگر( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    حرف* name = name_buf.ڈیٹا();

    // name must start with letter or '_'
    اگر( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    جب( ptr < ptr2 )
    {
        حرف c = *ptr++;
        اگر( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.ڈیٹا();
    اگر( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    واپس name;
}


صحیح FileStorage::getFormat() ثابت
{
    واپس p->fmt;
}

File_گرہ FileStorage::عامل [](ثابت حرف* کلید) ثابت
{
    واپس یہ->عامل[](std::string(کلید));
}

File_گرہ FileStorage::عامل [](ثابت std::string& کلید) ثابت
{
    File_گرہ res;
    برائے (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][کلید];
        اگر (!res.empty())
            توڑو;
    }
    واپس res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    واپس buf;
}

خالی FileStorage::writeRaw( ثابت String& fmt, ثابت خالی* vec, size_t len )
{
    p->write_Raw_ڈیٹا(fmt, (ثابت uchar*)vec, len);
}

خالی FileStorage::writeComment( ثابت String& comment, منطقی eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

خالی writeScalar( FileStorage& fs, صحیح value )
{
    fs.p->اكتب(String(), value);
}

خالی writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->اكتب(String(), value);
}

خالی writeScalar( FileStorage& fs, اعشاری value )
{
    fs.p->اكتب(String(), (دوہرا)value);
}

خالی writeScalar( FileStorage& fs, دوہرا value )
{
    fs.p->اكتب(String(), value);
}

خالی writeScalar( FileStorage& fs, ثابت String& value )
{
    fs.p->اكتب(String(), value);
}

خالی اكتب( FileStorage& fs, ثابت String& name, صحیح value )
{
    fs.p->اكتب(name, value);
}

خالی اكتب( FileStorage& fs, ثابت String& name, int64_t value )
{
    fs.p->اكتب(name, value);
}

خالی اكتب( FileStorage& fs, ثابت String& name, اعشاری value )
{
    fs.p->اكتب(name, (دوہرا)value);
}

خالی اكتب( FileStorage& fs, ثابت String& name, دوہرا value )
{
    fs.p->اكتب(name, value);
}

خالی اكتب( FileStorage& fs, ثابت String& name, ثابت String& value )
{
    fs.p->اكتب(name, value);
}

خالی اكتب( FileStorage& fs, ثابت String& name, منطقی value )
{
    fs.p->اكتب(name, value);
}

خالی FileStorage::اكتب(ثابت String& name, صحیح val) { p->اكتب(name, val); }
خالی FileStorage::اكتب(ثابت String& name, منطقی val) { p->اكتب(name, val); }
خالی FileStorage::اكتب(ثابت String& name, int64_t val) { p->اكتب(name, val); }
خالی FileStorage::اكتب(ثابت String& name, دوہرا val) { p->اكتب(name, val); }
خالی FileStorage::اكتب(ثابت String& name, ثابت String& val) { p->اكتب(name, val); }
خالی FileStorage::اكتب(ثابت String& name, ثابت Mat& val) { cv::اكتب(*یہ, name, val); }
خالی FileStorage::اكتب(ثابت String& name, ثابت std::vector<String>& val) { cv::اكتب(*یہ, name, val); }

FileStorage& عامل << (FileStorage& fs, ثابت String& str)
{
    شمار { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        قدر_EXPECTED = FileStorage::قدر_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    ثابت حرف* _str = str.c_str();
    اگر( !fs.isOpened() || !_str )
        واپس fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    حرف c = *_str;

    اگر( c == '}' || c == ']' )
    {
        اگر( fs_impl->write_ڈھیر.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        صحیح struct_flags = fs_impl->write_ڈھیر.back().flags;
        حرف expected_bracket = File_گرہ::isMap(struct_flags) ? '}' : ']';
        اگر( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->انتہا_Write_Struct();
        CV_Assert(!fs_impl->write_ڈھیر.empty());
        struct_flags = fs_impl->write_ڈھیر.back().flags;
        fs.state = File_گرہ::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : قدر_EXPECTED;
        fs.elname = String();
    }
    ورنہ اگر( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        اگر (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = قدر_EXPECTED + INSIDE_MAP;
    }
    ورنہ اگر( (fs.state & 3) == قدر_EXPECTED )
    {
        اگر( c == '{' || c == '[' )
        {
            صحیح struct_flags = c == '{' ? File_گرہ::MAP : File_گرہ::SEQ;
            fs.state = struct_flags == File_گرہ::MAP ? INSIDE_MAP + NAME_EXPECTED : قدر_EXPECTED;
            _str++;
            اگر( *_str == ':' )
            {
                _str++;
                اگر( !*_str )
                    struct_flags |= File_گرہ::FLOW;
            }
            fs_impl->آغاز_Write_Struct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        ورنہ
        {
            اكتب( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            اگر( fs.state == INSIDE_MAP + قدر_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    ورنہ
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    واپس fs;
}


File_گرہ::File_گرہ()
    : fs(تہی)
{
    بلاک_Idx = ofs = 0;
}

File_گرہ::File_گرہ(FileStorage::Impl* _fs, size_t بلاک_Idx_2, size_t _ofs)
    : fs(_fs)
{
    بلاک_Idx = بلاک_Idx_2;
    ofs = _ofs;
}

File_گرہ::File_گرہ(ثابت FileStorage* _fs, size_t بلاک_Idx_2, size_t _ofs)
    : File_گرہ(_fs->p.get(), بلاک_Idx_2, _ofs)
{
    // nothing
}

File_گرہ::File_گرہ(ثابت File_گرہ& گرہ)
{
    fs = گرہ.fs;
    بلاک_Idx = گرہ.بلاک_Idx;
    ofs = گرہ.ofs;
}

File_گرہ& File_گرہ::عامل=(ثابت File_گرہ& گرہ)
{
    fs = گرہ.fs;
    بلاک_Idx = گرہ.بلاک_Idx;
    ofs = گرہ.ofs;
    واپس *یہ;
}

File_گرہ File_گرہ::عامل[](ثابت std::string& nodename) ثابت
{
    اگر(!fs)
        واپس File_گرہ();

    CV_Assert( isMap() );

    غیرعلامتی کلید = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_گرہ_Iterator it = begin();

    برائے( i = 0; i < sz; i++, ++it )
    {
        File_گرہ n = *it;
        ثابت uchar* p = n.ptr();
        غیرعلامتی key2 = (غیرعلامتی)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_ڈیٹا.size() );
        اگر( کلید == key2 )
            واپس n;
    }
    واپس File_گرہ();
}

File_گرہ File_گرہ::عامل[](ثابت حرف* nodename) ثابت
{
    واپس یہ->عامل[](std::string(nodename));
}

File_گرہ File_گرہ::عامل[](صحیح i) ثابت
{
    اگر(!fs)
        واپس File_گرہ();

    CV_Assert( isSeq() );

    صحیح sz = (صحیح)size();
    CV_Assert( 0 <= i && i < sz );

    File_گرہ_Iterator it = begin();
    it += i;

    واپس *it;
}

std::vector<String> File_گرہ::کلیدیں() ثابت
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    برائے (File_گرہ_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    واپس res;
}

صحیح File_گرہ::type() ثابت
{
    ثابت uchar* p = ptr();
    اگر(!p)
        واپس NONE;
    واپس (*p & TYPE_نقاب);
}

منطقی File_گرہ::isMap(صحیح flags) { واپس (flags & TYPE_نقاب) == MAP; }
منطقی File_گرہ::isSeq(صحیح flags) { واپس (flags & TYPE_نقاب) == SEQ; }
منطقی File_گرہ::isCollection(صحیح flags) { واپس isMap(flags) || isSeq(flags); }
منطقی File_گرہ::isFlow(صحیح flags) { واپس (flags & FLOW) != 0; }
منطقی File_گرہ::is_خالی_خانے_Collection(صحیح flags) { واپس (flags & خالی_خانے_2) != 0; }

منطقی File_گرہ::empty() ثابت   { واپس fs == 0; }
منطقی File_گرہ::isNone() ثابت  { واپس type() == NONE; }
منطقی File_گرہ::isSeq() ثابت   { واپس type() == SEQ; }
منطقی File_گرہ::isMap() ثابت   { واپس type() == MAP; }
منطقی File_گرہ::isInt() ثابت   { واپس type() == INT;  }
منطقی File_گرہ::isReal() ثابت  { واپس type() == REAL; }
منطقی File_گرہ::isString() ثابت { واپس type() == STRING;  }
منطقی File_گرہ::isNamed() ثابت
{
    ثابت uchar* p = ptr();
    اگر(!p)
        واپس جھوٹ;
    واپس (*p & NAMED) != 0;
}

std::string File_گرہ::name() ثابت
{
    ثابت uchar* p = ptr();
    اگر(!p)
        واپس std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    واپس fs->getName(nameofs);
}

File_گرہ::عامل صحیح() ثابت
{
    ثابت uchar* p = ptr();
    اگر(!p)
        واپس 0;
    صحیح لیبل = *p;
    صحیح type = (لیبل & TYPE_نقاب);
    p += (لیبل & NAMED) ? 5 : 1;

    اگر( type == INT )
    {
        واپس readInt(p);
    }
    ورنہ اگر( type == REAL )
    {
        واپس cv_باری(readReal(p));
    }
    ورنہ
        واپس 0x7fffffff;
}

File_گرہ::عامل int64_t() ثابت
{
    ثابت uchar* p = ptr();
    اگر(!p)
        واپس 0;
    صحیح لیبل = *p;
    صحیح type = (لیبل & TYPE_نقاب);
    p += (لیبل & NAMED) ? 5 : 1;

    اگر( type == INT )
    {
        واپس readLong(p);
    }
    ورنہ اگر( type == REAL )
    {
        واپس cv_باری(readReal(p));
    }
    ورنہ
        واپس 0x7fffffff;
}

File_گرہ::عامل اعشاری() ثابت
{
    ثابت uchar* p = ptr();
    اگر(!p)
        واپس 0.f;
    صحیح لیبل = *p;
    صحیح type = (لیبل & TYPE_نقاب);
    p += (لیبل & NAMED) ? 5 : 1;

    اگر( type == INT )
    {
        واپس (اعشاری)readLong(p);
    }
    ورنہ اگر( type == REAL )
    {
        واپس (اعشاری)readReal(p);
    }
    ورنہ
        واپس FLT_MAX;
}

File_گرہ::عامل دوہرا() ثابت
{
    ثابت uchar* p = ptr();
    اگر(!p)
        واپس 0.f;
    صحیح لیبل = *p;
    صحیح type = (لیبل & TYPE_نقاب);
    p += (لیبل & NAMED) ? 5 : 1;

    اگر( type == INT )
    {
        واپس (دوہرا)readLong(p);
    }
    ورنہ اگر( type == REAL )
    {
        واپس readReal(p);
    }
    ورنہ
        واپس DBL_MAX;
}

دوہرا File_گرہ::real() ثابت  { واپس دوہرا(*یہ); }

std::string File_گرہ::string() ثابت
{
    ثابت uchar* p = ptr();
    اگر( !p || (*p & TYPE_نقاب) != STRING )
        واپس std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(غیرعلامتی)readInt(p);
    واپس std::string((ثابت حرف*)(p + 4), sz - 1);
}
Mat File_گرہ::mat() ثابت { Mat value; اقرأ(*یہ, value, Mat()); واپس value; }

File_گرہ_Iterator File_گرہ::begin() ثابت { واپس File_گرہ_Iterator(*یہ, جھوٹ); }
File_گرہ_Iterator File_گرہ::end() ثابت   { واپس File_گرہ_Iterator(*یہ, سچ); }

خالی File_گرہ::readRaw( ثابت std::string& fmt, خالی* vec, size_t len ) ثابت
{
    File_گرہ_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_گرہ::size() ثابت
{
    ثابت uchar* p = ptr();
    اگر( !p )
        واپس 0;
    صحیح لیبل = *p;
    صحیح tp = لیبل & TYPE_نقاب;
    اگر( tp == MAP || tp == SEQ )
    {
        اگر( لیبل & NAMED )
            p += 4;
        واپس (size_t)(غیرعلامتی)readInt(p + 5);
    }
    واپس tp != NONE;
}

size_t File_گرہ::raw_سائز() ثابت
{
    ثابت uchar* p0 = ptr(), *p = p0;
    اگر( !p )
        واپس 0;
    صحیح لیبل = *p++;
    صحیح tp = لیبل & TYPE_نقاب;
    اگر( لیبل & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    اگر( tp == INT )
        واپس sz0 + 8;
    اگر( tp == REAL )
        واپس sz0 + 8;
    اگر( tp == NONE )
        واپس sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    واپس sz0 + 4 + readInt(p);
}

uchar* File_گرہ::ptr()
{
    واپس !fs ? 0 : (uchar*)fs->get_گرہ_Ptr(بلاک_Idx, ofs);
}

ثابت uchar* File_گرہ::ptr() ثابت
{
    واپس !fs ? 0 : fs->get_گرہ_Ptr(بلاک_Idx, ofs);
}

خالی File_گرہ::سیٹ_قدر( صحیح type, ثابت خالی* value, صحیح len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    صحیح لیبل = *p;
    صحیح current_type = لیبل & TYPE_نقاب;
    CV_Assert( current_type == NONE || current_type == type );

    صحیح sz = 1;

    اگر( لیبل & NAMED )
        sz += 4;

    اگر( type == INT )
        sz += 8;
    ورنہ اگر( type == REAL )
        sz += 8;
    ورنہ اگر( type == STRING )
    {
        اگر( len < 0 )
            len = (صحیح)لڑی_لمبائی((ثابت حرف*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    ورنہ
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_گرہ_Space(*یہ, sz);
    *p++ = (uchar)(type | (لیبل & NAMED));
    اگر( لیبل & NAMED )
        p += 4;

    اگر( type == INT )
    {
        int64_t ival = *(ثابت int64_t*)value;
        writeInt(p, ival);
    }
    ورنہ اگر( type == REAL )
    {
        دوہرا dbval = *(ثابت دوہرا*)value;
        writeReal(p, dbval);
    }
    ورنہ اگر( type == STRING )
    {
        ثابت حرف* str = (ثابت حرف*)value;
        writeInt(p, len + 1);
        حافظہ_نقل(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_گرہ_Iterator::File_گرہ_Iterator()
{
    fs = 0;
    بلاک_Idx = 0;
    ofs = 0;
    بلاک_سائز_2 = 0;
    گرہ_NElems = 0;
    idx = 0;
}

File_گرہ_Iterator::File_گرہ_Iterator( ثابت File_گرہ& گرہ, منطقی seek_انتہا )
{
    fs = گرہ.fs;
    idx = 0;
    اگر( !fs )
        بلاک_Idx = ofs = بلاک_سائز_2 = گرہ_NElems = 0;
    ورنہ
    {
        بلاک_Idx = گرہ.بلاک_Idx;
        ofs = گرہ.ofs;

        منطقی collection = گرہ.isSeq() || گرہ.isMap();
        اگر( گرہ.isNone() )
        {
            گرہ_NElems = 0;
        }
        ورنہ اگر( !collection )
        {
            گرہ_NElems = 1;
            اگر( seek_انتہا )
            {
                idx = 1;
                ofs += گرہ.raw_سائز();
            }
        }
        ورنہ
        {
            گرہ_NElems = گرہ.size();
            ثابت uchar* p0 = گرہ.ptr(), *p = p0 + 1;
            اگر(*p0 & File_گرہ::NAMED )
                p += 4;
            اگر( !seek_انتہا )
                ofs += (p - p0) + 8;
            ورنہ
            {
                size_t rawsz = (size_t)(غیرعلامتی)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = گرہ_NElems;
            }
        }
        fs->normalize_گرہ_Ofs(بلاک_Idx, ofs);
        بلاک_سائز_2 = fs->fs_ڈیٹا_blksz[بلاک_Idx];
    }
}

File_گرہ_Iterator::File_گرہ_Iterator(ثابت File_گرہ_Iterator& it)
{
    fs = it.fs;
    بلاک_Idx = it.بلاک_Idx;
    ofs = it.ofs;
    بلاک_سائز_2 = it.بلاک_سائز_2;
    گرہ_NElems = it.گرہ_NElems;
    idx = it.idx;
}

File_گرہ_Iterator& File_گرہ_Iterator::عامل=(ثابت File_گرہ_Iterator& it)
{
    fs = it.fs;
    بلاک_Idx = it.بلاک_Idx;
    ofs = it.ofs;
    بلاک_سائز_2 = it.بلاک_سائز_2;
    گرہ_NElems = it.گرہ_NElems;
    idx = it.idx;
    واپس *یہ;
}

File_گرہ File_گرہ_Iterator::عامل *() ثابت
{
    واپس File_گرہ(idx < گرہ_NElems ? fs : تہی, بلاک_Idx, ofs);
}

File_گرہ_Iterator& File_گرہ_Iterator::عامل ++ ()
{
    اگر( idx == گرہ_NElems || !fs )
        واپس *یہ;
    idx++;
    File_گرہ n(fs, بلاک_Idx, ofs);
    ofs += n.raw_سائز();
    اگر( ofs >= بلاک_سائز_2 )
    {
        fs->normalize_گرہ_Ofs(بلاک_Idx, ofs);
        بلاک_سائز_2 = fs->fs_ڈیٹا_blksz[بلاک_Idx];
    }
    واپس *یہ;
}

File_گرہ_Iterator File_گرہ_Iterator::عامل ++ (صحیح)
{
    File_گرہ_Iterator it = *یہ;
    ++(*یہ);
    واپس it;
}

File_گرہ_Iterator& File_گرہ_Iterator::عامل += (صحیح _ofs)
{
    CV_Assert( _ofs >= 0 );
    برائے( ; _ofs > 0; _ofs-- )
        یہ->عامل ++();
    واپس *یہ;
}

File_گرہ_Iterator& File_گرہ_Iterator::readRaw( ثابت String& fmt, خالی* _data0, size_t maxsz)
{
    اگر( fs && idx < گرہ_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        صحیح fmt_جوڑے[CV_FS_MAX_FMT_جوڑے*2];
        صحیح fmt_pair_گنتی = fs::decodeFormat( fmt.c_str(), fmt_جوڑے, CV_FS_MAX_FMT_جوڑے );
        size_t esz = fs::calc_Struct_سائز( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        برائے( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            برائے( صحیح k = 0; k < fmt_pair_گنتی; k++ )
            {
                صحیح elem_type = fmt_جوڑے[k*2+1];
                صحیح elem_سائز_2 = CV_ELEM_سائز(elem_type);

                صحیح گنتی_2 = fmt_جوڑے[k*2];
                offset = align_سائز( offset, elem_سائز_2 );
                uchar* ڈیٹا = data0 + offset;

                برائے( صحیح i = 0; i < گنتی_2; i++, ++(*یہ) )
                {
                    File_گرہ گرہ = *(*یہ);
                    اگر( گرہ.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_سائز_2 == 8 ? (int64_t)گرہ : (صحیح)گرہ);
                        انتخاب( elem_type )
                        {
                        حالت CV_8U:
                            *(uchar*)ڈیٹا = saturate_cast<uchar>(ival);
                            ڈیٹا++;
                            توڑو;
                        حالت CV_8S:
                            *(حرف*)ڈیٹا = saturate_cast<schar>(ival);
                            ڈیٹا++;
                            توڑو;
                        حالت CV_Bool:
                            *(منطقی*)ڈیٹا = ival != 0;
                            ڈیٹا++;
                            توڑو;
                        حالت CV_16U:
                            *(ushort*)ڈیٹا = saturate_cast<ushort>(ival);
                            ڈیٹا += حجم(ushort);
                            توڑو;
                        حالت CV_16S:
                            *(مختصر*)ڈیٹا = saturate_cast<مختصر>(ival);
                            ڈیٹا += حجم(مختصر);
                            توڑو;
                        حالت CV_32U:
                            *(غیرعلامتی*)ڈیٹا = (غیرعلامتی)std::max(ival, (int64_t)0);
                            ڈیٹا += حجم(غیرعلامتی);
                            توڑو;
                        حالت CV_32S:
                            *(صحیح*)ڈیٹا = (صحیح)ival;
                            ڈیٹا += حجم(صحیح);
                            توڑو;
                        حالت CV_32F:
                            *(اعشاری*)ڈیٹا = (اعشاری)ival;
                            ڈیٹا += حجم(اعشاری);
                            توڑو;
                        حالت CV_64U:
                            *(uint64_t*)ڈیٹا = (uint64_t)ival;
                            ڈیٹا += حجم(uint64_t);
                            توڑو;
                        حالت CV_64S:
                            *(int64_t*)ڈیٹا = (int64_t)ival;
                            ڈیٹا += حجم(int64_t);
                            توڑو;
                        حالت CV_64F:
                            *(دوہرا*)ڈیٹا = (دوہرا)ival;
                            ڈیٹا += حجم(دوہرا);
                            توڑو;
                        حالت CV_16F:
                            *(hfloat*)ڈیٹا = hfloat((اعشاری)ival);
                            ڈیٹا += حجم(hfloat);
                            توڑو;
                        حالت CV_16BF:
                            *(bfloat*)ڈیٹا = bfloat((اعشاری)ival);
                            ڈیٹا += حجم(bfloat);
                            توڑو;
                        حالت CV_8F_E4M3FN:
                            *(fp8_t*)ڈیٹا = fp8_t((اعشاری)ival);
                            ڈیٹا += حجم(fp8_t);
                            توڑو;
                        حالت CV_8F_E4M3FNUZ:
                            *(fp8a_t*)ڈیٹا = fp8a_t((اعشاری)ival);
                            ڈیٹا += حجم(fp8a_t);
                            توڑو;
                        طےشدہ:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    ورنہ اگر( گرہ.isReal() )
                    {
                        دوہرا fval = (دوہرا)گرہ;

                        انتخاب( elem_type )
                        {
                        حالت CV_8U:
                            *(uchar*)ڈیٹا = saturate_cast<uchar>(fval);
                            ڈیٹا++;
                            توڑو;
                        حالت CV_8S:
                            *(حرف*)ڈیٹا = saturate_cast<schar>(fval);
                            ڈیٹا++;
                            توڑو;
                        حالت CV_16U:
                            *(ushort*)ڈیٹا = saturate_cast<ushort>(fval);
                            ڈیٹا += حجم(ushort);
                            توڑو;
                        حالت CV_16S:
                            *(مختصر*)ڈیٹا = saturate_cast<مختصر>(fval);
                            ڈیٹا += حجم(مختصر);
                            توڑو;
                        حالت CV_32U:
                            *(صحیح*)ڈیٹا = saturate_cast<غیرعلامتی>(fval);
                            ڈیٹا += حجم(صحیح);
                            توڑو;
                        حالت CV_32S:
                            *(صحیح*)ڈیٹا = saturate_cast<صحیح>(fval);
                            ڈیٹا += حجم(صحیح);
                            توڑو;
                        حالت CV_32F:
                            *(اعشاری*)ڈیٹا = (اعشاری)fval;
                            ڈیٹا += حجم(اعشاری);
                            توڑو;
                        حالت CV_64U:
                            *(uint64_t*)ڈیٹا = (uint64_t)round(std::max(fval, 0.));
                            ڈیٹا += حجم(uint64_t);
                            توڑو;
                        حالت CV_64S:
                            *(int64_t*)ڈیٹا = (int64_t)round(std::max(fval, 0.));
                            ڈیٹا += حجم(int64_t);
                            توڑو;
                        حالت CV_64F:
                            *(دوہرا*)ڈیٹا = fval;
                            ڈیٹا += حجم(دوہرا);
                            توڑو;
                        حالت CV_16F:
                            *(hfloat*)ڈیٹا = hfloat((اعشاری)fval);
                            ڈیٹا += حجم(hfloat);
                            توڑو;
                        حالت CV_16BF:
                            *(bfloat*)ڈیٹا = bfloat((اعشاری)fval);
                            ڈیٹا += حجم(bfloat);
                            توڑو;
                        حالت CV_8F_E4M3FN:
                            *(fp8_t*)ڈیٹا = fp8_t((اعشاری)fval);
                            ڈیٹا += حجم(fp8_t);
                            توڑو;
                        حالت CV_8F_E4M3FNUZ:
                            *(fp8a_t*)ڈیٹا = fp8a_t((اعشاری)fval);
                            ڈیٹا += حجم(fp8a_t);
                            توڑو;
                        طےشدہ:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    ورنہ
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (صحیح)(ڈیٹا - data0);
            }
        }
    }

    واپس *یہ;
}

منطقی File_گرہ_Iterator::equal_مقصد(ثابت File_گرہ_Iterator& it) ثابت
{
    واپس fs == it.fs && بلاک_Idx == it.بلاک_Idx && ofs == it.ofs &&
           idx == it.idx && گرہ_NElems == it.گرہ_NElems;
}

size_t File_گرہ_Iterator::بقایا() ثابت
{
    واپس گرہ_NElems - idx;
}

منطقی عامل == ( ثابت File_گرہ_Iterator& it1, ثابت File_گرہ_Iterator& it2 )
{
    واپس it1.equal_مقصد(it2);
}

منطقی عامل != ( ثابت File_گرہ_Iterator& it1, ثابت File_گرہ_Iterator& it2 )
{
    واپس !it1.equal_مقصد(it2);
}

خالی اقرأ(ثابت File_گرہ& گرہ, صحیح& val, صحیح default_val)
{
    val = default_val;
    اگر( !گرہ.empty() )
    {
        val = (صحیح)گرہ;
    }
}

خالی اقرأ(ثابت File_گرہ& گرہ, int64_t& val, int64_t default_val)
{
    val = default_val;
    اگر( !گرہ.empty() )
    {
        val = (int64_t)گرہ;
    }
}

خالی اقرأ(ثابت File_گرہ& گرہ, دوہرا& val, دوہرا default_val)
{
    val = default_val;
    اگر( !گرہ.empty() )
    {
        val = (دوہرا)گرہ;
    }
}

خالی اقرأ(ثابت File_گرہ& گرہ, اعشاری& val, اعشاری default_val)
{
    val = default_val;
    اگر( !گرہ.empty() )
    {
        val = (اعشاری)گرہ;
    }
}

خالی اقرأ(ثابت File_گرہ& گرہ, std::string& val, ثابت std::string& default_val)
{
    val = default_val;
    اگر( !گرہ.empty() )
    {
        val = (std::string)گرہ;
    }
}
خالی FileStorage::Impl::اكتب(ثابت String &کلید, منطقی value)
{
    CV_Assert(write_منوال);
    اگر (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().اكتب(کلید.c_str(), (صحیح)value);
    }
    ورنہ
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().اكتب(کلید.c_str(), value ? "true" : "false", جھوٹ);
    }
}

FileStorage_API::~FileStorage_API() {}

نامخلا internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, ثابت std::string& name,
                                       صحیح flags, ثابت std::string& typeName)
{
    fs = &_fs;
    fs->آغاز_Write_Struct(name, flags, typeName);
}

WriteStructContext::~WriteStructContext()
{
    fs->انتہا_Write_Struct();
}

}

}
