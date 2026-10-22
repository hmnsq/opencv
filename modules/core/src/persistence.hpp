// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_PERSISTENCE_HPP
#define SRC_PERSISTENCE_HPP

#അന്തര്ഭാവ <deque>
#അന്തര്ഭാവ <sstream>
#അന്തര്ഭാവ <string>
#അന്തര്ഭാവ <iterator>

#define USE_ZLIB 1
#എങ്കിൽ USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#അല്ലെങ്കിൽ
തരനിർവചനം ശൂന്യം* gzFile;
#endif

//=====================================================================================

സ്ഥിതം സ്ഥിരം size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ CV_PERSISTENCE_MAX_DEPTH = 1024;

നാമാകാശ base64 {

എണ്ണൽ
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

വരിയിൽ തർക്കം cv_isalnum(അക്ഷരം c)
{
    തിരികെ ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

വരിയിൽ തർക്കം cv_isalpha(അക്ഷരം c)
{
    തിരികെ ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

വരിയിൽ തർക്കം cv_isdigit(അക്ഷരം c)
{
    തിരികെ '0' <= c && c <= '9';
}

വരിയിൽ തർക്കം cv_isspace(അക്ഷരം c)
{
    തിരികെ (9 <= c && c <= 13) || c == ' ';
}

വരിയിൽ അക്ഷരം* cv_skip_BOM(അക്ഷരം* ptr)
{
    എങ്കിൽ((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      തിരികെ ptr + 3;
    }
    തിരികെ ptr;
}

നാമാകാശ cv
{
നാമാകാശ fs
{
പൂർണ്ണസംഖ്യ strcasecmp(സ്ഥിരം അക്ഷരം* str1, സ്ഥിരം അക്ഷരം* str2);
അക്ഷരം* itoa( പൂർണ്ണസംഖ്യ _val, അക്ഷരം* buffer, പൂർണ്ണസംഖ്യ /*radix*/ );
അക്ഷരം* itoa( int64_t _val, അക്ഷരം* buffer, പൂർണ്ണസംഖ്യ /*radix*/, തർക്കം _signed );
അക്ഷരം* floatToString( അക്ഷരം* buf, size_t bufSize, ദശാംശം value, തർക്കം halfprecision, തർക്കം explicitZero );
അക്ഷരം* doubleToString( അക്ഷരം* buf, size_t bufSize, ഇരട്ട value, തർക്കം explicitZero );

പൂർണ്ണസംഖ്യ calcStructSize( സ്ഥിരം അക്ഷരം* dt, പൂർണ്ണസംഖ്യ initial_size );
പൂർണ്ണസംഖ്യ calcElemSize( സ്ഥിരം അക്ഷരം* dt, പൂർണ്ണസംഖ്യ initial_size );
CV_DEPRECATED അക്ഷരം* encodeFormat( പൂർണ്ണസംഖ്യ elem_type, അക്ഷരം* dt );
അക്ഷരം* encodeFormat( പൂർണ്ണസംഖ്യ elem_type, അക്ഷരം* dt, size_t dt_len );
പൂർണ്ണസംഖ്യ decodeFormat( സ്ഥിരം അക്ഷരം* dt, പൂർണ്ണസംഖ്യ* fmt_pairs, പൂർണ്ണസംഖ്യ max_len );
പൂർണ്ണസംഖ്യ decodeSimpleFormat( സ്ഥിരം അക്ഷരം* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) ചെയ്യുക { (ശൂന്യം)fs; abort(); } ആകുമ്പോൾ (0)
#അല്ലെങ്കിൽ
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() ചെയ്യുക { \
    CV_DbgAssert(ptr); \
    എങ്കിൽ((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} ആകുമ്പോൾ (0)


വർഗ്ഗം FileStorageParser;
വർഗ്ഗം FileStorageEmitter;

ഘടന FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( സ്ഥിരം std::string& _struct_tag,
                 പൂർണ്ണസംഖ്യ _struct_flags, പൂർണ്ണസംഖ്യ _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    പൂർണ്ണസംഖ്യ flags;
    പൂർണ്ണസംഖ്യ indent;
};

വർഗ്ഗം FileStorage_API
{
പൊതു:
    മായാ ~FileStorage_API();
    മായാ FileStorage* getFS() = 0;

    മായാ പൂർണ്ണസംഖ്യ getFormat() സ്ഥിരം = 0;

    മായാ ശൂന്യം puts( സ്ഥിരം അക്ഷരം* str ) = 0;
    മായാ അക്ഷരം* gets() = 0;
    മായാ തർക്കം eof() = 0;
    മായാ ശൂന്യം setEof() = 0;
    മായാ ശൂന്യം closeFile() = 0;
    മായാ ശൂന്യം rewind() = 0;
    മായാ അക്ഷരം* resizeWriteBuffer( അക്ഷരം* ptr, പൂർണ്ണസംഖ്യ len ) = 0;
    മായാ അക്ഷരം* bufferPtr() സ്ഥിരം = 0;
    മായാ അക്ഷരം* bufferStart() സ്ഥിരം = 0;
    മായാ അക്ഷരം* bufferEnd() സ്ഥിരം = 0;
    മായാ ശൂന്യം setBufferPtr(അക്ഷരം* ptr) = 0;
    മായാ അക്ഷരം* flush() = 0;
    മായാ ശൂന്യം setNonEmpty() = 0;
    മായാ പൂർണ്ണസംഖ്യ wrapMargin() സ്ഥിരം = 0;

    മായാ FStructData& getCurrentStruct() = 0;

    മായാ ശൂന്യം convertToCollection( പൂർണ്ണസംഖ്യ type, File_കണ്ണി& കണ്ണി ) = 0;
    മായാ File_കണ്ണി add_കണ്ണി( File_കണ്ണി& collection, സ്ഥിരം std::string& താക്കോൽ,
                               പൂർണ്ണസംഖ്യ type, സ്ഥിരം ശൂന്യം* value=0, പൂർണ്ണസംഖ്യ len=-1 ) = 0;
    മായാ ശൂന്യം finalizeCollection( File_കണ്ണി& collection ) = 0;
    മായാ ഇരട്ട strtod(അക്ഷരം* ptr, അക്ഷരം** endptr) = 0;

    മായാ അക്ഷരം* parseBase64(അക്ഷരം* ptr, പൂർണ്ണസംഖ്യ indent, File_കണ്ണി& collection) = 0;
    CV_NORETURN
    മായാ ശൂന്യം parseError(സ്ഥിരം അക്ഷരം* funcname, സ്ഥിരം std::string& സന്ദേശം,
                            സ്ഥിരം അക്ഷരം* filename, പൂർണ്ണസംഖ്യ lineno) = 0;

സ്വകാര്യ:
    എണ്ണൽ Base64State{
        Uncertain,
        NotUse,
        InUse,
    };

    friend വർഗ്ഗം cv::FileStorage::Impl;
    friend വർഗ്ഗം cv::FileStorage;
    friend വർഗ്ഗം JSONEmitter;
    friend വർഗ്ഗം XMLEmitter;
    friend വർഗ്ഗം YAMLEmitter;

    മായാ ശൂന്യം check_if_write_struct_is_delayed(തർക്കം change_type_to_base64 = അസത്യം) = 0;
    മായാ ശൂന്യം switch_to_Base64_state(Base64State state) = 0;
    മായാ Base64State get_state_of_writing_base64() = 0;
    മായാ പൂർണ്ണസംഖ്യ get_space() = 0;
};

വർഗ്ഗം FileStorageEmitter
{
പൊതു:
    മായാ ~FileStorageEmitter() {}

    മായാ FStructData startWriteStruct( സ്ഥിരം FStructData& parent, സ്ഥിരം അക്ഷരം* താക്കോൽ,
                                          പൂർണ്ണസംഖ്യ struct_flags, സ്ഥിരം അക്ഷരം* type_name=0 ) = 0;
    മായാ ശൂന്യം endWriteStruct(സ്ഥിരം FStructData& current_struct) = 0;
    മായാ ശൂന്യം ലിഖ(സ്ഥിരം അക്ഷരം* താക്കോൽ, പൂർണ്ണസംഖ്യ value) = 0;
    മായാ ശൂന്യം ലിഖ(സ്ഥിരം അക്ഷരം* താക്കോൽ, int64_t value) = 0;
    മായാ ശൂന്യം ലിഖ(സ്ഥിരം അക്ഷരം* താക്കോൽ, ഇരട്ട value) = 0;
    മായാ ശൂന്യം ലിഖ(സ്ഥിരം അക്ഷരം* താക്കോൽ, സ്ഥിരം അക്ഷരം* value, തർക്കം quote) = 0;
    മായാ ശൂന്യം writeScalar(സ്ഥിരം അക്ഷരം* താക്കോൽ, സ്ഥിരം അക്ഷരം* value) = 0;
    മായാ ശൂന്യം writeComment(സ്ഥിരം അക്ഷരം* comment, തർക്കം eol_comment) = 0;
    മായാ ശൂന്യം startNextStream() = 0;
};

വർഗ്ഗം FileStorageParser
{
പൊതു:
    മായാ ~FileStorageParser() {}
    മായാ തർക്കം parse(അക്ഷരം* ptr) = 0;
    മായാ തർക്കം getBase64Row(അക്ഷരം* ptr, പൂർണ്ണസംഖ്യ indent, അക്ഷരം* &beg, അക്ഷരം* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
