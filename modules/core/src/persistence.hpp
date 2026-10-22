// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_PERSISTENCE_HPP
#define SRC_PERSISTENCE_HPP

#অন্তর্ভাব <deque>
#অন্তর্ভাব <sstream>
#অন্তর্ভাব <string>
#অন্তর্ভাব <iterator>

#define USE_ZLIB 1
#যদি USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#অন্যথা
প্রকারনির্দেশ শূন্য* gzFile;
#endif

//=====================================================================================

স্থির ধ্রুব size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
স্থির ধ্রুব পূর্ণাঙ্ক CV_PERSISTENCE_MAX_DEPTH = 1024;

নামাকাশ base64 {

গণনা
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

অন্তঃস্থ তার্কিক cv_isalnum(বর্ণ c)
{
    প্রত্যাগম ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

অন্তঃস্থ তার্কিক cv_isalpha(বর্ণ c)
{
    প্রত্যাগম ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

অন্তঃস্থ তার্কিক cv_isdigit(বর্ণ c)
{
    প্রত্যাগম '0' <= c && c <= '9';
}

অন্তঃস্থ তার্কিক cv_isspace(বর্ণ c)
{
    প্রত্যাগম (9 <= c && c <= 13) || c == ' ';
}

অন্তঃস্থ বর্ণ* cv_skip_BOM(বর্ণ* ptr)
{
    যদি((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      প্রত্যাগম ptr + 3;
    }
    প্রত্যাগম ptr;
}

নামাকাশ cv
{
নামাকাশ fs
{
পূর্ণাঙ্ক strcasecmp(ধ্রুব বর্ণ* str1, ধ্রুব বর্ণ* str2);
বর্ণ* itoa( পূর্ণাঙ্ক _val, বর্ণ* buffer, পূর্ণাঙ্ক /*radix*/ );
বর্ণ* itoa( int64_t _val, বর্ণ* buffer, পূর্ণাঙ্ক /*radix*/, তার্কিক _signed );
বর্ণ* floatToString( বর্ণ* buf, size_t bufSize, ভিন্ন value, তার্কিক halfprecision, তার্কিক explicitZero );
বর্ণ* doubleToString( বর্ণ* buf, size_t bufSize, দ্বিগুণ value, তার্কিক explicitZero );

পূর্ণাঙ্ক calcStructSize( ধ্রুব বর্ণ* dt, পূর্ণাঙ্ক initial_size );
পূর্ণাঙ্ক calcElemSize( ধ্রুব বর্ণ* dt, পূর্ণাঙ্ক initial_size );
CV_DEPRECATED বর্ণ* encodeFormat( পূর্ণাঙ্ক elem_type, বর্ণ* dt );
বর্ণ* encodeFormat( পূর্ণাঙ্ক elem_type, বর্ণ* dt, size_t dt_len );
পূর্ণাঙ্ক decodeFormat( ধ্রুব বর্ণ* dt, পূর্ণাঙ্ক* fmt_pairs, পূর্ণাঙ্ক max_len );
পূর্ণাঙ্ক decodeSimpleFormat( ধ্রুব বর্ণ* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) কুরু { (শূন্য)fs; abort(); } যাবত্ (0)
#অন্যথা
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() কুরু { \
    CV_DbgAssert(ptr); \
    যদি((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} যাবত্ (0)


বর্গ FileStorageParser;
বর্গ FileStorageEmitter;

সংরচনা FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( ধ্রুব std::string& _struct_tag,
                 পূর্ণাঙ্ক _struct_flags, পূর্ণাঙ্ক _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    পূর্ণাঙ্ক flags;
    পূর্ণাঙ্ক indent;
};

বর্গ FileStorage_API
{
সার্বজনিক:
    আভাস ~FileStorage_API();
    আভাস FileStorage* getFS() = 0;

    আভাস পূর্ণাঙ্ক getFormat() ধ্রুব = 0;

    আভাস শূন্য puts( ধ্রুব বর্ণ* str ) = 0;
    আভাস বর্ণ* gets() = 0;
    আভাস তার্কিক eof() = 0;
    আভাস শূন্য setEof() = 0;
    আভাস শূন্য closeFile() = 0;
    আভাস শূন্য rewind() = 0;
    আভাস বর্ণ* resizeWriteBuffer( বর্ণ* ptr, পূর্ণাঙ্ক len ) = 0;
    আভাস বর্ণ* bufferPtr() ধ্রুব = 0;
    আভাস বর্ণ* bufferStart() ধ্রুব = 0;
    আভাস বর্ণ* bufferEnd() ধ্রুব = 0;
    আভাস শূন্য setBufferPtr(বর্ণ* ptr) = 0;
    আভাস বর্ণ* flush() = 0;
    আভাস শূন্য setNonEmpty() = 0;
    আভাস পূর্ণাঙ্ক wrapMargin() ধ্রুব = 0;

    আভাস FStructData& getCurrentStruct() = 0;

    আভাস শূন্য convertToCollection( পূর্ণাঙ্ক type, File_গ্রন্থি& গ্রন্থি ) = 0;
    আভাস File_গ্রন্থি add_গ্রন্থি( File_গ্রন্থি& collection, ধ্রুব std::string& চাবি,
                               পূর্ণাঙ্ক type, ধ্রুব শূন্য* value=0, পূর্ণাঙ্ক len=-1 ) = 0;
    আভাস শূন্য finalizeCollection( File_গ্রন্থি& collection ) = 0;
    আভাস দ্বিগুণ strtod(বর্ণ* ptr, বর্ণ** endptr) = 0;

    আভাস বর্ণ* parseBase64(বর্ণ* ptr, পূর্ণাঙ্ক indent, File_গ্রন্থি& collection) = 0;
    CV_NORETURN
    আভাস শূন্য parseError(ধ্রুব বর্ণ* funcname, ধ্রুব std::string& বার্তা,
                            ধ্রুব বর্ণ* filename, পূর্ণাঙ্ক lineno) = 0;

গুপ্ত:
    গণনা Base64State{
        Uncertain,
        NotUse,
        InUse,
    };

    friend বর্গ cv::FileStorage::Impl;
    friend বর্গ cv::FileStorage;
    friend বর্গ JSONEmitter;
    friend বর্গ XMLEmitter;
    friend বর্গ YAMLEmitter;

    আভাস শূন্য check_if_write_struct_is_delayed(তার্কিক change_type_to_base64 = অসত্য) = 0;
    আভাস শূন্য switch_to_Base64_state(Base64State state) = 0;
    আভাস Base64State get_state_of_writing_base64() = 0;
    আভাস পূর্ণাঙ্ক get_space() = 0;
};

বর্গ FileStorageEmitter
{
সার্বজনিক:
    আভাস ~FileStorageEmitter() {}

    আভাস FStructData startWriteStruct( ধ্রুব FStructData& parent, ধ্রুব বর্ণ* চাবি,
                                          পূর্ণাঙ্ক struct_flags, ধ্রুব বর্ণ* type_name=0 ) = 0;
    আভাস শূন্য endWriteStruct(ধ্রুব FStructData& current_struct) = 0;
    আভাস শূন্য লিখ(ধ্রুব বর্ণ* চাবি, পূর্ণাঙ্ক value) = 0;
    আভাস শূন্য লিখ(ধ্রুব বর্ণ* চাবি, int64_t value) = 0;
    আভাস শূন্য লিখ(ধ্রুব বর্ণ* চাবি, দ্বিগুণ value) = 0;
    আভাস শূন্য লিখ(ধ্রুব বর্ণ* চাবি, ধ্রুব বর্ণ* value, তার্কিক quote) = 0;
    আভাস শূন্য writeScalar(ধ্রুব বর্ণ* চাবি, ধ্রুব বর্ণ* value) = 0;
    আভাস শূন্য writeComment(ধ্রুব বর্ণ* comment, তার্কিক eol_comment) = 0;
    আভাস শূন্য startNextStream() = 0;
};

বর্গ FileStorageParser
{
সার্বজনিক:
    আভাস ~FileStorageParser() {}
    আভাস তার্কিক parse(বর্ণ* ptr) = 0;
    আভাস তার্কিক getBase64Row(বর্ণ* ptr, পূর্ণাঙ্ক indent, বর্ণ* &beg, বর্ণ* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
