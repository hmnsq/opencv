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
প্রকারলেখ শূন্য* gzFile;
#endif

//=====================================================================================

স্থির ধ্রুবক size_t PARSER_BASE64_বাফার_আকার = 1024U * 1024U / 8U;
স্থির ধ্রুবক পূর্ণসংখ্যা CV_PERSISTENCE_MAX_DEPTH = 1024;

নামাকাশ base64 {

গণনা
{
    HEADER_আকার         = 24,
    ENCODED_HEADER_আকার = 32
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

অন্তঃরেখ যৌক্তিক cv_isalnum(অক্ষর c)
{
    ফেরত ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

অন্তঃরেখ যৌক্তিক cv_isalpha(অক্ষর c)
{
    ফেরত ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

অন্তঃরেখ যৌক্তিক cv_isdigit(অক্ষর c)
{
    ফেরত '0' <= c && c <= '9';
}

অন্তঃরেখ যৌক্তিক cv_isspace(অক্ষর c)
{
    ফেরত (9 <= c && c <= 13) || c == ' ';
}

অন্তঃরেখ অক্ষর* cv_skip_BOM(অক্ষর* ptr)
{
    যদি((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      ফেরত ptr + 3;
    }
    ফেরত ptr;
}

নামাকাশ cv
{
নামাকাশ fs
{
পূর্ণসংখ্যা strcasecmp(ধ্রুবক অক্ষর* str1, ধ্রুবক অক্ষর* str2);
অক্ষর* itoa( পূর্ণসংখ্যা _val, অক্ষর* বাফার, পূর্ণসংখ্যা /*radix*/ );
অক্ষর* itoa( int64_t _val, অক্ষর* বাফার, পূর্ণসংখ্যা /*radix*/, যৌক্তিক _signed );
অক্ষর* floatToString( অক্ষর* buf, size_t buf_আকার, দশমিক value, যৌক্তিক halfprecision, যৌক্তিক explicitZero );
অক্ষর* doubleToString( অক্ষর* buf, size_t buf_আকার, দ্বিগুণ value, যৌক্তিক explicitZero );

পূর্ণসংখ্যা calc_Struct_আকার( ধ্রুবক অক্ষর* dt, পূর্ণসংখ্যা initial_আকার );
পূর্ণসংখ্যা calc_Elem_আকার( ধ্রুবক অক্ষর* dt, পূর্ণসংখ্যা initial_আকার );
CV_DEPRECATED অক্ষর* encodeFormat( পূর্ণসংখ্যা elem_type, অক্ষর* dt );
অক্ষর* encodeFormat( পূর্ণসংখ্যা elem_type, অক্ষর* dt, size_t dt_len );
পূর্ণসংখ্যা decodeFormat( ধ্রুবক অক্ষর* dt, পূর্ণসংখ্যা* fmt_pairs, পূর্ণসংখ্যা max_len );
পূর্ণসংখ্যা decodeSimpleFormat( ধ্রুবক অক্ষর* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) করো { (শূন্য)fs; abort(); } যখন (0)
#অন্যথা
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_বাফার_BUG_CPP() করো { \
    CV_DbgAssert(ptr); \
    যদি((ptr)[0] == 0 && (ptr) == fs->বাফার_End() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} যখন (0)


শ্রেণী FileStorageParser;
শ্রেণী FileStorageEmitter;

কাঠামো FStruct_তথ্য
{
    FStruct_তথ্য() { indent = flags = 0; }
    FStruct_তথ্য( ধ্রুবক std::string& _struct_tag,
                 পূর্ণসংখ্যা _struct_flags, পূর্ণসংখ্যা _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    পূর্ণসংখ্যা flags;
    পূর্ণসংখ্যা indent;
};

শ্রেণী FileStorage_API
{
সর্বজনীন:
    আভাসী ~FileStorage_API();
    আভাসী FileStorage* getFS() = 0;

    আভাসী পূর্ণসংখ্যা getFormat() ধ্রুবক = 0;

    আভাসী শূন্য puts( ধ্রুবক অক্ষর* str ) = 0;
    আভাসী অক্ষর* gets() = 0;
    আভাসী যৌক্তিক eof() = 0;
    আভাসী শূন্য setEof() = 0;
    আভাসী শূন্য closeFile() = 0;
    আভাসী শূন্য rewind() = 0;
    আভাসী অক্ষর* resize_Write_বাফার( অক্ষর* ptr, পূর্ণসংখ্যা len ) = 0;
    আভাসী অক্ষর* বাফার_Ptr() ধ্রুবক = 0;
    আভাসী অক্ষর* বাফার_Start() ধ্রুবক = 0;
    আভাসী অক্ষর* বাফার_End() ধ্রুবক = 0;
    আভাসী শূন্য set_বাফার_Ptr(অক্ষর* ptr) = 0;
    আভাসী অক্ষর* flush() = 0;
    আভাসী শূন্য setNonEmpty() = 0;
    আভাসী পূর্ণসংখ্যা wrapMargin() ধ্রুবক = 0;

    আভাসী FStruct_তথ্য& getCurrentStruct() = 0;

    আভাসী শূন্য convertToCollection( পূর্ণসংখ্যা type, File_গ্রন্থি& গ্রন্থি ) = 0;
    আভাসী File_গ্রন্থি add_গ্রন্থি( File_গ্রন্থি& collection, ধ্রুবক std::string& চাবি,
                               পূর্ণসংখ্যা type, ধ্রুবক শূন্য* value=0, পূর্ণসংখ্যা len=-1 ) = 0;
    আভাসী শূন্য finalizeCollection( File_গ্রন্থি& collection ) = 0;
    আভাসী দ্বিগুণ strtod(অক্ষর* ptr, অক্ষর** endptr) = 0;

    আভাসী অক্ষর* parseBase64(অক্ষর* ptr, পূর্ণসংখ্যা indent, File_গ্রন্থি& collection) = 0;
    CV_NORETURN
    আভাসী শূন্য parseError(ধ্রুবক অক্ষর* funcname, ধ্রুবক std::string& বার্তা,
                            ধ্রুবক অক্ষর* filename, পূর্ণসংখ্যা lineno) = 0;

ব্যক্তিগত:
    গণনা Base64State{
        Uncertain,
        NotUse,
        InUse,
    };

    friend শ্রেণী cv::FileStorage::Impl;
    friend শ্রেণী cv::FileStorage;
    friend শ্রেণী JSONEmitter;
    friend শ্রেণী XMLEmitter;
    friend শ্রেণী YAMLEmitter;

    আভাসী শূন্য check_if_write_struct_is_delayed(যৌক্তিক change_type_to_base64 = মিথ্যা) = 0;
    আভাসী শূন্য switch_to_Base64_state(Base64State state) = 0;
    আভাসী Base64State get_state_of_writing_base64() = 0;
    আভাসী পূর্ণসংখ্যা get_space() = 0;
};

শ্রেণী FileStorageEmitter
{
সর্বজনীন:
    আভাসী ~FileStorageEmitter() {}

    আভাসী FStruct_তথ্য startWriteStruct( ধ্রুবক FStruct_তথ্য& parent, ধ্রুবক অক্ষর* চাবি,
                                          পূর্ণসংখ্যা struct_flags, ধ্রুবক অক্ষর* type_name=0 ) = 0;
    আভাসী শূন্য endWriteStruct(ধ্রুবক FStruct_তথ্য& current_struct) = 0;
    আভাসী শূন্য লিখ(ধ্রুবক অক্ষর* চাবি, পূর্ণসংখ্যা value) = 0;
    আভাসী শূন্য লিখ(ধ্রুবক অক্ষর* চাবি, int64_t value) = 0;
    আভাসী শূন্য লিখ(ধ্রুবক অক্ষর* চাবি, দ্বিগুণ value) = 0;
    আভাসী শূন্য লিখ(ধ্রুবক অক্ষর* চাবি, ধ্রুবক অক্ষর* value, যৌক্তিক quote) = 0;
    আভাসী শূন্য writeScalar(ধ্রুবক অক্ষর* চাবি, ধ্রুবক অক্ষর* value) = 0;
    আভাসী শূন্য writeComment(ধ্রুবক অক্ষর* comment, যৌক্তিক eol_comment) = 0;
    আভাসী শূন্য start_পরবর্তী_Stream() = 0;
};

শ্রেণী FileStorageParser
{
সর্বজনীন:
    আভাসী ~FileStorageParser() {}
    আভাসী যৌক্তিক parse(অক্ষর* ptr) = 0;
    আভাসী যৌক্তিক getBase64Row(অক্ষর* ptr, পূর্ণসংখ্যা indent, অক্ষর* &beg, অক্ষর* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
