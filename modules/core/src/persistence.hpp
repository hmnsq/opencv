// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_PERSISTENCE_HPP
#define SRC_PERSISTENCE_HPP

#અન્તર્ભાવ <deque>
#અન્તર્ભાવ <sstream>
#અન્તર્ભાવ <string>
#અન્તર્ભાવ <iterator>

#define USE_ZLIB 1
#જો USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#નહીંતર
પ્રકારવ્યાખ્યા શૂન્ય* gzFile;
#endif

//=====================================================================================

સ્થિર અચલ size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
સ્થિર અચલ પૂર્ણાંક CV_PERSISTENCE_MAX_DEPTH = 1024;

નામાકાશ base64 {

ગણના
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

અંતઃરેખ તાર્કિક cv_isalnum(અક્ષર c)
{
    પાછા ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

અંતઃરેખ તાર્કિક cv_isalpha(અક્ષર c)
{
    પાછા ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

અંતઃરેખ તાર્કિક cv_isdigit(અક્ષર c)
{
    પાછા '0' <= c && c <= '9';
}

અંતઃરેખ તાર્કિક cv_isspace(અક્ષર c)
{
    પાછા (9 <= c && c <= 13) || c == ' ';
}

અંતઃરેખ અક્ષર* cv_skip_BOM(અક્ષર* ptr)
{
    જો((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      પાછા ptr + 3;
    }
    પાછા ptr;
}

નામાકાશ cv
{
નામાકાશ fs
{
પૂર્ણાંક strcasecmp(અચલ અક્ષર* str1, અચલ અક્ષર* str2);
અક્ષર* itoa( પૂર્ણાંક _val, અક્ષર* buffer, પૂર્ણાંક /*radix*/ );
અક્ષર* itoa( int64_t _val, અક્ષર* buffer, પૂર્ણાંક /*radix*/, તાર્કિક _signed );
અક્ષર* floatToString( અક્ષર* buf, size_t bufSize, દશાંશ value, તાર્કિક halfprecision, તાર્કિક explicitZero );
અક્ષર* doubleToString( અક્ષર* buf, size_t bufSize, દ્વિગુણ value, તાર્કિક explicitZero );

પૂર્ણાંક calcStructSize( અચલ અક્ષર* dt, પૂર્ણાંક initial_size );
પૂર્ણાંક calcElemSize( અચલ અક્ષર* dt, પૂર્ણાંક initial_size );
CV_DEPRECATED અક્ષર* encodeFormat( પૂર્ણાંક elem_type, અક્ષર* dt );
અક્ષર* encodeFormat( પૂર્ણાંક elem_type, અક્ષર* dt, size_t dt_len );
પૂર્ણાંક decodeFormat( અચલ અક્ષર* dt, પૂર્ણાંક* fmt_pairs, પૂર્ણાંક max_len );
પૂર્ણાંક decodeSimpleFormat( અચલ અક્ષર* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) કરો { (શૂન્ય)fs; abort(); } જ્યારે (0)
#નહીંતર
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() કરો { \
    CV_DbgAssert(ptr); \
    જો((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} જ્યારે (0)


વર્ગ FileStorageParser;
વર્ગ FileStorageEmitter;

સંરચના FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( અચલ std::string& _struct_tag,
                 પૂર્ણાંક _struct_flags, પૂર્ણાંક _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    પૂર્ણાંક flags;
    પૂર્ણાંક indent;
};

વર્ગ FileStorage_API
{
જાહેર:
    આભાસી ~FileStorage_API();
    આભાસી FileStorage* getFS() = 0;

    આભાસી પૂર્ણાંક getFormat() અચલ = 0;

    આભાસી શૂન્ય puts( અચલ અક્ષર* str ) = 0;
    આભાસી અક્ષર* gets() = 0;
    આભાસી તાર્કિક eof() = 0;
    આભાસી શૂન્ય setEof() = 0;
    આભાસી શૂન્ય closeFile() = 0;
    આભાસી શૂન્ય rewind() = 0;
    આભાસી અક્ષર* resizeWriteBuffer( અક્ષર* ptr, પૂર્ણાંક len ) = 0;
    આભાસી અક્ષર* bufferPtr() અચલ = 0;
    આભાસી અક્ષર* bufferStart() અચલ = 0;
    આભાસી અક્ષર* bufferEnd() અચલ = 0;
    આભાસી શૂન્ય setBufferPtr(અક્ષર* ptr) = 0;
    આભાસી અક્ષર* flush() = 0;
    આભાસી શૂન્ય setNonEmpty() = 0;
    આભાસી પૂર્ણાંક wrapMargin() અચલ = 0;

    આભાસી FStructData& getCurrentStruct() = 0;

    આભાસી શૂન્ય convertToCollection( પૂર્ણાંક type, File_ગાંઠ& ગાંઠ ) = 0;
    આભાસી File_ગાંઠ add_ગાંઠ( File_ગાંઠ& collection, અચલ std::string& ચાવી,
                               પૂર્ણાંક type, અચલ શૂન્ય* value=0, પૂર્ણાંક len=-1 ) = 0;
    આભાસી શૂન્ય finalizeCollection( File_ગાંઠ& collection ) = 0;
    આભાસી દ્વિગુણ strtod(અક્ષર* ptr, અક્ષર** endptr) = 0;

    આભાસી અક્ષર* parseBase64(અક્ષર* ptr, પૂર્ણાંક indent, File_ગાંઠ& collection) = 0;
    CV_NORETURN
    આભાસી શૂન્ય parseError(અચલ અક્ષર* funcname, અચલ std::string& સંદેશ,
                            અચલ અક્ષર* filename, પૂર્ણાંક lineno) = 0;

ખાનગી:
    ગણના Base64State{
        Uncertain,
        NotUse,
        InUse,
    };

    friend વર્ગ cv::FileStorage::Impl;
    friend વર્ગ cv::FileStorage;
    friend વર્ગ JSONEmitter;
    friend વર્ગ XMLEmitter;
    friend વર્ગ YAMLEmitter;

    આભાસી શૂન્ય check_if_write_struct_is_delayed(તાર્કિક change_type_to_base64 = ખોટું) = 0;
    આભાસી શૂન્ય switch_to_Base64_state(Base64State state) = 0;
    આભાસી Base64State get_state_of_writing_base64() = 0;
    આભાસી પૂર્ણાંક get_space() = 0;
};

વર્ગ FileStorageEmitter
{
જાહેર:
    આભાસી ~FileStorageEmitter() {}

    આભાસી FStructData startWriteStruct( અચલ FStructData& parent, અચલ અક્ષર* ચાવી,
                                          પૂર્ણાંક struct_flags, અચલ અક્ષર* type_name=0 ) = 0;
    આભાસી શૂન્ય endWriteStruct(અચલ FStructData& current_struct) = 0;
    આભાસી શૂન્ય લિખ(અચલ અક્ષર* ચાવી, પૂર્ણાંક value) = 0;
    આભાસી શૂન્ય લિખ(અચલ અક્ષર* ચાવી, int64_t value) = 0;
    આભાસી શૂન્ય લિખ(અચલ અક્ષર* ચાવી, દ્વિગુણ value) = 0;
    આભાસી શૂન્ય લિખ(અચલ અક્ષર* ચાવી, અચલ અક્ષર* value, તાર્કિક quote) = 0;
    આભાસી શૂન્ય writeScalar(અચલ અક્ષર* ચાવી, અચલ અક્ષર* value) = 0;
    આભાસી શૂન્ય writeComment(અચલ અક્ષર* comment, તાર્કિક eol_comment) = 0;
    આભાસી શૂન્ય startNextStream() = 0;
};

વર્ગ FileStorageParser
{
જાહેર:
    આભાસી ~FileStorageParser() {}
    આભાસી તાર્કિક parse(અક્ષર* ptr) = 0;
    આભાસી તાર્કિક getBase64Row(અક્ષર* ptr, પૂર્ણાંક indent, અક્ષર* &beg, અક્ષર* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
