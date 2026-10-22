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

#define CV_SEQ_ELTYPE_PTR            CV_MAKE_TYPE(CV_8U, 8 /*sizeof(void*)*/)

नामाकाश cv
{

नामाकाश fs
{

पूर्णांक strcasecmp(स्थिरांक अक्षर* s1, स्थिरांक अक्षर* s2)
{
    स्थिरांक अक्षर* dummy="";
    जर(!s1) s1=dummy;
    जर(!s2) s2=dummy;

    माप_प्रकार len1 = म_माप(s1);
    माप_प्रकार len2 = म_माप(s2);
    माप_प्रकार i, len = std::min(len1, len2);
    साठी( i = 0; i < len; i++ )
    {
        पूर्णांक d = छोटे((पूर्णांक)s1[i]) - छोटे((पूर्णांक)s2[i]);
        जर( d != 0 )
            परत d;
    }
    परत len1 < len2 ? -1 : len1 > len2 ? 1 : 0;
}

अक्षर* itoa( पूर्णांक _val, अक्षर* buffer, पूर्णांक /*radix*/ )
{
    स्थिरांक पूर्णांक radix = 10;
    अक्षर* ptr=buffer + 23 /* enough even for 64-bit integers */;
    अचिन्हित val = असल(_val);

    *ptr = '\0';
    करा
    {
        अचिन्हित r = val / radix;
        *--ptr = (अक्षर)(val - (r*radix) + '0');
        val = r;
    }
    पर्यंत( val != 0 );

    जर( _val < 0 )
        *--ptr = '-';

    परत ptr;
}

अक्षर* itoa( int64_t _val, अक्षर* buffer, पूर्णांक /*radix*/, तार्किक _signed)
{
    स्थिरांक पूर्णांक radix = 10;
    अक्षर* ptr=buffer + 23 /* enough even for 64-bit integers */;
    पूर्णांक sign = _signed && _val < 0 ? -1 : 1;
    uint64_t val = !_signed ? (uint64_t)_val : असल(_val);

    *ptr = '\0';
    करा
    {
        uint64_t r = val / radix;
        *--ptr = (अक्षर)(val - (r*radix) + '0');
        val = r;
    }
    पर्यंत( val != 0 );

    जर( sign < 0 )
        *--ptr = '-';

    परत ptr;
}

अक्षर* doubleToString( अक्षर* buf, माप_प्रकार bufSize, द्विगुण value, तार्किक explicitZero )
{
    Cv64suf val;
    अचिन्हित ieee754_hi;

    val.f = value;
    ieee754_hi = (अचिन्हित)(val.u >> 32);

    जर( (ieee754_hi & 0x7ff00000) != 0x7ff00000 )
    {
        पूर्णांक ivalue = cvRound(value);
        जर( ivalue == value )
        {
            जर( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            अन्यथा
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        अन्यथा
        {
            // binary64 has 52 bit fraction with hidden bit.
            // 53 * log_10(2) is 15.955. So "%.16f" should be fine, but its test fails.
            snprintf( buf, bufSize, "%.17g", value );

            अक्षर* ptr = buf;
            जर( *ptr == '+' || *ptr == '-' )
                ptr++;
            साठी( ; cv_isdigit(*ptr); ptr++ )
                ;
            जर( *ptr == ',' )
                *ptr = '.';
        }
    }
    अन्यथा
    {
        अचिन्हित ieee754_lo = (अचिन्हित)val.u;
        जर( (ieee754_hi & 0x7fffffff) + (ieee754_lo != 0) > 0x7ff00000 )
            म_नकल( buf, ".Nan" );
        अन्यथा
            म_नकल( buf, (पूर्णांक)ieee754_hi < 0 ? "-.Inf" : ".Inf" );
    }

    परत buf;
}

अक्षर* floatToString( अक्षर* buf, माप_प्रकार bufSize, दशांश value, तार्किक halfprecision, तार्किक explicitZero )
{
    Cv32suf val;
    अचिन्हित ieee754;
    val.f = value;
    ieee754 = val.u;

    जर( (ieee754 & 0x7f800000) != 0x7f800000 )
    {
        पूर्णांक ivalue = cvRound(value);
        जर( ivalue == value )
        {
            जर( explicitZero )
                snprintf( buf, bufSize, "%d.0", ivalue );
            अन्यथा
                snprintf( buf, bufSize, "%d.", ivalue );
        }
        अन्यथा
        {
            जर (halfprecision)
            {
                // bfloat16 has 7 bit fraction with hidden bit.
                // binary16 has 10 bit fraction with hidden bit.
                // 11 * log_10(2) is 3.311. So "%.4f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.5g", value);
            }
            अन्यथा
            {
                // binray32 has 23 bit fraction with hidden bit.
                // 24 * log_10(2) is 7.225. So "%.8f" should be fine, but its test fails.
                snprintf(buf, bufSize, "%.9g", value);
            }

            अक्षर* ptr = buf;
            जर( *ptr == '+' || *ptr == '-' )
                ptr++;
            साठी( ; cv_isdigit(*ptr); ptr++ )
                ;
            जर( *ptr == ',' )
                *ptr = '.';
        }
    }
    अन्यथा
    {
        जर( (ieee754 & 0x7fffffff) != 0x7f800000 )
            म_नकल( buf, ".Nan" );
        अन्यथा
            म_नकल( buf, (पूर्णांक)ieee754 < 0 ? "-.Inf" : ".Inf" );
    }

    परत buf;
}

स्थिर स्थिरांक अक्षर symbols[] = "ucwsifdhHbUIneE";

स्थिर अक्षर type_चिन्ह(पूर्णांक depth)
{
    CV_StaticAssert(CV_64F == 6, "");
    CV_CheckDepth(depth, depth >= 0 && depth <= CV_8F_E4M3FNUZ, "");
    परत symbols[depth];
}

स्थिर पूर्णांक चिन्ह_To_Type(अक्षर c)
{
    जर (c == 'r')
        परत CV_SEQ_ELTYPE_PTR;
    स्थिरांक अक्षर* pos = म_अक्षर( symbols, c );
    जर( !pos )
        CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );
    परत static_cast<पूर्णांक>(pos - symbols);
}

अक्षर* encodeFormat(पूर्णांक elem_type, अक्षर* dt, माप_प्रकार dt_len)
{
    पूर्णांक cn = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 1 : CV_MAT_CN(elem_type);
    अक्षर चिन्ह = (elem_type == CV_SEQ_ELTYPE_PTR/*CV_USRTYPE1*/) ? 'r' : type_चिन्ह(CV_MAT_DEPTH(elem_type));
    snprintf(dt, dt_len, "%d%c", cn, चिन्ह);
    परत dt + (cn == 1 ? 1 : 0);
}

// Deprecated due to size of dt buffer being unknowable.
अक्षर* encodeFormat(पूर्णांक elem_type, अक्षर* dt)
{
    constexpr माप_प्रकार max = 20+1+1; // UINT64_MAX + one char + nul termination.
    परत encodeFormat(elem_type, dt, max);
}

पूर्णांक decodeFormat( स्थिरांक अक्षर* dt, पूर्णांक* fmt_pairs, पूर्णांक max_len )
{
    पूर्णांक fmt_pair_मोजणी = 0;
    पूर्णांक i = 0, k = 0, len = dt ? (पूर्णांक)म_माप(dt) : 0;

    जर( !dt || !len )
        परत 0;

    CV_Assert( fmt_pairs != 0 && max_len > 0 );
    fmt_pairs[0] = 0;
    max_len *= 2;

    साठी( ; k < len; k++ )
    {
        अक्षर c = dt[k];

        जर( cv_isdigit(c) )
        {
            पूर्णांक मोजणी_2 = c - '0';
            जर( cv_isdigit(dt[k+1]) )
            {
                अक्षर* endptr = 0;
                मोजणी_2 = (पूर्णांक)म_से_दीर्घ( dt+k, &endptr, 10 );
                k = (पूर्णांक)(endptr - dt) - 1;
            }

            जर( मोजणी_2 <= 0 )
                CV_Error( cv::Error::StsBadArg, "Invalid data type specification" );

            fmt_pairs[i] = मोजणी_2;
        }
        अन्यथा
        {
            पूर्णांक depth = चिन्ह_To_Type(c);
            जर( fmt_pairs[i] == 0 )
                fmt_pairs[i] = 1;
            fmt_pairs[i+1] = depth;
            जर( i > 0 && fmt_pairs[i+1] == fmt_pairs[i-1] )
                fmt_pairs[i-2] += fmt_pairs[i];
            अन्यथा
            {
                i += 2;
                जर( i >= max_len )
                    CV_Error( cv::Error::StsBadArg, "Too long data type specification" );
            }
            fmt_pairs[i] = 0;
        }
    }

    fmt_pair_मोजणी = i/2;
    परत fmt_pair_मोजणी;
}

पूर्णांक calcElemSize( स्थिरांक अक्षर* dt, पूर्णांक initial_size )
{
    पूर्णांक size = 0;
    पूर्णांक fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], i, fmt_pair_मोजणी;
    पूर्णांक comp_size;

    fmt_pair_मोजणी = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    fmt_pair_मोजणी *= 2;
    साठी( i = 0, size = initial_size; i < fmt_pair_मोजणी; i += 2 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[i+1]);
        size = cvAlign( size, comp_size );
        size += comp_size * fmt_pairs[i];
    }
    जर( initial_size == 0 )
    {
        comp_size = CV_ELEM_SIZE(fmt_pairs[1]);
        size = cvAlign( size, comp_size );
    }
    परत size;
}


पूर्णांक calcStructSize( स्थिरांक अक्षर* dt, पूर्णांक initial_size )
{
    पूर्णांक size = calcElemSize( dt, initial_size );
    माप_प्रकार elem_max_size = 0;
    साठी ( स्थिरांक अक्षर * type = dt; *type != '\0'; type++ )
    {
        अक्षर v = *type;
        जर (v >= '0' && v <= '9')
            सुरूठेवा;  // skip vector size
        निवड (v)
        {
        स्थिती 'u': { elem_max_size = std::max( elem_max_size, आकार(uchar ) ); थांबा; }
        स्थिती 'b': { elem_max_size = std::max( elem_max_size, आकार(तार्किक  ) ); थांबा; }
        स्थिती 'c': { elem_max_size = std::max( elem_max_size, आकार(schar ) ); थांबा; }
        स्थिती 'w': { elem_max_size = std::max( elem_max_size, आकार(ushort) ); थांबा; }
        स्थिती 's': { elem_max_size = std::max( elem_max_size, आकार(लघु ) ); थांबा; }
        स्थिती 'i': { elem_max_size = std::max( elem_max_size, आकार(पूर्णांक   ) ); थांबा; }
        स्थिती 'n': { elem_max_size = std::max( elem_max_size, आकार(अचिन्हित) ); थांबा; }
        स्थिती 'f': { elem_max_size = std::max( elem_max_size, आकार(दशांश ) ); थांबा; }
        स्थिती 'd': { elem_max_size = std::max( elem_max_size, आकार(द्विगुण) ); थांबा; }
        स्थिती 'h': { elem_max_size = std::max( elem_max_size, आकार(hfloat)); थांबा; }
        स्थिती 'H': { elem_max_size = std::max( elem_max_size, आकार(bfloat)); थांबा; }
        स्थिती 'e': { elem_max_size = std::max( elem_max_size, आकार(fp8_t) ); थांबा; }
        स्थिती 'E': { elem_max_size = std::max( elem_max_size, आकार(fp8a_t)); थांबा; }
        स्थिती 'I': { elem_max_size = std::max( elem_max_size, आकार(int64_t)); थांबा; }
        स्थिती 'U': { elem_max_size = std::max( elem_max_size, आकार(uint64_t)); थांबा; }
        पूर्वनिर्धारित:
            CV_Error_(Error::StsNotImplemented, ("Unknown type identifier: '%c' in '%s'", (अक्षर)(*type), dt));
        }
    }
    size = cvAlign( size, static_cast<पूर्णांक>(elem_max_size) );
    परत size;
}

पूर्णांक decodeSimpleFormat( स्थिरांक अक्षर* dt )
{
    पूर्णांक elem_type = -1;
    पूर्णांक fmt_pairs[CV_FS_MAX_FMT_PAIRS*2], fmt_pair_मोजणी;

    fmt_pair_मोजणी = decodeFormat( dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS );
    जर( fmt_pair_मोजणी != 1 || fmt_pairs[0] >= CV_CN_MAX)
        CV_Error( cv::Error::StsError, "Too complex format for the matrix" );

    elem_type = CV_MAKETYPE( fmt_pairs[1], fmt_pairs[0] );

    परत elem_type;
}

}

#जर defined __i386__ || defined(_M_IX86) || defined __x86_64__ || defined(_M_X64) || \
    (defined (__LITTLE_ENDIAN__) && __LITTLE_ENDIAN__)
#define CV_LITTLE_ENDIAN_MEM_ACCESS 1
#अन्यथा
#define CV_LITTLE_ENDIAN_MEM_ACCESS 0
#endif

स्थिर ओळीत पूर्णांक readInt(स्थिरांक uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#जर CV_LITTLE_ENDIAN_MEM_ACCESS
    पूर्णांक val;
    स_नकल(&val, p, आकार(val));
    परत val;
#अन्यथा
    पूर्णांक val = (पूर्णांक)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    परत val;
#endif
}

स्थिर ओळीत int64_t readLong(स्थिरांक uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#जर CV_LITTLE_ENDIAN_MEM_ACCESS
    int64_t val;
    स_नकल(&val, p, आकार(val));
    परत val;
#अन्यथा
    अचिन्हित val0 = (अचिन्हित)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    अचिन्हित val1 = (अचिन्हित)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    परत val0 | ((int64_t)val1 << 32);
#endif
}

स्थिर ओळीत द्विगुण readReal(स्थिरांक uchar* p)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#जर CV_LITTLE_ENDIAN_MEM_ACCESS
    द्विगुण val;
    स_नकल(&val, p, आकार(val));
    परत val;
#अन्यथा
    अचिन्हित val0 = (अचिन्हित)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
    अचिन्हित val1 = (अचिन्हित)(p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24));
    Cv64suf val;
    val.u = val0 | ((uint64)val1 << 32);
    परत val.f;
#endif
}

ढाँचा <typename T>
स्थिर ओळीत शून्य writeInt(uchar* p, T ival)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#जर CV_LITTLE_ENDIAN_MEM_ACCESS
    स_नकल(p, &ival, आकार(ival));
#अन्यथा
    साठी (माप_प्रकार i = 0, j = 0; i < आकार(ival); ++i, j += 8)
        p[i] = (uchar)(ival >> j);
#endif
}

स्थिर ओळीत शून्य writeReal(uchar* p, द्विगुण fval)
{
    // On little endian CPUs, both branches produce the same result. On big endian, only the else branch does.
#जर CV_LITTLE_ENDIAN_MEM_ACCESS
    स_नकल(p, &fval, आकार(fval));
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
#endif
}



शून्य FileStorage::Impl::init() {
    flags = 0;
    buffer.clear();
    bufofs = 0;
    state = UNDEFINED;
    is_using_base64 = असत्य;
    state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;
    is_write_struct_delayed = असत्य;
    delayed_struct_किल्ली = nullptr;
    delayed_struct_flags = 0;
    delayed_type_name = nullptr;
    base64_writer = nullptr;
    is_opened = असत्य;
    dummy_eof = असत्य;
    write_mode = असत्य;
    mem_mode = असत्य;
    space = 0;
    wrap_margin = 71;
    fmt = 0;
    file = 0;
    gzfile = 0;
    empty_stream = सत्य;

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

शून्य FileStorage::Impl::release(String *out) {
    जर (is_opened) {
        जर (out)
            out->clear();
        जर (write_mode) {
            पर्यंत (write_चळत.size() > 1) {
                endWriteStruct();
            }
            flush();
            जर (fmt == FileStorage::FORMAT_XML)
                माला_दो("</opencv_storage>\n");
            अन्यथा जर (fmt == FileStorage::FORMAT_JSON)
                माला_दो("}\n");
        }
        जर (mem_mode && out) {
            *out = cv::String(outbuf.begin(), outbuf.end());
        }
    }
    closeFile();
    init();
}

शून्य FileStorage::Impl::analyze_file_name(स्थिरांक std::string &file_name, std::vector<std::string> &params) {
    params.clear();
    स्थिर स्थिरांक अक्षर not_file_name = '\n';
    स्थिर स्थिरांक अक्षर parameter_begin = '?';
    स्थिर स्थिरांक अक्षर parameter_separator = '&';

    जर (file_name.find(not_file_name, (माप_प्रकार) 0) != std::string::npos)
        परत;

    माप_प्रकार beg = file_name.find_last_of(parameter_begin);
    params.push_back(file_name.substr((माप_प्रकार) 0, beg));

    जर (beg != std::string::npos) {
        माप_प्रकार end = file_name.size();
        beg++;
        साठी (माप_प्रकार param_beg = beg, param_end = beg;
             param_end < end;
             param_beg = param_end + 1) {
            param_end = file_name.find_first_of(parameter_separator, param_beg);
            जर ((param_end == std::string::npos || param_end != param_beg) && param_beg + 1 < end) {
                params.push_back(file_name.substr(param_beg, param_end - param_beg));
            }
        }
    }
}

तार्किक FileStorage::Impl::खोलो(स्थिरांक अक्षर *filename_or_buf, पूर्णांक _flags, स्थिरांक अक्षर *encoding) {
    तार्किक ok = सत्य;
    release();

    तार्किक append = (_flags & 3) == FileStorage::APPEND;
    mem_mode = (_flags & FileStorage::MEMORY) != 0;

    write_mode = (_flags & 3) != 0;
    तार्किक write_base64 = (write_mode || append) && (_flags & FileStorage::BASE64) != 0;

    तार्किक isGZ = असत्य;

    std::vector<std::string> params;
    //if ( !mem_mode )
    {
        analyze_file_name(filename_or_buf, params);
        जर (!params.empty())
            filename = params[0];

        जर (!write_base64 && params.size() >= 2 &&
            std::find(params.begin() + 1, params.end(), std::string("base64")) != params.end())
            write_base64 = (write_mode || append);
    }

    जर (filename.size() == 0 && !mem_mode && !write_mode)
        CV_Error(cv::Error::StsNullPtr, "NULL or empty filename");

    जर (mem_mode && append)
        CV_Error(cv::Error::StsBadFlag, "FileStorage::APPEND and FileStorage::MEMORY are not currently compatible");

    flags = _flags;

    जर (!mem_mode) {
        माप_प्रकार dot_idx = filename.find_last_of('.');
        अक्षर compression = '\0';

        जर (dot_idx != std::string::npos)
        {
            std::string ext = filename.substr(dot_idx);

            // Instead of `ext.starts_with(".gz")
            जर (ext.size() >= 3 && (ext[1] == 'g') && (ext[2] == 'z') )
            {
                जर (ext.size() == 3)
                {
                    // ".gz"
                    isGZ = सत्य;
                }

                अन्यथा जर(ext.size() == 4 && cv_isdigit(ext[3]))
                {
                    // ".gz[0-9]"
                    isGZ = सत्य;
                    compression = ext[3];
                    filename.pop_back(); // Replace `gz[0-9]' to 'gz'.
                }
            }
        }

        जर (!isGZ) {
            file = ख_खोलो(filename.c_str(), !write_mode ? "rt" : !append ? "wt" : "a+t");
            जर (!file)
            {
                CV_LOG_ERROR(रिक्त, "Can't open file: '" << filename << "' in " << (!write_mode ? "read" : !append ? "write" : "append") << " mode");
                परत असत्य;
            }
        } अन्यथा {
#जर USE_ZLIB
            जर (append) {
                CV_Error(cv::Error::StsNotImplemented, "Appending data to compressed file is not implemented");
            }
            अक्षर mode[] = {write_mode ? 'w' : 'r', 'b', compression ? compression : '3', '\0'};
            gzfile = gzopen(filename.c_str(), mode);
            जर (!gzfile)
            {
                CV_LOG_ERROR(रिक्त, "Can't open archive: '" << filename << "' mode=" << mode);
                परत असत्य;
            }
#अन्यथा
            CV_Error(cv::Error::StsNotImplemented, "There is no compressed file storage support in this configuration");
#endif
        }
    }

    // FIXIT release() must do that, use CV_Assert() here instead
    roots.clear();
    fs_data.clear();

    wrap_margin = 71;
    fmt = FileStorage::FORMAT_AUTO;

    जर (write_mode) {
        fmt = flags & FileStorage::FORMAT_MASK;

        जर (mem_mode)
            outbuf.clear();

        जर (fmt == FileStorage::FORMAT_AUTO && !filename.empty()) {
            स्थिरांक अक्षर *dot_pos = रिक्त;
            स्थिरांक अक्षर *dot_pos2 = रिक्त;
            // like strrchr() implementation, but save two last positions simultaneously
            साठी (स्थिरांक अक्षर *pos = &filename[0]; pos[0] != 0; pos++) {
                जर (pos[0] == '.') {
                    dot_pos2 = dot_pos;
                    dot_pos = pos;
                }
            }
            जर (fs::strcasecmp(dot_pos, ".gz") == 0 && dot_pos2 != रिक्त) {
                dot_pos = dot_pos2;
            }
            fmt = (fs::strcasecmp(dot_pos, ".xml") == 0 || fs::strcasecmp(dot_pos, ".xml.gz") == 0)
                  ? FileStorage::FORMAT_XML
                  : (fs::strcasecmp(dot_pos, ".json") == 0 || fs::strcasecmp(dot_pos, ".json.gz") == 0)
                    ? FileStorage::FORMAT_JSON
                    : FileStorage::FORMAT_YAML;
        } अन्यथा जर (fmt == FileStorage::FORMAT_AUTO) {
            fmt = FileStorage::FORMAT_XML;
        }

        // we use factor=6 for XML (the longest characters (' and ") are encoded with 6 bytes (&apos; and &quot;)
        // and factor=4 for YAML ( as we use 4 bytes for non ASCII characters (e.g. \xAB))
        पूर्णांक buf_size = CV_FS_MAX_LEN * (fmt == FileStorage::FORMAT_XML ? 6 : 4) + 1024;

        जर (append) {
            ख_जाओ(file, 0, अंत_से);
            जर (ख_बताओ(file) == 0)
                append = असत्य;
        }

        write_चळत.clear();
        empty_stream = सत्य;
        write_चळत.push_back(FStructData("", File_गाठ::MAP | File_गाठ::EMPTY, 0));
        buffer.reserve(buf_size + 1024);
        buffer.resize(buf_size);
        bufofs = 0;
        is_using_base64 = write_base64;
        state_of_writing_base64 = FileStorage_API::Base64State::Uncertain;

        जर (fmt == FileStorage::FORMAT_XML) {
            माप_प्रकार file_size = file ? (माप_प्रकार) ख_बताओ(file) : (माप_प्रकार) 0;
            जर (!append || file_size == 0) {
                जर (encoding && *encoding != '\0') {
                    जर (fs::strcasecmp(encoding, "UTF-16") == 0) {
                        release();
                        CV_Error(cv::Error::StsBadArg, "UTF-16 XML encoding is not supported! Use 8-bit encoding\n");
                    }

                    CV_Assert(म_माप(encoding) < 1000);
                    अक्षर buf[1100];
                    snprintf(buf, आकार(buf), "<?xml version=\"1.0\" encoding=\"%s\"?>\n", encoding);
                    माला_दो(buf);
                } अन्यथा
                    माला_दो("<?xml version=\"1.0\"?>\n");
                माला_दो("<opencv_storage>\n");
            } अन्यथा {
                पूर्णांक xml_buf_size = 1 << 10;
                अक्षर substr[] = "</opencv_storage>";
                पूर्णांक last_occurrence = -1;
                xml_buf_size = MIN(xml_buf_size, पूर्णांक(file_size));
                ख_जाओ(file, -xml_buf_size, अंत_से);
                // find the last occurrence of </opencv_storage>
                साठी (;;) {
                    पूर्णांक ओळ_offset = (पूर्णांक) ख_बताओ(file);
                    स्थिरांक अक्षर *ptr0 = हे->माला_लो(xml_buf_size);
                    स्थिरांक अक्षर *ptr = रिक्त;
                    जर (!ptr0)
                        थांबा;
                    ptr = ptr0;
                    साठी (;;) {
                        ptr = म_माला(ptr, substr);
                        जर (!ptr)
                            थांबा;
                        last_occurrence = ओळ_offset + (पूर्णांक) (ptr - ptr0);
                        ptr += म_माप(substr);
                    }
                }
                जर (last_occurrence < 0) {
                    release();
                    CV_Error(cv::Error::StsError, "Could not find </opencv_storage> in the end of file.\n");
                }
                closeFile();
                file = ख_खोलो(filename.c_str(), "r+t");
                CV_Assert(file != 0);
                ख_जाओ(file, last_occurrence, शुरू_से);
                // replace the last "</opencv_storage>" with " <!-- resumed -->", which has the same length
                माला_दो(" <!-- resumed -->");
                ख_जाओ(file, 0, अंत_से);
                माला_दो("\n");
            }

            emitter_do_not_use_direct_dereference = createXMLEmitter(हे);
        } अन्यथा जर (fmt == FileStorage::FORMAT_YAML || fmt == FileStorage::FORMAT_YAML_1_0) {
                जर (!append) {
                    जर (fmt == FileStorage::FORMAT_YAML_1_0)
                        माला_दो("%YAML:1.0\n---\n"); // Legacy Flag -> Legacy Header
                    अन्यथा
                        माला_दो("%YAML 1.2\n---\n"); // Default -> Modern Header
                }
            अन्यथा
                माला_दो("...\n---\n");

            emitter_do_not_use_direct_dereference = createYAMLEmitter(हे);
        } अन्यथा {
            CV_Assert(fmt == FileStorage::FORMAT_JSON);
            जर (!append)
                माला_दो("{\n");
            अन्यथा {
                तार्किक valid = असत्य;
                दीर्घ roffset = 0;
                साठी (;
                        ख_जाओ(file, roffset, अंत_से) == 0;
                        roffset -= 1) {
                    स्थिरांक अक्षर end_mark = '}';
                    जर (ख_अक्षर_लो(file) == end_mark) {
                        ख_जाओ(file, roffset, अंत_से);
                        valid = सत्य;
                        थांबा;
                    }
                }

                जर (valid) {
                    closeFile();
                    file = ख_खोलो(filename.c_str(), "r+t");
                    CV_Assert(file != 0);
                    ख_जाओ(file, roffset, अंत_से);
                    ख_माला_दो(",", file);
                } अन्यथा {
                    CV_Error(cv::Error::StsError, "Could not find '}' in the end of file.\n");
                }
            }
            write_चळत.back().indent = 4;
            emitter_do_not_use_direct_dereference = createJSONEmitter(हे);
        }
        is_opened = सत्य;
    } अन्यथा {
        स्थिरांक माप_प्रकार buf_size0 = 40;
        buffer.resize(buf_size0);
        जर (mem_mode) {
            strbuf = (अक्षर *) filename_or_buf;
            strbufsize = म_माप(strbuf);
        }

        स्थिरांक अक्षर *yaml_signature = "%YAML";
        स्थिरांक अक्षर *json_signature = "{";
        स्थिरांक अक्षर *xml_signature = "<?xml";
        अक्षर *buf = हे->माला_लो(16);
        CV_Assert(buf);
        अक्षर *bufPtr = cv_skip_BOM(buf);
        माप_प्रकार bufOffset = bufPtr - buf;

        // NOTE: Yaml 1.2 allows to skip header. Data without type signature is read as YAML
        जर (म_भेदन(bufPtr, yaml_signature, म_माप(yaml_signature)) == 0)
            fmt = FileStorage::FORMAT_YAML;
        अन्यथा जर (म_भेदन(bufPtr, json_signature, म_माप(json_signature)) == 0)
            fmt = FileStorage::FORMAT_JSON;
        अन्यथा जर (म_भेदन(bufPtr, xml_signature, म_माप(xml_signature)) == 0)
            fmt = FileStorage::FORMAT_XML;
        अन्यथा
            fmt = FileStorage::FORMAT_YAML;

        शुरुआत();
        strbufpos = bufOffset;
        bufofs = 0;

        प्रयत्न {
            अक्षर *ptr = bufferStart();
            ptr[0] = ptr[1] = ptr[2] = '\0';
            File_गाठ मूळ_nodes(fs_ext, 0, 0);

            uchar *rptr = reserve_गाठ_Space(मूळ_nodes, 9);
            *rptr = File_गाठ::SEQ;
            writeInt(rptr + 1, 4);
            writeInt(rptr + 5, 0);

            roots.clear();

            निवड (fmt) {
                स्थिती FileStorage::FORMAT_XML:
                    parser_do_not_use_direct_dereference = createXMLParser(हे);
                    थांबा;
                स्थिती FileStorage::FORMAT_YAML:
                स्थिती FileStorage::FORMAT_YAML_1_0:
                    parser_do_not_use_direct_dereference = createYAMLParser(हे);
                    थांबा;
                स्थिती FileStorage::FORMAT_JSON:
                    parser_do_not_use_direct_dereference = createJSONParser(हे);
                    थांबा;
                पूर्वनिर्धारित:
                    parser_do_not_use_direct_dereference = Ptr<FileStorageParser>();
            }

            जर (!parser_do_not_use_direct_dereference.empty()) {
                ok = getParser().parse(ptr);
                जर (ok) {
                    finalizeCollection(मूळ_nodes);

                    CV_Assert(!fs_data_ptrs.empty());
                    File_गाठ roots_गाठ(fs_ext, 0, 0);
                    माप_प्रकार i, nroots = roots_गाठ.size();
                    File_गाठ_Iterator it = roots_गाठ.begin();

                    साठी (i = 0; i < nroots; i++, ++it)
                        roots.push_back(*it);
                }
            }
        }
        पकडा (...)
        {
            // FIXIT log error message
            is_opened = सत्य;
            release();
            फेका;
        }

        // release resources that we do not need anymore
        closeFile();
        is_opened = सत्य;
        std::vector<अक्षर> tmpbuf;
        std::swap(buffer, tmpbuf);
        bufofs = 0;
    }
    परत ok;
}

शून्य FileStorage::Impl::माला_दो(स्थिरांक अक्षर *str) {
    CV_Assert(write_mode);
    जर (mem_mode)
        std::copy(str, str + म_माप(str), std::back_inserter(outbuf));
    अन्यथा जर (file)
        ख_माला_दो(str, file);
#जर USE_ZLIB
    अन्यथा जर (gzfile)
        gzputs(gzfile, str);
#endif
    अन्यथा
        CV_Error(cv::Error::StsError, "The storage is not opened");
}

अक्षर *FileStorage::Impl::getsFromFile(अक्षर *buf, पूर्णांक मोजणी_2) {
    जर (file)
        परत ख_माला_लो(buf, मोजणी_2, file);
#जर USE_ZLIB
    जर (gzfile)
        परत gzgets(gzfile, buf, मोजणी_2);
#endif
    CV_Error(cv::Error::StsError, "The storage is not opened");
}

अक्षर *FileStorage::Impl::माला_लो(माप_प्रकार max_मोजणी) {
    जर (strbuf) {
        माप_प्रकार i = strbufpos, len = strbufsize;
        स्थिरांक अक्षर *instr = strbuf;
        साठी (; i < len; i++) {
            अक्षर c = instr[i];
            जर (c == '\0' || c == '\n') {
                जर (c == '\n')
                    i++;
                थांबा;
            }
        }
        माप_प्रकार मोजणी_2 = i - strbufpos;
        जर (max_मोजणी == 0 || max_मोजणी > मोजणी_2)
            max_मोजणी = मोजणी_2;
        buffer.resize(std::max(buffer.size(), max_मोजणी + 8));
        स_नकल(&buffer[0], instr + strbufpos, max_मोजणी);
        buffer[max_मोजणी] = '\0';
        strbufpos = i;
        परत max_मोजणी > 0 ? &buffer[0] : 0;
    }

    स्थिरांक माप_प्रकार MAX_BLOCK_SIZE = पूर्णांक_उच्च / 2; // hopefully, that will be enough
    जर (max_मोजणी == 0)
        max_मोजणी = MAX_BLOCK_SIZE;
    अन्यथा
        CV_Assert(max_मोजणी < MAX_BLOCK_SIZE);
    माप_प्रकार ofs = 0;

    साठी (;;) {
        पूर्णांक मोजणी_2 = (पूर्णांक) std::min(buffer.size() - ofs - 16, max_मोजणी);
        अक्षर *ptr = getsFromFile(&buffer[ofs], मोजणी_2 + 1);
        जर (!ptr)
            थांबा;
        पूर्णांक delta = (पूर्णांक) म_माप(ptr);
        ofs += delta;
        max_मोजणी -= delta;
        जर (delta == 0 || ptr[delta - 1] == '\n' || max_मोजणी == 0)
            थांबा;
        जर (delta == मोजणी_2)
            buffer.resize((माप_प्रकार) (buffer.size() * 1.5));
    }
    परत ofs > 0 ? &buffer[0] : 0;
}

अक्षर *FileStorage::Impl::माला_लो() {
    अक्षर *ptr = हे->माला_लो(0);
    जर (!ptr) {
        ptr = bufferStart();  // FIXIT Why do we need this hack? What is about other parsers JSON/YAML?
        *ptr = '\0';
        setEof();
        परत 0;
    } अन्यथा {
        माप_प्रकार l = म_माप(ptr);
        जर (l > 0 && ptr[l - 1] != '\n' && ptr[l - 1] != '\r' && !eof()) {
            ptr[l] = '\n';
            ptr[l + 1] = '\0';
        }
    }
    lineno++;
    परत ptr;
}

तार्किक FileStorage::Impl::eof() {
    जर (dummy_eof)
        परत सत्य;
    जर (strbuf)
        परत strbufpos >= strbufsize;
    जर (file)
        परत ख_पूर्ण(file) != 0;
#जर USE_ZLIB
    जर (gzfile)
        परत gzeof(gzfile) != 0;
#endif
    परत असत्य;
}

शून्य FileStorage::Impl::setEof() {
    dummy_eof = सत्य;
}

शून्य FileStorage::Impl::closeFile() {
    जर (file)
        ख_बंद(file);
#जर USE_ZLIB
    अन्यथा जर (gzfile)
        gzclose(gzfile);
#endif
    file = 0;
    gzfile = 0;
    strbuf = 0;
    strbufpos = 0;
    is_opened = असत्य;
}

शून्य FileStorage::Impl::शुरुआत() {
    जर (file)
        ::शुरुआत(file);
#जर USE_ZLIB
    अन्यथा जर (gzfile)
        gzrewind(gzfile);
#endif
    strbufpos = 0;
}

अक्षर *FileStorage::Impl::resizeWriteBuffer(अक्षर *ptr, पूर्णांक len) {
    स्थिरांक अक्षर *buffer_end = &buffer[0] + buffer.size();
    जर (ptr + len < buffer_end)
        परत ptr;

    स्थिरांक अक्षर *buffer_start = &buffer[0];
    पूर्णांक written_len = (पूर्णांक) (ptr - buffer_start);

    CV_Assert(written_len <= (पूर्णांक) buffer.size());
    पूर्णांक new_size = (पूर्णांक) ((buffer_end - buffer_start) * 3 / 2);
    new_size = MAX(written_len + len, new_size);
    buffer.reserve(new_size + 256);
    buffer.resize(new_size);
    bufofs = written_len;
    परत &buffer[0] + bufofs;
}

अक्षर *FileStorage::Impl::flush() {
    अक्षर *buffer_start = &buffer[0];
    अक्षर *ptr = buffer_start + bufofs;

    जर (ptr > buffer_start + space) {
        ptr[0] = '\n';
        ptr[1] = '\0';
        माला_दो(buffer_start);
        bufofs = 0;
    }

    पूर्णांक indent = write_चळत.back().indent;

    जर (space != indent) {
        स_रखो(buffer_start, ' ', indent);
        space = indent;
    }
    bufofs = space;
    ptr = buffer_start + bufofs;

    परत ptr;
}

शून्य FileStorage::Impl::endWriteStruct() {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(असत्य);
    जर (state_of_writing_base64 != FileStorage_API::Uncertain)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    CV_Assert(!write_चळत.empty());

    FStructData &current_struct = write_चळत.back();
    जर (fmt == FileStorage::FORMAT_JSON && !File_गाठ::isFlow(current_struct.flags) && write_चळत.size() > 1)
        current_struct.indent = write_चळत[write_चळत.size() - 2].indent;

    getEmitter().endWriteStruct(current_struct);

    write_चळत.pop_back();
    जर (!write_चळत.empty())
        write_चळत.back().flags &= ~File_गाठ::EMPTY;
}

शून्य FileStorage::Impl::startWriteStruct_helper(स्थिरांक अक्षर *किल्ली, पूर्णांक struct_flags,
                                                स्थिरांक अक्षर *type_name) {
    CV_Assert(write_mode);

    struct_flags = (struct_flags & (File_गाठ::TYPE_MASK | File_गाठ::FLOW)) | File_गाठ::EMPTY;
    जर (!File_गाठ::isCollection(struct_flags))
        CV_Error(cv::Error::StsBadArg,
                 "Some collection type: FileNode::SEQ or FileNode::MAP must be specified");

    जर (type_name && type_name[0] == '\0')
        type_name = 0;

    FStructData s = getEmitter().startWriteStruct(write_चळत.back(), किल्ली, struct_flags, type_name);

    write_चळत.push_back(s);
    माप_प्रकार write_चळत_size = write_चळत.size();
    जर (write_चळत_size > 1)
        write_चळत[write_चळत_size - 2].flags &= ~File_गाठ::EMPTY;

    जर (fmt != FileStorage::FORMAT_JSON && !File_गाठ::isFlow(s.flags))
        flush();

    जर (fmt == FileStorage::FORMAT_JSON && type_name && type_name[0] && File_गाठ::isMap(struct_flags)) {
        getEmitter().लिख("type_id", type_name, असत्य);
    }
}

शून्य FileStorage::Impl::startWriteStruct(स्थिरांक अक्षर *किल्ली, पूर्णांक struct_flags,
                                         स्थिरांक अक्षर *type_name) {
    check_if_write_struct_is_delayed(असत्य);
    जर (state_of_writing_base64 == FileStorage_API::NotUse)
        switch_to_Base64_state(FileStorage_API::Uncertain);

    जर (state_of_writing_base64 == FileStorage_API::Uncertain && File_गाठ::isSeq(struct_flags)
        && is_using_base64 && type_name == 0) {
        /* Uncertain whether output Base64 data */
        make_write_struct_delayed(किल्ली, struct_flags, type_name);
    } अन्यथा जर (type_name && स_भेद(type_name, "binary", 6) == 0) {
        /* Must output Base64 data */
        जर ((File_गाठ::TYPE_MASK & struct_flags) != File_गाठ::SEQ)
            CV_Error(cv::Error::StsBadArg, "must set 'struct_flags |= CV_NODE_SEQ' if using Base64.");
        अन्यथा जर (state_of_writing_base64 != FileStorage_API::Uncertain)
            CV_Error(cv::Error::StsError, "function \'cvStartWriteStruct\' calls cannot be nested if using Base64.");

        startWriteStruct_helper(किल्ली, struct_flags, "binary");

        जर (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::InUse);
    } अन्यथा {
        /* Won't output Base64 data */
        जर (state_of_writing_base64 == FileStorage_API::InUse)
            CV_Error(cv::Error::StsError, "At the end of the output Base64, `cvEndWriteStruct` is needed.");

        startWriteStruct_helper(किल्ली, struct_flags, type_name);

        जर (state_of_writing_base64 != FileStorage_API::Uncertain)
            switch_to_Base64_state(FileStorage_API::Uncertain);
        switch_to_Base64_state(FileStorage_API::NotUse);
    }
}

शून्य FileStorage::Impl::writeComment(स्थिरांक अक्षर *comment, तार्किक eol_comment) {
    CV_Assert(write_mode);
    getEmitter().writeComment(comment, eol_comment);
}

शून्य FileStorage::Impl::startNextStream() {
    CV_Assert(write_mode);
    जर (!empty_stream) {
        पर्यंत (!write_चळत.empty())
            endWriteStruct();
        flush();
        getEmitter().startNextStream();
        empty_stream = सत्य;
        write_चळत.push_back(FStructData("", File_गाठ::EMPTY, 0));
        bufofs = 0;
    }
}

शून्य FileStorage::Impl::लिख(स्थिरांक String &किल्ली, पूर्णांक value) {
    CV_Assert(write_mode);
    getEmitter().लिख(किल्ली.c_str(), value);
}

शून्य FileStorage::Impl::लिख(स्थिरांक String &किल्ली, int64_t value) {
    CV_Assert(write_mode);
    getEmitter().लिख(किल्ली.c_str(), value);
}

शून्य FileStorage::Impl::लिख(स्थिरांक String &किल्ली, द्विगुण value) {
    CV_Assert(write_mode);
    getEmitter().लिख(किल्ली.c_str(), value);
}

शून्य FileStorage::Impl::लिख(स्थिरांक String &किल्ली, स्थिरांक String &value) {
    CV_Assert(write_mode);
    getEmitter().लिख(किल्ली.c_str(), value.c_str(), असत्य);
}

शून्य FileStorage::Impl::writeRawData(स्थिरांक std::string &dt, स्थिरांक शून्य *_data, माप_प्रकार len) {
    CV_Assert(write_mode);

    जर (is_using_base64 || state_of_writing_base64 == FileStorage_API::Base64State::InUse) {
        writeRawDataBase64(_data, len, dt.c_str());
        परत;
    } अन्यथा जर (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::NotUse);
    }

    माप_प्रकार elemSize = fs::calcStructSize(dt.c_str(), 0);
    CV_Assert(elemSize);
    CV_Assert(len % elemSize == 0);
    len /= elemSize;

    तार्किक explicitZero = fmt == FileStorage::FORMAT_JSON;
    स्थिरांक uchar *data0 = (स्थिरांक uchar *) _data;
    पूर्णांक fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2], k, fmt_pair_मोजणी;
    अक्षर buf[256] = "";

    fmt_pair_मोजणी = fs::decodeFormat(dt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS);

    जर (!len)
        परत;

    जर (!data0)
        CV_Error(cv::Error::StsNullPtr, "Null data pointer");

    जर (fmt_pair_मोजणी == 1) {
        fmt_pairs[0] *= (पूर्णांक) len;
        len = 1;
    }

    साठी (; len--; data0 += elemSize) {
        पूर्णांक offset = 0;
        साठी (k = 0; k < fmt_pair_मोजणी; k++) {
            पूर्णांक i, मोजणी_2 = fmt_pairs[k * 2];
            पूर्णांक elem_type = fmt_pairs[k * 2 + 1];
            पूर्णांक elem_size = CV_ELEM_SIZE(elem_type);
            स्थिरांक अक्षर *ptr;

            offset = cvAlign(offset, elem_size);
            स्थिरांक uchar *data = data0 + offset;

            साठी (i = 0; i < मोजणी_2; i++) {
                निवड (elem_type) {
                    स्थिती CV_8U:
                        ptr = fs::itoa(*(uchar *) data, buf, 10);
                        data++;
                        थांबा;
                    स्थिती CV_Bool:
                        ptr = fs::itoa(*(uchar *) data != 0, buf, 10);
                        data++;
                        थांबा;
                    स्थिती CV_8S:
                        ptr = fs::itoa(*(अक्षर *) data, buf, 10);
                        data++;
                        थांबा;
                    स्थिती CV_16U:
                        ptr = fs::itoa(*(ushort *) data, buf, 10);
                        data += आकार(ushort);
                        थांबा;
                    स्थिती CV_16S:
                        ptr = fs::itoa(*(लघु *) data, buf, 10);
                        data += आकार(लघु);
                        थांबा;
                    स्थिती CV_32U:
                        ptr = fs::itoa((int64_t)*(अचिन्हित*) data, buf, 10, असत्य);
                        data += आकार(अचिन्हित);
                        थांबा;
                    स्थिती CV_32S:
                        ptr = fs::itoa(*(पूर्णांक *) data, buf, 10);
                        data += आकार(पूर्णांक);
                        थांबा;
                    स्थिती CV_64U:
                        ptr = fs::itoa(*(uint64_t*) data, buf, 10, असत्य);
                        data += आकार(uint64_t);
                        थांबा;
                    स्थिती CV_64S:
                        ptr = fs::itoa(*(int64_t*) data, buf, 10, सत्य);
                        data += आकार(int64_t);
                        थांबा;
                    स्थिती CV_32F:
                        ptr = fs::floatToString(buf, आकार(buf), *(दशांश *) data, असत्य, explicitZero);
                        data += आकार(दशांश);
                        थांबा;
                    स्थिती CV_64F:
                        ptr = fs::doubleToString(buf, आकार(buf), *(द्विगुण *) data, explicitZero);
                        data += आकार(द्विगुण);
                        थांबा;
                    स्थिती CV_16F:
                        ptr = fs::floatToString(buf, आकार(buf), (दशांश) *(hfloat *) data, सत्य, explicitZero);
                        data += आकार(hfloat);
                        थांबा;
                    स्थिती CV_16BF:
                        ptr = fs::floatToString(buf, आकार(buf), (दशांश) *(bfloat *) data, सत्य, explicitZero);
                        data += आकार(bfloat);
                        थांबा;
                    स्थिती CV_8F_E4M3FN:
                        ptr = fs::floatToString(buf, आकार(buf), (दशांश) *(fp8_t *) data, सत्य, explicitZero);
                        data += आकार(fp8_t);
                        थांबा;
                    स्थिती CV_8F_E4M3FNUZ:
                        ptr = fs::floatToString(buf, आकार(buf), (दशांश) *(fp8a_t *) data, सत्य, explicitZero);
                        data += आकार(fp8a_t);
                        थांबा;
                    पूर्वनिर्धारित:
                        CV_Error(cv::Error::StsUnsupportedFormat, "Unsupported type");
                        परत;
                }

                getEmitter().writeScalar(0, ptr);
            }

            offset = (पूर्णांक) (data - data0);
        }
    }
}

शून्य FileStorage::Impl::workaround() {
    check_if_write_struct_is_delayed(असत्य);

    जर (state_of_writing_base64 != FileStorage_API::Base64State::Uncertain)
        switch_to_Base64_state(FileStorage_API::Base64State::Uncertain);
}

शून्य FileStorage::Impl::switch_to_Base64_state(FileStorage_API::Base64State new_state) {
    स्थिरांक अक्षर *err_unkonwn_state = "Unexpected error, unable to determine the Base64 state.";
    स्थिरांक अक्षर *err_unable_to_switch = "Unexpected error, unable to switch to this state.";

    /* like a finite state machine */
    निवड (state_of_writing_base64) {
        स्थिती FileStorage_API::Base64State::Uncertain:
            निवड (new_state) {
                स्थिती FileStorage_API::Base64State::InUse:
                {
                    CV_DbgAssert(base64_writer == 0);
                    तार्किक can_indent = (fmt != cv::FileStorage::Mode::FORMAT_JSON);
                    base64_writer = नवीन base64::Base64Writer(*हे, can_indent);
                    जर (!can_indent) {
                        अक्षर *ptr = bufferPtr();
                        *ptr++ = '\0';
                        माला_दो(bufferStart());
                        setBufferPtr(bufferStart());
                        स_रखो(bufferStart(), 0, static_cast<पूर्णांक>(space));
                        माला_दो("\"$base64$");
                    }
                    थांबा;
                }
                स्थिती FileStorage_API::Base64State::Uncertain:
                    थांबा;
                स्थिती FileStorage_API::Base64State::NotUse:
                    थांबा;
                पूर्वनिर्धारित:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    थांबा;
            }
            थांबा;
        स्थिती FileStorage_API::Base64State::InUse:
            निवड (new_state) {
                स्थिती FileStorage_API::Base64State::InUse:
                स्थिती FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    थांबा;
                स्थिती FileStorage_API::Base64State::Uncertain:
                    delete base64_writer;
                    base64_writer = 0;
                    जर ( fmt == cv::FileStorage::FORMAT_JSON )
                    {
                        माला_दो("\"");
                        setBufferPtr(bufferStart());
                        flush();
                        स_रखो(bufferStart(), 0, static_cast<पूर्णांक>(space) );
                        setBufferPtr(bufferStart());
                    }
                    थांबा;
                पूर्वनिर्धारित:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    थांबा;
            }
            थांबा;
        स्थिती FileStorage_API::Base64State::NotUse:
            निवड (new_state) {
                स्थिती FileStorage_API::Base64State::InUse:
                स्थिती FileStorage_API::Base64State::NotUse:
                    CV_Error(cv::Error::StsError, err_unable_to_switch);
                    थांबा;
                स्थिती FileStorage_API::Base64State::Uncertain:
                    थांबा;
                पूर्वनिर्धारित:
                    CV_Error(cv::Error::StsError, err_unkonwn_state);
                    थांबा;
            }
            थांबा;
        पूर्वनिर्धारित:
            CV_Error(cv::Error::StsError, err_unkonwn_state);
            थांबा;
    }

    state_of_writing_base64 = new_state;
}

शून्य FileStorage::Impl::make_write_struct_delayed(स्थिरांक अक्षर *किल्ली, पूर्णांक struct_flags, स्थिरांक अक्षर *type_name) {
    CV_Assert(is_write_struct_delayed == असत्य);
    CV_DbgAssert(delayed_struct_किल्ली == nullptr);
    CV_DbgAssert(delayed_struct_flags == 0);
    CV_DbgAssert(delayed_type_name == nullptr);

    delayed_struct_flags = struct_flags;

    जर (किल्ली != nullptr) {
        delayed_struct_किल्ली = नवीन अक्षर[म_माप(किल्ली) + 1U];
        म_नकल(delayed_struct_किल्ली, किल्ली);
    }

    जर (type_name != nullptr) {
        delayed_type_name = नवीन अक्षर[म_माप(type_name) + 1U];
        म_नकल(delayed_type_name, type_name);
    }

    is_write_struct_delayed = सत्य;
}

शून्य FileStorage::Impl::check_if_write_struct_is_delayed(तार्किक change_type_to_base64) {
    जर (is_write_struct_delayed) {
        /* save data to prevent recursive call errors */
        std::string struct_किल्ली;
        std::string type_name;
        पूर्णांक struct_flags = delayed_struct_flags;

        जर (delayed_struct_किल्ली != nullptr && *delayed_struct_किल्ली != '\0') {
            struct_किल्ली.assign(delayed_struct_किल्ली);
        }
        जर (delayed_type_name != nullptr && *delayed_type_name != '\0') {
            type_name.assign(delayed_type_name);
        }

        /* reset */
        delete[] delayed_struct_किल्ली;
        delete[] delayed_type_name;
        delayed_struct_किल्ली = nullptr;
        delayed_struct_flags = 0;
        delayed_type_name = nullptr;

        is_write_struct_delayed = असत्य;

        /* call */
        जर (change_type_to_base64) {
            startWriteStruct_helper(struct_किल्ली.c_str(), struct_flags, "binary");
            जर (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::InUse);
        } अन्यथा {
            startWriteStruct_helper(struct_किल्ली.c_str(), struct_flags, type_name.c_str());
            जर (state_of_writing_base64 != FileStorage_API::Uncertain)
                switch_to_Base64_state(FileStorage_API::Uncertain);
            switch_to_Base64_state(FileStorage_API::NotUse);
        }
    }
}

शून्य FileStorage::Impl::writeRawDataBase64(स्थिरांक शून्य *_data, माप_प्रकार len, स्थिरांक अक्षर *dt) {
    CV_Assert(write_mode);

    check_if_write_struct_is_delayed(सत्य);

    जर (state_of_writing_base64 == FileStorage_API::Base64State::Uncertain) {
        switch_to_Base64_state(FileStorage_API::Base64State::InUse);
    } अन्यथा जर (state_of_writing_base64 != FileStorage_API::Base64State::InUse) {
        CV_Error(cv::Error::StsError, "Base64 should not be used at present.");
    }

    base64_writer->लिख(_data, len, dt);
}

File_गाठ FileStorage::Impl::get_First_शीर्ष_Level_गाठ() स्थिरांक {
    परत roots.empty() ? File_गाठ() : roots[0];
}

File_गाठ FileStorage::Impl::मूळ(पूर्णांक streamIdx) स्थिरांक {
    परत streamIdx >= 0 && streamIdx < (पूर्णांक) roots.size() ? roots[streamIdx] : File_गाठ();
}

File_गाठ FileStorage::Impl::चालक[](स्थिरांक String &nodename) स्थिरांक {
    परत हे->चालक[](nodename.c_str());
}

File_गाठ FileStorage::Impl::चालक[](स्थिरांक अक्षर * /*nodename*/) स्थिरांक {
    परत File_गाठ();
}

पूर्णांक FileStorage::Impl::getFormat() स्थिरांक { परत fmt; }

अक्षर *FileStorage::Impl::bufferPtr() स्थिरांक { परत (अक्षर *) (&buffer[0] + bufofs); }

अक्षर *FileStorage::Impl::bufferStart() स्थिरांक { परत (अक्षर *) &buffer[0]; }

अक्षर *FileStorage::Impl::bufferEnd() स्थिरांक { परत (अक्षर *) (&buffer[0] + buffer.size()); }

शून्य FileStorage::Impl::setBufferPtr(अक्षर *ptr) {
    अक्षर *bufferstart = bufferStart();
    CV_Assert(ptr >= bufferstart && ptr <= bufferEnd());
    bufofs = ptr - bufferstart;
}

पूर्णांक FileStorage::Impl::wrapMargin() स्थिरांक { परत wrap_margin; }

FStructData &FileStorage::Impl::getCurrentStruct() {
    CV_Assert(!write_चळत.empty());
    परत write_चळत.back();
}

शून्य FileStorage::Impl::setNonEmpty() {
    empty_stream = असत्य;
}

शून्य FileStorage::Impl::processSpecialDouble(अक्षर *buf, द्विगुण *value, अक्षर **endptr) {
    FileStorage_API *fs = हे;
    अक्षर c = buf[0];
    पूर्णांक inf_hi = 0x7ff00000;

    जर (c == '-' || c == '+') {
        inf_hi = c == '-' ? 0xfff00000 : 0x7ff00000;
        c = *++buf;
    }

    जर (c != '.')
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");

    Cv64suf v;
    v.f = 0.;
    जर (बड़े(buf[1]) == 'I' && बड़े(buf[2]) == 'N' && बड़े(buf[3]) == 'F')
        v.u = (uint64) inf_hi << 32;
    अन्यथा जर (बड़े(buf[1]) == 'N' && बड़े(buf[2]) == 'A' && बड़े(buf[3]) == 'N')
        v.u = (uint64) -1;
    अन्यथा
        CV_PARSE_ERROR_CPP("Bad format of floating-point constant");
    *value = v.f;
    *endptr = buf + 4;
}

द्विगुण FileStorage::Impl::म_से_भग्न(अक्षर *ptr, अक्षर **endptr) {
    द्विगुण fval = ::म_से_भग्न(ptr, endptr);
    जर (**endptr == '.') {
        अक्षर *dot_pos = *endptr;
        *dot_pos = ',';
        द्विगुण fval2 = ::म_से_भग्न(ptr, endptr);
        *dot_pos = '.';
        जर (*endptr > dot_pos)
            fval = fval2;
        अन्यथा
            *endptr = dot_pos;
    }

    जर (*endptr == ptr || cv_isalpha(**endptr))
        processSpecialDouble(ptr, &fval, endptr);

    परत fval;
}

शून्य FileStorage::Impl::convertToCollection(पूर्णांक type, File_गाठ &गाठ) {
    CV_Assert(type == File_गाठ::SEQ || type == File_गाठ::MAP);

    पूर्णांक गाठ_type = गाठ.type();
    जर (गाठ_type == type)
        परत;

    तार्किक named = गाठ.isNamed();
    uchar *ptr = गाठ.ptr() + 1 + (named ? 4 : 0);

    int64_t ival = 0;
    द्विगुण fval = 0;
    std::string sval;
    तार्किक add_first_scalar = असत्य;

    जर (गाठ_type != File_गाठ::NONE) {
        // scalar nodes can only be converted to sequences, e.g. in XML:
        // <a>5[parser_position]... => create 5 with name "a"
        // <a>5 6[parser_position]... => 5 is converted to [5] and then 6 is added to it
        //
        // otherwise we don't know where to get the element names from
        CV_Assert(type == File_गाठ::SEQ);
        जर (गाठ_type == File_गाठ::INT) {
            ival = readLong(ptr);
            add_first_scalar = सत्य;
        } अन्यथा जर (गाठ_type == File_गाठ::REAL) {
            fval = readReal(ptr);
            add_first_scalar = सत्य;
        } अन्यथा जर (गाठ_type == File_गाठ::STRING) {
            sval = std::string(गाठ);
            add_first_scalar = सत्य;
        } अन्यथा
            CV_Error_(Error::StsError, ("The node of type %d cannot be converted to collection", गाठ_type));
    }

    ptr = reserve_गाठ_Space(गाठ, 1 + (named ? 4 : 0) + 4 + 4);
    *ptr++ = (uchar) (type | (named ? File_गाठ::NAMED : 0));
    // name has been copied automatically
    जर (named)
        ptr += 4;
    // set raw_size(collection)==4, nelems(collection)==1
    writeInt(ptr, 4);
    writeInt(ptr + 4, 0);

    जर (add_first_scalar)
        add_गाठ(गाठ, std::string(), गाठ_type,
                गाठ_type == File_गाठ::INT ? (स्थिरांक शून्य *) &ival :
                गाठ_type == File_गाठ::REAL ? (स्थिरांक शून्य *) &fval :
                गाठ_type == File_गाठ::STRING ? (स्थिरांक शून्य *) sval.c_str() : 0,
                -1);
}

// a) allocates new FileNode (for that just set blockIdx to the last block and ofs to freeSpaceOfs) or
// b) reallocates just created new node (blockIdx and ofs must be taken from FileNode).
//    If there is no enough space in the current block (it should be the last block added so far),
//    the last block is shrunk so that it ends immediately before the reallocated node. Then,
//    a new block of sufficient size is allocated and the FileNode is placed in the beginning of it.
// The case (a) can be used to allocate the very first node by setting blockIdx == ofs == 0.
// In the case (b) the existing tag and the name are copied automatically.
uchar *FileStorage::Impl::reserve_गाठ_Space(File_गाठ &गाठ, माप_प्रकार sz) {
    तार्किक shrinkBlock = असत्य;
    माप_प्रकार shrinkBlockIdx = 0, shrinkSize = 0;

    uchar *ptr = 0, *blockEnd = 0;

    जर (!fs_data_ptrs.empty()) {
        माप_प्रकार blockIdx = गाठ.blockIdx;
        माप_प्रकार ofs = गाठ.ofs;
        CV_Assert(blockIdx == fs_data_ptrs.size() - 1);
        CV_Assert(ofs <= fs_data_blksz[blockIdx]);
        CV_Assert(freeSpaceOfs <= fs_data_blksz[blockIdx]);
        //CV_Assert( freeSpaceOfs <= ofs + sz );

        ptr = fs_data_ptrs[blockIdx] + ofs;
        blockEnd = fs_data_ptrs[blockIdx] + fs_data_blksz[blockIdx];

        CV_Assert(ptr >= fs_data_ptrs[blockIdx] && ptr <= blockEnd);
        जर (ptr + sz <= blockEnd) {
            freeSpaceOfs = ofs + sz;
            परत ptr;
        }

        जर (ofs ==
            0)  // FileNode is a first component of this block. Resize current block instead of allocation of new one.
        {
            fs_data[blockIdx]->resize(sz);
            ptr = &fs_data[blockIdx]->at(0);
            fs_data_ptrs[blockIdx] = ptr;
            fs_data_blksz[blockIdx] = sz;
            freeSpaceOfs = sz;
            परत ptr;
        }

        shrinkBlock = सत्य;
        shrinkBlockIdx = blockIdx;
        shrinkSize = ofs;
    }

    माप_प्रकार blockSize = std::max((माप_प्रकार) CV_FS_MAX_LEN * 4 - 256, sz) + 256;
    Ptr<std::vector<uchar> > pv = makePtr<std::vector<uchar> >(blockSize);
    fs_data.push_back(pv);
    uchar *new_ptr = &pv->at(0);
    fs_data_ptrs.push_back(new_ptr);
    fs_data_blksz.push_back(blockSize);
    गाठ.blockIdx = fs_data_ptrs.size() - 1;
    गाठ.ofs = 0;
    freeSpaceOfs = sz;

    जर (ptr && ptr + 5 <= blockEnd) {
        new_ptr[0] = ptr[0];
        जर (ptr[0] & File_गाठ::NAMED) {
            new_ptr[1] = ptr[1];
            new_ptr[2] = ptr[2];
            new_ptr[3] = ptr[3];
            new_ptr[4] = ptr[4];
        }
    }

    जर (shrinkBlock) {
        fs_data[shrinkBlockIdx]->resize(shrinkSize);
        fs_data_blksz[shrinkBlockIdx] = shrinkSize;
    }

    परत new_ptr;
}

अचिन्हित FileStorage::Impl::getStringOfs(स्थिरांक std::string &किल्ली) स्थिरांक {
    str_hash_t::const_iterator it = str_hash.find(किल्ली);
    परत it != str_hash.end() ? it->second : 0;
}

File_गाठ FileStorage::Impl::add_गाठ(File_गाठ &collection, स्थिरांक std::string &किल्ली,
                                    पूर्णांक elem_type, स्थिरांक शून्य *value, पूर्णांक len) {
    FileStorage_API *fs = हे;
    तार्किक noname = किल्ली.empty() || (fmt == FileStorage::FORMAT_XML && म_भेद(किल्ली.c_str(), "_") == 0);
    convertToCollection(noname ? File_गाठ::SEQ : File_गाठ::MAP, collection);

    तार्किक isseq = collection.empty() ? असत्य : collection.isSeq();
    जर (noname != isseq)
        CV_PARSE_ERROR_CPP(noname ? "Map element should have a name" :
                           "Sequence element should not have name (use <_></_>)");
    अचिन्हित strofs = 0;
    जर (!noname) {
        strofs = getStringOfs(किल्ली);
        जर (!strofs) {
            strofs = (अचिन्हित) str_hash_data.size();
            माप_प्रकार keysize = किल्ली.size() + 1;
            str_hash_data.resize(strofs + keysize);
            स_नकल(&str_hash_data[0] + strofs, &किल्ली[0], keysize);
            str_hash.insert(std::make_pair(किल्ली, strofs));
        }
    }

    uchar *cp = collection.ptr();

    माप_प्रकार blockIdx = fs_data_ptrs.size() - 1;
    माप_प्रकार ofs = freeSpaceOfs;
    File_गाठ गाठ(fs_ext, blockIdx, ofs);

    माप_प्रकार sz0 = 1 + (noname ? 0 : 4) + 8;
    uchar *ptr = reserve_गाठ_Space(गाठ, sz0);

    *ptr++ = (uchar) (elem_type | (noname ? 0 : File_गाठ::NAMED));
    जर (elem_type == File_गाठ::NONE)
        freeSpaceOfs -= 8;

    जर (!noname) {
        writeInt(ptr, (पूर्णांक) strofs);
        ptr += 4;
    }

    जर (elem_type == File_गाठ::SEQ || elem_type == File_गाठ::MAP) {
        writeInt(ptr, 4);
        writeInt(ptr, 0);
    }

    जर (value)
        गाठ.set_मूल्य(elem_type, value, len);

    जर (collection.isNamed())
        cp += 4;
    पूर्णांक nelems = readInt(cp + 5);
    writeInt(cp + 5, nelems + 1);

    परत गाठ;
}

शून्य FileStorage::Impl::finalizeCollection(File_गाठ &collection) {
    जर (!collection.isSeq() && !collection.isMap())
        परत;
    uchar *ptr0 = collection.ptr(), *ptr = ptr0 + 1;
    जर (*ptr0 & File_गाठ::NAMED)
        ptr += 4;
    माप_प्रकार blockIdx = collection.blockIdx;
    माप_प्रकार ofs = collection.ofs + (माप_प्रकार) (ptr + 8 - ptr0);
    माप_प्रकार rawSize = 4;
    अचिन्हित sz = (अचिन्हित) readInt(ptr + 4);
    जर (sz > 0) {
        माप_प्रकार lastBlockIdx = fs_data_ptrs.size() - 1;

        साठी (; blockIdx < lastBlockIdx; blockIdx++) {
            rawSize += fs_data_blksz[blockIdx] - ofs;
            ofs = 0;
        }
    }
    rawSize += freeSpaceOfs - ofs;
    writeInt(ptr, (पूर्णांक) rawSize);
}

शून्य FileStorage::Impl::normalize_गाठ_Ofs(माप_प्रकार &blockIdx, माप_प्रकार &ofs) स्थिरांक {
    पर्यंत (ofs >= fs_data_blksz[blockIdx]) {
        जर (blockIdx == fs_data_blksz.size() - 1) {
            CV_Assert(ofs == fs_data_blksz[blockIdx]);
            थांबा;
        }
        ofs -= fs_data_blksz[blockIdx];
        blockIdx++;
    }
}

FileStorage::Impl::Base64State FileStorage::Impl::get_state_of_writing_base64() {
    परत state_of_writing_base64;
}

पूर्णांक FileStorage::Impl::get_space() {
    परत space;
}


FileStorage::Impl::Base64Decoder::Base64Decoder() {
    ofs = 0;
    ptr = 0;
    indent = 0;
    totalchars = 0;
    eos = सत्य;
}

शून्य FileStorage::Impl::Base64Decoder::init(स्थिरांक Ptr<FileStorageParser> &_parser, अक्षर *_ptr, पूर्णांक _indent) {
    parser_do_not_use_direct_dereference = _parser;
    ptr = _ptr;
    indent = _indent;
    encoded.clear();
    decoded.clear();
    ofs = 0;
    totalchars = 0;
    eos = असत्य;
}

तार्किक FileStorage::Impl::Base64Decoder::readMore(पूर्णांक needed) {
    स्थिर स्थिरांक uchar base64tab[] =
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

    जर (eos)
        परत असत्य;

    माप_प्रकार sz = decoded.size();
    CV_Assert(ofs <= sz);
    sz -= ofs;
    साठी (माप_प्रकार i = 0; i < sz; i++)
        decoded[i] = decoded[ofs + i];

    decoded.resize(sz);
    ofs = 0;

    CV_Assert(ptr);
    अक्षर *beg = 0, *end = 0;
    तार्किक ok = getParser().getBase64Row(ptr, indent, beg, end);
    ptr = end;
    std::copy(beg, end, std::back_inserter(encoded));
    totalchars += end - beg;

    जर (!ok || beg == end) {
        // in the end of base64 sequence pad it with '=' characters so that
        // its total length is multiple of
        eos = सत्य;
        माप_प्रकार tc = totalchars;
        साठी (; tc % 4 != 0; tc++)
            encoded.push_back('=');
    }

    पूर्णांक i = 0, j, n = (पूर्णांक) encoded.size();
    जर (n > 0) {
        स्थिरांक uchar *tab = base64tab;
        अक्षर *src = &encoded[0];

        साठी (; i <= n - 4; i += 4) {
            // dddddd cccccc bbbbbb aaaaaa => ddddddcc ccccbbbb bbaaaaaa
            uchar d = tab[(पूर्णांक) (uchar) src[i]], c = tab[(पूर्णांक) (uchar) src[i + 1]];
            uchar b = tab[(पूर्णांक) (uchar) src[i + 2]], a = tab[(पूर्णांक) (uchar) src[i + 3]];

            decoded.push_back((uchar) ((d << 2) | (c >> 4)));
            decoded.push_back((uchar) ((c << 4) | (b >> 2)));
            decoded.push_back((uchar) ((b << 6) | a));
        }
    }

    जर (i > 0 && encoded[i - 1] == '=') {
        जर (i > 1 && encoded[i - 2] == '=' && !decoded.empty())
            decoded.pop_back();
        जर (!decoded.empty())
            decoded.pop_back();
    }

    n -= i;
    साठी (j = 0; j < n; j++)
        encoded[j] = encoded[i + j];
    encoded.resize(n);

    परत (पूर्णांक) decoded.size() >= needed;
}

uchar FileStorage::Impl::Base64Decoder::getUInt8() {
    माप_प्रकार sz = decoded.size();
    जर (ofs >= sz && !readMore(1))
        परत (uchar) 0;
    परत decoded[ofs++];
}

ushort FileStorage::Impl::Base64Decoder::getUInt16() {
    माप_प्रकार sz = decoded.size();
    जर (ofs + 2 > sz && !readMore(2))
        परत (ushort) 0;
    ushort val = (decoded[ofs] + (decoded[ofs + 1] << 8));
    ofs += 2;
    परत val;
}

पूर्णांक FileStorage::Impl::Base64Decoder::getInt32() {
    माप_प्रकार sz = decoded.size();
    जर (ofs + 4 > sz && !readMore(4))
        परत 0;
    पूर्णांक ival = readInt(&decoded[ofs]);
    ofs += 4;
    परत ival;
}

int64_t FileStorage::Impl::Base64Decoder::getInt64() {
    माप_प्रकार sz = decoded.size();
    जर (ofs + 8 > sz && !readMore(8))
        परत 0;
    int64_t ival = readLong(&decoded[ofs]);
    ofs += 8;
    परत ival;
}

द्विगुण FileStorage::Impl::Base64Decoder::getFloat64() {
    माप_प्रकार sz = decoded.size();
    जर (ofs + 8 > sz && !readMore(8))
        परत 0;
    द्विगुण fval = readReal(&decoded[ofs]);
    ofs += 8;
    परत fval;
}

तार्किक FileStorage::Impl::Base64Decoder::endOfStream() स्थिरांक { परत eos; }

अक्षर *FileStorage::Impl::Base64Decoder::getPtr() स्थिरांक { परत ptr; }


अक्षर *FileStorage::Impl::parseBase64(अक्षर *ptr, पूर्णांक indent, File_गाठ &collection) {
    स्थिरांक पूर्णांक BASE64_HDR_SIZE = 24;
    अक्षर dt[BASE64_HDR_SIZE + 1] = {0};
    base64decoder.init(parser_do_not_use_direct_dereference, ptr, indent);

    पूर्णांक i, k;

    साठी (i = 0; i < BASE64_HDR_SIZE; i++)
        dt[i] = (अक्षर) base64decoder.getUInt8();
    साठी (i = 0; i < BASE64_HDR_SIZE; i++)
        जर (है_खाली(dt[i]))
            थांबा;
    dt[i] = '\0';

    CV_Assert(!base64decoder.endOfStream());

    पूर्णांक fmt_pairs[CV_FS_MAX_FMT_PAIRS * 2];
    पूर्णांक fmt_pair_मोजणी = fs::decodeFormat(dt, fmt_pairs, CV_FS_MAX_FMT_PAIRS);
    int64_t ival = 0;
    द्विगुण fval = 0;

    साठी (;;) {
        साठी (k = 0; k < fmt_pair_मोजणी; k++) {
            पूर्णांक elem_type = fmt_pairs[k * 2 + 1];
            पूर्णांक मोजणी_2 = fmt_pairs[k * 2];

            साठी (i = 0; i < मोजणी_2; i++) {
                पूर्णांक गाठ_type = File_गाठ::INT;
                निवड (elem_type) {
                    स्थिती CV_8U:
                        ival = base64decoder.getUInt8();
                        थांबा;
                    स्थिती CV_8S:
                        ival = (अक्षर) base64decoder.getUInt8();
                        थांबा;
                    स्थिती CV_Bool:
                        ival = base64decoder.getUInt8() != 0;
                        थांबा;
                    स्थिती CV_16U:
                        ival = base64decoder.getUInt16();
                        थांबा;
                    स्थिती CV_16S:
                        ival = (लघु) base64decoder.getUInt16();
                        थांबा;
                    स्थिती CV_32U:
                        ival = base64decoder.getInt32();
                        थांबा;
                    स्थिती CV_32S:
                        ival = base64decoder.getInt32();
                        थांबा;
                    स्थिती CV_64U:
                        ival = base64decoder.getInt64();
                        थांबा;
                    स्थिती CV_64S:
                        ival = base64decoder.getInt64();
                        थांबा;
                    स्थिती CV_32F: {
                        Cv32suf v;
                        v.i = base64decoder.getInt32();
                        fval = v.f;
                        गाठ_type = File_गाठ::REAL;
                    }
                        थांबा;
                    स्थिती CV_64F:
                        fval = base64decoder.getFloat64();
                        गाठ_type = File_गाठ::REAL;
                        थांबा;
                    स्थिती CV_16F:
                        fval = दशांश(hfloatFromBits(base64decoder.getUInt16()));
                        गाठ_type = File_गाठ::REAL;
                        थांबा;
                    स्थिती CV_8F_E4M3FN:
                        fval = fp8_t::decodeLUT()[base64decoder.getUInt8()];
                        गाठ_type = File_गाठ::REAL;
                        थांबा;
                    स्थिती CV_8F_E4M3FNUZ:
                        fval = fp8a_t::decodeLUT()[base64decoder.getUInt8()];
                        गाठ_type = File_गाठ::REAL;
                        थांबा;
                    पूर्वनिर्धारित:
                        CV_Error(Error::StsUnsupportedFormat, "Unsupported type");
                }

                जर (base64decoder.endOfStream())
                    थांबा;
                add_गाठ(collection, std::string(), गाठ_type,
                        गाठ_type == File_गाठ::INT ? (शून्य *) &ival : (शून्य *) &fval, -1);
            }
        }
        जर (base64decoder.endOfStream())
            थांबा;
    }

    finalizeCollection(collection);
    परत base64decoder.getPtr();
}

शून्य FileStorage::Impl::parseError(स्थिरांक अक्षर *func_name, स्थिरांक std::string &err_संदेश, स्थिरांक अक्षर *source_file,
                                   पूर्णांक source_ओळ) {
    std::string संदेश = format("%s(%d): %s", filename.c_str(), lineno, err_संदेश.c_str());
    error(Error::StsParseError, func_name, संदेश.c_str(), source_file, source_ओळ);
}

स्थिरांक uchar *FileStorage::Impl::get_गाठ_Ptr(माप_प्रकार blockIdx, माप_प्रकार ofs) स्थिरांक {
    CV_Assert(blockIdx < fs_data_ptrs.size());
    CV_Assert(ofs < fs_data_blksz[blockIdx]);

    परत fs_data_ptrs[blockIdx] + ofs;
}

std::string FileStorage::Impl::getName(माप_प्रकार nameofs) स्थिरांक {
    CV_Assert(nameofs < str_hash_data.size());
    परत std::string(&str_hash_data[nameofs]);
}

FileStorage *FileStorage::Impl::getFS() { परत fs_ext; }


FileStorage::FileStorage()
    : state(0)
{
    p = makePtr<FileStorage::Impl>(हे);
}

FileStorage::FileStorage(स्थिरांक String& filename, पूर्णांक flags, स्थिरांक String& encoding)
    : state(0)
{
    p = makePtr<FileStorage::Impl>(हे);
    तार्किक ok = p->खोलो(filename.c_str(), flags, encoding.c_str());
    जर(ok)
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

शून्य FileStorage::startWriteStruct(स्थिरांक String& name, पूर्णांक struct_flags, स्थिरांक String& typeName)
{
    p->startWriteStruct(name.size() ? name.c_str() : 0, struct_flags, typeName.size() ? typeName.c_str() : 0);
    elname = String();
    जर ((struct_flags & File_गाठ::TYPE_MASK) == File_गाठ::SEQ)
        state = FileStorage::मूल्य_EXPECTED;
    अन्यथा
        state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
}

शून्य FileStorage::endWriteStruct()
{
    p->endWriteStruct();
    state = p->write_चळत.empty() || File_गाठ::isMap(p->write_चळत.back().flags) ?
        FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP :
        FileStorage::मूल्य_EXPECTED;
    elname = String();
}

FileStorage::~FileStorage()
{
}

तार्किक FileStorage::खोलो(स्थिरांक String& filename, पूर्णांक flags, स्थिरांक String& encoding)
{
    प्रयत्न
    {
        तार्किक ok = p->खोलो(filename.c_str(), flags, encoding.c_str());
        जर(ok)
            state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        परत ok;
    }
    पकडा (...)
    {
        release();
        फेका;  // re-throw
    }
}

तार्किक FileStorage::isOpened() स्थिरांक { परत p->is_opened; }

शून्य FileStorage::release()
{
    p->release();
}

File_गाठ FileStorage::मूळ(पूर्णांक i) स्थिरांक
{
    जर( p.empty() || p->roots.empty() || i < 0 || i >= (पूर्णांक)p->roots.size() )
        परत File_गाठ();

    परत p->roots[i];
}

File_गाठ FileStorage::get_First_शीर्ष_Level_गाठ() स्थिरांक
{
    File_गाठ r = मूळ();
    File_गाठ_Iterator it = r.begin();
    परत it != r.end() ? *it : File_गाठ();
}

std::string FileStorage::getDefaultObjectName(स्थिरांक std::string& _filename)
{
    स्थिर स्थिरांक अक्षर* stubname = "unnamed";
    स्थिरांक अक्षर* filename = _filename.c_str();
    स्थिरांक अक्षर* ptr2 = filename + _filename.size();
    स्थिरांक अक्षर* ptr = ptr2 - 1;
    cv::AutoBuffer<अक्षर> name_buf(_filename.size()+1);

    पर्यंत( ptr >= filename && *ptr != '\\' && *ptr != '/' && *ptr != ':' )
    {
        जर( *ptr == '.' && (!*ptr2 || म_भेदन(ptr2, ".gz", 3) == 0) )
            ptr2 = ptr;
        ptr--;
    }
    ptr++;
    जर( ptr == ptr2 )
        CV_Error( cv::Error::StsBadArg, "Invalid filename" );

    अक्षर* name = name_buf.data();

    // name must start with letter or '_'
    जर( !cv_isalpha(*ptr) && *ptr!= '_' ){
        *name++ = '_';
    }

    पर्यंत( ptr < ptr2 )
    {
        अक्षर c = *ptr++;
        जर( !cv_isalnum(c) && c != '-' && c != '_' )
            c = '_';
        *name++ = c;
    }
    *name = '\0';
    name = name_buf.data();
    जर( म_भेद( name, "_" ) == 0 )
        म_नकल( name, stubname );
    परत name;
}


पूर्णांक FileStorage::getFormat() स्थिरांक
{
    परत p->fmt;
}

File_गाठ FileStorage::चालक [](स्थिरांक अक्षर* किल्ली) स्थिरांक
{
    परत हे->चालक[](std::string(किल्ली));
}

File_गाठ FileStorage::चालक [](स्थिरांक std::string& किल्ली) स्थिरांक
{
    File_गाठ res;
    साठी (माप_प्रकार i = 0; i < p->roots.size(); i++)
    {
        res = p->roots[i][किल्ली];
        जर (!res.empty())
            थांबा;
    }
    परत res;
}

String FileStorage::releaseAndGetString()
{
    String buf;
    p->release(&buf);
    परत buf;
}

शून्य FileStorage::writeRaw( स्थिरांक String& fmt, स्थिरांक शून्य* vec, माप_प्रकार len )
{
    p->writeRawData(fmt, (स्थिरांक uchar*)vec, len);
}

शून्य FileStorage::writeComment( स्थिरांक String& comment, तार्किक eol_comment )
{
    p->writeComment(comment.c_str(), eol_comment);
}

शून्य writeScalar( FileStorage& fs, पूर्णांक value )
{
    fs.p->लिख(String(), value);
}

शून्य writeScalar( FileStorage& fs, int64_t value )
{
    fs.p->लिख(String(), value);
}

शून्य writeScalar( FileStorage& fs, दशांश value )
{
    fs.p->लिख(String(), (द्विगुण)value);
}

शून्य writeScalar( FileStorage& fs, द्विगुण value )
{
    fs.p->लिख(String(), value);
}

शून्य writeScalar( FileStorage& fs, स्थिरांक String& value )
{
    fs.p->लिख(String(), value);
}

शून्य लिख( FileStorage& fs, स्थिरांक String& name, पूर्णांक value )
{
    fs.p->लिख(name, value);
}

शून्य लिख( FileStorage& fs, स्थिरांक String& name, int64_t value )
{
    fs.p->लिख(name, value);
}

शून्य लिख( FileStorage& fs, स्थिरांक String& name, दशांश value )
{
    fs.p->लिख(name, (द्विगुण)value);
}

शून्य लिख( FileStorage& fs, स्थिरांक String& name, द्विगुण value )
{
    fs.p->लिख(name, value);
}

शून्य लिख( FileStorage& fs, स्थिरांक String& name, स्थिरांक String& value )
{
    fs.p->लिख(name, value);
}

शून्य लिख( FileStorage& fs, स्थिरांक String& name, तार्किक value )
{
    fs.p->लिख(name, value);
}

शून्य FileStorage::लिख(स्थिरांक String& name, पूर्णांक val) { p->लिख(name, val); }
शून्य FileStorage::लिख(स्थिरांक String& name, तार्किक val) { p->लिख(name, val); }
शून्य FileStorage::लिख(स्थिरांक String& name, int64_t val) { p->लिख(name, val); }
शून्य FileStorage::लिख(स्थिरांक String& name, द्विगुण val) { p->लिख(name, val); }
शून्य FileStorage::लिख(स्थिरांक String& name, स्थिरांक String& val) { p->लिख(name, val); }
शून्य FileStorage::लिख(स्थिरांक String& name, स्थिरांक Mat& val) { cv::लिख(*हे, name, val); }
शून्य FileStorage::लिख(स्थिरांक String& name, स्थिरांक std::vector<String>& val) { cv::लिख(*हे, name, val); }

FileStorage& चालक << (FileStorage& fs, स्थिरांक String& str)
{
    गणना { NAME_EXPECTED = FileStorage::NAME_EXPECTED,
        मूल्य_EXPECTED = FileStorage::मूल्य_EXPECTED,
        INSIDE_MAP = FileStorage::INSIDE_MAP };
    स्थिरांक अक्षर* _str = str.c_str();
    जर( !fs.isOpened() || !_str )
        परत fs;
    Ptr<FileStorage::Impl>& fs_impl = fs.p;
    अक्षर c = *_str;

    जर( c == '}' || c == ']' )
    {
        जर( fs_impl->write_चळत.empty() )
            CV_Error_( cv::Error::StsError, ("Extra closing '%c'", *_str) );

        fs_impl->workaround();

        पूर्णांक struct_flags = fs_impl->write_चळत.back().flags;
        अक्षर expected_bracket = File_गाठ::isMap(struct_flags) ? '}' : ']';
        जर( c != expected_bracket )
            CV_Error_( cv::Error::StsError, ("The closing '%c' does not match the opening '%c'", c, expected_bracket));
        fs_impl->endWriteStruct();
        CV_Assert(!fs_impl->write_चळत.empty());
        struct_flags = fs_impl->write_चळत.back().flags;
        fs.state = File_गाठ::isMap(struct_flags) ? INSIDE_MAP + NAME_EXPECTED : मूल्य_EXPECTED;
        fs.elname = String();
    }
    अन्यथा जर( fs.state == NAME_EXPECTED + INSIDE_MAP )
    {
        जर (!cv_isalpha(c) && c != '_')
            CV_Error_( cv::Error::StsError, ("Incorrect element name %s; should start with a letter or '_'", _str) );
        fs.elname = str;
        fs.state = मूल्य_EXPECTED + INSIDE_MAP;
    }
    अन्यथा जर( (fs.state & 3) == मूल्य_EXPECTED )
    {
        जर( c == '{' || c == '[' )
        {
            पूर्णांक struct_flags = c == '{' ? File_गाठ::MAP : File_गाठ::SEQ;
            fs.state = struct_flags == File_गाठ::MAP ? INSIDE_MAP + NAME_EXPECTED : मूल्य_EXPECTED;
            _str++;
            जर( *_str == ':' )
            {
                _str++;
                जर( !*_str )
                    struct_flags |= File_गाठ::FLOW;
            }
            fs_impl->startWriteStruct(!fs.elname.empty() ? fs.elname.c_str() : 0, struct_flags, *_str ? _str : 0 );
            fs.elname = String();
        }
        अन्यथा
        {
            लिख( fs, fs.elname, (c == '\\' && (_str[1] == '{' || _str[1] == '}' ||
                                _str[1] == '[' || _str[1] == ']')) ? String(_str+1) : str );
            जर( fs.state == INSIDE_MAP + मूल्य_EXPECTED )
                fs.state = INSIDE_MAP + NAME_EXPECTED;
        }
    }
    अन्यथा
        CV_Error( cv::Error::StsError, "Invalid fs.state" );
    परत fs;
}


File_गाठ::File_गाठ()
    : fs(रिक्त)
{
    blockIdx = ofs = 0;
}

File_गाठ::File_गाठ(FileStorage::Impl* _fs, माप_प्रकार _blockIdx, माप_प्रकार _ofs)
    : fs(_fs)
{
    blockIdx = _blockIdx;
    ofs = _ofs;
}

File_गाठ::File_गाठ(स्थिरांक FileStorage* _fs, माप_प्रकार _blockIdx, माप_प्रकार _ofs)
    : File_गाठ(_fs->p.get(), _blockIdx, _ofs)
{
    // nothing
}

File_गाठ::File_गाठ(स्थिरांक File_गाठ& गाठ)
{
    fs = गाठ.fs;
    blockIdx = गाठ.blockIdx;
    ofs = गाठ.ofs;
}

File_गाठ& File_गाठ::चालक=(स्थिरांक File_गाठ& गाठ)
{
    fs = गाठ.fs;
    blockIdx = गाठ.blockIdx;
    ofs = गाठ.ofs;
    परत *हे;
}

File_गाठ File_गाठ::चालक[](स्थिरांक std::string& nodename) स्थिरांक
{
    जर(!fs)
        परत File_गाठ();

    CV_Assert( isMap() );

    अचिन्हित किल्ली = fs->getStringOfs(nodename);
    माप_प्रकार i, sz = size();
    File_गाठ_Iterator it = begin();

    साठी( i = 0; i < sz; i++, ++it )
    {
        File_गाठ n = *it;
        स्थिरांक uchar* p = n.ptr();
        अचिन्हित key2 = (अचिन्हित)readInt(p + 1);
        CV_Assert( key2 < fs->str_hash_data.size() );
        जर( किल्ली == key2 )
            परत n;
    }
    परत File_गाठ();
}

File_गाठ File_गाठ::चालक[](स्थिरांक अक्षर* nodename) स्थिरांक
{
    परत हे->चालक[](std::string(nodename));
}

File_गाठ File_गाठ::चालक[](पूर्णांक i) स्थिरांक
{
    जर(!fs)
        परत File_गाठ();

    CV_Assert( isSeq() );

    पूर्णांक sz = (पूर्णांक)size();
    CV_Assert( 0 <= i && i < sz );

    File_गाठ_Iterator it = begin();
    it += i;

    परत *it;
}

std::vector<String> File_गाठ::किल्ल्या() स्थिरांक
{
    CV_Assert(isMap());

    std::vector<String> res;
    res.reserve(size());
    साठी (File_गाठ_Iterator it = begin(); it != end(); ++it)
    {
        res.push_back((*it).name());
    }
    परत res;
}

पूर्णांक File_गाठ::type() स्थिरांक
{
    स्थिरांक uchar* p = ptr();
    जर(!p)
        परत NONE;
    परत (*p & TYPE_MASK);
}

तार्किक File_गाठ::isMap(पूर्णांक flags) { परत (flags & TYPE_MASK) == MAP; }
तार्किक File_गाठ::isSeq(पूर्णांक flags) { परत (flags & TYPE_MASK) == SEQ; }
तार्किक File_गाठ::isCollection(पूर्णांक flags) { परत isMap(flags) || isSeq(flags); }
तार्किक File_गाठ::isFlow(पूर्णांक flags) { परत (flags & FLOW) != 0; }
तार्किक File_गाठ::isEmptyCollection(पूर्णांक flags) { परत (flags & EMPTY) != 0; }

तार्किक File_गाठ::empty() स्थिरांक   { परत fs == 0; }
तार्किक File_गाठ::isNone() स्थिरांक  { परत type() == NONE; }
तार्किक File_गाठ::isSeq() स्थिरांक   { परत type() == SEQ; }
तार्किक File_गाठ::isMap() स्थिरांक   { परत type() == MAP; }
तार्किक File_गाठ::isInt() स्थिरांक   { परत type() == INT;  }
तार्किक File_गाठ::isReal() स्थिरांक  { परत type() == REAL; }
तार्किक File_गाठ::isString() स्थिरांक { परत type() == STRING;  }
तार्किक File_गाठ::isNamed() स्थिरांक
{
    स्थिरांक uchar* p = ptr();
    जर(!p)
        परत असत्य;
    परत (*p & NAMED) != 0;
}

std::string File_गाठ::name() स्थिरांक
{
    स्थिरांक uchar* p = ptr();
    जर(!p)
        परत std::string();
    माप_प्रकार nameofs = p[1] | (p[2]<<8) | (p[3]<<16) | (p[4]<<24);
    परत fs->getName(nameofs);
}

File_गाठ::चालक पूर्णांक() स्थिरांक
{
    स्थिरांक uchar* p = ptr();
    जर(!p)
        परत 0;
    पूर्णांक tag = *p;
    पूर्णांक type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    जर( type == INT )
    {
        परत readInt(p);
    }
    अन्यथा जर( type == REAL )
    {
        परत cvRound(readReal(p));
    }
    अन्यथा
        परत 0x7fffffff;
}

File_गाठ::चालक int64_t() स्थिरांक
{
    स्थिरांक uchar* p = ptr();
    जर(!p)
        परत 0;
    पूर्णांक tag = *p;
    पूर्णांक type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    जर( type == INT )
    {
        परत readLong(p);
    }
    अन्यथा जर( type == REAL )
    {
        परत cvRound(readReal(p));
    }
    अन्यथा
        परत 0x7fffffff;
}

File_गाठ::चालक दशांश() स्थिरांक
{
    स्थिरांक uchar* p = ptr();
    जर(!p)
        परत 0.f;
    पूर्णांक tag = *p;
    पूर्णांक type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    जर( type == INT )
    {
        परत (दशांश)readLong(p);
    }
    अन्यथा जर( type == REAL )
    {
        परत (दशांश)readReal(p);
    }
    अन्यथा
        परत भग्न_उच्च;
}

File_गाठ::चालक द्विगुण() स्थिरांक
{
    स्थिरांक uchar* p = ptr();
    जर(!p)
        परत 0.f;
    पूर्णांक tag = *p;
    पूर्णांक type = (tag & TYPE_MASK);
    p += (tag & NAMED) ? 5 : 1;

    जर( type == INT )
    {
        परत (द्विगुण)readLong(p);
    }
    अन्यथा जर( type == REAL )
    {
        परत readReal(p);
    }
    अन्यथा
        परत द्विग_उच्च;
}

द्विगुण File_गाठ::real() स्थिरांक  { परत द्विगुण(*हे); }

std::string File_गाठ::string() स्थिरांक
{
    स्थिरांक uchar* p = ptr();
    जर( !p || (*p & TYPE_MASK) != STRING )
        परत std::string();
    p += (*p & NAMED) ? 5 : 1;
    माप_प्रकार sz = (माप_प्रकार)(अचिन्हित)readInt(p);
    परत std::string((स्थिरांक अक्षर*)(p + 4), sz - 1);
}
Mat File_गाठ::mat() स्थिरांक { Mat value; पठ(*हे, value, Mat()); परत value; }

File_गाठ_Iterator File_गाठ::begin() स्थिरांक { परत File_गाठ_Iterator(*हे, असत्य); }
File_गाठ_Iterator File_गाठ::end() स्थिरांक   { परत File_गाठ_Iterator(*हे, सत्य); }

शून्य File_गाठ::readRaw( स्थिरांक std::string& fmt, शून्य* vec, माप_प्रकार len ) स्थिरांक
{
    File_गाठ_Iterator it = begin();
    it.readRaw( fmt, vec, len );
}

माप_प्रकार File_गाठ::size() स्थिरांक
{
    स्थिरांक uchar* p = ptr();
    जर( !p )
        परत 0;
    पूर्णांक tag = *p;
    पूर्णांक tp = tag & TYPE_MASK;
    जर( tp == MAP || tp == SEQ )
    {
        जर( tag & NAMED )
            p += 4;
        परत (माप_प्रकार)(अचिन्हित)readInt(p + 5);
    }
    परत tp != NONE;
}

माप_प्रकार File_गाठ::rawSize() स्थिरांक
{
    स्थिरांक uchar* p0 = ptr(), *p = p0;
    जर( !p )
        परत 0;
    पूर्णांक tag = *p++;
    पूर्णांक tp = tag & TYPE_MASK;
    जर( tag & NAMED )
        p += 4;
    माप_प्रकार sz0 = (माप_प्रकार)(p - p0);
    जर( tp == INT )
        परत sz0 + 8;
    जर( tp == REAL )
        परत sz0 + 8;
    जर( tp == NONE )
        परत sz0;
    CV_Assert( tp == STRING || tp == SEQ || tp == MAP );
    परत sz0 + 4 + readInt(p);
}

uchar* File_गाठ::ptr()
{
    परत !fs ? 0 : (uchar*)fs->get_गाठ_Ptr(blockIdx, ofs);
}

स्थिरांक uchar* File_गाठ::ptr() स्थिरांक
{
    परत !fs ? 0 : fs->get_गाठ_Ptr(blockIdx, ofs);
}

शून्य File_गाठ::set_मूल्य( पूर्णांक type, स्थिरांक शून्य* value, पूर्णांक len )
{
    uchar *p = ptr();
    CV_Assert(p != 0);

    पूर्णांक tag = *p;
    पूर्णांक current_type = tag & TYPE_MASK;
    CV_Assert( current_type == NONE || current_type == type );

    पूर्णांक sz = 1;

    जर( tag & NAMED )
        sz += 4;

    जर( type == INT )
        sz += 8;
    अन्यथा जर( type == REAL )
        sz += 8;
    अन्यथा जर( type == STRING )
    {
        जर( len < 0 )
            len = (पूर्णांक)म_माप((स्थिरांक अक्षर*)value);
        sz += 4 + len + 1; // besides the string content,
                           // take the size (4 bytes) and the final '\0' into account
    }
    अन्यथा
        CV_Error(Error::StsNotImplemented, "Only scalar types can be dynamically assigned to a file node");

    p = fs->reserve_गाठ_Space(*हे, sz);
    *p++ = (uchar)(type | (tag & NAMED));
    जर( tag & NAMED )
        p += 4;

    जर( type == INT )
    {
        int64_t ival = *(स्थिरांक int64_t*)value;
        writeInt(p, ival);
    }
    अन्यथा जर( type == REAL )
    {
        द्विगुण dbval = *(स्थिरांक द्विगुण*)value;
        writeReal(p, dbval);
    }
    अन्यथा जर( type == STRING )
    {
        स्थिरांक अक्षर* str = (स्थिरांक अक्षर*)value;
        writeInt(p, len + 1);
        स_नकल(p + 4, str, len);
        p[4 + len] = (uchar)'\0';
    }
}

File_गाठ_Iterator::File_गाठ_Iterator()
{
    fs = 0;
    blockIdx = 0;
    ofs = 0;
    blockSize = 0;
    गाठ_NElems = 0;
    idx = 0;
}

File_गाठ_Iterator::File_गाठ_Iterator( स्थिरांक File_गाठ& गाठ, तार्किक seekEnd )
{
    fs = गाठ.fs;
    idx = 0;
    जर( !fs )
        blockIdx = ofs = blockSize = गाठ_NElems = 0;
    अन्यथा
    {
        blockIdx = गाठ.blockIdx;
        ofs = गाठ.ofs;

        तार्किक collection = गाठ.isSeq() || गाठ.isMap();
        जर( गाठ.isNone() )
        {
            गाठ_NElems = 0;
        }
        अन्यथा जर( !collection )
        {
            गाठ_NElems = 1;
            जर( seekEnd )
            {
                idx = 1;
                ofs += गाठ.rawSize();
            }
        }
        अन्यथा
        {
            गाठ_NElems = गाठ.size();
            स्थिरांक uchar* p0 = गाठ.ptr(), *p = p0 + 1;
            जर(*p0 & File_गाठ::NAMED )
                p += 4;
            जर( !seekEnd )
                ofs += (p - p0) + 8;
            अन्यथा
            {
                माप_प्रकार rawsz = (माप_प्रकार)(अचिन्हित)readInt(p);
                ofs += (p - p0) + 4 + rawsz;
                idx = गाठ_NElems;
            }
        }
        fs->normalize_गाठ_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
}

File_गाठ_Iterator::File_गाठ_Iterator(स्थिरांक File_गाठ_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    गाठ_NElems = it.गाठ_NElems;
    idx = it.idx;
}

File_गाठ_Iterator& File_गाठ_Iterator::चालक=(स्थिरांक File_गाठ_Iterator& it)
{
    fs = it.fs;
    blockIdx = it.blockIdx;
    ofs = it.ofs;
    blockSize = it.blockSize;
    गाठ_NElems = it.गाठ_NElems;
    idx = it.idx;
    परत *हे;
}

File_गाठ File_गाठ_Iterator::चालक *() स्थिरांक
{
    परत File_गाठ(idx < गाठ_NElems ? fs : रिक्त, blockIdx, ofs);
}

File_गाठ_Iterator& File_गाठ_Iterator::चालक ++ ()
{
    जर( idx == गाठ_NElems || !fs )
        परत *हे;
    idx++;
    File_गाठ n(fs, blockIdx, ofs);
    ofs += n.rawSize();
    जर( ofs >= blockSize )
    {
        fs->normalize_गाठ_Ofs(blockIdx, ofs);
        blockSize = fs->fs_data_blksz[blockIdx];
    }
    परत *हे;
}

File_गाठ_Iterator File_गाठ_Iterator::चालक ++ (पूर्णांक)
{
    File_गाठ_Iterator it = *हे;
    ++(*हे);
    परत it;
}

File_गाठ_Iterator& File_गाठ_Iterator::चालक += (पूर्णांक _ofs)
{
    CV_Assert( _ofs >= 0 );
    साठी( ; _ofs > 0; _ofs-- )
        हे->चालक ++();
    परत *हे;
}

File_गाठ_Iterator& File_गाठ_Iterator::readRaw( स्थिरांक String& fmt, शून्य* _data0, माप_प्रकार maxsz)
{
    जर( fs && idx < गाठ_NElems )
    {
        uchar* data0 = (uchar*)_data0;
        पूर्णांक fmt_pairs[CV_FS_MAX_FMT_PAIRS*2];
        पूर्णांक fmt_pair_मोजणी = fs::decodeFormat( fmt.c_str(), fmt_pairs, CV_FS_MAX_FMT_PAIRS );
        माप_प्रकार esz = fs::calcStructSize( fmt.c_str(), 0 );

        CV_Assert( maxsz % esz == 0 );
        maxsz /= esz;

        साठी( ; maxsz > 0; maxsz--, data0 += esz )
        {
            माप_प्रकार offset = 0;
            साठी( पूर्णांक k = 0; k < fmt_pair_मोजणी; k++ )
            {
                पूर्णांक elem_type = fmt_pairs[k*2+1];
                पूर्णांक elem_size = CV_ELEM_SIZE(elem_type);

                पूर्णांक मोजणी_2 = fmt_pairs[k*2];
                offset = alignSize( offset, elem_size );
                uchar* data = data0 + offset;

                साठी( पूर्णांक i = 0; i < मोजणी_2; i++, ++(*हे) )
                {
                    File_गाठ गाठ = *(*हे);
                    जर( गाठ.isInt() )
                    {
                        int64_t ival = static_cast<int64_t>(elem_size == 8 ? (int64_t)गाठ : (पूर्णांक)गाठ);
                        निवड( elem_type )
                        {
                        स्थिती CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(ival);
                            data++;
                            थांबा;
                        स्थिती CV_8S:
                            *(अक्षर*)data = saturate_cast<schar>(ival);
                            data++;
                            थांबा;
                        स्थिती CV_Bool:
                            *(तार्किक*)data = ival != 0;
                            data++;
                            थांबा;
                        स्थिती CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(ival);
                            data += आकार(ushort);
                            थांबा;
                        स्थिती CV_16S:
                            *(लघु*)data = saturate_cast<लघु>(ival);
                            data += आकार(लघु);
                            थांबा;
                        स्थिती CV_32U:
                            *(अचिन्हित*)data = (अचिन्हित)std::max(ival, (int64_t)0);
                            data += आकार(अचिन्हित);
                            थांबा;
                        स्थिती CV_32S:
                            *(पूर्णांक*)data = (पूर्णांक)ival;
                            data += आकार(पूर्णांक);
                            थांबा;
                        स्थिती CV_32F:
                            *(दशांश*)data = (दशांश)ival;
                            data += आकार(दशांश);
                            थांबा;
                        स्थिती CV_64U:
                            *(uint64_t*)data = (uint64_t)ival;
                            data += आकार(uint64_t);
                            थांबा;
                        स्थिती CV_64S:
                            *(int64_t*)data = (int64_t)ival;
                            data += आकार(int64_t);
                            थांबा;
                        स्थिती CV_64F:
                            *(द्विगुण*)data = (द्विगुण)ival;
                            data += आकार(द्विगुण);
                            थांबा;
                        स्थिती CV_16F:
                            *(hfloat*)data = hfloat((दशांश)ival);
                            data += आकार(hfloat);
                            थांबा;
                        स्थिती CV_16BF:
                            *(bfloat*)data = bfloat((दशांश)ival);
                            data += आकार(bfloat);
                            थांबा;
                        स्थिती CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((दशांश)ival);
                            data += आकार(fp8_t);
                            थांबा;
                        स्थिती CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((दशांश)ival);
                            data += आकार(fp8a_t);
                            थांबा;
                        पूर्वनिर्धारित:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    अन्यथा जर( गाठ.isReal() )
                    {
                        द्विगुण fval = (द्विगुण)गाठ;

                        निवड( elem_type )
                        {
                        स्थिती CV_8U:
                            *(uchar*)data = saturate_cast<uchar>(fval);
                            data++;
                            थांबा;
                        स्थिती CV_8S:
                            *(अक्षर*)data = saturate_cast<schar>(fval);
                            data++;
                            थांबा;
                        स्थिती CV_16U:
                            *(ushort*)data = saturate_cast<ushort>(fval);
                            data += आकार(ushort);
                            थांबा;
                        स्थिती CV_16S:
                            *(लघु*)data = saturate_cast<लघु>(fval);
                            data += आकार(लघु);
                            थांबा;
                        स्थिती CV_32U:
                            *(पूर्णांक*)data = saturate_cast<अचिन्हित>(fval);
                            data += आकार(पूर्णांक);
                            थांबा;
                        स्थिती CV_32S:
                            *(पूर्णांक*)data = saturate_cast<पूर्णांक>(fval);
                            data += आकार(पूर्णांक);
                            थांबा;
                        स्थिती CV_32F:
                            *(दशांश*)data = (दशांश)fval;
                            data += आकार(दशांश);
                            थांबा;
                        स्थिती CV_64U:
                            *(uint64_t*)data = (uint64_t)round(std::max(fval, 0.));
                            data += आकार(uint64_t);
                            थांबा;
                        स्थिती CV_64S:
                            *(int64_t*)data = (int64_t)round(std::max(fval, 0.));
                            data += आकार(int64_t);
                            थांबा;
                        स्थिती CV_64F:
                            *(द्विगुण*)data = fval;
                            data += आकार(द्विगुण);
                            थांबा;
                        स्थिती CV_16F:
                            *(hfloat*)data = hfloat((दशांश)fval);
                            data += आकार(hfloat);
                            थांबा;
                        स्थिती CV_16BF:
                            *(bfloat*)data = bfloat((दशांश)fval);
                            data += आकार(bfloat);
                            थांबा;
                        स्थिती CV_8F_E4M3FN:
                            *(fp8_t*)data = fp8_t((दशांश)fval);
                            data += आकार(fp8_t);
                            थांबा;
                        स्थिती CV_8F_E4M3FNUZ:
                            *(fp8a_t*)data = fp8a_t((दशांश)fval);
                            data += आकार(fp8a_t);
                            थांबा;
                        पूर्वनिर्धारित:
                            CV_Error( Error::StsUnsupportedFormat, "Unsupported type" );
                        }
                    }
                    अन्यथा
                        CV_Error( Error::StsError, "readRawData can only be used to read plain sequences of numbers" );
                }
                offset = (पूर्णांक)(data - data0);
            }
        }
    }

    परत *हे;
}

तार्किक File_गाठ_Iterator::equalTo(स्थिरांक File_गाठ_Iterator& it) स्थिरांक
{
    परत fs == it.fs && blockIdx == it.blockIdx && ofs == it.ofs &&
           idx == it.idx && गाठ_NElems == it.गाठ_NElems;
}

माप_प्रकार File_गाठ_Iterator::remaining() स्थिरांक
{
    परत गाठ_NElems - idx;
}

तार्किक चालक == ( स्थिरांक File_गाठ_Iterator& it1, स्थिरांक File_गाठ_Iterator& it2 )
{
    परत it1.equalTo(it2);
}

तार्किक चालक != ( स्थिरांक File_गाठ_Iterator& it1, स्थिरांक File_गाठ_Iterator& it2 )
{
    परत !it1.equalTo(it2);
}

शून्य पठ(स्थिरांक File_गाठ& गाठ, पूर्णांक& val, पूर्णांक default_val)
{
    val = default_val;
    जर( !गाठ.empty() )
    {
        val = (पूर्णांक)गाठ;
    }
}

शून्य पठ(स्थिरांक File_गाठ& गाठ, int64_t& val, int64_t default_val)
{
    val = default_val;
    जर( !गाठ.empty() )
    {
        val = (int64_t)गाठ;
    }
}

शून्य पठ(स्थिरांक File_गाठ& गाठ, द्विगुण& val, द्विगुण default_val)
{
    val = default_val;
    जर( !गाठ.empty() )
    {
        val = (द्विगुण)गाठ;
    }
}

शून्य पठ(स्थिरांक File_गाठ& गाठ, दशांश& val, दशांश default_val)
{
    val = default_val;
    जर( !गाठ.empty() )
    {
        val = (दशांश)गाठ;
    }
}

शून्य पठ(स्थिरांक File_गाठ& गाठ, std::string& val, स्थिरांक std::string& default_val)
{
    val = default_val;
    जर( !गाठ.empty() )
    {
        val = (std::string)गाठ;
    }
}
शून्य FileStorage::Impl::लिख(स्थिरांक String &किल्ली, तार्किक value)
{
    CV_Assert(write_mode);
    जर (fmt != FileStorage::FORMAT_YAML)
    {
        // Legacy behavior: Write as integer 1 or 0
        getEmitter().लिख(किल्ली.c_str(), (पूर्णांक)value);
    }
    अन्यथा
    {
        // Default/Modern behavior: Write as "true" or "false"
        getEmitter().लिख(किल्ली.c_str(), value ? "true" : "false", असत्य);
    }
}

FileStorage_API::~FileStorage_API() {}

नामाकाश internal
{

WriteStructContext::WriteStructContext(FileStorage& _fs, स्थिरांक std::string& name,
                                       पूर्णांक flags, स्थिरांक std::string& typeName)
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
