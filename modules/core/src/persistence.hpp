// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_PERSISTENCE_HPP
#define SRC_PERSISTENCE_HPP

#ضمن <deque>
#ضمن <sstream>
#ضمن <string>
#ضمن <iterator>

#define USE_ZLIB 1
#אם USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#אחרת
تعريفنوع فراغ* gzFile;
#endif

//=====================================================================================

ساكن ثابت size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
ساكن ثابت صحيح CV_PERSISTENCE_MAX_DEPTH = 1024;

نطاق base64 {

تعداد
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

مضمن منطقي cv_isalnum(حرف c)
{
    החזר ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

مضمن منطقي cv_isalpha(حرف c)
{
    החזר ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

مضمن منطقي cv_isdigit(حرف c)
{
    החזר '0' <= c && c <= '9';
}

مضمن منطقي cv_isspace(حرف c)
{
    החזר (9 <= c && c <= 13) || c == ' ';
}

مضمن حرف* cv_skip_BOM(حرف* ptr)
{
    אם((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      החזר ptr + 3;
    }
    החזר ptr;
}

نطاق cv
{
نطاق fs
{
صحيح strcasecmp(ثابت حرف* str1, ثابت حرف* str2);
حرف* itoa( صحيح _val, حرف* buffer, صحيح /*radix*/ );
حرف* itoa( int64_t _val, حرف* buffer, صحيح /*radix*/, منطقي _signed );
حرف* floatToString( حرف* buf, size_t bufSize, عائم value, منطقي halfprecision, منطقي explicitZero );
حرف* doubleToString( حرف* buf, size_t bufSize, مزدوج value, منطقي explicitZero );

صحيح calcStructSize( ثابت حرف* dt, صحيح initial_size );
صحيح calcElemSize( ثابت حرف* dt, صحيح initial_size );
CV_DEPRECATED حرف* encodeFormat( صحيح elem_type, حرف* dt );
حرف* encodeFormat( صحيح elem_type, حرف* dt, size_t dt_len );
صحيح decodeFormat( ثابت حرف* dt, صحيح* fmt_pairs, صحيح max_len );
صحيح decodeSimpleFormat( ثابت حرف* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) עשה { (فراغ)fs; abort(); } כלעוד (0)
#אחרת
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() עשה { \
    CV_DbgAssert(ptr); \
    אם((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} כלעוד (0)


מחלקה FileStorageParser;
מחלקה FileStorageEmitter;

بنية FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( ثابت std::string& _struct_tag,
                 صحيح _struct_flags, صحيح _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    صحيح flags;
    صحيح indent;
};

מחלקה FileStorage_API
{
عام:
    virtual ~FileStorage_API();
    virtual FileStorage* getFS() = 0;

    virtual صحيح getFormat() ثابت = 0;

    virtual فراغ puts( ثابت حرف* str ) = 0;
    virtual حرف* gets() = 0;
    virtual منطقي eof() = 0;
    virtual فراغ setEof() = 0;
    virtual فراغ closeFile() = 0;
    virtual فراغ rewind() = 0;
    virtual حرف* resizeWriteBuffer( حرف* ptr, صحيح len ) = 0;
    virtual حرف* bufferPtr() ثابت = 0;
    virtual حرف* bufferStart() ثابت = 0;
    virtual حرف* bufferEnd() ثابت = 0;
    virtual فراغ setBufferPtr(حرف* ptr) = 0;
    virtual حرف* flush() = 0;
    virtual فراغ setNonEmpty() = 0;
    virtual صحيح wrapMargin() ثابت = 0;

    virtual FStructData& getCurrentStruct() = 0;

    virtual فراغ convertToCollection( صحيح type, FileNode& node ) = 0;
    virtual FileNode addNode( FileNode& collection, ثابت std::string& key,
                               صحيح type, ثابت فراغ* value=0, صحيح len=-1 ) = 0;
    virtual فراغ finalizeCollection( FileNode& collection ) = 0;
    virtual مزدوج strtod(حرف* ptr, حرف** endptr) = 0;

    virtual حرف* parseBase64(حرف* ptr, صحيح indent, FileNode& collection) = 0;
    CV_NORETURN
    virtual فراغ parseError(ثابت حرف* funcname, ثابت std::string& msg,
                            ثابت حرف* filename, صحيح lineno) = 0;

خاص:
    تعداد Base64State{
        Uncertain,
        NotUse,
        InUse,
    };

    friend מחלקה cv::FileStorage::Impl;
    friend מחלקה cv::FileStorage;
    friend מחלקה JSONEmitter;
    friend מחלקה XMLEmitter;
    friend מחלקה YAMLEmitter;

    virtual فراغ check_if_write_struct_is_delayed(منطقي change_type_to_base64 = שקר) = 0;
    virtual فراغ switch_to_Base64_state(Base64State state) = 0;
    virtual Base64State get_state_of_writing_base64() = 0;
    virtual صحيح get_space() = 0;
};

מחלקה FileStorageEmitter
{
عام:
    virtual ~FileStorageEmitter() {}

    virtual FStructData startWriteStruct( ثابت FStructData& parent, ثابت حرف* key,
                                          صحيح struct_flags, ثابت حرف* type_name=0 ) = 0;
    virtual فراغ endWriteStruct(ثابت FStructData& current_struct) = 0;
    virtual فراغ اكتب(ثابت حرف* key, صحيح value) = 0;
    virtual فراغ اكتب(ثابت حرف* key, int64_t value) = 0;
    virtual فراغ اكتب(ثابت حرف* key, مزدوج value) = 0;
    virtual فراغ اكتب(ثابت حرف* key, ثابت حرف* value, منطقي quote) = 0;
    virtual فراغ writeScalar(ثابت حرف* key, ثابت حرف* value) = 0;
    virtual فراغ writeComment(ثابت حرف* comment, منطقي eol_comment) = 0;
    virtual فراغ startNextStream() = 0;
};

מחלקה FileStorageParser
{
عام:
    virtual ~FileStorageParser() {}
    virtual منطقي parse(حرف* ptr) = 0;
    virtual منطقي getBase64Row(حرف* ptr, صحيح indent, حرف* &beg, حرف* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
