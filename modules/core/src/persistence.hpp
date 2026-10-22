// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_PERSISTENCE_HPP
#define SRC_PERSISTENCE_HPP

#ಅನ್ತರ್ಭಾವ <deque>
#ಅನ್ತರ್ಭಾವ <sstream>
#ಅನ್ತರ್ಭಾವ <string>
#ಅನ್ತರ್ಭಾವ <iterator>

#define USE_ZLIB 1
#ಯದಿ USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#ಅನ್ಯಥಾ
ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ* gzFile;
#endif

//=====================================================================================

ಸ್ಥಿರ ಧ್ರುವ size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
ಸ್ಥಿರ ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ CV_PERSISTENCE_MAX_DEPTH = 1024;

ನಾಮಾಕಾಶ base64 {

ಗಣನಾ
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

ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ cv_isalnum(ವರ್ಣ c)
{
    ಪ್ರತ್ಯಾಗಮ ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ cv_isalpha(ವರ್ಣ c)
{
    ಪ್ರತ್ಯಾಗಮ ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ cv_isdigit(ವರ್ಣ c)
{
    ಪ್ರತ್ಯಾಗಮ '0' <= c && c <= '9';
}

ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ cv_isspace(ವರ್ಣ c)
{
    ಪ್ರತ್ಯಾಗಮ (9 <= c && c <= 13) || c == ' ';
}

ಅನ್ತಃಸ್ಥ ವರ್ಣ* cv_skip_BOM(ವರ್ಣ* ptr)
{
    ಯದಿ((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      ಪ್ರತ್ಯಾಗಮ ptr + 3;
    }
    ಪ್ರತ್ಯಾಗಮ ptr;
}

ನಾಮಾಕಾಶ cv
{
ನಾಮಾಕಾಶ fs
{
ಪೂರ್ಣಾಙ್ಕ strcasecmp(ಧ್ರುವ ವರ್ಣ* str1, ಧ್ರುವ ವರ್ಣ* str2);
ವರ್ಣ* itoa( ಪೂರ್ಣಾಙ್ಕ _val, ವರ್ಣ* buffer, ಪೂರ್ಣಾಙ್ಕ /*radix*/ );
ವರ್ಣ* itoa( int64_t _val, ವರ್ಣ* buffer, ಪೂರ್ಣಾಙ್ಕ /*radix*/, ತಾರ್ಕಿಕ _signed );
ವರ್ಣ* floatToString( ವರ್ಣ* buf, size_t bufSize, ಭಿನ್ನ value, ತಾರ್ಕಿಕ halfprecision, ತಾರ್ಕಿಕ explicitZero );
ವರ್ಣ* doubleToString( ವರ್ಣ* buf, size_t bufSize, ದ್ವಿಗುಣ value, ತಾರ್ಕಿಕ explicitZero );

ಪೂರ್ಣಾಙ್ಕ calcStructSize( ಧ್ರುವ ವರ್ಣ* dt, ಪೂರ್ಣಾಙ್ಕ initial_size );
ಪೂರ್ಣಾಙ್ಕ calcElemSize( ಧ್ರುವ ವರ್ಣ* dt, ಪೂರ್ಣಾಙ್ಕ initial_size );
CV_DEPRECATED ವರ್ಣ* encodeFormat( ಪೂರ್ಣಾಙ್ಕ elem_type, ವರ್ಣ* dt );
ವರ್ಣ* encodeFormat( ಪೂರ್ಣಾಙ್ಕ elem_type, ವರ್ಣ* dt, size_t dt_len );
ಪೂರ್ಣಾಙ್ಕ decodeFormat( ಧ್ರುವ ವರ್ಣ* dt, ಪೂರ್ಣಾಙ್ಕ* fmt_pairs, ಪೂರ್ಣಾಙ್ಕ max_len );
ಪೂರ್ಣಾಙ್ಕ decodeSimpleFormat( ಧ್ರುವ ವರ್ಣ* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) ಕುರು { (ಶೂನ್ಯ)fs; abort(); } ಯಾವತ್ (0)
#ಅನ್ಯಥಾ
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() ಕುರು { \
    CV_DbgAssert(ptr); \
    ಯದಿ((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} ಯಾವತ್ (0)


ವರ್ಗ FileStorageParser;
ವರ್ಗ FileStorageEmitter;

ಸಂರಚನಾ FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( ಧ್ರುವ std::string& _struct_tag,
                 ಪೂರ್ಣಾಙ್ಕ _struct_flags, ಪೂರ್ಣಾಙ್ಕ _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    ಪೂರ್ಣಾಙ್ಕ flags;
    ಪೂರ್ಣಾಙ್ಕ indent;
};

ವರ್ಗ FileStorage_API
{
ಸಾರ್ವಜನಿಕ:
    ಆಭಾಸ ~FileStorage_API();
    ಆಭಾಸ FileStorage* getFS() = 0;

    ಆಭಾಸ ಪೂರ್ಣಾಙ್ಕ getFormat() ಧ್ರುವ = 0;

    ಆಭಾಸ ಶೂನ್ಯ puts( ಧ್ರುವ ವರ್ಣ* str ) = 0;
    ಆಭಾಸ ವರ್ಣ* gets() = 0;
    ಆಭಾಸ ತಾರ್ಕಿಕ eof() = 0;
    ಆಭಾಸ ಶೂನ್ಯ setEof() = 0;
    ಆಭಾಸ ಶೂನ್ಯ closeFile() = 0;
    ಆಭಾಸ ಶೂನ್ಯ rewind() = 0;
    ಆಭಾಸ ವರ್ಣ* resizeWriteBuffer( ವರ್ಣ* ptr, ಪೂರ್ಣಾಙ್ಕ len ) = 0;
    ಆಭಾಸ ವರ್ಣ* bufferPtr() ಧ್ರುವ = 0;
    ಆಭಾಸ ವರ್ಣ* bufferStart() ಧ್ರುವ = 0;
    ಆಭಾಸ ವರ್ಣ* bufferEnd() ಧ್ರುವ = 0;
    ಆಭಾಸ ಶೂನ್ಯ setBufferPtr(ವರ್ಣ* ptr) = 0;
    ಆಭಾಸ ವರ್ಣ* flush() = 0;
    ಆಭಾಸ ಶೂನ್ಯ setNonEmpty() = 0;
    ಆಭಾಸ ಪೂರ್ಣಾಙ್ಕ wrapMargin() ಧ್ರುವ = 0;

    ಆಭಾಸ FStructData& getCurrentStruct() = 0;

    ಆಭಾಸ ಶೂನ್ಯ convertToCollection( ಪೂರ್ಣಾಙ್ಕ type, File_ಗಂಟು& ಗಂಟು ) = 0;
    ಆಭಾಸ File_ಗಂಟು add_ಗಂಟು( File_ಗಂಟು& collection, ಧ್ರುವ std::string& ಕೀಲಿ,
                               ಪೂರ್ಣಾಙ್ಕ type, ಧ್ರುವ ಶೂನ್ಯ* value=0, ಪೂರ್ಣಾಙ್ಕ len=-1 ) = 0;
    ಆಭಾಸ ಶೂನ್ಯ finalizeCollection( File_ಗಂಟು& collection ) = 0;
    ಆಭಾಸ ದ್ವಿಗುಣ strtod(ವರ್ಣ* ptr, ವರ್ಣ** endptr) = 0;

    ಆಭಾಸ ವರ್ಣ* parseBase64(ವರ್ಣ* ptr, ಪೂರ್ಣಾಙ್ಕ indent, File_ಗಂಟು& collection) = 0;
    CV_NORETURN
    ಆಭಾಸ ಶೂನ್ಯ parseError(ಧ್ರುವ ವರ್ಣ* funcname, ಧ್ರುವ std::string& ಸಂದೇಶ,
                            ಧ್ರುವ ವರ್ಣ* filename, ಪೂರ್ಣಾಙ್ಕ lineno) = 0;

ಗುಪ್ತ:
    ಗಣನಾ Base64State{
        Uncertain,
        NotUse,
        InUse,
    };

    friend ವರ್ಗ cv::FileStorage::Impl;
    friend ವರ್ಗ cv::FileStorage;
    friend ವರ್ಗ JSONEmitter;
    friend ವರ್ಗ XMLEmitter;
    friend ವರ್ಗ YAMLEmitter;

    ಆಭಾಸ ಶೂನ್ಯ check_if_write_struct_is_delayed(ತಾರ್ಕಿಕ change_type_to_base64 = ಅಸತ್ಯ) = 0;
    ಆಭಾಸ ಶೂನ್ಯ switch_to_Base64_state(Base64State state) = 0;
    ಆಭಾಸ Base64State get_state_of_writing_base64() = 0;
    ಆಭಾಸ ಪೂರ್ಣಾಙ್ಕ get_space() = 0;
};

ವರ್ಗ FileStorageEmitter
{
ಸಾರ್ವಜನಿಕ:
    ಆಭಾಸ ~FileStorageEmitter() {}

    ಆಭಾಸ FStructData startWriteStruct( ಧ್ರುವ FStructData& parent, ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ,
                                          ಪೂರ್ಣಾಙ್ಕ struct_flags, ಧ್ರುವ ವರ್ಣ* type_name=0 ) = 0;
    ಆಭಾಸ ಶೂನ್ಯ endWriteStruct(ಧ್ರುವ FStructData& current_struct) = 0;
    ಆಭಾಸ ಶೂನ್ಯ ಲಿಖ(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಪೂರ್ಣಾಙ್ಕ value) = 0;
    ಆಭಾಸ ಶೂನ್ಯ ಲಿಖ(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, int64_t value) = 0;
    ಆಭಾಸ ಶೂನ್ಯ ಲಿಖ(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ದ್ವಿಗುಣ value) = 0;
    ಆಭಾಸ ಶೂನ್ಯ ಲಿಖ(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ* value, ತಾರ್ಕಿಕ quote) = 0;
    ಆಭಾಸ ಶೂನ್ಯ writeScalar(ಧ್ರುವ ವರ್ಣ* ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ* value) = 0;
    ಆಭಾಸ ಶೂನ್ಯ writeComment(ಧ್ರುವ ವರ್ಣ* comment, ತಾರ್ಕಿಕ eol_comment) = 0;
    ಆಭಾಸ ಶೂನ್ಯ startNextStream() = 0;
};

ವರ್ಗ FileStorageParser
{
ಸಾರ್ವಜನಿಕ:
    ಆಭಾಸ ~FileStorageParser() {}
    ಆಭಾಸ ತಾರ್ಕಿಕ parse(ವರ್ಣ* ptr) = 0;
    ಆಭಾಸ ತಾರ್ಕಿಕ getBase64Row(ವರ್ಣ* ptr, ಪೂರ್ಣಾಙ್ಕ indent, ವರ್ಣ* &beg, ವರ್ಣ* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
