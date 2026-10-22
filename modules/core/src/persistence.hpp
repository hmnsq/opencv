// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SRC_PERSISTENCE_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SRC_PERSISTENCE_HPP

#ਅਨ੍ਤਰ੍ਭਾਵ <deque>
#ਅਨ੍ਤਰ੍ਭਾਵ <sstream>
#ਅਨ੍ਤਰ੍ਭਾਵ <string>
#ਅਨ੍ਤਰ੍ਭਾਵ <iterator>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_ZLIB 1
#ਜੇ USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef FILE_ਵਿੱਥ_ਬਿੱਟ
#    define FILE_ਵਿੱਥ_ਬਿੱਟ 0
#  endif
#  include <zlib.h>
#ਨਹੀਂਤਾਂ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ* gzFile;
#ਜੇ_ਅੰਤ

//=====================================================================================

ਸਥਿਰ ਅਚਲ size_t PARSER_BASE64_ਬਫਰ_ਮਾਪ = 1024U * 1024U / 8U;
ਸਥਿਰ ਅਚਲ ਪੂਰਨ CV_PERSISTENCE_MAX_ਡੂੰਘਾਈ = 1024;

ਨਾਮਾਕਾਸ਼ base64 {

ਗਿਣਤੀ
{
    ਸਿਰਲੇਖ_ਮਾਪ         = 24,
    ENCODED_ਸਿਰਲੇਖ_ਮਾਪ = 32
};

} // base64::

//=====================================================================================

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_FS_MAX_LEN 4096
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_FS_MAX_FMT_ਜੋੜੇ  128

/****************************************************************************************\
*                            Common macros and type definitions                          *
\****************************************************************************************/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_isprint(c)     ((uchar)(c) >= (uchar)' ')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cv_isprint_or_tab(c)  ((uchar)(c) >= (uchar)' ' || (c) == '\t')

ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ cv_isalnum(ਅੱਖਰ c)
{
    ਵਾਪਸ ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ cv_isalpha(ਅੱਖਰ c)
{
    ਵਾਪਸ ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ cv_isdigit(ਅੱਖਰ c)
{
    ਵਾਪਸ '0' <= c && c <= '9';
}

ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ cv_isspace(ਅੱਖਰ c)
{
    ਵਾਪਸ (9 <= c && c <= 13) || c == ' ';
}

ਸਤਰਅੰਦਰ ਅੱਖਰ* cv_skip_BOM(ਅੱਖਰ* ptr)
{
    ਜੇ((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      ਵਾਪਸ ptr + 3;
    }
    ਵਾਪਸ ptr;
}

ਨਾਮਾਕਾਸ਼ cv
{
ਨਾਮਾਕਾਸ਼ fs
{
ਪੂਰਨ strcasecmp(ਅਚਲ ਅੱਖਰ* str1, ਅਚਲ ਅੱਖਰ* str2);
ਅੱਖਰ* itoa( ਪੂਰਨ _val, ਅੱਖਰ* ਬਫਰ, ਪੂਰਨ /*radix*/ );
ਅੱਖਰ* itoa( int64_t _val, ਅੱਖਰ* ਬਫਰ, ਪੂਰਨ /*radix*/, ਤਾਰਕਿਕ _signed );
ਅੱਖਰ* float_ਮੰਜ਼ਿਲ_String( ਅੱਖਰ* buf, size_t buf_ਮਾਪ, ਦਸ਼ਮਲਵ value, ਤਾਰਕਿਕ halfprecision, ਤਾਰਕਿਕ explicitZero );
ਅੱਖਰ* double_ਮੰਜ਼ਿਲ_String( ਅੱਖਰ* buf, size_t buf_ਮਾਪ, ਦੁੱਗਣਾ value, ਤਾਰਕਿਕ explicitZero );

ਪੂਰਨ calc_Struct_ਮਾਪ( ਅਚਲ ਅੱਖਰ* dt, ਪੂਰਨ initial_ਮਾਪ );
ਪੂਰਨ calc_Elem_ਮਾਪ( ਅਚਲ ਅੱਖਰ* dt, ਪੂਰਨ initial_ਮਾਪ );
CV_DEPRECATED ਅੱਖਰ* encodeFormat( ਪੂਰਨ elem_type, ਅੱਖਰ* dt );
ਅੱਖਰ* encodeFormat( ਪੂਰਨ elem_type, ਅੱਖਰ* dt, size_t dt_len );
ਪੂਰਨ decodeFormat( ਅਚਲ ਅੱਖਰ* dt, ਪੂਰਨ* fmt_ਜੋੜੇ, ਪੂਰਨ max_len );
ਪੂਰਨ decodeSimpleFormat( ਅਚਲ ਅੱਖਰ* dt );
}


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_STATIC_ANALYSIS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_PARSE_ERROR_CPP(errmsg) ਕਰੋ { (ਖਾਲੀ)fs; abort(); } ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#ਜੇ_ਅੰਤ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_PERSISTENCE_CHECK_ਸਿਰਾ_ਦਾ_ਬਫਰ_BUG_CPP() ਕਰੋ { \
    CV_DbgAssert(ptr); \
    ਜੇ((ptr)[0] == 0 && (ptr) == fs->ਬਫਰ_ਸਿਰਾ_2() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} ਜਦੋਂ (0)


ਵਰਗ FileStorageParser;
ਵਰਗ FileStorageEmitter;

ਬਣਤਰ FStruct_ਅੰਕੜੇ
{
    FStruct_ਅੰਕੜੇ() { indent = flags = 0; }
    FStruct_ਅੰਕੜੇ( ਅਚਲ std::string& struct_ਟੈਗ,
                 ਪੂਰਨ _struct_flags, ਪੂਰਨ _struct_indent )
    {
        ਟੈਗ = struct_ਟੈਗ;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string ਟੈਗ;
    ਪੂਰਨ flags;
    ਪੂਰਨ indent;
};

ਵਰਗ FileStorage_API
{
ਜਨਤਕ:
    ਆਭਾਸੀ ~FileStorage_API();
    ਆਭਾਸੀ FileStorage* getFS() = 0;

    ਆਭਾਸੀ ਪੂਰਨ getFormat() ਅਚਲ = 0;

    ਆਭਾਸੀ ਖਾਲੀ puts( ਅਚਲ ਅੱਖਰ* str ) = 0;
    ਆਭਾਸੀ ਅੱਖਰ* gets() = 0;
    ਆਭਾਸੀ ਤਾਰਕਿਕ eof() = 0;
    ਆਭਾਸੀ ਖਾਲੀ ਸੈੱਟ_Eof() = 0;
    ਆਭਾਸੀ ਖਾਲੀ closeFile() = 0;
    ਆਭਾਸੀ ਖਾਲੀ rewind() = 0;
    ਆਭਾਸੀ ਅੱਖਰ* resize_Write_ਬਫਰ( ਅੱਖਰ* ptr, ਪੂਰਨ len ) = 0;
    ਆਭਾਸੀ ਅੱਖਰ* ਬਫਰ_Ptr() ਅਚਲ = 0;
    ਆਭਾਸੀ ਅੱਖਰ* ਬਫਰ_ਸ਼ੁਰੂ() ਅਚਲ = 0;
    ਆਭਾਸੀ ਅੱਖਰ* ਬਫਰ_ਸਿਰਾ_2() ਅਚਲ = 0;
    ਆਭਾਸੀ ਖਾਲੀ ਸੈੱਟ_ਬਫਰ_Ptr(ਅੱਖਰ* ptr) = 0;
    ਆਭਾਸੀ ਅੱਖਰ* flush() = 0;
    ਆਭਾਸੀ ਖਾਲੀ ਸੈੱਟ_Non_ਖਾਲੀ_ਥਾਵਾਂ() = 0;
    ਆਭਾਸੀ ਪੂਰਨ wrapMargin() ਅਚਲ = 0;

    ਆਭਾਸੀ FStruct_ਅੰਕੜੇ& getCurrentStruct() = 0;

    ਆਭਾਸੀ ਖਾਲੀ convert_ਮੰਜ਼ਿਲ_Collection( ਪੂਰਨ type, File_ਗੰਢ& ਗੰਢ ) = 0;
    ਆਭਾਸੀ File_ਗੰਢ ਜੋੜੋ_ਗੰਢ( File_ਗੰਢ& collection, ਅਚਲ std::string& ਕੁੰਜੀ,
                               ਪੂਰਨ type, ਅਚਲ ਖਾਲੀ* value=0, ਪੂਰਨ len=-1 ) = 0;
    ਆਭਾਸੀ ਖਾਲੀ finalizeCollection( File_ਗੰਢ& collection ) = 0;
    ਆਭਾਸੀ ਦੁੱਗਣਾ strtod(ਅੱਖਰ* ptr, ਅੱਖਰ** endptr) = 0;

    ਆਭਾਸੀ ਅੱਖਰ* parseBase64(ਅੱਖਰ* ptr, ਪੂਰਨ indent, File_ਗੰਢ& collection) = 0;
    CV_NORETURN
    ਆਭਾਸੀ ਖਾਲੀ parseError(ਅਚਲ ਅੱਖਰ* funcname, ਅਚਲ std::string& ਸੁਨੇਹਾ,
                            ਅਚਲ ਅੱਖਰ* filename, ਪੂਰਨ lineno) = 0;

ਨਿਜੀ:
    ਗਿਣਤੀ Base64State{
        Uncertain,
        NotUse,
        ਵਿੱਚ_Use,
    };

    ਮਿੱਤਰ ਵਰਗ cv::FileStorage::Impl;
    ਮਿੱਤਰ ਵਰਗ cv::FileStorage;
    ਮਿੱਤਰ ਵਰਗ JSONEmitter;
    ਮਿੱਤਰ ਵਰਗ XMLEmitter;
    ਮਿੱਤਰ ਵਰਗ YAMLEmitter;

    ਆਭਾਸੀ ਖਾਲੀ check_if_write_struct_is_delayed(ਤਾਰਕਿਕ change_type_ਮੰਜ਼ਿਲ_base64 = ਝੂਠ) = 0;
    ਆਭਾਸੀ ਖਾਲੀ switch_ਮੰਜ਼ਿਲ_Base64_state(Base64State state) = 0;
    ਆਭਾਸੀ Base64State get_state_ਦਾ_writing_base64() = 0;
    ਆਭਾਸੀ ਪੂਰਨ get_space() = 0;
};

ਵਰਗ FileStorageEmitter
{
ਜਨਤਕ:
    ਆਭਾਸੀ ~FileStorageEmitter() {}

    ਆਭਾਸੀ FStruct_ਅੰਕੜੇ ਸ਼ੁਰੂ_Write_Struct( ਅਚਲ FStruct_ਅੰਕੜੇ& ਮਾਪਾ, ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ,
                                          ਪੂਰਨ struct_flags, ਅਚਲ ਅੱਖਰ* type_name=0 ) = 0;
    ਆਭਾਸੀ ਖਾਲੀ ਸਿਰਾ_Write_Struct(ਅਚਲ FStruct_ਅੰਕੜੇ& current_struct) = 0;
    ਆਭਾਸੀ ਖਾਲੀ ਲਿਖ(ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਪੂਰਨ value) = 0;
    ਆਭਾਸੀ ਖਾਲੀ ਲਿਖ(ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, int64_t value) = 0;
    ਆਭਾਸੀ ਖਾਲੀ ਲਿਖ(ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਦੁੱਗਣਾ value) = 0;
    ਆਭਾਸੀ ਖਾਲੀ ਲਿਖ(ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ* value, ਤਾਰਕਿਕ quote) = 0;
    ਆਭਾਸੀ ਖਾਲੀ writeScalar(ਅਚਲ ਅੱਖਰ* ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ* value) = 0;
    ਆਭਾਸੀ ਖਾਲੀ writeComment(ਅਚਲ ਅੱਖਰ* comment, ਤਾਰਕਿਕ eol_comment) = 0;
    ਆਭਾਸੀ ਖਾਲੀ ਸ਼ੁਰੂ_ਅਗਲਾ_ਧਾਰਾ() = 0;
};

ਵਰਗ FileStorageParser
{
ਜਨਤਕ:
    ਆਭਾਸੀ ~FileStorageParser() {}
    ਆਭਾਸੀ ਤਾਰਕਿਕ parse(ਅੱਖਰ* ptr) = 0;
    ਆਭਾਸੀ ਤਾਰਕਿਕ get_Base64_ਪੰਕਤੀ(ਅੱਖਰ* ptr, ਪੂਰਨ indent, ਅੱਖਰ* &beg, ਅੱਖਰ* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#ਜੇ_ਅੰਤ // SRC_PERSISTENCE_HPP
