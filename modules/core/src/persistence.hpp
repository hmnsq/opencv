// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_PERSISTENCE_HPP
#define SRC_PERSISTENCE_HPP

#अन्तर्भाव <deque>
#अन्तर्भाव <sstream>
#अन्तर्भाव <string>
#अन्तर्भाव <iterator>

#define USE_ZLIB 1
#जर USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#अन्यथा
प्रकारव्याख्या शून्य* gzFile;
#endif

//=====================================================================================

स्थिर स्थिरांक माप_प्रकार PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
स्थिर स्थिरांक पूर्णांक CV_PERSISTENCE_MAX_DEPTH = 1024;

नामाकाश base64 {

गणना
{
    HEADER_SIZE         = 24,
    ENCODED_HEADER_SIZE = 32
};

} // base64::

//=====================================================================================

#define CV_FS_MAX_LEN 4096
#define CV_FS_MAX_FMT_PAIRS  128

/****************************************************************************************\
*                            Common macros and type definitions                          *
\****************************************************************************************/

#define cv_isprint(c)     ((uchar)(c) >= (uchar)' ')
#define cv_isprint_or_tab(c)  ((uchar)(c) >= (uchar)' ' || (c) == '\t')

ओळीत तार्किक cv_isalnum(अक्षर c)
{
    परत ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

ओळीत तार्किक cv_isalpha(अक्षर c)
{
    परत ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

ओळीत तार्किक cv_isdigit(अक्षर c)
{
    परत '0' <= c && c <= '9';
}

ओळीत तार्किक cv_isspace(अक्षर c)
{
    परत (9 <= c && c <= 13) || c == ' ';
}

ओळीत अक्षर* cv_skip_BOM(अक्षर* ptr)
{
    जर((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      परत ptr + 3;
    }
    परत ptr;
}

नामाकाश cv
{
नामाकाश fs
{
पूर्णांक strcasecmp(स्थिरांक अक्षर* str1, स्थिरांक अक्षर* str2);
अक्षर* itoa( पूर्णांक _val, अक्षर* buffer, पूर्णांक /*radix*/ );
अक्षर* itoa( int64_t _val, अक्षर* buffer, पूर्णांक /*radix*/, तार्किक _signed );
अक्षर* floatToString( अक्षर* buf, माप_प्रकार bufSize, दशांश value, तार्किक halfprecision, तार्किक explicitZero );
अक्षर* doubleToString( अक्षर* buf, माप_प्रकार bufSize, द्विगुण value, तार्किक explicitZero );

पूर्णांक calcStructSize( स्थिरांक अक्षर* dt, पूर्णांक initial_size );
पूर्णांक calcElemSize( स्थिरांक अक्षर* dt, पूर्णांक initial_size );
CV_DEPRECATED अक्षर* encodeFormat( पूर्णांक elem_type, अक्षर* dt );
अक्षर* encodeFormat( पूर्णांक elem_type, अक्षर* dt, माप_प्रकार dt_len );
पूर्णांक decodeFormat( स्थिरांक अक्षर* dt, पूर्णांक* fmt_pairs, पूर्णांक max_len );
पूर्णांक decodeSimpleFormat( स्थिरांक अक्षर* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) करा { (शून्य)fs; पात(); } पर्यंत (0)
#अन्यथा
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() करा { \
    CV_DbgAssert(ptr); \
    जर((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} पर्यंत (0)


वर्ग FileStorageParser;
वर्ग FileStorageEmitter;

संरचना FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( स्थिरांक std::string& _struct_tag,
                 पूर्णांक _struct_flags, पूर्णांक _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    पूर्णांक flags;
    पूर्णांक indent;
};

वर्ग FileStorage_API
{
सार्वजनिक:
    आभासी ~FileStorage_API();
    आभासी FileStorage* getFS() = 0;

    आभासी पूर्णांक getFormat() स्थिरांक = 0;

    आभासी शून्य माला_दो( स्थिरांक अक्षर* str ) = 0;
    आभासी अक्षर* माला_लो() = 0;
    आभासी तार्किक eof() = 0;
    आभासी शून्य setEof() = 0;
    आभासी शून्य closeFile() = 0;
    आभासी शून्य शुरुआत() = 0;
    आभासी अक्षर* resizeWriteBuffer( अक्षर* ptr, पूर्णांक len ) = 0;
    आभासी अक्षर* bufferPtr() स्थिरांक = 0;
    आभासी अक्षर* bufferStart() स्थिरांक = 0;
    आभासी अक्षर* bufferEnd() स्थिरांक = 0;
    आभासी शून्य setBufferPtr(अक्षर* ptr) = 0;
    आभासी अक्षर* flush() = 0;
    आभासी शून्य setNonEmpty() = 0;
    आभासी पूर्णांक wrapMargin() स्थिरांक = 0;

    आभासी FStructData& getCurrentStruct() = 0;

    आभासी शून्य convertToCollection( पूर्णांक type, File_गाठ& गाठ ) = 0;
    आभासी File_गाठ add_गाठ( File_गाठ& collection, स्थिरांक std::string& किल्ली,
                               पूर्णांक type, स्थिरांक शून्य* value=0, पूर्णांक len=-1 ) = 0;
    आभासी शून्य finalizeCollection( File_गाठ& collection ) = 0;
    आभासी द्विगुण म_से_भग्न(अक्षर* ptr, अक्षर** endptr) = 0;

    आभासी अक्षर* parseBase64(अक्षर* ptr, पूर्णांक indent, File_गाठ& collection) = 0;
    CV_NORETURN
    आभासी शून्य parseError(स्थिरांक अक्षर* funcname, स्थिरांक std::string& संदेश,
                            स्थिरांक अक्षर* filename, पूर्णांक lineno) = 0;

खाजगी:
    गणना Base64State{
        Uncertain,
        NotUse,
        InUse,
    };

    मित्र वर्ग cv::FileStorage::Impl;
    मित्र वर्ग cv::FileStorage;
    मित्र वर्ग JSONEmitter;
    मित्र वर्ग XMLEmitter;
    मित्र वर्ग YAMLEmitter;

    आभासी शून्य check_if_write_struct_is_delayed(तार्किक change_type_to_base64 = असत्य) = 0;
    आभासी शून्य switch_to_Base64_state(Base64State state) = 0;
    आभासी Base64State get_state_of_writing_base64() = 0;
    आभासी पूर्णांक get_space() = 0;
};

वर्ग FileStorageEmitter
{
सार्वजनिक:
    आभासी ~FileStorageEmitter() {}

    आभासी FStructData startWriteStruct( स्थिरांक FStructData& parent, स्थिरांक अक्षर* किल्ली,
                                          पूर्णांक struct_flags, स्थिरांक अक्षर* type_name=0 ) = 0;
    आभासी शून्य endWriteStruct(स्थिरांक FStructData& current_struct) = 0;
    आभासी शून्य लिख(स्थिरांक अक्षर* किल्ली, पूर्णांक value) = 0;
    आभासी शून्य लिख(स्थिरांक अक्षर* किल्ली, int64_t value) = 0;
    आभासी शून्य लिख(स्थिरांक अक्षर* किल्ली, द्विगुण value) = 0;
    आभासी शून्य लिख(स्थिरांक अक्षर* किल्ली, स्थिरांक अक्षर* value, तार्किक quote) = 0;
    आभासी शून्य writeScalar(स्थिरांक अक्षर* किल्ली, स्थिरांक अक्षर* value) = 0;
    आभासी शून्य writeComment(स्थिरांक अक्षर* comment, तार्किक eol_comment) = 0;
    आभासी शून्य startNextStream() = 0;
};

वर्ग FileStorageParser
{
सार्वजनिक:
    आभासी ~FileStorageParser() {}
    आभासी तार्किक parse(अक्षर* ptr) = 0;
    आभासी तार्किक getBase64Row(अक्षर* ptr, पूर्णांक indent, अक्षर* &beg, अक्षर* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
