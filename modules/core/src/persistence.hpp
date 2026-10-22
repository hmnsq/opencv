// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अगर_अघोषित SRC_PERSISTENCE_HPP
#घोषणा SRC_PERSISTENCE_HPP

#समावेश <deque>
#समावेश <sstream>
#समावेश <string>
#समावेश <iterator>

#घोषणा USE_ZLIB 1
#अगर USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 0
#  endif
#  include <zlib.h>
#अन्यथा
प्रकार व्योम* gzFile;
#पूर्ण_अगर

//=====================================================================================

अटल स्थिर माप_प्रकार PARSER_BASE64_बफर_आकार = 1024U * 1024U / 8U;
अटल स्थिर पूर्णांक CV_PERSISTENCE_MAX_DEPTH = 1024;

namespace base64 {

क्रमागत
{
    HEADER_आकार         = 24,
    ENCODED_HEADER_आकार = 32
};

} // base64::

//=====================================================================================

#घोषणा CV_FS_MAX_LEN 4096
#घोषणा CV_FS_MAX_FMT_PAIRS  128

/****************************************************************************************\
*                            Common macros and type definitions                          *
\****************************************************************************************/

#घोषणा cv_isprint(c)     ((uchar)(c) >= (uchar)' ')
#घोषणा cv_isprint_or_tab(c)  ((uchar)(c) >= (uchar)' ' || (c) == '\t')

अंतरभूत bool cv_isalnum(अक्षर c)
{
    वापस ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

अंतरभूत bool cv_isalpha(अक्षर c)
{
    वापस ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

अंतरभूत bool cv_isdigit(अक्षर c)
{
    वापस '0' <= c && c <= '9';
}

अंतरभूत bool cv_isspace(अक्षर c)
{
    वापस (9 <= c && c <= 13) || c == ' ';
}

अंतरभूत अक्षर* cv_skip_BOM(अक्षर* ptr)
{
    अगर((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      वापस ptr + 3;
    }
    वापस ptr;
}

namespace cv
{
namespace fs
{
पूर्णांक strcasecmp(स्थिर अक्षर* str1, स्थिर अक्षर* str2);
अक्षर* itoa( पूर्णांक _val, अक्षर* बफर, पूर्णांक /*radix*/ );
अक्षर* itoa( int64_t _val, अक्षर* बफर, पूर्णांक /*radix*/, bool _signed );
अक्षर* floatToString( अक्षर* buf, माप_प्रकार buf_आकार, भग्न value, bool halfprecision, bool explicitZero );
अक्षर* doubleToString( अक्षर* buf, माप_प्रकार buf_आकार, द्विगुन value, bool explicitZero );

पूर्णांक calc_Struct_आकार( स्थिर अक्षर* dt, पूर्णांक initial_आकार );
पूर्णांक calc_Elem_आकार( स्थिर अक्षर* dt, पूर्णांक initial_आकार );
CV_DEPRECATED अक्षर* encodeFormat( पूर्णांक elem_type, अक्षर* dt );
अक्षर* encodeFormat( पूर्णांक elem_type, अक्षर* dt, माप_प्रकार dt_len );
पूर्णांक decodeFormat( स्थिर अक्षर* dt, पूर्णांक* fmt_pairs, पूर्णांक max_len );
पूर्णांक decodeSimpleFormat( स्थिर अक्षर* dt );
}


#अगर_घोषित CV_STATIC_ANALYSIS
#घोषणा CV_PARSE_ERROR_CPP(errmsg) करो { (व्योम)fs; पात(); } जबतक (0)
#अन्यथा
#घोषणा CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __खाता__, __LINE__ )
#पूर्ण_अगर


#घोषणा CV_PERSISTENCE_CHECK_END_OF_बफर_BUG_CPP() करो { \
    CV_DbgAssert(ptr); \
    अगर((ptr)[0] == 0 && (ptr) == fs->बफर_End() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} जबतक (0)


श्रेणी FileStorageParser;
श्रेणी FileStorageEmitter;

काष्ठा FStruct_तथ्य
{
    FStruct_तथ्य() { indent = flags = 0; }
    FStruct_तथ्य( स्थिर std::string& _struct_tag,
                 पूर्णांक _struct_flags, पूर्णांक _struct_indent )
    {
        tag = _struct_tag;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string tag;
    पूर्णांक flags;
    पूर्णांक indent;
};

श्रेणी FileStorage_API
{
खुला:
    भव ~FileStorage_API();
    भव FileStorage* getFS() = 0;

    भव पूर्णांक getFormat() स्थिर = 0;

    भव व्योम माला_दो( स्थिर अक्षर* str ) = 0;
    भव अक्षर* माला_लो() = 0;
    भव bool खप() = 0;
    भव व्योम setEof() = 0;
    भव व्योम closeFile() = 0;
    भव व्योम शुरुआत() = 0;
    भव अक्षर* resize_Write_बफर( अक्षर* ptr, पूर्णांक len ) = 0;
    भव अक्षर* बफर_Ptr() स्थिर = 0;
    भव अक्षर* बफर_Start() स्थिर = 0;
    भव अक्षर* बफर_End() स्थिर = 0;
    भव व्योम set_बफर_Ptr(अक्षर* ptr) = 0;
    भव अक्षर* सफाई() = 0;
    भव व्योम setNonEmpty() = 0;
    भव पूर्णांक wrapMargin() स्थिर = 0;

    भव FStruct_तथ्य& getCurrentStruct() = 0;

    भव व्योम convertToCollection( पूर्णांक type, File_गाँठ& गाँठ ) = 0;
    भव File_गाँठ add_गाँठ( File_गाँठ& collection, स्थिर std::string& कुंजी,
                               पूर्णांक type, स्थिर व्योम* value=0, पूर्णांक len=-1 ) = 0;
    भव व्योम finalizeCollection( File_गाँठ& collection ) = 0;
    भव द्विगुन म_से_भग्न(अक्षर* ptr, अक्षर** endptr) = 0;

    भव अक्षर* parseBase64(अक्षर* ptr, पूर्णांक indent, File_गाँठ& collection) = 0;
    CV_NORETURN
    भव व्योम parseError(स्थिर अक्षर* funcname, स्थिर std::string& संदेश,
                            स्थिर अक्षर* filename, पूर्णांक lineno) = 0;

निजी:
    क्रमागत Base64State{
        Uncertain,
        NotUse,
        InUse,
    };

    मित्र श्रेणी cv::FileStorage::Impl;
    मित्र श्रेणी cv::FileStorage;
    मित्र श्रेणी JSONEmitter;
    मित्र श्रेणी XMLEmitter;
    मित्र श्रेणी YAMLEmitter;

    भव व्योम check_if_write_struct_is_delayed(bool change_type_to_base64 = false) = 0;
    भव व्योम switch_to_Base64_state(Base64State state) = 0;
    भव Base64State get_state_of_writing_base64() = 0;
    भव पूर्णांक get_space() = 0;
};

श्रेणी FileStorageEmitter
{
खुला:
    भव ~FileStorageEmitter() {}

    भव FStruct_तथ्य startWriteStruct( स्थिर FStruct_तथ्य& parent, स्थिर अक्षर* कुंजी,
                                          पूर्णांक struct_flags, स्थिर अक्षर* type_name=0 ) = 0;
    भव व्योम endWriteStruct(स्थिर FStruct_तथ्य& current_struct) = 0;
    भव व्योम ग_लिखो(स्थिर अक्षर* कुंजी, पूर्णांक value) = 0;
    भव व्योम ग_लिखो(स्थिर अक्षर* कुंजी, int64_t value) = 0;
    भव व्योम ग_लिखो(स्थिर अक्षर* कुंजी, द्विगुन value) = 0;
    भव व्योम ग_लिखो(स्थिर अक्षर* कुंजी, स्थिर अक्षर* value, bool quote) = 0;
    भव व्योम writeScalar(स्थिर अक्षर* कुंजी, स्थिर अक्षर* value) = 0;
    भव व्योम writeComment(स्थिर अक्षर* comment, bool eol_comment) = 0;
    भव व्योम start_अग्र_Stream() = 0;
};

श्रेणी FileStorageParser
{
खुला:
    भव ~FileStorageParser() {}
    भव bool parse(अक्षर* ptr) = 0;
    भव bool getBase64Row(अक्षर* ptr, पूर्णांक indent, अक्षर* &beg, अक्षर* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#पूर्ण_अगर // SRC_PERSISTENCE_HPP
