// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_PERSISTENCE_HPP
#define SRC_PERSISTENCE_HPP

#include <deque>
#include <sstream>
#include <string>
#include <iterator>

#define USE_ZLIB 1
#အကယ်၍ USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#မဟုတ်ပါက
typedef void* gzFile;
#endif

//=====================================================================================

static const size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
static const ကိန်းပြည့် CV_PERSISTENCE_MAX_DEPTH = 1024;

namespace base64 {

ga_nanaa
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

anta_hstha bool cv_isalnum(var_na c)
{
    ပြန်ပေး ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

anta_hstha bool cv_isalpha(var_na c)
{
    ပြန်ပေး ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

anta_hstha bool cv_isdigit(var_na c)
{
    ပြန်ပေး '0' <= c && c <= '9';
}

anta_hstha bool cv_isspace(var_na c)
{
    ပြန်ပေး (9 <= c && c <= 13) || c == ' ';
}

anta_hstha var_na* cv_skip_BOM(var_na* ptr)
{
    အကယ်၍((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      ပြန်ပေး ptr + 3;
    }
    ပြန်ပေး ptr;
}

namespace cv
{
namespace fs
{
ကိန်းပြည့် strcasecmp(const var_na* str1, const var_na* str2);
var_na* itoa( ကိန်းပြည့် _val, var_na* buffer, ကိန်းပြည့် /*radix*/ );
var_na* itoa( int64_t _val, var_na* buffer, ကိန်းပြည့် /*radix*/, bool _signed );
var_na* floatToString( var_na* buf, size_t bufSize, float value, bool halfprecision, bool explicitZero );
var_na* doubleToString( var_na* buf, size_t bufSize, dviguu_na value, bool explicitZero );

ကိန်းပြည့် calcStructSize( const var_na* dt, ကိန်းပြည့် initial_size );
ကိန်းပြည့် calcElemSize( const var_na* dt, ကိန်းပြည့် initial_size );
CV_DEPRECATED var_na* encodeFormat( ကိန်းပြည့် elem_type, var_na* dt );
var_na* encodeFormat( ကိန်းပြည့် elem_type, var_na* dt, size_t dt_len );
ကိန်းပြည့် decodeFormat( const var_na* dt, ကိန်းပြည့်* fmt_pairs, ကိန်းပြည့် max_len );
ကိန်းပြည့် decodeSimpleFormat( const var_na* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) do { (void)fs; abort(); } နေစဉ် (0)
#မဟုတ်ပါက
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() do { \
    CV_DbgAssert(ptr); \
    အကယ်၍((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} နေစဉ် (0)


class FileStorageParser;
class FileStorageEmitter;

sa_nracanaa FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( const std::string& _struct_tag,
                 ကိန်းပြည့် _struct_flags, ကိန်းပြည့် _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    ကိန်းပြည့် flags;
    ကိန်းပြည့် indent;
};

class FileStorage_API
{
public:
    virtual ~FileStorage_API();
    virtual FileStorage* getFS() = 0;

    virtual ကိန်းပြည့် getFormat() const = 0;

    virtual void puts( const var_na* str ) = 0;
    virtual var_na* gets() = 0;
    virtual bool eof() = 0;
    virtual void setEof() = 0;
    virtual void closeFile() = 0;
    virtual void rewind() = 0;
    virtual var_na* resizeWriteBuffer( var_na* ptr, ကိန်းပြည့် len ) = 0;
    virtual var_na* bufferPtr() const = 0;
    virtual var_na* bufferStart() const = 0;
    virtual var_na* bufferEnd() const = 0;
    virtual void setBufferPtr(var_na* ptr) = 0;
    virtual var_na* flush() = 0;
    virtual void setNonEmpty() = 0;
    virtual ကိန်းပြည့် wrapMargin() const = 0;

    virtual FStructData& getCurrentStruct() = 0;

    virtual void convertToCollection( ကိန်းပြည့် type, FileNode& node ) = 0;
    virtual FileNode addNode( FileNode& collection, const std::string& key,
                               ကိန်းပြည့် type, const void* value=0, ကိန်းပြည့် len=-1 ) = 0;
    virtual void finalizeCollection( FileNode& collection ) = 0;
    virtual dviguu_na strtod(var_na* ptr, var_na** endptr) = 0;

    virtual var_na* parseBase64(var_na* ptr, ကိန်းပြည့် indent, FileNode& collection) = 0;
    CV_NORETURN
    virtual void parseError(const var_na* funcname, const std::string& msg,
                            const var_na* filename, ကိန်းပြည့် lineno) = 0;

private:
    ga_nanaa Base64State{
        Uncertain,
        NotUse,
        InUse,
    };

    friend class cv::FileStorage::Impl;
    friend class cv::FileStorage;
    friend class JSONEmitter;
    friend class XMLEmitter;
    friend class YAMLEmitter;

    virtual void check_if_write_struct_is_delayed(bool change_type_to_base64 = false) = 0;
    virtual void switch_to_Base64_state(Base64State state) = 0;
    virtual Base64State get_state_of_writing_base64() = 0;
    virtual ကိန်းပြည့် get_space() = 0;
};

class FileStorageEmitter
{
public:
    virtual ~FileStorageEmitter() {}

    virtual FStructData startWriteStruct( const FStructData& parent, const var_na* key,
                                          ကိန်းပြည့် struct_flags, const var_na* type_name=0 ) = 0;
    virtual void endWriteStruct(const FStructData& current_struct) = 0;
    virtual void write(const var_na* key, ကိန်းပြည့် value) = 0;
    virtual void write(const var_na* key, int64_t value) = 0;
    virtual void write(const var_na* key, dviguu_na value) = 0;
    virtual void write(const var_na* key, const var_na* value, bool quote) = 0;
    virtual void writeScalar(const var_na* key, const var_na* value) = 0;
    virtual void writeComment(const var_na* comment, bool eol_comment) = 0;
    virtual void startNextStream() = 0;
};

class FileStorageParser
{
public:
    virtual ~FileStorageParser() {}
    virtual bool parse(var_na* ptr) = 0;
    virtual bool getBase64Row(var_na* ptr, ကိန်းပြည့် indent, var_na* &beg, var_na* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
