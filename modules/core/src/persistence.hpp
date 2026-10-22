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
#如果 USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#否则
typedef void* gzFile;
#endif

//=====================================================================================

static const size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
static const 整数 CV_PERSISTENCE_MAX_DEPTH = 1024;

namespace base64 {

enum
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

inline bool cv_isalnum(char c)
{
    返回 ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

inline bool cv_isalpha(char c)
{
    返回 ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

inline bool cv_isdigit(char c)
{
    返回 '0' <= c && c <= '9';
}

inline bool cv_isspace(char c)
{
    返回 (9 <= c && c <= 13) || c == ' ';
}

inline char* cv_skip_BOM(char* ptr)
{
    如果((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      返回 ptr + 3;
    }
    返回 ptr;
}

namespace cv
{
namespace fs
{
整数 strcasecmp(const char* str1, const char* str2);
char* itoa( 整数 _val, char* buffer, 整数 /*radix*/ );
char* itoa( int64_t _val, char* buffer, 整数 /*radix*/, bool _signed );
char* floatToString( char* buf, size_t bufSize, float value, bool halfprecision, bool explicitZero );
char* doubleToString( char* buf, size_t bufSize, double value, bool explicitZero );

整数 calcStructSize( const char* dt, 整数 initial_size );
整数 calcElemSize( const char* dt, 整数 initial_size );
CV_DEPRECATED char* encodeFormat( 整数 elem_type, char* dt );
char* encodeFormat( 整数 elem_type, char* dt, size_t dt_len );
整数 decodeFormat( const char* dt, 整数* fmt_pairs, 整数 max_len );
整数 decodeSimpleFormat( const char* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) do { (void)fs; abort(); } 当 (0)
#否则
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() do { \
    CV_DbgAssert(ptr); \
    如果((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} 当 (0)


class FileStorageParser;
class FileStorageEmitter;

struct FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( const std::string& _struct_tag,
                 整数 _struct_flags, 整数 _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    整数 flags;
    整数 indent;
};

class FileStorage_API
{
public:
    virtual ~FileStorage_API();
    virtual FileStorage* getFS() = 0;

    virtual 整数 getFormat() const = 0;

    virtual void puts( const char* str ) = 0;
    virtual char* gets() = 0;
    virtual bool eof() = 0;
    virtual void setEof() = 0;
    virtual void closeFile() = 0;
    virtual void rewind() = 0;
    virtual char* resizeWriteBuffer( char* ptr, 整数 len ) = 0;
    virtual char* bufferPtr() const = 0;
    virtual char* bufferStart() const = 0;
    virtual char* bufferEnd() const = 0;
    virtual void setBufferPtr(char* ptr) = 0;
    virtual char* flush() = 0;
    virtual void setNonEmpty() = 0;
    virtual 整数 wrapMargin() const = 0;

    virtual FStructData& getCurrentStruct() = 0;

    virtual void convertToCollection( 整数 type, FileNode& node ) = 0;
    virtual FileNode addNode( FileNode& collection, const std::string& key,
                               整数 type, const void* value=0, 整数 len=-1 ) = 0;
    virtual void finalizeCollection( FileNode& collection ) = 0;
    virtual double strtod(char* ptr, char** endptr) = 0;

    virtual char* parseBase64(char* ptr, 整数 indent, FileNode& collection) = 0;
    CV_NORETURN
    virtual void parseError(const char* funcname, const std::string& msg,
                            const char* filename, 整数 lineno) = 0;

private:
    enum Base64State{
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
    virtual 整数 get_space() = 0;
};

class FileStorageEmitter
{
public:
    virtual ~FileStorageEmitter() {}

    virtual FStructData startWriteStruct( const FStructData& parent, const char* key,
                                          整数 struct_flags, const char* type_name=0 ) = 0;
    virtual void endWriteStruct(const FStructData& current_struct) = 0;
    virtual void write(const char* key, 整数 value) = 0;
    virtual void write(const char* key, int64_t value) = 0;
    virtual void write(const char* key, double value) = 0;
    virtual void write(const char* key, const char* value, bool quote) = 0;
    virtual void writeScalar(const char* key, const char* value) = 0;
    virtual void writeComment(const char* comment, bool eol_comment) = 0;
    virtual void startNextStream() = 0;
};

class FileStorageParser
{
public:
    virtual ~FileStorageParser() {}
    virtual bool parse(char* ptr) = 0;
    virtual bool getBase64Row(char* ptr, 整数 indent, char* &beg, char* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
