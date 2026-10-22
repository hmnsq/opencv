// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_PERSISTENCE_HPP
#define SRC_PERSISTENCE_HPP

#அந்தர்பாவ <deque>
#அந்தர்பாவ <sstream>
#அந்தர்பாவ <string>
#அந்தர்பாவ <iterator>

#define USE_ZLIB 1
#எனில் USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#இல்லையெனில்
வகைவரை வெறுமை* gzFile;
#endif

//=====================================================================================

நிலையான மாறிலி size_t PARSER_BASE64_BUFFER_SIZE = 1024U * 1024U / 8U;
நிலையான மாறிலி முழு CV_PERSISTENCE_MAX_DEPTH = 1024;

நாமாகாஶ base64 {

எண்ணிக்கை
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

உள்வரி தருக்க cv_isalnum(எழுத்து c)
{
    திரும்பு ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

உள்வரி தருக்க cv_isalpha(எழுத்து c)
{
    திரும்பு ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

உள்வரி தருக்க cv_isdigit(எழுத்து c)
{
    திரும்பு '0' <= c && c <= '9';
}

உள்வரி தருக்க cv_isspace(எழுத்து c)
{
    திரும்பு (9 <= c && c <= 13) || c == ' ';
}

உள்வரி எழுத்து* cv_skip_BOM(எழுத்து* ptr)
{
    எனில்((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      திரும்பு ptr + 3;
    }
    திரும்பு ptr;
}

நாமாகாஶ cv
{
நாமாகாஶ fs
{
முழு strcasecmp(மாறிலி எழுத்து* str1, மாறிலி எழுத்து* str2);
எழுத்து* itoa( முழு _val, எழுத்து* buffer, முழு /*radix*/ );
எழுத்து* itoa( int64_t _val, எழுத்து* buffer, முழு /*radix*/, தருக்க _signed );
எழுத்து* floatToString( எழுத்து* buf, size_t bufSize, தசம value, தருக்க halfprecision, தருக்க explicitZero );
எழுத்து* doubleToString( எழுத்து* buf, size_t bufSize, இரட்டை value, தருக்க explicitZero );

முழு calcStructSize( மாறிலி எழுத்து* dt, முழு initial_size );
முழு calcElemSize( மாறிலி எழுத்து* dt, முழு initial_size );
CV_DEPRECATED எழுத்து* encodeFormat( முழு elem_type, எழுத்து* dt );
எழுத்து* encodeFormat( முழு elem_type, எழுத்து* dt, size_t dt_len );
முழு decodeFormat( மாறிலி எழுத்து* dt, முழு* fmt_pairs, முழு max_len );
முழு decodeSimpleFormat( மாறிலி எழுத்து* dt );
}


#ifdef CV_STATIC_ANALYSIS
#define CV_PARSE_ERROR_CPP(errmsg) செய் { (வெறுமை)fs; abort(); } போது (0)
#இல்லையெனில்
#define CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#endif


#define CV_PERSISTENCE_CHECK_END_OF_BUFFER_BUG_CPP() செய் { \
    CV_DbgAssert(ptr); \
    எனில்((ptr)[0] == 0 && (ptr) == fs->bufferEnd() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} போது (0)


வகுப்பு FileStorageParser;
வகுப்பு FileStorageEmitter;

கட்டமைப்பு FStructData
{
    FStructData() { indent = flags = 0; }
    FStructData( மாறிலி std::string& _struct_tag,
                 முழு _struct_flags, முழு _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    முழு flags;
    முழு indent;
};

வகுப்பு FileStorage_API
{
பொது:
    மெய்நிகர் ~FileStorage_API();
    மெய்நிகர் FileStorage* getFS() = 0;

    மெய்நிகர் முழு getFormat() மாறிலி = 0;

    மெய்நிகர் வெறுமை puts( மாறிலி எழுத்து* str ) = 0;
    மெய்நிகர் எழுத்து* gets() = 0;
    மெய்நிகர் தருக்க eof() = 0;
    மெய்நிகர் வெறுமை setEof() = 0;
    மெய்நிகர் வெறுமை closeFile() = 0;
    மெய்நிகர் வெறுமை rewind() = 0;
    மெய்நிகர் எழுத்து* resizeWriteBuffer( எழுத்து* ptr, முழு len ) = 0;
    மெய்நிகர் எழுத்து* bufferPtr() மாறிலி = 0;
    மெய்நிகர் எழுத்து* bufferStart() மாறிலி = 0;
    மெய்நிகர் எழுத்து* bufferEnd() மாறிலி = 0;
    மெய்நிகர் வெறுமை setBufferPtr(எழுத்து* ptr) = 0;
    மெய்நிகர் எழுத்து* flush() = 0;
    மெய்நிகர் வெறுமை setNonEmpty() = 0;
    மெய்நிகர் முழு wrapMargin() மாறிலி = 0;

    மெய்நிகர் FStructData& getCurrentStruct() = 0;

    மெய்நிகர் வெறுமை convertToCollection( முழு type, File_முனை& முனை ) = 0;
    மெய்நிகர் File_முனை add_முனை( File_முனை& collection, மாறிலி std::string& சாவி,
                               முழு type, மாறிலி வெறுமை* value=0, முழு len=-1 ) = 0;
    மெய்நிகர் வெறுமை finalizeCollection( File_முனை& collection ) = 0;
    மெய்நிகர் இரட்டை strtod(எழுத்து* ptr, எழுத்து** endptr) = 0;

    மெய்நிகர் எழுத்து* parseBase64(எழுத்து* ptr, முழு indent, File_முனை& collection) = 0;
    CV_NORETURN
    மெய்நிகர் வெறுமை parseError(மாறிலி எழுத்து* funcname, மாறிலி std::string& செய்தி,
                            மாறிலி எழுத்து* filename, முழு lineno) = 0;

தனிப்பட்ட:
    எண்ணிக்கை Base64State{
        Uncertain,
        NotUse,
        InUse,
    };

    friend வகுப்பு cv::FileStorage::Impl;
    friend வகுப்பு cv::FileStorage;
    friend வகுப்பு JSONEmitter;
    friend வகுப்பு XMLEmitter;
    friend வகுப்பு YAMLEmitter;

    மெய்நிகர் வெறுமை check_if_write_struct_is_delayed(தருக்க change_type_to_base64 = பொய்) = 0;
    மெய்நிகர் வெறுமை switch_to_Base64_state(Base64State state) = 0;
    மெய்நிகர் Base64State get_state_of_writing_base64() = 0;
    மெய்நிகர் முழு get_space() = 0;
};

வகுப்பு FileStorageEmitter
{
பொது:
    மெய்நிகர் ~FileStorageEmitter() {}

    மெய்நிகர் FStructData startWriteStruct( மாறிலி FStructData& parent, மாறிலி எழுத்து* சாவி,
                                          முழு struct_flags, மாறிலி எழுத்து* type_name=0 ) = 0;
    மெய்நிகர் வெறுமை endWriteStruct(மாறிலி FStructData& current_struct) = 0;
    மெய்நிகர் வெறுமை லிக(மாறிலி எழுத்து* சாவி, முழு value) = 0;
    மெய்நிகர் வெறுமை லிக(மாறிலி எழுத்து* சாவி, int64_t value) = 0;
    மெய்நிகர் வெறுமை லிக(மாறிலி எழுத்து* சாவி, இரட்டை value) = 0;
    மெய்நிகர் வெறுமை லிக(மாறிலி எழுத்து* சாவி, மாறிலி எழுத்து* value, தருக்க quote) = 0;
    மெய்நிகர் வெறுமை writeScalar(மாறிலி எழுத்து* சாவி, மாறிலி எழுத்து* value) = 0;
    மெய்நிகர் வெறுமை writeComment(மாறிலி எழுத்து* comment, தருக்க eol_comment) = 0;
    மெய்நிகர் வெறுமை startNextStream() = 0;
};

வகுப்பு FileStorageParser
{
பொது:
    மெய்நிகர் ~FileStorageParser() {}
    மெய்நிகர் தருக்க parse(எழுத்து* ptr) = 0;
    மெய்நிகர் தருக்க getBase64Row(எழுத்து* ptr, முழு indent, எழுத்து* &beg, எழுத்து* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#endif // SRC_PERSISTENCE_HPP
