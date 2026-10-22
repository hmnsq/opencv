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
#නම් USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#නැතහොත්
typedef void* gzFile;
#endif

//=====================================================================================

static const size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
static const පූර්ණ_සංඛ්‍යා CV_PERSISTENCE_MAX_DEPTH = 1024;

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
    ආපසු ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

anta_hstha bool cv_isalpha(var_na c)
{
    ආපසු ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

anta_hstha bool cv_isdigit(var_na c)
{
    ආපසු '0' <= c && c <= '9';
}

anta_hstha bool cv_isspace(var_na c)
{
    ආපසු (9 <= c && c <= 13) || c == ' ';
}

anta_hstha var_na* cv_skip_BOM(var_na* ptr)
{
    නම්((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      ආපසු ptr + 3;
    }
    ආපසු ptr;
}

namespace cv
{
namespace fs
{
පූර්ණ_සංඛ්‍යා strcasecmp(const var_na* str1, const var_na* str2);
var_na* itoa( පූර්ණ_සංඛ්‍යා _val, var_na* buffer, පූර්ණ_සංඛ්‍යා /*radix*/ );
var_na* itoa( int64_t _val, var_na* buffer, පූර්ණ_සංඛ්‍යා /*radix*/, bool _signed );
var_na* floatToString( var_na* buf, size_t bufSize, float value, bool halfprecision, bool explicitZero );
var_na* doubleToString( var_na* buf, size_t bufSize, dviguu_na value, bool explicitZero );

පූර්ණ_සංඛ්‍යා calcStructSize( const var_na* dt, පූර්ණ_සංඛ්‍යා initial_size );
පූර්ණ_සංඛ්‍යා calcElemSize( const var_na* dt, පූර්ණ_සංඛ්‍යා initial_size );
CV_DEPRECATED var_na* encodeFormat( පූර්ණ_සංඛ්‍යා elem_type, var_na* dt );
var_na* encodeFormat( පූර්ණ_සංඛ්‍යා elem_type, var_na* dt, size_t dt_len );
පූර්ණ_සංඛ්‍යා decodeFormat( const var_na* dt, පූර්ණ_සංඛ්‍යා* fmt_pairs, පූර්ණ_සංඛ්‍යා max_len );
පූර්ණ_සංඛ්‍යා decodeSimpleFormat( const var_na* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) do { (void)fs; abort(); } අතරතුර (0)
#නැතහොත්
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() do { \
    CV_DbgAssert(ptr); \
    නම්((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} අතරතුර (0)


class FileStorageParser;
class FileStorageEmitter;

sa_nracanaa FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( const std::string& _struct_tag,
                 පූර්ණ_සංඛ්‍යා _struct_flags, පූර්ණ_සංඛ්‍යා _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    පූර්ණ_සංඛ්‍යා flags;
    පූර්ණ_සංඛ්‍යා indent;
};

class FileStorage_API
{
public:
    virtual ~FileStorage_API();
    virtual FileStorage* getFS() = 0;

    virtual පූර්ණ_සංඛ්‍යා getFormat() const = 0;

    virtual void puts( const var_na* str ) = 0;
    virtual var_na* gets() = 0;
    virtual bool eof() = 0;
    virtual void setEof() = 0;
    virtual void closeFile() = 0;
    virtual void rewind() = 0;
    virtual var_na* resizeWriteBuffer( var_na* ptr, පූර්ණ_සංඛ්‍යා len ) = 0;
    virtual var_na* bufferPtr() const = 0;
    virtual var_na* bufferStart() const = 0;
    virtual var_na* bufferEnd() const = 0;
    virtual void setBufferPtr(var_na* ptr) = 0;
    virtual var_na* flush() = 0;
    virtual void setNonEmpty() = 0;
    virtual පූර්ණ_සංඛ්‍යා wrapMargin() const = 0;

    virtual FStructData& getCurrentStruct() = 0;

    virtual void convertToCollection( පූර්ණ_සංඛ්‍යා type, FileNode& node ) = 0;
    virtual FileNode addNode( FileNode& collection, const std::string& key,
                               පූර්ණ_සංඛ්‍යා type, const void* value=0, පූර්ණ_සංඛ්‍යා len=-1 ) = 0;
    virtual void finalizeCollection( FileNode& collection ) = 0;
    virtual dviguu_na strtod(var_na* ptr, var_na** endptr) = 0;

    virtual var_na* parseBase64(var_na* ptr, පූර්ණ_සංඛ්‍යා indent, FileNode& collection) = 0;
    CV_NORETURN
    virtual void parseError(const var_na* funcname, const std::string& msg,
                            const var_na* filename, පූර්ණ_සංඛ්‍යා lineno) = 0;

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
    virtual පූර්ණ_සංඛ්‍යා get_space() = 0;
};

class FileStorageEmitter
{
public:
    virtual ~FileStorageEmitter() {}

    virtual FStructData startWriteStruct( const FStructData& parent, const var_na* key,
                                          පූර්ණ_සංඛ්‍යා struct_flags, const var_na* type_name=0 ) = 0;
    virtual void endWriteStruct(const FStructData& current_struct) = 0;
    virtual void write(const var_na* key, පූර්ණ_සංඛ්‍යා value) = 0;
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
    virtual bool getBase64Row(var_na* ptr, පූර්ණ_සංඛ්‍යා indent, var_na* &beg, var_na* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
