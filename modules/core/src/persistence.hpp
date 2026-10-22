// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_PERSISTENCE_HPP
#define SRC_PERSISTENCE_HPP

#ଅନ୍ତର୍ଭାଵ <deque>
#ଅନ୍ତର୍ଭାଵ <sstream>
#ଅନ୍ତର୍ଭାଵ <string>
#ଅନ୍ତର୍ଭାଵ <iterator>

#define USE_ZLIB 1
#ଯଦି USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#ଅନ୍ୟଥା
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ* gzFile;
#endif

//=====================================================================================

ସ୍ଥିର ଅଚଳ size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
ସ୍ଥିର ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା CV_PERSISTENCE_MAX_DEPTH = 1024;

ନାମାକାଶ base64 {

ଗଣନା
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

ଧାଡ଼ିଭିତରେ ତାର୍କିକ cv_isalnum(ଅକ୍ଷର c)
{
    ଫେରନ୍ତୁ ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

ଧାଡ଼ିଭିତରେ ତାର୍କିକ cv_isalpha(ଅକ୍ଷର c)
{
    ଫେରନ୍ତୁ ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

ଧାଡ଼ିଭିତରେ ତାର୍କିକ cv_isdigit(ଅକ୍ଷର c)
{
    ଫେରନ୍ତୁ '0' <= c && c <= '9';
}

ଧାଡ଼ିଭିତରେ ତାର୍କିକ cv_isspace(ଅକ୍ଷର c)
{
    ଫେରନ୍ତୁ (9 <= c && c <= 13) || c == ' ';
}

ଧାଡ଼ିଭିତରେ ଅକ୍ଷର* cv_skip_BOM(ଅକ୍ଷର* ptr)
{
    ଯଦି((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      ଫେରନ୍ତୁ ptr + 3;
    }
    ଫେରନ୍ତୁ ptr;
}

ନାମାକାଶ cv
{
ନାମାକାଶ fs
{
ପୂର୍ଣ୍ଣସଂଖ୍ୟା strcasecmp(ଅଚଳ ଅକ୍ଷର* str1, ଅଚଳ ଅକ୍ଷର* str2);
ଅକ୍ଷର* itoa( ପୂର୍ଣ୍ଣସଂଖ୍ୟା _val, ଅକ୍ଷର* buffer, ପୂର୍ଣ୍ଣସଂଖ୍ୟା /*radix*/ );
ଅକ୍ଷର* itoa( int64_t _val, ଅକ୍ଷର* buffer, ପୂର୍ଣ୍ଣସଂଖ୍ୟା /*radix*/, ତାର୍କିକ _signed );
ଅକ୍ଷର* floatToString( ଅକ୍ଷର* buf, size_t bufSize, ଦଶମିକ value, ତାର୍କିକ halfprecision, ତାର୍କିକ explicitZero );
ଅକ୍ଷର* doubleToString( ଅକ୍ଷର* buf, size_t bufSize, ଦ୍ୱିଗୁଣ value, ତାର୍କିକ explicitZero );

ପୂର୍ଣ୍ଣସଂଖ୍ୟା calcStructSize( ଅଚଳ ଅକ୍ଷର* dt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା initial_size );
ପୂର୍ଣ୍ଣସଂଖ୍ୟା calcElemSize( ଅଚଳ ଅକ୍ଷର* dt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା initial_size );
CV_DEPRECATED ଅକ୍ଷର* encodeFormat( ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_type, ଅକ୍ଷର* dt );
ଅକ୍ଷର* encodeFormat( ପୂର୍ଣ୍ଣସଂଖ୍ୟା elem_type, ଅକ୍ଷର* dt, size_t dt_len );
ପୂର୍ଣ୍ଣସଂଖ୍ୟା decodeFormat( ଅଚଳ ଅକ୍ଷର* dt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* fmt_pairs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_len );
ପୂର୍ଣ୍ଣସଂଖ୍ୟା decodeSimpleFormat( ଅଚଳ ଅକ୍ଷର* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) କର { (ଶୂନ୍ୟ)fs; abort(); } ଯେତେବେଳେ (0)
#ଅନ୍ୟଥା
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() କର { \
    CV_DbgAssert(ptr); \
    ଯଦି((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} ଯେତେବେଳେ (0)


ଶ୍ରେଣୀ FileStorageParser;
ଶ୍ରେଣୀ FileStorageEmitter;

ସଂରଚନା FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( ଅଚଳ std::string& _struct_tag,
                 ପୂର୍ଣ୍ଣସଂଖ୍ୟା _struct_flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା indent;
};

ଶ୍ରେଣୀ FileStorage_API
{
ସର୍ବସାଧାରଣ:
    ଆଭାସୀ ~FileStorage_API();
    ଆଭାସୀ FileStorage* getFS() = 0;

    ଆଭାସୀ ପୂର୍ଣ୍ଣସଂଖ୍ୟା getFormat() ଅଚଳ = 0;

    ଆଭାସୀ ଶୂନ୍ୟ puts( ଅଚଳ ଅକ୍ଷର* str ) = 0;
    ଆଭାସୀ ଅକ୍ଷର* gets() = 0;
    ଆଭାସୀ ତାର୍କିକ eof() = 0;
    ଆଭାସୀ ଶୂନ୍ୟ setEof() = 0;
    ଆଭାସୀ ଶୂନ୍ୟ closeFile() = 0;
    ଆଭାସୀ ଶୂନ୍ୟ rewind() = 0;
    ଆଭାସୀ ଅକ୍ଷର* resizeWriteBuffer( ଅକ୍ଷର* ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len ) = 0;
    ଆଭାସୀ ଅକ୍ଷର* bufferPtr() ଅଚଳ = 0;
    ଆଭାସୀ ଅକ୍ଷର* bufferStart() ଅଚଳ = 0;
    ଆଭାସୀ ଅକ୍ଷର* bufferEnd() ଅଚଳ = 0;
    ଆଭାସୀ ଶୂନ୍ୟ setBufferPtr(ଅକ୍ଷର* ptr) = 0;
    ଆଭାସୀ ଅକ୍ଷର* flush() = 0;
    ଆଭାସୀ ଶୂନ୍ୟ setNonEmpty() = 0;
    ଆଭାସୀ ପୂର୍ଣ୍ଣସଂଖ୍ୟା wrapMargin() ଅଚଳ = 0;

    ଆଭାସୀ FStructData& getCurrentStruct() = 0;

    ଆଭାସୀ ଶୂନ୍ୟ convertToCollection( ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, File_ଗଣ୍ଠି& ଗଣ୍ଠି ) = 0;
    ଆଭାସୀ File_ଗଣ୍ଠି add_ଗଣ୍ଠି( File_ଗଣ୍ଠି& collection, ଅଚଳ std::string& ଚାବି,
                               ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ଅଚଳ ଶୂନ୍ୟ* value=0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len=-1 ) = 0;
    ଆଭାସୀ ଶୂନ୍ୟ finalizeCollection( File_ଗଣ୍ଠି& collection ) = 0;
    ଆଭାସୀ ଦ୍ୱିଗୁଣ strtod(ଅକ୍ଷର* ptr, ଅକ୍ଷର** endptr) = 0;

    ଆଭାସୀ ଅକ୍ଷର* parseBase64(ଅକ୍ଷର* ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା indent, File_ଗଣ୍ଠି& collection) = 0;
    CV_NORETURN
    ଆଭାସୀ ଶୂନ୍ୟ parseError(ଅଚଳ ଅକ୍ଷର* funcname, ଅଚଳ std::string& ସନ୍ଦେଶ,
                            ଅଚଳ ଅକ୍ଷର* filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lineno) = 0;

ବ୍ୟକ୍ତିଗତ:
    ଗଣନା Base64State{
        Uncertain,
        NotUse,
        InUse,
    };

    friend ଶ୍ରେଣୀ cv::FileStorage::Impl;
    friend ଶ୍ରେଣୀ cv::FileStorage;
    friend ଶ୍ରେଣୀ JSONEmitter;
    friend ଶ୍ରେଣୀ XMLEmitter;
    friend ଶ୍ରେଣୀ YAMLEmitter;

    ଆଭାସୀ ଶୂନ୍ୟ check_if_write_struct_is_delayed(ତାର୍କିକ change_type_to_base64 = ମିଥ୍ୟା) = 0;
    ଆଭାସୀ ଶୂନ୍ୟ switch_to_Base64_state(Base64State state) = 0;
    ଆଭାସୀ Base64State get_state_of_writing_base64() = 0;
    ଆଭାସୀ ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_space() = 0;
};

ଶ୍ରେଣୀ FileStorageEmitter
{
ସର୍ବସାଧାରଣ:
    ଆଭାସୀ ~FileStorageEmitter() {}

    ଆଭାସୀ FStructData startWriteStruct( ଅଚଳ FStructData& parent, ଅଚଳ ଅକ୍ଷର* ଚାବି,
                                          ପୂର୍ଣ୍ଣସଂଖ୍ୟା struct_flags, ଅଚଳ ଅକ୍ଷର* type_name=0 ) = 0;
    ଆଭାସୀ ଶୂନ୍ୟ endWriteStruct(ଅଚଳ FStructData& current_struct) = 0;
    ଆଭାସୀ ଶୂନ୍ୟ ଲିଖ(ଅଚଳ ଅକ୍ଷର* ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value) = 0;
    ଆଭାସୀ ଶୂନ୍ୟ ଲିଖ(ଅଚଳ ଅକ୍ଷର* ଚାବି, int64_t value) = 0;
    ଆଭାସୀ ଶୂନ୍ୟ ଲିଖ(ଅଚଳ ଅକ୍ଷର* ଚାବି, ଦ୍ୱିଗୁଣ value) = 0;
    ଆଭାସୀ ଶୂନ୍ୟ ଲିଖ(ଅଚଳ ଅକ୍ଷର* ଚାବି, ଅଚଳ ଅକ୍ଷର* value, ତାର୍କିକ quote) = 0;
    ଆଭାସୀ ଶୂନ୍ୟ writeScalar(ଅଚଳ ଅକ୍ଷର* ଚାବି, ଅଚଳ ଅକ୍ଷର* value) = 0;
    ଆଭାସୀ ଶୂନ୍ୟ writeComment(ଅଚଳ ଅକ୍ଷର* comment, ତାର୍କିକ eol_comment) = 0;
    ଆଭାସୀ ଶୂନ୍ୟ startNextStream() = 0;
};

ଶ୍ରେଣୀ FileStorageParser
{
ସର୍ବସାଧାରଣ:
    ଆଭାସୀ ~FileStorageParser() {}
    ଆଭାସୀ ତାର୍କିକ parse(ଅକ୍ଷର* ptr) = 0;
    ଆଭାସୀ ତାର୍କିକ getBase64Row(ଅକ୍ଷର* ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା indent, ଅକ୍ଷର* &beg, ଅକ୍ଷର* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
