// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_PERSISTENCE_HPP
#define SRC_PERSISTENCE_HPP

#অন্তর্ভাৱ <deque>
#অন্তর্ভাৱ <sstream>
#অন্তর্ভাৱ <string>
#অন্তর্ভাৱ <iterator>

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

স্থির ধ্রুৱ size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
স্থির ধ্রুৱ পূর্ণাঙ্ক CV_PERSISTENCE_MAX_DEPTH = 1024;

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

অন্তঃস্থ তার্কিক cv_isalnum(ৱর্ণ c)
{
    ঘূৰাই ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

অন্তঃস্থ তার্কিক cv_isalpha(ৱর্ণ c)
{
    ঘূৰাই ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

অন্তঃস্থ তার্কিক cv_isdigit(ৱর্ণ c)
{
    ঘূৰাই '0' <= c && c <= '9';
}

অন্তঃস্থ তার্কিক cv_isspace(ৱর্ণ c)
{
    ঘূৰাই (9 <= c && c <= 13) || c == ' ';
}

অন্তঃস্থ ৱর্ণ* cv_skip_BOM(ৱর্ণ* ptr)
{
    যদি((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      ঘূৰাই ptr + 3;
    }
    ঘূৰাই ptr;
}

নামাকাশ cv
{
নামাকাশ fs
{
পূর্ণাঙ্ক strcasecmp(ধ্রুৱ ৱর্ণ* str1, ধ্রুৱ ৱর্ণ* str2);
ৱর্ণ* itoa( পূর্ণাঙ্ক _val, ৱর্ণ* buffer, পূর্ণাঙ্ক /*radix*/ );
ৱর্ণ* itoa( int64_t _val, ৱর্ণ* buffer, পূর্ণাঙ্ক /*radix*/, তার্কিক _signed );
ৱর্ণ* floatToString( ৱর্ণ* buf, size_t bufSize, ভিন্ন value, তার্কিক halfprecision, তার্কিক explicitZero );
ৱর্ণ* doubleToString( ৱর্ণ* buf, size_t bufSize, দ্ৱিগুণ value, তার্কিক explicitZero );

পূর্ণাঙ্ক calcStructSize( ধ্রুৱ ৱর্ণ* dt, পূর্ণাঙ্ক initial_size );
পূর্ণাঙ্ক calcElemSize( ধ্রুৱ ৱর্ণ* dt, পূর্ণাঙ্ক initial_size );
CV_DEPRECATED ৱর্ণ* encodeFormat( পূর্ণাঙ্ক elem_type, ৱর্ণ* dt );
ৱর্ণ* encodeFormat( পূর্ণাঙ্ক elem_type, ৱর্ণ* dt, size_t dt_len );
পূর্ণাঙ্ক decodeFormat( ধ্রুৱ ৱর্ণ* dt, পূর্ণাঙ্ক* fmt_pairs, পূর্ণাঙ্ক max_len );
পূর্ণাঙ্ক decodeSimpleFormat( ধ্রুৱ ৱর্ণ* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) কৰা { (শূন্য)fs; abort(); } যেতিয়ালৈ (0)
#অন্যথা
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() কৰা { \
    CV_DbgAssert(ptr); \
    যদি((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} যেতিয়ালৈ (0)


শ্ৰেণী FileStorageParser;
শ্ৰেণী FileStorageEmitter;

সংরচনা FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( ধ্রুৱ std::string& _struct_tag,
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

শ্ৰেণী FileStorage_API
{
সার্ৱজনিক:
    আভাস ~FileStorage_API();
    আভাস FileStorage* getFS() = 0;

    আভাস পূর্ণাঙ্ক getFormat() ধ্রুৱ = 0;

    আভাস শূন্য puts( ধ্রুৱ ৱর্ণ* str ) = 0;
    আভাস ৱর্ণ* gets() = 0;
    আভাস তার্কিক eof() = 0;
    আভাস শূন্য setEof() = 0;
    আভাস শূন্য closeFile() = 0;
    আভাস শূন্য rewind() = 0;
    আভাস ৱর্ণ* resizeWriteBuffer( ৱর্ণ* ptr, পূর্ণাঙ্ক len ) = 0;
    আভাস ৱর্ণ* bufferPtr() ধ্রুৱ = 0;
    আভাস ৱর্ণ* bufferStart() ধ্রুৱ = 0;
    আভাস ৱর্ণ* bufferEnd() ধ্রুৱ = 0;
    আভাস শূন্য setBufferPtr(ৱর্ণ* ptr) = 0;
    আভাস ৱর্ণ* flush() = 0;
    আভাস শূন্য setNonEmpty() = 0;
    আভাস পূর্ণাঙ্ক wrapMargin() ধ্রুৱ = 0;

    আভাস FStructData& getCurrentStruct() = 0;

    আভাস শূন্য convertToCollection( পূর্ণাঙ্ক type, File_গ্ৰন্থি& গ্ৰন্থি ) = 0;
    আভাস File_গ্ৰন্থি add_গ্ৰন্থি( File_গ্ৰন্থি& collection, ধ্রুৱ std::string& চাবি,
                               পূর্ণাঙ্ক type, ধ্রুৱ শূন্য* value=0, পূর্ণাঙ্ক len=-1 ) = 0;
    আভাস শূন্য finalizeCollection( File_গ্ৰন্থি& collection ) = 0;
    আভাস দ্ৱিগুণ strtod(ৱর্ণ* ptr, ৱর্ণ** endptr) = 0;

    আভাস ৱর্ণ* parseBase64(ৱর্ণ* ptr, পূর্ণাঙ্ক indent, File_গ্ৰন্থি& collection) = 0;
    CV_NORETURN
    আভাস শূন্য parseError(ধ্রুৱ ৱর্ণ* funcname, ধ্রুৱ std::string& বাৰ্তা,
                            ধ্রুৱ ৱর্ণ* filename, পূর্ণাঙ্ক lineno) = 0;

গুপ্ত:
    গণনা Base64State{
        Uncertain,
        NotUse,
        InUse,
    };

    friend শ্ৰেণী cv::FileStorage::Impl;
    friend শ্ৰেণী cv::FileStorage;
    friend শ্ৰেণী JSONEmitter;
    friend শ্ৰেণী XMLEmitter;
    friend শ্ৰেণী YAMLEmitter;

    আভাস শূন্য check_if_write_struct_is_delayed(তার্কিক change_type_to_base64 = মিছা) = 0;
    আভাস শূন্য switch_to_Base64_state(Base64State state) = 0;
    আভাস Base64State get_state_of_writing_base64() = 0;
    আভাস পূর্ণাঙ্ক get_space() = 0;
};

শ্ৰেণী FileStorageEmitter
{
সার্ৱজনিক:
    আভাস ~FileStorageEmitter() {}

    আভাস FStructData startWriteStruct( ধ্রুৱ FStructData& parent, ধ্রুৱ ৱর্ণ* চাবি,
                                          পূর্ণাঙ্ক struct_flags, ধ্রুৱ ৱর্ণ* type_name=0 ) = 0;
    আভাস শূন্য endWriteStruct(ধ্রুৱ FStructData& current_struct) = 0;
    আভাস শূন্য লিখ(ধ্রুৱ ৱর্ণ* চাবি, পূর্ণাঙ্ক value) = 0;
    আভাস শূন্য লিখ(ধ্রুৱ ৱর্ণ* চাবি, int64_t value) = 0;
    আভাস শূন্য লিখ(ধ্রুৱ ৱর্ণ* চাবি, দ্ৱিগুণ value) = 0;
    আভাস শূন্য লিখ(ধ্রুৱ ৱর্ণ* চাবি, ধ্রুৱ ৱর্ণ* value, তার্কিক quote) = 0;
    আভাস শূন্য writeScalar(ধ্রুৱ ৱর্ণ* চাবি, ধ্রুৱ ৱর্ণ* value) = 0;
    আভাস শূন্য writeComment(ধ্রুৱ ৱর্ণ* comment, তার্কিক eol_comment) = 0;
    আভাস শূন্য startNextStream() = 0;
};

শ্ৰেণী FileStorageParser
{
সার্ৱজনিক:
    আভাস ~FileStorageParser() {}
    আভাস তার্কিক parse(ৱর্ণ* ptr) = 0;
    আভাস তার্কিক getBase64Row(ৱর্ণ* ptr, পূর্ণাঙ্ক indent, ৱর্ণ* &beg, ৱর্ণ* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
