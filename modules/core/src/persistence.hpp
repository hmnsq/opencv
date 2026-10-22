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
#якщо USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#інакше
typedef void* gzFile;
#endif

//=====================================================================================

static const size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
static const ціле CV_PERSISTENCE_MAX_DEPTH = 1024;

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
    повернути ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

inline bool cv_isalpha(char c)
{
    повернути ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

inline bool cv_isdigit(char c)
{
    повернути '0' <= c && c <= '9';
}

inline bool cv_isspace(char c)
{
    повернути (9 <= c && c <= 13) || c == ' ';
}

inline char* cv_skip_BOM(char* ptr)
{
    якщо((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      повернути ptr + 3;
    }
    повернути ptr;
}

namespace cv
{
namespace fs
{
ціле strcasecmp(const char* str1, const char* str2);
char* itoa( ціле _val, char* buffer, ціле /*radix*/ );
char* itoa( int64_t _val, char* buffer, ціле /*radix*/, bool _signed );
char* floatToString( char* buf, size_t bufSize, float value, bool halfprecision, bool explicitZero );
char* doubleToString( char* buf, size_t bufSize, double value, bool explicitZero );

ціле calcStructSize( const char* dt, ціле initial_size );
ціле calcElemSize( const char* dt, ціле initial_size );
CV_DEPRECATED char* encodeFormat( ціле elem_type, char* dt );
char* encodeFormat( ціле elem_type, char* dt, size_t dt_len );
ціле decodeFormat( const char* dt, ціле* fmt_pairs, ціле max_len );
ціле decodeSimpleFormat( const char* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) do { (void)fs; abort(); } поки (0)
#інакше
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() do { \
    CV_DbgAssert(ptr); \
    якщо((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} поки (0)


class FileStorageParser;
class FileStorageEmitter;

struct FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( const std::string& _struct_tag,
                 ціле _struct_flags, ціле _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    ціле flags;
    ціле indent;
};

class FileStorage_API
{
public:
    virtual ~FileStorage_API();
    virtual FileStorage* getFS() = 0;

    virtual ціле getFormat() const = 0;

    virtual void puts( const char* str ) = 0;
    virtual char* gets() = 0;
    virtual bool eof() = 0;
    virtual void setEof() = 0;
    virtual void closeFile() = 0;
    virtual void rewind() = 0;
    virtual char* resizeWriteBuffer( char* ptr, ціле len ) = 0;
    virtual char* bufferPtr() const = 0;
    virtual char* bufferStart() const = 0;
    virtual char* bufferEnd() const = 0;
    virtual void setBufferPtr(char* ptr) = 0;
    virtual char* flush() = 0;
    virtual void setNonEmpty() = 0;
    virtual ціле wrapMargin() const = 0;

    virtual FStructData& getCurrentStruct() = 0;

    virtual void convertToCollection( ціле type, FileNode& node ) = 0;
    virtual FileNode addNode( FileNode& collection, const std::string& key,
                               ціле type, const void* value=0, ціле len=-1 ) = 0;
    virtual void finalizeCollection( FileNode& collection ) = 0;
    virtual double strtod(char* ptr, char** endptr) = 0;

    virtual char* parseBase64(char* ptr, ціле indent, FileNode& collection) = 0;
    CV_NORETURN
    virtual void parseError(const char* funcname, const std::string& msg,
                            const char* filename, ціле lineno) = 0;

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
    virtual ціле get_space() = 0;
};

class FileStorageEmitter
{
public:
    virtual ~FileStorageEmitter() {}

    virtual FStructData startWriteStruct( const FStructData& parent, const char* key,
                                          ціле struct_flags, const char* type_name=0 ) = 0;
    virtual void endWriteStruct(const FStructData& current_struct) = 0;
    virtual void write(const char* key, ціле value) = 0;
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
    virtual bool getBase64Row(char* ptr, ціле indent, char* &beg, char* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
