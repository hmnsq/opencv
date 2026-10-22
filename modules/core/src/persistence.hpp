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
#ᰀᰧ USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#ᰀᰧᰀ
typedef void* gzFile;
#endif

//=====================================================================================

static const size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
static const puur_naa_nka CV_PERSISTENCE_MAX_DEPTH = 1024;

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
    ᰀᰧᰌ ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

anta_hstha bool cv_isalpha(var_na c)
{
    ᰀᰧᰌ ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

anta_hstha bool cv_isdigit(var_na c)
{
    ᰀᰧᰌ '0' <= c && c <= '9';
}

anta_hstha bool cv_isspace(var_na c)
{
    ᰀᰧᰌ (9 <= c && c <= 13) || c == ' ';
}

anta_hstha var_na* cv_skip_BOM(var_na* ptr)
{
    ᰀᰧ((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      ᰀᰧᰌ ptr + 3;
    }
    ᰀᰧᰌ ptr;
}

namespace cv
{
namespace fs
{
puur_naa_nka strcasecmp(const var_na* str1, const var_na* str2);
var_na* itoa( puur_naa_nka _val, var_na* buffer, puur_naa_nka /*radix*/ );
var_na* itoa( int64_t _val, var_na* buffer, puur_naa_nka /*radix*/, bool _signed );
var_na* floatToString( var_na* buf, size_t bufSize, float value, bool halfprecision, bool explicitZero );
var_na* doubleToString( var_na* buf, size_t bufSize, dviguu_na value, bool explicitZero );

puur_naa_nka calcStructSize( const var_na* dt, puur_naa_nka initial_size );
puur_naa_nka calcElemSize( const var_na* dt, puur_naa_nka initial_size );
CV_DEPRECATED var_na* encodeFormat( puur_naa_nka elem_type, var_na* dt );
var_na* encodeFormat( puur_naa_nka elem_type, var_na* dt, size_t dt_len );
puur_naa_nka decodeFormat( const var_na* dt, puur_naa_nka* fmt_pairs, puur_naa_nka max_len );
puur_naa_nka decodeSimpleFormat( const var_na* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) do { (void)fs; abort(); } ᰀᰬ (0)
#ᰀᰧᰀ
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() do { \
    CV_DbgAssert(ptr); \
    ᰀᰧ((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} ᰀᰬ (0)


class FileStorageParser;
class FileStorageEmitter;

sa_nracanaa FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( const std::string& _struct_tag,
                 puur_naa_nka _struct_flags, puur_naa_nka _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    puur_naa_nka flags;
    puur_naa_nka indent;
};

class FileStorage_API
{
public:
    virtual ~FileStorage_API();
    virtual FileStorage* getFS() = 0;

    virtual puur_naa_nka getFormat() const = 0;

    virtual void puts( const var_na* str ) = 0;
    virtual var_na* gets() = 0;
    virtual bool eof() = 0;
    virtual void setEof() = 0;
    virtual void closeFile() = 0;
    virtual void rewind() = 0;
    virtual var_na* resizeWriteBuffer( var_na* ptr, puur_naa_nka len ) = 0;
    virtual var_na* bufferPtr() const = 0;
    virtual var_na* bufferStart() const = 0;
    virtual var_na* bufferEnd() const = 0;
    virtual void setBufferPtr(var_na* ptr) = 0;
    virtual var_na* flush() = 0;
    virtual void setNonEmpty() = 0;
    virtual puur_naa_nka wrapMargin() const = 0;

    virtual FStructData& getCurrentStruct() = 0;

    virtual void convertToCollection( puur_naa_nka type, FileNode& node ) = 0;
    virtual FileNode addNode( FileNode& collection, const std::string& key,
                               puur_naa_nka type, const void* value=0, puur_naa_nka len=-1 ) = 0;
    virtual void finalizeCollection( FileNode& collection ) = 0;
    virtual dviguu_na strtod(var_na* ptr, var_na** endptr) = 0;

    virtual var_na* parseBase64(var_na* ptr, puur_naa_nka indent, FileNode& collection) = 0;
    CV_NORETURN
    virtual void parseError(const var_na* funcname, const std::string& msg,
                            const var_na* filename, puur_naa_nka lineno) = 0;

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
    virtual puur_naa_nka get_space() = 0;
};

class FileStorageEmitter
{
public:
    virtual ~FileStorageEmitter() {}

    virtual FStructData startWriteStruct( const FStructData& parent, const var_na* key,
                                          puur_naa_nka struct_flags, const var_na* type_name=0 ) = 0;
    virtual void endWriteStruct(const FStructData& current_struct) = 0;
    virtual void write(const var_na* key, puur_naa_nka value) = 0;
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
    virtual bool getBase64Row(var_na* ptr, puur_naa_nka indent, var_na* &beg, var_na* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
