// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SRC_PERSISTENCE_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು SRC_PERSISTENCE_HPP

#ಅನ್ತರ್ಭಾವ <deque>
#ಅನ್ತರ್ಭಾವ <sstream>
#ಅನ್ತರ್ಭಾವ <string>
#ಅನ್ತರ್ಭಾವ <iterator>

#ವ್ಯಾಖ್ಯಾನಿಸು USE_ZLIB 1
#ಆದರೆ USE_ZLIB
#  ifndef _LFS64_LARGEFILE
#    define _LFS64_LARGEFILE 0
#  endif
#  ifndef FILE_ಸ್ಥಾನಾಂತರ_ಬಿಟ್ಟುಗಳು
#    define FILE_ಸ್ಥಾನಾಂತರ_ಬಿಟ್ಟುಗಳು 0
#  endif
#  include <zlib.h>
#ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ* gzFile;
#ಆದರೆ_ಅಂತ್ಯ

//=====================================================================================

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ size_t PARSER_BASE64_ಬಫರ್_ಗಾತ್ರ = 1024U * 1024U / 8U;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ CV_PERSISTENCE_MAX_ಆಳ = 1024;

ನಾಮಾಕಾಶ base64 {

ಎಣಿಕೆ
{
    ಶೀರ್ಷಿಕೆ_ಗಾತ್ರ         = 24,
    ENCODED_ಶೀರ್ಷಿಕೆ_ಗಾತ್ರ = 32
};

} // base64::

//=====================================================================================

#ವ್ಯಾಖ್ಯಾನಿಸು CV_FS_MAX_LEN 4096
#ವ್ಯಾಖ್ಯಾನಿಸು CV_FS_MAX_FMT_ಜೋಡಿಗಳು  128

/****************************************************************************************\
*                            Common macros and type definitions                          *
\****************************************************************************************/

#ವ್ಯಾಖ್ಯಾನಿಸು cv_isprint(c)     ((uchar)(c) >= (uchar)' ')
#ವ್ಯಾಖ್ಯಾನಿಸು cv_isprint_or_tab(c)  ((uchar)(c) >= (uchar)' ' || (c) == '\t')

ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ cv_isalnum(ಅಕ್ಷರ c)
{
    ಹಿಂತಿರುಗಿ ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ cv_isalpha(ಅಕ್ಷರ c)
{
    ಹಿಂತಿರುಗಿ ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ cv_isdigit(ಅಕ್ಷರ c)
{
    ಹಿಂತಿರುಗಿ '0' <= c && c <= '9';
}

ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ cv_isspace(ಅಕ್ಷರ c)
{
    ಹಿಂತಿರುಗಿ (9 <= c && c <= 13) || c == ' ';
}

ಸಾಲೊಳಗೆ ಅಕ್ಷರ* cv_skip_BOM(ಅಕ್ಷರ* ptr)
{
    ಆದರೆ((uchar)ptr[0] == 0xef && (uchar)ptr[1] == 0xbb && (uchar)ptr[2] == 0xbf) //UTF-8 BOM
    {
      ಹಿಂತಿರುಗಿ ptr + 3;
    }
    ಹಿಂತಿರುಗಿ ptr;
}

ನಾಮಾಕಾಶ cv
{
ನಾಮಾಕಾಶ fs
{
ಪೂರ್ಣಾಂಕ strcasecmp(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* str1, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* str2);
ಅಕ್ಷರ* itoa( ಪೂರ್ಣಾಂಕ _val, ಅಕ್ಷರ* ಬಫರ್, ಪೂರ್ಣಾಂಕ /*radix*/ );
ಅಕ್ಷರ* itoa( int64_t _val, ಅಕ್ಷರ* ಬಫರ್, ಪೂರ್ಣಾಂಕ /*radix*/, ತಾರ್ಕಿಕ _signed );
ಅಕ್ಷರ* float_ಗಮ್ಯ_String( ಅಕ್ಷರ* buf, size_t buf_ಗಾತ್ರ, ದಶಮಾಂಶ value, ತಾರ್ಕಿಕ halfprecision, ತಾರ್ಕಿಕ explicitZero );
ಅಕ್ಷರ* double_ಗಮ್ಯ_String( ಅಕ್ಷರ* buf, size_t buf_ಗಾತ್ರ, ದ್ವಿಗುಣ value, ತಾರ್ಕಿಕ explicitZero );

ಪೂರ್ಣಾಂಕ calc_Struct_ಗಾತ್ರ( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt, ಪೂರ್ಣಾಂಕ initial_ಗಾತ್ರ );
ಪೂರ್ಣಾಂಕ calc_Elem_ಗಾತ್ರ( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt, ಪೂರ್ಣಾಂಕ initial_ಗಾತ್ರ );
CV_DEPRECATED ಅಕ್ಷರ* encodeFormat( ಪೂರ್ಣಾಂಕ elem_type, ಅಕ್ಷರ* dt );
ಅಕ್ಷರ* encodeFormat( ಪೂರ್ಣಾಂಕ elem_type, ಅಕ್ಷರ* dt, size_t dt_len );
ಪೂರ್ಣಾಂಕ decodeFormat( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt, ಪೂರ್ಣಾಂಕ* fmt_ಜೋಡಿಗಳು, ಪೂರ್ಣಾಂಕ max_len );
ಪೂರ್ಣಾಂಕ decodeSimpleFormat( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* dt );
}


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_STATIC_ANALYSIS
#ವ್ಯಾಖ್ಯಾನಿಸು CV_PARSE_ERROR_CPP(errmsg) ಮಾಡು { (ಶೂನ್ಯ)fs; abort(); } ಆಗ (0)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CV_PARSE_ERROR_CPP( errmsg ) \
    fs->parseError( CV_Func, (errmsg), __FILE__, __LINE__ )
#ಆದರೆ_ಅಂತ್ಯ


#ವ್ಯಾಖ್ಯಾನಿಸು CV_PERSISTENCE_CHECK_ಕೊನೆ_ದ_ಬಫರ್_BUG_CPP() ಮಾಡು { \
    CV_DbgAssert(ptr); \
    ಆದರೆ((ptr)[0] == 0 && (ptr) == fs->ಬಫರ್_ಕೊನೆ_2() - 1) CV_PARSE_ERROR_CPP("OpenCV persistence doesn't support very long lines"); \
} ಆಗ (0)


ವರ್ಗ FileStorageParser;
ವರ್ಗ FileStorageEmitter;

ರಚನೆ FStruct_ದತ್ತಾಂಶ
{
    FStruct_ದತ್ತಾಂಶ() { indent = flags = 0; }
    FStruct_ದತ್ತಾಂಶ( ಸ್ಥಿರಾಂಕ std::string& struct_ಟ್ಯಾಗ್,
                 ಪೂರ್ಣಾಂಕ _struct_flags, ಪೂರ್ಣಾಂಕ _struct_indent )
    {
        ಟ್ಯಾಗ್ = struct_ಟ್ಯಾಗ್;
        flags = _struct_flags;
        indent = _struct_indent;
    }

    std::string ಟ್ಯಾಗ್;
    ಪೂರ್ಣಾಂಕ flags;
    ಪೂರ್ಣಾಂಕ indent;
};

ವರ್ಗ FileStorage_API
{
ಸಾರ್ವಜನಿಕ:
    ವಾಸ್ತವಿಕ ~FileStorage_API();
    ವಾಸ್ತವಿಕ FileStorage* getFS() = 0;

    ವಾಸ್ತವಿಕ ಪೂರ್ಣಾಂಕ getFormat() ಸ್ಥಿರಾಂಕ = 0;

    ವಾಸ್ತವಿಕ ಶೂನ್ಯ puts( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* str ) = 0;
    ವಾಸ್ತವಿಕ ಅಕ್ಷರ* gets() = 0;
    ವಾಸ್ತವಿಕ ತಾರ್ಕಿಕ eof() = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ ಗಣ_Eof() = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ closeFile() = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ rewind() = 0;
    ವಾಸ್ತವಿಕ ಅಕ್ಷರ* resize_Write_ಬಫರ್( ಅಕ್ಷರ* ptr, ಪೂರ್ಣಾಂಕ len ) = 0;
    ವಾಸ್ತವಿಕ ಅಕ್ಷರ* ಬಫರ್_Ptr() ಸ್ಥಿರಾಂಕ = 0;
    ವಾಸ್ತವಿಕ ಅಕ್ಷರ* ಬಫರ್_ಆರಂಭ() ಸ್ಥಿರಾಂಕ = 0;
    ವಾಸ್ತವಿಕ ಅಕ್ಷರ* ಬಫರ್_ಕೊನೆ_2() ಸ್ಥಿರಾಂಕ = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ ಗಣ_ಬಫರ್_Ptr(ಅಕ್ಷರ* ptr) = 0;
    ವಾಸ್ತವಿಕ ಅಕ್ಷರ* flush() = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ ಗಣ_Non_ಖಾಲಿಗಳು() = 0;
    ವಾಸ್ತವಿಕ ಪೂರ್ಣಾಂಕ wrapMargin() ಸ್ಥಿರಾಂಕ = 0;

    ವಾಸ್ತವಿಕ FStruct_ದತ್ತಾಂಶ& getCurrentStruct() = 0;

    ವಾಸ್ತವಿಕ ಶೂನ್ಯ convert_ಗಮ್ಯ_Collection( ಪೂರ್ಣಾಂಕ type, File_ಗಂಟು& ಗಂಟು ) = 0;
    ವಾಸ್ತವಿಕ File_ಗಂಟು ಕೂಡಿಸು_ಗಂಟು( File_ಗಂಟು& collection, ಸ್ಥಿರಾಂಕ std::string& ಕೀಲಿ,
                               ಪೂರ್ಣಾಂಕ type, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* value=0, ಪೂರ್ಣಾಂಕ len=-1 ) = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ finalizeCollection( File_ಗಂಟು& collection ) = 0;
    ವಾಸ್ತವಿಕ ದ್ವಿಗುಣ strtod(ಅಕ್ಷರ* ptr, ಅಕ್ಷರ** endptr) = 0;

    ವಾಸ್ತವಿಕ ಅಕ್ಷರ* parseBase64(ಅಕ್ಷರ* ptr, ಪೂರ್ಣಾಂಕ indent, File_ಗಂಟು& collection) = 0;
    CV_NORETURN
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ parseError(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* funcname, ಸ್ಥಿರಾಂಕ std::string& ಸಂದೇಶ,
                            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* filename, ಪೂರ್ಣಾಂಕ lineno) = 0;

ಖಾಸಗಿ:
    ಎಣಿಕೆ Base64State{
        Uncertain,
        NotUse,
        ಒಳ_Use,
    };

    ಮಿತ್ರ ವರ್ಗ cv::FileStorage::Impl;
    ಮಿತ್ರ ವರ್ಗ cv::FileStorage;
    ಮಿತ್ರ ವರ್ಗ JSONEmitter;
    ಮಿತ್ರ ವರ್ಗ XMLEmitter;
    ಮಿತ್ರ ವರ್ಗ YAMLEmitter;

    ವಾಸ್ತವಿಕ ಶೂನ್ಯ check_if_write_struct_is_delayed(ತಾರ್ಕಿಕ change_type_ಗಮ್ಯ_base64 = ಸುಳ್ಳು) = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ switch_ಗಮ್ಯ_Base64_state(Base64State state) = 0;
    ವಾಸ್ತವಿಕ Base64State get_state_ದ_writing_base64() = 0;
    ವಾಸ್ತವಿಕ ಪೂರ್ಣಾಂಕ get_space() = 0;
};

ವರ್ಗ FileStorageEmitter
{
ಸಾರ್ವಜನಿಕ:
    ವಾಸ್ತವಿಕ ~FileStorageEmitter() {}

    ವಾಸ್ತವಿಕ FStruct_ದತ್ತಾಂಶ ಆರಂಭ_Write_Struct( ಸ್ಥಿರಾಂಕ FStruct_ದತ್ತಾಂಶ& ಪೋಷಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ,
                                          ಪೂರ್ಣಾಂಕ struct_flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* type_name=0 ) = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ ಕೊನೆ_Write_Struct(ಸ್ಥಿರಾಂಕ FStruct_ದತ್ತಾಂಶ& current_struct) = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ ಲಿಖ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಪೂರ್ಣಾಂಕ value) = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ ಲಿಖ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, int64_t value) = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ ಲಿಖ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ದ್ವಿಗುಣ value) = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ ಲಿಖ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* value, ತಾರ್ಕಿಕ quote) = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ writeScalar(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* value) = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ writeComment(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* comment, ತಾರ್ಕಿಕ eol_comment) = 0;
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ ಆರಂಭ_ಮುಂದಿನ_ಹರಿವು() = 0;
};

ವರ್ಗ FileStorageParser
{
ಸಾರ್ವಜನಿಕ:
    ವಾಸ್ತವಿಕ ~FileStorageParser() {}
    ವಾಸ್ತವಿಕ ತಾರ್ಕಿಕ parse(ಅಕ್ಷರ* ptr) = 0;
    ವಾಸ್ತವಿಕ ತಾರ್ಕಿಕ get_Base64_ಅಡ್ಡಸಾಲು(ಅಕ್ಷರ* ptr, ಪೂರ್ಣಾಂಕ indent, ಅಕ್ಷರ* &beg, ಅಕ್ಷರ* &end) = 0;
};

Ptr<FileStorageEmitter> createXMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createYAMLEmitter(FileStorage_API* fs);
Ptr<FileStorageEmitter> createJSONEmitter(FileStorage_API* fs);

Ptr<FileStorageParser> createXMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createYAMLParser(FileStorage_API* fs);
Ptr<FileStorageParser> createJSONParser(FileStorage_API* fs);

}

#ಆದರೆ_ಅಂತ್ಯ // SRC_PERSISTENCE_HPP
