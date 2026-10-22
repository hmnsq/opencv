// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "persistence.hpp"
#అన్తర్భావ "persistence_impl.hpp"
#అన్తర్భావ "persistence_base64_encoding.hpp"
#అన్తర్భావ <unordered_map>
#అన్తర్భావ <iterator>

#అన్తర్భావ <opencv2/core/utils/logger.hpp>

#నిర్వచించు CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

నామాకాశ cv
{

నామాకాశ fs
{

పూర్ణాంకం strcasecmp(స్థిరరాశి అక్షరం* s1, స్థిరరాశి అక్షరం* s2)
{
    స్థిరరాశి అక్షరం* dummy="";
    ఒకవేళ(!s1) s1=dummy;
    ఒకవేళ(!s2) s2=dummy;

    size_t len1 = అక్షరశ్రేణి_పొడవు(s1);
    size_t len2 = అక్షరశ్రేణి_పొడవు(s2);
    size_t i, len = std::min(len1, len2);
    కొరకు( i = 0; i < len; i++ )
    {
        పూర్ణాంకం d = tolower((పూర్ణాంకం)s1[i]) - tolower((పూర్ణాంకం)s2[i]);
        ఒకవేళ( d != 0 )
            తిరిగి d;
    }
    తిరిగి len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

అక్షరం* itoa( పూర్ణాంకం _val, అక్షరం* బఫరు, పూర్ణాంకం /*radix*/ )
{
    స్థిరరాశి పూర్ణాంకం radix = 10;
    అక్షరం* ptr=బఫరు + 23 /* enough even for 64-bit integers */;
    గుర్తులేని val = నిరపేక్ష(_val);

    *ptr = '\0';
    చేయి
    {
        గుర్తులేని r = val / radix;
        *--ptr = (అక్షరం)(val - (r*radix) + '0');
        val = r;
    }
    అయితే( val != 0 );

    ఒకవేళ( _val < 0 )
        *--ptr = '-';

    తిరిగి ptr;
}

అక్షరం* itoa( int64_t _val, అక్షరం* బఫరు, పూర్ణాంకం /*radix*/, తార్కిక _signed)
{
    స్థిరరాశి పూర్ణాంకం radix = 10;
    అక్షరం* ptr=బఫరు + 23 /* enough even for 64-bit integers */;
    పూర్ణాంకం sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : నిరపేక్ష(_val);

    *ptr = '\0';
    చేయి
    {
        uint64_t r = val / radix;
        *--ptr = (అక్షరం)(val - (r*radix) + '0');
        val = r;
    }
    అయితే( val != 0 );

    ఒకవేళ( sign < 0 )
        *--ptr = '-';

    తిరిగి ptr;
}

అక్షరం* double_గమ్యం_String( అక్షరం* buf, size_t buf_కొలత, ద్విగుణ value, తార్కిక explicitZero )
{
    Cv64suf val;
    గుర్తులేని ieee754_hi;

    val.f = value;
    ieee754_hi = (గుర్తులేని)(val.u >> 32);

    ఒకవేళ( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        పూర్ణాంకం ivalue = cv_విడత(value);
        ఒకవేళ( ivalue == value )
        {
            ఒకవేళ( explicitZero )
                snprintf( buf, buf_కొలత, "%d.0", ivalue );
            లేకపోతే
                snprintf( buf, buf_కొలత, "%d.", ivalue );
        }
        లేకపోతే
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, buf_కొలత, "%.17g", value );

            అక్షరం* ptr = buf;
            ఒకవేళ( *ptr == '+' || *ptr == '-' )
                ptr++;
            కొరకు( ; cv_isdigit(*ptr); ptr++ )
                ;
            ఒకవేళ( *ptr == ',' )
                *ptr = '.';
        }
    }
    లేకపోతే
    {
        గుర్తులేని ieee754_lo = (గుర్తులేని)val.u;
        ఒకవేళ( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            strcpy( buf, ".Nan" );
        లేకపోతే
            strcpy( buf, (పూర్ణాంకం)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    తిరిగి buf;
}

అక్షరం* float_గమ్యం_String( అక్షరం* buf, size_t buf_కొలత, దశాంశం value, తార్కిక halfprecision, తార్కిక explicitZero )
{
    Cv32suf val;
    గుర్తులేని ieee754;
    val.f = value;
    ieee754 = val.u;

    ఒకవేళ( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        పూర్ణాంకం ivalue = cv_విడత(value);
        ఒకవేళ( ivalue == value )
        {
            ఒకవేళ( explicitZero )
                snprintf( buf, buf_కొలత, "%d.0", ivalue );
            లేకపోతే
                snprintf( buf, buf_కొలత, "%d.", ivalue );
        }
        లేకపోతే
        {
            ఒకవేళ (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, buf_కొలత, "%.5g", value);
            }
            లేకపోతే
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, buf_కొలత, "%.9g", value);
            }

            అక్షరం* ptr = buf;
            ఒకవేళ( *ptr == '+' || *ptr == '-' )
                ptr++;
            కొరకు( ; cv_isdigit(*ptr); ptr++ )
                ;
            ఒకవేళ( *ptr == ',' )
                *ptr = '.';
        }
    }
    లేకపోతే
    {
        ఒకవేళ( (ieee754 & 0x7fffffff) != 0x7f800000 )
            strcpy( buf, ".Nan" );
        లేకపోతే
            strcpy( buf, (పూర్ణాంకం)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    తిరిగి buf;
}

స్థిర స్థిరరాశి అక్షరం symbols[] = "ucwsifdhHbUIneE";

స్థిర అక్షరం type_చిహ్నం(పూర్ణాంకం లోతు)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_Check_లోతు(లోతు, లోతు >= 0 && లోతు <= CV_8F_E4M3FNUZ, "");
    తిరిగి symbols[లోతు];
}

స్థిర పూర్ణాంకం చిహ్నం_గమ్యం_Type(అక్షరం c)
{
    ఒకవేళ (c == 'r')
        తిరిగి CV_SEQ_ELTYPE_PTR;
    స్థిరరాశి అక్షరం* pos = strchr( symbols, c );
    ఒకవేళ( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    తిరిగి static_cast<పూర్ణాంకం>(pos - symbols);
}

అక్షరం* encodeFormat(పూర్ణాంకం elem_type, అక్షరం* dt, size_t dt_len)
{
    పూర్ణాంకం cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    అక్షరం చిహ్నం = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_చిహ్నం(CV_MAT_లోతు(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, చిహ్నం);
    తిరిగి dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
అక్షరం* encodeFormat(పూర్ణాంకం elem_type, అక్షరం* dt)
{
    constexpr size_t max = 20+1+1; // UINT64_MAX + one char + nul termination.
    తిరిగి encodeFormat(elem_type, dt, max);
}

పూర్ణాంకం decodeFormat( స్థిరరాశి అక్షరం* dt, పూర్ణాంకం* fmt_జతలు, పూర్ణాంకం max_len )
{
    పూర్ణాంకం fmt_pair_లెక్క = 0;
    పూర్ణాంకం i = 0, k = 0, len = dt ? (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు(dt) : 0;

    ఒకవేళ( !dt || !len )
        తిరిగి 0;

    CV_Assert( fmt_జతలు != 0 && max_len > 0 );
    fmt_జతలు[0] = 0;
    max_len *= 2;

    కొరకు( ; k < len; k++ )
    {
        అక్షరం c = dt[k];

        ఒకవేళ( cv_isdigit(c) )
        {
            పూర్ణాంకం లెక్క_2 = c - '0';
            ఒకవేళ( cv_isdigit(dt[k+1]) )
            {
                అక్షరం* endptr = 0;
                లెక్క_2 = (పూర్ణాంకం)strtol( dt+k, &endptr, 10 );
                k = (పూర్ణాంకం)(endptr - dt) - 1;
            }

            ఒకవేళ( లెక్క_2 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_జతలు[i] = లెక్క_2;
        }
        లేకపోతే
        {
            పూర్ణాంకం లోతు = చిహ్నం_గమ్యం_Type(c);
            ఒకవేళ( fmt_జతలు[i] == 0 )
                fmt_జతలు[i] = 1;
            fmt_జతలు[i+1] = లోతు;
            ఒకవేళ( i > 0 && fmt_జతలు[i+1] == fmt_జతలు[i-1] )
                fmt_జతలు[i-2] += fmt_జతలు[i];
            లేకపోతే
            {
                i += 2;
                ఒకవేళ( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_జతలు[i] = 0;
        }
    }

    fmt_pair_లెక్క = i/2;
    తిరిగి fmt_pair_లెక్క;
}

పూర్ణాంకం calc_Elem_కొలత( స్థిరరాశి అక్షరం* dt, పూర్ణాంకం initial_కొలత )
{
    పూర్ణాంకం size = 0;
    పూర్ణాంకం fmt_జతలు[CV_FS_MAX_FMT_జతలు*2], i, fmt_pair_లెక్క;
    పూర్ణాంకం comp_కొలత;

    fmt_pair_లెక్క = decodeFormat( dt, fmt_జతలు, CV_FS_MAX_FMT_జతలు );
    fmt_pair_లెక్క *= 2;
    కొరకు( i = 0, size = initial_కొలత; i < fmt_pair_లెక్క; i += 2 )
    {
        comp_కొలత = CV_ELEM_కొలత(fmt_జతలు[i+1]);
        size = cvAlign( size, comp_కొలత );
        size += comp_కొలత * fmt_జతలు[i];
    }
    ఒకవేళ( initial_కొలత == 0 )
    {
        comp_కొలత = CV_ELEM_కొలత(fmt_జతలు[1]);
        size = cvAlign( size, comp_కొలత );
    }
    తిరిగి size;
}


పూర్ణాంకం calc_Struct_కొలత( స్థిరరాశి అక్షరం* dt, పూర్ణాంకం initial_కొలత )
{
    పూర్ణాంకం size = calc_Elem_కొలత( dt, initial_కొలత );
    size_t elem_max_కొలత = 0;
    కొరకు ( స్థిరరాశి అక్షరం * type = dt; *type != '\0'; type++ )
    {
        అక్షరం v = *type;
        ఒకవేళ (v >= '0' && v <= '9')
            కొనసాగు;  // skip vector size
        ఎంపిక (v)
        {
        స్థితి 'u': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(uchar ) ); ఆపు; }
        స్థితి 'b': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(తార్కిక  ) ); ఆపు; }
        స్థితి 'c': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(schar ) ); ఆపు; }
        స్థితి 'w': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(ushort) ); ఆపు; }
        స్థితి 's': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(పొట్టి ) ); ఆపు; }
        స్థితి 'i': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(పూర్ణాంకం   ) ); ఆపు; }
        స్థితి 'n': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(గుర్తులేని) ); ఆపు; }
        స్థితి 'f': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(దశాంశం ) ); ఆపు; }
        స్థితి 'd': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(ద్విగుణ) ); ఆపు; }
        స్థితి 'h': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(hfloat)); ఆపు; }
        స్థితి 'H': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(bfloat)); ఆపు; }
        స్థితి 'e': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(fp8_t) ); ఆపు; }
        స్థితి 'E': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(fp8a_t)); ఆపు; }
        స్థితి 'I': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(int64_t)); ఆపు; }
        స్థితి 'U': { elem_max_కొలత = std::max( elem_max_కొలత, పరిమాణం(uint64_t)); ఆపు; }
        అప్రమేయ:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (అక్షరం)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<పూర్ణాంకం>(elem_max_కొలత) );
    తిరిగి size;
}

పూర్ణాంకం decodeSimpleFormat( స్థిరరాశి అక్షరం* dt )
{
    పూర్ణాంకం elem_type = -1;
    పూర్ణాంకం fmt_జతలు[CV_FS_MAX_FMT_జతలు*2], fmt_pair_లెక్క;

    fmt_pair_లెక్క = decodeFormat( dt, fmt_జతలు, CV_FS_MAX_FMT_జతలు );
    ఒకవేళ( fmt_pair_లెక్క != 1 || fmt_జతలు[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_జతలు[1], fmt_జతలు[0] );

    తిరిగి elem_type;
}

}

#ఒకవేళ defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#నిర్వచించు CV_LITTLE_ENDIAN_MEM_ACCESS 1
#లేకపోతే
#నిర్వచించు CV_LITTLE_ENDIAN_MEM_ACCESS 0
#అయితే_ముగింపు

స్థిర అంతఃవరుస పూర్ణాంకం readInt(స్థిరరాశి uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ఒకవేళ CV_LITTLE_ENDIAN_MEM_ACCESS
    పూర్ణాంకం val;
    జ్ఞాపకం_నకలు(&val, p, పరిమాణం(val));
    తిరిగి val;
#లేకపోతే
    పూర్ణాంకం val = (పూర్ణాంకం)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    తిరిగి val;
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస int64_t readLong(స్థిరరాశి uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ఒకవేళ CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    జ్ఞాపకం_నకలు(&val, p, పరిమాణం(val));
    తిరిగి val;
#లేకపోతే
    గుర్తులేని val0 = (గుర్తులేని)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    గుర్తులేని val1 = (గుర్తులేని)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    తిరిగి val0 | ((int64_t)val1 << 32);
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస ద్విగుణ readReal(స్థిరరాశి uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ఒకవేళ CV_LITTLE_ENDIAN_MEM_ACCESS
    ద్విగుణ val;
    జ్ఞాపకం_నకలు(&val, p, పరిమాణం(val));
    తిరిగి val;
#లేకపోతే
    గుర్తులేని val0 = (గుర్తులేని)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    గుర్తులేని val1 = (గుర్తులేని)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    తిరిగి val.f;
#అయితే_ముగింపు
}

మూస <రకం_పేరు T>
స్థిర అంతఃవరుస శూన్య writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ఒకవేళ CV_LITTLE_ENDIAN_MEM_ACCESS
    జ్ఞాపకం_నకలు(p, &ival, పరిమాణం(ival));
#లేకపోతే
    కొరకు (size_t i = 0, j = 0; i < పరిమాణం(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస శూన్య writeReal(uchar* p, ద్విగుణ fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#ఒకవేళ CV_LITTLE_ENDIAN_MEM_ACCESS
    జ్ఞాపకం_నకలు(p, &fval, పరిమాణం(fval));
#లేకపోతే
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
#అయితే_ముగింపు
}



శూన్య FileStorage::Impl::init() {
    flags = 0;
    బఫరు.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = అబద్ధం;
    state_యొక్క_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = అబద్ధం;
    delayed_struct_కీలకం = శూన్యసూచిక;
    delayed_struct_flags = 0;
    delayed_type_name = శూన్యసూచిక;
    base64_writer = శూన్యసూచిక;
    is_opened = అబద్ధం;
    dummy_eof = అబద్ధం;
    write_బహుళకం = అబద్ధం;
    mem_బహుళకం = అబద్ధం;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    ఖాళీలు_ప్రవాహం = నిజం;

    strbufv.clear();
    strbuf = 0;
    strbufsize = strbufpos = 0;
    roots.clear();

    fs_దత్తాంశం.clear();
    fs_దత్తాంశం_ptrs.clear();
    fs_దత్తాంశం_blksz.clear();
    freeSpaceOfs = 0;

    str_hash.clear();
    str_hash_దత్తాంశం.clear();
    str_hash_దత్తాంశం.resize(1);
    str_hash_దత్తాంశం[0] = '\0';

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

శూన్య FileStorage::Impl::release(String *out) {
    ఒకవేళ (is_opened) {
        ఒకవేళ (out)
            out->clear();
        ఒకవేళ (write_బహుళకం) {
            అయితే (write_దొంతర.size() > 1) {
                చివర_Write_Struct();
            }
            flush();
            ఒకవేళ (fmt == FileStorage::FORMAT_XML)
                puts("</opencv_storage>\n");
            లేకపోతే ఒకవేళ (fmt == FileStorage::FORMAT_JSON)
                puts("}\n");
        }
        ఒకవేళ (mem_బహుళకం && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

శూన్య FileStorage::Impl::analyze_file_name(స్థిరరాశి std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    స్థిర స్థిరరాశి అక్షరం not_file_name = '\n';
    స్థిర స్థిరరాశి అక్షరం parameter_begin = '?';
    స్థిర స్థిరరాశి అక్షరం parameter_separator = '&';

    ఒకవేళ (file_name.find(not_file_name, (size_t) 0) != std::string::npos)
        తిరిగి;

    size_t beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((size_t) 0, beg));

    ఒకవేళ (beg != std::string::npos) {
        size_t end = file_name.size();
        beg++;
        కొరకు (size_t param_beg = beg, param_చివర = beg;
             param_చివర < end;
             param_beg = param_చివర + 1) {
            param_చివర = file_name.find_first_of(parameter_separator, param_beg);
            ఒకవేళ ((param_చివర == std::string::npos || param_చివర != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_చివర - param_beg));
            }
        }
    }
}

తార్కిక FileStorage::Impl::open(స్థిరరాశి అక్షరం *filename_or_buf, పూర్ణాంకం _flags, స్థిరరాశి అక్షరం *encoding) {
    తార్కిక ok = నిజం;
    release();

    తార్కిక append = (_flags & 3) == FileStorage::APPEND;
    mem_బహుళకం = (_flags & FileStorage::జ్ఞాపకం) != 0;

    write_బహుళకం = (_flags & 3) != 0;
    తార్కిక write_base64 = (write_బహుళకం || append) && (_flags & FileStorage::BASE64) != 0;

    తార్కిక isGZ = అబద్ధం;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        ఒకవేళ (!params.empty())
            filename = params[0];

        ఒకవేళ (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_బహుళకం || append);
    }

    ఒకవేళ (filename.size() == 0 && !mem_బహుళకం && !write_బహుళకం)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    ఒకవేళ (mem_బహుళకం && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    ఒకవేళ (!mem_బహుళకం) {
        size_t dot_idx = filename.find_last_of('.');
        అక్షరం compression = '\0';

        ఒకవేళ (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            ఒకవేళ (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                ఒకవేళ (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = నిజం;
                }

                లేకపోతే ఒకవేళ(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = నిజం;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        ఒకవేళ (!isGZ) {
            file = fopen(filename.c_str(), !write_బహుళకం ? "rt" : !append ? "wt" : "a+t");
            ఒకవేళ (!file)
            {
                CV_LOG_ERROR(ఖాళీ, "Can't open file: '" << filename << "' in " << (!write_బహుళకం ? "read" : !append ? "write" : "append") << " mode");
                తిరిగి అబద్ధం;
            }
        } లేకపోతే {
#ఒకవేళ USE_ZLIB
            ఒకవేళ (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            అక్షరం బహుళకం[] = {write_బహుళకం ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), బహుళకం);
            ఒకవేళ (!gzfile)
            {
                CV_LOG_ERROR(ఖాళీ, "Can't open archive: '" << filename << "' mode=" << బహుళకం);
                తిరిగి అబద్ధం;
            }
#లేకపోతే
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#అయితే_ముగింపు
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_దత్తాంశం.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    ఒకవేళ (write_బహుళకం) {
        fmt = flags & FileStorage::FORMAT_ముసుగు;

        ఒకవేళ (mem_బహుళకం)
            outbuf.clear();

        ఒకవేళ (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            స్థిరరాశి అక్షరం *dot_pos = ఖాళీ;
            స్థిరరాశి అక్షరం *dot_pos2 = ఖాళీ;
            // like strrchr() implementation, but save two last positions simultaneously
            కొరకు (స్థిరరాశి అక్షరం *pos = &filename[0]; pos[0] != 0; pos++) {
                ఒకవేళ (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            ఒకవేళ (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != ఖాళీ) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } లేకపోతే ఒకవేళ (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        పూర్ణాంకం buf_కొలత_2 = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        ఒకవేళ (append) {
            fseek(file, 0, SEEK_END);
            ఒకవేళ (ftell(file) == 0)
                append = అబద్ధం;
        }

        write_దొంతర.clear();
        ఖాళీలు_ప్రవాహం = నిజం;
        write_దొంతర.push_back(FStruct_దత్తాంశం("", File_కణుపు::MAP | File_కణుపు::ఖాళీలు_2, 0));
        బఫరు.reserve(buf_కొలత_2 + 1024);
        బఫరు.resize(buf_కొలత_2);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_యొక్క_writing_base64 = FileStorage_API::Base64State::Uncertain;

        ఒకవేళ (fmt == FileStorage::FORMAT_XML) {
            size_t file_కొలత_2 = file ? (size_t) ftell(file) : (size_t) 0;
            ఒకవేళ (!append || file_కొలత_2 == 0) {
                ఒకవేళ (encoding && *encoding != '\0') {
                    ఒకవేళ (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(అక్షరశ్రేణి_పొడవు(encoding) < 1000);
                    అక్షరం buf[1100];
                    snprintf(buf, పరిమాణం(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    puts(buf);
                } లేకపోతే
                    puts("<?xml version=\"1.0\"?>\n");
                puts("<opencv_storage>\n");
            } లేకపోతే {
                పూర్ణాంకం xml_buf_కొలత = 1 << 10;
                అక్షరం substr[] = "</opencv_storage>";
                పూర్ణాంకం చివరి_occurrence = -1;
                xml_buf_కొలత = MIN(xml_buf_కొలత, పూర్ణాంకం(file_కొలత_2));
                fseek(file, -xml_buf_కొలత, SEEK_END);
                // find the last occurrence of </opencv_storage>
                కొరకు (;;) {
                    పూర్ణాంకం పంక్తి_స్థానభ్రంశం = (పూర్ణాంకం) ftell(file);
                    స్థిరరాశి అక్షరం *ptr0 = ఇది->gets(xml_buf_కొలత);
                    స్థిరరాశి అక్షరం *ptr = ఖాళీ;
                    ఒకవేళ (!ptr0)
                        ఆపు;
                    ptr = ptr0;
                    కొరకు (;;) {
                        ptr = అక్షరశ్రేణి_శోధన(ptr, substr);
                        ఒకవేళ (!ptr)
                            ఆపు;
                        చివరి_occurrence = పంక్తి_స్థానభ్రంశం + (పూర్ణాంకం) (ptr - ptr0);
                        ptr += అక్షరశ్రేణి_పొడవు(substr);
                    }
                }
                ఒకవేళ (చివరి_occurrence < 0) {
                    release();
                    CV_Error(cv::Error::StsError, "Could not find </opencv_storage> in the end of file.\n");
                }
                closeFile();
                file = fopen(filename.c_str(), "r+t");
                CV_Assert(file != 0);
                fseek(file, చివరి_occurrence, SEEK_SET);
                // replace the last "</opencv_storage>" with " <!-- resumed -->", which has the same length
                puts(" <!-- resumed -->");
                fseek(file, 0, SEEK_END);
                puts("\n");
            }

            emitter_do_not_use_ప్రత్యక్ష_dereference = createXMLEmitter(ఇది);
        } లేకపోతే ఒకవేళ (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                ఒకవేళ (!append) {
                    ఒకవేళ (fmt == FileStorage::FORMAT_YAML_1_0)
                        puts("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    లేకపోతే
                        puts("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            లేకపోతే
                puts("...\n---\n");

            emitter_do_not_use_ప్రత్యక్ష_dereference = createYAMLEmitter(ఇది);
        } లేకపోతే {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            ఒకవేళ (!append)
                puts("{\n");
            లేకపోతే {
                తార్కిక valid = అబద్ధం;
                దీర్ఘ roffset = 0;
                కొరకు (;
                        fseek(file, roffset, SEEK_END) == 0;
                        roffset -= 1) {
                    స్థిరరాశి అక్షరం చివర_mark = '}';
                    ఒకవేళ (fgetc(file) == చివర_mark) {
                        fseek(file, roffset, SEEK_END);
                        valid = నిజం;
                        ఆపు;
                    }
                }

                ఒకవేళ (valid) {
                    closeFile();
                    file = fopen(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    fseek(file, roffset, SEEK_END);
                    fputs(",", file);
                } లేకపోతే {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_దొంతర.back().indent = 4;
            emitter_do_not_use_ప్రత్యక్ష_dereference = createJSONEmitter(ఇది);
        }
        is_opened = నిజం;
    } లేకపోతే {
        స్థిరరాశి size_t buf_size0 = 40;
        బఫరు.resize(buf_size0);
        ఒకవేళ (mem_బహుళకం) {
            strbuf = (అక్షరం *) filename_or_buf;
            strbufsize = అక్షరశ్రేణి_పొడవు(strbuf);
        }

        స్థిరరాశి అక్షరం *yaml_signature = "%YAML";
        స్థిరరాశి అక్షరం *json_signature = "{";
        స్థిరరాశి అక్షరం *xml_signature = "<?xml";
        అక్షరం *buf = ఇది->gets(16);
        CV_Assert(buf);
        అక్షరం *bufPtr = cv_skip_BOM(buf);
        size_t buf_స్థానభ్రంశం = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        ఒకవేళ (అక్షరశ్రేణి_పోలిక(bufPtr, yaml_signature, అక్షరశ్రేణి_పొడవు(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        లేకపోతే ఒకవేళ (అక్షరశ్రేణి_పోలిక(bufPtr, json_signature, అక్షరశ్రేణి_పొడవు(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        లేకపోతే ఒకవేళ (అక్షరశ్రేణి_పోలిక(bufPtr, xml_signature, అక్షరశ్రేణి_పొడవు(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        లేకపోతే
            fmt = FileStorage::FORMAT_YAML;

        rewind();
        strbufpos = buf_స్థానభ్రంశం;
        bufofs = 0;

        ప్రయత్నించు {
            అక్షరం *ptr = బఫరు_ప్రారంభం();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_కణుపు మూలం_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_కణుపు_Space(మూలం_nodes, 9);
            *rptr = File_కణుపు::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            ఎంపిక (fmt) {
                స్థితి FileStorage::FORMAT_XML:
                    parser_do_not_use_ప్రత్యక్ష_dereference = createXMLParser(ఇది);
                    ఆపు;
                స్థితి FileStorage::FORMAT_YAML:
                స్థితి FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_ప్రత్యక్ష_dereference = createYAMLParser(ఇది);
                    ఆపు;
                స్థితి FileStorage::FORMAT_JSON:
                    parser_do_not_use_ప్రత్యక్ష_dereference = createJSONParser(ఇది);
                    ఆపు;
                అప్రమేయ:
                    parser_do_not_use_ప్రత్యక్ష_dereference = Ptr<FileStorageParser>();
            }

            ఒకవేళ (!parser_do_not_use_ప్రత్యక్ష_dereference.empty()) {
                ok = getParser().parse(ptr);
                ఒకవేళ (ok) {
                    finalizeCollection(మూలం_nodes);

                    CV_Assert(!fs_దత్తాంశం_ptrs.empty());
                    File_కణుపు roots_కణుపు(fs_ext, 0, 0);
                    size_t i, nroots = roots_కణుపు.size();
                    File_కణుపు_Iterator it = roots_కణుపు.begin();

                    కొరకు (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        పట్టుకో (...)
        {
            // FIXIT log error message
            is_opened = నిజం;
            release();
            విసురు;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = నిజం;
        std::vector<అక్షరం> tmpbuf;
        std::swap(బఫరు, tmpbuf);
        bufofs = 0;
    }
    తిరిగి ok;
}

శూన్య FileStorage::Impl::puts(స్థిరరాశి అక్షరం *str) {
    CV_Assert(write_బహుళకం);
    ఒకవేళ (mem_బహుళకం)
        std::copy(str, str + అక్షరశ్రేణి_పొడవు(str), std::back_inserter(outbuf));
    లేకపోతే ఒకవేళ (file)
        fputs(str, file);
#ఒకవేళ USE_ZLIB
    లేకపోతే ఒకవేళ (gzfile)
        gzputs(gzfile, str);
#అయితే_ముగింపు
    లేకపోతే
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

అక్షరం *FileStorage::Impl::getsFromFile(అక్షరం *buf, పూర్ణాంకం లెక్క_2) {
    ఒకవేళ (file)
        తిరిగి fgets(buf, లెక్క_2, file);
#ఒకవేళ USE_ZLIB
    ఒకవేళ (gzfile)
        తిరిగి gzgets(gzfile, buf, లెక్క_2);
#అయితే_ముగింపు
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

అక్షరం *FileStorage::Impl::gets(size_t max_లెక్క) {
    ఒకవేళ (strbuf) {
        size_t i = strbufpos, len = strbufsize;
        స్థిరరాశి అక్షరం *instr = strbuf;
        కొరకు (; i < len; i++) {
            అక్షరం c = instr[i];
            ఒకవేళ (c == '\0' || c == '\n') {
                ఒకవేళ (c == '\n')
                    i++;
                ఆపు;
            }
        }
        size_t లెక్క_2 = i - strbufpos;
        ఒకవేళ (max_లెక్క == 0 || max_లెక్క > లెక్క_2)
            max_లెక్క = లెక్క_2;
        బఫరు.resize(std::max(బఫరు.size(), max_లెక్క + 8));
        జ్ఞాపకం_నకలు(&బఫరు[0], instr + strbufpos, max_లెక్క);
        బఫరు[max_లెక్క] = '\0';
        strbufpos = i;
        తిరిగి max_లెక్క > 0 ? &బఫరు[0] : 0;
    }

    స్థిరరాశి size_t MAX_ఖండం_కొలత = గరిష్ఠ_పూర్ణాంకం / 2; // hopefully, that will be enough
    ఒకవేళ (max_లెక్క == 0)
        max_లెక్క = MAX_ఖండం_కొలత;
    లేకపోతే
        CV_Assert(max_లెక్క < MAX_ఖండం_కొలత);
    size_t ofs = 0;

    కొరకు (;;) {
        పూర్ణాంకం లెక్క_2 = (పూర్ణాంకం) std::min(బఫరు.size() - ofs - 16, max_లెక్క);
        అక్షరం *ptr = getsFromFile(&బఫరు[ofs], లెక్క_2 + 1);
        ఒకవేళ (!ptr)
            ఆపు;
        పూర్ణాంకం delta = (పూర్ణాంకం) అక్షరశ్రేణి_పొడవు(ptr);
        ofs += delta;
        max_లెక్క -= delta;
        ఒకవేళ (delta == 0 || ptr[delta - 1] == '\n' || max_లెక్క == 0)
            ఆపు;
        ఒకవేళ (delta == లెక్క_2)
            బఫరు.resize((size_t) (బఫరు.size() * 1.5));
    }
    తిరిగి ofs > 0 ? &బఫరు[0] : 0;
}

అక్షరం *FileStorage::Impl::gets() {
    అక్షరం *ptr = ఇది->gets(0);
    ఒకవేళ (!ptr) {
        ptr = బఫరు_ప్రారంభం();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        సమితి_Eof();
        తిరిగి 0;
    } లేకపోతే {
        size_t l = అక్షరశ్రేణి_పొడవు(ptr);
        ఒకవేళ (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    తిరిగి ptr;
}

తార్కిక FileStorage::Impl::eof() {
    ఒకవేళ (dummy_eof)
        తిరిగి నిజం;
    ఒకవేళ (strbuf)
        తిరిగి strbufpos >= strbufsize;
    ఒకవేళ (file)
        తిరిగి feof(file) != 0;
#ఒకవేళ USE_ZLIB
    ఒకవేళ (gzfile)
        తిరిగి gzeof(gzfile) != 0;
#అయితే_ముగింపు
    తిరిగి అబద్ధం;
}

శూన్య FileStorage::Impl::సమితి_Eof() {
    dummy_eof = నిజం;
}

శూన్య FileStorage::Impl::closeFile() {
    ఒకవేళ (file)
        fclose(file);
#ఒకవేళ USE_ZLIB
    లేకపోతే ఒకవేళ (gzfile)
        gzclose(gzfile);
#అయితే_ముగింపు
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = అబద్ధం;
}

శూన్య FileStorage::Impl::rewind() {
    ఒకవేళ (file)
        ::rewind(file);
#ఒకవేళ USE_ZLIB
    లేకపోతే ఒకవేళ (gzfile)
        gzrewind(gzfile);
#అయితే_ముగింపు
    strbufpos = 0;
}

అక్షరం *FileStorage::Impl::resize_Write_బఫరు(అక్షరం *ptr, పూర్ణాంకం len) {
    స్థిరరాశి అక్షరం *బఫరు_చివర = &బఫరు[0] + బఫరు.size();
    ఒకవేళ (ptr + len < బఫరు_చివర)
        తిరిగి ptr;

    స్థిరరాశి అక్షరం *బఫరు_ప్రారంభం_2 = &బఫరు[0];
    పూర్ణాంకం written_len = (పూర్ణాంకం) (ptr - బఫరు_ప్రారంభం_2);

    CV_Assert(written_len <= (పూర్ణాంకం) బఫరు.size());
    పూర్ణాంకం new_కొలత_2 = (పూర్ణాంకం) ((బఫరు_చివర - బఫరు_ప్రారంభం_2) * 3 / 2);
    new_కొలత_2 = MAX(written_len + len, new_కొలత_2);
    బఫరు.reserve(new_కొలత_2 + 256);
    బఫరు.resize(new_కొలత_2);
    bufofs = written_len;
    తిరిగి &బఫరు[0] + bufofs;
}

అక్షరం *FileStorage::Impl::flush() {
    అక్షరం *బఫరు_ప్రారంభం_2 = &బఫరు[0];
    అక్షరం *ptr = బఫరు_ప్రారంభం_2 + bufofs;

    ఒకవేళ (ptr > బఫరు_ప్రారంభం_2 + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        puts(బఫరు_ప్రారంభం_2);
        bufofs = 0;
    }

    పూర్ణాంకం indent = write_దొంతర.back().indent;

    ఒకవేళ (space != indent) {
        memset(బఫరు_ప్రారంభం_2, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = బఫరు_ప్రారంభం_2 + bufofs;

    తిరిగి ptr;
}

శూన్య FileStorage::Impl::చివర_Write_Struct() {
    CV_Assert(write_బహుళకం);

    check_if_write_struct_is_delayed(అబద్ధం);
    ఒకవేళ (state_యొక్క_writing_base64 != FileStorage_API::Uncertain)
        switch_గమ్యం_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_దొంతర.empty());

    FStruct_దత్తాంశం &current_struct = write_దొంతర.back();
    ఒకవేళ (fmt == FileStorage::FORMAT_JSON && !File_కణుపు::isFlow(current_struct.flags) && write_దొంతర.size() > 1)
        current_struct.indent = write_దొంతర[write_దొంతర.size() - 2].indent;

    getEmitter().చివర_Write_Struct(current_struct);

    write_దొంతర.pop_back();
    ఒకవేళ (!write_దొంతర.empty())
        write_దొంతర.back().flags &= ~File_కణుపు::ఖాళీలు_2;
}

శూన్య FileStorage::Impl::ప్రారంభం_Write_Struct_helper(స్థిరరాశి అక్షరం *కీలకం, పూర్ణాంకం struct_flags,
                                                స్థిరరాశి అక్షరం *type_name) {
    CV_Assert(write_బహుళకం);

    struct_flags = (struct_flags & (File_కణుపు::TYPE_ముసుగు | File_కణుపు::FLOW)) | File_కణుపు::ఖాళీలు_2;
    ఒకవేళ (!File_కణుపు::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    ఒకవేళ (type_name && type_name[0] == '\0')
        type_name = 0;

    FStruct_దత్తాంశం s = getEmitter().ప్రారంభం_Write_Struct(write_దొంతర.back(), కీలకం, struct_flags, type_name);

    write_దొంతర.push_back(s);
    size_t write_దొంతర_కొలత = write_దొంతర.size();
    ఒకవేళ (write_దొంతర_కొలత > 1)
        write_దొంతర[write_దొంతర_కొలత - 2].flags &= ~File_కణుపు::ఖాళీలు_2;

    ఒకవేళ (fmt != FileStorage::FORMAT_JSON && !File_కణుపు::isFlow(s.flags))
        flush();

    ఒకవేళ (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_కణుపు::isMap(struct_flags)) {
        getEmitter().లిఖ("type_id", type_name, అబద్ధం);
    }
}

శూన్య FileStorage::Impl::ప్రారంభం_Write_Struct(స్థిరరాశి అక్షరం *కీలకం, పూర్ణాంకం struct_flags,
                                         స్థిరరాశి అక్షరం *type_name) {
    check_if_write_struct_is_delayed(అబద్ధం);
    ఒకవేళ (state_యొక్క_writing_base64 == FileStorage_API::NotUse)
        switch_గమ్యం_Base64_state(FileStorage_API::Uncertain);

    ఒకవేళ (state_యొక్క_writing_base64 == FileStorage_API::Uncertain && File_కణుపు::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(కీలకం, struct_flags, type_name);
    } లేకపోతే ఒకవేళ (type_name && memcmp(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        ఒకవేళ ((File_కణుపు::TYPE_ముసుగు & struct_flags) != File_కణుపు::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        లేకపోతే ఒకవేళ (state_యొక్క_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        ప్రారంభం_Write_Struct_helper(కీలకం, struct_flags, "binary");

        ఒకవేళ (state_యొక్క_writing_base64 != FileStorage_API::Uncertain)
            switch_గమ్యం_Base64_state(FileStorage_API::Uncertain);
        switch_గమ్యం_Base64_state(FileStorage_API::లోపల_Use);
    } లేకపోతే {
        /* Won't output Base64 data */
        ఒకవేళ (state_యొక్క_writing_base64 == FileStorage_API::లోపల_Use)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        ప్రారంభం_Write_Struct_helper(కీలకం, struct_flags, type_name);

        ఒకవేళ (state_యొక్క_writing_base64 != FileStorage_API::Uncertain)
            switch_గమ్యం_Base64_state(FileStorage_API::Uncertain);
        switch_గమ్యం_Base64_state(FileStorage_API::NotUse);
    }
}

శూన్య FileStorage::Impl::writeComment(స్థిరరాశి అక్షరం *comment, తార్కిక eol_comment) {
    CV_Assert(write_బహుళకం);
    getEmitter().writeComment(comment, eol_comment);
}

శూన్య FileStorage::Impl::ప్రారంభం_తదుపరి_ప్రవాహం() {
    CV_Assert(write_బహుళకం);
    ఒకవేళ (!ఖాళీలు_ప్రవాహం) {
        అయితే (!write_దొంతర.empty())
            చివర_Write_Struct();
        flush();
        getEmitter().ప్రారంభం_తదుపరి_ప్రవాహం();
        ఖాళీలు_ప్రవాహం = నిజం;
        write_దొంతర.push_back(FStruct_దత్తాంశం("", File_కణుపు::ఖాళీలు_2, 0));
        bufofs = 0;
    }
}

శూన్య FileStorage::Impl::లిఖ(స్థిరరాశి String &కీలకం, పూర్ణాంకం value) {
    CV_Assert(write_బహుళకం);
    getEmitter().లిఖ(కీలకం.c_str(), value);
}

శూన్య FileStorage::Impl::లిఖ(స్థిరరాశి String &కీలకం, int64_t value) {
    CV_Assert(write_బహుళకం);
    getEmitter().లిఖ(కీలకం.c_str(), value);
}

శూన్య FileStorage::Impl::లిఖ(స్థిరరాశి String &కీలకం, ద్విగుణ value) {
    CV_Assert(write_బహుళకం);
    getEmitter().లిఖ(కీలకం.c_str(), value);
}

శూన్య FileStorage::Impl::లిఖ(స్థిరరాశి String &కీలకం, స్థిరరాశి String &value) {
    CV_Assert(write_బహుళకం);
    getEmitter().లిఖ(కీలకం.c_str(), value.c_str(), అబద్ధం);
}

శూన్య FileStorage::Impl::write_Raw_దత్తాంశం(స్థిరరాశి std::string &dt, స్థిరరాశి శూన్య *దత్తాంశం_3, size_t len) {
    CV_Assert(write_బహుళకం);

    ఒకవేళ (is_using_base64 || state_యొక్క_writing_base64 == FileStorage_API::Base64State::లోపల_Use) {
        write_Raw_దత్తాంశం_Base64(దత్తాంశం_3, len, dt.c_str());
        తిరిగి;
    } లేకపోతే ఒకవేళ (state_యొక్క_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_గమ్యం_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    size_t elem_కొలత = fs::calc_Struct_కొలత(dt.c_str(), 0);
    CV_Assert(elem_కొలత);
    CV_Assert(len % elem_కొలత == 0);
    len /= elem_కొలత;

    తార్కిక explicitZero = fmt == FileStorage::FORMAT_JSON;
    స్థిరరాశి uchar *data0 = (స్థిరరాశి uchar *) దత్తాంశం_3;
    పూర్ణాంకం fmt_జతలు[CV_FS_MAX_FMT_జతలు * 2], k, fmt_pair_లెక్క;
    అక్షరం buf[256] = "";

    fmt_pair_లెక్క = fs::decodeFormat(dt.c_str(), fmt_జతలు, CV_FS_MAX_FMT_జతలు);

    ఒకవేళ (!len)
        తిరిగి;

    ఒకవేళ (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    ఒకవేళ (fmt_pair_లెక్క == 1) {
        fmt_జతలు[0] *= (పూర్ణాంకం) len;
        len = 1;
    }

    కొరకు (; len--; data0 += elem_కొలత) {
        పూర్ణాంకం offset = 0;
        కొరకు (k = 0; k < fmt_pair_లెక్క; k++) {
            పూర్ణాంకం i, లెక్క_2 = fmt_జతలు[k * 2];
            పూర్ణాంకం elem_type = fmt_జతలు[k * 2 + 1];
            పూర్ణాంకం elem_కొలత_2 = CV_ELEM_కొలత(elem_type);
            స్థిరరాశి అక్షరం *ptr;

            offset = cvAlign(offset, elem_కొలత_2);
            స్థిరరాశి uchar *దత్తాంశం = data0 + offset;

            కొరకు (i = 0; i < లెక్క_2; i++) {
                ఎంపిక (elem_type) {
                    స్థితి CV_8U:
                        ptr = fs::itoa(*(uchar *) దత్తాంశం, buf, 10);
                        దత్తాంశం++;
                        ఆపు;
                    స్థితి CV_Bool:
                        ptr = fs::itoa(*(uchar *) దత్తాంశం != 0, buf, 10);
                        దత్తాంశం++;
                        ఆపు;
                    స్థితి CV_8S:
                        ptr = fs::itoa(*(అక్షరం *) దత్తాంశం, buf, 10);
                        దత్తాంశం++;
                        ఆపు;
                    స్థితి CV_16U:
                        ptr = fs::itoa(*(ushort *) దత్తాంశం, buf, 10);
                        దత్తాంశం += పరిమాణం(ushort);
                        ఆపు;
                    స్థితి CV_16S:
                        ptr = fs::itoa(*(పొట్టి *) దత్తాంశం, buf, 10);
                        దత్తాంశం += పరిమాణం(పొట్టి);
                        ఆపు;
                    స్థితి CV_32U:
                        ptr = fs::itoa((int64_t)*(గుర్తులేని*) దత్తాంశం, buf, 10, అబద్ధం);
                        దత్తాంశం += పరిమాణం(గుర్తులేని);
                        ఆపు;
                    స్థితి CV_32S:
                        ptr = fs::itoa(*(పూర్ణాంకం *) దత్తాంశం, buf, 10);
                        దత్తాంశం += పరిమాణం(పూర్ణాంకం);
                        ఆపు;
                    స్థితి CV_64U:
                        ptr = fs::itoa(*(uint64_t*) దత్తాంశం, buf, 10, అబద్ధం);
                        దత్తాంశం += పరిమాణం(uint64_t);
                        ఆపు;
                    స్థితి CV_64S:
                        ptr = fs::itoa(*(int64_t*) దత్తాంశం, buf, 10, నిజం);
                        దత్తాంశం += పరిమాణం(int64_t);
                        ఆపు;
                    స్థితి CV_32F:
                        ptr = fs::float_గమ్యం_String(buf, పరిమాణం(buf), *(దశాంశం *) దత్తాంశం, అబద్ధం, explicitZero);
                        దత్తాంశం += పరిమాణం(దశాంశం);
                        ఆపు;
                    స్థితి CV_64F:
                        ptr = fs::double_గమ్యం_String(buf, పరిమాణం(buf), *(ద్విగుణ *) దత్తాంశం, explicitZero);
                        దత్తాంశం += పరిమాణం(ద్విగుణ);
                        ఆపు;
                    స్థితి CV_16F:
                        ptr = fs::float_గమ్యం_String(buf, పరిమాణం(buf), (దశాంశం) *(hfloat *) దత్తాంశం, నిజం, explicitZero);
                        దత్తాంశం += పరిమాణం(hfloat);
                        ఆపు;
                    స్థితి CV_16BF:
                        ptr = fs::float_గమ్యం_String(buf, పరిమాణం(buf), (దశాంశం) *(bfloat *) దత్తాంశం, నిజం, explicitZero);
                        దత్తాంశం += పరిమాణం(bfloat);
                        ఆపు;
                    స్థితి CV_8F_E4M3FN:
                        ptr = fs::float_గమ్యం_String(buf, పరిమాణం(buf), (దశాంశం) *(fp8_t *) దత్తాంశం, నిజం, explicitZero);
                        దత్తాంశం += పరిమాణం(fp8_t);
                        ఆపు;
                    స్థితి CV_8F_E4M3FNUZ:
                        ptr = fs::float_గమ్యం_String(buf, పరిమాణం(buf), (దశాంశం) *(fp8a_t *) దత్తాంశం, నిజం, explicitZero);
                        దత్తాంశం += పరిమాణం(fp8a_t);
                        ఆపు;
                    అప్రమేయ:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        తిరిగి;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (పూర్ణాంకం) (దత్తాంశం - data0);
        }
    }
}

శూన్య FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(అబద్ధం);

    ఒకవేళ (state_యొక్క_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_గమ్యం_Base64_state(FileStorage_API::Base64State::Uncertain);
}

శూన్య FileStorage::Impl::switch_గమ్యం_Base64_state(FileStorage_API::Base64State new_state) {
    స్థిరరాశి అక్షరం *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    స్థిరరాశి అక్షరం *err_unable_గమ్యం_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    ఎంపిక (state_యొక్క_writing_base64) {
        స్థితి FileStorage_API::Base64State::Uncertain:
            ఎంపిక (new_state) {
                స్థితి FileStorage_API::Base64State::లోపల_Use:
                {
                    CV_DbgAssert(base64_writer == 0);
                    తార్కిక can_indent = (fmt != cv::FileStorage::బహుళకం_2::FORMAT_JSON);
                    base64_writer = కొత్త base64::Base64Writer(*ఇది, can_indent);
                    ఒకవేళ (!can_indent) {
                        అక్షరం *ptr = బఫరు_Ptr();
                        *ptr++ = '\0';
                        puts(బఫరు_ప్రారంభం());
                        సమితి_బఫరు_Ptr(బఫరు_ప్రారంభం());
                        memset(బఫరు_ప్రారంభం(), 0, static_cast<పూర్ణాంకం>(space));
                        puts("\"$base64$");
                    }
                    ఆపు;
                }
                స్థితి FileStorage_API::Base64State::Uncertain:
                    ఆపు;
                స్థితి FileStorage_API::Base64State::NotUse:
                    ఆపు;
                అప్రమేయ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ఆపు;
            }
            ఆపు;
        స్థితి FileStorage_API::Base64State::లోపల_Use:
            ఎంపిక (new_state) {
                స్థితి FileStorage_API::Base64State::లోపల_Use:
                స్థితి FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_గమ్యం_switch);
                    ఆపు;
                స్థితి FileStorage_API::Base64State::Uncertain:
                    తొలగించు base64_writer;
                    base64_writer = 0;
                    ఒకవేళ ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        puts("\"");
                        సమితి_బఫరు_Ptr(బఫరు_ప్రారంభం());
                        flush();
                        memset(బఫరు_ప్రారంభం(), 0, static_cast<పూర్ణాంకం>(space) );
                        సమితి_బఫరు_Ptr(బఫరు_ప్రారంభం());
                    }
                    ఆపు;
                అప్రమేయ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ఆపు;
            }
            ఆపు;
        స్థితి FileStorage_API::Base64State::NotUse:
            ఎంపిక (new_state) {
                స్థితి FileStorage_API::Base64State::లోపల_Use:
                స్థితి FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_గమ్యం_switch);
                    ఆపు;
                స్థితి FileStorage_API::Base64State::Uncertain:
                    ఆపు;
                అప్రమేయ:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    ఆపు;
            }
            ఆపు;
        అప్రమేయ:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            ఆపు;
    }

    state_యొక్క_writing_base64 = new_state;
}

శూన్య FileStorage::Impl::make_write_struct_delayed(స్థిరరాశి అక్షరం *కీలకం, పూర్ణాంకం struct_flags, స్థిరరాశి అక్షరం *type_name) {
    CV_Assert(is_write_struct_delayed == అబద్ధం);
    CV_DbgAssert(delayed_struct_కీలకం == శూన్యసూచిక);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == శూన్యసూచిక);

    delayed_struct_flags = struct_flags;

    ఒకవేళ (కీలకం != శూన్యసూచిక) {
        delayed_struct_కీలకం = కొత్త అక్షరం[అక్షరశ్రేణి_పొడవు(కీలకం) + 1U];
        strcpy(delayed_struct_కీలకం, కీలకం);
    }

    ఒకవేళ (type_name != శూన్యసూచిక) {
        delayed_type_name = కొత్త అక్షరం[అక్షరశ్రేణి_పొడవు(type_name) + 1U];
        strcpy(delayed_type_name, type_name);
    }

    is_write_struct_delayed = నిజం;
}

శూన్య FileStorage::Impl::check_if_write_struct_is_delayed(తార్కిక change_type_గమ్యం_base64) {
    ఒకవేళ (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_కీలకం;
        std::string type_name;
        పూర్ణాంకం struct_flags = delayed_struct_flags;

        ఒకవేళ (delayed_struct_కీలకం != శూన్యసూచిక && *delayed_struct_కీలకం != '\0') {
            struct_కీలకం.assign(delayed_struct_కీలకం);
        }
        ఒకవేళ (delayed_type_name != శూన్యసూచిక && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        తొలగించు[] delayed_struct_కీలకం;
        తొలగించు[] delayed_type_name;
        delayed_struct_కీలకం = శూన్యసూచిక;
        delayed_struct_flags = 0;
        delayed_type_name = శూన్యసూచిక;

        is_write_struct_delayed = అబద్ధం;

        /* call */
        ఒకవేళ (change_type_గమ్యం_base64) {
            ప్రారంభం_Write_Struct_helper(struct_కీలకం.c_str(), struct_flags, "binary");
            ఒకవేళ (state_యొక్క_writing_base64 != FileStorage_API::Uncertain)
                switch_గమ్యం_Base64_state(FileStorage_API::Uncertain);
            switch_గమ్యం_Base64_state(FileStorage_API::లోపల_Use);
        } లేకపోతే {
            ప్రారంభం_Write_Struct_helper(struct_కీలకం.c_str(), struct_flags, type_name.c_str());
            ఒకవేళ (state_యొక్క_writing_base64 != FileStorage_API::Uncertain)
                switch_గమ్యం_Base64_state(FileStorage_API::Uncertain);
            switch_గమ్యం_Base64_state(FileStorage_API::NotUse);
        }
    }
}

శూన్య FileStorage::Impl::write_Raw_దత్తాంశం_Base64(స్థిరరాశి శూన్య *దత్తాంశం_3, size_t len, స్థిరరాశి అక్షరం *dt) {
    CV_Assert(write_బహుళకం);

    check_if_write_struct_is_delayed(నిజం);

    ఒకవేళ (state_యొక్క_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_గమ్యం_Base64_state(FileStorage_API::Base64State::లోపల_Use);
    } లేకపోతే ఒకవేళ (state_యొక్క_writing_base64 != FileStorage_API::Base64State::లోపల_Use) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->లిఖ(దత్తాంశం_3, len, dt);
}

File_కణుపు FileStorage::Impl::get_మొదటి_శిఖరం_Level_కణుపు() స్థిరరాశి {
    తిరిగి roots.empty() ? File_కణుపు() : roots[0];
}

File_కణుపు FileStorage::Impl::మూలం(పూర్ణాంకం ప్రవాహం_Idx) స్థిరరాశి {
    తిరిగి ప్రవాహం_Idx >= 0 && ప్రవాహం_Idx < (పూర్ణాంకం) roots.size() ? roots[ప్రవాహం_Idx] : File_కణుపు();
}

File_కణుపు FileStorage::Impl::క్రియాకారకం[](స్థిరరాశి String &nodename) స్థిరరాశి {
    తిరిగి ఇది->క్రియాకారకం[](nodename.c_str());
}

File_కణుపు FileStorage::Impl::క్రియాకారకం[](స్థిరరాశి అక్షరం * /*nodename*/) స్థిరరాశి {
    తిరిగి File_కణుపు();
}

పూర్ణాంకం FileStorage::Impl::getFormat() స్థిరరాశి { తిరిగి fmt; }

అక్షరం *FileStorage::Impl::బఫరు_Ptr() స్థిరరాశి { తిరిగి (అక్షరం *) (&బఫరు[0] + bufofs); }

అక్షరం *FileStorage::Impl::బఫరు_ప్రారంభం() స్థిరరాశి { తిరిగి (అక్షరం *) &బఫరు[0]; }

అక్షరం *FileStorage::Impl::బఫరు_చివర_2() స్థిరరాశి { తిరిగి (అక్షరం *) (&బఫరు[0] + బఫరు.size()); }

శూన్య FileStorage::Impl::సమితి_బఫరు_Ptr(అక్షరం *ptr) {
    అక్షరం *bufferstart = బఫరు_ప్రారంభం();
    CV_Assert(ptr >= bufferstart && ptr <= బఫరు_చివర_2());
    bufofs = ptr - bufferstart;
}

పూర్ణాంకం FileStorage::Impl::wrapMargin() స్థిరరాశి { తిరిగి wrap_margin; }

FStruct_దత్తాంశం &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_దొంతర.empty());
    తిరిగి write_దొంతర.back();
}

శూన్య FileStorage::Impl::సమితి_Non_ఖాళీలు() {
    ఖాళీలు_ప్రవాహం = అబద్ధం;
}

శూన్య FileStorage::Impl::processSpecialDouble(అక్షరం *buf, ద్విగుణ *value, అక్షరం **endptr) {
    FileStorage_API *fs = ఇది;
    అక్షరం c = buf[0];
    పూర్ణాంకం inf_hi = 0x7ff00000;

    ఒకవేళ (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    ఒకవేళ (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    ఒకవేళ (పెద్ద_అక్షరం(buf[1]) == 'I' && పెద్ద_అక్షరం(buf[2]) == 'N' && పెద్ద_అక్షరం(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    లేకపోతే ఒకవేళ (పెద్ద_అక్షరం(buf[1]) == 'N' && పెద్ద_అక్షరం(buf[2]) == 'A' && పెద్ద_అక్షరం(buf[3]) == 'N')
        v.u = (uint64) -1;
    లేకపోతే
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

ద్విగుణ FileStorage::Impl::strtod(అక్షరం *ptr, అక్షరం **endptr) {
    ద్విగుణ fval = ::strtod(ptr, endptr);
    ఒకవేళ (**endptr == '.') {
        అక్షరం *dot_pos = *endptr;
        *dot_pos = ',';
        ద్విగుణ fval2 = ::strtod(ptr, endptr);
        *dot_pos = '.';
        ఒకవేళ (*endptr > dot_pos)
            fval = fval2;
        లేకపోతే
            *endptr = dot_pos;
    }

    ఒకవేళ (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    తిరిగి fval;
}

శూన్య FileStorage::Impl::convert_గమ్యం_Collection(పూర్ణాంకం type, File_కణుపు &కణుపు) {
    CV_Assert(type == File_కణుపు::SEQ || type == File_కణుపు::MAP);

    పూర్ణాంకం కణుపు_type = కణుపు.type();
    ఒకవేళ (కణుపు_type == type)
        తిరిగి;

    తార్కిక named = కణుపు.isNamed();
    uchar *ptr = కణుపు.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    ద్విగుణ fval = 0;
    std::string sval;
    తార్కిక కూడు_మొదటి_scalar = అబద్ధం;

    ఒకవేళ (కణుపు_type != File_కణుపు::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_కణుపు::SEQ);
        ఒకవేళ (కణుపు_type == File_కణుపు::INT) {
            ival = readLong(ptr);
            కూడు_మొదటి_scalar = నిజం;
        } లేకపోతే ఒకవేళ (కణుపు_type == File_కణుపు::REAL) {
            fval = readReal(ptr);
            కూడు_మొదటి_scalar = నిజం;
        } లేకపోతే ఒకవేళ (కణుపు_type == File_కణుపు::STRING) {
            sval = std::string(కణుపు);
            కూడు_మొదటి_scalar = నిజం;
        } లేకపోతే
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", కణుపు_type));
    }

    ptr = reserve_కణుపు_Space(కణుపు, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_కణుపు::NAMED : 0));
    // name has been copied automatically
    ఒకవేళ (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    ఒకవేళ (కూడు_మొదటి_scalar)
        కూడు_కణుపు(కణుపు, std::string(), కణుపు_type,
                కణుపు_type == File_కణుపు::INT ? (స్థిరరాశి శూన్య *) &ival :
                కణుపు_type == File_కణుపు::REAL ? (స్థిరరాశి శూన్య *) &fval :
                కణుపు_type == File_కణుపు::STRING ? (స్థిరరాశి శూన్య *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_కణుపు_Space(File_కణుపు &కణుపు, size_t sz) {
    తార్కిక shrink_ఖండం = అబద్ధం;
    size_t shrink_ఖండం_Idx = 0, shrink_కొలత = 0;

    uchar *ptr = 0, *ఖండం_చివర = 0;

    ఒకవేళ (!fs_దత్తాంశం_ptrs.empty()) {
        size_t ఖండం_Idx = కణుపు.ఖండం_Idx;
        size_t ofs = కణుపు.ofs;
        CV_Assert(ఖండం_Idx == fs_దత్తాంశం_ptrs.size() - 1);
        CV_Assert(ofs <= fs_దత్తాంశం_blksz[ఖండం_Idx]);
        CV_Assert(freeSpaceOfs <= fs_దత్తాంశం_blksz[ఖండం_Idx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_దత్తాంశం_ptrs[ఖండం_Idx] + ofs;
        ఖండం_చివర = fs_దత్తాంశం_ptrs[ఖండం_Idx] + fs_దత్తాంశం_blksz[ఖండం_Idx];

        CV_Assert(ptr >= fs_దత్తాంశం_ptrs[ఖండం_Idx] && ptr <= ఖండం_చివర);
        ఒకవేళ (ptr + sz <= ఖండం_చివర) {
            freeSpaceOfs = ofs + sz;
            తిరిగి ptr;
        }

        ఒకవేళ (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_దత్తాంశం[ఖండం_Idx]->resize(sz);
            ptr = &fs_దత్తాంశం[ఖండం_Idx]->at(0);
            fs_దత్తాంశం_ptrs[ఖండం_Idx] = ptr;
            fs_దత్తాంశం_blksz[ఖండం_Idx] = sz;
            freeSpaceOfs = sz;
            తిరిగి ptr;
        }

        shrink_ఖండం = నిజం;
        shrink_ఖండం_Idx = ఖండం_Idx;
        shrink_కొలత = ofs;
    }

    size_t ఖండం_కొలత_2 = std::max((size_t) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(ఖండం_కొలత_2);
    fs_దత్తాంశం.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_దత్తాంశం_ptrs.push_back(new_ptr);
    fs_దత్తాంశం_blksz.push_back(ఖండం_కొలత_2);
    కణుపు.ఖండం_Idx = fs_దత్తాంశం_ptrs.size() - 1;
    కణుపు.ofs = 0;
    freeSpaceOfs = sz;

    ఒకవేళ (ptr && ptr + 5 <= ఖండం_చివర) {
        new_ptr[0] = ptr[0];
        ఒకవేళ (ptr[0] & File_కణుపు::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    ఒకవేళ (shrink_ఖండం) {
        fs_దత్తాంశం[shrink_ఖండం_Idx]->resize(shrink_కొలత);
        fs_దత్తాంశం_blksz[shrink_ఖండం_Idx] = shrink_కొలత;
    }

    తిరిగి new_ptr;
}

గుర్తులేని FileStorage::Impl::getStringOfs(స్థిరరాశి std::string &కీలకం) స్థిరరాశి {
    str_hash_t::const_iterator it = str_hash.find(కీలకం);
    తిరిగి it != str_hash.end() ? it->second : 0;
}

File_కణుపు FileStorage::Impl::కూడు_కణుపు(File_కణుపు &collection, స్థిరరాశి std::string &కీలకం,
                                    పూర్ణాంకం elem_type, స్థిరరాశి శూన్య *value, పూర్ణాంకం len) {
    FileStorage_API *fs = ఇది;
    తార్కిక noname = కీలకం.empty() || (fmt == FileStorage::FORMAT_XML && strcmp(కీలకం.c_str(), "_") == 0);
    convert_గమ్యం_Collection(noname ? File_కణుపు::SEQ : File_కణుపు::MAP, collection);

    తార్కిక isseq = collection.empty() ? అబద్ధం : collection.isSeq();
    ఒకవేళ (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    గుర్తులేని strofs = 0;
    ఒకవేళ (!noname) {
        strofs = getStringOfs(కీలకం);
        ఒకవేళ (!strofs) {
            strofs = (గుర్తులేని) str_hash_దత్తాంశం.size();
            size_t keysize = కీలకం.size() + 1;
            str_hash_దత్తాంశం.resize(strofs + keysize);
            జ్ఞాపకం_నకలు(&str_hash_దత్తాంశం[0] + strofs, &కీలకం[0], keysize);
            str_hash.insert(std::make_pair(కీలకం, strofs));
        }
    }

    uchar *cp = collection.ptr();

    size_t ఖండం_Idx = fs_దత్తాంశం_ptrs.size() - 1;
    size_t ofs = freeSpaceOfs;
    File_కణుపు కణుపు(fs_ext, ఖండం_Idx, ofs);

    size_t sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_కణుపు_Space(కణుపు, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_కణుపు::NAMED));
    ఒకవేళ (elem_type == File_కణుపు::NONE)
        freeSpaceOfs -= 8;

    ఒకవేళ (!noname) {
        writeInt(ptr, (పూర్ణాంకం) strofs);
        ptr += 4;
    }

    ఒకవేళ (elem_type == File_కణుపు::SEQ || elem_type == File_కణుపు::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    ఒకవేళ (value)
        కణుపు.సమితి_విలువ(elem_type, value, len);

    ఒకవేళ (collection.isNamed())
        cp += 4;
    పూర్ణాంకం nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    తిరిగి కణుపు;
}

శూన్య FileStorage::Impl::finalizeCollection(File_కణుపు &collection) {
    ఒకవేళ (!collection.isSeq() && !collection.isMap())
        తిరిగి;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    ఒకవేళ (*ptr0 & File_కణుపు::NAMED)
        ptr += 4;
    size_t ఖండం_Idx = collection.ఖండం_Idx;
    size_t ofs = collection.ofs + (size_t) (ptr + 8 - ptr0);
    size_t raw_కొలత = 4;
    గుర్తులేని sz = (గుర్తులేని) readInt(ptr + 4);
    ఒకవేళ (sz > 0) {
        size_t చివరి_ఖండం_Idx = fs_దత్తాంశం_ptrs.size() - 1;

        కొరకు (; ఖండం_Idx < చివరి_ఖండం_Idx; ఖండం_Idx++) {
            raw_కొలత += fs_దత్తాంశం_blksz[ఖండం_Idx] - ofs;
            ofs = 0;
        }
    }
    raw_కొలత += freeSpaceOfs - ofs;
    writeInt(ptr, (పూర్ణాంకం) raw_కొలత);
}

శూన్య FileStorage::Impl::normalize_కణుపు_Ofs(size_t &ఖండం_Idx, size_t &ofs) స్థిరరాశి {
    అయితే (ofs >= fs_దత్తాంశం_blksz[ఖండం_Idx]) {
        ఒకవేళ (ఖండం_Idx == fs_దత్తాంశం_blksz.size() - 1) {
            CV_Assert(ofs == fs_దత్తాంశం_blksz[ఖండం_Idx]);
            ఆపు;
        }
        ofs -= fs_దత్తాంశం_blksz[ఖండం_Idx];
        ఖండం_Idx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_యొక్క_writing_base64() {
    తిరిగి state_యొక్క_writing_base64;
}

పూర్ణాంకం FileStorage::Impl::get_space() {
    తిరిగి space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = నిజం;
}

శూన్య FileStorage::Impl::Base64Decoder::init(స్థిరరాశి Ptr<FileStorageParser> &_parser, అక్షరం *_ptr, పూర్ణాంకం _indent) {
    parser_do_not_use_ప్రత్యక్ష_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = అబద్ధం;
}

తార్కిక FileStorage::Impl::Base64Decoder::read_మరిన్ని(పూర్ణాంకం needed) {
    స్థిర స్థిరరాశి uchar base64tab[] =
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

    ఒకవేళ (eos)
        తిరిగి అబద్ధం;

    size_t sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    కొరకు (size_t i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    అక్షరం *beg = 0, *end = 0;
    తార్కిక ok = getParser().get_Base64_అడ్డువరుస(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    ఒకవేళ (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = నిజం;
        size_t tc = totalchars;
        కొరకు (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    పూర్ణాంకం i = 0, j, n = (పూర్ణాంకం) encoded.size();
    ఒకవేళ (n > 0) {
        స్థిరరాశి uchar *tab = base64tab;
        అక్షరం *src = &encoded[0];

        కొరకు (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(పూర్ణాంకం) (uchar) src[i]], c = tab[(పూర్ణాంకం) (uchar) src[i + 1]];
            uchar b = tab[(పూర్ణాంకం) (uchar) src[i + 2]], a = tab[(పూర్ణాంకం) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    ఒకవేళ (i > 0 && encoded[i - 1] == '=') {
        ఒకవేళ (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        ఒకవేళ (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    కొరకు (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    తిరిగి (పూర్ణాంకం) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    size_t sz = decoded.size();
    ఒకవేళ (ofs >= sz && !read_మరిన్ని(1))
        తిరిగి (uchar) 0;
    తిరిగి decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    size_t sz = decoded.size();
    ఒకవేళ (ofs + 2 > sz && !read_మరిన్ని(2))
        తిరిగి (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    తిరిగి val;
}

పూర్ణాంకం FileStorage::Impl::Base64Decoder::getInt32() {
    size_t sz = decoded.size();
    ఒకవేళ (ofs + 4 > sz && !read_మరిన్ని(4))
        తిరిగి 0;
    పూర్ణాంకం ival = readInt(&decoded[ofs]);
    ofs += 4;
    తిరిగి ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    size_t sz = decoded.size();
    ఒకవేళ (ofs + 8 > sz && !read_మరిన్ని(8))
        తిరిగి 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    తిరిగి ival;
}

ద్విగుణ FileStorage::Impl::Base64Decoder::getFloat64() {
    size_t sz = decoded.size();
    ఒకవేళ (ofs + 8 > sz && !read_మరిన్ని(8))
        తిరిగి 0;
    ద్విగుణ fval = readReal(&decoded[ofs]);
    ofs += 8;
    తిరిగి fval;
}

తార్కిక FileStorage::Impl::Base64Decoder::చివర_యొక్క_ప్రవాహం() స్థిరరాశి { తిరిగి eos; }

అక్షరం *FileStorage::Impl::Base64Decoder::getPtr() స్థిరరాశి { తిరిగి ptr; }


అక్షరం *FileStorage::Impl::parseBase64(అక్షరం *ptr, పూర్ణాంకం indent, File_కణుపు &collection) {
    స్థిరరాశి పూర్ణాంకం BASE64_HDR_కొలత = 24;
    అక్షరం dt[BASE64_HDR_కొలత + 1] = {0};
    base64decoder.init(parser_do_not_use_ప్రత్యక్ష_dereference, ptr, indent);

    పూర్ణాంకం i, k;

    కొరకు (i = 0; i < BASE64_HDR_కొలత; i++)
        dt[i] = (అక్షరం) base64decoder.getUInt8();
    కొరకు (i = 0; i < BASE64_HDR_కొలత; i++)
        ఒకవేళ (ఖాళీనా(dt[i]))
            ఆపు;
    dt[i] = '\0';

    CV_Assert(!base64decoder.చివర_యొక్క_ప్రవాహం());

    పూర్ణాంకం fmt_జతలు[CV_FS_MAX_FMT_జతలు * 2];
    పూర్ణాంకం fmt_pair_లెక్క = fs::decodeFormat(dt, fmt_జతలు, CV_FS_MAX_FMT_జతలు);
    int64_t ival = 0;
    ద్విగుణ fval = 0;

    కొరకు (;;) {
        కొరకు (k = 0; k < fmt_pair_లెక్క; k++) {
            పూర్ణాంకం elem_type = fmt_జతలు[k * 2 + 1];
            పూర్ణాంకం లెక్క_2 = fmt_జతలు[k * 2];

            కొరకు (i = 0; i < లెక్క_2; i++) {
                పూర్ణాంకం కణుపు_type = File_కణుపు::INT;
                ఎంపిక (elem_type) {
                    స్థితి CV_8U:
                        ival = base64decoder.getUInt8();
                        ఆపు;
                    స్థితి CV_8S:
                        ival = (అక్షరం) base64decoder.getUInt8();
                        ఆపు;
                    స్థితి CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        ఆపు;
                    స్థితి CV_16U:
                        ival = base64decoder.getUInt16();
                        ఆపు;
                    స్థితి CV_16S:
                        ival = (పొట్టి) base64decoder.getUInt16();
                        ఆపు;
                    స్థితి CV_32U:
                        ival = base64decoder.getInt32();
                        ఆపు;
                    స్థితి CV_32S:
                        ival = base64decoder.getInt32();
                        ఆపు;
                    స్థితి CV_64U:
                        ival = base64decoder.getInt64();
                        ఆపు;
                    స్థితి CV_64S:
                        ival = base64decoder.getInt64();
                        ఆపు;
                    స్థితి CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        కణుపు_type = File_కణుపు::REAL;
                    }
                        ఆపు;
                    స్థితి CV_64F:
                        fval = base64decoder.getFloat64();
                        కణుపు_type = File_కణుపు::REAL;
                        ఆపు;
                    స్థితి CV_16F:
                        fval = దశాంశం(hfloat_From_బిట్లు(base64decoder.getUInt16()));
                        కణుపు_type = File_కణుపు::REAL;
                        ఆపు;
                    స్థితి CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        కణుపు_type = File_కణుపు::REAL;
                        ఆపు;
                    స్థితి CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        కణుపు_type = File_కణుపు::REAL;
                        ఆపు;
                    అప్రమేయ:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                ఒకవేళ (base64decoder.చివర_యొక్క_ప్రవాహం())
                    ఆపు;
                కూడు_కణుపు(collection, std::string(), కణుపు_type,
                        కణుపు_type == File_కణుపు::INT ? (శూన్య *) &ival : (శూన్య *) &fval, -1);
            }
        }
        ఒకవేళ (base64decoder.చివర_యొక్క_ప్రవాహం())
            ఆపు;
    }

    finalizeCollection(collection);
    తిరిగి base64decoder.getPtr();
}

శూన్య FileStorage::Impl::parseError(స్థిరరాశి అక్షరం *func_name, స్థిరరాశి std::string &err_సందేశం, స్థిరరాశి అక్షరం *ఆధారం_file,
                                   పూర్ణాంకం ఆధారం_పంక్తి) {
    std::string సందేశం = format("%s(%d): %s", filename.c_str(), lineno, err_సందేశం.c_str());
    error(Error::StsParseError, func_name, సందేశం.c_str(), ఆధారం_file, ఆధారం_పంక్తి);
}

స్థిరరాశి uchar *FileStorage::Impl::get_కణుపు_Ptr(size_t ఖండం_Idx, size_t ofs) స్థిరరాశి {
    CV_Assert(ఖండం_Idx < fs_దత్తాంశం_ptrs.size());
    CV_Assert(ofs < fs_దత్తాంశం_blksz[ఖండం_Idx]);

    తిరిగి fs_దత్తాంశం_ptrs[ఖండం_Idx] + ofs;
}

std::string FileStorage::Impl::getName(size_t nameofs) స్థిరరాశి {
    CV_Assert(nameofs < str_hash_దత్తాంశం.size());
    తిరిగి std::string(&str_hash_దత్తాంశం[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { తిరిగి fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(ఇది);
}

FileStorage::FileStorage(స్థిరరాశి String& filename, పూర్ణాంకం flags, స్థిరరాశి String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(ఇది);
    తార్కిక ok = p->open(filename.c_str(), flags, encoding.c_str());
    ఒకవేళ(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

శూన్య FileStorage::ప్రారంభం_Write_Struct(స్థిరరాశి String& name, పూర్ణాంకం struct_flags, స్థిరరాశి String& typeName)
{
    p->ప్రారంభం_Write_Struct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    ఒకవేళ ((struct_flags & File_కణుపు::TYPE_ముసుగు) == File_కణుపు::SEQ)
        state = FileStorage::విలువ_EXPECTED;
    లేకపోతే
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

శూన్య FileStorage::చివర_Write_Struct()
{
    p->చివర_Write_Struct();
    state = p->write_దొంతర.empty() || File_కణుపు::isMap(p->write_దొంతర.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::విలువ_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

తార్కిక FileStorage::open(స్థిరరాశి String& filename, పూర్ణాంకం flags, స్థిరరాశి String& encoding)
{
    ప్రయత్నించు
    {
        తార్కిక ok = p->open(filename.c_str(), flags, encoding.c_str());
        ఒకవేళ(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        తిరిగి ok;
    }
    పట్టుకో (...)
    {
        release();
        విసురు;  // re-throw
    }
}

తార్కిక FileStorage::isOpened() స్థిరరాశి { తిరిగి p->is_opened; }

శూన్య FileStorage::release()
{
    p->release();
}

File_కణుపు FileStorage::మూలం(పూర్ణాంకం i) స్థిరరాశి
{
    ఒకవేళ( p.empty() || p->roots.empty() || i < 0 || i >= (పూర్ణాంకం)p->roots.size() )
        తిరిగి File_కణుపు();

    తిరిగి p->roots[i];
}

File_కణుపు FileStorage::get_మొదటి_శిఖరం_Level_కణుపు() స్థిరరాశి
{
    File_కణుపు r = మూలం();
    File_కణుపు_Iterator it = r.begin();
    తిరిగి it != r.end() ? *it : File_కణుపు();
}

std::string FileStorage::getDefaultObjectName(స్థిరరాశి std::string& _filename)
{
    స్థిర స్థిరరాశి అక్షరం* stubname = "unnamed";
    స్థిరరాశి అక్షరం* filename = _filename.c_str();
    స్థిరరాశి అక్షరం* ptr2 = filename + _filename.size();
    స్థిరరాశి అక్షరం* ptr = ptr2 - 1;
    cv::Auto_బఫరు<అక్షరం> name_buf(_filename.size()+1);

    అయితే( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        ఒకవేళ( *ptr == '.' && (!*ptr2 || అక్షరశ్రేణి_పోలిక(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    ఒకవేళ( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    అక్షరం* name = name_buf.దత్తాంశం();

    // name must start with letter or '_'
    ఒకవేళ( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    అయితే( ptr < ptr2 )
    {
        అక్షరం c = *ptr++;
        ఒకవేళ( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.దత్తాంశం();
    ఒకవేళ( strcmp( name, "_" ) == 0 )
        strcpy( name, stubname );
    తిరిగి name;
}


పూర్ణాంకం FileStorage::getFormat() స్థిరరాశి
{
    తిరిగి p->fmt;
}

File_కణుపు FileStorage::క్రియాకారకం [](స్థిరరాశి అక్షరం* కీలకం) స్థిరరాశి
{
    తిరిగి ఇది->క్రియాకారకం[](std::string(కీలకం));
}

File_కణుపు FileStorage::క్రియాకారకం [](స్థిరరాశి std::string& కీలకం) స్థిరరాశి
{
    File_కణుపు res;
    కొరకు (size_t i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][కీలకం];
        ఒకవేళ (!res.empty())
            ఆపు;
    }
    తిరిగి res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    తిరిగి buf;
}

శూన్య FileStorage::writeRaw( స్థిరరాశి String& fmt, స్థిరరాశి శూన్య* vec, size_t len )
{
    p->write_Raw_దత్తాంశం(fmt, (స్థిరరాశి uchar*)vec, len);
}

శూన్య FileStorage::writeComment( స్థిరరాశి String& comment, తార్కిక eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

శూన్య writeScalar( FileStorage& fs, పూర్ణాంకం value )
{
    fs.p->లిఖ(String(), value);
}

శూన్య writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->లిఖ(String(), value);
}

శూన్య writeScalar( FileStorage& fs, దశాంశం value )
{
    fs.p->లిఖ(String(), (ద్విగుణ)value);
}

శూన్య writeScalar( FileStorage& fs, ద్విగుణ value )
{
    fs.p->లిఖ(String(), value);
}

శూన్య writeScalar( FileStorage& fs, స్థిరరాశి String& value )
{
    fs.p->లిఖ(String(), value);
}

శూన్య లిఖ( FileStorage& fs, స్థిరరాశి String& name, పూర్ణాంకం value )
{
    fs.p->లిఖ(name, value);
}

శూన్య లిఖ( FileStorage& fs, స్థిరరాశి String& name, int64_t value )
{
    fs.p->లిఖ(name, value);
}

శూన్య లిఖ( FileStorage& fs, స్థిరరాశి String& name, దశాంశం value )
{
    fs.p->లిఖ(name, (ద్విగుణ)value);
}

శూన్య లిఖ( FileStorage& fs, స్థిరరాశి String& name, ద్విగుణ value )
{
    fs.p->లిఖ(name, value);
}

శూన్య లిఖ( FileStorage& fs, స్థిరరాశి String& name, స్థిరరాశి String& value )
{
    fs.p->లిఖ(name, value);
}

శూన్య లిఖ( FileStorage& fs, స్థిరరాశి String& name, తార్కిక value )
{
    fs.p->లిఖ(name, value);
}

శూన్య FileStorage::లిఖ(స్థిరరాశి String& name, పూర్ణాంకం val) { p->లిఖ(name, val); }
శూన్య FileStorage::లిఖ(స్థిరరాశి String& name, తార్కిక val) { p->లిఖ(name, val); }
శూన్య FileStorage::లిఖ(స్థిరరాశి String& name, int64_t val) { p->లిఖ(name, val); }
శూన్య FileStorage::లిఖ(స్థిరరాశి String& name, ద్విగుణ val) { p->లిఖ(name, val); }
శూన్య FileStorage::లిఖ(స్థిరరాశి String& name, స్థిరరాశి String& val) { p->లిఖ(name, val); }
శూన్య FileStorage::లిఖ(స్థిరరాశి String& name, స్థిరరాశి Mat& val) { cv::లిఖ(*ఇది, name, val); }
శూన్య FileStorage::లిఖ(స్థిరరాశి String& name, స్థిరరాశి std::vector<String>& val) { cv::లిఖ(*ఇది, name, val); }

FileStorage& క్రియాకారకం << (FileStorage& fs, స్థిరరాశి String& str)
{
    గణన { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        విలువ_EXPECTED = FileStorage::విలువ_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    స్థిరరాశి అక్షరం* _str = str.c_str();
    ఒకవేళ( !fs.isOpened() || !_str )
        తిరిగి fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    అక్షరం c = *_str;

    ఒకవేళ( c == '}' || c == ']' )
    {
        ఒకవేళ( fs_impl->write_దొంతర.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        పూర్ణాంకం struct_flags = fs_impl->write_దొంతర.back().flags;
        అక్షరం expected_bracket = File_కణుపు::isMap(struct_flags) ? '}' : ']';
        ఒకవేళ( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->చివర_Write_Struct();
        CV_Assert(!fs_impl->write_దొంతర.empty());
        struct_flags = fs_impl->write_దొంతర.back().flags;
        fs.state = File_కణుపు::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : విలువ_EXPECTED;
        fs.elname = String();
    }
    లేకపోతే ఒకవేళ( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        ఒకవేళ (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = విలువ_EXPECTED + INSIDE_MAP;
    }
    లేకపోతే ఒకవేళ( (fs.state & 3) == విలువ_EXPECTED )
    {
        ఒకవేళ( c == '{' || c == '[' )
        {
            పూర్ణాంకం struct_flags = c == '{' ? File_కణుపు::MAP : File_కణుపు::SEQ;
            fs.state = struct_flags == File_కణుపు::MAP ? INSIDE_MAP + NAME_EXPECTED : విలువ_EXPECTED;
            _str++;
            ఒకవేళ( *_str == ':' )
            {
                _str++;
                ఒకవేళ( !*_str )
                    struct_flags |= File_కణుపు::FLOW;
            }
            fs_impl->ప్రారంభం_Write_Struct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        లేకపోతే
        {
            లిఖ( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            ఒకవేళ( fs.state == INSIDE_MAP + విలువ_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    లేకపోతే
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    తిరిగి fs;
}


File_కణుపు::File_కణుపు()
    : fs(ఖాళీ)
{
    ఖండం_Idx = ofs = 0;
}

File_కణుపు::File_కణుపు(FileStorage::Impl* _fs, size_t ఖండం_Idx_2, size_t _ofs)
    : fs(_fs)
{
    ఖండం_Idx = ఖండం_Idx_2;
    ofs = _ofs;
}

File_కణుపు::File_కణుపు(స్థిరరాశి FileStorage* _fs, size_t ఖండం_Idx_2, size_t _ofs)
    : File_కణుపు(_fs->p.get(), ఖండం_Idx_2, _ofs)
{
    // nothing
}

File_కణుపు::File_కణుపు(స్థిరరాశి File_కణుపు& కణుపు)
{
    fs = కణుపు.fs;
    ఖండం_Idx = కణుపు.ఖండం_Idx;
    ofs = కణుపు.ofs;
}

File_కణుపు& File_కణుపు::క్రియాకారకం=(స్థిరరాశి File_కణుపు& కణుపు)
{
    fs = కణుపు.fs;
    ఖండం_Idx = కణుపు.ఖండం_Idx;
    ofs = కణుపు.ofs;
    తిరిగి *ఇది;
}

File_కణుపు File_కణుపు::క్రియాకారకం[](స్థిరరాశి std::string& nodename) స్థిరరాశి
{
    ఒకవేళ(!fs)
        తిరిగి File_కణుపు();

    CV_Assert( isMap() );

    గుర్తులేని కీలకం = fs->getStringOfs(nodename);
    size_t i, sz = size();
    File_కణుపు_Iterator it = begin();

    కొరకు( i = 0; i < sz; i++, ++it )
    {
        File_కణుపు n = *it;
        స్థిరరాశి uchar* p = n.ptr();
        గుర్తులేని key2 = (గుర్తులేని)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_దత్తాంశం.size() );
        ఒకవేళ( కీలకం == key2 )
            తిరిగి n;
    }
    తిరిగి File_కణుపు();
}

File_కణుపు File_కణుపు::క్రియాకారకం[](స్థిరరాశి అక్షరం* nodename) స్థిరరాశి
{
    తిరిగి ఇది->క్రియాకారకం[](std::string(nodename));
}

File_కణుపు File_కణుపు::క్రియాకారకం[](పూర్ణాంకం i) స్థిరరాశి
{
    ఒకవేళ(!fs)
        తిరిగి File_కణుపు();

    CV_Assert( isSeq() );

    పూర్ణాంకం sz = (పూర్ణాంకం)size();
    CV_Assert( 0 <= i && i < sz );

    File_కణుపు_Iterator it = begin();
    it += i;

    తిరిగి *it;
}

std::vector<String> File_కణుపు::కీలకాలు() స్థిరరాశి
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    కొరకు (File_కణుపు_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    తిరిగి res;
}

పూర్ణాంకం File_కణుపు::type() స్థిరరాశి
{
    స్థిరరాశి uchar* p = ptr();
    ఒకవేళ(!p)
        తిరిగి NONE;
    తిరిగి (*p & TYPE_ముసుగు);
}

తార్కిక File_కణుపు::isMap(పూర్ణాంకం flags) { తిరిగి (flags & TYPE_ముసుగు) == MAP; }
తార్కిక File_కణుపు::isSeq(పూర్ణాంకం flags) { తిరిగి (flags & TYPE_ముసుగు) == SEQ; }
తార్కిక File_కణుపు::isCollection(పూర్ణాంకం flags) { తిరిగి isMap(flags) || isSeq(flags); }
తార్కిక File_కణుపు::isFlow(పూర్ణాంకం flags) { తిరిగి (flags & FLOW) != 0; }
తార్కిక File_కణుపు::is_ఖాళీలు_Collection(పూర్ణాంకం flags) { తిరిగి (flags & ఖాళీలు_2) != 0; }

తార్కిక File_కణుపు::empty() స్థిరరాశి   { తిరిగి fs == 0; }
తార్కిక File_కణుపు::isNone() స్థిరరాశి  { తిరిగి type() == NONE; }
తార్కిక File_కణుపు::isSeq() స్థిరరాశి   { తిరిగి type() == SEQ; }
తార్కిక File_కణుపు::isMap() స్థిరరాశి   { తిరిగి type() == MAP; }
తార్కిక File_కణుపు::isInt() స్థిరరాశి   { తిరిగి type() == INT;  }
తార్కిక File_కణుపు::isReal() స్థిరరాశి  { తిరిగి type() == REAL; }
తార్కిక File_కణుపు::isString() స్థిరరాశి { తిరిగి type() == STRING;  }
తార్కిక File_కణుపు::isNamed() స్థిరరాశి
{
    స్థిరరాశి uchar* p = ptr();
    ఒకవేళ(!p)
        తిరిగి అబద్ధం;
    తిరిగి (*p & NAMED) != 0;
}

std::string File_కణుపు::name() స్థిరరాశి
{
    స్థిరరాశి uchar* p = ptr();
    ఒకవేళ(!p)
        తిరిగి std::string();
    size_t nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    తిరిగి fs->getName(nameofs);
}

File_కణుపు::క్రియాకారకం పూర్ణాంకం() స్థిరరాశి
{
    స్థిరరాశి uchar* p = ptr();
    ఒకవేళ(!p)
        తిరిగి 0;
    పూర్ణాంకం ట్యాగ్ = *p;
    పూర్ణాంకం type = (ట్యాగ్ & TYPE_ముసుగు);
    p += (ట్యాగ్ & NAMED) ? 5 : 1;

    ఒకవేళ( type == INT )
    {
        తిరిగి readInt(p);
    }
    లేకపోతే ఒకవేళ( type == REAL )
    {
        తిరిగి cv_విడత(readReal(p));
    }
    లేకపోతే
        తిరిగి 0x7fffffff;
}

File_కణుపు::క్రియాకారకం int64_t() స్థిరరాశి
{
    స్థిరరాశి uchar* p = ptr();
    ఒకవేళ(!p)
        తిరిగి 0;
    పూర్ణాంకం ట్యాగ్ = *p;
    పూర్ణాంకం type = (ట్యాగ్ & TYPE_ముసుగు);
    p += (ట్యాగ్ & NAMED) ? 5 : 1;

    ఒకవేళ( type == INT )
    {
        తిరిగి readLong(p);
    }
    లేకపోతే ఒకవేళ( type == REAL )
    {
        తిరిగి cv_విడత(readReal(p));
    }
    లేకపోతే
        తిరిగి 0x7fffffff;
}

File_కణుపు::క్రియాకారకం దశాంశం() స్థిరరాశి
{
    స్థిరరాశి uchar* p = ptr();
    ఒకవేళ(!p)
        తిరిగి 0.f;
    పూర్ణాంకం ట్యాగ్ = *p;
    పూర్ణాంకం type = (ట్యాగ్ & TYPE_ముసుగు);
    p += (ట్యాగ్ & NAMED) ? 5 : 1;

    ఒకవేళ( type == INT )
    {
        తిరిగి (దశాంశం)readLong(p);
    }
    లేకపోతే ఒకవేళ( type == REAL )
    {
        తిరిగి (దశాంశం)readReal(p);
    }
    లేకపోతే
        తిరిగి FLT_MAX;
}

File_కణుపు::క్రియాకారకం ద్విగుణ() స్థిరరాశి
{
    స్థిరరాశి uchar* p = ptr();
    ఒకవేళ(!p)
        తిరిగి 0.f;
    పూర్ణాంకం ట్యాగ్ = *p;
    పూర్ణాంకం type = (ట్యాగ్ & TYPE_ముసుగు);
    p += (ట్యాగ్ & NAMED) ? 5 : 1;

    ఒకవేళ( type == INT )
    {
        తిరిగి (ద్విగుణ)readLong(p);
    }
    లేకపోతే ఒకవేళ( type == REAL )
    {
        తిరిగి readReal(p);
    }
    లేకపోతే
        తిరిగి DBL_MAX;
}

ద్విగుణ File_కణుపు::real() స్థిరరాశి  { తిరిగి ద్విగుణ(*ఇది); }

std::string File_కణుపు::string() స్థిరరాశి
{
    స్థిరరాశి uchar* p = ptr();
    ఒకవేళ( !p || (*p & TYPE_ముసుగు) != STRING )
        తిరిగి std::string();
    p += (*p & NAMED) ? 5 : 1;
    size_t sz = (size_t)(గుర్తులేని)readInt(p);
    తిరిగి std::string((స్థిరరాశి అక్షరం*)(p + 4), sz - 1);
}
Mat File_కణుపు::mat() స్థిరరాశి { Mat value; పఠ(*ఇది, value, Mat()); తిరిగి value; }

File_కణుపు_Iterator File_కణుపు::begin() స్థిరరాశి { తిరిగి File_కణుపు_Iterator(*ఇది, అబద్ధం); }
File_కణుపు_Iterator File_కణుపు::end() స్థిరరాశి   { తిరిగి File_కణుపు_Iterator(*ఇది, నిజం); }

శూన్య File_కణుపు::readRaw( స్థిరరాశి std::string& fmt, శూన్య* vec, size_t len ) స్థిరరాశి
{
    File_కణుపు_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

size_t File_కణుపు::size() స్థిరరాశి
{
    స్థిరరాశి uchar* p = ptr();
    ఒకవేళ( !p )
        తిరిగి 0;
    పూర్ణాంకం ట్యాగ్ = *p;
    పూర్ణాంకం tp = ట్యాగ్ & TYPE_ముసుగు;
    ఒకవేళ( tp == MAP || tp == SEQ )
    {
        ఒకవేళ( ట్యాగ్ & NAMED )
            p += 4;
        తిరిగి (size_t)(గుర్తులేని)readInt(p + 5);
    }
    తిరిగి tp != NONE;
}

size_t File_కణుపు::raw_కొలత() స్థిరరాశి
{
    స్థిరరాశి uchar* p0 = ptr(), *p = p0;
    ఒకవేళ( !p )
        తిరిగి 0;
    పూర్ణాంకం ట్యాగ్ = *p++;
    పూర్ణాంకం tp = ట్యాగ్ & TYPE_ముసుగు;
    ఒకవేళ( ట్యాగ్ & NAMED )
        p += 4;
    size_t sz0 = (size_t)(p - p0);
    ఒకవేళ( tp == INT )
        తిరిగి sz0 + 8;
    ఒకవేళ( tp == REAL )
        తిరిగి sz0 + 8;
    ఒకవేళ( tp == NONE )
        తిరిగి sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    తిరిగి sz0 + 4 + readInt(p);
}

uchar* File_కణుపు::ptr()
{
    తిరిగి !fs ? 0 : (uchar*)fs->get_కణుపు_Ptr(ఖండం_Idx, ofs);
}

స్థిరరాశి uchar* File_కణుపు::ptr() స్థిరరాశి
{
    తిరిగి !fs ? 0 : fs->get_కణుపు_Ptr(ఖండం_Idx, ofs);
}

శూన్య File_కణుపు::సమితి_విలువ( పూర్ణాంకం type, స్థిరరాశి శూన్య* value, పూర్ణాంకం len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    పూర్ణాంకం ట్యాగ్ = *p;
    పూర్ణాంకం current_type = ట్యాగ్ & TYPE_ముసుగు;
    CV_Assert( current_type == NONE || current_type == type );

    పూర్ణాంకం sz = 1;

    ఒకవేళ( ట్యాగ్ & NAMED )
        sz += 4;

    ఒకవేళ( type == INT )
        sz += 8;
    లేకపోతే ఒకవేళ( type == REAL )
        sz += 8;
    లేకపోతే ఒకవేళ( type == STRING )
    {
        ఒకవేళ( len < 0 )
            len = (పూర్ణాంకం)అక్షరశ్రేణి_పొడవు((స్థిరరాశి అక్షరం*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    లేకపోతే
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_కణుపు_Space(*ఇది, sz);
    *p++ = (uchar)(type | (ట్యాగ్ & NAMED));
    ఒకవేళ( ట్యాగ్ & NAMED )
        p += 4;

    ఒకవేళ( type == INT )
    {
        int64_t ival = *(స్థిరరాశి int64_t*)value;
        writeInt(p, ival);
    }
    లేకపోతే ఒకవేళ( type == REAL )
    {
        ద్విగుణ dbval = *(స్థిరరాశి ద్విగుణ*)value;
        writeReal(p, dbval);
    }
    లేకపోతే ఒకవేళ( type == STRING )
    {
        స్థిరరాశి అక్షరం* str = (స్థిరరాశి అక్షరం*)value;
        writeInt(p, len + 1);
        జ్ఞాపకం_నకలు(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_కణుపు_Iterator::File_కణుపు_Iterator()
{
    fs = 0;
    ఖండం_Idx = 0;
    ofs = 0;
    ఖండం_కొలత_2 = 0;
    కణుపు_NElems = 0;
    idx = 0;
}

File_కణుపు_Iterator::File_కణుపు_Iterator( స్థిరరాశి File_కణుపు& కణుపు, తార్కిక seek_చివర )
{
    fs = కణుపు.fs;
    idx = 0;
    ఒకవేళ( !fs )
        ఖండం_Idx = ofs = ఖండం_కొలత_2 = కణుపు_NElems = 0;
    లేకపోతే
    {
        ఖండం_Idx = కణుపు.ఖండం_Idx;
        ofs = కణుపు.ofs;

        తార్కిక collection = కణుపు.isSeq() || కణుపు.isMap();
        ఒకవేళ( కణుపు.isNone() )
        {
            కణుపు_NElems = 0;
        }
        లేకపోతే ఒకవేళ( !collection )
        {
            కణుపు_NElems = 1;
            ఒకవేళ( seek_చివర )
            {
                idx = 1;
                ofs += కణుపు.raw_కొలత();
            }
        }
        లేకపోతే
        {
            కణుపు_NElems = కణుపు.size();
            స్థిరరాశి uchar* p0 = కణుపు.ptr(), *p = p0 + 1;
            ఒకవేళ(*p0 & File_కణుపు::NAMED )
                p += 4;
            ఒకవేళ( !seek_చివర )
                ofs += (p - p0) + 8;
            లేకపోతే
            {
                size_t rawsz = (size_t)(గుర్తులేని)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = కణుపు_NElems;
            }
        }
        fs->normalize_కణుపు_Ofs(ఖండం_Idx, ofs);
        ఖండం_కొలత_2 = fs->fs_దత్తాంశం_blksz[ఖండం_Idx];
    }
}

File_కణుపు_Iterator::File_కణుపు_Iterator(స్థిరరాశి File_కణుపు_Iterator& it)
{
    fs = it.fs;
    ఖండం_Idx = it.ఖండం_Idx;
    ofs = it.ofs;
    ఖండం_కొలత_2 = it.ఖండం_కొలత_2;
    కణుపు_NElems = it.కణుపు_NElems;
    idx = it.idx;
}

File_కణుపు_Iterator& File_కణుపు_Iterator::క్రియాకారకం=(స్థిరరాశి File_కణుపు_Iterator& it)
{
    fs = it.fs;
    ఖండం_Idx = it.ఖండం_Idx;
    ofs = it.ofs;
    ఖండం_కొలత_2 = it.ఖండం_కొలత_2;
    కణుపు_NElems = it.కణుపు_NElems;
    idx = it.idx;
    తిరిగి *ఇది;
}

File_కణుపు File_కణుపు_Iterator::క్రియాకారకం *() స్థిరరాశి
{
    తిరిగి File_కణుపు(idx < కణుపు_NElems ? fs : ఖాళీ, ఖండం_Idx, ofs);
}

File_కణుపు_Iterator& File_కణుపు_Iterator::క్రియాకారకం ++ ()
{
    ఒకవేళ( idx == కణుపు_NElems || !fs )
        తిరిగి *ఇది;
    idx++;
    File_కణుపు n(fs, ఖండం_Idx, ofs);
    ofs += n.raw_కొలత();
    ఒకవేళ( ofs >= ఖండం_కొలత_2 )
    {
        fs->normalize_కణుపు_Ofs(ఖండం_Idx, ofs);
        ఖండం_కొలత_2 = fs->fs_దత్తాంశం_blksz[ఖండం_Idx];
    }
    తిరిగి *ఇది;
}

File_కణుపు_Iterator File_కణుపు_Iterator::క్రియాకారకం ++ (పూర్ణాంకం)
{
    File_కణుపు_Iterator it = *ఇది;
    ++(*ఇది);
    తిరిగి it;
}

File_కణుపు_Iterator& File_కణుపు_Iterator::క్రియాకారకం += (పూర్ణాంకం _ofs)
{
    CV_Assert( _ofs >= 0 );
    కొరకు( ; _ofs > 0; _ofs-- )
        ఇది->క్రియాకారకం ++();
    తిరిగి *ఇది;
}

File_కణుపు_Iterator& File_కణుపు_Iterator::readRaw( స్థిరరాశి String& fmt, శూన్య* _data0, size_t maxsz)
{
    ఒకవేళ( fs && idx < కణుపు_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        పూర్ణాంకం fmt_జతలు[CV_FS_MAX_FMT_జతలు*2];
        పూర్ణాంకం fmt_pair_లెక్క = fs::decodeFormat( fmt.c_str(), fmt_జతలు, CV_FS_MAX_FMT_జతలు );
        size_t esz = fs::calc_Struct_కొలత( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        కొరకు( ; maxsz > 0; maxsz--, data0 += esz )
        {
            size_t offset = 0;
            కొరకు( పూర్ణాంకం k = 0; k < fmt_pair_లెక్క; k++ )
            {
                పూర్ణాంకం elem_type = fmt_జతలు[k*2+1];
                పూర్ణాంకం elem_కొలత_2 = CV_ELEM_కొలత(elem_type);

                పూర్ణాంకం లెక్క_2 = fmt_జతలు[k*2];
                offset = align_కొలత( offset, elem_కొలత_2 );
                uchar* దత్తాంశం = data0 + offset;

                కొరకు( పూర్ణాంకం i = 0; i < లెక్క_2; i++, ++(*ఇది) )
                {
                    File_కణుపు కణుపు = *(*ఇది);
                    ఒకవేళ( కణుపు.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_కొలత_2 == 8 ? (int64_t)కణుపు : (పూర్ణాంకం)కణుపు);
                        ఎంపిక( elem_type )
                        {
                        స్థితి CV_8U:
                            *(uchar*)దత్తాంశం = saturate_cast<uchar>(ival);
                            దత్తాంశం++;
                            ఆపు;
                        స్థితి CV_8S:
                            *(అక్షరం*)దత్తాంశం = saturate_cast<schar>(ival);
                            దత్తాంశం++;
                            ఆపు;
                        స్థితి CV_Bool:
                            *(తార్కిక*)దత్తాంశం = ival != 0;
                            దత్తాంశం++;
                            ఆపు;
                        స్థితి CV_16U:
                            *(ushort*)దత్తాంశం = saturate_cast<ushort>(ival);
                            దత్తాంశం += పరిమాణం(ushort);
                            ఆపు;
                        స్థితి CV_16S:
                            *(పొట్టి*)దత్తాంశం = saturate_cast<పొట్టి>(ival);
                            దత్తాంశం += పరిమాణం(పొట్టి);
                            ఆపు;
                        స్థితి CV_32U:
                            *(గుర్తులేని*)దత్తాంశం = (గుర్తులేని)std::max(ival, (int64_t)0);
                            దత్తాంశం += పరిమాణం(గుర్తులేని);
                            ఆపు;
                        స్థితి CV_32S:
                            *(పూర్ణాంకం*)దత్తాంశం = (పూర్ణాంకం)ival;
                            దత్తాంశం += పరిమాణం(పూర్ణాంకం);
                            ఆపు;
                        స్థితి CV_32F:
                            *(దశాంశం*)దత్తాంశం = (దశాంశం)ival;
                            దత్తాంశం += పరిమాణం(దశాంశం);
                            ఆపు;
                        స్థితి CV_64U:
                            *(uint64_t*)దత్తాంశం = (uint64_t)ival;
                            దత్తాంశం += పరిమాణం(uint64_t);
                            ఆపు;
                        స్థితి CV_64S:
                            *(int64_t*)దత్తాంశం = (int64_t)ival;
                            దత్తాంశం += పరిమాణం(int64_t);
                            ఆపు;
                        స్థితి CV_64F:
                            *(ద్విగుణ*)దత్తాంశం = (ద్విగుణ)ival;
                            దత్తాంశం += పరిమాణం(ద్విగుణ);
                            ఆపు;
                        స్థితి CV_16F:
                            *(hfloat*)దత్తాంశం = hfloat((దశాంశం)ival);
                            దత్తాంశం += పరిమాణం(hfloat);
                            ఆపు;
                        స్థితి CV_16BF:
                            *(bfloat*)దత్తాంశం = bfloat((దశాంశం)ival);
                            దత్తాంశం += పరిమాణం(bfloat);
                            ఆపు;
                        స్థితి CV_8F_E4M3FN:
                            *(fp8_t*)దత్తాంశం = fp8_t((దశాంశం)ival);
                            దత్తాంశం += పరిమాణం(fp8_t);
                            ఆపు;
                        స్థితి CV_8F_E4M3FNUZ:
                            *(fp8a_t*)దత్తాంశం = fp8a_t((దశాంశం)ival);
                            దత్తాంశం += పరిమాణం(fp8a_t);
                            ఆపు;
                        అప్రమేయ:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    లేకపోతే ఒకవేళ( కణుపు.isReal() )
                    {
                        ద్విగుణ fval = (ద్విగుణ)కణుపు;

                        ఎంపిక( elem_type )
                        {
                        స్థితి CV_8U:
                            *(uchar*)దత్తాంశం = saturate_cast<uchar>(fval);
                            దత్తాంశం++;
                            ఆపు;
                        స్థితి CV_8S:
                            *(అక్షరం*)దత్తాంశం = saturate_cast<schar>(fval);
                            దత్తాంశం++;
                            ఆపు;
                        స్థితి CV_16U:
                            *(ushort*)దత్తాంశం = saturate_cast<ushort>(fval);
                            దత్తాంశం += పరిమాణం(ushort);
                            ఆపు;
                        స్థితి CV_16S:
                            *(పొట్టి*)దత్తాంశం = saturate_cast<పొట్టి>(fval);
                            దత్తాంశం += పరిమాణం(పొట్టి);
                            ఆపు;
                        స్థితి CV_32U:
                            *(పూర్ణాంకం*)దత్తాంశం = saturate_cast<గుర్తులేని>(fval);
                            దత్తాంశం += పరిమాణం(పూర్ణాంకం);
                            ఆపు;
                        స్థితి CV_32S:
                            *(పూర్ణాంకం*)దత్తాంశం = saturate_cast<పూర్ణాంకం>(fval);
                            దత్తాంశం += పరిమాణం(పూర్ణాంకం);
                            ఆపు;
                        స్థితి CV_32F:
                            *(దశాంశం*)దత్తాంశం = (దశాంశం)fval;
                            దత్తాంశం += పరిమాణం(దశాంశం);
                            ఆపు;
                        స్థితి CV_64U:
                            *(uint64_t*)దత్తాంశం = (uint64_t)round(std::max(fval, 0.));
                            దత్తాంశం += పరిమాణం(uint64_t);
                            ఆపు;
                        స్థితి CV_64S:
                            *(int64_t*)దత్తాంశం = (int64_t)round(std::max(fval, 0.));
                            దత్తాంశం += పరిమాణం(int64_t);
                            ఆపు;
                        స్థితి CV_64F:
                            *(ద్విగుణ*)దత్తాంశం = fval;
                            దత్తాంశం += పరిమాణం(ద్విగుణ);
                            ఆపు;
                        స్థితి CV_16F:
                            *(hfloat*)దత్తాంశం = hfloat((దశాంశం)fval);
                            దత్తాంశం += పరిమాణం(hfloat);
                            ఆపు;
                        స్థితి CV_16BF:
                            *(bfloat*)దత్తాంశం = bfloat((దశాంశం)fval);
                            దత్తాంశం += పరిమాణం(bfloat);
                            ఆపు;
                        స్థితి CV_8F_E4M3FN:
                            *(fp8_t*)దత్తాంశం = fp8_t((దశాంశం)fval);
                            దత్తాంశం += పరిమాణం(fp8_t);
                            ఆపు;
                        స్థితి CV_8F_E4M3FNUZ:
                            *(fp8a_t*)దత్తాంశం = fp8a_t((దశాంశం)fval);
                            దత్తాంశం += పరిమాణం(fp8a_t);
                            ఆపు;
                        అప్రమేయ:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    లేకపోతే
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (పూర్ణాంకం)(దత్తాంశం - data0);
            }
        }
    }

    తిరిగి *ఇది;
}

తార్కిక File_కణుపు_Iterator::equal_గమ్యం(స్థిరరాశి File_కణుపు_Iterator& it) స్థిరరాశి
{
    తిరిగి fs == it.fs && ఖండం_Idx == it.ఖండం_Idx && ofs == it.ofs &&
           idx == it.idx && కణుపు_NElems == it.కణుపు_NElems;
}

size_t File_కణుపు_Iterator::మిగిలిన() స్థిరరాశి
{
    తిరిగి కణుపు_NElems - idx;
}

తార్కిక క్రియాకారకం == ( స్థిరరాశి File_కణుపు_Iterator& it1, స్థిరరాశి File_కణుపు_Iterator& it2 )
{
    తిరిగి it1.equal_గమ్యం(it2);
}

తార్కిక క్రియాకారకం != ( స్థిరరాశి File_కణుపు_Iterator& it1, స్థిరరాశి File_కణుపు_Iterator& it2 )
{
    తిరిగి !it1.equal_గమ్యం(it2);
}

శూన్య పఠ(స్థిరరాశి File_కణుపు& కణుపు, పూర్ణాంకం& val, పూర్ణాంకం default_val)
{
    val = default_val;
    ఒకవేళ( !కణుపు.empty() )
    {
        val = (పూర్ణాంకం)కణుపు;
    }
}

శూన్య పఠ(స్థిరరాశి File_కణుపు& కణుపు, int64_t& val, int64_t default_val)
{
    val = default_val;
    ఒకవేళ( !కణుపు.empty() )
    {
        val = (int64_t)కణుపు;
    }
}

శూన్య పఠ(స్థిరరాశి File_కణుపు& కణుపు, ద్విగుణ& val, ద్విగుణ default_val)
{
    val = default_val;
    ఒకవేళ( !కణుపు.empty() )
    {
        val = (ద్విగుణ)కణుపు;
    }
}

శూన్య పఠ(స్థిరరాశి File_కణుపు& కణుపు, దశాంశం& val, దశాంశం default_val)
{
    val = default_val;
    ఒకవేళ( !కణుపు.empty() )
    {
        val = (దశాంశం)కణుపు;
    }
}

శూన్య పఠ(స్థిరరాశి File_కణుపు& కణుపు, std::string& val, స్థిరరాశి std::string& default_val)
{
    val = default_val;
    ఒకవేళ( !కణుపు.empty() )
    {
        val = (std::string)కణుపు;
    }
}
శూన్య FileStorage::Impl::లిఖ(స్థిరరాశి String &కీలకం, తార్కిక value)
{
    CV_Assert(write_బహుళకం);
    ఒకవేళ (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().లిఖ(కీలకం.c_str(), (పూర్ణాంకం)value);
    }
    లేకపోతే
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().లిఖ(కీలకం.c_str(), value ? "true" : "false", అబద్ధం);
    }
}

FileStorage_API::~FileStorage_API() {}

నామాకాశ internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, స్థిరరాశి std::string& name,
                                       పూర్ణాంకం flags, స్థిరరాశి std::string& typeName)
{
    fs = &_fs;
    fs->ప్రారంభం_Write_Struct(name, flags, typeName);
}

WriteStructContext::~WriteStructContext()
{
    fs->చివర_Write_Struct();
}

}

}
