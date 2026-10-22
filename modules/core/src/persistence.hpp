// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_PERSISTENCE_HPP
#define SRC_PERSISTENCE_HPP

#شامل <deque>
#شامل <sstream>
#شامل <string>
#شامل <iterator>

#define USE_ZLIB 1
#اگر USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#وگرنه
تعریفنوع تهی* gzFile;
#endif

//=====================================================================================

ایستا ثابت size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
ایستا ثابت صحیح CV_PERSISTENCE_MAX_DEPTH = 1024;

فضاینام base64 {

شمارش
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

درونخطی منطقی cv_isalnum(نویسه c)
{
    بازگشت ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

درونخطی منطقی cv_isalpha(نویسه c)
{
    بازگشت ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

درونخطی منطقی cv_isdigit(نویسه c)
{
    بازگشت '0' <= c && c <= '9';
}

درونخطی منطقی cv_isspace(نویسه c)
{
    بازگشت (9 <= c && c <= 13) || c == ' ';
}

درونخطی نویسه* cv_skip_BOM(نویسه* ptr)
{
    اگر((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      بازگشت ptr + 3;
    }
    بازگشت ptr;
}

فضاینام cv
{
فضاینام fs
{
صحیح strcasecmp(ثابت نویسه* str1, ثابت نویسه* str2);
نویسه* itoa( صحیح _val, نویسه* buffer, صحیح /*radix*/ );
نویسه* itoa( int64_t _val, نویسه* buffer, صحیح /*radix*/, منطقی _signed );
نویسه* floatToString( نویسه* buf, size_t bufSize, اعشاری value, منطقی halfprecision, منطقی explicitZero );
نویسه* doubleToString( نویسه* buf, size_t bufSize, دوگانه value, منطقی explicitZero );

صحیح calcStructSize( ثابت نویسه* dt, صحیح initial_size );
صحیح calcElemSize( ثابت نویسه* dt, صحیح initial_size );
CV_DEPRECATED نویسه* encodeFormat( صحیح elem_type, نویسه* dt );
نویسه* encodeFormat( صحیح elem_type, نویسه* dt, size_t dt_len );
صحیح decodeFormat( ثابت نویسه* dt, صحیح* fmt_pairs, صحیح max_len );
صحیح decodeSimpleFormat( ثابت نویسه* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) انجامبده { (تهی)fs; abort(); } وقتی (0)
#وگرنه
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() انجامبده { \
    CV_DbgAssert(ptr); \
    اگر((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} وقتی (0)


رده FileStorageParser;
رده FileStorageEmitter;

ساختار FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( ثابت std::string& _struct_tag,
                 صحیح _struct_flags, صحیح _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    صحیح flags;
    صحیح indent;
};

رده FileStorage_API
{
عمومی:
    مجازی ~FileStorage_API();
    مجازی FileStorage* getFS() = 0;

    مجازی صحیح getFormat() ثابت = 0;

    مجازی تهی puts( ثابت نویسه* str ) = 0;
    مجازی نویسه* gets() = 0;
    مجازی منطقی eof() = 0;
    مجازی تهی setEof() = 0;
    مجازی تهی closeFile() = 0;
    مجازی تهی rewind() = 0;
    مجازی نویسه* resizeWriteBuffer( نویسه* ptr, صحیح len ) = 0;
    مجازی نویسه* bufferPtr() ثابت = 0;
    مجازی نویسه* bufferStart() ثابت = 0;
    مجازی نویسه* bufferEnd() ثابت = 0;
    مجازی تهی setBufferPtr(نویسه* ptr) = 0;
    مجازی نویسه* flush() = 0;
    مجازی تهی setNonEmpty() = 0;
    مجازی صحیح wrapMargin() ثابت = 0;

    مجازی FStructData& getCurrentStruct() = 0;

    مجازی تهی convertToCollection( صحیح type, File_گره& گره ) = 0;
    مجازی File_گره add_گره( File_گره& collection, ثابت std::string& کلید,
                               صحیح type, ثابت تهی* value=0, صحیح len=-1 ) = 0;
    مجازی تهی finalizeCollection( File_گره& collection ) = 0;
    مجازی دوگانه strtod(نویسه* ptr, نویسه** endptr) = 0;

    مجازی نویسه* parseBase64(نویسه* ptr, صحیح indent, File_گره& collection) = 0;
    CV_NORETURN
    مجازی تهی parseError(ثابت نویسه* funcname, ثابت std::string& پیام,
                            ثابت نویسه* filename, صحیح lineno) = 0;

خصوصی:
    شمارش Base64State{
        Uncertain,
        NotUse,
        InUse,
    };

    friend رده cv::FileStorage::Impl;
    friend رده cv::FileStorage;
    friend رده JSONEmitter;
    friend رده XMLEmitter;
    friend رده YAMLEmitter;

    مجازی تهی check_if_write_struct_is_delayed(منطقی change_type_to_base64 = نادرست) = 0;
    مجازی تهی switch_to_Base64_state(Base64State state) = 0;
    مجازی Base64State get_state_of_writing_base64() = 0;
    مجازی صحیح get_space() = 0;
};

رده FileStorageEmitter
{
عمومی:
    مجازی ~FileStorageEmitter() {}

    مجازی FStructData startWriteStruct( ثابت FStructData& parent, ثابت نویسه* کلید,
                                          صحیح struct_flags, ثابت نویسه* type_name=0 ) = 0;
    مجازی تهی endWriteStruct(ثابت FStructData& current_struct) = 0;
    مجازی تهی اكتب(ثابت نویسه* کلید, صحیح value) = 0;
    مجازی تهی اكتب(ثابت نویسه* کلید, int64_t value) = 0;
    مجازی تهی اكتب(ثابت نویسه* کلید, دوگانه value) = 0;
    مجازی تهی اكتب(ثابت نویسه* کلید, ثابت نویسه* value, منطقی quote) = 0;
    مجازی تهی writeScalar(ثابت نویسه* کلید, ثابت نویسه* value) = 0;
    مجازی تهی writeComment(ثابت نویسه* comment, منطقی eol_comment) = 0;
    مجازی تهی startNextStream() = 0;
};

رده FileStorageParser
{
عمومی:
    مجازی ~FileStorageParser() {}
    مجازی منطقی parse(نویسه* ptr) = 0;
    مجازی منطقی getBase64Row(نویسه* ptr, صحیح indent, نویسه* &beg, نویسه* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
