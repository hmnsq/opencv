// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "persistence.hpp"
#अन्तर्भाव "persistence_impl.hpp"
#अन्तर्भाव "persistence_base64_encoding.hpp"
#अन्तर्भाव <unordered_map>
#अन्तर्भाव <iterator>

#अन्तर्भाव <opencv2/core/utils/logger.hpp>

#परिभाषय CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

नामाकाश cv
{

नामाकाश fs
{

पूर्णाङ्क strcasecmp(ध्रुव वर्ण* s1, ध्रुव वर्ण* s2)
{
    ध्रुव वर्ण* dummy="";
    यदि(!s1) s1=dummy;
    यदि(!s2) s2=dummy;

    size_t len1 = वाक्यदैर्घ्यम्(s1);
    size_t len2 = वाक्यदैर्घ्यम्(s2);
    size_t i, len = std::min(len1, len2);
    प्रति( i = 0; i < len; i++ )
    {
        पूर्णाङ्क d = tolower((पूर्णाङ्क)s1[i]) - tolower((पूर्णाङ्क)s2[i]);
        यदि( d != 0 )
            प्रत्यागम d;
    }
    प्रत्यागम len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

वर्ण* itoa( पूर्णाङ्क _val, वर्ण* बफर, पूर्णाङ्क /*radix*/ )
{
    ध्रुव पूर्णाङ्क radix = 10;
    वर्ण* ptr=बफर + 23 /* enough even for 64-bit integers */;
    अचिह्नित val = निरपेक्षम्(_val);

    *ptr = '\0';
    कुरु
    {
        अचिह्नित r = val / radix;
        *--ptr = (वर्ण)(val - (r*radix) + '0');
        val = r;
    }
    यावत्( val != 0 );

    यदि( _val < 0 )
        *--ptr = '-';

    प्रत्यागम ptr;
}

वर्ण* itoa( int64_t _val, वर्ण* बफर, पूर्णाङ्क /*radix*/, तार्किक _signed)
{
    ध्रुव पूर्णाङ्क radix = 10;
    वर्ण* ptr=बफर + 23 /* enough even for 64-bit integers */;
    पूर्णाङ्क sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : निरपेक्षम्(_val);

    *ptr = '\0';
    कुरु
    {
        uint64_t r = val / radix;
        *--ptr = (वर्ण)(val - (r*radix) + '0');
        val = r;
    }
    यावत्( val != 0 );

    यदि( sign < 0 )
        *--ptr = '-';

    प्रत्यागम ptr;
}

वर्ण* double_गन्तव्यम्_String( वर्ण* buf, size_t buf_आकार, द्विगुण value, तार्किक explicitZero )
{
    Cv64suf val;
    अचिह्नित ieee754_hi;

    val.f = value;
    ieee754_hi = (अचिह्नित)(val.u >> 32);

    यदि( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        पूर्णाङ्क ivalue = cv_आवर्तनम्(value);
        यदि( ivalue == value )
        {
            यदि( explicitZero )
                snprintf( buf, buf_आकार, "%d.0", ivalue );
            अन्यथा
                snprintf( buf, buf_आकार, "%d.", ivalue );
        }
        अन्यथा
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, buf_आकार, "%.17g", value );

            वर्ण* ptr = buf;
            यदि( *ptr == '+' || *ptr == '-' )
                ptr++;
            प्रति( ; cv_isdigit(*ptr); ptr++ )
                ;
            यदि( *ptr == ',' )
                *ptr = '.';
        }
    }
    अन्यथा
    {
        अचिह्नित ieee754_lo = (अचिह्नित)val.u;
        यदि( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            वाक्यं_प्रतिलिख( buf, ".Nan" );
        अन्यथा
            वाक्यं_प्रतिलिख( buf, (पूर्णाङ्क)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    प्रत्यागम buf;
}

वर्ण* float_गन्तव्यम्_String( वर्ण* buf, size_t buf_आकार, भिन्न value, तार्किक halfprecision, तार्किक explicitZero )
{
    Cv32suf val;
    अचिह्नित ieee754;
    val.f = value;
    ieee754 = val.u;

    यदि( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        पूर्णाङ्क ivalue = cv_आवर्तनम्(value);
        यदि( ivalue == value )
        {
            यदि( explicitZero )
                snprintf( buf, buf_आकार, "%d.0", ivalue );
            अन्यथा
                snprintf( buf, buf_आकार, "%d.", ivalue );
        }
        अन्यथा
        {
            यदि (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, buf_आकार, "%.5g", value);
            }
            अन्यथा
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, buf_आकार, "%.9g", value);
            }

            वर्ण* ptr = buf;
            यदि( *ptr == '+' || *ptr == '-' )
                ptr++;
            प्रति( ; cv_isdigit(*ptr); ptr++ )
                ;
            यदि( *ptr == ',' )
                *ptr = '.';
        }
    }
    अन्यथा
    {
        यदि( (ieee754 & 0x7fffffff) != 0x7f800000 )
            वाक्यं_प्रतिलिख( buf, ".Nan" );
        अन्यथा
            वाक्यं_प्रतिलिख( buf, (पूर्णाङ्क)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    प्रत्यागम buf;
}

स्थिर ध्रुव वर्ण symbols[] = "ucwsifdhHbUIneE";

स्थिर वर्ण type_चिह्नम्(पूर्णाङ्क गभीरता)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_Check_गभीरता(गभीरता, गभीरता >= 0 && गभीरता <= CV_8F_E4M3FNUZ, "");
    प्रत्यागम symbols[गभीरता];
}

स्थिर पूर्णाङ्क चिह्नम्_गन्तव्यम्_Type(वर्ण c)
{
    यदि (c == 'r')
        प्रत्यागम CV_SEQ_ELTYPE_PTR;
    ध्रुव वर्ण* pos = strchr( symbols, c );
    यदि( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    प्रत्यागम static_cast<पूर्णाङ्क>(pos - symbols);
}

वर्ण* encodeFormat(पूर्णाङ्क elem_type, वर्ण* dt, size_t dt_len)
{
    पूर्णाङ्क cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    वर्ण चिह्नम् = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_चिह्नम्(CV_MAT_गभीरता(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, चिह्नम्);
    प्रत्यागम dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
वर्ण* encodeFormat(पूर्णाङ्क elem_type, वर्ण* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    प्रत्यागम encodeFormat(elem_type, dt, max);
}

पूर्णाङ्क decodeFormat( ध्रुव वर्ण* dt, पूर्णाङ्क* fmt_युग्मानि, पूर्णाङ्क max_len )
{
    पूर्णाङ्क fmt_pair_गणनम् = 0;
    पूर्णाङ्क i = 0, k = 0, len = dt ? (पूर्णाङ्क)वाक्यदैर्घ्यम्(dt) : 0;

    यदि( !dt || !len )
        प्रत्यागम 0;

    CV_Assert( fmt_युग्मानि != 0 && max_len > 0 );
    fmt_युग्मानि[0] = 0;
    max_len *= 2;

    प्रति( ; k < len; k++ )
    {
        वर्ण c = dt[k];

        यदि( cv_isdigit(c) )
        {
            पूर्णाङ्क गणनम्_2 = c - '0';
            यदि( cv_isdigit(dt[k+1]) )
            {
                वर्ण* endptr = 0;
                गणनम्_2 = (पूर्णाङ्क)strtol( dt+k, &endptr, 10 );
                k = (पूर्णाङ्क)(endptr - dt) - 1;
            }

            यदि( गणनम्_2 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_युग्मानि[i] = गणनम्_2;
        }
        अन्यथा
        {
            पूर्णाङ्क गभीरता = चिह्नम्_गन्तव्यम्_Type(c);
            यदि( fmt_युग्मानि[i] == 0 )
                fmt_युग्मानि[i] = 1;
            fmt_युग्मानि[i+1] = गभीरता;
            यदि( i > 0 && fmt_युग्मानि[i+1] == fmt_युग्मानि[i-1] )
                fmt_युग्मानि[i-2] += fmt_युग्मानि[i];
            अन्यथा
            {
                i += 2;
                यदि( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_युग्मानि[i] = 0;
        }
    }

    fmt_pair_गणनम् = i/2;
    प्रत्यागम fmt_pair_गणनम्;
}

पूर्णाङ्क calc_Elem_आकार( ध्रुव वर्ण* dt, पूर्णाङ्क initial_आकार )
{
    पूर्णाङ्क size = 0;
    पूर्णाङ्क fmt_युग्मानि[CV_FS_MAX_FMT_युग्मानि*2], i, fmt_pair_गणनम्;
    पूर्णाङ्क comp_आकार;

    fmt_pair_गणनम् = decodeFormat( dt, fmt_युग्मानि, CV_FS_MAX_FMT_युग्मानि );
    fmt_pair_गणनम् *= 2;
    प्रति( i = 0, size = initial_आकार; i < fmt_pair_गणनम्; i += 2 )
    {
        comp_आकार = CV_ELEM_आकार(fmt_युग्मानि[i+1]);
        size = cvAlign( size, comp_आकार );
        size += comp_आकार * fmt_युग्मानि[i];
    }
    यदि( initial_आकार == 0 )
    {
        comp_आकार = CV_ELEM_आकार(fmt_युग्मानि[1]);
        size = cvAlign( size, comp_आकार );
    }
    प्रत्यागम size;
}


पूर्णाङ्क calc_Struct_आकार( ध्रुव वर्ण* dt, पूर्णाङ्क initial_आकार )
{
    पूर्णाङ्क size = calc_Elem_आकार( dt, initial_आकार );
    size_t elem_max_आकार = 0;
    प्रति ( ध्रुव वर्ण * type = dt; *type != '\0'; type++ )
    {
        वर्ण v = *type;
        यदि (v >= '0' && v <= '9')
            अनुवर्त;  // skip vector size
        विकल्प (v)
        {
        पक्ष 'u': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(uchar ) ); भङ्ग; }
        पक्ष 'b': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(तार्किक  ) ); भङ्ग; }
        पक्ष 'c': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(schar ) ); भङ्ग; }
        पक्ष 'w': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(ushort) ); भङ्ग; }
        पक्ष 's': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(ह्रस्व ) ); भङ्ग; }
        पक्ष 'i': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(पूर्णाङ्क   ) ); भङ्ग; }
        पक्ष 'n': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(अचिह्नित) ); भङ्ग; }
        पक्ष 'f': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(भिन्न ) ); भङ्ग; }
        पक्ष 'd': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(द्विगुण) ); भङ्ग; }
        पक्ष 'h': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(hfloat)); भङ्ग; }
        पक्ष 'H': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(bfloat)); भङ्ग; }
        पक्ष 'e': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(fp8_t) ); भङ्ग; }
        पक्ष 'E': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(fp8a_t)); भङ्ग; }
        पक्ष 'I': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(int64_t)); भङ्ग; }
        पक्ष 'U': { elem_max_आकार = std::max( elem_max_आकार, परिमाण(uint64_t)); भङ्ग; }
        स्वभाव:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (वर्ण)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<पूर्णाङ्क>(elem_max_आकार) );
    प्रत्यागम size;
}

पूर्णाङ्क decodeSimpleFormat( ध्रुव वर्ण* dt )
{
    पूर्णाङ्क elem_type = -1;
    पूर्णाङ्क fmt_युग्मानि[CV_FS_MAX_FMT_युग्मानि*2], fmt_pair_गणनम्;

    fmt_pair_गणनम् = decodeFormat( dt, fmt_युग्मानि, CV_FS_MAX_FMT_युग्मानि );
    यदि( fmt_pair_गणनम् != 1 || fmt_युग्मानि[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_युग्मानि[1], fmt_युग्मानि[0] );

    प्रत्यागम elem_type;
}

}

#यदि defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#परिभाषय CV_LITTLE_ENDIAN_MEM_ACCESS 1
#अन्यथा
#परिभाषय CV_LITTLE_ENDIAN_MEM_ACCESS 0
#यदि_अन्तः

स्थिर अन्तःस्थ पूर्णाङ्क readInt(ध्रुव uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#यदि CV_LITTLE_ENDIAN_MEM_ACCESS
    पूर्णाङ्क val;
    स्मृतिं_प्रतिलिख(&val, p, परिमाण(val));
    प्रत्यागम val;
#अन्यथा
    पूर्णाङ्क val = (पूर्णाङ्क)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    प्रत्यागम val;
#यदि_अन्तः
}

स्थिर अन्तःस्थ int64_t readLong(ध्रुव uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#यदि CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    स्मृतिं_प्रतिलिख(&val, p, परिमाण(val));
    प्रत्यागम val;
#अन्यथा
    अचिह्नित val0 = (अचिह्नित)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    अचिह्नित val1 = (अचिह्नित)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    प्रत्यागम val0 | ((int64_t)val1 << 32);
#यदि_अन्तः
}

स्थिर अन्तःस्थ द्विगुण readReal(ध्रुव uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#यदि CV_LITTLE_ENDIAN_MEM_ACCESS
    द्विगुण val;
    स्मृतिं_प्रतिलिख(&val, p, परिमाण(val));
    प्रत्यागम val;
#अन्यथा
    अचिह्नित val0 = (अचिह्नित)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    अचिह्नित val1 = (अचिह्नित)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    प्रत्यागम val.f;
#यदि_अन्तः
}

प्रतिरूपम् <प्रकारनाम T>
स्थिर अन्तःस्थ शून्य writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#यदि CV_LITTLE_ENDIAN_MEM_ACCESS
    स्मृतिं_प्रतिलिख(p, &ival, परिमाण(ival));
#अन्यथा
    प्रति (size_t i = 0, j = 0; i < परिमाण(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#यदि_अन्तः
}

स्थिर अन्तःस्थ शून्य writeReal(uchar* p, द्विगुण fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#यदि CV_LITTLE_ENDIAN_MEM_ACCESS
    स्मृतिं_प्रतिलिख(p, &fval, परिमाण(fval));
#अन्यथा
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
#यदि_अन्तः
}



शून्य FileStorage::Impl::init() {
    flags = 0;
    बफर.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = असत्य;
    state_सम्बन्धि_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = असत्य;
    delayed_struct_कुञ्चिका = शून्यसूचकः;
    delayed_struct_flags = 0;
    delayed_type_name = शून्यसूचकः;
    base64_writer = शून्यसूचकः;
    is_opened = असत्य;
    dummy_eof = असत्य;
    write_बहुलकम् = असत्य;
    mem_बहुलकम् = असत्य;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    रिक्तानि_प्रवाहः = सत्य;

    strbufv.clear();
    strbuf = 0;
    strbufsize = strbufpos = 0;
    roots.clear();

    fs_दत्त.clear();
    fs_दत्त_ptrs.clear();
    fs_दत्त_blksz.clear();
    freeSpaceOfs = 0;

    str_hash.clear();
    str_hash_दत्त.clear();
    str_hash_दत्त.resize(1);
    str_hash_दत्त[0] = '\0';

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

शून्य FileStorage::Impl::release(String *out) {
    यदि (is_opened) {
        यदि (out)
            out->clear();
        यदि (write_बहुलकम्) {
            यावत् (write_राशिः.size() > 1) {
                अन्तः_Write_Struct();
            }
            flush();
            यदि (fmt == FileStorage::FORMAT_XML)
                पङ्क्तिं_लिख("</opencv_storage>\n");
            अन्यथा यदि (fmt == FileStorage::FORMAT_JSON)
                पङ्क्तिं_लिख("}\n");
        }
        यदि (mem_बहुलकम् && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

शून्य FileStorage::Impl::analyze_file_name(ध्रुव std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    स्थिर ध्रुव वर्ण not_file_name = '\n';
    स्थिर ध्रुव वर्ण parameter_begin = '?';
    स्थिर ध्रुव वर्ण parameter_separator = '&';

    यदि (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        प्रत्यागम;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    यदि (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        प्रति (size_t param_beg = beg, param_अन्तः = beg;
             param_अन्तः < end;
             param_beg = param_अन्तः + 1) {
            param_अन्तः = file_name.find_first_of(parameter_separator, param_beg);
            यदि ((param_अन्तः == std::string::npos || param_अन्तः != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_अन्तः - param_beg));
            }
        }
    }
}

तार्किक FileStorage::Impl::उद्घाटय(ध्रुव वर्ण *filename_or_buf, पूर्णाङ्क _flags, ध्रुव वर्ण *encoding) {
    तार्किक ok = सत्य;
    release();

    तार्किक append = (_flags & 3) == FileStorage::APPEND;
    mem_बहुलकम् = (_flags & FileStorage::स्मृतिः) != 0;

    write_बहुलकम् = (_flags & 3) != 0;
    तार्किक write_base64 = (write_बहुलकम् || append) && (_flags & FileStorage::BASE64) != 0;

    तार्किक isGZ = असत्य;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        यदि (!params.empty())
            filename = params[0];

        यदि (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_बहुलकम् || append);
    }

    यदि (filename.size() == 0 && !mem_बहुलकम् && !write_बहुलकम्)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    यदि (mem_बहुलकम् && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    यदि (!mem_बहुलकम्) {
        size_t dot_idx = filename.find_last_of('.');
        वर्ण compression = '\0';

        यदि (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            यदि (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                यदि (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = सत्य;
                }

                अन्यथा यदि(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = सत्य;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        यदि (!isGZ) {
            file = fopen(filename.c_str(), !write_बहुलकम् ? "rt" : !append ? "wt" : "a+t");
            यदि (!file)
            {
                CV_LOG_ERROR(रिक्त, "Can't open file: '" << filename << "' in " << (!write_बहुलकम् ? "read" : !append ? "write" : "append") << " mode");
                प्रत्यागम असत्य;
            }
        } अन्यथा {
#यदि USE_ZLIB
            यदि (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            वर्ण बहुलकम्[] = {write_बहुलकम् ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), बहुलकम्);
            यदि (!gzfile)
            {
                CV_LOG_ERROR(रिक्त, "Can't open archive: '" << filename << "' mode=" << बहुलकम्);
                प्रत्यागम असत्य;
            }
#अन्यथा
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#यदि_अन्तः
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_दत्त.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    यदि (write_बहुलकम्) {
        fmt = flags & FileStorage::FORMAT_आवरणम्;

        यदि (mem_बहुलकम्)
            outbuf.clear();

        यदि (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            ध्रुव वर्ण *dot_pos = रिक्त;
            ध्रुव वर्ण *dot_pos2 = रिक्त;
            // like strrchr() implementation, but save two last positions simultaneously
            प्रति (ध्रुव वर्ण *pos = &filename[0]; pos[0] != 0; pos++) {
                यदि (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            यदि (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != रिक्त) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } अन्यथा यदि (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        पूर्णाङ्क buf_आकार_2 = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        यदि (append) {
            fseek(file, 0, SEEK_END);
            यदि (ftell(file) == 0)
                append = असत्य;
        }

        write_राशिः.clear();
        रिक्तानि_प्रवाहः = सत्य;
        write_राशिः.push_back(FStruct_दत्त("", File_ग्रन्थिः::MAP | File_ग्रन्थिः::रिक्तानि_2, 0));
        बफर.reserve(buf_आकार_2 + 1024);
        बफर.resize(buf_आकार_2);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_सम्बन्धि_writing_base64 = FileStorage_API::Base64State::Uncertain;

        यदि (fmt == FileStorage::FORMAT_XML) {
            size_t file_आकार_2 = file ? (size_t) ftell(file) : (size_t) 0;
            यदि (!append || file_आकार_2 == 0) {
                यदि (encoding && *encoding != '\0') {
                    यदि (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(वाक्यदैर्घ्यम्(encoding) < 1000);
                    वर्ण buf[1100];
                    snprintf(buf, परिमाण(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    पङ्क्तिं_लिख(buf);
                } अन्यथा
                    पङ्क्तिं_लिख("<?xml version=\"1.0\"?>\n");
                पङ्क्तिं_लिख("<opencv_storage>\n");
            } अन्यथा {
                पूर्णाङ्क xml_buf_आकार = 1 << 10;
                वर्ण substr[] = "</opencv_storage>";
                पूर्णाङ्क चरमः_occurrence = -1;
                xml_buf_आकार = MIN(xml_buf_आकार, पूर्णाङ्क(file_आकार_2));
                fseek(file, -xml_buf_आकार, SEEK_END);
                // find the last occurrence of </opencv_storage>
                प्रति (;;) {
                    पूर्णाङ्क पङ्क्तिः_विस्थापनम् = (पूर्णाङ्क) ftell(file);
                    ध्रुव वर्ण *ptr0 = एतत्->gets(xml_buf_आकार);
                    ध्रुव वर्ण *ptr = रिक्त;
                    यदि (!ptr0)
                        भङ्ग;
                    ptr = ptr0;
                    प्रति (;;) {
                        ptr = वाक्ये_अन्विष(ptr, substr);
                        यदि (!ptr)
                            भङ्ग;
                        चरमः_occurrence = पङ्क्तिः_विस्थापनम् + (पूर्णाङ्क) (ptr - ptr0);
                        ptr += वाक्यदैर्घ्यम्(substr);
                    }
                }
                यदि (चरमः_occurrence < 0) {
                    release();
                    CV_Error(cv::Error::StsError, "Could not find </opencv_storage> in the end of file.\n");
                }
                closeFile();
                file = fopen(filename.c_str(), "r+t");
                CV_Assert(file != 0);
                fseek(file, चरमः_occurrence, SEEK_SET);
                // replace the last "</opencv_storage>" with " <!-- resumed -->", which has the same length
                पङ्क्तिं_लिख(" <!-- resumed -->");
                fseek(file, 0, SEEK_END);
                पङ्क्तिं_लिख("\n");
            }

            emitter_do_not_use_प्रत्यक्षम्_dereference = createXMLEmitter(एतत्);
        } अन्यथा यदि (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                यदि (!append) {
                    यदि (fmt == FileStorage::FORMAT_YAML_1_0)
                        पङ्क्तिं_लिख("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    अन्यथा
                        पङ्क्तिं_लिख("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            अन्यथा
                पङ्क्तिं_लिख("...\n---\n");

            emitter_do_not_use_प्रत्यक्षम्_dereference = createYAMLEmitter(एतत्);
        } अन्यथा {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            यदि (!append)
                पङ्क्तिं_लिख("{\n");
            अन्यथा {
                तार्किक valid = असत्य;
                दीर्घ roffset = 0;
                प्रति (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    ध्रुव वर्ण अन्तः_mark = '}';
                    यदि (fgetc(file) == अन्तः_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = सत्य;
                        भङ्ग;
                    }
                }

                यदि (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } अन्यथा {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_राशिः.back().indent = 4;
            emitter_do_not_use_प्रत्यक्षम्_dereference = createJSONEmitter(एतत्);
        }
        is_opened = सत्य;
    } अन्यथा {
        ध्रुव size_t buf_size0 = 40;
        बफर.resize(buf_size0);
        यदि (mem_बहुलकम्) {
            strbuf = (वर्ण *) filename_or_buf;
            strbufsize = वाक्यदैर्घ्यम्(strbuf);
        }

        ध्रुव वर्ण *yaml_signature = "%YAML";
        ध्रुव वर्ण *json_signature = "{";
        ध्रुव वर्ण *xml_signature = "<?xml";
        वर्ण *buf = एतत्->gets(16);
        CV_Assert(buf);
        वर्ण *bufPtr = cv_skip_BOM(buf);
        size_t buf_विस्थापनम् = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        यदि (वाक्ये_तुलय(bufPtr, yaml_signature, वाक्यदैर्घ्यम्(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        अन्यथा यदि (वाक्ये_तुलय(bufPtr, json_signature, वाक्यदैर्घ्यम्(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        अन्यथा यदि (वाक्ये_तुलय(bufPtr, xml_signature, वाक्यदैर्घ्यम्(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        अन्यथा
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = buf_विस्थापनम्;
        bufofs = 0;

        यत्न {
            वर्ण *ptr = बफर_Start();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_ग्रन्थिः मूलम्_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_ग्रन्थिः_Space(मूलम्_nodes, 9);
            *rptr = File_ग्रन्थिः::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            विकल्प (fmt) {
                पक्ष FileStorage::FORMAT_XML:
                    parser_do_not_use_प्रत्यक्षम्_dereference = createXMLParser(एतत्);
                    भङ्ग;
                पक्ष FileStorage::FORMAT_YAML:
                पक्ष FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_प्रत्यक्षम्_dereference = createYAMLParser(एतत्);
                    भङ्ग;
                पक्ष FileStorage::FORMAT_JSON:
                    parser_do_not_use_प्रत्यक्षम्_dereference = createJSONParser(एतत्);
                    भङ्ग;
                स्वभाव:
                    parser_do_not_use_प्रत्यक्षम्_dereference = Ptr<FileStorageParser>();
            }

            यदि (!parser_do_not_use_प्रत्यक्षम्_dereference.empty()) {
                ok = getParser().parse(ptr);
                यदि (ok) {
                    finalizeCollection(मूलम्_nodes);

                    CV_Assert(!fs_दत्त_ptrs.empty());
                    File_ग्रन्थिः roots_ग्रन्थिः(fs_ext, 0, 0);
                    size_t i, nroots = roots_ग्रन्थिः.size();
                    File_ग्रन्थिः_Iterator it = roots_ग्रन्थिः.begin();

                    प्रति (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        गृहाण (...)
        {
            // FIXIT log error message
            is_opened = सत्य;
            release();
            क्षिप;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = सत्य;
        std::vector<वर्ण> tmpbuf;
        std::swap(बफर, tmpbuf);
        bufofs = 0;
    }
    प्रत्यागम ok;
}

शून्य FileStorage::Impl::पङ्क्तिं_लिख(ध्रुव वर्ण *str) {
    CV_Assert(write_बहुलकम्);
    यदि (mem_बहुलकम्)
        std::copy(str, str + वाक्यदैर्घ्यम्(str), std::back_inserter(outbuf));
    अन्यथा यदि (file)
        fputs(str, file);
#यदि USE_ZLIB
    अन्यथा यदि (gzfile)
        gzputs(gzfile, str);
#यदि_अन्तः
    अन्यथा
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

वर्ण *FileStorage::Impl::getsFromFile(वर्ण *buf, पूर्णाङ्क गणनम्_2) {
    यदि (file)
        प्रत्यागम fgets(buf, गणनम्_2, file);
#यदि USE_ZLIB
    यदि (gzfile)
        प्रत्यागम gzgets(gzfile, buf, गणनम्_2);
#यदि_अन्तः
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

वर्ण *FileStorage::Impl::gets(size_t max_गणनम्) {
    यदि (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        ध्रुव वर्ण *instr = strbuf;
        प्रति (; i < len; i++) {
            वर्ण c = instr[i];
            यदि (c == '\0' || c == '\n') {
                यदि (c == '\n')
                    i++;
                भङ्ग;
            }
        }
        size_t गणनम्_2 = i - strbufpos;
        यदि (max_गणनम् == 0 || max_गणनम् > गणनम्_2)
            max_गणनम् = गणनम्_2;
        बफर.resize(std::max(बफर.size(), max_गणनम् + 8));
        स्मृतिं_प्रतिलिख(&बफर[0], instr + strbufpos, max_गणनम्);
        बफर[max_गणनम्] = '\0';
        strbufpos = i;
        प्रत्यागम max_गणनम् > 0 ? &बफर[0] : 0;
    }

    ध्रुव size_t MAX_खण्डम्_आकार = अधिकतम_पूर्णाङ्कः / 2; // hopefully, that will be enough
    यदि (max_गणनम् == 0)
        max_गणनम् = MAX_खण्डम्_आकार;
    अन्यथा
        CV_Assert(max_गणनम् < MAX_खण्डम्_आकार);
    size_t ofs = 0;

    प्रति (;;) {
        पूर्णाङ्क गणनम्_2 = (पूर्णाङ्क) std::min(बफर.size() - ofs - 16, max_गणनम्);
        वर्ण *ptr = getsFromFile(&बफर[ofs], गणनम्_2 + 1);
        यदि (!ptr)
            भङ्ग;
        पूर्णाङ्क delta = (पूर्णाङ्क) वाक्यदैर्घ्यम्(ptr);
        ofs += delta;
        max_गणनम् -= delta;
        यदि (delta == 0 || ptr[delta - 1] == '\n' || max_गणनम् == 0)
            भङ्ग;
        यदि (delta == गणनम्_2)
            बफर.resize((size_t) (बफर.size() * 1.5));
    }
    प्रत्यागम ofs > 0 ? &बफर[0] : 0;
}

वर्ण *FileStorage::Impl::gets() {
    वर्ण *ptr = एतत्->gets(0);
    यदि (!ptr) {
        ptr = बफर_Start();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        समुच्चयः_Eof();
        प्रत्यागम 0;
    } अन्यथा {
        size_t l = वाक्यदैर्घ्यम्(ptr);
        यदि (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    प्रत्यागम ptr;
}

तार्किक FileStorage::Impl::eof() {
    यदि (dummy_eof)
        प्रत्यागम सत्य;
    यदि (strbuf)
        प्रत्यागम strbufpos >= strbufsize;
    यदि (file)
        प्रत्यागम feof(file) != 0;
#यदि USE_ZLIB
    यदि (gzfile)
        प्रत्यागम gzeof(gzfile) != 0;
#यदि_अन्तः
    प्रत्यागम असत्य;
}

शून्य FileStorage::Impl::समुच्चयः_Eof() {
    dummy_eof = सत्य;
}

शून्य FileStorage::Impl::closeFile() {
    यदि (file)
        fclose(file);
#यदि USE_ZLIB
    अन्यथा यदि (gzfile)
        gzclose(gzfile);
#यदि_अन्तः
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = असत्य;
}

शून्य FileStorage::Impl::rewind() {
    यदि (file)
        ::rewind(file);
#यदि USE_ZLIB
    अन्यथा यदि (gzfile)
        gzrewind(gzfile);
#यदि_अन्तः
    strbufpos = 0;
}

वर्ण *FileStorage::Impl::resize_Write_बफर(वर्ण *ptr, पूर्णाङ्क len) {
    ध्रुव वर्ण *बफर_अन्तः = &बफर[0] + बफर.size();
    यदि (ptr + len < बफर_अन्तः)
        प्रत्यागम ptr;

    ध्रुव वर्ण *बफर_start = &बफर[0];
    पूर्णाङ्क written_len = (पूर्णाङ्क) (ptr - बफर_start);

    CV_Assert(written_len <= (पूर्णाङ्क) बफर.size());
    पूर्णाङ्क new_आकार_2 = (पूर्णाङ्क) ((बफर_अन्तः - बफर_start) * 3 / 2);
    new_आकार_2 = MAX(written_len + len, new_आकार_2);
    बफर.reserve(new_आकार_2 + 256);
    बफर.resize(new_आकार_2);
    bufofs = written_len;
    प्रत्यागम &बफर[0] + bufofs;
}

वर्ण *FileStorage::Impl::flush() {
    वर्ण *बफर_start = &बफर[0];
    वर्ण *ptr = बफर_start + bufofs;

    यदि (ptr > बफर_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        पङ्क्तिं_लिख(बफर_start);
        bufofs = 0;
    }

    पूर्णाङ्क indent = write_राशिः.back().indent;

    यदि (space != indent) {
        स्मृतिं_पूरय(बफर_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = बफर_start + bufofs;

    प्रत्यागम ptr;
}

शून्य FileStorage::Impl::अन्तः_Write_Struct() {
    CV_Assert(write_बहुलकम्);

    check_if_write_struct_is_delayed(असत्य);
    यदि (state_सम्बन्धि_writing_base64 != FileStorage_API::Uncertain)
        switch_गन्तव्यम्_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_राशिः.empty());

    FStruct_दत्त &current_struct = write_राशिः.back();
    यदि (fmt == FileStorage::FORMAT_JSON && !File_ग्रन्थिः::isFlow(current_struct.flags) && write_राशिः.size() > 1)
        current_struct.indent = write_राशिः[write_राशिः.size() - 2].indent;

    getEmitter().अन्तः_Write_Struct(current_struct);

    write_राशिः.pop_back();
    यदि (!write_राशिः.empty())
        write_राशिः.back().flags &= ~File_ग्रन्थिः::रिक्तानि_2;
}

शून्य FileStorage::Impl::startWriteStruct_helper(ध्रुव वर्ण *कुञ्चिका, पूर्णाङ्क struct_flags,
                                                ध्रुव वर्ण *type_name) {
    CV_Assert(write_बहुलकम्);

    struct_flags = (struct_flags & (File_ग्रन्थिः::TYPE_आवरणम् | File_ग्रन्थिः::FLOW)) | File_ग्रन्थिः::रिक्तानि_2;
    यदि (!File_ग्रन्थिः::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    यदि (type_name && type_name[0] == '\0')
        type_name = 0;

    FStruct_दत्त s = getEmitter().startWriteStruct(write_राशिः.back(), कुञ्चिका, struct_flags, type_name);

    write_राशिः.push_back(s);
    size_t write_राशिः_आकार = write_राशिः.size();
    यदि (write_राशिः_आकार > 1)
        write_राशिः[write_राशिः_आकार - 2].flags &= ~File_ग्रन्थिः::रिक्तानि_2;

    यदि (fmt != FileStorage::FORMAT_JSON && !File_ग्रन्थिः::isFlow(s.flags))
        flush();

    यदि (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_ग्रन्थिः::isMap(struct_flags)) {
        getEmitter().लिख("type_id", type_name, असत्य);
    }
}

शून्य FileStorage::Impl::startWriteStruct(ध्रुव वर्ण *कुञ्चिका, पूर्णाङ्क struct_flags,
                                         ध्रुव वर्ण *type_name) {
    check_if_write_struct_is_delayed(असत्य);
    यदि (state_सम्बन्धि_writing_base64 == FileStorage_API::NotUse)
        switch_गन्तव्यम्_Base64_state(FileStorage_API::Uncertain);

    यदि (state_सम्बन्धि_writing_base64 == FileStorage_API::Uncertain && File_ग्रन्थिः::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(कुञ्चिका, struct_flags, type_name);
    } अन्यथा यदि (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        यदि ((File_ग्रन्थिः::TYPE_आवरणम् & struct_flags) != File_ग्रन्थिः::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        अन्यथा यदि (state_सम्बन्धि_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(कुञ्चिका, struct_flags, "binary");

        यदि (state_सम्बन्धि_writing_base64 != FileStorage_API::Uncertain)
            switch_गन्तव्यम्_Base64_state(FileStorage_API::Uncertain);
        switch_गन्तव्यम्_Base64_state(FileStorage_API::अन्तर्_Use);
    } अन्यथा {
        /* Won't output Base64 data */
        यदि (state_सम्बन्धि_writing_base64 == FileStorage_API::अन्तर्_Use)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(कुञ्चिका, struct_flags, type_name);

        यदि (state_सम्बन्धि_writing_base64 != FileStorage_API::Uncertain)
            switch_गन्तव्यम्_Base64_state(FileStorage_API::Uncertain);
        switch_गन्तव्यम्_Base64_state(FileStorage_API::NotUse);
    }
}

शून्य FileStorage::Impl::writeComment(ध्रुव वर्ण *comment, तार्किक eol_comment) {
    CV_Assert(write_बहुलकम्);
    getEmitter().writeComment(comment, eol_comment);
}

शून्य FileStorage::Impl::start_अग्र_प्रवाहः() {
    CV_Assert(write_बहुलकम्);
    यदि (!रिक्तानि_प्रवाहः) {
        यावत् (!write_राशिः.empty())
            अन्तः_Write_Struct();
        flush();
        getEmitter().start_अग्र_प्रवाहः();
        रिक्तानि_प्रवाहः = सत्य;
        write_राशिः.push_back(FStruct_दत्त("", File_ग्रन्थिः::रिक्तानि_2, 0));
        bufofs = 0;
    }
}

शून्य FileStorage::Impl::लिख(ध्रुव String &कुञ्चिका, पूर्णाङ्क value) {
    CV_Assert(write_बहुलकम्);
    getEmitter().लिख(कुञ्चिका.c_str(), value);
}

शून्य FileStorage::Impl::लिख(ध्रुव String &कुञ्चिका, int64_t value) {
    CV_Assert(write_बहुलकम्);
    getEmitter().लिख(कुञ्चिका.c_str(), value);
}

शून्य FileStorage::Impl::लिख(ध्रुव String &कुञ्चिका, द्विगुण value) {
    CV_Assert(write_बहुलकम्);
    getEmitter().लिख(कुञ्चिका.c_str(), value);
}

शून्य FileStorage::Impl::लिख(ध्रुव String &कुञ्चिका, ध्रुव String &value) {
    CV_Assert(write_बहुलकम्);
    getEmitter().लिख(कुञ्चिका.c_str(), value.c_str(), असत्य);
}

शून्य FileStorage::Impl::write_Raw_दत्त(ध्रुव std::string &dt, ध्रुव शून्य *दत्त_3, size_t len) {
    CV_Assert(write_बहुलकम्);

    यदि (is_using_base64 || state_सम्बन्धि_writing_base64 == FileStorage_API::Base64State::अन्तर्_Use) {
        write_Raw_दत्त_Base64(दत्त_3, len, dt.c_str());
        प्रत्यागम;
    } अन्यथा यदि (state_सम्बन्धि_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_गन्तव्यम्_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elem_आकार = fs::calc_Struct_आकार(dt.c_str(), 0);
    CV_Assert(elem_आकार);
    CV_Assert(len % elem_आकार == 0);
    len /= elem_आकार;

    तार्किक explicitZero = fmt == FileStorage::FORMAT_JSON;
    ध्रुव uchar *data0 = (ध्रुव uchar *) दत्त_3;
    पूर्णाङ्क fmt_युग्मानि[CV_FS_MAX_FMT_युग्मानि * 2], k, fmt_pair_गणनम्;
    वर्ण buf[256] = "";

    fmt_pair_गणनम् = fs::decodeFormat(dt.c_str(), fmt_युग्मानि, CV_FS_MAX_FMT_युग्मानि);

    यदि (!len)
        प्रत्यागम;

    यदि (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    यदि (fmt_pair_गणनम् == 1) {
        fmt_युग्मानि[0] *= (पूर्णाङ्क) len;
        len = 1;
    }

    प्रति (; len--; data0 += elem_आकार) {
        पूर्णाङ्क offset = 0;
        प्रति (k = 0; k < fmt_pair_गणनम्; k++) {
            पूर्णाङ्क i, गणनम्_2 = fmt_युग्मानि[k * 2];
            पूर्णाङ्क elem_type = fmt_युग्मानि[k * 2 + 1];
            पूर्णाङ्क elem_आकार_2 = CV_ELEM_आकार(elem_type);
            ध्रुव वर्ण *ptr;

            offset = cvAlign(offset, elem_आकार_2);
            ध्रुव uchar *दत्त = data0 + offset;

            प्रति (i = 0; i < गणनम्_2; i++) {
                विकल्प (elem_type) {
                    पक्ष CV_8U:
                        ptr = fs::itoa(*(uchar *) दत्त, buf, 10);
                        दत्त++;
                        भङ्ग;
                    पक्ष CV_Bool:
                        ptr = fs::itoa(*(uchar *) दत्त != 0, buf, 10);
                        दत्त++;
                        भङ्ग;
                    पक्ष CV_8S:
                        ptr = fs::itoa(*(वर्ण *) दत्त, buf, 10);
                        दत्त++;
                        भङ्ग;
                    पक्ष CV_16U:
                        ptr = fs::itoa(*(ushort *) दत्त, buf, 10);
                        दत्त += परिमाण(ushort);
                        भङ्ग;
                    पक्ष CV_16S:
                        ptr = fs::itoa(*(ह्रस्व *) दत्त, buf, 10);
                        दत्त += परिमाण(ह्रस्व);
                        भङ्ग;
                    पक्ष CV_32U:
                        ptr = fs::itoa((int64_t)*(अचिह्नित*) दत्त, buf, 10, असत्य);
                        दत्त += परिमाण(अचिह्नित);
                        भङ्ग;
                    पक्ष CV_32S:
                        ptr = fs::itoa(*(पूर्णाङ्क *) दत्त, buf, 10);
                        दत्त += परिमाण(पूर्णाङ्क);
                        भङ्ग;
                    पक्ष CV_64U:
                        ptr = fs::itoa(*(uint64_t*) दत्त, buf, 10, असत्य);
                        दत्त += परिमाण(uint64_t);
                        भङ्ग;
                    पक्ष CV_64S:
                        ptr = fs::itoa(*(int64_t*) दत्त, buf, 10, सत्य);
                        दत्त += परिमाण(int64_t);
                        भङ्ग;
                    पक्ष CV_32F:
                        ptr = fs::float_गन्तव्यम्_String(buf, परिमाण(buf), *(भिन्न *) दत्त, असत्य, explicitZero);
                        दत्त += परिमाण(भिन्न);
                        भङ्ग;
                    पक्ष CV_64F:
                        ptr = fs::double_गन्तव्यम्_String(buf, परिमाण(buf), *(द्विगुण *) दत्त, explicitZero);
                        दत्त += परिमाण(द्विगुण);
                        भङ्ग;
                    पक्ष CV_16F:
                        ptr = fs::float_गन्तव्यम्_String(buf, परिमाण(buf), (भिन्न) *(hfloat *) दत्त, सत्य, explicitZero);
                        दत्त += परिमाण(hfloat);
                        भङ्ग;
                    पक्ष CV_16BF:
                        ptr = fs::float_गन्तव्यम्_String(buf, परिमाण(buf), (भिन्न) *(bfloat *) दत्त, सत्य, explicitZero);
                        दत्त += परिमाण(bfloat);
                        भङ्ग;
                    पक्ष CV_8F_E4M3FN:
                        ptr = fs::float_गन्तव्यम्_String(buf, परिमाण(buf), (भिन्न) *(fp8_t *) दत्त, सत्य, explicitZero);
                        दत्त += परिमाण(fp8_t);
                        भङ्ग;
                    पक्ष CV_8F_E4M3FNUZ:
                        ptr = fs::float_गन्तव्यम्_String(buf, परिमाण(buf), (भिन्न) *(fp8a_t *) दत्त, सत्य, explicitZero);
                        दत्त += परिमाण(fp8a_t);
                        भङ्ग;
                    स्वभाव:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        प्रत्यागम;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (पूर्णाङ्क) (दत्त - data0);
        }
    }
}

शून्य FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(असत्य);

    यदि (state_सम्बन्धि_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_गन्तव्यम्_Base64_state(FileStorage_API::Base64State::Uncertain);
}

शून्य FileStorage::Impl::switch_गन्तव्यम्_Base64_state(FileStorage_API::Base64State new_state) {
    ध्रुव वर्ण *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    ध्रुव वर्ण *err_unable_गन्तव्यम्_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    विकल्प (state_सम्बन्धि_writing_base64) {
        पक्ष FileStorage_API::Base64State::Uncertain:
            विकल्प (new_state) {
                पक्ष FileStorage_API::Base64State::अन्तर्_Use:
                {
                    CV_DbgAssert(base64_writer == 0);
                    तार्किक can_indent = (fmt != cv::FileStorage::बहुलकम्_2::FORMAT_JSON);
                    base64_writer = नूतन base64::Base64Writer(*एतत्, can_indent);
                    यदि (!can_indent) {
                        वर्ण *ptr = बफर_Ptr();
                        *ptr++ = '\0';
                        पङ्क्तिं_लिख(बफर_Start());
                        समुच्चयः_बफर_Ptr(बफर_Start());
                        स्मृतिं_पूरय(बफर_Start(), 0, static_cast<पूर्णाङ्क>(space));
                        पङ्क्तिं_लिख("\"$base64$");
                    }
                    भङ्ग;
                }
                पक्ष FileStorage_API::Base64State::Uncertain:
                    भङ्ग;
                पक्ष FileStorage_API::Base64State::NotUse:
                    भङ्ग;
                स्वभाव:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    भङ्ग;
            }
            भङ्ग;
        पक्ष FileStorage_API::Base64State::अन्तर्_Use:
            विकल्प (new_state) {
                पक्ष FileStorage_API::Base64State::अन्तर्_Use:
                पक्ष FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_गन्तव्यम्_switch);
                    भङ्ग;
                पक्ष FileStorage_API::Base64State::Uncertain:
                    विलोपय base64_writer;
                    base64_writer = 0;
                    यदि ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        पङ्क्तिं_लिख("\"");
                        समुच्चयः_बफर_Ptr(बफर_Start());
                        flush();
                        स्मृतिं_पूरय(बफर_Start(), 0, static_cast<पूर्णाङ्क>(space) );
                        समुच्चयः_बफर_Ptr(बफर_Start());
                    }
                    भङ्ग;
                स्वभाव:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    भङ्ग;
            }
            भङ्ग;
        पक्ष FileStorage_API::Base64State::NotUse:
            विकल्प (new_state) {
                पक्ष FileStorage_API::Base64State::अन्तर्_Use:
                पक्ष FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_गन्तव्यम्_switch);
                    भङ्ग;
                पक्ष FileStorage_API::Base64State::Uncertain:
                    भङ्ग;
                स्वभाव:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    भङ्ग;
            }
            भङ्ग;
        स्वभाव:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            भङ्ग;
    }

    state_सम्बन्धि_writing_base64 = new_state;
}

शून्य FileStorage::Impl::make_write_struct_delayed(ध्रुव वर्ण *कुञ्चिका, पूर्णाङ्क struct_flags, ध्रुव वर्ण *type_name) {
    CV_Assert(is_write_struct_delayed == असत्य);
    CV_DbgAssert(delayed_struct_कुञ्चिका == शून्यसूचकः);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == शून्यसूचकः);

    delayed_struct_flags = struct_flags;

    यदि (कुञ्चिका != शून्यसूचकः) {
        delayed_struct_कुञ्चिका = नूतन वर्ण[वाक्यदैर्घ्यम्(कुञ्चिका) + 1U];
        वाक्यं_प्रतिलिख(delayed_struct_कुञ्चिका, कुञ्चिका);
    }

    यदि (type_name != शून्यसूचकः) {
        delayed_type_name = नूतन वर्ण[वाक्यदैर्घ्यम्(type_name) + 1U];
        वाक्यं_प्रतिलिख(delayed_type_name, type_name);
    }

    is_write_struct_delayed = सत्य;
}

शून्य FileStorage::Impl::check_if_write_struct_is_delayed(तार्किक change_type_गन्तव्यम्_base64) {
    यदि (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_कुञ्चिका;
        std::string type_name;
        पूर्णाङ्क struct_flags = delayed_struct_flags;

        यदि (delayed_struct_कुञ्चिका != शून्यसूचकः && *delayed_struct_कुञ्चिका != '\0') {
            struct_कुञ्चिका.assign(delayed_struct_कुञ्चिका);
        }
        यदि (delayed_type_name != शून्यसूचकः && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        विलोपय[] delayed_struct_कुञ्चिका;
        विलोपय[] delayed_type_name;
        delayed_struct_कुञ्चिका = शून्यसूचकः;
        delayed_struct_flags = 0;
        delayed_type_name = शून्यसूचकः;

        is_write_struct_delayed = असत्य;

        /* call */
        यदि (change_type_गन्तव्यम्_base64) {
            startWriteStruct_helper(struct_कुञ्चिका.c_str(), struct_flags, "binary");
            यदि (state_सम्बन्धि_writing_base64 != FileStorage_API::Uncertain)
                switch_गन्तव्यम्_Base64_state(FileStorage_API::Uncertain);
            switch_गन्तव्यम्_Base64_state(FileStorage_API::अन्तर्_Use);
        } अन्यथा {
            startWriteStruct_helper(struct_कुञ्चिका.c_str(), struct_flags, type_name.c_str());
            यदि (state_सम्बन्धि_writing_base64 != FileStorage_API::Uncertain)
                switch_गन्तव्यम्_Base64_state(FileStorage_API::Uncertain);
            switch_गन्तव्यम्_Base64_state(FileStorage_API::NotUse);
        }
    }
}

शून्य FileStorage::Impl::write_Raw_दत्त_Base64(ध्रुव शून्य *दत्त_3, size_t len, ध्रुव वर्ण *dt) {
    CV_Assert(write_बहुलकम्);

    check_if_write_struct_is_delayed(सत्य);

    यदि (state_सम्बन्धि_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_गन्तव्यम्_Base64_state(FileStorage_API::Base64State::अन्तर्_Use);
    } अन्यथा यदि (state_सम्बन्धि_writing_base64 != FileStorage_API::Base64State::अन्तर्_Use) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->लिख(दत्त_3, len, dt);
}

File_ग्रन्थिः FileStorage::Impl::get_First_शिखरम्_Level_ग्रन्थिः() ध्रुव {
    प्रत्यागम roots.empty() ? File_ग्रन्थिः() : roots[0];
}

File_ग्रन्थिः FileStorage::Impl::मूलम्(पूर्णाङ्क प्रवाहः_Idx) ध्रुव {
    प्रत्यागम प्रवाहः_Idx >= 0 && प्रवाहः_Idx < (पूर्णाङ्क) roots.size() ? roots[प्रवाहः_Idx] : File_ग्रन्थिः();
}

File_ग्रन्थिः FileStorage::Impl::संक्रियकः[](ध्रुव String &nodename) ध्रुव {
    प्रत्यागम एतत्->संक्रियकः[](nodename.c_str());
}

File_ग्रन्थिः FileStorage::Impl::संक्रियकः[](ध्रुव वर्ण * /*nodename*/) ध्रुव {
    प्रत्यागम File_ग्रन्थिः();
}

पूर्णाङ्क FileStorage::Impl::getFormat() ध्रुव { प्रत्यागम fmt; }

वर्ण *FileStorage::Impl::बफर_Ptr() ध्रुव { प्रत्यागम (वर्ण *) (&बफर[0] + bufofs); }

वर्ण *FileStorage::Impl::बफर_Start() ध्रुव { प्रत्यागम (वर्ण *) &बफर[0]; }

वर्ण *FileStorage::Impl::बफर_अन्तः_2() ध्रुव { प्रत्यागम (वर्ण *) (&बफर[0] + बफर.size()); }

शून्य FileStorage::Impl::समुच्चयः_बफर_Ptr(वर्ण *ptr) {
    वर्ण *bufferstart = बफर_Start();
    CV_Assert(ptr >= bufferstart && ptr <= बफर_अन्तः_2());
    bufofs = ptr - bufferstart;
}

पूर्णाङ्क FileStorage::Impl::wrapMargin() ध्रुव { प्रत्यागम wrap_margin; }

FStruct_दत्त &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_राशिः.empty());
    प्रत्यागम write_राशिः.back();
}

शून्य FileStorage::Impl::समुच्चयः_Non_रिक्तानि() {
    रिक्तानि_प्रवाहः = असत्य;
}

शून्य FileStorage::Impl::processSpecialDouble(वर्ण *buf, द्विगुण *value, वर्ण **endptr) {
    FileStorage_API *fs = एतत्;
    वर्ण c = buf[0];
    पूर्णाङ्क inf_hi = 0x7ff00000;

    यदि (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    यदि (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    यदि (महाक्षरम्(buf[1]) == 'I' && महाक्षरम्(buf[2]) == 'N' && महाक्षरम्(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    अन्यथा यदि (महाक्षरम्(buf[1]) == 'N' && महाक्षरम्(buf[2]) == 'A' && महाक्षरम्(buf[3]) == 'N')
        v.u = (uint64) -1;
    अन्यथा
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

द्विगुण FileStorage::Impl::strtod(वर्ण *ptr, वर्ण **endptr) {
    द्विगुण fval = ::strtod(ptr, endptr);
    यदि (**endptr == '.') {
        वर्ण *dot_pos = *endptr;
        *dot_pos = ',';
        द्विगुण fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        यदि (*endptr > dot_pos)
            fval = fval2;
        अन्यथा
            *endptr = dot_pos;
    }

    यदि (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    प्रत्यागम fval;
}

शून्य FileStorage::Impl::convert_गन्तव्यम्_Collection(पूर्णाङ्क type, File_ग्रन्थिः &ग्रन्थिः) {
    CV_Assert(type == File_ग्रन्थिः::SEQ || type == File_ग्रन्थिः::MAP);

    पूर्णाङ्क ग्रन्थिः_type = ग्रन्थिः.type();
    यदि (ग्रन्थिः_type == type)
        प्रत्यागम;

    तार्किक named = ग्रन्थिः.isNamed();
    uchar *ptr = ग्रन्थिः.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    द्विगुण fval = 0;
    std::string sval;
    तार्किक सङ्कलय_first_scalar = असत्य;

    यदि (ग्रन्थिः_type != File_ग्रन्थिः::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_ग्रन्थिः::SEQ);
        यदि (ग्रन्थिः_type == File_ग्रन्थिः::INT) {
            ival = readLong(ptr);
            सङ्कलय_first_scalar = सत्य;
        } अन्यथा यदि (ग्रन्थिः_type == File_ग्रन्थिः::REAL) {
            fval = readReal(ptr);
            सङ्कलय_first_scalar = सत्य;
        } अन्यथा यदि (ग्रन्थिः_type == File_ग्रन्थिः::STRING) {
            sval = std::string(ग्रन्थिः);
            सङ्कलय_first_scalar = सत्य;
        } अन्यथा
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", ग्रन्थिः_type));
    }

    ptr = reserve_ग्रन्थिः_Space(ग्रन्थिः, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_ग्रन्थिः::NAMED : 0));
    // name has been copied automatically
    यदि (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    यदि (सङ्कलय_first_scalar)
        सङ्कलय_ग्रन्थिः(ग्रन्थिः, std::string(), ग्रन्थिः_type,
                ग्रन्थिः_type == File_ग्रन्थिः::INT ? (ध्रुव शून्य *) &ival :
                ग्रन्थिः_type == File_ग्रन्थिः::REAL ? (ध्रुव शून्य *) &fval :
                ग्रन्थिः_type == File_ग्रन्थिः::STRING ? (ध्रुव शून्य *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_ग्रन्थिः_Space(File_ग्रन्थिः &ग्रन्थिः, size_t sz) {
    तार्किक shrink_खण्डम् = असत्य;
    size_t shrink_खण्डम्_Idx = 0, shrink_आकार = 0;

    uchar *ptr = 0, *खण्डम्_अन्तः = 0;

    यदि (!fs_दत्त_ptrs.empty()) {
        size_t खण्डम्_Idx = ग्रन्थिः.खण्डम्_Idx;
        size_t ofs = ग्रन्थिः.ofs;
        CV_Assert(खण्डम्_Idx == fs_दत्त_ptrs.size() - 1);
        CV_Assert(ofs <= fs_दत्त_blksz[खण्डम्_Idx]);
        CV_Assert(freeSpaceOfs <= fs_दत्त_blksz[खण्डम्_Idx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_दत्त_ptrs[खण्डम्_Idx] + ofs;
        खण्डम्_अन्तः = fs_दत्त_ptrs[खण्डम्_Idx] + fs_दत्त_blksz[खण्डम्_Idx];

        CV_Assert(ptr >= fs_दत्त_ptrs[खण्डम्_Idx] && ptr <= खण्डम्_अन्तः);
        यदि (ptr + sz <= खण्डम्_अन्तः) {
            freeSpaceOfs = ofs + sz;
            प्रत्यागम ptr;
        }

        यदि (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_दत्त[खण्डम्_Idx]->resize(sz);
            ptr = &fs_दत्त[खण्डम्_Idx]->at(0);
            fs_दत्त_ptrs[खण्डम्_Idx] = ptr;
            fs_दत्त_blksz[खण्डम्_Idx] = sz;
            freeSpaceOfs = sz;
            प्रत्यागम ptr;
        }

        shrink_खण्डम् = सत्य;
        shrink_खण्डम्_Idx = खण्डम्_Idx;
        shrink_आकार = ofs;
    }

    size_t खण्डम्_आकार_2 = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(खण्डम्_आकार_2);
    fs_दत्त.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_दत्त_ptrs.push_back(new_ptr);
    fs_दत्त_blksz.push_back(खण्डम्_आकार_2);
    ग्रन्थिः.खण्डम्_Idx = fs_दत्त_ptrs.size() - 1;
    ग्रन्थिः.ofs = 0;
    freeSpaceOfs = sz;

    यदि (ptr && ptr + 5 <= खण्डम्_अन्तः) {
        new_ptr[0] = ptr[0];
        यदि (ptr[0] & File_ग्रन्थिः::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    यदि (shrink_खण्डम्) {
        fs_दत्त[shrink_खण्डम्_Idx]->resize(shrink_आकार);
        fs_दत्त_blksz[shrink_खण्डम्_Idx] = shrink_आकार;
    }

    प्रत्यागम new_ptr;
}

अचिह्नित FileStorage::Impl::getStringOfs(ध्रुव std::string &कुञ्चिका) ध्रुव {
    str_hash_t::const_iterator it = str_hash.find(कुञ्चिका);
    प्रत्यागम it != str_hash.end() ? it->second : 0;
}

File_ग्रन्थिः FileStorage::Impl::सङ्कलय_ग्रन्थिः(File_ग्रन्थिः &collection, ध्रुव std::string &कुञ्चिका,
                                    पूर्णाङ्क elem_type, ध्रुव शून्य *value, पूर्णाङ्क len) {
    FileStorage_API *fs = एतत्;
    तार्किक noname = कुञ्चिका.empty() || (fmt == FileStorage::FORMAT_XML && वाक्यं_तुलय(कुञ्चिका.c_str(), "_") == 0);
    convert_गन्तव्यम्_Collection(noname ? File_ग्रन्थिः::SEQ : File_ग्रन्थिः::MAP, collection);

    तार्किक isseq = collection.empty() ? असत्य : collection.isSeq();
    यदि (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    अचिह्नित strofs = 0;
    यदि (!noname) {
        strofs = getStringOfs(कुञ्चिका);
        यदि (!strofs) {
            strofs = (अचिह्नित) str_hash_दत्त.size();
            size_t keysize = कुञ्चिका.size() + 1;
            str_hash_दत्त.resize(strofs + keysize);
            स्मृतिं_प्रतिलिख(&str_hash_दत्त[0] + strofs, &कुञ्चिका[0], keysize);
            str_hash.insert(std::make_pair(कुञ्चिका, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t खण्डम्_Idx = fs_दत्त_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_ग्रन्थिः ग्रन्थिः(fs_ext, खण्डम्_Idx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_ग्रन्थिः_Space(ग्रन्थिः, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_ग्रन्थिः::NAMED));
    यदि (elem_type == File_ग्रन्थिः::NONE)
        freeSpaceOfs -= 8;

    यदि (!noname) {
        writeInt(ptr, (पूर्णाङ्क) strofs);
        ptr += 4;
    }

    यदि (elem_type == File_ग्रन्थिः::SEQ || elem_type == File_ग्रन्थिः::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    यदि (value)
        ग्रन्थिः.समुच्चयः_मानम्(elem_type, value, len);

    यदि (collection.isNamed())
        cp += 4;
    पूर्णाङ्क nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    प्रत्यागम ग्रन्थिः;
}

शून्य FileStorage::Impl::finalizeCollection(File_ग्रन्थिः &collection) {
    यदि (!collection.isSeq() && !collection.isMap())
        प्रत्यागम;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    यदि (*ptr0 & File_ग्रन्थिः::NAMED)
        ptr += 4;
    size_t खण्डम्_Idx = collection.खण्डम्_Idx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t raw_आकार = 4;
    अचिह्नित sz = (अचिह्नित) readInt(ptr + 4);
    यदि (sz > 0) {
        size_t चरमः_खण्डम्_Idx = fs_दत्त_ptrs.size() - 1;

        प्रति (; खण्डम्_Idx < चरमः_खण्डम्_Idx; खण्डम्_Idx++) {
            raw_आकार += fs_दत्त_blksz[खण्डम्_Idx] - ofs;
            ofs = 0;
        }
    }
    raw_आकार += freeSpaceOfs - ofs;
    writeInt(ptr, (पूर्णाङ्क) raw_आकार);
}

शून्य FileStorage::Impl::normalize_ग्रन्थिः_Ofs(size_t &खण्डम्_Idx, size_t &ofs) ध्रुव {
    यावत् (ofs >= fs_दत्त_blksz[खण्डम्_Idx]) {
        यदि (खण्डम्_Idx == fs_दत्त_blksz.size() - 1) {
            CV_Assert(ofs == fs_दत्त_blksz[खण्डम्_Idx]);
            भङ्ग;
        }
        ofs -= fs_दत्त_blksz[खण्डम्_Idx];
        खण्डम्_Idx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_सम्बन्धि_writing_base64() {
    प्रत्यागम state_सम्बन्धि_writing_base64;
}

पूर्णाङ्क FileStorage::Impl::get_space() {
    प्रत्यागम space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = सत्य;
}

शून्य FileStorage::Impl::Base64Decoder::init(ध्रुव Ptr<FileStorageParser> &_parser, वर्ण *_ptr, पूर्णाङ्क _indent) {
    parser_do_not_use_प्रत्यक्षम्_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = असत्य;
}

तार्किक FileStorage::Impl::Base64Decoder::read_अधिकम्(पूर्णाङ्क needed) {
    स्थिर ध्रुव uchar base64tab[] =
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

    यदि (eos)
        प्रत्यागम असत्य;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    प्रति (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    वर्ण *beg = 0, *end = 0;
    तार्किक ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    यदि (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = सत्य;
        size_t tc = totalchars;
        प्रति (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    पूर्णाङ्क i = 0, j, n = (पूर्णाङ्क) encoded.size();
    यदि (n > 0) {
        ध्रुव uchar *tab = base64tab;
        वर्ण *src = &encoded[0];

        प्रति (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(पूर्णाङ्क) (uchar) src[i]], c = tab[(पूर्णाङ्क) (uchar) src[i + 1]];
            uchar b = tab[(पूर्णाङ्क) (uchar) src[i + 2]], a = tab[(पूर्णाङ्क) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    यदि (i > 0 && encoded[i - 1] == '=') {
        यदि (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        यदि (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    प्रति (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    प्रत्यागम (पूर्णाङ्क) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    यदि (ofs >= sz && !read_अधिकम्(1))
        प्रत्यागम (uchar) 0;
    प्रत्यागम decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    यदि (ofs + 2 > sz && !read_अधिकम्(2))
        प्रत्यागम (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    प्रत्यागम val;
}

पूर्णाङ्क FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    यदि (ofs + 4 > sz && !read_अधिकम्(4))
        प्रत्यागम 0;
    पूर्णाङ्क ival = readInt(&decoded[ofs]);
    ofs += 4;
    प्रत्यागम ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    यदि (ofs + 8 > sz && !read_अधिकम्(8))
        प्रत्यागम 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    प्रत्यागम ival;
}

द्विगुण FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    यदि (ofs + 8 > sz && !read_अधिकम्(8))
        प्रत्यागम 0;
    द्विगुण fval = readReal(&decoded[ofs]);
    ofs += 8;
    प्रत्यागम fval;
}

तार्किक FileStorage::Impl::Base64Decoder::अन्तः_सम्बन्धि_प्रवाहः() ध्रुव { प्रत्यागम eos; }

वर्ण *FileStorage::Impl::Base64Decoder::getPtr() ध्रुव { प्रत्यागम ptr; }


वर्ण *FileStorage::Impl::parseBase64(वर्ण *ptr, पूर्णाङ्क indent, File_ग्रन्थिः &collection) {
    ध्रुव पूर्णाङ्क BASE64_HDR_आकार = 24;
    वर्ण dt[BASE64_HDR_आकार + 1] = {0};
    base64decoder.init(parser_do_not_use_प्रत्यक्षम्_dereference, ptr, indent);

    पूर्णाङ्क i, k;

    प्रति (i = 0; i < BASE64_HDR_आकार; i++)
        dt[i] = (वर्ण) base64decoder.getUInt8();
    प्रति (i = 0; i < BASE64_HDR_आकार; i++)
        यदि (रिक्तम्_वा(dt[i]))
            भङ्ग;
    dt[i] = '\0';

    CV_Assert(!base64decoder.अन्तः_सम्बन्धि_प्रवाहः());

    पूर्णाङ्क fmt_युग्मानि[CV_FS_MAX_FMT_युग्मानि * 2];
    पूर्णाङ्क fmt_pair_गणनम् = fs::decodeFormat(dt, fmt_युग्मानि, CV_FS_MAX_FMT_युग्मानि);
    int64_t ival = 0;
    द्विगुण fval = 0;

    प्रति (;;) {
        प्रति (k = 0; k < fmt_pair_गणनम्; k++) {
            पूर्णाङ्क elem_type = fmt_युग्मानि[k * 2 + 1];
            पूर्णाङ्क गणनम्_2 = fmt_युग्मानि[k * 2];

            प्रति (i = 0; i < गणनम्_2; i++) {
                पूर्णाङ्क ग्रन्थिः_type = File_ग्रन्थिः::INT;
                विकल्प (elem_type) {
                    पक्ष CV_8U:
                        ival = base64decoder.getUInt8();
                        भङ्ग;
                    पक्ष CV_8S:
                        ival = (वर्ण) base64decoder.getUInt8();
                        भङ्ग;
                    पक्ष CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        भङ्ग;
                    पक्ष CV_16U:
                        ival = base64decoder.getUInt16();
                        भङ्ग;
                    पक्ष CV_16S:
                        ival = (ह्रस्व) base64decoder.getUInt16();
                        भङ्ग;
                    पक्ष CV_32U:
                        ival = base64decoder.getInt32();
                        भङ्ग;
                    पक्ष CV_32S:
                        ival = base64decoder.getInt32();
                        भङ्ग;
                    पक्ष CV_64U:
                        ival = base64decoder.getInt64();
                        भङ्ग;
                    पक्ष CV_64S:
                        ival = base64decoder.getInt64();
                        भङ्ग;
                    पक्ष CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        ग्रन्थिः_type = File_ग्रन्थिः::REAL;
                    }
                        भङ्ग;
                    पक्ष CV_64F:
                        fval = base64decoder.getFloat64();
                        ग्रन्थिः_type = File_ग्रन्थिः::REAL;
                        भङ्ग;
                    पक्ष CV_16F:
                        fval = भिन्न(hfloat_From_द्वयङ्काः(base64decoder.getUInt16()));
                        ग्रन्थिः_type = File_ग्रन्थिः::REAL;
                        भङ्ग;
                    पक्ष CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        ग्रन्थिः_type = File_ग्रन्थिः::REAL;
                        भङ्ग;
                    पक्ष CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        ग्रन्थिः_type = File_ग्रन्थिः::REAL;
                        भङ्ग;
                    स्वभाव:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                यदि (base64decoder.अन्तः_सम्बन्धि_प्रवाहः())
                    भङ्ग;
                सङ्कलय_ग्रन्थिः(collection, std::string(), ग्रन्थिः_type,
                        ग्रन्थिः_type == File_ग्रन्थिः::INT ? (शून्य *) &ival : (शून्य *) &fval, -1);
            }
        }
        यदि (base64decoder.अन्तः_सम्बन्धि_प्रवाहः())
            भङ्ग;
    }

    finalizeCollection(collection);
    प्रत्यागम base64decoder.getPtr();
}

शून्य FileStorage::Impl::parseError(ध्रुव वर्ण *func_name, ध्रुव std::string &err_सन्देशः, ध्रुव वर्ण *source_file,
                                   पूर्णाङ्क source_पङ्क्तिः) {
    std::string सन्देशः = format("%s(%d): %s", filename.c_str(), lineno, err_सन्देशः.c_str());
    error(Error::StsParseError, func_name, सन्देशः.c_str(), source_file, source_पङ्क्तिः);
}

ध्रुव uchar *FileStorage::Impl::get_ग्रन्थिः_Ptr(size_t खण्डम्_Idx, size_t ofs) ध्रुव {
    CV_Assert(खण्डम्_Idx < fs_दत्त_ptrs.size());
    CV_Assert(ofs < fs_दत्त_blksz[खण्डम्_Idx]);

    प्रत्यागम fs_दत्त_ptrs[खण्डम्_Idx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) ध्रुव {
    CV_Assert(nameofs < str_hash_दत्त.size());
    प्रत्यागम std::string(&str_hash_दत्त[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { प्रत्यागम fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(एतत्);
}

FileStorage::FileStorage(ध्रुव String& filename, पूर्णाङ्क flags, ध्रुव String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(एतत्);
    तार्किक ok = p->उद्घाटय(filename.c_str(), flags, encoding.c_str());
    यदि(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

शून्य FileStorage::startWriteStruct(ध्रुव String& name, पूर्णाङ्क struct_flags, ध्रुव String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    यदि ((struct_flags & File_ग्रन्थिः::TYPE_आवरणम्) == File_ग्रन्थिः::SEQ)
        state = FileStorage::मानम्_EXPECTED;
    अन्यथा
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

शून्य FileStorage::अन्तः_Write_Struct()
{
    p->अन्तः_Write_Struct();
    state = p->write_राशिः.empty() || File_ग्रन्थिः::isMap(p->write_राशिः.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::मानम्_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

तार्किक FileStorage::उद्घाटय(ध्रुव String& filename, पूर्णाङ्क flags, ध्रुव String& encoding)
{
    यत्न
    {
        तार्किक ok = p->उद्घाटय(filename.c_str(), flags, encoding.c_str());
        यदि(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        प्रत्यागम ok;
    }
    गृहाण (...)
    {
        release();
        क्षिप;  // re-throw
    }
}

तार्किक FileStorage::isOpened() ध्रुव { प्रत्यागम p->is_opened; }

शून्य FileStorage::release()
{
    p->release();
}

File_ग्रन्थिः FileStorage::मूलम्(पूर्णाङ्क i) ध्रुव
{
    यदि( p.empty() || p->roots.empty() || i < 0 || i >= (पूर्णाङ्क)p->roots.size() )
        प्रत्यागम File_ग्रन्थिः();

    प्रत्यागम p->roots[i];
}

File_ग्रन्थिः FileStorage::get_First_शिखरम्_Level_ग्रन्थिः() ध्रुव
{
    File_ग्रन्थिः r = मूलम्();
    File_ग्रन्थिः_Iterator it = r.begin();
    प्रत्यागम it != r.end() ? *it : File_ग्रन्थिः();
}

std::string FileStorage::getDefaultObjectName(ध्रुव std::string& _filename)
{
    स्थिर ध्रुव वर्ण* stubname = "unnamed";
    ध्रुव वर्ण* filename = _filename.c_str();
    ध्रुव वर्ण* ptr2 = filename + _filename.size();
    ध्रुव वर्ण* ptr = ptr2 - 1;
    cv::Auto_बफर<वर्ण> name_buf(_filename.size()+1);

    यावत्( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        यदि( *ptr == '.' && (!*ptr2 || वाक्ये_तुलय(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    यदि( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    वर्ण* name = name_buf.दत्त();

    // name must start with letter or '_'
    यदि( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    यावत्( ptr < ptr2 )
    {
        वर्ण c = *ptr++;
        यदि( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.दत्त();
    यदि( वाक्यं_तुलय( name, "_" ) == 0 )
        वाक्यं_प्रतिलिख( name, stubname );
    प्रत्यागम name;
}


पूर्णाङ्क FileStorage::getFormat() ध्रुव
{
    प्रत्यागम p->fmt;
}

File_ग्रन्थिः FileStorage::संक्रियकः [](ध्रुव वर्ण* कुञ्चिका) ध्रुव
{
    प्रत्यागम एतत्->संक्रियकः[](std::string(कुञ्चिका));
}

File_ग्रन्थिः FileStorage::संक्रियकः [](ध्रुव std::string& कुञ्चिका) ध्रुव
{
    File_ग्रन्थिः res;
    प्रति (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][कुञ्चिका];
        यदि (!res.empty())
            भङ्ग;
    }
    प्रत्यागम res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    प्रत्यागम buf;
}

शून्य FileStorage::writeRaw( ध्रुव String& fmt, ध्रुव शून्य* vec, size_t len )
{
    p->write_Raw_दत्त(fmt, (ध्रुव uchar*)vec, len);
}

शून्य FileStorage::writeComment( ध्रुव String& comment, तार्किक eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

शून्य writeScalar( FileStorage& fs, पूर्णाङ्क value )
{
    fs.p->लिख(String(), value);
}

शून्य writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->लिख(String(), value);
}

शून्य writeScalar( FileStorage& fs, भिन्न value )
{
    fs.p->लिख(String(), (द्विगुण)value);
}

शून्य writeScalar( FileStorage& fs, द्विगुण value )
{
    fs.p->लिख(String(), value);
}

शून्य writeScalar( FileStorage& fs, ध्रुव String& value )
{
    fs.p->लिख(String(), value);
}

शून्य लिख( FileStorage& fs, ध्रुव String& name, पूर्णाङ्क value )
{
    fs.p->लिख(name, value);
}

शून्य लिख( FileStorage& fs, ध्रुव String& name, int64_t value )
{
    fs.p->लिख(name, value);
}

शून्य लिख( FileStorage& fs, ध्रुव String& name, भिन्न value )
{
    fs.p->लिख(name, (द्विगुण)value);
}

शून्य लिख( FileStorage& fs, ध्रुव String& name, द्विगुण value )
{
    fs.p->लिख(name, value);
}

शून्य लिख( FileStorage& fs, ध्रुव String& name, ध्रुव String& value )
{
    fs.p->लिख(name, value);
}

शून्य लिख( FileStorage& fs, ध्रुव String& name, तार्किक value )
{
    fs.p->लिख(name, value);
}

शून्य FileStorage::लिख(ध्रुव String& name, पूर्णाङ्क val) { p->लिख(name, val); }
शून्य FileStorage::लिख(ध्रुव String& name, तार्किक val) { p->लिख(name, val); }
शून्य FileStorage::लिख(ध्रुव String& name, int64_t val) { p->लिख(name, val); }
शून्य FileStorage::लिख(ध्रुव String& name, द्विगुण val) { p->लिख(name, val); }
शून्य FileStorage::लिख(ध्रुव String& name, ध्रुव String& val) { p->लिख(name, val); }
शून्य FileStorage::लिख(ध्रुव String& name, ध्रुव Mat& val) { cv::लिख(*एतत्, name, val); }
शून्य FileStorage::लिख(ध्रुव String& name, ध्रुव std::vector<String>& val) { cv::लिख(*एतत्, name, val); }

FileStorage& संक्रियकः << (FileStorage& fs, ध्रुव String& str)
{
    गणना { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        मानम्_EXPECTED = FileStorage::मानम्_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    ध्रुव वर्ण* _str = str.c_str();
    यदि( !fs.isOpened() || !_str )
        प्रत्यागम fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    वर्ण c = *_str;

    यदि( c == '}' || c == ']' )
    {
        यदि( fs_impl->write_राशिः.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        पूर्णाङ्क struct_flags = fs_impl->write_राशिः.back().flags;
        वर्ण expected_bracket = File_ग्रन्थिः::isMap(struct_flags) ? '}' : ']';
        यदि( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->अन्तः_Write_Struct();
        CV_Assert(!fs_impl->write_राशिः.empty());
        struct_flags = fs_impl->write_राशिः.back().flags;
        fs.state = File_ग्रन्थिः::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : मानम्_EXPECTED;
        fs.elname = String();
    }
    अन्यथा यदि( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        यदि (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = मानम्_EXPECTED + INSIDE_MAP;
    }
    अन्यथा यदि( (fs.state & 3) == मानम्_EXPECTED )
    {
        यदि( c == '{' || c == '[' )
        {
            पूर्णाङ्क struct_flags = c == '{' ? File_ग्रन्थिः::MAP : File_ग्रन्थिः::SEQ;
            fs.state = struct_flags == File_ग्रन्थिः::MAP ? INSIDE_MAP + NAME_EXPECTED : मानम्_EXPECTED;
            _str++;
            यदि( *_str == ':' )
            {
                _str++;
                यदि( !*_str )
                    struct_flags |= File_ग्रन्थिः::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        अन्यथा
        {
            लिख( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            यदि( fs.state == INSIDE_MAP + मानम्_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    अन्यथा
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    प्रत्यागम fs;
}


File_ग्रन्थिः::File_ग्रन्थिः()
    : fs(रिक्त)
{
    खण्डम्_Idx = ofs = 0;
}

File_ग्रन्थिः::File_ग्रन्थिः(FileStorage::Impl* _fs, size_t खण्डम्_Idx_2, size_t _ofs)
    : fs(_fs)
{
    खण्डम्_Idx = खण्डम्_Idx_2;
    ofs = _ofs;
}

File_ग्रन्थिः::File_ग्रन्थिः(ध्रुव FileStorage* _fs, size_t खण्डम्_Idx_2, size_t _ofs)
    : File_ग्रन्थिः(_fs->p.get(), खण्डम्_Idx_2, _ofs)
{
    // nothing
}

File_ग्रन्थिः::File_ग्रन्थिः(ध्रुव File_ग्रन्थिः& ग्रन्थिः)
{
    fs = ग्रन्थिः.fs;
    खण्डम्_Idx = ग्रन्थिः.खण्डम्_Idx;
    ofs = ग्रन्थिः.ofs;
}

File_ग्रन्थिः& File_ग्रन्थिः::संक्रियकः=(ध्रुव File_ग्रन्थिः& ग्रन्थिः)
{
    fs = ग्रन्थिः.fs;
    खण्डम्_Idx = ग्रन्थिः.खण्डम्_Idx;
    ofs = ग्रन्थिः.ofs;
    प्रत्यागम *एतत्;
}

File_ग्रन्थिः File_ग्रन्थिः::संक्रियकः[](ध्रुव std::string& nodename) ध्रुव
{
    यदि(!fs)
        प्रत्यागम File_ग्रन्थिः();

    CV_Assert( isMap() );

    अचिह्नित कुञ्चिका = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_ग्रन्थिः_Iterator it = begin();

    प्रति( i = 0; i < sz; i++, ++it )
    {
        File_ग्रन्थिः n = *it;
        ध्रुव uchar* p = n.ptr();
        अचिह्नित key2 = (अचिह्नित)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_दत्त.size() );
        यदि( कुञ्चिका == key2 )
            प्रत्यागम n;
    }
    प्रत्यागम File_ग्रन्थिः();
}

File_ग्रन्थिः File_ग्रन्थिः::संक्रियकः[](ध्रुव वर्ण* nodename) ध्रुव
{
    प्रत्यागम एतत्->संक्रियकः[](std::string(nodename));
}

File_ग्रन्थिः File_ग्रन्थिः::संक्रियकः[](पूर्णाङ्क i) ध्रुव
{
    यदि(!fs)
        प्रत्यागम File_ग्रन्थिः();

    CV_Assert( isSeq() );

    पूर्णाङ्क sz = (पूर्णाङ्क)size();
    CV_Assert( 0 <= i && i < sz );

    File_ग्रन्थिः_Iterator it = begin();
    it += i;

    प्रत्यागम *it;
}

std::vector<String> File_ग्रन्थिः::कुञ्चिकाः() ध्रुव
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    प्रति (File_ग्रन्थिः_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    प्रत्यागम res;
}

पूर्णाङ्क File_ग्रन्थिः::type() ध्रुव
{
    ध्रुव uchar* p = ptr();
    यदि(!p)
        प्रत्यागम NONE;
    प्रत्यागम (*p & TYPE_आवरणम्);
}

तार्किक File_ग्रन्थिः::isMap(पूर्णाङ्क flags) { प्रत्यागम (flags & TYPE_आवरणम्) == MAP; }
तार्किक File_ग्रन्थिः::isSeq(पूर्णाङ्क flags) { प्रत्यागम (flags & TYPE_आवरणम्) == SEQ; }
तार्किक File_ग्रन्थिः::isCollection(पूर्णाङ्क flags) { प्रत्यागम isMap(flags) || isSeq(flags); }
तार्किक File_ग्रन्थिः::isFlow(पूर्णाङ्क flags) { प्रत्यागम (flags & FLOW) != 0; }
तार्किक File_ग्रन्थिः::is_रिक्तानि_Collection(पूर्णाङ्क flags) { प्रत्यागम (flags & रिक्तानि_2) != 0; }

तार्किक File_ग्रन्थिः::empty() ध्रुव   { प्रत्यागम fs == 0; }
तार्किक File_ग्रन्थिः::isNone() ध्रुव  { प्रत्यागम type() == NONE; }
तार्किक File_ग्रन्थिः::isSeq() ध्रुव   { प्रत्यागम type() == SEQ; }
तार्किक File_ग्रन्थिः::isMap() ध्रुव   { प्रत्यागम type() == MAP; }
तार्किक File_ग्रन्थिः::isInt() ध्रुव   { प्रत्यागम type() == INT;  }
तार्किक File_ग्रन्थिः::isReal() ध्रुव  { प्रत्यागम type() == REAL; }
तार्किक File_ग्रन्थिः::isString() ध्रुव { प्रत्यागम type() == STRING;  }
तार्किक File_ग्रन्थिः::isNamed() ध्रुव
{
    ध्रुव uchar* p = ptr();
    यदि(!p)
        प्रत्यागम असत्य;
    प्रत्यागम (*p & NAMED) != 0;
}

std::string File_ग्रन्थिः::name() ध्रुव
{
    ध्रुव uchar* p = ptr();
    यदि(!p)
        प्रत्यागम std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    प्रत्यागम fs->getName(nameofs);
}

File_ग्रन्थिः::संक्रियकः पूर्णाङ्क() ध्रुव
{
    ध्रुव uchar* p = ptr();
    यदि(!p)
        प्रत्यागम 0;
    पूर्णाङ्क चिह्नकम् = *p;
    पूर्णाङ्क type = (चिह्नकम् & TYPE_आवरणम्);
    p += (चिह्नकम् & NAMED) ? 5 : 1;

    यदि( type == INT )
    {
        प्रत्यागम readInt(p);
    }
    अन्यथा यदि( type == REAL )
    {
        प्रत्यागम cv_आवर्तनम्(readReal(p));
    }
    अन्यथा
        प्रत्यागम 0x7fffffff;
}

File_ग्रन्थिः::संक्रियकः int64_t() ध्रुव
{
    ध्रुव uchar* p = ptr();
    यदि(!p)
        प्रत्यागम 0;
    पूर्णाङ्क चिह्नकम् = *p;
    पूर्णाङ्क type = (चिह्नकम् & TYPE_आवरणम्);
    p += (चिह्नकम् & NAMED) ? 5 : 1;

    यदि( type == INT )
    {
        प्रत्यागम readLong(p);
    }
    अन्यथा यदि( type == REAL )
    {
        प्रत्यागम cv_आवर्तनम्(readReal(p));
    }
    अन्यथा
        प्रत्यागम 0x7fffffff;
}

File_ग्रन्थिः::संक्रियकः भिन्न() ध्रुव
{
    ध्रुव uchar* p = ptr();
    यदि(!p)
        प्रत्यागम 0.f;
    पूर्णाङ्क चिह्नकम् = *p;
    पूर्णाङ्क type = (चिह्नकम् & TYPE_आवरणम्);
    p += (चिह्नकम् & NAMED) ? 5 : 1;

    यदि( type == INT )
    {
        प्रत्यागम (भिन्न)readLong(p);
    }
    अन्यथा यदि( type == REAL )
    {
        प्रत्यागम (भिन्न)readReal(p);
    }
    अन्यथा
        प्रत्यागम FLT_MAX;
}

File_ग्रन्थिः::संक्रियकः द्विगुण() ध्रुव
{
    ध्रुव uchar* p = ptr();
    यदि(!p)
        प्रत्यागम 0.f;
    पूर्णाङ्क चिह्नकम् = *p;
    पूर्णाङ्क type = (चिह्नकम् & TYPE_आवरणम्);
    p += (चिह्नकम् & NAMED) ? 5 : 1;

    यदि( type == INT )
    {
        प्रत्यागम (द्विगुण)readLong(p);
    }
    अन्यथा यदि( type == REAL )
    {
        प्रत्यागम readReal(p);
    }
    अन्यथा
        प्रत्यागम DBL_MAX;
}

द्विगुण File_ग्रन्थिः::real() ध्रुव  { प्रत्यागम द्विगुण(*एतत्); }

std::string File_ग्रन्थिः::string() ध्रुव
{
    ध्रुव uchar* p = ptr();
    यदि( !p || (*p & TYPE_आवरणम्) != STRING )
        प्रत्यागम std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(अचिह्नित)readInt(p);
    प्रत्यागम std::string((ध्रुव वर्ण*)(p + 4), sz - 1);
}
Mat File_ग्रन्थिः::mat() ध्रुव { Mat value; पठ(*एतत्, value, Mat()); प्रत्यागम value; }

File_ग्रन्थिः_Iterator File_ग्रन्थिः::begin() ध्रुव { प्रत्यागम File_ग्रन्थिः_Iterator(*एतत्, असत्य); }
File_ग्रन्थिः_Iterator File_ग्रन्थिः::end() ध्रुव   { प्रत्यागम File_ग्रन्थिः_Iterator(*एतत्, सत्य); }

शून्य File_ग्रन्थिः::readRaw( ध्रुव std::string& fmt, शून्य* vec, size_t len ) ध्रुव
{
    File_ग्रन्थिः_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_ग्रन्थिः::size() ध्रुव
{
    ध्रुव uchar* p = ptr();
    यदि( !p )
        प्रत्यागम 0;
    पूर्णाङ्क चिह्नकम् = *p;
    पूर्णाङ्क tp = चिह्नकम् & TYPE_आवरणम्;
    यदि( tp == MAP || tp == SEQ )
    {
        यदि( चिह्नकम् & NAMED )
            p += 4;
        प्रत्यागम (size_t)(अचिह्नित)readInt(p + 5);
    }
    प्रत्यागम tp != NONE;
}

size_t File_ग्रन्थिः::raw_आकार() ध्रुव
{
    ध्रुव uchar* p0 = ptr(), *p = p0;
    यदि( !p )
        प्रत्यागम 0;
    पूर्णाङ्क चिह्नकम् = *p++;
    पूर्णाङ्क tp = चिह्नकम् & TYPE_आवरणम्;
    यदि( चिह्नकम् & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    यदि( tp == INT )
        प्रत्यागम sz0 + 8;
    यदि( tp == REAL )
        प्रत्यागम sz0 + 8;
    यदि( tp == NONE )
        प्रत्यागम sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    प्रत्यागम sz0 + 4 + readInt(p);
}

uchar* File_ग्रन्थिः::ptr()
{
    प्रत्यागम !fs ? 0 : (uchar*)fs->get_ग्रन्थिः_Ptr(खण्डम्_Idx, ofs);
}

ध्रुव uchar* File_ग्रन्थिः::ptr() ध्रुव
{
    प्रत्यागम !fs ? 0 : fs->get_ग्रन्थिः_Ptr(खण्डम्_Idx, ofs);
}

शून्य File_ग्रन्थिः::समुच्चयः_मानम्( पूर्णाङ्क type, ध्रुव शून्य* value, पूर्णाङ्क len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    पूर्णाङ्क चिह्नकम् = *p;
    पूर्णाङ्क current_type = चिह्नकम् & TYPE_आवरणम्;
    CV_Assert( current_type == NONE || current_type == type );

    पूर्णाङ्क sz = 1;

    यदि( चिह्नकम् & NAMED )
        sz += 4;

    यदि( type == INT )
        sz += 8;
    अन्यथा यदि( type == REAL )
        sz += 8;
    अन्यथा यदि( type == STRING )
    {
        यदि( len < 0 )
            len = (पूर्णाङ्क)वाक्यदैर्घ्यम्((ध्रुव वर्ण*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    अन्यथा
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_ग्रन्थिः_Space(*एतत्, sz);
    *p++ = (uchar)(type | (चिह्नकम् & NAMED));
    यदि( चिह्नकम् & NAMED )
        p += 4;

    यदि( type == INT )
    {
        int64_t ival = *(ध्रुव int64_t*)value;
        writeInt(p, ival);
    }
    अन्यथा यदि( type == REAL )
    {
        द्विगुण dbval = *(ध्रुव द्विगुण*)value;
        writeReal(p, dbval);
    }
    अन्यथा यदि( type == STRING )
    {
        ध्रुव वर्ण* str = (ध्रुव वर्ण*)value;
        writeInt(p, len + 1);
        स्मृतिं_प्रतिलिख(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_ग्रन्थिः_Iterator::File_ग्रन्थिः_Iterator()
{
    fs = 0;
    खण्डम्_Idx = 0;
    ofs = 0;
    खण्डम्_आकार_2 = 0;
    ग्रन्थिः_NElems = 0;
    idx = 0;
}

File_ग्रन्थिः_Iterator::File_ग्रन्थिः_Iterator( ध्रुव File_ग्रन्थिः& ग्रन्थिः, तार्किक seek_अन्तः )
{
    fs = ग्रन्थिः.fs;
    idx = 0;
    यदि( !fs )
        खण्डम्_Idx = ofs = खण्डम्_आकार_2 = ग्रन्थिः_NElems = 0;
    अन्यथा
    {
        खण्डम्_Idx = ग्रन्थिः.खण्डम्_Idx;
        ofs = ग्रन्थिः.ofs;

        तार्किक collection = ग्रन्थिः.isSeq() || ग्रन्थिः.isMap();
        यदि( ग्रन्थिः.isNone() )
        {
            ग्रन्थिः_NElems = 0;
        }
        अन्यथा यदि( !collection )
        {
            ग्रन्थिः_NElems = 1;
            यदि( seek_अन्तः )
            {
                idx = 1;
                ofs += ग्रन्थिः.raw_आकार();
            }
        }
        अन्यथा
        {
            ग्रन्थिः_NElems = ग्रन्थिः.size();
            ध्रुव uchar* p0 = ग्रन्थिः.ptr(), *p = p0 + 1;
            यदि(*p0 & File_ग्रन्थिः::NAMED )
                p += 4;
            यदि( !seek_अन्तः )
                ofs += (p - p0) + 8;
            अन्यथा
            {
                size_t rawsz = (size_t)(अचिह्नित)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = ग्रन्थिः_NElems;
            }
        }
        fs->normalize_ग्रन्थिः_Ofs(खण्डम्_Idx, ofs);
        खण्डम्_आकार_2 = fs->fs_दत्त_blksz[खण्डम्_Idx];
    }
}

File_ग्रन्थिः_Iterator::File_ग्रन्थिः_Iterator(ध्रुव File_ग्रन्थिः_Iterator& it)
{
    fs = it.fs;
    खण्डम्_Idx = it.खण्डम्_Idx;
    ofs = it.ofs;
    खण्डम्_आकार_2 = it.खण्डम्_आकार_2;
    ग्रन्थिः_NElems = it.ग्रन्थिः_NElems;
    idx = it.idx;
}

File_ग्रन्थिः_Iterator& File_ग्रन्थिः_Iterator::संक्रियकः=(ध्रुव File_ग्रन्थिः_Iterator& it)
{
    fs = it.fs;
    खण्डम्_Idx = it.खण्डम्_Idx;
    ofs = it.ofs;
    खण्डम्_आकार_2 = it.खण्डम्_आकार_2;
    ग्रन्थिः_NElems = it.ग्रन्थिः_NElems;
    idx = it.idx;
    प्रत्यागम *एतत्;
}

File_ग्रन्थिः File_ग्रन्थिः_Iterator::संक्रियकः *() ध्रुव
{
    प्रत्यागम File_ग्रन्थिः(idx < ग्रन्थिः_NElems ? fs : रिक्त, खण्डम्_Idx, ofs);
}

File_ग्रन्थिः_Iterator& File_ग्रन्थिः_Iterator::संक्रियकः ++ ()
{
    यदि( idx == ग्रन्थिः_NElems || !fs )
        प्रत्यागम *एतत्;
    idx++;
    File_ग्रन्थिः n(fs, खण्डम्_Idx, ofs);
    ofs += n.raw_आकार();
    यदि( ofs >= खण्डम्_आकार_2 )
    {
        fs->normalize_ग्रन्थिः_Ofs(खण्डम्_Idx, ofs);
        खण्डम्_आकार_2 = fs->fs_दत्त_blksz[खण्डम्_Idx];
    }
    प्रत्यागम *एतत्;
}

File_ग्रन्थिः_Iterator File_ग्रन्थिः_Iterator::संक्रियकः ++ (पूर्णाङ्क)
{
    File_ग्रन्थिः_Iterator it = *एतत्;
    ++(*एतत्);
    प्रत्यागम it;
}

File_ग्रन्थिः_Iterator& File_ग्रन्थिः_Iterator::संक्रियकः += (पूर्णाङ्क _ofs)
{
    CV_Assert( _ofs >= 0 );
    प्रति( ; _ofs > 0; _ofs-- )
        एतत्->संक्रियकः ++();
    प्रत्यागम *एतत्;
}

File_ग्रन्थिः_Iterator& File_ग्रन्थिः_Iterator::readRaw( ध्रुव String& fmt, शून्य* _data0, size_t maxsz)
{
    यदि( fs && idx < ग्रन्थिः_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        पूर्णाङ्क fmt_युग्मानि[CV_FS_MAX_FMT_युग्मानि*2];
        पूर्णाङ्क fmt_pair_गणनम् = fs::decodeFormat( fmt.c_str(), fmt_युग्मानि, CV_FS_MAX_FMT_युग्मानि );
        size_t esz = fs::calc_Struct_आकार( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        प्रति( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            प्रति( पूर्णाङ्क k = 0; k < fmt_pair_गणनम्; k++ )
            {
                पूर्णाङ्क elem_type = fmt_युग्मानि[k*2+1];
                पूर्णाङ्क elem_आकार_2 = CV_ELEM_आकार(elem_type);

                पूर्णाङ्क गणनम्_2 = fmt_युग्मानि[k*2];
                offset = align_आकार( offset, elem_आकार_2 );
                uchar* दत्त = data0 + offset;

                प्रति( पूर्णाङ्क i = 0; i < गणनम्_2; i++, ++(*एतत्) )
                {
                    File_ग्रन्थिः ग्रन्थिः = *(*एतत्);
                    यदि( ग्रन्थिः.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_आकार_2 == 8 ? (int64_t)ग्रन्थिः : (पूर्णाङ्क)ग्रन्थिः);
                        विकल्प( elem_type )
                        {
                        पक्ष CV_8U:
                            *(uchar*)दत्त = saturate_cast<uchar>(ival);
                            दत्त++;
                            भङ्ग;
                        पक्ष CV_8S:
                            *(वर्ण*)दत्त = saturate_cast<schar>(ival);
                            दत्त++;
                            भङ्ग;
                        पक्ष CV_Bool:
                            *(तार्किक*)दत्त = ival != 0;
                            दत्त++;
                            भङ्ग;
                        पक्ष CV_16U:
                            *(ushort*)दत्त = saturate_cast<ushort>(ival);
                            दत्त += परिमाण(ushort);
                            भङ्ग;
                        पक्ष CV_16S:
                            *(ह्रस्व*)दत्त = saturate_cast<ह्रस्व>(ival);
                            दत्त += परिमाण(ह्रस्व);
                            भङ्ग;
                        पक्ष CV_32U:
                            *(अचिह्नित*)दत्त = (अचिह्नित)std::max(ival, (int64_t)0);
                            दत्त += परिमाण(अचिह्नित);
                            भङ्ग;
                        पक्ष CV_32S:
                            *(पूर्णाङ्क*)दत्त = (पूर्णाङ्क)ival;
                            दत्त += परिमाण(पूर्णाङ्क);
                            भङ्ग;
                        पक्ष CV_32F:
                            *(भिन्न*)दत्त = (भिन्न)ival;
                            दत्त += परिमाण(भिन्न);
                            भङ्ग;
                        पक्ष CV_64U:
                            *(uint64_t*)दत्त = (uint64_t)ival;
                            दत्त += परिमाण(uint64_t);
                            भङ्ग;
                        पक्ष CV_64S:
                            *(int64_t*)दत्त = (int64_t)ival;
                            दत्त += परिमाण(int64_t);
                            भङ्ग;
                        पक्ष CV_64F:
                            *(द्विगुण*)दत्त = (द्विगुण)ival;
                            दत्त += परिमाण(द्विगुण);
                            भङ्ग;
                        पक्ष CV_16F:
                            *(hfloat*)दत्त = hfloat((भिन्न)ival);
                            दत्त += परिमाण(hfloat);
                            भङ्ग;
                        पक्ष CV_16BF:
                            *(bfloat*)दत्त = bfloat((भिन्न)ival);
                            दत्त += परिमाण(bfloat);
                            भङ्ग;
                        पक्ष CV_8F_E4M3FN:
                            *(fp8_t*)दत्त = fp8_t((भिन्न)ival);
                            दत्त += परिमाण(fp8_t);
                            भङ्ग;
                        पक्ष CV_8F_E4M3FNUZ:
                            *(fp8a_t*)दत्त = fp8a_t((भिन्न)ival);
                            दत्त += परिमाण(fp8a_t);
                            भङ्ग;
                        स्वभाव:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    अन्यथा यदि( ग्रन्थिः.isReal() )
                    {
                        द्विगुण fval = (द्विगुण)ग्रन्थिः;

                        विकल्प( elem_type )
                        {
                        पक्ष CV_8U:
                            *(uchar*)दत्त = saturate_cast<uchar>(fval);
                            दत्त++;
                            भङ्ग;
                        पक्ष CV_8S:
                            *(वर्ण*)दत्त = saturate_cast<schar>(fval);
                            दत्त++;
                            भङ्ग;
                        पक्ष CV_16U:
                            *(ushort*)दत्त = saturate_cast<ushort>(fval);
                            दत्त += परिमाण(ushort);
                            भङ्ग;
                        पक्ष CV_16S:
                            *(ह्रस्व*)दत्त = saturate_cast<ह्रस्व>(fval);
                            दत्त += परिमाण(ह्रस्व);
                            भङ्ग;
                        पक्ष CV_32U:
                            *(पूर्णाङ्क*)दत्त = saturate_cast<अचिह्नित>(fval);
                            दत्त += परिमाण(पूर्णाङ्क);
                            भङ्ग;
                        पक्ष CV_32S:
                            *(पूर्णाङ्क*)दत्त = saturate_cast<पूर्णाङ्क>(fval);
                            दत्त += परिमाण(पूर्णाङ्क);
                            भङ्ग;
                        पक्ष CV_32F:
                            *(भिन्न*)दत्त = (भिन्न)fval;
                            दत्त += परिमाण(भिन्न);
                            भङ्ग;
                        पक्ष CV_64U:
                            *(uint64_t*)दत्त = (uint64_t)round(std::max(fval, 0.));
                            दत्त += परिमाण(uint64_t);
                            भङ्ग;
                        पक्ष CV_64S:
                            *(int64_t*)दत्त = (int64_t)round(std::max(fval, 0.));
                            दत्त += परिमाण(int64_t);
                            भङ्ग;
                        पक्ष CV_64F:
                            *(द्विगुण*)दत्त = fval;
                            दत्त += परिमाण(द्विगुण);
                            भङ्ग;
                        पक्ष CV_16F:
                            *(hfloat*)दत्त = hfloat((भिन्न)fval);
                            दत्त += परिमाण(hfloat);
                            भङ्ग;
                        पक्ष CV_16BF:
                            *(bfloat*)दत्त = bfloat((भिन्न)fval);
                            दत्त += परिमाण(bfloat);
                            भङ्ग;
                        पक्ष CV_8F_E4M3FN:
                            *(fp8_t*)दत्त = fp8_t((भिन्न)fval);
                            दत्त += परिमाण(fp8_t);
                            भङ्ग;
                        पक्ष CV_8F_E4M3FNUZ:
                            *(fp8a_t*)दत्त = fp8a_t((भिन्न)fval);
                            दत्त += परिमाण(fp8a_t);
                            भङ्ग;
                        स्वभाव:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    अन्यथा
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (पूर्णाङ्क)(दत्त - data0);
            }
        }
    }

    प्रत्यागम *एतत्;
}

तार्किक File_ग्रन्थिः_Iterator::equal_गन्तव्यम्(ध्रुव File_ग्रन्थिः_Iterator& it) ध्रुव
{
    प्रत्यागम fs == it.fs && खण्डम्_Idx == it.खण्डम्_Idx && ofs == it.ofs &&
           idx == it.idx && ग्रन्थिः_NElems == it.ग्रन्थिः_NElems;
}

size_t File_ग्रन्थिः_Iterator::remaining() ध्रुव
{
    प्रत्यागम ग्रन्थिः_NElems - idx;
}

तार्किक संक्रियकः == ( ध्रुव File_ग्रन्थिः_Iterator& it1, ध्रुव File_ग्रन्थिः_Iterator& it2 )
{
    प्रत्यागम it1.equal_गन्तव्यम्(it2);
}

तार्किक संक्रियकः != ( ध्रुव File_ग्रन्थिः_Iterator& it1, ध्रुव File_ग्रन्थिः_Iterator& it2 )
{
    प्रत्यागम !it1.equal_गन्तव्यम्(it2);
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, पूर्णाङ्क& val, पूर्णाङ्क default_val)
{
    val = default_val;
    यदि( !ग्रन्थिः.empty() )
    {
        val = (पूर्णाङ्क)ग्रन्थिः;
    }
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, int64_t& val, int64_t default_val)
{
    val = default_val;
    यदि( !ग्रन्थिः.empty() )
    {
        val = (int64_t)ग्रन्थिः;
    }
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, द्विगुण& val, द्विगुण default_val)
{
    val = default_val;
    यदि( !ग्रन्थिः.empty() )
    {
        val = (द्विगुण)ग्रन्थिः;
    }
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, भिन्न& val, भिन्न default_val)
{
    val = default_val;
    यदि( !ग्रन्थिः.empty() )
    {
        val = (भिन्न)ग्रन्थिः;
    }
}

शून्य पठ(ध्रुव File_ग्रन्थिः& ग्रन्थिः, std::string& val, ध्रुव std::string& default_val)
{
    val = default_val;
    यदि( !ग्रन्थिः.empty() )
    {
        val = (std::string)ग्रन्थिः;
    }
}
शून्य FileStorage::Impl::लिख(ध्रुव String &कुञ्चिका, तार्किक value)
{
    CV_Assert(write_बहुलकम्);
    यदि (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().लिख(कुञ्चिका.c_str(), (पूर्णाङ्क)value);
    }
    अन्यथा
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().लिख(कुञ्चिका.c_str(), value ? "true" : "false", असत्य);
    }
}

FileStorage_API::~FileStorage_API() {}

नामाकाश internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, ध्रुव std::string& name,
                                       पूर्णाङ्क flags, ध्रुव std::string& typeName)
{
    fs = &_fs;
    fs->startWriteStruct(name, flags, typeName);
}

WriteStructContext::~WriteStructContext()
{
    fs->अन्तः_Write_Struct();
}

}

}
